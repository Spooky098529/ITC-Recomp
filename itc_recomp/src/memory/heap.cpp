#include "memory/heap.h"

#include "base/log.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>
#include <rex/system/xtypes.h>

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
constexpr uint32_t kRangeBytes = 384u << 20;
constexpr uint32_t kPageBytes = 4096;

struct Block {
  uint32_t size;  // what was asked for
  uint32_t span;  // what the block takes in the range
  bool pages;     // from AllocPages
};

std::mutex mutex;
uint8_t* membase = nullptr;
std::atomic<uint32_t> range_start{0};
uint32_t range_end = 0;
std::map<uint32_t, uint32_t> free_ranges;  // start -> bytes, sorted by address
std::unordered_map<uint32_t, Block> blocks;
std::set<uint64_t> seen;  // what's been logged once: kind << 32 | value

enum Seen : uint64_t {
  kAllocFlags = 1,
  kReallocFlags,
  kUnknownFree,
  kUnknownRealloc,
  kUnknownSize,
  kUnknownFreePages,
  kPhysicalProtection,
};

// Under the mutex.
bool FirstTime(Seen kind, uint32_t value) {
  return seen.insert(uint64_t(kind) << 32 | value).second;
}

[[noreturn]] void OutOfMemory(const char* what, uint32_t size) {
  ITC_LOG_ERROR("native heap: out of memory: {} of {} bytes", what, size);
  std::abort();
}

// Reserves the range, on the first allocation (under the mutex).
void ReserveRange() {
  if (range_start.load(std::memory_order_relaxed) != 0) {
    return;
  }
  rex::memory::Memory* memory = REX_KERNEL_MEMORY();
  uint32_t address = 0;
  if (!memory->LookupHeapByType(false, kPageBytes)
           ->Alloc(kRangeBytes, 64 * 1024,
                   rex::memory::kMemoryAllocationReserve | rex::memory::kMemoryAllocationCommit,
                   rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite, false,
                   &address)) {
    OutOfMemory("reserving the range", kRangeBytes);
  }
  membase = memory->virtual_membase();
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

// Returns bytes to the free ranges, joined with their neighbors.
void Give(uint32_t start, uint32_t bytes) {
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

uint32_t HeapSpan(uint32_t size) {
  return std::max((size + 7) & ~7u, 8u);
}

void Clear(uint32_t address, uint32_t bytes) {
  std::memset(membase + address, 0, bytes);
}

// The SDK's FromXdkProtectFlags (not in a header).
uint32_t HostProtection(uint32_t protection) {
  uint32_t result = 0;
  if (protection & (rex::X_PAGE_READONLY | rex::X_PAGE_EXECUTE_READ)) {
    result = rex::memory::kMemoryProtectRead;
  } else if (protection & (rex::X_PAGE_READWRITE | rex::X_PAGE_EXECUTE_READWRITE)) {
    result = rex::memory::kMemoryProtectRead | rex::memory::kMemoryProtectWrite;
  }
  if (protection & rex::X_PAGE_NOCACHE) {
    result |= rex::memory::kMemoryProtectNoCache;
  }
  if (protection & rex::X_PAGE_WRITECOMBINE) {
    result |= rex::memory::kMemoryProtectWriteCombine;
  }
  return result;
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
  const uint32_t address = Take(span, 8);
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
  size = std::max(size, 1u);
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
      result = Take(span, 8);
      if (result == 0) {
        OutOfMemory("HeapReAlloc", size);
      }
      std::memmove(membase + result, membase + address, std::min(old_size, size));
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
  const uint32_t start = range_start.load(std::memory_order_relaxed);
  return start != 0 && address >= start && address - start < kRangeBytes;
}

uint32_t AllocPhysical(uint32_t size, uint32_t low, uint32_t high, uint32_t alignment,
                       uint32_t protection) {
  ITC_NATIVE_RUNNING("sub_82296550 XPhysicalAllocEx");
  if (!(protection & (rex::X_PAGE_READONLY | rex::X_PAGE_READWRITE))) {
    std::lock_guard lock(mutex);
    if (FirstTime(kPhysicalProtection, protection)) {
      ITC_LOG_ERROR("native heap: physical memory with protection {:#x}", protection);
    }
    return 0;
  }
  // As the SDK's MmAllocatePhysicalMemoryEx: the page size from the
  // protection, the physical range moved into the heap's addresses, top down.
  uint32_t page_size = kPageBytes;
  if (protection & rex::X_MEM_LARGE_PAGES) {
    page_size = 64 * 1024;
  } else if (protection & rex::X_MEM_16MB_PAGES) {
    page_size = 16 * 1024 * 1024;
  }
  const uint32_t rounded_size = (size + page_size - 1) & ~(page_size - 1);
  const uint32_t rounded_alignment = (alignment + page_size - 1) & ~(page_size - 1);
  auto* heap = static_cast<rex::memory::PhysicalHeap*>(
      REX_KERNEL_MEMORY()->LookupHeapByType(true, page_size));
  const uint32_t heap_base = heap->heap_base();
  const uint32_t offset = heap->GetPhysicalAddress(heap_base);
  const uint32_t heap_size = heap->heap_size();
  const uint32_t heap_low = heap_base + std::min(low > offset ? low - offset : 0, heap_size - 1);
  const uint32_t heap_high = heap_base + std::min(high > offset ? high - offset : 0, heap_size - 1);
  uint32_t address = 0;
  if (!heap->AllocRange(heap_low, heap_high, rounded_size, rounded_alignment,
                        rex::memory::kMemoryAllocationReserve |
                            rex::memory::kMemoryAllocationCommit,
                        HostProtection(protection), true, &address)) {
    OutOfMemory("physical memory", size);
  }
  return address;
}

void FreePhysical(uint32_t address) {
  ITC_NATIVE_RUNNING("sub_822965C8 XPhysicalFree");
  REX_KERNEL_MEMORY()->LookupHeap(address)->Release(address);
}

}  // namespace memory
