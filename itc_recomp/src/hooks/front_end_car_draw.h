// The front end's car draw's register steps (src/front_end_car_draw/,
// docs/functions/front-end-car-draw.md), shared by the adapters
// (src/hooks/front_end_car_draw.cpp, behind --native_front_end_car_draw) and
// the harness (without the flag). Each runs the original unless
// FrontEndCarDrawCalleesNative(), leaves r3 as the original does and
// flush-to-zero off.

#pragma once

#include "front_end_car_draw/front_end_car_draw.h"
#include "guest.h"

namespace hooks {

// Whether the plain C++'s callees run natively: --native_vehicle_accessors
// (ScenePools::GetB), --native_math (tan), --native_menu_sounds,
// --native_load_poll with its callees (SceneModel::PollLoad).
bool FrontEndCarDrawCalleesNative();
// The guest calls the plain C++ makes (front_end_car_draw::Calls::call).
front_end_car_draw::Calls FrontEndCarDrawCalls(PPCContext& ctx, uint8_t* base);

// Plates::Hide sub_820DF190 and Show sub_820DF180 (r3 the point block).
void FrontEndCarDrawPlatesHide(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawPlatesShow(PPCContext& ctx, uint8_t* base);
// GarageStage::IsBusy sub_82179BE0, SyncPlates sub_8217AFE8 (r3 the
// holder) and SetDrawWord sub_82178B40 (r3 the holder, r4 the word).
void FrontEndCarDrawIsBusy(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawSyncPlates(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawSetDrawWord(PPCContext& ctx, uint8_t* base);
// GarageStage::StepAngles sub_82179C88 (r4 the command, r5 the angles),
// GarageAnims::Add sub_8217F9E0 (r3 the list, r4 the type, r5 the value's
// address, r6 the index, r7 a word, f1 the steps), GarageStage::ApplyPreset
// sub_8217D2B0 (r4 the preset, r5 the stage, r6 the index) and Command
// sub_82180108 (r3 the holder, r4 the command).
void FrontEndCarDrawStepAngles(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawAnimsAdd(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawApplyPreset(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawCommand(PPCContext& ctx, uint8_t* base);
// GarageAnims::Turn sub_82179D78, Open sub_82179DF0 and Close sub_82179EA8
// (r3 the list, r4 an entry) and Run sub_8217D120 (r3 the list).
void FrontEndCarDrawAnimsTurn(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawAnimsOpen(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawAnimsClose(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawAnimsRun(PPCContext& ctx, uint8_t* base);
// SceneModel::ApplyLightKit sub_820D9D10 (r3 the scene model),
// GarageStage::SetLightKit sub_82179678 (r3 the holder, r4 the group, r5
// the word, r6 the index, f1 to f3) and Floor::Draw sub_820C8480 (r3 the
// scene, r4 the matrix, r5 the draw word; r24 handed to Model::DrawShape).
void FrontEndCarDrawApplyLightKit(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawSetLightKit(PPCContext& ctx, uint8_t* base);
void FrontEndCarDrawFloorDraw(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
