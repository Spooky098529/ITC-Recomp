// The glow (tx0, times the vertex color) added to the scene (tx2), the glow
// exposed when PS_CONSTANT_PARAM_0.w (its exposure) is over 0: scaled so
// its brightest channel m becomes 1 - exp(-m * exposure). Alpha 1.
#include "common.glsl"

layout(location = 0) in vec4 uv;
layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 out_color;

void main() {
  vec3 glow = sample_2d(tx0, uv.xy).rgb * v_color.rgb;
  float exposure = PS_CONSTANT_PARAM_0.w;
  if (exposure > 1e-9) {
    float brightest = max(max(glow.r, glow.g), glow.b);
    glow *= (1.0 - exp(-brightest * exposure)) / brightest;
  }
  vec4 color = vec4(sample_2d(tx2, uv.xy).rgb + glow, 1.0);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
