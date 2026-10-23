// The AI drivers (src/ai_drivers/, docs/functions/ai-drivers.md) against
// the recompiled functions. Each case fills a block from the allocation
// layer with random words (mostly floats: NaN, infinite and zero ones among
// them): a driver at +0 (6,160 bytes: its path at +512, its skill at
// +6048), a vehicle at +0x2000, the inputs at +0x4000, an output at
// +0x4100, a route file at +0x4200 and the vehicle's box shape at +0x4300;
// then the fields the function reads set to the cases it tells apart. Both
// versions run on a copy of the same bytes; the block (in 256-byte pieces,
// the float outputs as floats), r3 and f1 (for the functions returning a
// float) are compared. The drivers' table ([0x826C6074]) and the course's
// route ([0x826C5B10]) are set for each case and put back after. The
// originals run with --native_ai_drivers off.

#include "harness.h"
#include "test_route.h"

#include "ai_drivers/ai_drivers.h"
#include "hooks/guest_call.h"
#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_ai_drivers);

namespace harness {

namespace {

namespace ai = ai_drivers;

// --- The register steps, as the adapters (src/hooks/ai_drivers.cpp) ---

#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeInitPath, ai::InitPath(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeOf, ctx.r3.u64 = ai::Of(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCarInput, ai::ClearCarInput(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearFollow, ai::ClearFollow(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearOtherIndexes, ai::ClearOtherIndexes(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCorners, ai::ClearCorners(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCornerKind, ai::ClearCornerKind(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetSkill, ai::ResetSkill(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeWrapAngle, ai::WrapAngle(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeTimeFromTicks, ctx.r3.u64 = ai::TimeFromTicks(ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetFlag2, ctx.r3.u64 = ai::SetFlag2(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeIsFree, ctx.r3.u64 = ai::IsFree(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSpeedOverDistance,
                ctx.f1.f64 = ai::SpeedOverDistance(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeCornerFactor,
                ctx.f1.f64 = ai::CornerFactor(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeReadCar, ctx.r3.u64 = ai::ReadCar(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateHalfWidth, ai::UpdateHalfWidth(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeTargetPoint,
                ctx.r3.u64 = ai::TargetPoint(base, ctx.r3.u32, ctx.f1.f64, ctx.r4.u32))
ITC_NATIVE_STEP(NativeClearSkill, ai::ClearSkill(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetSkillSource, ai::SetSkillSource(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeLoadSkill, ai::LoadSkill(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeLimitSpeed,
                ctx.f1.f64 = ai::LimitSpeed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                            ctx.r7.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeLimitByCurve, ai::LimitByCurve(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeEaseThrottleForSteer, ai::EaseThrottleForSteer(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeHasKind2Ahead, ctx.r3.u64 = ai::HasKind2Ahead(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeIsSlow, ctx.r3.u64 = ai::IsSlow(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePreferredSections,
                ctx.r3.s64 = int32_t(ai::PreferredSections(base, ctx.r3.u32, ctx.r4.u32)))
ITC_NATIVE_STEP(NativeRaceAllowsSides, ctx.r3.u64 = ai::RaceAllowsSides(base))
ITC_NATIVE_STEP(NativeRefreshPoints, ai::RefreshPoints(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeResetPath, ctx.r3.u64 = ai::ResetPath(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetLaneOffset, ai::SetLaneOffset(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateOtherFlags, ai::UpdateOtherFlags(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateOtherTimer,
                ai::UpdateOtherTimer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeClearOthers, ai::ClearOthers(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePickTarget, ai::PickTarget(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePickTargetBehind, ai::PickTargetBehind(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeKeepsGapFrom, ctx.r3.u64 = ai::KeepsGapFrom(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeSetParams, ai::SetParams(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 += 6048)
ITC_NATIVE_STEP(NativeSetSkill, ai::SetSkill(base, ctx.r3.u32, ctx.r4.u32); ctx.r3.u64 += 6048)
ITC_NATIVE_STEP(NativeClearPlan, ctx.r3.u64 = ai::ClearPlan(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeLimitSpeedBlend,
                ctx.f1.f64 = ai::LimitSpeedBlend(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32, ctx.r7.u32, ctx.r8.u32,
                                                 ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeSetMode, ctx.r3.u64 = ai::SetMode(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeResetFor, ctx.r3.u64 = ai::ResetFor(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeNormalizeByLane,
                ctx.r3.u64 = ai::NormalizeByLane(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                                 ctx.r7.u32))
ITC_NATIVE_STEP(NativeUpdateLane, ai::UpdateLane(base, ctx.r3.u32, ctx.r1.u32 - 128);
                ctx.r3.u64 = ctx.r1.u32 - 128 + 80)
ITC_NATIVE_STEP(NativeResetOthers, ctx.r3.u64 = ai::ResetOthers(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeClearCarState, ai::ClearCarState(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateState4, ai::UpdateState4(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeUpdateState5, ctx.r3.u64 = ai::UpdateState5(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeIsSettled, ctx.r3.u64 = ai::IsSettled(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeMirrorOther, ai::MirrorOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeBlockLeft, ai::BlockLeft(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r6.u32))
ITC_NATIVE_STEP(NativeBlockRight, ai::BlockRight(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32))
ITC_NATIVE_STEP(NativeInitCorner, ai::InitCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32))
ITC_NATIVE_STEP(NativeWithinRoadAt, ctx.r3.u64 = ai::WithinRoadAt(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeResetToMode1, ai::ResetToMode1(base, ctx.r3.u32, ctx.r1.u32 - 112);
                ctx.r3.u64 += 6048)
ITC_NATIVE_STEP(NativeResetToMode0, ai::ResetToMode0(base, ctx.r3.u32, ctx.r1.u32 - 128);
                ctx.r3.u64 += 6048)
ITC_NATIVE_STEP(NativeStartRace, ctx.r3.u64 = ai::StartRace(base, ctx.r3.u32, ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeResetFollow, ai::ResetFollow(base, ctx.r3.u32))
// The original where a copy can't be made natively (counted: none expected).
int g_fallbacks = 0;
void NativeAllSettled(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  if (ai::AllSettled(base, ctx.r1.u32 - 6320, &result)) {
    ctx.r3.u64 = result;
    return;
  }
  g_fallbacks++;
  __imp__sub_8213ADF8(ctx, base);
}
ITC_NATIVE_STEP(NativeAccumulateSteer, ai::AccumulateSteer(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeThrottleForSpeed, ai::ThrottleForSpeed(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeEaseThrottleForSlide,
                ai::EaseThrottleForSlide(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeLimitThrottle, ai::LimitThrottle(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeAdjustForOther,
                ai::AdjustForOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeUpdateAgainstPlayer, ctx.r3.u64 = ai::UpdateAgainstPlayer(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateOther, ai::UpdateOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeResetControl, ai::ResetControl(base, ctx.r3.u32, ctx.r1.u32 - 112);
                ctx.r3.u64 += 6048)
ITC_NATIVE_STEP(NativeResetDriver, ctx.r3.u64 = ai::ResetDriver(base, ctx.r3.u32))
void NativeUpdateSteerInput(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0;
  bool f1_set = false;
  ctx.r3.u64 = ai::UpdateSteerInput(base, ctx.r3.u32, ctx.r1.u32, &f1, &f1_set);
  if (f1_set) {
    ctx.f1.f64 = f1;
  }
}
ITC_NATIVE_STEP(NativeDropPassedSections, ai::DropPassedSections(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSharesSections,
                ctx.r3.u64 = ai::SharesSections(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeWiden, ai::Widen(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeNarrowToOther,
                ai::NarrowToOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
void NativeAdvanceCorners(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (ai::AdvanceCorners(base, ctx.r3.u32, ctx.r4.u32, &r3)) {
    ctx.r3.u64 = r3;
    return;
  }
  g_fallbacks++;
  __imp__sub_8213D020(ctx, base);
}

ITC_NATIVE_STEP(NativeRestart,
                ctx.r3.u64 = ai::Restart(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeResetAll, ctx.r3.u64 = ai::ResetAll(base, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeSetState2or3,
                ctx.r3.u64 = ai::SetState2or3(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                              ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeSetStateFromList,
                ctx.r3.u64 = ai::SetStateFromList(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                  ctx.r6.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeSetState4,
                ctx.r3.u64 = ai::SetState4(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeClearAll, ctx.r3.u64 = ai::ClearAll(base, ctx.r1.u32 - 128))

ITC_NATIVE_STEP(NativeJoinCorners,
                ctx.r3.u64 = ai::JoinCorners(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
ITC_NATIVE_STEP(NativeJoinCornersShort,
                ctx.r3.u64 = ai::JoinCornersShort(base, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32))
void RunJoinStep(PPCContext& ctx, uint8_t* base, uint32_t step, uint32_t frame,
                 uint32_t chain) {
  const uint32_t saved = ctx.r1.u32;
  guest::StoreU32(base, frame, chain);
  ctx.r1.u32 = frame;
  ctx.r6.u64 = frame + 80;
  if (step == 2) {
    sub_8213BDF0(ctx, base);
  } else {
    sub_8213BE80(ctx, base);
  }
  ctx.r1.u32 = saved;
}
void NativeJoinCornerPair(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  const uint32_t frame = ctx.r1.u32 - 112;
  const uint32_t step = ai::JoinCornerPair(base, ctx.r4.u32, ctx.r5.u32, frame, &r3);
  if (step == 1) {
    ctx.r3.u64 = r3;
  } else if (step >= 2) {
    RunJoinStep(ctx, base, step, frame, ctx.r1.u32);
  }
}
void NativeAddCornersAt(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t path = ctx.r3.u32;
  const uint32_t frame = ctx.r1.u32 - 144;
  uint32_t r3 = 0, a = 0, b = 0;
  const uint32_t step = ai::AddCornersAt(base, path, ctx.r4.u32, frame, &r3, &a, &b);
  if (step >= 2) {
    guest::StoreU32(base, frame, ctx.r1.u32);
    ctx.r3.u64 = path;
    ctx.r4.u64 = a;
    ctx.r5.u64 = b;
    RunJoinStep(ctx, base, step, frame - 112, frame);
    ai::AddCornersAtFinish(base, path);
    return;
  }
  ctx.r3.u64 = r3;
}

ITC_NATIVE_STEP(NativeUpdateState2,
                ctx.r3.u64 = ai::UpdateState2(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeUpdateState3,
                ctx.r3.u64 = ai::UpdateState3(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeUpdateState, ctx.r3.u64 = ai::UpdateState(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeUpdateState6,
                ctx.r3.u64 = ai::UpdateState6(base, ctx.r3.u32, ctx.r4.u32))

// The target speed's steps: f1 LimitSpeedBlend's when it ran.
#define ITC_NATIVE_F1_STEP(name, call)        \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    double f1 = 0;                            \
    bool f1_set = false;                      \
    ctx.r3.u64 = call;                        \
    if (f1_set) {                             \
      ctx.f1.f64 = f1;                        \
    }                                         \
  }
ITC_NATIVE_F1_STEP(NativeLimitForCorner,
                   ai::LimitForCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                      ctx.r1.u32 - 192, &f1, &f1_set))
ITC_NATIVE_F1_STEP(NativeUpdateTargetSpeed,
                   ai::UpdateTargetSpeed(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128, &f1,
                                         &f1_set))
ITC_NATIVE_F1_STEP(NativeUpdateSpeed,
                   ai::UpdateSpeed(base, ctx.r3.u32, ctx.r1.u32 - 96, &f1, &f1_set))

void NativeSteerToTarget(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0;
  ctx.r3.u64 = ai::SteerToTarget(base, ctx.r3.u32, &f1);
  ctx.f1.f64 = f1;
}
ITC_NATIVE_STEP(NativeUpdatePedals, ai::UpdatePedals(base, ctx.r3.u32, ctx.r1.u32 - 112))

void NativeCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint64_t r3 = 0;
  if (ai::Create(base, &r3)) {
    ctx.r3.u64 = r3;
    return;
  }
  g_fallbacks++;
  __imp__sub_8213B1E0(ctx, base);
}
ITC_NATIVE_STEP(NativeApplyCornerLine, ai::ApplyCornerLine(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeApplyCornerLines, ai::ApplyCornerLines(base, ctx.r3.u32, ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeKeepToCornerSide, ai::KeepToCornerSide(base, ctx.r3.u32, ctx.r4.u32,
                                                             ctx.r5.u32, ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeSetRanges,
                ctx.r3.u64 = ai::SetRanges(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeDropOffRouteCorners,
                ctx.r3.u64 = ai::DropOffRouteCorners(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSpreadOffset, ai::SpreadOffset(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeClampOffset, ai::ClampOffset(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32, ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeNarrowToOtherEx,
                ctx.r3.u64 = ai::NarrowToOtherEx(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32))
ITC_NATIVE_STEP(NativeNarrowBehind,
                ctx.r3.u64 = ai::NarrowBehind(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                              ctx.r6.u32, ctx.r7.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeCompareBranches,
                ctx.r3.s64 = ai::CompareBranches(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32, ctx.r7.u32, ctx.r8.u32))
ITC_NATIVE_STEP(NativeMeasureOthers,
                ctx.r3.u64 = ai::MeasureOthers(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeAvoidOnLeft, ctx.r3.u64 = ai::AvoidOnLeft(base, ctx.r3.u32, ctx.r4.u32,
                                                                ctx.r5.u32, ctx.r1.u32 - 176))
ITC_NATIVE_STEP(NativeAvoidOnRight, ctx.r3.u64 = ai::AvoidOnRight(base, ctx.r3.u32, ctx.r4.u32,
                                                                  ctx.r5.u32, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeChooseSideForOther,
                ctx.r3.u64 = ai::ChooseSideForOther(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                    ctx.r1.u32))
ITC_NATIVE_STEP(NativeAvoidByLane, ctx.r3.u64 = ai::AvoidByLane(base, ctx.r3.u32, ctx.r4.u32,
                                                                ctx.r5.u32, ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeAvoidOthers,
                ctx.r3.u64 = ai::AvoidOthers(base, ctx.r3.u32, ctx.r1.u32 - 272))
ITC_NATIVE_STEP(NativeSkillEntryWord, ctx.r3.u64 = ai::SkillEntryWord(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSkillEntryValue, ctx.f1.f64 = ai::SkillEntryValue(base, ctx.r3.u32))
#define ITC_NATIVE_TRIGGER_STEP(name, call)   \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    double f1 = 0;                            \
    bool f1_set = false;                      \
    ctx.r3.u64 = call;                        \
    if (f1_set) {                             \
      ctx.f1.f64 = f1;                        \
    }                                         \
  }
ITC_NATIVE_TRIGGER_STEP(NativeTriggerStarts,
                        ai::TriggerStarts(base, ctx.r4.u32, ctx.r5.u32, &f1, &f1_set))
ITC_NATIVE_TRIGGER_STEP(NativeTriggerEnds, ai::TriggerEnds(base, ctx.r3.u32, ctx.r4.u32,
                                                           ctx.r5.u32, &f1, &f1_set))
ITC_NATIVE_TRIGGER_STEP(NativeUpdateSkill,
                        ai::UpdateSkill(base, ctx.r3.u32, ctx.r4.u32, &f1, &f1_set))
ITC_NATIVE_STEP(NativeLimitSlide, ai::LimitSlide(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeSteerByRoom,
                ctx.r3.u64 = ai::SteerByRoom(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeSteerOutOfCorner,
                ctx.r3.u64 = ai::SteerOutOfCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96))
ITC_NATIVE_TRIGGER_STEP(NativeSteerAtCornerPoint,
                        ai::SteerAtCornerPoint(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r1.u32 - 128, &f1, &f1_set))
ITC_NATIVE_TRIGGER_STEP(NativeSteerIntoCorner,
                        ai::SteerIntoCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96, &f1,
                                            &f1_set))
ITC_NATIVE_TRIGGER_STEP(NativeUpdateSteering,
                        ai::UpdateSteering(base, ctx.r3.u32, ctx.r1.u32 - 112, &f1, &f1_set))
void NativeDrive(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  double f1 = 0;
  ctx.r3.u64 = ai::Drive(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 160, &f1);
  ctx.f1.f64 = f1;
}
ITC_NATIVE_STEP(NativeNormalizeByBranch,
                ctx.r3.u64 = ai::NormalizeByBranch(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32, ctx.r7.u32))

ITC_NATIVE_STEP(NativeContinueLine, ai::ContinueLine(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r1.u32 - 112))
ITC_NATIVE_STEP(NativeApproachCorner,
                ctx.r3.u64 = ai::ApproachCorner(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                ctx.r6.u32, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeCornerToEntry,
                ctx.r3.u64 = ai::CornerToEntry(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r6.u32, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeCornerToApex,
                ctx.r3.u64 = ai::CornerToApex(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                              ctx.r6.u32, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeCornerToExit,
                ctx.r3.u64 = ai::CornerToExit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                              ctx.r6.u32, ctx.r1.u32 - 160))

ITC_NATIVE_STEP(NativePlanCorners,
                ctx.r3.u64 = ai::PlanCorners(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                             ctx.r6.u32, ctx.r1.u32 - 144))
ITC_NATIVE_STEP(NativePlanLine, ctx.r3.u64 = ai::PlanLine(base, ctx.r3.u32, ctx.r1.u32 - 96))

ITC_NATIVE_STEP(NativeBranchMasksFor,
                ctx.r3.s64 = int32_t(ai::BranchMasksFor(base, ctx.r3.u32)))
ITC_NATIVE_STEP(NativeLeaderBranchKind,
                ctx.r3.u64 = ai::LeaderBranchKind(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeFindSideSection,
                ctx.r3.u64 = ai::FindSideSection(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                 ctx.r6.u32, ctx.r1.u32 - 208))
ITC_NATIVE_STEP(NativeFollowTargets, ai::FollowTargets(base, ctx.r3.u32, ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeFollowLeaderBranch, ctx.r3.u64 = ai::FollowLeaderBranch(base, ctx.r3.u32))

ITC_NATIVE_STEP(NativeCompareWith,
                ctx.r3.u64 = ai::CompareWith(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32 - 144))
// UpdateAll's drivers' updates as the adapter makes them (recompiled code
// through +6148, from a frame like the original's).
struct UpdateCall {
  PPCContext* ctx;
  uint8_t* base;
};
uint64_t CallDriverUpdate(void* user, uint64_t driver, uint32_t function) {
  auto& call = *static_cast<UpdateCall*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  guest_call::InFrame(ctx, base, 176, 0x82139684, [&](uint32_t) {
    ctx.r3.u64 = driver;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
  });
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}
void NativeUpdateAll(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  UpdateCall call{&ctx, base};
  ctx.r3.u64 = ai::UpdateAll(base, ctx.r1.u32 - 176, CallDriverUpdate, &call);
}

ITC_NATIVE_STEP(NativePlanReaction, ai::PlanReaction(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r7.u32))
ITC_NATIVE_STEP(NativeReactToOthers,
                ctx.r3.u64 = ai::ReactToOthers(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 240))
ITC_NATIVE_STEP(NativeStartGroup, ctx.r3.u64 = ai::StartGroup(base, ctx.r1.u32 - 160))
ITC_NATIVE_STEP(NativeStartRanked, ctx.r3.u64 = ai::StartRanked(base, ctx.r1.u32 - 224))
ITC_NATIVE_STEP(NativeReadVehicle, ctx.r3.u64 = ai::ReadVehicle(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeReadVehicleOwnSlice, ctx.r3.u64 = ai::ReadVehicleOwnSlice(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeBranchMasks,
                ctx.r3.u64 = ai::BranchMasks(base, ctx.r3.u32, ctx.r1.u32 - 96))
ITC_NATIVE_STEP(NativeKeepBranch,
                ctx.r3.u64 = ai::KeepBranch(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                            ctx.r1.u32 - 128))
ITC_NATIVE_STEP(NativeLeftEntry, ctx.r3.u64 = ai::LeftEntry(base, ctx.r3.u32, ctx.r4.u32,
                                                            ctx.r5.u32, ctx.r1.u32 - 160))
// The path's update's calls on the translated code, as the adapters make them.
struct PathCall {
  PPCContext* ctx;
  uint8_t* base;
};
uint64_t CallNormalizer(void* user, uint32_t function, uint32_t frame, uint32_t path,
                        uint32_t entry, uint32_t position, uint32_t lane, uint64_t masks) {
  auto& call = *static_cast<PathCall*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  const uint64_t saved = ctx.r1.u64;
  guest::StoreU32(base, frame, frame + 208);
  ctx.r1.u64 = frame;
  ctx.lr = 0x8213EEDC;
  ctx.r3.u64 = path;
  ctx.r4.u64 = entry;
  ctx.r5.u64 = position;
  ctx.r6.u64 = lane;
  ctx.r7.u64 = masks;
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = saved;
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}
uint64_t CallJoinStep(void* user, uint32_t step, uint32_t frame, uint32_t path, uint32_t a,
                      uint32_t b) {
  auto& call = *static_cast<PathCall*>(user);
  PPCContext& ctx = *call.ctx;
  uint8_t* base = call.base;
  guest::StoreU32(base, frame, frame + 144);
  ctx.r3.u64 = path;
  ctx.r4.u64 = a;
  ctx.r5.u64 = b;
  RunJoinStep(ctx, base, step, frame - 112, frame);
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}
void NativeExtend(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  PathCall call{&ctx, base};
  const ai::PathCalls calls{&call, CallNormalizer, CallJoinStep};
  ctx.r3.u64 = ai::Extend(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 208, calls);
}
void NativeUpdatePath(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  PathCall call{&ctx, base};
  const ai::PathCalls calls{&call, CallNormalizer, CallJoinStep};
  uint64_t r3 = 0;
  if (ai::UpdatePath(base, ctx.r3.u32, ctx.r1.u32 - 144, calls, &r3)) {
    ctx.r3.u64 = r3;
    return;
  }
  g_fallbacks++;
  __imp__sub_8213F530(ctx, base);
}
void NativeDropPassed(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint64_t r3 = 0;
  if (ai::DropPassed(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 160, &r3)) {
    ctx.r3.u64 = r3;
    return;
  }
  g_fallbacks++;
  __imp__sub_8213F2E0(ctx, base);
}

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_ai_drivers);
  ITC_FLAG(native_ai_drivers) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_ai_drivers) = saved;
}

// The block's layout.
constexpr uint32_t kAreaBytes = 0x4600;
constexpr uint32_t kDriver = 0, kPath = 512, kSkill = 6048;
constexpr uint32_t kVehicle = 0x2000, kIo = 0x4000, kOut = 0x4100, kFile = 0x4200,
                   kShape = 0x4300, kRace = 0x4380, kRaceState = 0x4390, kInputWords = 0x43C0;
// The race data ([0x826C5B2C], its words +0xA3AF0 and +0xA3AF4 at kRace),
// the race state ([0x826D82E8], its +6100 at kRaceState) and the racers'
// inputs ([0x826C5B30], eight words pointing at kInputWords).
constexpr uint32_t kRaceData = 0x826C5B2C, kRaceStateGlobal = 0x826D82E8, kInputs = 0x826C5B30;
// The race ([0x826D82F4]): its racers' list (+2308) at kRacers, its count
// (+2364) at kRacers + 56, the racers' records (+0 flags, +20 the index, 24
// bytes each) at kRacerRecords. The vehicles ([0x826C2E34], 6,272 bytes each)
// in a block of their own.
constexpr uint32_t kRacers = 0x4400, kRacerRecords = 0x4480;
constexpr uint32_t kRaceGlobal = 0x826D82F4, kVehiclesGlobal = 0x826C2E34, kVehicleBytes = 6272;
// The drivers' table for Of and KeepsGapFrom: 64 drivers.
constexpr uint32_t kTableDrivers = 64;

// A float: now and then NaN (any sign, payload), an infinity or a zero.
uint32_t RandomFloat(Random& r, float range) {
  switch (r.Below(32)) {
    case 0:
      return 0x7F800001u | (uint32_t(r.Next()) & 0x803FFFFFu) | 0x00400000u;
    case 1:
      return r.Below(2) ? 0x7F800000u : 0xFF800000u;
    case 2:
      return r.Below(2) ? 0u : 0x80000000u;
    default: {
      const float unit = float(r.Below(1u << 24)) / float(1u << 24);
      return std::bit_cast<uint32_t>((unit * 2.0f - 1.0f) * range);
    }
  }
}
// A word of the block's fill from one random number: a quarter raw bits,
// else a float as RandomFloat makes them (within 200.0; now and then NaN,
// an infinity or a zero), cheaper than it.
uint32_t FillWord(uint64_t x) {
  if ((x & 3) == 0) {
    return uint32_t(x >> 32);
  }
  switch ((x >> 2) & 31) {
    case 0:
      return 0x7FC00001u | (uint32_t(x >> 32) & 0x803FFFFFu);
    case 1:
      return (x >> 40) & 1 ? 0x7F800000u : 0xFF800000u;
    case 2:
      return (x >> 40) & 1 ? 0u : 0x80000000u;
    default: {
      const float unit = float(uint32_t(x >> 40)) / float(1u << 24);
      return std::bit_cast<uint32_t>((unit * 2.0f - 1.0f) * 200.0f);
    }
  }
}
uint32_t FloatBits(float value) {
  return std::bit_cast<uint32_t>(value);
}

enum Kind {
  kInitPath,
  kOf,
  kClearCarInput,
  kClearFollow,
  kClearOtherIndexes,
  kClearCorners,
  kClearCornerKind,
  kResetSkill,
  kWrapAngle,
  kTimeFromTicks,
  kSetFlag2,
  kIsFree,
  kSpeedOverDistance,
  kCornerFactor,
  kReadCar,
  kUpdateHalfWidth,
  kTargetPoint,
  kClearSkill,
  kSetSkillSource,
  kLoadSkill,
  kLimitSpeed,
  kLimitByCurve,
  kEaseThrottleForSteer,
  kHasKind2Ahead,
  kIsSlow,
  kPreferredSections,
  kRaceAllowsSides,
  kRefreshPoints,
  kResetPath,
  kSetLaneOffset,
  kUpdateOtherFlags,
  kUpdateOtherTimer,
  kClearOthers,
  kPickTarget,
  kPickTargetBehind,
  kKeepsGapFrom,
  kSetParams,
  kSetSkill,
  kClearPlan,
  kLimitSpeedBlend,
  kSetMode,
  kResetFor,
  kNormalizeByLane,
  kUpdateLane,
  kResetOthers,
  kClearCarState,
  kUpdateState4,
  kUpdateState5,
  kIsSettled,
  kMirrorOther,
  kBlockLeft,
  kBlockRight,
  kInitCorner,
  kWithinRoadAt,
  kResetToMode1,
  kResetToMode0,
  kStartRace,
  kResetFollow,
  kAllSettled,
  kAccumulateSteer,
  kThrottleForSpeed,
  kEaseThrottleForSlide,
  kLimitThrottle,
  kAdjustForOther,
  kUpdateAgainstPlayer,
  kUpdateOther,
  kResetControl,
  kResetDriver,
  kUpdateSteerInput,
  kDropPassedSections,
  kSharesSections,
  kWiden,
  kNarrowToOther,
  kAdvanceCorners,
  kRestart,
  kResetAll,
  kSetState2or3,
  kSetStateFromList,
  kSetState4,
  kClearAll,
  kJoinCorners,
  kJoinCornersShort,
  kJoinCornerPair,
  kAddCornersAt,
  kUpdateState2,
  kUpdateState3,
  kUpdateState,
  kUpdateState6,
  kLimitForCorner,
  kUpdateTargetSpeed,
  kUpdateSpeed,
  kSteerToTarget,
  kUpdatePedals,
  kApplyCornerLine,
  kApplyCornerLines,
  kKeepToCornerSide,
  kSetRanges,
  kDropOffRouteCorners,
  kSpreadOffset,
  kClampOffset,
  kNarrowToOtherEx,
  kNarrowBehind,
  kCompareBranches,
  kNormalizeByBranch,
  kMeasureOthers,
  kAvoidOnLeft,
  kAvoidOnRight,
  kChooseSideForOther,
  kAvoidByLane,
  kAvoidOthers,
  kSkillEntryWord,
  kSkillEntryValue,
  kTriggerStarts,
  kTriggerEnds,
  kUpdateSkill,
  kLimitSlide,
  kSteerByRoom,
  kSteerOutOfCorner,
  kSteerAtCornerPoint,
  kSteerIntoCorner,
  kUpdateSteering,
  kDrive,
  kContinueLine,
  kApproachCorner,
  kCornerToEntry,
  kCornerToApex,
  kCornerToExit,
  kPlanCorners,
  kPlanLine,
  kBranchMasksFor,
  kLeaderBranchKind,
  kFindSideSection,
  kFollowTargets,
  kFollowLeaderBranch,
  kCompareWith,
  kUpdateAll,
  kPlanReaction,
  kReactToOthers,
  kStartGroup,
  kStartRanked,
  kReadVehicle,
  kReadVehicleOwnSlice,
  kBranchMasks,
  kKeepBranch,
  kLeftEntry,
  kDropPassed,
  kExtend,
  kUpdatePath,
};

}  // namespace

void AiDrivers(Context& h) {
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
    int cases;
  } functions[] = {
      {"AiDriver::InitPath (sub_8213AD48)", Original<__imp__sub_8213AD48>, NativeInitPath,
       kInitPath, 200},
      {"AiDriver::Of (sub_82139728)", Original<__imp__sub_82139728>, NativeOf, kOf, 1000},
      {"CarInput::Clear (sub_820C96E8)", Original<__imp__sub_820C96E8>, NativeClearCarInput,
       kClearCarInput, 200},
      {"AiDriver::ClearFollow (sub_82146508)", Original<__imp__sub_82146508>,
       NativeClearFollow, kClearFollow, 200},
      {"AiDriver::ClearOtherIndexes (sub_82144ED8)", Original<__imp__sub_82144ED8>,
       NativeClearOtherIndexes, kClearOtherIndexes, 200},
      {"AiPath::ClearCorners (sub_8213C720)", Original<__imp__sub_8213C720>,
       NativeClearCorners, kClearCorners, 200},
      {"AiPath::ClearCornerKind (sub_8213C710)", Original<__imp__sub_8213C710>,
       NativeClearCornerKind, kClearCornerKind, 200},
      {"AiSkill::Reset (sub_8213F698)", Original<__imp__sub_8213F698>, NativeResetSkill,
       kResetSkill, 200},
      {"Angle::Wrap (sub_82145408)", Original<__imp__sub_82145408>, NativeWrapAngle,
       kWrapAngle, 3000},
      {"RaceTime::FromTicks (sub_82101760)", Original<__imp__sub_82101760>,
       NativeTimeFromTicks, kTimeFromTicks, 3000},
      {"AiDriver::SetFlag2 (sub_82138D20)", Original<__imp__sub_82138D20>, NativeSetFlag2,
       kSetFlag2, 1000},
      {"AiDriver::IsFree (sub_821464B8)", Original<__imp__sub_821464B8>, NativeIsFree, kIsFree,
       2000},
      {"AiDriver::SpeedOverDistance (sub_821450E0)", Original<__imp__sub_821450E0>,
       NativeSpeedOverDistance, kSpeedOverDistance, 3000},
      {"AiDriver::CornerFactor (sub_821453A0)", Original<__imp__sub_821453A0>,
       NativeCornerFactor, kCornerFactor, 3000},
      {"AiDriver::ReadCar (sub_82144F50)", Original<__imp__sub_82144F50>, NativeReadCar,
       kReadCar, 3000},
      {"AiDriver::UpdateHalfWidth (sub_82145230)", Original<__imp__sub_82145230>,
       NativeUpdateHalfWidth, kUpdateHalfWidth, 3000},
      {"AiPath::TargetPoint (sub_8213B4D8)", Original<__imp__sub_8213B4D8>, NativeTargetPoint,
       kTargetPoint, 3000},
      {"AiSkill::Clear (sub_8213F700)", Original<__imp__sub_8213F700>, NativeClearSkill,
       kClearSkill, 200},
      {"AiSkill::SetSource (sub_8213F900)", Original<__imp__sub_8213F900>,
       NativeSetSkillSource, kSetSkillSource, 500},
      {"AiSkill::Load (sub_8213F730)", Original<__imp__sub_8213F730>, NativeLoadSkill,
       kLoadSkill, 1000},
      {"AiDriver::LimitSpeed (sub_82139248)", Original<__imp__sub_82139248>, NativeLimitSpeed,
       kLimitSpeed, 3000},
      {"AiDriver::LimitByCurve (sub_821390D0)", Original<__imp__sub_821390D0>,
       NativeLimitByCurve, kLimitByCurve, 3000},
      {"AiDriver::EaseThrottleForSteer (sub_82139B98)", Original<__imp__sub_82139B98>,
       NativeEaseThrottleForSteer, kEaseThrottleForSteer, 3000},
      {"AiPath::HasKind2Ahead (sub_8213C148)", Original<__imp__sub_8213C148>,
       NativeHasKind2Ahead, kHasKind2Ahead, 2000},
      {"AiPath::IsSlow (sub_8213C0E8)", Original<__imp__sub_8213C0E8>, NativeIsSlow, kIsSlow,
       2000},
      {"AiPath::PreferredSections (sub_8213C358)", Original<__imp__sub_8213C358>,
       NativePreferredSections, kPreferredSections, 2000},
      {"AiPath::RaceAllowsSides (sub_8213D318)", Original<__imp__sub_8213D318>,
       NativeRaceAllowsSides, kRaceAllowsSides, 1000},
      {"AiPath::RefreshPoints (sub_8213B5D8)", Original<__imp__sub_8213B5D8>,
       NativeRefreshPoints, kRefreshPoints, 2000},
      {"AiPath::Reset (sub_8213D438)", Original<__imp__sub_8213D438>, NativeResetPath,
       kResetPath, 300},
      {"AiPathEntry::SetLaneOffset (sub_8213B390)", Original<__imp__sub_8213B390>,
       NativeSetLaneOffset, kSetLaneOffset, 3000},
      {"AiOther::UpdateFlags (sub_82145450)", Original<__imp__sub_82145450>,
       NativeUpdateOtherFlags, kUpdateOtherFlags, 2000},
      {"AiOther::UpdateTimer (sub_821456A0)", Original<__imp__sub_821456A0>,
       NativeUpdateOtherTimer, kUpdateOtherTimer, 3000},
      {"AiDriver::ClearOthers (sub_821459B0)", Original<__imp__sub_821459B0>,
       NativeClearOthers, kClearOthers, 200},
      {"AiDriver::PickTarget (sub_82145118)", Original<__imp__sub_82145118>, NativePickTarget,
       kPickTarget, 3000},
      {"AiDriver::PickTargetBehind (sub_821451B8)", Original<__imp__sub_821451B8>,
       NativePickTargetBehind, kPickTargetBehind, 3000},
      {"AiDriver::KeepsGapFrom (sub_82145048)", Original<__imp__sub_82145048>,
       NativeKeepsGapFrom, kKeepsGapFrom, 3000},
      {"AiDriver::SetParams (sub_82138D00)", Original<__imp__sub_82138D00>, NativeSetParams,
       kSetParams, 500},
      {"AiDriver::SetSkill (sub_821393D8)", Original<__imp__sub_821393D8>, NativeSetSkill,
       kSetSkill, 1000},
      {"AiDriver::ClearPlan (sub_82139820)", Original<__imp__sub_82139820>, NativeClearPlan,
       kClearPlan, 500},
      {"AiDriver::LimitSpeedBlend (sub_82139B40)", Original<__imp__sub_82139B40>,
       NativeLimitSpeedBlend, kLimitSpeedBlend, 3000},
      {"AiPath::SetMode (sub_8213D508)", Original<__imp__sub_8213D508>, NativeSetMode,
       kSetMode, 500},
      {"AiPath::ResetFor (sub_8213E658)", Original<__imp__sub_8213E658>, NativeResetFor,
       kResetFor, 300},
      {"AiPath::NormalizeByLane (sub_8213B428)", Original<__imp__sub_8213B428>,
       NativeNormalizeByLane, kNormalizeByLane, 3000},
      {"AiDriver::UpdateLane (sub_82144EF8)", Original<__imp__sub_82144EF8>, NativeUpdateLane,
       kUpdateLane, 3000},
      {"AiDriver::ResetOthers (sub_82145A48)", Original<__imp__sub_82145A48>,
       NativeResetOthers, kResetOthers, 300},
      {"AiDriver::ClearCarState (sub_82146588)", Original<__imp__sub_82146588>,
       NativeClearCarState, kClearCarState, 200},
      {"AiDriver::UpdateState4 (sub_82146220)", Original<__imp__sub_82146220>,
       NativeUpdateState4, kUpdateState4, 3000},
      {"AiDriver::UpdateState5 (sub_82146388)", Original<__imp__sub_82146388>,
       NativeUpdateState5, kUpdateState5, 3000},
      {"AiDriver::IsSettled (sub_82146408)", Original<__imp__sub_82146408>, NativeIsSettled,
       kIsSettled, 3000},
      {"AiOther::Mirror (sub_82145610)", Original<__imp__sub_82145610>, NativeMirrorOther,
       kMirrorOther, 2000},
      {"AiPathEntry::BlockLeft (sub_8213C5C8)", Original<__imp__sub_8213C5C8>, NativeBlockLeft,
       kBlockLeft, 3000},
      {"AiPathEntry::BlockRight (sub_8213C630)", Original<__imp__sub_8213C630>,
       NativeBlockRight, kBlockRight, 3000},
      {"AiCorner::Init (sub_8213C698)", Original<__imp__sub_8213C698>, NativeInitCorner,
       kInitCorner, 2000},
      {"AiPath::WithinRoadAt (sub_8213C1A0)", Original<__imp__sub_8213C1A0>,
       NativeWithinRoadAt, kWithinRoadAt, 3000},
      {"AiDriver::ResetToMode1 (sub_82139450)", Original<__imp__sub_82139450>,
       NativeResetToMode1, kResetToMode1, 500},
      {"AiDriver::ResetToMode0 (sub_821394F0)", Original<__imp__sub_821394F0>,
       NativeResetToMode0, kResetToMode0, 500},
      {"AiDriver::StartRace (sub_82139DA0)", Original<__imp__sub_82139DA0>, NativeStartRace,
       kStartRace, 500},
      {"AiDriver::ResetFollow (sub_821467B8)", Original<__imp__sub_821467B8>,
       NativeResetFollow, kResetFollow, 200},
      {"AiDrivers::AllSettled (sub_8213ADF8)", Original<__imp__sub_8213ADF8>, NativeAllSettled,
       kAllSettled, 1000},
      {"AiDriver::AccumulateSteer (sub_82138F00)", Original<__imp__sub_82138F00>,
       NativeAccumulateSteer, kAccumulateSteer, 3000},
      {"AiDriver::ThrottleForSpeed (sub_82139168)", Original<__imp__sub_82139168>,
       NativeThrottleForSpeed, kThrottleForSpeed, 3000},
      {"AiDriver::EaseThrottleForSlide (sub_82139BE8)", Original<__imp__sub_82139BE8>,
       NativeEaseThrottleForSlide, kEaseThrottleForSlide, 3000},
      {"AiDriver::LimitThrottle (sub_82139CB0)", Original<__imp__sub_82139CB0>,
       NativeLimitThrottle, kLimitThrottle, 3000},
      {"AiDriver::AdjustForOther (sub_821452A0)", Original<__imp__sub_821452A0>,
       NativeAdjustForOther, kAdjustForOther, 3000},
      {"AiDriver::UpdateAgainstPlayer (sub_82146608)", Original<__imp__sub_82146608>,
       NativeUpdateAgainstPlayer, kUpdateAgainstPlayer, 3000},
      {"AiOther::Update (sub_821454C8)", Original<__imp__sub_821454C8>, NativeUpdateOther,
       kUpdateOther, 3000},
      {"AiDriver::ResetControl (sub_82139580)", Original<__imp__sub_82139580>,
       NativeResetControl, kResetControl, 500},
      {"AiDriver::Reset (sub_8213AAA8)", Original<__imp__sub_8213AAA8>, NativeResetDriver,
       kResetDriver, 300},
      {"AiDriver::UpdateSteerInput (sub_8213AC70)", Original<__imp__sub_8213AC70>,
       NativeUpdateSteerInput, kUpdateSteerInput, 3000},
      {"AiPath::DropPassedSections (sub_8213BA48)", Original<__imp__sub_8213BA48>,
       NativeDropPassedSections, kDropPassedSections, 3000},
      {"AiPath::SharesSections (sub_8213C010)", Original<__imp__sub_8213C010>,
       NativeSharesSections, kSharesSections, 3000},
      {"AiPathEntry::Widen (sub_8213C3A8)", Original<__imp__sub_8213C3A8>, NativeWiden, kWiden,
       3000},
      {"AiPathEntry::NarrowToOther (sub_8213C420)", Original<__imp__sub_8213C420>,
       NativeNarrowToOther, kNarrowToOther, 3000},
      {"AiPath::AdvanceCorners (sub_8213D020)", Original<__imp__sub_8213D020>,
       NativeAdvanceCorners, kAdvanceCorners, 3000},
      {"AiDriver::Restart (sub_821395D0)", Original<__imp__sub_821395D0>, NativeRestart,
       kRestart, 300},
      {"AiDrivers::ResetAll (sub_82139750)", Original<__imp__sub_82139750>, NativeResetAll,
       kResetAll, 300},
      {"AiDriver::SetState2or3 (sub_82139E40)", Original<__imp__sub_82139E40>,
       NativeSetState2or3, kSetState2or3, 300},
      {"AiDriver::SetStateFromList (sub_82139EA8)", Original<__imp__sub_82139EA8>,
       NativeSetStateFromList, kSetStateFromList, 1000},
      {"AiDriver::SetState4 (sub_82139FA0)", Original<__imp__sub_82139FA0>, NativeSetState4,
       kSetState4, 300},
      {"AiDrivers::ClearAll (sub_8213A140)", Original<__imp__sub_8213A140>, NativeClearAll,
       kClearAll, 300},
      {"AiPath::JoinCorners (sub_8213BCD0)", Original<__imp__sub_8213BCD0>, NativeJoinCorners,
       kJoinCorners, 3000},
      {"AiPath::JoinCornersShort (sub_8213BF28)", Original<__imp__sub_8213BF28>,
       NativeJoinCornersShort, kJoinCornersShort, 3000},
      {"AiPath::JoinCornerPair (sub_8213DD40)", Original<__imp__sub_8213DD40>,
       NativeJoinCornerPair, kJoinCornerPair, 3000},
      {"AiPath::AddCornersAt (sub_8213E970)", Original<__imp__sub_8213E970>,
       NativeAddCornersAt, kAddCornersAt, 3000},
      {"AiDriver::UpdateState2 (sub_82145F48)", Original<__imp__sub_82145F48>,
       NativeUpdateState2, kUpdateState2, 3000},
      {"AiDriver::UpdateState3 (sub_821460C8)", Original<__imp__sub_821460C8>,
       NativeUpdateState3, kUpdateState3, 3000},
      {"AiDriver::UpdateState (sub_82139AC8)", Original<__imp__sub_82139AC8>, NativeUpdateState,
       kUpdateState, 3000},
      {"AiDriver::UpdateState6 (sub_821462A0, never seen in play)", Original<__imp__sub_821462A0>,
       NativeUpdateState6, kUpdateState6, 3000},
      {"AiDriver::LimitForCorner (sub_8213A620)", Original<__imp__sub_8213A620>,
       NativeLimitForCorner, kLimitForCorner, 3000},
      {"AiDriver::UpdateTargetSpeed (sub_8213AB98)", Original<__imp__sub_8213AB98>,
       NativeUpdateTargetSpeed, kUpdateTargetSpeed, 3000},
      {"AiDriver::UpdateSpeed (sub_8213ACD0)", Original<__imp__sub_8213ACD0>, NativeUpdateSpeed,
       kUpdateSpeed, 3000},
      {"AiDriver::SteerToTarget (sub_82138D78)", Original<__imp__sub_82138D78>,
       NativeSteerToTarget, kSteerToTarget, 3000},
      {"AiDriver::UpdatePedals (sub_8213A428)", Original<__imp__sub_8213A428>,
       NativeUpdatePedals, kUpdatePedals, 3000},
      {"AiPath::ApplyCornerLine (sub_8213BB58)", Original<__imp__sub_8213BB58>,
       NativeApplyCornerLine, kApplyCornerLine, 3000},
      {"AiPath::ApplyCornerLines (sub_8213CFC8)", Original<__imp__sub_8213CFC8>,
       NativeApplyCornerLines, kApplyCornerLines, 3000},
      {"AiPath::KeepToCornerSide (sub_8213CA18)", Original<__imp__sub_8213CA18>,
       NativeKeepToCornerSide, kKeepToCornerSide, 3000},
      {"AiPath::SetRanges (sub_8213D670)", Original<__imp__sub_8213D670>, NativeSetRanges,
       kSetRanges, 3000},
      {"AiPath::DropOffRouteCorners (sub_8213DC08)", Original<__imp__sub_8213DC08>,
       NativeDropOffRouteCorners, kDropOffRouteCorners, 3000},
      {"AiPath::SpreadOffset (sub_8213B890)", Original<__imp__sub_8213B890>, NativeSpreadOffset,
       kSpreadOffset, 3000},
      {"AiPath::ClampOffset (sub_8213CF58)", Original<__imp__sub_8213CF58>, NativeClampOffset,
       kClampOffset, 3000},
      {"AiPathEntry::NarrowToOtherEx (sub_8213C4C0)", Original<__imp__sub_8213C4C0>,
       NativeNarrowToOtherEx, kNarrowToOtherEx, 3000},
      {"AiPath::NarrowBehind (sub_8213D140)", Original<__imp__sub_8213D140>, NativeNarrowBehind,
       kNarrowBehind, 3000},
      {"AiPath::CompareBranches (sub_8213C788)", Original<__imp__sub_8213C788>,
       NativeCompareBranches, kCompareBranches, 3000},
      {"AiPath::NormalizeByBranch (sub_8213C908)", Original<__imp__sub_8213C908>,
       NativeNormalizeByBranch, kNormalizeByBranch, 3000},
      {"AiPath::MeasureOthers (sub_8213B650)", Original<__imp__sub_8213B650>,
       NativeMeasureOthers, kMeasureOthers, 3000},
      {"AiPath::AvoidOnLeft (sub_8213CB30)", Original<__imp__sub_8213CB30>, NativeAvoidOnLeft,
       kAvoidOnLeft, 3000},
      {"AiPath::AvoidOnRight (sub_8213CD50)", Original<__imp__sub_8213CD50>, NativeAvoidOnRight,
       kAvoidOnRight, 3000},
      {"AiPath::ChooseSideForOther (sub_8213D788)", Original<__imp__sub_8213D788>,
       NativeChooseSideForOther, kChooseSideForOther, 3000},
      {"AiPath::AvoidByLane (sub_8213D898, never seen in play)", Original<__imp__sub_8213D898>,
       NativeAvoidByLane, kAvoidByLane, 3000},
      {"AiPath::AvoidOthers (sub_8213E690)", Original<__imp__sub_8213E690>, NativeAvoidOthers,
       kAvoidOthers, 3000},
      {"AiSkill::EntryWord (sub_8213F7E0, never seen in play)", Original<__imp__sub_8213F7E0>,
       NativeSkillEntryWord, kSkillEntryWord, 2000},
      {"AiSkill::EntryValue (sub_8213F868, never seen in play)", Original<__imp__sub_8213F868>,
       NativeSkillEntryValue, kSkillEntryValue, 2000},
      {"AiSkill::TriggerStarts (sub_8213F958)", Original<__imp__sub_8213F958>,
       NativeTriggerStarts, kTriggerStarts, 3000},
      {"AiSkill::TriggerEnds (sub_8213FB48)", Original<__imp__sub_8213FB48>, NativeTriggerEnds,
       kTriggerEnds, 3000},
      {"AiSkill::Update (sub_8213FCF8)", Original<__imp__sub_8213FCF8>, NativeUpdateSkill,
       kUpdateSkill, 3000},
      {"AiDriver::LimitSlide (sub_821392D8, never seen in play)", Original<__imp__sub_821392D8>,
       NativeLimitSlide, kLimitSlide, 3000},
      {"AiDriver::SteerByRoom (sub_82138FA8, never seen in play)", Original<__imp__sub_82138FA8>,
       NativeSteerByRoom, kSteerByRoom, 3000},
      {"AiDriver::SteerOutOfCorner (sub_82139A08, never seen in play)",
       Original<__imp__sub_82139A08>, NativeSteerOutOfCorner, kSteerOutOfCorner, 3000},
      {"AiDriver::SteerAtCornerPoint (sub_8213A200, never seen in play)",
       Original<__imp__sub_8213A200>, NativeSteerAtCornerPoint, kSteerAtCornerPoint, 3000},
      {"AiDriver::SteerIntoCorner (sub_8213AB40, never seen in play)",
       Original<__imp__sub_8213AB40>, NativeSteerIntoCorner, kSteerIntoCorner, 3000},
      {"AiDriver::UpdateSteering (sub_8213AEF0)", Original<__imp__sub_8213AEF0>,
       NativeUpdateSteering, kUpdateSteering, 3000},
      {"AiDriver::Drive (sub_8213B058)", Original<__imp__sub_8213B058>, NativeDrive, kDrive,
       3000},
      {"AiPath::ContinueLine (sub_8213DB08)", Original<__imp__sub_8213DB08>, NativeContinueLine,
       kContinueLine, 2000},
      {"AiPath::ApproachCorner (sub_8213DE30)", Original<__imp__sub_8213DE30>,
       NativeApproachCorner, kApproachCorner, 2000},
      {"AiPath::CornerToEntry (sub_8213E018)", Original<__imp__sub_8213E018>,
       NativeCornerToEntry, kCornerToEntry, 2000},
      {"AiPath::CornerToApex (sub_8213E1D0)", Original<__imp__sub_8213E1D0>, NativeCornerToApex,
       kCornerToApex, 2000},
      {"AiPath::CornerToExit (sub_8213E360)", Original<__imp__sub_8213E360>, NativeCornerToExit,
       kCornerToExit, 2000},
      {"AiPath::PlanCorners (sub_8213E7B8)", Original<__imp__sub_8213E7B8>, NativePlanCorners,
       kPlanCorners, 2000},
      {"AiPath::PlanLine (sub_8213F4B0)", Original<__imp__sub_8213F4B0>, NativePlanLine,
       kPlanLine, 2000},
      {"AiPath::BranchMasksFor (sub_8213D378)", Original<__imp__sub_8213D378>,
       NativeBranchMasksFor, kBranchMasksFor, 500},
      {"AiPath::LeaderBranchKind (sub_8213D1F0)", Original<__imp__sub_8213D1F0>,
       NativeLeaderBranchKind, kLeaderBranchKind, 1000},
      {"AiPath::FindSideSection (sub_8213D550)", Original<__imp__sub_8213D550>,
       NativeFindSideSection, kFindSideSection, 2000},
      {"AiPath::FollowTargets (sub_8213D9D8)", Original<__imp__sub_8213D9D8>,
       NativeFollowTargets, kFollowTargets, 2000},
      {"AiPath::FollowLeaderBranch (sub_8213EB48)", Original<__imp__sub_8213EB48>,
       NativeFollowLeaderBranch, kFollowLeaderBranch, 1000},
      {"AiDriver::CompareWith (sub_82145B08)", Original<__imp__sub_82145B08>, NativeCompareWith,
       kCompareWith, 2000},
      {"AiDrivers::UpdateAll (sub_82139628)", Original<__imp__sub_82139628>, NativeUpdateAll,
       kUpdateAll, 500},
      {"AiOther::PlanReaction (sub_821456F8)", Original<__imp__sub_821456F8>, NativePlanReaction,
       kPlanReaction, 3000},
      {"AiDriver::ReactToOthers (sub_82145CA8)", Original<__imp__sub_82145CA8>,
       NativeReactToOthers, kReactToOthers, 2000},
      {"AiDrivers::StartGroup (sub_8213A000)", Original<__imp__sub_8213A000>, NativeStartGroup,
       kStartGroup, 2000},
      {"AiDrivers::StartRanked (sub_8213A8B0)", Original<__imp__sub_8213A8B0>,
       NativeStartRanked, kStartRanked, 2000},
      {"AiDriver::ReadVehicle (sub_821467F0)", Original<__imp__sub_821467F0>, NativeReadVehicle,
       kReadVehicle, 3000},
      {"AiDriver::ReadVehicleOwnSlice (sub_821469D0)", Original<__imp__sub_821469D0>,
       NativeReadVehicleOwnSlice, kReadVehicleOwnSlice, 2000},
      {"AiPath::BranchMasks (sub_8213E500)", Original<__imp__sub_8213E500>, NativeBranchMasks,
       kBranchMasks, 3000},
      {"AiPath::KeepBranch (sub_8213EAB0)", Original<__imp__sub_8213EAB0>, NativeKeepBranch,
       kKeepBranch, 3000},
      {"AiPath::LeftEntry (sub_8213F140)", Original<__imp__sub_8213F140>, NativeLeftEntry,
       kLeftEntry, 3000},
      {"AiPath::DropPassed (sub_8213F2E0)", Original<__imp__sub_8213F2E0>, NativeDropPassed,
       kDropPassed, 3000},
      {"AiPath::Extend (sub_8213ECF8)", Original<__imp__sub_8213ECF8>, NativeExtend, kExtend,
       2000},
      {"AiPath::Update (sub_8213F530)", Original<__imp__sub_8213F530>, NativeUpdatePath,
       kUpdatePath, 2000},
  };
  const uint32_t area = memory::Alloc(kAreaBytes, 0);
  const uint32_t driver = area + kDriver, vehicle = area + kVehicle, io = area + kIo;
  const uint32_t saved_drivers = guest::LoadU32(h.base, ai::kDrivers);
  const uint32_t saved_route = guest::LoadU32(h.base, track_route::kRoute);
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  const uint32_t saved_state = guest::LoadU32(h.base, kRaceStateGlobal);
  uint32_t saved_inputs[8];
  for (uint32_t i = 0; i < 8; i++) {
    saved_inputs[i] = guest::LoadU32(h.base, kInputs + 4 * i);
  }
  const uint32_t table = memory::Alloc(kTableDrivers * ai::kDriverBytes, 0);
  const uint32_t vehicles = memory::Alloc(8 * kVehicleBytes, 0);
  const uint32_t saved_race_global = guest::LoadU32(h.base, kRaceGlobal);
  const uint32_t saved_vehicles = guest::LoadU32(h.base, kVehiclesGlobal);
  {
    Random fill;
    for (uint32_t i = 0; i < kTableDrivers * ai::kDriverBytes; i += 4) {
      guest::StoreU32(h.base, table + i, uint32_t(fill.Next()));
    }
  }
  // A course's route for the functions reading one (test_route.h).
  const uint32_t route_area = memory::Alloc(test_route::kRouteBytes, 0);
  // Race data with its words +4, +20 and +0xA3AF4 (BranchMasks' cases).
  const uint32_t race_block = memory::Alloc(0xA3B00, 0);
  const uint32_t route_table = memory::Alloc(4 * test_route::kTableEntries, 0);
  test_route::FillTable(h.base, route_table, route_area);
  // The table's first seven drivers, for ResetOthers (which writes them).
  constexpr uint32_t kSevenBytes = 7 * ai::kDriverBytes;
  Random r;
  for (const auto& f : functions) {
    Results results(f.name);
    for (int c = 0; c < f.cases; c++) {
      // Random words, mostly floats, every fourth case (the others start
      // from what the last one left, and set their fields again).
      if (c % 4 == 0) {
        for (uint32_t i = 0; i < kAreaBytes; i += 4) {
          guest::StoreU32(h.base, area + i, FillWord(r.Next()));
        }
      }
      // The driver's vehicle (now and then none) and its box shape.
      guest::StoreU32(h.base, driver + 160, r.Below(8) == 0 ? 0 : vehicle);
      guest::StoreU32(h.base, vehicle + 344, r.Below(4) == 0 ? 0 : area + kShape);
      const uint32_t flags[] = {0, 1, 0x80, 0x81, 0x100, uint32_t(r.Next())};
      guest::StoreU32(h.base, vehicle + 6148, flags[r.Below(6)]);
      guest::StoreU32(h.base, driver + 16, r.Below(2) ? uint32_t(r.Next()) : r.Below(4));
      guest::StoreU32(h.base, driver + 32, r.Below(8) ? r.Below(8) : uint32_t(r.Next()));
      uint32_t drivers = saved_drivers;
      // The race data: its words +0xA3AF0 and +0xA3AF4 at kRace, or (the
      // branch kinds' cases) its flags +20 at io + 0xF0.
      uint32_t race_data = area + kRace - 0xA3AF0;
      uint64_t r3 = driver, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0;
      uint32_t route_file = area + kFile;
      bool table_written = false;
      uint64_t f1 = 0x7FF8DEADBEEF0000ull;
      std::vector<std::pair<uint32_t, uint32_t>> floats;  // (offset, count) in the block
      std::string what = fmt::format("case {}", c);
      // The steering's fields (UpdateSteerInput and its steps, UpdateSteering,
      // Drive): with `records`, up to four corner records (+5676: their data,
      // four in the vehicle's bytes, 0x300 apart, with +50 and +52, the offset
      // +10, the direction +16, the kind +40) and their count; the ring (count,
      // start, look-ahead; each entry's point, +40, +44 and slice, now and then
      // none) and the driver's fields.
      const auto steering = [&](bool records) {
        const uint32_t path = driver + kPath;
        if (records) {
          guest::StoreU16(h.base, driver + 5674, uint16_t(int32_t(r.Below(6)) - 1));
          for (uint32_t i = 0; i < 4; i++) {
            const uint32_t data = vehicle + 0x300 * i;
            guest::StoreU16(h.base, data + 50, uint16_t(int32_t(r.Below(60)) - 10));
            guest::StoreU16(h.base, data + 52, uint16_t(int32_t(r.Below(60)) - 20));
            const uint32_t at = driver + 5676 + 44 * i;
            guest::StoreU32(h.base, at + 0, r.Below(6) == 0 ? 0 : vehicle + 0x300 * r.Below(4));
            guest::StoreU16(h.base, at + 10, uint16_t(int32_t(r.Below(40)) - 20));
            guest::StoreU32(h.base, at + 16, r.Below(8) == 0 ? RandomFloat(r, 2.0f)
                                             : FloatBits(r.Below(2) ? 1.0f : -1.0f));
          }
        }
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, driver + 5676 + 44 * i + 40, r.Below(6));
        }
        guest::StoreU16(h.base, path + 5136, uint16_t(int32_t(r.Below(84)) - 2));
        guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
        guest::StoreU16(h.base, path + 5142, uint16_t(int32_t(r.Below(30)) - 2));
        for (uint32_t k = 0; k < 80; k++) {
          const uint32_t entry = path + 16 + 64 * k;
          guest::StoreU32(h.base, entry + 0, RandomFloat(r, 100.0f));
          guest::StoreU32(h.base, entry + 8, RandomFloat(r, 100.0f));
          guest::StoreU32(h.base, entry + 40, RandomFloat(r, 4.0f));
          guest::StoreU32(h.base, entry + 44, RandomFloat(r, 4.0f));
          guest::StoreU32(h.base, entry + 52, r.Below(10) == 0 ? 0 : vehicle + 0x1000);
        }
        if (r.Below(16) == 0) {  // now and then the target point at the driver's
          for (uint32_t k = 0; k < 80; k++) {
            guest::StoreU32(h.base, path + 16 + 64 * k + 0, guest::LoadU32(h.base, driver + 80));
            guest::StoreU32(h.base, path + 16 + 64 * k + 8, guest::LoadU32(h.base, driver + 88));
          }
        }
        guest::StoreU32(h.base, driver + 100, r.Below(2) ? FloatBits(float(r.Below(140)))
                                                         : RandomFloat(r, 300.0f));
        guest::StoreU32(h.base, driver + 96, RandomFloat(r, 4.0f));
        guest::StoreU32(h.base, driver + 104, RandomFloat(r, 1.0f));
        guest::StoreU32(h.base, driver + 108, RandomFloat(r, 2.0f));
        guest::StoreU32(h.base, driver + 112, RandomFloat(r, 2.0f));
        guest::StoreU32(h.base, driver + 116, RandomFloat(r, 2.0f));
        guest::StoreU32(h.base, driver + 128, RandomFloat(r, 3.0f));
        guest::StoreU32(h.base, driver + 132, RandomFloat(r, 1.5f));
        guest::StoreU32(h.base, driver + 144, RandomFloat(r, 5.0f));
        guest::StoreU32(h.base, driver + 6088, RandomFloat(r, 200.0f));
        for (uint32_t at : {6112u, 6116u}) {
          guest::StoreU32(h.base, driver + at, RandomFloat(r, 1.0f));
        }
        for (uint32_t at : {6120u, 6136u, 6140u}) {
          guest::StoreU32(h.base, driver + at, RandomFloat(r, 1.5f));
        }
        floats.push_back({kDriver + 6136, 2});
        floats.push_back({kDriver + 6120, 1});
      };
      switch (f.kind) {
        case kInitPath:
        case kClearFollow:
        case kClearOtherIndexes:
          break;
        case kOf: {
          const uint32_t tables[] = {0, table, 0x80000000u, uint32_t(r.Next())};
          drivers = tables[r.Below(4)];
          guest::StoreU32(h.base, io, r.Below(4) ? r.Below(8) : uint32_t(r.Next()));
          r3 = io;
          break;
        }
        case kClearCarInput:
        case kClearCornerKind:
          r3 = io;
          break;
        case kClearCorners:
          r3 = driver + kPath;
          break;
        case kResetSkill:
          r3 = driver + kSkill;
          break;
        case kWrapAngle: {
          const float edges[] = {3.14159265f, -3.14159265f, 3.1415927f, -3.1415927f,
                                 6.2831855f, -6.2831855f, 9.5f, -9.5f, 0.0f};
          guest::StoreU32(h.base, io, r.Below(4) == 0 ? FloatBits(edges[r.Below(9)])
                                                      : RandomFloat(r, 10.0f));
          r3 = io;
          floats.push_back({kIo, 1});
          break;
        }
        case kTimeFromTicks: {
          const uint32_t ticks[] = {0, 1, 29, 30, 1799, 1800, 0xFFFFFFFFu, 0x80000000u,
                                    0x7FFFFFFFu, 1800u * 32768u, 1800u * 32767u + 1799u};
          const uint32_t low = r.Below(4) == 0  ? ticks[r.Below(11)]
                               : r.Below(2) == 0 ? uint32_t(int32_t(r.Below(200000)) - 100000)
                                                 : uint32_t(r.Next());
          r3 = (r.Below(2) ? (r.Next() << 32) : 0) | low;
          break;
        }
        case kSetFlag2:
          guest::StoreU32(h.base, io, r.Below(2) ? 0 : uint32_t(r.Next()));
          r4 = io;
          break;
        case kIsFree:
          break;
        case kSpeedOverDistance:
          guest::StoreU32(h.base, driver + 100,
                          r.Below(8) == 0 ? FloatBits(100.0f) : RandomFloat(r, 300.0f));
          r4 = io;
          break;
        case kCornerFactor:
          guest::StoreU32(h.base, io, r.Below(8) == 0 ? FloatBits(10.0f) : RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, driver + 128, RandomFloat(r, 10.0f));
          r4 = io;
          r5 = io + 4;
          break;
        case kReadCar:
          floats.push_back({kDriver + 116, 7});
          break;
        case kUpdateHalfWidth:
          guest::StoreU32(h.base, driver + 148, RandomFloat(r, 7.0f));
          floats.push_back({kDriver + 116, 1});
          break;
        case kTargetPoint: {
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          const int32_t count = int32_t(r.Below(85)) - 2;
          const int32_t k = r.Below(4) == 0 ? int32_t(r.Below(90)) - 4 : int32_t(r.Below(80));
          const int32_t start = r.Below(8) == 0 ? int32_t(int16_t(r.Next()))
                                                : int32_t(r.Below(200)) - 60;
          guest::StoreU16(h.base, driver + kPath + 5136, uint16_t(count));
          guest::StoreU16(h.base, driver + kPath + 5142, uint16_t(k));
          guest::StoreU16(h.base, driver + kPath + 5138, uint16_t(start));
          r3 = driver + kPath;
          r4 = area + kOut;
          // The distance ahead: near the look-ahead entry's, now and then
          // anywhere (a float, as the callers pass one).
          const float length = std::bit_cast<float>(guest::LoadU32(h.base, area + kFile + 8));
          const float ahead = r.Below(8) == 0
                                  ? std::bit_cast<float>(RandomFloat(r, 100.0f))
                                  : (float(k) + float(r.Below(1200)) / 1000.0f - 0.1f) * length;
          f1 = std::bit_cast<uint64_t>(double(ahead));
          floats.push_back({kOut, 3});
          what += fmt::format(" count {} k {} start {}", count, k, start);
          break;
        }
        case kClearSkill:
          r3 = driver + kSkill;
          break;
        case kSetSkillSource:
          r3 = driver + kSkill;
          r4 = r.Below(2) ? 0 : io;
          break;
        case kLoadSkill:
          r3 = driver + kSkill;
          guest::StoreU32(h.base, driver + kSkill, r.Below(2) ? 0 : io);
          guest::StoreU32(h.base, area + kRace, r.Below(2) ? 0 : uint32_t(r.Next()));
          break;
        case kLimitSpeed:
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 8, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, io + 12,
                          r.Below(4) ? uint32_t(int32_t(r.Below(44)) - 3) : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          r4 = io;
          r5 = io + 4;
          r6 = io + 8;
          r7 = io + 12;
          floats.push_back({kDriver + 6088, 1});
          break;
        case kLimitByCurve: {
          guest::StoreU32(h.base, io + 0x20, io + 0x30);
          guest::StoreU32(h.base, io + 0x24, io + 0x40);
          guest::StoreU16(h.base, io + 0x30, uint16_t(int32_t(r.Below(10)) - 1));
          guest::StoreU32(h.base, io + 4,
                          r.Below(8) ? uint32_t(int32_t(r.Below(12)) - 2) : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + 144, RandomFloat(r, 30.0f));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, io + 0x44 + 8 * i, RandomFloat(r, 30.0f));
          }
          guest::StoreU32(h.base, io + 0x80 + 32, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, io + 0x80 + 36, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, io, RandomFloat(r, 1.5f));
          r4 = io + 0x20;
          r5 = io;
          r6 = io + 4;
          r7 = r.Below(8) == 0 ? 0 : io + 0x80;
          floats.push_back({kIo, 1});
          break;
        }
        case kEaseThrottleForSteer:
          guest::StoreU32(h.base, driver + 6140, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, driver + 152, RandomFloat(r, 20.0f));
          floats.push_back({kDriver + 6140, 1});
          break;
        case kHasKind2Ahead:
        case kIsSlow:
          guest::StoreU16(h.base, driver + kPath + 5162, uint16_t(int32_t(r.Below(11)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = driver + kPath + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(7)) - 4));
            guest::StoreU32(h.base, record + 40, r.Below(8) ? r.Below(6) : uint32_t(r.Next()));
          }
          r3 = driver + kPath;
          break;
        case kPreferredSections:
          guest::StoreU8(h.base, io, uint8_t(r.Below(4) ? int32_t(r.Below(14)) - 2 : r.Below(256)));
          guest::StoreU8(h.base, io + 1,
                         uint8_t(r.Below(4) ? int32_t(r.Below(16)) - 3 : r.Below(256)));
          r3 = io;
          r4 = io + 1;
          break;
        case kRaceAllowsSides:
          guest::StoreU32(h.base, area + kRace + 4, r.Below(8) ? r.Below(10) : uint32_t(r.Next()));
          guest::StoreU32(h.base, area + kRaceState, r.Below(8) ? r.Below(5) : uint32_t(r.Next()));
          break;
        case kRefreshPoints:
          guest::StoreU16(h.base, driver + kPath + 5140, uint16_t(int32_t(r.Below(84)) - 1));
          guest::StoreU16(h.base, driver + kPath + 5138, uint16_t(r.Below(200)));
          for (uint32_t k = 0; k < 80; k++) {
            const uint32_t entry = driver + kPath + 16 + 64 * k;
            guest::StoreU32(h.base, entry + 52, vehicle + 84 * r.Below(60));
            floats.push_back({kDriver + kPath + 16 + 64 * k, 3});
          }
          r3 = driver + kPath;
          break;
        case kResetPath:
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          r3 = driver + kPath;
          break;
        case kSetLaneOffset:
          guest::StoreU32(h.base, io + 0x100 + 52, io + 0x180);
          guest::StoreU32(h.base, io + 0x100 + 20,
                          r.Below(8) ? uint32_t(int32_t(r.Below(22)) - 3) : uint32_t(r.Next()));
          guest::StoreU8(h.base, io + 0x180 + 56, uint8_t(r.Below(256)));
          guest::StoreU32(h.base, io + 0x180 + 12, RandomFloat(r, 20.0f));
          r3 = io + 0x100;
          floats.push_back({kIo + 0x100 + 24, 1});
          floats.push_back({kIo + 0x100 + 48, 1});
          break;
        case kUpdateOtherFlags:
          guest::StoreU16(h.base, io + 24, uint16_t(r.Below(4) ? r.Below(5) : r.Next()));
          r3 = io;
          break;
        case kUpdateOtherTimer:
          guest::StoreU32(h.base, io + 0, RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 0x40, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, io + 0x44, (uint32_t(r.Next()) & 0xFFFFFF00u) | r.Below(8));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, area + kInputWords + 4 * i,
                            r.Below(2) ? 0x20000u | uint32_t(r.Next()) : uint32_t(r.Next()));
          }
          r3 = io;
          r4 = io + 0x44;
          r5 = io + 0x40;
          break;
        case kClearOthers:
          break;
        case kPickTarget:
        case kPickTargetBehind: {
          guest::StoreU32(h.base, driver + 0, r.Below(8) ? r.Below(8) : uint32_t(r.Next()));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(2) ? 0x7F : r.Next()));
          const uint32_t current[] = {0xFFFFFFFFu, 0, 3, 6, uint32_t(r.Next())};
          guest::StoreU32(h.base, driver + 480, current[r.Below(5)]);
          break;
        }
        case kKeepsGapFrom:
          guest::StoreU32(h.base, driver + 32, r.Below(8) ? r.Below(6) : uint32_t(r.Next()));
          guest::StoreU32(h.base, io, r.Below(8) ? r.Below(7) : 32 + r.Below(8));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(2) ? r.Below(0x80) : r.Next()));
          for (uint32_t i = 0; i < 40; i++) {
            guest::StoreU32(h.base, table + i * ai::kDriverBytes + 32, r.Below(6));
          }
          drivers = table;
          r4 = io;
          break;
        case kSetParams:
          r4 = r.Below(4) == 0 ? 0 : io;
          break;
        case kSetSkill:
          guest::StoreU32(h.base, driver + kSkill, r.Below(2) ? 0 : io);
          guest::StoreU32(h.base, area + kRace, r.Below(2) ? 0 : uint32_t(r.Next()));
          r4 = io + 0x40;
          break;
        case kClearPlan:
          guest::StoreU32(h.base, driver + kPath + 5524, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          guest::StoreU32(h.base, driver + kPath + 5528, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          break;
        case kLimitSpeedBlend:
          guest::StoreU32(h.base, area + kFile + 8, FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 8, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, io + 12, RandomFloat(r, 60.0f));
          guest::StoreU32(h.base, io + 16, RandomFloat(r, 1.5f));
          guest::StoreU32(h.base, io + 20,
                          r.Below(4) ? uint32_t(int32_t(r.Below(44)) - 3) : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          r4 = io;
          r5 = io + 4;
          r6 = io + 12;
          r7 = io + 16;
          r8 = io + 20;
          floats.push_back({kDriver + 6088, 1});
          break;
        case kSetMode:
          guest::StoreU32(h.base, io, r.Below(8) ? r.Below(4) : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + kPath + 5520, r.Below(4));
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          r3 = driver + kPath;
          r4 = io;
          break;
        case kResetFor:
          r3 = driver + kPath;
          r4 = r.Below(4) == 0 ? 0 : driver;
          break;
        case kNormalizeByLane: {
          const test_route::Route route =
              test_route::BuildRoute(h.base, route_area, route_table, r);
          route_file = route_area + test_route::kFile;
          test_route::RandomPosition(h.base, io, route, r);
          guest::StoreU16(h.base, io + 8, uint16_t(r.Below(4) ? r.Below(8) : r.Next()));
          const uint32_t masks = r.Below(3);
          for (uint32_t i = 0; i < masks; i++) {
            const uint32_t word = r.Below(3) == 0 ? 0xDFFFFE7Fu : uint32_t(r.Next());
            guest::StoreU32(h.base, io + 0x20 + 4 * i, word != 0 ? word : 1u);
          }
          guest::StoreU32(h.base, io + 0x20 + 4 * masks, 0);
          r3 = uint32_t(r.Next());
          r4 = io + 0x100;
          r5 = io;
          r6 = io + 8;
          r7 = r.Below(2) ? 0 : io + 0x20;
          floats.push_back({kIo + 0x100 + 24, 1});
          floats.push_back({kIo + 0x100 + 48, 1});
          break;
        }
        case kUpdateLane: {
          const test_route::Route route =
              test_route::BuildRoute(h.base, route_area, route_table, r);
          route_file = route_area + test_route::kFile;
          guest::StoreU32(h.base, driver + 160, vehicle);
          test_route::RandomPosition(h.base, vehicle + 5232 + 4, route, r);
          for (uint32_t i = 0; i < 3; i++) {
            guest::StoreU32(h.base, vehicle + 5232 + 16 + 4 * i, RandomFloat(r, 250.0f));
          }
          break;
        }
        case kResetOthers:
          guest::StoreU32(h.base, driver + 0, r.Below(8));
          drivers = table;
          table_written = true;
          break;
        case kClearCarState:
          break;
        case kUpdateState2:
        case kUpdateState3:
        case kUpdateState:
        case kUpdateState6:
        case kUpdateState4:
        case kUpdateState5:
        case kIsSettled: {
          guest::StoreU32(h.base, driver + 0, r.Below(8) ? r.Below(8) : uint32_t(r.Next()));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(2) ? 0x7F : r.Next()));
          // A target already picked stays (its record is read): 0 to 6 only.
          const uint32_t current[] = {0xFFFFFFFFu, 0, 3, 6, 0x80000000u};
          guest::StoreU32(h.base, driver + 480, current[r.Below(5)]);
          guest::StoreU32(h.base, driver + 32,
                          f.kind == kUpdateState ? (r.Below(8) ? r.Below(8) : uint32_t(r.Next()))
                          : r.Below(8)           ? r.Below(5)
                                                 : uint32_t(r.Next()));
          // States 2 and 3 tell gaps up to 100.0 apart, and beyond.
          const bool far = f.kind == kUpdateState2 || f.kind == kUpdateState3 ||
                           f.kind == kUpdateState || f.kind == kUpdateState6;
          for (uint32_t i = 0; i < 7; i++) {
            guest::StoreU32(h.base, driver + 172 + 40 * i, RandomFloat(r, far ? 140.0f : 60.0f));
            guest::StoreU32(h.base, table + i * ai::kDriverBytes + 100, RandomFloat(r, 200.0f));
          }
          guest::StoreU32(h.base, io, RandomFloat(r, 300.0f));
          drivers = table;
          r4 = io;
          if (f.kind != kIsSettled) {
            floats.push_back({kIo, 1});
          }
          if (f.kind == kUpdateState6) {
            // The others' states (6 for the step), lengths and rates.
            for (uint32_t i = 0; i < 7; i++) {
              const uint32_t other = table + i * ai::kDriverBytes;
              guest::StoreU32(h.base, other + 32, r.Below(2) ? 6 : r.Below(8));
              guest::StoreU32(h.base, other + 124, RandomFloat(r, 5.0f));
            }
            guest::StoreU32(h.base, driver + 124, RandomFloat(r, 5.0f));
            guest::StoreU32(h.base, driver + 136, RandomFloat(r, 3.0f));
          }
          if (f.kind == kUpdateState || f.kind == kUpdateState6) {
            guest::StoreU32(h.base, driver + 100, RandomFloat(r, 200.0f));
            guest::StoreU32(h.base, driver + 6088, RandomFloat(r, 200.0f));
            floats.push_back({kDriver + 6088, 1});
          }
          break;
        }
        case kMirrorOther:
          guest::StoreU16(h.base, io + 24, uint16_t(r.Below(4) ? r.Below(5) : r.Next()));
          r3 = io + 0x100;
          r4 = io;
          r5 = io + 0x80;
          floats.push_back({kIo + 0x100, 3});
          floats.push_back({kIo + 0x100 + 16, 2});
          break;
        case kBlockLeft:
        case kBlockRight:
          for (uint32_t i = 0; i < 3; i++) {
            guest::StoreU32(h.base, io + 4 * i, RandomFloat(r, 10.0f));
          }
          for (uint32_t at : {28u, 40u, 44u}) {
            guest::StoreU32(h.base, io + 0x100 + at, RandomFloat(r, 10.0f));
          }
          r3 = io + 0x100;
          r4 = io;
          r5 = io + 4;
          r6 = io + 8;
          floats.push_back({kIo, 3});
          floats.push_back({kIo + 0x100 + 40, 2});
          break;
        case kInitCorner:
          r3 = io + 0x100;
          r4 = io + 0x200;
          r5 = io;
          r6 = io + 8;
          break;
        case kWithinRoadAt:
          guest::StoreU32(h.base, area + kFile + 8, FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, driver + kPath, driver);
          guest::StoreU32(h.base, driver + 128, FloatBits(float(r.Below(4000)) / 20.0f));
          guest::StoreU16(h.base, driver + kPath + 5136, uint16_t(int32_t(r.Below(84)) - 1));
          guest::StoreU16(h.base, driver + kPath + 5138, uint16_t(r.Below(200)));
          for (uint32_t k = 0; k < 80; k++) {
            guest::StoreU32(h.base, driver + kPath + 16 + 64 * k + 52, vehicle + 84 * r.Below(60));
          }
          guest::StoreU32(h.base, io, RandomFloat(r, 12.0f));
          r3 = driver + kPath;
          r4 = io;
          break;
        case kResetToMode1:
        case kResetToMode0:
        case kStartRace:
          if (f.kind == kStartRace) {
            guest::StoreU32(h.base, driver + 160, vehicle);  // read without a check
            guest::StoreU32(h.base, driver + kSkill, r.Below(2) ? 0 : io);
            guest::StoreU32(h.base, area + kRace, r.Below(2) ? 0 : uint32_t(r.Next()));
            guest::StoreU32(h.base, driver + kPath + 5524,
                            r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
            guest::StoreU32(h.base, driver + kPath + 5528,
                            r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          }
          guest::StoreU32(h.base, driver + kPath + 5520, r.Below(3));
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          break;
        case kResetFollow:
          break;
        case kAllSettled:
        case kReadVehicle:
        case kReadVehicleOwnSlice:
        case kUpdateAgainstPlayer: {
          // The racers: up to 7, flag 0x10 half the time, an index 0 to 6 or -1.
          const uint32_t count = f.kind == kAllSettled && r.Below(8) == 0 ? 0 : 1 + r.Below(7);
          guest::StoreU32(h.base, area + kRacers + 56, r.Below(16) == 0 ? 0xFFFFFFFFu : count);
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t racer = area + kRacerRecords + 24 * i;
            guest::StoreU32(h.base, area + kRacers + 4 * i, racer);
            guest::StoreU32(h.base, racer + 0, uint32_t(r.Next()) & (r.Below(2) ? ~0x10u : ~0u));
            guest::StoreU32(h.base, racer + 20, r.Below(8) ? r.Below(7) : 0xFFFFFFFFu);
          }
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t other = table + i * ai::kDriverBytes;
            guest::StoreU32(h.base, other + 32, r.Below(5));
            guest::StoreU32(h.base, other + 0, r.Below(8));
            guest::StoreU16(h.base, other + 40, uint16_t(r.Below(2) ? 0x7F : r.Next()));
            guest::StoreU32(h.base, other + 100, RandomFloat(r, 100.0f));
            for (uint32_t k = 0; k < 7; k++) {
              guest::StoreU32(h.base, other + 172 + 40 * k, RandomFloat(r, 40.0f));
            }
            const uint32_t flags[] = {0, 1, 0x80, 0x81, uint32_t(r.Next())};
            guest::StoreU32(h.base, vehicles + i * kVehicleBytes + 6148, flags[r.Below(5)]);
          }
          drivers = table;
          if (f.kind == kReadVehicle || f.kind == kReadVehicleOwnSlice) {
            // Always a vehicle and its body (io + 0x20: its angular velocity
            // at +96, its orientation at +224), read without checks; the
            // vehicle's track position (its identity, +5232: the route
            // position +4, the point +16) on a route (test_route.h), its
            // speed (+48), +2356, +316 and +320; the orientation's x and z
            // now and then 0.0 (either sign); the slice record (r4): one of
            // the route's, now and then any bytes or none.
            const test_route::Route route =
                test_route::BuildRoute(h.base, route_area, route_table, r);
            route_file = route_area + test_route::kFile;
            guest::StoreU32(h.base, driver + 160, vehicle);
            const uint32_t body = io + 0x20;
            guest::StoreU32(h.base, vehicle + 336, body);
            test_route::RandomPosition(h.base, vehicle + 5232 + 4, route, r);
            for (uint32_t i = 0; i < 4; i++) {
              guest::StoreU32(h.base, vehicle + 5232 + 16 + 4 * i, RandomFloat(r, 150.0f));
            }
            guest::StoreU32(h.base, vehicle + 48, RandomFloat(r, 80.0f));
            guest::StoreU32(h.base, vehicle + 2356, RandomFloat(r, 3.0f));
            guest::StoreU32(h.base, vehicle + 316, RandomFloat(r, 1.0f));
            guest::StoreU32(h.base, vehicle + 320, RandomFloat(r, 1.0f));
            guest::StoreU32(h.base, body + 96 + 4, RandomFloat(r, 4.0f));
            for (uint32_t at : {0u, 8u}) {
              guest::StoreU32(h.base, body + 224 + at, r.Below(3) == 0
                                                           ? (r.Below(2) ? 0u : 0x80000000u)
                                                           : RandomFloat(r, 1.0f));
            }
            for (uint32_t at : {120u, 124u}) {
              guest::StoreU32(h.base, driver + at, RandomFloat(r, 3.0f));
            }
            if (f.kind == kReadVehicle) {
              const uint32_t section = r.Below(route.sections);
              r4 = r.Below(8) == 0   ? 0
                   : r.Below(8) == 0 ? io + 0xC0
                                     : route.Address(section) + 124 +
                                           84 * r.Below(route.Slices(section));
            }
            floats.push_back({kDriver + 96, 3});
            floats.push_back({kDriver + 112, 2});
            floats.push_back({kDriver + 144, 2});
            floats.push_back({kDriver + 156, 1});
          }
          if (f.kind == kUpdateAgainstPlayer) {
            guest::StoreU32(h.base, driver + 0, r.Below(8));
            guest::StoreU32(h.base, driver + 100, FloatBits(float(r.Below(400)) / 10.0f));
            guest::StoreU16(h.base, driver + 492,
                            uint16_t(r.Below(4) ? r.Below(5000) : r.Next()));
            for (uint32_t k = 0; k < 7; k++) {
              guest::StoreU8(h.base, driver + 172 + 40 * k + 32, uint8_t(r.Below(3)));
            }
          }
          break;
        }
        case kAccumulateSteer:
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 400.0f));
          break;
        case kThrottleForSpeed:
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 120.0f));
          guest::StoreU32(h.base, driver + 6088, RandomFloat(r, 120.0f));
          guest::StoreU32(h.base, driver + 6116, RandomFloat(r, 3.0f));
          guest::StoreU32(h.base, driver + 6140, RandomFloat(r, 1.5f));
          floats.push_back({kDriver + 6140, 2});
          break;
        case kEaseThrottleForSlide:
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 120.0f));
          guest::StoreU32(h.base, driver + 6112, RandomFloat(r, 1.0f));
          guest::StoreU32(h.base, driver + 6140, RandomFloat(r, 1.5f));
          guest::StoreU32(h.base, vehicle + 3548, r.Below(4));
          guest::StoreU32(h.base, io, r.Below(2) ? 0 : uint32_t(r.Next()));
          r4 = io;
          floats.push_back({kDriver + 6140, 1});
          break;
        case kLimitThrottle: {
          guest::StoreU32(h.base, driver + 160, vehicle);  // read without a check
          guest::StoreU16(h.base, driver + 6054, uint16_t(int32_t(r.Below(140)) - 20));
          guest::StoreU32(h.base, driver + 6124, RandomFloat(r, 1.5f));
          const float limit = 6000.0f + float(r.Below(3000));
          guest::StoreU32(h.base, vehicle + 4660, r.Below(16) ? FloatBits(limit) : RandomFloat(r, 9000.0f));
          guest::StoreU32(h.base, vehicle + 2228,
                          FloatBits(limit * (0.9f + float(r.Below(150)) / 1000.0f)));
          floats.push_back({kDriver + 6124, 1});
          break;
        }
        case kAdjustForOther:
          guest::StoreU32(h.base, area + kFile + 8, FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, io + 0x100, RandomFloat(r, 60.0f));
          guest::StoreU16(h.base, io + 0x100 + 24, uint16_t(r.Below(4) ? 1 : r.Below(5)));
          guest::StoreU16(h.base, io + 0x100 + 26, uint16_t(int32_t(r.Below(12)) - 2));
          guest::StoreU16(h.base, io + 0x100 + 28, uint16_t(r.Below(2) ? 0 : r.Next()));
          guest::StoreU32(h.base, io, r.Below(2) ? 0 : uint32_t(r.Next()));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          r4 = io + 0x100;
          r5 = io;
          floats.push_back({kDriver + 164, 2});
          break;
        case kUpdateOther:
          guest::StoreU32(h.base, io, RandomFloat(r, 250.0f));
          guest::StoreU32(h.base, io + 0x40, RandomFloat(r, 250.0f));
          guest::StoreU32(h.base, io + 0x44, RandomFloat(r, 10.0f));
          guest::StoreU16(h.base, io + 34, uint16_t(r.Next()));
          r3 = io;
          r4 = io + 0x40;
          r5 = io + 0x44;
          floats.push_back({kIo, 3});
          floats.push_back({kIo + 20, 1});
          break;
        case kResetControl:
          guest::StoreU32(h.base, driver + kPath + 5520, r.Below(3));
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          break;
        case kResetDriver:
          guest::StoreU32(h.base, driver + 0, r.Below(8));
          drivers = table;
          table_written = true;
          break;
        case kUpdateSteerInput:
        case kLimitSlide:
        case kSteerByRoom:
        case kSteerOutOfCorner:
        case kSteerAtCornerPoint:
        case kSteerIntoCorner:
        case kUpdateSteering:
          steering(true);
          r4 = driver + 5676 + 44 * r.Below(4);
          if (f.kind == kSteerIntoCorner && guest::LoadU32(h.base, r4) == 0) {
            guest::StoreU32(h.base, r4, vehicle);  // its data read without a check
          }
          guest::StoreU32(h.base, io, f.kind == kLimitSlide ? r.Below(2)
                                                            : uint32_t(int32_t(r.Below(90)) - 5));
          r5 = io;
          floats.push_back({kIo, 1});
          for (uint32_t i = 0; i < 4; i++) {
            floats.push_back({kDriver + 5676 + 44 * i + 40, 1});
          }
          break;
        case kDropPassedSections: {
          guest::StoreU32(h.base, driver + kPath, driver);
          guest::StoreU16(h.base, driver + 4, uint16_t(r.Below(6)));
          guest::StoreU16(h.base, driver + kPath + 5144, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU16(h.base, driver + kPath + 5146 + 2 * i, uint16_t(r.Below(7)));
          }
          r3 = driver + kPath;
          break;
        }
        case kSharesSections:
          guest::StoreU16(h.base, driver + kPath + 5144, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU16(h.base, driver + kPath + 5146 + 2 * i, uint16_t(r.Below(7)));
          }
          guest::StoreU16(h.base, io + 2, uint16_t(int32_t(r.Below(14)) - 1));
          for (uint32_t i = 0; i < 12; i++) {
            guest::StoreU16(h.base, io + 36 + 2 * i, uint16_t(r.Below(7)));
          }
          r3 = driver + kPath;
          r4 = io;
          break;
        case kWiden:
          for (uint32_t at : {32u, 36u}) {
            guest::StoreU32(h.base, io + 0x100 + at, RandomFloat(r, 10.0f));
          }
          guest::StoreU32(h.base, io, RandomFloat(r, 3.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 3.0f));
          r3 = io + 0x100;
          r4 = io;
          r5 = io + 4;
          floats.push_back({kIo + 0x100 + 40, 2});
          break;
        case kNarrowToOther:
          for (uint32_t at : {32u, 36u, 40u, 44u}) {
            guest::StoreU32(h.base, io + 0x100 + at, RandomFloat(r, 10.0f));
          }
          for (uint32_t at : {32u, 36u}) {
            guest::StoreU32(h.base, io + 0x180 + at, RandomFloat(r, 10.0f));
          }
          guest::StoreU32(h.base, io + 0x180 + 52, r.Below(8) == 0 ? 0 : io + 0x200);
          guest::StoreU32(h.base, io + 0x200 + 24, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, io + 0x200 + 28, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, io, RandomFloat(r, 3.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 3.0f));
          r3 = io + 0x100;
          r4 = r.Below(8) == 0 ? 0 : io + 0x180;
          r5 = io;
          r6 = io + 4;
          floats.push_back({kIo + 0x100 + 32, 4});
          break;
        case kAdvanceCorners:
          guest::StoreU16(h.base, driver + kPath + 5162, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = driver + kPath + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 12, uint16_t(int32_t(r.Below(14)) - 3));
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(14)) - 3));
          }
          guest::StoreU16(h.base, io, uint16_t(r.Below(6)));
          r3 = driver + kPath;
          r4 = io;
          break;
        case kRestart:
        case kSetState2or3:
        case kSetStateFromList:
        case kSetState4: {
          // The path's driver word (AiPath::Reset's) and what ClearPlan and
          // ResetToMode1 read.
          guest::StoreU32(h.base, driver + kPath, r.Below(4) == 0 ? 0 : driver);
          guest::StoreU32(h.base, driver + kPath + 5520, r.Below(3));
          guest::StoreU32(h.base, driver + kPath + 5524, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          guest::StoreU32(h.base, driver + kPath + 5528, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          guest::StoreU32(h.base, driver + 0, r.Below(8));
          guest::StoreU32(h.base, io, uint32_t(r.Next()));
          guest::StoreU32(h.base, io + 4, r.Below(2) ? 0 : uint32_t(r.Next()));
          // The list's count (-1 to 9) and its words (racers 0 to 7, now and
          // then a shift of 32 or more).
          const int32_t n = int32_t(r.Below(11)) - 1;
          guest::StoreU32(h.base, io + 8, r.Below(16) == 0 ? uint32_t(r.Next()) : uint32_t(n));
          for (uint32_t i = 0; i < 12; i++) {
            guest::StoreU32(h.base, io + 0x10 + 4 * i, r.Below(8) ? r.Below(8) : r.Below(70));
          }
          r4 = io;
          r5 = f.kind == kSetState2or3 ? io + 4 : io + 8;
          r6 = io + 0x10;
          what += fmt::format(" n {}", n);
          break;
        }
        case kResetAll:
        case kClearAll: {
          // The racers: up to 7, flag 0x10 half the time, an index 0 to 6
          // (ResetAll: or -1, which it skips; ClearAll doesn't check).
          const uint32_t count = r.Below(8) == 0 ? 0 : 1 + r.Below(7);
          guest::StoreU32(h.base, area + kRacers + 56, r.Below(16) == 0 ? 0xFFFFFFFFu : count);
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t racer = area + kRacerRecords + 24 * i;
            guest::StoreU32(h.base, area + kRacers + 4 * i, racer);
            guest::StoreU32(h.base, racer + 0, uint32_t(r.Next()) & (r.Below(2) ? ~0x10u : ~0u));
            guest::StoreU32(h.base, racer + 20,
                            f.kind == kResetAll && r.Below(8) == 0 ? 0xFFFFFFFFu : r.Below(7));
          }
          // The drivers they reset: a vehicle or none, the path's driver word
          // (itself or 0) and what ClearPlan and ResetToMode1 read.
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t other = table + i * ai::kDriverBytes;
            guest::StoreU32(h.base, other + 160, r.Below(4) == 0 ? 0 : vehicle);
            guest::StoreU32(h.base, other + kPath, r.Below(4) == 0 ? 0 : other);
            guest::StoreU32(h.base, other + kPath + 5520, r.Below(3));
            guest::StoreU32(h.base, other + kPath + 5524,
                            r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
            guest::StoreU32(h.base, other + kPath + 5528,
                            r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          }
          drivers = r.Below(16) == 0 ? 0 : table;
          table_written = true;
          break;
        }
        case kJoinCorners:
        case kJoinCornersShort:
        case kJoinCornerPair: {
          // Two corner records (+0x100, +0x140 in the driver's bytes), their
          // data (+0x200, +0x280) and tables (+0x400, +0x600, 8-byte entries:
          // the fill's floats); the count at io.
          const uint32_t a = driver + 0x100, b = driver + 0x140;
          const uint32_t data_a = driver + 0x200, data_b = driver + 0x280;
          const bool none = f.kind == kJoinCornerPair && r.Below(16) == 0;
          guest::StoreU32(h.base, a + 0, none && r.Below(4) == 0 ? 0 : data_a);
          guest::StoreU32(h.base, a + 4, none && r.Below(4) == 0 ? 0 : driver + 0x400);
          guest::StoreU32(h.base, b + 0, none && r.Below(4) == 0 ? 0 : data_b);
          guest::StoreU32(h.base, b + 4, none && r.Below(4) == 0 ? 0 : driver + 0x600);
          guest::StoreU16(h.base, data_b + 0, uint16_t(int32_t(r.Below(15)) - 2));
          guest::StoreU16(h.base, data_b + 52, uint16_t(r.Below(41)));
          guest::StoreU16(h.base, data_b + 54, uint16_t(r.Below(41)));
          guest::StoreU16(h.base, data_a + 50, uint16_t(r.Below(31)));
          guest::StoreU32(h.base, data_a + 12, RandomFloat(r, 20.0f));
          guest::StoreU32(h.base, data_b + 28, r.Below(16) == 0 ? FloatBits(100.0f)
                                                               : RandomFloat(r, 200.0f));
          guest::StoreU16(h.base, b + 14, uint16_t(r.Below(51)));
          guest::StoreU16(h.base, a + 10, uint16_t(int32_t(r.Below(41)) - 10));
          guest::StoreU16(h.base, b + 10, uint16_t(int32_t(r.Below(41)) - 10));
          // The directions: 1.0 or -1.0 (now and then a fill float); the same
          // for the translated steps (never seen in play) now and then.
          const float sides[] = {1.0f, -1.0f};
          const uint32_t side_a = r.Below(8) ? FloatBits(sides[r.Below(2)]) : RandomFloat(r, 2.0f);
          guest::StoreU32(h.base, a + 16, side_a);
          guest::StoreU32(h.base, b + 16, r.Below(8) == 0 ? side_a : FloatBits(-std::bit_cast<float>(side_a)));
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, io, r.Below(16) == 0 ? uint32_t(r.Next()) & 0xFF : r.Below(41));
          r4 = a;
          r5 = b;
          r6 = io;
          floats.push_back({kDriver + 0x100 + 28, 1});
          floats.push_back({kDriver + 0x140 + 32, 1});
          break;
        }
        case kAddCornersAt: {
          // Four corners' data in the vehicle's bytes (0x300 apart: +0 the
          // length, +2 and +36 its sections, +4 its direction, +12, +28, +50
          // to +54, its table from +56), the route file's corner table (+16:
          // 8 words, some 0) at +0xD00, the slice record (+58 the count, its
          // corners from +60) at +0xC00 and the path entry at io + 0x100.
          const uint32_t path = driver + kPath;
          for (uint32_t i = 0; i < 4; i++) {
            const uint32_t data = vehicle + 0x300 * i;
            guest::StoreU16(h.base, data + 0, uint16_t(r.Below(13)));
            guest::StoreU16(h.base, data + 2, uint16_t(int32_t(r.Below(8)) - 1));
            for (uint32_t k = 0; k < 8; k++) {
              guest::StoreU16(h.base, data + 36 + 2 * k, uint16_t(r.Below(7)));
            }
            guest::StoreU32(h.base, data + 4, RandomFloat(r, 2.0f));
            guest::StoreU32(h.base, data + 12, RandomFloat(r, 20.0f));
            guest::StoreU32(h.base, data + 28, RandomFloat(r, 200.0f));
            guest::StoreU16(h.base, data + 50, uint16_t(r.Below(31)));
            guest::StoreU16(h.base, data + 52, uint16_t(r.Below(41)));
            guest::StoreU16(h.base, data + 54, uint16_t(r.Below(41)));
          }
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, vehicle + 0xD00 + 4 * i,
                            r.Below(6) == 0 ? 0 : vehicle + 0x300 * r.Below(4));
          }
          guest::StoreU32(h.base, area + kFile + 16, vehicle + 0xD00);
          guest::StoreU32(h.base, area + kFile + 8, FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          const uint32_t slice = vehicle + 0xC00;
          guest::StoreU16(h.base, slice + 58, uint16_t(int32_t(r.Below(8)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU16(h.base, slice + 60 + 4 * i, uint16_t(r.Below(8)));
            guest::StoreU16(h.base, slice + 62 + 4 * i, uint16_t(r.Below(41)));
          }
          guest::StoreU32(h.base, io + 0x100 + 52, slice);
          // The path: its records (the data, their tables, numbers, offsets
          // and directions), count, entries' count and sections.
          guest::StoreU16(h.base, path + 5162, uint16_t(r.Below(10)));
          guest::StoreU16(h.base, path + 5136, uint16_t(r.Below(81)));
          guest::StoreU16(h.base, path + 5144, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU16(h.base, path + 5146 + 2 * i, uint16_t(r.Below(7)));
          }
          // Most corners share the path's sections (copies of its list).
          for (uint32_t i = 0; i < 4; i++) {
            const uint32_t data = vehicle + 0x300 * i;
            if (r.Below(4) == 0) {
              continue;
            }
            guest::StoreU16(h.base, data + 2, guest::LoadU16(h.base, path + 5144));
            for (uint32_t k = 0; k < 8; k++) {
              guest::StoreU16(h.base, data + 36 + 2 * k, guest::LoadU16(h.base, path + 5146 + 2 * k));
            }
          }
          for (uint32_t i = 0; i < 9; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            const uint32_t data = r.Below(8) == 0 ? 0 : vehicle + 0x300 * r.Below(4);
            guest::StoreU32(h.base, record + 0, data);
            guest::StoreU32(h.base, record + 4, data == 0 ? 0 : data + 56);
            guest::StoreU16(h.base, record + 8, uint16_t(r.Below(10)));
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(41)) - 10));
            guest::StoreU16(h.base, record + 12, uint16_t(r.Below(90)));
            guest::StoreU16(h.base, record + 14, uint16_t(r.Below(51)));
            guest::StoreU32(h.base, record + 16, FloatBits(r.Below(2) ? 1.0f : -1.0f));
          }
          r3 = path;
          r4 = io + 0x100;
          for (uint32_t i = 0; i < 9; i++) {
            floats.push_back({kDriver + kPath + 5164 + 44 * i + 28, 2});
          }
          break;
        }
        case kLimitForCorner:
        case kUpdateTargetSpeed:
        case kUpdateSpeed:
        case kDrive: {
          // Two corners' data in the vehicle's bytes (0x300 apart: +0 the
          // table's count, +8 to +28 the speeds, +50 to +54 the entry,
          // middle and exit, the table from +56) and up to three corner
          // records (+5676); the path's entries (+5136), start (+5138) and
          // look-ahead (+5142); the slice length.
          guest::StoreU32(h.base, driver + 160, vehicle);  // read without a check
          guest::StoreU32(h.base, area + kFile + 8, FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          for (uint32_t i = 0; i < 2; i++) {
            const uint32_t data = vehicle + 0x300 * i;
            guest::StoreU16(h.base, data + 0, uint16_t(r.Below(13)));
            for (uint32_t k = 8; k <= 28; k += 4) {
              guest::StoreU32(h.base, data + k, RandomFloat(r, 200.0f));
            }
            for (uint32_t k = 50; k <= 54; k += 2) {
              guest::StoreU16(h.base, data + k, uint16_t(int32_t(r.Below(46)) - 5));
            }
            for (uint32_t k = 0; k < 13; k++) {
              guest::StoreU32(h.base, data + 56 + 8 * k + 4, RandomFloat(r, 30.0f));
            }
          }
          guest::StoreU16(h.base, driver + 5674, uint16_t(int32_t(r.Below(5)) - 1));
          for (uint32_t i = 0; i < 3; i++) {
            const uint32_t record = driver + 5676 + 44 * i;
            const uint32_t data = vehicle + 0x300 * r.Below(2);
            guest::StoreU32(h.base, record + 0, data);
            guest::StoreU32(h.base, record + 4, data + 56);
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(41)) - 20));
            guest::StoreU32(h.base, record + 16, FloatBits(r.Below(2) ? 1.0f : -1.0f));
            floats.push_back({kDriver + 5676 + 44 * i + 20, 1});
          }
          guest::StoreU16(h.base, driver + 5648, uint16_t(int32_t(r.Below(84)) - 2));
          guest::StoreU16(h.base, driver + 5650, uint16_t(r.Below(200)));
          guest::StoreU16(h.base, driver + 5654, uint16_t(int32_t(r.Below(14)) - 2));
          guest::StoreU32(h.base, driver + 144, RandomFloat(r, 30.0f));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          guest::StoreU32(h.base, driver + 6032, r.Below(3));
          guest::StoreU16(h.base, driver + 6056, uint16_t(r.Below(4) ? int32_t(r.Below(300)) - 150
                                                                     : int32_t(r.Next())));
          guest::StoreU16(h.base, driver + 6058, uint16_t(r.Below(4) ? int32_t(r.Below(300)) - 150
                                                                     : int32_t(r.Next())));
          guest::StoreU32(h.base, driver + 6088, RandomFloat(r, 200.0f));
          // UpdateTargetSpeed's flags and states (the others as UpdateState's
          // cases).
          const uint32_t vehicle_flags[] = {0,          0x20000000u, 0x40000000u, 0x04000000u,
                                            0x08000000u, 0x10000000u, 0x80000000u,
                                            uint32_t(r.Next())};
          guest::StoreU32(h.base, vehicle + 6148, vehicle_flags[r.Below(8)]);
          guest::StoreU32(h.base, driver + 32, r.Below(2) ? 0 : r.Below(8));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(2) ? 0x7F : r.Next()));
          const uint32_t current[] = {0xFFFFFFFFu, 0, 3, 6, 0x80000000u};
          guest::StoreU32(h.base, driver + 480, current[r.Below(5)]);
          guest::StoreU32(h.base, driver + 0, r.Below(8));
          for (uint32_t i = 0; i < 7; i++) {
            guest::StoreU32(h.base, driver + 172 + 40 * i, RandomFloat(r, 140.0f));
            guest::StoreU32(h.base, table + i * ai::kDriverBytes + 100, RandomFloat(r, 200.0f));
          }
          drivers = table;
          guest::StoreU32(h.base, io, r.Below(16) == 0 ? RandomFloat(r, 3.0f)
                                                       : FloatBits(0.75f + float(r.Below(50)) / 100.0f));
          r4 = io;  // the scale (LimitForCorner, UpdateTargetSpeed)
          r5 = driver + 5676 + 44 * r.Below(3);  // the corner record (LimitForCorner)
          floats.push_back({kDriver + 6088, 1});
          if (f.kind == kDrive) {
            // The steering (its corner records as UpdateSpeed's), SteerToTarget's
            // vehicle fields and the driver's +156, UpdatePedals' and the
            // pedals, the skill's +12 (+6060) and the input record (io + 0x100).
            steering(false);
            guest::StoreU32(h.base, driver + 156, RandomFloat(r, 20.0f));
            for (uint32_t at : {92u, 384u, 392u}) {
              guest::StoreU32(h.base, vehicle + at, RandomFloat(r, 4.0f));
            }
            guest::StoreU32(h.base, vehicle + 3548, r.Below(4));
            guest::StoreU32(h.base, driver + 164, RandomFloat(r, 200.0f));
            guest::StoreU32(h.base, driver + 168, RandomFloat(r, 300.0f));
            for (uint32_t at : {6124u, 6128u, 6144u}) {
              guest::StoreU32(h.base, driver + at, RandomFloat(r, 1.6f));
            }
            guest::StoreU16(h.base, driver + 6060, uint16_t(int32_t(r.Below(5)) - 2));
            r4 = io + 0x100;
            floats.push_back({kDriver + 152, 1});
            floats.push_back({kDriver + 6096, 3});
            floats.push_back({kDriver + 6112, 5});
            floats.push_back({kDriver + 6144, 1});
          }
          break;
        }
        case kSteerToTarget: {
          // The path's corners (IsSlow), ring (TargetPoint: its count, start
          // and look-ahead), the slice length, the driver's point, heading and
          // speed, the vehicle's +92, +384 and +392.
          guest::StoreU32(h.base, driver + 160, vehicle);  // read without a check
          const uint32_t path = driver + kPath;
          guest::StoreU16(h.base, path + 5162, uint16_t(int32_t(r.Below(5)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(7)) - 4));
            guest::StoreU32(h.base, record + 40, r.Below(6));
          }
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU16(h.base, path + 5136, uint16_t(int32_t(r.Below(85)) - 2));
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          guest::StoreU32(h.base, driver + 156, RandomFloat(r, 20.0f));
          for (uint32_t at : {80u, 88u, 96u}) {
            guest::StoreU32(h.base, driver + at, RandomFloat(r, 100.0f));
          }
          // Now and then the target point at the driver's (no angle).
          if (r.Below(16) == 0) {
            for (uint32_t k = 0; k < 80; k++) {
              const uint32_t entry = path + 16 + 64 * k;
              guest::StoreU32(h.base, entry + 16, guest::LoadU32(h.base, driver + 80));
              guest::StoreU32(h.base, entry + 24, guest::LoadU32(h.base, driver + 88));
            }
          }
          for (uint32_t at : {92u, 384u, 392u}) {
            guest::StoreU32(h.base, vehicle + at, RandomFloat(r, 4.0f));
          }
          floats.push_back({kDriver + 152, 1});
          floats.push_back({kDriver + 6096, 3});
          floats.push_back({kDriver + 6112, 2});
          break;
        }
        case kUpdatePedals: {
          // The path's corners (HasKind2Ahead), the speeds (ThrottleForSpeed),
          // the slide (EaseThrottleForSlide), the requests and pedals.
          guest::StoreU32(h.base, driver + 160, r.Below(8) == 0 ? 0 : vehicle);
          const uint32_t path = driver + kPath;
          guest::StoreU16(h.base, path + 5162, uint16_t(int32_t(r.Below(5)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(7)) - 4));
            guest::StoreU32(h.base, record + 40, r.Below(4));
          }
          guest::StoreU32(h.base, vehicle + 3548, r.Below(4));
          guest::StoreU32(h.base, driver + 100, RandomFloat(r, 300.0f));
          guest::StoreU32(h.base, driver + 6088, RandomFloat(r, 300.0f));
          guest::StoreU32(h.base, driver + 168, RandomFloat(r, 300.0f));
          for (uint32_t at : {104u, 152u, 6112u, 6116u, 6120u}) {
            guest::StoreU32(h.base, driver + at, RandomFloat(r, 1.5f));
          }
          for (uint32_t at : {6124u, 6128u, 6140u, 6144u}) {
            guest::StoreU32(h.base, driver + at, RandomFloat(r, 1.6f));
          }
          floats.push_back({kDriver + 6124, 2});
          floats.push_back({kDriver + 6140, 2});
          break;
        }
        case kApplyCornerLine:
        case kApplyCornerLines: {
          // Three corners' data in the vehicle's bytes (0x300 apart: +0 the
          // table's count, +50 and +52, the table's floats from +56), nine
          // records (+5164: the data, the table, +10, +14, the slopes +28 and
          // +32, the bytes +36 and +37), the ring's count and start.
          const uint32_t path = driver + kPath;
          for (uint32_t i = 0; i < 3; i++) {
            const uint32_t data = vehicle + 0x300 * i;
            guest::StoreU16(h.base, data + 0, uint16_t(int32_t(r.Below(40)) - 2));
            guest::StoreU16(h.base, data + 50, uint16_t(int32_t(r.Below(46)) - 5));
            guest::StoreU16(h.base, data + 52, uint16_t(int32_t(r.Below(46)) - 5));
            for (uint32_t k = 0; k < 40; k++) {
              guest::StoreU32(h.base, data + 56 + 8 * k + 4, RandomFloat(r, 30.0f));
            }
          }
          for (uint32_t i = 0; i < 9; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            const uint32_t data = r.Below(8) == 0 ? 0 : vehicle + 0x300 * r.Below(3);
            guest::StoreU32(h.base, record + 0, data);
            guest::StoreU32(h.base, record + 4, data == 0 || r.Below(10) == 0 ? 0 : data + 56);
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(100)) - 30));
            guest::StoreU16(h.base, record + 14, uint16_t(int32_t(r.Below(43)) - 3));
            guest::StoreU32(h.base, record + 28, RandomFloat(r, 2.0f));
            guest::StoreU32(h.base, record + 32, RandomFloat(r, 2.0f));
            guest::StoreU8(h.base, record + 36, uint8_t(r.Below(3)));
            guest::StoreU8(h.base, record + 37, uint8_t(r.Below(3)));
          }
          guest::StoreU16(h.base, path + 5136, uint16_t(int32_t(r.Below(85)) - 2));
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          guest::StoreU16(h.base, path + 5162, uint16_t(int32_t(r.Below(10)) - 1));
          guest::StoreU32(h.base, io, r.Below(9));
          r3 = path;
          r4 = io;
          for (uint32_t k = 0; k < 80; k++) {
            floats.push_back({kPath + 16 + 64 * k + 48, 1});
          }
          break;
        }
        case kKeepToCornerSide:
        case kSetRanges: {
          // The corner records' +10 and directions (+16), the ring's start,
          // the count (io), the margin and shift (io + 4, + 8: KeepToCornerSide)
          // or the driver's +116, +128, +152 and the mode (SetRanges).
          const uint32_t path = driver + kPath;
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(100)) - 20));
            guest::StoreU32(h.base, record + 16, r.Below(4) == 0 ? RandomFloat(r, 2.0f)
                                                 : FloatBits(r.Below(2) ? 1.0f : -1.0f));
          }
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          guest::StoreU16(h.base, path + 5162, uint16_t(int32_t(r.Below(10)) - 1));
          guest::StoreU32(h.base, io, uint32_t(int32_t(r.Below(84)) - 2));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, io + 8, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, path + 0, driver);
          guest::StoreU32(h.base, path + 5520, r.Below(3));
          guest::StoreU32(h.base, driver + 116, RandomFloat(r, 3.0f));
          guest::StoreU32(h.base, driver + 128, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, driver + 152, RandomFloat(r, 10.0f));
          r3 = path;
          if (f.kind == kKeepToCornerSide) {
            r4 = path + 5164 + 44 * r.Below(8);
            r5 = io;
            r6 = io + 4;
            r7 = io + 8;
          } else {
            r4 = io;
          }
          for (uint32_t k = 0; k < 80; k++) {
            floats.push_back({kPath + 16 + 64 * k + 40, 2});
          }
          break;
        }
        case kDropOffRouteCorners: {
          // Four corners' data in the vehicle's bytes (0x300 apart: +2 and
          // +36 its sections, +52), the path's sections (+5144, +5146; most
          // corners share them), nine records (+5164: the data, +14, +37).
          const uint32_t path = driver + kPath;
          guest::StoreU16(h.base, path + 5144, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU16(h.base, path + 5146 + 2 * i, uint16_t(r.Below(7)));
          }
          for (uint32_t i = 0; i < 4; i++) {
            const uint32_t data = vehicle + 0x300 * i;
            const bool shares = r.Below(5) != 0;
            guest::StoreU16(h.base, data + 2, shares ? guest::LoadU16(h.base, path + 5144)
                                                     : uint16_t(int32_t(r.Below(8)) - 1));
            for (uint32_t k = 0; k < 8; k++) {
              guest::StoreU16(h.base, data + 36 + 2 * k,
                              shares ? guest::LoadU16(h.base, path + 5146 + 2 * k)
                                     : uint16_t(r.Below(7)));
            }
            guest::StoreU16(h.base, data + 52, uint16_t(int32_t(r.Below(50)) - 5));
          }
          guest::StoreU16(h.base, path + 5162, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 9; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            guest::StoreU32(h.base, record + 0,
                            r.Below(10) == 0 ? 0 : vehicle + 0x300 * r.Below(4));
            guest::StoreU16(h.base, record + 14, uint16_t(int32_t(r.Below(60)) - 5));
            guest::StoreU8(h.base, record + 37, uint8_t(r.Below(3)));
          }
          r3 = path;
          break;
        }
        case kSpreadOffset:
        case kClampOffset: {
          // The ring (count, start; each entry's offset +24 and range +32 to
          // +36, mostly ordered), the indexes (io, io + 4: first and last,
          // the last within the count) and the amount (io + 8).
          const uint32_t path = driver + kPath;
          const uint32_t count = 40 + r.Below(41);
          guest::StoreU16(h.base, path + 5136, uint16_t(count));
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          for (uint32_t k = 0; k < 80; k++) {
            const uint32_t entry = path + 16 + 64 * k;
            uint32_t low = RandomFloat(r, 3.0f), high = RandomFloat(r, 3.0f);
            if (r.Below(8) != 0 && std::bit_cast<float>(low) > std::bit_cast<float>(high)) {
              std::swap(low, high);
            }
            guest::StoreU32(h.base, entry + 24, RandomFloat(r, 4.0f));
            guest::StoreU32(h.base, entry + 32, low);
            guest::StoreU32(h.base, entry + 36, high);
          }
          const uint32_t last = r.Below(count + 1);
          const int32_t first = r.Below(8) == 0 ? int32_t(r.Below(last + 3)) - 2
                                                : int32_t(r.Below(last + 2));
          guest::StoreU32(h.base, io, uint32_t(first));
          guest::StoreU32(h.base, io + 4, last);
          guest::StoreU32(h.base, io + 8, RandomFloat(r, 6.0f));
          r3 = path;
          if (f.kind == kSpreadOffset) {
            r4 = io;
            r5 = io + 4;
            r6 = io + 8;
          } else {
            r4 = path + 16 + 64 * r.Below(80);
            r5 = io + 4;
            r6 = io;
          }
          for (uint32_t k = 0; k < 80; k++) {
            floats.push_back({kPath + 16 + 64 * k + 24, 1});
          }
          break;
        }
        case kContinueLine:
        case kApproachCorner:
        case kCornerToEntry:
        case kCornerToApex:
        case kCornerToExit:
        case kPlanCorners:
        case kPlanLine:
        case kFollowTargets: {
          // The ring (count, start, look-ahead +5140 within the count; each
          // entry's +24, +28, ranges +32 to +36 and +40 to +44 (mostly
          // ordered) and target +48), the step limit +5516, the index (io),
          // the entry before (io + 4: a ring entry or one at kOut), the first
          // index (io + 8) and a corner record at io + 0x20 (its data in the
          // vehicle's bytes with +50, +52, +54; its table's +4; the offset
          // +10 and side +16).
          const uint32_t path = driver + kPath;
          const uint32_t count = 30 + r.Below(51);
          const uint32_t look = r.Below(4) == 0 ? r.Below(count + 1) : count - r.Below(6);
          guest::StoreU16(h.base, path + 5136, uint16_t(count));
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          guest::StoreU16(h.base, path + 5140, uint16_t(look));
          guest::StoreU32(h.base, path + 5516,
                          r.Below(8) == 0 ? RandomFloat(r, 1.0f)
                                          : FloatBits(0.01f + float(r.Below(100)) / 200.0f));
          const auto range = [&](uint32_t at, float size) {
            uint32_t low = RandomFloat(r, size), high = RandomFloat(r, size);
            if (r.Below(8) != 0 && std::bit_cast<float>(low) > std::bit_cast<float>(high)) {
              std::swap(low, high);
            }
            guest::StoreU32(h.base, at, low);
            guest::StoreU32(h.base, at + 4, high);
          };
          const auto fill = [&](uint32_t entry) {
            guest::StoreU32(h.base, entry + 24, RandomFloat(r, 4.0f));
            guest::StoreU32(h.base, entry + 28, RandomFloat(r, 0.5f));
            range(entry + 32, 5.0f);
            range(entry + 40, 4.0f);
            guest::StoreU32(h.base, entry + 48, RandomFloat(r, 4.0f));
          };
          for (uint32_t k = 0; k < 80; k++) {
            fill(path + 16 + 64 * k);
          }
          fill(area + kOut);
          const uint32_t index = r.Below(16) == 0 ? r.Below(look + 3) : r.Below(look / 2 + 1);
          guest::StoreU32(h.base, io, index);
          const uint32_t before =
              r.Below(4) == 0 ? area + kOut : path + 16 + 64 * r.Below(80);
          guest::StoreU32(h.base, io + 4, before);
          guest::StoreU32(h.base, io + 8, r.Below(8) == 0 ? index + 1 : r.Below(index + 1));
          const uint32_t record = io + 0x20, data = vehicle + 0x40;
          guest::StoreU32(h.base, record + 0, data);
          guest::StoreU32(h.base, record + 4, vehicle + 0x80);
          guest::StoreU32(h.base, vehicle + 0x84, RandomFloat(r, 4.0f));
          const int32_t offset = int32_t(r.Below(look + 4)) - 4;
          guest::StoreU16(h.base, record + 10, uint16_t(offset));
          guest::StoreU32(h.base, record + 16, r.Below(8) == 0 ? RandomFloat(r, 2.0f)
                                               : FloatBits(r.Below(2) ? 1.0f : -1.0f));
          for (uint32_t at : {50u, 52u, 54u}) {
            guest::StoreU16(h.base, data + at, uint16_t(int32_t(r.Below(40)) - 8));
          }
          // PlanCorners and PlanLine: up to four corner records from +5164
          // (now and then without data or a table, or a count of 0 to 8 or
          // negative), their data four in the vehicle's bytes, 0x40 apart;
          // the end index at io + 8.
          if (f.kind == kPlanCorners || f.kind == kPlanLine) {
            const int32_t corners = r.Below(8) == 0 ? int32_t(r.Below(10)) - 1
                                                    : int32_t(r.Below(5));
            guest::StoreU16(h.base, path + 5162, uint16_t(corners));
            int32_t at = int32_t(r.Below(6)) - 2;
            for (uint32_t i = 0; i < 4; i++) {
              const uint32_t corner = path + 5164 + 44 * i;
              const uint32_t its = vehicle + 0x100 + 0x40 * i;
              guest::StoreU32(h.base, corner + 0, r.Below(10) == 0 ? 0 : its);
              guest::StoreU32(h.base, corner + 4, r.Below(10) == 0 ? 0 : vehicle + 0x80);
              at += int32_t(r.Below(look / 3 + 2)) - 1;
              guest::StoreU16(h.base, corner + 10, uint16_t(at));
              guest::StoreU32(h.base, corner + 16, r.Below(8) == 0 ? RandomFloat(r, 2.0f)
                                                   : FloatBits(r.Below(2) ? 1.0f : -1.0f));
              int32_t point = int32_t(r.Below(4));
              for (uint32_t p : {50u, 52u, 54u}) {
                guest::StoreU16(h.base, its + p, uint16_t(point));
                point += int32_t(r.Below(look / 4 + 2)) - (r.Below(8) == 0 ? 3 : 0);
              }
            }
            guest::StoreU32(h.base, io + 8, r.Below(look + 2));
          }
          r3 = path;
          if (f.kind == kPlanLine || f.kind == kFollowTargets) {
          } else if (f.kind == kPlanCorners) {
            r4 = before;
            r5 = io;
            r6 = io + 8;
          } else if (f.kind == kContinueLine) {
            r4 = before;
            r5 = io;
            r6 = io + 8;
          } else {
            r4 = record;
            r5 = io;
            r6 = io + 4;
          }
          for (uint32_t k = 0; k < 80; k++) {
            floats.push_back({kPath + 16 + 64 * k + 24, 1});
          }
          floats.push_back({kOut + 24, 1});
          what += fmt::format(" count {} look {} index {}", count, look, index);
          break;
        }
        case kBranchMasksFor:
        case kLeaderBranchKind:
        case kFindSideSection:
        case kFollowLeaderBranch: {
          // The race flags (io + 0xF0: mostly the bits 0x4, 0x8, 0x10, 0x20
          // in any mix), a route (test_route.h) with the vehicle's position
          // (its identity +4), the kind (io: 0 to 11, now and then -1 or any)
          // and best (io + 4); FindSideSection's point (io + 0x10, the
          // section's bounds' scale) and masks (io + 0x80); FollowLeaderBranch's
          // driver +16, +488 (around -50.0), its +500 object (io + 0x40,
          // +88), the racers (up to 8, their vehicles' flags +6148) and +5528.
          const uint32_t path = driver + kPath;
          race_data = io + 0xF0 - 20;
          guest::StoreU32(h.base, io + 0xF0, r.Below(8) == 0 ? uint32_t(r.Next())
                                                           : (r.Below(64) << 2) | r.Below(4));
          const test_route::Route route =
              test_route::BuildRoute(h.base, route_area, route_table, r);
          route_file = route_area + test_route::kFile;
          test_route::RandomPosition(h.base, vehicle + 5232 + 4, route, r);
          guest::StoreU32(h.base, path + 0, driver);
          guest::StoreU32(h.base, io, r.Below(8) == 0 ? uint32_t(r.Next())
                                                    : uint32_t(int32_t(r.Below(13)) - 1));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 100.0f));
          guest::StoreU32(h.base, driver + 488, r.Below(2) ? RandomFloat(r, 100.0f)
                                                           : FloatBits(-50.0f - float(r.Below(3)) + 1.0f));
          r3 = path;
          if (f.kind == kBranchMasksFor) {
            r3 = io;
          } else if (f.kind == kLeaderBranchKind) {
            r4 = io;
            r5 = io + 4;
            floats.push_back({kIo + 4, 1});
          } else if (f.kind == kFindSideSection) {
            // Always a vehicle (the original reads its identity without a
            // check).
            guest::StoreU32(h.base, driver + 160, vehicle);
            for (uint32_t i = 0; i < 3; i++) {
              guest::StoreU32(h.base, io + 0x10 + 4 * i, RandomFloat(r, 150.0f));
            }
            const uint32_t masks = r.Below(4);
            for (uint32_t i = 0; i < masks; i++) {
              const uint32_t word = r.Below(3) == 0 ? 0xDFFFFE7Fu : uint32_t(r.Next());
              guest::StoreU32(h.base, io + 0x80 + 4 * i, word != 0 ? word : 1u);
            }
            guest::StoreU32(h.base, io + 0x80 + 4 * masks, 0);
            const uint32_t object = io + 0x40;
            guest::StoreU32(h.base, driver + 500, r.Below(6) == 0 ? 0 : object);
            guest::StoreU16(h.base, object + 88, uint16_t(r.Below(5) + 2));
            guest::StoreU8(h.base, object + 36, uint8_t(4 + r.Below(6)));
            guest::StoreU8(h.base, object + 90, uint8_t(r.Below(4)));
            r4 = io + 8;
            r5 = io + 0x10;
            r6 = r.Below(4) == 0 ? 0 : io + 0x80;
          } else {
            const uint32_t object = io + 0x40;
            guest::StoreU32(h.base, driver + 500, r.Below(6) == 0 ? 0 : object);
            guest::StoreU16(h.base, object + 88, uint16_t(r.Below(8)));
            guest::StoreU32(h.base, driver + 16, r.Below(4) == 0 ? 0 : 4u | r.Below(16));
            const uint32_t count = r.Below(9);
            guest::StoreU32(h.base, area + kRacers + 56, r.Below(16) == 0 ? uint32_t(-1) : count);
            for (uint32_t i = 0; i < 8; i++) {
              const uint32_t record = area + kRacerRecords + 24 * i;
              guest::StoreU32(h.base, area + kRacers + 4 * i, record);
              guest::StoreU32(h.base, record + 20, r.Below(8) == 0 ? uint32_t(-1) : r.Below(8));
              guest::StoreU32(h.base, vehicles + kVehicleBytes * i + 6148,
                              r.Below(4) == 0 ? 0x80u >> r.Below(8) : 0u);
              test_route::RandomPosition(h.base, vehicles + kVehicleBytes * i + 5232 + 4, route,
                                         r);
            }
            guest::StoreU32(h.base, path + 5528, uint32_t(int32_t(r.Below(9)) - 1));
          }
          break;
        }
        case kCompareWith:
        case kUpdateAll: {
          // The table's first seven drivers (written: their bytes compared by
          // hash): +0 their index, +8 the lane, +16, +100 the speed, +124,
          // +156, +160 a vehicle (one of eight, their identities' positions
          // on a route, test_route.h) or none, +6148 the update (UpdateAll:
          // one of three recompiled leaves), their seven other-racer records
          // (six floats, then the halfwords and bytes); finite floats, so
          // the hash compares them exactly. The racers (up to 8, indexes 0
          // to 6, now and then repeated) and their inputs.
          drivers = table;
          table_written = true;
          const test_route::Route route =
              test_route::BuildRoute(h.base, route_area, route_table, r);
          route_file = route_area + test_route::kFile;
          for (uint32_t v = 0; v < 8; v++) {
            test_route::RandomPosition(h.base, vehicles + kVehicleBytes * v + 5232 + 4, route, r);
          }
          const auto finite = [&](float range) {
            return FloatBits((float(r.Below(1u << 20)) / float(1u << 19) - 1.0f) * range);
          };
          const uint32_t updates[] = {0x82144ED8, 0x82145230, 0x820C96E8};
          for (uint32_t d = 0; d < 7; d++) {
            const uint32_t at = table + ai::kDriverBytes * d;
            guest::StoreU32(h.base, at + 0, r.Below(16) == 0 ? r.Below(7) : d);
            guest::StoreU16(h.base, at + 8, uint16_t(r.Below(3)));
            guest::StoreU32(h.base, at + 16, r.Below(4) | (r.Below(2) ? 4u : 0u));
            guest::StoreU32(h.base, at + 100, finite(150.0f));
            guest::StoreU32(h.base, at + 124, finite(3.0f));
            guest::StoreU32(h.base, at + 148, finite(3.0f));
            guest::StoreU32(h.base, at + 156, finite(5.0f));
            // Without a vehicle only as the other racer (the original reads
            // the driver's own vehicle without a check): driver 6, the
            // highest index.
            guest::StoreU32(h.base, at + 160, d == 6 && r.Below(3) == 0
                                                  ? 0
                                                  : vehicles + kVehicleBytes * r.Below(8));
            guest::StoreU32(h.base, at + 6148, updates[r.Below(3)]);
            for (uint32_t k = 0; k < 7; k++) {
              const uint32_t record = at + 172 + 40 * k;
              for (uint32_t w = 0; w < 6; w++) {
                guest::StoreU32(h.base, record + 4 * w, finite(w == 0 ? 100.0f : 20.0f));
              }
              guest::StoreU16(h.base, record + 24, uint16_t(r.Below(5)));
              guest::StoreU16(h.base, record + 26, uint16_t(int32_t(r.Below(80)) - 1));
              guest::StoreU16(h.base, record + 28, uint16_t(r.Below(64)));
              guest::StoreU16(h.base, record + 30, uint16_t(r.Below(130)));
              guest::StoreU8(h.base, record + 32, uint8_t(r.Below(2)));
              guest::StoreU8(h.base, record + 33, uint8_t(r.Below(2)));
              guest::StoreU16(h.base, record + 34, uint16_t(r.Below(64)));
              guest::StoreU16(h.base, record + 36, uint16_t(r.Below(4)));
            }
          }
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, area + kInputWords + 4 * i,
                            r.Below(2) ? 0x20000u : uint32_t(r.Next()));
          }
          if (f.kind == kCompareWith) {
            const uint32_t a = r.Below(6);
            guest::StoreU32(h.base, io, r.Below(16) == 0 ? a : r.Below(7));
            r3 = table + ai::kDriverBytes * a;
            r4 = io;
          } else {
            const uint32_t count = r.Below(16) == 0 ? 0 : 1 + r.Below(7);
            guest::StoreU32(h.base, area + kRacers + 56, count);
            for (uint32_t i = 0; i < 8; i++) {
              const uint32_t racer = area + kRacerRecords + 24 * i;
              guest::StoreU32(h.base, area + kRacers + 4 * i, racer);
              guest::StoreU32(h.base, racer + 20, r.Below(8) == 0 ? r.Below(7) : i % 7);
            }
            if (r.Below(16) == 0) {
              drivers = 0;
            }
          }
          break;
        }
        case kBranchMasks: {
          // The path's kinds (+5524, +5528: -1 mostly, else 0 to 11 or any
          // word), the driver's +500 object (now and then; its byte +36 any)
          // and vehicle (now and then none; its flags +6148 by the bits 0x81
          // and 0x4, its +5712 object (or none) with the word +12 around 66 to
          // 70), the race data (a block of its own, read only: its word +4 0
          // to 9, its flags +20 for BranchMasksFor, the mode +0xA3AF4 for
          // RaceAllowsSides) and the race state's +6100.
          const uint32_t path = driver + kPath;
          guest::StoreU32(h.base, path + 0, driver);
          const auto kind = [&]() {
            return r.Below(3) ? 0xFFFFFFFFu
                   : r.Below(8) == 0 ? uint32_t(r.Next())
                                     : r.Below(12);
          };
          guest::StoreU32(h.base, path + 5524, kind());
          guest::StoreU32(h.base, path + 5528, kind());
          const uint32_t object = io + 0x40;
          guest::StoreU32(h.base, driver + 500, r.Below(4) == 0 ? object : 0);
          guest::StoreU8(h.base, object + 36, uint8_t(r.Below(2) ? r.Below(12) : r.Next()));
          const uint32_t flags[] = {0, 1, 4, 5, 0x80, 0x84, 0x81, uint32_t(r.Next())};
          guest::StoreU32(h.base, vehicle + 6148, flags[r.Below(8)]);
          const uint32_t other = io + 0x80;
          guest::StoreU32(h.base, vehicle + 5712, r.Below(6) == 0 ? 0 : other);
          guest::StoreU32(h.base, other + 12, r.Below(8) == 0 ? uint32_t(r.Next())
                                                              : 64 + r.Below(8));
          race_data = race_block;
          guest::StoreU32(h.base, race_block + 20, r.Below(8) == 0 ? uint32_t(r.Next())
                                                                 : (r.Below(64) << 2) | r.Below(4));
          guest::StoreU32(h.base, race_block + 4, r.Below(8) == 0 ? uint32_t(r.Next())
                                                                : r.Below(10));
          guest::StoreU32(h.base, race_block + 0xA3AF4, r.Below(10));
          guest::StoreU32(h.base, area + kRaceState, r.Below(5));
          r3 = path;
          break;
        }
        case kKeepBranch:
        case kLeftEntry:
        case kDropPassed:
        case kExtend:
        case kUpdatePath: {
          // A route (test_route.h) and a point (io); the driver's route
          // position (+4) and lane (+8); BranchMasks' inputs (the path's
          // kinds, the driver's +500 object, the vehicle's flags, the race
          // data in its own block); the ring (its count +5136, first +5138)
          // and its entries: a route position
          // (now and then the driver's), a lane (-1 to 3), a slice record of
          // the route (now and then any bytes) made to lie near the point
          // most of the time (its center within two slice lengths, unit
          // directions +36 to +48, a width +12 and margins +16, +20), floats;
          // WithinRoadAt's +128; the corners (+5162, +5164: their +10, +12)
          // and the section list (+5144, +5146). KeepBranch's positions at io
          // + 0x60 and io + 0x64; LeftEntry's entry one of the ring's.
          const uint32_t path = driver + kPath;
          // Extend's and Update's: every section 8 slices and every link to
          // a slice 0, so the ring crosses at most two sections in the ten
          // entries a call adds: the section list (+5146, its count +5144)
          // isn't bounded by the original, which runs past its 8 entries into
          // the corners' count and records (the game drops passed sections
          // first, DropPassedSections).
          const bool extend = f.kind == kExtend || f.kind == kUpdatePath;
          const test_route::Route route =
              test_route::BuildRoute(h.base, route_area, route_table, r, extend);
          if (extend) {
            for (uint32_t section = 0; section < 8; section++) {
              for (uint32_t i = 0; i < 12; i++) {
                guest::StoreU16(h.base, route.Address(section) + 30 + 4 * i, 0);
              }
            }
          }
          route_file = route_area + test_route::kFile;
          // A slice length the game has (positive: WithinRoadAt's index, from
          // the driver's +128 over it, stays on the ring).
          const float unit = 0.5f + float(r.Below(80)) / 8.0f;
          guest::StoreU32(h.base, route_file + 8, FloatBits(unit));
          guest::StoreU32(h.base, path + 0, driver);
          test_route::RandomPosition(h.base, driver + 4, route, r);
          guest::StoreU16(h.base, driver + 8, uint16_t(int32_t(r.Below(5)) - 1));
          const float px = float(int32_t(r.Below(200)) - 100), pz = float(int32_t(r.Below(200)) - 100);
          guest::StoreU32(h.base, io + 0, r.Below(32) == 0 ? RandomFloat(r, 100.0f) : FloatBits(px));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 10.0f));
          guest::StoreU32(h.base, io + 8, r.Below(32) == 0 ? RandomFloat(r, 100.0f) : FloatBits(pz));
          // BranchMasks.
          const auto kind = [&]() { return r.Below(3) ? 0xFFFFFFFFu : r.Below(6); };
          guest::StoreU32(h.base, path + 5524, kind());
          guest::StoreU32(h.base, path + 5528, kind());
          const uint32_t object = io + 0x40;
          guest::StoreU32(h.base, driver + 500, r.Below(4) == 0 ? object : 0);
          guest::StoreU8(h.base, object + 36, uint8_t(r.Below(10)));
          const uint32_t vflags[] = {0, 4, 0x80, 0x84, 0x81};
          guest::StoreU32(h.base, vehicle + 6148, vflags[r.Below(5)]);
          guest::StoreU32(h.base, vehicle + 5712, 0);
          race_data = race_block;
          guest::StoreU32(h.base, race_block + 20, (r.Below(64) << 2) | r.Below(4));
          guest::StoreU32(h.base, race_block + 4, r.Below(8));
          guest::StoreU32(h.base, race_block + 0xA3AF4, r.Below(10));
          // The ring.
          guest::StoreU16(h.base, path + 5136, uint16_t(r.Below(8) == 0 ? int32_t(r.Below(84)) - 2
                                                                        : int32_t(1 + r.Below(12))));
          // (32768 or more only for DropPassed, which then returns: else the
          // first entry is in the ring, as in the game, whose entries all
          // have a slice record.)
          guest::StoreU16(h.base, path + 5138,
                          uint16_t(f.kind == kDropPassed && r.Below(16) == 0 ? 32768 + r.Below(100)
                                                                              : r.Below(80)));
          guest::StoreU32(h.base, driver + 128, FloatBits(float(r.Below(400)) / 20.0f));
          for (uint32_t k = 0; k < 80; k++) {
            const uint32_t entry = path + 16 + 64 * k;
            if (r.Below(4) == 0) {
              guest::StoreU32(h.base, entry + 16, guest::LoadU32(h.base, driver + 4));
            } else {
              test_route::RandomPosition(h.base, entry + 16, route, r);
            }
            guest::StoreU32(h.base, entry + 20, uint32_t(int32_t(r.Below(5)) - 1));
            const uint32_t section = r.Below(route.sections);
            const uint32_t record =
                r.Below(16) == 0 ? io + 0xC0
                                 : route.Address(section) + 124 + 84 * r.Below(route.Slices(section));
            guest::StoreU32(h.base, entry + 52, record);
            if (record != io + 0xC0 && r.Below(4) != 0) {
              const auto near = [&]() {
                return (float(r.Below(1u << 16)) / float(1u << 15) - 1.0f) * 2.0f * unit;
              };
              guest::StoreU32(h.base, record + 0, FloatBits(px + near()));
              guest::StoreU32(h.base, record + 8, FloatBits(pz + near()));
              const float a = float(r.Below(628)) / 100.0f;
              guest::StoreU32(h.base, record + 36, FloatBits(std::cos(a)));
              guest::StoreU32(h.base, record + 40, FloatBits(std::sin(a)));
              guest::StoreU32(h.base, record + 44, FloatBits(-std::sin(a)));
              guest::StoreU32(h.base, record + 48, FloatBits(std::cos(a)));
              guest::StoreU32(h.base, record + 12, FloatBits(float(1 + r.Below(12))));
              guest::StoreU32(h.base, record + 16, FloatBits(float(r.Below(8)) / 2.0f));
              guest::StoreU32(h.base, record + 20, FloatBits(float(r.Below(8)) / 2.0f));
            }
            for (uint32_t at : {0u, 8u, 24u, 40u, 44u}) {
              guest::StoreU32(h.base, entry + at, RandomFloat(r, 8.0f));
            }
            floats.push_back({kPath + 16 + 64 * k, 3});
            floats.push_back({kPath + 16 + 64 * k + 24, 1});
          }
          // The corners and the section list.
          guest::StoreU16(h.base, path + 5162, uint16_t(int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t record = path + 5164 + 44 * i;
            guest::StoreU16(h.base, record + 12, uint16_t(int32_t(r.Below(14)) - 3));
            guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(14)) - 3));
          }
          guest::StoreU16(h.base, path + 5144, uint16_t(extend ? r.Below(5)
                                                               : int32_t(r.Below(10)) - 1));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU16(h.base, path + 5146 + 2 * i, uint16_t(r.Below(route.sections)));
          }
          r3 = path;
          if (f.kind == kKeepBranch) {
            test_route::RandomPosition(h.base, io + 0x60, route, r);
            if (r.Below(4) == 0) {
              guest::StoreU32(h.base, io + 0x64, guest::LoadU32(h.base, io + 0x60));
            } else {
              test_route::RandomPosition(h.base, io + 0x64, route, r);
            }
            r4 = io + 0x60;
            r5 = io + 0x64;
            r6 = io;
          } else if (f.kind == kLeftEntry) {
            r4 = io;
            r5 = path + 16 + 64 * r.Below(80);
          } else {
            r4 = io;
          }
          if (f.kind == kExtend || f.kind == kUpdatePath) {
            // The corners (as AddCornersAt's cases): four corners' data in the
            // vehicle's bytes (0x300 apart; +48 to +54 their slices), the route
            // file's corner table (+16) at vehicle + 0xD00, each route slice's
            // corner list (+58 the count, 0 to 3, entries from +60), the path's
            // corner records (+5164) and count; the look-ahead +5142, the mode
            // and its normalizer (now and then a recompiled leaf, on the
            // translated code: ClearOtherIndexes, returning the path); the
            // driver's vehicle (FindSideSection reads its route position
            // without a check); fewer entries (none a fifth of the time).
            for (uint32_t i = 0; i < 4; i++) {
              const uint32_t data = vehicle + 0x300 * i;
              guest::StoreU16(h.base, data + 0, uint16_t(r.Below(13)));
              guest::StoreU16(h.base, data + 2, uint16_t(int32_t(r.Below(8)) - 1));
              for (uint32_t k = 0; k < 8; k++) {
                guest::StoreU16(h.base, data + 36 + 2 * k, uint16_t(r.Below(route.sections)));
              }
              guest::StoreU32(h.base, data + 4, RandomFloat(r, 2.0f));
              guest::StoreU32(h.base, data + 12, RandomFloat(r, 20.0f));
              guest::StoreU32(h.base, data + 28, RandomFloat(r, 200.0f));
              guest::StoreU16(h.base, data + 48, uint16_t(r.Below(21)));
              guest::StoreU16(h.base, data + 50, uint16_t(r.Below(31)));
              guest::StoreU16(h.base, data + 52, uint16_t(r.Below(41)));
              guest::StoreU16(h.base, data + 54, uint16_t(r.Below(41)));
            }
            for (uint32_t i = 0; i < 8; i++) {
              guest::StoreU32(h.base, vehicle + 0xD00 + 4 * i,
                              r.Below(6) == 0 ? 0 : vehicle + 0x300 * r.Below(4));
            }
            guest::StoreU32(h.base, route_file + 16, vehicle + 0xD00);
            for (uint32_t section = 0; section < 8; section++) {
              for (uint32_t k = 0; k < route.Slices(section); k++) {
                const uint32_t slice = route.Address(section) + 124 + 84 * k;
                guest::StoreU16(h.base, slice + 58, uint16_t(r.Below(3) ? 0 : r.Below(4)));
                for (uint32_t i = 0; i < 3; i++) {
                  guest::StoreU16(h.base, slice + 60 + 4 * i, uint16_t(r.Below(8)));
                  guest::StoreU16(h.base, slice + 62 + 4 * i, uint16_t(r.Below(41)));
                }
              }
            }
            guest::StoreU16(h.base, path + 5162, uint16_t(r.Below(5)));
            for (uint32_t i = 0; i < 9; i++) {
              const uint32_t record = path + 5164 + 44 * i;
              const uint32_t data = r.Below(8) == 0 ? 0 : vehicle + 0x300 * r.Below(4);
              guest::StoreU32(h.base, record + 0, data);
              guest::StoreU32(h.base, record + 4, data == 0 ? 0 : data + 56);
              guest::StoreU16(h.base, record + 8, uint16_t(r.Below(10)));
              guest::StoreU16(h.base, record + 10, uint16_t(int32_t(r.Below(41)) - 10));
              guest::StoreU16(h.base, record + 12, uint16_t(r.Below(90)));
              guest::StoreU16(h.base, record + 14, uint16_t(r.Below(51)));
              guest::StoreU32(h.base, record + 16, FloatBits(r.Below(2) ? 1.0f : -1.0f));
              for (uint32_t at : {20u, 24u, 28u, 32u}) {
                guest::StoreU32(h.base, record + at, RandomFloat(r, 4.0f));
              }
              guest::StoreU8(h.base, record + 36, uint8_t(r.Below(2)));
              guest::StoreU8(h.base, record + 37, uint8_t(r.Below(2)));
              guest::StoreU32(h.base, record + 40, r.Below(4));
            }
            guest::StoreU16(h.base, path + 5136,
                            uint16_t(r.Below(5) == 0 ? 0 : r.Below(16) == 0 ? 75 + r.Below(6)
                                                                            : 1 + r.Below(12)));
            guest::StoreU16(h.base, path + 5142, uint16_t(int32_t(r.Below(30)) - 2));
            const uint32_t mode = r.Below(2);
            guest::StoreU32(h.base, path + 5520, mode);
            guest::StoreU32(h.base, path + 5532, r.Below(16) == 0 ? 0x82144ED8u
                                                 : mode == 1        ? 0x8213B428u
                                                                    : 0x8213C908u);
            guest::StoreU32(h.base, driver + 160, vehicle);
            test_route::RandomPosition(h.base, vehicle + 5232 + 4, route, r);
            guest::StoreU16(h.base, object + 88, uint16_t(r.Below(5) + 2));
            guest::StoreU8(h.base, object + 90, uint8_t(r.Below(4)));
            for (uint32_t at : {116u, 128u, 152u}) {
              guest::StoreU32(h.base, driver + at, RandomFloat(r, 3.0f));
            }
            guest::StoreU32(h.base, path + 5516, RandomFloat(r, 1.0f));
            for (uint32_t i = 0; i < 9; i++) {
              floats.push_back({kPath + 5164 + 44 * i + 28, 2});
            }
            for (uint32_t k = 0; k < 80; k++) {
              floats.push_back({kPath + 16 + 64 * k + 28, 6});
            }
          }
          if (f.kind == kUpdatePath) {
            // The point (the driver's +80), the driver at index 0 of a table
            // at the block (the others' +80, +88, +116, +144 in the block
            // past it), its +16 (bit 0 now and then), state, mask, speed and
            // records of the others (+172), its skill's percentages; the
            // racers (up to 3, indexes 0 to 2) and their vehicles' flags and
            // route positions (FollowLeaderBranch).
            guest::StoreU32(h.base, driver + 80, guest::LoadU32(h.base, io + 0));
            guest::StoreU32(h.base, driver + 84, guest::LoadU32(h.base, io + 4));
            guest::StoreU32(h.base, driver + 88, guest::LoadU32(h.base, io + 8));
            guest::StoreU16(h.base, driver + 8, uint16_t(r.Below(16) == 0 ? 0x8000 + r.Below(8)
                                                                          : r.Below(4)));
            drivers = area + kDriver;
            guest::StoreU32(h.base, driver + 0, 0);
            guest::StoreU32(h.base, driver + 16, (r.Below(8) == 0 ? 1u : 0u) |
                                                     (r.Below(2) ? 4u : 0u) | (r.Below(2) ? 2u : 0u));
            guest::StoreU32(h.base, driver + 32, r.Below(7));
            guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(8)));
            guest::StoreU32(h.base, driver + 100, FloatBits(float(r.Below(250))));
            guest::StoreU32(h.base, driver + 488, RandomFloat(r, 100.0f));
            for (uint32_t k = 0; k < 7; k++) {
              const uint32_t record = driver + 172 + 40 * k;
              guest::StoreU32(h.base, record + 0, FloatBits(float(r.Below(4001)) / 10.0f - 200.0f));
              guest::StoreU32(h.base, record + 4, FloatBits(float(r.Below(400)) / 100.0f - 2.0f));
              guest::StoreU32(h.base, record + 16, FloatBits(float(r.Below(250))));
              guest::StoreU32(h.base, record + 20, FloatBits(float(r.Below(400)) / 40.0f - 1.0f));
              guest::StoreU16(h.base, record + 24, uint16_t(r.Below(5)));
              guest::StoreU16(h.base, record + 30, uint16_t(int32_t(r.Below(4)) - 1));
              guest::StoreU8(h.base, record + 33, uint8_t(r.Below(2)));
            }
            guest::StoreU16(h.base, driver + kSkill + 14, uint16_t(r.Below(101)));
            guest::StoreU16(h.base, driver + kSkill + 16, uint16_t(r.Below(101)));
            for (uint32_t j = 1; j < 3; j++) {
              const uint32_t other = area + kDriver + ai::kDriverBytes * j;
              for (uint32_t at : {80u, 88u, 144u}) {
                guest::StoreU32(h.base, other + at, RandomFloat(r, 20.0f));
              }
              guest::StoreU32(h.base, other + 116, RandomFloat(r, 2.0f));
            }
            const uint32_t count = r.Below(4);
            guest::StoreU32(h.base, area + kRacers + 56, count);
            for (uint32_t i = 0; i < 8; i++) {
              const uint32_t racer = area + kRacerRecords + 24 * i;
              guest::StoreU32(h.base, area + kRacers + 4 * i, racer);
              guest::StoreU32(h.base, racer + 20, r.Below(8) == 0 ? r.Below(3) : i % 3);
              guest::StoreU32(h.base, vehicles + kVehicleBytes * i + 6148,
                              r.Below(4) == 0 ? 0x80u >> r.Below(8) : 0u);
              test_route::RandomPosition(h.base, vehicles + kVehicleBytes * i + 5232 + 4, route,
                                         r);
            }
            for (uint32_t k = 0; k < 7; k++) {
              floats.push_back({kDriver + 172 + 40 * k + 12, 1});
            }
            floats.push_back({kDriver + 164, 2});
            floats.push_back({kPath + 5516, 1});
          }
          break;
        }
        case kStartGroup:
        case kStartRanked: {
          // The racers (up to 7: flags 0x10, 0x4 and 0x8 in any mix, or any
          // word; an index 0 to 6, now and then -1), the race flags and a
          // route for LeaderBranchKind (as FollowLeaderBranch's cases), the
          // vehicles (their flags +6148, positions, box shapes) and the
          // table's first seven drivers: +0 their index, +16, +488 (ties
          // often, for the sort), a vehicle (StartGroup: now and then none;
          // StartRanked reads its flags without a check), the path's driver
          // word (itself: LeaderBranchKind reads it without a check) and what
          // ClearPlan and ResetToMode1 read.
          drivers = r.Below(16) == 0 ? 0 : table;
          table_written = true;
          race_data = io + 0xF0 - 20;
          guest::StoreU32(h.base, io + 0xF0, r.Below(8) == 0 ? uint32_t(r.Next())
                                                           : (r.Below(64) << 2) | r.Below(4));
          const test_route::Route route =
              test_route::BuildRoute(h.base, route_area, route_table, r);
          route_file = route_area + test_route::kFile;
          for (uint32_t v = 0; v < 8; v++) {
            const uint32_t at = vehicles + kVehicleBytes * v;
            test_route::RandomPosition(h.base, at + 5232 + 4, route, r);
            guest::StoreU32(h.base, at + 6148, r.Below(4) == 0 ? 0x80u >> r.Below(8) : 0u);
            guest::StoreU32(h.base, at + 344, r.Below(4) == 0 ? 0 : area + kShape);
          }
          const uint32_t count = r.Below(8) == 0 ? 0 : 1 + r.Below(7);
          guest::StoreU32(h.base, area + kRacers + 56, r.Below(16) == 0 ? 0xFFFFFFFFu : count);
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t racer = area + kRacerRecords + 24 * i;
            guest::StoreU32(h.base, area + kRacers + 4 * i, racer);
            const uint32_t flags = r.Below(8) == 0
                                       ? uint32_t(r.Next())
                                       : (r.Below(4) ? 0x10u : 0u) | (r.Below(3) == 0 ? 4u : 0u) |
                                             (r.Below(3) == 0 ? 8u : 0u);
            guest::StoreU32(h.base, racer + 0, flags);
            guest::StoreU32(h.base, racer + 20, r.Below(8) == 0 ? 0xFFFFFFFFu
                                                : r.Below(4) == 0 ? r.Below(7)
                                                                  : i);
          }
          for (uint32_t d = 0; d < 7; d++) {
            const uint32_t other = table + d * ai::kDriverBytes;
            guest::StoreU32(h.base, other + 0, d);
            guest::StoreU32(h.base, other + 16, r.Below(2) ? uint32_t(r.Next()) : r.Below(8));
            guest::StoreU32(h.base, other + 488, r.Below(4) == 0
                                                     ? RandomFloat(r, 100.0f)
                                                     : FloatBits(-60.0f + 10.0f * float(r.Below(5))));
            guest::StoreU32(h.base, other + 160, f.kind == kStartGroup && r.Below(6) == 0
                                                     ? 0
                                                     : vehicles + kVehicleBytes * r.Below(8));
            guest::StoreU32(h.base, other + kPath, other);
            guest::StoreU32(h.base, other + kPath + 5520, r.Below(3));
            guest::StoreU32(h.base, other + kPath + 5524,
                            r.Below(2) ? 0xFFFFFFFFu : uint32_t(int32_t(r.Below(9)) - 1));
            guest::StoreU32(h.base, other + kPath + 5528,
                            r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
          }
          what += fmt::format(" racers {}", count);
          break;
        }
        case kPlanReaction: {
          // A record at kOut: its gap +0, rate +4, speed +16 and time to meet
          // +20 near the limits (10.0 and the floats at 0x8200277C,
          // 0x82000F14, 0x820029F8: some exactly), the state +24 (0 to 3, now
          // and then others or negative), +26, +28, the timer +30 and byte
          // +33; the skill's percentages (io + 14, + 16: -20 to 120); the
          // rate (io + 0x20); the flag and mode.
          const uint32_t record = area + kOut;
          const auto near = [&](uint32_t constant, float spread) {
            const float at = guest::LoadFloat(h.base, constant);
            switch (r.Below(4)) {
              case 0:
                return FloatBits(at);
              case 1:
                return RandomFloat(r, 4.0f * spread);
              default:
                return FloatBits(at + (float(r.Below(2001)) / 1000.0f - 1.0f) * spread);
            }
          };
          guest::StoreU32(h.base, record + 0, r.Below(2) ? near(0x8200277C, 30.0f)
                                                       : near(0x82001FD8, 10.0f));
          guest::StoreU32(h.base, record + 4, r.Below(2) ? near(0x82001FBC, 1.0f)
                                                       : near(0x820029F8, 2.0f));
          guest::StoreU32(h.base, record + 16, near(0x82000F14, 20.0f));
          guest::StoreU32(h.base, record + 20, r.Below(4) == 0 ? RandomFloat(r, 20.0f)
                                                               : FloatBits(float(r.Below(400)) / 40.0f - 1.0f));
          guest::StoreU16(h.base, record + 24,
                          uint16_t(r.Below(8) == 0 ? int32_t(r.Below(10)) - 3 : int32_t(r.Below(4))));
          guest::StoreU16(h.base, record + 26, uint16_t(r.Next()));
          guest::StoreU16(h.base, record + 28, uint16_t(r.Below(4) ? r.Below(32) : r.Next()));
          guest::StoreU16(h.base, record + 30, uint16_t(int32_t(r.Below(6)) - 2));
          guest::StoreU8(h.base, record + 33, uint8_t(r.Below(4) ? r.Below(2) : r.Below(256)));
          guest::StoreU16(h.base, io + 14, uint16_t(int32_t(r.Below(141)) - 20));
          guest::StoreU16(h.base, io + 16, uint16_t(int32_t(r.Below(141)) - 20));
          guest::StoreU32(h.base, io + 0x20, r.Below(8) == 0 ? RandomFloat(r, 10.0f)
                                                             : FloatBits(float(r.Below(100)) / 20.0f));
          r3 = record;
          r4 = io;
          r5 = io + 0x20;
          r6 = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2);
          r7 = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3);
          floats.push_back({kOut + 12, 1});
          break;
        }
        case kReactToOthers: {
          // The driver at kDriver (its index +0, 0 to 6; the mask +40, the
          // state +32, +16, +100, its vehicle's flags) in the table at its
          // index, so AiDriver::Of finds it (the table's drivers: their +32
          // and +40 for KeepsGapFrom); its seven records (+172: the gap, the
          // rate, the speed, the time to meet, the state, +30, +33); the
          // racers (up to 8, indexes 0 to 6, now and then repeated), a route
          // file (its slice length) and the count (io).
          const uint32_t index = r.Below(7);
          drivers = area + kDriver - ai::kDriverBytes * index;
          guest::StoreU32(h.base, driver + 0, index);
          guest::StoreU32(h.base, driver + 32, r.Below(8) ? r.Below(7) : uint32_t(r.Next()));
          guest::StoreU16(h.base, driver + 40, uint16_t(r.Below(4) ? r.Below(128) : r.Next()));
          guest::StoreU32(h.base, driver + 100, FloatBits(float(r.Below(250))));
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          for (uint32_t k = 0; k < 7; k++) {
            const uint32_t record = driver + 172 + 40 * k;
            const float gaps[] = {-120.0f, -10.0f, 10.0f, 120.0f};
            guest::StoreU32(h.base, record + 0, r.Below(8) == 0
                                                    ? FloatBits(gaps[r.Below(4)])
                                                    : FloatBits(float(r.Below(4001)) / 10.0f - 200.0f));
            guest::StoreU32(h.base, record + 4, FloatBits(float(r.Below(400)) / 100.0f - 2.0f));
            guest::StoreU32(h.base, record + 16, FloatBits(float(r.Below(250))));
            guest::StoreU32(h.base, record + 20, FloatBits(float(r.Below(400)) / 40.0f - 1.0f));
            guest::StoreU16(h.base, record + 24, uint16_t(r.Below(5)));
            guest::StoreU16(h.base, record + 30, uint16_t(int32_t(r.Below(4)) - 1));
            guest::StoreU8(h.base, record + 33, uint8_t(r.Below(2)));
          }
          guest::StoreU16(h.base, driver + kSkill + 14, uint16_t(int32_t(r.Below(141)) - 20));
          guest::StoreU16(h.base, driver + kSkill + 16, uint16_t(int32_t(r.Below(141)) - 20));
          const uint32_t count = r.Below(16) == 0 ? 0 : 1 + r.Below(7);
          guest::StoreU32(h.base, area + kRacers + 56, count);
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t racer = area + kRacerRecords + 24 * i;
            guest::StoreU32(h.base, area + kRacers + 4 * i, racer);
            guest::StoreU32(h.base, racer + 20, r.Below(8) == 0 ? r.Below(7) : i % 7);
          }
          guest::StoreU32(h.base, io, r.Below(8) == 0 ? uint32_t(int32_t(r.Below(5)) - 2)
                                                      : r.Below(40));
          r3 = driver;
          r4 = io;
          for (uint32_t k = 0; k < 7; k++) {
            floats.push_back({kDriver + 172 + 40 * k + 12, 1});
          }
          break;
        }
        case kNarrowToOtherEx: {
          // The entry at kOut, the other at kOut + 64 (its range +32, +36 and
          // record +52: its +24 and +28, in the vehicle's bytes), the step,
          // offset, margins and limit (io to io + 16).
          const uint32_t entry = area + kOut, other = area + kOut + 64;
          guest::StoreU32(h.base, other + 52, r.Below(8) == 0 ? 0 : vehicle + 0x40);
          guest::StoreU32(h.base, vehicle + 0x40 + 24, RandomFloat(r, 4.0f));
          guest::StoreU32(h.base, vehicle + 0x40 + 28, RandomFloat(r, 4.0f));
          for (uint32_t at : {32u, 36u}) {
            guest::StoreU32(h.base, entry + at, RandomFloat(r, 4.0f));
            guest::StoreU32(h.base, other + at, RandomFloat(r, 4.0f));
          }
          for (uint32_t i = 0; i < 5; i++) {
            guest::StoreU32(h.base, io + 4 * i, RandomFloat(r, 2.0f));
          }
          r3 = entry;
          r4 = r.Below(8) == 0 ? 0 : other;
          r5 = io;
          r6 = io + 4;
          r7 = io + 8;
          r8 = io + 12;
          r9 = io + 16;
          floats.push_back({kOut + 32, 4});
          floats.push_back({kIo, 5});
          break;
        }
        case kNarrowBehind: {
          // The ring (count, start; each entry's +28, range +32, +36 and
          // record +52, one of eight in the vehicle's bytes, now and then
          // none), the step and limit (io, io + 4).
          const uint32_t path = driver + kPath;
          guest::StoreU16(h.base, path + 5136, uint16_t(int32_t(r.Below(83)) - 2));
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          for (uint32_t i = 0; i < 8; i++) {
            guest::StoreU32(h.base, vehicle + 0x40 + 16 * i + 0, RandomFloat(r, 4.0f));
            guest::StoreU32(h.base, vehicle + 0x40 + 16 * i + 4, RandomFloat(r, 4.0f));
          }
          for (uint32_t k = 0; k < 80; k++) {
            const uint32_t entry = path + 16 + 64 * k;
            guest::StoreU32(h.base, entry + 52,
                            r.Below(16) == 0 ? 0 : vehicle + 0x40 + 16 * r.Below(8) - 24);
            for (uint32_t at : {28u, 32u, 36u}) {
              guest::StoreU32(h.base, entry + at, RandomFloat(r, 4.0f));
            }
          }
          guest::StoreU32(h.base, io, RandomFloat(r, 1.0f));
          guest::StoreU32(h.base, io + 4, RandomFloat(r, 3.0f));
          r3 = path;
          r4 = path + 16 + 64 * r.Below(80);
          r5 = path + 16 + 64 * r.Below(80);
          r6 = io;
          r7 = io + 4;
          for (uint32_t k = 0; k < 80; k++) {
            floats.push_back({kPath + 16 + 64 * k + 32, 4});
          }
          break;
        }
        case kCompareBranches:
        case kNormalizeByBranch: {
          // The driver's +500 object (io + 0x40: +88, the bytes +36 and +90
          // choosing a preferred list) and vehicle flags; CompareBranches'
          // sections (io + 0x20, + 0x30: kinds +2, branch numbers +3), their
          // numbers (io + 0x60, + 0x62, now and then from the preferred
          // list); NormalizeByBranch's route, position (io), lane (io + 8);
          // the masks (io + 0x80).
          const uint32_t path = driver + kPath;
          const uint32_t object = io + 0x40;
          guest::StoreU32(h.base, path + 0, driver);
          guest::StoreU32(h.base, driver + 500, r.Below(6) == 0 ? 0 : object);
          guest::StoreU16(h.base, object + 88, uint16_t(r.Below(5) + 2));
          const uint32_t a = 4 + r.Below(6), b = r.Below(4);
          guest::StoreU8(h.base, object + 36, uint8_t(r.Below(8) == 0 ? r.Below(256) : a));
          guest::StoreU8(h.base, object + 90, uint8_t(r.Below(8) == 0 ? r.Below(256) : b));
          guest::StoreU32(h.base, vehicle + 6148, r.Below(2) ? 4u : uint32_t(r.Next()));
          const uint32_t list = 0x82008A08 + 64 * ((a - 4) * 2 + b);
          const auto number = [&]() {
            if (r.Below(2) && (a - 4) * 2 + b <= 11) {
              return guest::LoadU16(h.base, list + 2 * r.Below(4));
            }
            return uint16_t(r.Below(400));
          };
          for (uint32_t i = 0; i < 2; i++) {
            const uint32_t section = io + 0x20 + 0x10 * i;
            guest::StoreU8(h.base, section + 2,
                           uint8_t((r.Below(16) << 4) | (r.Below(3) == 0 ? r.Below(16) : 0)));
            const uint8_t branches[] = {7, 8, 0, 13, 29, 31, 32, 40};
            guest::StoreU8(h.base, section + 3,
                           uint8_t(r.Below(2) ? branches[r.Below(8)] : r.Below(256)));
          }
          guest::StoreU16(h.base, io + 0x60, number());
          guest::StoreU16(h.base, io + 0x62, number());
          const uint32_t masks = r.Below(4);
          for (uint32_t i = 0; i < masks; i++) {
            uint32_t word = r.Below(3) == 0 ? 0xDFFFFE7Fu : uint32_t(r.Next());
            if (r.Below(2)) {  // the sections' bits, one or both
              word = (r.Below(2) ? 1u << (guest::LoadU8(h.base, io + 0x23) & 31) : 0) |
                     (r.Below(2) ? 1u << (guest::LoadU8(h.base, io + 0x33) & 31) : 0);
            }
            guest::StoreU32(h.base, io + 0x80 + 4 * i, word != 0 ? word : 1u);
          }
          guest::StoreU32(h.base, io + 0x80 + 4 * masks, 0);
          r3 = path;
          if (f.kind == kCompareBranches) {
            r4 = io + 0x20;
            r5 = io + 0x30;
            r6 = io + 0x60;
            r7 = io + 0x62;
            r8 = r.Below(4) == 0 ? 0 : io + 0x80;
          } else {
            const test_route::Route route =
                test_route::BuildRoute(h.base, route_area, route_table, r);
            route_file = route_area + test_route::kFile;
            test_route::RandomPosition(h.base, io, route, r);
            guest::StoreU16(h.base, io + 8, uint16_t(r.Below(4) ? r.Below(8) : r.Next()));
            r4 = io + 0x100;
            r5 = io;
            r6 = io + 8;
            r7 = r.Below(2) ? 0 : io + 0x80;
            floats.push_back({kIo + 0x100, 3});
          }
          break;
        }
        case kMeasureOthers:
        case kAvoidOnLeft:
        case kAvoidOnRight:
        case kChooseSideForOther:
        case kAvoidByLane:
        case kAvoidOthers: {
          // The racers (their count, the driver's list +452: indexes 0 to 6,
          // now and then -1 or others), the other drivers in the table (+80,
          // +88, +116, +144), the driver's records of them (+0 the gap, +4,
          // +12, +16, +24 the kind, +26 the ring index, +28 the flags), the
          // ring (count, start, look-ahead; each entry's +20 lane, +28, +40,
          // +44 and slice record +52, one of four in the vehicle's bytes:
          // +0, +8, +12, +36, +40, the lanes +56), the slice length, the
          // path's step +5516 and mode, the driver's +100, +116, +136, +144,
          // +152, +164, +168; the MeasureOthers record (io) for the others.
          const uint32_t path = driver + kPath;
          guest::StoreU32(h.base, path + 0, driver);
          const uint32_t count = r.Below(9);
          guest::StoreU32(h.base, area + kRacers + 56, count);
          for (uint32_t i = 0; i < 8; i++) {
            const uint32_t index = r.Below(8) == 0 ? (r.Below(2) ? 0xFFFFFFFFu : r.Below(16))
                                                   : r.Below(7);
            guest::StoreU32(h.base, driver + 452 + 4 * i, index);
          }
          drivers = table;
          for (uint32_t i = 0; i < 16; i++) {
            const uint32_t other = table + i * ai::kDriverBytes;
            for (uint32_t at : {80u, 88u, 144u}) {
              guest::StoreU32(h.base, other + at, RandomFloat(r, 20.0f));
            }
            guest::StoreU32(h.base, other + 116, RandomFloat(r, 2.0f));
          }
          for (uint32_t i = 0; i < 16; i++) {
            const uint32_t record = driver + 172 + 40 * i;
            if (record + 40 > driver + 452) {
              break;
            }
            guest::StoreU32(h.base, record + 0, RandomFloat(r, 40.0f));
            guest::StoreU32(h.base, record + 4, RandomFloat(r, 10.0f));
            guest::StoreU32(h.base, record + 12, RandomFloat(r, 3.0f));
            guest::StoreU32(h.base, record + 16, RandomFloat(r, 100.0f));
            guest::StoreU16(h.base, record + 24, uint16_t(r.Below(8) == 0 ? r.Next() : r.Below(5)));
            guest::StoreU16(h.base, record + 26, uint16_t(int32_t(r.Below(90)) - 3));
            guest::StoreU16(h.base, record + 28,
                            uint16_t(r.Below(4) == 0 ? r.Next() : (1u << r.Below(6)) & 0x17));
          }
          guest::StoreU16(h.base, path + 5136, uint16_t(int32_t(r.Below(84)) - 2));
          guest::StoreU16(h.base, path + 5138, uint16_t(r.Below(200)));
          guest::StoreU16(h.base, path + 5140, uint16_t(int32_t(r.Below(90)) - 3));
          for (uint32_t i = 0; i < 4; i++) {
            const uint32_t slice = vehicle + 0x80 * i;
            for (uint32_t at : {0u, 8u, 36u, 40u}) {
              guest::StoreU32(h.base, slice + at, RandomFloat(r, 20.0f));
            }
            guest::StoreU32(h.base, slice + 12, RandomFloat(r, 12.0f));
            guest::StoreU8(h.base, slice + 56, uint8_t((r.Below(16) << 4) | r.Below(5)));
          }
          for (uint32_t k = 0; k < 80; k++) {
            const uint32_t entry = path + 16 + 64 * k;
            guest::StoreU32(h.base, entry + 20, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(5));
            for (uint32_t at : {28u, 40u, 44u}) {
              guest::StoreU32(h.base, entry + at, RandomFloat(r, 4.0f));
            }
            // None now and then for AvoidByLane, which checks (the others
            // read it without a check).
            guest::StoreU32(h.base, entry + 52, f.kind == kAvoidByLane && r.Below(16) == 0
                                                    ? 0
                                                    : vehicle + 0x80 * r.Below(4));
          }
          guest::StoreU32(h.base, area + kFile + 8,
                          r.Below(16) == 0 ? RandomFloat(r, 8.0f)
                                           : FloatBits(0.5f + float(r.Below(80)) / 8.0f));
          guest::StoreU32(h.base, path + 5516, RandomFloat(r, 1.0f));
          guest::StoreU32(h.base, path + 5520, r.Below(3));
          for (uint32_t at : {100u, 136u, 164u, 168u}) {
            guest::StoreU32(h.base, driver + at, RandomFloat(r, 200.0f));
          }
          guest::StoreU32(h.base, driver + 116, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, driver + 144, RandomFloat(r, 6.0f));
          guest::StoreU32(h.base, driver + 152, RandomFloat(r, 3.0f));
          guest::StoreU16(h.base, io + 0, uint16_t(int32_t(r.Below(12)) - 2));
          guest::StoreU16(h.base, io + 2, uint16_t(int32_t(r.Below(14)) - 2));
          for (uint32_t at : {4u, 8u, 12u, 16u}) {
            guest::StoreU32(h.base, io + at, RandomFloat(r, at == 12 ? 3.0f : 6.0f));
          }
          r3 = path;
          if (f.kind == kMeasureOthers) {
            r4 = area + kOut;
            for (uint32_t i = 0; i < 8; i++) {
              floats.push_back({kOut + 20 * i + 4, 4});
            }
          } else if (f.kind != kAvoidOthers) {
            r4 = io;
            r5 = driver + 172 + 40 * r.Below(7);
            floats.push_back({kIo + 16, 1});
          }
          for (uint32_t k = 0; k < 80; k++) {
            floats.push_back({kPath + 16 + 64 * k + 40, 2});
          }
          floats.push_back({kDriver + 164, 2});
          break;
        }
        case kSkillEntryWord:
        case kSkillEntryValue:
        case kTriggerStarts:
        case kTriggerEnds:
        case kUpdateSkill: {
          // The race state's records (its +2184, in the vehicle's bytes: a
          // count, 0 to 3, 972-byte records keyed by their word +4, their
          // +788 and +828) and tick count (+6116), the driver's +500 object
          // (io + 0x80: the key +0), +160, +484, +486, +488, +496, the
          // vehicle's +6152, the input word; the skill's source (io + 0x40:
          // three 8-byte triggers from +16, a kind and a value), +20, +36 and
          // the triggers' states (+24: a timer, the byte active).
          const uint32_t state_base = area + kRaceState - 6100;
          const uint32_t list = state_base + 2184;
          const uint32_t n = r.Below(4);
          guest::StoreU32(h.base, list, r.Below(16) == 0 ? uint32_t(int32_t(-1)) : n);
          const uint32_t object = io + 0x80;
          const int16_t key = int16_t(r.Below(6));
          guest::StoreU16(h.base, object + 0, uint16_t(key));
          for (uint32_t i = 0; i < 3; i++) {
            const uint32_t record = list + 972 * i + 4;
            guest::StoreU32(h.base, record + 4, r.Below(6));
            guest::StoreU32(h.base, record + 824, RandomFloat(r, 200.0f));
            guest::StoreU32(h.base, record + 784, uint32_t(int32_t(r.Below(400)) - 200));
          }
          guest::StoreU32(h.base, state_base + 6116,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(30 * 600));
          guest::StoreU32(h.base, driver + 500, r.Below(6) == 0 ? 0 : object);
          guest::StoreU32(h.base, driver + 488, RandomFloat(r, 150.0f));
          guest::StoreU32(h.base, driver + 496, r.Below(2) ? 0 : uint32_t(r.Next()));
          guest::StoreU16(h.base, driver + 484, uint16_t(int32_t(r.Below(5)) - 2));
          guest::StoreU16(h.base, driver + 486, uint16_t(int32_t(r.Below(5)) - 2));
          guest::StoreU32(h.base, vehicle + 6152, uint32_t(r.Next()));
          guest::StoreU32(h.base, area + kInputWords, uint32_t(r.Next()));
          const uint32_t skill = driver + kSkill;
          const uint32_t source = io + 0x40;
          guest::StoreU32(h.base, skill + 0, r.Below(8) == 0 ? 0 : source);
          guest::StoreU32(h.base, skill + 36, r.Below(8) == 0 ? r.Below(3) : 1);
          guest::StoreU32(h.base, skill + 20, RandomFloat(r, 150.0f));
          for (uint32_t i = 0; i < 3; i++) {
            const uint32_t trigger = source + 16 + 8 * i;
            guest::StoreU32(h.base, trigger + 0,
                            r.Below(8) == 0 ? (r.Below(2) ? 0xFFFFFFFFu : 15 + r.Below(4))
                                            : r.Below(15));
            const int32_t value = r.Below(4) == 0 ? int32_t(int16_t(r.Next()))
                                                  : int32_t(r.Below(400)) - 200;
            guest::StoreU16(h.base, trigger + 4, uint16_t(value));
            guest::StoreU16(h.base, skill + 24 + 4 * i, uint16_t(r.Below(3) ? r.Below(3) : r.Next()));
            guest::StoreU8(h.base, skill + 26 + 4 * i, uint8_t(r.Below(4) == 0 ? r.Below(256)
                                                                              : r.Below(2)));
          }
          const uint32_t pick = source + 16 + 8 * r.Below(3);
          if (f.kind == kSkillEntryWord || f.kind == kSkillEntryValue) {
            r3 = driver;
          } else if (f.kind == kTriggerStarts) {
            r3 = uint32_t(r.Next());
            r4 = driver;
            r5 = pick;
          } else if (f.kind == kTriggerEnds) {
            r3 = skill + 24 + 4 * r.Below(3);
            r4 = driver;
            r5 = pick;
          } else {
            r3 = skill;
            r4 = driver;
          }
          floats.push_back({kSkill + 20, 1});
          break;
        }
      }
      // Guest memory is one host mapping: the blocks copied as they are.
      const std::vector<uint8_t> inputs(h.base + area, h.base + area + kAreaBytes);
      const std::vector<uint8_t> seven =
          table_written ? std::vector<uint8_t>(h.base + table, h.base + table + kSevenBytes)
                        : std::vector<uint8_t>();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        std::memcpy(h.base + area, inputs.data(), kAreaBytes);
        if (table_written) {
          std::memcpy(h.base + table, seven.data(), kSevenBytes);
        }
        guest::StoreU32(h.base, ai::kDrivers, drivers);
        guest::StoreU32(h.base, track_route::kRoute, route_file);
        guest::StoreU32(h.base, kRaceData, race_data);
        guest::StoreU32(h.base, kRaceStateGlobal, area + kRaceState - 6100);
        guest::StoreU32(h.base, kRaceGlobal, area + kRacers - 2308);
        guest::StoreU32(h.base, kVehiclesGlobal, vehicles);
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, kInputs + 4 * i, area + kInputWords + 4 * i);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = r3;
        t.r4.u64 = r4;
        t.r5.u64 = r5;
        t.r6.u64 = r6;
        t.r7.u64 = r7;
        t.r8.u64 = r8;
        t.r9.u64 = r9;
        t.f1.u64 = f1;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        guest::StoreU32(h.base, ai::kDrivers, saved_drivers);
        guest::StoreU32(h.base, track_route::kRoute, saved_route);
        guest::StoreU32(h.base, kRaceData, saved_race);
        guest::StoreU32(h.base, kRaceStateGlobal, saved_state);
        guest::StoreU32(h.base, kRaceGlobal, saved_race_global);
        guest::StoreU32(h.base, kVehiclesGlobal, saved_vehicles);
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, kInputs + 4 * i, saved_inputs[i]);
        }
        // The float outputs as floats, then the block with them zeroed.
        std::vector<uint8_t> bytes(h.base + area, h.base + area + kAreaBytes);
        for (const auto& [offset, count] : floats) {
          out[v].Floats(h.base, area + offset, count);
          std::memset(bytes.data() + offset, 0, 4 * count);
        }
        for (uint32_t i = 0; i < kAreaBytes; i += 256) {
          out[v].Bytes(std::vector<uint8_t>(bytes.begin() + i, bytes.begin() + i + 256));
        }
        if (table_written) {  // the other drivers' records, by their hash
          const uint64_t hash = base::Hash64(h.base + table, kSevenBytes);
          out[v].Word(uint32_t(hash >> 32));
          out[v].Word(uint32_t(hash));
        }
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r3.u32);
        if (f.kind == kSpeedOverDistance || f.kind == kCornerFactor || f.kind == kLimitSpeed ||
            f.kind == kLimitSpeedBlend || f.kind == kLimitForCorner ||
            f.kind == kUpdateTargetSpeed || f.kind == kUpdateSpeed || f.kind == kSteerToTarget ||
            f.kind == kSkillEntryValue || f.kind == kTriggerStarts || f.kind == kTriggerEnds ||
            f.kind == kUpdateSkill || f.kind == kUpdateSteerInput ||
            f.kind == kSteerAtCornerPoint || f.kind == kSteerIntoCorner ||
            f.kind == kUpdateSteering || f.kind == kDrive) {
          out[v].Double(t.f1.u64);
        }
      }
      results.Check(out[0], out[1], what);
    }
    results.Report();
  }
  // AiDrivers::Create (sub_8213B1E0): each run makes its own table, so the
  // tables are compared with the words pointing into them as offsets (and
  // r3, a driver in it, the same way); both freed after.
  {
    Results results("AiDrivers::Create (sub_8213B1E0)");
    constexpr uint32_t kTableBytes = 7 * ai::kDriverBytes;
    for (int c = 0; c < 50; c++) {
      const uint32_t array = r.Below(4) == 0 ? 0 : vehicles;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, kVehiclesGlobal, array);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        (v == 0 ? Original<__imp__sub_8213B1E0> : NativeCreate)(t, h.base);
        const uint32_t made = guest::LoadU32(h.base, ai::kDrivers);
        guest::StoreU32(h.base, ai::kDrivers, saved_drivers);
        guest::StoreU32(h.base, kVehiclesGlobal, saved_vehicles);
        const auto relative = [&](uint32_t word) {
          return made != 0 && word - made < kTableBytes ? 0xA1000000u | (word - made) : word;
        };
        std::vector<uint8_t> bytes;
        if (made != 0) {
          for (uint32_t i = 0; i < kTableBytes; i += 4) {
            const uint32_t word = relative(guest::LoadU32(h.base, made + i));
            for (int b = 0; b < 4; b++) {
              bytes.push_back(uint8_t(word >> (24 - 8 * b)));
            }
          }
        }
        out[v].Bytes(bytes);
        out[v].Word(made == 0 ? 0 : 1);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(relative(t.r3.u32));
        if (made != 0) {
          memory::crt::OperatorDelete(made);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  if (g_fallbacks != 0) {
    ITC_LOG_INFO("harness: ai_drivers: {} cases handed to the original", g_fallbacks);
  }
  memory::Free(vehicles);
  memory::Free(route_table);
  memory::Free(race_block);
  memory::Free(route_area);
  memory::Free(table);
  memory::Free(area);
}

}  // namespace harness
