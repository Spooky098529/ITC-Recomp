#version 450
// Vertex shader 0F5D499758C1F179, first found in 46BA935F0FEB99A7.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_VERTEX
#define XE_FLOAT_CONSTANTS 6
#include "xe_interface.glsl"

#define matWVPVS_0 xe_fc[0]  // c0
#define matWVPVS_1 xe_fc[1]  // c1
#define matWVPVS_2 xe_fc[2]  // c2
#define matWVPVS_3 xe_fc[3]  // c3
#define MatColorVS xe_fc[4]  // c13
#define fogParamVS xe_fc[5]  // c86

// Vertex fetch 0 (instruction 3): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 100) const uint XE_VF0_CONSTANT = 95u;
layout(constant_id = 101) const int XE_VF0_OFFSET = 0;
layout(constant_id = 102) const uint XE_VF0_STRIDE = 0u;
layout(constant_id = 103) const uint XE_VF0_FORMAT = 0u;
layout(constant_id = 104) const uint XE_VF0_FLAGS = 0u;
layout(constant_id = 105) const int XE_VF0_EXP_ADJUST = 0;
layout(constant_id = 106) const uint XE_VF0_SWIZZLE = 0x688u;
// Vertex fetch 1 (instruction 4): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 110) const uint XE_VF1_CONSTANT = 95u;
layout(constant_id = 111) const int XE_VF1_OFFSET = 0;
layout(constant_id = 112) const uint XE_VF1_STRIDE = 0u;
layout(constant_id = 113) const uint XE_VF1_FORMAT = 0u;
layout(constant_id = 114) const uint XE_VF1_FLAGS = 0u;
layout(constant_id = 115) const int XE_VF1_EXP_ADJUST = 0;
layout(constant_id = 116) const uint XE_VF1_SWIZZLE = 0x688u;
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
  precise vec4 r2 = vec4(0.0);
  bool p = false;
  do {
    r2 = xe_vfetch(XE_VF0_CONSTANT, XE_VF0_OFFSET, XE_VF0_STRIDE, XE_VF0_FORMAT, XE_VF0_FLAGS, XE_VF0_EXP_ADJUST, XE_VF0_SWIZZLE, r0.x, 15u);
    r1 = xe_vfetch(XE_VF1_CONSTANT, XE_VF1_OFFSET, XE_VF1_STRIDE, XE_VF1_FORMAT, XE_VF1_FLAGS, XE_VF1_EXP_ADJUST, XE_VF1_SWIZZLE, r0.x, 15u);
    r0 = xe_mul(r2.yyyy, matWVPVS_1.wzyx);
    r0 = xe_mul(r2.xxxx, matWVPVS_0.xzwy) + r0.wyxz;
    r0 = xe_mul(r2.zzzz, matWVPVS_2.yzxw) + r0.wyxz;
    r0 = xe_mul(r2.wwww, matWVPVS_3) + r0.zxyw;
    { precise vec4 e = r0; xe_position = e; }
    r0.x = -r0.w + fogParamVS.y;
    { precise float e = xe_mul(fogParamVS.z, r0.x); xe_interpolators[0].x = e; }
    { precise vec4 e = xe_mul(r1, MatColorVS); xe_interpolators[1] = e; }
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
  uint host_index = uint(gl_VertexIndex);
  uint index = host_index == xe_sys.line_loop_closing_index ? 0u : host_index;
  // A rectangle list runs the shader for the rectangle's three vertices.
  vec4 positions[3];
  vec4 interpolators[3][16];
  uint runs = XE_RECTANGLE_LIST ? 3u : 1u;
  for (uint g = 0u; g < runs; g++) {
    uint vertex;
    if (XE_RECTANGLE_LIST) {
      vertex = ((g == 0u ? index : host_index) >> 2u) * 3u + g;
      if ((xe_sys.flags & kXeFlagPrimitiveVertexIndexLoad) != 0u) vertex = xe_load_vertex_index(vertex);
    } else {
      vertex = xe_endian_swap(index, xe_sys.vertex_index_endian);
    }
    xe_shader(xe_remap_vertex_index(vertex));
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
  uint corner = host_index & 3u;
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
