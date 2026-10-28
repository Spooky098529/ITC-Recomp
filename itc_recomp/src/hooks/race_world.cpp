// The race world and the racers' manager natively: adapters in front of
// src/race_world/ (docs/functions/race-world.md), with the register steps
// the harness shares (hooks/race_world.h).

#include "hooks/race_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "race_world/race_world.h"

ITC_FLAG_BOOL(native_race_world, true, "ITC",
              "Run the race world and the racers' manager (docs/functions/race-world.md: "
              "RaceManager::EntryOfRecord sub_821705A8, BranchesMeet sub_82170860 with "
              "sub_821708B4, Traffic::PickModel sub_82171070, SortedList::Insert sub_82170AA0 "
              "and Clear sub_82170A88, RaceWorld::OnPausedTick sub_82170560 and Release "
              "sub_82171360, RoutePosition::ToMainRoad sub_82143608 and StepToMainRoad "
              "sub_82170F18, RaceEvent::Entry sub_82143508 and PlaceEntry sub_82143150, "
              "SaveData::FirstUnsetFlag sub_82112190) natively instead of through the "
              "translated code (the same words, bit for bit; checked by "
              "--run_harnesses=race_world)");

namespace hooks {

namespace rw = race_world;

void RaceWorldEntryOfRecord(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::EntryOfRecord(base, ctx.r3.u32);
}

void RaceWorldBranchesMeet(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::BranchesMeet(ctx.r3.u32, ctx.r4.u32);
}

void RaceWorldPickModel(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::PickModel(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceWorldSortedListInsert(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::SortedListInsert(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}

void RaceWorldSortedListClear(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::SortedListClear(base, ctx.r3.u32);
}

void RaceWorldOnPausedTick(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::OnPausedTick();
  ctx.r11.u64 = 0xFFFFFFFF82010000ull;
  ctx.r5.u64 = 0;
  ctx.r6.u64 = 0xFFFFFFFF820099ACull;
  ctx.r4.u64 = 400;
  ctx.r3.u64 = 400;
}

void RaceWorldRelease(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::Release(base, ctx.r3.u32);
}

void RaceWorldToMainRoad(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::RoutePositionToMainRoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceWorldStepToMainRoad(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::StepToMainRoad(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112);
}

void RaceWorldRaceEventEntry(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::RaceEventEntry(base, ctx.r3.u32);
}

void RaceWorldPlaceEntry(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::PlaceEntry(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                              ctx.r8.u32);
}

void RaceWorldFirstUnsetFlag(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::FirstUnsetFlag(base, ctx.r3.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_WORLD(address, step)     \
  extern "C" REX_FUNC(sub_##address) {    \
    REX_FUNC_PROLOGUE();                  \
    if (!ITC_FLAG(native_race_world)) {   \
      __imp__sub_##address(ctx, base);    \
      return;                             \
    }                                     \
    hooks::step(ctx, base);               \
  }

ITC_RACE_WORLD(821705A8, RaceWorldEntryOfRecord)
ITC_RACE_WORLD(82170860, RaceWorldBranchesMeet)
ITC_RACE_WORLD(82171070, RaceWorldPickModel)
ITC_RACE_WORLD(82170AA0, RaceWorldSortedListInsert)
ITC_RACE_WORLD(82170A88, RaceWorldSortedListClear)
ITC_RACE_WORLD(82170560, RaceWorldOnPausedTick)
ITC_RACE_WORLD(82171360, RaceWorldRelease)
ITC_RACE_WORLD(82143608, RaceWorldToMainRoad)
ITC_RACE_WORLD(82170F18, RaceWorldStepToMainRoad)
ITC_RACE_WORLD(82143508, RaceWorldRaceEventEntry)
ITC_RACE_WORLD(82143150, RaceWorldPlaceEntry)
ITC_RACE_WORLD(82112190, RaceWorldFirstUnsetFlag)
// sub_821708B4 (BranchesMeet's tail) has no adapter: never seen in play, and
// its only caller is BranchesMeet.
