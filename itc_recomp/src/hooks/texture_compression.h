// XGraphics' surface compression's register steps (src/texture_compression/,
// docs/functions/texture-compression.md), shared by the adapters
// (src/hooks/texture_compression.cpp, behind --native_texture_compression)
// and the harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// DxtAlpha::EncodeBlock sub_82422178: r3 = the 8-byte block, r4 = 16 float4
// pixels (their alphas at +12), r5 = dither (nonzero diffuses the error).
// r3 = 0.
void TextureCompressionEncodeAlphaBlock(PPCContext& ctx, uint8_t* base);
// DxtColor::EncodeBlock sub_82420F50: r3 = the 8-byte block, r4 = 16 float4
// pixels, r5 = 1-bit alpha (nonzero), r6 = dither (nonzero). r6 and r3 also
// stored in their slots of the caller's frame (r1 + 44, r1 + 20) as the
// original does. r3 = 0.
void TextureCompressionEncodeColorBlock(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
