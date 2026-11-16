// The type 2 dialog natively: adapters in front of src/type2_dialog/
// (docs/functions/type2-dialog.md), with the register steps the harness
// shares (hooks/type2_dialog.h).

#include "hooks/type2_dialog.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "type2_dialog/type2_dialog.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_type2_dialog, true, "ITC",
              "Run the type 2 mode's dialog (docs/functions/type2-dialog.md: the yes/no "
              "questions and notices over UI movies 16 to 19) natively instead of through the "
              "translated code: the steps OpenConfirmStep sub_82187728, CloseConfirmStep "
              "sub_821877F8, OpenNoticeStep sub_821878B0, CloseNoticeStep sub_82187948, "
              "OpenConfirm2Step sub_821879F8, CloseConfirm2Step sub_82187AC8, OpenNotice2Step "
              "sub_82187B80, CloseNotice2Step sub_82187C18, MoveCursorStep sub_82187CC8 and "
              "MoveCursor2Step sub_82187D80 (MenuSound::Play and the UI movie calls through "
              "their overrides, UiMovies::Find its plain C++); the pages ConfirmPage "
              "sub_82187160, NoticePage sub_82187508, Confirm2Page sub_82187330 and Notice2Page "
              "sub_82187618 (Menu::Step, Menu::Pressed and MenuSound::Play through their "
              "overrides); Draw sub_82187E50 and Update sub_82187EC8 (the pages and steps "
              "through the guest from their tables, UiMovie::Run through its override); the "
              "machine's handlers OnEnter sub_821869B8 and "
              "OnLeft sub_82186428 (VirtualAlloc and VirtualFree through their overrides), "
              "WaitForMovies sub_821880D0 (UiMovies::IsLoaded's plain C++), Shown sub_82186490, "
              "Wait sub_82186AD8, Cancel sub_82186E40, OpenConfirm sub_82186C30 "
              "(UiMovie::ResetPose through its override), OpenNotice sub_82186AF0, OpenConfirm2 "
              "sub_82186D40 and OpenNotice2 sub_82186B90; TakesInput sub_821864A0; "
              "Type2Dialog::Init sub_82188178 (its step table, StateMachine::Add's plain C++) "
              "(the same writes; checked by --run_harnesses=type2_dialog)");

namespace hooks {

namespace {

namespace t2d = type2_dialog;

// The guest calls the plain C++ makes (type2_dialog::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  t2d::Calls calls() { return {this, &GuestCalls::Call}; }

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

}  // namespace

ITC_REGS_STEP(Type2DialogOpenConfirmStep) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OpenConfirmStep(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogCloseConfirmStep) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::CloseConfirmStep(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogOpenNoticeStep) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, false, false, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogCloseNoticeStep) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, false, true, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogOpenConfirm2Step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OpenConfirmStep(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogCloseConfirm2Step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::CloseConfirmStep(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogOpenNotice2Step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, true, false, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogCloseNotice2Step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, true, true, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogMoveCursorStep) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::MoveCursorStep(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogMoveCursor2Step) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::MoveCursorStep(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogConfirmPage) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::ConfirmPage(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogNoticePage) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticePage(base, ctx.r3.u64, false, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogConfirm2Page) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::Confirm2Page(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogNotice2Page) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticePage(base, ctx.r3.u64, true, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogDraw) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogUpdate) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::Update(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogOnEnter) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OnEnter(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogWaitForMovies) {
  ctx.r3.u64 = t2d::WaitForMovies(base, ctx.r3.u64, ctx.r1.u32);
}
void Type2DialogShown(lifted::Regs&, uint8_t* base) { t2d::Shown(base); }
ITC_REGS_STEP(Type2DialogWait) { t2d::Wait(base, ctx.r3.u32); }
ITC_REGS_STEP(Type2DialogCancel) {
  ctx.r3.u64 = t2d::Cancel(base, ctx.r3.u64, ctx.r4.u32);
}
ITC_REGS_STEP(Type2DialogOpenConfirm) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OpenConfirm(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(Type2DialogOpenNotice) {
  t2d::OpenNotice(base, ctx.r3.u32, false);
}
ITC_REGS_STEP(Type2DialogOpenConfirm2) {
  ctx.r3.u64 = t2d::OpenConfirm2(base, ctx.r3.u32, ctx.r1.u32);
}
ITC_REGS_STEP(Type2DialogOpenNotice2) {
  t2d::OpenNotice(base, ctx.r3.u32, true);
}
ITC_REGS_STEP(Type2DialogOnLeft) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OnLeft(base, ctx.r1.u32, guest.calls());
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 screens' Inits.
ITC_REGS_STEP(Type2DialogInit) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = t2d::Init(base, machine, ctx.r1.u32 - t2d::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}
ITC_REGS_STEP(Type2DialogTakesInput) {
  ctx.r3.u64 = t2d::TakesInput(base);
}

}  // namespace hooks

#define ITC_TYPE2_DIALOG(address, step)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_type2_dialog)) { \
    hooks::step(ctx, base);                \
  }

// The steps (from Update's table on its stack).
ITC_TYPE2_DIALOG(82187728, Type2DialogOpenConfirmStep)
ITC_TYPE2_DIALOG(821877F8, Type2DialogCloseConfirmStep)
ITC_TYPE2_DIALOG(821878B0, Type2DialogOpenNoticeStep)
ITC_TYPE2_DIALOG(82187948, Type2DialogCloseNoticeStep)
ITC_TYPE2_DIALOG(821879F8, Type2DialogOpenConfirm2Step)
ITC_TYPE2_DIALOG(82187AC8, Type2DialogCloseConfirm2Step)
ITC_TYPE2_DIALOG(82187B80, Type2DialogOpenNotice2Step)
ITC_TYPE2_DIALOG(82187C18, Type2DialogCloseNotice2Step)
ITC_TYPE2_DIALOG(82187CC8, Type2DialogMoveCursorStep)
ITC_TYPE2_DIALOG(82187D80, Type2DialogMoveCursor2Step)
// The pages (from Draw's table on its stack).
ITC_TYPE2_DIALOG(82187160, Type2DialogConfirmPage)
ITC_TYPE2_DIALOG(82187508, Type2DialogNoticePage)
ITC_TYPE2_DIALOG(82187330, Type2DialogConfirm2Page)
ITC_TYPE2_DIALOG(82187618, Type2DialogNotice2Page)
// Draw and Update (functors the machine calls).
ITC_TYPE2_DIALOG(82187E50, Type2DialogDraw)
ITC_TYPE2_DIALOG(82187EC8, Type2DialogUpdate)
// The machine's other handlers (functors).
ITC_TYPE2_DIALOG(821869B8, Type2DialogOnEnter)
ITC_TYPE2_DIALOG(821880D0, Type2DialogWaitForMovies)
ITC_TYPE2_DIALOG(82186490, Type2DialogShown)
ITC_TYPE2_DIALOG(82186AD8, Type2DialogWait)
ITC_TYPE2_DIALOG(82186E40, Type2DialogCancel)
ITC_TYPE2_DIALOG(82186C30, Type2DialogOpenConfirm)
ITC_TYPE2_DIALOG(82186AF0, Type2DialogOpenNotice)
ITC_TYPE2_DIALOG(82186D40, Type2DialogOpenConfirm2)
ITC_TYPE2_DIALOG(82186B90, Type2DialogOpenNotice2)
ITC_TYPE2_DIALOG(82186428, Type2DialogOnLeft)
// Init (from the type 2 mode's Init sub_82197278, recompiled).
ITC_TYPE2_DIALOG(82188178, Type2DialogInit)
// TakesInput (from the type 2 mode's sub_82197858, recompiled).
ITC_TYPE2_DIALOG(821864A0, Type2DialogTakesInput)
