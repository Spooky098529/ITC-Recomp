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
              "adapter), sub_820FF558 (no adapter: from sub_82101EB0 and CourseRecord::Reset), "
              "RunningMean::Add sub_82101698, Time::Pack sub_82101700, "
              "sub_82110AC0; RaceEvent::FirstOwned sub_821430A0, RaceEvent::Current "
              "sub_821434E8, RaceEvent::Count sub_821434F8, RaceEvent::CurrentFirstOwned "
              "sub_821434C8; sub_82101EB0 and CourseRecord::Reset sub_82105C90; CourseRoutes::Get sub_820FFF50, "
              "its table written into its stack frame); Stages::Reset sub_82101860, RacerRecord::Init "
              "sub_820FF808, Route::Set sub_82101A10, Route::Reset sub_82101990, "
              "Race::StartPosition sub_82101A98 (with its switch piece sub_82101ADC); "
              "Racers::Reset sub_820FF978, Racers::Add sub_820FF860, Race::AddRacer "
              "sub_821017C0, sub_82101F68, Race::SetRoute sub_82101D38, Stages::SetRoute "
              "sub_82101E90; RaceData::Reset sub_821020F0 (with sub_82101FD0, no adapter), "
              "RaceData::ResetGlobal sub_821021A8, Racers::SetStart sub_82101D50, "
              "Race::PlaceRacers sub_82102048 (its save data branch, sub_82105BF8, the "
              "original's)) natively instead of "
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

void RaceSetupEventCurrentFirstOwned(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = rs::EventCurrentFirstOwned(base);
}

void RaceSetupEntryReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::EntryReset(base, ctx.r3.u32);
  ctx.r3.u64 = ctx.r1.u64 - 144 + 80;
  ctx.f1.f64 = double(guest::LoadFloat(base, rs::kZero));
}

void RaceSetupCourseRecordReset(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::CourseRecordReset(base, ctx.r3.u32);
}

void RaceSetupEventCurrent(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::EventCurrent(base);
}

void RaceSetupEventCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::EventCount(base);
}

void RaceSetupCourseRoutesGet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t source = 0;
  uint64_t result = 0;
  if (!rs::CourseRoutesGet(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32 - rs::kRoutesFrameBytes,
                           &source, &result)) {
    // memcpy's override (its original copy, or its records) from the
    // original's frame.
    const uint32_t r1 = ctx.r1.u32;
    ctx.r1.u64 = r1 - rs::kRoutesFrameBytes;
    ctx.r3.u64 = result;
    ctx.r4.u64 = source;
    ctx.r5.u64 = rs::kRouteBytes;
    ctx.lr = 0x82101624;
    sub_825A7410(ctx, base);
    ctx.r1.u64 = r1;
  }
  ctx.r3.u64 = result;
}

void RaceSetupRaceDataEntryConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::RaceDataEntryConstruct(base, ctx.r3.u32);
}

void RaceSetupStagesReset(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::StagesReset(base, ctx.r3.u32);
}

void RaceSetupRacerRecordInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacerRecordInit(base, ctx.r3.u32);
}

void RaceSetupRouteSet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RouteSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                    ctx.r1.u32 - rs::kRouteFrameBytes)) {
    __imp__sub_82101A10(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

void RaceSetupRouteReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RouteReset(base, ctx.r3.u32, ctx.r1.u32 - rs::kRouteFrameBytes)) {
    __imp__sub_82101990(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

void RaceSetupStartPosition(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  if (!rs::StartPosition(base, ctx.r3.u32, ctx.r1.u32 - rs::kStartFrameBytes, &result)) {
    __imp__sub_82101A98(ctx, base);
    return;
  }
  ctx.r3.u64 = result;
}

void RaceSetupRacersReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersReset(base, ctx.r3.u32);
}

void RaceSetupRacersAdd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RacersAddNative(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820FF860(ctx, base);
    return;
  }
  ctx.r3.s64 = rs::RacersAdd(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32);
}

void RaceSetupAddRacer(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::AddRacerNative(base, ctx.r3.u32)) {
    __imp__sub_821017C0(ctx, base);
    return;
  }
  ctx.r3.s64 = rs::AddRacer(base, ctx.r3.u32, ctx.r5.u32, ctx.f1.f64, ctx.r6.u32);
}

void RaceSetupRecordReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 128;
  if (!rs::RecordReset(base, ctx.r3.u32, frame)) {
    __imp__sub_82101F68(ctx, base);
    return;
  }
  // Route::Reset's r3 (its frame + 80), and f1 as the entries' resets leave
  // it (0.0).
  ctx.r3.u64 = ctx.r1.u64 - 128 - rs::kRouteFrameBytes + 80;
  ctx.f1.f64 = double(guest::LoadFloat(base, rs::kZero));
}

void RaceSetupRaceSetRoute(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RaceSetRoute(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - rs::kRouteFrameBytes)) {
    __imp__sub_82101D38(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

void RaceSetupStagesSetRoute(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::StagesSetRoute(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                          ctx.r1.u32 - rs::kRouteFrameBytes)) {
    __imp__sub_82101E90(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

void RaceSetupRaceDataReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RaceDataReset(base, ctx.r1.u32 - 112)) {
    __imp__sub_821020F0(ctx, base);
    return;
  }
  // The last sub_82101F68's r3 (its Route::Reset's frame + 80) and f1.
  ctx.r3.u64 = ctx.r1.u64 - 112 - 112 - 128 - rs::kRouteFrameBytes + 80;
  ctx.f1.f64 = double(guest::LoadFloat(base, rs::kZero));
}

void RaceSetupRaceDataResetGlobal(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RaceDataResetGlobal(base, ctx.r1.u32 - 112)) {
    __imp__sub_821021A8(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - 112 - 112 - 128 - rs::kRouteFrameBytes + 80;
  ctx.f1.f64 = double(guest::LoadFloat(base, rs::kZero));
}

namespace {

// Car::Refresh's original from Racers::SetStart's frame (`r1`), where its
// plain C++ refuses.
struct RefreshCall {
  PPCContext* ctx;
  uint8_t* base;
  uint32_t r1;
};

void GuestRefresh(void* user, uint32_t car) {
  RefreshCall& call = *static_cast<RefreshCall*>(user);
  PPCContext& ctx = *call.ctx;
  const uint64_t r1 = ctx.r1.u64;
  ctx.r1.u64 = call.r1;
  ctx.r3.u64 = car;
  ctx.lr = 0x82101DC8;
  sub_820C4998(ctx, call.base);
  ctx.r1.u64 = r1;
}

}  // namespace

void RaceSetupSetStart(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 4096;
  if (!rs::SetStartNative(base, ctx.r3.u32, ctx.r4.u32, frame)) {
    __imp__sub_82101D50(ctx, base);
    return;
  }
  RefreshCall call{&ctx, base, frame};
  ctx.r3.u64 = rs::SetStart(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame, GuestRefresh, &call);
}

void RaceSetupPlaceRacers(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 128;
  if (!rs::PlaceRacersNative(base, frame)) {
    __imp__sub_82102048(ctx, base);
    return;
  }
  RefreshCall call{&ctx, base, frame - 4096};
  ctx.r3.u64 = rs::PlaceRacers(base, frame, GuestRefresh, &call);
}

}  // namespace hooks

// Car::ResetDecals40, Car::ResetDecals6, Car::ResetSettings and sub_820CCC70
// have no adapters: only Car::Clear calls them, and its plain C++ calls
// theirs (the flag switches Car::Clear); nor CarTuning::Reset (from
// Car::Clear and Car::SetModel), CarDecal::FromBuild (from Car::FromBuild) or
// sub_820FF558 (from sub_82101EB0 and CourseRecord::Reset); nor sub_82101EB0
// (from sub_82101F68), Route::Set (from Race::SetRoute and Stages::SetRoute)
// or Racers::Add (from Race::AddRacer): their register steps stay for the
// harness.

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
ITC_RACE_SETUP(82101698, RaceSetupRunningMeanAdd)
ITC_RACE_SETUP(82101700, RaceSetupTimePack)
ITC_RACE_SETUP(82110AC0, RaceSetupCountRace)
ITC_RACE_SETUP(821430A0, RaceSetupEventFirstOwned)
ITC_RACE_SETUP(821434E8, RaceSetupEventCurrent)
ITC_RACE_SETUP(821434F8, RaceSetupEventCount)
ITC_RACE_SETUP(821434C8, RaceSetupEventCurrentFirstOwned)
ITC_RACE_SETUP(82105C90, RaceSetupCourseRecordReset)
ITC_RACE_SETUP(820FFF50, RaceSetupCourseRoutesGet)
ITC_RACE_SETUP(82101860, RaceSetupStagesReset)
ITC_RACE_SETUP(820FF808, RaceSetupRacerRecordInit)
ITC_RACE_SETUP(82101990, RaceSetupRouteReset)
ITC_RACE_SETUP(82101A98, RaceSetupStartPosition)
ITC_RACE_SETUP(820FF978, RaceSetupRacersReset)
ITC_RACE_SETUP(821017C0, RaceSetupAddRacer)
ITC_RACE_SETUP(82101F68, RaceSetupRecordReset)
ITC_RACE_SETUP(82101D38, RaceSetupRaceSetRoute)
ITC_RACE_SETUP(82101E90, RaceSetupStagesSetRoute)
ITC_RACE_SETUP(821020F0, RaceSetupRaceDataReset)
ITC_RACE_SETUP(821021A8, RaceSetupRaceDataResetGlobal)
ITC_RACE_SETUP(82101D50, RaceSetupSetStart)
ITC_RACE_SETUP(82102048, RaceSetupPlaceRacers)
// sub_82101FD0 has no adapter: only RaceData::Reset calls it.
// sub_82101ADC (a case of Race::StartPosition's switch, in its frame) needs
// no adapter: only the original Race::StartPosition reaches it.
// sub_82101D20 has no adapter: the array helper's plain C++
// (c_runtime::VectorConstruct, --native_c_runtime) calls its plain C++ when
// handed its address, as RaceData::RaceData's does.
