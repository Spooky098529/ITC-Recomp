// The type 10 mode's shared helpers (src/type10_mode/ only).

#pragma once

#include <cstdint>

#include "game_modes/game_modes.h"
#include "guest.h"
#include "schema/game_modes.h"
#include "schema/boot_mode.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "schema/save_interface.h"
#include "schema/save_screen.h"
#include "schema/tuning_screen.h"
#include "schema/type10_mode.h"
#include "type10_mode/type10_mode.h"
#include "ui_movies/game.h"

namespace type10_mode {

inline uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

using G = schema::Type10ModeGlobals;
using Machine = schema::ReplayTheaterMachine;
using PageObject = schema::PageObject;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// UiMovies::Find for movie 45 with its id stored at `key` (the original's
// frame word): the movie in the pair's value.
inline uint32_t FindMovie(uint32_t key) {
  Store(key, kMovie);
  return Load(ui_movies::PairsFind(ui_movies::kMovieTable, key));
}

inline PageObject Pages() { return G::pages(); }
inline schema::ReplayTheaterData Data() { return G::data(); }
// The replay list ([0x828161E4], race-screens.toml's MenuList).
inline schema::MenuList List() { return G::list(); }
inline schema::SaveBuffers SaveBuffers() { return G::save_buffers(); }
// The `index`th of the replays' 36-byte records from `records`.
inline schema::SaveReplayRecord Record(uint32_t records, uint32_t index) {
  return schema::SaveReplayRecord(records + index * 36);
}
using Node = schema::UiMovieNode;

// Posts the message (id, 0, 0) on the dispatcher's ring (boot-mode.toml's
// ModeDispatcher: its count +804, start +808, 50 slots of 12 bytes from
// +204), as the originals do inline.
inline void Post(uint32_t dispatcher, uint32_t id) {
  const schema::ModeDispatcher d(dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t start = d.ring_start();
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + 12 * ((start + count - 1) % 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}
// On the ring at `ring` (a dispatcher's +204).
inline void PostRing(uint32_t ring, uint32_t id) { Post(ring - 204, id); }

// The page object's +8 = its page unless that is `unless`.
inline void KeepPage(const PageObject& pages, uint32_t unless) {
  const uint32_t page = pages.page();
  if (page != unless) {
    pages.set_previous_page(page);
  }
}

}  // namespace type10_mode
