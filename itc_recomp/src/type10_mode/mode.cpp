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

// The mode's delegate (+8, its holder +4) to `function`: the function and
// adjustment from the doubleword at `slot` (function, 0) written first, as
// the originals build them there. The holder 0 when the delegate's address
// wraps to 0 (never: kept as the originals test it).
// The delegate's words from the doubleword at `slot` (function, then
// adjustment: two word stores).
void FillDelegate(uint8_t* base, const Mode& m, uint32_t slot) {
  const uint32_t delegate = m.address + 8;  // the delegate (in host memory, E5 step 4)
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
  const schema::ModeAnswer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// The answer's 8 bytes = `value` (two word stores: word0, then next_type).
void StoreAnswer(uint8_t* base, uint32_t answer, uint64_t value) {
  const schema::ModeAnswer a(base, answer);
  a.set_word0(uint32_t(value >> 32));
  a.set_next_type(uint32_t(value));
}

// The state and loaded words of the machine the block's +19280 names: 0
// the theater, 1 the save screen (the block's embedded machines), any other
// address 0 (whose words the originals read as they are).
int32_t ShownState(const Block& block) {
  const uint32_t index = block.shown();
  if (index == 0) {
    return int32_t(block.theater().state());
  }
  if (index == 1) {
    return int32_t(block.save_screen().state());
  }
  return int32_t(schema::StateMachine(block.base, 0).state());
}
int32_t ShownLoaded(const Block& block) {
  const uint32_t index = block.shown();
  if (index == 0) {
    return int32_t(block.theater().loaded());
  }
  if (index == 1) {
    return int32_t(block.save_screen().loaded());
  }
  return int32_t(schema::StateMachine(block.base, 0).loaded());
}

// The state of the block's machine `index` (0 the theater, 1 the save
// screen).
int32_t MachineState(const Block& block, uint32_t index) {
  return int32_t(index == 0 ? block.theater().state() : block.save_screen().state());
}

// The type 9 mode shares NextModeAnswer and ForceExit (its vtable's slot 12
// and its +32 interface's slot 1): its block (todays-result.toml's
// ModeType9Block) has the answer at the same +19292. True for this mode.
bool IsType10(uint8_t* base, uint32_t mode) {
  return Mode(base, mode).vtable() == Mode::kVtable;
}

// The mode's dispatcher (+36) and ring (+240): addresses inside the mode (a
// handle's interior since E5 step 4), reached through the dual views.
uint32_t Dispatcher(uint8_t* base, uint32_t mode) { return Mode(base, mode).address + 36; }
uint32_t Ring(uint8_t* base, uint32_t mode) { return Mode(base, mode).address + 240; }

void Dispatch(uint8_t* base, const Calls& calls, uint32_t mode, uint32_t frame, uint32_t lr) {
  calls.Call(kDispatch, frame, lr, Dispatcher(base, mode), mode);
}

}  // namespace

void FindShown(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_822888F0 ModeType10::FindShown");
  const Block b(base, block);
  for (uint32_t index = 0; index < 2; index++) {
    const uint32_t machine = index == 0 ? block : b.save_screen().address;
    if (machine == 0) {
      continue;
    }
    const int32_t state = MachineState(b, index);
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
  // The block lives in host memory (E5 step 4): the original's
  // VirtualAlloc(19300, MEM_COMMIT) at 0x8228897C (zero-filled, as New's);
  // the mode's +848 holds its handle.
  const Block allocated = Block::New(base);
  m.set_block(allocated);
  bool replaced = false;
  InitStates(base, allocated.theater().address, frame - kInitStatesFrame, &replaced);
  calls.Call(kSaveScreenInit, frame, 0x82288990, uint64_t(block().save_screen().address));
  const uint64_t r3 = calls.Call(kFadeIn, frame, 0x822889AC, G::fades_in(base), 0, 0, 0);
  Store(base, frame + 84, 0);
  for (const bool save_screen : {false, true}) {
    const uint32_t count = m.handler_count();
    if (!save_screen) {
      Store(base, frame + 80, kSlot8Thunk);
    }
    m.set_handler_count(count + 1);
    const Block b = block();
    schema::host::StoreU32(m.handlers() + (count << 2),  // guest order (E5 step 4)
                           save_screen ? b.save_screen().address : b.theater().address);
  }
  block().set_shown_at_init(0);
  block().set_shown(0);
  block().theater().set_reenter(1);
  block().theater().set_delete_movie(0);
  block().save_screen().set_load_at_once(1);
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
  if (block.theater().loaded() != 0 && block.save_screen().loaded() != 0) {
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
  auto shown_state = [&] { return ShownState(m.block()); };
  if (shown_state() == 8) {
    SetDelegate(base, mode, frame + 80, kSlot11Thunk);
  }
  if (shown_state() == 10) {
    PostRing(base, m.address + 240, 4);
  }
  if (G::pad_blocked0(base) == 0 && G::pad_blocked1(base) == 0) {
    PostRing(base, m.address + 240, 1);
  }
  PostRing(base, m.address + 240, 0);
  Dispatch(base, calls, mode, frame, 0x82289038);
  // The answer (byte 1, the next mode) built over the frame's doubleword
  // at +80 (its other three bytes as they are).
  if (shown_state() == 11) {
    Store(base, frame + 84, 1);
    guest::StoreU8(base, frame + 80, 1);
    StoreAnswer(base, answer, Load64(base, frame + 80));
  } else if (shown_state() == 14) {
    guest::StoreU8(base, frame + 80, 1);
    Store(base, frame + 84, 4);
    const uint64_t next = Load64(base, frame + 80);
    G::set_entered(base, 0);
    StoreAnswer(base, answer, next);
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
  if (ShownLoaded(m.block()) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, m.address + 240, 1);
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
    const uint32_t machine = index == 0 ? block.address : block.save_screen().address;
    if (machine == 0) {
      continue;
    }
    const int32_t state = MachineState(block, index);
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

void NextModeAnswer(uint8_t* base, uint32_t answer, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8225D638 ModeType9::NextModeAnswer");
  uint32_t first, second;
  if (IsType10(base, mode)) {
    const Block block = Mode(base, mode).block();
    first = block.answer_word0();
    second = block.answer_next_type();
  } else {
    // The type 9 mode's block (its view is todays-result's): the words at
    // its +19292 and +19296, as the original loads them.
    const uint32_t block = Mode(base, mode).block<schema::View>().address;
    first = Load(base, block + 19292);
    second = Load(base, block + 19296);
  }
  const schema::ModeAnswer a(base, answer);
  a.set_word0(first);
  a.set_next_type(second);
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
  if (block.theater().loaded() == 0 || block.save_screen().loaded() == 0) {
    return 0;
  }
  if (int32_t(block.save_screen().state()) != 5 && OnList(base) != 1) {
    return 1;
  }
  PostRing(base, iface.address + 208, 15);  // the ring (+208 from the +32 interface)
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
  // The block (the mode's +848), then the doubleword, as the original
  // loads them.
  const uint32_t block = Interface32(base, object).block<schema::View>().address;
  const uint64_t next = Load64(base, slot);
  if (IsType10(base, object - 32)) {
    const Block b(base, block);
    b.set_answer_word0(uint32_t(next >> 32));
    b.set_answer_next_type(uint32_t(next));
  } else {
    // The type 9 mode's block (its view is todays-result's): the words at
    // its +19292 and +19296, as the original stores them.
    Store(base, block + 19292, uint32_t(next >> 32));
    Store(base, block + 19296, uint32_t(next));
  }
  if (number < 2) {
    G::set_entered(base, 1);
  }
}

void Resume(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82288B20 ModeType10::Resume");
  PostRing(base, Interface32(base, object).address + 208, 16);  // the ring (+208 from the +32 interface)
}

}  // namespace type10_mode
