// The career's register steps (src/career/, docs/functions/career.md),
// shared by the adapters (src/hooks/career.cpp, behind --native_career) and
// the harness (without the flag). Each leaves r3 as the original does and
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

}  // namespace hooks
