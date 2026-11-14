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
              "stays recompiled) (the UI movie calls through their "
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
