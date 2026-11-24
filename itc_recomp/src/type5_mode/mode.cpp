// The type 5 mode's Init, delegate functions, +32 interface and the helper
// over its block (docs/functions/type5-mode.md): see type5_mode.h.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/location_screen.h"
#include "schema/type5_mode.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type5ModeGlobals;
using Mode = schema::ModeType5;
using Block = schema::Type5ModeBlock;

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

// The mode's delegate (+8, its holder +4) from the doubleword at `slot`
// (the function, 0, written there first as the originals build it). The
// holder 0 when the delegate's address wraps to 0 (never: kept as the
// originals test it).
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot, function);
  const Mode m(base, mode);
  const uint32_t delegate = mode + 8;
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    m.set_delegate_object(mode);
    // The function and the adjustment, copied as one doubleword (the high
    // word stored first).
    const uint64_t pair = Load64(base, slot);
    m.set_delegate_function(uint32_t(pair >> 32));
    m.set_delegate_adjustment(uint32_t(pair));
    m.set_delegate_vtable(kDelegateVtable);
  }
  m.set_delegate_ptr(holder);
}

// The block's next-mode answer stored as the originals copy it: two words,
// the high one first.
void StoreAnswer(const Block& block, uint64_t value) {
  block.set_answer_word0(uint32_t(value >> 32));
  block.set_answer_next_type(uint32_t(value));
}

// The answer (r3) cleared: +4 and its first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  const schema::ModeAnswer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// Posts the message (id, 0, 0) on the mode's ring (50 slots of 12 bytes),
// as the originals do inline.
void PostRing(uint8_t* base, uint32_t mode, uint32_t id) {
  const Mode m(base, mode);
  const uint32_t count = m.ring_count() + 1;
  const uint32_t end = m.ring_start() + count - 1;
  m.set_ring_count(count);
  const game_modes::RingSlot slot{m.ring() + 12 * (end - (end / 50) * 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// The machine for an id: 3 the car settings (the block), 16 the dialog, 21
// the parking area; any other 0 (whose words NextMode reads as they are).
// Each is a StateMachine (its state at +9604).
schema::StateMachine MachineOf(const Block& block, uint32_t id) {
  switch (id) {
    case 3:
      return schema::StateMachine(block.base, block.car_settings_address());
    case 16:
      return schema::StateMachine(block.base, block.dialog_address());
    case 21:
      return schema::StateMachine(block.base, block.location_screen_address());
    default:
      return schema::StateMachine(block.base, 0);
  }
}

// The three machines' movies loaded (each loaded word 1).
bool AllLoaded(const Block& block) {
  return int32_t(block.location_loaded()) == 1 && int32_t(block.car_settings_loaded()) == 1 &&
         int32_t(block.dialog_loaded()) == 1;
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_8222FD08 ModeType5::FindShown");
  const Block b(base, block);
  for (uint32_t id = 0; id < 47; id++) {
    const auto machine = MachineOf(b, id);
    if (machine.address == 0) {
      continue;
    }
    const int32_t state = int32_t(machine.state());
    if (state == 5 || state == 4) {
      b.set_shown(id);
    }
  }
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222FD68 ModeType5::Init");
  const uint32_t frame = Frame(base, sp, 128);
  const Mode m(base, mode);
  auto block = [&] { return m.block(); };
  calls.Call(kScreenStart, frame, 0x8222FD7C, mode);
  const uint64_t strings =
      calls.Call(kStringsLoad, frame, 0x8222FD8C, G::string_table(base), 0x7FFB);
  calls.Call(kArchiveWaitIdle, frame, 0x8222FD90, strings, 0x7FFB);
  // VirtualAlloc(0, 28940, MEM_COMMIT, PAGE_READWRITE) at 0x8222FDA4: the
  // block in host memory (E5 step 4), zeroed as the pages were; its handle
  // is the r3 the original kept (freed by ModeType5::Release's FreePages).
  const uint64_t allocated = Block::New(base).address;
  m.set_block(uint32_t(allocated));
  calls.Call(kLocationInitStates, frame, 0x8222FDB4,
             Block(base, uint32_t(allocated)).location_screen_address());
  calls.Call(kCarSettingsInitType5, frame, 0x8222FDBC, block().address);
  bool replaced = false;
  DialogInit(base, block().dialog_address(), frame - kDialogInitFrame, &replaced);
  // The three machines added to the dispatcher in the order +9640, +0,
  // +19280.
  for (int i = 0; i < 3; i++) {
    const uint32_t count = m.handler_count();
    m.set_handler_count(count + 1);
    const Block b = block();
    const uint32_t machine = i == 0   ? b.location_screen_address()
                             : i == 1 ? b.car_settings_address()
                                      : b.dialog_address();
    schema::host::StoreU32(m.handlers() + (count << 2), machine);  // guest order (E5 step 4)
  }
  block().set_first_shown(21);
  block().set_shown(21);
  block().set_location_reenter(1);
  block().set_location_delete_movie(0);
  block().set_car_settings_reenter(1);
  block().set_car_settings_delete_movie(0);
  block().set_dialog_reenter(1);
  block().set_dialog_delete_movie(0);
  calls.CallArgs(kFadeIn, frame, 0x8222FE78, {G::fade_in_passes(base), 0, 0, 0, 0, 0, 0, 0});
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
  if (AllLoaded(Mode(base, mode).block())) {
    // Bank 1's cue 3, the two words in the frame.
    StoreU32(base, frame + 80, 3);
    StoreU32(base, frame + 88, 1);
    calls.Call(kSoundPlay, frame, 0x822300BC, frame + 88, frame + 80);
    calls.Call(kLoadingFinish, frame, 0x822300C4, ~uint64_t(0));
    SetDelegate(base, mode, frame + 88, kNextModeThunk);
  }
  PostRing(base, mode, 1);
  calls.Call(kDispatch, frame, 0x82230158, mode + 36, mode);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230458 ModeType5::NextMode");
  const uint32_t frame = Frame(base, sp, 160);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address);
  {
    const Block block = Mode(base, mode).block();
    const auto machine = MachineOf(block, block.shown());
    if (int32_t(machine.state()) == 8) {
      SetDelegate(base, mode, frame + 80, kSlot12Thunk);
    }
  }
  if (int32_t(Mode(base, mode).block().location_state()) == 10) {
    PostRing(base, mode, 4);
  }
  PostRing(base, mode, 1);
  PostRing(base, mode, 0);
  calls.Call(kDispatch, frame, 0x822305F4, mode + 36, 0);
  // The answer (byte 1, the next mode) built over the frame's doubleword
  // at +80 (its other three bytes as they are: the Slot12Thunk word's when
  // the state 8 branch ran this call, else the stack's).
  const Block block = Mode(base, mode).block();
  const int32_t state = int32_t(block.location_state());
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
  StoreAnswer(block, next);
  SetDelegate(base, mode, frame + 80, kSlot14Thunk);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230260 ModeType5::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address);
  PostRing(base, mode, 3);
  calls.Call(kDispatch, frame, 0x822302DC, mode + 36, mode);
  SetDelegate(base, mode, frame + 80, kSlot11Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230168 ModeType5::WaitForShown");
  const uint32_t frame = Frame(base, sp, 112);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address);
  if (AllLoaded(Mode(base, mode).block())) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, mode, 1);
  calls.Call(kDispatch, frame, 0x82230248, mode + 36, mode);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82230328 ModeType5::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  PostRing(base, mode, 4);
  calls.Call(kDispatch, frame, 0x8223039C, mode + 36, mode);
  // The machines' check over the ids 0 to 2, as the original's (none of
  // which is a machine: the check always passes).
  const Block block = Mode(base, mode).block();
  for (uint32_t id = 0; id < 3; id++) {
    const auto machine = MachineOf(block, id);
    if (machine.address == 0) {
      continue;
    }
    const int32_t state = int32_t(machine.state());
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot13Thunk);
}

uint32_t Interrupt(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8222FED0 ModeType5::Interrupt");
  // `object` is the mode's +32 interface.
  const Block block = Mode(base, object - 32).block();
  if (!AllLoaded(block)) {
    return 0;
  }
  if (int32_t(block.dialog_state()) == 9) {
    return 1;
  }
  PostRing(base, object - 32, 18);
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
    G::set_leaving_flag(base, 1);
  }
  StoreAnswer(Mode(base, object - 32).block(), next);
}

void Resume(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8222FC38 ModeType5::Resume");
  // The parking area's screen's restart word (location-screen.toml's).
  G::location_screen<schema::LocationScreen>(base).set_restart(1);
}

uint64_t Slot14Thunk(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222FC50 Mode::Slot14Thunk");
  const uint32_t target =
      schema::Type5SlotObject(base, uint32_t(r4)).vtable().leaving();
  if (target == 0x82230328) {
    Leaving(base, uint32_t(r3), uint32_t(r4), sp, calls);
    return r3;
  }
  return calls.Call(target, sp, lr, r3, r4);
}

}  // namespace type5_mode
