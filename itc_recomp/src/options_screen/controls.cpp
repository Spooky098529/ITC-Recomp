// The options screen's controls' tables and the settings' leaves
// (docs/functions/options-screen.md): see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/options_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <cstring>
#include <utility>

namespace options_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

using G = schema::OptionsScreenGlobals;
using S = schema::OptionsSettings;
using Word = SettingsWord;

// The edits copied (the original's copy below `sp` from `at`; a C++ array
// since E5 step 6), then 1 when an item other than `item` holds item
// `item`'s word (at `4 * item`, wrapped to 32 bits, on every try); one
// outside the copy reads the stack word the original reads there.
uint32_t Taken(uint32_t item, uint32_t at, const Word* words, uint32_t count) {
  const S o = G::settings();
  uint32_t edits[12];
  for (uint32_t i = 0; i < count; i++) {
    edits[i] = (o.*words[i])();
  }
  for (uint32_t i = 0; i < count; i++) {
    if (int32_t(i) == int32_t(item)) {
      continue;
    }
    const uint32_t mine = edits[i];
    const uint32_t offset = item << 2;
    const uint32_t theirs =
        offset < 4 * count ? edits[offset >> 2] : guest::LoadU32(at + offset);
    if (mine == theirs) {
      return 1;
    }
  }
  return 0;
}

// StepButtons and StepAnalog: `set_side` the settings' side word's store,
// `last` the last item (OK/Default).
uint32_t Step(void (S::*set_side)(uint32_t) const, int32_t last) {
  const schema::FrontEndMenu menu = G::menu();
  const int32_t side = menu.side();
  const int32_t cursor = int32_t(menu.cursor());
  (G::settings().*set_side)(0);
  if (side != -1 && side != 1) {
    return 0;
  }
  (G::settings().*set_side)(uint32_t(side));
  if (cursor != last) {
    return 1;
  }
  const S o = G::settings();
  const int32_t choice = int32_t(o.ok_default());
  if (side == -1 ? choice != 1 : choice != 0) {
    return 0;
  }
  o.set_ok_default(side == -1 ? 0 : 1);
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

uint32_t IsButtonTaken(uint32_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BA7E0 Options::IsButtonTaken");
  return Taken(item, sp - 48, kButtonEdits, 12);
}

uint32_t IsAnalogTaken(uint32_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BAB58 Options::IsAnalogTaken");
  return Taken(item, sp - 32, kAnalogEdits, 6);  // sp - 8 and - 4 not written
}

namespace {

// HasButtonClash and HasAnalogClash: the edits copied below `sp` in menu
// order (as the taken checks copy them), then 1 at the first pair of items
// holding the same word, else 0.
// (A C++ array since E5 step 6; `at` the original's copy, unused.)
uint32_t Clash([[maybe_unused]] uint32_t at, const Word* words, uint32_t count) {
  const S o = G::settings();
  uint32_t edits[12];
  for (uint32_t i = 0; i < count; i++) {
    edits[i] = (o.*words[i])();
  }
  for (uint32_t i = 0; i + 1 < count; i++) {
    const uint32_t mine = edits[i];
    for (uint32_t j = i + 1; j < count; j++) {
      if (mine == edits[j]) {
        return 1;
      }
    }
  }
  return 0;
}

}  // namespace

uint32_t HasButtonClash(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BA890 Options::HasButtonClash");
  return Clash(sp - 48, kButtonEdits, 12);
}

uint32_t HasAnalogClash(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BAC78 Options::HasAnalogClash");
  return Clash(sp - 32, kAnalogEdits, 6);
}

uint32_t StepButtons() {
  ITC_NATIVE_RUNNING("sub_821BA550 Options::StepButtons");
  return Step(&S::set_buttons_side, 12);
}

uint32_t StepAnalog() {
  ITC_NATIVE_RUNNING("sub_821BABD8 Options::StepAnalog");
  return Step(&S::set_analog_side, 6);
}

uint32_t StepSound() {
  ITC_NATIVE_RUNNING("sub_821BE5F8 Options::StepSound");
  const schema::FrontEndMenu menu = G::menu();
  const int32_t side = menu.side();
  const uint32_t cursor = menu.cursor();
  if (side != -1 && side != 1) {
    return 0;
  }
  const S o = G::settings();
  // The volumes (0 to 15): a step past a bound stores the bound and answers 0.
  const auto volume = [&](uint32_t (S::*get)() const, void (S::*set)(uint32_t) const) -> uint32_t {
    const int32_t value = int32_t((o.*get)()) + side;
    (o.*set)(uint32_t(value));
    if (side == -1 ? value >= 0 : value <= 15) {
      return 1;
    }
    (o.*set)(side == -1 ? 0 : 15);
    return 0;
  };
  switch (cursor) {
    case 0: return volume(&S::edit_music_volume, &S::set_edit_music_volume);
    case 1: return volume(&S::edit_effects_volume, &S::set_edit_effects_volume);
    case 2:
      // OK/Default: left on a 1, right on a 0.
      if (int32_t(o.ok_default()) != (side == -1 ? 1 : 0)) {
        return 0;
      }
      o.set_ok_default(side == -1 ? 0 : 1);
      return 1;
    case 3: {
      // The track, wrapping within 0 to 16.
      const int32_t value = int32_t(o.music_track()) + side;
      o.set_music_track(uint32_t(value));
      if (value < 0) {
        o.set_music_track(16);
      } else if (value > 16) {
        o.set_music_track(0);
      }
      return 1;
    }
    default: return 0;
  }
}

uint32_t StepSliders(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BEAC8 Options::StepSliders");
  // No guest frame (E5 step 6): the movie id passed as a value.
  // Node (7, 0)'s children 4 to 6 looked up, unused (as the original).
  const uint32_t node = ui_movies::FindNode(FindMovie(sp), 7, 0);
  for (uint32_t i = 4; i <= 6; i++) {
    ui_movies::Child(node, i);
  }
  const schema::FrontEndMenu menu = G::menu();
  const int32_t side = menu.side();
  const uint32_t cursor = menu.cursor();
  if (side != -1 && side != 1) {
    return 0;
  }
  const S o = G::settings();
  if (cursor < 3) {
    // The sliders (-10 to 10): no change at a bound.
    const uint32_t field = o.edit_slider_0_address() + 4 * cursor;
    const int32_t value = int32_t(guest::LoadU32(field));
    if (side == -1 ? value <= -10 : value >= 10) {
      return 0;
    }
    guest::StoreU32(field, uint32_t(value + side));
    return 1;
  }
  if (cursor != 3 || int32_t(o.ok_default()) != (side == -1 ? 1 : 0)) {
    return 0;
  }
  o.set_ok_default(side == -1 ? 0 : 1);
  return 1;
}

uint32_t SettingsReset(uint32_t settings, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821B9DB8 Settings::Reset");
  (void)sp;  // its frame held only the back chain (E5 step 6)
  const S o(settings);
  o.set_set_up(0);
  // memset (sub_825A71D0) with 0: the same bytes (+12: the name to the day).
  for (const auto& [at, bytes] : {std::pair<uint32_t, uint32_t>{o.current_block_address(), 192},
                                  {o.edit_block_address(), 192},
                                  {o.default_block_address(), 192},
                                  {o.name_0_address(), 32}}) {
    guest::ForEachHostPart(at, bytes,
                           [](uint8_t* host, uint32_t, uint32_t size) {
                             std::memset(host, 0, size);
                           });
  }
  o.set_category_cursor(0);
  o.set_ok_default(0);
  o.set_wait_counter(0);
  o.set_save_page_closed(1);
  o.set_load_dialog_state(kNone);
  o.set_load_dialog_next(kNone);
  o.set_load_dialog_kind(kNone);
  o.set_load_dialog_passes(0);
  o.set_load_dialog_default(0);
  o.set_save_dialog_state(kNone);
  o.set_save_dialog_next(kNone);
  o.set_save_dialog_kind(kNone);
  o.set_save_dialog_passes(0);
  o.set_save_dialog_default(0);
  return o.name_0_address();
}

}  // namespace options_screen
