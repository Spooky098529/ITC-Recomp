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

}  // namespace hooks
