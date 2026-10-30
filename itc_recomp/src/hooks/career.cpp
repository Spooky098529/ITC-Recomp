// The career's progress natively: adapters in front of src/career/
// (docs/functions/career.md), with the register steps the harness shares
// (hooks/career.h).

#include "hooks/career.h"

#include "base/flags.h"
#include "career/career.h"
#include "hooks/guest_call.h"
#include "hooks/race_event.h"

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
              "sub_821154E0 with GarageList::Insert sub_82113468; Condition::Test sub_82112950 "
              "with its case sub_821129E0 merged; Rewards::Grant sub_82114C08 with its case "
              "sub_82114C7C merged; Stats::AddRaceResult sub_82115190 with RunningMean::MergeTwo "
              "sub_82112148 (no adapter); Career::PickNext sub_821159B0's gate (past it, never "
              "seen in play, the original); Progress::AfterRace sub_82116588 with the stage tests "
              "Items::AnyWonKind3Stage2 sub_82111818, Items::AllWonKind3 sub_821118B0, "
              "Items::AllWonKind4Stage2 sub_82111940, Items::AllWonKind4Stage3 sub_821119D0, "
              "Items::AllWonKind5 sub_82111A60 and Locations::Unlock sub_82111D20 and "
              "Unlock2 sub_82111D98 (no adapters); Career::NextDay sub_82116AF0 with "
              "Stats::CloseDay sub_82113898 (no adapter); no adapters either for RunningMean::Merge, "
              "Items::AnyWonKind3Stage1, ItemRecords::AddWin and AddLoss, Results::Push, "
              "Stats::StartDay, Rewards::Grant, Entry56::Allows, Condition::Test, Car::SpecWord744 "
              "and Garage::InsertCar: only native callers reach them in play; "
              "Career::EventQuery sub_82111638 and Career::EventLocation sub_82111650 over the "
              "race event's plain C++ (src/race_event/), DayBlock::BestStreak sub_82110588 with "
              "no adapter: from the race event's EventCondition::SectionRival; Career::SetUpEvent "
              "sub_82114268 over the race event's RaceEvent::Make plain C++, its cases in "
              "--run_harnesses=race_event; Garage::Add sub_82116CE0 with Car::SetModelAndClass "
              "sub_821104D8) natively instead of through the translated code (the same writes; "
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

void CareerConditionTest(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 =
          cr::ConditionTest(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  __imp__sub_82112950(ctx, base);
}

void CareerRewardsGrant(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = cr::RewardsGrant(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

void CareerAddRaceResult(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cr::AddRaceResultNative(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32)) {
    __imp__sub_82115190(ctx, base);
    return;
  }
  ctx.r3.u64 = cr::StatsAddRaceResult(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void CareerPickNext(PPCContext& ctx, uint8_t* base) {
  if (!cr::PickNextSkips(base, ctx.r3.u32)) {
    __imp__sub_821159B0(ctx, base);
  }
}

void CareerAfterRace(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!cr::AfterRaceNative(base, ctx.r1.u32)) {
    __imp__sub_82116588(ctx, base);
    return;
  }
  ctx.r3.s64 = int32_t(cr::ProgressAfterRace(base, ctx.r3.u32, ctx.r1.u32));
}

namespace {

struct GuestPickNext {
  PPCContext& ctx;
  uint8_t* base;
};

// Career::PickNext from Career::NextDay's frame (its adapter: the gate, then
// the original).
void PickNextThrough(void* user, uint32_t save) {
  auto& guest = *static_cast<GuestPickNext*>(user);
  PPCContext& ctx = guest.ctx;
  guest_call::InFrame(ctx, guest.base, cr::kNextDayFrame, 0x82116BA8, [&](uint32_t) {
    ctx.r3.u64 = save;
    ctx.r4.u64 = 0;
    sub_821159B0(ctx, guest.base);
  });
}

}  // namespace

void CareerNextDay(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  GuestPickNext guest{ctx, base};
  const uint32_t save = ctx.r3.u32;
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cr::CareerNextDay(base, save, sp, PickNextThrough, &guest);
}

void CareerEventQuery(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t flag = ctx.r4.u32;
  RaceEventGuest guest(ctx, base);
  ctx.r3.u64 = cr::EventQuery(base, flag, guest.get());
}

void CareerEventLocation(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t save = ctx.r3.u32;
  RaceEventGuest guest(ctx, base);
  cr::EventLocation(base, save, guest.get());
}

namespace {

struct GuestMake {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t sp;
};

// RaceEvent::Make's original from Career::SetUpEvent's frame, where its
// plain C++ refuses (r3 = &the mode, r4 = &the location in the caller's
// frame, r5 = the frame's locations).
uint64_t MakeThrough(void* user, uint32_t) {
  auto& guest = *static_cast<GuestMake*>(user);
  PPCContext& ctx = guest.ctx;
  guest_call::InFrame(ctx, guest.base, cr::kSetUpEventFrame, 0x82114A7C, [&](uint32_t frame) {
    ctx.r3.u64 = guest.sp + 28;
    ctx.r4.u64 = guest.sp + 36;
    ctx.r5.u64 = frame + 80;
    __imp__sub_82144E58(ctx, guest.base);
  });
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}

}  // namespace

void CareerSetUpEvent(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t save = ctx.r3.u32, mode = ctx.r4.u32, location = ctx.r5.u32;
  const uint32_t sp = ctx.r1.u32;
  if (!cr::SetUpEventNative(base, sp)) {
    __imp__sub_82114268(ctx, base);
    return;
  }
  RaceEventGuest guest(ctx, base);
  GuestMake make{ctx, base, sp};
  ctx.r3.u64 = cr::SetUpEvent(base, save, mode, location, sp, guest.get(), MakeThrough, &make);
}

void CareerCarSetModelAndClass(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cr::CarSetModelAndClass(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

namespace {

struct GuestCopy {
  PPCContext& ctx;
  uint8_t* base;
};

// memcpy (its adapter) from Garage::InsertCar's frame under Garage::Add's,
// where the plain C++ can't copy.
void CopyThrough(void* user, uint32_t destination, uint32_t car) {
  auto& guest = *static_cast<GuestCopy*>(user);
  PPCContext& ctx = guest.ctx;
  guest_call::InFrame(ctx, guest.base, cr::kGarageAddFrame + 112, 0x82115548, [&](uint32_t) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = car;
    ctx.r5.u64 = 1968;
    sub_825A7410(ctx, guest.base);  // memcpy
  });
  ctx.fpscr.disableFlushMode();
}

}  // namespace

void CareerGarageAdd(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint32_t save = ctx.r3.u32, car = ctx.r4.u32, sp = ctx.r1.u32;
  GuestCopy copy{ctx, base};
  ctx.r3.u64 = cr::GarageAdd(base, save, car, sp, CopyThrough, &copy);
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
CAREER_ADAPTER(82115CC0, CareerPickFromCar)
CAREER_ADAPTER(821121F8, CareerStageOf)
CAREER_ADAPTER(82115190, CareerAddRaceResult)
CAREER_ADAPTER(821159B0, CareerPickNext)
CAREER_ADAPTER(82116588, CareerAfterRace)
CAREER_ADAPTER(82116AF0, CareerNextDay)
CAREER_ADAPTER(82111638, CareerEventQuery)
CAREER_ADAPTER(82111650, CareerEventLocation)
CAREER_ADAPTER(82114268, CareerSetUpEvent)
CAREER_ADAPTER(821104D8, CareerCarSetModelAndClass)
CAREER_ADAPTER(82116CE0, CareerGarageAdd)
