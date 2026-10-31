// The part models' material passes natively: adapters in front of
// src/material_passes/ (docs/functions/material-passes.md), with the
// register steps the harness shares (hooks/material_passes.h).

#include "hooks/material_passes.h"

#include "base/flags.h"
#include "material_passes/material_passes.h"

ITC_FLAG_BOOL(native_material_passes, true, "ITC",
              "Run the part models' material passes and the helpers beside them "
              "(docs/functions/material-passes.md: Color::HsvToRgb sub_820D0D60, "
              "Color::HsvToRgb8 sub_820C9808 (no adapter: its callers are native), the tables' lookups sub_820D35D0, sub_820D3600, "
              "sub_820D3638 and sub_820D3728, Matrix::TransposeRotation sub_820D1CB0, the passes PartModel::SetTexture sub_820D17D8, "
              "FlipCulling sub_820D1950, ScaleBlended sub_820D1A68, SetPair1002Alpha sub_820D1B38 "
              "and SetAlpha sub_820D1C00, PartModel::SetMaterials sub_820D3318 (with "
              "SetMaterialFields sub_820D3250, Model::MaterialTotal sub_82594AC0 and "
              "Model::MaterialAt sub_82594B00, no adapters: only it calls them), the paint passes "
              "PartModel::SetPaint sub_820D0EA8, SetPaintBody sub_820D1378 and ApplyPalette "
              "sub_820D2EA8 (calling the colors' plain C++)) natively "
              "instead of through the translated code (the same writes; checked by "
              "--run_harnesses=material_passes)");

namespace hooks {

namespace mp = material_passes;

void MaterialPassesHsvToRgb(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  mp::HsvToRgb(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void MaterialPassesTableVector(PPCContext& ctx, uint8_t* base) {
  mp::TableVector(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void MaterialPassesIsBlockSetA(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mp::IsBlockSetA(ctx.r3.s32);
}

void MaterialPassesIsBlockSetB(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mp::IsBlockSetB(ctx.r3.s32);
}

void MaterialPassesRecordAt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mp::RecordAt(ctx.r3.u64);
}

void MaterialPassesTransposeRotation(PPCContext& ctx, uint8_t* base) {
  if (!mp::TransposeRotation(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32)) {
    __imp__sub_820D1CB0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
}

void MaterialPassesSetTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mp::SetTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r1.u32);
}

void MaterialPassesFlipCulling(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = mp::FlipCulling(base, ctx.r3.u32, ctx.r1.u32);
}

void MaterialPassesScaleBlended(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::ScaleBlended(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}

void MaterialPassesSetPair1002Alpha(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetPair1002Alpha(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}

void MaterialPassesSetAlpha(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetAlpha(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}

void MaterialPassesSetMaterials(PPCContext& ctx, uint8_t* base) {
  const uint64_t regs[7] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                            ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  std::optional<uint64_t> r3;
  if (!mp::SetMaterials(base, ctx.r3.u32, regs, ctx.r1.u32, &r3)) {
    __imp__sub_820D3318(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (r3) {
    ctx.r3.u64 = *r3;
  }
}

void MaterialPassesSetPaint(PPCContext& ctx, uint8_t* base) {
  const uint64_t regs[6] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                            ctx.r7.u64, ctx.r8.u64, ctx.r9.u64};
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetPaint(base, ctx.r3.u32, regs, ctx.r1.u32);
}

void MaterialPassesSetPaintBody(PPCContext& ctx, uint8_t* base) {
  const uint64_t regs[6] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                            ctx.r7.u64, ctx.r8.u64, ctx.r9.u64};
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetPaintBody(base, ctx.r3.u32, regs, ctx.r1.u32);
}

void MaterialPassesApplyPalette(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::ApplyPalette(base, ctx.r3.u32, ctx.r1.u32);
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off.

#define ITC_MATERIAL_PASSES(address, step)    \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_material_passes)) {  \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

ITC_MATERIAL_PASSES(820D0D60, MaterialPassesHsvToRgb)
ITC_MATERIAL_PASSES(820D35D0, MaterialPassesTableVector)
ITC_MATERIAL_PASSES(820D3600, MaterialPassesIsBlockSetA)
ITC_MATERIAL_PASSES(820D3638, MaterialPassesIsBlockSetB)
ITC_MATERIAL_PASSES(820D3728, MaterialPassesRecordAt)
ITC_MATERIAL_PASSES(820D1CB0, MaterialPassesTransposeRotation)
ITC_MATERIAL_PASSES(820D17D8, MaterialPassesSetTexture)
ITC_MATERIAL_PASSES(820D1950, MaterialPassesFlipCulling)
ITC_MATERIAL_PASSES(820D1A68, MaterialPassesScaleBlended)
ITC_MATERIAL_PASSES(820D1B38, MaterialPassesSetPair1002Alpha)
ITC_MATERIAL_PASSES(820D1C00, MaterialPassesSetAlpha)
ITC_MATERIAL_PASSES(820D3318, MaterialPassesSetMaterials)
ITC_MATERIAL_PASSES(820D0EA8, MaterialPassesSetPaint)
ITC_MATERIAL_PASSES(820D1378, MaterialPassesSetPaintBody)
ITC_MATERIAL_PASSES(820D2EA8, MaterialPassesApplyPalette)
