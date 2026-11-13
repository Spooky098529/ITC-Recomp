// The power graph screen natively: adapters in front of
// src/power_graph_screen/ (docs/functions/power-graph-screen.md), with the
// register steps the harness shares (hooks/power_graph_screen.h).

#include "hooks/power_graph_screen.h"

#include "base/flags.h"
#include "power_graph_screen/power_graph_screen.h"

ITC_FLAG_BOOL(native_power_graph_screen, true, "ITC",
              "Run the type 6 and 7 modes' power graph screen (docs/functions/"
              "power-graph-screen.md: UI movie 9 over the picked car, its specs as texts) "
              "natively instead of through the translated code: the members' vtable slots "
              "PowerGraphScreen6::PostLeave sub_82237A88, PostClosing sub_82237AD8, PostClosed "
              "sub_82237B28, PowerGraphScreen7::PostLeave sub_82246FB8 and PostClosing "
              "sub_82247008; the machine's handlers OnEnter sub_82250778 and OnLeft "
              "sub_822502C0 (VirtualAlloc and VirtualFree through their overrides), "
              "WaitForLoad sub_82250D70 (UiMovies::IsLoaded's plain C++), Shown sub_82250338, "
              "Open sub_82250890; the steps Step0 sub_82251320 (with SetTexts sub_82250DD8, "
              "SetParts sub_82250CD0, SetPartIcon sub_82250B48, SetPartRow2 sub_82250BA0 and "
              "SetPartRow1 sub_82250C40; UiMovie::SetText, Play and Run through their "
              "overrides, UiMovies::Find and UiMovie::FindNode their plain C++) and Step1 "
              "sub_82250AA8; the pages MenuPage sub_82250948 (Menu::Step, Menu::Pressed, "
              "MenuSound::Play and the member's slot 2 through their overrides) and ClosedPage "
              "sub_82250348 (slot 3), Leave sub_82250900 (UiMovies::Delete and slot 1) "
              "(the same writes; checked by "
              "--run_harnesses=power_graph_screen)");

namespace hooks {

namespace {

namespace pgs = power_graph_screen;

// The guest calls the plain C++ makes (power_graph_screen::Calls), through
// this context: r1, r3 to r10, f1 when given and lr set, the function run
// through the function table (its override when it has one), r1 put back.
// The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  pgs::Calls calls() { return {this, &GuestCalls::Call}; }

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

void PowerGraphScreen6PostLeave(PPCContext& ctx, uint8_t* base) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82237A88);
}
void PowerGraphScreen6PostClosing(PPCContext& ctx, uint8_t* base) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82237AD8);
}
void PowerGraphScreen6PostClosed(PPCContext& ctx, uint8_t* base) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82237B28);
}
void PowerGraphScreen7PostLeave(PPCContext& ctx, uint8_t* base) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82246FB8);
}
void PowerGraphScreen7PostClosing(PPCContext& ctx, uint8_t* base) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82247008);
}
void PowerGraphScreenOnEnter(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::OnEnter(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenWaitForLoad(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pgs::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void PowerGraphScreenShown(PPCContext&, uint8_t* base) { pgs::Shown(base); }
void PowerGraphScreenOpen(PPCContext& ctx, uint8_t* base) { pgs::Open(base, ctx.r3.u32); }
void PowerGraphScreenLeave(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Leave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenOnLeft(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::OnLeft(base, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenStep0(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Step0(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenStep1(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Step1(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenSetTexts(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::SetTexts(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenSetParts(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pgs::SetParts(base, ctx.r4.u32, ctx.r1.u32);
}
void PowerGraphScreenSetPartIcon(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pgs::SetPartIcon(base, ctx.r4.u32, ctx.r1.u32);
}
void PowerGraphScreenSetPartRow2(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pgs::SetPartRow2(base, ctx.r4.u32, ctx.r1.u32);
}
void PowerGraphScreenSetPartRow1(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = pgs::SetPartRow1(base, ctx.r4.u32, ctx.r1.u32);
}
void PowerGraphScreenMenuPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::MenuPage(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenClosedPage(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::ClosedPage(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr),
                               guest.calls());
}
void PowerGraphScreenDraw(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
void PowerGraphScreenUpdate(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Update(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_POWER_GRAPH_SCREEN(address, step)    \
  extern "C" REX_FUNC(sub_##address) {           \
    REX_FUNC_PROLOGUE();                         \
    if (!ITC_FLAG(native_power_graph_screen)) {  \
      __imp__sub_##address(ctx, base);           \
      return;                                    \
    }                                            \
    hooks::step(ctx, base);                      \
  }

// The members' vtable slots (vtables 0x82010A68 and 0x82010FD0).
ITC_POWER_GRAPH_SCREEN(82237A88, PowerGraphScreen6PostLeave)
ITC_POWER_GRAPH_SCREEN(82237AD8, PowerGraphScreen6PostClosing)
ITC_POWER_GRAPH_SCREEN(82237B28, PowerGraphScreen6PostClosed)
ITC_POWER_GRAPH_SCREEN(82246FB8, PowerGraphScreen7PostLeave)
ITC_POWER_GRAPH_SCREEN(82247008, PowerGraphScreen7PostClosing)
// The machine's handlers (functors the machine calls).
ITC_POWER_GRAPH_SCREEN(82250778, PowerGraphScreenOnEnter)
ITC_POWER_GRAPH_SCREEN(82250D70, PowerGraphScreenWaitForLoad)
ITC_POWER_GRAPH_SCREEN(82250338, PowerGraphScreenShown)
ITC_POWER_GRAPH_SCREEN(82250890, PowerGraphScreenOpen)
ITC_POWER_GRAPH_SCREEN(822502C0, PowerGraphScreenOnLeft)
ITC_POWER_GRAPH_SCREEN(82250900, PowerGraphScreenLeave)
// The pages (from Draw's table on its stack).
ITC_POWER_GRAPH_SCREEN(82250948, PowerGraphScreenMenuPage)
ITC_POWER_GRAPH_SCREEN(82250348, PowerGraphScreenClosedPage)
// The steps (from Update's table on its stack).
ITC_POWER_GRAPH_SCREEN(82251320, PowerGraphScreenStep0)
ITC_POWER_GRAPH_SCREEN(82250AA8, PowerGraphScreenStep1)
