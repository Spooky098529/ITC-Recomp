// What the hand-written material shaders share: texture sampling and the
// game's lighting model.
//
// compile_game.py puts a header in front of each shader made from these
// files (see variants.txt): xe_interface.glsl (the buffers and helpers every
// game shader uses), the shader's constants by their D3D names (EyePos,
// DLightDir(i), ...), and its textures by their sampler names (tx0, cubetx0,
// ...). A texture name stands for its views, its sampler and its fetch
// constant, so it can be handed to the sampling macros below as is.

// --- textures --------------------------------------------------------------------
// Every fetch the game's shaders make is offset by 1.5/1024 of a texel (the
// console's rounding), and picks its level from the screen-space gradients.
const float kTexelRounding = 0.0014648438;

#define sample_2d(texture, uv) \
  xe_tfetch2d(texture, uv, vec2(kTexelRounding), 0.0, true)

// A cube map seen along a direction. The console's cube fetch takes the
// face and the coordinates on it, which the cube instruction makes from the
// direction.
vec3 cube_face_coords(vec3 direction) {
  vec4 cube = xe_cube(direction.zzxy);
  return vec3(cube.yx / abs(cube.z) + 1.5, cube.w);
}
#define sample_cube(texture, direction) \
  xe_tfetchcube(texture, cube_face_coords(direction), vec3(kTexelRounding, kTexelRounding, 0.0), \
                0.0, true)

// --- light-space textures --------------------------------------------------------
// Light space coordinates as the shaders test them: inside the frustum
// when x and y are within -1 to 1 and the depth within 0 to 1, after
// dividing by w; the texture coordinates have y down.
bool in_light_frustum(vec4 coords, out vec2 uv, out float depth) {
  vec3 p = coords.xyz * (1.0 / coords.w);
  uv = vec2(p.x * 0.5 + 0.5, 1.0 - (p.y * 0.5 + 0.5));
  depth = p.z;
  return abs(p.x) < 1.0 && abs(p.y) < 1.0 && p.z < 1.0 && p.z > 0.0;
}

// A shadow map's 16 taps around the point, in texels.
const vec2 kShadowTaps[16] = vec2[](
    vec2(0.460800, -2.403328), vec2(-0.708608, -0.888832), vec2(1.925120, -1.630208),
    vec2(0.791552, -2.091008), vec2(-0.936960, -0.435200), vec2(-1.236992, -1.226752),
    vec2(1.760256, -0.771072), vec2(-0.051200, 0.107520), vec2(-1.241088, 1.167360),
    vec2(-1.899520, -0.004096), vec2(0.662528, 0.181248), vec2(0.700416, 0.279552),
    vec2(0.431104, 0.830464), vec2(-1.482752, 2.145280), vec2(2.423808, 0.549888),
    vec2(0.555008, 1.526784));

// The share of a shadow map's taps whose stored depth is less than the
// point's: 1 outside the map's frustum. map_size is the map's width in
// texels. Sampled at every pixel (call it outside branches), so the
// texture level is defined.
float shadow_taps(texture2DArray unsigned_view, texture2DArray signed_view, sampler s,
                  uint fetch_constant, vec4 coords, float map_size) {
  vec2 uv;
  float depth;
  bool inside = in_light_frustum(coords, uv, depth);
  float texel = 1.0 / map_size;
  float count = 0.0;
  for (int k = 0; k < 16; k++) {
    float stored = xe_tfetch2d(unsigned_view, signed_view, s, fetch_constant,
                               uv + kShadowTaps[k] * texel, vec2(kTexelRounding), 0.0, true).x;
    if (stored < depth) count += 1.0;
  }
  return inside ? count / 16.0 : 1.0;
}
#define shadow_test(map, coords, map_size) shadow_taps(map, coords, map_size)

// --- lighting --------------------------------------------------------------------
// The game lights a material with up to 3 directional lights (loop constant
// 31 holds how many) and up to 8 point lights (loop constant 30), each point
// light with a spot cone. The material's colors:
//   MatAmbientDiffuse  ambient (rgb) and how much diffuse light it takes (w)
//   MatSpecularColor   specular color (rgb)
//   MatParam           specular exponent (x), environment map strength (y)
// Constants a shader doesn't use don't exist in it, so their use is behind
// the feature defines.

// A light loop's range: how many lights, the first one's index, and the step.
struct LightRange {
  int count;
  int first;
  int step;
};

LightRange light_range(uint loop_constant) {
  uint value = xe_loop_constant(loop_constant);
  return LightRange(int(value & 0xFFu), int((value >> 8u) & 0xFFu),
                    bitfieldExtract(int(value), 16, 8));
}

// The specular highlight: pow(cosine, shininess), the cosine kept off 0 by
// the shader's floor. log2(0) is held at the most negative float, so a
// shininess of 0 still gives 1. With a floor of 0, a negative shininess on
// a cosine of 0 gives infinity.
#ifndef SPECULAR_FLOOR
#define SPECULAR_FLOOR 1e-5
#endif
float specular_power(float cosine, float shininess) {
  float log_cosine = max(log2(max(cosine, SPECULAR_FLOOR)), -3.40282347e38);
  return exp2(shininess * log_cosine);
}

// Multiplies where 0 times anything is 0, even infinity (a highlight's
// power or a light's falloff can be infinite). The console multiplies this
// way; plain GLSL would give NaN. times() for two numbers or two colors,
// scale_color() for a color and a factor.
float times(float a, float b) { return (a == 0.0 || b == 0.0) ? 0.0 : a * b; }
vec3 scale_color(vec3 color, float factor) {
  if (factor == 0.0) return vec3(0.0);
  return mix(color * factor, vec3(0.0), equal(color, vec3(0.0)));
}
vec3 times(vec3 a, vec3 b) {
  return vec3(times(a.x, b.x), times(a.y, b.y), times(a.z, b.z));
}
