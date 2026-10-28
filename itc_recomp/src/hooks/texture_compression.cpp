// XGraphics' surface compression natively: adapters in front of
// src/texture_compression/ (docs/functions/texture-compression.md), with the
// register steps the harness shares (hooks/texture_compression.h). The
// codecs are made by ImageCodec::Create, the constructors and Prepare,
// called by the recompiled Blitter::Init (and Create's by its compare tree);
// their row functions and destructors are reached through their vtables,
// the DXT4/5 block's encoder through the DXT codec's +144. The adapters of
// the functions reached only from native callers went (DxtAlpha::
// FitEndpoints and EncodeBlock, DxtColor::PackR5G6B5, FitEndpoints and
// EncodeBlock, Dxt45::EncodeBlockPremultiplied, ImageCodec::~ImageCodec,
// DxtCodec::~DxtCodec): their callers' plain C++ calls theirs directly.

#include "hooks/texture_compression.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "texture_compression/codecs.h"
#include "texture_compression/dxt45.h"
#include "texture_compression/dxt_alpha.h"

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
              "and sub_823F6BC0, with the native heap; the codecs made, ImageCodec::Create "
              "sub_823F6C60 (its YUV formats the original), ImageCodec::ImageCodec "
              "sub_823EA800, ImageCodec::Prepare sub_823EAA90 and DxtCodec::DxtCodec "
              "sub_823F5FA0, with the native heap; the functions marked Native there "
              "are reached only through their native callers, and this switches those) "
              "natively instead of through the "
              "translated code (the same block bytes; checked by "
              "--run_harnesses=texture_compression)");

namespace hooks {

namespace tc = texture_compression;

void TextureCompressionEncodeDxt45Block(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  float pixels[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      pixels[i][c] = guest::LoadFloat(base, ctx.r4.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  uint8_t block[16];
  tc::EncodeDxt45Block(base, pixels, ctx.r5.s32, ctx.r1.u32, block);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(base, ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
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

namespace {

// A guest function called as the originals call through ctr, at `stack`.
tc::GuestCall CallThroughCtr(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t function, uint32_t stack, uint32_t r3, uint32_t r4,
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
}

}  // namespace

void TextureCompressionDxtEncodeRow(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_823F6298(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  // The codec's block functions (+140, +144) as the original calls them
  // through ctr.
  const tc::GuestCall call = CallThroughCtr(ctx, base);
  const auto r3 = tc::DxtEncodeRow(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                   ctx.r1.u32, call);
  if (!r3) {
    __imp__sub_823F6298(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

namespace {

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

void TextureCompressionImageCodecDelete(PPCContext& ctx, uint8_t* base) {
  DeleteWith(ctx, base, tc::ImageCodecDelete, __imp__sub_823F6B70);
}

void TextureCompressionDxtCodecDelete(PPCContext& ctx, uint8_t* base) {
  DeleteWith(ctx, base, tc::DxtCodecDelete, __imp__sub_823F6BC0);
}

void TextureCompressionImageCodecConstruct(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> r3;
  if (NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    r3 = tc::ImageCodecConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  }
  if (!r3) {
    __imp__sub_823EA800(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void TextureCompressionPrepare(PPCContext& ctx, uint8_t* base) {
  const auto r3 =
      NativeHeapActive() ? tc::PrepareCodecs(base, ctx.r3.u32, ctx.r4.u32) : std::nullopt;
  if (!r3) {
    __imp__sub_823EAA90(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void TextureCompressionDxtCodecConstruct(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> r3;
  if (NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    r3 = tc::DxtCodecConstruct(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!r3) {
    __imp__sub_823F5FA0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void TextureCompressionCreate(PPCContext& ctx, uint8_t* base) {
  if (!NativeHeapActive()) {
    __imp__sub_823F6C60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const auto r3 = tc::CreateCodec(base, ctx.r3.u32, ctx.r1.u32, CallThroughCtr(ctx, base));
  if (!r3) {
    __imp__sub_823F6C60(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
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

ITC_TEXTURE_COMPRESSION_ADAPTER(82422A68, TextureCompressionEncodeDxt45Block)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F2620, TextureCompressionDecodeA8R8G8B8Row)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6298, TextureCompressionDxtEncodeRow)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6B70, TextureCompressionImageCodecDelete)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6BC0, TextureCompressionDxtCodecDelete)
ITC_TEXTURE_COMPRESSION_ADAPTER(823EA800, TextureCompressionImageCodecConstruct)
ITC_TEXTURE_COMPRESSION_ADAPTER(823EAA90, TextureCompressionPrepare)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F5FA0, TextureCompressionDxtCodecConstruct)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6C60, TextureCompressionCreate)
