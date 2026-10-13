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
};

struct TranslatedShader {
  ShaderStage stage;
  uint64_t hash = 0;
  std::vector<uint32_t> spirv;
  // The float constants the shader reads, one bit per register (c0-c255).
  // The translated shader takes only these, packed in order.
  std::array<uint64_t, 4> float_constants{};
  // Its texture descriptors (set 2 or 3): images, then samplers.
  std::vector<ShaderTexture> textures;
  uint32_t sampler_count = 0;
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

  // ucode: the microcode as in guest memory (big-endian words). num_reg is
  // the stage's register count from SQ_PROGRAM_CNTL (value minus 1).
  // Returns null if the shader can't be translated.
  TranslatedShader* Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                        uint32_t num_reg);

  uint32_t translated() const { return translated_; }
  uint32_t failed() const { return failed_; }

 private:
  struct State;
  std::unique_ptr<State> state_;
  uint32_t translated_ = 0, failed_ = 0;
};

}  // namespace native
