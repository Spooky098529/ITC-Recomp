// The options screen's register steps (src/options_screen/,
// docs/functions/options-screen.md), shared by the adapters
// (src/hooks/options_screen.cpp, behind --native_options_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"

namespace hooks {

// Controls::ButtonIndex sub_821BA950, ButtonCode sub_821BAA18, AnalogIndex
// sub_821BAAE0, AnalogPicture sub_821BAD08 (r3 the code or index).
void OptionsButtonIndex(PPCContext& ctx, uint8_t* base);
void OptionsButtonCode(PPCContext& ctx, uint8_t* base);
void OptionsAnalogIndex(PPCContext& ctx, uint8_t* base);
void OptionsAnalogPicture(PPCContext& ctx, uint8_t* base);
// Options::IsButtonTaken sub_821BA7E0, IsAnalogTaken sub_821BAB58 (r3 the
// item); StepButtons sub_821BA550, StepAnalog sub_821BABD8 (no arguments).
void OptionsIsButtonTaken(PPCContext& ctx, uint8_t* base);
void OptionsIsAnalogTaken(PPCContext& ctx, uint8_t* base);
void OptionsStepButtons(PPCContext& ctx, uint8_t* base);
void OptionsStepAnalog(PPCContext& ctx, uint8_t* base);
// Settings::Reset sub_821B9DB8 (r3 the settings).
void OptionsSettingsReset(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
