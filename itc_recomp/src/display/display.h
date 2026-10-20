// What the display tells the game: the answers the kernel's video functions
// (Vd*) gave, now ours. The values are the ones the SDK answered with
// (rexglue-sdk/src/kernel/xboxkrnl/xboxkrnl_video.cpp). See B3 in
// docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace display {

// The display's gamma (VdGetCurrentDisplayGamma): its type (1 sRGB, 2
// BT.709, 3 a power; anything else is taken as BT.709 by the game's ramp
// functions) and, for type 3, the power.
struct Gamma {
  uint32_t type;
  float power;
};

constexpr Gamma kGamma{2, float(2.22222233)};

}  // namespace display
