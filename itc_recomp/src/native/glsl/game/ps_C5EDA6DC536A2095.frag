#version 450
// Pixel shader C5EDA6DC536A2095, first found in 00000066.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 7
#include "xe_interface.glsl"

layout(set = 3, binding = 0) uniform texture2DArray xe_texture0;  // fetch constant 0, unsigned
layout(set = 3, binding = 1) uniform texture2DArray xe_texture1;  // fetch constant 0, signed
layout(set = 3, binding = 2) uniform sampler xe_sampler0;  // fetch constant 0

// PS_CONSTANT_BOOL_USE_GAMMA: bool 0
// PS_CONSTANT_BOOL_USE_HSV_SCALE: bool 1
// tx0: sampler 0
#define PS_CONSTANT_PARAM_GAMMA xe_fc[0]  // c37
#define PS_CONSTANT_PARAM_CARIBRATE xe_fc[1]  // c38
#define PS_CONSTANT_PARAM_HSV_SCALE xe_fc[2]  // c39
const vec4 lit252 = vec4(0.0, 0.33333334, 0.0, 0.0);
const vec4 lit253 = vec4(0.5, 3.0, 2.0, 6.0);
const vec4 lit254 = vec4(-0.5, 4.0, 1.0, 0.6666667);
const vec4 lit255 = vec4(0.114, 0.299, 0.587, 0.16666667);

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
layout(location = 2) in vec4 xe_in_interpolator_2;
layout(location = 3) in vec4 xe_in_interpolator_3;
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
  bool p = false;
  int pc = 0;
  uvec4 loop_count = uvec4(0u);  // the loop counters, innermost first
  ivec4 loop_aL = ivec4(0);  // the loop indices (aL is .x)
  while (true) {
    switch (pc) {
    case 0:
      r0 = xe_tfetch2d(xe_texture0, xe_texture1, xe_sampler0, 0u, r0.xy, vec2(0.0014648438, 0.0014648438), 0.0, true);
      r1 = clamp(r0.ywzx, 0.0, 1.0);
      if (!xe_bool(129u /* PS_CONSTANT_BOOL_USE_HSV_SCALE */)) {
        pc = 35; continue;
      }
      r0.x = xe_min(r1.w, r1.x);
      r0.y = xe_max(r1.w, r1.x);
      r2.x = xe_max(r0.y, r1.z);
      r0.x = xe_min(r0.x, r1.z);
      r0.x = r2.x + -r0.x;
      { vec4 pa = lit252.xxxx, pb = r0.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); precise float v = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; precise float s = (-abs(r0.x) > 0.0 ? 1.0 : 0.0); r2.z = v; r0.y = s; }
      if (p) {
        r2.w = (r1.w == r2.x ? 1.0 : 0.0);
        { precise float v = xe_mul(r0.x, lit253.x); precise float s = 1.0 / r2.x; r0.z = v; r0.w = s; }
        r2.y = xe_mul(r0.x, r0.w);
        { precise vec3 v = r2.xxx + -r1.zxw; precise float s = 1.0 / r0.x; r3.xyz = v; r0.x = s; }
        r3.xyz = xe_mul(r3.yzx, lit255.www) + r0.zzz;
        r0.xzw = xe_mul(r3.xzy, r0.xxx);
      }
      { vec4 pa = r2.zzzz, pb = r2.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r2.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.y = r0.z + -r0.x;
      }
      p = r2.z == 1.0;
      r2.z = (p ? 0.0 : (r2.z == 0.0 ? 1.0 : r2.z));
      if (p) {
        r2.w = (r1.x == r2.x ? 1.0 : 0.0);
      }
      { vec4 pa = r2.zzzz, pb = r2.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r2.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.x = lit252.y + r0.w;
        r0.y = r0.x + -r0.z;
      }
      p = r2.z == 1.0;
      r2.z = (p ? 0.0 : (r2.z == 0.0 ? 1.0 : r2.z));
      if (p) {
        r2.w = (r1.z == r2.x ? 1.0 : 0.0);
      }
      { vec4 pa = r2.zzzz, pb = r2.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r2.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.x = lit254.w + r0.x;
        r0.y = r0.x + -r0.w;
      }
      { float pop = r2.z - 1.0; p = pop <= 0.0; r2.z = (p ? 0.0 : pop); }
      { float pop = r2.z - 1.0; p = pop <= 0.0; r2.z = (p ? 0.0 : pop); }
      { float pop = r2.z - 1.0; p = pop <= 0.0; r2.z = (p ? 0.0 : pop); }
      if (p) {
        r0.x = (-r0.y > 0.0 ? 1.0 : 0.0);
      }
      { vec4 pa = r2.zzzz, pb = abs(r0.xxxx); p = pa.w == 0.0 && pb.w > 0.0; r2.z = ((pa.x == 0.0 && pb.x > 0.0) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.y = lit254.z + r0.y;
      }
      { float pop = r2.z - 1.0; p = pop <= 0.0; r2.z = (p ? 0.0 : pop); }
      if (p) {
        r0.x = (r0.y > lit254.z ? 1.0 : 0.0);
      }
      { vec4 pa = r2.zzzz, pb = r0.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); r2.z = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.y = r0.y + -lit254.z;
      }
      { float pop = r2.z - 1.0; p = pop <= 0.0; r2.z = (p ? 0.0 : pop); }
      p = r2.z == 1.0;
      r2.z = (p ? 0.0 : (r2.z == 0.0 ? 1.0 : r2.z));
      if (p) {
        r2.y = r0.y;
      }
      { float pop = r2.z - 1.0; p = pop <= 0.0; r2.z = (p ? 0.0 : pop); }
      { precise vec2 v = xe_mul(r2.yx, PS_CONSTANT_PARAM_HSV_SCALE.yz); precise float s = PS_CONSTANT_PARAM_HSV_SCALE.x + r0.y; r1.xw = v; r0.x = s; }
      { vec4 pa = lit252.xxxx, pb = r1.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); precise float v = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; precise float s = fract(r0.x); r0.w = v; r0.x = s; }
      if (p) {
        r0.x = xe_mul(lit253.w, r0.x);
      }
      if (p) {
        r0.z = floor(r0.x);
        r0.x = r0.x + -r0.z;
        r0.y = lit254.z - r0.x;
        r2.xy = xe_mul(-r1.xx, r0.xy) + lit254.zz;
        { precise float v = xe_mul(r1.w, r2.x); precise float s = lit254.z - r1.x; r0.x = v; r2.z = s; }
        r1.xz = xe_mul(r1.ww, r2.yz);
      }
      { vec4 pa = r0.wwww, pb = r0.zzzz; p = pa.w == 0.0 && pb.w == 0.0; r0.w = ((pa.x == 0.0 && pb.x == 0.0) ? -1.0 : pa.x) + 1.0; }
      p = r0.w == 1.0;
      r0.w = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      if (p) {
        r0.y = (r0.z == lit254.z ? 1.0 : 0.0);
      }
      { vec4 pa = r0.wwww, pb = r0.yyyy; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r1.x = r1.w;
      }
      p = r0.w == 1.0;
      r0.w = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      if (p) {
        r0.y = (r0.z == lit253.z ? 1.0 : 0.0);
      }
      { vec4 pa = r0.wwww, pb = r0.yyyy; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r1.x = r1.x;
      }
      p = r0.w == 1.0;
      r0.w = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      if (p) {
        r0.y = (r0.z == lit253.y ? 1.0 : 0.0);
      }
      { vec4 pa = r0.wwww, pb = r0.yyyy; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        { precise vec2 v = r1.zw; precise float s = r0.x; r1.xw = v; r1.z = s; }
      }
      p = r0.w == 1.0;
      r0.w = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      if (p) {
        r0.y = (r0.z == lit254.y ? 1.0 : 0.0);
      }
      { vec4 pa = r0.wwww, pb = r0.yyyy; p = pa.w == 0.0 && !(pb.w == 0.0); r0.w = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
      if (p) {
        r0.x = r1.w;
      }
      p = r0.w == 1.0;
      r0.w = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      if (p) {
        r1.x = r1.w;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; r0.w = (p ? 0.0 : pop); }
      if (p) {
        r1.w = r0.x;
      }
      if (p) {
        r1.z = r1.z;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; r0.w = (p ? 0.0 : pop); }
      if (p) {
        r1.xzw = r1.wxz;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; r0.w = (p ? 0.0 : pop); }
      if (p) {
        r0.x = r1.z;
        { precise vec2 v = r1.wx; precise float s = r0.x; r1.xz = v; r0.x = s; }
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; r0.w = (p ? 0.0 : pop); }
      if (p) {
        r1.w = r0.x;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; r0.w = (p ? 0.0 : pop); }
      p = r0.w == 1.0;
      r0.w = (p ? 0.0 : (r0.w == 0.0 ? 1.0 : r0.w));
      if (p) {
        r1.xz = r1.ww;
      }
      { float pop = r0.w - 1.0; p = pop <= 0.0; r0.w = (p ? 0.0 : pop); }
    case 35:
      r0 = clamp(r1.yzxw + PS_CONSTANT_PARAM_CARIBRATE.xxxx, 0.0, 1.0);
      if (xe_bool(128u /* PS_CONSTANT_BOOL_USE_GAMMA */)) {
        r1.x = log2(abs(r0.y));
        r1.y = log2(abs(r0.z));
        r1.z = log2(abs(r0.w));
        r1.xyz = xe_mul(r1.xyz, PS_CONSTANT_PARAM_GAMMA.zyx);
        r0.w = clamp(exp2(r1.z), 0.0, 1.0);
        r0.z = clamp(exp2(r1.y), 0.0, 1.0);
      }
      if (xe_bool(128u /* PS_CONSTANT_BOOL_USE_GAMMA */)) {
        r0.y = clamp(exp2(r1.x), 0.0, 1.0);
      }
      r0 = r0 + lit254.xxxx;
      r0 = xe_mul(r0, PS_CONSTANT_PARAM_CARIBRATE.yyyy);
      { precise float e = clamp(r0.x + lit253.x, 0.0, 1.0); xe_color0.w = e; }
      r0.xyz = clamp(r0.wzy + lit253.xxx, 0.0, 1.0);
      r0.w = xe_dp3(r0.zxy, lit255.xyz);
      r1.xyz = -r0.yzx + r0.www;
      { precise vec3 e = xe_mul(r1.zxy, PS_CONSTANT_PARAM_CARIBRATE.zzz) + r0.xyz; xe_color0.xyz = e; }
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
