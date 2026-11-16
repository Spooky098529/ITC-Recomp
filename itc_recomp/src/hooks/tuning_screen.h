// The tuning screen's register steps (src/tuning_screen/,
// docs/functions/tuning-screen.md), shared by the adapters
// (src/hooks/tuning_screen.cpp, behind --native_tuning_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include <functional>
#include <optional>

namespace tuning_screen {
struct StandCalls;
}  // namespace tuning_screen

namespace hooks {

// The part tables: r3 = the screen. TuningScreen::CategoryItemCount
// sub_821CC6C0, IsItemAvailable sub_821CDB18 (r4 = an item),
// WheelNumber sub_821CDED8 (r4 = a brand, r5 = a model), GetPrice
// sub_821CD160, GetCurrentChoice sub_821CD868, GetChoiceFlag sub_821CDC38
// (r4 = a choice).
void TuningCategoryItemCount(lifted::Regs& ctx, uint8_t* base);
void TuningIsItemAvailable(lifted::Regs& ctx, uint8_t* base);
void TuningWheelNumber(lifted::Regs& ctx, uint8_t* base);
void TuningGetPrice(lifted::Regs& ctx, uint8_t* base);
void TuningGetCurrentChoice(lifted::Regs& ctx, uint8_t* base);
void TuningGetChoiceFlag(lifted::Regs& ctx, uint8_t* base);

// The garage car: StoreToGarageCar sub_821DCC28, EquipChoice sub_821DF448
// (r3 = the screen).
void TuningStoreToGarageCar(lifted::Regs& ctx, uint8_t* base);
void TuningEquipChoice(lifted::Regs& ctx, uint8_t* base);
// AeroPartValue sub_821DE2D8: r3 = the screen, r4 = the type - 1, r5 = the
// aero item.
void TuningAeroPartValue(lifted::Regs& ctx, uint8_t* base);

// The part list's marks: HidePartMarks sub_821EE6D0, ShowPartMarks
// sub_821EE760 (r3 = the screen, r4 = a row node, r5 = a group, r6 = an
// index).
void TuningHidePartMarks(lifted::Regs& ctx, uint8_t* base);
void TuningShowPartMarks(lifted::Regs& ctx, uint8_t* base);

// Movie 2's node setters (r3 = the screen): ShowInteriorPicture
// sub_821DA3F8 (r4 a type, r5 an index), HideTireMarks sub_821DFB20,
// HideNode19 sub_821DFBE8, ShowTireMarks sub_821E22E8.
void TuningShowInteriorPicture(lifted::Regs& ctx, uint8_t* base);
void TuningHideTireMarks(lifted::Regs& ctx, uint8_t* base);
void TuningHideNode19(lifted::Regs& ctx, uint8_t* base);
void TuningShowTireMarks(lifted::Regs& ctx, uint8_t* base);

// The lists on movies 2 and 13 (r3 = the screen; r4 = 0 to hide, else
// show, for ShowOptionMarks and ShowItems): UpdateListDots sub_821EE9B0,
// ShowOptionMarks sub_82202F30, PlaceStateNodes sub_82202C08, LayOutItems
// sub_82203110, ShowItems sub_82203B00.
void TuningUpdateListDots(lifted::Regs& ctx, uint8_t* base);
void TuningShowOptionMarks(lifted::Regs& ctx, uint8_t* base);
void TuningPlaceStateNodes(lifted::Regs& ctx, uint8_t* base);
void TuningLayOutItems(lifted::Regs& ctx, uint8_t* base);
void TuningShowItems(lifted::Regs& ctx, uint8_t* base);

// The movie calls (r3 = the screen; the UI movie calls through their
// overrides, lr put back): UpdateListCounter sub_821EED00, SetPartName
// sub_821F2068, PlayGroupAnimation sub_821EE880, Command31 sub_82202880,
// Command9 sub_821E9050.
void TuningUpdateListCounter(lifted::Regs& ctx, uint8_t* base);
void TuningSetPartName(lifted::Regs& ctx, uint8_t* base);
void TuningPlayGroupAnimation(lifted::Regs& ctx, uint8_t* base);
void TuningCommand31(lifted::Regs& ctx, uint8_t* base);
void TuningCommand9(lifted::Regs& ctx, uint8_t* base);
// ShowDialog sub_821D6230 and Command30 sub_82202750 (r4 = the update's
// dispatcher), Command7 sub_821E8B68, Command23 sub_82202368, Command18
// sub_82200600, Command25 sub_82204888.
void TuningShowDialog(lifted::Regs& ctx, uint8_t* base);
void TuningCommand30(lifted::Regs& ctx, uint8_t* base);
void TuningCommand7(lifted::Regs& ctx, uint8_t* base);
void TuningCommand23(lifted::Regs& ctx, uint8_t* base);
void TuningCommand18(lifted::Regs& ctx, uint8_t* base);
void TuningCommand25(lifted::Regs& ctx, uint8_t* base);
// PlayIntro sub_821D06B0, OpenItems sub_821D0A48, Command15 sub_821FF1D0,
// Command22 sub_82202090.
void TuningPlayIntro(lifted::Regs& ctx, uint8_t* base);
void TuningOpenItems(lifted::Regs& ctx, uint8_t* base);
void TuningCommand15(lifted::Regs& ctx, uint8_t* base);
void TuningCommand22(lifted::Regs& ctx, uint8_t* base);

// The category bar and the lists (src/tuning_screen/menus.cpp; r3 = the
// screen): MoveCategory sub_821D07D8 (the player's mapper's slot 10 by
// ActionMapper::Pressed's plain C++ with --native_action_mappers), MarkPick
// sub_821D6420, Command16 sub_821FF6F0, ShowOptions sub_821D5028,
// ShowLevels sub_821D12C0.
void TuningMoveCategory(lifted::Regs& ctx, uint8_t* base);
void TuningMarkPick(lifted::Regs& ctx, uint8_t* base);
void TuningCommand16(lifted::Regs& ctx, uint8_t* base);
void TuningShowOptions(lifted::Regs& ctx, uint8_t* base);
void TuningShowLevels(lifted::Regs& ctx, uint8_t* base);
// UpdateItems sub_822036E8 (the mapper's slot 8 by ActionMapper::Held's
// plain C++ with --native_action_mappers), ShowTypes sub_821D2A80 (the aero
// parts by their plain C++ with --native_car_specs), SetStateTexts
// sub_821F1DE0 (f1 as the original leaves it) and FillPartRows sub_821F4BF8.
void TuningUpdateItems(lifted::Regs& ctx, uint8_t* base);
void TuningShowTypes(lifted::Regs& ctx, uint8_t* base);
void TuningSetStateTexts(lifted::Regs& ctx, uint8_t* base);
void TuningFillPartRows(lifted::Regs& ctx, uint8_t* base);
// Command8 sub_821F5F70 (r3 all 64 bits).
void TuningCommand8(lifted::Regs& ctx, uint8_t* base);
// RefreshMenu sub_821D8978 (f1 as the original leaves it: each native
// AnimationTime's or NamedAnimationTime's put in f1 before the next guest
// call and at the end).
void TuningRefreshMenu(lifted::Regs& ctx, uint8_t* base);
// RefreshMovie13 sub_822076E8 (r3 the screen; f1 as the original leaves it).
void TuningRefreshMovie13(lifted::Regs& ctx, uint8_t* base);
// Movie 2's command runners never seen in play, for the harness only (no
// recompiled caller reaches them: the update's plain C++ calls their plain
// C++; r3 = the screen): Command5 sub_821D4180, Command11 sub_821E9730,
// Command12 sub_821E9CA0, Command54 sub_821EB8E8, Command55 sub_821EBDC8,
// Command56 sub_821EC6A0, Command57 sub_821ECC68 (f1 as the original leaves
// it, RefreshMenu's way) and Command58 sub_821EDB70.
void TuningCommand5(lifted::Regs& ctx, uint8_t* base);
void TuningCommand11(lifted::Regs& ctx, uint8_t* base);
void TuningCommand12(lifted::Regs& ctx, uint8_t* base);
void TuningCommand54(lifted::Regs& ctx, uint8_t* base);
void TuningCommand55(lifted::Regs& ctx, uint8_t* base);
void TuningCommand56(lifted::Regs& ctx, uint8_t* base);
void TuningCommand57(lifted::Regs& ctx, uint8_t* base);
void TuningCommand58(lifted::Regs& ctx, uint8_t* base);
// Command10 sub_821F35C0, Command13 sub_821EA460 and Command14 sub_821EAFA0
// (the paint and light kit pages; the same way, B7 loop, 2026-11-13).
void TuningCommand10(lifted::Regs& ctx, uint8_t* base);
void TuningCommand13(lifted::Regs& ctx, uint8_t* base);
void TuningCommand14(lifted::Regs& ctx, uint8_t* base);
// The editors' command runners never seen in play, for the harness only
// (the same way; r3 = the screen): Command17 sub_82200100, Command19
// sub_82201C58, Command20 sub_82201FA0 (r4, r5 the update's +9644 and
// +9648), Command21 sub_822087C8 (r3 all 64 bits), Command26 sub_82204980,
// Command27 sub_82204A78, Command32Done sub_821F7498, Command42
// sub_821F7618, Command52 sub_821F77F0 (r3 all 64 bits, r4 the update's
// dispatcher) and Command53 sub_821F7948 (r3 left).
void TuningCommand17(lifted::Regs& ctx, uint8_t* base);
void TuningCommand19(lifted::Regs& ctx, uint8_t* base);
void TuningCommand20(lifted::Regs& ctx, uint8_t* base);
void TuningCommand21(lifted::Regs& ctx, uint8_t* base);
void TuningCommand26(lifted::Regs& ctx, uint8_t* base);
void TuningCommand27(lifted::Regs& ctx, uint8_t* base);
void TuningCommand32Done(lifted::Regs& ctx, uint8_t* base);
void TuningCommand42(lifted::Regs& ctx, uint8_t* base);
void TuningCommand52(lifted::Regs& ctx, uint8_t* base);
void TuningCommand53(lifted::Regs& ctx, uint8_t* base);

// The calls onto the stand (src/tuning_screen/stand.cpp; the adapters only
// with TuningStandCalleesNative: --native_stand_models, --native_race_setup
// and CarMakersSetCarCalleesNative): SetInteriorPair sub_821CCC50 (r4 a
// choice, r5 a level; no adapter, the harness's step), GarageStage::FitSelectedCar sub_8217DAC0 (r3 the
// holder, r4, r5 two bytes; f1 FitCar's; no adapter since 2026-11-13, the
// harness's step), PickCameraPreset sub_821D99A0,
// UpdateBusyIcon sub_821D6100, PreviewWheels sub_821E0E00 (f1 as
// PreviewWheel leaves it) and ApplyDecals sub_822042D8 (r3 the screen).
bool TuningStandCalleesNative();
void TuningSetInteriorPair(lifted::Regs& ctx, uint8_t* base);
void TuningFitSelectedCar(lifted::Regs& ctx, uint8_t* base);
void TuningPickCameraPreset(lifted::Regs& ctx, uint8_t* base);
void TuningUpdateBusyIcon(lifted::Regs& ctx, uint8_t* base);
void TuningPreviewWheels(lifted::Regs& ctx, uint8_t* base);
void TuningApplyDecals(lifted::Regs& ctx, uint8_t* base);
// ApplyAreaDecals sub_821F7B90 and ApplyDecalWord sub_821FE4E0 (r3 the
// screen), without adapters (only UpdateStand's plain C++ calls them; the
// harness's).
void TuningApplyAreaDecals(lifted::Regs& ctx, uint8_t* base);
void TuningApplyDecalWord(lifted::Regs& ctx, uint8_t* base);
// PreviewParts sub_821CCAA8 (r3 the screen, r4 a pick), without an adapter
// (only PreviewItem's and RestoreStand's plain C++ call it; the harness's).
void TuningPreviewParts(lifted::Regs& ctx, uint8_t* base);
// DecalCommand sub_822019D0 (r3 the screen; f1 as its last text call leaves
// it), with TuningStandCalleesNative.
void TuningDecalCommand(lifted::Regs& ctx, uint8_t* base);
// PreviewItem sub_821E07B0 and RestoreStand sub_821E0FB0 (r3 the screen;
// RestoreStand's f1 as the original leaves it), with
// TuningStandCalleesNative.
void TuningPreviewItem(lifted::Regs& ctx, uint8_t* base);
void TuningRestoreStand(lifted::Regs& ctx, uint8_t* base);
// The tuning screen's stand setters never seen in play, without adapters
// (only PreviewItem's, RestoreStand's and the refresh's plain C++ reach
// them; the harness's; lr put back): SetCaliper sub_821DA330 (r3 the
// screen, r4, r5 a choice), SetAccessory sub_821CCBE8 (r4 a kind, r5 a
// choice), RefitAxles sub_821DF9D0 (r3 the screen; f1 FitAxles'),
// PreviewInteriorChoices sub_821E7E48 (r3 the screen), PreviewLightKit
// sub_821CC930 (r3 the screen; f1 SetLightKit's) and UiNode::SetRgb
// sub_821E7DA0 (r4 a node, r5 to r7 the color; r3 left).
void TuningSetCaliper(lifted::Regs& ctx, uint8_t* base);
void TuningSetAccessory(lifted::Regs& ctx, uint8_t* base);
void TuningRefitAxles(lifted::Regs& ctx, uint8_t* base);
void TuningPreviewInteriorChoices(lifted::Regs& ctx, uint8_t* base);
void TuningPreviewLightKit(lifted::Regs& ctx, uint8_t* base);
void TuningSetRgb(lifted::Regs& ctx, uint8_t* base);
// SetCarStats sub_821DF788 (r3 the screen; f1 its last SetText's), only with
// TuningCarStatsCalleesNative: CarMakersGarageCalleesNative and
// --native_vehicle.
bool TuningCarStatsCalleesNative();
void TuningSetCarStats(lifted::Regs& ctx, uint8_t* base);
// Refresh sub_821E3AD0 (r3 the screen, all 64 bits; r3 1 or 0; f1 as the
// original leaves it, RefreshMenu's way), with TuningStandCalleesNative and
// TuningCarStatsCalleesNative (and --native_car_specs, its adapter's check):
// every state natively.
void TuningRefresh(lifted::Regs& ctx, uint8_t* base);
// The refresh's guest calls for a native caller (TuningMachine::Start):
// `body` run with the stand's calls (as TuningRefresh's), the pending `*f1`
// put in f1 before each guest call through `call` (and at the end); lr put
// back, flush-to-zero off, ctx.r3 the body's.
void TuningWithRefreshStand(
    lifted::Regs& ctx, uint8_t* base, std::optional<double>* f1,
    const std::function<uint64_t(const tuning_screen::StandCalls&)>& body);
// UpdateStand sub_821D9F10 (r3 the screen; r3 GarageStage::Draw's), only
// with TuningUpdateStandCalleesNative: TuningStandCalleesNative,
// FrontEndCarDrawDrawNative, --native_vehicle_accessors and
// --native_scene_models.
bool TuningUpdateStandCalleesNative();
// `caller_r24` (handed to GarageStage::Draw's Floor::Draw, a diagnostic
// word) isn't carried (the rewiring, stage 3): callers pass 0.
void TuningUpdateStand(lifted::Regs& ctx, uint8_t* base);
void TuningUpdateStand(lifted::Regs& ctx, uint8_t* base, uint32_t caller_r24);

// Update sub_821E6C50 (r3 the screen, r4 the mode's dispatcher, both all
// 64 bits; r3 UpdateStand's or RestoreStand's; f1 as the original leaves
// it, RefreshMenu's way), with TuningUpdateCalleesNative:
// TuningUpdateStandCalleesNative, TuningCarStatsCalleesNative and
// --native_car_specs (the refresh's and its other callees' adapters'
// checks).
bool TuningUpdateCalleesNative();
// `caller_r24` (handed to GarageStage::Draw's Floor::Draw, a diagnostic
// word) isn't carried (the rewiring, stage 3): callers pass 0.
void TuningUpdate(lifted::Regs& ctx, uint8_t* base);
void TuningUpdate(lifted::Regs& ctx, uint8_t* base, uint32_t caller_r24);

// The debug state handlers (r3 = the screen): DebugMainMenu sub_821CBE08,
// DebugLevelSelect sub_821CBEE8, DebugTypeSelect sub_821CBFC0,
// DebugOptionSelect sub_821CC248, DebugConfirmBuy sub_821CC3A8,
// DebugConfirmEquip sub_821CC488, DebugCategoryMenu sub_821CFCE8,
// DebugWheelPartSelect sub_821E8068, DebugWheelKindSelect sub_821E8128,
// DebugWheelInchSelect sub_821E8298, DebugDecalTopMenu sub_821FD710,
// DebugLayerSelect sub_821FD880, DebugLayerEdit sub_821FDCB8, DebugLayerSet
// sub_821FE080, DebugEditPosition sub_821FE100, DebugEditView sub_821FE178,
// DebugPaintConfirm sub_821FE190.
void TuningDebugMainMenu(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLevelSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugTypeSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugOptionSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugConfirmBuy(lifted::Regs& ctx, uint8_t* base);
void TuningDebugConfirmEquip(lifted::Regs& ctx, uint8_t* base);
void TuningDebugCategoryMenu(lifted::Regs& ctx, uint8_t* base);
void TuningDebugWheelPartSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugWheelKindSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugWheelInchSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugDecalTopMenu(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLayerSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLayerEdit(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLayerSet(lifted::Regs& ctx, uint8_t* base);
void TuningDebugEditPosition(lifted::Regs& ctx, uint8_t* base);
void TuningDebugEditView(lifted::Regs& ctx, uint8_t* base);
void TuningDebugPaintConfirm(lifted::Regs& ctx, uint8_t* base);
// The debug state handlers never seen in play, for the harness only (no
// recompiled caller reaches them in play: the update's plain C++ calls their
// plain C++): DebugColorSelect sub_821CC078 to DebugPaintRemoveConfirm
// sub_821FE2D0 (tuning_screen/tuning_screen.h).
void TuningDebugColorSelect(lifted::Regs& ctx, uint8_t* base);
void TuningDebugWarnMoney(lifted::Regs& ctx, uint8_t* base);
void TuningDebugReflectSet(lifted::Regs& ctx, uint8_t* base);
void TuningDebugAnnulSet(lifted::Regs& ctx, uint8_t* base);
void TuningDebugRemoveConfirm(lifted::Regs& ctx, uint8_t* base);
void TuningDebugBodyColor(lifted::Regs& ctx, uint8_t* base);
void TuningDebugNeonPart(lifted::Regs& ctx, uint8_t* base);
void TuningDebugNeonKind(lifted::Regs& ctx, uint8_t* base);
void TuningDebugNeonEdit(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLedEdit(lifted::Regs& ctx, uint8_t* base);
void TuningDebugCockpitTop(lifted::Regs& ctx, uint8_t* base);
void TuningDebugCockpitType(lifted::Regs& ctx, uint8_t* base);
void TuningDebugCockpitColor(lifted::Regs& ctx, uint8_t* base);
void TuningDebugMeterKind(lifted::Regs& ctx, uint8_t* base);
void TuningDebugMeterPoint(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLayerOperate(lifted::Regs& ctx, uint8_t* base);
void TuningDebugLayerOrder(lifted::Regs& ctx, uint8_t* base);
void TuningDebugEditRotate(lifted::Regs& ctx, uint8_t* base);
void TuningDebugEditScale(lifted::Regs& ctx, uint8_t* base);
void TuningDebugEditColor(lifted::Regs& ctx, uint8_t* base);
void TuningDebugEditFlip(lifted::Regs& ctx, uint8_t* base);
void TuningDebugPaintCancelConfirm(lifted::Regs& ctx, uint8_t* base);
void TuningDebugPaintRemoveConfirm(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
