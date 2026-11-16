// The kernel's thread local storage, for the game (E1, step 3:
// docs/rexglue-removal.md, "E1: the plan").
//
// Each thread's slots are words in guest memory at its kernel thread
// object's TLS address (the thread object at [r13 + 256], its +0x68), set up
// when the thread is made. Ours read and write them there, as the SDK does,
// and hand out slot numbers from our own table (lowest free first, 256, as
// the SDK's process bitmap): only KeTlsAlloc hands them out, and all of
// them come here.

#include "threads/native_imports.h"

#include "base/log.h"
#include "guest.h"

#include <bitset>
#include <iterator>
#include <mutex>

namespace threads {

namespace {

constexpr uint32_t kPcrCurrentThread = 256;
constexpr uint32_t kThreadTlsAddress = 0x68;
constexpr uint32_t kSlotCount = 256;
constexpr uint32_t kOutOfIndexes = 0xFFFFFFFF;

std::mutex g_mutex;
std::bitset<kSlotCount> g_used;

uint32_t SlotAddress(uint8_t* base, const lifted::Args& args, uint32_t slot) {
  const uint32_t thread = guest::LoadU32(base, uint32_t(args.r13) + kPcrCurrentThread);
  return guest::LoadU32(base, thread + kThreadTlsAddress) + 4 * slot;
}

// KeTlsAlloc(): a free slot (0 in this thread), or kOutOfIndexes.
lifted::Result TlsAlloc(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeTlsAlloc");
  uint32_t slot = kOutOfIndexes;
  {
    std::lock_guard lock(g_mutex);
    for (uint32_t i = 0; i < kSlotCount; i++) {
      if (!g_used[i]) {
        g_used[i] = true;
        slot = i;
        break;
      }
    }
  }
  if (slot == kOutOfIndexes) {
    ITC_LOG_ERROR("threads: out of TLS slots");
  } else {
    guest::StoreU32(base, SlotAddress(base, args, slot), 0);
  }
  return {slot, 0.0};
}

// KeTlsFree(slot): 1, or 0 for kOutOfIndexes.
lifted::Result TlsFree(uint8_t*, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeTlsFree");
  const uint32_t slot = uint32_t(args.r[0]);
  if (slot == kOutOfIndexes) {
    return {0, 0.0};
  }
  if (slot < kSlotCount) {
    std::lock_guard lock(g_mutex);
    g_used[slot] = false;
  }
  return {1, 0.0};
}

// KeTlsGetValue(slot).
lifted::Result TlsGetValue(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeTlsGetValue");
  return {guest::LoadU32(base, SlotAddress(base, args, uint32_t(args.r[0]))), 0.0};
}

// KeTlsSetValue(slot, value): 1.
lifted::Result TlsSetValue(uint8_t* base, const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeTlsSetValue");
  guest::StoreU32(base, SlotAddress(base, args, uint32_t(args.r[0])), uint32_t(args.r[1]));
  return {1, 0.0};
}

const NativeImportEntry kEntries[] = {
    {__imp__KeTlsAlloc, TlsAlloc},
    {__imp__KeTlsFree, TlsFree},
    {__imp__KeTlsGetValue, TlsGetValue},
    {__imp__KeTlsSetValue, TlsSetValue},
};

const bool kRegistered = RegisterNativeImports(kEntries, std::size(kEntries));

}  // namespace

}  // namespace threads
