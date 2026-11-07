// The type 6 and 7 modes' register steps (src/type67_modes/,
// docs/functions/type6-type7-modes.md), shared by the adapters
// (src/hooks/type67_modes.cpp, behind --native_type67_modes) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "type67_modes/type67_modes.h"

#include <cstdint>

namespace hooks {

// --native_type67_modes.
bool Type67ModesNative();

// A state machine Init (type67_modes::kInits) at `address`: r3 the object;
// r3 after as the last StateMachine::Add's adapter leaves it.
void Type67Init(PPCContext& ctx, uint8_t* base, uint32_t address);

// The handlers without guest calls (type67_modes handlers.cpp), each with
// the registers its original takes. `Freeing` steps return false, with
// nothing done, where a block isn't the allocation layer's pages (the
// adapters then run the originals); the allocating ones run only over the
// layer (the adapters check NativeHeapActive).
void GarageStageSetPlates(PPCContext& ctx, uint8_t* base);
void GarageStageResetPose(PPCContext& ctx, uint8_t* base);
void CameraPresetConstruct(PPCContext& ctx, uint8_t* base);
void CameraPresetCopy(PPCContext& ctx, uint8_t* base);
bool GarageCameraFreePresets(PPCContext& ctx, uint8_t* base);
bool GarageCameraOnPresetsLoaded(PPCContext& ctx, uint8_t* base);
bool StandSlotFree(PPCContext& ctx, uint8_t* base);
void ControllerOptionsOnEnter(PPCContext& ctx, uint8_t* base);
bool ControllerOptionsOnLeft(PPCContext& ctx, uint8_t* base);
void ControllerOptionsShow(PPCContext& ctx, uint8_t* base);
void ControllerOptionsOpen(PPCContext& ctx, uint8_t* base);
void ControllerOptionsLeave(PPCContext& ctx, uint8_t* base);
void PanelScreenInitWords(PPCContext& ctx, uint8_t* base);
void ScreenMachineWait(PPCContext& ctx, uint8_t* base);
void ScreenReenter(PPCContext& ctx, uint8_t* base);
void Type6DialogShown(PPCContext& ctx, uint8_t* base);
void Type7DialogShown(PPCContext& ctx, uint8_t* base);
void Type6DialogOnEnter(PPCContext& ctx, uint8_t* base);
void Type7DialogOnEnter(PPCContext& ctx, uint8_t* base);
void Type6DialogWaitForMovies(PPCContext& ctx, uint8_t* base);
void Type7DialogWaitForMovies(PPCContext& ctx, uint8_t* base);
void Type6Screen0OnEnter(PPCContext& ctx, uint8_t* base);
void Type7Screen0OnEnter(PPCContext& ctx, uint8_t* base);
bool Type6Screen0OnLeft(PPCContext& ctx, uint8_t* base);
bool Type7Screen0OnLeft(PPCContext& ctx, uint8_t* base);
void Type6Screen0WaitForMovie(PPCContext& ctx, uint8_t* base);
void Type7Screen0WaitForMovie(PPCContext& ctx, uint8_t* base);
void Type6Screen0PickYes(PPCContext& ctx, uint8_t* base);
void Type6Screen0PickNo(PPCContext& ctx, uint8_t* base);
void Type7Screen0PickYes(PPCContext& ctx, uint8_t* base);
void Type6Screen0GoOn(PPCContext& ctx, uint8_t* base);

// The modes' delegate functions (r3 the answer, r4 the mode; r3 left the
// answer): Type6Mode::NextMode sub_822363B8, Type7Mode::NextMode
// sub_82246800, Type6Mode::Leaving sub_82236100, Type7Mode::Leaving
// sub_82246590. Their guest calls through `ctx`.
void Type6ModeNextMode(PPCContext& ctx, uint8_t* base);
void Type7ModeNextMode(PPCContext& ctx, uint8_t* base);
void Type6ModeLeaving(PPCContext& ctx, uint8_t* base);
void Type7ModeLeaving(PPCContext& ctx, uint8_t* base);

// The plain C++'s guest calls through `ctx` (r1 and lr put back after):
// any function, and UiMovies::Open and Delete natively where their
// adapters would run natively, else their adapters.
type67_modes::Calls Type67Calls(PPCContext& ctx, uint8_t* base);

// The handlers with guest calls (screens.cpp), with their originals'
// registers: Dialog::OpenMovies sub_82225810, the dialogs' Leave
// sub_82225C70 and sub_82241E58 and the first screens' sub_82233DF8 and
// sub_82244350 (r3 the screen, r4 the dispatcher), ListScreen::Init
// sub_822515A0, PanelScreen::Init sub_82248E10, NodeScreen::Init
// sub_82259128, and the Draw handlers sub_82234ED8, sub_82232740,
// sub_822421C0, sub_82245148 and sub_821C7F28 (r3, r4).
void DialogOpenMovies(PPCContext& ctx, uint8_t* base);
void Type6DialogLeave(PPCContext& ctx, uint8_t* base);
void Type7DialogLeave(PPCContext& ctx, uint8_t* base);
void Type6Screen0Leave(PPCContext& ctx, uint8_t* base);
void Type7Screen0Leave(PPCContext& ctx, uint8_t* base);
void ListScreenInit(PPCContext& ctx, uint8_t* base);
void PanelScreenInit(PPCContext& ctx, uint8_t* base);
void NodeScreenInit(PPCContext& ctx, uint8_t* base);
void Type6Screen0Draw(PPCContext& ctx, uint8_t* base);
void Type6DialogDraw(PPCContext& ctx, uint8_t* base);
void Type7DialogDraw(PPCContext& ctx, uint8_t* base);
void Type7Screen0Draw(PPCContext& ctx, uint8_t* base);
void ControllerOptionsDraw(PPCContext& ctx, uint8_t* base);

// GarageCamera::LoadPresets sub_821802D8 (r3 the camera; needs the native
// heap and the archive's queue: the adapter checks) and StandScene::Release
// sub_82180738 (false where a block isn't the layer's pages).
void GarageCameraLoadPresets(PPCContext& ctx, uint8_t* base);
bool StandSceneRelease(PPCContext& ctx, uint8_t* base);

// The dialogs' confirm (r3 the dialog): Type6Dialog::OpenConfirm
// sub_822320D8, Type7Dialog::OpenConfirm sub_82241B40, Dialog::DrawWaiting
// sub_82241C18, Dialog::OpenConfirmStep sub_822325E0, CloseConfirmStep
// sub_8223AA10 and MoveCursorStep sub_8223AB50.
void Type6DialogOpenConfirm(PPCContext& ctx, uint8_t* base);
void Type7DialogOpenConfirm(PPCContext& ctx, uint8_t* base);
void DialogDrawWaiting(PPCContext& ctx, uint8_t* base);
void DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base);
void DialogCloseConfirmStep(PPCContext& ctx, uint8_t* base);
void DialogMoveCursorStep(PPCContext& ctx, uint8_t* base);
// The dialogs' page 0 and Update (r3 the dialog, r4 the dispatcher):
// Type6Dialog::ConfirmPage sub_82232328, Type7Dialog::ConfirmPage
// sub_82241EF8, Type6Dialog::Update sub_82232798 and Type7Dialog::Update
// sub_82242218.
void Type6DialogConfirmPage(PPCContext& ctx, uint8_t* base);
void Type7DialogConfirmPage(PPCContext& ctx, uint8_t* base);
void Type6DialogUpdate(PPCContext& ctx, uint8_t* base);
void Type7DialogUpdate(PPCContext& ctx, uint8_t* base);

// The first screens' confirms (r4 the dispatcher): Type6Screen0::Confirm
// sub_82233FC8, ConfirmAgain sub_822340B0 and Type7Screen0::Confirm
// sub_82244528; Type6Screen0::ShowPage sub_822344A8 (r4 the page). The
// confirms need the UI movies' text setters native (Type67TextsNative).
bool Type67TextsNative(uint8_t* base);
void Type6Screen0Confirm(PPCContext& ctx, uint8_t* base);
void Type6Screen0ConfirmAgain(PPCContext& ctx, uint8_t* base);
void Type7Screen0Confirm(PPCContext& ctx, uint8_t* base);
void Type6Screen0ShowPage(PPCContext& ctx, uint8_t* base);
// The type 7 first screen's helpers: ShowPage sub_82244930 and PlaceCursor
// sub_822448B0 (r4 the page or item), ShowHelp sub_82244AB8 (r4 the item;
// needs Type67TextsNative), FadeOutStep sub_82244760 (r3 the screen) and
// ChosenPage sub_82244610 (r3 the screen, r4 the dispatcher).
void Type7Screen0ShowPage(PPCContext& ctx, uint8_t* base);
void Type7Screen0PlaceCursor(PPCContext& ctx, uint8_t* base);
void Type7Screen0ShowHelp(PPCContext& ctx, uint8_t* base);
void Type7Screen0FadeOutStep(PPCContext& ctx, uint8_t* base);
void Type7Screen0ChosenPage(PPCContext& ctx, uint8_t* base);
// The first screens' OpenMovie (Type6 sub_822338E8, Type7 sub_82243E80) and
// SetUpCar (Type6 sub_82233E80, Type7 sub_822443E0; r4 the dispatcher).
void Type6Screen0OpenMovie(PPCContext& ctx, uint8_t* base);
void Type7Screen0OpenMovie(PPCContext& ctx, uint8_t* base);
void Type6Screen0SetUpCar(PPCContext& ctx, uint8_t* base);
void Type7Screen0SetUpCar(PPCContext& ctx, uint8_t* base);
// Type6Screen0::OnDialogClosed sub_82234768 (r3 the screen, r4 the
// dispatcher), Type7Screen0::OnDialogClosed sub_82244B60 (r3 the screen)
// and ControllerOptions::Update sub_821C7F98.
void Type6Screen0OnDialogClosed(PPCContext& ctx, uint8_t* base);
void Type7Screen0OnDialogClosed(PPCContext& ctx, uint8_t* base);
void ControllerOptionsUpdate(PPCContext& ctx, uint8_t* base);
// The modes' WaitForLoads (r3 the answer, r4 the mode): Type6Mode
// sub_82235F28, Type7Mode sub_822463D8.
void Type6ModeWaitForLoads(PPCContext& ctx, uint8_t* base);
void Type7ModeWaitForLoads(PPCContext& ctx, uint8_t* base);

// Quaternion::FromAngles sub_8217C120 (r3 the destination, f1 to f3 the
// angles; r3 left) and GarageCamera::Reset sub_8217D888 (r3 the camera; r3
// after the camera + 192, as its FromAngles call leaves it).
void QuaternionFromAngles(PPCContext& ctx, uint8_t* base);
void GarageCameraReset(PPCContext& ctx, uint8_t* base);
// GarageLights::Init sub_8217D3C8 (r3 the lights; r3 left as the original
// leaves it: its frame + 96).
void GarageLightsInit(PPCContext& ctx, uint8_t* base);
// GarageCamera::Init sub_82180918 (r3 the camera; r3 after the presets'
// load's serial), GarageStage::Init sub_82180968 (r3 the stage, r4 its
// index) and StandSlot::Init sub_82180AA0 (r3 the slot, r4 its index): r3
// as GarageLights::Init leaves it where they reach it. They need the
// native heap and the archive's native queue (the adapter checks).
void GarageCameraInit(PPCContext& ctx, uint8_t* base);
// StandScene::PollTeardown sub_82179AE8 (no arguments; r3 0 or 1).
void StandScenePollTeardown(PPCContext& ctx, uint8_t* base);
// The modes' TearDown (r3 the answer, r4 the mode): Type6Mode sub_82235C18,
// Type7Mode sub_82246098.
void Type6ModeTearDown(PPCContext& ctx, uint8_t* base);
void Type7ModeTearDown(PPCContext& ctx, uint8_t* base);
// The first screens' Show (Type6 sub_82233C80, Type7 sub_822441D8; r3 the
// screen) and ShowCarInfo (Type6 sub_82233B70, Type7 sub_822440D8; no
// adapters: only Show calls them, natively): they need the UI movies' text
// setters native (Type67TextsNative).
void Type6Screen0Show(PPCContext& ctx, uint8_t* base);
void Type7Screen0Show(PPCContext& ctx, uint8_t* base);
void Type6Screen0ShowCarInfo(PPCContext& ctx, uint8_t* base);
void Type7Screen0ShowCarInfo(PPCContext& ctx, uint8_t* base);
// Type7Screen0::MenuPage sub_82244BF8 (r3 the screen, r4 the dispatcher).
void Type7Screen0MenuPage(PPCContext& ctx, uint8_t* base);
// The first screens' Update (Type6 sub_82234F08, Type7 sub_82245200; r3 the
// screen).
void Type6Screen0Update(PPCContext& ctx, uint8_t* base);
void Type7Screen0Update(PPCContext& ctx, uint8_t* base);
void GarageStageInit(PPCContext& ctx, uint8_t* base);
void StandSlotInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
