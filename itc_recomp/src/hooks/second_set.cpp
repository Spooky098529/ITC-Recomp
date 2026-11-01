// The second set's objects natively: adapters in front of src/second_set/
// (docs/functions/second-set.md), with the register steps the harness shares
// (hooks/second_set.h).

#include "hooks/second_set.h"

#include "base/flags.h"
#include "native/buffers.h"
#include "second_set/blocks.h"
#include "second_set/finds.h"
#include "second_set/point_groups.h"
#include "second_set/points.h"
#include "scene_loaders/model_point.h"

ITC_FLAG_DECLARE(bool, native_material_passes);
ITC_FLAG_DECLARE(bool, native_models);

ITC_FLAG_BOOL(native_second_set, true, "ITC",
              "Run the second set of the car scene's objects (docs/functions/second-set.md: "
              "PointRecord::SetNode sub_820CE8D8 (no adapter: its callers are native) and SetShape "
              "sub_820CE900, with native memcpy "
              "PointRecord::Clear sub_820CEA08, PointRecord::PointRecord sub_820D0270 and "
              "ModelPoint::FindShape sub_820CEB30, the point groups' resets and inits: "
              "PartsPoints::ResetFrontBumper sub_820DE0E0, ResetRearBumper sub_820DE2C8, "
              "ResetRearSpoiler sub_820DE498, ResetBonnet sub_820DE548, ResetLeftDoor "
              "sub_820DE600, ResetRightDoor sub_820DE7E8 (with --native_material_passes), "
              "ResetDoors sub_820DE9D0, ResetHeadlights sub_820DF740, ResetTailLights "
              "sub_820DEBF0, ResetFenders sub_820DECD0, ResetCabin sub_820DEE88, ResetEngine "
              "sub_820DF020, ResetMufflers sub_820DF0C0, ResetShadows sub_820DF128, InitCooling "
              "sub_820DF410, InitSideSkirts sub_820DF570, InitLeftMirror sub_820DF668, "
              "InitRightMirror sub_820DF6B0, InitMirror sub_820DF6F8, CockpitPoints::ResetFenders "
              "sub_820DB878, ResetCabin sub_820DB930, ResetDoors sub_820DB9E8, ResetHeadlights "
              "sub_820DBAA0, InitBonnet sub_820DC0A8 and InitMirror sub_820DC0F0, "
              "InteriorPoints::InitBody sub_820E39B0, ResetDials sub_820E2FC8, ResetMeters "
              "sub_820E3200, ResetSteering sub_820E33F0, ResetSeats sub_820E3440, ResetRollCage "
              "sub_820E34F8, LightPoints::ResetFront sub_820DC748, ResetSides sub_820DC7A0, "
              "ResetRear sub_820DC7F8, TrafficPoints::ResetBody sub_820E54E8, ResetHeadlights "
              "sub_820E56D0 and ResetTailLights sub_820E57A0, the point blocks' constructors and "
              "resets: PartsPoints::PartsPoints sub_820E2A10 and Reset sub_820E00F0 (with "
              "--native_material_passes; with InitLeftDoor sub_820DF5B8, InitRightDoor "
              "sub_820DF610, InitHeadlights sub_820E1388 and InitMufflers sub_820E0010, no "
              "adapters: only PartsPoints::PartsPoints calls them), "
              "CockpitPoints::CockpitPoints sub_820DC190 and Reset sub_820DBC10, "
              "InteriorPoints::InteriorPoints sub_820E4FD8 and Reset sub_820E35D8, "
              "LightPoints::LightPoints sub_820DDCF8 and Reset sub_820DC8A8, "
              "TrafficPoints::TrafficPoints sub_820E68E8 and Reset sub_820E5880; with "
              "--native_models and --native_material_passes the point set-ups "
              "PartsPoints::Find sub_820E0258, CockpitPoints::Find sub_820DBC88, "
              "InteriorPoints::Find sub_820E3660, LightPoints::Find sub_820DC900 and "
              "TrafficPoints::Find sub_820E58C8 (with the "
              "groups' finds under them, no adapters: only they call them; "
              "Skeleton::UpdateMatrices' original where its plain C++ declines)) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=second_set)");

namespace hooks {

namespace ss = second_set;

void SecondSetPointSetNode(PPCContext& ctx, uint8_t* base) {
  ss::PointRecordSetNode(base, ctx.r3.u32, ctx.r4.u32);
}

void SecondSetPointSetShape(PPCContext& ctx, uint8_t* base) {
  ss::PointRecordSetShape(base, ctx.r3.u32, ctx.r4.u32);
}

void SecondSetPointClear(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820CEA08(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ss::PointRecordClear(base, ctx.r3.u32, ctx.r1.u32);
}

void SecondSetPointConstruct(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820D0270(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ss::PointRecordConstruct(base, ctx.r3.u32, ctx.r1.u32);
}

void SecondSetFindShape(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy()) {
    __imp__sub_820CEB30(ctx, base);
    return;
  }
  ctx.r3.u64 = ss::ModelPointFindShape(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

namespace {

// A point group's reset or init: r3 = the group or record, r3 as the plain
// C++ returns it; the original where memcpy isn't native.
template <typename Step>
void GroupStep(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
               Step step) {
  if (!native::Buffers_NativeMemcpy()) {
    original(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = step(base, ctx.r3.u32, ctx.r1.u32);
}

}  // namespace

#define ITC_SECOND_SET_GROUP(address, name)                         \
  void SecondSet##name(PPCContext& ctx, uint8_t* base) {            \
    GroupStep(ctx, base, __imp__sub_##address, ss::name);           \
  }

ITC_SECOND_SET_GROUP(820DE0E0, PartsResetFrontBumper)
ITC_SECOND_SET_GROUP(820DE2C8, PartsResetRearBumper)
ITC_SECOND_SET_GROUP(820DE498, PartsResetRearSpoiler)
ITC_SECOND_SET_GROUP(820DE548, PartsResetBonnet)
ITC_SECOND_SET_GROUP(820DE600, PartsResetLeftDoor)
ITC_SECOND_SET_GROUP(820DE9D0, PartsResetDoors)
ITC_SECOND_SET_GROUP(820DF740, PartsResetHeadlights)
ITC_SECOND_SET_GROUP(820DEBF0, PartsResetTailLights)
ITC_SECOND_SET_GROUP(820DECD0, PartsResetFenders)
ITC_SECOND_SET_GROUP(820DEE88, PartsResetCabin)
ITC_SECOND_SET_GROUP(820DF020, PartsResetEngine)
ITC_SECOND_SET_GROUP(820DF0C0, PartsResetMufflers)
ITC_SECOND_SET_GROUP(820DF128, PartsResetShadows)
ITC_SECOND_SET_GROUP(820DF410, PartsInitCooling)
ITC_SECOND_SET_GROUP(820DF570, PartsInitSideSkirts)
ITC_SECOND_SET_GROUP(820DF668, PartsInitLeftMirror)
ITC_SECOND_SET_GROUP(820DF6B0, PartsInitRightMirror)
ITC_SECOND_SET_GROUP(820DF6F8, PartsInitMirror)
ITC_SECOND_SET_GROUP(820DC0A8, CockpitInitBonnet)
ITC_SECOND_SET_GROUP(820DC0F0, CockpitInitMirror)
ITC_SECOND_SET_GROUP(820DB878, CockpitResetFenders)
ITC_SECOND_SET_GROUP(820DB930, CockpitResetCabin)
ITC_SECOND_SET_GROUP(820DB9E8, CockpitResetDoors)
ITC_SECOND_SET_GROUP(820DBAA0, CockpitResetHeadlights)
ITC_SECOND_SET_GROUP(820E39B0, InteriorInitBody)
ITC_SECOND_SET_GROUP(820E2FC8, InteriorResetDials)
ITC_SECOND_SET_GROUP(820E3200, InteriorResetMeters)
ITC_SECOND_SET_GROUP(820E33F0, InteriorResetSteering)
ITC_SECOND_SET_GROUP(820E3440, InteriorResetSeats)
ITC_SECOND_SET_GROUP(820E34F8, InteriorResetRollCage)
ITC_SECOND_SET_GROUP(820DC748, LightsResetFront)
ITC_SECOND_SET_GROUP(820DC7A0, LightsResetSides)
ITC_SECOND_SET_GROUP(820DC7F8, LightsResetRear)
ITC_SECOND_SET_GROUP(820E54E8, TrafficResetBody)
ITC_SECOND_SET_GROUP(820E56D0, TrafficResetHeadlights)
ITC_SECOND_SET_GROUP(820E57A0, TrafficResetTailLights)
ITC_SECOND_SET_GROUP(820DC190, CockpitPointsConstruct)
ITC_SECOND_SET_GROUP(820DBC10, CockpitPointsReset)
ITC_SECOND_SET_GROUP(820E4FD8, InteriorPointsConstruct)
ITC_SECOND_SET_GROUP(820E35D8, InteriorPointsReset)
ITC_SECOND_SET_GROUP(820DDCF8, LightPointsConstruct)
ITC_SECOND_SET_GROUP(820DC8A8, LightPointsReset)
ITC_SECOND_SET_GROUP(820E68E8, TrafficPointsConstruct)
ITC_SECOND_SET_GROUP(820E5880, TrafficPointsReset)

void SecondSetFindStep(PPCContext& ctx, uint8_t* base, FindFunction find) {
  if (!native::Buffers_NativeMemcpy() || !ITC_FLAG(native_models) ||
      !ITC_FLAG(native_material_passes)) {
    return;
  }
  ctx.fpscr.disableFlushMode();
  ss::FindCalls calls;
  calls.update_matrices = [&ctx, base](uint32_t skeleton, uint32_t frame) {
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    // ModelPoint::Find's frame (its back chain written, as the original's).
    guest::StoreU32(base, frame, frame + scene_loaders::kModelPointFrameBytes);
    ctx.r1.u64 = frame;
    ctx.r3.u64 = skeleton;
    ctx.lr = 0x820D03EC;
    __imp__sub_825998A0(ctx, base);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    ctx.fpscr.disableFlushMode();
  };
  ctx.r3.u64 = find(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
  if (calls.flush) {
    ctx.fpscr.enableFlushMode();
  } else {
    ctx.fpscr.disableFlushMode();
  }
}

bool SecondSetFindsNative() {
  return native::Buffers_NativeMemcpy() && ITC_FLAG(native_models) &&
         ITC_FLAG(native_material_passes);
}

void SecondSetPartsFind(PPCContext& ctx, uint8_t* base) {
  if (!SecondSetFindsNative()) {
    __imp__sub_820E0258(ctx, base);
    return;
  }
  SecondSetFindStep(ctx, base, ss::PartsFind);
}

void SecondSetCockpitFind(PPCContext& ctx, uint8_t* base) {
  if (!SecondSetFindsNative()) {
    __imp__sub_820DBC88(ctx, base);
    return;
  }
  SecondSetFindStep(ctx, base, ss::CockpitFind);
}

void SecondSetInteriorFind(PPCContext& ctx, uint8_t* base) {
  if (!SecondSetFindsNative()) {
    __imp__sub_820E3660(ctx, base);
    return;
  }
  SecondSetFindStep(ctx, base, ss::InteriorFind);
}

void SecondSetLightsFind(PPCContext& ctx, uint8_t* base) {
  if (!SecondSetFindsNative()) {
    __imp__sub_820DC900(ctx, base);
    return;
  }
  SecondSetFindStep(ctx, base, ss::LightsFind);
}

void SecondSetTrafficFind(PPCContext& ctx, uint8_t* base) {
  if (!SecondSetFindsNative()) {
    __imp__sub_820E58C8(ctx, base);
    return;
  }
  SecondSetFindStep(ctx, base, ss::TrafficFind);
}

void SecondSetPartsPointsConstruct(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_material_passes)) {
    __imp__sub_820E2A10(ctx, base);
    return;
  }
  GroupStep(ctx, base, __imp__sub_820E2A10, ss::PartsPointsConstruct);
}

void SecondSetPartsPointsReset(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_material_passes)) {
    __imp__sub_820E00F0(ctx, base);
    return;
  }
  GroupStep(ctx, base, __imp__sub_820E00F0, ss::PartsPointsReset);
}

void SecondSetPartsResetRightDoor(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_material_passes)) {
    __imp__sub_820DE7E8(ctx, base);
    return;
  }
  GroupStep(ctx, base, __imp__sub_820DE7E8, ss::PartsResetRightDoor);
}

}  // namespace hooks

#define ITC_SECOND_SET(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_second_set)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// No adapter: PointRecord::SetNode sub_820CE8D8 (its callers in play are
// native and call its plain C++; the step above is the harness's).
ITC_SECOND_SET(820CE900, SecondSetPointSetShape)
ITC_SECOND_SET(820CEA08, SecondSetPointClear)
ITC_SECOND_SET(820D0270, SecondSetPointConstruct)
ITC_SECOND_SET(820CEB30, SecondSetFindShape)
ITC_SECOND_SET(820DE0E0, SecondSetPartsResetFrontBumper)
ITC_SECOND_SET(820DE2C8, SecondSetPartsResetRearBumper)
ITC_SECOND_SET(820DE498, SecondSetPartsResetRearSpoiler)
ITC_SECOND_SET(820DE548, SecondSetPartsResetBonnet)
ITC_SECOND_SET(820DE600, SecondSetPartsResetLeftDoor)
ITC_SECOND_SET(820DE7E8, SecondSetPartsResetRightDoor)
ITC_SECOND_SET(820DE9D0, SecondSetPartsResetDoors)
ITC_SECOND_SET(820DF740, SecondSetPartsResetHeadlights)
ITC_SECOND_SET(820DEBF0, SecondSetPartsResetTailLights)
ITC_SECOND_SET(820DECD0, SecondSetPartsResetFenders)
ITC_SECOND_SET(820DEE88, SecondSetPartsResetCabin)
ITC_SECOND_SET(820DF020, SecondSetPartsResetEngine)
ITC_SECOND_SET(820DF0C0, SecondSetPartsResetMufflers)
ITC_SECOND_SET(820DF128, SecondSetPartsResetShadows)
ITC_SECOND_SET(820DF410, SecondSetPartsInitCooling)
ITC_SECOND_SET(820DF570, SecondSetPartsInitSideSkirts)
ITC_SECOND_SET(820DF668, SecondSetPartsInitLeftMirror)
ITC_SECOND_SET(820DF6B0, SecondSetPartsInitRightMirror)
ITC_SECOND_SET(820DF6F8, SecondSetPartsInitMirror)
ITC_SECOND_SET(820DC0A8, SecondSetCockpitInitBonnet)
ITC_SECOND_SET(820DC0F0, SecondSetCockpitInitMirror)
ITC_SECOND_SET(820DB878, SecondSetCockpitResetFenders)
ITC_SECOND_SET(820DB930, SecondSetCockpitResetCabin)
ITC_SECOND_SET(820DB9E8, SecondSetCockpitResetDoors)
ITC_SECOND_SET(820DBAA0, SecondSetCockpitResetHeadlights)
ITC_SECOND_SET(820E0258, SecondSetPartsFind)
ITC_SECOND_SET(820DBC88, SecondSetCockpitFind)
ITC_SECOND_SET(820E39B0, SecondSetInteriorInitBody)
ITC_SECOND_SET(820E2FC8, SecondSetInteriorResetDials)
ITC_SECOND_SET(820E3200, SecondSetInteriorResetMeters)
ITC_SECOND_SET(820E33F0, SecondSetInteriorResetSteering)
ITC_SECOND_SET(820E3440, SecondSetInteriorResetSeats)
ITC_SECOND_SET(820E34F8, SecondSetInteriorResetRollCage)
ITC_SECOND_SET(820DC748, SecondSetLightsResetFront)
ITC_SECOND_SET(820DC7A0, SecondSetLightsResetSides)
ITC_SECOND_SET(820DC7F8, SecondSetLightsResetRear)
ITC_SECOND_SET(820E54E8, SecondSetTrafficResetBody)
ITC_SECOND_SET(820E56D0, SecondSetTrafficResetHeadlights)
ITC_SECOND_SET(820E57A0, SecondSetTrafficResetTailLights)
ITC_SECOND_SET(820E3660, SecondSetInteriorFind)
ITC_SECOND_SET(820DC900, SecondSetLightsFind)
ITC_SECOND_SET(820E58C8, SecondSetTrafficFind)
ITC_SECOND_SET(820E2A10, SecondSetPartsPointsConstruct)
ITC_SECOND_SET(820E00F0, SecondSetPartsPointsReset)
ITC_SECOND_SET(820DC190, SecondSetCockpitPointsConstruct)
ITC_SECOND_SET(820DBC10, SecondSetCockpitPointsReset)
ITC_SECOND_SET(820E4FD8, SecondSetInteriorPointsConstruct)
ITC_SECOND_SET(820E35D8, SecondSetInteriorPointsReset)
ITC_SECOND_SET(820DDCF8, SecondSetLightPointsConstruct)
ITC_SECOND_SET(820DC8A8, SecondSetLightPointsReset)
ITC_SECOND_SET(820E68E8, SecondSetTrafficPointsConstruct)
ITC_SECOND_SET(820E5880, SecondSetTrafficPointsReset)
