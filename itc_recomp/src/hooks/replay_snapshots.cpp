// The replay snapshots natively: the adapters in front of
// src/replay_snapshots/ (docs/functions/replay-snapshots.md), with the
// register steps the harness shares (hooks/replay_snapshots.h).

#include "hooks/replay_snapshots.h"

#include "base/flags.h"
#include "native/buffers.h"
#include "replay_snapshots/replay_snapshots.h"

ITC_FLAG_BOOL(native_replay_snapshots, true, "ITC",
              "Run the replay's snapshots (docs/functions/replay-snapshots.md: a vehicle's "
              "5,232-byte record put back, Record5232::Restore sub_821044D8, and the race "
              "saved into a snapshot block, RaceSnapshot::Save sub_82104AB8, with their parts) "
              "natively instead of through the translated code (the same writes; checked by "
              "--run_harnesses=replay_snapshots)");

namespace hooks {

void Record5232Restore(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::Record5232Restore(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceSnapshotSave(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = replay_snapshots::RaceSnapshotSave(base, ctx.r3.u32);
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
