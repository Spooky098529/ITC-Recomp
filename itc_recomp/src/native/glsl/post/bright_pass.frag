// A bright pass for the glow: a block of GRID x GRID taps one texel
// (PS_CONSTANT_TEX_SIZE) apart, from the pixel down and right, each times
// the vertex color; reduced to one color, then scaled by how far its
// brightest channel is over the threshold (PS_CONSTANT_PARAM_0.x), times
// 0.1. Alpha 1.
//   GRID       3 or 4
//   AVERAGE    the taps' average
//   MAXIMUM    the taps' maximum, per channel (and at least 0)
//   EXPONENT   the result is exp() of the averaged one
//   LOG_LUMINANCE  the average of the taps' log luminance, in all channels
#include "common.glsl"

layout(location = 0) in vec4 uv;
layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 out_color;

void main() {
  vec3 color = vec3(0.0);
  float log_luminance = 0.0;
  for (int y = 0; y < GRID; y++) {
    for (int x = 0; x < GRID; x++) {
      vec2 at = uv.xy + vec2(float(x), float(y)) * PS_CONSTANT_TEX_SIZE.xy;
      vec3 tap = sample_2d(tx0, at).rgb * v_color.rgb;
#if defined(MAXIMUM)
      color = max(color, tap);
#else
      color += tap;
#endif
      // The console's log takes the absolute value.
      log_luminance += log(abs(dot(tap, vec3(0.299, 0.587, 0.114)) + 1e-5));
    }
  }
#if defined(AVERAGE) || defined(EXPONENT)
  color /= float(GRID * GRID);
#elif defined(LOG_LUMINANCE)
  color = vec3(log_luminance / float(GRID * GRID));
#endif
  float brightest = max(max(color.r, color.g), color.b);
  color *= max(brightest - PS_CONSTANT_PARAM_0.x, 0.0) * 0.1;
#ifdef EXPONENT
  color = exp(color);
#endif
  vec4 result = vec4(color, 1.0);
  if (!xe_alpha_test_passes(result.a)) discard;
  out_color = xe_color_output(result, 0);
}
