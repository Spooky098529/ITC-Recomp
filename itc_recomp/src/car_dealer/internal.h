// The car dealer's shared helpers (src/car_dealer/ only).

#pragma once

#include <cstdint>

#include "car_dealer/car_dealer.h"
#include "guest.h"
#include "schema/car_dealer.h"
#include "schema/menu_helpers.h"
#include "ui_movies/game.h"

namespace car_dealer {

// The views of the dealer's objects (itc_recomp/schema/car-dealer.toml).
using G = schema::CarDealerGlobals;
using Machine = schema::CarDealer;
using Mode = schema::CarDealerMode;
using Data = schema::CarDealerData;
using List = schema::CarDealerList;
using Colors = schema::CarDealerColors;
using Entry = schema::CarDealerEntry;
using SpecColor = schema::CarDealerSpecColor;
using Pages = schema::PageObject;

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

// The selected car's entry: the shown list's entry at the data's car_scroll
// + car_selection.
inline Entry SelectedEntry(uint8_t* base) {
  const Data data = G::data(base);
  const uint32_t index = (data.car_scroll() + data.car_selection()) << 2;
  return Entry(base, Load(base, index + data.shown_list().address));
}

// The stand's garage stage: the holder's +0.
inline uint32_t Stage(uint8_t* base) { return Load(base, G::stand_holder(base).address); }

}  // namespace car_dealer
