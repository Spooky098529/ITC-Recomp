// The car models' load slots and the deferred releases as plain C++ (B7 in
// docs/rexglue-removal.md; docs/functions/car-models.md): a 24-byte load
// slot (+0 the request, +4 -1 when free, +12 shared, +16 the buffer, +20 the
// state) and the first pool set's records of releases put off (20 bytes:
// four words, +16 the state). Guest data shared with the recompiled code, so
// these work on guest memory with the originals' loads and stores in their
// order. Adapters: src/hooks/car_models.cpp.

#pragma once

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

// sub_820C3F50 (a release put off): the four words and state 2 in the first
// record of the set's +52 whose +16 is 0, among 4 x (+40 + +12) (nothing
// when none is free).
void DeferRelease(uint8_t* base, uint32_t set, uint32_t a, uint32_t b, uint32_t c, uint32_t d);

}  // namespace car_models
