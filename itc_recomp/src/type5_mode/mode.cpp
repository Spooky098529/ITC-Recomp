// The type 5 mode's Init, delegate functions, +32 interface and the helper
// over its block (docs/functions/type5-mode.md): see type5_mode.h.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "guest.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// The delegate's vtable (ModeDelegate::Invoke) and the thunks.
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;
constexpr uint32_t kSlot13Thunk = 0x8222FC80;
constexpr uint32_t kSlot14Thunk = 0x8222FC50;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The 8-byte word at `at`, as the originals copy it.
uint64_t Load64(uint8_t* base, uint32_t at) {
  return (uint64_t(LoadU32(base, at)) << 32) | LoadU32(base, at + 4);
}
void Store64(uint8_t* base, uint32_t at, uint64_t value) {
  StoreU32(base, at, uint32_t(value >> 32));
  StoreU32(base, at + 4, uint32_t(value));
}

// The mode's delegate (+8, its holder +4) from the doubleword at `slot`
// (the function, 0, written there first as the originals build it). The
// holder 0 when the delegate's address wraps to 0 (never: kept as the
// originals test it).
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot, function);
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    StoreU32(base, delegate + 4, mode);
    Store64(base, delegate + 8, Load64(base, slot));
    StoreU32(base, delegate + 0, kDelegateVtable);
  }
  StoreU32(base, mode + 4, holder);
}

// The answer (r3) cleared: +4 and its first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  StoreU32(base, answer + 4, 0);
  guest::StoreU8(base, answer, 0);
}

// Posts the message (id, 0, 0) on the ring at `ring` (its count +600, start
// +604, 50 slots of 12 bytes), as the originals do inline.
void PostRing(uint8_t* base, uint32_t ring, uint32_t id) {
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + 12 * (end - (end / 50) * 50);
  StoreU32(base, slot + 0, id);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

// The machine for an id: 3 the car settings (the block), 16 the dialog, 21
// the parking area; any other 0 (whose words NextMode reads as they are).
uint32_t MachineOf(uint32_t block, uint32_t id) {
  switch (id) {
    case 3:
      return block + kCarSettingsAt;
    case 16:
      return block + kDialogAt;
    case 21:
      return block + kLocationAt;
    default:
      return 0;
  }
}

// The three machines' movies loaded (each loaded word 1).
bool AllLoaded(uint8_t* base, uint32_t block) {
  return int32_t(LoadU32(base, block + kLocationAt + kMachineLoaded)) == 1 &&
         int32_t(LoadU32(base, block + kCarSettingsAt + kMachineLoaded)) == 1 &&
         int32_t(LoadU32(base, block + kDialogAt + kMachineLoaded)) == 1;
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8222FD08 ModeType5::FindShown");
  for (uint32_t id = 0; id < 47; id++) {
    const uint32_t machine = MachineOf(block, id);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(LoadU32(base, machine + kMachineState));
    if (state == 5 || state == 4) {
      StoreU32(base, block + kShown, id);
    }
  }
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222FD68 ModeType5::Init");
  const uint32_t frame = Frame(base, sp, 128);
  auto block = [&] { return LoadU32(base, mode + kBlock); };
  calls.Call(kScreenStart, frame, 0x8222FD7C, mode);
  const uint64_t strings =
      calls.Call(kStringsLoad, frame, 0x8222FD8C, LoadU32(base, kStringTable), 0x7FFB);
  calls.Call(kArchiveWaitIdle, frame, 0x8222FD90, strings, 0x7FFB);
  const uint64_t allocated =
      calls.Call(kVirtualAlloc, frame, 0x8222FDA4, 0, kBlockBytes, 4096, 4);
  StoreU32(base, mode + kBlock, uint32_t(allocated));
  calls.Call(kLocationInitStates, frame, 0x8222FDB4, uint32_t(allocated) + kLocationAt);
  calls.Call(kCarSettingsInitType5, frame, 0x8222FDBC, block());
  bool replaced = false;
  DialogInit(base, block() + kDialogAt, frame - kDialogInitFrame, &replaced);
  // The three machines added to the dispatcher in the order +9640, +0,
  // +19280.
  const uint32_t dispatcher = mode + kDispatcher;
  for (const uint32_t offset : {kLocationAt, kCarSettingsAt, kDialogAt}) {
    const uint32_t count = LoadU32(base, dispatcher + 200);
    StoreU32(base, dispatcher + 200, count + 1);
    StoreU32(base, dispatcher + (count << 2), block() + offset);
  }
  StoreU32(base, block() + kFirstShown, 21);
  StoreU32(base, block() + kShown, 21);
  StoreU32(base, block() + kLocationAt + kMachineReenter, 1);
  StoreU32(base, block() + kLocationAt + kMachineDelete, 0);
  StoreU32(base, block() + kCarSettingsAt + kMachineReenter, 1);
  StoreU32(base, block() + kCarSettingsAt + kMachineDelete, 0);
  StoreU32(base, block() + kDialogAt + kMachineReenter, 1);
  StoreU32(base, block() + kDialogAt + kMachineDelete, 0);
  calls.CallArgs(kFadeIn, frame, 0x8222FE78, {LoadU32(base, kFadesIn), 0, 0, 0, 0, 0, 0, 0});
  const uint64_t r3 = calls.Call(kLoadingShow, frame, 0x8222FE88, 1, 0, ~uint64_t(0));
  // The delegate: WaitForLoads through Mode::Slot8Thunk.
  SetDelegate(base, mode, frame + 80, kSlot8Thunk);
  return r3;
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230050 ModeType5::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  calls.Call(kLoadingRun, frame, 0x82230078, ~uint64_t(0));
  if (AllLoaded(base, LoadU32(base, mode + kBlock))) {
    // Bank 1's cue 3, the two words in the frame.
    StoreU32(base, frame + 80, 3);
    StoreU32(base, frame + 88, 1);
    calls.Call(kSoundPlay, frame, 0x822300BC, frame + 88, frame + 80);
    calls.Call(kLoadingFinish, frame, 0x822300C4, ~uint64_t(0));
    SetDelegate(base, mode, frame + 88, kNextModeThunk);
  }
  PostRing(base, mode + kRing, 1);
  calls.Call(kDispatch, frame, 0x82230158, mode + kDispatcher, mode);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230458 ModeType5::NextMode");
  const uint32_t frame = Frame(base, sp, 160);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock));
  {
    const uint32_t block = LoadU32(base, mode + kBlock);
    const uint32_t machine = MachineOf(block, LoadU32(base, block + kShown));
    if (int32_t(LoadU32(base, machine + kMachineState)) == 8) {
      SetDelegate(base, mode, frame + 80, kSlot12Thunk);
    }
  }
  if (int32_t(LoadU32(base, LoadU32(base, mode + kBlock) + kLocationAt + kMachineState)) == 10) {
    PostRing(base, mode + kRing, 4);
  }
  PostRing(base, mode + kRing, 1);
  PostRing(base, mode + kRing, 0);
  calls.Call(kDispatch, frame, 0x822305F4, mode + kDispatcher, 0);
  // The answer (byte 1, the next mode) built over the frame's doubleword
  // at +80 (its other three bytes as they are: the Slot12Thunk word's when
  // the state 8 branch ran this call, else the stack's).
  const uint32_t block = LoadU32(base, mode + kBlock);
  const int32_t state = int32_t(LoadU32(base, block + kLocationAt + kMachineState));
  if (state == 11) {
    StoreU32(base, frame + 84, 1);
  } else if (state == 13) {
    StoreU32(base, frame + 84, 9);
  } else {
    return;
  }
  guest::StoreU8(base, frame + 80, 1);
  const uint64_t next = Load64(base, frame + 80);
  StoreU32(base, frame + 84, 0);
  Store64(base, block + kNextAnswer, next);
  SetDelegate(base, mode, frame + 80, kSlot14Thunk);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230260 ModeType5::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock));
  PostRing(base, mode + kRing, 3);
  calls.Call(kDispatch, frame, 0x822302DC, mode + kDispatcher, mode);
  SetDelegate(base, mode, frame + 80, kSlot11Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230168 ModeType5::WaitForShown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock));
  if (AllLoaded(base, LoadU32(base, mode + kBlock))) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, mode + kRing, 1);
  calls.Call(kDispatch, frame, 0x82230248, mode + kDispatcher, mode);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230328 ModeType5::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  PostRing(base, mode + kRing, 4);
  calls.Call(kDispatch, frame, 0x8223039C, mode + kDispatcher, mode);
  // The machines' check over the ids 0 to 2, as the original's (none of
  // which is a machine: the check always passes).
  const uint32_t block = LoadU32(base, mode + kBlock);
  for (uint32_t id = 0; id < 3; id++) {
    const uint32_t machine = MachineOf(block, id);
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(LoadU32(base, machine + kMachineState));
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot13Thunk);
}

uint32_t Interrupt(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8222FED0 ModeType5::Interrupt");
  const uint32_t block = LoadU32(base, object + 816);
  if (!AllLoaded(base, block)) {
    return 0;
  }
  if (int32_t(LoadU32(base, block + kDialogAt + kMachineState)) == 9) {
    return 1;
  }
  PostRing(base, object + 208, 18);
  return 0;
}

void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8222FF60 ModeType5::ForceExit");
  if (number >= 4) {
    return;
  }
  // The doubleword built below the stack pointer (sp - 16), as the original.
  const uint32_t slot = sp - 16;
  SetDelegate(base, object - 32, slot, kSlot14Thunk);
  StoreU32(base, slot + 4, number < 2 ? 4 : 12);
  guest::StoreU8(base, slot, 1);
  const uint64_t next = Load64(base, slot);
  if (number < 2) {
    StoreU32(base, kEntered, 1);
  }
  Store64(base, LoadU32(base, object + 816) + kNextAnswer, next);
}

void Resume(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8222FC38 ModeType5::Resume");
  StoreU32(base, LoadU32(base, kLocationScreen) + 900, 1);
}

uint64_t Slot14Thunk(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222FC50 Mode::Slot14Thunk");
  const uint32_t target = LoadU32(base, LoadU32(base, uint32_t(r4)) + 56);
  if (target == 0x82230328) {
    Leaving(base, uint32_t(r3), uint32_t(r4), sp, calls);
    return r3;
  }
  return calls.Call(target, sp, lr, r3, r4);
}

}  // namespace type5_mode
