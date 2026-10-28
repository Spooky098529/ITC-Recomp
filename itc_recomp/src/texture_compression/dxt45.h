// The DXT4/5 block's encoder (16 bytes: the alpha block, then the color
// block), from XGraphics' surface compression
// (docs/functions/texture-compression.md). Plain C++ over host arrays; it
// calls the two halves' plain C++ (dxt_alpha.h, dxt_color.h) directly.

#pragma once

#include <cstdint>

namespace texture_compression {

// Dxt45::EncodeBlock's premultiplied switch (XGCompressSurface's flags 1).
constexpr uint32_t kPremultiplied = 0x82954660;

// Dxt45::EncodeBlock's and EncodeBlockPremultiplied's stack frames.
constexpr uint32_t kDxt45FrameBytes = 112;
constexpr uint32_t kDxt45PremultipliedFrameBytes = 368;

// sub_82422960 Dxt45::EncodeBlockPremultiplied: the 16 float4 pixels' red,
// green and blue times their alpha (red x alpha, green x alpha, alpha x blue,
// rounded to single, the alpha kept), then the color half (EncodeColorBlock,
// no 1-bit alpha) into `block` + 8 and the alpha half (EncodeAlphaBlock) into
// `block`. `stack` is the guest r1 at its entry. The original returns 0 (its
// copy of the pixels, at its r1 + 80, is below its caller's stack pointer and
// isn't kept).
void EncodeDxt45BlockPremultiplied(uint8_t* base, const float pixels[16][4], int32_t dither,
                                   uint32_t stack, uint8_t block[16]);

// sub_82422A68 Dxt45::EncodeBlock: with the word at kPremultiplied set,
// EncodeDxt45BlockPremultiplied; else the color half (no 1-bit alpha) into
// `block` + 8 and the alpha half into `block`, on the pixels as they are.
// `stack` is the guest r1 at its entry; `block` the 16 bytes in the guest's
// order. The original returns 0 and writes nothing at or above `stack`.
void EncodeDxt45Block(uint8_t* base, const float pixels[16][4], int32_t dither, uint32_t stack,
                      uint8_t block[16]);

}  // namespace texture_compression
