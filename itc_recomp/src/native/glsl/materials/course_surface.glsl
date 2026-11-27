// The course's surfaces: a color texture (tx0) and an optional second one
// (tx1) over it, each scaled by its ModTexAlpha component (x for tx0, y for
// tx1). The course shaders (course.frag, course_projected.frag) light what
// this returns.
//
// The defines (see variants.txt):
//   TX0_UV, TX1_UV      the textures' coordinates
//   DETAIL_ADD          tx1 is added over tx0, by its coverage
//   DETAIL_BLEND        tx1 is blended over tx0, by its coverage
//   VERTEX_ALPHA        the vertex alpha scales tx1's coverage (and, with
//                       tx0 alone, the alpha)
//   SHADE_BASE          the shade multiplies tx0 only
//   SHADE_DETAIL        the shade multiplies tx1
//   SHADE_RESULT        the shade multiplies the result
// The shade is the vertex color, plus the light where the shader has
// lights. With two textures the alpha is tx0's plus tx1's.

struct Surface {
  vec4 color;          // rgb, and the alpha
  float detail_alpha;  // tx1's alpha (0 with tx0 alone)
};

Surface course_surface(vec3 shade, float vertex_alpha) {
  vec4 base = sample_2d(tx0, TX0_UV.xy);
  vec3 color = base.rgb * ModTexAlpha.x;
#ifdef SHADE_BASE
  color *= shade;
#endif
#ifdef VERTEX_ALPHA
  float alpha_scale = vertex_alpha;
#else
  float alpha_scale = 1.0;
#endif

#ifdef TX1_UV
  vec4 detail = sample_2d(tx1, TX1_UV.xy);
  vec3 detail_color = detail.rgb * ModTexAlpha.y;
#ifdef SHADE_DETAIL
  detail_color *= shade;
#endif
  float coverage = detail.a * alpha_scale;
#if defined(DETAIL_ADD)
  color += detail_color * coverage;
#elif defined(DETAIL_BLEND)
  color = mix(color, detail_color, coverage);
#endif
  float alpha = clamp(base.a + detail.a, 0.0, 1.0);
#else
  vec4 detail = vec4(0.0);
  float alpha = clamp(base.a * alpha_scale, 0.0, 1.0);
#endif

#ifdef SHADE_RESULT
  color *= shade;
#endif
  return Surface(vec4(color, alpha), detail.a);
}
