// The race's states and the race's progress as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/race-states.md): the race mode's state machine at +107168
// (RaceStates::, its handlers) and the race's progress at [0x826D82E8]
// (RaceProgress::, 7,248 bytes: four racer slots of 536 bytes from +40, the
// results record at +2184 with its 972-byte entries from +2188, the stage's
// route and its checks), and the route helpers its checks use. The progress,
// the manager, the vehicles and the race data are guest objects shared with
// the recompiled code, so these work on guest memory, with the originals'
// loads and stores in their order and their arithmetic step for step
// (src/car_physics/float_ops.h). The originals' temporaries are on their
// stack: `frame` is the original's stack frame (its r1 after the stwu) and
// `sp` the stack pointer it came with; the temporaries go at the same
// offsets, register saves and back chains aren't written. Flush-to-zero off
// on entry (the adapters' job). Callees made native earlier are called as
// plain C++. Adapters: src/hooks/race_states.cpp, behind
// --native_race_states.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "race_world/race_world.h"

namespace race_states {

constexpr uint32_t kProgress = 0x826D82E8;  // the race's progress (a pointer)

// --- The race's progress (src/race_states/race_progress.cpp) ---

// RaceProgress::Create (sub_821698B0): 7,248 bytes from operator new; with
// a block, its four 972-byte entries from +2188 get +792 = the word at
// 0x82000A34 (0.0) and +796 = 0; [0x826D82E8] = the block (or 0). Returns
// the original's r3 (the block); nullopt, with nothing done, where operator
// new's plain C++ refuses.
std::optional<uint32_t> ProgressCreate(uint8_t* base);
// RaceProgress::Reset (sub_821648E0): RecordReset (sub_82101F68) on +2184
// (its frame `frame` - 128); the four slots (+40 + 536 i): +0, +4, +8, +524
// = 0, +528 = 2, halfwords +532, +534 = -1; the 256 halfwords from +6120
// and from +6636 = -1; +6632, +7148 = 0, +7156 = the word at 0x82000A34; the
// 15 words from +7160 = 0; +7228, +7220, +7224 = 0, +7240 = -1; +7232,
// +7236 = the word at 0x82000A30 (1.0). `frame`: the original's 96 bytes.
// ProgressResetNative says first whether it's done natively (RecordReset's
// copies); false, with nothing written, where it isn't.
bool ProgressResetNative(uint32_t frame);
void ProgressReset(uint8_t* base, uint32_t progress, uint32_t frame);
// RaceProgress::SetChecks (sub_821649B8): +6100 = `type`, +7220, +7224 = 0;
// types 1 and 3: +7220 = 0x11, +7224 = 0x417; types 0 and 2: +7220 = 0x3
// (| 0x8 for the kind 1 with `word` set), +7224 = 0x481; others nothing.
void SetChecks(uint8_t* base, uint32_t progress, uint32_t kind, uint32_t type, uint32_t word);
// RaceProgress::ReleaseRacers (sub_82163440): each slot's manager entry
// (+48 + 536 i, when there) gets flag 0x20 when it has 0x1000, then loses
// 0x2000, 0x1000, 0x10, 0x4000 and 0x400; then RaceManager::Refresh of
// [0x826D82F4] (a tail call: its frame `sp` - 1904; its callees' originals
// through `original` as race_world::Refresh says).
void ReleaseRacers(uint8_t* base, uint32_t progress, uint32_t sp, bool heap,
                   const race_world::OriginalCall& original);
// RaceProgress::CountTime (sub_82162E08): the entry `index`'s (+2188 + 972
// i) ticks +12 up by one, +16 = RaceTime::FromTicks of them; out (+0, +4) =
// 0. Returns `out` (the original's r3).
uint32_t CountTime(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index);
// RaceProgress::TickFraction (sub_82163140): with the slot `index`'s
// vehicle's distance a frame not 0.0, the packed time at `time` less the
// part of the tick past the line, (1.0 - its +6228 / that distance) x
// [0x826193A4] x [0x82001BDC] ms (fctiwz, at `frame` + 80), packed again
// (Time::Pack). `frame`: the original's 112 bytes.
void TickFraction(uint8_t* base, uint32_t progress, uint32_t index, uint32_t time,
                  uint32_t frame);
// RaceProgress::Rank (sub_82162D80): the results entries (+2184 of them,
// the count read again each pass) inserted by their +824 into the sorted
// list at `frame` + 80 (SortedList::Clear, Insert); then from the list's
// first (`frame` + 1680) each entry's +8 = the count - 1, - 2, ... `frame`:
// the original's 1,728 bytes.
void Rank(uint8_t* base, uint32_t progress, uint32_t frame);

// --- The race's checks each tick (src/race_states/race_checks.cpp): r3 an
// out pair (an end and a reason), the progress, a results entry `index`
// (E = +2188 + 972 i, its slot S = +40 + 536 i); each returns `out` (the
// original's r3) ---

// Time::Sub (sub_82162230, never seen in play): two packed times (the words
// at `a` and `b`) unpacked to signed milliseconds, a's less b's packed again
// (Time::Pack). Returns its r3.
uint64_t TimeSub(uint8_t* base, uint32_t a, uint32_t b);
// RaceProgress::CountHits (sub_82164F08): see the row. `frame`: the
// original's 128 bytes (nothing kept in it).
uint32_t CountHits(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index);
// RaceProgress::Damage (sub_82164228): see the row; the temporaries at
// `frame` + 80 and + 88 as the original leaves them. `frame`: its 160
// bytes.
uint32_t Damage(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index, uint32_t frame);
// RaceProgress::DrainSp (sub_821650A0): see the row; the temporaries at
// `frame` + 72 and + 80 as the original leaves them (its debug prints, the
// empty sub_82106540, not made). `frame`: its 224 bytes.
uint32_t DrainSp(uint8_t* base, uint32_t out, uint32_t progress, uint32_t frame);
// RaceProgress::CheckLap (sub_82164090): see the row; the course route
// copied as the original (CourseRoutes::Get at `frame` + 288, memcpy to +
// 96), the positions at + 80 and + 84. `frame`: its 544 bytes.
// CheckLapNative says first whether the copies are made natively.
bool CheckLapNative(uint8_t* base, uint32_t progress, uint32_t frame);
uint32_t CheckLap(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index, uint32_t frame);
// RaceProgress::CheckFinish (sub_82164E18): see the row; the position at
// `frame` + 80. `frame`: its 144 bytes.
uint32_t CheckFinish(uint8_t* base, uint32_t out, uint32_t progress, uint32_t index,
                     uint32_t frame);

// --- The state machine's handlers (src/race_states/handlers.cpp): the
// machine (the race mode + 107168: +9604 the state, +9608 the pause menu's
// quit, +9612 a step, +9616 a tick count, +9624 the progress) and the
// dispatcher (the race mode + 36) whose ring they post to. r3 isn't kept:
// the state machine's handler call doesn't read it ---

// RaceStates::FadeInFree (sub_82163398): +9616, +9620, +9612 = 0;
// ScreenFades::FadeIn([0x82001BEC], black, layer 0).
void FadeInFree(uint8_t* base, uint32_t machine);
// RaceStates::FadeOut (sub_82162310): ScreenFades::FadeOut([0x82001BF0],
// black, layer 0).
void FadeOut(uint8_t* base);
// RaceStates::EndWhenFaded (sub_82168F50): when the fades are done, posts
// 49.
void EndWhenFaded(uint8_t* base, uint32_t dispatcher);
// RaceStates::Unload (sub_82163920): operator delete of [0x826D82E8]. False,
// with nothing done, where operator delete's plain C++ refuses.
bool Unload(uint8_t* base);
// RaceStates::OnRollingStart (sub_821632C8): FadeOut as above when the
// alpha 0x826A50D4 is 0; +9612 = 0.
void OnRollingStart(uint8_t* base, uint32_t machine);
// RaceStates::OnStandingStart (sub_821643B0): +9620, +9628, +9632, +9612,
// +9616, +9636, +9640 = 0, [0x826C6B04] = 0; AiDrivers::StartRanked (a
// tail call: its frame `sp` - 224). Returns its r3.
uint64_t OnStandingStart(uint8_t* base, uint32_t machine, uint32_t sp);
// RaceStates::ClearStep (sub_82163348): +9612 = 0. RaceStates::OnQuit
// (sub_82163378): +9608 = 0.
void ClearStep(uint8_t* base, uint32_t machine);
void OnQuit(uint8_t* base, uint32_t machine);
// RaceStates::OnReplay (sub_821647B0): FadeOut as above; +9612, +9628,
// +9632 = 0; the race data's flags |= 0x2000.
void OnReplay(uint8_t* base, uint32_t machine);
// RaceStates::FreeStartTick (sub_82168B80): see the row.
void FreeStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher);
// RaceStates::QuitTick (sub_82168500) and AfterRaceTick (sub_82167970):
// see the rows.
void QuitTick(uint8_t* base, uint32_t machine, uint32_t dispatcher);
void AfterRaceTick(uint8_t* base, uint32_t machine, uint32_t dispatcher);
// RaceStates::RollingStartTick (sub_82166E50): when the fades are done:
// without the race data's flag 0x100 posts 31 and sets it; the flags |=
// 0x2000, 0x4000; RaceManager::Refresh (its frame `frame` - 1904, its
// callees' originals through `refresh`), RaceStart::PlaceRolling(machine,
// 1) (its frame `frame` - 272, its calls `calls`); posts 15; +9612 = 0.
// `frame`: the original's 112 bytes.
void RollingStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      bool heap, const race_world::OriginalCall& refresh,
                      const race_world::StartCalls& calls);

// --- The racers at the start (src/race_states/racers.cpp) ---

// RaceStates::RacersNear (sub_82165AB0): 1 when every racer of the
// current stage but the player's (record flag 0x1) is on the player's
// section (its vehicle's track position's) or on a section one of whose
// links 0 to 9 is it, else 0. The link list (0 to 9, -1) at `frame` + 80 as
// the original leaves it. `frame`: its 208 bytes.
uint32_t RacersNear(uint8_t* base, uint32_t frame);
// RaceStates::RacersClose (sub_82165C78): 0 when a racer of the current
// stage (not flag 0x1) has its entry's slice center farther than
// [0x8200257C] (fsqrts of vmsum3fp128) from the player's entry's, else 1.
// The vectors at `frame` + 80 to + 127 as the original leaves them.
// `frame`: its 208 bytes.
uint32_t RacersClose(uint8_t* base, uint32_t frame);
// RaceStates::RacersStill (sub_82164A90): the count at `count` = 0 when a
// racer's vehicle has a contact (its collision object's +240) or collision
// flags (+6152 & 7), else up by one to 30 at most (its debug print, the
// empty sub_82106540, not made); 1 at 30, else 0.
uint32_t RacersStill(uint8_t* base, uint32_t count);
// RaceStates::CountdownTick (sub_821674A0): unless RacersNear: posts 19,
// +9616, +9620 = 0, AiDrivers::ClearAll; else +9616 up to 120, then posts
// 28, +9616, +9620 = 0, the race data's flags lose 0x1000 and 0x2000,
// AiDrivers::ResetAll. `frame`: its 112 bytes (the callees' below it).
void CountdownTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame);

// RaceStates::StandingStartTick (sub_821692C8)'s guest calls: the callees'
// originals as their adapters make them (RaceManager::Refresh's,
// RaceStart::Place's with Stages::Copy's through start.original,
// RaceManager::ListRacers'), and AiDrivers::AllSettled's original where its
// plain C++ refuses (r1 = `sp`; its r3).
struct StandingCalls {
  race_world::OriginalCall refresh;
  race_world::StartCalls start;
  race_world::OriginalR3 list;
  std::function<uint32_t(uint32_t sp)> all_settled;
};
// Whether StandingStartTick runs natively: not in the System Link case
// (the race data's kind +0xA3AF0 6 with the current stage's +0 not 1, the
// racers settled (+9636) and +9640 at most 900, +9616 below 900), which
// the original runs (no preserved behavior: the game has no System Link
// mode).
bool StandingStartTickNative(uint8_t* base, uint32_t machine);
// RaceStates::StandingStartTick (sub_821692C8): see the row. `frame`: the
// original's 2,816 bytes (the stage's copy at +80; the callees' frames
// below it).
void StandingStartTick(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t frame,
                       bool heap, const StandingCalls& calls);

// --- The route's sections and positions ---

// RouteSections::Beside (sub_82162F78): 1 when `a` and `b` are the same
// (cmpw) or `b` is one of a's links 6 to 9 (halfwords +52 to +64 of its
// record), else 0. With a and b not the same, the link list (6, 7, 8, 9,
// -1) is left at `sp` - 32 (a leaf without a frame).
uint32_t SectionsBeside(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp);
// RouteSections::Before (sub_82162E70): `a` (`same` set) when `b` is one of
// a's forward links (0 to 2); else the first of a's links 6 to 9 (not -1)
// one of whose forward links is `b`, sign-extended; else -1. The link
// lists (0, 1, 2, -1 and 6, 7, 8, 9, -1) are left at `sp` - 48 and - 32.
int64_t SectionsBefore(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp, bool* same);
// RoutePosition::NotAfter (sub_821622A8): 1 when the position at `a`
// (signed halfwords section, slice) is before or at `b`'s, else 0.
uint32_t PositionNotAfter(uint8_t* base, uint32_t a, uint32_t b);
// RoutePosition::MapToSection (sub_82163930): the position at `position`
// put on `section` where it can be (see the row): 1, or 0 when the two
// share a back link (3 to 5) and the position's slice is at or past the
// section's slice count. The original's temporaries below `sp` (-96 to -33,
// the dot products' at -32 rounded down to 16) as it leaves them (a leaf
// without a frame; its register saves at -16 and -8 not written).
uint32_t MapToSection(uint8_t* base, uint32_t section, uint32_t position, uint32_t sp);

}  // namespace race_states
