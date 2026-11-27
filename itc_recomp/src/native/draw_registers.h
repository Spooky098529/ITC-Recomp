// What the draw still keeps beside the renderer's own state
// (render/state.h): the fetch constant block the shaders' uniform takes
// (the sampler stages' texture descriptors and the streams' vertex fetch
// constants, built from render:: at each draw), the color write mask's
// normalization and the host viewport (docs/pc-renderer.md, step 3: the
// device's register shadow is no longer read).

#pragma once

#include <cstdint>

namespace render {
struct Viewport;
}

namespace native {

constexpr uint32_t kFetchConstantWords = 32 * 6;

struct DrawRegisters {
  // The fetch constants (textures, 6 words each; vertex streams, 2 each in
  // the same space), in host order.
  uint32_t fetch[kFetchConstantWords];

  // The color write mask (4 bits a target) for the targets the pixel shader
  // writes, from each target's write enables (the render states), components
  // the format lacks counted as written; nothing without a pixel shader
  // (depth only). formats: each target's color format (its surface's color
  // info, bits 16-19).
  uint32_t NormalizedColorMask(uint32_t pixel_shader_writes_targets,
                               const uint32_t color_write[4], bool has_pixel_shader,
                               const uint32_t formats[4]) const;

  struct Viewport {
    uint32_t offset[2], extent[2];
    float z_min, z_max;
    float ndc_scale[3], ndc_offset[3];
  };
};

// The host viewport (integer pixels, unscaled) and how the shaders rescale
// positions into it (ndc_scale, ndc_offset), from D3D's viewport (clamped to
// the target) and the ViewportEnable and HalfPixelOffset render states:
// origin top left, 8192 pixels at most, D3D's clip space, Z neither
// converted nor written by pixel shaders.
DrawRegisters::Viewport HostViewport(const render::Viewport& viewport, bool viewport_enable,
                                     bool half_pixel_offset);

}  // namespace native
