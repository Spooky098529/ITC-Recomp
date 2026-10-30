// The car models' load slots and the deferred releases as plain C++ (B7 in
// docs/rexglue-removal.md; docs/functions/car-models.md): a 24-byte load
// slot (+0 the request, +4 -1 when free, +12 shared, +16 the buffer, +20 the
// state) and the first pool set's records of releases put off (20 bytes:
// four words, +16 the state). Guest data shared with the recompiled code, so
// these work on guest memory with the originals' loads and stores in their
// order. Adapters: src/hooks/car_models.cpp.

#pragma once

#include "archive/archive.h"

#include <cstdint>
#include <optional>

namespace car_models {

// LoadSlot::Reset (sub_820D0730): +0 = 0, +4 = -1, +8, +16, +12, +20 = 0.
void LoadSlotReset(uint8_t* base, uint32_t slot);

// LoadSlot::ReleaseBuffer (sub_820D0758): unless +12 (shared) is set, the
// buffer at +16, when set, freed (VirtualFree(buffer, 0, MEM_RELEASE)) and
// +16 = 0; +16 = 0 when shared. Returns the r3 the original leaves: `slot`
// when shared, the buffer word when it's 0, VirtualFree's 1 after the free.
// nullopt, with nothing done, where the buffer isn't pages the allocation
// layer gave (VirtualFree's adapter would go to the original; only over
// the layer: the adapter's condition).
std::optional<uint32_t> LoadSlotReleaseBuffer(uint8_t* base, uint32_t slot);

// Whether LoadSlotReleaseBuffer runs natively on `slot` (nothing changed):
// shared, no buffer, or the layer's pages. `*freed` = the pages it would
// free (0 for none), for callers releasing several slots: two slots with the
// same pages would fail the second free.
bool LoadSlotReleasable(uint8_t* base, uint32_t slot, uint32_t* freed);

// The scene's second set ([0x826C2E44]): its shared load buffer at +5056,
// the buffer's size at +5060.
constexpr uint32_t kSceneSet = 0x826C2E44;
constexpr uint32_t kSharedBuffer = 5056;
constexpr uint32_t kSharedBufferSize = 5060;

// LoadSlot::LoadShared (sub_820D0860): +0 = id, +8 = Archive::AssetSize(id);
// below the shared buffer's size (both read before +0 is written), +16 =
// the buffer, +4 = Archive::Load(id, the buffer, `callback`, `context`), +20
// = 2, +12 = 1, returning 1; otherwise (0 too) +20 = 1, returning 0.
uint32_t LoadSlotLoadShared(uint8_t* base, uint32_t slot, uint32_t id, uint32_t context,
                            uint32_t callback, const archive::LoadCall& load);

// LoadSlot::Load (sub_820D07C8): +0 = id, +8 = Archive::AssetSize(id); 0:
// +20 = 1, returning 0; else +16 = VirtualAlloc(size) (the allocation
// layer's AllocPages; GetLastError on the PCR `pcr` when that fails, its
// answer not read), +4 = Archive::Load(id, +16, `callback`, `context`), +20
// = 2, returning 1. Only while --native_heap routes VirtualAlloc to the
// layer (the adapter's condition).
uint32_t LoadSlotLoad(uint8_t* base, uint32_t slot, uint32_t id, uint32_t context,
                      uint32_t callback, uint32_t pcr, const archive::LoadCall& load);

// sub_820C3F50 (a release put off): the four words and state 2 in the first
// record of the set's +52 whose +16 is 0, among 4 x (+40 + +12) (nothing
// when none is free).
void DeferRelease(uint8_t* base, uint32_t set, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

}  // namespace car_models
