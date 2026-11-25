// The title screen natively: adapters in front of src/title_screen/
// (docs/functions/title-screen.md), with the register steps the harness
// shares (hooks/title_screen_native.h). TitleScreen::Update sub_822221A0
// keeps its wrap in hooks/title_screen.cpp (--attract_after_s).

#include "hooks/title_screen_native.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "title_screen/title_screen.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_title_screen, true, "ITC",
              "Run the type 4 front-end mode (docs/functions/title-screen.md: the title "
              "screen, its menus and the title load before it) natively instead of through "
              "the translated code: the title load's machine, Create sub_82220BA8 and Destroy "
              "sub_82220468 (VirtualAlloc and VirtualFree through their overrides), OpenMovie "
              "sub_822204B0, WaitForMovie sub_82220C18 (UiMovies::IsLoaded's plain C++), Start "
              "sub_82220C88 (BootData::Load through its override), Draw sub_82221198 and "
              "Update sub_82221228 (the pages and transitions through the guest from their "
              "tables, Archive::QueueLength through its override), PlayLogo sub_822210C8 "
              "(UiMovie::PlayNamed, Run and the fade through their overrides), WaitForArchive "
              "sub_822204C0, PostClose sub_82221078, Unload sub_82220EF8, OpenTitle "
              "sub_82220F60 and Leave sub_82220FC8 (UiMovie::FinishAll and UiMovies::Delete "
              "through their overrides); TitleLoad::Init sub_822213B0 and TitleScreen::Init "
              "sub_82224270 (their step tables, StateMachine::Add's plain C++); the title "
              "screen's machine, Create sub_82221D98 and Destroy sub_82221EC0 (VirtualAlloc, "
              "VirtualFree and MenuList::Release through their overrides, MenuList::Selection's "
              "plain C++), OpenMovie sub_82221990, WaitForMovie sub_82221FC8, Start sub_82223AB8 "
              "with SetUp sub_82222A50 (UiMovie::ResetPose and SetText through their overrides, "
              "SetText's unset varargs 0; the sound manager's slot 14, Sound::Play and the fade "
              "through their overrides), Draw sub_82223A18 and Update sub_822240B0 (the pages "
              "and transitions through the guest from their tables, UiMovie::Run through its "
              "override), OnGameDataLoaded sub_822219A0, OnInterrupt sub_82221A78, OnResume "
              "sub_82221A90, Unload sub_822224D0, OpenNext sub_82222538, Leave sub_822225D8 and "
              "the pages LoadGameData sub_82222688 and LeavePage sub_822229D0; the "
              "transitions sub_82222B60 to sub_822235D0 (8 is the type 2 hub's) with the menus' "
              "set-ups SetUpMainMenu sub_82223700, SetUpCareerMenu sub_82223840 and SetUpMenu4 "
              "sub_82223940 (the UI movie calls, MenuList::Init, SaveSlots::State, "
              "Pad::IsConnected and TitleScreen::ResetPlayer through their overrides) and the "
              "menu pages MainMenu sub_82222700, CareerMenu sub_82221AC0 and Menu4 sub_82222378 "
              "(MenuList::Update, Accepted, Cancelled, Menu::Step and Pressed, MenuSound::Play, "
              "the Progress and Career calls and CarDealer::StartFirstRace through their "
              "overrides); the mode, ModeType4::Init sub_82224C70 (the title Inits as plain C++, "
              "the screen start, Strings::Load, VirtualAlloc and the other machines' Inits "
              "through their overrides), Restart sub_82225098, NextMode sub_82225390, "
              "LeaveScreen sub_822252C8, WaitForLeave sub_822251D0, Leaving sub_82225698, "
              "NextModeAnswer sub_82224B98 (GameMessages::Dispatch through its override), the "
              "+32 interface Interrupt sub_82224E20, ForceExit sub_82224ED0 and Resume "
              "sub_82225048, with MachineOf sub_82224B40, AllLoaded sub_82224BC0 and FindShown "
              "sub_82224C10; press start, TitleScreen::Update sub_822221A0 (its wrap kept; the "
              "mappers' slot 10, MenuSound::Play and SetUpAttractRace, recompiled, through the "
              "guest), OnControllerPicked sub_822219E0 and Players::AssignController "
              "sub_8209F6B8; the title load's debug sequence, never seen in play, its pages "
              "sub_82220CD8 to sub_82220E70 and steps sub_822204F8 to sub_82220B10 (the same writes; checked by --run_harnesses=title_screen)");

ITC_FLAG_BOOL(multiplayer_menu_items, false, "ITC",
              "Let the title screen's main menu open its two multiplayer items, Xbox Live (the "
              "type 12 mode, docs/functions/network-mode.md) and split screen (the controller "
              "pick, then the type 8 mode, docs/functions/type8-mode.md), as the original does. "
              "Off by default (the user's call, 2026-11-14: no multiplayer in this port): "
              "selecting either is silent, answered as an input the menu ignores: no sound, and "
              "the cursor, page and mode stay (the items aren't greyed out). Applies to the "
              "native main menu (TitleScreen::MainMenu sub_82222700, --native_title_screen). "
              "Off, Mode::Create's plain C++ also leaves type 12 to the original (nothing asks "
              "for it then), so the static map sees the mode unreachable; see "
              "docs/functions/title-screen.md");

namespace hooks {

namespace {

namespace ts = title_screen;

// The guest calls the plain C++ makes (title_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  ts::Calls calls() { return {this, &GuestCalls::Call}; }

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

// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 screens' Inits.
void InitSteps(lifted::Regs& ctx, uint32_t (*init)(uint32_t, uint32_t, bool*), uint32_t frame_bytes) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = init(machine, ctx.r1.u32 - frame_bytes, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace

bool TitleScreenNative() { return ITC_FLAG(native_title_screen); }

ITC_REGS_STEP(TitleLoadCreate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadCreate(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadOpenMovie) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadOpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
ITC_REGS_STEP(TitleLoadWaitForMovie) {
  ctx.r3.u64 = ts::LoadWaitForMovie(ctx.r3.u64, ctx.r1.u32);
}
ITC_REGS_STEP(TitleLoadStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadStart(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadDraw) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadDraw(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadUpdate(ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadPlayLogo) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadPlayLogo(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadWaitForArchive) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadWaitForArchive(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadPostClose) { ts::LoadPostClose(ctx.r3.u32); }
ITC_REGS_STEP(TitleLoadUnload) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadUnload(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadOpenTitle) {
  ts::LoadOpenTitle(ctx.r3.u32, ctx.r4.u32);
}
ITC_REGS_STEP(TitleLoadLeave) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadLeave(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadDestroy) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadDestroy(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleLoadInit) {
  InitSteps(ctx, ts::LoadInit, ts::kLoadInitFrame);
}
ITC_REGS_STEP(TitleScreenInit) {
  InitSteps(ctx, ts::ScreenInit, ts::kScreenInitFrame);
}

ITC_REGS_STEP(TitleScreenCreate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenCreate(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenOpenMovie) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenOpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
ITC_REGS_STEP(TitleScreenWaitForMovie) {
  ctx.r3.u64 = ts::ScreenWaitForMovie(ctx.r3.u64, ctx.r1.u32);
}
ITC_REGS_STEP(TitleScreenStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenStart(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenSetUp) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenSetUp(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenDraw) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenDraw(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenUpdateTransitions) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenUpdate(ctx.r1.u32, guest.calls());
}
void TitleScreenOnGameDataLoaded(lifted::Regs&) { ts::ScreenOnGameDataLoaded(); }
void TitleScreenOnInterrupt(lifted::Regs&) { ts::ScreenOnInterrupt(); }
void TitleScreenOnResume(lifted::Regs&) { ts::ScreenOnResume(); }
ITC_REGS_STEP(TitleScreenUnload) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenUnload(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenOpenNext) {
  ctx.r3.u64 = ts::ScreenOpenNext(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
ITC_REGS_STEP(TitleScreenLeave) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenLeave(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenDestroy) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ScreenDestroy(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenLoadGameData) {
  ts::ScreenLoadGameData(ctx.r3.u32);
}
ITC_REGS_STEP(TitleScreenLeavePage) {
  ts::ScreenLeavePage(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(TitleScreenIntro) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::Intro(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenPressStartLoop) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::PressStartLoop(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenTransition2) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::BackToPressStart(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenLeavePressStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LeavePressStart(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenShowMainMenu) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ShowMainMenu(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenShowMainMenuAtOnce) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ShowMainMenuAtOnce(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenHighlightMain) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::HighlightMain(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenMoveMainCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::MoveMainCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenTransition9) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::RefreshSaveItem(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenTransition10) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::RefreshPadItem(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenShowCareerMenu) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ShowCareerMenu(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenHighlightCareer) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::HighlightCareer(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenMoveCareerCursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::MoveCareerCursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenBackFromCareer) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::BackFromCareer(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenShowMenu4) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ShowMenu4(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenBackFromMenu4) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::BackFromMenu4(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenMoveMenu4Cursor) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::MoveMenu4Cursor(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenSetUpMainMenu) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::SetUpMainMenu(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenSetUpCareerMenu) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::SetUpCareerMenu(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenSetUpMenu4) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::SetUpMenu4(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenCareerMenu) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::CareerMenu(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenMenu4) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::Menu4(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenMainMenu) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::MainMenu(ctx.r3.u32, ctx.r1.u32, guest.calls(),
                            ITC_FLAG(multiplayer_menu_items));
}

ITC_REGS_STEP(ModeType4Init) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::ModeInit(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// The delegate functions return the answer (r3 as it came).
#define ITC_MODE_TYPE4_DELEGATE(name)                                       \
  void ModeType4##name(lifted::Regs& ctx) {                    \
    const uint64_t answer = ctx.r3.u64;                                     \
    {                                                                       \
      GuestCalls guest(ctx);                                          \
      ts::name(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());    \
    }                                                                       \
    ctx.r3.u64 = answer;                                                    \
  }
ITC_MODE_TYPE4_DELEGATE(Restart)
ITC_MODE_TYPE4_DELEGATE(NextMode)
ITC_MODE_TYPE4_DELEGATE(LeaveScreen)
ITC_MODE_TYPE4_DELEGATE(WaitForLeave)
ITC_MODE_TYPE4_DELEGATE(Leaving)
#undef ITC_MODE_TYPE4_DELEGATE
ITC_REGS_STEP(ModeType4NextModeAnswer) {
  ts::NextModeAnswer(ctx.r3.u32, ctx.r4.u32);
}
ITC_REGS_STEP(ModeType4Interrupt) {
  ctx.r3.u64 = ts::Interrupt(ctx.r3.u32, ctx.r1.u32);
}
ITC_REGS_STEP(ModeType4ForceExit) {
  ts::ForceExit(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
ITC_REGS_STEP(ModeType4Resume) { ts::Resume(ctx.r3.u32); }
ITC_REGS_STEP(ModeType4MachineOf) {
  ctx.r3.u64 = ts::MachineOf(ctx.r3.u64, ctx.r4.u64);
}
ITC_REGS_STEP(ModeType4AllLoaded) {
  ctx.r3.u64 = ts::AllLoaded(ctx.r3.u32);
}
ITC_REGS_STEP(ModeType4FindShown) {
  ctx.r3.u64 = ts::FindShown(ctx.r3.u64, ctx.r1.u32);
}

ITC_REGS_STEP(TitleScreenPressStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::PressStart(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(TitleScreenOnControllerPicked) {
  ctx.r3.u64 = ts::ScreenOnControllerPicked(ctx.r3.u64);
}
ITC_REGS_STEP(PlayersAssignController) {
  ts::AssignController(ctx.r3.u32, ctx.r4.u32);
}

template <uint32_t kWhich>
void TitleLoadDebugPage(lifted::Regs& ctx) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = ts::LoadDebugPage(kWhich, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
template <uint32_t kWhich>
void TitleLoadDebugStep(lifted::Regs& ctx) {
  ctx.r3.u64 = ts::LoadDebugStep(kWhich, ctx.r3.u32, ctx.r1.u32);
}
#define ITC_TITLE_LOAD_DEBUG(name, index)                  \
  template void name<index>(lifted::Regs&);
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugPage, 0)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugPage, 1)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugPage, 2)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugPage, 3)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 0)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 1)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 2)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 3)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 4)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 5)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 6)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 7)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 8)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 9)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 10)
ITC_TITLE_LOAD_DEBUG(TitleLoadDebugStep, 11)
#undef ITC_TITLE_LOAD_DEBUG

}  // namespace hooks

#define ITC_TITLE_SCREEN(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_title_screen)) { \
    hooks::step(ctx);               \
  }

// The title load's handlers (member function functors the machine calls; no
// direct callers).
ITC_TITLE_SCREEN(82220BA8, TitleLoadCreate)
ITC_TITLE_SCREEN(822204B0, TitleLoadOpenMovie)
ITC_TITLE_SCREEN(82220C18, TitleLoadWaitForMovie)
ITC_TITLE_SCREEN(82220C88, TitleLoadStart)
ITC_TITLE_SCREEN(82221198, TitleLoadDraw)
ITC_TITLE_SCREEN(82221228, TitleLoadUpdate)
ITC_TITLE_SCREEN(82220EF8, TitleLoadUnload)
ITC_TITLE_SCREEN(82220F60, TitleLoadOpenTitle)
ITC_TITLE_SCREEN(82220FC8, TitleLoadLeave)
ITC_TITLE_SCREEN(82220468, TitleLoadDestroy)
// Its pages (from Draw's table on its stack) and transition (from Update's).
ITC_TITLE_SCREEN(82221078, TitleLoadPostClose)
ITC_TITLE_SCREEN(822204C0, TitleLoadWaitForArchive)
ITC_TITLE_SCREEN(822210C8, TitleLoadPlayLogo)
// The Inits TitleLoad::Init sub_822213B0 and TitleScreen::Init
// sub_82224270: no adapters (removed, B7 loop, 2026-11-11: their only
// caller, ModeType4::Init, is native and calls their plain C++; the register
// steps above are the harness's).
// The title screen's handlers (functors the machine calls; no direct
// callers), Update the (5, 0) functor (the transitions).
ITC_TITLE_SCREEN(82221D98, TitleScreenCreate)
ITC_TITLE_SCREEN(82221990, TitleScreenOpenMovie)
ITC_TITLE_SCREEN(82221FC8, TitleScreenWaitForMovie)
ITC_TITLE_SCREEN(82223AB8, TitleScreenStart)
ITC_TITLE_SCREEN(82223A18, TitleScreenDraw)
ITC_TITLE_SCREEN(822240B0, TitleScreenUpdateTransitions)
ITC_TITLE_SCREEN(822219A0, TitleScreenOnGameDataLoaded)
ITC_TITLE_SCREEN(82221A78, TitleScreenOnInterrupt)
ITC_TITLE_SCREEN(82221A90, TitleScreenOnResume)
ITC_TITLE_SCREEN(822224D0, TitleScreenUnload)
ITC_TITLE_SCREEN(82222538, TitleScreenOpenNext)
ITC_TITLE_SCREEN(822225D8, TitleScreenLeave)
ITC_TITLE_SCREEN(82221EC0, TitleScreenDestroy)
// Its pages (from Draw's table on its stack). SetUp sub_82222A50: no adapter
// (its only caller, Start, is native and calls its plain C++; the register
// step above is the harness's).
ITC_TITLE_SCREEN(82222688, TitleScreenLoadGameData)
ITC_TITLE_SCREEN(822229D0, TitleScreenLeavePage)
// The transitions (from Update's table on its stack; 8 is the type 2 hub's)
// and the menu pages (from Draw's). The menus' set-ups SetUpMainMenu
// sub_82223700, SetUpCareerMenu sub_82223840 and SetUpMenu4 sub_82223940:
// no adapters (only the transitions, native, call them; the register steps
// above are the harness's).
ITC_TITLE_SCREEN(82222B60, TitleScreenIntro)
ITC_TITLE_SCREEN(82222BF8, TitleScreenPressStartLoop)
ITC_TITLE_SCREEN(82222C90, TitleScreenTransition2)
ITC_TITLE_SCREEN(82222D40, TitleScreenLeavePressStart)
ITC_TITLE_SCREEN(82223B78, TitleScreenShowMainMenu)
ITC_TITLE_SCREEN(82223C38, TitleScreenShowMainMenuAtOnce)
ITC_TITLE_SCREEN(82222DD0, TitleScreenHighlightMain)
ITC_TITLE_SCREEN(82222E88, TitleScreenMoveMainCursor)
ITC_TITLE_SCREEN(82223050, TitleScreenTransition9)
ITC_TITLE_SCREEN(822231B0, TitleScreenTransition10)
ITC_TITLE_SCREEN(82223D70, TitleScreenShowCareerMenu)
ITC_TITLE_SCREEN(82223328, TitleScreenHighlightCareer)
ITC_TITLE_SCREEN(82223428, TitleScreenMoveCareerCursor)
ITC_TITLE_SCREEN(82223E58, TitleScreenBackFromCareer)
ITC_TITLE_SCREEN(82223F30, TitleScreenShowMenu4)
ITC_TITLE_SCREEN(82223FD8, TitleScreenBackFromMenu4)
ITC_TITLE_SCREEN(822235D0, TitleScreenMoveMenu4Cursor)
ITC_TITLE_SCREEN(82222700, TitleScreenMainMenu)
ITC_TITLE_SCREEN(82221AC0, TitleScreenCareerMenu)
ITC_TITLE_SCREEN(82222378, TitleScreenMenu4)
// The mode: Init (vtable 0x82006BE0's slot 0, from Modes::Tick), the
// delegate functions (its slots 6, 8, 10 to 13, through the slot thunks)
// and the +32 interface (vtable 0x82006BD4). MachineOf sub_82224B40,
// AllLoaded sub_82224BC0 and FindShown sub_82224C10: no adapters (only the
// mode's functions, native, call them; the register steps above are the
// harness's).
ITC_TITLE_SCREEN(82224C70, ModeType4Init)
ITC_TITLE_SCREEN(82225098, ModeType4Restart)
ITC_TITLE_SCREEN(82225390, ModeType4NextMode)
ITC_TITLE_SCREEN(822252C8, ModeType4LeaveScreen)
ITC_TITLE_SCREEN(822251D0, ModeType4WaitForLeave)
ITC_TITLE_SCREEN(82225698, ModeType4Leaving)
ITC_TITLE_SCREEN(82224B98, ModeType4NextModeAnswer)
ITC_TITLE_SCREEN(82224E20, ModeType4Interrupt)
ITC_TITLE_SCREEN(82224ED0, ModeType4ForceExit)
ITC_TITLE_SCREEN(82225048, ModeType4Resume)
// "Press start": OnControllerPicked (the (5, 16) functor). TitleScreen::
// Update sub_822221A0's override is the wrap in hooks/title_screen.cpp
// (--attract_after_s around TitleScreenPressStart). Players::AssignController
// sub_8209F6B8: no adapter (its callers in play, Update and
// OnControllerPicked, are native; sub_8209E698, never seen in play, reaches
// the original).
ITC_TITLE_SCREEN(822219E0, TitleScreenOnControllerPicked)
// The title load's debug sequence, never seen in play (from Draw's and
// Update's tables).
ITC_TITLE_SCREEN(82220CD8, TitleLoadDebugPage<0>)
ITC_TITLE_SCREEN(82220D60, TitleLoadDebugPage<1>)
ITC_TITLE_SCREEN(82220DE8, TitleLoadDebugPage<2>)
ITC_TITLE_SCREEN(82220E70, TitleLoadDebugPage<3>)
ITC_TITLE_SCREEN(822204F8, TitleLoadDebugStep<0>)
ITC_TITLE_SCREEN(82220580, TitleLoadDebugStep<1>)
ITC_TITLE_SCREEN(82220610, TitleLoadDebugStep<2>)
ITC_TITLE_SCREEN(82220698, TitleLoadDebugStep<3>)
ITC_TITLE_SCREEN(82220720, TitleLoadDebugStep<4>)
ITC_TITLE_SCREEN(822207B0, TitleLoadDebugStep<5>)
ITC_TITLE_SCREEN(82220840, TitleLoadDebugStep<6>)
ITC_TITLE_SCREEN(822208D0, TitleLoadDebugStep<7>)
ITC_TITLE_SCREEN(82220960, TitleLoadDebugStep<8>)
ITC_TITLE_SCREEN(822209F0, TitleLoadDebugStep<9>)
ITC_TITLE_SCREEN(82220A80, TitleLoadDebugStep<10>)
ITC_TITLE_SCREEN(82220B10, TitleLoadDebugStep<11>)
