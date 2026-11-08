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

}  // namespace type2_screens
