// The career's register steps (src/career/, docs/functions/career.md),
// shared by the adapters (src/hooks/career.cpp, behind --native_career) and
// the harness (without the flag; for the functions whose adapters went, only
// the harness). Each leaves r3 as the original does and
// turns flush-to-zero off first where floats are met.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

void CareerPlayTimeMark(lifted::Regs& ctx, uint8_t* base);
void CareerDayBlockWinStreak(lifted::Regs& ctx, uint8_t* base);
void CareerDayBlockAddWord4832(lifted::Regs& ctx, uint8_t* base);
void CareerDayBlockWord4832(lifted::Regs& ctx, uint8_t* base);
void CareerStatsWinStreak(lifted::Regs& ctx, uint8_t* base);
void CareerStatsAddPoints(lifted::Regs& ctx, uint8_t* base);
void CareerItemFlagsSet(lifted::Regs& ctx, uint8_t* base);
void CareerCarSpecWord744(lifted::Regs& ctx, uint8_t* base);
void CareerPickFromCar(lifted::Regs& ctx, uint8_t* base);
void CareerRunningMeanMerge(lifted::Regs& ctx, uint8_t* base);
void CareerStageOf(lifted::Regs& ctx, uint8_t* base);
// Entry56::Allows sub_82111BA8: no adapter (only native callers); the
// harness's step.
void CareerEntry56Allows(lifted::Regs& ctx, uint8_t* base);
void CareerAnyWonKind3Stage1(lifted::Regs& ctx, uint8_t* base);
// ItemRecords::AddWin sub_82114B88, ItemRecords::AddLoss sub_82114B08: r3 =
// the save data, r4 = the item id; r3 = the stats record + 4924.
void CareerItemRecordsAddWin(lifted::Regs& ctx, uint8_t* base);
void CareerItemRecordsAddLoss(lifted::Regs& ctx, uint8_t* base);
// Results::Push sub_82111FC8: r3 = the last results, r4 = the race data; r3
// = the result record (the original where a copy isn't native).
void CareerResultsPush(lifted::Regs& ctx, uint8_t* base);
// Stats::StartDay sub_82112088: r3 = the stats record, r4 = the weekday; r3
// = the day block.
void CareerStatsStartDay(lifted::Regs& ctx, uint8_t* base);
// Garage::InsertCar sub_821154E0: r3 = the garage, r4 = the car, r5 = the
// position; r3 = the slot (memcpy's: through memcpy's adapter where its plain
// C++ can't copy). No adapter (only Garage::Add's plain C++ calls it);
// the harness's step, as Car::SpecWord744 sub_821104B0's.
void CareerGarageInsertCar(lifted::Regs& ctx, uint8_t* base);
// Condition::Test sub_82112950: r3 = the condition, r4 = the location, r5 =
// the kinds' mask; r3 = 0 or 1 (the original where the items' copies aren't
// native). No adapter (only native callers); the harness's step.
void CareerConditionTest(lifted::Regs& ctx, uint8_t* base);
// Rewards::Grant sub_82114C08: r3 = the save data, r4 = the entry's index,
// r5 = the kind, r6 = the value; r3 left.
void CareerRewardsGrant(lifted::Regs& ctx, uint8_t* base);
// Stats::AddRaceResult sub_82115190: r3 = the save data, r4 = the race
// data; r3 as the original leaves it (the original where a copy isn't
// native).
void CareerAddRaceResult(lifted::Regs& ctx, uint8_t* base);
// Career::PickNext sub_821159B0: r3 = the save data, r4 = the flag; nothing
// natively below its gate, the original past it.
void CareerPickNext(lifted::Regs& ctx, uint8_t* base);
// Progress::AfterRace sub_82116588: r3 = the save data; r3 = the stage
// reached or -1 (sign-extended; the original where a copy isn't native).
void CareerAfterRace(lifted::Regs& ctx, uint8_t* base);
// Progress::CheckUnlocks sub_821162B0: r3 = the save data; r3 = 0 (the
// original where a copy isn't native).
void CareerCheckUnlocks(lifted::Regs& ctx, uint8_t* base);
// Career::NextDay sub_82116AF0: r3 = the save data; r3 = Stats::StartDay's.
// Career::PickNext past its gate through its adapter, from a frame like the
// original's.
void CareerNextDay(lifted::Regs& ctx, uint8_t* base);

// Career::EventQuery sub_82111638: r4 = the flag; r3 = -1 or
// RaceEvent::RefreshCurrent's. Career::EventLocation sub_82111650: r3 = the
// save data.
void CareerEventQuery(lifted::Regs& ctx, uint8_t* base);
void CareerEventLocation(lifted::Regs& ctx, uint8_t* base);

// Career::SetUpEvent sub_82114268: r3 = the save data, r4 = the mode, r5 =
// the location; r3 as the original leaves it (the original where a copy
// isn't native; RaceEvent::Make's original from its frame where the plain
// C++ refuses).
void CareerSetUpEvent(lifted::Regs& ctx, uint8_t* base);

// Car::SetModelAndClass sub_821104D8: r3 = a car object, r4 = a car id; r3
// = the car's spec. Garage::Add sub_82116CE0: r3 = the save data, r4 = a car
// object; r3 as the original leaves it (memcpy's adapter from a frame like
// Garage::InsertCar's where its plain C++ can't copy).
void CareerCarSetModelAndClass(lifted::Regs& ctx, uint8_t* base);
void CareerGarageAdd(lifted::Regs& ctx, uint8_t* base);

}  // namespace hooks
