// The part models' material passes natively: adapters in front of
// src/material_passes/ (docs/functions/material-passes.md), with the
// register steps the harness shares (hooks/material_passes.h).

#include "hooks/material_passes.h"

#include "base/flags.h"
#include "material_passes/material_passes.h"

ITC_FLAG_BOOL(native_material_passes, true, "ITC",
              "Run the part models' material passes and the helpers beside them "
              "(docs/functions/material-passes.md: Color::HsvToRgb sub_820D0D60, "
              "Color::HsvToRgb8 sub_820C9808, the tables' lookups sub_820D35D0, sub_820D3600, "
              "sub_820D3638 and sub_820D3728, Matrix::TransposeRotation sub_820D1CB0) natively "
              "instead of through the translated code (the same writes; checked by "
              "--run_harnesses=material_passes)");

namespace hooks {

namespace mp = material_passes;

void MaterialPassesHsvToRgb(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  mp::HsvToRgb(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void MaterialPassesHsvToRgb8(PPCContext& ctx, uint8_t* base) {
  mp::HsvToRgb8(base, ctx.r3.u32, ctx.r4.u32);
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
ITC_MATERIAL_PASSES(820C9808, MaterialPassesHsvToRgb8)
ITC_MATERIAL_PASSES(820D35D0, MaterialPassesTableVector)
ITC_MATERIAL_PASSES(820D3600, MaterialPassesIsBlockSetA)
ITC_MATERIAL_PASSES(820D3638, MaterialPassesIsBlockSetB)
ITC_MATERIAL_PASSES(820D3728, MaterialPassesRecordAt)
ITC_MATERIAL_PASSES(820D1CB0, MaterialPassesTransposeRotation)
