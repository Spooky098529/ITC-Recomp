// The graphics device's fields the native renderer reads, and the
// physical memory's addresses. The draw's register state is read by
// draw_registers.h.

#pragma once

#include <cstdint>
#include <vector>
#include "guest.h"

namespace native {

// The device pointer and fields read directly.
constexpr uint32_t kDevicePointer = 0x8296BD90;
constexpr uint32_t kDevicePixelShader = 12432, kDeviceVertexShader = 12436;
constexpr uint32_t kDeviceIndexBuffer = 12180;
// The vertex declaration (its id at +48), and the stream strides the
// vertex fetches take with it (a byte a stream, in words): those the shaders
// were loaded with (Device::LoadShaders copies them from the stream
// strides, +12272, which an inline draw changes around its load).
constexpr uint32_t kDeviceVertexDeclaration = 11552;
constexpr uint32_t kDeviceStreamStrides = 11568;
// The command whose handler is running (0: none).
extern uint32_t g_current_command;
// Bumped by each command handler call (to tell one command's library calls from
// the next's).
extern uint32_t g_command_serial;
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

// The physical memory: 512 MB of host memory of its own (memory::g_physical,
// E5 step 7).
constexpr uint32_t kPhysicalMemorySize = 0x20000000u;

inline uint8_t* PhysicalHostAddress(uint32_t physical) {
  return memory::g_physical + physical;
}

}  // namespace native
