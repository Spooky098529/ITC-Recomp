// The location screen's opponent pages: an opponent's info and first text
// and its card (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "location_screen/internal.h"
#include "race_event/race_event.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/tuning_screen.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

using namespace detail;

namespace {

// An item's record in the save data (+0x199F8 + 12 id).
schema::CareerItemRecord Record(uint32_t save, int32_t id) {
  return schema::CareerItemRecord(schema::SaveData(save).item_records_address() + uint32_t(id) * 12);
}

// The picked opponent's item (RaceEvent::GroupItem of +804 and +800; +800
// the original's frame +80, read only here: a value since E5 step 6) copied
// to `to`.
void CopyOpponent(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t to) {
  const uint32_t picked = Screen().picked_opponent();
  const uint32_t event = guest::LoadU32(race_event::kEvent);
  const uint32_t item = race_event::GroupItem(event, Screen().group_kind(), picked);
  Copy(calls, frame, lr, to, item, 3008);
}

// An item's state by its record: 0 none, 1 seen (+4 or +0 set), 2 won (+2).
uint32_t ItemState(uint32_t save, int32_t id) {
  if (int16_t(Record(save, id).wins()) != 0) {
    return 2;
  }
  if (Record(save, id).outcome3() != 0 || Record(save, id).losses() != 0) {
    return 1;
  }
  return 0;
}

uint32_t SetTexture(uint32_t movie, uint32_t sub_scene, uint32_t child,
                    uint32_t texture) {
  return ui_movies::SetNodeTexture(ui_movies::FindNode(movie, sub_scene, child),
                                   texture);
}

// The card table's row (LocationScreenCard, 24 bytes at 0x82636578) for the
// location and the item's card category (+90).
schema::LocationScreenCard CardRow(uint32_t frame) {
  const int32_t category = int8_t(guest::LoadU8(frame + 186));
  const uint32_t location = Screen().location_index();
  return schema::LocationScreenCard((((location << 1) & 0xFFFFFFFEu) + uint32_t(category)) * 24 + G::cards_address());
}

}  // namespace

uint32_t ShowOpponentInfo(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B438 LocationScreen::ShowOpponentInfo");
  const uint32_t frame = sp - 6240;
  guest::StoreU32(frame, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const uint32_t movie = Movie(kMenusMovie);
  CopyOpponent(calls, frame, 0x8222B4A8, frame + 128);
  const int32_t category = int8_t(guest::LoadU8(frame + 162));
  const int32_t id = int16_t(guest::LoadU16(frame + 128));
  const uint32_t kind = guest::LoadU8(frame + 163);
  uint32_t level = uint32_t(category + 5299);
  const uint32_t name = uint32_t(id + 4650);
  // The chain's items: the original's words at the frame's +96 (five set
  // to -1), read only here (values since E5 step 6). The chain's steps
  // store one word each from +96 on; past the eight words below the item's
  // copy (+128) they go to the frame as the original's (never seen).
  uint32_t owned_items[8] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu};
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
  const int32_t card = int16_t(guest::LoadU16(frame + 216));
  uint32_t card_text = 478;
  if (card == 2) {
    card_text = 480;
  } else if (card == 3) {
    card_text = 481;
  } else if (card == 4 || card == 5) {
    card_text = 479;
  }
  SetText(calls, frame, movie, 12, 6, card_text);
  SetText(calls, frame, movie, 12, 4, level);
  SetText(calls, frame, movie, 12, 7, kind_text);
  SetText(calls, frame, movie, 12, 8, name);
  HideNode(movie, 0xFFFFFFFFu, 13);
  for (uint32_t i = 0; i < 3; i++) {
    HideNode(movie, 13, 5 + i);
  }
  bool chained = false;
  uint32_t k = 0;
  while (k < 3 && guest::LoadU16(frame + 220 + 2 * k) == 0xFFFF) {
    k++;
  }
  if (k < 3) {
    ShowNode(movie, 0xFFFFFFFFu, 13);
    if (card == 3) {
      Copy(calls, frame, 0x8222B644, frame + 3136, frame + 128, 3008);
      chained = true;
      uint32_t child = 5, slot = 0;
      for (;;) {
        uint32_t j = 0;
        while (j < 3 && guest::LoadU16(frame + 3228 + 2 * j) == 0xFFFF) {
          j++;
        }
        if (j == 3) {
          break;
        }
        const int32_t next = int16_t(guest::LoadU16(frame + 3228 + 2 * j));
        if (slot < 8) {
          owned_items[slot] = uint32_t(next);
        } else {
          guest::StoreU32(frame + 96 + 4 * slot, uint32_t(next));
        }
        ShowNode(movie, 13, child);
        SetText(calls, frame, movie, 13, child, uint32_t(next + 4650));
        Copy(calls, frame, 0x8222B764, frame + 3136, data_tables::Item(uint32_t(next)),
             3008);
        slot++;
        child++;
      }
    }
    for (uint32_t i = 0; i < 3; i++) {
      const int32_t text = int16_t(guest::LoadU16(frame + 220 + 2 * i));
      if (text != -1) {
        ShowNode(movie, 13, 5 + i);
        SetText(calls, frame, movie, 13, 5 + i, uint32_t(text + 4650));
      }
    }
  }
  const uint32_t state = ItemState(save, id);
  SetTexture(movie, 12, 17, state);
  if (chained && state == 2) {
    for (uint32_t i = 0; i < 5; i++) {
      const int32_t owned = int32_t(owned_items[i]);
      if (owned == -1) {
        break;
      }
      if (Record(save, owned).wins() == 0) {
        SetTexture(movie, 12, 17, 1);
        break;
      }
    }
  }
  const uint32_t node = ui_movies::FindNode(movie, 0xFFFFFFFFu, 12);
  const uint32_t name_node = ui_movies::Child(node, 14);
  const uint32_t level_node = ui_movies::Child(node, 15);
  uint32_t child;
  if (category == 48 || category == 49) {
    HideNode(movie, 12, 14);
    ShowNode(movie, 12, 15);
    child = ui_movies::Child(level_node, 0);
    schema::UiMovieNode(child).set_texture_entry(uint32_t(id - 310));
  } else {
    ShowNode(movie, 12, 14);
    HideNode(movie, 12, 15);
    child = ui_movies::Child(name_node, 0);
    schema::UiMovieNode(child).set_texture_entry(uint32_t(category));
  }
  return child;
}

uint32_t Transition12(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B8A8 LocationScreen::Transition12");
  const uint32_t frame = sp - 6192;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  CopyOpponent(calls, frame, 0x8222B90C, frame + 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  if (guest::LoadU32(step) == 0) {
    for (uint32_t i = 0; i < Screen().opponent_count(); i++) {
      FinishNamed(calls, frame, 0x8222B948, movie, 84 + i);
    }
    calls.Call(0x8211DE38, frame, 0x8222B96C, movie, 0xFFFFFFFFFFFFFFFFull, 13);
    HideNode(movie, 0xFFFFFFFFu, 17);
    HideNode(movie, 0xFFFFFFFFu, 18);
    ui_movies::Play(movie, 0xFFFFFFFFu, 9, 0, 0, 0, 0);
    const int32_t id = int16_t(guest::LoadU16(frame + 96));
    bool owned = false;
    uint32_t kind;
    if (race_event::ItemsAvailable(uint32_t(id), calls.EventFrom(frame)) == 0) {
      kind = 9;
    } else {
      const uint32_t state = ItemState(save, id);
      kind = 6 + state;
      owned = state == 2;
    }
    if (guest::LoadU16(frame + 184) == 3 && owned) {
      Copy(calls, frame, 0x8222BA60, frame + 3104, frame + 96, 3008);
      int32_t last = -1;
      for (;;) {
        bool found = false;
        uint32_t j = 0;
        while (j < 3 && guest::LoadU16(frame + 3196 + 2 * j) == 0xFFFF) {
          j++;
        }
        if (j < 3) {
          last = int16_t(guest::LoadU16(frame + 3196 + 2 * j));
          Copy(calls, frame, 0x8222BAB4, frame + 3104,
               data_tables::Item(uint32_t(last)), 3008);
          found = true;
        }
        if (Record(save, last).wins() == 0) {
          kind = 7;
          break;
        }
        if (!found) {
          break;
        }
      }
    }
    const int32_t index = data_tables::Find20(uint32_t(id), kind);
    const schema::LocationScreenEntry20 entry(data_tables::Entry20(Screen().entry_offset() + uint32_t(index)));
    SetText(calls, frame, movie, 5, 1,
            Screen().entry_offset() + uint32_t(index) + 5918);
    HideNode(movie, 5, 0);
    SetPictureTexture(movie, 6, 1, PictureA(entry.picture()));
    SetPictureTexture(movie, 6, 0, PictureB(entry.picture()));
    HidePictures(frame);
    guest::StoreU32(step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222BB7C, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    if (Required(ui_movies::HasMoreText(movie, 5, 1), "sub_8211D8D8") != 0) {
      ShowNode(movie, 5, 0);
    } else {
      HideNode(movie, 5, 0);
    }
    G::opponent_menu().set_up_down_locked(1);
  }
  return done;
}

uint64_t ShowCard(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222BBD0 LocationScreen::ShowCard");
  const uint32_t frame = sp - 3232;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kCardMovie);
  CopyOpponent(calls, frame, 0x8222BC2C, frame + 96);
  const schema::LocationScreenCard row = CardRow(frame);
  const uint32_t first = row.named_0(), second = row.named_1();
  const uint32_t has_card = HasCard(calls, frame, 0x8222BC70);
  for (uint32_t m = 0; m < 27; m++) {
    const uint32_t node = ui_movies::FindNode(movie, 2, m + 1);
    schema::UiMovieNode(node).set_visible(0);
    for (uint32_t j = 0; j < 4; j++) {
      schema::UiMovieNode(ui_movies::Child(node, j + 6)).set_visible(0);
    }
    FinishNamed(calls, frame, 0x8222BCC4, movie, m);
  }
  for (uint32_t m = 0; m < 27; m++) {
    schema::UiMovieNode(ui_movies::FindNode(movie, 3, m + 6)).set_visible(0);
  }
  SetText(calls, frame, movie, 3, 55, 402);
  SetText(calls, frame, movie, 3, 56, 401);
  SetText(calls, frame, movie, 3, 57, 407);
  SetText(calls, frame, movie, 3, 58, 408);
  if (has_card != 0) {
    ShowNode(movie, 0xFFFFFFFFu, 6);
    SetText(calls, frame, movie, 0xFFFFFFFFu, 12, row.text());
    ShowNode(movie, 2, first + 1);
    ShowNode(movie, 2, second + 1);
    ShowNode(movie, 3, first + 6);
    ShowNode(movie, 3, second + 6);
    PlayNamed(movie, row.named_0());
    LoopLast(frame, movie);
    PlayNamed(movie, row.named_1());
  } else {
    ShowNode(movie, 2, first + 1);
    ShowNode(movie, 3, first + 6);
    PlayNamed(movie, row.named_0());
  }
  LoopLast(frame, movie);
  const uint32_t node = ui_movies::FindNode(movie, 2, first + 1);
  schema::UiMovieNode(ui_movies::Child(node, row.child())).set_visible(1);
  uint64_t r3 = 0;
  for (uint32_t m = 0; m < 12; m++) {
    r3 = calls.Call(kFinishNamed, frame, 0x8222BE4C, movie, 39 + m);
  }
  return r3;
}

uint32_t Transition17(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222EAD0 LocationScreen::Transition17");
  const uint32_t frame = sp - 3152;
  guest::StoreU32(frame, sp);
  const uint32_t menus = Movie(kMenusMovie);
  const uint32_t card = Movie(kCardMovie);
  if (guest::LoadU32(step) == 0) {
    CopyOpponent(calls, frame, 0x8222EB60, frame + 96);
    const schema::LocationScreenCard row = CardRow(frame);
    ShowCard(frame, calls);
    PlayNamed(card, 52);
    PlayNamed(card, row.card_named());
    LoopLast(frame, card);
    guest::StoreU32(step, 1);
  }
  Run(calls, frame, 0x8222EBDC, menus);
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222EBE8, card)) == 0 ? 1 : 0;
  if (done != 0) {
    G::confirm_menu().set_up_down_locked(1);
  }
  return done;
}

}  // namespace location_screen
