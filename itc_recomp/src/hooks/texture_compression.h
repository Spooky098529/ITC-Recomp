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
// Dxt45::EncodeBlock sub_82422A68 and Dxt45::EncodeBlockPremultiplied
// sub_82422960: r3 = the 16-byte block, r4 = 16 float4 pixels, r5 = dither.
// r3 = 0.
void TextureCompressionEncodeDxt45Block(PPCContext& ctx, uint8_t* base);
void TextureCompressionEncodeDxt45BlockPremultiplied(PPCContext& ctx, uint8_t* base);
// A8R8G8B8Codec::DecodeRow sub_823F2620: r3 = the codec, r4 = the row, r5 =
// the slice, r6 = the float4s. The whole original where the plain C++
// refuses (a color key or sRGB in). r3 as the original leaves it.
void TextureCompressionDecodeA8R8G8B8Row(PPCContext& ctx, uint8_t* base);
// DxtCodec::EncodeRow sub_823F6298: r3 = the codec, r4 = the row, r5 = the
// slice, r6 = the float4s; the codec's block functions other than
// Dxt45::EncodeBlock run their originals. The whole original where the plain
// C++ refuses, or without the native heap. r3 as the original leaves it.
void TextureCompressionDxtEncodeRow(PPCContext& ctx, uint8_t* base);
// ImageCodec::~ImageCodec sub_823EA070 and DxtCodec::~DxtCodec sub_823EA770:
// r3 = the codec; the scalar deleting destructors sub_823F6B70 (plain codecs)
// and sub_823F6BC0 (DXT): r3 = the codec, r4 = flags (bit 0 deletes it). The
// whole original where the plain C++ refuses, or without the native heap. r3
// as the original leaves it.
void TextureCompressionImageCodecDestruct(PPCContext& ctx, uint8_t* base);
void TextureCompressionDxtCodecDestruct(PPCContext& ctx, uint8_t* base);
void TextureCompressionImageCodecDelete(PPCContext& ctx, uint8_t* base);
void TextureCompressionDxtCodecDelete(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
