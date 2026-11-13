// The type 10 mode's shared helpers (src/type10_mode/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "type10_mode/type10_mode.h"
#include "ui_movies/game.h"

namespace type10_mode {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// UiMovies::Find for movie 45 with its id stored at `key` (the original's
// frame word): the movie in the pair's value.
inline uint32_t FindMovie(uint8_t* base, uint32_t key) {
  Store(base, key, kMovie);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

inline uint32_t Pages(uint8_t* base) { return Load(base, kPages); }
inline uint32_t Data(uint8_t* base) { return Load(base, kData); }

// Posts the message (id, 0, 0) on the ring at `ring` (its count +600, start
// +604, 50 slots of 12 bytes), as the originals do inline.
inline void PostRing(uint8_t* base, uint32_t ring, uint32_t id) {
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}
// On a dispatcher's ring (+204).
inline void Post(uint8_t* base, uint32_t dispatcher, uint32_t id) {
  PostRing(base, dispatcher + 204, id);
}

// The page object's +8 = its page unless that is `unless`.
inline void KeepPage(uint8_t* base, uint32_t pages, uint32_t unless) {
  const uint32_t page = Load(base, pages + 0);
  if (page != unless) {
    Store(base, pages + 8, page);
  }
}

}  // namespace type10_mode
