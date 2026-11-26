#version 450
// Pixel shader 0E7B2A6B2E322308, first found in 00022805.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 22
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 1, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 1, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 0, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 0, signed
layout(set = 3, binding = 4) uniform texture2DArray xe_texture4;  // fetch constant 6, unsigned
layout(set = 3, binding = 5) uniform texture2DArray xe_texture5;  // fetch constant 6, signed
layout(set = 3, binding = 6) uniform sampler xe_sampler0;  // fetch constant 1
layout(set = 3, binding = 7) uniform sampler xe_sampler1;  // fetch constant 0
layout(set = 3, binding = 8) uniform sampler xe_sampler2;  // fetch constant 6

// UseProjTex: bool 0
// refmasktx0: sampler 1
// spheretx0: sampler 6
// tx0: sampler 0
// tx1: sampler 1
#define matView_0 xe_fc[0]  // c0
#define matView_1 xe_fc[1]  // c1
#define matView_2 xe_fc[2]  // c2
#define EyePos xe_fc[3]  // c4
#define PntLightPos_0 xe_fc[4]  // c8
#define PntLightPos_1 xe_fc[5]  // c9
#define PntLightAtten_0 xe_fc[6]  // c16
#define PntLightAtten_1 xe_fc[7]  // c17
#define SpotLightDir_0 xe_fc[8]  // c24
#define SpotLightDir_1 xe_fc[9]  // c25
#define SpotLightCone_0 xe_fc[10]  // c32
#define SpotLightCone_1 xe_fc[11]  // c33
#define AmbientColor xe_fc[12]  // c40
#define PntLightColor_0 xe_fc[13]  // c47
#define PntLightColor_1 xe_fc[14]  // c48
#define PntLightSpecularColor_0 xe_fc[15]  // c55
#define PntLightSpecularColor_1 xe_fc[16]  // c56
#define MatParam xe_fc[17]  // c64
#define MatAmbientDiffuse xe_fc[18]  // c65
#define MatSpecularColor xe_fc[19]  // c66
const vec4 lit254 = vec4(0.1, 0.8, 0.2, 0.7);
const vec4 lit255 = vec4(0.0, 1.0, 0.001, 0.5);

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
  bool p = false;
  precise float ps = 0.0;
  do {
    r2.z = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).w;
    r10.w = -MatParam.y + lit254.w;
    r10.xyz = -MatSpecularColor.xyz + lit254.xxx;
    r5.xyz = -r0.zyx + PntLightPos_0.zyx;
    r2.w = xe_dp3(r1.zxy, r1.zxy);
    r6.xyz = -r0.xyz + EyePos.xyz;
    { precise float v = xe_dp3(r6.zxy, r6.zxy); precise float s = (-abs(r4.x) > 0.0 ? 1.0 : 0.0); ps = s; r0.w = v; r9.x = s; }
    { precise float v = xe_dp3(r5.xzy, r5.xzy); precise float s = inversesqrt(abs(r2.w)); ps = s; r7.y = v; r2.w = s; }
    { precise vec3 v = xe_mul(r2.www, r1.xyz); precise float s = lit255.y; ps = s; r8.xyz = v; r7.z = s; }
    { precise vec4 v = xe_mul(r10.zxyw, r2.zzzz); precise float s = inversesqrt(abs(r7.y)); ps = s; r1 = v; r2.z = s; }
    { precise vec3 v = xe_mul(r5.zxy, r2.zzz); precise float s = inversesqrt(abs(r0.w)); ps = s; r5.xyw = v; r0.w = s; }
    { precise vec3 v = xe_mul(r6.zxy, r0.www); precise float s = sqrt(abs(r7.y)); ps = s; r6.xyz = v; r7.x = s; }
    { precise float v = xe_dp3(PntLightAtten_0.xyz, r7.yxz); precise float s = r1.w; ps = s; r2.w = v; r0.w = s; }
    { precise float v = xe_dp3(-r6.xyz, r8.zxy); precise float s = MatParam.y + r0.w; ps = s; r3.w = v; r2.z = s; }
    { precise float v = xe_dp3(r6.xyz, r8.zxy); precise float s = r1.y; ps = s; r5.z = v; r0.w = s; }
    { precise float v = xe_dp3(r5.yxw, SpotLightDir_0.zxy); precise float s = MatSpecularColor.x + r0.w; ps = s; r1.w = v; r1.y = s; }
    { precise float v = (r1.w > SpotLightCone_0.x ? 1.0 : 0.0); precise float s = MatSpecularColor.z + r1.x; ps = s; r3.z = v; r1.x = s; }
    { precise float v = xe_max(r5.z, lit255.x); precise float s = r1.w; ps = s; r0.w = v; }
    { precise float v = r3.w + r3.w; precise float s = -SpotLightCone_0.y + ps; ps = s; r3.w = v; r1.w = s; }
    r6.xzw = xe_mul(-r8.zyx, r3.www) + -r6.xzy;
    { precise float v = xe_mul(r1.w, SpotLightCone_0.z); precise float s = lit255.y - r0.w; ps = s; r1.w = v; r0.w = s; }
    r6.y = (r3.z == 0.0 ? r1.w : lit255.y);
    { precise float v = xe_mul(r0.w, r0.w); precise float s = MatSpecularColor.y + r1.z; ps = s; r3.z = v; r1.z = s; }
    { precise float v = xe_mul(r3.z, r3.z); precise float s = 1.0 / r2.w; ps = s; r1.w = v; r5.z = s; }
    { precise float v = xe_mul(r1.w, r0.w); p = r6.y > 0.0; precise float s = (p ? 0.0 : 1.0); ps = s; r0.w = v; }
    r7.z = xe_mul(r0.w, lit254.y) + lit254.z;
    ps = (-abs(r4.w) > 0.0 ? 1.0 : 0.0);
    r1.w = ps;
    if (xe_bool(128u /* UseProjTex */)) {
      ps = (abs(r4.w) >= 0.0 ? 1.0 : 0.0);
      r1.w = ps;
    }
    if (!p) {
      r9.yz = r9.xx;
      r5.xyz = r9.xxx;
    }
    if (p) {
      r9.xyz = xe_mul(PntLightColor_0.xyz, MatAmbientDiffuse.www);
      r7.y = xe_mul(r8.z, r5.y);
      r11.x = xe_mul(r5.x, r6.w);
      { precise float v = xe_mul(r5.w, r6.z); precise float s = r8.x; ps = s; r3.z = v; }
      { precise vec2 v = xe_mul(r5.yz, r6.xy); precise float s = xe_mul(r5.x, ps); ps = s; r7.xw = v; r11.y = s; }
      { precise vec3 v = xe_mul(r1.yzx, PntLightSpecularColor_0.xyz); precise float s = r8.y; ps = s; r10.xyz = v; }
    }
    if (p) {
      { precise vec3 v = xe_mul(r10.zxy, r7.zzz); precise float s = xe_mul(r5.w, ps); ps = s; r10.xyz = v; r3.w = s; }
      r3.zw = r11.xy + r3.zw;
      r3.zw = r3.zw + r7.xy;
      r11.xyz = xe_mul(r9.zxy, abs(r3.www));
      r9.yz = xe_mul(r11.xz, r7.ww);
      { precise float v = xe_mul(r11.y, r6.y); precise float s = log2(abs(r3.z)); ps = s; r2.w = v; r0.w = s; }
    }
    if (p) {
      ps = xe_mul(MatParam.x, r0.w);
      r0.w = ps;
      { precise float v = xe_mul(r2.w, r5.z); precise float s = exp2(r0.w); ps = s; r9.x = v; r0.w = s; }
      r5.xyz = xe_mul(r10.zxy, r0.www);
      r5.xyz = xe_mul(r5.xyz, r7.www);
    }
    r0.yzw = -r0.xyz + PntLightPos_1.xyz;
    r10.x = xe_dp3(r0.wyz, r0.wyz);
    { precise float v = lit255.y; precise float s = inversesqrt(abs(r10.x)); ps = s; r10.z = v; r0.x = s; }
    r7.x = xe_mul(r0.w, r0.x);
    { precise vec2 v = xe_mul(r0.zy, r0.xx); precise float s = sqrt(abs(r10.x)); ps = s; r0.xy = v; r10.y = s; }
    r2.w = xe_dp3(PntLightAtten_1.xyz, r10.xyz);
    r10.xyz = mix(r7.xxx, r0.yxx, equal(lit255.xxy, vec3(0.0)));
    r0.z = xe_dp3(r10.xyz, SpotLightDir_1.xyz);
    r0.w = (r0.z > SpotLightCone_1.x ? 1.0 : 0.0);
    r0.z = r0.z + -SpotLightCone_1.y;
    { precise float v = xe_mul(r0.z, SpotLightCone_1.z); precise float s = 1.0 / r2.w; ps = s; r0.z = v; r7.y = s; }
    r6.y = (r0.w == 0.0 ? r0.z : lit255.y);
    p = r6.y > 0.0;
    ps = (p ? 0.0 : 1.0);
    if (p) {
      r12.xyz = xe_mul(PntLightColor_1.zyx, MatAmbientDiffuse.www);
      { precise float v = xe_mul(r0.y, r6.w); precise float s = r8.z; ps = s; r3.z = v; }
      { precise float v = xe_mul(r8.x, r0.y); precise float s = xe_mul(r7.x, ps); ps = s; r3.w = v; r10.y = s; }
      { precise float v = xe_mul(r8.y, r0.x); precise float s = r0.x; ps = s; r0.w = v; }
      { precise vec2 v = xe_mul(r7.xy, r6.xy); precise float s = xe_mul(r6.z, ps); ps = s; r10.xz = v; r0.z = s; }
      { precise vec2 v = r3.zw + r0.zw; precise float s = xe_mul(PntLightSpecularColor_1.x, r1.y); ps = s; r0.zw = v; r11.x = s; }
    }
    if (p) {
      { precise vec2 v = r0.zw + r10.xy; precise float s = xe_mul(PntLightSpecularColor_1.y, r1.z); ps = s; r0.xy = v; r11.y = s; }
      { precise vec3 v = xe_mul(r12.zxy, abs(r0.yyy)); precise float s = log2(abs(r0.x)); ps = s; r12.xyz = v; r0.y = s; }
      r9.yz = xe_mul(r12.yz, r10.zz) + r9.yz;
      { precise float v = xe_mul(r12.x, r6.y); precise float s = xe_mul(PntLightSpecularColor_1.z, r1.x); ps = s; r11.w = v; r11.z = s; }
      { precise vec4 v = xe_mul(r11.zxyw, r7.zzzy); precise float s = xe_mul(MatParam.x, r0.y); ps = s; r8 = v; r0.x = s; }
      { precise float v = r8.w + r9.x; precise float s = exp2(r0.x); ps = s; r9.x = v; r0.x = s; }
    }
    if (p) {
      r11.xyz = xe_mul(r8.zxy, r0.xxx);
      r5.xyz = xe_mul(r11.xyz, r10.zzz) + r5.xyz;
    }
    r0.xyz = xe_mul(r6.zzz, matView_1.zyx);
    r0.xyz = xe_mul(r6.www, matView_0.xzy) + r0.zxy;
    r0.yzw = xe_mul(-r6.xxx, matView_2.yxz) + -r0.zxy;
    r0.x = xe_dp3(r0.wzy, r0.wzy);
    ps = inversesqrt(abs(r0.x));
    r0.x = ps;
    r0.xyz = xe_mul(r0.wyz, r0.xxx);
    ps = lit255.y + r0.x;
    r0.w = ps;
    r1.x = xe_dp3(r0.zyw, r0.zyw);
    ps = sqrt(abs(r1.x));
    r1.x = ps;
    ps = r1.x + r1.x;
    r1.x = ps;
    ps = 1.0 / r1.x;
    r1.x = ps;
    r1.xy = xe_mul(r0.zy, r1.xx) + lit255.ww;
    r3 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r1.xyz = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 6u, r1.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r7.xyw = AmbientColor.zyx + MatAmbientDiffuse.zyx;
    r1.xyz = xe_mul(r2.zzz, r1.xyz);
    r4.w = xe_max(r0.w, r4.w);
    r7.xyw = r7.wxy + r9.xyz;
    r4.xyz = xe_mul(r7.wxy, r4.yxz);
    r2.w = r4.w + r3.w;
    r6.xyz = xe_mul(r4.www, r0.zyx);
    r2.xyz = xe_mul(r3.xzy, r3.www) + r6.zxy;
    r4.w = xe_mul(-r4.w, r3.w) + r2.w;
    { precise float v = (r4.w > lit255.z ? 1.0 : 0.0); precise float s = 1.0 / r4.w; ps = s; r0.w = v; r0.x = s; }
    r2.xyz = xe_mul(-r6.xzy, r3.www) + r2.yxz;
    r0.xyz = xe_mul(r2.zyx, r0.xxx);
    r0.xyz = mix(r0.yzx, lit255.xxx, equal(r0.www, vec3(0.0)));
    r4.xyz = xe_mul(r4.xzy, r0.zyx) + r5.xyz;
    r0.xyz = xe_mul(r1.yzx, r7.zzz) + r4.xyz;
    { precise vec3 v = xe_max(r0.yxz, lit255.xxx); p = !(r1.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r4.xyz = v; }
    r4.xyz = mix(r4.xyz, r0.yxz, equal(r1.www, vec3(0.0)));
    if (p) {
      r4.xyz = xe_max(r4.xyz, lit255.xxx);
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
