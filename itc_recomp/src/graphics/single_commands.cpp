// The render command replay's single commands
// (docs/functions/single-commands.md): see single_commands.h.

#include "graphics/single_commands.h"

#include "guest.h"

#include "base/log.h"

#include <cstdint>

namespace graphics::single_commands {

namespace {

constexpr uint32_t kDevice = 0x8296BD90;
constexpr uint32_t kDrawer = 0x82970BB8;
constexpr uint32_t kDrawerState = 0x82970BBC;

// A frame of `bytes` below `sp`, with its back chain.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  guest::StoreU32(base, frame, sp);
  return frame;
}

}  // namespace

uint32_t InlineVertices(uint8_t* base, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_8258E8E8 Single::InlineVertices");
  const uint32_t count = guest::LoadU32(base, slot + 4);
  const uint32_t state = guest::LoadU32(base, kDrawerState);
  guest::StoreU32(base, state + 12, slot + 16);
  guest::StoreU32(base, state + 16, count);
  return uint32_t(count * 24u + 15u) >> 4;
}

uint32_t SetUpDrawer(uint8_t* base, uint32_t slot, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E958 Single::SetUpDrawer");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t vertex_shader = guest::LoadU32(base, slot + 8);
  const uint32_t pixel_shader = guest::LoadU32(base, slot + 12);
  const uint32_t type = guest::LoadU32(base, slot + 4);
  const uint32_t state = guest::LoadU32(base, kDrawerState);
  const uint32_t drawer = guest::LoadU32(base, kDrawer);
  const uint32_t device = guest::LoadU32(base, kDevice);
  guest::StoreU32(base, state + 0, type);
  guest::StoreU32(base, state + 4, vertex_shader);
  guest::StoreU32(base, state + 8, pixel_shader);
  const uint32_t set_shaders = guest::LoadU32(base, guest::LoadU32(base, drawer) + 8);
  call(set_shaders, frame, 0x8258E9A4, {drawer, device, vertex_shader, pixel_shader});
  return 0;
}

}  // namespace graphics::single_commands
