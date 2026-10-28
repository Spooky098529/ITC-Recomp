// XGraphics' surface compression's register steps (src/texture_compression/,
// docs/functions/texture-compression.md), shared by the adapters
// (src/hooks/texture_compression.cpp, behind --native_texture_compression)
// and the harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

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
// ImageCodec::Prepare sub_823EAA90: r3 = the destination's codec, r4 = the
// source's. ImageCodec::Create sub_823F6C60: r3 = the description; the color
// key's slot 3 runs its original. Each the whole original where the plain
// C++ refuses, or without the native heap. r3 as the original leaves it.
void TextureCompressionPrepare(PPCContext& ctx, uint8_t* base);
void TextureCompressionCreate(PPCContext& ctx, uint8_t* base);
// Blitter::Blitter sub_82368D20: r3 = the blit object. r3 left.
void TextureCompressionBlitterConstruct(PPCContext& ctx, uint8_t* base);
// Blitter::Release sub_82368D30: r3 = the blit object. Blitter::CopyRows
// sub_82369860 and Blitter::ConvertRows sub_823699F8: r3 = the blit object.
// Blitter::Init sub_8236C630: r3 = the blit object, r4 = the destination's
// description, r5 the source's, r6 = the filter. Their callees that never ran
// in play (the filters, CopyBlocks, the YUV codec's functions) run as guest
// calls. Each the whole original where the plain C++ refuses, or (all but
// CopyRows) without the native heap. r3 as the original leaves it.
void TextureCompressionBlitterRelease(PPCContext& ctx, uint8_t* base);
void TextureCompressionCopyRows(PPCContext& ctx, uint8_t* base);
void TextureCompressionConvertRows(PPCContext& ctx, uint8_t* base);
void TextureCompressionBlitterInit(PPCContext& ctx, uint8_t* base);
// XGGetBlockDimensions sub_8236D3C8: r3 = a GPU format, r4, r5 = where the
// block's width and height go (null: not stored). r3 left.
void TextureCompressionGetBlockDimensions(PPCContext& ctx, uint8_t* base);
// XGCompressSurface sub_8235AAC0: r3 to r10, the caller's +84, +92 and +100,
// f1. The whole original for the paths never seen in play (DXN and CTX1
// destinations, a source copied first, a destination swapped after), or
// without the native heap. r3 as the original leaves it.
void TextureCompressionCompressSurface(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
