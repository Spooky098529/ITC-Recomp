// Shader translation for the native renderer: Xenos microcode to SPIR-V,
// with the SDK's translator for now (docs/native-renderer.md), cached by
// the microcode's hash (XXH3-64 of the raw big-endian words, as the SDK
// names its dumps).

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace native {

enum class ShaderStage { kVertex, kPixel };

struct TranslatedShader {
  uint64_t hash = 0;
  std::vector<uint32_t> spirv;  // empty if translation failed
};

class ShaderCache {
 public:
  ShaderCache();
  ~ShaderCache();

  // ucode: the microcode as in guest memory (big-endian words). num_reg is
  // the stage's register count from SQ_PROGRAM_CNTL (value minus 1).
  // Returns null if the shader can't be translated.
  const TranslatedShader* Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                              uint32_t num_reg);

  uint32_t translated() const { return translated_; }
  uint32_t failed() const { return failed_; }

 private:
  struct State;
  std::unique_ptr<State> state_;
  uint32_t translated_ = 0, failed_ = 0;
};

}  // namespace native
