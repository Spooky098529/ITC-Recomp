#version 450
// Pixel shader 33AE995D552BA7E8, first found in 00001288.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 42
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 2, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 2, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 8, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 8, signed
layout(set = 3, binding = 4) uniform texture2DArray xe_texture4;  // fetch constant 11, unsigned
layout(set = 3, binding = 5) uniform texture2DArray xe_texture5;  // fetch constant 11, signed
layout(set = 3, binding = 6) uniform texture2DArray xe_texture6;  // fetch constant 1, unsigned
layout(set = 3, binding = 7) uniform texture2DArray xe_texture7;  // fetch constant 1, signed
layout(set = 3, binding = 8) uniform texture2DArray xe_texture8;  // fetch constant 0, unsigned
layout(set = 3, binding = 9) uniform texture2DArray xe_texture9;  // fetch constant 0, signed
layout(set = 3, binding = 10) uniform texture2DArray xe_texture10;  // fetch constant 9, unsigned
layout(set = 3, binding = 11) uniform texture2DArray xe_texture11;  // fetch constant 9, signed
layout(set = 3, binding = 12) uniform texture2DArray xe_texture12;  // fetch constant 10, unsigned
layout(set = 3, binding = 13) uniform texture2DArray xe_texture13;  // fetch constant 10, signed
layout(set = 3, binding = 14) uniform sampler xe_sampler0;  // fetch constant 2
layout(set = 3, binding = 15) uniform sampler xe_sampler1;  // fetch constant 8
layout(set = 3, binding = 16) uniform sampler xe_sampler2;  // fetch constant 11
layout(set = 3, binding = 17) uniform sampler xe_sampler3;  // fetch constant 1
layout(set = 3, binding = 18) uniform sampler xe_sampler4;  // fetch constant 0
layout(set = 3, binding = 19) uniform sampler xe_sampler5;  // fetch constant 9
layout(set = 3, binding = 20) uniform sampler xe_sampler6;  // fetch constant 10

// UseProjTex: bool 0
// btx0: sampler 2
// plmask00: sampler 11
// plmask10: sampler 11
// plmask20: sampler 11
// plmask30: sampler 11
// plshd00: sampler 8
// prjtx0: sampler 9
// prjtx1: sampler 10
// tx0: sampler 0
// tx1: sampler 1
#define c0 xe_fc[0]  // c0
#define EyePos xe_fc[1]  // c4
#define PntLightPos_0 xe_fc[2]  // c8
#define PntLightPos_1 xe_fc[3]  // c9
#define PntLightPos_2 xe_fc[4]  // c10
#define PntLightPos_3 xe_fc[5]  // c11
#define PntLightAtten_0 xe_fc[6]  // c16
#define PntLightAtten_1 xe_fc[7]  // c17
#define PntLightAtten_2 xe_fc[8]  // c18
#define PntLightAtten_3 xe_fc[9]  // c19
#define SpotLightDir_0 xe_fc[10]  // c24
#define SpotLightDir_1 xe_fc[11]  // c25
#define SpotLightDir_2 xe_fc[12]  // c26
#define SpotLightDir_3 xe_fc[13]  // c27
#define SpotLightCone_0 xe_fc[14]  // c32
#define SpotLightCone_1 xe_fc[15]  // c33
#define SpotLightCone_2 xe_fc[16]  // c34
#define SpotLightCone_3 xe_fc[17]  // c35
#define PntLightColor_0 xe_fc[18]  // c47
#define PntLightColor_1 xe_fc[19]  // c48
#define PntLightColor_2 xe_fc[20]  // c49
#define PntLightColor_3 xe_fc[21]  // c50
#define PntLightSpecularColor_0 xe_fc[22]  // c55
#define PntLightSpecularColor_1 xe_fc[23]  // c56
#define PntLightSpecularColor_2 xe_fc[24]  // c57
#define PntLightSpecularColor_3 xe_fc[25]  // c58
#define MatSpecularColor xe_fc[26]  // c66
#define ModTexAlpha xe_fc[27]  // c67
#define ProjectionPos_0 xe_fc[28]  // c68
#define ProjectionPos_1 xe_fc[29]  // c69
#define shadowMapSize xe_fc[30]  // c77
const vec4 lit245 = vec4(2.0, -1.0, 0.0, 0.0);
const vec4 lit246 = vec4(1e-05, 3.0, 0.5, 0.33333334);
const vec4 lit247 = vec4(0.0, 1.0, 0.0625, 30.0);
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
  precise vec4 r18 = vec4(0.0);
  precise vec4 r19 = vec4(0.0);
  precise vec4 r20 = vec4(0.0);
  precise vec4 r21 = vec4(0.0);
  bool p = false;
  precise float ps = 0.0;
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r15 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 2u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r16 = xe_mul(r2.yzxy, r1.zxyx);
      r12.yzw = -r0.xyz + EyePos.xyz;
      r13.xyz = xe_mul(r15.xzy, lit245.xxx) + lit245.yyy;
      r12.x = xe_dp3(r12.wyz, r12.wyz);
      ps = r16.z - r16.w;
      r14.z = ps;
      r14.xy = xe_mul(-r2.zx, r1.yz) + r16.xy;
      { precise vec3 v = xe_mul(r14.yxz, r2.www); precise float s = inversesqrt(abs(r12.x)); ps = s; r16.xyz = v; r12.x = s; }
      r12.xyz = xe_mul(r12.wyz, r12.xxx);
      r14 = mix(lit247.yyyy, -r12.xyzz, equal(lit247.xxxy, vec4(0.0)));
      r16.xyz = xe_mul(r16.zyx, r13.zzz);
      r2.xyz = xe_mul(r13.xxx, r2.yxz) + r16.zyx;
      r2.xyz = xe_mul(r13.yyy, r1.zxy) + r2.zyx;
      r12.w = xe_dp3(r2.xyz, r2.xyz);
      { precise float v = (PntLightColor_0.w > lit246.z ? 1.0 : 0.0); precise float s = inversesqrt(abs(r12.w)); ps = s; r0.w = v; r12.w = s; }
      { precise vec3 v = xe_mul(r2.xzy, r12.www); precise float s = (-abs(r12.x) > 0.0 ? 1.0 : 0.0); ps = s; r2.xyz = v; r13.z = s; }
      { precise vec3 v = xe_mul(r2.zyx, r15.www); precise float s = (-abs(r12.x) > 0.0 ? 1.0 : 0.0); ps = s; r2.xyz = v; r13.x = s; }
      { precise float v = xe_dp3(-r12.xyz, r2.zxy); precise float s = (-abs(r12.x) > 0.0 ? 1.0 : 0.0); ps = s; r2.w = v; r13.y = s; }
      { precise float v = xe_dp4(r2.zxyw, r14); p = !(r0.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r12.w = v; }
      r14.xyz = xe_mul(-r2.zyx, r12.www) + -r12.xzy;
      ps = (-abs(r12.w) > 0.0 ? 1.0 : 0.0);
      r1.w = ps;
      if (xe_bool(128u /* UseProjTex */)) {
        ps = (abs(r12.w) >= 0.0 ? 1.0 : 0.0);
        r1.w = ps;
      }
      if (!p) {
        pc = 27; continue;
      }
      r2.xyz = -r0.xzy + PntLightPos_0.xzy;
      r12.y = xe_dp3(r2.yxz, r2.yxz);
      { precise float v = lit247.y; precise float s = inversesqrt(abs(r12.y)); ps = s; r12.z = v; r12.x = s; }
      { precise vec3 v = xe_mul(r2.yzx, r12.xxx); precise float s = sqrt(abs(r12.y)); ps = s; r2.xyz = v; r12.x = s; }
      r12.y = xe_dp3(PntLightAtten_0.xyz, r12.yxz);
      r12.x = xe_dp3(r2.xzy, SpotLightDir_0.zxy);
      r12.z = (r12.x > SpotLightCone_0.x ? 1.0 : 0.0);
      r12.x = r12.x + -SpotLightCone_0.y;
      { precise float v = xe_mul(r12.x, SpotLightCone_0.z); precise float s = (-abs(r12.x) > 0.0 ? 1.0 : 0.0); ps = s; r12.x = v; r13.z = s; }
      r12.x = (r12.z == 0.0 ? r12.x : lit247.y);
      { vec4 pa = lit247.xxxx, pb = r12.xxxx; p = pa.w == 0.0 && pb.w > 0.0; precise float v = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; precise float s = 1.0 / r12.y; ps = s; r0.w = v; r12.y = s; }
      if (p) {
        r12.w = xe_mul(r12.y, r12.x);
      }
      if (p) {
        ps = lit247.y;
        r12.x = ps;
      }
      { vec4 pa = r0.wwww, pb = r1.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        { precise float v = lit247.y; precise float s = 1.0 / r5.w; ps = s; r2.w = v; r12.x = s; }
        r12.xyz = xe_mul(r12.xxx, r5.xyz);
        r3.zw = vec2(greaterThan(lit247.yy, abs(r12.xy)));
        { precise float v = (lit247.y > r12.z ? 1.0 : 0.0); precise float s = (r12.z > 0.0 ? 1.0 : 0.0); ps = s; r4.w = v; r4.z = s; }
      }
      if (p) {
        r4.z = xe_mul(r4.z, r4.w);
        ps = (!(r4.z == 0.0) ? 1.0 : 0.0);
        r4.z = ps;
        r4.z = xe_mul(r3.z, r4.z);
        ps = (!(r4.z == 0.0) ? 1.0 : 0.0);
        r4.z = ps;
        r3.z = xe_mul(r3.w, r4.z);
      }
      { vec4 pa = r0.wwww, pb = r3.zzzz; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r5.xz = xe_mul(r12.xy, lit246.zz) + lit246.zz;
        { precise float v = -r5.z + lit247.y; precise float s = 1.0 / shadowMapSize.x; ps = s; r5.y = v; r12.x = s; }
        r19 = xe_mul(r12.xxxx, lit252) + r5.xyxy;
        r18 = xe_mul(r12.xxxx, lit253) + r5.xyxy;
        r15 = xe_mul(r12.xxxx, lit254) + r5.xyxy;
        r21 = xe_mul(r12.xxxx, lit255) + r5.xyxy;
      }
      if (p) {
        r16 = xe_mul(r12.xxxx, lit250) + r5.xyxy;
        r17 = xe_mul(r12.xxxx, lit251) + r5.xyxy;
        r20 = xe_mul(r12.xxxx, lit248) + r5.xyxy;
        r13 = xe_mul(r12.xxxx, lit249) + r5.xyxy;
        r5.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r13.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r5.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r13.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      if (p) {
        r5.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r20.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r13.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r17.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r13.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r16.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r13.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r16.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r16.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r21.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r16.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r21.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      if (p) {
        r16.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r15.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r16.w = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r20.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r15.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r15.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r15.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r18.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r15.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r18.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r18.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r19.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      if (p) {
        r18.y = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r19.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        r18.z = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 8u, r17.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
        { precise vec3 v = -r12.zzz + r15.xyz; precise float s = -r12.z; ps = s; r15.xyz = v; }
        { precise vec4 v = -r12.zzzz + r16; precise float s = r18.x + ps; ps = s; r16 = v; r17.y = s; }
        { precise vec3 v = -r12.zzz + r13.xyz; precise float s = -r12.z; ps = s; r13.xyz = v; }
        { precise vec3 v = -r12.zzz + r5.xyz; precise float s = r18.y + ps; ps = s; r5.xyz = v; r17.z = s; }
      }
      if (p) {
        { precise vec3 v = vec3(greaterThan(-r5.zxy, lit247.xxx)); precise float s = -r12.z; ps = s; r5.xyz = v; }
        { precise vec3 v = vec3(greaterThan(-r13.zxy, lit247.xxx)); precise float s = r18.z + ps; ps = s; r13.xyz = v; r17.w = s; }
        { precise vec3 v = vec3(greaterThan(-r15.zxy, lit247.xxx)); precise float s = (-r16.z > 0.0 ? 1.0 : 0.0); ps = s; r15.xyz = v; r17.x = s; }
        { precise vec3 v = vec3(greaterThan(-r17.ywz, lit247.xxx)); precise float s = (-r16.x > 0.0 ? 1.0 : 0.0); ps = s; r12.xyz = v; r17.y = s; }
        r15.w = xe_dp2add(r12.zx, lit247.zz, lit247.x);
        { precise float v = xe_mul(r15.y, lit247.z); precise float s = (-r16.y > 0.0 ? 1.0 : 0.0); ps = s; r17.w = v; r17.z = s; }
      }
      if (p) {
        { precise float v = xe_dp4(r17.yzxw, lit247.zzzy); precise float s = (-r16.w > 0.0 ? 1.0 : 0.0); ps = s; r2.w = v; r12.x = s; }
        { precise float v = xe_dp3(r15.zxw, lit247.zzy); precise float s = xe_mul(lit247.z, r12.y); ps = s; r3.z = v; r13.w = s; }
        { precise float v = xe_dp4(r13.yxzw, lit247.zzzy); precise float s = xe_mul(lit247.z, r12.x); ps = s; r3.w = v; r5.w = s; }
        r12.x = xe_dp4(r5.yzxw, lit247.zzzy);
        r12.x = r12.x + r3.w;
        r12.x = r12.x + r3.z;
      }
      if (p) {
        r2.w = r12.x + r2.w;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
      if (p) {
        { precise float v = (-abs(r12.x) > lit247.x ? 1.0 : 0.0); precise float s = 1.0 / r8.w; ps = s; r12.z = v; r3.z = s; }
        { precise vec2 v = xe_mul(r3.zz, r8.xy); precise float s = r3.z; ps = s; r12.xy = v; }
        { precise vec2 v = vec2(greaterThan(lit247.yy, abs(r12.yx))); precise float s = xe_mul(r8.z, ps); ps = s; r3.zw = v; r4.z = s; }
        { precise float v = (lit247.y > r4.z ? 1.0 : 0.0); precise float s = (r4.z > 0.0 ? 1.0 : 0.0); ps = s; r4.w = v; r4.z = s; }
        r4.z = xe_mul(r4.z, r4.w);
      }
      if (p) {
        ps = (!(r4.z == 0.0) ? 1.0 : 0.0);
        r4.z = ps;
        r4.z = xe_mul(r3.w, r4.z);
        ps = (!(r4.z == 0.0) ? 1.0 : 0.0);
        r4.z = ps;
        r3.z = xe_mul(r3.z, r4.z);
      }
      { vec4 pa = r0.wwww, pb = r3.zzzz; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r12.xy = xe_mul(r12.yx, lit246.zz) + lit246.zz;
        ps = lit247.y - r12.x;
        r12.z = ps;
        r12.z = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 11u, r12.yz, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
      if (p) {
        r12.x = xe_mul(r2.w, r12.z);
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
      if (p) {
        r12.y = xe_dp3(r2.xzy, r14.xzy);
        r12.z = xe_max(r12.y, lit246.x);
        { precise float v = xe_mul(r12.w, r12.x); precise float s = log2(abs(r12.z)); ps = s; r12.y = v; r12.x = s; }
        ps = xe_mul(lit247.w, r12.x);
        r12.x = ps;
        { precise vec3 v = xe_mul(PntLightSpecularColor_0.xyz, MatSpecularColor.xyz); precise float s = exp2(r12.x); ps = s; r2.xyz = v; r12.x = s; }
        r12.xzw = xe_mul(r2.zxy, r12.xxx);
      }
      if (p) {
        r13.xyz = xe_mul(r12.wxz, r12.yyy);
      }
      p = r0.w == 1.0;
      ps = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      r0.w = ps;
      if (p) {
        r13.xy = r13.zz;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
    case 27:
      r12.x = (PntLightColor_1.w > lit246.z ? 1.0 : 0.0);
      p = !(r12.x == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (!p) {
        pc = 39; continue;
      }
      r2.xyz = -r0.xyz + PntLightPos_1.xyz;
      r12.x = xe_dp3(r2.zxy, r2.zxy);
      { precise float v = lit247.y; precise float s = inversesqrt(abs(r12.x)); ps = s; r12.z = v; r12.y = s; }
      { precise vec3 v = xe_mul(r2.zyx, r12.yyy); precise float s = sqrt(abs(r12.x)); ps = s; r2.xyz = v; r12.y = s; }
      r12.y = xe_dp3(PntLightAtten_1.xyz, r12.xyz);
      r12.x = xe_dp3(r2.xzy, SpotLightDir_1.zxy);
      r12.z = (r12.x > SpotLightCone_1.x ? 1.0 : 0.0);
      r12.x = r12.x + -SpotLightCone_1.y;
      ps = xe_mul(SpotLightCone_1.z, r12.x);
      r12.x = ps;
      r12.x = (r12.z == 0.0 ? r12.x : lit247.y);
      { vec4 pa = lit247.xxxx, pb = r12.xxxx; p = pa.w == 0.0 && pb.w > 0.0; precise float v = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; precise float s = 1.0 / r12.y; ps = s; r12.w = v; r12.y = s; }
      if (p) {
        r2.w = xe_mul(r12.y, r12.x);
      }
      if (p) {
        ps = lit247.y;
        r14.w = ps;
      }
      { vec4 pa = r12.wwww, pb = r1.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r12.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        { precise float v = (-abs(r12.x) > lit247.x ? 1.0 : 0.0); precise float s = 1.0 / r9.w; ps = s; r14.w = v; r12.z = s; }
        { precise vec2 v = xe_mul(r12.zz, r9.xy); precise float s = r12.z; ps = s; r12.xy = v; }
        { precise vec2 v = vec2(greaterThan(lit247.yy, abs(r12.xy))); precise float s = xe_mul(r9.z, ps); ps = s; r3.zw = v; r12.z = s; }
        { precise float v = (lit247.y > r12.z ? 1.0 : 0.0); precise float s = (r12.z > 0.0 ? 1.0 : 0.0); ps = s; r0.w = v; r12.z = s; }
      }
      if (p) {
        r12.z = xe_mul(r12.z, r0.w);
        ps = (!(r12.z == 0.0) ? 1.0 : 0.0);
        r12.z = ps;
        r12.z = xe_mul(r3.z, r12.z);
        ps = (!(r12.z == 0.0) ? 1.0 : 0.0);
        r12.z = ps;
        r12.z = xe_mul(r3.w, r12.z);
      }
      { vec4 pa = r12.wwww, pb = r12.zzzz; p = pa.w == 0.0 && !(pb.w == 0.0); r12.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r12.xy = xe_mul(r12.yx, lit246.zz) + lit246.zz;
        ps = lit247.y - r12.x;
        r12.z = ps;
        r14.w = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 11u, r12.yz, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      { float pop = r12.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.w = ps; }
      { float pop = r12.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.w = ps; }
      if (p) {
        r12.x = xe_dp3(r2.xzy, r14.xzy);
        r12.x = xe_max(r12.x, lit246.x);
        { precise float v = xe_mul(r2.w, r14.w); precise float s = log2(abs(r12.x)); ps = s; r12.y = v; r12.x = s; }
        ps = xe_mul(lit247.w, r12.x);
        r12.x = ps;
        { precise vec3 v = xe_mul(PntLightSpecularColor_1.zyx, MatSpecularColor.zyx); precise float s = exp2(r12.x); ps = s; r2.xyz = v; r12.x = s; }
        r5.xyz = xe_mul(r2.zxy, r12.xxx);
      }
      if (p) {
        r13.xyz = xe_mul(r5.zyx, r12.yyy) + r13.xyz;
      }
      { float pop = r12.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.w = ps; }
    case 39:
      r12.x = (PntLightColor_2.w > lit246.z ? 1.0 : 0.0);
      p = !(r12.x == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (!p) {
        pc = 51; continue;
      }
      r2.xyz = -r0.xyz + PntLightPos_2.xyz;
      r12.x = xe_dp3(r2.zxy, r2.zxy);
      { precise float v = lit247.y; precise float s = inversesqrt(abs(r12.x)); ps = s; r12.z = v; r12.y = s; }
      { precise vec3 v = xe_mul(r2.zyx, r12.yyy); precise float s = sqrt(abs(r12.x)); ps = s; r2.xyz = v; r12.y = s; }
      r12.z = xe_dp3(PntLightAtten_2.xyz, r12.xyz);
      r12.x = xe_dp3(r2.xzy, SpotLightDir_2.zxy);
      r12.y = (r12.x > SpotLightCone_2.x ? 1.0 : 0.0);
      r12.x = r12.x + -SpotLightCone_2.y;
      ps = xe_mul(SpotLightCone_2.z, r12.x);
      r12.x = ps;
      r12.y = (r12.y == 0.0 ? r12.x : lit247.y);
      { vec4 pa = lit247.xxxx, pb = r12.yyyy; p = pa.w == 0.0 && pb.w > 0.0; precise float v = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; precise float s = 1.0 / r12.z; ps = s; r0.w = v; r12.x = s; }
      if (p) {
        r12.w = xe_mul(r12.x, r12.y);
      }
      if (p) {
        ps = lit247.y;
        r12.x = ps;
      }
      { vec4 pa = r0.wwww, pb = r1.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        { precise float v = (-abs(r12.x) > lit247.x ? 1.0 : 0.0); precise float s = 1.0 / r10.w; ps = s; r12.x = v; r2.w = s; }
        { precise vec2 v = xe_mul(r2.ww, r10.xy); precise float s = r2.w; ps = s; r12.yz = v; }
        { precise vec2 v = vec2(greaterThan(lit247.yy, abs(r12.yz))); precise float s = xe_mul(r10.z, ps); ps = s; r3.zw = v; r2.w = s; }
        { precise float v = (lit247.y > r2.w ? 1.0 : 0.0); precise float s = (r2.w > 0.0 ? 1.0 : 0.0); ps = s; r4.z = v; r2.w = s; }
      }
      if (p) {
        r2.w = xe_mul(r2.w, r4.z);
        ps = (!(r2.w == 0.0) ? 1.0 : 0.0);
        r2.w = ps;
        r2.w = xe_mul(r3.z, r2.w);
        ps = (!(r2.w == 0.0) ? 1.0 : 0.0);
        r2.w = ps;
        r2.w = xe_mul(r3.w, r2.w);
      }
      { vec4 pa = r0.wwww, pb = r2.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r12.xy = xe_mul(r12.zy, lit246.zz) + lit246.zz;
        ps = lit247.y - r12.x;
        r12.z = ps;
        r12.x = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 11u, r12.yz, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
      if (p) {
        r12.y = xe_dp3(r2.xzy, r14.xzy);
        r12.z = xe_max(r12.y, lit246.x);
        { precise float v = xe_mul(r12.w, r12.x); precise float s = log2(abs(r12.z)); ps = s; r12.y = v; r12.x = s; }
        ps = xe_mul(lit247.w, r12.x);
        r12.x = ps;
        { precise vec3 v = xe_mul(PntLightSpecularColor_2.zyx, MatSpecularColor.zyx); precise float s = exp2(r12.x); ps = s; r2.xyz = v; r12.x = s; }
        r5.xyz = xe_mul(r2.zxy, r12.xxx);
      }
      if (p) {
        r13.xyz = xe_mul(r5.zyx, r12.yyy) + r13.xyz;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.w = ps; }
    case 51:
      r12.x = (PntLightColor_3.w > lit246.z ? 1.0 : 0.0);
      p = !(r12.x == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (!p) {
        pc = 63; continue;
      }
      r2.xyz = -r0.xyz + PntLightPos_3.xyz;
      r12.x = xe_dp3(r2.zxy, r2.zxy);
      { precise float v = lit247.y; precise float s = inversesqrt(abs(r12.x)); ps = s; r12.z = v; r12.y = s; }
      { precise vec3 v = xe_mul(r2.zyx, r12.yyy); precise float s = sqrt(abs(r12.x)); ps = s; r2.xyz = v; r12.y = s; }
      r12.z = xe_dp3(PntLightAtten_3.xyz, r12.xyz);
      r12.x = xe_dp3(r2.xzy, SpotLightDir_3.zxy);
      r12.y = (r12.x > SpotLightCone_3.x ? 1.0 : 0.0);
      r12.x = r12.x + -SpotLightCone_3.y;
      ps = xe_mul(SpotLightCone_3.z, r12.x);
      r12.x = ps;
      r12.y = (r12.y == 0.0 ? r12.x : lit247.y);
      { vec4 pa = lit247.xxxx, pb = r12.yyyy; p = pa.w == 0.0 && pb.w > 0.0; precise float v = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; precise float s = 1.0 / r12.z; ps = s; r12.w = v; r12.x = s; }
      if (p) {
        r2.w = xe_mul(r12.x, r12.y);
      }
      if (p) {
        ps = lit247.y;
        r14.w = ps;
      }
      { vec4 pa = r12.wwww, pb = r1.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r12.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        { precise float v = (-abs(r12.x) > lit247.x ? 1.0 : 0.0); precise float s = 1.0 / r11.w; ps = s; r14.w = v; r12.z = s; }
        { precise vec2 v = xe_mul(r12.zz, r11.xy); precise float s = r12.z; ps = s; r12.xy = v; }
        { precise vec2 v = vec2(greaterThan(lit247.yy, abs(r12.xy))); precise float s = xe_mul(r11.z, ps); ps = s; r3.zw = v; r0.w = s; }
        { precise float v = (lit247.y > r0.w ? 1.0 : 0.0); precise float s = (r0.w > 0.0 ? 1.0 : 0.0); ps = s; r12.z = v; r0.w = s; }
      }
      if (p) {
        r12.z = xe_mul(r0.w, r12.z);
        ps = (!(r12.z == 0.0) ? 1.0 : 0.0);
        r12.z = ps;
        r12.z = xe_mul(r3.z, r12.z);
        ps = (!(r12.z == 0.0) ? 1.0 : 0.0);
        r12.z = ps;
        r12.z = xe_mul(r3.w, r12.z);
      }
      { vec4 pa = r12.wwww, pb = r12.zzzz; p = pa.w == 0.0 && !(pb.w == 0.0); r12.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r12.xy = xe_mul(r12.yx, lit246.zz) + lit246.zz;
        ps = lit247.y - r12.x;
        r12.z = ps;
        r14.w = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 11u, r12.yz, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
      }
      { float pop = r12.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.w = ps; }
      { float pop = r12.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.w = ps; }
      if (p) {
        r12.x = xe_dp3(r2.xzy, r14.xzy);
        r12.x = xe_max(r12.x, lit246.x);
        { precise float v = xe_mul(r2.w, r14.w); precise float s = log2(abs(r12.x)); ps = s; r12.y = v; r12.x = s; }
        ps = xe_mul(lit247.w, r12.x);
        r12.x = ps;
        { precise vec3 v = xe_mul(PntLightSpecularColor_3.zyx, MatSpecularColor.zyx); precise float s = exp2(r12.x); ps = s; r2.xyz = v; r12.x = s; }
        r5.xyz = xe_mul(r2.zxy, r12.xxx);
      }
      if (p) {
        r13.xyz = xe_mul(r5.zyx, r12.yyy) + r13.xyz;
      }
      { float pop = r12.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.w = ps; }
    case 63:
      r4 = xe_tfetch2d(xe_texture6, xe_texture7, xe_sampler3, 1u, r4.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r12 = xe_tfetch2d(xe_texture8, xe_texture9, xe_sampler4, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r2.w = clamp(r4.w + r12.w, 0.0, 1.0);
      r12.xyz = xe_mul(r12.xyz, ModTexAlpha.xxx);
      r2.xyz = xe_mul(r4.xyz, ModTexAlpha.yyy) + -r12.xyz;
      r12.xyz = xe_mul(r2.zxy, r4.www) + r12.zxy;
      { precise vec3 v = r12.xzy + r13.yxz; p = !(r1.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r2.xyz = v; }
      if (!p) {
        pc = 89; continue;
      }
      { precise float v = (-abs(r12.x) > lit247.x ? 1.0 : 0.0); p = !(r1.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r0.z = v; }
      if (!p) {
        pc = 76; continue;
      }
      ps = ProjectionPos_0.w - r0.y;
      r12.z = ps;
      { precise float v = xe_dp3(r1.zxy, ProjectionPos_0.zxy); precise float s = 1.0 / r6.w; ps = s; r12.w = v; r12.y = s; }
      { precise float v = xe_mul(r12.y, r6.z); precise float s = abs(r12.z); ps = s; r12.x = v; }
      { precise float v = (lit247.y > r12.x ? 1.0 : 0.0); precise float s = xe_mul(lit246.w, ps); ps = s; r0.x = v; r12.x = clamp(s, 0.0, 1.0); }
      ps = lit247.y - r12.x;
      r12.z = ps;
      r12.x = r6.z;
      r3.xyz = vec3(greaterThan(r12.xzw, lit247.xxx));
      r12.x = xe_mul(r3.x, r0.x);
      ps = (!(r12.x == 0.0) ? 1.0 : 0.0);
      r12.x = ps;
      r12.x = xe_mul(r3.z, r12.x);
      ps = (!(r12.x == 0.0) ? 1.0 : 0.0);
      r12.x = ps;
      r12.x = xe_mul(r3.y, r12.x);
      { vec4 pa = lit247.xxxx, pb = r12.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r0.x = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r3.xy = xe_mul(r6.xy, lit246.zz);
        r12.xy = xe_mul(r3.yx, r12.yy) + lit246.zz;
        r3.zw = vec2(greaterThan(r12.xy, lit247.xx));
        r3.xy = vec2(greaterThan(lit247.yy, r12.xy));
        r0.w = xe_mul(r3.w, r3.y);
      }
      if (p) {
        ps = (!(r0.w == 0.0) ? 1.0 : 0.0);
        r0.w = ps;
        r0.w = xe_mul(r3.z, r0.w);
        ps = (!(r0.w == 0.0) ? 1.0 : 0.0);
        r0.w = ps;
        r0.w = xe_mul(r3.x, r0.w);
      }
      { vec4 pa = r0.xxxx, pb = r0.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r0.x = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        ps = lit247.y - r12.x;
        r12.x = ps;
        r3 = xe_tfetch2d(xe_texture10, xe_texture11, xe_sampler5, 9u, r12.yx, vec2(0.0014648438, 0.0014648438), 0.0, true);
        r12.x = xe_dp4(r3.wzxy, lit247.yyyy);
        r12.x = xe_mul(r12.x, r12.w);
        r0.z = xe_mul(r12.x, r12.z);
      }
      { float pop = r0.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.x = ps; }
      { float pop = r0.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r0.x = ps; }
      r12.x = r0.z;
      r12.y = r0.z;
      r0.xw = r12.xy;
      pc = 77; continue;
    case 76:
      r0.xw = r0.zz;
    case 77:
      { precise float v = (-abs(r12.x) > lit247.x ? 1.0 : 0.0); p = !(r1.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r12.w = v; }
      if (!p) {
        pc = 87; continue;
      }
      ps = ProjectionPos_1.w - r0.y;
      r12.z = ps;
      { precise float v = xe_dp3(r1.zxy, ProjectionPos_1.zxy); precise float s = 1.0 / r7.w; ps = s; r1.z = v; r12.y = s; }
      { precise float v = xe_mul(r12.y, r7.z); precise float s = abs(r12.z); ps = s; r12.x = v; }
      { precise float v = (lit247.y > r12.x ? 1.0 : 0.0); precise float s = xe_mul(lit246.w, ps); ps = s; r12.z = v; r12.x = clamp(s, 0.0, 1.0); }
      ps = lit247.y - r12.x;
      r1.y = ps;
      r1.x = r7.z;
      r3.xyz = vec3(greaterThan(r1.xyz, lit247.xxx));
      r12.x = xe_mul(r3.x, r12.z);
      ps = (!(r12.x == 0.0) ? 1.0 : 0.0);
      r12.x = ps;
      r12.x = xe_mul(r3.z, r12.x);
      ps = (!(r12.x == 0.0) ? 1.0 : 0.0);
      r12.x = ps;
      r12.x = xe_mul(r3.y, r12.x);
      { vec4 pa = lit247.xxxx, pb = r12.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r12.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r3.xy = xe_mul(r7.xy, lit246.zz);
        r12.xy = xe_mul(r3.yx, r12.yy) + lit246.zz;
        r3.zw = vec2(greaterThan(r12.xy, lit247.xx));
        r3.xy = vec2(greaterThan(lit247.yy, r12.xy));
        r0.y = xe_mul(r3.w, r3.y);
      }
      if (p) {
        ps = (!(r0.y == 0.0) ? 1.0 : 0.0);
        r0.y = ps;
        r0.y = xe_mul(r3.z, r0.y);
        ps = (!(r0.y == 0.0) ? 1.0 : 0.0);
        r0.y = ps;
        r0.y = xe_mul(r3.x, r0.y);
      }
      { vec4 pa = r12.zzzz, pb = r0.yyyy; p = pa.w == 0.0 && !(pb.w == 0.0); r12.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        ps = lit247.y - r12.x;
        r12.x = ps;
        r12.xyw = xe_tfetch2d(xe_texture12, xe_texture13, xe_sampler6, 10u, r12.yx, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
        r12.xyw = xe_mul(r1.zzz, r12.xyw);
        r12.xyw = xe_mul(r12.wxy, r1.yyy);
        r12.xyw = xe_mul(r12.xwy, lit246.yyy);
      }
      p = r12.z == 1.0;
      ps = (p ? 0.0 : (r12.z == 0.0 ? 1.0 : r12.z));
      r12.z = ps;
      if (p) {
        r12.xy = r12.ww;
      }
      { float pop = r12.z - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.z = ps; }
      p = r12.z == 1.0;
      ps = (p ? 0.0 : (r12.z == 0.0 ? 1.0 : r12.z));
      r12.z = ps;
      if (p) {
        r12.xy = r12.ww;
      }
      { float pop = r12.z - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r12.z = ps; }
      pc = 88; continue;
    case 87:
      r12.xy = r12.ww;
    case 88:
      r1.xyz = -r0.xzw + r12.xwy;
      r12.xyz = r1.xyz + r2.xzy;
      r2.xyz = xe_max(r12.xzy, lit247.xxx);
    case 89:
      p = !(r1.w == 0.0);
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r2.xyz = xe_max(r2.xyz, lit247.xxx);
      }
      { precise vec4 e = r2.zyxw; xe_color0 = e; }
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
