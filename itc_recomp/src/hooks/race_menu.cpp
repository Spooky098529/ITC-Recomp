// The race menu natively: adapters in front of src/race_menu/
// (docs/functions/race-menu.md), with the register steps the harness shares
// (hooks/race_menu.h).

#include "hooks/race_menu.h"

#include "base/flags.h"
#include "race_menu/race_menu.h"

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
              "sub_8218AFD8, AnimateKindPage sub_821898C0 and VariantPage sub_8218B750 "
              "(Menu::Step, MenuSound::Play, the fade and the mapper's slots through their "
              "overrides, AnimateVariantPage as plain C++) (the same writes; checked by "
              "--run_harnesses=race_menu)");

namespace hooks {

namespace {

namespace rm = race_menu;

// The guest calls the plain C++ makes (race_menu::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  rm::Calls calls() { return {this, &GuestCalls::Call}; }

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

void RaceMenuCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::OpenMovie(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rm::WaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}
void RaceMenuOnYes(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rm::OnAnswer(base, ctx.r3.u32, true, ctx.r1.u32);
}
void RaceMenuOnNo(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rm::OnAnswer(base, ctx.r3.u32, false, ctx.r1.u32);
}
void RaceMenuOnBack(PPCContext& ctx, uint8_t* base) { rm::OnBack(base, ctx.r3.u32); }
void RaceMenuOnDialogClosed(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::OnDialogClosed(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Unload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuLeaveToHub(PPCContext& ctx, uint8_t* base) {
  rm::LeaveToHub(base, ctx.r3.u32, ctx.r4.u32);
}
void RaceMenuDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Destroy(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Start(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void RaceMenuBack(PPCContext& ctx, uint8_t* base) { rm::Back(base, ctx.r3.u32); }
void RaceMenuSetEventRange(PPCContext& ctx, uint8_t* base) {
  rm::SetEventRange(base, ctx.r3.u32);
}
void RaceMenuSetUpEvents(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::SetUpEvents(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void RaceMenuRefresh(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Refresh(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void RaceMenuTransition0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Transition0(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuTransition3(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Transition3(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuTransition5(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Transition5(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuAskConfirm(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::AskConfirm(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuTransition12(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Transition12(base, ctx.r1.u32, guest.calls());
}
void RaceMenuDebugKindPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rm::DebugKindPage(base, ctx.r1.u32);
}
void RaceMenuDebugEventPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rm::DebugEventPage(base, ctx.r3.u32, ctx.r1.u32);
}
void RaceMenuDebugVariantPage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rm::DebugVariantPage(base, ctx.r1.u32);
}
void RaceMenuAnimateVariantPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::AnimateVariantPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuKindPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::KindPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuAnimateKindPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::AnimateKindPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuVariantPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::VariantPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RaceMenuUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = rm::Update(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 dialog's Init.
void RaceMenuInit(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = rm::Init(base, machine, ctx.r1.u32 - rm::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define ITC_RACE_MENU(address, step)     \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_race_menu)) {   \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
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
// Back (from Draw and its piece sub_8218C5DC, recompiled).
ITC_RACE_MENU(82188A20, RaceMenuBack)
// SetEventRange sub_82189158 and SetUpEvents sub_821891C8: no adapters
// (removed, B7 loop, 2026-11-10: their only caller, Start, is native and
// calls their plain C++; the register steps above are the harness's).
// Refresh sub_8218BC18, the transitions sub_82189740, sub_82189C28,
// sub_8218A7F0, sub_8218ACE8, sub_8218AE48 and the debug pages
// sub_8218BB68, sub_82188A98, sub_82188C00: no adapters (removed, B7 loop,
// 2026-11-10: their callers, Update and Start, are native and call their
// plain C++; the register steps above are the harness's).
// AnimateVariantPage (from VariantPage, recompiled; Update calls its plain
// C++).
ITC_RACE_MENU(8218A928, RaceMenuAnimateVariantPage)
// The pages (from Draw and its piece sub_8218C5DC, recompiled; KindPage
// calls AnimateKindPage's plain C++).
ITC_RACE_MENU(8218AFD8, RaceMenuKindPage)
ITC_RACE_MENU(821898C0, RaceMenuAnimateKindPage)
ITC_RACE_MENU(8218B750, RaceMenuVariantPage)
// Init (from the type 2 mode's Init sub_82197278, recompiled).
ITC_RACE_MENU(8218CE10, RaceMenuInit)
