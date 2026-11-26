#version 450
// Pixel shader 89DB3429AA053B0E, first found in 00000034.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 1
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 1, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 1, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 0
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 1

// tx0: sampler 0
// tx1: sampler 1
const vec4 lit255 = vec4(0.010050506, 0.5, 0.0, 0.0);

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
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
  bool p = false;
  do {
    r0.x = lit255.y;
    r0.y = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xx, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
    r0.x = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 1u, r0.xx, vec2(0.0014648438, 0.0014648438), 0.0, true).x;
    { precise float e = r1.w; xe_color0.w = e; }
    r0.y = r0.y + -r0.x;
    r0.x = xe_mul(r0.y, lit255.x) + r0.x;
    { precise vec3 e = xe_mul(r0.xxx, r1.xyz); xe_color0.xyz = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
