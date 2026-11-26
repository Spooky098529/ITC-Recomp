#version 450
// Pixel shader 0E1E9AD0349FAC43 (ps_19), first found in 00002626.part at 0x161E0
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform texture2DArray xe_texture2;  // fetch constant 1, unsigned
layout(set = 3, binding = 3) uniform texture2DArray xe_texture3;  // fetch constant 1, signed
layout(set = 3, binding = 4) uniform sampler xe_sampler0;  // fetch constant 0
layout(set = 3, binding = 5) uniform sampler xe_sampler1;  // fetch constant 1

// UseProjTex: bool 0
// tx0: sampler 0
// tx1: sampler 1
#define EyePos xe_fc[4]  // c4
#define AmbientColor xe_fc[40]  // c40
#define MatParam xe_fc[64]  // c64
#define MatAmbientDiffuse xe_fc[65]  // c65
#define MatSpecularColor xe_fc[66]  // c66
#define ModTexAlpha xe_fc[67]  // c67
const vec4 lit254 = vec4(1.0, 0.0, 4.0, 3.0);
const vec4 lit255 = vec4(1e-05, 0.0, 0.0, 0.0);

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
layout(location = 10) in vec4 xe_in_interpolator_10;
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
  precise vec4 r10 = xe_input(10, xe_in_interpolator_10);
  bool p = false;
  precise float ps = 0.0;
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r6.xyz = vec3(greaterThan(-abs(r4.xxx), lit254.yyy));
      r7.xyz = -r0.xyz + EyePos.xyz;
      { precise float v = xe_dp3(r1.xyz, r1.xyz); precise float s = lit254.x; ps = s; r2.z = v; r1.w = s; }
      { precise float v = xe_dp3(r7.xyz, r7.xyz); precise float s = inversesqrt(abs(r2.z)); ps = s; r0.w = v; r2.z = s; }
      { precise vec3 v = xe_mul(r2.zzz, r1.xyz); precise float s = inversesqrt(abs(r0.w)); ps = s; r5.xyz = v; r0.w = s; }
      { precise vec3 v = xe_mul(r7.xyz, r0.www); precise float s = lit254.y; ps = s; r1.xyz = v; r8.x = s; }
      { precise float v = xe_dp3(-r1.xyz, r5.xyz); precise float s = lit254.y; ps = s; r0.w = v; r8.y = s; }
      { precise float v = r0.w + r0.w; precise float s = lit254.y; ps = s; r0.w = v; r8.z = s; }
      r9.xyz = xe_mul(-r5.xyz, r0.www) + -r1.xyz;
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 7; continue; }
    case 3:
      r7.xyz = xe_mul(xe_fc[loop_aL.x + 41].xyz, MatAmbientDiffuse.www);
      { precise float v = xe_dp3(r5.xyz, xe_fc[loop_aL.x + 5].xyz); precise float s = (-abs(r4.x) > 0.0 ? 1.0 : 0.0); ps = s; r1.x = v; r0.w = s; }
      { precise float v = xe_max(r1.x, lit254.y); p = !(r1.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r1.x = v; }
      r6.xyz = xe_mul(r7.xyz, r1.xxx) + r6.xyz;
      if (p) {
        r1.x = xe_dp3(r9.xyz, xe_fc[loop_aL.x + 5].xyz);
        r1.x = xe_max(r1.x, lit255.x);
        ps = xe_log_clamped(abs(r1.x));
        r1.y = ps;
        ps = xe_mul(MatParam.x, r1.y);
        r1.x = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 44].xyz, MatSpecularColor.xyz); precise float s = exp2(r1.x); ps = s; r1.yzw = v; r1.x = s; }
        r8.xyz = xe_mul(r1.yzw, r1.xxx) + r8.xyz;
      }
      ps = lit254.x + r0.w;
      r0.w = ps;
      r1.w = (lit254.w > r0.w ? 1.0 : 0.0);
      // end of loop 31
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(31u)), 16, 8);
          pc = 3; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 7:
      r1.xy = lit254.xy;
      // loop 30
      loop_count = uvec4(xe_loop_constant(30u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(30u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 14; continue; }
    case 9:
      r10.xyz = xe_fc[loop_aL.x + 8].xyz + -r0.xyz;
      r7.x = xe_dp3(r10.xyz, r10.xyz);
      { precise float v = lit254.x; precise float s = sqrt(abs(r7.x)); ps = s; r7.z = v; r7.y = s; }
      p = r1.x == 0.0;
      ps = (p ? 0.0 : 1.0);
      { precise float v = xe_dp3(r7.xyz, xe_fc[loop_aL.x + 16].xyz); precise float s = inversesqrt(abs(r7.x)); ps = s; r0.w = v; r1.z = s; }
      { precise vec3 v = xe_mul(r10.xyz, r1.zzz); precise float s = 1.0 / r0.w; ps = s; r7.xyz = v; r1.w = s; }
      if (!p) {
        pc = 20; continue;
      }
      r10.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r5.xyz, r7.xyz);
      r0.w = xe_max(r0.w, lit254.y);
      r7.xyz = xe_mul(r10.xyz, r0.www);
      r6.xyz = xe_mul(r7.xyz, r1.www) + r6.xyz;
    case 12:
      ps = lit254.x + r1.y;
      r0.w = ps;
      r1.x = (lit254.z > r0.w ? 1.0 : 0.0);
      // end of loop 30
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(30u)), 16, 8);
          pc = 9; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 14:
      r5 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r2.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r7 = xe_tfetch2d(xe_texture2, xe_texture3, xe_sampler1, 1u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r0.xyz = AmbientColor.xyz + MatAmbientDiffuse.xyz;
      r1.xyz = r0.xyz + r6.xyz;
      r1.w = r4.w;
      r0.y = xe_mul(r7.w, ModTexAlpha.y);
      ps = lit254.x - r0.y;
      r0.x = ps;
      r3 = mix(r5.zzzx, r7.zxxx, equal(lit254.yyxx, vec4(0.0)));
      r0.zw = mix(r5.yy, r7.yy, equal(lit254.yx, vec2(0.0)));
      r2.x = xe_dp2add(r0.yx, r0.zw, lit254.y);
      r2.y = xe_dp2add(r0.yx, r3.xz, lit254.y);
      r2.z = xe_dp2add(r0.yx, r3.yw, lit254.y);
      r4.w = xe_mul(-r5.w, ModTexAlpha.x) + lit254.x;
      r0.w = xe_mul(-r4.w, r0.x) + lit254.x;
      r0.xyz = xe_mul(r2.zxy, r4.xyz);
      r0 = xe_mul(r0, r1);
      r4.xyz = r0.xyz + r8.xyz;
      ps = r0.w;
      r4.w = clamp(ps, 0.0, 1.0);
      if (xe_bool(128u /* UseProjTex */)) {
        r4.xyz = xe_max(r4.xyz, lit254.yyy);
      }
      { precise vec4 e = r4; xe_color0 = e; }
      break;  // the end
    case 20:
      r0.w = xe_dp3(r7.xyz, xe_fc[loop_aL.x + 24].xyz);
      r1.z = (r0.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r1.z == 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r0.w = r0.w + -xe_fc[loop_aL.x + 32].y;
        r0.w = xe_mul(r0.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        ps = lit254.x;
        r0.w = ps;
      }
      { vec4 pa = lit254.yyyy, pb = r0.wwww; p = pa.w == 0.0 && pb.w > 0.0; r2.z = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r10.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r1.z = xe_dp3(r5.xyz, r7.xyz);
        r1.z = xe_max(r1.z, lit254.y);
        { precise vec3 v = xe_mul(r10.xyz, r1.zzz); precise float s = r1.w; ps = s; r10.xyz = v; }
        { precise vec3 v = xe_mul(r10.yxz, r0.www); precise float s = xe_mul(r0.w, ps); ps = s; r10.xyz = v; r1.z = s; }
      }
      if (p) {
        r6.xyz = xe_mul(r10.yxz, r1.www) + r6.xyz;
      }
      { vec4 pa = r2.zzzz, pb = r1.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r2.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.w = xe_dp3(r9.xyz, r7.xyz);
        r0.w = xe_max(r0.w, lit255.x);
        ps = xe_log_clamped(abs(r0.w));
        r0.w = ps;
        ps = xe_mul(MatParam.x, r0.w);
        r0.w = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 55].xyz, MatSpecularColor.xyz); precise float s = exp2(r0.w); ps = s; r7.xyz = v; r0.w = s; }
        r7.xyz = xe_mul(r7.xyz, r0.www);
      }
      if (p) {
        r8.xyz = xe_mul(r7.xyz, r1.zzz) + r8.xyz;
      }
      { float pop = r2.z - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.z = ps; }
      { float pop = r2.z - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.z = ps; }
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
