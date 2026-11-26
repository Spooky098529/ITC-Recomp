// A constant color (DLightColor(2), which the game sets for these draws),
// optionally times a texture.
//   UV_REG  the texture's coordinates (with tx0)
#include "common.glsl"

#ifdef UV_REG
layout(location = UV_REG) in vec4 uv;
#endif

layout(location = 0) out vec4 out_color;

void main() {
  vec4 color = DLightColor(2);
#ifdef UV_REG
  color *= sample_2d(tx0, uv.xy);
#endif
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
