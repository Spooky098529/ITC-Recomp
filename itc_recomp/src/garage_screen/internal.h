// The garage screen's shared helpers (src/garage_screen/ only).

#pragma once

#include <cstdint>

#include "garage_screen/garage_screen.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace garage_screen {

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

// UiMovies::Find with the id stored at `key` (the original's frame word):
// the pair's value's address, and the movie in it.
inline uint32_t FindPair(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
}
inline uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  return Load(base, FindPair(base, id, key));
}

inline uint32_t Data(uint8_t* base) { return Load(base, kData); }

// The garage car record at `index` in the save's garage (+6120): the list
// walked from its head (the halfword +204) `index` times through each
// node's halfword +0 (nodes at the garage + 4 × the index), then the slot
// (the node's halfword +2); the record at the garage + 1,968 × the slot
// (the car itself at its +212). As the originals walk it inline.
inline uint32_t GarageRecord(uint8_t* base, uint32_t save, uint32_t index) {
  const uint32_t garage = save + 6120;
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
  while (index != 0) {
    link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, link))));
    index -= 1;
  }
  const int32_t slot = int16_t(guest::LoadU16(base, link + 2));
  return garage + 1968 * uint32_t(slot);
}

// The page object's moves the pages make inline: to `page` after
// `transition` then `queued` (-1 for none), +8 the page before unless it
// was `page`.
inline void PagesGo(uint8_t* base, uint32_t pages, uint32_t page, uint32_t transition,
                    uint32_t queued) {
  const uint32_t before = Load(base, pages + 0);
  if (before != page) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 0, 0xFFFFFFFFu);
  Store(base, pages + 16, queued);
  Store(base, pages + 12, transition);
  Store(base, pages + 4, page);
}

}  // namespace garage_screen
