#include "render/state.h"

#include <algorithm>
#include <bit>
#include <climits>
#include <cstring>

#include "base/hash.h"
#include "schema/device_state.h"

namespace render {

namespace {

RenderStates g_states;
Targets g_targets;
Bindings g_bindings;
Constants g_constants;

// A sampler stage: its descriptor words and what the sampler setters keep
// beside them (the device's per-stage bytes), and the bound texture.
struct Stage {
  uint32_t words[6] = {};
  uint8_t max_anisotropy = 0, z_filters = 0, max_mip = 0, min_mip = 0;
  uint32_t texture = 0;
  // The sampler as Vulkan's (SamplerState), and what it's made from: the
  // mag and min filters (point 0, linear 1) and whether each asked for
  // anisotropy.
  SamplerState sampler;
  uint8_t filters[2] = {};
  bool anisotropic[2] = {};
};
constexpr uint32_t kStages = 26;
Stage g_stages[kStages];
bool g_stages_set_up = false;

// rlwimi: `word`'s bits in `mask` from `value` rotated left by `shift`.
void Insert(uint32_t& word, uint32_t value, int shift, uint32_t mask) {
  word = (std::rotl(value, shift) & mask) | (word & ~mask);
}
// fctidz as the recompiled code does it.
int64_t ToInt64(double d) {
  if (d != d) {
    return INT64_MIN;
  }
  if (d > double(LLONG_MAX)) {
    return LLONG_MAX;
  }
  return int64_t(d);
}

// The whole surface: the viewport and the scissor rectangle.
void Whole() {
  g_targets.viewport = {0, 0, uint32_t(INT_MAX), uint32_t(INT_MAX), 0.0f, 1.0f};
  g_states.scissor[0] = g_states.scissor[1] = 0;
  g_states.scissor[2] = g_states.scissor[3] = INT_MAX;
}

// The blend as D3D's state builds it (the device's software copy, +11616,
// and its enables): the fields at the blend control's places, kept so
// separate alpha and blending turned on and off give what the device's
// setters give (src/graphics/device_state.cpp).
uint32_t g_blend_fields = 0x00010001;  // ONE, ADD, ZERO for color and alpha
bool g_blend_enable = false, g_separate_alpha = false;

// The color half copied into the alpha half (Device's AlphaFromColor, the
// original's bit steps).
uint32_t AlphaFromColor(uint32_t blend) {
  const uint32_t color = blend & 0xFFFF;
  uint32_t alpha = (color & 0x1010) | (std::rotl(color, 4) & 0xFFFFFFF0);
  alpha = std::rotl(alpha, 12) & 0xFFFF0000;
  alpha &= ~0x00100000u;
  alpha &= ~0x10000000u;
  return alpha | color;
}

BlendState Decode(uint32_t control) {
  BlendState b;
  b.fields = control;
  b.src_color = BlendFactor(control & 0x1F);
  b.color_op = BlendOp(control >> 5 & 7);
  b.dst_color = BlendFactor(control >> 8 & 0x1F);
  b.src_alpha = BlendFactor(control >> 16 & 0x1F);
  b.alpha_op = BlendOp(control >> 21 & 7);
  b.dst_alpha = BlendFactor(control >> 24 & 0x1F);
  b.enable = !(b.src_color == VK_BLEND_FACTOR_ONE && b.dst_color == VK_BLEND_FACTOR_ZERO &&
               b.color_op == VK_BLEND_OP_ADD && b.src_alpha == VK_BLEND_FACTOR_ONE &&
               b.dst_alpha == VK_BLEND_FACTOR_ZERO && b.alpha_op == VK_BLEND_OP_ADD);
  return b;
}

// All four targets' blends, as the setters store them.
void StoreBlends(uint32_t control) {
  BlendState b = Decode(control);
  for (BlendState& target : g_states.blend) {
    target = b;
  }
}

uint32_t CurrentBlend() {
  if (!g_blend_enable) {
    return 0x00010001;
  }
  return g_separate_alpha ? g_blend_fields : AlphaFromColor(g_blend_fields);
}

void SetField(uint32_t value, int shift, uint32_t mask) {
  g_blend_fields = (g_blend_fields & ~mask) | ((value << shift) & mask);
}

float Float(uint32_t bits) { return std::bit_cast<float>(bits); }

}  // namespace

VkBlendFactor BlendFactor(uint32_t factor) {
  static const VkBlendFactor kMap[17] = {
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_FACTOR_ONE,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_FACTOR_SRC_COLOR,
      VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
      VK_BLEND_FACTOR_SRC_ALPHA,
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      VK_BLEND_FACTOR_DST_COLOR,
      VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
      VK_BLEND_FACTOR_DST_ALPHA,
      VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
      VK_BLEND_FACTOR_CONSTANT_COLOR,
      VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
      VK_BLEND_FACTOR_CONSTANT_ALPHA,
      VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
      VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
  };
  return factor < 17 ? kMap[factor] : VK_BLEND_FACTOR_ZERO;
}

VkBlendOp BlendOp(uint32_t op) {
  static const VkBlendOp kMap[5] = {VK_BLEND_OP_ADD, VK_BLEND_OP_SUBTRACT, VK_BLEND_OP_MIN,
                                    VK_BLEND_OP_MAX, VK_BLEND_OP_REVERSE_SUBTRACT};
  return op < 5 ? kMap[op] : VK_BLEND_OP_ADD;
}

const RenderStates& States() { return g_states; }
const Targets& BoundTargets() { return g_targets; }
const Bindings& BoundResources() { return g_bindings; }
const Constants& ShaderConstants() { return g_constants; }

namespace {

// Word 4's low two bits from word 3's filters and the z filters (the
// original's steps).
uint32_t ZFilterBits(uint32_t word3, uint32_t z) {
  uint32_t bits = (std::rotl(word3, 31) & 0x7FFFF) | (word3 & 0xFFF80000u);
  bits = (std::rotl(word3, 31) & 0x7FF00000u) | (bits & 0x800FFFFFu);
  const uint32_t filters = std::rotl(bits, 13) & 0xFFF;
  const uint32_t m = (z >> 2) - 1;
  return (filters & m) + (z & ~m);
}

void SetFilter(Stage& s, uint32_t value, bool mag) {
  uint32_t& word3 = s.words[3];
  uint32_t& word4 = s.words[4];
  const uint32_t entry =
      guest::LoadU32(schema::DeviceStateGlobals::anisotropy_table_address() +
                     4 * s.max_anisotropy);
  const uint32_t anisotropic = value >> 2;
  const uint32_t other = (word4 >> (mag ? 11 : 10)) & 1;
  const uint32_t own = mag ? 0x400 : 0x800;
  word4 = (std::rotl(anisotropic, mag ? 10 : 11) & own) | (word4 & ~own);
  const uint32_t m = (other | anisotropic) - 1;
  const uint32_t bits = (mag ? (std::rotl(entry & ~m, 6) & 0xFFFFFFC0u)
                             : (std::rotl(entry & ~m, 4) & 0xFFFFFFF0u)) |
                        anisotropic | value;
  const int shift = mag ? 19 : 21;
  const uint32_t field = mag ? 0x180000 : 0x600000;
  word3 = (std::rotl(bits, shift) & field) | (word3 & ~field);
  word3 = (std::rotl(bits, shift) & 0xE000000) | (word3 & ~0xE000000u);
  word4 = (word4 & 0xFFFFFFFCu) | (ZFilterBits(word3, s.z_filters) & 3);
}

void SetZFilter(Stage& s, uint32_t value, int shift) {
  const uint32_t z = (s.z_filters & ~(1u << shift)) | (value << shift);
  s.words[4] = (s.words[4] & 0xFFFFFFFCu) | (ZFilterBits(s.words[3], z) & 3);
  s.z_filters = uint8_t(z);
}

void SetMipLevel(Stage& s, uint32_t value, bool max) {
  if (s.texture) {
    const uint32_t own = schema::DeviceTexture(s.texture).mip_levels();
    uint32_t level = max ? (own >> 2) & 0xF : (own >> 6) & 0xF;
    if (max ? !(level > value) : !(level < value)) {
      level = value;
    }
    Insert(s.words[4], level, max ? 2 : 6, max ? 0x3Cu : 0x3C0u);
  }
  (max ? s.max_mip : s.min_mip) = uint8_t(value);
}

// The GPU's clamp modes (D3D's address modes on the 360) as Vulkan's;
// mirror clamp to edge where a device lacks it is the sampler cache's.
VkSamplerAddressMode AddressMode(uint32_t mode) {
  switch (mode & 7) {
    case 0: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case 1: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case 6: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case 3: case 5: case 7: return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    default: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;  // 2, 4: to the edge, halfway
  }
}

// The stage's sampler from what its sampler states set: as the GPU reads
// the merged descriptor (a filter is linear when its field is 1 or the
// stage is anisotropic; anisotropy from the game's table by MaxAnisotropy
// when either filter asks for it).
void MapSampler(Stage& s) {
  using G = schema::DeviceStateGlobals;
  SamplerState& m = s.sampler;
  const uint32_t aniso = (s.anisotropic[0] || s.anisotropic[1])
                             ? guest::LoadU32(G::anisotropy_table_address() +
                                              4 * s.max_anisotropy) & 7
                             : 0;
  m.max_anisotropy = aniso >= 1 ? float(1u << (aniso - 1)) : 0.0f;
  m.mag_filter = s.filters[0] == 1 || aniso >= 1 ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
  m.min_filter = s.filters[1] == 1 || aniso >= 1 ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
}

void ApplySamplerState(Stage& s, uint32_t state, uint32_t value) {
  using G = schema::DeviceStateGlobals;
  // The sampler, as Vulkan's.
  switch (state) {
    case 0: case 4: case 8:  // AddressU, V, W
      s.sampler.address[state / 4] = AddressMode(value);
      break;
    case 12:  // BorderColor: opaque white unless 0
      s.sampler.border = value != 0 ? VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
                                    : VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
      break;
    case 16: case 20:  // MagFilter, MinFilter (4 anisotropic, as linear)
      s.filters[state == 20] = uint8_t((value | value >> 2) & 3);
      s.anisotropic[state == 20] = (value >> 2) != 0;
      break;
    case 24:  // MipFilter: point 0, linear 1, none 2
      s.sampler.mip_mode =
          (value & 3) == 1 ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
      s.sampler.base_level_only = (value & 3) == 2;
      break;
    case 28:  // MipMapLodBias, in levels
      s.sampler.mip_lod_bias = std::bit_cast<float>(value);
      break;
    default:
      break;
  }
  switch (state) {
    case 0: Insert(s.words[0], value, 10, 0x1C00); break;   // AddressU
    case 4: Insert(s.words[0], value, 13, 0xE000); break;   // AddressV
    case 8: Insert(s.words[0], value, 16, 0x70000); break;  // AddressW
    case 12:  // BorderColor
      s.words[5] = (s.words[5] & 0xFFFFFFFCu) | (value != 0 ? 1 : 0);
      break;
    case 16: SetFilter(s, value, true); break;   // MagFilter
    case 20: SetFilter(s, value, false); break;  // MinFilter
    case 24: Insert(s.words[3], value, 23, 0x1800000); break;  // MipFilter
    case 28:  // MipMapLodBias: a float, in the GPU's fixed point
      Insert(s.words[4],
             uint32_t(ToInt64(double(std::bit_cast<float>(value) * G::mip_map_lod_bias_scale()))),
             12, 0x3FF000);
      break;
    case 32: SetMipLevel(s, value, true); break;  // MaxMipLevel
    case 36:  // MaxAnisotropy
      if ((s.words[4] & 0x800) || (s.words[4] & 0x400)) {
        Insert(s.words[3], guest::LoadU32(G::anisotropy_table_address() + (value << 2)), 25,
               0xE000000);
      }
      s.max_anisotropy = uint8_t(value);
      break;
    case 40: SetZFilter(s, value, 0); break;  // MagFilterZ
    case 44: SetZFilter(s, value, 1); break;  // MinFilterZ
    case 48: SetZFilter(s, value, 2); break;  // SeparateZFilterEnable
    case 52: SetMipLevel(s, value, false); break;  // MinMipLevel
    case 56: Insert(s.words[5], value, 3, 0x18); break;  // TrilinearThreshold
    case 60:  // AnisotropyBias
      Insert(s.words[5],
             uint32_t(ToInt64(double(std::bit_cast<float>(value) * G::anisotropy_bias_scale()))),
             5, 0x1E0);
      break;
    case 64: Insert(s.words[4], value, 22, 0x7C00000); break;    // HGradientExpBias
    case 68: Insert(s.words[4], value, 27, 0xF8000000u); break;  // VGradientExpBias
    case 72: Insert(s.words[5], value, 2, 0x4); break;           // WhiteBorderColorW
    case 76: Insert(s.words[1], value == 0 ? 1 : 0, 11, 0x800); break;  // PointBorderEnable
    default: break;
  }
  MapSampler(s);
}

void ApplyTexture(Stage& s, uint32_t texture) {
  s.texture = texture;
  if (!texture) {
    return;  // the descriptor stays
  }
  const schema::DeviceTexture t(texture);
  auto physical = [](uint32_t address) {
    return ((((address >> 20) & 0xFFF) + 512) & 0x1000) + (address & 0x1FFFFFFF);
  };
  const uint32_t mip = t.mip_address();
  const uint32_t mip_address = ((((mip >> 20) & 0xFFF) + 512) & 0x1000) + (mip & 0x1FFFFE00);
  const uint32_t levels = t.mip_levels();
  uint32_t* w = s.words;
  w[2] = t.fetch2();
  w[0] = (w[0] & 0x3FFC00) | (t.fetch0() & ~0x3FFC00u);  // the clamps kept
  w[5] = (w[5] & 0x1FF) | (mip_address & ~0x1FFu);
  uint32_t max_level = s.max_mip;
  if (((levels >> 2) & 0xF) > max_level) {
    max_level = (levels >> 2) & 0xF;
  }
  uint32_t word4 = (w[4] & 0xFFFFFC03u) | (levels & 0x3FC);
  w[3] = (w[3] & 0x7FF80000) | (t.fetch3() & ~0x7FF80000u);  // the filters kept
  w[1] = (w[1] & 0x800) | (physical(t.base_address()) & ~0x800u);
  word4 = (std::rotl(max_level, 2) & 0x3C) | (word4 & ~0x3Cu);
  uint32_t min_level = s.min_mip;
  if (((levels >> 6) & 0xF) < min_level) {
    min_level = (levels >> 6) & 0xF;
  }
  w[4] = (std::rotl(min_level, 6) & 0x3C0) | (word4 & ~0x3C0u);
}

}  // namespace

void SetUpStages() {
  g_stages_set_up = true;
  for (uint32_t stage = 0; stage < kStages; stage++) {
    for (uint32_t state = 0; state < 80; state += 4) {
      ApplySamplerState(g_stages[stage], state,
                        guest::LoadU32(0x82638D78 + 3 * state + 8));
    }
    ApplyTexture(g_stages[stage], 0);
  }
}

void SetTexture(uint32_t stage, uint32_t texture) {
  if (!g_stages_set_up) {
    SetUpStages();
  }
  if (stage < kStages) {
    ApplyTexture(g_stages[stage], texture);
  }
}

void SetSamplerState(uint32_t stage, uint32_t state, uint32_t value) {
  if (!g_stages_set_up) {
    SetUpStages();
  }
  if (stage < kStages) {
    ApplySamplerState(g_stages[stage], state, value);
  }
}

const SamplerState& StageSampler(uint32_t stage) {
  if (!g_stages_set_up) {
    SetUpStages();
  }
  return g_stages[stage < kStages ? stage : 0].sampler;
}

const uint32_t* TextureDescriptor(uint32_t stage) {
  if (!g_stages_set_up) {
    SetUpStages();
  }
  return g_stages[stage < kStages ? stage : 0].words;
}

void SetConstants(bool vertex, uint32_t type, uint32_t first, uint32_t count, uint32_t data) {
  switch (type) {
    case 0:  // loops: an int4 each (count, start, step)
      for (uint32_t i = 0; i < count && first + i < 16; i++, data += 16) {
        int32_t* loop = g_constants.bool_loop.loops[vertex ? 0 : 1][first + i];
        for (uint32_t c = 0; c < 3; c++) {
          loop[c] = int32_t(guest::LoadU32(data + 4 * c));
        }
      }
      break;
    case 1:  // floats: a vec4 a register
      for (uint32_t i = 0; i < count && first + i < 256; i++, data += 16) {
        for (uint32_t c = 0; c < 4; c++) {
          g_constants.floats[vertex ? 0 : 1][first + i][c] = guest::LoadFloat(data + 4 * c);
        }
      }
      break;
    case 2:  // bools: a word each, its low bit
      for (uint32_t i = 0; i < count && first + i < 128; i++, data += 4) {
        uint32_t n = first + i;
        uint32_t& word = g_constants.bool_loop.bools[vertex ? 0 : 1][n >> 5];
        word = (word & ~(1u << (n & 31))) | (guest::LoadU32(data) & 1) << (n & 31);
      }
      break;
    default:
      break;
  }
}

// The device's bool and loop block, a word at a time (what a shader's
// literal records write): words 0-3 the vertex shader's 128 bools, 4-7 the
// pixel shader's, then 32 loop constants (0-15 the vertex shader's, 16-31
// the pixel shader's), each count | start << 8 | step << 16 (step signed).
namespace {

void SetBoolLoopWord(uint32_t word, uint32_t value) {
  if (word < 8) {
    g_constants.bool_loop.bools[word >> 2][word & 3] = value;
    return;
  }
  int32_t* loop = g_constants.bool_loop.loops[(word - 8) >> 4][(word - 8) & 15];
  loop[0] = int32_t(value & 0xFF);
  loop[1] = int32_t(value >> 8 & 0xFF);
  loop[2] = int32_t(int8_t(value >> 16));
}
uint32_t BoolLoopWord(uint32_t word) {
  if (word < 8) {
    return g_constants.bool_loop.bools[word >> 2][word & 3];
  }
  const int32_t* loop = g_constants.bool_loop.loops[(word - 8) >> 4][(word - 8) & 15];
  return uint32_t(loop[0] & 0xFF) | uint32_t(loop[1] & 0xFF) << 8 | uint32_t(loop[2] & 0xFF) << 16;
}

}  // namespace

// A shader's literal constants (its file's def and defi: the float, bool
// and loop constants it sets for itself), from the records its object
// carries (the file header copied at the vertex shader's +592, the pixel
// shader's +56; its +12 the records' offset): words copied to places in
// the block from the device's +1024 (as Device::SetVertexShader and
// SetPixelShader apply them), of which the constants are taken here.
void ApplyLiterals(uint32_t header) {
  const uint32_t at = guest::LoadU32(header + 12);
  if (!at) {
    return;
  }
  const uint32_t p = header + at;
  auto put = [](uint32_t offset, uint32_t value) {
    if (offset >= 768 && offset < 768 + 4096) {  // vertex floats (device +1792)
      uint32_t i = (offset - 768) >> 2;
      g_constants.floats[0][i >> 2][i & 3] = std::bit_cast<float>(value);
    } else if (offset >= 4864 && offset < 4864 + 4096) {  // pixel floats (+5888)
      uint32_t i = (offset - 4864) >> 2;
      g_constants.floats[1][i >> 2][i & 3] = std::bit_cast<float>(value);
    } else if (offset >= 8960 && offset < 8960 + 160) {  // bools and loops (+9984)
      SetBoolLoopWord((offset - 8960) >> 2, value);
    }
  };
  auto get = [](uint32_t offset) -> uint32_t {
    if (offset >= 8960 && offset < 8960 + 160) {
      return BoolLoopWord((offset - 8960) >> 2);
    }
    return 0;
  };
  uint32_t record = p + 24;
  const uint32_t end = guest::LoadU32(p + 16) + record;
  bool masked = false;
  while (record < end) {
    const uint16_t count = guest::LoadU16(record + 2), offset = guest::LoadU16(record);
    record += 4;
    if (count == 0) {
      masked = true;
      break;
    }
    for (uint32_t i = 0; i < count; i++) {
      put(offset + 4 * i, guest::LoadU32(record + 4 * i));
    }
    record += uint32_t(count) << 2;
  }
  // The masked records: a pair of words a word (and-mask, or-value).
  while (masked && record < end) {
    uint16_t count = guest::LoadU16(record + 2);
    uint32_t offset = guest::LoadU16(record);
    record += 4;
    if (count == 0) {
      break;
    }
    do {
      put(offset, (guest::LoadU32(record) & get(offset)) | guest::LoadU32(record + 4));
      record += 8;
      offset += 4;
      count = uint16_t(count - 2);
    } while (count != 0);
  }
}

void SetVertexShader(uint32_t shader) {
  g_bindings.vertex_shader = shader;
  if (shader) {
    ApplyLiterals(shader + 592);
  }
}

void SetPixelShader(uint32_t shader) {
  g_bindings.pixel_shader = shader;
  if (shader) {
    ApplyLiterals(shader + 56);
  }
}
void SetVertexLayout(uint32_t declaration) {
  g_bindings.declaration = declaration;
  g_bindings.declaration_hash = 0;
  if (declaration) {
    // Its element count (+24) and 12-byte elements (+52).
    const uint32_t count = guest::LoadU32(declaration + 24);
    g_bindings.declaration_hash =
        base::Hash64(guest::Pointer(declaration + 52), 12 * std::min(count, 64u)) ^ count;
  }
}
void SetIndexBuffer(uint32_t buffer) { g_bindings.index_buffer = buffer; }

void SetVertexBuffer(uint32_t stream, uint32_t buffer, uint32_t offset, uint32_t stride) {
  if (stream >= 16) {
    return;
  }
  if (buffer) {
    const schema::DeviceVertexBuffer b(buffer);
    const uint32_t address = b.data() + offset;
    // The GPU's address of the CPU's (as the library's Physical).
    g_bindings.stream_fetch[stream][0] =
        ((((address >> 20) & 0xFFF) + 512) & 0x1000) + (address & 0x1FFFFFFF);
    g_bindings.stream_fetch[stream][1] = b.data_size() - offset;
  }
  g_bindings.strides[stream] = uint8_t(stride >> 2);
}

void ShaderControls(uint32_t& program_control, uint32_t& context_misc) {
  program_control = context_misc = 0;
  if (uint32_t vs = g_bindings.vertex_shader) {
    const uint32_t header = vs + 592 + guest::LoadU32(vs + 612);
    program_control = guest::LoadU32(header);
    context_misc = guest::LoadU32(header + 4);
  }
  if (uint32_t ps = g_bindings.pixel_shader) {
    const uint32_t header = ps + 56 + guest::LoadU32(ps + 76);
    program_control |= guest::LoadU32(header);
    context_misc |= guest::LoadU32(header + 4);
  }
}

void SetRenderTarget(uint32_t index, uint32_t surface) {
  if (index > 3) {
    return;
  }
  g_targets.color[index] = surface;
  if (index == 0 && (surface || g_targets.depth)) {
    Whole();
  }
}

void SetDepthTarget(uint32_t surface) {
  g_targets.depth = surface;
  if (surface && !g_targets.color[0]) {
    Whole();
  }
}

void BeginTiling(uint32_t width, uint32_t height, float clear) {
  TilingBegun(width, height, clear);
}

void EndTiling(uint32_t flags, uint32_t destination, float clear) {
  TilingEnded(flags, destination, clear);
}

Targets g_saved_targets;

void SaveTargets() { g_saved_targets = g_targets; }

void RestoreTargets() {
  // As the handler puts back each render target, the depth surface, then
  // the viewport (binding target 0 makes them whole first).
  for (uint32_t i = 0; i < 4; i++) {
    SetRenderTarget(i, g_saved_targets.color[i]);
  }
  SetDepthTarget(g_saved_targets.depth);
  g_targets.viewport = g_saved_targets.viewport;
}

void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, float min_z,
                 float max_z) {
  g_targets.viewport = {x, y, width, height, min_z, max_z};
}

void ResetRenderStates() {
  g_states = RenderStates{};
  g_blend_fields = 0x00010001;
  g_blend_enable = g_separate_alpha = false;
  StoreBlends(CurrentBlend());
}

void SetScissorRect(int32_t left, int32_t top, int32_t right, int32_t bottom) {
  g_states.scissor[0] = left;
  g_states.scissor[1] = top;
  g_states.scissor[2] = right;
  g_states.scissor[3] = bottom;
}

void SetBlendControl(uint32_t target, uint32_t value) {
  if (target < 4) {
    g_states.blend[target] = Decode(value);
  }
}

void SetRenderState(uint32_t state, uint32_t value) {
  RenderStates& s = g_states;
  switch (state) {
    case 40:  // ZEnable
      s.z_enable = value != 0;
      break;
    case 44:  // ZFunc: the GPU's compare numbers, Vulkan's order
      s.z_func = VkCompareOp(value & 7);
      break;
    case 48:  // ZWriteEnable
      s.z_write = value != 0;
      break;
    case 56:  // CullMode: bit 0 cull front faces, 1 back faces, 2 front is clockwise
      s.cull = ((value & 1) ? VK_CULL_MODE_FRONT_BIT : 0) |
               ((value & 2) ? VK_CULL_MODE_BACK_BIT : 0);
      s.front_clockwise = (value & 4) != 0;
      break;
    case 60:  // AlphaBlendEnable
      g_blend_enable = value != 0;
      StoreBlends(CurrentBlend());
      break;
    case 64:  // SeparateAlphaBlendEnable
      g_separate_alpha = value != 0;
      StoreBlends(CurrentBlend());
      break;
    case 68:  // BlendFactor: a D3DCOLOR (ARGB)
      s.blend_constants[0] = float(value >> 16 & 0xFF) / 255.0f;
      s.blend_constants[1] = float(value >> 8 & 0xFF) / 255.0f;
      s.blend_constants[2] = float(value & 0xFF) / 255.0f;
      s.blend_constants[3] = float(value >> 24 & 0xFF) / 255.0f;
      break;
    case 72:  // SrcBlend
    case 76:  // DestBlend
    case 80:  // BlendOp
      SetField(value, state == 72 ? 0 : state == 76 ? 8 : 5,
               state == 72 ? 0x1F : state == 76 ? 0x1F00 : 0xE0);
      if (g_blend_enable) {
        StoreBlends(CurrentBlend());
      }
      break;
    case 84:  // SrcBlendAlpha
    case 88:  // DestBlendAlpha
    case 92:  // BlendOpAlpha
      SetField(value, state == 84 ? 16 : state == 88 ? 24 : 21,
               state == 84 ? 0x1F0000 : state == 88 ? 0x1F000000 : 0xE00000);
      if (g_blend_enable && g_separate_alpha) {
        StoreBlends(g_blend_fields);
      }
      break;
    case 96:  // AlphaTestEnable
      s.alpha_test = value != 0;
      break;
    case 100:  // AlphaRef: 0-255
      s.alpha_ref = float(double(value)) * (1.0f / 255.0f);
      break;
    case 104:  // AlphaFunc
      s.alpha_func = VkCompareOp(value & 7);
      break;
    case 200:  // ScissorTestEnable
      s.scissor_enable = value != 0;
      break;
    case 204:  // SlopeScaleDepthBias: a float, in 1/16 pixel units on the GPU
      s.depth_bias_slope = Float(value) * 16.0f;
      break;
    case 208:  // DepthBias: a float
      s.depth_bias_constant = Float(value);
      break;
    case 212:  // ColorWriteEnable, 1, 2, 3
    case 216:
    case 220:
    case 224:
      s.color_write[(state - 212) / 4] = value & 0xF;
      break;
    case 304:  // ViewportEnable
      s.viewport_enable = value != 0;
      break;
    case 324:  // HalfPixelOffset
      s.half_pixel_offset = value != 0;
      break;
    case 328:  // PrimitiveResetEnable
      s.primitive_restart = value != 0;
      break;
    default:
      // The rest change nothing the renderer draws with: fill mode, stencil
      // (not drawn), clip planes, points, multisample mask, tessellation,
      // texture wraps (the sampler states'), high-precision blend (one
      // target format either way), the guard bands, HiStencil, the present
      // interval.
      break;
  }
}

}  // namespace render
