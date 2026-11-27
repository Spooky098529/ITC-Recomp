// The game's shaders for the native renderer: each microcode found in the
// table of its GLSL (game_shader_ports.h, from tools/shader_port; see
// docs/shaders.md) and fitted to the draw (shader_ports.h), by the
// microcode's hash (XXH3-64 of the raw big-endian words). A shader the
// table lacks stops the program, named (docs/pc-renderer.md).

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "vulkan_functions.h"

namespace native {

enum class ShaderStage { kVertex, kPixel };

// A texture the shader samples: its fetch constant and how it's declared
// (the descriptor's image view type).
struct ShaderTexture {
  uint32_t fetch_constant;
  VkImageViewType view_type;
  // A signed and an unsigned view of a texture are bound separately, and
  // the shader picks per component by the system constants' signs.
  bool is_signed;
};

// A sampler the shader uses: its fetch constant and the fetch instruction's
// filter overrides (xenos::TextureFilter and AnisoFilter values).
struct ShaderSampler {
  uint32_t fetch_constant;
  uint32_t mag_filter, min_filter, mip_filter, aniso_filter;
};

struct TranslatedShader {
  ShaderStage stage;
  uint64_t hash = 0;
  // The hash its GLSL is found by (for vertex shaders the layout-free one,
  // glsl/game/shaders.json's lookup_hash).
  uint64_t port_hash = 0;
  std::vector<uint32_t> spirv;
  // The float constants the shader reads, one bit per register (c0-c255).
  // The shader takes only these, packed in order.
  std::array<uint64_t, 4> float_constants{};
  // The vertex fetch constants it reads (0-95), one bit each: the vertex
  // buffers it fetches from.
  std::array<uint32_t, 3> vertex_fetch_constants{};
  // A vertex shader's fetches, in its order (fetch n reads its vertex's
  // element from the attributes at locations 3n to 3n + 2, the raw words in
  // the guest's byte order): the fetch constant (its stream), where the
  // element starts in the vertex and the vertex's size, in words, and how
  // many words the element's format has. The renderer makes the vertex
  // input state from them (docs/pc-renderer.md, step 2).
  struct VertexFetch {
    uint32_t fetch_constant;
    int32_t offset_words;
    uint32_t stride_words;
    uint32_t words;
  };
  std::vector<VertexFetch> vertex_fetches;
  // Made for a rectangle list: an instance a rectangle, each fetch read
  // for its three vertices through bindings at instance rate.
  bool rectangle_list = false;
  // Its texture descriptors (set 2 or 3): images, then samplers.
  std::vector<ShaderTexture> textures;
  std::vector<ShaderSampler> samplers;
  // The render targets the (pixel) shader writes, a bit each.
  uint32_t writes_color_targets = 0;
  VkShaderModule module = VK_NULL_HANDLE;  // created by the renderer
};

// The cache's key for a shader's microcode (as in guest memory).
uint64_t MicrocodeHash(const uint8_t* ucode, uint32_t size_bytes);

class ShaderCache {
 public:
  ShaderCache();
  ~ShaderCache();

  // How a vertex and a pixel shader connect:
  // the interpolators the vertex shader writes and the pixel shader reads,
  // and the one the pixel shader gets its generated parameter (PsParamGen)
  // in, if any. An interpolator the pixel shader reads that isn't in the
  // mask reads as zero; with all of them on, it was undefined (flickering
  // garbage).
  struct Interface {
    uint32_t interpolator_mask = 0;
    uint32_t param_gen_interpolator = UINT32_MAX;
    bool param_gen_point = false;
    // Vertex shaders: drawing a rectangle list, expanded in the vertex
    // shader into a two-triangle strip per rectangle (the translator's
    // kRectangleListAsTriangleStrip: each invocation runs the guest shader
    // for the rectangle's 3 vertices and makes the 4th).
    bool rectangle_list = false;
  };
  // sq_program_cntl, sq_context_misc: the registers (the generated
  // parameter's). The hashes are each microcode's XXH3-64 (MicrocodeHash),
  // worked out once a draw by the caller: hashing the microcode was ~5% of
  // the render thread when each lookup did it again. remap: the vertex
  // shader's, as for Get.
  Interface GetInterface(const uint8_t* vertex_ucode, uint32_t vertex_size_bytes,
                         uint64_t vertex_hash, const uint8_t* pixel_ucode,
                         uint32_t pixel_size_bytes, uint64_t pixel_hash,
                         uint32_t sq_program_cntl, uint32_t sq_context_misc,
                         const struct VertexFetchRemap* remap = nullptr);

  // ucode: the microcode as in guest memory (big-endian words); hash: its
  // MicrocodeHash. remap: a vertex shader's file microcode and fetch fields
  // as its GLSL is found and fitted by (shader_ports.h). Stops the program,
  // named, for a shader the table lacks or a draw its GLSL can't make (point
  // sprites).
  TranslatedShader* Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                        uint64_t hash, const Interface& interface,
                        const struct VertexFetchRemap* remap = nullptr);

  // Shaders made from their GLSL so far.
  uint32_t ported() const { return ported_; }

 private:
  struct State;
  std::unique_ptr<State> state_;
  uint32_t ported_ = 0;
};

}  // namespace native
