// The graphics library's gamma ramps, as plain C++: a 1024-entry curve from
// the display's gamma, and the game's ramps run through it (sub_822B0910,
// sub_822B0AB0; their adapters are in src/hooks/gamma.cpp). What they make
// is what `--xbox_gamma` copied (src/native/xbox_gamma_table.h). See B3 in
// docs/rexglue-removal.md.

#pragma once

#include "display/display.h"

#include <array>
#include <cstdint>

namespace display {

// sRGB's curve (sub_822B07B0): to linear, or from it.
float Srgb(float x, bool to_linear);
// BT.709's curve (sub_822B0860): to linear, or from it.
float Bt709(float x, bool to_linear);
// pow as the game's ramps use it (the C runtime's, sub_825A8ED0).
double Pow(double x, double y);

// 1024 10-bit values. `inverse` false: a ramp's sRGB values to the
// display's; true: back from the display's to sRGB.
using GammaCurve = std::array<uint16_t, 1024>;
GammaCurve MakeGammaCurve(const Gamma& gamma, bool inverse);

// A 256-entry ramp, three channels of 16-bit values (sub_822B0910).
using GammaRamp = std::array<std::array<uint16_t, 256>, 3>;
GammaRamp ApplyGammaCurve(const GammaCurve& curve, const GammaRamp& ramp);

// A 128-entry piecewise-linear ramp, three channels of (base, delta) pairs
// (sub_822B0AB0).
struct PwlEntry {
  uint16_t base;
  uint16_t delta;
};
using GammaRampPwl = std::array<std::array<PwlEntry, 128>, 3>;
GammaRampPwl ApplyGammaCurve(const GammaCurve& curve, const GammaRampPwl& ramp);

}  // namespace display
