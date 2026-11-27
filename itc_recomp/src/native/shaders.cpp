#include "shaders.h"

#include <cstdio>
#include <cstdlib>
#include <format>
#include <string>
#include <tuple>
#include <unordered_map>

#include "shader_ports.h"
#include "base/hash.h"
#include "base/log.h"

namespace native {

namespace {

// The caches' keys' hashes (looked up twice a draw; ordered maps of tuples
// were ~3% of the render thread). The microcode hash is already well mixed.
using ShaderKey = std::tuple<ShaderStage, uint64_t, uint32_t, uint32_t, bool>;
struct ShaderKeyHash {
  size_t operator()(const ShaderKey& key) const {
    auto [stage, hash, interpolators, param_gen, rectangle] = key;
    uint64_t rest = uint64_t(interpolators) | (uint64_t(param_gen) << 16) |
                    (uint64_t(rectangle) << 40) | (uint64_t(stage) << 41);
    return size_t(hash ^ (rest * 0x9E3779B97F4A7C15ull));
  }
};

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

const char* StageName(ShaderStage stage) {
  return stage == ShaderStage::kVertex ? "vertex" : "pixel";
}

// A shader the renderer can't draw: named, its microcode written into
// ITC_DUMP_UNPORTED if set (for tools/shader_port), and the program
// stopped (docs/pc-renderer.md: a missing shader is found at once).
[[noreturn]] void Missing(ShaderStage stage, uint64_t hash, const uint8_t* ucode,
                          uint32_t size_bytes, const char* why) {
  ITC_LOG_ERROR("native renderer: {} shader {:016X}: {}; stopping (docs/shaders.md)",
                StageName(stage), hash, why);
  if (const char* dir = std::getenv("ITC_DUMP_UNPORTED"); dir && *dir) {
    std::string path =
        std::format("{}/{}_{:016X}.bin", dir, stage == ShaderStage::kVertex ? "vs" : "ps", hash);
    if (std::FILE* out = std::fopen(path.c_str(), "wb")) {
      std::fwrite(ucode, 1, size_bytes, out);
      std::fclose(out);
    }
  }
  std::abort();
}

// The vertex fetch constants a ported vertex shader reads, from its fetches'
// first words in `ucode` (the file's layout, the game's fields): constant
// index (bits 20-24) x 3 + select (bits 25-26).
std::array<uint32_t, 3> PortVertexFetchConstants(const GamePort& port, const uint8_t* ucode) {
  std::array<uint32_t, 3> bits{};
  for (uint32_t i = 0; i < port.vertex_fetch_count; i++) {
    const uint8_t* p = ucode + size_t(port.vertex_fetches[i]) * 12;
    uint32_t word = uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
    uint32_t constant = (word >> 20 & 0x1F) * 3 + (word >> 25 & 3);
    if (constant < 96) {
      bits[constant >> 5] |= 1u << (constant & 31);
    }
  }
  return bits;
}

}  // namespace

struct ShaderCache::State {
  std::unordered_map<ShaderKey, std::unique_ptr<TranslatedShader>, ShaderKeyHash> shaders;
  // Interfaces worked out so far (once per vertex/pixel shader pair, not
  // every draw).
  std::unordered_map<InterfaceKey, Interface, InterfaceKeyHash> interfaces;
};

uint64_t MicrocodeHash(const uint8_t* ucode, uint32_t size_bytes) {
  return base::Hash64(ucode, size_bytes);
}

ShaderCache::ShaderCache() : state_(std::make_unique<State>()) {}
ShaderCache::~ShaderCache() = default;

ShaderCache::Interface ShaderCache::GetInterface(const uint8_t* vertex_ucode,
                                                 uint32_t vertex_size_bytes,
                                                 uint64_t vertex_hash,
                                                 const uint8_t* pixel_ucode,
                                                 uint32_t pixel_size_bytes,
                                                 uint64_t pixel_hash,
                                                 uint32_t sq_program_cntl,
                                                 uint32_t sq_context_misc,
                                                 const VertexFetchRemap* remap) {
  Interface result;
  if (!pixel_ucode) {
    return result;
  }
  InterfaceKey key{vertex_hash, pixel_hash, sq_program_cntl, sq_context_misc};
  if (auto it = state_->interfaces.find(key); it != state_->interfaces.end()) {
    return it->second;
  }
  const GamePort* vertex = FindGamePort(
      ShaderStage::kVertex, remap ? remap->ucode.data() : vertex_ucode, vertex_size_bytes,
      vertex_hash);
  const GamePort* pixel =
      FindGamePort(ShaderStage::kPixel, pixel_ucode, pixel_size_bytes, pixel_hash);
  if (!vertex || !pixel) {
    return result;  // Get stops the program, naming it
  }
  // The pixel shader reads its first registers' worth of interpolators (up
  // to 16), less the one the generated parameter goes in (SQ_PROGRAM_CNTL
  // bit 18; SQ_CONTEXT_MISC bits 8-15 its position).
  uint32_t reads = pixel->interpolators;
  uint32_t position = (sq_context_misc >> 8) & 0xFF;
  if ((sq_program_cntl >> 18 & 1) && position < 32 && (reads >> position & 1)) {
    reads &= ~(1u << position);
    result.param_gen_interpolator = position;
  }
  result.interpolator_mask = vertex->interpolators & reads;
  state_->interfaces[key] = result;
  return result;
}

TranslatedShader* ShaderCache::Get(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                                   uint64_t hash, const Interface& interface,
                                   const VertexFetchRemap* remap) {
  bool rectangle_list = stage == ShaderStage::kVertex && interface.rectangle_list;
  ShaderKey key{stage, hash, interface.interpolator_mask, interface.param_gen_interpolator,
                rectangle_list};
  if (auto it = state_->shaders.find(key); it != state_->shaders.end()) {
    return it->second.get();
  }
  const uint8_t* port_ucode = remap ? remap->ucode.data() : ucode;
  const GamePort* port = FindGamePort(stage, port_ucode, size_bytes, hash);
  if (!port) {
    Missing(stage, hash, port_ucode, size_bytes,
            stage == ShaderStage::kVertex && !remap
                ? "no GLSL (its vertex fetches couldn't be matched to the file's)"
                : "no GLSL in the table");
  }
  if (interface.param_gen_point) {
    Missing(stage, hash, port_ucode, size_bytes, "drawn as point sprites, which no GLSL makes");
  }
  auto shader = std::make_unique<TranslatedShader>();
  shader->stage = stage;
  shader->hash = hash;
  shader->port_hash = port->hash;
  if (!MakeFromPort(*port, port_ucode, size_bytes, interface.interpolator_mask,
                    interface.param_gen_interpolator, rectangle_list, *shader, {},
                    remap ? &remap->swizzles : nullptr)) {
    Missing(stage, hash, port_ucode, size_bytes, "its GLSL couldn't be fitted to the draw");
  }
  // What the microcode reads, from the table; the vertex fetch constants as
  // the game patched them.
  shader->float_constants = port->float_constants;
  for (uint32_t r = 0; r < 256; r++) {
    if (port->float_constants[r >> 6] >> (r & 63) & 1) {
      if (!shader->float_count) {
        shader->float_first = r;
      }
      shader->float_count = r - shader->float_first + 1;
    }
  }
  shader->writes_color_targets = port->color_targets;
  shader->vertex_fetch_constants = PortVertexFetchConstants(*port, port_ucode);
  ported_++;
  ITC_LOG_DEBUG("native renderer: {} shader {:016X} from its GLSL ({})", StageName(stage), hash,
                port->found_in);
  auto& result = state_->shaders[key];
  result = std::move(shader);
  return result.get();
}

}  // namespace native
