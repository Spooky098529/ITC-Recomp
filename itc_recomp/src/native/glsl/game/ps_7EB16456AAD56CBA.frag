#version 450
// Pixel shader 7EB16456AAD56CBA, first found in 00000008.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 4
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 1, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 1, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 0, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 0, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 1
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 0

// tx0: sampler 0
// tx1: sampler 1
#define PS_CONSTANT_PARAM_POSTBLUR xe_fc[0]  // c40
#define PS_CONSTANT_PARAM_POSTBLUR2 xe_fc[1]  // c41
const vec4 lit254 = vec4(0.3, 0.0, 0.6, 0.0);
const vec4 lit255 = vec4(0.6666667, 0.33333334, -1.0, 0.1);

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
layout(location = 2) in vec4 xe_in_interpolator_2;
layout(location = 0) out vec4 xe_out_color_0;

vec4 xe_color0 = vec4(0.0);
bool xe_killed = false;

vec4 xe_input(int n, vec4 interpolator) {
  if (n == XE_PARAM_GEN_REGISTER) return xe_param_gen();
  return ((XE_INTERPOLATOR_MASK >> uint(n)) & 1u) != 0u ? interpolator : vec4(0.0);
}

void xe_shader() {
  precise vec4 r0 = xe_input(0, xe_in_interpolator_0);
  precise vec4 r1 = xe_input(1, xe_in_interpolator_1);
  precise vec4 r2 = xe_input(2, xe_in_interpolator_2);
  bool p = false;
  do {
    r1.xy = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 1u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xy;
    r1.zw = r1.xy + lit255.zz;
    r0.w = -r1.w + -r1.y;
    r0.z = r1.z + r1.x;
    r1.x = xe_dp2add(r0.zw, r0.zw, lit254.y);
    r1.x = (r1.x > PS_CONSTANT_PARAM_POSTBLUR2.x ? 1.0 : 0.0);
    p = !(r1.x == 0.0);
    if (p) {
      r2 = xe_mul(r0.zwzw, lit255.xxyy);
      r1.xw = xe_mul(r2.yx, PS_CONSTANT_PARAM_POSTBLUR.xx);
      { precise vec2 v = xe_mul(r2.zw, PS_CONSTANT_PARAM_POSTBLUR.xx); precise float s = xe_mul(PS_CONSTANT_PARAM_POSTBLUR.x, r0.z); r2.xy = v; r1.y = s; }
      { precise vec2 v = xe_max(r2.xy, -PS_CONSTANT_PARAM_POSTBLUR.yy); precise float s = xe_mul(PS_CONSTANT_PARAM_POSTBLUR.x, r0.w); r2.xy = v; r1.z = s; }
      r1 = xe_max(r1, -PS_CONSTANT_PARAM_POSTBLUR.yyyy);
      r2.xy = xe_min(r2.yx, PS_CONSTANT_PARAM_POSTBLUR.yy);
    }
    if (p) {
      r2.xy = r2.xy + r0.yx;
      r1 = xe_min(r1.zyxw, PS_CONSTANT_PARAM_POSTBLUR.yyyy);
      r0 = r1 + r0.yxyx;
      r1.xyz = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r0.yx, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
      r2.xyz = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r2.yx, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
      r0.xyz = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r0.wz, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    }
    if (p) {
      r0.xyz = xe_mul(r0.xyz, lit254.xxx);
      r0.xyz = xe_mul(r2.xyz, lit254.zzz) + r0.xyz;
      r0.xyz = xe_mul(r1.xyz, lit255.www) + r0.xyz;
      { precise vec3 v = xe_mul(r0.xyz, PS_CONSTANT_PARAM_POSTBLUR.www); precise float s = PS_CONSTANT_PARAM_POSTBLUR.z; r0.xyz = v; r0.w = s; }
      { precise vec4 e = r0; xe_color0 = e; }
    }
    if (!p) {
      { precise vec3 e = vec3(0.0); xe_color0.yzw = e; }
      { precise float e = (-abs(r0.x) > 0.0 ? 1.0 : 0.0); xe_color0.x = e; }
    }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
