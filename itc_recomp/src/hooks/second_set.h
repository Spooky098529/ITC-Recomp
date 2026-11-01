// The second set's objects' register steps (src/second_set/,
// docs/functions/second-set.md), shared by the adapters
// (src/hooks/second_set.cpp, behind --native_second_set) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ can't (memcpy not native).

#pragma once

#include "guest.h"
#include "second_set/finds.h"
#include "second_set/objects.h"
#include "second_set/wheels.h"

namespace hooks {

// PointRecord::SetNode sub_820CE8D8 (no adapter: its callers are native;
// the harness's) and SetShape sub_820CE900: r3 = the record (left), r4 =
// the string.
void SecondSetPointSetNode(PPCContext& ctx, uint8_t* base);
void SecondSetPointSetShape(PPCContext& ctx, uint8_t* base);
// PointRecord::Clear sub_820CEA08 (r3 = the record; r3 = +80) and
// PointRecord::PointRecord sub_820D0270 (r3 = the record, left).
void SecondSetPointClear(PPCContext& ctx, uint8_t* base);
void SecondSetPointConstruct(PPCContext& ctx, uint8_t* base);
// ModelPoint::FindShape sub_820CEB30: r3 = the record, r4 = the part model;
// r3 = 1 or 0.
void SecondSetFindShape(PPCContext& ctx, uint8_t* base);

// The point groups' resets and inits (src/second_set/point_groups.h): r3 =
// the group or record; r3 as the original leaves it. ResetRightDoor
// natively only with --native_material_passes (else the original).
void SecondSetPartsResetFrontBumper(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetRearBumper(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetRearSpoiler(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetBonnet(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetLeftDoor(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetRightDoor(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetDoors(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetHeadlights(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetTailLights(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetFenders(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetCabin(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetEngine(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetMufflers(PPCContext& ctx, uint8_t* base);
void SecondSetPartsResetShadows(PPCContext& ctx, uint8_t* base);
void SecondSetPartsInitCooling(PPCContext& ctx, uint8_t* base);
void SecondSetPartsInitSideSkirts(PPCContext& ctx, uint8_t* base);
void SecondSetPartsInitLeftMirror(PPCContext& ctx, uint8_t* base);
void SecondSetPartsInitRightMirror(PPCContext& ctx, uint8_t* base);
void SecondSetPartsInitMirror(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitInitBonnet(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitInitMirror(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitResetFenders(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitResetCabin(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitResetDoors(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitResetHeadlights(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorInitBody(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorResetDials(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorResetMeters(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorResetSteering(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorResetSeats(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorResetRollCage(PPCContext& ctx, uint8_t* base);
void SecondSetLightsResetFront(PPCContext& ctx, uint8_t* base);
void SecondSetLightsResetSides(PPCContext& ctx, uint8_t* base);
void SecondSetLightsResetRear(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficResetBody(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficResetHeadlights(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficResetTailLights(PPCContext& ctx, uint8_t* base);
// The point blocks' constructors and resets (src/second_set/blocks.h): r3 =
// the block; r3 as the original leaves it. The parts' natively only with
// --native_material_passes (else the original).
void SecondSetPartsPointsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetPartsPointsReset(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitPointsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitPointsReset(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorPointsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorPointsReset(PPCContext& ctx, uint8_t* base);
void SecondSetLightPointsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetLightPointsReset(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficPointsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficPointsReset(PPCContext& ctx, uint8_t* base);
// The wheels' group resets (point_groups.h) and the objects' resets
// (objects.h): r3 = the group or object; r3 as the original leaves it.
// SceneParts::Reset natively only with --native_material_passes,
// SceneCarModel::Release only where ScenePools::ReleaseA's plain C++ runs
// (hooks::SceneLoadersReleaseANative; else the original).
void SecondSetWheelsResetTire(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsResetRimA(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsResetRimB(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsResetBrakes(PPCContext& ctx, uint8_t* base);
void SecondSetScenePartsReset(PPCContext& ctx, uint8_t* base);
void SecondSetSceneCockpitReset(PPCContext& ctx, uint8_t* base);
void SecondSetSceneInteriorReset(PPCContext& ctx, uint8_t* base);
void SecondSetSceneLightsReset(PPCContext& ctx, uint8_t* base);
void SecondSetSceneTrafficReset(PPCContext& ctx, uint8_t* base);
void SecondSetSceneCarModelRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneModelBlockConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneModelBlockReset(PPCContext& ctx, uint8_t* base);
// The wheels' block and inits (blocks.h), SceneWheels::Reset, the releases
// (natively where ReleaseA's plain C++ runs, the parts' with
// --native_material_passes) and the constructors (r4 the list; the parts'
// with --native_material_passes) (objects.h): r3 as the original leaves it.
void SecondSetSceneWheelsReset(PPCContext& ctx, uint8_t* base);
void SecondSetWheelPointsReset(PPCContext& ctx, uint8_t* base);
void SecondSetWheelPointsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsInitTire(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsInitRimA(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsInitRimB(PPCContext& ctx, uint8_t* base);
void SecondSetWheelsInitBrakes(PPCContext& ctx, uint8_t* base);
void SecondSetScenePartsRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneInteriorRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneLightsRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneWheelsRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneTrafficRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneCarModelConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetScenePartsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneCockpitConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneInteriorConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneLightsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneWheelsConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneTrafficConstruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneModelConstruct(PPCContext& ctx, uint8_t* base);
// The morph and the tire's helpers (morph.h): the shape accessors (r3 the
// model, r4 the shape; r3 the word), Morph::SetUp (r3 the model, r4 the
// record, r5 the base shape, r6 and r7 the indices and weights, r8 the
// count), Blend (r3 the record) and Run (r4 the record, r5 and r6 the
// outputs; the original where the plain C++ declines), sub_820E6F28 (r3 the
// entries, r4 an index, r5 a float's address, f1) and sub_820E7010 (r3 an
// index, r4 the entries, r5 to r7 the outputs); the flush mode as the
// originals leave it.
void SecondSetModelStreamData(PPCContext& ctx, uint8_t* base);
void SecondSetModelStreamSize(PPCContext& ctx, uint8_t* base);
void SecondSetModelStreamData2(PPCContext& ctx, uint8_t* base);
void SecondSetModelStreamSize2(PPCContext& ctx, uint8_t* base);
void SecondSetMorphSetUp(PPCContext& ctx, uint8_t* base);
void SecondSetMorphBlend(PPCContext& ctx, uint8_t* base);
void SecondSetMorphRun(PPCContext& ctx, uint8_t* base);
void SecondSetTireFillEntries(PPCContext& ctx, uint8_t* base);
void SecondSetTireReadEntry(PPCContext& ctx, uint8_t* base);
// ScenePools::ReleaseB sub_820C5C88 (r3 the set, r4 the pool, r5 the
// index; its adapter is the diagnostics' wrap, src/debug/frame_state_diff.cpp),
// SceneCockpit::Release sub_820DBF88 and SceneModel::Release sub_820DA5D0
// (no adapter: only ReleaseB calls it; the harness's) (objects.h): r3 the
// object; natively where SecondSetReleaseNative says (else the original), the
// nested ReleaseB's original where its plain C++ declines. r3 as the original
// leaves it.
bool SecondSetReleaseNative(uint32_t pool);
second_set::ReleaseCalls SecondSetReleaseCalls(PPCContext& ctx, uint8_t* base);
void SecondSetReleaseB(PPCContext& ctx, uint8_t* base);
void SecondSetSceneCockpitRelease(PPCContext& ctx, uint8_t* base);
void SecondSetSceneModelRelease(PPCContext& ctx, uint8_t* base);
// The destructors and deleting destructors (objects.h; r3 the object, left;
// r4 the flags), natively where their releases are (else the original), and
// the point blocks' destructors (blocks.h; r3 as the original leaves it).
void SecondSetSceneCarModelDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetScenePartsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneInteriorDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneLightsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneWheelsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneTrafficDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneCarModelDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetScenePartsDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneInteriorDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneLightsDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneWheelsDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetSceneTrafficDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetPoolObjectDeletingDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetPartsPointsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitPointsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorPointsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetLightPointsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetWheelPointsDestruct(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficPointsDestruct(PPCContext& ctx, uint8_t* base);


// The point set-ups (src/second_set/finds.h): r3 = the group or block, r4 =
// the part model or the models' words; r3 as the original leaves it, the
// flush mode as its last float step. PartsPoints::Find sub_820E0258 and
// CockpitPoints::Find sub_820DBC88, InteriorPoints::Find sub_820E3660,
// LightPoints::Find sub_820DC900 (r4 one part model) and TrafficPoints::Find
// sub_820E58C8 natively only with native memcpy,
// --native_models and --native_material_passes (else the original).
using FindFunction = uint32_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t,
                                  second_set::FindCalls&);
bool SecondSetFindsNative();
// One set-up's plain C++ (nothing done unless SecondSetFindsNative()), for
// the adapters and the harness.
void SecondSetFindStep(PPCContext& ctx, uint8_t* base, FindFunction find);
void SecondSetPartsFind(PPCContext& ctx, uint8_t* base);
void SecondSetCockpitFind(PPCContext& ctx, uint8_t* base);
void SecondSetInteriorFind(PPCContext& ctx, uint8_t* base);
void SecondSetLightsFind(PPCContext& ctx, uint8_t* base);
void SecondSetTrafficFind(PPCContext& ctx, uint8_t* base);


// The wheels' set-up (src/second_set/wheels.h): WheelPoints::SetUp
// sub_820E8668 (r3 = the block) natively only with native memcpy, the native
// heap and --native_material_passes (else the original); SetUpTire
// sub_820E7598, SetUpRimA sub_820E79C8, SetUpRimB sub_820E7CA0 and
// SetUpBrakes sub_820E8028 have no adapters (only SetUp calls them): their
// step (r3 the group, r4 the model) for the harness. r3 as the original
// leaves it, the flush mode as its last float step; the originals of
// Morph::Run, memcpy and FreePagesIn where their plain C++ declines.
using WheelFunction = uint64_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t, second_set::WheelCalls&);
bool SecondSetWheelsNative();
second_set::WheelCalls SecondSetWheelCalls(PPCContext& ctx, uint8_t* base);
void SecondSetWheelStep(PPCContext& ctx, uint8_t* base, WheelFunction step);
void SecondSetWheelPointsSetUp(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
