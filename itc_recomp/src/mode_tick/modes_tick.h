// The mode manager's tick itself (Modes::Tick, sub_82107910,
// docs/functions/game-modes.md) as plain C++: the play time, the first
// mode's creation, the menu music, the action mappers, the network, the
// sign-in watch, the billboards' ring, the current mode's delegate and its
// change, the screen fades. Its callees (src/mode_tick/mode_tick.h's
// services and the rest) are called through their overrides or the
// recompiled code (the dispatch table, hooks/lifted_call.h). Its wrap in
// src/frame_pacing/per_frame_logic.cpp (the play time held to the tick)
// stays.

#pragma once

#include <cstdint>

namespace mode_tick {

// Modes::Tick (sub_82107910): r3 = the modes' holder (+4 the current mode),
// r4 = the state layer (its slot 1 asked twice after a mode is made). From
// its 208-byte frame below `sp`, r13 the PCR; returns the original's r3.
uint64_t ModesTick(uint32_t sp, uint64_t r13, uint32_t holder, uint32_t layer);

}  // namespace mode_tick
