// The save screen's shared helpers (src/save_screen/ only).

#pragma once

#include <cstdint>

#include "game_modes/game_modes.h"
#include "guest.h"
#include "save_screen/save_screen.h"
#include "schema/action_mappers.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/race_screens.h"
#include "schema/racer_controllers.h"
#include "schema/save_interface.h"
#include "schema/save_screen.h"
#include "schema/system_services.h"
#include "ui_movies/game.h"

namespace save_screen {

using G = schema::SaveScreenGlobals;
using Machine = schema::SaveScreen;

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kNone = 0xFFFFFFFFu;

// An original's frame below `sp` (its back chain written) and its guest
// calls from it, keeping the last call's r3.
struct Fn {
  Fn(uint8_t* base, uint32_t sp, uint32_t bytes, const Calls& calls, uint64_t r3 = 0)
      : base(base), frame(sp - bytes), calls(calls), r3(r3) {
    Store(base, frame, sp);
  }
  uint64_t Call(uint32_t function, uint32_t lr, uint64_t a = 0, uint64_t b = 0, uint64_t c = 0,
                uint64_t d = 0) {
    r3 = calls.Call(function, frame, lr, a, b, c, d);
    return r3;
  }
  // A call's r3 as the int32 the originals compare.
  int32_t CallInt(uint32_t function, uint32_t lr, uint64_t a = 0) {
    return int32_t(uint32_t(Call(function, lr, a)));
  }
  uint8_t* base;
  uint32_t frame;
  const Calls& calls;
  uint64_t r3;
};

// The screen's blocks, from their globals at the call: the data
// (SaveScreenData), the page block (PageObject) and the menu list's address
// (a MenuList, race-screens.toml's; G::list(base) for its fields).
inline schema::SaveScreenData Data(uint8_t* base) { return G::data(base); }
inline schema::PageObject Pages(uint8_t* base) { return G::pages(base); }
inline uint32_t List(uint8_t* base) { return Load(base, G::list_address()); }

// The page block's moves the pages make inline: to `page` after `step`
// then `queued` (-1 for none), +8 the page before unless it was `page`.
inline void Go(uint8_t* base, uint32_t page, uint32_t step, uint32_t queued) {
  const auto pages = Pages(base);
  const uint32_t before = pages.page();
  if (int32_t(before) != int32_t(page)) {
    pages.set_previous_page(before);
  }
  pages.set_step(step);
  pages.set_queued_step(queued);
  pages.set_next_page(page);
  pages.set_page(kNone);
}
// Straight to `page`: +8 the page before unless it was `page`, +0 the page,
// +12 and +16 -1.
inline void Jump(uint8_t* base, uint32_t page) {
  const auto pages = Pages(base);
  const uint32_t before = pages.page();
  if (int32_t(before) != int32_t(page)) {
    pages.set_previous_page(before);
  }
  pages.set_page(page);
  pages.set_step(kNone);
  pages.set_queued_step(kNone);
}
// The data's next message (+12) and the dialog's state after the close
// (+20).
inline void Message(uint8_t* base, uint32_t message, uint32_t state) {
  Data(base).set_message(message);
  Data(base).set_state_after_close(state);
}

// The message {id, 0, 0} posted on the mode's ring (the count up by one,
// the slot (start + count - 1) mod 50, 12 bytes).
inline void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const schema::SaveScreenMode ring(base, mode);
  const uint32_t count = ring.message_count() + 1;
  const uint32_t at = ring.message_start() + count - 1;
  ring.set_message_count(count);
  const game_modes::RingSlot entry{ring.messages() + 12 * (at % 50)};
  entry.set_arg1(0);
  entry.set_arg0(0);
  entry.set_id(id);
}

// UiMovies::Find(id) as the originals make it: the id stored at `key` (a
// frame word), the movie table's pair for it, its movie.
inline uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

}  // namespace save_screen
