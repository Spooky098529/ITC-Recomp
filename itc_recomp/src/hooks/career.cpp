// The career's progress natively: adapters in front of src/career/
// (docs/functions/career.md), with the register steps the harness shares
// (hooks/career.h).

#include "hooks/career.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "lifted/originals.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "career/career.h"
#include "hooks/app_start_up.h"
#include "hooks/guest_call.h"
#include "hooks/race_event.h"
#include "lifted/sdk.h"

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
              "sub_82114C7C merged; Stats::AddRaceResult sub_82115190 (no adapter: native RaceStates::OnRaceEnd calls it) with RunningMean::MergeTwo "
              "sub_82112148 (no adapter); Career::PickNext sub_821159B0's gate (past it, never "
              "seen in play, the original); Progress::AfterRace sub_82116588 with the stage tests "
              "Items::AnyWonKind3Stage2 sub_82111818, Items::AllWonKind3 sub_821118B0, "
              "Items::AllWonKind4Stage2 sub_82111940, Items::AllWonKind4Stage3 sub_821119D0, "
              "Items::AllWonKind5 sub_82111A60 and Locations::Unlock sub_82111D20 and "
              "Unlock2 sub_82111D98 (no adapters); Progress::CheckUnlocks sub_821162B0 (never seen "
              "in play, ported with the race's end; both with no adapters: the race's end calls them); "
              "Career::NextDay sub_82116AF0 with "
              "Stats::CloseDay sub_82113898 (no adapter); no adapters either for RunningMean::Merge, "
              "Items::AnyWonKind3Stage1, ItemRecords::AddWin and AddLoss, Results::Push, "
              "Stats::StartDay, Rewards::Grant, Entry56::Allows, Condition::Test, Car::SpecWord744 "
              "and Garage::InsertCar: only native callers reach them in play; "
              "Career::EventQuery sub_82111638 and Career::EventLocation sub_82111650 (no adapter) over the "
              "race event's plain C++ (src/race_event/), DayBlock::BestStreak sub_82110588 with "
              "no adapter: from the race event's EventCondition::SectionRival; Career::SetUpEvent "
              "sub_82114268 over the race event's RaceEvent::Make plain C++, its cases in "
              "--run_harnesses=race_event; Garage::Add sub_82116CE0 with Car::SetModelAndClass "
              "sub_821104D8) natively instead of through the translated code (the same writes; "
              "checked by --run_harnesses=career)");

namespace hooks {

namespace cr = career;

ITC_REGS_STEP(CareerPlayTimeMark) { cr::PlayTimeMark(ctx.r3.u32); }

ITC_REGS_STEP(CareerDayBlockWinStreak) {
  ctx.r3.u64 = cr::DayBlockWinStreak(ctx.r3.u32);
}

ITC_REGS_STEP(CareerDayBlockAddWord4832) {
  cr::DayBlockAddWord4832(ctx.r3.u32, ctx.r4.u64);
}

ITC_REGS_STEP(CareerDayBlockWord4832) {
  ctx.r3.u64 = cr::DayBlockWord4832(ctx.r3.u32);
}

ITC_REGS_STEP(CareerStatsWinStreak) {
  ctx.r3.u64 = cr::StatsWinStreak(ctx.r3.u32);
}

ITC_REGS_STEP(CareerStatsAddPoints) {
  cr::StatsAddPoints(ctx.r3.u32, ctx.r4.u64);
}

ITC_REGS_STEP(CareerItemFlagsSet) {
  cr::ItemFlagsSet(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(CareerCarSpecWord744) {
  ctx.r3.u64 = cr::CarSpecWord744(ctx.r3.u32);
}

ITC_REGS_STEP(CareerPickFromCar) { cr::PickFromCar(ctx.r3.u32); }

ITC_REGS_STEP(CareerRunningMeanMerge) {
  ctx.fpscr.disableFlushMode();
  cr::RunningMeanMerge(ctx.r3.u32, ctx.r4.u64, ctx.r1.u32);
}

ITC_REGS_STEP(CareerStageOf) {
  ctx.r3.u64 = cr::StageOf(ctx.r3.u32);
}

ITC_REGS_STEP(CareerEntry56Allows) {
  ctx.r3.u64 = cr::Entry56Allows(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                 ctx.r7.u32);
}

ITC_REGS_STEP(CareerAnyWonKind3Stage1) {
  ctx.r3.u64 = cr::AnyWonKind3Stage1();
}

ITC_REGS_STEP(CareerItemRecordsAddWin) {
  ctx.r3.u64 = cr::ItemRecordsAddWin(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CareerItemRecordsAddLoss) {
  ctx.r3.u64 = cr::ItemRecordsAddLoss(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CareerResultsPush) {
  if (!cr::ResultsPushNative(ctx.r3.u32)) {
    lifted::original::c82111FC8(ctx);
    return;
  }
  ctx.r3.u64 = cr::ResultsPush(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(CareerStatsStartDay) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cr::StatsStartDay(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(CareerGarageInsertCar) {
  const uint32_t car = ctx.r4.u32;
  uint32_t destination = 0;
  if (!cr::GarageInsertCar(ctx.r3.u32, car, ctx.r5.u32, &destination)) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = car;
    ctx.r5.u64 = 1968;
    lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy
    return;
  }
  ctx.r3.u64 = destination;
}

ITC_REGS_STEP(CareerConditionTest) {
  if (const auto r3 =
          cr::ConditionTest(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32)) {
    ctx.r3.u64 = *r3;
    return;
  }
  lifted::original::c82112950(ctx);
}

ITC_REGS_STEP(CareerRewardsGrant) {
  ctx.r3.u64 = cr::RewardsGrant(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

ITC_REGS_STEP(CareerAddRaceResult) {
  ctx.fpscr.disableFlushMode();
  if (!cr::AddRaceResultNative(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32)) {
    lifted::original::c82115190(ctx);
    return;
  }
  ctx.r3.u64 = cr::StatsAddRaceResult(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(CareerPickNext) {
  if (!cr::PickNextSkips(ctx.r3.u32)) {
    lifted::original::c821159B0(ctx);
  }
}

ITC_REGS_STEP(CareerAfterRace) {
  ctx.fpscr.disableFlushMode();
  if (!cr::AfterRaceNative(ctx.r1.u32)) {
    lifted::original::c82116588(ctx);
    return;
  }
  ctx.r3.s64 = int32_t(cr::ProgressAfterRace(ctx.r3.u32, ctx.r1.u32));
}

ITC_REGS_STEP(CareerCheckUnlocks) {
  ctx.fpscr.disableFlushMode();
  if (!cr::CheckUnlocksNative(ctx.r1.u32)) {
    lifted::original::c821162B0(ctx);
    return;
  }
  ctx.r3.u64 = cr::ProgressCheckUnlocks(ctx.r3.u32, ctx.r1.u32);
}

namespace {

struct GuestPickNext {
  lifted::Regs& ctx;
  uint8_t* base;
};

// Career::PickNext from Career::NextDay's frame (its adapter: the gate, then
// the original).
void PickNextThrough(void* user, uint32_t save) {
  auto& guest = *static_cast<GuestPickNext*>(user);
  lifted::Regs& ctx = guest.ctx;
  guest_call::InFrame(ctx, cr::kNextDayFrame, 0x82116BA8, [&](uint32_t) {
    ctx.r3.u64 = save;
    ctx.r4.u64 = 0;
    lifted::Enter(ctx, game::Career::PickNext);
  });
}

}  // namespace

ITC_REGS_STEP(CareerNextDay) {
  ctx.fpscr.disableFlushMode();
  GuestPickNext guest{ctx};
  const uint32_t save = ctx.r3.u32;
  const uint32_t sp = ctx.r1.u32;
  ctx.r3.u64 = cr::CareerNextDay(save, sp, PickNextThrough, &guest);
}

ITC_REGS_STEP(CareerEventQuery) {
  if (hooks::StartUpRecorded(ctx, 0x82111638)) {
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t flag = ctx.r4.u32;
  RaceEventGuest guest(ctx);
  ctx.r3.u64 = cr::EventQuery(flag, guest.get());
}

ITC_REGS_STEP(CareerEventLocation) {
  ctx.fpscr.disableFlushMode();
  const uint32_t save = ctx.r3.u32;
  RaceEventGuest guest(ctx);
  cr::EventLocation(save, guest.get());
}

namespace {

struct GuestMake {
  lifted::Regs& ctx;
  uint8_t* base;
  uint32_t sp;
};

// RaceEvent::Make's original from Career::SetUpEvent's frame, where its
// plain C++ refuses (r3 = &the mode, r4 = &the location in the caller's
// frame, r5 = the frame's locations).
uint64_t MakeThrough(void* user, uint32_t) {
  auto& guest = *static_cast<GuestMake*>(user);
  lifted::Regs& ctx = guest.ctx;
  guest_call::InFrame(ctx, cr::kSetUpEventFrame, 0x82114A7C, [&](uint32_t frame) {
    ctx.r3.u64 = guest.sp + 28;
    ctx.r4.u64 = guest.sp + 36;
    ctx.r5.u64 = frame + 80;
    lifted::Sdk(ctx, lifted::original::c82144E58);
  });
  ctx.fpscr.disableFlushMode();
  return ctx.r3.u64;
}

}  // namespace

ITC_REGS_STEP(CareerSetUpEvent) {
  ctx.fpscr.disableFlushMode();
  const uint32_t save = ctx.r3.u32, mode = ctx.r4.u32, location = ctx.r5.u32;
  const uint32_t sp = ctx.r1.u32;
  if (!cr::SetUpEventNative(sp)) {
    lifted::original::c82114268(ctx);
    return;
  }
  RaceEventGuest guest(ctx);
  GuestMake make{ctx, sp};
  ctx.r3.u64 = cr::SetUpEvent(save, mode, location, sp, guest.get(), MakeThrough, &make);
}

ITC_REGS_STEP(CareerCarSetModelAndClass) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = cr::CarSetModelAndClass(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

namespace {

struct GuestCopy {
  lifted::Regs& ctx;
  uint8_t* base;
};

// memcpy (its adapter) from Garage::InsertCar's frame under Garage::Add's,
// where the plain C++ can't copy.
void CopyThrough(void* user, uint32_t destination, uint32_t car) {
  auto& guest = *static_cast<GuestCopy*>(user);
  lifted::Regs& ctx = guest.ctx;
  guest_call::InFrame(ctx, cr::kGarageAddFrame + 112, 0x82115548, [&](uint32_t) {
    ctx.r3.u64 = destination;
    ctx.r4.u64 = car;
    ctx.r5.u64 = 1968;
    lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy
  });
  ctx.fpscr.disableFlushMode();
}

}  // namespace

ITC_REGS_STEP(CareerGarageAdd) {
  ctx.fpscr.disableFlushMode();
  const uint32_t save = ctx.r3.u32, car = ctx.r4.u32, sp = ctx.r1.u32;
  GuestCopy copy{ctx};
  ctx.r3.u64 = cr::GarageAdd(save, car, sp, CopyThrough, &copy);
}

}  // namespace hooks

#define CAREER_ADAPTER(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_career)) { \
    hooks::step(ctx);             \
  }

CAREER_ADAPTER(82111628, CareerPlayTimeMark)
CAREER_ADAPTER(82110580, CareerDayBlockWinStreak)
// DayBlock::AddWord4832 has no adapter (removed, B7 loop, 2026-11-08): its
// only caller in play, RaceHudMachine::AwardPrize, is native and calls its
// plain C++; its step is the harness's.
CAREER_ADAPTER(821105A0, CareerDayBlockWord4832)
CAREER_ADAPTER(82110B10, CareerStatsWinStreak)
// Stats::AddPoints has no adapter (removed, 2026-11-08): its only caller in
// play, RaceHudMachine::AwardPrize, calls its plain C++ (sub_82192958, never
// seen in play, the original).
CAREER_ADAPTER(82112860, CareerItemFlagsSet)
CAREER_ADAPTER(82115CC0, CareerPickFromCar)
CAREER_ADAPTER(821121F8, CareerStageOf)
// Stats::AddRaceResult (sub_82115190) has no adapter (removed, B7 loop,
// 2026-11-13): its only caller, RaceStates::OnRaceEnd, calls its plain C++;
// its step is the harness's.
CAREER_ADAPTER(821159B0, CareerPickNext)
// Progress::AfterRace and Progress::CheckUnlocks have no adapters (removed,
// B7 loop, 2026-11-12): their callers in play, the race's end (and the
// today's result's two for CheckUnlocks), call their plain C++; their steps
// are the harness's.
// Career::NextDay asks the harness recorder first (the title screen's
// harness records it: its career menu's third item).
ITC_NATIVE_ENTRY_ALWAYS(82116AF0) {
  
  if (hooks::StartUpRecorded(ctx, 0x82116AF0)) {
    return;
  }
  if (!ITC_FLAG(native_career)) {
    lifted::original::c82116AF0(ctx);
    return;
  }
  hooks::CareerNextDay(ctx);
}
CAREER_ADAPTER(82111638, CareerEventQuery)
// Career::EventLocation has no adapter (removed, B7 loop, 2026-11-12): its
// only caller in play, the race's end, calls its plain C++; its step is the
// race event harness's.
CAREER_ADAPTER(82114268, CareerSetUpEvent)
CAREER_ADAPTER(821104D8, CareerCarSetModelAndClass)
CAREER_ADAPTER(82116CE0, CareerGarageAdd)
