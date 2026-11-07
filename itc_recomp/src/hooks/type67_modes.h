// The type 6 and 7 modes' register steps (src/type67_modes/,
// docs/functions/type6-type7-modes.md), shared by the adapters
// (src/hooks/type67_modes.cpp, behind --native_type67_modes) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

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

}  // namespace hooks
