// The game's render commands decoded into our interface: see commands.h.
// Payload layouts and what each handler did are in
// docs/functions/render-commands.md; what each render and sampler state
// meant, in the game's setters (src/graphics/device_state.cpp).

#include "render/commands.h"

#include "guest.h"

#include "base/flags.h"
#include "base/log.h"
#include "interpolation/blend_state.h"
#include "native/refcount.h"

#include <algorithm>
#include <bit>
#include <chrono>
#include <cstring>
#include <set>

ITC_FLAG_DECLARE(bool, log_frame_times);

namespace render {

namespace {

// The image's tables (see "Render states and sampler states" in
// docs/gpu-functions.md): 12-byte entries of getter, setter and default.
constexpr uint32_t kRenderStateTable = 0x826388F8;
constexpr uint32_t kRenderStates = 96;
constexpr uint32_t kSamplerStateTable = 0x82638D78;
constexpr uint32_t kSamplerStates = 20;

constexpr uint32_t kOneOver255 = 0x82002200;
constexpr uint32_t kZero = 0x82000A34;  // the tiling's clear color and Z
// Set by SetSurfaceInfo: the whole surface's viewport and scissor.
constexpr uint32_t kSurfaceViewport = 0x82013600;
constexpr uint32_t kSurfaceScissor = 0x82013618;

constexpr uint32_t kDrawer = 0x82970BB8;
constexpr uint32_t kDrawerState = 0x82970BBC;
constexpr uint32_t kSavedTargets = 0x82970BC0;
constexpr uint32_t kReleaseList = 0x8296BD8C;
constexpr uint32_t kDevice = 0x8296BD90;
constexpr uint32_t kPrimitiveDrawerVtable = 0x820721D4;
constexpr uint32_t kPrimitiveDrawerShaders[2] = {0x82970F2C, 0x82970F44};

constexpr uint32_t kResourceRelease = 0x822A67B0;
constexpr uint32_t kPushBack = 0x82591540;

float Float(uint32_t bits) {
  return std::bit_cast<float>(bits);
}

// The blend control with separate alpha off: the color half copied into
// the alpha half (as the game's setters do it).
uint32_t AlphaFromColor(uint32_t blend) {
  const uint32_t color = blend & 0xFFFF;
  uint32_t alpha = (color & 0x1010) | (std::rotl(color, 4) & 0xFFFFFFF0);
  alpha = std::rotl(alpha, 12) & 0xFFFF0000;
  alpha &= ~0x00100000u;
  alpha &= ~0x10000000u;
  return alpha | color;
}

Blend DecodeBlend(uint32_t control) {
  Blend b;
  b.src_color = BlendFactor(control & 0x1F);
  b.color_op = BlendOp((control >> 5) & 7);
  b.dst_color = BlendFactor((control >> 8) & 0x1F);
  b.src_alpha = BlendFactor((control >> 16) & 0x1F);
  b.alpha_op = BlendOp((control >> 21) & 7);
  b.dst_alpha = BlendFactor((control >> 24) & 0x1F);
  return b;
}

void LogOnce(const char* what, uint32_t value) {
  static std::set<std::pair<const char*, uint32_t>> logged;
  if (logged.insert({what, value}).second) {
    ITC_LOG_INFO("conventional renderer: {} {:08X}", what, value);
  }
}

Commands g_commands;

}  // namespace

Commands* GetCommands() {
  return &g_commands;
}

void Commands::BeginReplay(uint8_t* base) {
  if (defaults_set_) {
    return;
  }
  defaults_set_ = true;
  // As Device::SetDefaultState: each render state's and sampler state's
  // default from the tables, textures unset.
  for (uint32_t state = 0; state < 4 * kRenderStates; state += 4) {
    SetRenderState(base, state, guest::LoadU32(base, kRenderStateTable + 3 * state + 8));
  }
  for (uint32_t sampler = 0; sampler < kStages; sampler++) {
    for (uint32_t state = 0; state < 4 * kSamplerStates; state += 4) {
      SetSamplerState(base, sampler, state,
                      guest::LoadU32(base, kSamplerStateTable + 3 * state + 8));
    }
  }
  state_.dirty = kDirtyAll;
}

void Commands::UpdateBlend() {
  for (uint32_t i = 0; i < kTargets; i++) {
    state_.blend[i] = DecodeBlend(target_blend_[i]);
  }
  state_.dirty |= kDirtyBlend;
}

void Commands::SetRenderState(uint8_t* base, uint32_t state, uint32_t value) {
  State& s = state_;
  const uint32_t setter = guest::LoadU32(base, kRenderStateTable + 3 * state + 4);
  // The four targets' blend controls as the blend render states store them.
  auto store_blend = [&](uint32_t control) {
    for (uint32_t& target : target_blend_) {
      target = control;
    }
    UpdateBlend();
  };
  auto effective_blend = [&] {
    return separate_alpha_ ? blend_control_ : AlphaFromColor(blend_control_);
  };
  auto color_field = [&](int shift, uint32_t mask) {
    blend_control_ = (std::rotl(value, shift) & mask) | (blend_control_ & ~mask);
    if (blending_) {
      store_blend(effective_blend());
    }
  };
  auto alpha_field = [&](int shift, uint32_t mask) {
    blend_control_ = (std::rotl(value, shift) & mask) | (blend_control_ & ~mask);
    if (blending_ && separate_alpha_) {
      store_blend(blend_control_);
    }
  };
  DepthStencil& ds = s.depth_stencil;
  switch (setter) {
    case 0x822A18D8:  // ZEnable
      ds.depth_test = value & 1;
      break;
    case 0x822A1950:  // ZFunc
      ds.depth_compare = Compare(value & 7);
      break;
    case 0x822A1920:  // ZWriteEnable
      ds.depth_write = value & 1;
      break;
    case 0x822A1290:  // FillMode
      s.raster.fill_mode = uint8_t(value);
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1260:  // CullMode: bit 0 front, 1 back, 2 front is clockwise
      s.raster.cull_front = value & 1;
      s.raster.cull_back = value & 2;
      s.raster.front_clockwise = value & 4;
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1300:  // AlphaBlendEnable
      blending_ = value & 1;
      store_blend(value == 0 ? 0x00010001 : effective_blend());
      return;
    case 0x822A1690:  // SeparateAlphaBlendEnable
      separate_alpha_ = value & 1;
      store_blend(!blending_ ? 0x00010001 : value == 0 ? AlphaFromColor(blend_control_)
                                                       : blend_control_);
      return;
    case 0x822A17C0: {  // BlendFactor (ARGB)
      const float k = guest::LoadFloat(base, kOneOver255);
      s.blend_color = {float((value >> 16) & 0xFF) * k, float((value >> 8) & 0xFF) * k,
                       float(value & 0xFF) * k, float((value >> 24) & 0xFF) * k};
      s.dirty |= kDirtyBlendColor;
      return;
    }
    case 0x822A1420:  // SrcBlend
      color_field(0, 0x1F);
      return;
    case 0x822A14B0:  // DestBlend
      color_field(8, 0x1F00);
      return;
    case 0x822A1390:  // BlendOp
      color_field(5, 0xE0);
      return;
    case 0x822A15B0:  // SrcBlendAlpha
      alpha_field(16, 0x1F0000);
      return;
    case 0x822A1620:  // DestBlendAlpha
      alpha_field(24, 0x1F000000);
      return;
    case 0x822A1540:  // BlendOpAlpha
      alpha_field(21, 0xE00000);
      return;
    case 0x822A12C0:  // AlphaTestEnable
      s.alpha_test.enable = value & 1;
      s.dirty |= kDirtyAlphaTest;
      return;
    case 0x822A1728:  // AlphaRef (0-255)
      s.alpha_test.reference = float(double(value)) * guest::LoadFloat(base, kOneOver255);
      s.dirty |= kDirtyAlphaTest;
      return;
    case 0x822A1790:  // AlphaFunc
      s.alpha_test.compare = Compare(value & 7);
      s.dirty |= kDirtyAlphaTest;
      return;
    case 0x822A1990:  // StencilEnable
      ds.stencil = value & 1;
      break;
    case 0x822A19D8:  // TwoSidedStencilMode
      ds.two_sided = value & 1;
      break;
    case 0x822A1A48:
      ds.front.fail = StencilOp(value & 7);
      break;
    case 0x822A1A88:
      ds.front.depth_fail = StencilOp(value & 7);
      break;
    case 0x822A1AC8:
      ds.front.pass = StencilOp(value & 7);
      break;
    case 0x822A1A18:
      ds.front.compare = Compare(value & 7);
      break;
    case 0x822A1BD8:
      ds.front.reference = uint8_t(value);
      break;
    case 0x822A1BF8:
      ds.front.read_mask = uint8_t(value);
      break;
    case 0x822A1C18:
      ds.front.write_mask = uint8_t(value);
      break;
    case 0x822A1B28:
      ds.back.fail = StencilOp(value & 7);
      break;
    case 0x822A1B68:
      ds.back.depth_fail = StencilOp(value & 7);
      break;
    case 0x822A1BA8:
      ds.back.pass = StencilOp(value & 7);
      break;
    case 0x822A1AF8:
      ds.back.compare = Compare(value & 7);
      break;
    case 0x822A1C38:
      ds.back.reference = uint8_t(value);
      break;
    case 0x822A1C58:
      ds.back.read_mask = uint8_t(value);
      break;
    case 0x822A1C78:
      ds.back.write_mask = uint8_t(value);
      break;
    case 0x822A1C98:  // ClipPlaneEnable
      s.raster.clip_planes = uint8_t(value & 0x3F);
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1E60:  // MultiSampleAntiAlias
      s.raster.multisample = value & 1;
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1E90:  // MultiSampleMask
      s.raster.sample_mask = uint16_t(value);
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1CF0:  // SlopeScaleDepthBias
      s.raster.slope_scaled_depth_bias = Float(value);
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1DB8:  // DepthBias
      s.raster.depth_bias = Float(value);
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A1EB0:
    case 0x822A1EE8:
    case 0x822A1F20:
    case 0x822A1F58: {  // ColorWriteEnable, 1, 2, 3
      const uint32_t target = setter == 0x822A1EB0   ? 0
                              : setter == 0x822A1EE8 ? 1
                              : setter == 0x822A1F20 ? 2
                                                     : 3;
      s.write_mask[target] = uint8_t(value & 0xF);
      s.dirty |= kDirtyBlend;
      return;
    }
    case 0x822A2430:  // ViewportEnable
      s.raster.screen_space = value == 0;
      s.dirty |= kDirtyRaster | kDirtyViewport;
      return;
    case 0x822A2480:
    case 0x822A2530:
    case 0x822A25E0:
    case 0x822A2690: {  // HighPrecisionBlendEnable, 1, 2, 3
      const uint32_t target = setter == 0x822A2480   ? 0
                              : setter == 0x822A2530 ? 1
                              : setter == 0x822A25E0 ? 2
                                                     : 3;
      s.high_precision_blend[target] = value != 0;
      s.dirty |= kDirtyTargets;
      return;
    }
    case 0x822A27E8:  // HalfPixelOffset
      s.raster.half_pixel_offset = value & 1;
      s.dirty |= kDirtyRaster | kDirtyViewport;
      return;
    case 0x822A2820:  // PrimitiveResetEnable
      s.raster.primitive_restart = value & 1;
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A2850:  // PrimitiveResetIndex
      s.raster.restart_index = value;
      s.dirty |= kDirtyRaster;
      return;
    case 0x822A2870:  // AlphaToMaskEnable
      s.alpha_test.alpha_to_coverage = value & 1;
      s.dirty |= kDirtyAlphaTest;
      return;
    case 0x822A28A0:  // AlphaToMaskOffsets
      s.alpha_test.alpha_to_coverage_offsets = uint8_t(value);
      s.dirty |= kDirtyAlphaTest;
      return;
    case 0x822A3D20:  // ScissorTestEnable (render state 200)
      s.scissor = value != 0;
      s.dirty |= kDirtyScissor;
      return;
    case 0x822A20B0: case 0x822A20D8: case 0x822A2108: case 0x822A2138:
    case 0x822A2168: case 0x822A2198: case 0x822A21C8: case 0x822A21F8:
    case 0x822A2220: case 0x822A2248: case 0x822A2278: case 0x822A22A8:
    case 0x822A22D8: case 0x822A2308: case 0x822A2338: case 0x822A2368: {  // Wrap0-15
      static constexpr uint32_t kWraps[16] = {
          0x822A20B0, 0x822A20D8, 0x822A2108, 0x822A2138, 0x822A2168, 0x822A2198,
          0x822A21C8, 0x822A21F8, 0x822A2220, 0x822A2248, 0x822A2278, 0x822A22A8,
          0x822A22D8, 0x822A2308, 0x822A2338, 0x822A2368};
      for (uint32_t i = 0; i < 16; i++) {
        if (kWraps[i] == setter) {
          s.wrap[i] = uint8_t(value & 0xF);
        }
      }
      s.dirty |= kDirtyShaders;
      return;
    }
    // Nothing a frame shows from these (points, tessellation, guard and
    // discard bands, hierarchical stencil, presentation), or no setter.
    case 0x822A1FA0: case 0x822A2000: case 0x822A1F90: case 0x822A2058:
    case 0x822A27B0: case 0x822A2740: case 0x822A2778: case 0x822A28D8:
    case 0x822A2908: case 0x822A2938: case 0x822A2960: case 0x822A2990:
    case 0x822A29C0: case 0x822A29F0: case 0x822A2A20: case 0x822A2A40:
    case 0x822A2A50: case 0x82106540:
      return;
    default:
      LogOnce("unknown render state setter", setter);
      return;
  }
  s.dirty |= kDirtyDepthStencil;
}

void Commands::SetSamplerState(uint8_t* base, uint32_t sampler, uint32_t state, uint32_t value) {
  if (sampler >= kStages) {
    return;
  }
  Sampler& s = state_.samplers[sampler];
  const uint32_t setter = guest::LoadU32(base, kSamplerStateTable + 3 * state + 4);
  switch (setter) {
    case 0x822A31A0:
      s.address_u = Address(value & 7);
      break;
    case 0x822A31F0:
      s.address_v = Address(value & 7);
      break;
    case 0x822A3240:
      s.address_w = Address(value & 7);
      break;
    case 0x822A3130:
      s.border_color = value;
      break;
    case 0x822A2BF8:
      s.mag_filter = uint8_t(value);
      break;
    case 0x822A2A70:
      s.min_filter = uint8_t(value);
      break;
    case 0x822A2D80:
      s.mip_filter = uint8_t(value & 3);
      break;
    case 0x822A2F90:
      s.lod_bias = Float(value);
      break;
    case 0x822A3030:
      s.max_mip_level = uint8_t(value);
      break;
    case 0x822A2E70:
      s.max_anisotropy = uint8_t(value);
      break;
    case 0x822A2CE8:
      s.mag_filter_z = uint8_t(value);
      break;
    case 0x822A2B60:
      s.min_filter_z = uint8_t(value);
      break;
    case 0x822A2DD8:
      s.separate_z_filter = value != 0;
      break;
    case 0x822A30B0:
      s.min_mip_level = uint8_t(value);
      break;
    case 0x822A3290:
      s.trilinear_threshold = uint8_t(value & 3);
      break;
    case 0x822A2EE8:
      s.anisotropy_bias = Float(value);
      break;
    case 0x822A32E8:
      s.h_gradient_exp_bias = uint8_t(value & 0x1F);
      break;
    case 0x822A3340:
      s.v_gradient_exp_bias = uint8_t(value & 0x1F);
      break;
    case 0x822A3398:
      s.white_border_w = value & 1;
      break;
    case 0x822A33F0:
      s.point_border = value != 0;
      break;
    default:
      LogOnce("unknown sampler state setter", setter);
      return;
  }
  state_.dirty |= kDirtyTextures;
}

void Commands::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, float min_z,
                           float max_z, uint8_t* base) {
  Surface surface = state_.targets[0];
  if (!surface) {
    surface = state_.depth;
    if (!surface) {
      return;
    }
  }
  uint32_t max_width, max_height;
  if (tiling_) {
    max_width = tiling_width_;
    max_height = tiling_height_;
  } else {
    const uint32_t size = guest::LoadU32(base, surface.address + 36);
    max_height = (std::rotl(size, 29) & 0x7FFF) + 1;
    max_width = (std::rotl(size, 14) & 0x3FFF) + 1;
  }
  uint32_t right = width + x;
  if (!(right < max_width)) {
    right = max_width;
  }
  uint32_t bottom = height + y;
  if (!(bottom < max_height)) {
    bottom = max_height;
  }
  state_.viewport = {x, y, right - x, bottom - y, min_z, max_z};
  state_.dirty |= kDirtyViewport;
}

void Commands::SetSurfaceInfo(uint8_t* base) {
  Rect& r = state_.scissor_rect;
  r.left = int32_t(guest::LoadU32(base, kSurfaceScissor));
  r.top = int32_t(guest::LoadU32(base, kSurfaceScissor + 4));
  r.right = int32_t(guest::LoadU32(base, kSurfaceScissor + 8));
  r.bottom = int32_t(guest::LoadU32(base, kSurfaceScissor + 12));
  state_.dirty |= kDirtyScissor;
  const uint32_t v = kSurfaceViewport;
  SetViewport(guest::LoadU32(base, v), guest::LoadU32(base, v + 4), guest::LoadU32(base, v + 8),
              guest::LoadU32(base, v + 12), guest::LoadFloat(base, v + 16),
              guest::LoadFloat(base, v + 20), base);
}

void Commands::SetRenderTarget(uint8_t* base, uint32_t index, uint32_t surface) {
  if (index >= kTargets) {
    LogOnce("render target index", index);
    return;
  }
  state_.targets[index].address = surface;
  state_.dirty |= kDirtyTargets | kDirtyBlend;
  if (index == 0 && (surface || state_.depth)) {
    SetSurfaceInfo(base);
  }
}

void Commands::SetDepthStencilSurface(uint8_t* base, uint32_t surface) {
  state_.depth.address = surface;
  state_.dirty |= kDirtyTargets | kDirtyDepthStencil;
  if (surface && !state_.targets[0]) {
    SetSurfaceInfo(base);
  }
}

void Commands::ApplyShaderPatches(uint8_t* base, uint32_t p, bool pixel) {
  // The shader's register patches (device_shaders.cpp's ApplyPatches), at
  // offsets from the device's +1024: copy records, then masked records.
  // Those reaching the float, bool or loop constants are applied to ours;
  // others are counted and logged (the fetch constants and registers).
  constexpr uint32_t kFloats = 1792 - 1024, kBools = 9984 - 1024, kLoops = 10016 - 1024,
                     kLoopsEnd = 10144 - 1024;
  Constants& c = state_.constants;
  auto store = [&](uint32_t offset, uint32_t word) {
    if (offset >= kFloats && offset < kFloats + 2 * 4096) {
      const uint32_t i = (offset - kFloats) / 4;
      auto& file = i < 1024 ? c.vertex : c.pixel;
      const uint32_t r = i % 1024;
      std::memcpy(&file[r / 4][r % 4], &word, 4);
      state_.dirty |= i < 1024 ? kDirtyVertexConstants : kDirtyPixelConstants;
    } else if (offset >= kBools && offset < kLoops) {
      c.bools[(offset - kBools) / 4] = word;
      state_.dirty |= kDirtyBoolLoopConstants;
    } else if (offset >= kLoops && offset < kLoopsEnd) {
      c.loops[(offset - kLoops) / 4] = word;
      state_.dirty |= kDirtyBoolLoopConstants;
    } else {
      unhandled_patch_records_++;
      LogOnce(pixel ? "pixel shader patch at device +1024 +" : "vertex shader patch at device +1024 +",
              offset);
    }
  };
  auto load = [&](uint32_t offset) -> uint32_t {
    uint32_t word = 0;
    if (offset >= kFloats && offset < kFloats + 2 * 4096) {
      const uint32_t i = (offset - kFloats) / 4;
      auto& file = i < 1024 ? c.vertex : c.pixel;
      const uint32_t r = i % 1024;
      std::memcpy(&word, &file[r / 4][r % 4], 4);
    } else if (offset >= kBools && offset < kLoops) {
      word = c.bools[(offset - kBools) / 4];
    } else if (offset >= kLoops && offset < kLoopsEnd) {
      word = c.loops[(offset - kLoops) / 4];
    }
    return word;
  };
  uint32_t record = p + 24;
  const uint32_t end = guest::LoadU32(base, p + 16) + record;
  bool masked = false;
  while (record < end) {
    const uint16_t count = guest::LoadU16(base, record + 2);
    const uint16_t offset = guest::LoadU16(base, record);
    record += 4;
    if (count == 0) {
      masked = true;
      break;
    }
    for (uint32_t k = 0; k < count; k++) {
      store(offset + 4 * k, guest::LoadU32(base, record + 4 * k));
    }
    record += 4 * count;
  }
  if (!masked) {
    return;
  }
  while (record < end) {
    uint16_t count = guest::LoadU16(base, record + 2);
    const uint16_t offset = guest::LoadU16(base, record);
    record += 4;
    if (count == 0) {
      return;
    }
    uint32_t at = offset;
    do {
      store(at, (guest::LoadU32(base, record) & load(at)) | guest::LoadU32(base, record + 4));
      record += 8;
      at += 4;
      count = uint16_t(count - 2);
    } while (count != 0);
  }
}

void Commands::SetVertexShader(uint8_t* base, uint32_t shader) {
  state_.vertex_shader.address = shader;
  state_.dirty |= kDirtyShaders;
  if (shader) {
    const uint32_t header = shader + 592;
    if (const uint32_t offset = guest::LoadU32(base, header + 12)) {
      ApplyShaderPatches(base, header + offset, false);
    }
  }
}

void Commands::SetPixelShader(uint8_t* base, uint32_t shader) {
  state_.pixel_shader.address = shader;
  state_.dirty |= kDirtyShaders;
  if (shader) {
    const uint32_t header = shader + 56;
    if (const uint32_t offset = guest::LoadU32(base, header + 12)) {
      ApplyShaderPatches(base, header + offset, true);
    }
  }
}

void Commands::SetVertexConstants(uint8_t* base, uint32_t first, uint32_t data, uint32_t count,
                                  bool pixel) {
  auto& file = pixel ? state_.constants.pixel : state_.constants.vertex;
  for (uint32_t i = 0; i < count && first + i < kFloatConstants; i++) {
    for (uint32_t k = 0; k < 4; k++) {
      file[first + i][k] = guest::LoadFloat(base, data + 16 * i + 4 * k);
    }
  }
  state_.dirty |= pixel ? kDirtyPixelConstants : kDirtyVertexConstants;
}

void Commands::SetBoolConstants(uint32_t first, const uint32_t* values, uint32_t count,
                                bool pixel) {
  // A bit each: vertex from +9984, pixel from +10000 (four words each).
  uint32_t* words = state_.constants.bools.data() + (pixel ? 4 : 0);
  do {
    const uint32_t bit = first & 0x1F, word = first >> 5;
    if (word < 4) {
      words[word] = (words[word] & ~(1u << bit)) | ((*values & 1) << bit);
    }
    values++;
    first++;
  } while (--count != 0);
  state_.dirty |= kDirtyBoolLoopConstants;
}

void Commands::SetLoopConstants(uint8_t* base, uint32_t first, uint32_t data, uint32_t count,
                                bool pixel) {
  // A word each from an integer vector's x, y and z bytes: vertex from
  // +10016, pixel from +10080.
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t at = (pixel ? 16 : 0) + first + i;
    const uint32_t item = data + 16 * i;
    if (at < state_.constants.loops.size()) {
      state_.constants.loops[at] = uint32_t(guest::LoadU8(base, item + 11)) << 16 |
                                   uint32_t(guest::LoadU8(base, item + 7)) << 8 |
                                   guest::LoadU8(base, item + 3);
    }
  }
  state_.dirty |= kDirtyBoolLoopConstants;
}

void Commands::SetShaderConstants(uint8_t* base, uint32_t payload, uint32_t count,
                                  uint32_t packed) {
  const uint32_t stage = packed >> 24, type = (packed >> 16) & 0xFF,
                 first = (packed >> 8) & 0xFF;
  if (stage > 1) {
    return;
  }
  const bool pixel = stage == 1;
  switch (type) {
    case 0:
      SetLoopConstants(base, first, payload, count, pixel);
      break;
    case 1:
      SetVertexConstants(base, first, payload, count, pixel);
      break;
    case 2: {
      uint32_t values[256];
      const uint32_t n = std::max(1u, std::min(count, 256u));
      for (uint32_t i = 0; i < n; i++) {
        values[i] = guest::LoadU32(base, payload + 4 * i);
      }
      SetBoolConstants(first, values, n, pixel);
      break;
    }
    default:
      break;  // 3: a debug string, nothing set
  }
}

void Commands::InlineDraw(uint8_t* base, uint32_t type, uint32_t data, uint32_t count,
                          uint32_t stride) {
  if (backend_ && count) {
    backend_->DrawInline(state_, {Primitive(type), base + data, count, stride});
  }
}

void Commands::DrawDrawer(uint8_t* base, const GameCall& call, bool single) {
  // The drawer's state: +0 the type, +4 1 for screen-space vertices, +12
  // the vertices, +16 their count (24 bytes each).
  const uint32_t drawer_state = guest::LoadU32(base, kDrawerState);
  if (guest::LoadU32(base, drawer_state + 16) == 0) {
    return;
  }
  const bool screen_space_before = state_.raster.screen_space;
  state_.raster.screen_space = guest::LoadU32(base, drawer_state + 4) == 1;
  state_.dirty |= kDirtyRaster | kDirtyViewport;
  const uint32_t drawer = guest::LoadU32(base, kDrawer);
  const uint32_t vtable = guest::LoadU32(base, drawer);
  if (vtable == kPrimitiveDrawerVtable) {
    // PrimitiveDrawer::Draw: its declaration, then the vertices inline.
    state_.declaration.address = guest::LoadU32(base, guest::LoadU32(base, drawer + 4));
    state_.dirty |= kDirtyShaders;
    InlineDraw(base, guest::LoadU32(base, drawer_state), guest::LoadU32(base, drawer_state + 12),
               guest::LoadU32(base, drawer_state + 16), 24);
  } else {
    LogOnce("drawer vtable (a guest call)", vtable);
    // Its slot 3 with (drawer, device, type, vertices, count) can't be
    // passed through `call` (three arguments): not seen in play.
  }
  if (single) {
    // Stream 0 and texture 0 unbound, the flag put back.
    state_.streams[0] = {};
    state_.textures[0] = {};
    state_.raster.screen_space = screen_space_before;
    state_.dirty |= kDirtyStreams | kDirtyTextures;
  } else {
    state_.raster.screen_space = false;
  }
  (void)call;
}

void Commands::Resolve(uint8_t* base, uint32_t item) {
  // A 68-byte item: +0 which fields are given (bit 0 the rect at +8, 1 the
  // destination at +24, 2 the point at +28, 3 the clear color, 4 the
  // parameters), +4 the flags, +36 the level, +40 the slice, +44 the clear
  // color's bytes, +48 the clear Z, +52 the stencil.
  const uint32_t fields = guest::LoadU32(base, item);
  render::Resolve r;
  r.flags = guest::LoadU32(base, item + 4);
  r.has_rect = fields & 1;
  r.rect = {int32_t(guest::LoadU32(base, item + 8)), int32_t(guest::LoadU32(base, item + 12)),
            int32_t(guest::LoadU32(base, item + 16)), int32_t(guest::LoadU32(base, item + 20))};
  if (fields & 2) {
    r.destination.address = guest::LoadU32(base, item + 24);
  }
  r.has_point = fields & 4;
  r.point[0] = int32_t(guest::LoadU32(base, item + 28));
  r.point[1] = int32_t(guest::LoadU32(base, item + 32));
  r.level_or_slice = guest::LoadU32(base, item + 36) || guest::LoadU32(base, item + 40);
  for (uint32_t c = 0; (fields & 8) && c < 4; c++) {
    r.clear_color[c] = float(guest::LoadU8(base, item + 44 + c)) * guest::LoadFloat(base, kOneOver255);
  }
  r.clear_depth = guest::LoadFloat(base, item + 48);
  r.clear_stencil = guest::LoadU32(base, item + 52);
  if (backend_) {
    backend_->Resolve(state_, r);
  }
}

void Commands::Execute(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count,
                       uint32_t arg2, const GameCall& call) {
  State& s = state_;
  auto u32 = [&](uint32_t address) { return guest::LoadU32(base, address); };
  switch (id) {
    case 1:
      for (uint32_t i = 0; i < count; i++) {
        SetRenderState(base, u32(payload + 16 * i), u32(payload + 16 * i + 4));
      }
      return;
    case 2:
      for (uint32_t i = 0; i < count; i++) {
        SetSamplerState(base, u32(payload + 16 * i), u32(payload + 16 * i + 4),
                        u32(payload + 16 * i + 8));
      }
      return;
    case 3:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t target = u32(payload + 16 * i);
        if (target < kTargets) {
          target_blend_[target] = u32(payload + 16 * i + 4);
        }
      }
      UpdateBlend();
      return;
    case 4:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t stage = u32(payload + 16 * i);
        if (stage < kStages) {
          s.textures[stage].address = u32(payload + 16 * i + 4);
        }
      }
      s.dirty |= kDirtyTextures;
      return;
    case 5: {
      const uint32_t drawer_state = u32(kDrawerState);
      guest::StoreU32(base, drawer_state + 12, payload);
      guest::StoreU32(base, drawer_state + 16, count);
      return;
    }
    case 6: {
      const uint32_t drawer_state = u32(kDrawerState);
      guest::StoreU32(base, drawer_state + 20, payload);
      guest::StoreU32(base, drawer_state + 24, count);
      guest::StoreU32(base, drawer_state + 28, arg2);
      return;
    }
    case 7: {
      // Interpolation's substitution, around our upload (timed with
      // --log_frame_times).
      if (!ITC_FLAG(log_frame_times)) {
        interpolation::BlendState_ReplayConstants(
            base, payload, arg2, [&] { SetShaderConstants(base, payload, count, arg2); });
        return;
      }
      using Clock = std::chrono::steady_clock;
      const Clock::time_point start = Clock::now();
      int64_t upload_ns = 0;
      interpolation::BlendState_ReplayConstants(base, payload, arg2, [&] {
        const Clock::time_point upload_start = Clock::now();
        SetShaderConstants(base, payload, count, arg2);
        upload_ns = (Clock::now() - upload_start).count();
      });
      interpolation::BlendState_AddReplayTimes((Clock::now() - start).count(), upload_ns);
      return;
    }
    case 8:
      for (uint32_t i = 0; i < count; i++) {
        SetVertexShader(base, u32(payload + 16 * i));
      }
      return;
    case 9:
      for (uint32_t i = 0; i < count; i++) {
        SetPixelShader(base, u32(payload + 16 * i));
      }
      return;
    case 10:
      for (uint32_t i = 0; i < count; i++) {
        s.declaration.address = u32(payload + 16 * i);
      }
      s.dirty |= kDirtyShaders;
      return;
    case 11:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 16 * i;
        const uint32_t stream = u32(item);
        if (stream < kStreams) {
          s.streams[stream] = {{u32(item + 4)}, u32(item + 8), u32(item + 12)};
        }
      }
      s.dirty |= kDirtyStreams | kDirtyShaders;
      return;
    case 12:
      for (uint32_t i = 0; i < count; i++) {
        s.indices.address = u32(payload + 16 * i);
      }
      s.dirty |= kDirtyIndices;
      return;
    case 13:
    case 14:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 16 * i;
        const uint32_t drawer_state = u32(kDrawerState);
        guest::StoreU32(base, drawer_state + 0, u32(item));
        guest::StoreU32(base, drawer_state + 4, u32(item + 4));
        guest::StoreU32(base, drawer_state + 8, u32(item + 8));
        const uint32_t drawer = u32(kDrawer);
        if (u32(drawer) == kPrimitiveDrawerVtable) {
          // PrimitiveDrawer::SetShaders: vertex shader 0 or 1, pixel shader
          // 0 to 4, from its tables (each entry's +8).
          const uint32_t vs = u32(item + 4), ps = u32(item + 8);
          const int32_t v = vs < 1 ? 0 : vs == 1 ? 1 : -1;
          const int32_t p = ps <= 4 ? int32_t(ps) : -1;
          SetVertexShader(base, u32(kPrimitiveDrawerShaders[0] + 8 + 12 * v));
          SetPixelShader(base, u32(kPrimitiveDrawerShaders[1] + 8 + 12 * p));
        } else {
          LogOnce("drawer vtable at set-up", u32(drawer));
        }
      }
      return;
    case 15:
      for (uint32_t i = 0; i < count; i++) {
        DrawDrawer(base, call, false);
      }
      return;
    case 16:
      LogOnce("command (not drawn)", id);
      return;
    case 17:
      LogOnce("command (not drawn)", id);
      return;
    case 18:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 12 * i;
        const uint32_t type = u32(item), base_start = u32(item + 4);
        if (backend_) {
          backend_->DrawIndexed(s, {Primitive(type), int32_t(base_start) >> 16,
                                    base_start & 0xFFFF, u32(item + 8)});
        }
      }
      return;
    case 19:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 12 * i;
        InlineDraw(base, u32(item), u32(item + 8), guest::LoadU16(base, item + 4),
                   guest::LoadU16(base, item + 6));
      }
      return;
    case 20:
      for (uint32_t i = 0; i < count; i++) {
        if (const uint32_t object = u32(payload + 16 * i)) {
          call(kResourceRelease, 1, object, 0, 0);
        }
      }
      return;
    case 21:
      return;
    case 22:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 16 * i;
        const uint32_t object = u32(item + 4);
        auto slot = [&](uint32_t n) { return u32(u32(object) + 4 * n); };
        if (uint8_t(call(slot(2), 1, object, 0, 0)) == 0 && u32(item + 8) == 1) {
          call(slot(1), 1, object, 0, 0);
          call(kPushBack, 2, u32(kReleaseList), item + 4, 0);
          call(slot(3), 1, object, 0, 0);
        }
      }
      return;
    case 23:
      for (uint32_t i = 0; i < count; i++) {
        SetRenderTarget(base, u32(payload + 16 * i), u32(payload + 16 * i + 4));
      }
      return;
    case 24:
      for (uint32_t i = 0; i < count; i++) {
        SetDepthStencilSurface(base, u32(payload + 16 * i));
      }
      return;
    case 25:
      for (uint32_t i = 0; i < count; i++) {
        Resolve(base, payload + 68 * i);
      }
      return;
    case 26:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 280 * i;
        render::Clear c;
        c.flags = u32(item + 8);
        c.color = u32(item + 268);
        c.depth = guest::LoadFloat(base, item + 272);
        c.stencil = u32(item + 276);
        if (backend_) {
          backend_->Clear(s, c);
        }
      }
      return;
    case 27:
      LogOnce("command (not drawn)", id);
      return;
    case 28:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t object = u32(payload + 16 * i + 4);
        LogOnce("callback command's object", object);
        call(u32(u32(object)), 2, object, u32(kDevice), 0);
      }
      return;
    case 29:
      for (uint32_t i = 0; i < count; i++) {
        // [0x82970BC0]: the set RenderQueue::Init kept (the queue state's +48).
        const uint32_t mask = u32(payload + 4 * i), saved = u32(kSavedTargets);
        for (uint32_t t = 0; t < kTargets; t++) {
          if (mask & (1u << t)) {
            SetRenderTarget(base, t, u32(saved + 4 * t));
          }
        }
        if (mask & 0x80) {
          SetDepthStencilSurface(base, u32(saved + 16));
        }
      }
      return;
    case 30:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 16 * i;
        SetViewport(guest::LoadU16(base, item), guest::LoadU16(base, item + 2),
                    guest::LoadU16(base, item + 4), guest::LoadU16(base, item + 6),
                    guest::LoadFloat(base, item + 8), guest::LoadFloat(base, item + 12), base);
      }
      return;
    case 31:
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t item = payload + 8 * i;
        s.scissor = true;
        s.scissor_rect = {int16_t(guest::LoadU16(base, item)), int16_t(guest::LoadU16(base, item + 2)),
                          int16_t(guest::LoadU16(base, item + 4)),
                          int16_t(guest::LoadU16(base, item + 6))};
      }
      s.dirty |= kDirtyScissor;
      return;
    case 32:
      for (uint32_t i = 0; i < count; i++) {
        // [flags, tile count, tile rects (left, top, right, bottom)]
        tile_count_ = u32(payload + 4);
        tile_rects_ = u32(payload + 8);
        tiling_width_ = tiling_height_ = 0;
        for (uint32_t t = 0; tile_rects_ && t < tile_count_ && t < 16; t++) {
          tiling_width_ = std::max(tiling_width_, uint32_t(std::max(int32_t(u32(tile_rects_ + 16 * t + 8)), 0)));
          tiling_height_ = std::max(tiling_height_, uint32_t(std::max(int32_t(u32(tile_rects_ + 16 * t + 12)), 0)));
        }
        tiling_ = true;
        if (backend_) {
          backend_->BeginTiling(s, tiling_width_, tiling_height_, guest::LoadFloat(base, kZero));
        }
      }
      return;
    case 33:
      for (uint32_t i = 0; i < count; i++) {
        // [flags, destination]: a resolve of each tile into its place.
        const uint32_t flags = u32(payload), destination = u32(payload + 4);
        for (uint32_t t = 0; backend_ && tile_rects_ && t < tile_count_; t++) {
          render::Resolve r;
          r.flags = flags;
          r.has_rect = r.has_point = true;
          const uint32_t rect = tile_rects_ + 16 * t;
          r.rect = {int32_t(u32(rect)), int32_t(u32(rect + 4)), int32_t(u32(rect + 8)),
                    int32_t(u32(rect + 12))};
          r.point[0] = r.rect.left;
          r.point[1] = r.rect.top;
          r.destination.address = destination;
          const float zero = guest::LoadFloat(base, kZero);
          for (float& c : r.clear_color) {
            c = zero;
          }
          r.clear_depth = zero;
          r.apply_clears = t + 1 >= tile_count_;
          backend_->Resolve(s, r);
        }
        tiling_ = false;
        if (backend_) {
          backend_->EndTiling(s);
        }
      }
      return;
    case 34:
      saved_viewport_ = s.viewport;
      for (uint32_t t = 0; t < kTargets; t++) {
        saved_targets_[t] = s.targets[t];
        if (s.targets[t]) {
          refcount::AddRef(base, s.targets[t].address);  // as GetRenderTarget
        }
      }
      saved_depth_ = s.depth;
      if (s.depth) {
        refcount::AddRef(base, s.depth.address);
      }
      return;
    case 35:
      for (uint32_t t = 0; t < kTargets; t++) {
        SetRenderTarget(base, t, saved_targets_[t].address);
      }
      SetDepthStencilSurface(base, saved_depth_.address);
      SetViewport(saved_viewport_.x, saved_viewport_.y, saved_viewport_.width,
                  saved_viewport_.height, saved_viewport_.min_z, saved_viewport_.max_z, base);
      return;
    default:
      LogOnce("unknown command", id);
      return;
  }
}

uint32_t Commands::Run(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count,
                       uint32_t arg2, const GameCall& call) {
  Execute(base, id, payload, count, arg2, call);
  return Slots(id, count, arg2);
}

uint32_t Commands::Slots(uint32_t id, uint32_t count, uint32_t arg2) {
  switch (id) {
    case 5:
      return (count * 24 + 15) / 16;
    case 6:
      return (count * (arg2 ? 4 : 2) + 15) / 16;
    case 7:
      return (arg2 >> 24) <= 1 ? count : 0;
    case 21:
      return (count + 15) / 16;
    case 25:
      return (count * 68 + 15) / 16;
    case 26:
      return (count * 280 + 15) / 16;
    case 27:
      return (count * 32 + 15) / 16;
    default:
      return count;
  }
}

uint32_t Commands::RunSingle(uint8_t* base, uint32_t slot, const GameCall& call) {
  // [id, arguments]; the items of those with several from +16.
  auto u32 = [&](uint32_t address) { return guest::LoadU32(base, address); };
  const uint32_t id = u32(slot), count = u32(slot + 4);
  switch (id) {
    case 5: {
      const uint32_t drawer_state = u32(kDrawerState);
      guest::StoreU32(base, drawer_state + 12, slot + 16);
      guest::StoreU32(base, drawer_state + 16, count);
      return (count * 24 + 15) / 16;
    }
    case 7: {
      const uint32_t stage = u32(slot + 4), type = u32(slot + 8);
      const uint32_t first = guest::LoadU8(base, slot + 12), n = guest::LoadU8(base, slot + 13);
      if (stage > 1) {
        return 0;
      }
      if (type == 4) {
        // Bool constants from a bitmask: +16 the bits, +20 how many.
        const uint32_t bits = u32(slot + 16), how_many = u32(slot + 20);
        uint32_t values[32];
        const uint32_t m = std::max(1u, std::min(how_many, 32u));
        for (uint32_t i = 0; i < m; i++) {
          values[i] = (bits >> i) & 1;
        }
        SetBoolConstants(first, values, m, stage == 1);
      } else {
        SetShaderConstants(base, slot + 16, n, stage << 24 | type << 16 | first << 8);
      }
      return n;
    }
    case 13:
    case 14: {
      const uint32_t drawer_state = u32(kDrawerState);
      guest::StoreU32(base, drawer_state + 0, u32(slot + 4));
      guest::StoreU32(base, drawer_state + 4, u32(slot + 8));
      guest::StoreU32(base, drawer_state + 8, u32(slot + 12));
      const uint32_t drawer = u32(kDrawer);
      if (u32(drawer) == kPrimitiveDrawerVtable) {
        const uint32_t vs = u32(slot + 8), ps = u32(slot + 12);
        const int32_t v = vs < 1 ? 0 : vs == 1 ? 1 : -1;
        const int32_t p = ps <= 4 ? int32_t(ps) : -1;
        SetVertexShader(base, u32(kPrimitiveDrawerShaders[0] + 8 + 12 * v));
        SetPixelShader(base, u32(kPrimitiveDrawerShaders[1] + 8 + 12 * p));
      } else {
        LogOnce("drawer vtable at set-up", u32(drawer));
      }
      return 0;
    }
    case 15:
      DrawDrawer(base, call, true);
      return 0;
    case 28: {
      const uint32_t object = u32(slot + 4);
      call(u32(u32(object)), 2, object, u32(kDevice), 0);
      return 0;
    }
    default:
      // The others aren't seen in play; their single forms as the commands'.
      LogOnce("single command (not handled)", id);
      return id == 21 ? count : 0;
  }
}

}  // namespace render
