// The race world and the racers' manager natively: adapters in front of
// src/race_world/ (docs/functions/race-world.md), with the register steps
// the harness shares (hooks/race_world.h).

#include "hooks/lifted_call.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"
#include "hooks/race_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/ai_drivers.h"
#include "hooks/car_physics.h"
#include "hooks/heap.h"
#include "hooks/vehicle_native.h"
#include "race_world/race_world.h"
#include "lifted/sdk.h"

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
              "sub_82143920; RaceManager::ListRacers sub_82171E50 (Vehicle::RemoveFromWorld's "
              "original unless --native_heap routes the game's heap to the allocation layer) "
              "and AddRacers sub_82173050; RaceWorld::OnLoaded sub_82173A90 (only while "
              "--native_heap routes the game's heap to the allocation layer), RaceStart::Place "
              "sub_82165E20 and PlaceRolling sub_821662E0 (their callees' originals where those "
              "refuse); RaceManager::StreamOpponents sub_82172118 (Vehicle::RemoveFromWorld's "
              "original unless --native_heap routes the game's heap to the allocation layer; "
              "RaceManager::GiveVehicle sub_821706E8, natively too), StreamTraffic sub_821724F8 "
              "(Vehicle::RemoveFromWorld's and the physics set-up's originals unless --native_heap "
              "routes the game's heap to the allocation layer; its callees' originals where those "
              "refuse) and MoveUnseen sub_82173E28; RaceManager::Stream sub_82174338 (with "
              "sub_8217437C), StreamNear sub_82172E60, StreamCruise sub_82174200, "
              "StreamTwoPlayers sub_82172D20 and RaceWorld::Tick sub_82174BC0 (sub_821743A8's "
              "original; the callees' originals where those refuse); RaceManager::Refresh "
              "sub_82171978 (as StreamOpponents) natively instead of "
              "through the "
              "translated code (the same words, bit for bit; checked by "
              "--run_harnesses=race_world)");

namespace hooks {

namespace rw = race_world;

ITC_REGS_STEP(RaceWorldEntryOfRecord) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::EntryOfRecord(ctx.r3.u32);
}

ITC_REGS_STEP(RaceWorldBranchesMeet) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::BranchesMeet(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceWorldPickModel) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::PickModel(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceWorldSortedListInsert) {
  ctx.fpscr.disableFlushMode();
  rw::SortedListInsert(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}

ITC_REGS_STEP(RaceWorldSortedListClear) {
  ctx.fpscr.disableFlushMode();
  rw::SortedListClear(ctx.r3.u32);
}

ITC_REGS_STEP(RaceWorldOnPausedTick) {
  ctx.fpscr.disableFlushMode();
  rw::OnPausedTick();
  // (r11 = 0x82010000, a leftover for recompiled callers: none now)
  ctx.r5.u64 = 0;
  ctx.r6.u64 = 0xFFFFFFFF820099ACull;
  ctx.r4.u64 = 400;
  ctx.r3.u64 = 400;
}

ITC_REGS_STEP(RaceWorldRelease) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::Release(ctx.r3.u32);
}

ITC_REGS_STEP(RaceWorldToMainRoad) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::RoutePositionToMainRoad(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceWorldStepToMainRoad) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::StepToMainRoad(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112);
}

ITC_REGS_STEP(RaceWorldRaceEventEntry) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::RaceEventEntry(ctx.r3.u32);
}

ITC_REGS_STEP(RaceWorldPlaceEntry) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::PlaceEntry(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                              ctx.r8.u32);
}

ITC_REGS_STEP(RaceWorldCreate) {
  ctx.fpscr.disableFlushMode();
  const auto r3 = NativeHeapActive() ? rw::Create() : std::nullopt;
  if (!r3) {
    lifted::original::c821715B8(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(RaceWorldNoteMarkedRacer) {
  ctx.fpscr.disableFlushMode();
  rw::NoteMarkedRacer(ctx.r1.u32 - 144);
}

ITC_REGS_STEP(RaceWorldNearSegment) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 =
      rw::NearSegment(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.r1.u32 - 208);
}

ITC_REGS_STEP(RaceWorldBuildNeighbors) {
  ctx.fpscr.disableFlushMode();
  rw::BuildNeighbors(ctx.r3.u32, ctx.r1.u32 - 336);
}

ITC_REGS_STEP(RaceWorldNeighbours) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::Neighbours(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(RaceWorldJumpSections) {
  ctx.fpscr.disableFlushMode();
  rw::JumpSections(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                   ctx.r1.u32 - 176);
}

namespace {

// The callees' originals the plain C++ hands back, run with r1 = `sp`.
rw::Original Originals(lifted::Regs& ctx) {
  return [&ctx](uint32_t address, uint32_t sp, uint32_t r3) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    switch (address) {
      case 0x820C0118:
        lifted::original::c820C0118(ctx);
        break;
      case 0x820C20E0:
        lifted::original::c820C20E0(ctx);
        break;
      case 0x820C3B78:
        lifted::original::c820C3B78(ctx);
        break;
      case 0x8213B2B8:
        lifted::original::c8213B2B8(ctx);
        break;
      case 0x820FEFB0:
        lifted::original::c820FEFB0(ctx);
        break;
      case 0x825A70E8:
        lifted::original::c825A70E8(ctx);
        break;
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
}

}  // namespace

ITC_REGS_STEP(RaceWorldTickVehicles) {
  ctx.fpscr.disableFlushMode();
  rw::TickVehicles(ctx.r1.u32 - 128, VehicleTickCalls(ctx));
}

ITC_REGS_STEP(RaceWorldRefreshVehicles) {
  ctx.fpscr.disableFlushMode();
  rw::RefreshVehicles(ctx.r1.u32 - 112, Originals(ctx));
}

ITC_REGS_STEP(RaceWorldInit) {
  ctx.fpscr.disableFlushMode();
  const uint64_t machine_r3 = ctx.r3.u64;
  const uint32_t machine = ctx.r3.u32;
  bool replaced = false;
  const uint32_t r3 = rw::Init(machine, ctx.r1.u32 - 1696, &replaced);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

ITC_REGS_STEP(RaceWorldReleaseForStage) {
  ctx.fpscr.disableFlushMode();
  rw::ReleaseForStage(ctx.r1.u32 - 192,
                      [&](uint32_t address, uint32_t sp, uint32_t r3, uint32_t r4, uint32_t r5) {
                        const uint64_t r1 = ctx.r1.u64;
                        ctx.r1.u64 = sp;
                        ctx.r3.u64 = r3;
                        ctx.r4.u64 = r4;
                        ctx.r5.u64 = int64_t(int32_t(r5));
                        if (address == 0x820FFD88) {
                          lifted::original::c820FFD88(ctx);
                        }
                        ctx.r1.u64 = r1;
                        ctx.fpscr.disableFlushMode();
                      });
}

ITC_REGS_STEP(RaceWorldResetWorld) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    lifted::original::c82170668(ctx);
    return;
  }
  rw::ResetWorld(ctx.r3.u32, ctx.r1.u32 - 128, Originals(ctx));
}

ITC_REGS_STEP(RaceWorldResetVehicles) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    lifted::original::c82171718(ctx);
    return;
  }
  const uint32_t frame = ctx.r1.u32 - 224;
  rw::ResetVehicles(ctx.r3.u32, ctx.r4.u32, frame,
                    VehicleSetUpOriginals(ctx, frame - 224));
}

ITC_REGS_STEP(RaceWorldUnload) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    lifted::original::c82171250(ctx);
    return;
  }
  rw::Unload(ctx.r1.u32 - 112, Originals(ctx));
}

ITC_REGS_STEP(RaceWorldAddDistance) {
  ctx.fpscr.disableFlushMode();
  rw::AddDistance(ctx.r3.u32, ctx.f1.f64);
}

ITC_REGS_STEP(RaceWorldFirstUnsetFlag) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rw::FirstUnsetFlag(ctx.r3.u32);
}

ITC_REGS_STEP(RaceWorldListRacers) {
  ctx.fpscr.disableFlushMode();
  const rw::Original original = Originals(ctx);
  ctx.r3.u64 = rw::ListRacers(ctx.r3.u32, ctx.r1.u32 - 1872, NativeHeapActive(),
                              [&](uint32_t address, uint32_t sp, uint32_t r3) {
                                original(address, sp, r3);
                                return ctx.r3.u64;
                              });
}

ITC_REGS_STEP(RaceWorldAddRacers) {
  ctx.fpscr.disableFlushMode();
  rw::AddRacers(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 2000);
}

ITC_REGS_STEP(RaceWorldPlaceStartsCurrent) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceStartsCurrent(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
}

ITC_REGS_STEP(RaceWorldPlaceStarts) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceStarts(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 176);
}

ITC_REGS_STEP(RaceWorldPlaceGroupFixed) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroupFixed(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 96);
}

ITC_REGS_STEP(RaceWorldPlaceGroup) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroup(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                 ctx.r1.u32 - 192);
}

ITC_REGS_STEP(RaceWorldPlaceGroupTwoRows) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroupTwoRows(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                        ctx.r1.u32 - 160);
}

ITC_REGS_STEP(RaceWorldPlaceGroupSplit) {
  ctx.fpscr.disableFlushMode();
  rw::PlaceGroupSplit(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                      ctx.r1.u32 - 176);
}

namespace {

// The start functions' guest calls (race_world::StartCalls), with
// Vehicle::SetUp's callees' originals in `setup_frame`.
rw::StartCalls StartCalls(lifted::Regs& ctx, uint32_t setup_frame) {
  namespace vh = vehicle;
  rw::StartCalls calls;
  calls.original = [&ctx](uint32_t address, uint32_t sp, uint64_t r3, uint64_t r4,
                                uint64_t r5) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    switch (address) {
      case 0x820FFD88:
        lifted::original::c820FFD88(ctx);
        break;
      case 0x820C3BD0:
        lifted::original::c820C3BD0(ctx);
        break;
      case 0x8213B1E0:
        lifted::original::c8213B1E0(ctx);
        break;
      case 0x821715B8:
        lifted::original::c821715B8(ctx);
        break;
      case 0x820C20E0:
        lifted::original::c820C20E0(ctx);
        break;
      case 0x821706E8:
        lifted::Enter(ctx, game::RaceManager::GiveVehicle);  // the original, through the harness's stand-in
        break;
      case 0x820C0118:
        lifted::original::c820C0118(ctx);
        break;
      case 0x821743A8:
        ctx.lr = 0x82174CF4;  // as RaceWorld::Tick calls it
        lifted::Enter(ctx, lifted::entry::e821743A8);  // the original, through the harness's stand-in
        break;
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  calls.setup_original = VehicleSetUpOriginals(ctx, setup_frame);
  calls.tick = VehicleTickCalls(ctx);
  calls.place = [&ctx](uint32_t address, uint32_t r1, const vh::GuestRegisters& r) {
    const uint64_t saved = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.r3.u64 = r.r3;
    ctx.r4.u64 = r.r4;
    ctx.r5.u64 = r.r5;
    if (address == 0x820C3608) {
      ctx.r6.u64 = r.r6;
      ctx.r7.u64 = 1;
      ctx.f1.f64 = r.f1;
      ctx.f2.f64 = r.f2;
      lifted::original::c820C3608(ctx);
    } else if (address == 0x820B7998) {
      lifted::original::c820B7998(ctx);
    } else {
      lifted::Enter(ctx, game::memcpy_825A7410);  // memcpy, through its override as the original
    }
    ctx.r1.u64 = saved;
    ctx.fpscr.disableFlushMode();
  };
  calls.collision = CollisionWorldUpdateCalls(ctx);
  calls.update_drivers = [&ctx](uint32_t sp, uint32_t lr) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    AiUpdateAll(ctx);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
  };
  calls.drive_car = [&ctx](uint32_t input, uint32_t index, uint32_t flag, uint32_t sp,
                                 uint32_t lr) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.lr = lr;
    ctx.r3.u64 = input;
    ctx.r4.u64 = index;
    ctx.r5.u64 = flag;
    AiDriveCar(ctx);
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
  return calls;
}

}  // namespace

ITC_REGS_STEP(RaceWorldOnLoaded) {
  ctx.fpscr.disableFlushMode();
  if (!NativeHeapActive()) {
    lifted::original::c82173A90(ctx);
    return;
  }
  const uint32_t frame = ctx.r1.u32 - 352;
  rw::OnLoaded(frame, StartCalls(ctx, frame - 224));
}

ITC_REGS_STEP(RaceWorldPlace) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 400;
  rw::Place(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32,
            StartCalls(ctx, frame - 224));
}

ITC_REGS_STEP(RaceWorldPlaceRolling) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 272;
  rw::PlaceRolling(ctx.r3.u32, ctx.r4.u32, frame, StartCalls(ctx, frame - 224));
}

RaceWorldStandIn g_give_vehicle_stand_in = nullptr;

namespace {

// StreamOpponents' and Refresh's guest calls: Vehicle::RemoveFromWorld's
// and RaceManager::GiveVehicle's originals.
rw::OriginalCall OpponentsCall(lifted::Regs& ctx) {
  return [&ctx](uint32_t address, uint32_t sp, uint64_t r3, uint64_t r4, uint64_t) {
    const uint64_t r1 = ctx.r1.u64;
    ctx.r1.u64 = sp;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    if (address == 0x820C20E0) {
      lifted::original::c820C20E0(ctx);
    } else if (address == 0x821706E8) {
      lifted::Enter(ctx, game::RaceManager::GiveVehicle);  // the original, through the harness's stand-in
    }
    ctx.r1.u64 = r1;
    ctx.fpscr.disableFlushMode();
    return ctx.r3.u64;
  };
}

}  // namespace

rw::OriginalCall RaceWorldOpponentsCall(lifted::Regs& ctx) {
  return OpponentsCall(ctx);
}

rw::StartCalls RaceWorldStartCalls(lifted::Regs& ctx, uint32_t setup_frame) {
  return StartCalls(ctx, setup_frame);
}

rw::OriginalR3 RaceWorldListRacersOriginal(lifted::Regs& ctx) {
  const rw::Original original = Originals(ctx);
  return [&ctx, original](uint32_t address, uint32_t sp, uint32_t r3) {
    original(address, sp, r3);
    return ctx.r3.u64;
  };
}

ITC_REGS_STEP(RaceWorldStreamOpponents) {
  ctx.fpscr.disableFlushMode();
  rw::StreamOpponents(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64, ctx.r1.u32 - 1920,
                      NativeHeapActive(), OpponentsCall(ctx));
}

ITC_REGS_STEP(RaceWorldRefresh) {
  ctx.fpscr.disableFlushMode();
  rw::Refresh(ctx.r3.u32, ctx.r1.u32 - 1904, NativeHeapActive(), OpponentsCall(ctx));
}

ITC_REGS_STEP(RaceWorldStreamTraffic) {
  ctx.fpscr.disableFlushMode();
  const uint32_t frame = ctx.r1.u32 - 464;
  rw::StreamTraffic(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64, ctx.f2.f64, ctx.r5.u32, frame,
                    NativeHeapActive(), StartCalls(ctx, frame - 224));
}

ITC_REGS_STEP(RaceWorldMoveUnseen) {
  ctx.fpscr.disableFlushMode();
  rw::MoveUnseen(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 272);
}

namespace {

// The streaming's and the tick's guest calls, made for a set-up frame.
rw::MakeCalls MakeCalls(lifted::Regs& ctx) {
  return [&ctx](uint32_t setup_frame) { return StartCalls(ctx, setup_frame); };
}

}  // namespace

ITC_REGS_STEP(RaceWorldStream) {
  ctx.fpscr.disableFlushMode();
  rw::Stream(ctx.r3.u32, ctx.r1.u32, NativeHeapActive(), MakeCalls(ctx));
}

ITC_REGS_STEP(RaceWorldStreamNear) {
  ctx.fpscr.disableFlushMode();
  rw::StreamNear(ctx.r3.u32, ctx.r1.u32, NativeHeapActive(), MakeCalls(ctx));
}

ITC_REGS_STEP(RaceWorldStreamCruise) {
  ctx.fpscr.disableFlushMode();
  rw::StreamCruise(ctx.r3.u32, ctx.r1.u32, NativeHeapActive(), MakeCalls(ctx));
}

ITC_REGS_STEP(RaceWorldStreamTwoPlayers) {
  ctx.fpscr.disableFlushMode();
  rw::StreamTwoPlayers(ctx.r3.u32, ctx.r1.u32, NativeHeapActive(), MakeCalls(ctx));
}

ITC_REGS_STEP(RaceWorldTick) {
  ctx.fpscr.disableFlushMode();
  rw::Tick(ctx.r3.u32, ctx.r1.u32 - 128, NativeHeapActive(), MakeCalls(ctx));
}

RaceWorldStandIn g_recorded_stand_in = nullptr;

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_RACE_WORLD(address, step)     \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_race_world)) { \
    hooks::step(ctx);               \
  }

ITC_RACE_WORLD(821705A8, RaceWorldEntryOfRecord)
ITC_RACE_WORLD(82170860, RaceWorldBranchesMeet)
ITC_RACE_WORLD(82170AA0, RaceWorldSortedListInsert)
ITC_RACE_WORLD(82170A88, RaceWorldSortedListClear)
ITC_RACE_WORLD(82170560, RaceWorldOnPausedTick)
ITC_RACE_WORLD(82171360, RaceWorldRelease)
ITC_RACE_WORLD(82170F18, RaceWorldStepToMainRoad)
ITC_RACE_WORLD(82112190, RaceWorldFirstUnsetFlag)
ITC_RACE_WORLD(82174D28, RaceWorldInit)
ITC_RACE_WORLD(821713C0, RaceWorldReleaseForStage)
ITC_RACE_WORLD(82170668, RaceWorldResetWorld)
ITC_RACE_WORLD(82171718, RaceWorldResetVehicles)
ITC_RACE_WORLD(82171250, RaceWorldUnload)
ITC_RACE_WORLD(82171E50, RaceWorldListRacers)
ITC_RACE_WORLD(82173A90, RaceWorldOnLoaded)
ITC_RACE_WORLD(82165E20, RaceWorldPlace)
ITC_RACE_WORLD(821662E0, RaceWorldPlaceRolling)
ITC_RACE_WORLD(82174BC0, RaceWorldTick)
ITC_RACE_WORLD(82171978, RaceWorldRefresh)
// RaceManager::Stream (sub_82174338), TickVehicles (sub_821714D8) and
// RefreshVehicles (sub_82171560) have no adapters: their only caller,
// RaceWorld::Tick, is native; nor have StreamCruise (sub_82174200),
// StreamNear (sub_82172E60) and StreamTwoPlayers (sub_82172D20), whose only
// caller is Stream, StreamOpponents (sub_82172118) and StreamTraffic
// (sub_821724F8), whose callers are those three, MoveUnseen (sub_82173E28),
// from StreamCruise, JumpSections (sub_821737F8) and Neighbours
// (sub_82170920), from MoveUnseen, and Traffic::PickModel (sub_82171070),
// from StreamTraffic and RaceManager::AddRacers. --native_race_world
// switches RaceWorld::Tick (and RaceWorld::OnLoaded for AddRacers). Their
// register steps stay for the harness.
// sub_8217437C (RaceManager::Stream's case 0, split off: a tail call to
// StreamNear) has no adapter: its only caller is Stream, whose plain C++
// does its work.
// sub_821743A8 (never seen in play, from RaceWorld::Tick): the original, or
// the harness's stand-in while it's set (hooks::g_recorded_stand_in).
ITC_NATIVE_ENTRY_ALWAYS(821743A8) {
  
  if (hooks::g_recorded_stand_in != nullptr) {
    lifted::Sdk(ctx, hooks::g_recorded_stand_in);
    return;
  }
  lifted::original::c821743A8(ctx);
}
// RaceManager::GiveVehicle (sub_821706E8, never seen in play): the harness's
// stand-in while it's set (hooks::g_give_vehicle_stand_in); else its plain
// C++ (race_world::GiveVehicle) with --native_race_world, or the original.
ITC_NATIVE_ENTRY_ALWAYS(821706E8) {
  
  if (hooks::g_give_vehicle_stand_in != nullptr) {
    lifted::Sdk(ctx, hooks::g_give_vehicle_stand_in);
    return;
  }
  if (!ITC_FLAG(native_race_world)) {
    lifted::original::c821706E8(ctx);
    return;
  }
  lifted::Enter(ctx, [](const lifted::Args& a) -> lifted::Result {
    return {race_world::GiveVehicle(uint32_t(a.r1), a.r13, uint32_t(a.r[0]),
                                    uint32_t(a.r[1])),
            a.f[0]};
  });
}
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
// RaceManager::Create (sub_821715B8), AddRacers (sub_82173050) and
// NoteMarkedRacer (sub_82172F90) have no adapters: their only caller,
// RaceWorld::OnLoaded, is native (its original only without --native_heap);
// --native_race_world switches it. Their register steps stay for the
// harness.
// RaceManager::BuildNeighbors (sub_82170CB8), RaceEvent::PlaceStartsCurrent
// (sub_82144A38) and RaceEvent::Entry (sub_82143508) have no adapters: their
// only caller, RaceManager::AddRacers, is native; SaveData::AddDistance
// (sub_82116C38) has none either: its only caller, RaceWorld::Unload, is
// native (its original only without --native_heap). --native_race_world
// switches those callers. Their register steps stay for the harness.
