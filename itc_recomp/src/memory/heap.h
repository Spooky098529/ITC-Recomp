// The native allocation layer: every allocation the game makes, recompiled
// and native alike, goes through these functions (the recompiled ones by the
// adapters in src/hooks/heap.cpp; native code calls them directly). The heap
// and the pages come from one guest range reserved once from the SDK's 4 KiB
// virtual heap, first fit over the free ranges, with block sizes kept here
// (no headers in guest memory); physical memory comes from the SDK's
// physical heaps. Addresses are guest addresses. See docs/native-memory.md.

#pragma once

#include <cstdint>

namespace memory {

// HeapAlloc's and HeapReAlloc's flags the layer acts on.
constexpr uint32_t kHeapZeroMemory = 0x8;

// HeapAlloc (sub_82299D60): a 16-aligned block of `size` bytes (size 0 gives
// a block of size 0), cleared with kHeapZeroMemory; 0 for a size over
// 0x7FFFFFFF. Out of memory logs and aborts.
uint32_t Alloc(uint32_t size, uint32_t flags);
// HeapFree (sub_8229A648): false (logged once) for an address the layer
// didn't give; true for 0, which frees nothing.
bool Free(uint32_t address);
// HeapReAlloc (sub_8229A930): the block resized, in place when it fits (or
// its free neighbor does), else moved with the smaller of the two sizes
// copied; the part past the old size cleared with kHeapZeroMemory. 0 for
// address 0, a size over 0x7FFFFFFF, or an address the layer didn't give
// (logged once). A size of 0 keeps a block of size 0, as the game's does.
uint32_t Realloc(uint32_t address, uint32_t size, uint32_t flags);
// HeapSize (sub_82298FF8): the size asked for; 0xFFFFFFFF for an address the
// layer didn't give.
uint32_t Size(uint32_t address);

// VirtualAlloc (sub_82295398) with MEM_COMMIT and no address: `size`
// rounded up to 4 KiB, at `alignment` (4,096, or 65,536 for large pages),
// cleared; 0 for size 0. Out of memory logs and aborts.
uint32_t AllocPages(uint32_t size, uint32_t alignment);
// VirtualFree (sub_82295330) with MEM_RELEASE: false (logged once) for an
// address the layer didn't give.
bool FreePages(uint32_t address);
// Whether `address` is inside the layer's range (false before the first
// allocation).
bool InRange(uint32_t address);

// MmAllocatePhysicalMemoryEx as XPhysicalAlloc (sub_822964C8) and
// XPhysicalAllocEx (sub_82296550) call it: `size` bytes in physical
// memory whose physical address is within [low, high], at `alignment`, with
// the Xbox protection bits (X_PAGE_*, X_MEM_LARGE_PAGES for 64 KiB pages,
// X_MEM_16MB_PAGES); from the SDK's physical heap for the page size, as the
// SDK's import does. Out of memory logs and aborts.
uint32_t AllocPhysical(uint32_t size, uint32_t low, uint32_t high, uint32_t alignment,
                       uint32_t protection);
// XPhysicalFree (sub_822965C8): MmFreePhysicalMemory's release.
void FreePhysical(uint32_t address);

}  // namespace memory
