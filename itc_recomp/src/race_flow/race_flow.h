// The race flow natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/race-flow.md): one of the race mode's state machines
// (race mode + 97548, RaceFlow::), which picks how each racer's input record
// is filled through a race. Plain C++ over guest memory (the machine is
// shared with the recompiled code). Adapters: src/hooks/race_flow.cpp. Its
// handlers are the racer controllers' (src/hooks/racer_controllers.cpp).

#pragma once

#include <cstdint>

namespace race_flow {

// The machine's words.
constexpr uint32_t kState = 9604;    // the state (1 at set-up)
constexpr uint32_t kFlags = 9608;    // 0 at set-up; bit 0x1 set by OnSetUp
constexpr uint32_t kPlayers = 9612;  // the human cars' players (+9612, +9616)
// The race data's pointer and its kind.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRaceKind = 0xA3AF0;

// RaceFlow::Init's frame (its r1 less this).
constexpr uint32_t kInitFrame = 3600;
// sub_82160AE0 RaceFlow::Init: the state = 1, the flags 0, the players 0
// and 1 (both 0 when the race kind is 6), then the 53 transitions added
// (StateMachine::Add), built in the original's stack frame at `frame` (its
// r1 less kInitFrame: see race_flow_init.cpp). Returns the last
// StateMachine::Add's answer (`replaced` as its).
uint32_t Init(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace race_flow
