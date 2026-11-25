// The light glows' billboards natively: adapters in front of
// src/sprites/billboards.cpp (docs/functions/sprites.md). The vertex
// builder's adapter (sub_8209C6E0) is in src/interpolation/blend_state.cpp,
// with the camera blend around it.

#include "guest.h"
#include "hooks/native_entry.h"
#include "lifted/sdk.h"

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
              "wrap) and Draw sub_8209CDB0 (no adapter); the light groups' draw sub_82148658 (no adapter)) natively instead of through the "
              "translated code (same results; checked by --run_harnesses=billboards)");

// BillboardMaterial::Init: r3 = the material.
ITC_NATIVE_ENTRY(8209F5A0, ITC_FLAG(native_billboards)) {
  sprites::InitBillboardMaterial(ctx.r3.u32);
}

// BillboardMaterial::SetLights: r3 = the material, r4 = light count, r5 =
// the lights.
ITC_NATIVE_ENTRY(8209F5C8, ITC_FLAG(native_billboards)) {
  sprites::SetBillboardLights(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// --- The recording (src/sprites/billboard_record.cpp) ---

bool hooks::BillboardsNative() {
  return ITC_FLAG(native_billboards) && ITC_FLAG(native_matrix_math) &&
         !hooks::SceneryListsSlotDiagnostics();
}

void hooks::BillboardsRecord(lifted::Regs& ctx) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx);
  const uint64_t r3 = sprites::RecordBillboards(calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

void hooks::BillboardsDraw(lifted::Regs& ctx) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx);
  const uint64_t r3 = sprites::DrawBillboards(calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

// Billboards::Draw (sub_8209CDB0) has no adapter (removed 2026-11-13, B7
// loop): its only caller in play, the light groups' draw, calls its plain C++.

bool hooks::LightGroupsNative() {
  return hooks::BillboardsNative() && ITC_FLAG(native_models) &&
         ITC_FLAG(native_course_scenery) && ITC_FLAG(native_scenery_lists);
}

void hooks::LightGroupsDraw(lifted::Regs& ctx) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const car_scene_draw::SceneDrawCalls calls = hooks::CarSceneDrawRootCalls(ctx);
  const uint64_t r3 = sprites::DrawLightGroups(calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                               ctx.r5.u32, ctx.r6.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

// LightGroups::Draw (sub_82148658) has no adapter (removed 2026-11-13, B7
// loop): its callers in play, Scenery::DrawArea and Scenery::Draw, call its
// plain C++; LightGroupsDraw stays as the harness's steps.
