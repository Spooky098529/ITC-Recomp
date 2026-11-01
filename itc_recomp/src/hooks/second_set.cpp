// The second set's objects natively: adapters in front of src/second_set/
// (docs/functions/second-set.md), with the register steps the harness shares
// (hooks/second_set.h).

#include "hooks/second_set.h"

#include "base/flags.h"

#include <optional>
#include "native/buffers.h"
#include "second_set/blocks.h"
#include "second_set/finds.h"
#include "second_set/objects.h"
#include "second_set/point_groups.h"
#include "second_set/points.h"
#include "scene_loaders/model_point.h"
#include "hooks/scene_loaders.h"

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
              "TrafficPoints::TrafficPoints sub_820E68E8 and Reset sub_820E5880, the wheels' "
              "WheelPoints::ResetTire sub_820E74B8, ResetRimA sub_820E7918, ResetRimB "
              "sub_820E7C08 and ResetBrakes sub_820E7F90, the objects' resets "
              "SceneParts::Reset sub_820E0510 (with --native_material_passes), "
              "SceneCockpit::Reset sub_820DBE20, SceneInterior::Reset sub_820E3748, "
              "SceneLights::Reset sub_820DCA40 and SceneTraffic::Reset sub_820E5930, "
              "SceneCarModel::Release sub_820DB6C0 (where ScenePools::ReleaseA's plain C++ "
              "runs), the scene model's block SceneModelBlock::SceneModelBlock sub_820DA108 and "
              "Reset sub_820D9F50, SceneWheels::Reset sub_820E88A8, WheelPoints::Reset sub_820E85C8 and "
              "WheelPoints sub_820ECC48 with InitTire sub_820E8A80, InitRimA sub_820E99C0, InitRimB "
              "sub_820EA8E8 and InitBrakes sub_820EB320, the releases SceneParts::Release "
              "sub_820E0590 (with --native_material_passes), SceneInterior::Release sub_820E3890, "
              "SceneLights::Release sub_820DCB20, SceneWheels::Release sub_820E89B0 and "
              "SceneTraffic::Release sub_820E5A18 (where ScenePools::ReleaseA's plain C++ runs), the "
              "constructors SceneCarModel sub_820DB770, SceneParts sub_820E2F18 (with "
              "--native_material_passes), SceneCockpit sub_820DC3E8, SceneInterior sub_820E5340, "
              "SceneLights sub_820DE030, SceneWheels sub_820ECE40, SceneTraffic sub_820E6E70 and "
              "SceneModel sub_820DB330; with "
              "--native_models and --native_material_passes the point set-ups "
              "PartsPoints::Find sub_820E0258, CockpitPoints::Find sub_820DBC88, "
              "InteriorPoints::Find sub_820E3660, LightPoints::Find sub_820DC900 and "
              "TrafficPoints::Find sub_820E58C8 (with the "
              "groups' finds under them, no adapters: only they call them; "
              "Skeleton::UpdateMatrices' original where its plain C++ declines)) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=second_set); the point groups' "
              "resets and inits, SetNode, Clear, SetShape and the cockpit's, interior's, light kit's and "
              "traffic body's block resets have no adapters (their callers are native)");

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
ITC_SECOND_SET_GROUP(820E74B8, WheelsResetTire)
ITC_SECOND_SET_GROUP(820E7918, WheelsResetRimA)
ITC_SECOND_SET_GROUP(820E7C08, WheelsResetRimB)
ITC_SECOND_SET_GROUP(820E7F90, WheelsResetBrakes)
ITC_SECOND_SET_GROUP(820DBE20, SceneCockpitReset)
ITC_SECOND_SET_GROUP(820E3748, SceneInteriorReset)
ITC_SECOND_SET_GROUP(820DCA40, SceneLightsReset)
ITC_SECOND_SET_GROUP(820E5930, SceneTrafficReset)
ITC_SECOND_SET_GROUP(820D9F50, SceneModelBlockReset)

void SecondSetScenePartsReset(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_material_passes)) {
    __imp__sub_820E0510(ctx, base);
    return;
  }
  GroupStep(ctx, base, __imp__sub_820E0510, ss::ScenePartsReset);
}

void SecondSetSceneCarModelRelease(PPCContext& ctx, uint8_t* base) {
  if (!native::Buffers_NativeMemcpy() || !SceneLoadersReleaseANative(0)) {
    __imp__sub_820DB6C0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (const auto r3 = ss::SceneCarModelRelease(base, ctx.r3.u32, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_820DB6C0(ctx, base);
}

ITC_SECOND_SET_GROUP(820E88A8, SceneWheelsReset)
ITC_SECOND_SET_GROUP(820E85C8, WheelPointsReset)
ITC_SECOND_SET_GROUP(820ECC48, WheelPointsConstruct)
ITC_SECOND_SET_GROUP(820E8A80, WheelsInitTire)
ITC_SECOND_SET_GROUP(820E99C0, WheelsInitRimA)
ITC_SECOND_SET_GROUP(820EA8E8, WheelsInitRimB)
ITC_SECOND_SET_GROUP(820EB320, WheelsInitBrakes)

namespace {

using Release = std::optional<uint32_t> (*)(uint8_t*, uint32_t, uint32_t);
using Construct = uint32_t (*)(uint8_t*, uint32_t, uint32_t, uint32_t);

// A release: natively where memcpy is native and ReleaseA's plain C++ runs
// for `pool` (and with --native_material_passes for the parts'); r3 as the
// plain C++ returns it, else the original.
void ReleaseStep(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
                 uint32_t pool, Release release) {
  if (!native::Buffers_NativeMemcpy() || !SceneLoadersReleaseANative(pool) ||
      (pool == 1 && !ITC_FLAG(native_material_passes))) {
    original(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  if (const auto r3 = release(base, ctx.r3.u32, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  original(ctx, base);
}

// A constructor: r3 = the object, r4 = its list; r3 = the object.
void ConstructStep(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
                   Construct construct) {
  if (!native::Buffers_NativeMemcpy()) {
    original(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = construct(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

}  // namespace

#define ITC_SECOND_SET_RELEASE(address, pool, name)                       \
  void SecondSet##name(PPCContext& ctx, uint8_t* base) {                  \
    ReleaseStep(ctx, base, __imp__sub_##address, pool, ss::name);         \
  }
#define ITC_SECOND_SET_CONSTRUCT(address, name)                           \
  void SecondSet##name(PPCContext& ctx, uint8_t* base) {                  \
    ConstructStep(ctx, base, __imp__sub_##address, ss::name);             \
  }

ITC_SECOND_SET_RELEASE(820E0590, 1, ScenePartsRelease)
ITC_SECOND_SET_RELEASE(820E3890, 3, SceneInteriorRelease)
ITC_SECOND_SET_RELEASE(820DCB20, 4, SceneLightsRelease)
ITC_SECOND_SET_RELEASE(820E89B0, 5, SceneWheelsRelease)
ITC_SECOND_SET_RELEASE(820E5A18, 6, SceneTrafficRelease)
ITC_SECOND_SET_CONSTRUCT(820DB770, SceneCarModelConstruct)
ITC_SECOND_SET_CONSTRUCT(820DC3E8, SceneCockpitConstruct)
ITC_SECOND_SET_CONSTRUCT(820E5340, SceneInteriorConstruct)
ITC_SECOND_SET_CONSTRUCT(820DE030, SceneLightsConstruct)
ITC_SECOND_SET_CONSTRUCT(820ECE40, SceneWheelsConstruct)
ITC_SECOND_SET_CONSTRUCT(820E6E70, SceneTrafficConstruct)
ITC_SECOND_SET_CONSTRUCT(820DB330, SceneModelConstruct)

void SecondSetScenePartsConstruct(PPCContext& ctx, uint8_t* base) {
  if (!ITC_FLAG(native_material_passes)) {
    __imp__sub_820E2F18(ctx, base);
    return;
  }
  ConstructStep(ctx, base, __imp__sub_820E2F18, ss::ScenePartsConstruct);
}

void SecondSetSceneModelBlockConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ss::SceneModelBlockConstruct(base, ctx.r3.u32);
}

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

// No adapters (their callers in play are native and call their plain C++;
// the steps above are the harness's): PointRecord::SetNode sub_820CE8D8, and
// the point groups' resets and inits (2026-11-01): the parts' 19, the
// cockpit's 6, the interior's 6, the light kit's 3 and the traffic body's 3;
// PointRecord::Clear sub_820CEA08 and SetShape sub_820CE900, CockpitPoints::Reset
// sub_820DBC10, InteriorPoints::Reset sub_820E35D8, LightPoints::Reset
// sub_820DC8A8 and TrafficPoints::Reset sub_820E5880 (2026-11-01, their callers
// in play native).
ITC_SECOND_SET(820D0270, SecondSetPointConstruct)
ITC_SECOND_SET(820CEB30, SecondSetFindShape)
ITC_SECOND_SET(820E0258, SecondSetPartsFind)
ITC_SECOND_SET(820DBC88, SecondSetCockpitFind)
ITC_SECOND_SET(820E3660, SecondSetInteriorFind)
ITC_SECOND_SET(820DC900, SecondSetLightsFind)
ITC_SECOND_SET(820E58C8, SecondSetTrafficFind)
ITC_SECOND_SET(820E2A10, SecondSetPartsPointsConstruct)
ITC_SECOND_SET(820E00F0, SecondSetPartsPointsReset)
ITC_SECOND_SET(820DC190, SecondSetCockpitPointsConstruct)
ITC_SECOND_SET(820E4FD8, SecondSetInteriorPointsConstruct)
ITC_SECOND_SET(820DDCF8, SecondSetLightPointsConstruct)
ITC_SECOND_SET(820E68E8, SecondSetTrafficPointsConstruct)
ITC_SECOND_SET(820E74B8, SecondSetWheelsResetTire)
ITC_SECOND_SET(820E7918, SecondSetWheelsResetRimA)
ITC_SECOND_SET(820E7C08, SecondSetWheelsResetRimB)
ITC_SECOND_SET(820E7F90, SecondSetWheelsResetBrakes)
ITC_SECOND_SET(820E0510, SecondSetScenePartsReset)
ITC_SECOND_SET(820DBE20, SecondSetSceneCockpitReset)
ITC_SECOND_SET(820E3748, SecondSetSceneInteriorReset)
ITC_SECOND_SET(820DCA40, SecondSetSceneLightsReset)
ITC_SECOND_SET(820E5930, SecondSetSceneTrafficReset)
ITC_SECOND_SET(820DB6C0, SecondSetSceneCarModelRelease)
ITC_SECOND_SET(820DA108, SecondSetSceneModelBlockConstruct)
ITC_SECOND_SET(820D9F50, SecondSetSceneModelBlockReset)
ITC_SECOND_SET(820E88A8, SecondSetSceneWheelsReset)
ITC_SECOND_SET(820E85C8, SecondSetWheelPointsReset)
ITC_SECOND_SET(820ECC48, SecondSetWheelPointsConstruct)
ITC_SECOND_SET(820E8A80, SecondSetWheelsInitTire)
ITC_SECOND_SET(820E99C0, SecondSetWheelsInitRimA)
ITC_SECOND_SET(820EA8E8, SecondSetWheelsInitRimB)
ITC_SECOND_SET(820EB320, SecondSetWheelsInitBrakes)
ITC_SECOND_SET(820E0590, SecondSetScenePartsRelease)
ITC_SECOND_SET(820E3890, SecondSetSceneInteriorRelease)
ITC_SECOND_SET(820DCB20, SecondSetSceneLightsRelease)
ITC_SECOND_SET(820E89B0, SecondSetSceneWheelsRelease)
ITC_SECOND_SET(820E5A18, SecondSetSceneTrafficRelease)
ITC_SECOND_SET(820DB770, SecondSetSceneCarModelConstruct)
ITC_SECOND_SET(820E2F18, SecondSetScenePartsConstruct)
ITC_SECOND_SET(820DC3E8, SecondSetSceneCockpitConstruct)
ITC_SECOND_SET(820E5340, SecondSetSceneInteriorConstruct)
ITC_SECOND_SET(820DE030, SecondSetSceneLightsConstruct)
ITC_SECOND_SET(820ECE40, SecondSetSceneWheelsConstruct)
ITC_SECOND_SET(820E6E70, SecondSetSceneTrafficConstruct)
ITC_SECOND_SET(820DB330, SecondSetSceneModelConstruct)
