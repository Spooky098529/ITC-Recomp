// The graphics library's shader load at each draw (Device::LoadShaders,
// sub_822AED18) as plain C++ without its GPU packets (docs/pc-renderer.md,
// step 3): no GPU loads the microcode, so its IM_LOAD packets, the shaders'
// fences and fence list, the HiZ event and the tiling's register packets
// are left out, and a vertex shader patched as a copy (the original's copy in
// the command buffer) is patched in a buffer of our own. What the draw and
// the renderer read is kept: SQ_PROGRAM_CNTL (+10400), SQ_CONTEXT_MISC
// (+10404), SQ_INTERPOLATOR_CNTL (+10408), RB_HIZCONTROL (+10432),
// RB_MODECONTROL (+10452), the tiling's last EDRAM mode (+12440), the flags
// (+10809 bit 2, +10810 bits 4 and 7), the strides the shader is loaded with
// (+11568, +11576), the vertex shader's patch for the bound declaration
// (in place, or in the copy, with the exports remapped to the pixel shader's
// interpolators when they differ) and what it's patched for (+24, +32, +40;
// the declaration's id, +48, given from the counter at 0x829537F0).

#pragma once

#include <cstdint>
#include <functional>

namespace graphics::shader_load {

struct Calls {
  // The vertex shader's patch (sub_822AE340: the shader object, where the
  // patched instructions go, the declaration, the streams' strides).
  std::function<void(uint32_t shader, uint32_t microcode, uint32_t declaration,
                     uint32_t streams)>
      patch;
  // The exports' remap to the pixel shader's interpolators (sub_822AEAA0,
  // lifted: a scratch area, the vertex shader's header, the copy, the
  // program control word's address, the pixel shader's header).
  std::function<void(uint32_t scratch, uint32_t vertex_header, uint32_t copy,
                     uint32_t program_control, uint32_t pixel_header)>
      remap_exports;
};

// r3 the device, r4 its shader state's dirty mask (+16). `scratch` 96
// bytes of guest memory the remap may use and the program control word's
// home. Returns the mask with the bits the original adds (its callers don't
// read it).
uint64_t LoadShaders(uint32_t device, uint64_t mask, uint32_t scratch, const Calls& calls);

}  // namespace graphics::shader_load
