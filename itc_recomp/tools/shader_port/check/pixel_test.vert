#version 450
// Feeds a ported pixel shader: one triangle over the target, with each of
// its three vertices' 16 interpolators from shared memory (words 0-191).
layout(set = 0, binding = 0, std430) readonly buffer XeSharedMemory { float m[]; };
layout(location = 0) out vec4 o[16];
void main() {
  int v = gl_VertexIndex;
  gl_Position = vec4(v == 1 ? 3.0 : -1.0, v == 2 ? 3.0 : -1.0, 0.5, 1.0);
  for (int i = 0; i < 16; i++) {
    int at = (v * 16 + i) * 4;
    o[i] = vec4(m[at], m[at + 1], m[at + 2], m[at + 3]);
  }
}
