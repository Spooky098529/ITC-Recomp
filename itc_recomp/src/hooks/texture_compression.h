// XGraphics' surface compression's register steps (src/texture_compression/,
// docs/functions/texture-compression.md), shared by the adapters
// (src/hooks/texture_compression.cpp, behind --native_texture_compression)
// and the harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// DxtAlpha::FitEndpoints sub_824207D8: r3 = the low end's float, r4 = the
// high end's, r5 = 16 floats, r6 = the steps (6 or 8; the original for any
// other count). r3 left.
void TextureCompressionFitAlphaEndpoints(PPCContext& ctx, uint8_t* base);
// DxtAlpha::EncodeBlock sub_82422178: r3 = the 8-byte block, r4 = 16 float4
// pixels (their alphas at +12), r5 = dither (nonzero diffuses the error).
// r3 = 0.
void TextureCompressionEncodeAlphaBlock(PPCContext& ctx, uint8_t* base);
// DxtColor::PackR5G6B5 sub_824206E8: r3 = a float4 color (red, green, blue
// read). r3 = the 5:6:5 color.
void TextureCompressionPackR5G6B5(PPCContext& ctx, uint8_t* base);
// DxtColor::FitEndpoints sub_82420AB8: r3 = the low end's three floats, r4
// the high end's, r5 = 16 float4 pixels, r6 = the colors (3 or 4; the
// original for any other count). r3, r4 and r6 left, r5 + 8 (as the
// original leaves it).
void TextureCompressionFitColorEndpoints(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
