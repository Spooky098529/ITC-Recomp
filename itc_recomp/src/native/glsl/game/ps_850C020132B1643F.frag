#version 450
// Pixel shader 850C020132B1643F, first found in 1B241258D8D8FA8F.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 5
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

// cubetx0: sampler 6
// tx0: sampler 0
// tx1: sampler 1
#define EyePos xe_fc[0]  // c4
#define MatParam xe_fc[1]  // c64
#define ModTexAlpha xe_fc[2]  // c67
#define fogColor xe_fc[3]  // c76
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
    r0.yzw = -r0.xyz + EyePos.xyz;
    r1.w = xe_dp3(r1.zxy, r1.zxy);
    { precise float v = xe_dp3(r0.wyz, r0.wyz); precise float s = inversesqrt(abs(r1.w)); r0.x = v; r1.w = s; }
    { precise vec3 v = xe_mul(r1.www, r1.xyz); precise float s = inversesqrt(abs(r0.x)); r1.xyz = v; r0.x = s; }
    r0.xyz = xe_mul(r0.wyz, r0.xxx);
    r6 = mix(lit255.yyyy, -r0.xyzz, equal(lit255.xxxy, vec4(0.0)));
    r1.w = xe_dp3(-r0.xyz, r1.zxy);
    r0.w = xe_dp4(r1.zxyw, r6);
    r0.xyz = xe_mul(-r1.xzy, r0.www) + -r0.yxz;
    r0 = xe_cube(r0.yyxz);
    { precise float v = r0.w; precise float s = 1.0 / abs(r0.z); r1.z = v; r1.x = s; }
    r1.xy = xe_mul(r0.yx, r1.xx) + lit255.zz;
    r1.xyz = xe_tfetchcube(xe_texture0, xe_texture1, xe_sampler0, 6u, r1.xyz, vec3(0.0014648438, 0.0014648438, 0.0), 0.0, true).xyz;
    r4 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 1u, r4.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r3 = xe_tfetch2d(xe_texture4, xe_texture5, xe_sampler2, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0.x = xe_max(r2.x, fogColor.w);
    r2.xyz = xe_mul(r4.xyz, r5.xyz) + -r3.xyz;
    { precise float v = xe_mul(r4.w, ModTexAlpha.y); precise float s = r0.x; r0.y = v; r0.x = s; }
    { precise float e = clamp(xe_mul(r3.w, ModTexAlpha.x) + r0.y, 0.0, 1.0); xe_color0.w = e; }
    { precise float v = xe_mul(r0.y, r5.w); precise float s = lit255.y; r5.x = v; r0.y = s; }
    r5.xyz = xe_mul(r5.xxx, r2.yzx) + r3.yzx;
    r5.xyz = xe_mul(r1.xyz, MatParam.yyy) + r5.zxy;
    { precise vec3 v = r5.zxy + -fogColor.zxy; precise float s = xe_min(r0.x, r0.y); r5.yzw = v; r5.x = s; }
    { precise vec3 e = xe_mul(r5.zwy, r5.xxx) + fogColor.xyz; xe_color0.xyz = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
