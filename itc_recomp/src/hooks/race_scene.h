// The race scene's register steps (src/race_scene/,
// docs/functions/race-scene.md), shared by the adapters
// (src/hooks/race_scene.cpp, behind --native_race_scene) and the harness
// (without the flag). Each turns flush-to-zero off first and leaves r3 as
// the original does.

#pragma once

#include "guest.h"

namespace hooks {

// RaceSceneMachine::Init sub_8215D808: r3 = the machine (race mode +
// 87936); r3 as the last StateMachine::Add leaves it. The original where
// operator new's plain C++ refuses (or without --native_heap's layer).
void RaceSceneMachineInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
