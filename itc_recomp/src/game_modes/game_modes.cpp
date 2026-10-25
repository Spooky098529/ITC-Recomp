// The game modes and their state machines as plain C++
// (docs/functions/game-modes.md); see game_modes.h.

#include "game_modes/game_modes.h"

#include "base/log.h"
#include "guest.h"

namespace game_modes {

namespace {

// The mode's base vtables (+0 and +32, after the intermediate one at +0)
// and its default delegate's (at +8).
constexpr uint32_t kModeVtableIntermediate = 0x82006B44;
constexpr uint32_t kModeVtable = 0x820069EC;
constexpr uint32_t kModeVtable32 = 0x82006A0C;
constexpr uint32_t kDefaultDelegateVtable = 0x82006B20;

// A transition's parts: the key (+0, four words), the functor pointer
// (+16), its storage (six words from +20) and the next state (+44). A
// functor part (StateMachine::Add's r5) is laid out as a transition's from
// +16: the pointer at +0, the storage from +4, the next state at +28.
constexpr uint32_t kFunctorPointer = 16;
constexpr uint32_t kFunctorStorage = 20;
constexpr uint32_t kNextState = 44;
constexpr uint32_t kStorageWords = 6;

// StateMachine::Find's static default functor part (functor pointer,
// storage, next state at +28) and its guard (bit 0x1).
constexpr uint32_t kDefaultGuard = 0x826C5B88;

void CopyKey(uint8_t* base, uint32_t destination, uint32_t source) {
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, destination + 4 * i, guest::LoadU32(base, source + 4 * i));
  }
}

}  // namespace

StateKey LoadStateKey(uint8_t* base, uint32_t address) {
  StateKey key;
  for (uint32_t i = 0; i < 4; i++) {
    key.words[i] = guest::LoadU32(base, address + 4 * i);
  }
  return key;
}

bool StateKeyEquals(uint8_t* base, uint32_t a, const StateKey& b) {
  ITC_NATIVE_RUNNING("sub_82106040 StateKey::Equals");
  for (uint32_t i = 0; i < 4; i++) {
    if (int32_t(guest::LoadU32(base, a + 4 * i)) != int32_t(b.words[i])) {
      return false;
    }
  }
  return true;
}

int32_t StateMachineIndexOf(uint8_t* base, uint32_t machine, const StateKey& key) {
  ITC_NATIVE_RUNNING("sub_821060A0 StateMachine::IndexOf");
  const uint32_t count = guest::LoadU32(base, machine + kTransitionCount);
  uint32_t transition = machine;
  for (uint32_t i = 0; i < count; i++, transition += kTransitionBytes) {
    if (StateKeyEquals(base, transition, key)) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StateMachineAdd(uint8_t* base, uint32_t machine, uint32_t key, uint32_t functor,
                         bool* replaced) {
  ITC_NATIVE_RUNNING("sub_82106158 StateMachine::Add");
  const StateKey k = LoadStateKey(base, key);
  const int32_t index = StateMachineIndexOf(base, machine, k);
  *replaced = index != -1;
  if (index != -1) {
    // The functor part replaced, the key kept.
    const uint32_t part = machine + kTransitionBytes * uint32_t(index) + kFunctorPointer;
    for (uint32_t i = 0; i < kStorageWords; i++) {
      guest::StoreU32(base, part + 4 + 4 * i, guest::LoadU32(base, functor + 4 + 4 * i));
    }
    guest::StoreU32(base, part, part + 4);
    guest::StoreU32(base, part + 28, guest::LoadU32(base, functor + 28));
    return uint32_t(index);
  }
  // The original makes the transition on its stack (Transition::Make), then
  // counts it and copies it in (Transition::Assign): its words read first.
  uint32_t storage[kStorageWords];
  for (uint32_t i = 0; i < kStorageWords; i++) {
    storage[i] = guest::LoadU32(base, functor + 4 + 4 * i);
  }
  const uint32_t next = guest::LoadU32(base, functor + 28);
  const uint32_t count = guest::LoadU32(base, machine + kTransitionCount);
  guest::StoreU32(base, machine + kTransitionCount, count + 1);
  const uint32_t transition = machine + kTransitionBytes * count;
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, transition + 4 * i, k.words[i]);
  }
  for (uint32_t i = 0; i < kStorageWords; i++) {
    guest::StoreU32(base, transition + kFunctorStorage + 4 * i, storage[i]);
  }
  guest::StoreU32(base, transition + kFunctorPointer, transition + kFunctorStorage);
  guest::StoreU32(base, transition + kNextState, next);
  return transition;
}

uint32_t StateMachineFind(uint8_t* base, uint32_t machine, const StateKey& key, bool* found) {
  ITC_NATIVE_RUNNING("sub_821061F0 StateMachine::Find");
  const uint32_t guard = guest::LoadU32(base, kDefaultGuard);
  if ((guard & 1) == 0) {
    guest::StoreU32(base, kDefaultGuard, guard | 1);
    guest::StoreU32(base, kDefaultTransition + 4, kNoFunctor);
    guest::StoreU32(base, kDefaultTransition + 0, kDefaultTransition + 4);
    guest::StoreU32(base, kDefaultTransition + 28, 0);
  }
  const int32_t index = StateMachineIndexOf(base, machine, key);
  *found = index != -1;
  if (index == -1) {
    return kDefaultTransition;
  }
  return machine + kTransitionBytes * uint32_t(index) + kFunctorPointer;
}

uint32_t StateMachineHandle(uint8_t* base, uint32_t machine, uint32_t message, bool* found) {
  ITC_NATIVE_RUNNING("sub_82106280 StateMachine::Handle");
  // The key as the original builds it on its stack: the message's words,
  // then the state.
  StateKey key;
  key.words[1] = guest::LoadU32(base, message + 0);
  key.words[2] = guest::LoadU32(base, message + 4);
  key.words[3] = guest::LoadU32(base, message + 8);
  key.words[0] = guest::LoadU32(base, machine + kState);
  const uint32_t part = StateMachineFind(base, machine, key, found);
  const uint32_t next = guest::LoadU32(base, part + 28);
  if (next != 0) {
    guest::StoreU32(base, machine + kState, next);
  }
  return part;
}

void TransitionAssign(uint8_t* base, uint32_t destination, uint32_t source) {
  ITC_NATIVE_RUNNING("sub_82105FE0 Transition::Assign");
  CopyKey(base, destination, source);
  for (uint32_t i = 0; i < kStorageWords; i++) {
    guest::StoreU32(base, destination + kFunctorStorage + 4 * i,
                    guest::LoadU32(base, source + kFunctorStorage + 4 * i));
  }
  guest::StoreU32(base, destination + kFunctorPointer, destination + kFunctorStorage);
  guest::StoreU32(base, destination + kNextState, guest::LoadU32(base, source + kNextState));
}

void StateMachineConstruct(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82107108 StateMachine::StateMachine");
  uint32_t transition = machine;
  for (uint32_t i = 0; i < kTransitions; i++, transition += kTransitionBytes) {
    for (uint32_t w = 0; w < 4; w++) {
      guest::StoreU32(base, transition + 4 * w, 0);
    }
    guest::StoreU32(base, transition + kFunctorPointer, 0);
    // The functor's storage gets the empty functor's vtable (a null
    // storage address, which can't happen, would get nothing).
    const uint32_t storage = transition + kFunctorStorage;
    if (storage != 0) {
      guest::StoreU32(base, storage, kNoFunctor);
    }
    guest::StoreU32(base, transition + kNextState, 0);
    guest::StoreU32(base, transition + kFunctorPointer, storage);
  }
}

void ModeConstruct(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82106C20 Mode::Mode");
  guest::StoreU32(base, mode + 0, kModeVtableIntermediate);
  guest::StoreU32(base, mode + 4, 0);
  const uint32_t delegate = mode + 8;
  uint32_t pointer = 0;
  if (delegate != 0) {
    pointer = delegate;
    guest::StoreU32(base, delegate, kDefaultDelegateVtable);
  }
  guest::StoreU32(base, mode + 4, pointer);
  guest::StoreU32(base, mode + 32, kModeVtable32);
  guest::StoreU32(base, mode + 0, kModeVtable);
  guest::StoreU32(base, mode + 32, kModeVtable32);
}

namespace {

// The overlays' handlers: `each(slot)` for each of the 25 slots that holds
// an overlay (the slot read again for each store, as the originals do).
template <typename Each>
void ForEachOverlay(uint8_t* base, uint32_t machine, Each each) {
  for (uint32_t i = 0; i < kOverlaySlots; i++) {
    const uint32_t slot = machine + kOverlays + 4 * i;
    if (guest::LoadU32(base, slot) != 0) {
      each(slot);
    }
  }
}

void StoreInOverlay(uint8_t* base, uint32_t slot, uint32_t offset, uint32_t value) {
  guest::StoreU32(base, guest::LoadU32(base, slot) + offset, value);
}

}  // namespace

void OverlaysSetFlags(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DD58 RaceOverlaysMachine::SetFlags");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    StoreInOverlay(base, slot, 820, 1);
    StoreInOverlay(base, slot, 824, 1);
  });
}

void OverlaysClearFlags(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DD90 RaceOverlaysMachine::ClearFlags");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    StoreInOverlay(base, slot, 820, 0);
    StoreInOverlay(base, slot, 824, 0);
    StoreInOverlay(base, slot, 828, 0);
  });
}

void OverlaysSetFlag820(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8215DDD0 RaceOverlaysMachine::SetFlag820");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    StoreInOverlay(base, slot, 820, 1);
    StoreInOverlay(base, slot, 824, 0);
  });
}

void OverlaysReset(uint8_t* base, uint32_t machine,
                   const std::function<void(uint32_t overlay, uint32_t target)>& reset) {
  ITC_NATIVE_RUNNING("sub_8215DE10 RaceOverlaysMachine::ResetOverlays");
  ForEachOverlay(base, machine, [&](uint32_t slot) {
    const uint32_t overlay = guest::LoadU32(base, slot);
    reset(overlay, guest::LoadU32(base, guest::LoadU32(base, overlay) + 12));
    StoreInOverlay(base, slot, 828, 1);
  });
}

}  // namespace game_modes
