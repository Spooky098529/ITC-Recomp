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

namespace {

graphics::SpirvShaderTranslator::Features TranslatorFeatures(const ShaderFeatures& ours) {
  // Only what our device enables; the rest is off (no memory export, no
  // fragment shader interlock: render targets are ordinary attachments).
  graphics::SpirvShaderTranslator::Features features(false);
  features.spirv_version = spv::Spv_1_5;
  features.max_storage_buffer_range = ours.max_storage_buffer_range;
  features.full_draw_index_uint32 = true;
  features.clip_distance = ours.clip_distance;
  features.cull_distance = ours.cull_distance;
  features.image_view_format_swizzle = true;
  features.demote_to_helper_invocation = ours.demote_to_helper_invocation;
  return features;
}

}  // namespace

struct ShaderCache::State {
  explicit State(const ShaderFeatures& features)
      : translator(TranslatorFeatures(features), false, false, false) {}
  graphics::SpirvShaderTranslator translator;
  rex::string::StringBuffer disassembly;
  // Keyed by stage, microcode hash and register count (the modification
  // depends on it).
  std::map<std::tuple<ShaderStage, uint64_t, uint32_t>, std::unique_ptr<TranslatedShader>>
      translations;
  std::map<std::pair<ShaderStage, uint64_t>, std::unique_ptr<graphics::SpirvShader>> shaders;
};

ShaderCache::ShaderCache(const ShaderFeatures& features)
    : state_(std::make_unique<State>(features)) {}
ShaderCache::~ShaderCache() = default;

TranslatedShader* ShaderCache::Get(ShaderStage stage, const uint8_t* ucode,
                                         uint32_t size_bytes, uint32_t num_reg) {
  uint64_t hash = XXH3_64bits(ucode, size_bytes);
  auto key = std::make_tuple(stage, hash, num_reg);
  if (auto it = state_->translations.find(key); it != state_->translations.end()) {
    return it->second->spirv.empty() ? nullptr : it->second.get();
  }
  const char* stage_name = stage == ShaderStage::kVertex ? "vertex" : "pixel";
  REXLOG_DEBUG("native renderer: translating {} shader {:016X}, {} bytes, num_reg {}",
               stage_name, hash, size_bytes, num_reg);
  auto& result = state_->translations[key];
  result = std::make_unique<TranslatedShader>();
  result->stage = stage;
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
  // All 16 interpolators on both sides, so every vertex shader's outputs
  // match every pixel shader's inputs (exact masks: later).
  graphics::SpirvShaderTranslator::Modification modification(
      stage == ShaderStage::kVertex
          ? state_->translator.GetDefaultVertexShaderModification(registers)
          : state_->translator.GetDefaultPixelShaderModification(registers));
  if (stage == ShaderStage::kVertex) {
    modification.vertex.interpolator_mask = 0xFFFF;
  } else {
    modification.pixel.interpolator_mask = 0xFFFF;
  }
  graphics::Shader::Translation* translation =
      shader->GetOrCreateTranslation(modification.value);
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
  const auto& constants = shader->constant_register_map();
  for (uint32_t i = 0; i < 4; i++) {
    result->float_constants[i] = constants.float_bitmap[i];
  }
  for (uint32_t i = 0; i < 3; i++) {
    result->vertex_fetch_constants[i] = constants.vertex_fetch_bitmap[i];
  }
  // Declared as in the translator's texture fetch code: 1D and 2D as 2D
  // arrays, 3D (or stacked) as 3D, cube as cube.
  for (const auto& binding : shader->GetTextureBindingsAfterTranslation()) {
    VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    if (binding.dimension == graphics::xenos::FetchOpDimension::k3DOrStacked) {
      view_type = VK_IMAGE_VIEW_TYPE_3D;
    } else if (binding.dimension == graphics::xenos::FetchOpDimension::kCube) {
      view_type = VK_IMAGE_VIEW_TYPE_CUBE;
    }
    result->textures.push_back({binding.fetch_constant, view_type});
  }
  result->sampler_count = uint32_t(shader->GetSamplerBindingsAfterTranslation().size());
  translated_++;
  REXLOG_DEBUG("native renderer: translated {} shader {:016X}: {} SPIR-V words", stage_name, hash,
              result->spirv.size());
  return result.get();
}

}  // namespace native
