// The graphics library's two gamma ramp functions, adapters in front of
// src/display/gamma.cpp. Their callers (sub_822A3A20, sub_822A3AA8 and
// sub_822B4F10) are recompiled, and the ramps are in guest memory
// (big-endian). They were VdGetCurrentDisplayGamma's only callers. See B3 in
// docs/rexglue-removal.md.

#include "guest.h"

#include "display/display.h"
#include "display/gamma.h"

// The 256-entry ramp: r3 = out, r4 = in (three channels of 256 16-bit
// values each; may be the same), r5 = inverse.
extern "C" REX_FUNC(sub_822B0910) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r3.u32, in = ctx.r4.u32;
  display::GammaRamp ramp;
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 256; i++) {
      ramp[c][i] = guest::LoadU16(base, in + 512 * c + 2 * i);
    }
  }
  const display::GammaRamp result = display::ApplyGammaCurve(
      display::MakeGammaCurve(display::kGamma, ctx.r5.u32 != 0), ramp);
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 256; i++) {
      guest::StoreU16(base, out + 512 * c + 2 * i, result[c][i]);
    }
  }
}

// The 128-entry piecewise-linear ramp: r3 = out, r4 = in (three channels
// of 128 (base, delta) pairs), r5 = inverse.
extern "C" REX_FUNC(sub_822B0AB0) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r3.u32, in = ctx.r4.u32;
  display::GammaRampPwl ramp;
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 128; i++) {
      ramp[c][i].base = guest::LoadU16(base, in + 512 * c + 4 * i);
      ramp[c][i].delta = guest::LoadU16(base, in + 512 * c + 4 * i + 2);
    }
  }
  const display::GammaRampPwl result = display::ApplyGammaCurve(
      display::MakeGammaCurve(display::kGamma, ctx.r5.u32 != 0), ramp);
  for (int c = 0; c < 3; c++) {
    for (int i = 0; i < 128; i++) {
      guest::StoreU16(base, out + 512 * c + 4 * i, result[c][i].base);
      guest::StoreU16(base, out + 512 * c + 4 * i + 2, result[c][i].delta);
    }
  }
}
