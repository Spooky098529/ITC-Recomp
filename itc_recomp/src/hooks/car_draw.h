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

// Model::Draw (sub_820D04E8): r3 = the model, r4 = the view-projection, r5 =
// the world matrix, r6 = the shape, r7 = the depth entry too.
void CarDrawModelDraw(PPCContext& ctx, uint8_t* base);
// CarPart::Draw (sub_820D06F0): r3 = the point record, r4, r5, r6, r7.
void CarDrawCarPartDraw(PPCContext& ctx, uint8_t* base);
// CarLighting::Gather (sub_820C9170): r3 = the record, r4 = the world
// matrix, r5 = the flag.
void CarDrawCarLightingGather(PPCContext& ctx, uint8_t* base);
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

// The head and tail lights (src/car_draw/lamps.cpp): r3 = the group, r4 =
// the block, r5 = the camera pass (the headlights' r6 and r7 the color
// table's index and sub-index).
void CarDrawPartsTailLights(PPCContext& ctx, uint8_t* base);         // sub_820DF8B0
void CarDrawPartsHeadlights(PPCContext& ctx, uint8_t* base);         // sub_820E1690
// The shadows (src/car_draw/shadows.cpp; the parts' and the lighting's
// callees native): r3 = the group, r4 = the block.
bool CarDrawShadowsCalleesNative();
void CarDrawPartsShadows(PPCContext& ctx, uint8_t* base);            // sub_820E1D88
// The parts' top, SceneParts::Draw (sub_820E2EE8; inside the diagnostics'
// wrap, src/debug/frame_state_diff.cpp; the shadows' callees native): r3 =
// the parts object, r4 = the block.
void CarDrawSceneParts(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
