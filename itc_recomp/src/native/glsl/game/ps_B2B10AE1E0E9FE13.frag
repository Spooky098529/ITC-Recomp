#version 450
// Pixel shader B2B10AE1E0E9FE13 (ps_05), first found in 00000587.part at 0x44EC0
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 6, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 6, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 6

// UseProjTex: bool 0
// spheretx0: sampler 6
#define matView_0 xe_fc[0]  // c0
#define matView_1 xe_fc[1]  // c1
#define matView_2 xe_fc[2]  // c2
#define EyePos xe_fc[4]  // c4
#define AmbientColor xe_fc[40]  // c40
#define MatParam xe_fc[64]  // c64
#define MatAmbientDiffuse xe_fc[65]  // c65
#define MatSpecularColor xe_fc[66]  // c66
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
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r7.xyz = vec3(greaterThan(-abs(r2.xxx), lit255.yyy));
      { precise vec3 v = -r0.xyz + EyePos.xyz; precise float s = r2.w; ps = s; r3.xyz = v; r1.w = clamp(s, 0.0, 1.0); }
      { precise float v = xe_dp3(r1.xyz, r1.xyz); precise float s = lit255.x; ps = s; r4.x = v; r0.w = s; }
      { precise float v = xe_dp3(r3.xyz, r3.xyz); precise float s = inversesqrt(abs(r4.x)); ps = s; r3.w = v; r4.x = s; }
      { precise vec3 v = xe_mul(r4.xxx, r1.xyz); precise float s = inversesqrt(abs(r3.w)); ps = s; r5.xyz = v; r1.x = s; }
      { precise vec3 v = xe_mul(r3.xyz, r1.xxx); precise float s = lit255.y; ps = s; r1.xyz = v; r3.x = s; }
      { precise float v = xe_dp3(-r1.xyz, r5.xyz); precise float s = lit255.y; ps = s; r3.z = v; r3.y = s; }
      { precise float v = r3.z + r3.z; precise float s = lit255.y; ps = s; r3.w = v; r3.z = s; }
      r6.xyz = xe_mul(-r5.xyz, r3.www) + -r1.xyz;
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 7; continue; }
    case 3:
      r4.xyz = xe_mul(xe_fc[loop_aL.x + 41].xyz, MatAmbientDiffuse.www);
      { precise float v = xe_dp3(r5.xyz, xe_fc[loop_aL.x + 5].xyz); precise float s = (-abs(r2.x) > 0.0 ? 1.0 : 0.0); ps = s; r1.x = v; r2.w = s; }
      { precise float v = xe_max(r1.x, lit255.y); p = !(r0.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r1.x = v; }
      r7.xyz = xe_mul(r4.xyz, r1.xxx) + r7.xyz;
      if (p) {
        r0.w = xe_dp3(r6.xyz, xe_fc[loop_aL.x + 5].xyz);
        r0.w = xe_max(r0.w, lit255.w);
        ps = xe_log_clamped(abs(r0.w));
        r0.w = ps;
        ps = xe_mul(MatParam.x, r0.w);
        r0.w = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 44].xyz, MatSpecularColor.xyz); precise float s = exp2(r0.w); ps = s; r1.xyz = v; r0.w = s; }
        r3.xyz = xe_mul(r1.xyz, r0.www) + r3.xyz;
      }
      ps = lit255.x + r2.w;
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
      r8.xyz = xe_fc[loop_aL.x + 8].xyz + -r0.xyz;
      r4.x = xe_dp3(r8.xyz, r8.xyz);
      { precise float v = lit255.x; precise float s = sqrt(abs(r4.x)); ps = s; r4.z = v; r4.y = s; }
      p = r1.x == 0.0;
      ps = (p ? 0.0 : 1.0);
      { precise float v = xe_dp3(r4.xyz, xe_fc[loop_aL.x + 16].xyz); precise float s = inversesqrt(abs(r4.x)); ps = s; r0.w = v; r1.z = s; }
      { precise vec3 v = xe_mul(r8.xyz, r1.zzz); precise float s = 1.0 / r0.w; ps = s; r4.xyz = v; r1.z = s; }
      if (!p) {
        pc = 20; continue;
      }
      r8.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r5.xyz, r4.xyz);
      r0.w = xe_max(r0.w, lit255.y);
      r4.xyz = xe_mul(r8.xyz, r0.www);
      r7.xyz = xe_mul(r4.xyz, r1.zzz) + r7.xyz;
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
      r1.xyz = xe_mul(r6.zzz, matView_2.xyz);
      r0.xyz = xe_mul(r6.yyy, matView_1.xzy);
      r4.xyz = xe_mul(r6.xxx, matView_0.yzx) + r0.zyx;
      r0.xy = -r4.zx + -r1.xy;
      r2.w = r4.y + r1.z;
      ps = lit255.x - r2.w;
      r0.z = ps;
      r2.w = xe_dp2add(r0.xy, r0.xy, lit255.y);
      r2.w = xe_mul(r0.z, r0.z) + r2.w;
      ps = sqrt(abs(r2.w));
      r2.w = ps;
      ps = r2.w + r2.w;
      r2.w = ps;
      ps = 1.0 / r2.w;
      r2.w = ps;
      r0.xy = xe_mul(r0.xy, r2.ww) + lit254.yy;
      r0.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 6u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
      r1.xyz = AmbientColor.xyz + MatAmbientDiffuse.xyz;
      r1.xyz = r1.xyz + r7.xyz;
      r2.xyz = xe_mul(r1.xyz, r2.xyz) + r3.xyz;
      r1.xyz = xe_mul(r0.xyz, MatParam.yyy) + r2.xyz;
      if (xe_bool(128u /* UseProjTex */)) {
        r1.xyz = xe_max(r1.xyz, lit255.yyy);
      }
      { precise vec4 e = r1; xe_color0 = e; }
      break;  // the end
    case 20:
      r2.w = xe_dp3(r4.xyz, xe_fc[loop_aL.x + 24].xyz);
      r0.w = (r2.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r0.w == 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r2.w = r2.w + -xe_fc[loop_aL.x + 32].y;
        r2.w = xe_mul(r2.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        ps = lit255.x;
        r2.w = ps;
      }
      { vec4 pa = lit255.yyyy, pb = r2.wwww; p = pa.w == 0.0 && pb.w > 0.0; r3.w = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r8.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r0.w = xe_dp3(r5.xyz, r4.xyz);
        r0.w = xe_max(r0.w, lit255.y);
        { precise vec3 v = xe_mul(r8.xyz, r0.www); precise float s = r1.z; ps = s; r8.xyz = v; }
        { precise vec3 v = xe_mul(r8.yxz, r2.www); precise float s = xe_mul(r2.w, ps); ps = s; r8.xyz = v; r0.w = s; }
      }
      if (p) {
        r7.xyz = xe_mul(r8.yxz, r1.zzz) + r7.xyz;
      }
      { vec4 pa = r3.wwww, pb = r1.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r3.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r2.w = xe_dp3(r6.xyz, r4.xyz);
        r2.w = xe_max(r2.w, lit255.w);
        ps = xe_log_clamped(abs(r2.w));
        r2.w = ps;
        ps = xe_mul(MatParam.x, r2.w);
        r1.z = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 55].xyz, MatSpecularColor.xyz); precise float s = exp2(r1.z); ps = s; r4.xyz = v; r1.z = s; }
        r4.xyz = xe_mul(r4.xyz, r1.zzz);
      }
      if (p) {
        r3.xyz = xe_mul(r4.xyz, r0.www) + r3.xyz;
      }
      { float pop = r3.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r3.w = ps; }
      { float pop = r3.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r3.w = ps; }
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
