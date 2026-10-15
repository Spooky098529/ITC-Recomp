#include "state.h"

#include "guest.h"
#include <cstring>
#include <x86intrin.h>

namespace native {

namespace {

// The blocks of the shadow read: the register blocks the draw path writes
// (sub_822ADE30's callers in DrawIndexedVertices, sub_825BE708), and the
// fetch, bool and loop constants. Float constants aren't copied: each
// shader's are packed straight from the device (kDeviceFloatConstants).
//
// Each unit (a register, or a 6-word fetch constant) has a dirty bit in one
// of the device's masks (+16 shader state, +24 render state, +32 other
// state): the draw shifts bits lo..lo+width-1 of the mask to the top and
// passes them to sub_822ADE30 / sub_822AE070, which take the top bit as
// unit 0. So unit k is bit lo+width-1-k. Bool and loop constants share one
// bit (width 1: all of them).
struct ShadowBlock {
  uint32_t first_register, units, shadow_offset;
  uint32_t mask, lo, width;
  uint32_t words_per_unit = 1;
};
enum { kShaderState, kRenderState, kOtherState };
constexpr ShadowBlock kBlocks[kRegisterReadBlocks] = {
    {0x2000, 16, 10240, kRenderState, 23, 16},
    {0x2100, 21, 10316, kRenderState, 2, 21},
    {0x2180, 5, 10400, kShaderState, 44, 5},
    {0x2200, 12, 10420, kShaderState, 0, 12},
    {0x2280, 21, 10468, kRenderState, 39, 21},
    {0x2300, 38, 10552, kOtherState, 0, 38},
    // PA_SU_POLY_OFFSET_FRONT/BACK_SCALE/OFFSET, written by render states 204
    // and 208 (the first 4 of the mask's 8).
    {0x2380, 4, 10704, kOtherState, 38, 8},
    // Fetch constants (see the device field table in docs/gpu-functions.md).
    {0x4800, 32, 1024, kShaderState, 12, 32, 6},
    {0x4900, 8, 9984, kRenderState, 1, 1},    // bool constants, vertex then pixel
    {0x4908, 32, 10016, kRenderState, 1, 1},  // loop constants, vertex then pixel
};
constexpr size_t kFetchBlock = 7;
// Shader state bit 8 has the draw call sub_822ADC08, which writes register
// 0x2203 (+10432, unit 3 of block 0x2200) itself: the shader load sets it
// after changing it.
constexpr uint64_t kShaderStateHiz = 0x100;
constexpr size_t kHizBlock = 3;
constexpr uint64_t kHizUnit = 1ull << 3;

constexpr uint64_t LowBits(uint32_t n) {
  return n >= 64 ? ~0ull : (1ull << n) - 1;
}

// Units not read since the library marked them dirty, by block: bit k is
// unit k. All at first (regs starts zeroed). Render thread.
uint64_t g_pending[kRegisterReadBlocks] = {~0ull, ~0ull, ~0ull, ~0ull, ~0ull,
                                           ~0ull, ~0ull, ~0ull, ~0ull, ~0ull};

}  // namespace

void MarkDeviceDirty(uint64_t shader_state, uint64_t render_state, uint64_t other_state) {
  const uint64_t masks[3] = {shader_state, render_state, other_state};
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    uint64_t field = (masks[block.mask] >> block.lo) & LowBits(block.width);
    if (field) {
      // Field bit width-1-k is unit k.
      g_pending[b] |= block.width == 1 ? ~0ull
                                       : __builtin_bitreverse64(field) >> (64 - block.width);
    }
  }
  if (shader_state & kShaderStateHiz) {
    g_pending[kHizBlock] |= kHizUnit;
  }
}

void MarkRegistersDirty(uint32_t first_register, uint64_t mask) {
  // Bit k of the reversed mask is register first_register + k.
  uint64_t registers = __builtin_bitreverse64(mask);
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    if (b == kFetchBlock) {
      continue;
    }
    if (block.first_register >= first_register) {
      if (uint32_t shift = block.first_register - first_register; shift < 64) {
        g_pending[b] |= registers >> shift;
      }
    } else if (uint32_t shift = first_register - block.first_register; shift < 64) {
      g_pending[b] |= registers << shift;
    }
  }
}

void MarkFetchConstantsDirty(uint64_t mask) {
  g_pending[kFetchBlock] |= __builtin_bitreverse64(mask);
}

void MarkAllDirty() {
  for (uint64_t& pending : g_pending) {
    pending = ~0ull;
  }
}

void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs) {
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    uint64_t bits = g_pending[b] & LowBits(block.units);
    g_pending[b] = 0;
    // Each run of dirty units.
    while (bits) {
      uint32_t first = uint32_t(__builtin_ctzll(bits));
      uint64_t from_first = bits >> first;
      uint32_t count = ~from_first ? uint32_t(__builtin_ctzll(~from_first)) : 64 - first;
      bits &= ~(LowBits(count) << first);
      uint32_t word = first * block.words_per_unit;
      guest::CopyU32s(base, device + block.shadow_offset + 4 * word,
                      &regs.values[block.first_register + word], count * block.words_per_unit);
    }
  }
}

void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs,
                   RegisterReadStats& stats) {
  // The words as last read (from regs, not touching the shadow).
  uint32_t old[512];
  uint32_t n = 0;
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    uint32_t words = block.units * block.words_per_unit;
    std::memcpy(old + n, &regs.values[block.first_register], 4 * words);
    n += words;
    stats.words_copied += uint32_t(__builtin_popcountll(g_pending[b] & LowBits(block.units))) *
                          block.words_per_unit;
  }
  _mm_lfence();
  uint64_t start = __rdtsc();
  _mm_lfence();
  ReadRegisters(base, device, regs);
  _mm_lfence();
  stats.copy_cycles += __rdtsc() - start;
  stats.calls++;
  stats.words += n;
  bool any = false;
  n = 0;
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    uint32_t words = block.units * block.words_per_unit;
    // What reading the whole block gives (as ReadRegisters did before
    // reading only what's dirty).
    uint32_t full[256];
    guest::CopyU32s(base, device + block.shadow_offset, full, words);
    uint32_t changed = 0;
    for (uint32_t i = 0; i < words; i++) {
      uint32_t value = regs.values[block.first_register + i];
      changed += old[n + i] != value;
      if (value != full[i]) {
        if (!stats.mismatched_words) {
          stats.mismatch_sample = {block.first_register + i, value, full[i]};
        }
        stats.mismatched_words++;
        stats.blocks_mismatched[b]++;
      }
    }
    n += words;
    stats.words_changed += changed;
    if (changed) {
      stats.blocks_changed[b]++;
      any = true;
    }
  }
  stats.calls_changed += any;
}

}  // namespace native
