// The cutscene's scripts (docs/functions/camera-modes.md): Cutscene::
// Cutscene's big switch (the jump table 0x82009340, 0x821572A4 to
// 0x82158B48), each script's fade cues and shots. Plain C++ over guest
// memory, the original's stores in its order, in its stack frame.
//
// Each script is a straight run of stores to the frame and calls (the
// original's shared tails followed, its cue list's reset loop one step).
// The tables below were read from the generated code
// (itc_recomp/generated/default/itc_recomp.87.cpp, sub_82156CC0) by a
// script that follows each case's registers: every store's value is a
// constant (the registers the original set before the switch: 0, 1, 255,
// -1 and the shot kinds), the first record's vehicle (the original's r9)
// or another record's vehicle (the frame's +740, +748, +756, read where
// it's stored: nothing in the scripts writes those records). The frame's
// words a case doesn't write stay as the stack leaves them and are copied
// with the rest by List16::Insert32 and CutsceneShot::Init, as in the
// original.
//
// The frame (the original's r1): +80 the cue being built (32 bytes, a
// FadeCue: +0 and +4 its ticks, then its kind and color words), +112 the
// script entry (+0 the shot's kind, +4 its vehicle, +8 its 16-slot cue
// list, its +60 to +72 at the frame's +180 to +192), +784 the shot (608
// bytes) CutsceneShot::Init makes from the entry.

#include "camera_modes/camera_modes.h"

#include "guest.h"

#include <span>

namespace camera_modes {

namespace {

enum class Kind : uint8_t {
  kWord,      // the frame's word at `at` = value
  kHalf,      // the frame's halfword at `at` = value
  kVehicle,   // the frame's word at `at` = the first record's vehicle
  kRacer,     // the frame's word at `at` = record `value`'s vehicle (+728 + 8 n + 4)
  kReset,     // the 16-slot list at `at`: its slots' links (List16::Reset's loop)
  kCueFirst,  // List16::Insert32 of the cue (+80) into the entry's list (+120) at 0
  kCue,       // the same at the list's tail (its +72, the frame's +192)
  kShot,      // CutsceneShot::Init of the shot (+784) from the entry (+112)
  kAddShot,   // List16::Insert608 of the shot into the cutscene's +952 at its tail
};

// `value` is the return address of the original's call for the calls.
struct Step {
  Kind kind;
  uint16_t at;
  uint32_t value;
};

constexpr Step W(uint16_t at, uint32_t value) { return {Kind::kWord, at, value}; }
constexpr Step H(uint16_t at, uint32_t value) { return {Kind::kHalf, at, value}; }
constexpr Step Vehicle(uint16_t at) { return {Kind::kVehicle, at, 0}; }
constexpr Step Racer(uint16_t at, uint32_t record) { return {Kind::kRacer, at, record}; }
constexpr Step Reset(uint16_t at) { return {Kind::kReset, at, 0}; }
constexpr Step CueFirst(uint32_t back) { return {Kind::kCueFirst, 0, back}; }
constexpr Step Cue(uint32_t back) { return {Kind::kCue, 0, back}; }
constexpr Step Shot(uint32_t back) { return {Kind::kShot, 0, back}; }
constexpr Step AddShot(uint32_t back) { return {Kind::kAddShot, 0, back}; }

// Script 0 (0x821572A4): 2 cues, 1 shot.
constexpr Step kScript0[] = {
    W(112, 9), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 29), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82157318),
    W(80, 120), W(84, 149), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Script 1 (0x82157328): 5 cues, 1 shot.
constexpr Step kScript1[] = {
    W(112, 8), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x8215739C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 90), W(108, 0), W(84, 104),
        Cue(0x821573D4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 105), W(108, 0), W(84, 119),
        Cue(0x8215740C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 120), W(108, 0), W(84, 134),
        Cue(0x82157444),
    W(80, 210), W(84, 249), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Script 2 (0x82157454): 5 cues, 1 shot.
constexpr Step kScript2[] = {
    W(112, 10), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x821574C8),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 110), W(108, 0), W(84, 124),
        Cue(0x82157500),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 125), W(108, 0), W(84, 139),
        Cue(0x82157538),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 140), W(108, 0), W(84, 155),
        Cue(0x82157570),
    W(80, 240), W(84, 269), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Scripts 3, 6, 7 (0x82157580): 8 cues, 2 shots.
constexpr Step kScript3_6_7[] = {
    W(112, 11), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 29), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x821575F4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 50), W(108, 0), W(84, 64),
        Cue(0x8215762C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 65), W(108, 0), W(84, 79),
        Cue(0x82157664),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 80), W(108, 0), W(84, 125),
        Cue(0x8215769C),
    Shot(0x821576A8), AddShot(0x821576B8),
    W(112, 14), Racer(116, 1),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 30), W(84, 44), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0),
        W(108, 0), CueFirst(0x82157734),
    W(80, 45), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(84, 59), W(104, 0), W(108, 0),
        Cue(0x82157768),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 60), W(108, 0), W(84, 75),
        Cue(0x821577A0),
    W(80, 210), W(84, 239), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Scripts 4, 19 (0x821577B0): 11 cues, 3 shots.
constexpr Step kScript4_19[] = {
    W(112, 11), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 29), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82157824),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 50), W(108, 0), W(84, 64),
        Cue(0x8215785C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 65), W(108, 0), W(84, 79),
        Cue(0x82157894),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 80), W(108, 0), W(84, 125),
        Cue(0x821578CC),
    Shot(0x821578D8), AddShot(0x821578E8),
    W(112, 12), Racer(116, 1),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 30), W(84, 44), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0),
        W(108, 0), CueFirst(0x82157960),
    W(80, 45), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(84, 59), W(104, 0), W(108, 0),
        Cue(0x82157994),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 60), W(84, 75), W(108, 0),
        Cue(0x821579CC),
    Shot(0x821579D8), AddShot(0x821579E8),
    W(112, 15), Racer(116, 2),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 30), W(84, 44), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0),
        W(108, 0), CueFirst(0x82157A64),
    W(80, 45), W(84, 59), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82157A94),
    W(80, 60), W(84, 75), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82157AC4),
    W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(80, 240), W(108, 0), W(84, 269),
        Cue(0x82157AFC),
    Shot(0x82157B08), AddShot(0x82157B18),
};

// Script 5 (0x82157B24): 14 cues, 4 shots.
constexpr Step kScript5[] = {
    W(112, 11), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 29), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82157B98),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 50), W(108, 0), W(84, 64),
        Cue(0x82157BD0),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 65), W(108, 0), W(84, 79),
        Cue(0x82157C08),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 80), W(108, 0), W(84, 125),
        Cue(0x82157C40),
    Shot(0x82157C4C), AddShot(0x82157C5C),
    W(112, 12), Racer(116, 1),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 30), W(84, 44), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0),
        W(108, 0), CueFirst(0x82157CD4),
    W(80, 45), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(84, 59), W(104, 0), W(108, 0),
        Cue(0x82157D08),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 60), W(84, 75), W(108, 0),
        Cue(0x82157D40),
    Shot(0x82157D4C), AddShot(0x82157D5C),
    W(112, 13), Racer(116, 2),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 30), W(84, 44), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0),
        W(108, 0), CueFirst(0x82157DD8),
    W(80, 45), W(84, 59), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82157E08),
    W(80, 60), W(84, 75), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82157E38),
    Shot(0x82157E44), AddShot(0x82157E54),
    W(112, 15), Racer(116, 3),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 30), W(84, 44), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0),
        W(108, 0), CueFirst(0x82157A64),
    W(80, 45), W(84, 59), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82157A94),
    W(80, 60), W(84, 75), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82157AC4),
    W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(80, 240), W(108, 0), W(84, 269),
        Cue(0x82157AFC),
    Shot(0x82157B08), AddShot(0x82157B18),
};

// Scripts 8, 9, 10, 16, 17, 18 (0x82157E8C): 7 cues, 1 shot.
constexpr Step kScript8_9_10_16_17_18[] = {
    W(112, 0), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82157F00),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 145), W(108, 0), W(84, 154),
        Cue(0x82157F38),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 155), W(108, 0), W(84, 164),
        Cue(0x82157F70),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 310), W(108, 0), W(84, 319),
        Cue(0x82157FA8),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 320), W(108, 0), W(84, 329),
        Cue(0x82157FE0),
    W(80, 330), W(84, 344), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158018),
    W(80, 450), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Scripts 11, 12, 13, 23, 24, 25 (0x82158020): 5 cues, 1 shot.
constexpr Step kScript11_12_13_23_24_25[] = {
    W(112, 1), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82158094),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 330), W(108, 0), W(84, 344),
        Cue(0x821580CC),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 345), W(108, 0), W(84, 359),
        Cue(0x82158104),
    W(80, 360), W(84, 374), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158018),
    W(80, 450), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Scripts 14, 15 (0x82158114): 12 cues, 1 shot.
constexpr Step kScript14_15[] = {
    Vehicle(116), W(112, 3),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x8215818C),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 90), W(108, 0), W(84, 99),
        Cue(0x821581C4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 100), W(108, 0), W(84, 109),
        Cue(0x821581FC),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 160), W(108, 0), W(84, 169),
        Cue(0x82158234),
    W(80, 170), W(84, 179), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x8215826C),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 230), W(108, 0), W(84, 239),
        Cue(0x821582A4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 240), W(108, 0), W(84, 249),
        Cue(0x821582DC),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 280), W(108, 0), W(84, 289),
        Cue(0x82158314),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 290), W(108, 0), W(84, 299),
        Cue(0x8215834C),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 330), W(108, 0), W(84, 339),
        Cue(0x82158384),
    W(80, 340), W(84, 349), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158018),
    W(80, 450), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Scripts 20, 21, 22 (0x82158394): 7 cues, 1 shot.
constexpr Step kScript20_21_22[] = {
    Vehicle(116), W(112, 5),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x8215840C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 145), W(108, 0), W(84, 154),
        Cue(0x82158444),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 155), W(108, 0), W(84, 164),
        Cue(0x8215847C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 310), W(108, 0), W(84, 319),
        Cue(0x821584B4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 320), W(108, 0), W(84, 329),
        Cue(0x821584EC),
    W(80, 330), W(84, 354), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158018),
    W(80, 450), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Script 26 (0x821584FC): 10 cues, 1 shot.
constexpr Step kScript26[] = {
    W(112, 2), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82158570),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 110), W(108, 0), W(84, 119),
        Cue(0x821585A8),
    W(80, 120), W(84, 129), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x8215826C),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 230), W(108, 0), W(84, 239),
        Cue(0x821582A4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 240), W(108, 0), W(84, 249),
        Cue(0x821582DC),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 280), W(108, 0), W(84, 289),
        Cue(0x82158314),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 290), W(108, 0), W(84, 299),
        Cue(0x8215834C),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 330), W(108, 0), W(84, 339),
        Cue(0x82158384),
    W(80, 340), W(84, 349), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158018),
    W(80, 450), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Script 27 (0x821585B8): 4 cues, 1 shot.
constexpr Step kScript27[] = {
    Vehicle(116), W(112, 4),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x82158630),
    W(88, 1), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 240), W(108, 0), W(84, 329),
        Cue(0x82158668),
    W(80, 330), W(84, 349), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158018),
    W(80, 450), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Script 28 (0x82158670): 6 cues, 1 shot.
constexpr Step kScript28[] = {
    W(112, 6), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x821586E4),
    W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(80, 60), W(108, 0), W(84, 84),
        Cue(0x8215871C),
    W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(80, 100), W(108, 0), W(84, 149),
        Cue(0x82158754),
    W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(80, 150), W(108, 0), W(84, 174),
        Cue(0x8215878C),
    W(92, 0), W(96, 0), W(100, 0), W(80, 190), W(84, 209), W(88, 0), W(104, 0), W(108, 0),
        Cue(0x82158AF4),
    W(80, 480), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// Script 29 (0x821587A8): 15 cues, 1 shot.
constexpr Step kScript29[] = {
    W(112, 7), Vehicle(116),
    Reset(120), H(180, 0xFFFF), H(188, 0xFFFF), H(190, 0xFFFF), H(184, 0), H(186, 0xFFFF),
        H(192, 0), W(80, 0), W(84, 39), W(88, 0), W(92, 0), W(96, 0), W(100, 0), W(104, 0),
        W(108, 0), CueFirst(0x8215881C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 70), W(108, 0), W(84, 79),
        Cue(0x82158854),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 80), W(108, 0), W(84, 89),
        Cue(0x8215888C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 120), W(108, 0), W(84, 129),
        Cue(0x821588C4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 130), W(108, 0), W(84, 139),
        Cue(0x821588FC),
    W(80, 140), W(84, 149), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(108, 0),
        Cue(0x82158934),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 180), W(108, 0), W(84, 189),
        Cue(0x8215896C),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 190), W(108, 0), W(84, 199),
        Cue(0x821589A4),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 210), W(108, 0), W(84, 219),
        Cue(0x821589DC),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 220), W(108, 0), W(84, 229),
        Cue(0x82158A14),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 230), W(108, 0), W(84, 239),
        Cue(0x82158A4C),
    W(80, 240), W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(84, 249), W(104, 0), W(108, 0),
        Cue(0x82158A84),
    W(88, 0), W(92, 255), W(96, 255), W(100, 255), W(104, 0), W(80, 340), W(108, 0), W(84, 349),
        Cue(0x82158ABC),
    W(92, 255), W(96, 255), W(100, 255), W(80, 355), W(84, 364), W(88, 0), W(104, 0), W(108, 0),
        Cue(0x82158AF4),
    W(80, 480), W(84, 509), W(88, 1), W(92, 0), W(96, 0), W(100, 0), W(104, 0), W(108, 0),
        Cue(0x82158B2C),
    Shot(0x82158B38), AddShot(0x82158B48),
};

// The jump table 0x82009340: each script's steps.
constexpr std::span<const Step> kScripts[30] = {
    kScript0,  // 0
    kScript1,  // 1
    kScript2,  // 2
    kScript3_6_7,  // 3
    kScript4_19,  // 4
    kScript5,  // 5
    kScript3_6_7,  // 6
    kScript3_6_7,  // 7
    kScript8_9_10_16_17_18,  // 8
    kScript8_9_10_16_17_18,  // 9
    kScript8_9_10_16_17_18,  // 10
    kScript11_12_13_23_24_25,  // 11
    kScript11_12_13_23_24_25,  // 12
    kScript11_12_13_23_24_25,  // 13
    kScript14_15,  // 14
    kScript14_15,  // 15
    kScript8_9_10_16_17_18,  // 16
    kScript8_9_10_16_17_18,  // 17
    kScript8_9_10_16_17_18,  // 18
    kScript4_19,  // 19
    kScript20_21_22,  // 20
    kScript20_21_22,  // 21
    kScript20_21_22,  // 22
    kScript11_12_13_23_24_25,  // 23
    kScript11_12_13_23_24_25,  // 24
    kScript11_12_13_23_24_25,  // 25
    kScript26,  // 26
    kScript27,  // 27
    kScript28,  // 28
    kScript29,  // 29
};

}  // namespace

uint32_t CutsceneScripts(uint8_t* base, uint32_t cutscene, uint32_t script, uint32_t vehicle,
                         uint32_t frame, const CutsceneScriptCalls& calls) {
  const uint32_t f = frame;
  const uint32_t cues = f + 120;
  const uint32_t shots = cutscene + 952;
  for (const Step& step : kScripts[script]) {
    switch (step.kind) {
      case Kind::kWord:
        guest::StoreU32(base, f + step.at, step.value);
        break;
      case Kind::kHalf:
        guest::StoreU16(base, f + step.at, uint16_t(step.value));
        break;
      case Kind::kVehicle:
        guest::StoreU32(base, f + step.at, vehicle);
        break;
      case Kind::kRacer:
        guest::StoreU32(base, f + step.at,
                        guest::LoadU32(base, f + kCutsceneRecords + 8 * step.value + 4));
        break;
      case Kind::kReset:
        for (uint32_t i = 0; i < 16; i++) {
          guest::StoreU16(base, f + step.at + 4 * i + 2, uint16_t(i));
          guest::StoreU16(base, f + step.at + 4 * i + 0, uint16_t(i + 1));
        }
        break;
      case Kind::kCueFirst:
      case Kind::kCue: {
        const uint32_t position =
            step.kind == Kind::kCueFirst
                ? 0
                : uint32_t(int32_t(int16_t(guest::LoadU16(base, cues + 72))));
        uint32_t to = 0;
        if (!List16Insert(base, cues, f + 80, position, 32, &to)) {
          calls.insert32(to, f + 80, 32, step.value);
        }
        break;
      }
      case Kind::kShot:
        // From its frame 192 bytes below this one (its stwu).
        CutsceneInit(base, f + 784, f + 112, f - 192, calls.shot);
        break;
      case Kind::kAddShot: {
        const uint32_t position = uint32_t(int32_t(int16_t(guest::LoadU16(base, shots + 72))));
        uint32_t to = 0;
        if (!List16Insert(base, shots, f + 784, position, 608, &to)) {
          calls.insert608(to, f + 784, 608, step.value);
        }
        break;
      }
    }
  }
  // Scripts 4, 5 and 19 read the cutscene back from the frame's +1652 (their
  // r23 held a cue's tick); the others keep it.
  if (script == 4 || script == 5 || script == 19) {
    return guest::LoadU32(base, f + 1652);
  }
  return cutscene;
}

}  // namespace camera_modes
