// The app framework natively: adapters in front of src/framework/
// (docs/functions/framework.md). The getters and the thunks are reached
// through the framework's vtables (and the folded getters also from other
// classes' code), so their adapters stay while recompiled code calls them.

#include "guest.h"

#include "base/flags.h"
#include "framework/framework.h"

ITC_FLAG_BOOL(native_framework, true, "ITC",
              "Run the app framework (docs/functions/framework.md): the framework's flags "
              "sub_82090760 and sub_82090768, the folded getters sub_82090770, sub_82090778, "
              "sub_82090788, sub_8248F030 and sub_824892F8, the interfaces' thunks sub_82090828, "
              "sub_82090830, sub_82090838, sub_82090860, sub_82090868, sub_820908E0, sub_820908E8 "
              "and sub_82090908, DebugText::SetPosition sub_82091050 and DebugFont::SetPosition "
              "sub_8258BE08) natively instead of through the translated code (the same results; "
              "checked by --run_harnesses=framework)");

namespace fw = framework;

#define ITC_FRAMEWORK_FALLBACK(address)  \
  REX_FUNC_PROLOGUE();                   \
  if (!ITC_FLAG(native_framework)) {     \
    __imp__sub_##address(ctx, base);     \
    return;                              \
  }

// A function of r3 answering a word in r3.
#define ITC_FRAMEWORK_WORD(address, function) \
  extern "C" REX_FUNC(sub_##address) {        \
    ITC_FRAMEWORK_FALLBACK(address)           \
    ctx.r3.u64 = fw::function(base, ctx.r3.u32); \
  }

ITC_FRAMEWORK_WORD(82090760, Flag133)
ITC_FRAMEWORK_WORD(82090768, Flag134)
ITC_FRAMEWORK_WORD(82090770, Word60)
ITC_FRAMEWORK_WORD(82090778, Word64)
ITC_FRAMEWORK_WORD(82090788, Word88)
ITC_FRAMEWORK_WORD(8248F030, Word68)
ITC_FRAMEWORK_WORD(824892F8, Word44)
ITC_FRAMEWORK_WORD(82090830, InputManagerFrom36)
ITC_FRAMEWORK_WORD(82090828, RendererFrom52)
ITC_FRAMEWORK_WORD(82090838, WorkerPoolFrom52)
ITC_FRAMEWORK_WORD(82090860, RendererOrTextFrom24)
ITC_FRAMEWORK_WORD(82090868, SceneFrom52)
ITC_FRAMEWORK_WORD(820908E0, RendererFrom36)
ITC_FRAMEWORK_WORD(820908E8, WorkerPoolFrom36)
ITC_FRAMEWORK_WORD(82090908, SceneFrom36)

// DebugFont::SetPosition: r3 = x, r4 = y; r11 left at the position, as the
// original's.
extern "C" REX_FUNC(sub_8258BE08) {
  ITC_FRAMEWORK_FALLBACK(8258BE08)
  fw::DebugFontSetPosition(base, ctx.r3.u32, ctx.r4.u32);
  ctx.r11.u64 = fw::kDebugFontPosition;
}

// DebugText::SetPosition: r4, r5 = pointers to x and y; r3, r4 the words
// and r11 as the original leaves them.
extern "C" REX_FUNC(sub_82091050) {
  ITC_FRAMEWORK_FALLBACK(82091050)
  const uint32_t x = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t y = guest::LoadU32(base, ctx.r5.u32);
  fw::DebugTextSetPosition(base, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
  ctx.r11.u64 = fw::kDebugFontPosition;
}
