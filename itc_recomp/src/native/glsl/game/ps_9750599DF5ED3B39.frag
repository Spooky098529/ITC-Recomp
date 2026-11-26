#version 450
// Pixel shader 9750599DF5ED3B39, first found in 00000040.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 2
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 3, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 3, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 2, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 2, signed
layout(set = 3, binding = 4) uniform texture2DArray xe_texture4;  // fetch constant 0, unsigned
layout(set = 3, binding = 5) uniform texture2DArray xe_texture5;  // fetch constant 0, signed
layout(set = 3, binding = 6) uniform texture2DArray xe_texture6;  // fetch constant 1, unsigned
layout(set = 3, binding = 7) uniform texture2DArray xe_texture7;  // fetch constant 1, signed
layout(set = 3, binding = 8) uniform sampler xe_sampler0;  // fetch constant 3
layout(set = 3, binding = 9) uniform sampler xe_sampler1;  // fetch constant 2
layout(set = 3, binding = 10) uniform sampler xe_sampler2;  // fetch constant 0
layout(set = 3, binding = 11) uniform sampler xe_sampler3;  // fetch constant 1

// tx0: sampler 0
// tx1: sampler 1
// tx2: sampler 2
// tx3: sampler 3
#define PS_CONSTANT_PARAM_0 xe_fc[0]  // c33
#define PS_CONSTANT_PARAM_1 xe_fc[1]  // c34

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
layout(location = 2) in vec4 xe_in_interpolator_2;
layout(location = 3) in vec4 xe_in_interpolator_3;
layout(location = 4) in vec4 xe_in_interpolator_4;
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
  precise vec4 r3 = xe_input(3, xe_in_interpolator_3);
  precise vec4 r4 = xe_input(4, xe_in_interpolator_4);
  bool p = false;
  do {
    r2.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 3u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r3.xyz = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 2u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r4.xyz = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r0.xyz = xe_tfetch2d(xe_texture6, xe_texture7, xe_sampler3, 1u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r0.xyz = r4.xyz + r0.xyz;
    r0.xyz = r0.xyz + r3.xyz;
    r0.xyz = r0.xyz + r2.xyz;
    r0.xyz = xe_mul(r0.xyz, PS_CONSTANT_PARAM_0.yyy);
    r0.xyz = xe_mul(r0.zyx, r1.zyx);
    r1.x = xe_max(r0.z, r0.y);
    r1.x = xe_max(r1.x, r0.x);
    r1.y = (r1.x > PS_CONSTANT_PARAM_1.x ? 1.0 : 0.0);
    p = !(r1.y == 0.0);
    if (p) {
      r1.y = 1.0 / r1.x;
      r1.z = xe_mul(PS_CONSTANT_PARAM_1.x, r1.y);
      r0.xyz = xe_mul(r0.xyz, r1.zzz);
    }
    { precise vec3 e = r0.zyx; xe_color0.xyz = e; }
    xe_color0.w = 1.0;
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
