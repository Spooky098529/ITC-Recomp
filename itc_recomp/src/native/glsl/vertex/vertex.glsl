// What every hand-written vertex shader shares: reading the vertex's
// attributes, the matrices, the interpolators, and main().
//
// A shader file includes this and defines vertex_shader(vertex_index),
// which sets out_position (the position as the game's shader writes it)
// and the out_interpolators the pixel shader reads (by register; the
// variant's *_REG defines say which). Interpolators it doesn't set are 0.

// The attribute read by vertex fetch n (the variant's POSITION, NORMAL, ...
// say which fetch is which), with the fields the game patched into it.
// used: the components the shader reads, a bit each.
#define fetch_attribute(n, used) fetch_attribute_(n, used)
#define fetch_attribute_(n, used)                                                         \
  xe_vfetch(n, XE_VF##n##_CONSTANT, XE_VF##n##_OFFSET, XE_VF##n##_STRIDE, XE_VF##n##_FORMAT, \
            XE_VF##n##_FLAGS, XE_VF##n##_EXP_ADJUST, XE_VF##n##_SWIZZLE, vertex_index, used)
// Vertex fetch n's fetch constant and stride (in words).
#define XE_VF_CONSTANT_OF(n) XE_VF_CONSTANT_OF_(n)
#define XE_VF_CONSTANT_OF_(n) XE_VF##n##_CONSTANT
#define XE_VF_STRIDE_OF(n) XE_VF_STRIDE_OF_(n)
#define XE_VF_STRIDE_OF_(n) XE_VF##n##_STRIDE

// A D3D matrix constant (four float4 rows, for row vectors: v * M) as a
// GLSL matrix, so that matrix * v is the same transform. at: the first row,
// for constants that hold several matrices.
#define MATRIX(name) MATRIX_AT(name, 0)
#define MATRIX_AT(name, at) mat4(name(at), name((at) + 1), name((at) + 2), name((at) + 3))
// Its rotation and scale only, for directions.
#define MATRIX3(name) mat3(name(0).xyz, name(1).xyz, name(2).xyz)

// A direction scaled to length 1; a zero direction stays 0 (the console's
// 0 times infinity).
vec3 normalized(vec3 v) {
  float length_squared = dot(v, v);
  return length_squared == 0.0 ? vec3(0.0) : v * inversesqrt(abs(length_squared));
}

vec4 out_position;
vec4 out_interpolators[16];

void vertex_shader(float vertex_index);

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

void write_interpolators(vec4 interpolators[16]) {
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

// Runs the shader for the vertex. A rectangle list (XE_RECTANGLE_LIST) is
// drawn an instance a rectangle, four vertices each, with its three
// vertices' attributes at instance rate: each invocation runs the shader for
// all three and makes the fourth corner from them.
void main() {
  vec4 positions[3];
  vec4 interpolators[3][16];
  uint runs = XE_RECTANGLE_LIST ? 3u : 1u;
  for (uint g = 0u; g < runs; g++) {
    xe_vf_slot = g;
    out_position = vec4(0.0);
    for (int i = 0; i < 16; i++) out_interpolators[i] = vec4(0.0);
    vertex_shader(XE_RECTANGLE_LIST ? float(uint(gl_InstanceIndex) * 3u + g)
                                    : float(gl_VertexIndex));
    positions[g] = xe_host_position(out_position);
    interpolators[g] = out_interpolators;
  }
  if (!XE_RECTANGLE_LIST) {
    gl_Position = positions[0];
    write_interpolators(interpolators[0]);
    return;
  }
  // The corners in order from the one opposite the longest edge; the fourth
  // is v1 - v0 + v2.
  float edges[3];
  for (int e = 0; e < 3; e++) {
    vec2 d = positions[(2 + e) % 3].xy - positions[(1 + e) % 3].xy;
    edges[e] = d.x * d.x + d.y * d.y;
  }
  int v0 = (edges[0] > edges[1] && edges[0] > edges[2]) ? 0 : (edges[1] > edges[2] ? 1 : 2);
  int v1 = (v0 + 1) % 3, v2 = (v0 + 2) % 3;
  uint corner = uint(gl_VertexIndex) & 3u;
  int pick = corner == 0u ? v0 : (corner == 1u ? v1 : v2);
  precise vec4 fourth_position = (positions[v1] - positions[v0]) + positions[v2];
  gl_Position = corner == 3u ? fourth_position : positions[pick];
  vec4 corner_interpolators[16];
  for (int i = 0; i < 16; i++) {
    precise vec4 fourth = (interpolators[v1][i] - interpolators[v0][i]) + interpolators[v2][i];
    corner_interpolators[i] = corner == 3u ? fourth : interpolators[pick][i];
  }
  write_interpolators(corner_interpolators);
}
