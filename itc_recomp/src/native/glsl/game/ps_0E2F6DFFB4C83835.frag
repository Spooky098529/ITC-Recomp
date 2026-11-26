#version 450
// Pixel shader 0E2F6DFFB4C83835, first found in 00000024.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 3
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// tx0: sampler 0
#define PS_CONSTANT_TEX_SIZE xe_fc[0]  // c32
#define PS_CONSTANT_PARAM_0 xe_fc[1]  // c33
const vec4 lit255 = vec4(0.0, 0.1, 3.0, 0.0);

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
layout(location = 7) in vec4 xe_in_interpolator_7;
layout(location = 8) in vec4 xe_in_interpolator_8;
layout(location = 9) in vec4 xe_in_interpolator_9;
layout(location = 10) in vec4 xe_in_interpolator_10;
layout(location = 11) in vec4 xe_in_interpolator_11;
layout(location = 12) in vec4 xe_in_interpolator_12;
layout(location = 13) in vec4 xe_in_interpolator_13;
layout(location = 14) in vec4 xe_in_interpolator_14;
layout(location = 15) in vec4 xe_in_interpolator_15;
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
  precise vec4 r7 = xe_input(7, xe_in_interpolator_7);
  precise vec4 r8 = xe_input(8, xe_in_interpolator_8);
  precise vec4 r9 = xe_input(9, xe_in_interpolator_9);
  precise vec4 r10 = xe_input(10, xe_in_interpolator_10);
  precise vec4 r11 = xe_input(11, xe_in_interpolator_11);
  precise vec4 r12 = xe_input(12, xe_in_interpolator_12);
  precise vec4 r13 = xe_input(13, xe_in_interpolator_13);
  precise vec4 r14 = xe_input(14, xe_in_interpolator_14);
  precise vec4 r15 = xe_input(15, xe_in_interpolator_15);
  precise vec4 r16 = vec4(0.0);
  precise vec4 r17 = vec4(0.0);
  bool p = false;
  precise float ps = 0.0;
  do {
    ps = r0.y;
    r6.y = ps;
    { precise float v = r0.x; precise float s = r0.y; ps = s; r7.z = v; r3.y = s; }
    { precise vec2 v = xe_mul(PS_CONSTANT_TEX_SIZE.yx, lit255.zz); precise float s = r0.x; ps = s; r2.zw = v; r4.z = s; }
    { precise float v = r0.x + PS_CONSTANT_TEX_SIZE.x; precise float s = PS_CONSTANT_TEX_SIZE.y + PS_CONSTANT_TEX_SIZE.y; ps = s; r6.x = v; r2.x = s; }
    { precise float v = r0.y + PS_CONSTANT_TEX_SIZE.y; precise float s = PS_CONSTANT_TEX_SIZE.x + PS_CONSTANT_TEX_SIZE.x; ps = s; r5.w = v; r2.y = s; }
    { precise float v = r5.w; precise float s = r5.w; ps = s; r7.y = v; r7.w = s; }
    { precise vec2 v = r2.yz + r0.xy; precise float s = r6.x; ps = s; r0.zw = v; r7.x = s; }
    { precise vec2 v = r2.wx + r0.xy; precise float s = r5.w; ps = s; r3.xz = v; r5.y = s; }
    { precise float v = r3.z; precise float s = r6.x; ps = s; r6.z = v; r2.z = s; }
    { precise float v = r3.x; precise float s = r0.z; ps = s; r5.x = v; r5.z = s; }
    { precise float v = r3.z; precise float s = r3.z; ps = s; r4.y = v; r4.w = s; }
    { precise float v = r0.w; precise float s = r0.z; ps = s; r2.w = v; r4.x = s; }
    { precise float v = r3.x; precise float s = r0.w; ps = s; r2.x = v; r2.y = s; }
    r13.yzw = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r15.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r6.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r14.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.zy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r16.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r9.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r7.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r11.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r7.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r10.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r5.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r12.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r5.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r5.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r4.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r7.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r6.xz, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r6.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r4.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r8.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xz, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r17.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r3.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r0.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r4.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r2.xyz = xe_mul(r0.zyx, r1.zyx);
    r3.xyz = xe_mul(r3.zyx, r1.zyx);
    r0.yzw = xe_mul(r17.zyx, r1.zyx);
    { precise vec3 v = xe_mul(r6.zyx, r1.zyx); precise float s = r4.z; ps = s; r6.xyz = v; }
    { precise vec3 v = xe_mul(r7.zyx, r1.zyx); precise float s = xe_mul(r1.z, ps); ps = s; r7.xyz = v; r0.x = s; }
    { precise vec3 v = xe_mul(r5.zyx, r1.zyx); precise float s = r8.z; ps = s; r5.yzw = v; }
    { precise vec3 v = xe_mul(r10.zyx, r1.zyx); precise float s = xe_mul(r1.z, ps); ps = s; r10.xyz = v; r5.x = s; }
    { precise vec3 v = xe_mul(r11.zyx, r1.zyx); precise float s = r12.z; ps = s; r11.xyz = v; }
    { precise vec3 v = xe_mul(r9.zyx, r1.zyx); precise float s = xe_mul(r1.z, ps); ps = s; r9.yzw = v; r9.x = s; }
    { precise vec3 v = xe_mul(r14.zyx, r1.zyx); precise float s = r16.z; ps = s; r14.xyz = v; }
    { precise vec3 v = xe_mul(r15.zyx, r1.zyx); precise float s = xe_mul(r1.z, ps); ps = s; r15.xyz = v; r13.x = s; }
    { precise vec3 v = xe_mul(r13.yzw, r1.xyz); precise float s = r16.y; ps = s; r13.yzw = v; }
    { precise vec3 v = xe_max(r13.wyz, lit255.xxx); precise float s = xe_mul(r1.y, ps); ps = s; r17.xyz = v; r13.y = s; }
    { precise vec3 v = xe_max(r17.zxy, r15.yxz); precise float s = r16.x; ps = s; r15.xyz = v; }
    { precise vec3 v = xe_max(r15.zxy, r14.zyx); precise float s = xe_mul(r1.x, ps); ps = s; r14.xyz = v; r13.z = s; }
    { precise vec3 v = xe_max(r14.zxy, r13.xzy); precise float s = r12.y; ps = s; r13.xyz = v; }
    { precise vec3 v = xe_max(r13.zxy, r9.zyw); precise float s = xe_mul(r1.y, ps); ps = s; r13.xyz = v; r9.y = s; }
    { precise vec3 v = xe_max(r13.zxy, r11.zyx); precise float s = r12.x; ps = s; r11.xyz = v; }
    { precise vec3 v = xe_max(r11.zxy, r10.xzy); precise float s = xe_mul(r1.x, ps); ps = s; r10.xyz = v; r9.z = s; }
    { precise vec3 v = xe_max(r10.zxy, r9.yxz); precise float s = r8.y; ps = s; r9.xyz = v; }
    { precise vec3 v = xe_max(r9.zxy, r5.wzy); precise float s = xe_mul(r1.y, ps); ps = s; r9.xyz = v; r5.y = s; }
    { precise vec3 v = xe_max(r9.zxy, r7.xzy); precise float s = r8.x; ps = s; r7.xyz = v; }
    { precise vec3 v = xe_max(r7.zxy, r6.yxz); precise float s = xe_mul(r1.x, ps); ps = s; r6.xyz = v; r5.z = s; }
    { precise vec3 v = xe_max(r6.zxy, r5.zyx); precise float s = r4.y; ps = s; r5.xyz = v; }
    { precise vec3 v = xe_max(r5.zxy, r0.ywz); precise float s = xe_mul(r1.y, ps); ps = s; r5.xyz = v; r0.y = s; }
    { precise vec3 v = xe_max(r5.zxy, r3.yxz); precise float s = r4.x; ps = s; r3.xyz = v; }
    { precise vec3 v = xe_max(r3.zxy, r2.zyx); precise float s = xe_mul(r1.x, ps); ps = s; r2.xyz = v; r0.z = s; }
    r1.yzw = xe_max(r2.zxy, r0.xzy);
    ps = xe_max(r1.z, r1.w);
    r1.x = ps;
    r1.x = xe_max(r1.x, r1.y);
    r1.x = r1.x + -PS_CONSTANT_PARAM_0.x;
    r1.x = xe_max(r1.x, lit255.x);
    ps = xe_mul(lit255.y, r1.x);
    r1.x = ps;
    { precise vec3 e = xe_mul(r1.xxx, r1.zwy); xe_color0.xyz = e; }
    xe_color0.w = 1.0;
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
