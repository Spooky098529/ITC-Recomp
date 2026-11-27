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

#extension GL_EXT_scalar_block_layout : require

// --- buffers -------------------------------------------------------------------
// The system constants: the renderer's SystemConstants struct (the SDK's
// SpirvShaderTranslator::SystemConstants). Only the fields ports read, at
// their offsets; shaders.cpp static_asserts the offsets.
layout(set = 1, binding = 0, scalar) uniform XeSystemConstants {
  layout(offset = 0) uint flags;
  layout(offset = 4) uint vertex_index_load_address;
  layout(offset = 8) uint vertex_index_endian;
  layout(offset = 12) uint line_loop_closing_index;
  layout(offset = 16) int vertex_base_index;
  layout(offset = 124) vec3 ndc_scale;
  // SBX's own: the translator's point_vertex_diameter_min, which no port
  // reads (ports don't make point sprites) and the renderer doesn't set
  // otherwise; the 2D sprite shaders' mode (passes.cpp), its low byte:
  // 1 menu texture filtering (boot/menu_filter.glsl); 2 a distance-field
  // font, its spread in 1/256 sheet pixels from bit 8 (boot/font_sdf.glsl).
  layout(offset = 136) uint sbx_sprite_mode;
  layout(offset = 140) vec3 ndc_offset;
  layout(offset = 176) uint texture_swizzled_signs[8];
  layout(offset = 272) float alpha_test_reference;
  layout(offset = 288) vec4 color_exp_bias;
  // Each texture fetch constant's reader scale: 0 nothing; above 0 times it
  // and clamped to 0-1; below 0 times its negation (a pass's image handed to
  // its texture: what the resolve's exponent bias and a normalized format
  // did, docs/pc-renderer.md, step 4).
  layout(offset = 304) float texture_scale[32];
  layout(offset = 496) uint vertex_index_min;
  layout(offset = 500) uint vertex_index_max;
} xe_sys;

// The flags' bits (SpirvShaderTranslator::kSysFlag_*).
const uint kXeFlagPrimitiveVertexIndexLoad = 1u << 1;
const uint kXeFlagPrimitiveVertexIndexLoad32Bit = 1u << 2;
const uint kXeFlagXYDividedByW = 1u << 8;
const uint kXeFlagZDividedByW = 1u << 9;
const uint kXeFlagWNotReciprocal = 1u << 10;
const uint kXeFlagPrimitivePolygonal = 1u << 11;
const uint kXeFlagPrimitiveLine = 1u << 12;
const uint kXeFlagAlphaTestShift = 16u;  // 3 bits: the compare function
const uint kXeFlagConvertColor0ToGamma = 1u << 19;
// SBX's own (bit 31, past the translator's): the alpha test is a dither
// instead, passing where the alpha is over a per-pixel threshold that the
// reference moves each frame (passes.cpp, the smooth-alpha solids' copies).
const uint kXeFlagSbxDitheredAlpha = 1u << 31;
// SBX's own (bit 30): the draw takes no AO (passes.cpp: the mirrors, the
// sky); in the normals target its view distance is the sign bit alone.
const uint kXeFlagSbxNoAo = 1u << 30;

// The float constants the shader reads, packed in register order (all 256
// when it addresses them relative to the loop index).
#ifdef XE_VERTEX
layout(set = 1, binding = 1, std140) uniform XeFloatConstants { vec4 xe_fc[XE_FLOAT_CONSTANTS]; };
#else
layout(set = 1, binding = 2, std140) uniform XeFloatConstants { vec4 xe_fc[XE_FLOAT_CONSTANTS]; };
#endif
// 256 bool constants (a bit each), then 32 loop constants.
layout(set = 1, binding = 3, std140) uniform XeBoolLoopConstants {
  uvec4 xe_bools[2];
  uvec4 xe_loops[8];
};
// The fetch constants: a texture is 6 words, a vertex buffer 2.
layout(set = 1, binding = 4, std140) uniform XeFetchConstants { uvec4 xe_fetch[48]; };

bool xe_bool(uint n) { return (xe_bools[n >> 7u][(n >> 5u) & 3u] & (1u << (n & 31u))) != 0u; }
uint xe_loop_constant(uint n) { return xe_loops[n >> 2u][n & 3u]; }
uint xe_fetch_word(uint n) { return xe_fetch[n >> 2u][n & 3u]; }

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
// The Xbox's piecewise linear gamma curve, both ways, as the translator
// computes it.
float xe_gamma_to_linear(float gamma) {
  gamma = clamp(gamma, 0.0, 1.0);
  float scale = gamma >= 96.0 / 255.0 ? (gamma >= 192.0 / 255.0 ? 8.0 / 1024.0 : 4.0 / 1024.0)
                                      : (gamma >= 64.0 / 255.0 ? 2.0 / 1024.0 : 1.0 / 1024.0);
  float offset = gamma >= 96.0 / 255.0 ? (gamma >= 192.0 / 255.0 ? -1024.0 : -256.0)
                                       : (gamma >= 64.0 / 255.0 ? -64.0 : 0.0);
  precise float linear = (gamma * (255.0 * 1024.0)) * scale + offset;
  linear = linear + trunc(linear * scale);
  linear = linear * (1.0 / 1023.0);
  return linear;
}
vec3 xe_linear_to_gamma(vec3 linear) {
  linear = clamp(linear, 0.0, 1.0);
  bvec3 at_least_2 = greaterThanEqual(linear, vec3(128.0 / 1023.0));
  bvec3 at_least_3 = greaterThanEqual(linear, vec3(512.0 / 1023.0));
  bvec3 at_least_1 = greaterThanEqual(linear, vec3(64.0 / 1023.0));
  vec3 scale = mix(mix(vec3(1023.0), vec3(1023.0 / 2.0), at_least_1),
                   mix(vec3(1023.0 / 4.0), vec3(1023.0 / 8.0), at_least_3), at_least_2);
  vec3 offset = mix(mix(vec3(0.0), vec3(32.0 / 255.0), at_least_1),
                    mix(vec3(64.0 / 255.0), vec3(128.0 / 255.0), at_least_3), at_least_2);
  precise vec3 gamma = trunc(linear * scale) * (1.0 / 255.0) + offset;
  return gamma;
}

// A texture fetch's result from the unsigned and the signed view's samples:
// each component's sign mode (from the system constants: signed, biased
// x * 2 - 1, or gamma), then the exponent bias. The fetch constant's
// swizzle is in the image views.
vec4 xe_texture_result(vec4 unsigned_sample, vec4 signed_sample, uint fetch_constant) {
  float reader_scale = xe_sys.texture_scale[fetch_constant & 31u];
  if (reader_scale > 0.0) {
    unsigned_sample = clamp(unsigned_sample * reader_scale, 0.0, 1.0);
  } else if (reader_scale < 0.0) {
    unsigned_sample = unsigned_sample * -reader_scale;
  }
  uint signs = xe_sys.texture_swizzled_signs[fetch_constant >> 2u] >> (8u * (fetch_constant & 3u));
  // The translator takes the exponent from the fetch constant's word 4,
  // bits 13-18 (the hardware's is in word 3); kept for the same result.
  int exponent = bitfieldExtract(int(xe_fetch_word(fetch_constant * 6u + 4u)), 13, 6);
  precise vec4 result;
  for (int c = 0; c < 4; c++) {
    uint sign = (signs >> (2 * c)) & 3u;
    precise float value = unsigned_sample[c];
    if (sign == 1u) value = signed_sample[c];
    else if (sign == 2u) value = value * 2.0 + -1.0;
    else if (sign == 3u) value = xe_gamma_to_linear(value);
    result[c] = value * ldexp(1.0, exponent);
  }
  return result;
}

// The LOD bias from the fetch constant (word 4, bits 12-21, 1/32 steps) and
// the instruction.
float xe_texture_lod(uint fetch_constant, float instruction_bias) {
  precise float lod = float(bitfieldExtract(int(xe_fetch_word(fetch_constant * 6u + 4u)), 12, 10)) *
                      (1.0 / 32.0);
  lod = lod + instruction_bias;
  return lod;
}

// The size of a 2D or cube texture, from the fetch constant's word 2.
vec2 xe_texture_size_2d(uint fetch_constant) {
  uint word = xe_fetch_word(fetch_constant * 6u + 2u);
  return vec2(float(bitfieldExtract(word, 0, 13) + 1u), float(bitfieldExtract(word, 13, 13) + 1u));
}

// 1D and 2D textures are 2D arrays (a 1D texture is a row); offset is the
// instruction's texel offset plus the rounding offset (1.5/1024 of a texel).
vec4 xe_tfetch2d(texture2DArray unsigned_view, texture2DArray signed_view, sampler s,
                 uint fetch_constant, vec2 uv, vec2 offset, float bias, bool computed_lod) {
  precise vec2 c = uv + offset / xe_texture_size_2d(fetch_constant);
  float lod = xe_texture_lod(fetch_constant, bias);
#ifdef XE_PIXEL
  if (computed_lod) {
  precise float gradient_scale = exp2(lod);
  precise vec2 dx = dFdxCoarse(c) * gradient_scale, dy = dFdyCoarse(c) * gradient_scale;
  return xe_texture_result(textureGrad(sampler2DArray(unsigned_view, s), vec3(c, 0.0), dx, dy),
                           textureGrad(sampler2DArray(signed_view, s), vec3(c, 0.0), dx, dy),
                           fetch_constant);
  }
#endif
  return xe_texture_result(textureLod(sampler2DArray(unsigned_view, s), vec3(c, 0.0), lod),
                           textureLod(sampler2DArray(signed_view, s), vec3(c, 0.0), lod),
                           fetch_constant);
}
vec4 xe_tfetch1d(texture2DArray unsigned_view, texture2DArray signed_view, sampler s,
                 uint fetch_constant, float u, float offset, float bias, bool computed_lod) {
  // A 1D size is word 2's low 24 bits; the row is at v = 0.
  precise float c = u + offset / float(bitfieldExtract(xe_fetch_word(fetch_constant * 6u + 2u), 0, 24) + 1u);
  float lod = xe_texture_lod(fetch_constant, bias);
#ifdef XE_PIXEL
  if (computed_lod) {
  precise float gradient_scale = exp2(lod);
  precise vec2 dx = vec2(dFdxCoarse(c) * gradient_scale, 0.0), dy = vec2(dFdyCoarse(c) * gradient_scale, 0.0);
  return xe_texture_result(textureGrad(sampler2DArray(unsigned_view, s), vec3(c, 0.0, 0.0), dx, dy),
                           textureGrad(sampler2DArray(signed_view, s), vec3(c, 0.0, 0.0), dx, dy),
                           fetch_constant);
  }
#endif
  return xe_texture_result(textureLod(sampler2DArray(unsigned_view, s), vec3(c, 0.0, 0.0), lod),
                           textureLod(sampler2DArray(signed_view, s), vec3(c, 0.0, 0.0), lod),
                           fetch_constant);
}
// A cube fetch takes what the cube instruction makes: s and t in 1 to 2,
// and the face (offset: s and t's as for 2D, then the face's).
vec4 xe_tfetchcube(textureCube unsigned_view, textureCube signed_view, sampler s,
                   uint fetch_constant, vec3 stf, vec3 offset, float bias, bool computed_lod) {
  precise vec2 st = stf.xy + offset.xy / xe_texture_size_2d(fetch_constant);
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
  float lod = xe_texture_lod(fetch_constant, bias);
#ifdef XE_PIXEL
  if (computed_lod) {
  precise float gradient_scale = exp2(lod);
  precise vec3 dx = dFdxCoarse(direction) * gradient_scale, dy = dFdyCoarse(direction) * gradient_scale;
  return xe_texture_result(textureGrad(samplerCube(unsigned_view, s), direction, dx, dy),
                           textureGrad(samplerCube(signed_view, s), direction, dx, dy),
                           fetch_constant);
  }
#endif
  return xe_texture_result(textureLod(samplerCube(unsigned_view, s), direction, lod),
                           textureLod(samplerCube(signed_view, s), direction, lod),
                           fetch_constant);
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
// The position as the shader wrote it, to Vulkan's clip space.
vec4 xe_host_position(vec4 guest) {
  precise float w = (xe_sys.flags & kXeFlagWNotReciprocal) != 0u ? guest.w : 1.0 / guest.w;
  precise vec2 xy = (xe_sys.flags & kXeFlagXYDividedByW) != 0u ? guest.xy * w : guest.xy;
  precise float z = (xe_sys.flags & kXeFlagZDividedByW) != 0u ? guest.z * w : guest.z;
  precise vec3 xyz = vec3(xy, z) * xe_sys.ndc_scale + xe_sys.ndc_offset * w;
  return vec4(xyz, w);
}
#endif

#ifdef XE_PIXEL
// --- the pixel's inputs and outputs --------------------------------------------
// The generated parameter (the pixel position, the facing in x's sign, and
// whether it's a line in z's), when the renderer asks for it in register n.
vec4 xe_param_gen() {
  float x = abs(floor(gl_FragCoord.x));
  if ((xe_sys.flags & kXeFlagPrimitivePolygonal) != 0u && !gl_FrontFacing) {
    x = uintBitsToFloat(floatBitsToUint(x) ^ 0x80000000u);
  }
  float z = (xe_sys.flags & kXeFlagPrimitiveLine) != 0u ? uintBitsToFloat(0x80000000u) : 0.0;
  return vec4(x, abs(floor(gl_FragCoord.y)), z, 0.0);
}

// The alpha test, on the first color target's alpha.
bool xe_alpha_test_passes(float alpha) {
  if ((xe_sys.flags & kXeFlagSbxDitheredAlpha) != 0u) {
    // Interleaved gradient noise (Jimenez), shifted each frame.
    vec2 p = floor(gl_FragCoord.xy) + 5.588238 * xe_sys.alpha_test_reference;
    float threshold = fract(52.9829189 * fract(dot(p, vec2(0.06711056, 0.00583715))));
    return alpha > threshold;
  }
  uint function = (xe_sys.flags >> kXeFlagAlphaTestShift) & 7u;
  float reference = xe_sys.alpha_test_reference;
  if (function == 7u) return true;
  if (function == 5u) return !(alpha == reference);
  return (alpha < reference && (function & 1u) != 0u) ||
         (alpha == reference && (function & 2u) != 0u) ||
         (alpha > reference && (function & 4u) != 0u);
}
// A color as the shader wrote it, to what the render target takes: the
// target's exponent bias, and gamma if the target is gamma but its format
// isn't.
//
// SBX's light target (--xegtao; passes.cpp, "View targets"): render target
// 0's color goes to location 2 as well, blended the same way, when the
// renderer says the draw only adds light (XE_LIGHT: the models' unlit
// material, additive blends); otherwise the part of the color a shader put
// in xe_light_extra (light it added, such as course_projected's projector,
// the neon underglow and headlights), with the same alpha, so the blend
// fades what's under it as it fades the color. The AO then darkens the
// color but not that light. Dropped where the pipeline has no light target.
layout(constant_id = 900) const bool XE_LIGHT = false;
layout(location = 2) out vec4 xe_out_light;
// SBX's normals target (location 1; glsl/materials/normals_out.glsl): no
// normal, and the fragment's view distance, unless the material writes its
// normal after this (write_normal). Dropped where the pipeline has no
// normals target or doesn't write it.
layout(location = 1) out uvec2 xe_out_normal;
// The view distance as the normals target holds it: its float bits, or,
// for a draw that takes no AO (kXeFlagSbxNoAo), the sign bit alone (no
// distance: the AO's depth there is the game's; xegtao_depth.comp).
uint xe_normal_distance() {
  return (xe_sys.flags & kXeFlagSbxNoAo) != 0u ? 0x80000000u
                                               : floatBitsToUint(1.0 / gl_FragCoord.w);
}
vec3 xe_light_extra = vec3(0.0);
vec3 xe_color_output_rgb(vec3 rgb, int target) {
  rgb *= xe_sys.color_exp_bias[target];
  if ((xe_sys.flags & (kXeFlagConvertColor0ToGamma << uint(target))) != 0u) {
    rgb = xe_linear_to_gamma(rgb);
  }
  return rgb;
}
vec4 xe_color_output(vec4 color, int target) {
  precise vec4 c = color * xe_sys.color_exp_bias[target];
  if ((xe_sys.flags & (kXeFlagConvertColor0ToGamma << uint(target))) != 0u) {
    c.rgb = xe_linear_to_gamma(c.rgb);
  }
  if (target == 0) {
    xe_out_normal = uvec2(0u, xe_normal_distance());
    if (XE_LIGHT) {
      xe_out_light = c;
    } else {
      // The light's share of the output, so gamma stays exact.
      vec3 rest = xe_color_output_rgb(max(color.rgb - xe_light_extra, 0.0), target);
      xe_out_light = vec4(max(c.rgb - rest, 0.0), c.a);
    }
  }
  return c;
}
#endif
