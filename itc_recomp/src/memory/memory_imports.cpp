// The kernel's virtual memory imports, for the game (E2's first step,
// docs/rexglue-removal.md), from our allocation layer (memory/heap.h).
//
// In play only the process heap's creation calls them (RtlCreateHeap
// sub_822997B0, from CreateHeap at start-up: the pad replay's two calls):
// a reserve of the heap's size, then a commit of its first pages inside
// it. Plugged in for that: a reserve at no address is pages from our layer
// (committed and cleared already), a commit inside the layer's range has
// nothing left to do; both answer the rounded address and size as the
// kernel does. Anything else (a reserve at an address, a commit outside the
// layer, a free) is logged and, since E2 (no SDK runtime), stops the
// program.

#include "imports/native_imports.h"
#include "memory/heap.h"

#include "base/flags.h"
#include "base/log.h"
#include "guest.h"

#include <iterator>
#include <mutex>
#include <string_view>
#include <unordered_map>

ITC_FLAG_BOOL(native_memory_imports, true, "ITC",
              "Answer the kernel's virtual memory imports the game calls with our "
              "allocation layer (src/memory/memory_imports.cpp, E2 in "
              "docs/rexglue-removal.md) instead of the SDK's; read at start-up");

namespace memory {

namespace {

constexpr uint32_t kStatusSuccess = 0;
constexpr uint32_t kMemCommit = 0x1000;
constexpr uint32_t kMemReserve = 0x2000;
constexpr uint32_t kPageSize = 4096;
constexpr uint32_t kLargePageSize = 65536;

bool Enabled() {
  return ITC_FLAG(native_memory_imports);
}

void LogOnce(const char* what, uint32_t value) {
  static std::mutex mutex;
  static std::unordered_map<std::string_view, bool> logged;
  std::lock_guard lock(mutex);
  if (!logged[what]) {
    logged[what] = true;
    ITC_LOG_INFO("memory imports: {} {:08X} not handled by ours (the SDK's runtime is gone: stopping)", what, value);
  }
}

// NtAllocateVirtualMemory(base in/out, size in/out, type, protection, 0).
lifted::Result AllocateVirtualMemory(uint8_t* base, const lifted::Args& args) {
  const uint32_t base_pointer = uint32_t(args.r[0]);
  const uint32_t size_pointer = uint32_t(args.r[1]);
  const uint32_t type = uint32_t(args.r[2]);
  const uint32_t address = guest::LoadU32(base, base_pointer);
  const uint32_t size = guest::LoadU32(base, size_pointer);
  if ((type & kMemReserve) && address == 0) {
    ITC_NATIVE_RUNNING("import NtAllocateVirtualMemory (reserve)");
    const uint32_t rounded = (size + kLargePageSize - 1) & ~(kLargePageSize - 1);
    const uint32_t pages = AllocPages(rounded, kLargePageSize);
    guest::StoreU32(base, base_pointer, pages);
    guest::StoreU32(base, size_pointer, rounded);
    return {kStatusSuccess, 0.0};
  }
  if ((type & kMemCommit) && !(type & kMemReserve) && address != 0 && InRange(address) &&
      InRange(address + size - 1)) {
    ITC_NATIVE_RUNNING("import NtAllocateVirtualMemory (commit)");
    const uint32_t start = address & ~(kPageSize - 1);
    const uint32_t end = (address + size + kPageSize - 1) & ~(kPageSize - 1);
    guest::StoreU32(base, base_pointer, start);
    guest::StoreU32(base, size_pointer, end - start);
    return {kStatusSuccess, 0.0};
  }
  LogOnce("NtAllocateVirtualMemory of type", type);
  return lifted::ImportFromSdk(base, __imp__NtAllocateVirtualMemory, args);
}

const imports::NativeImportEntry kEntries[] = {
    {__imp__NtAllocateVirtualMemory, AllocateVirtualMemory},
};

const bool kRegistered = imports::RegisterNativeImports(kEntries, std::size(kEntries), Enabled);

}  // namespace

}  // namespace memory
