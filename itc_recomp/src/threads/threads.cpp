// The game's threads as host threads (E1, step 4: docs/rexglue-removal.md,
// "E1: the plan").
//
// ExCreateThread makes a host thread (std::thread) and, in guest memory from
// our allocation layer, only what the game's code still reads through r1
// and r13 (plug in data: the fields read in generated/, nothing else of a
// kernel thread): a stack (r1 at its top; the code keeps its frames there),
// a per-thread block (r13: +256 the thread object, +268 the CPU number,
// +336 0) and a thread object (+0x14C the thread ID; +0x58 and +0x160, the
// last error, start 0; +0x68 the TLS slots, threads/tls.cpp). Then it runs
// the start routine (through the XAPI start-up when one is given) through
// our dispatch table. Its handle is ours (threads/handles.h). Priorities
// are plugged in (kept and answered back, not applied); an affinity sets
// the CPU number the thread reads.
//
// Left to the SDK, logged once: a system thread (creation flag 2), a
// creator whose TLS has per-thread data (__declspec(thread)), and calls on
// threads that aren't ours (the main thread, the SDK's own).

#include "threads/threads.h"
#include "threads/handles.h"
#include "imports/native_imports.h"

#include "base/flags.h"
#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"

#include <algorithm>
#include <bit>
#include <condition_variable>
#include <functional>
#include <iterator>
#include <memory>
#include <mutex>
#include <string_view>
#include <thread>
#include <unordered_map>

ITC_FLAG_BOOL(native_threads, true, "ITC",
              "Answer the kernel's thread, sync and TLS imports with our own code (E1 in "
              "docs/rexglue-removal.md: src/threads/) instead of the SDK's; read at start-up");

namespace threads {

bool Enabled() {
  return ITC_FLAG(native_threads);
}

namespace {

constexpr uint32_t kStatusSuccess = 0;
constexpr uint32_t kStatusInvalidHandle = 0xC0000008;
constexpr uint32_t kStatusInvalidParameter = 0xC000000D;

constexpr uint32_t kCreateSuspended = 1;
constexpr uint32_t kCreateSystemThread = 2;
constexpr uint32_t kCurrentThreadHandle = 0xFFFFFFFE;

// The per-thread block (r13): the words game code reads, +256 the thread
// object, +268 the CPU number (a byte) and +336 (0: SetLastError's check).
constexpr uint32_t kPcrSize = 0x154;
constexpr uint32_t kPcrCurrentThread = 0x100;
constexpr uint32_t kPcrCurrentCpu = 0x10C;
// The SDK's block's TLS data pointer, read from the creator (the main
// thread, the SDK's) only.
constexpr uint32_t kPcrTls = 0x0;

// The thread object: +0x58 (read, 0), +0x68 our TLS slots, +0x14C the
// thread ID, +0x160 the last error (read and written, starts 0).
constexpr uint32_t kThreadSize = 0x164;
constexpr uint32_t kThreadTls = 0x68;
constexpr uint32_t kThreadId = 0x14C;

// TLS slots for each thread (our KeTlsAlloc hands out fewer).
constexpr uint32_t kTlsBytes = 256 * 4;
constexpr uint32_t kMinimumStack = 256 * 1024;

struct Thread {
  uint32_t handle = 0;
  uint32_t object = 0;  // the thread object
  uint32_t pcr = 0;
  uint8_t cpu = 0;
  int32_t priority = 0;
  // Suspensions left before it starts (1 when made suspended).
  uint32_t suspend_count = 0;
  bool closed = false;
};

struct Threads {
  std::mutex mutex;
  std::condition_variable resumed;
  std::unordered_map<uint32_t, std::shared_ptr<Thread>> by_handle;
  std::unordered_map<uint32_t, std::shared_ptr<Thread>> by_object;
  uint32_t next_handle = kThreadHandleBase;
  uint32_t next_id = 0x1000;
  uint8_t next_cpu = 0;
};

thread_local uint32_t t_pcr = 0;
thread_local uint8_t* t_base = nullptr;

Threads& All() {
  static Threads threads;
  return threads;
}

void LogOnce(const char* what, uint32_t value) {
  static std::mutex mutex;
  static std::unordered_map<std::string_view, bool> logged;
  std::lock_guard lock(mutex);
  if (!logged[what]) {
    logged[what] = true;
    ITC_LOG_INFO("threads: {} {:08X} not handled by ours (the SDK's runtime is gone: stopping)", what, value);
  }
}

// The SDK's CPU number for an affinity mask (logical processors): the
// mask's bit, or the next of six in turn for none.
uint8_t CpuFor(Threads& threads, uint8_t mask) {
  if (mask == 0) {
    threads.next_cpu = uint8_t((threads.next_cpu + 1) % 6);
    return threads.next_cpu;
  }
  return uint8_t(7 - std::countl_zero(mask));
}

// A thread of ours by its thread object, or by its handle where a thread
// object is expected (Device::CreateWorkers passes ExCreateThread's handle
// to KeSetBasePriorityThread; the SDK read it as a pointer).
std::shared_ptr<Thread> ByObject(uint32_t object) {
  Threads& threads = All();
  std::lock_guard lock(threads.mutex);
  if (const auto it = threads.by_object.find(object); it != threads.by_object.end()) {
    return it->second;
  }
  const auto it = threads.by_handle.find(object);
  return it == threads.by_handle.end() ? nullptr : it->second;
}

std::shared_ptr<Thread> ByHandle(uint32_t handle) {
  Threads& threads = All();
  std::lock_guard lock(threads.mutex);
  const auto it = threads.by_handle.find(handle);
  return it == threads.by_handle.end() || it->second->closed ? nullptr : it->second;
}

void SetCpu(uint8_t* base, Thread& thread, uint8_t cpu) {
  thread.cpu = cpu;
  guest::StoreU8(base, thread.pcr + kPcrCurrentCpu, cpu);
}

// A thread of ours, made but not started.
struct Made {
  std::shared_ptr<Thread> thread;
  uint32_t stack_base;
  uint32_t id;
};

// Our blocks for a new thread (stack, thread object, per-thread block,
// TLS slots), registered under a new handle; the creation flags' suspend
// bit and CPU mask applied.
Made Make(uint8_t* base, uint32_t stack_size, uint32_t flags) {
  const uint32_t size = std::max(kMinimumStack, (stack_size + 0xFFF) & ~0xFFFu);
  const uint32_t stack_limit = memory::AllocPages(size, 65536);
  const uint32_t object = memory::Alloc(kThreadSize, memory::kHeapZeroMemory);
  const uint32_t pcr = memory::Alloc(kPcrSize, memory::kHeapZeroMemory);
  const uint32_t tls = memory::Alloc(kTlsBytes, memory::kHeapZeroMemory);
  Made made{std::make_shared<Thread>(), stack_limit + size, 0};
  Threads& threads = All();
  std::lock_guard lock(threads.mutex);
  made.thread->handle = threads.next_handle;
  threads.next_handle += 4;
  made.id = threads.next_id;
  threads.next_id += 4;
  made.thread->object = object;
  made.thread->pcr = pcr;
  made.thread->suspend_count = (flags & kCreateSuspended) ? 1 : 0;
  threads.by_handle[made.thread->handle] = made.thread;
  threads.by_object[object] = made.thread;
  guest::StoreU32(base, object + kThreadTls, tls);
  guest::StoreU32(base, object + kThreadId, made.id);
  guest::StoreU32(base, pcr + kPcrCurrentThread, object);
  SetCpu(base, *made.thread, CpuFor(threads, uint8_t(flags >> 24)));
  return made;
}

// The host thread: once resumed, `body` with r1 at the stack's top and r13
// the per-thread block.
std::thread Run(uint8_t* base, const Made& made, std::function<void(lifted::Args)> body) {
  return std::thread([base, made, body = std::move(body)] {
    {
      Threads& threads = All();
      std::unique_lock lock(threads.mutex);
      threads.resumed.wait(lock, [&] { return made.thread->suspend_count == 0; });
    }
    t_pcr = made.thread->pcr;
    t_base = base;
    lifted::Args call;
    call.r1 = made.stack_base;
    call.r13 = made.thread->pcr;
    body(call);
    ITC_LOG_INFO("threads: thread {:08X} ended", made.thread->handle);
  });
}

// ExCreateThread(handle out, stack size, thread ID out, XAPI start-up,
// start routine, start context, creation flags).
lifted::Result CreateThread(uint8_t* base, const lifted::Args& args) {
  const uint32_t handle_out = uint32_t(args.r[0]);
  const uint32_t stack_size = uint32_t(args.r[1]);
  const uint32_t id_out = uint32_t(args.r[2]);
  const uint32_t startup = uint32_t(args.r[3]);
  const uint32_t start = uint32_t(args.r[4]);
  const uint32_t context = uint32_t(args.r[5]);
  const uint32_t flags = uint32_t(args.r[6]);
  const uint32_t creator_pcr = uint32_t(args.r13);
  const uint32_t creator = guest::LoadU32(base, creator_pcr + kPcrCurrentThread);
  if (flags & kCreateSystemThread) {
    LogOnce("ExCreateThread of a system thread, start", start);
    return lifted::ImportFromSdk(base, __imp__ExCreateThread, args);
  }
  // The SDK's threads put per-thread TLS data (__declspec(thread)) before
  // the slots; ours have none to copy, so a creator of the SDK's with some
  // is left to it. A creator of ours had none either.
  if (CurrentPcr() == 0 &&
      guest::LoadU32(base, creator + kThreadTls) != guest::LoadU32(base, creator_pcr + kPcrTls)) {
    LogOnce("ExCreateThread with per-thread TLS data, start", start);
    return lifted::ImportFromSdk(base, __imp__ExCreateThread, args);
  }
  ITC_NATIVE_RUNNING("import ExCreateThread");

  const Made made = Make(base, stack_size, flags);
  if (handle_out) {
    guest::StoreU32(base, handle_out, made.thread->handle);
  }
  if (id_out) {
    guest::StoreU32(base, id_out, made.id);
  }
  ITC_LOG_INFO("threads: thread {:08X} (ID {:X}) made: start {:08X}, context {:08X}{}, CPU {}",
               made.thread->handle, made.id, start, context,
               startup ? " through the XAPI start-up" : "", made.thread->cpu);
  Run(base, made, [base, startup, start, context](lifted::Args call) {
    if (startup) {
      call.r[0] = start;
      call.r[1] = context;
      lifted::Call(base, startup, call);
    } else {
      call.r[0] = context;
      lifted::Call(base, start, call);
    }
  }).detach();
  return {kStatusSuccess, 0.0};
}

// A resume: the previous suspend count.
uint32_t Resume(Thread& thread) {
  Threads& threads = All();
  uint32_t previous;
  {
    std::lock_guard lock(threads.mutex);
    previous = thread.suspend_count;
    if (thread.suspend_count > 0) {
      thread.suspend_count--;
    }
  }
  threads.resumed.notify_all();
  return previous;
}

// NtResumeThread(handle, suspend count out).
lifted::Result NtResumeThread(uint8_t* base, const lifted::Args& args) {
  const uint32_t handle = uint32_t(args.r[0]);
  if (!ThreadHandle(handle)) {
    LogOnce("NtResumeThread on handle", handle);
    return lifted::ImportFromSdk(base, __imp__NtResumeThread, args);
  }
  ITC_NATIVE_RUNNING("import NtResumeThread");
  const std::shared_ptr<Thread> thread = ByHandle(handle);
  if (!thread) {
    return {kStatusInvalidHandle, 0.0};
  }
  const uint32_t previous = Resume(*thread);
  if (const uint32_t out = uint32_t(args.r[1])) {
    guest::StoreU32(base, out, previous);
  }
  return {kStatusSuccess, 0.0};
}

// KeResumeThread(thread object).
lifted::Result KeResumeThread(uint8_t* base, const lifted::Args& args) {
  const uint32_t object = uint32_t(args.r[0]);
  const std::shared_ptr<Thread> thread = ByObject(object);
  if (!thread) {
    LogOnce("KeResumeThread on the thread object at", object);
    return lifted::ImportFromSdk(base, __imp__KeResumeThread, args);
  }
  ITC_NATIVE_RUNNING("import KeResumeThread");
  Resume(*thread);
  return {kStatusSuccess, 0.0};
}

// KeSetBasePriorityThread(thread object, priority): the previous one
// (plugged in: kept, not applied to the host thread).
lifted::Result SetBasePriorityThread(uint8_t* base, const lifted::Args& args) {
  const uint32_t object = uint32_t(args.r[0]);
  const std::shared_ptr<Thread> thread = ByObject(object);
  if (!thread) {
    LogOnce("KeSetBasePriorityThread on the thread object at", object);
    return lifted::ImportFromSdk(base, __imp__KeSetBasePriorityThread, args);
  }
  ITC_NATIVE_RUNNING("import KeSetBasePriorityThread");
  std::lock_guard lock(All().mutex);
  const int32_t previous = thread->priority;
  thread->priority = int32_t(uint32_t(args.r[1]));
  return {uint32_t(previous), 0.0};
}

// KeSetAffinityThread(thread object, affinity, previous affinity out).
lifted::Result SetAffinityThread(uint8_t* base, const lifted::Args& args) {
  const uint32_t object = uint32_t(args.r[0]);
  const uint32_t affinity = uint32_t(args.r[1]);
  const std::shared_ptr<Thread> thread = ByObject(object);
  if (!thread) {
    LogOnce("KeSetAffinityThread on the thread object at", object);
    return lifted::ImportFromSdk(base, __imp__KeSetAffinityThread, args);
  }
  ITC_NATIVE_RUNNING("import KeSetAffinityThread");
  if (affinity == 0) {
    return {kStatusInvalidParameter, 0.0};
  }
  Threads& threads = All();
  std::lock_guard lock(threads.mutex);
  if (const uint32_t out = uint32_t(args.r[2])) {
    guest::StoreU32(base, out, 1u << thread->cpu);
  }
  SetCpu(base, *thread, CpuFor(threads, uint8_t(affinity)));
  return {kStatusSuccess, 0.0};
}

// ObReferenceObjectByHandle(handle, type, object out): a thread's object
// for one of ours (or the current thread's pseudo-handle on one of ours).
lifted::Result ReferenceObjectByHandle(uint8_t* base, const lifted::Args& args) {
  const uint32_t handle = uint32_t(args.r[0]);
  std::shared_ptr<Thread> thread;
  if (handle == kCurrentThreadHandle) {
    thread = ByObject(guest::LoadU32(base, uint32_t(args.r13) + kPcrCurrentThread));
  } else if (ThreadHandle(handle)) {
    thread = ByHandle(handle);
    if (!thread) {
      return {kStatusInvalidHandle, 0.0};
    }
  }
  if (!thread) {
    LogOnce("ObReferenceObjectByHandle on handle", handle);
    return lifted::ImportFromSdk(base, __imp__ObReferenceObjectByHandle, args);
  }
  ITC_NATIVE_RUNNING("import ObReferenceObjectByHandle");
  if (const uint32_t out = uint32_t(args.r[2])) {
    guest::StoreU32(base, out, thread->object);
  }
  return {kStatusSuccess, 0.0};
}

// ObDereferenceObject(object): nothing to do for a thread of ours.
lifted::Result DereferenceObject(uint8_t* base, const lifted::Args& args) {
  const uint32_t object = uint32_t(args.r[0]);
  if (!ByObject(object)) {
    LogOnce("ObDereferenceObject on the object at", object);
    return lifted::ImportFromSdk(base, __imp__ObDereferenceObject, args);
  }
  ITC_NATIVE_RUNNING("import ObDereferenceObject");
  return {0, 0.0};
}

// KeGetCurrentProcessType(): plugged in, 1 (a title's process, X_PROCTYPE_USER,
// as the SDK answers for every thread of the game). Read by RtlCreateHeap
// (sub_822997B0, kept in the heap's +379) and XAudio (sub_8245A828, checks
// for 1).
lifted::Result GetCurrentProcessType(uint8_t*, const lifted::Args&) {
  ITC_NATIVE_RUNNING("import KeGetCurrentProcessType");
  return {1, 0.0};
}

// ExRegisterTitleTerminateNotification(record, register): plugged in, nothing
// kept: the SDK never runs title terminate routines either (XAudio's
// sub_8245B0D0 is the one registered in play).
lifted::Result RegisterTitleTerminateNotification(uint8_t*, const lifted::Args&) {
  ITC_NATIVE_RUNNING("import ExRegisterTitleTerminateNotification");
  return {0, 0.0};
}

const imports::NativeImportEntry kEntries[] = {
    {__imp__ExCreateThread, CreateThread},
    {__imp__NtResumeThread, NtResumeThread},
    {__imp__KeResumeThread, KeResumeThread},
    {__imp__KeSetBasePriorityThread, SetBasePriorityThread},
    {__imp__KeSetAffinityThread, SetAffinityThread},
    {__imp__ObReferenceObjectByHandle, ReferenceObjectByHandle},
    {__imp__ObDereferenceObject, DereferenceObject},
    {__imp__KeGetCurrentProcessType, GetCurrentProcessType},
    {__imp__ExRegisterTitleTerminateNotification, RegisterTitleTerminateNotification},
};

const bool kRegistered = imports::RegisterNativeImports(kEntries, std::size(kEntries), Enabled);

}  // namespace

std::thread StartThread(uint8_t* base, uint32_t stack_size, const char* name,
                        std::function<void(lifted::Args)> body) {
  const Made made = Make(base, stack_size, 0);
  ITC_LOG_INFO("threads: {} {:08X} (ID {:X}) made, CPU {}", name, made.thread->handle, made.id,
               made.thread->cpu);
  return Run(base, made, std::move(body));
}

std::thread StartMainThread(uint8_t* base, uint32_t stack_size, lifted::Entry entry) {
  return StartThread(base, stack_size, "the main thread",
                     [base, entry](lifted::Args call) { entry(base, call); });
}

uint32_t CurrentPcr() {
  return t_pcr;
}

uint32_t CurrentThreadIdIfOurs() {
  if (t_pcr == 0) {
    return 0;
  }
  return guest::LoadU32(t_base, guest::LoadU32(t_base, t_pcr + kPcrCurrentThread) + kThreadId);
}

bool CloseThreadHandle(uint32_t handle) {
  Threads& threads = All();
  std::lock_guard lock(threads.mutex);
  const auto it = threads.by_handle.find(handle);
  if (it == threads.by_handle.end() || it->second->closed) {
    return false;
  }
  // The thread runs on; its handle is gone (its object stays known).
  it->second->closed = true;
  return true;
}

}  // namespace threads
