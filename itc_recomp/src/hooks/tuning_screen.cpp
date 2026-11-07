// The tuning screen natively: adapters in front of src/tuning_screen/
// (docs/functions/tuning-screen.md), with the register steps the harness
// shares (hooks/tuning_screen.h). The screen's update and refresh, and other
// front-end screens, are still recompiled and call these, so the adapters
// stay.

#include "hooks/tuning_screen.h"

#include "base/flags.h"
#include "tuning_screen/tuning_screen.h"

ITC_FLAG_BOOL(native_tuning_screen, true, "ITC",
              "Run the tuning screen's functions (docs/functions/tuning-screen.md: the part "
              "tables' TuningScreen::CategoryItemCount sub_821CC6C0, IsItemAvailable "
              "sub_821CDB18, WheelNumber sub_821CDED8, GetPrice sub_821CD160, GetCurrentChoice "
              "sub_821CD868 and GetChoiceFlag sub_821CDC38) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=tuning_screen)");

namespace hooks {

namespace ts = tuning_screen;

void TuningCategoryItemCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::CategoryItemCount(base, ctx.r3.u32);
}

void TuningIsItemAvailable(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::IsItemAvailable(base, ctx.r3.u32, ctx.r4.u64);
}

void TuningWheelNumber(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::WheelNumber(ctx.r4.u64, ctx.r5.u64);
}

void TuningGetPrice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetPrice(base, ctx.r3.u32);
}

void TuningGetCurrentChoice(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetCurrentChoice(base, ctx.r3.u32);
}

void TuningGetChoiceFlag(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::GetChoiceFlag(base, ctx.r3.u32, ctx.r4.u64);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_TUNING_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_tuning_screen)) { \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

ITC_TUNING_SCREEN(821CC6C0, TuningCategoryItemCount)
ITC_TUNING_SCREEN(821CDB18, TuningIsItemAvailable)
ITC_TUNING_SCREEN(821CDED8, TuningWheelNumber)
ITC_TUNING_SCREEN(821CD160, TuningGetPrice)
ITC_TUNING_SCREEN(821CD868, TuningGetCurrentChoice)
ITC_TUNING_SCREEN(821CDC38, TuningGetChoiceFlag)
