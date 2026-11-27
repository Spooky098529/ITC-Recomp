// From SBX-Recomp (src/renderer/vulkan/ at b128953), used with its author's OK
// (docs/shaders.md, "In this repository"); changed here: our namespace and
// paths, and the vertex fetches' remap and swizzle (docs/pc-renderer.md, step 2).

#include "shader_ports.h"

#include <algorithm>
#include <cstring>
#include <unordered_map>

#include "game_shader_ports.h"
#include "port_spirv.h"

namespace native {

namespace {

uint32_t Word(const uint8_t* ucode, uint32_t index) {
  const uint8_t* p = ucode + size_t(index) * 4;
  return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
}

int32_t SignExtend(uint32_t value, int bits) {
  return int32_t(value << (32 - bits)) >> (32 - bits);
}

// The ports' specialization constants (tools/shader_port/ucode_to_glsl.py).
constexpr uint32_t kSpecInterpolatorMask = 0;   // pixel
constexpr uint32_t kSpecParamGenRegister = 1;   // pixel; -1 for none
constexpr uint32_t kSpecRectangleList = 2;      // vertex
constexpr uint32_t kSpecVertexFetches = 100;    // vertex: 10 a fetch, from 100
// The hand-written post effects' sample counts (glsl/post).
constexpr uint32_t kSpecMotionBlurQuality = 10;  // pixel
constexpr uint32_t kSpecBloomQuality = 11;       // pixel

// The words a vertex format's element has (xe_vf_needed_words with every
// component used).
uint32_t VertexFormatWords(uint32_t format) {
  switch (format) {
    case 26:  // 16_16_16_16
    case 32:  // 16_16_16_16_FLOAT
    case 34:  // 32_32
    case 37:  // 32_32_FLOAT
      return 2;
    case 57:  // 32_32_32_FLOAT
      return 3;
    case 6:   // 8_8_8_8
    case 7:   // 2_10_10_10
    case 16:  // 10_11_11
    case 17:  // 11_11_10
    case 25:  // 16_16
    case 31:  // 16_16_FLOAT
    case 33:  // 32
    case 36:  // 32_FLOAT
      return 1;
    default:  // 32_32_32_32, 32_32_32_32_FLOAT, and as the shader reads others
      return 4;
  }
}

}  // namespace

uint32_t GamePortCount() { return uint32_t(std::size(kGamePorts)); }

namespace {

void PutWord(uint8_t* ucode, uint32_t index, uint32_t value) {
  uint8_t* p = ucode + size_t(index) * 4;
  p[0] = uint8_t(value >> 24);
  p[1] = uint8_t(value >> 16);
  p[2] = uint8_t(value >> 8);
  p[3] = uint8_t(value);
}

uint32_t BigWord(const uint8_t* p) {
  return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
}

}  // namespace

bool FitVertexFetches(const uint8_t* ucode, uint32_t size_bytes, uint32_t fetch_count,
                      const uint32_t* fetch_records, const uint8_t* original_fetches,
                      const std::array<uint32_t, 3>* made, VertexFetchRemap& out) {
  out.ucode.assign(ucode, ucode + size_bytes);
  out.swizzles.clear();
  if (fetch_count > 32) {
    return false;
  }
  for (uint32_t j = 0; j < fetch_count; j++) {
    uint32_t slot = fetch_records[j] & 0xFFF;
    if ((slot * 3 + 3) * 4 > size_bytes) {
      return false;
    }
    const uint8_t* original = original_fetches + 12 * j;
    uint32_t o0 = BigWord(original), o1 = BigWord(original + 4);
    const std::array<uint32_t, 3>& m = made[j];
    // The file's instruction, with the layout fields made for the
    // declaration (as LayoutFreeHash masks them).
    PutWord(out.ucode.data(), slot * 3, (o0 & ~(0x7Fu << 20)) | (m[0] & (0x7Fu << 20)));
    PutWord(out.ucode.data(), slot * 3 + 1, (o1 & ~(0x3FFFFu << 12)) | (m[1] & (0x3FFFFu << 12)));
    PutWord(out.ucode.data(), slot * 3 + 2, m[2]);
    // The swizzle: for each destination component the file's swizzle reads
    // from the fetched value (0-3), what the made swizzle gives that
    // component instead.
    uint32_t file = o1 & 0xFFF, now = m[1] & 0xFFF;
    uint32_t swizzle = 0x688;
    uint32_t set = 0;
    for (uint32_t c = 0; c < 4; c++) {
      uint32_t f = (file >> (3 * c)) & 7, n = (now >> (3 * c)) & 7;
      if (f == 7) {
        continue;  // not written
      }
      if (f >= 4) {
        if (n != f) {
          return false;  // a constant the declaration turns into something else
        }
        continue;
      }
      if (n == 7) {
        n = 4;  // left unwritten: 0
      }
      uint32_t old = (swizzle >> (3 * f)) & 7;
      if ((set >> f) & 1) {
        if (old != n) {
          return false;  // one fetched component read two ways
        }
        continue;
      }
      swizzle = (swizzle & ~(7u << (3 * f))) | (n << (3 * f));
      set |= 1u << f;
    }
    out.swizzles.emplace_back(slot, swizzle);
  }
  return true;
}

uint64_t LayoutFreeHash(const GamePort& port, const uint8_t* ucode, uint32_t size_bytes) {
  std::vector<uint8_t> masked(ucode, ucode + size_bytes);
  auto clear = [&](uint32_t word, uint32_t bits) {
    uint8_t* p = masked.data() + size_t(word) * 4;
    uint32_t v = (uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]) & ~bits;
    p[0] = uint8_t(v >> 24);
    p[1] = uint8_t(v >> 16);
    p[2] = uint8_t(v >> 8);
    p[3] = uint8_t(v);
  };
  for (uint32_t i = 0; i < port.vertex_fetch_count; i++) {
    uint32_t at = port.vertex_fetches[i] * 3u;
    if ((at + 3) * 4 > size_bytes) {
      return 0;
    }
    clear(at, 0x7Fu << 20);         // fetch constant
    clear(at + 1, 0x3FFFFu << 12);  // format, signedness, exponent
    clear(at + 2, 0xFFFFFFFFu);     // stride, offset
  }
  return MicrocodeHash(masked.data(), size_bytes);
}

const GamePort* FindGamePort(ShaderStage stage, const uint8_t* ucode, uint32_t size_bytes,
                             uint64_t hash) {
  for (const GamePort& port : kGamePorts) {
    if (port.stage != stage || port.microcode_words * 4 != size_bytes) {
      continue;
    }
    if (stage == ShaderStage::kPixel ? port.hash == hash
                                     : port.hash == LayoutFreeHash(port, ucode, size_bytes)) {
      return &port;
    }
  }
  return nullptr;
}

bool MakeFromPort(const GamePort& port, const uint8_t* ucode, uint32_t size_bytes,
                  uint32_t interpolator_mask, uint32_t param_gen_interpolator,
                  bool rectangle_list, TranslatedShader& shader,
                  const PostQuality& post_quality,
                  const std::vector<std::pair<uint32_t, uint32_t>>* swizzles) {
  PortSpecialization specialization;
  specialization.vertex = port.stage == ShaderStage::kVertex;
  specialization.interpolator_mask = interpolator_mask & 0xFFFF;
  auto& constants = specialization.constants;
  shader.vertex_fetches.clear();
  if (specialization.vertex) {
    constants.push_back({kSpecRectangleList, rectangle_list ? 1u : 0u});
    // Each vertex fetch's fields as the game patched them.
    for (uint32_t i = 0; i < port.vertex_fetch_count; i++) {
      uint32_t at = port.vertex_fetches[i] * 3u;
      if ((at + 3) * 4 > size_bytes) {
        return false;
      }
      uint32_t w0 = Word(ucode, at), w1 = Word(ucode, at + 1), w2 = Word(ucode, at + 2);
      uint32_t fetch_constant = ((w0 >> 20) & 31) * 3 + ((w0 >> 25) & 3);
      uint32_t flags = ((w1 >> 12) & 1)          // signed
                       | ((w1 >> 13) & 1) << 1   // integer
                       | ((w1 >> 14) & 1) << 2   // signed values have no zero
                       | ((w1 >> 15) & 1) << 3;  // the index is rounded
      uint32_t base = kSpecVertexFetches + 10 * i;
      constants.push_back({base + 0, fetch_constant});
      constants.push_back({base + 1, uint32_t(SignExtend((w2 >> 8) & 0x7FFFFF, 23))});
      constants.push_back({base + 2, w2 & 0xFF});
      constants.push_back({base + 3, (w1 >> 16) & 63});
      constants.push_back({base + 4, flags});
      constants.push_back({base + 5, uint32_t(SignExtend((w1 >> 24) & 63, 6))});
      uint32_t swizzle = 0x688;  // xyzw
      if (swizzles) {
        for (const auto& [instruction, value] : *swizzles) {
          if (instruction == port.vertex_fetches[i]) {
            swizzle = value;
          }
        }
      }
      constants.push_back({base + 6, swizzle});
      shader.vertex_fetches.push_back({fetch_constant,
                                       SignExtend((w2 >> 8) & 0x7FFFFF, 23), w2 & 0xFF,
                                       VertexFormatWords((w1 >> 16) & 63)});
    }
    shader.rectangle_list = rectangle_list;
  } else {
    constants.push_back({kSpecInterpolatorMask, specialization.interpolator_mask});
    constants.push_back({kSpecParamGenRegister,
                         param_gen_interpolator < 16 ? param_gen_interpolator : 0xFFFFFFFFu});
    constants.push_back({kSpecMotionBlurQuality, post_quality.motion_blur});
    constants.push_back({kSpecBloomQuality, post_quality.bloom});
  }
  shader.spirv = SpecializePort(port.spirv, port.spirv_words, specialization);
  if (shader.spirv.empty()) {
    return false;
  }
  shader.textures.clear();
  for (uint32_t i = 0; i < port.texture_count; i++) {
    const GamePortTexture& t = port.textures[i];
    shader.textures.push_back({t.fetch_constant,
                               t.cube ? gpu::ImageType::kCube : gpu::ImageType::k2DArray});
  }
  shader.samplers.clear();
  for (uint32_t i = 0; i < port.sampler_count; i++) {
    const GamePortSampler& s = port.samplers[i];
    // A sampler is the stage's (render::StageSampler): every game shader's
    // fetches take their filters from it (3 and 7: "from the fetch
    // constant"); one that overrides them isn't drawn (named).
    if (s.mag_filter != 3 || s.min_filter != 3 || s.mip_filter != 3 || s.aniso_filter != 7) {
      return false;
    }
    shader.samplers.push_back(
        {s.fetch_constant, s.mag_filter, s.min_filter, s.mip_filter, s.aniso_filter});
  }
  return true;
}

}  // namespace native
