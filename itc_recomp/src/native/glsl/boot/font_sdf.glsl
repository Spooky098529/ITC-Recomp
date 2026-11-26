// SBX's distance-field font (docs/game_data.md, "Font"; font_upscale in
// sbxr_tools makes it): the sheet holds each glyph's signed distance to its
// edge instead of its coverage, 0.5 on the edge, `spread` sheet pixels
// inside reaching 1 and outside 0. The renderer sets sbx_sprite_mode to 2
// with the spread (in 1/256 pixels, from bit 8) for the font shaders' draws
// of that sheet only (src/hooks/font.cpp names it; passes.cpp). Sharp at any
// size: the edge is blended over one screen pixel, however many sheet pixels
// that is.

// A distance sample (the channel's value, sampled bilinearly as the game
// does) as coverage of this pixel.
float font_sdf_coverage(float distance, texture2DArray view, sampler s, vec2 uv) {
  float spread = float(xe_sys.sbx_sprite_mode >> 8u) / 256.0;
  vec2 texels = uv * vec2(textureSize(sampler2DArray(view, s), 0).xy);
  vec2 per_pixel = fwidth(texels);
  float texels_per_pixel = max(0.5 * (per_pixel.x + per_pixel.y), 1e-4);
  // Sheet pixels from the edge (positive inside), then screen pixels.
  float edge = (distance - 0.5) * 2.0 * spread;
  return clamp(edge / texels_per_pixel + 0.5, 0.0, 1.0);
}
