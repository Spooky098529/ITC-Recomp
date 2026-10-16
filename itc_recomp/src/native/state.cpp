#include "state.h"

#include "guest.h"
#include <cstring>

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

constexpr uint64_t LowBits(uint32_t n) {
  return n >= 64 ? ~0ull : (1ull << n) - 1;
}

// What each bit of the device's masks has copied: shadow offset, first
// register, words (0: none of ours).
struct BitCopy {
  uint16_t shadow_offset = 0, first_register = 0;
  uint8_t words = 0;
};
struct BitCopies {
  BitCopy bits[3][64];
  uint64_t known[3] = {};
};
constexpr BitCopies MakeBitCopies() {
  BitCopies copies;
  for (const ShadowBlock& block : kBlocks) {
    if (block.width == 1) {
      // Bool and loop constants: one bit for both blocks, contiguous.
      BitCopy& copy = copies.bits[block.mask][block.lo];
      if (!copy.words) {
        copy = {uint16_t(block.shadow_offset), uint16_t(block.first_register), 0};
      }
      copy.words += uint8_t(block.units);
      copies.known[block.mask] |= 1ull << block.lo;
      continue;
    }
    for (uint32_t k = 0; k < block.units; k++) {
      uint32_t bit = block.lo + block.width - 1 - k, word = k * block.words_per_unit;
      copies.bits[block.mask][bit] = {uint16_t(block.shadow_offset + 4 * word),
                                      uint16_t(block.first_register + word),
                                      uint8_t(block.words_per_unit)};
      copies.known[block.mask] |= 1ull << bit;
    }
  }
  return copies;
}
constexpr BitCopies kBitCopies = MakeBitCopies();
static_assert(kBitCopies.bits[kRenderState][1].words == 40);
static_assert(kBitCopies.bits[kRenderState][1].shadow_offset == 9984);

// The device's masks taken from draws (MarkDeviceDirty, ORed). Units marked
// dirty otherwise (by hand, or by the library's flush helpers), by block:
// bit k is unit k, and which blocks have any. All at first (regs starts
// zeroed). Render thread.
uint64_t g_device_dirty[3] = {};
uint64_t g_pending[kRegisterReadBlocks] = {~0ull, ~0ull, ~0ull, ~0ull, ~0ull,
                                           ~0ull, ~0ull, ~0ull, ~0ull, ~0ull};
uint32_t g_pending_blocks = (1u << kRegisterReadBlocks) - 1;

// The units of block b dirty (for the counts of --native_renderer_log_register_reads).
uint64_t PendingUnits(size_t b) {
  const ShadowBlock& block = kBlocks[b];
  uint64_t field = (g_device_dirty[block.mask] >> block.lo) & LowBits(block.width);
  // Field bit width-1-k is unit k.
  uint64_t units = block.width == 1 ? (field ? ~0ull : 0)
                                    : __builtin_bitreverse64(field) >> (64 - block.width);
  return (g_pending[b] | units) & LowBits(block.units);
}

}  // namespace

void MarkDeviceDirty(uint64_t shader_state, uint64_t render_state, uint64_t other_state) {
  g_device_dirty[kShaderState] |= shader_state;
  g_device_dirty[kRenderState] |= render_state;
  g_device_dirty[kOtherState] |= other_state;
}

void MarkRegistersDirty(uint32_t first_register, uint64_t mask) {
  // Bit k of the reversed mask is register first_register + k.
  uint64_t registers = __builtin_bitreverse64(mask);
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    if (b == kFetchBlock) {
      continue;
    }
    uint64_t units = 0;
    if (block.first_register >= first_register) {
      if (uint32_t shift = block.first_register - first_register; shift < 64) {
        units = registers >> shift;
      }
    } else if (uint32_t shift = first_register - block.first_register; shift < 64) {
      units = registers << shift;
    }
    units &= LowBits(block.units);
    g_pending[b] |= units;
    g_pending_blocks |= uint32_t(units != 0) << b;
  }
}

void MarkFetchConstantsDirty(uint64_t mask) {
  g_pending[kFetchBlock] |= __builtin_bitreverse64(mask);
  g_pending_blocks |= uint32_t(mask != 0) << kFetchBlock;
}

void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs) {
  // Each bit of the device's masks: a register, a fetch constant (6 words),
  // or the bool and loop constants.
  for (size_t m = 0; m < 3; m++) {
    uint64_t bits = g_device_dirty[m] & kBitCopies.known[m];
    g_device_dirty[m] = 0;
    while (bits) {
      const BitCopy& copy = kBitCopies.bits[m][__builtin_ctzll(bits)];
      bits &= bits - 1;
      if (copy.words == 1) {
        regs.values[copy.first_register] = guest::LoadU32(base, device + copy.shadow_offset);
      } else {
        guest::CopyU32s(base, device + copy.shadow_offset, &regs.values[copy.first_register],
                        copy.words);
      }
    }
  }
  // Units marked otherwise (rare).
  while (g_pending_blocks) {
    size_t b = size_t(__builtin_ctz(g_pending_blocks));
    g_pending_blocks &= g_pending_blocks - 1;
    const ShadowBlock& block = kBlocks[b];
    uint64_t bits = g_pending[b] & LowBits(block.units);
    g_pending[b] = 0;
    while (bits) {
      uint32_t k = uint32_t(__builtin_ctzll(bits));
      bits &= bits - 1;
      guest::CopyU32s(base, device + block.shadow_offset + 4 * block.words_per_unit * k,
                      &regs.values[block.first_register + block.words_per_unit * k],
                      block.words_per_unit);
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
    stats.words_copied += uint32_t(__builtin_popcountll(PendingUnits(b))) * block.words_per_unit;
  }
  ReadRegisters(base, device, regs);
  stats.calls++;
  stats.words += n;
  bool any = false;
  n = 0;
  for (size_t b = 0; b < kRegisterReadBlocks; b++) {
    const ShadowBlock& block = kBlocks[b];
    uint32_t words = block.units * block.words_per_unit;
    uint32_t changed = 0;
    for (uint32_t i = 0; i < words; i++) {
      changed += old[n + i] != regs.values[block.first_register + i];
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
