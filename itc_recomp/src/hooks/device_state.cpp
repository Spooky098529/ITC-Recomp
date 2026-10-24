// The graphics device's state setters natively: adapters in front of
// src/graphics/device_state.h (docs/functions/device-state.md). The setters
// are reached through the device's setter tables (filled with their guest
// addresses by Device::SetDefaultState), so these adapters stay. Each leaves
// r3 as it came (the device; the originals don't change it), except the
// getter, which returns its value in r3.

#include "guest.h"

#include "base/flags.h"
#include "graphics/device_state.h"

ITC_FLAG_BOOL(native_device_state, true, "ITC",
              "Run the graphics device's state setters (docs/functions/device-state.md: "
              "the render state setters sub_822A1260 to sub_822A2A50, the sampler state "
              "setters sub_822A2A70 to sub_822A33F0, Device::GetRenderState_ViewportEnable "
              "sub_822A2470, GetViewport sub_822A3740, SetStreamSource sub_822A3758, "
              "SetIndices sub_822A3908, SetTexture sub_822A6008, SetBlendControl "
              "sub_822A3B30, the shader constants sub_822A81F8, sub_822A8308 and "
              "sub_822A8470, SetFVF sub_822AA0E8 with VertexDeclaration::FromFvf "
              "sub_822A6A88, SetScissorRect sub_822A3450, ScissorTestEnable sub_822A3D20, "
              "SetViewport sub_822A3518, SetSurfaceInfo sub_822A3C88, SetRenderTarget "
              "sub_822A3D30 with SetRenderTarget_External sub_822A4340, "
              "SetDepthStencilSurface sub_822A4060, with SetScreenScissor "
              "sub_822A0F80 dropped; the fence list's flush left to the originals) natively instead of "
              "through the translated code (the same writes to the register shadow and the "
              "dirty masks; checked by --run_harnesses=device_state).");

namespace ds = graphics::device_state;

// Written out one a line (not through ITC_RENDER_STATE_SETTERS) so
// tools/check_function_statuses.py finds each override.
#define ITC_DEVICE_STATE_SETTER(address, name, uses_float) \
  extern "C" REX_FUNC(sub_##address) {                      \
    REX_FUNC_PROLOGUE();                                    \
    if (!ITC_FLAG(native_device_state)) {                   \
      __imp__sub_##address(ctx, base);                      \
      return;                                               \
    }                                                       \
    if (uses_float) {                                       \
      ctx.fpscr.disableFlushMode();                         \
    }                                                       \
    ds::name(base, ctx.r3.u32, ctx.r4.u32);                 \
  }

ITC_DEVICE_STATE_SETTER(822A18D8, SetZEnable, false)
ITC_DEVICE_STATE_SETTER(822A1950, SetZFunc, false)
ITC_DEVICE_STATE_SETTER(822A1920, SetZWriteEnable, false)
ITC_DEVICE_STATE_SETTER(822A1290, SetFillMode, false)
ITC_DEVICE_STATE_SETTER(822A1260, SetCullMode, false)
ITC_DEVICE_STATE_SETTER(822A1300, SetAlphaBlendEnable, false)
ITC_DEVICE_STATE_SETTER(822A1690, SetSeparateAlphaBlendEnable, false)
ITC_DEVICE_STATE_SETTER(822A17C0, SetBlendFactor, true)
ITC_DEVICE_STATE_SETTER(822A1420, SetSrcBlend, false)
ITC_DEVICE_STATE_SETTER(822A14B0, SetDestBlend, false)
ITC_DEVICE_STATE_SETTER(822A1390, SetBlendOp, false)
ITC_DEVICE_STATE_SETTER(822A15B0, SetSrcBlendAlpha, false)
ITC_DEVICE_STATE_SETTER(822A1620, SetDestBlendAlpha, false)
ITC_DEVICE_STATE_SETTER(822A1540, SetBlendOpAlpha, false)
ITC_DEVICE_STATE_SETTER(822A12C0, SetAlphaTestEnable, false)
ITC_DEVICE_STATE_SETTER(822A1728, SetAlphaRef, true)
ITC_DEVICE_STATE_SETTER(822A1790, SetAlphaFunc, false)
ITC_DEVICE_STATE_SETTER(822A1990, SetStencilEnable, false)
ITC_DEVICE_STATE_SETTER(822A19D8, SetTwoSidedStencilMode, false)
ITC_DEVICE_STATE_SETTER(822A1A48, SetStencilFail, false)
ITC_DEVICE_STATE_SETTER(822A1A88, SetStencilZFail, false)
ITC_DEVICE_STATE_SETTER(822A1AC8, SetStencilPass, false)
ITC_DEVICE_STATE_SETTER(822A1A18, SetStencilFunc, false)
ITC_DEVICE_STATE_SETTER(822A1BD8, SetStencilRef, false)
ITC_DEVICE_STATE_SETTER(822A1BF8, SetStencilMask, false)
ITC_DEVICE_STATE_SETTER(822A1C18, SetStencilWriteMask, false)
ITC_DEVICE_STATE_SETTER(822A1B28, SetCcwStencilFail, false)
ITC_DEVICE_STATE_SETTER(822A1B68, SetCcwStencilZFail, false)
ITC_DEVICE_STATE_SETTER(822A1BA8, SetCcwStencilPass, false)
ITC_DEVICE_STATE_SETTER(822A1AF8, SetCcwStencilFunc, false)
ITC_DEVICE_STATE_SETTER(822A1C38, SetCcwStencilRef, false)
ITC_DEVICE_STATE_SETTER(822A1C58, SetCcwStencilMask, false)
ITC_DEVICE_STATE_SETTER(822A1C78, SetCcwStencilWriteMask, false)
ITC_DEVICE_STATE_SETTER(822A1C98, SetClipPlaneEnable, false)
ITC_DEVICE_STATE_SETTER(822A1FA0, SetPointSize, true)
ITC_DEVICE_STATE_SETTER(822A2000, SetPointSizeMin, true)
ITC_DEVICE_STATE_SETTER(822A1F90, SetPointSpriteEnable, false)
ITC_DEVICE_STATE_SETTER(822A2058, SetPointSizeMax, true)
ITC_DEVICE_STATE_SETTER(822A1E60, SetMultiSampleAntiAlias, false)
ITC_DEVICE_STATE_SETTER(822A1E90, SetMultiSampleMask, false)
ITC_DEVICE_STATE_SETTER(822A1CF0, SetSlopeScaleDepthBias, true)
ITC_DEVICE_STATE_SETTER(822A1DB8, SetDepthBias, true)
ITC_DEVICE_STATE_SETTER(822A1EB0, SetColorWriteEnable, false)
ITC_DEVICE_STATE_SETTER(822A1EE8, SetColorWriteEnable1, false)
ITC_DEVICE_STATE_SETTER(822A1F20, SetColorWriteEnable2, false)
ITC_DEVICE_STATE_SETTER(822A1F58, SetColorWriteEnable3, false)
ITC_DEVICE_STATE_SETTER(822A27B0, SetTessellationMode, false)
ITC_DEVICE_STATE_SETTER(822A2740, SetMinTessellationLevel, true)
ITC_DEVICE_STATE_SETTER(822A2778, SetMaxTessellationLevel, true)
ITC_DEVICE_STATE_SETTER(822A20B0, SetWrap0, false)
ITC_DEVICE_STATE_SETTER(822A20D8, SetWrap1, false)
ITC_DEVICE_STATE_SETTER(822A2108, SetWrap2, false)
ITC_DEVICE_STATE_SETTER(822A2138, SetWrap3, false)
ITC_DEVICE_STATE_SETTER(822A2168, SetWrap4, false)
ITC_DEVICE_STATE_SETTER(822A2198, SetWrap5, false)
ITC_DEVICE_STATE_SETTER(822A21C8, SetWrap6, false)
ITC_DEVICE_STATE_SETTER(822A21F8, SetWrap7, false)
ITC_DEVICE_STATE_SETTER(822A2220, SetWrap8, false)
ITC_DEVICE_STATE_SETTER(822A2248, SetWrap9, false)
ITC_DEVICE_STATE_SETTER(822A2278, SetWrap10, false)
ITC_DEVICE_STATE_SETTER(822A22A8, SetWrap11, false)
ITC_DEVICE_STATE_SETTER(822A22D8, SetWrap12, false)
ITC_DEVICE_STATE_SETTER(822A2308, SetWrap13, false)
ITC_DEVICE_STATE_SETTER(822A2338, SetWrap14, false)
ITC_DEVICE_STATE_SETTER(822A2368, SetWrap15, false)
ITC_DEVICE_STATE_SETTER(822A2430, SetViewportEnable, false)
ITC_DEVICE_STATE_SETTER(822A2480, SetHighPrecisionBlendEnable, false)
ITC_DEVICE_STATE_SETTER(822A2530, SetHighPrecisionBlendEnable1, false)
ITC_DEVICE_STATE_SETTER(822A25E0, SetHighPrecisionBlendEnable2, false)
ITC_DEVICE_STATE_SETTER(822A2690, SetHighPrecisionBlendEnable3, false)
ITC_DEVICE_STATE_SETTER(822A27E8, SetHalfPixelOffset, false)
ITC_DEVICE_STATE_SETTER(822A2820, SetPrimitiveResetEnable, false)
ITC_DEVICE_STATE_SETTER(822A2850, SetPrimitiveResetIndex, false)
ITC_DEVICE_STATE_SETTER(822A2870, SetAlphaToMaskEnable, false)
ITC_DEVICE_STATE_SETTER(822A28A0, SetAlphaToMaskOffsets, false)
ITC_DEVICE_STATE_SETTER(822A28D8, SetGuardBandX, true)
ITC_DEVICE_STATE_SETTER(822A2908, SetGuardBandY, true)
ITC_DEVICE_STATE_SETTER(822A2938, SetDiscardBandX, true)
ITC_DEVICE_STATE_SETTER(822A2960, SetDiscardBandY, true)
ITC_DEVICE_STATE_SETTER(822A2990, SetHiStencilEnable, false)
ITC_DEVICE_STATE_SETTER(822A29C0, SetHiStencilWriteEnable, false)
ITC_DEVICE_STATE_SETTER(822A29F0, SetHiStencilFunc, false)
ITC_DEVICE_STATE_SETTER(822A2A20, SetHiStencilRef, false)
ITC_DEVICE_STATE_SETTER(822A2A40, SetPresentInterval, false)
ITC_DEVICE_STATE_SETTER(822A2A50, SetPresentImmediateThreshold, false)

#define ITC_DEVICE_SAMPLER_SETTER(address, name, uses_float) \
  extern "C" REX_FUNC(sub_##address) {                        \
    REX_FUNC_PROLOGUE();                                      \
    if (!ITC_FLAG(native_device_state)) {                     \
      __imp__sub_##address(ctx, base);                        \
      return;                                                 \
    }                                                         \
    if (uses_float) {                                         \
      ctx.fpscr.disableFlushMode();                           \
    }                                                         \
    ds::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);       \
  }

ITC_DEVICE_SAMPLER_SETTER(822A31A0, SetAddressU, false)
ITC_DEVICE_SAMPLER_SETTER(822A31F0, SetAddressV, false)
ITC_DEVICE_SAMPLER_SETTER(822A3240, SetAddressW, false)
ITC_DEVICE_SAMPLER_SETTER(822A3130, SetBorderColor, false)
ITC_DEVICE_SAMPLER_SETTER(822A2BF8, SetMagFilter, false)
ITC_DEVICE_SAMPLER_SETTER(822A2A70, SetMinFilter, false)
ITC_DEVICE_SAMPLER_SETTER(822A2D80, SetMipFilter, false)
ITC_DEVICE_SAMPLER_SETTER(822A2F90, SetMipMapLodBias, true)
ITC_DEVICE_SAMPLER_SETTER(822A3030, SetMaxMipLevel, false)
ITC_DEVICE_SAMPLER_SETTER(822A2E70, SetMaxAnisotropy, false)
ITC_DEVICE_SAMPLER_SETTER(822A2CE8, SetMagFilterZ, false)
ITC_DEVICE_SAMPLER_SETTER(822A2B60, SetMinFilterZ, false)
ITC_DEVICE_SAMPLER_SETTER(822A2DD8, SetSeparateZFilterEnable, false)
ITC_DEVICE_SAMPLER_SETTER(822A30B0, SetMinMipLevel, false)
ITC_DEVICE_SAMPLER_SETTER(822A3290, SetTrilinearThreshold, false)
ITC_DEVICE_SAMPLER_SETTER(822A2EE8, SetAnisotropyBias, true)
ITC_DEVICE_SAMPLER_SETTER(822A32E8, SetHGradientExpBias, false)
ITC_DEVICE_SAMPLER_SETTER(822A3340, SetVGradientExpBias, false)
ITC_DEVICE_SAMPLER_SETTER(822A3398, SetWhiteBorderColorW, false)
ITC_DEVICE_SAMPLER_SETTER(822A33F0, SetPointBorderEnable, false)

extern "C" REX_FUNC(sub_822A2470) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_state)) {
    __imp__sub_822A2470(ctx, base);
    return;
  }
  ctx.r3.u64 = ds::GetViewportEnable(base, ctx.r3.u32);
}

// The bound objects and the shader constants: the original runs where the
// plain C++ returns false (GetViewport's copy left to memcpy's override, a
// fence on a full fence list).
#define ITC_DEVICE_STATE_CALL(address, ...)  \
  extern "C" REX_FUNC(sub_##address) {       \
    REX_FUNC_PROLOGUE();                     \
    if (!ITC_FLAG(native_device_state)) {    \
      __imp__sub_##address(ctx, base);       \
      return;                                \
    }                                        \
    __VA_ARGS__;                             \
  }

ITC_DEVICE_STATE_CALL(822A3740, {
  if (!ds::GetViewport(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_822A3740(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r4.u32;  // memcpy's: the destination
})
ITC_DEVICE_STATE_CALL(822A3758, {
  if (!ds::SetStreamSource(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                           ctx.r8.u64)) {
    __imp__sub_822A3758(ctx, base);
  }
})
ITC_DEVICE_STATE_CALL(822A3908, {
  if (!ds::SetIndices(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_822A3908(ctx, base);
  }
})
ITC_DEVICE_STATE_CALL(822A6008, {
  if (!ds::SetTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u64)) {
    __imp__sub_822A6008(ctx, base);
  }
})
ITC_DEVICE_STATE_CALL(822A3B30, ds::SetBlendControl(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_DEVICE_STATE_CALL(822A81F8, ds::SetVertexShaderConstantF(base, ctx.r3.u32, ctx.r4.u32,
                                                             ctx.r5.u32, ctx.r6.u32, ctx.r7.u64))
ITC_DEVICE_STATE_CALL(822A8308, ds::SetPixelShaderConstantF(base, ctx.r3.u32, ctx.r4.u32,
                                                            ctx.r5.u32, ctx.r6.u32, ctx.r7.u64))
ITC_DEVICE_STATE_CALL(822A8470, ds::SetPixelShaderConstantB(base, ctx.r3.u32, ctx.r4.u32,
                                                            ctx.r5.u32, ctx.r6.u32))
// VertexDeclaration::FromFvf (sub_822A6A88) has no adapter: its only caller
// is Device::SetFVF, whose plain C++ calls it.
ITC_DEVICE_STATE_CALL(822AA0E8, ds::SetFVF(base, ctx.r3.u32, ctx.r4.u32))

// Device::SetScreenScissor (sub_822A0F80): Dropped. It only wrote the window
// scissor (or one per tile, and the binning packets) into the command
// buffer, which nothing reads since the emulated GPU went (the kick only
// looks for fences and interrupts, src/native/gpu_link.cpp); our renderer
// takes the scissor from +11600 and +12416. Its recompiled callers
// (Device::DirtyAllState, sub_8229F680, Device::Resolve) don't read r3 after
// it. The original runs with the flag off.
extern "C" REX_FUNC(sub_822A0F80) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_state)) {
    __imp__sub_822A0F80(ctx, base);
  }
}

ITC_DEVICE_STATE_CALL(822A3450, ds::SetScissorRect(base, ctx.r3.u32, ctx.r4.u32))
ITC_DEVICE_STATE_CALL(822A3D20, ds::SetScissorTestEnable(base, ctx.r3.u32, ctx.r4.u32))
ITC_DEVICE_STATE_CALL(822A3518, {
  ctx.fpscr.disableFlushMode();
  ds::SetViewport(base, ctx.r3.u32, ctx.r4.u32);
})
ITC_DEVICE_STATE_CALL(822A3C88, {
  ctx.fpscr.disableFlushMode();
  ds::SetSurfaceInfo(base, ctx.r3.u32, ctx.r4.u32);
})
// SetRenderTarget, SetRenderTarget_External and SetDepthStencilSurface: the
// originals leave a temporary in r3, which none of their callers read; these
// leave the device.
ITC_DEVICE_STATE_CALL(822A3D30, {
  ctx.fpscr.disableFlushMode();
  ds::SetRenderTarget(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
})
ITC_DEVICE_STATE_CALL(822A4340, {
  ctx.fpscr.disableFlushMode();
  ds::SetRenderTargetExternal(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
})
ITC_DEVICE_STATE_CALL(822A4060, {
  ctx.fpscr.disableFlushMode();
  ds::SetDepthStencilSurface(base, ctx.r3.u32, ctx.r4.u32);
})
