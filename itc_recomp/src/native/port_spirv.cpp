// From SBX-Recomp (src/renderer/vulkan/ at b128953), used with its author's OK
// (docs/shaders.md, "In this repository"); changed here: our namespace and
// paths, and the vertex fetches' remap and swizzle (docs/pc-renderer.md, step 2).

#include "port_spirv.h"

#include <bit>
#include <unordered_map>
#include <unordered_set>

namespace native {

namespace {

// The few SPIR-V opcodes and enumerants this needs (from the SPIR-V
// specification).
constexpr uint32_t kSpirvMagic = 0x07230203;
constexpr uint32_t kOpSpecConstantTrue = 48;
constexpr uint32_t kOpSpecConstantFalse = 49;
constexpr uint32_t kOpSpecConstant = 50;
constexpr uint32_t kOpVariable = 59;
constexpr uint32_t kOpDecorate = 71;
constexpr uint32_t kDecorationSpecId = 1;
constexpr uint32_t kDecorationLocation = 30;
constexpr uint32_t kStorageClassInput = 1;
constexpr uint32_t kStorageClassOutput = 3;

// Calls fn(at, opcode, word_count) for each instruction; false if the
// instructions don't fit the words.
template <typename Fn>
bool ForEachInstruction(const std::vector<uint32_t>& words, Fn fn) {
  size_t at = 5;
  while (at < words.size()) {
    uint32_t count = words[at] >> 16, opcode = words[at] & 0xFFFF;
    if (count == 0 || at + count > words.size()) {
      return false;
    }
    fn(at, opcode, count);
    at += count;
  }
  return true;
}

}  // namespace

std::vector<uint32_t> SpecializePort(const uint32_t* spirv, size_t word_count,
                                     const PortSpecialization& specialization) {
  std::vector<uint32_t> words(spirv, spirv + word_count);
  if (words.size() < 5 || words[0] != kSpirvMagic) {
    return {};
  }
  // What the decorations point at: the constants' ids by SpecId, and the
  // interpolators' variables.
  std::unordered_map<uint32_t, uint32_t> spec_ids;  // result id -> SpecId
  std::unordered_set<uint32_t> interpolators;
  uint32_t interpolator_class =
      specialization.vertex ? kStorageClassOutput : kStorageClassInput;
  bool read = ForEachInstruction(words, [&](size_t at, uint32_t opcode, uint32_t count) {
    if (opcode == kOpDecorate && count >= 4 && words[at + 2] == kDecorationSpecId) {
      spec_ids[words[at + 1]] = words[at + 3];
    } else if (opcode == kOpVariable && count >= 4 && words[at + 3] == interpolator_class) {
      interpolators.insert(words[at + 2]);
    }
  });
  if (!read) {
    return {};
  }
  std::unordered_map<uint32_t, uint32_t> values(specialization.constants.begin(),
                                                specialization.constants.end());
  uint32_t mask = specialization.interpolator_mask;
  ForEachInstruction(words, [&](size_t at, uint32_t opcode, uint32_t count) {
    if ((opcode == kOpSpecConstantTrue || opcode == kOpSpecConstantFalse) && count >= 3) {
      auto id = spec_ids.find(words[at + 2]);
      if (id != spec_ids.end()) {
        if (auto value = values.find(id->second); value != values.end()) {
          words[at] = (count << 16) | (value->second ? kOpSpecConstantTrue : kOpSpecConstantFalse);
        }
      }
    } else if (opcode == kOpSpecConstant && count >= 4) {
      auto id = spec_ids.find(words[at + 2]);
      if (id != spec_ids.end()) {
        if (auto value = values.find(id->second); value != values.end()) {
          words[at + 3] = value->second;  // 32-bit constants only
        }
      }
    } else if (opcode == kOpDecorate && count >= 4 && words[at + 2] == kDecorationLocation &&
               interpolators.count(words[at + 1])) {
      // Inputs or outputs that aren't built-ins: the interpolators (a pixel
      // shader's color outputs are outputs, so they're left alone).
      uint32_t n = words[at + 3];
      if (n < 16) {
        words[at + 3] = (mask >> n) & 1 ? uint32_t(std::popcount(mask & ((1u << n) - 1))) : 16 + n;
      }
    }
  });
  return words;
}

}  // namespace native
