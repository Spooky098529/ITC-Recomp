// The race's update natively: adapters in front of src/race_update/
// (docs/functions/race-update.md), with the register steps the harness
// shares (hooks/race_update.h).

#include "hooks/race_update.h"

#include "base/flags.h"
#include "race_update/race_update.h"

ITC_FLAG_BOOL(native_race_update, true, "ITC",
              "Run the race's update (docs/functions/race-update.md: RaceScene::VaryCar "
              "sub_8215B330; RaceLights::Update sub_8215D008 with RaceLights::UploadDirectional "
              "sub_8215C698, no adapter: only Update's plain C++ calls it) natively instead of "
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
// RaceLights::UploadDirectional (sub_8215C698) has no adapter: its only
// caller, RaceLights::Update, is native; --native_race_update switches it.
