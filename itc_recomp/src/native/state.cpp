#include "state.h"

#include "guest.h"
#include <cstring>

namespace native {

namespace {

// Register blocks the draw path writes from the shadow
// (sub_822ADE30's callers): first register, count, shadow offset.
struct ShadowBlock {
  uint32_t first_register, count, shadow_offset;
};
constexpr ShadowBlock kRegisterBlocks[] = {
    {0x2000, 16, 10240}, {0x2100, 21, 10316}, {0x2180, 5, 10400},
    {0x2200, 12, 10420}, {0x2280, 21, 10468}, {0x2300, 38, 10552},
    // PA_SU_POLY_OFFSET_FRONT/BACK_SCALE/OFFSET, written by render states 204
    // and 208.
    {0x2380, 4, 10704},
};

// Constants (see the device field table in docs/gpu-functions.md). Float
// constants aren't copied: each shader's are packed straight from the
// device (kDeviceFloatConstants).
constexpr ShadowBlock kConstantBlocks[] = {
    {0x4800, 32 * 6, 1024},   // fetch constants
    {0x4900, 8, 9984},        // bool constants, vertex then pixel
    {0x4908, 32, 10016},      // loop constants, vertex then pixel
};

void CopyBlock(uint8_t* base, uint32_t device, const ShadowBlock& block,
               rex::graphics::RegisterFile& regs) {
  guest::CopyU32s(base, device + block.shadow_offset, &regs.values[block.first_register],
                  block.count);
}

}  // namespace

void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs) {
  for (const ShadowBlock& block : kRegisterBlocks) {
    CopyBlock(base, device, block, regs);
  }
  for (const ShadowBlock& block : kConstantBlocks) {
    CopyBlock(base, device, block, regs);
  }
}

}  // namespace native
