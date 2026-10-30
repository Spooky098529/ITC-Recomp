// The career's progress natively: adapters in front of src/career/
// (docs/functions/career.md), with the register steps the harness shares
// (hooks/career.h).

#include "hooks/career.h"

#include "base/flags.h"
#include "career/career.h"

ITC_FLAG_BOOL(native_career, true, "ITC",
              "Run the career's progress (docs/functions/career.md: PlayTime::Mark "
              "sub_82111628, DayBlock::WinStreak sub_82110580, DayBlock::AddWord4832 "
              "sub_82110590, DayBlock::Word4832 sub_821105A0, Stats::WinStreak sub_82110B10, "
              "Stats::AddPoints sub_82110B18, ItemFlags::Set sub_82112860, Car::SpecWord744 "
              "sub_821104B0, Career::PickFromCar sub_82115CC0, RunningMean::Merge sub_82111CB0, "
              "Stage::Of sub_821121F8, Entry56::Allows sub_82111BA8, Items::AnyWonKind3Stage1 "
              "sub_82111780; ItemRecords::AddWin sub_82114B88 with Stats::AddWin sub_821139B8, "
              "Counter::AddWin sub_82111E10 and Opponents::AddWin sub_821134D0, "
              "ItemRecords::AddLoss sub_82114B08 with Stats::AddLoss sub_82113AB8, Counter::AddLoss "
              "sub_82111E80, WeekCounter::AddLoss sub_82111F60 and Opponents::AddLoss sub_82113580, "
              "and the outcome 3's ItemRecords::AddOutcome3 sub_82114A88, Stats::AddOutcome3 "
              "sub_82113B50, Counter::AddOutcome3 sub_82111EF0, Opponents::AddOutcome3 "
              "sub_82113628 (no adapters for these callees: only their callers' plain C++ calls "
              "them); Results::Push sub_82111FC8 with ResultRecord::Fill sub_82110850; "
              "Stats::StartDay sub_82112088 with DayLog::Push sub_82112028; Garage::InsertCar "
              "sub_821154E0 with GarageList::Insert sub_82113468) natively instead of through the translated code (the same writes; "
              "checked by --run_harnesses=career)");

namespace hooks {

namespace cr = career;

void CareerPlayTimeMark(PPCContext& ctx, uint8_t* base) { cr::PlayTimeMark(base, ctx.r3.u32); }

void CareerDayBlockWinStreak(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::DayBlockWinStreak(base, ctx.r3.u32);
}

void CareerDayBlockAddWord4832(PPCContext& ctx, uint8_t* base) {
  cr::DayBlockAddWord4832(base, ctx.r3.u32, ctx.r4.u64);
}

void CareerDayBlockWord4832(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::DayBlockWord4832(base, ctx.r3.u32);
}

void CareerStatsWinStreak(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::StatsWinStreak(base, ctx.r3.u32);
}

void CareerStatsAddPoints(PPCContext& ctx, uint8_t* base) {
  cr::StatsAddPoints(base, ctx.r3.u32, ctx.r4.u64);
}

void CareerItemFlagsSet(PPCContext& ctx, uint8_t* base) {
  cr::ItemFlagsSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void CareerCarSpecWord744(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::CarSpecWord744(base, ctx.r3.u32);
}

void CareerPickFromCar(PPCContext& ctx, uint8_t* base) { cr::PickFromCar(base, ctx.r3.u32); }

void CareerRunningMeanMerge(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  cr::RunningMeanMerge(base, ctx.r3.u32, ctx.r4.u64, ctx.r1.u32);
}

void CareerStageOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::StageOf(base, ctx.r3.u32);
}

void CareerEntry56Allows(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::Entry56Allows(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                 ctx.r7.u32);
}

void CareerAnyWonKind3Stage1(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::AnyWonKind3Stage1(base);
}

void CareerItemRecordsAddWin(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::ItemRecordsAddWin(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CareerItemRecordsAddLoss(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::ItemRecordsAddLoss(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CareerResultsPush(PPCContext& ctx, uint8_t* base) {
  if (!cr::ResultsPushNative(ctx.r3.u32)) {
    __imp__sub_82111FC8(ctx, base);
    return;
  }
  ctx.r3.u64 = cr::ResultsPush(base, ctx.r3.u32, ctx.r4.u32);
}

void CareerStatsStartDay(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cr::StatsStartDay(base, ctx.r3.u32, ctx.r4.u32);
}

void CareerGarageInsertCar(PPCContext& ctx, uint8_t* base) {
  const uint32_t car = ctx.r4.u32;
  uint32_t destination = 0;
  if (!cr::GarageInsertCar(base, ctx.r3.u32, car, ctx.r5.u32, &destination)) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = car;
    ctx.r5.u64 = 1968;
    sub_825A7410(ctx, base);  // memcpy
    return;
  }
  ctx.r3.u64 = destination;
}

}  // namespace hooks

#define CAREER_ADAPTER(address, step)   \
  extern "C" REX_FUNC(sub_##address) {  \
    REX_FUNC_PROLOGUE();                \
    if (!ITC_FLAG(native_career)) {     \
      __imp__sub_##address(ctx, base);  \
      return;                           \
    }                                   \
    hooks::step(ctx, base);             \
  }

CAREER_ADAPTER(82111628, CareerPlayTimeMark)
CAREER_ADAPTER(82110580, CareerDayBlockWinStreak)
CAREER_ADAPTER(82110590, CareerDayBlockAddWord4832)
CAREER_ADAPTER(821105A0, CareerDayBlockWord4832)
CAREER_ADAPTER(82110B10, CareerStatsWinStreak)
CAREER_ADAPTER(82110B18, CareerStatsAddPoints)
CAREER_ADAPTER(82112860, CareerItemFlagsSet)
CAREER_ADAPTER(821104B0, CareerCarSpecWord744)
CAREER_ADAPTER(82115CC0, CareerPickFromCar)
CAREER_ADAPTER(82111CB0, CareerRunningMeanMerge)
CAREER_ADAPTER(821121F8, CareerStageOf)
CAREER_ADAPTER(82111BA8, CareerEntry56Allows)
CAREER_ADAPTER(82111780, CareerAnyWonKind3Stage1)
CAREER_ADAPTER(82114B88, CareerItemRecordsAddWin)
CAREER_ADAPTER(82114B08, CareerItemRecordsAddLoss)
CAREER_ADAPTER(82111FC8, CareerResultsPush)
CAREER_ADAPTER(82112088, CareerStatsStartDay)
CAREER_ADAPTER(821154E0, CareerGarageInsertCar)
