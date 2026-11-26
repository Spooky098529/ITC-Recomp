#version 450
// Pixel shader 6E9AB15C8DAAAB83, first found in 00000016.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 49
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 10, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 10, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 0, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 0, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 10
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 0

// UseProjTex: bool 0
// dlshd00: sampler 10
// tx0: sampler 0
#define EyePos xe_fc[0]  // c4
#define DLightDir_0 xe_fc[1]  // c5
#define DLightDir_1 xe_fc[2]  // c6
#define DLightDir_2 xe_fc[3]  // c7
#define PntLightPos_0 xe_fc[4]  // c8
#define PntLightPos_1 xe_fc[5]  // c9
#define PntLightPos_2 xe_fc[6]  // c10
#define PntLightPos_3 xe_fc[7]  // c11
#define PntLightAtten_0 xe_fc[8]  // c16
#define PntLightAtten_1 xe_fc[9]  // c17
#define PntLightAtten_2 xe_fc[10]  // c18
#define PntLightAtten_3 xe_fc[11]  // c19
#define SpotLightDir_0 xe_fc[12]  // c24
#define SpotLightDir_1 xe_fc[13]  // c25
#define SpotLightDir_2 xe_fc[14]  // c26
#define SpotLightDir_3 xe_fc[15]  // c27
#define SpotLightCone_0 xe_fc[16]  // c32
#define SpotLightCone_1 xe_fc[17]  // c33
#define SpotLightCone_2 xe_fc[18]  // c34
#define SpotLightCone_3 xe_fc[19]  // c35
#define AmbientColor xe_fc[20]  // c40
#define DLightColor_0 xe_fc[21]  // c41
#define DLightColor_1 xe_fc[22]  // c42
#define DLightColor_2 xe_fc[23]  // c43
#define DLightSpecularColor_0 xe_fc[24]  // c44
#define DLightSpecularColor_1 xe_fc[25]  // c45
#define DLightSpecularColor_2 xe_fc[26]  // c46
#define PntLightColor_0 xe_fc[27]  // c47
#define PntLightColor_1 xe_fc[28]  // c48
#define PntLightColor_2 xe_fc[29]  // c49
#define PntLightColor_3 xe_fc[30]  // c50
#define PntLightSpecularColor_0 xe_fc[31]  // c55
#define PntLightSpecularColor_1 xe_fc[32]  // c56
#define PntLightSpecularColor_2 xe_fc[33]  // c57
#define PntLightSpecularColor_3 xe_fc[34]  // c58
#define MatParam xe_fc[35]  // c64
#define MatAmbientDiffuse xe_fc[36]  // c65
#define MatSpecularColor xe_fc[37]  // c66
#define shadowMapSize xe_fc[38]  // c77
const vec4 lit246 = vec4(1e-05, 0.0, 0.0625, 1.0);
const vec4 lit247 = vec4(0.5, 0.0, 0.0, 0.0);
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
  bool p = false;
  precise float ps = 0.0;
  do {
    r5.xyz = -r0.xyz + EyePos.xyz;
    r2.z = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = xe_dp3(r5.zxy, r5.zxy); precise float s = inversesqrt(abs(r2.z)); ps = s; r0.w = v; r2.z = s; }
    { precise vec3 v = xe_mul(r2.zzz, r1.xzy); precise float s = inversesqrt(abs(r0.w)); ps = s; r8.xyz = v; r0.w = s; }
    { precise vec3 v = xe_mul(r5.zxy, r0.www); precise float s = lit246.w; ps = s; r5.xyz = v; r1.y = s; }
    { precise float v = xe_dp3(-r5.xyz, r8.yxz); precise float s = lit246.w; ps = s; r0.w = v; r1.x = s; }
    { precise float v = r0.w + r0.w; precise float s = lit246.w; ps = s; r0.w = v; r1.z = s; }
    r5.xzw = xe_mul(-r8.yzx, r0.www) + -r5.xzy;
    if (xe_bool(128u /* UseProjTex */)) {
      ps = 1.0 / r3.w;
      r0.w = ps;
      r1.xyz = xe_mul(r0.www, r3.xyz);
      r6.xz = xe_mul(r1.xy, lit247.xx) + lit247.xx;
      { precise float v = -r6.z + lit246.w; precise float s = 1.0 / shadowMapSize.x; ps = s; r6.y = v; r0.w = s; }
      r12 = xe_mul(r0.wwww, lit252) + r6.xyxy;
      r3 = xe_mul(r0.wwww, lit253) + r6.xyxy;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r9 = xe_mul(r0.wwww, lit254) + r6.xyxy;
      r14 = xe_mul(r0.wwww, lit255) + r6.xyxy;
      r13 = xe_mul(r0.wwww, lit248) + r6.xyxy;
      r10 = xe_mul(r0.wwww, lit249) + r6.xyxy;
      r7 = xe_mul(r0.wwww, lit250) + r6.xyxy;
      r11 = xe_mul(r0.wwww, lit251) + r6.xyxy;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r6.x = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r11.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r6.y = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r7.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r6.z = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r7.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r7.x = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r10.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r7.y = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r10.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r7.z = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r13.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r10.x = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r14.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r10.y = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r14.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r10.z = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r9.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r10.w = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r13.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r9.x = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r9.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r9.y = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r9.z = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r3.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r3.x = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r12.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r3.y = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r12.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r3.z = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 10u, r11.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      r2.zw = vec2(greaterThan(lit246.ww, abs(r1.xy)));
      r3.xzw = -r1.zzz + r3.xyz;
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r9.xyz = -r1.zzz + r9.xyz;
      r0.w = (lit246.w > r1.z ? 1.0 : 0.0);
      r11 = -r1.zzzz + r10;
      r12.xyz = -r1.zzz + r7.xyz;
      r6.xyz = -r1.zzz + r6.xyz;
      { precise vec3 v = vec3(greaterThan(-r6.zxy, lit246.yyy)); precise float s = (r1.z > 0.0 ? 1.0 : 0.0); ps = s; r6.xyz = v; r1.x = s; }
    }
    if (xe_bool(128u /* UseProjTex */)) {
      { precise vec3 v = vec3(greaterThan(-r11.zxy, lit246.yyy)); precise float s = (-r12.z > 0.0 ? 1.0 : 0.0); ps = s; r10.xyz = v; r7.x = s; }
      { precise float v = xe_mul(r1.x, r0.w); precise float s = (-r12.x > 0.0 ? 1.0 : 0.0); ps = s; r3.y = v; r7.y = s; }
      { precise vec3 v = vec3(greaterThan(-r9.zxy, lit246.yyy)); precise float s = (-r12.y > 0.0 ? 1.0 : 0.0); ps = s; r9.xyz = v; r7.z = s; }
      { precise vec3 v = vec3(greaterThan(-r3.wxz, lit246.yyy)); precise float s = (-r11.w > 0.0 ? 1.0 : 0.0); ps = s; r1.xyz = v; r0.w = s; }
      r9.w = xe_dp2add(r1.zy, lit246.zz, lit246.y);
      { precise float v = xe_mul(r9.y, lit246.z); precise float s = xe_mul(lit246.z, r1.x); ps = s; r10.w = v; r6.w = s; }
    }
    if (xe_bool(128u /* UseProjTex */)) {
      { precise float v = xe_dp4(r10.yzxw, lit246.zzzw); precise float s = xe_mul(lit246.z, r0.w); ps = s; r1.w = v; r7.w = s; }
      { precise float v = xe_dp3(r9.zxw, lit246.zzw); precise float s = (!(r3.y == 0.0) ? 1.0 : 0.0); ps = s; r3.x = v; r3.z = s; }
      { precise float v = xe_dp4(r7.yzxw, lit246.zzzw); precise float s = r2.z; ps = s; r3.y = v; }
      { precise float v = xe_dp4(r6.yxzw, lit246.zzzw); precise float s = xe_mul(r3.z, ps); ps = s; r0.w = v; r1.x = s; }
      { precise float v = r3.y + r0.w; precise float s = (!(r1.x == 0.0) ? 1.0 : 0.0); ps = s; r0.w = v; r1.x = s; }
      { precise float v = r0.w + r3.x; precise float s = r2.w; ps = s; r0.w = v; }
    }
    if (xe_bool(128u /* UseProjTex */)) {
      { precise float v = r0.w + r1.w; precise float s = xe_mul(r1.x, ps); ps = s; r0.w = v; r1.x = s; }
      r1.y = (r1.x == 0.0 ? lit246.w : r0.w);
      r1.xz = r1.yy;
    }
    r3.xyz = xe_mul(DLightSpecularColor_2.zyx, MatSpecularColor.zyx);
    r9.xyz = xe_mul(DLightSpecularColor_1.zyx, MatSpecularColor.zyx);
    r10.xyz = xe_mul(DLightSpecularColor_0.xyz, MatSpecularColor.xyz);
    r13.xyz = xe_mul(DLightColor_2.zyx, MatAmbientDiffuse.www);
    r14.xyz = xe_mul(DLightColor_1.zyx, MatAmbientDiffuse.www);
    r7.xyz = xe_mul(DLightColor_0.zyx, MatAmbientDiffuse.www);
    r2.z = xe_dp3(r5.xwz, DLightDir_0.zxy);
    r2.w = xe_dp3(r8.yxz, DLightDir_0.zxy);
    r11.x = xe_dp3(r5.wzx, DLightDir_2.xyz);
    r11.y = xe_dp3(r8.xzy, DLightDir_2.xyz);
    r11.z = xe_dp3(r8.xzy, DLightDir_1.xyz);
    r11.w = xe_dp3(r5.wzx, DLightDir_1.xyz);
    r6.xyz = -r0.xzy + PntLightPos_0.xzy;
    r15.y = xe_dp3(r6.yxz, r6.yxz);
    r12 = xe_max(r11, lit246.xyyx);
    { precise vec2 v = xe_max(r2.zw, lit246.xy); precise float s = inversesqrt(abs(r15.y)); ps = s; r11.zw = v; r0.w = s; }
    r6.xzw = xe_mul(r6.yxz, r0.www);
    { precise vec3 v = xe_mul(r7.zxy, r11.www); precise float s = lit246.w; ps = s; r7.xyz = v; r15.z = s; }
    { precise vec3 v = xe_mul(r7.zxy, r1.xyz); precise float s = sqrt(abs(r15.y)); ps = s; r7.xyz = v; r15.x = s; }
    { precise float v = xe_dp3(PntLightAtten_0.xyz, r15.yxz); precise float s = log2(abs(r12.x)); ps = s; r1.w = v; r11.x = s; }
    { precise float v = xe_dp3(r6.xzw, SpotLightDir_0.zxy); precise float s = log2(abs(r12.w)); ps = s; r0.w = v; r11.y = s; }
    { precise float v = (r0.w > SpotLightCone_0.x ? 1.0 : 0.0); precise float s = log2(abs(r11.z)); ps = s; r2.w = v; r11.z = s; }
    { precise vec3 v = xe_mul(r11.xyz, MatParam.xxx); precise float s = 1.0 / r1.w; ps = s; r11.xyz = v; r6.y = s; }
    r7.xyz = xe_mul(r14.xyz, r12.zzz) + r7.zxy;
    r7.xyz = xe_mul(r13.zxy, r12.yyy) + r7.zxy;
    { precise float v = r0.w + -SpotLightCone_0.y; precise float s = exp2(r11.x); ps = s; r0.w = v; r1.w = s; }
    { precise float v = xe_mul(r0.w, SpotLightCone_0.z); precise float s = exp2(r11.z); ps = s; r0.w = v; r2.z = s; }
    { precise vec3 v = xe_mul(r10.zxy, r2.zzz); precise float s = exp2(r11.y); ps = s; r10.xyz = v; r2.z = s; }
    r5.y = (r2.w == 0.0 ? r0.w : lit246.w);
    { precise vec3 v = xe_mul(r10.zxy, r1.xzy); p = r5.y > 0.0; precise float s = (p ? 0.0 : 1.0); ps = s; r1.xyz = v; }
    r1.xyz = xe_mul(r9.zyx, r2.zzz) + r1.zxy;
    r3.xyz = xe_mul(r3.xzy, r1.www) + r1.zxy;
    if (p) {
      r10.xyz = xe_mul(PntLightSpecularColor_0.xyz, MatSpecularColor.xyz);
      r1.xyz = xe_mul(PntLightColor_0.xzy, MatAmbientDiffuse.www);
      r9.y = xe_mul(r8.y, r6.x);
      r11.x = xe_mul(r6.z, r5.w);
      r11.y = xe_mul(r8.x, r6.z);
      { precise float v = xe_mul(r8.z, r6.w); precise float s = r6.w; ps = s; r2.w = v; }
    }
    if (p) {
      { precise vec2 v = xe_mul(r6.xy, r5.xy); precise float s = xe_mul(r5.z, ps); ps = s; r9.xz = v; r2.z = s; }
      r2.zw = r11.xy + r2.zw;
      r2.zw = r2.zw + r9.xy;
      r2.zw = xe_max(r2.zw, lit246.xy);
      r11.xyz = xe_mul(r1.zxy, r2.www);
      r1.xy = xe_mul(r11.zx, r9.zz);
    }
    if (p) {
      { precise float v = xe_mul(r11.y, r5.y); precise float s = log2(abs(r2.z)); ps = s; r1.z = v; r0.w = s; }
      ps = xe_mul(MatParam.x, r0.w);
      r0.w = ps;
      { precise float v = xe_mul(r1.z, r6.y); precise float s = exp2(r0.w); ps = s; r1.z = v; r0.w = s; }
      r10.xyz = xe_mul(r10.zxy, r0.www);
      r3.xyz = xe_mul(r10.xyz, r9.zzz) + r3.xyz;
      r7.xyz = r7.xyz + r1.zxy;
    }
    r1.xyz = -r0.xyz + PntLightPos_1.xyz;
    r6.x = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = lit246.w; precise float s = inversesqrt(abs(r6.x)); ps = s; r6.z = v; r0.w = s; }
    { precise vec3 v = xe_mul(r1.zyx, r0.www); precise float s = sqrt(abs(r6.x)); ps = s; r1.xyz = v; r6.y = s; }
    r2.w = xe_dp3(PntLightAtten_1.xyz, r6.xyz);
    r0.w = xe_dp3(r1.xzy, SpotLightDir_1.zxy);
    r2.z = (r0.w > SpotLightCone_1.x ? 1.0 : 0.0);
    r0.w = r0.w + -SpotLightCone_1.y;
    { precise float v = xe_mul(r0.w, SpotLightCone_1.z); precise float s = 1.0 / r2.w; ps = s; r1.w = v; r0.w = s; }
    r5.y = (r2.z == 0.0 ? r1.w : lit246.w);
    p = r5.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r10.xyz = xe_mul(PntLightSpecularColor_1.zyx, MatSpecularColor.zyx);
      r6.xyz = xe_mul(PntLightColor_1.zyx, MatAmbientDiffuse.www);
      r9.y = xe_mul(r8.y, r1.x);
      r11.x = xe_mul(r1.z, r5.w);
      r11.y = xe_mul(r8.x, r1.z);
      { precise float v = xe_mul(r8.z, r1.y); precise float s = r1.y; ps = s; r2.w = v; }
    }
    if (p) {
      r1.y = r0.w;
      { precise vec2 v = xe_mul(r1.xy, r5.xy); precise float s = xe_mul(r5.z, ps); ps = s; r9.xz = v; r2.z = s; }
      r2.zw = r11.xy + r2.zw;
      r1.zw = r2.zw + r9.xy;
      r2.zw = xe_max(r1.zw, lit246.xy);
      r11.xyz = xe_mul(r6.zxy, r2.www);
    }
    if (p) {
      r6.xy = xe_mul(r11.zy, r9.zz);
      { precise float v = xe_mul(r11.x, r5.y); precise float s = log2(abs(r2.z)); ps = s; r1.z = v; r0.w = s; }
      ps = xe_mul(MatParam.x, r0.w);
      r0.w = ps;
      { precise float v = xe_mul(r1.z, r1.y); precise float s = exp2(r0.w); ps = s; r6.z = v; r0.w = s; }
      r10.xyz = xe_mul(r10.zxy, r0.www);
      r3.xyz = xe_mul(r10.yxz, r9.zzz) + r3.xyz;
    }
    if (p) {
      r7.xyz = r6.zyx + r7.xyz;
    }
    r1.xyz = -r0.xyz + PntLightPos_2.xyz;
    r6.x = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = lit246.w; precise float s = inversesqrt(abs(r6.x)); ps = s; r6.z = v; r0.w = s; }
    { precise vec3 v = xe_mul(r1.zyx, r0.www); precise float s = sqrt(abs(r6.x)); ps = s; r1.xyz = v; r6.y = s; }
    r1.w = xe_dp3(PntLightAtten_2.xyz, r6.xyz);
    r0.w = xe_dp3(r1.xzy, SpotLightDir_2.zxy);
    r2.z = (r0.w > SpotLightCone_2.x ? 1.0 : 0.0);
    r0.w = r0.w + -SpotLightCone_2.y;
    { precise float v = xe_mul(r0.w, SpotLightCone_2.z); precise float s = 1.0 / r1.w; ps = s; r0.w = v; r1.w = s; }
    r5.y = (r2.z == 0.0 ? r0.w : lit246.w);
    p = r5.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r10.xyz = xe_mul(PntLightSpecularColor_2.zyx, MatSpecularColor.zyx);
      r6.xyz = xe_mul(PntLightColor_2.zyx, MatAmbientDiffuse.www);
      r9.y = xe_mul(r8.y, r1.x);
      r11.x = xe_mul(r1.z, r5.w);
      r11.y = xe_mul(r8.x, r1.z);
      { precise float v = xe_mul(r8.z, r1.y); precise float s = r1.y; ps = s; r2.w = v; }
    }
    if (p) {
      r1.y = r1.w;
      { precise vec2 v = xe_mul(r1.xy, r5.xy); precise float s = xe_mul(r5.z, ps); ps = s; r9.xz = v; r2.z = s; }
      r2.zw = r11.xy + r2.zw;
      r1.zw = r2.zw + r9.xy;
      r2.zw = xe_max(r1.zw, lit246.xy);
      r11.xyz = xe_mul(r6.zxy, r2.www);
    }
    if (p) {
      r6.xy = xe_mul(r11.zy, r9.zz);
      { precise float v = xe_mul(r11.x, r5.y); precise float s = log2(abs(r2.z)); ps = s; r1.z = v; r0.w = s; }
      ps = xe_mul(MatParam.x, r0.w);
      r0.w = ps;
      { precise float v = xe_mul(r1.z, r1.y); precise float s = exp2(r0.w); ps = s; r6.z = v; r0.w = s; }
      r10.xyz = xe_mul(r10.zxy, r0.www);
      r3.xyz = xe_mul(r10.yxz, r9.zzz) + r3.xyz;
    }
    if (p) {
      r7.xyz = r6.zyx + r7.xyz;
    }
    r0.yzw = -r0.xyz + PntLightPos_3.xyz;
    r1.x = xe_dp3(r0.wyz, r0.wyz);
    { precise float v = lit246.w; precise float s = inversesqrt(abs(r1.x)); ps = s; r1.z = v; r0.x = s; }
    { precise vec3 v = xe_mul(r0.wzy, r0.xxx); precise float s = sqrt(abs(r1.x)); ps = s; r0.yzw = v; r1.y = s; }
    r1.z = xe_dp3(PntLightAtten_3.xyz, r1.xyz);
    r0.x = xe_dp3(r0.ywz, SpotLightDir_3.zxy);
    r1.y = (r0.x > SpotLightCone_3.x ? 1.0 : 0.0);
    r0.x = r0.x + -SpotLightCone_3.y;
    { precise float v = xe_mul(r0.x, SpotLightCone_3.z); precise float s = 1.0 / r1.z; ps = s; r1.x = v; r0.x = s; }
    r5.y = (r1.y == 0.0 ? r1.x : lit246.w);
    p = r5.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r9.xyz = xe_mul(PntLightSpecularColor_3.zyx, MatSpecularColor.zyx);
      r1.xyz = xe_mul(PntLightColor_3.zyx, MatAmbientDiffuse.www);
      r6.y = xe_mul(r8.y, r0.y);
      r10.x = xe_mul(r0.w, r5.w);
      r10.y = xe_mul(r8.x, r0.w);
      { precise float v = xe_mul(r8.z, r0.z); precise float s = r0.z; ps = s; r2.w = v; }
    }
    if (p) {
      r0.z = r0.x;
      { precise vec2 v = xe_mul(r0.yz, r5.xy); precise float s = xe_mul(r5.z, ps); ps = s; r6.xz = v; r2.z = s; }
      r2.zw = r10.xy + r2.zw;
      r0.xy = r2.zw + r6.xy;
      r0.xy = xe_max(r0.xy, lit246.xy);
      r8.xyz = xe_mul(r1.zxy, r0.yyy);
    }
    if (p) {
      r1.xy = xe_mul(r8.zy, r6.zz);
      { precise float v = xe_mul(r8.x, r5.y); precise float s = log2(abs(r0.x)); ps = s; r0.w = v; r0.y = s; }
      ps = xe_mul(MatParam.x, r0.y);
      r0.x = ps;
      { precise float v = xe_mul(r0.w, r0.z); precise float s = exp2(r0.x); ps = s; r1.z = v; r0.x = s; }
      r9.xyz = xe_mul(r9.zxy, r0.xxx);
      r3.xyz = xe_mul(r9.yxz, r6.zzz) + r3.xyz;
    }
    if (p) {
      r7.xyz = r1.zyx + r7.xyz;
    }
    r0 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r1.xyz = AmbientColor.zyx + MatAmbientDiffuse.zyx;
    { precise vec3 v = r1.zxy + r7.xyz; precise float s = r0.w; ps = s; r1.xyz = v; }
    { precise vec3 v = xe_mul(r1.yxz, r4.zxy); precise float s = xe_mul(r4.w, ps); ps = s; r1.xyz = v; r4.w = clamp(s, 0.0, 1.0); }
    r4.xyz = xe_mul(r1.xzy, r0.zyx) + r3.xzy;
    if (xe_bool(128u /* UseProjTex */)) {
      r4.xyz = xe_max(r4.xyz, lit246.yyy);
    }
    if (xe_bool(128u /* UseProjTex */)) {
      r4.xyz = xe_max(r4.xyz, lit246.yyy);
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
