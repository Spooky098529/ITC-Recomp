// The render command replay's single commands
// (docs/functions/single-commands.md): see single_commands.h.

#include "graphics/single_commands.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/device_state.h"
#include "helpers/debug_output.h"

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

uint32_t SetShaderConstants(uint8_t* base, uint32_t slot, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258F0F0 Single::SetShaderConstants");
  const uint32_t frame = Frame(base, sp, 224);
  const uint32_t data = slot + 16;
  const int32_t shader = int32_t(guest::LoadU32(base, slot + 4));
  if (shader != 0 && shader != 1) {
    return 0;
  }
  const bool vertex = shader == 0;
  // The bool constants: the vertex shader's setter (sub_822A8418, not seen
  // in play) a guest call, the pixel shader's its plain C++.
  auto set_bools = [&](uint32_t values, uint32_t count) {
    const uint32_t first = guest::LoadU8(base, slot + 12);
    const uint32_t device = guest::LoadU32(base, kDevice);
    if (vertex) {
      call(0x822A8418, frame, 0x8258F250, {device, first, values, count});
    } else {
      device_state::SetPixelShaderConstantB(base, device, first, values, count);
    }
  };
  switch (guest::LoadU32(base, slot + 8)) {
    case 0: {  // the loop constants (sub_822A84C8, sub_822A8518): not seen in play
      const uint32_t count = guest::LoadU8(base, slot + 13);
      const uint32_t first = guest::LoadU8(base, slot + 12);
      const uint32_t device = guest::LoadU32(base, kDevice);
      call(vertex ? 0x822A84C8 : 0x822A8518, frame, vertex ? 0x8258F160 : 0x8258F2BC,
           {device, first, data, count});
      break;
    }
    case 1: {  // the floats, with the dirty mask of the 4-register blocks
      const uint32_t first = guest::LoadU8(base, slot + 12);
      const uint32_t count = guest::LoadU8(base, slot + 13);
      const uint32_t device = guest::LoadU32(base, kDevice);
      const uint32_t last_block = uint32_t(first + count - 1) >> 2;
      const uint32_t first_block = first >> 2;
      uint32_t shift = (last_block - first_block) & 0x7F;
      if (shift > 63) shift = 63;
      const int64_t ones = int64_t(0x8000000000000000ull) >> shift;
      const uint64_t dirty = (first_block & 0x40) ? 0 : (uint64_t(ones) >> (first_block & 0x7F));
      if (vertex) {
        device_state::SetVertexShaderConstantF(base, device, first, data, count, dirty);
      } else {
        device_state::SetPixelShaderConstantF(base, device, first, data, count, dirty);
      }
      break;
    }
    case 2:
      set_bools(data, guest::LoadU8(base, slot + 13));
      break;
    case 3:  // OutputDebugStringA("aaaa...\n") (0x8206CB64), which goes nowhere
      helpers::DebugOutput(nullptr);
      break;
    case 4: {  // a bit mask expanded into a word a bit at the frame's +80
      const uint32_t count = guest::LoadU32(base, data + 4);
      const uint32_t mask = guest::LoadU32(base, data + 0);
      if (count != 0) {
        uint32_t i = 0;
        uint32_t at = frame + 80;
        do {
          const uint32_t shift = i & 0x3F;
          const uint32_t bit = (shift & 0x20) ? 0 : (1u << shift);
          i++;
          guest::StoreU32(base, at, (bit & mask) != 0 ? 1 : 0);
          at += 4;
        } while (i < count);
      }
      set_bools(frame + 80, count);
      break;
    }
    default:  // above 4: nothing
      break;
  }
  return guest::LoadU8(base, slot + 13);
}

uint32_t DrawerDraw(uint8_t* base, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258F5C0 Single::DrawerDraw");
  const uint32_t frame = Frame(base, sp, 112);
  if (guest::LoadU32(base, guest::LoadU32(base, kDrawerState) + 16) == 0) {
    return 0;
  }
  const uint32_t viewport = device_state::GetViewportEnable(base, guest::LoadU32(base, kDevice));
  {
    const uint32_t state = guest::LoadU32(base, kDrawerState);
    const uint32_t device = guest::LoadU32(base, kDevice);
    const uint32_t on = int32_t(guest::LoadU32(base, state + 4)) == 1 ? 0 : 1;
    device_state::SetViewportEnable(base, device, on);
  }
  {
    const uint32_t drawer = guest::LoadU32(base, kDrawer);
    const uint32_t state = guest::LoadU32(base, kDrawerState);
    const uint32_t device = guest::LoadU32(base, kDevice);
    const uint32_t vtable = guest::LoadU32(base, drawer);
    const uint32_t count = guest::LoadU32(base, state + 16);
    const uint32_t vertices = guest::LoadU32(base, state + 12);
    const uint32_t type = guest::LoadU32(base, state + 0);
    const uint32_t draw = guest::LoadU32(base, vtable + 12);
    call(draw, frame, 0x8258F640, {drawer, device, type, vertices, count});
  }
  {
    const uint32_t device = guest::LoadU32(base, kDevice);
    if (!device_state::SetStreamSource(base, device, 0, 0, 0, 0, 4096)) {
      call(0x822A3758, frame, 0x8258F65C, {device, 0, 0, 0, 0, 4096});
    }
  }
  {
    const uint32_t device = guest::LoadU32(base, kDevice);
    constexpr uint64_t kStage0 = 1ull << 43;
    if (!device_state::SetTexture(base, device, 0, 0, kStage0)) {
      call(0x822A6008, frame, 0x8258F674, {device, 0, 0, kStage0});
    }
  }
  device_state::SetViewportEnable(base, guest::LoadU32(base, kDevice), viewport);
  return 0;
}

uint32_t Callback(uint8_t* base, uint32_t slot, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E9B8 Single::Callback");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t object = guest::LoadU32(base, slot + 4);
  const uint32_t device = guest::LoadU32(base, kDevice);
  const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object));
  call(function, frame, 0x8258E9E0, {object, device});
  return 0;
}

}  // namespace graphics::single_commands
