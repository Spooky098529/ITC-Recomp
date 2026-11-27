// What every ported game shader shares: the buffers it reads, the
// instructions that need more than a GLSL operator, texture and vertex
// fetches, and what happens before and after the shader's own code.
//
// A port defines XE_VERTEX or XE_PIXEL and XE_FLOAT_CONSTANTS, then includes
// this file. The bindings and the results are the translated shaders' (the
// SDK's SPIR-V translator), so a port can take a translation's place: the
// same descriptors, and the same value for every pixel and vertex
// (tools/shader_port/check compares them). docs/shaders.md explains the
// choices.
//
// Every arithmetic result is `precise`: the translator marks its operations
// NoContraction, so a multiply and an add stay two roundings, never a fused
// multiply-add.

// --- buffers -------------------------------------------------------------------
// What the draw gives its shaders besides the game's constants (the
// renderer's DrawConstants, draw_constants.h), read from vec4s at its byte
// offsets: std140 vec4s, unlike a block of named members with these offsets,
// can be written as HLSL (docs/d3d11-backend.md, step 4).
layout(set = 1, binding = 0, std140) uniform XeDrawConstants { vec4 xe_draw[11]; };
// 0: the flags (the bits below).
uint xe_draw_flags() { return floatBitsToUint(xe_draw[0].x); }
// 4: the alpha test's reference.
float xe_draw_alpha_test_reference() { return xe_draw[0].y; }
// 8 and 20: the position to Vulkan's clip space: xyz * scale + offset * w.
vec3 xe_draw_ndc_scale() { return vec3(xe_draw[0].zw, xe_draw[1].x); }
vec3 xe_draw_ndc_offset() { return xe_draw[1].yzw; }
// 32: each color target's exponent bias, as a factor.
float xe_draw_color_exp_bias(int target) { return xe_draw[2][target]; }
// 48: each texture stage's reader scale: 0 nothing; above 0 times it and
// clamped to 0-1; below 0 times its negation (a pass's image handed to its
// texture: what the resolve's exponent bias and a normalized format did,
// docs/pc-renderer.md, step 4).
float xe_draw_texture_scale(uint stage) { return xe_draw[3u + (stage >> 2u)][stage & 3u]; }

// The flags' bits.
const uint kXeFlagPrimitivePolygonal = 1u << 0;
const uint kXeFlagPrimitiveLine = 1u << 1;
const uint kXeFlagAlphaTestShift = 2u;  // 3 bits: the compare function (7 always)

// The float constant registers the shader reads, from its lowest to its
// highest (all 256 when it addresses them relative to the loop index): a
// constant's name is xe_fc[its register - the lowest].
#ifdef XE_VERTEX
layout(set = 1, binding = 1, std140) uniform XeFloatConstants { vec4 xe_fc[XE_FLOAT_CONSTANTS]; };
#else
layout(set = 1, binding = 2, std140) uniform XeFloatConstants { vec4 xe_fc[XE_FLOAT_CONSTANTS]; };
#endif
// The bool registers (128 a stage, a bit each: the vertex shader's, then the
// pixel shader's), then the integer registers for loops (16 a stage: count,
// start, step; the vertex shader's, then the pixel shader's).
layout(set = 1, binding = 3, std140) uniform XeBoolLoopConstants {
  uvec4 xe_bools[2];
  ivec4 xe_loops[32];
};

bool xe_bool(uint n) { return (xe_bools[n >> 7u][(n >> 5u) & 3u] & (1u << (n & 31u))) != 0u; }
uint xe_loop_count(uint n) { return uint(xe_loops[n].x); }
int xe_loop_start(uint n) { return xe_loops[n].y; }
int xe_loop_step(uint n) { return xe_loops[n].z; }

const float kXeFltMax = 3.40282347e+38;

// --- instructions --------------------------------------------------------------
// A multiply is 0 when either factor is 0, even infinity or NaN times 0
// (as on the console, which D3D9 shaders count on).
float xe_mul(float a, float b) {
  precise float product = a * b;
  return (a == 0.0 || b == 0.0) ? 0.0 : product;
}
vec2 xe_mul(vec2 a, vec2 b) { return vec2(xe_mul(a.x, b.x), xe_mul(a.y, b.y)); }
vec3 xe_mul(vec3 a, vec3 b) { return vec3(xe_mul(a.xy, b.xy), xe_mul(a.z, b.z)); }
vec4 xe_mul(vec4 a, vec4 b) { return vec4(xe_mul(a.xy, b.xy), xe_mul(a.zw, b.zw)); }

// Dot products: the products in order, each with the multiply's 0 rule.
float xe_dp2add(vec2 a, vec2 b, float c) {
  precise float sum = xe_mul(a.x, b.x) + xe_mul(a.y, b.y);
  precise float result = sum + c;
  return result;
}
float xe_dp3(vec3 a, vec3 b) {
  precise float sum = xe_mul(a.x, b.x) + xe_mul(a.y, b.y);
  sum = sum + xe_mul(a.z, b.z);
  return sum;
}
float xe_dp4(vec4 a, vec4 b) {
  precise float sum = xe_dp3(a.xyz, b.xyz) + xe_mul(a.w, b.w);
  return sum;
}

// max and min pick the first operand when the test holds, else the second.
float xe_max(float a, float b) { return a >= b ? a : b; }
vec2 xe_max(vec2 a, vec2 b) { return mix(b, a, greaterThanEqual(a, b)); }
vec3 xe_max(vec3 a, vec3 b) { return mix(b, a, greaterThanEqual(a, b)); }
vec4 xe_max(vec4 a, vec4 b) { return mix(b, a, greaterThanEqual(a, b)); }
float xe_min(float a, float b) { return a < b ? a : b; }
vec2 xe_min(vec2 a, vec2 b) { return mix(b, a, lessThan(a, b)); }
vec3 xe_min(vec3 a, vec3 b) { return mix(b, a, lessThan(a, b)); }
vec4 xe_min(vec4 a, vec4 b) { return mix(b, a, lessThan(a, b)); }

// The cube instruction, from the operand's x, z and w (written as the HLSL
// compiler does, src.zzxy): the face's t and s coordinates (from -major to
// +major), twice the major axis, and the face.
vec4 xe_cube(vec4 src) {
  vec3 d = vec3(src.z, src.w, src.x);  // x, y, z of the direction
  vec3 m = abs(d);
  precise vec4 r;
  if (m.z >= m.x && m.z >= m.y) {
    r = vec4(-d.y, d.z < 0.0 ? -d.x : d.x, 2.0 * d.z, d.z < 0.0 ? 5.0 : 4.0);
  } else if (m.y >= m.x) {
    r = vec4(d.y < 0.0 ? -d.z : d.z, d.x, 2.0 * d.y, d.y < 0.0 ? 3.0 : 2.0);
  } else {
    r = vec4(-d.y, d.x < 0.0 ? d.z : -d.z, 2.0 * d.x, d.x < 0.0 ? 1.0 : 0.0);
  }
  return r;
}

float xe_log_clamped(float a) {
  float r = log2(a);
  return r == -1.0 / 0.0 ? -kXeFltMax : r;
}
float xe_clamp_infinity(float r) {
  return r == -1.0 / 0.0 ? -kXeFltMax : (r == 1.0 / 0.0 ? kXeFltMax : r);
}
// rcpf, rsqf: +infinity to 0, -infinity to -0.
float xe_infinity_to_zero(float r) {
  if (r == 1.0 / 0.0) return 0.0;
  return r == -1.0 / 0.0 ? uintBitsToFloat(0x80000000u) : r;
}
float xe_muls_prev2(float a, float b, float ps) {
  bool multiply = !(ps == -kXeFltMax) && -abs(ps) >= -kXeFltMax && -abs(b) >= -kXeFltMax && b > 0.0;
  return multiply ? xe_mul(a, ps) : -kXeFltMax;
}

// --- textures ------------------------------------------------------------------
// A texture is one view (1D and 2D textures a 2D array's first layer, cube
// maps a cube) and a sampler, bound by the shader's sampler stage (its
// fetch constant): already in RGBA order and its format's own values (the
// views' swizzle; docs/pc-renderer.md, step 2). What the draw says of it
// besides: its reader scale.
vec4 xe_texture_result(vec4 texel, uint stage) {
  float reader_scale = xe_draw_texture_scale(stage & 31u);
  if (reader_scale > 0.0) {
    texel = clamp(texel * reader_scale, 0.0, 1.0);
  } else if (reader_scale < 0.0) {
    texel = texel * -reader_scale;
  }
  return texel;
}

// 1D and 2D textures are 2D arrays (a 1D texture is a row); offset is the
// instruction's texel offset plus the rounding offset (1.5/1024 of a texel);
// bias the instruction's LOD bias.
vec4 xe_tfetch2d(texture2DArray view, sampler s, uint stage, vec2 uv, vec2 offset, float bias,
                 bool computed_lod) {
  precise vec2 c = uv + offset / vec2(textureSize(sampler2DArray(view, s), 0).xy);
#ifdef XE_PIXEL
  if (computed_lod) {
  precise float gradient_scale = exp2(bias);
  precise vec2 dx = dFdxCoarse(c) * gradient_scale, dy = dFdyCoarse(c) * gradient_scale;
  return xe_texture_result(textureGrad(sampler2DArray(view, s), vec3(c, 0.0), dx, dy), stage);
  }
#endif
  return xe_texture_result(textureLod(sampler2DArray(view, s), vec3(c, 0.0), bias), stage);
}
vec4 xe_tfetch1d(texture2DArray view, sampler s, uint stage, float u, float offset, float bias,
                 bool computed_lod) {
  precise float c = u + offset / float(textureSize(sampler2DArray(view, s), 0).x);
#ifdef XE_PIXEL
  if (computed_lod) {
  precise float gradient_scale = exp2(bias);
  precise vec2 dx = vec2(dFdxCoarse(c) * gradient_scale, 0.0), dy = vec2(dFdyCoarse(c) * gradient_scale, 0.0);
  return xe_texture_result(textureGrad(sampler2DArray(view, s), vec3(c, 0.0, 0.0), dx, dy), stage);
  }
#endif
  return xe_texture_result(textureLod(sampler2DArray(view, s), vec3(c, 0.0, 0.0), bias), stage);
}
// A cube fetch takes what the cube instruction makes: s and t in 1 to 2,
// and the face (offset: s and t's as for 2D, then the face's).
vec4 xe_tfetchcube(textureCube view, sampler s, uint stage, vec3 stf, vec3 offset, float bias,
                   bool computed_lod) {
  precise vec2 st = stf.xy + offset.xy / vec2(textureSize(samplerCube(view, s), 0));
  st = st * 2.0 + -3.0;
  precise float face_value = stf.z + offset.z;
  uint face = uint(clamp(face_value, 0.0, 5.0));
  bool negative = (face & 1u) != 0u;
  float face_sign = negative ? -1.0 : 1.0;
  vec3 direction;
  if ((face >> 1u) == 0u) {
    direction = vec3(face_sign, -st.y, negative ? st.x : -st.x);
  } else if ((face >> 1u) == 1u) {
    direction = vec3(st.x, face_sign, negative ? -st.y : st.y);
  } else {
    direction = vec3(negative ? -st.x : st.x, -st.y, face_sign);
  }
#ifdef XE_PIXEL
  if (computed_lod) {
  precise float gradient_scale = exp2(bias);
  precise vec3 dx = dFdxCoarse(direction) * gradient_scale, dy = dFdyCoarse(direction) * gradient_scale;
  return xe_texture_result(textureGrad(samplerCube(view, s), direction, dx, dy), stage);
  }
#endif
  return xe_texture_result(textureLod(samplerCube(view, s), direction, bias), stage);
}

#ifdef XE_VERTEX
// --- vertex fetches ------------------------------------------------------------
// The game patches each vertex fetch instruction for the vertex declaration
// in use, so the same shader file runs with different layouts. A port
// takes each fetch's fields as specialization constants, which the renderer
// reads from the patched instruction: the fetch constant, the offset and
// stride in words, the format, the flags below and the exponent.
const uint kXeVfSigned = 1u, kXeVfInteger = 2u, kXeVfNoZero = 4u, kXeVfIndexRounded = 8u;

// 1 / (2^n - 1) and 1 / (2^(n-1) - 1) (or - 0.5 for no zero), as floats.
float xe_vf_scale(int width, bool is_signed, bool no_zero) {
  if (!is_signed) {
    switch (width) {
      case 2: return uintBitsToFloat(0x3EAAAAABu);    // 1/3
      case 8: return uintBitsToFloat(0x3B808081u);    // 1/255
      case 10: return uintBitsToFloat(0x3A802008u);   // 1/1023
      case 11: return uintBitsToFloat(0x3A001002u);   // 1/2047
      case 16: return uintBitsToFloat(0x37800080u);   // 1/65535
    }
    return uintBitsToFloat(0x2F800000u);              // 1/4294967295
  }
  if (no_zero) {
    switch (width) {
      case 2: return uintBitsToFloat(0x3F2AAAABu);    // 1/1.5
      case 8: return uintBitsToFloat(0x3C008081u);    // 1/127.5
      case 10: return uintBitsToFloat(0x3B002008u);   // 1/511.5
      case 11: return uintBitsToFloat(0x3A801002u);   // 1/1023.5
      case 16: return uintBitsToFloat(0x38000080u);   // 1/32767.5
    }
    return uintBitsToFloat(0x30000000u);              // 1/2147483647.5
  }
  switch (width) {
    case 2: return 1.0;
    case 8: return uintBitsToFloat(0x3C010204u);      // 1/127
    case 10: return uintBitsToFloat(0x3B004020u);     // 1/511
    case 11: return uintBitsToFloat(0x3A802008u);     // 1/1023
    case 16: return uintBitsToFloat(0x38000100u);     // 1/32767
  }
  return uintBitsToFloat(0x30000000u);                // 1/2147483647
}

// A packed component: extracted, converted, and normalized unless integer.
float xe_vf_packed(uint word, int at, int width, uint flags) {
  bool is_signed = (flags & kXeVfSigned) != 0u;
  float value = is_signed ? float(bitfieldExtract(int(word), at, width))
                          : float(bitfieldExtract(word, at, width));
  if ((flags & kXeVfInteger) != 0u) return value;
  bool no_zero = (flags & kXeVfNoZero) != 0u;
  precise float scaled = value * xe_vf_scale(width, is_signed, no_zero);
  if (is_signed) {
    scaled = no_zero ? scaled + 0.5 * xe_vf_scale(width, true, true) : max(scaled, -1.0);
  }
  return scaled;
}
// A 32-bit integer component.
float xe_vf_32(uint word, uint flags) {
  bool is_signed = (flags & kXeVfSigned) != 0u;
  float value = is_signed ? float(int(word)) : float(word);
  if ((flags & kXeVfInteger) != 0u) return value;
  bool no_zero = (flags & kXeVfNoZero) != 0u;
  precise float scaled = value * xe_vf_scale(32, is_signed, no_zero);
  if (is_signed && no_zero) scaled = scaled + uintBitsToFloat(0x2F800000u);  // 0.5 / 2147483647.5
  return scaled;
}

uint xe_vf_component_count(uint format) {
  switch (format) {
    case 6u: case 7u: case 26u: case 32u: case 35u: case 38u: return 4u;
    case 16u: case 17u: case 57u: return 3u;
    case 25u: case 31u: case 34u: case 37u: return 2u;
  }
  return 1u;
}

// The words a format needs for the components used (bit i: word i).
uint xe_vf_needed_words(uint format, uint used) {
  switch (format) {
    case 6u: case 7u: return 1u;
    case 16u: case 17u: return (used & 7u) != 0u ? 1u : 0u;
    case 25u: case 31u: return (used & 3u) != 0u ? 1u : 0u;
    case 26u: case 32u: return ((used & 3u) != 0u ? 1u : 0u) | ((used & 12u) != 0u ? 2u : 0u);
    case 33u: case 36u: return used & 1u;
    case 34u: case 37u: return used & 3u;
    case 57u: return used & 7u;
  }
  return used;
}

// Vertex fetch n's attribute: the raw words of the vertex's element (in the
// host's byte order), one input a fetch and a vertex of the rectangle
// (locations 3n to 3n + 2; a rectangle list reads its three vertices'
// through bindings at instance rate, other draws read the first). The
// renderer makes the vertex input state from the fetches' fields
// (passes.cpp, docs/pc-renderer.md, step 2).
uint xe_vf_slot = 0u;
#define XE_VF_IN(n) XE_VF_IN_(n)
#define XE_VF_IN_(n) \
  (xe_vf_slot == 0u ? xe_vf_in_##n##_0 : (xe_vf_slot == 1u ? xe_vf_in_##n##_1 : xe_vf_in_##n##_2))
// The fetch n with its fields; index (the register the instruction reads)
// is the vertex the invocation runs for, already applied by the vertex input.
#define xe_vfetch(n, fetch_constant, offset, stride, format, flags, exp_adjust, swizzle, index, \
                  used)                                                                         \
  xe_vfetch_words(XE_VF_IN(n), fetch_constant, format, flags, exp_adjust, swizzle, used)

// used: the result components the shader reads (a bit each).
// swizzle: 3 bits a component, what the fetch's result gives that
// component of the value the shader's instruction then swizzles as the file
// has it (0-3 a component of the fetched value, 4 zero, 5 one): the
// declaration's component order and defaults, as the game's patch of the
// instruction gives them (identity: 0x688, xyzw).
vec4 xe_vfetch_words(uvec4 words, uint fetch_constant, uint format, uint flags, int exp_adjust,
                     uint swizzle, uint used) {
  // The words are in the host's byte order (the vertex arena swaps them as
  // it copies them in, docs/pc-renderer.md, step 2).
  uint needed = xe_vf_needed_words(format, used);
  uvec4 w = uvec4(0u);
  for (int i = 0; i < 4; i++) {
    if ((needed & (1u << i)) != 0u) {
      w[i] = words[i];
    }
  }
  vec4 r = vec4(0.0);
  switch (format) {
    case 6u:  // 8_8_8_8
      for (int c = 0; c < 4; c++) r[c] = xe_vf_packed(w.x, 8 * c, 8, flags);
      break;
    case 7u:  // 2_10_10_10
      r = vec4(xe_vf_packed(w.x, 0, 10, flags), xe_vf_packed(w.x, 10, 10, flags),
               xe_vf_packed(w.x, 20, 10, flags), xe_vf_packed(w.x, 30, 2, flags));
      break;
    case 16u:  // 10_11_11
      r.xyz = vec3(xe_vf_packed(w.x, 0, 11, flags), xe_vf_packed(w.x, 11, 11, flags),
                   xe_vf_packed(w.x, 22, 10, flags));
      break;
    case 17u:  // 11_11_10
      r.xyz = vec3(xe_vf_packed(w.x, 0, 10, flags), xe_vf_packed(w.x, 10, 11, flags),
                   xe_vf_packed(w.x, 21, 11, flags));
      break;
    case 25u:  // 16_16
      r.xy = vec2(xe_vf_packed(w.x, 0, 16, flags), xe_vf_packed(w.x, 16, 16, flags));
      break;
    case 26u:  // 16_16_16_16
      r = vec4(xe_vf_packed(w.x, 0, 16, flags), xe_vf_packed(w.x, 16, 16, flags),
               xe_vf_packed(w.y, 0, 16, flags), xe_vf_packed(w.y, 16, 16, flags));
      break;
    case 31u:  // 16_16_FLOAT
      r.xy = unpackHalf2x16(w.x);
      break;
    case 32u:  // 16_16_16_16_FLOAT
      r = vec4(unpackHalf2x16(w.x), unpackHalf2x16(w.y));
      break;
    case 33u: case 34u: case 35u:  // 32, 32_32, 32_32_32_32
      for (int c = 0; c < 4; c++) r[c] = xe_vf_32(w[c], flags);
      break;
    default:  // 32_FLOAT, 32_32_FLOAT, 32_32_32_32_FLOAT, 32_32_32_FLOAT
      r = uintBitsToFloat(w);
      break;
  }
  // Components past the format's are 0.
  for (uint c = xe_vf_component_count(format); c < 4u; c++) r[c] = 0.0;
  precise vec4 result = r * ldexp(1.0, exp_adjust);
  if (swizzle == 0x688u) {
    return result;
  }
  vec4 swizzled;
  for (int c = 0; c < 4; c++) {
    uint s = (swizzle >> (3u * uint(c))) & 7u;
    swizzled[c] = s < 4u ? result[s] : (s == 5u ? 1.0 : 0.0);
  }
  return swizzled;
}

// --- the position -------------------------------------------------------------
// The position as the shader wrote it (the game's shaders write W as it is,
// X, Y and Z not divided by it), to Vulkan's clip space.
vec4 xe_host_position(vec4 guest) {
  precise vec3 xyz = guest.xyz * xe_draw_ndc_scale() + xe_draw_ndc_offset() * guest.w;
  return vec4(xyz, guest.w);
}
#endif

#ifdef XE_PIXEL
// --- the pixel's inputs and outputs --------------------------------------------
// The generated parameter (the pixel position, the facing in x's sign, and
// whether it's a line in z's), when the renderer asks for it in register n.
vec4 xe_param_gen() {
  float x = abs(floor(gl_FragCoord.x));
  if ((xe_draw_flags() & kXeFlagPrimitivePolygonal) != 0u && !gl_FrontFacing) {
    x = uintBitsToFloat(floatBitsToUint(x) ^ 0x80000000u);
  }
  float z = (xe_draw_flags() & kXeFlagPrimitiveLine) != 0u ? uintBitsToFloat(0x80000000u) : 0.0;
  return vec4(x, abs(floor(gl_FragCoord.y)), z, 0.0);
}

// The alpha test, on the first color target's alpha.
bool xe_alpha_test_passes(float alpha) {
  uint function = (xe_draw_flags() >> kXeFlagAlphaTestShift) & 7u;
  float reference = xe_draw_alpha_test_reference();
  if (function == 7u) return true;
  if (function == 5u) return !(alpha == reference);
  return (alpha < reference && (function & 1u) != 0u) ||
         (alpha == reference && (function & 2u) != 0u) ||
         (alpha > reference && (function & 4u) != 0u);
}
// A color as the shader wrote it, to what the render target takes: the
// target's exponent bias.
vec4 xe_color_output(vec4 color, int target) {
  precise vec4 c = color * xe_draw_color_exp_bias(target);
  return c;
}
#endif
