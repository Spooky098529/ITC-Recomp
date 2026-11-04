// The garage stage's scene lights (docs/functions/front-end-car-draw.md),
// step for step as the translated code (see front_end_car_draw.h): the
// scalar float work through doubles rounded to single, the spots'
// normalize on SSE vectors with flush-to-zero on, as translated
// (car_scene_draw/ops.h).

#include "front_end_car_draw/front_end_car_draw.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "car_scene_draw/ops.h"
#include "guest.h"

namespace front_end_car_draw {

namespace {

using namespace car_scene_draw::ops;

constexpr uint32_t kMinusOne = 0x82001BE0;  // -1.0
constexpr uint32_t kHalf = 0x82001BE4;      // 0.5
constexpr uint32_t kDegree = 0x82002948;    // π/180
constexpr uint32_t kThirdPi = 0x82009EB4;   // π/3

// The scene lights' constants (docs/known-data.md).
constexpr uint32_t kAmbient = 0x8265ED00;
constexpr uint32_t kLightA = 0x8296B950, kLightB = 0x8296B980, kLightC = 0x8296B9B0;
constexpr uint32_t kSpotPosition = 0x8296B9E0, kSpotColor = 0x8296BA60;
constexpr uint32_t kSpotA = 0x8296BAE0, kSpotB = 0x8296BB60;  // 128 bytes each
constexpr uint32_t kSpotDirection = 0x8296BBE0, kSpotCone = 0x8296BC60;

}  // namespace

uint32_t GarageStageSetLights(uint8_t* base, uint32_t lights, uint32_t mirrored,
                              uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_8217BA20 GarageStage::SetLights");
  const uint32_t f = stack - 368;
  Store(base, f, stack);
  const bool mirror = int32_t(mirrored) != 0;
  Copy16Interleaved(base, kAmbient, lights + 1072);
  const double minus_one = Lfs(base, kMinusOne);

  // The three lights: +32 (y negated when mirrored, through the frame's
  // +128), +48, +64.
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t light = lights + 32 + 96 * i;
    Copy16(base, f + 128, light);
    if (mirror) {
      Stfs(base, f + 132, Fmuls(Lfs(base, f + 132), minus_one));
    }
    Copy16(base, kLightA + 16 * i, f + 128);
    Copy16Interleaved(base, kLightB + 16 * i, light + 16);
    Copy16Interleaved(base, kLightC + 16 * i, light + 32);
  }

  // Two 128-byte blocks of 0.0 through the frame's +80 to +111.
  const double zero = Lfs(base, kZero);
  for (uint32_t k = 0; k < 8; k++) {
    Stfs(base, f + 80 + 4 * k, zero);
  }
  for (uint32_t at = 0; at < 128; at += 16) {
    const uint32_t a0 = Load(base, f + 80), a1 = Load(base, f + 84);
    const uint32_t b0 = Load(base, f + 96), b1 = Load(base, f + 100);
    const uint32_t a2 = Load(base, f + 88), a3 = Load(base, f + 92);
    const uint32_t b2 = Load(base, f + 104), b3 = Load(base, f + 108);
    Store(base, kSpotA + at, a0);
    Store(base, kSpotA + at + 4, a1);
    Store(base, kSpotB + at, b0);
    Store(base, kSpotB + at + 4, b1);
    Store(base, kSpotA + at + 8, a2);
    Store(base, kSpotA + at + 12, a3);
    Store(base, kSpotB + at + 8, b2);
    Store(base, kSpotB + at + 12, b3);
  }

  // The four spots (q = +304 + 96i).
  const double third_pi = Lfs(base, kThirdPi);
  const double degree = Lfs(base, kDegree);
  const double half = Lfs(base, kHalf);
  Stfs(base, f + 140, third_pi);
  const double one = Lfs(base, kOne);
  Stfs(base, f + 108, one);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t q = lights + 304 + 96 * i;
    Stfs(base, f + 124, one);
    Stfs(base, f + 156, one);
    Stfs(base, f + 92, one);
    const double inner = Frsp(c_runtime::Cos(Fmuls(Fmuls(Lfs(base, q + 64), half), degree)));
    const double outer = Frsp(c_runtime::Cos(Fmuls(Fmuls(Lfs(base, q + 68), half), degree)));
    const double outer_again = c_runtime::Cos(Fmuls(Fmuls(Lfs(base, q + 68), half), degree));
    const double inner_again = c_runtime::Cos(Fmuls(Fmuls(Lfs(base, q + 64), half), degree));
    // The direction q +0 to +8 less q +16 to +24 (as (q16 - q0) x -1.0).
    Stfs(base, f + 100, Fsubs(Lfs(base, q + 20), Lfs(base, q + 4)));
    Stfs(base, f + 104, Fsubs(Lfs(base, q + 24), Lfs(base, q + 8)));
    Stfs(base, f + 132, Frsp(outer_again));
    Stfs(base, f + 136, Fdivs(one, Fsubs(inner, outer)));
    const double x = Fsubs(Lfs(base, q + 16), Lfs(base, q + 0));
    Stfs(base, f + 96, x);
    Stfs(base, f + 128, Frsp(inner_again));
    Copy16(base, f + 80, f + 96);
    const uint32_t c0 = Load(base, q + 80), c1 = Load(base, q + 84);
    const uint32_t c2 = Load(base, q + 88), c3 = Load(base, q + 92);
    Stfs(base, f + 80, Fmuls(x, minus_one));
    Stfs(base, f + 84, Fmuls(Lfs(base, f + 84), minus_one));
    Store(base, f + 144, c0);
    Store(base, f + 148, c1);
    Stfs(base, f + 88, Fmuls(Lfs(base, f + 88), minus_one));
    const uint32_t p0 = Load(base, q + 0), p1 = Load(base, q + 4);
    Store(base, f + 152, c2);
    Store(base, f + 156, c3);
    const uint32_t p2 = Load(base, q + 8), p3 = Load(base, q + 12);
    Store(base, f + 112, p0);
    Store(base, f + 116, p1);
    {
      const FlushToZero flush;
      const simde__m128 v = Lvx(base, f + 80);
      Store(base, f + 120, p2);
      Store(base, f + 124, p3);
      Stvx(base, f + 80, Normalized(v, Dot3(v, v)));
    }
    if (mirror) {
      Stfs(base, f + 116, Fmuls(Lfs(base, f + 116), minus_one));
      Stfs(base, f + 84, Fmuls(Lfs(base, f + 84), minus_one));
    }
    Copy16(base, kSpotPosition + 16 * i, f + 112);
    Copy16(base, kSpotDirection + 16 * i, f + 80);
    Copy16Interleaved(base, kSpotA + 16 * i, q + 32);
    Copy16Interleaved(base, kSpotB + 16 * i, q + 48);
    Copy16(base, kSpotCone + 16 * i, f + 128);
    Copy16(base, kSpotColor + 16 * i, f + 144);
  }
  return f + 128;
}

}  // namespace front_end_car_draw
