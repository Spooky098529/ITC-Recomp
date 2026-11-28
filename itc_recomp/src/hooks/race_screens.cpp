// The other in-race screens natively: adapters in front of src/race_screens/
// (docs/functions/race-screens.md), with the register steps the harness
// shares (hooks/race_screens.h).

#include "hooks/race_screens.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "race_screens/race_screens.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_race_screens, true, "ITC",
              "Run the other in-race screens' functions (docs/functions/race-screens.md: the "
              "page object's ScreenPages::Init sub_8220BA98, Go sub_8220BA68 and Finish "
              "sub_8220BA28; the menu list's Selection sub_8220BC20, SetSelection sub_8220BC30, "
              "SetEnabled sub_8220BC00, IsEnabled sub_8220BC10, Fit sub_8220BC40, Fired "
              "sub_8220C020, Update sub_8220C1E0, Accepted sub_8220BD50, Cancelled sub_8220BDE0, "
              "Pressed152 sub_8220BE70 and Pressed166 sub_8220BF90, the mapper's read slots by "
              "their plain C++ with --native_action_mappers, else through the vtable; Go and "
              "Update keep their --log_pause_movie and --log_pause_menu lines; the screens' makers "
              "and teardowns, PauseMenu::, PauseMenu4::, AfterRaceMenu::, RaceDialog::, "
              "CareerResults::, RaceResults::, SplitResults:: and Kind6Menu::Create and "
              "Destroy (no adapters: only RaceScreens::Destroy's plain C++ reaches the "
              "Destroys), with --native_heap, and AfterRaceMenu::Close; PauseMenu::LoadData, "
              "SaveOptions, Close, SaveHudOptions and AnswerPage, AfterRaceMenu::AnswerPage and "
              "RaceDialog::Answer; the pages' dispatch, PauseMenu::, CareerResults::, "
              "AfterRaceMenu:: and RaceDialog::Update, the pages' handlers through the function "
              "table; CareerResults::Start; PauseMenu::StepVolume and StepAdjust; the pause menu's "
              "transitions RunTen, BackToMain, CloseMain, MoveMain, ShowOptionsRow, ToggleOptions, "
              "ShowVolumeRow, ShowAdjustRow, MoveOptions, MoveAdjust and ShowMain, ConfirmPage, "
              "ShowItems, SetArrowA, ShowOptions, ShowVolumes, ShowVolume (in its log wrap), ShowSlider, RestoreNodes, "
              "ColorItems, ShowAdjust, Open, MainPage, AdjustPage, ShowCareer, SetUpMain, "
              "ShowVolumesB, ShowAdjustB, SetUpVolume and SetUpAdjust, the transitions OpenMain, "
              "CloseOptions, CloseVolume, CloseAdjust, OpenVolume and OpenAdjust, VolumePage (with "
              "SaveVolumes, never seen in play, by its plain C++), the options page's ToggleOption, OptionsPage, "
              "SetUpOptions and SetAdjust, the menu after the race's ConfirmPage, Intro, Show, "
              "CloseMain, Move, ShowItems, SetUpItems, ColorItems, Start, Open and MainPage (the "
              "replay's save flow, never seen in play, the original), "
              "the confirm dialog's StartSave, ShowSaving and SetNote (never seen in play; "
              "SetNote's unset varargs 0), Question, OpenAnim, Move, ShowMessage (its messages 0 to 11, "
              "never seen in play, the original), Draw, SetText (its UI movie "
              "setters' unset varargs 0) and Open (SetText's plain C++, its unset varargs 0), "
              "PauseMenu::OpenOptions, "
              "the draws PauseMenu::, AfterRaceMenu:: and CareerResults::Draw, the career's "
              "results' Intro, Show, Close1, Page1, SetTexts (its UI movie setters' unset "
              "varargs 0) and Open, "
              "the UI movie calls through their overrides) natively instead "
              "of through the translated code (the same writes; checked by "
              "--run_harnesses=race_screens)");
ITC_FLAG_DECLARE(bool, native_action_mappers);

namespace hooks {

namespace {

// The guest calls the plain C++ makes (race_screens::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  race_screens::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
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
  uint64_t lr_;
};

bool NativeSlots() { return ITC_FLAG(native_action_mappers); }

}  // namespace

ITC_REGS_STEP(RaceScreensPagesInit) {
  race_screens::PagesInit(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceScreensPagesGo) {
  race_screens::PagesGo(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

ITC_REGS_STEP(RaceScreensPagesFinish) {
  race_screens::PagesFinish(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceScreensListSelection) {
  ctx.r3.u64 = race_screens::ListSelection(ctx.r3.u32);
}

ITC_REGS_STEP(RaceScreensListSetSelection) {
  race_screens::ListSetSelection(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceScreensListSetEnabled) {
  race_screens::ListSetEnabled(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(RaceScreensListIsEnabled) {
  ctx.r3.u64 = race_screens::ListIsEnabled(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceScreensListFit) {
  race_screens::ListFit(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceScreensListFired) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::ListFired(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, NativeSlots(),
                                       guest.calls());
}

ITC_REGS_STEP(RaceScreensListUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::ListUpdate(ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls());
}

#define ITC_RACE_SCREENS_PRESSED(name)                                                         \
  void RaceScreensList##name(lifted::Regs& ctx) {                                \
    GuestCalls guest(ctx);                                                               \
    ctx.r3.u64 =                                                                               \
        race_screens::List##name(ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls()); \
  }
ITC_RACE_SCREENS_PRESSED(Accepted)
ITC_RACE_SCREENS_PRESSED(Cancelled)
ITC_RACE_SCREENS_PRESSED(Pressed152)
ITC_RACE_SCREENS_PRESSED(Pressed166)
#undef ITC_RACE_SCREENS_PRESSED

#define ITC_RACE_SCREENS_CREATE(name, address)                       \
  void RaceScreens##name##Create(lifted::Regs& ctx) {   \
    if (!NativeHeapActive()) {                                       \
      lifted::original::c##address(ctx);                               \
      return;                                                        \
    }                                                                \
    ctx.r3.u64 = race_screens::name##Create(ctx.r1.u32);       \
  }
ITC_RACE_SCREENS_CREATE(PauseMenu, 8220D7E8)
ITC_RACE_SCREENS_CREATE(PauseMenu4, 82210868)
ITC_RACE_SCREENS_CREATE(AfterRaceMenu, 8221B1F0)
ITC_RACE_SCREENS_CREATE(RaceDialog, 82217D70)
ITC_RACE_SCREENS_CREATE(CareerResults, 82219138)
ITC_RACE_SCREENS_CREATE(RaceResults, 8221F390)
ITC_RACE_SCREENS_CREATE(SplitResults, 8221FA30)
ITC_RACE_SCREENS_CREATE(Kind6Menu, 8221BDD0)
#undef ITC_RACE_SCREENS_CREATE

#define ITC_RACE_SCREENS_FREE(function, address)                           \
  void RaceScreens##function(lifted::Regs& ctx) {             \
    if (NativeHeapActive()) {                                              \
      if (const auto r3 = race_screens::function(ctx.r1.u32)) {      \
        ctx.r3.u64 = *r3;                                                  \
        return;                                                            \
      }                                                                    \
    }                                                                      \
    lifted::original::c##address(ctx);                                       \
  }
ITC_RACE_SCREENS_FREE(PauseMenuDestroy, 8220C4C8)
ITC_RACE_SCREENS_FREE(PauseMenu4Destroy, 8220FC98)
ITC_RACE_SCREENS_FREE(AfterRaceMenuDestroy, 8221A3E8)
ITC_RACE_SCREENS_FREE(RaceDialogDestroy, 82217CD0)
ITC_RACE_SCREENS_FREE(CareerResultsDestroy, 82219170)
ITC_RACE_SCREENS_FREE(RaceResultsDestroy, 8221F3C8)
ITC_RACE_SCREENS_FREE(SplitResultsDestroy, 8221FA68)
ITC_RACE_SCREENS_FREE(Kind6MenuDestroy, 8221BE30)
ITC_RACE_SCREENS_FREE(AfterRaceMenuClose, 8221A498)
#undef ITC_RACE_SCREENS_FREE

ITC_REGS_STEP(RaceScreensPauseMenuLoadData) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuLoadData(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuSaveOptions) {
  ctx.r3.u64 = race_screens::PauseMenuSaveOptions(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuClose) {
  ctx.r3.u64 = race_screens::PauseMenuClose(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuSaveHudOptions) {
  ctx.r3.u64 = race_screens::PauseMenuSaveHudOptions(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuAnswerPage) {
  ctx.r3.u64 = race_screens::PauseMenuAnswerPage();
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuAnswerPage) {
  ctx.r3.u64 = race_screens::AfterRaceMenuAnswerPage();
}

ITC_REGS_STEP(RaceScreensRaceDialogAnswer) {
  ctx.r3.u64 = race_screens::RaceDialogAnswer();
}

ITC_REGS_STEP(RaceScreensPauseMenuUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::PauseMenuUpdate(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensCareerResultsUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::CareerResultsUpdate(ctx.r3.u64, ctx.r4.u64, ctx.r5.u64,
                                                 ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::AfterRaceMenuUpdate(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogUpdate(ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogSetText) {
  GuestCalls guest(ctx);
  const uint64_t varargs[7] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                               ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  ctx.r3.u64 = race_screens::RaceDialogSetText(ctx.r3.u64, varargs, ctx.r1.u32,
                                               guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogOpen) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogOpen(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32,
                                            guest.calls());
}

ITC_REGS_STEP(RaceScreensCareerResultsStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::CareerResultsStart(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuStepVolume) {
  ctx.r3.u64 = race_screens::PauseMenuStepVolume(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuStepAdjust) {
  ctx.r3.u64 = race_screens::PauseMenuStepAdjust(ctx.r1.u32);
}

#define ITC_RACE_SCREENS_TRANSITION(name)                                                  \
  void RaceScreensPauseMenu##name(lifted::Regs& ctx) {                          \
    GuestCalls guest(ctx);                                                             \
    ctx.r3.u64 = race_screens::PauseMenu##name(ctx.r3.u32, ctx.r1.u32, guest.calls()); \
  }
ITC_RACE_SCREENS_TRANSITION(RunTen)
ITC_RACE_SCREENS_TRANSITION(BackToMain)
ITC_RACE_SCREENS_TRANSITION(CloseMain)
ITC_RACE_SCREENS_TRANSITION(MoveMain)
ITC_RACE_SCREENS_TRANSITION(ShowOptionsRow)
ITC_RACE_SCREENS_TRANSITION(ToggleOptions)
ITC_RACE_SCREENS_TRANSITION(ShowVolumeRow)
ITC_RACE_SCREENS_TRANSITION(ShowAdjustRow)
ITC_RACE_SCREENS_TRANSITION(MoveOptions)
ITC_RACE_SCREENS_TRANSITION(MoveAdjust)
ITC_RACE_SCREENS_TRANSITION(ShowMain)
ITC_RACE_SCREENS_TRANSITION(SetAdjust)
ITC_RACE_SCREENS_TRANSITION(OpenMain)
ITC_RACE_SCREENS_TRANSITION(CloseOptions)
ITC_RACE_SCREENS_TRANSITION(CloseVolume)
ITC_RACE_SCREENS_TRANSITION(CloseAdjust)
ITC_RACE_SCREENS_TRANSITION(OpenVolume)
ITC_RACE_SCREENS_TRANSITION(OpenAdjust)
#undef ITC_RACE_SCREENS_TRANSITION

ITC_REGS_STEP(RaceScreensPauseMenuConfirmPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuConfirmPage(ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuShowItems) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuShowItems(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuSetArrowA) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = race_screens::PauseMenuSetArrowA(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuShowOptions) {
  ctx.r3.u64 = race_screens::PauseMenuShowOptions(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuShowVolumes) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuShowVolumes(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuShowVolume) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::PauseMenuShowVolume(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuShowSlider) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::PauseMenuShowSlider(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuRestoreNodes) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::PauseMenuRestoreNodes(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuColorItems) {
  ctx.r3.u64 = race_screens::PauseMenuColorItems(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuShowAdjust) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuShowAdjust(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuOpen) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuOpen(ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32,
                                           guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuToggleOption) {
  ctx.r3.u64 = race_screens::PauseMenuToggleOption(ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensPauseMenuOptionsPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuOptionsPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuMainPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuMainPage(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuMainPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::AfterRaceMenuMainPage(ctx.r1.u32, guest.calls());
}

#define ITC_RACE_SCREENS_STEP(name)                                                \
  void RaceScreens##name(lifted::Regs& ctx) {                       \
    GuestCalls guest(ctx);                                                 \
    ctx.r3.u64 = race_screens::name(ctx.r1.u32, guest.calls());            \
  }
ITC_RACE_SCREENS_STEP(PauseMenuShowCareer)
ITC_RACE_SCREENS_STEP(PauseMenuSetUpMain)
ITC_RACE_SCREENS_STEP(PauseMenuShowVolumesB)
ITC_RACE_SCREENS_STEP(PauseMenuShowAdjustB)
ITC_RACE_SCREENS_STEP(PauseMenuAdjustPage)
ITC_RACE_SCREENS_STEP(PauseMenuSetUpVolume)
ITC_RACE_SCREENS_STEP(PauseMenuSetUpAdjust)
ITC_RACE_SCREENS_STEP(PauseMenuVolumePage)
#undef ITC_RACE_SCREENS_STEP

ITC_REGS_STEP(RaceScreensPauseMenuSetUpOptions) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuSetUpOptions(ctx.r1.u32, guest.calls());
}

#define ITC_AFTER_RACE_TRANSITION(name)                                                          \
  void RaceScreensAfterRaceMenu##name(lifted::Regs& ctx) {                          \
    GuestCalls guest(ctx);                                                                 \
    ctx.r3.u64 = race_screens::AfterRaceMenu##name(ctx.r3.u32, ctx.r1.u32, guest.calls()); \
  }
ITC_AFTER_RACE_TRANSITION(Intro)
ITC_AFTER_RACE_TRANSITION(Show)
ITC_AFTER_RACE_TRANSITION(CloseMain)
ITC_AFTER_RACE_TRANSITION(Move)
#undef ITC_AFTER_RACE_TRANSITION

ITC_REGS_STEP(RaceScreensAfterRaceMenuConfirmPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::AfterRaceMenuConfirmPage(ctx.r4.u64, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuShowItems) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::AfterRaceMenuShowItems(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuSetUpItems) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::AfterRaceMenuSetUpItems(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuColorItems) {
  ctx.r3.u64 = race_screens::AfterRaceMenuColorItems(ctx.r3.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::AfterRaceMenuStart(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensAfterRaceMenuOpen) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::AfterRaceMenuOpen(ctx.r3.u32, ctx.r4.u64, ctx.r5.u64,
                                               ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogShowSaving) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogShowSaving(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogStartSave) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogStartSave(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogSetNote) {
  GuestCalls guest(ctx);
  const uint64_t varargs[7] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                               ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  ctx.r3.u64 = race_screens::RaceDialogSetNote(ctx.r3.u64, varargs, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogQuestion) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogQuestion(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogOpenAnim) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogOpenAnim(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogMove) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogMove(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensRaceDialogShowMessage) {
  std::optional<uint64_t> r3;
  {
    GuestCalls guest(ctx);
    r3 = race_screens::RaceDialogShowMessage(ctx.r3.u32, ctx.r1.u32, guest.calls());
  }
  if (!r3) {
    lifted::original::c82218C10(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(RaceScreensRaceDialogDraw) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::RaceDialogDraw(ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensPauseMenuOpenOptions) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::PauseMenuOpenOptions(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

#define ITC_RACE_SCREENS_DRAW(name)                                                         \
  void RaceScreens##name##Draw(lifted::Regs& ctx) {                            \
    GuestCalls guest(ctx);                                                            \
    ctx.r3.u64 = race_screens::name##Draw(ctx.r3.u32, ctx.r1.u32, guest.calls());     \
  }
ITC_RACE_SCREENS_DRAW(PauseMenu)
ITC_RACE_SCREENS_DRAW(AfterRaceMenu)
ITC_RACE_SCREENS_DRAW(CareerResults)
#undef ITC_RACE_SCREENS_DRAW

#define ITC_CAREER_RESULTS_TRANSITION(name)                                                      \
  void RaceScreensCareerResults##name(lifted::Regs& ctx) {                          \
    GuestCalls guest(ctx);                                                                 \
    ctx.r3.u64 = race_screens::CareerResults##name(ctx.r3.u32, ctx.r1.u32, guest.calls()); \
  }
ITC_CAREER_RESULTS_TRANSITION(Intro)
ITC_CAREER_RESULTS_TRANSITION(Show)
ITC_CAREER_RESULTS_TRANSITION(Close1)
#undef ITC_CAREER_RESULTS_TRANSITION

ITC_REGS_STEP(RaceScreensCareerResultsPage1) {
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::CareerResultsPage1(ctx.r1.u32, NativeSlots(), guest.calls());
}

ITC_REGS_STEP(RaceScreensCareerResultsSetTexts) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 = race_screens::CareerResultsSetTexts(ctx.r3.u32, ctx.r1.u32, guest.calls());
}

ITC_REGS_STEP(RaceScreensCareerResultsOpen) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx);
  ctx.r3.u64 =
      race_screens::CareerResultsOpen(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. ScreenPages::Go's and MenuList::Update's are their logging overrides
// (src/debug/pause_movie_log.cpp, pause_menu_log.cpp), which call the steps.
// None for MenuList::Fired (its only caller in play, MenuList::Update, calls
// its plain C++) and AfterRaceMenu::Close (its only caller in play, the race
// HUD machine's native CloseResults, calls its plain C++) and
// CareerResults::Update (its only caller in play, the machine's native
// ResultsUpdate, calls its plain C++), and PauseMenu::SetArrowA, ShowSlider
// and ColorItems (their only callers in play, MoveOptions, ShowAdjust and
// ShowMain, call their plain C++), and PauseMenu::ToggleOption and
// ShowOptions (their callers in play, OptionsPage, ToggleOption and
// SetUpOptions, call their plain C++), and PauseMenu::Open and LoadData
// (their callers in play, the race HUD machine's OpenPause, OpenRacePause
// and OpenCareerPause and Open, call their plain C++), and AfterRaceMenu::
// ShowItems, SetUpItems and ColorItems (their callers in play, SetUpItems,
// Open and Start, call their plain C++), and ScreenPages::Finish (its
// callers in play, the draws, call its plain C++), PauseMenu::SaveOptions
// and SaveHudOptions (Close and OptionsPage), PauseMenu::Close (the race HUD
// machine's ClosePause), PauseMenu::Draw (RaceScreens::DrawPauseMenu,
// race-hud-object.md), CareerResults::Draw (the race HUD machine's
// ResultsDraw), AfterRaceMenu::Draw (RaceScreens::DrawAfterRaceMenu,
// race-hud-object.md), SetUpOptions (OpenOptions),
// RaceDialog::Update (the two ConfirmPages) and RaceDialog::Draw (the pause
// menu's and the menu after the race's draws), the eight makers
// (RaceScreens::Create, race-hud-object.md), PauseMenu::Update and
// AfterRaceMenu::Update (the race HUD machine's PauseUpdate and
// AfterRaceUpdate); their register steps stay for the harness.

#define ITC_RACE_SCREENS(address, step)       \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_screens)) { \
    hooks::step(ctx);                   \
  }

ITC_RACE_SCREENS(8220BA98, RaceScreensPagesInit)
ITC_RACE_SCREENS(8220BC20, RaceScreensListSelection)
ITC_RACE_SCREENS(8220BC30, RaceScreensListSetSelection)
ITC_RACE_SCREENS(8220BC00, RaceScreensListSetEnabled)
ITC_RACE_SCREENS(8220BC10, RaceScreensListIsEnabled)
ITC_RACE_SCREENS(8220BC40, RaceScreensListFit)
ITC_RACE_SCREENS(8220BD50, RaceScreensListAccepted)
ITC_RACE_SCREENS(8220BDE0, RaceScreensListCancelled)
ITC_RACE_SCREENS(8220BE70, RaceScreensListPressed152)
ITC_RACE_SCREENS(8220BF90, RaceScreensListPressed166)
// The eight screens' Destroys have no adapters (removed, B7 loop,
// 2026-11-08): only RaceScreens::Destroy's plain C++ (src/race_hud_object/)
// calls them; their steps above are the harness's.
ITC_RACE_SCREENS(8220C5D0, RaceScreensPauseMenuAnswerPage)
ITC_RACE_SCREENS(8221A720, RaceScreensAfterRaceMenuAnswerPage)
ITC_RACE_SCREENS(82217D40, RaceScreensRaceDialogAnswer)
ITC_RACE_SCREENS(822191D0, RaceScreensCareerResultsStart)
// PauseMenu::StepVolume has no adapter (removed, 2026-11-08): its caller in
// play, VolumePage's plain C++, calls its plain C++.
// PauseMenu::StepAdjust has no adapter (removed, 2026-11-08): its caller in
// play, AdjustPage's plain C++, calls its plain C++.
ITC_RACE_SCREENS(8220C5E0, RaceScreensPauseMenuRunTen)
ITC_RACE_SCREENS(8220C668, RaceScreensPauseMenuBackToMain)
ITC_RACE_SCREENS(8220C700, RaceScreensPauseMenuCloseMain)
ITC_RACE_SCREENS(8220C7B8, RaceScreensPauseMenuMoveMain)
ITC_RACE_SCREENS(8220C868, RaceScreensPauseMenuShowOptionsRow)
ITC_RACE_SCREENS(8220C930, RaceScreensPauseMenuToggleOptions)
ITC_RACE_SCREENS(8220CA28, RaceScreensPauseMenuShowVolumeRow)
ITC_RACE_SCREENS(8220CAB0, RaceScreensPauseMenuShowAdjustRow)
ITC_RACE_SCREENS(8220C578, RaceScreensPauseMenuConfirmPage)
// PauseMenu::ShowItems has no adapter (removed, 2026-11-08): its caller in
// play, SetUpMain's plain C++, calls its plain C++.
// PauseMenu::ShowVolumes has no adapter (removed, 2026-11-08): its callers in
// play, VolumePage's and SetUpVolume's plain C++, call its plain C++.
// PauseMenu::RestoreNodes has no adapter (removed, 2026-11-08): its callers
// in play, SetUpOptions', SetUpVolume's and SetUpAdjust's plain C++, call
// its plain C++.
ITC_RACE_SCREENS(8220DD98, RaceScreensPauseMenuMoveOptions)
ITC_RACE_SCREENS(8220E1F8, RaceScreensPauseMenuMoveAdjust)
ITC_RACE_SCREENS(8220EE28, RaceScreensPauseMenuShowMain)
// PauseMenu::ShowAdjust has no adapter (removed, 2026-11-08): its callers in
// play, AdjustPage's, SetAdjust's and SetUpAdjust's plain C++, call its
// plain C++.
ITC_RACE_SCREENS(8220DB98, RaceScreensPauseMenuOptionsPage)
// The two main pages (reached through the Updates' page tables).
ITC_RACE_SCREENS(8220D898, RaceScreensPauseMenuMainPage)
ITC_RACE_SCREENS(8221A4A8, RaceScreensAfterRaceMenuMainPage)
ITC_RACE_SCREENS(8220EBD8, RaceScreensPauseMenuAdjustPage)
// PauseMenu::ShowCareer has no adapter (removed, 2026-11-08): its only
// caller, SetUpMain, is native; its step is the harness's.
// PauseMenu::SetUpMain, ShowVolumesB and ShowAdjustB have no adapters
// (removed, 2026-11-08): their callers in play (OpenMain and the three
// Closes; VolumePage and SetUpVolume; AdjustPage and SetUpAdjust) are
// native and call their plain C++.
ITC_RACE_SCREENS(8220F1B8, RaceScreensPauseMenuSetAdjust)
ITC_RACE_SCREENS(8220ED90, RaceScreensPauseMenuOpenMain)
ITC_RACE_SCREENS(8220EFA8, RaceScreensPauseMenuCloseOptions)
ITC_RACE_SCREENS(8220F0A0, RaceScreensPauseMenuCloseVolume)
ITC_RACE_SCREENS(8220F2A8, RaceScreensPauseMenuCloseAdjust)
// PauseMenu::SetUpVolume and SetUpAdjust have no adapters (removed,
// 2026-11-08): their only callers, OpenVolume and OpenAdjust, are native and
// call their plain C++.
ITC_RACE_SCREENS(8220F6D0, RaceScreensPauseMenuOpenVolume)
ITC_RACE_SCREENS(8220F788, RaceScreensPauseMenuOpenAdjust)
ITC_RACE_SCREENS(8220E988, RaceScreensPauseMenuVolumePage)
ITC_RACE_SCREENS(8221A6C8, RaceScreensAfterRaceMenuConfirmPage)
ITC_RACE_SCREENS(8221A730, RaceScreensAfterRaceMenuIntro)
ITC_RACE_SCREENS(8221A7A8, RaceScreensAfterRaceMenuShow)
ITC_RACE_SCREENS(8221A950, RaceScreensAfterRaceMenuCloseMain)
ITC_RACE_SCREENS(8221AA20, RaceScreensAfterRaceMenuMove)
ITC_RACE_SCREENS(8221B860, RaceScreensAfterRaceMenuStart)
// AfterRaceMenu::Open has no adapter (removed, 2026-11-08): its caller in
// play, the race HUD machine's AwardPrize, calls its plain C++ (the unseen
// ShowSplitResults and ShowResults the original).
// RaceDialog::ShowSaving: the recompiled AfterRaceMenu::ListPage (never
// seen in play) reaches it.
ITC_RACE_SCREENS(82218AB0, RaceScreensRaceDialogShowSaving)
ITC_RACE_SCREENS(82217F40, RaceScreensRaceDialogQuestion)
ITC_RACE_SCREENS(822186A8, RaceScreensRaceDialogOpenAnim)
ITC_RACE_SCREENS(82218788, RaceScreensRaceDialogMove)
ITC_RACE_SCREENS(82218C10, RaceScreensRaceDialogShowMessage)
// RaceDialog::SetText has no adapter (removed, 2026-11-08): its callers in
// play, Open's and ShowMessage's plain C++, call its plain C++ (AskSlot and
// ShowMessage's messages, never seen in play, the original).
// RaceDialog::Open has no adapter (removed, 2026-11-08): its callers in
// play, PauseMenu::MainPage's and AfterRaceMenu::MainPage's plain C++, call
// its plain C++ (PauseMenu4::MainPage sub_8220FD48's too, never seen in play,
// race_screens_unseen; the unseen sub_8221E788 the original).
ITC_RACE_SCREENS(8220EEF0, RaceScreensPauseMenuOpenOptions)
ITC_RACE_SCREENS(82219210, RaceScreensCareerResultsIntro)
ITC_RACE_SCREENS(822192B0, RaceScreensCareerResultsShow)
ITC_RACE_SCREENS(822193D8, RaceScreensCareerResultsClose1)
ITC_RACE_SCREENS(82219A78, RaceScreensCareerResultsPage1)
// CareerResults::SetTexts and Open have no adapters (removed, 2026-11-08):
// their only callers, Open and the race HUD machine's AwardPrize, are native
// and call their plain C++; their steps are the harness's.

// For the fallback originals (docs/missing-functions.md).
ITC_DIRECT_ENTRY(8220BA28) { hooks::RaceScreensPagesFinish(ctx); }
ITC_DIRECT_ENTRY(8220D6A0) { hooks::RaceScreensPauseMenuSaveOptions(ctx); }
ITC_DIRECT_ENTRY(8220F840) { hooks::RaceScreensPauseMenuLoadData(ctx); }
ITC_DIRECT_ENTRY(8220FB10) { hooks::RaceScreensPauseMenuDraw(ctx); }
ITC_DIRECT_ENTRY(822197A8) { hooks::RaceScreensCareerResultsSetTexts(ctx); }
ITC_DIRECT_ENTRY(8221AC00) { hooks::RaceScreensAfterRaceMenuShowItems(ctx); }
ITC_DIRECT_ENTRY(8221BB48) { hooks::RaceScreensAfterRaceMenuDraw(ctx); }
// The lift.py batches call these by address too (round of 2026-11-28).
ITC_DIRECT_ENTRY(8220FA88) { hooks::RaceScreensPauseMenuOpen(ctx); }
ITC_DIRECT_ENTRY(8221B5C8) { hooks::RaceScreensAfterRaceMenuSetUpItems(ctx); }
ITC_DIRECT_ENTRY(8221BA68) { hooks::RaceScreensAfterRaceMenuOpen(ctx); }
// For the fallback originals (docs/missing-functions.md): the stale
// originals round (2026-11-28), so lifted code calls the ports.
ITC_DIRECT_ENTRY(8220C020) { hooks::RaceScreensListFired(ctx); }
ITC_DIRECT_ENTRY(8220C4C8) { hooks::RaceScreensPauseMenuDestroy(ctx); }
ITC_DIRECT_ENTRY(8220D7E8) { hooks::RaceScreensPauseMenuCreate(ctx); }
ITC_DIRECT_ENTRY(8220D890) { hooks::RaceScreensPauseMenuClose(ctx); }
ITC_DIRECT_ENTRY(8220F638) { hooks::RaceScreensPauseMenuUpdate(ctx); }
ITC_DIRECT_ENTRY(8220FC98) { hooks::RaceScreensPauseMenu4Destroy(ctx); }
ITC_DIRECT_ENTRY(82210868) { hooks::RaceScreensPauseMenu4Create(ctx); }
ITC_DIRECT_ENTRY(82217CD0) { hooks::RaceScreensRaceDialogDestroy(ctx); }
ITC_DIRECT_ENTRY(82217D70) { hooks::RaceScreensRaceDialogCreate(ctx); }
ITC_DIRECT_ENTRY(82218838) { hooks::RaceScreensRaceDialogSetText(ctx); }
ITC_DIRECT_ENTRY(82218918) { hooks::RaceScreensRaceDialogSetNote(ctx); }
ITC_DIRECT_ENTRY(82219138) { hooks::RaceScreensCareerResultsCreate(ctx); }
ITC_DIRECT_ENTRY(82219170) { hooks::RaceScreensCareerResultsDestroy(ctx); }
ITC_DIRECT_ENTRY(8221A090) { hooks::RaceScreensCareerResultsUpdate(ctx); }
ITC_DIRECT_ENTRY(8221A1F0) { hooks::RaceScreensCareerResultsOpen(ctx); }
ITC_DIRECT_ENTRY(8221A2F8) { hooks::RaceScreensCareerResultsDraw(ctx); }
ITC_DIRECT_ENTRY(8221A3E8) { hooks::RaceScreensAfterRaceMenuDestroy(ctx); }
ITC_DIRECT_ENTRY(8221A498) { hooks::RaceScreensAfterRaceMenuClose(ctx); }
ITC_DIRECT_ENTRY(8221B1F0) { hooks::RaceScreensAfterRaceMenuCreate(ctx); }
ITC_DIRECT_ENTRY(8221BD20) { hooks::RaceScreensAfterRaceMenuUpdate(ctx); }
ITC_DIRECT_ENTRY(8221BDD0) { hooks::RaceScreensKind6MenuCreate(ctx); }
ITC_DIRECT_ENTRY(8221BE30) { hooks::RaceScreensKind6MenuDestroy(ctx); }
ITC_DIRECT_ENTRY(8221F390) { hooks::RaceScreensRaceResultsCreate(ctx); }
ITC_DIRECT_ENTRY(8221F3C8) { hooks::RaceScreensRaceResultsDestroy(ctx); }
ITC_DIRECT_ENTRY(8221FA30) { hooks::RaceScreensSplitResultsCreate(ctx); }
ITC_DIRECT_ENTRY(8221FA68) { hooks::RaceScreensSplitResultsDestroy(ctx); }
