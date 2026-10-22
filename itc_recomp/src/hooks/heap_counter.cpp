// TEMPORARY: counts what the process heap and VirtualAlloc hand out, to size
// the native heap's range (docs/native-memory.md, "Still to find"). Each
// override runs the game's own function first and only records the result:
// bytes in use at their peak (heap blocks rounded to 8, pages to 4,096), and
// each flag value seen once. Removed once the numbers are in the doc.

#include "guest.h"

#include "base/log.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <set>
#include <unordered_map>

namespace {

constexpr uint32_t kProcessHeapHandle = 0x8297149C;

std::mutex counter_mutex;
std::unordered_map<uint32_t, uint32_t> heap_blocks;  // address -> rounded size
std::unordered_map<uint32_t, uint32_t> page_blocks;
uint64_t heap_bytes = 0;
uint64_t page_bytes = 0;
uint64_t page_bytes_64k = 0;  // the same pages rounded to 64 KiB, as the SDK does
uint64_t peak_64k = 0;
uint64_t peak_bytes = 0;
uint64_t peak_heap_bytes = 0;
uint64_t peak_page_bytes = 0;
uint64_t next_report = 1 << 20;
uint32_t largest_heap_request = 0;
uint32_t largest_page_request = 0;
std::set<uint64_t> seen;  // kind << 32 | value

// Set while HeapReAlloc runs: its moved path calls HeapAlloc and HeapFree.
thread_local bool in_realloc = false;

enum Seen : uint64_t {
  kHeapAllocFlags = 1,
  kHeapReAllocFlags,
  kHeapFreeFlags,
  kHeapOwnFlags,
  kOtherHeap,
  kVirtualType,
  kVirtualProtect,
  kVirtualAddress,
  kVirtualFreeArgs,
};

// Under counter_mutex.
bool FirstTime(Seen kind, uint32_t value) {
  return seen.insert(uint64_t(kind) << 32 | value).second;
}

void ReportPeak() {
  const uint64_t total = heap_bytes + page_bytes;
  if (total > peak_bytes) {
    peak_bytes = total;
    peak_heap_bytes = heap_bytes;
    peak_page_bytes = page_bytes;
  }
  peak_64k = std::max(peak_64k, heap_bytes + page_bytes_64k);
  if (total < next_report) return;
  next_report = (total / (1 << 20) + 1) << 20;
  ITC_LOG_INFO(
      "heap counter: peak {:.1f} MiB (heap {:.1f} MiB in {} blocks, pages {:.1f} MiB in {} "
      "blocks); largest request heap {} pages {}; with pages at 64 KiB: {:.1f} MiB now, peak {:.1f} MiB",
      total / 1048576.0, heap_bytes / 1048576.0, heap_blocks.size(), page_bytes / 1048576.0,
      page_blocks.size(), largest_heap_request, largest_page_request,
      (heap_bytes + page_bytes_64k) / 1048576.0, peak_64k / 1048576.0);
}

void AddHeap(uint32_t address, uint32_t size) {
  const uint32_t rounded = (std::max(size, 1u) + 7) & ~7u;
  largest_heap_request = std::max(largest_heap_request, size);
  auto [it, added] = heap_blocks.emplace(address, rounded);
  if (!added) {
    heap_bytes -= it->second;
    it->second = rounded;
  }
  heap_bytes += rounded;
  ReportPeak();
}

void RemoveHeap(uint32_t address) {
  auto it = heap_blocks.find(address);
  if (it == heap_blocks.end()) return;
  heap_bytes -= it->second;
  heap_blocks.erase(it);
}

// Under counter_mutex. False (logged once) for a heap other than the process
// heap's, which isn't counted.
bool IsProcessHeap(uint8_t* base, uint32_t heap, uint32_t caller) {
  if (heap == guest::LoadU32(base, kProcessHeapHandle)) {
    const uint32_t own = guest::LoadU32(base, heap + 24);
    if (FirstTime(kHeapOwnFlags, own)) ITC_LOG_INFO("heap counter: the heap's own flags {:#x}", own);
    return true;
  }
  if (FirstTime(kOtherHeap, heap)) ITC_LOG_INFO("heap counter: other heap {:#x} from {:#x}", heap, caller);
  return false;
}

}  // namespace

// HeapAlloc: r3 = heap, r4 = flags, r5 = size; returns the block or 0.
extern "C" REX_FUNC(sub_82299D60) {
  REX_FUNC_PROLOGUE();
  const uint32_t heap = ctx.r3.u32, flags = ctx.r4.u32, size = ctx.r5.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_82299D60(ctx, base);
  if (in_realloc) return;
  std::lock_guard lock(counter_mutex);
  if (!IsProcessHeap(base, heap, caller)) return;
  if (FirstTime(kHeapAllocFlags, flags))
    ITC_LOG_INFO("heap counter: HeapAlloc flags {:#x} from {:#x}", flags, caller);
  if (ctx.r3.u32 != 0) AddHeap(ctx.r3.u32, size);
}

// HeapFree: r3 = heap, r4 = flags, r5 = block.
extern "C" REX_FUNC(sub_8229A648) {
  REX_FUNC_PROLOGUE();
  const uint32_t heap = ctx.r3.u32, flags = ctx.r4.u32, block = ctx.r5.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_8229A648(ctx, base);
  if (in_realloc) return;
  std::lock_guard lock(counter_mutex);
  if (!IsProcessHeap(base, heap, caller)) return;
  if (FirstTime(kHeapFreeFlags, flags))
    ITC_LOG_INFO("heap counter: HeapFree flags {:#x} from {:#x}", flags, caller);
  RemoveHeap(block);
}

// HeapReAlloc: r3 = heap, r4 = flags, r5 = block, r6 = size; returns the
// block or 0.
extern "C" REX_FUNC(sub_8229A930) {
  REX_FUNC_PROLOGUE();
  const uint32_t heap = ctx.r3.u32, flags = ctx.r4.u32, block = ctx.r5.u32, size = ctx.r6.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  in_realloc = true;
  __imp__sub_8229A930(ctx, base);
  in_realloc = false;
  std::lock_guard lock(counter_mutex);
  if (!IsProcessHeap(base, heap, caller)) return;
  if (FirstTime(kHeapReAllocFlags, flags))
    ITC_LOG_INFO("heap counter: HeapReAlloc flags {:#x} from {:#x}", flags, caller);
  if (ctx.r3.u32 == 0) return;
  RemoveHeap(block);
  AddHeap(ctx.r3.u32, size);
}

// HeapSize: r3 = heap, r4 = flags, r5 = block; only checks the handle.
extern "C" REX_FUNC(sub_82298FF8) {
  REX_FUNC_PROLOGUE();
  const uint32_t heap = ctx.r3.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_82298FF8(ctx, base);
  std::lock_guard lock(counter_mutex);
  IsProcessHeap(base, heap, caller);
}

// VirtualAlloc: r3 = address, r4 = size, r5 = type, r6 = protection;
// returns the address or 0.
extern "C" REX_FUNC(sub_82295398) {
  REX_FUNC_PROLOGUE();
  const uint32_t address = ctx.r3.u32, size = ctx.r4.u32, type = ctx.r5.u32,
                 protect = ctx.r6.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_82295398(ctx, base);
  std::lock_guard lock(counter_mutex);
  if (FirstTime(kVirtualType, type))
    ITC_LOG_INFO("heap counter: VirtualAlloc type {:#x} from {:#x}", type, caller);
  if (FirstTime(kVirtualProtect, protect))
    ITC_LOG_INFO("heap counter: VirtualAlloc protection {:#x} from {:#x}", protect, caller);
  if (address != 0 && FirstTime(kVirtualAddress, caller))
    ITC_LOG_INFO("heap counter: VirtualAlloc at {:#x} (size {:#x}, type {:#x}) from {:#x}",
                 address, size, type, caller);
  // Only committed memory is counted (MEM_COMMIT 0x1000).
  if (ctx.r3.u32 == 0 || !(type & 0x1000)) return;
  const uint32_t rounded = (size + 4095) & ~4095u;
  largest_page_request = std::max(largest_page_request, size);
  auto [it, added] = page_blocks.emplace(ctx.r3.u32, rounded);
  if (!added) return;  // committed again inside a block already counted
  page_bytes += rounded;
  page_bytes_64k += (size + 65535) & ~65535u;
  ReportPeak();
}

// VirtualFree: r3 = address, r4 = size, r5 = type (MEM_RELEASE 0x8000).
extern "C" REX_FUNC(sub_82295330) {
  REX_FUNC_PROLOGUE();
  const uint32_t address = ctx.r3.u32, size = ctx.r4.u32, type = ctx.r5.u32;
  const uint32_t caller = uint32_t(ctx.lr);
  __imp__sub_82295330(ctx, base);
  std::lock_guard lock(counter_mutex);
  if ((size != 0 || type != 0x8000) && FirstTime(kVirtualFreeArgs, caller))
    ITC_LOG_INFO("heap counter: VirtualFree({:#x}, {:#x}, {:#x}) from {:#x}", address, size, type,
                 caller);
  if (type != 0x8000) return;
  auto it = page_blocks.find(address);
  if (it == page_blocks.end()) return;
  page_bytes -= it->second;
  page_bytes_64k -= (it->second + 65535) & ~65535u;
  page_blocks.erase(it);
}
