// The achievements natively: adapters in front of src/achievements/
// (docs/functions/achievements.md), with the register steps the harness
// shares (hooks/achievements.h). AllBeaten and BeatsCourseTime have no
// adapters: only native Check calls them. Achievements::Check's callers
// (RaceStates::OnRaceEnd, the network battle screen) and Time::Less's other
// callers (Time::Clamp, sub_82105EA0) are recompiled, so the adapters stay.

#include "hooks/achievements.h"

#include "achievements/achievements.h"
#include "base/flags.h"
#include "hooks/app_start_up.h"

ITC_FLAG_BOOL(native_achievements, true, "ITC",
              "Run the achievements (docs/functions/achievements.md: Achievements::Check "
              "sub_82290788 with AllBeaten sub_82290698, BeatsCourseTime sub_82290710 and "
              "Time::Less sub_820FF708; Achievements::Write sub_82290A30, which still calls "
              "XUserWriteAchievements sub_82294E58 through the function table, so the SDK "
              "records unlocks) natively (AllBeaten and BeatsCourseTime only from native Check; the "
              "flag switches Check) instead of through the translated code (the same "
              "words; checked by --run_harnesses=achievements)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (achievements::Calls), through this
// context: r1, r3 to r5 and lr set, the function run through the function
// table (its override when it has one), r1 put back. The adapter's lr is
// put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  achievements::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[3], uint32_t r1,
                       uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = args[0];
    ctx.r4.u64 = args[1];
    ctx.r5.u64 = args[2];
    ctx.lr = lr;
    ctx.ctr.u64 = function;
    REX_CALL_INDIRECT_FUNC(function);
    ctx.r1.u64 = stack;
    return ctx.r3.u64;
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

void TimeLessSteps(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = achievements::TimeLess(guest::LoadU32(base, ctx.r3.u32),
                                      guest::LoadU32(base, ctx.r4.u32));
}

void AchievementsCheckSteps(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = achievements::Check(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void AchievementsWriteSteps(PPCContext& ctx, uint8_t* base) {
  GuestCalls guest(ctx, base);
  ctx.r3.u64 = achievements::Write(base, ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_ACHIEVEMENTS(address, step)   \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_achievements)) { \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_ACHIEVEMENTS(820FF708, TimeLessSteps)
ITC_ACHIEVEMENTS(82290788, AchievementsCheckSteps)
ITC_ACHIEVEMENTS(82290A30, AchievementsWriteSteps)

// XUserWriteAchievements (system-services.md) wrapped for the achievements
// harness only: while its recorder is set (hooks::g_start_up_recorder), the
// call is handed to it, so a harness case never unlocks anything in the
// SDK; otherwise the original, always.
extern "C" REX_FUNC(sub_82294E58) {
  REX_FUNC_PROLOGUE();
  if (hooks::StartUpRecorded(ctx, 0x82294E58)) {
    return;
  }
  __imp__sub_82294E58(ctx, base);
}
