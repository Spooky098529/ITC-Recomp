// The career's register steps (src/career/, docs/functions/career.md),
// shared by the adapters (src/hooks/career.cpp, behind --native_career) and
// the harness (without the flag; for the functions whose adapters went, only
// the harness). Each leaves r3 as the original does and
// turns flush-to-zero off first where floats are met.

#pragma once

#include "guest.h"

namespace hooks {

void CareerPlayTimeMark(PPCContext& ctx, uint8_t* base);
void CareerDayBlockWinStreak(PPCContext& ctx, uint8_t* base);
void CareerDayBlockAddWord4832(PPCContext& ctx, uint8_t* base);
void CareerDayBlockWord4832(PPCContext& ctx, uint8_t* base);
void CareerStatsWinStreak(PPCContext& ctx, uint8_t* base);
void CareerStatsAddPoints(PPCContext& ctx, uint8_t* base);
void CareerItemFlagsSet(PPCContext& ctx, uint8_t* base);
void CareerCarSpecWord744(PPCContext& ctx, uint8_t* base);
void CareerPickFromCar(PPCContext& ctx, uint8_t* base);
void CareerRunningMeanMerge(PPCContext& ctx, uint8_t* base);
void CareerStageOf(PPCContext& ctx, uint8_t* base);
void CareerEntry56Allows(PPCContext& ctx, uint8_t* base);
void CareerAnyWonKind3Stage1(PPCContext& ctx, uint8_t* base);
// ItemRecords::AddWin sub_82114B88, ItemRecords::AddLoss sub_82114B08: r3 =
// the save data, r4 = the item id; r3 = the stats record + 4924.
void CareerItemRecordsAddWin(PPCContext& ctx, uint8_t* base);
void CareerItemRecordsAddLoss(PPCContext& ctx, uint8_t* base);
// Results::Push sub_82111FC8: r3 = the last results, r4 = the race data; r3
// = the result record (the original where a copy isn't native).
void CareerResultsPush(PPCContext& ctx, uint8_t* base);
// Stats::StartDay sub_82112088: r3 = the stats record, r4 = the weekday; r3
// = the day block.
void CareerStatsStartDay(PPCContext& ctx, uint8_t* base);
// Garage::InsertCar sub_821154E0: r3 = the garage, r4 = the car, r5 = the
// position; r3 = the slot (memcpy's: through memcpy's adapter where its plain
// C++ can't copy).
void CareerGarageInsertCar(PPCContext& ctx, uint8_t* base);
// Condition::Test sub_82112950: r3 = the condition, r4 = the location, r5 =
// the kinds' mask; r3 = 0 or 1 (the original where the items' copies aren't
// native).
void CareerConditionTest(PPCContext& ctx, uint8_t* base);
// Rewards::Grant sub_82114C08: r3 = the save data, r4 = the entry's index,
// r5 = the kind, r6 = the value; r3 left.
void CareerRewardsGrant(PPCContext& ctx, uint8_t* base);
// Stats::AddRaceResult sub_82115190: r3 = the save data, r4 = the race
// data; r3 as the original leaves it (the original where a copy isn't
// native).
void CareerAddRaceResult(PPCContext& ctx, uint8_t* base);
// Career::PickNext sub_821159B0: r3 = the save data, r4 = the flag; nothing
// natively below its gate, the original past it.
void CareerPickNext(PPCContext& ctx, uint8_t* base);
// Progress::AfterRace sub_82116588: r3 = the save data; r3 = the stage
// reached or -1 (sign-extended; the original where a copy isn't native).
void CareerAfterRace(PPCContext& ctx, uint8_t* base);
// Career::NextDay sub_82116AF0: r3 = the save data; r3 = Stats::StartDay's.
// Career::PickNext past its gate through its adapter, from a frame like the
// original's.
void CareerNextDay(PPCContext& ctx, uint8_t* base);

// Career::EventQuery sub_82111638: r4 = the flag; r3 = -1 or
// RaceEvent::RefreshCurrent's. Career::EventLocation sub_82111650: r3 = the
// save data.
void CareerEventQuery(PPCContext& ctx, uint8_t* base);
void CareerEventLocation(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
