#version 450
// Vertex shader 6B79546EE6E18088, first found in 00001275.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_VERTEX
#define XE_FLOAT_CONSTANTS 38
#include "xe_interface.glsl"

#define matWVPVS_0 xe_fc[0]  // c0
#define matWVPVS_1 xe_fc[1]  // c1
#define matWVPVS_2 xe_fc[2]  // c2
#define matWVPVS_3 xe_fc[3]  // c3
#define matWorldVS_0 xe_fc[4]  // c4
#define matWorldVS_1 xe_fc[5]  // c5
#define matWorldVS_2 xe_fc[6]  // c6
#define matWorldVS_3 xe_fc[7]  // c7
#define TexOffsetVS xe_fc[8]  // c9
#define MatColorVS xe_fc[9]  // c13
#define matTXVPVS_0 xe_fc[10]  // c70
#define matTXVPVS_1 xe_fc[11]  // c71
#define matTXVPVS_2 xe_fc[12]  // c72
#define matTXVPVS_3 xe_fc[13]  // c73
#define matTXVPVS_4 xe_fc[14]  // c74
#define matTXVPVS_5 xe_fc[15]  // c75
#define matTXVPVS_6 xe_fc[16]  // c76
#define matTXVPVS_7 xe_fc[17]  // c77
#define matPLVPVS_0 xe_fc[18]  // c87
#define matPLVPVS_1 xe_fc[19]  // c88
#define matPLVPVS_2 xe_fc[20]  // c89
#define matPLVPVS_3 xe_fc[21]  // c90
#define matPLMVPVS_0 xe_fc[22]  // c119
#define matPLMVPVS_1 xe_fc[23]  // c120
#define matPLMVPVS_2 xe_fc[24]  // c121
#define matPLMVPVS_3 xe_fc[25]  // c122
#define matPLMVPVS_4 xe_fc[26]  // c123
#define matPLMVPVS_5 xe_fc[27]  // c124
#define matPLMVPVS_6 xe_fc[28]  // c125
#define matPLMVPVS_7 xe_fc[29]  // c126
#define matPLMVPVS_8 xe_fc[30]  // c127
#define matPLMVPVS_9 xe_fc[31]  // c128
#define matPLMVPVS_10 xe_fc[32]  // c129
#define matPLMVPVS_11 xe_fc[33]  // c130
#define matPLMVPVS_12 xe_fc[34]  // c131
#define matPLMVPVS_13 xe_fc[35]  // c132
#define matPLMVPVS_14 xe_fc[36]  // c133
#define matPLMVPVS_15 xe_fc[37]  // c134

// Vertex fetch 0 (instruction 6): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 100) const uint XE_VF0_CONSTANT = 95u;
layout(constant_id = 101) const int XE_VF0_OFFSET = 0;
layout(constant_id = 102) const uint XE_VF0_STRIDE = 0u;
layout(constant_id = 103) const uint XE_VF0_FORMAT = 0u;
layout(constant_id = 104) const uint XE_VF0_FLAGS = 0u;
layout(constant_id = 105) const int XE_VF0_EXP_ADJUST = 0;
layout(constant_id = 106) const uint XE_VF0_SWIZZLE = 0x688u;
// Vertex fetch 1 (instruction 7): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 110) const uint XE_VF1_CONSTANT = 95u;
layout(constant_id = 111) const int XE_VF1_OFFSET = 0;
layout(constant_id = 112) const uint XE_VF1_STRIDE = 0u;
layout(constant_id = 113) const uint XE_VF1_FORMAT = 0u;
layout(constant_id = 114) const uint XE_VF1_FLAGS = 0u;
layout(constant_id = 115) const int XE_VF1_EXP_ADJUST = 0;
layout(constant_id = 116) const uint XE_VF1_SWIZZLE = 0x688u;
// Vertex fetch 2 (instruction 8): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 120) const uint XE_VF2_CONSTANT = 95u;
layout(constant_id = 121) const int XE_VF2_OFFSET = 0;
layout(constant_id = 122) const uint XE_VF2_STRIDE = 0u;
layout(constant_id = 123) const uint XE_VF2_FORMAT = 0u;
layout(constant_id = 124) const uint XE_VF2_FLAGS = 0u;
layout(constant_id = 125) const int XE_VF2_EXP_ADJUST = 0;
layout(constant_id = 126) const uint XE_VF2_SWIZZLE = 0x688u;
// Vertex fetch 3 (instruction 9): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 130) const uint XE_VF3_CONSTANT = 95u;
layout(constant_id = 131) const int XE_VF3_OFFSET = 0;
layout(constant_id = 132) const uint XE_VF3_STRIDE = 0u;
layout(constant_id = 133) const uint XE_VF3_FORMAT = 0u;
layout(constant_id = 134) const uint XE_VF3_FLAGS = 0u;
layout(constant_id = 135) const int XE_VF3_EXP_ADJUST = 0;
layout(constant_id = 136) const uint XE_VF3_SWIZZLE = 0x688u;
// Vertex fetch 4 (instruction 10): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 140) const uint XE_VF4_CONSTANT = 95u;
layout(constant_id = 141) const int XE_VF4_OFFSET = 0;
layout(constant_id = 142) const uint XE_VF4_STRIDE = 0u;
layout(constant_id = 143) const uint XE_VF4_FORMAT = 0u;
layout(constant_id = 144) const uint XE_VF4_FLAGS = 0u;
layout(constant_id = 145) const int XE_VF4_EXP_ADJUST = 0;
layout(constant_id = 146) const uint XE_VF4_SWIZZLE = 0x688u;
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
  precise vec4 r3 = vec4(0.0);
  precise vec4 r4 = vec4(0.0);
  precise vec4 r5 = vec4(0.0);
  precise vec4 r6 = vec4(0.0);
  precise vec4 r7 = vec4(0.0);
  precise vec4 r8 = vec4(0.0);
  precise vec4 r9 = vec4(0.0);
  bool p = false;
  do {
    r1 = xe_vfetch(XE_VF0_CONSTANT, XE_VF0_OFFSET, XE_VF0_STRIDE, XE_VF0_FORMAT, XE_VF0_FLAGS, XE_VF0_EXP_ADJUST, XE_VF0_SWIZZLE, r0.x, 15u);
    r6.xyz = xe_vfetch(XE_VF1_CONSTANT, XE_VF1_OFFSET, XE_VF1_STRIDE, XE_VF1_FORMAT, XE_VF1_FLAGS, XE_VF1_EXP_ADJUST, XE_VF1_SWIZZLE, r0.x, 7u).xyz;
    r2 = xe_vfetch(XE_VF2_CONSTANT, XE_VF2_OFFSET, XE_VF2_STRIDE, XE_VF2_FORMAT, XE_VF2_FLAGS, XE_VF2_EXP_ADJUST, XE_VF2_SWIZZLE, r0.x, 15u);
    r4 = xe_vfetch(XE_VF3_CONSTANT, XE_VF3_OFFSET, XE_VF3_STRIDE, XE_VF3_FORMAT, XE_VF3_FLAGS, XE_VF3_EXP_ADJUST, XE_VF3_SWIZZLE, r0.x, 15u);
    r3.xy = xe_vfetch(XE_VF4_CONSTANT, XE_VF4_OFFSET, XE_VF4_STRIDE, XE_VF4_FORMAT, XE_VF4_FLAGS, XE_VF4_EXP_ADJUST, XE_VF4_SWIZZLE, r0.x, 3u).xy;
    r0 = xe_mul(r1.yyyy, matWVPVS_1.wzyx);
    r0 = xe_mul(r1.xxxx, matWVPVS_0.xzwy) + r0.wyxz;
    r0 = xe_mul(r1.zzzz, matWVPVS_2.yzxw) + r0.wyxz;
    { precise vec4 e = xe_mul(r1.wwww, matWVPVS_3) + r0.zxyw; xe_position = e; }
    r0 = xe_mul(r1.yyyy, matWorldVS_1);
    r7.xyz = xe_mul(r2.yyy, matWorldVS_1.xyz);
    r5.xyz = xe_mul(r6.yyy, matWorldVS_1.xyz);
    r5.xyz = xe_mul(r6.xxx, matWorldVS_0.zxy) + r5.zxy;
    r7.xyz = xe_mul(r2.xxx, matWorldVS_0.zxy) + r7.zxy;
    r0 = xe_mul(r1.xxxx, matWorldVS_0.wyxz) + r0.wyxz;
    r0 = xe_mul(r1.zzzz, matWorldVS_2.zywx) + r0.wyxz;
    r8.yzw = xe_mul(r2.zzz, matWorldVS_2.yzx) + r7.zxy;
    r9.yzw = xe_mul(r6.zzz, matWorldVS_2.yzx) + r5.zxy;
    { precise vec4 e = xe_mul(r4, MatColorVS); xe_interpolators[11] = e; }
    { precise vec2 e = r3.xy + TexOffsetVS.xy; xe_interpolators[3].xy = e; }
    { precise float e = r2.w; xe_interpolators[2].w = e; }
    r9.x = xe_dp3(r9.zwy, r9.zwy);
    r7.x = xe_dp3(r8.zwy, r8.zwy);
    r1 = xe_mul(r1.wwww, matWorldVS_3.zyxw) + r0.xywz;
    { precise vec3 e = r1.zyx; xe_interpolators[0].xyz = e; }
    r0 = xe_mul(r1.yyyy, matPLMVPVS_13.wzyx);
    r2 = xe_mul(r1.yyyy, matPLMVPVS_9.wzyx);
    r3 = xe_mul(r1.yyyy, matPLMVPVS_5.wzyx);
    r4 = xe_mul(r1.yyyy, matPLMVPVS_1.wzyx);
    r5 = xe_mul(r1.yyyy, matTXVPVS_5.wzyx);
    { precise vec4 v = xe_mul(r1.yyyy, matTXVPVS_1.wzyx); precise float s = inversesqrt(abs(r7.x)); r6 = v; r8.x = s; }
    { precise vec4 v = xe_mul(r1.yyyy, matPLVPVS_1.wzyx); precise float s = inversesqrt(abs(r9.x)); r7 = v; r9.x = s; }
    { precise vec3 e = xe_mul(r9.wyz, r9.xxx); xe_interpolators[1].xyz = e; }
    xe_interpolators[1].w = 0.0;
    { precise vec3 e = xe_mul(r8.wyz, r8.xxx); xe_interpolators[2].xyz = e; }
    r7 = xe_mul(r1.zzzz, matPLVPVS_0.xzwy) + r7.wyxz;
    r6 = xe_mul(r1.zzzz, matTXVPVS_0.xzwy) + r6.wyxz;
    r5 = xe_mul(r1.zzzz, matTXVPVS_4.xzwy) + r5.wyxz;
    r4 = xe_mul(r1.zzzz, matPLMVPVS_0.xzwy) + r4.wyxz;
    r3 = xe_mul(r1.zzzz, matPLMVPVS_4.xzwy) + r3.wyxz;
    r2 = xe_mul(r1.zzzz, matPLMVPVS_8.xzwy) + r2.wyxz;
    r0 = xe_mul(r1.zzzz, matPLMVPVS_12.xzwy) + r0.wyxz;
    r0 = xe_mul(r1.xxxx, matPLMVPVS_14.yzxw) + r0.wyxz;
    r2 = xe_mul(r1.xxxx, matPLMVPVS_10.yzxw) + r2.wyxz;
    r3 = xe_mul(r1.xxxx, matPLMVPVS_6.yzxw) + r3.wyxz;
    r4 = xe_mul(r1.xxxx, matPLMVPVS_2.yzxw) + r4.wyxz;
    r5 = xe_mul(r1.xxxx, matTXVPVS_6.yzxw) + r5.wyxz;
    r6 = xe_mul(r1.xxxx, matTXVPVS_2.yzxw) + r6.wyxz;
    r7 = xe_mul(r1.xxxx, matPLVPVS_2.yzxw) + r7.wyxz;
    { precise vec4 e = xe_mul(r1.wwww, matPLVPVS_3) + r7.zxyw; xe_interpolators[4] = e; }
    { precise vec4 e = xe_mul(r1.wwww, matTXVPVS_3) + r6.zxyw; xe_interpolators[5] = e; }
    { precise vec4 e = xe_mul(r1.wwww, matTXVPVS_7) + r5.zxyw; xe_interpolators[6] = e; }
    { precise vec4 e = xe_mul(r1.wwww, matPLMVPVS_3) + r4.zxyw; xe_interpolators[7] = e; }
    { precise vec4 e = xe_mul(r1.wwww, matPLMVPVS_7) + r3.zxyw; xe_interpolators[8] = e; }
    { precise vec4 e = xe_mul(r1.wwww, matPLMVPVS_11) + r2.zxyw; xe_interpolators[9] = e; }
    { precise vec4 e = xe_mul(r1.wwww, matPLMVPVS_15) + r0.zxyw; xe_interpolators[10] = e; }
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
