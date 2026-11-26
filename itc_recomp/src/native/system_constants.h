// The constants every draw hands its shaders besides the game's own (set 1,
// binding 0), at the offsets the GLSL reads them
// (glsl/game/xe_interface.glsl): the layout the SDK's translator gave its
// "system constants", kept so the ported shaders read the same words.

#pragma once

#include <cstddef>
#include <cstdint>

namespace native {

struct SystemConstants {
  uint32_t flags;                      // 0: the kFlag* bits
  uint32_t vertex_index_load_address;  // 4
  uint32_t vertex_index_endian;        // 8
  uint32_t line_loop_closing_index;    // 12
  int32_t vertex_base_index;           // 16
  uint32_t unused_20[26];
  float ndc_scale[3];                  // 124
  uint32_t sprite_mode;                // 136 (SBX's; not set: ports make no point sprites)
  float ndc_offset[3];                 // 140
  uint32_t unused_152[6];
  uint32_t texture_swizzled_signs[8];  // 176: a byte a fetch constant
  uint32_t unused_208[16];
  float alpha_test_reference;          // 272
  uint32_t unused_276[3];
  float color_exp_bias[4];             // 288
  uint32_t unused_304[48];
  uint32_t vertex_index_min;           // 496
  uint32_t vertex_index_max;           // 500
};
static_assert(offsetof(SystemConstants, vertex_base_index) == 16);
static_assert(offsetof(SystemConstants, ndc_scale) == 124);
static_assert(offsetof(SystemConstants, ndc_offset) == 140);
static_assert(offsetof(SystemConstants, texture_swizzled_signs) == 176);
static_assert(offsetof(SystemConstants, alpha_test_reference) == 272);
static_assert(offsetof(SystemConstants, color_exp_bias) == 288);
static_assert(offsetof(SystemConstants, vertex_index_min) == 496);
static_assert(sizeof(SystemConstants) == 504);

// SystemConstants::flags (xe_interface.glsl's kXeFlag*).
constexpr uint32_t kFlagXYDividedByW = 1u << 8;
constexpr uint32_t kFlagZDividedByW = 1u << 9;
constexpr uint32_t kFlagWNotReciprocal = 1u << 10;
constexpr uint32_t kFlagPrimitivePolygonal = 1u << 11;
constexpr uint32_t kFlagPrimitiveLine = 1u << 12;
constexpr uint32_t kFlagAlphaTestShift = 16;  // 3 bits: the compare function (7 always)

}  // namespace native
