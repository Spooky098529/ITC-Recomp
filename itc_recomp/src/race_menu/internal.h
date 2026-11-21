// The race menu's shared helpers (src/race_menu/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "race_menu/race_menu.h"
#include "schema/boot_mode.h"
#include "schema/career.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/racer_controllers.h"
#include "schema/race_menu.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"

namespace race_menu {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The views (schema/race-menu.toml, menu-helpers.toml).
using Machine = schema::RaceMenu;
using G = schema::RaceMenuGlobals;
using Pages = schema::PageObject;
using Menu = schema::FrontEndMenu;
using Event = schema::RaceMenuEvent;
using EventKind = schema::RaceMenuEventKind;
// Other areas' views: player 0's save data (race-event.toml; its +0x199E0
// through career.toml's CareerSave), a UI movie node (tuning-screen.toml),
// a mode message (game-modes.toml).
using Save = schema::SaveData;
using CareerSave = schema::CareerSave;
using Node = schema::UiMovieNode;
using Message = schema::ModeMessage;
// A player's mapper entry (racer-controllers.toml).
using Entry = schema::PlayerMapperEntry;

// The machine's event pairs (+9852) and kind pairs (+9980) by index.
inline Event EventAt(uint8_t* base, uint32_t machine, uint32_t i) {
  return Event(base, Machine(base, machine).events_address() + 8 * i);
}
inline EventKind EventKindAt(uint8_t* base, uint32_t machine, uint32_t i) {
  return EventKind(base, Machine(base, machine).event_kinds_address() + 8 * i);
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


constexpr uint32_t kSoundPlay = 0x82175850;  // Sound::Play, through its override

// The sound manager's vtable's slots (bytes into it).
constexpr uint32_t kStopCueSlot = 52;       // 13
constexpr uint32_t kIsCuePlayingSlot = 56;  // 14

// The sound manager's slot with r4 = the bank's word, r5 = the cue's.
inline uint64_t CallSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame,
                         uint32_t lr, uint32_t bank, uint32_t cue) {
  const uint32_t manager = G::sound_manager(base);
  return calls.Call(Load(base, Load(base, manager) + slot), frame, lr, manager, bank, cue);
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
inline void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const schema::RaceMenuMode m(base, mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  const Message slot(base, m.messages_address() + 12 * ((start + count - 1) % 50));
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

}  // namespace race_menu
