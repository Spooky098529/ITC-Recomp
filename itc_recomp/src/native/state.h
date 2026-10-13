// The GPU state of a draw, read from the graphics library's register and
// constant shadow in the device (see "The register shadow as a whole" in
// docs/gpu-functions.md). Read after the library's draw call, when it holds
// everything the draw used.

#pragma once

#include <cstdint>
#include <rex/graphics/register_file.h>

namespace native {

// The device pointer and fields read directly.
constexpr uint32_t kDevicePointer = 0x8296BD90;
constexpr uint32_t kDevicePixelShader = 12432, kDeviceVertexShader = 12436;
constexpr uint32_t kDeviceIndexBuffer = 12180;
constexpr uint32_t kDeviceRenderTarget0 = 12184;
// Scissor: enable (render state 200) and rectangle (left, top, right,
// bottom) from SetScissorRect.
constexpr uint32_t kDeviceScissorEnable = 11600, kDeviceScissorRect = 12416;
// Float constants c0-c255, 16 bytes each: vertex shader's, then pixel
// shader's.
constexpr uint32_t kDeviceVertexFloatConstants = 1792, kDevicePixelFloatConstants = 5888;

// Fills regs (host byte order) from the device's shadow: the register
// blocks the draw path flushes, and the fetch, bool and loop constants
// (not the float constants). Registers outside them are zero.
void ReadRegisters(uint8_t* base, uint32_t device, rex::graphics::RegisterFile& regs);

// The GPU's physical address for a guest address, as the graphics library
// converts it (IM_LOAD, SetTexture, SetStreamSource).
inline uint32_t GpuPhysicalAddress(uint32_t address) {
  return (address & 0x1FFFFFFFu) + (((address >> 20) + 0x200u) & 0x1000u);
}

// Guest physical memory is one 512 MB mapping 4 GB above the virtual base
// (rex::memory::Memory: physical membase = mapping base + 4 GB).
constexpr uint64_t kPhysicalMemoryOffset = 0x100000000ull;
constexpr uint32_t kPhysicalMemorySize = 0x20000000u;

inline uint8_t* PhysicalHostAddress(uint8_t* base, uint32_t physical) {
  return base + kPhysicalMemoryOffset + physical;
}

}  // namespace native
