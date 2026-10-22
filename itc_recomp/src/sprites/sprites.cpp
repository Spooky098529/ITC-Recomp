#include "sprites/sprites.h"

#include "guest.h"

#include "command_list/command_list.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse.h>

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

int32_t TurnSide(const float a[4], const float b[4], float zero) {
  if (Equal4(a, b)) {
    return 0;
  }
  using Fpscr = rex::platform::FPSCRPlatform;
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  const simde__m128 x_z = simde_mm_mul_ss(simde_mm_set_ss(a[0]), simde_mm_set_ss(b[2]));
  const simde__m128 z_x = simde_mm_mul_ss(simde_mm_set_ss(a[2]), simde_mm_set_ss(b[0]));
  const float y = -simde_mm_cvtss_f32(simde_mm_sub_ss(x_z, z_x));
  Fpscr::setcsr(csr);
  return y > zero ? 1 : -1;
}

namespace {

// One command slot's words (+0, +4, +8), stored in the original's order.
void Slot3(uint8_t* base, uint32_t slot, uint32_t w0, uint32_t w1, uint32_t w2) {
  guest::StoreU32(base, slot + 0, w0);
  guest::StoreU32(base, slot + 4, w1);
  guest::StoreU32(base, slot + 8, w2);
}
void Slot2(uint8_t* base, uint32_t slot, uint32_t w0, uint32_t w1) {
  guest::StoreU32(base, slot + 0, w0);
  guest::StoreU32(base, slot + 4, w1);
}

// The slots a screen rectangle allocates, in order (the vertices' 6 last but
// three), each checked as AllocateSlots does (32-bit).
constexpr uint32_t kScreenRectAllocations[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                               1, 1, 1, 1, 1, 6, 1, 1, 1};

}  // namespace

std::optional<uint32_t> RecordScreenRect(uint8_t* base, uint32_t list, uint32_t rect) {
  // What the original's command list calls would print about: another chunk
  // open (OpenChunk), or the list full on any allocation.
  if (guest::LoadU8(base, list + 52) == 0 && guest::LoadU32(base, command_list::kOpenChunks) != 0) {
    return std::nullopt;
  }
  {
    uint32_t used = guest::LoadU32(base, list + 12);
    const uint32_t limit = guest::LoadU32(base, list + 8);
    for (uint32_t count : kScreenRectAllocations) {
      used += count;
      if (used >= limit) {
        return std::nullopt;
      }
    }
  }
  command_list::OpenChunk(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };

  const uint32_t chunk = slot();
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  uint32_t s = slot();
  guest::StoreU32(base, s + 8, 0);
  guest::StoreU32(base, s + 0, 4);
  guest::StoreU32(base, s + 4, 1);
  s = slot();
  guest::StoreU32(base, s + 0, 0);
  guest::StoreU32(base, s + 4, guest::LoadU32(base, rect + 112));
  Slot3(base, slot(), 1, 5, 0);
  // The blend states: alpha blending on (+116) or off.
  const bool blend = guest::LoadU32(base, rect + 116) != 0;
  Slot2(base, slot(), 56, 6);
  Slot2(base, slot(), 96, 0);
  Slot2(base, slot(), 60, blend ? 1 : 0);
  Slot2(base, slot(), 72, blend ? 6 : 1);
  Slot2(base, slot(), 76, blend ? 7 : 0);
  Slot3(base, slot(), 2, 2, 0);
  Slot3(base, slot(), 0, 20, 1);
  Slot3(base, slot(), 0, 16, 1);
  Slot3(base, slot(), 13, 1, 0);
  Slot3(base, slot(), 6, 1, 2);
  Slot3(base, slot(), 5, 4, 0);

  // The quad: four 24-byte vertices (x, y, 1.0, white, u, v) at the
  // rectangle's corners, the integers converted to floats.
  const uint32_t v = command_list::AllocateVertices(base, list, 4, false);
  // The floats go through registers (lfs, stfs) in the original, but the
  // translated code stores their words unchanged (a signaling NaN stays one,
  // seen in the harness), so they're copied as words.
  const uint32_t one = guest::LoadU32(base, kOne);
  const int32_t x = int32_t(guest::LoadU32(base, rect + 80));
  const int32_t y = int32_t(guest::LoadU32(base, rect + 84));
  const int32_t width = int32_t(guest::LoadU32(base, rect + 88));
  const int32_t height = int32_t(guest::LoadU32(base, rect + 92));
  const uint32_t x0 = std::bit_cast<uint32_t>(float(x));
  const uint32_t y0 = std::bit_cast<uint32_t>(float(y));
  const uint32_t x1 = std::bit_cast<uint32_t>(float(int32_t(uint32_t(x) + uint32_t(width))));
  const uint32_t y1 = std::bit_cast<uint32_t>(float(int32_t(uint32_t(y) + uint32_t(height))));
  const uint32_t u0 = guest::LoadU32(base, rect + 96);
  const uint32_t v0 = guest::LoadU32(base, rect + 100);
  const uint32_t u1 = guest::LoadU32(base, rect + 104);
  const uint32_t v1 = guest::LoadU32(base, rect + 108);
  const uint32_t corners[4][4] = {{x0, y0, u0, v0}, {x1, y0, u1, v0}, {x0, y1, u0, v1},
                                  {x1, y1, u1, v1}};
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t at = v + 24 * i;
    guest::StoreU32(base, at + 0, corners[i][0]);
    guest::StoreU32(base, at + 4, corners[i][1]);
    guest::StoreU32(base, at + 8, one);
    guest::StoreU32(base, at + 12, 0xFFFFFFFF);
    guest::StoreU32(base, at + 16, corners[i][2]);
    guest::StoreU32(base, at + 20, corners[i][3]);
  }
  Slot3(base, slot(), 15, 1, 0);
  slot();  // allocated and left as it was, as the original does
  Slot3(base, slot(), 36, 0, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

}  // namespace sprites
