// The race's set-up natively: adapters in front of src/race_setup/
// (docs/functions/race-setup.md), with the register steps the harness shares
// (hooks/race_setup.h).

#include "hooks/race_setup.h"

#include "base/flags.h"
#include "race_setup/race_setup.h"

ITC_FLAG_BOOL(native_race_setup, true, "ITC",
              "Run the race's set-up (docs/functions/race-setup.md: the car object's parts "
              "Car::ResetDecals40 sub_820CC4A0, Car::ResetDecals6 sub_820CCBF0, "
              "Car::ResetSettings sub_820CD760, sub_820CCC70, CarTuning::Reset sub_820CD618, "
              "CarDecal::FromBuild sub_820CDC18 (with its setters sub_820CC450 to sub_820CC478 "
              "merged), Car::Clear sub_820CD7F0 and Car::Init sub_820CD5D8) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=race_setup)");

namespace hooks {

namespace rs = race_setup;

void RaceSetupResetDecals40(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::ResetDecals40(base, ctx.r3.u32);
}

void RaceSetupResetDecals6(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::ResetDecals6(base, ctx.r3.u32);
}

void RaceSetupResetSettings(PPCContext& ctx, uint8_t* base) {
  rs::ResetSettings(base, ctx.r3.u32);
}

void RaceSetupReset1568(PPCContext& ctx, uint8_t* base) {
  rs::Reset1568(base, ctx.r3.u32);
}

void RaceSetupTuningReset(PPCContext& ctx, uint8_t* base) {
  rs::TuningReset(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSetupDecalFromBuild(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::DecalFromBuild(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSetupCarClear(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CarClear(base, ctx.r3.u32);
}

void RaceSetupCarInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CarInit(base, ctx.r3.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_SETUP(address, step)     \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_race_setup)) {   \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_RACE_SETUP(820CC4A0, RaceSetupResetDecals40)
ITC_RACE_SETUP(820CCBF0, RaceSetupResetDecals6)
ITC_RACE_SETUP(820CD760, RaceSetupResetSettings)
ITC_RACE_SETUP(820CCC70, RaceSetupReset1568)
ITC_RACE_SETUP(820CD618, RaceSetupTuningReset)
ITC_RACE_SETUP(820CDC18, RaceSetupDecalFromBuild)
ITC_RACE_SETUP(820CD7F0, RaceSetupCarClear)
ITC_RACE_SETUP(820CD5D8, RaceSetupCarInit)
