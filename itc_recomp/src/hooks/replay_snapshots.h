// The replay snapshots' register steps (src/replay_snapshots/,
// docs/functions/replay-snapshots.md), shared by the adapters
// (src/hooks/replay_snapshots.cpp, behind --native_replay_snapshots) and the
// harness (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// Record5232::Restore sub_821044D8: r3 = a record, r4 = a vehicle; r3 as
// the original leaves it.
void Record5232Restore(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
