// The power graph screen natively: adapters in front of
// src/power_graph_screen/ (docs/functions/power-graph-screen.md), with the
// register steps the harness shares (hooks/power_graph_screen.h).

#include "hooks/power_graph_screen.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "guest.h"
#include "memory/handles.h"
#include "schema/power_graph_screen.h"
#include "power_graph_screen/power_graph_screen.h"
#include "lifted/sdk.h"

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
              "sub_82250348 (slot 3), Leave sub_82250900 (UiMovies::Delete and slot 1); Draw "
              "sub_82250A20 and Update sub_822513B8 (the pages and steps through the guest from "
              "their tables, UiMovie::Run through its override) "
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
  GuestCalls(lifted::Regs& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  pgs::Calls calls() { return {this, &GuestCalls::Call}; }

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

ITC_REGS_STEP(PowerGraphScreen6PostLeave) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82237A88);
}
ITC_REGS_STEP(PowerGraphScreen6PostClosing) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82237AD8);
}
ITC_REGS_STEP(PowerGraphScreen6PostClosed) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82237B28);
}
ITC_REGS_STEP(PowerGraphScreen7PostLeave) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82246FB8);
}
ITC_REGS_STEP(PowerGraphScreen7PostClosing) {
  pgs::SlotPost(base, ctx.r4.u32, 0x82247008);
}
ITC_REGS_STEP(PowerGraphScreenOnEnter) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::OnEnter(base, ctx.r3.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenWaitForLoad) {
  ctx.r3.u64 = pgs::WaitForLoad(base, ctx.r3.u64, ctx.r1.u32);
}
void PowerGraphScreenShown(lifted::Regs&, uint8_t* base) { pgs::Shown(base); }
ITC_REGS_STEP(PowerGraphScreenOpen) { pgs::Open(base, ctx.r3.u32); }
ITC_REGS_STEP(PowerGraphScreenLeave) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Leave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenOnLeft) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::OnLeft(base, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenStep0) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Step0(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenStep1) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Step1(base, ctx.r4.u32, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenSetTexts) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::SetTexts(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenSetParts) {
  // r4 the record's +60: the data's +68, a handle inside the host object
  // since E5 step 4 (its words through the view, bytes in guest order), or
  // a guest address (its bytes as they are). SetParts reads bytes 1 to 11.
  uint8_t parts[20] = {};
  const uint32_t at = ctx.r4.u32;
  if (memory::IsHandle(at)) {
    const schema::PowerGraphScreenData data(base, at - 68);
    const uint32_t words[5] = {data.parts_word_0(), data.parts_word_1(), data.parts_word_2(),
                               data.parts_word_3(), data.parts_word_4()};
    for (uint32_t i = 0; i < 20; i++) {
      parts[i] = uint8_t(words[i / 4] >> (24 - 8 * (i % 4)));
    }
  } else {
    for (uint32_t i = 0; i < 12; i++) {
      parts[i] = guest::LoadU8(base, at + i);
    }
  }
  ctx.r3.u64 = pgs::SetParts(base, parts, ctx.r1.u32);
}
ITC_REGS_STEP(PowerGraphScreenSetPartIcon) {
  ctx.r3.u64 = pgs::SetPartIcon(base, ctx.r4.u32, ctx.r1.u32);
}
ITC_REGS_STEP(PowerGraphScreenSetPartRow2) {
  ctx.r3.u64 = pgs::SetPartRow2(base, ctx.r4.u32, ctx.r1.u32);
}
ITC_REGS_STEP(PowerGraphScreenSetPartRow1) {
  ctx.r3.u64 = pgs::SetPartRow1(base, ctx.r4.u32, ctx.r1.u32);
}
ITC_REGS_STEP(PowerGraphScreenMenuPage) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::MenuPage(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenClosedPage) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::ClosedPage(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr),
                               guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenDraw) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Draw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, guest.calls());
}
ITC_REGS_STEP(PowerGraphScreenUpdate) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = pgs::Update(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

#define ITC_POWER_GRAPH_SCREEN(address, step)    \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_power_graph_screen)) { \
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
// Draw and Update (functors the machine calls).
ITC_POWER_GRAPH_SCREEN(82250A20, PowerGraphScreenDraw)
ITC_POWER_GRAPH_SCREEN(822513B8, PowerGraphScreenUpdate)
// The steps (from Update's table on its stack).
ITC_POWER_GRAPH_SCREEN(82251320, PowerGraphScreenStep0)
ITC_POWER_GRAPH_SCREEN(82250AA8, PowerGraphScreenStep1)
