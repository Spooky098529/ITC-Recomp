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

// The second migration's: Options::ShowVolumeBar sub_821BE5A8 (r3 the node,
// r4 the volume; its override the pause movie log's wrap), ShowVolumeNumbers
// sub_821BE820, ShowSliderNumbers sub_821BECA8, ShowSliderMarks
// sub_821BED38, ShowHudValues sub_821BE290, StepSound sub_821BE5F8 and
// StepSliders sub_821BEAC8 (no arguments).
void OptionsShowVolumeBar(PPCContext& ctx, uint8_t* base);
void OptionsShowVolumeNumbers(PPCContext& ctx, uint8_t* base);
void OptionsShowSliderNumbers(PPCContext& ctx, uint8_t* base);
void OptionsShowSliderMarks(PPCContext& ctx, uint8_t* base);
void OptionsShowHudValues(PPCContext& ctx, uint8_t* base);
void OptionsStepSound(PPCContext& ctx, uint8_t* base);
void OptionsStepSliders(PPCContext& ctx, uint8_t* base);
// ShowButtonList sub_821BEFE8, ShowAnalogList sub_821BF520, ShowButtonClashes
// sub_821BF158, ShowAnalogClashes sub_821BF698 (no arguments); the steps
// EnterAnalog sub_821BDE88, MoveSaveDialogCursor sub_821C09B0,
// MoveCategory6DialogCursor sub_821C0B50, ChangeSaveDialog sub_821C08B0,
// ChangeCategory6Dialog sub_821C0A50 (r3 the step word).
void OptionsShowButtonList(PPCContext& ctx, uint8_t* base);
void OptionsShowAnalogList(PPCContext& ctx, uint8_t* base);
void OptionsShowButtonClashes(PPCContext& ctx, uint8_t* base);
void OptionsShowAnalogClashes(PPCContext& ctx, uint8_t* base);
void OptionsEnterAnalog(PPCContext& ctx, uint8_t* base);
void OptionsMoveSaveDialogCursor(PPCContext& ctx, uint8_t* base);
void OptionsMoveCategory6DialogCursor(PPCContext& ctx, uint8_t* base);
void OptionsChangeSaveDialog(PPCContext& ctx, uint8_t* base);
void OptionsChangeCategory6Dialog(PPCContext& ctx, uint8_t* base);
// Draw sub_821C80F0 (r4 the dispatcher), Update sub_821C81E8, SavePage
// sub_821C6F58 (r3 passed on), Category6Page sub_821BD228 (r3, r4 passed
// on), StepHud sub_821BE3E0, ShowVolumeBars sub_821BE8A8, ShowPrompts
// sub_821C1068 (r3 to r8), Settings::Reload sub_821C0FF0, OnLeft
// sub_821B9E70 (the allocation layer's pages).
void OptionsDraw(PPCContext& ctx, uint8_t* base);
void OptionsUpdate(PPCContext& ctx, uint8_t* base);
void OptionsSavePage(PPCContext& ctx, uint8_t* base);
void OptionsCategory6Page(PPCContext& ctx, uint8_t* base);
void OptionsStepHud(PPCContext& ctx, uint8_t* base);
void OptionsShowVolumeBars(PPCContext& ctx, uint8_t* base);
void OptionsShowPrompts(PPCContext& ctx, uint8_t* base);
void OptionsSettingsReload(PPCContext& ctx, uint8_t* base);
void OptionsOnLeft(PPCContext& ctx, uint8_t* base);
// CategoryPage sub_821BC670, SetUpMovie sub_821BCFF0 (no arguments), Show
// sub_821C1158 (r4 the dispatcher); the steps MoveCategoryCursor
// sub_821C2BE0, MoveHudCursor sub_821C2E28, MoveSoundCursor sub_821C3008,
// ChangeSoundValue sub_821C3278, MoveSlidersCursor sub_821C3398 (r3 the
// step word).
void OptionsCategoryPage(PPCContext& ctx, uint8_t* base);
void OptionsSetUpMovie(PPCContext& ctx, uint8_t* base);
void OptionsShow(PPCContext& ctx, uint8_t* base);
void OptionsMoveCategoryCursor(PPCContext& ctx, uint8_t* base);
void OptionsMoveHudCursor(PPCContext& ctx, uint8_t* base);
void OptionsMoveSoundCursor(PPCContext& ctx, uint8_t* base);
void OptionsChangeSoundValue(PPCContext& ctx, uint8_t* base);
void OptionsMoveSlidersCursor(PPCContext& ctx, uint8_t* base);
// The steps ChangeSlidersValue sub_821C35F0, LeaveButtons sub_821C2778 and
// LeaveAnalog sub_821C2A10 (r3 the step word).
void OptionsChangeSlidersValue(PPCContext& ctx, uint8_t* base);
void OptionsLeaveButtons(PPCContext& ctx, uint8_t* base);
void OptionsLeaveAnalog(PPCContext& ctx, uint8_t* base);

// The category pages (no arguments): Options::HudPage sub_821C1270,
// SlidersPage sub_821C1888, ButtonsPage sub_821C1D08, AnalogPage
// sub_821C21A8.
void OptionsHudPage(PPCContext& ctx, uint8_t* base);
void OptionsSlidersPage(PPCContext& ctx, uint8_t* base);
void OptionsButtonsPage(PPCContext& ctx, uint8_t* base);
void OptionsAnalogPage(PPCContext& ctx, uint8_t* base);
// Options::SoundPage sub_821C1498 and LeaveSoundPage sub_821BA380 (r3 as
// Options::Draw leaves it).
void OptionsSoundPage(PPCContext& ctx, uint8_t* base);
void OptionsLeaveSoundPage(PPCContext& ctx, uint8_t* base);
// The set-ups (no arguments): Options::SetUpCategoryPage sub_821C4BE0,
// SetUpHudPage sub_821C4DB0, SetUpSoundPage sub_821C4FD0, SetUpSlidersPage
// sub_821C51D0, SetUpButtonsPage sub_821C5580, SetUpAnalogPage sub_821C57D0.
void OptionsSetUpCategoryPage(PPCContext& ctx, uint8_t* base);
void OptionsSetUpHudPage(PPCContext& ctx, uint8_t* base);
void OptionsSetUpSoundPage(PPCContext& ctx, uint8_t* base);
void OptionsSetUpSlidersPage(PPCContext& ctx, uint8_t* base);
void OptionsSetUpButtonsPage(PPCContext& ctx, uint8_t* base);
void OptionsSetUpAnalogPage(PPCContext& ctx, uint8_t* base);
// The steps (r3 the step word): Options::FadeIn sub_821C7080, CloseHud
// sub_821C75E0, CloseSound sub_821C7710, CloseSliders sub_821C7868,
// CloseButtons sub_821C7B08, CloseAnalog sub_821C7D18.
void OptionsFadeIn(PPCContext& ctx, uint8_t* base);
void OptionsCloseHud(PPCContext& ctx, uint8_t* base);
void OptionsCloseSound(PPCContext& ctx, uint8_t* base);
void OptionsCloseSliders(PPCContext& ctx, uint8_t* base);
void OptionsCloseButtons(PPCContext& ctx, uint8_t* base);
void OptionsCloseAnalog(PPCContext& ctx, uint8_t* base);

// The type 2 and 4 modes' Inits (r3 the machine): Type2Options::Init
// sub_821C9E00, Type4Options::Init sub_821CA458, Type2ControllerOptions::Init
// sub_821C8578, Type4ControllerOptions::Init sub_821C8A60 by `address`.
void OptionsInit(PPCContext& ctx, uint8_t* base, uint32_t address);

}  // namespace hooks
