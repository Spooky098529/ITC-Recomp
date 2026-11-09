// The save screen natively: adapters in front of src/save_screen/
// (docs/functions/save-screen.md), with the register steps the harness
// shares (hooks/save_screen.h).

#include "hooks/save_screen.h"

#include "base/flags.h"
#include "save_screen/save_screen.h"

ITC_FLAG_BOOL(native_save_screen, true, "ITC",
              "Run the save screen (docs/functions/save-screen.md: the dialog steps "
              "SaveScreen::MoveSelection sub_821A5118 and CloseDialog sub_821A5038; the pages "
              "Page0 sub_821A28E0, Page1 sub_821A2AF8, Page2 sub_821A2C98, Page3 sub_821A2EE8 "
              "and the end pages Page4 sub_821A4D30, Page9 sub_821A4DE0, Page16 sub_821A4E60 and "
              "Page25 sub_821A4FD8; the menu list, MenuSound::Play, the UI movie calls and the "
              "save layer through their overrides) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=save_screen)");

namespace hooks {

namespace {

namespace ss = save_screen;

// The guest calls the plain C++ makes (save_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  ss::Calls calls() { return {this, &GuestCalls::Call}; }

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

using Step = uint64_t (*)(uint8_t*, uint32_t, uint32_t, const ss::Calls&);
using Page = uint64_t (*)(uint8_t*, uint64_t, uint32_t, const ss::Calls&);

void RunStep(PPCContext& ctx, uint8_t* base, Step step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = step(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RunPage(PPCContext& ctx, uint8_t* base, Page page) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = page(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace

void SaveScreenMoveSelection(PPCContext& ctx, uint8_t* base) {
  RunStep(ctx, base, ss::MoveSelection);
}
void SaveScreenCloseDialog(PPCContext& ctx, uint8_t* base) {
  RunStep(ctx, base, ss::CloseDialog);
}
void SaveScreenPage0(PPCContext& ctx, uint8_t* base) { RunPage(ctx, base, ss::Page0); }
void SaveScreenPage1(PPCContext& ctx, uint8_t* base) { RunPage(ctx, base, ss::Page1); }
void SaveScreenPage2(PPCContext& ctx, uint8_t* base) { RunPage(ctx, base, ss::Page2); }
void SaveScreenPage3(PPCContext& ctx, uint8_t* base) { RunPage(ctx, base, ss::Page3); }
void SaveScreenPage4(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = ss::Page4(base, ctx.r3.u64); }
void SaveScreenPage9(PPCContext& ctx, uint8_t* base) { ctx.r3.u64 = ss::Page9(base, ctx.r3.u64); }
void SaveScreenPage16(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::Page16(base, ctx.r3.u64);
}
void SaveScreenPage25(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ss::Page25(base, ctx.r3.u64);
}

}  // namespace hooks

#define ITC_SAVE_SCREEN(address, step)      \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_save_screen)) {    \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The dialog steps (SaveScreen::Update's table on its stack, no direct
// callers).
ITC_SAVE_SCREEN(821A5118, SaveScreenMoveSelection)
ITC_SAVE_SCREEN(821A5038, SaveScreenCloseDialog)
// The pages (SaveScreen::Draw's table on its stack, no direct callers).
ITC_SAVE_SCREEN(821A28E0, SaveScreenPage0)
ITC_SAVE_SCREEN(821A2AF8, SaveScreenPage1)
ITC_SAVE_SCREEN(821A2C98, SaveScreenPage2)
ITC_SAVE_SCREEN(821A2EE8, SaveScreenPage3)
ITC_SAVE_SCREEN(821A4D30, SaveScreenPage4)
ITC_SAVE_SCREEN(821A4DE0, SaveScreenPage9)
ITC_SAVE_SCREEN(821A4E60, SaveScreenPage16)
ITC_SAVE_SCREEN(821A4FD8, SaveScreenPage25)
