// The race's set-up natively: adapters in front of src/race_setup/
// (docs/functions/race-setup.md), with the register steps the harness shares
// (hooks/race_setup.h).

#include "hooks/race_setup.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "hooks/app_start_up.h"
#include "base/log.h"
#include "race_setup/race_setup.h"
#include "schema/race_setup.h"
#include "lifted/sdk.h"

ITC_FLAG_DECLARE(bool, native_car_physics);

ITC_FLAG_BOOL(native_race_setup, true, "ITC",
              "Run the race's set-up (docs/functions/race-setup.md: the car object's parts "
              "Car::ResetDecals40 sub_820CC4A0, Car::ResetDecals6 sub_820CCBF0, "
              "Car::ResetSettings sub_820CD760, sub_820CCC70 (these four without adapters: "
              "reached from Car::Clear), CarTuning::Reset sub_820CD618 (no adapter: from Car::Clear "
              "and Car::SetModel), CarDecal::FromBuild sub_820CDC18 (no adapter: from "
              "Car::FromBuild; with its setters sub_820CC450 to sub_820CC478 merged), Car::Clear sub_820CD7F0, Car::Init sub_820CD5D8, Car::SetupVehicle sub_820CD5C8 (Vehicle::SetupFromCar's plain C++ under --native_car_physics), Car::SetModel sub_820CD8A8, "
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
              "original's); Race::SetUpRival sub_82112DB0 with its switch piece sub_82112E2C (its "
              "kind 1, with sub_820FFA88, the original's); Race::SetUp sub_82115D98; Stages::Clear sub_82101900) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=race_setup)");

namespace hooks {

namespace rs = race_setup;

ITC_REGS_STEP(RaceSetupResetDecals40) {
  ctx.fpscr.disableFlushMode();
  rs::ResetDecals40(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupResetDecals6) {
  ctx.fpscr.disableFlushMode();
  rs::ResetDecals6(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupResetSettings) {
  rs::ResetSettings(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupReset1568) {
  rs::Reset1568(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupTuningReset) {
  rs::TuningReset(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceSetupDecalFromBuild) {
  ctx.fpscr.disableFlushMode();
  rs::DecalFromBuild(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceSetupCarClear) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CarClear(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupCarInit) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CarInit(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupSetupVehicle) {
  const uint32_t frame = ctx.r1.u32 - 368;  // SetupFromCar's, from the same r1
  double f1 = 0.0;
  if (ITC_FLAG(native_car_physics)) {
    ctx.fpscr.disableFlushMode();
    if (rs::CarSetupVehicle(ctx.r3.u32, ctx.r4.u32, frame, &f1)) {
      ctx.f1.f64 = f1;
      ctx.r3.u64 = frame - 1008 + 352;
      return;
    }
  }
  const uint64_t car = ctx.r3.u64;
  // (r11 = the car, as the original's caller leaves it: a leftover, the
  // original doesn't read it)
  ctx.r3.u64 = ctx.r4.u64;
  ctx.r4.u64 = car;
  lifted::original::c820C27E8(ctx);
}

ITC_REGS_STEP(RaceSetupSetModel) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::SetModel(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceSetupFromBuild) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::FromBuild(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceSetupRivalLocation) {
  ctx.r3.u64 = rs::RivalLocation(ctx.r3.s32, ctx.r4.u64);
}

ITC_REGS_STEP(RaceSetupAddStage) {
  rs::AddStage(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupStagesAddRacer) {
  rs::StagesAddRacer(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32);
}

ITC_REGS_STEP(RaceSetupRaceDataConstruct) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RaceDataConstruct(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupLapRecordReset) {
  rs::LapRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupRunningMeanAdd) {
  ctx.fpscr.disableFlushMode();
  rs::RunningMeanAdd(ctx.r3.u32, ctx.f1.f64, ctx.r5.u32);
}

ITC_REGS_STEP(RaceSetupTimePack) {
  ctx.r3.u64 = rs::TimePack(ctx.r3.u64);
}

ITC_REGS_STEP(RaceSetupCountRace) {
  rs::CountRace(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceSetupEventFirstOwned) {
  ctx.r3.s64 = rs::EventFirstOwned(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupEventCurrentFirstOwned) {
  ctx.r3.s64 = rs::EventCurrentFirstOwned();
}

ITC_REGS_STEP(RaceSetupEntryReset) {
  ctx.fpscr.disableFlushMode();
  rs::EntryReset(ctx.r3.u32);
  ctx.r3.u64 = ctx.r1.u64 - 144 + 80;
  ctx.f1.f64 = double(schema::RaceSetupGlobals::zero());
}

ITC_REGS_STEP(RaceSetupCourseRecordReset) {
  ctx.r3.u64 = rs::CourseRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupEventCurrent) {
  ctx.r3.u64 = rs::EventCurrent();
}

ITC_REGS_STEP(RaceSetupEventCount) {
  ctx.r3.u64 = rs::EventCount();
}

ITC_REGS_STEP(RaceSetupCourseRoutesGet) {
  ctx.fpscr.disableFlushMode();
  uint32_t source = 0;
  uint64_t result = 0;
  if (!rs::CourseRoutesGet(ctx.r3.u64, ctx.r4.u32, ctx.r1.u32 - rs::kRoutesFrameBytes,
                           &source, &result)) {
    // memcpy's override (its original copy, or its records) from the
    // original's frame.
    const uint32_t r1 = ctx.r1.u32;
    ctx.r1.u64 = r1 - rs::kRoutesFrameBytes;
    ctx.r3.u64 = result;
    ctx.r4.u64 = source;
    ctx.r5.u64 = rs::kRouteBytes;
    ctx.lr = 0x82101624;
    lifted::Enter(ctx, game::memcpy_825A7410);
    ctx.r1.u64 = r1;
  }
  ctx.r3.u64 = result;
}

ITC_REGS_STEP(RaceSetupRaceDataEntryConstruct) {
  ctx.fpscr.disableFlushMode();
  rs::RaceDataEntryConstruct(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupStagesReset) {
  ctx.r3.u64 = rs::StagesReset(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupRacerRecordInit) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacerRecordInit(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupRouteSet) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RouteSet(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                    ctx.r1.u32 - rs::kRouteFrameBytes)) {
    lifted::original::c82101A10(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

ITC_REGS_STEP(RaceSetupRouteReset) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RouteReset(ctx.r3.u32, ctx.r1.u32 - rs::kRouteFrameBytes)) {
    lifted::original::c82101990(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

ITC_REGS_STEP(RaceSetupStartPosition) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  if (!rs::StartPosition(ctx.r3.u32, ctx.r1.u32 - rs::kStartFrameBytes, &result)) {
    lifted::original::c82101A98(ctx);
    return;
  }
  ctx.r3.u64 = result;
}

ITC_REGS_STEP(RaceSetupRacersReset) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersReset(ctx.r3.u32);
}

ITC_REGS_STEP(RaceSetupRacersAdd) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RacersAddNative(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820FF860(ctx);
    return;
  }
  ctx.r3.s64 = rs::RacersAdd(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r6.u32);
}

ITC_REGS_STEP(RaceSetupAddRacer) {
  ctx.fpscr.disableFlushMode();
  if (!rs::AddRacerNative(ctx.r3.u32)) {
    lifted::original::c821017C0(ctx);
    return;
  }
  ctx.r3.s64 = rs::AddRacer(ctx.r3.u32, ctx.r5.u32, ctx.f1.f64, ctx.r6.u32);
}

ITC_REGS_STEP(RaceSetupRecordReset) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 128;
  if (!rs::RecordReset(ctx.r3.u32, frame)) {
    lifted::original::c82101F68(ctx);
    return;
  }
  // Route::Reset's r3 (its frame + 80), and f1 as the entries' resets leave
  // it (0.0).
  ctx.r3.u64 = ctx.r1.u64 - 128 - rs::kRouteFrameBytes + 80;
  ctx.f1.f64 = double(schema::RaceSetupGlobals::zero());
}

ITC_REGS_STEP(RaceSetupRaceSetRoute) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RaceSetRoute(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - rs::kRouteFrameBytes)) {
    lifted::original::c82101D38(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

ITC_REGS_STEP(RaceSetupStagesSetRoute) {
  ctx.fpscr.disableFlushMode();
  if (!rs::StagesSetRoute(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                          ctx.r1.u32 - rs::kRouteFrameBytes)) {
    lifted::original::c82101E90(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - rs::kRouteFrameBytes + 80;
}

ITC_REGS_STEP(RaceSetupRaceDataReset) {
  ctx.fpscr.disableFlushMode();
  if (!rs::RaceDataReset(ctx.r1.u32 - 112)) {
    lifted::original::c821020F0(ctx);
    return;
  }
  // The last sub_82101F68's r3 (its Route::Reset's frame + 80) and f1.
  ctx.r3.u64 = ctx.r1.u64 - 112 - 112 - 128 - rs::kRouteFrameBytes + 80;
  ctx.f1.f64 = double(schema::RaceSetupGlobals::zero());
}

ITC_REGS_STEP(RaceSetupRaceDataResetGlobal) {
  if (hooks::StartUpRecorded(ctx, 0x821021A8)) {  // the data load's harness
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (!rs::RaceDataResetGlobal(ctx.r1.u32 - 112)) {
    lifted::original::c821021A8(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r1.u64 - 112 - 112 - 128 - rs::kRouteFrameBytes + 80;
  ctx.f1.f64 = double(schema::RaceSetupGlobals::zero());
}

namespace {

// Car::Refresh's original from Racers::SetStart's frame (`r1`), where its
// plain C++ refuses.
struct RefreshCall {
  lifted::Regs* ctx;
  uint32_t r1;
  uint32_t lr;  // a return address of the original's Car::Refresh calls
};

void GuestRefresh(void* user, uint32_t car) {
  RefreshCall& call = *static_cast<RefreshCall*>(user);
  lifted::Regs& ctx = *call.ctx;
  const uint64_t r1 = ctx.r1.u64;
  ctx.r1.u64 = call.r1;
  ctx.r3.u64 = car;
  ctx.lr = call.lr;
  lifted::Enter(ctx, game::Car::Refresh);
  ctx.r1.u64 = r1;
}

}  // namespace

ITC_REGS_STEP(RaceSetupSetStart) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 4096;
  if (!rs::SetStartNative(ctx.r3.u32, ctx.r4.u32, frame)) {
    lifted::original::c82101D50(ctx);
    return;
  }
  RefreshCall call{&ctx, frame, 0x82101DC8};
  ctx.r3.u64 = rs::SetStart(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame, GuestRefresh, &call);
}

ITC_REGS_STEP(RaceSetupPlaceRacers) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 128;
  if (!rs::PlaceRacersNative(frame)) {
    lifted::original::c82102048(ctx);
    return;
  }
  RefreshCall call{&ctx, frame - 4096, 0x82101DC8};
  ctx.r3.u64 = rs::PlaceRacers(frame, GuestRefresh, &call);
}

ITC_REGS_STEP(RaceSetupSetUpRival) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 4112;
  if (!rs::SetUpRivalNative(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, frame)) {
    lifted::original::c82112DB0(ctx);
    return;
  }
  RefreshCall call{&ctx, frame, 0x82112EC4};
  ctx.r3.u64 = rs::SetUpRival(ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, frame, GuestRefresh, &call);
}

namespace {

// Race::SetUp's guest calls: the callees' originals, from its frame.
struct SetUpCall {
  lifted::Regs* ctx;
};

uint64_t SetUpGuestCall(void* user, uint32_t function, const uint64_t args[4], double f1,
                        uint32_t r1, uint32_t lr) {
  SetUpCall& call = *static_cast<SetUpCall*>(user);
  lifted::Regs& ctx = *call.ctx;
  const uint64_t saved_r1 = ctx.r1.u64;
  ctx.r1.u64 = r1;
  ctx.r3.u64 = args[0];
  ctx.r4.u64 = args[1];
  ctx.r5.u64 = args[2];
  ctx.r6.u64 = args[3];
  ctx.f1.f64 = f1;
  ctx.lr = lr;
  switch (function) {
    case 0x820C4998:
      lifted::Enter(ctx, game::Car::Refresh);
      break;
    case 0x82112DB0:
      lifted::Enter(ctx, game::Race::SetUpRival);
      break;
    case 0x82102048:
      lifted::Enter(ctx, game::Race::PlaceRacers);
      break;
    case 0x821017C0:
      lifted::Enter(ctx, game::Race::AddRacer);
      break;
    case 0x821021A8:
      lifted::Enter(ctx, game::RaceData::ResetGlobal);
      break;
    case 0x82101D38:
      lifted::Enter(ctx, game::Race::SetRoute);
      break;
    default:
      ITC_LOG_ERROR("race_setup: no guest call for {:08X}", function);
      break;
  }
  ctx.r1.u64 = saved_r1;
  return ctx.r3.u64;
}

}  // namespace

ITC_REGS_STEP(RaceSetupSetUp) {
  if (hooks::StartUpRecorded(ctx, 0x82115D98)) {
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 11984;
  if (!rs::SetUpNative(ctx.r3.u32, frame)) {
    lifted::original::c82115D98(ctx);
    return;
  }
  SetUpCall call{&ctx};
  const uint64_t save = ctx.r3.u64;
  rs::SetUp(ctx.r3.u32, ctx.r4.u64, frame, SetUpGuestCall, &call);
  ctx.r3.u64 = save + 48;
}

ITC_REGS_STEP(RaceSetupStagesClear) {
  ctx.r3.u64 = rs::StagesClear(ctx.r3.u32);
}

}  // namespace hooks

// Car::ResetDecals40, Car::ResetDecals6, Car::ResetSettings and sub_820CCC70
// have no adapters: only Car::Clear calls them, and its plain C++ calls
// theirs (the flag switches Car::Clear); nor CarTuning::Reset (from
// Car::Clear and Car::SetModel), CarDecal::FromBuild (from Car::FromBuild) or
// sub_820FF558 (from sub_82101EB0 and CourseRecord::Reset); nor sub_82101EB0
// (from sub_82101F68), Route::Set (from Race::SetRoute and Stages::SetRoute)
// or Racers::Add (from Race::AddRacer); nor Racers::SetStart (from
// Race::PlaceRacers), Race::StartPosition (from Racers::SetStart),
// Racers::Reset (from RaceData::Reset), Route::Reset (from sub_82101F68 and
// RaceData::Reset), RaceData::Reset (from RaceData::ResetGlobal),
// RacerRecord::Init (from Racers::Add, Racers::Reset, Racers::SetStart,
// Race::SetUpRival and Race::SetUp), sub_82101630 (from Race::SetUpRival),
// sub_82110AC0, RaceEvent::CurrentFirstOwned or RaceEvent::Count (from
// Race::SetUp), Stages::Reset (from Stages::Clear and RaceData::Reset):
// their register steps stay for the harness.

// Defines an adapter: the flag, else the original.
#define ITC_RACE_SETUP(address, step)     \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_setup)) { \
    hooks::step(ctx);               \
  }

ITC_RACE_SETUP(820CD7F0, RaceSetupCarClear)
ITC_RACE_SETUP(820CD5D8, RaceSetupCarInit)
ITC_RACE_SETUP(820CD5C8, RaceSetupSetupVehicle)
ITC_RACE_SETUP(820CD8A8, RaceSetupSetModel)
ITC_RACE_SETUP(820CDCE0, RaceSetupFromBuild)
ITC_RACE_SETUP(82101828, RaceSetupAddStage)
ITC_RACE_SETUP(820FFAB8, RaceSetupStagesAddRacer)
ITC_RACE_SETUP(821021B8, RaceSetupRaceDataConstruct)
ITC_RACE_SETUP(82101698, RaceSetupRunningMeanAdd)
ITC_RACE_SETUP(82101700, RaceSetupTimePack)
ITC_RACE_SETUP(821430A0, RaceSetupEventFirstOwned)
ITC_RACE_SETUP(821434E8, RaceSetupEventCurrent)
ITC_RACE_SETUP(82105C90, RaceSetupCourseRecordReset)
ITC_RACE_SETUP(820FFF50, RaceSetupCourseRoutesGet)
ITC_RACE_SETUP(821017C0, RaceSetupAddRacer)
ITC_RACE_SETUP(82101F68, RaceSetupRecordReset)
ITC_RACE_SETUP(82101D38, RaceSetupRaceSetRoute)
ITC_RACE_SETUP(82101E90, RaceSetupStagesSetRoute)
ITC_RACE_SETUP(821021A8, RaceSetupRaceDataResetGlobal)
ITC_RACE_SETUP(82102048, RaceSetupPlaceRacers)
ITC_RACE_SETUP(82112DB0, RaceSetupSetUpRival)
ITC_RACE_SETUP(82115D98, RaceSetupSetUp)
ITC_RACE_SETUP(82101900, RaceSetupStagesClear)
// sub_82112E2C (a case of Race::SetUpRival's switch, in its frame) needs no
// adapter: only the original Race::SetUpRival reaches it.
// sub_82101FD0 has no adapter: only RaceData::Reset calls it.
// sub_82101ADC (a case of Race::StartPosition's switch, in its frame) needs
// no adapter: only the original Race::StartPosition reaches it.
// sub_82101D20 has no adapter: the array helper's plain C++
// (c_runtime::VectorConstruct, --native_c_runtime) calls its plain C++ when
// handed its address, as RaceData::RaceData's does.

// For the fallback originals (docs/missing-functions.md): RaceStages::Reset,
// r3 = the list; r3 as the original leaves it (its step).
ITC_DIRECT_ENTRY(82101860) { hooks::RaceSetupStagesReset(ctx); }
