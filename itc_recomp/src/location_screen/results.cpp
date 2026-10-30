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
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace location_screen {

using namespace detail;

namespace {

// An item's record in the save data: save + kItemRecords + 12 id.
uint32_t Record(uint32_t save, int32_t id) { return uint32_t(id) * 12 + save + kItemRecords; }

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
    const uint32_t records = guest::LoadU32(base, kRaceData) + kRacerRecords;
    const uint32_t human = vehicle_accessors::RacerRecordsAt(base, records, 0xFFFFFFFFu);
    const uint32_t id =
        guest::LoadU32(base, vehicle_accessors::RacerRecordsAt(base, records, 0) + 980);
    Copy(base, calls, frame, lrs.copy, frame + 96, data_tables::Item(base, id), 3008);
    const uint32_t owner =
        guest::LoadU16(base, frame + 184) == 3 ? guest::LoadU32(base, human + 980) : id;
    uint32_t k = 12;
    if (int32_t(guest::LoadU32(base, human + 3924)) == 1) {
      if (guest::LoadU16(base, Record(save, int32_t(owner)) + 2) != 1) {
        k = 11;
      } else {
        k = 10;
        const int32_t kind = int8_t(guest::LoadU8(base, frame + 131));
        if (kind == 3 || kind == 4 || kind == 5) {
          guest::StoreU32(base, Screen(base) + 820, 1);
        }
      }
    }
    const int32_t found = data_tables::Find20(base, id, k);
    const uint32_t entry =
        data_tables::Entry20(base, guest::LoadU32(base, Screen(base) + 872) + uint32_t(found));
    const uint32_t result = guest::LoadU32(base, Screen(base) + 872) + uint32_t(found) + 5918;
    const uint32_t text = guest::LoadU32(base, entry + 12) + 4650;
    SetText(base, calls, frame, movie, 4, 2, result);
    SetText(base, calls, frame, movie, 4, 1, text);
    HideNode(base, movie, 4, 0);
    SetPictureTexture(base, movie, 6, 1, PictureA(base, guest::LoadU32(base, entry + 16)));
    SetPictureTexture(base, movie, 6, 0, PictureB(base, guest::LoadU32(base, entry + 16)));
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

// A float word through a float register and back (lfs, stfs).
uint32_t ThroughFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, address))));
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
         guest::LoadU32(base, Screen(base) + 852) * 100 + save + kLocations, 100);
    const uint32_t entry = guest::LoadU32(base, guest::LoadU32(base, kListMenu)) * 12 + frame + 96;
    const uint32_t index = guest::LoadU32(base, entry + 8);
    const uint32_t table = guest::LoadU32(base, entry + 4);
    const uint32_t e56 = data_tables::Entry56(base, table, index);
    const uint32_t location = guest::LoadU32(base, Screen(base) + 852);
    guest::StoreU32(base, entry, 1);
    const uint32_t cursor = guest::LoadU32(base, guest::LoadU32(base, kListMenu));
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
      career::ItemFlagsSet(base, save, guest::LoadU32(base, e56 + 48),
                           guest::LoadU32(base, e56 + 52));
    }
    const uint32_t text = uint32_t(KindText(table, index));
    FinishNamed(calls, frame, 0x8222AF4C, movie,
                guest::LoadU32(base, guest::LoadU32(base, kListMenu)) + 52);
    ui_movies::Play(base, movie, 0xFFFFFFFFu, 8, 0, 0, 0, 0);
    SetText(base, calls, frame, movie, 4, 1, guest::LoadU32(base, e56 + 12) + 4650);
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
    HideNode(base, movie, 0xFFFFFFFFu, guest::LoadU32(base, guest::LoadU32(base, kListMenu)) + 19);
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
  for (uint32_t i = 0; i < guest::LoadU32(base, Screen(base) + 808); i++) {
    guest::StoreU32(base, frame + 80, i);
    const uint32_t event = guest::LoadU32(base, race_event::kEvent);
    const uint32_t item = race_event::GroupItem(
        base, event, guest::LoadU32(base, Screen(base) + 804), guest::LoadU32(base, frame + 80));
    Copy(base, calls, frame, 0x8222B2E4, frame + 128, item, 3008);
    ShowNode(base, movie, 14, i);
    const int32_t id = int16_t(guest::LoadU16(base, frame + 128));
    const bool won = int16_t(guest::LoadU16(base, Record(save, id) + 2)) != 0;
    bool owned = false;
    uint32_t texture;
    if (!won && guest::LoadU16(base, Record(save, id) + 4) == 0 &&
        guest::LoadU16(base, Record(save, id)) == 0) {
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
      if (guest::LoadU16(base, Record(save, next) + 2) == 0) {
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
  const uint32_t cursor = guest::LoadU32(base, guest::LoadU32(base, kOpponentMenu));
  const uint32_t time = guest::LoadU32(base, ((cursor << 2) & 0xFFFFFFFCu) + 0x82010780);
  const uint32_t location = guest::LoadU32(base, Screen(base) + 852);
  const uint32_t id = guest::LoadU32(base, ((location << 4) & 0xFFFFFFF0u) + 0x82636520);
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
  constexpr uint32_t kA = 0x8296B950, kB = 0x8296B980, kC = 0x8296B9B0, kED = 0x8265ED00;
  constexpr uint32_t k6A0 = 0x826366A0, k6B0 = 0x826366B0, k6C0 = 0x826366C0;
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
    Store64(base, 0x8296BAE0 + i, x0);
    Store64(base, 0x8296BB60 + i, y0);
    Store64(base, 0x8296BAE0 + i + 8, x1);
    Store64(base, 0x8296BB60 + i + 8, y1);
  }
  // r3 the third array's address as the original made it (li, addi: sign-extended).
  return uint64_t(int64_t(int32_t(kC)));
}

}  // namespace location_screen
