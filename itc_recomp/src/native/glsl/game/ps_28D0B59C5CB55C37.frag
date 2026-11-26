#version 450
// Pixel shader 28D0B59C5CB55C37, first found in 00000042.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 3
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 3, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 3, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 2, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 2, signed
layout(set = 3, binding = 4) uniform texture2DArray xe_texture4;  // fetch constant 0, unsigned
layout(set = 3, binding = 5) uniform texture2DArray xe_texture5;  // fetch constant 0, signed
layout(set = 3, binding = 6) uniform texture2DArray xe_texture6;  // fetch constant 1, unsigned
layout(set = 3, binding = 7) uniform texture2DArray xe_texture7;  // fetch constant 1, signed
layout(set = 3, binding = 8) uniform texture2DArray xe_texture8;  // fetch constant 4, unsigned
layout(set = 3, binding = 9) uniform texture2DArray xe_texture9;  // fetch constant 4, signed
layout(set = 3, binding = 10) uniform sampler xe_sampler0;  // fetch constant 3
layout(set = 3, binding = 11) uniform sampler xe_sampler1;  // fetch constant 2
layout(set = 3, binding = 12) uniform sampler xe_sampler2;  // fetch constant 0
layout(set = 3, binding = 13) uniform sampler xe_sampler3;  // fetch constant 1
layout(set = 3, binding = 14) uniform sampler xe_sampler4;  // fetch constant 4

// tx0: sampler 0
// tx1: sampler 1
// tx2: sampler 2
// tx3: sampler 3
// tx4: sampler 4
#define PS_CONSTANT_PARAM_0 xe_fc[0]  // c33
#define PS_CONSTANT_PARAM_1 xe_fc[1]  // c34
const vec4 lit255 = vec4(1.0, 1.442695, 0.0, 0.0);

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
layout(location = 2) in vec4 xe_in_interpolator_2;
layout(location = 3) in vec4 xe_in_interpolator_3;
layout(location = 4) in vec4 xe_in_interpolator_4;
layout(location = 5) in vec4 xe_in_interpolator_5;
layout(location = 6) in vec4 xe_in_interpolator_6;
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
  precise vec4 r5 = xe_input(5, xe_in_interpolator_5);
  precise vec4 r6 = xe_input(6, xe_in_interpolator_6);
  bool p = false;
  do {
    r3.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 3u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r4.xyz = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 2u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r6.xyz = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r5.xyz = xe_tfetch2d(xe_texture6, xe_texture7, xe_sampler3, 1u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r0.xyz = xe_tfetch2d(xe_texture8, xe_texture9, xe_sampler4, 4u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r2.xyz = xe_mul(r0.xyz, r1.xyz);
    r0.xyz = r6.xyz + r5.xyz;
    r0.xyz = r0.xyz + r4.xyz;
    r2.xyz = xe_mul(-r2.zyx, PS_CONSTANT_PARAM_0.zzz);
    r2.xyz = xe_mul(r2.zxy, lit255.yyy);
    { precise vec3 v = r0.xyz + r3.xyz; precise float s = exp2(r2.y); r0.yzw = v; r0.x = s; }
    { precise vec3 v = xe_mul(r0.yzw, PS_CONSTANT_PARAM_0.yyy); precise float s = exp2(r2.z); r3.xyz = v; r0.y = s; }
    { precise vec3 v = xe_mul(r3.xyz, r1.xyz); precise float s = exp2(r2.x); r1.xzw = v; r0.z = s; }
    { precise vec3 v = -r0.zyx + lit255.xxx; precise float s = xe_max(r1.x, r1.z); r2.xyz = v; r1.y = s; }
    { precise float v = xe_max(r1.y, r1.w); precise float s = xe_max(r2.x, r2.y); r3.y = v; r1.y = s; }
    { precise float v = xe_max(r1.y, r2.z); precise float s = 1.0 / r3.y; r3.x = v; r1.y = s; }
    r1.y = xe_mul(PS_CONSTANT_PARAM_1.x, r1.y);
    r0.xyz = xe_mul(r1.wzx, r1.yyy);
    r3.yz = vec2(greaterThan(r3.xy, PS_CONSTANT_PARAM_1.yx));
    r1.yzw = mix(r0.zyx, r1.xzw, equal(r3.zzz, vec3(0.0)));
    p = !(r3.y == 0.0);
    if (p) {
      r1.x = 1.0 / r3.x;
      r0.x = xe_mul(PS_CONSTANT_PARAM_1.y, r1.x);
      r2.xyz = xe_mul(r0.xxx, r2.xyz);
    }
    { precise vec3 e = r1.yzw + r2.xyz; xe_color0.xyz = e; }
    xe_color0.w = 1.0;
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
