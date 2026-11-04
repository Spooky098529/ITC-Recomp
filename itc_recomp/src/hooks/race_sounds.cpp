// The race's sounds natively: adapters in front of src/race_sounds/
// (docs/functions/race-sounds.md), with the register steps the harness
// shares (hooks/race_sounds.h). The handlers are reached through the race
// mode's state machine (functors) and the callbacks through function
// pointers, so the adapters stay; PickRivalMusic's and PlayRivalMusic's
// are gone (their callers are all native: their register steps stay for
// the harness).

#include "hooks/race_sounds.h"

#include "base/flags.h"
#include "hooks/car_sounds.h"
#include "hooks/menu_sounds.h"
#include "hooks/mode_tick.h"
#include "race_sounds/race_sounds.h"

ITC_FLAG_BOOL(native_race_sounds, true, "ITC",
              "Run the race's sounds (docs/functions/race-sounds.md: the race mode's sound "
              "machine at +138284; SetListener sub_821758A0, FadeMusic sub_82175908, "
              "RestoreMusicVolume sub_821759C8, EndTick sub_82175A40, Pause sub_82175AE8, Resume "
              "sub_82175B80, PauseCars sub_82175C00, ResumeCars sub_82175C58, OnStageCountdown "
              "sub_82175D20, OnRollingStartDone sub_82175D48, OnEffectsBankLoaded sub_82175ED0, "
              "SetUpCars sub_82176188, UpdateCars sub_82175F20, PickRivalMusic sub_821764F0, "
              "PlayRivalMusic sub_821762F8, RaceTick sub_821769D0, StageTick sub_82176F28, "
              "RollingStartTick sub_82177130, AfterRaceTick sub_82176C10, OnCountdown "
              "sub_82176968, OnRaceEnd sub_82176BA0, OnLeaveAfterRace sub_82176E60, OnNextStage "
              "sub_82176EC8, OnRollingStart sub_821770C0, PlayStartMusic sub_82175DB0 (rand's plain "
              "C++), IdleTick sub_821767A8, Begin sub_82176618; the car sounds, Music::Play and Sound::Play through their plain C++, the sound manager's slots through the "
              "function table) natively instead of through the translated code (the same "
              "words; checked by --run_harnesses=race_sounds)");

namespace hooks {

namespace {

namespace rs = race_sounds;

// The guest calls the plain C++ makes (race_sounds::Calls), through this
// context: r1, the first `set` of r3 to r6 and lr set; the car sounds run as
// their register steps (their plain C++), the rest through the function
// table (its override when it has one); r1 put back. The adapter's lr is
// put back when it ends.
class GuestCalls {
 public:
  GuestCalls(PPCContext& ctx, uint8_t* base) : ctx_(ctx), base_(base), lr_(ctx.lr) {}
  ~GuestCalls() { ctx_.lr = lr_; }
  GuestCalls(const GuestCalls&) = delete;
  GuestCalls& operator=(const GuestCalls&) = delete;
  rs::Calls calls() { return {this, &GuestCalls::Call}; }

 private:
  static uint64_t Call(void* user, uint32_t function, const uint64_t args[4], int set,
                       uint32_t r1, uint32_t lr) {
    auto& self = *static_cast<GuestCalls*>(user);
    PPCContext& ctx = self.ctx_;
    uint8_t* base = self.base_;
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    PPCRegister* registers[4] = {&ctx.r3, &ctx.r4, &ctx.r5, &ctx.r6};
    for (int i = 0; i < set && i < 4; i++) {
      registers[i]->u64 = args[i];
    }
    if (set > 4) {
      ctx.r7.u64 = args[4];
    }
    if (set > 5) {
      ctx.r8.u64 = args[5];
    }
    ctx.lr = lr;
    switch (function) {
      case rs::kCarSoundPauseAll:
        CarSoundsPauseAll(ctx, base);
        break;
      case rs::kCarSoundResumeAll:
        CarSoundsResumeAll(ctx, base);
        break;
      case rs::kCarSoundsStopAll:
        CarSoundsStopAll(ctx, base);
        break;
      case rs::kCarSoundsRelease:
        CarSoundsReleaseAll(ctx, base);
        break;
      case rs::kCarSoundStop:
        CarSoundsStop(ctx, base);
        break;
      case rs::kCarSoundStart:
        CarSoundsCarSoundStart(ctx, base);
        break;
      case rs::kCarSoundUpdate:
        CarSoundsUpdate(ctx, base);
        break;
      case rs::kMusicPlay:
        ModeTickMusicPlay(ctx, base);
        break;
      case rs::kSoundPlay:
        SoundPlay(ctx, base);
        break;
      default:
        ctx.ctr.u64 = function;
        REX_CALL_INDIRECT_FUNC(function);
    }
    ctx.r1.u64 = stack;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  }
  PPCContext& ctx_;
  uint8_t* base_;
  uint64_t lr_;
};

// Runs `body(calls)` with the guest calls through `ctx`, its r3 into r3.
template <typename Body>
void Step(PPCContext& ctx, uint8_t* base, Body body) {
  ctx.fpscr.disableFlushMode();
  uint64_t r3;
  {
    GuestCalls guest(ctx, base);
    r3 = body(guest.calls());
  }
  ctx.r3.u64 = r3;
}

}  // namespace

void RaceSoundsSetListener(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64, r4 = ctx.r4.u64, r5 = ctx.r5.u64, r6 = ctx.r6.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base,
       [&](const rs::Calls& c) { return rs::SetListener(base, r3, r4, r5, r6, sp, c); });
}

void RaceSoundsFadeMusic(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::FadeMusic(base, sp, c); });
}

void RaceSoundsRestoreMusicVolume(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::RestoreMusicVolume(base, sp, c); });
}

void RaceSoundsEndTick(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::EndTick(base, r3, sp, c); });
}

void RaceSoundsPause(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::Pause(base, sp, c); });
}

void RaceSoundsResume(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::Resume(base, sp, c); });
}

void RaceSoundsPauseCars(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::PauseCars(base, sp, c); });
}

void RaceSoundsResumeCars(PPCContext& ctx, uint8_t* base) {
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::ResumeCars(base, sp, c); });
}

void RaceSoundsOnStageCountdown(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::OnStageCountdown(base, ctx.r3.u64);
}

void RaceSoundsOnRollingStartDone(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::OnRollingStartDone(base, r3, sp, c); });
}

void RaceSoundsOnEffectsBankLoaded(PPCContext&, uint8_t* base) {
  race_sounds::OnEffectsBankLoaded(base);
}

void RaceSoundsSetUpCars(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::SetUpCars(base, r3, sp, c); });
}

void RaceSoundsUpdateCars(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::UpdateCars(base, r3, sp, c); });
}

void RaceSoundsPickRivalMusic(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = rs::PickRivalMusic(base, ctx.r3.u64, ctx.r1.u32);
}

void RaceSoundsPlayRivalMusic(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::PlayRivalMusic(base, r3, sp, c); });
}

void RaceSoundsRaceTick(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::RaceTick(base, r3, sp, c); });
}

void RaceSoundsStageTick(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::StageTick(base, r3, sp, c); });
}

void RaceSoundsRollingStartTick(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::RollingStartTick(base, r3, sp, c); });
}

void RaceSoundsAfterRaceTick(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::AfterRaceTick(base, r3, sp, c); });
}

void RaceSoundsOnCountdown(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::OnCountdown(base, r3, sp, c); });
}

void RaceSoundsOnRaceEnd(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::OnRaceEnd(base, r3, sp, c); });
}

void RaceSoundsOnLeaveAfterRace(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::OnLeaveAfterRace(base, r3, sp, c); });
}

void RaceSoundsOnNextStage(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::OnNextStage(base, r3, sp, c); });
}

void RaceSoundsOnRollingStart(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::OnRollingStart(base, r3, sp, c); });
}

void RaceSoundsPlayStartMusic(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::PlayStartMusic(base, r3, sp, c); });
}

void RaceSoundsIdleTick(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::IdleTick(base, r3, sp, c); });
}

void RaceSoundsBegin(PPCContext& ctx, uint8_t* base) {
  const uint64_t r3 = ctx.r3.u64;
  const uint32_t sp = ctx.r1.u32;
  Step(ctx, base, [&](const rs::Calls& c) { return rs::Begin(base, r3, sp, c); });
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_SOUNDS(address, step)   \
  extern "C" REX_FUNC(sub_##address) {   \
    REX_FUNC_PROLOGUE();                 \
    if (!ITC_FLAG(native_race_sounds)) { \
      __imp__sub_##address(ctx, base);   \
      return;                            \
    }                                    \
    hooks::step(ctx, base);              \
  }

ITC_RACE_SOUNDS(821758A0, RaceSoundsSetListener)
ITC_RACE_SOUNDS(82175908, RaceSoundsFadeMusic)
ITC_RACE_SOUNDS(821759C8, RaceSoundsRestoreMusicVolume)
ITC_RACE_SOUNDS(82175A40, RaceSoundsEndTick)
ITC_RACE_SOUNDS(82175AE8, RaceSoundsPause)
ITC_RACE_SOUNDS(82175B80, RaceSoundsResume)
ITC_RACE_SOUNDS(82175C00, RaceSoundsPauseCars)
ITC_RACE_SOUNDS(82175C58, RaceSoundsResumeCars)
ITC_RACE_SOUNDS(82175D20, RaceSoundsOnStageCountdown)
ITC_RACE_SOUNDS(82175D48, RaceSoundsOnRollingStartDone)
ITC_RACE_SOUNDS(82175ED0, RaceSoundsOnEffectsBankLoaded)
ITC_RACE_SOUNDS(82176188, RaceSoundsSetUpCars)
ITC_RACE_SOUNDS(82175F20, RaceSoundsUpdateCars)
ITC_RACE_SOUNDS(821769D0, RaceSoundsRaceTick)
ITC_RACE_SOUNDS(82176F28, RaceSoundsStageTick)
ITC_RACE_SOUNDS(82177130, RaceSoundsRollingStartTick)
ITC_RACE_SOUNDS(82176C10, RaceSoundsAfterRaceTick)
ITC_RACE_SOUNDS(82176968, RaceSoundsOnCountdown)
ITC_RACE_SOUNDS(82176BA0, RaceSoundsOnRaceEnd)
ITC_RACE_SOUNDS(82176E60, RaceSoundsOnLeaveAfterRace)
ITC_RACE_SOUNDS(82176EC8, RaceSoundsOnNextStage)
ITC_RACE_SOUNDS(821770C0, RaceSoundsOnRollingStart)
ITC_RACE_SOUNDS(82175DB0, RaceSoundsPlayStartMusic)
ITC_RACE_SOUNDS(821767A8, RaceSoundsIdleTick)
ITC_RACE_SOUNDS(82176618, RaceSoundsBegin)
