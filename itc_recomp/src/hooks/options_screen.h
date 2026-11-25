// The options screen's register steps (src/options_screen/,
// docs/functions/options-screen.md), shared by the adapters
// (src/hooks/options_screen.cpp, behind --native_options_screen) and the
// harness (without the flag). Each leaves r3 as the original does.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Controls::ButtonIndex sub_821BA950, ButtonCode sub_821BAA18, AnalogIndex
// sub_821BAAE0, AnalogPicture sub_821BAD08 (r3 the code or index).
void OptionsButtonIndex(lifted::Regs& ctx);
void OptionsButtonCode(lifted::Regs& ctx);
void OptionsAnalogIndex(lifted::Regs& ctx);
void OptionsAnalogPicture(lifted::Regs& ctx);
// Options::IsButtonTaken sub_821BA7E0, IsAnalogTaken sub_821BAB58 (r3 the
// item); StepButtons sub_821BA550, StepAnalog sub_821BABD8 (no arguments).
void OptionsIsButtonTaken(lifted::Regs& ctx);
void OptionsIsAnalogTaken(lifted::Regs& ctx);
void OptionsStepButtons(lifted::Regs& ctx);
void OptionsStepAnalog(lifted::Regs& ctx);
// Settings::Reset sub_821B9DB8 (r3 the settings).
void OptionsSettingsReset(lifted::Regs& ctx);

// Settings::Read sub_821BBF00, Settings::Write sub_821BAE10 (no arguments).
void OptionsSettingsRead(lifted::Regs& ctx);
void OptionsSettingsWrite(lifted::Regs& ctx);
// Options::OnEnter sub_821BB9A8 (r3 the machine; the allocation layer's
// pages), WaitForMovie sub_821BBBA0 (r3 the machine), OpenMovie sub_821B9F88,
// OnClosed sub_821BCE38 (r3 the machine, r4 the dispatcher).
void OptionsOnEnter(lifted::Regs& ctx);
void OptionsWaitForMovie(lifted::Regs& ctx);
void OptionsOpenMovie(lifted::Regs& ctx);
void OptionsOnClosed(lifted::Regs& ctx);

// Options::Close sub_821BCD28 and Leave sub_821BCF08 (r3 the machine, r4 the
// dispatcher), ClosePage sub_821BD330 (r3 the dispatcher), Wait sub_821BD3C8
// (r3 the step word).
void OptionsClose(lifted::Regs& ctx);
void OptionsLeave(lifted::Regs& ctx);
void OptionsClosePage(lifted::Regs& ctx);
void OptionsWait(lifted::Regs& ctx);

// The steps on the movie (r3 the step word): Options::OpenCategories
// sub_821BD468, ReturnToCategories sub_821BD5B0, EnterHud sub_821BD728,
// ChangeHudValue sub_821BD800, EnterSound sub_821BD908, EnterSliders
// sub_821BD9A0, EnterButtons sub_821BDAD0.
void OptionsOpenCategories(lifted::Regs& ctx);
void OptionsReturnToCategories(lifted::Regs& ctx);
void OptionsEnterHud(lifted::Regs& ctx);
void OptionsChangeHudValue(lifted::Regs& ctx);
void OptionsEnterSound(lifted::Regs& ctx);
void OptionsEnterSliders(lifted::Regs& ctx);
void OptionsEnterButtons(lifted::Regs& ctx);

// The node helpers: Options::SetCategoryArrow sub_821BE218 (r3 the value),
// ShowVolumeNumber sub_821BE518 (r3 the node, r4 the volume), ShowMusicName
// sub_821BE930 (r3), ShowSlider sub_821BE988 and ShowSliderMark sub_821BEA48
// (r3 the slider, r4 the value), ShowButtonPicture sub_821BF0F0 and
// ShowAnalogPicture sub_821BF630 (r3 the picture), ResetNode sub_821BFA68
// (r3 the group, r4 the child).
void OptionsSetCategoryArrow(lifted::Regs& ctx);
void OptionsShowVolumeNumber(lifted::Regs& ctx);
void OptionsShowMusicName(lifted::Regs& ctx);
void OptionsShowSlider(lifted::Regs& ctx);
void OptionsShowSliderMark(lifted::Regs& ctx);
void OptionsShowButtonPicture(lifted::Regs& ctx);
void OptionsShowAnalogPicture(lifted::Regs& ctx);
void OptionsResetNode(lifted::Regs& ctx);

// The second migration's: Options::ShowVolumeBar sub_821BE5A8 (r3 the node,
// r4 the volume; its override the pause movie log's wrap), ShowVolumeNumbers
// sub_821BE820, ShowSliderNumbers sub_821BECA8, ShowSliderMarks
// sub_821BED38, ShowHudValues sub_821BE290, StepSound sub_821BE5F8 and
// StepSliders sub_821BEAC8 (no arguments).
void OptionsShowVolumeBar(lifted::Regs& ctx);
void OptionsShowVolumeNumbers(lifted::Regs& ctx);
void OptionsShowSliderNumbers(lifted::Regs& ctx);
void OptionsShowSliderMarks(lifted::Regs& ctx);
void OptionsShowHudValues(lifted::Regs& ctx);
void OptionsStepSound(lifted::Regs& ctx);
void OptionsStepSliders(lifted::Regs& ctx);
// ShowButtonList sub_821BEFE8, ShowAnalogList sub_821BF520, ShowButtonClashes
// sub_821BF158, ShowAnalogClashes sub_821BF698 (no arguments); the steps
// EnterAnalog sub_821BDE88, MoveSaveDialogCursor sub_821C09B0,
// MoveCategory6DialogCursor sub_821C0B50, ChangeSaveDialog sub_821C08B0,
// ChangeCategory6Dialog sub_821C0A50 (r3 the step word).
void OptionsShowButtonList(lifted::Regs& ctx);
void OptionsShowAnalogList(lifted::Regs& ctx);
void OptionsShowButtonClashes(lifted::Regs& ctx);
void OptionsShowAnalogClashes(lifted::Regs& ctx);
void OptionsEnterAnalog(lifted::Regs& ctx);
void OptionsMoveSaveDialogCursor(lifted::Regs& ctx);
void OptionsMoveCategory6DialogCursor(lifted::Regs& ctx);
void OptionsChangeSaveDialog(lifted::Regs& ctx);
void OptionsChangeCategory6Dialog(lifted::Regs& ctx);
// Draw sub_821C80F0 (r4 the dispatcher), Update sub_821C81E8, SavePage
// sub_821C6F58 (r3 passed on), Category6Page sub_821BD228 (r3, r4 passed
// on), StepHud sub_821BE3E0, ShowVolumeBars sub_821BE8A8, ShowPrompts
// sub_821C1068 (r3 to r8), Settings::Reload sub_821C0FF0, OnLeft
// sub_821B9E70 (the allocation layer's pages).
void OptionsDraw(lifted::Regs& ctx);
void OptionsUpdate(lifted::Regs& ctx);
void OptionsSavePage(lifted::Regs& ctx);
void OptionsCategory6Page(lifted::Regs& ctx);
void OptionsStepHud(lifted::Regs& ctx);
void OptionsShowVolumeBars(lifted::Regs& ctx);
void OptionsShowPrompts(lifted::Regs& ctx);
void OptionsSettingsReload(lifted::Regs& ctx);
void OptionsOnLeft(lifted::Regs& ctx);
// CategoryPage sub_821BC670, SetUpMovie sub_821BCFF0 (no arguments), Show
// sub_821C1158 (r4 the dispatcher); the steps MoveCategoryCursor
// sub_821C2BE0, MoveHudCursor sub_821C2E28, MoveSoundCursor sub_821C3008,
// ChangeSoundValue sub_821C3278, MoveSlidersCursor sub_821C3398 (r3 the
// step word).
void OptionsCategoryPage(lifted::Regs& ctx);
void OptionsSetUpMovie(lifted::Regs& ctx);
void OptionsShow(lifted::Regs& ctx);
void OptionsMoveCategoryCursor(lifted::Regs& ctx);
void OptionsMoveHudCursor(lifted::Regs& ctx);
void OptionsMoveSoundCursor(lifted::Regs& ctx);
void OptionsChangeSoundValue(lifted::Regs& ctx);
void OptionsMoveSlidersCursor(lifted::Regs& ctx);
// The steps ChangeSlidersValue sub_821C35F0, LeaveButtons sub_821C2778 and
// LeaveAnalog sub_821C2A10 (r3 the step word).
void OptionsChangeSlidersValue(lifted::Regs& ctx);
void OptionsLeaveButtons(lifted::Regs& ctx);
void OptionsLeaveAnalog(lifted::Regs& ctx);

// The category pages (no arguments): Options::HudPage sub_821C1270,
// SlidersPage sub_821C1888, ButtonsPage sub_821C1D08, AnalogPage
// sub_821C21A8.
void OptionsHudPage(lifted::Regs& ctx);
void OptionsSlidersPage(lifted::Regs& ctx);
void OptionsButtonsPage(lifted::Regs& ctx);
void OptionsAnalogPage(lifted::Regs& ctx);
// Options::SoundPage sub_821C1498 and LeaveSoundPage sub_821BA380 (r3 as
// Options::Draw leaves it).
void OptionsSoundPage(lifted::Regs& ctx);
void OptionsLeaveSoundPage(lifted::Regs& ctx);
// The set-ups (no arguments): Options::SetUpCategoryPage sub_821C4BE0,
// SetUpHudPage sub_821C4DB0, SetUpSoundPage sub_821C4FD0, SetUpSlidersPage
// sub_821C51D0, SetUpButtonsPage sub_821C5580, SetUpAnalogPage sub_821C57D0.
void OptionsSetUpCategoryPage(lifted::Regs& ctx);
void OptionsSetUpHudPage(lifted::Regs& ctx);
void OptionsSetUpSoundPage(lifted::Regs& ctx);
void OptionsSetUpSlidersPage(lifted::Regs& ctx);
void OptionsSetUpButtonsPage(lifted::Regs& ctx);
void OptionsSetUpAnalogPage(lifted::Regs& ctx);
// The steps (r3 the step word): Options::FadeIn sub_821C7080, CloseHud
// sub_821C75E0, CloseSound sub_821C7710, CloseSliders sub_821C7868,
// CloseButtons sub_821C7B08, CloseAnalog sub_821C7D18.
void OptionsFadeIn(lifted::Regs& ctx);
void OptionsCloseHud(lifted::Regs& ctx);
void OptionsCloseSound(lifted::Regs& ctx);
void OptionsCloseSliders(lifted::Regs& ctx);
void OptionsCloseButtons(lifted::Regs& ctx);
void OptionsCloseAnalog(lifted::Regs& ctx);

// The type 2 and 4 modes' Inits (r3 the machine): Type2Options::Init
// sub_821C9E00, Type4Options::Init sub_821CA458, Type2ControllerOptions::Init
// sub_821C8578, Type4ControllerOptions::Init sub_821C8A60 by `address`.
void OptionsInit(lifted::Regs& ctx, uint32_t address);

// The load and save pages (docs/functions/options-load-save.md; behind
// --native_options_load_save): LoadPage::Close sub_821B9FB0, SavePage::Close sub_821BA508,
// OptionsSavePages::Cancel sub_821B9FF8, the sub-pages LoadPage::CheckDevice
// sub_821BBC10, Answer sub_821BC7B0, Note sub_821BC940, Wait sub_821C2620,
// DeviceAnswer sub_821BBC68, SavePage::CheckDevice sub_821BBCC0, Answer
// sub_821BCA60, Note sub_821BCC10, Wait sub_821BBD18 (no arguments read); the
// steps LoadPage::Leave sub_821C6468 and SavePage::Leave sub_821C6550 (r3 the
// step word).
void LoadPageClose(lifted::Regs& ctx);
void SavePageClose(lifted::Regs& ctx);
void SavePagesCancel(lifted::Regs& ctx);
void LoadPageCheckDevice(lifted::Regs& ctx);
void LoadPageAnswer(lifted::Regs& ctx);
void LoadPageNote(lifted::Regs& ctx);
void LoadPageWait(lifted::Regs& ctx);
void LoadPageDeviceAnswer(lifted::Regs& ctx);
void SavePageCheckDevice(lifted::Regs& ctx);
void SavePageAnswer(lifted::Regs& ctx);
void SavePageNote(lifted::Regs& ctx);
void SavePageWait(lifted::Regs& ctx);
void LoadPageLeave(lifted::Regs& ctx);
void SavePageLeave(lifted::Regs& ctx);

}  // namespace hooks
