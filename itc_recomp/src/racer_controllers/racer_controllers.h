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

// The calls into code that still takes the guest context, made by the
// adapters (src/hooks/racer_controllers.cpp): each runs with r1 = `frame`
// (the native function's frame, as the original's call site has it) and
// the link register `lr` (the original's return address).
struct Calls {
  void* user;
  // AiDriver::DriveCar (sub_8215F678; hooks::AiDriveCar) into the record at
  // `input` for racer `index` with the hold flag. f1 is left as it leaves
  // it.
  void (*drive_car)(void* user, uint32_t input, uint32_t index, uint32_t flag, uint32_t frame,
                    uint32_t lr);
  // The guest function `function` (through a vtable) with r3 (and r4 when
  // `set_r4`; the other registers as they are); its r3, and its f1 into `f1`
  // when not null.
  uint64_t (*call)(void* user, uint32_t function, uint64_t r3, uint64_t r4, bool set_r4,
                   uint32_t frame, uint32_t lr, double* f1);
  // AiDriver::UpdateIdle (sub_821399A8; hooks::AiUpdateIdle) on `driver`
  // (Of's r3), with f1 set to `f1` first when not null (what the original
  // leaves there before the call).
  void (*update_idle)(void* user, uint64_t driver, uint32_t frame, uint32_t lr,
                      const double* f1);
  // Whether the action mappers' read slots run natively (their flag,
  // --native_action_mappers); when not, they're called through `call`.
  bool native_slots;
};

// What a function leaves in r3 and f1 (f1 only when `f1_set`).
struct Regs {
  uint64_t r3;
  double f1;
  bool f1_set;
};

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

// RaceFlow::OnSetUp's sibling handlers and the AI drivers' hand-off. `sp`
// is the stack pointer the original is called with; `r3` the original's r3
// at its return.

// RacerControl::DriveByAi (sub_8215F968): unless the race data's flags have
// 0x100, the racer's AI driver fills a record in the original's frame (at
// +112; the frame 160 bytes below `sp`) with the hold flag (DriveCar),
// copied to the racer's input record ([0x826C5B30 + 4 x (index & 0xFF)]).
// `r3` comes in as the original's r3 (kept when frozen).
void DriveByAi(uint8_t* base, uint32_t index, uint32_t hold, uint32_t sp, const Calls& calls,
               uint64_t* r3);
// RaceFlow::HoldAll (sub_8215FA10; sub_8215FC28 jumps to it): each racer
// driven by its AI driver held (DriveByAi with 1). Returns 1.
uint32_t HoldAll(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8215F5F0 (never seen in play): a record cleared in the original's
// frame (CarInput::Clear at +80; its bytes +18 and +19 the frame's) and
// copied to every racer's input record. Returns 1.
uint32_t ClearAllRecords(uint8_t* base, uint32_t sp);
// RaceFlow::OnState16 (sub_8215FB90): with the current stage of type 2 or
// 3, or the mode 6 with the stage's word +0 not 1, ClearAllRecords; else
// HoldAll. Returns 1.
uint32_t OnState16(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The player's controls ---

// The action mappers' table (0x826BAB80, 8 bytes an entry: the mapper, a
// word +4: 1 when it isn't read), each player's entry index (0x826BABA4)
// and the input manager (0x826BAB78).
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kPlayerEntries = 0x826BABA4;
constexpr uint32_t kInputManager = 0x826BAB78;
// The player's mapper entry.
uint32_t MapperEntry(uint8_t* base, uint32_t player);

// PlayerInput::Read (sub_82102350): action `id` of the player's mapper
// into `out`: with the entry's word +4 1, 0 (0.0, [0x82000A34], for kind
// 4); else through the mapper's vtable: kind 0 slot 8 (held), 1 slot 10
// (pressed), 2 slot 12 (fired), each as a byte made a word; 3 slot 14 (the
// analog word); 4 slot 5 (a float). Kinds 5 and up: nothing. The slots
// seen in play (ActionMapper::Held, Pressed, Fired, AnalogRaw) are the
// action mappers' plain C++ (with --log_pause_menu's line); others are
// called. `regs.r3` comes in as the original's r3 (kept where it's left).
void Read(uint8_t* base, uint32_t player, uint32_t id, uint32_t kind, uint32_t out, uint32_t sp,
          const Calls& calls, Regs* regs);
// PlayerInput::Control (sub_82102488, its case 1 sub_821024D0 merged in):
// control `control` (0 to 30) read into `out` as one action or an axis of
// two (the first's value negated when it isn't 0, kinds 3 and 4; else the
// second's). Returns 1 or 0 by the control (see the row); 0 above 30 or for
// 0, with nothing read.
uint32_t Control(uint8_t* base, uint32_t player, uint32_t control, uint32_t kind, uint32_t out,
                 uint32_t sp, const Calls& calls, Regs* regs);
// Pad::IsConnected (sub_8209F340, sub_82591C08 merged in): the player's pad
// (the mapper's +2888) through the input manager's slot 9, then that
// object's slot 5 with &the player (in the original's frame, +80); 1 when
// its flags (+4) have bit 0x1.
uint32_t PadIsConnected(uint8_t* base, uint32_t entry, uint32_t sp, const Calls& calls);
// Pad::IsWheel (sub_8209F3B8): the pad as PadIsConnected finds it; 1 when
// its byte +193 has bit 0x2.
uint32_t PadIsWheel(uint8_t* base, uint32_t entry, uint32_t sp, const Calls& calls);

// --- The pad reader and the human cars ---

// InputRecording::Next (sub_82102A28, never seen in play): with the
// player's recording's position (its count word) under 54,000, the record
// there copied out and the position up, 1; else 0.
uint32_t RecordingNext(uint8_t* base, uint32_t record, uint32_t player);
// A message posted to the ring at the owner's +204 (50 records of 12
// bytes, the count at +804 and the start at +808): (message, 0, 0).
void PostMessage(uint8_t* base, uint32_t owner, uint32_t message);
// PlayerControl::ReadPad (sub_8215FCA8): the record filled from the
// player's pad through the player's control map (see the row); the control
// words in the original's frame (+80 and +84; 288 bytes below `sp`).
// Returns 1 after the pause messages or with no pad, else 0. `f1` the
// vehicle's speed when it was read (the original's f1 then, before
// UpdateIdle).
uint32_t ReadPad(uint8_t* base, uint32_t record, uint32_t index, uint32_t player, uint32_t owner,
                 uint32_t may_pause, uint32_t sp, const Calls& calls);
// PlayerControl::Update (sub_821604C8): the racer's record of the race
// data (`slot`) picks the source: a cleared record, or ReadPad with the
// recording or replay; the record in the original's frame (+80, 192 bytes
// below `sp`; its bytes +18 and +19 the frame's) copied to the racer's
// input record. Returns 0 when frozen, else ReadPad's answer (0 cleared).
uint32_t PlayerUpdate(uint8_t* base, uint32_t slot, uint32_t index, uint32_t player,
                      uint32_t owner, uint32_t may_pause, uint32_t sp, const Calls& calls);
// RacerControl::DriveByAiWithPad (sub_8215F7E8): as DriveByAi (its frame
// 192 bytes below `sp`, the record at +128), with the player's pad's
// controls +20 (flag 0x40) and +48 (the halfword +12) added (see the row).
// `regs.r3` the second control's answer (kept when frozen); f1 DriveCar's.
void DriveByAiWithPad(uint8_t* base, uint32_t index, uint32_t hold, uint32_t player,
                      uint32_t sp, const Calls& calls, Regs* regs);
// RaceFlow::AllByAi (sub_8215FA68): each racer by its AI driver, held:
// those whose vehicle's flags +6148 have 0x1 with the pad of the player at
// the flow's +9612, 0x80 with +9616's (DriveByAiWithPad), the others
// DriveByAi. Returns 1.
uint32_t AllByAi(uint8_t* base, uint32_t flow, uint32_t sp, const Calls& calls);

// --- The race flow's per-racer states ---

// RaceFlow::PlayersAndHeldAi (sub_821606D0; sub_82160940, state 7's
// handler, jumps to it): each racer: with its vehicle's flags +6148 0x1,
// PlayerUpdate (its record slot the racer's +24, the player at the flow's
// +9612, may pause), its answer kept; with 0x80, the same with +9616's
// player, may pause only when the first answered 0; the others DriveByAi
// held. Returns 1.
uint32_t PlayersAndHeldAi(uint8_t* base, uint32_t flow, uint32_t owner, uint32_t sp,
                          const Calls& calls);
// RaceFlow::PlayersAndAi (sub_821607A0): as PlayersAndHeldAi, the others
// held unless their word +0 has 0x10 and not 0x4000. Returns 1.
uint32_t PlayersAndAi(uint8_t* base, uint32_t flow, uint32_t owner, uint32_t sp,
                      const Calls& calls);
// sub_82160880 (never seen in play): as PlayersAndAi with no player able to
// pause, the others held only when their word +0 has 0x4000. Returns 1.
uint32_t PlayersAndAiNoPause(uint8_t* base, uint32_t flow, uint32_t owner, uint32_t sp,
                             const Calls& calls);
// RaceFlow::OnState22 (sub_82160948): with the current stage of type 2 or
// 3, or the mode 6 with the stage's word +0 not 1, PlayersAndAiNoPause;
// else AllByAi. Returns 1.
uint32_t OnState22(uint8_t* base, uint32_t flow, uint32_t owner, uint32_t sp,
                   const Calls& calls);
// RaceFlow::OnState23 (sub_821609E0): with the mode 6 and the stage's word
// +0 not 1, PlayersAndAiNoPause, else PlayersAndAi; then the race data's
// flag 0x20000 (the replay's end) cleared and, while replaying (0x40),
// message 34 posted. Returns 1.
uint32_t OnState23(uint8_t* base, uint32_t flow, uint32_t owner, uint32_t sp,
                   const Calls& calls);

// RaceFlow::OnState34 (sub_8215FC38; the attract races' state): each racer
// by its AI driver, held unless its vehicle's flags +6148 have 0x1
// (DriveByAi). Returns the last racer count read (the original's r3: it
// doesn't set one).
uint32_t OnState34(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The AI drivers' set-up ---

// AiDriver::Setup (sub_82170FF8): the driver of the racer at `index`
// (AiDriver::Of) reset (AiDriver::Reset), given the item at `id` (a word; 0
// for a negative one) as its profile (AiDriver::SetParams), then
// ResetControl with the word at `hold` set, StartRace without (their frames
// below the original's, 128 bytes below `sp`). Returns the r3 the original
// leaves (the driver's skill, +6048, or StartRace's).
uint64_t Setup(uint8_t* base, uint32_t index, uint32_t id, uint32_t hold, uint32_t sp);

}  // namespace racer_controllers
