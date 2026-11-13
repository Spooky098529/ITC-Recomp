// The type 10 mode (the replay theater) natively: adapters in front of
// src/type10_mode/ (docs/functions/type10-mode.md), with the register steps
// the harness shares (hooks/type10_mode.h).

#include "hooks/type10_mode.h"

#include "base/flags.h"
#include "type10_mode/type10_mode.h"

ITC_FLAG_BOOL(native_type10_mode, true, "ITC",
              "Run the type 10 front-end mode (docs/functions/type10-mode.md: the replay "
              "theater) natively instead of through the translated code: the mode, "
              "ModeType10::Init sub_82288940 (the theater's InitStates sub_8228AC48 as plain "
              "C++ over StateMachine::Add's; the screen start, Strings::Load, VirtualAlloc, the "
              "save screen's Init and ScreenFades::FadeIn through their overrides), "
              "WaitForLoads sub_82288B70, NextMode sub_82288E68, LeaveScreen sub_82288DA0, "
              "WaitForShown sub_82288CA0 (LoadingScreen::Run through its override), Leaving "
              "sub_822890D8 and ModeType9::NextModeAnswer sub_8225D638 (GameMessages::Dispatch "
              "through its override; FindShown sub_822888F0 their plain C++), the +32 "
              "interface Interrupt sub_82288A68 (OnList sub_822893E8), ModeType9::ForceExit "
              "sub_8225D828 and Resume sub_82288B20 (NextModeAnswer and ForceExit are the type "
              "9 mode's too, the same code); the theater's machine, Create sub_82289408 and "
              "Destroy sub_822894E8 (VirtualAlloc and VirtualFree through their overrides), "
              "OpenMovie sub_822891F8, WaitForMovie sub_82289570, Start sub_8228AA58 with "
              "StartPage sub_8228A508 (SaveBuffers::Create through its override), Draw "
              "sub_8228A868 and Update sub_8228A8F8 (the pages and transitions through the "
              "guest from their tables, UiMovie::Run through its override), the save "
              "screen's replies OnDone sub_8228AAB0 (Replay::SetUpRace, recompiled, through "
              "the guest), OnFailed sub_82289208, OnRetry sub_8228ABE0, OnBack sub_82289308, "
              "OnInterrupt sub_82289358 and OnResume sub_822893A8, Unload sub_82289600, Wait "
              "sub_82289668 and Leave sub_822896C0 (SaveBuffers::Destroy, UiMovie::FinishAll "
              "and UiMovies::Delete through their overrides); the pages ListPage sub_82289768 "
              "(SaveSlots::State, MenuList::Update, Accepted, Cancelled and MenuSound::Play "
              "through their overrides), AskList sub_82289958, AskLoad sub_822899E0, LeavePage "
              "sub_82289A70 and WaitPage sub_8228A4B0 with SetPrompts sub_8228A3B8; the "
              "transitions Intro sub_82289AC0, FadeOut sub_82289BD0, FadeInList sub_82289CC0, "
              "ShowCursor sub_82289D70, MoveCursor sub_8228A658, HideCursor sub_82289E08 and "
              "PickReplay sub_82289EB8 (the UI movie calls and the fades through their "
              "overrides) with the list's BuildList sub_8228A560 (MenuList::Init through its "
              "override), ShowListPage sub_82289F48 and ShowRecord sub_8228A0D8 (the text "
              "setters through their overrides, their unset varargs 0) (checked by "
              "--run_harnesses=type10_mode)");

namespace hooks {

namespace {

namespace t10 = type10_mode;

// The guest calls the plain C++ makes (type10_mode::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  t10::Calls calls() { return {this, &GuestCalls::Call}; }

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

bool Type10ModeNative() { return ITC_FLAG(native_type10_mode); }

void ModeType10Init(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::ModeInit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// The delegate functions return the answer (r3 as it came).
#define ITC_MODE_TYPE10_DELEGATE(name)                                      \
  void ModeType10##name(PPCContext& ctx, uint8_t* base) {                   \
    const uint64_t answer = ctx.r3.u64;                                     \
    {                                                                       \
      GuestCalls guest(ctx, base);                                          \
      t10::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());   \
    }                                                                       \
    ctx.r3.u64 = answer;                                                    \
  }
ITC_MODE_TYPE10_DELEGATE(WaitForLoads)
ITC_MODE_TYPE10_DELEGATE(NextMode)
ITC_MODE_TYPE10_DELEGATE(LeaveScreen)
ITC_MODE_TYPE10_DELEGATE(WaitForShown)
ITC_MODE_TYPE10_DELEGATE(Leaving)
#undef ITC_MODE_TYPE10_DELEGATE
void ModeType10NextModeAnswer(PPCContext& ctx, uint8_t* base) {
  t10::NextModeAnswer(base, ctx.r3.u32, ctx.r4.u32);
}
void ModeType10Interrupt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t10::Interrupt(base, ctx.r3.u32, ctx.r1.u32);
}
void ModeType10ForceExit(PPCContext& ctx, uint8_t* base) {
  t10::ForceExit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void ModeType10Resume(PPCContext& ctx, uint8_t* base) { t10::Resume(base, ctx.r3.u32); }
void ModeType10FindShown(PPCContext& ctx, uint8_t* base) { t10::FindShown(base, ctx.r3.u32); }
void ReplayTheaterInitStates(PPCContext& ctx, uint8_t* base) {
  // StateMachine::Add's r3: a new transition's address from the machine's
  // 64-bit r3, as the title screen's Inits.
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 =
      t10::InitStates(base, machine, ctx.r1.u32 - t10::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
void ReplayTheaterOnList(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = t10::OnList(base); }

void ReplayTheaterCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void ReplayTheaterOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::OpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void ReplayTheaterWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t10::WaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void ReplayTheaterStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Start(base, ctx.r1.u32, guest.calls());
}
void ReplayTheaterStartPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::StartPage(base, ctx.r1.u32, guest.calls());
}
void ReplayTheaterDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void ReplayTheaterUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Update(base, ctx.r1.u32, guest.calls());
}
void ReplayTheaterOnDone(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::OnDone(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void ReplayTheaterOnFailed(PPCContext&, uint8_t* base) { t10::OnFailed(base); }
void ReplayTheaterOnRetry(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::OnRetry(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void ReplayTheaterOnBack(PPCContext&, uint8_t* base) { t10::OnBack(base); }
void ReplayTheaterOnInterrupt(PPCContext&, uint8_t* base) { t10::OnInterrupt(base); }
void ReplayTheaterOnResume(PPCContext&, uint8_t* base) { t10::OnResume(base); }
void ReplayTheaterUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Unload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void ReplayTheaterWait(PPCContext& ctx, uint8_t* base) {
  t10::Wait(base, ctx.r3.u32, ctx.r4.u32);
}
void ReplayTheaterLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Leave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void ReplayTheaterDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::Destroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void ReplayTheaterListPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::ListPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void ReplayTheaterAskList(PPCContext& ctx, uint8_t* base) { t10::AskList(base, ctx.r3.u32); }
void ReplayTheaterAskLoad(PPCContext& ctx, uint8_t* base) { t10::AskLoad(base, ctx.r3.u32); }
void ReplayTheaterLeavePage(PPCContext& ctx, uint8_t* base) {
  t10::LeavePage(base, ctx.r3.u32);
}
void ReplayTheaterWaitPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::WaitPage(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void ReplayTheaterSetPrompts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  const uint32_t a[3] = {ctx.r3.u32, ctx.r5.u32, ctx.r7.u32};
  const uint32_t b[3] = {ctx.r4.u32, ctx.r6.u32, ctx.r8.u32};
  ctx.r3.u64 = t10::SetPrompts(base, a, b, ctx.r1.u32, guest.calls());
}

#define ITC_REPLAY_THEATER_TRANSITION(name)                                         \
  void ReplayTheater##name(PPCContext& ctx, uint8_t* base) {                        \
    GuestCalls guest(ctx, base);                                                    \
    ctx.r3.u64 = t10::name(base, ctx.r3.u32, ctx.r1.u32, guest.calls());            \
  }
ITC_REPLAY_THEATER_TRANSITION(Intro)
ITC_REPLAY_THEATER_TRANSITION(FadeOut)
ITC_REPLAY_THEATER_TRANSITION(FadeInList)
ITC_REPLAY_THEATER_TRANSITION(ShowCursor)
ITC_REPLAY_THEATER_TRANSITION(MoveCursor)
ITC_REPLAY_THEATER_TRANSITION(HideCursor)
ITC_REPLAY_THEATER_TRANSITION(PickReplay)
ITC_REPLAY_THEATER_TRANSITION(ShowRecord)
#undef ITC_REPLAY_THEATER_TRANSITION
void ReplayTheaterBuildList(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::BuildList(base, ctx.r1.u32, guest.calls());
}
void ReplayTheaterShowListPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t10::ShowListPage(base, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_TYPE10_MODE(address, step)   \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_type10_mode)) { \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// The mode: Init (vtable 0x82006CF0's slot 0, from Modes::Tick), the
// delegate functions (its slots 6, 8, 10 to 13, through the slot thunks; 12
// and the +32 slot 1 the type 9 mode's too) and the +32 interface (vtable
// 0x82006CE4). FindShown sub_822888F0, ReplayTheater::InitStates
// sub_8228AC48 and OnList sub_822893E8: no adapters (only the mode's
// functions, native, call them; the register steps above are the
// harness's).
ITC_TYPE10_MODE(82288940, ModeType10Init)
ITC_TYPE10_MODE(82288B70, ModeType10WaitForLoads)
ITC_TYPE10_MODE(82288E68, ModeType10NextMode)
ITC_TYPE10_MODE(82288DA0, ModeType10LeaveScreen)
ITC_TYPE10_MODE(82288CA0, ModeType10WaitForShown)
ITC_TYPE10_MODE(822890D8, ModeType10Leaving)
ITC_TYPE10_MODE(8225D638, ModeType10NextModeAnswer)
ITC_TYPE10_MODE(82288A68, ModeType10Interrupt)
ITC_TYPE10_MODE(8225D828, ModeType10ForceExit)
ITC_TYPE10_MODE(82288B20, ModeType10Resume)
// The theater's handlers (member function functors the machine calls; no
// direct callers). StartPage sub_8228A508: no adapter (only Start, native,
// calls it).
ITC_TYPE10_MODE(82289408, ReplayTheaterCreate)
ITC_TYPE10_MODE(822891F8, ReplayTheaterOpenMovie)
ITC_TYPE10_MODE(82289570, ReplayTheaterWaitForMovie)
ITC_TYPE10_MODE(8228AA58, ReplayTheaterStart)
ITC_TYPE10_MODE(8228A868, ReplayTheaterDraw)
ITC_TYPE10_MODE(8228A8F8, ReplayTheaterUpdate)
ITC_TYPE10_MODE(8228AAB0, ReplayTheaterOnDone)
ITC_TYPE10_MODE(82289208, ReplayTheaterOnFailed)
ITC_TYPE10_MODE(8228ABE0, ReplayTheaterOnRetry)
ITC_TYPE10_MODE(82289308, ReplayTheaterOnBack)
ITC_TYPE10_MODE(82289358, ReplayTheaterOnInterrupt)
ITC_TYPE10_MODE(822893A8, ReplayTheaterOnResume)
ITC_TYPE10_MODE(82289600, ReplayTheaterUnload)
ITC_TYPE10_MODE(82289668, ReplayTheaterWait)
ITC_TYPE10_MODE(822896C0, ReplayTheaterLeave)
ITC_TYPE10_MODE(822894E8, ReplayTheaterDestroy)
// The pages (from Draw's table on its stack). SetPrompts sub_8228A3B8: no
// adapter (only WaitPage, StartPage and BuildList, native, call it).
ITC_TYPE10_MODE(82289768, ReplayTheaterListPage)
ITC_TYPE10_MODE(82289958, ReplayTheaterAskList)
ITC_TYPE10_MODE(822899E0, ReplayTheaterAskLoad)
ITC_TYPE10_MODE(82289A70, ReplayTheaterLeavePage)
ITC_TYPE10_MODE(8228A4B0, ReplayTheaterWaitPage)
// The transitions (from Update's table on its stack). BuildList
// sub_8228A560, ShowListPage sub_82289F48 and ShowRecord sub_8228A0D8: no
// adapters (only OnDone, OnRetry, BuildList and MoveCursor, native, call
// them).
ITC_TYPE10_MODE(82289AC0, ReplayTheaterIntro)
ITC_TYPE10_MODE(82289BD0, ReplayTheaterFadeOut)
ITC_TYPE10_MODE(82289CC0, ReplayTheaterFadeInList)
ITC_TYPE10_MODE(82289D70, ReplayTheaterShowCursor)
ITC_TYPE10_MODE(8228A658, ReplayTheaterMoveCursor)
ITC_TYPE10_MODE(82289E08, ReplayTheaterHideCursor)
ITC_TYPE10_MODE(82289EB8, ReplayTheaterPickReplay)
