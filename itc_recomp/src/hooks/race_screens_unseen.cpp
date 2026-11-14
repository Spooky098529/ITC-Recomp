// The in-race screens never seen in play natively: adapters in front of
// src/race_screens/unseen.h (docs/functions/race-screens-unseen.md), with
// the register steps the harness shares (hooks/race_screens_unseen.h).
// Ported though never seen in play (the user's call, 2026-11-12).

#include "hooks/race_screens_unseen.h"

#include "base/flags.h"
#include "race_screens/unseen.h"

ITC_FLAG_DECLARE(bool, native_action_mappers);

ITC_FLAG_BOOL(native_race_screens_unseen, true, "ITC",
              "Run the in-race screens never seen in play natively "
              "(docs/functions/race-screens-unseen.md): kind 4's pause menu, "
              "PauseMenu4::Open sub_82211390, Update sub_82211458 and Draw sub_82211590 with "
              "their pages, helpers and transitions; the results at +1028, RaceResults::Open "
              "sub_8221F990, Update sub_8221F930 and Draw sub_8221F7D0 with SetTexts, MainPage, "
              "Intro, Show and Close; the results at +1032, SplitResults::Open sub_82220050 and "
              "Update sub_8221FFF0 with MainPage, Intro, Show and Close (Draw sub_822200E0 "
              "stays recompiled); kind 6's screen after the race, Kind6Menu::Open sub_8221EC48, "
              "Draw sub_8221EF60 and Close sub_8221C168 with SetUpTexts, ShowCourse, ShowHelp, "
              "ShowPanelA, HidePanels, SetCursorX, SetUpList, ShowList, ShowReplay and the "
              "transitions of its two page objects, and Update sub_8221F1E8 with its 19 pages, "
              "ShowPanelB, ShowTimeOnly, ShowRanking and RankingTime and the network "
              "manager's sub_822914A8, sub_8225FFC8 and sub_822927B0 (the Xbox Live work, "
              "sub_8214B330, sub_822917B8 and the leaderboard read sub_82292CC0, the "
              "originals; SaveReplay, RaceDialog::StartSave, ShowSaving and "
              "UiMovie::SetNodeCharText the originals too) (the UI movie calls through their "
              "overrides, SetText's and SetNodeText's unset varargs 0), instead of through the "
              "translated code (the same writes; checked by "
              "--run_harnesses=race_screens_unseen)");

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

}  // namespace

void RaceScreensUnseenPause4Open(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Pause4Open(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32,
                                        guest.calls());
}

void RaceScreensUnseenPause4Update(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::Pause4Update(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenPause4Draw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Pause4Draw(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

// The steps without adapters (the harness's).
#define ITC_UNSEEN_CALLS(name)                                                             \
  void RaceScreensUnseenPause4##name(PPCContext& ctx, uint8_t* base) {                     \
    GuestCalls guest(ctx, base);                                                           \
    ctx.r3.u64 = race_screens::Pause4##name(base, ctx.r1.u32, guest.calls());              \
  }
ITC_UNSEEN_CALLS(MainPage)
ITC_UNSEEN_CALLS(VolumePage)
ITC_UNSEEN_CALLS(SetUpMain)
ITC_UNSEEN_CALLS(ShowItems)
ITC_UNSEEN_CALLS(SetUpVolume)
ITC_UNSEEN_CALLS(ShowVolumes)
ITC_UNSEEN_CALLS(ShowVolumesB)
#undef ITC_UNSEEN_CALLS

#define ITC_UNSEEN_PLAIN(name)                                             \
  void RaceScreensUnseenPause4##name(PPCContext& ctx, uint8_t* base) {     \
    ctx.r3.u64 = race_screens::Pause4##name(base, ctx.r1.u32);             \
  }
ITC_UNSEEN_PLAIN(AnswerPage)
ITC_UNSEEN_PLAIN(StepVolume)
ITC_UNSEEN_PLAIN(LoadVolumes)
ITC_UNSEEN_PLAIN(SaveVolumes)
ITC_UNSEEN_PLAIN(ColorItems)
#undef ITC_UNSEEN_PLAIN

#define ITC_UNSEEN_TRANSITION(name)                                                         \
  void RaceScreensUnseenPause4##name(PPCContext& ctx, uint8_t* base) {                      \
    GuestCalls guest(ctx, base);                                                            \
    ctx.r3.u64 = race_screens::Pause4##name(base, ctx.r3.u32, ctx.r1.u32, guest.calls());   \
  }
ITC_UNSEEN_TRANSITION(RunTen)
ITC_UNSEEN_TRANSITION(OpenMain)
ITC_UNSEEN_TRANSITION(ShowMain)
ITC_UNSEEN_TRANSITION(BackToMain)
ITC_UNSEEN_TRANSITION(MoveMain)
ITC_UNSEEN_TRANSITION(CloseMain)
ITC_UNSEEN_TRANSITION(OpenVolume)
ITC_UNSEEN_TRANSITION(ShowVolumeRow)
ITC_UNSEEN_TRANSITION(MoveVolume)
ITC_UNSEEN_TRANSITION(SetVolume)
ITC_UNSEEN_TRANSITION(CloseVolume)
#undef ITC_UNSEEN_TRANSITION

void RaceScreensUnseenPause4ConfirmPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Pause4ConfirmPage(base, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenPause4RestoreNodes(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::Pause4RestoreNodes(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenPause4ShowVolume(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::Pause4ShowVolume(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenPause4SetArrow(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = race_screens::Pause4SetArrow(base, ctx.r3.u64, ctx.r1.u32);
}

bool NativeSlotsUnseen() { return ITC_FLAG(native_action_mappers); }

void RaceScreensUnseenRaceResultsOpen(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceResultsOpen(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
                                             guest.calls());
}

void RaceScreensUnseenRaceResultsUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceResultsUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32,
                                               NativeSlotsUnseen(), guest.calls());
}

void RaceScreensUnseenRaceResultsDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceResultsDraw(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenRaceResultsSetTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::RaceResultsSetTexts(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenRaceResultsMainPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::RaceResultsMainPage(base, ctx.r1.u32, NativeSlotsUnseen(), guest.calls());
}

void RaceScreensUnseenSplitResultsOpen(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::SplitResultsOpen(base, ctx.r3.u32, ctx.r4.u64, ctx.r5.u64,
                                              ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenSplitResultsUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::SplitResultsUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32,
                                                NativeSlotsUnseen(), guest.calls());
}

void RaceScreensUnseenSplitResultsMainPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::SplitResultsMainPage(base, ctx.r1.u32, NativeSlotsUnseen(), guest.calls());
}

#define ITC_UNSEEN_RESULTS_TRANSITION(name)                                              \
  void RaceScreensUnseen##name(PPCContext& ctx, uint8_t* base) {                         \
    GuestCalls guest(ctx, base);                                                         \
    ctx.r3.u64 = race_screens::name(base, ctx.r3.u32, ctx.r1.u32, guest.calls());        \
  }
ITC_UNSEEN_RESULTS_TRANSITION(RaceResultsIntro)
ITC_UNSEEN_RESULTS_TRANSITION(RaceResultsShow)
ITC_UNSEEN_RESULTS_TRANSITION(RaceResultsClose)
ITC_UNSEEN_RESULTS_TRANSITION(SplitResultsIntro)
ITC_UNSEEN_RESULTS_TRANSITION(SplitResultsShow)
ITC_UNSEEN_RESULTS_TRANSITION(SplitResultsClose)
#undef ITC_UNSEEN_RESULTS_TRANSITION

void RaceScreensUnseenKind6Open(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      race_screens::Kind6Open(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenKind6Draw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Kind6Draw(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenKind6SetUpTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Kind6SetUpTexts(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenKind6SetCursorX(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = race_screens::Kind6SetCursorX(base, ctx.r3.u64, ctx.r1.u32);
}

void RaceScreensUnseenKind6ShowReplay(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Kind6ShowReplay(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

#define ITC_UNSEEN_KIND6(name)                                                \
  void RaceScreensUnseenKind6##name(PPCContext& ctx, uint8_t* base) {         \
    GuestCalls guest(ctx, base);                                              \
    ctx.r3.u64 = race_screens::Kind6##name(base, ctx.r1.u32, guest.calls());  \
  }
ITC_UNSEEN_KIND6(Close)
ITC_UNSEEN_KIND6(ShowPanelA)
ITC_UNSEEN_KIND6(SetUpList)
ITC_UNSEEN_KIND6(ShowCourse)
ITC_UNSEEN_KIND6(ShowHelp)
ITC_UNSEEN_KIND6(HidePanels)
ITC_UNSEEN_KIND6(ShowList)
#undef ITC_UNSEEN_KIND6

#define ITC_UNSEEN_KIND6_TRANSITION(name)                                                    \
  void RaceScreensUnseenKind6##name(PPCContext& ctx, uint8_t* base) {                        \
    GuestCalls guest(ctx, base);                                                             \
    ctx.r3.u64 = race_screens::Kind6##name(base, ctx.r3.u32, ctx.r1.u32, guest.calls());     \
  }
ITC_UNSEEN_KIND6_TRANSITION(Intro)
ITC_UNSEEN_KIND6_TRANSITION(Show)
ITC_UNSEEN_KIND6_TRANSITION(RunToEnd)
ITC_UNSEEN_KIND6_TRANSITION(MoveMain)
ITC_UNSEEN_KIND6_TRANSITION(BackToQuit)
ITC_UNSEEN_KIND6_TRANSITION(OpenList)
ITC_UNSEEN_KIND6_TRANSITION(CloseList)
ITC_UNSEEN_KIND6_TRANSITION(MoveList)
ITC_UNSEEN_KIND6_TRANSITION(OpenQuestion)
ITC_UNSEEN_KIND6_TRANSITION(CloseQuestion)
ITC_UNSEEN_KIND6_TRANSITION(MoveQuestion)
ITC_UNSEEN_KIND6_TRANSITION(OpenMessage)
ITC_UNSEEN_KIND6_TRANSITION(CloseMessage)
ITC_UNSEEN_KIND6_TRANSITION(OpenWaiting)
ITC_UNSEEN_KIND6_TRANSITION(CloseWaiting)
#undef ITC_UNSEEN_KIND6_TRANSITION

void RaceScreensUnseenKind6Update(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Kind6Update(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32, guest.calls());
}

void RaceScreensUnseenKind6AnswerPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::Kind6AnswerPage(base);
}

void RaceScreensUnseenKind6QuestionAnswerPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::Kind6QuestionAnswerPage(base, ctx.r1.u32);
}

void RaceScreensUnseenKind6WaitingPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::Kind6WaitingPage(base, ctx.r1.u32);
}

void RaceScreensUnseenKind6RankingTime(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::Kind6RankingTime(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

void RaceScreensUnseenKind6Rank(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = race_screens::Kind6Rank(base);
}

void RaceScreensUnseenKind6CarLogo(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = race_screens::Kind6CarLogo(ctx.r3.u32);
}

void RaceScreensUnseenKind6NetState(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = race_screens::Kind6NetState(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

#define ITC_UNSEEN_KIND6_DIALOG_PAGE(name)                                                   \
  void RaceScreensUnseenKind6##name(PPCContext& ctx, uint8_t* base) {                        \
    GuestCalls guest(ctx, base);                                                             \
    ctx.r3.u64 = race_screens::Kind6##name(base, ctx.r4.u64, ctx.r1.u32, guest.calls());     \
  }
ITC_UNSEEN_KIND6_DIALOG_PAGE(SavePage)
ITC_UNSEEN_KIND6_DIALOG_PAGE(OverwritePage)
ITC_UNSEEN_KIND6_DIALOG_PAGE(SavingPage)
ITC_UNSEEN_KIND6_DIALOG_PAGE(QuitPage)
#undef ITC_UNSEEN_KIND6_DIALOG_PAGE

#define ITC_UNSEEN_KIND6_PAGE(name)                                           \
  void RaceScreensUnseenKind6##name(PPCContext& ctx, uint8_t* base) {         \
    GuestCalls guest(ctx, base);                                              \
    ctx.r3.u64 = race_screens::Kind6##name(base, ctx.r1.u32, guest.calls());  \
  }
ITC_UNSEEN_KIND6_PAGE(MainPage)
ITC_UNSEEN_KIND6_PAGE(SlotPage)
ITC_UNSEEN_KIND6_PAGE(CheckRecordPage)
ITC_UNSEEN_KIND6_PAGE(RankingReadPage)
ITC_UNSEEN_KIND6_PAGE(UploadPage)
ITC_UNSEEN_KIND6_PAGE(MenuBackPage)
ITC_UNSEEN_KIND6_PAGE(UploadResultPage)
ITC_UNSEEN_KIND6_PAGE(RankingPage)
ITC_UNSEEN_KIND6_PAGE(MessageBackPage)
ITC_UNSEEN_KIND6_PAGE(QuestionPage)
ITC_UNSEEN_KIND6_PAGE(MessagePage)
ITC_UNSEEN_KIND6_PAGE(ShowPanelB)
ITC_UNSEEN_KIND6_PAGE(ShowTimeOnly)
ITC_UNSEEN_KIND6_PAGE(ShowRanking)
#undef ITC_UNSEEN_KIND6_PAGE

}  // namespace hooks

#define ITC_RACE_SCREENS_UNSEEN(address, step)    \
  extern "C" REX_FUNC(sub_##address) {            \
    REX_FUNC_PROLOGUE();                          \
    if (!ITC_FLAG(native_race_screens_unseen)) {  \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    hooks::step(ctx, base);                       \
  }

ITC_RACE_SCREENS_UNSEEN(82211390, RaceScreensUnseenPause4Open)
ITC_RACE_SCREENS_UNSEEN(82211458, RaceScreensUnseenPause4Update)
ITC_RACE_SCREENS_UNSEEN(82211590, RaceScreensUnseenPause4Draw)
ITC_RACE_SCREENS_UNSEEN(8221F990, RaceScreensUnseenRaceResultsOpen)
ITC_RACE_SCREENS_UNSEEN(8221F930, RaceScreensUnseenRaceResultsUpdate)
ITC_RACE_SCREENS_UNSEEN(8221F7D0, RaceScreensUnseenRaceResultsDraw)
ITC_RACE_SCREENS_UNSEEN(82220050, RaceScreensUnseenSplitResultsOpen)
ITC_RACE_SCREENS_UNSEEN(8221FFF0, RaceScreensUnseenSplitResultsUpdate)
// SplitResults' transitions: the recompiled SplitResults::Draw (sub_822200E0,
// kept for its network battle's sends) reaches them through its table.
ITC_RACE_SCREENS_UNSEEN(8221FAC8, RaceScreensUnseenSplitResultsIntro)
ITC_RACE_SCREENS_UNSEEN(8221FBE8, RaceScreensUnseenSplitResultsShow)
ITC_RACE_SCREENS_UNSEEN(8221FD98, RaceScreensUnseenSplitResultsClose)
// Kind 6's screen after the race: its opener, draw and close (the race HUD
// machine's recompiled fallbacks reach them). ShowPanelA and SetUpList have
// no adapters since Update's pages are native (all their callers are).
ITC_RACE_SCREENS_UNSEEN(8221C168, RaceScreensUnseenKind6Close)
ITC_RACE_SCREENS_UNSEEN(8221EC48, RaceScreensUnseenKind6Open)
ITC_RACE_SCREENS_UNSEEN(8221EF60, RaceScreensUnseenKind6Draw)
// Kind6Menu::Update (the race HUD machine's AfterRaceUpdate fallback reaches
// it); its pages and their helpers have no adapters.
ITC_RACE_SCREENS_UNSEEN(8221F1E8, RaceScreensUnseenKind6Update)
