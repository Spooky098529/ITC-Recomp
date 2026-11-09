// The save screen's shared helpers (src/save_screen/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "save_screen/save_screen.h"
#include "ui_movies/game.h"

namespace save_screen {

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

inline uint32_t Data(uint8_t* base) { return Load(base, kData); }
inline uint32_t Pages(uint8_t* base) { return Load(base, kPages); }
inline uint32_t List(uint8_t* base) { return Load(base, kList); }

// The page block's moves the pages make inline: to `page` after `step`
// then `queued` (-1 for none), +8 the page before unless it was `page`.
inline void Go(uint8_t* base, uint32_t page, uint32_t step, uint32_t queued) {
  const uint32_t pages = Pages(base);
  const uint32_t before = Load(base, pages + 0);
  if (int32_t(before) != int32_t(page)) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 12, step);
  Store(base, pages + 16, queued);
  Store(base, pages + 4, page);
  Store(base, pages + 0, kNone);
}
// Straight to `page`: +8 the page before unless it was `page`, +0 the page,
// +12 and +16 -1.
inline void Jump(uint8_t* base, uint32_t page) {
  const uint32_t pages = Pages(base);
  const uint32_t before = Load(base, pages + 0);
  if (int32_t(before) != int32_t(page)) {
    Store(base, pages + 8, before);
  }
  Store(base, pages + 0, page);
  Store(base, pages + 12, kNone);
  Store(base, pages + 16, kNone);
}
// The data's next message (+12) and the dialog's state after the close
// (+20).
inline void Message(uint8_t* base, uint32_t message, uint32_t state) {
  Store(base, Data(base) + 12, message);
  Store(base, Data(base) + 20, state);
}

// The message {id, 0, 0} posted on the mode's ring (+204: the count +600 up
// by one, the slot (+604 + count - 1) mod 50, 12 bytes).
inline void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t at = Load(base, ring + 604) + count - 1;
  Store(base, ring + 600, count);
  const uint32_t entry = ring + 12 * (at % 50);
  Store(base, entry + 8, 0);
  Store(base, entry + 4, 0);
  Store(base, entry + 0, id);
}

// UiMovies::Find(id) as the originals make it: the id stored at `key` (a
// frame word), the movie table's pair for it, its movie.
inline uint32_t FindMovie(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

}  // namespace save_screen
