#version 450
// Pixel shader D6135642F477C186, first found in k8206E308_ps_3_0
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_PIXEL
#define XE_FLOAT_CONSTANTS 4
#include "xe_interface.glsl"

#define StartY xe_fc[0]  // c2
#define EndY xe_fc[1]  // c3
const vec4 lit254 = vec4(2.0, 6.0, 3.0, 0.0);
const vec4 lit255 = vec4(0.0, 1.0, 5.0, 4.0);

// The interpolators the vertex shader writes (a bit each), and the register
// the generated parameter goes in (-1: none).
layout(constant_id = 0) const uint XE_INTERPOLATOR_MASK = 0xFFFFu;
layout(constant_id = 1) const int XE_PARAM_GEN_REGISTER = -1;
layout(location = 0) in vec4 xe_in_interpolator_0;
layout(location = 1) in vec4 xe_in_interpolator_1;
layout(location = 2) in vec4 xe_in_interpolator_2;
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
  bool p = false;
  precise float ps = 0.0;
  do {
    r0.x = EndY.y + -StartY.y;
    { precise float v = abs(r2.y) + -StartY.y; precise float s = 1.0 / r0.x; ps = s; r0.y = v; r0.x = s; }
    r0.y = xe_mul(r0.y, r0.x);
    r0.x = r0.y + -lit255.y;
    r0.z = (r0.y >= lit255.y ? 1.0 : 0.0);
    r0.x = (r0.z == 0.0 ? r0.y : r0.x);
    ps = xe_mul(lit254.y, r0.x);
    r0.y = ps;
    ps = floor(r0.y);
    r0.z = ps;
    { precise float v = (lit255.y > r0.z ? 1.0 : 0.0); precise float s = r0.y; ps = s; r0.x = v; }
    { vec4 pa = lit255.xxxx, pb = r0.xxxx; p = pa.w == 0.0 && !(pb.w == 0.0); precise float v = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; precise float s = -r0.z + ps; ps = s; r2.x = v; r0.x = s; }
    ps = lit255.y - r0.x;
    r0.x = ps;
    ps = lit255.y - r0.x;
    r0.y = ps;
    if (p) {
      r0.xz = lit255.yx;
    }
    p = r2.x == 1.0;
    ps = (p ? 0.0 : (r2.x == 0.0 ? 1.0 : r2.x));
    r2.x = ps;
    if (p) {
      r0.w = (lit254.x > r0.z ? 1.0 : 0.0);
    }
    { vec4 pa = r2.xxxx, pb = r0.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r2.x = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
    if (p) {
      r0.yz = lit255.yx;
    }
    p = r2.x == 1.0;
    ps = (p ? 0.0 : (r2.x == 0.0 ? 1.0 : r2.x));
    r2.x = ps;
    if (p) {
      r0.w = (lit254.z > r0.z ? 1.0 : 0.0);
    }
    { vec4 pa = r2.xxxx, pb = r0.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r2.x = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
    if (p) {
      r0.xw = lit255.xy;
    }
    if (p) {
      ps = r0.y;
      r0.z = ps;
    }
    p = r2.x == 1.0;
    ps = (p ? 0.0 : (r2.x == 0.0 ? 1.0 : r2.x));
    r2.x = ps;
    if (p) {
      r0.w = (lit255.w > r0.z ? 1.0 : 0.0);
    }
    { vec4 pa = r2.xxxx, pb = r0.wwww; p = pa.w == 0.0 && !(pb.w == 0.0); r2.x = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
    if (p) {
      r0.yz = lit255.xy;
    }
    if (p) {
      ps = r0.x;
      r0.w = ps;
    }
    p = r2.x == 1.0;
    ps = (p ? 0.0 : (r2.x == 0.0 ? 1.0 : r2.x));
    r2.x = ps;
    if (p) {
      r0.z = (lit255.z > r0.z ? 1.0 : 0.0);
    }
    { vec4 pa = r2.xxxx, pb = r0.zzzz; p = pa.w == 0.0 && !(pb.w == 0.0); r2.x = ((pa.x == 0.0 && !(pb.x == 0.0)) ? -1.0 : pa.x) + 1.0; }
    if (p) {
      ps = lit255.y;
      r0.x = ps;
    }
    p = r2.x == 1.0;
    ps = (p ? 0.0 : (r2.x == 0.0 ? 1.0 : r2.x));
    r2.x = ps;
    if (p) {
      ps = lit255.y;
      r0.y = ps;
    }
    { float pop = r2.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.x = ps; }
    if (p) {
      { precise float v = (-abs(r1.x) > lit255.x ? 1.0 : 0.0); precise float s = r0.x; ps = s; r0.w = v; r0.z = s; }
    }
    { float pop = r2.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.x = ps; }
    if (p) {
      ps = r0.y;
      r0.x = ps;
    }
    { float pop = r2.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.x = ps; }
    if (p) {
      ps = r0.w;
      r0.y = ps;
    }
    { float pop = r2.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.x = ps; }
    { float pop = r2.x - 1.0; p = pop <= 0.0; ps = (p ? 0.0 : pop); r2.x = ps; }
    r1.xyz = xe_mul(r0.xyz, r1.xyz);
    { precise vec4 e = r1; xe_color0 = e; }
    break;  // the end
  } while (false);
}

void main() {
  xe_shader();
  if (!xe_alpha_test_passes(xe_color0.a)) discard;
  xe_out_color_0 = xe_color_output(xe_color0, 0);
}
