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

void CopyKey(uint8_t* base, uint32_t destination, uint32_t source) {
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, destination + 4 * i, guest::LoadU32(base, source + 4 * i));
  }
}

}  // namespace

bool StateKeyEquals(uint8_t* base, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82106040 StateKey::Equals");
  for (uint32_t i = 0; i < 4; i++) {
    if (int32_t(guest::LoadU32(base, a + 4 * i)) != int32_t(guest::LoadU32(base, b + 4 * i))) {
      return false;
    }
  }
  return true;
}

int32_t StateMachineIndexOf(uint8_t* base, uint32_t machine, uint32_t key) {
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

void TransitionMake(uint8_t* base, uint32_t transition, uint32_t key, uint32_t functor) {
  ITC_NATIVE_RUNNING("sub_82106100 Transition::Make");
  CopyKey(base, transition, key);
  // Word by word, as the original copies (the source may overlap).
  for (uint32_t i = 0; i < kStorageWords; i++) {
    guest::StoreU32(base, transition + kFunctorStorage + 4 * i,
                    guest::LoadU32(base, functor + 4 + 4 * i));
  }
  guest::StoreU32(base, transition + kFunctorPointer, transition + kFunctorStorage);
  guest::StoreU32(base, transition + kNextState, guest::LoadU32(base, functor + 28));
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

}  // namespace game_modes
