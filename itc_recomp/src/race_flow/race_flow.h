// The race flow natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/race-flow.md): one of the race mode's state machines
// (race mode + 97548, RaceFlow::), which picks how each racer's input record
// is filled through a race. Plain C++ over guest memory (the machine is
// shared with the recompiled code). Adapters: src/hooks/race_flow.cpp. Its
// handlers are the racer controllers' (src/hooks/racer_controllers.cpp).

#pragma once

#include <cstdint>

namespace race_flow {

// The machine is schema::RaceFlow (racer-controllers.toml), the race data
// schema::RaceData (race-states.toml).

// RaceFlow::Init's frame (its r1 less this).
constexpr uint32_t kInitFrame = 3600;
// sub_82160AE0 RaceFlow::Init: the state = 1, the flags 0, the players 0
// and 1 (both 0 when the race kind is 6), then the 53 transitions added
// (StateMachine::Add), built in the original's stack frame at `frame` (its
// r1 less kInitFrame: see race_flow_init.cpp). Returns the last
// StateMachine::Add's answer (`replaced` as its).
uint32_t Init(uint32_t machine, uint32_t frame, bool* replaced);

}  // namespace race_flow
