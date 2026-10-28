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

}  // namespace hooks
