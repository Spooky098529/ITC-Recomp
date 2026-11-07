// The tuning screen's register steps (src/tuning_screen/,
// docs/functions/tuning-screen.md), shared by the adapters
// (src/hooks/tuning_screen.cpp, behind --native_tuning_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// The part tables: r3 = the screen. TuningScreen::CategoryItemCount
// sub_821CC6C0, IsItemAvailable sub_821CDB18 (r4 = an item),
// WheelNumber sub_821CDED8 (r4 = a brand, r5 = a model), GetPrice
// sub_821CD160, GetCurrentChoice sub_821CD868, GetChoiceFlag sub_821CDC38
// (r4 = a choice).
void TuningCategoryItemCount(PPCContext& ctx, uint8_t* base);
void TuningIsItemAvailable(PPCContext& ctx, uint8_t* base);
void TuningWheelNumber(PPCContext& ctx, uint8_t* base);
void TuningGetPrice(PPCContext& ctx, uint8_t* base);
void TuningGetCurrentChoice(PPCContext& ctx, uint8_t* base);
void TuningGetChoiceFlag(PPCContext& ctx, uint8_t* base);

// The garage car: StoreToGarageCar sub_821DCC28, EquipChoice sub_821DF448
// (r3 = the screen).
void TuningStoreToGarageCar(PPCContext& ctx, uint8_t* base);
void TuningEquipChoice(PPCContext& ctx, uint8_t* base);
// AeroPartValue sub_821DE2D8: r3 = the screen, r4 = the type - 1, r5 = the
// aero item.
void TuningAeroPartValue(PPCContext& ctx, uint8_t* base);

// The part list's marks: HidePartMarks sub_821EE6D0, ShowPartMarks
// sub_821EE760 (r3 = the screen, r4 = a row node, r5 = a group, r6 = an
// index).
void TuningHidePartMarks(PPCContext& ctx, uint8_t* base);
void TuningShowPartMarks(PPCContext& ctx, uint8_t* base);

// Movie 2's node setters (r3 = the screen): ShowInteriorPicture
// sub_821DA3F8 (r4 a type, r5 an index), HideTireMarks sub_821DFB20,
// HideNode19 sub_821DFBE8, ShowTireMarks sub_821E22E8.
void TuningShowInteriorPicture(PPCContext& ctx, uint8_t* base);
void TuningHideTireMarks(PPCContext& ctx, uint8_t* base);
void TuningHideNode19(PPCContext& ctx, uint8_t* base);
void TuningShowTireMarks(PPCContext& ctx, uint8_t* base);

// The lists on movies 2 and 13 (r3 = the screen; r4 = 0 to hide, else
// show, for ShowOptionMarks and ShowItems): UpdateListDots sub_821EE9B0,
// ShowOptionMarks sub_82202F30, PlaceStateNodes sub_82202C08, LayOutItems
// sub_82203110, ShowItems sub_82203B00.
void TuningUpdateListDots(PPCContext& ctx, uint8_t* base);
void TuningShowOptionMarks(PPCContext& ctx, uint8_t* base);
void TuningPlaceStateNodes(PPCContext& ctx, uint8_t* base);
void TuningLayOutItems(PPCContext& ctx, uint8_t* base);
void TuningShowItems(PPCContext& ctx, uint8_t* base);

// The movie calls (r3 = the screen; the UI movie calls through their
// overrides, lr put back): UpdateListCounter sub_821EED00, SetPartName
// sub_821F2068, PlayGroupAnimation sub_821EE880, Command31 sub_82202880,
// Command9 sub_821E9050.
void TuningUpdateListCounter(PPCContext& ctx, uint8_t* base);
void TuningSetPartName(PPCContext& ctx, uint8_t* base);
void TuningPlayGroupAnimation(PPCContext& ctx, uint8_t* base);
void TuningCommand31(PPCContext& ctx, uint8_t* base);
void TuningCommand9(PPCContext& ctx, uint8_t* base);
// ShowDialog sub_821D6230 and Command30 sub_82202750 (r4 = the update's
// dispatcher), Command7 sub_821E8B68, Command23 sub_82202368, Command18
// sub_82200600, Command25 sub_82204888.
void TuningShowDialog(PPCContext& ctx, uint8_t* base);
void TuningCommand30(PPCContext& ctx, uint8_t* base);
void TuningCommand7(PPCContext& ctx, uint8_t* base);
void TuningCommand23(PPCContext& ctx, uint8_t* base);
void TuningCommand18(PPCContext& ctx, uint8_t* base);
void TuningCommand25(PPCContext& ctx, uint8_t* base);
// PlayIntro sub_821D06B0, OpenItems sub_821D0A48, Command15 sub_821FF1D0,
// Command22 sub_82202090.
void TuningPlayIntro(PPCContext& ctx, uint8_t* base);
void TuningOpenItems(PPCContext& ctx, uint8_t* base);
void TuningCommand15(PPCContext& ctx, uint8_t* base);
void TuningCommand22(PPCContext& ctx, uint8_t* base);

// The category bar and the lists (src/tuning_screen/menus.cpp; r3 = the
// screen): MoveCategory sub_821D07D8 (the player's mapper's slot 10 by
// ActionMapper::Pressed's plain C++ with --native_action_mappers), MarkPick
// sub_821D6420, Command16 sub_821FF6F0, ShowOptions sub_821D5028,
// ShowLevels sub_821D12C0.
void TuningMoveCategory(PPCContext& ctx, uint8_t* base);
void TuningMarkPick(PPCContext& ctx, uint8_t* base);
void TuningCommand16(PPCContext& ctx, uint8_t* base);
void TuningShowOptions(PPCContext& ctx, uint8_t* base);
void TuningShowLevels(PPCContext& ctx, uint8_t* base);
// UpdateItems sub_822036E8 (the mapper's slot 8 by ActionMapper::Held's
// plain C++ with --native_action_mappers), ShowTypes sub_821D2A80 (the aero
// parts by their plain C++ with --native_car_specs), SetStateTexts
// sub_821F1DE0 (f1 as the original leaves it) and FillPartRows sub_821F4BF8.
void TuningUpdateItems(PPCContext& ctx, uint8_t* base);
void TuningShowTypes(PPCContext& ctx, uint8_t* base);
void TuningSetStateTexts(PPCContext& ctx, uint8_t* base);
void TuningFillPartRows(PPCContext& ctx, uint8_t* base);
// Command8 sub_821F5F70 (r3 all 64 bits).
void TuningCommand8(PPCContext& ctx, uint8_t* base);
// RefreshMenu sub_821D8978 (f1 as the original leaves it: each native
// AnimationTime's or NamedAnimationTime's put in f1 before the next guest
// call and at the end).
void TuningRefreshMenu(PPCContext& ctx, uint8_t* base);
// RefreshMovie13 sub_822076E8 (r3 the screen; f1 as the original leaves it).
void TuningRefreshMovie13(PPCContext& ctx, uint8_t* base);

// The calls onto the stand (src/tuning_screen/stand.cpp; the adapters only
// with TuningStandCalleesNative: --native_stand_models, --native_race_setup
// and CarMakersSetCarCalleesNative): SetInteriorPair sub_821CCC50 (r4 a
// choice, r5 a level; no adapter, the harness's step), GarageStage::FitSelectedCar sub_8217DAC0 (r3 the
// holder, r4, r5 two bytes; f1 FitCar's), PickCameraPreset sub_821D99A0,
// UpdateBusyIcon sub_821D6100, PreviewWheels sub_821E0E00 (f1 as
// PreviewWheel leaves it) and ApplyDecals sub_822042D8 (r3 the screen).
bool TuningStandCalleesNative();
void TuningSetInteriorPair(PPCContext& ctx, uint8_t* base);
void TuningFitSelectedCar(PPCContext& ctx, uint8_t* base);
void TuningPickCameraPreset(PPCContext& ctx, uint8_t* base);
void TuningUpdateBusyIcon(PPCContext& ctx, uint8_t* base);
void TuningPreviewWheels(PPCContext& ctx, uint8_t* base);
void TuningApplyDecals(PPCContext& ctx, uint8_t* base);
// DecalCommand sub_822019D0 (r3 the screen; f1 as its last text call leaves
// it), with TuningStandCalleesNative.
void TuningDecalCommand(PPCContext& ctx, uint8_t* base);
// PreviewItem sub_821E07B0 and RestoreStand sub_821E0FB0 (r3 the screen;
// RestoreStand's f1 as the original leaves it), with
// TuningStandCalleesNative.
void TuningPreviewItem(PPCContext& ctx, uint8_t* base);
void TuningRestoreStand(PPCContext& ctx, uint8_t* base);
// SetCarStats sub_821DF788 (r3 the screen; f1 its last SetText's), only with
// TuningCarStatsCalleesNative: CarMakersGarageCalleesNative and
// --native_vehicle.
bool TuningCarStatsCalleesNative();
void TuningSetCarStats(PPCContext& ctx, uint8_t* base);
// UpdateStand sub_821D9F10 (r3 the screen; r3 GarageStage::Draw's), only
// with TuningUpdateStandCalleesNative: TuningStandCalleesNative,
// FrontEndCarDrawDrawNative, --native_vehicle_accessors and
// --native_scene_models.
bool TuningUpdateStandCalleesNative();
void TuningUpdateStand(PPCContext& ctx, uint8_t* base);

// The debug state handlers (r3 = the screen): DebugMainMenu sub_821CBE08,
// DebugLevelSelect sub_821CBEE8, DebugTypeSelect sub_821CBFC0,
// DebugOptionSelect sub_821CC248, DebugConfirmBuy sub_821CC3A8,
// DebugConfirmEquip sub_821CC488, DebugCategoryMenu sub_821CFCE8,
// DebugWheelPartSelect sub_821E8068, DebugWheelKindSelect sub_821E8128,
// DebugWheelInchSelect sub_821E8298, DebugDecalTopMenu sub_821FD710,
// DebugLayerSelect sub_821FD880, DebugLayerEdit sub_821FDCB8, DebugLayerSet
// sub_821FE080, DebugEditPosition sub_821FE100, DebugEditView sub_821FE178,
// DebugPaintConfirm sub_821FE190.
void TuningDebugMainMenu(PPCContext& ctx, uint8_t* base);
void TuningDebugLevelSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugTypeSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugOptionSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugConfirmBuy(PPCContext& ctx, uint8_t* base);
void TuningDebugConfirmEquip(PPCContext& ctx, uint8_t* base);
void TuningDebugCategoryMenu(PPCContext& ctx, uint8_t* base);
void TuningDebugWheelPartSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugWheelKindSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugWheelInchSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugDecalTopMenu(PPCContext& ctx, uint8_t* base);
void TuningDebugLayerSelect(PPCContext& ctx, uint8_t* base);
void TuningDebugLayerEdit(PPCContext& ctx, uint8_t* base);
void TuningDebugLayerSet(PPCContext& ctx, uint8_t* base);
void TuningDebugEditPosition(PPCContext& ctx, uint8_t* base);
void TuningDebugEditView(PPCContext& ctx, uint8_t* base);
void TuningDebugPaintConfirm(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
