// The type 4 mode's Init, delegate functions, +32 interface and the helpers
// over its block (docs/functions/title-screen.md): see title_screen.h.

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "schema/game_modes.h"
#include "schema/options_screen.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

using Mode = schema::ModeType4;
using Interface32 = schema::ModeType4Interface32;
using Block = schema::Type4Block;

// The machines in the block (MachineOf's and Init's offsets: Type4Block's
// options, controller_options, save_screen, title_load and title_screen).
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

// Posts the message (id, 0, 0) on the ring of `v` (the mode, or its +32
// interface): its count, start, 50 slots of 12 bytes.
template <typename V>
void PostRing(uint8_t* base, const V& v, uint32_t id) {
  const uint32_t count = v.ring_count() + 1;
  const uint32_t start = v.ring_start();
  v.set_ring_count(count);
  const game_modes::RingSlot slot{v.ring() + 12 * ((start + count - 1) % 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// The 8-byte word at `at`, as the originals copy it.
uint64_t Load64(uint8_t* base, uint32_t at) {
  return (uint64_t(Load(base, at)) << 32) | Load(base, at + 4);
}

// The mode's delegate (+8, its holder +4) from the frame's doubleword at
// `slot` (function, adjustment). The holder 0 when the delegate's address
// wraps to 0 (never: kept as the originals test it).
void InstallDelegate(uint8_t* base, uint32_t mode, uint32_t slot) {
  const Mode m(base, mode);
  const uint32_t delegate = mode + 8;  // the delegate (in host memory, E5 step 4)
  uint32_t holder = 0;
  if (delegate != 0) {
    holder = delegate;
    m.set_delegate_object(mode);
    const uint64_t function = Load64(base, slot);
    m.set_delegate_function(uint32_t(function >> 32));
    m.set_delegate_adjustment(uint32_t(function));
    m.set_delegate_vtable(kDelegateVtable);
  }
  m.set_delegate_ptr(holder);
}

// The mode's delegate to `function`: the function and adjustment from the
// frame's doubleword at `slot` (function, 0) written first, as the
// originals build them there.
void SetDelegate(uint8_t* base, uint32_t mode, uint32_t slot, uint32_t function) {
  Store(base, slot + 4, 0);
  Store(base, slot, function);
  InstallDelegate(base, mode, slot);
}

// The answer (r3, boot-mode's ModeAnswer) cleared: its type, then its
// first byte.
void ClearAnswer(uint8_t* base, uint32_t answer) {
  const schema::ModeAnswer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// The state of the machine the block's `shown` names.
int32_t ShownState(uint8_t* base, const Mode& m) {
  const Block block = m.block();
  const uint32_t machine = uint32_t(MachineOf(block.address, block.shown()));
  return int32_t(Machine(base, machine).state());
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
  const Block b(base, block);
  if (b.title_screen_loaded() == 0 || b.title_load_loaded() == 0 || b.options_loaded() == 0 ||
      b.save_screen_loaded() == 0) {
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
    const int32_t state = int32_t(Machine(base, uint32_t(r3)).state());
    if (state == 5 || state == 4) {
      Block(base, uint32_t(block)).set_shown(index);
    }
  }
  return r3;
}

uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82224C70 ModeType4::Init");
  const uint32_t frame = Frame(base, sp, 112);
  const Mode m(base, mode);
  auto block = [&] { return m.block(); };
  calls.Call(kScreenStart, frame, 0x82224C8C, mode);
  calls.Call(kStringsLoad, frame, 0x82224C9C, G::string_table(base), 3);
  const uint64_t allocated = calls.Call(kVirtualAlloc, frame, 0x82224CB4, 0, 48220, 4096, 4);
  m.set_block(uint32_t(allocated));
  calls.Call(kOptionsInit, frame, 0x82224CBC, allocated);
  calls.Call(kControllerOptionsInit, frame, 0x82224CC8,
             uint64_t(block().controller_options_address()));
  bool replaced = false;
  LoadInit(base, block().title_load_address(), frame - kLoadInitFrame, &replaced);
  ScreenInit(base, block().title_screen_address(), frame - kScreenInitFrame, &replaced);
  const uint64_t r3 =
      calls.Call(kSaveScreenInit, frame, 0x82224CF4, uint64_t(block().save_screen_address()));
  for (const uint32_t offset :
       {kOptionsAt, kControllerOptionsAt, kTitleLoadAt, kTitleScreenAt, kSaveScreenAt}) {
    const uint32_t count = m.handler_count();
    m.set_handler_count(count + 1);
    schema::host::StoreU32(m.handlers() + (count << 2), block().address + offset);  // guest order (E5 step 4)
  }
  // The machines' reenter (open at once) and delete_movie (delete the movie
  // when unloaded): the controller options' the other way round.
  Store(base, frame + 84, 0);
  Store(base, frame + 80, kSlot8Thunk);
  block().set_title_load_reenter(1);
  block().set_title_load_delete_movie(0);
  block().set_title_screen_reenter(1);
  block().set_title_screen_delete_movie(0);
  block().set_options_reenter(1);
  block().set_options_delete_movie(0);
  block().set_controller_options_reenter(0);
  block().set_controller_options_delete_movie(1);
  block().set_save_screen_reenter(1);
  block().set_save_screen_delete_movie(0);
  // The delegate: Restart through Mode::Slot8Thunk (its doubleword already
  // in the frame).
  InstallDelegate(base, mode, frame + 80);
  return r3;
}

void Restart(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225098 ModeType4::Restart");
  const uint32_t frame = Frame(base, sp, 112);
  const Mode m(base, mode);
  const uint32_t block = m.block().address;
  ClearAnswer(base, answer);
  if (AllLoaded(base, block) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
    const uint32_t entered = G::entered(base);
    if (entered < 2) {
      PostRing(base, m, 8);
    } else if (entered == 2) {
      PostRing(base, m, 6);
    }
  }
  PostRing(base, m, 1);
  calls.Call(kDispatch, frame, 0x822251B8, m.address + 36);
}

void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225390 ModeType4::NextMode");
  const uint32_t frame = Frame(base, sp, 144);
  const Mode m(base, mode);
  ClearAnswer(base, answer);
  FindShown(base, m.block().address, frame);
  if (ShownState(base, m) == 8) {
    SetDelegate(base, mode, frame + 80, kSlot11Thunk);
  }
  if (ShownState(base, m) == 10) {
    PostRing(base, m, 4);
  }
  if (G::pad_blocked0(base) == 0 && G::pad_blocked1(base) == 0) {
    PostRing(base, m, 1);
  }
  PostRing(base, m, 0);
  calls.Call(kDispatch, frame, 0x82225538, m.address + 36);
  const Block block = m.block();
  uint32_t type = 0;
  switch (ShownState(base, m)) {
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
  block.set_next_answer(uint32_t(next >> 32));
  block.set_next_type(uint32_t(next));
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822252C8 ModeType4::LeaveScreen");
  const uint32_t frame = Frame(base, sp, 128);
  const Mode m(base, mode);
  ClearAnswer(base, answer);
  FindShown(base, m.block().address, frame);
  PostRing(base, m, 3);
  calls.Call(kDispatch, frame, 0x82225344, m.address + 36);
  SetDelegate(base, mode, frame + 80, kSlot10Thunk);
}

void WaitForLeave(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822251D0 ModeType4::WaitForLeave");
  const uint32_t frame = Frame(base, sp, 112);
  const Mode m(base, mode);
  ClearAnswer(base, answer);
  FindShown(base, m.block().address, frame);
  const Block block = m.block();
  const uint32_t machine = uint32_t(MachineOf(block.address, block.shown()));
  if (int32_t(Machine(base, machine).loaded()) == 1) {
    SetDelegate(base, mode, frame + 80, kNextModeThunk);
  }
  PostRing(base, m, 1);
  calls.Call(kDispatch, frame, 0x822252AC, m.address + 36);
}

void NextModeAnswer(uint8_t* base, uint32_t answer, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82224B98 ModeType4::NextModeAnswer");
  const Block block = Mode(base, mode).block();
  const uint32_t first = block.next_answer();
  const uint32_t second = block.next_type();
  const schema::ModeAnswer a(base, answer);
  a.set_word0(first);
  a.set_next_type(second);
}

void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225698 ModeType4::Leaving");
  const uint32_t frame = Frame(base, sp, 128);
  const Mode m(base, mode);
  ClearAnswer(base, answer);
  PostRing(base, m, 4);
  calls.Call(kDispatch, frame, 0x8222570C, m.address + 36);
  const uint32_t block = m.block().address;
  for (uint32_t index = 0; index < 47; index++) {
    const uint32_t machine = uint32_t(MachineOf(block, index));
    if (machine == 0) {
      continue;
    }
    const int32_t state = int32_t(Machine(base, machine).state());
    if (state != 6 && state != 11 && state != 14 && state != 12 && state != 20) {
      return;
    }
  }
  SetDelegate(base, mode, frame + 80, kSlot12Thunk);
}

uint32_t Interrupt(uint8_t* base, uint32_t object, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82224E20 ModeType4::Interrupt");
  Frame(base, sp, 96);
  const Interface32 o(base, object);
  const uint32_t block = o.block().address;
  if (AllLoaded(base, block) == 0) {
    return 0;
  }
  // The options' settings (options-screen's OptionsSettings).
  if (int32_t(Block(base, block).save_screen_state()) != 5 &&
      G::options_settings(base).save_page_closed() != 0) {
    return 1;
  }
  PostRing(base, o, 17);
  return 0;
}

void ForceExit(uint8_t* base, uint32_t object, uint32_t number, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82224ED0 ModeType4::ForceExit");
  if (number >= 4) {
    return;
  }
  const Interface32 o(base, object);
  PostRing(base, o, 4);
  // The doubleword built below the stack pointer (sp - 32), as the original.
  const uint32_t slot = sp - 32;
  const uint32_t mode = object - 32;
  SetDelegate(base, mode, slot, kSlot12Thunk);
  Store(base, slot + 4, number < 2 ? 4 : 12);
  guest::StoreU8(base, slot, 1);
  const Block block = o.block();
  const uint64_t next = Load64(base, slot);
  block.set_next_answer(uint32_t(next >> 32));
  block.set_next_type(uint32_t(next));
}

void Resume(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82225048 ModeType4::Resume");
  PostRing(base, Interface32(base, object), 18);
}

}  // namespace title_screen
