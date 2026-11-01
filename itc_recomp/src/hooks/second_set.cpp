// The second set's objects natively: adapters in front of src/second_set/
// (docs/functions/second-set.md), with the register steps the harness shares
// (hooks/second_set.h).

#include "hooks/second_set.h"

#include "base/flags.h"

#include <optional>
#include "native/buffers.h"
#include "second_set/blocks.h"
#include "second_set/finds.h"
#include "second_set/morph.h"
#include "second_set/objects.h"
#include "second_set/point_groups.h"
#include "second_set/points.h"
#include "scene_loaders/model_point.h"
#include "hooks/scene_loaders.h"
#include "hooks/heap.h"
#include "second_set/wheels.h"

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
              "SceneModel sub_820DB330, the destructors ~SceneCarModel sub_820DB720, ~SceneParts "
              "sub_820E2E10, ~SceneInterior sub_820E5178, ~SceneLights sub_820DDE00, ~SceneWheels "
              "sub_820ECDD0 (where their releases run natively) and ~SceneTraffic sub_820E69F0, the "
              "deleting destructors sub_820DB7C8, sub_820E2F78, sub_820E53A0, sub_820DE090, "
              "sub_820ECEB8, sub_820E6ED8 and the pool objects' base class's sub_820DB400 (without "
              "operator delete: r4 bit 0 goes to the original), the point blocks' destructors "
              "sub_820E2C10, sub_820DC258, sub_820E50C0, sub_820DDD90, sub_820ECCB8 and "
              "sub_820E6980, the model's shape accessors Model::StreamData sub_82594A60, StreamSize "
              "sub_82594A78, StreamData2 sub_82594A90 and StreamSize2 sub_82594AA8, the morph "
              "Morph::SetUp sub_82594C18, Run sub_82594D10 and Blend sub_8258BD28 (with Blend6 "
              "sub_8258B578 and Blend7 sub_8258B678, no adapters: only Blend calls them; kind 8 "
              "sub_8258B7E0 and memcpys that would go to the original the original), and the "
              "tire's sub_820E6F28 and sub_820E7010, the wheels' set-up WheelPoints::SetUp "
              "sub_820E8668 (with SetUpTire sub_820E7598, SetUpRimA sub_820E79C8, SetUpRimB "
              "sub_820E7CA0 and SetUpBrakes sub_820E8028, no adapters: only SetUp calls them; "
              "with native memcpy, the native heap and --native_material_passes); with "
              "--native_models and --native_material_passes the point set-ups "
              "PartsPoints::Find sub_820E0258, CockpitPoints::Find sub_820DBC88, "
              "InteriorPoints::Find sub_820E3660, LightPoints::Find sub_820DC900 and "
              "TrafficPoints::Find sub_820E58C8 (with the "
              "groups' finds under them, no adapters: only they call them; "
              "Skeleton::UpdateMatrices' original where its plain C++ declines)) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=second_set); the point groups' "
              "resets and inits, SetNode, Clear, SetShape, PointRecord::PointRecord, the point blocks' "
              "constructors and the cockpit's, interior's, light kit's and traffic body's block "
              "resets, the wheels' groups and WheelPoints::Reset, and the objects' resets but "
              "SceneCockpit's and SceneModelBlock's have no adapters (their callers are native), nor "
              "do ModelPoint::FindShape, the shape accessors and Morph::Run");

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

namespace {

// Whether a destructor's release runs natively: memcpy native and
// ReleaseA's plain C++ for `pool` (with --native_material_passes for the
// parts'); a pool past 6 for the ones that release nothing.
bool DestructNative(uint32_t pool) {
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  if (pool > 6) {
    return true;
  }
  return SceneLoadersReleaseANative(pool) && (pool != 1 || ITC_FLAG(native_material_passes));
}

// A destructor: r3 = the object, left; the original where it declines.
void DestructStep(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
                  uint32_t pool, bool (*destruct)(uint8_t*, uint32_t, uint32_t)) {
  if (DestructNative(pool)) {
    ctx.fpscr.disableFlushMode();
    if (destruct(base, ctx.r3.u32, ctx.r1.u32)) {
      return;
    }
  }
  original(ctx, base);
}

// A deleting destructor: r3 = the object (left), r4 = the flags.
void DeletingStep(PPCContext& ctx, uint8_t* base, void (*original)(PPCContext&, uint8_t*),
                  uint32_t pool, bool (*destruct)(uint8_t*, uint32_t, uint32_t, uint32_t)) {
  if (DestructNative(pool)) {
    ctx.fpscr.disableFlushMode();
    if (destruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32)) {
      return;
    }
  }
  original(ctx, base);
}

}  // namespace

#define ITC_SECOND_SET_DESTRUCT(address, pool, name)                      \
  void SecondSet##name(PPCContext& ctx, uint8_t* base) {                  \
    DestructStep(ctx, base, __imp__sub_##address, pool, ss::name);        \
  }
#define ITC_SECOND_SET_DELETING(address, pool, name)                      \
  void SecondSet##name(PPCContext& ctx, uint8_t* base) {                  \
    DeletingStep(ctx, base, __imp__sub_##address, pool, ss::name);        \
  }
#define ITC_SECOND_SET_POINTS_DESTRUCT(name)                              \
  void SecondSet##name(PPCContext& ctx, uint8_t* base) {                  \
    (void)base;                                                           \
    ctx.r3.u64 = ss::name(ctx.r3.u32);                                    \
  }

ITC_SECOND_SET_DESTRUCT(820DB720, 0, SceneCarModelDestruct)
ITC_SECOND_SET_DESTRUCT(820E2E10, 1, ScenePartsDestruct)
ITC_SECOND_SET_DESTRUCT(820E5178, 3, SceneInteriorDestruct)
ITC_SECOND_SET_DESTRUCT(820DDE00, 4, SceneLightsDestruct)
ITC_SECOND_SET_DESTRUCT(820ECDD0, 5, SceneWheelsDestruct)
ITC_SECOND_SET_DELETING(820DB7C8, 0, SceneCarModelDeletingDestruct)
ITC_SECOND_SET_DELETING(820E2F78, 1, ScenePartsDeletingDestruct)
ITC_SECOND_SET_DELETING(820E53A0, 3, SceneInteriorDeletingDestruct)
ITC_SECOND_SET_DELETING(820DE090, 4, SceneLightsDeletingDestruct)
ITC_SECOND_SET_DELETING(820ECEB8, 5, SceneWheelsDeletingDestruct)
ITC_SECOND_SET_DELETING(820E6ED8, 7, SceneTrafficDeletingDestruct)
ITC_SECOND_SET_DELETING(820DB400, 7, PoolObjectDeletingDestruct)
ITC_SECOND_SET_POINTS_DESTRUCT(PartsPointsDestruct)
ITC_SECOND_SET_POINTS_DESTRUCT(CockpitPointsDestruct)
ITC_SECOND_SET_POINTS_DESTRUCT(InteriorPointsDestruct)
ITC_SECOND_SET_POINTS_DESTRUCT(LightPointsDestruct)
ITC_SECOND_SET_POINTS_DESTRUCT(WheelPointsDestruct)
ITC_SECOND_SET_POINTS_DESTRUCT(TrafficPointsDestruct)

void SecondSetSceneTrafficDestruct(PPCContext& ctx, uint8_t* base) {
  ss::SceneTrafficDestruct(base, ctx.r3.u32);
}

namespace {

// The morph's plain C++ runs with the flush mode off (its scalar steps follow
// the originals' disableFlushMode; its vector steps turn it on themselves);
// after, the mode as the original leaves it: `was_on` put back when it's
// left unchanged.
bool FlushOffForMorph(PPCContext& ctx) {
  const bool was_on = (ctx.fpscr.csr & uint32_t(decltype(ctx.fpscr)::FlushMask)) != 0;
  ctx.fpscr.disableFlushMode();
  return was_on;
}

void SetFlush(PPCContext& ctx, ss::Flush flush, bool was_on) {
  if (flush == ss::Flush::kOn || (flush == ss::Flush::kUnchanged && was_on)) {
    ctx.fpscr.enableFlushMode();
  } else {
    ctx.fpscr.disableFlushMode();
  }
}

}  // namespace

void SecondSetModelStreamData(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::ModelStreamData(base, ctx.r3.u32, ctx.r4.u32);
}
void SecondSetModelStreamSize(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::ModelStreamSize(base, ctx.r3.u32, ctx.r4.u32);
}
void SecondSetModelStreamData2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::ModelStreamData2(base, ctx.r3.u32, ctx.r4.u32);
}
void SecondSetModelStreamSize2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::ModelStreamSize2(base, ctx.r3.u32, ctx.r4.u32);
}

void SecondSetMorphSetUp(PPCContext& ctx, uint8_t* base) {
  const bool was_on = FlushOffForMorph(ctx);
  SetFlush(ctx,
           ss::MorphSetUp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                          ctx.r8.u32),
           was_on);
}

void SecondSetMorphBlend(PPCContext& ctx, uint8_t* base) {
  const bool was_on = FlushOffForMorph(ctx);
  const auto result = ss::MorphBlend(base, ctx.r3.u32, ctx.r1.u32);
  if (!result) {
    SetFlush(ctx, ss::Flush::kUnchanged, was_on);
    __imp__sub_8258BD28(ctx, base);
    return;
  }
  ctx.r3.u64 = result->r3;
  SetFlush(ctx, result->flush, was_on);
}

void SecondSetMorphRun(PPCContext& ctx, uint8_t* base) {
  const bool was_on = FlushOffForMorph(ctx);
  const auto result = ss::MorphRun(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
  if (!result) {
    SetFlush(ctx, ss::Flush::kUnchanged, was_on);
    __imp__sub_82594D10(ctx, base);
    return;
  }
  ctx.r3.u64 = result->r3;
  SetFlush(ctx, result->flush, was_on);
}

void SecondSetTireFillEntries(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ss::TireFillEntries(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.r1.u32);
}

void SecondSetTireReadEntry(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ss::TireReadEntry(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                    ctx.r1.u32);
}

bool SecondSetWheelsNative() {
  return native::Buffers_NativeMemcpy() && NativeHeapActive() &&
         ITC_FLAG(native_material_passes);
}

namespace {

// A call to an original from the plain C++'s frame at `sp`, r1 and lr put
// back after, the flush mode left off.
template <typename Body>
void FromFrame(PPCContext& ctx, uint32_t sp, uint32_t lr, Body body) {
  const uint64_t stack = ctx.r1.u64, link = ctx.lr;
  ctx.r1.u64 = sp;
  ctx.lr = lr;
  body();
  ctx.r1.u64 = stack;
  ctx.lr = link;
  ctx.fpscr.disableFlushMode();
}

}  // namespace

ss::WheelCalls SecondSetWheelCalls(PPCContext& ctx, uint8_t* base) {
  ss::WheelCalls calls;
  calls.morph_run = [&ctx, base](uint32_t model, uint32_t record, uint32_t out1, uint32_t out2,
                                 uint32_t sp, uint32_t lr) {
    bool on = false;
    FromFrame(ctx, sp, lr, [&] {
      ctx.r3.u64 = model;
      ctx.r4.u64 = record;
      ctx.r5.u64 = out1;
      ctx.r6.u64 = out2;
      __imp__sub_82594D10(ctx, base);
      on = (ctx.fpscr.csr & uint32_t(decltype(ctx.fpscr)::FlushMask)) != 0;
    });
    return on;
  };
  calls.memcpy = [&ctx, base](uint32_t to, uint32_t from, uint32_t bytes, uint32_t sp,
                              uint32_t lr) {
    FromFrame(ctx, sp, lr, [&] {
      ctx.r3.u64 = to;
      ctx.r4.u64 = from;
      ctx.r5.u64 = bytes;
      __imp__sub_825A7410(ctx, base);
    });
  };
  calls.free_pages_in = [&ctx, base](uint32_t at, uint32_t sp, uint32_t lr) {
    uint64_t r3 = 0;
    FromFrame(ctx, sp, lr, [&] {
      ctx.r3.u64 = at;
      __imp__sub_820D09A0(ctx, base);
      r3 = ctx.r3.u64;
    });
    return r3;
  };
  return calls;
}

void SecondSetWheelStep(PPCContext& ctx, uint8_t* base, WheelFunction step) {
  const bool was_on = FlushOffForMorph(ctx);
  ss::WheelCalls calls = SecondSetWheelCalls(ctx, base);
  ctx.r3.u64 = step(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, calls);
  SetFlush(ctx, calls.flush, was_on);
}

void SecondSetWheelPointsSetUp(PPCContext& ctx, uint8_t* base) {
  if (!SecondSetWheelsNative()) {
    __imp__sub_820E8668(ctx, base);
    return;
  }
  const bool was_on = FlushOffForMorph(ctx);
  ss::WheelCalls calls = SecondSetWheelCalls(ctx, base);
  ctx.r3.u64 = ss::WheelPointsSetUp(base, ctx.r3.u32, ctx.r1.u32, calls);
  SetFlush(ctx, calls.flush, was_on);
}

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
// in play native); PointRecord::PointRecord sub_820D0270, the objects' resets
// SceneParts sub_820E0510, SceneInterior sub_820E3748, SceneLights sub_820DCA40,
// SceneWheels sub_820E88A8 and SceneTraffic sub_820E5930, the point blocks'
// constructors PartsPoints sub_820E2A10, CockpitPoints sub_820DC190,
// InteriorPoints sub_820E4FD8, LightPoints sub_820DDCF8, TrafficPoints
// sub_820E68E8 and WheelPoints sub_820ECC48, WheelPoints::Reset sub_820E85C8,
// the wheels' group inits and resets (sub_820E8A80, sub_820E74B8, sub_820E99C0,
// sub_820E7918, sub_820EA8E8, sub_820E7C08, sub_820EB320, sub_820E7F90)
// (2026-11-01, their callers in play native); the destructors the deleting
// destructors call (~SceneCarModel sub_820DB720, ~SceneParts sub_820E2E10,
// ~SceneInterior sub_820E5178, ~SceneLights sub_820DDE00, ~SceneWheels
// sub_820ECDD0, ~SceneTraffic sub_820E69F0) and their point blocks'
// (sub_820E2C10, sub_820E50C0, sub_820DDD90, sub_820ECCB8, sub_820E6980)
// (2026-11-01, their callers native); ModelPoint::FindShape sub_820CEB30, the
// model's shape accessors (sub_82594A60, sub_82594A78, sub_82594A90,
// sub_82594AA8) and Morph::Run sub_82594D10 (2026-11-01, their callers in play
// native: the point set-ups and the wheels' set-ups).
ITC_SECOND_SET(820E0258, SecondSetPartsFind)
ITC_SECOND_SET(820DBC88, SecondSetCockpitFind)
ITC_SECOND_SET(820E3660, SecondSetInteriorFind)
ITC_SECOND_SET(820DC900, SecondSetLightsFind)
ITC_SECOND_SET(820E58C8, SecondSetTrafficFind)
ITC_SECOND_SET(820E00F0, SecondSetPartsPointsReset)
ITC_SECOND_SET(820DBE20, SecondSetSceneCockpitReset)
ITC_SECOND_SET(820DB6C0, SecondSetSceneCarModelRelease)
ITC_SECOND_SET(820DA108, SecondSetSceneModelBlockConstruct)
ITC_SECOND_SET(820D9F50, SecondSetSceneModelBlockReset)
ITC_SECOND_SET(82594C18, SecondSetMorphSetUp)
ITC_SECOND_SET(8258BD28, SecondSetMorphBlend)
ITC_SECOND_SET(820E6F28, SecondSetTireFillEntries)
ITC_SECOND_SET(820E7010, SecondSetTireReadEntry)
ITC_SECOND_SET(820E8668, SecondSetWheelPointsSetUp)
ITC_SECOND_SET(820DB7C8, SecondSetSceneCarModelDeletingDestruct)
ITC_SECOND_SET(820E2F78, SecondSetScenePartsDeletingDestruct)
ITC_SECOND_SET(820E53A0, SecondSetSceneInteriorDeletingDestruct)
ITC_SECOND_SET(820DE090, SecondSetSceneLightsDeletingDestruct)
ITC_SECOND_SET(820ECEB8, SecondSetSceneWheelsDeletingDestruct)
ITC_SECOND_SET(820E6ED8, SecondSetSceneTrafficDeletingDestruct)
ITC_SECOND_SET(820DB400, SecondSetPoolObjectDeletingDestruct)
ITC_SECOND_SET(820DC258, SecondSetCockpitPointsDestruct)
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
