// The light glows' billboards natively: adapters in front of
// src/sprites/billboards.cpp (docs/functions/sprites.md). The vertex
// builder's adapter (sub_8209C6E0) is in src/interpolation/blend_state.cpp,
// with the camera blend around it.

#include "guest.h"

#include "base/flags.h"
#include "car_scene_draw/car_scene_draw.h"
#include "hooks/billboards.h"
#include "hooks/car_scene_draw.h"
#include "hooks/scenery_lists.h"
#include "sprites/billboards.h"

ITC_FLAG_DECLARE(bool, native_course_scenery);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_models);
ITC_FLAG_DECLARE(bool, native_scenery_lists);

ITC_FLAG_BOOL(native_billboards, true, "ITC",
              "Run the light glows' billboard functions (the material's set-up, "
              "sub_8209F5A0 and sub_8209F5C8; the vertices, sub_8209C6E0; the recording, "
              "Billboards::RecordState sub_8209C1A0 (no adapter), Record sub_8209CC38 (in its "
              "wrap) and Draw sub_8209CDB0; the light groups' draw sub_82148658) natively instead of through the "
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

// --- The recording (src/sprites/billboard_record.cpp) ---

bool hooks::BillboardsNative() {
  return ITC_FLAG(native_billboards) && ITC_FLAG(native_matrix_math) &&
         !hooks::SceneryListsSlotDiagnostics();
}

void hooks::BillboardsRecord(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  const uint64_t r3 = sprites::RecordBillboards(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

void hooks::BillboardsDraw(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  const uint64_t r3 = sprites::DrawBillboards(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

// Billboards::Draw: r3 = the set, r4 = the material; r3 = 0 or 1. From the
// light groups' draw (sub_82148658).
extern "C" REX_FUNC(sub_8209CDB0) {
  REX_FUNC_PROLOGUE();
  if (!hooks::BillboardsNative()) {
    __imp__sub_8209CDB0(ctx, base);
    return;
  }
  hooks::BillboardsDraw(ctx, base);
}

bool hooks::LightGroupsNative() {
  return hooks::BillboardsNative() && ITC_FLAG(native_models) &&
         ITC_FLAG(native_course_scenery) && ITC_FLAG(native_scenery_lists);
}

void hooks::LightGroupsDraw(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx, base);
  const uint64_t r3 = sprites::DrawLightGroups(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                               ctx.r5.u32, ctx.r6.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

// LightGroups::Draw: r3 = the set, r4 = the groups, r5 = the XPR, r6 = the
// animation set. From Scenery::DrawArea and Scenery::Draw (recompiled).
extern "C" REX_FUNC(sub_82148658) {
  REX_FUNC_PROLOGUE();
  if (!hooks::LightGroupsNative()) {
    __imp__sub_82148658(ctx, base);
    return;
  }
  hooks::LightGroupsDraw(ctx, base);
}
