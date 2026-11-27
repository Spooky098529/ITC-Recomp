#include "render/state.h"

#include <algorithm>
#include <bit>
#include <climits>
#include <cstring>

#include "schema/device_state.h"

namespace render {

namespace {

RenderStates g_states;
Targets g_targets;
// D3D's viewport as last set (before the clamp to a surface).
Viewport g_set_viewport;
Bindings g_bindings;
// While tiling: the tiles' extent together.
uint32_t g_tiling_width = 0, g_tiling_height = 0;

// The viewport clamped to the surface render target 0 (or the depth
// surface) is, as the game's SetViewport.
void ClampViewport(const Viewport& v) {
  uint32_t surface = g_targets.color[0] ? g_targets.color[0] : g_targets.depth;
  if (!surface) {
    return;  // kept as it was
  }
  const uint32_t size = schema::DeviceSurface(surface).size();
  uint32_t max_height = (std::rotl(size, 29) & 0x7FFF) + 1;
  uint32_t max_width = (std::rotl(size, 14) & 0x3FFF) + 1;
  if (g_tiling_width) {
    max_width = g_tiling_width;
    max_height = g_tiling_height;
  }
  const uint32_t right = std::min(v.width + v.x, max_width);
  const uint32_t bottom = std::min(v.height + v.y, max_height);
  g_targets.viewport = {v.x, v.y, right - v.x, bottom - v.y, v.min_z, v.max_z};
}

// The whole surface: the viewport and the scissor rectangle.
void Whole() {
  g_set_viewport = {0, 0, uint32_t(INT_MAX), uint32_t(INT_MAX), 0.0f, 1.0f};
  ClampViewport(g_set_viewport);
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

void SetVertexShader(uint32_t shader) { g_bindings.vertex_shader = shader; }
void SetPixelShader(uint32_t shader) { g_bindings.pixel_shader = shader; }
void SetVertexLayout(uint32_t declaration) { g_bindings.declaration = declaration; }
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

void BeginTiling(uint32_t width, uint32_t height) {
  g_tiling_width = width;
  g_tiling_height = height;
  ClampViewport(g_set_viewport);
}

void EndTiling() {
  g_tiling_width = g_tiling_height = 0;
  ClampViewport(g_set_viewport);
}

void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, float min_z,
                 float max_z) {
  g_set_viewport = {x, y, width, height, min_z, max_z};
  ClampViewport(g_set_viewport);
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
