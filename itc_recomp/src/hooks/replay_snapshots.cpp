// The replay snapshots natively: the adapters in front of
// src/replay_snapshots/ (docs/functions/replay-snapshots.md), with the
// register steps the harness shares (hooks/replay_snapshots.h).

#include "hooks/replay_snapshots.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "hooks/race_world.h"
#include "hooks/vehicle_native.h"
#include "native/buffers.h"
#include "replay_snapshots/replay_snapshots.h"

ITC_FLAG_BOOL(native_replay_snapshots, true, "ITC",
              "Run the replay's snapshots (docs/functions/replay-snapshots.md: a vehicle's "
              "5,232-byte record put back, Record5232::Restore sub_821044D8, and the race "
              "saved into a snapshot block, RaceSnapshot::Save sub_82104AB8, with their parts; "
              "the race kept for its replay, Replay::Keep sub_82104F68, with the copies; "
              "RaceReplay::SaveStart sub_82104D40, SaveEnd sub_82104E68, RestoreStart "
              "sub_821046F8 and RestoreEnd sub_82104898, the last three only while "
              "--native_heap routes the game's heap to the allocation layer) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=replay_snapshots)");

namespace hooks {

namespace {

// The calls RaceReplay's functions' callees make where their plain C++
// refuses, as the callees' adapters make them; `frame` is the caller's
// (ResetVehicles' frame 224 bytes below it, Vehicle::SetUp's below that).
replay_snapshots::ReplayCalls ReplayCalls(PPCContext& ctx, uint8_t* base, uint32_t frame) {
  replay_snapshots::ReplayCalls calls;
  calls.list = RaceWorldListRacersOriginal(ctx, base);
  calls.setup = VehicleSetUpOriginals(ctx, base, frame - 224 - 224);
  calls.callee = [&ctx, base](uint32_t address, uint32_t sp, uint32_t r3) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    if (address == 0x820C1D08) {
      __imp__sub_820C1D08(ctx, base);
    } else {
      __imp__sub_820B7998(ctx, base);
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
  // As Vehicle::PlaceOnRoute's adapter (src/hooks/vehicle_native.cpp).
  calls.place = [&ctx, base](uint32_t address, uint32_t r1, const vehicle::GuestRegisters& r) {
    const uint64_t saved = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = r.r3;
    ctx.r4.u64 = r.r4;
    ctx.r5.u64 = r.r5;
    if (address == 0x820B7998) {
      __imp__sub_820B7998(ctx, base);
    } else {
      sub_825A7410(ctx, base);  // memcpy, through its override as the original
    }
    ctx.r1.u64 = saved;
    ctx.fpscr.disableFlushMode();
  };
  calls.place_original = [&ctx, base](uint32_t sp, uint32_t vehicle, uint32_t position,
                                      double speed, double offset, uint32_t lane,
                                      uint32_t direction) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = vehicle;
    ctx.r4.u64 = position;
    ctx.f1.f64 = speed;
    ctx.f2.f64 = offset;
    ctx.r6.u64 = lane;
    ctx.r7.u64 = direction;
    __imp__sub_820C3608(ctx, base);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
  return calls;
}

}  // namespace

void Record5232Restore(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::Record5232Restore(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSnapshotSave(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::RaceSnapshotSave(base, ctx.r3.u32);
}

void ReplayKeep(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::ReplayKeep(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r1.u32 - 272,
      [&ctx, base](uint32_t out, uint32_t stages, uint32_t stage, uint32_t sp) {
        const uint64_t r1 = ctx.r1.u64;
        ctx.r1.u64 = sp;
        ctx.r3.u64 = out;
        ctx.r4.u64 = stages;
        ctx.r5.u64 = int64_t(int32_t(stage));
        __imp__sub_820FFD88(ctx, base);
        ctx.r1.u64 = r1;
        ctx.fpscr.disableFlushMode();
      });
}

void RaceReplaySaveStart(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 144;
  const auto calls = ReplayCalls(ctx, base, frame);
  ctx.r3.u64 = replay_snapshots::RaceReplaySaveStart(base, frame, calls);
}

void RaceReplaySaveEnd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::RaceReplaySaveEnd(base);
}

void RaceReplayRestoreStart(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 144;
  const auto calls = ReplayCalls(ctx, base, frame);
  ctx.r3.u64 = replay_snapshots::RaceReplayRestoreStart(base, frame, calls);
}

void RaceReplayRestoreEnd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 160;
  const auto calls = ReplayCalls(ctx, base, frame);
  ctx.r3.u64 = replay_snapshots::RaceReplayRestoreEnd(base, frame, calls);
}

}  // namespace hooks

namespace {

// Whether the adapters run the plain C++: the flag, and memcpy's plain C++
// (its copies are done through native::Buffers_Memcpy).
bool Native() {
  return ITC_FLAG(native_replay_snapshots) && native::Buffers_NativeMemcpy();
}

}  // namespace

// The adapters: the register steps above, or the original.
// Callers: RaceReplay::SaveStart, RestoreStart, RestoreEnd (recompiled).
extern "C" REX_FUNC(sub_821044D8) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_821044D8(ctx, base);
    return;
  }
  hooks::Record5232Restore(ctx, base);
}

// Callers: RaceReplay::SaveStart, SaveEnd (recompiled).
extern "C" REX_FUNC(sub_82104AB8) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_82104AB8(ctx, base);
    return;
  }
  hooks::RaceSnapshotSave(ctx, base);
}

// Caller: RaceStates::OnRaceEnd (recompiled).
extern "C" REX_FUNC(sub_82104F68) {
  REX_FUNC_PROLOGUE();
  if (!Native()) {
    __imp__sub_82104F68(ctx, base);
    return;
  }
  hooks::ReplayKeep(ctx, base);
}

// Callers: RaceStates::OnCountdown (SaveStart), ReplaySetUpTick (SaveEnd,
// RestoreStart), AfterReplayTick (RestoreEnd), sub_82168858 (RestoreStart,
// not seen in play); all recompiled.
#define ITC_RACE_REPLAY(address, step, heap)                    \
  extern "C" REX_FUNC(sub_##address) {                          \
    REX_FUNC_PROLOGUE();                                        \
    if (!Native() || ((heap) && !hooks::NativeHeapActive())) {  \
      __imp__sub_##address(ctx, base);                          \
      return;                                                   \
    }                                                           \
    hooks::step(ctx, base);                                     \
  }
ITC_RACE_REPLAY(82104D40, RaceReplaySaveStart, true)
ITC_RACE_REPLAY(82104E68, RaceReplaySaveEnd, false)
ITC_RACE_REPLAY(821046F8, RaceReplayRestoreStart, true)
ITC_RACE_REPLAY(82104898, RaceReplayRestoreEnd, true)
