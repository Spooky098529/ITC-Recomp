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
              "sub_82189158; RaceMenu::Init sub_8218CE10 (its step table, StateMachine::Add's "
              "plain C++) (the same writes; checked by --run_harnesses=race_menu)");

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
void RaceMenuBack(PPCContext& ctx, uint8_t* base) { rm::Back(base, ctx.r3.u32); }
void RaceMenuSetEventRange(PPCContext& ctx, uint8_t* base) {
  rm::SetEventRange(base, ctx.r3.u32);
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
// Back (from Draw and its piece sub_8218C5DC) and SetEventRange (from
// Start), recompiled.
ITC_RACE_MENU(82188A20, RaceMenuBack)
ITC_RACE_MENU(82189158, RaceMenuSetEventRange)
// Init (from the type 2 mode's Init sub_82197278, recompiled).
ITC_RACE_MENU(8218CE10, RaceMenuInit)
