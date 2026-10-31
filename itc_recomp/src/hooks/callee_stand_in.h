// A harness's stand-in for the callees of the functions it checks: while
// set, the adapters of those callees (the race HUD object's, the alternate
// HUD's and RaceHud::RunMovie's) and the race HUD machine's calls of them
// (src/hooks/race_hud_machine.cpp) hand their call to it first, with r1, r3,
// r4 and lr as the call made them; when it answers true, the call ends there
// (the stand-in set r3 and f1). So the race HUD machine's harness sees what
// the recompiled handlers and the native ones hand each callee without
// setting up the race HUD's world. Null in the game.

#pragma once

#include "guest.h"

namespace hooks {

using CalleeStandIn = bool (*)(PPCContext& ctx, uint32_t address);
extern CalleeStandIn g_callee_stand_in;
inline bool CalleeStoodIn(PPCContext& ctx, uint32_t address) {
  return g_callee_stand_in != nullptr && g_callee_stand_in(ctx, address);
}

}  // namespace hooks
