#version 450
// Records a ported vertex shader's outputs: for vertex n (a point), row n
// gets the position, then the interpolators, a texel each.
layout(points) in;
layout(points, max_vertices = 17) out;
layout(push_constant) uniform Push { uint width, height, interpolators; };
in gl_PerVertex { vec4 gl_Position; } gl_in[];
layout(location = 0) in vec4 i[][16];
layout(location = 0) flat out vec4 value;
void main() {
  for (uint j = 0u; j <= interpolators && j < 17u; j++) {
    vec2 at = (vec2(float(j), float(gl_PrimitiveIDIn)) + 0.5) / vec2(float(width), float(height));
    gl_Position = vec4(at * 2.0 - 1.0, 0.5, 1.0);
    value = j == 0u ? gl_in[0].gl_Position : i[0][j - 1u];
    EmitVertex();
    EndPrimitive();
  }
}
