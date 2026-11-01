// The second set's objects' register steps (src/second_set/,
// docs/functions/second-set.md), shared by the adapters
// (src/hooks/second_set.cpp, behind --native_second_set) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ can't (memcpy not native).

#pragma once

#include "guest.h"
#include "second_set/finds.h"

namespace hooks {

// PointRecord::SetNode sub_820CE8D8 and SetShape sub_820CE900: r3 = the
// record (left), r4 = the string.
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

}  // namespace hooks
