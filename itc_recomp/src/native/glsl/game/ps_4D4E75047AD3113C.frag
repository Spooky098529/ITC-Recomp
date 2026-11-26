#version 450
// Pixel shader 4D4E75047AD3113C (ps_10), first found in 00000888.part at 0x1A500
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

#define AmbientColor xe_fc[40]  // c40
#define MatAmbientDiffuse xe_fc[65]  // c65
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
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r5.xyz = vec3(greaterThan(-abs(r2.xxx), lit255.yyy));
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 4; continue; }
    case 2:
      r3.xyz = xe_mul(xe_fc[loop_aL.x + 41].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r1.xyz, xe_fc[loop_aL.x + 5].xyz);
      r0.w = xe_max(r0.w, lit255.y);
      r5.xyz = xe_mul(r3.xyz, r0.www) + r5.xyz;
      // end of loop 31
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(31u)), 16, 8);
          pc = 2; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 4:
      r3.xy = lit255.xy;
      // loop 30
      loop_count = uvec4(xe_loop_constant(30u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(30u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 11; continue; }
    case 6:
      r6.xyz = xe_fc[loop_aL.x + 8].xyz + -r0.xyz;
      r4.x = xe_dp3(r6.xyz, r6.xyz);
      { precise float v = lit255.x; precise float s = sqrt(abs(r4.x)); r4.z = v; r4.y = s; }
      p = r3.x == 0.0;
      { precise float v = xe_dp3(r4.xyz, xe_fc[loop_aL.x + 16].xyz); precise float s = inversesqrt(abs(r4.x)); r0.w = v; r1.w = s; }
      { precise vec3 v = xe_mul(r6.xyz, r1.www); precise float s = 1.0 / r0.w; r3.xzw = v; r1.w = s; }
      if (!p) {
        pc = 13; continue;
      }
      r4.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r1.xyz, r3.xzw);
      r0.w = xe_max(r0.w, lit255.y);
      r4.xyz = xe_mul(r4.xyz, r0.www);
      r5.xyz = xe_mul(r4.xyz, r1.www) + r5.xyz;
    case 9:
      r0.w = lit255.x + r3.y;
      r3.x = (lit255.z > r0.w ? 1.0 : 0.0);
      // end of loop 30
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(30u)), 16, 8);
          pc = 6; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 11:
      r0.xyz = AmbientColor.xyz + MatAmbientDiffuse.xyz;
      r0.xyz = r0.xyz + r5.xyz;
      r2.xyz = xe_mul(r0.xyz, r2.xyz);
      { precise vec4 e = r2; xe_color0 = e; }
      break;  // the end
    case 13:
      r0.w = xe_dp3(r3.xzw, xe_fc[loop_aL.x + 24].xyz);
      r4.x = (r0.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r4.x == 0.0;
      if (p) {
        r0.w = r0.w + -xe_fc[loop_aL.x + 32].y;
        r0.w = xe_mul(r0.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        r0.w = lit255.x;
      }
      p = r0.w > 0.0;
      if (p) {
        r4.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r3.z = xe_dp3(r1.xyz, r3.xzw);
        r3.z = xe_max(r3.z, lit255.y);
        r4.xyz = xe_mul(r4.xyz, r3.zzz);
        r4.xyz = xe_mul(r4.yxz, r0.www);
        r5.xyz = xe_mul(r4.yxz, r1.www) + r5.xyz;
      }
      pc = 9; continue;
    }
    break;
  }
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
