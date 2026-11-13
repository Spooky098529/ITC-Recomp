// The tuning screen natively: adapters in front of src/tuning_screen/
// (docs/functions/tuning-screen.md), with the register steps the harness
// shares (hooks/tuning_screen.h). The adapters left are those recompiled
// code still reaches (the update itself through the tuning machine's
// functor, FitSelectedCar); the others went once their callers were native.

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
ITC_FLAG_DECLARE(bool, native_tuning_pages);

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
              "DebugEditView sub_821FE178, DebugPaintConfirm sub_821FE190, and those never seen in play, "
              "called only by the update's plain C++: DebugColorSelect sub_821CC078, DebugWarnMoney "
              "sub_821CC430, DebugReflectSet sub_821CC510, DebugAnnulSet sub_821CC598, "
              "DebugRemoveConfirm sub_821CC620, DebugBodyColor sub_821E8358, DebugNeonPart "
              "sub_821E8440, DebugNeonKind sub_821E84F8, DebugNeonEdit sub_821E85A0, DebugLedEdit "
              "sub_821E8688, DebugCockpitTop sub_821E8758, DebugCockpitType sub_821E8800, "
              "DebugCockpitColor sub_821E88C0, DebugMeterKind sub_821E8998, DebugMeterPoint "
              "sub_821E8A88, DebugLayerOperate sub_821FDA80, DebugLayerOrder sub_821FDF10, "
              "DebugEditRotate sub_821FE118, DebugEditScale sub_821FE130, DebugEditColor "
              "sub_821FE148, DebugEditFlip sub_821FE160, DebugPaintCancelConfirm sub_821FE230, "
              "DebugPaintRemoveConfirm sub_821FE2D0; the selected garage car's "
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
              "sub_821E0FB0 (their callees never seen in play through the guest, but PreviewParts "
              "sub_821CCAA8 and GarageStage::SetColor by their plain C++, no adapters), UpdateStand "
              "sub_821D9F10 (with GarageStage::Draw's plain C++ under its flag; GarageStage::SetColor "
              "sub_8217C6C8, ApplyAreaDecals sub_821F7B90 and ApplyDecalWord sub_821FE4E0, never "
              "seen in play, by their plain C++, no adapters); SetCarStats sub_821DF788 with the "
              "car makers' and --native_vehicle; Refresh sub_821E3AD0 with both and --native_car_specs, every state (RefreshInterior "
              "sub_821F2AD8 with IsSlotAvailable sub_821CE188 by their plain C++, no adapters); "
              "Update sub_821E6C50 (its pieces sub_821E6CD4 and sub_821E6F90 merged) with the refresh's and "
              "UpdateStand's checks, its callees never seen in play and DrawPowerGraph through the guest "
              "but movie 2's command runners never seen in play, called only by the update's plain C++: "
              "Command5 sub_821D4180, Command11 sub_821E9730, Command12 sub_821E9CA0, Command54 "
              "sub_821EB8E8, Command55 sub_821EBDC8, Command56 sub_821EC6A0, Command57 sub_821ECC68 "
              "(UiMovie::NamedAnimationTime by its plain C++), Command58 sub_821EDB70 and the paint and "
              "light kit pages' Command10 sub_821F35C0, Command13 sub_821EA460 and Command14 "
              "sub_821EAFA0 (SetPaintGradients sub_821E9488, SetLightKind0Bars sub_821EA130 and "
              "SetLightKind1Bar sub_821EAE68 by their plain C++, no adapters), and the "
              "editors': Command17 sub_82200100, Command19 sub_82201C58, Command20 sub_82201FA0, "
              "Command21 sub_822087C8 (SwapLayerRecords and SwapOrderTextures by their plain C++ "
              "with --native_tuning_pages), "
              "Command26 sub_82204980, Command27 sub_82204A78, Command32Done sub_821F7498, Command42 "
              "sub_821F7618, Command52 sub_821F77F0 (movie 16's dialog) and Command53 sub_821F7948, "
              "and with --native_tuning_pages the sticker commands Command32 to Command41 "
              "(sub_821FC7B8, sub_821FCA80, sub_821FA658, sub_821FA9C0, sub_821FAA28, sub_821F8C30, "
              "sub_821FAD00, sub_821FB0B8, sub_821FCC48, sub_821FCED0) and Command43 to Command51 "
              "(sub_821FD128, sub_821FD3F8, sub_821FB1F0, sub_821F8FC0 for 46, 48 and 50, sub_821FB450, "
              "sub_821FB6B0, sub_821FB8D8; src/tuning_pages/sticker_commands.cpp); "
              "its swatch color setter UiNode::SetRgb sub_821E7DA0; the stand setters never seen in play, "
              "SetCaliper sub_821DA330, SetAccessory sub_821CCBE8, RefitAxles sub_821DF9D0, "
              "PreviewInteriorChoices sub_821E7E48 and PreviewLightKit sub_821CC930, and the refresh's pages "
              "RefreshPaint sub_821F2110, RefreshLightKits sub_821F2378 and RefreshMovie14 sub_821F7A98, "
              "no adapters) natively instead of through the "
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

void TuningDebugColorSelect(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugColorSelect(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugWarnMoney(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugWarnMoney(base, ctx.r1.u32);
}

void TuningDebugReflectSet(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugReflectSet(base, ctx.r1.u32);
}

void TuningDebugAnnulSet(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugAnnulSet(base, ctx.r1.u32);
}

void TuningDebugRemoveConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugRemoveConfirm(base, ctx.r1.u32);
}

void TuningDebugBodyColor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugBodyColor(base, ctx.r1.u32);
}

void TuningDebugNeonPart(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugNeonPart(base, ctx.r1.u32);
}

void TuningDebugNeonKind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugNeonKind(base, ctx.r1.u32);
}

void TuningDebugNeonEdit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugNeonEdit(base, ctx.r1.u32);
}

void TuningDebugLedEdit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLedEdit(base, ctx.r1.u32);
}

void TuningDebugCockpitTop(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugCockpitTop(base, ctx.r1.u32);
}

void TuningDebugCockpitType(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugCockpitType(base, ctx.r1.u32);
}

void TuningDebugCockpitColor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugCockpitColor(base, ctx.r1.u32);
}

void TuningDebugMeterKind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugMeterKind(base, ctx.r1.u32);
}

void TuningDebugMeterPoint(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugMeterPoint(base, ctx.r1.u32);
}

void TuningDebugLayerOperate(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerOperate(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugLayerOrder(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugLayerOrder(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningDebugEditRotate(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditRotate();
}

void TuningDebugEditScale(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditScale();
}

void TuningDebugEditColor(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditColor();
}

void TuningDebugEditFlip(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugEditFlip();
}

void TuningDebugPaintCancelConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugPaintCancelConfirm(base, ctx.r1.u32);
}

void TuningDebugPaintRemoveConfirm(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::DebugPaintRemoveConfirm(base, ctx.r1.u32);
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
ITC_TUNING_CALLS_STEP(Command5)
ITC_TUNING_CALLS_STEP(Command11)
ITC_TUNING_CALLS_STEP(Command12)
ITC_TUNING_CALLS_STEP(Command54)
ITC_TUNING_CALLS_STEP(Command55)
ITC_TUNING_CALLS_STEP(Command56)
ITC_TUNING_CALLS_STEP(Command58)
ITC_TUNING_CALLS_STEP(Command10)
ITC_TUNING_CALLS_STEP(Command13)
ITC_TUNING_CALLS_STEP(Command14)
ITC_TUNING_CALLS_STEP(Command17)
ITC_TUNING_CALLS_STEP(Command19)
ITC_TUNING_CALLS_STEP(Command26)
ITC_TUNING_CALLS_STEP(Command27)
ITC_TUNING_CALLS_STEP(Command42)

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

void TuningCommand20(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  const uint64_t a = ctx.r4.u64, b = ctx.r5.u64;
  WithCalls(ctx, base, [&](const ts::GuestCall& call) {
    return ts::Command20(base, call, screen, sp, a, b);
  });
}

void TuningCommand21(PPCContext& ctx, uint8_t* base) {
  const uint64_t screen = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  const bool native_pages = ITC_FLAG(native_tuning_pages);
  WithCalls(ctx, base, [&](const ts::GuestCall& call) {
    return ts::Command21(base, call, screen, sp, native_pages);
  });
}

void TuningCommand32Done(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::Command32Done(base, ctx.r3.u32, ctx.r1.u32);
}

void TuningCommand53(PPCContext& ctx, uint8_t* base) { ts::Command53(base, ctx.r3.u32); }

void TuningCommand57(PPCContext& ctx, uint8_t* base) {
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
    return ts::Command57(base, call, screen, sp, &f1);
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
ITC_TUNING_DISPATCH_STEP(Command52)

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

// WithStand with f1 tracked RefreshMenu's way: `body(stand, &f1)`, the f1
// the native code left put in f1 before each later guest call and at the
// end.
template <typename Body>
void WithStandF1(PPCContext& ctx, uint8_t* base, Body body) {
  std::optional<double> f1;
  WithStand(ctx, base, [&](const ts::StandCalls& inner) {
    const ts::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                   uint32_t frame, uint32_t lr) {
      if (f1) {
        ctx.f1.f64 = *f1;
        f1.reset();
      }
      return inner.call(function, args, frame, lr);
    };
    const ts::StandCalls stand{call, inner.calls};
    return body(stand, &f1);
  });
  if (f1) {
    ctx.f1.f64 = *f1;
  }
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
ITC_TUNING_STAND_STEP(ApplyAreaDecals)

#undef ITC_TUNING_STAND_STEP

void TuningApplyDecalWord(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithStand(ctx, base,
            [&](const ts::StandCalls& stand) { return ts::ApplyDecalWord(base, stand.call, r3, sp); });
}

void TuningPreviewParts(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64;
  const uint32_t sp = ctx.r1.u32;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::PreviewParts(base, stand.call, r3, r4, sp);
  });
}

void TuningDecalCommand(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  double f1 = 0.0;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::DecalCommand(base, stand, screen, sp, &f1);
  });
  ctx.f1.f64 = f1;
}

void TuningPreviewItem(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithStandF1(ctx, base, [&](const ts::StandCalls& stand, std::optional<double>* f1) {
    return ts::PreviewItem(base, stand, r3, sp, f1);
  });
}

void TuningRestoreStand(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithStandF1(ctx, base, [&](const ts::StandCalls& stand, std::optional<double>* f1) {
    return ts::RestoreStand(base, stand, r3, sp, f1);
  });
}

void TuningSetCaliper(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, r4 = ctx.r4.u32, r5 = ctx.r5.u32, sp = ctx.r1.u32;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::SetCaliper(base, stand, screen, r4, r5, sp);
  });
}

void TuningSetAccessory(PPCContext& ctx, uint8_t* base) {
  const uint32_t r4 = ctx.r4.u32, r5 = ctx.r5.u32, sp = ctx.r1.u32;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::SetAccessory(base, stand.call, r4, r5, sp);
  });
}

void TuningRefitAxles(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  WithStandF1(ctx, base, [&](const ts::StandCalls& stand, std::optional<double>* f1) {
    return ts::RefitAxles(base, stand, screen, sp, f1);
  });
}

void TuningPreviewInteriorChoices(PPCContext& ctx, uint8_t* base) {
  const uint32_t screen = ctx.r3.u32, sp = ctx.r1.u32;
  WithStand(ctx, base, [&](const ts::StandCalls& stand) {
    return ts::PreviewInteriorChoices(base, stand.call, screen, sp);
  });
}

void TuningPreviewLightKit(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  WithStandF1(ctx, base, [&](const ts::StandCalls& stand, std::optional<double>* f1) {
    return ts::PreviewLightKit(base, stand, r3, sp, f1);
  });
}

void TuningSetRgb(PPCContext& ctx, uint8_t* base) {
  ts::SetRgb(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32);
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

bool TuningUpdateCalleesNative() {
  return TuningUpdateStandCalleesNative() && TuningCarStatsCalleesNative() &&
         ITC_FLAG(native_car_specs);
}

void TuningUpdate(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64;
  const uint32_t sp = ctx.r1.u32;
  ctx.fpscr.disableFlushMode();
  std::optional<double> f1;
  load_poll::Calls poll = LoadPollCalls(ctx, base);
  SceneryDrawGuest guest{ctx, base};
  front_end_car_draw::Calls calls = FrontEndCarDrawCalls(ctx, base);
  calls.poll = &poll;
  calls.draw = SceneryListsDrawCalls(&guest);
  const ts::GuestCall inner = StandModelsGuestCall(ctx, base);
  // RefreshMenu's f1 wrapper: the f1 the native code left put in f1 before
  // the next guest call.
  const auto apply_f1 = [&] {
    if (f1) {
      ctx.f1.f64 = *f1;
      f1.reset();
    }
  };
  const ts::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                 uint32_t frame, uint32_t back) {
    apply_f1();
    return inner(function, args, frame, back);
  };
  const ts::StandCalls stand{call, calls};
  const front_end_car_draw::DrawCallees draw = FrontEndCarDrawDrawCallees(ctx);
  load_poll::Calls* paint_poll =
      ITC_FLAG(native_load_poll) && LoadPollCalleesNative() ? &poll : nullptr;
  const ts::UpdateCalls update{
      stand,
      draw,
      paint_poll,
      ITC_FLAG(native_action_mappers),
      ITC_FLAG(native_car_specs),
      ITC_FLAG(native_tuning_pages),
      [&](uint32_t frame, uint64_t screen, uint64_t r30) {
        apply_f1();
        const uint64_t stack = ctx.r1.u64;
        const uint64_t r30_was = ctx.r30.u64, r31_was = ctx.r31.u64;
        ctx.r1.u64 = frame;
        ctx.r3.u64 = screen;
        ctx.r30.u64 = r30;
        ctx.r31.u64 = screen;
        ctx.lr = 0x821E7318;
        sub_821DF350(ctx, base);
        ctx.r1.u64 = stack;
        ctx.r30.u64 = r30_was;
        ctx.r31.u64 = r31_was;
        ctx.fpscr.disableFlushMode();
      }};
  const uint64_t answer = ts::Update(base, update, r3, r4, sp, &f1);
  apply_f1();
  ctx.lr = lr;
  ctx.r3.u64 = answer;
  ctx.fpscr.disableFlushMode();
}

void TuningWithRefreshStand(
    PPCContext& ctx, uint8_t* base, std::optional<double>* f1,
    const std::function<uint64_t(const ts::StandCalls&)>& body) {
  WithStand(ctx, base, [&](const ts::StandCalls& inner) {
    const ts::GuestCall call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                                   uint32_t frame, uint32_t lr) {
      if (*f1) {
        ctx.f1.f64 = **f1;
        f1->reset();
      }
      return inner.call(function, args, frame, lr);
    };
    const ts::StandCalls stand{call, inner.calls};
    return body(stand);
  });
  if (*f1) {
    ctx.f1.f64 = **f1;
  }
  ctx.fpscr.disableFlushMode();
}

}  // namespace hooks

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

// No adapters (removed, B7 loop, 2026-11-11, after the refresh went native:
// their callers in play are native and call their plain C++; the register
// steps above are the harness's): IsItemAvailable sub_821CDB18 (the refresh,
// PreviewItem and tuning_pages' InputLevels, InputTypes, MoveLevel and
// MoveType), GetPrice sub_821CD160 (the refresh and tuning_pages'
// InputLevels, InputTypes, InputOptions, InputInches and InputWheels),
// GetCurrentChoice sub_821CD868 (the refresh, ShowLevels, ShowTypes,
// ShowTireMarks and tuning_pages' MoveLevel, MoveType and MoveOption),
// GetChoiceFlag sub_821CDC38, EquipChoice sub_821DF448, AeroPartValue
// sub_821DE2D8, ShowInteriorPicture sub_821DA3F8, HideNode19 sub_821DFBE8,
// ShowTireMarks sub_821E22E8, SetStateTexts sub_821F1DE0, RefreshMovie13
// sub_822076E8, PickCameraPreset sub_821D99A0 and PreviewItem sub_821E07B0
// (the refresh).

// SetInteriorPair sub_821CCC50 has no adapter (removed, B7 loop, 2026-11-07):
// its callers in play, PreviewItem and RestoreStand, are native and call its
// plain C++ (its register step above is the harness's).

// No adapters (removed, B7 loop, 2026-11-11, after the update went native:
// their callers in play, the update sub_821E6C50 and its pieces, merged, and
// the other native callers named, call their plain C++; the register steps
// above are the harness's; HideTireMarks sub_821DFB20 and PlaceStateNodes
// sub_82202C08 never had one): the debug handlers DebugMainMenu
// sub_821CBE08, DebugLevelSelect sub_821CBEE8, DebugTypeSelect sub_821CBFC0,
// DebugOptionSelect sub_821CC248, DebugConfirmBuy sub_821CC3A8,
// DebugConfirmEquip sub_821CC488, DebugCategoryMenu sub_821CFCE8,
// DebugWheelPartSelect sub_821E8068, DebugWheelKindSelect sub_821E8128,
// DebugWheelInchSelect sub_821E8298, DebugDecalTopMenu sub_821FD710,
// DebugLayerSelect sub_821FD880, DebugLayerEdit sub_821FDCB8, DebugLayerSet
// sub_821FE080, DebugEditPosition sub_821FE100, DebugEditView sub_821FE178,
// DebugPaintConfirm sub_821FE190; the runners PlayIntro sub_821D06B0,
// MoveCategory sub_821D07D8 (with tuning_pages' InputCategories), OpenItems
// sub_821D0A48, ShowLevels sub_821D12C0, ShowTypes sub_821D2A80, ShowOptions
// sub_821D5028, Command7 sub_821E8B68, Command8 sub_821F5F70, Command9
// sub_821E9050, Command15 sub_821FF1D0, Command16 sub_821FF6F0, Command18
// sub_82200600, DecalCommand sub_822019D0, Command22 sub_82202090,
// Command23 sub_82202368, Command25 sub_82204888, Command30 sub_82202750,
// Command31 sub_82202880, ShowDialog sub_821D6230, MarkPick sub_821D6420;
// UpdateBusyIcon sub_821D6100, UpdateItems sub_822036E8, RefreshMenu
// sub_821D8978, UpdateListDots sub_821EE9B0, UpdateListCounter sub_821EED00,
// UpdateStand sub_821D9F10, StoreToGarageCar sub_821DCC28 (with tuning_pages'
// InputItems and Leave), SetCarStats sub_821DF788 (with the refresh),
// RestoreStand sub_821E0FB0 and the refresh sub_821E3AD0 (with
// TuningMachine::Start).

// Update: the original unless its callees' adapters would run their plain
// C++ (the refresh's, UpdateStand's and SetCarStats' checks).
extern "C" REX_FUNC(sub_821E6C50) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_tuning_screen) || !hooks::TuningUpdateCalleesNative()) {
    __imp__sub_821E6C50(ctx, base);
    return;
  }
  hooks::TuningUpdate(ctx, base);
}
