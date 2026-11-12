// The type 12 mode (the game's Xbox Live mode) natively: adapters in front
// of src/network_mode/ (docs/functions/network-mode.md), with the register
// steps the harness shares (hooks/network_mode.h). Its Xbox Live calls are
// answered by plugging in data (the user's call, 2026-11-12).

#include "hooks/network_mode.h"

#include "base/flags.h"
#include "network_mode/network_mode.h"

ITC_FLAG_BOOL(native_network_mode, true, "ITC",
              "Run the type 12 front-end mode (docs/functions/network-mode.md: the Xbox Live "
              "mode) natively instead of through the translated code: ModeType12::Init "
              "sub_82278058 (the screen start, Strings::Load, Archive::WaitIdle, "
              "StandScene::Create, VirtualAlloc, ScreenFades::FadeIn and LoadingScreen::Show "
              "through their overrides, the InitStates their plain C++); the machines' set-ups NetModeSelect::InitStates "
              "sub_8227D680, Type12Screen1::InitStates sub_82276510, Type12Screen3::InitStates "
              "sub_8225F330, NetRanking::InitStates sub_822873A0, NetTaCourse::InitStates "
              "sub_82281858, Type12Screen6 to 9::InitStates sub_82268410, sub_8226EB40, "
              "sub_82262A88 and sub_822646B0, Type12Dialog10 to 13::InitStates sub_82288600, "
              "sub_82277B18, sub_8227F6C8 and sub_8227EA50 and PowerGraphScreen12::InitStates "
              "sub_821B96B8, NetScreen::InitBase sub_8227B3E0 (with its jump sub_8227B41C) and "
              "the eight base Inits sub_82278C20, sub_82279118, sub_8227A000, sub_8227A4F8, "
              "sub_82279610, sub_82279B08, sub_8227A9F0 and sub_8227AEE8 (StateMachine::Add's "
              "plain C++); the block's helpers ModeType12::Machine sub_82277E10 (with "
              "sub_82277E44), FindShown sub_82277FF0 and AllLoaded sub_82277EE0; the delegate "
              "functions WaitForLoads sub_8227B440, NextMode sub_8227B930 (with the time "
              "attack's race set-up through the race set-up's overrides), LeaveScreen "
              "sub_8227B868, WaitForShown sub_8227B770 and Teardown sub_8227BE10; the +32 slots "
              "ForceExit sub_822788D8 and Resume sub_82278B98; Session::Free sub_82292710 with "
              "Session::Release sub_82291670 (operator delete, VirtualFree and the sound "
              "manager's slots through the guest); the machines' set-up and teardown handlers "
              "(VirtualAlloc, VirtualFree, UiMovies::Open and Delete, UiMovie::FinishAll and "
              "GarageCar::Reset through their overrides): NetModeSelect's Create sub_8227C398, "
              "OpenMovie sub_8227C078, WaitForMovie sub_8227C558, Leave sub_8227C8C0, Destroy "
              "sub_8227C4B8; Type12Screen1's sub_822720A8 (with ResetData sub_82272030), "
              "sub_82271D18, sub_82272398, sub_822727E0, sub_822722B0; Type12Screen3's "
              "sub_8225DFE8, sub_8225DFD8, sub_8225E1B0, sub_8225E518, sub_8225E100; NetRanking's "
              "sub_82282358, sub_82282098, sub_822825C0, sub_82282848, sub_82282508; "
              "NetTaCourse's sub_8227FD40, sub_8227FC70, sub_8227FF10, sub_82280218, "
              "sub_8227FE70; Type12Screen6's sub_82265A88, sub_82265920, sub_82265C00, "
              "sub_82265DC0, sub_82265B70; Type12Screen7's sub_82268D18, sub_822688A8, "
              "sub_82268F70, sub_822697D8, sub_82268EC8; Type12Screen8's sub_82261030, "
              "sub_822609C0, sub_822611F8, sub_82261358, sub_82261168; Type12Screen9's "
              "sub_822630C0, sub_82263048, sub_822632C8, sub_82263448, sub_82263238; "
              "Type12Dialog10's sub_82287DD8, sub_82287CD0, sub_82287F40, sub_82288028, "
              "sub_82287EC0; Type12Dialog11's sub_82276E60, sub_82276E40, sub_82276FC0, "
              "sub_822770B8, sub_82276F40; Type12Dialog12's sub_8227ED58, sub_8227ECE8, "
              "sub_8227EEC0, sub_8227F038, sub_8227EE40; Type12Dialog13's sub_8227DED0, "
              "sub_8227DEC0, sub_8227E030, sub_8227E118, sub_8227DFB0; the mode select's running "
              "screen (the movie's setters and players, the fades, Menu::Step and Pressed, "
              "MenuSound::Play and SaveSlots::State through their overrides; the privilege "
              "check sub_8214F620 plugged in as 0, XShowSigninUI not called): Start "
              "sub_8227D300 with SetUp sub_8227CCE0, Open sub_8227C5C8, Draw sub_8227D248, "
              "Update sub_8227D4E8, the pages MenuPage sub_8227C658 (with Session::Make "
              "sub_822926B8 and Session::Init sub_822915C0), LeavePage sub_8227C970, BattlePage "
              "sub_8227C9F0, TimeAttackPage sub_8227CB38, SignInPage sub_8227C088, SignInPage2 "
              "sub_8227C210 and BackPage sub_8227CC78, the transitions Intro sub_8227CE50, "
              "ShowCursor sub_8227D138, MoveCursor sub_8227D380 with SetCursorY sub_8227D1D8, "
              "FadeOut sub_8227D088 and FadeIn sub_8227CFA8, the replies OnNo sub_8227BF80, "
              "OnYes sub_8227BFB8 and OnDialogClosed sub_8227BFD8, Unload sub_8227C7A8 and "
              "Unloaded sub_8227C810 (checked by "
              "--run_harnesses=network_mode)");

namespace hooks {

namespace {

namespace nm = network_mode;

// The guest calls the plain C++ makes (network_mode::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  nm::Calls calls() { return {this, &GuestCalls::Call}; }

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

// StateMachine::Add's r3: a new transition's address from the machine's
// 64-bit r3, as the type 9 mode's Inits.
uint64_t AddR3(uint64_t machine_r3, uint32_t r3, bool replaced) {
  return replaced ? r3 : machine_r3 + (r3 - uint32_t(machine_r3));
}

template <uint32_t (*InitStates)(uint8_t*, uint32_t, uint32_t, bool*), uint32_t kFrame>
void InitStatesStep(PPCContext& ctx, uint8_t* base) {
  bool replaced = false;
  const uint32_t r3 = InitStates(base, ctx.r3.u32, ctx.r1.u32 - kFrame, &replaced);
  ctx.r3.u64 = AddR3(ctx.r3.u64, r3, replaced);
}

template <uint32_t (*InitBase)(uint8_t*, uint32_t, uint32_t, uint32_t, bool*)>
void InitBaseStep(PPCContext& ctx, uint8_t* base) {
  bool replaced = false;
  const uint32_t r3 =
      InitBase(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - nm::kInitBaseFrame, &replaced);
  ctx.r3.u64 = AddR3(ctx.r3.u64, r3, replaced);
}

}  // namespace

bool NetworkModeNative() { return ITC_FLAG(native_network_mode); }

void ModeType12Init(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = nm::ModeInit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

#define ITC_NET_INIT_STATES(name, function, frame) \
  void name(PPCContext& ctx, uint8_t* base) { InitStatesStep<nm::function, nm::frame>(ctx, base); }
ITC_NET_INIT_STATES(NetModeSelectInitStates, ModeSelectInitStates, kModeSelectInitFrame)
ITC_NET_INIT_STATES(Type12Screen1InitStates, Screen1InitStates, kScreen1InitFrame)
ITC_NET_INIT_STATES(Type12Screen3InitStates, Screen3InitStates, kScreen3InitFrame)
ITC_NET_INIT_STATES(NetRankingInitStates, RankingInitStates, kRankingInitFrame)
ITC_NET_INIT_STATES(NetTaCourseInitStates, TaCourseInitStates, kTaCourseInitFrame)
ITC_NET_INIT_STATES(Type12Screen6InitStates, Screen6InitStates, kScreen6InitFrame)
ITC_NET_INIT_STATES(Type12Screen7InitStates, Screen7InitStates, kScreen7InitFrame)
ITC_NET_INIT_STATES(Type12Screen8InitStates, Screen8InitStates, kScreen8InitFrame)
ITC_NET_INIT_STATES(Type12Screen9InitStates, Screen9InitStates, kScreen9InitFrame)
ITC_NET_INIT_STATES(Type12Dialog10InitStates, Dialog10InitStates, kDialog10InitFrame)
ITC_NET_INIT_STATES(Type12Dialog11InitStates, Dialog11InitStates, kDialog11InitFrame)
ITC_NET_INIT_STATES(Type12Dialog12InitStates, Dialog12InitStates, kDialog12InitFrame)
ITC_NET_INIT_STATES(Type12Dialog13InitStates, Dialog13InitStates, kDialog13InitFrame)
ITC_NET_INIT_STATES(PowerGraphScreen12InitStates, PowerGraphInitStates, kPowerGraphInitFrame)
#undef ITC_NET_INIT_STATES

#define ITC_NET_INIT_BASE(name, function) \
  void name(PPCContext& ctx, uint8_t* base) { InitBaseStep<nm::function>(ctx, base); }
ITC_NET_INIT_BASE(Type12Screen6InitBase, Screen6InitBase)
ITC_NET_INIT_BASE(Type12Screen7InitBase, Screen7InitBase)
ITC_NET_INIT_BASE(Type12Screen8InitBase, Screen8InitBase)
ITC_NET_INIT_BASE(Type12Screen9InitBase, Screen9InitBase)
ITC_NET_INIT_BASE(Type12Dialog10InitBase, Dialog10InitBase)
ITC_NET_INIT_BASE(Type12Dialog11InitBase, Dialog11InitBase)
ITC_NET_INIT_BASE(Type12Dialog12InitBase, Dialog12InitBase)
ITC_NET_INIT_BASE(Type12Dialog13InitBase, Dialog13InitBase)
#undef ITC_NET_INIT_BASE

// r4 = r5 moved first; out of the table's range, r3 as it came.
void NetScreenInitBase(PPCContext& ctx, uint8_t* base) {
  bool replaced = false;
  uint32_t r3 = 0;
  if (nm::InitBase(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32, &replaced, &r3)) {
    ctx.r3.u64 = AddR3(ctx.r3.u64, r3, replaced);
  }
}

void ModeType12Machine(PPCContext& ctx, uint8_t* base) {
  const uint32_t machine = nm::Machine(ctx.r3.u32, ctx.r4.u32);
  // The original adds the offset to r3's 64 bits (case 0 returns r3).
  ctx.r3.u64 = machine == 0 ? 0 : ctx.r3.u64 + (machine - ctx.r3.u32);
}
// r3 left as the original's last Machine call leaves it.
void ModeType12FindShown(PPCContext& ctx, uint8_t* base) {
  ctx.r4.u64 = nm::FindShown(base, ctx.r3.u32);
  ModeType12Machine(ctx, base);
}
void ModeType12AllLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = nm::AllLoaded(base, ctx.r3.u32);
}

// The delegate functions return the answer (r3 as it came).
#define ITC_MODE_TYPE12_DELEGATE(name)                                    \
  void ModeType12##name(PPCContext& ctx, uint8_t* base) {                 \
    const uint64_t answer = ctx.r3.u64;                                   \
    {                                                                     \
      GuestCalls guest(ctx, base);                                        \
      nm::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());  \
    }                                                                     \
    ctx.r3.u64 = answer;                                                  \
  }
ITC_MODE_TYPE12_DELEGATE(WaitForLoads)
ITC_MODE_TYPE12_DELEGATE(NextMode)
ITC_MODE_TYPE12_DELEGATE(LeaveScreen)
ITC_MODE_TYPE12_DELEGATE(WaitForShown)
ITC_MODE_TYPE12_DELEGATE(Teardown)
#undef ITC_MODE_TYPE12_DELEGATE

void ModeType12ForceExit(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::ForceExit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}
void ModeType12Resume(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = nm::Resume(base, ctx.r3.u32, ctx.r1.u32);
}
void SessionFree(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::SessionFree(base, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}

template <uint32_t kIndex>
void NetMachineCreate(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::MachineCreate(base, kIndex, ctx.r3.u32, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}
template <uint32_t kIndex>
void NetMachineOpenMovie(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::MachineOpenMovie(base, kIndex, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
  }
  ctx.r3.u64 = r3;
}
template <uint32_t kIndex>
void NetMachineWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = nm::MachineWaitForMovie(base, kIndex, ctx.r3.u64, ctx.r1.u32);
}
template <uint32_t kIndex>
void NetMachineLeave(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::MachineLeave(base, kIndex, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}
template <uint32_t kIndex>
void NetMachineDestroy(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::MachineDestroy(base, kIndex, ctx.r3.u32, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}
#define ITC_NET_MACHINE(index)                                       \
  template void NetMachineCreate<index>(PPCContext&, uint8_t*);      \
  template void NetMachineOpenMovie<index>(PPCContext&, uint8_t*);   \
  template void NetMachineWaitForMovie<index>(PPCContext&, uint8_t*); \
  template void NetMachineLeave<index>(PPCContext&, uint8_t*);       \
  template void NetMachineDestroy<index>(PPCContext&, uint8_t*);
ITC_NET_MACHINE(0)
ITC_NET_MACHINE(1)
ITC_NET_MACHINE(3)
ITC_NET_MACHINE(4)
ITC_NET_MACHINE(5)
ITC_NET_MACHINE(6)
ITC_NET_MACHINE(7)
ITC_NET_MACHINE(8)
ITC_NET_MACHINE(9)
ITC_NET_MACHINE(10)
ITC_NET_MACHINE(11)
ITC_NET_MACHINE(12)
ITC_NET_MACHINE(13)
#undef ITC_NET_MACHINE

void Type12Screen1ResetData(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::Screen1ResetData(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}

void SessionMake(PPCContext& ctx, uint8_t* base) {
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = nm::SessionMake(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
  }
  ctx.r3.u64 = r3;
}
void SessionInit(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = nm::SessionInit(base, ctx.r3.u64, ctx.r1.u32);
}

// The mode select's steps: those making guest calls through GuestCalls,
// r3 as the plain C++ answers; the leaves leave r3 as it came.
#define ITC_NET_SELECT_CALLS(name, expression) \
  void name(PPCContext& ctx, uint8_t* base) {  \
    uint64_t r3;                               \
    {                                          \
      GuestCalls guest(ctx, base);             \
      const nm::Calls calls = guest.calls();   \
      r3 = expression;                         \
    }                                          \
    ctx.r3.u64 = r3;                           \
  }
ITC_NET_SELECT_CALLS(NetModeSelectStart, nm::SelectStart(base, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectSetUp, nm::SelectSetUp(base, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectDraw,
                     nm::SelectDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectUpdate, nm::SelectUpdate(base, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectMenuPage, nm::SelectMenuPage(base, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectBattlePage,
                     nm::SelectBattlePage(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectTimeAttackPage,
                     nm::SelectTimeAttackPage(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectSignInPage,
                     nm::SelectSignInPage(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectSignInPage2,
                     nm::SelectSignInPage2(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectIntro, nm::SelectIntro(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectShowCursor,
                     nm::SelectShowCursor(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectMoveCursor,
                     nm::SelectMoveCursor(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectFadeOut, nm::SelectFadeOut(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectFadeIn, nm::SelectFadeIn(base, ctx.r3.u32, ctx.r1.u32, calls))
ITC_NET_SELECT_CALLS(NetModeSelectUnload, nm::SelectUnload(base, ctx.r3.u32, ctx.r1.u32, calls))
#undef ITC_NET_SELECT_CALLS

void NetModeSelectOpen(PPCContext& ctx, uint8_t* base) { nm::SelectOpen(base, ctx.r3.u32); }
void NetModeSelectLeavePage(PPCContext& ctx, uint8_t* base) {
  nm::SelectLeavePage(base, ctx.r3.u32);
}
void NetModeSelectBackPage(PPCContext& ctx, uint8_t* base) {
  nm::SelectBackPage(base, ctx.r3.u32);
}
void NetModeSelectSetCursorY(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = nm::SelectSetCursorY(base, ctx.r3.u32, ctx.r1.u32);
}
void NetModeSelectOnNo(PPCContext& ctx, uint8_t* base) { nm::SelectOnNo(base); }
void NetModeSelectOnYes(PPCContext& ctx, uint8_t* base) { nm::SelectOnYes(base); }
void NetModeSelectOnDialogClosed(PPCContext& ctx, uint8_t* base) {
  nm::SelectOnDialogClosed(base);
}
void NetModeSelectUnloaded(PPCContext& ctx, uint8_t* base) {
  nm::SelectUnloaded(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

}  // namespace hooks

#define ITC_NETWORK_MODE(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_network_mode)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

// Init (the mode's vtable 0x82006C6C slot 0, from Modes::Tick).
ITC_NETWORK_MODE(82278058, ModeType12Init)
// The fourteen InitStates, NetScreen::InitBase sub_8227B3E0 and its eight
// base Inits: no adapters (only the native Init and InitStates call them;
// the register steps above are the harness's).
// The delegate functions (the vtable's slots 8, 6, 11, 10 and 12, through
// the slot thunks and Mode::NextModeThunk) and the +32 vtable 0x82006C60's
// slots 1 and 2 (no direct callers). Machine sub_82277E10 (with
// sub_82277E44), FindShown sub_82277FF0, AllLoaded sub_82277EE0 and
// Session::Release sub_82291670: no adapters (only the mode's functions and
// Session::Free, native, call them in play; the recompiled Interrupt
// sub_822785E0, never seen in play, calls the originals).
ITC_NETWORK_MODE(8227B440, ModeType12WaitForLoads)
ITC_NETWORK_MODE(8227B930, ModeType12NextMode)
ITC_NETWORK_MODE(8227B868, ModeType12LeaveScreen)
ITC_NETWORK_MODE(8227B770, ModeType12WaitForShown)
ITC_NETWORK_MODE(8227BE10, ModeType12Teardown)
ITC_NETWORK_MODE(822788D8, ModeType12ForceExit)
ITC_NETWORK_MODE(82278B98, ModeType12Resume)
// Session::Free: also from the recompiled race's end (sub_82150E00, seen in
// play), Type12Screen3's (8, 3) sub_8225E418 and sub_82211CF8.
ITC_NETWORK_MODE(82292710, SessionFree)

// The machines' set-up and teardown handlers (through their machines'
// functors: no direct callers), by machine: Create, OpenMovie,
// WaitForMovie, Leave, Destroy. Type12Screen1::ResetData sub_82272030: no
// adapter (only its Create, native, calls it).
ITC_NETWORK_MODE(8227C398, NetMachineCreate<0>)
ITC_NETWORK_MODE(8227C078, NetMachineOpenMovie<0>)
ITC_NETWORK_MODE(8227C558, NetMachineWaitForMovie<0>)
ITC_NETWORK_MODE(8227C8C0, NetMachineLeave<0>)
ITC_NETWORK_MODE(8227C4B8, NetMachineDestroy<0>)
ITC_NETWORK_MODE(822720A8, NetMachineCreate<1>)
ITC_NETWORK_MODE(82271D18, NetMachineOpenMovie<1>)
ITC_NETWORK_MODE(82272398, NetMachineWaitForMovie<1>)
ITC_NETWORK_MODE(822727E0, NetMachineLeave<1>)
ITC_NETWORK_MODE(822722B0, NetMachineDestroy<1>)
ITC_NETWORK_MODE(8225DFE8, NetMachineCreate<3>)
ITC_NETWORK_MODE(8225DFD8, NetMachineOpenMovie<3>)
ITC_NETWORK_MODE(8225E1B0, NetMachineWaitForMovie<3>)
ITC_NETWORK_MODE(8225E518, NetMachineLeave<3>)
ITC_NETWORK_MODE(8225E100, NetMachineDestroy<3>)
ITC_NETWORK_MODE(82282358, NetMachineCreate<4>)
ITC_NETWORK_MODE(82282098, NetMachineOpenMovie<4>)
ITC_NETWORK_MODE(822825C0, NetMachineWaitForMovie<4>)
ITC_NETWORK_MODE(82282848, NetMachineLeave<4>)
ITC_NETWORK_MODE(82282508, NetMachineDestroy<4>)
ITC_NETWORK_MODE(8227FD40, NetMachineCreate<5>)
ITC_NETWORK_MODE(8227FC70, NetMachineOpenMovie<5>)
ITC_NETWORK_MODE(8227FF10, NetMachineWaitForMovie<5>)
ITC_NETWORK_MODE(82280218, NetMachineLeave<5>)
ITC_NETWORK_MODE(8227FE70, NetMachineDestroy<5>)
ITC_NETWORK_MODE(82265A88, NetMachineCreate<6>)
ITC_NETWORK_MODE(82265920, NetMachineOpenMovie<6>)
ITC_NETWORK_MODE(82265C00, NetMachineWaitForMovie<6>)
ITC_NETWORK_MODE(82265DC0, NetMachineLeave<6>)
ITC_NETWORK_MODE(82265B70, NetMachineDestroy<6>)
ITC_NETWORK_MODE(82268D18, NetMachineCreate<7>)
ITC_NETWORK_MODE(822688A8, NetMachineOpenMovie<7>)
ITC_NETWORK_MODE(82268F70, NetMachineWaitForMovie<7>)
ITC_NETWORK_MODE(822697D8, NetMachineLeave<7>)
ITC_NETWORK_MODE(82268EC8, NetMachineDestroy<7>)
ITC_NETWORK_MODE(82261030, NetMachineCreate<8>)
ITC_NETWORK_MODE(822609C0, NetMachineOpenMovie<8>)
ITC_NETWORK_MODE(822611F8, NetMachineWaitForMovie<8>)
ITC_NETWORK_MODE(82261358, NetMachineLeave<8>)
ITC_NETWORK_MODE(82261168, NetMachineDestroy<8>)
ITC_NETWORK_MODE(822630C0, NetMachineCreate<9>)
ITC_NETWORK_MODE(82263048, NetMachineOpenMovie<9>)
ITC_NETWORK_MODE(822632C8, NetMachineWaitForMovie<9>)
ITC_NETWORK_MODE(82263448, NetMachineLeave<9>)
ITC_NETWORK_MODE(82263238, NetMachineDestroy<9>)
ITC_NETWORK_MODE(82287DD8, NetMachineCreate<10>)
ITC_NETWORK_MODE(82287CD0, NetMachineOpenMovie<10>)
ITC_NETWORK_MODE(82287F40, NetMachineWaitForMovie<10>)
ITC_NETWORK_MODE(82288028, NetMachineLeave<10>)
ITC_NETWORK_MODE(82287EC0, NetMachineDestroy<10>)
ITC_NETWORK_MODE(82276E60, NetMachineCreate<11>)
ITC_NETWORK_MODE(82276E40, NetMachineOpenMovie<11>)
ITC_NETWORK_MODE(82276FC0, NetMachineWaitForMovie<11>)
ITC_NETWORK_MODE(822770B8, NetMachineLeave<11>)
ITC_NETWORK_MODE(82276F40, NetMachineDestroy<11>)
ITC_NETWORK_MODE(8227ED58, NetMachineCreate<12>)
ITC_NETWORK_MODE(8227ECE8, NetMachineOpenMovie<12>)
ITC_NETWORK_MODE(8227EEC0, NetMachineWaitForMovie<12>)
ITC_NETWORK_MODE(8227F038, NetMachineLeave<12>)
ITC_NETWORK_MODE(8227EE40, NetMachineDestroy<12>)
ITC_NETWORK_MODE(8227DED0, NetMachineCreate<13>)
ITC_NETWORK_MODE(8227DEC0, NetMachineOpenMovie<13>)
ITC_NETWORK_MODE(8227E030, NetMachineWaitForMovie<13>)
ITC_NETWORK_MODE(8227E118, NetMachineLeave<13>)
ITC_NETWORK_MODE(8227DFB0, NetMachineDestroy<13>)

// The mode select's running screen (through its machine's functors and its
// Draw's and Update's tables: no direct callers). SetUp sub_8227CCE0,
// SetCursorY sub_8227D1D8, Session::Make sub_822926B8 and Session::Init
// sub_822915C0: no adapters (only Start, MoveCursor, MenuPage and
// Session::Make, native, call them).
ITC_NETWORK_MODE(8227D300, NetModeSelectStart)
ITC_NETWORK_MODE(8227C5C8, NetModeSelectOpen)
ITC_NETWORK_MODE(8227D248, NetModeSelectDraw)
ITC_NETWORK_MODE(8227D4E8, NetModeSelectUpdate)
ITC_NETWORK_MODE(8227C658, NetModeSelectMenuPage)
ITC_NETWORK_MODE(8227C970, NetModeSelectLeavePage)
ITC_NETWORK_MODE(8227C9F0, NetModeSelectBattlePage)
ITC_NETWORK_MODE(8227CB38, NetModeSelectTimeAttackPage)
ITC_NETWORK_MODE(8227C088, NetModeSelectSignInPage)
ITC_NETWORK_MODE(8227C210, NetModeSelectSignInPage2)
ITC_NETWORK_MODE(8227CC78, NetModeSelectBackPage)
ITC_NETWORK_MODE(8227CE50, NetModeSelectIntro)
ITC_NETWORK_MODE(8227D138, NetModeSelectShowCursor)
ITC_NETWORK_MODE(8227D380, NetModeSelectMoveCursor)
ITC_NETWORK_MODE(8227D088, NetModeSelectFadeOut)
ITC_NETWORK_MODE(8227CFA8, NetModeSelectFadeIn)
ITC_NETWORK_MODE(8227BF80, NetModeSelectOnNo)
ITC_NETWORK_MODE(8227BFB8, NetModeSelectOnYes)
ITC_NETWORK_MODE(8227BFD8, NetModeSelectOnDialogClosed)
ITC_NETWORK_MODE(8227C7A8, NetModeSelectUnload)
ITC_NETWORK_MODE(8227C810, NetModeSelectUnloaded)
