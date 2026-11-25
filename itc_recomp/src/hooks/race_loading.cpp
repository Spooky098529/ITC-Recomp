// The race's loading natively: adapters in front of src/race_loading/
// (docs/functions/race-loading.md), with the register steps the harness
// shares (hooks/race_loading.h). The handlers are reached through the race
// mode's state machine (functors), so their adapters stay; Init's only
// caller, RaceMode::Init sub_82151D20, is recompiled, and so are the loading
// screen's callers (RaceMode::Init and NextMode, the race's states, the
// front end's modes) and sub_82293460's (RaceMode::NextMode, the front
// end's).

#include "hooks/race_loading.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/archive.h"
#include "race_loading/race_loading.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_race_loading, true, "ITC",
              "Run the race's loading (docs/functions/race-loading.md: the race mode's "
              "+848 machine, CourseSetup::Init sub_821517D8 (the 11 transitions' words replayed "
              "in its frame, StateMachine::Add's plain C++), Start sub_82151408, Begin "
              "sub_82150DE0, OnCourseReady sub_82150DD0 and WaitDraw sub_821515A0 (the fades' "
              "and Archive::QueueLength's plain C++), WaitCourse sub_821514A0 and EndTick "
              "sub_821516F0 (Scenery::IsLoaded's, Text::IsLoaded's, DataTables::IsLoading's "
              "and CarScene::LoadsDone's plain C++, CarScene::Update a guest call); the loading screen's Show sub_822478D0 "
              "(with UiMovie::SetFrames sub_82290B30), Run sub_82247898 and Finish "
              "sub_82247A60 (the UI movie calls and GetLocalTime through their overrides); "
              "sub_82293460 (its callees through the function table; no adapter: RaceMode::NextMode "
              "calls it)) natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=race_loading)");

namespace hooks {

namespace {

namespace rl = race_loading;

// The guest calls the plain C++ makes (race_loading::Calls), through this
// context: r1, the first `set` of r3 to r10 and lr set, the function through
// the function table (its override when it has one); r1 put back. The
// adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), base_(), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  rl::Calls calls() { return {this, &GuestCalls::Call, NativeArchiveQueue()}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], int set,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    lifted::Temp* registers[8] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6,
                                 &ctx.r7, &ctx.r8, &ctx.r9, &ctx.r10};
    for (int i = 0; i < set && i < 8; i++) {
      registers[i]->u64 = args[i];
    }
    ctx.lr = lr;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

// Runs `body(calls)` with the guest calls through `ctx`, its r3 into r3.
template <typename Body>
void Step(lifted::Regs& ctx, Body body) {
  ctx.fpscr.disableFlushMode();
  uint64_t r3;
  {
    GuestCalls guest(ctx);
    r3 = body(guest.calls());
  }
  ctx.r3.u64 = r3;
}

}  // namespace

ITC_REGS_STEP(CourseSetupInit) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = rl::Init(machine, ctx.r1.u32 - rl::kInitFrame, &replaced);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

ITC_REGS_STEP(CourseSetupStart) {
  ctx.r3.u64 = rl::Start(ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CourseSetupBegin) {
  rl::Begin(ctx.r3.u32);
}

ITC_REGS_STEP(CourseSetupOnCourseReady) {
  rl::OnCourseReady(ctx.r3.u32);
}

ITC_REGS_STEP(CourseSetupWaitDraw) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t dispatcher = ctx.r4.u32, sp = ctx.r1.u32;
  Step(ctx,
       [&](const rl::Calls& c) { return rl::WaitDraw(r3, dispatcher, sp, c); });
}

ITC_REGS_STEP(CourseSetupWaitCourse) {
  const uint32_t machine = ctx.r3.u32, dispatcher = ctx.r4.u32, sp = ctx.r1.u32;
  Step(ctx, [&](const rl::Calls& c) {
    return rl::WaitCourse(machine, dispatcher, sp, c);
  });
}

ITC_REGS_STEP(CourseSetupEndTick) {
  const uint32_t machine = ctx.r3.u32, dispatcher = ctx.r4.u32, sp = ctx.r1.u32;
  Step(ctx, [&](const rl::Calls& c) {
    return rl::EndTick(machine, dispatcher, sp, c);
  });
}

ITC_REGS_STEP(LoadingScreenShow) {
  const uint64_t tip = ctx.r3.u64;
  const uint32_t which = ctx.r4.u32, frames = ctx.r5.u32, sp = ctx.r1.u32;
  Step(ctx,
       [&](const rl::Calls& c) { return rl::Show(tip, which, frames, sp, c); });
}

ITC_REGS_STEP(LoadingScreenRun) {
  const uint64_t which = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32, lr = uint32_t(ctx.lr);
  Step(ctx, [&](const rl::Calls& c) { return rl::Run(which, sp, lr, c); });
}

ITC_REGS_STEP(LoadingScreenFinish) {
  const uint64_t which = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32, lr = uint32_t(ctx.lr);
  Step(ctx, [&](const rl::Calls& c) { return rl::Finish(which, sp, lr, c); });
}

ITC_REGS_STEP(CheckSystemRequest) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, [&](const rl::Calls& c) { return rl::CheckSystemRequest(sp, c); });
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag off.
#define ITC_RACE_LOADING(address, step)      \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_loading)) { \
    hooks::step(ctx);                  \
  }

ITC_RACE_LOADING(821517D8, CourseSetupInit)
ITC_RACE_LOADING(82151408, CourseSetupStart)
ITC_RACE_LOADING(82150DE0, CourseSetupBegin)
ITC_RACE_LOADING(82150DD0, CourseSetupOnCourseReady)
ITC_RACE_LOADING(821515A0, CourseSetupWaitDraw)
ITC_RACE_LOADING(821514A0, CourseSetupWaitCourse)
ITC_RACE_LOADING(821516F0, CourseSetupEndTick)
ITC_RACE_LOADING(822478D0, LoadingScreenShow)
ITC_RACE_LOADING(82247898, LoadingScreenRun)
ITC_RACE_LOADING(82247A60, LoadingScreenFinish)
// sub_82293460 has no adapter (removed, B7 loop, 2026-11-12): its only
// caller in play, RaceMode::NextMode (--native_race_end), calls its plain
// C++; the front end's callers, never seen in play, reach the original. Its
// step is the harness's.
