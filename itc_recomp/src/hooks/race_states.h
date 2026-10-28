// The race's states' register steps (src/race_states/,
// docs/functions/race-states.md), shared by the adapters
// (src/hooks/race_states.cpp, behind --native_race_states) and the harness
// (without the flag). Each turns flush-to-zero off first and leaves r3 as
// the original does where a caller reads it.

#pragma once

#include "guest.h"

namespace hooks {

// RaceProgress::Create sub_821698B0: no arguments; r3 = the block (the
// original where operator new's plain C++ refuses).
void RaceStatesProgressCreate(PPCContext& ctx, uint8_t* base);
// RaceProgress::Reset sub_821648E0: r3 = the progress (r3 not kept: no
// caller reads it); the original where RecordReset's copies aren't native.
void RaceStatesProgressReset(PPCContext& ctx, uint8_t* base);
// RaceProgress::SetChecks sub_821649B8: r3 = the progress, r4 = the kind,
// r5 = the stage's type, r6 = its word +4 (r3 left).
void RaceStatesSetChecks(PPCContext& ctx, uint8_t* base);
// RaceProgress::ReleaseRacers sub_82163440: r3 = the progress (r3 not kept:
// no caller reads it; RaceManager::Refresh's callees' originals as its
// adapter's).
void RaceStatesReleaseRacers(PPCContext& ctx, uint8_t* base);
// RaceProgress::CountTime sub_82162E08: r3 = out, r4 = the progress, r5 =
// the entry's index; r3 = out.
void RaceStatesCountTime(PPCContext& ctx, uint8_t* base);
// RaceProgress::TickFraction sub_82163140: r3 = the progress, r4 = the
// slot's index, r5 = &a packed time (r3 not kept: no caller reads it).
void RaceStatesTickFraction(PPCContext& ctx, uint8_t* base);
// RaceProgress::Rank sub_82162D80: r3 = the progress (r3 not kept: no caller
// reads it).
void RaceStatesRank(PPCContext& ctx, uint8_t* base);
// RouteSections::Beside sub_82162F78: r3, r4 = two sections; r3 = 0 or 1.
void RaceStatesSectionsBeside(PPCContext& ctx, uint8_t* base);
// RouteSections::Before sub_82162E70: r3, r4 = two sections; r3 = r3 left,
// a section or -1.
void RaceStatesSectionsBefore(PPCContext& ctx, uint8_t* base);
// RoutePosition::NotAfter sub_821622A8: r3, r4 = &two positions; r3 = 0 or
// 1.
void RaceStatesPositionNotAfter(PPCContext& ctx, uint8_t* base);

// The race's checks: r3 = out, r4 = the progress, r5 = the entry's index
// (DrainSp: r3 = out, r4 = the progress); r3 = out.
// RaceProgress::CountHits sub_82164F08, Damage sub_82164228, DrainSp
// sub_821650A0, CheckLap sub_82164090 (the original where the course
// route's copies aren't native) and CheckFinish sub_82164E18.
void RaceStatesCountHits(PPCContext& ctx, uint8_t* base);
void RaceStatesDamage(PPCContext& ctx, uint8_t* base);
void RaceStatesDrainSp(PPCContext& ctx, uint8_t* base);
void RaceStatesCheckLap(PPCContext& ctx, uint8_t* base);
void RaceStatesCheckFinish(PPCContext& ctx, uint8_t* base);
// RoutePosition::MapToSection sub_82163930: r3 = a section, r4 = &a
// position; r3 = 0 or 1.
void RaceStatesMapToSection(PPCContext& ctx, uint8_t* base);

// The state machine's handlers: r3 = the machine, r4 = the dispatcher (r3
// not kept: the handler call doesn't read it; OnStandingStart's r3 as
// AiDrivers::StartRanked leaves it). RaceStates::FadeInFree sub_82163398,
// FadeOut sub_82162310, EndWhenFaded sub_82168F50, Unload sub_82163920 (the
// original unless --native_heap routes the game's heap to the allocation
// layer), OnRollingStart sub_821632C8, OnStandingStart sub_821643B0,
// ClearStep sub_82163348, OnQuit sub_82163378, OnReplay sub_821647B0,
// FreeStartTick sub_82168B80, QuitTick sub_82168500, AfterRaceTick
// sub_82167970 and RollingStartTick sub_82166E50 (RaceManager::Refresh's and
// RaceStart::PlaceRolling's callees' originals as their adapters make them).
void RaceStatesFadeInFree(PPCContext& ctx, uint8_t* base);
void RaceStatesFadeOut(PPCContext& ctx, uint8_t* base);
void RaceStatesEndWhenFaded(PPCContext& ctx, uint8_t* base);
void RaceStatesUnload(PPCContext& ctx, uint8_t* base);
void RaceStatesOnRollingStart(PPCContext& ctx, uint8_t* base);
void RaceStatesOnStandingStart(PPCContext& ctx, uint8_t* base);
void RaceStatesClearStep(PPCContext& ctx, uint8_t* base);
void RaceStatesOnQuit(PPCContext& ctx, uint8_t* base);
void RaceStatesOnReplay(PPCContext& ctx, uint8_t* base);
void RaceStatesFreeStartTick(PPCContext& ctx, uint8_t* base);
void RaceStatesQuitTick(PPCContext& ctx, uint8_t* base);
void RaceStatesAfterRaceTick(PPCContext& ctx, uint8_t* base);
void RaceStatesRollingStartTick(PPCContext& ctx, uint8_t* base);

// RaceStates::RacersNear sub_82165AB0 and RacersClose sub_82165C78: no
// arguments; r3 = 0 or 1. RaceStates::RacersStill sub_82164A90: r3 = &the
// count; r3 = 0 or 1. RaceStates::CountdownTick sub_821674A0: a handler (r3
// = the machine, r4 = the dispatcher; r3 not kept).
void RaceStatesRacersNear(PPCContext& ctx, uint8_t* base);
void RaceStatesRacersClose(PPCContext& ctx, uint8_t* base);
void RaceStatesRacersStill(PPCContext& ctx, uint8_t* base);
void RaceStatesCountdownTick(PPCContext& ctx, uint8_t* base);

// RaceStates::Begin sub_82164DC0: a handler (r3 = the machine; r3 not
// kept); the original where RaceProgress::Reset's copies aren't native.
void RaceStatesBegin(PPCContext& ctx, uint8_t* base);
// RaceProgress::FollowRoute sub_82163C50: r3 = out, r4 = the progress, r5 =
// the entry's index; r3 = out. sub_82163000 (no adapter: its only caller,
// FollowRoute, is native): r3, r4 = two sections, r5, r6 = the out words;
// r3 = 0 or 1 (the harness's).
void RaceStatesFollowRoute(PPCContext& ctx, uint8_t* base);
void RaceStatesSectionsJoin(PPCContext& ctx, uint8_t* base);
// RaceProgress::StartRoute sub_821635D0: r3 = the race's states (r3 not
// kept: neither caller reads it); the original where StartRouteNative says
// the course route's copies aren't native. RaceProgress::RouteDistance
// sub_82162330 (no adapter: its only caller, StartRoute, is native): r3 =
// the progress, r4 = the course route's index (r3 not kept; the original
// where RouteDistanceNative says no; the harness's).
void RaceStatesStartRoute(PPCContext& ctx, uint8_t* base);
void RaceStatesRouteDistance(PPCContext& ctx, uint8_t* base);
// RaceStates::RaceTick sub_82167610: a handler (r3 not kept); the original
// where RaceTickNative says so.
void RaceStatesRaceTick(PPCContext& ctx, uint8_t* base);
// RaceStates::OnGo sub_82164458: a handler (r3 = the machine; r3 not kept);
// the original where OnGoNative says so.
void RaceStatesOnGo(PPCContext& ctx, uint8_t* base);

// RaceStates::StandingStartTick sub_821692C8: a handler (r3 not kept); the
// original in the System Link case (StandingStartTickNative); the callees'
// originals as their adapters make them.
void RaceStatesStandingStartTick(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
