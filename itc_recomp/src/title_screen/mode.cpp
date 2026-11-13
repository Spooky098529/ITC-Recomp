// The type 4 mode's Init, delegate functions, +32 interface and the helpers
// over its block (docs/functions/title-screen.md): see title_screen.h.

#include "base/log.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

// The mode's fields: the delegate's holder (+4) and the delegate (+8: its
// vtable, the mode, the function, the adjustment), the dispatcher (+36: the
// handlers, the count +200), the message ring (+240: 50 entries of 12
// bytes, the count +840, the start +844) and the block (+848).
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kRing = 240;
constexpr uint32_t kBlock = 848;
// The block's words: FindShown's index and the next mode's 8-byte answer.
constexpr uint32_t kShown = 48200;
constexpr uint32_t kNextAnswer = 48212;
// The machines in the block.
constexpr uint32_t kOptionsAt = 0;
constexpr uint32_t kControllerOptionsAt = 9640;
constexpr uint32_t kSaveScreenAt = 19280;
constexpr uint32_t kTitleLoadAt = 28920;
constexpr uint32_t kTitleScreenAt = 38560;

// The delegate's vtable (ModeDelegate::Invoke) and the thunks.
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;
constexpr uint32_t kSlot10Thunk = 0x8223D4A0;
constexpr uint32_t kSlot11Thunk = 0x8222FC60;
constexpr uint32_t kSlot12Thunk = 0x8222FC70;

// The callees by address.
constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kOptionsInit = 0x821CA458;
constexpr uint32_t kControllerOptionsInit = 0x821C8A60;
constexpr uint32_t kSaveScreenInit = 0x821A7470;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kOptionsSettings = 0x826D8478;

// Posts the message (id, 0, 0) on the ring at `ring` (its count +600,
// start +604, 50 slots of 12 bytes).
void PostRing(uint8_t* base, uint32_t ring, uint32_t id) {
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

// The 8-byte word at `at`, as the originals copy it.
uint64_t Load64(uint8_t* base, uint32_t at) {
  return (uint64_t(Load(base, at)) << 32) | Load(base, at + 4);
}
void Store64(uint8_t* base, uint32_t at, uint64_t value) {
  Store(base, at, uint32_t(value >> 32));
  Store(base, at + 4, uint32_t(value));
}

// The mode's delegate (+8, its holder +4) to `function`: the function and
// adjustment from the frame's doubleword at `slot` (function, 0) written
// first, as the originals build them there. The holder 0 when the
// delegate's address wraps to 0 (never: kept as the originals test it).
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

}  // namespace

uint64_t MachineOf(uint64_t block, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_82224B40 ModeType4::MachineOf");
  switch (uint32_t(index)) {
    case 0: return block + kTitleLoadAt;
    case 1: return block + kTitleScreenAt;
    case 2: return block + kOptionsAt;
    case 3: return block + kControllerOptionsAt;
    case 4: return block + kSaveScreenAt;
    default: return 0;
  }
}

uint32_t AllLoaded(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82224BC0 ModeType4::AllLoaded");
  if (Load(base, block + kTitleScreenAt + kMachineLoaded) == 0 ||
      Load(base, block + kTitleLoadAt + kMachineLoaded) == 0 ||
      Load(base, block + kOptionsAt + kMachineLoaded) == 0 ||
      Load(base, block + kSaveScreenAt + kMachineLoaded) == 0) {
    return 0;
  }
  return 1;
}

uint64_t FindShown(uint8_t* base, uint64_t block, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82224C10 ModeType4::FindShown");
  Frame(base, sp, 96);
  uint64_t r3 = 0;
  for (uint32_t index = 0; index < 5; index++) {
    r3 = MachineOf(block, index);
    if (uint32_t(r3) == 0) {
      continue;
    }
    const int32_t state = int32_t(Load(base, uint32_t(r3) + kMachineState));
    if (state == 5 || state == 4) {
      Store(base, uint32_t(block) + kShown, index);
    }
  }
  return r3;
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82224C70 ModeType4::Init");
  const uint32_t frame = Frame(base, sp, 112);
  auto block = [&] { return Load(base, mode + kBlock); };
  calls.Call(kScreenStart, frame, 0x82224C8C, mode);
  calls.Call(kStringsLoad, frame, 0x82224C9C, Load(base, kStringTable), 3);
  const uint64_t allocated = calls.Call(kVirtualAlloc, frame, 0x82224CB4, 0, 48220, 4096, 4);
  Store(base, mode + kBlock, uint32_t(allocated));
  calls.Call(kOptionsInit, frame, 0x82224CBC, allocated);
  calls.Call(kControllerOptionsInit, frame, 0x82224CC8, uint64_t(block()) + kControllerOptionsAt);
  bool replaced = false;
  LoadInit(base, block() + kTitleLoadAt, frame - kLoadInitFrame, &replaced);
  ScreenInit(base, block() + kTitleScreenAt, frame - kScreenInitFrame, &replaced);
  const uint64_t r3 =
      calls.Call(kSaveScreenInit, frame, 0x82224CF4, uint64_t(block()) + kSaveScreenAt);
  const uint32_t dispatcher = mode + kDispatcher;
  for (const uint32_t offset :
       {kOptionsAt, kControllerOptionsAt, kTitleLoadAt, kTitleScreenAt, kSaveScreenAt}) {
    const uint32_t count = Load(base, dispatcher + 200);
    Store(base, dispatcher + 200, count + 1);
    Store(base, dispatcher + (count << 2), block() + offset);
  }
  // The machines' +9612 (open at once) and +9616 (delete the movie when
  // unloaded): the controller options' the other way round.
  Store(base, frame + 84, 0);
  Store(base, frame + 80, kSlot8Thunk);
  for (const uint32_t offset : {kTitleLoadAt, kTitleScreenAt, kOptionsAt}) {
    Store(base, block() + offset + kMachineReenter, 1);
    Store(base, block() + offset + kMachineDelete, 0);
  }
  Store(base, block() + kControllerOptionsAt + kMachineReenter, 0);
  Store(base, block() + kControllerOptionsAt + kMachineDelete, 1);
  Store(base, block() + kSaveScreenAt + kMachineReenter, 1);
  Store(base, block() + kSaveScreenAt + kMachineDelete, 0);
  // The delegate: Restart through Mode::Slot8Thunk (its doubleword already
  // in the frame).
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

void Restart(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225098 ModeType4::Restart");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t block = Load(base, mode + kBlock);
  ClearAnswer(base, answer);
  if (AllLoaded(base, block) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
    const uint32_t entered = Load(base, kEntered);
    if (entered < 2) {
      PostRing(base, mode + kRing, 8);
    } else if (entered == 2) {
      PostRing(base, mode + kRing, 6);
    }
  }
  PostRing(base, mode + kRing, 1);
  calls.Call(kDispatch, frame, 0x822251B8, mode + kDispatcher);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225390 ModeType4::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock), frame);
  auto shown_state = [&] {
    const uint32_t block = Load(base, mode + kBlock);
    const uint32_t machine = uint32_t(MachineOf(block, Load(base, block + kShown)));
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
  calls.Call(kDispatch, frame, 0x82225538, mode + kDispatcher);
  const uint32_t block = Load(base, mode + kBlock);
  uint32_t type = 0;
  switch (shown_state()) {
    case 11: type = 1; break;
    case 13: type = 2; break;
    case 15: type = 12; break;
    case 16: type = 6; break;
    case 17: type = 7; break;
    case 18: type = 8; break;
    case 19: type = 10; break;
    case 20: type = 3; break;
    default: return;
  }
  // The answer (byte 1, the type) built over the frame's doubleword at +80
  // (its other three bytes as they are), then the frame's +84 0.
  Store(base, frame + 84, type);
  guest::StoreU8(base, frame + 80, 1);
  const uint64_t next = Load64(base, frame + 80);
  Store(base, frame + 84, 0);
  Store64(base, block + kNextAnswer, next);
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822252C8 ModeType4::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock), frame);
  PostRing(base, mode + kRing, 3);
  calls.Call(kDispatch, frame, 0x82225344, mode + kDispatcher);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForLeave(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822251D0 ModeType4::WaitForLeave");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  FindShown(base, Load(base, mode + kBlock), frame);
  const uint32_t block = Load(base, mode + kBlock);
  const uint32_t machine = uint32_t(MachineOf(block, Load(base, block + kShown)));
  if (int32_t(Load(base, machine + kMachineLoaded)) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, mode + kRing, 1);
  calls.Call(kDispatch, frame, 0x822252AC, mode + kDispatcher);
}

void NextModeAnswer(uint8_t* base, uint32_t answer, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82224B98 ModeType4::NextModeAnswer");
  const uint32_t at = Load(base, mode + kBlock) + kNextAnswer;
  const uint32_t first = Load(base, at);
  const uint32_t second = Load(base, at + 4);
  Store(base, answer + 0, first);
  Store(base, answer + 4, second);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225698 ModeType4::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  PostRing(base, mode + kRing, 4);
  calls.Call(kDispatch, frame, 0x8222570C, mode + kDispatcher);
  const uint32_t block = Load(base, mode + kBlock);
  for (uint32_t index = 0; index < 47; index++) {
    const uint32_t machine = uint32_t(MachineOf(block, index));
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

uint32_t Interrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82224E20 ModeType4::Interrupt");
  Frame(base, sp, 96);
  const uint32_t block = Load(base, object + 816);
  if (AllLoaded(base, block) == 0) {
    return 0;
  }
  if (int32_t(Load(base, block + kSaveScreenAt + kMachineState)) != 5 &&
      Load(base, Load(base, kOptionsSettings) + 712) != 0) {
    return 1;
  }
  PostRing(base, object + 208, 17);
  return 0;
}

void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82224ED0 ModeType4::ForceExit");
  if (number >= 4) {
    return;
  }
  PostRing(base, object + 208, 4);
  // The doubleword built below the stack pointer (sp - 32), as the original.
  const uint32_t slot = sp - 32;
  const uint32_t mode = object - 32;
  SetDelegate(base, mode, slot, kSlot12Thunk);
  Store(base, slot + 4, number < 2 ? 4 : 12);
  guest::StoreU8(base, slot, 1);
  Store64(base, Load(base, object + 816) + kNextAnswer, Load64(base, slot));
}

void Resume(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82225048 ModeType4::Resume");
  PostRing(base, object + 208, 18);
}

}  // namespace title_screen
