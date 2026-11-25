// The race menu natively: adapters in front of src/race_menu/
// (docs/functions/race-menu.md), with the register steps the harness shares
// (hooks/race_menu.h).

#include "hooks/race_menu.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "race_menu/race_menu.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_race_menu, true, "ITC",
              "Run the type 2 mode's race menu (docs/functions/race-menu.md: the career's "
              "race mode, event and variant pick over UI movie 1) natively instead of through "
              "the translated code: the machine's handlers Create sub_82188E20 and Destroy "
              "sub_82188FE0 (VirtualAlloc and VirtualFree through their overrides), OpenMovie "
              "sub_821889E0, WaitForMovie sub_821890D8 (UiMovies::IsLoaded's plain C++), OnYes "
              "sub_821895F0, OnNo sub_82189648, OnBack sub_82188938, OnDialogClosed "
              "sub_821896A0 (the fade and UiMovie::Run through their overrides), Unload "
              "sub_8218AF08 and LeaveToHub sub_8218AF80; Back sub_82188A20 and SetEventRange "
              "sub_82189158; SetUpEvents sub_821891C8 (the second memcpy's plain C++) and Refresh "
              "sub_8218BC18 (the UI movie setters through their overrides, SetText's unset "
              "varargs 0); RaceMenu::Init sub_8218CE10 (its step table, StateMachine::Add's "
              "plain C++); RaceMenu::Start sub_8218C9C8 (SaveData::Of, SetUpEvents, SetEventRange, "
              "Refresh and UiMovie::FindNode as plain C++, the sound manager's slots 14 and 13, "
              "Sound::Play and the fade through their overrides); Update's callees Transition0 "
              "sub_82189740, Transition3 sub_82189C28, Transition5 sub_8218A7F0, AskConfirm "
              "sub_8218ACE8, Transition12 sub_8218AE48, AnimateVariantPage sub_8218A928 (UiMovie's "
              "animation calls and the mapper's slots through their overrides) and the debug "
              "pages sub_8218BB68, sub_82188A98 and sub_82188C00 (their calls to the empty "
              "sub_82106540 left out); RaceMenu::Update sub_8218C678 (UiMovie::AnimationTime, "
              "Refresh, the transitions and the debug pages as plain C++, UiMovie::Run and the "
              "animation calls through their overrides); the pages Draw runs KindPage "
              "sub_8218AFD8, AnimateKindPage sub_821898C0, VariantPage sub_8218B750, EventPage "
              "sub_8218B2C8 and AnimateEventPage sub_82189FE8 (movie 16's texts with SetText's "
              "unset varargs 0) (Menu::Step, MenuSound::Play, the fade and the mapper's slots through their "
              "overrides, AnimateVariantPage and SaveData::Of as plain C++); RaceMenu::Draw "
              "sub_8218C578 (the pages as plain C++), LeavePage sub_8218B9F0 (the sound manager's "
              "slots 14 and 13 and Sound::Play through their overrides), StartPage sub_8218BB18 and "
              "StartEvent sub_82188CE8 (Career::SetUpEvent and Race::SetUp through their overrides) "
              "(the same writes; checked by --run_harnesses=race_menu,race_event)");

namespace hooks {

namespace {

namespace rm = race_menu;

// The guest calls the plain C++ makes (race_menu::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  rm::Calls calls() { return {this, &GuestCalls::Call}; }

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

}  // namespace

ITC_REGS_STEP(RaceMenuCreate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Create(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuOpenMovie) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::OpenMovie(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuWaitForMovie) {
  ctx.r3.u64 = rm::WaitForMovie(ctx.r3.u64, ctx.r1.u32);
}
ITC_REGS_STEP(RaceMenuOnYes) {
  ctx.r3.u64 = rm::OnAnswer(ctx.r3.u32, true, ctx.r1.u32);
}
ITC_REGS_STEP(RaceMenuOnNo) {
  ctx.r3.u64 = rm::OnAnswer(ctx.r3.u32, false, ctx.r1.u32);
}
ITC_REGS_STEP(RaceMenuOnBack) { rm::OnBack(ctx.r3.u32); }
ITC_REGS_STEP(RaceMenuOnDialogClosed) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::OnDialogClosed(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuUnload) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Unload(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuLeaveToHub) {
  rm::LeaveToHub(ctx.r3.u32, ctx.r4.u32);
}
ITC_REGS_STEP(RaceMenuDestroy) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Destroy(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuStart) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Start(ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuBack) { rm::Back(ctx.r3.u32); }
ITC_REGS_STEP(RaceMenuSetEventRange) {
  rm::SetEventRange(ctx.r3.u32);
}
ITC_REGS_STEP(RaceMenuSetUpEvents) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::SetUpEvents(ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuRefresh) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Refresh(ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuTransition0) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Transition0(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuTransition3) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Transition3(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuTransition5) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Transition5(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuAskConfirm) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::AskConfirm(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuTransition12) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Transition12(ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuDebugKindPage) {
  ctx.r3.u64 = rm::DebugKindPage(ctx.r1.u32);
}
ITC_REGS_STEP(RaceMenuDebugEventPage) {
  ctx.r3.u64 = rm::DebugEventPage(ctx.r3.u32, ctx.r1.u32);
}
ITC_REGS_STEP(RaceMenuDebugVariantPage) {
  ctx.r3.u64 = rm::DebugVariantPage(ctx.r1.u32);
}
ITC_REGS_STEP(RaceMenuAnimateVariantPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::AnimateVariantPage(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuKindPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::KindPage(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuAnimateKindPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::AnimateKindPage(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuVariantPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::VariantPage(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuEventPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::EventPage(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuAnimateEventPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::AnimateEventPage(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuUpdate) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Update(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuStartEvent) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::StartEvent(ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuStartPage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::StartPage(ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuLeavePage) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::LeavePage(ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(RaceMenuDraw) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = rm::Draw(ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 dialog's Init.
ITC_REGS_STEP(RaceMenuInit) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = rm::Init(machine, ctx.r1.u32 - rm::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define ITC_RACE_MENU(address, step)     \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_menu)) { \
    hooks::step(ctx);              \
  }

// The machine's handlers (functors).
ITC_RACE_MENU(82188E20, RaceMenuCreate)
ITC_RACE_MENU(821889E0, RaceMenuOpenMovie)
ITC_RACE_MENU(821890D8, RaceMenuWaitForMovie)
ITC_RACE_MENU(821895F0, RaceMenuOnYes)
ITC_RACE_MENU(82189648, RaceMenuOnNo)
ITC_RACE_MENU(82188938, RaceMenuOnBack)
ITC_RACE_MENU(821896A0, RaceMenuOnDialogClosed)
ITC_RACE_MENU(8218AF08, RaceMenuUnload)
ITC_RACE_MENU(8218AF80, RaceMenuLeaveToHub)
ITC_RACE_MENU(82188FE0, RaceMenuDestroy)
ITC_RACE_MENU(8218C9C8, RaceMenuStart)
// Update (the (5, 0) functor, from the recompiled state machine).
ITC_RACE_MENU(8218C678, RaceMenuUpdate)
// Back sub_82188A20: no adapter (removed, B7 loop, 2026-11-10: its only
// caller, Draw with its piece sub_8218C5DC, is native and calls its plain
// C++; the register step above is the harness's).
// SetEventRange sub_82189158 and SetUpEvents sub_821891C8: no adapters
// (removed, B7 loop, 2026-11-10: their only caller, Start, is native and
// calls their plain C++; the register steps above are the harness's).
// Refresh sub_8218BC18, the transitions sub_82189740, sub_82189C28,
// sub_8218A7F0, sub_8218ACE8, sub_8218AE48 and the debug pages
// sub_8218BB68, sub_82188A98, sub_82188C00: no adapters (removed, B7 loop,
// 2026-11-10: their callers, Update and Start, are native and call their
// plain C++; the register steps above are the harness's).
// AnimateVariantPage sub_8218A928: no adapter (removed, B7 loop,
// 2026-11-10: its callers, Update and VariantPage, are native and call its
// plain C++; the register step above is the harness's).
// The pages KindPage sub_8218AFD8, VariantPage sub_8218B750 and EventPage
// sub_8218B2C8: no adapters (removed, B7 loop, 2026-11-10: their only
// caller, Draw with its piece sub_8218C5DC, is native and calls their plain
// C++; the register steps above are the harness's).
// AnimateEventPage sub_82189FE8: no adapter (removed, B7 loop, 2026-11-10:
// its only caller, EventPage, is native and calls its plain C++; the
// register step above is the harness's).
// AnimateKindPage sub_821898C0: no adapter (removed, B7 loop, 2026-11-10:
// its only caller, KindPage, is native and calls its plain C++; the
// register step above is the harness's).
// Draw (the (5, 1) functor, from the recompiled state machine).
ITC_RACE_MENU(8218C578, RaceMenuDraw)
// LeavePage sub_8218B9F0, StartPage sub_8218BB18 and StartEvent sub_82188CE8:
// no adapters (removed, B7 loop, 2026-11-10: their callers, Draw and
// StartPage, are native and call their plain C++; the register steps above
// are the harness's).
// Init (from the type 2 mode's Init sub_82197278, recompiled).
ITC_RACE_MENU(8218CE10, RaceMenuInit)
