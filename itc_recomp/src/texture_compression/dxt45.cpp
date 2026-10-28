// The DXT4/5 block's encoder natively (docs/functions/texture-compression.md):
// Dxt45::EncodeBlock sub_82422A68 and Dxt45::EncodeBlockPremultiplied
// sub_82422960, step for step as the translated code does them (steps.h).

#include "texture_compression/dxt45.h"

#include "base/log.h"
#include "guest.h"
#include "texture_compression/dxt_alpha.h"
#include "texture_compression/dxt_color.h"
#include "texture_compression/steps.h"

namespace texture_compression {

using namespace steps;

namespace {

// The two halves on `pixels`: the color block first (r3 + 8, r5 = 0, r6 =
// dither; it always returns 0, so the alpha half always runs), then the
// alpha block (r3, the pixels, r5 = dither). `frame` is the caller's r1.
void EncodeHalves(uint8_t* base, const float pixels[16][4], int32_t dither, uint32_t frame,
                  uint8_t block[16]) {
  EncodeColorBlock(base, pixels, false, dither, frame, block + 8);
  float alphas[16];
  for (uint32_t i = 0; i < 16; i++) {
    alphas[i] = pixels[i][3];
  }
  EncodeAlphaBlock(base, alphas, dither, block);
}

}  // namespace

void EncodeDxt45BlockPremultiplied(uint8_t* base, const float pixels[16][4], int32_t dither,
                                   uint32_t stack, uint8_t block[16]) {
  ITC_NATIVE_RUNNING("sub_82422960 Dxt45::EncodeBlockPremultiplied");
  // The copy at r1 + 80: lfs alpha (f0), red (f13), green (f12), blue
  // (f11); fmuls f13 x f0, f12 x f0, f0 x f11; stfs each.
  float copy[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    const double a = double(pixels[i][3]);
    copy[i][0] = float(Fmuls(double(pixels[i][0]), a));
    copy[i][1] = float(Fmuls(double(pixels[i][1]), a));
    copy[i][2] = float(Fmuls(a, double(pixels[i][2])));
    copy[i][3] = float(a);
  }
  EncodeHalves(base, copy, dither, stack - kDxt45PremultipliedFrameBytes, block);
}

void EncodeDxt45Block(uint8_t* base, const float pixels[16][4], int32_t dither, uint32_t stack,
                      uint8_t block[16]) {
  ITC_NATIVE_RUNNING("sub_82422A68 Dxt45::EncodeBlock");
  const uint32_t frame = stack - kDxt45FrameBytes;
  if (guest::LoadU32(base, kPremultiplied) != 0) {
    EncodeDxt45BlockPremultiplied(base, pixels, dither, frame, block);
    return;
  }
  EncodeHalves(base, pixels, dither, frame, block);
}

}  // namespace texture_compression
