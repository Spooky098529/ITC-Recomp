// A separable Gaussian blur, one direction: 5, 9 or 13 taps one texel
// (PS_CONSTANT_TEX_SIZE) apart, times the vertex color (the alpha isn't).
//   TAPS        5, 9 or 13
//   HORIZONTAL  along x; otherwise along y
//
// BLOOM_QUALITY (--bloom_quality, 1-8) takes that many taps per texel over
// the same width, weighted along the kernel between its taps and scaled to
// the kernel's total: 1 is the game's blur.
#include "common.glsl"

layout(constant_id = 11) const int BLOOM_QUALITY = 1;

layout(location = 0) in vec4 uv;
layout(location = 1) in vec4 v_color;

layout(location = 0) out vec4 out_color;

// The weights from the middle tap out.
#if TAPS == 5
const float kWeights[3] = float[](0.50262, 0.144201, 0.054489);
#elif TAPS == 9
const float kWeights[5] = float[](0.148946, 0.141682, 0.121947, 0.094972, 0.066926);
#elif TAPS == 13
const float kWeights[7] = float[](0.095172, 0.093599, 0.089034, 0.081915, 0.072895, 0.062741, 0.052231);
#endif

// The kernel's weight this many texels out, between its taps.
float kernel_weight(float texels) {
  int below = int(texels);
  float between = texels - float(below);
  float a = kWeights[below];
  float b = below + 1 < kWeights.length() ? kWeights[below + 1] : 0.0;
  return mix(a, b, between);
}

void main() {
#ifdef HORIZONTAL
  vec2 step = vec2(PS_CONSTANT_TEX_SIZE.x, 0.0);
#else
  vec2 step = vec2(0.0, PS_CONSTANT_TEX_SIZE.y);
#endif
  int per_texel = clamp(BLOOM_QUALITY, 1, 8);
  int last = (kWeights.length() - 1) * per_texel;
  vec4 sum = sample_2d(tx0, uv.xy) * kWeights[0];
  float total = kWeights[0], kernel_total = kWeights[0];
  for (int k = 1; k < kWeights.length(); k++) {
    kernel_total += 2.0 * kWeights[k];
  }
  for (int k = 1; k <= last; k++) {
    float texels = float(k) / float(per_texel);
    float weight = kernel_weight(texels);
    sum += (sample_2d(tx0, uv.xy + step * texels) + sample_2d(tx0, uv.xy - step * texels)) * weight;
    total += 2.0 * weight;
  }
  sum *= kernel_total / total;
  vec4 color = vec4(sum.rgb * v_color.rgb, sum.a);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
