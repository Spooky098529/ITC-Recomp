// The front end's car draw natively: adapters in front of
// src/front_end_car_draw/ (docs/functions/front-end-car-draw.md), with the
// register steps the harness shares (hooks/front_end_car_draw.h).

#include "hooks/front_end_car_draw.h"

#include "base/flags.h"
#include "hooks/load_poll.h"
#include "hooks/scenery_lists.h"
#include "load_poll/load_poll.h"
#include "native/buffers.h"

ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_math);
ITC_FLAG_DECLARE(bool, native_menu_sounds);
ITC_FLAG_DECLARE(bool, native_load_poll);
ITC_FLAG_DECLARE(bool, native_scenery_lists);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_accessories);

ITC_FLAG_BOOL(native_front_end_car_draw, true, "ITC",
              "Run the front end's car draw (docs/functions/front-end-car-draw.md): so far "
              "Plates::Hide sub_820DF190 and Show sub_820DF180, GarageStage::IsBusy "
              "sub_82179BE0, SyncPlates sub_8217AFE8, SetDrawWord sub_82178B40, StepAngles "
              "sub_82179C88, ApplyPreset sub_8217D2B0 and Command sub_82180108, and "
              "GarageAnims::Add sub_8217F9E0, Run sub_8217D120, Turn sub_82179D78, Open "
              "sub_82179DF0 and Close sub_82179EA8 (the doors' runners left to their "
              "originals), GarageStage::SetLightKit sub_82179678 with "
              "SceneModel::ApplyLightKit sub_820D9D10, and Floor::Draw sub_820C8480 (with "
              "--native_scenery_lists and the command list's diagnostics off), "
              "GarageStage::SetLights sub_8217BA20, and GarageStage::PoseCar sub_8217CAB8 (with "
              "--native_matrix_math, --native_accessories and --native_memcpy), natively instead "
              "of through the translated code, "
              "with --native_vehicle_accessors, --native_math, --native_menu_sounds and "
              "--native_load_poll with its callees (else the originals; the same writes; "
              "checked by --run_harnesses=front_end_car_draw)");

namespace hooks {

namespace fe = front_end_car_draw;

bool FrontEndCarDrawCalleesNative() {
  return ITC_FLAG(native_vehicle_accessors) && ITC_FLAG(native_math) &&
         ITC_FLAG(native_menu_sounds) && ITC_FLAG(native_load_poll) && LoadPollCalleesNative();
}

fe::Calls FrontEndCarDrawCalls(PPCContext& ctx, uint8_t* base) {
  fe::Calls calls;
  calls.call = [&ctx, base](uint32_t function, const uint64_t* regs, int count,
                            const double* fregs, int fcount, uint32_t sp, uint32_t lr,
                            double* f1) -> uint64_t {
    const uint64_t stack = ctx.r1.u64;
    const uint64_t link = ctx.lr;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    PPCRegister* gprs[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                            &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < count; i++) {
      gprs[i]->u64 = regs[i];
    }
    ctx.fpscr.disableFlushMode();
    PPCRegister* fprs[4] = {&ctx.f1, &ctx.f2, &ctx.f3, &ctx.f4};
    for (int i = 0; i < fcount; i++) {
      fprs[i]->f64 = fregs[i];
    }
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    ctx.fpscr.disableFlushMode();
    if (f1 != nullptr) {
      *f1 = ctx.f1.f64;
    }
    return ctx.r3.u64;
  };
  return calls;
}

// The original unless the callees are native; else flush-to-zero off.
#define ITC_FRONT_END_ORIGINAL(address)  \
  if (!FrontEndCarDrawCalleesNative()) { \
    __imp__sub_##address(ctx, base);     \
    return;                              \
  }                                      \
  ctx.fpscr.disableFlushMode();

void FrontEndCarDrawPlatesHide(PPCContext& ctx, uint8_t* base) {
  fe::PlatesHide(base, ctx.r3.u32);
}

void FrontEndCarDrawPlatesShow(PPCContext& ctx, uint8_t* base) {
  fe::PlatesShow(base, ctx.r3.u32);
}

void FrontEndCarDrawIsBusy(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(82179BE0)
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  ctx.r3.u64 = fe::GarageStageIsBusy(base, calls, ctx.r3.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawSyncPlates(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(8217AFE8)
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  ctx.r3.u64 = fe::GarageStageSyncPlates(base, calls, ctx.r3.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawSetDrawWord(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(82178B40)
  ctx.r3.u64 = fe::GarageStageSetDrawWord(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void FrontEndCarDrawStepAngles(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  fe::GarageStageStepAngles(base, ctx.r4.u32, ctx.r5.u32);
}

void FrontEndCarDrawAnimsAdd(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(8217F9E0)
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageAnimsAdd(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                  ctx.r7.u32, ctx.f1.f64, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawApplyPreset(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(8217D2B0)
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageStageApplyPreset(base, calls, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32,
                                          ctx.r6.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawCommand(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(82180108)
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageStageCommand(base, calls, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawAnimsTurn(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = fe::GarageAnimsTurn(base, ctx.r4.u32);
}

void FrontEndCarDrawAnimsOpen(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(82179DF0)
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageAnimsOpen(base, calls, ctx.r4.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawAnimsClose(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(82179EA8)
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageAnimsClose(base, calls, ctx.r4.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawAnimsRun(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(8217D120)
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageAnimsRun(base, calls, ctx.r3.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawApplyLightKit(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(820D9D10)
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  ctx.r3.u64 = fe::SceneModelApplyLightKit(base, calls, ctx.r3.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawSetLightKit(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(82179678)
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  ctx.r3.u64 = fe::GarageStageSetLightKit(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                          ctx.r6.u32, ctx.f1.f64, ctx.f2.f64, ctx.f3.f64,
                                          ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawFloorDraw(PPCContext& ctx, uint8_t* base) {
  // Model::DrawShape's plain C++ only with its flag and without the command
  // list's diagnostics (as its adapter).
  if (!FrontEndCarDrawCalleesNative() || !ITC_FLAG(native_scenery_lists) ||
      SceneryListsSlotDiagnostics()) {
    __imp__sub_820C8480(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  SceneryDrawGuest guest{ctx, base};
  fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.draw = SceneryListsDrawCalls(&guest);
  ctx.r3.u64 = fe::FloorDraw(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r24.u32,
                             ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawSetLights(PPCContext& ctx, uint8_t* base) {
  ITC_FRONT_END_ORIGINAL(8217BA20)
  ctx.r3.u64 = fe::GarageStageSetLights(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void FrontEndCarDrawPoseCar(PPCContext& ctx, uint8_t* base) {
  // The matrix math's and Matrix::FromScaleRotationTranslation's plain C++
  // only where their adapters would run it.
  if (!FrontEndCarDrawCalleesNative() || !ITC_FLAG(native_matrix_math) ||
      !ITC_FLAG(native_accessories) || !native::Buffers_NativeMemcpy()) {
    __imp__sub_8217CAB8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const fe::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = fe::GarageStagePoseCar(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                      ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

#undef ITC_FRONT_END_ORIGINAL

}  // namespace hooks

#define ITC_FRONT_END_CAR_DRAW(address, step)    \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    if (!ITC_FLAG(native_front_end_car_draw)) {  \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    hooks::step(ctx, base);                      \
  }

ITC_FRONT_END_CAR_DRAW(82179BE0, FrontEndCarDrawIsBusy)
ITC_FRONT_END_CAR_DRAW(8217AFE8, FrontEndCarDrawSyncPlates)
ITC_FRONT_END_CAR_DRAW(82178B40, FrontEndCarDrawSetDrawWord)
ITC_FRONT_END_CAR_DRAW(8217D2B0, FrontEndCarDrawApplyPreset)
ITC_FRONT_END_CAR_DRAW(82180108, FrontEndCarDrawCommand)
ITC_FRONT_END_CAR_DRAW(8217D120, FrontEndCarDrawAnimsRun)
ITC_FRONT_END_CAR_DRAW(820D9D10, FrontEndCarDrawApplyLightKit)
ITC_FRONT_END_CAR_DRAW(82179678, FrontEndCarDrawSetLightKit)
ITC_FRONT_END_CAR_DRAW(820C8480, FrontEndCarDrawFloorDraw)
ITC_FRONT_END_CAR_DRAW(8217BA20, FrontEndCarDrawSetLights)
ITC_FRONT_END_CAR_DRAW(8217CAB8, FrontEndCarDrawPoseCar)

// No adapters (2026-11-04: their callers in play are native, which call
// their plain C++; the steps above are the harness's): Plates::Hide
// sub_820DF190 and Show sub_820DF180 (GarageStage::SyncPlates),
// GarageStage::StepAngles sub_82179C88 and GarageAnims::Add sub_8217F9E0
// (GarageStage::Command), GarageAnims::Turn sub_82179D78, Open sub_82179DF0
// and Close sub_82179EA8 (GarageAnims::Run).
