// The helpers the location screen's transitions share
// (src/location_screen/transitions.cpp and the files beside it): the movie
// lookups, the text setters through their plain C++ (a refusal after the
// adapters' check is a logged abort), the node setters, and the stops and
// runs through location_screen::Calls.

#pragma once

#include "base/log.h"
#include "guest.h"
#include "location_screen/location_screen.h"
#include "mode_tick/mode_tick.h"
#include "schema/location_screen.h"
#include "schema/menu_helpers.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"

#include <bit>
#include <cstdlib>
#include <optional>

namespace location_screen::detail {

constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;

using G = schema::LocationScreenGlobals;

inline schema::LocationScreen Screen() { return G::screen(); }

// A float word through a float register and back (lfs, stfs).
inline uint32_t ThroughFloat(uint32_t address) {
  return std::bit_cast<uint32_t>(float(double(guest::LoadFloat(address))));
}

// The movie with id `id`: UiMovies::Find over the movie table (the
// original's key word at its frame's +80, read only by the find: a value
// since E5 step 6).
inline uint32_t Movie(uint32_t id) {
  return guest::LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, id));
}

inline uint32_t Required(const std::optional<uint32_t>& r3, const char* what) {
  if (!r3) {
    ITC_LOG_ERROR("location screen: {} refused after the adapter's checks", what);
    std::abort();
  }
  return *r3;
}

// UiMovie::SetText from the frame at `frame` (its r1), no arguments.
inline uint32_t SetText(const Calls& calls, uint32_t frame, uint32_t movie,
                        uint32_t sub_scene, uint32_t child, uint32_t id) {
  static constexpr uint64_t kNoArguments[4] = {0, 0, 0, 0};
  return Required(
      ui_movies::SetText(*calls.format, frame, movie, sub_scene, child, id, kNoArguments),
      "UiMovie::SetText");
}

// UiMovie::SetNodeText from the frame at `frame`, no arguments (r6 to r10
// passed as 0: the user's call for SetText's unset varargs, 2026-10-30).
inline uint32_t SetNodeText(const Calls& calls, uint32_t frame, uint32_t movie,
                            uint32_t node, uint32_t id) {
  static constexpr uint64_t kNoArguments[5] = {0, 0, 0, 0, 0};
  return Required(ui_movies::SetNodeText(*calls.format, frame, movie, node, id, kNoArguments),
                  "UiMovie::SetNodeText");
}

inline uint32_t ShowNode(uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  schema::UiMovieNode(node).set_visible(1);
  return node;
}

inline uint32_t HideNode(uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  schema::UiMovieNode(node).set_visible(0);
  return node;
}

// UiMovie::SetNodeColor: the node (sub_scene, child)'s color.
inline uint32_t SetNodeColor(uint32_t movie, uint32_t sub_scene, uint32_t child,
                             uint32_t color) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  ui_movies::SetColor(node, color);
  return node;
}

// UiMovie::SetPictureTexture: the picture's texture swapped; the old one.
inline uint32_t SetPictureTexture(uint32_t movie, uint32_t sub_scene, uint32_t child,
                                  uint32_t texture) {
  const uint32_t picture = ui_movies::FindNode(movie, sub_scene, child);
  const uint32_t slot = ui_movies::PictureTextureSlot(movie, picture);
  const uint32_t old = guest::LoadU32(slot);
  guest::StoreU32(slot, texture);
  return old;
}

inline uint32_t PlayNamed(uint32_t movie, uint32_t named, uint32_t chained = 0,
                          uint32_t reversed = 0) {
  return ui_movies::PlayNamed(movie, named, chained, reversed, 0, 0);
}

// UiMovie::LoopLast called from the frame at `frame`: its r3 (the slot it
// moved to, else a copy in its 176-byte frame or the movie).
inline uint32_t LoopLast(uint32_t frame, uint32_t movie) {
  const ui_movies::Moved moved = ui_movies::LoopLast(movie);
  if (moved.slot) {
    return *moved.slot;
  }
  return moved.looked ? frame - 176 + 80 : movie;
}

inline void FinishNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                        uint32_t named) {
  calls.Call(kFinishNamed, frame, lr, movie, named);
}

inline uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// Whether a fade runs (either fade's counts word set).
inline bool Fading() {
  return (guest::LoadU32(mode_tick::kFadeOutCounts) |
          guest::LoadU32(mode_tick::kFadeInCounts)) != 0;
}

// LocationScreen::OpponentHasCard from the frame at `frame`: its plain C++,
// or its adapter through `calls` where the item's copy can't be native.
inline uint32_t HasCard(const Calls& calls, uint32_t frame, uint32_t lr) {
  if (const auto r3 = OpponentHasCard(frame)) {
    return *r3;
  }
  return uint32_t(calls.Call(0x82227708, frame, lr, 0));
}

}  // namespace location_screen::detail
