// The race's update natively: adapters in front of src/race_update/
// (docs/functions/race-update.md), with the register steps the harness
// shares (hooks/race_update.h).

#include "hooks/race_update.h"

#include "base/flags.h"
#include "hooks/course_scenery.h"
#include "race_update/race_update.h"

ITC_FLAG_BOOL(native_race_update, true, "ITC",
              "Run the race's update (docs/functions/race-update.md: RaceScene::VaryCar "
              "sub_8215B330; RaceLights::Update sub_8215D008 with RaceLights::UploadDirectional "
              "sub_8215C698, no adapter: only Update's plain C++ calls it; RaceScene::LoadScenery "
              "sub_8215B790, over the course scenery's plain C++ where it may run; RaceScene::RemakeCarModel "
              "sub_8215C0D8, its callees through their adapters) natively instead of "
              "through the translated code (the same words, bit for bit; checked by "
              "--run_harnesses=race_update)");

namespace hooks {

namespace ru = race_update;

void RaceUpdateVaryCar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ru::VaryCar(base, ctx.r3.u32, ctx.r4.s32, ctx.r5.s32, ctx.r6.s32);
}

void RaceUpdateLights(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r4.u64 = ru::LightsUpdate(base, ctx.r3.u32, ctx.r1.u32 - ru::kLightsFrame);
  ctx.r3.u64 = ctx.r3.u64 + 16 + 96 * 11;
}

void RaceUpdateLoadScenery(PPCContext& ctx, uint8_t* base) {
  CourseSceneryGuest guest{&ctx, base};
  const course_scenery::Calls calls = CourseSceneryCalls(&guest);
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const uint32_t r3 = race_update::LoadScenery(base, calls, ctx.r1.u32, ctx.r3.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

race_update::GuestCall RaceUpdateGuestCall(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t function, std::initializer_list<uint64_t> args, uint32_t frame,
                      uint32_t lr) {
    const uint64_t stack = ctx.r1.u64;
    PPCRegister* registers[5] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6, &ctx.r7};
    int i = 0;
    for (const uint64_t arg : args) {
      registers[i++]->u64 = arg;
    }
    ctx.r1.u64 = frame;
    ctx.lr = lr;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
}

void RaceUpdateRemakeCarModel(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const race_update::GuestCall call = RaceUpdateGuestCall(ctx, base);
  const uint32_t r3 = race_update::RemakeCarModel(base, call, ctx.r1.u32, ctx.r4.u32, ctx.r5.u32,
                                                  ctx.r6.u32, ctx.r7.u64);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_UPDATE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_race_update)) {  \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

// Their callers, Car::TickUpdate and RaceScene::Update, are recompiled.
ITC_RACE_UPDATE(8215B330, RaceUpdateVaryCar)
ITC_RACE_UPDATE(8215D008, RaceUpdateLights)
// RaceScene::RemakeCarModel: r3 = the car slots (not read), r4 = the scene,
// r5 = a slot, r6 = a car, r7 = the paint pick; r3 = 0 or 1. Also from
// RaceScene::SetUpCars (recompiled).
ITC_RACE_UPDATE(8215C0D8, RaceUpdateRemakeCarModel)
// RaceLights::UploadDirectional (sub_8215C698) has no adapter: its only
// caller, RaceLights::Update, is native; --native_race_update switches it.

// RaceScene::LoadScenery: r3 = the race scene; from RaceSceneMachine::OnStart
// (recompiled). The original where the course scenery's set-up can't run
// natively.
extern "C" REX_FUNC(sub_8215B790) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_race_update) || !hooks::CourseScenerySetUpNative()) {
    __imp__sub_8215B790(ctx, base);
    return;
  }
  hooks::RaceUpdateLoadScenery(ctx, base);
}
