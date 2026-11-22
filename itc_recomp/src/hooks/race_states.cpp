// The race's states and the race's progress natively: adapters in front of
// src/race_states/ (docs/functions/race-states.md), with the register steps
// the harness shares (hooks/race_states.h).

#include "hooks/race_states.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/race_world.h"
#include "native/buffers.h"
#include "race_states/race_states.h"
#include "schema/race_states.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_race_states, true, "ITC",
              "Run the race's states and the race's progress (docs/functions/race-states.md: "
              "RaceProgress::Create sub_821698B0, Reset sub_821648E0, SetChecks sub_821649B8, "
              "ReleaseRacers sub_82163440, CountTime sub_82162E08, TickFraction sub_82163140 and "
              "Rank sub_82162D80; RouteSections::Beside sub_82162F78 and Before sub_82162E70, "
              "RoutePosition::NotAfter sub_821622A8; RaceProgress::CountHits sub_82164F08, Damage "
              "sub_82164228, DrainSp sub_821650A0, CheckLap sub_82164090 (with Time::Sub "
              "sub_82162230; the original where memcpy's copies of the course route aren't "
              "native) and CheckFinish sub_82164E18, RoutePosition::MapToSection sub_82163930; RaceStates::Begin sub_82164DC0, "
              "RaceProgress::FollowRoute sub_82163C50 with sub_82163000, RaceTick sub_82167610, OnGo sub_82164458; "
              "RaceProgress::StartRoute sub_821635D0 with RouteDistance sub_82162330, the "
              "original where memcpy's copies of the course route aren't native; RaceStates::Init "
              "sub_82169920; RaceStates::OnRaceEnd sub_821644D8 with the course record, "
              "CourseEntry::Clear sub_82105BC8, CourseRecord::Place sub_82105EA0, Link "
              "sub_82105D60, Unlink sub_82105DC8, Insert sub_82105E30, Add sub_82105F50 and "
              "CourseRecords::Save sub_82105C18, never seen in play, and Time::Add "
              "sub_820FF458) natively instead of through the translated "
              "code (the same words, bit for bit; checked by --run_harnesses=race_states)");

namespace hooks {

namespace rs = race_states;

ITC_REGS_STEP(RaceStatesProgressCreate) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = NativeHeapActive() ? rs::ProgressCreate(base) : std::nullopt;
  if (!r3) {
    lifted::original::c821698B0(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(RaceStatesProgressReset) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 96;
  if (!rs::ProgressResetNative(frame)) {
    lifted::original::c821648E0(ctx, base);
    return;
  }
  rs::ProgressReset(base, ctx.r3.u32, frame);
}

ITC_REGS_STEP(RaceStatesSetChecks) {
  ctx.fpscr.disableFlushMode();
  rs::SetChecks(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

ITC_REGS_STEP(RaceStatesReleaseRacers) {
  ctx.fpscr.disableFlushMode();
  rs::ReleaseRacers(base, ctx.r3.u32, ctx.r1.u32, NativeHeapActive(),
                    RaceWorldOpponentsCall(ctx, base));
}

ITC_REGS_STEP(RaceStatesCountTime) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CountTime(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(RaceStatesTickFraction) {
  ctx.fpscr.disableFlushMode();
  rs::TickFraction(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112);
}

ITC_REGS_STEP(RaceStatesRank) {
  ctx.fpscr.disableFlushMode();
  rs::Rank(base, ctx.r3.u32, ctx.r1.u32 - 1728);
}

ITC_REGS_STEP(RaceStatesSectionsBeside) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::SectionsBeside(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceStatesSectionsBefore) {
  ctx.fpscr.disableFlushMode();
  bool same = false;
  const int64_t r3 = rs::SectionsBefore(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, &same);
  if (!same) {
    ctx.r3.s64 = r3;
  }
}

ITC_REGS_STEP(RaceStatesPositionNotAfter) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::PositionNotAfter(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceStatesCountHits) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CountHits(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(RaceStatesDamage) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::Damage(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 160);
}

ITC_REGS_STEP(RaceStatesDrainSp) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::DrainSp(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 224);
}

ITC_REGS_STEP(RaceStatesCheckLap) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 544;
  if (!rs::CheckLapNative(base, ctx.r4.u32, frame)) {
    lifted::original::c82164090(ctx, base);
    return;
  }
  ctx.r3.u64 = rs::CheckLap(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, frame);
}

ITC_REGS_STEP(RaceStatesCheckFinish) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::CheckFinish(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 144);
}

ITC_REGS_STEP(RaceStatesMapToSection) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::MapToSection(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceStatesFadeInFree) {
  ctx.fpscr.disableFlushMode();
  rs::FadeInFree(base, ctx.r3.u32);
}

ITC_REGS_STEP(RaceStatesFadeOut) {
  ctx.fpscr.disableFlushMode();
  rs::FadeOut(base);
}

ITC_REGS_STEP(RaceStatesEndWhenFaded) {
  ctx.fpscr.disableFlushMode();
  rs::EndWhenFaded(base, ctx.r4.u32);
}

ITC_REGS_STEP(RaceStatesUnload) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive() || !rs::Unload(base)) {
    lifted::original::c82163920(ctx, base);
  }
}

ITC_REGS_STEP(RaceStatesOnRollingStart) {
  ctx.fpscr.disableFlushMode();
  rs::OnRollingStart(base, ctx.r3.u32);
}

ITC_REGS_STEP(RaceStatesOnStandingStart) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::OnStandingStart(base, ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceStatesClearStep) {
  ctx.fpscr.disableFlushMode();
  rs::ClearStep(base, ctx.r3.u32);
}

ITC_REGS_STEP(RaceStatesOnQuit) {
  ctx.fpscr.disableFlushMode();
  rs::OnQuit(base, ctx.r3.u32);
}

ITC_REGS_STEP(RaceStatesOnReplay) {
  ctx.fpscr.disableFlushMode();
  rs::OnReplay(base, ctx.r3.u32);
}

ITC_REGS_STEP(RaceStatesFreeStartTick) {
  ctx.fpscr.disableFlushMode();
  rs::FreeStartTick(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceStatesQuitTick) {
  ctx.fpscr.disableFlushMode();
  rs::QuitTick(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceStatesAfterRaceTick) {
  ctx.fpscr.disableFlushMode();
  rs::AfterRaceTick(base, ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceStatesRollingStartTick) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 112;
  rs::RollingStartTick(base, ctx.r3.u32, ctx.r4.u32, frame, NativeHeapActive(),
                       RaceWorldOpponentsCall(ctx, base),
                       RaceWorldStartCalls(ctx, base, frame - 272 - 224));
}

ITC_REGS_STEP(RaceStatesRacersNear) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersNear(base, ctx.r1.u32 - 208);
}

ITC_REGS_STEP(RaceStatesRacersClose) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersClose(base, ctx.r1.u32 - 208);
}

ITC_REGS_STEP(RaceStatesRacersStill) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::RacersStill(base, ctx.r3.u32);
}

ITC_REGS_STEP(RaceStatesCountdownTick) {
  ctx.fpscr.disableFlushMode();
  rs::CountdownTick(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112);
}

ITC_REGS_STEP(RaceStatesStandingStartTick) {
  ctx.fpscr.disableFlushMode();
  if (!rs::StandingStartTickNative(base, ctx.r3.u32)) {
    lifted::original::c821692C8(ctx, base);
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
    lifted::original::c8213ADF8(ctx, base);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u32;
  };
  rs::StandingStartTick(base, ctx.r3.u32, ctx.r4.u32, frame, NativeHeapActive(), calls);
}

ITC_REGS_STEP(RaceStatesBegin) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 112;
  if (!rs::ProgressResetNative(frame - 96)) {
    lifted::original::c82164DC0(ctx, base);
    return;
  }
  rs::Begin(base, ctx.r3.u32, frame);
}

ITC_REGS_STEP(RaceStatesFollowRoute) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::FollowRoute(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 240);
}

ITC_REGS_STEP(RaceStatesSectionsJoin) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::SectionsJoin(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceStatesStartRoute) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 496;
  if (!rs::StartRouteNative(base, ctx.r3.u32, frame)) {
    lifted::original::c821635D0(ctx, base);
    return;
  }
  rs::StartRoute(base, ctx.r3.u32, frame);
}

ITC_REGS_STEP(RaceStatesRouteDistance) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 656;
  if (!rs::RouteDistanceNative(ctx.r4.u32, frame)) {
    lifted::original::c82162330(ctx, base);
    return;
  }
  rs::RouteDistance(base, ctx.r3.u32, ctx.r4.u32, frame);
}

ITC_REGS_STEP(RaceStatesRaceTick) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 208;
  if (!rs::RaceTickNative(base, ctx.r3.u32, frame)) {
    lifted::original::c82167610(ctx, base);
    return;
  }
  rs::RaceTick(base, ctx.r3.u32, ctx.r4.u32, frame);
}

ITC_REGS_STEP(RaceStatesOnGo) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 112;
  if (!rs::OnGoNative(base, ctx.r3.u32, frame)) {
    lifted::original::c82164458(ctx, base);
    return;
  }
  rs::OnGo(base, ctx.r3.u32, frame);
}

ITC_REGS_STEP(RaceStatesInit) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const auto r3 = NativeHeapActive()
                      ? rs::Init(base, machine, ctx.r1.u32 - 5824, &replaced)
                      : std::nullopt;
  if (!r3) {
    lifted::original::c82169920(ctx, base);
    return;
  }
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? *r3 : machine_r3 + (*r3 - machine);
}

ITC_REGS_STEP(RaceStatesOnRaceEnd) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const rs::GuestCall call = [&ctx, base](uint32_t function, uint32_t sp, uint32_t back,
                                          uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6,
                                          uint64_t r7) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    ctx.r6.u64 = r6;
    ctx.r7.u64 = r7;
    ctx.lr = back;
    lifted::CallFromContext(ctx, base, function);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  ctx.r3.u64 = rs::OnRaceEnd(base, ctx.r3.u64, ctx.r1.u32 - 272, call,
                             native::Buffers_NativeMemcpy());
  ctx.lr = lr;
}

ITC_REGS_STEP(RaceStatesTimeAdd) {
  ctx.r3.u64 = rs::TimeAdd(schema::PackedTime(base, ctx.r3.u32).packed(),
                           schema::PackedTime(base, ctx.r4.u32).packed());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_STATES(address, step)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_states)) { \
    hooks::step(ctx, base);               \
  }

// RaceProgress::Create (sub_821698B0) has no adapter: its only caller,
// RaceStates::Init, is native; --native_race_states switches it. Its register
// step stays for the harness.
ITC_RACE_STATES(821648E0, RaceStatesProgressReset)
ITC_RACE_STATES(821649B8, RaceStatesSetChecks)
ITC_RACE_STATES(82163440, RaceStatesReleaseRacers)
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
ITC_RACE_STATES(821674A0, RaceStatesCountdownTick)
ITC_RACE_STATES(821692C8, RaceStatesStandingStartTick)
ITC_RACE_STATES(82164DC0, RaceStatesBegin)
ITC_RACE_STATES(82167610, RaceStatesRaceTick)
ITC_RACE_STATES(82164458, RaceStatesOnGo)
ITC_RACE_STATES(821635D0, RaceStatesStartRoute)
ITC_RACE_STATES(82169920, RaceStatesInit)
ITC_RACE_STATES(821644D8, RaceStatesOnRaceEnd)
// Time::Add (sub_820FF458) keeps an adapter: the race HUD's splits
// (sub_82214BA8, sub_82214E78) and sub_820FF4D0, sub_820FF628 are recompiled.
ITC_RACE_STATES(820FF458, RaceStatesTimeAdd)
// The course record's CourseEntry::Clear (sub_82105BC8), Place
// (sub_82105EA0), Link (sub_82105D60), Unlink (sub_82105DC8), Insert
// (sub_82105E30), Add (sub_82105F50) and CourseRecords::Save (sub_82105C18)
// have no adapters: their only callers are OnRaceEnd and each other, native;
// --native_race_states switches OnRaceEnd.
// RaceProgress::RouteDistance (sub_82162330) has no adapter: its only
// caller, RaceProgress::StartRoute, is native; --native_race_states switches
// it. Its register step stays for the harness.
// sub_82163000 (never seen in play) has no adapter: its only caller,
// RaceProgress::FollowRoute, is native; --native_race_states switches it.
// RaceProgress::TickFraction (sub_82163140) and RoutePosition::NotAfter
// (sub_821622A8) have no adapters: their callers, RaceProgress::CheckLap and
// CheckFinish, are native; --native_race_states switches those callers.
// Their register steps stay for the harness.
// RaceStates::RacersNear (sub_82165AB0), RacersClose (sub_82165C78) and
// RacersStill (sub_82164A90) have no adapters: their callers,
// RaceStates::CountdownTick and StandingStartTick, are native;
// --native_race_states switches those callers. Their register steps stay
// for the harness.
// RouteSections::Before (sub_82162E70) and RoutePosition::MapToSection
// (sub_82163930) have no adapters: their callers, RaceProgress::FollowRoute,
// CheckLap and CheckFinish, are native. RaceProgress::CountTime
// (sub_82162E08), CheckLap (sub_82164090), CheckFinish (sub_82164E18),
// CountHits (sub_82164F08), Damage (sub_82164228), DrainSp (sub_821650A0)
// and Rank (sub_82162D80) have none either: their only caller,
// RaceStates::RaceTick, is native. --native_race_states switches those
// callers; the register steps stay for the harness.
// RouteSections::Beside (sub_82162F78) and RaceProgress::FollowRoute
// (sub_82163C50) have no adapters: their callers (StartRoute, FollowRoute,
// CheckLap and CheckFinish; OnGo and RaceTick) are native;
// --native_race_states switches those callers. The register steps stay for
// the harness.
