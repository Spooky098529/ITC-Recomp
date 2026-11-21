// The location screen's result and list texts after a race, the location
// list's entry picked, the opponents' pictures and the course view's lights
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "camera_modes/camera_modes.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "location_screen/internal.h"
#include "race_event/race_event.h"
#include "schema/career.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_states.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace location_screen {

using namespace detail;

namespace {

// An item's record in the save data (+0x199F8 + 12 id).
schema::CareerItemRecord Record(uint8_t* base, uint32_t save, int32_t id) {
  return schema::CareerItemRecord(
      base, schema::SaveData(base, save).item_records_address() + uint32_t(id) * 12);
}

// Transition23 and Transition33 (with and without the fade): `lrs` the
// return addresses of memcpy, FinishNamed and Run.
struct ResultLrs {
  uint32_t copy, finish, run;
};
uint32_t ResultText(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls, bool fade,
                    const ResultLrs& lrs) {
  const uint32_t frame = sp - 3168;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    const uint32_t records = schema::RaceStatesGlobals::race_data(base).results_address();
    const schema::RacerResultsRecord human(
        base, vehicle_accessors::RacerRecordsAt(base, records, 0xFFFFFFFFu));
    const uint32_t id =
        schema::RacerResultsRecord(base, vehicle_accessors::RacerRecordsAt(base, records, 0))
            .e1_item();
    Copy(base, calls, frame, lrs.copy, frame + 96, data_tables::Item(base, id), 3008);
    const uint32_t owner = guest::LoadU16(base, frame + 184) == 3 ? human.e1_item() : id;
    uint32_t k = 12;
    if (int32_t(human.finish()) == 1) {
      if (Record(base, save, int32_t(owner)).wins() != 1) {
        k = 11;
      } else {
        k = 10;
        const int32_t kind = int8_t(guest::LoadU8(base, frame + 131));
        if (kind == 3 || kind == 4 || kind == 5) {
          Screen(base).set_won_card_item(1);
        }
      }
    }
    const int32_t found = data_tables::Find20(base, id, k);
    const schema::LocationScreenEntry20 entry(
        base, data_tables::Entry20(base, Screen(base).entry_offset() + uint32_t(found)));
    const uint32_t result = Screen(base).entry_offset() + uint32_t(found) + 5918;
    const uint32_t text = entry.text() + 4650;
    SetText(base, calls, frame, movie, 4, 2, result);
    SetText(base, calls, frame, movie, 4, 1, text);
    HideNode(base, movie, 4, 0);
    SetPictureTexture(base, movie, 6, 1, PictureA(base, entry.picture()));
    SetPictureTexture(base, movie, 6, 0, PictureB(base, entry.picture()));
    FinishNamed(calls, frame, lrs.finish, movie, 5);
    PlayNamed(base, movie, 5);
    LoopLast(base, frame, movie);
    if (fade) {
      mode_tick::ScreenFadesFadeIn(base, 60, 0, 0, 0, 0);
    }
    PlayNamed(base, movie, 95);
    HidePictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  if (fade && Fading(base)) {
    return 0;
  }
  const uint32_t done = uint32_t(Run(calls, frame, lrs.run, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    if (Required(ui_movies::HasMoreText(base, movie, 4, 2), "sub_8211D8D8") != 0) {
      ShowNode(base, movie, 4, 0);
    } else {
      HideNode(base, movie, 4, 0);
    }
  }
  return done;
}

uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(guest::LoadU32(base, address)) << 32 | guest::LoadU32(base, address + 4);
}

void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

}  // namespace

uint32_t Transition23(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222A470 LocationScreen::Transition23");
  return ResultText(base, sp, step, calls, true, {0x8222A4FC, 0x8222A630, 0x8222A6CC});
}

uint32_t Transition33(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222A718 LocationScreen::Transition33");
  return ResultText(base, sp, step, calls, false, {0x8222A7A4, 0x8222A8D8, 0x8222A92C});
}

uint32_t Transition7(uint8_t* base, uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222AE00 LocationScreen::Transition7");
  const uint32_t frame = sp - 288;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  if (guest::LoadU32(base, step) == 0) {
    const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
    constexpr uint32_t kLocations = 0x1AD20;
    Copy(base, calls, frame, 0x8222AE74, frame + 96,
         Screen(base).location_index() * 100 + save + kLocations, 100);
    const uint32_t entry = G::list_menu(base).cursor() * 12 + frame + 96;
    const uint32_t index = guest::LoadU32(base, entry + 8);
    const uint32_t table = guest::LoadU32(base, entry + 4);
    const schema::CareerEntry56 e56(base, data_tables::Entry56(base, table, index));
    const uint32_t location = Screen(base).location_index();
    guest::StoreU32(base, entry, 1);
    const uint32_t cursor = G::list_menu(base).cursor();
    guest::StoreU32(base, location * 100 + cursor * 12 + save + kLocations, 1);
    const uint32_t flags = ((table * 192 + index + 27614) << 2) + save;
    bool marked = false;
    if (int32_t(guest::LoadU32(base, flags)) != 2) {
      guest::StoreU32(base, flags, 2);
      marked = true;
    }
    if (table - 2 <= 5) {
      guest::StoreU16(base, save + 6076, 0);
    }
    if (marked) {
      career::ItemFlagsSet(base, save, e56.unknown_48(), e56.unknown_52());
    }
    const uint32_t text = uint32_t(KindText(table, index));
    FinishNamed(calls, frame, 0x8222AF4C, movie,
                G::list_menu(base).cursor() + 52);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 8, 0, 0, 0, 0);
    SetText(base, calls, frame, movie, 4, 1, e56.text() + 4650);
    SetText(base, calls, frame, movie, 4, 2, text);
    HideNode(base, movie, 4, 0);
    FinishNamed(calls, frame, 0x8222AFB4, movie, 5);
    PlayNamed(base, movie, 5);
    LoopLast(base, frame, movie);
    HidePictures(base, frame);
    guest::StoreU32(base, step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222AFEC, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    HideNode(base, movie, 0xFFFFFFFFu, G::list_menu(base).cursor() + 19);
    if (Required(ui_movies::HasMoreText(base, movie, 4, 2), "sub_8211D8D8") != 0) {
      ShowNode(base, movie, 4, 0);
    }
  }
  return done;
}

uint32_t ShowOpponentPictures(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222B200 LocationScreen::ShowOpponentPictures");
  const uint32_t frame = sp - 6224;
  guest::StoreU32(base, frame, sp);
  const uint32_t movie = Movie(base, frame, kMenusMovie);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  constexpr uint32_t kChildren[7] = {7, 13, 19, 25, 31, 37, 0};
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU32(base, frame + 96 + 4 * i, kChildren[i]);
  }
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 7; i++) {
    r3 = HideNode(base, movie, 14, i);
  }
  for (uint32_t i = 0; i < Screen(base).opponent_count(); i++) {
    guest::StoreU32(base, frame + 80, i);
    const uint32_t event = guest::LoadU32(base, race_event::kEvent);
    const uint32_t item = race_event::GroupItem(
        base, event, Screen(base).group_kind(), guest::LoadU32(base, frame + 80));
    Copy(base, calls, frame, 0x8222B2E4, frame + 128, item, 3008);
    ShowNode(base, movie, 14, i);
    const int32_t id = int16_t(guest::LoadU16(base, frame + 128));
    const bool won = int16_t(Record(base, save, id).wins()) != 0;
    bool owned = false;
    uint32_t texture;
    if (!won && Record(base, save, id).outcome3() == 0 && Record(base, save, id).losses() == 0) {
      texture = 0;
    } else if (!won) {
      texture = 1;
    } else {
      texture = 2;
      owned = true;
    }
    const uint32_t child = guest::LoadU32(base, frame + 96 + 4 * i);
    r3 = ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 14, child), texture);
    if (guest::LoadU16(base, frame + 216) != 3 || !owned) {
      continue;
    }
    r3 = Copy(base, calls, frame, 0x8222B39C, frame + 3136, frame + 128, 3008);
    for (;;) {
      uint32_t k = 0;
      while (k < 3 && guest::LoadU16(base, frame + 3228 + 2 * k) == 0xFFFF) {
        k++;
      }
      if (k == 3) {
        break;
      }
      const int32_t next = int16_t(guest::LoadU16(base, frame + 3228 + 2 * k));
      r3 = Copy(base, calls, frame, 0x8222B3E8, frame + 3136,
                data_tables::Item(base, uint32_t(next)), 3008);
      if (next == -1) {
        break;
      }
      if (Record(base, save, next).wins() == 0) {
        r3 = ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 14, child), 1);
        break;
      }
    }
  }
  return r3;
}

uint64_t UpdateView(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82227E20 LocationScreen::UpdateView");
  const uint32_t frame = sp - 304;
  guest::StoreU32(base, frame, sp);
  // The matrix's w words and two vectors of the float at 0x82000A30.
  const uint32_t one = ThroughFloat(base, 0x82000A30);
  for (uint32_t offset : {220u, 236u, 252u, 268u, 112u, 116u, 120u, 124u, 160u, 164u, 168u, 172u}) {
    guest::StoreU32(base, frame + offset, one);
  }
  // The third camera animation's object, by the opponent menu's cursor.
  const uint32_t cursor = G::opponent_menu(base).cursor();
  const uint32_t time =
      guest::LoadU32(base, ((cursor << 2) & 0xFFFFFFFCu) + G::opponent_times_address());
  const uint32_t location = Screen(base).location_index();
  const uint32_t id =
      schema::LocationScreenLocation(base, ((location << 4) & 0xFFFFFFF0u) + G::locations_address())
          .list_anim();
  const uint32_t out = frame + 208;
  if ((frame & 0xF) != 0) {
    calls.Call(0x82150A10, frame, 0x82227EA8, out, id, time);
  } else {
    camera_modes::AnimsObjectMatrix(
        base, out, id, time, frame - 96 - 320, [&](uint32_t to, uint32_t from, uint32_t size) {
          calls.Call(kMemcpy, frame - 96 - 320, 0x82150600, to, from, size);
        });
  }
  // The lights' three arrays of four vectors, each moved on by one, the
  // original's loads and stores in their order.
  constexpr uint32_t kA = G::light_array_a_address(), kB = G::light_array_b_address(),
                     kC = G::light_array_c_address(), kED = G::light_view_address();
  constexpr uint32_t k6A0 = G::light_source_a_address(), k6B0 = G::light_source_b_address(),
                     k6C0 = G::light_source_c_address();
  uint64_t w0 = Load64(base, k6B0), w1 = Load64(base, k6B0 + 8);
  Store64(base, kC + 0, w0);
  Store64(base, kC + 8, w1);
  uint64_t c = Load64(base, kC + 0);
  uint64_t p0 = Load64(base, k6B0), p1 = Load64(base, k6B0 + 8);
  Store64(base, kC + 16, c);
  c = Load64(base, kC + 8);
  Store64(base, kB + 0, p0);
  Store64(base, kB + 8, p1);
  uint64_t b = Load64(base, kB + 0);
  uint64_t q0 = Load64(base, k6A0), q1 = Load64(base, k6A0 + 8);
  Store64(base, kC + 24, c);
  c = Load64(base, kC + 16);
  Store64(base, kB + 16, b);
  b = Load64(base, kB + 8);
  Store64(base, kED + 0, q0);
  Store64(base, kC + 32, c);
  c = Load64(base, kC + 24);
  Store64(base, kED + 8, q1);
  Store64(base, kB + 24, b);
  b = Load64(base, kB + 16);
  Store64(base, kC + 40, c);
  const uint32_t zero = ThroughFloat(base, 0x82000A34);
  guest::StoreU32(base, frame + 80, zero);
  guest::StoreU32(base, frame + 84, zero);
  w0 = Load64(base, k6B0);
  w1 = Load64(base, k6B0 + 8);
  Store64(base, kB + 32, b);
  b = Load64(base, kB + 24);
  guest::StoreU32(base, frame + 88, zero);
  Store64(base, kA + 0, w0);
  Store64(base, kA + 8, w1);
  uint64_t a = Load64(base, kA + 0);
  Store64(base, kB + 40, b);
  for (uint32_t offset : {92u, 96u}) {
    guest::StoreU32(base, frame + offset, zero);
  }
  Store64(base, kA + 16, a);
  guest::StoreU32(base, frame + 100, zero);
  a = Load64(base, kA + 8);
  guest::StoreU32(base, frame + 104, zero);
  const uint64_t f0 = Load64(base, frame + 112);
  guest::StoreU32(base, frame + 108, zero);
  const uint64_t f1 = Load64(base, frame + 120);
  const uint64_t r0 = Load64(base, k6C0), r1 = Load64(base, k6C0 + 8);
  Store64(base, kA + 24, a);
  a = Load64(base, kA + 16);
  Store64(base, kB + 0, f0);
  Store64(base, kB + 8, f1);
  const uint64_t g0 = Load64(base, frame + 160), g1 = Load64(base, frame + 168);
  Store64(base, kA + 32, a);
  a = Load64(base, kA + 24);
  Store64(base, kC + 0, g0);
  Store64(base, kC + 8, g1);
  Store64(base, kA + 40, a);
  Store64(base, kA + 0, r0);
  Store64(base, kA + 8, r1);
  // 128 bytes each at 0x8296BAE0 and 0x8296BB60 from the frame's +80 and
  // +96 (16 bytes each, repeated).
  for (uint32_t i = 0; i < 128; i += 16) {
    const uint64_t x0 = Load64(base, frame + 80), y0 = Load64(base, frame + 96);
    const uint64_t x1 = Load64(base, frame + 88), y1 = Load64(base, frame + 104);
    Store64(base, G::light_zeros_a_address() + i, x0);
    Store64(base, G::light_zeros_b_address() + i, y0);
    Store64(base, G::light_zeros_a_address() + i + 8, x1);
    Store64(base, G::light_zeros_b_address() + i + 8, y1);
  }
  // r3 the third array's address as the original made it (li, addi: sign-extended).
  return uint64_t(int64_t(int32_t(kC)));
}

}  // namespace location_screen
