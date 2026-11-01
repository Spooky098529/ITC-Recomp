// The device's fields the renderer reads (the front buffer's render target,
// the movies' and the passes' guest addresses) and guest physical memory.

#pragma once

#include <cstdint>

namespace native {

// The device pointer and fields read directly.
constexpr uint32_t kDevicePointer = 0x8296BD90;
constexpr uint32_t kDevicePixelShader = 12432, kDeviceVertexShader = 12436;
constexpr uint32_t kDeviceIndexBuffer = 12180;
// The vertex declaration (its id at +48; a vertex shader object's +24 holds
// the id of the one its microcode was last patched for).
constexpr uint32_t kDeviceVertexDeclaration = 11552;
constexpr uint32_t kDeviceRenderTarget0 = 12184;
// Scissor: enable (render state 200) and rectangle (left, top, right,
// bottom) from SetScissorRect.
constexpr uint32_t kDeviceScissorEnable = 11600, kDeviceScissorRect = 12416;
// Float constants c0-c255, 16 bytes each: vertex shader's, then pixel
// shader's.
constexpr uint32_t kDeviceVertexFloatConstants = 1792, kDevicePixelFloatConstants = 5888;

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
