// The front end's car draw's register steps (src/front_end_car_draw/,
// docs/functions/front-end-car-draw.md), shared by the adapters
// (src/hooks/front_end_car_draw.cpp, behind --native_front_end_car_draw) and
// the harness (without the flag). Each runs the original unless
// FrontEndCarDrawCalleesNative(), leaves r3 as the original does and
// flush-to-zero off.

#pragma once

#include "front_end_car_draw/front_end_car_draw.h"
#include "hooks/native_entry.h"
#include "guest.h"

namespace hooks {

// Whether the plain C++'s callees run natively: --native_vehicle_accessors
// (ScenePools::GetB), --native_math (tan), --native_menu_sounds,
// --native_load_poll with its callees (SceneModel::PollLoad).
bool FrontEndCarDrawCalleesNative();
// The guest calls the plain C++ makes (front_end_car_draw::Calls::call).
front_end_car_draw::Calls FrontEndCarDrawCalls(lifted::Regs& ctx);

// Plates::Hide sub_820DF190 and Show sub_820DF180 (r3 the point block).
void FrontEndCarDrawPlatesHide(lifted::Regs& ctx);
void FrontEndCarDrawPlatesShow(lifted::Regs& ctx);
// GarageStage::IsBusy sub_82179BE0, SyncPlates sub_8217AFE8 (r3 the
// holder) and SetDrawWord sub_82178B40 (r3 the holder, r4 the word).
void FrontEndCarDrawIsBusy(lifted::Regs& ctx);
void FrontEndCarDrawSyncPlates(lifted::Regs& ctx);
void FrontEndCarDrawSetDrawWord(lifted::Regs& ctx);
// GarageStage::StepAngles sub_82179C88 (r4 the command, r5 the angles),
// GarageAnims::Add sub_8217F9E0 (r3 the list, r4 the type, r5 the value's
// address, r6 the index, r7 a word, f1 the steps), GarageStage::ApplyPreset
// sub_8217D2B0 (r4 the preset, r5 the stage, r6 the index) and Command
// sub_82180108 (r3 the holder, r4 the command).
void FrontEndCarDrawStepAngles(lifted::Regs& ctx);
void FrontEndCarDrawAnimsAdd(lifted::Regs& ctx);
void FrontEndCarDrawApplyPreset(lifted::Regs& ctx);
void FrontEndCarDrawCommand(lifted::Regs& ctx);
// GarageAnims::Turn sub_82179D78, Open sub_82179DF0 and Close sub_82179EA8
// (r3 the list, r4 an entry) and Run sub_8217D120 (r3 the list).
void FrontEndCarDrawAnimsTurn(lifted::Regs& ctx);
void FrontEndCarDrawAnimsOpen(lifted::Regs& ctx);
void FrontEndCarDrawAnimsClose(lifted::Regs& ctx);
void FrontEndCarDrawAnimsRun(lifted::Regs& ctx);
// SceneModel::ApplyLightKit sub_820D9D10 (r3 the scene model),
// GarageStage::SetLightKit sub_82179678 (r3 the holder, r4 the group, r5
// the word, r6 the index, f1 to f3) and Floor::Draw sub_820C8480 (r3 the
// scene, r4 the matrix, r5 the draw word; r24 handed to Model::DrawShape).
void FrontEndCarDrawApplyLightKit(lifted::Regs& ctx);
void FrontEndCarDrawSetLightKit(lifted::Regs& ctx);
// GarageStage::ClearLightKit sub_821795E0 (r3 the holder, r4 a group; no
// adapter: only the tuning screen's plain C++ reaches it; the harness's).
void FrontEndCarDrawClearLightKit(lifted::Regs& ctx);
void FrontEndCarDrawFloorDraw(lifted::Regs& ctx);
void FrontEndCarDrawFloorDraw(lifted::Regs& ctx, uint32_t caller_r24);
// GarageStage::SetLights sub_8217BA20 (r3 the stage's lights, r4 mirrored).
void FrontEndCarDrawSetLights(lifted::Regs& ctx);
// GarageStage::PoseCar sub_8217CAB8 (r3 the holder, r4 the scene model's
// block, r5 out; with --native_matrix_math, --native_accessories and
// --native_memcpy, else the original).
void FrontEndCarDrawPoseCar(lifted::Regs& ctx);
// GarageStage::Draw sub_8217FE60 (r3 the holder, r5 turn the car); r3 as
// CarScene::Draw leaves it. `caller_r24` (handed to Floor::Draw, a
// diagnostic word) isn't carried (the rewiring, stage 3): callers pass 0.
void FrontEndCarDrawDraw(lifted::Regs& ctx);
void FrontEndCarDrawDraw(lifted::Regs& ctx, uint32_t caller_r24);
// Whether GarageStage::Draw's adapter runs its plain C++ (its flag and
// FrontEndCarDrawCalleesNative()), and the callees' choices it makes
// (PoseCar's and Floor::Draw's adapters' checks, r24), for native callers.
bool FrontEndCarDrawDrawNative();
// `caller_r24`: the caller's r24 for the recorder's context's diagnostic
// word, not carried (the rewiring, stage 3): callers pass 0.
front_end_car_draw::DrawCallees FrontEndCarDrawDrawCallees(uint32_t caller_r24);

}  // namespace hooks
