// SBX's menu texture filtering (--menu_texture_filtering): bilinear
// sampling for the 2D screens (XMD menus, the HUD) that the game draws
// point-sampled. Recommended above the original 720p.
//
// The game's UI movie drawer asks for point filtering on sampler 0 unless
// its movie's context flag is set, and the game never sets it
// (docs/shaders.md, "Menu texture filtering"). That matches the screen 1:1
// at 720p, but is blocky when scaled up. The renderer leaves the sampler as
// the game set it and sets sbx_sprite_mode to 1 for the sprite shaders' draws only
// (passes.cpp). Here, when the fetch constant asks for point on both mag and
// min, the 4 nearest texels are read one at a time (texel centers through
// the game's point sampler, so its clamp or wrap still applies) and blended.
// Colors are weighted by their alpha, so the transparent texels' color
// (often black) doesn't darken the edges of cut-out pictures. Off, or a
// fetch constant that asks for linear, samples as the game does.

// The fetch constant's mag and min filters (word 3, bits 19-20 and 21-22;
// xenos::TextureFilter): both point (0).
bool menu_fetch_is_point(uint fetch_constant) {
  uint word = xe_fetch_word(fetch_constant * 6u + 3u);
  return bitfieldExtract(word, 19, 2) == 0u && bitfieldExtract(word, 21, 2) == 0u;
}

vec4 sample_menu(texture2DArray unsigned_view, texture2DArray signed_view, sampler s,
                 uint fetch_constant, vec2 uv) {
  // The level the game's sampler picks (its LOD range: the base level for
  // the game's textures, a sidecar's own levels), worked out before any
  // branch since it takes derivatives. The image's real size, not the fetch
  // constant's (a sidecar is larger than its stand-in).
  float lod = textureQueryLod(sampler2DArray(unsigned_view, s), uv).x;
  int levels = textureQueryLevels(sampler2DArray(unsigned_view, s));
  int level = clamp(int(lod + 0.5), 0, max(levels - 1, 0));
  vec2 size = vec2(textureSize(sampler2DArray(unsigned_view, s), level).xy);

  if ((xe_sys.sbx_sprite_mode & 0xFFu) != 1u || !menu_fetch_is_point(fetch_constant)) {
    return xe_tfetch2d(unsigned_view, signed_view, s, fetch_constant, uv, vec2(kTexelRounding),
                       0.0, true);
  }

  // Texel space with the texel centers on whole numbers.
  vec2 p = (uv + vec2(kTexelRounding) / xe_texture_size_2d(fetch_constant)) * size - 0.5;
  vec2 base = floor(p);
  vec2 f = p - base;
  float lod_f = float(level);
  vec3 weighted_rgb = vec3(0.0), rgb = vec3(0.0);
  float alpha = 0.0;
  vec4 signed_sum = vec4(0.0);
  for (int i = 0; i < 4; i++) {
    vec2 offset = vec2(float(i & 1), float(i >> 1));
    vec2 w2 = mix(1.0 - f, f, offset);
    float weight = w2.x * w2.y;
    vec3 c = vec3((base + offset + 0.5) / size, 0.0);
    vec4 u = textureLod(sampler2DArray(unsigned_view, s), c, lod_f);
    weighted_rgb += u.rgb * (u.a * weight);
    alpha += u.a * weight;
    rgb += u.rgb * weight;
    signed_sum += textureLod(sampler2DArray(signed_view, s), c, lod_f) * weight;
  }
  vec4 unsigned_sum = vec4(alpha > 1e-5 ? weighted_rgb / alpha : rgb, alpha);
  return xe_texture_result(unsigned_sum, signed_sum, fetch_constant);
}
