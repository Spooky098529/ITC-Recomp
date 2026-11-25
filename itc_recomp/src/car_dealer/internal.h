// The car dealer's shared helpers (src/car_dealer/ only).

#pragma once

#include <cstdint>

#include "car_dealer/car_dealer.h"
#include "guest.h"
#include "schema/car_dealer.h"
#include "schema/career.h"
#include "schema/material_passes.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "schema/race_setup.h"
#include "schema/tuning_screen.h"
#include "schema/type2_screens.h"
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
using MenuList = schema::MenuList;
using Save = schema::CareerSave;
using GarageList = schema::CareerGarageList;
using Type2G = schema::Type2ScreensGlobals;
using Node = schema::UiMovieNode;
using Swatch = schema::CarDealerSwatch;

inline uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// UiMovies::Find with the id the original stores at `key` (its frame word,
// read only by the find): the pair's value's address. The id is a value
// since E5 step 6; `key` names the slot the original used and isn't written.
inline uint32_t FindPair(uint32_t id, [[maybe_unused]] uint32_t key) {
  return ui_movies::PairsFindKey(ui_movies::kMovieTable, id);
}
inline uint32_t FindMovie(uint32_t id, uint32_t key) {
  return Load(FindPair(id, key));
}

// The selected car's entry: the shown list's entry at the data's car_scroll
// + car_selection.
inline Entry SelectedEntry() {
  const Data data = G::data();
  const uint32_t index = (data.car_scroll() + data.car_selection()) << 2;
  return Entry(uint32_t(data.shown_list().load_at(index, 4)));
}

// The stand's garage stage: the holder's +0.
inline uint32_t Stage() {
  return schema::StandHolder(G::stand_holder().address).stage_block_0();
}

}  // namespace car_dealer
