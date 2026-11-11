// The type 2 mode natively: adapters in front of src/type2_mode/
// (docs/functions/type2-mode.md), with the register steps the harness shares
// (hooks/type2_mode.h).

#include "hooks/type2_mode.h"

#include "base/flags.h"
#include "type2_mode/type2_mode.h"

ITC_FLAG_BOOL(native_type2_mode, true, "ITC",
              "Run the type 2 front-end mode (docs/functions/type2-mode.md: the career's garage "
              "menus' mode over its block of 14 screen machines) natively instead of through the "
              "translated code: MachineOf sub_82196FC0 (with its case 0 sub_82197000), AllLoaded "
              "sub_82197130 and FindShown sub_82197218; the delegate functions TearDown "
              "sub_821970C0 (StandScene::PollTeardown through its override), LeaveScreen "
              "sub_82197C88, AfterLeaveScreen sub_821977E0, Restart sub_82197A30, Leaving "
              "sub_82198008, NextMode sub_82197D50 and WaitForLoads sub_82197B00 "
              "(GameMessages::Dispatch, LoadingScreen::Run and Finish, Sound::Play and the hub's "
              "sub_8218F620 through their overrides); the slot thunks Mode::Slot8Thunk "
              "sub_8223D4B0 to Slot13Thunk sub_8222FC80 (the type 2 mode's delegate functions "
              "their plain C++, others a guest call); the +32 interface's Interrupt sub_82197858 "
              "(Type2Dialog::TakesInput's plain C++) and ForceExit sub_82197930; ModeType2::Init "
              "sub_82197278 (the screen start, Strings::Load, Archive::WaitIdle, "
              "StandScene::Create, VirtualAlloc, the 14 machines' Inits, ScreenFades::FadeIn, "
              "Career::NextDay and LoadingScreen::Show through their overrides, SaveData::Of its "
              "plain C++) "
              "(the same writes; checked by --run_harnesses=type2_mode)");

namespace hooks {

namespace {

namespace t2m = type2_mode;

// The guest calls the plain C++ makes (type2_mode::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  t2m::Calls calls() { return {this, &GuestCalls::Call}; }

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

// A delegate function's step: r3 the answer, left as it is.
template <void (*Function)(uint8_t*, uint32_t, uint32_t, uint32_t, const t2m::Calls&)>
void Delegate(PPCContext& ctx, uint8_t* base) {
  const uint64_t answer = ctx.r3.u64;
  GuestCalls guest(ctx, base);
  Function(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
  ctx.r3.u64 = answer;
}

void Thunk(PPCContext& ctx, uint8_t* base, uint32_t slot) {
  const uint32_t lr = uint32_t(ctx.lr);
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2m::SlotThunk(base, slot, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, lr, guest.calls());
}

}  // namespace

void Type2ModeMachineOf(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = t2m::MachineOf(ctx.r3.u64, ctx.r4.u64);
}
void Type2ModeAllLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2m::AllLoaded(base, ctx.r3.u32);
}
void Type2ModeFindShown(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2m::FindShown(base, ctx.r3.u64, ctx.r1.u32);
}
void Type2ModeTearDown(PPCContext& ctx, uint8_t* base) { Delegate<t2m::TearDown>(ctx, base); }
void Type2ModeLeaveScreen(PPCContext& ctx, uint8_t* base) {
  Delegate<t2m::LeaveScreen>(ctx, base);
}
void Type2ModeAfterLeaveScreen(PPCContext& ctx, uint8_t* base) {
  t2m::AfterLeaveScreen(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void Type2ModeRestart(PPCContext& ctx, uint8_t* base) { Delegate<t2m::Restart>(ctx, base); }
void Type2ModeLeaving(PPCContext& ctx, uint8_t* base) { Delegate<t2m::Leaving>(ctx, base); }
void Type2ModeNextMode(PPCContext& ctx, uint8_t* base) { Delegate<t2m::NextMode>(ctx, base); }
void Type2ModeWaitForLoads(PPCContext& ctx, uint8_t* base) {
  Delegate<t2m::WaitForLoads>(ctx, base);
}
void ModeSlot8Thunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, 8); }
void ModeSlot9Thunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, 9); }
void ModeSlot10Thunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, 10); }
void ModeSlot11Thunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, 11); }
void ModeSlot12Thunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, 12); }
void ModeSlot13Thunk(PPCContext& ctx, uint8_t* base) { Thunk(ctx, base, 13); }
void Type2ModeInterrupt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t2m::Interrupt(base, ctx.r3.u32, ctx.r1.u32);
}
void Type2ModeForceExit(PPCContext& ctx, uint8_t* base) {
  t2m::ForceExit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void Type2ModeInit(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2m::Init(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_TYPE2_MODE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_type2_mode)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// The block's helpers (from the recompiled screens' code and the mode's
// functions).
ITC_TYPE2_MODE(82196FC0, Type2ModeMachineOf)
ITC_TYPE2_MODE(82197130, Type2ModeAllLoaded)
ITC_TYPE2_MODE(82197218, Type2ModeFindShown)
// The delegate functions (the mode's vtable slots 6 and 8 to 13, reached
// through the thunks and Mode::NextModeThunk).
ITC_TYPE2_MODE(821970C0, Type2ModeTearDown)
ITC_TYPE2_MODE(82197C88, Type2ModeLeaveScreen)
ITC_TYPE2_MODE(821977E0, Type2ModeAfterLeaveScreen)
ITC_TYPE2_MODE(82197A30, Type2ModeRestart)
ITC_TYPE2_MODE(82198008, Type2ModeLeaving)
ITC_TYPE2_MODE(82197D50, Type2ModeNextMode)
ITC_TYPE2_MODE(82197B00, Type2ModeWaitForLoads)
// The slot thunks (the delegates' functions, from ModeDelegate::Invoke).
ITC_TYPE2_MODE(8223D4B0, ModeSlot8Thunk)
ITC_TYPE2_MODE(8223D4C0, ModeSlot9Thunk)
ITC_TYPE2_MODE(8223D4A0, ModeSlot10Thunk)
ITC_TYPE2_MODE(8222FC60, ModeSlot11Thunk)
ITC_TYPE2_MODE(8222FC70, ModeSlot12Thunk)
ITC_TYPE2_MODE(8222FC80, ModeSlot13Thunk)
// Init (the mode's vtable 0x82006B9C slot 0, from Modes::Tick).
ITC_TYPE2_MODE(82197278, Type2ModeInit)
// The +32 interface (its vtable 0x82006B90; ModeType2::Resume sub_821970B8,
// a jump into the hub's recompiled sub_8218DE18, stays recompiled).
ITC_TYPE2_MODE(82197858, Type2ModeInterrupt)
ITC_TYPE2_MODE(82197930, Type2ModeForceExit)
