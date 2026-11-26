// A texture times the vertex color.
//   UV_REG, COLOR_REG  the interpolators
//   TEXTURE_ALPHA      the alpha is the texture's times the vertex alpha
//                      (clamped to 0-1); otherwise it's the vertex alpha
//   MINUS_ONE          the result less 1, clamped to 0-1
//   PROJTEX_CLAMP      no negative colors when UseProjTex is set
//   SBX_GLASS          glass for the renderer (passes.cpp, ShaderFamily 4:
//                      the AO looks through it)
#include "common.glsl"

#ifdef SBX_GLASS
layout(constant_id = 901) const int SBX_FAMILY = 4;
#endif

layout(location = UV_REG) in vec4 uv;
layout(location = COLOR_REG) in vec4 v_color;

layout(location = 0) out vec4 out_color;

void main() {
  vec4 texel = sample_2d(tx0, uv.xy);
  vec4 color = vec4(texel.rgb * v_color.rgb, v_color.a);
#ifdef TEXTURE_ALPHA
  color.a = clamp(texel.a * v_color.a, 0.0, 1.0);
#endif
#ifdef MINUS_ONE
  color = clamp(color - 1.0, 0.0, 1.0);
#endif
#ifdef PROJTEX_CLAMP
  if (UseProjTex) color.rgb = max(color.rgb, 0.0);
#endif
  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
}
