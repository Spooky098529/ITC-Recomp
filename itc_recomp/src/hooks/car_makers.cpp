// The car objects' makers natively: adapters in front of src/car_makers/
// (docs/functions/car-makers.md), with the register steps the harness
// shares (hooks/car_makers.h). Each leaves r3 as the original does.

#include "hooks/car_makers.h"

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
              "SceneModelRecord::Copy's adapter for the copy onto itself), and the stand's busy "
              "marker sub_8219ED10 and sub_8219EDB0 (with --native_ui_movies_game; the named "
              "animations through their overrides), GarageStage::ReapplyRecord sub_82178DF8 and "
              "ReapplyRecordNoDecals sub_82178E60 (with --native_scene_models, "
              "--native_front_end_car_draw and the load poll's flags; SceneModel::ApplyRecord's "
              "adapter for the race's path), GarageStage::SetCar sub_8217FD58 and SetCarClass "
              "sub_8217FDC8 (with all of these makers' flags) natively instead of through the translated code "
              "(the same writes; checked by --run_harnesses=car_makers)");

namespace hooks {

namespace cm = car_makers;

void CarMakersBlockOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::BlockOf(ctx.r3.u32);
}

void CarMakersSetPaint(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::SetPaint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void CarMakersSetWheels(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cm::SetWheels(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarMakersSetChoice(PPCContext& ctx, uint8_t* base) {
  cm::SetChoice(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32);
}

void CarMakersSetBytes(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cm::SetBytes(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CarMakersFromCar(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_scene_models) || !ITC_FLAG(native_car_specs)) {
    __imp__sub_8215B878(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::FromCar(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r5.u64, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

bool CarMakersGarageCalleesNative() {
  return ITC_FLAG(native_race_setup) && ITC_FLAG(native_save_data) &&
         ITC_FLAG(native_car_specs) && ITC_FLAG(native_car_physics);
}

void CarMakersGarageCarReset(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersGarageCalleesNative()) {
    __imp__sub_82110448(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::GarageCarReset(base, calls, ctx.r3.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void CarMakersStaticInit(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersGarageCalleesNative()) {
    __imp__sub_825D9EF0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::GarageCarsStaticInit(base, calls, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

void CarMakersFromGarageCar(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersGarageCalleesNative()) {
    __imp__sub_82179A60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  double f1 = 0.0;
  ctx.r3.u64 = cm::FromGarageCar(base, calls, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

void CarMakersSetUpCarData(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersGarageCalleesNative()) {
    __imp__sub_821DF2F8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  double f1 = 0.0;
  ctx.r3.u64 = cm::TuningScreenSetUpCarData(base, calls, ctx.r3.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

void CarMakersFitCar(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_car_physics) || !ITC_FLAG(native_math)) {
    __imp__sub_82178C40(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  double f1 = 0.0;
  ctx.r3.u64 = cm::GarageStageFitCar(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &f1);
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

void CarMakersCopyRecord(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_vehicle_accessors) || !ITC_FLAG(native_scene_models)) {
    __imp__sub_8217FC60(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::GarageStageCopyRecord(base, calls, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  ctx.fpscr.disableFlushMode();
}

bool CarMakersReapplyCalleesNative() {
  return ITC_FLAG(native_scene_models) && ITC_FLAG(native_front_end_car_draw) &&
         FrontEndCarDrawCalleesNative();
}

namespace {

void Reapply(PPCContext& ctx, uint8_t* base, bool decals) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx, base);
  const uint64_t r3 =
      cm::GarageStageReapplyRecord(base, calls, apply, ctx.r3.u32, ctx.r1.u32, decals);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

}  // namespace

void CarMakersReapplyRecord(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersReapplyCalleesNative()) {
    __imp__sub_82178DF8(ctx, base);
    return;
  }
  Reapply(ctx, base, true);
}

void CarMakersReapplyRecordNoDecals(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersReapplyCalleesNative()) {
    __imp__sub_82178E60(ctx, base);
    return;
  }
  Reapply(ctx, base, false);
}

bool CarMakersSetCarClassCalleesNative() {
  return ITC_FLAG(native_car_specs) && ITC_FLAG(native_vehicle_accessors) &&
         CarMakersReapplyCalleesNative();
}

bool CarMakersSetCarCalleesNative() {
  return CarMakersSetCarClassCalleesNative() && ITC_FLAG(native_car_physics) &&
         ITC_FLAG(native_math);
}

void CarMakersSetCar(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersSetCarCalleesNative()) {
    __imp__sub_8217FD58(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx, base);
  double f1 = ctx.f1.f64;
  const uint64_t r3 = cm::GarageStageSetCar(base, calls, apply, ctx.r3.u32, ctx.r4.u32,
                                            ctx.r5.u64, ctx.r1.u32, &f1);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

void CarMakersSetCarClass(PPCContext& ctx, uint8_t* base) {
  if (!CarMakersSetCarClassCalleesNative()) {
    __imp__sub_8217FDC8(ctx, base);
    return;
  }
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  const stand_models::GuestCall apply = StandModelsGuestCall(ctx, base);
  const uint64_t r3 = cm::GarageStageSetCarClass(base, calls, apply, ctx.r3.u32, ctx.r4.u32,
                                                 ctx.r5.u64, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

void CarMakersShowBusyMarker(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies_game)) {
    __imp__sub_8219ED10(ctx, base);
    return;
  }
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::ShowBusyMarker(base, calls, ctx.r1.u32);
}

void CarMakersHideBusyMarker(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_ui_movies_game)) {
    __imp__sub_8219EDB0(ctx, base);
    return;
  }
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  ctx.r3.u64 = cm::HideBusyMarker(base, calls, ctx.r1.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_CAR_MAKERS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_car_makers)) {   \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_CAR_MAKERS(8215AAD0, CarMakersBlockOf)
ITC_CAR_MAKERS(8215B878, CarMakersFromCar)
ITC_CAR_MAKERS(82110448, CarMakersGarageCarReset)
ITC_CAR_MAKERS(825D9EF0, CarMakersStaticInit)
ITC_CAR_MAKERS(821DF2F8, CarMakersSetUpCarData)
ITC_CAR_MAKERS(82178C40, CarMakersFitCar)
ITC_CAR_MAKERS(8217FC60, CarMakersCopyRecord)
ITC_CAR_MAKERS(82178DF8, CarMakersReapplyRecord)
ITC_CAR_MAKERS(82178E60, CarMakersReapplyRecordNoDecals)
ITC_CAR_MAKERS(8217FD58, CarMakersSetCar)
ITC_CAR_MAKERS(8217FDC8, CarMakersSetCarClass)
ITC_CAR_MAKERS(8219ED10, CarMakersShowBusyMarker)
ITC_CAR_MAKERS(8219EDB0, CarMakersHideBusyMarker)
// sub_8215AB04 (BlockOf's case for id 1, the jump table's base) needs no
// adapter: only the original BlockOf reaches it. SceneModelRecord::SetPaint
// sub_8215AC70, SetWheels sub_8215B238, SetChoice sub_8215B1A0 and
// CarRecord::SetBytes sub_8215B0E0 (with sub_820D0900) have none: only
// SceneModelRecord::FromCar calls them, and its plain C++ calls theirs (their
// register steps above are the harness's). CarData::FromGarageCar
// sub_82179A60's adapter is removed: its callers in play,
// TuningScreen::SetUpCarData and CarList::DrawEntryPowerGraph, are native
// and call its plain C++ (its register step above is the harness's).
