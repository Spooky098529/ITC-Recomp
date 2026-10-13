#include "shaders.h"

#include <cstring>
#include <map>
#include <tuple>
#include <rex/graphics/pipeline/shader/spirv.h>
#include <rex/graphics/pipeline/shader/spirv_translator.h>
#include <rex/hash.h>
#include <rex/logging.h>
#include <rex/string/buffer.h>

// The translator reads this GPU setting, which the emulated GPU's plugin
// defines (and registers) in its own copy; ours is a plain storage function,
// empty, so translated shaders aren't dumped.
std::string& FLAGS_dump_shaders_storage_() {
  static std::string dump_shaders;
  return dump_shaders;
}

namespace native {

namespace graphics = rex::graphics;

struct ShaderCache::State {
  graphics::SpirvShaderTranslator translator{graphics::SpirvShaderTranslator::Features(true),
                                             false, false, false};
  rex::string::StringBuffer disassembly;
  // Keyed by stage, microcode hash and register count (the modification
  // depends on it).
  std::map<std::tuple<ShaderStage, uint64_t, uint32_t>, std::unique_ptr<TranslatedShader>>
      translations;
  std::map<std::pair<ShaderStage, uint64_t>, std::unique_ptr<graphics::SpirvShader>> shaders;
};

ShaderCache::ShaderCache() : state_(std::make_unique<State>()) {}
ShaderCache::~ShaderCache() = default;

const TranslatedShader* ShaderCache::Get(ShaderStage stage, const uint8_t* ucode,
                                         uint32_t size_bytes, uint32_t num_reg) {
  uint64_t hash = XXH3_64bits(ucode, size_bytes);
  auto key = std::make_tuple(stage, hash, num_reg);
  if (auto it = state_->translations.find(key); it != state_->translations.end()) {
    return it->second->spirv.empty() ? nullptr : it->second.get();
  }
  auto& result = state_->translations[key];
  result = std::make_unique<TranslatedShader>();
  result->hash = hash;

  auto& shader = state_->shaders[{stage, hash}];
  if (!shader) {
    std::vector<uint32_t> words(size_bytes / 4);
    std::memcpy(words.data(), ucode, words.size() * 4);
    shader = std::make_unique<graphics::SpirvShader>(
        stage == ShaderStage::kVertex ? graphics::xenos::ShaderType::kVertex
                                      : graphics::xenos::ShaderType::kPixel,
        hash, words.data(), words.size(), std::endian::big);
    shader->AnalyzeUcode(state_->disassembly);
  }
  uint32_t registers = shader->GetDynamicAddressableRegisterCount(num_reg);
  uint64_t modification =
      stage == ShaderStage::kVertex
          ? state_->translator.GetDefaultVertexShaderModification(registers)
          : state_->translator.GetDefaultPixelShaderModification(registers);
  graphics::Shader::Translation* translation = shader->GetOrCreateTranslation(modification);
  if (!translation->is_translated() && !state_->translator.TranslateAnalyzedShader(*translation)) {
    failed_++;
    REXLOG_ERROR("native renderer: couldn't translate {} shader {:016X}",
                 stage == ShaderStage::kVertex ? "vertex" : "pixel", hash);
    return nullptr;
  }
  if (!translation->is_valid()) {
    failed_++;
    REXLOG_ERROR("native renderer: {} shader {:016X} is invalid",
                 stage == ShaderStage::kVertex ? "vertex" : "pixel", hash);
    return nullptr;
  }
  const std::vector<uint8_t>& binary = translation->translated_binary();
  result->spirv.resize(binary.size() / 4);
  std::memcpy(result->spirv.data(), binary.data(), result->spirv.size() * 4);
  translated_++;
  return result.get();
}

}  // namespace native
