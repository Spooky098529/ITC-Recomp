#version 450
// Pixel shader F21079DB9D2217A1, first found in 00000032.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 5
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// tx0: sampler 0
#define PS_CONSTANT_TEX_SIZE xe_fc[0]  // c32
#define PS_CONSTANT_PARAM_0 xe_fc[1]  // c33
const vec4 lit253 = vec4(0.6931472, 1.0, 0.1, 0.11111111);
const vec4 lit254 = vec4(0.114, 0.299, 0.587, 1e-05);
const vec4 lit255 = vec4(0.0, 0.0, 0.0, 0.0);

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
  bool p = false;
  precise float ps = 0.0;
  do {
    { precise float v = r0.y; precise float s = r0.x; ps = s; r5.y = v; r2.x = s; }
    { precise float v = r0.x + PS_CONSTANT_TEX_SIZE.x; precise float s = PS_CONSTANT_TEX_SIZE.y + PS_CONSTANT_TEX_SIZE.y; ps = s; r0.w = v; r0.z = s; }
    { precise float v = r0.y + PS_CONSTANT_TEX_SIZE.y; precise float s = PS_CONSTANT_TEX_SIZE.x + PS_CONSTANT_TEX_SIZE.x; ps = s; r2.y = v; r2.z = s; }
    { precise float v = r0.w; precise float s = r2.y; ps = s; r5.z = v; r5.w = s; }
    { precise float v = r0.z + r0.y; precise float s = r2.z; ps = s; r0.z = v; }
    { precise float v = r0.z; precise float s = r0.x + ps; ps = s; r3.y = v; r2.z = s; }
    { precise float v = r2.z; precise float s = r2.z; ps = s; r5.x = v; r3.x = s; }
    r4.yzw = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.wz, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r7.yzw = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xz, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r10.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.wy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r8.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r9.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.zy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r3.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r2.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r5.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r5.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r5.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r6.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r5.xyz = xe_mul(r5.zyx, r1.zyx);
    { precise vec3 v = xe_mul(r2.zyx, r1.zyx); precise float s = r6.x; ps = s; r2.yzw = v; }
    { precise vec3 v = xe_mul(r3.zyx, r1.zyx); precise float s = xe_mul(r1.x, ps); ps = s; r3.yzw = v; r3.x = s; }
    { precise vec3 v = xe_mul(r8.zyx, r1.zyx); precise float s = r9.z; ps = s; r8.xyz = v; }
    { precise vec3 v = xe_mul(r10.zyx, r1.zyx); precise float s = xe_mul(r1.z, ps); ps = s; r0.yzw = v; r7.x = s; }
    { precise vec3 v = xe_mul(r7.wzy, r1.zyx); precise float s = r9.y; ps = s; r7.yzw = v; }
    { precise float v = xe_dp3(r7.ywz, lit254.xyz); precise float s = xe_mul(r1.y, ps); ps = s; r0.x = v; r7.y = s; }
    { precise float v = xe_dp3(r0.ywz, lit254.xyz); precise float s = r9.x; ps = s; r0.y = v; }
    { precise float v = xe_dp3(r8.xzy, lit254.xyz); precise float s = xe_mul(r1.x, ps); ps = s; r0.z = v; r7.z = s; }
    { precise float v = xe_dp3(r7.xzy, lit254.xyz); precise float s = r6.y; ps = s; r0.w = v; }
    { precise float v = xe_dp3(r3.ywz, lit254.xyz); precise float s = xe_mul(r1.y, ps); ps = s; r2.x = v; r3.y = s; }
    { precise float v = xe_dp3(r2.ywz, lit254.xyz); precise float s = r6.z; ps = s; r2.y = v; }
    { precise float v = xe_dp3(r5.xzy, lit254.xyz); precise float s = xe_mul(r1.z, ps); ps = s; r2.z = v; r3.z = s; }
    r2.w = xe_dp3(r3.zxy, lit254.xyz);
    r5 = r2 + lit254.wwww;
    { precise vec4 v = r0 + lit254.wwww; precise float s = log2(abs(r5.w)); ps = s; r2 = v; r3.x = s; }
    ps = log2(abs(r2.y));
    r3.y = ps;
    { precise float v = lit253.y; precise float s = log2(abs(r5.x)); ps = s; r0.w = v; r4.x = s; }
    { precise vec3 v = xe_mul(r4.wzy, r1.zyx); precise float s = log2(abs(r2.x)); ps = s; r0.xyz = v; r4.y = s; }
    { precise float v = xe_dp4(lit254, r0.xzyw); precise float s = log2(abs(r5.z)); ps = s; r0.y = v; r4.w = s; }
    { precise float v = xe_mul(r4.w, lit253.x); precise float s = log2(abs(r5.y)); ps = s; r0.w = v; r4.z = s; }
    { precise float v = xe_dp3(r4.xyz, lit253.xxx); precise float s = log2(abs(r2.z)); ps = s; r0.x = v; r3.z = s; }
    { precise float v = xe_dp3(r3.zxy, lit253.xxx); precise float s = log2(abs(r0.y)); ps = s; r1.x = v; r0.y = s; }
    { precise float v = r0.x + r1.x; precise float s = log2(abs(r2.w)); ps = s; r0.z = v; r0.x = s; }
    r1.x = xe_dp4(r0.yxzw, lit253.xxyy);
    ps = xe_mul(lit253.w, r1.x);
    r1.y = ps;
    r1.x = r1.y + -PS_CONSTANT_PARAM_0.x;
    r1.x = xe_max(r1.x, lit255.x);
    ps = xe_mul(lit253.z, r1.x);
    r1.x = ps;
    { precise vec3 e = xe_mul(r1.yyy, r1.xxx); xe_color0.xyz = e; }
    xe_color0.w = 1.0;
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
