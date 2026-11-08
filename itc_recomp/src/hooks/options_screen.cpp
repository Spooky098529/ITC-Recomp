// The options screen natively: adapters in front of src/options_screen/
// (docs/functions/options-screen.md), with the register steps the harness
// shares (hooks/options_screen.h).

#include "hooks/options_screen.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "options_screen/options_screen.h"

ITC_FLAG_BOOL(native_options_screen, true, "ITC",
              "Run the front end's options screen (docs/functions/options-screen.md: the "
              "controls' tables Controls::ButtonIndex sub_821BA950, ButtonCode sub_821BAA18, "
              "AnalogIndex sub_821BAAE0 and AnalogPicture sub_821BAD08; Options::IsButtonTaken "
              "sub_821BA7E0, IsAnalogTaken sub_821BAB58, StepButtons sub_821BA550 and StepAnalog "
              "sub_821BABD8; Settings::Reset sub_821B9DB8, Read sub_821BBF00 and Write "
              "sub_821BAE10; Options::OnEnter sub_821BB9A8 (with --native_heap), WaitForMovie "
              "sub_821BBBA0, OpenMovie sub_821B9F88, OnClosed sub_821BCE38, Close sub_821BCD28, "
              "Leave sub_821BCF08, ClosePage sub_821BD330 and Wait sub_821BD3C8; the steps OpenCategories "
              "sub_821BD468, ReturnToCategories sub_821BD5B0, EnterHud sub_821BD728, "
              "ChangeHudValue sub_821BD800, EnterSound sub_821BD908, EnterSliders sub_821BD9A0 "
              "and EnterButtons sub_821BDAD0; the sound "
              "options, the UI movie calls, UiMovies::Open and Delete and memcpy where its plain C++ declines through their overrides) "
              "natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=options_screen)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (options_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  options_screen::Calls calls() { return {this, &GuestCalls::Call}; }

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

void OptionsButtonIndex(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::ButtonIndex(ctx.r3.u32))));
}

void OptionsButtonCode(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = options_screen::ButtonCode(ctx.r3.u32);
}

void OptionsAnalogIndex(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::AnalogIndex(ctx.r3.u32))));
}

void OptionsAnalogPicture(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = uint64_t(int64_t(int32_t(options_screen::AnalogPicture(ctx.r3.u32))));
}

void OptionsIsButtonTaken(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::IsButtonTaken(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsIsAnalogTaken(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::IsAnalogTaken(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsStepButtons(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepButtons(base);
}

void OptionsStepAnalog(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::StepAnalog(base);
}

void OptionsSettingsReset(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::SettingsReset(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsSettingsRead(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SettingsRead(base, ctx.r1.u32, guest.calls());
}

void OptionsSettingsWrite(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::SettingsWrite(base, ctx.r1.u32, guest.calls());
}

void OptionsOnEnter(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::OnEnter(base, ctx.r3.u32, ctx.r1.u32);
}

void OptionsWaitForMovie(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::WaitForMovie(base, ctx.r3.u64, ctx.r1.u32);
}

void OptionsOpenMovie(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OpenMovie(ctx.r1.u32, lr, guest.calls());
}

void OptionsOnClosed(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OnClosed(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Leave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

void OptionsClosePage(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = options_screen::ClosePage(base, ctx.r3.u64);
}

void OptionsWait(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::Wait(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsOpenCategories(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::OpenCategories(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsReturnToCategories(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ReturnToCategories(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterHud(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterHud(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsChangeHudValue(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::ChangeHudValue(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterSound(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterSound(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterSliders(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterSliders(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void OptionsEnterButtons(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = options_screen::EnterButtons(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// No adapters (Native: their only callers in play call their plain C++)
// for Controls::ButtonIndex and AnalogIndex (Settings::Read), ButtonCode
// (Settings::Write), Settings::Reset (Options::OnEnter) and Settings::Write
// (Options::OnClosed); their register steps stay for the harness.

#define ITC_OPTIONS_SCREEN(address, step)   \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_options_screen)) { \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_OPTIONS_SCREEN(821BAD08, OptionsAnalogPicture)
ITC_OPTIONS_SCREEN(821BA7E0, OptionsIsButtonTaken)
ITC_OPTIONS_SCREEN(821BAB58, OptionsIsAnalogTaken)
ITC_OPTIONS_SCREEN(821BA550, OptionsStepButtons)
ITC_OPTIONS_SCREEN(821BABD8, OptionsStepAnalog)
ITC_OPTIONS_SCREEN(821BBF00, OptionsSettingsRead)
ITC_OPTIONS_SCREEN(821BBBA0, OptionsWaitForMovie)
ITC_OPTIONS_SCREEN(821B9F88, OptionsOpenMovie)
ITC_OPTIONS_SCREEN(821BCE38, OptionsOnClosed)
ITC_OPTIONS_SCREEN(821BCD28, OptionsClose)
ITC_OPTIONS_SCREEN(821BCF08, OptionsLeave)
ITC_OPTIONS_SCREEN(821BD330, OptionsClosePage)
ITC_OPTIONS_SCREEN(821BD3C8, OptionsWait)
ITC_OPTIONS_SCREEN(821BD468, OptionsOpenCategories)
ITC_OPTIONS_SCREEN(821BD5B0, OptionsReturnToCategories)
ITC_OPTIONS_SCREEN(821BD728, OptionsEnterHud)
ITC_OPTIONS_SCREEN(821BD800, OptionsChangeHudValue)
ITC_OPTIONS_SCREEN(821BD908, OptionsEnterSound)
ITC_OPTIONS_SCREEN(821BD9A0, OptionsEnterSliders)
ITC_OPTIONS_SCREEN(821BDAD0, OptionsEnterButtons)

// An adapter whose step allocates: the original where the native heap isn't
// active.
extern "C" REX_FUNC(sub_821BB9A8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_options_screen) || !hooks::NativeHeapActive()) {
    __imp__sub_821BB9A8(ctx, base);
    return;
  }
  hooks::OptionsOnEnter(ctx, base);
}
