#include "native/draw_registers.h"

#include "guest.h"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>

namespace native {

namespace {

// The shadow's blocks: first register, registers, offset in the device.
struct ShadowBlock {
  uint32_t first_register, count, offset;
};
constexpr ShadowBlock kBlocks[] = {
    {0x2000, 16, 10240}, {0x2100, 21, 10316}, {0x2180, 5, 10400}, {0x2200, 12, 10420},
    {0x2280, 21, 10468}, {0x2300, 38, 10552}, {0x2380, 4, 10704},
};
// The fetch constants (32 of 6 words), and the bool and loop constants
// (8 words, vertex then pixel, then 32), one after the other.
constexpr uint32_t kFetchOffset = 1024, kBoolLoopOffset = 9984;

uint32_t Word(uint32_t device, uint32_t reg) {
  return guest::LoadU32(device + ShadowOffset(reg));
}
float Float(uint32_t device, uint32_t reg) {
  return std::bit_cast<float>(Word(device, reg));
}

float ClampFloat(float value, float low, float high) {
  float clamped = std::isgreater(value, low) ? value : low;
  return std::isless(clamped, high) ? clamped : high;
}
float Saturate(float value) { return ClampFloat(value, 0.0f, 1.0f); }

uint32_t ComponentCount(uint32_t format) {
  switch (format) {
    case 4: case 6: case 15:  // 16_16, 16_16_FLOAT, 32_32_FLOAT
      return 2;
    case 14:  // 32_FLOAT
      return 1;
    default:  // 8_8_8_8 (and gamma), 2_10_10_10 (and float, and as 16 bits), 16x4
      return 4;
  }
}

}  // namespace

uint32_t ShadowOffset(uint32_t reg) {
  for (const ShadowBlock& block : kBlocks) {
    if (reg >= block.first_register && reg < block.first_register + block.count) {
      return block.offset + 4 * (reg - block.first_register);
    }
  }
  return 0;
}

void DrawRegisters::Read(uint32_t device) {
  guest::CopyU32s(device + kFetchOffset, fetch, kFetchConstantWords);
  guest::CopyU32s(device + kBoolLoopOffset, bool_loop, kBoolLoopConstantWords);
  depth_control = Word(device, gpu_reg::kDepthControl);
  color_control = Word(device, gpu_reg::kColorControl);
  clip_control = Word(device, gpu_reg::kClipControl);
  mode_control = Word(device, gpu_reg::kModeControl);
  viewport_control = Word(device, gpu_reg::kViewportControl);
  edram_mode = Word(device, gpu_reg::kEdramModeControl);
  vertex_control = Word(device, gpu_reg::kVertexControl);
  color_mask = Word(device, gpu_reg::kColorMask);
  depth_info = Word(device, gpu_reg::kDepthInfo);
  for (uint32_t i = 0; i < 4; i++) {
    blend_control[i] = Word(device, gpu_reg::kBlendControl[i]);
    color_info[i] = Word(device, gpu_reg::kColorInfo[i]);
    blend_constants[i] = Float(device, gpu_reg::kBlendRed + i);
    polygon_offset[i] = Float(device, gpu_reg::kPolygonOffset + i);
  }
  vertex_index_min = Word(device, gpu_reg::kVertexIndexMin);
  vertex_index_max = Word(device, gpu_reg::kVertexIndexMax);
  vertex_index_offset = int32_t(Word(device, gpu_reg::kVertexIndexOffset));
  program_control = Word(device, gpu_reg::kProgramControl);
  context_misc = Word(device, gpu_reg::kContextMisc);
  alpha_ref = Float(device, gpu_reg::kAlphaRef);
  for (uint32_t i = 0; i < 3; i++) {
    viewport_scale[i] = Float(device, gpu_reg::kViewportXScale + 2 * i);
    viewport_offset[i] = Float(device, gpu_reg::kViewportXScale + 2 * i + 1);
  }
}

uint32_t DrawRegisters::NormalizedColorMask(uint32_t pixel_shader_writes_targets,
                                            const uint32_t color_write[4]) const {
  if (!draws_color()) {
    return 0;
  }
  uint32_t mask = 0;
  for (uint32_t i = 0; i < 4; i++) {
    if (!(pixel_shader_writes_targets & (1u << i))) {
      continue;
    }
    uint32_t components = (1u << ComponentCount(color_format(i))) - 1;
    uint32_t target = color_write[i] & components;
    if (target) {
      mask |= (target | (0xF & ~components)) << (4 * i);
    }
  }
  return mask;
}

DrawRegisters::Viewport DrawRegisters::HostViewport(bool viewport_enable,
                                                    bool half_pixel_offset) const {
  constexpr uint32_t kMax = 8192;
  Viewport v{};
  float scale_xy[2] = {viewport_enable ? viewport_scale[0] : 1.0f,
                       viewport_enable ? viewport_scale[1] : 1.0f};
  float scale_z = viewport_enable ? viewport_scale[2] : 1.0f;
  float offset_base_xy[2] = {viewport_enable ? viewport_offset[0] : 0.0f,
                             viewport_enable ? viewport_offset[1] : 0.0f};
  float offset_z = viewport_enable ? viewport_offset[2] : 0.0f;
  // The window offset (PA_SC_WINDOW_OFFSET) isn't in the shadow's blocks:
  // 0, as the register file always had it.
  float offset_add_xy[2] = {};
  if (!half_pixel_offset) {
    offset_add_xy[0] += 0.5f;
    offset_add_xy[1] += 0.5f;
  }
  if (!viewport_enable) {
    for (uint32_t i = 0; i < 2; i++) {
      v.offset[i] = 0;
      v.extent[i] = kMax;
      float extent = float(kMax);
      float pixels_to_ndc = 2.0f / extent;
      v.ndc_scale[i] = scale_xy[i] * pixels_to_ndc;
      v.ndc_offset[i] = (offset_base_xy[i] - extent * 0.5f + offset_add_xy[i]) * pixels_to_ndc;
    }
    v.z_min = 0.0f;
    v.z_max = 1.0f;
    v.ndc_scale[2] = scale_z;
    v.ndc_offset[2] = offset_z;
    return v;
  }
  for (uint32_t i = 0; i < 2; i++) {
    float offset = offset_base_xy[i] + offset_add_xy[i];
    float scale_abs = std::abs(scale_xy[i]);
    uint32_t a = uint32_t(ClampFloat(offset - scale_abs, 0.0f, float(kMax)));
    uint32_t b = uint32_t(ClampFloat(offset + scale_abs, 0.0f, float(kMax)));
    uint32_t extent = b - a;
    v.offset[i] = a;
    v.extent[i] = extent;
    if (extent) {
      float rounded = float(extent);
      v.ndc_scale[i] = scale_xy[i] * 2.0f / rounded;
      v.ndc_offset[i] = (float(offset) - (float(a) + rounded * 0.5f)) * 2.0f / rounded;
    } else {
      v.ndc_scale[i] = 1.0f;
      v.ndc_offset[i] = 0.0f;
    }
  }
  float clip_offset_z, clip_scale_z;
  if (direct3d_clip_space()) {
    clip_offset_z = offset_z;
    clip_scale_z = scale_z;
    v.ndc_scale[2] = 1.0f;
    v.ndc_offset[2] = 0.0f;
  } else {
    clip_offset_z = offset_z - scale_z;
    clip_scale_z = scale_z * 2.0f;
    v.ndc_scale[2] = 0.5f;
    v.ndc_offset[2] = 0.5f;
  }
  v.z_min = Saturate(clip_offset_z);
  v.z_max = Saturate(clip_offset_z + clip_scale_z);
  return v;
}

}  // namespace native
