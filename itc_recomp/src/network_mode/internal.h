// The type 12 mode's shared helpers (src/network_mode/ only).

#pragma once

#include <cstdint>

#include "guest.h"
#include "network_mode/network_mode.h"

namespace network_mode {

inline uint32_t Load(uint8_t* base, uint32_t at) { return guest::LoadU32(base, at); }
inline void Store(uint8_t* base, uint32_t at, uint32_t value) { guest::StoreU32(base, at, value); }

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// Posts the message (id, 0, 0) on the ring at `ring` (its count +600, start
// +604, 50 slots of 12 bytes), as the originals do inline.
inline void PostRing(uint8_t* base, uint32_t ring, uint32_t id) {
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

}  // namespace network_mode
