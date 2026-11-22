// The location screen's small helpers and its lists' steps
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_event/race_event.h"
#include "schema/career.h"
#include "schema/location_screen.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

namespace {

using G = schema::LocationScreenGlobals;

schema::LocationScreen Screen(uint8_t* base) { return G::screen(base); }

// Whether an item copies natively onto the frame (memcpy's plain C++): below
// the physical views and not sharing memory with the source.
bool CanCopyItem(uint32_t destination, uint32_t item) {
  return destination < 0xA0000000u && !c_runtime::MayShare(destination, 3008, item, 3008);
}

// The 20-byte entry at +872 + `index` stepped: its next (+4) added to +872
// (1), or +872 and +876 cleared (0).
uint32_t StepEntry20(uint8_t* base, int32_t index) {
  const schema::LocationScreenEntry20 entry(
      base, data_tables::Entry20(base, Screen(base).entry_offset() + uint32_t(index)));
  const uint32_t next = entry.next();
  if (next != 0) {
    const schema::LocationScreen screen = Screen(base);
    screen.set_entry_offset(screen.entry_offset() + next);
    return 1;
  }
  Screen(base).set_entry_table(0);
  Screen(base).set_entry_offset(0);
  return 0;
}

}  // namespace

uint64_t SceneryConfig(uint64_t a, uint64_t b) {
  ITC_NATIVE_RUNNING("sub_8228F0A0 LocationScreen::SceneryConfig");
  return (a + 2) * 3 + b;
}

uint64_t KindText(uint32_t kind, uint64_t index) {
  ITC_NATIVE_RUNNING("sub_82227690 LocationScreen::KindText");
  switch (int32_t(kind)) {
    case 1:
      return index + 6718;
    case 2:
      return index + 7108;
    case 3:
      return index + 6722;
    case 4:
      return index + 7268;
    case 5:
      return index + 6887;
    case 6:
      return index + 7017;
    case 7:
      return index + 7411;
    default:
      return 0;
  }
}

uint32_t IsPictureQueued(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_822291E8 LocationScreen::IsPictureQueued");
  const schema::LocationScreen screen = Screen(base);
  const uint32_t ring = screen.queued_a_address();
  const uint32_t count = screen.queued_count_a();
  const uint32_t start = screen.queued_start_a();
  for (uint32_t i = 0; i < count; i++) {
    if (guest::LoadU32(base, ring + (((start + i) << 2) & 0x7C)) == id) {
      return 1;
    }
  }
  return 0;
}

namespace {

// The texture record of list A's or B's picture with id `id`, else the
// list's default texture.
uint32_t FindPicture(uint8_t* base, bool list_b, uint32_t id) {
  const schema::LocationScreen screen = Screen(base);
  const uint32_t count = list_b ? screen.picture_count_b() : screen.picture_count_a();
  const uint32_t list = list_b ? screen.pictures_b_address() : screen.pictures_a_address();
  for (uint32_t i = 0; i < count; i++) {
    const schema::LocationScreenPicture picture(base, guest::LoadU32(base, list + 4 * i));
    if (picture.id() == id) {
      return picture.texture().address;
    }
  }
  return list_b ? screen.default_texture_b() : screen.default_texture_a();
}

}  // namespace

uint32_t PictureA(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82229108 LocationScreen::PictureA");
  return FindPicture(base, false, id);
}

uint32_t PictureB(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_82229158 LocationScreen::PictureB");
  return FindPicture(base, true, id);
}

void OnMessage14(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82227158 LocationScreen::OnMessage14");
  Screen(base).set_queued_page(0);
  Screen(base).set_queued_transition(0xFFFFFFFFu);
  const int32_t cursor = int32_t(G::menu(base).cursor());
  uint32_t page;
  if (cursor == 3) {
    page = 8;
  } else if (cursor == 4) {
    page = 9;
  } else {
    return;
  }
  Screen(base).set_queued_page(page);
  Screen(base).set_queued_transition(1);
}

uint32_t HasMoreUnlocks(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822281D8 LocationScreen::HasMoreUnlocks");
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t count = schema::SaveData(base, save).unlocks_shown_count();
  return Screen(base).unlock_position() < count ? 1 : 0;
}

uint32_t NextUnlockEntry(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82227618 LocationScreen::NextUnlockEntry");
  vehicle_accessors::SaveDataOf(base, 0);
  const schema::LocationScreen screen = Screen(base);
  const schema::CareerEntry56 entry(
      base, data_tables::Entry56(base, screen.entry_table(), screen.entry_offset()));
  const uint32_t next = entry.next_page();
  if (next != 0) {
    const schema::LocationScreen at = Screen(base);
    at.set_entry_offset(next + at.entry_offset());
    return 1;
  }
  Screen(base).set_entry_table(0);
  Screen(base).set_entry_offset(0);
  return 0;
}

std::optional<uint32_t> OpponentHasCard(uint8_t* base, uint32_t sp) {
  const uint32_t frame = sp - kItemFrame;
  const schema::LocationScreen screen = Screen(base);
  const uint32_t event = guest::LoadU32(base, race_event::kEvent);
  const uint32_t item =
      race_event::GroupItem(base, event, screen.group_kind(), screen.picked_opponent());
  if (!CanCopyItem(frame + 96, item)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82227708 LocationScreen::OpponentHasCard");
  guest::StoreU32(base, frame + 80, screen.picked_opponent());
  c_runtime::Memcpy(base, frame + 96, item, 3008);
  const int32_t kind = int16_t(guest::LoadU16(base, frame + 184));
  return kind == 4 || kind == 5 ? 1 : 0;
}

std::optional<uint32_t> NextResultText(uint8_t* base, uint32_t sp) {
  const uint32_t frame = sp - kItemFrame;
  const uint32_t records = schema::RaceStatesGlobals::race_data(base).results_address();
  const uint32_t id =
      schema::RacerResultsRecord(base, vehicle_accessors::RacerRecordsAt(base, records, 0))
          .e1_item();
  const uint32_t item = data_tables::Item(base, id);
  if (!CanCopyItem(frame + 80, item)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_822271F8 LocationScreen::NextResultText");
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const schema::RacerResultsRecord human(
      base, vehicle_accessors::RacerRecordsAt(base, records, 0xFFFFFFFFu));
  c_runtime::Memcpy(base, frame + 80, item, 3008);
  const uint32_t owner = guest::LoadU16(base, frame + 168) == 3 ? human.e1_item() : id;
  uint32_t k = 12;
  if (int32_t(human.finish()) == 1) {
    const schema::CareerItemRecord record(
        base, schema::SaveData(base, save).item_records_address() + uint32_t(uint64_t(owner) * 12));
    k = int16_t(record.wins()) == 1 ? 10 : 11;
  }
  return StepEntry20(base, data_tables::Find20(base, id, k));
}

uint32_t Copy(uint8_t* base, const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
              uint32_t source, uint32_t size) {
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    calls.Call(kMemcpy, r1, lr, destination, source, size);
  }
  return destination;
}

uint32_t NextOpponentText(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82227430 LocationScreen::NextOpponentText");
  const uint32_t frame = sp - kNextOpponentTextFrame;
  guest::StoreU32(base, frame, sp);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  vehicle_accessors::RacerRecordsAt(
      base, schema::RaceStatesGlobals::race_data(base).results_address(), 0xFFFFFFFFu);
  guest::StoreU32(base, frame + 80, Screen(base).picked_opponent());
  const uint32_t event = guest::LoadU32(base, race_event::kEvent);
  const uint32_t item = race_event::GroupItem(base, event, Screen(base).group_kind(),
                                              guest::LoadU32(base, frame + 80));
  Copy(base, calls, frame, 0x82227498, frame + 96, item, 3008);
  const int32_t id = int16_t(guest::LoadU16(base, frame + 96));
  // An item's record in the save data (+0x199F8 + 12 id).
  auto record = [&](int32_t of) {
    return schema::CareerItemRecord(
        base, schema::SaveData(base, save).item_records_address() + uint32_t(of) * 12);
  };
  bool owned_kind3 = false;
  uint32_t kind;
  if (race_event::ItemsAvailable(base, uint32_t(id), calls.EventFrom(frame)) == 0) {
    kind = 9;
  } else {
    const bool won = int16_t(record(id).wins()) != 0;
    if (!won && record(id).outcome3() == 0 && record(id).losses() == 0) {
      kind = 6;
    } else if (!won) {
      kind = 7;
    } else {
      owned_kind3 = true;
      kind = 8;
    }
  }
  if (guest::LoadU16(base, frame + 184) == 3 && owned_kind3) {
    Copy(base, calls, frame, 0x82227550, frame + 3104, frame + 96, 3008);
    for (;;) {
      uint32_t i = 0;
      while (i < 3 && guest::LoadU16(base, frame + 3196 + 2 * i) == 0xFFFF) {
        i++;
      }
      if (i == 3) {
        break;
      }
      const int32_t next = int16_t(guest::LoadU16(base, frame + 3196 + 2 * i));
      Copy(base, calls, frame, 0x8222759C, frame + 3104, data_tables::Item(base, uint32_t(next)),
           3008);
      if (next == -1) {
        break;
      }
      if (record(next).wins() == 0) {
        kind = 7;
        break;
      }
    }
  }
  return StepEntry20(base, data_tables::Find20(base, uint32_t(id), kind));
}

}  // namespace location_screen
