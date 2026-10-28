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
              "RoutePosition::NotAfter sub_821622A8) natively instead of through the translated "
              "code (the same words, bit for bit; checked by --run_harnesses=race_states)");

namespace hooks {

namespace rs = race_states;

void RaceStatesProgressCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = rs::ProgressCreate(base);
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
ITC_RACE_STATES(82163140, RaceStatesTickFraction)
ITC_RACE_STATES(82162D80, RaceStatesRank)
ITC_RACE_STATES(82162F78, RaceStatesSectionsBeside)
ITC_RACE_STATES(82162E70, RaceStatesSectionsBefore)
ITC_RACE_STATES(821622A8, RaceStatesPositionNotAfter)
