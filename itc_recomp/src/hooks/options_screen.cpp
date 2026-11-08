// The options screen natively: adapters in front of src/options_screen/
// (docs/functions/options-screen.md), with the register steps the harness
// shares (hooks/options_screen.h).

#include "hooks/options_screen.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "options_screen/options_screen.h"

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
              "sub_821C51D0, SetUpButtonsPage sub_821C5580 and SetUpAnalogPage sub_821C57D0; the pages' and steps' handlers, Menu::Pressed, MenuSound::Play and the sound "
              "options, the UI movie calls, UiMovies::Open and Delete and memcpy where its plain C++ declines through their overrides) "
              "natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=options_screen)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (options_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  options_screen::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
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
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

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

void OptionsSettingsRead(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SettingsRead(base, ctx.r1.u32, guest.calls());
}

void OptionsSettingsWrite(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SettingsWrite(base, ctx.r1.u32, guest.calls());
}

void OptionsOnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::OnEnter(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::WaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}

void OptionsOpenMovie(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OpenMovie(ctx.r1.u32, lr, guest.calls());
}

void OptionsOnClosed(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OnClosed(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Leave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsClosePage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ClosePage(base, ctx.r3.u64);
}

void OptionsWait(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Wait(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsOpenCategories(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OpenCategories(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsReturnToCategories(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ReturnToCategories(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterHud(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterHud(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsChangeHudValue(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ChangeHudValue(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterSound(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterSound(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterSliders(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterSliders(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterButtons(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterButtons(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsSetCategoryArrow(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::SetCategoryArrow(base, ctx.r3.u64, ctx.r1.u32);
}

void OptionsShowVolumeNumber(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      options_screen::ShowVolumeNumber(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void OptionsShowMusicName(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowMusicName(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

void OptionsShowSlider(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowSlider(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void OptionsShowSliderMark(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowSliderMark(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32);
}

void OptionsShowButtonPicture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowButtonPicture(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsShowAnalogPicture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowAnalogPicture(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsResetNode(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      options_screen::ResetNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsShowVolumeBar(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowVolumeBar(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32);
}

void OptionsShowVolumeNumbers(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowVolumeNumbers(base, ctx.r1.u32, guest.calls());
}

void OptionsShowSliderNumbers(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowSliderNumbers(base, ctx.r1.u32, guest.calls());
}

void OptionsShowSliderMarks(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowSliderMarks(base, ctx.r1.u32);
}

void OptionsShowHudValues(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowHudValues(base, ctx.r1.u32);
}

void OptionsStepSound(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepSound(base);
}

void OptionsStepSliders(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepSliders(base, ctx.r1.u32);
}

void OptionsShowButtonList(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowButtonList(base, ctx.r1.u32, guest.calls());
}

void OptionsShowAnalogList(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowAnalogList(base, ctx.r1.u32, guest.calls());
}

void OptionsShowButtonClashes(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowButtonClashes(base, ctx.r1.u32, guest.calls());
}

void OptionsShowAnalogClashes(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ShowAnalogClashes(base, ctx.r1.u32, guest.calls());
}

void OptionsEnterAnalog(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterAnalog(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsMoveSaveDialogCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::MoveSaveDialogCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsMoveCategory6DialogCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::MoveCategory6DialogCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsChangeSaveDialog(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ChangeSaveDialog(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsChangeCategory6Dialog(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ChangeCategory6Dialog(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Draw(base, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void OptionsUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Update(base, ctx.r1.u32, guest.calls());
}

void OptionsSavePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SavePage(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

void OptionsCategory6Page(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      options_screen::Category6Page(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void OptionsStepHud(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepHud(base, ctx.r1.u32);
}

void OptionsShowVolumeBars(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ShowVolumeBars(base, ctx.r1.u32);
}

void OptionsShowPrompts(PPCContext& ctx, uint8_t* base) {
  const uint64_t values[6] = {ctx.r3.u64, ctx.r4.u64, ctx.r5.u64,
                              ctx.r6.u64, ctx.r7.u64, ctx.r8.u64};
  ctx.r3.u64 = options_screen::ShowPrompts(base, values, ctx.r1.u32);
}

void OptionsSettingsReload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SettingsReload(base, ctx.r1.u32, guest.calls());
}

void OptionsOnLeft(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OnLeft(base, ctx.r1.u32, guest.calls());
}

void OptionsCategoryPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::CategoryPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSetUpMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpMovie(base, ctx.r1.u32, guest.calls());
}

void OptionsShow(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Show(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsMoveCategoryCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::MoveCategoryCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsMoveHudCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::MoveHudCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsMoveSoundCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::MoveSoundCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsChangeSoundValue(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ChangeSoundValue(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsMoveSlidersCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::MoveSlidersCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsChangeSlidersValue(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ChangeSlidersValue(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsLeaveButtons(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::LeaveButtons(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsLeaveAnalog(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::LeaveAnalog(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsHudPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::HudPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSlidersPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SlidersPage(base, ctx.r1.u32, guest.calls());
}

void OptionsButtonsPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ButtonsPage(base, ctx.r1.u32, guest.calls());
}

void OptionsAnalogPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::AnalogPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSoundPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SoundPage(base, ctx.r1.u32, guest.calls());
}

void OptionsLeaveSoundPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::LeaveSoundPage(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

void OptionsSetUpCategoryPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpCategoryPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSetUpHudPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpHudPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSetUpSoundPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpSoundPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSetUpSlidersPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpSlidersPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSetUpButtonsPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpButtonsPage(base, ctx.r1.u32, guest.calls());
}

void OptionsSetUpAnalogPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SetUpAnalogPage(base, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// No adapters (Native: their only callers in play call their plain C++)
// for Controls::ButtonIndex and AnalogIndex (Settings::Read), ButtonCode
// (Settings::Write), Settings::Reset (Options::OnEnter), Settings::Write
// (Options::OnClosed), Options::IsButtonTaken (ShowButtonClashes),
// IsAnalogTaken (ShowAnalogClashes), ShowSlider (ShowSliderNumbers),
// Settings::Read (Options::Show, Settings::Reload), Options::SetUpMovie
// (Show), SetCategoryArrow (MoveCategoryCursor) and ShowVolumeNumber
// (ShowVolumeNumbers, ChangeSoundValue); their register steps stay for the
// harness.

#define ITC_OPTIONS_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_options_screen)) { \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_OPTIONS_SCREEN(821BAD08, OptionsAnalogPicture)
ITC_OPTIONS_SCREEN(821BA550, OptionsStepButtons)
ITC_OPTIONS_SCREEN(821BABD8, OptionsStepAnalog)
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
ITC_OPTIONS_SCREEN(821BE930, OptionsShowMusicName)
ITC_OPTIONS_SCREEN(821BEA48, OptionsShowSliderMark)
ITC_OPTIONS_SCREEN(821BF0F0, OptionsShowButtonPicture)
ITC_OPTIONS_SCREEN(821BF630, OptionsShowAnalogPicture)
ITC_OPTIONS_SCREEN(821BFA68, OptionsResetNode)
ITC_OPTIONS_SCREEN(821BE820, OptionsShowVolumeNumbers)
ITC_OPTIONS_SCREEN(821BECA8, OptionsShowSliderNumbers)
ITC_OPTIONS_SCREEN(821BED38, OptionsShowSliderMarks)
ITC_OPTIONS_SCREEN(821BE290, OptionsShowHudValues)
ITC_OPTIONS_SCREEN(821BE5F8, OptionsStepSound)
ITC_OPTIONS_SCREEN(821BEAC8, OptionsStepSliders)
ITC_OPTIONS_SCREEN(821BEFE8, OptionsShowButtonList)
ITC_OPTIONS_SCREEN(821BF520, OptionsShowAnalogList)
ITC_OPTIONS_SCREEN(821BF158, OptionsShowButtonClashes)
ITC_OPTIONS_SCREEN(821BF698, OptionsShowAnalogClashes)
ITC_OPTIONS_SCREEN(821BDE88, OptionsEnterAnalog)
ITC_OPTIONS_SCREEN(821C09B0, OptionsMoveSaveDialogCursor)
ITC_OPTIONS_SCREEN(821C0B50, OptionsMoveCategory6DialogCursor)
ITC_OPTIONS_SCREEN(821C08B0, OptionsChangeSaveDialog)
ITC_OPTIONS_SCREEN(821C0A50, OptionsChangeCategory6Dialog)
ITC_OPTIONS_SCREEN(821C80F0, OptionsDraw)
ITC_OPTIONS_SCREEN(821C81E8, OptionsUpdate)
ITC_OPTIONS_SCREEN(821C6F58, OptionsSavePage)
ITC_OPTIONS_SCREEN(821BD228, OptionsCategory6Page)
ITC_OPTIONS_SCREEN(821BE3E0, OptionsStepHud)
ITC_OPTIONS_SCREEN(821BE8A8, OptionsShowVolumeBars)
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
// Options::ShowVolumeBar sub_821BE5A8: its override is the pause movie log's
// wrap (src/debug/pause_movie_log.cpp), which calls the step under the flag.

// An adapter whose step allocates: the original where the native heap isn't
// active.
extern "C" REX_FUNC(sub_821BB9A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_options_screen) || !hooks::NativeHeapActive()) {
    __imp__sub_821BB9A8(ctx, base);
    return;
  }
  hooks::OptionsOnEnter(ctx, base);
}

// Options::OnLeft frees the blocks OnEnter took from the allocation layer:
// the original where the native heap isn't active.
extern "C" REX_FUNC(sub_821B9E70) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_options_screen) || !hooks::NativeHeapActive()) {
    __imp__sub_821B9E70(ctx, base);
    return;
  }
  hooks::OptionsOnLeft(ctx, base);
}
