// XGraphics' surface compression's register steps (src/texture_compression/,
// docs/functions/texture-compression.md), shared by the adapters
// (src/hooks/texture_compression.cpp, behind --native_texture_compression)
// and the harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "texture_compression/codecs.h"

namespace hooks {

// The GuestCall the plain C++ makes its guest calls through: the function
// called as the originals call through ctr, with r1 the stack given and r3
// to r6 set; ctx's r1 put back after.
texture_compression::GuestCall TextureCompressionGuestCall(PPCContext& ctx, uint8_t* base);

// Dxt45::EncodeBlock sub_82422A68: r3 = the 16-byte block, r4 = 16 float4
// pixels, r5 = dither. r3 = 0.
void TextureCompressionEncodeDxt45Block(PPCContext& ctx, uint8_t* base);
// A8R8G8B8Codec::DecodeRow sub_823F2620: r3 = the codec, r4 = the row, r5 =
// the slice, r6 = the float4s. The whole original where the plain C++
// refuses (a color key or sRGB in). r3 as the original leaves it.
void TextureCompressionDecodeA8R8G8B8Row(PPCContext& ctx, uint8_t* base);
// DxtCodec::EncodeRow sub_823F6298: r3 = the codec, r4 = the row, r5 = the
// slice, r6 = the float4s; the codec's block functions other than
// Dxt45::EncodeBlock run their originals. The whole original where the plain
// C++ refuses, or without the native heap. r3 as the original leaves it.
void TextureCompressionDxtEncodeRow(PPCContext& ctx, uint8_t* base);
// The scalar deleting destructors sub_823F6B70 (plain codecs) and
// sub_823F6BC0 (DXT): r3 = the codec, r4 = flags (bit 0 deletes it). The
// whole original where the plain C++ refuses, or without the native heap. r3
// as the original leaves it.
void TextureCompressionImageCodecDelete(PPCContext& ctx, uint8_t* base);
void TextureCompressionDxtCodecDelete(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
