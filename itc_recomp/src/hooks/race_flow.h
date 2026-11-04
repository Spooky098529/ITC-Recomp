// The race flow's register steps (src/race_flow/,
// docs/functions/race-flow.md), shared by the adapter
// (src/hooks/race_flow.cpp, behind --native_race_flow) and the harness
// (without the flag).

#pragma once

#include "guest.h"

namespace hooks {

// RaceFlow::Init sub_82160AE0: r3 = the race flow (race mode + 97548); r3
// as the last StateMachine::Add leaves it.
void RaceFlowInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
