// The native allocation layer: every allocation the game makes, recompiled
// and native alike, goes through these functions (the recompiled ones by the
// adapters in src/hooks/heap.cpp; native code calls them directly). The heap
// and the pages come from one guest range of our own mapping
// (memory/mapping.h, 384 MiB at 0x10000), first fit over the free ranges,
// with block sizes kept here (no headers in guest memory); physical memory
// from our own allocator over its 512 MB, seen through the mapping's
// physical views. Addresses are guest addresses. See docs/native-memory.md.

#pragma once

#include <cstdint>
#include <string>

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
// Whether `address` is a heap block the layer gave (one HeapFree frees and
// HeapSize answers), with nothing logged: for native callers checking
// before they free.
bool IsHeapBlock(uint32_t address);

// VirtualAlloc (sub_82295398) with MEM_COMMIT and no address: `size`
// rounded up to 4 KiB, at `alignment` (4,096, or 65,536 for large pages),
// cleared; 0 for size 0. Out of memory logs and aborts.
uint32_t AllocPages(uint32_t size, uint32_t alignment);
// VirtualFree (sub_82295330) with MEM_RELEASE: false (logged once) for an
// address the layer didn't give.
bool FreePages(uint32_t address);
// Whether `address` is pages the layer gave (VirtualFree frees it), with
// nothing logged: for native callers and harnesses checking before they
// free.
bool IsPages(uint32_t address);
// Whether `address` is inside the layer's range (false before the first
// allocation), or a handle (a host object, E5: ours too, freed through it).
bool InRange(uint32_t address);

// MmAllocatePhysicalMemoryEx as XPhysicalAlloc (sub_822964C8) and
// XPhysicalAllocEx (sub_82296550) call it: `size` bytes in physical
// memory whose physical address is within [low, high], at `alignment`, with
// the Xbox protection bits (X_PAGE_*, X_MEM_LARGE_PAGES for 64 KiB pages,
// X_MEM_16MB_PAGES); top down, at the view for the page size (0xA0000000,
// 0xC0000000, 0xE0000000), as the SDK's import did. Out of memory logs and
// aborts.
uint32_t AllocPhysical(uint32_t size, uint32_t low, uint32_t high, uint32_t alignment,
                       uint32_t protection);
// XPhysicalFree (sub_822965C8): MmFreePhysicalMemory's release; logged once
// for an address AllocPhysical didn't give.
void FreePhysical(uint32_t address);
// Called after a physical block is freed (its address and size), from the
// freeing thread: the renderer lets go of what it holds of it
// (docs/pc-renderer.md, step 2b). One listener; set once at start-up.
void SetPhysicalFreeListener(void (*listener)(uint32_t address, uint32_t size));
// XPhysicalSize (sub_822965C0, MmQueryAllocationSize's wrapper): the block's
// allocated size, as the SDK's import answers it (the size AllocPhysical
// asked for rounded up to its page size, one page for 0), recorded by
// AllocPhysical; 0
// (logged once) for an address it didn't give. The user's call, 2026-11-06.
uint32_t PhysicalSize(uint32_t address);

// What the physical memory holds, by who asked (docs/pc-renderer.md, step
// 2b): a tag set for the calling thread while a loader allocates
// (PhysicalTag), counted per live block; "models 120.3 MB (812), ..." for
// the memory log.
class PhysicalTag {
 public:
  // keep_outer: an outer tag already set (not "other") is kept (a loader
  // naming what its model's vertices are for, around Model::Load's own).
  explicit PhysicalTag(const char* name, bool keep_outer = false);
  ~PhysicalTag();
 private:
  const char* previous_;
};
std::string DescribePhysical();

}  // namespace memory
