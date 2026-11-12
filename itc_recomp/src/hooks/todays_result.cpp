// The type 9 mode (the career's end of the day) natively: adapters in front
// of src/todays_result/ (docs/functions/todays-result.md), with the register
// steps the harness shares (hooks/todays_result.h).

#include "hooks/todays_result.h"

#include "base/flags.h"
#include "todays_result/todays_result.h"

ITC_FLAG_BOOL(native_todays_result, true, "ITC",
              "Run the type 9 front-end mode (docs/functions/todays-result.md: the career's "
              "end of the day, today's result and today's event) natively instead of through "
              "the translated code: ModeType9::Init sub_8225D6D0 (TodaysResult::InitStates "
              "sub_8225B358 and TodaysEvent::InitStates sub_8225CF30 as plain C++ over "
              "StateMachine::Add's; the screen start, Strings::Load, VirtualAlloc, the loading "
              "screen and ScreenFades::FadeIn through their overrides), WaitForLoads "
              "sub_8225D918, NextMode sub_8225DC38, LeaveScreen sub_8225DB70, WaitForShown "
              "sub_8225DA68 and Leaving sub_8225DE78 (GameMessages::Dispatch through its "
              "override; FindShown sub_8225D658 their plain C++), IsLoaded sub_8225D6A8 and "
              "TodaysEvent::Kind sub_8225BA48 (checked by --run_harnesses=todays_result)");

namespace hooks {

namespace {

namespace tr = todays_result;

// The guest calls the plain C++ makes (todays_result::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  tr::Calls calls() { return {this, &GuestCalls::Call}; }

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
// 64-bit r3, as the title screen's Inits.
template <uint32_t (*InitStates)(uint8_t*, uint32_t, uint32_t, bool*)>
void InitStatesStep(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = InitStates(base, machine, ctx.r1.u32 - tr::kInitStatesFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace

bool TodaysResultNative() { return ITC_FLAG(native_todays_result); }

void ModeType9Init(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = tr::ModeInit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// The delegate functions return the answer (r3 as it came).
#define ITC_MODE_TYPE9_DELEGATE(name)                                       \
  void ModeType9##name(PPCContext& ctx, uint8_t* base) {                    \
    const uint64_t answer = ctx.r3.u64;                                     \
    {                                                                       \
      GuestCalls guest(ctx, base);                                          \
      tr::name(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());    \
    }                                                                       \
    ctx.r3.u64 = answer;                                                    \
  }
ITC_MODE_TYPE9_DELEGATE(WaitForLoads)
ITC_MODE_TYPE9_DELEGATE(NextMode)
ITC_MODE_TYPE9_DELEGATE(LeaveScreen)
ITC_MODE_TYPE9_DELEGATE(WaitForShown)
ITC_MODE_TYPE9_DELEGATE(Leaving)
#undef ITC_MODE_TYPE9_DELEGATE
void ModeType9IsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::IsLoaded(base, ctx.r3.u32);
}
void ModeType9FindShown(PPCContext& ctx, uint8_t* base) { tr::FindShown(base, ctx.r3.u32); }
void TodaysEventKind(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = tr::Kind(base, ctx.r1.u32);
}
void TodaysResultInitStates(PPCContext& ctx, uint8_t* base) {
  InitStatesStep<tr::ResultInitStates>(ctx, base);
}
void TodaysEventInitStates(PPCContext& ctx, uint8_t* base) {
  InitStatesStep<tr::EventInitStates>(ctx, base);
}

}  // namespace hooks

#define ITC_TODAYS_RESULT(address, step)   \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_todays_result)) { \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

// The mode: Init (vtable 0x82006CAC's slot 0, from Modes::Tick), the
// delegate functions (its slots 6, 8, 10, 11 and 13, through the slot
// thunks) and IsLoaded (the +32 vtable 0x82006CA0's slot 0). FindShown
// sub_8225D658 and the two InitStates: no adapters (only the mode's
// functions, native, call them; the register steps above are the
// harness's). Kind: today's event's Create and EndPage call it too.
ITC_TODAYS_RESULT(8225D6D0, ModeType9Init)
ITC_TODAYS_RESULT(8225D918, ModeType9WaitForLoads)
ITC_TODAYS_RESULT(8225DC38, ModeType9NextMode)
ITC_TODAYS_RESULT(8225DB70, ModeType9LeaveScreen)
ITC_TODAYS_RESULT(8225DA68, ModeType9WaitForShown)
ITC_TODAYS_RESULT(8225DE78, ModeType9Leaving)
ITC_TODAYS_RESULT(8225D6A8, ModeType9IsLoaded)
ITC_TODAYS_RESULT(8225BA48, TodaysEventKind)
