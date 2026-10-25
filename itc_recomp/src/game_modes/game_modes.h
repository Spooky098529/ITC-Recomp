// The game modes and their state machines as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/game-modes.md, the layouts in docs/known-data.md: "game
// mode", "race mode", "state machine", "transition"). The modes, their state
// machines and the race overlays' list are guest data shared with the
// recompiled code, so these work on guest memory, with the originals' loads
// and stores in their order. Adapters: src/hooks/game_modes.cpp.

#pragma once

#include <cstdint>
#include <functional>

namespace game_modes {

// --- The state machines (StateMachine::, Transition::, StateKey::) ---

// A state machine: up to 200 48-byte transitions from +0, their count at
// +9600 and the state at +9604.
constexpr uint32_t kTransitionBytes = 48;
constexpr uint32_t kTransitions = 200;
constexpr uint32_t kTransitionCount = 9600;
constexpr uint32_t kState = 9604;
// The empty functor's vtable (a transition with nothing to run).
constexpr uint32_t kNoFunctor = 0x820069BC;

// sub_82106040 StateKey::Equals: whether the two 16-byte keys' four words
// match.
bool StateKeyEquals(uint8_t* base, uint32_t a, uint32_t b);
// sub_821060A0 StateMachine::IndexOf: the first transition whose key matches,
// or -1.
int32_t StateMachineIndexOf(uint8_t* base, uint32_t machine, uint32_t key);
// sub_82106100 Transition::Make: a transition from a key and a functor part
// (its storage from +4, the next state at +28).
void TransitionMake(uint8_t* base, uint32_t transition, uint32_t key, uint32_t functor);
// sub_82105FE0 Transition::Assign: a transition copied over another.
void TransitionAssign(uint8_t* base, uint32_t destination, uint32_t source);
// sub_82107108 StateMachine::StateMachine: the 200 transitions emptied.
void StateMachineConstruct(uint8_t* base, uint32_t machine);

// --- The modes (Mode::) ---

// sub_82106C20 Mode::Mode: the base's vtables and default delegate.
void ModeConstruct(uint8_t* base, uint32_t mode);

}  // namespace game_modes
