// The race's set-up's register steps (src/race_setup/,
// docs/functions/race-setup.md), shared by the adapters
// (src/hooks/race_setup.cpp, behind --native_race_setup) and the harness
// (without the flag). Each leaves r3 as the original does and turns
// flush-to-zero off first.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// Car::ResetDecals40 sub_820CC4A0, Car::ResetDecals6 sub_820CCBF0,
// Car::ResetSettings sub_820CD760, sub_820CCC70: r3 = the part; r3 left.
void RaceSetupResetDecals40(lifted::Regs& ctx);
void RaceSetupResetDecals6(lifted::Regs& ctx);
void RaceSetupResetSettings(lifted::Regs& ctx);
void RaceSetupReset1568(lifted::Regs& ctx);
// CarTuning::Reset sub_820CD618: r3 = the tuning, r4 = the flag; r3 left.
void RaceSetupTuningReset(lifted::Regs& ctx);
// CarDecal::FromBuild sub_820CDC18: r3 = the decal, r4 = the build's entry;
// r3 left.
void RaceSetupDecalFromBuild(lifted::Regs& ctx);
// Car::Clear sub_820CD7F0, Car::Init sub_820CD5D8: r3 = the car; r3 = the car
// + 1568.
void RaceSetupCarClear(lifted::Regs& ctx);
void RaceSetupCarInit(lifted::Regs& ctx);
// Car::SetupVehicle sub_820CD5C8: r3 = the car, r4 = the car data; r3 and f1
// as Vehicle::SetupFromCar leaves them. Its original (r3 and r4 swapped, the
// tail call) where --native_car_physics is off or it refuses.
void RaceSetupSetupVehicle(lifted::Regs& ctx);
// Car::SetModel sub_820CD8A8: r3 = the car, r4 = the id; r3 = the last
// rand() (0 without the parameters).
void RaceSetupSetModel(lifted::Regs& ctx);
// Car::FromBuild sub_820CDCE0: r3 = the car, r4 = the build; r3 = the last
// Params::FirstWheelOfModel.
void RaceSetupFromBuild(lifted::Regs& ctx);

// sub_82101630: r3 = a location, r4 = a byte; r3 the location.
void RaceSetupRivalLocation(lifted::Regs& ctx);
// Race::AddStage sub_82101828: r3 = the type; r3 left.
void RaceSetupAddStage(lifted::Regs& ctx);
// Stages::AddRacer sub_820FFAB8: r3 = the list, r4 = the racer, r5 = the
// word, r6 = the stage or -1; r3 left.
void RaceSetupStagesAddRacer(lifted::Regs& ctx);
// RaceData::RaceData sub_821021B8: r3 = the race data; r3 left.
void RaceSetupRaceDataConstruct(lifted::Regs& ctx);
// sub_820FF558: r3 = the record; r3 left.
void RaceSetupLapRecordReset(lifted::Regs& ctx);
// RunningMean::Add sub_82101698: r3 = the mean, f1 = the value, r5 = the
// weight; r3 left.
void RaceSetupRunningMeanAdd(lifted::Regs& ctx);
// Time::Pack sub_82101700: r3 = milliseconds; r3 the packed time.
void RaceSetupTimePack(lifted::Regs& ctx);
// sub_82110AC0: r3 = save data +48, r4 = the mode; r3 left.
void RaceSetupCountRace(lifted::Regs& ctx);
// RaceEvent::FirstOwned sub_821430A0: r3 = the event; r3 = the id or -1
// (sign-extended).
void RaceSetupEventFirstOwned(lifted::Regs& ctx);
// RaceEvent::CurrentFirstOwned sub_821434C8: r3 = the id or -1
// (sign-extended).
void RaceSetupEventCurrentFirstOwned(lifted::Regs& ctx);
// sub_82101EB0: r3 = the entry; r3 = the original's frame + 80 (its stack
// running mean: r1 - 64), f1 = 0.0.
void RaceSetupEntryReset(lifted::Regs& ctx);
// CourseRecord::Reset sub_82105C90: r3 = the record; r3 = the record + 332.
void RaceSetupCourseRecordReset(lifted::Regs& ctx);
// RaceEvent::Current sub_821434E8, RaceEvent::Count sub_821434F8.
void RaceSetupEventCurrent(lifted::Regs& ctx);
void RaceSetupEventCount(lifted::Regs& ctx);
// CourseRoutes::Get sub_820FFF50: r3 = out, r4 = the index; r3 = out (in the
// original's stack frame, the caller's r1 - 4480).
void RaceSetupCourseRoutesGet(lifted::Regs& ctx);
// sub_82101D20 (no adapter: the array helper's plain C++ calls it): r3 =
// the entry; r3 left.
void RaceSetupRaceDataEntryConstruct(lifted::Regs& ctx);

// Stages::Reset sub_82101860: r3 = the list; r3 = the list + 812 + 332.
void RaceSetupStagesReset(lifted::Regs& ctx);
// RacerRecord::Init sub_820FF808: r3 = the record; r3 = the record + 1568.
void RaceSetupRacerRecordInit(lifted::Regs& ctx);
// Route::Set sub_82101A10: r3 = the route, r4 = the course route's index,
// r5 = the location; Route::Reset sub_82101990: r3 = the route. r3 = their
// frame + 80 (r1 - 416). The original where a copy wouldn't be native.
void RaceSetupRouteSet(lifted::Regs& ctx);
void RaceSetupRouteReset(lifted::Regs& ctx);
// Race::StartPosition sub_82101A98: r3 = the location; r3 = the position
// (zero-extended). The original where a copy wouldn't be native.
void RaceSetupStartPosition(lifted::Regs& ctx);

// Racers::Reset sub_820FF978: r3 = the racer records; r3 = the last record
// + 1568.
void RaceSetupRacersReset(lifted::Regs& ctx);
// Racers::Add sub_820FF860: r3 = the records, r4 = a car object, f1, r6; r3
// = its index or -1. Race::AddRacer sub_821017C0: r3 = a racer record, r5 =
// a flag, f1, r6; r3 = Racers::Add's. The originals where the copy wouldn't
// be native.
void RaceSetupRacersAdd(lifted::Regs& ctx);
void RaceSetupAddRacer(lifted::Regs& ctx);
// sub_82101F68: r3 = one of the 77 records; r3 = Route::Reset's frame + 80
// (r1 - 544), f1 = 0.0. The original where a copy wouldn't be native.
void RaceSetupRecordReset(lifted::Regs& ctx);
// Race::SetRoute sub_82101D38: r3 = the index, r4 = the location; Stages::
// SetRoute sub_82101E90: r3 = the list, r4, r5 as Route::Set's, r6 = the
// stage or -1. r3 = Route::Set's (r1 - 416). The originals where a copy
// wouldn't be native.
void RaceSetupRaceSetRoute(lifted::Regs& ctx);
void RaceSetupStagesSetRoute(lifted::Regs& ctx);

// RaceData::Reset sub_821020F0, RaceData::ResetGlobal sub_821021A8: r3 =
// the last sub_82101F68's (r1 - 768), f1 = 0.0. Racers::SetStart
// sub_82101D50: r3 = the records, r4 = the location, r5 = the stage's type;
// Race::PlaceRacers sub_82102048: r3 = 1 for both. The originals where a copy
// wouldn't be native (and Race::PlaceRacers' save data branch).
void RaceSetupRaceDataReset(lifted::Regs& ctx);
void RaceSetupRaceDataResetGlobal(lifted::Regs& ctx);
void RaceSetupSetStart(lifted::Regs& ctx);
void RaceSetupPlaceRacers(lifted::Regs& ctx);

// Race::SetUpRival sub_82112DB0: r3 = the location, r4 = the rival's item
// id, r5 = add; r3 = 1. The original for kind 1 and where a copy wouldn't be
// native.
void RaceSetupSetUpRival(lifted::Regs& ctx);

// Race::SetUp sub_82115D98: r3 = the save data, r4 = a rival's item id or
// -1; r3 = the save data + 48. The original where memcpy's override wouldn't
// copy natively.
void RaceSetupSetUp(lifted::Regs& ctx);

// Stages::Clear sub_82101900: r3 = the list; r3 = Stages::Reset's.
void RaceSetupStagesClear(lifted::Regs& ctx);

}  // namespace hooks
