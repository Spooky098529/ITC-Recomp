// The type 2 dialog natively: adapters in front of src/type2_dialog/
// (docs/functions/type2-dialog.md), with the register steps the harness
// shares (hooks/type2_dialog.h).

#include "hooks/type2_dialog.h"

#include "base/flags.h"
#include "type2_dialog/type2_dialog.h"

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
              "through the guest from their tables, UiMovie::Run through its override) "
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
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  t2d::Calls calls() { return {this, &GuestCalls::Call}; }

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

void Type2DialogOpenConfirmStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OpenConfirmStep(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
void Type2DialogCloseConfirmStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::CloseConfirmStep(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
void Type2DialogOpenNoticeStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, false, false, ctx.r1.u32, guest.calls());
}
void Type2DialogCloseNoticeStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, false, true, ctx.r1.u32, guest.calls());
}
void Type2DialogOpenConfirm2Step(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::OpenConfirmStep(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
void Type2DialogCloseConfirm2Step(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::CloseConfirmStep(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
void Type2DialogOpenNotice2Step(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, true, false, ctx.r1.u32, guest.calls());
}
void Type2DialogCloseNotice2Step(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticeStep(base, ctx.r3.u32, true, true, ctx.r1.u32, guest.calls());
}
void Type2DialogMoveCursorStep(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::MoveCursorStep(base, ctx.r3.u32, false, ctx.r1.u32, guest.calls());
}
void Type2DialogMoveCursor2Step(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::MoveCursorStep(base, ctx.r3.u32, true, ctx.r1.u32, guest.calls());
}
void Type2DialogConfirmPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::ConfirmPage(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void Type2DialogNoticePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticePage(base, ctx.r3.u64, false, ctx.r1.u32, guest.calls());
}
void Type2DialogConfirm2Page(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::Confirm2Page(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void Type2DialogNotice2Page(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::NoticePage(base, ctx.r3.u64, true, ctx.r1.u32, guest.calls());
}
void Type2DialogDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void Type2DialogUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = t2d::Update(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_TYPE2_DIALOG(address, step)    \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_type2_dialog)) {  \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
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
