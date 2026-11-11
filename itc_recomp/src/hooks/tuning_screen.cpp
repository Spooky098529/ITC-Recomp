// The tuning screen natively: adapters in front of src/tuning_screen/
// (docs/functions/tuning-screen.md), with the register steps the harness
// shares (hooks/tuning_screen.h). The screen's update and refresh, and other
// front-end screens, are still recompiled and call these, so the adapters
// stay.

#include "hooks/tuning_screen.h"

#include "base/flags.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "hooks/car_makers.h"
#include "hooks/front_end_car_draw.h"
#include "hooks/load_poll.h"
#include "hooks/scenery_lists.h"
#include "hooks/stand_models.h"
#include "load_poll/load_poll.h"
#include "tuning_screen/tuning_screen.h"

#include <optional>

ITC_FLAG_DECLARE(bool, native_stand_models);
ITC_FLAG_DECLARE(bool, native_race_setup);
ITC_FLAG_DECLARE(bool, native_action_mappers);
ITC_FLAG_DECLARE(bool, native_car_specs);
ITC_FLAG_DECLARE(bool, native_vehicle);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);
ITC_FLAG_DECLARE(bool, native_scene_models);
ITC_FLAG_DECLARE(bool, native_load_poll);

ITC_FLAG_BOOL(native_tuning_screen, true, "ITC",
              "Run the tuning screen's functions (docs/functions/tuning-screen.md: the part "
              "tables' TuningScreen::CategoryItemCount sub_821CC6C0, IsItemAvailable "
              "sub_821CDB18, WheelNumber sub_821CDED8, GetPrice sub_821CD160, GetCurrentChoice "
              "sub_821CD868 and GetChoiceFlag sub_821CDC38; the debug state handlers, whose prints "
              "through the empty sub_82106540 are left out: DebugMainMenu sub_821CBE08, "
              "DebugLevelSelect sub_821CBEE8, DebugTypeSelect sub_821CBFC0, DebugOptionSelect "
              "sub_821CC248, DebugConfirmBuy sub_821CC3A8, DebugConfirmEquip sub_821CC488, "
              "DebugCategoryMenu sub_821CFCE8, DebugWheelPartSelect sub_821E8068, "
              "DebugWheelKindSelect sub_821E8128, DebugWheelInchSelect sub_821E8298, "
              "DebugDecalTopMenu sub_821FD710, DebugLayerSelect sub_821FD880, DebugLayerEdit "
              "sub_821FDCB8, DebugLayerSet sub_821FE080, DebugEditPosition sub_821FE100, "
              "DebugEditView sub_821FE178, DebugPaintConfirm sub_821FE190; the selected garage car's "
              "StoreToGarageCar sub_821DCC28, EquipChoice sub_821DF448 and AeroPartValue "
              "sub_821DE2D8; the part list's marks HidePartMarks sub_821EE6D0 and ShowPartMarks "
              "sub_821EE760, ShowInteriorPicture sub_821DA3F8, HideTireMarks sub_821DFB20, HideNode19 "
              "sub_821DFBE8 and ShowTireMarks sub_821E22E8, movie 2's nodes through the UI movies' plain C++; "
              "UpdateListCounter sub_821EED00, SetPartName sub_821F2068, PlayGroupAnimation "
              "sub_821EE880 (its piece sub_821EE8E8 merged), Command31 sub_82202880 and Command9 "
              "sub_821E9050, ShowDialog sub_821D6230, Command30 sub_82202750, Command7 sub_821E8B68, "
              "Command23 sub_82202368, Command18 sub_82200600, Command25 sub_82204888, "
              "PlayIntro sub_821D06B0, OpenItems sub_821D0A48, Command15 sub_821FF1D0 and "
              "Command22 sub_82202090, MoveCategory sub_821D07D8 (the player's action mapper's "
              "slot 10 by ActionMapper::Pressed's plain C++ with --native_action_mappers, else "
              "through the vtable), MarkPick sub_821D6420, Command16 sub_821FF6F0, ShowOptions "
              "sub_821D5028, ShowLevels sub_821D12C0, UpdateItems sub_822036E8 (the mapper's slot 8 by "
              "ActionMapper::Held's plain C++ with --native_action_mappers; HighlightItem and "
              "HighlightSelected through the guest), ShowTypes sub_821D2A80 (its piece sub_821D2DFC "
              "merged; the aero parts by their plain C++ with --native_car_specs), SetStateTexts "
              "sub_821F1DE0, FillPartRows sub_821F4BF8 (its piece sub_821F4CF0 merged), Command8 sub_821F5F70, RefreshMenu sub_821D8978 and RefreshMovie13 sub_822076E8 (UiMovie::SetNumberText and SetChildNameText through their overrides), UiMovie::Play, Loop, Finish, FinishNamed, PlayNamed, "
              "LoopNamed, SetText, SetNodeText, ScrollText and sub_8211C5F8 "
              "through their overrides; the lists' "
              "UpdateListDots sub_821EE9B0, ShowOptionMarks sub_82202F30, PlaceStateNodes sub_82202C08, "
              "LayOutItems sub_82203110 and ShowItems sub_82203B00, movies 2's and 13's nodes; the calls onto the "
              "stand, with the stand's, the car makers' and the front end's car draw's flags "
              "(else the originals): SetInteriorPair sub_821CCC50, GarageStage::FitSelectedCar "
              "sub_8217DAC0, PickCameraPreset sub_821D99A0 (its piece sub_821D9B60 merged), "
              "UpdateBusyIcon sub_821D6100, PreviewWheels sub_821E0E00, ApplyDecals "
              "sub_822042D8, DecalCommand sub_822019D0, PreviewItem sub_821E07B0 and RestoreStand "
              "sub_821E0FB0 (their callees never seen in play through the guest), UpdateStand "
              "sub_821D9F10 (with GarageStage::Draw's plain C++ under its flag); SetCarStats sub_821DF788 with the "
              "car makers' and --native_vehicle; Refresh sub_821E3AD0 with both, its states 6 to 8 not ported "
              "yet and run by the original) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=tuning_screen and "
              "car_makers)");

namespace hooks {

namespace ts = tuning_screen;

void TuningCategoryItemCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::CategoryItemCount(base, ctx.r3.u32);
}

void TuningIsItemAvailable(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::IsItemAvailable(base, ctx.r3.u32, ctx.r4.u64);
}

void TuningWheelNumber(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::WheelNumber(ctx.r4.u64, ctx.r5.u64);
}

void TuningGetPrice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetPrice(base, ctx.r3.u32);
}

void TuningGetCurrentChoice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetCurrentChoice(base, ctx.r3.u32);
}

void TuningGetChoiceFlag(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetChoiceFlag(base, ctx.r3.u32, ctx.r4.u64);
}

void TuningDebugMainMenu(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugMainMenu(base, ctx.r1.u32);
}

void TuningDebugLevelSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLevelSelect(base, ctx.r1.u32);
}

void TuningDebugTypeSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugTypeSelect(base, ctx.r1.u32);
}

void TuningDebugOptionSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugOptionSelect(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugConfirmBuy(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugConfirmBuy(base, ctx.r1.u32);
}

void TuningDebugConfirmEquip(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugConfirmEquip(base, ctx.r1.u32);
}

void TuningDebugCategoryMenu(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugCategoryMenu(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugWheelPartSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWheelPartSelect(base, ctx.r1.u32);
}

void TuningDebugWheelKindSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWheelKindSelect(base, ctx.r1.u32);
}

void TuningDebugWheelInchSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWheelInchSelect(base, ctx.r1.u32);
}

void TuningDebugDecalTopMenu(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugDecalTopMenu(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugLayerSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerSelect(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugLayerEdit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerEdit(base, ctx.r1.u32);
}

void TuningDebugLayerSet(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerSet(base, ctx.r1.u32);
}

void TuningDebugEditPosition(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditPosition();
}

void TuningDebugEditView(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditView();
}

void TuningDebugPaintConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugPaintConfirm(base, ctx.r1.u32);
}

void TuningStoreToGarageCar(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ts::StoreToGarageCar(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningEquipChoice(PPCContext& ctx, uint8_t* base) { ts::EquipChoice(base, ctx.r3.u32); }

void TuningAeroPartValue(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::AeroPartValue(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32);
}

void TuningHidePartMarks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::HidePartMarks(base, ctx.r1.u32);
}

void TuningShowPartMarks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 =
      ts::ShowPartMarks(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, ctx.r6.u64, ctx.r1.u32);
}

void TuningShowInteriorPicture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ShowInteriorPicture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32);
}

void TuningHideTireMarks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::HideTireMarks(base, ctx.r1.u32);
}

void TuningHideNode19(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::HideNode19(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningShowTireMarks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ShowTireMarks(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningUpdateListDots(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::UpdateListDots(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningShowOptionMarks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ShowOptionMarks(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void TuningPlaceStateNodes(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ts::PlaceStateNodes(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningLayOutItems(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ts::LayOutItems(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningShowItems(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ShowItems(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

namespace {

// A step with the guest calls (hooks::StandModelsGuestCall), lr put back.
template <typename Body>
void WithCalls(PPCContext& ctx, uint8_t* base, Body body) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const ts::GuestCall call = StandModelsGuestCall(ctx, base);
  const uint64_t r3 = body(call);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

}  // namespace

#define ITC_TUNING_CALLS_STEP(Name)                                                    \
  void Tuning##Name(PPCContext& ctx, uint8_t* base) {                                 \
    const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;                              \
    WithCalls(ctx, base,                                                              \
              [&](const ts::GuestCall& call) { return ts::Name(base, call, screen, sp); }); \
  }

ITC_TUNING_CALLS_STEP(UpdateListCounter)
ITC_TUNING_CALLS_STEP(SetPartName)
ITC_TUNING_CALLS_STEP(PlayGroupAnimation)
ITC_TUNING_CALLS_STEP(Command31)
ITC_TUNING_CALLS_STEP(Command9)
ITC_TUNING_CALLS_STEP(Command7)
ITC_TUNING_CALLS_STEP(Command23)
ITC_TUNING_CALLS_STEP(Command18)
ITC_TUNING_CALLS_STEP(Command25)
ITC_TUNING_CALLS_STEP(PlayIntro)
ITC_TUNING_CALLS_STEP(OpenItems)
ITC_TUNING_CALLS_STEP(Command15)
ITC_TUNING_CALLS_STEP(Command22)
ITC_TUNING_CALLS_STEP(MarkPick)
ITC_TUNING_CALLS_STEP(Command16)
ITC_TUNING_CALLS_STEP(ShowOptions)
ITC_TUNING_CALLS_STEP(ShowLevels)

#undef ITC_TUNING_CALLS_STEP

void TuningMoveCategory(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const ts::GuestCall& call) {
    return ts::MoveCategory(base, call, screen, sp, native_slots);
  });
}

void TuningUpdateItems(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_slots = ITC_FLAG(native_action_mappers);
  WithCalls(ctx, base, [&](const ts::GuestCall& call) {
    return ts::UpdateItems(base, call, r3, sp, native_slots);
  });
  ctx.fpscr.disableFlushMode();
}

void TuningShowTypes(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const bool native_specs = ITC_FLAG(native_car_specs);
  WithCalls(ctx, base, [&](const ts::GuestCall& call) {
    return ts::ShowTypes(base, call, screen, sp, native_specs);
  });
  ctx.fpscr.disableFlushMode();
}

void TuningSetStateTexts(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  std::optional<double> f1;
  WithCalls(ctx, base, [&](const ts::GuestCall& call) {
    return ts::SetStateTexts(base, call, screen, sp, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
  ctx.fpscr.disableFlushMode();
}

void TuningFillPartRows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::FillPartRows(base, ctx.r3.u64, ctx.r1.u32);
}

void TuningCommand8(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithCalls(ctx, base, [&](const ts::GuestCall& call) { return ts::Command8(base, call, r3, sp); });
}

void TuningRefreshMenu(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  std::optional<double> f1;
  WithCalls(ctx, base, [&](const ts::GuestCall& inner) {
    const ts::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                   uint32_t frame, uint32_t lr) {
      if (f1) {
        ctx.f1.f64 = *f1;
        f1.reset();
      }
      return inner(function, args, frame, lr);
    };
    return ts::RefreshMenu(base, call, screen, sp, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
  ctx.fpscr.disableFlushMode();
}

void TuningRefreshMovie13(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  std::optional<double> f1;
  WithCalls(ctx, base, [&](const ts::GuestCall& inner) {
    const ts::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                   uint32_t frame, uint32_t lr) {
      if (f1) {
        ctx.f1.f64 = *f1;
        f1.reset();
      }
      return inner(function, args, frame, lr);
    };
    return ts::RefreshMovie13(base, call, screen, sp, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
  ctx.fpscr.disableFlushMode();
}

// The same with r3 all 64 bits and r4 the update's dispatcher.
#define ITC_TUNING_DISPATCH_STEP(Name)                                                \
  void Tuning##Name(PPCContext& ctx, uint8_t* base) {                                \
    const uint64_t r3 = ctx.r3.u64;                                                  \
    const uint32_t dispatcher = ctx.r4.u32, sp = ctx.r1.u32;                         \
    WithCalls(ctx, base, [&](const ts::GuestCall& call) {                            \
      return ts::Name(base, call, r3, dispatcher, sp);                               \
    });                                                                              \
  }

ITC_TUNING_DISPATCH_STEP(ShowDialog)
ITC_TUNING_DISPATCH_STEP(Command30)

#undef ITC_TUNING_DISPATCH_STEP

bool TuningStandCalleesNative() {
  return ITC_FLAG(native_stand_models) && ITC_FLAG(native_race_setup) &&
         CarMakersSetCarCalleesNative();
}

namespace {

// A step onto the stand: the guest calls and the car makers' (with the load
// poll's), lr put back, flush-to-zero off.
template <typename Body>
void WithStand(PPCContext& ctx, uint8_t* base, Body body) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  const ts::GuestCall call = StandModelsGuestCall(ctx, base);
  const ts::StandCalls stand{call, calls};
  const uint64_t r3 = body(stand);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

}  // namespace

void TuningSetInteriorPair(PPCContext& ctx, uint8_t* base) {
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64;
  const uint32_t sp = ctx.r1.u32;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::SetInteriorPair(base, stand.call, r4, r5, sp);
  });
}

void TuningFitSelectedCar(PPCContext& ctx, uint8_t* base) {
  const uint32_t holder = ctx.r3.u32, sp = ctx.r1.u32;
  const uint64_t r4 = ctx.r4.u64, r5 = ctx.r5.u64;
  double f1 = 0.0;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::FitSelectedCar(base, stand.calls, holder, r4, r5, sp, &f1);
  });
  ctx.f1.f64 = f1;
}

#define ITC_TUNING_STAND_STEP(Name)                                             \
  void Tuning##Name(PPCContext& ctx, uint8_t* base) {                          \
    const uint64_t r3 = ctx.r3.u64;                                            \
    const uint32_t sp = ctx.r1.u32;                                            \
    WithStand(ctx, base,                                                       \
              [&](const ts::StandCalls& stand) { return ts::Name(base, stand, r3, sp); }); \
  }

ITC_TUNING_STAND_STEP(PickCameraPreset)
ITC_TUNING_STAND_STEP(UpdateBusyIcon)
ITC_TUNING_STAND_STEP(ApplyDecals)
ITC_TUNING_STAND_STEP(PreviewItem)

#undef ITC_TUNING_STAND_STEP

void TuningDecalCommand(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  double f1 = 0.0;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::DecalCommand(base, stand, screen, sp, &f1);
  });
  ctx.f1.f64 = f1;
}

void TuningRestoreStand(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  std::optional<double> f1;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::RestoreStand(base, stand, r3, sp, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
}

// UpdateStand: its callees' plain C++ where their adapters would run it
// (GarageStage::Draw's and Command's, the stand's setters', ApplyDecals'),
// else the original.
bool TuningUpdateStandCalleesNative() {
  return TuningStandCalleesNative() && FrontEndCarDrawDrawNative() &&
         ITC_FLAG(native_vehicle_accessors) && ITC_FLAG(native_scene_models);
}

void TuningUpdateStand(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  SceneryDrawGuest guest{ctx, base};
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  calls.draw = SceneryListsDrawCalls(&guest);
  const ts::GuestCall call = StandModelsGuestCall(ctx, base);
  const ts::StandCalls stand{call, calls};
  const front_end_car_draw::DrawCallees draw = FrontEndCarDrawDrawCallees(ctx);
  // SceneModel::ApplyWheelPaint's plain C++ under StepSecondWheels with the
  // load poll's flags (as its adapter).
  load_poll::Calls* paint_poll =
      ITC_FLAG(native_load_poll) && LoadPollCalleesNative() ? &poll : nullptr;
  const uint64_t r3 = ts::UpdateStand(base, stand, draw, paint_poll, ctx.r3.u32, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

bool TuningCarStatsCalleesNative() {
  return CarMakersGarageCalleesNative() && ITC_FLAG(native_vehicle);
}

void TuningSetCarStats(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  double f1 = 0.0;
  const uint64_t r3 = ts::SetCarStats(base, calls, ctx.r3.u32, ctx.r1.u32, &f1);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.f1.f64 = f1;
  ctx.fpscr.disableFlushMode();
}

void TuningPreviewWheels(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  std::optional<double> f1;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::PreviewWheels(base, stand.call, r3, sp, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
}

void TuningRefresh(PPCContext& ctx, uint8_t* base) {
  // The gate for the refresh's split migration (temporary): states 6 to 8
  // aren't ported yet and run the original; parts 3 and 4 remove it state by
  // state (6 TYPE_SELECT, 7 and 8 the choice and option pages).
  const int32_t state = int32_t(guest::LoadU32(base, ctx.r3.u32 + ts::kState));
  if (state >= 6 && state <= 8) {
    __imp__sub_821E3AD0(ctx, base);
    return;
  }
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  std::optional<double> f1;
  WithStand(ctx, base, [&](const ts::StandCalls& inner) {
    // RefreshMenu's f1 wrapper: the f1 the native code left put in f1
    // before the next guest call.
    const ts::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                   uint32_t frame, uint32_t lr) {
      if (f1) {
        ctx.f1.f64 = *f1;
        f1.reset();
      }
      return inner.call(function, args, frame, lr);
    };
    const ts::StandCalls stand{call, inner.calls};
    return ts::Refresh(base, stand, r3, sp, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
  ctx.fpscr.disableFlushMode();
}

}  // namespace hooks

// Defines an adapter: the flag, else the original. HideTireMarks sub_821DFB20
// and PlaceStateNodes sub_82202C08 have none: only native callers
// (ShowTireMarks, Command25) reach them, and the flag switches those; their
// register steps are the harness's.
#define ITC_TUNING_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_tuning_screen)) { \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

ITC_TUNING_SCREEN(821CDB18, TuningIsItemAvailable)
ITC_TUNING_SCREEN(821CD160, TuningGetPrice)
ITC_TUNING_SCREEN(821CD868, TuningGetCurrentChoice)
ITC_TUNING_SCREEN(821CDC38, TuningGetChoiceFlag)
ITC_TUNING_SCREEN(821CBE08, TuningDebugMainMenu)
ITC_TUNING_SCREEN(821CBEE8, TuningDebugLevelSelect)
ITC_TUNING_SCREEN(821CBFC0, TuningDebugTypeSelect)
ITC_TUNING_SCREEN(821CC248, TuningDebugOptionSelect)
ITC_TUNING_SCREEN(821CC3A8, TuningDebugConfirmBuy)
ITC_TUNING_SCREEN(821CC488, TuningDebugConfirmEquip)
ITC_TUNING_SCREEN(821CFCE8, TuningDebugCategoryMenu)
ITC_TUNING_SCREEN(821E8068, TuningDebugWheelPartSelect)
ITC_TUNING_SCREEN(821E8128, TuningDebugWheelKindSelect)
ITC_TUNING_SCREEN(821E8298, TuningDebugWheelInchSelect)
ITC_TUNING_SCREEN(821FD710, TuningDebugDecalTopMenu)
ITC_TUNING_SCREEN(821FD880, TuningDebugLayerSelect)
ITC_TUNING_SCREEN(821FDCB8, TuningDebugLayerEdit)
ITC_TUNING_SCREEN(821FE080, TuningDebugLayerSet)
ITC_TUNING_SCREEN(821FE100, TuningDebugEditPosition)
ITC_TUNING_SCREEN(821FE178, TuningDebugEditView)
ITC_TUNING_SCREEN(821FE190, TuningDebugPaintConfirm)
ITC_TUNING_SCREEN(821DCC28, TuningStoreToGarageCar)
ITC_TUNING_SCREEN(821DF448, TuningEquipChoice)
ITC_TUNING_SCREEN(821DE2D8, TuningAeroPartValue)
ITC_TUNING_SCREEN(821DA3F8, TuningShowInteriorPicture)
ITC_TUNING_SCREEN(821DFBE8, TuningHideNode19)
ITC_TUNING_SCREEN(821E22E8, TuningShowTireMarks)
ITC_TUNING_SCREEN(821EED00, TuningUpdateListCounter)
ITC_TUNING_SCREEN(82202880, TuningCommand31)
ITC_TUNING_SCREEN(821E9050, TuningCommand9)
ITC_TUNING_SCREEN(821EE9B0, TuningUpdateListDots)
ITC_TUNING_SCREEN(821D6230, TuningShowDialog)
ITC_TUNING_SCREEN(82202750, TuningCommand30)
ITC_TUNING_SCREEN(821E8B68, TuningCommand7)
ITC_TUNING_SCREEN(82202368, TuningCommand23)
ITC_TUNING_SCREEN(82200600, TuningCommand18)
ITC_TUNING_SCREEN(82204888, TuningCommand25)
ITC_TUNING_SCREEN(821D06B0, TuningPlayIntro)
ITC_TUNING_SCREEN(821D0A48, TuningOpenItems)
ITC_TUNING_SCREEN(821FF1D0, TuningCommand15)
ITC_TUNING_SCREEN(82202090, TuningCommand22)
ITC_TUNING_SCREEN(821D07D8, TuningMoveCategory)
ITC_TUNING_SCREEN(821D6420, TuningMarkPick)
ITC_TUNING_SCREEN(821FF6F0, TuningCommand16)
ITC_TUNING_SCREEN(821D5028, TuningShowOptions)
ITC_TUNING_SCREEN(821D12C0, TuningShowLevels)
ITC_TUNING_SCREEN(822036E8, TuningUpdateItems)
ITC_TUNING_SCREEN(821D2A80, TuningShowTypes)
ITC_TUNING_SCREEN(821F1DE0, TuningSetStateTexts)
ITC_TUNING_SCREEN(821F5F70, TuningCommand8)
ITC_TUNING_SCREEN(821D8978, TuningRefreshMenu)
ITC_TUNING_SCREEN(822076E8, TuningRefreshMovie13)

// The calls onto the stand: also the original unless their callees are
// native.
#define ITC_TUNING_STAND(address, step)                                      \
  extern "C" REX_FUNC(sub_##address) {                                       \
    REX_FUNC_PROLOGUE();                                                     \
    if (!ITC_FLAG(native_tuning_screen) || !hooks::TuningStandCalleesNative()) { \
      __imp__sub_##address(ctx, base);                                       \
      return;                                                                \
    }                                                                        \
    hooks::step(ctx, base);                                                  \
  }

ITC_TUNING_STAND(8217DAC0, TuningFitSelectedCar)
ITC_TUNING_STAND(821D99A0, TuningPickCameraPreset)
ITC_TUNING_STAND(821D6100, TuningUpdateBusyIcon)
ITC_TUNING_STAND(822019D0, TuningDecalCommand)

ITC_TUNING_STAND(821E07B0, TuningPreviewItem)
ITC_TUNING_STAND(821E0FB0, TuningRestoreStand)

// No adapters (removed, B7 loop, 2026-11-07, the fifth migration: their
// callers in play are native and call their plain C++; the register steps
// above are the harness's): ShowOptionMarks sub_82202F30 and ShowItems
// sub_82203B00 (RefreshMovie13), LayOutItems sub_82203110 (UpdateItems and
// RefreshMovie13), ApplyDecals sub_822042D8 (UpdateStand).

// No adapters (removed, B7 loop, 2026-11-07, a cleanup lead: their callers
// in play are native and call their plain C++; the register steps above are
// the harness's): FillPartRows sub_821F4BF8, SetPartName sub_821F2068 and
// PlayGroupAnimation sub_821EE880 (tuning_pages' sub_821F5F70 and
// sub_821F60E0), PreviewWheels sub_821E0E00 (tuning_pages' sub_821EE390,
// sub_821EEEA8 and sub_821F6688), HidePartMarks sub_821EE6D0 and
// ShowPartMarks sub_821EE760 (FillPartRows and tuning_pages' sub_821F40B8);
// then CategoryItemCount sub_821CC6C0 (DebugCategoryMenu and tuning_pages'
// InputCategories) and WheelNumber sub_821CDED8 (PreviewWheels, FillPartRows,
// ShowPartMarks, SetPartName and tuning_pages' SetCurrentChoice and
// FillPartRowsRight).

// SetInteriorPair sub_821CCC50 has no adapter (removed, B7 loop, 2026-11-07):
// its callers in play, PreviewItem and RestoreStand, are native and call its
// plain C++ (its register step above is the harness's).

// UpdateStand: the original unless its callees are native.
extern "C" REX_FUNC(sub_821D9F10) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_tuning_screen) || !hooks::TuningUpdateStandCalleesNative()) {
    __imp__sub_821D9F10(ctx, base);
    return;
  }
  hooks::TuningUpdateStand(ctx, base);
}

// SetCarStats: the original unless the car data's and the power curve's
// callees are native.
extern "C" REX_FUNC(sub_821DF788) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_tuning_screen) || !hooks::TuningCarStatsCalleesNative()) {
    __imp__sub_821DF788(ctx, base);
    return;
  }
  hooks::TuningSetCarStats(ctx, base);
}

// Refresh: the original unless its callees onto the stand and SetCarStats'
// are native (states 6 to 8 still the original: the register step's gate).
extern "C" REX_FUNC(sub_821E3AD0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_tuning_screen) || !hooks::TuningStandCalleesNative() ||
      !hooks::TuningCarStatsCalleesNative()) {
    __imp__sub_821E3AD0(ctx, base);
    return;
  }
  hooks::TuningRefresh(ctx, base);
}
