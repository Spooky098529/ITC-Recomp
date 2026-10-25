// The game modes and their state machines as plain C++
// (docs/functions/game-modes.md); see game_modes.h.

#include "game_modes/game_modes.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"

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

namespace {

constexpr uint32_t kMember67760Vtable = 0x820093C4;
constexpr uint32_t kBuffer28Vtable = 0x82009390;
constexpr uint32_t kMember13360Vtable = 0x820093BC;
constexpr uint32_t kMember43136Vtable = 0x82006B24;
// The word Member13360's +452 is set to.
constexpr uint32_t kMember13360Word452 = 0x82618BB4;

// The 1.0s the camera modes' objects start with: from +44 to +252 and +284
// to +332, every 16 bytes (the w words of their matrices, probably).
void StoreOnes(uint8_t* base, uint32_t object, uint32_t one) {
  for (uint32_t offset = 44; offset <= 252; offset += 16) {
    guest::StoreU32(base, object + offset, one);
  }
  for (uint32_t offset = 284; offset <= 332; offset += 16) {
    guest::StoreU32(base, object + offset, one);
  }
}
// 1.0 in .rdata, the w words' value.
constexpr uint32_t kOne = 0x82000A30;

}  // namespace

void Member67760Construct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82155D90 Member67760::Member67760");
  // The original loads the float and stores it back as a single (the same
  // bits for the constant 1.0).
  const uint32_t one = guest::LoadU32(base, kOne);
  guest::StoreU32(base, object + 16, 0);
  guest::StoreU32(base, object + 20, 0);
  StoreOnes(base, object, one);
  guest::StoreU32(base, object + 0, kMember67760Vtable);
  guest::StoreU32(base, object + 748, 0);
  guest::StoreU32(base, object + 752, 0);
  guest::StoreU32(base, object + 756, 2);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(base, object + 352 + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, object + 352 + 4 * i, uint16_t(i + 1));
  }
  guest::StoreU16(base, object + 416, 0);
  guest::StoreU16(base, object + 424, 0);
  guest::StoreU16(base, object + 412, 0xFFFF);
  guest::StoreU16(base, object + 420, 0xFFFF);
  guest::StoreU16(base, object + 422, 0xFFFF);
  guest::StoreU16(base, object + 418, 0xFFFF);
}

void Member13360Construct(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82155118 Member13360::Member13360");
  const uint32_t one = guest::LoadU32(base, kOne);
  guest::StoreU32(base, object + 16, 0);
  guest::StoreU32(base, object + 20, 0);
  StoreOnes(base, object, one);
  guest::StoreU32(base, object + 0, kMember13360Vtable);
  guest::StoreU32(base, object + 396, one);
  guest::StoreU32(base, object + 412, one);
  guest::StoreU32(base, object + 428, one);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t buffer = object + kBuffers28 + kBuffer28Bytes * i;
    guest::StoreU32(base, buffer + 12, 0xFFFFFFFF);
    guest::StoreU32(base, buffer + 0, kBuffer28Vtable);
    guest::StoreU32(base, buffer + 16, 0xFFFFFFFF);
    guest::StoreU32(base, buffer + 20, 0);
    guest::StoreU32(base, buffer + 24, 0);
  }
  const uint32_t inner = object + 43136;
  guest::StoreU32(base, inner + 16, 0);
  guest::StoreU32(base, inner + 20, 0);
  guest::StoreU32(base, inner + 44, one);
  guest::StoreU32(base, inner + 0, kMember43136Vtable);
  StoreOnes(base, inner, one);
  guest::StoreU32(base, inner + 364, one);
  const uint32_t word = guest::LoadU32(base, kMember13360Word452);
  guest::StoreU32(base, object + 360, 0xFFFFFFFF);
  guest::StoreU32(base, object + 448, 0);
  guest::StoreU32(base, object + 452, word);
  // Each buffer reset (Buffer28::Reset; with no block yet, nothing freed).
  for (uint32_t i = 0; i < 4; i++) {
    Buffer28Reset(base, object + kBuffers28 + kBuffer28Bytes * i);
  }
}

bool Buffer28Freeable(uint8_t* base, uint32_t buffer) {
  const uint32_t block = guest::LoadU32(base, buffer + kBuffer28Block);
  return block == 0 || memory::IsPages(block);
}

uint32_t Buffer28Free(uint8_t* base, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152028 Buffer28::Free");
  const uint32_t block = guest::LoadU32(base, buffer + kBuffer28Block);
  if (block == 0) {
    return 0;
  }
  const uint32_t r3 = memory::FreePages(block) ? 1 : 0;
  guest::StoreU32(base, buffer + kBuffer28Block, 0);
  return r3;
}

uint32_t Buffer28Reset(uint8_t* base, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152D38 Buffer28::Reset");
  const uint32_t r3 = Buffer28Free(base, buffer);
  guest::StoreU32(base, buffer + 12, 0xFFFFFFFF);
  guest::StoreU32(base, buffer + 16, 0xFFFFFFFF);
  guest::StoreU32(base, buffer + 20, 0);
  guest::StoreU32(base, buffer + 24, 0);
  return r3;
}

uint32_t Buffer28Destroy(uint8_t* base, uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_82152D28 Buffer28::~Buffer28");
  guest::StoreU32(base, buffer + 0, kBuffer28Vtable);
  return Buffer28Free(base, buffer);
}

bool Buffers28Freeable(uint8_t* base, uint32_t object) {
  for (uint32_t i = 0; i < 4; i++) {
    if (!Buffer28Freeable(base, object + kBuffers28 + kBuffer28Bytes * i)) {
      return false;
    }
  }
  return true;
}

uint32_t Buffers28Destroy(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82106A00 Buffers28::~Buffers28");
  uint32_t r3 = 0;
  for (int32_t i = 3; i >= 0; i--) {
    r3 = Buffer28Destroy(base, object + kBuffers28 + kBuffer28Bytes * uint32_t(i));
  }
  return r3;
}

}  // namespace game_modes
