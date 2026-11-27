// The glow's four blur levels (tx0-tx3) added, scaled by
// PS_CONSTANT_PARAM_0.y and the vertex color, with the brightest channel
// held to PS_CONSTANT_PARAM_1.x. Alpha 1.
//   EXPOSURE   also adds the scene (tx4) exposed: 1 - exp(-scene * color *
//              PS_CONSTANT_PARAM_0.z), its brightest channel held to
//              PS_CONSTANT_PARAM_1.y
#include "common.glsl"

layout(location = 0) in vec4 uv;
layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 out_color;

// Scales a color down so its brightest channel is at most limit.
vec3 hold_brightest(vec3 color, float limit) {
  float brightest = max(max(color.r, color.g), color.b);
  return brightest > limit ? color * (limit / brightest) : color;
}

void main() {
  vec3 glow = sample_2d(tx0, uv.xy).rgb + sample_2d(tx1, uv.xy).rgb +
              sample_2d(tx2, uv.xy).rgb + sample_2d(tx3, uv.xy).rgb;
  glow = hold_brightest(glow * PS_CONSTANT_PARAM_0.y * v_color.rgb, PS_CONSTANT_PARAM_1.x);
  vec3 color = glow;
#ifdef EXPOSURE
  vec3 scene = sample_2d(tx4, uv.xy).rgb * v_color.rgb;
  color += hold_brightest(1.0 - exp(-scene * PS_CONSTANT_PARAM_0.z), PS_CONSTANT_PARAM_1.y);
#endif
  vec4 result = vec4(color, 1.0);
  if (!xe_alpha_test_passes(result.a)) discard;
  out_color = xe_color_output(result, 0);
}
