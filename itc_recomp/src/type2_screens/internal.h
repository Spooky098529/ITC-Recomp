// The type 2 screens' shared helpers (src/type2_screens/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "type2_screens/type2_screens.h"
#include "ui_movies/game.h"

namespace type2_screens {

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

// The page object's moves the handlers make inline: to `page` after
// `transition` then `queued` (-1 for none), +8 the page before unless it
// was `page`.
inline void PagesGo(uint8_t* base, uint32_t pages, uint32_t page, uint32_t transition,
                    uint32_t queued) {
  const uint32_t before = Load(base, pages + 0);
  if (before != page) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 12, transition);
  Store(base, pages + 16, queued);
  Store(base, pages + 4, page);
  Store(base, pages + 0, 0xFFFFFFFFu);
}

// Player 0's selected garage car: the garage (save + 6120) walked by its
// halfword slot links from +204, [save + 0x19920] steps, to the slot at the
// link's +2; its 1,968-byte record's address (garage + 1,968 x the slot;
// the car itself at +212).
inline uint32_t SelectedGarageCar(uint8_t* base, uint32_t save) {
  const uint32_t garage = save + 6120;
  uint32_t steps = Load(base, save + 0x19920);
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(base, garage + 204))));
  while (steps != 0) {
    const int32_t next = int16_t(guest::LoadU16(base, link));
    steps -= 1;
    link = garage + 4 * uint32_t(next);
  }
  const int32_t slot = int16_t(guest::LoadU16(base, link + 2));
  return garage + 1968 * uint32_t(slot);
}

}  // namespace type2_screens
