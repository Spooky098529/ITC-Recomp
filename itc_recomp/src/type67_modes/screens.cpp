// The type 6 and 7 modes' handlers with guest calls
// (docs/functions/type6-type7-modes.md): the dialogs' movies opened and
// deleted, the first screens' leave, the screens' Inits over their vtable
// slots, and the Draw handlers that call their page's handler from a table.
// UiMovies::Open and Delete go through Calls (natively where their adapters
// would); the pages' handlers and the vtable slots not ours are guest calls
// from frames like the originals', the state machine Inits' and
// PanelScreen::InitWords' plain C++ where a slot holds them.

#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "guest.h"

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;
using G = schema::Type6Type7ModesGlobals;

constexpr uint32_t kMinusOne = 0xFFFFFFFF;

// A message (`message`, 0, 0) queued in a dispatcher's ring (+204), as the
// handlers do it inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const schema::ModeDispatcher d(base, dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + (end - (end / 50) * 50) * 12};
  slot.set_id(message);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// A vtable slot of `object` called from the frame at `frame` (returning to
// `lr`) with r3 the object: the state machine Inits' and
// PanelScreen::InitWords' plain C++ where the slot holds them, else a guest
// call. Its r3.
uint64_t Slot(uint8_t* base, uint64_t object_r3, uint32_t slot, uint32_t frame, uint32_t lr,
              const Calls& calls) {
  const uint32_t object = uint32_t(object_r3);
  const uint32_t target = LoadU32(base, LoadU32(base, object) + slot);
  if (target == 0x82230740) {
    return PanelScreenInitWords(base, object_r3);
  }
  for (const InitInfo& info : kInits) {
    if (info.address == target) {
      const InitResult result = RunInit(base, target, object, frame - info.frame);
      // StateMachine::Add's adapter's r3: a new transition's address from
      // the machine's 64-bit r3.
      const uint64_t machine_r3 = object_r3 + (result.machine - object);
      return result.replaced ? result.r3 : machine_r3 + (result.r3 - result.machine);
    }
  }
  return calls.call(target, frame, lr, object_r3, 0);
}

}  // namespace

uint64_t DialogOpenMovies(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225810 Dialog::OpenMovies");
  (void)base;
  const uint32_t frame = sp - 96;
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    r3 = calls.open(16 + i, 22559 + i, frame, 0x82225828 + 12 * i);
  }
  return r3;
}

uint64_t DialogLeave(uint8_t* base, uint32_t dialog, uint32_t dispatcher, bool type7,
                     uint32_t sp, const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82241E58 Type7Dialog::Leave");
  } else {
    ITC_NATIVE_RUNNING("sub_82225C70 Dialog::Leave");
  }
  const uint32_t frame = sp - 96;
  const uint32_t lr = type7 ? 0x82241E80 : 0x82225C98;
  schema::Type67Dialog(base, dialog).set_state(7);
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    r3 = calls.remove(16 + i, frame, lr + 8 * i);
  }
  Post(base, dispatcher, type7 ? 3 : 4);
  return r3;
}

uint64_t Screen0Leave(uint8_t* base, uint32_t screen, uint32_t dispatcher, bool type7,
                      uint32_t sp, const Calls& calls) {
  uint64_t r3;
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82244350 Type7Screen0::Leave");
    r3 = calls.remove(22, sp - 112, 0x82244374);
    schema::Type7Screen0(base, screen).set_state(7);
  } else {
    ITC_NATIVE_RUNNING("sub_82233DF8 Type6Screen0::Leave");
    schema::Type6Screen0(base, screen).set_state(7);
    r3 = calls.remove(22, sp - 96, 0x82233E20);
  }
  Post(base, dispatcher, type7 ? 3 : 4);
  return r3;
}

uint64_t ListScreenInit(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822515A0 ListScreen::Init");
  const schema::ListScreen screen(base, uint32_t(r3));
  const uint32_t frame = sp - 112;
  screen.set_next_page(0);
  screen.set_unknown_9664(kMinusOne);
  screen.set_step(kMinusOne);
  screen.set_page(0);
  screen.set_previous_page(0);
  screen.set_step_started(0);
  Slot(base, r3, 16, frame, 0x822515E8, calls);  // ListScreenVtable slot4
  const uint64_t result = Slot(base, r3, 20, frame, 0x822515FC, calls);  // slot5
  screen.set_state(0);
  return result;
}

uint64_t PanelScreenInit(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248E10 PanelScreen::Init");
  const uint32_t frame = sp - 96;
  Slot(base, r3, 0, frame, 0x82248E34, calls);
  return Slot(base, r3, 4, frame, 0x82248E48, calls);
}

uint64_t NodeScreenInit(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82259128 NodeScreen::Init");
  const schema::NodeScreen screen(base, uint32_t(r3));
  const uint32_t frame = sp - 112;
  screen.set_movie(15);
  screen.set_table_movie(15);
  screen.set_next_page(2);
  screen.set_page(2);
  screen.set_previous_page(2);
  screen.set_unknown_10124(kMinusOne);
  screen.set_step(kMinusOne);
  screen.set_step_started(0);
  Slot(base, r3, 28, frame, 0x82259180, calls);  // NodeScreenVtable slot7
  const uint64_t result = Slot(base, r3, 32, frame, 0x82259194, calls);  // slot8
  screen.set_state(0);
  return result;
}

uint64_t Type6Screen0Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82234ED8 Type6Screen0::Draw");
  const uint32_t page = schema::Type6Screen0(base, uint32_t(r3)).pages().page();
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(base, G::type6_draw_table_address() + (page << 2));
  return calls.call(target, sp, lr, r3, r4);  // a tail call
}

uint64_t DialogDraw(uint8_t* base, uint64_t r3, uint64_t r4, bool type7, uint32_t sp,
                    const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822421C0 Type7Dialog::Draw");
  } else {
    ITC_NATIVE_RUNNING("sub_82232740 Type6Dialog::Draw");
  }
  const uint32_t frame = sp - 96;
  const uint32_t page = schema::Type67Dialog(base, uint32_t(r3)).page();
  StoreU32(base, frame + 80, type7 ? 0x82241EF8 : 0x82232328);
  StoreU32(base, frame + 84, type7 ? 0x822420C0 : 0x822324E8);
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(base, frame + 80 + (page << 2));
  return calls.call(target, frame, type7 ? 0x82242204 : 0x82232784, r3, r4);
}

uint64_t Type7Screen0Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82245148 Type7Screen0::Draw");
  constexpr uint32_t kTable = G::type7_draw_table_address();
  const uint32_t frame = sp - 112;
  const uint32_t guard = G::type7_draw_guard(base);
  const uint32_t page = schema::Type7Screen0(base, uint32_t(r3)).pages().page();
  if ((guard & 1) == 0) {
    StoreU32(base, frame + 80, 0x82244BF8);
    StoreU32(base, frame + 88, 0x82244E00);
    StoreU32(base, frame + 96, 0x82244610);
    StoreU32(base, frame + 84, 0);
    StoreU32(base, frame + 92, 0);
    StoreU32(base, frame + 100, 0);
    G::set_type7_draw_guard(base, guard | 1);
    for (uint32_t i = 0; i < 6; i++) {
      StoreU32(base, kTable + 4 * i, LoadU32(base, frame + 80 + 4 * i));
    }
  }
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(base, kTable + (page << 3));
  const uint32_t adjustment = LoadU32(base, kTable + 4 + (page << 3));
  return calls.call(target, frame, 0x822451EC, uint64_t(adjustment) + r3, r4);
}

uint64_t ControllerOptionsDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821C7F28 ControllerOptions::Draw");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame + 80, 0x821C5A20);
  const uint32_t page = G::controller_page(base).page();
  StoreU32(base, frame + 84, 0x821C5B70);
  StoreU32(base, frame + 88, 0x821C5C70);
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(base, frame + 80 + (page << 2));
  return calls.call(target, frame, 0x821C7F84, r4, r4);
}

}  // namespace type67_modes
