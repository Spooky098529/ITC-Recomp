// The car objects' makers natively: adapters in front of src/car_makers/
// (docs/functions/car-makers.md), with the register steps the harness
// shares (hooks/car_makers.h). Each leaves r3 as the original does.

#include "hooks/car_makers.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "car_makers/car_makers.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/load_poll.h"
#include "hooks/stand_models.h"

ITC_FLAG_DECLARE(bool, native_scene_models);
ITC_FLAG_DECLARE(bool, native_car_specs);
ITC_FLAG_DECLARE(bool, native_race_setup);
ITC_FLAG_DECLARE(bool, native_save_data);
ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_math);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_ui_movies_game);
ITC_FLAG_DECLARE(bool, native_front_end_car_draw);

ITC_FLAG_BOOL(native_car_makers, true, "ITC",
              "Run the car objects' makers (docs/functions/car-makers.md): CarModels::BlockOf "
              "sub_8215AAD0 (its case sub_8215AB04 merged), SceneModelRecord::FromCar "
              "sub_8215B878 with SetPaint sub_8215AC70, SetWheels sub_8215B238, SetChoice "
              "sub_8215B1A0 and CarRecord::SetBytes sub_8215B0E0 (with CarRecord::ResetBytes "
              "sub_820D0900; these five without adapters: only FromCar reaches them) (with "
              "--native_scene_models and --native_car_specs; memcpy and AeroParts::Get82 its "
              "originals where theirs decline); GarageCar::Reset sub_82110448, "
              "GarageCars::StaticInit sub_825D9EF0, CarData::FromGarageCar sub_82179A60 (no "
              "adapter: its native callers call it) and TuningScreen::SetUpCarData "
              "sub_821DF2F8 (with --native_race_setup, "
              "--native_save_data, --native_car_specs and --native_car_physics; their callees' "
              "originals where theirs decline); GarageStage::FitCar sub_82178C40 (with "
              "--native_car_physics and --native_math), GarageStage::CopyRecord sub_8217FC60 "
              "(with --native_vehicle_accessors and --native_scene_models; "
              "SceneModelRecord::Copy's adapter for the copy onto itself; no adapter: SetCar's "
              "and SetCarClass' plain C++ call it), GarageStage::ReloadModel sub_8217F880 (with --native_vehicle_accessors and "
              "--native_scene_models; SceneModel::PollLoad, ScenePools::ReleaseB and SetUpB and "
              "CarScene::Update through their adapters), and the stand's busy "
              "marker sub_8219ED10 and sub_8219EDB0 (with --native_ui_movies_game; the named "
              "animations through their overrides), GarageStage::ReapplyRecord sub_82178DF8 and "
              "ReapplyRecordNoDecals sub_82178E60 (with --native_scene_models, "
              "--native_front_end_car_draw and the load poll's flags; SceneModel::ApplyRecord's "
              "adapter for the race's path; no adapters: SetCar's and SetCarClass' plain C++ call "
              "them), GarageStage::SetCar sub_8217FD58 and SetCarClass "
              "sub_8217FDC8 (with all of these makers' flags), and the makers by id above them, "
              "GarageStage::ShowCarById sub_82182C90 and ShowCarByIdWithClass sub_82182D30 "
              "(with --native_race_setup too) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=car_makers)");

namespace hooks {

namespace cm = car_makers;

ITC_REGS_STEP(CarMakersBlockOf) {
  ctx.r3.u64 = cm::BlockOf(ctx.r3.u32);
}

ITC_REGS_STEP(CarMakersSetPaint) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::SetPaint(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarMakersSetWheels) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::SetWheels(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarMakersSetChoice) {
  cm::SetChoice(ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32);
}

ITC_REGS_STEP(CarMakersSetBytes) {
  ctx.r3.u64 = cm::SetBytes(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CarMakersFromCar) {
  if (!ITC_FLAG(native_scene_models) || !ITC_FLAG(native_car_specs)) {
    lifted::original::c8215B878(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  ctx.r3.u64 = cm::FromCar(calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u64, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

bool CarMakersGarageCalleesNative() {
  return ITC_FLAG(native_race_setup) && ITC_FLAG(native_save_data) &&
         ITC_FLAG(native_car_specs) && ITC_FLAG(native_car_physics);
}

ITC_REGS_STEP(CarMakersGarageCarReset) {
  if (!CarMakersGarageCalleesNative()) {
    lifted::original::c82110448(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  ctx.r3.u64 = cm::GarageCarReset(calls, ctx.r3.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(CarMakersStaticInit) {
  if (!CarMakersGarageCalleesNative()) {
    lifted::original::c825D9EF0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  ctx.r3.u64 = cm::GarageCarsStaticInit(calls, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(CarMakersFromGarageCar) {
  if (!CarMakersGarageCalleesNative()) {
    lifted::original::c82179A60(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  double f1 = 0.0;
  ctx.r3.u64 = cm::FromGarageCar(calls, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(CarMakersSetUpCarData) {
  if (!CarMakersGarageCalleesNative()) {
    lifted::original::c821DF2F8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  double f1 = 0.0;
  ctx.r3.u64 = cm::TuningScreenSetUpCarData(calls, ctx.r3.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(CarMakersFitCar) {
  if (!ITC_FLAG(native_car_physics) || !ITC_FLAG(native_math)) {
    lifted::original::c82178C40(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  double f1 = 0.0;
  ctx.r3.u64 = cm::GarageStageFitCar(calls, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(CarMakersCopyRecord) {
  if (!ITC_FLAG(native_vehicle_accessors) || !ITC_FLAG(native_scene_models)) {
    lifted::original::c8217FC60(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  ctx.r3.u64 = cm::GarageStageCopyRecord(calls, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

bool CarMakersReapplyCalleesNative() {
  return ITC_FLAG(native_scene_models) && ITC_FLAG(native_front_end_car_draw) &&
         FrontEndCarDrawCalleesNative();
}

namespace {

void Reapply(lifted::Regs& ctx, bool decals) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx);
  const uint64_t r3 =
      cm::GarageStageReapplyRecord(calls, apply, ctx.r3.u32, ctx.r1.u32, decals);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

}  // namespace

ITC_REGS_STEP(CarMakersReapplyRecord) {
  if (!CarMakersReapplyCalleesNative()) {
    lifted::original::c82178DF8(ctx);
    return;
  }
  Reapply(ctx, true);
}

ITC_REGS_STEP(CarMakersReapplyRecordNoDecals) {
  if (!CarMakersReapplyCalleesNative()) {
    lifted::original::c82178E60(ctx);
    return;
  }
  Reapply(ctx, false);
}

bool CarMakersSetCarClassCalleesNative() {
  return ITC_FLAG(native_car_specs) && ITC_FLAG(native_vehicle_accessors) &&
         CarMakersReapplyCalleesNative();
}

bool CarMakersSetCarCalleesNative() {
  return CarMakersSetCarClassCalleesNative() && ITC_FLAG(native_car_physics) &&
         ITC_FLAG(native_math);
}

ITC_REGS_STEP(CarMakersSetCar) {
  if (!CarMakersSetCarCalleesNative()) {
    lifted::original::c8217FD58(ctx);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx);
  double f1 = ctx.f1.f64;
  const uint64_t r3 = cm::GarageStageSetCar(calls, apply, ctx.r3.u32, ctx.r4.u32,
                                            ctx.r5.u64, ctx.r1.u32, &f1);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(CarMakersSetCarClass) {
  if (!CarMakersSetCarClassCalleesNative()) {
    lifted::original::c8217FDC8(ctx);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx);
  const uint64_t r3 = cm::GarageStageSetCarClass(calls, apply, ctx.r3.u32, ctx.r4.u32,
                                                 ctx.r5.u64, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

bool CarMakersShowCarByIdCalleesNative(bool with_class) {
  return ITC_FLAG(native_race_setup) &&
         (with_class ? CarMakersSetCarClassCalleesNative() : CarMakersSetCarCalleesNative());
}

namespace {

void ShowCarById(lifted::Regs& ctx, bool with_class) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx);
  double f1 = ctx.f1.f64;
  const uint64_t r3 =
      cm::GarageStageShowCarById(calls, apply, ctx.r3.u64, ctx.r1.u32, with_class, &f1);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

}  // namespace

ITC_REGS_STEP(CarMakersShowCarById) {
  if (!CarMakersShowCarByIdCalleesNative(false)) {
    lifted::original::c82182C90(ctx);
    return;
  }
  ShowCarById(ctx, false);
}

ITC_REGS_STEP(CarMakersShowCarByIdWithClass) {
  if (!CarMakersShowCarByIdCalleesNative(true)) {
    lifted::original::c82182D30(ctx);
    return;
  }
  ShowCarById(ctx, true);
}

ITC_REGS_STEP(CarMakersShowBusyMarker) {
  if (!ITC_FLAG(native_ui_movies_game)) {
    lifted::original::c8219ED10(ctx);
    return;
  }
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  ctx.r3.u64 = cm::ShowBusyMarker(calls, ctx.r1.u32);
}

ITC_REGS_STEP(CarMakersHideBusyMarker) {
  if (!ITC_FLAG(native_ui_movies_game)) {
    lifted::original::c8219EDB0(ctx);
    return;
  }
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  ctx.r3.u64 = cm::HideBusyMarker(calls, ctx.r1.u32);
}


ITC_REGS_STEP(CarMakersReloadModel) {
  if (!ITC_FLAG(native_vehicle_accessors) || !ITC_FLAG(native_scene_models)) {
    lifted::original::c8217F880(ctx);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx);
  const uint64_t r3 = cm::GarageStageReloadModel(calls, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_CAR_MAKERS(address, step)     \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_car_makers)) { \
    hooks::step(ctx);               \
  }

ITC_CAR_MAKERS(8215AAD0, CarMakersBlockOf)
ITC_CAR_MAKERS(8215B878, CarMakersFromCar)
ITC_CAR_MAKERS(82110448, CarMakersGarageCarReset)
ITC_CAR_MAKERS(825D9EF0, CarMakersStaticInit)
ITC_CAR_MAKERS(821DF2F8, CarMakersSetUpCarData)
ITC_CAR_MAKERS(82178C40, CarMakersFitCar)
ITC_CAR_MAKERS(8217FD58, CarMakersSetCar)
ITC_CAR_MAKERS(8217FDC8, CarMakersSetCarClass)
ITC_CAR_MAKERS(82182C90, CarMakersShowCarById)
ITC_CAR_MAKERS(82182D30, CarMakersShowCarByIdWithClass)
ITC_CAR_MAKERS(8219ED10, CarMakersShowBusyMarker)
ITC_CAR_MAKERS(8219EDB0, CarMakersHideBusyMarker)
ITC_CAR_MAKERS(8217F880, CarMakersReloadModel)
// sub_8215AB04 (BlockOf's case for id 1, the jump table's base) needs no
// adapter: only the original BlockOf reaches it. SceneModelRecord::SetPaint
// sub_8215AC70, SetWheels sub_8215B238, SetChoice sub_8215B1A0 and
// CarRecord::SetBytes sub_8215B0E0 (with sub_820D0900) have none: only
// SceneModelRecord::FromCar calls them, and its plain C++ calls theirs (their
// register steps above are the harness's). CarData::FromGarageCar
// sub_82179A60's adapter is removed: its callers in play,
// TuningScreen::SetUpCarData and CarList::DrawEntryPowerGraph, are native
// and call its plain C++ (its register step above is the harness's).
// GarageStage::CopyRecord sub_8217FC60, ReapplyRecord sub_82178DF8 and
// ReapplyRecordNoDecals sub_82178E60 have none since their callers,
// GarageStage::SetCar and SetCarClass, are native and call their plain C++
// (their register steps above are the harness's).

// For the fallback originals (docs/missing-functions.md): the lift.py
// batches call it by address (CarData::FromGarageCar: r4 a garage car, r5
// the data; the original where the garage's callees aren't native).
ITC_DIRECT_ENTRY(82179A60) { hooks::CarMakersFromGarageCar(ctx); }
