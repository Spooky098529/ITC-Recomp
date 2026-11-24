// The title screen's shared helpers (src/title_screen/ only).

#pragma once

#include <cstdint>

#include "game_modes/game_modes.h"
#include "guest.h"
#include "hooks/heap.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "schema/title_screen.h"
#include "title_screen/title_screen.h"
#include "ui_movies/game.h"

namespace title_screen {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

using G = schema::TitleScreenGlobals;
using Machine = schema::Type4Machine;
// A transition's started word (the page object's +24, the title load
// block's +16), as the transitions get it (r3).
using Started = schema::TitleTransitionStarted;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// VirtualFree(address, 0, MEM_RELEASE) returning to `lr`: straight to the
// allocation layer (hooks::VirtualFree, its adapter's answer, E5 step 6);
// through the guest from `frame` where the adapter would run the original
// or answer an address of 0.
inline uint64_t FreePages(uint32_t address, uint32_t frame, uint32_t lr, const Calls& calls) {
  if (address != 0) {
    if (const auto freed = hooks::VirtualFree(address, 0, kMemRelease, lr)) {
      return *freed;
    }
  }
  return calls.Call(kVirtualFree, frame, lr, address, 0, kMemRelease);
}

// UiMovies::Find with the id stored at `key` (the original's frame word):
// the movie in the pair's value.
inline uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The title's words ([0x82814E34], loaded at each call).
inline schema::TitleScreenData Words(uint8_t* base) { return G::data(base); }

// The menu list ([0x82814E30], race-screens' MenuList; loaded at each call).
inline schema::MenuList MenuListOf(uint8_t* base) { return G::list(base); }
inline uint32_t ListAddress(uint8_t* base) { return G::list(base).address; }

// Menu4's menu reset (TitleScreen::Create, SetUpMenu4): first item 0, last
// 1, wrapping, the rest cleared, in the originals' order.
inline void ResetMenu(const schema::FrontEndMenu& menu) {
  menu.set_first_item(0);
  menu.set_last_item(1);
  menu.set_wraps(1);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
}

// Posts the message (id, 0, 0) on the dispatcher's ring (`mode`, the
// handlers' r4: boot-mode's ModeDispatcher, the mode's +36; its count,
// start, 50 slots of 12 bytes), as the handlers do inline.
inline void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const schema::ModeDispatcher d(base, mode);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t start = d.ring_start();
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + 12 * ((start + count - 1) % 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

}  // namespace title_screen
