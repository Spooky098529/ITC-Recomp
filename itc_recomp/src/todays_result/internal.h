// The type 9 mode's shared helpers (src/todays_result/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/career.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/menu_sounds.h"
#include "schema/race_event.h"
#include "schema/todays_result.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"
#include "todays_result/todays_result.h"
#include "ui_movies/game.h"

namespace todays_result {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

using G = schema::TodaysResultGlobals;
using Pages = schema::PageObject;
// A UI movie's node (+80 visible, +108 its texture entry).
using Node = schema::UiMovieNode;
// The save data (SaveData::Of(0)).
using Save = schema::SaveData;

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// UiMovies::Find for movie `id` with the id stored at `key` (the original's
// frame word): the movie in the pair's value.
inline uint32_t FindMovie(uint8_t* base, uint32_t key, uint32_t id) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// Posts the message (id, 0, 0) on a ring (50 ModeMessages of 12 bytes from
// `ring`, its count at `count_at` and start at `start_at`), as the
// originals do inline.
inline void PostRing(uint8_t* base, uint32_t ring, uint32_t count_at, uint32_t start_at,
                     uint32_t id) {
  const uint32_t count = Load(base, count_at) + 1;
  const uint32_t start = Load(base, start_at);
  Store(base, count_at, count);
  const schema::ModeMessage slot(base, ring + schema::ModeMessage::kSize * ((start + count - 1) % 50));
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}
// On a dispatcher's ring.
inline void Post(uint8_t* base, uint32_t dispatcher, uint32_t id) {
  const schema::ModeDispatcher d(base, dispatcher);
  PostRing(base, d.ring_address(), d.ring_count_address(), d.ring_start_address(), id);
}
// On the type 9 mode's ring (its dispatcher's at +36).
inline void PostMode(const schema::ModeType9& mode, uint32_t id) {
  PostRing(mode.base, mode.ring_address(), mode.ring_count_address(), mode.ring_start_address(),
           id);
}

// A screen's new menu set up as both Creates do: every word 0 but the last
// item (+16), 4, in the originals' order.
inline void InitMenu(const schema::FrontEndMenu& menu) {
  menu.set_first_item(0);
  menu.set_wraps(0);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_last_item(4);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
}

// The page object's +8 = its page unless that is `unless`.
inline void KeepPage(const Pages& pages, uint32_t unless) {
  const uint32_t page = pages.page();
  if (page != unless) {
    pages.set_previous_page(page);
  }
}

}  // namespace todays_result
