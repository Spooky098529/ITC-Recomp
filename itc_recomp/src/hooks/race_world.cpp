// The race world and the racers' manager natively: adapters in front of
// src/race_world/ (docs/functions/race-world.md), with the register steps
// the harness shares (hooks/race_world.h).

#include "hooks/race_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/vehicle_native.h"
#include "race_world/race_world.h"

ITC_FLAG_BOOL(native_race_world, true, "ITC",
              "Run the race world and the racers' manager (docs/functions/race-world.md: "
              "RaceManager::EntryOfRecord sub_821705A8, BranchesMeet sub_82170860 with "
              "sub_821708B4, Traffic::PickModel sub_82171070, SortedList::Insert sub_82170AA0 "
              "and Clear sub_82170A88, RaceWorld::OnPausedTick sub_82170560 and Release "
              "sub_82171360, RoutePosition::ToMainRoad sub_82143608 and StepToMainRoad "
              "sub_82170F18, RaceEvent::Entry sub_82143508 and PlaceEntry sub_82143150, "
              "SaveData::FirstUnsetFlag sub_82112190; RaceManager::Create sub_821715B8 (only while "
              "--native_heap routes the game's heap to the allocation layer), NoteMarkedRacer "
              "sub_82172F90, NearSegment sub_82170BC0, BuildNeighbors sub_82170CB8, Neighbours "
              "sub_82170920 and JumpSections sub_821737F8; TickVehicles sub_821714D8 and "
              "RefreshVehicles sub_82171560, their callees' originals where those refuse; "
              "RaceWorld::Init sub_82174D28 and ReleaseForStage sub_821713C0, "
              "SaveData::AddDistance sub_82116C38; RaceManager::ResetWorld sub_82170668, "
              "ResetVehicles sub_82171718 and RaceWorld::Unload sub_82171250 (only while "
              "--native_heap routes the game's heap to the allocation layer; their callees' "
              "originals where those refuse); RaceEvent::PlaceStartsCurrent sub_82144A38, "
              "PlaceStarts sub_82144798 with sub_82144874, PlaceGroupFixed sub_821443B0, "
              "PlaceGroup sub_82143A40, PlaceGroupTwoRows sub_82143B78 and PlaceGroupSplit "
              "sub_82143920 natively instead of through the "
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

void RaceWorldCreate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = NativeHeapActive() ? rw::Create(base) : std::nullopt;
  if (!r3) {
    __imp__sub_821715B8(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

void RaceWorldNoteMarkedRacer(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::NoteMarkedRacer(base, ctx.r1.u32 - 144);
}

void RaceWorldNearSegment(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      rw::NearSegment(base, ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32 - 208);
}

void RaceWorldBuildNeighbors(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::BuildNeighbors(base, ctx.r3.u32, ctx.r1.u32 - 336);
}

void RaceWorldNeighbours(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::Neighbours(base, ctx.r3.u32, ctx.r4.u32);
}

void RaceWorldJumpSections(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::JumpSections(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                   ctx.r1.u32 - 176);
}

namespace {

// The callees' originals the plain C++ hands back, run with r1 = `sp`.
rw::Original Originals(PPCContext& ctx, uint8_t* base) {
  return [&ctx, base](uint32_t address, uint32_t sp, uint32_t r3) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    switch (address) {
      case 0x820C0118:
        __imp__sub_820C0118(ctx, base);
        break;
      case 0x820C20E0:
        __imp__sub_820C20E0(ctx, base);
        break;
      case 0x820C3B78:
        __imp__sub_820C3B78(ctx, base);
        break;
      case 0x8213B2B8:
        __imp__sub_8213B2B8(ctx, base);
        break;
      case 0x820FEFB0:
        __imp__sub_820FEFB0(ctx, base);
        break;
      case 0x825A70E8:
        __imp__sub_825A70E8(ctx, base);
        break;
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
}

}  // namespace

void RaceWorldTickVehicles(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::TickVehicles(base, ctx.r1.u32 - 128, VehicleTickCalls(ctx, base));
}

void RaceWorldRefreshVehicles(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::RefreshVehicles(base, ctx.r1.u32 - 112, Originals(ctx, base));
}

void RaceWorldInit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = rw::Init(base, machine, ctx.r1.u32 - 1696, &replaced);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

void RaceWorldReleaseForStage(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::ReleaseForStage(base, ctx.r1.u32 - 192,
                      [&](uint32_t address, uint32_t sp, uint32_t r3, uint32_t r4, uint32_t r5) {
                        const uint64_t r1 = ctx.r1.u64;
                        ctx.r1.u64 = sp;
                        ctx.r3.u64 = r3;
                        ctx.r4.u64 = r4;
                        ctx.r5.u64 = int64_t(int32_t(r5));
                        if (address == 0x820FFD88) {
                          __imp__sub_820FFD88(ctx, base);
                        }
                        ctx.r1.u64 = r1;
                        ctx.fpscr.disableFlushMode();
                      });
}

void RaceWorldResetWorld(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    __imp__sub_82170668(ctx, base);
    return;
  }
  rw::ResetWorld(base, ctx.r3.u32, ctx.r1.u32 - 128, Originals(ctx, base));
}

void RaceWorldResetVehicles(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    __imp__sub_82171718(ctx, base);
    return;
  }
  const uint32_t frame = ctx.r1.u32 - 224;
  rw::ResetVehicles(base, ctx.r3.u32, ctx.r4.u32, frame,
                    VehicleSetUpOriginals(ctx, base, frame - 224));
}

void RaceWorldUnload(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    __imp__sub_82171250(ctx, base);
    return;
  }
  rw::Unload(base, ctx.r1.u32 - 112, Originals(ctx, base));
}

void RaceWorldAddDistance(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::AddDistance(base, ctx.r3.u32, ctx.f1.f64);
}

void RaceWorldFirstUnsetFlag(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::FirstUnsetFlag(base, ctx.r3.u32);
}

void RaceWorldPlaceStartsCurrent(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceStartsCurrent(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

void RaceWorldPlaceStarts(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceStarts(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 176);
}

void RaceWorldPlaceGroupFixed(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroupFixed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 96);
}

void RaceWorldPlaceGroup(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroup(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                 ctx.r1.u32 - 192);
}

void RaceWorldPlaceGroupTwoRows(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroupTwoRows(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                        ctx.r1.u32 - 160);
}

void RaceWorldPlaceGroupSplit(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroupSplit(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                      ctx.r1.u32 - 176);
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
ITC_RACE_WORLD(82170F18, RaceWorldStepToMainRoad)
ITC_RACE_WORLD(82143508, RaceWorldRaceEventEntry)
ITC_RACE_WORLD(82112190, RaceWorldFirstUnsetFlag)
ITC_RACE_WORLD(821715B8, RaceWorldCreate)
ITC_RACE_WORLD(82172F90, RaceWorldNoteMarkedRacer)
ITC_RACE_WORLD(82170CB8, RaceWorldBuildNeighbors)
ITC_RACE_WORLD(82170920, RaceWorldNeighbours)
ITC_RACE_WORLD(821737F8, RaceWorldJumpSections)
ITC_RACE_WORLD(821714D8, RaceWorldTickVehicles)
ITC_RACE_WORLD(82171560, RaceWorldRefreshVehicles)
ITC_RACE_WORLD(82174D28, RaceWorldInit)
ITC_RACE_WORLD(821713C0, RaceWorldReleaseForStage)
ITC_RACE_WORLD(82116C38, RaceWorldAddDistance)
ITC_RACE_WORLD(82170668, RaceWorldResetWorld)
ITC_RACE_WORLD(82171718, RaceWorldResetVehicles)
ITC_RACE_WORLD(82171250, RaceWorldUnload)
ITC_RACE_WORLD(82144A38, RaceWorldPlaceStartsCurrent)
// RaceManager::NearSegment (sub_82170BC0) and RoutePosition::ToMainRoad
// (sub_82143608) have no adapters: their callers in play
// (RaceManager::BuildNeighbors, RoutePosition::StepToMainRoad) are native;
// --native_race_world switches those callers. Their register steps stay for
// the harness.
// sub_821708B4 (BranchesMeet's tail) has no adapter: never seen in play, and
// its only caller is BranchesMeet.
// sub_82144874 (a case of RaceEvent::PlaceStarts' jump table, split off by
// the recompiler) has no adapter: its only caller is PlaceStarts, whose
// plain C++ does its work.
// RaceEvent::PlaceEntry (sub_82143150), PlaceStarts (sub_82144798),
// PlaceGroupFixed (sub_821443B0), PlaceGroup (sub_82143A40),
// PlaceGroupTwoRows (sub_82143B78) and PlaceGroupSplit (sub_82143920) have
// no adapters: their callers are native (PlaceStartsCurrent's plain C++ and
// each other's); --native_race_world switches PlaceStartsCurrent. Their
// register steps stay for the harness.
