// The screen-space motion between two positions of a vertex (in clip
// space), stored as 0.5 + motion / 4: the velocity the motion blur reads.
#include "common.glsl"

layout(location = 0) in vec4 v_position;
layout(location = 1) in vec4 v_old_position;

layout(location = 0) out vec4 out_color;

void main() {
  vec2 motion = v_position.xy * (1.0 / v_position.w) - v_old_position.xy * (1.0 / v_old_position.w);
  vec4 color = vec4(motion * 0.25 + 0.5, 0.0, 1.0);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
