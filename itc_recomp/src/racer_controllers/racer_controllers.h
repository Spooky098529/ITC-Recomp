// The racer controllers as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/racer-controllers.md, the layouts in
// docs/known-data.md: "car input record", 0x826C5B30, "race flow", "control
// map", 0x826C5B4C, the race data's +0x59AE4). What fills each car's 20-byte
// input record every tick: the cars' records made and freed, the race
// flow's per-state handlers, the pad reader, the input recording and the AI
// drivers' hand-off. The records, the race data, the racers and the
// vehicles are guest data shared with the recompiled code, so these work on
// guest memory, with the originals' loads and stores in their order. The
// originals' stack temporaries aren't written, except the words a callee
// reads or writes through a pointer (the read controls' outputs, the pad's
// player word): those are at the originals' frame offsets, so what a call
// that writes nothing leaves there is the same word. Adapters:
// src/hooks/racer_controllers.cpp.

#pragma once

#include <cstdint>

namespace racer_controllers {

// The race data ([0x826C5B2C]), the cars' input records (seven pointers),
// the input recordings ([0x826C5B4C]) and the race ([0x826D82F4]: +2308 the
// racers' records, +2364 their count).
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kInputs = 0x826C5B30;
constexpr uint32_t kRecordings = 0x826C5B4C;
constexpr uint32_t kRace = 0x826D82F4;
// The race data's stage list, flags, mode and finish count.
constexpr uint32_t kStages = 0x59620;
constexpr uint32_t kFlags = 0x59AE4;
constexpr uint32_t kMode = 0xA3AF0;
constexpr uint32_t kCount = 0xA3AD8;
// A player's recording: 54,000 records of 20 bytes, the count (and read
// position) at +0x107AC0, 0x107AC4 bytes a player.
constexpr uint32_t kRecordingBytes = 0x107AC4;
constexpr uint32_t kRecordingCount = 0x107AC0;
constexpr uint32_t kRecordingMax = 54000;

// --- The cars' input records ---

// CarInputs::Create (sub_82102258): seven 20-byte records from operator
// new, each into 0x826C5B30 + 4i and cleared (CarInput::Clear). `last` the
// last record (the original's r3). False, with nothing done, when operator
// new can't be native (the native heap off, or a failed allocation).
bool CreateInputs(uint8_t* base, uint32_t* last);
// CarInputs::Destroy (sub_82102300): the seven records deleted (operator
// delete); the pointers left as they were. `last` the last pointer (the
// original's r3). False, with nothing done, when one isn't a block the
// layer gave (operator delete's error path, the original's then).
bool DestroyInputs(uint8_t* base, uint32_t* last);

// --- The input recording ---

// InputRecording::Append (sub_821029A8): with the player's recording under
// 54,000 records, the record copied to its end and counted, 1; else 0.
uint32_t RecordingAppend(uint8_t* base, uint32_t record, uint32_t player);
// InputRecording::HasRoom (sub_82102968): 1 when the player's recording
// has under 54,000 records, else 0.
uint32_t RecordingHasRoom(uint8_t* base, uint32_t player);

// --- The race flow's handlers ---

// RaceFlow::OnSetUp (sub_8215F728): CreateInputs; then, unless the current
// stage (of the race data's +0x59620) is of type 2 or 3, and unless the
// race data's mode (+0xA3AF0) is 6 with the stage's word +0 not 1, the
// flow's +9608 bit 0x1 set. `r3` the stage check's answer (the original's
// r3). False, with nothing done, where CreateInputs is.
bool OnSetUp(uint8_t* base, uint32_t flow, uint32_t* r3);

}  // namespace racer_controllers
