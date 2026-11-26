#version 450
// Pixel shader 0D4BBF8CA1BD0F47, first found in 00001324.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 22
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 1, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 1, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 0, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 0, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 1
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 0

// UseProjTex: bool 0
// tx0: sampler 0
// tx1: sampler 1
#define PntLightPos_0 xe_fc[0]  // c8
#define PntLightPos_1 xe_fc[1]  // c9
#define PntLightPos_2 xe_fc[2]  // c10
#define PntLightPos_3 xe_fc[3]  // c11
#define PntLightAtten_0 xe_fc[4]  // c16
#define PntLightAtten_1 xe_fc[5]  // c17
#define PntLightAtten_2 xe_fc[6]  // c18
#define PntLightAtten_3 xe_fc[7]  // c19
#define SpotLightDir_0 xe_fc[8]  // c24
#define SpotLightDir_1 xe_fc[9]  // c25
#define SpotLightDir_2 xe_fc[10]  // c26
#define SpotLightDir_3 xe_fc[11]  // c27
#define SpotLightCone_0 xe_fc[12]  // c32
#define SpotLightCone_1 xe_fc[13]  // c33
#define SpotLightCone_2 xe_fc[14]  // c34
#define SpotLightCone_3 xe_fc[15]  // c35
#define PntLightColor_0 xe_fc[16]  // c47
#define PntLightColor_1 xe_fc[17]  // c48
#define PntLightColor_2 xe_fc[18]  // c49
#define PntLightColor_3 xe_fc[19]  // c50
#define ModTexAlpha xe_fc[20]  // c67
const vec4 lit255 = vec4(0.5, 0.0, 1.0, 0.0);

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
  bool p = false;
  precise float ps = 0.0;
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r6.xyz = vec3(greaterThan(-abs(r4.xxx), lit255.yyy));
      r2.z = xe_dp3(r1.zxy, r1.zxy);
      { precise float v = (PntLightColor_0.w > lit255.x ? 1.0 : 0.0); precise float s = inversesqrt(abs(r2.z)); ps = s; r0.w = v; r2.z = s; }
      { precise vec3 v = xe_mul(r2.zzz, r1.xyz); p = !(r0.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r5.yzw = v; }
      if (!p) {
        pc = 7; continue;
      }
      r1.xyz = -r0.xzy + PntLightPos_0.xzy;
      r6.y = xe_dp3(r1.yxz, r1.yxz);
      { precise float v = lit255.z; precise float s = inversesqrt(abs(r6.y)); ps = s; r6.z = v; r4.w = s; }
      { precise vec3 v = xe_mul(r1.yzx, r4.www); precise float s = sqrt(abs(r6.y)); ps = s; r1.yzw = v; r6.x = s; }
      r1.x = xe_dp3(PntLightAtten_0.xyz, r6.yxz);
      r4.w = xe_dp3(r1.ywz, SpotLightDir_0.zxy);
      r0.w = (r4.w > SpotLightCone_0.x ? 1.0 : 0.0);
      r4.w = r4.w + -SpotLightCone_0.y;
      { precise float v = xe_mul(r4.w, SpotLightCone_0.z); precise float s = 1.0 / r1.x; ps = s; r4.w = v; r5.x = s; }
      r1.x = (r0.w == 0.0 ? r4.w : lit255.z);
      { precise float v = (-abs(r4.x) > lit255.y ? 1.0 : 0.0); p = r1.x > 0.0; precise float s = (p ? 0.0 : 1.0); ps = s; r6.z = v; }
      if (!p) {
        r6.xy = r6.zz;
      }
      if (p) {
        r2.zw = xe_mul(r5.yx, r1.wx);
        r0.w = xe_dp2add(r5.wz, r1.yz, r2.z);
        r0.w = xe_max(r0.w, lit255.y);
        r6.xyz = xe_mul(r0.www, PntLightColor_0.xzy);
        r0.w = xe_mul(r6.x, r1.x);
        r6.xy = xe_mul(r6.zy, r2.ww);
      }
      if (p) {
        r6.z = xe_mul(r0.w, r5.x);
      }
    case 7:
      r0.w = (PntLightColor_1.w > lit255.x ? 1.0 : 0.0);
      p = !(r0.w == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (!p) {
        pc = 13; continue;
      }
      r1.xyz = -r0.xyz + PntLightPos_1.xyz;
      r7.x = xe_dp3(r1.zxy, r1.zxy);
      { precise float v = lit255.z; precise float s = inversesqrt(abs(r7.x)); ps = s; r7.z = v; r4.w = s; }
      { precise vec3 v = xe_mul(r1.xyz, r4.www); precise float s = sqrt(abs(r7.x)); ps = s; r1.yzw = v; r7.y = s; }
      r1.x = xe_dp3(PntLightAtten_1.xyz, r7.xyz);
      r4.w = xe_dp3(r1.wyz, SpotLightDir_1.zxy);
      r0.w = (r4.w > SpotLightCone_1.x ? 1.0 : 0.0);
      r4.w = r4.w + -SpotLightCone_1.y;
      { precise float v = xe_mul(r4.w, SpotLightCone_1.z); precise float s = 1.0 / r1.x; ps = s; r4.w = v; r5.x = s; }
      r1.x = (r0.w == 0.0 ? r4.w : lit255.z);
      p = r1.x > 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r0.w = xe_dp3(r5.yzw, r1.yzw);
        { precise float v = xe_max(r0.w, lit255.y); precise float s = r5.x; ps = s; r0.w = v; }
        { precise vec3 v = xe_mul(r0.www, PntLightColor_1.xyz); precise float s = xe_mul(r1.x, ps); ps = s; r8.xyz = v; r0.w = s; }
        r7.xy = xe_mul(r8.zy, r0.ww);
        r0.w = xe_mul(r8.x, r1.x);
        r7.z = xe_mul(r0.w, r5.x);
      }
      if (p) {
        r6.xyz = r7.yxz + r6.xyz;
      }
    case 13:
      r0.w = (PntLightColor_2.w > lit255.x ? 1.0 : 0.0);
      p = !(r0.w == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (!p) {
        pc = 19; continue;
      }
      r1.xyz = -r0.xyz + PntLightPos_2.xyz;
      r7.x = xe_dp3(r1.zxy, r1.zxy);
      { precise float v = lit255.z; precise float s = inversesqrt(abs(r7.x)); ps = s; r7.z = v; r4.w = s; }
      { precise vec3 v = xe_mul(r1.zyx, r4.www); precise float s = sqrt(abs(r7.x)); ps = s; r1.yzw = v; r7.y = s; }
      r1.x = xe_dp3(PntLightAtten_2.xyz, r7.xyz);
      r4.w = xe_dp3(r1.ywz, SpotLightDir_2.zxy);
      r0.w = (r4.w > SpotLightCone_2.x ? 1.0 : 0.0);
      r4.w = r4.w + -SpotLightCone_2.y;
      { precise float v = xe_mul(r4.w, SpotLightCone_2.z); precise float s = 1.0 / r1.x; ps = s; r4.w = v; r5.x = s; }
      r1.x = (r0.w == 0.0 ? r4.w : lit255.z);
      p = r1.x > 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r2.zw = xe_mul(r5.yx, r1.wx);
        r0.w = xe_dp2add(r5.wz, r1.yz, r2.z);
        r0.w = xe_max(r0.w, lit255.y);
        r8.xyz = xe_mul(r0.www, PntLightColor_2.xyz);
        r7.xy = xe_mul(r8.zy, r2.ww);
        r0.w = xe_mul(r8.x, r1.x);
      }
      if (p) {
        r7.z = xe_mul(r0.w, r5.x);
        r6.xyz = r7.yxz + r6.xyz;
      }
    case 19:
      r0.w = (PntLightColor_3.w > lit255.x ? 1.0 : 0.0);
      p = !(r0.w == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (!p) {
        pc = 25; continue;
      }
      r0.xyz = -r0.xyz + PntLightPos_3.xyz;
      r1.x = xe_dp3(r0.zxy, r0.zxy);
      { precise float v = lit255.z; precise float s = inversesqrt(abs(r1.x)); ps = s; r1.z = v; r4.w = s; }
      { precise vec3 v = xe_mul(r0.xyz, r4.www); precise float s = sqrt(abs(r1.x)); ps = s; r0.yzw = v; r1.y = s; }
      r1.x = xe_dp3(PntLightAtten_3.xyz, r1.xyz);
      r4.w = xe_dp3(r0.wyz, SpotLightDir_3.zxy);
      r0.x = (r4.w > SpotLightCone_3.x ? 1.0 : 0.0);
      r4.w = r4.w + -SpotLightCone_3.y;
      { precise float v = xe_mul(r4.w, SpotLightCone_3.z); precise float s = 1.0 / r1.x; ps = s; r4.w = v; r5.x = s; }
      r0.x = (r0.x == 0.0 ? r4.w : lit255.z);
      p = r0.x > 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r1.x = xe_dp3(r5.yzw, r0.yzw);
        { precise float v = xe_max(r1.x, lit255.y); precise float s = r5.x; ps = s; r1.x = v; }
        { precise vec3 v = xe_mul(r1.xxx, PntLightColor_3.xyz); precise float s = xe_mul(r0.x, ps); ps = s; r7.xyz = v; r1.x = s; }
        r1.xy = xe_mul(r7.zy, r1.xx);
        r0.x = xe_mul(r7.x, r0.x);
        r1.z = xe_mul(r0.x, r5.x);
      }
      if (p) {
        r6.xyz = r1.yxz + r6.xyz;
      }
    case 25:
      r5 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r0 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r1.xyz = xe_mul(r0.xyz, ModTexAlpha.xxx);
      r4.xyz = r6.xyz + r4.yzx;
      { precise vec3 v = xe_mul(r5.xyz, ModTexAlpha.yyy); precise float s = r5.w; ps = s; r2.xyz = v; }
      r2.xyz = xe_mul(r2.xyz, r4.zxy) + -r1.xyz;
      r1.xyz = xe_mul(r2.zxy, r5.www) + r1.zxy;
      { precise vec3 v = xe_mul(r1.xzy, r4.yxz); precise float s = r0.w + ps; ps = s; r4.xyz = v; r4.w = clamp(s, 0.0, 1.0); }
      if (xe_bool(128u /* UseProjTex */)) {
        r4.xyz = xe_max(r4.xyz, lit255.yyy);
      }
      if (xe_bool(128u /* UseProjTex */)) {
        r4.xyz = xe_max(r4.xyz, lit255.yyy);
      }
      { precise vec4 e = r4.zyxw; xe_color0 = e; }
      break;  // the end
    }
    break;
  }
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
