// The options screen natively: adapters in front of src/options_screen/
// (docs/functions/options-screen.md), with the register steps the harness
// shares (hooks/options_screen.h).

#include "hooks/options_screen.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "options_screen/options_screen.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_options_screen, true, "ITC",
              "Run the front end's options screen (docs/functions/options-screen.md: the "
              "controls' tables Controls::ButtonIndex sub_821BA950, ButtonCode sub_821BAA18, "
              "AnalogIndex sub_821BAAE0 and AnalogPicture sub_821BAD08; Options::IsButtonTaken "
              "sub_821BA7E0, IsAnalogTaken sub_821BAB58, StepButtons sub_821BA550 and StepAnalog "
              "sub_821BABD8; Settings::Reset sub_821B9DB8, Read sub_821BBF00 and Write "
              "sub_821BAE10; Options::OnEnter sub_821BB9A8 (with --native_heap), WaitForMovie "
              "sub_821BBBA0, OpenMovie sub_821B9F88, OnClosed sub_821BCE38, Close sub_821BCD28, "
              "Leave sub_821BCF08, ClosePage sub_821BD330 and Wait sub_821BD3C8; the steps OpenCategories "
              "sub_821BD468, ReturnToCategories sub_821BD5B0, EnterHud sub_821BD728, "
              "ChangeHudValue sub_821BD800, EnterSound sub_821BD908, EnterSliders sub_821BD9A0 "
              "and EnterButtons sub_821BDAD0; the node helpers SetCategoryArrow sub_821BE218, "
              "ShowVolumeNumber sub_821BE518, ShowMusicName sub_821BE930, ShowSlider "
              "sub_821BE988, ShowSliderMark sub_821BEA48, ShowButtonPicture sub_821BF0F0, "
              "ShowAnalogPicture sub_821BF630 and ResetNode sub_821BFA68; ShowVolumeBar "
              "sub_821BE5A8 (inside the pause movie log's wrap), ShowVolumeNumbers sub_821BE820, "
              "ShowSliderNumbers sub_821BECA8, ShowSliderMarks sub_821BED38, ShowHudValues "
              "sub_821BE290, StepSound sub_821BE5F8, StepSliders sub_821BEAC8, ShowButtonList "
              "sub_821BEFE8, ShowAnalogList sub_821BF520, ShowButtonClashes sub_821BF158 and "
              "ShowAnalogClashes sub_821BF698; the steps EnterAnalog sub_821BDE88, "
              "MoveSaveDialogCursor sub_821C09B0, MoveCategory6DialogCursor sub_821C0B50, "
              "ChangeSaveDialog sub_821C08B0 and ChangeCategory6Dialog sub_821C0A50; Options::Draw "
              "sub_821C80F0, Update sub_821C81E8, SavePage sub_821C6F58, Category6Page "
              "sub_821BD228, StepHud sub_821BE3E0, ShowVolumeBars sub_821BE8A8, ShowPrompts "
              "sub_821C1068, Settings::Reload sub_821C0FF0 and Options::OnLeft sub_821B9E70 "
              "(with --native_heap), CategoryPage sub_821BC670, SetUpMovie sub_821BCFF0, Show "
              "sub_821C1158, MoveCategoryCursor sub_821C2BE0, MoveHudCursor sub_821C2E28, "
              "MoveSoundCursor sub_821C3008, ChangeSoundValue sub_821C3278, MoveSlidersCursor "
              "sub_821C3398, ChangeSlidersValue sub_821C35F0, LeaveButtons sub_821C2778 and "
              "LeaveAnalog sub_821C2A10; the category pages HudPage sub_821C1270, SlidersPage "
              "sub_821C1888, ButtonsPage sub_821C1D08 and AnalogPage sub_821C21A8 with "
              "HasButtonClash sub_821BA890 and HasAnalogClash sub_821BAC78, SoundPage sub_821C1498 "
              "and LeaveSoundPage sub_821BA380; the set-ups SetUpCategoryPage sub_821C4BE0, "
              "SetUpHudPage sub_821C4DB0, SetUpSoundPage sub_821C4FD0, SetUpSlidersPage "
              "sub_821C51D0, SetUpButtonsPage sub_821C5580 and SetUpAnalogPage sub_821C57D0; the "
              "steps FadeIn sub_821C7080, CloseHud sub_821C75E0, CloseSound sub_821C7710, "
              "CloseSliders sub_821C7868, CloseButtons sub_821C7B08 and CloseAnalog sub_821C7D18; the "
              "type 2 and 4 modes' Inits Type2Options::Init sub_821C9E00, Type4Options::Init "
              "sub_821CA458, Type2ControllerOptions::Init sub_821C8578 and "
              "Type4ControllerOptions::Init sub_821C8A60, and the type 8 mode's Type8Options::Init "
              "sub_821CAAB0 and Type8ControllerOptions::Init sub_821C8F48 (no adapters: "
              "ModeType8::Init's plain C++ calls them) (step tables, "
              "tools/init_step_tables.py); the pages' and steps' handlers, Menu::Pressed, MenuSound::Play and the sound "
              "options, the UI movie calls, UiMovies::Open and Delete and memcpy where its plain C++ declines through their overrides) "
              "natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=options_screen)");

ITC_FLAG_BOOL(native_options_load_save, true, "ITC",
              "Run the options screen's load page (category 5, page 9) and save page "
              "(category 6, page 10) (docs/functions/options-load-save.md: LoadPage::SetDialog "
              "sub_821BB858 and SavePage::SetDialog sub_821BB708 (their callers), LoadPage::Close sub_821B9FB0, "
              "SavePage::Close sub_821BA508, OptionsSavePages::Cancel sub_821B9FF8, the sub-pages "
              "LoadPage::CheckDevice sub_821BBC10, Answer sub_821BC7B0, Note sub_821BC940, Wait "
              "sub_821C2620 and DeviceAnswer sub_821BBC68, SavePage::CheckDevice sub_821BBCC0, "
              "Answer sub_821BCA60, Note sub_821BCC10 and Wait sub_821BBD18, and the steps "
              "LoadPage::Leave sub_821C6468 and SavePage::Leave sub_821C6550; the save layer, "
              "Menu::Step and Pressed, MenuSound::Play, the UI movie calls and memcpy where its "
              "plain C++ declines through their overrides) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=options_load_save)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (options_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), base_(), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  options_screen::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = args[0];
    ctx.r4.u64 = args[1];
    ctx.r5.u64 = args[2];
    ctx.r6.u64 = args[3];
    ctx.r7.u64 = args[4];
    ctx.r8.u64 = args[5];
    ctx.r9.u64 = args[6];
    ctx.r10.u64 = args[7];
    if (f1 != nullptr) {
      ctx.f1.f64 = *f1;
    }
    ctx.lr = lr;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

void OptionsButtonIndex(lifted::Regs& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::ButtonIndex(ctx.r3.u32))));
}

void OptionsButtonCode(lifted::Regs& ctx, uint8_t*) {
  ctx.r3.u64 = options_screen::ButtonCode(ctx.r3.u32);
}

void OptionsAnalogIndex(lifted::Regs& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::AnalogIndex(ctx.r3.u32))));
}

void OptionsAnalogPicture(lifted::Regs& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::AnalogPicture(ctx.r3.u32))));
}

ITC_REGS_STEP(OptionsIsButtonTaken) {
  ctx.r3.u64 = options_screen::IsButtonTaken(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsIsAnalogTaken) {
  ctx.r3.u64 = options_screen::IsAnalogTaken(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsStepButtons) {
  ctx.r3.u64 = options_screen::StepButtons();
}

ITC_REGS_STEP(OptionsStepAnalog) {
  ctx.r3.u64 = options_screen::StepAnalog();
}

ITC_REGS_STEP(OptionsSettingsReset) {
  ctx.r3.u64 = options_screen::SettingsReset(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsSettingsRead) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SettingsRead(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSettingsWrite) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SettingsWrite(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsOnEnter) {
  ctx.r3.u64 = options_screen::OnEnter(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsWaitForMovie) {
  ctx.r3.u64 = options_screen::WaitForMovie(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsOpenMovie) {
  const uint32_t lr = uint32_t(ctx.lr);
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::OpenMovie(ctx.r1.u32, lr, guest.calls());
}

ITC_REGS_STEP(OptionsOnClosed) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::OnClosed(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsClose) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::Close(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsLeave) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::Leave(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsClosePage) {
  ctx.r3.u64 = options_screen::ClosePage(ctx.r3.u64);
}

ITC_REGS_STEP(OptionsWait) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::Wait(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsOpenCategories) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::OpenCategories(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsReturnToCategories) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ReturnToCategories(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsEnterHud) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::EnterHud(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsChangeHudValue) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ChangeHudValue(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsEnterSound) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::EnterSound(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsEnterSliders) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::EnterSliders(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsEnterButtons) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::EnterButtons(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetCategoryArrow) {
  ctx.r3.u64 = options_screen::SetCategoryArrow(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowVolumeNumber) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      options_screen::ShowVolumeNumber(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowMusicName) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowMusicName(ctx.r3.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowSlider) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowSlider(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowSliderMark) {
  ctx.r3.u64 = options_screen::ShowSliderMark(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowButtonPicture) {
  ctx.r3.u64 = options_screen::ShowButtonPicture(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowAnalogPicture) {
  ctx.r3.u64 = options_screen::ShowAnalogPicture(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsResetNode) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      options_screen::ResetNode(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowVolumeBar) {
  ctx.r3.u64 = options_screen::ShowVolumeBar(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowVolumeNumbers) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowVolumeNumbers(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowSliderNumbers) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowSliderNumbers(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowSliderMarks) {
  ctx.r3.u64 = options_screen::ShowSliderMarks(ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowHudValues) {
  ctx.r3.u64 = options_screen::ShowHudValues(ctx.r1.u32);
}

ITC_REGS_STEP(OptionsStepSound) {
  ctx.r3.u64 = options_screen::StepSound();
}

ITC_REGS_STEP(OptionsStepSliders) {
  ctx.r3.u64 = options_screen::StepSliders(ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowButtonList) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowButtonList(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowAnalogList) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowAnalogList(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowButtonClashes) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowButtonClashes(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShowAnalogClashes) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ShowAnalogClashes(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsEnterAnalog) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::EnterAnalog(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsMoveSaveDialogCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::MoveSaveDialogCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsMoveCategory6DialogCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::MoveCategory6DialogCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsChangeSaveDialog) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ChangeSaveDialog(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsChangeCategory6Dialog) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ChangeCategory6Dialog(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsDraw) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::Draw(ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::Update(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSavePage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SavePage(ctx.r3.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCategory6Page) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      options_screen::Category6Page(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsStepHud) {
  ctx.r3.u64 = options_screen::StepHud(ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowVolumeBars) {
  ctx.r3.u64 = options_screen::ShowVolumeBars(ctx.r1.u32);
}

ITC_REGS_STEP(OptionsShowPrompts) {
  const uint64_t values[6] = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64,
                              ctx.r6.u64, ctx.r7.u64, ctx.r8.u64};
  ctx.r3.u64 = options_screen::ShowPrompts(values, ctx.r1.u32);
}

ITC_REGS_STEP(OptionsSettingsReload) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SettingsReload(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsOnLeft) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::OnLeft(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCategoryPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::CategoryPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpMovie) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpMovie(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsShow) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::Show(ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsMoveCategoryCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::MoveCategoryCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsMoveHudCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::MoveHudCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsMoveSoundCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::MoveSoundCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsChangeSoundValue) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ChangeSoundValue(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsMoveSlidersCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::MoveSlidersCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsChangeSlidersValue) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ChangeSlidersValue(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsLeaveButtons) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::LeaveButtons(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsLeaveAnalog) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::LeaveAnalog(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsHudPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::HudPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSlidersPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SlidersPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsButtonsPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::ButtonsPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsAnalogPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::AnalogPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSoundPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SoundPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsLeaveSoundPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::LeaveSoundPage(ctx.r3.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpCategoryPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpCategoryPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpHudPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpHudPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpSoundPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpSoundPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpSlidersPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpSlidersPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpButtonsPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpButtonsPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsSetUpAnalogPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SetUpAnalogPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsFadeIn) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::FadeIn(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCloseHud) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::CloseHud(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCloseSound) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::CloseSound(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCloseSliders) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::CloseSliders(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCloseButtons) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::CloseButtons(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(OptionsCloseAnalog) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::CloseAnalog(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void LoadPageClose(lifted::Regs&) { options_screen::LoadPageClose(); }

void SavePageClose(lifted::Regs&) { options_screen::SavePageClose(); }

void SavePagesCancel(lifted::Regs&) { options_screen::SavePagesCancel(); }

#define ITC_LOAD_SAVE_PAGE(name)                                                \
  ITC_REGS_STEP(name) {                                   \
    GuestCalls guest(ctx);                                                \
    ctx.r3.u64 = options_screen::name(ctx.r1.u32, guest.calls());         \
  }
ITC_LOAD_SAVE_PAGE(LoadPageCheckDevice)
ITC_LOAD_SAVE_PAGE(LoadPageAnswer)
ITC_LOAD_SAVE_PAGE(LoadPageNote)
ITC_LOAD_SAVE_PAGE(LoadPageWait)
ITC_LOAD_SAVE_PAGE(LoadPageDeviceAnswer)
ITC_LOAD_SAVE_PAGE(SavePageCheckDevice)
ITC_LOAD_SAVE_PAGE(SavePageAnswer)
ITC_LOAD_SAVE_PAGE(SavePageNote)
ITC_LOAD_SAVE_PAGE(SavePageWait)
#undef ITC_LOAD_SAVE_PAGE

ITC_REGS_STEP(LoadPageLeave) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::LoadPageLeave(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(SavePageLeave) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = options_screen::SavePageLeave(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsInit(lifted::Regs& ctx, uint32_t address) {
  const uint64_t object_r3 = ctx.r3.u64;
  const options_screen::InitResult result =
      options_screen::RunInit(address, ctx.r3.u32, ctx.r1.u32);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  const uint64_t machine_r3 = object_r3 + (result.machine - ctx.r3.u32);
  ctx.r3.u64 = result.replaced ? result.r3 : machine_r3 + (result.r3 - result.machine);
}

}  // namespace hooks

// No adapters (Native: their only callers in play call their plain C++)
// for Controls::ButtonIndex and AnalogIndex (Settings::Read), ButtonCode
// (Settings::Write), Settings::Reset (Options::OnEnter), Settings::Write
// (Options::OnClosed), Options::IsButtonTaken (ShowButtonClashes),
// IsAnalogTaken (ShowAnalogClashes), ShowSlider (ShowSliderNumbers),
// Settings::Read (Options::Show, Settings::Reload), Options::SetUpMovie
// (Show), SetCategoryArrow (MoveCategoryCursor) and ShowVolumeNumber
// (ShowVolumeNumbers, ChangeSoundValue); and (the third migration) for
// Controls::AnalogPicture, Options::ResetNode, ShowMusicName,
// ShowButtonPicture, ShowAnalogPicture, StepHud, ShowHudValues, StepSound,
// ShowVolumeNumbers, ShowVolumeBars, StepSliders, ShowSliderNumbers,
// ShowSliderMarks, StepButtons, ShowButtonList, ShowButtonClashes,
// StepAnalog, ShowAnalogList and ShowAnalogClashes (their callers in play
// the pages, their set-ups and steps, all calling the plain C++); their
// register steps stay for the harness.

#define ITC_OPTIONS_SCREEN(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_options_screen)) { \
    hooks::step(ctx);                 \
  }

ITC_OPTIONS_SCREEN(821BBBA0, OptionsWaitForMovie)
ITC_OPTIONS_SCREEN(821B9F88, OptionsOpenMovie)
ITC_OPTIONS_SCREEN(821BCE38, OptionsOnClosed)
ITC_OPTIONS_SCREEN(821BCD28, OptionsClose)
ITC_OPTIONS_SCREEN(821BCF08, OptionsLeave)
ITC_OPTIONS_SCREEN(821BD330, OptionsClosePage)
ITC_OPTIONS_SCREEN(821BD3C8, OptionsWait)
ITC_OPTIONS_SCREEN(821BD468, OptionsOpenCategories)
ITC_OPTIONS_SCREEN(821BD5B0, OptionsReturnToCategories)
ITC_OPTIONS_SCREEN(821BD728, OptionsEnterHud)
ITC_OPTIONS_SCREEN(821BD800, OptionsChangeHudValue)
ITC_OPTIONS_SCREEN(821BD908, OptionsEnterSound)
ITC_OPTIONS_SCREEN(821BD9A0, OptionsEnterSliders)
ITC_OPTIONS_SCREEN(821BDAD0, OptionsEnterButtons)
ITC_OPTIONS_SCREEN(821BEA48, OptionsShowSliderMark)
ITC_OPTIONS_SCREEN(821BDE88, OptionsEnterAnalog)
ITC_OPTIONS_SCREEN(821C09B0, OptionsMoveSaveDialogCursor)
ITC_OPTIONS_SCREEN(821C0B50, OptionsMoveCategory6DialogCursor)
ITC_OPTIONS_SCREEN(821C08B0, OptionsChangeSaveDialog)
ITC_OPTIONS_SCREEN(821C0A50, OptionsChangeCategory6Dialog)
ITC_OPTIONS_SCREEN(821C80F0, OptionsDraw)
ITC_OPTIONS_SCREEN(821C81E8, OptionsUpdate)
ITC_OPTIONS_SCREEN(821C6F58, OptionsSavePage)
ITC_OPTIONS_SCREEN(821BD228, OptionsCategory6Page)
ITC_OPTIONS_SCREEN(821C1068, OptionsShowPrompts)
ITC_OPTIONS_SCREEN(821C0FF0, OptionsSettingsReload)
ITC_OPTIONS_SCREEN(821BC670, OptionsCategoryPage)
ITC_OPTIONS_SCREEN(821C1158, OptionsShow)
ITC_OPTIONS_SCREEN(821C2BE0, OptionsMoveCategoryCursor)
ITC_OPTIONS_SCREEN(821C2E28, OptionsMoveHudCursor)
ITC_OPTIONS_SCREEN(821C3008, OptionsMoveSoundCursor)
ITC_OPTIONS_SCREEN(821C3278, OptionsChangeSoundValue)
ITC_OPTIONS_SCREEN(821C3398, OptionsMoveSlidersCursor)
ITC_OPTIONS_SCREEN(821C35F0, OptionsChangeSlidersValue)
ITC_OPTIONS_SCREEN(821C2778, OptionsLeaveButtons)
ITC_OPTIONS_SCREEN(821C2A10, OptionsLeaveAnalog)
ITC_OPTIONS_SCREEN(821C1270, OptionsHudPage)
ITC_OPTIONS_SCREEN(821C1888, OptionsSlidersPage)
ITC_OPTIONS_SCREEN(821C1D08, OptionsButtonsPage)
ITC_OPTIONS_SCREEN(821C21A8, OptionsAnalogPage)
ITC_OPTIONS_SCREEN(821C1498, OptionsSoundPage)
ITC_OPTIONS_SCREEN(821BA380, OptionsLeaveSoundPage)
ITC_OPTIONS_SCREEN(821C4BE0, OptionsSetUpCategoryPage)
ITC_OPTIONS_SCREEN(821C4DB0, OptionsSetUpHudPage)
ITC_OPTIONS_SCREEN(821C4FD0, OptionsSetUpSoundPage)
ITC_OPTIONS_SCREEN(821C51D0, OptionsSetUpSlidersPage)
ITC_OPTIONS_SCREEN(821C5580, OptionsSetUpButtonsPage)
ITC_OPTIONS_SCREEN(821C57D0, OptionsSetUpAnalogPage)
ITC_OPTIONS_SCREEN(821C7080, OptionsFadeIn)
ITC_OPTIONS_SCREEN(821C75E0, OptionsCloseHud)
ITC_OPTIONS_SCREEN(821C7710, OptionsCloseSound)
ITC_OPTIONS_SCREEN(821C7868, OptionsCloseSliders)
ITC_OPTIONS_SCREEN(821C7B08, OptionsCloseButtons)
ITC_OPTIONS_SCREEN(821C7D18, OptionsCloseAnalog)
// The type 2 and 4 modes' Inits.
#define ITC_OPTIONS_INIT(address)            \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_options_screen)) { \
    hooks::OptionsInit(ctx, 0x##address); \
  }
ITC_OPTIONS_INIT(821C9E00)
ITC_OPTIONS_INIT(821CA458)
ITC_OPTIONS_INIT(821C8578)
ITC_OPTIONS_INIT(821C8A60)
#undef ITC_OPTIONS_INIT

// Options::ShowVolumeBar sub_821BE5A8: its override is the pause movie log's
// wrap (src/debug/pause_movie_log.cpp), which calls the step under the flag.

// An adapter whose step allocates: the original where the native heap isn't
// active.
ITC_NATIVE_ENTRY_ALWAYS(821BB9A8) {
  
  if (!ITC_FLAG(native_options_screen) || !hooks::NativeHeapActive()) {
    lifted::original::c821BB9A8(ctx);
    return;
  }
  hooks::OptionsOnEnter(ctx);
}

// Options::OnLeft frees the blocks OnEnter took from the allocation layer:
// the original where the native heap isn't active.
ITC_NATIVE_ENTRY_ALWAYS(821B9E70) {
  
  if (!ITC_FLAG(native_options_screen) || !hooks::NativeHeapActive()) {
    lifted::original::c821B9E70(ctx);
    return;
  }
  hooks::OptionsOnLeft(ctx);
}

// The load and save pages (docs/functions/options-load-save.md). No adapters
// (Native: their only callers in play, the pages' sub-pages, call their plain
// C++) for LoadPage::SetDialog and SavePage::SetDialog; the flag switches
// only their callers.
#define ITC_OPTIONS_LOAD_SAVE(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_options_load_save)) { \
    hooks::step(ctx);                    \
  }

ITC_OPTIONS_LOAD_SAVE(821B9FB0, LoadPageClose)
ITC_OPTIONS_LOAD_SAVE(821BA508, SavePageClose)
ITC_OPTIONS_LOAD_SAVE(821B9FF8, SavePagesCancel)
ITC_OPTIONS_LOAD_SAVE(821BBC10, LoadPageCheckDevice)
ITC_OPTIONS_LOAD_SAVE(821BC7B0, LoadPageAnswer)
ITC_OPTIONS_LOAD_SAVE(821BC940, LoadPageNote)
ITC_OPTIONS_LOAD_SAVE(821C2620, LoadPageWait)
ITC_OPTIONS_LOAD_SAVE(821BBC68, LoadPageDeviceAnswer)
ITC_OPTIONS_LOAD_SAVE(821BBCC0, SavePageCheckDevice)
ITC_OPTIONS_LOAD_SAVE(821BCA60, SavePageAnswer)
ITC_OPTIONS_LOAD_SAVE(821BCC10, SavePageNote)
ITC_OPTIONS_LOAD_SAVE(821BBD18, SavePageWait)
ITC_OPTIONS_LOAD_SAVE(821C6468, LoadPageLeave)
ITC_OPTIONS_LOAD_SAVE(821C6550, SavePageLeave)
