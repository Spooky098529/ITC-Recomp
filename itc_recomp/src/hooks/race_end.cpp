// The race's end natively: adapters in front of src/race_end/
// (docs/functions/race-end.md, race-loading.md), with the register steps the
// harness shares (hooks/race_end.h). RaceMode::NextMode is the race mode's
// slot 6, reached through Mode::NextModeThunk (no direct callers), so its
// adapter stays; RaceMode::ModeAfterRace's only caller is NextMode.

#include "hooks/race_end.h"

#include "base/flags.h"
#include "hooks/archive.h"
#include "hooks/race_event.h"
#include "race_end/race_end.h"

ITC_FLAG_BOOL(native_race_end, true, "ITC",
              "Run the race's end (docs/functions/race-end.md: RaceMode::ModeAfterRace "
              "sub_82150E00, the mode a race goes to once it's over, with Progress::AfterRace, "
              "Progress::CheckUnlocks, Career::EventLocation, ScreenFades::FadeIn and "
              "Session::Free's plain C++; and race-loading.md's RaceMode::NextMode "
              "sub_82151EF0, the race mode's next-mode query, with LoadingScreen::Run's and "
              "sub_82293460's plain C++ and GameMessages::Dispatch through its override) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=race_end)");

namespace hooks {

namespace {

namespace re = race_end;

// The guest calls the plain C++ makes, through this context: r1, the
// registers given and lr set, the function through the function table (its
// override when it has one), r1 put back; in both shapes (race_hud_object's,
// r3 to r10 and f1; race_loading's, the first `set` of r3 to r10), and the
// race event's tests. The adapter's lr is put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base)
      : ctx_(ctx), base_(base), lr_(ctx.lr), event_(ctx, base) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  re::Calls calls() {
    return {{this, &GuestCalls::Call}, {this, &GuestCalls::CallSet, NativeArchiveQueue()},
            event_.get()};
  }

 private:
  uint64_t Run(uint32_t function, uint32_t r1, uint32_t lr) {
    const uint64_t stack = ctx_.r1.u64;
    ctx_.r1.u64 = r1;
    ctx_.lr = lr;
    ctx_.ctr.u64 = function;
    PPCContext& ctx = ctx_;
    uint8_t* base = base_;
    REX_CALL_INDIRECT_FUNC(function);
    ctx_.r1.u64 = stack;
    ctx_.fpscr.disableFlushMode();
    return ctx_.r3.u64;
  }
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[8], const double* f1,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCRegister* registers[8] = {&self.ctx_.r3, &self.ctx_.r4, &self.ctx_.r5, &self.ctx_.r6,
                                 &self.ctx_.r7, &self.ctx_.r8, &self.ctx_.r9, &self.ctx_.r10};
    for (int i = 0; i < 8; i++) {
      registers[i]->u64 = args[i];
    }
    if (f1 != nullptr) {
      self.ctx_.f1.f64 = *f1;
    }
    return self.Run(function, r1, lr);
  }
  static uint64_t CallSet(void* user, uint32_t function, const uint64_t args[8], int set,
                          uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCRegister* registers[8] = {&self.ctx_.r3, &self.ctx_.r4, &self.ctx_.r5, &self.ctx_.r6,
                                 &self.ctx_.r7, &self.ctx_.r8, &self.ctx_.r9, &self.ctx_.r10};
    for (int i = 0; i < set && i < 8; i++) {
      registers[i]->u64 = args[i];
    }
    return self.Run(function, r1, lr);
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
  RaceEventGuest event_;
};

}  // namespace

void RaceModeModeAfterRace(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  {
    GuestCalls guest(ctx, base);
    re::ModeAfterRace(base, uint32_t(r3), sp, guest.calls());
  }
  ctx.r3.u64 = r3;
}

void RaceModeNextMode(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t mode = ctx.r4.u32, sp = ctx.r1.u32;
  {
    GuestCalls guest(ctx, base);
    re::NextMode(base, uint32_t(r3), mode, sp, guest.calls());
  }
  ctx.r3.u64 = r3;
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag off.
#define ITC_RACE_END(address, step)       \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_race_end)) {     \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

// RaceMode::ModeAfterRace: its only caller, RaceMode::NextMode, calls its
// plain C++; the adapter for NextMode's fallback and the harness.
ITC_RACE_END(82150E00, RaceModeModeAfterRace)
ITC_RACE_END(82151EF0, RaceModeNextMode)
