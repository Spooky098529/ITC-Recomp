// XGraphics' surface compression natively: adapters in front of
// src/texture_compression/ (docs/functions/texture-compression.md), with the
// register steps the harness shares (hooks/texture_compression.h). The
// codecs' row functions and destructors are reached through their vtables,
// the DXT4/5 block's encoder through the DXT codec's +144. The
// helpers' adapters (DxtAlpha::FitEndpoints, DxtColor::PackR5G6B5 and
// DxtColor::FitEndpoints) went once their callers were native: the encoders'
// plain C++ calls them directly.

#include "hooks/texture_compression.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "texture_compression/codecs.h"
#include "texture_compression/dxt45.h"
#include "texture_compression/dxt_alpha.h"
#include "texture_compression/dxt_color.h"

ITC_FLAG_BOOL(native_texture_compression, true, "ITC",
              "Run XGraphics' surface compression (docs/functions/texture-compression.md: "
              "the DXT alpha block's encoder, DxtAlpha::EncodeBlock sub_82422178 with "
              "DxtAlpha::FitEndpoints sub_824207D8; the DXT color block's helpers, "
              "DxtColor::PackR5G6B5 sub_824206E8 and DxtColor::FitEndpoints sub_82420AB8; "
              "the DXT color block's encoder, DxtColor::EncodeBlock sub_82420F50; the "
              "DXT4/5 block's encoder, Dxt45::EncodeBlock sub_82422A68 with "
              "EncodeBlockPremultiplied sub_82422960; the A8R8G8B8 codec's row decode, "
              "sub_823F2620, and the DXT codec's row encode, DxtCodec::EncodeRow "
              "sub_823F6298, their never-seen paths (a color key, sRGB, ConvertRange) the "
              "originals; the codecs' destructors sub_823EA070, sub_823EA770, sub_823F6B70 "
              "and sub_823F6BC0, with the native heap; the "
              "helpers, Native, are reached only through the encoders) "
              "natively instead of through the "
              "translated code (the same block bytes; checked by "
              "--run_harnesses=texture_compression)");

namespace hooks {

namespace tc = texture_compression;

void TextureCompressionEncodeAlphaBlock(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  float alphas[16];
  for (uint32_t i = 0; i < 16; i++) {
    alphas[i] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + tc::kAlphaOffset);
  }
  uint8_t block[8];
  tc::EncodeAlphaBlock(base, alphas, ctx.r5.s32, block);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

void TextureCompressionEncodeColorBlock(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  // stw r6,1084(r1) and stw r7,1060(r1) past its 1040-byte frame: the
  // caller's slots for r6 and r3.
  guest::StoreU32(base, ctx.r1.u32 + 44, ctx.r6.u32);
  guest::StoreU32(base, ctx.r1.u32 + 20, ctx.r3.u32);
  float pixels[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      pixels[i][c] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  uint8_t block[8];
  tc::EncodeColorBlock(base, pixels, ctx.r5.u32 != 0, ctx.r6.s32, ctx.r1.u32, block);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

namespace {

using EncodeDxt45 = void (*)(uint8_t*, const float[16][4], int32_t, uint32_t, uint8_t[16]);

void EncodeDxt45With(PPCContext& ctx, uint8_t* base, EncodeDxt45 encode) {
  ctx.fpscr.disableFlushMode();
  float pixels[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      pixels[i][c] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  uint8_t block[16];
  encode(base, pixels, ctx.r5.s32, ctx.r1.u32, block);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

}  // namespace

void TextureCompressionEncodeDxt45Block(PPCContext& ctx, uint8_t* base) {
  EncodeDxt45With(ctx, base, tc::EncodeDxt45Block);
}

void TextureCompressionEncodeDxt45BlockPremultiplied(PPCContext& ctx, uint8_t* base) {
  EncodeDxt45With(ctx, base, tc::EncodeDxt45BlockPremultiplied);
}

void TextureCompressionDecodeA8R8G8B8Row(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto r3 =
      tc::DecodeA8R8G8B8Row(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  if (!r3) {
    __imp__sub_823F2620(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void TextureCompressionDxtEncodeRow(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_823F6298(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  // The codec's block functions (+140, +144) as the original calls them
  // through ctr.
  auto call = [&](uint32_t function, uint32_t stack, uint32_t r3, uint32_t r4,
                  uint32_t r5) -> uint32_t {
    const uint64_t saved = ctx.r1.u64;
    ctx.r1.u64 = stack;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = saved;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  };
  const auto r3 = tc::DxtEncodeRow(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                   ctx.r1.u32, call);
  if (!r3) {
    __imp__sub_823F6298(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

namespace {

void DestructWith(PPCContext& ctx, uint8_t* base,
                  std::optional<uint32_t> (*destruct)(uint8_t*, uint32_t),
                  void (*original)(PPCContext&, uint8_t*)) {
  const auto r3 = NativeHeapActive() ? destruct(base, ctx.r3.u32) : std::nullopt;
  if (!r3) {
    original(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void DeleteWith(PPCContext& ctx, uint8_t* base,
                std::optional<uint32_t> (*destruct)(uint8_t*, uint32_t, uint32_t),
                void (*original)(PPCContext&, uint8_t*)) {
  const auto r3 = NativeHeapActive() ? destruct(base, ctx.r3.u32, ctx.r4.u32) : std::nullopt;
  if (!r3) {
    original(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

}  // namespace

void TextureCompressionImageCodecDestruct(PPCContext& ctx, uint8_t* base) {
  DestructWith(ctx, base, tc::ImageCodecDestruct, __imp__sub_823EA070);
}

void TextureCompressionDxtCodecDestruct(PPCContext& ctx, uint8_t* base) {
  DestructWith(ctx, base, tc::DxtCodecDestruct, __imp__sub_823EA770);
}

void TextureCompressionImageCodecDelete(PPCContext& ctx, uint8_t* base) {
  DeleteWith(ctx, base, tc::ImageCodecDelete, __imp__sub_823F6B70);
}

void TextureCompressionDxtCodecDelete(PPCContext& ctx, uint8_t* base) {
  DeleteWith(ctx, base, tc::DxtCodecDelete, __imp__sub_823F6BC0);
}

}  // namespace hooks

#define ITC_TEXTURE_COMPRESSION_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {                 \
    REX_FUNC_PROLOGUE();                               \
    if (!ITC_FLAG(native_texture_compression)) {       \
      __imp__sub_##address(ctx, base);                 \
      return;                                          \
    }                                                  \
    hooks::step(ctx, base);                            \
  }

ITC_TEXTURE_COMPRESSION_ADAPTER(82422178, TextureCompressionEncodeAlphaBlock)
ITC_TEXTURE_COMPRESSION_ADAPTER(82420F50, TextureCompressionEncodeColorBlock)
ITC_TEXTURE_COMPRESSION_ADAPTER(82422A68, TextureCompressionEncodeDxt45Block)
ITC_TEXTURE_COMPRESSION_ADAPTER(82422960, TextureCompressionEncodeDxt45BlockPremultiplied)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F2620, TextureCompressionDecodeA8R8G8B8Row)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6298, TextureCompressionDxtEncodeRow)
ITC_TEXTURE_COMPRESSION_ADAPTER(823EA070, TextureCompressionImageCodecDestruct)
ITC_TEXTURE_COMPRESSION_ADAPTER(823EA770, TextureCompressionDxtCodecDestruct)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6B70, TextureCompressionImageCodecDelete)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6BC0, TextureCompressionDxtCodecDelete)
