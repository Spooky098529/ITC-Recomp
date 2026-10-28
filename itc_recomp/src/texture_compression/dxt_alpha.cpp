// The DXT alpha block's encoder natively (docs/functions/texture-compression.md):
// DxtAlpha::FitEndpoints sub_824207D8 and DxtAlpha::EncodeBlock sub_82422178,
// step for step as the translated code does them (steps.h).

#include "texture_compression/dxt_alpha.h"

#include "base/log.h"
#include "texture_compression/steps.h"

#include <cstring>

namespace texture_compression {

using namespace steps;

namespace {

// The game's constants (lfs from .rdata).
constexpr uint32_t kOne = 0x82000A30;          // 1.0
constexpr uint32_t kZero = 0x82000A34;         // 0.0
constexpr uint32_t kHalf = 0x82001BE4;         // probably 0.5
constexpr uint32_t k255 = 0x82000C44;          // probably 255.0
constexpr uint32_t kInv255 = 0x82002200;       // probably 1/255
constexpr uint32_t kInv5 = 0x82002208;         // probably 1/5
constexpr uint32_t kInv7 = 0x8203B7B0;         // probably 1/7
constexpr uint32_t kRangeMin = 0x82008058;     // FitEndpoints' smallest range
constexpr uint32_t kGradientMin = 0x8201607C;  // FitEndpoints' squared step to stop
// The error diffusion's weights: right, below left, below, below right
// (probably 7/16, 3/16, 5/16, 1/16).
constexpr uint32_t kWeightRight = 0x8203EB04;
constexpr uint32_t kWeightBelowLeft = 0x8203B78C;
constexpr uint32_t kWeightBelow = 0x8203B790;
constexpr uint32_t kWeightBelowRight = 0x820093A0;
// FitEndpoints' weights of the low and high ends for each step.
constexpr uint32_t kLowWeights6 = 0x8203EA90;
constexpr uint32_t kHighWeights6 = 0x8203EA78;
constexpr uint32_t kLowWeights8 = 0x8203EA58;
constexpr uint32_t kHighWeights8 = 0x8203EA38;
// EncodeBlock's step to index tables (words), ordered from the low end.
constexpr uint32_t kIndices6 = 0x8203EB28;
constexpr uint32_t kIndices8 = 0x8203EB08;

}  // namespace

void FitAlphaEndpoints(uint8_t* base, const float values[16], uint32_t steps, float* low,
                       float* high) {
  ITC_NATIVE_RUNNING("sub_824207D8 DxtAlpha::FitEndpoints");
  const uint32_t low_weights = steps == 6 ? kLowWeights6 : kLowWeights8;     // r7
  const uint32_t high_weights = steps == 6 ? kHighWeights6 : kHighWeights8;  // r29
  const double one = Lfs(base, kOne);    // f30
  const double zero = Lfs(base, kZero);  // f4
  double lo = one;                       // f7
  double hi = zero;                      // f6
  if (steps == 8) {
    for (int i = 0; i < 16; i++) {
      const double v = values[i];
      if (v < lo) {
        lo = v;
      }
      if (v > hi) {
        hi = v;
      }
    }
  } else {
    for (int i = 0; i < 16; i++) {
      const double v = values[i];
      if (v < lo && v > zero) {
        lo = v;
      }
      if (v > hi && v < one) {
        hi = v;
      }
    }
    if (lo == hi) {
      hi = one;
    }
  }

  const uint32_t last = steps - 1;                       // r31
  const double gradient_min = Lfs(base, kGradientMin);   // f1
  const double last_f = IntToSingle(int64_t(last));      // f31
  const double half = Lfs(base, kHalf);                  // f3
  const double range_min = Lfs(base, kRangeMin);         // f2
  double points[8] = {};  // the steps' values (the original's r1 - 80)
  for (uint32_t pass = 0; pass < 8; pass++) {
    const double range = Fsubs(hi, lo);
    if (range < range_min) {
      break;
    }
    const double scale = Fdivs(last_f, range);  // f5
    for (uint32_t k = 0; k < steps; k++) {
      const double l = Fmuls(Lfs(base, low_weights + 4 * k), lo);
      points[k] = double(float(Fmadds(Lfs(base, high_weights + 4 * k), hi, l)));
    }
    double low_sq = zero;   // f11
    double high_sq = zero;  // f10
    double low_err = zero;  // f9
    double high_err = zero; // f8
    for (int i = 0; i < 16; i++) {
      const double v = values[i];  // f13
      const double t = Fmuls(Fsubs(v, lo), scale);
      uint32_t index;
      if (!(t > zero)) {
        if (steps == 6 && !(v > Fmuls(lo, half))) {
          continue;
        }
        index = 0;
      } else if (!(t < last_f)) {
        if (steps == 6 && !(v < Fmuls(Fadds(hi, one), half))) {
          continue;
        }
        index = last;
      } else {
        index = uint32_t(Fctiwz(Fadds(t, half)));
      }
      if (!(index < steps)) {
        continue;
      }
      const double lw = Lfs(base, (index << 2) + low_weights);
      const double point = points[index];
      low_sq = Fmadds(lw, lw, low_sq);
      const double d = Fsubs(v, point);
      const double hw = Lfs(base, (index << 2) + high_weights);
      high_sq = Fmadds(hw, hw, high_sq);
      low_err = Fmadds(lw, d, low_err);
      high_err = Fmadds(hw, d, high_err);
    }
    if (low_sq > zero) {
      lo = Fsubs(lo, Fdivs(low_err, low_sq));
    }
    if (high_sq > zero) {
      hi = Fsubs(hi, Fdivs(high_err, high_sq));
    }
    if (lo > hi) {
      const double swap = lo;
      lo = hi;
      hi = swap;
    }
    if (Fmuls(low_err, low_err) < gradient_min && Fmuls(high_err, high_err) < gradient_min) {
      break;
    }
  }
  const double lo_out = lo < zero ? zero : (lo > one ? one : lo);
  *low = float(lo_out);
  const double hi_out = hi < zero ? zero : (hi > one ? one : hi);
  *high = float(hi_out);
}

namespace {

// Spreads a pixel's error to the pixels right of and below it (4 x 4, row
// by row), as both of EncodeBlock's passes do.
void Diffuse(float err[16], uint32_t i, double e, double right, double below_left, double below,
             double below_right) {
  const uint32_t column = i & 3;
  if (column != 3) {
    err[i + 1] = float(Fmadds(e, right, err[i + 1]));
  }
  if (i < 12) {
    if (column != 0) {
      err[i + 3] = float(Fmadds(e, below_left, err[i + 3]));
    }
    err[i + 4] = float(Fmadds(e, below, err[i + 4]));
    if (column != 3) {
      err[i + 5] = float(Fmadds(e, below_right, err[i + 5]));
    }
  }
}

}  // namespace

void EncodeAlphaBlock(uint8_t* base, const float alphas[16], int32_t dither, uint8_t block[8]) {
  ITC_NATIVE_RUNNING("sub_82422178 DxtAlpha::EncodeBlock");
  const double w_below_right = Lfs(base, kWeightBelowRight);  // f23
  const double w_below = Lfs(base, kWeightBelow);             // f24
  const double w_below_left = Lfs(base, kWeightBelowLeft);    // f25
  const double w_right = Lfs(base, kWeightRight);             // f26
  const double inv255 = Lfs(base, kInv255);                   // f30
  const double half = Lfs(base, kHalf);                       // f29
  const double scale255 = Lfs(base, k255);                    // f31

  // The alphas rounded to 1/255 steps (with the error carried when
  // dithering), and their range; both ends start at the first raw alpha.
  float err[16] = {};       // r1 + 160, zeroed when dithering
  float quantized[16];      // r1 + 96
  double mn = alphas[0];    // f12
  double mx = mn;           // f11
  for (uint32_t i = 0; i < 16; i++) {
    double a = alphas[i];
    if (dither != 0) {
      a = Fadds(double(err[i]), a);
    }
    const int32_t n = Fctiwz(Fmadds(a, scale255, half));
    const double q = Fmuls(IntToSingle(n), inv255);
    quantized[i] = float(q);
    if (q < mn) {
      mn = q;
    } else if (q > mx) {
      mx = q;
    }
    if (dither != 0) {
      Diffuse(err, i, Fsubs(a, q), w_right, w_below_left, w_below, w_below_right);
    }
  }

  const double one = Lfs(base, kOne);  // f27
  auto solid = [&](uint8_t first, uint8_t second) {
    block[1] = second;
    block[0] = first;
    std::memset(block + 2, 0, 6);
  };
  if (mn == one) {
    solid(255, 255);
    return;
  }
  const double zero = Lfs(base, kZero);  // f28
  const uint32_t steps = (mn == zero || mx == one) ? 6 : 8;

  float lo_fit;
  float hi_fit;
  FitAlphaEndpoints(base, quantized, steps, &lo_fit, &hi_fit);
  const uint32_t lo_byte = uint32_t(Fctiwz(Fmadds(lo_fit, scale255, half))) & 0xFF;
  const uint32_t hi_byte = uint32_t(Fctiwz(Fmadds(hi_fit, scale255, half))) & 0xFF;
  const double lo_value = Fmuls(IntToSingle(int32_t(lo_byte)), inv255);
  const double hi_value = Fmuls(IntToSingle(int32_t(hi_byte)), inv255);

  // The palette (r1 + 96, over the rounded alphas): its two ends, then the
  // steps between them.
  float palette[8];
  uint32_t indices;
  if (steps == 8) {
    if (lo_byte == hi_byte) {
      solid(uint8_t(lo_byte), uint8_t(hi_byte));
      return;
    }
    block[1] = uint8_t(lo_byte);
    palette[1] = float(lo_value);
    palette[0] = float(hi_value);
    block[0] = uint8_t(hi_byte);
    const double inv7 = Lfs(base, kInv7);
    for (uint32_t k = 1; k < 7; k++) {
      const double h = Fmuls(IntToSingle(int64_t(k)), double(palette[1]));
      const double p = Fmadds(IntToSingle(int64_t(7 - k)), double(palette[0]), h);
      palette[k + 1] = float(Fmuls(p, inv7));
    }
    indices = kIndices8;
  } else {
    block[0] = uint8_t(lo_byte);
    palette[0] = float(lo_value);
    palette[1] = float(hi_value);
    block[1] = uint8_t(hi_byte);
    const double inv5 = Lfs(base, kInv5);
    for (uint32_t k = 1; k < 5; k++) {
      const double h = Fmuls(IntToSingle(int64_t(k)), double(palette[1]));
      const double p = Fmadds(IntToSingle(int64_t(5 - k)), double(palette[0]), h);
      palette[k + 1] = float(Fmuls(p, inv5));
    }
    palette[6] = float(zero);
    palette[7] = float(one);
    indices = kIndices6;
  }

  // Each alpha's step (3 bits, the first pixel lowest), eight pixels to three
  // bytes, the error carried again when dithering.
  const double first = palette[0];                               // f12
  const double second = palette[1];                              // f10
  const double last_f = IntToSingle(int64_t(uint32_t(steps - 1)));  // f9
  const double scale = first == second ? zero : Fdivs(last_f, Fsubs(second, first));  // f11
  if (dither != 0) {
    std::memset(err, 0, sizeof(err));
  }
  for (uint32_t group = 0; group < 2; group++) {
    uint32_t bits = 0;
    for (uint32_t i = 8 * group; i < 8 * group + 8; i++) {
      double a = alphas[i];
      if (dither != 0) {
        a = Fadds(double(err[i]), a);
      }
      const double t = Fmuls(Fsubs(a, first), scale);
      uint32_t index;
      if (!(t > zero)) {
        index = (steps == 6 && !(a > Fmuls(first, half))) ? 6 : 0;
      } else if (!(t < last_f)) {
        index = (steps == 6 && !(a < Fmuls(Fadds(second, one), half))) ? 7 : 1;
      } else {
        const int32_t n = Fctiwz(Fadds(t, half));
        index = guest::LoadU32(base, (uint32_t(n) << 2) + indices);
      }
      bits = (index << 21) | (bits >> 3);
      if (dither != 0) {
        if (index >= 8) {
          // The tables hold 0 to 7; the original would read past the palette.
          ITC_LOG_ERROR("DxtAlpha::EncodeBlock: step index {} out of the palette", index);
          index &= 7;
        }
        Diffuse(err, i, Fsubs(a, double(palette[index])), w_right, w_below_left, w_below,
                w_below_right);
      }
    }
    block[2 + 3 * group] = uint8_t(bits);
    block[3 + 3 * group] = uint8_t(bits >> 8);
    block[4 + 3 * group] = uint8_t(bits >> 16);
  }
  // Each halfword's bytes swapped (the 360's 8-in-16 order).
  for (int i = 0; i < 8; i += 2) {
    const uint8_t swap = block[i];
    block[i] = block[i + 1];
    block[i + 1] = swap;
  }
}

}  // namespace texture_compression
