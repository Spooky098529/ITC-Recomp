// The car draw's register steps (src/car_draw/, docs/functions/car-draw.md),
// shared by the adapters (src/hooks/car_draw.cpp, behind --native_car_draw;
// SceneModel::IsNearPlayerHeight's in the diagnostics' wrap,
// src/debug/frame_state_diff.cpp, and BlinkTimers::Advance's in its
// render-only skip, src/frame_pacing/per_frame_logic.cpp) and the harness
// (without the flag). Each leaves r3 as the original
// does, and runs the original where the plain C++ declines or its callees'
// plain C++ isn't native.

#pragma once

#include "guest.h"

#include "car_draw/car_draw.h"
#include "hooks/scenery_lists.h"

namespace hooks {

// Whether the callees' plain C++ runs natively: the model draws'
// (--native_scenery_lists, --native_models, the command list's diagnostics
// off), the lighting's (--native_matrix_math, --native_math,
// --native_nearest_lights) and the height cull's (--native_vehicle_accessors,
// --native_track_zones, --native_matrix_math). CarDrawCalleesNative is all
// of them, for the native callers of these functions.
bool CarDrawModelCalleesNative();
bool CarDrawLightingCalleesNative();
bool CarDrawHeightCalleesNative();
bool CarDrawCalleesNative();
// The morph's (the command list's diagnostics off, --native_second_set,
// --native_models, --native_c_runtime) and the light kit's (the model
// draws', --native_material_passes, --native_second_set,
// --native_matrix_math).
bool CarDrawMorphCalleesNative();
bool CarDrawLightKitCalleesNative();
// The parts' small draws' (the model draws', --native_material_passes,
// --native_matrix_math).
bool CarDrawPartsCalleesNative();

// The guest calls (car_draw::Calls) through `guest`'s context: the draws'
// (hooks::SceneryListsDrawCalls) and memcpy's original.
car_draw::Calls CarDrawCalls(SceneryDrawGuest* guest);

// Whether BlinkTimers::Advance is held on this pass (a render-only pass with
// --per_tick_blink_timers): its native callers skip it then, as its adapter
// does.
bool CarDrawBlinkTimersHeld();

// CarPart::Draw (sub_820D06F0): r3 = the point record, r4, r5, r6, r7.
void CarDrawCarPartDraw(PPCContext& ctx, uint8_t* base);
// BlinkTimers::Advance (sub_820DC958): r3 = the point block (no skip here).
void CarDrawBlinkTimersAdvance(PPCContext& ctx, uint8_t* base);
// SceneModel::IsNearPlayerHeight (sub_820DAB58): r3 = the pass
// description, r4 = the position.
void CarDrawIsNearPlayerHeight(PPCContext& ctx, uint8_t* base);

// Model::MorphShape (sub_82595110): r3 = the model, r4 = the morph record,
// r5 = the shape's index.
void CarDrawMorphShape(PPCContext& ctx, uint8_t* base);
// SceneLights::Draw (sub_820DDE58): r3 = the light kit, r4 = the block
// (inside the diagnostics' wrap, src/debug/frame_state_diff.cpp; it skips
// BlinkTimers::Advance as CarDrawBlinkTimersHeld says).
void CarDrawSceneLightsDraw(PPCContext& ctx, uint8_t* base);

// The shadows' callees' plain C++ native (the parts' and the lighting's).
bool CarDrawShadowsCalleesNative();
// The parts' top, SceneParts::Draw (sub_820E2EE8; inside the diagnostics'
// wrap, src/debug/frame_state_diff.cpp; the shadows' callees native): r3 =
// the parts object, r4 = the block.
void CarDrawSceneParts(PPCContext& ctx, uint8_t* base);

// The interior's callees' plain C++ native (the parts' draws').
bool CarDrawInteriorCalleesNative();
// The interior's top, SceneInterior::Draw (sub_820E53F0; inside the
// diagnostics' wrap, src/debug/frame_state_diff.cpp; the interior's callees
// native, with --native_accessories and --native_scene_models for the
// accessory): r3 = the interior, r4 = the block.
void CarDrawSceneInterior(PPCContext& ctx, uint8_t* base);

// The cockpit's callees' plain C++ native (the interior's with
// --native_accessories and --native_scene_models, and the lighting's).
bool CarDrawCockpitCalleesNative();
// The cockpit's top, SceneCockpit::Draw (sub_820DC498; inside the
// diagnostics' wrap, src/debug/frame_state_diff.cpp; the cockpit's callees
// native): r3 = the cockpit, r4 = the block.
void CarDrawSceneCockpit(PPCContext& ctx, uint8_t* base);

// The traffic body's top, SceneTraffic::Draw (sub_820E6A40; inside the
// diagnostics' wrap, src/debug/frame_state_diff.cpp; the shadows' callees
// native): r3 = the traffic body, r4 = the block.
void CarDrawSceneTraffic(PPCContext& ctx, uint8_t* base);

// The wheels' callees' plain C++ native (the parts' and the morph's).
bool CarDrawWheelsCalleesNative();
// WheelPoints::DrawTire (sub_820E8AE8; the wheels' callees native): r3 = the
// tire group, r4 = the camera pass, r5 = the block.
void CarDrawWheelsDrawTire(PPCContext& ctx, uint8_t* base);
// WheelPoints::DrawTireMorphed (sub_820E9318; the wheels' callees native):
// r3 = the tire group, r4 = the camera pass, r5 = the block (r28 its first
// draw's).
void CarDrawWheelsDrawTireMorphed(PPCContext& ctx, uint8_t* base);
// WheelPoints::DrawRimA (sub_820E9A28; the wheels' callees native): r3 = the
// rim group, r4 = the camera pass, r5 = the block (r24 its first draws').
void CarDrawWheelsDrawRimA(PPCContext& ctx, uint8_t* base);
// WheelPoints::DrawRimAMorphed (sub_820EA148): as DrawRimA's.
void CarDrawWheelsDrawRimAMorphed(PPCContext& ctx, uint8_t* base);
// WheelPoints::DrawRimB (sub_820EA950; the wheels' callees native): r3 = the
// rim group, r4 = the camera pass, r5 = the block.
void CarDrawWheelsDrawRimB(PPCContext& ctx, uint8_t* base);
// WheelPoints::DrawBrakes (sub_820EB388; the wheels' callees native): r3 =
// the brake group, r4 = the camera pass, r5 = the block (r24 its draws').
void CarDrawWheelsDrawBrakes(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
