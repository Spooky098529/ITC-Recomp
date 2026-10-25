// The C runtime's heap over the native allocation layer (memory/heap.h):
// GetProcessHeap, malloc, free, calloc, realloc, _msize, LocalAlloc,
// LocalFree, XMemAlloc, XMemFree and operator new and delete, as plain C++
// (docs/functions/heap.md). Adapters in src/hooks/c_runtime_heap.cpp, behind
// --native_c_runtime_heap; they only call these while --native_heap routes
// the game's heap to the layer, as these call the layer directly.
//
// The error paths (errno through _errno, _callnewh and the new handler,
// std::bad_alloc, the fatal error with no process heap, _invalid_parameter)
// never ran in play and stay recompiled: a function that would take one
// returns std::nullopt (or false) with nothing changed, and its adapter runs
// the game's own function from the start, which fails the same way and takes
// the path. The layer aborts when out of memory, so in play only sizes the
// game's functions refuse get there.

#pragma once

#include <cstdint>
#include <optional>

namespace memory::crt {

// sub_8229B670 GetProcessHeap: the process heap's handle (the word at
// 0x8297149C, made at boot by sub_82297C08).
uint32_t GetProcessHeap(uint8_t* base);

// sub_825A9A90 malloc: a block of `size` bytes (0 taken as 1); nullopt for a
// size over 0xFFFFF000, no process heap, or the layer giving 0.
std::optional<uint32_t> Malloc(uint8_t* base, uint32_t size);
// sub_825A9B58 free: false when the layer refuses the block (errno path).
bool Free(uint32_t block);
// sub_825AC248 _calloc_impl: `count` x `size` bytes cleared (0 taken as 1);
// nullopt when the product is over 0xFFFFF000 or the layer gives 0.
std::optional<uint32_t> CallocImpl(uint32_t count, uint32_t size);
// sub_825AC320 calloc: _calloc_impl with its error left to the original.
std::optional<uint32_t> Calloc(uint32_t count, uint32_t size);
// sub_825AEBC8 realloc: malloc for a null block, free (and 0) for size 0,
// else the layer's Realloc; nullopt on any failure.
std::optional<uint32_t> Realloc(uint8_t* base, uint32_t block, uint32_t size);
// sub_825AED30 _msize: the block's size; nullopt for a null block.
std::optional<uint32_t> Msize(uint32_t block);

// sub_82297F48 LocalAlloc: LMEM_ZEROINIT (0x40) clears the block.
uint32_t LocalAlloc(uint32_t flags, uint32_t size);
// sub_82297F90 LocalFree: 0 when freed, else the block.
uint32_t LocalFree(uint32_t block);
// sub_822976F8 XMemAlloc: physical memory with bit 0x80000000 of
// `attributes`, else LocalAlloc; bit 0x40000000 clears the block.
uint32_t XMemAlloc(uint8_t* base, uint32_t size, uint32_t attributes);
// sub_82296550 XPhysicalAllocEx as its adapter (src/hooks/heap.cpp) does it:
// X_MEM_LARGE_PAGES cleared from the protection when the word at 0x829537B0
// isn't 0, then the layer's AllocPhysical.
uint32_t XPhysicalAllocEx(uint8_t* base, uint32_t size, uint32_t low, uint32_t high,
                          uint32_t alignment, uint32_t protection);
// sub_82297790 XMemFree: what the game's leaves in r3 (0 or the block).
uint32_t XMemFree(uint32_t block, uint32_t attributes);

// sub_825A6880 operator new and sub_825A6940 (operator new[]): malloc;
// nullopt when it fails (the new handler and std::bad_alloc).
std::optional<uint32_t> OperatorNew(uint8_t* base, uint32_t size);
std::optional<uint32_t> OperatorNewArray(uint8_t* base, uint32_t size);
// sub_825A70E8 operator delete and sub_825A71C0 (operator delete[]): free.
bool OperatorDelete(uint32_t block);
bool OperatorDeleteArray(uint32_t block);

}  // namespace memory::crt
