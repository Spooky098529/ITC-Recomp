// The car dealer's shared helpers (src/car_dealer/ only).

#pragma once

#include <cstdint>

#include "car_dealer/car_dealer.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace car_dealer {

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
// the pair's value's address.
inline uint32_t FindPair(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
}
inline uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  return Load(base, FindPair(base, id, key));
}

// The selected car's entry: the shown list's (+1136) entry at the data's
// +48 + +52.
inline uint32_t SelectedEntry(uint8_t* base) {
  const uint32_t data = Load(base, kData);
  const uint32_t index = (Load(base, data + 52) + Load(base, data + 48)) << 2;
  return Load(base, index + Load(base, data + 1136));
}

}  // namespace car_dealer
