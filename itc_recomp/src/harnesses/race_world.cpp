// The race world and the racers' manager (src/race_world/,
// docs/functions/race-world.md) against the recompiled functions. The
// originals run with --native_race_world off (their callees' own flags as
// they are: those were checked by their harnesses), the plain C++ through
// the register steps (hooks/race_world.h). The inputs go in the scratch area
// and blocks from the allocation layer: a manager (its pointer at
// 0x826D82F4), the seven vehicles (0x826C2E34), the course's route
// (test_route.h, at 0x826C5B10), the race data's flags (0x826C5B2C) and the
// race event (0x826C6078) where a function reads them, with the game's
// globals put back after each side. Compared with harness::Outputs: every
// value exactly (the only floats are keys and fields copied as words or
// converted the same way), r3 and the stack temporaries the originals keep.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/test_route.h"
#include "hooks/race_world.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_world/race_world.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_world);
ITC_FLAG_DECLARE(bool, native_collision_world);

namespace harness {

// The collision world's harness's course (src/harnesses/collision_world.cpp).
uint32_t TestCourseBytes();
void FillTestCourseTable(uint8_t* base, uint32_t area, uint32_t table);
void BuildTestCourse(uint8_t* base, uint32_t area, uint32_t table, Random& r);

// The car physics' harness's world, vehicle and update world
// (src/harnesses/car_physics_objects.cpp).
namespace car_objects {
uint32_t WorldBytes();
uint32_t VehicleRegionBytes();
void BuildWorld(Context& h, uint32_t region, Random& r, bool keep_one_free = false);
void FillVehicleRegion(uint8_t* base, uint32_t region, Random& r, bool nans);
void FillStepInputs(uint8_t* base, uint32_t region, Random& r);
void VehicleRegionOutputs(Outputs& out, uint8_t* base, uint32_t region);
void FillStack(Context& h, Random& r);
uint32_t UpdateRegionBytes();
void BuildUpdateWorld(Context& h, uint32_t region, Random& r);
void UpdateWorldOutputs(Outputs& out, uint8_t* base, uint32_t region);
uint32_t UpdateObject(uint32_t region, uint32_t i);
std::vector<std::pair<uint32_t, uint32_t>> UpdateGlobals();
}  // namespace car_objects

// The vehicle's harness's tick inputs (src/harnesses/vehicle.cpp).
namespace vehicle_cases {
void FillTickInputs(Context& h, uint32_t region, uint32_t race, Random& r);
}  // namespace vehicle_cases

namespace {

namespace rw = race_world;

constexpr uint32_t kVehicles = 0x826C2E34;
constexpr uint32_t kMarkedRacer = 0x826C61A4;
constexpr uint32_t kManagerBytes = 0x60000;  // the manager is 363,592 bytes
constexpr uint32_t kVehiclesBytes = 7 * 6272;

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The original with --native_race_world off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_race_world) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_race_world) = true;
}

// The game's globals a case changes, put back by Restore.
class Globals {
 public:
  Globals(uint8_t* base, std::vector<uint32_t> addresses) : base_(base), addresses_(addresses) {
    for (uint32_t a : addresses_) {
      saved_.push_back(guest::LoadU32(base_, a));
    }
  }
  ~Globals() { Restore(); }
  void Restore() {
    for (size_t i = 0; i < addresses_.size(); i++) {
      guest::StoreU32(base_, addresses_[i], saved_[i]);
    }
  }

 private:
  uint8_t* base_;
  std::vector<uint32_t> addresses_;
  std::vector<uint32_t> saved_;
};

// --- The manager's leaves ---

void Leaves(Context& h, const Block& manager) {
  {
    Results results("RaceManager::EntryOfRecord (sub_821705A8)");
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 31 + 7);
        Globals globals(h.base, {rw::kManager});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, manager.address, 4096, r);
        const int32_t count = seed % 16 == 0 ? -int32_t(r.Below(3)) : int32_t(r.Below(85));
        guest::StoreU32(h.base, manager.address + 2368, uint32_t(count));
        for (uint32_t i = 0; i < 85; i++) {
          guest::StoreU32(h.base, manager.address + 4 + 28 * i + 24,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(12));
        }
        guest::StoreU32(h.base, rw::kManager, seed % 25 == 0 ? 0 : manager.address);
        PPCContext t = CallContext(h);
        t.r3.u64 = r.Below(8) == 0 ? r.Next() : r.Below(12);
        (v == 0 ? Original<__imp__sub_821705A8> : hooks::RaceWorldEntryOfRecord)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceManager::BranchesMeet (sub_82170860)");
    Random r(0x5EED);
    for (uint32_t a = 0; a < 34; a++) {
      for (uint32_t b = 0; b < 34; b++) {
        const uint64_t high_a = r.Below(4) == 0 ? (r.Next() & ~0xFFull) : 0;
        const uint64_t high_b = r.Below(4) == 0 ? (r.Next() & ~0xFFull) : 0;
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = a | high_a;
          t.r4.u64 = b | high_b;
          (v == 0 ? Original<__imp__sub_82170860> : hooks::RaceWorldBranchesMeet)(t, h.base);
          out[v].Word(t.r3.u32);
          out[v].Word(uint32_t(t.r3.u64 >> 32));
        }
        results.Check(out[0], out[1], fmt::format("{}, {}", a, b));
      }
    }
    results.Report();
  }
  {
    // The tables written below the stack pointer (a leaf without a frame).
    Results results("Traffic::PickModel (sub_82171070)");
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 37 + 3);
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(h.base, sp - 512, 512, r);
        t.r3.u64 = r.Below(4) == 0 ? r.Next() : r.Below(200);
        t.r4.u64 = r.Below(8) == 0 ? r.Next() : r.Below(20);
        (v == 0 ? Original<__imp__sub_82171070> : hooks::RaceWorldPickModel)(t, h.base);
        out[v].Bytes(Read(h.base, sp - 400, 336));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("SaveData::FirstUnsetFlag (sub_82112190)");
    for (uint64_t seed = 0; seed < 128; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 29 + 1);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t word = h.data + 4 * r.Below(1000);
        guest::StoreU32(h.base, word, seed < 64 ? uint32_t(seed) | (uint32_t(r.Next()) & ~63u)
                                                : uint32_t(r.Next()));
        PPCContext t = CallContext(h);
        t.r3.u64 = word - 6092;
        (v == 0 ? Original<__imp__sub_82112190> : hooks::RaceWorldFirstUnsetFlag)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceWorld::OnPausedTick (sub_82170560)");
    for (uint64_t seed = 0; seed < 4; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed + 11);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = r.Next();
        t.r4.u64 = r.Next();
        t.r11.u64 = r.Next();
        (v == 0 ? Original<__imp__sub_82170560> : hooks::RaceWorldOnPausedTick)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        for (uint64_t value : {t.r3.u64, t.r4.u64, t.r5.u64, t.r6.u64, t.r11.u64}) {
          out[v].Word(uint32_t(value));
          out[v].Word(uint32_t(value >> 32));
        }
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The sorted lists: in the scratch area, a run of inserts from a
// cleared list (keys from a small set, so some are equal, with NaNs,
// infinities and zeros of either sign), compared after each ---

void SortedLists(Context& h) {
  const uint32_t keys[] = {0x00000000, 0x80000000, 0x3F800000, 0x40000000, 0xBF800000,
                           0x42C80000, 0x7F800000, 0xFF800000, 0x7FC00000, 0xFFC00001};
  {
    Results results("SortedList::Insert (sub_82170AA0)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Random pick(seed * 53 + 5);
      const uint32_t inserts = 1 + pick.Below(100);
      std::vector<uint64_t> key_bits, items;
      for (uint32_t i = 0; i < inserts; i++) {
        const bool exact = pick.Below(4) != 0;
        key_bits.push_back(exact ? uint64_t(std::bit_cast<uint64_t>(double(
                                       std::bit_cast<float>(keys[pick.Below(seed < 32 ? 6 : 10)]))))
                                 : std::bit_cast<uint64_t>(double(pick.Below(1000)) / 7.0 - 50.0));
        items.push_back(pick.Next());
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 59 + 9);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t list = h.data + 16;
        guest::StoreU32(h.base, list + 1600, 0);
        guest::StoreU32(h.base, list + 1604, 0);
        guest::StoreU32(h.base, list + 1608, 0);
        for (uint32_t i = 0; i < inserts; i++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = list;
          t.r4.u64 = items[i];
          t.f1.u64 = key_bits[i];
          (v == 0 ? Original<__imp__sub_82170AA0> : hooks::RaceWorldSortedListInsert)(t, h.base);
          out[v].Word(t.r3.u32);
        }
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      }
      results.Check(out[0], out[1], fmt::format("seed {}: {} inserts", seed, inserts));
    }
    results.Report();
  }
  {
    Results results("SortedList::Clear (sub_82170A88)");
    for (uint64_t seed = 0; seed < 16; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 61 + 2);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 4 * r.Below(500);
        (v == 0 ? Original<__imp__sub_82170A88> : hooks::RaceWorldSortedListClear)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- RaceWorld::Release: a manager with racers whose entries name the
// seven vehicles ---

void Release(Context& h, const Block& manager, const Block& vehicles) {
  Results results("RaceWorld::Release (sub_82171360)");
  for (uint64_t seed = 0; seed < 64; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 67 + 4);
      Globals globals(h.base, {rw::kManager, kVehicles});
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, manager.address, 4096, r);
      WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
      const uint32_t racers = seed % 8 == 0 ? 0 : 1 + r.Below(7);
      guest::StoreU32(h.base, manager.address + 2364, racers);
      for (uint32_t i = 0; i < 7; i++) {
        const uint32_t entry = manager.address + 4 + 28 * r.Below(80);
        guest::StoreU32(h.base, entry + 20, r.Below(7));
        guest::StoreU32(h.base, manager.address + 2308 + 4 * i, entry);
      }
      guest::StoreU32(h.base, rw::kManager, manager.address);
      guest::StoreU32(h.base, kVehicles, vehicles.address);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data;
      (v == 0 ? Original<__imp__sub_82171360> : hooks::RaceWorldRelease)(t, h.base);
      out[v].Bytes(Read(h.base, vehicles.address, vehicles.bytes));
      out[v].Bytes(Read(h.base, manager.address, 4096));
      out[v].Word(t.r3.u32);  // Vehicle::Get's high word is the multiply's
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- The route positions and the race event: the course's route
// (test_route.h) at 0x826C5B10, the race data's flags for BranchMasksFor ---

void Routes(Context& h) {
  namespace tr = test_route;
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  const Block race(0x40);
  if (!area.address || !table.address || !race.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  const uint32_t flags[] = {0, 0x4, 0x8, 0x10, 0x20, 0x3C};
  const auto set_up = [&](Random& r, tr::Route& route) {
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, area.address, area.bytes, r);
    route = tr::BuildRoute(h.base, area.address, table.address, r);
    guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
    WriteRandom(h.base, race.address, race.bytes, r);
    guest::StoreU32(h.base, race.address + 20,
                    r.Below(4) == 0 ? uint32_t(r.Next()) : flags[r.Below(6)]);
    guest::StoreU32(h.base, rw::kRaceData, race.address);
  };
  {
    Results results("RoutePosition::ToMainRoad (sub_82143608)");
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 71 + 19);
        Globals globals(h.base, {track_route::kRoute, rw::kRaceData});
        tr::Route route;
        set_up(r, route);
        const uint32_t position = h.data + 4 * r.Below(256);
        tr::RandomPosition(h.base, position, route, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = position;
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
        (v == 0 ? Original<__imp__sub_82143608> : hooks::RaceWorldToMainRoad)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Bytes(Read(h.base, area.address, area.bytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RoutePosition::StepToMainRoad (sub_82170F18)");
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 73 + 23);
        Globals globals(h.base, {track_route::kRoute, rw::kRaceData});
        tr::Route route;
        set_up(r, route);
        const uint32_t position = h.data + 4 * r.Below(256);
        tr::RandomPosition(h.base, position, route, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = position;
        const int32_t steps[] = {-1, 1, 0, 2, -3};
        t.r4.u64 =
            uint64_t(int64_t(r.Below(4) == 0 ? int32_t(r.Below(40)) - 20 : steps[r.Below(5)]));
        t.r5.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(12)) - 1));
        (v == 0 ? Original<__imp__sub_82170F18> : hooks::RaceWorldStepToMainRoad)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Bytes(Read(h.base, area.address, area.bytes));
        out[v].Word(guest::LoadU32(h.base, CallContext(h).r1.u32 - 112 + 80));  // the kind
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The event in the scratch area, its list (+156) there too; the route
    // the block's (its +0 the file).
    Results results("RaceEvent::PlaceEntry (sub_82143150)");
    const uint32_t masks[] = {0, 0x82008970, 0x82008980, 0x82008990, 0x820089A0};
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 79 + 29);
        Globals globals(h.base, {track_route::kRoute, rw::kRaceData});
        tr::Route route;
        set_up(r, route);
        const uint32_t event = h.data;
        const uint32_t list = h.data + 1024;
        guest::StoreU32(h.base, event + 156, list);
        const uint32_t index = h.data + 200;
        guest::StoreU16(h.base, index, uint16_t(int32_t(r.Below(200)) - 20));
        const uint32_t position = h.data + 208;
        tr::RandomPosition(h.base, position, route, r);
        const uint32_t step = h.data + 212;
        guest::StoreU16(h.base, step, uint16_t(r.Below(4) == 0 ? r.Next() : r.Below(8)));
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        t.r4.u64 = area.address;
        t.r5.u64 = index;
        t.r6.u64 = position;
        t.r7.u64 = step;
        t.r8.u64 = masks[r.Below(5)];
        (v == 0 ? Original<__imp__sub_82143150> : hooks::RaceWorldPlaceEntry)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Bytes(Read(h.base, area.address, area.bytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceEvent::Entry (sub_82143508)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 83 + 31);
        Globals globals(h.base, {rw::kRaceEvent});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        guest::StoreU32(h.base, rw::kRaceEvent, h.data);
        const uint32_t index = h.data + 400;
        guest::StoreU32(h.base, index, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(40));
        PPCContext t = CallContext(h);
        t.r3.u64 = index;
        (v == 0 ? Original<__imp__sub_82143508> : hooks::RaceWorldRaceEventEntry)(t, h.base);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// A float's bits: now and then NaN (any sign, payload), an infinity or a
// zero of either sign; else a number in +-range.
uint32_t RandomFloat(Random& r, float range) {
  return test_route::RandomFloat(r, range);
}

// --- RaceManager::Create: each side's block compared where it writes, then
// freed; the global put back ---

void Create(Context& h) {
  Results results("RaceManager::Create (sub_821715B8)");
  for (uint64_t seed = 0; seed < 4; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Globals globals(h.base, {rw::kManager});
      PPCContext t = CallContext(h);
      (v == 0 ? Original<__imp__sub_821715B8> : hooks::RaceWorldCreate)(t, h.base);
      const uint32_t block = guest::LoadU32(h.base, rw::kManager);
      out[v].Word(block == t.r3.u32 ? 1 : 0);
      out[v].Word(block != 0 ? 1 : 0);
      if (block != 0) {
        out[v].Bytes(Read(h.base, block, 2392));
        out[v].Bytes(Read(h.base, block + 362392, 1200));
        memory::crt::OperatorDelete(block);
      }
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- The manager's tables over a random route (test_route.h): its
// sections' branch numbers now and then 0, 11, 12 or 29 ---

void Tables(Context& h, const Block& manager, const Block& vehicles) {
  namespace tr = test_route;
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  const Block race(7 * 3936 + 0x100);
  const Block items(8 * 3008);
  if (!area.address || !table.address || !race.address || !items.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  const auto route_up = [&](Random& r, tr::Route& route) {
    WriteRandom(h.base, area.address, area.bytes, r);
    route = tr::BuildRoute(h.base, area.address, table.address, r);
    const uint8_t branches[] = {0, 11, 12, 29, 7, 1};
    for (uint32_t i = 0; i < 8; i++) {
      if (r.Below(2) == 0) {
        guest::StoreU8(h.base, route.address[i] + 3, branches[r.Below(6)]);
      }
    }
    guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
  };
  const auto stack = [&](Random& r) {
    const uint32_t sp = CallContext(h).r1.u32;
    WriteRandom(h.base, sp - 2048, 2048, r);
    return sp;
  };
  {
    // The racers' vehicles' bodies in the scratch area; the records' flags
    // now and then 0x400.
    Results results("RaceManager::NoteMarkedRacer (sub_82172F90)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 89 + 37);
        Globals globals(h.base, {rw::kManager, kVehicles, rw::kRaceData, kMarkedRacer,
                                 kMarkedRacer + 4, kMarkedRacer + 8});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, manager.address, 4096, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        WriteRandom(h.base, race.address, race.bytes, r);
        const uint32_t sp = stack(r);
        const uint32_t racers = seed % 8 == 0 ? 0 : 1 + r.Below(7);
        guest::StoreU32(h.base, manager.address + 2364, racers);
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t entry = manager.address + 4 + 28 * r.Below(80);
          guest::StoreU32(h.base, entry + 20, r.Below(7));
          guest::StoreU32(h.base, entry + 24, r.Below(6));
          guest::StoreU32(h.base, manager.address + 2308 + 4 * i, entry);
          guest::StoreU32(h.base, vehicles.address + 6272 * i + 336,
                          h.data + 16 * r.Below(180) - 208 + 1024);
          const uint32_t record = race.address + 48 + 3936 * i;
          guest::StoreU32(h.base, record + 3888,
                          r.Below(3) == 0 ? 0x400 | uint32_t(r.Next()) : uint32_t(r.Next()) & ~0x400u);
        }
        for (uint32_t i = 0; i < 512; i++) {
          guest::StoreU32(h.base, h.data + 1024 + 4 * i, RandomFloat(r, 1000.0f));
        }
        guest::StoreU32(h.base, rw::kManager, manager.address);
        guest::StoreU32(h.base, kVehicles, vehicles.address);
        guest::StoreU32(h.base, rw::kRaceData, race.address);
        PPCContext t = CallContext(h);
        (v == 0 ? Original<__imp__sub_82172F90> : hooks::RaceWorldNoteMarkedRacer)(t, h.base);
        out[v].Bytes(Read(h.base, kMarkedRacer, 12));
        out[v].Bytes(Read(h.base, sp - 144 + 80, 16));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // Segments and points in the scratch area: near (within 1.0 to 200.0 of
    // the line), or random floats with NaNs and infinities.
    Results results("RaceManager::NearSegment (sub_82170BC0)");
    int hits = 0;
    for (uint64_t seed = 0; seed < 400; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 97 + 41);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t sp = stack(r);
        const uint32_t segment = h.data + 16 * r.Below(100);
        const uint32_t point = h.data + 2048 + 16 * r.Below(100);
        const bool wild = seed % 8 == 0;
        for (uint32_t i = 0; i < 8; i++) {
          guest::StoreU32(h.base, segment + 4 * i,
                          wild ? RandomFloat(r, 500.0f)
                               : std::bit_cast<uint32_t>(float(int32_t(r.Below(400)) - 200)));
        }
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, point + 4 * i,
                          wild ? RandomFloat(r, 500.0f)
                               : std::bit_cast<uint32_t>(float(int32_t(r.Below(400)) - 200)));
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = segment;
        t.r4.u64 = point;
        t.f1.f64 = r.Below(8) == 0 ? double(std::bit_cast<float>(RandomFloat(r, 300.0f)))
                                   : double(r.Below(300));
        (v == 0 ? Original<__imp__sub_82170BC0> : hooks::RaceWorldNearSegment)(t, h.base);
        hits += v == 1 && t.r3.u32 == 1 ? 1 : 0;
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Floats(h.base, sp - 208 + 80, 20);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    ITC_LOG_INFO("harness: race_world: NearSegment answered 1 in {} of 400 cases", hits);
    results.Report();
  }
  {
    // The table in the manager's block (the records and the counts).
    Results results("RaceManager::BuildNeighbors (sub_82170CB8)");
    int neighbors = 0;
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 101 + 43);
        Globals globals(h.base, {track_route::kRoute});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, manager.address, 8 * 1200, r);
        WriteRandom(h.base, manager.address + 360000, 64, r);
        tr::Route route;
        route_up(r, route);
        const uint32_t sp = stack(r);
        PPCContext t = CallContext(h);
        t.r3.u64 = manager.address;
        (v == 0 ? Original<__imp__sub_82170CB8> : hooks::RaceWorldBuildNeighbors)(t, h.base);
        for (uint32_t i = 0; v == 1 && i < route.sections; i++) {
          neighbors += guest::LoadU32(h.base, manager.address + 360000 + 4 * i) > 1 ? 1 : 0;
        }
        out[v].Bytes(Read(h.base, manager.address, 8 * 1200));
        out[v].Bytes(Read(h.base, manager.address + 360000, 64));
        out[v].Bytes(Read(h.base, sp - 336 + 80, 4));
        out[v].Floats(h.base, sp - 336 + 80, 32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    ITC_LOG_INFO("harness: race_world: BuildNeighbors found neighbors for {} sections", neighbors);
    results.Report();
  }
  {
    // The manager's entries on the route, their flags among 0x1, 0x4, 0x8,
    // 0x20.
    Results results("RaceManager::Neighbours (sub_82170920)");
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 103 + 47);
        Globals globals(h.base, {track_route::kRoute});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, manager.address, 4096, r);
        tr::Route route;
        route_up(r, route);
        const uint32_t count = seed % 16 == 0 ? r.Below(2) : 1 + r.Below(40);
        guest::StoreU32(h.base, manager.address + 2368, count);
        const uint32_t flags[] = {0x1, 0x1, 0x1, 0x21, 0x5, 0x9, 0x0, 0x41};
        for (uint32_t i = 0; i < 41; i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          guest::StoreU32(h.base, entry, r.Below(8) == 0 ? uint32_t(r.Next()) : flags[r.Below(8)]);
          tr::RandomPosition(h.base, entry + 8, route, r);
          if (r.Below(16) == 0) {
            guest::StoreU16(h.base, entry + 8, uint16_t(0x8000 | r.Below(0x8000)));
          }
        }
        const uint32_t index = h.data + 64;
        guest::StoreU32(h.base, index, r.Below(count + 1));
        PPCContext t = CallContext(h);
        t.r3.u64 = manager.address;
        t.r4.u64 = index;
        (v == 0 ? Original<__imp__sub_82170920> : hooks::RaceWorldNeighbours)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // An entry in the manager's block on a section of 0 to 299 (the route's
    // table maps each to one of its eight), its record in the race data
    // naming an item of kind 2 or 3 (now and then another, or none), the
    // player near or far, on its branch or not.
    Results results("RaceManager::JumpSections (sub_821737F8)");
    int jumps = 0;
    for (uint64_t seed = 0; seed < 600; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 107 + 53);
        Globals globals(h.base, {track_route::kRoute, rw::kRaceData, data_tables::kTables});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, manager.address, 256, r);
        WriteRandom(h.base, race.address, race.bytes, r);
        tr::Route route;
        route_up(r, route);
        const uint32_t sp = stack(r);
        const uint32_t tables = h.data + 3072;
        guest::StoreU32(h.base, tables + 16, items.address);
        for (uint32_t i = 0; i < 8; i++) {
          const uint8_t kinds[] = {2, 3, 2, 3, 0, 1, 0xFE};
          guest::StoreU8(h.base, items.address + 3008 * i + 36, kinds[r.Below(7)]);
        }
        guest::StoreU32(h.base, data_tables::kTables, tables);
        guest::StoreU32(h.base, rw::kRaceData, race.address);
        const uint32_t entry = manager.address + 4;
        const uint32_t racer = r.Below(6);
        guest::StoreU32(h.base, entry + 24, racer);
        guest::StoreU32(h.base, race.address + 3936 * racer + 3960,
                        (uint32_t(r.Next()) & 0xFFFF0000u) |
                            (r.Below(8) == 0 ? 0xFFFF : r.Below(8)));
        const uint32_t sections[] = {40, 46, 50, 53, 79, 83, 87, 187, 189, 191, 193, 194, 195,
                                     197, 199, 201, 203, 204, 251, 254, 256, 258, 262, 263, 266};
        guest::StoreU16(h.base, entry + 8, uint16_t(r.Below(3) == 0 ? r.Below(300)
                                                                    : sections[r.Below(25)]));
        guest::StoreU16(h.base, entry + 10, uint16_t(r.Below(8)));
        const uint32_t player = h.data + 128;
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, player + 4 * i, RandomFloat(r, 150.0f));
        }
        const uint32_t branch = h.data + 160;
        const uint32_t its =
            guest::LoadU8(h.base, route.Address(guest::LoadU16(h.base, entry + 8)) + 3);
        const uint32_t pick[] = {its, its, 1, 0, 12};
        guest::StoreU32(h.base, branch, pick[r.Below(5)]);
        guest::StoreU32(h.base, h.data + 164, r.Below(3) == 0 ? 0 : 1 + r.Below(4));
        guest::StoreU32(h.base, h.data + 168, r.Below(3) == 0 ? 0 : 1 + r.Below(4));
        PPCContext t = CallContext(h);
        t.r3.u64 = entry;
        t.r4.u64 = player;
        t.r5.u64 = branch;
        t.r6.u64 = h.data + 164;
        t.r7.u64 = h.data + 168;
        const uint32_t flags = guest::LoadU32(h.base, h.data + 164) | guest::LoadU32(h.base, h.data + 168);
        (v == 0 ? Original<__imp__sub_821737F8> : hooks::RaceWorldJumpSections)(t, h.base);
        jumps += v == 1 && flags != (guest::LoadU32(h.base, h.data + 164) |
                                     guest::LoadU32(h.base, h.data + 168)) ? 1 : 0;
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Bytes(Read(h.base, manager.address, 256));
        out[v].Bytes(Read(h.base, sp - 176 + 80, 4));
        out[v].Floats(h.base, sp - 176 + 96, 8);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    ITC_LOG_INFO("harness: race_world: JumpSections moved the entry in {} of 600 cases", jumps);
    results.Report();
  }
}

// Guest memory ranges a case changes, put back by the destructor.
class SavedBytes {
 public:
  SavedBytes(uint8_t* base, std::vector<std::pair<uint32_t, uint32_t>> ranges)
      : base_(base), ranges_(std::move(ranges)) {
    for (const auto& [address, bytes] : ranges_) {
      saved_.push_back(Read(base_, address, bytes));
    }
  }
  ~SavedBytes() {
    for (size_t i = 0; i < ranges_.size(); i++) {
      std::memcpy(base_ + ranges_[i].first, saved_[i].data(), ranges_[i].second);
    }
  }

 private:
  uint8_t* base_;
  std::vector<std::pair<uint32_t, uint32_t>> ranges_;
  std::vector<std::vector<uint8_t>> saved_;
};

// --- The racers' vehicles each tick: one racer (or none) whose vehicle is
// the car physics' vehicle region (the seven vehicles' global pointed so
// that vehicle k is the region), the vehicle harness's tick inputs and
// course, the racer's record in the race data with its car fields ---

void VehicleLoops(Context& h, const Block& manager) {
  constexpr uint32_t kInputs = 0x826C5B30, kLinkedTried = 0x826C5B04;
  const Block world(car_objects::WorldBytes());
  const Block region(car_objects::VehicleRegionBytes());
  const Block course(TestCourseBytes());
  const Block table(4 * 0x10000);
  const Block race(48 + 3936 * 8);
  if (!world.address || !region.address || !course.address || !table.address ||
      !race.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, course.address, table.address);
  const auto racer_up = [&](Random& r, uint32_t racers, uint32_t k, uint32_t record) {
    WriteRandom(h.base, manager.address, 4096, r);
    const uint32_t entry = manager.address + 4 + 28 * r.Below(80);
    guest::StoreU32(h.base, manager.address + 2364, racers);
    guest::StoreU32(h.base, manager.address + 2308, entry);
    guest::StoreU32(h.base, entry + 20, k);
    guest::StoreU32(h.base, entry + 24, record);
    guest::StoreU32(h.base, rw::kManager, manager.address);
    guest::StoreU32(h.base, kVehicles, region.address - 6272 * k);
  };
  {
    Results results("RaceManager::TickVehicles (sub_821714D8)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      const uint32_t k = uint32_t(seed % 7);
      for (int v = 0; v < 2; v++) {
        Random r(seed * 113 + 59);
        SavedBytes saved(h.base, {{track_collision::kMesh, 12}, {track_route::kRoute, 4},
                                  {rw::kRaceData, 4}, {0x826C2D88, 4}, {kLinkedTried, 16},
                                  {kInputs + 4 * k, 4}, {rw::kManager, 4}, {kVehicles, 4}});
        WriteRandom(h.base, course.address, course.bytes, r);
        BuildTestCourse(h.base, course.address, table.address, r);
        car_objects::BuildWorld(h, world.address, r);
        car_objects::FillVehicleRegion(h.base, region.address, r, false);
        car_objects::FillStepInputs(h.base, region.address, r);
        car_objects::FillStack(h, r);
        WriteRandom(h.base, race.address, race.bytes, r);
        vehicle_cases::FillTickInputs(h, region.address, race.address, r);
        const uint32_t record = r.Below(7);
        const uint32_t car = race.address + 48 + 3936 * record;
        guest::StoreU32(h.base, car + 1656, r.Below(12));
        guest::StoreU32(h.base, car + 1664, r.Below(6));
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t input = h.data + 0x100;
        guest::StoreU32(h.base, input + 0, uint32_t(r.Next()) & 0x82010200u);
        guest::StoreU8(h.base, input + 17, uint8_t(r.Below(2) == 0 ? 0 : 2));
        guest::StoreU8(h.base, region.address + 2249, 0);
        guest::StoreU32(h.base, kInputs + 4 * k, input);
        racer_up(r, seed % 10 == 0 ? 0 : 1, k, record);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        (v == 0 ? Original<__imp__sub_821714D8> : hooks::RaceWorldTickVehicles)(t, h.base);
        car_objects::VehicleRegionOutputs(out[v], h.base, region.address);
        out[v].Word(guest::LoadU32(h.base, region.address + 6160 + 8));
        out[v].Bytes(Read(h.base, input, 20));
        out[v].Bytes(Read(h.base, kLinkedTried, 16));
      }
      results.Check(out[0], out[1], fmt::format("seed {}: vehicle {}", seed, k));
    }
    results.Report();
  }
  {
    Results results("RaceManager::RefreshVehicles (sub_82171560)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      const uint32_t k = uint32_t(seed % 7);
      for (int v = 0; v < 2; v++) {
        Random r(seed * 127 + 61);
        SavedBytes saved(h.base, {{0x826C2D88, 4}, {rw::kManager, 4}, {kVehicles, 4}});
        car_objects::BuildWorld(h, world.address, r);
        car_objects::FillVehicleRegion(h.base, region.address, r, seed % 2 == 1);
        for (uint32_t w = 0; w < 4; w++) {
          guest::StoreU32(h.base, region.address + 416 + 368 * w + 20, 0);
        }
        car_objects::FillStack(h, r);
        racer_up(r, seed % 10 == 0 ? 0 : 1, k, 0);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        (v == 0 ? Original<__imp__sub_82171560> : hooks::RaceWorldRefreshVehicles)(t, h.base);
        car_objects::VehicleRegionOutputs(out[v], h.base, region.address);
      }
      results.Check(out[0], out[1], fmt::format("seed {}: vehicle {}", seed, k));
    }
    results.Report();
  }
}

// --- TrackCollision::Tick: the car physics' update world, the course's
// mesh and route marked loaded (or not), one to three racers whose
// vehicles (in the seven vehicles' block) have the world's objects, the
// race data's car count and flags (CarsCollide) ---

void TrackCollisionTicks(Context& h, const Block& manager, const Block& vehicles) {
  const Block world(car_objects::UpdateRegionBytes());
  const Block race(0x5A000);
  if (!world.address || !race.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  Results results("TrackCollision::Tick (sub_820FF050)");
  int contacts = 0, landings = 0;
  for (uint64_t seed = 0; seed < 150; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 131 + 67);
      auto ranges = car_objects::UpdateGlobals();
      ranges.insert(ranges.end(), {{track_collision::kMesh, 12}, {track_route::kRoute, 12},
                                   {rw::kRaceData, 4}, {rw::kManager, 4}, {kVehicles, 4}});
      SavedBytes saved(h.base, ranges);
      car_objects::BuildUpdateWorld(h, world.address, r);
      // The contacts' chains (+68) made to end: each the next record or none
      // (the update links the ones it makes in front).
      for (uint32_t i = 0; i < 8; i++) {
        const uint32_t contact = world.address + 0x800 + 80 * i;
        guest::StoreU32(h.base, contact + 68, i < 7 && r.Below(2) == 0 ? contact + 80 : 0);
      }
      car_objects::FillStack(h, r);
      guest::StoreU32(h.base, track_collision::kMesh + 8, seed % 16 == 1 ? 1 : 2);
      guest::StoreU32(h.base, track_route::kRoute + 8, seed % 16 == 2 ? 0 : 2);
      WriteRandom(h.base, race.address, 48 + 3936 * 4, r);
      const uint32_t records = race.address + 48;
      guest::StoreU32(h.base, records + 0x595E0, 1 + r.Below(3));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(h.base, records + 3888 + 3936 * i,
                        r.Below(4) == 0 ? 0x8 : uint32_t(r.Next()) & ~0x8u);
      }
      guest::StoreU32(h.base, rw::kRaceData, race.address);
      WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
      WriteRandom(h.base, manager.address, 4096, r);
      const uint32_t racers = seed % 10 == 0 ? 0 : 1 + r.Below(3);
      guest::StoreU32(h.base, manager.address + 2364, racers);
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t entry = manager.address + 4 + 28 * (10 * i + r.Below(10));
        guest::StoreU32(h.base, manager.address + 2308 + 4 * i, entry);
        guest::StoreU32(h.base, entry + 20, i);
        const uint32_t vehicle = vehicles.address + 6272 * i;
        guest::StoreU32(h.base, vehicle + 340, car_objects::UpdateObject(world.address, r.Below(4)));
        guest::StoreU32(h.base, vehicle + 348, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
        guest::StoreU32(h.base, vehicle + 6240, r.Below(20));
      }
      guest::StoreU32(h.base, rw::kManager, manager.address);
      guest::StoreU32(h.base, kVehicles, vehicles.address);
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      ITC_FLAG(native_collision_world) = v == 1;
      (v == 0 ? __imp__sub_820FF050 : sub_820FF050)(t, h.base);
      ITC_FLAG(native_collision_world) = true;
      car_objects::UpdateWorldOutputs(out[v], h.base, world.address);
      for (uint32_t i = 0; i < 3; i++) {
        const uint32_t vehicle = vehicles.address + 6272 * i;
        out[v].Word(guest::LoadU32(h.base, vehicle + 6152));
        contacts += v == 1 && i < racers && (guest::LoadU32(h.base, vehicle + 6152) & 3) ? 1 : 0;
        landings += v == 1 && i < racers && (guest::LoadU32(h.base, vehicle + 6152) & 4) ? 1 : 0;
        out[v].Word(guest::LoadU32(h.base, vehicle + 6232));
        out[v].Float(guest::LoadU32(h.base, vehicle + 6236));
        out[v].Word(guest::LoadU32(h.base, vehicle + 6240));
      }
      const uint32_t frame = CallContext(h).r1.u32 - 144 - 176;  // the update's
      out[v].Word(guest::LoadU32(h.base, frame + 80));
      out[v].Word(guest::LoadU32(h.base, frame + 84));
      out[v].Floats(h.base, frame + 96, 8);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  ITC_LOG_INFO("harness: race_world: TrackCollision::Tick: {} racers with contacts, {} landings",
               contacts, landings);
  results.Report();
}

// --- RaceWorld::Init: an empty state machine or one with random
// transitions already (fewer than 150, so the 24 fit), the race data's mode
// 6 with the current stage's +76 of 0 to 3 or another mode; the original's
// frame filled with the same random words for both (the words it never
// writes go into the transitions); the machine, the frame (but its back
// chain and saved registers) and r3 compared ---

void Inits(Context& h) {
  namespace gm = game_modes;
  constexpr uint32_t kFrame = 1696;
  const Block world(0x2600);
  const Block race(0xA4000);
  if (!world.address || !race.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  Results results("RaceWorld::Init (sub_82174D28)");
  const uint32_t frame = CallContext(h).r1.u32 - kFrame;
  for (uint64_t seed = 0; seed < 24; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 137 + 71);
      Globals globals(h.base, {rw::kRaceData});
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, world.address, world.bytes, r);
      WriteRandom(h.base, frame, kFrame, r);
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(h.base, world.address);
        guest::StoreU32(h.base, world.address + gm::kTransitionCount, 0);
      } else {
        guest::StoreU32(h.base, world.address + gm::kTransitionCount, r.Below(150));
        for (uint32_t i = 0; i < 200; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, world.address + 48 * i + 4 * k, r.Below(k < 2 ? 60 : 2));
          }
        }
      }
      const uint32_t stages = race.address + 0x59620;
      const uint32_t current = r.Below(10);
      guest::StoreU32(h.base, stages + 804, current);
      guest::StoreU32(h.base, stages + 80 * current + 76, r.Below(4));
      guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(2) == 0 ? 6 : r.Below(8));
      guest::StoreU32(h.base, rw::kRaceData, race.address);
      PPCContext t = CallContext(h);
      t.r3.u64 = world.address;
      (v == 0 ? Original<__imp__sub_82174D28> : hooks::RaceWorldInit)(t, h.base);
      out[v].Bytes(Read(h.base, world.address, 9612));
      out[v].Bytes(Read(h.base, frame + 80, kFrame - 80 - 64));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// --- RaceWorld::ReleaseForStage: a manager with racers on the seven
// vehicles and entries for records 0 to 11, the race data's stages (the
// current one's type, count and racers) and mode ---

void ReleasesForStage(Context& h, const Block& manager, const Block& vehicles) {
  const Block race(0xA4000);
  if (!race.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  Results results("RaceWorld::ReleaseForStage (sub_821713C0)");
  int held = 0;
  for (uint64_t seed = 0; seed < 100; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 139 + 73);
      Globals globals(h.base, {rw::kManager, kVehicles, rw::kRaceData});
      const uint32_t sp = CallContext(h).r1.u32;
      WriteRandom(h.base, sp - 1024, 1024, r);
      WriteRandom(h.base, manager.address, 4096, r);
      WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
      guest::StoreU32(h.base, manager.address + 2364, r.Below(8));
      for (uint32_t i = 0; i < 7; i++) {
        const uint32_t entry = manager.address + 4 + 28 * r.Below(12);
        guest::StoreU32(h.base, manager.address + 2308 + 4 * i, entry);
      }
      guest::StoreU32(h.base, manager.address + 2368, 12);
      for (uint32_t i = 0; i < 12; i++) {
        guest::StoreU32(h.base, manager.address + 4 + 28 * i + 20, r.Below(7));
        guest::StoreU32(h.base, manager.address + 4 + 28 * i + 24, i);
      }
      const uint32_t stages = race.address + 0x59620;
      WriteRandom(h.base, stages, 820, r);
      const uint32_t current = r.Below(10);
      const uint32_t stage = stages + 80 * current;
      guest::StoreU32(h.base, stages + 804, current);
      guest::StoreU32(h.base, stage + 0, r.Below(5));
      guest::StoreU32(h.base, stage + 76, r.Below(5));
      for (uint32_t k = 0; k < 4; k++) {
        guest::StoreU32(h.base, stage + 44 + 8 * k, r.Below(12));
      }
      guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(2) == 0 ? 6 : r.Below(8));
      guest::StoreU32(h.base, rw::kManager, manager.address);
      guest::StoreU32(h.base, kVehicles, vehicles.address);
      guest::StoreU32(h.base, rw::kRaceData, race.address);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data;
      (v == 0 ? Original<__imp__sub_821713C0> : hooks::RaceWorldReleaseForStage)(t, h.base);
      for (uint32_t i = 0; i < 7; i++) {
        out[v].Bytes(Read(h.base, vehicles.address + 6272 * i + 2384, 1));
        held += v == 1 && guest::LoadU8(h.base, vehicles.address + 6272 * i + 2384) == 1 ? 1 : 0;
      }
      out[v].Bytes(Read(h.base, vehicles.address, vehicles.bytes));
      out[v].Bytes(Read(h.base, sp - 192 + 80, 80));  // the stage's copy
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  ITC_LOG_INFO("harness: race_world: ReleaseForStage held {} vehicles", held);
  results.Report();
}

// --- SaveData::AddDistance: a save data block (the garage list at +6120,
// its nodes' links and car numbers small, the picked count 0 to 5), the
// three floats it adds to compared as floats (NaNs and infinities among
// them), the rest exactly ---

void AddDistances(Context& h) {
  const Block save(0x20000);
  if (!save.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  Results results("SaveData::AddDistance (sub_82116C38)");
  const uint32_t list = save.address + 6120;
  const auto car = [&] {
    uint32_t node = (uint32_t(int32_t(int16_t(guest::LoadU16(h.base, list + 204)))) << 2) + list;
    for (uint32_t n = guest::LoadU32(h.base, save.address + 0x19920); n != 0; n--) {
      node = (uint32_t(int32_t(int16_t(guest::LoadU16(h.base, node)))) << 2) + list;
    }
    return uint32_t(int32_t(int16_t(guest::LoadU16(h.base, node + 2)))) * 1968 + list;
  };
  for (uint64_t seed = 0; seed < 200; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 149 + 79);
      WriteRandom(h.base, save.address, save.bytes, r);
      for (uint32_t i = 0; i < 64; i++) {
        guest::StoreU16(h.base, list + 4 * i, uint16_t(r.Below(64)));
        guest::StoreU16(h.base, list + 4 * i + 2, uint16_t(r.Below(30)));
      }
      guest::StoreU16(h.base, list + 204, uint16_t(r.Below(64)));
      guest::StoreU32(h.base, save.address + 0x19920, r.Below(6));
      const uint32_t picked = car() + 2104;
      const bool wild = seed % 4 == 0;
      for (uint32_t at : {save.address + 4908, save.address + 6040, picked}) {
        guest::StoreU32(h.base, at,
                        wild ? RandomFloat(r, 1e6f) : std::bit_cast<uint32_t>(float(r.Below(100000))));
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = save.address;
      t.f1.f64 = wild ? double(std::bit_cast<float>(RandomFloat(r, 1e6f)))
                      : double(r.Below(200000)) / 3.0;
      (v == 0 ? Original<__imp__sub_82116C38> : hooks::RaceWorldAddDistance)(t, h.base);
      std::vector<uint8_t> bytes = Read(h.base, save.address, save.bytes);
      for (uint32_t at : {save.address + 4908, save.address + 6040, picked}) {
        out[v].Float(guest::LoadU32(h.base, at));
        std::memset(bytes.data() + (at - save.address), 0, 4);
      }
      out[v].Bytes(bytes);
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceWorld(Context& h) {
  const Block manager(kManagerBytes);
  const Block vehicles(kVehiclesBytes);
  if (!manager.address || !vehicles.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  Leaves(h, manager);
  SortedLists(h);
  Release(h, manager, vehicles);
  Routes(h);
  Create(h);
  Tables(h, manager, vehicles);
  VehicleLoops(h, manager);
  TrackCollisionTicks(h, manager, vehicles);
  Inits(h);
  ReleasesForStage(h, manager, vehicles);
  AddDistances(h);
}

}  // namespace harness
