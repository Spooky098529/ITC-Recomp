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

// A transition's key: (state, the message's three words). Read once: the
// lookups only read, so a key in guest memory is the same throughout.
struct StateKey {
  uint32_t words[4];
};
StateKey LoadStateKey(uint8_t* base, uint32_t address);

// sub_82106040 StateKey::Equals: whether the 16-byte key at `a` and `b` have
// the same four words.
bool StateKeyEquals(uint8_t* base, uint32_t a, const StateKey& b);
// sub_821060A0 StateMachine::IndexOf: the first transition whose key matches,
// or -1.
int32_t StateMachineIndexOf(uint8_t* base, uint32_t machine, const StateKey& key);
// sub_82106158 StateMachine::Add: the key's transition given the functor
// part's functor and next state, or a new transition made (no bound check,
// as the original); the index (`replaced` true) or the new transition.
uint32_t StateMachineAdd(uint8_t* base, uint32_t machine, uint32_t key, uint32_t functor,
                         bool* replaced);
// sub_821061F0 StateMachine::Find: the key's transition's functor part (+16),
// or the static default (made on the first call; `found` false). The
// original's r3 for the default is the address sign-extended, and for a
// transition the machine's r3 plus the offset (the adapters' r3).
uint32_t StateMachineFind(uint8_t* base, uint32_t machine, const StateKey& key, bool* found);
constexpr uint32_t kDefaultTransition = 0x826C5B68;
// sub_82106280 StateMachine::Handle: the functor part for the current state
// and the 12-byte message at `message`; the state moved to the transition's
// next one when that isn't 0.
uint32_t StateMachineHandle(uint8_t* base, uint32_t machine, uint32_t message, bool* found);
// sub_82105FE0 Transition::Assign: a transition copied over another.
void TransitionAssign(uint8_t* base, uint32_t destination, uint32_t source);
// sub_82107108 StateMachine::StateMachine: the 200 transitions emptied.
void StateMachineConstruct(uint8_t* base, uint32_t machine);

// --- The modes (Mode::) ---

// sub_82106C20 Mode::Mode: the base's vtables and default delegate.
void ModeConstruct(uint8_t* base, uint32_t mode);

// --- The race overlays' state machine's message handlers ---

// The race overlays' list in their state machine: 25 slots from +9608.
constexpr uint32_t kOverlays = 9608;
constexpr uint32_t kOverlaySlots = 25;

// sub_8215DD58: each overlay's +820 and +824 = 1.
void OverlaysSetFlags(uint8_t* base, uint32_t machine);
// sub_8215DD90: each overlay's +820, +824 and +828 = 0.
void OverlaysClearFlags(uint8_t* base, uint32_t machine);
// sub_8215DDD0: each overlay's +820 = 1, +824 = 0.
void OverlaysSetFlag820(uint8_t* base, uint32_t machine);
// sub_8215DE10: each overlay's slot 3 (`reset(overlay, target)`, a guest
// call: CarOverlays::Reset in races), then its +828 = 1.
void OverlaysReset(uint8_t* base, uint32_t machine,
                   const std::function<void(uint32_t overlay, uint32_t target)>& reset);

}  // namespace game_modes
