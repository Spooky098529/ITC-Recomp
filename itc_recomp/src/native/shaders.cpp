#include "shaders.h"
#include "frame_costs.h"

#include <cstdio>
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

REXCVAR_DEFINE_STRING(native_renderer_log_shader_ucode, "", "ITC",
                      "Log the microcode disassembly of this shader (hash in hex) when first "
                      "used (a debugging aid)");

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

using TranslationKey = std::tuple<ShaderStage, uint64_t, uint32_t, uint32_t, uint32_t, bool, bool>;

// How a vertex and a pixel shader connect depends on both microcodes and
// two registers.
struct InterfaceKey {
  uint64_t vertex_hash, pixel_hash;
  uint32_t program_cntl, context_misc;
  bool operator==(const InterfaceKey&) const = default;
};
struct InterfaceKeyHash {
  size_t operator()(const InterfaceKey& key) const {
    return size_t(key.vertex_hash ^ (key.pixel_hash * 0x9E3779B97F4A7C15ull) ^
                  (uint64_t(key.program_cntl) << 32 | key.context_misc));
  }
};

// The disk cache file (disk_cache.h): a header, then records, each a kind,
// a size and that many bytes (the key, then the value). Bump kDiskVersion
// whenever what a translation produces changes (the SDK's translator, the
// modification set in ShaderCache::Get, TranslatedShader's fields), or old
// translations would be used.
constexpr uint32_t kDiskMagic = 0x53435449;  // "ITCS"
constexpr uint32_t kDiskVersion = 1;
enum DiskRecord : uint32_t { kDiskTranslation = 1, kDiskInterface = 2 };

class DiskWriter {
 public:
  void U32(uint32_t v) { Bytes(&v, sizeof(v)); }
  void U64(uint64_t v) { Bytes(&v, sizeof(v)); }
  void Bytes(const void* data, size_t size) {
    auto* p = static_cast<const uint8_t*>(data);
    bytes.insert(bytes.end(), p, p + size);
  }
  std::vector<uint8_t> bytes;
};

class DiskReader {
 public:
  DiskReader(const uint8_t* data, size_t size) : p_(data), end_(data + size) {}
  uint32_t U32() { uint32_t v = 0; Bytes(&v, sizeof(v)); return v; }
  uint64_t U64() { uint64_t v = 0; Bytes(&v, sizeof(v)); return v; }
  void Bytes(void* out, size_t size) {
    if (size_t(end_ - p_) < size) {
      ok_ = false;
      return;
    }
    std::memcpy(out, p_, size);
    p_ += size;
  }
  // A count of items of `item_size` bytes still to come, or 0 if it can't be.
  uint32_t Count(size_t item_size) {
    uint32_t n = U32();
    if (n > size_t(end_ - p_) / item_size) {
      ok_ = false;
      return 0;
    }
    return n;
  }
  bool ok() const { return ok_; }
  bool done() const { return p_ == end_; }

 private:
  const uint8_t* p_;
  const uint8_t* end_;
  bool ok_ = true;
};

void WriteTranslationKey(DiskWriter& w, const TranslationKey& key) {
  auto [stage, hash, num_reg, interpolators, param_gen, point, rectangle] = key;
  w.U32(uint32_t(stage));
  w.U64(hash);
  w.U32(num_reg);
  w.U32(interpolators);
  w.U32(param_gen);
  w.U32(point);
  w.U32(rectangle);
}

TranslationKey ReadTranslationKey(DiskReader& r) {
  auto stage = ShaderStage(r.U32());
  uint64_t hash = r.U64();
  uint32_t num_reg = r.U32(), interpolators = r.U32(), param_gen = r.U32();
  bool point = r.U32() != 0, rectangle = r.U32() != 0;
  return {stage, hash, num_reg, interpolators, param_gen, point, rectangle};
}

void WriteTranslation(DiskWriter& w, const TranslatedShader& t) {
  w.U32(uint32_t(t.spirv.size()));
  w.Bytes(t.spirv.data(), t.spirv.size() * 4);
  for (uint64_t bits : t.float_constants) {
    w.U64(bits);
  }
  for (uint32_t bits : t.vertex_fetch_constants) {
    w.U32(bits);
  }
  w.U32(uint32_t(t.textures.size()));
  for (const ShaderTexture& texture : t.textures) {
    w.U32(texture.fetch_constant);
    w.U32(uint32_t(texture.view_type));
    w.U32(texture.is_signed);
  }
  w.U32(uint32_t(t.samplers.size()));
  for (const ShaderSampler& sampler : t.samplers) {
    w.U32(sampler.fetch_constant);
    w.U32(sampler.mag_filter);
    w.U32(sampler.min_filter);
    w.U32(sampler.mip_filter);
    w.U32(sampler.aniso_filter);
  }
  w.U32(uint32_t(t.vertex_strides.size()));
  for (const auto& [fetch_constant, stride] : t.vertex_strides) {
    w.U32(fetch_constant);
    w.U32(stride);
  }
  w.U32(t.writes_color_targets);
}

bool ReadTranslation(DiskReader& r, TranslatedShader& t) {
  t.spirv.resize(r.Count(4));
  r.Bytes(t.spirv.data(), t.spirv.size() * 4);
  for (uint64_t& bits : t.float_constants) {
    bits = r.U64();
  }
  for (uint32_t& bits : t.vertex_fetch_constants) {
    bits = r.U32();
  }
  t.textures.resize(r.Count(12));
  for (ShaderTexture& texture : t.textures) {
    texture.fetch_constant = r.U32();
    texture.view_type = VkImageViewType(r.U32());
    texture.is_signed = r.U32() != 0;
  }
  t.samplers.resize(r.Count(20));
  for (ShaderSampler& sampler : t.samplers) {
    sampler.fetch_constant = r.U32();
    sampler.mag_filter = r.U32();
    sampler.min_filter = r.U32();
    sampler.mip_filter = r.U32();
    sampler.aniso_filter = r.U32();
  }
  t.vertex_strides.resize(r.Count(8));
  for (auto& [fetch_constant, stride] : t.vertex_strides) {
    fetch_constant = r.U32();
    stride = r.U32();
  }
  t.writes_color_targets = r.U32();
  return r.ok() && r.done() && !t.spirv.empty();
}

}  // namespace

struct ShaderCache::State {
  explicit State(const ShaderFeatures& features)
      : features(features), translator(TranslatorFeatures(features), false, false, false) {}
  ShaderFeatures features;
  graphics::SpirvShaderTranslator translator;
  rex::string::StringBuffer disassembly;
  bool ucode_logged = false;  // --native_renderer_log_shader_ucode
  // Keyed by stage, microcode hash, register count and interface (the
  // modification depends on them).
  std::unordered_map<TranslationKey, std::unique_ptr<TranslatedShader>, TranslationKeyHash>
      translations;
  // Interfaces worked out so far (once per vertex/pixel shader pair, not
  // every draw).
  std::unordered_map<InterfaceKey, Interface, InterfaceKeyHash> interfaces;

  // The disk cache: translations from earlier sessions not used yet (the
  // value as stored), and the file new records are appended to.
  std::unordered_map<TranslationKey, std::vector<uint8_t>, TranslationKeyHash> stored;
  std::FILE* disk = nullptr;

  ~State() {
    if (disk) {
      std::fclose(disk);
    }
  }

  // The header: what translations depend on besides their keys.
  void WriteHeader(DiskWriter& w) const {
    w.U32(kDiskMagic);
    w.U32(kDiskVersion);
    w.U32(features.max_storage_buffer_range);
    w.U32(uint32_t(features.clip_distance) | uint32_t(features.cull_distance) << 1 |
          uint32_t(features.demote_to_helper_invocation) << 2);
  }

  void Append(DiskRecord kind, const DiskWriter& record) {
    if (!disk) {
      return;
    }
    uint32_t head[2] = {uint32_t(kind), uint32_t(record.bytes.size())};
    if (std::fwrite(head, sizeof(head), 1, disk) != 1 ||
        std::fwrite(record.bytes.data(), record.bytes.size(), 1, disk) != 1 ||
        std::fflush(disk) != 0) {
      REXLOG_WARN("native renderer: shader disk cache: write failed; not writing more");
      std::fclose(disk);
      disk = nullptr;
    }
  }
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
      // Memory export (eM writes) decides whether resolution scaling must
      // stay a whole number (see docs/native-renderer.md, "Resolution").
      REXLOG_INFO("native renderer: analyzed {} shader {:016X}: memexport eM {:02X}, {} stream "
                  "constants",
                  stage == ShaderStage::kVertex ? "vertex" : "pixel", hash,
                  shader->memexport_eM_written(), shader->memexport_stream_constants().size());
    }
    return shader.get();
  }
};

uint64_t MicrocodeHash(const uint8_t* ucode, uint32_t size_bytes) {
  return XXH3_64bits(ucode, size_bytes);
}

void ShaderCache::OpenDiskCache(const std::filesystem::path& file) {
  State& s = *state_;
  std::vector<uint8_t> data;
  if (std::FILE* in = std::fopen(file.c_str(), "rb")) {
    uint8_t buffer[65536];
    while (size_t n = std::fread(buffer, 1, sizeof(buffer), in)) {
      data.insert(data.end(), buffer, buffer + n);
    }
    std::fclose(in);
  }
  DiskWriter header;
  s.WriteHeader(header);
  // Keep the file if its header matches, up to its last whole record (the
  // game exits without warning, so a record may be cut short).
  size_t keep = 0;
  uint32_t translations = 0, interfaces = 0;
  if (data.size() >= header.bytes.size() &&
      std::equal(header.bytes.begin(), header.bytes.end(), data.begin())) {
    keep = header.bytes.size();
    while (data.size() - keep >= 8) {
      uint32_t kind, size;
      std::memcpy(&kind, &data[keep], 4);
      std::memcpy(&size, &data[keep + 4], 4);
      if (size > data.size() - keep - 8) {
        break;
      }
      DiskReader r(&data[keep + 8], size);
      if (kind == kDiskTranslation) {
        TranslationKey key = ReadTranslationKey(r);
        size_t key_size = 32;
        if (!r.ok()) {
          break;
        }
        const uint8_t* value = &data[keep + 8 + key_size];
        s.stored[key].assign(value, value + (size - key_size));
        translations++;
      } else if (kind == kDiskInterface) {
        InterfaceKey key{r.U64(), r.U64(), r.U32(), r.U32()};
        Interface interface;
        interface.interpolator_mask = r.U32();
        interface.param_gen_interpolator = r.U32();
        if (!r.ok() || !r.done()) {
          break;
        }
        s.interfaces[key] = interface;
        interfaces++;
      } else {
        break;
      }
      keep += 8 + size;
    }
  }
  std::error_code error;
  if (keep == 0) {
    s.disk = std::fopen(file.c_str(), "wb");
    if (s.disk && (std::fwrite(header.bytes.data(), header.bytes.size(), 1, s.disk) != 1 ||
                   std::fflush(s.disk) != 0)) {
      std::fclose(s.disk);
      s.disk = nullptr;
    }
  } else {
    if (keep < data.size()) {
      std::filesystem::resize_file(file, keep, error);
    }
    s.disk = error ? nullptr : std::fopen(file.c_str(), "ab");
  }
  if (!s.disk) {
    REXLOG_WARN("native renderer: shader disk cache: can't write {}", file.string());
  }
  REXLOG_INFO("native renderer: shader disk cache {}: {} translations and {} interfaces loaded{}",
              file.string(), translations, interfaces,
              keep == 0 && !data.empty() ? " (old or other device's file replaced)" : "");
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
  InterfaceKey key{vertex_hash, pixel_hash, sq_program_cntl, sq_context_misc};
  if (auto it = state_->interfaces.find(key); it != state_->interfaces.end()) {
    return it->second;
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
  state_->interfaces[key] = result;
  DiskWriter record;
  record.U64(key.vertex_hash);
  record.U64(key.pixel_hash);
  record.U32(key.program_cntl);
  record.U32(key.context_misc);
  record.U32(result.interpolator_mask);
  record.U32(result.param_gen_interpolator);
  state_->Append(kDiskInterface, record);
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
  if (const std::string& logged = REXCVAR_GET(native_renderer_log_shader_ucode);
      !logged.empty() && std::strtoull(logged.c_str(), nullptr, 16) == hash &&
      !state_->ucode_logged) {
    state_->ucode_logged = true;
    REXLOG_INFO("native renderer: shader {:016X}'s microcode:\n{}", hash,
                state_->Analyzed(stage, ucode, size_bytes, hash)->ucode_disassembly());
  }
  // From an earlier session (disk_cache.h).
  if (auto it = state_->stored.find(key); it != state_->stored.end()) {
    auto shader = std::make_unique<TranslatedShader>();
    shader->stage = stage;
    shader->hash = hash;
    DiskReader r(it->second.data(), it->second.size());
    bool read = ReadTranslation(r, *shader);
    state_->stored.erase(it);
    if (read) {
      from_disk_++;
      auto& result = state_->translations[key];
      result = std::move(shader);
      return result.get();
    }
    REXLOG_WARN("native renderer: shader disk cache: bad record for {:016X}; translating", hash);
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
  DiskWriter record;
  WriteTranslationKey(record, key);
  WriteTranslation(record, *result);
  state_->Append(kDiskTranslation, record);
  REXLOG_DEBUG("native renderer: translated {} shader {:016X}: {} SPIR-V words", stage_name, hash,
              result->spirv.size());
  return result.get();
}

}  // namespace native
