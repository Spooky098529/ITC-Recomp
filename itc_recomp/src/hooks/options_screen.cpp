// The options screen natively: adapters in front of src/options_screen/
// (docs/functions/options-screen.md), with the register steps the harness
// shares (hooks/options_screen.h).

#include "hooks/options_screen.h"

#include "base/flags.h"
#include "options_screen/options_screen.h"

ITC_FLAG_BOOL(native_options_screen, true, "ITC",
              "Run the front end's options screen (docs/functions/options-screen.md: the "
              "controls' tables Controls::ButtonIndex sub_821BA950, ButtonCode sub_821BAA18, "
              "AnalogIndex sub_821BAAE0 and AnalogPicture sub_821BAD08; Options::IsButtonTaken "
              "sub_821BA7E0, IsAnalogTaken sub_821BAB58, StepButtons sub_821BA550 and StepAnalog "
              "sub_821BABD8; Settings::Reset sub_821B9DB8) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=options_screen)");

namespace hooks {

void OptionsButtonIndex(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::ButtonIndex(ctx.r3.u32))));
}

void OptionsButtonCode(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = options_screen::ButtonCode(ctx.r3.u32);
}

void OptionsAnalogIndex(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::AnalogIndex(ctx.r3.u32))));
}

void OptionsAnalogPicture(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::AnalogPicture(ctx.r3.u32))));
}

void OptionsIsButtonTaken(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::IsButtonTaken(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsIsAnalogTaken(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::IsAnalogTaken(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsStepButtons(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepButtons(base);
}

void OptionsStepAnalog(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepAnalog(base);
}

void OptionsSettingsReset(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::SettingsReset(base, ctx.r3.u32, ctx.r1.u32);
}

}  // namespace hooks

#define ITC_OPTIONS_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_options_screen)) { \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_OPTIONS_SCREEN(821BA950, OptionsButtonIndex)
ITC_OPTIONS_SCREEN(821BAA18, OptionsButtonCode)
ITC_OPTIONS_SCREEN(821BAAE0, OptionsAnalogIndex)
ITC_OPTIONS_SCREEN(821BAD08, OptionsAnalogPicture)
ITC_OPTIONS_SCREEN(821BA7E0, OptionsIsButtonTaken)
ITC_OPTIONS_SCREEN(821BAB58, OptionsIsAnalogTaken)
ITC_OPTIONS_SCREEN(821BA550, OptionsStepButtons)
ITC_OPTIONS_SCREEN(821BABD8, OptionsStepAnalog)
ITC_OPTIONS_SCREEN(821B9DB8, OptionsSettingsReset)
