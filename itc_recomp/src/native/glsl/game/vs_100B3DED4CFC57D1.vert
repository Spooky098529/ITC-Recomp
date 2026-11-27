#version 450
// Vertex shader 100B3DED4CFC57D1, first found in k820076C8_vs_3_0
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_VERTEX
#define XE_FLOAT_CONSTANTS 1
#include "xe_interface.glsl"

const vec4 lit255 = vec4(-0.5, 0.0, 0.0, 0.0);

// Vertex fetch 0 (instruction 3): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 100) const uint XE_VF0_CONSTANT = 95u;
layout(constant_id = 101) const int XE_VF0_OFFSET = 0;
layout(constant_id = 102) const uint XE_VF0_STRIDE = 0u;
layout(constant_id = 103) const uint XE_VF0_FORMAT = 0u;
layout(constant_id = 104) const uint XE_VF0_FLAGS = 0u;
layout(constant_id = 105) const int XE_VF0_EXP_ADJUST = 0;
layout(constant_id = 106) const uint XE_VF0_SWIZZLE = 0x688u;
layout(location = 0) in uvec4 xe_vf_in_0_0;
layout(location = 1) in uvec4 xe_vf_in_0_1;
layout(location = 2) in uvec4 xe_vf_in_0_2;
// Vertex fetch 1 (instruction 4): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 110) const uint XE_VF1_CONSTANT = 95u;
layout(constant_id = 111) const int XE_VF1_OFFSET = 0;
layout(constant_id = 112) const uint XE_VF1_STRIDE = 0u;
layout(constant_id = 113) const uint XE_VF1_FORMAT = 0u;
layout(constant_id = 114) const uint XE_VF1_FLAGS = 0u;
layout(constant_id = 115) const int XE_VF1_EXP_ADJUST = 0;
layout(constant_id = 116) const uint XE_VF1_SWIZZLE = 0x688u;
layout(location = 3) in uvec4 xe_vf_in_1_0;
layout(location = 4) in uvec4 xe_vf_in_1_1;
layout(location = 5) in uvec4 xe_vf_in_1_2;
// Vertex fetch 2 (instruction 5): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 120) const uint XE_VF2_CONSTANT = 95u;
layout(constant_id = 121) const int XE_VF2_OFFSET = 0;
layout(constant_id = 122) const uint XE_VF2_STRIDE = 0u;
layout(constant_id = 123) const uint XE_VF2_FORMAT = 0u;
layout(constant_id = 124) const uint XE_VF2_FLAGS = 0u;
layout(constant_id = 125) const int XE_VF2_EXP_ADJUST = 0;
layout(constant_id = 126) const uint XE_VF2_SWIZZLE = 0x688u;
layout(location = 6) in uvec4 xe_vf_in_2_0;
layout(location = 7) in uvec4 xe_vf_in_2_1;
layout(location = 8) in uvec4 xe_vf_in_2_2;
// Drawing a rectangle list: each invocation runs the shader for the
// rectangle's three vertices and makes the fourth.
layout(constant_id = 2) const bool XE_RECTANGLE_LIST = false;

// The interpolators, at locations the renderer sets for the pixel shader.
layout(location = 0) out vec4 xe_out_interpolator_0;
layout(location = 1) out vec4 xe_out_interpolator_1;
layout(location = 2) out vec4 xe_out_interpolator_2;
layout(location = 3) out vec4 xe_out_interpolator_3;
layout(location = 4) out vec4 xe_out_interpolator_4;
layout(location = 5) out vec4 xe_out_interpolator_5;
layout(location = 6) out vec4 xe_out_interpolator_6;
layout(location = 7) out vec4 xe_out_interpolator_7;
layout(location = 8) out vec4 xe_out_interpolator_8;
layout(location = 9) out vec4 xe_out_interpolator_9;
layout(location = 10) out vec4 xe_out_interpolator_10;
layout(location = 11) out vec4 xe_out_interpolator_11;
layout(location = 12) out vec4 xe_out_interpolator_12;
layout(location = 13) out vec4 xe_out_interpolator_13;
layout(location = 14) out vec4 xe_out_interpolator_14;
layout(location = 15) out vec4 xe_out_interpolator_15;

vec4 xe_position = vec4(0.0);
vec4 xe_interpolators[16];

void xe_shader(float vertex_index) {
  for (int i = 0; i < 16; i++) xe_interpolators[i] = vec4(0.0);
  precise vec4 r0 = vec4(vertex_index, 0.0, 0.0, 0.0);
  precise vec4 r1 = vec4(0.0);
  bool p = false;
  do {
    r0.zw = xe_vfetch(0, XE_VF0_CONSTANT, XE_VF0_OFFSET, XE_VF0_STRIDE, XE_VF0_FORMAT, XE_VF0_FLAGS, XE_VF0_EXP_ADJUST, XE_VF0_SWIZZLE, r0.x, 3u).xy;
    r1 = xe_vfetch(1, XE_VF1_CONSTANT, XE_VF1_OFFSET, XE_VF1_STRIDE, XE_VF1_FORMAT, XE_VF1_FLAGS, XE_VF1_EXP_ADJUST, XE_VF1_SWIZZLE, r0.x, 15u);
    r0.xy = xe_vfetch(2, XE_VF2_CONSTANT, XE_VF2_OFFSET, XE_VF2_STRIDE, XE_VF2_FORMAT, XE_VF2_FLAGS, XE_VF2_EXP_ADJUST, XE_VF2_SWIZZLE, r0.x, 3u).xy;
    { precise vec2 e = r0.zw + lit255.xx; xe_position.xy = e; }
    xe_position.z = 0.0;
    xe_position.w = 1.0;
    { precise vec2 e = r0.xy; xe_interpolators[0].xy = e; }
    { precise vec4 e = r1; xe_interpolators[1] = e; }
    break;  // the end
  } while (false);
}

void xe_write_interpolators(vec4 interpolators[16]) {
  xe_out_interpolator_0 = interpolators[0];
  xe_out_interpolator_1 = interpolators[1];
  xe_out_interpolator_2 = interpolators[2];
  xe_out_interpolator_3 = interpolators[3];
  xe_out_interpolator_4 = interpolators[4];
  xe_out_interpolator_5 = interpolators[5];
  xe_out_interpolator_6 = interpolators[6];
  xe_out_interpolator_7 = interpolators[7];
  xe_out_interpolator_8 = interpolators[8];
  xe_out_interpolator_9 = interpolators[9];
  xe_out_interpolator_10 = interpolators[10];
  xe_out_interpolator_11 = interpolators[11];
  xe_out_interpolator_12 = interpolators[12];
  xe_out_interpolator_13 = interpolators[13];
  xe_out_interpolator_14 = interpolators[14];
  xe_out_interpolator_15 = interpolators[15];
}

void main() {
  // A rectangle list runs the shader for the rectangle's three vertices
  // (an instance a rectangle, its vertices' attributes at instance rate).
  vec4 positions[3];
  vec4 interpolators[3][16];
  uint runs = XE_RECTANGLE_LIST ? 3u : 1u;
  for (uint g = 0u; g < runs; g++) {
    xe_vf_slot = g;
    xe_shader(XE_RECTANGLE_LIST ? float(uint(gl_InstanceIndex) * 3u + g) : float(gl_VertexIndex));
    positions[g] = xe_host_position(xe_position);
    interpolators[g] = xe_interpolators;
  }
  if (!XE_RECTANGLE_LIST) {
    gl_Position = positions[0];
    xe_write_interpolators(interpolators[0]);
    return;
  }
  // The rectangle's corners in order from the one opposite the longest edge;
  // the fourth vertex is v1 - v0 + v2.
  float edges[3];
  for (int e = 0; e < 3; e++) {
    vec2 d = positions[(2 + e) % 3].xy - positions[(1 + e) % 3].xy;
    edges[e] = d.x * d.x + d.y * d.y;
  }
  int v0 = (edges[0] > edges[1] && edges[0] > edges[2]) ? 0 : (edges[1] > edges[2] ? 1 : 2);
  int v1 = (v0 + 1) % 3, v2 = (v0 + 2) % 3;
  uint corner = uint(gl_VertexIndex) & 3u;
  int pick = corner == 0u ? v0 : (corner == 1u ? v1 : v2);
  precise vec4 synthetic_position = (positions[v1] - positions[v0]) + positions[v2];
  gl_Position = corner == 3u ? synthetic_position : positions[pick];
  vec4 out_interpolators[16];
  for (int i = 0; i < 16; i++) {
    precise vec4 synthetic = (interpolators[v1][i] - interpolators[v0][i]) + interpolators[v2][i];
    out_interpolators[i] = corner == 3u ? synthetic : interpolators[pick][i];
  }
  xe_write_interpolators(out_interpolators);
}
