// The type 2 mode's delegate functions, slot thunks and +32 interface
// (docs/functions/type2-mode.md): see type2_mode.h. The messages queued in
// the mode's ring as the originals do it inline (no overflow check),
// GameMessages::Dispatch through its override (a guest call), the
// delegate's member functor rebuilt from its two words in the original's
// frame (copied from there as the original's ld does).

#include "type2_mode/type2_mode.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/options_screen.h"
#include "schema/type2_mode.h"
#include "type2_dialog/type2_dialog.h"

namespace type2_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type2ModeGlobals;
using Mode = schema::ModeType2;
using Block = schema::Type2ModeBlock;
using Machine = schema::Type2ScreenMachine;
using Message = game_modes::RingSlot;  // a slot in the mode's ring (its bytes, E5 step 4)

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

void ClearAnswer(uint8_t* base, uint32_t answer) {
  const schema::ModeAnswer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// The mode's ring's next slot (its count +840, the start +844), the count
// raised.
Message NextSlot(uint8_t* base, uint32_t mode) {
  const Mode m(base, mode);
  const uint32_t count = m.ring_count() + 1;
  const uint32_t end = m.ring_start() + count - 1;
  m.set_ring_count(count);
  return Message{m.ring() + (end - (end / 50) * 50) * schema::ModeMessage::kSize};
}

// A message (`message`, 0, 0) queued in the mode's ring (+240), the words
// in the original's order (`id_last`: +8, +4, then the id).
void Queue(uint8_t* base, uint32_t mode, uint32_t message, bool id_last = false) {
  const Message slot = NextSlot(base, mode);
  if (id_last) {
    slot.set_arg1(0);
    slot.set_arg0(0);
    slot.set_id(message);
  } else {
    slot.set_id(message);
    slot.set_arg0(0);
    slot.set_arg1(0);
  }
}

uint64_t Dispatch(const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  return calls.Call(kDispatch, frame, lr, mode + 36, mode);  // the dispatcher (+36)
}

// The delegate (mode +4 = mode +8 when that isn't 0) given the member
// functor's two words (the function and the adjustment; the original built
// them in its frame and read them back, C++ values since E5 step 6):
// vtable 0x82010FA0, the mode, the doubleword.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t function, uint32_t adjustment = 0) {
  const Mode m(base, mode);
  uint32_t delegate = mode + 8;  // the delegate (in host memory, E5 step 4)
  if (delegate != 0) {
    m.set_delegate_object(mode);
    m.set_delegate_function(function);
    m.set_delegate_adjustment(adjustment);
    m.set_delegate_vtable(kDelegateVtable);
  } else {
    delegate = 0;
  }
  m.set_delegate_ptr(delegate);
}

// The shown machine's state (MachineOf(block, the block's +0x27B68), read
// again each time).
int32_t ShownState(uint8_t* base, uint32_t mode) {
  const Block block = Mode(base, mode).block();
  const uint32_t machine = uint32_t(MachineOf(block.address, block.shown()));
  return int32_t(Machine(base, machine).state());
}

}  // namespace

void TearDown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821970C0 ModeType2::TearDown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  if (uint32_t(calls.Call(kPollTeardown, frame, 0x821970EC, answer, mode)) == 0) {
    return;
  }
  const Block from = Mode(base, mode).block();
  const uint32_t w0 = from.answer_word0(), w1 = from.answer_next_type();
  const schema::ModeAnswer to(base, answer);
  to.set_word0(w0);
  to.set_next_type(w1);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197C88 ModeType2::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address, frame);
  Queue(base, mode, 3);
  Dispatch(calls, mode, frame, 0x82197D04);
  SetDelegate(base, mode, kSlot12Thunk);
}

void AfterLeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821977E0 ModeType2::AfterLeaveScreen");
  // No frame (E5 step 6: its words are values, no guest call); FindShown's
  // below where it was.
  const uint32_t frame = sp - 96;
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address, frame);
  SetDelegate(base, mode, kNextModeThunk);
}

void Restart(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197A30 ModeType2::Restart");
  const uint32_t frame = Frame(base, sp, 112);  // for Dispatch's guest call
  ClearAnswer(base, answer);
  SetDelegate(base, mode, kSlot9Thunk);
  Queue(base, mode, 1);
  Dispatch(calls, mode, frame, 0x82197AE4);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82198008 ModeType2::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  const Message slot = NextSlot(base, mode);
  ClearAnswer(base, answer);
  slot.set_id(4);
  slot.set_arg0(0);
  slot.set_arg1(0);
  Dispatch(calls, mode, frame, 0x8219807C);
  const uint32_t block = Mode(base, mode).block().address;
  for (uint32_t i = 0; i < 47; i++) {
    const uint32_t machine = uint32_t(MachineOf(block, i));
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Machine(base, machine).state());
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, kSlot11Thunk);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197D50 ModeType2::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address, frame);
  if (ShownState(base, mode) == 8) {
    // The functor's words stay in the frame: the answer below takes the
    // three low bytes the frame holds at +80 (this one's or an earlier
    // call's, E5 step 6).
    StoreU32(base, frame + 84, 0);
    StoreU32(base, frame + 80, kSlot10Thunk);
    SetDelegate(base, mode, kSlot10Thunk, 0);
  }
  if (ShownState(base, mode) == 10) {
    Queue(base, mode, 4);
  }
  if (G::pad_blocked0(base) == 0 && G::pad_blocked1(base) == 0) {
    Queue(base, mode, 1);
  }
  Queue(base, mode, 0);
  Dispatch(calls, mode, frame, 0x82197EF8);
  const Block block = Mode(base, mode).block();
  uint32_t next = 0;
  bool leaving_flag = false;
  if (ShownState(base, mode) == 11) {
    next = 1;
  } else if (ShownState(base, mode) == 12) {
    next = 5;
  } else if (int32_t(block.hub_state()) == 14 ||
             int32_t(Machine(base, block.car_dealer_address()).state()) == 14) {
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
  block.set_answer_word0(w0);
  block.set_answer_next_type(w1);
  StoreU32(base, frame + 80, kSlot11Thunk);
  if (leaving_flag) {
    G::set_leaving_flag(base, 0);
  }
  StoreU32(base, frame + 84, 0);
  SetDelegate(base, mode, kSlot11Thunk, 0);
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82197B00 ModeType2::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 144);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address, frame);
  constexpr uint64_t kAll = ~uint64_t(0);
  calls.Call(kLoadingRun, frame, 0x82197B30, kAll);
  if (AllLoaded(base, Mode(base, mode).block().address) == 1) {
    calls.Call(kLoadingFinish, frame, 0x82197B4C, kAll);
    const uint32_t shown = Mode(base, mode).block().shown();
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
    // The functor's words kept in the frame beside Sound::Play's (+88 is
    // handed to it); the delegate given them as values.
    StoreU32(base, frame + 92, 0);
    StoreU32(base, frame + 88, kNextModeThunk);
    SetDelegate(base, mode, kNextModeThunk, 0);
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
  const uint32_t target =
      LoadU32(base, schema::ModeSlotObject(base, uint32_t(r4)).vtable().address + 4 * slot);
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
  (void)sp;  // no frame (E5 step 6: the original's held nothing, no call made)
  // The mode's fields from its +32 interface (`object` = the mode + 32).
  const Mode mode(base, object - 32);
  const Block block = mode.block();
  if (AllLoaded(base, block.address) == 0) {
    return 0;
  }
  bool post = false;
  if (int32_t(block.save_screen_state()) == 5 ||
      G::options_settings(base).save_page_closed() == 0) {
    post = true;
  } else if (int32_t(G::dialog_status(base)) == 3) {
    return 1;
  } else {
    post = type2_dialog::TakesInput(base) != 0;
  }
  if (post) {
    const Message slot = NextSlot(base, mode.address);
    slot.set_arg1(0);
    slot.set_arg0(0);
    slot.set_id(48);
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
  // The functor's doubleword (the original's below the stack pointer, sp -
  // 16: the function, 0), then the answer's over it (byte 1, the function's
  // three low bytes, the mode): values since E5 step 6.
  (void)sp;
  const uint32_t mode = object - 32;
  SetDelegate(base, mode, kSlot13Thunk, 0);
  const Block to = Mode(base, mode).block();
  const uint32_t w0 = 0x01000000u | (kSlot13Thunk & 0x00FFFFFFu), w1 = next;
  to.set_answer_word0(w0);
  to.set_answer_next_type(w1);
  if (next == 4) {
    G::set_leaving_flag(base, 1);
  }
}

}  // namespace type2_mode
