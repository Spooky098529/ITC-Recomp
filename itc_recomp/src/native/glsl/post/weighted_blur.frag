// A blur of 8 taps at the given offsets (g_avSampleOffsets) with the given
// weights per channel (g_avSampleWeights), times the vertex color; the
// alpha is the vertex alpha.
#include "common.glsl"

layout(location = 0) in vec4 uv;
layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 out_color;

void main() {
  vec3 sum = vec3(0.0);
  for (int k = 0; k < 8; k++) {
    sum += sample_2d(tx0, uv.xy + g_avSampleOffsets(k).xy).rgb * g_avSampleWeights(k).rgb;
  }
  vec4 color = vec4(sum * v_color.rgb, v_color.a);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
