// The save screen natively: adapters in front of src/save_screen/
// (docs/functions/save-screen.md), with the register steps the harness
// shares (hooks/save_screen.h).

#include "hooks/save_screen.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "save_screen/save_screen.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_save_screen, true, "ITC",
              "Run the save screen (docs/functions/save-screen.md: the machines' set-ups "
              "Type2SaveScreen::Init sub_821A6E18, Type4SaveScreen::Init sub_821A7470 and "
              "Type10SaveScreen::Init sub_821A7AC8, and Type8SaveScreen::Init sub_821A87F0 (no "
              "adapter: ModeType8::Init's plain C++ calls it) (their step tables, "
              "StateMachine::Add's "
              "plain C++); the machine's handlers "
              "SaveScreen::OnEnter sub_821A2608, OpenMovie sub_821A1D10, WaitMovie sub_821A26F0, "
              "OpenKind0 to OpenKind7, Show sub_821A6608 (with ShowKind2 sub_821A1D38 and "
              "ShowKind7 sub_821A2138), Update sub_821A64A0, Draw sub_821A6AF0, Cancel "
              "sub_821A1D20, Leave sub_821A4C58 and OnLeft sub_821A1CA0 (the steps and pages "
              "called through the guest from their tables); the dialog steps "
              "SaveScreen::MoveSelection sub_821A5118 and CloseDialog sub_821A5038; the pages "
              "Page0 sub_821A28E0, Page1 sub_821A2AF8, Page2 sub_821A2C98, Page3 sub_821A2EE8 "
              "and the end pages Page4 sub_821A4D30, Page9 sub_821A4DE0, Page16 sub_821A4E60 and "
              "Page25 sub_821A4FD8; kind 1's Page6 sub_821A32C0 (with StoreSettings sub_821A24B0), "
              "Page7 sub_821A33E8 and Page8 sub_821A3728; the replays' Page12 sub_821A3A08, "
              "Page13 sub_821A3C60, Page14 sub_821A67E8 and Page15 sub_821A3DE8; kind 7's Page24 "
              "sub_821A4AC0; the menu list, MenuSound::Play, the UI movie calls and the "
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
  GuestCalls(lifted::Regs& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  ss::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
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
    lifted::CallFromContext(ctx, base, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

using Step = uint64_t (*)(uint8_t*, uint32_t, uint32_t, const ss::Calls&);
using Page = uint64_t (*)(uint8_t*, uint64_t, uint32_t, const ss::Calls&);

void RunStep(lifted::Regs& ctx, uint8_t* base, Step step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = step(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void RunPage(lifted::Regs& ctx, uint8_t* base, Page page) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = page(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace

ITC_REGS_STEP(SaveScreenMoveSelection) {
  RunStep(ctx, base, ss::MoveSelection);
}
ITC_REGS_STEP(SaveScreenCloseDialog) {
  RunStep(ctx, base, ss::CloseDialog);
}
ITC_REGS_STEP(SaveScreenPage0) { RunPage(ctx, base, ss::Page0); }
ITC_REGS_STEP(SaveScreenPage1) { RunPage(ctx, base, ss::Page1); }
ITC_REGS_STEP(SaveScreenPage2) { RunPage(ctx, base, ss::Page2); }
ITC_REGS_STEP(SaveScreenPage3) { RunPage(ctx, base, ss::Page3); }
ITC_REGS_STEP(SaveScreenPage4) { ctx.r3.u64 = ss::Page4(base, ctx.r3.u64); }
ITC_REGS_STEP(SaveScreenPage9) { ctx.r3.u64 = ss::Page9(base, ctx.r3.u64); }
ITC_REGS_STEP(SaveScreenPage16) {
  ctx.r3.u64 = ss::Page16(base, ctx.r3.u64);
}
ITC_REGS_STEP(SaveScreenPage25) {
  ctx.r3.u64 = ss::Page25(base, ctx.r3.u64);
}

ITC_REGS_STEP(SaveScreenPage6) { RunPage(ctx, base, ss::Page6); }
ITC_REGS_STEP(SaveScreenPage7) { RunPage(ctx, base, ss::Page7); }
ITC_REGS_STEP(SaveScreenPage8) { RunPage(ctx, base, ss::Page8); }
ITC_REGS_STEP(SaveScreenPage12) { RunPage(ctx, base, ss::Page12); }
ITC_REGS_STEP(SaveScreenPage13) { RunPage(ctx, base, ss::Page13); }
ITC_REGS_STEP(SaveScreenPage14) { RunPage(ctx, base, ss::Page14); }
ITC_REGS_STEP(SaveScreenPage15) { RunPage(ctx, base, ss::Page15); }
ITC_REGS_STEP(SaveScreenPage24) { RunPage(ctx, base, ss::Page24); }
ITC_REGS_STEP(SaveScreenStoreSettings) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::StoreSettings(base, ctx.r1.u32, guest.calls());
}

void SaveScreenInit(lifted::Regs& ctx, uint8_t* base, uint32_t address) {
  uint32_t frame_bytes = 0;
  for (const ss::InitInfo& info : ss::kInits) {
    if (info.address == address) {
      frame_bytes = info.frame;
    }
  }
  const uint64_t object_r3 = ctx.r3.u64;
  const ss::InitResult result =
      ss::RunInit(base, address, ctx.r3.u32, ctx.r1.u32 - frame_bytes);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  const uint64_t machine_r3 = object_r3 + (result.machine - ctx.r3.u32);
  ctx.r3.u64 = result.replaced ? result.r3 : machine_r3 + (result.r3 - result.machine);
}

ITC_REGS_STEP(SaveScreenOnEnter) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::OnEnter(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(SaveScreenOpenMovie) {
  // A tail call: UiMovies::Open runs on the caller's frame with its lr.
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::OpenMovie(base, ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
ITC_REGS_STEP(SaveScreenWaitMovie) {
  ctx.r3.u64 = ss::WaitMovie(base, ctx.r3.u64, ctx.r1.u32);
}
ITC_REGS_STEP(SaveScreenOpenKind0) { ss::OpenKind(base, ctx.r3.u32, 0); }
ITC_REGS_STEP(SaveScreenOpenKind1) { ss::OpenKind(base, ctx.r3.u32, 1); }
ITC_REGS_STEP(SaveScreenOpenKind2) { ss::OpenKind(base, ctx.r3.u32, 2); }
ITC_REGS_STEP(SaveScreenOpenKind3) { ss::OpenKind(base, ctx.r3.u32, 3); }
ITC_REGS_STEP(SaveScreenOpenKind4) { ss::OpenKind(base, ctx.r3.u32, 4); }
ITC_REGS_STEP(SaveScreenOpenKind5) { ss::OpenKind(base, ctx.r3.u32, 5); }
ITC_REGS_STEP(SaveScreenOpenKind6) { ss::OpenKind(base, ctx.r3.u32, 6); }
ITC_REGS_STEP(SaveScreenOpenKind7) { ss::OpenKind(base, ctx.r3.u32, 7); }
ITC_REGS_STEP(SaveScreenShow) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::Show(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(SaveScreenUpdate) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::Update(base, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(SaveScreenDraw) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::Draw(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(SaveScreenShowKind2) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::ShowKind2(base, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(SaveScreenShowKind7) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::ShowKind7(base, ctx.r1.u32, guest.calls());
}
void SaveScreenCancel(lifted::Regs&, uint8_t* base) { ss::Cancel(base); }
ITC_REGS_STEP(SaveScreenLeave) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::Leave(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(SaveScreenOnLeft) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ss::OnLeft(base, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_SAVE_SCREEN_INIT(address)                  \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_save_screen)) { \
    hooks::SaveScreenInit(ctx, base, 0x##address);     \
  }

#define ITC_SAVE_SCREEN(address, step)      \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_save_screen)) { \
    hooks::step(ctx, base);                 \
  }

// The machines' set-ups (from the type 2, 4 and 10 modes' Inits
// sub_82197278, sub_82224C70 and sub_82288940, recompiled).
ITC_SAVE_SCREEN_INIT(821A6E18)
ITC_SAVE_SCREEN_INIT(821A7470)
ITC_SAVE_SCREEN_INIT(821A7AC8)
// The machine's handlers: member function functors the machine calls (no
// direct callers).
ITC_SAVE_SCREEN(821A2608, SaveScreenOnEnter)
ITC_SAVE_SCREEN(821A1D10, SaveScreenOpenMovie)
ITC_SAVE_SCREEN(821A26F0, SaveScreenWaitMovie)
ITC_SAVE_SCREEN(821A2760, SaveScreenOpenKind0)
ITC_SAVE_SCREEN(821A2790, SaveScreenOpenKind1)
ITC_SAVE_SCREEN(821A2820, SaveScreenOpenKind2)
ITC_SAVE_SCREEN(821A27F0, SaveScreenOpenKind3)
ITC_SAVE_SCREEN(821A2850, SaveScreenOpenKind4)
ITC_SAVE_SCREEN(821A2880, SaveScreenOpenKind5)
ITC_SAVE_SCREEN(821A27C0, SaveScreenOpenKind6)
ITC_SAVE_SCREEN(821A28B0, SaveScreenOpenKind7)
ITC_SAVE_SCREEN(821A6608, SaveScreenShow)
ITC_SAVE_SCREEN(821A64A0, SaveScreenUpdate)
ITC_SAVE_SCREEN(821A6AF0, SaveScreenDraw)
ITC_SAVE_SCREEN(821A1D20, SaveScreenCancel)
ITC_SAVE_SCREEN(821A4C58, SaveScreenLeave)
ITC_SAVE_SCREEN(821A1CA0, SaveScreenOnLeft)
// No adapters for Show's case 0 sub_821A6650 (reached only from its jump
// table) and ShowKind2 sub_821A1D38 and ShowKind7 sub_821A2138 (only Show
// calls them); their steps above are the harness's.
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
ITC_SAVE_SCREEN(821A32C0, SaveScreenPage6)
ITC_SAVE_SCREEN(821A33E8, SaveScreenPage7)
ITC_SAVE_SCREEN(821A3728, SaveScreenPage8)
ITC_SAVE_SCREEN(821A3A08, SaveScreenPage12)
ITC_SAVE_SCREEN(821A3C60, SaveScreenPage13)
ITC_SAVE_SCREEN(821A67E8, SaveScreenPage14)
ITC_SAVE_SCREEN(821A3DE8, SaveScreenPage15)
ITC_SAVE_SCREEN(821A4AC0, SaveScreenPage24)
// No adapter for SaveScreen::StoreSettings sub_821A24B0: its only caller,
// Page6, is native (the flag switches it); its step above is the harness's.
