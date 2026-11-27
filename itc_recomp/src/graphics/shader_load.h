// The graphics library's shader load at each draw (Device::LoadShaders,
// sub_822AED18) as plain C++ without its GPU packets (docs/pc-renderer.md,
// step 3): no GPU loads the microcode, so its IM_LOAD packets, the shaders'
// fences and fence list, the HiZ event and the tiling's register packets
// are left out, and so is the vertex shader's patch for the bound
// declaration (the renderer takes the fetches' fields from the declaration,
// docs/pc-renderer.md, step 2). What the draw and
// the renderer read is kept: SQ_PROGRAM_CNTL (+10400), SQ_CONTEXT_MISC
// (+10404), SQ_INTERPOLATOR_CNTL (+10408), RB_HIZCONTROL (+10432),
// RB_MODECONTROL (+10452), the tiling's last EDRAM mode (+12440), the flags
// (+10809 bit 2, +10810 bits 4 and 7), the strides the shader is loaded with
// (+11568, +11576) and the declaration's id (+48, given from the counter at
// 0x829537F0). A vertex shader whose outputs aren't the pixel shader's inputs
// (the original remaps its exports in a copy: never seen) is logged.

#pragma once

#include <cstdint>

namespace graphics::shader_load {

// r3 the device, r4 its shader state's dirty mask (+16). `scratch` 96
// bytes of guest memory, the program control word's home. Returns the mask with the bits the original adds (its callers don't
// read it).
uint64_t LoadShaders(uint32_t device, uint64_t mask, uint32_t scratch);

}  // namespace graphics::shader_load
