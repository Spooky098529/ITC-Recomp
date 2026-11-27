// Screen-space vertices (post effects, sprites, the boot screens): the
// position as given, and the color and coordinates passed on.
//   POSITION, COLOR, UV0-UV2  the vertex fetch each attribute is read with
//   COLOR_REG, UV0_REG-UV2_REG  the interpolator each goes to (the
//                coordinates in x and y)
//   POSITION_2D  the position has only x and y (z 0, w 1);
//   POSITION_3D  x, y and z (w 1); otherwise all four
//   PIXEL_CENTER the position moved by half a pixel (-0.5 in x and y)
//   POSITION_OFFSET  VS_CONSTANT_PARAM_POS_OFFSET added to the position
//   TRANSFORM=<constant>  the position through that matrix
#include "vertex.glsl"

void vertex_shader(float vertex_index) {
#if defined(POSITION_2D)
  vec4 position = vec4(fetch_attribute(POSITION, 3u).xy, 0.0, 1.0);
#elif defined(POSITION_3D)
  vec4 position = vec4(fetch_attribute(POSITION, 7u).xyz, 1.0);
#else
  vec4 position = fetch_attribute(POSITION, 15u);
#endif
#ifdef PIXEL_CENTER
  position.xy -= 0.5;
#endif
#ifdef POSITION_OFFSET
  position += VS_CONSTANT_PARAM_POS_OFFSET;
#endif
#ifdef TRANSFORM
  position = MATRIX(TRANSFORM) * position;
#endif
  out_position = position;
#ifdef COLOR_REG
  out_interpolators[COLOR_REG] = fetch_attribute(COLOR, 15u);
#endif
#ifdef UV0_REG
  out_interpolators[UV0_REG].xy = fetch_attribute(UV0, 3u).xy;
#endif
#ifdef UV1_REG
  out_interpolators[UV1_REG].xy = fetch_attribute(UV1, 3u).xy;
#endif
#ifdef UV2_REG
  out_interpolators[UV2_REG].xy = fetch_attribute(UV2, 3u).xy;
#endif
}
