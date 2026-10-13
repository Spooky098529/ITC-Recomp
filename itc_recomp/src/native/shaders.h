// Shader translation for the native renderer: Xenos microcode to SPIR-V,
// with the SDK's translator for now (docs/native-renderer.md), cached by
// the microcode's hash (XXH3-64 of the raw big-endian words, as the SDK
// names its dumps).

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
  };
  // sq_program_cntl, sq_context_misc: the registers.
  Interface GetInterface(const uint8_t* vertex_ucode, uint32_t vertex_size_bytes,
                         const uint8_t* pixel_ucode, uint32_t pixel_size_bytes,
                         uint32_t sq_program_cntl, uint32_t sq_context_misc);

  // ucode: the microcode as in guest memory (big-endian words). num_reg is
  // the stage's register count from SQ_PROGRAM_CNTL (value minus 1).
  // Returns null if the shader can't be translated.
  TranslatedShader* Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                        uint32_t num_reg, const Interface& interface);

  uint32_t translated() const { return translated_; }
  uint32_t failed() const { return failed_; }

 private:
  struct State;
  std::unique_ptr<State> state_;
  uint32_t translated_ = 0, failed_ = 0;
};

}  // namespace native
