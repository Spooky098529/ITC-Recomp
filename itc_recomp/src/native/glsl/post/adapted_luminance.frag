// The eye's adapted luminance: last frame's (tx1) moved 1% of the way to
// this frame's (tx0), both read from the middle of 1x1 textures; times the
// vertex color.
#include "common.glsl"

layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 out_color;

void main() {
  float current = sample_2d(tx0, vec2(0.5)).x;
  float previous = sample_2d(tx1, vec2(0.5)).x;
  float adapted = previous + (current - previous) * 0.010050506;
  vec4 color = vec4(adapted * v_color.rgb, v_color.a);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
