// The race's update's register steps (src/race_update/,
// docs/functions/race-update.md), shared by the adapters
// (src/hooks/race_update.cpp, behind --native_race_update) and the harness
// (without the flag). Each turns flush-to-zero off first and leaves r3 as
// the original does.

#pragma once

#include "guest.h"

#include "race_update/race_update.h"

namespace hooks {

// RaceScene::VaryCar sub_8215B330: r3 = the car, r4 = the racer index, r5 =
// the vehicle index, r6 = the racer's +16; r3 left (the car).
void RaceUpdateVaryCar(PPCContext& ctx, uint8_t* base);
// RaceLights::Update sub_8215D008: r3 = the light set (the scene + 30064);
// r3 = the set + 1072 (past the last entry) and r4 the count, as the
// original leaves them.
void RaceUpdateLights(PPCContext& ctx, uint8_t* base);
// RaceScene::LoadScenery sub_8215B790: r3 = the race scene; r3 as
// Scenery::StartLoad leaves it. Only where hooks::CourseScenerySetUpNative.
void RaceUpdateLoadScenery(PPCContext& ctx, uint8_t* base);
// The guest calls the race update's plain C++ makes through `ctx` (the
// callee by its address, so overridden ones run their adapters).
race_update::GuestCall RaceUpdateGuestCall(PPCContext& ctx, uint8_t* base);
// RaceScene::RemakeCarModel sub_8215C0D8: r4 = the scene, r5 = a car slot,
// r6 = a car, r7 = the paint pick; r3 = 0 or 1, lr put back.
void RaceUpdateRemakeCarModel(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
