// The page object and the menu list (docs/functions/race-screens.md): see
// race_screens.h.

#include "race_screens/race_screens.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/action_mappers.h"
#include "schema/race_screens.h"
#include "schema/racer_controllers.h"

namespace race_screens {

namespace {

int32_t S(uint32_t value) { return int32_t(value); }

schema::MenuList L(uint32_t list) { return schema::MenuList(list); }

schema::PageObject P(uint32_t pages) { return schema::PageObject(pages); }

using Controllers = schema::RacerControllersGlobals;

// A player's entry in the action mappers' table (racer-controllers.toml's
// PlayerMapperEntry: +0 the mapper, +4 1 when it isn't read).
schema::PlayerMapperEntry E(uint32_t entry) {
  return schema::PlayerMapperEntry(entry);
}

// The list's player's entry in the action mappers' table.
uint32_t EntryOf(uint32_t list) {
  const uint32_t index =
      guest::LoadU32(L(list).player() * 4 + Controllers::player_entries_address());
  return index * 8 + Controllers::mapper_entries_address();
}

// A mapper's read slot (`offset` in its vtable) on `action`, as the
// original's bctrl from `frame` returning to `lr`: the slot's plain C++ when
// it's `native` and `native_slots`, logged for --log_pause_menu as its
// adapter does, else the slot through `calls`.
uint64_t ReadSlot(uint32_t mapper, uint32_t offset, uint32_t native, int slot,
                  uint32_t action, uint32_t frame, uint32_t lr, bool native_slots,
                  const Calls& calls) {
  const uint32_t function = guest::LoadU32(guest::LoadU32(mapper) + offset);
  if (native_slots && function == native) {
    const uint32_t r3 = native == kPressedSlot ? action_mappers::Pressed(mapper, action)
                                               : action_mappers::Fired(mapper, action);
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, action, lr, double(r3));
    return r3;
  }
  return calls.Call(function, frame, lr, mapper, action);
}

// One Fired try on the entry: 0 when its +4 is 1, else the slot's answer
// (its low byte).
bool FiredTry(uint32_t entry, uint32_t action, uint32_t frame, uint32_t lr,
              bool native_slots, const Calls& calls) {
  if (S(E(entry).unread()) == 1) {
    return false;
  }
  const uint32_t mapper = E(entry).mapper().address;
  return (ReadSlot(mapper, 48, kFiredSlot, 12, action, frame, lr, native_slots, calls) &
          0xFF) != 0;
}

uint32_t Pressed(uint32_t list, uint32_t action, uint32_t sp, uint32_t lr,
                 bool native_slots, const Calls& calls) {
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  const uint32_t entry = EntryOf(list);
  if (S(E(entry).unread()) == 1) {
    return 0;
  }
  const uint32_t mapper = E(entry).mapper().address;
  const uint64_t r3 =
      ReadSlot(mapper, 40, kPressedSlot, 10, action, frame, lr, native_slots, calls);
  if ((r3 & 0xFF) == 0) {
    return 0;
  }
  return S(L(list).move()) == 0 ? 1 : 0;
}

// The scroll kept around `selection` (the absolute item): the original's
// steps after a move (MenuList::Update's 0x8220C314 and 0x8220C420), with
// `count` the item count it compares against.
void KeepInView(uint32_t list, int32_t selection, int32_t count) {
  const int32_t shown = S(L(list).shown());
  const int32_t scroll = S(L(list).scroll());
  if (selection < scroll + shown) {
    if (scroll > selection) {
      L(list).set_scroll(uint32_t(selection));
    }
  } else {
    L(list).set_scroll(uint32_t(selection - shown + 1));
  }
  const int32_t last = count - shown;
  const int32_t now = S(L(list).scroll());
  if (now > last) {
    L(list).set_scroll(uint32_t(last));
  } else if (now < 0) {
    L(list).set_scroll(0);
  }
}

}  // namespace

void PagesInit(uint32_t pages, uint32_t page) {
  ITC_NATIVE_RUNNING("sub_8220BA98 ScreenPages::Init");
  P(pages).set_next_page(page);
  P(pages).set_page(page);
  P(pages).set_previous_page(page);
  P(pages).set_unknown_20(uint32_t(-1));
  P(pages).set_step(uint32_t(-1));
  P(pages).set_step_started(0);
}

void PagesGo(uint32_t pages, uint32_t page, uint32_t transition, uint32_t queued) {
  ITC_NATIVE_RUNNING("sub_8220BA68 ScreenPages::Go");
  if (S(transition) == -1) {
    P(pages).set_page(page);
    P(pages).set_step(uint32_t(-1));
    P(pages).set_queued_step(uint32_t(-1));
    return;
  }
  P(pages).set_step(transition);
  P(pages).set_queued_step(queued);
  P(pages).set_next_page(page);
  P(pages).set_page(uint32_t(-1));
}

void PagesFinish(uint32_t pages, uint32_t result) {
  ITC_NATIVE_RUNNING("sub_8220BA28 ScreenPages::Finish");
  if (S(result) == 0) {
    return;
  }
  const uint32_t queued = P(pages).queued_step();
  if (S(queued) == -1) {
    const uint32_t page = P(pages).next_page();
    P(pages).set_step(uint32_t(-1));
    P(pages).set_page(page);
  } else {
    P(pages).set_unknown_20(0);
    P(pages).set_step(queued);
    P(pages).set_queued_step(uint32_t(-1));
  }
  P(pages).set_step_started(0);
}

uint64_t MapperPressed(uint32_t mapper, uint32_t action, uint32_t frame,
                       uint32_t lr, bool native_slots, const Calls& calls) {
  return ReadSlot(mapper, 40, kPressedSlot, 10, action, frame, lr, native_slots, calls);
}

uint32_t ListSelection(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8220BC20 MenuList::Selection");
  return L(list).selection();
}

void ListSetSelection(uint32_t list, uint32_t selection) {
  ITC_NATIVE_RUNNING("sub_8220BC30 MenuList::SetSelection");
  const uint32_t old = L(list).selection();
  L(list).set_selection(selection);
  L(list).set_selection_before(old);
}

void ListSetEnabled(uint32_t list, uint32_t item, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8220BC00 MenuList::SetEnabled");
  guest::StoreU32(L(list).enabled().address + item * 4, value);
}

uint32_t ListIsEnabled(uint32_t list, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_8220BC10 MenuList::IsEnabled");
  return guest::LoadU32(L(list).enabled().address + item * 4);
}

void ListFit(uint32_t list, uint32_t quiet) {
  ITC_NATIVE_RUNNING("sub_8220BC40 MenuList::Fit");
  const int32_t count = S(L(list).count());
  const int32_t shown = S(L(list).shown());
  const int32_t old_selection = S(L(list).selection());
  int32_t scroll = S(L(list).scroll());
  const int32_t last = int32_t(uint32_t(count) - uint32_t(shown));
  int32_t item = int32_t(uint32_t(scroll) + uint32_t(old_selection));
  const int32_t room = last < 0 ? 0 : last;
  if (room < scroll) {
    scroll = room;
  }
  L(list).set_scroll(uint32_t(scroll));
  if (S(L(list).wraps()) != 0) {
    if (item < 0) {
      item = count;
    } else if (item > count) {
      item = 0;
    }
  } else {
    if (item < 0) {
      item = 0;
    } else if (item > count) {
      item = count;
    }
  }
  if (item >= int32_t(uint32_t(shown) + uint32_t(scroll))) {
    L(list).set_scroll(uint32_t(item) - uint32_t(shown) + 1);
  } else if (scroll > item) {
    L(list).set_scroll(uint32_t(item));
  }
  const int32_t now = S(L(list).scroll());
  if (now > last) {
    L(list).set_scroll(uint32_t(last));
  } else if (now < 0) {
    L(list).set_scroll(0);
  }
  const uint32_t top = L(list).scroll();
  const uint32_t selection = uint32_t(item) - top;
  L(list).set_selection(selection);
  if (S(quiet) != 0) {
    L(list).set_scroll_before(top);
    L(list).set_selection_before(selection);
    L(list).set_move(0);
    return;
  }
  L(list).set_selection_before(uint32_t(old_selection));
  const int32_t moved = int32_t(selection - uint32_t(old_selection));
  L(list).set_move(moved > 0 ? 1u : moved < 0 ? uint32_t(-1) : 0u);
}

uint32_t ListFired(uint32_t list, uint32_t action, uint32_t sp, bool native_slots,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C020 MenuList::Fired");
  const uint32_t frame = sp - 96;
  guest::StoreU32(frame, sp);
  const uint32_t entry = EntryOf(list);
  // A direction's two ids, the first tried with its own return address,
  // the second with the single call's.
  uint32_t first = 0, second = action, first_lr = 0;
  switch (S(action)) {
    case 154:
    case 168:
      first = 154, second = 168, first_lr = 0x8220C1B8;
      break;
    case 153:
    case 167:
      first = 153, second = 167, first_lr = 0x8220C178;
      break;
    case 155:
    case 169:
      first = 155, second = 169, first_lr = 0x8220C138;
      break;
    case 156:
    case 170:
      first = 156, second = 170, first_lr = 0x8220C0F8;
      break;
    default:
      break;
  }
  if (first_lr != 0 && FiredTry(entry, first, frame, first_lr, native_slots, calls)) {
    return 1;
  }
  return FiredTry(entry, second, frame, 0x8220C0B4, native_slots, calls) ? 1 : 0;
}

uint64_t ListUpdate(uint32_t list, uint32_t sp, bool native_slots,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C1E0 MenuList::Update");
  const uint32_t frame = sp - 144;
  guest::StoreU32(frame, sp);
  int32_t scroll = int32_t(L(list).count() - L(list).shown());
  if (scroll < 0) {
    scroll = 0;
  }
  const int32_t old_scroll = S(L(list).scroll());
  if (!(scroll < old_scroll)) {
    scroll = old_scroll;
  }
  int32_t item = int32_t(L(list).selection() + uint32_t(scroll));
  L(list).set_scroll(uint32_t(scroll));
  L(list).set_scroll_before(uint32_t(scroll));
  L(list).set_side(0);
  const int32_t from = item;
  L(list).set_wrapped(0);
  if (ListFired(list, L(list).left_action(), frame, native_slots, calls) != 0) {
    L(list).set_side(uint32_t(-1));
  } else if (ListFired(list, L(list).right_action(), frame, native_slots, calls) !=
             0) {
    L(list).set_side(1);
  } else if (ListFired(list, L(list).up_action(), frame, native_slots, calls) != 0) {
    // Up: the nearest enabled item above, else (wrapping) the last one
    // below the count down to it.
    bool found = false;
    const uint32_t block = L(list).enabled().address;
    for (int32_t k = item - 1; k >= 0; k--) {
      if (guest::LoadU32(block + uint32_t(k) * 4) != 0) {
        item = k;
        found = true;
        break;
      }
    }
    if (S(L(list).wraps()) != 0 && !found) {
      int32_t k = S(L(list).count());
      if (k > item) {
        const uint32_t wrap_block = L(list).enabled().address;
        for (; k > item; k--) {
          if (guest::LoadU32(wrap_block + uint32_t(k) * 4) != 0) {
            item = k;
            break;
          }
        }
      }
    }
    KeepInView(list, item, S(L(list).count()));
    if (item > from) {
      L(list).set_wrapped(1);
    }
  } else if (ListFired(list, L(list).down_action(), frame, native_slots, calls) !=
             0) {
    // Down: the nearest enabled item below, else (wrapping) the first one
    // from 0 up to it.
    const int32_t count = S(L(list).count());
    bool found = false;
    int32_t k = item + 1;
    if (k < count) {
      const uint32_t block = L(list).enabled().address;
      for (;;) {
        if (guest::LoadU32(block + uint32_t(k) * 4) != 0) {
          item = k;
          found = true;
          break;
        }
        k++;
        if (!(k < S(L(list).count()))) {
          break;
        }
      }
    }
    if (S(L(list).wraps()) != 0 && !found && item > 0) {
      const uint32_t block = L(list).enabled().address;
      for (int32_t j = 0; j < item; j++) {
        if (guest::LoadU32(block + uint32_t(j) * 4) != 0) {
          item = j;
          break;
        }
      }
    }
    KeepInView(list, item, count);
    if (item < from) {
      L(list).set_wrapped(1);
    }
  }
  const uint32_t old_selection = L(list).selection();
  const uint32_t top = L(list).scroll();
  L(list).set_move(0);
  const uint32_t selection = uint32_t(item) - top;
  L(list).set_selection_before(old_selection);
  const int32_t moved = int32_t(selection - old_selection);
  L(list).set_selection(selection);
  if (moved > 0) {
    L(list).set_move(1);
  } else if (moved < 0) {
    L(list).set_move(uint32_t(-1));
  }
  return ~uint64_t(0);
}

uint32_t ListAccepted(uint32_t list, uint32_t sp, bool native_slots,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BD50 MenuList::Accepted");
  return Pressed(list, L(list).accept_action(), sp, 0x8220BDAC, native_slots, calls);
}

uint32_t ListCancelled(uint32_t list, uint32_t sp, bool native_slots,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BDE0 MenuList::Cancelled");
  return Pressed(list, L(list).back_action(), sp, 0x8220BE3C, native_slots, calls);
}

uint32_t ListPressed152(uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BE70 MenuList::Pressed152");
  return Pressed(list, 152, sp, 0x8220BECC, native_slots, calls);
}

uint32_t ListPressed166(uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BF90 MenuList::Pressed166");
  return Pressed(list, 166, sp, 0x8220BFEC, native_slots, calls);
}

}  // namespace race_screens
