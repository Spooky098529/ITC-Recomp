// The achievements natively: adapters in front of src/achievements/
// (docs/functions/achievements.md), with the register steps the harness
// shares (hooks/achievements.h). AllBeaten and BeatsCourseTime have no
// adapters: only native Check calls them; nor do Check and Write since
// RaceStates::OnRaceEnd is native. Time::Less's adapter stays for
// Time::Clamp (recompiled).

#include "hooks/achievements.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "achievements/achievements.h"
#include "base/flags.h"
#include "hooks/app_start_up.h"
#include "hooks/lifted_call.h"
#include "lifted/sdk.h"
#include "schema/achievements.h"

ITC_FLAG_BOOL(native_achievements, true, "ITC",
              "Run the achievements (docs/functions/achievements.md: Achievements::Check "
              "sub_82290788 with AllBeaten sub_82290698, BeatsCourseTime sub_82290710 and "
              "Time::Less sub_820FF708; Achievements::Write sub_82290A30) natively (no adapters but Time::Less's: native RaceStates::OnRaceEnd "
              "calls Check and Write) instead of through the translated code (the same "
              "words; checked by --run_harnesses=achievements)");

namespace hooks {

namespace {

// The guest calls the plain C++ makes (achievements::Calls), through this
// context: r1, r3 to r5 and lr set, the function run through the function
// table (its override when it has one), r1 put back. The adapter's lr is
// put back when it ends.
class GuestCalls {
 public:
  GuestCalls(lifted::Regs& ctx) : ctx_(ctx), base_(), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  achievements::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[3], uint32_t r1,
                       uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    lifted::Regs& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = args[0];
    ctx.r4.u64 = args[1];
    ctx.r5.u64 = args[2];
    ctx.lr = lr;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    return ctx.r3.u64;
  }
  lifted::Regs& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

}  // namespace

ITC_REGS_STEP(TimeLessSteps) {
  ctx.r3.u64 = achievements::TimeLess(schema::AchievementsPackedTime(ctx.r3.u32).packed(),
                                      schema::AchievementsPackedTime(ctx.r4.u32).packed());
}

ITC_REGS_STEP(AchievementsCheckSteps) {
  ctx.r3.u64 = achievements::Check(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(WriteAchievementsSteps) {
  lifted::Enter(ctx, [](const lifted::Args& a) -> lifted::Result {
    return {achievements::WriteAchievements(uint32_t(a.r[0]), uint32_t(a.r[1]),
                                            uint32_t(a.r[2])),
            a.f[0]};
  });
}

ITC_REGS_STEP(AchievementsWriteSteps) {
  GuestCalls guest(ctx);
  ctx.r3.u64 = achievements::Write(ctx.r3.u64, ctx.r1.u32, guest.calls());
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_ACHIEVEMENTS(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_achievements)) { \
    hooks::step(ctx);               \
  }

ITC_ACHIEVEMENTS(820FF708, TimeLessSteps)
// Achievements::Check (sub_82290788) and Write (sub_82290A30) have no
// adapters (removed, B7 loop, 2026-11-13): their caller in play,
// RaceStates::OnRaceEnd, calls their plain C++; the network battle screen
// (sub_82264B90, never seen in play, recompiled) runs the originals. Their
// steps are the harness's.

// XUserWriteAchievements (system-services.md), plugged in whatever the flag
// (the user's call, 2026-11-16): the game reads nothing back, so the SDK's
// import isn't reached and no unlock is recorded.
ITC_NATIVE_ENTRY_ALWAYS(82294E58) {
  hooks::WriteAchievementsSteps(ctx);
}
