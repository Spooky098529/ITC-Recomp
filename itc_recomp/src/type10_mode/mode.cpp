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

using Mode = schema::ModeType10;
using Block = schema::ModeType10Block;
using Interface32 = schema::ModeType10Interface32;

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
// The delegate's words from the doubleword at `slot` (function, then
// adjustment: two word stores, as Store64).
void FillDelegate(uint8_t* base, const Mode& m, uint32_t slot) {
  const uint32_t delegate = m.delegate_vtable_address();
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    m.set_delegate_object(m.address);
    const uint64_t words = Load64(base, slot);
    m.set_delegate_function(uint32_t(words >> 32));
    m.set_delegate_adjustment(uint32_t(words));
    m.set_delegate_vtable(kDelegateVtable);
  }
  m.set_delegate_ptr(holder);
}

void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  Store(base, slot + 4, 0);
  Store(base, slot, function);
  FillDelegate(base, Mode(base, mode), slot);
}

// The answer (r3) cleared: +4 and its first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  Store(base, answer + 4, 0);
  guest::StoreU8(base, answer, 0);
}

// The machine the block's +19280 names: 0 the theater, 1 the save screen,
// any other address 0 (whose words the originals read as they are).
schema::StateMachine ShownMachine(const Block& block) {
  const uint32_t index = block.shown();
  if (index == 0) {
    return schema::StateMachine(block.base, block.address);
  }
  return schema::StateMachine(block.base, index == 1 ? block.save_screen_address() : 0);
}

// The mode's dispatcher (+36) and ring (+240).
uint32_t Dispatcher(uint8_t* base, uint32_t mode) { return Mode(base, mode).handlers_address(); }
uint32_t Ring(uint8_t* base, uint32_t mode) { return Mode(base, mode).ring_address(); }

void Dispatch(uint8_t* base, const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  calls.Call(kDispatch, frame, lr, Dispatcher(base, mode), mode);
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_822888F0 ModeType10::FindShown");
  const Block b(base, block);
  for (uint32_t index = 0; index < 2; index++) {
    const uint32_t machine = index == 0 ? block : b.save_screen_address();
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(schema::StateMachine(base, machine).state());
    if (state == 5 || state == 4) {
      b.set_shown(index);
    }
  }
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288940 ModeType10::Init");
  const uint32_t frame = Frame(base, sp, 112);
  const Mode m(base, mode);
  auto block = [&] { return m.block(); };
  calls.Call(kScreenStart, frame, 0x82288958, mode);
  calls.Call(kStringsLoad, frame, 0x82288968, G::string_table(base), 11);
  const uint64_t allocated =
      calls.Call(kVirtualAlloc, frame, 0x8228897C, 0, kBlockBytes, 4096, 4);
  m.set_block(uint32_t(allocated));
  bool replaced = false;
  InitStates(base, uint32_t(allocated), frame - kInitStatesFrame, &replaced);
  calls.Call(kSaveScreenInit, frame, 0x82288990, uint64_t(block().save_screen_address()));
  const uint64_t r3 = calls.Call(kFadeIn, frame, 0x822889AC, G::fades_in(base), 0, 0, 0);
  Store(base, frame + 84, 0);
  for (const bool save_screen : {false, true}) {
    const uint32_t count = m.handler_count();
    if (!save_screen) {
      Store(base, frame + 80, kSlot8Thunk);
    }
    m.set_handler_count(count + 1);
    const Block b = block();
    Store(base, m.handlers_address() + (count << 2),
          save_screen ? b.save_screen_address() : b.theater_address());
  }
  block().set_shown_at_init(0);
  block().set_shown(0);
  block().set_theater_reenter(1);
  block().set_theater_delete_movie(0);
  block().set_save_screen_reenter(1);
  block().set_save_screen_delete_movie(0);
  // The delegate: WaitForLoads through Mode::Slot8Thunk (its doubleword
  // already in the frame).
  FillDelegate(base, m, frame + 80);
  return r3;
}

void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288B70 ModeType10::WaitForLoads");
  const uint32_t frame = Frame(base, sp, 112);
  const Block block = Mode(base, mode).block();
  ClearAnswer(base, answer);
  if (block.theater_loaded() != 0 && block.save_screen_loaded() != 0) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
    PostRing(base, Ring(base, mode), 5);
  }
  PostRing(base, Ring(base, mode), 1);
  Dispatch(base, calls, mode, frame, 0x82288C84);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288E68 ModeType10::NextMode");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  const Mode m(base, mode);
  FindShown(base, m.block().address);
  auto shown_state = [&] { return int32_t(ShownMachine(m.block()).state()); };
  if (shown_state() == 8) {
    SetDelegate(base, mode, frame + 80, kSlot11Thunk);
  }
  if (shown_state() == 10) {
    PostRing(base, m.ring_address(), 4);
  }
  if (G::pad_blocked0(base) == 0 && G::pad_blocked1(base) == 0) {
    PostRing(base, m.ring_address(), 1);
  }
  PostRing(base, m.ring_address(), 0);
  Dispatch(base, calls, mode, frame, 0x82289038);
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
    G::set_entered(base, 0);
    Store64(base, answer, next);
  }
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288DA0 ModeType10::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  FindShown(base, Mode(base, mode).block().address);
  PostRing(base, Ring(base, mode), 3);
  Dispatch(base, calls, mode, frame, 0x82288E1C);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82288CA0 ModeType10::WaitForShown");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  const Mode m(base, mode);
  FindShown(base, m.block().address);
  calls.Call(kLoadingRun, frame, 0x82288CD0, ~uint64_t(0));
  const schema::StateMachine machine = ShownMachine(m.block());
  if (int32_t(machine.loaded()) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, m.ring_address(), 1);
  Dispatch(base, calls, mode, frame, 0x82288D94);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822890D8 ModeType10::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  ClearAnswer(base, answer);
  PostRing(base, Ring(base, mode), 4);
  Dispatch(base, calls, mode, frame, 0x8228914C);
  const Block block = Mode(base, mode).block();
  for (uint32_t index = 0; index < 47; index++) {
    if (index > 1) {
      continue;
    }
    const uint32_t machine = index == 0 ? block.address : block.save_screen_address();
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(schema::StateMachine(base, machine).state());
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

void NextModeAnswer(uint8_t* base, uint32_t answer, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8225D638 ModeType9::NextModeAnswer");
  const Block block = Mode(base, mode).block();
  const uint32_t first = block.answer_word0();
  const uint32_t second = block.answer_next_type();
  Store(base, answer + 0, first);
  Store(base, answer + 4, second);
}

uint32_t OnList(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822893E8 ReplayTheater::OnList");
  return Pages(base).page() == 0 ? 1 : 0;
}

uint32_t Interrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82288A68 ModeType10::Interrupt");
  Frame(base, sp, 96);
  const Interface32 iface(base, object);
  const Block block = iface.block();
  if (block.theater_loaded() == 0 || block.save_screen_loaded() == 0) {
    return 0;
  }
  if (int32_t(block.save_screen_state()) != 5 && OnList(base) != 1) {
    return 1;
  }
  PostRing(base, iface.ring_address(), 15);
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
  const Block block = Interface32(base, object).block();
  const uint64_t next = Load64(base, slot);
  block.set_answer_word0(uint32_t(next >> 32));
  block.set_answer_next_type(uint32_t(next));
  if (number < 2) {
    G::set_entered(base, 1);
  }
}

void Resume(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82288B20 ModeType10::Resume");
  PostRing(base, Interface32(base, object).ring_address(), 16);
}

}  // namespace type10_mode
