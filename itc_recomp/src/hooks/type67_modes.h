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

}  // namespace hooks
