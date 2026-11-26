#version 450
// Pixel shader 9C3EB0925F0E75E8 (ps_00), first found in 00000107.part at 0x1C60
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
const vec4 lit255 = vec4(0.0, 1.0, 4.0, 0.0);

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
  precise float ps = 0.0;
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r0.w = xe_dp3(r1.zxy, r1.zxy);
      { precise vec3 v = vec3(greaterThan(-abs(r3.xxx), lit255.xxx)); precise float s = inversesqrt(abs(r0.w)); ps = s; r4.xyz = v; r0.w = s; }
      r6.xyz = xe_mul(r0.www, r1.xzy);
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 4; continue; }
    case 2:
      r1.xyz = xe_mul(xe_fc[loop_aL.x + 41].yzx, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r6.yxz, xe_fc[loop_aL.x + 5].zxy);
      r0.w = xe_max(r0.w, lit255.x);
      r4.xyz = xe_mul(r1.zyx, r0.www) + r4.xyz;
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
      if (loop_count.x == 0u) { pc = 17; continue; }
    case 6:
      r5.xyz = -r0.xzy + xe_fc[loop_aL.x + 8].xzy;
      { precise float v = xe_dp3(r5.yxz, r5.yxz); p = !(r1.y == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r7.y = v; }
      { precise float v = lit255.y; precise float s = sqrt(abs(r7.y)); ps = s; r7.z = v; r7.x = s; }
      { precise float v = xe_dp3(xe_fc[loop_aL.x + 16].xyz, r7.yxz); precise float s = inversesqrt(abs(r7.y)); ps = s; r0.w = v; r1.z = s; }
      { precise vec3 v = xe_mul(r5.yzx, r1.zzz); precise float s = 1.0 / r0.w; ps = s; r5.xyz = v; r6.w = s; }
      if (!p) {
        pc = 14; continue;
      }
      r0.w = xe_dp3(r5.xzy, xe_fc[loop_aL.x + 24].zxy);
      r1.z = (r0.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r1.z == 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r0.w = r0.w + -xe_fc[loop_aL.x + 32].y;
        r5.w = xe_mul(r0.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        ps = lit255.y;
        r5.w = ps;
      }
      p = r5.w > 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r7.xyz = xe_mul(xe_fc[loop_aL.x + 47].xzy, MatAmbientDiffuse.www);
        r0.w = xe_dp3(r6.yxz, r5.xzy);
        { precise float v = xe_max(r0.w, lit255.x); precise float s = r6.w; ps = s; r0.w = v; }
        { precise vec3 v = xe_mul(r7.zxy, r0.www); precise float s = xe_mul(r5.w, ps); ps = s; r7.xyz = v; r0.w = s; }
        r4.xyz = xe_mul(r7.yzx, r0.www) + r4.xyz;
      }
      pc = 15; continue;
    case 14:
      r7.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r6.yxz, r5.xzy);
      r0.w = xe_max(r0.w, lit255.x);
      r5.xyz = xe_mul(r7.zxy, r0.www);
      r4.xyz = xe_mul(r5.yxz, r6.www) + r4.xyz;
    case 15:
      ps = lit255.y + r1.x;
      r0.w = ps;
      r1.y = (lit255.z > r0.w ? 1.0 : 0.0);
      // end of loop 30
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(30u)), 16, 8);
          pc = 6; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 17:
      r0 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r1.xyz = AmbientColor.zyx + MatAmbientDiffuse.zyx;
      r1.yzw = r1.zxy + r4.xyz;
      r1.x = xe_mul(r0.w, ModTexAlpha.x);
      { precise vec3 v = xe_mul(r1.zyw, r3.zxy); precise float s = r1.x; ps = s; r3.xyz = v; }
      { precise vec3 v = xe_mul(r3.yzx, r0.xyz); precise float s = xe_mul(r3.w, ps); ps = s; xe_color0.xyz = v; { precise float e = clamp(s, 0.0, 1.0); xe_color0.w = e; } }
      break;  // the end
    }
    break;
  }
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
