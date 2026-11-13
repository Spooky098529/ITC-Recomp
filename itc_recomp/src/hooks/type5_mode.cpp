// The type 5 mode natively: adapters in front of src/type5_mode/
// (docs/functions/type5-mode.md), with the register steps the harness
// shares (hooks/type5_mode.h).

#include "hooks/type5_mode.h"

#include "base/flags.h"
#include "type5_mode/type5_mode.h"

ITC_FLAG_BOOL(native_type5_mode, true, "ITC",
              "Run the type 5 mode (docs/functions/type5-mode.md: the parking area's front-end "
              "mode) and its dialog (the yes/no questions and notices over UI movies 16 to 19) "
              "natively instead of through the translated code: the dialog's steps "
              "OpenConfirmStep sub_822261E0, CloseConfirmStep sub_822262B8, CloseNoticeStep "
              "sub_82226370, OpenConfirm2Step sub_82226420, CloseConfirm2Step sub_822264B8, "
              "CloseNotice2Step sub_82226568, MoveCursorStep sub_82226618 and Step9 sub_822266D0 "
              "(MenuSound::Play and the UI movie calls through their overrides, UiMovies::Find "
              "its plain C++); its pages ConfirmPage sub_82225D10, NoticePage sub_82225F30 and "
              "Notice2Page sub_82226088 (Menu::Step, Menu::Pressed and MenuSound::Play through "
              "their overrides); Draw sub_822266F8 and Update sub_82226788 (the pages and steps "
              "through the guest from their tables, UiMovie::Run through its override); the "
              "handlers OnEnter sub_82225878 and OnLeft sub_822257A8 (VirtualAlloc and "
              "VirtualFree through their overrides), WaitForMovies sub_82226978 "
              "(UiMovies::IsLoaded's plain C++), OnInterrupt sub_82225860, OpenConfirm "
              "sub_82225A90 (UiMovie::ResetPose through its override), OpenNotice sub_82225980, "
              "OpenConfirm2 sub_82225B88 and OpenNotice2 sub_82225A08; Type5Dialog::Init "
              "sub_82226A10 (its step table, StateMachine::Add's plain C++); the mode's Init "
              "sub_8222FD68 (the dialog's Init as plain C++, the other callees through their "
              "overrides), FindShown sub_8222FD08, WaitForLoads sub_82230050, NextMode "
              "sub_82230458, LeaveScreen sub_82230260, WaitForShown sub_82230168, Leaving "
              "sub_82230328, Mode::Slot14Thunk sub_8222FC50, Interrupt sub_8222FED0, ForceExit "
              "sub_8222FF60 and Resume sub_8222FC38 (the same writes; checked by "
              "--run_harnesses=type5_mode)");

namespace hooks {

namespace {

namespace t5 = type5_mode;

// The guest calls the plain C++ makes (type5_mode::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  t5::Calls calls() { return {this, &GuestCalls::Call}; }

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

void Simple(PPCContext& ctx, uint8_t* base, t5::Simple which) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::SimpleStep(base, ctx.r3.u32, which, ctx.r1.u32, guest.calls());
}

// A delegate function: r3 the answer (returned as it came), r4 the mode.
template <void (*Function)(uint8_t*, uint32_t, uint32_t, uint32_t, const t5::Calls&)>
void Delegate(PPCContext& ctx, uint8_t* base) {
  const uint64_t answer = ctx.r3.u64;
  GuestCalls guest(ctx, base);
  Function(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
  ctx.r3.u64 = answer;
}

}  // namespace

void Type5DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::OpenConfirmStep(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogCloseConfirmStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::CloseConfirmStep(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogCloseNoticeStep(PPCContext& ctx, uint8_t* base) {
  Simple(ctx, base, t5::Simple::kCloseNotice);
}
void Type5DialogOpenConfirm2Step(PPCContext& ctx, uint8_t* base) {
  Simple(ctx, base, t5::Simple::kOpenConfirm2);
}
void Type5DialogCloseConfirm2Step(PPCContext& ctx, uint8_t* base) {
  Simple(ctx, base, t5::Simple::kCloseConfirm2);
}
void Type5DialogCloseNotice2Step(PPCContext& ctx, uint8_t* base) {
  Simple(ctx, base, t5::Simple::kCloseNotice2);
}
void Type5DialogMoveCursorStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::MoveCursorStep(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogStep9(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::Step9(base, ctx.r1.u32, guest.calls());
}
void Type5DialogConfirmPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::ConfirmPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogNoticePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::NoticePage(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
void Type5DialogNotice2Page(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::NoticePage(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
void Type5DialogDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void Type5DialogUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::Update(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogOnEnter(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::OnEnter(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogWaitForMovies(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t5::WaitForMovies(base, ctx.r3.u64, ctx.r1.u32);
}
void Type5DialogOnInterrupt(PPCContext&, uint8_t* base) { t5::OnInterrupt(base); }
void Type5DialogOpenConfirm(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::OpenConfirm(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void Type5DialogOpenNotice(PPCContext& ctx, uint8_t* base) {
  t5::OpenNotice(base, ctx.r3.u32, false);
}
void Type5DialogOpenConfirm2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t5::OpenConfirm2(base, ctx.r3.u32, ctx.r1.u32);
}
void Type5DialogOpenNotice2(PPCContext& ctx, uint8_t* base) {
  t5::OpenNotice(base, ctx.r3.u32, true);
}
void Type5DialogOnLeft(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::OnLeft(base, ctx.r1.u32, guest.calls());
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 dialog's Init.
void Type5DialogInit(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 =
      t5::DialogInit(base, machine, ctx.r1.u32 - t5::kDialogInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
void ModeType5Init(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::ModeInit(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void ModeType5FindShown(PPCContext& ctx, uint8_t* base) { t5::FindShown(base, ctx.r3.u32); }
void ModeType5WaitForLoads(PPCContext& ctx, uint8_t* base) {
  Delegate<t5::WaitForLoads>(ctx, base);
}
void ModeType5NextMode(PPCContext& ctx, uint8_t* base) { Delegate<t5::NextMode>(ctx, base); }
void ModeType5LeaveScreen(PPCContext& ctx, uint8_t* base) {
  Delegate<t5::LeaveScreen>(ctx, base);
}
void ModeType5WaitForShown(PPCContext& ctx, uint8_t* base) {
  Delegate<t5::WaitForShown>(ctx, base);
}
void ModeType5Leaving(PPCContext& ctx, uint8_t* base) { Delegate<t5::Leaving>(ctx, base); }
void ModeSlot14Thunk(PPCContext& ctx, uint8_t* base) {
  const uint32_t lr = uint32_t(ctx.lr);
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t5::Slot14Thunk(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, lr, guest.calls());
}
void ModeType5Interrupt(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = t5::Interrupt(base, ctx.r3.u32);
}
void ModeType5ForceExit(PPCContext& ctx, uint8_t* base) {
  t5::ForceExit(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}
void ModeType5Resume(PPCContext&, uint8_t* base) { t5::Resume(base); }

}  // namespace hooks

#define ITC_TYPE5_MODE(address, step)    \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_type5_mode)) {  \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

// The dialog's steps (from Update's table on its stack).
ITC_TYPE5_MODE(822261E0, Type5DialogOpenConfirmStep)
ITC_TYPE5_MODE(822262B8, Type5DialogCloseConfirmStep)
ITC_TYPE5_MODE(82226370, Type5DialogCloseNoticeStep)
ITC_TYPE5_MODE(82226420, Type5DialogOpenConfirm2Step)
ITC_TYPE5_MODE(822264B8, Type5DialogCloseConfirm2Step)
ITC_TYPE5_MODE(82226568, Type5DialogCloseNotice2Step)
ITC_TYPE5_MODE(82226618, Type5DialogMoveCursorStep)
ITC_TYPE5_MODE(822266D0, Type5DialogStep9)
// The dialog's pages (from Draw's table on its stack).
ITC_TYPE5_MODE(82225D10, Type5DialogConfirmPage)
ITC_TYPE5_MODE(82225F30, Type5DialogNoticePage)
ITC_TYPE5_MODE(82226088, Type5DialogNotice2Page)
// Draw and Update and the dialog's other handlers (functors the machine
// calls).
ITC_TYPE5_MODE(822266F8, Type5DialogDraw)
ITC_TYPE5_MODE(82226788, Type5DialogUpdate)
ITC_TYPE5_MODE(82225878, Type5DialogOnEnter)
ITC_TYPE5_MODE(82226978, Type5DialogWaitForMovies)
ITC_TYPE5_MODE(82225860, Type5DialogOnInterrupt)
ITC_TYPE5_MODE(82225A90, Type5DialogOpenConfirm)
ITC_TYPE5_MODE(82225980, Type5DialogOpenNotice)
ITC_TYPE5_MODE(82225B88, Type5DialogOpenConfirm2)
ITC_TYPE5_MODE(82225A08, Type5DialogOpenNotice2)
ITC_TYPE5_MODE(822257A8, Type5DialogOnLeft)
// The mode: Init (vtable 0x82006C24's slot 0, from Modes::Tick), the
// delegate functions (through the slot thunks and Mode::NextModeThunk), the
// slot 14 thunk (the delegate's function) and the +32 interface (vtable
// 0x82006C18). Type5Dialog::Init sub_82226A10 and FindShown sub_8222FD08: no
// adapters (only the mode's functions, native, call them; the register
// steps above are the harness's).
ITC_TYPE5_MODE(8222FD68, ModeType5Init)
ITC_TYPE5_MODE(82230050, ModeType5WaitForLoads)
ITC_TYPE5_MODE(82230458, ModeType5NextMode)
ITC_TYPE5_MODE(82230260, ModeType5LeaveScreen)
ITC_TYPE5_MODE(82230168, ModeType5WaitForShown)
ITC_TYPE5_MODE(82230328, ModeType5Leaving)
ITC_TYPE5_MODE(8222FC50, ModeSlot14Thunk)
ITC_TYPE5_MODE(8222FED0, ModeType5Interrupt)
ITC_TYPE5_MODE(8222FF60, ModeType5ForceExit)
ITC_TYPE5_MODE(8222FC38, ModeType5Resume)
