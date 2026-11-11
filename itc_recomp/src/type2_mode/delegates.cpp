// The type 2 mode's delegate functions, slot thunks and +32 interface
// (docs/functions/type2-mode.md): see type2_mode.h. The messages queued in
// the mode's ring as the originals do it inline (no overflow check),
// GameMessages::Dispatch through its override (a guest call), the
// delegate's member functor rebuilt from its two words in the original's
// frame (copied from there as the original's ld does).

#include "type2_mode/type2_mode.h"

#include "base/log.h"
#include "guest.h"
#include "type2_dialog/type2_dialog.h"

namespace type2_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

void ClearAnswer(uint8_t* base, uint32_t answer) {
  StoreU32(base, answer + 4, 0);
  guest::StoreU8(base, answer + 0, 0);
}

// The ring's next slot (`ring`: its count at +600, the start at +604), the
// count raised.
uint32_t NextSlot(uint8_t* base, uint32_t ring) {
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  return ring + (end - (end / 50) * 50) * 12;
}

// A message (`message`, 0, 0) queued in the mode's ring (+240), the words
// in the original's order (`id_last`: +8, +4, then the id).
void Queue(uint8_t* base, uint32_t mode, uint32_t message, bool id_last = false) {
  const uint32_t slot = NextSlot(base, mode + kRing);
  if (id_last) {
    StoreU32(base, slot + 8, 0);
    StoreU32(base, slot + 4, 0);
    StoreU32(base, slot + 0, message);
  } else {
    StoreU32(base, slot + 0, message);
    StoreU32(base, slot + 4, 0);
    StoreU32(base, slot + 8, 0);
  }
}

uint64_t Dispatch(const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  return calls.Call(kDispatch, frame, lr, mode + kDispatcher, mode);
}

// The delegate (mode +4 = mode +8 when that isn't 0) given the member
// functor whose two words are at `words` (the function and the adjustment
// the frame holds): vtable 0x82010FA0, the mode, the doubleword.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t words) {
  const uint32_t holder = mode + 4;
  uint32_t delegate = holder + 4;
  if (delegate != 0) {
    const uint32_t function = LoadU32(base, words), adjustment = LoadU32(base, words + 4);
    StoreU32(base, delegate + 4, mode);
    StoreU32(base, delegate + 8, function);
    StoreU32(base, delegate + 12, adjustment);
    StoreU32(base, delegate + 0, kDelegateVtable);
  } else {
    delegate = 0;
  }
  StoreU32(base, holder, delegate);
}

// The shown machine's state (MachineOf(block, the block's +0x27B68), read
// again each time).
int32_t ShownState(uint8_t* base, uint32_t mode) {
  const uint32_t block = LoadU32(base, mode + kBlock);
  const uint32_t machine = uint32_t(MachineOf(block, LoadU32(base, block + kShown)));
  return int32_t(LoadU32(base, machine + 9604));
}

}  // namespace

void TearDown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821970C0 ModeType2::TearDown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  if (uint32_t(calls.Call(kPollTeardown, frame, 0x821970EC, answer, mode)) == 0) {
    return;
  }
  const uint32_t from = LoadU32(base, mode + kBlock) + kNextAnswer;
  const uint32_t w0 = LoadU32(base, from), w1 = LoadU32(base, from + 4);
  StoreU32(base, answer + 0, w0);
  StoreU32(base, answer + 4, w1);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197C88 ModeType2::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock), frame);
  Queue(base, mode, 3);
  Dispatch(calls, mode, frame, 0x82197D04);
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot12Thunk);
  SetDelegate(base, mode, frame + 80);
}

void AfterLeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821977E0 ModeType2::AfterLeaveScreen");
  const uint32_t frame = Frame(base, sp, 96);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock), frame);
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kNextModeThunk);
  SetDelegate(base, mode, frame + 80);
}

void Restart(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197A30 ModeType2::Restart");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot9Thunk);
  ClearAnswer(base, answer);
  SetDelegate(base, mode, frame + 80);
  Queue(base, mode, 1);
  Dispatch(calls, mode, frame, 0x82197AE4);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198008 ModeType2::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t slot = NextSlot(base, mode + kRing);
  ClearAnswer(base, answer);
  StoreU32(base, slot + 0, 4);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
  Dispatch(calls, mode, frame, 0x8219807C);
  const uint32_t block = LoadU32(base, mode + kBlock);
  for (uint32_t i = 0; i < 47; i++) {
    const uint32_t machine = uint32_t(MachineOf(block, i));
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(LoadU32(base, machine + 9604));
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, kSlot11Thunk);
  SetDelegate(base, mode, frame + 80);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197D50 ModeType2::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock), frame);
  if (ShownState(base, mode) == 8) {
    StoreU32(base, frame + 84, 0);
    StoreU32(base, frame + 80, kSlot10Thunk);
    SetDelegate(base, mode, frame + 80);
  }
  if (ShownState(base, mode) == 10) {
    Queue(base, mode, 4);
  }
  if (LoadU32(base, kPadBlocked0) == 0 && LoadU32(base, kPadBlocked1) == 0) {
    Queue(base, mode, 1);
  }
  Queue(base, mode, 0);
  Dispatch(calls, mode, frame, 0x82197EF8);
  const uint32_t block = LoadU32(base, mode + kBlock);
  uint32_t next = 0;
  bool leaving_flag = false;
  if (ShownState(base, mode) == 11) {
    next = 1;
  } else if (ShownState(base, mode) == 12) {
    next = 5;
  } else if (int32_t(LoadU32(base, block + 9604)) == 14 ||
             int32_t(LoadU32(base, block + 0x19978 + 9604)) == 14) {
    next = 4;
    leaving_flag = true;
  } else if (ShownState(base, mode) == 20) {
    next = 3;
  } else {
    return;
  }
  // The answer's doubleword at the frame's +80: byte 1, the three bytes the
  // frame holds there, the mode.
  StoreU32(base, frame + 84, next);
  guest::StoreU8(base, frame + 80, 1);
  const uint32_t w0 = LoadU32(base, frame + 80), w1 = LoadU32(base, frame + 84);
  StoreU32(base, block + kNextAnswer, w0);
  StoreU32(base, block + kNextAnswer + 4, w1);
  StoreU32(base, frame + 80, kSlot11Thunk);
  if (leaving_flag) {
    StoreU32(base, kLeavingFlag, 0);
  }
  StoreU32(base, frame + 84, 0);
  SetDelegate(base, mode, frame + 80);
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197B00 ModeType2::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  FindShown(base, LoadU32(base, mode + kBlock), frame);
  constexpr uint64_t kAll = ~uint64_t(0);
  calls.Call(kLoadingRun, frame, 0x82197B30, kAll);
  if (AllLoaded(base, LoadU32(base, mode + kBlock)) == 1) {
    calls.Call(kLoadingFinish, frame, 0x82197B4C, kAll);
    const uint32_t shown = LoadU32(base, LoadU32(base, mode + kBlock) + kShown);
    uint32_t message = 0;
    if (shown == 0) {
      message = 6;
    } else if (shown == 3) {
      calls.Call(kHubPickItem2, frame, 0x82197B80);
      message = 19;
    } else if (shown == 4) {
      message = 10;
    }
    if (message != 0) {
      Queue(base, mode, message, true);
    }
    StoreU32(base, frame + 92, 0);
    StoreU32(base, frame + 88, kNextModeThunk);
    SetDelegate(base, mode, frame + 88);
    StoreU32(base, frame + 80, 2);
    StoreU32(base, frame + 88, 1);
    calls.Call(kSoundPlay, frame, 0x82197C2C, frame + 88, frame + 80);
  }
  Queue(base, mode, 1);
  Dispatch(calls, mode, frame, 0x82197C7C);
}

uint64_t SlotThunk(uint8_t* base, uint32_t slot, uint64_t r3, uint64_t r4, uint32_t sp,
                   uint32_t lr, const Calls& calls) {
  switch (slot) {
    case 8:
      ITC_NATIVE_RUNNING("sub_8223D4B0 Mode::Slot8Thunk");
      break;
    case 9:
      ITC_NATIVE_RUNNING("sub_8223D4C0 Mode::Slot9Thunk");
      break;
    case 10:
      ITC_NATIVE_RUNNING("sub_8223D4A0 Mode::Slot10Thunk");
      break;
    case 11:
      ITC_NATIVE_RUNNING("sub_8222FC60 Mode::Slot11Thunk");
      break;
    case 12:
      ITC_NATIVE_RUNNING("sub_8222FC70 Mode::Slot12Thunk");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_8222FC80 Mode::Slot13Thunk");
      break;
  }
  const uint32_t target = LoadU32(base, LoadU32(base, uint32_t(r4)) + 4 * slot);
  const uint32_t answer = uint32_t(r3), mode = uint32_t(r4);
  switch (target) {
    case 0x82197A30:
      Restart(base, answer, mode, sp, calls);
      return r3;
    case 0x82197B00:
      WaitForLoads(base, answer, mode, sp, calls);
      return r3;
    case 0x82197C88:
      LeaveScreen(base, answer, mode, sp, calls);
      return r3;
    case 0x821970C0:
      TearDown(base, answer, mode, sp, calls);
      return r3;
    case 0x821977E0:
      AfterLeaveScreen(base, answer, mode, sp);
      return r3;
    case 0x82198008:
      Leaving(base, answer, mode, sp, calls);
      return r3;
    default:
      return calls.Call(target, sp, lr, r3, r4);
  }
}

uint32_t Interrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82197858 ModeType2::Interrupt");
  Frame(base, sp, 96);
  const uint32_t block = LoadU32(base, object + 816);
  if (AllLoaded(base, block) == 0) {
    return 0;
  }
  bool post = false;
  if (int32_t(LoadU32(base, block + kSaveScreenState)) == 5 ||
      LoadU32(base, LoadU32(base, kOptionsSettings) + 712) == 0) {
    post = true;
  } else if (int32_t(LoadU32(base, kDialogStatus)) == 3) {
    return 1;
  } else {
    post = type2_dialog::TakesInput(base) != 0;
  }
  if (post) {
    const uint32_t slot = NextSlot(base, object + 208);
    StoreU32(base, slot + 8, 0);
    StoreU32(base, slot + 4, 0);
    StoreU32(base, slot + 0, 48);
  }
  return 0;
}

void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82197930 ModeType2::ForceExit");
  uint32_t next;
  if (number < 2) {
    next = 4;
  } else if (number < 4) {
    next = 12;
  } else {
    return;
  }
  // The functor's doubleword below the stack pointer (sp - 16: the function,
  // 0), then the answer's over it (byte 1, the function's three low bytes,
  // the mode).
  const uint32_t mode = object - 32;
  const uint32_t words = sp - 16;
  StoreU32(base, words, kSlot13Thunk);
  StoreU32(base, words + 4, 0);
  SetDelegate(base, mode, words);
  StoreU32(base, words + 4, next);
  guest::StoreU8(base, words, 1);
  const uint32_t to = LoadU32(base, object + 816) + kNextAnswer;
  const uint32_t w0 = LoadU32(base, words), w1 = LoadU32(base, words + 4);
  StoreU32(base, to, w0);
  StoreU32(base, to + 4, w1);
  if (next == 4) {
    StoreU32(base, kLeavingFlag, 1);
  }
}

}  // namespace type2_mode
