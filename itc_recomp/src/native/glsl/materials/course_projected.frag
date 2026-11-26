// The course's lit materials: the textures of course_surface.glsl, lit by
// the course's point lights (course_lights.glsl) with diffuse light and a
// highlight, and, when UseProjTex is set, with the light-space textures:
//   - a shadow map for light 0 (plshd00, 16 taps),
//   - a mask for each light (plmask00, one texture for all four): a light
//     reaches only inside its mask's frustum, scaled by the mask,
//   - two projectors: prjtx0 darkens by the sum of its channels, prjtx1
//     adds its color (times 3), each fading out over 3 units above or
//     below its height (ProjectionPos.w) and scaled by how much the surface
//     faces it (ProjectionPos.xyz).
//
// variants.txt picks the features for each of the game's shaders:
//   POSITION_REG, NORMAL_REG, TANGENT_REG, UV0_REG, UV1_REG
//       the world-space position and normal, the tangent (with the
//       bitangent's sign in w), the texture coordinates
//   SHADOW_REG    light 0's shadow map coordinates; the two projectors'
//                 coordinates follow it, then the four lights' masks'
//   COLOR_REG     the vertex color (without it, the surface isn't shaded
//                 by the diffuse light, only the highlight is added)
//   BTX0_UV       a normal map; its alpha scales the normal
//   the surface's defines (course_surface.glsl)
#include "common.glsl"

// The shader's family for the renderer (passes.cpp, ShaderFamily: 1 the
// models' lit material, 2 their unlit one, 3 the course's), read from the
// SPIR-V; nothing reads it here.
layout(constant_id = 901) const int SBX_FAMILY = 3;

layout(location = POSITION_REG) in vec4 v_position;
layout(location = NORMAL_REG) in vec4 v_normal;
#ifdef TANGENT_REG
layout(location = TANGENT_REG) in vec4 v_tangent;
#endif
layout(location = UV0_REG) in vec4 uv0;
#ifdef UV1_REG
layout(location = UV1_REG) in vec4 uv1;
#endif
layout(location = SHADOW_REG) in vec4 v_shadow;
layout(location = SHADOW_REG + 1) in vec4 v_projector0;
layout(location = SHADOW_REG + 2) in vec4 v_projector1;
layout(location = SHADOW_REG + 3) in vec4 v_mask[4];  // one per light
#ifdef COLOR_REG
layout(location = COLOR_REG) in vec4 v_color;
#endif

layout(location = 0) out vec4 out_color;
#include "normals_out.glsl"

#include "course_surface.glsl"
#include "course_lights.glsl"

const float kShininess = 30.0;

// The light-space textures are sampled at every pixel and used where they
// apply: a texture's level comes from how its coordinates change between
// neighboring pixels, which is only defined when they all sample it. (The
// game's shaders sample inside the branches, so where a pixel's neighbors
// don't, its level is whatever the hardware makes of that.)

// A light's mask: 0 outside its frustum.
float light_mask(int i) {
  vec2 uv;
  float depth;
  bool inside = in_light_frustum(v_mask[i], uv, depth);
  float mask = sample_2d(plmask00, uv).x;
  return inside ? mask : 0.0;
}

struct Light {
  vec3 diffuse;
  vec3 specular;
};

// shadowing: how much of each light the shadow map and masks let through.
Light gather_light(vec3 position, vec3 normal, vec3 reflection, float shadowing[kCourseLights]) {
  Light light = Light(vec3(0.0), vec3(0.0));
  for (int i = 0; i < kCourseLights; i++) {
    if (!course_light_on(i)) continue;
    LightReach reach = course_light_reach(i, position);
    if (!reach.reaches) continue;
    float strength = times(reach.strength, shadowing[i]);
    vec3 diffuse = scale_color(PntLightColor(i).rgb, max(dot(normal, reach.direction), 0.0));
    light.diffuse += scale_color(diffuse, strength);
    vec3 highlight = scale_color(PntLightSpecularColor(i).rgb * MatSpecularColor.rgb,
                                 specular_power(dot(reflection, reach.direction), kShininess));
    light.specular += scale_color(highlight, strength);
  }
  return light;
}

// Where a projector reaches the point: its texture coordinates, and how
// much (the height fade times how much the surface faces it; 0 where it
// doesn't reach).
float projector_reach(vec4 coords, vec4 projection, out vec2 uv) {
  float facing = dot(v_normal.xyz, projection.xyz);
  float fade = 1.0 - clamp(abs(projection.w - v_position.y) / 3.0, 0.0, 1.0);
  float inverse_w = 1.0 / coords.w;
  vec2 p = coords.xy * 0.5 * inverse_w + 0.5;
  uv = vec2(p.x, 1.0 - p.y);
  bool inside = coords.z * inverse_w < 1.0 && coords.z > 0.0 && facing > 0.0 && fade > 0.0 &&
                all(greaterThan(p, vec2(0.0))) && all(lessThan(p, vec2(1.0)));
  return inside ? facing * fade : 0.0;
}

void main() {
  vec3 position = v_position.xyz;
#ifdef BTX0_UV
  // The normal map's xyz (0-1 for -1 to 1) along the tangent, the bitangent
  // and the normal.
  vec4 bump_texel = sample_2d(btx0, BTX0_UV.xy);
  vec3 bump = bump_texel.xyz * 2.0 - 1.0;
  vec3 bitangent = cross(v_tangent.xyz, v_normal.xyz) * v_tangent.w;
  // The map's alpha scales the normal, so it dims the light it takes.
  vec3 normal = normalize(bump.x * v_tangent.xyz + bump.y * bitangent + bump.z * v_normal.xyz) *
                bump_texel.a;
#else
  vec3 normal = normalize(v_normal.xyz);
#endif
  vec3 to_eye = normalize(EyePos.xyz - position);
  vec3 reflection = 2.0 * dot(to_eye, normal) * normal - to_eye;
  bool projected = UseProjTex;
  float shadowing[kCourseLights] = float[](1.0, 1.0, 1.0, 1.0);
  if (projected) {
    shadowing[0] = shadow_test(plshd00, v_shadow, shadowMapSize.x);
    for (int i = 0; i < kCourseLights; i++) shadowing[i] *= light_mask(i);
  }
  Light light = gather_light(position, normal, reflection, shadowing);

#ifdef COLOR_REG
  Surface surface = course_surface(v_color.rgb + light.diffuse, v_color.a);
#else
  Surface surface = course_surface(vec3(1.0), 1.0);
#endif
  vec4 color = surface.color;
  color.rgb += light.specular;

  if (projected) {
    vec2 shade_uv, light_uv;
    float shade = projector_reach(v_projector0, ProjectionPos(0), shade_uv);
    float lit = projector_reach(v_projector1, ProjectionPos(1), light_uv);
    vec4 shade_texel = sample_2d(prjtx0, shade_uv), light_texel = sample_2d(prjtx1, light_uv);
    if (shade != 0.0) color.rgb -= dot(shade_texel, vec4(1.0)) * shade;
    vec3 added = lit != 0.0 ? light_texel.rgb * lit * 3.0 : vec3(0.0);
    color.rgb = max(color.rgb + added, 0.0);
    xe_light_extra = min(added, color.rgb);  // light, for the AO (xe_interface.glsl)
  }

  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
  write_normal(v_normal.xyz);  // the geometry's, without the normal map
}
