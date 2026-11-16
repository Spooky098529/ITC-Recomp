// The kernel's critical sections, for the game (E1, step 2:
// docs/rexglue-removal.md, "E1: the plan").
//
// A critical section is the game's 28-byte RTL_CRITICAL_SECTION in guest
// memory: a dispatcher header (+0), the lock count (+0x10, -1 when free),
// the recursion count (+0x14) and the owning thread (+0x18, the owner's
// kernel thread object, [r13 + 256]; 0 when free). Ours is a host lock per
// critical section, found by its guest address, and the struct's fields
// are kept as the SDK keeps them (the depth readable, the user's call of
// 2026-10-29), but for the lock count, written big-endian (the SDK wrote it
// in host order; nothing in the game reads it). The SDK made contended
// waits on the struct as a kernel event; ours wait on the host lock.

#include "kernel/native_imports.h"

#include "base/log.h"
#include "guest.h"

#include <condition_variable>
#include <iterator>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace kernel {

namespace {

constexpr uint32_t kLockCount = 0x10;
constexpr uint32_t kRecursionCount = 0x14;
constexpr uint32_t kOwningThread = 0x18;
constexpr uint32_t kPcrCurrentThread = 256;

struct HostSection {
  std::mutex mutex;
  std::condition_variable released;
  uint32_t owner = 0;  // the owner's kernel thread object; 0 when free
  int32_t depth = 0;
  int32_t lock_count = -1;  // as the guest field: -1 + depth + waiters
};

struct Sections {
  std::shared_mutex mutex;
  std::unordered_map<uint32_t, std::unique_ptr<HostSection>> by_address;
};

Sections& All() {
  static Sections sections;
  return sections;
}

// The host lock for the critical section at `address`, made on first use
// (sections the game set up without RtlInitializeCriticalSection too).
HostSection& Find(uint32_t address) {
  Sections& all = All();
  {
    std::shared_lock lock(all.mutex);
    if (const auto it = all.by_address.find(address); it != all.by_address.end()) {
      return *it->second;
    }
  }
  std::unique_lock lock(all.mutex);
  auto& slot = all.by_address[address];
  if (!slot) {
    slot = std::make_unique<HostSection>();
  }
  return *slot;
}

uint32_t CurrentThread(uint8_t* base, const lifted::Args& args) {
  const uint32_t thread = guest::LoadU32(base, uint32_t(args.r13) + kPcrCurrentThread);
  if (thread == 0) {
    // 0 would read as free: the PCR's address stands in (logged once).
    static bool logged = false;
    if (!logged) {
      logged = true;
      ITC_LOG_ERROR("kernel: a critical section entered on a thread without a thread object "
                    "(r13 {:08X})",
                    uint32_t(args.r13));
    }
    return uint32_t(args.r13);
  }
  return thread;
}

// The guest fields from the host state (under the section's mutex).
void Publish(uint8_t* base, uint32_t address, const HostSection& section) {
  guest::StoreU32(base, address + kLockCount, uint32_t(section.lock_count));
  guest::StoreU32(base, address + kRecursionCount, uint32_t(section.depth));
  guest::StoreU32(base, address + kOwningThread, section.owner);
}

// RtlInitializeCriticalSection(cs): the struct as the SDK sets it up (an
// auto-reset event's header, no spin count), and a fresh host lock.
lifted::Result InitializeCriticalSection(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import RtlInitializeCriticalSection");
  const uint32_t address = uint32_t(args.r[0]);
  guest::StoreU8(base, address + 0, 1);  // type: EventSynchronizationObject
  guest::StoreU8(base, address + 1, 0);  // absolute: the spin count / 256
  guest::StoreU32(base, address + 4, 0);  // signal state
  {
    Sections& all = All();
    std::unique_lock lock(all.mutex);
    all.by_address[address] = std::make_unique<HostSection>();
  }
  HostSection& section = Find(address);
  std::lock_guard lock(section.mutex);
  Publish(base, address, section);
  return {0, 0.0};
}

// RtlEnterCriticalSection(cs).
lifted::Result EnterCriticalSection(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import RtlEnterCriticalSection");
  const uint32_t address = uint32_t(args.r[0]);
  const uint32_t self = CurrentThread(base, args);
  HostSection& section = Find(address);
  std::unique_lock lock(section.mutex);
  section.lock_count++;
  if (section.owner != self) {
    section.released.wait(lock, [&] { return section.owner == 0; });
    section.owner = self;
  }
  section.depth++;
  Publish(base, address, section);
  return {0, 0.0};
}

// RtlTryEnterCriticalSection(cs): 1 when entered, else 0.
lifted::Result TryEnterCriticalSection(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import RtlTryEnterCriticalSection");
  const uint32_t address = uint32_t(args.r[0]);
  const uint32_t self = CurrentThread(base, args);
  HostSection& section = Find(address);
  std::lock_guard lock(section.mutex);
  if (section.owner != 0 && section.owner != self) {
    return {0, 0.0};
  }
  section.owner = self;
  section.lock_count++;
  section.depth++;
  Publish(base, address, section);
  return {1, 0.0};
}

// RtlLeaveCriticalSection(cs).
lifted::Result LeaveCriticalSection(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import RtlLeaveCriticalSection");
  const uint32_t address = uint32_t(args.r[0]);
  HostSection& section = Find(address);
  bool wake = false;
  {
    std::lock_guard lock(section.mutex);
    if (section.depth <= 0) {
      ITC_LOG_ERROR("kernel: critical section {:08X} left while not entered", address);
      return {0, 0.0};
    }
    section.lock_count--;
    if (--section.depth == 0) {
      section.owner = 0;
      wake = section.lock_count != -1;
    }
    Publish(base, address, section);
  }
  if (wake) {
    section.released.notify_one();
  }
  return {0, 0.0};
}

const NativeImportEntry kEntries[] = {
    {__imp__RtlInitializeCriticalSection, InitializeCriticalSection},
    {__imp__RtlEnterCriticalSection, EnterCriticalSection},
    {__imp__RtlTryEnterCriticalSection, TryEnterCriticalSection},
    {__imp__RtlLeaveCriticalSection, LeaveCriticalSection},
};

const bool kRegistered = RegisterNativeImports(kEntries, std::size(kEntries));

}  // namespace

}  // namespace kernel
