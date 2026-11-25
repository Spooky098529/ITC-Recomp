// The scene models' load poll's register steps (src/load_poll/,
// docs/functions/load-poll.md), shared by the adapters
// (src/hooks/load_poll.cpp and the diagnostics' wraps in
// src/debug/frame_state_diff.cpp, behind --native_load_poll) and the
// harness (without the flag). Each runs the original unless
// LoadPollCalleesNative(), leaves r3 as the original does and the flush mode
// as the original's last float step.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "load_poll/load_poll.h"

namespace hooks {

// Whether the plain C++'s callees run natively: --native_second_set,
// --native_scene_loaders, --native_car_models and the point set-ups' checks
// (native memcpy, --native_models, --native_material_passes) and the wheels'
// (native memcpy, --native_heap, --native_material_passes).
bool LoadPollCalleesNative();
// The guest calls the plain C++ makes where a callee's plain C++ declines.
load_poll::Calls LoadPollCalls(lifted::Regs& ctx);

// Wheels::SetRecord sub_820D8EC0 (r3 the wheels, r4 the record) and
// SetCaliperTexture sub_820D8ED0 (r3 the wheels; no adapter: its caller in
// play is native).
void LoadPollWheelsSetRecord(lifted::Regs& ctx);
void LoadPollWheelsSetCaliperTexture(lifted::Regs& ctx);
// CarModel::IsReady sub_820D4048 (r3 the car model; a wrap's) and
// CarParts::SetVinylTextures sub_820D72D8 (r3 the parts; a wrap's).
void LoadPollCarModelIsReady(lifted::Regs& ctx);
void LoadPollCarPartsSetVinylTextures(lifted::Regs& ctx);
// InteriorDials::SetBlock sub_820E3040 (r3 the dials, r4 the block; no
// adapter: its caller in play is native),
// InteriorMeters::SetGauge sub_820E32E8 (no adapter: only SetGauges calls
// it; r3 the meters, r4 the gauge, r5 the entry) and SceneInterior::SetGauges
// sub_820E3938 (r3 the interior).
void LoadPollInteriorDialsSetBlock(lifted::Regs& ctx);
void LoadPollInteriorMetersSetGauge(lifted::Regs& ctx);
void LoadPollSceneInteriorSetGauges(lifted::Regs& ctx);
// SceneLights::Finish sub_820DDA60 (r3 the light kit; a wrap's),
// SetRecord sub_820DCB70 (no adapter: only ApplyRecord calls it; r4 the
// record) and ApplyRecord sub_820DCBC0 (r4 the record; a wrap's);
// SceneTraffic::Finish sub_820E5A68 (r3 the traffic body; a wrap's).
void LoadPollSceneLightsFinish(lifted::Regs& ctx);
void LoadPollSceneLightsSetRecord(lifted::Regs& ctx);
void LoadPollSceneLightsApplyRecord(lifted::Regs& ctx);
void LoadPollSceneTrafficFinish(lifted::Regs& ctx);

// The paint (r4 to r9 the colors; the wheels' r4 to r10 and the caller's
// frame): SceneModel::ApplyPaint sub_820D9B68 (r3 the scene model; no wrap
// left: its native callers call it) and ApplyWheelPaint sub_820D9C50 (r3
// the scene model; the wrap's); no adapters, only native
// callers call them (the harness's): SceneParts::ApplyPaint sub_820E0908,
// SceneInterior::ApplyPaint sub_820E4250, SceneCockpit::ApplyPaint
// sub_820DC138, SceneTraffic::ApplyPaint sub_820E61D0, TrafficCar::SetPaint
// sub_820D7E18, SceneWheels::ApplyPaint sub_820E8A00, Wheels::ApplyPaint
// sub_820D91E0.
void LoadPollSceneModelApplyPaint(lifted::Regs& ctx);
void LoadPollSceneModelApplyWheelPaint(lifted::Regs& ctx);
void LoadPollScenePartsApplyPaint(lifted::Regs& ctx);
void LoadPollSceneInteriorApplyPaint(lifted::Regs& ctx);
void LoadPollSceneCockpitApplyPaint(lifted::Regs& ctx);
void LoadPollSceneTrafficApplyPaint(lifted::Regs& ctx);
void LoadPollTrafficCarSetPaint(lifted::Regs& ctx);
void LoadPollSceneWheelsApplyPaint(lifted::Regs& ctx);
void LoadPollWheelsApplyPaint(lifted::Regs& ctx);
// SceneInterior::Finish sub_820E4140 and SceneWheels::SetUpPoints
// sub_820E8908 (r3 the object; the wraps'), SceneParts::SetSelectedTexture
// sub_820DF250 (no adapter: only Build calls it) and Build sub_820E05E0 (r3
// the parts object).
void LoadPollSceneInteriorFinish(lifted::Regs& ctx);
void LoadPollSceneWheelsSetUpPoints(lifted::Regs& ctx);
void LoadPollScenePartsSetSelectedTexture(lifted::Regs& ctx);
void LoadPollScenePartsBuild(lifted::Regs& ctx);
// Wheels::BindTextures sub_820D9638 (no adapter: only SceneWheels::Finish
// calls it), SceneWheels::Finish sub_820E7328, Wheels::ApplyPalette
// sub_820D92A0, SceneParts::Finish sub_820E2E68 and SceneCockpit::Finish
// sub_820DBFF8 (r3 the object; the wraps').
void LoadPollWheelsBindTextures(lifted::Regs& ctx);
void LoadPollSceneWheelsFinish(lifted::Regs& ctx);
void LoadPollWheelsApplyPalette(lifted::Regs& ctx);
void LoadPollScenePartsFinish(lifted::Regs& ctx);
void LoadPollSceneCockpitFinish(lifted::Regs& ctx);
// SceneModel::ApplyPalettes sub_820D9EA0 (r3 the scene model; the wrap's)
// and SceneModel::PollLoad sub_820DA318 (r3 the scene model; r3 = 0 or 1).
void LoadPollSceneModelApplyPalettes(lifted::Regs& ctx);
void LoadPollSceneModelPollLoad(lifted::Regs& ctx);

}  // namespace hooks
