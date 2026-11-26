// The course's simpler materials: the textures of course_surface.glsl,
// shaded by the vertex color, with an optional reflection or point lights.
//
// variants.txt picks the features for each of the game's shaders:
//   POSITION_REG, NORMAL_REG, UV0_REG, UV1_REG, COLOR_REG
//       the interpolators the vertex shader writes (the world-space
//       position and normal, the texture coordinates, the vertex color)
//   the surface's defines (course_surface.glsl)
//   ENV_CUBE      a reflection from cubetx0, by MatParam.y, where tx1
//                 doesn't cover it (by tx1's alpha)
//   POINT_LIGHTS  the course's point lights (course_lights.glsl), diffuse
//                 only, added to the vertex color
//   STREAM_NORMAL_REG  without NORMAL_REG: the interpolator where the vertex
//                 shader puts the mesh's own normal, which the game's shader
//                 doesn't read (model.vert); only for the AO's normals
//                 target, not the shading (0 where the mesh has none)
// Without a vertex color the shade is white.
#include "common.glsl"

// The shader's family for the renderer (passes.cpp, ShaderFamily: 1 the
// models' lit material, 2 their unlit one, 3 the course's), read from the
// SPIR-V; nothing reads it here.
layout(constant_id = 901) const int SBX_FAMILY = 3;

#ifdef POSITION_REG
layout(location = POSITION_REG) in vec4 v_position;
#endif
#ifdef NORMAL_REG
layout(location = NORMAL_REG) in vec4 v_normal;
#endif
#ifdef UV0_REG
layout(location = UV0_REG) in vec4 uv0;
#endif
#ifdef UV1_REG
layout(location = UV1_REG) in vec4 uv1;
#endif
#ifdef COLOR_REG
layout(location = COLOR_REG) in vec4 v_color;
#endif

layout(location = 0) out vec4 out_color;
#ifdef STREAM_NORMAL_REG
layout(location = STREAM_NORMAL_REG) in vec4 v_stream_normal;
#endif
#if defined(NORMAL_REG) || defined(STREAM_NORMAL_REG)
#include "normals_out.glsl"
#endif

#include "course_surface.glsl"
#ifdef POINT_LIGHTS
#include "course_lights.glsl"
#endif

#ifdef POINT_LIGHTS
// The diffuse light from the point lights.
vec3 point_light(vec3 position, vec3 normal) {
  vec3 light = vec3(0.0);
  for (int i = 0; i < kCourseLights; i++) {
    if (!course_light_on(i)) continue;
    LightReach reach = course_light_reach(i, position);
    if (reach.reaches) {
      vec3 diffuse = scale_color(PntLightColor(i).rgb, max(dot(normal, reach.direction), 0.0));
      light += scale_color(diffuse, reach.strength);
    }
  }
  return light;
}
#endif

void main() {
#ifdef COLOR_REG
  vec4 vertex_color = v_color;
#else
  vec4 vertex_color = vec4(1.0);
#endif
  vec3 shade = vertex_color.rgb;
#ifdef POINT_LIGHTS
  shade += point_light(v_position.xyz, normalize(v_normal.xyz));
#endif

  Surface surface = course_surface(shade, vertex_color.a);
  vec4 color = surface.color;
#ifdef ENV_CUBE
  vec3 normal = normalize(v_normal.xyz);
  vec3 to_eye = normalize(EyePos.xyz - v_position.xyz);
  vec3 reflection = 2.0 * dot(to_eye, normal) * normal - to_eye;
  color.rgb += sample_cube(cubetx0, reflection).rgb * MatParam.y * (1.0 - surface.detail_alpha);
#endif
  if (UseProjTex) color.rgb = max(color.rgb, 0.0);

  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
#ifdef NORMAL_REG
  write_normal(v_normal.xyz);
#elif defined(STREAM_NORMAL_REG)
  write_normal(v_stream_normal.xyz);
#endif
}
