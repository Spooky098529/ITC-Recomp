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

// RaceLights::SetUp sub_8215CF78: r3 = the light set (the scene + 30064);
// r3 = the last entry (r3 + 976) and f1 = the last cos, as the original
// leaves them.
void RaceSceneLightsSetUp(PPCContext& ctx, uint8_t* base);

// RaceScene::SetEffectParams sub_8228F5C0: r3 = the kind, r4 = the index;
// r3 = the last color filter, f1 = its value, as the original leaves them.
// The original where the effects' slots aren't the setters
// (race_scene::EffectParamsReady).
void RaceSceneSetEffectParams(PPCContext& ctx, uint8_t* base);

// The machine's handlers: r3 = the machine (r4 the dispatcher, not read).
// RaceSceneMachine::TogglePause sub_8215C750 (r3 as Race::ToggleEffects
// leaves it) and Resume sub_8215D0D8 (the same, or r3 left when +9608 is
// 0).
void RaceSceneTogglePause(PPCContext& ctx, uint8_t* base);
void RaceSceneResume(PPCContext& ctx, uint8_t* base);
// RaceScene::Destroy sub_8215D790: no arguments; r3 = the scene (left
// without one). The original where operator delete's plain C++ wouldn't
// free it.
void RaceSceneDestroy(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
