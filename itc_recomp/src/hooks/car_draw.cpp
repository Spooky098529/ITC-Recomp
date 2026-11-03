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
ITC_FLAG_DECLARE(bool, native_second_set);
ITC_FLAG_DECLARE(bool, native_c_runtime);
ITC_FLAG_DECLARE(bool, native_material_passes);

ITC_FLAG_BOOL(native_car_draw, true, "ITC",
              "Run the car draw (docs/functions/car-draw.md: Model::Draw sub_820D04E8 and "
              "CarPart::Draw sub_820D06F0 (the recorders, Model::RecordMaterials and "
              "Model::DrawShapeDepth as plain C++, the render queue's slot 4 a guest call), "
              "Matrix::Shadow sub_82597FE0 (no adapter: Gather's plain C++ calls it), CarLighting::Gather sub_820C9170 (with "
              "Lights::FindNearest, Vec4::Transform and cos as plain C++), "
              "LightKit::BlinkLevel sub_820DC4B0 (no adapter: the neons call it), BlinkTimers::Advance sub_820DC958 (inside "
              "its render-only skip, --per_tick_blink_timers), "
              "SceneModel::IsNearPlayerHeight sub_820DAB58 (inside the diagnostics' wrap), "
              "the morph Model::MorphShape sub_82595110 (the job maker a guest call), "
              "WorkMem::AllocVertexBuffer sub_8258A9E8 (no adapter: MorphShape calls it), Model::QueueJobRelease sub_82594E88 (no adapter: MorphShape calls it) and "
              "XGSetVertexBufferHeader sub_8235A688 (no adapter: its caller calls it), the light "
              "kit SceneLights::Draw sub_820DDE58 (inside the diagnostics' wrap; it skips "
              "BlinkTimers::Advance as --per_tick_blink_timers says) with LightPoints::DrawFront "
              "sub_820DDAE8, DrawSides sub_820DDB98, DrawRear sub_820DDC48 and their neons "
              "sub_820DCC10, sub_820DD0B0 and sub_820DD5C0 (no adapters: only it calls them; the "
              "LEDs and spots guest calls), the parts' small draws "
              "(PartsPoints::DrawFrontBumperLights sub_820DE1C8, DrawRearBumperLights "
              "sub_820DE3C0, DrawLeftDoorPanel sub_820DE6E0, DrawDoorWindows02 sub_820DE760, "
              "DrawRightDoorPanel sub_820DE8C8, DrawDoorWindows01 sub_820DE948, DrawDoorsBody "
              "sub_820DEA88, DrawDoorsWindows sub_820DEAF0, DrawFendersBody sub_820DED88, "
              "DrawFenderLights sub_820DEDD8, DrawCabinWindow02 sub_820DEF38, DrawCabinWindow01 "
              "sub_820DEFA8, DrawEngine sub_820DF070, DrawFrontBumper sub_820DF308, "
              "DrawRearBumper sub_820DF458, DrawBonnet sub_820E0998, DrawMirror sub_820E1138 "
              "and DrawWithShadow sub_820E1260; PartModel::SetPair1002Alpha, XPR::GetTexture "
              "and the matrix functions as plain C++)) "
              "natively instead of through the translated code, with --native_scenery_lists, "
              "--native_models, --native_matrix_math, --native_math, --native_nearest_lights, "
              "--native_vehicle_accessors, --native_track_zones, --native_second_set, "
              "--native_c_runtime and --native_material_passes (else the originals; the "
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

bool CarDrawMorphCalleesNative() {
  return !SceneryListsSlotDiagnostics() && ITC_FLAG(native_second_set) &&
         ITC_FLAG(native_models) && ITC_FLAG(native_c_runtime);
}

bool CarDrawLightKitCalleesNative() {
  return CarDrawModelCalleesNative() && ITC_FLAG(native_material_passes) &&
         ITC_FLAG(native_second_set) && ITC_FLAG(native_matrix_math);
}

bool CarDrawPartsCalleesNative() {
  return CarDrawModelCalleesNative() && ITC_FLAG(native_material_passes) &&
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
  calls.call = [guest](uint32_t function, const uint64_t* regs, int count, uint32_t sp,
                       uint32_t lr) {
    PPCContext& ctx = guest->ctx;
    uint8_t* base = guest->base;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    PPCRegister* registers[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                                 &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < count; i++) {
      registers[i]->u64 = regs[i];
    }
    ctx.fpscr.disableFlushMode();
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  calls.call_r28 = [guest, call = calls.call](uint32_t function, const uint64_t* regs, int count,
                                              uint32_t sp, uint32_t lr, uint32_t r28) {
    PPCContext& ctx = guest->ctx;
    const uint64_t saved = ctx.r28.u64;
    ctx.r28.u64 = r28;
    const uint64_t r3 = call(function, regs, count, sp, lr);
    ctx.r28.u64 = saved;
    return r3;
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

namespace {

void LeaveLeft(PPCContext& ctx, const car_draw::Left& left) {
  if (!left.in_ctx) {
    ctx.r3.u64 = left.r3;
  }
  ctx.fpscr.disableFlushMode();
}

}  // namespace

void CarDrawMorphShape(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawMorphCalleesNative()) {
    __imp__sub_82595110(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const auto left = car_draw::ModelMorphShape(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u64,
                                              ctx.r1.u32);
  if (!left) {
    __imp__sub_82595110(ctx, base);
    return;
  }
  ctx.lr = lr;
  LeaveLeft(ctx, *left);
}

void CarDrawSceneLightsDraw(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawLightKitCalleesNative()) {
    __imp__sub_820DDE58(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const auto left =
      car_draw::SceneLightsDraw(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32,
                                !CarDrawBlinkTimersHeld(), ctx.r24.u32, ctx.r28.u32);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}

// --- The parts' small draws ---

namespace {

// A part draw's register steps: the original unless the callees' plain C++
// is native; `draw` runs the plain C++ with the calls.
template <typename Draw>
void PartsStep(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
               Draw draw) {
  if (!CarDrawPartsCalleesNative()) {
    original(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const car_draw::Left left = draw(calls);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}

}  // namespace

#define ITC_CAR_DRAW_PARTS(name, address, ...)                                 \
  void CarDrawParts##name(PPCContext& ctx, uint8_t* base) {                    \
    const uint32_t r3 = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32;          \
    const uint32_t r6 = ctx.r6.u32, r1 = ctx.r1.u32, r24 = ctx.r24.u32;        \
    (void)r4;                                                                  \
    (void)r6;                                                                  \
    PartsStep(ctx, base, __imp__sub_##address, [&](const car_draw::Calls& calls) { \
      return car_draw::__VA_ARGS__;                                            \
    });                                                                        \
  }

ITC_CAR_DRAW_PARTS(FrontBumperLights, 820DE1C8,
                   PartsDrawFrontBumperLights(base, calls, r1, r3, r4, r5, r6, r24))
ITC_CAR_DRAW_PARTS(RearBumperLights, 820DE3C0,
                   PartsDrawRearBumperLights(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(LeftDoorPanel, 820DE6E0,
                   PartsDrawDoorPanel(base, calls, false, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(DoorWindows02, 820DE760,
                   PartsDrawDoorWindows(base, calls, false, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(RightDoorPanel, 820DE8C8,
                   PartsDrawDoorPanel(base, calls, true, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(DoorWindows01, 820DE948,
                   PartsDrawDoorWindows(base, calls, true, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(DoorsBody, 820DEA88, PartsDrawDoorsBody(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(DoorsWindows, 820DEAF0,
                   PartsDrawDoorsWindows(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(FendersBody, 820DED88, PartsDrawFendersBody(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(FenderLights, 820DEDD8,
                   PartsDrawFenderLights(base, calls, r1, r3, r4, r5, r6, r24))
ITC_CAR_DRAW_PARTS(CabinWindow02, 820DEF38,
                   PartsDrawCabinWindow02(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(CabinWindow01, 820DEFA8,
                   PartsDrawCabinWindow01(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(Engine, 820DF070, PartsDrawEngine(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(FrontBumper, 820DF308, PartsDrawFrontBumper(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(RearBumper, 820DF458, PartsDrawRearBumper(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(Bonnet, 820E0998, PartsDrawBonnet(base, calls, r1, r3, r4, r5, r24))
ITC_CAR_DRAW_PARTS(Mirror, 820E1138, PartsDrawShadowed(base, calls, true, r1, r3, r5, r6, r24))
ITC_CAR_DRAW_PARTS(WithShadow, 820E1260,
                   PartsDrawShadowed(base, calls, false, r1, r3, r5, r4 + 16, r24))
#undef ITC_CAR_DRAW_PARTS

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
// (2026-11-01; its only caller, CarLighting::Gather, calls the plain C++),
// WorkMem::AllocVertexBuffer sub_8258A9E8 and Model::QueueJobRelease
// sub_82594E88 (2026-11-03; their caller in play, Model::MorphShape, calls
// the plain C++), LightKit::BlinkLevel sub_820DC4B0 (2026-11-03; its
// callers, the neons, are plain C++).
ITC_CAR_DRAW(820D04E8, CarDrawModelDraw)
ITC_CAR_DRAW(820D06F0, CarDrawCarPartDraw)
ITC_CAR_DRAW(820C9170, CarDrawCarLightingGather)
ITC_CAR_DRAW(82595110, CarDrawMorphShape)
// The parts' small draws (their callers, the parts' top, are recompiled).
ITC_CAR_DRAW(820DE1C8, CarDrawPartsFrontBumperLights)
ITC_CAR_DRAW(820DE3C0, CarDrawPartsRearBumperLights)
ITC_CAR_DRAW(820DE6E0, CarDrawPartsLeftDoorPanel)
ITC_CAR_DRAW(820DE760, CarDrawPartsDoorWindows02)
ITC_CAR_DRAW(820DE8C8, CarDrawPartsRightDoorPanel)
ITC_CAR_DRAW(820DE948, CarDrawPartsDoorWindows01)
ITC_CAR_DRAW(820DEA88, CarDrawPartsDoorsBody)
ITC_CAR_DRAW(820DEAF0, CarDrawPartsDoorsWindows)
ITC_CAR_DRAW(820DED88, CarDrawPartsFendersBody)
ITC_CAR_DRAW(820DEDD8, CarDrawPartsFenderLights)
ITC_CAR_DRAW(820DEF38, CarDrawPartsCabinWindow02)
ITC_CAR_DRAW(820DEFA8, CarDrawPartsCabinWindow01)
ITC_CAR_DRAW(820DF070, CarDrawPartsEngine)
ITC_CAR_DRAW(820DF308, CarDrawPartsFrontBumper)
ITC_CAR_DRAW(820DF458, CarDrawPartsRearBumper)
ITC_CAR_DRAW(820E0998, CarDrawPartsBonnet)
ITC_CAR_DRAW(820E1138, CarDrawPartsMirror)
ITC_CAR_DRAW(820E1260, CarDrawPartsWithShadow)
#undef ITC_CAR_DRAW
