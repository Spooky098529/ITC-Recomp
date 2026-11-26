#version 450
// Pixel shader BDEC67D7B4DF91A1 (ps_03), first found in 00000581.part at 0xAE9A0
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// tx0: sampler 0
#define AmbientColor xe_fc[40]  // c40
#define MatAmbientDiffuse xe_fc[65]  // c65
#define ModTexAlpha xe_fc[67]  // c67
const vec4 lit255 = vec4(1.0, 0.0, 4.0, 0.0);

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
  bool p = false;
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r0.w = xe_dp3(r1.xyz, r1.xyz);
      { precise vec3 v = vec3(greaterThan(-abs(r3.xxx), lit255.yyy)); precise float s = inversesqrt(abs(r0.w)); r5.xyz = v; r0.w = s; }
      r4.xyz = xe_mul(r0.www, r1.xyz);
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 4; continue; }
    case 2:
      r1.xyz = xe_mul(xe_fc[loop_aL.x + 41].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r4.xyz, xe_fc[loop_aL.x + 5].xyz);
      r0.w = xe_max(r0.w, lit255.y);
      r5.xyz = xe_mul(r1.xyz, r0.www) + r5.xyz;
      // end of loop 31
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(31u)), 16, 8);
          pc = 2; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 4:
      r1.xy = lit255.xy;
      // loop 30
      loop_count = uvec4(xe_loop_constant(30u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(30u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 15; continue; }
    case 6:
      r7.xyz = xe_fc[loop_aL.x + 8].xyz + -r0.xyz;
      r6.x = xe_dp3(r7.xyz, r7.xyz);
      { precise float v = lit255.x; precise float s = sqrt(abs(r6.x)); r6.z = v; r6.y = s; }
      p = !(r1.x == 0.0);
      { precise float v = xe_dp3(r6.xyz, xe_fc[loop_aL.x + 16].xyz); precise float s = inversesqrt(abs(r6.x)); r0.w = v; r1.x = s; }
      { precise vec3 v = xe_mul(r7.xyz, r1.xxx); precise float s = 1.0 / r0.w; r1.xzw = v; r2.z = s; }
      if (!p) {
        pc = 18; continue;
      }
      r0.w = xe_dp3(r1.xzw, xe_fc[loop_aL.x + 24].xyz);
      r2.w = (r0.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r2.w == 0.0;
      if (p) {
        r0.w = r0.w + -xe_fc[loop_aL.x + 32].y;
        r0.w = xe_mul(r0.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        r0.w = lit255.x;
      }
      p = r0.w > 0.0;
      if (p) {
        r6.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r1.z = xe_dp3(r4.xyz, r1.xzw);
        r1.z = xe_max(r1.z, lit255.y);
        r6.xyz = xe_mul(r6.xyz, r1.zzz);
        r6.xyz = xe_mul(r6.yxz, r0.www);
        r5.xyz = xe_mul(r6.yxz, r2.zzz) + r5.xyz;
      }
    case 13:
      r0.w = lit255.x + r1.y;
      r1.x = (lit255.z > r0.w ? 1.0 : 0.0);
      // end of loop 30
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(30u)), 16, 8);
          pc = 6; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 15:
      r4 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r0.xyz = AmbientColor.xyz + MatAmbientDiffuse.xyz;
      { precise vec3 v = xe_mul(r4.xyz, r3.xyz); precise float s = r4.w; r1.xyz = v; r3.y = s; }
      { precise vec3 v = r0.xyz + r5.xyz; precise float s = xe_mul(ModTexAlpha.x, r3.y); r0.xyz = v; r3.x = s; }
      { precise vec3 e = xe_mul(r1.xyz, r0.xyz); xe_color0.xyz = e; }
      { precise float e = clamp(xe_mul(r3.x, r3.w), 0.0, 1.0); xe_color0.w = e; }
      break;  // the end
    case 18:
      r6.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r4.xyz, r1.xzw);
      r0.w = xe_max(r0.w, lit255.y);
      r6.xyz = xe_mul(r6.xyz, r0.www);
      r5.xyz = xe_mul(r6.xyz, r2.zzz) + r5.xyz;
      pc = 13; continue;
    }
    break;
  }
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
