// The type 9 mode's Init, delegate functions, IsLoaded and the helpers over
// its block (docs/functions/todays-result.md): see todays_result.h.

#include "base/log.h"
#include "todays_result/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace todays_result {

namespace {

// The delegate's vtable (ModeDelegate::Invoke) and the thunks.
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;
constexpr uint32_t kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;

constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kStringFile = 171;

// The 8-byte word at `at`, as the originals copy it.
uint64_t Load64(uint8_t* base, uint32_t at) {
  return (uint64_t(Load(base, at)) << 32) | Load(base, at + 4);
}
void Store64(uint8_t* base, uint32_t at, uint64_t value) {
  Store(base, at, uint32_t(value >> 32));
  Store(base, at + 4, uint32_t(value));
}

// The mode's delegate (+8, its holder +4) from the doubleword at `slot`.
// The holder 0 when the delegate's address wraps to 0 (never: kept as the
// originals test it).
void Delegate(uint8_t* base, uint32_t mode, uint32_t slot) {
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
// The same to `function`: the doubleword (function, 0) written at `slot`
// first, as the originals build it there.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  Store(base, slot + 4, 0);
  Store(base, slot, function);
  Delegate(base, mode, slot);
}

// The answer (r3) cleared: +4 and its first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  Store(base, answer + 4, 0);
  guest::StoreU8(base, answer, 0);
}

// The machine of index `index` (0 today's event, 27 today's result), 0 for
// any other (whose words the originals read as they are).
uint32_t MachineOf(uint32_t block, uint32_t index) {
  if (index == 0) {
    return block + kEventAt;
  }
  return index == kResultIndex ? block : 0;
}
// The machine the block's +19280 names.
uint32_t ShownMachine(uint8_t* base, uint32_t block) {
  return MachineOf(block, Load(base, block + kShown));
}

void Dispatch(const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  // Its r4 a leftover the dispatcher doesn't read.
  calls.Call(kDispatch, frame, lr, mode + kDispatcher, mode);
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8225D658 ModeType9::FindShown");
  for (uint32_t index = 0; index < 47; index++) {
    if (index != 0 && index != kResultIndex) {
      continue;
    }
    const uint32_t machine = MachineOf(block, index);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Load(base, machine + kMachineState));
    if (state == 5 || state == 4) {
      Store(base, block + kShown, index);
    }
  }
}

uint32_t Kind(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225BA48 TodaysEvent::Kind");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  if (Load(base, save + kUnlockCount) == 0) {
    return 0;
  }
  const int32_t movie = int32_t(Load(base, save + kQueuedMovie));
  if (movie < 2) {
    return 0;
  }
  if (movie <= 5) {
    return 1;
  }
  return movie == 6 ? 2 : 0;
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225D6D0 ModeType9::Init");
  const uint32_t frame = Frame(base, sp, 112);
  auto block = [&] { return Load(base, mode + kBlock); };
  calls.Call(kScreenStart, frame, 0x8225D6E8, mode);
  calls.Call(kStringsLoad, frame, 0x8225D6F8, Load(base, kStringTable), kStringFile);
  const uint64_t allocated =
      calls.Call(kVirtualAlloc, frame, 0x8225D70C, 0, kBlockBytes, 4096, 4);
  Store(base, mode + kBlock, uint32_t(allocated));
  bool replaced = false;
  ResultInitStates(base, uint32_t(allocated), frame - kInitStatesFrame, &replaced);
  EventInitStates(base, block() + kEventAt, frame - kInitStatesFrame, &replaced);
  const uint32_t dispatcher = mode + kDispatcher;
  for (const uint32_t offset : {0u, kEventAt}) {
    const uint32_t count = Load(base, dispatcher + 200);
    const uint32_t machine = block() + offset;
    Store(base, dispatcher + 200, count + 1);
    Store(base, dispatcher + (count << 2), machine);
  }
  const uint32_t shown = Kind(base, frame) == 0 ? kResultIndex : 0;
  Store(base, block() + 19284, shown);
  Store(base, block() + kShown, shown);
  Store(base, frame + 84, 0);
  Store(base, block() + kMachineReenter, 1);
  Store(base, block() + kMachineDelete, 0);
  Store(base, frame + 80, kSlot8Thunk);
  Store(base, block() + kEventAt + kMachineReenter, 1);
  Store(base, block() + kEventAt + kMachineDelete, 0);
  // The delegate: WaitForLoads through Mode::Slot8Thunk.
  Delegate(base, mode, frame + 80);
  calls.Call(kLoadingShow, frame, 0x8225D7F8, 1, 0, ~uint64_t(0));
  return calls.Call(kFadeIn, frame, 0x8225D814, Load(base, kFadesIn), 0, 0, 0);
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225D918 ModeType9::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  calls.Call(kLoadingRun, frame, 0x8225D940, ~uint64_t(0));
  const uint32_t block = Load(base, mode + kBlock);
  if (Load(base, block + kMachineLoaded) != 0 &&
      Load(base, block + kEventAt + kMachineLoaded) != 0) {
    const uint32_t shown = Load(base, block + kShown);
    if (shown == 0) {
      PostRing(base, mode + kRing, 8);
    } else if (shown == kResultIndex) {
      PostRing(base, mode + kRing, 6);
    }
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
    calls.Call(kLoadingFinish, frame, 0x8225DA0C, ~uint64_t(0));
  }
  PostRing(base, mode + kRing, 1);
  Dispatch(calls, mode, frame, 0x8225DA5C);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DC38 ModeType9::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
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
  PostRing(base, mode + kRing, 1);
  PostRing(base, mode + kRing, 0);
  Dispatch(calls, mode, frame, 0x8225DDE8);
  // The answer (byte 1, the next mode) built over the frame's doubleword
  // at +80 (its other three bytes as they are).
  uint32_t next = 0;
  if (shown_state() == 13) {
    next = 2;
  } else if (shown_state() == 20) {
    next = 3;
  } else {
    return;
  }
  Store(base, frame + 84, next);
  guest::StoreU8(base, frame + 80, 1);
  Store64(base, answer, Load64(base, frame + 80));
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DB70 ModeType9::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock));
  PostRing(base, mode + kRing, 3);
  Dispatch(calls, mode, frame, 0x8225DBEC);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DA68 ModeType9::WaitForShown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock));
  const uint32_t machine = ShownMachine(base, Load(base, mode + kBlock));
  if (int32_t(Load(base, machine + kMachineLoaded)) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, mode + kRing, 1);
  Dispatch(calls, mode, frame, 0x8225DB54);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225DE78 ModeType9::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  PostRing(base, mode + kRing, 4);
  Dispatch(calls, mode, frame, 0x8225DEEC);
  const uint32_t block = Load(base, mode + kBlock);
  for (uint32_t index = 0; index < 47; index++) {
    if (index != 0 && index != kResultIndex) {
      continue;
    }
    const uint32_t machine = MachineOf(block, index);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Load(base, machine + kMachineState));
    if (state != 6 && state != 13 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

uint32_t IsLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8225D6A8 ModeType9::IsLoaded");
  const uint32_t block = Load(base, object + 816);
  if (Load(base, block + kMachineLoaded) == 0) {
    return 0;
  }
  return Load(base, block + kEventAt + kMachineLoaded) != 0 ? 1 : 0;
}

}  // namespace todays_result
