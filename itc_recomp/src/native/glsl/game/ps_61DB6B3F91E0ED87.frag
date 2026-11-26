#version 450
// Pixel shader 61DB6B3F91E0ED87, first found in 00000038.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 16
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// tx0: sampler 0
#define g_avSampleOffsets_0 xe_fc[0]  // c0
#define g_avSampleOffsets_1 xe_fc[1]  // c1
#define g_avSampleOffsets_2 xe_fc[2]  // c2
#define g_avSampleOffsets_3 xe_fc[3]  // c3
#define g_avSampleOffsets_4 xe_fc[4]  // c4
#define g_avSampleOffsets_5 xe_fc[5]  // c5
#define g_avSampleOffsets_6 xe_fc[6]  // c6
#define g_avSampleOffsets_7 xe_fc[7]  // c7
#define g_avSampleWeights_0 xe_fc[8]  // c16
#define g_avSampleWeights_1 xe_fc[9]  // c17
#define g_avSampleWeights_2 xe_fc[10]  // c18
#define g_avSampleWeights_3 xe_fc[11]  // c19
#define g_avSampleWeights_4 xe_fc[12]  // c20
#define g_avSampleWeights_5 xe_fc[13]  // c21
#define g_avSampleWeights_6 xe_fc[14]  // c22
#define g_avSampleWeights_7 xe_fc[15]  // c23

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
  bool p = false;
  precise float ps = 0.0;
  do {
    r0.zw = r0.xy + g_avSampleOffsets_1.xy;
    r8.xy = r0.xy + g_avSampleOffsets_0.xy;
    r7.xy = r0.xy + g_avSampleOffsets_2.xy;
    { precise vec2 v = r0.xy + g_avSampleOffsets_4.xy; precise float s = r0.x; ps = s; r5.xy = v; }
    { precise vec2 v = r0.xy + g_avSampleOffsets_5.xy; precise float s = g_avSampleOffsets_3.x + ps; ps = s; r4.xy = v; r6.x = s; }
    { precise vec2 v = r0.xy + g_avSampleOffsets_6.xy; precise float s = r0.y; ps = s; r3.xy = v; }
    { precise vec2 v = r0.xy + g_avSampleOffsets_7.xy; precise float s = g_avSampleOffsets_3.y + ps; ps = s; r2.xy = v; r6.y = s; }
    r2.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r3.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r4.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r4.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r5.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r5.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r6.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r6.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r7.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r7.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r8.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r8.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    r0.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.zw, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
    ps = r1.w;
    xe_color0.w = ps;
    r0.xyz = xe_mul(r0.xyz, g_avSampleWeights_1.xyz);
    r0.xyz = xe_mul(r8.xyz, g_avSampleWeights_0.xyz) + r0.xyz;
    r0.xyz = xe_mul(r7.zxy, g_avSampleWeights_2.zxy) + r0.zxy;
    r0.xyz = xe_mul(r6.yzx, g_avSampleWeights_3.yzx) + r0.zxy;
    r0.xyz = xe_mul(r5.xyz, g_avSampleWeights_4.xyz) + r0.zxy;
    r0.xyz = xe_mul(r4.zxy, g_avSampleWeights_5.zxy) + r0.zxy;
    r0.xyz = xe_mul(r3.yzx, g_avSampleWeights_6.yzx) + r0.zxy;
    r0.xyz = xe_mul(r2.zyx, g_avSampleWeights_7.zyx) + r0.yxz;
    { precise vec3 e = xe_mul(r0.zyx, r1.xyz); xe_color0.xyz = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
