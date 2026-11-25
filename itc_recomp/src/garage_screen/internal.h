// The garage screen's shared helpers (src/garage_screen/ only).

#pragma once

#include <bit>
#include <climits>
#include <cmath>
#include <cstdint>

#include "garage_screen/garage_screen.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/garage_screen.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/race_screens.h"
#include "schema/stand_models.h"
#include "schema/tuning_screen.h"
#include "schema/type2_screens.h"
#include "ui_movies/game.h"

namespace garage_screen {

using G = schema::GarageScreenGlobals;
using Machine = schema::GarageScreen;
using PageObject = schema::PageObject;
using ScreenData = schema::GarageScreenData;
using MenuList = schema::MenuList;
using SaveData = schema::SaveData;
using Node = schema::UiMovieNode;

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

// UiMovies::Find with the id: the pair's value's address, and the movie in
// it. `key` is the frame word the original stored the id at for the find;
// only the find read it, so it's a value now (E5 step 6) and the word isn't
// written (the argument is left for the callers, unused).
inline uint32_t FindPair(uint32_t id, [[maybe_unused]] uint32_t key) {
  return ui_movies::PairsFindKey(ui_movies::kMovieTable, id);
}
inline uint32_t FindMovie(uint32_t id, uint32_t key) {
  return Load(FindPair(id, key));
}

// The screen's data ([0x826D842C]).
inline ScreenData Data() { return G::data(); }
// The page object ([0x826D8420]).
inline PageObject PagesOf() { return G::pages(); }
// The two menu lists' addresses ([0x826D8424], [0x826D8428]).
inline uint32_t List() { return G::list().address; }
inline uint32_t List2() { return G::list2().address; }
// The two menu lists as race-screens' MenuList.
inline MenuList ListView() { return G::list(); }
inline MenuList List2View() { return G::list2(); }
// The screen's save data (the data's +36).
inline SaveData Save() { return Data().save(); }

// The garage car record at `index` in the save's garage (+6120): the list
// walked from its head (the halfword +204) `index` times through each
// node's halfword +0 (nodes at the garage + 4 × the index), then the slot
// (the node's halfword +2); the record at the garage + 1,968 × the slot
// (the car itself at its +212). As the originals walk it inline.
inline uint32_t GarageRecord(const SaveData& save, uint32_t index) {
  const schema::CareerGarageList list(save.garage_address());
  const uint32_t garage = list.address;
  uint32_t link = garage + 4 * uint32_t(int32_t(int16_t(list.head())));
  while (index != 0) {
    link = garage + 4 * uint32_t(int32_t(int16_t(guest::LoadU16(link))));
    index -= 1;
  }
  const int32_t slot = int16_t(guest::LoadU16(link + 2));
  return garage + 1968 * uint32_t(slot);
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
inline void Post(uint32_t mode, uint32_t id) {
  const schema::GarageScreenMode m(mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  uint8_t* slot = m.messages() + 12 * ((start + count - 1) % 50);  // guest order (E5 step 4)
  schema::host::StoreU32(slot + 0, id);
  schema::host::StoreU32(slot + 4, 0);
  schema::host::StoreU32(slot + 8, 0);
}

// The garage stage's holder ([[0x826D83E0]]).
inline uint32_t Holder() { return G::stand_holder().stage_block_0(); }
// The garage stage (the holder's +0).
inline schema::GarageStage Stage() {
  return schema::GarageStage(Load(Holder()));
}

// The page object's moves the pages make inline: to `page` after
// `transition` then `queued` (-1 for none), +8 the page before unless it
// was `page`.
inline void PagesGo(const PageObject& pages, uint32_t page, uint32_t transition, uint32_t queued) {
  const uint32_t before = pages.page();
  if (before != page) {
    pages.set_previous_page(before);
  }
  pages.set_page(0xFFFFFFFFu);
  pages.set_queued_step(queued);
  pages.set_step(transition);
  pages.set_next_page(page);
}

// A float word read as the translated lfs does (a double of the float).
inline double Lfs(uint32_t address) {
  return double(std::bit_cast<float>(Load(address)));
}
// A double stored as the translated stfs does (rounded to a float).
inline void Stfs(uint32_t address, double value) {
  Store(address, std::bit_cast<uint32_t>(float(value)));
}
// A doubleword at `address` (two words, high first).
inline void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
inline uint64_t Load64(uint32_t address) {
  return (uint64_t(Load(address)) << 32) | Load(address + 4);
}
inline double Lfd(uint32_t address) {
  return std::bit_cast<double>(Load64(address));
}

// fctiwz as the translated code does it: NaN 0x80000000, at or above
// INT_MAX INT_MAX, else truncated (0x80000000 below INT_MIN).
inline uint32_t Fctiwz(double d) {
  if (std::isnan(d)) {
    return 0x80000000u;
  }
  if (d >= double(INT_MAX)) {
    return uint32_t(INT_MAX);
  }
  if (!(d > -2147483649.0)) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(d));
}

// An integer word converted as the originals do through their frame: its
// 64-bit sign extension (the std to `at` and the lfd back), fcfid and frsp.
// The float as a double. Only the conversion read the frame's doubleword, so
// it's a value now (E5 step 6) and `at` isn't written (left for the callers,
// unused).
inline double IntToFloat([[maybe_unused]] uint32_t at,
                         uint32_t word) {
  const int64_t wide = int64_t(int32_t(word));
  return double(float(double(wide)));
}

// The garage stage's holder's command (GarageStage::Command through its
// override) from `frame` returning to `lr`.
inline uint64_t StageCommand(uint32_t command, uint32_t frame, uint32_t lr,
                             const Calls& calls) {
  return calls.Call(kStageCommand, frame, lr, Holder(), command);
}

// UiMovie::SetText with r3 to r7 given and r8 to r10 0 (the varargs the
// call sites leave unset: the user's call for the parking area,
// 2026-10-30), or r10 given where the call site sets it.
inline uint64_t SetText(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                        uint64_t sub_scene, uint64_t child, uint64_t text, uint64_t r7 = 0,
                        uint64_t r10 = 0) {
  return calls.CallArgs(kSetText, frame, lr, {movie, sub_scene, child, text, r7, 0, 0, r10});
}

// memcpy: native::Buffers_Memcpy, else the guest call from `frame` with
// `lr`. The destination (memcpy's r3).
inline uint64_t Memcpy(const Calls& calls, uint64_t to, uint32_t from,
                       uint32_t bytes, uint32_t frame, uint32_t lr) {
  if (!native::Buffers_Memcpy(uint32_t(to), from, bytes, lr)) {
    return calls.Call(0x825A7410, frame, lr, to, from, bytes);
  }
  return to;
}

}  // namespace garage_screen
