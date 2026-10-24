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
              "the render state setters sub_822A1260 to sub_822A2A50 and "
              "Device::GetRenderState_ViewportEnable sub_822A2470) natively instead of "
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

extern "C" REX_FUNC(sub_822A2470) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_device_state)) {
    __imp__sub_822A2470(ctx, base);
    return;
  }
  ctx.r3.u64 = ds::GetViewportEnable(base, ctx.r3.u32);
}
