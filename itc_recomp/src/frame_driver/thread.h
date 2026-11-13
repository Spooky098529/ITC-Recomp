// The game's Thread class (vtable 0x8206CB30: the render thread, the worker
// pools' threads, the dirty-disc report) and the XAPI thread wrappers it
// starts its threads with, as plain C++ (docs/functions/frame-driver.md, the
// wrappers in docs/functions/frame-driver.md and app-start-up.md). The kernel's
// threads are the core's: by the user's call (2026-10-29, XAudio's threads;
// 2026-11-06, the predicated tiling's workers) native code calls the kernel
// imports exactly as the original does, through ThreadCalls (guest calls
// made by the adapters, src/hooks/frame_driver.cpp), from frames like
// the originals' (their locals there). The last error is the game's
// (hooks/last_error.h).

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>

namespace frame_driver {

constexpr uint32_t kThreadVtable = 0x8206CB30;
constexpr uint32_t kThreadHandle = 4;     // -1 for none
constexpr uint32_t kThreadObject = 8;     // the object whose slot 1 runs
constexpr uint32_t kThreadProcessor = 12;
constexpr uint32_t kThreadPriority = 16;  // an index into {2, 1, 0, -1, -2}
constexpr uint32_t kThreadStart = 0x8258DE50;      // Thread::Start
constexpr uint32_t kXapiThreadStart = 0x825BA558;  // CreateThread's start routine
constexpr uint32_t kThreadObjectType = 0x820007FC; // [it]: the thread object type

enum class Import {
  kExCreateThread,
  kObReferenceObjectByHandle,
  kKeSetBasePriorityThread,
  kObDereferenceObject,
  kKeSetAffinityThread,
  kNtResumeThread,
};

struct ThreadCalls {
  // A kernel import with r3 onward set to `args`, returning to
  // `return_address`; its r3.
  std::function<uint32_t(Import import, std::initializer_list<uint64_t> args,
                         uint32_t return_address)>
      import;
  // `body(frame)` in a guest frame of `bytes` below the stack pointer (its
  // back chain stored) with the link register `return_address`.
  std::function<void(uint32_t bytes, uint32_t return_address,
                     const std::function<void(uint32_t frame)>& body)>
      in_frame;
  // SetLastErrorFromStatus (sub_82296E88's plain C++) on the calling thread.
  std::function<void(uint32_t status)> set_last_error_from_status;
};

// sub_8258DE88 Thread::Thread: +0 the vtable, +4 = -1, +8 = 0, +12 = 0, +16
// = 2.
void ThreadConstruct(uint8_t* base, uint32_t thread);

// sub_825BA380 CreateThread (with a processor; -1 for none): from a 96-byte
// frame, ExCreateThread(&handle at frame + 80, `stack`, `id`, the XAPI start
// routine, `entry`, `parameter`, the suspended bit (`flags` bit 2) with
// 1 << `processor` in the top byte). The handle, or 0 with the last error
// set from the status when it fails.
uint32_t CreateThread(uint8_t* base, uint32_t stack, uint32_t entry, uint32_t parameter,
                      uint32_t flags, uint32_t processor, uint32_t id,
                      const ThreadCalls& calls);

// sub_822977B0 SetThreadPriority: from a 112-byte frame, the thread object
// (ObReferenceObjectByHandle with [kThreadObjectType]), its base priority
// (15 and -15 as 16 and -16), dereferenced; 1, or 0 with the last error set
// when the handle fails.
uint32_t SetThreadPriority(uint8_t* base, uint32_t handle, int32_t priority,
                           const ThreadCalls& calls);

// sub_82297888 XSetThreadProcessor: from a 112-byte frame, the thread
// object's affinity set to 1 << `processor` (KeSetAffinityThread, the old
// one at frame + 84), dereferenced; the old affinity's highest bit (0 for
// none), or -1 with the last error set (STATUS_INVALID_PARAMETER for a
// processor above 5).
uint32_t XSetThreadProcessor(uint8_t* base, uint32_t handle, uint32_t processor,
                             const ThreadCalls& calls);

// sub_825BA118 ResumeThread: from a 96-byte frame, NtResumeThread(handle,
// &count at frame + 80); the count, or -1 with the last error set.
uint32_t ResumeThread(uint8_t* base, uint32_t handle, const ThreadCalls& calls);

// sub_8258DF18 Thread::Begin: `thread`'s +8 = `object` (`thread` when 0),
// +12 and +16 from the words at `processor` and `priority`; from a 128-byte
// frame, CreateThread (sub_825BA158: no processor) with Thread::Start on
// `thread`, suspended, into +4; unless -1, SetThreadPriority with the
// table {2, 1, 0, -1, -2} (on the frame at +80, read at +80 + 4 x the
// index, as the original), XSetThreadProcessor and ResumeThread. Returns r3
// as the original leaves it (the last call's).
uint32_t ThreadBegin(uint8_t* base, uint32_t thread, uint32_t object, uint32_t processor,
                     uint32_t priority, const ThreadCalls& calls);

}  // namespace frame_driver
