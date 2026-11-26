#version 450
// Pixel shader 76DEE068496E05C1 (ps_02), first found in 00000581.part at 0xADEA0
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 6, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 6, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 0, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 0, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 6
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 0

// UseProjTex: bool 0
// spheretx0: sampler 6
// tx0: sampler 0
#define matView_0 xe_fc[0]  // c0
#define matView_1 xe_fc[1]  // c1
#define matView_2 xe_fc[2]  // c2
#define EyePos xe_fc[4]  // c4
#define AmbientColor xe_fc[40]  // c40
#define MatParam xe_fc[64]  // c64
#define MatAmbientDiffuse xe_fc[65]  // c65
#define MatSpecularColor xe_fc[66]  // c66
#define ModTexAlpha xe_fc[67]  // c67
const vec4 lit254 = vec4(3.0, 0.5, 0.0, 0.0);
const vec4 lit255 = vec4(1.0, 0.0, 4.0, 1e-05);

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
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r6.xyz = vec3(greaterThan(-abs(r3.xxx), lit255.yyy));
      r5.xyz = -r0.xyz + EyePos.xyz;
      { precise float v = xe_dp3(r1.xyz, r1.xyz); precise float s = lit255.x; ps = s; r2.w = v; r0.w = s; }
      { precise float v = xe_dp3(r5.xyz, r5.xyz); precise float s = inversesqrt(abs(r2.w)); ps = s; r2.z = v; r2.w = s; }
      { precise vec3 v = xe_mul(r2.www, r1.xyz); precise float s = inversesqrt(abs(r2.z)); ps = s; r4.xyz = v; r1.x = s; }
      { precise vec3 v = xe_mul(r5.xyz, r1.xxx); precise float s = lit255.y; ps = s; r1.xyz = v; r7.x = s; }
      { precise float v = xe_dp3(-r1.xyz, r4.xyz); precise float s = lit255.y; ps = s; r1.w = v; r7.y = s; }
      { precise float v = r1.w + r1.w; precise float s = lit255.y; ps = s; r1.w = v; r7.z = s; }
      r5.xyz = xe_mul(-r4.xyz, r1.www) + -r1.xyz;
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 7; continue; }
    case 3:
      r8.xyz = xe_mul(xe_fc[loop_aL.x + 41].xyz, MatAmbientDiffuse.www);
      { precise float v = xe_dp3(r4.xyz, xe_fc[loop_aL.x + 5].xyz); precise float s = (-abs(r3.x) > 0.0 ? 1.0 : 0.0); ps = s; r1.x = v; r1.y = s; }
      { precise float v = xe_max(r1.x, lit255.y); p = !(r0.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r1.x = v; }
      r6.xyz = xe_mul(r8.xyz, r1.xxx) + r6.xyz;
      if (p) {
        r0.w = xe_dp3(r5.xyz, xe_fc[loop_aL.x + 5].xyz);
        r0.w = xe_max(r0.w, lit255.w);
        ps = xe_log_clamped(abs(r0.w));
        r0.w = ps;
        ps = xe_mul(MatParam.x, r0.w);
        r0.w = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 44].xyz, MatSpecularColor.xyz); precise float s = exp2(r0.w); ps = s; r1.xzw = v; r0.w = s; }
        r7.xyz = xe_mul(r1.xzw, r0.www) + r7.xyz;
      }
      ps = lit255.x + r1.y;
      r0.w = ps;
      r0.w = (lit254.x > r0.w ? 1.0 : 0.0);
      // end of loop 31
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(31u)), 16, 8);
          pc = 3; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 7:
      r1.xy = lit255.xy;
      // loop 30
      loop_count = uvec4(xe_loop_constant(30u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(30u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 14; continue; }
    case 9:
      r9.xyz = xe_fc[loop_aL.x + 8].xyz + -r0.xyz;
      r8.x = xe_dp3(r9.xyz, r9.xyz);
      { precise float v = lit255.x; precise float s = sqrt(abs(r8.x)); ps = s; r8.z = v; r8.y = s; }
      p = r1.x == 0.0;
      ps = (p ? 0.0 : 1.0);
      { precise float v = xe_dp3(r8.xyz, xe_fc[loop_aL.x + 16].xyz); precise float s = inversesqrt(abs(r8.x)); ps = s; r0.w = v; r1.z = s; }
      { precise vec3 v = xe_mul(r9.xyz, r1.zzz); precise float s = 1.0 / r0.w; ps = s; r8.xyz = v; r1.z = s; }
      if (!p) {
        pc = 21; continue;
      }
      r9.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r4.xyz, r8.xyz);
      r0.w = xe_max(r0.w, lit255.y);
      r8.xyz = xe_mul(r9.xyz, r0.www);
      r6.xyz = xe_mul(r8.xyz, r1.zzz) + r6.xyz;
    case 12:
      ps = lit255.x + r1.y;
      r0.w = ps;
      r1.x = (lit255.z > r0.w ? 1.0 : 0.0);
      // end of loop 30
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(30u)), 16, 8);
          pc = 9; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 14:
      r1.xyz = xe_mul(r5.zzz, matView_2.xyz);
      r0.xyz = xe_mul(r5.yyy, matView_1.xzy);
      r4.xyz = xe_mul(r5.xxx, matView_0.yzx) + r0.zyx;
      r0.zw = -r4.zx + -r1.xy;
      r0.y = r4.y + r1.z;
      ps = lit255.x - r0.y;
      r0.y = ps;
      r0.x = xe_dp2add(r0.zw, r0.zw, lit255.y);
      r0.x = xe_mul(r0.y, r0.y) + r0.x;
      ps = sqrt(abs(r0.x));
      r0.x = ps;
      ps = r0.x + r0.x;
      r0.x = ps;
      ps = 1.0 / r0.x;
      r0.x = ps;
      r0.xz = xe_mul(r0.zw, r0.xx) + lit254.yy;
      r0.xzw = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 6u, r0.xz, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
      r4 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r2.xyz = AmbientColor.xyz + MatAmbientDiffuse.xyz;
      r1.xyz = xe_mul(r4.xyz, r3.xyz);
      r3.xyz = r2.xyz + r6.xyz;
      r0.y = xe_mul(r4.w, ModTexAlpha.x);
      r3.w = clamp(xe_mul(r0.y, r3.w), 0.0, 1.0);
      r3.xyz = xe_mul(r1.xyz, r3.xyz) + r7.xyz;
      r3.xyz = xe_mul(r0.xzw, MatParam.yyy) + r3.xyz;
      if (xe_bool(128u /* UseProjTex */)) {
        r3.xyz = xe_max(r3.xyz, lit255.yyy);
      }
      { precise vec4 e = r3; xe_color0 = e; }
      break;  // the end
    case 21:
      r0.w = xe_dp3(r8.xyz, xe_fc[loop_aL.x + 24].xyz);
      r1.w = (r0.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r1.w == 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r0.w = r0.w + -xe_fc[loop_aL.x + 32].y;
        r0.w = xe_mul(r0.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        ps = lit255.x;
        r0.w = ps;
      }
      { vec4 pa = lit255.yyyy, pb = r0.wwww; p = pa.w == 0.0 && pb.w > 0.0; r1.w = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r9.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r2.z = xe_dp3(r4.xyz, r8.xyz);
        r2.z = xe_max(r2.z, lit255.y);
        { precise vec3 v = xe_mul(r9.xyz, r2.zzz); precise float s = r1.z; ps = s; r9.xyz = v; }
        { precise vec3 v = xe_mul(r9.yxz, r0.www); precise float s = xe_mul(r0.w, ps); ps = s; r9.xyz = v; r2.z = s; }
      }
      if (p) {
        r6.xyz = xe_mul(r9.yxz, r1.zzz) + r6.xyz;
      }
      { vec4 pa = r1.wwww, pb = r1.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r1.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.w = xe_dp3(r5.xyz, r8.xyz);
        r0.w = xe_max(r0.w, lit255.w);
        ps = xe_log_clamped(abs(r0.w));
        r0.w = ps;
        ps = xe_mul(MatParam.x, r0.w);
        r0.w = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 55].xyz, MatSpecularColor.xyz); precise float s = exp2(r0.w); ps = s; r8.xyz = v; r0.w = s; }
        r8.xyz = xe_mul(r8.xyz, r0.www);
      }
      if (p) {
        r7.xyz = xe_mul(r8.xyz, r2.zzz) + r7.xyz;
      }
      { float pop = r1.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r1.w = ps; }
      { float pop = r1.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r1.w = ps; }
      pc = 12; continue;
    }
    break;
  }
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
