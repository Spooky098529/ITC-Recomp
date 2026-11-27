// A draw's GPU state as the renderer takes it (docs/pc-renderer.md, step 3):
// read straight from the graphics device's register shadow (the game's
// Direct3D keeps it current: its setters write it, the draw doesn't change
// it) when a draw is made, with our own decoding of the few registers the
// renderer uses. In place of the SDK's register file, filled word by word
// from the device's dirty masks (ReadRegisters, gone), and its draw_util.

#pragma once

#include <cstdint>

namespace render {
struct Viewport;
}

namespace native {

// The device's shadow of the registers the renderer reads: a register's word
// is at device + ShadowOffset(register) (the blocks the draw path flushes;
// see "The register shadow as a whole" in docs/gpu-functions.md). 0 for a
// register outside them.
uint32_t ShadowOffset(uint32_t reg);

namespace gpu_reg {
// Register numbers (the GPU's, as the shadow's blocks are laid out).
constexpr uint32_t kColorInfo[4] = {0x2001, 0x2003, 0x2004, 0x2005};
constexpr uint32_t kDepthInfo = 0x2002;
constexpr uint32_t kVertexIndexMax = 0x2100, kVertexIndexMin = 0x2101,
                   kVertexIndexOffset = 0x2102;
constexpr uint32_t kColorMask = 0x2104;
constexpr uint32_t kBlendRed = 0x2105;  // to 0x2108: red, green, blue, alpha
constexpr uint32_t kAlphaRef = 0x210E;
constexpr uint32_t kViewportXScale = 0x210F;  // to 0x2114: x scale, offset, y.., z..
constexpr uint32_t kProgramControl = 0x2180, kContextMisc = 0x2181;
constexpr uint32_t kDepthControl = 0x2200;
constexpr uint32_t kBlendControl[4] = {0x2201, 0x2209, 0x220A, 0x220B};
constexpr uint32_t kColorControl = 0x2202;
constexpr uint32_t kClipControl = 0x2204;
constexpr uint32_t kModeControl = 0x2205;      // PA_SU_SC_MODE_CNTL
constexpr uint32_t kViewportControl = 0x2206;  // PA_CL_VTE_CNTL
constexpr uint32_t kEdramModeControl = 0x2208; // RB_MODECONTROL
constexpr uint32_t kVertexControl = 0x2302;    // PA_SU_VTX_CNTL
constexpr uint32_t kPolygonOffset = 0x2380;    // to 0x2383: front scale, offset, back..
}  // namespace gpu_reg

constexpr uint32_t kFetchConstantWords = 32 * 6;
constexpr uint32_t kBoolLoopConstantWords = 8 + 32;

struct DrawRegisters {
  // The fetch constants (textures, 6 words each; vertex streams, 2 each in
  // the same space) and the bool and loop constants, in host order.
  uint32_t fetch[kFetchConstantWords];
  uint32_t bool_loop[kBoolLoopConstantWords];

  uint32_t depth_control, color_control, clip_control, mode_control, viewport_control,
      edram_mode, vertex_control, color_mask, depth_info;
  uint32_t blend_control[4], color_info[4];
  uint32_t vertex_index_min, vertex_index_max;
  int32_t vertex_index_offset;
  uint32_t program_control, context_misc;
  float blend_constants[4], alpha_ref;
  float viewport_scale[3], viewport_offset[3];  // x, y, z
  float polygon_offset[4];  // front scale and offset, back scale and offset

  // Everything above from the device's shadow.
  void Read(uint32_t device);

  // The decoding (the GPU's fields).
  bool stencil_enable() const { return depth_control & 1; }
  bool z_enable() const { return depth_control >> 1 & 1; }
  bool z_write() const { return depth_control >> 2 & 1; }
  uint32_t z_func() const { return depth_control >> 4 & 7; }
  uint32_t alpha_func() const { return color_control & 7; }
  bool alpha_test() const { return color_control >> 3 & 1; }
  bool cull_front() const { return mode_control & 1; }
  bool cull_back() const { return mode_control >> 1 & 1; }
  bool front_clockwise() const { return mode_control >> 2 & 1; }
  bool polygon_offset_front() const { return mode_control >> 11 & 1; }
  bool polygon_offset_back() const { return mode_control >> 12 & 1; }
  bool polygon_offset_lines() const { return mode_control >> 13 & 1; }
  bool window_offset() const { return mode_control >> 16 & 1; }
  bool primitive_restart() const { return mode_control >> 21 & 1; }
  bool clip_disable() const { return clip_control >> 16 & 1; }
  bool direct3d_clip_space() const { return clip_control >> 19 & 1; }
  // RB_MODECONTROL: 4 color and depth, 5 depth only.
  bool draws_color() const { return (edram_mode & 7) == 4; }
  bool draws_depth() const { return (edram_mode & 7) == 4 || (edram_mode & 7) == 5; }
  uint32_t color_format(uint32_t target) const { return color_info[target] >> 16 & 0xF; }
  int32_t color_exp_bias(uint32_t target) const {
    return int32_t(color_info[target] << 6) >> 26;
  }
  bool depth_float24() const { return depth_info >> 16 & 1; }

  // The color write mask (4 bits a target) for the targets the pixel shader
  // writes, from each target's write enables (the render states,
  // render/state.h), components the format lacks counted as written.
  // Nothing without a pixel shader (depth only).
  // formats: each target's color format (its surface's color info, bits
  // 16-19).
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
// positions into it (ndc_scale, ndc_offset), from D3D's viewport and the
// ViewportEnable and HalfPixelOffset render states (render/state.h), as the
// SDK's GetHostViewportInfo for this renderer's arguments: no resolution
// scale, origin top left, 8192 pixels at most, D3D's clip space, Z neither
// converted nor written by pixel shaders.
DrawRegisters::Viewport HostViewport(const render::Viewport& viewport, bool viewport_enable,
                                     bool half_pixel_offset);

}  // namespace native
