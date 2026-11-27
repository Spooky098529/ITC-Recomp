// The render command replay's single commands
// (docs/functions/single-commands.md): see single_commands.h.

#include "render/state.h"
#include "graphics/single_commands.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/device_state.h"
#include "helpers/debug_output.h"
#include "schema/device_shaders.h"
#include "schema/model_animation.h"
#include "schema/single_commands.h"

#include <cstdint>

namespace graphics::single_commands {

namespace {

using Globals = schema::DeviceShadersGlobals;

// A frame of `bytes` below `sp`, with its back chain.
uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  guest::StoreU32(frame, sp);
  return frame;
}

}  // namespace

uint32_t InlineVertices(uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_8258E8E8 Single::InlineVertices");
  const schema::Single single(slot);
  const uint32_t count = single.argument0();
  const uint32_t state = Globals::replay_drawer_state();
  guest::StoreU32(state + 12, single.data_address());
  guest::StoreU32(state + 16, count);
  return uint32_t(count * 24u + 15u) >> 4;
}

uint32_t SetUpDrawer(uint32_t slot, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E958 Single::SetUpDrawer");
  const uint32_t frame = Frame(sp, 96);
  const schema::Single single(slot);
  const uint32_t vertex_shader = single.argument1();
  const uint32_t pixel_shader = single.argument2();
  const uint32_t type = single.argument0();
  const uint32_t state = Globals::replay_drawer_state();
  const uint32_t drawer = Globals::replay_drawer();
  const uint32_t device = Globals::replay_device();
  guest::StoreU32(state + 0, type);
  guest::StoreU32(state + 4, vertex_shader);
  guest::StoreU32(state + 8, pixel_shader);
  const uint32_t set_shaders = guest::LoadU32(schema::PrimitiveDrawer(drawer).vtable() + 8);
  call(set_shaders, frame, 0x8258E9A4, {drawer, device, vertex_shader, pixel_shader});
  return 0;
}

uint32_t SetShaderConstants(uint32_t slot, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258F0F0 Single::SetShaderConstants");
  const uint32_t frame = Frame(sp, 224);
  const schema::Single single(slot);
  const uint32_t data = single.data_address();
  const int32_t shader = int32_t(single.argument0());
  if (shader != 0 && shader != 1) {
    return 0;
  }
  const bool vertex = shader == 0;
  // The bool constants: the vertex shader's setter (sub_822A8418, not seen
  // in play) a guest call, the pixel shader's its plain C++.
  auto set_bools = [&](uint32_t values, uint32_t count) {
    const uint32_t first = single.first_register();
    const uint32_t device = Globals::replay_device();
    if (vertex) {
      call(0x822A8418, frame, 0x8258F250, {device, first, values, count});
    } else {
      device_state::SetPixelShaderConstantB(device, first, values, count);
    }
  };
  switch (single.argument1()) {
    case 0: {  // the loop constants (sub_822A84C8, sub_822A8518): not seen in play
      const uint32_t count = single.register_count();
      const uint32_t first = single.first_register();
      const uint32_t device = Globals::replay_device();
      call(vertex ? 0x822A84C8 : 0x822A8518, frame, vertex ? 0x8258F160 : 0x8258F2BC,
           {device, first, data, count});
      break;
    }
    case 1: {  // the floats, with the dirty mask of the 4-register blocks
      const uint32_t first = single.first_register();
      const uint32_t count = single.register_count();
      const uint32_t device = Globals::replay_device();
      const uint32_t last_block = uint32_t(first + count - 1) >> 2;
      const uint32_t first_block = first >> 2;
      uint32_t shift = (last_block - first_block) & 0x7F;
      if (shift > 63) shift = 63;
      const int64_t ones = int64_t(0x8000000000000000ull) >> shift;
      const uint64_t dirty = (first_block & 0x40) ? 0 : (uint64_t(ones) >> (first_block & 0x7F));
      if (vertex) {
        device_state::SetVertexShaderConstantF(device, first, data, count, dirty);
      } else {
        device_state::SetPixelShaderConstantF(device, first, data, count, dirty);
      }
      break;
    }
    case 2:
      set_bools(data, single.register_count());
      break;
    case 3:  // OutputDebugStringA("aaaa...\n") (0x8206CB64), which goes nowhere
      helpers::DebugOutput(nullptr);
      break;
    case 4: {  // a bit mask expanded into a word a bit at the frame's +80
      const uint32_t count = single.data_4();
      const uint32_t mask = single.data();
      if (count != 0) {
        uint32_t i = 0;
        uint32_t at = frame + 80;
        do {
          const uint32_t shift = i & 0x3F;
          const uint32_t bit = (shift & 0x20) ? 0 : (1u << shift);
          i++;
          guest::StoreU32(at, (bit & mask) != 0 ? 1 : 0);
          at += 4;
        } while (i < count);
      }
      set_bools(frame + 80, count);
      break;
    }
    default:  // above 4: nothing
      break;
  }
  return single.register_count();
}

uint32_t DrawerDraw(uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258F5C0 Single::DrawerDraw");
  const uint32_t frame = Frame(sp, 112);
  if (guest::LoadU32(Globals::replay_drawer_state() + 16) == 0) {
    return 0;
  }
  const uint32_t viewport = device_state::GetViewportEnable(Globals::replay_device());
  const bool render_viewport = render::States().viewport_enable;
  {
    const uint32_t state = Globals::replay_drawer_state();
    const uint32_t device = Globals::replay_device();
    const uint32_t on = int32_t(guest::LoadU32(state + 4)) == 1 ? 0 : 1;
    device_state::SetViewportEnable(device, on);
    render::SetRenderState(304, on);
  }
  {
    const uint32_t drawer = Globals::replay_drawer();
    const uint32_t state = Globals::replay_drawer_state();
    const uint32_t device = Globals::replay_device();
    const uint32_t vtable = schema::PrimitiveDrawer(drawer).vtable();
    const uint32_t count = guest::LoadU32(state + 16);
    const uint32_t vertices = guest::LoadU32(state + 12);
    const uint32_t type = guest::LoadU32(state + 0);
    const uint32_t draw = guest::LoadU32(vtable + 12);
    call(draw, frame, 0x8258F640, {drawer, device, type, vertices, count});
  }
  {
    const uint32_t device = Globals::replay_device();
    if (!device_state::SetStreamSource(device, 0, 0, 0, 0, 4096)) {
      call(0x822A3758, frame, 0x8258F65C, {device, 0, 0, 0, 0, 4096});
    }
  }
  {
    const uint32_t device = Globals::replay_device();
    constexpr uint64_t kStage0 = 1ull << 43;
    if (!device_state::SetTexture(device, 0, 0, kStage0)) {
      call(0x822A6008, frame, 0x8258F674, {device, 0, 0, kStage0});
    }
  }
  device_state::SetViewportEnable(Globals::replay_device(), viewport);
  render::SetRenderState(304, render_viewport ? 1 : 0);
  return 0;
}

uint32_t Callback(uint32_t slot, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258E9B8 Single::Callback");
  const uint32_t frame = Frame(sp, 96);
  const uint32_t object = schema::Single(slot).argument0();
  const uint32_t device = Globals::replay_device();
  // In play the movie, in host memory (E5 step 4): its vtable word through
  // guest::VtableWord (a handle or a guest address).
  const uint32_t function =
      schema::SingleCallbackVtable(guest::VtableWord(object)).call();
  call(function, frame, 0x8258E9E0, {object, device});
  return 0;
}

}  // namespace graphics::single_commands
