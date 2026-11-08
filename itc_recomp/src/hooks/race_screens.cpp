// The other in-race screens natively: adapters in front of src/race_screens/
// (docs/functions/race-screens.md), with the register steps the harness
// shares (hooks/race_screens.h).

#include "hooks/race_screens.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "race_screens/race_screens.h"

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
              "ShowItems, SetArrowA, ShowOptions, ShowVolumes, ShowSlider, RestoreNodes, "
              "ColorItems, ShowAdjust and Open, the options page's ToggleOption, OptionsPage, "
              "SetUpOptions and SetAdjust, the menu after the race's ConfirmPage, Intro, Show, "
              "CloseMain, Move, ShowItems, SetUpItems, ColorItems, Start and Open, "
              "the confirm dialog's Question, OpenAnim, Move, ShowMessage (its messages 0 to 11, "
              "never seen in play, the original), Draw, SetText (its UI movie "
              "setters' unset varargs 0) and Open (SetText's plain C++, its unset varargs 0), "
              "PauseMenu::OpenOptions, "
              "the draws PauseMenu::, AfterRaceMenu:: and CareerResults::Draw, the career's "
              "results' Intro, Show, Close1 and Page1, "
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
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  race_screens::Calls calls() { return {this, &GuestCalls::Call}; }

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

bool NativeSlots() { return ITC_FLAG(native_action_mappers); }

}  // namespace

void RaceScreensPagesInit(PPCContext& ctx, uint8_t* base) {
  race_screens::PagesInit(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensPagesGo(PPCContext& ctx, uint8_t* base) {
  race_screens::PagesGo(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

void RaceScreensPagesFinish(PPCContext& ctx, uint8_t* base) {
  race_screens::PagesFinish(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListSelection(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::ListSelection(base, ctx.r3.u32);
}

void RaceScreensListSetSelection(PPCContext& ctx, uint8_t* base) {
  race_screens::ListSetSelection(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListSetEnabled(PPCContext& ctx, uint8_t* base) {
  race_screens::ListSetEnabled(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void RaceScreensListIsEnabled(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::ListIsEnabled(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListFit(PPCContext& ctx, uint8_t* base) {
  race_screens::ListFit(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceScreensListFired(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::ListFired(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, NativeSlots(),
                                       guest.calls());
}

void RaceScreensListUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::ListUpdate(base, ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls());
}

#define ITC_RACE_SCREENS_PRESSED(name)                                                         \
  void RaceScreensList##name(PPCContext& ctx, uint8_t* base) {                                \
    GuestCalls guest(ctx, base);                                                               \
    ctx.r3.u64 =                                                                               \
        race_screens::List##name(base, ctx.r3.u32, ctx.r1.u32, NativeSlots(), guest.calls()); \
  }
ITC_RACE_SCREENS_PRESSED(Accepted)
ITC_RACE_SCREENS_PRESSED(Cancelled)
ITC_RACE_SCREENS_PRESSED(Pressed152)
ITC_RACE_SCREENS_PRESSED(Pressed166)
#undef ITC_RACE_SCREENS_PRESSED

#define ITC_RACE_SCREENS_CREATE(name, address)                       \
  void RaceScreens##name##Create(PPCContext& ctx, uint8_t* base) {   \
    if (!NativeHeapActive()) {                                       \
      __imp__sub_##address(ctx, base);                               \
      return;                                                        \
    }                                                                \
    ctx.r3.u64 = race_screens::name##Create(base, ctx.r1.u32);       \
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
  void RaceScreens##function(PPCContext& ctx, uint8_t* base) {             \
    if (NativeHeapActive()) {                                              \
      if (const auto r3 = race_screens::function(base, ctx.r1.u32)) {      \
        ctx.r3.u64 = *r3;                                                  \
        return;                                                            \
      }                                                                    \
    }                                                                      \
    __imp__sub_##address(ctx, base);                                       \
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

void RaceScreensPauseMenuLoadData(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuLoadData(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuSaveOptions(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuSaveOptions(base, ctx.r1.u32);
}

void RaceScreensPauseMenuClose(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuClose(base, ctx.r1.u32);
}

void RaceScreensPauseMenuSaveHudOptions(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuSaveHudOptions(base, ctx.r1.u32);
}

void RaceScreensPauseMenuAnswerPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuAnswerPage(base);
}

void RaceScreensAfterRaceMenuAnswerPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::AfterRaceMenuAnswerPage(base);
}

void RaceScreensRaceDialogAnswer(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::RaceDialogAnswer(base);
}

void RaceScreensPauseMenuUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::PauseMenuUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensCareerResultsUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::CareerResultsUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64,
                                                 ctx.r1.u32, guest.calls());
}

void RaceScreensAfterRaceMenuUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::AfterRaceMenuUpdate(base, ctx.r1.u32, guest.calls());
}

void RaceScreensRaceDialogUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceDialogUpdate(base, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensRaceDialogSetText(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  const uint64_t varargs[7] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                               ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  ctx.r3.u64 = race_screens::RaceDialogSetText(base, ctx.r3.u64, varargs, ctx.r1.u32,
                                               guest.calls());
}

void RaceScreensRaceDialogOpen(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceDialogOpen(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32,
                                            guest.calls());
}

void RaceScreensCareerResultsStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::CareerResultsStart(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuStepVolume(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuStepVolume(base, ctx.r1.u32);
}

void RaceScreensPauseMenuStepAdjust(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuStepAdjust(base, ctx.r1.u32);
}

#define ITC_RACE_SCREENS_TRANSITION(name)                                                  \
  void RaceScreensPauseMenu##name(PPCContext& ctx, uint8_t* base) {                          \
    GuestCalls guest(ctx, base);                                                             \
    ctx.r3.u64 = race_screens::PauseMenu##name(base, ctx.r3.u32, ctx.r1.u32, guest.calls()); \
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
#undef ITC_RACE_SCREENS_TRANSITION

void RaceScreensPauseMenuConfirmPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuConfirmPage(base, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuShowItems(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuShowItems(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuSetArrowA(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = race_screens::PauseMenuSetArrowA(base, ctx.r3.u64, ctx.r1.u32);
}

void RaceScreensPauseMenuShowOptions(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuShowOptions(base, ctx.r1.u32);
}

void RaceScreensPauseMenuShowVolumes(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuShowVolumes(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuShowSlider(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::PauseMenuShowSlider(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuRestoreNodes(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::PauseMenuRestoreNodes(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuColorItems(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuColorItems(base, ctx.r1.u32);
}

void RaceScreensPauseMenuShowAdjust(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuShowAdjust(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuOpen(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuOpen(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32,
                                           guest.calls());
}

void RaceScreensPauseMenuToggleOption(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::PauseMenuToggleOption(base, ctx.r1.u32);
}

void RaceScreensPauseMenuOptionsPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuOptionsPage(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuSetUpOptions(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuSetUpOptions(base, ctx.r1.u32, guest.calls());
}

#define ITC_AFTER_RACE_TRANSITION(name)                                                          \
  void RaceScreensAfterRaceMenu##name(PPCContext& ctx, uint8_t* base) {                          \
    GuestCalls guest(ctx, base);                                                                 \
    ctx.r3.u64 = race_screens::AfterRaceMenu##name(base, ctx.r3.u32, ctx.r1.u32, guest.calls()); \
  }
ITC_AFTER_RACE_TRANSITION(Intro)
ITC_AFTER_RACE_TRANSITION(Show)
ITC_AFTER_RACE_TRANSITION(CloseMain)
ITC_AFTER_RACE_TRANSITION(Move)
#undef ITC_AFTER_RACE_TRANSITION

void RaceScreensAfterRaceMenuConfirmPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::AfterRaceMenuConfirmPage(base, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensAfterRaceMenuShowItems(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::AfterRaceMenuShowItems(base, ctx.r1.u32, guest.calls());
}

void RaceScreensAfterRaceMenuSetUpItems(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::AfterRaceMenuSetUpItems(base, ctx.r1.u32, guest.calls());
}

void RaceScreensAfterRaceMenuColorItems(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::AfterRaceMenuColorItems(base, ctx.r3.u32, ctx.r1.u32);
}

void RaceScreensAfterRaceMenuStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::AfterRaceMenuStart(base, ctx.r1.u32, guest.calls());
}

void RaceScreensAfterRaceMenuOpen(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::AfterRaceMenuOpen(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64,
                                               ctx.r1.u32, guest.calls());
}

void RaceScreensRaceDialogQuestion(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceDialogQuestion(base, ctx.r1.u32, guest.calls());
}

void RaceScreensRaceDialogOpenAnim(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceDialogOpenAnim(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensRaceDialogMove(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceDialogMove(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensRaceDialogShowMessage(PPCContext& ctx, uint8_t* base) {
  std::optional<uint64_t> r3;
  {
    GuestCalls guest(ctx, base);
    r3 = race_screens::RaceDialogShowMessage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
  }
  if (!r3) {
    __imp__sub_82218C10(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void RaceScreensRaceDialogDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceDialogDraw(base, ctx.r1.u32, guest.calls());
}

void RaceScreensPauseMenuOpenOptions(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::PauseMenuOpenOptions(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

#define ITC_RACE_SCREENS_DRAW(name)                                                         \
  void RaceScreens##name##Draw(PPCContext& ctx, uint8_t* base) {                            \
    GuestCalls guest(ctx, base);                                                            \
    ctx.r3.u64 = race_screens::name##Draw(base, ctx.r3.u32, ctx.r1.u32, guest.calls());     \
  }
ITC_RACE_SCREENS_DRAW(PauseMenu)
ITC_RACE_SCREENS_DRAW(AfterRaceMenu)
ITC_RACE_SCREENS_DRAW(CareerResults)
#undef ITC_RACE_SCREENS_DRAW

#define ITC_CAREER_RESULTS_TRANSITION(name)                                                      \
  void RaceScreensCareerResults##name(PPCContext& ctx, uint8_t* base) {                          \
    GuestCalls guest(ctx, base);                                                                 \
    ctx.r3.u64 = race_screens::CareerResults##name(base, ctx.r3.u32, ctx.r1.u32, guest.calls()); \
  }
ITC_CAREER_RESULTS_TRANSITION(Intro)
ITC_CAREER_RESULTS_TRANSITION(Show)
ITC_CAREER_RESULTS_TRANSITION(Close1)
#undef ITC_CAREER_RESULTS_TRANSITION

void RaceScreensCareerResultsPage1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::CareerResultsPage1(base, ctx.r1.u32, NativeSlots(), guest.calls());
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
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_race_screens)) {     \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
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
ITC_RACE_SCREENS(8220CED0, RaceScreensPauseMenuStepVolume)
ITC_RACE_SCREENS(8220D248, RaceScreensPauseMenuStepAdjust)
ITC_RACE_SCREENS(8220C5E0, RaceScreensPauseMenuRunTen)
ITC_RACE_SCREENS(8220C668, RaceScreensPauseMenuBackToMain)
ITC_RACE_SCREENS(8220C700, RaceScreensPauseMenuCloseMain)
ITC_RACE_SCREENS(8220C7B8, RaceScreensPauseMenuMoveMain)
ITC_RACE_SCREENS(8220C868, RaceScreensPauseMenuShowOptionsRow)
ITC_RACE_SCREENS(8220C930, RaceScreensPauseMenuToggleOptions)
ITC_RACE_SCREENS(8220CA28, RaceScreensPauseMenuShowVolumeRow)
ITC_RACE_SCREENS(8220CAB0, RaceScreensPauseMenuShowAdjustRow)
ITC_RACE_SCREENS(8220C578, RaceScreensPauseMenuConfirmPage)
ITC_RACE_SCREENS(8220CB38, RaceScreensPauseMenuShowItems)
ITC_RACE_SCREENS(8220D0B8, RaceScreensPauseMenuShowVolumes)
ITC_RACE_SCREENS(8220D5D0, RaceScreensPauseMenuRestoreNodes)
ITC_RACE_SCREENS(8220DD98, RaceScreensPauseMenuMoveOptions)
ITC_RACE_SCREENS(8220E1F8, RaceScreensPauseMenuMoveAdjust)
ITC_RACE_SCREENS(8220EE28, RaceScreensPauseMenuShowMain)
ITC_RACE_SCREENS(8220E888, RaceScreensPauseMenuShowAdjust)
ITC_RACE_SCREENS(8220DB98, RaceScreensPauseMenuOptionsPage)
ITC_RACE_SCREENS(8220F1B8, RaceScreensPauseMenuSetAdjust)
ITC_RACE_SCREENS(8221A6C8, RaceScreensAfterRaceMenuConfirmPage)
ITC_RACE_SCREENS(8221A730, RaceScreensAfterRaceMenuIntro)
ITC_RACE_SCREENS(8221A7A8, RaceScreensAfterRaceMenuShow)
ITC_RACE_SCREENS(8221A950, RaceScreensAfterRaceMenuCloseMain)
ITC_RACE_SCREENS(8221AA20, RaceScreensAfterRaceMenuMove)
ITC_RACE_SCREENS(8221B860, RaceScreensAfterRaceMenuStart)
ITC_RACE_SCREENS(8221BA68, RaceScreensAfterRaceMenuOpen)
ITC_RACE_SCREENS(82217F40, RaceScreensRaceDialogQuestion)
ITC_RACE_SCREENS(822186A8, RaceScreensRaceDialogOpenAnim)
ITC_RACE_SCREENS(82218788, RaceScreensRaceDialogMove)
ITC_RACE_SCREENS(82218C10, RaceScreensRaceDialogShowMessage)
// RaceDialog::SetText has no adapter (removed, 2026-11-08): its callers in
// play, Open's and ShowMessage's plain C++, call its plain C++ (AskSlot and
// ShowMessage's messages, never seen in play, the original).
// RaceDialog::Open (from PauseMenu::MainPage and AfterRaceMenu::MainPage,
// recompiled).
ITC_RACE_SCREENS(82218B30, RaceScreensRaceDialogOpen)
ITC_RACE_SCREENS(8220EEF0, RaceScreensPauseMenuOpenOptions)
ITC_RACE_SCREENS(82219210, RaceScreensCareerResultsIntro)
ITC_RACE_SCREENS(822192B0, RaceScreensCareerResultsShow)
ITC_RACE_SCREENS(822193D8, RaceScreensCareerResultsClose1)
ITC_RACE_SCREENS(82219A78, RaceScreensCareerResultsPage1)
