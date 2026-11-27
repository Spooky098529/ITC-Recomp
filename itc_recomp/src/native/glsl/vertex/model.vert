// Model vertices (cars, the course, the garage and menu models): the
// position through matWVPVS, and the attributes the pixel shader reads,
// each in the interpolator its *_REG define names.
//   POSITION, NORMAL, TANGENT, COLOR, UV0-UV2
//       the vertex fetch each attribute is read with
//   WORLD_POSITION_REG  the position in world space (matWorldVS), xyz
//   NORMAL_REG       the normal in world space (matWorldVS without its
//                    translation); NORMALIZE_NORMAL scales it to length 1
//   TANGENT_REG      the tangent in world space, length 1, with the
//                    bitangent's sign (the tangent's w) in w
//   UV0_REG-UV2_REG  the coordinates, plus TexOffsetVS (one per set of
//                    coordinates when the constant has several)
//   COLOR_REG        the vertex color times MatColorVS
//   CLIP_POSITION_REG, CLIP_POSITION_REG_2  the position as drawn, again
//   OLD_CLIP_POSITION_REG  the position through matOldWVPVS (last frame's,
//                    for the velocity pass)
//   SHADOW_REG       the world position in the directional light's shadow
//                    map (matDLVPVS)
//   COURSE_LIGHTS_REG  the world position in light space for the course's
//                    lights, seven interpolators from there: light 0's
//                    shadow map (matPLVPVS), the two projectors (matTXVPVS)
//                    and the four lights' masks (matPLMVPVS)
#include "vertex.glsl"

#if TexOffsetVS_COUNT == 1
#define tex_offset(i) TexOffsetVS
#else
#define tex_offset(i) TexOffsetVS(i)
#endif

void vertex_shader(float vertex_index) {
  vec4 position = fetch_attribute(POSITION, 15u);
  out_position = MATRIX(matWVPVS) * position;
#if defined(WORLD_POSITION_REG) || defined(SHADOW_REG) || defined(COURSE_LIGHTS_REG)
  vec4 world_position = MATRIX(matWorldVS) * position;
#endif
#ifdef WORLD_POSITION_REG
  out_interpolators[WORLD_POSITION_REG].xyz = world_position.xyz;
#endif
#ifdef NORMAL_REG
  vec3 normal = MATRIX3(matWorldVS) * fetch_attribute(NORMAL, 7u).xyz;
#ifdef NORMALIZE_NORMAL
  normal = normalized(normal);
#endif
  out_interpolators[NORMAL_REG].xyz = normal;
#endif
#ifdef TANGENT_REG
  vec4 tangent = fetch_attribute(TANGENT, 15u);
  out_interpolators[TANGENT_REG] = vec4(normalized(MATRIX3(matWorldVS) * tangent.xyz), tangent.w);
#endif
#ifdef UV0_REG
  out_interpolators[UV0_REG].xy = fetch_attribute(UV0, 3u).xy + tex_offset(0).xy;
#endif
#ifdef UV1_REG
  out_interpolators[UV1_REG].xy = fetch_attribute(UV1, 3u).xy + tex_offset(1).xy;
#endif
#ifdef UV2_REG
  out_interpolators[UV2_REG].xy = fetch_attribute(UV2, 3u).xy + tex_offset(2).xy;
#endif
#ifdef COLOR_REG
  out_interpolators[COLOR_REG] = fetch_attribute(COLOR, 15u) * MatColorVS;
#endif
#ifdef CLIP_POSITION_REG
  out_interpolators[CLIP_POSITION_REG] = out_position;
#endif
#ifdef CLIP_POSITION_REG_2
  out_interpolators[CLIP_POSITION_REG_2] = out_position;
#endif
#ifdef OLD_CLIP_POSITION_REG
  out_interpolators[OLD_CLIP_POSITION_REG] = MATRIX(matOldWVPVS) * position;
#endif
#ifdef SHADOW_REG
  out_interpolators[SHADOW_REG] = MATRIX(matDLVPVS) * world_position;
#endif
#ifdef COURSE_LIGHTS_REG
  out_interpolators[COURSE_LIGHTS_REG] = MATRIX(matPLVPVS) * world_position;
  for (int i = 0; i < 2; i++) {
    out_interpolators[COURSE_LIGHTS_REG + 1 + i] = MATRIX_AT(matTXVPVS, 4 * i) * world_position;
  }
  for (int i = 0; i < 4; i++) {
    out_interpolators[COURSE_LIGHTS_REG + 3 + i] = MATRIX_AT(matPLMVPVS, 4 * i) * world_position;
  }
#endif
}
