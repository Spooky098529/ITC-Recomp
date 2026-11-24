// The type 2 screens' shared helpers (src/type2_screens/ only).

#pragma once

#include <climits>
#include <cmath>
#include <cstdint>

#include "guest.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/game_modes.h"
#include "schema/garage_screen.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/race_screens.h"
#include "schema/racer_controllers.h"
#include "schema/tuning_screen.h"
#include "schema/type2_screens.h"
#include "schema/ui_movies_game.h"
#include "schema/user_info.h"
#include "type2_screens/type2_screens.h"
#include "ui_movies/game.h"

namespace type2_screens {

using Globals = schema::Type2ScreensGlobals;
using Node = schema::UiMovieNode;          // tuning-screen.toml
using MenuList = schema::MenuList;         // race-screens.toml
using SaveData = schema::SaveData;         // race-event.toml

// A UI movie's texture word `index` (UiMovie +28's vector data).
inline uint32_t MovieTextureSlot(uint8_t* base, uint32_t movie, uint32_t index) {
  return schema::UiMovie(base, movie).textures() + 4 * index;
}

// A group node's first child index (+100) and child count (+104), read as
// words.
inline uint32_t FirstChild(uint8_t* base, uint32_t node) {
  return guest::LoadU32(base, Node(base, node).width_address());
}
inline uint32_t ChildCount(uint8_t* base, uint32_t node) {
  return guest::LoadU32(base, Node(base, node).height_address());
}

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// UiMovies::Find for `id`: the pair's value's address, and the movie in it.
// The original stored the id at `key` (its frame word) and passed its
// address; only the lookup read it, so the id goes by value (E5 step 6) and
// `key` is unused.
inline uint32_t FindPair(uint8_t* base, uint32_t id, [[maybe_unused]] uint32_t key) {
  return ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id);
}
inline uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  return Load(base, FindPair(base, id, key));
}

// The page object's moves the handlers make inline: to `page` after
// `transition` then `queued` (-1 for none), +8 the page before unless it
// was `page`.
inline void PagesGo(uint8_t* base, uint32_t pages_address, uint32_t page, uint32_t transition,
                    uint32_t queued) {
  const schema::PageObject pages(base, pages_address);
  const uint32_t before = pages.page();
  if (before != page) {
    pages.set_previous_page(before);
  }
  pages.set_step(transition);
  pages.set_queued_step(queued);
  pages.set_next_page(page);
  pages.set_page(0xFFFFFFFFu);
}

// Player 0's selected garage car: the garage (save + 6120) walked by its
// halfword slot links from +204, [save + 0x19920] steps, to the slot at the
// link's +2; its 1,968-byte record's address (garage + 1,968 x the slot;
// the car itself at +212).
inline uint32_t SelectedGarageCar(uint8_t* base, uint32_t save) {
  const SaveData sv(base, save);
  const uint32_t garage = sv.garage_address();
  uint32_t steps = sv.selected_car();
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(sv.garage_head())));
  while (steps != 0) {
    const int32_t next = int16_t(guest::LoadU16(base, link));
    steps -= 1;
    link = garage + 4 * uint32_t(next);
  }
  const int32_t slot = int16_t(guest::LoadU16(base, link + 2));
  return garage + 1968 * uint32_t(slot);
}

// fctiwz as the recompiled code does it: NaN 0x80000000, from INT_MAX
// saturated, below INT_MIN 0x80000000 (cvttsd2si).
inline int32_t TruncateWord(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value < -2147483648.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

// fcfid then frsp: an integer word as the single the original rounds it to.
inline float Single(int32_t value) { return float(double(int64_t(value))); }

}  // namespace type2_screens
