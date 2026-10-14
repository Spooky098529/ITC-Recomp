#include "shaders.h"
#include "frame_costs.h"

#include <cstring>
#include <map>
#include <tuple>
#include <unordered_map>
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

// Hashes for the caches' keys (looked up twice a draw; ordered maps of
// tuples were ~3% of the render thread). The microcode hash is already
// well mixed.
struct TranslationKeyHash {
  size_t operator()(
      const std::tuple<ShaderStage, uint64_t, uint32_t, uint32_t, uint32_t, bool, bool>& key) const {
    auto [stage, hash, num_reg, interpolators, param_gen, point, rectangle] = key;
    uint64_t rest = uint64_t(num_reg) | (uint64_t(interpolators) << 8) |
                    (uint64_t(param_gen) << 24) | (uint64_t(point) << 40) |
                    (uint64_t(rectangle) << 41) | (uint64_t(stage) << 42);
    return size_t(hash ^ (rest * 0x9E3779B97F4A7C15ull));
  }
};
struct ShaderKeyHash {
  size_t operator()(const std::pair<ShaderStage, uint64_t>& key) const {
    return size_t(key.second ^ uint64_t(key.first));
  }
};

}  // namespace

struct ShaderCache::State {
  explicit State(const ShaderFeatures& features)
      : translator(TranslatorFeatures(features), false, false, false) {}
  graphics::SpirvShaderTranslator translator;
  rex::string::StringBuffer disassembly;
  // Keyed by stage, microcode hash, register count and interface (the
  // modification depends on them).
  std::unordered_map<std::tuple<ShaderStage, uint64_t, uint32_t, uint32_t, uint32_t, bool, bool>,
                     std::unique_ptr<TranslatedShader>, TranslationKeyHash>
      translations;
  std::unordered_map<std::pair<ShaderStage, uint64_t>, std::unique_ptr<graphics::SpirvShader>,
                     ShaderKeyHash>
      shaders;

  // The shader, its microcode analyzed (made on first use).
  graphics::SpirvShader* Analyzed(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                                  uint64_t hash) {
    auto& shader = shaders[{stage, hash}];
    if (!shader) {
      std::vector<uint32_t> words(size_bytes / 4);
      std::memcpy(words.data(), ucode, words.size() * 4);
      shader = std::make_unique<graphics::SpirvShader>(
          stage == ShaderStage::kVertex ? graphics::xenos::ShaderType::kVertex
                                        : graphics::xenos::ShaderType::kPixel,
          hash, words.data(), words.size(), std::endian::big);
      shader->AnalyzeUcode(disassembly);
    }
    return shader.get();
  }
};

uint64_t MicrocodeHash(const uint8_t* ucode, uint32_t size_bytes) {
  return XXH3_64bits(ucode, size_bytes);
}

ShaderCache::ShaderCache(const ShaderFeatures& features)
    : state_(std::make_unique<State>(features)) {}
ShaderCache::~ShaderCache() = default;

ShaderCache::Interface ShaderCache::GetInterface(const uint8_t* vertex_ucode,
                                                 uint32_t vertex_size_bytes,
                                                 uint64_t vertex_hash,
                                                 const uint8_t* pixel_ucode,
                                                 uint32_t pixel_size_bytes,
                                                 uint64_t pixel_hash,
                                                 uint32_t sq_program_cntl,
                                                 uint32_t sq_context_misc) {
  // As the plugin's command processor: what the vertex shader writes and the
  // pixel shader reads, without the generated parameter's interpolator.
  Interface result;
  if (!pixel_ucode) {
    return result;
  }
  graphics::SpirvShader* vertex =
      state_->Analyzed(ShaderStage::kVertex, vertex_ucode, vertex_size_bytes, vertex_hash);
  graphics::SpirvShader* pixel =
      state_->Analyzed(ShaderStage::kPixel, pixel_ucode, pixel_size_bytes, pixel_hash);
  graphics::reg::SQ_PROGRAM_CNTL program_cntl;
  program_cntl.value = sq_program_cntl;
  graphics::reg::SQ_CONTEXT_MISC context_misc;
  context_misc.value = sq_context_misc;
  uint32_t param_gen = UINT32_MAX;
  result.interpolator_mask = vertex->writes_interpolators() &
                             pixel->GetInterpolatorInputMask(program_cntl, context_misc, param_gen);
  result.param_gen_interpolator = param_gen;
  return result;
}

TranslatedShader* ShaderCache::Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                                   uint64_t hash, uint32_t num_reg, const Interface& interface) {
  auto key = std::make_tuple(stage, hash, num_reg, interface.interpolator_mask,
                             interface.param_gen_interpolator, interface.param_gen_point,
                             stage == ShaderStage::kVertex && interface.rectangle_list);
  if (auto it = state_->translations.find(key); it != state_->translations.end()) {
    return it->second->spirv.empty() ? nullptr : it->second.get();
  }
  ScopedCost cost(g_frame_costs.shader_ns);
  g_frame_costs.shaders++;
  const char* stage_name = stage == ShaderStage::kVertex ? "vertex" : "pixel";
  REXLOG_DEBUG("native renderer: translating {} shader {:016X}, {} bytes, num_reg {}",
               stage_name, hash, size_bytes, num_reg);
  auto& result = state_->translations[key];
  result = std::make_unique<TranslatedShader>();
  result->stage = stage;
  result->hash = hash;

  graphics::SpirvShader* shader = state_->Analyzed(stage, ucode, size_bytes, hash);
  uint32_t registers = shader->GetDynamicAddressableRegisterCount(num_reg);
  // The interface as the plugin's pipeline cache sets it (without centroid
  // sampling and the depth modes, for now).
  graphics::SpirvShaderTranslator::Modification modification(
      stage == ShaderStage::kVertex
          ? state_->translator.GetDefaultVertexShaderModification(registers)
          : state_->translator.GetDefaultPixelShaderModification(registers));
  if (stage == ShaderStage::kVertex) {
    modification.vertex.interpolator_mask = interface.interpolator_mask;
    if (interface.rectangle_list) {
      modification.vertex.host_vertex_shader_type =
          graphics::Shader::HostVertexShaderType::kRectangleListAsTriangleStrip;
    }
  } else {
    modification.pixel.interpolator_mask = interface.interpolator_mask;
    if (interface.param_gen_interpolator < 16) {
      modification.pixel.param_gen_enable = 1;
      modification.pixel.param_gen_interpolator = interface.param_gen_interpolator;
      modification.pixel.param_gen_point = interface.param_gen_point;
    }
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
    result->textures.push_back({binding.fetch_constant, view_type, bool(binding.is_signed)});
  }
  for (const auto& binding : shader->GetSamplerBindingsAfterTranslation()) {
    result->samplers.push_back({binding.fetch_constant, uint32_t(binding.mag_filter),
                                uint32_t(binding.min_filter), uint32_t(binding.mip_filter),
                                uint32_t(binding.aniso_filter)});
  }
  result->writes_color_targets = shader->writes_color_targets();
  for (const auto& binding : shader->vertex_bindings()) {
    result->vertex_strides.emplace_back(binding.fetch_constant, binding.stride_words);
  }
  translated_++;
  REXLOG_DEBUG("native renderer: translated {} shader {:016X}: {} SPIR-V words", stage_name, hash,
              result->spirv.size());
  return result.get();
}

}  // namespace native
