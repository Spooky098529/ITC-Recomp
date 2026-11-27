#version 450
// Pixel shader 334EC6D5C29CDC42, first found in 0A701B48C97038D5.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 256
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 1
layout(set = 3, binding = 1) uniform texture2DArray xe_texture2;  // fetch constant 0
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 1
layout(set = 3, binding = 3) uniform sampler xe_sampler1;  // fetch constant 0

// UseProjTex: bool 0
// btx0: sampler 1
// tx0: sampler 0
#define EyePos xe_fc[4]  // c4
#define AmbientColor xe_fc[40]  // c40
#define MatParam xe_fc[64]  // c64
#define MatAmbientDiffuse xe_fc[65]  // c65
#define MatSpecularColor xe_fc[66]  // c66
#define ModTexAlpha xe_fc[67]  // c67
#define fogColor xe_fc[76]  // c76
const vec4 lit254 = vec4(0.0, 1.0, -1.0, 2.0);
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
      r4.xyz = xe_tfetch2d(xe_texture0, xe_sampler0, 1u, r4.xy, vec2(0.0014648438, 0.0014648438), 0.0, true).xyz;
      r9 = xe_mul(r2.yzxy, r1.zxyx);
      r6.yzw = -r0.xyz + EyePos.xyz;
      { precise float v = xe_dp3(r6.wyz, r6.wyz); precise float s = (-abs(r5.x) > 0.0 ? 1.0 : 0.0); ps = s; r3.z = v; r8.y = s; }
      r4.xyz = xe_mul(r4.xzy, lit254.www) + lit254.zzz;
      { precise float v = r9.z + -r9.w; precise float s = (-abs(r5.x) > 0.0 ? 1.0 : 0.0); ps = s; r7.z = v; r8.z = s; }
      r7.xy = xe_mul(-r2.zx, r1.yz) + r9.xy;
      { precise vec3 v = xe_mul(r7.yxz, r2.www); precise float s = (-abs(r5.x) > 0.0 ? 1.0 : 0.0); ps = s; r7.xyz = v; r8.x = s; }
      { precise vec3 v = xe_mul(r7.zyx, r4.zzz); precise float s = lit254.x; ps = s; r7.xyz = v; r6.x = s; }
      r2.xyz = xe_mul(r4.xxx, r2.yxz) + r7.zyx;
      r4.xyz = xe_mul(r4.yyy, r1.zxy) + r2.zyx;
      { precise float v = xe_dp3(r4.xyz, r4.xyz); precise float s = inversesqrt(abs(r3.z)); ps = s; r0.w = v; r2.x = s; }
      { precise vec3 v = xe_mul(r6.wyz, r2.xxx); precise float s = inversesqrt(abs(r0.w)); ps = s; r2.xyz = v; r0.w = s; }
      { precise vec3 v = xe_mul(r4.xzy, r0.www); precise float s = lit254.x; ps = s; r7.xyz = v; r6.z = s; }
      { precise float v = xe_dp3(-r2.xyz, r7.xzy); precise float s = lit254.x; ps = s; r0.w = v; r6.y = s; }
      { precise float v = r0.w + r0.w; precise float s = lit254.y; ps = s; r0.w = v; r2.w = s; }
      r2.xyz = xe_mul(-r7.xyz, r0.www) + -r2.xzy;
      // loop 31
      loop_count = uvec4(xe_loop_constant(31u) & 0xFFu, loop_count.xyz);
      loop_aL = ivec4(int((xe_loop_constant(31u) >> 8u) & 0xFFu), loop_aL.xyz);
      if (loop_count.x == 0u) { pc = 19; continue; }
    case 4:
      { precise vec3 v = -r0.xzy + xe_fc[loop_aL.x + 8].xzy; precise float s = (-abs(r5.x) > 0.0 ? 1.0 : 0.0); ps = s; r4.xyz = v; r1.x = s; }
      { precise float v = xe_dp3(r4.yxz, r4.yxz); p = r2.w == 0.0; precise float s = (p ? 0.0 : 1.0); ps = s; r9.y = v; }
      { precise float v = lit254.y; precise float s = sqrt(abs(r9.y)); ps = s; r9.z = v; r9.x = s; }
      { precise float v = xe_dp3(xe_fc[loop_aL.x + 16].xyz, r9.yxz); precise float s = inversesqrt(abs(r9.y)); ps = s; r0.w = v; r1.y = s; }
      { precise vec3 v = xe_mul(r4.yzx, r1.yyy); precise float s = 1.0 / r0.w; ps = s; r4.xyz = v; r1.y = s; }
      if (!p) {
        pc = 8; continue;
      }
      r9.xyz = xe_mul(xe_fc[loop_aL.x + 47].xzy, MatAmbientDiffuse.www);
      r0.w = xe_dp3(r7.xzy, r4.xzy);
      r0.w = xe_max(r0.w, lit254.x);
      r4.xyz = xe_mul(r9.zxy, r0.www);
      r6.xyz = xe_mul(r4.yzx, r1.yyy) + r6.xyz;
      pc = 17; continue;
    case 8:
      r0.w = xe_dp3(r4.xzy, xe_fc[loop_aL.x + 24].zxy);
      r3.z = (r0.w > xe_fc[loop_aL.x + 32].x ? 1.0 : 0.0);
      p = r3.z == 0.0;
      ps = (p ? 0.0 : 1.0);
      if (p) {
        r0.w = r0.w + -xe_fc[loop_aL.x + 32].y;
        r0.w = xe_mul(r0.w, xe_fc[loop_aL.x + 32].z);
      }
      if (!p) {
        ps = lit254.y;
        r0.w = ps;
      }
      { vec4 pa = lit254.xxxx, pb = r0.wwww; p = pa.w == 0.0 && pb.w > 0.0; r1.z = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r9.xyz = xe_mul(xe_fc[loop_aL.x + 47].xyz, MatAmbientDiffuse.www);
        r3.z = xe_dp3(r7.xzy, r4.xzy);
        { precise float v = xe_max(r3.z, lit254.x); precise float s = r1.y; ps = s; r3.z = v; }
        { precise vec3 v = xe_mul(r9.zxy, r3.zzz); precise float s = xe_mul(r0.w, ps); ps = s; r9.xyz = v; r1.y = s; }
        r6.xyz = xe_mul(r9.yxz, r1.yyy) + r6.xyz;
      }
      { vec4 pa = r1.zzzz, pb = r2.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r1.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.w = xe_dp3(r4.xzy, r2.xzy);
        r0.w = xe_max(r0.w, lit255.x);
        ps = log2(abs(r0.w));
        r0.w = ps;
        ps = xe_mul(MatParam.x, r0.w);
        r0.w = ps;
        { precise vec3 v = xe_mul(xe_fc[loop_aL.x + 55].xyz, MatSpecularColor.xyz); precise float s = exp2(r0.w); ps = s; r4.xyz = v; r0.w = s; }
        r4.xyz = xe_mul(r4.zxy, r0.www);
      }
      if (p) {
        r8.xyz = xe_mul(r4.xyz, r1.yyy) + r8.xyz;
      }
      { float pop = r1.z - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r1.z = ps; }
      { float pop = r1.z - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r1.z = ps; }
    case 17:
      ps = lit254.y + r1.x;
      r0.w = ps;
      r2.w = (lit254.w > r0.w ? 1.0 : 0.0);
      // end of loop 31
      { uint count = loop_count.x - 1u;
        if (count != 0u) {
          loop_count.x = count;
          loop_aL.x += bitfieldExtract(int(xe_loop_constant(31u)), 16, 8);
          pc = 4; continue;
        }
        loop_count = uvec4(loop_count.yzw, 0u); loop_aL = ivec4(loop_aL.yzw, 0); }
    case 19:
      r0 = xe_tfetch2d(xe_texture2, xe_sampler1, 0u, r3.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r2.xyz = AmbientColor.zyx + MatAmbientDiffuse.zyx;
      r2.xyz = r2.zxy + r6.xyz;
      r2.w = xe_mul(r0.w, ModTexAlpha.x);
      r5.w = clamp(xe_mul(r2.w, r5.w), 0.0, 1.0);
      r5.xyz = xe_mul(r2.yxz, r5.zxy);
      r5.xyz = xe_mul(r5.xzy, r0.zyx) + r8.xzy;
      if (!xe_bool(128u /* UseProjTex */)) {
        pc = 23; continue;
      }
      r5.xyz = xe_max(r5.xyz, lit254.xxx);
    case 23:
      r0.x = xe_max(r1.w, fogColor.w);
      r0.x = xe_min(r0.x, lit254.y);
      r5.xyz = r5.xyz + -fogColor.zyx;
      r5.xyz = xe_mul(r5.zyx, r0.xxx) + fogColor.xyz;
      { precise vec4 e = r5; xe_color0 = e; }
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
