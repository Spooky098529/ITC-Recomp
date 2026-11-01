// The second set's objects natively: adapters in front of src/second_set/
// (docs/functions/second-set.md), with the register steps the harness shares
// (hooks/second_set.h).

#include "hooks/second_set.h"

#include "base/flags.h"
#include "native/buffers.h"
#include "second_set/point_groups.h"
#include "second_set/points.h"

ITC_FLAG_DECLARE(bool, native_material_passes);

ITC_FLAG_BOOL(native_second_set, true, "ITC",
              "Run the second set of the car scene's objects (docs/functions/second-set.md: "
              "PointRecord::SetNode sub_820CE8D8 and SetShape sub_820CE900, with native memcpy "
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
              "sub_820DBAA0, InitBonnet sub_820DC0A8 and InitMirror sub_820DC0F0) natively instead of through the translated "
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

ITC_SECOND_SET(820CE8D8, SecondSetPointSetNode)
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
