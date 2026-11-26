#version 450
// Pixel shader 3D74C039B92C9A15, first found in 00001290.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 4
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform textureCube xe_texture0;  // fetch constant 6, unsigned
layout(set = 3, binding = 1) uniform textureCube xe_texture1;  // fetch constant 6, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 1, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 1, signed
layout(set = 3, binding = 4) uniform texture2DArray xe_texture4;  // fetch constant 0, unsigned
layout(set = 3, binding = 5) uniform texture2DArray xe_texture5;  // fetch constant 0, signed
layout(set = 3, binding = 6) uniform sampler xe_sampler0;  // fetch constant 6
layout(set = 3, binding = 7) uniform sampler xe_sampler1;  // fetch constant 1
layout(set = 3, binding = 8) uniform sampler xe_sampler2;  // fetch constant 0

// UseProjTex: bool 0
// cubetx0: sampler 6
// tx0: sampler 0
// tx1: sampler 1
#define EyePos xe_fc[0]  // c4
#define MatParam xe_fc[1]  // c64
#define ModTexAlpha xe_fc[2]  // c67
const vec4 lit255 = vec4(0.0, 1.0, 1.5, 0.0);

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
  bool p = false;
  precise float ps = 0.0;
  do {
    r0.yzw = -r0.xyz + EyePos.xyz;
    r2.z = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = xe_dp3(r0.wyz, r0.wyz); precise float s = inversesqrt(abs(r2.z)); ps = s; r0.x = v; r2.z = s; }
    { precise vec3 v = xe_mul(r2.zzz, r1.xyz); precise float s = inversesqrt(abs(r0.x)); ps = s; r1.xyz = v; r0.x = s; }
    r0.xyz = xe_mul(r0.wyz, r0.xxx);
    r5 = mix(lit255.yyyy, -r0.xyzz, equal(lit255.xxxy, vec4(0.0)));
    r1.w = xe_dp3(-r0.xyz, r1.zxy);
    r0.w = xe_dp4(r1.zxyw, r5);
    r0.xyz = xe_mul(-r1.xzy, r0.www) + -r0.yxz;
    r0 = xe_cube(r0.yyxz);
    { precise float v = r0.w; precise float s = 1.0 / abs(r0.z); ps = s; r1.z = v; r1.x = s; }
    r1.xy = xe_mul(r0.yx, r1.xx) + lit255.zz;
    r0.xzw = xe_tfetchcube(xe_texture0, xe_texture1, xe_sampler0, 6u, r1.xyz, vec3(0.0014648438, 0.0014648438, 0.0), 0.0, true).xyz;
    r5 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r2 = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    { precise float v = -r5.w + lit255.y; precise float s = r5.w; ps = s; r1.x = v; }
    { precise vec3 v = xe_mul(r2.xyz, ModTexAlpha.xxx); precise float s = r2.w + ps; ps = s; r3.xyz = v; r0.y = clamp(s, 0.0, 1.0); }
    { precise vec3 v = xe_mul(r5.xyz, ModTexAlpha.yyy); precise float s = xe_mul(MatParam.y, r0.x); ps = s; r2.xyz = v; r1.y = s; }
    { precise vec3 v = xe_mul(r2.zyx, r5.www); precise float s = xe_mul(MatParam.y, r0.z); ps = s; r2.xyz = v; r1.z = s; }
    { precise vec3 v = xe_mul(r2.zxy, r4.www); precise float s = xe_mul(MatParam.y, r0.w); ps = s; r2.xyz = v; r1.w = s; }
    r4.xyz = xe_mul(r3.yxz, r4.yxz) + r2.zxy;
    r0.xzw = xe_mul(r1.wzy, r1.xxx) + r4.zxy;
    if (xe_bool(128u /* UseProjTex */)) {
      r0.xzw = xe_max(r0.xzw, lit255.xxx);
    }
    { precise vec4 e = r0.wzxy; xe_color0 = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
