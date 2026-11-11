// The title screen natively: adapters in front of src/title_screen/
// (docs/functions/title-screen.md), with the register steps the harness
// shares (hooks/title_screen_native.h). TitleScreen::Update sub_822221A0
// keeps its wrap in hooks/title_screen.cpp (--attract_after_s).

#include "hooks/title_screen_native.h"

#include "base/flags.h"
#include "title_screen/title_screen.h"

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
              "sub_8209F6B8 (the same writes; checked by --run_harnesses=title_screen)");

namespace hooks {

namespace {

namespace ts = title_screen;

// The guest calls the plain C++ makes (title_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  ts::Calls calls() { return {this, &GuestCalls::Call}; }

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

// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 screens' Inits.
void InitSteps(PPCContext& ctx, uint8_t* base,
               uint32_t (*init)(uint8_t*, uint32_t, uint32_t, bool*), uint32_t frame_bytes) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = init(base, machine, ctx.r1.u32 - frame_bytes, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace

bool TitleScreenNative() { return ITC_FLAG(native_title_screen); }

void TitleLoadCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadOpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void TitleLoadWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::LoadWaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void TitleLoadStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadStart(base, ctx.r1.u32, guest.calls());
}
void TitleLoadDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void TitleLoadUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadUpdate(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadPlayLogo(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadPlayLogo(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadWaitForArchive(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadWaitForArchive(base, ctx.r1.u32, guest.calls());
}
void TitleLoadPostClose(PPCContext& ctx, uint8_t* base) { ts::LoadPostClose(base, ctx.r3.u32); }
void TitleLoadUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadUnload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadOpenTitle(PPCContext& ctx, uint8_t* base) {
  ts::LoadOpenTitle(base, ctx.r3.u32, ctx.r4.u32);
}
void TitleLoadLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadLeave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TitleLoadDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LoadDestroy(base, ctx.r1.u32, guest.calls());
}
void TitleLoadInit(PPCContext& ctx, uint8_t* base) {
  InitSteps(ctx, base, ts::LoadInit, ts::kLoadInitFrame);
}
void TitleScreenInit(PPCContext& ctx, uint8_t* base) {
  InitSteps(ctx, base, ts::ScreenInit, ts::kScreenInitFrame);
}

void TitleScreenCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenCreate(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenOpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void TitleScreenWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ScreenWaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void TitleScreenStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenStart(base, ctx.r1.u32, guest.calls());
}
void TitleScreenSetUp(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenSetUp(base, ctx.r1.u32, guest.calls());
}
void TitleScreenDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void TitleScreenUpdateTransitions(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenUpdate(base, ctx.r1.u32, guest.calls());
}
void TitleScreenOnGameDataLoaded(PPCContext&, uint8_t* base) { ts::ScreenOnGameDataLoaded(base); }
void TitleScreenOnInterrupt(PPCContext&, uint8_t* base) { ts::ScreenOnInterrupt(base); }
void TitleScreenOnResume(PPCContext&, uint8_t* base) { ts::ScreenOnResume(base); }
void TitleScreenUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenUnload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenOpenNext(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ScreenOpenNext(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void TitleScreenLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenLeave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ScreenDestroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenLoadGameData(PPCContext& ctx, uint8_t* base) {
  ts::ScreenLoadGameData(base, ctx.r3.u32);
}
void TitleScreenLeavePage(PPCContext& ctx, uint8_t* base) {
  ts::ScreenLeavePage(base, ctx.r3.u32, ctx.r4.u32);
}

void TitleScreenIntro(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::Intro(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenPressStartLoop(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::PressStartLoop(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenTransition2(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::BackToPressStart(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenLeavePressStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::LeavePressStart(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenShowMainMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ShowMainMenu(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenShowMainMenuAtOnce(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ShowMainMenuAtOnce(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenHighlightMain(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::HighlightMain(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenMoveMainCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::MoveMainCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenTransition9(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::RefreshSaveItem(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenTransition10(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::RefreshPadItem(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenShowCareerMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ShowCareerMenu(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenHighlightCareer(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::HighlightCareer(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenMoveCareerCursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::MoveCareerCursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenBackFromCareer(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::BackFromCareer(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenShowMenu4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ShowMenu4(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenBackFromMenu4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::BackFromMenu4(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenMoveMenu4Cursor(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::MoveMenu4Cursor(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void TitleScreenSetUpMainMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::SetUpMainMenu(base, ctx.r1.u32, guest.calls());
}
void TitleScreenSetUpCareerMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::SetUpCareerMenu(base, ctx.r1.u32, guest.calls());
}
void TitleScreenSetUpMenu4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::SetUpMenu4(base, ctx.r1.u32, guest.calls());
}
void TitleScreenCareerMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::CareerMenu(base, ctx.r1.u32, guest.calls());
}
void TitleScreenMenu4(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::Menu4(base, ctx.r1.u32, guest.calls());
}
void TitleScreenMainMenu(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::MainMenu(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void ModeType4Init(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::ModeInit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// The delegate functions return the answer (r3 as it came).
#define ITC_MODE_TYPE4_DELEGATE(name)                                       \
  void ModeType4##name(PPCContext& ctx, uint8_t* base) {                    \
    const uint64_t answer = ctx.r3.u64;                                     \
    {                                                                       \
      GuestCalls guest(ctx, base);                                          \
      ts::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());    \
    }                                                                       \
    ctx.r3.u64 = answer;                                                    \
  }
ITC_MODE_TYPE4_DELEGATE(Restart)
ITC_MODE_TYPE4_DELEGATE(NextMode)
ITC_MODE_TYPE4_DELEGATE(LeaveScreen)
ITC_MODE_TYPE4_DELEGATE(WaitForLeave)
ITC_MODE_TYPE4_DELEGATE(Leaving)
#undef ITC_MODE_TYPE4_DELEGATE
void ModeType4NextModeAnswer(PPCContext& ctx, uint8_t* base) {
  ts::NextModeAnswer(base, ctx.r3.u32, ctx.r4.u32);
}
void ModeType4Interrupt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::Interrupt(base, ctx.r3.u32, ctx.r1.u32);
}
void ModeType4ForceExit(PPCContext& ctx, uint8_t* base) {
  ts::ForceExit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void ModeType4Resume(PPCContext& ctx, uint8_t* base) { ts::Resume(base, ctx.r3.u32); }
void ModeType4MachineOf(PPCContext& ctx, uint8_t* base) {
  (void)base;
  ctx.r3.u64 = ts::MachineOf(ctx.r3.u64, ctx.r4.u64);
}
void ModeType4AllLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::AllLoaded(base, ctx.r3.u32);
}
void ModeType4FindShown(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::FindShown(base, ctx.r3.u64, ctx.r1.u32);
}

void TitleScreenPressStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ts::PressStart(base, ctx.r1.u32, guest.calls());
}
void TitleScreenOnControllerPicked(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ts::ScreenOnControllerPicked(base, ctx.r3.u64);
}
void PlayersAssignController(PPCContext& ctx, uint8_t* base) {
  ts::AssignController(base, ctx.r3.u32, ctx.r4.u32);
}

}  // namespace hooks

#define ITC_TITLE_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_title_screen)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
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
