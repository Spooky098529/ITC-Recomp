#version 450
// Pixel shader 92A5199E7EA052BB, first found in 00000048.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 2
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// tx0: sampler 0
#define PS_CONSTANT_TEX_SIZE xe_fc[0]  // c32
const vec4 lit255 = vec4(0.054489, 0.50262, 0.144201, 0.0);

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
    ps = r0.x;
    r3.x = ps;
    { precise float v = r0.y + -PS_CONSTANT_TEX_SIZE.y; precise float s = r0.x; ps = s; r2.w = v; r2.z = s; }
    { precise float v = PS_CONSTANT_TEX_SIZE.y + PS_CONSTANT_TEX_SIZE.y; precise float s = r0.x; ps = s; r0.z = v; r2.x = s; }
    { precise float v = r0.z + r0.y; precise float s = r0.y; ps = s; r3.y = v; }
    { precise float v = -r0.z + r0.y; precise float s = PS_CONSTANT_TEX_SIZE.y + ps; ps = s; r0.z = v; r2.y = s; }
    r3 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r4 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.zw, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r5 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r2 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xz, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0 = xe_mul(r0.wzyx, lit255.xxxx);
    r2 = xe_mul(r2, lit255.yyyy);
    r2 = xe_mul(r5.xzwy, lit255.zzzz) + r2.xzwy;
    r2 = xe_mul(r4.yzxw, lit255.zzzz) + r2.wyxz;
    r2 = xe_mul(r3.wzyx, lit255.xxxx) + r2.wyxz;
    { precise float e = r2.x + r0.x; xe_color0.w = e; }
    r0.xyz = r2.wyz + r0.wyz;
    { precise vec3 e = xe_mul(r0.xzy, r1.xyz); xe_color0.xyz = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
