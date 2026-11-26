#version 450
// Pixel shader 375EE1E85E22C6A3, first found in 00022803.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 53
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 7, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 7, signed
layout(set = 3, binding = 4) uniform texture2DArray xe_texture4;  // fetch constant 6, unsigned
layout(set = 3, binding = 5) uniform texture2DArray xe_texture5;  // fetch constant 6, signed
layout(set = 3, binding = 6) uniform sampler xe_sampler0;  // fetch constant 0
layout(set = 3, binding = 7) uniform sampler xe_sampler1;  // fetch constant 7
layout(set = 3, binding = 8) uniform sampler xe_sampler2;  // fetch constant 6

// UseProjTex: bool 0
// dlshd00: sampler 7
// spheretx0: sampler 6
// sprefalphatx0: sampler 0
// tx0: sampler 0
#define matView_0 xe_fc[0]  // c0
#define matView_1 xe_fc[1]  // c1
#define matView_2 xe_fc[2]  // c2
#define EyePos xe_fc[3]  // c4
#define DLightDir_0 xe_fc[4]  // c5
#define DLightDir_1 xe_fc[5]  // c6
#define DLightDir_2 xe_fc[6]  // c7
#define PntLightPos_0 xe_fc[7]  // c8
#define PntLightPos_1 xe_fc[8]  // c9
#define PntLightPos_2 xe_fc[9]  // c10
#define PntLightPos_3 xe_fc[10]  // c11
#define PntLightAtten_0 xe_fc[11]  // c16
#define PntLightAtten_1 xe_fc[12]  // c17
#define PntLightAtten_2 xe_fc[13]  // c18
#define PntLightAtten_3 xe_fc[14]  // c19
#define SpotLightDir_0 xe_fc[15]  // c24
#define SpotLightDir_1 xe_fc[16]  // c25
#define SpotLightDir_2 xe_fc[17]  // c26
#define SpotLightDir_3 xe_fc[18]  // c27
#define SpotLightCone_0 xe_fc[19]  // c32
#define SpotLightCone_1 xe_fc[20]  // c33
#define SpotLightCone_2 xe_fc[21]  // c34
#define SpotLightCone_3 xe_fc[22]  // c35
#define AmbientColor xe_fc[23]  // c40
#define DLightColor_0 xe_fc[24]  // c41
#define DLightColor_1 xe_fc[25]  // c42
#define DLightColor_2 xe_fc[26]  // c43
#define DLightSpecularColor_0 xe_fc[27]  // c44
#define DLightSpecularColor_1 xe_fc[28]  // c45
#define DLightSpecularColor_2 xe_fc[29]  // c46
#define PntLightColor_0 xe_fc[30]  // c47
#define PntLightColor_1 xe_fc[31]  // c48
#define PntLightColor_2 xe_fc[32]  // c49
#define PntLightColor_3 xe_fc[33]  // c50
#define PntLightSpecularColor_0 xe_fc[34]  // c55
#define PntLightSpecularColor_1 xe_fc[35]  // c56
#define PntLightSpecularColor_2 xe_fc[36]  // c57
#define PntLightSpecularColor_3 xe_fc[37]  // c58
#define BaseColor xe_fc[38]  // c63
#define MatParam xe_fc[39]  // c64
#define MatAmbientDiffuse xe_fc[40]  // c65
#define MatSpecularColor xe_fc[41]  // c66
#define shadowMapSize xe_fc[42]  // c77
const vec4 lit246 = vec4(0.2, 0.5, 0.8, 0.0);
const vec4 lit247 = vec4(0.0, 1.0, 0.0625, 1e-05);
const vec4 lit248 = vec4(0.4608, -2.403328, -0.708608, -0.888832);
const vec4 lit249 = vec4(1.92512, -1.630208, 0.791552, -2.091008);
const vec4 lit250 = vec4(-0.93696, -0.4352, -1.236992, -1.226752);
const vec4 lit251 = vec4(1.760256, -0.771072, -0.0512, 0.10752);
const vec4 lit252 = vec4(-1.241088, 1.16736, -1.89952, -0.004096);
const vec4 lit253 = vec4(0.662528, 0.181248, 0.700416, 0.279552);
const vec4 lit254 = vec4(0.431104, 0.830464, -1.482752, 2.14528);
const vec4 lit255 = vec4(2.423808, 0.549888, 0.555008, 1.526784);

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
    r0.w = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).w;
    r5.xyz = -r0.xyz + EyePos.xyz;
    r2.w = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = xe_dp3(r5.zxy, r5.zxy); precise float s = inversesqrt(abs(r2.w)); ps = s; r2.z = v; r2.w = s; }
    { precise vec3 v = xe_mul(r2.www, r1.xzy); precise float s = inversesqrt(abs(r2.z)); ps = s; r8.xyz = v; r1.x = s; }
    r1.yzw = xe_mul(r5.zxy, r1.xxx);
    r2.z = xe_dp3(-r1.yzw, r8.yxz);
    { precise float v = xe_dp3(r1.yzw, r8.yxz); precise float s = lit247.y; ps = s; r1.x = v; r5.y = s; }
    { precise float v = xe_max(r1.x, lit247.x); precise float s = lit247.y; ps = s; r1.x = v; r5.x = s; }
    { precise float v = r2.z + r2.z; precise float s = lit247.y; ps = s; r2.z = v; r5.z = s; }
    r6.xzw = xe_mul(-r8.yxz, r2.zzz) + -r1.yzw;
    { precise float v = -r1.x + lit247.y; precise float s = xe_mul(MatParam.y, r0.w); ps = s; r1.x = v; r5.w = s; }
    { precise float v = xe_mul(r1.x, r1.x); precise float s = xe_mul(MatSpecularColor.x, r0.w); ps = s; r1.z = v; r1.y = s; }
    { precise float v = xe_mul(r1.z, r1.z); precise float s = xe_mul(MatSpecularColor.y, r0.w); ps = s; r1.w = v; r1.z = s; }
    { precise float v = xe_mul(r1.w, r1.x); precise float s = xe_mul(MatSpecularColor.z, r0.w); ps = s; r1.w = v; r1.x = s; }
    r9.z = xe_mul(r1.w, lit246.z) + lit246.x;
    if (xe_bool(128u /* UseProjTex */)) {
      ps = 1.0 / r3.w;
      r0.w = ps;
      r3.xyz = xe_mul(r0.www, r3.xyz);
      r5.xz = xe_mul(r3.xy, lit246.yy) + lit246.yy;
      { precise float v = -r5.z + lit247.y; precise float s = 1.0 / shadowMapSize.x; ps = s; r5.y = v; r0.w = s; }
      r14 = xe_mul(r0.wwww, lit252) + r5.xyxy;
      r15 = xe_mul(r0.wwww, lit253) + r5.xyxy;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r11 = xe_mul(r0.wwww, lit254) + r5.xyxy;
      r17 = xe_mul(r0.wwww, lit255) + r5.xyxy;
      r16 = xe_mul(r0.wwww, lit248) + r5.xyxy;
      r12 = xe_mul(r0.wwww, lit249) + r5.xyxy;
      r10 = xe_mul(r0.wwww, lit250) + r5.xyxy;
      r13 = xe_mul(r0.wwww, lit251) + r5.xyxy;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r7.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r13.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r7.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r10.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r7.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r10.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r10.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r12.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r10.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r12.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r10.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r16.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r12.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r17.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r12.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r17.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r12.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r11.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r12.w = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r16.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r11.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r11.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r11.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r15.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r11.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r15.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r9.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r14.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r9.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r14.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r9.w = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 7u, r13.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r5.xy = vec2(greaterThan(lit247.yy, abs(r3.xy)));
      r9.xyw = -r3.zzz + r9.xyw;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r11.xyz = -r3.zzz + r11.xyz;
      r0.w = (lit247.y > r3.z ? 1.0 : 0.0);
      r13 = -r3.zzzz + r12;
      r14.xyz = -r3.zzz + r10.xyz;
      r7.xyz = -r3.zzz + r7.xyz;
      { precise vec3 v = vec3(greaterThan(-r7.zxy, lit247.xxx)); precise float s = (r3.z > 0.0 ? 1.0 : 0.0); ps = s; r7.xyz = v; r1.w = s; }
    }
    if (xe_bool(128u /* UseProjTex */)) {
      { precise vec3 v = vec3(greaterThan(-r13.zxy, lit247.xxx)); precise float s = (-r14.z > 0.0 ? 1.0 : 0.0); ps = s; r12.xyz = v; r10.x = s; }
      { precise float v = xe_mul(r1.w, r0.w); precise float s = (-r14.x > 0.0 ? 1.0 : 0.0); ps = s; r1.w = v; r10.y = s; }
      { precise vec3 v = vec3(greaterThan(-r11.zxy, lit247.xxx)); precise float s = (-r14.y > 0.0 ? 1.0 : 0.0); ps = s; r11.xyz = v; r10.z = s; }
      { precise vec3 v = vec3(greaterThan(-r9.wxy, lit247.xxx)); precise float s = (-r13.w > 0.0 ? 1.0 : 0.0); ps = s; r3.xyz = v; r0.w = s; }
      r11.w = xe_dp2add(r3.zy, lit247.zz, lit247.x);
      { precise float v = xe_mul(r11.y, lit247.z); precise float s = xe_mul(lit247.z, r3.x); ps = s; r12.w = v; r7.w = s; }
    }
    if (xe_bool(128u /* UseProjTex */)) {
      { precise float v = xe_dp4(r12.yzxw, lit247.zzzy); precise float s = xe_mul(lit247.z, r0.w); ps = s; r2.z = v; r10.w = s; }
      { precise float v = xe_dp3(r11.zxw, lit247.zzy); precise float s = (!(r1.w == 0.0) ? 1.0 : 0.0); ps = s; r2.w = v; r1.w = s; }
      { precise float v = xe_dp4(r10.yzxw, lit247.zzzy); precise float s = r5.x; ps = s; r3.w = v; }
      { precise float v = xe_dp4(r7.yxzw, lit247.zzzy); precise float s = xe_mul(r1.w, ps); ps = s; r0.w = v; r1.w = s; }
      { precise float v = r3.w + r0.w; precise float s = (!(r1.w == 0.0) ? 1.0 : 0.0); ps = s; r0.w = v; r1.w = s; }
      { precise float v = r0.w + r2.w; precise float s = r5.y; ps = s; r0.w = v; }
    }
    if (xe_bool(128u /* UseProjTex */)) {
      { precise float v = r0.w + r2.z; precise float s = xe_mul(r1.w, ps); ps = s; r0.w = v; r1.w = s; }
      r5.y = (r1.w == 0.0 ? lit247.y : r0.w);
      r5.xz = r5.yy;
    }
    r15.xyz = xe_mul(DLightColor_2.zyx, MatAmbientDiffuse.www);
    r16.xyz = xe_mul(DLightColor_1.zyx, MatAmbientDiffuse.www);
    r7.xyz = xe_mul(DLightColor_0.zyx, MatAmbientDiffuse.www);
    r17.x = xe_dp3(r8.yxz, DLightDir_0.zxy);
    r17.y = xe_dp3(r8.xzy, DLightDir_1.xyz);
    r17.z = xe_dp3(r8.xzy, DLightDir_2.xyz);
    r17.w = xe_dp3(r6.zwx, DLightDir_1.xyz);
    r2.z = xe_dp3(r6.zwx, DLightDir_2.xyz);
    r2.w = xe_dp3(r6.xzw, DLightDir_0.zxy);
    r12.xyz = xe_mul(r1.yzx, DLightSpecularColor_0.xyz);
    r3.y = r0.x;
    r3.x = r0.z;
    r3.z = r0.y;
    r11.xyz = xe_mul(r1.yzx, DLightSpecularColor_1.xyz);
    { precise vec2 v = xe_mul(r1.yz, DLightSpecularColor_2.xy); precise float s = PntLightPos_0.x - r3.y; ps = s; r9.yw = v; r9.x = s; }
    { precise vec2 v = xe_mul(r9.wy, r9.zz); precise float s = PntLightPos_0.z - r3.x; ps = s; r10.yz = v; r9.y = s; }
    { precise vec3 v = xe_mul(r11.zyx, r9.zzz); precise float s = PntLightPos_0.y - r3.z; ps = s; r11.xyz = v; r9.w = s; }
    { precise float v = xe_dp3(r9.yxw, r9.yxw); precise float s = xe_mul(DLightSpecularColor_2.z, r1.x); ps = s; r13.z = v; r12.w = s; }
    r12 = xe_mul(r12.zxyw, r9.zzzz);
    r10.xw = xe_max(r2.zw, lit247.ww);
    r14 = xe_max(r17.xzyw, lit247.xxxw);
    { precise vec3 v = xe_min(r17.xyz, lit247.xxx); precise float s = inversesqrt(abs(r13.z)); ps = s; r3.xzw = v; r0.w = s; }
    { precise float v = xe_mul(r9.w, r0.w); precise float s = lit247.y; ps = s; r1.w = v; r13.w = s; }
    { precise vec2 v = xe_mul(r9.yx, r0.ww); precise float s = sqrt(abs(r13.z)); ps = s; r9.xw = v; r13.y = s; }
    { precise vec3 v = xe_mul(r7.zxy, r14.xxx); precise float s = log2(abs(r10.x)); ps = s; r7.xyz = v; r13.x = s; }
    { precise float v = xe_dp3(PntLightAtten_0.xyz, r13.zyw); precise float s = log2(abs(r14.w)); ps = s; r2.w = v; r13.y = s; }
    { precise vec3 v = xe_mul(r7.zxy, r5.xyz); precise float s = log2(abs(r10.w)); ps = s; r7.xyz = v; r13.z = s; }
    r17.xyz = mix(r1.www, r9.xww, equal(lit247.xxy, vec3(0.0)));
    r13.xyz = xe_mul(r13.xyz, MatParam.xxx);
    r0.w = xe_dp3(r17.xyz, SpotLightDir_0.zxy);
    r7.xyz = xe_mul(r16.xyz, r14.zzz) + r7.zxy;
    r7.xyz = xe_mul(r15.zxy, r14.yyy) + r7.zxy;
    { precise float v = (r0.w > SpotLightCone_0.x ? 1.0 : 0.0); precise float s = lit247.y - r3.w; ps = s; r6.y = v; r7.w = s; }
    { precise float v = r0.w + -SpotLightCone_0.y; precise float s = exp2(r13.y); ps = s; r0.w = v; r2.z = s; }
    { precise vec3 v = xe_mul(r11.zyx, r2.zzz); precise float s = exp2(r13.x); ps = s; r11.xyz = v; r10.x = s; }
    { precise vec2 v = xe_mul(r10.zy, r10.xx); precise float s = lit247.y - r3.z; ps = s; r10.yz = v; r2.z = s; }
    { precise float v = xe_mul(r0.w, SpotLightCone_0.z); precise float s = lit247.y - r3.x; ps = s; r0.w = v; r3.y = s; }
    r6.y = (r6.y == 0.0 ? r0.w : lit247.y);
    { precise vec2 v = xe_mul(r10.zy, r7.ww); precise float s = exp2(r13.z); ps = s; r10.yz = v; r10.w = s; }
    r12 = xe_mul(r12.wzxy, r10.xwww);
    r10.x = xe_mul(r12.x, r7.w);
    r3.xyz = xe_mul(r12.wyz, r3.yyy);
    { precise vec3 v = xe_mul(r3.yxz, r5.xyz); precise float s = 1.0 / r2.w; ps = s; r3.xyz = v; r9.y = s; }
    r11.xyz = xe_mul(r11.yxz, r2.zzz) + r3.xyz;
    { precise vec3 v = r11.yzx + r10.zxy; p = r6.y > 0.0; precise float s = (p ? 0.0 : 1.0); ps = s; r3.xyz = v; }
    if (p) {
      r11.xyz = xe_mul(PntLightColor_0.xyz, MatAmbientDiffuse.www);
      r5.y = xe_mul(r8.y, r9.x);
      r10.x = xe_mul(r9.w, r6.z);
      r10.y = xe_mul(r8.x, r9.w);
      { precise float v = xe_mul(r8.z, r1.w); precise float s = r1.w; ps = s; r2.w = v; }
      { precise vec2 v = xe_mul(r9.xy, r6.xy); precise float s = xe_mul(r6.w, ps); ps = s; r5.xz = v; r2.z = s; }
    }
    if (p) {
      r2.zw = r10.xy + r2.zw;
      r2.zw = r2.zw + r5.xy;
      { precise vec2 v = xe_max(r2.zw, lit247.wx); precise float s = xe_mul(PntLightSpecularColor_0.x, r1.y); ps = s; r12.xy = v; r10.x = s; }
      { precise float v = xe_min(r2.w, lit247.x); precise float s = xe_mul(PntLightSpecularColor_0.y, r1.z); ps = s; r0.w = v; r10.y = s; }
      { precise float v = -r0.w + lit247.y; precise float s = xe_mul(PntLightSpecularColor_0.z, r1.x); ps = s; r2.z = v; r10.z = s; }
      { precise vec3 v = xe_mul(r11.zxy, r12.yyy); precise float s = log2(abs(r12.x)); ps = s; r11.xyz = v; r0.w = s; }
    }
    if (p) {
      r7.yz = xe_mul(r11.xz, r5.zz) + r7.yz;
      { precise float v = xe_mul(r11.y, r6.y); precise float s = xe_mul(MatParam.x, r0.w); ps = s; r10.w = v; r2.w = s; }
      { precise vec4 v = xe_mul(r10.zxyw, r9.zzzy); precise float s = exp2(r2.w); ps = s; r11 = v; r0.w = s; }
      { precise vec3 v = xe_mul(r11.zxy, r0.www); precise float s = r7.x; ps = s; r9.xyw = v; }
      { precise vec3 v = xe_mul(r9.wxy, r2.zzz); precise float s = r11.w + ps; ps = s; r10.xyz = v; r7.x = s; }
      r3.xyz = xe_mul(r10.xzy, r5.zzz) + r3.xyz;
    }
    r10.xyz = -r0.xyz + PntLightPos_1.xyz;
    r5.x = xe_dp3(r10.zxy, r10.zxy);
    { precise float v = lit247.y; precise float s = inversesqrt(abs(r5.x)); ps = s; r5.z = v; r0.w = s; }
    r9.x = xe_mul(r10.z, r0.w);
    { precise vec2 v = xe_mul(r10.yx, r0.ww); precise float s = sqrt(abs(r5.x)); ps = s; r2.zw = v; r5.y = s; }
    r3.w = xe_dp3(PntLightAtten_1.xyz, r5.xyz);
    r5.xyz = mix(r9.xxx, r2.wzz, equal(lit247.xxy, vec3(0.0)));
    r0.w = xe_dp3(r5.xyz, SpotLightDir_1.xyz);
    r1.w = (r0.w > SpotLightCone_1.x ? 1.0 : 0.0);
    r0.w = r0.w + -SpotLightCone_1.y;
    { precise float v = xe_mul(r0.w, SpotLightCone_1.z); precise float s = 1.0 / r3.w; ps = s; r0.w = v; r9.y = s; }
    r6.y = (r1.w == 0.0 ? r0.w : lit247.y);
    p = r6.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r11.xyz = xe_mul(PntLightColor_1.zyx, MatAmbientDiffuse.www);
      r5.y = xe_mul(r8.y, r9.x);
      r10.z = xe_mul(r2.w, r6.z);
      r10.w = xe_mul(r8.x, r2.w);
      { precise float v = xe_mul(r8.z, r2.z); precise float s = r2.z; ps = s; r10.y = v; }
      { precise vec2 v = xe_mul(r9.xy, r6.xy); precise float s = xe_mul(r6.w, ps); ps = s; r5.xz = v; r10.x = s; }
    }
    if (p) {
      r10.xy = r10.zw + r10.xy;
      r10.yz = r10.xy + r5.xy;
      { precise vec2 v = xe_max(r10.yz, lit247.wx); precise float s = xe_mul(PntLightSpecularColor_1.x, r1.y); ps = s; r2.zw = v; r10.x = s; }
      { precise float v = xe_min(r10.z, lit247.x); precise float s = xe_mul(PntLightSpecularColor_1.y, r1.z); ps = s; r0.w = v; r10.y = s; }
      { precise float v = -r0.w + lit247.y; precise float s = xe_mul(PntLightSpecularColor_1.z, r1.x); ps = s; r1.w = v; r10.z = s; }
      { precise vec3 v = xe_mul(r11.zxy, r2.www); precise float s = log2(abs(r2.z)); ps = s; r11.xyz = v; r0.w = s; }
    }
    if (p) {
      r7.yz = xe_mul(r11.yz, r5.zz) + r7.yz;
      { precise float v = xe_mul(r11.x, r6.y); precise float s = xe_mul(MatParam.x, r0.w); ps = s; r10.w = v; r3.w = s; }
      { precise vec4 v = xe_mul(r10.zxyw, r9.zzzy); precise float s = exp2(r3.w); ps = s; r10 = v; r0.w = s; }
      { precise vec3 v = xe_mul(r10.zxy, r0.www); precise float s = r10.w; ps = s; r9.xyw = v; }
      { precise vec3 v = xe_mul(r9.wxy, r1.www); precise float s = r7.x + ps; ps = s; r10.xyz = v; r7.x = s; }
      r3.xyz = xe_mul(r10.xzy, r5.zzz) + r3.xyz;
    }
    r10.xyz = -r0.xyz + PntLightPos_2.xyz;
    r5.x = xe_dp3(r10.zxy, r10.zxy);
    { precise float v = lit247.y; precise float s = inversesqrt(abs(r5.x)); ps = s; r5.z = v; r0.w = s; }
    r9.x = xe_mul(r10.z, r0.w);
    { precise vec2 v = xe_mul(r10.yx, r0.ww); precise float s = sqrt(abs(r5.x)); ps = s; r2.zw = v; r5.y = s; }
    r3.w = xe_dp3(PntLightAtten_2.xyz, r5.xyz);
    r5.xyz = mix(r9.xxx, r2.wzz, equal(lit247.xxy, vec3(0.0)));
    r0.w = xe_dp3(r5.xyz, SpotLightDir_2.xyz);
    r1.w = (r0.w > SpotLightCone_2.x ? 1.0 : 0.0);
    r0.w = r0.w + -SpotLightCone_2.y;
    { precise float v = xe_mul(r0.w, SpotLightCone_2.z); precise float s = 1.0 / r3.w; ps = s; r0.w = v; r9.y = s; }
    r6.y = (r1.w == 0.0 ? r0.w : lit247.y);
    p = r6.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r11.xyz = xe_mul(PntLightColor_2.zyx, MatAmbientDiffuse.www);
      r5.y = xe_mul(r8.y, r9.x);
      r10.z = xe_mul(r2.w, r6.z);
      r10.w = xe_mul(r8.x, r2.w);
      { precise float v = xe_mul(r8.z, r2.z); precise float s = r2.z; ps = s; r10.y = v; }
      { precise vec2 v = xe_mul(r9.xy, r6.xy); precise float s = xe_mul(r6.w, ps); ps = s; r5.xz = v; r10.x = s; }
    }
    if (p) {
      r10.xy = r10.zw + r10.xy;
      r10.yz = r10.xy + r5.xy;
      { precise vec2 v = xe_max(r10.yz, lit247.wx); precise float s = xe_mul(PntLightSpecularColor_2.x, r1.y); ps = s; r2.zw = v; r10.x = s; }
      { precise float v = xe_min(r10.z, lit247.x); precise float s = xe_mul(PntLightSpecularColor_2.y, r1.z); ps = s; r0.w = v; r10.y = s; }
      { precise float v = -r0.w + lit247.y; precise float s = xe_mul(PntLightSpecularColor_2.z, r1.x); ps = s; r1.w = v; r10.z = s; }
      { precise vec3 v = xe_mul(r11.zxy, r2.www); precise float s = log2(abs(r2.z)); ps = s; r11.xyz = v; r0.w = s; }
    }
    if (p) {
      r7.yz = xe_mul(r11.yz, r5.zz) + r7.yz;
      { precise float v = xe_mul(r11.x, r6.y); precise float s = xe_mul(MatParam.x, r0.w); ps = s; r10.w = v; r3.w = s; }
      { precise vec4 v = xe_mul(r10.zxyw, r9.zzzy); precise float s = exp2(r3.w); ps = s; r10 = v; r0.w = s; }
      { precise vec3 v = xe_mul(r10.zxy, r0.www); precise float s = r10.w; ps = s; r9.xyw = v; }
      { precise vec3 v = xe_mul(r9.wxy, r1.www); precise float s = r7.x + ps; ps = s; r10.xyz = v; r7.x = s; }
      r3.xyz = xe_mul(r10.xzy, r5.zzz) + r3.xyz;
    }
    r0.yzw = -r0.xyz + PntLightPos_3.xyz;
    r5.x = xe_dp3(r0.wyz, r0.wyz);
    { precise float v = lit247.y; precise float s = inversesqrt(abs(r5.x)); ps = s; r5.z = v; r0.x = s; }
    r9.x = xe_mul(r0.w, r0.x);
    { precise vec2 v = xe_mul(r0.zy, r0.xx); precise float s = sqrt(abs(r5.x)); ps = s; r0.yz = v; r5.y = s; }
    r1.w = xe_dp3(PntLightAtten_3.xyz, r5.xyz);
    r5.xyz = mix(r9.xxx, r0.zyy, equal(lit247.xxy, vec3(0.0)));
    r0.x = xe_dp3(r5.xyz, SpotLightDir_3.xyz);
    r0.w = (r0.x > SpotLightCone_3.x ? 1.0 : 0.0);
    r0.x = r0.x + -SpotLightCone_3.y;
    { precise float v = xe_mul(r0.x, SpotLightCone_3.z); precise float s = 1.0 / r1.w; ps = s; r0.x = v; r9.y = s; }
    r6.y = (r0.w == 0.0 ? r0.x : lit247.y);
    p = r6.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r11.xyz = xe_mul(PntLightColor_3.zyx, MatAmbientDiffuse.www);
      r5.y = xe_mul(r8.y, r9.x);
      r2.z = xe_mul(r0.z, r6.z);
      r2.w = xe_mul(r8.x, r0.z);
      { precise float v = xe_mul(r8.z, r0.y); precise float s = r0.y; ps = s; r0.w = v; }
      { precise vec2 v = xe_mul(r9.xy, r6.xy); precise float s = xe_mul(r6.w, ps); ps = s; r5.xz = v; r0.x = s; }
    }
    if (p) {
      r0.xw = r2.zw + r0.xw;
      r0.xw = r0.xw + r5.xy;
      { precise vec2 v = xe_max(r0.xw, lit247.wx); precise float s = xe_mul(PntLightSpecularColor_3.x, r1.y); ps = s; r0.yz = v; r10.x = s; }
      { precise float v = xe_min(r0.w, lit247.x); precise float s = xe_mul(PntLightSpecularColor_3.y, r1.z); ps = s; r0.x = v; r10.y = s; }
      { precise float v = -r0.x + lit247.y; precise float s = xe_mul(PntLightSpecularColor_3.z, r1.x); ps = s; r0.x = v; r10.z = s; }
      { precise vec3 v = xe_mul(r11.zxy, r0.zzz); precise float s = log2(abs(r0.y)); ps = s; r11.xyz = v; r0.y = s; }
    }
    if (p) {
      r7.yz = xe_mul(r11.yz, r5.zz) + r7.yz;
      { precise float v = xe_mul(r11.x, r6.y); precise float s = xe_mul(MatParam.x, r0.y); ps = s; r10.w = v; r0.w = s; }
      { precise vec4 v = xe_mul(r10.zxyw, r9.zzzy); precise float s = exp2(r0.w); ps = s; r1 = v; r0.y = s; }
      { precise vec3 v = xe_mul(r1.zxy, r0.yyy); precise float s = r1.w; ps = s; r0.yzw = v; }
      { precise vec3 v = xe_mul(r0.wyz, r0.xxx); precise float s = r7.x + ps; ps = s; r10.xyz = v; r7.x = s; }
      r3.xyz = xe_mul(r10.xzy, r5.zzz) + r3.xyz;
    }
    r0.x = r4.w;
    r0.yzw = xe_mul(r6.www, matView_1.xyz);
    r0.yzw = xe_mul(r6.zzz, matView_0.zxy) + r0.wyz;
    r0.yzw = xe_mul(-r6.xxx, matView_2.yzx) + -r0.wyz;
    r4.w = xe_dp3(r0.zwy, r0.zwy);
    ps = inversesqrt(abs(r4.w));
    r4.w = ps;
    r0.yzw = xe_mul(r0.wzy, r4.www);
    ps = lit247.y + r0.z;
    r1.x = ps;
    r4.w = xe_dp2add(r0.yw, r0.yw, lit247.x);
    r4.w = xe_mul(r1.x, r1.x) + r4.w;
    ps = sqrt(abs(r4.w));
    r4.w = ps;
    ps = r4.w + r4.w;
    r4.w = ps;
    ps = 1.0 / r4.w;
    r4.w = ps;
    r1.xy = xe_mul(r0.yw, r4.ww) + lit246.yy;
    r10 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0.yzw = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 6u, r1.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r8.xyz = AmbientColor.zyx + MatAmbientDiffuse.zyx;
    r1.xyz = xe_mul(r5.www, r0.yzw);
    r5.xyz = -r10.xyz + BaseColor.xyz;
    r8.xyz = r8.zxy + r7.xyz;
    { precise vec3 v = xe_mul(r8.zxy, r4.yxz); precise float s = xe_mul(BaseColor.w, r0.x); ps = s; r8.xyz = v; r4.w = s; }
    r5.xyz = xe_mul(r5.xyz, r10.www) + r10.xyz;
    r4.xyz = xe_mul(r8.yzx, r5.xzy) + r3.xyz;
    r4.xyz = xe_mul(r1.zyx, r9.zzz) + r4.yzx;
    if (xe_bool(128u /* UseProjTex */)) {
      r4.xyz = xe_max(r4.xyz, lit247.xxx);
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r4.xyz = xe_max(r4.xyz, lit247.xxx);
    }
    { precise vec4 e = r4.zyxw; xe_color0 = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
