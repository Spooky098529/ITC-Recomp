#version 450
// Vertex shader 87970D553E7A7218, first found in E06A09FDAEA78377.xpu
// Written by tools/shader_port from the game's microcode; docs/shaders.md explains it.
#extension GL_GOOGLE_include_directive : require
#define XE_VERTEX
#define XE_FLOAT_CONSTANTS 87
#include "xe_interface.glsl"

#define matWVPVS_0 xe_fc[0]  // c0
#define matWVPVS_1 xe_fc[1]  // c1
#define matWVPVS_2 xe_fc[2]  // c2
#define matWVPVS_3 xe_fc[3]  // c3
#define matWorldVS_0 xe_fc[4]  // c4
#define matWorldVS_1 xe_fc[5]  // c5
#define matWorldVS_2 xe_fc[6]  // c6
#define matWorldVS_3 xe_fc[7]  // c7
#define TexOffsetVS_0 xe_fc[9]  // c9
#define TexOffsetVS_1 xe_fc[10]  // c10
#define TexOffsetVS_2 xe_fc[11]  // c11
#define MatColorVS xe_fc[13]  // c13
#define fogParamVS xe_fc[86]  // c86

// Vertex fetch 0 (instruction 5): its fields as in the shader file, until the renderer sets them from the patched instruction.
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
// Vertex fetch 1 (instruction 6): its fields as in the shader file, until the renderer sets them from the patched instruction.
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
// Vertex fetch 2 (instruction 7): its fields as in the shader file, until the renderer sets them from the patched instruction.
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
// Vertex fetch 3 (instruction 8): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 130) const uint XE_VF3_CONSTANT = 95u;
layout(constant_id = 131) const int XE_VF3_OFFSET = 0;
layout(constant_id = 132) const uint XE_VF3_STRIDE = 0u;
layout(constant_id = 133) const uint XE_VF3_FORMAT = 0u;
layout(constant_id = 134) const uint XE_VF3_FLAGS = 0u;
layout(constant_id = 135) const int XE_VF3_EXP_ADJUST = 0;
layout(constant_id = 136) const uint XE_VF3_SWIZZLE = 0x688u;
layout(location = 9) in uvec4 xe_vf_in_3_0;
layout(location = 10) in uvec4 xe_vf_in_3_1;
layout(location = 11) in uvec4 xe_vf_in_3_2;
// Vertex fetch 4 (instruction 9): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 140) const uint XE_VF4_CONSTANT = 95u;
layout(constant_id = 141) const int XE_VF4_OFFSET = 0;
layout(constant_id = 142) const uint XE_VF4_STRIDE = 0u;
layout(constant_id = 143) const uint XE_VF4_FORMAT = 0u;
layout(constant_id = 144) const uint XE_VF4_FLAGS = 0u;
layout(constant_id = 145) const int XE_VF4_EXP_ADJUST = 0;
layout(constant_id = 146) const uint XE_VF4_SWIZZLE = 0x688u;
layout(location = 12) in uvec4 xe_vf_in_4_0;
layout(location = 13) in uvec4 xe_vf_in_4_1;
layout(location = 14) in uvec4 xe_vf_in_4_2;
// Vertex fetch 5 (instruction 10): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 150) const uint XE_VF5_CONSTANT = 95u;
layout(constant_id = 151) const int XE_VF5_OFFSET = 0;
layout(constant_id = 152) const uint XE_VF5_STRIDE = 0u;
layout(constant_id = 153) const uint XE_VF5_FORMAT = 0u;
layout(constant_id = 154) const uint XE_VF5_FLAGS = 0u;
layout(constant_id = 155) const int XE_VF5_EXP_ADJUST = 0;
layout(constant_id = 156) const uint XE_VF5_SWIZZLE = 0x688u;
layout(location = 15) in uvec4 xe_vf_in_5_0;
layout(location = 16) in uvec4 xe_vf_in_5_1;
layout(location = 17) in uvec4 xe_vf_in_5_2;
// Vertex fetch 6 (instruction 11): its fields as in the shader file, until the renderer sets them from the patched instruction.
layout(constant_id = 160) const uint XE_VF6_CONSTANT = 95u;
layout(constant_id = 161) const int XE_VF6_OFFSET = 0;
layout(constant_id = 162) const uint XE_VF6_STRIDE = 0u;
layout(constant_id = 163) const uint XE_VF6_FORMAT = 0u;
layout(constant_id = 164) const uint XE_VF6_FLAGS = 0u;
layout(constant_id = 165) const int XE_VF6_EXP_ADJUST = 0;
layout(constant_id = 166) const uint XE_VF6_SWIZZLE = 0x688u;
layout(location = 18) in uvec4 xe_vf_in_6_0;
layout(location = 19) in uvec4 xe_vf_in_6_1;
layout(location = 20) in uvec4 xe_vf_in_6_2;
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
    r2 = xe_vfetch(0, XE_VF0_CONSTANT, XE_VF0_OFFSET, XE_VF0_STRIDE, XE_VF0_FORMAT, XE_VF0_FLAGS, XE_VF0_EXP_ADJUST, XE_VF0_SWIZZLE, r0.x, 15u);
    r9.xyz = xe_vfetch(1, XE_VF1_CONSTANT, XE_VF1_OFFSET, XE_VF1_STRIDE, XE_VF1_FORMAT, XE_VF1_FLAGS, XE_VF1_EXP_ADJUST, XE_VF1_SWIZZLE, r0.x, 7u).xyz;
    r6 = xe_vfetch(2, XE_VF2_CONSTANT, XE_VF2_OFFSET, XE_VF2_STRIDE, XE_VF2_FORMAT, XE_VF2_FLAGS, XE_VF2_EXP_ADJUST, XE_VF2_SWIZZLE, r0.x, 15u);
    r1 = xe_vfetch(3, XE_VF3_CONSTANT, XE_VF3_OFFSET, XE_VF3_STRIDE, XE_VF3_FORMAT, XE_VF3_FLAGS, XE_VF3_EXP_ADJUST, XE_VF3_SWIZZLE, r0.x, 15u);
    r4.xy = xe_vfetch(4, XE_VF4_CONSTANT, XE_VF4_OFFSET, XE_VF4_STRIDE, XE_VF4_FORMAT, XE_VF4_FLAGS, XE_VF4_EXP_ADJUST, XE_VF4_SWIZZLE, r0.x, 3u).xy;
    r3.zw = xe_vfetch(5, XE_VF5_CONSTANT, XE_VF5_OFFSET, XE_VF5_STRIDE, XE_VF5_FORMAT, XE_VF5_FLAGS, XE_VF5_EXP_ADJUST, XE_VF5_SWIZZLE, r0.x, 3u).xy;
    r3.xy = xe_vfetch(6, XE_VF6_CONSTANT, XE_VF6_OFFSET, XE_VF6_STRIDE, XE_VF6_FORMAT, XE_VF6_FLAGS, XE_VF6_EXP_ADJUST, XE_VF6_SWIZZLE, r0.x, 3u).xy;
    r0 = xe_mul(r2.yyyy, matWVPVS_1.wzyx);
    r0 = xe_mul(r2.xxxx, matWVPVS_0.xzwy) + r0.wyxz;
    r0 = xe_mul(r2.zzzz, matWVPVS_2.yzxw) + r0.wyxz;
    r8 = xe_mul(r2.wwww, matWVPVS_3) + r0.zxyw;
    { precise vec4 e = r8; xe_position = e; }
    r0.xyz = xe_mul(r2.yyy, matWorldVS_1.xyz);
    r7.xyz = xe_mul(r6.yyy, matWorldVS_1.xyz);
    r5.xyz = xe_mul(r9.yyy, matWorldVS_1.xyz);
    r5.xyz = xe_mul(r9.xxx, matWorldVS_0.zxy) + r5.zxy;
    r7.xyz = xe_mul(r6.xxx, matWorldVS_0.zxy) + r7.zxy;
    r0.xyz = xe_mul(r2.xxx, matWorldVS_0.zxy) + r0.zxy;
    r0.yzw = xe_mul(r2.zzz, matWorldVS_2.yzx) + r0.zxy;
    r7.xyz = xe_mul(r6.zzz, matWorldVS_2.yzx) + r7.zxy;
    r5.xyz = xe_mul(r9.zzz, matWorldVS_2.yzx) + r5.zxy;
    r0.x = xe_dp3(r5.yzx, r5.yzx);
    { precise float v = xe_dp3(r7.yzx, r7.yzx); precise float s = inversesqrt(abs(r0.x)); r4.w = v; r4.z = s; }
    { precise float v = -r8.w + fogParamVS.y; precise float s = inversesqrt(abs(r4.w)); r0.x = v; r4.w = s; }
    { precise vec3 v = xe_mul(r7.zxy, r4.www); precise float s = r6.w; xe_interpolators[2].xyz = v; xe_interpolators[2].w = s; }
    { precise vec3 v = xe_mul(r5.zxy, r4.zzz); precise float s = xe_mul(fogParamVS.z, r0.x); xe_interpolators[1].xyz = v; xe_interpolators[1].w = s; }
    { precise vec2 e = r4.xy + TexOffsetVS_0.xy; xe_interpolators[3].xy = e; }
    { precise vec2 e = r3.zw + TexOffsetVS_1.xy; xe_interpolators[4].xy = e; }
    { precise vec2 e = r3.xy + TexOffsetVS_2.xy; xe_interpolators[5].xy = e; }
    { precise vec3 e = xe_mul(r2.www, matWorldVS_3.xyz) + r0.wyz; xe_interpolators[0].xyz = e; }
    { precise vec4 e = xe_mul(r1, MatColorVS); xe_interpolators[6] = e; }
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
