// The DXT alpha block's encoder (DXT4/5's first 8 bytes and DXT5A), from
// XGraphics' surface compression (docs/functions/texture-compression.md).
// Plain C++ over host arrays; the game's constants and tables are read from
// guest memory as the original reads them.

#pragma once

#include <cstdint>

namespace texture_compression {

// The 16-byte float4 pixels' stride and the alpha's offset in each.
constexpr uint32_t kPixelBytes = 16;
constexpr uint32_t kAlphaOffset = 12;

// sub_824207D8 DxtAlpha::FitEndpoints (probably): the 16 values' range (for
// 6 steps only the values strictly inside 0-1, the top one set to 1.0 when
// both ends meet), refined up to 8 times by least squares against the steps
// (tables 0x8203EA90 and 0x8203EA78 for 6, 0x8203EA58 and 0x8203EA38 for
// 8), then clamped to 0-1. `steps` is 6 or 8 (the adapter hands any other
// count to the original).
void FitAlphaEndpoints(uint8_t* base, const float values[16], uint32_t steps, float* low,
                       float* high);

// sub_82422178 DxtAlpha::EncodeBlock: the 16 alphas (pixel +12) into the 8
// bytes at `block` (the guest's bytes, in order), with Floyd-Steinberg error
// diffusion over the 4 x 4 block when `dither` is nonzero. The original
// returns 0.
void EncodeAlphaBlock(uint8_t* base, const float alphas[16], int32_t dither, uint8_t block[8]);

}  // namespace texture_compression
