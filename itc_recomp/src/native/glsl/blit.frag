#version 450
// A blit for the D3D11 backend (D3D11 has no scaled image copy, as
// vkCmdBlitImage is): the source rectangle scaled onto the destination's,
// filtered by the sampler the backend binds (BlitImage, and what's shown
// fitted to the window). Drawn with resolve.vert over the destination
// rectangle's viewport.

layout(set = 0, binding = 0) uniform sampler2D source;

layout(push_constant) uniform Constants {
  vec4 dest;    // the destination rectangle: x, y, width, height (pixels)
  vec4 source;  // the source rectangle, in the source's 0-1 coordinates
} constants;

layout(location = 0) out vec4 color;

void main() {
  vec2 at = (gl_FragCoord.xy - constants.dest.xy) / constants.dest.zw;
  color = textureLod(source, constants.source.xy + at * constants.source.zw, 0.0);
}
