// The game's lit material (the car parts, and most other models): ambient,
// diffuse and specular light from the directional and point lights, a
// color texture or two, and an optional environment map.
//
// variants.txt picks the features for each of the game's shaders:
//   POSITION_REG, NORMAL_REG, COLOR_REG, UV0_REG, UV1_REG, UV2_REG, TANGENT_REG
//       the interpolators the vertex shader writes (the world-space
//       position and normal, the vertex color, texture coordinates, and the
//       tangent with the bitangent's sign in w)
//   NORMALIZE_NORMAL  normalize the interpolated normal (else it's used as is)
//   TX0_UV, TX1_UV, TX2_UV  color textures tx0-tx2 and their coordinates;
//       with tx0 and tx1, tx1 is blended over tx0 by its alpha
//   TX1_OVER          with tx0 and tx1, tx1 laid over tx0 as one layer over
//                     another: tx0's alpha is at least the vertex alpha, the
//                     alpha is the two layers' together, and the color is
//                     divided by it (0 where the alpha is under 0.001)
//   BASE_COLOR        tx0 over the BaseColor constant, by tx0's alpha
//   BTX0_UV           normal map btx0 (needs TANGENT_REG)
//   SPECULAR          specular highlights; SPTX0_UV adds a specular map
//                     (multiplying MatSpecularColor)
//   SPECULAR_FLOOR    the smallest cosine the highlight uses (common.glsl)
//   TWO_SIDED         lit from both sides: the diffuse light by |N.L| and the
//                     highlights by |R.L| (with SPECULAR_FLOOR=0.0)
//   BACKLIT_HIGHLIGHT each highlight times 1 - min(N.L, 0): stronger where
//                     the surface faces away from the light
//   REFLECTION_MASK   refmasktx0's alpha, at these coordinates, moves the
//                     specular color toward 0.1 and the reflection strength
//                     toward 0.7
//   FRESNEL           the highlights and the reflection times
//                     0.2 + 0.8 * (1 - N.V)^5
//   SPECULAR_ALPHA    tx0's alpha scales the specular color and the
//                     reflection strength
//   BASE_COLOR_OVER   with BASE_COLOR, the base color over tx0 instead (by
//                     tx0's alpha)
//   BASE_ALPHA        the alpha is BaseColor's (times the vertex alpha)
//   SPHERE_NORMALIZE  the sphere map's view direction is normalized
//   ENV_CUBE, ENV_SPHERE  a reflection from cubetx0 or spheretx0
//   PROJTEX_CLAMP     no negative colors when UseProjTex is set
//   ALPHA_SATURATE    clamp the alpha to 0-1
//   FIXED_LIGHTS      a fixed number of lights, DIRECTIONAL_COUNT and
//                     POINT_COUNT (3 and 4 unless set; else the loop
//                     constants say how many); a shader without one kind's
//                     constants has none of it
//   SHADOW_REG        a shadow map for directional light 0 (dlshd00), at
//                     these light-space coordinates, when UseProjTex is set
#include "common.glsl"

// The shader's family for the renderer (passes.cpp, ShaderFamily: 1 the
// models' lit material, 2 their unlit one, 3 the course's, 4 glass), read
// from the SPIR-V; nothing reads it here. The glass: the shaders with a
// third texture coordinate (UV2_REG), which only the windows and light
// covers have (stream type 0x10, docs/game_data.md); the AO looks through
// it.
#ifdef UV2_REG
layout(constant_id = 901) const int SBX_FAMILY = 4;
#else
layout(constant_id = 901) const int SBX_FAMILY = 1;
#endif

layout(location = POSITION_REG) in vec4 v_position;
layout(location = NORMAL_REG) in vec4 v_normal;
layout(location = COLOR_REG) in vec4 v_color;
#ifdef UV0_REG
layout(location = UV0_REG) in vec4 uv0;
#endif
#ifdef UV1_REG
layout(location = UV1_REG) in vec4 uv1;
#endif
#ifdef UV2_REG
layout(location = UV2_REG) in vec4 uv2;
#endif
#ifdef TANGENT_REG
layout(location = TANGENT_REG) in vec4 v_tangent;
#endif
#ifdef SHADOW_REG
layout(location = SHADOW_REG) in vec4 v_shadow;
#endif

layout(location = 0) out vec4 out_color;
#include "normals_out.glsl"

// The surface at this pixel.
vec3 surface_normal() {
#ifdef BTX0_UV
  // The normal map's xyz (0-1 for -1 to 1) along the tangent, the bitangent
  // and the normal.
  vec3 bump = sample_2d(btx0, BTX0_UV.xy).xyz * 2.0 - 1.0;
  vec3 bitangent = cross(v_tangent.xyz, v_normal.xyz) * v_tangent.w;
  return normalize(bump.x * v_tangent.xyz + bump.y * bitangent + bump.z * v_normal.xyz);
#elif defined(NORMALIZE_NORMAL)
  return normalize(v_normal.xyz);
#else
  return v_normal.xyz;
#endif
}

// ModTexAlpha scales the textures' alphas where the shader has it.
#ifdef ModTexAlpha
#define TEXTURE_ALPHA_SCALE ModTexAlpha
#else
#define TEXTURE_ALPHA_SCALE vec4(1.0)
#endif

// The surface's color (rgb) and opacity (a) before lighting.
vec4 surface_color() {
#if defined(TX0_UV) && defined(TX1_UV) && defined(TX1_OVER)
  // tx1 over tx0, as layers.
  vec4 base = sample_2d(tx0, TX0_UV.xy), detail = sample_2d(tx1, TX1_UV.xy);
  float base_alpha = max(base.a, v_color.a);
  float alpha = base_alpha + detail.a - base_alpha * detail.a;
  vec3 color = detail.rgb * detail.a + base.rgb * base_alpha * (1.0 - detail.a);
  return vec4(alpha > 0.001 ? color / alpha : vec3(0.0), alpha);
#elif defined(TX0_UV) && defined(TX1_UV)
  // tx1 over tx0.
  vec4 base = sample_2d(tx0, TX0_UV.xy), detail = sample_2d(tx1, TX1_UV.xy);
  float base_alpha = base.a * TEXTURE_ALPHA_SCALE.x, detail_alpha = detail.a * TEXTURE_ALPHA_SCALE.y;
  return vec4(mix(base.rgb, detail.rgb, detail_alpha),
              1.0 - (1.0 - base_alpha) * (1.0 - detail_alpha));
#elif defined(TX0_UV) && defined(BASE_COLOR)
  // tx0 over the base color.
  vec4 texel = sample_2d(tx0, TX0_UV.xy);
  float alpha = texel.a * TEXTURE_ALPHA_SCALE.x;
#if defined(BASE_ALPHA) && defined(BASE_COLOR_OVER)
  return vec4(mix(texel.rgb, BaseColor.rgb, alpha), BaseColor.a);
#elif defined(BASE_ALPHA)
  return vec4(mix(BaseColor.rgb, texel.rgb, alpha), BaseColor.a);
#else
  return vec4(mix(BaseColor.rgb, texel.rgb, alpha), 1.0 - (1.0 - BaseColor.a) * (1.0 - alpha));
#endif
#elif defined(TX0_UV)
  vec4 texel = sample_2d(tx0, TX0_UV.xy);
  return vec4(texel.rgb, texel.a * TEXTURE_ALPHA_SCALE.x);
#elif defined(TX1_UV)
  vec4 texel = sample_2d(tx1, TX1_UV.xy);
  return vec4(texel.rgb, texel.a * TEXTURE_ALPHA_SCALE.y);
#elif defined(TX2_UV)
  vec4 texel = sample_2d(tx2, TX2_UV.xy);
  return vec4(texel.rgb, texel.a * TEXTURE_ALPHA_SCALE.z);
#else
  return vec4(1.0);
#endif
}

#ifdef SPECULAR
// How much of a light's highlight shows, by how the surface faces it.
float highlight_facing(vec3 normal, vec3 direction) {
#ifdef BACKLIT_HIGHLIGHT
  return 1.0 - min(dot(normal, direction), 0.0);
#else
  return 1.0;
#endif
}

// The cosine a highlight is raised to a power of.
float highlight_cosine(vec3 reflection, vec3 direction) {
#ifdef TWO_SIDED
  return abs(dot(reflection, direction));
#else
  return dot(reflection, direction);
#endif
}

// How much the surface reflects by the view angle (1 without FRESNEL).
float fresnel(vec3 normal, vec3 to_eye) {
#ifdef FRESNEL
  float facing = 1.0 - max(dot(to_eye, normal), 0.0);
  float square = facing * facing;
  return square * square * facing * 0.8 + 0.2;  // facing^5
#else
  return 1.0;
#endif
}

// The reflection mask (tx0's alpha; 0 without REFLECTION_MASK).
float reflection_mask() {
#ifdef REFLECTION_MASK
  return sample_2d(refmasktx0, REFLECTION_MASK.xy).a;
#else
  return 0.0;
#endif
}

vec3 specular_color() {
#ifdef SPTX0_UV
  vec3 color = MatSpecularColor.rgb * sample_2d(sptx0, SPTX0_UV.xy).rgb;
#else
  vec3 color = MatSpecularColor.rgb;
#endif
#ifdef REFLECTION_MASK
  color = mix(color, vec3(0.1), reflection_mask());
#endif
#ifdef SPECULAR_ALPHA
  color *= sample_2d(tx0, TX0_UV.xy).a;
#endif
  return color;
}
#endif

// How much a light reaches the surface by the angle between them (cosine:
// N.L, or R.L for a highlight).
float light_facing(float cosine) {
#ifdef TWO_SIDED
  return abs(cosine);
#else
  return max(cosine, 0.0);
#endif
}

// The diffuse and specular light reaching the surface.
struct Light {
  vec3 diffuse;
  vec3 specular;
};

// normal: the surface normal; reflection: the view direction reflected
// about it (used for the highlights); shadow: how much of directional
// light 0 gets through.
Light gather_light(vec3 position, vec3 normal, vec3 reflection, float shadow, float view_factor) {
  Light light = Light(vec3(0.0), vec3(0.0));
#ifdef SPECULAR
  vec3 material_specular = specular_color() * view_factor;
#endif

#ifdef FIXED_LIGHTS
#ifndef DIRECTIONAL_COUNT
#define DIRECTIONAL_COUNT 3
#endif
#ifndef POINT_COUNT
#define POINT_COUNT 4
#endif
  LightRange directional = LightRange(DIRECTIONAL_COUNT, 0, 1);
  LightRange points = LightRange(POINT_COUNT, 0, 1);
#else
  LightRange directional = light_range(31u);
  LightRange points = light_range(30u);
#endif

#ifdef DLightDir
  for (int n = 0, i = directional.first; n < directional.count; n++, i += directional.step) {
    vec3 direction = DLightDir(i).xyz;  // toward the light
    float reach = i == 0 ? shadow : 1.0;
    vec3 diffuse = DLightColor(i).rgb * MatAmbientDiffuse.w * light_facing(dot(normal, direction));
    light.diffuse += scale_color(diffuse, reach);
#ifdef SPECULAR
    vec3 highlight = scale_color(DLightSpecularColor(i).rgb * material_specular,
                                 specular_power(highlight_cosine(reflection, direction), MatParam.x));
    highlight = scale_color(highlight, highlight_facing(normal, direction));
    light.specular += scale_color(highlight, reach);
#endif
  }
#endif

#ifdef PntLightPos
  for (int n = 0, i = points.first; n < points.count; n++, i += points.step) {
    vec3 to_light = PntLightPos(i).xyz - position;
    float distance2 = dot(to_light, to_light);
    vec3 direction = to_light * inversesqrt(distance2);
    // 1 / (a*d^2 + b*d + c)
    vec3 falloff = PntLightAtten(i).xyz;
    float attenuation = 1.0 / (falloff.x * distance2 + falloff.y * sqrt(distance2) + falloff.z);
    // Full inside the cone's cosine x, fading by (cosine - y) * z outside.
    vec4 cone = SpotLightCone(i);
    float cosine = dot(direction, SpotLightDir(i).xyz);
    float spot = cosine > cone.x ? 1.0 : (cosine - cone.y) * cone.z;
    if (spot > 0.0) {
      float strength = spot * attenuation;
      vec3 diffuse = scale_color(PntLightColor(i).rgb * MatAmbientDiffuse.w,
                                 light_facing(dot(normal, direction)));
      light.diffuse += scale_color(diffuse, strength);
#ifdef SPECULAR
      vec3 highlight = scale_color(PntLightSpecularColor(i).rgb * material_specular,
                                   specular_power(highlight_cosine(reflection, direction), MatParam.x));
      highlight = scale_color(highlight, highlight_facing(normal, direction));
      light.specular += scale_color(highlight, strength);
#endif
    }
  }
#endif
  return light;
}

#ifdef ENV_SPHERE
// A sphere map: the reflection in view space, onto the map's disc.
vec3 sphere_reflection(vec3 reflection) {
  vec3 view = reflection.x * matView(0).xyz + reflection.y * matView(1).xyz +
              reflection.z * matView(2).xyz;
#ifdef SPHERE_NORMALIZE
  view = normalize(view);
#endif
  vec3 r = vec3(-view.xy, 1.0 - view.z);
  vec2 uv = r.xy / (2.0 * length(r)) + 0.5;
  return sample_2d(spheretx0, uv).rgb;
}
#endif

void main() {
  vec3 position = v_position.xyz;
  vec3 normal = surface_normal();
  vec3 reflection = vec3(0.0);
  float view_factor = 1.0;
#ifdef SPECULAR
  vec3 to_eye = normalize(EyePos.xyz - position);
  reflection = 2.0 * dot(to_eye, normal) * normal - to_eye;
  view_factor = fresnel(normal, to_eye);
#endif
  float shadow = 1.0;
#ifdef SHADOW_REG
  if (UseProjTex) shadow = shadow_test(dlshd00, v_shadow, shadowMapSize.x);
#endif
  Light light = gather_light(position, normal, reflection, shadow, view_factor);

  vec4 surface = surface_color();

  vec3 lighting = AmbientColor.rgb + MatAmbientDiffuse.rgb + light.diffuse;
  vec4 color;
  // times(): a light's falloff can be infinite, and the console's 0 times
  // infinity is 0 (where the surface's color is 0).
  color.rgb = times(times(surface.rgb, v_color.rgb), lighting) + light.specular;
#ifdef TX1_OVER
  color.a = surface.a;  // the vertex alpha is in it
#else
  color.a = surface.a * v_color.a;
#endif
#ifdef ALPHA_SATURATE
  color.a = clamp(color.a, 0.0, 1.0);
#endif
#ifdef ENV_CUBE
  color.rgb += sample_cube(cubetx0, reflection).rgb * MatParam.y;
#endif
#ifdef ENV_SPHERE
  float strength = MatParam.y;
#ifdef REFLECTION_MASK
  strength = mix(strength, 0.7, reflection_mask());
#endif
#ifdef SPECULAR_ALPHA
  strength *= sample_2d(tx0, TX0_UV.xy).a;
#endif
  color.rgb += sphere_reflection(reflection) * strength * view_factor;
#endif
#ifdef PROJTEX_CLAMP
  if (UseProjTex) color.rgb = max(color.rgb, 0.0);
#endif

  if (!xe_alpha_test_passes(color.a)) discard;
  out_color = xe_color_output(color, 0);
  write_normal(v_normal.xyz);  // the geometry's, without the normal map
}
