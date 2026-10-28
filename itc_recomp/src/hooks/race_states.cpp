// The race's states and the race's progress natively: adapters in front of
// src/race_states/ (docs/functions/race-states.md), with the register steps
// the harness shares (hooks/race_states.h).

#include "hooks/race_states.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/race_world.h"
#include "race_states/race_states.h"

ITC_FLAG_BOOL(native_race_states, true, "ITC",
              "Run the race's states and the race's progress (docs/functions/race-states.md: "
              "RaceProgress::Create sub_821698B0, Reset sub_821648E0, SetChecks sub_821649B8, "
              "ReleaseRacers sub_82163440, CountTime sub_82162E08, TickFraction sub_82163140 and "
              "Rank sub_82162D80; RouteSections::Beside sub_82162F78 and Before sub_82162E70, "
              "RoutePosition::NotAfter sub_821622A8; RaceProgress::CountHits sub_82164F08, Damage "
              "sub_82164228, DrainSp sub_821650A0, CheckLap sub_82164090 (with Time::Sub "
              "sub_82162230; the original where memcpy's copies of the course route aren't "
              "native) and CheckFinish sub_82164E18, RoutePosition::MapToSection sub_82163930) natively instead of through the translated "
              "code (the same words, bit for bit; checked by --run_harnesses=race_states)");

namespace hooks {

namespace rs = race_states;

void RaceStatesProgressCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = NativeHeapActive() ? rs::ProgressCreate(base) : std::nullopt;
  if (!r3) {
    __imp__sub_821698B0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void RaceStatesProgressReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 96;
  if (!rs::ProgressResetNative(frame)) {
    __imp__sub_821648E0(ctx, base);
    return;
  }
  rs::ProgressReset(base, ctx.r3.u32, frame);
}

void RaceStatesSetChecks(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::SetChecks(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

void RaceStatesReleaseRacers(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::ReleaseRacers(base, ctx.r3.u32, ctx.r1.u32, NativeHeapActive(),
                    RaceWorldOpponentsCall(ctx, base));
}

void RaceStatesCountTime(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CountTime(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void RaceStatesTickFraction(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::TickFraction(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112);
}

void RaceStatesRank(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::Rank(base, ctx.r3.u32, ctx.r1.u32 - 1728);
}

void RaceStatesSectionsBeside(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::SectionsBeside(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceStatesSectionsBefore(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  bool same = false;
  const int64_t r3 = rs::SectionsBefore(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &same);
  if (!same) {
    ctx.r3.s64 = r3;
  }
}

void RaceStatesPositionNotAfter(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::PositionNotAfter(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceStatesCountHits(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CountHits(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void RaceStatesDamage(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::Damage(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 160);
}

void RaceStatesDrainSp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::DrainSp(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 224);
}

void RaceStatesCheckLap(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 544;
  if (!rs::CheckLapNative(base, ctx.r4.u32, frame)) {
    __imp__sub_82164090(ctx, base);
    return;
  }
  ctx.r3.u64 = rs::CheckLap(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame);
}

void RaceStatesCheckFinish(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CheckFinish(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 144);
}

void RaceStatesMapToSection(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::MapToSection(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceStatesFadeInFree(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::FadeInFree(base, ctx.r3.u32);
}

void RaceStatesFadeOut(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::FadeOut(base);
}

void RaceStatesEndWhenFaded(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::EndWhenFaded(base, ctx.r4.u32);
}

void RaceStatesUnload(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive() || !rs::Unload(base)) {
    __imp__sub_82163920(ctx, base);
  }
}

void RaceStatesOnRollingStart(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::OnRollingStart(base, ctx.r3.u32);
}

void RaceStatesOnStandingStart(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::OnStandingStart(base, ctx.r3.u32, ctx.r1.u32);
}

void RaceStatesClearStep(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::ClearStep(base, ctx.r3.u32);
}

void RaceStatesOnQuit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::OnQuit(base, ctx.r3.u32);
}

void RaceStatesOnReplay(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::OnReplay(base, ctx.r3.u32);
}

void RaceStatesFreeStartTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::FreeStartTick(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceStatesQuitTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::QuitTick(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceStatesAfterRaceTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::AfterRaceTick(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceStatesRollingStartTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 112;
  rs::RollingStartTick(base, ctx.r3.u32, ctx.r4.u32, frame, NativeHeapActive(),
                       RaceWorldOpponentsCall(ctx, base),
                       RaceWorldStartCalls(ctx, base, frame - 272 - 224));
}

void RaceStatesRacersNear(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersNear(base, ctx.r1.u32 - 208);
}

void RaceStatesRacersClose(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersClose(base, ctx.r1.u32 - 208);
}

void RaceStatesRacersStill(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersStill(base, ctx.r3.u32);
}

void RaceStatesCountdownTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rs::CountdownTick(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112);
}

void RaceStatesStandingStartTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!rs::StandingStartTickNative(base, ctx.r3.u32)) {
    __imp__sub_821692C8(ctx, base);
    return;
  }
  const uint32_t frame = ctx.r1.u32 - 2816;
  rs::StandingCalls calls;
  calls.refresh = RaceWorldOpponentsCall(ctx, base);
  calls.start = RaceWorldStartCalls(ctx, base, frame - 400 - 224);
  calls.list = RaceWorldListRacersOriginal(ctx, base);
  calls.all_settled = [&ctx, base](uint32_t sp) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    __imp__sub_8213ADF8(ctx, base);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  };
  rs::StandingStartTick(base, ctx.r3.u32, ctx.r4.u32, frame, NativeHeapActive(), calls);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_STATES(address, step)    \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_race_states)) {  \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_RACE_STATES(821698B0, RaceStatesProgressCreate)
ITC_RACE_STATES(821648E0, RaceStatesProgressReset)
ITC_RACE_STATES(821649B8, RaceStatesSetChecks)
ITC_RACE_STATES(82163440, RaceStatesReleaseRacers)
ITC_RACE_STATES(82162E08, RaceStatesCountTime)
ITC_RACE_STATES(82162D80, RaceStatesRank)
ITC_RACE_STATES(82162F78, RaceStatesSectionsBeside)
ITC_RACE_STATES(82162E70, RaceStatesSectionsBefore)
ITC_RACE_STATES(82164F08, RaceStatesCountHits)
ITC_RACE_STATES(82164228, RaceStatesDamage)
ITC_RACE_STATES(821650A0, RaceStatesDrainSp)
ITC_RACE_STATES(82164090, RaceStatesCheckLap)
ITC_RACE_STATES(82164E18, RaceStatesCheckFinish)
ITC_RACE_STATES(82163930, RaceStatesMapToSection)
// Time::Sub (sub_82162230, never seen in play) has no adapter: its only
// caller, RaceProgress::CheckLap, is native; --native_race_states switches
// it.
ITC_RACE_STATES(82163398, RaceStatesFadeInFree)
ITC_RACE_STATES(82162310, RaceStatesFadeOut)
ITC_RACE_STATES(82168F50, RaceStatesEndWhenFaded)
ITC_RACE_STATES(82163920, RaceStatesUnload)
ITC_RACE_STATES(821632C8, RaceStatesOnRollingStart)
ITC_RACE_STATES(821643B0, RaceStatesOnStandingStart)
ITC_RACE_STATES(82163348, RaceStatesClearStep)
ITC_RACE_STATES(82163378, RaceStatesOnQuit)
ITC_RACE_STATES(821647B0, RaceStatesOnReplay)
ITC_RACE_STATES(82168B80, RaceStatesFreeStartTick)
ITC_RACE_STATES(82168500, RaceStatesQuitTick)
ITC_RACE_STATES(82167970, RaceStatesAfterRaceTick)
ITC_RACE_STATES(82166E50, RaceStatesRollingStartTick)
ITC_RACE_STATES(82165AB0, RaceStatesRacersNear)
ITC_RACE_STATES(82165C78, RaceStatesRacersClose)
ITC_RACE_STATES(82164A90, RaceStatesRacersStill)
ITC_RACE_STATES(821674A0, RaceStatesCountdownTick)
ITC_RACE_STATES(821692C8, RaceStatesStandingStartTick)
// RaceProgress::TickFraction (sub_82163140) and RoutePosition::NotAfter
// (sub_821622A8) have no adapters: their callers, RaceProgress::CheckLap and
// CheckFinish, are native; --native_race_states switches those callers.
// Their register steps stay for the harness.
