// The options screen's controls' tables and the settings' leaves
// (docs/functions/options-screen.md): see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <cstring>
#include <utility>

namespace options_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

uint32_t Settings(uint8_t* base) { return guest::LoadU32(base, kSettings); }

// The edits copied below `sp` from `at`, then 1 when an item
// other than `item` holds item `item`'s word (read from `at + 4 * item`, the
// 32-bit wrap of the original's address, on every try).
uint32_t Taken(uint8_t* base, uint32_t item, uint32_t at, const uint32_t* offsets,
               uint32_t count) {
  const uint32_t o = Settings(base);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, at + 4 * i, guest::LoadU32(base, o + offsets[i]));
  }
  for (uint32_t i = 0; i < count; i++) {
    if (int32_t(i) == int32_t(item)) {
      continue;
    }
    const uint32_t mine = guest::LoadU32(base, at + 4 * i);
    const uint32_t theirs = guest::LoadU32(base, at + (item << 2));
    if (mine == theirs) {
      return 1;
    }
  }
  return 0;
}

// StepButtons and StepAnalog: `side_word` the settings' word, `last` the
// last item (OK/Default).
uint32_t Step(uint8_t* base, uint32_t side_word, int32_t last) {
  const uint32_t menu = guest::LoadU32(base, kMenu);
  const int32_t side = int32_t(guest::LoadU32(base, menu + 36));
  const int32_t cursor = int32_t(guest::LoadU32(base, menu + 0));
  guest::StoreU32(base, Settings(base) + side_word, 0);
  if (side != -1 && side != 1) {
    return 0;
  }
  guest::StoreU32(base, Settings(base) + side_word, uint32_t(side));
  if (cursor != last) {
    return 1;
  }
  const uint32_t o = Settings(base);
  const int32_t choice = int32_t(guest::LoadU32(base, o + 660));
  if (side == -1 ? choice != 1 : choice != 0) {
    return 0;
  }
  guest::StoreU32(base, o + 660, side == -1 ? 0 : 1);
  return 1;
}

}  // namespace

uint32_t ButtonIndex(uint32_t code) {
  ITC_NATIVE_RUNNING("sub_821BA950 Controls::ButtonIndex");
  // Codes 0 to 30; -1 for the rest.
  static constexpr int8_t kIndex[31] = {-1, -1, -1, -1, -1, -1, -1, 2,  4,  3,  5,
                                        -1, 14, 7,  6,  13, 12, 16, 1,  -1, -1, -1,
                                        9,  11, 15, 0,  -1, -1, -1, 8,  10};
  return code > 30 ? kNone : uint32_t(int32_t(kIndex[code]));
}

uint32_t ButtonCode(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821BAA18 Controls::ButtonCode");
  // By index + 1 (0 to 19).
  static constexpr uint8_t kCode[20] = {0,  25, 18, 7,  9,  8,  10, 14, 13, 29,
                                        22, 30, 23, 16, 15, 12, 24, 17, 16, 15};
  const uint32_t i = index + 1;
  return i > 19 ? 0 : kCode[i];
}

uint32_t AnalogIndex(uint32_t code) {
  ITC_NATIVE_RUNNING("sub_821BAAE0 Controls::AnalogIndex");
  switch (code) {
    case 7: return 2;
    case 8: return 4;
    case 9: return 3;
    case 10: return 5;
    case 12: return 14;
    case 15: return 18;
    case 16: return 17;
    default: return kNone;
  }
}

uint32_t AnalogPicture(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821BAD08 Controls::AnalogPicture");
  switch (index + 1) {
    case 3: return 2;
    case 4: return 3;
    case 5: return 4;
    case 6: return 5;
    case 13: return 0;
    case 14: return 1;
    case 15: return 10;
    case 18: return 0;
    case 19: return 1;
    default: return kNone;
  }
}

uint32_t IsButtonTaken(uint8_t* base, uint32_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BA7E0 Options::IsButtonTaken");
  static constexpr uint32_t kEdits[12] = {284, 288, 292, 296, 300, 304,
                                          320, 312, 316, 308, 324, 328};
  return Taken(base, item, sp - 48, kEdits, 12);
}

uint32_t IsAnalogTaken(uint8_t* base, uint32_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BAB58 Options::IsAnalogTaken");
  static constexpr uint32_t kEdits[6] = {380, 384, 388, 396, 392, 400};
  return Taken(base, item, sp - 32, kEdits, 6);  // sp - 8 and - 4 not written
}

namespace {

// HasButtonClash and HasAnalogClash: the edits copied below `sp` in menu
// order (as the taken checks copy them), then 1 at the first pair of items
// holding the same word, else 0.
uint32_t Clash(uint8_t* base, uint32_t at, const uint32_t* offsets, uint32_t count) {
  const uint32_t o = Settings(base);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, at + 4 * i, guest::LoadU32(base, o + offsets[i]));
  }
  for (uint32_t i = 0; i + 1 < count; i++) {
    const uint32_t mine = guest::LoadU32(base, at + 4 * i);
    for (uint32_t j = i + 1; j < count; j++) {
      if (mine == guest::LoadU32(base, at + 4 * j)) {
        return 1;
      }
    }
  }
  return 0;
}

}  // namespace

uint32_t HasButtonClash(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BA890 Options::HasButtonClash");
  static constexpr uint32_t kEdits[12] = {284, 288, 292, 296, 300, 304,
                                          320, 312, 316, 308, 324, 328};
  return Clash(base, sp - 48, kEdits, 12);
}

uint32_t HasAnalogClash(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BAC78 Options::HasAnalogClash");
  static constexpr uint32_t kEdits[6] = {380, 384, 388, 396, 392, 400};
  return Clash(base, sp - 32, kEdits, 6);
}

uint32_t StepButtons(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821BA550 Options::StepButtons");
  return Step(base, 668, 12);
}

uint32_t StepAnalog(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821BABD8 Options::StepAnalog");
  return Step(base, 676, 6);
}

uint32_t StepSound(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821BE5F8 Options::StepSound");
  const uint32_t menu = guest::LoadU32(base, kMenu);
  const int32_t side = int32_t(guest::LoadU32(base, menu + 36));
  const uint32_t cursor = guest::LoadU32(base, menu + 0);
  if (side != -1 && side != 1) {
    return 0;
  }
  const uint32_t o = Settings(base);
  // The volumes (0 to 15): a step past a bound stores the bound and answers 0.
  const auto volume = [&](uint32_t field) -> uint32_t {
    const int32_t value = int32_t(guest::LoadU32(base, o + field)) + side;
    guest::StoreU32(base, o + field, uint32_t(value));
    if (side == -1 ? value >= 0 : value <= 15) {
      return 1;
    }
    guest::StoreU32(base, o + field, side == -1 ? 0 : 15);
    return 0;
  };
  switch (cursor) {
    case 0: return volume(252);
    case 1: return volume(256);
    case 2:
      // OK/Default: left on a 1, right on a 0.
      if (int32_t(guest::LoadU32(base, o + 660)) != (side == -1 ? 1 : 0)) {
        return 0;
      }
      guest::StoreU32(base, o + 660, side == -1 ? 0 : 1);
      return 1;
    case 3: {
      // The track, wrapping within 0 to 16.
      const int32_t value = int32_t(guest::LoadU32(base, o + 688)) + side;
      guest::StoreU32(base, o + 688, uint32_t(value));
      if (value < 0) {
        guest::StoreU32(base, o + 688, 16);
      } else if (value > 16) {
        guest::StoreU32(base, o + 688, 0);
      }
      return 1;
    }
    default: return 0;
  }
}

uint32_t StepSliders(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BEAC8 Options::StepSliders");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  // Node (7, 0)'s children 4 to 6 looked up, unused (as the original).
  const uint32_t node = ui_movies::FindNode(base, FindMovie(base, frame), 7, 0);
  for (uint32_t i = 4; i <= 6; i++) {
    ui_movies::Child(base, node, i);
  }
  const uint32_t menu = guest::LoadU32(base, kMenu);
  const int32_t side = int32_t(guest::LoadU32(base, menu + 36));
  const uint32_t cursor = guest::LoadU32(base, menu + 0);
  if (side != -1 && side != 1) {
    return 0;
  }
  const uint32_t o = Settings(base);
  if (cursor < 3) {
    // The sliders (-10 to 10): no change at a bound.
    const uint32_t field = 260 + 4 * cursor;
    const int32_t value = int32_t(guest::LoadU32(base, o + field));
    if (side == -1 ? value <= -10 : value >= 10) {
      return 0;
    }
    guest::StoreU32(base, o + field, uint32_t(value + side));
    return 1;
  }
  if (cursor != 3 || int32_t(guest::LoadU32(base, o + 660)) != (side == -1 ? 1 : 0)) {
    return 0;
  }
  guest::StoreU32(base, o + 660, side == -1 ? 0 : 1);
  return 1;
}

uint32_t SettingsReset(uint8_t* base, uint32_t settings, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821B9DB8 Settings::Reset");
  guest::StoreU32(base, sp - 112, sp);
  guest::StoreU32(base, settings + 0, 0);
  // memset (sub_825A71D0) with 0: the same bytes.
  for (const auto& [offset, bytes] : {std::pair<uint32_t, uint32_t>{44, 192},
                                      {236, 192},
                                      {428, 192},
                                      {12, 32}}) {
    guest::ForEachHostPart(base, settings + offset, bytes,
                           [](uint8_t* host, uint32_t, uint32_t size) {
                             std::memset(host, 0, size);
                           });
  }
  for (const uint32_t field : {664u, 660u, 684u}) {
    guest::StoreU32(base, settings + field, 0);
  }
  guest::StoreU32(base, settings + 712, 1);
  for (const uint32_t field : {620u, 624u, 628u}) {
    guest::StoreU32(base, settings + field, kNone);
  }
  guest::StoreU32(base, settings + 632, 0);
  guest::StoreU32(base, settings + 636, 0);
  for (const uint32_t field : {640u, 644u, 648u}) {
    guest::StoreU32(base, settings + field, kNone);
  }
  guest::StoreU32(base, settings + 652, 0);
  guest::StoreU32(base, settings + 656, 0);
  return settings + 12;
}

}  // namespace options_screen
