// --check_conventional_renderer (temporary): see interface_check.cpp.

#pragma once

#include <rex/graphics/register_file.h>

#include <cstdint>

namespace native {

// At a draw made from the registers `regs` (read from the device's shadow
// after the game's draw), compares what our interface's state gives.
// Nothing without the check.
void InterfaceCheck_Draw(uint8_t* base, uint32_t device, const rex::graphics::RegisterFile& regs,
                         bool inline_draw, int32_t base_vertex, uint32_t inline_stride);

}  // namespace native
