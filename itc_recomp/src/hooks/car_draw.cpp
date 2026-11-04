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
#include "hooks/load_poll.h"
#include "hooks/second_set.h"

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
ITC_FLAG_DECLARE(bool, native_accessories);
ITC_FLAG_DECLARE(bool, native_scene_models);
ITC_FLAG_DECLARE(bool, native_load_poll);

ITC_FLAG_BOOL(native_car_draw, true, "ITC",
              "Run the car draw (docs/functions/car-draw.md: Model::Draw sub_820D04E8 and "
              "CarPart::Draw sub_820D06F0 (the recorders, Model::RecordMaterials and "
              "Model::DrawShapeDepth as plain C++, the render queue's slot 4 a guest call), "
              "Matrix::Shadow sub_82597FE0 (no adapter: Gather's plain C++ calls it), CarLighting::Gather sub_820C9170 (with "
              "Lights::FindNearest, Vec4::Transform and cos as plain C++), "
              "LightKit::BlinkLevel sub_820DC4B0 (no adapter: the neons call it), BlinkTimers::Advance sub_820DC958 (inside "
              "its render-only skip, --per_tick_blink_timers), "
              "SceneModel::IsNearPlayerHeight sub_820DAB58 (inside the diagnostics' wrap), "
              "the morph Model::MorphShape sub_82595110 (no adapter: the wheels' draws call it; the job "
              "maker a guest call), "
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
              "DrawRearBumper sub_820DF458, DrawBonnet sub_820E0998, DrawLeftDoor sub_820E0B40 and "
              "DrawRightDoor sub_820E0E38 (the doors' matrices), DrawHeadlightBody sub_820E13E0 "
              "(the pop-up's matrix), DrawMirror sub_820E1138 "
              "and DrawWithShadow sub_820E1260, no adapters: the parts' top calls them; PartModel::SetPair1002Alpha, XPR::GetTexture "
              "and the matrix functions as plain C++), the head and tail lights with their "
              "camera-facing glows (PartsPoints::DrawTailLights sub_820DF8B0 and "
              "DrawHeadlights sub_820E1690, no adapters: the parts' top calls them; the material passes, Projector::Matrices and "
              "Model::Draw as plain C++), the shadows PartsPoints::DrawShadows sub_820E1D88 "
              "(no adapter: the parts' top calls it; CarLighting::Gather and Model::DrawShapeDepth "
              "as plain C++), and the parts' top "
              "SceneParts::Draw sub_820E2EE8 (inside the diagnostics' wrap) with "
              "PartsPoints::DrawFrontEnd sub_820E2448 and DrawRace sub_820E2760 (no adapters: "
              "only it calls them; the parts' draws above as plain C++), the interior's "
              "draws InteriorDials::DrawFaces sub_820E3170, InteriorPoints::DrawRollCage "
              "sub_820E3548, InteriorDials::DrawDigitalSpeed sub_820E39F8, DrawReadouts "
              "sub_820E51D0 (the cockpit dials' Dash::PoseTachometer a guest call), "
              "InteriorMeters::DrawFaces sub_820E3AD8 and InteriorPoints::DrawSeats "
              "sub_820E3CF0 (no adapters: the interior's lists call them; PartModel::SetTexture, "
              "Model::Draw and the matrix functions as plain C++), and the interior's top SceneInterior::Draw sub_820E53F0 (inside the "
              "diagnostics' wrap; Accessory::SetTexture and the accessories' set as plain C++ "
              "with --native_accessories and --native_scene_models, Accessory::Draw a guest "
              "call) with InteriorPoints::DrawFrontEnd sub_820E4078 and DrawRace sub_820E5220 "
              "(no adapters: only it calls them; Dash::PoseDials a guest call)), and the cockpit's "
              "top SceneCockpit::Draw sub_820DC498 (inside the diagnostics' wrap) with "
              "SceneCockpit::DrawBody sub_820DC350 (CarLighting::Gather and SceneInterior::Draw "
              "as plain C++), CockpitPoints::Draw sub_820DBD38, DrawFenders sub_820DB8C8, "
              "DrawCabinWindow sub_820DB980, DrawDoorWindow sub_820DBA38 and DrawHeadlights "
              "sub_820DBB88 (no adapters: only the cockpit's top reaches them), and the traffic "
              "body's top SceneTraffic::Draw sub_820E6A40 (inside the diagnostics' wrap; "
              "CarLighting::Gather and Model::DrawShapeDepth as plain C++) with "
              "TrafficPoints::Draw sub_820E6820, DrawBody sub_820E55A0, DrawWindow "
              "sub_820E5668, DrawTailLights sub_820E5B08 and DrawHeadlights sub_820E61D8 (no "
              "adapters: only it reaches them; their glows as the parts' lamps'), and the wheels' "
              "tires WheelPoints::DrawTire sub_820E8AE8 and DrawTireMorphed sub_820E9318 and set "
              "a's rims DrawRimA sub_820E9A28 and DrawRimAMorphed sub_820EA148 and set b's "
              "DrawRimB sub_820EA950 and the brakes DrawBrakes sub_820EB388 and "
              "DrawBrakesMorphed sub_820EBEB8 (no adapters: the wheels' lists call them), and the wheels' top SceneWheels::Draw "
              "sub_820ECE28 (inside the diagnostics' wrap) with WheelPoints::Draw sub_820ECA30 "
              "and DrawMorphed sub_820ECB68 (no adapters: only it calls them), and the root "
              "Car::DrawUpdate sub_820DAC68 (inside the diagnostics' wrap; the scene models' "
              "finishers and apply steps, ScenePools::ReleaseB, Vec4::Transform and the draws "
              "above as plain C++, each inside its wrap's checks, with --native_load_poll) (the matrix "
              "functions, memcpy, CarPart::Draw, PartModel::FlipCulling and SetAlpha, the "
              "tire's weights, "
              "Morph::SetUp and Model::MorphShape as plain C++) "
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
                                              uint32_t sp, uint32_t lr, uint64_t r28) {
    PPCContext& ctx = guest->ctx;
    const uint64_t saved = ctx.r28.u64;
    ctx.r28.u64 = r28;
    const uint64_t r3 = call(function, regs, count, sp, lr);
    ctx.r28.u64 = saved;
    return r3;
  };
  calls.call_r24 = [guest, call = calls.call](uint32_t function, const uint64_t* regs, int count,
                                              uint32_t sp, uint32_t lr, uint64_t r24) {
    PPCContext& ctx = guest->ctx;
    const uint64_t saved = ctx.r24.u64;
    ctx.r24.u64 = r24;
    const uint64_t r3 = call(function, regs, count, sp, lr);
    ctx.r24.u64 = saved;
    return r3;
  };
  calls.call_r24_r28 = [guest, call = calls.call](uint32_t function, const uint64_t* regs,
                                                  int count, uint32_t sp, uint32_t lr,
                                                  uint64_t r24, uint64_t r28) {
    PPCContext& ctx = guest->ctx;
    const uint64_t saved24 = ctx.r24.u64, saved28 = ctx.r28.u64;
    ctx.r24.u64 = r24;
    ctx.r28.u64 = r28;
    const uint64_t r3 = call(function, regs, count, sp, lr);
    ctx.r24.u64 = saved24;
    ctx.r28.u64 = saved28;
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

bool CarDrawShadowsCalleesNative() {
  return CarDrawPartsCalleesNative() && CarDrawLightingCalleesNative();
}

void CarDrawSceneParts(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawShadowsCalleesNative()) {
    __imp__sub_820E2EE8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const car_draw::Left left =
      car_draw::ScenePartsDraw(base, calls, ctx.r1.u32, ctx.r3.u64, ctx.r4.u32, ctx.r24.u32);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}


// --- The interior ---

bool CarDrawInteriorCalleesNative() {
  return CarDrawPartsCalleesNative();
}

void CarDrawSceneInterior(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawInteriorCalleesNative() || !ITC_FLAG(native_accessories) ||
      !ITC_FLAG(native_scene_models)) {
    __imp__sub_820E53F0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const car_draw::Left left = car_draw::SceneInteriorDraw(
      base, calls, ctx.r1.u32, ctx.r3.u64, ctx.r4.u32, ctx.r24.u32, ctx.r28.u32);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}

// --- The cockpit ---

bool CarDrawCockpitCalleesNative() {
  return CarDrawInteriorCalleesNative() && ITC_FLAG(native_accessories) &&
         ITC_FLAG(native_scene_models) && CarDrawLightingCalleesNative();
}

void CarDrawSceneCockpit(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawCockpitCalleesNative()) {
    __imp__sub_820DC498(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const car_draw::Left left = car_draw::SceneCockpitDraw(
      base, calls, ctx.r1.u32, ctx.r3.u64, ctx.r4.u32, ctx.r24.u32, ctx.r28.u32);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}

// --- The traffic body ---

void CarDrawSceneTraffic(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawShadowsCalleesNative()) {
    __imp__sub_820E6A40(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const car_draw::Left left =
      car_draw::SceneTrafficDraw(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r24.u32);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}

// --- The wheels ---

bool CarDrawWheelsCalleesNative() {
  return CarDrawPartsCalleesNative() && CarDrawMorphCalleesNative();
}

void CarDrawSceneWheels(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawWheelsCalleesNative()) {
    __imp__sub_820ECE28(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  const car_draw::Left left =
      car_draw::SceneWheelsDraw(base, calls, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r24.u64);
  ctx.lr = lr;
  LeaveLeft(ctx, left);
}

// --- The root ---

bool CarDrawUpdateCalleesNative() {
  return CarDrawHeightCalleesNative() && CarDrawLightKitCalleesNative() &&
         CarDrawShadowsCalleesNative() && CarDrawCockpitCalleesNative() &&
         CarDrawWheelsCalleesNative() && ITC_FLAG(native_load_poll) && LoadPollCalleesNative() &&
         ITC_FLAG(native_second_set);
}

void CarDrawUpdate(PPCContext& ctx, uint8_t* base) {
  if (!CarDrawUpdateCalleesNative()) {
    __imp__sub_820DAC68(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  SceneryDrawGuest guest{ctx, base};
  const car_draw::Calls calls = CarDrawCalls(&guest);
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  const second_set::ReleaseCalls release = SecondSetReleaseCalls(ctx, base);
  const car_draw::Left left =
      car_draw::CarDrawUpdate(base, calls, poll, release, ctx.r1.u32, ctx.r3.u64, ctx.r24.u64,
                              !CarDrawBlinkTimersHeld());
  ctx.lr = lr;
  LeaveLeft(ctx, left);
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
// (2026-11-01; its only caller, CarLighting::Gather, calls the plain C++),
// WorkMem::AllocVertexBuffer sub_8258A9E8 and Model::QueueJobRelease
// sub_82594E88 (2026-11-03; their caller in play, Model::MorphShape, calls
// the plain C++), LightKit::BlinkLevel sub_820DC4B0 (2026-11-03; its
// callers, the neons, are plain C++), the parts' small draws
// PartsPoints::DrawFrontBumperLights sub_820DE1C8 to DrawWithShadow
// sub_820E1260, the head and tail lights PartsPoints::DrawHeadlights
// sub_820E1690 and DrawTailLights sub_820DF8B0 and the shadows
// PartsPoints::DrawShadows sub_820E1D88 (2026-11-03; their callers, the
// parts' top, are plain C++), and the interior's leaves
// InteriorDials::DrawFaces sub_820E3170, DrawDigitalSpeed sub_820E39F8 and
// DrawReadouts sub_820E51D0, InteriorMeters::DrawFaces sub_820E3AD8,
// InteriorPoints::DrawSeats sub_820E3CF0 and DrawRollCage sub_820E3548
// (2026-11-04; their callers, the interior's lists, are plain C++), and the
// traffic body's TrafficPoints::DrawBody sub_820E55A0, DrawWindow
// sub_820E5668, DrawTailLights sub_820E5B08 and DrawHeadlights sub_820E61D8
// and TrafficPoints::Draw sub_820E6820 (2026-11-04; their callers,
// TrafficPoints::Draw and SceneTraffic::Draw, are plain C++), Model::Draw
// sub_820D04E8 and CarLighting::Gather sub_820C9170 (2026-11-04; their
// callers in play, CarPart::Draw, the lamps, the seats, the shadows, the
// cockpit's body and SceneTraffic::Draw, are plain C++), and
// Model::MorphShape sub_82595110 (2026-11-04; its callers, the morphed wheel
// draws and WheelPoints::DrawBrakes, are plain C++), and the wheels' tires,
// rims and brakes WheelPoints::DrawTire sub_820E8AE8, DrawTireMorphed
// sub_820E9318, DrawRimA sub_820E9A28, DrawRimAMorphed sub_820EA148, DrawRimB
// sub_820EA950, DrawBrakes sub_820EB388 and DrawBrakesMorphed sub_820EBEB8
// (2026-11-04; their callers, WheelPoints::Draw and DrawMorphed, are plain
// C++).
ITC_CAR_DRAW(820D06F0, CarDrawCarPartDraw)
#undef ITC_CAR_DRAW
