#version 450
// Pixel shader CC253DC700BC8028, first found in 00000046.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 2
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 2, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 2, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 0
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 2

// tx0: sampler 0
// tx2: sampler 2
#define PS_CONSTANT_PARAM_0 xe_fc[0]  // c33
const vec4 lit255 = vec4(1e-09, 1.442695, 1.0, 0.0);

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
    r2.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r0.z = (PS_CONSTANT_PARAM_0.w > lit255.x ? 1.0 : 0.0);
    { precise vec3 v = xe_mul(r2.xyz, r1.xyz); p = !(r0.z == 0.0); precise float s = (p ? 0.0 : 1.0); r1.yzw = v; }
    if (p) {
      r1.x = xe_max(r1.y, r1.z);
      r0.w = xe_max(r1.x, r1.w);
      r1.x = xe_mul(-r0.w, PS_CONSTANT_PARAM_0.w);
      r1.x = xe_mul(lit255.y, r1.x);
      r1.x = exp2(r1.x);
      { precise float v = -r1.x + lit255.z; precise float s = 1.0 / r0.w; r0.z = v; r1.x = s; }
    }
    if (p) {
      r0.z = xe_mul(r0.z, r1.x);
      r1.yzw = xe_mul(r1.yzw, r0.zzz);
    }
    r2.xyz = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 2u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    { precise vec3 e = r2.xyz + r1.yzw; xe_color0.xyz = e; }
    xe_color0.w = 1.0;
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
