// What every draw hands its shaders besides the game's own constants (set 1,
// binding 0), at the offsets the GLSL reads them
// (glsl/game/xe_interface.glsl's XeDrawConstants).

#pragma once

#include <cstddef>
#include <cstdint>

namespace native {

struct DrawConstants {
  uint32_t flags;               // 0: the kFlag* bits
  float alpha_test_reference;   // 4
  // The position to Vulkan's clip space: xyz * scale + offset * w.
  float ndc_scale[3];           // 8
  float ndc_offset[3];          // 20
  // Each color target's exponent bias, as a factor.
  float color_exp_bias[4];      // 32
  // A texture stage's reader scale (TextureCache::ReaderScale: a pass's
  // image handed to its destination, docs/pc-renderer.md, step 4).
  float texture_scale[32];      // 48
};
static_assert(offsetof(DrawConstants, alpha_test_reference) == 4);
static_assert(offsetof(DrawConstants, ndc_scale) == 8);
static_assert(offsetof(DrawConstants, ndc_offset) == 20);
static_assert(offsetof(DrawConstants, color_exp_bias) == 32);
static_assert(offsetof(DrawConstants, texture_scale) == 48);
static_assert(sizeof(DrawConstants) == 176);

// DrawConstants::flags (xe_interface.glsl's kXeFlag*).
constexpr uint32_t kFlagPrimitivePolygonal = 1u << 0;
constexpr uint32_t kFlagPrimitiveLine = 1u << 1;
constexpr uint32_t kFlagAlphaTestShift = 2;  // 3 bits: the compare function (7 always)

}  // namespace native
