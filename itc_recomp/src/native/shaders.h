// Shader translation for the native renderer: Xenos microcode to SPIR-V,
// with the SDK's translator for now (docs/native-renderer.md), cached by
// the microcode's hash (XXH3-64 of the raw big-endian words, as the SDK
// names its dumps).

#pragma once

#include <filesystem>
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
  // The translator binds a signed and an unsigned view of a texture
  // separately, and picks per component by the system constants' signs.
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
  std::vector<uint32_t> spirv;
  // The float constants the shader reads, one bit per register (c0-c255).
  // The translated shader takes only these, packed in order.
  std::array<uint64_t, 4> float_constants{};
  // The vertex fetch constants it reads (0-95), one bit each: the vertex
  // buffers it fetches from.
  std::array<uint32_t, 3> vertex_fetch_constants{};
  // Its texture descriptors (set 2 or 3): images, then samplers.
  std::vector<ShaderTexture> textures;
  std::vector<ShaderSampler> samplers;
  // The vertex buffers it fetches from: fetch constant and stride in words.
  std::vector<std::pair<uint32_t, uint32_t>> vertex_strides;
  // The render targets the (pixel) shader writes, a bit each.
  uint32_t writes_color_targets = 0;
  VkShaderModule module = VK_NULL_HANDLE;  // created by the renderer
};

struct ShaderFeatures {
  uint32_t max_storage_buffer_range = UINT32_MAX;
  bool clip_distance = false;
  bool cull_distance = false;
  bool demote_to_helper_invocation = false;
};

// The cache's key for a shader's microcode (as in guest memory).
uint64_t MicrocodeHash(const uint8_t* ucode, uint32_t size_bytes);

class ShaderCache {
 public:
  explicit ShaderCache(const ShaderFeatures& features);
  ~ShaderCache();

  // How a vertex and a pixel shader connect (as the plugin works it out):
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
  // sq_program_cntl, sq_context_misc: the registers. The hashes are each
  // microcode's XXH3-64 (MicrocodeHash), worked out once a draw by the
  // caller: hashing the microcode was ~5% of the render thread when each
  // lookup did it again.
  Interface GetInterface(const uint8_t* vertex_ucode, uint32_t vertex_size_bytes,
                         uint64_t vertex_hash, const uint8_t* pixel_ucode,
                         uint32_t pixel_size_bytes, uint64_t pixel_hash,
                         uint32_t sq_program_cntl, uint32_t sq_context_misc);

  // ucode: the microcode as in guest memory (big-endian words). num_reg is
  // the stage's register count from SQ_PROGRAM_CNTL (value minus 1).
  // Returns null if the shader can't be translated. hash: MicrocodeHash.
  TranslatedShader* Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                        uint64_t hash, uint32_t num_reg, const Interface& interface);

  uint32_t translated() const { return translated_; }
  uint32_t failed() const { return failed_; }
  // Translations taken from the disk cache instead of translated.
  uint32_t from_disk() const { return from_disk_; }

  // Loads earlier sessions' translations and interfaces from `file` and
  // appends new ones to it (see disk_cache.h).
  void OpenDiskCache(const std::filesystem::path& file);

 private:
  struct State;
  std::unique_ptr<State> state_;
  uint32_t translated_ = 0, failed_ = 0, from_disk_ = 0;
};

}  // namespace native
