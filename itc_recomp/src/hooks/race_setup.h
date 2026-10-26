// The race's set-up's register steps (src/race_setup/,
// docs/functions/race-setup.md), shared by the adapters
// (src/hooks/race_setup.cpp, behind --native_race_setup) and the harness
// (without the flag). Each leaves r3 as the original does and turns
// flush-to-zero off first.

#pragma once

#include "guest.h"

namespace hooks {

// Car::ResetDecals40 sub_820CC4A0, Car::ResetDecals6 sub_820CCBF0,
// Car::ResetSettings sub_820CD760, sub_820CCC70: r3 = the part; r3 left.
void RaceSetupResetDecals40(PPCContext& ctx, uint8_t* base);
void RaceSetupResetDecals6(PPCContext& ctx, uint8_t* base);
void RaceSetupResetSettings(PPCContext& ctx, uint8_t* base);
void RaceSetupReset1568(PPCContext& ctx, uint8_t* base);
// CarTuning::Reset sub_820CD618: r3 = the tuning, r4 = the flag; r3 left.
void RaceSetupTuningReset(PPCContext& ctx, uint8_t* base);
// CarDecal::FromBuild sub_820CDC18: r3 = the decal, r4 = the build's entry;
// r3 left.
void RaceSetupDecalFromBuild(PPCContext& ctx, uint8_t* base);
// Car::Clear sub_820CD7F0, Car::Init sub_820CD5D8: r3 = the car; r3 = the car
// + 1568.
void RaceSetupCarClear(PPCContext& ctx, uint8_t* base);
void RaceSetupCarInit(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
