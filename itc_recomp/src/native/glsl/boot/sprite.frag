// The game's 2D shaders (compiled at boot from HLSL): a texture (Texture),
// the vertex color, or both multiplied; or the vertex color with one of the
// texture's channels as the alpha (the font).
//   TEXTURE       the texture
//   VERTEX_COLOR  the vertex color
//   ALPHA_CHANNEL the texture channel (x, y, z or w) that, times the vertex
//                 alpha, is the alpha; the color is the vertex color's
#include "common.glsl"

layout(location = 0) in vec4 uv;
#ifdef VERTEX_COLOR
layout(location = 1) in vec4 v_color;
#endif

layout(location = 0) out vec4 out_color;

void main() {
#if defined(ALPHA_CHANNEL)
  float coverage = sample_2d(Texture, uv.xy).ALPHA_CHANNEL;
  vec4 color = vec4(v_color.rgb, coverage * v_color.a);
#elif defined(TEXTURE) && defined(VERTEX_COLOR)
  vec4 color = sample_2d(Texture, uv.xy) * v_color;
#elif defined(TEXTURE)
  vec4 color = sample_2d(Texture, uv.xy);
#else
  vec4 color = v_color;
#endif
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
