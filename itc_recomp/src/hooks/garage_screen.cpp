// The garage screen natively: adapters in front of src/garage_screen/
// (docs/functions/garage-screen.md), with the register steps the harness
// shares (hooks/garage_screen.h).

#include "hooks/garage_screen.h"

#include "base/flags.h"
#include "garage_screen/garage_screen.h"

ITC_FLAG_BOOL(native_garage_screen, true, "ITC",
              "Run the type 2 mode's garage screen (docs/functions/garage-screen.md: the "
              "player's garage cars over the stand, UI movie 5) natively instead of through the "
              "translated code: the helpers SetCarRow sub_82193180, FillRows sub_82194CD8, "
              "SetPrompts sub_821939A0, SetHelpLine sub_82193A90 and SetUpLists sub_821956C0 "
              "(the UI movie and menu list calls through their overrides, SetScrollArrows the "
              "original); the transitions Transition0 sub_82195BA0, Transition1 sub_82192D60 "
              "and Transition3 sub_82192E20; the pages CarListPage sub_82194078 (ReloadStandCar "
              "the original) and LeavePage sub_82192C18; UpdatePage sub_82194EA8 and Update "
              "sub_82196230 (the pages and transitions through the guest from their tables, the "
              "stand's calls through their overrides, PutCarOnStand and the busy marker's three "
              "the originals); the machine's handlers Create sub_82191C58 and Destroy "
              "sub_821915B0 (VirtualAlloc and VirtualFree through their overrides), OpenMovie "
              "sub_82191618, WaitForLoad sub_82191D70, Start sub_82193DC0 with ReadSave "
              "sub_82191BB0 and SetHeader sub_82192C80, Unload sub_821927E8 and Close "
              "sub_82192850; InitStates sub_82196520 (its step table, StateMachine::Add's plain C++) "
              "(the same writes; checked by "
              "--run_harnesses=garage_screen)");

namespace hooks {

namespace {

namespace gs = garage_screen;

// The guest calls the plain C++ makes (garage_screen::Calls), through this
// context: r1, r3 to r10, f1 when given and lr set, the function run through
// the function table (its override when it has one), r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  gs::Calls calls() { return {this, &GuestCalls::Call}; }

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

void GarageScreenSetCarRow(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetCarRow(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenFillRows(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::FillRows(base, ctx.r1.u32, guest.calls());
}
void GarageScreenSetPrompts(PPCContext& ctx, uint8_t* base) {
  const uint32_t args[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                            ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  ctx.r3.u64 = gs::SetPrompts(base, args, ctx.r1.u32);
}
void GarageScreenSetHelpLine(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetHelpLine(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void GarageScreenSetUpLists(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetUpLists(base, ctx.r1.u32, guest.calls());
}
void GarageScreenTransition0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Transition0(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenTransition1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Transition1(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenTransition3(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Transition3(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}

void GarageScreenCarListPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::CarListPage(base, ctx.r1.u32, guest.calls());
}
void GarageScreenLeavePage(PPCContext& ctx, uint8_t* base) {
  gs::LeavePage(base, ctx.r3.u32);
}
void GarageScreenUpdatePage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 =
      gs::UpdatePage(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32, guest.calls());
}
void GarageScreenUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Update(base, ctx.r1.u32, guest.calls());
}
void GarageScreenCreate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Create(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
// A tail call: Open runs from the adapter's frame with its lr.
void GarageScreenOpenMovie(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::OpenMovie(ctx.r1.u32, uint32_t(ctx.lr), guest.calls());
}
void GarageScreenWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gs::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void GarageScreenReadSave(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gs::ReadSave(base, ctx.r1.u32);
}
void GarageScreenSetHeader(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::SetHeader(base, ctx.r1.u32, guest.calls());
}
void GarageScreenStart(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Start(base, ctx.r1.u32, guest.calls());
}
void GarageScreenUnload(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Unload(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenClose(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Close(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void GarageScreenDestroy(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = gs::Destroy(base, ctx.r1.u32, guest.calls());
}
// StateMachine::Add's adapter's r3: a new transition's address from the
// machine's 64-bit r3, as the type 2 dialog's Init.
void GarageScreenInitStates(PPCContext& ctx, uint8_t* base) {
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = gs::InitStates(base, machine, ctx.r1.u32 - gs::kInitFrame, &replaced);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define ITC_GARAGE_SCREEN(address, step)    \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_garage_screen)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The movie 5 helpers' adapters removed (B7 loop, 2026-11-09): their callers
// in play are native and call their plain C++; the recompiled callers never
// seen in play run the originals. Their register steps stay for the harness.

// The transitions seen in play (from Update's table on its stack).
ITC_GARAGE_SCREEN(82195BA0, GarageScreenTransition0)
ITC_GARAGE_SCREEN(82192D60, GarageScreenTransition1)
ITC_GARAGE_SCREEN(82192E20, GarageScreenTransition3)
// The pages seen in play (from UpdatePage's table on its stack).
ITC_GARAGE_SCREEN(82194078, GarageScreenCarListPage)
ITC_GARAGE_SCREEN(82192C18, GarageScreenLeavePage)
// UpdatePage and Update (functors the machine calls).
ITC_GARAGE_SCREEN(82194EA8, GarageScreenUpdatePage)
ITC_GARAGE_SCREEN(82196230, GarageScreenUpdate)
// The machine's handlers (functors); Start's two helpers' adapters removed
// (Start's plain C++ calls theirs).
ITC_GARAGE_SCREEN(82191C58, GarageScreenCreate)
ITC_GARAGE_SCREEN(82191618, GarageScreenOpenMovie)
ITC_GARAGE_SCREEN(82191D70, GarageScreenWaitForLoad)
ITC_GARAGE_SCREEN(82193DC0, GarageScreenStart)
ITC_GARAGE_SCREEN(821927E8, GarageScreenUnload)
ITC_GARAGE_SCREEN(82192850, GarageScreenClose)
ITC_GARAGE_SCREEN(821915B0, GarageScreenDestroy)
// InitStates (from the type 2 mode's Init sub_82197278, recompiled).
ITC_GARAGE_SCREEN(82196520, GarageScreenInitStates)
