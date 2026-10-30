// The location screen's opponent pages: an opponent's info and first text
// and its card (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "location_screen/internal.h"
#include "race_event/race_event.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

using namespace detail;

namespace {

// An item's record in the save data: save + kItemRecords + 12 id.
uint32_t Record(uint32_t save, int32_t id) { return uint32_t(id) * 12 + save + kItemRecords; }

// The picked opponent's item (RaceEvent::GroupItem of +804 and +800, +800
// stored at the frame's +80) copied to `to`.
void CopyOpponent(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr, uint32_t to) {
  guest::StoreU32(base, frame + 80, guest::LoadU32(base, Screen(base) + 800));
  const uint32_t event = guest::LoadU32(base, race_event::kEvent);
  const uint32_t item = race_event::GroupItem(base, event, guest::LoadU32(base, Screen(base) + 804),
                                              guest::LoadU32(base, frame + 80));
  Copy(base, calls, frame, lr, to, item, 3008);
}

// An item's state by its record: 0 none, 1 seen (+4 or +0 set), 2 won (+2).
uint32_t ItemState(uint8_t* base, uint32_t save, int32_t id) {
  if (int16_t(guest::LoadU16(base, Record(save, id) + 2)) != 0) {
    return 2;
  }
  if (guest::LoadU16(base, Record(save, id) + 4) != 0 ||
      guest::LoadU16(base, Record(save, id)) != 0) {
    return 1;
  }
  return 0;
}

uint32_t SetTexture(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                    uint32_t texture) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, sub_scene, child),
                                   texture);
}

// The card table (0x82636578): 24 bytes for each location and the item's
// card category (+90): +0 and +4 named animations, +8 a child, +16 a text,
// +20 movie 30's named animation.
constexpr uint32_t kCards = 0x82636578;
uint32_t CardRow(uint8_t* base, uint32_t frame) {
  const int32_t category = int8_t(guest::LoadU8(base, frame + 186));
  const uint32_t location = guest::LoadU32(base, Screen(base) + 852);
  return (((location << 1) & 0xFFFFFFFEu) + uint32_t(category)) * 24;
}

}  // namespace

uint32_t ShowOpponentInfo(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B438 LocationScreen::ShowOpponentInfo");
  const uint32_t frame = sp - 6240;
  guest::StoreU32(base, frame, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  CopyOpponent(base, calls, frame, 0x8222B4A8, frame + 128);
  const int32_t category = int8_t(guest::LoadU8(base, frame + 162));
  const int32_t id = int16_t(guest::LoadU16(base, frame + 128));
  const uint32_t kind = guest::LoadU8(base, frame + 163);
  uint32_t level = uint32_t(category + 5299);
  const uint32_t name = uint32_t(id + 4650);
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, frame + 96 + 4 * i, 0xFFFFFFFFu);
  }
  // The item's kind (+35, unsigned compares of the sign-extended byte).
  const uint32_t kind_word = uint32_t(int32_t(int8_t(uint8_t(kind))));
  uint32_t kind_text = 482;
  if (kind_word == 1) {
    level = 1711;
    kind_text = 484;
  } else if (kind_word == 2) {
    kind_text = 483;
  } else if (kind_word >= 3 && kind_word <= 5) {
    kind_text = 485;
  }
  const int32_t card = int16_t(guest::LoadU16(base, frame + 216));
  uint32_t card_text = 478;
  if (card == 2) {
    card_text = 480;
  } else if (card == 3) {
    card_text = 481;
  } else if (card == 4 || card == 5) {
    card_text = 479;
  }
  SetText(base, calls, frame, movie, 12, 6, card_text);
  SetText(base, calls, frame, movie, 12, 4, level);
  SetText(base, calls, frame, movie, 12, 7, kind_text);
  SetText(base, calls, frame, movie, 12, 8, name);
  HideNode(base, movie, 0xFFFFFFFFu, 13);
  for (uint32_t i = 0; i < 3; i++) {
    HideNode(base, movie, 13, 5 + i);
  }
  bool chained = false;
  uint32_t k = 0;
  while (k < 3 && guest::LoadU16(base, frame + 220 + 2 * k) == 0xFFFF) {
    k++;
  }
  if (k < 3) {
    ShowNode(base, movie, 0xFFFFFFFFu, 13);
    if (card == 3) {
      Copy(base, calls, frame, 0x8222B644, frame + 3136, frame + 128, 3008);
      chained = true;
      uint32_t child = 5, slot = frame + 96;
      for (;;) {
        uint32_t j = 0;
        while (j < 3 && guest::LoadU16(base, frame + 3228 + 2 * j) == 0xFFFF) {
          j++;
        }
        if (j == 3) {
          break;
        }
        const int32_t next = int16_t(guest::LoadU16(base, frame + 3228 + 2 * j));
        guest::StoreU32(base, slot, uint32_t(next));
        ShowNode(base, movie, 13, child);
        SetText(base, calls, frame, movie, 13, child, uint32_t(next + 4650));
        Copy(base, calls, frame, 0x8222B764, frame + 3136, data_tables::Item(base, uint32_t(next)),
             3008);
        slot += 4;
        child++;
      }
    }
    for (uint32_t i = 0; i < 3; i++) {
      const int32_t text = int16_t(guest::LoadU16(base, frame + 220 + 2 * i));
      if (text != -1) {
        ShowNode(base, movie, 13, 5 + i);
        SetText(base, calls, frame, movie, 13, 5 + i, uint32_t(text + 4650));
      }
    }
  }
  const uint32_t state = ItemState(base, save, id);
  SetTexture(base, movie, 12, 17, state);
  if (chained && state == 2) {
    for (uint32_t i = 0; i < 5; i++) {
      const int32_t owned = int32_t(guest::LoadU32(base, frame + 96 + 4 * i));
      if (owned == -1) {
        break;
      }
      if (guest::LoadU16(base, Record(save, owned) + 2) == 0) {
        SetTexture(base, movie, 12, 17, 1);
        break;
      }
    }
  }
  const uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 12);
  const uint32_t name_node = ui_movies::Child(base, node, 14);
  const uint32_t level_node = ui_movies::Child(base, node, 15);
  uint32_t child;
  if (category == 48 || category == 49) {
    HideNode(base, movie, 12, 14);
    ShowNode(base, movie, 12, 15);
    child = ui_movies::Child(base, level_node, 0);
    guest::StoreU32(base, child + 108, uint32_t(id - 310));
  } else {
    ShowNode(base, movie, 12, 14);
    HideNode(base, movie, 12, 15);
    child = ui_movies::Child(base, name_node, 0);
    guest::StoreU32(base, child + 108, uint32_t(category));
  }
  return child;
}

uint32_t Transition12(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B8A8 LocationScreen::Transition12");
  const uint32_t frame = sp - 6192;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  CopyOpponent(base, calls, frame, 0x8222B90C, frame + 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  if (guest::LoadU32(base, step) == 0) {
    for (uint32_t i = 0; i < guest::LoadU32(base, Screen(base) + 808); i++) {
      FinishNamed(calls, frame, 0x8222B948, movie, 84 + i);
    }
    calls.Call(0x8211DE38, frame, 0x8222B96C, movie, 0xFFFFFFFFFFFFFFFFull, 13);
    HideNode(base, movie, 0xFFFFFFFFu, 17);
    HideNode(base, movie, 0xFFFFFFFFu, 18);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 9, 0, 0, 0, 0);
    const int32_t id = int16_t(guest::LoadU16(base, frame + 96));
    bool owned = false;
    uint32_t kind;
    if (race_event::ItemsAvailable(base, uint32_t(id), calls.EventFrom(frame)) == 0) {
      kind = 9;
    } else {
      const uint32_t state = ItemState(base, save, id);
      kind = 6 + state;
      owned = state == 2;
    }
    if (guest::LoadU16(base, frame + 184) == 3 && owned) {
      Copy(base, calls, frame, 0x8222BA60, frame + 3104, frame + 96, 3008);
      int32_t last = -1;
      for (;;) {
        bool found = false;
        uint32_t j = 0;
        while (j < 3 && guest::LoadU16(base, frame + 3196 + 2 * j) == 0xFFFF) {
          j++;
        }
        if (j < 3) {
          last = int16_t(guest::LoadU16(base, frame + 3196 + 2 * j));
          Copy(base, calls, frame, 0x8222BAB4, frame + 3104,
               data_tables::Item(base, uint32_t(last)), 3008);
          found = true;
        }
        if (guest::LoadU16(base, Record(save, last) + 2) == 0) {
          kind = 7;
          break;
        }
        if (!found) {
          break;
        }
      }
    }
    const int32_t index = data_tables::Find20(base, uint32_t(id), kind);
    const uint32_t entry =
        data_tables::Entry20(base, guest::LoadU32(base, Screen(base) + 872) + uint32_t(index));
    SetText(base, calls, frame, movie, 5, 1,
            guest::LoadU32(base, Screen(base) + 872) + uint32_t(index) + 5918);
    HideNode(base, movie, 5, 0);
    SetPictureTexture(base, movie, 6, 1, PictureA(base, guest::LoadU32(base, entry + 16)));
    SetPictureTexture(base, movie, 6, 0, PictureB(base, guest::LoadU32(base, entry + 16)));
    HidePictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222BB7C, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    if (Required(ui_movies::HasMoreText(base, movie, 5, 1), "sub_8211D8D8") != 0) {
      ShowNode(base, movie, 5, 0);
    } else {
      HideNode(base, movie, 5, 0);
    }
    guest::StoreU32(base, guest::LoadU32(base, kOpponentMenu) + 24, 1);
  }
  return done;
}

uint64_t ShowCard(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222BBD0 LocationScreen::ShowCard");
  const uint32_t frame = sp - 3232;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kCardMovie);
  CopyOpponent(base, calls, frame, 0x8222BC2C, frame + 96);
  const uint32_t row = CardRow(base, frame) + kCards;
  const uint32_t first = guest::LoadU32(base, row), second = guest::LoadU32(base, row + 4);
  const uint32_t has_card = HasCard(base, calls, frame, 0x8222BC70);
  for (uint32_t m = 0; m < 27; m++) {
    const uint32_t node = ui_movies::FindNode(base, movie, 2, m + 1);
    guest::StoreU32(base, node + 80, 0);
    for (uint32_t j = 0; j < 4; j++) {
      guest::StoreU32(base, ui_movies::Child(base, node, j + 6) + 80, 0);
    }
    FinishNamed(calls, frame, 0x8222BCC4, movie, m);
  }
  for (uint32_t m = 0; m < 27; m++) {
    guest::StoreU32(base, ui_movies::FindNode(base, movie, 3, m + 6) + 80, 0);
  }
  SetText(base, calls, frame, movie, 3, 55, 402);
  SetText(base, calls, frame, movie, 3, 56, 401);
  SetText(base, calls, frame, movie, 3, 57, 407);
  SetText(base, calls, frame, movie, 3, 58, 408);
  if (has_card != 0) {
    ShowNode(base, movie, 0xFFFFFFFFu, 6);
    SetText(base, calls, frame, movie, 0xFFFFFFFFu, 12, guest::LoadU32(base, row + 16));
    ShowNode(base, movie, 2, first + 1);
    ShowNode(base, movie, 2, second + 1);
    ShowNode(base, movie, 3, first + 6);
    ShowNode(base, movie, 3, second + 6);
    PlayNamed(base, movie, guest::LoadU32(base, row));
    LoopLast(base, frame, movie);
    PlayNamed(base, movie, guest::LoadU32(base, row + 4));
  } else {
    ShowNode(base, movie, 2, first + 1);
    ShowNode(base, movie, 3, first + 6);
    PlayNamed(base, movie, guest::LoadU32(base, row));
  }
  LoopLast(base, frame, movie);
  const uint32_t node = ui_movies::FindNode(base, movie, 2, first + 1);
  guest::StoreU32(base, ui_movies::Child(base, node, guest::LoadU32(base, row + 8)) + 80, 1);
  uint64_t r3 = 0;
  for (uint32_t m = 0; m < 12; m++) {
    r3 = calls.Call(kFinishNamed, frame, 0x8222BE4C, movie, 39 + m);
  }
  return r3;
}

uint32_t Transition17(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222EAD0 LocationScreen::Transition17");
  const uint32_t frame = sp - 3152;
  guest::StoreU32(base, frame, sp);
  const uint32_t menus = Movie(base, frame, kMenusMovie);
  const uint32_t card = Movie(base, frame, kCardMovie);
  if (guest::LoadU32(base, step) == 0) {
    CopyOpponent(base, calls, frame, 0x8222EB60, frame + 96);
    const uint32_t row = CardRow(base, frame);
    ShowCard(base, frame, calls);
    PlayNamed(base, card, 52);
    PlayNamed(base, card, guest::LoadU32(base, row + kCards + 20));
    LoopLast(base, frame, card);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8222EBDC, menus);
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222EBE8, card)) == 0 ? 1 : 0;
  if (done != 0) {
    guest::StoreU32(base, guest::LoadU32(base, kConfirmMenu) + 24, 1);
  }
  return done;
}

}  // namespace location_screen
