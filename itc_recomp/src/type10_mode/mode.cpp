// The type 10 mode's Init, delegate functions, +32 interface and the helper
// over its block (docs/functions/type10-mode.md): see type10_mode.h.

#include "base/log.h"
#include "type10_mode/internal.h"

namespace type10_mode {

namespace {

// The delegate's vtable (ModeDelegate::Invoke) and the thunks.
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;
constexpr uint32_t kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;
constexpr uint32_t kSlot13Thunk = 0x8222FC80;

constexpr uint32_t kStringTable = 0x826C5C34;

// The 8-byte word at `at`, as the originals copy it.
uint64_t Load64(uint8_t* base, uint32_t at) {
  return (uint64_t(Load(base, at)) << 32) | Load(base, at + 4);
}
void Store64(uint8_t* base, uint32_t at, uint64_t value) {
  Store(base, at, uint32_t(value >> 32));
  Store(base, at + 4, uint32_t(value));
}

// The mode's delegate (+8, its holder +4) to `function`: the function and
// adjustment from the doubleword at `slot` (function, 0) written first, as
// the originals build them there. The holder 0 when the delegate's address
// wraps to 0 (never: kept as the originals test it).
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  Store(base, slot + 4, 0);
  Store(base, slot, function);
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    Store(base, delegate + 4, mode);
    Store64(base, delegate + 8, Load64(base, slot));
    Store(base, delegate + 0, kDelegateVtable);
  }
  Store(base, mode + 4, holder);
}

// The answer (r3) cleared: +4 and its first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  Store(base, answer + 4, 0);
  guest::StoreU8(base, answer, 0);
}

// The machine the block's +19280 names: 0 the theater, 1 the save screen,
// any other address 0 (whose words the originals read as they are).
uint32_t ShownMachine(uint8_t* base, uint32_t block) {
  const uint32_t index = Load(base, block + kShown);
  if (index == 0) {
    return block;
  }
  return index == 1 ? block + kSaveScreenAt : 0;
}

void Dispatch(const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  calls.Call(kDispatch, frame, lr, mode + kDispatcher, mode);
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_822888F0 ModeType10::FindShown");
  for (uint32_t index = 0; index < 2; index++) {
    const uint32_t machine = index == 0 ? block : block + kSaveScreenAt;
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Load(base, machine + kMachineState));
    if (state == 5 || state == 4) {
      Store(base, block + kShown, index);
    }
  }
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288940 ModeType10::Init");
  const uint32_t frame = Frame(base, sp, 112);
  auto block = [&] { return Load(base, mode + kBlock); };
  calls.Call(kScreenStart, frame, 0x82288958, mode);
  calls.Call(kStringsLoad, frame, 0x82288968, Load(base, kStringTable), 11);
  const uint64_t allocated =
      calls.Call(kVirtualAlloc, frame, 0x8228897C, 0, kBlockBytes, 4096, 4);
  Store(base, mode + kBlock, uint32_t(allocated));
  bool replaced = false;
  InitStates(base, uint32_t(allocated), frame - kInitStatesFrame, &replaced);
  calls.Call(kSaveScreenInit, frame, 0x82288990, uint64_t(block()) + kSaveScreenAt);
  const uint64_t r3 = calls.Call(kFadeIn, frame, 0x822889AC, Load(base, kFadesIn), 0, 0, 0);
  Store(base, frame + 84, 0);
  const uint32_t dispatcher = mode + kDispatcher;
  for (const uint32_t offset : {0u, kSaveScreenAt}) {
    const uint32_t count = Load(base, dispatcher + 200);
    if (offset == 0) {
      Store(base, frame + 80, kSlot8Thunk);
    }
    Store(base, dispatcher + 200, count + 1);
    Store(base, dispatcher + (count << 2), block() + offset);
  }
  Store(base, block() + 19284, 0);
  Store(base, block() + kShown, 0);
  Store(base, block() + kMachineReenter, 1);
  Store(base, block() + kMachineDelete, 0);
  Store(base, block() + kSaveScreenAt + kMachineReenter, 1);
  Store(base, block() + kSaveScreenAt + kMachineDelete, 0);
  // The delegate: WaitForLoads through Mode::Slot8Thunk (its doubleword
  // already in the frame).
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    Store(base, delegate + 4, mode);
    Store64(base, delegate + 8, Load64(base, frame + 80));
    Store(base, delegate + 0, kDelegateVtable);
  }
  Store(base, mode + 4, holder);
  return r3;
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288B70 ModeType10::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t block = Load(base, mode + kBlock);
  ClearAnswer(base, answer);
  if (Load(base, block + kMachineLoaded) != 0 &&
      Load(base, block + kSaveScreenAt + kMachineLoaded) != 0) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
    PostRing(base, mode + kRing, 5);
  }
  PostRing(base, mode + kRing, 1);
  Dispatch(calls, mode, frame, 0x82288C84);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288E68 ModeType10::NextMode");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock));
  auto shown_state = [&] {
    const uint32_t machine = ShownMachine(base, Load(base, mode + kBlock));
    return int32_t(Load(base, machine + kMachineState));
  };
  if (shown_state() == 8) {
    SetDelegate(base, mode, frame + 80, kSlot11Thunk);
  }
  if (shown_state() == 10) {
    PostRing(base, mode + kRing, 4);
  }
  if (Load(base, kPadBlocked0) == 0 && Load(base, kPadBlocked1) == 0) {
    PostRing(base, mode + kRing, 1);
  }
  PostRing(base, mode + kRing, 0);
  Dispatch(calls, mode, frame, 0x82289038);
  // The answer (byte 1, the next mode) built over the frame's doubleword
  // at +80 (its other three bytes as they are).
  if (shown_state() == 11) {
    Store(base, frame + 84, 1);
    guest::StoreU8(base, frame + 80, 1);
    Store64(base, answer, Load64(base, frame + 80));
  } else if (shown_state() == 14) {
    guest::StoreU8(base, frame + 80, 1);
    Store(base, frame + 84, 4);
    const uint64_t next = Load64(base, frame + 80);
    Store(base, kEntered, 0);
    Store64(base, answer, next);
  }
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288DA0 ModeType10::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock));
  PostRing(base, mode + kRing, 3);
  Dispatch(calls, mode, frame, 0x82288E1C);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288CA0 ModeType10::WaitForShown");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock));
  calls.Call(kLoadingRun, frame, 0x82288CD0, ~uint64_t(0));
  const uint32_t machine = ShownMachine(base, Load(base, mode + kBlock));
  if (int32_t(Load(base, machine + kMachineLoaded)) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, mode + kRing, 1);
  Dispatch(calls, mode, frame, 0x82288D94);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822890D8 ModeType10::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  PostRing(base, mode + kRing, 4);
  Dispatch(calls, mode, frame, 0x8228914C);
  const uint32_t block = Load(base, mode + kBlock);
  for (uint32_t index = 0; index < 47; index++) {
    if (index > 1) {
      continue;
    }
    const uint32_t machine = index == 0 ? block : block + kSaveScreenAt;
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Load(base, machine + kMachineState));
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

void NextModeAnswer(uint8_t* base, uint32_t answer, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8225D638 ModeType9::NextModeAnswer");
  const uint32_t at = Load(base, mode + kBlock) + kNextAnswer;
  const uint32_t first = Load(base, at);
  const uint32_t second = Load(base, at + 4);
  Store(base, answer + 0, first);
  Store(base, answer + 4, second);
}

uint32_t OnList(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822893E8 ReplayTheater::OnList");
  return Load(base, Pages(base) + 0) == 0 ? 1 : 0;
}

uint32_t Interrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82288A68 ModeType10::Interrupt");
  Frame(base, sp, 96);
  const uint32_t block = Load(base, object + 816);
  if (Load(base, block + kMachineLoaded) == 0 ||
      Load(base, block + kSaveScreenAt + kMachineLoaded) == 0) {
    return 0;
  }
  if (int32_t(Load(base, block + kSaveScreenAt + kMachineState)) != 5 && OnList(base) != 1) {
    return 1;
  }
  PostRing(base, object + 208, 15);
  return 0;
}

void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225D828 ModeType9::ForceExit");
  if (number >= 4) {
    return;
  }
  // The doubleword built below the stack pointer (sp - 16), as the original.
  const uint32_t slot = sp - 16;
  SetDelegate(base, object - 32, slot, kSlot13Thunk);
  Store(base, slot + 4, number < 2 ? 4 : 12);
  guest::StoreU8(base, slot, 1);
  Store64(base, Load(base, object + 816) + kNextAnswer, Load64(base, slot));
  if (number < 2) {
    Store(base, kEntered, 1);
  }
}

void Resume(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82288B20 ModeType10::Resume");
  PostRing(base, object + 208, 16);
}

}  // namespace type10_mode
