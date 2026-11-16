// The second set's objects' register steps (src/second_set/,
// docs/functions/second-set.md), shared by the adapters
// (src/hooks/second_set.cpp, behind --native_second_set) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ can't (memcpy not native).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
#include "second_set/finds.h"
#include "second_set/objects.h"
#include "second_set/wheels.h"

namespace hooks {

// PointRecord::SetNode sub_820CE8D8 (no adapter: its callers are native;
// the harness's) and SetShape sub_820CE900: r3 = the record (left), r4 =
// the string.
void SecondSetPointSetNode(lifted::Regs& ctx, uint8_t* base);
void SecondSetPointSetShape(lifted::Regs& ctx, uint8_t* base);
// PointRecord::Clear sub_820CEA08 (r3 = the record; r3 = +80) and
// PointRecord::PointRecord sub_820D0270 (r3 = the record, left).
void SecondSetPointClear(lifted::Regs& ctx, uint8_t* base);
void SecondSetPointConstruct(lifted::Regs& ctx, uint8_t* base);
// ModelPoint::FindShape sub_820CEB30: r3 = the record, r4 = the part model;
// r3 = 1 or 0.
void SecondSetFindShape(lifted::Regs& ctx, uint8_t* base);

// The point groups' resets and inits (src/second_set/point_groups.h): r3 =
// the group or record; r3 as the original leaves it. ResetRightDoor
// natively only with --native_material_passes (else the original).
void SecondSetPartsResetFrontBumper(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetRearBumper(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetRearSpoiler(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetBonnet(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetLeftDoor(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetRightDoor(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetDoors(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetHeadlights(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetTailLights(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetFenders(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetCabin(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetEngine(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetMufflers(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsResetShadows(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsInitCooling(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsInitSideSkirts(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsInitLeftMirror(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsInitRightMirror(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsInitMirror(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitInitBonnet(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitInitMirror(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitResetFenders(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitResetCabin(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitResetDoors(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitResetHeadlights(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorInitBody(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorResetDials(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorResetMeters(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorResetSteering(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorResetSeats(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorResetRollCage(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightsResetFront(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightsResetSides(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightsResetRear(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficResetBody(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficResetHeadlights(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficResetTailLights(lifted::Regs& ctx, uint8_t* base);
// The point blocks' constructors and resets (src/second_set/blocks.h): r3 =
// the block; r3 as the original leaves it. The parts' natively only with
// --native_material_passes (else the original).
void SecondSetPartsPointsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsPointsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitPointsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitPointsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorPointsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorPointsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightPointsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightPointsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficPointsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficPointsReset(lifted::Regs& ctx, uint8_t* base);
// The wheels' group resets (point_groups.h) and the objects' resets
// (objects.h): r3 = the group or object; r3 as the original leaves it.
// SceneParts::Reset natively only with --native_material_passes,
// SceneCarModel::Release only where ScenePools::ReleaseA's plain C++ runs
// (hooks::SceneLoadersReleaseANative; else the original).
void SecondSetWheelsResetTire(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsResetRimA(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsResetRimB(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsResetBrakes(lifted::Regs& ctx, uint8_t* base);
void SecondSetScenePartsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCockpitReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneInteriorReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneLightsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneTrafficReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCarModelRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneModelBlockConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneModelBlockReset(lifted::Regs& ctx, uint8_t* base);
// The wheels' block and inits (blocks.h), SceneWheels::Reset, the releases
// (natively where ReleaseA's plain C++ runs, the parts' with
// --native_material_passes) and the constructors (r4 the list; the parts'
// with --native_material_passes) (objects.h): r3 as the original leaves it.
void SecondSetSceneWheelsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelPointsReset(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelPointsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsInitTire(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsInitRimA(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsInitRimB(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelsInitBrakes(lifted::Regs& ctx, uint8_t* base);
void SecondSetScenePartsRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneInteriorRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneLightsRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneWheelsRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneTrafficRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCarModelConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetScenePartsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCockpitConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneInteriorConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneLightsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneWheelsConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneTrafficConstruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneModelConstruct(lifted::Regs& ctx, uint8_t* base);
// The morph and the tire's helpers (morph.h): the shape accessors (r3 the
// model, r4 the shape; r3 the word), Morph::SetUp (r3 the model, r4 the
// record, r5 the base shape, r6 and r7 the indices and weights, r8 the
// count), Blend (r3 the record) and Run (r4 the record, r5 and r6 the
// outputs; the original where the plain C++ declines), sub_820E6F28 (r3 the
// entries, r4 an index, r5 a float's address, f1) and sub_820E7010 (r3 an
// index, r4 the entries, r5 to r7 the outputs); the flush mode as the
// originals leave it.
void SecondSetModelStreamData(lifted::Regs& ctx, uint8_t* base);
void SecondSetModelStreamSize(lifted::Regs& ctx, uint8_t* base);
void SecondSetModelStreamData2(lifted::Regs& ctx, uint8_t* base);
void SecondSetModelStreamSize2(lifted::Regs& ctx, uint8_t* base);
void SecondSetMorphSetUp(lifted::Regs& ctx, uint8_t* base);
void SecondSetMorphBlend(lifted::Regs& ctx, uint8_t* base);
void SecondSetMorphRun(lifted::Regs& ctx, uint8_t* base);
void SecondSetTireFillEntries(lifted::Regs& ctx, uint8_t* base);
void SecondSetTireReadEntry(lifted::Regs& ctx, uint8_t* base);
// ScenePools::ReleaseB sub_820C5C88 (r3 the set, r4 the pool, r5 the
// index; its adapter is the diagnostics' wrap, src/debug/frame_state_diff.cpp),
// SceneCockpit::Release sub_820DBF88 and SceneModel::Release sub_820DA5D0
// (no adapter: only ReleaseB calls it; the harness's) (objects.h): r3 the
// object; natively where SecondSetReleaseNative says (else the original), the
// nested ReleaseB's original where its plain C++ declines. r3 as the original
// leaves it.
bool SecondSetReleaseNative(uint32_t pool);
second_set::ReleaseCalls SecondSetReleaseCalls(lifted::Regs& ctx, uint8_t* base);
void SecondSetReleaseB(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCockpitRelease(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneModelRelease(lifted::Regs& ctx, uint8_t* base);
// SceneCockpit::~SceneCockpit sub_820DC2F8 (no adapter: only its deleting
// destructor calls it; the harness's) and the deleting destructor
// sub_820DC448 (r3 the object, left; r4 the flags, bit 0 to the original):
// natively where pool 2's release is (else the original).
void SecondSetSceneCockpitDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCockpitDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
// The destructors and deleting destructors (objects.h; r3 the object, left;
// r4 the flags), natively where their releases are (else the original), and
// the point blocks' destructors (blocks.h; r3 as the original leaves it).
void SecondSetSceneCarModelDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetScenePartsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneInteriorDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneLightsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneWheelsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneTrafficDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneCarModelDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetScenePartsDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneInteriorDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneLightsDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneWheelsDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetSceneTrafficDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetPoolObjectDeletingDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetPartsPointsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitPointsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorPointsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightPointsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelPointsDestruct(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficPointsDestruct(lifted::Regs& ctx, uint8_t* base);


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
// The guest calls the set-ups' plain C++ makes (Skeleton::UpdateMatrices'
// original from ModelPoint::Find's frame), for native callers.
second_set::FindCalls SecondSetFindCalls(lifted::Regs& ctx, uint8_t* base);
// One set-up's plain C++ (nothing done unless SecondSetFindsNative()), for
// the adapters and the harness.
void SecondSetFindStep(lifted::Regs& ctx, uint8_t* base, FindFunction find);
void SecondSetPartsFind(lifted::Regs& ctx, uint8_t* base);
void SecondSetCockpitFind(lifted::Regs& ctx, uint8_t* base);
void SecondSetInteriorFind(lifted::Regs& ctx, uint8_t* base);
void SecondSetLightsFind(lifted::Regs& ctx, uint8_t* base);
void SecondSetTrafficFind(lifted::Regs& ctx, uint8_t* base);


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
second_set::WheelCalls SecondSetWheelCalls(lifted::Regs& ctx, uint8_t* base);
void SecondSetWheelStep(lifted::Regs& ctx, uint8_t* base, WheelFunction step);
void SecondSetWheelPointsSetUp(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
