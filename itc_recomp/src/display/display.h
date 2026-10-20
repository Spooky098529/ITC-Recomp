// What the display tells the game: the answers the kernel's video functions
// (Vd*) gave, now ours. The values are the ones the SDK answered with
// (rexglue-sdk/src/kernel/xboxkrnl/xboxkrnl_video.cpp). See B3 in
// docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace display {

// The display mode's size (VdQueryVideoMode): the game's own resolution,
// what the SDK answered with its video_mode_* settings at their defaults.
// Our --width and --height don't change it: every pass is drawn scaled.
constexpr uint32_t kWidth = 1280;
constexpr uint32_t kHeight = 720;

// Whether the display is widescreen (XGetVideoMode's is_widescreen, as the
// SDK worked it out from the size).
constexpr bool kWidescreen = kWidth * 3 >= kHeight * 4;

// The display's gamma (VdGetCurrentDisplayGamma): its type (1 sRGB, 2
// BT.709, 3 a power; anything else is taken as BT.709 by the game's ramp
// functions) and, for type 3, the power.
struct Gamma {
  uint32_t type;
  float power;
};

constexpr Gamma kGamma{2, float(2.22222233)};

}  // namespace display
