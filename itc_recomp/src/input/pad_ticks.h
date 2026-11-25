// The pad by tick (docs/pad-replay.md, "By tick"): every pad read the game
// makes (once a tick per user, PhysicalPad_Read) recorded to a file
// (--pad_ticks_record), or answered from one (--pad_ticks_play), so a replay
// gives the game the same input on the same ticks every run, unlike the
// virtual pad, which presses by the clock. For the state digest
// (--state_digest): two runs on the same input should match tick for tick.
//
// The number of ticks before a race varies from run to run (loads take as
// long as the disk does), so the recording marks each race's start, and the
// playback lines up there: reads left before the mark are skipped when the
// game enters the race sooner, and the last read is repeated while the game
// hasn't entered it yet. A race then gets its recorded input from its first
// tick.

#pragma once

#include "xinput.h"

#include <cstdint>

namespace input {

// Whether reads are answered from a recording.
bool PadTicks_Playing();

// While playing: the next recorded read for `user`; returns its result code
// (kXInputDeviceNotConnected for users never recorded, and after the end).
uint32_t PadTicks_Next(uint32_t user, XInputState* state);

// While recording: a read the game made and what it got.
void PadTicks_Record(uint32_t user, uint32_t result, const XInputState& state);

// Main thread, when the game enters a race (frame_pacing/game_messages.cpp):
// the mark recorded, or the playback lined up on it.
void PadTicks_OnRaceEntered();

// Main thread, when a race starts (the race world loaded: the course
// set-up's message 4, race_loading/race_loading.cpp): the mark recorded, or
// the playback lined up on it, as at a race entered.
void PadTicks_OnRaceStarted();

}  // namespace input
