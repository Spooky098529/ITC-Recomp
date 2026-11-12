// The race's end natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/race-end.md and, for RaceMode::NextMode, race-loading.md):
// the race mode's next-mode query (its slot 6, run every tick) and the mode
// it answers once the race is over. Plain C++ over guest memory (the race
// data, the save data, the race mode and the globals are shared with the
// recompiled code), with the originals' loads and stores in their order and
// their frames' words. Adapters: src/hooks/race_end.cpp (behind
// --native_race_end), register steps shared with the harness in
// src/hooks/race_end.h.

#pragma once

#include <cstdint>

#include "race_event/race_event.h"
#include "race_hud_object/race_hud_object.h"
#include "race_loading/race_loading.h"

namespace race_end {

// The guest calls: `guest` for GameMessages::Dispatch (its override) and the
// fallbacks (Progress::AfterRace and CheckUnlocks where Condition::Test's
// copies can't be native), and Session::Free's; `loading` for the loading
// screen's Run and sub_82293460; `event` for Career::EventLocation's
// condition tests, its depth from the adapter's stack pointer.
struct Calls {
  race_hud_object::Calls guest;
  race_loading::Calls loading;
  race_event::Guest event;
};

// The originals' frames (their stwu sizes).
constexpr uint32_t kNextModeFrame = 112;
constexpr uint32_t kModeAfterRaceFrame = 144;

// The race data ([0x826C5B2C]) words: the flags (0x8 an event race, 0x10 a
// rival race, 0x8000 and 0x10000 the race mode's +32 slot 0's hold), the
// kind of race and the race mode.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kFlags = 0x59AE4;
constexpr uint32_t kKind = 0xA3AF0;
constexpr uint32_t kRaceMode = 0xA3AF4;
// The save data's words: the unlocks to show (the table-0 list's count) and
// the stage whose movies were shown.
constexpr uint32_t kUnlocksShown = 0x1CA78;
constexpr uint32_t kStageShown = 0x1D524;
// The globals: the title's flag, the session, the fades' passes, the boot
// movies' table (five ids, +20 the mode after, +24 skippable, +28 the
// index).
constexpr uint32_t kTitleFlag = 0x826364EC;
constexpr uint32_t kSession = 0x82816E3C;
constexpr uint32_t kFadeInPasses = 0x82001BEC;
constexpr uint32_t kBootMovies = 0x826C5BAC;

// The race mode's words: the dispatcher (+36), its ring (+240; the count at
// +840, the start at +844), the +848 machine's state, whether to post the
// Update message.
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kRing = 240;
constexpr uint32_t kMachineState = 10452;
constexpr uint32_t kPostUpdate = 167260;

// The guest functions.
constexpr uint32_t kDispatch = 0x821062E8;      // GameMessages::Dispatch
constexpr uint32_t kAfterRace = 0x82116588;     // Progress::AfterRace
constexpr uint32_t kCheckUnlocks = 0x821162B0;  // Progress::CheckUnlocks

// sub_82150E00 RaceMode::ModeAfterRace: the 8-byte answer at `answer` (byte
// +0 1 when a mode is asked for, word +4 its type), built in the original's
// frame below `sp` (its +80 to +87; +81 to +83 never written, the stack's)
// and copied; see race-end.md's row. Returns r3 as the original leaves it
// (the answer: the adapter keeps its r3).
void ModeAfterRace(uint8_t* base, uint32_t answer, uint32_t sp, const Calls& calls);

// sub_82151EF0 RaceMode::NextMode: the answer cleared; messages 0 (when
// +167260 isn't 0) and 1 posted into the mode's ring; LoadingScreen::Run(-1)
// with the +848 machine in state 3 or 4; sub_82293460; GameMessages::Dispatch;
// in state 35, ModeAfterRace's answer (in its frame's +80) copied. Returns r3
// as the original leaves it (the answer).
void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);

}  // namespace race_end
