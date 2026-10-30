// The car's part loads as plain C++ (B7 in docs/rexglue-removal.md;
// docs/functions/car-models.md): the wheels (scene pool 5) with their rims'
// and brakes' asset ids. Guest data shared with the recompiled code, so
// these work on guest memory with the originals' loads and stores in their
// order. Adapters: src/hooks/car_models.cpp.

#pragma once

#include "archive/archive.h"

#include <cstdint>
#include <optional>

namespace car_models {

// The callback the wheels' loads hand Archive::Load (not mapped).
constexpr uint32_t kOnWheelPartLoaded = 0x820D8D20;

// sub_820D8BA8 (the rims): the front's (+44) and rear's (+60) wheel number
// n: -1 gives, for the car's block b ([[+24] + 32]) up to 19 (signed), the
// XPR2 587 + 3b and the rim 588 + 3b, else 695 and 696; n gives 695 + 3n and
// 696 + 3n. The XPR2s into +2144 and +2168, the rims into +1168 and +1192.
void WheelsRims(uint8_t* base, uint32_t wheels);

// sub_820D8C50 (the brakes): as the rims with 589 + 3b and 697 + 3n for the
// part; for traffic (+28 = 3), wheel 20 + [[+3264]] for both ends.
void WheelsBrakes(uint8_t* base, uint32_t wheels);

// sub_820D9110 (the wheels' set-up): the 156-byte set-up copied to +28
// (memcpy, native::Buffers_Memcpy), the rims or (by [[+3268] + 8] not 0) the
// brakes, then for the front and rear (i 0, 1) through LoadSlot::LoadShared
// with the wheels and kOnWheelPartLoaded: the tire 582, or 584 when +144 +
// 4i isn't 0, into +192 + 24i; the part into +1168 + 24i and the XPR2 into
// +2144 + 24i (their own +0's ids); the calipers 887 into +2280 + 24i.
// Returns the last LoadShared's r3; nullopt, with nothing done, where the
// copy can't be native (the original runs).
std::optional<uint32_t> WheelsSetUp(uint8_t* base, uint32_t wheels, uint32_t setup,
                                    const archive::LoadCall& load);

}  // namespace car_models
