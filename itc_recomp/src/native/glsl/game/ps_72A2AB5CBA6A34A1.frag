#version 450
// Pixel shader 72A2AB5CBA6A34A1, first found in 00001326.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 2
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
#define ModTexAlpha xe_fc[0]  // c67
const vec4 lit255 = vec4(0.0, 0.0, 0.0, 0.0);

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
layout(location = 2) in vec4 xe_in_interpolator_2;
layout(location = 3) in vec4 xe_in_interpolator_3;
layout(location = 4) in vec4 xe_in_interpolator_4;
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
  bool p = false;
  precise float ps = 0.0;
  do {
    r4 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    r0 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
    { precise vec3 v = xe_mul(r4.xyz, ModTexAlpha.yyy); precise float s = r4.w; ps = s; r1.xyz = v; }
    { precise vec3 v = xe_mul(r1.zyx, r4.www); precise float s = r0.w + ps; ps = s; r4.xyz = v; r4.w = clamp(s, 0.0, 1.0); }
    r4.xyz = xe_mul(r0.zyx, ModTexAlpha.xxx) + r4.xyz;
    if (xe_bool(128u /* UseProjTex */)) {
      r4.xyz = xe_max(r4.xyz, lit255.xxx);
    }
    if (xe_bool(128u /* UseProjTex */)) {
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
