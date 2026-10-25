// The model's texture provider (20 bytes: +0 the vtable 0x8206CCB4, +4 a
// std::vector of texture pointers; docs/functions/models.md,
// "TextureProvider::") and the vector's helpers as plain C++ over the guest
// layout. The vector (16 bytes, at the provider's +4): +0 the allocator
// (empty), +4 the first element, +8 the end of the elements, +12 the end of
// the storage; offsets below are the vector's. Counts are the originals'
// (byte differences shifted right as signed words: srawi).
//
// Storage comes from operator new and goes back with operator delete (the C
// runtime heap's plain C++, memory/c_runtime_heap.h), so the functions that
// allocate or free run natively only while --native_heap routes the game's
// heap to the allocation layer (the adapters' condition). The paths that
// never ran in play stay on the recompiled code: a function that would take
// one (_Xlen's length_error, std::bad_alloc and the new handler, free's
// error path for a block the layer didn't give, InsertN's copy_backward for
// a tail at least the count long, a memmove_s the C runtime's plain C++
// leaves to the original) returns nullopt with nothing changed, and its
// adapter runs the game's own function from the start. Adapters:
// src/hooks/models.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace models {

constexpr uint32_t kTextureProviderVtable = 0x8206CCB4;
constexpr uint32_t kTextureProviderVector = 4;
constexpr uint32_t kTextureProviderSlots = 64;  // a count of -1
constexpr uint32_t kVectorFirst = 4;
constexpr uint32_t kVectorLast = 8;
constexpr uint32_t kVectorEnd = 12;
constexpr uint32_t kVectorMaxSize = 0x3FFFFFFF;

// What a function leaves in r3: `kept` when the original leaves the r3 it
// was given (the whole register), else `value`.
struct R3 {
  bool kept;
  uint32_t value;
};

// Whether TextureProvider::Allocate gives `count` pointers natively: no
// bad_alloc path (0xFFFFFFFF / count below 4), a size malloc takes, a
// process heap.
bool CanAllocatePointers(uint8_t* base, uint32_t count);

// sub_825942B8 TextureProvider::Allocate (std::allocator<T*>::allocate):
// operator new(4 x count). nullopt where the original must run.
std::optional<uint32_t> PointersAllocate(uint8_t* base, uint32_t count);

// sub_82594380 TextureProvider::UninitializedCopy: the words from `first` up
// to `last` copied to `destination` on (a null destination slot skipped);
// returns the destination's end.
uint32_t PointersUninitializedCopy(uint8_t* base, uint32_t first, uint32_t last,
                                   uint32_t destination);

// sub_82594570 TextureProvider::Reserve (std::vector<T*>::reserve).
std::optional<R3> PointersReserve(uint8_t* base, uint32_t vector, uint32_t count);

// sub_82590EF0 TextureProvider::InsertN (std::vector<T*>::_Insert_n):
// `count` copies of `value` inserted before `where`.
std::optional<R3> PointersInsertN(uint8_t* base, uint32_t vector, uint32_t where,
                                  uint32_t count, uint32_t value);

// sub_825947D0 TextureProvider::Assign (std::vector<T*>::assign): emptied,
// then `count` copies of `value`.
std::optional<R3> PointersAssign(uint8_t* base, uint32_t vector, uint32_t count,
                                 uint32_t value);

// sub_82590D68 TextureProvider::Clear (std::vector<T*>::clear): the end of
// the elements put back to the first; returns the first (r3).
uint32_t PointersClear(uint8_t* base, uint32_t vector);

// sub_82594850 TextureProvider::TextureProvider: the vtable, the vector
// emptied, then reserved and assigned `count` nulls (-1: 64). Returns the
// provider; nullopt where the original must run.
std::optional<uint32_t> TextureProviderConstruct(uint8_t* base, uint32_t provider,
                                                 int32_t count);

// sub_825943E0 TextureProvider::~TextureProvider: the vtable, the vector
// cleared, its storage freed, its three words = 0. Returns r3 (the storage);
// nullopt where the original must run.
std::optional<uint32_t> TextureProviderDestruct(uint8_t* base, uint32_t provider);

}  // namespace models
