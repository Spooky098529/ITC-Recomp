// The page object and the menu list (docs/functions/race-screens.md): see
// race_screens.h.

#include "race_screens/race_screens.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"

namespace race_screens {

namespace {

int32_t S(uint32_t value) { return int32_t(value); }

uint32_t Word(uint8_t* base, uint32_t list, uint32_t field) {
  return guest::LoadU32(base, list + field);
}

void SetWord(uint8_t* base, uint32_t list, uint32_t field, uint32_t value) {
  guest::StoreU32(base, list + field, value);
}

// The list's player's entry in the action mappers' table.
uint32_t EntryOf(uint8_t* base, uint32_t list) {
  const uint32_t index =
      guest::LoadU32(base, Word(base, list, kListPlayer) * 4 + kControllerEntries);
  return index * 8 + kMapperEntries;
}

// A mapper's read slot (`offset` in its vtable) on `action`, as the
// original's bctrl from `frame` returning to `lr`: the slot's plain C++ when
// it's `native` and `native_slots`, logged for --log_pause_menu as its
// adapter does, else the slot through `calls`.
uint64_t ReadSlot(uint8_t* base, uint32_t mapper, uint32_t offset, uint32_t native, int slot,
                  uint32_t action, uint32_t frame, uint32_t lr, bool native_slots,
                  const Calls& calls) {
  const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, mapper) + offset);
  if (native_slots && function == native) {
    const uint32_t r3 = native == kPressedSlot ? action_mappers::Pressed(base, mapper, action)
                                               : action_mappers::Fired(base, mapper, action);
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, action, lr, double(r3));
    return r3;
  }
  return calls.Call(function, frame, lr, mapper, action);
}

// One Fired try on the entry: 0 when its +4 is 1, else the slot's answer
// (its low byte).
bool FiredTry(uint8_t* base, uint32_t entry, uint32_t action, uint32_t frame, uint32_t lr,
              bool native_slots, const Calls& calls) {
  if (S(guest::LoadU32(base, entry + 4)) == 1) {
    return false;
  }
  const uint32_t mapper = guest::LoadU32(base, entry);
  return (ReadSlot(base, mapper, 48, kFiredSlot, 12, action, frame, lr, native_slots, calls) &
          0xFF) != 0;
}

uint32_t Pressed(uint8_t* base, uint32_t list, uint32_t action, uint32_t sp, uint32_t lr,
                 bool native_slots, const Calls& calls) {
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const uint32_t entry = EntryOf(base, list);
  if (S(guest::LoadU32(base, entry + 4)) == 1) {
    return 0;
  }
  const uint32_t mapper = guest::LoadU32(base, entry);
  const uint64_t r3 =
      ReadSlot(base, mapper, 40, kPressedSlot, 10, action, frame, lr, native_slots, calls);
  if ((r3 & 0xFF) == 0) {
    return 0;
  }
  return S(Word(base, list, kListMove)) == 0 ? 1 : 0;
}

// The scroll kept around `selection` (the absolute item): the original's
// steps after a move (MenuList::Update's 0x8220C314 and 0x8220C420), with
// `count` the item count it compares against.
void KeepInView(uint8_t* base, uint32_t list, int32_t selection, int32_t count) {
  const int32_t shown = S(Word(base, list, kListShown));
  const int32_t scroll = S(Word(base, list, kListScroll));
  if (selection < scroll + shown) {
    if (scroll > selection) {
      SetWord(base, list, kListScroll, uint32_t(selection));
    }
  } else {
    SetWord(base, list, kListScroll, uint32_t(selection - shown + 1));
  }
  const int32_t last = count - shown;
  const int32_t now = S(Word(base, list, kListScroll));
  if (now > last) {
    SetWord(base, list, kListScroll, uint32_t(last));
  } else if (now < 0) {
    SetWord(base, list, kListScroll, 0);
  }
}

}  // namespace

void PagesInit(uint8_t* base, uint32_t pages, uint32_t page) {
  ITC_NATIVE_RUNNING("sub_8220BA98 ScreenPages::Init");
  guest::StoreU32(base, pages + 4, page);
  guest::StoreU32(base, pages + 0, page);
  guest::StoreU32(base, pages + 8, page);
  guest::StoreU32(base, pages + 20, uint32_t(-1));
  guest::StoreU32(base, pages + 12, uint32_t(-1));
  guest::StoreU32(base, pages + 24, 0);
}

void PagesGo(uint8_t* base, uint32_t pages, uint32_t page, uint32_t transition, uint32_t queued) {
  ITC_NATIVE_RUNNING("sub_8220BA68 ScreenPages::Go");
  if (S(transition) == -1) {
    guest::StoreU32(base, pages + 0, page);
    guest::StoreU32(base, pages + 12, uint32_t(-1));
    guest::StoreU32(base, pages + 16, uint32_t(-1));
    return;
  }
  guest::StoreU32(base, pages + 12, transition);
  guest::StoreU32(base, pages + 16, queued);
  guest::StoreU32(base, pages + 4, page);
  guest::StoreU32(base, pages + 0, uint32_t(-1));
}

void PagesFinish(uint8_t* base, uint32_t pages, uint32_t result) {
  ITC_NATIVE_RUNNING("sub_8220BA28 ScreenPages::Finish");
  if (S(result) == 0) {
    return;
  }
  const uint32_t queued = guest::LoadU32(base, pages + 16);
  if (S(queued) == -1) {
    const uint32_t page = guest::LoadU32(base, pages + 4);
    guest::StoreU32(base, pages + 12, uint32_t(-1));
    guest::StoreU32(base, pages + 0, page);
  } else {
    guest::StoreU32(base, pages + 20, 0);
    guest::StoreU32(base, pages + 12, queued);
    guest::StoreU32(base, pages + 16, uint32_t(-1));
  }
  guest::StoreU32(base, pages + 24, 0);
}

uint64_t MapperPressed(uint8_t* base, uint32_t mapper, uint32_t action, uint32_t frame,
                       uint32_t lr, bool native_slots, const Calls& calls) {
  return ReadSlot(base, mapper, 40, kPressedSlot, 10, action, frame, lr, native_slots, calls);
}

uint32_t ListSelection(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8220BC20 MenuList::Selection");
  return Word(base, list, kListSelection);
}

void ListSetSelection(uint8_t* base, uint32_t list, uint32_t selection) {
  ITC_NATIVE_RUNNING("sub_8220BC30 MenuList::SetSelection");
  const uint32_t old = Word(base, list, kListSelection);
  SetWord(base, list, kListSelection, selection);
  SetWord(base, list, kListSelectionBefore, old);
}

void ListSetEnabled(uint8_t* base, uint32_t list, uint32_t item, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_8220BC00 MenuList::SetEnabled");
  guest::StoreU32(base, Word(base, list, kListBlock) + item * 4, value);
}

uint32_t ListIsEnabled(uint8_t* base, uint32_t list, uint32_t item) {
  ITC_NATIVE_RUNNING("sub_8220BC10 MenuList::IsEnabled");
  return guest::LoadU32(base, Word(base, list, kListBlock) + item * 4);
}

void ListFit(uint8_t* base, uint32_t list, uint32_t quiet) {
  ITC_NATIVE_RUNNING("sub_8220BC40 MenuList::Fit");
  const int32_t count = S(Word(base, list, kListCount));
  const int32_t shown = S(Word(base, list, kListShown));
  const int32_t old_selection = S(Word(base, list, kListSelection));
  int32_t scroll = S(Word(base, list, kListScroll));
  const int32_t last = int32_t(uint32_t(count) - uint32_t(shown));
  int32_t item = int32_t(uint32_t(scroll) + uint32_t(old_selection));
  const int32_t room = last < 0 ? 0 : last;
  if (room < scroll) {
    scroll = room;
  }
  SetWord(base, list, kListScroll, uint32_t(scroll));
  if (S(Word(base, list, kListWraps)) != 0) {
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
    SetWord(base, list, kListScroll, uint32_t(item) - uint32_t(shown) + 1);
  } else if (scroll > item) {
    SetWord(base, list, kListScroll, uint32_t(item));
  }
  const int32_t now = S(Word(base, list, kListScroll));
  if (now > last) {
    SetWord(base, list, kListScroll, uint32_t(last));
  } else if (now < 0) {
    SetWord(base, list, kListScroll, 0);
  }
  const uint32_t top = Word(base, list, kListScroll);
  const uint32_t selection = uint32_t(item) - top;
  SetWord(base, list, kListSelection, selection);
  if (S(quiet) != 0) {
    SetWord(base, list, kListScrollBefore, top);
    SetWord(base, list, kListSelectionBefore, selection);
    SetWord(base, list, kListMove, 0);
    return;
  }
  SetWord(base, list, kListSelectionBefore, uint32_t(old_selection));
  const int32_t moved = int32_t(selection - uint32_t(old_selection));
  SetWord(base, list, kListMove, moved > 0 ? 1u : moved < 0 ? uint32_t(-1) : 0u);
}

uint32_t ListFired(uint8_t* base, uint32_t list, uint32_t action, uint32_t sp, bool native_slots,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C020 MenuList::Fired");
  const uint32_t frame = sp - 96;
  guest::StoreU32(base, frame, sp);
  const uint32_t entry = EntryOf(base, list);
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
  if (first_lr != 0 && FiredTry(base, entry, first, frame, first_lr, native_slots, calls)) {
    return 1;
  }
  return FiredTry(base, entry, second, frame, 0x8220C0B4, native_slots, calls) ? 1 : 0;
}

uint64_t ListUpdate(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220C1E0 MenuList::Update");
  const uint32_t frame = sp - 144;
  guest::StoreU32(base, frame, sp);
  int32_t scroll = int32_t(Word(base, list, kListCount) - Word(base, list, kListShown));
  if (scroll < 0) {
    scroll = 0;
  }
  const int32_t old_scroll = S(Word(base, list, kListScroll));
  if (!(scroll < old_scroll)) {
    scroll = old_scroll;
  }
  int32_t item = int32_t(Word(base, list, kListSelection) + uint32_t(scroll));
  SetWord(base, list, kListScroll, uint32_t(scroll));
  SetWord(base, list, kListScrollBefore, uint32_t(scroll));
  SetWord(base, list, kListSide, 0);
  const int32_t from = item;
  SetWord(base, list, kListWrapped, 0);
  if (ListFired(base, list, Word(base, list, kListLeft), frame, native_slots, calls) != 0) {
    SetWord(base, list, kListSide, uint32_t(-1));
  } else if (ListFired(base, list, Word(base, list, kListRight), frame, native_slots, calls) !=
             0) {
    SetWord(base, list, kListSide, 1);
  } else if (ListFired(base, list, Word(base, list, kListUp), frame, native_slots, calls) != 0) {
    // Up: the nearest enabled item above, else (wrapping) the last one
    // below the count down to it.
    bool found = false;
    const uint32_t block = Word(base, list, kListBlock);
    for (int32_t k = item - 1; k >= 0; k--) {
      if (guest::LoadU32(base, block + uint32_t(k) * 4) != 0) {
        item = k;
        found = true;
        break;
      }
    }
    if (S(Word(base, list, kListWraps)) != 0 && !found) {
      int32_t k = S(Word(base, list, kListCount));
      if (k > item) {
        const uint32_t wrap_block = Word(base, list, kListBlock);
        for (; k > item; k--) {
          if (guest::LoadU32(base, wrap_block + uint32_t(k) * 4) != 0) {
            item = k;
            break;
          }
        }
      }
    }
    KeepInView(base, list, item, S(Word(base, list, kListCount)));
    if (item > from) {
      SetWord(base, list, kListWrapped, 1);
    }
  } else if (ListFired(base, list, Word(base, list, kListDown), frame, native_slots, calls) !=
             0) {
    // Down: the nearest enabled item below, else (wrapping) the first one
    // from 0 up to it.
    const int32_t count = S(Word(base, list, kListCount));
    bool found = false;
    int32_t k = item + 1;
    if (k < count) {
      const uint32_t block = Word(base, list, kListBlock);
      for (;;) {
        if (guest::LoadU32(base, block + uint32_t(k) * 4) != 0) {
          item = k;
          found = true;
          break;
        }
        k++;
        if (!(k < S(Word(base, list, kListCount)))) {
          break;
        }
      }
    }
    if (S(Word(base, list, kListWraps)) != 0 && !found && item > 0) {
      const uint32_t block = Word(base, list, kListBlock);
      for (int32_t j = 0; j < item; j++) {
        if (guest::LoadU32(base, block + uint32_t(j) * 4) != 0) {
          item = j;
          break;
        }
      }
    }
    KeepInView(base, list, item, count);
    if (item < from) {
      SetWord(base, list, kListWrapped, 1);
    }
  }
  const uint32_t old_selection = Word(base, list, kListSelection);
  const uint32_t top = Word(base, list, kListScroll);
  SetWord(base, list, kListMove, 0);
  const uint32_t selection = uint32_t(item) - top;
  SetWord(base, list, kListSelectionBefore, old_selection);
  const int32_t moved = int32_t(selection - old_selection);
  SetWord(base, list, kListSelection, selection);
  if (moved > 0) {
    SetWord(base, list, kListMove, 1);
  } else if (moved < 0) {
    SetWord(base, list, kListMove, uint32_t(-1));
  }
  return ~uint64_t(0);
}

uint32_t ListAccepted(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BD50 MenuList::Accepted");
  return Pressed(base, list, Word(base, list, kListAccept), sp, 0x8220BDAC, native_slots, calls);
}

uint32_t ListCancelled(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BDE0 MenuList::Cancelled");
  return Pressed(base, list, Word(base, list, kListBack), sp, 0x8220BE3C, native_slots, calls);
}

uint32_t ListPressed152(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BE70 MenuList::Pressed152");
  return Pressed(base, list, 152, sp, 0x8220BECC, native_slots, calls);
}

uint32_t ListPressed166(uint8_t* base, uint32_t list, uint32_t sp, bool native_slots,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220BF90 MenuList::Pressed166");
  return Pressed(base, list, 166, sp, 0x8220BFEC, native_slots, calls);
}

}  // namespace race_screens
