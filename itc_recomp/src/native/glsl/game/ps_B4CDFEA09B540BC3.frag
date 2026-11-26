#version 450
// Pixel shader B4CDFEA09B540BC3 (ps_18), first found in 00002618.part at 0x8300
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// UseProjTex: bool 0
// btx0: sampler 0
#define EyePos xe_fc[4]  // c4
#define AmbientColor xe_fc[40]  // c40
#define MatParam xe_fc[64]  // c64
#define MatAmbientDiffuse xe_fc[65]  // c65
#define MatSpecularColor xe_fc[66]  // c66
const vec4 lit254 = vec4(-1.0, 3.0, 0.0, 0.0);
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
      r5.xyz = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
      r6.xyz = xe_mul(r2.zxy, r1.yzx);
      r3.xyz = -r0.xyz + EyePos.xyz;
      { precise float v = xe_dp3(r3.xyz, r3.xyz); precise float s = r4.w; ps = s; r3.w = v; r1.w = clamp(s, 0.0, 1.0); }
      { precise vec3 v = r5.yxz + lit254.xxx; precise float s = (-abs(r4.x) > 0.0 ? 1.0 : 0.0); ps = s; r8.xyz = v; r7.z = s; }
      r6.xyz = xe_mul(r2.yzx, r1.zxy) + -r6.xyz;
      { precise vec3 v = xe_mul(r6.xyz, r2.www); precise float s = (-abs(r4.x) > 0.0 ? 1.0 : 0.0); ps = s; r6.xyz = v; r7.x = s; }
      { precise vec3 v = r8.yxz + r5.xyz; precise float s = (-abs(r4.x) > 0.0 ? 1.0 : 0.0); ps = s; r5.xyz = v; r7.y = s; }
      { precise vec3 v = xe_mul(r6.zxy, r5.yyy); precise float s = lit255.x; ps = s; r6.xyz = v; r4.w = s; }
      r2.xyz = xe_mul(r5.xxx, r2.yxz) + r6.zyx;
      r2.xyz = xe_mul(r5.zzz, r1.yxz) + r2.xyz;
      { precise float v = xe_dp3(r2.yxz, r2.yxz); precise float s = inversesqrt(abs(r3.w)); ps = s; r0.w = v; r1.x = s; }
      { precise vec3 v = xe_mul(r3.xyz, r1.xxx); precise float s = inversesqrt(abs(r0.w)); ps = s; r1.xyz = v; r0.w = s; }
      { precise vec3 v = xe_mul(r2.yxz, r0.www); precise float s = lit255.y; ps = s; r2.xyz = v; r5.x = s; }
      { precise float v = xe_dp3(-r1.xyz, r2.xyz); precise float s = lit255.y; ps = s; r0.w = v; r5.y = s; }
      { precise float v = r0.w + r0.w; precise float s = lit255.y; ps = s; r0.w = v; r5.z = s; }
      r3.xyz = xe_mul(-r2.xyz, r0.www) + -r1.xyz;
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 8; continue; }
    case 4:
      r6.xyz = xe_mul(xe_fc[loop_aL.x + 41].xyz, MatAmbientDiffuse.www);
      { precise float v = xe_dp3(r2.xyz, xe_fc[loop_aL.x + 5].xyz); precise float s = (-abs(r4.x) > 0.0 ? 1.0 : 0.0); ps = s; r1.x = v; r0.w = s; }
      { precise float v = xe_max(r1.x, lit255.y); p = !(r4.w == 0.0); precise float s = (p ? 0.0 : 1.0); ps = s; r1.x = v; }
      r7.xyz = xe_mul(r6.xyz, r1.xxx) + r7.xyz;
      if (p) {
        r4.w = xe_dp3(r3.xyz, xe_fc[loop_aL.x + 5].xyz);
        r4.w = xe_max(r4.w, lit255.w);
        ps = xe_log_clamped(abs(r4.w));
        r4.w = ps;
        ps = xe_mul(MatParam.x, r4.w);
        r1.x = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 44].xyz, MatSpecularColor.xyz); precise float s = exp2(r1.x); ps = s; r6.xyz = v; r1.x = s; }
        r5.xyz = xe_mul(r6.xyz, r1.xxx) + r5.xyz;
      }
      ps = lit255.x + r0.w;
      r4.w = ps;
      r4.w = (lit254.y > r4.w ? 1.0 : 0.0);
      // end of loop 31
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(31u)), 16, 8);
          pc = 4; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 8:
      r1.xy = lit255.xy;
      // loop 30
      loop_count = uvec4(xe_loop_constant(30u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(30u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 15; continue; }
    case 10:
      r8.xyz = xe_fc[loop_aL.x + 8].xyz + -r0.xyz;
      r6.x = xe_dp3(r8.xyz, r8.xyz);
      { precise float v = lit255.x; precise float s = sqrt(abs(r6.x)); ps = s; r6.z = v; r6.y = s; }
      p = r1.x == 0.0;
      ps = (p ? 0.0 : 1.0);
      { precise float v = xe_dp3(r6.xyz, xe_fc[loop_aL.x + 16].xyz); precise float s = inversesqrt(abs(r6.x)); ps = s; r0.w = v; r1.z = s; }
      { precise vec3 v = xe_mul(r8.xyz, r1.zzz); precise float s = 1.0 / r0.w; ps = s; r6.xyz = v; r1.z = s; }
      if (!p) {
        pc = 19; continue;
      }
      r8.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r2.xyz, r6.xyz);
      r0.w = xe_max(r0.w, lit255.y);
      r6.xyz = xe_mul(r8.xyz, r0.www);
      r7.xyz = xe_mul(r6.xyz, r1.zzz) + r7.xyz;
    case 13:
      ps = lit255.x + r1.y;
      r0.w = ps;
      r1.x = (lit255.z > r0.w ? 1.0 : 0.0);
      // end of loop 30
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(30u)), 16, 8);
          pc = 10; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 15:
      r0.xyz = AmbientColor.xyz + MatAmbientDiffuse.xyz;
      r0.xyz = r0.xyz + r7.xyz;
      r1.xyz = xe_mul(r0.xyz, r4.xyz) + r5.xyz;
      if (xe_bool(128u /* UseProjTex */)) {
        r1.xyz = xe_max(r1.xyz, lit255.yyy);
      }
      { precise vec4 e = r1; xe_color0 = e; }
      break;  // the end
    case 19:
      r4.w = xe_dp3(r6.xyz, xe_fc[loop_aL.x + 24].xyz);
      r0.w = (r4.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r0.w == 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r4.w = r4.w + -xe_fc[loop_aL.x + 32].y;
        r4.w = xe_mul(r4.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        ps = lit255.x;
        r4.w = ps;
      }
      { vec4 pa = lit255.yyyy, pb = r4.wwww; p = pa.w == 0.0 && pb.w > 0.0; r2.w = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r8.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r0.w = xe_dp3(r2.xyz, r6.xyz);
        r0.w = xe_max(r0.w, lit255.y);
        { precise vec3 v = xe_mul(r8.xyz, r0.www); precise float s = r1.z; ps = s; r8.xyz = v; }
        { precise vec3 v = xe_mul(r8.yxz, r4.www); precise float s = xe_mul(r4.w, ps); ps = s; r8.xyz = v; r0.w = s; }
      }
      if (p) {
        r7.xyz = xe_mul(r8.yxz, r1.zzz) + r7.xyz;
      }
      { vec4 pa = r2.wwww, pb = r1.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r2.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r4.w = xe_dp3(r3.xyz, r6.xyz);
        r4.w = xe_max(r4.w, lit255.w);
        ps = xe_log_clamped(abs(r4.w));
        r4.w = ps;
        ps = xe_mul(MatParam.x, r4.w);
        r1.z = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 55].xyz, MatSpecularColor.xyz); precise float s = exp2(r1.z); ps = s; r6.xyz = v; r1.z = s; }
        r6.xyz = xe_mul(r6.xyz, r1.zzz);
      }
      if (p) {
        r5.xyz = xe_mul(r6.xyz, r0.www) + r5.xyz;
      }
      { float pop = r2.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.w = ps; }
      { float pop = r2.w - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.w = ps; }
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
