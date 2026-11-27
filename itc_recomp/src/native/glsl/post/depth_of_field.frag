// Depth of field: the scene (tx1) times PS_CONSTANT_PARAM_DOF.w, with the
// alpha (0 sharp to 0.92 blurred) from the depth (tx0) as a distance past
// the focus (PS_CONSTANT_PARAM_DOF.x): beyond it, blurred over DOF.z;
// nearer, over DOF.y.
#include "common.glsl"

layout(location = 0) in vec4 uv;

layout(location = 0) out vec4 out_color;

void main() {
  float depth = sample_2d(tx0, uv.xy).x;
  vec4 camera = PS_CONSTANT_PARAM_CAMERA, dof = PS_CONSTANT_PARAM_DOF;
  // From the depth buffer's value to a distance from the camera.
  float distance = -camera.w / ((1.0 - depth) * camera.z - camera.y);
  float from_focus = distance - dof.x;
  float blur = from_focus > 0.0 ? clamp((from_focus - dof.z) / dof.z, 0.0, 1.0)
                                : clamp((-from_focus - dof.y) / dof.y, 0.0, 1.0);
  vec4 color = vec4(sample_2d(tx1, uv.xy).rgb * dof.w, 0.92 * blur);
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
