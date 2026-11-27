// From SBX-Recomp (src/renderer/vulkan/ at b128953), used with its author's OK
// (docs/shaders.md, "In this repository"); changed here: our namespace and
// paths, and the vertex fetches' remap and swizzle (docs/pc-renderer.md, step 2).

// The game's shaders ported to GLSL (src/native/glsl/game, made by
// tools/shader_port; docs/shaders.md): looking a shader up by its microcode,
// and fitting the port to a draw.
//
// A port gives the same values as the SDK's translation of the same
// microcode, with the same descriptors (tools/shader_port/check compares
// them), so ShaderCache can hand it out in the translation's place.

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "shaders.h"

namespace native {

struct GamePortTexture {
  uint32_t fetch_constant;
  bool cube;  // else a 2D array (1D and 2D textures)
};

struct GamePortSampler {
  uint32_t fetch_constant;
  uint32_t mag_filter, min_filter, mip_filter, aniso_filter;
};

struct GamePort {
  ShaderStage stage;
  // Pixel shaders: MicrocodeHash of the microcode. Vertex shaders: the same
  // with each vertex fetch's layout fields zeroed (LayoutFreeHash), as the
  // game patches those for the vertex declaration in use.
  uint64_t hash;
  uint32_t microcode_words;
  const uint32_t* spirv;
  size_t spirv_words;
  const char* found_in;  // where tools/shader_port found it first
  // The vertex fetch instructions, in order (instruction index: words
  // 3n to 3n+2 of the microcode).
  std::array<uint16_t, 16> vertex_fetches;
  uint32_t vertex_fetch_count;
  // The descriptors, in the translator's order.
  std::array<GamePortTexture, 32> textures;
  uint32_t texture_count;
  std::array<GamePortSampler, 16> samplers;
  uint32_t sampler_count;
  // The float constants read (a bit each, c0-c255), the interpolators
  // written (vertex shaders) or read (pixel shaders), and the color targets
  // written: what the renderer took from the SDK translator's analysis.
  std::array<uint64_t, 4> float_constants;
  uint32_t interpolators;
  uint32_t color_targets;
};

// The port of this microcode (as in guest memory, big-endian words), or
// null. hash: its MicrocodeHash.
const GamePort* FindGamePort(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                             uint64_t hash);

// The microcode's hash with the port's vertex fetches' layout fields zeroed.
uint64_t LayoutFreeHash(const GamePort& port, const uint8_t* ucode, uint32_t size_bytes);

// The port's SPIR-V for a draw: the vertex fetches' fields from the (patched)
// microcode, the interpolators' locations, and the rest of the interface.
// Its textures and samplers go in `shader`. False if it can't be made.
// How many samples the hand-written post effects take, as multiples of the
// game's (1 is the game's): the motion blur's taps along the motion, the
// bloom's Gaussian blur taps per texel. A shader without the setting
// ignores it.
struct PostQuality {
  uint32_t motion_blur = 1;
  uint32_t bloom = 1;
};

// A vertex shader as its GLSL is found and fitted (docs/pc-renderer.md,
// step 2): the file's microcode (the shader object's: the graphics
// library's patch of it for the declaration no longer runs) with each
// vertex fetch's layout fields (fetch constant, format, signedness,
// exponent, stride and offset) as made for the bound declaration
// (graphics::shader_patch::FetchFromDeclaration), and `swizzles`: each
// fetch's instruction index and what its result gives each component of
// the value the file's destination swizzle then reads (xe_vfetch's
// swizzle).
struct VertexFetchRemap {
  std::vector<uint8_t> ucode;
  std::vector<std::pair<uint32_t, uint32_t>> swizzles;
};
// From the shader object's microcode, its fetch records (+60 the count,
// +64 a word a fetch: bits 0-11 its instruction index), its fetches' file
// instructions (+192, big-endian) and each fetch's instruction as made for
// the declaration (host words). False when a fetch can't be fitted.
bool FitVertexFetches(const uint8_t* ucode, uint32_t size_bytes, uint32_t fetch_count,
                      const uint32_t* fetch_records, const uint8_t* original_fetches,
                      const std::array<uint32_t, 3>* made, VertexFetchRemap& out);

bool MakeFromPort(const GamePort& port, const uint8_t* ucode, uint32_t size_bytes,
                  uint32_t interpolator_mask, uint32_t param_gen_interpolator,
                  bool rectangle_list, TranslatedShader& shader,
                  const PostQuality& post_quality = {},
                  const std::vector<std::pair<uint32_t, uint32_t>>* swizzles = nullptr);

uint32_t GamePortCount();

}  // namespace native
