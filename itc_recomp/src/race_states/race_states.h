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

}  // namespace race_states
