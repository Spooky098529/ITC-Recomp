// The graphics device's state setters as plain C++
// (docs/functions/device-state.md): the render states, set through the
// device's setter table at +56 (filled by Device::SetDefaultState from the
// image's table at 0x826388F8), and the one render state getter seen in
// play. Each writes the device's register shadow and the software copies
// the setters build registers from, and marks what it wrote in the dirty
// masks (+16 the shader state, +24 the render state, +32 the other state),
// exactly as the original (the WRAP8-15 quirk kept: they mark +16 bit 45,
// 0x2183's bit, not 0x2184's). Floats come in r4 as their bits. Adapters:
// src/hooks/device_state.cpp (the tables point at the guest addresses, so
// the adapters stay).

#pragma once

#include <cstdint>

namespace graphics::device_state {

// The render state setters: X(address, name, float), `float` when the
// original does float math (its adapter turns the flush mode off first, as
// the original does).
#define ITC_RENDER_STATE_SETTERS(X)                  \
  X(822A18D8, SetZEnable, false)                     \
  X(822A1950, SetZFunc, false)                       \
  X(822A1920, SetZWriteEnable, false)                \
  X(822A1290, SetFillMode, false)                    \
  X(822A1260, SetCullMode, false)                    \
  X(822A1300, SetAlphaBlendEnable, false)            \
  X(822A1690, SetSeparateAlphaBlendEnable, false)    \
  X(822A17C0, SetBlendFactor, true)                  \
  X(822A1420, SetSrcBlend, false)                    \
  X(822A14B0, SetDestBlend, false)                   \
  X(822A1390, SetBlendOp, false)                     \
  X(822A15B0, SetSrcBlendAlpha, false)               \
  X(822A1620, SetDestBlendAlpha, false)              \
  X(822A1540, SetBlendOpAlpha, false)                \
  X(822A12C0, SetAlphaTestEnable, false)             \
  X(822A1728, SetAlphaRef, true)                     \
  X(822A1790, SetAlphaFunc, false)                   \
  X(822A1990, SetStencilEnable, false)               \
  X(822A19D8, SetTwoSidedStencilMode, false)         \
  X(822A1A48, SetStencilFail, false)                 \
  X(822A1A88, SetStencilZFail, false)                \
  X(822A1AC8, SetStencilPass, false)                 \
  X(822A1A18, SetStencilFunc, false)                 \
  X(822A1BD8, SetStencilRef, false)                  \
  X(822A1BF8, SetStencilMask, false)                 \
  X(822A1C18, SetStencilWriteMask, false)            \
  X(822A1B28, SetCcwStencilFail, false)              \
  X(822A1B68, SetCcwStencilZFail, false)             \
  X(822A1BA8, SetCcwStencilPass, false)              \
  X(822A1AF8, SetCcwStencilFunc, false)              \
  X(822A1C38, SetCcwStencilRef, false)               \
  X(822A1C58, SetCcwStencilMask, false)              \
  X(822A1C78, SetCcwStencilWriteMask, false)         \
  X(822A1C98, SetClipPlaneEnable, false)             \
  X(822A1FA0, SetPointSize, true)                    \
  X(822A2000, SetPointSizeMin, true)                 \
  X(822A1F90, SetPointSpriteEnable, false)           \
  X(822A2058, SetPointSizeMax, true)                 \
  X(822A1E60, SetMultiSampleAntiAlias, false)        \
  X(822A1E90, SetMultiSampleMask, false)             \
  X(822A1CF0, SetSlopeScaleDepthBias, true)          \
  X(822A1DB8, SetDepthBias, true)                    \
  X(822A1EB0, SetColorWriteEnable, false)            \
  X(822A1EE8, SetColorWriteEnable1, false)           \
  X(822A1F20, SetColorWriteEnable2, false)           \
  X(822A1F58, SetColorWriteEnable3, false)           \
  X(822A27B0, SetTessellationMode, false)            \
  X(822A2740, SetMinTessellationLevel, true)         \
  X(822A2778, SetMaxTessellationLevel, true)         \
  X(822A20B0, SetWrap0, false)                       \
  X(822A20D8, SetWrap1, false)                       \
  X(822A2108, SetWrap2, false)                       \
  X(822A2138, SetWrap3, false)                       \
  X(822A2168, SetWrap4, false)                       \
  X(822A2198, SetWrap5, false)                       \
  X(822A21C8, SetWrap6, false)                       \
  X(822A21F8, SetWrap7, false)                       \
  X(822A2220, SetWrap8, false)                       \
  X(822A2248, SetWrap9, false)                       \
  X(822A2278, SetWrap10, false)                      \
  X(822A22A8, SetWrap11, false)                      \
  X(822A22D8, SetWrap12, false)                      \
  X(822A2308, SetWrap13, false)                      \
  X(822A2338, SetWrap14, false)                      \
  X(822A2368, SetWrap15, false)                      \
  X(822A2430, SetViewportEnable, false)              \
  X(822A2480, SetHighPrecisionBlendEnable, false)    \
  X(822A2530, SetHighPrecisionBlendEnable1, false)   \
  X(822A25E0, SetHighPrecisionBlendEnable2, false)   \
  X(822A2690, SetHighPrecisionBlendEnable3, false)   \
  X(822A27E8, SetHalfPixelOffset, false)             \
  X(822A2820, SetPrimitiveResetEnable, false)        \
  X(822A2850, SetPrimitiveResetIndex, false)         \
  X(822A2870, SetAlphaToMaskEnable, false)           \
  X(822A28A0, SetAlphaToMaskOffsets, false)          \
  X(822A28D8, SetGuardBandX, true)                   \
  X(822A2908, SetGuardBandY, true)                   \
  X(822A2938, SetDiscardBandX, true)                 \
  X(822A2960, SetDiscardBandY, true)                 \
  X(822A2990, SetHiStencilEnable, false)             \
  X(822A29C0, SetHiStencilWriteEnable, false)        \
  X(822A29F0, SetHiStencilFunc, false)               \
  X(822A2A20, SetHiStencilRef, false)                \
  X(822A2A40, SetPresentInterval, false)             \
  X(822A2A50, SetPresentImmediateThreshold, false)

// Each: r3 the device, r4 the value (a float's bits for the float ones).
#define ITC_DECLARE_SETTER(address, name, uses_float) \
  void name(uint8_t* base, uint32_t device, uint32_t value);
ITC_RENDER_STATE_SETTERS(ITC_DECLARE_SETTER)
#undef ITC_DECLARE_SETTER

// sub_822A2470 Device::GetRenderState_ViewportEnable: PA_CL_VTE_CNTL's
// shadow (+10444) bit 0.
uint32_t GetViewportEnable(uint8_t* base, uint32_t device);

// Device fields the setters write (byte offsets).
constexpr uint32_t kShaderDirty = 16;  // the dirty masks: the shader state
constexpr uint32_t kRenderDirty = 24;  // the render state
constexpr uint32_t kOtherDirty = 32;   // the other state
constexpr uint32_t kRenderTargets = 12184;  // four surfaces
constexpr uint32_t kDepthSurface = 12200;

}  // namespace graphics::device_state
