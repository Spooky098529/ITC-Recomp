// The final color adjustments, from the game's display settings: the scene
// (tx0) clamped to 0-1; with PS_CONSTANT_BOOL_USE_HSV_SCALE, the hue moved
// by PS_CONSTANT_PARAM_HSV_SCALE.x (in turns) and the saturation and value
// scaled by .y and .z; then the brightness (PS_CONSTANT_PARAM_CARIBRATE.x,
// added); with PS_CONSTANT_BOOL_USE_GAMMA, a gamma per channel
// (PS_CONSTANT_PARAM_GAMMA.xyz, as exponents); the contrast (CARIBRATE.y,
// around 0.5); each of those clamped to 0-1. Last, the color moved toward
// its luminance by CARIBRATE.z (less saturation).
#include "common.glsl"

layout(location = 0) in vec4 uv;

layout(location = 0) out vec4 out_color;

// Hue (0-1), saturation and value of a color.
vec3 rgb_to_hsv(vec3 color) {
  float value = max(max(color.r, color.g), color.b);
  float delta = value - min(min(color.r, color.g), color.b);
  if (delta == 0.0) return vec3(0.0, 0.0, value);
  // How far each channel is below the top, as a share of the range.
  vec3 below = ((value - color) / 6.0 + delta * 0.5) * (1.0 / delta);
  float hue;
  if (color.r == value) {
    hue = below.b - below.g;
  } else if (color.g == value) {
    hue = 1.0 / 3.0 + below.r - below.b;
  } else {
    hue = 2.0 / 3.0 + below.g - below.r;
  }
  if (hue < 0.0) hue += 1.0;
  if (hue > 1.0) hue -= 1.0;
  return vec3(hue, delta * (1.0 / value), value);
}

vec3 hsv_to_rgb(vec3 hsv) {
  float hue = fract(hsv.x), saturation = hsv.y, value = hsv.z;
  if (saturation == 0.0) return vec3(value);
  float h = hue * 6.0;
  float sector = floor(h);
  float f = h - sector;
  float p = value * (1.0 - saturation);
  float q = value * (1.0 - saturation * f);
  float t = value * (1.0 - saturation * (1.0 - f));
  if (sector == 0.0) return vec3(value, t, p);
  if (sector == 1.0) return vec3(q, value, p);
  if (sector == 2.0) return vec3(p, value, t);
  if (sector == 3.0) return vec3(p, q, value);
  if (sector == 4.0) return vec3(t, p, value);
  return vec3(value, p, q);
}

void main() {
  vec4 color = clamp(sample_2d(tx0, uv.xy), 0.0, 1.0);
  if (PS_CONSTANT_BOOL_USE_HSV_SCALE) {
    vec4 scale = PS_CONSTANT_PARAM_HSV_SCALE;
    vec3 hsv = rgb_to_hsv(color.rgb);
    color.rgb = hsv_to_rgb(vec3(hsv.x + scale.x, hsv.y * scale.y, hsv.z * scale.z));
  }
  vec4 calibration = PS_CONSTANT_PARAM_CARIBRATE;
  color = clamp(color + calibration.x, 0.0, 1.0);
  if (PS_CONSTANT_BOOL_USE_GAMMA) {
    // pow(color, gamma) as the console works it out: the log of the
    // absolute value, and 0 times the log of 0 (an exponent of 0 on black)
    // is 0, giving 1.
    vec3 log_color = log2(abs(color.rgb));
    vec3 gamma = PS_CONSTANT_PARAM_GAMMA.xyz;
    vec3 power = vec3(times(log_color.r, gamma.x), times(log_color.g, gamma.y),
                      times(log_color.b, gamma.z));
    color.rgb = clamp(exp2(power), 0.0, 1.0);
  }
  color = clamp((color - 0.5) * calibration.y + 0.5, 0.0, 1.0);
  float luminance = dot(color.rgb, vec3(0.299, 0.587, 0.114));
  color.rgb = mix(color.rgb, vec3(luminance), calibration.z);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
