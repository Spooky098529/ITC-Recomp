// XGraphics' surface compression natively: adapters in front of
// src/texture_compression/ (docs/functions/texture-compression.md), with the
// register steps the harness shares (hooks/texture_compression.h). The
// helpers' adapters (DxtAlpha::FitEndpoints, DxtColor::PackR5G6B5 and
// DxtColor::FitEndpoints) went once their callers were native: the encoders'
// plain C++ calls them directly.

#include "hooks/texture_compression.h"

#include "base/flags.h"
#include "texture_compression/dxt_alpha.h"
#include "texture_compression/dxt_color.h"

ITC_FLAG_BOOL(native_texture_compression, true, "ITC",
              "Run XGraphics' surface compression (docs/functions/texture-compression.md: "
              "the DXT alpha block's encoder, DxtAlpha::EncodeBlock sub_82422178 with "
              "DxtAlpha::FitEndpoints sub_824207D8; the DXT color block's helpers, "
              "DxtColor::PackR5G6B5 sub_824206E8 and DxtColor::FitEndpoints sub_82420AB8; "
              "the DXT color block's encoder, DxtColor::EncodeBlock sub_82420F50; the "
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
