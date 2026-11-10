// The race menu's shared helpers (src/race_menu/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "race_menu/race_menu.h"
#include "ui_movies/game.h"

namespace race_menu {

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kNone = 0xFFFFFFFFu;

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

// The fades' counts (both 0 once the fades are done).
constexpr uint32_t kFadeCountA = 0x826A50E4;
constexpr uint32_t kFadeCountB = 0x826A50DC;

constexpr uint32_t kSoundPlay = 0x82175850;  // Sound::Play, through its override

// The sound manager and its vtable's slots (bytes into it).
constexpr uint32_t kSoundManager = 0x826C5C80;
constexpr uint32_t kStopCueSlot = 52;       // 13
constexpr uint32_t kIsCuePlayingSlot = 56;  // 14

// The sound manager's slot with r4 = the bank's word, r5 = the cue's.
inline uint64_t CallSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame,
                         uint32_t lr, uint32_t bank, uint32_t cue) {
  const uint32_t manager = Load(base, kSoundManager);
  return calls.Call(Load(base, Load(base, manager) + slot), frame, lr, manager, bank, cue);
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
inline void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

}  // namespace race_menu
