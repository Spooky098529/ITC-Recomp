// The part models' material passes natively: adapters in front of
// src/material_passes/ (docs/functions/material-passes.md), with the
// register steps the harness shares (hooks/material_passes.h).

#include "hooks/material_passes.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "material_passes/material_passes.h"
#include "guest.h"

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
              "sub_820D2EA8 (calling the colors' plain C++), Projector::Matrices sub_820D1EB8 (with "
              "Matrix::LookAt sub_820D1D70, no adapter: only it calls it; the matrix functions' "
              "plain C++, Copy and Multiply noted for frame interpolation), PaintPalette::Init sub_820D2330 "
              "(the paint palette's constants)) natively "
              "instead of through the translated code (the same writes; checked by "
              "--run_harnesses=material_passes)");

namespace hooks {

namespace mp = material_passes;

ITC_REGS_STEP(MaterialPassesHsvToRgb) {
  ctx.fpscr.disableFlushMode();
  mp::HsvToRgb(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesTableVector) {
  mp::TableVector(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(MaterialPassesIsBlockSetA) {
  ctx.r3.u64 = mp::IsBlockSetA(ctx.r3.s32);
}

ITC_REGS_STEP(MaterialPassesIsBlockSetB) {
  ctx.r3.u64 = mp::IsBlockSetB(ctx.r3.s32);
}

ITC_REGS_STEP(MaterialPassesRecordAt) {
  ctx.r3.u64 = mp::RecordAt(ctx.r3.u64);
}

ITC_REGS_STEP(MaterialPassesTransposeRotation) {
  if (!mp::TransposeRotation(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32)) {
    lifted::original::c820D1CB0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(MaterialPassesSetTexture) {
  ctx.r3.u64 = mp::SetTexture(ctx.r3.u32, ctx.r4.u32, ctx.r5.s32, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesFlipCulling) {
  ctx.r3.u64 = mp::FlipCulling(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesScaleBlended) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::ScaleBlended(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesSetPair1002Alpha) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetPair1002Alpha(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesSetAlpha) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetAlpha(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesSetMaterials) {
  const uint64_t regs[7] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                            ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  std::optional<uint64_t> r3;
  if (!mp::SetMaterials(ctx.r3.u32, regs, ctx.r1.u32, &r3)) {
    lifted::original::c820D3318(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (r3) {
    ctx.r3.u64 = *r3;
  }
}

ITC_REGS_STEP(MaterialPassesSetPaint) {
  const uint64_t regs[6] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                            ctx.r7.u64, ctx.r8.u64, ctx.r9.u64};
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetPaint(ctx.r3.u32, regs, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesSetPaintBody) {
  const uint64_t regs[6] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                            ctx.r7.u64, ctx.r8.u64, ctx.r9.u64};
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::SetPaintBody(ctx.r3.u32, regs, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesApplyPalette) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = mp::ApplyPalette(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(MaterialPassesPaintPaletteInit) {
  ctx.fpscr.disableFlushMode();
  if (const auto r3 = mp::PaintPaletteInit(ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
  }
}

ITC_REGS_STEP(MaterialPassesProjectorMatrices) {
  const mp::ProjectorArgs args{ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                               ctx.r6.u32, ctx.r7.u32, ctx.r9.u32};
  ctx.fpscr.disableFlushMode();
  const auto r3 = mp::ProjectorMatrices(args, ctx.r1.u32);
  if (!r3) {
    lifted::original::c820D1EB8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = *r3;
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off.

#define ITC_MATERIAL_PASSES(address, step)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_material_passes)) { \
    hooks::step(ctx);                   \
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
ITC_MATERIAL_PASSES(820D1EB8, MaterialPassesProjectorMatrices)
ITC_MATERIAL_PASSES(820D2330, MaterialPassesPaintPaletteInit)

// For the fallback originals (docs/missing-functions.md): the stale
// originals round (2026-11-28), so lifted code calls the ports.

// Matrix::LookAt: r3 out, r4 position, r5 direction, r6 up; r3 is left as
// out (the inverse's answer), as the original leaves it.
ITC_DIRECT_ENTRY(820D1D70) {
  ctx.fpscr.disableFlushMode();
  material_passes::LookAt(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

// PartModel::SetMaterialFields (a leaf without a frame): r3 to r10 the
// eight args, homed by the original at r1 + 16 to + 80 in its caller's
// frame; the material at r1 + 116, the 24 bytes SetMaterials copied at
// r1 + 88 (+88's doubleword the third float4's second half, the floats at
// +96 to +104). The homes are stored as the original does. r3 untouched.
ITC_DIRECT_ENTRY(820D3250) {
  const uint32_t sp = ctx.r1.u32;
  const uint64_t args[8] = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                            ctx.r7.u64, ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  static constexpr uint32_t kHomes[8] = {16, 24, 32, 48, 56, 64, 72, 80};
  for (int i = 0; i < 8; i++) {
    guest::StoreU64(sp + kHomes[i], args[i]);
  }
  uint32_t copied[6];
  for (uint32_t w = 0; w < 6; w++) {
    copied[w] = guest::LoadU32(sp + 88 + 4 * w);
  }
  ctx.fpscr.disableFlushMode();
  material_passes::SetMaterialFields(guest::LoadU32(sp + 116), args, copied);
}

// Model::MaterialTotal: r3 model, r4 shape; r3 = the count (lwz).
ITC_DIRECT_ENTRY(82594AC0) {
  ctx.r3.u64 = material_passes::MaterialTotal(ctx.r3.u32, ctx.r4.u64);
}

// Model::MaterialAt: r3 model, r4 shape, r5 index; r3 = the material.
ITC_DIRECT_ENTRY(82594B00) {
  ctx.r3.u64 = material_passes::MaterialAt(ctx.r3.u32, ctx.r4.u64, ctx.r5.u64);
}
