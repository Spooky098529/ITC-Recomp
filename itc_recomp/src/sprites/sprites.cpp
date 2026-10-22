#include "sprites/sprites.h"

#include "guest.h"

#include <bit>

namespace sprites {

namespace {

constexpr uint32_t kOne = 0x82000A30;  // the float 1.0 the game stores as w

// The word an `lfs` then `stfs` of `address` stores (through a double, as
// the original's registers hold floats).
uint32_t FloatThroughDouble(uint8_t* base, uint32_t address) {
  float value = std::bit_cast<float>(guest::LoadU32(base, address));
  volatile double through = value;
  return std::bit_cast<uint32_t>(float(through));
}

}  // namespace

void ResetSprites(uint8_t* base, uint32_t sprite, uint32_t corners) {
  const uint32_t w = FloatThroughDouble(base, kOne);
  for (uint32_t row = 0; row < 4; row++) {
    guest::StoreU32(base, sprite + 16 * row + 12, w);
  }
  guest::StoreU32(base, sprite + 64, 0);
  guest::StoreU32(base, sprite + 68, 0);
  guest::StoreU32(base, sprite + 72, 0);
  // The identity, by `stvx` (16-byte aligned), over the w's above when the
  // sprite is aligned.
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    const uint32_t at = (sprite + 16 * row) & ~0xFu;
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, at + 4 * column, row == column ? kOneBits : 0);
    }
  }
  for (uint32_t i = 0; i < corners; i++) {
    guest::StoreU32(base, sprite + 92 + 16 * i, w);
  }
}

uint32_t LayerFor(uint32_t pass_index) {
  switch (pass_index) {
    case 2:
      return 491;
    case 3:
      return 53;
    default:
      return 270;
  }
}

bool Equal4(const float a[4], const float b[4]) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}

}  // namespace sprites
