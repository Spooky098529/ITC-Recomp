// Tone mapping by the adapted luminance (tx1, the middle of a 1x1 texture):
// the scene (tx0) scaled to the key PS_CONSTANT_PARAM_0.w over the
// luminance, then x / (x + KNEE).
//   LUMINANCE_BIAS  added to the luminance (no divide by 0)
//   KNEE            where the curve bends
//   BRIGHT_PASS     only what's over 1 (for the glow), times the vertex
//                   color, with the vertex alpha; otherwise alpha 1
#include "common.glsl"

layout(location = 0) in vec4 uv;
#ifdef BRIGHT_PASS
layout(location = 1) in vec4 v_color;
#endif

layout(location = 0) out vec4 out_color;

void main() {
  float luminance = sample_2d(tx1, vec2(0.5)).x + LUMINANCE_BIAS;
  vec3 scene = sample_2d(tx0, uv.xy).rgb * (PS_CONSTANT_PARAM_0.w / luminance);
#ifdef BRIGHT_PASS
  scene = max(scene - 1.0, 0.0);
#endif
  vec4 color = vec4(scene / (scene + KNEE), 1.0);
#ifdef BRIGHT_PASS
  color = vec4(color.rgb * v_color.rgb, v_color.a);
#endif
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
