// The user info screen natively: adapters in front of src/user_info/
// (docs/functions/user-info.md), with the register steps the harness shares
// (hooks/user_info.h).

#include "hooks/user_info.h"

#include "base/flags.h"
#include "user_info/user_info.h"

ITC_FLAG_BOOL(native_user_info, true, "ITC",
              "Run the type 2 mode's user info screen (docs/functions/user-info.md: the "
              "player's record over UI movie 10) natively instead of through the translated "
              "code: Create sub_8220AA50 and Destroy sub_8220A9D8 (VirtualAlloc and VirtualFree "
              "through their overrides), OpenMovie sub_8220AA40 (UiMovies::Open through its "
              "override), WaitForLoad sub_8220B070 (UiMovies::IsLoaded's plain C++), Reenter "
              "sub_8220AB60, Close sub_8220ABB8 (UiMovie::FinishAll and UiMovies::Delete "
              "through their overrides), UpdatePage sub_8220B260 and Update sub_8220B2C0 (the "
              "page and the transitions through the guest from their tables, UiMovie::Run "
              "through its override), MenuPage sub_8220B0D8 with SetPlayTimeText sub_8220AFC0 "
              "(Menu::Step, Menu::Pressed, MenuSound::Play, PlayTime::Add and UiMovie::SetText "
              "through their overrides, SaveData::Of its plain C++), the transitions Opening "
              "sub_8220B1C8 with SetTexts sub_8220AD08 and Closing sub_8220AC60 (UiMovie::Play, "
              "Run, SetText and SetNumberText through their overrides; SetText's unset varargs "
              "0) and UserInfo::InitStates sub_8220B408 (its step table, StateMachine::Add's "
              "plain C++) (the same writes; checked by --run_harnesses=user_info)");

namespace hooks {

namespace {

namespace ui = user_info;

// The guest calls the plain C++ makes (user_info::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  ui::Calls calls() { return {this, &GuestCalls::Call}; }

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

void UserInfoCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void UserInfoOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::OpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void UserInfoWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = ui::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void UserInfoReenter(PPCContext& ctx, uint8_t* base) { ui::Reenter(base, ctx.r3.u32); }
void UserInfoClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void UserInfoDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::Destroy(base, ctx.r1.u32, guest.calls());
}
void UserInfoUpdatePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::UpdatePage(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void UserInfoUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::Update(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void UserInfoMenuPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::MenuPage(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void UserInfoSetPlayTimeText(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::SetPlayTimeText(base, ctx.r1.u32, guest.calls());
}
void UserInfoOpening(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::Opening(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void UserInfoClosing(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::Closing(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void UserInfoSetTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = ui::SetTexts(base, ctx.r1.u32, guest.calls());
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 screens' Inits.
void UserInfoInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = ui::InitStates(base, machine, ctx.r1.u32 - ui::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define ITC_USER_INFO(address, step)    \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_user_info)) {  \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

// The machine's handlers (member function functors the machine calls; no
// direct callers).
ITC_USER_INFO(8220AA50, UserInfoCreate)
ITC_USER_INFO(8220AA40, UserInfoOpenMovie)
ITC_USER_INFO(8220B070, UserInfoWaitForLoad)
ITC_USER_INFO(8220AB60, UserInfoReenter)
ITC_USER_INFO(8220ABB8, UserInfoClose)
ITC_USER_INFO(8220A9D8, UserInfoDestroy)
ITC_USER_INFO(8220B260, UserInfoUpdatePage)
ITC_USER_INFO(8220B2C0, UserInfoUpdate)
// The page (from UpdatePage's table on its stack) and the transitions (from
// Update's). No adapters for SetPlayTimeText sub_8220AFC0 (only MenuPage)
// and SetTexts sub_8220AD08 (only Opening): their steps above are the
// harness's.
ITC_USER_INFO(8220B0D8, UserInfoMenuPage)
ITC_USER_INFO(8220B1C8, UserInfoOpening)
ITC_USER_INFO(8220AC60, UserInfoClosing)
// InitStates (from the type 2 mode's Init sub_82197278).
ITC_USER_INFO(8220B408, UserInfoInitStates)
