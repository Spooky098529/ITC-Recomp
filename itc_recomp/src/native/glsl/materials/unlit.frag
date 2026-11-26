// The game's unlit material: the vertex color, times a color texture if
// there is one.
//
// variants.txt picks the features for each of the game's shaders:
//   COLOR_REG, UV0_REG  the interpolators: the vertex color and the
//                       texture coordinates
//   TX0_UV              color texture tx0 and its coordinates
#include "common.glsl"

// The shader's family for the renderer (passes.cpp, ShaderFamily: 1 the
// models' lit material, 2 their unlit one, 3 the course's), read from the
// SPIR-V; nothing reads it here.
layout(constant_id = 901) const int SBX_FAMILY = 2;

layout(location = COLOR_REG) in vec4 v_color;
#ifdef UV0_REG
layout(location = UV0_REG) in vec4 uv0;
#endif

layout(location = 0) out vec4 out_color;

void main() {
  vec4 color = v_color;
#ifdef TX0_UV
  vec4 texel = sample_2d(tx0, TX0_UV.xy);
  color.rgb *= texel.rgb;
  color.a *= texel.a * ModTexAlpha.x;
#endif
  color.a = clamp(color.a, 0.0, 1.0);

  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
