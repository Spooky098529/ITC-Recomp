// The car draw natively: adapters in front of src/car_draw/
// (docs/functions/car-draw.md), with the register steps the harness shares
// (hooks/car_draw.h). SceneModel::IsNearPlayerHeight's adapter is in the
// diagnostics' wrap (src/debug/frame_state_diff.cpp) and
// BlinkTimers::Advance's in its render-only skip
// (src/frame_pacing/per_frame_logic.cpp), each calling these steps under the
// flag.

#include "hooks/car_draw.h"

#include "base/flags.h"
#include "frame_pacing/game_messages.h"

ITC_FLAG_DECLARE(bool, native_scenery_lists);
ITC_FLAG_DECLARE(bool, native_models);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_math);
ITC_FLAG_DECLARE(bool, native_nearest_lights);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_track_zones);
ITC_FLAG_DECLARE(bool, per_tick_blink_timers);

ITC_FLAG_BOOL(native_car_draw, true, "ITC",
              "Run the car draw (docs/functions/car-draw.md: Model::Draw sub_820D04E8 and "
              "CarPart::Draw sub_820D06F0 (the recorders, Model::RecordMaterials and "
              "Model::DrawShapeDepth as plain C++, the render queue's slot 4 a guest call), "
              "Matrix::Shadow sub_82597FE0 (no adapter: Gather's plain C++ calls it), CarLighting::Gather sub_820C9170 (with "
              "Lights::FindNearest, Vec4::Transform and cos as plain C++), "
              "LightKit::BlinkLevel sub_820DC4B0, BlinkTimers::Advance sub_820DC958 (inside "
              "its render-only skip, --per_tick_blink_timers) and "
              "SceneModel::IsNearPlayerHeight sub_820DAB58 (inside the diagnostics' wrap)) "
              "natively instead of through the translated code, with --native_scenery_lists, "
              "--native_models, --native_matrix_math, --native_math, --native_nearest_lights, "
              "--native_vehicle_accessors and --native_track_zones (else the originals; the "
              "same writes; checked by --run_harnesses=car_draw)");

namespace hooks {

bool CarDrawModelCalleesNative() {
  return ITC_FLAG(native_scenery_lists) && ITC_FLAG(native_models) &&
         !SceneryListsSlotDiagnostics();
}

bool CarDrawLightingCalleesNative() {
  return ITC_FLAG(native_matrix_math) && ITC_FLAG(native_math) &&
         ITC_FLAG(native_nearest_lights);
}

bool CarDrawHeightCalleesNative() {
  return ITC_FLAG(native_vehicle_accessors) && ITC_FLAG(native_track_zones) &&
         ITC_FLAG(native_matrix_math);
}

bool CarDrawCalleesNative() {
  return CarDrawModelCalleesNative() && CarDrawLightingCalleesNative() &&
         CarDrawHeightCalleesNative();
}

car_draw::Calls CarDrawCalls(SceneryDrawGuest* guest) {
  car_draw::Calls calls;
  calls.draw = SceneryListsDrawCalls(guest);
  calls.memcpy = [guest](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp, uint32_t lr) {
    PPCContext& ctx = guest->ctx;
    uint8_t* base = guest->base;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = bytes;
    __imp__sub_825A7410(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
  };
  calls.find_nearest = [guest](const uint64_t regs[7], double f1, double f2, double f3,
                               uint32_t sp, uint32_t lr) {
    PPCContext& ctx = guest->ctx;
    uint8_t* base = guest->base;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    PPCRegister* registers[7] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7, &ctx.r8, &ctx.r9};
    for (int i = 0; i < 7; i++) {
      registers[i]->u64 = regs[i];
    }
    ctx.f1.f64 = f1;
    ctx.f2.f64 = f2;
    ctx.f3.f64 = f3;
    ctx.fpscr.disableFlushMode();
    __imp__sub_820F4128(ctx, base);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  return calls;
}

bool CarDrawBlinkTimersHeld() {
  return frame_pacing::GameMessages_IsRenderOnlyPass() && ITC_FLAG(per_tick_blink_timers);
}

namespace {

// r3 as a draw leaves it: the last queue call's (left in ctx by the guest
// call), else the draw's (r3 as it came, all 64 bits, when that's the
// draw's: Model::Draw's model when its +28 is 0).
void LeaveDraw(PPCContext& ctx, const car_draw::Draw& draw) {
  if (!draw.queued && draw.r3 != ctx.r3.u32) {
    ctx.r3.u64 = draw.r3;
  }
  ctx.fpscr.disableFlushMode();
}

}  // namespace

void CarDrawModelDraw(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawModelCalleesNative()) {
    __imp__sub_820D04E8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const auto draw = car_draw::ModelDraw(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                        ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r24.u32);
  if (!draw) {
    __imp__sub_820D04E8(ctx, base);
    return;
  }
  ctx.lr = lr;
  LeaveDraw(ctx, *draw);
}

void CarDrawCarPartDraw(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawModelCalleesNative()) {
    __imp__sub_820D06F0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const auto draw =
      car_draw::CarPartDraw(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                            ctx.r6.u32, ctx.r7.u32, ctx.r24.u32, ctx.r28.u32);
  if (!draw) {
    __imp__sub_820D06F0(ctx, base);
    return;
  }
  ctx.lr = lr;
  LeaveDraw(ctx, *draw);
}

void CarDrawCarLightingGather(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawLightingCalleesNative()) {
    __imp__sub_820C9170(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  ctx.r3.u64 = car_draw::CarLightingGather(base, calls, ctx.r1.u32, ctx.r3.u64, ctx.r4.u32,
                                           ctx.r5.u32);
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

void CarDrawBlinkLevel(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = car_draw::LightKitBlinkLevel(base, ctx.r3.u64, ctx.f1.f64, ctx.f2.f64,
                                            ctx.f3.f64, ctx.r1.u32);
}

void CarDrawBlinkTimersAdvance(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  car_draw::BlinkTimersAdvance(base, ctx.r3.u32, ctx.r1.u32);
}

void CarDrawIsNearPlayerHeight(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawHeightCalleesNative()) {
    __imp__sub_820DAB58(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  ctx.r3.u64 =
      car_draw::SceneModelIsNearPlayerHeight(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
  ctx.lr = lr;
  ctx.fpscr.disableFlushMode();
}

}  // namespace hooks

#define ITC_CAR_DRAW(address, step)     \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_car_draw)) {   \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

// CarPart::Draw's adapter is also what the native cockpit dials call
// (src/hooks/cockpit_dials.cpp). No adapter: Matrix::Shadow sub_82597FE0
// (2026-11-01; its only caller, CarLighting::Gather, calls the plain C++).
ITC_CAR_DRAW(820D04E8, CarDrawModelDraw)
ITC_CAR_DRAW(820D06F0, CarDrawCarPartDraw)
ITC_CAR_DRAW(820C9170, CarDrawCarLightingGather)
ITC_CAR_DRAW(820DC4B0, CarDrawBlinkLevel)
#undef ITC_CAR_DRAW
