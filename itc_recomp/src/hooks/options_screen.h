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

// Settings::Read sub_821BBF00, Settings::Write sub_821BAE10 (no arguments).
void OptionsSettingsRead(PPCContext& ctx, uint8_t* base);
void OptionsSettingsWrite(PPCContext& ctx, uint8_t* base);
// Options::OnEnter sub_821BB9A8 (r3 the machine; the allocation layer's
// pages), WaitForMovie sub_821BBBA0 (r3 the machine), OpenMovie sub_821B9F88,
// OnClosed sub_821BCE38 (r3 the machine, r4 the dispatcher).
void OptionsOnEnter(PPCContext& ctx, uint8_t* base);
void OptionsWaitForMovie(PPCContext& ctx, uint8_t* base);
void OptionsOpenMovie(PPCContext& ctx, uint8_t* base);
void OptionsOnClosed(PPCContext& ctx, uint8_t* base);

// Options::Close sub_821BCD28 and Leave sub_821BCF08 (r3 the machine, r4 the
// dispatcher), ClosePage sub_821BD330 (r3 the dispatcher), Wait sub_821BD3C8
// (r3 the step word).
void OptionsClose(PPCContext& ctx, uint8_t* base);
void OptionsLeave(PPCContext& ctx, uint8_t* base);
void OptionsClosePage(PPCContext& ctx, uint8_t* base);
void OptionsWait(PPCContext& ctx, uint8_t* base);

// The steps on the movie (r3 the step word): Options::OpenCategories
// sub_821BD468, ReturnToCategories sub_821BD5B0, EnterHud sub_821BD728,
// ChangeHudValue sub_821BD800, EnterSound sub_821BD908, EnterSliders
// sub_821BD9A0, EnterButtons sub_821BDAD0.
void OptionsOpenCategories(PPCContext& ctx, uint8_t* base);
void OptionsReturnToCategories(PPCContext& ctx, uint8_t* base);
void OptionsEnterHud(PPCContext& ctx, uint8_t* base);
void OptionsChangeHudValue(PPCContext& ctx, uint8_t* base);
void OptionsEnterSound(PPCContext& ctx, uint8_t* base);
void OptionsEnterSliders(PPCContext& ctx, uint8_t* base);
void OptionsEnterButtons(PPCContext& ctx, uint8_t* base);

// The node helpers: Options::SetCategoryArrow sub_821BE218 (r3 the value),
// ShowVolumeNumber sub_821BE518 (r3 the node, r4 the volume), ShowMusicName
// sub_821BE930 (r3), ShowSlider sub_821BE988 and ShowSliderMark sub_821BEA48
// (r3 the slider, r4 the value), ShowButtonPicture sub_821BF0F0 and
// ShowAnalogPicture sub_821BF630 (r3 the picture), ResetNode sub_821BFA68
// (r3 the group, r4 the child).
void OptionsSetCategoryArrow(PPCContext& ctx, uint8_t* base);
void OptionsShowVolumeNumber(PPCContext& ctx, uint8_t* base);
void OptionsShowMusicName(PPCContext& ctx, uint8_t* base);
void OptionsShowSlider(PPCContext& ctx, uint8_t* base);
void OptionsShowSliderMark(PPCContext& ctx, uint8_t* base);
void OptionsShowButtonPicture(PPCContext& ctx, uint8_t* base);
void OptionsShowAnalogPicture(PPCContext& ctx, uint8_t* base);
void OptionsResetNode(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
