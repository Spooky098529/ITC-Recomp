#version 450
// Pixel shader 41798B3BB7FDDB71, first found in 00000054.xpu
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
const vec4 lit254 = vec4(0.141682, 0.094972, 0.148946, 0.0);
const vec4 lit255 = vec4(4.0, 3.0, 0.066926, 0.121947);

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
  bool p = false;
  precise float ps = 0.0;
  do {
    ps = r0.y;
    r3.y = ps;
    { precise float v = r0.y; precise float s = r0.y; ps = s; r5.y = v; r4.y = s; }
    { precise float v = r0.y; precise float s = r0.y; ps = s; r7.y = v; r6.y = s; }
    { precise float v = r0.x + -PS_CONSTANT_TEX_SIZE.x; precise float s = r0.y; ps = s; r2.z = v; r2.w = s; }
    { precise float v = r0.x + PS_CONSTANT_TEX_SIZE.x; precise float s = r0.y; ps = s; r2.x = v; r2.y = s; }
    { precise vec2 v = xe_mul(PS_CONSTANT_TEX_SIZE.xx, lit255.xy); precise float s = PS_CONSTANT_TEX_SIZE.x + PS_CONSTANT_TEX_SIZE.x; ps = s; r0.zw = v; r3.z = s; }
    { precise float v = -r0.w + r0.x; precise float s = r0.z; ps = s; r4.x = v; }
    { precise float v = r0.w + r0.x; precise float s = r0.x + ps; ps = s; r5.x = v; r3.x = s; }
    { precise float v = r3.z + r0.x; precise float s = -r3.z; ps = s; r7.x = v; }
    { precise float v = -r0.z + r0.x; precise float s = r0.x + ps; ps = s; r0.z = v; r6.x = s; }
    r3 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r4 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r4.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r5 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r5.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r6 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r6.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r7 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r7.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r8 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.zw, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r9 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r2 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.zy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0 = xe_mul(r0.wzyx, lit255.zzzz);
    r2 = xe_mul(r2, lit254.zzzz);
    r2 = xe_mul(r9.xzwy, lit254.xxxx) + r2.xzwy;
    r2 = xe_mul(r8.yzxw, lit254.xxxx) + r2.wyxz;
    r2 = xe_mul(r7.wzyx, lit255.wwww) + r2.wyxz;
    r2 = xe_mul(r6.xzwy, lit255.wwww) + r2.wyxz;
    r2 = xe_mul(r5.yzxw, lit254.yyyy) + r2.wyxz;
    r2 = xe_mul(r4.wzyx, lit254.yyyy) + r2.wyxz;
    r2 = xe_mul(r3.xzwy, lit255.zzzz) + r2.wyxz;
    { precise float e = r2.z + r0.x; xe_color0.w = e; }
    r0.xyz = r2.wyx + r0.zyw;
    { precise vec3 e = xe_mul(r0.zxy, r1.xyz); xe_color0.xyz = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
