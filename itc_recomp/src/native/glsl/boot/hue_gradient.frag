// A rainbow over the screen's height: the hue goes round once from StartY.y
// to EndY.y (a second time past it), times the vertex color.
#include "common.glsl"

layout(location = 1) in vec4 v_color;
layout(location = 2) in vec4 v_position;

// Interpolator 2 can be the pixel position the hardware generates; the
// renderer says so with this constant (as in the ports).
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;

layout(location = 0) out vec4 out_color;

// Full saturation and value, the hue h from 0 to 6 by sixths of the circle.
vec3 hue_color(float h) {
  float sector = floor(h);
  float f = h - sector;
  if (sector < 1.0) return vec3(1.0, f, 0.0);
  if (sector < 2.0) return vec3(1.0 - f, 1.0, 0.0);
  if (sector < 3.0) return vec3(0.0, 1.0, f);
  if (sector < 4.0) return vec3(0.0, 1.0 - f, 1.0);
  if (sector < 5.0) return vec3(f, 0.0, 1.0);
  return vec3(1.0, 0.0, 1.0 - f);
}

void main() {
  vec4 position = XE_PARAM_GEN_REGISTER == 2 ? xe_param_gen() : v_position;
  float t = (abs(position.y) - StartY.y) * (1.0 / (EndY.y - StartY.y));
  if (t >= 1.0) t -= 1.0;
  vec4 color = vec4(hue_color(6.0 * t) * v_color.rgb, v_color.a);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
