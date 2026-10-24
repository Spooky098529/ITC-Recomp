// The graphics device's state setters as plain C++
// (docs/functions/device-state.md): the render states, set through the
// device's setter table at +56 (filled by Device::SetDefaultState from the
// image's table at 0x826388F8), and the one render state getter seen in
// play, and the sampler states, set through the table at +440. Each writes the device's register shadow and the software copies
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

// The sampler state setters: X(address, name, float), as the render
// states'. Each: r3 the device, r4 the sampler (0-25), r5 the value; they
// write the sampler's fetch constant (device + 1024 + 24 x sampler) and
// mark +16 bit 43 - sampler.
#define ITC_SAMPLER_STATE_SETTERS(X)                 \
  X(822A31A0, SetAddressU, false)                    \
  X(822A31F0, SetAddressV, false)                    \
  X(822A3240, SetAddressW, false)                    \
  X(822A3130, SetBorderColor, false)                 \
  X(822A2BF8, SetMagFilter, false)                   \
  X(822A2A70, SetMinFilter, false)                   \
  X(822A2D80, SetMipFilter, false)                   \
  X(822A2F90, SetMipMapLodBias, true)                \
  X(822A3030, SetMaxMipLevel, false)                 \
  X(822A2E70, SetMaxAnisotropy, false)               \
  X(822A2CE8, SetMagFilterZ, false)                  \
  X(822A2B60, SetMinFilterZ, false)                  \
  X(822A2DD8, SetSeparateZFilterEnable, false)       \
  X(822A30B0, SetMinMipLevel, false)                 \
  X(822A3290, SetTrilinearThreshold, false)          \
  X(822A2EE8, SetAnisotropyBias, true)               \
  X(822A32E8, SetHGradientExpBias, false)            \
  X(822A3340, SetVGradientExpBias, false)            \
  X(822A3398, SetWhiteBorderColorW, false)           \
  X(822A33F0, SetPointBorderEnable, false)

#define ITC_DECLARE_SAMPLER_SETTER(address, name, uses_float) \
  void name(uint8_t* base, uint32_t device, uint32_t sampler, uint32_t value);
ITC_SAMPLER_STATE_SETTERS(ITC_DECLARE_SAMPLER_SETTER)
#undef ITC_DECLARE_SAMPLER_SETTER

// sub_822A2470 Device::GetRenderState_ViewportEnable: PA_CL_VTE_CNTL's
// shadow (+10444) bit 0.
uint32_t GetViewportEnable(uint8_t* base, uint32_t device);

// The bound objects and the shader constants. The ones that fence the
// object they replace return false, with nothing written, when the fence
// would go on a full fence list (its flush, sub_822AD790, is guest code and
// isn't seen in play): the original runs then.

// sub_822A3740 Device::GetViewport: the 24-byte viewport at +12392 copied
// to `out` (memcpy's copy). False, with nothing copied, where memcpy's
// override has to run (a destination in the physical views, which it
// records for the renderer, or ranges that may share memory).
bool GetViewport(uint8_t* base, uint32_t device, uint32_t out);

// sub_822A3758 Device::SetStreamSource: the stream's vertex fetch constant
// (with a buffer; `dirty` ORed into +16), the previous buffer fenced, the
// buffer and its stride / 4 kept, and +16 bit 51 when the stride differs
// from the vertex shader's.
bool SetStreamSource(uint8_t* base, uint32_t device, uint32_t stream, uint32_t buffer,
                     uint32_t offset, uint32_t stride, uint64_t dirty);

// sub_822A3908 Device::SetIndices: the previous index buffer fenced, then
// +12180 = `buffer`.
bool SetIndices(uint8_t* base, uint32_t device, uint32_t buffer);

// sub_822A6008 Device::SetTexture: with a texture, its fetch constant
// merged into the stage's (the sampler states' bits kept, the mip levels
// clamped by the stage's), `dirty` ORed into +16; the texture kept and the
// previous one fenced.
bool SetTexture(uint8_t* base, uint32_t device, uint32_t stage, uint32_t texture,
                uint64_t dirty);

// sub_822A3B30 Device::SetBlendControl: RB_BLENDCONTROL of render target
// `target` = `value`; its register's dirty bit and +16 bit 49.
void SetBlendControl(uint8_t* base, uint32_t device, uint32_t target, uint32_t value);

// sub_822A81F8 Device::SetVertexShaderConstantF and sub_822A8308
// Device::SetPixelShaderConstantF: `count` four-float constants from
// `data` (any alignment) to device + 1792 (+5888) + 16 x `first`, stored
// 16-byte aligned as the original's stvx; `dirty` ORed into +0 (+8).
void SetVertexShaderConstantF(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                              uint32_t count, uint64_t dirty);
void SetPixelShaderConstantF(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                             uint32_t count, uint64_t dirty);

// sub_822A8470 Device::SetPixelShaderConstantB: `count` (at least 1, as
// the original's do-while) bool constants from `data` (a word each, bit 0)
// into the bits at +10000; +24 bit 1.
void SetPixelShaderConstantB(uint8_t* base, uint32_t device, uint32_t first, uint32_t data,
                             uint32_t count);

// sub_822A6A88 VertexDeclaration::FromFvf: the declaration for flexible
// vertex format `fvf` built at `declaration` (its 12-byte elements from
// +52: stream, offset, type, method, usage, usage index; the end marker
// after them), its element count at +24 and +28, +48 = 0. Returns the
// elements written before the end marker. The end marker's last byte (a
// pad) is the original's uninitialized stack byte; 0 here.
uint32_t VertexDeclarationFromFvf(uint8_t* base, uint32_t fvf, uint32_t declaration);

// sub_822AA0E8 Device::SetFVF: +12176 = `fvf`, the device's own declaration
// built from it at +11936 and made the current one (+11552), +16 bit 51 (the
// vertex shader, patched again).
void SetFVF(uint8_t* base, uint32_t device, uint32_t fvf);

// Device fields the setters write (byte offsets).
constexpr uint32_t kShaderDirty = 16;  // the dirty masks: the shader state
constexpr uint32_t kRenderDirty = 24;  // the render state
constexpr uint32_t kOtherDirty = 32;   // the other state
constexpr uint32_t kRenderTargets = 12184;  // four surfaces
constexpr uint32_t kDepthSurface = 12200;
constexpr uint32_t kTextures = 12288;       // a texture a stage
constexpr uint32_t kFetchConstants = 1024;  // 24 bytes a sampler

}  // namespace graphics::device_state
