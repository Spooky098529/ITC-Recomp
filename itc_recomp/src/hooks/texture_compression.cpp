// XGraphics' surface compression natively: adapters in front of
// src/texture_compression/ (docs/functions/texture-compression.md), with the
// register steps the harness shares (hooks/texture_compression.h).

#include "hooks/texture_compression.h"

#include "base/flags.h"
#include "texture_compression/dxt_alpha.h"

ITC_FLAG_BOOL(native_texture_compression, true, "ITC",
              "Run XGraphics' surface compression (docs/functions/texture-compression.md: "
              "the DXT alpha block's encoder, DxtAlpha::EncodeBlock sub_82422178 with "
              "DxtAlpha::FitEndpoints sub_824207D8) natively instead of through the "
              "translated code (the same block bytes; checked by "
              "--run_harnesses=texture_compression)");

namespace hooks {

namespace tc = texture_compression;

void TextureCompressionFitAlphaEndpoints(PPCContext& ctx, uint8_t* base) {
  if (ctx.r6.u32 != 6 && ctx.r6.u32 != 8) {
    __imp__sub_824207D8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  float values[16];
  for (uint32_t i = 0; i < 16; i++) {
    values[i] = guest::LoadFloat(base, ctx.r5.u32 + 4 * i);
  }
  float low;
  float high;
  tc::FitAlphaEndpoints(base, values, ctx.r6.u32, &low, &high);
  guest::StoreFloat(base, ctx.r3.u32, low);
  guest::StoreFloat(base, ctx.r4.u32, high);
}

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

ITC_TEXTURE_COMPRESSION_ADAPTER(824207D8, TextureCompressionFitAlphaEndpoints)
ITC_TEXTURE_COMPRESSION_ADAPTER(82422178, TextureCompressionEncodeAlphaBlock)
