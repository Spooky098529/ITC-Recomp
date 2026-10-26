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
// Car::SetModel sub_820CD8A8: r3 = the car, r4 = the id; r3 = the last
// rand() (0 without the parameters).
void RaceSetupSetModel(PPCContext& ctx, uint8_t* base);
// Car::FromBuild sub_820CDCE0: r3 = the car, r4 = the build; r3 = the last
// Params::FirstWheelOfModel.
void RaceSetupFromBuild(PPCContext& ctx, uint8_t* base);

// sub_82101630: r3 = a location, r4 = a byte; r3 the location.
void RaceSetupRivalLocation(PPCContext& ctx, uint8_t* base);
// Race::AddStage sub_82101828: r3 = the type; r3 left.
void RaceSetupAddStage(PPCContext& ctx, uint8_t* base);
// Stages::AddRacer sub_820FFAB8: r3 = the list, r4 = the racer, r5 = the
// word, r6 = the stage or -1; r3 left.
void RaceSetupStagesAddRacer(PPCContext& ctx, uint8_t* base);
// RaceData::RaceData sub_821021B8: r3 = the race data; r3 left.
void RaceSetupRaceDataConstruct(PPCContext& ctx, uint8_t* base);
// sub_820FF558: r3 = the record; r3 left.
void RaceSetupLapRecordReset(PPCContext& ctx, uint8_t* base);
// RunningMean::Add sub_82101698: r3 = the mean, f1 = the value, r5 = the
// weight; r3 left.
void RaceSetupRunningMeanAdd(PPCContext& ctx, uint8_t* base);
// Time::Pack sub_82101700: r3 = milliseconds; r3 the packed time.
void RaceSetupTimePack(PPCContext& ctx, uint8_t* base);
// sub_82110AC0: r3 = save data +48, r4 = the mode; r3 left.
void RaceSetupCountRace(PPCContext& ctx, uint8_t* base);
// RaceEvent::FirstOwned sub_821430A0: r3 = the event; r3 = the id or -1
// (sign-extended).
void RaceSetupEventFirstOwned(PPCContext& ctx, uint8_t* base);
// RaceEvent::CurrentFirstOwned sub_821434C8: r3 = the id or -1
// (sign-extended).
void RaceSetupEventCurrentFirstOwned(PPCContext& ctx, uint8_t* base);
// sub_82101EB0: r3 = the entry; r3 = the original's frame + 80 (its stack
// running mean: r1 - 64), f1 = 0.0.
void RaceSetupEntryReset(PPCContext& ctx, uint8_t* base);
// CourseRecord::Reset sub_82105C90: r3 = the record; r3 = the record + 332.
void RaceSetupCourseRecordReset(PPCContext& ctx, uint8_t* base);
// RaceEvent::Current sub_821434E8, RaceEvent::Count sub_821434F8.
void RaceSetupEventCurrent(PPCContext& ctx, uint8_t* base);
void RaceSetupEventCount(PPCContext& ctx, uint8_t* base);
// sub_82101D20 (no adapter: the array helper's plain C++ calls it): r3 =
// the entry; r3 left.
void RaceSetupRaceDataEntryConstruct(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
