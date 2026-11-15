// The render command replay's command handlers: see replay_commands.h.

#include "graphics/replay_commands.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/device_state.h"
#include "graphics/resources.h"
#include "helpers/debug_output.h"

#include <cstdint>

namespace graphics::replay_commands {

namespace {

namespace ds = device_state;

constexpr uint32_t kDevice = 0x8296BD90;
constexpr uint32_t kReleaseList = 0x8296BD8C;
constexpr uint32_t kSavedSurfaces = 0x8296BD60;  // 34 and 35's: the viewport, +24..+40, +48 the device
constexpr uint32_t kDrawer = 0x82970BB8;
constexpr uint32_t kDrawerState = 0x82970BBC;
constexpr uint32_t kRestoreSet = 0x82970BC0;     // 29's saved render targets and depth surface
constexpr uint32_t kPrimitiveCounts = 0x82000950;  // per type: multiplier, addend
constexpr uint32_t kByteToFloat = 0x82002200;    // 25's clear color scale (1/255)
constexpr uint32_t kTilingFloat = 0x82000A34;    // 32 and 33's clear values and Z

// A frame of `bytes` below `sp`, with its back chain.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  guest::StoreU32(base, frame, sp);
  return frame;
}

uint32_t Device(uint8_t* base) { return guest::LoadU32(base, kDevice); }

// 1 << 63 shifted right by the low 7 bits of `shift` (srd: 0 from 64 on).
uint64_t HighBitShifted(uint32_t shift) {
  shift &= 0x7F;
  return shift > 63 ? 0 : (0x8000000000000000ull >> shift);
}

// The vertex or index count for `primitives` of a primitive type.
uint32_t PrimitiveCount(uint8_t* base, uint32_t type, uint32_t primitives) {
  const uint32_t entry = kPrimitiveCounts + type * 8;
  return guest::LoadU32(base, entry) * primitives + guest::LoadU32(base, entry + 4);
}

}  // namespace

uint32_t SetRenderStates(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FBD0 Command::SetRenderStates");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t state = guest::LoadU32(base, item);
    const uint32_t device = Device(base);
    const uint32_t value = guest::LoadU32(base, item + 4);
    const uint32_t setter = guest::LoadU32(base, device + state + 56);
    call(setter, frame, 0x8258FC10, {device, value}, std::nullopt);
  }
  return count;
}

uint32_t SetSamplerStates(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FC30 Command::SetSamplerStates");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t state = guest::LoadU32(base, item + 4);
    const uint32_t device = Device(base);
    const uint32_t value = guest::LoadU32(base, item + 8);
    const uint32_t sampler = guest::LoadU32(base, item);
    const uint32_t setter = guest::LoadU32(base, device + state + 440);
    call(setter, frame, 0x8258FC74, {device, sampler, value}, std::nullopt);
  }
  return count;
}

uint32_t SetBlendControls(uint8_t* base, uint32_t items, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8258FC90 Command::SetBlendControls");
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t value = guest::LoadU32(base, item + 4);
    ds::SetBlendControl(base, Device(base), guest::LoadU32(base, item), value);
  }
  return count;
}

uint32_t SetTextures(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FCE0 Command::SetTextures");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t stage = guest::LoadU32(base, item);
    const uint32_t device = Device(base);
    const uint32_t texture = guest::LoadU32(base, item + 4);
    const uint64_t dirty = HighBitShifted(stage + 20);
    if (!ds::SetTexture(base, device, stage, texture, dirty)) {
      call(0x822A6008, frame, 0x8258FD28, {device, stage, texture, dirty}, std::nullopt);
    }
  }
  return count;
}

uint32_t InlineVertices(uint8_t* base, uint32_t items, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8258E9F8 Command::InlineVertices");
  const uint32_t state = guest::LoadU32(base, kDrawerState);
  guest::StoreU32(base, state + 12, items);
  guest::StoreU32(base, state + 16, count);
  return uint32_t(count * 24u + 15u) >> 4;
}

uint32_t InlineIndices(uint8_t* base, uint32_t items, uint32_t count, uint32_t third) {
  ITC_NATIVE_RUNNING("sub_8258EA20 Command::InlineIndices");
  const uint32_t size = third != 0 ? 4 : 2;
  const uint32_t state = guest::LoadU32(base, kDrawerState);
  guest::StoreU32(base, state + 20, items);
  guest::StoreU32(base, state + 24, count);
  guest::StoreU32(base, state + 28, third);
  return uint32_t(size * count + 15u) >> 4;
}

uint32_t SetShaderConstants(uint8_t* base, uint32_t payload, uint32_t count, uint32_t packed,
                            uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FD48 Command::SetShaderConstants");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t stage = int32_t(packed) >> 24;
  if (stage != 0 && stage != 1) {
    return 0;
  }
  const bool vertex = stage == 0;
  const uint32_t type = uint32_t(int32_t(packed << 8) >> 24);
  const uint32_t first = (packed >> 8) & 0xFF;
  switch (type) {
    case 0:  // the loop constants (sub_822A84C8, sub_822A8518)
      call(vertex ? 0x822A84C8 : 0x822A8518, frame, vertex ? 0x8258FDB8 : 0x8258FED4,
           {Device(base), first, payload, count}, std::nullopt);
      break;
    case 1: {  // the floats, with the dirty mask of their 4-register blocks
      const uint32_t last_block = uint32_t(first + count - 1) >> 2;
      const uint32_t first_block = first >> 2;
      uint32_t shift = (last_block - first_block) & 0x7F;
      if (shift > 63) shift = 63;
      const int64_t ones = int64_t(0x8000000000000000ull) >> shift;
      const uint64_t dirty = (first_block & 0x40) ? 0 : (uint64_t(ones) >> (first_block & 0x3F));
      if (vertex) {
        ds::SetVertexShaderConstantF(base, Device(base), first, payload, count, dirty);
      } else {
        ds::SetPixelShaderConstantF(base, Device(base), first, payload, count, dirty);
      }
      break;
    }
    case 2:  // the bools: the vertex shader's setter (sub_822A8418) a guest call
      if (vertex) {
        call(0x822A8418, frame, 0x8258FE40, {Device(base), first, payload, count},
             std::nullopt);
      } else {
        ds::SetPixelShaderConstantB(base, Device(base), first, payload, count);
      }
      break;
    case 3:  // OutputDebugStringA("aaaa...\n") (0x8206CB64), which goes nowhere
      helpers::DebugOutput(nullptr);
      break;
    default:  // above 3 (unsigned): nothing
      break;
  }
  return count;
}

uint32_t SetVertexShaders(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FF50 Command::SetVertexShaders");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    call(0x822A8958, frame, 0x8258FF80, {Device(base), guest::LoadU32(base, item)},
         std::nullopt);
  }
  return count;
}

uint32_t SetPixelShaders(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FFA0 Command::SetPixelShaders");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    call(0x822A8618, frame, 0x8258FFD0, {Device(base), guest::LoadU32(base, item)},
         std::nullopt);
  }
  return count;
}

uint32_t SetVertexDeclarations(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258FFF0 Command::SetVertexDeclarations");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    call(0x822A8960, frame, 0x82590020, {Device(base), guest::LoadU32(base, item)},
         std::nullopt);
  }
  return count;
}

uint32_t SetStreamSources(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590040 Command::SetStreamSources");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t stream = guest::LoadU32(base, item);
    const uint32_t device = Device(base);
    const uint32_t stride = guest::LoadU32(base, item + 12);
    const uint32_t offset = guest::LoadU32(base, item + 8);
    const uint32_t buffer = guest::LoadU32(base, item + 4);
    // subfic, mulli 21846, the high halfword (rlwinm 16,16,31), + 20.
    const uint32_t product = (95u - stream) * 21846u;
    const uint64_t dirty = HighBitShifted((product >> 16) + 20);
    if (!ds::SetStreamSource(base, device, stream, buffer, offset, stride, dirty)) {
      call(0x822A3758, frame, 0x8259009C, {device, stream, buffer, offset, stride, dirty},
           std::nullopt);
    }
  }
  return count;
}

uint32_t SetIndices(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825900B8 Command::SetIndices");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t device = Device(base);
    const uint32_t buffer = guest::LoadU32(base, item);
    if (!ds::SetIndices(base, device, buffer)) {
      call(0x822A3908, frame, 0x825900E8, {device, buffer}, std::nullopt);
    }
  }
  return count;
}

uint32_t SetUpDrawer(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258EA58 Command::SetUpDrawer");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t state = guest::LoadU32(base, kDrawerState);
    const uint32_t vertex_shader = guest::LoadU32(base, item + 4);
    const uint32_t pixel_shader = guest::LoadU32(base, item + 8);
    const uint32_t type = guest::LoadU32(base, item);
    const uint32_t drawer = guest::LoadU32(base, kDrawer);
    const uint32_t device = Device(base);
    guest::StoreU32(base, state + 4, vertex_shader);
    guest::StoreU32(base, state + 8, pixel_shader);
    guest::StoreU32(base, state + 0, type);
    const uint32_t set_shaders = guest::LoadU32(base, guest::LoadU32(base, drawer) + 8);
    call(set_shaders, frame, 0x8258EAB4, {drawer, device, vertex_shader, pixel_shader},
         std::nullopt);
  }
  return count;
}

uint32_t DrawerDraw(uint8_t* base, uint32_t count, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590108 Command::DrawerDraw");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0; i < count; i++) {
    if (guest::LoadU32(base, guest::LoadU32(base, kDrawerState) + 16) == 0) {
      continue;
    }
    ds::GetViewportEnable(base, Device(base));  // read, and not kept
    {
      const uint32_t state = guest::LoadU32(base, kDrawerState);
      const uint32_t on = int32_t(guest::LoadU32(base, state + 4)) == 1 ? 0 : 1;
      ds::SetViewportEnable(base, Device(base), on);
    }
    {
      const uint32_t drawer = guest::LoadU32(base, kDrawer);
      const uint32_t state = guest::LoadU32(base, kDrawerState);
      const uint32_t device = Device(base);
      const uint32_t draw = guest::LoadU32(base, guest::LoadU32(base, drawer) + 12);
      call(draw, frame, 0x8259018C,
           {drawer, device, guest::LoadU32(base, state + 0), guest::LoadU32(base, state + 12),
            guest::LoadU32(base, state + 16)},
           std::nullopt);
    }
    ds::SetViewportEnable(base, Device(base), 1);
  }
  return count;
}

uint32_t DrawerDrawIndexed(uint8_t* base, uint32_t count, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825901B0 Command::DrawerDrawIndexed");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0; i < count; i++) {
    if (guest::LoadU32(base, guest::LoadU32(base, kDrawerState) + 16) == 0) {
      continue;
    }
    const uint32_t viewport = ds::GetViewportEnable(base, Device(base));
    {
      const uint32_t state = guest::LoadU32(base, kDrawerState);
      const uint32_t on = int32_t(guest::LoadU32(base, state + 4)) == 1 ? 0 : 1;
      ds::SetViewportEnable(base, Device(base), on);
    }
    {
      const uint32_t drawer = guest::LoadU32(base, kDrawer);
      const uint32_t state = guest::LoadU32(base, kDrawerState);
      const uint32_t device = Device(base);
      const uint32_t draw = guest::LoadU32(base, guest::LoadU32(base, drawer) + 16);
      call(draw, frame, 0x82590244,
           {drawer, device, guest::LoadU32(base, state + 0), guest::LoadU32(base, state + 12),
            guest::LoadU32(base, state + 16), guest::LoadU32(base, state + 20),
            guest::LoadU32(base, state + 24), guest::LoadU32(base, state + 28)},
           std::nullopt);
    }
    ds::SetViewportEnable(base, Device(base), viewport);
  }
  return count;
}

uint32_t DrawVertices(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590268 Command::DrawVertices");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t type = guest::LoadU32(base, item);
    const uint32_t start = guest::LoadU32(base, item + 4);
    const uint32_t vertices = PrimitiveCount(base, type, guest::LoadU32(base, item + 8));
    call(0x825BE300, frame, 0x825902C0, {Device(base), type, start, vertices}, std::nullopt);
  }
  return count;
}

uint32_t DrawIndexedVertices(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                             const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825902E0 Command::DrawIndexedVertices");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 12) {
    const uint32_t type = guest::LoadU32(base, item);
    const uint32_t word = guest::LoadU32(base, item + 4);
    const uint32_t indices = PrimitiveCount(base, type, guest::LoadU32(base, item + 8));
    const uint64_t base_vertex = uint64_t(int64_t(int32_t(word) >> 16));  // srawi
    call(0x825BE708, frame, 0x82590340, {Device(base), type, base_vertex, word & 0xFFFF, indices},
         std::nullopt);
  }
  return count;
}

uint32_t DrawVerticesUP(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590360 Command::DrawVerticesUP");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 12) {
    const uint32_t vertices = guest::LoadU16(base, item + 4);
    const uint32_t stride = guest::LoadU16(base, item + 6);
    const uint32_t type = guest::LoadU32(base, item);
    const uint32_t data = uint32_t(
        call(0x825BD8F0, frame, 0x82590398, {Device(base), type, vertices, stride}, std::nullopt));
    if (data == 0) {
      continue;
    }
    const uint32_t bytes = guest::LoadU16(base, item + 6) * guest::LoadU16(base, item + 4);
    call(0x825A7410, frame, 0x825903B4, {data, guest::LoadU32(base, item + 8), bytes},
         std::nullopt);
    call(0x825BDDA0, frame, 0x825903BC, {Device(base)}, std::nullopt);
  }
  return count;
}

uint32_t ReleaseResources(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825903D8 Command::ReleaseResources");
  const uint32_t frame = Frame(base, sp, 112);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    if (const uint32_t resource = guest::LoadU32(base, item)) {
      call(0x822A67B0, frame, 0x82590408, {resource}, std::nullopt);
    }
  }
  return count;
}

uint32_t Skip(uint32_t count) {
  ITC_NATIVE_RUNNING("sub_8258EAD0 Command::Skip");
  return uint32_t(count + 15u) >> 4;
}

uint32_t DeferRelease(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82591798 Command::DeferRelease");
  const uint32_t frame = Frame(base, sp, 128);
  // Slot `slot` of the object at `entry` (r3 the object only: the
  // original's other registers are what they are, as here).
  auto slot = [&](uint32_t entry, uint32_t slot_offset, uint32_t return_address) {
    const uint32_t object = guest::LoadU32(base, entry);
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object) + slot_offset);
    return call(function, frame, return_address, {object}, std::nullopt);
  };
  for (uint32_t i = 0, entry = items + 4; i < count; i++, entry += 16) {
    if ((slot(entry, 8, 0x825917D0) & 0xFF) != 0) {
      continue;
    }
    if (guest::LoadU32(base, entry + 4) != 1) {
      continue;
    }
    slot(entry, 4, 0x825917FC);
    call(0x82591540, frame, 0x82591808, {guest::LoadU32(base, kReleaseList), entry},
         std::nullopt);
    slot(entry, 12, 0x8259181C);
  }
  return count;
}

uint32_t SetRenderTargets(uint8_t* base, uint32_t items, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82590428 Command::SetRenderTargets");
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t surface = guest::LoadU32(base, item + 4);
    ds::SetRenderTargetExternal(base, Device(base), guest::LoadU32(base, item), surface);
  }
  return count;
}

uint32_t SetDepthStencilSurfaces(uint8_t* base, uint32_t items, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82590478 Command::SetDepthStencilSurfaces");
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    ds::SetDepthStencilSurface(base, Device(base), guest::LoadU32(base, item));
  }
  return count;
}

uint32_t Resolves(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825904C8 Command::Resolves");
  const uint32_t frame = Frame(base, sp, 224);
  const float scale = guest::LoadFloat(base, kByteToFloat);
  for (uint32_t i = 0, item = items; i < count; i++, item += 68) {
    // The clear color's bytes as floats times the scale, at the frame's
    // +144 (whether or not the flags pass it).
    const uint32_t bytes[4] = {guest::LoadU8(base, item + 44), guest::LoadU8(base, item + 45),
                               guest::LoadU8(base, item + 46), guest::LoadU8(base, item + 47)};
    for (int c = 0; c < 4; c++) {
      guest::StoreFloat(base, frame + 144 + 4 * c, float(double(bytes[c])) * scale);
    }
    const uint32_t which = guest::LoadU32(base, item);
    const uint32_t source_rect = (which & 1) ? item + 8 : 0;
    const uint32_t texture = (which & 2) ? guest::LoadU32(base, item + 24) : 0;
    const uint32_t point = (which & 4) ? item + 28 : 0;
    const uint32_t color = (which & 8) ? frame + 144 : 0;
    const uint32_t parameters = (which & 16) ? item + 56 : 0;
    const uint32_t word52 = guest::LoadU32(base, item + 52);
    const double z = double(guest::LoadFloat(base, item + 48));
    const uint32_t slice = guest::LoadU32(base, item + 40);
    const uint32_t level = guest::LoadU32(base, item + 36);
    const uint32_t device = Device(base);
    const uint32_t flags = guest::LoadU32(base, item + 4);
    guest::StoreU32(base, frame + 100, parameters);
    guest::StoreU32(base, frame + 92, word52);
    call(0x822AFA58, frame, 0x825905F4,
         {device, flags, source_rect, texture, point, level, slice, color}, z);
  }
  return uint32_t(count * 68u + 15u) >> 4;
}

uint32_t Clears(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590620 Command::Clears");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 280) {
    const uint32_t rects = guest::LoadU32(base, item + 4);
    const uint32_t rect_data = rects != 0 ? item + 12 : 0;
    const uint32_t stencil = guest::LoadU32(base, item + 276);
    const double z = double(guest::LoadFloat(base, item + 272));
    const uint32_t color = guest::LoadU32(base, item + 268);
    const uint32_t device = Device(base);
    const uint32_t flags = guest::LoadU32(base, item + 8);
    // r8 isn't set by the original (Clear doesn't read it): 0 here.
    call(0x822A04F8, frame, 0x82590674, {device, rects, rect_data, flags, color, 0, stencil, 0},
         z);
  }
  return uint32_t(count * 280u + 15u) >> 4;
}

uint32_t DrawIndexedVerticesUP(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590698 Command::DrawIndexedVerticesUP");
  const uint32_t frame = Frame(base, sp, 160);
  for (uint32_t i = 0, item = items; i < count; i++, item += 32) {
    const uint32_t index_format = guest::LoadU32(base, item + 16);
    const uint32_t vertex_stride = guest::LoadU32(base, item + 20);
    guest::StoreU32(base, frame + 96, 0);
    guest::StoreU32(base, frame + 100, 0);
    guest::StoreU32(base, frame + 84, frame + 100);
    const uint32_t result = uint32_t(call(
        0x825BDDC0, frame, 0x825906F4,
        {Device(base), guest::LoadU32(base, item), guest::LoadU32(base, item + 4),
         guest::LoadU32(base, item + 8), guest::LoadU32(base, item + 12), index_format,
         vertex_stride, frame + 96},
        std::nullopt));
    if (int32_t(result) != 0) {
      continue;
    }
    const uint32_t index_size = int32_t(guest::LoadU32(base, item + 16)) == 1 ? 2 : 4;
    call(0x825A7410, frame, 0x82590724,
         {guest::LoadU32(base, frame + 96), guest::LoadU32(base, item + 24),
          guest::LoadU32(base, item + 12) * index_size},
         std::nullopt);
    call(0x825A7410, frame, 0x8259073C,
         {guest::LoadU32(base, frame + 100), guest::LoadU32(base, item + 28),
          guest::LoadU32(base, item + 20) * guest::LoadU32(base, item + 8)},
         std::nullopt);
    call(0x825BDDA0, frame, 0x82590744, {Device(base)}, std::nullopt);
  }
  return uint32_t((count << 5) + 15u) >> 4;
}

uint32_t Callbacks(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8258EAE0 Command::Callbacks");
  const uint32_t frame = Frame(base, sp, 128);
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    const uint32_t object = guest::LoadU32(base, item + 4);
    const uint32_t device = Device(base);
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object));
    call(function, frame, 0x8258EB1C, {object, device}, std::nullopt);
  }
  return count;
}

uint32_t RestoreSurfaces(uint8_t* base, uint32_t items, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82590768 Command::RestoreSurfaces");
  for (uint32_t i = 0, item = items; i < count; i++, item += 4) {
    for (uint32_t target = 0; target < 4; target++) {
      if (guest::LoadU32(base, item) & (1u << target)) {
        const uint32_t set = guest::LoadU32(base, kRestoreSet);
        ds::SetRenderTargetExternal(base, Device(base), target,
                                    guest::LoadU32(base, set + 4 * target));
      }
    }
    if (guest::LoadU32(base, item) & 0x80) {
      const uint32_t set = guest::LoadU32(base, kRestoreSet);
      ds::SetDepthStencilSurface(base, Device(base), guest::LoadU32(base, set + 16));
    }
  }
  return count;
}

uint32_t SetViewports(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82590860 Command::SetViewports");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t viewport = frame + 80;
  for (uint32_t i = 0, item = items; i < count; i++, item += 16) {
    guest::StoreFloat(base, viewport + 16, guest::LoadFloat(base, item + 8));
    guest::StoreFloat(base, viewport + 20, guest::LoadFloat(base, item + 12));
    guest::StoreU32(base, viewport + 0, guest::LoadU16(base, item + 0));
    guest::StoreU32(base, viewport + 4, guest::LoadU16(base, item + 2));
    guest::StoreU32(base, viewport + 8, guest::LoadU16(base, item + 4));
    guest::StoreU32(base, viewport + 12, guest::LoadU16(base, item + 6));
    ds::SetViewport(base, Device(base), viewport);
  }
  return count;
}

uint32_t SetScissorRects(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_825908E0 Command::SetScissorRects");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t rect = frame + 80;
  for (uint32_t i = 0, item = items; i < count; i++, item += 8) {
    for (uint32_t c = 0; c < 4; c++) {
      const int32_t value = int16_t(guest::LoadU16(base, item + 2 * c));
      guest::StoreU32(base, rect + 4 * c, uint32_t(value));
    }
    ds::SetScissorTestEnable(base, Device(base), 1);
    ds::SetScissorRect(base, Device(base), rect);
  }
  return count;
}

uint32_t BeginTiling(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590968 Command::BeginTiling");
  const uint32_t frame = Frame(base, sp, 144);
  const float value = guest::LoadFloat(base, kTilingFloat);
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreFloat(base, frame + 80 + 4 * c, value);
  }
  // Each pass takes the first item (the original doesn't step through
  // them); r8 isn't set by the original, nor read by BeginTiling: 0 here.
  for (uint32_t i = 0; i < count; i++) {
    call(0x825BED08, frame, 0x825909C8,
         {Device(base), guest::LoadU32(base, items), guest::LoadU32(base, items + 4),
          guest::LoadU32(base, items + 8), frame + 80, 0, 0},
         double(value));
  }
  return count;
}

uint32_t EndTiling(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825909E8 Command::EndTiling");
  const uint32_t frame = Frame(base, sp, 144);
  const float value = guest::LoadFloat(base, kTilingFloat);
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreFloat(base, frame + 80 + 4 * c, value);
  }
  // As BeginTiling: the first item each pass; r8 unset by the original and
  // not read by EndTiling.
  for (uint32_t i = 0; i < count; i++) {
    call(0x825BF200, frame, 0x82590A4C,
         {Device(base), guest::LoadU32(base, items), 0, guest::LoadU32(base, items + 4),
          frame + 80, 0, 0, 0},
         double(value));
  }
  return count;
}

uint32_t SaveSurfaces(uint8_t* base, uint32_t count, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82590A68 Command::SaveSurfaces");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t saved = kSavedSurfaces;
  {
    const uint32_t device = guest::LoadU32(base, saved + 48);
    if (!ds::GetViewport(base, device, saved)) {
      call(0x822A3740, frame, 0x82590A8C, {device, saved}, std::nullopt);
    }
  }
  for (uint32_t target = 0; target < 4; target++) {
    const uint32_t surface = GetRenderTarget(base, guest::LoadU32(base, saved + 48), target);
    guest::StoreU32(base, saved + 24 + 4 * target, surface);
  }
  guest::StoreU32(base, saved + 40,
                  GetDepthStencilSurface(base, guest::LoadU32(base, saved + 48)));
  return count;
}

uint32_t RestoreSavedSurfaces(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82590AD0 Command::RestoreSavedSurfaces");
  const uint32_t saved = kSavedSurfaces;
  for (uint32_t target = 0; target < 4; target++) {
    const uint32_t surface = guest::LoadU32(base, saved + 24 + 4 * target);
    ds::SetRenderTargetExternal(base, guest::LoadU32(base, saved + 48), target, surface);
  }
  ds::SetDepthStencilSurface(base, guest::LoadU32(base, saved + 48),
                             guest::LoadU32(base, saved + 40));
  ds::SetViewport(base, guest::LoadU32(base, saved + 48), saved);
  return count;
}

}  // namespace graphics::replay_commands
