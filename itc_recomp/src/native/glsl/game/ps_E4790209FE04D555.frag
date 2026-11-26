#version 450
// Pixel shader E4790209FE04D555, first found in 00001300.xpu
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
  do {
    r0.xyz = -r0.xyz + EyePos.xyz;
    r0.w = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = xe_dp3(r0.zxy, r0.zxy); precise float s = inversesqrt(abs(r0.w)); r4.y = v; r0.w = s; }
    { precise vec3 v = xe_mul(r0.www, r1.xyz); precise float s = inversesqrt(abs(r4.y)); r1.xyz = v; r4.y = s; }
    r0.xyz = xe_mul(r0.zxy, r4.yyy);
    r5 = mix(lit255.yyyy, -r0.xyzz, equal(lit255.xxxy, vec4(0.0)));
    r1.w = xe_dp3(-r0.xyz, r1.zxy);
    r4.y = xe_dp4(r1.zxyw, r5);
    r0.xyz = xe_mul(-r1.xzy, r4.yyy) + -r0.yxz;
    r0 = xe_cube(r0.yyxz);
    { precise float v = r0.w; precise float s = 1.0 / abs(r0.z); r1.z = v; r4.y = s; }
    r1.xy = xe_mul(r0.yx, r4.yy) + lit255.zz;
    r0.xyz = xe_tfetchcube(xe_texture0, xe_texture1, xe_sampler0, 6u, r1.xyz, vec3(0.0014648438, 0.0014648438, 0.0), 0.0, true).xyz;
    r3 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r2 = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r4.y = clamp(r3.w + r2.w, 0.0, 1.0);
    r0.yzw = xe_mul(r0.xyz, MatParam.yyy);
    { precise float v = xe_mul(r3.w, r4.w); precise float s = r3.w; r1.w = v; r4.x = s; }
    { precise vec3 v = xe_mul(r2.xyz, ModTexAlpha.xxx); precise float s = lit255.y - r4.x; r1.xyz = v; r0.x = s; }
    r4.xzw = xe_mul(r3.xyz, ModTexAlpha.yyy) + -r1.xyz;
    r4.xzw = xe_mul(r1.www, r4.zwx) + r1.yzx;
    r4.xzw = xe_mul(r0.wzy, r0.xxx) + r4.zxw;
    if (xe_bool(128u /* UseProjTex */)) {
      r4.xzw = xe_max(r4.xzw, lit255.xxx);
    }
    { precise vec4 e = r4.wzxy; xe_color0 = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
