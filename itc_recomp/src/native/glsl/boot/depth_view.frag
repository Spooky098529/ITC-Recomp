// A texture's x channel as gray: (x - 0.1) / 3000. Alpha 1.
#include "common.glsl"

layout(location = 0) in vec4 uv;

layout(location = 0) out vec4 out_color;

void main() {
  float value = (sample_2d(Texture, uv.xy).x - 0.1) * 0.00033334445;
  vec4 color = vec4(vec3(value), 1.0);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
