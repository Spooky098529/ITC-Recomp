#version 450
// A movie's frame (docs/native-movies.md): three 8-bit planes (Y, then the
// half-size U and V, sampled bilinearly as the movie library's sampler states
// do) turned to RGB by the numbers in the library's pixel shader for frames
// taller than 576 lines (BT.709, limited range: 0x8204D6B8 in the image).

layout(set = 0, binding = 0) uniform sampler2D y_plane;
layout(set = 0, binding = 1) uniform sampler2D u_plane;
layout(set = 0, binding = 2) uniform sampler2D v_plane;

layout(push_constant) uniform Constants {
  vec2 origin;  // the movie's rectangle in the target, in its pixels
  vec2 size;
} constants;

layout(location = 0) out vec4 color;

void main() {
  vec2 uv = (gl_FragCoord.xy - constants.origin) / constants.size;
  float y = 1.164383 * (texture(y_plane, uv).r - 0.0625);
  float u = texture(u_plane, uv).r - 0.5;
  float v = texture(v_plane, uv).r - 0.5;
  color = vec4(y + 1.792652 * v, y - 0.213238 * u - 0.533004 * v, y + 2.112418 * u, 1.0);
}
