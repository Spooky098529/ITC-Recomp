// The type 12 mode's shared helpers (src/network_mode/ only).

#pragma once

#include <cstdint>

#include "game_modes/game_modes.h"
#include "guest.h"
#include "network_mode/network_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/network_mode.h"

namespace network_mode {

inline uint32_t Load(uint32_t at) { return guest::LoadU32(at); }
inline void Store(uint32_t at, uint32_t value) { guest::StoreU32(at, value); }

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// Posts the message (id, 0, 0) on the ring of the mode's dispatcher at
// `dispatcher` (the mode's +36: 50 slots of 12 bytes), as the originals do
// inline.
inline void PostRing(uint32_t dispatcher, uint32_t id) {
  const schema::NetModeDispatcher d(dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t start = d.ring_start();
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + 12 * ((start + count - 1) % 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

}  // namespace network_mode
