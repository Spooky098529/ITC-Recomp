#include "memory/heap.h"
#include "guest.h"

#include "memory/handles.h"
#include "memory/mapping.h"

#include "base/log.h"

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <unordered_map>

namespace memory {

namespace {

// The range's size: the most the game had in use was 172.6 MiB (heap and
// pages, a drive through the front end and a race); see "What the counter
// saw" in docs/native-memory.md.
constexpr uint32_t kRangeBytes = kHeapBytes;  // guest.h: the heap's block
constexpr uint32_t kPageBytes = 4096;

struct Block {
  uint32_t size;  // what was asked for
  uint32_t span;  // what the block takes in the range
  bool pages;     // from AllocPages
};

std::mutex mutex;
std::atomic<uint32_t> range_start{0};
uint32_t range_end = 0;
std::map<uint32_t, uint32_t> free_ranges;  // start -> bytes, sorted by address
std::unordered_map<uint32_t, Block> blocks;
// Physical blocks' allocated sizes (the size rounded to their page size), by
// address: what MmQueryAllocationSize answers for them.
std::unordered_map<uint32_t, uint32_t> physical_sizes;
std::set<uint64_t> seen;  // what's been logged once: kind << 32 | value

enum Seen : uint64_t {
  kAllocFlags = 1,
  kReallocFlags,
  kUnknownFree,
  kUnknownRealloc,
  kUnknownSize,
  kUnknownFreePages,
  kPhysicalProtection,
  kUnknownPhysicalSize,
  kUnknownFreePhysical,
};

// Under the mutex.
bool FirstTime(Seen kind, uint32_t value) {
  return seen.insert(uint64_t(kind) << 32 | value).second;
}

[[noreturn]] void OutOfMemory(const char* what, uint32_t size) {
  ITC_LOG_ERROR("native heap: out of memory: {} of {} bytes", what, size);
  std::abort();
}

// The range's start: the first 64 KB past page 0, where the SDK's 4 KB
// heap gave it.
constexpr uint32_t kRangeStart = kHeapStart;

// Takes the range, on the first allocation (under the mutex): guest memory
// mapped by us (memory/mapping.h).
void ReserveRange() {
  if (range_start.load(std::memory_order_relaxed) != 0) {
    return;
  }
  if (!MapGuest()) {
    OutOfMemory("mapping guest memory", kRangeBytes);
  }
  const uint32_t address = kRangeStart;
  range_end = address + kRangeBytes;
  free_ranges.emplace(address, kRangeBytes);
  range_start.store(address, std::memory_order_relaxed);
  ITC_LOG_INFO("native heap: {} MiB at {:#x}", kRangeBytes >> 20, address);
}

// First fit: `span` bytes at `alignment` (a power of two) from the free
// ranges; 0 when none fits.
uint32_t Take(uint32_t span, uint32_t alignment) {
  for (auto it = free_ranges.begin(); it != free_ranges.end(); ++it) {
    const uint32_t start = it->first;
    const uint32_t end = start + it->second;
    const uint32_t aligned = (start + alignment - 1) & ~(alignment - 1);
    if (uint64_t(aligned) + span > end) {
      continue;
    }
    free_ranges.erase(it);
    if (aligned > start) {
      free_ranges.emplace(start, aligned - start);
    }
    if (aligned + span < end) {
      free_ranges.emplace(aligned + span, end - aligned - span);
    }
    return aligned;
  }
  return 0;
}

// Returns bytes to free ranges, joined with their neighbors.
void Give(std::map<uint32_t, uint32_t>& free_ranges, uint32_t start, uint32_t bytes) {
  auto next = free_ranges.lower_bound(start);
  if (next != free_ranges.end() && start + bytes == next->first) {
    bytes += next->second;
    next = free_ranges.erase(next);
  }
  if (next != free_ranges.begin()) {
    auto previous = std::prev(next);
    if (previous->first + previous->second == start) {
      previous->second += bytes;
      return;
    }
  }
  free_ranges.emplace_hint(next, start, bytes);
}

void Give(uint32_t start, uint32_t bytes) {
  Give(free_ranges, start, bytes);
}

// Heap blocks are 16-aligned and take multiples of 16 bytes, as the game's
// heap's: AltiVec loads and stores (lvx, stvx) on them drop the address's
// low four bits.
constexpr uint32_t kHeapAlignment = 16;

uint32_t HeapSpan(uint32_t size) {
  return std::max((size + kHeapAlignment - 1) & ~(kHeapAlignment - 1), kHeapAlignment);
}

void Clear(uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

// Xbox protection bits (X_PAGE_*, X_MEM_*) AllocPhysical reads.
constexpr uint32_t kPageReadOnly = 0x02;
constexpr uint32_t kPageReadWrite = 0x04;
constexpr uint32_t kMem16MbPages = 0x02000000;
constexpr uint32_t kMemLargePages = 0x20000000;

// Physical memory: [0, 512 MB), handed out top down (the SDK's physical
// heaps allocate top down) in whole 4 KB pages, by physical address; free
// ranges kept here (start -> bytes), as the virtual range's.
constexpr uint32_t kPhysicalBytes = 0x20000000;
std::map<uint32_t, uint32_t> physical_free;

// The views physical memory is seen through (memory/mapping.h), by page
// size: the guest address of physical 0 in each (the 4 KB view is a page
// off: guest 0xE0000000 + x is physical x + 0x1000) and the physical range
// it reaches, as the SDK's PhysicalHeaps.
struct PhysicalView {
  uint32_t base;     // guest address of the view
  uint32_t offset;   // physical address at the view's base
  uint32_t size;     // the view's size in guest addresses
};
constexpr PhysicalView kView64Kb = {0xA0000000, 0, 0x20000000};
constexpr PhysicalView kView16Mb = {0xC0000000, 0, 0x20000000};
constexpr PhysicalView kView4Kb = {0xE0000000, 0x1000, 0x1FD00000};

const PhysicalView& ViewOf(uint32_t address) {
  return address >= kView4Kb.base ? kView4Kb : address >= kView16Mb.base ? kView16Mb : kView64Kb;
}

// Takes [start, start + bytes) out of the physical free ranges (it must be
// inside one).
void TakePhysical(uint32_t start, uint32_t bytes) {
  auto it = std::prev(physical_free.upper_bound(start));
  const uint32_t range = it->first;
  const uint32_t end = range + it->second;
  physical_free.erase(it);
  if (start > range) {
    physical_free.emplace(range, start - range);
  }
  if (start + bytes < end) {
    physical_free.emplace(start + bytes, end - start - bytes);
  }
}

// The physical ranges the SDK's memory kept for itself at start-up, kept out
// the same way (under the mutex, once): 0-16 MB (the GPU writeback at
// 0xC0000000, also what 0x7F000000 shows), the 3.25 MB its 64 KB view
// reserved below the top 64 KB, and the top 64 KB.
void SetUpPhysical() {
  if (!physical_free.empty()) {
    return;
  }
  physical_free.emplace(0, kPhysicalBytes);
  TakePhysical(0, 0x01000000);
  TakePhysical(0x1FCB0000, 0x340000);
  TakePhysical(0x1FFF0000, 0x10000);
}

// Top down: the highest physical address at `alignment` (a multiple of
// 4 KB) where `bytes` fit free inside [low, high_end); ~0u when none does.
uint32_t FindPhysical(uint32_t bytes, uint32_t alignment, uint32_t low, uint64_t high_end) {
  for (auto it = physical_free.rbegin(); it != physical_free.rend(); ++it) {
    const uint64_t start = std::max<uint64_t>(it->first, low);
    const uint64_t end = std::min<uint64_t>(uint64_t(it->first) + it->second, high_end);
    if (end < start + bytes) {
      continue;
    }
    const uint64_t candidate = (end - bytes) & ~uint64_t(alignment - 1);
    if (candidate >= start) {
      return uint32_t(candidate);
    }
  }
  return ~0u;
}

}  // namespace

uint32_t Alloc(uint32_t size, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_82299D60 HeapAlloc");
  std::lock_guard lock(mutex);
  if (flags != 0 && flags != kHeapZeroMemory && FirstTime(kAllocFlags, flags)) {
    ITC_LOG_WARN("native heap: HeapAlloc flags {:#x} not seen before", flags);
  }
  if (size > 0x7FFFFFFF) {
    return 0;
  }
  ReserveRange();
  const uint32_t span = HeapSpan(size);
  const uint32_t address = Take(span, kHeapAlignment);
  if (address == 0) {
    OutOfMemory("HeapAlloc", size);
  }
  blocks[address] = {size, span, false};
  if (flags & kHeapZeroMemory) {
    Clear(address, size);
  }
  return address;
}

bool Free(uint32_t address) {
  ITC_NATIVE_RUNNING("sub_8229A648 HeapFree");
  if (address == 0) {
    return true;
  }
  if (IsHandle(address)) {
    return FreeHandle(address);  // a host object (E5)
  }
  std::lock_guard lock(mutex);
  auto it = blocks.find(address);
  if (it == blocks.end() || it->second.pages) {
    if (FirstTime(kUnknownFree, address)) {
      ITC_LOG_ERROR("native heap: HeapFree of {:#x}, not a heap block of the layer's", address);
    }
    return false;
  }
  Give(address, it->second.span);
  blocks.erase(it);
  return true;
}

uint32_t Realloc(uint32_t address, uint32_t size, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8229A930 HeapReAlloc");
  if (address == 0 || size > 0x7FFFFFFF) {
    return 0;
  }
  std::lock_guard lock(mutex);
  if (flags != 0 && flags != kHeapZeroMemory && FirstTime(kReallocFlags, flags)) {
    ITC_LOG_WARN("native heap: HeapReAlloc flags {:#x} not seen before", flags);
  }
  auto it = blocks.find(address);
  if (it == blocks.end() || it->second.pages) {
    if (FirstTime(kUnknownRealloc, address)) {
      ITC_LOG_ERROR("native heap: HeapReAlloc of {:#x}, not a heap block of the layer's", address);
    }
    return 0;
  }
  Block& block = it->second;
  const uint32_t old_size = block.size;
  const uint32_t span = HeapSpan(size);
  uint32_t result = address;
  if (span <= block.span) {
    // Shrinks (or stays) in place; the tail goes back.
    if (span < block.span) {
      Give(address + span, block.span - span);
      block.span = span;
    }
    block.size = size;
  } else {
    auto next = free_ranges.find(address + block.span);
    const uint32_t more = span - block.span;
    if (next != free_ranges.end() && next->second >= more) {
      // Grows in place into the free range after it.
      const uint32_t rest = next->second - more;
      free_ranges.erase(next);
      if (rest != 0) {
        free_ranges.emplace(address + span, rest);
      }
      block.span = span;
      block.size = size;
    } else {
      result = Take(span, kHeapAlignment);
      if (result == 0) {
        OutOfMemory("HeapReAlloc", size);
      }
      std::memmove(guest::Pointer(result), guest::Pointer(address),
                   std::min(old_size, size));
      Give(address, block.span);
      blocks.erase(it);
      blocks[result] = {size, span, false};
    }
  }
  if ((flags & kHeapZeroMemory) && size > old_size) {
    Clear(result + old_size, size - old_size);
  }
  return result;
}

uint32_t Size(uint32_t address) {
  ITC_NATIVE_RUNNING("sub_82298FF8 HeapSize");
  if (HandleInfo info; IsHandle(address) && DescribeHandle(address, &info)) {
    return info.size;  // a host object (E5)
  }
  std::lock_guard lock(mutex);
  auto it = blocks.find(address);
  if (it == blocks.end() || it->second.pages) {
    if (FirstTime(kUnknownSize, address)) {
      ITC_LOG_ERROR("native heap: HeapSize of {:#x}, not a heap block of the layer's", address);
    }
    return 0xFFFFFFFF;
  }
  return it->second.size;
}

bool IsHeapBlock(uint32_t address) {
  if (HandleInfo info; IsHandle(address) && DescribeHandle(address, &info)) {
    return info.offset == 0;  // a host object's handle (E5): freed by any of the frees
  }
  std::lock_guard lock(mutex);
  auto it = blocks.find(address);
  return it != blocks.end() && !it->second.pages;
}

bool IsPages(uint32_t address) {
  if (HandleInfo info; IsHandle(address) && DescribeHandle(address, &info)) {
    return info.offset == 0;  // a host object's handle (E5): freed by any of the frees
  }
  std::lock_guard lock(mutex);
  auto it = blocks.find(address);
  return it != blocks.end() && it->second.pages;
}

uint32_t AllocPages(uint32_t size, uint32_t alignment) {
  ITC_NATIVE_RUNNING("sub_82295398 VirtualAlloc");
  if (size == 0) {
    return 0;
  }
  std::lock_guard lock(mutex);
  ReserveRange();
  const uint32_t span = uint32_t((uint64_t(size) + kPageBytes - 1) & ~uint64_t(kPageBytes - 1));
  const uint32_t address = span == 0 ? 0 : Take(span, std::max(alignment, kPageBytes));
  if (address == 0) {
    OutOfMemory("VirtualAlloc", size);
  }
  blocks[address] = {size, span, true};
  Clear(address, span);
  return address;
}

bool FreePages(uint32_t address) {
  ITC_NATIVE_RUNNING("sub_82295330 VirtualFree");
  if (IsHandle(address)) {
    return FreeHandle(address);  // a host object (E5)
  }
  std::lock_guard lock(mutex);
  auto it = blocks.find(address);
  if (it == blocks.end() || !it->second.pages) {
    if (FirstTime(kUnknownFreePages, address)) {
      ITC_LOG_ERROR("native heap: VirtualFree of {:#x}, not pages of the layer's", address);
    }
    return false;
  }
  Give(address, it->second.span);
  blocks.erase(it);
  return true;
}

bool InRange(uint32_t address) {
  if (IsHandle(address)) {
    return true;  // a host object (E5): ours, freed through its handle
  }
  const uint32_t start = range_start.load(std::memory_order_relaxed);
  return start != 0 && address >= start && address - start < kRangeBytes;
}

uint32_t AllocPhysical(uint32_t size, uint32_t low, uint32_t high, uint32_t alignment,
                       uint32_t protection) {
  ITC_NATIVE_RUNNING("sub_82296550 XPhysicalAllocEx");
  if (!(protection & (kPageReadOnly | kPageReadWrite))) {
    std::lock_guard lock(mutex);
    if (FirstTime(kPhysicalProtection, protection)) {
      ITC_LOG_ERROR("native heap: physical memory with protection {:#x}", protection);
    }
    return 0;
  }
  // As the SDK's MmAllocatePhysicalMemoryEx: the page size from the
  // protection (it picks the view), the size and alignment rounded to it,
  // [low, high] clamped to the view's physical range and rounded up to the
  // alignment (BaseHeap::AllocRange), top down.
  uint32_t page_size = kPageBytes;
  const PhysicalView* view = &kView4Kb;
  if (protection & kMemLargePages) {
    page_size = 64 * 1024;
    view = &kView64Kb;
  } else if (protection & kMem16MbPages) {
    page_size = 16 * 1024 * 1024;
    view = &kView16Mb;
  }
  // A size of 0 takes a page, as the SDK's heaps (rex::round_up).
  const uint32_t rounded_size =
      std::max(uint32_t((uint64_t(size) + page_size - 1) & ~uint64_t(page_size - 1)), page_size);
  const uint32_t rounded_alignment =
      std::max((alignment + page_size - 1) & ~(page_size - 1), page_size);
  const uint32_t view_low = view->offset;
  const uint32_t view_high = view->offset + view->size - 1;
  const uint32_t clamped_low = std::clamp(low, view_low, view_high);
  const uint32_t clamped_high = std::clamp(high, view_low, view_high);
  const uint64_t aligned_low =
      (uint64_t(clamped_low) + rounded_alignment - 1) & ~uint64_t(rounded_alignment - 1);
  const uint64_t aligned_high = std::min<uint64_t>(
      (uint64_t(clamped_high) + rounded_alignment - 1) & ~uint64_t(rounded_alignment - 1),
      kPhysicalBytes - 1);
  // Through the end of the page aligned_high is in.
  const uint64_t high_end = (aligned_high & ~uint64_t(kPageBytes - 1)) + kPageBytes;
  uint32_t address;
  {
    std::lock_guard lock(mutex);
    SetUpPhysical();
    const uint32_t physical =
        aligned_low > 0xFFFFFFFFu
            ? ~0u
            : FindPhysical(rounded_size, rounded_alignment, uint32_t(aligned_low), high_end);
    if (physical == ~0u) {
      OutOfMemory("physical memory", size);
    }
    TakePhysical(physical, rounded_size);
    address = view->base + (physical - view->offset);
    // What MmQueryAllocationSize answers.
    physical_sizes[address] = rounded_size;
  }
  return address;
}

void FreePhysical(uint32_t address) {
  ITC_NATIVE_RUNNING("sub_822965C8 XPhysicalFree");
  if (IsHandle(address)) {
    FreeHandle(address);  // a host object (E5)
    return;
  }
  std::lock_guard lock(mutex);
  const auto it = physical_sizes.find(address);
  if (it == physical_sizes.end()) {
    if (FirstTime(kUnknownFreePhysical, address)) {
      ITC_LOG_ERROR("native heap: XPhysicalFree of {:#x}, not a physical block the layer gave",
                    address);
    }
    return;
  }
  const PhysicalView& view = ViewOf(address);
  Give(physical_free, address - view.base + view.offset, it->second);
  physical_sizes.erase(it);
}

uint32_t PhysicalSize(uint32_t address) {
  ITC_NATIVE_RUNNING("sub_822965C0 XPhysicalSize");
  if (HandleInfo info; IsHandle(address) && DescribeHandle(address, &info)) {
    // A blob made where the game asked for physical memory (E5 step 7):
    // whole 4 KB pages, as a 4 KB-page physical block's size.
    return info.size;
  }
  std::lock_guard lock(mutex);
  const auto it = physical_sizes.find(address);
  if (it == physical_sizes.end()) {
    if (FirstTime(kUnknownPhysicalSize, 0)) {
      ITC_LOG_ERROR("native heap: XPhysicalSize of {:#x}, not a physical block the layer gave",
                    address);
    }
    return 0;
  }
  return it->second;
}

}  // namespace memory
