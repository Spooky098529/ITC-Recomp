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
void SecondSetPointSetNode(lifted::Regs& ctx);
void SecondSetPointSetShape(lifted::Regs& ctx);
// PointRecord::Clear sub_820CEA08 (r3 = the record; r3 = +80) and
// PointRecord::PointRecord sub_820D0270 (r3 = the record, left).
void SecondSetPointClear(lifted::Regs& ctx);
void SecondSetPointConstruct(lifted::Regs& ctx);
// ModelPoint::FindShape sub_820CEB30: r3 = the record, r4 = the part model;
// r3 = 1 or 0.
void SecondSetFindShape(lifted::Regs& ctx);

// The point groups' resets and inits (src/second_set/point_groups.h): r3 =
// the group or record; r3 as the original leaves it. ResetRightDoor
// natively only with --native_material_passes (else the original).
void SecondSetPartsResetFrontBumper(lifted::Regs& ctx);
void SecondSetPartsResetRearBumper(lifted::Regs& ctx);
void SecondSetPartsResetRearSpoiler(lifted::Regs& ctx);
void SecondSetPartsResetBonnet(lifted::Regs& ctx);
void SecondSetPartsResetLeftDoor(lifted::Regs& ctx);
void SecondSetPartsResetRightDoor(lifted::Regs& ctx);
void SecondSetPartsResetDoors(lifted::Regs& ctx);
void SecondSetPartsResetHeadlights(lifted::Regs& ctx);
void SecondSetPartsResetTailLights(lifted::Regs& ctx);
void SecondSetPartsResetFenders(lifted::Regs& ctx);
void SecondSetPartsResetCabin(lifted::Regs& ctx);
void SecondSetPartsResetEngine(lifted::Regs& ctx);
void SecondSetPartsResetMufflers(lifted::Regs& ctx);
void SecondSetPartsResetShadows(lifted::Regs& ctx);
void SecondSetPartsInitCooling(lifted::Regs& ctx);
void SecondSetPartsInitSideSkirts(lifted::Regs& ctx);
void SecondSetPartsInitLeftMirror(lifted::Regs& ctx);
void SecondSetPartsInitRightMirror(lifted::Regs& ctx);
void SecondSetPartsInitMirror(lifted::Regs& ctx);
void SecondSetCockpitInitBonnet(lifted::Regs& ctx);
void SecondSetCockpitInitMirror(lifted::Regs& ctx);
void SecondSetCockpitResetFenders(lifted::Regs& ctx);
void SecondSetCockpitResetCabin(lifted::Regs& ctx);
void SecondSetCockpitResetDoors(lifted::Regs& ctx);
void SecondSetCockpitResetHeadlights(lifted::Regs& ctx);
void SecondSetInteriorInitBody(lifted::Regs& ctx);
void SecondSetInteriorResetDials(lifted::Regs& ctx);
void SecondSetInteriorResetMeters(lifted::Regs& ctx);
void SecondSetInteriorResetSteering(lifted::Regs& ctx);
void SecondSetInteriorResetSeats(lifted::Regs& ctx);
void SecondSetInteriorResetRollCage(lifted::Regs& ctx);
void SecondSetLightsResetFront(lifted::Regs& ctx);
void SecondSetLightsResetSides(lifted::Regs& ctx);
void SecondSetLightsResetRear(lifted::Regs& ctx);
void SecondSetTrafficResetBody(lifted::Regs& ctx);
void SecondSetTrafficResetHeadlights(lifted::Regs& ctx);
void SecondSetTrafficResetTailLights(lifted::Regs& ctx);
// The point blocks' constructors and resets (src/second_set/blocks.h): r3 =
// the block; r3 as the original leaves it. The parts' natively only with
// --native_material_passes (else the original).
void SecondSetPartsPointsConstruct(lifted::Regs& ctx);
void SecondSetPartsPointsReset(lifted::Regs& ctx);
void SecondSetCockpitPointsConstruct(lifted::Regs& ctx);
void SecondSetCockpitPointsReset(lifted::Regs& ctx);
void SecondSetInteriorPointsConstruct(lifted::Regs& ctx);
void SecondSetInteriorPointsReset(lifted::Regs& ctx);
void SecondSetLightPointsConstruct(lifted::Regs& ctx);
void SecondSetLightPointsReset(lifted::Regs& ctx);
void SecondSetTrafficPointsConstruct(lifted::Regs& ctx);
void SecondSetTrafficPointsReset(lifted::Regs& ctx);
// The wheels' group resets (point_groups.h) and the objects' resets
// (objects.h): r3 = the group or object; r3 as the original leaves it.
// SceneParts::Reset natively only with --native_material_passes,
// SceneCarModel::Release only where ScenePools::ReleaseA's plain C++ runs
// (hooks::SceneLoadersReleaseANative; else the original).
void SecondSetWheelsResetTire(lifted::Regs& ctx);
void SecondSetWheelsResetRimA(lifted::Regs& ctx);
void SecondSetWheelsResetRimB(lifted::Regs& ctx);
void SecondSetWheelsResetBrakes(lifted::Regs& ctx);
void SecondSetScenePartsReset(lifted::Regs& ctx);
void SecondSetSceneCockpitReset(lifted::Regs& ctx);
void SecondSetSceneInteriorReset(lifted::Regs& ctx);
void SecondSetSceneLightsReset(lifted::Regs& ctx);
void SecondSetSceneTrafficReset(lifted::Regs& ctx);
void SecondSetSceneCarModelRelease(lifted::Regs& ctx);
void SecondSetSceneModelBlockConstruct(lifted::Regs& ctx);
void SecondSetSceneModelBlockReset(lifted::Regs& ctx);
// The wheels' block and inits (blocks.h), SceneWheels::Reset, the releases
// (natively where ReleaseA's plain C++ runs, the parts' with
// --native_material_passes) and the constructors (r4 the list; the parts'
// with --native_material_passes) (objects.h): r3 as the original leaves it.
void SecondSetSceneWheelsReset(lifted::Regs& ctx);
void SecondSetWheelPointsReset(lifted::Regs& ctx);
void SecondSetWheelPointsConstruct(lifted::Regs& ctx);
void SecondSetWheelsInitTire(lifted::Regs& ctx);
void SecondSetWheelsInitRimA(lifted::Regs& ctx);
void SecondSetWheelsInitRimB(lifted::Regs& ctx);
void SecondSetWheelsInitBrakes(lifted::Regs& ctx);
void SecondSetScenePartsRelease(lifted::Regs& ctx);
void SecondSetSceneInteriorRelease(lifted::Regs& ctx);
void SecondSetSceneLightsRelease(lifted::Regs& ctx);
void SecondSetSceneWheelsRelease(lifted::Regs& ctx);
void SecondSetSceneTrafficRelease(lifted::Regs& ctx);
void SecondSetSceneCarModelConstruct(lifted::Regs& ctx);
void SecondSetScenePartsConstruct(lifted::Regs& ctx);
void SecondSetSceneCockpitConstruct(lifted::Regs& ctx);
void SecondSetSceneInteriorConstruct(lifted::Regs& ctx);
void SecondSetSceneLightsConstruct(lifted::Regs& ctx);
void SecondSetSceneWheelsConstruct(lifted::Regs& ctx);
void SecondSetSceneTrafficConstruct(lifted::Regs& ctx);
void SecondSetSceneModelConstruct(lifted::Regs& ctx);
// The morph and the tire's helpers (morph.h): the shape accessors (r3 the
// model, r4 the shape; r3 the word), Morph::SetUp (r3 the model, r4 the
// record, r5 the base shape, r6 and r7 the indices and weights, r8 the
// count), Blend (r3 the record) and Run (r4 the record, r5 and r6 the
// outputs; the original where the plain C++ declines), sub_820E6F28 (r3 the
// entries, r4 an index, r5 a float's address, f1) and sub_820E7010 (r3 an
// index, r4 the entries, r5 to r7 the outputs); the flush mode as the
// originals leave it.
void SecondSetModelStreamData(lifted::Regs& ctx);
void SecondSetModelStreamSize(lifted::Regs& ctx);
void SecondSetModelStreamData2(lifted::Regs& ctx);
void SecondSetModelStreamSize2(lifted::Regs& ctx);
void SecondSetMorphSetUp(lifted::Regs& ctx);
void SecondSetMorphBlend(lifted::Regs& ctx);
void SecondSetMorphRun(lifted::Regs& ctx);
void SecondSetTireFillEntries(lifted::Regs& ctx);
void SecondSetTireReadEntry(lifted::Regs& ctx);
// ScenePools::ReleaseB sub_820C5C88 (r3 the set, r4 the pool, r5 the
// index; its adapter is the diagnostics' wrap, src/debug/frame_state_diff.cpp),
// SceneCockpit::Release sub_820DBF88 and SceneModel::Release sub_820DA5D0
// (no adapter: only ReleaseB calls it; the harness's) (objects.h): r3 the
// object; natively where SecondSetReleaseNative says (else the original), the
// nested ReleaseB's original where its plain C++ declines. r3 as the original
// leaves it.
bool SecondSetReleaseNative(uint32_t pool);
second_set::ReleaseCalls SecondSetReleaseCalls(lifted::Regs& ctx);
void SecondSetReleaseB(lifted::Regs& ctx);
void SecondSetSceneCockpitRelease(lifted::Regs& ctx);
void SecondSetSceneModelRelease(lifted::Regs& ctx);
// SceneCockpit::~SceneCockpit sub_820DC2F8 (no adapter: only its deleting
// destructor calls it; the harness's) and the deleting destructor
// sub_820DC448 (r3 the object, left; r4 the flags, bit 0 to the original):
// natively where pool 2's release is (else the original).
void SecondSetSceneCockpitDestruct(lifted::Regs& ctx);
void SecondSetSceneCockpitDeletingDestruct(lifted::Regs& ctx);
// The destructors and deleting destructors (objects.h; r3 the object, left;
// r4 the flags), natively where their releases are (else the original), and
// the point blocks' destructors (blocks.h; r3 as the original leaves it).
void SecondSetSceneCarModelDestruct(lifted::Regs& ctx);
void SecondSetScenePartsDestruct(lifted::Regs& ctx);
void SecondSetSceneInteriorDestruct(lifted::Regs& ctx);
void SecondSetSceneLightsDestruct(lifted::Regs& ctx);
void SecondSetSceneWheelsDestruct(lifted::Regs& ctx);
void SecondSetSceneTrafficDestruct(lifted::Regs& ctx);
void SecondSetSceneCarModelDeletingDestruct(lifted::Regs& ctx);
void SecondSetScenePartsDeletingDestruct(lifted::Regs& ctx);
void SecondSetSceneInteriorDeletingDestruct(lifted::Regs& ctx);
void SecondSetSceneLightsDeletingDestruct(lifted::Regs& ctx);
void SecondSetSceneWheelsDeletingDestruct(lifted::Regs& ctx);
void SecondSetSceneTrafficDeletingDestruct(lifted::Regs& ctx);
void SecondSetPoolObjectDeletingDestruct(lifted::Regs& ctx);
void SecondSetPartsPointsDestruct(lifted::Regs& ctx);
void SecondSetCockpitPointsDestruct(lifted::Regs& ctx);
void SecondSetInteriorPointsDestruct(lifted::Regs& ctx);
void SecondSetLightPointsDestruct(lifted::Regs& ctx);
void SecondSetWheelPointsDestruct(lifted::Regs& ctx);
void SecondSetTrafficPointsDestruct(lifted::Regs& ctx);


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
second_set::FindCalls SecondSetFindCalls(lifted::Regs& ctx);
// One set-up's plain C++ (nothing done unless SecondSetFindsNative()), for
// the adapters and the harness.
void SecondSetFindStep(lifted::Regs& ctx, FindFunction find);
void SecondSetPartsFind(lifted::Regs& ctx);
void SecondSetCockpitFind(lifted::Regs& ctx);
void SecondSetInteriorFind(lifted::Regs& ctx);
void SecondSetLightsFind(lifted::Regs& ctx);
void SecondSetTrafficFind(lifted::Regs& ctx);


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
second_set::WheelCalls SecondSetWheelCalls(lifted::Regs& ctx);
void SecondSetWheelStep(lifted::Regs& ctx, WheelFunction step);
void SecondSetWheelPointsSetUp(lifted::Regs& ctx);

}  // namespace hooks
