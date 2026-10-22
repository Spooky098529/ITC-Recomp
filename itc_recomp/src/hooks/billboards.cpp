// The light glows' billboards natively: adapters in front of
// src/sprites/billboards.cpp (docs/functions/sprites.md). The vertex
// builder's adapter (sub_8209C6E0) is in src/interpolation/blend_state.cpp,
// with the camera blend around it.

#include "guest.h"

#include "base/flags.h"
#include "sprites/billboards.h"

ITC_FLAG_BOOL(native_billboards, true, "ITC",
              "Run the light glows' billboard functions (the material's set-up, "
              "sub_8209F5A0 and sub_8209F5C8) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=billboards)");

// BillboardMaterial::Init: r3 = the material.
extern "C" REX_FUNC(sub_8209F5A0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_billboards)) {
    __imp__sub_8209F5A0(ctx, base);
    return;
  }
  sprites::InitBillboardMaterial(base, ctx.r3.u32);
}

// BillboardMaterial::SetLights: r3 = the material, r4 = light count, r5 =
// the lights.
extern "C" REX_FUNC(sub_8209F5C8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_billboards)) {
    __imp__sub_8209F5C8(ctx, base);
    return;
  }
  sprites::SetBillboardLights(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
