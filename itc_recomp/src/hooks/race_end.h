// The race's end's register steps (src/race_end/, docs/functions/race-end.md
// and race-loading.md), shared by the adapters (src/hooks/race_end.cpp,
// behind --native_race_end) and the harness (without the flag). Each takes
// the original's registers (r3 the 8-byte answer, r4 the race mode) and
// leaves r3 as the original does (the answer); guest calls go through `ctx`.

#pragma once

#include "guest.h"

namespace hooks {

void RaceModeModeAfterRace(PPCContext& ctx, uint8_t* base);  // sub_82150E00
void RaceModeNextMode(PPCContext& ctx, uint8_t* base);       // sub_82151EF0

}  // namespace hooks
