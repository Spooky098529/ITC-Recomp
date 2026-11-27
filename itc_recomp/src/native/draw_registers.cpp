#include "native/draw_registers.h"

#include "render/state.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace native {

namespace {

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

uint32_t DrawRegisters::NormalizedColorMask(uint32_t pixel_shader_writes_targets,
                                            const uint32_t color_write[4],
                                            bool has_pixel_shader,
                                            const uint32_t formats[4]) const {
  if (!has_pixel_shader) {
    return 0;
  }
  uint32_t mask = 0;
  for (uint32_t i = 0; i < 4; i++) {
    if (!(pixel_shader_writes_targets & (1u << i))) {
      continue;
    }
    uint32_t components = (1u << ComponentCount(formats[i])) - 1;
    uint32_t target = color_write[i] & components;
    if (target) {
      mask |= (target | (0xF & ~components)) << (4 * i);
    }
  }
  return mask;
}

DrawRegisters::Viewport HostViewport(const render::Viewport& viewport, bool viewport_enable,
                                     bool half_pixel_offset) {
  constexpr uint32_t kMax = 8192;
  DrawRegisters::Viewport v{};
  // D3D's viewport transform (as the game's SetViewport builds
  // PA_CL_VPORT_*: y down), when it's on.
  float x_scale = float(double(viewport.width)) * 0.5f;
  float y_scale = float(double(viewport.height)) * 0.5f;
  float scale_xy[2] = {viewport_enable ? x_scale : 1.0f, viewport_enable ? -y_scale : 1.0f};
  float scale_z = viewport_enable ? viewport.max_z - viewport.min_z : 1.0f;
  float offset_base_xy[2] = {viewport_enable ? float(double(viewport.x)) + x_scale : 0.0f,
                             viewport_enable ? float(double(viewport.y)) + y_scale : 0.0f};
  float offset_z = viewport_enable ? viewport.min_z : 0.0f;
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
  // D3D's clip space (Z from 0 to W).
  float clip_offset_z = offset_z, clip_scale_z = scale_z;
  v.ndc_scale[2] = 1.0f;
  v.ndc_offset[2] = 0.0f;
  v.z_min = Saturate(clip_offset_z);
  v.z_max = Saturate(clip_offset_z + clip_scale_z);
  return v;
}

}  // namespace native
