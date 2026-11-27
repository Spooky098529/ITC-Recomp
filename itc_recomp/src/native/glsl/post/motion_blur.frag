// Motion blur: the velocity (tx1, stored as 0.5 + motion / 4 in x and y;
// see screen_motion.frag) scaled by PS_CONSTANT_PARAM_POSTBLUR.x and held
// within +-POSTBLUR.y, and three taps of the scene (tx0) along it, at a
// third (weight 0.6), two thirds (0.3) and all of it (0.1); times
// POSTBLUR.w, with alpha POSTBLUR.z. Where the motion is too small
// (squared length up to PS_CONSTANT_PARAM_POSTBLUR2.x), 0.
//
// MOTION_BLUR_QUALITY (--motion_blur_quality, 1-8) splits each of the three
// taps into that many along its third of the path, its weight shared
// between them: 1 is the game's three taps, 4 is twelve.
#include "common.glsl"

layout(constant_id = 10) const int MOTION_BLUR_QUALITY = 1;

layout(location = 0) in vec4 uv;

layout(location = 0) out vec4 out_color;

void main() {
  vec4 blur = PS_CONSTANT_PARAM_POSTBLUR;
  vec2 stored = sample_2d(tx1, uv.xy).xy;
  vec2 motion = vec2(stored.x * 2.0 - 1.0, 1.0 - stored.y * 2.0);
  // The weight of each third of the path, nearest first.
  const float kWeights[3] = float[](0.6, 0.3, 0.1);
  int taps = clamp(MOTION_BLUR_QUALITY, 1, 8);
  // Sampled at every pixel, so the texture level is defined, and used
  // where there's motion. Each tap's offset is held within +-POSTBLUR.y.
  vec3 sum = vec3(0.0);
  for (int third = 0; third < 3; third++) {
    for (int k = 1; k <= taps; k++) {
      float along = (float(third) + float(k) / float(taps)) / 3.0;
      vec2 offset = clamp(motion * along * blur.x, -blur.y, blur.y);
      sum += sample_2d(tx0, uv.xy + offset).rgb * (kWeights[third] / float(taps));
    }
  }
  bool moving = dot(motion, motion) > PS_CONSTANT_PARAM_POSTBLUR2.x;
  vec4 color = moving ? vec4(sum * blur.w, blur.z) : vec4(0.0);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
