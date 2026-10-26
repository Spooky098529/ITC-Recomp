// The race's set-up natively: adapters in front of src/race_setup/
// (docs/functions/race-setup.md), with the register steps the harness shares
// (hooks/race_setup.h).

#include "hooks/race_setup.h"

#include "base/flags.h"
#include "race_setup/race_setup.h"

ITC_FLAG_BOOL(native_race_setup, true, "ITC",
              "Run the race's set-up (docs/functions/race-setup.md: the car object's parts "
              "Car::ResetDecals40 sub_820CC4A0, Car::ResetDecals6 sub_820CCBF0, "
              "Car::ResetSettings sub_820CD760, sub_820CCC70 (these four without adapters: "
              "reached from Car::Clear), CarTuning::Reset sub_820CD618 (no adapter: from Car::Clear "
              "and Car::SetModel), CarDecal::FromBuild sub_820CDC18 (no adapter: from "
              "Car::FromBuild; with its setters sub_820CC450 to sub_820CC478 merged), Car::Clear sub_820CD7F0, Car::Init sub_820CD5D8, Car::SetModel sub_820CD8A8, "
              "Car::FromBuild sub_820CDCE0; the race data's pieces sub_82101630, "
              "Race::AddStage sub_82101828, Stages::AddRacer sub_820FFAB8, RaceData::RaceData "
              "sub_821021B8 (its entries' sub_82101D20 through the array helper, without an "
              "adapter), sub_820FF558, RunningMean::Add sub_82101698, Time::Pack sub_82101700, "
              "sub_82110AC0; RaceEvent::FirstOwned sub_821430A0, RaceEvent::Current "
              "sub_821434E8 and RaceEvent::Count sub_821434F8) natively instead of "
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

void RaceSetupSetModel(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::SetModel(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSetupFromBuild(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::FromBuild(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSetupRivalLocation(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::RivalLocation(ctx.r3.s32, ctx.r4.u64);
}

void RaceSetupAddStage(PPCContext& ctx, uint8_t* base) {
  rs::AddStage(base, ctx.r3.u32);
}

void RaceSetupStagesAddRacer(PPCContext& ctx, uint8_t* base) {
  rs::StagesAddRacer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32);
}

void RaceSetupRaceDataConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RaceDataConstruct(base, ctx.r3.u32);
}

void RaceSetupLapRecordReset(PPCContext& ctx, uint8_t* base) {
  rs::LapRecordReset(base, ctx.r3.u32);
}

void RaceSetupRunningMeanAdd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::RunningMeanAdd(base, ctx.r3.u32, ctx.f1.f64, ctx.r5.u32);
}

void RaceSetupTimePack(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::TimePack(ctx.r3.u64);
}

void RaceSetupCountRace(PPCContext& ctx, uint8_t* base) {
  rs::CountRace(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSetupEventFirstOwned(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = rs::EventFirstOwned(base, ctx.r3.u32);
}

void RaceSetupEventCurrent(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::EventCurrent(base);
}

void RaceSetupEventCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::EventCount(base);
}

void RaceSetupRaceDataEntryConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::RaceDataEntryConstruct(base, ctx.r3.u32);
}

}  // namespace hooks

// Car::ResetDecals40, Car::ResetDecals6, Car::ResetSettings and sub_820CCC70
// have no adapters: only Car::Clear calls them, and its plain C++ calls
// theirs (the flag switches Car::Clear); nor CarTuning::Reset (from
// Car::Clear and Car::SetModel) or CarDecal::FromBuild (from Car::FromBuild).

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

ITC_RACE_SETUP(820CD7F0, RaceSetupCarClear)
ITC_RACE_SETUP(820CD5D8, RaceSetupCarInit)
ITC_RACE_SETUP(820CD8A8, RaceSetupSetModel)
ITC_RACE_SETUP(820CDCE0, RaceSetupFromBuild)
ITC_RACE_SETUP(82101630, RaceSetupRivalLocation)
ITC_RACE_SETUP(82101828, RaceSetupAddStage)
ITC_RACE_SETUP(820FFAB8, RaceSetupStagesAddRacer)
ITC_RACE_SETUP(821021B8, RaceSetupRaceDataConstruct)
ITC_RACE_SETUP(820FF558, RaceSetupLapRecordReset)
ITC_RACE_SETUP(82101698, RaceSetupRunningMeanAdd)
ITC_RACE_SETUP(82101700, RaceSetupTimePack)
ITC_RACE_SETUP(82110AC0, RaceSetupCountRace)
ITC_RACE_SETUP(821430A0, RaceSetupEventFirstOwned)
ITC_RACE_SETUP(821434E8, RaceSetupEventCurrent)
ITC_RACE_SETUP(821434F8, RaceSetupEventCount)
// sub_82101D20 has no adapter: the array helper's plain C++
// (c_runtime::VectorConstruct, --native_c_runtime) calls its plain C++ when
// handed its address, as RaceData::RaceData's does.
