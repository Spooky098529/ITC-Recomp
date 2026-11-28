// XGraphics' surface compression natively: adapters in front of
// src/texture_compression/ (docs/functions/texture-compression.md), with the
// register steps the harness shares (hooks/texture_compression.h). The
// codecs are made by ImageCodec::Create and Prepare, called by
// Blitter::Init's plain C++ (Create calls the constructors' plain C++), which
// XGCompressSurface's calls (from Vinyl::Compress's plain C++);
// the codecs' row functions and destructors are reached through their
// vtables (from the blitter's plain C++, directly when the slots are ours),
// the DXT4/5 block's encoder through the DXT codec's +144. The adapters of
// the functions reached only from native callers went (DxtAlpha::
// FitEndpoints and EncodeBlock, DxtColor::PackR5G6B5, FitEndpoints and
// EncodeBlock, Dxt45::EncodeBlockPremultiplied, ImageCodec::~ImageCodec,
// DxtCodec::~DxtCodec, ImageCodec::ImageCodec, DxtCodec::DxtCodec, then
// ImageCodec::Create, ImageCodec::Prepare, Blitter::Blitter, Release, Init,
// CopyRows, ConvertRows, XGGetBlockDimensions and XGCompressSurface, whose
// only caller in play, Vinyl::Compress, is native in src/car_models/): their
// callers' plain C++ calls theirs directly. Those of the functions reached
// through the codecs' vtables and the DXT codec's +144 stay.

#include "hooks/texture_compression.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "texture_compression/blitter.h"
#include "texture_compression/codecs.h"
#include "texture_compression/compress_surface.h"
#include "texture_compression/dxt45.h"
#include "texture_compression/dxt_alpha.h"
#include "texture_compression/dxt_color.h"
#include "lifted/sdk.h"

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
              "sub_823F5FA0, with the native heap; the blitter, Blitter::Blitter "
              "sub_82368D20, Blitter::Release sub_82368D30, Blitter::Init sub_8236C630 (the "
              "filters, never seen in play, the originals), Blitter::CopyRows sub_82369860 "
              "(its CopyBlocks the original) and Blitter::ConvertRows sub_823699F8, with "
              "XGGetBlockDimensions sub_8236D3C8 and XGCompressSurface sub_8235AAC0 (its "
              "DXN, CTX1, source copy and destination swap paths, never seen in play, the "
              "original whole; reached only from Vinyl::Compress's plain C++, which "
              "--native_car_models switches), with the native heap; the functions marked "
              "Native there are reached only through their native callers, and this "
              "switches those) "
              "natively instead of through the "
              "translated code (the same block bytes; checked by "
              "--run_harnesses=texture_compression)");

namespace hooks {

namespace tc = texture_compression;

ITC_REGS_STEP(TextureCompressionEncodeDxt45Block) {
  ctx.fpscr.disableFlushMode();
  float pixels[16][4];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 4; c++) {
      pixels[i][c] = guest::LoadFloat(ctx.r4.u32 + tc::kPixelBytes * i + 4 * c);
    }
  }
  uint8_t block[16];
  tc::EncodeDxt45Block(pixels, ctx.r5.s32, ctx.r1.u32, block);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU8(ctx.r3.u32 + i, block[i]);
  }
  ctx.r3.u64 = 0;
}

ITC_REGS_STEP(TextureCompressionDecodeA8R8G8B8Row) {
  ctx.fpscr.disableFlushMode();
  const auto r3 =
      tc::DecodeA8R8G8B8Row(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
  if (!r3) {
    lifted::original::c823F2620(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

// A guest function called as the originals call through ctr, at `stack`.
tc::GuestCall TextureCompressionGuestCall(lifted::Regs& ctx) {
  return [&ctx](uint32_t function, uint32_t stack, uint32_t r3, uint32_t r4, uint32_t r5,
                      uint32_t r6) -> uint32_t {
    const uint64_t saved = ctx.r1.u64;
    ctx.r1.u64 = stack;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    ctx.r6.u64 = r6;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = saved;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  };
}

ITC_REGS_STEP(TextureCompressionDxtEncodeRow) {
  if (!NativeHeapActive()) {
    lifted::original::c823F6298(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  // The codec's block functions (+140, +144) as the original calls them
  // through ctr.
  const tc::GuestCall call = TextureCompressionGuestCall(ctx);
  const auto r3 = tc::DxtEncodeRow(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                   ctx.r1.u32, call);
  if (!r3) {
    lifted::original::c823F6298(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

namespace {

void DeleteWith(lifted::Regs& ctx, std::optional<uint32_t> (*destruct)(uint32_t, uint32_t),
                lifted::RegsFunction* original) {
  const auto r3 = NativeHeapActive() ? destruct(ctx.r3.u32, ctx.r4.u32) : std::nullopt;
  if (!r3) {
    lifted::Sdk(ctx, original);
    return;
  }
  ctx.r3.u64 = *r3;
}

}  // namespace

ITC_REGS_STEP(TextureCompressionImageCodecDelete) {
  DeleteWith(ctx, tc::ImageCodecDelete, lifted::original::c823F6B70);
}

ITC_REGS_STEP(TextureCompressionDxtCodecDelete) {
  DeleteWith(ctx, tc::DxtCodecDelete, lifted::original::c823F6BC0);
}

}  // namespace hooks

#define ITC_TEXTURE_COMPRESSION_ADAPTER(address, step) \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_texture_compression)) { \
    hooks::step(ctx);                            \
  }

ITC_TEXTURE_COMPRESSION_ADAPTER(82422A68, TextureCompressionEncodeDxt45Block)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F2620, TextureCompressionDecodeA8R8G8B8Row)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6298, TextureCompressionDxtEncodeRow)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6B70, TextureCompressionImageCodecDelete)
ITC_TEXTURE_COMPRESSION_ADAPTER(823F6BC0, TextureCompressionDxtCodecDelete)

// For the fallback originals (docs/missing-functions.md): XGCompressSurface,
// DxtColor::PackR5G6B5 and DxtColor::FitEndpoints, mapped as their removed
// adapters did.
ITC_DIRECT_ENTRY(8235AAC0) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c8235AAC0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const texture_compression::CompressArguments arguments{
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
      ctx.r8.u32, ctx.r9.u32, ctx.r10.u32, ctx.f1.f64};
  const auto r3 = texture_compression::CompressSurface(arguments, ctx.r1.u32,
                                                       hooks::TextureCompressionGuestCall(ctx));
  if (!r3) {
    lifted::original::c8235AAC0(ctx);
    return;
  }
  ctx.r3.s64 = int32_t(*r3);
}
// r3 = a float4 color; r3 the 5:6:5 value.
ITC_DIRECT_ENTRY(824206E8) {
  ctx.fpscr.disableFlushMode();
  float color[3];
  for (uint32_t i = 0; i < 3; i++) {
    color[i] = guest::LoadFloat(ctx.r3.u32 + 4 * i);
  }
  ctx.r3.u64 = texture_compression::PackR5G6B5(color);
}
// r3 the low end, r4 the high end, r5 the 16 float4 pixels, r6 3 or 4 colors
// (another count: the original); r5 left + 8 as the original walks it.
ITC_DIRECT_ENTRY(82420AB8) {
  if (ctx.r6.u32 != 3 && ctx.r6.u32 != 4) {
    lifted::original::c82420AB8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  float pixels[16][3];
  for (uint32_t i = 0; i < 16; i++) {
    for (uint32_t c = 0; c < 3; c++) {
      pixels[i][c] = guest::LoadFloat(ctx.r5.u32 + texture_compression::kPixelBytes * i + 4 * c);
    }
  }
  float low[3];
  float high[3];
  texture_compression::FitColorEndpoints(pixels, ctx.r6.u32, ctx.r1.u32, low, high);
  for (uint32_t c = 0; c < 3; c++) {
    guest::StoreFloat(ctx.r3.u32 + 4 * c, low[c]);
  }
  for (uint32_t c = 0; c < 3; c++) {
    guest::StoreFloat(ctx.r4.u32 + 4 * c, high[c]);
  }
  ctx.r5.s64 = ctx.r5.s64 + 8;
}
// For the fallback originals (docs/missing-functions.md): Blitter::CopyRows,
// Blitter::ConvertRows, ImageCodec::Prepare and ImageCodec::Create, mapped as
// their removed adapters did.
// r3 = the blitter; r3 0, E_FAIL or CopyBlocks' (made by lis: sign-extended).
ITC_DIRECT_ENTRY(82369860) {
  ctx.r3.s64 = int32_t(texture_compression::BlitterCopyRows(
      ctx.r3.u32, ctx.r1.u32, hooks::TextureCompressionGuestCall(ctx)));
}
// r3 = the blitter; r3 0 or E_FAIL (sign-extended); the original where the
// port declines or without the native heap.
ITC_DIRECT_ENTRY(823699F8) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c823699F8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const auto r3 = texture_compression::BlitterConvertRows(
      ctx.r3.u32, ctx.r1.u32, hooks::TextureCompressionGuestCall(ctx));
  if (!r3) {
    lifted::original::c823699F8(ctx);
    return;
  }
  ctx.r3.s64 = int32_t(*r3);
}
// r3 = the destination's codec, r4 = the source's; r3 0 or E_OUTOFMEMORY.
ITC_DIRECT_ENTRY(823EAA90) {
  const auto r3 = hooks::NativeHeapActive()
                      ? texture_compression::PrepareCodecs(ctx.r3.u32, ctx.r4.u32)
                      : std::nullopt;
  if (!r3) {
    lifted::original::c823EAA90(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// r3 = the image description; r3 the codec or 0.
ITC_DIRECT_ENTRY(823F6C60) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c823F6C60(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const auto r3 = texture_compression::CreateCodec(ctx.r3.u32, ctx.r1.u32,
                                                   hooks::TextureCompressionGuestCall(ctx));
  if (!r3) {
    lifted::original::c823F6C60(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// For the fallback originals (docs/missing-functions.md): ImageCodec::
// ImageCodec and DxtCodec::DxtCodec, mapped as their removed adapters did.
// r3 = the codec, r4 = the description, r5 = bits per pixel, r6 = the kind;
// r3 the codec; the original where the port declines or without the native
// heap.
ITC_DIRECT_ENTRY(823EA800) {
  std::optional<uint32_t> r3;
  if (hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    r3 = texture_compression::ImageCodecConstruct(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                  ctx.r6.u32);
  }
  if (!r3) {
    lifted::original::c823EA800(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
// r3 = the codec, r4 = the description; r3 the codec; as ImageCodec's.
ITC_DIRECT_ENTRY(823F5FA0) {
  std::optional<uint32_t> r3;
  if (hooks::NativeHeapActive()) {
    ctx.fpscr.disableFlushMode();
    r3 = texture_compression::DxtCodecConstruct(ctx.r3.u32, ctx.r4.u32);
  }
  if (!r3) {
    lifted::original::c823F5FA0(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}
