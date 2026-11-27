// The renderer's own draw state (docs/pc-renderer.md, "The seam: the
// handlers call our renderer"): what the render command replay's handlers
// set, as Vulkan values, mapped once where the game's Direct3D call is
// handled. The draw builds its pipeline and dynamic state from it instead of
// reading the graphics device's register shadow.
//
// This first part holds the render states (command 1, by the game's render
// state numbers: a byte offset into the device's setter table, the table
// at 0x826388F8 for their setters and defaults) and the blend controls
// (command 3). Render thread only.

#pragma once

#include <cstdint>

#include "native/vulkan_functions.h"

namespace render {

struct BlendState {
  bool enable = false;
  VkBlendFactor src_color = VK_BLEND_FACTOR_ONE, dst_color = VK_BLEND_FACTOR_ZERO;
  VkBlendOp color_op = VK_BLEND_OP_ADD;
  VkBlendFactor src_alpha = VK_BLEND_FACTOR_ONE, dst_alpha = VK_BLEND_FACTOR_ZERO;
  VkBlendOp alpha_op = VK_BLEND_OP_ADD;
  bool operator==(const BlendState&) const = default;
};

struct RenderStates {
  // Depth.
  bool z_enable = true, z_write = true;
  VkCompareOp z_func = VK_COMPARE_OP_LESS_OR_EQUAL;
  // Depth bias: the slope in the GPU's 1/16 pixel units (D3D's slope scale
  // times 16) and the constant, both faces; on when either isn't 0.
  float depth_bias_slope = 0.0f, depth_bias_constant = 0.0f;
  // Rasterization.
  VkCullModeFlags cull = VK_CULL_MODE_BACK_BIT;
  bool front_clockwise = true;
  bool primitive_restart = false;
  // The viewport transform applied (D3D's ViewportEnable; off: positions
  // are already in pixels and nothing is clipped).
  bool viewport_enable = true;
  // Pixel centers at half pixels (D3D's HalfPixelOffset).
  bool half_pixel_offset = false;
  bool scissor_enable = false;
  // The scissor rectangle (left, top, right, bottom; guest pixels).
  int32_t scissor[4] = {};
  // Color: each target's blend and write mask (RGBA bits), the blend
  // constants.
  BlendState blend[4];
  uint32_t color_write[4] = {0xF, 0xF, 0xF, 0xF};
  float blend_constants[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  // The alpha test: its compare (VK_COMPARE_OP_ALWAYS when off) and
  // reference (0-1).
  VkCompareOp alpha_func = VK_COMPARE_OP_ALWAYS;
  bool alpha_test = false;
  float alpha_ref = 0.0f;
};

// The viewport, as D3D's (pixels; Z range 0-1), clamped to the bound
// surface as the game's Direct3D clamps it.
struct Viewport {
  uint32_t x = 0, y = 0, width = 0, height = 0;
  float min_z = 0.0f, max_z = 1.0f;
};

// What's drawn into: the game's surface objects (render targets 0-3, the
// depth surface; 0 none) and the viewport.
struct Targets {
  uint32_t color[4] = {};
  uint32_t depth = 0;
  Viewport viewport;
};

// Commands 23, 24, 29, 34 and 35: a render target or the depth surface
// bound. Binding render target 0 (or the depth surface without one) makes
// the viewport and scissor the whole surface, as SetRenderTarget's
// SetSurfaceInfo does.
void SetRenderTarget(uint32_t index, uint32_t surface);
void SetDepthTarget(uint32_t surface);
// Commands 30 and 35: the viewport (pixels, Z range).
void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height, float min_z,
                 float max_z);
// Commands 32 and 33: predicated tiling begun with its tiles' rectangles
// (the viewport is then clamped to the tiles together, as the game's
// Direct3D clamps it while tiling holds the bound surfaces), and ended.
void BeginTiling(uint32_t width, uint32_t height);
void EndTiling();
const Targets& BoundTargets();

// The game's render state `state` (a byte offset: 40 ZEnable, 44 ZFunc, ...)
// set to `value`, as the device's setter for it gives it meaning.
void SetRenderState(uint32_t state, uint32_t value);
// Command 3: a target's blend control as the GPU's register holds it
// (source, op, destination for color at bits 0, 5, 8; for alpha at 16, 21,
// 24).
void SetBlendControl(uint32_t target, uint32_t value);
// Command 31: the scissor rectangle.
void SetScissorRect(int32_t left, int32_t top, int32_t right, int32_t bottom);
// The game's defaults (Device::SetDefaultState's, from the table).
void ResetRenderStates();

const RenderStates& States();

// The GPU's blend factor and op numbers (the blend control's fields, and
// D3D's on the 360) as Vulkan's.
VkBlendFactor BlendFactor(uint32_t factor);
VkBlendOp BlendOp(uint32_t op);

}  // namespace render
