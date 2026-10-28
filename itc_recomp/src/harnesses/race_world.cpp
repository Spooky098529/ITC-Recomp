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
#include "ai_drivers/ai_drivers.h"
#include "car_physics/float_ops.h"
#include "hooks/ai_drivers.h"
#include "hooks/heap.h"
#include "hooks/race_states.h"
#include "hooks/race_world.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_world/race_world.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cstring>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_world);
ITC_FLAG_DECLARE(bool, native_collision_world);
ITC_FLAG_DECLARE(bool, native_race_states);

namespace harness {

// The collision world's harness's course (src/harnesses/collision_world.cpp).
uint32_t TestCourseBytes();
void FillTestCourseTable(uint8_t* base, uint32_t area, uint32_t table);
void BuildTestCourse(uint8_t* base, uint32_t area, uint32_t table, Random& r);

// The car physics' harness's world, vehicle and update world
// (src/harnesses/car_physics_objects.cpp).
namespace car_objects {
uint32_t WorldBytes();
uint32_t WorldActiveObject(uint8_t* base, uint32_t region);
void BuildBox(Context& h, uint32_t box, Random& r);
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
void FillSetUpCase(Context& h, uint32_t world, uint32_t region, uint32_t car, uint32_t frame,
                   uint32_t r1, uint64_t seed, bool reset, Random& r,
                   std::vector<uint32_t>& given);
bool SetUpCaseSane(Context& h, uint32_t region, uint32_t car, uint32_t frame);
void SetUpCaseOutputs(Outputs& o, Context& h, uint32_t world, uint32_t region, uint32_t car,
                      uint32_t frame, const std::vector<uint32_t>& given);
void FreeSetUpCase(const std::vector<uint32_t>& given);
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

// --- The race event's start placements: the course's route (test_route.h)
// at 0x826C5B10, the race data's flags, the event in the scratch area (its
// list at +156, 12-byte entries; the groups' lists at the event + 12 x
// (group + 3), for groups -3 to 5, each with 0 to several 4-byte entries),
// [0x826C6078] the event and player 0's save ([0x826C5B58]) for
// SaveData::FirstUnsetFlag; the stack below the scratch area filled the
// same on both sides, and each function's frame temporaries compared (the
// originals' register saves and back chains aren't written natively). The
// originals' r3 isn't compared: no caller reads it. ---

void Placements(Context& h) {
  namespace tr = test_route;
  constexpr uint32_t kSaves = 0x826C5B58;  // SaveData::Of's table
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  const Block race(0x40);
  if (!area.address || !table.address || !race.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  const uint32_t flags[] = {0, 0x4, 0x8, 0x10, 0x20, 0x3C};
  const uint32_t sp = CallContext(h).r1.u32;
  const uint32_t event = h.data;
  const uint32_t list = h.data + 1536;
  const uint32_t save_word = h.data + 900;  // the save's +6092
  const uint32_t player = h.data + 904;
  const uint32_t kind = h.data + 908;
  const uint32_t step = h.data + 912;
  const uint32_t group = h.data + 916;
  const uint32_t flag = h.data + 920;
  const uint32_t row = h.data + 924;
  // Everything a case reads, random; the event's groups -3 to 5.
  const auto set_up = [&](Random& r, tr::Route& route) {
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, sp - 2048, 2048 + 1024, r);
    WriteRandom(h.base, area.address, area.bytes, r);
    route = tr::BuildRoute(h.base, area.address, table.address, r);
    guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
    WriteRandom(h.base, race.address, race.bytes, r);
    guest::StoreU32(h.base, race.address + 20,
                    r.Below(4) == 0 ? uint32_t(r.Next()) : flags[r.Below(6)]);
    guest::StoreU32(h.base, rw::kRaceData, race.address);
    guest::StoreU32(h.base, rw::kRaceEvent, event);
    guest::StoreU32(h.base, kSaves, save_word - 6092);
    const uint32_t words[] = {0x3F, 0x3E, 0, 0x1F, 0x7};
    guest::StoreU32(h.base, save_word,
                    r.Below(2) == 0 ? uint32_t(r.Next()) : words[r.Below(5)]);
    guest::StoreU32(h.base, event + 156, list);
    for (uint32_t slot = 0; slot < 9; slot++) {
      const uint32_t at = event + 12 * slot;
      const uint32_t entries = h.data + 256 + 64 * slot;
      const int32_t count = r.Below(8) == 0 ? int32_t(r.Below(20)) - 4
                            : r.Below(4) == 0 ? 0
                                              : int32_t(1 + r.Below(8));
      guest::StoreU16(h.base, at + 0, uint16_t(count));
      guest::StoreU32(h.base, at + 4, entries);
      for (uint32_t i = 0; i < 16; i++) {
        guest::StoreU16(h.base, entries + 4 * i + 2,
                        uint16_t(r.Below(6) == 0 ? int32_t(r.Below(20)) - 20
                                                 : int32_t(r.Below(40))));
      }
    }
    tr::RandomPosition(h.base, player, route, r);
    guest::StoreU16(h.base, step, uint16_t(r.Below(4) == 0 ? r.Next() : r.Below(8)));
  };
  const auto stack = [&](Outputs& out, uint32_t at, uint32_t bytes) {
    out.Bytes(Read(h.base, at, bytes));
  };
  const auto globals = [&] {
    return Globals(h.base, {track_route::kRoute, rw::kRaceData, rw::kRaceEvent, kSaves});
  };
  const auto common = [&](Outputs& out) {
    out.Bytes(Read(h.base, h.data, kScratchBytes));
    out.Bytes(Read(h.base, area.address, area.bytes));
  };
  // A kind: 0 to 19 in turn, now and then above 19.
  const auto random_kind = [&](Random& r, uint64_t seed) {
    return r.Below(8) == 0 ? (r.Below(2) ? 20 + r.Below(20) : uint32_t(r.Next()))
                           : uint32_t(seed % 20);
  };
  {
    Results results("RaceEvent::PlaceStartsCurrent (sub_82144A38)");
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 89 + 37);
        auto g = globals();
        tr::Route route;
        set_up(r, route);
        guest::StoreU32(h.base, kind, random_kind(r, seed));
        PPCContext t = CallContext(h);
        t.r3.u64 = kind;
        t.r4.u64 = player;
        (v == 0 ? Original<__imp__sub_82144A38> : hooks::RaceWorldPlaceStartsCurrent)(t, h.base);
        common(out[v]);
        stack(out[v], sp - 176 + 24, 16);
        stack(out[v], sp - 176 + 80, 32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceEvent::PlaceStarts (sub_82144798)");
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 97 + 41);
        auto g = globals();
        tr::Route route;
        set_up(r, route);
        guest::StoreU32(h.base, kind, random_kind(r, seed));
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        t.r4.u64 = kind;
        t.r5.u64 = player;
        (v == 0 ? Original<__imp__sub_82144798> : hooks::RaceWorldPlaceStarts)(t, h.base);
        common(out[v]);
        stack(out[v], sp - 176 + 24, 16);
        stack(out[v], sp - 176 + 80, 32);
      }
      results.Check(out[0], out[1], fmt::format("seed {} kind {}", seed, seed % 20));
    }
    results.Report();
  }
  {
    Results results("RaceEvent::PlaceGroupFixed (sub_821443B0)");
    const uint32_t groups[] = {0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 0xFFFFFFFF};
    for (uint64_t seed = 0; seed < 220; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 101 + 43);
        auto g = globals();
        tr::Route route;
        set_up(r, route);
        guest::StoreU32(h.base, group, groups[seed % 11]);
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        t.r4.u64 = step;
        t.r5.u64 = group;
        (v == 0 ? Original<__imp__sub_821443B0> : hooks::RaceWorldPlaceGroupFixed)(t, h.base);
        common(out[v]);
        stack(out[v], sp - 96 + 24, 16);
        stack(out[v], sp - 96 + 80, 8);
      }
      results.Check(out[0], out[1], fmt::format("seed {} group {}", seed, groups[seed % 11]));
    }
    results.Report();
  }
  {
    Results results("RaceEvent::PlaceGroup (sub_82143A40)");
    for (uint64_t seed = 0; seed < 300; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 103 + 47);
        auto g = globals();
        tr::Route route;
        set_up(r, route);
        const uint32_t position = h.data + 928;
        tr::RandomPosition(h.base, position, route, r);
        guest::StoreU32(h.base, group, uint32_t(int32_t(seed % 9) - 3));
        guest::StoreU32(h.base, flag, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        t.r4.u64 = position;
        t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(8);
        t.r6.u64 = group;
        t.r7.u64 = flag;
        (v == 0 ? Original<__imp__sub_82143A40> : hooks::RaceWorldPlaceGroup)(t, h.base);
        common(out[v]);
        stack(out[v], sp - 192 + 80, 4);
        stack(out[v], sp + 38, 2);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceEvent::PlaceGroupTwoRows (sub_82143B78)");
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 107 + 53);
        auto g = globals();
        tr::Route route;
        set_up(r, route);
        const uint32_t positions = h.data + 928;
        tr::RandomPosition(h.base, positions, route, r);
        tr::RandomPosition(h.base, positions + 4, route, r);
        guest::StoreU32(h.base, group, uint32_t(int32_t(seed % 9) - 3));
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        t.r4.u64 = guest::LoadU32(h.base, positions);
        t.r5.u64 = guest::LoadU32(h.base, positions + 4);
        t.r6.u64 = step;
        t.r7.u64 = group;
        (v == 0 ? Original<__imp__sub_82143B78> : hooks::RaceWorldPlaceGroupTwoRows)(t,
                                                                                     h.base);
        common(out[v]);
        stack(out[v], sp + 28, 12);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceEvent::PlaceGroupSplit (sub_82143920)");
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 109 + 59);
        auto g = globals();
        tr::Route route;
        set_up(r, route);
        const uint32_t position = h.data + 928;
        tr::RandomPosition(h.base, position, route, r);
        guest::StoreU32(h.base, group, uint32_t(int32_t(seed % 9) - 3));
        guest::StoreU32(h.base, row, r.Below(6));
        PPCContext t = CallContext(h);
        t.r3.u64 = event;
        t.r4.u64 = guest::LoadU32(h.base, position);
        t.r5.u64 = step;
        t.r6.u64 = group;
        t.r7.u64 = row;
        (v == 0 ? Original<__imp__sub_82143920> : hooks::RaceWorldPlaceGroupSplit)(t, h.base);
        common(out[v]);
        stack(out[v], sp + 28, 4);
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

// --- The racers' vehicles reset and the race world unloaded (only with
// --native_heap: the adapters' condition) ---

constexpr uint32_t kResetFloat = 0x826C2D80;
constexpr uint32_t kSurfaces = 0x826C2DF0, kSurfacesBytes = 64;
constexpr uint32_t kWorldPointer = 0x826C2D88;
constexpr uint32_t kSaves = 0x826C5B58;
constexpr uint32_t kRacerInputs = 0x826C5B30;
constexpr uint32_t kTried = 0x826C5B04;
constexpr uint32_t kDriversBytes = 7 * ai_drivers::kDriverBytes;

// The seven AI drivers as AiDriver::Setup's cases make them (racer
// controllers' harness): random words, each driver's index +0, its vehicle
// +160, +16, the path's driver +512 and modes +5520 to +5528.
void FillDrivers(uint8_t* base, uint32_t table, uint32_t vehicle, uint32_t vehicle_step,
                 Random& r) {
  WriteRandom(base, table, kDriversBytes, r);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t driver = table + ai_drivers::kDriverBytes * i;
    guest::StoreU32(base, driver + 0, i);
    guest::StoreU32(base, driver + 160, vehicle + vehicle_step * i);
    guest::StoreU32(base, driver + 16, r.Below(2) ? uint32_t(r.Next()) : r.Below(8));
    guest::StoreU32(base, driver + 512, r.Below(4) == 0 ? 0 : driver);
    guest::StoreU32(base, driver + 512 + 5520, r.Below(3));
    guest::StoreU32(base, driver + 512 + 5524, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
    guest::StoreU32(base, driver + 512 + 5528, r.Below(2) ? 0xFFFFFFFFu : r.Below(8));
  }
}

// RaceManager::ResetWorld: the car physics' fake world, the seven vehicles
// (random; those whose slot word is above 0 with a box shape of their own
// or none and one of the world's active objects or none), the manager's
// slot words (-1, 0, 1, 5 or random), the seven AI drivers. Compared: the
// world, the vehicles' fields RemoveFromWorld writes (pointers by number),
// which shapes were freed, the slots and +2388, the drivers, 0x826C2D80,
// the surface settings and the frame's index.
void ResetWorlds(Context& h, const Block& manager, const Block& vehicles) {
  const Block world(car_objects::WorldBytes());
  const Block drivers(kDriversBytes);
  if (!world.address || !drivers.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  Results results("RaceManager::ResetWorld (sub_82170668)");
  const uint32_t frame = CallContext(h).r1.u32 - 128;
  for (uint64_t seed = 0; seed < 150; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 157 + 83);
      SavedBytes saved(h.base, {{kWorldPointer, 4}, {kVehicles, 4}, {ai_drivers::kDrivers, 4},
                                {kResetFloat, 4}, {kSurfaces, kSurfacesBytes}});
      car_objects::BuildWorld(h, world.address, r);
      WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
      WriteRandom(h.base, manager.address, 4096, r);
      std::vector<uint32_t> active;
      for (uint32_t o = car_objects::WorldActiveObject(h.base, world.address); o != 0;
           o = guest::LoadU32(h.base, o + 228)) {
        active.push_back(o);
      }
      std::vector<uint32_t> shapes;
      for (uint32_t i = 0; i < 7; i++) {
        const uint32_t slots[] = {0xFFFFFFFFu, 0, 1, 5, uint32_t(r.Next())};
        const uint32_t slot = slots[r.Below(5)];
        guest::StoreU32(h.base, manager.address + 2336 + 4 * i, slot);
        if (int32_t(slot) <= 0) {
          continue;
        }
        const uint32_t vehicle = vehicles.address + 6272 * i;
        uint32_t shape = 0, object = 0;
        if (r.Below(4) != 0) {
          shape = memory::Alloc(496, 0);
          car_objects::BuildBox(h, shape, r);
          shapes.push_back(shape);
        }
        if (!active.empty() && r.Below(2) == 0) {
          const uint32_t k = r.Below(uint32_t(active.size()));
          object = active[k];
          active.erase(active.begin() + k);
        }
        guest::StoreU32(h.base, vehicle + 340, object);
        guest::StoreU32(h.base, vehicle + 344, shape);
      }
      FillDrivers(h.base, drivers.address, vehicles.address, 6272, r);
      WriteRandom(h.base, kSurfaces, kSurfacesBytes, r);
      guest::StoreU32(h.base, kVehicles, vehicles.address);
      guest::StoreU32(h.base, ai_drivers::kDrivers, drivers.address);
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      t.r3.u64 = manager.address;
      (v == 0 ? Original<__imp__sub_82170668> : hooks::RaceWorldResetWorld)(t, h.base);
      const auto name = [&](uint32_t w) {
        for (size_t i = 0; i < shapes.size(); i++) {
          if (w == shapes[i]) {
            return uint32_t(0xB10C0000u + i);
          }
        }
        return w;
      };
      out[v].Bytes(Read(h.base, world.address, world.bytes));
      for (uint32_t i = 0; i < 7; i++) {
        const uint32_t vehicle = vehicles.address + 6272 * i;
        out[v].Word(name(guest::LoadU32(h.base, vehicle + 340)));
        out[v].Word(name(guest::LoadU32(h.base, vehicle + 344)));
        out[v].Word(guest::LoadU32(h.base, vehicle + 6148));
      }
      for (uint32_t s : shapes) {
        out[v].Word(memory::IsHeapBlock(s) ? 1 : 0);
        if (memory::IsHeapBlock(s)) {
          memory::Free(s);
        }
      }
      out[v].Bytes(Read(h.base, manager.address + 2336, 28));
      out[v].Word(guest::LoadU32(h.base, manager.address + 2388));
      for (uint32_t i = 0; i < kDriversBytes; i += 4) {
        out[v].Word(name(guest::LoadU32(h.base, drivers.address + i)));
      }
      out[v].Float(guest::LoadU32(h.base, kResetFloat));
      out[v].Bytes(Read(h.base, kSurfaces, kSurfacesBytes));
      out[v].Word(guest::LoadU32(h.base, frame + 80));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// RaceManager::ResetVehicles: one vehicle set up again (or none), on a
// racer's entry (0 to 7 of them) or one of the three traffic entries, the
// others' +20 -1; the vehicle in the car physics' vehicle region with the
// car (racer record 0 to 2 of the race data) as the vehicle harness's
// Vehicle::SetUp and ResetForCar cases make them (vehicle_cases::
// FillSetUpCase: the fake world, the course, the car data; the slot word -1
// for SetUp, else ResetForCar), the item id at the record's +3960, the
// seven AI drivers on that vehicle with the data tables' items, the race
// data's mode, an input record. Cases whose car data the gearing's set-up
// would read out of its tables with are left out, as there. Compared: what
// the vehicle harness compares (vehicle_cases::SetUpCaseOutputs), the
// manager, the input record, the drivers (the new body and box by name)
// and the frame's temporaries.
void ResetVehicleCases(Context& h, const Block& manager) {
  const Block world(car_objects::WorldBytes());
  const Block region(car_objects::VehicleRegionBytes());
  const Block course(TestCourseBytes());
  const Block table(4 * 0x10000);
  const Block race(0xA4000);
  const Block drivers(kDriversBytes);
  const Block tables(0x40 + 4 * 3008);
  if (!world.address || !region.address || !course.address || !table.address ||
      !race.address || !drivers.address || !tables.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, course.address, table.address);
  const uint32_t r1 = CallContext(h).r1.u32;
  const uint32_t frame = r1 - 224;
  const uint32_t setup_frame = frame - 224;
  const uint32_t input = race.address + 0xA3000;
  const uint32_t items = tables.address + 0x40;
  Results results("RaceManager::ResetVehicles (sub_82171718)");
  uint32_t left_out = 0, set_up = 0;
  for (uint64_t seed = 0; seed < 240; seed++) {
    Outputs out[2];
    bool skip = false;
    for (int v = -1; v < 2 && !skip; v++) {
      Random r(seed * 163 + 89);
      const uint32_t k = r.Below(7);
      SavedBytes saved(h.base, {{track_collision::kMesh, 12}, {track_route::kRoute, 4},
                                {kTried, 16}, {rw::kManager, 4}, {kVehicles, 4},
                                {rw::kRaceData, 4}, {ai_drivers::kDrivers, 4},
                                {data_tables::kTables, 4}, {kWorldPointer, 4},
                                {kRacerInputs + 4 * k, 4}});
      WriteRandom(h.base, course.address, course.bytes, r);
      BuildTestCourse(h.base, course.address, table.address, r);
      const uint32_t slots[] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1};
      const uint32_t slot = slots[r.Below(4)];
      const uint32_t record = r.Below(3);
      const uint32_t car = race.address + 48 + 3936 * record;
      std::vector<uint32_t> given;
      vehicle_cases::FillSetUpCase(h, world.address, region.address, car, setup_frame, frame,
                                   seed, slot != 0xFFFFFFFFu, r, given);
      guest::StoreU32(h.base, car + 3912,
                      r.Below(4) == 0 ? 0xFFFFFFFFu - r.Below(3) : r.Below(4));
      if (v < 0) {
        const bool sane = vehicle_cases::SetUpCaseSane(h, region.address, car, setup_frame);
        vehicle_cases::FreeSetUpCase(given);
        if (!sane) {
          skip = true;
          left_out++;
        }
        continue;
      }
      WriteRandom(h.base, manager.address, 4096, r);
      const uint32_t count = r.Below(8);
      guest::StoreU32(h.base, manager.address + 2368, count);
      for (uint32_t i = 0; i < 80; i++) {
        guest::StoreU32(h.base, manager.address + 4 + 28 * i + 20, 0xFFFFFFFFu);
      }
      uint32_t entry = 0;
      if (seed % 10 != 0) {
        entry = count == 0 || r.Below(3) == 0 ? manager.address + 2160 + 28 * r.Below(3)
                                              : manager.address + 4 + 28 * r.Below(count);
        guest::StoreU32(h.base, entry + 20, k);
        guest::StoreU32(h.base, entry + 24, record);
        set_up += v == 1 ? 1 : 0;
      }
      guest::StoreU32(h.base, manager.address + 2336 + 4 * k, slot);
      const uint32_t flags[] = {0, 1, uint32_t(r.Next())};
      const uint32_t flag = flags[r.Below(3)];
      guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(3));
      WriteRandom(h.base, input, 20, r);
      FillDrivers(h.base, drivers.address, region.address, 0, r);
      guest::StoreU32(h.base, tables.address + 16, items);
      for (uint32_t n = 0; n < 4; n++) {
        for (uint32_t i = 0; i < 7; i++) {
          guest::StoreU16(h.base, items + 3008 * n + 120 + 2 * i,
                          uint16_t(int32_t(r.Below(300)) - 100));
        }
      }
      guest::StoreU32(h.base, rw::kManager, manager.address);
      guest::StoreU32(h.base, kVehicles, region.address - 6272 * k);
      guest::StoreU32(h.base, rw::kRaceData, race.address);
      guest::StoreU32(h.base, ai_drivers::kDrivers, drivers.address);
      guest::StoreU32(h.base, data_tables::kTables, tables.address);
      guest::StoreU32(h.base, kRacerInputs + 4 * k, input);
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      t.r3.u64 = manager.address;
      t.r4.u64 = flag;
      (v == 0 ? Original<__imp__sub_82171718> : hooks::RaceWorldResetVehicles)(t, h.base);
      const uint32_t body = guest::LoadU32(h.base, region.address + 336);
      const uint32_t shape = guest::LoadU32(h.base, region.address + 344);
      const auto name = [&](uint32_t w) {
        return w == 0 ? 0 : w == body ? 0xB0D10000u : w == shape ? 0xB0C50000u : w;
      };
      out[v].Bytes(Read(h.base, manager.address, 2400));
      out[v].Bytes(Read(h.base, input, 20));
      for (uint32_t i = 0; i < kDriversBytes; i += 4) {
        out[v].Word(name(guest::LoadU32(h.base, drivers.address + i)));
      }
      out[v].Bytes(Read(h.base, frame + 80, 68));
      vehicle_cases::SetUpCaseOutputs(out[v], h, world.address, region.address, car,
                                      setup_frame, given);
    }
    if (!skip) {
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
  }
  results.Report();
  ITC_LOG_INFO("harness: race_world: RaceManager::ResetVehicles: {} cases set a vehicle up, {} "
               "left out (car data the gearing's set-up would read out of its tables with)",
               set_up, left_out);
}

// RaceWorld::Unload: the race data's mode (1, 0 or 6) and kind (0 to 3),
// player 0's save data with its garage list (as SaveData::AddDistance's
// cases), the seven vehicles (a block of the layer's without bodies,
// objects or shapes) or none, the AI drivers' table (random bytes from
// operator new) or none, the collision world (no objects, or 1 to 3
// without box copies; its blocks +36 and +48 or none), the mesh's and the
// route's pages, the object at 0x826C5B28 or none, and the manager (from
// operator new) or none. Compared: the globals (the blocks by number),
// which blocks were freed and the save data's distances.
void Unloads(Context& h) {
  const Block race(0xA4000);
  const Block save(0x20000);
  if (!race.address || !save.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  constexpr uint32_t kGlobals = 0x826C5B10, kGlobalsBytes = 28;  // route, mesh, the object
  Results results("RaceWorld::Unload (sub_82171250)");
  const uint32_t list = save.address + 6120;
  const auto picked_car = [&] {
    uint32_t node = (uint32_t(int32_t(int16_t(guest::LoadU16(h.base, list + 204)))) << 2) + list;
    for (uint32_t n = guest::LoadU32(h.base, save.address + 0x19920); n != 0; n--) {
      node = (uint32_t(int32_t(int16_t(guest::LoadU16(h.base, node)))) << 2) + list;
    }
    return uint32_t(int32_t(int16_t(guest::LoadU16(h.base, node + 2)))) * 1968 + list;
  };
  uint32_t added = 0;
  for (uint64_t seed = 0; seed < 80; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 167 + 97);
      SavedBytes saved(h.base, {{rw::kRaceData, 4}, {kSaves, 4}, {kVehicles, 4},
                                {ai_drivers::kDrivers, 4}, {kWorldPointer, 4},
                                {kGlobals, kGlobalsBytes}, {rw::kManager, 4}});
      std::vector<uint32_t> heap, layer, pages;
      const auto block = [&](uint32_t size, bool array) {
        const auto b = array ? memory::crt::OperatorNewArray(h.base, size)
                             : memory::crt::OperatorNew(h.base, size);
        heap.push_back(*b);
        WriteRandom(h.base, *b, size, r);
        return *b;
      };
      const auto page = [&](uint32_t size) {
        const uint32_t p = memory::AllocPages(size, 4096);
        pages.push_back(p);
        return p;
      };
      // The race data and the save data.
      const uint32_t modes[] = {1, 1, 0, 6};
      const bool vehicles = r.Below(8) != 0;
      guest::StoreU32(h.base, race.address + 0xA3AF0, vehicles ? modes[r.Below(4)] : 0);
      guest::StoreU32(h.base, race.address + 0xA3AF4, r.Below(4));
      WriteRandom(h.base, save.address, save.bytes, r);
      for (uint32_t i = 0; i < 64; i++) {
        guest::StoreU16(h.base, list + 4 * i, uint16_t(r.Below(64)));
        guest::StoreU16(h.base, list + 4 * i + 2, uint16_t(r.Below(30)));
      }
      guest::StoreU16(h.base, list + 204, uint16_t(r.Below(64)));
      guest::StoreU32(h.base, save.address + 0x19920, r.Below(6));
      for (uint32_t at : {save.address + 4908, save.address + 6040, picked_car() + 2104}) {
        guest::StoreFloat(h.base, at, float(r.Below(100000)));
      }
      guest::StoreU32(h.base, rw::kRaceData, race.address);
      guest::StoreU32(h.base, kSaves, save.address);
      // The vehicles.
      uint32_t seven = 0;
      if (vehicles) {
        seven = memory::Alloc(kVehiclesBytes, 0);
        layer.push_back(seven);
        WriteRandom(h.base, seven, kVehiclesBytes, r);
        for (uint32_t i = 0; i < 7; i++) {
          for (uint32_t at : {336u, 340u, 344u}) {
            guest::StoreU32(h.base, seven + 6272 * i + at, 0);
          }
        }
        guest::StoreFloat(h.base, seven + 6244, float(r.Below(200000)) / 3.0f);
      }
      guest::StoreU32(h.base, kVehicles, seven);
      guest::StoreU32(h.base, ai_drivers::kDrivers,
                      r.Below(5) == 0 ? 0 : block(kDriversBytes, false));
      // The collision world, the mesh and the route.
      const uint32_t world = block(0x100, false);
      if (r.Below(4) == 0) {
        guest::StoreU32(h.base, world + 16, 0);
      } else {
        const uint32_t count = 1 + r.Below(3);
        const uint32_t objects = block(16 + 320 * count, true) + 16;
        guest::StoreU32(h.base, objects - 16, count);
        guest::StoreU32(h.base, world + 16, objects);
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t object = objects + 320 * i;
          guest::StoreU32(h.base, object + 0, 0x82002A94);
          guest::StoreU32(h.base, object + 36, 0);
          guest::StoreU32(h.base, object + 40, 0);
        }
      }
      guest::StoreU32(h.base, world + 36, r.Below(4) == 0 ? 0 : block(64, true));
      guest::StoreU32(h.base, world + 48, r.Below(4) == 0 ? 0 : block(64, true));
      guest::StoreU32(h.base, kWorldPointer, world);
      const uint32_t scratch = page(8);
      guest::StoreU32(h.base, scratch + 0, 0);
      guest::StoreU32(h.base, scratch + 4, page(8192));
      guest::StoreU32(h.base, track_collision::kMesh + 0, scratch);
      guest::StoreU32(h.base, track_collision::kMesh + 4, page(0x10000));
      guest::StoreU32(h.base, track_collision::kMesh + 8, 2);
      guest::StoreU32(h.base, track_route::kRoute + 0, page(0x10000));
      guest::StoreU32(h.base, track_route::kRoute + 4, uint32_t(r.Next()));
      guest::StoreU32(h.base, track_route::kRoute + 8, 2);
      guest::StoreU32(h.base, 0x826C5B28, r.Below(4) == 0 ? 0 : block(4, false));
      guest::StoreU32(h.base, rw::kManager, r.Below(6) == 0 ? 0 : block(0x1000, false));
      const bool adds = guest::LoadU32(h.base, race.address + 0xA3AF0) == 1 &&
                        guest::LoadU32(h.base, race.address + 0xA3AF4) >= 2;
      added += v == 1 && adds ? 1 : 0;
      PPCContext t = CallContext(h);
      t.fpscr.disableFlushModeUnconditional();
      t.r3.u64 = uint32_t(r.Next());
      (v == 0 ? Original<__imp__sub_82171250> : hooks::RaceWorldUnload)(t, h.base);
      const auto name = [&](uint32_t w) {
        for (size_t i = 0; i < heap.size(); i++) {
          if (w == heap[i]) {
            return uint32_t(0xB10C0000u + i);
          }
        }
        for (size_t i = 0; i < layer.size(); i++) {
          if (w == layer[i]) {
            return uint32_t(0x1A7E0000u + i);
          }
        }
        for (size_t i = 0; i < pages.size(); i++) {
          if (w == pages[i]) {
            return uint32_t(0x9A9E0000u + i);
          }
        }
        return w;
      };
      for (uint32_t at : {rw::kManager, kVehicles, ai_drivers::kDrivers, kWorldPointer}) {
        out[v].Word(name(guest::LoadU32(h.base, at)));
      }
      for (uint32_t i = 0; i < kGlobalsBytes; i += 4) {
        out[v].Word(name(guest::LoadU32(h.base, kGlobals + i)));
      }
      for (uint32_t at : {save.address + 4908, save.address + 6040, picked_car() + 2104}) {
        out[v].Float(guest::LoadU32(h.base, at));
      }
      for (uint32_t b : heap) {
        const bool left = memory::IsHeapBlock(b);
        out[v].Word(left ? 1 : 0);
        if (left) {
          memory::crt::Free(b);
        }
      }
      for (uint32_t b : layer) {
        const bool left = memory::IsHeapBlock(b);
        out[v].Word(left ? 1 : 0);
        if (left) {
          memory::Free(b);
        }
      }
      for (uint32_t p : pages) {
        const bool left = memory::IsPages(p);
        out[v].Word(left ? 1 : 0);
        if (left) {
          memory::FreePages(p);
        }
      }
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
  ITC_LOG_INFO("harness: race_world: RaceWorld::Unload: {} cases added the distance", added);
}

// --- The manager's lists: RaceManager::ListRacers and AddRacers over the
// random route (test_route.h, its branches now and then 0, 11, 12, 29, 7 or
// 1), the seven vehicles, a race data block (the racer records from +48,
// the stages at +0x59620, the traffic count, flags, spacing, mode and kind),
// the race event (its groups and list as the placements' cases make them)
// or none, player 0's save, the data tables' items, the seven AI drivers
// (FillDrivers) and the racers' input records. A player always: without
// one, both versions read the zero page (its +20 at 20, its position at
// 3904), which is protected. The stack below the scratch area filled the
// same on both sides; each original's frame temporaries compared (their
// vectors and the sorted lists' keys as floats). ---

// RaceManager::GiveVehicle's stand-in for StreamOpponents' random cases
// (the original would set a vehicle up from random words): its arguments
// and stack pointer logged; as the original, the first slot word (+2336)
// that's 0 or -1 taken (the entry's +20 its index, the slot 1, flags |=
// 0x240) and 1 answered, or 0 with none; now and then 0 with one free.
std::vector<uint32_t> g_give_log;
uint32_t g_give_calls = 0;
void StandInGiveVehicle(PPCContext& ctx, uint8_t* base) {
  g_give_log.insert(g_give_log.end(), {ctx.r3.u32, ctx.r4.u32, ctx.r1.u32});
  const uint32_t manager = ctx.r3.u32, entry = ctx.r4.u32;
  ctx.r3.u64 = 0;
  if (((g_give_calls++ * 0x9E3779B9u + entry) >> 28) < 3) {
    return;
  }
  for (uint32_t k = 0; k < 7; k++) {
    const uint32_t slot = guest::LoadU32(base, manager + 2336 + 4 * k);
    if (slot == 0 || slot == 0xFFFFFFFFu) {
      guest::StoreU32(base, entry + 20, k);
      guest::StoreU32(base, entry, guest::LoadU32(base, entry) | 0x240);
      guest::StoreU32(base, manager + 2336 + 4 * k, 1);
      ctx.r3.u64 = 1;
      return;
    }
  }
}
struct GiveVehicleStandIn {
  GiveVehicleStandIn() {
    hooks::g_give_vehicle_stand_in = StandInGiveVehicle;
    g_give_log.clear();
    g_give_calls = 0;
  }
  ~GiveVehicleStandIn() { hooks::g_give_vehicle_stand_in = nullptr; }
};

void ManagerLists(Context& h, const Block& manager, const Block& vehicles) {
  namespace tr = test_route;
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  const Block race(0xA4000);
  const Block drivers(kDriversBytes);
  const Block tables(0x40 + 4 * 3008);
  const Block event(4096);
  const Block save(8192);
  const Block inputs(8 * 32);
  if (!area.address || !table.address || !race.address || !drivers.address ||
      !tables.address || !event.address || !save.address || !inputs.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  const uint32_t sp = CallContext(h).r1.u32;
  const uint32_t items = tables.address + 0x40;
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
  // The globals both functions read, saved for each side.
  const auto saved = [&] {
    return SavedBytes(h.base, {{track_route::kRoute, 4}, {rw::kRaceData, 4},
                               {rw::kRaceEvent, 4}, {kSaves, 4}, {kVehicles, 4},
                               {ai_drivers::kDrivers, 4}, {data_tables::kTables, 4},
                               {kRacerInputs, 32}});
  };
  // The drivers, the items, the inputs, the save and the globals.
  const auto common = [&](Random& r) {
    WriteRandom(h.base, sp - 4096, 4096, r);
    FillDrivers(h.base, drivers.address, vehicles.address, 6272, r);
    WriteRandom(h.base, tables.address, tables.bytes, r);
    guest::StoreU32(h.base, tables.address + 16, items);
    for (uint32_t n = 0; n < 4; n++) {
      const uint8_t kinds[] = {0, 1, 2, 3, 4, 5, 0xFF, 0xFE};
      guest::StoreU8(h.base, items + 3008 * n + 36, kinds[r.Below(8)]);
      for (uint32_t i = 0; i < 7; i++) {
        guest::StoreU16(h.base, items + 3008 * n + 120 + 2 * i,
                        uint16_t(int32_t(r.Below(300)) - 100));
      }
    }
    WriteRandom(h.base, inputs.address, inputs.bytes, r);
    for (uint32_t k = 0; k < 8; k++) {
      guest::StoreU32(h.base, kRacerInputs + 4 * k, inputs.address + 32 * k);
    }
    WriteRandom(h.base, save.address, save.bytes, r);
    guest::StoreU32(h.base, kSaves, save.address);
    guest::StoreU32(h.base, kVehicles, vehicles.address);
    guest::StoreU32(h.base, ai_drivers::kDrivers, drivers.address);
    guest::StoreU32(h.base, data_tables::kTables, tables.address);
    guest::StoreU32(h.base, rw::kRaceData, race.address);
  };
  const auto drivers_out = [&](Outputs& out) {
    for (uint32_t i = 0; i < kDriversBytes; i += 4) {
      out.Word(guest::LoadU32(h.base, drivers.address + i));
    }
    out.Bytes(Read(h.base, inputs.address, inputs.bytes));
  };
  // A sorted list's nodes (the keys as floats) and its first, last and
  // count.
  const auto list_out = [&](Outputs& out, uint32_t list) {
    for (uint32_t k = 0; k < 100; k++) {
      const uint32_t node = list + 16 * k;
      out.Word(guest::LoadU32(h.base, node + 0));
      out.Float(guest::LoadU32(h.base, node + 4));
      out.Word(guest::LoadU32(h.base, node + 8));
      out.Word(guest::LoadU32(h.base, node + 12));
    }
    out.Bytes(Read(h.base, list + 1600, 12));
  };
  {
    // The manager's entries (1 to 20) with flags among 0x4 (the player: the
    // first so flagged, first or later), 0x1, 0x40, 0x20, 0x80, 0x10, 0x400,
    // 0x8, 0x100 and random; vehicles or none (-1), slot words -1, 0, 1 or
    // random; the traffic entries in use or not; the vehicles losing theirs
    // with a box shape of their own (only with --native_heap) or none.
    // Compared: the manager, the vehicles' +340, +344 (by name), +6148,
    // which shapes were freed, the frame and r3.
    Results results("RaceManager::ListRacers (sub_82171E50)");
    const uint32_t frame = sp - 1872;
    uint32_t lost = 0, listed = 0;
    for (uint64_t seed = 0; seed < 400; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 167 + 97);
        auto globals = saved();
        common(r);
        tr::Route route;
        route_up(r, route);
        WriteRandom(h.base, manager.address, 2400, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        const uint32_t count = 1 + r.Below(20);
        guest::StoreU32(h.base, manager.address + 2368, count);
        const uint32_t player = r.Below(2) == 0 ? 0 : r.Below(count);
        uint32_t with_vehicles = 0;
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          const uint32_t flags[] = {0x1, 0x41, 0x21, 0x60, 0x80, 0x10, 0x400, 0x0,
                                    0x8, 0x101, 0x40, 0x4};
          uint32_t f = r.Below(8) == 0 ? uint32_t(r.Next()) : flags[r.Below(12)];
          if (i < player) {
            f &= ~0x4u;
          } else if (i == player) {
            f = 0x4 | (r.Below(2) ? 0x40 : uint32_t(r.Next()) & ~0xA0u);
          }
          guest::StoreU32(h.base, entry, f);
          tr::RandomPosition(h.base, entry + 8, route, r);
          // At most seven entries with a vehicle (the racers' list from
          // +2308 holds seven before the counts).
          const bool vehicle = i == player || (with_vehicles < 6 && r.Below(3) != 0);
          with_vehicles += i != player && vehicle ? 1 : 0;
          guest::StoreU32(h.base, entry + 20, vehicle ? r.Below(7) : 0xFFFFFFFFu);
        }
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t slots[] = {0xFFFFFFFFu, 0, 1, 1, uint32_t(r.Next())};
          guest::StoreU32(h.base, manager.address + 2336 + 4 * k, slots[r.Below(5)]);
        }
        for (uint32_t t = 0; t < 3; t++) {
          guest::StoreU32(h.base, manager.address + 2160 + 28 * t,
                          r.Below(2) ? 0 : uint32_t(r.Next()));
        }
        std::vector<uint32_t> shapes;
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          uint32_t shape = 0;
          if (hooks::NativeHeapActive() && r.Below(3) == 0) {
            shape = memory::Alloc(496, 0);
            car_objects::BuildBox(h, shape, r);
            shapes.push_back(shape);
          }
          guest::StoreU32(h.base, vehicle + 340, 0);
          guest::StoreU32(h.base, vehicle + 344, shape);
          tr::RandomPosition(h.base, vehicle + 5236, route, r);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager.address;
        (v == 0 ? Original<__imp__sub_82171E50> : hooks::RaceWorldListRacers)(t, h.base);
        const auto name = [&](uint32_t w) {
          for (size_t i = 0; i < shapes.size(); i++) {
            if (w == shapes[i]) {
              return uint32_t(0xB10C0000u + i);
            }
          }
          return w;
        };
        if (v == 1) {
          listed += guest::LoadU32(h.base, manager.address + 2364);
          for (uint32_t i = 0; i < 7; i++) {
            lost += guest::LoadU32(h.base, vehicles.address + 6272 * i + 344) == 0 ? 1 : 0;
          }
        }
        out[v].Bytes(Read(h.base, manager.address, 2400));
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 340)));
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 344)));
          out[v].Word(guest::LoadU32(h.base, vehicle + 6148));
        }
        for (uint32_t s : shapes) {
          out[v].Word(memory::IsHeapBlock(s) ? 1 : 0);
          if (memory::IsHeapBlock(s)) {
            memory::Free(s);
          }
        }
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Floats(h.base, frame + 96, 12);
        list_out(out[v], frame + 144);
        out[v].Word(name(t.r3.u32));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: ListRacers listed {} entries, {} vehicles without a "
                 "shape after",
                 listed, lost);
  }
  {
    // The race data's records (1 to 14; the player, flagged 0x1, first or
    // later) with flags among 0x2, 0x80, 0x82, 0x4, 0x0, 0x102, 0x3, 0x8,
    // 0x402, 0x180 and random; their start positions (or none, 0xFFFF),
    // items (0 to 3 or none) and event entries (0 to 7 or none: none without
    // an event); the current stage's racers; the mode (0, 1 or 6) and kind
    // (0 to 3); the event or none; the save's +6100, +6108 and flags; the
    // traffic count (0 to 5 or random), the flag 0x2000, the spacing; the
    // manager's entries' count 0 (as RaceManager::Create leaves it) or 1 to
    // 2, its traffic records' 0 or 1. Compared: the manager (its entries,
    // lists, slots and neighbor table), the records' start positions, the
    // event, the drivers, the input records and the frame. The traffic
    // entries' +16 are 0 (as RaceManager::Create leaves them) or 1 to 42,
    // the spacing 0 to 19, so each traffic car's number (the spacing's sum
    // mod 10, plus its +16 and its index) is 0 or more, as in play; the
    // group "(expected to differ)" gives them random words: a negative
    // number has Traffic::PickModel read below its tables, where the
    // original reads what earlier calls left on the stack (their register
    // saves), which native callees don't write on the guest stack (Stack
    // leftovers in docs/loops/expected-differences.md).
    const uint32_t frame = sp - 2000;
    uint32_t racers = 0, traffic = 0, events = 0;
    for (const bool wild : {false, true}) {
      Results results(wild ? "RaceManager::AddRacers, random traffic numbers (expected to differ)"
                           : "RaceManager::AddRacers (sub_82173050)");
      for (uint64_t seed = wild ? 1000 : 0; seed < (wild ? 1200u : 500u); seed++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(seed * 173 + 101);
          auto globals = saved();
          common(r);
          tr::Route route;
          route_up(r, route);
          WriteRandom(h.base, manager.address, 2400, r);
          WriteRandom(h.base, manager.address + 2392, 8 * 1200, r);
          WriteRandom(h.base, manager.address + 360000, 64, r);
          guest::StoreU32(h.base, manager.address + 2364, 0);
          const uint32_t existing = r.Below(4) == 0 ? 1 + r.Below(2) : 0;
          guest::StoreU32(h.base, manager.address + 2368, existing);
          guest::StoreU32(h.base, manager.address + 2384, r.Below(4) == 0 ? 1 : 0);
          for (uint32_t i = 0; i < 80; i++) {
            const uint32_t entry = manager.address + 4 + 28 * i;
            tr::RandomPosition(h.base, entry + 8, route, r);
          }
          for (uint32_t i = 0; i < 7; i++) {
            guest::StoreU32(h.base, vehicles.address + 6272 * i + 344, 0);
          }
          // The race data.
          const uint32_t records = race.address + 48;
          const bool has_event = r.Below(3) != 0;
          const uint32_t count = 1 + r.Below(14);
          const uint32_t player = r.Below(2) == 0 ? 0 : r.Below(count);
          guest::StoreU32(h.base, records + 0x595E0, count);
          for (uint32_t i = 0; i < count; i++) {
            const uint32_t record = records + 3936 * i;
            const uint32_t flags[] = {0x2, 0x80, 0x82,  0x4,   0x0, 0x102,
                                      0x3, 0x8,  0x402, 0x180, 0x2, 0x2};
            uint32_t f = r.Below(10) == 0 ? uint32_t(r.Next()) : flags[r.Below(12)];
            if (i < player) {
              f &= ~0x1u;
            } else if (i == player) {
              f = 0x1 | (r.Below(4) == 0 ? 0x100 : 0);
            }
            guest::StoreU32(h.base, record + 3888, f);
            if (r.Below(5) == 0) {
              guest::StoreU16(h.base, record + 3904, 0xFFFF);
              guest::StoreU16(h.base, record + 3906, uint16_t(r.Next()));
            } else {
              tr::RandomPosition(h.base, record + 3904, route, r);
            }
            guest::StoreU32(h.base, record + 3912,
                            r.Below(3) == 0 ? 0xFFFFFFFFu - r.Below(3) : r.Below(4));
            guest::StoreU32(h.base, record + 3916,
                            !has_event || r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(8));
          }
          if (player != 0 && r.Below(2) == 0) {
            // A later record flagged 0x1 too (only the first is the player's).
            const uint32_t other = player + 1 + r.Below(count - player);
            if (other < count) {
              guest::StoreU32(h.base, records + 3936 * other + 3888, 0x1);
            }
          }
          const uint32_t stages = race.address + 0x59620;
          const uint32_t stage = r.Below(3);
          guest::StoreU32(h.base, stages + 804, stage);
          for (uint32_t i = 0; i < 4; i++) {
            guest::StoreU32(h.base, stages + 80 * stage + 44 + 8 * i,
                            r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(count + 1));
          }
          const uint32_t modes[] = {1, 1, 0, 6};
          guest::StoreU32(h.base, race.address + 0xA3AF0, modes[r.Below(4)]);
          guest::StoreU32(h.base, race.address + 0xA3AF4, r.Below(2) ? 2 : r.Below(4));
          guest::StoreU32(h.base, race.address + 0x59618,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6));
          guest::StoreU32(
              h.base, race.address + 0x59AE4,
              r.Below(4) == 0 ? uint32_t(r.Next()) | 0x2000 : uint32_t(r.Next()) & ~0x2000u);
          guest::StoreU32(h.base, race.address + 0xA3AE0,
                          wild && r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(20));
          for (uint32_t t = 0; t < 3; t++) {
            guest::StoreU32(h.base, manager.address + 2160 + 28 * t + 16,
                            wild         ? uint32_t(r.Next())
                            : r.Below(2) ? 0
                                         : r.Below(43));
          }
          // RaceEvent::PlaceStarts' flags word (BranchMasksFor) at +20.
          const uint32_t masks[] = {0, 0x4, 0x8, 0x10, 0x20, 0x3C};
          guest::StoreU32(h.base, race.address + 20, masks[r.Below(6)]);
          // The save: +6092 the flags, +6100 1 (back), +6108 the kind.
          const uint32_t words[] = {0x3F, 0x3E, 0, 0x1F, 0x7};
          guest::StoreU32(h.base, save.address + 6092,
                          r.Below(2) == 0 ? uint32_t(r.Next()) : words[r.Below(5)]);
          const uint32_t backs[] = {1, 1, 0, 1000, 2};
          guest::StoreU32(h.base, save.address + 6100,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : backs[r.Below(5)]);
          guest::StoreU32(
              h.base, save.address + 6108,
              r.Below(8) == 0 ? (r.Below(2) ? 20 + r.Below(20) : uint32_t(r.Next())) : r.Below(20));
          // The event: its groups -3 to 5 (+0, 12 bytes each: a count and its
          // entries' list), its list at +2048 (+156).
          WriteRandom(h.base, event.address, event.bytes, r);
          guest::StoreU32(h.base, event.address + 156, event.address + 2048);
          for (uint32_t slot = 0; slot < 9; slot++) {
            const uint32_t at = event.address + 12 * slot;
            const uint32_t entries = event.address + 256 + 64 * slot;
            const int32_t n = r.Below(8) == 0   ? int32_t(r.Below(20)) - 4
                              : r.Below(4) == 0 ? 0
                                                : int32_t(1 + r.Below(8));
            guest::StoreU16(h.base, at + 0, uint16_t(n));
            guest::StoreU32(h.base, at + 4, entries);
            for (uint32_t i = 0; i < 16; i++) {
              guest::StoreU16(
                  h.base, entries + 4 * i + 2,
                  uint16_t(r.Below(6) == 0 ? int32_t(r.Below(20)) - 20 : int32_t(r.Below(40))));
            }
          }
          for (uint32_t i = 0; i < 8; i++) {
            tr::RandomPosition(h.base, event.address + 2048 + 12 * i + 4, route, r);
          }
          guest::StoreU32(h.base, rw::kRaceEvent, has_event ? event.address : 0);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = manager.address;
          t.r4.u64 = records;
          (v == 0 ? Original<__imp__sub_82173050> : hooks::RaceWorldAddRacers)(t, h.base);
          if (v == 1) {
            racers += guest::LoadU32(h.base, manager.address + 2372);
            traffic += guest::LoadU32(h.base, manager.address + 2376);
            events += has_event && guest::LoadU32(h.base, race.address + 0xA3AF0) == 1 &&
                              guest::LoadU32(h.base, race.address + 0xA3AF4) == 2
                          ? 1
                          : 0;
          }
          out[v].Bytes(Read(h.base, manager.address, 2400));
          out[v].Bytes(Read(h.base, manager.address + 2392, 8 * 1200));
          out[v].Bytes(Read(h.base, manager.address + 360000, 64));
          for (uint32_t i = 0; i < count; i++) {
            out[v].Word(guest::LoadU32(h.base, records + 3936 * i + 3904));
          }
          out[v].Bytes(Read(h.base, event.address, event.bytes));
          drivers_out(out[v]);
          out[v].Bytes(Read(h.base, frame + 80, 16));
          out[v].Floats(h.base, frame + 96, 12);
          out[v].Bytes(Read(h.base, frame + 144, 64));
          list_out(out[v], frame + 208);
        }
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
      results.Report();
    }
    ITC_LOG_INFO("harness: race_world: AddRacers gave {} racers and {} traffic cars vehicles, "
                 "{} cases with the event's placements",
                 racers, traffic, events);
  }
  {
    // RaceManager::StreamOpponents: the manager's entries (1 to 24; the
    // player's, flagged 0x4 with a vehicle, first or later) with flags among
    // 0x1, 0x41, 0x21, 0x60, 0x80, 0x10, 0x400, 0x0, 0x8, 0x48, 0x101, 0x40
    // and random; vehicles or none (-1); the slot words 0, -1, 1, 2,
    // negative or random; RaceManager::GiveVehicle (never seen in play)
    // answered by a stand-in on both sides (StandInGiveVehicle; the loaded
    // world's cases in StartCases run its original); the vehicles losing theirs with a box shape of
    // their own (only with --native_heap) or none; the racers' count +2364
    // 0 (as RaceManager::StreamCruise leaves it) or 1 to 3; the radii
    // inside, between and beyond the keys (0.0, the far key and the
    // distances), now and then NaN. Compared: the manager, the vehicles'
    // +340, +344 (by name), +6148, which shapes were freed, the stand-in's
    // log, the drivers, the input records and the frame's temporaries (the
    // sorted list's keys and the vectors as floats).
    Results results("RaceManager::StreamOpponents (sub_82172118)");
    const uint32_t frame = sp - 1920;
    uint32_t listed = 0, lost = 0, given = 0;
    for (uint64_t seed = 0; seed < 500; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 193 + 113);
        auto globals = saved();
        common(r);
        tr::Route route;
        route_up(r, route);
        WriteRandom(h.base, manager.address, 2400, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        const uint32_t count = 1 + r.Below(24);
        guest::StoreU32(h.base, manager.address + 2368, count);
        guest::StoreU32(h.base, manager.address + 2364, r.Below(2) == 0 ? 0 : r.Below(4));
        const uint32_t player = r.Below(2) == 0 ? 0 : r.Below(count);
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          const uint32_t flags[] = {0x1, 0x41, 0x21, 0x60, 0x80, 0x10, 0x400,
                                    0x0, 0x8,  0x48, 0x101, 0x40, 0x1, 0x41};
          uint32_t f = r.Below(8) == 0 ? uint32_t(r.Next()) : flags[r.Below(14)];
          if (i == player) {
            f = 0x4 | (r.Below(2) ? 0x40 : uint32_t(r.Next()) & ~0xA0u);
          }
          guest::StoreU32(h.base, entry, f);
          tr::RandomPosition(h.base, entry + 8, route, r);
          const bool vehicle = i == player || r.Below(3) != 0;
          guest::StoreU32(h.base, entry + 20, vehicle ? r.Below(7) : 0xFFFFFFFFu);
          guest::StoreU32(h.base, entry + 24, r.Below(14));
        }
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t slots[] = {1, 1, 2, 0x80000000u, 0, 0xFFFFFFFFu, uint32_t(r.Next())};
          guest::StoreU32(h.base, manager.address + 2336 + 4 * k, slots[r.Below(7)]);
        }
        std::vector<uint32_t> shapes;
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          uint32_t shape = 0;
          if (hooks::NativeHeapActive() && r.Below(3) == 0) {
            shape = memory::Alloc(496, 0);
            car_objects::BuildBox(h, shape, r);
            shapes.push_back(shape);
          }
          guest::StoreU32(h.base, vehicle + 340, 0);
          guest::StoreU32(h.base, vehicle + 344, shape);
          tr::RandomPosition(h.base, vehicle + 5236, route, r);
        }
        const float spawns[] = {-1.0f, 0.5f, 20.0f, 60.0f, 150.0f, 1.0e9f};
        const float spawn = r.Below(16) == 0 ? std::bit_cast<float>(0x7FC00000u)
                                             : spawns[r.Below(6)];
        const float despawns[] = {-1.0f, 10.0f, 80.0f, 200.0f, 1.0e9f, spawn};
        const float despawn = r.Below(16) == 0 ? std::bit_cast<float>(0xFFC00001u)
                                               : despawns[r.Below(6)];
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager.address;
        t.r4.u64 = manager.address + 4 + 28 * player;
        t.f1.f64 = double(spawn);
        t.f2.f64 = double(despawn);
        GiveVehicleStandIn stand_in;
        (v == 0 ? Original<__imp__sub_82172118> : hooks::RaceWorldStreamOpponents)(t, h.base);
        out[v].Word(uint32_t(g_give_log.size()));
        for (uint32_t w : g_give_log) {
          out[v].Word(w);
        }
        if (v == 1) {
          given += uint32_t(g_give_log.size() / 3);
        }
        const auto name = [&](uint32_t w) {
          for (size_t i = 0; i < shapes.size(); i++) {
            if (w == shapes[i]) {
              return uint32_t(0xB10C0000u + i);
            }
          }
          return w;
        };
        if (v == 1) {
          listed += guest::LoadU32(h.base, manager.address + 2372);
          for (uint32_t i = 0; i < count; i++) {
            const uint32_t entry = manager.address + 4 + 28 * i;
            lost += guest::LoadU32(h.base, entry + 20) == 0xFFFFFFFFu ? 1 : 0;
          }
        }
        out[v].Bytes(Read(h.base, manager.address, 2400));
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 340)));
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 344)));
          out[v].Word(guest::LoadU32(h.base, vehicle + 6148));
        }
        for (uint32_t s : shapes) {
          out[v].Word(memory::IsHeapBlock(s) ? 1 : 0);
          if (memory::IsHeapBlock(s)) {
            memory::Free(s);
          }
        }
        drivers_out(out[v]);
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Floats(h.base, frame + 96, 12);
        out[v].Bytes(Read(h.base, frame + 144, 20));
        list_out(out[v], frame + 176);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: StreamOpponents listed {} racers, {} entries without a "
                 "vehicle after, {} calls to GiveVehicle's stand-in",
                 listed, lost, given);
  }
  {
    // RaceManager::Refresh: as StreamOpponents' cases, with the player (the
    // first flagged 0x4: none of those before it so flagged; it has a
    // vehicle) first or later, the counts +2364, +2372, +2376 random; the
    // radii the game's ([0x8200996C], [0x82009970]); the three traffic
    // entries (+2160) in use (their vehicles 0 to 6) or not; the race data's
    // flags (+0x59AE4) with 0x4000 or without (the other bits random).
    // RaceManager::GiveVehicle answered by the stand-in on both sides (now
    // and then none free, which ends the list's pass). Compared: the
    // manager, the vehicles' +340, +344 (by name), +6148, which shapes were
    // freed, the stand-in's log, the drivers, the input records and the
    // frame's temporaries (the sorted list's keys and the vectors as
    // floats).
    Results results("RaceManager::Refresh (sub_82171978)");
    const uint32_t frame = sp - 1904;
    uint32_t listed = 0, traffic = 0, removed = 0, given = 0;
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 229 + 151);
        auto globals = saved();
        common(r);
        tr::Route route;
        route_up(r, route);
        WriteRandom(h.base, manager.address, 2400, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        const uint32_t count = 1 + r.Below(24);
        guest::StoreU32(h.base, manager.address + 2368, count);
        const uint32_t player = r.Below(2) == 0 ? 0 : r.Below(count);
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          const uint32_t flags[] = {0x1, 0x41, 0x21, 0x60, 0x80, 0x10, 0x400,
                                    0x0, 0x8,  0x48, 0x101, 0x40, 0x1, 0x41};
          uint32_t f = r.Below(8) == 0 ? uint32_t(r.Next()) : flags[r.Below(14)];
          if (i < player) {
            f &= ~0x4u;
          } else if (i == player) {
            f = 0x4 | (r.Below(2) ? 0x40 : uint32_t(r.Next()) & ~0xA0u);
          }
          guest::StoreU32(h.base, entry, f);
          tr::RandomPosition(h.base, entry + 8, route, r);
          const bool vehicle = i == player || r.Below(3) != 0;
          guest::StoreU32(h.base, entry + 20, vehicle ? r.Below(7) : 0xFFFFFFFFu);
          guest::StoreU32(h.base, entry + 24, r.Below(14));
        }
        for (uint32_t t = 0; t < 3; t++) {
          const uint32_t entry = manager.address + 2160 + 28 * t;
          const uint32_t used[] = {578, 2626, 1, uint32_t(r.Next()) | 1};
          const bool in_use = r.Below(3) != 0;
          guest::StoreU32(h.base, entry, in_use ? used[r.Below(4)] : 0);
          if (in_use) {
            guest::StoreU32(h.base, entry + 20, r.Below(7));
          }
        }
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t slots[] = {1, 1, 2, 0x80000000u, 0, 0xFFFFFFFFu, uint32_t(r.Next())};
          guest::StoreU32(h.base, manager.address + 2336 + 4 * k, slots[r.Below(7)]);
        }
        const uint32_t race_flags =
            (uint32_t(r.Next()) & ~0x4000u) | (r.Below(3) == 0 ? 0x4000u : 0u);
        guest::StoreU32(h.base, race.address + 0x59AE4, race_flags);
        std::vector<uint32_t> shapes;
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          uint32_t shape = 0;
          if (hooks::NativeHeapActive() && r.Below(3) == 0) {
            shape = memory::Alloc(496, 0);
            car_objects::BuildBox(h, shape, r);
            shapes.push_back(shape);
          }
          guest::StoreU32(h.base, vehicle + 340, 0);
          guest::StoreU32(h.base, vehicle + 344, shape);
          tr::RandomPosition(h.base, vehicle + 5236, route, r);
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager.address;
        GiveVehicleStandIn stand_in;
        (v == 0 ? Original<__imp__sub_82171978> : hooks::RaceWorldRefresh)(t, h.base);
        out[v].Word(uint32_t(g_give_log.size()));
        for (uint32_t w : g_give_log) {
          out[v].Word(w);
        }
        const auto name = [&](uint32_t w) {
          for (size_t i = 0; i < shapes.size(); i++) {
            if (w == shapes[i]) {
              return uint32_t(0xB10C0000u + i);
            }
          }
          return w;
        };
        if (v == 1) {
          given += uint32_t(g_give_log.size() / 3);
          listed += guest::LoadU32(h.base, manager.address + 2372);
          traffic += guest::LoadU32(h.base, manager.address + 2376);
          for (uint32_t t = 0; t < 3; t++) {
            removed += (race_flags & 0x4000) != 0 &&
                               guest::LoadU32(h.base, manager.address + 2160 + 28 * t + 20) ==
                                   0xFFFFFFFFu
                           ? 1
                           : 0;
          }
        }
        out[v].Bytes(Read(h.base, manager.address, 2400));
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 340)));
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 344)));
          out[v].Word(guest::LoadU32(h.base, vehicle + 6148));
        }
        for (uint32_t s : shapes) {
          out[v].Word(memory::IsHeapBlock(s) ? 1 : 0);
          if (memory::IsHeapBlock(s)) {
            memory::Free(s);
          }
        }
        drivers_out(out[v]);
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Floats(h.base, frame + 96, 12);
        out[v].Bytes(Read(h.base, frame + 144, 20));
        list_out(out[v], frame + 176);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: Refresh listed {} racers and {} traffic cars, removed {} "
                 "traffic cars, {} calls to GiveVehicle's stand-in",
                 listed, traffic, removed, given);
  }
  {
    // RaceManager::MoveUnseen: the manager's entries (0 to 24; the
    // player's, flagged 0x4, now and then 0x10, with a vehicle, first or
    // later) with flags among 0x1, 0x41, 0x21, 0x81, 0x8, 0x0, 0x40, 0x401,
    // 0x4 and random (0x40 with a vehicle); their records' items 0 to 3 (of
    // kinds 2, 3 and others), negative, or negative words whose halfword
    // isn't; sections now and then 186 to 262 (item kinds 2 and 3's
    // ranges); +12 0.0, 1.0, near and past a slice's ticks, large, negative
    // or NaN; the slot words near 216,000 or random; the race data's flags
    // +20 0x10, 0x20, both, neither or random; the racers' count +2372 4 or
    // not; the player's vehicle's body position, +6168 and speed (0.0, NaN,
    // or not). Compared: the manager (the entries' +12 as floats) and the
    // frame's temporaries (the callees' frames below it hold the originals'
    // register saves, which native callees don't write: not compared).
    const Block bodies(7 * 512);
    if (!bodies.address) {
      ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
      return;
    }
    Results results("RaceManager::MoveUnseen (sub_82173E28)");
    const uint32_t frame = sp - 272;
    uint32_t steps = 0, wraps = 0;
    for (uint64_t seed = 0; seed < 500; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 197 + 127);
        auto globals = saved();
        common(r);
        tr::Route route;
        route_up(r, route);
        WriteRandom(h.base, manager.address, 2400, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        WriteRandom(h.base, bodies.address, bodies.bytes, r);
        const uint32_t count = seed % 25 == 0 ? 0 : 1 + r.Below(24);
        guest::StoreU32(h.base, manager.address + 2368, count);
        guest::StoreU32(h.base, manager.address + 2372,
                        r.Below(2) == 0 ? 4 : (r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(8)));
        const uint32_t player = count == 0 || r.Below(2) == 0 ? 0 : r.Below(count);
        const uint32_t sections[] = {186, 187, 188, 189, 192, 193, 194, 195, 199, 200,
                                     201, 203, 204, 250, 251, 252, 253, 254, 257, 258,
                                     259, 261, 262};
        for (uint32_t i = 0; i < std::max(count, 1u); i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          const uint32_t flags[] = {0x1, 0x1, 0x41, 0x21, 0x81, 0x8, 0x0, 0x40, 0x401, 0x4};
          uint32_t f = r.Below(10) == 0 ? uint32_t(r.Next()) : flags[r.Below(10)];
          if (i == player) {
            f = 0x44 | (r.Below(2) == 0 ? 0x10 : 0);
          }
          guest::StoreU32(h.base, entry, f);
          tr::RandomPosition(h.base, entry + 8, route, r);
          if (r.Below(2) == 0) {
            guest::StoreU16(h.base, entry + 8, uint16_t(sections[r.Below(23)]));
          }
          const uint32_t floats[] = {0x00000000u, 0x3F800000u, 0x40000000u, 0x41200000u,
                                     0x42C80000u, 0x447A0000u, 0xBF800000u, 0x7FC00000u,
                                     0x4E6E6B28u, test_route::RandomFloat(r, 50.0f)};
          guest::StoreU32(h.base, entry + 12, floats[r.Below(10)]);
          guest::StoreU32(h.base, entry + 20,
                          i == player || (f & 0x40) != 0 || r.Below(2) == 0 ? r.Below(7)
                                                                            : 0xFFFFFFFFu);
          const uint32_t number = r.Below(14);
          guest::StoreU32(h.base, entry + 24, number);
          const uint32_t ids[] = {0, 1, 2, 3, 0xFFFFFFFFu, 0xFFFF0002u, 0xFFFF0003u, 2};
          guest::StoreU32(h.base, race.address + 3936 * number + 3960, ids[r.Below(8)]);
        }
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t slots[] = {215999, 216000, 216001, 0x7FFFFFFFu, 59, uint32_t(r.Next())};
          guest::StoreU32(h.base, manager.address + 2336 + 4 * k, slots[r.Below(6)]);
        }
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          tr::RandomPosition(h.base, vehicle + 5236, route, r);
          guest::StoreU32(h.base, vehicle + 336, bodies.address + 512 * i);
          for (uint32_t w = 0; w < 4; w++) {
            guest::StoreU32(h.base, bodies.address + 512 * i + 208 + 4 * w,
                            test_route::RandomFloat(r, 150.0f));
          }
          guest::StoreU32(h.base, vehicle + 6168, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
          const uint32_t speeds[] = {0x00000000u, 0x80000000u, 0x41F00000u, 0x42480000u,
                                     0x7FC00000u, test_route::RandomFloat(r, 80.0f)};
          guest::StoreU32(h.base, vehicle + 48, speeds[r.Below(6)]);
        }
        const uint32_t masks[] = {0, 0x10, 0x20, 0x30, 0x3C, uint32_t(r.Next())};
        guest::StoreU32(h.base, race.address + 20, masks[r.Below(6)]);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager.address;
        t.r4.u64 = manager.address + 4 + 28 * player;
        if (v == 1) {
          for (uint32_t i = 0; i < count; i++) {
            const uint32_t entry = manager.address + 4 + 28 * i;
            const uint32_t f = guest::LoadU32(h.base, entry);
            if ((f & 0xE0) == 0x40 &&
                guest::LoadU32(h.base, manager.address + 2336 +
                                           4 * guest::LoadU32(h.base, entry + 20)) >= 216000) {
              wraps++;
            }
          }
        }
        std::vector<uint32_t> before;
        for (uint32_t i = 0; i < count && v == 1; i++) {
          before.push_back(guest::LoadU32(h.base, manager.address + 4 + 28 * i + 8));
        }
        (v == 0 ? Original<__imp__sub_82173E28> : hooks::RaceWorldMoveUnseen)(t, h.base);
        for (uint32_t i = 0; i < count && v == 1; i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          steps += (guest::LoadU32(h.base, entry) & 0xE0) == 0 &&
                           guest::LoadU32(h.base, entry + 8) != before[i]
                       ? 1
                       : 0;
        }
        for (uint32_t i = 0; i < 80; i++) {
          const uint32_t entry = manager.address + 4 + 28 * i;
          out[v].Bytes(Read(h.base, entry, 12));
          out[v].Float(guest::LoadU32(h.base, entry + 12));
          out[v].Bytes(Read(h.base, entry + 16, 12));
        }
        out[v].Bytes(Read(h.base, manager.address, 4));
        out[v].Bytes(Read(h.base, manager.address + 2244, 2400 - 2244));
        out[v].Bytes(Read(h.base, frame + 80, 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: MoveUnseen moved {} entries a slice on (or jumped), {} "
                 "slot words at 216,000 or more",
                 steps, wraps);
  }
  {
    // RaceManager::StreamTraffic on the random route, up to (not into) a
    // new traffic car (its vehicle's set-up runs on the loaded world's
    // cases, StreamTrafficGiven in StartCases): by kind, the race data's
    // flags (+0x59AE4) with 0x4000 (now and then 0x2000 too), with 0x2000,
    // or neither with +2388 below 90 (0, 1, 88, 89), or at 90 or more with
    // +2380 not above 0, or at 90 or more with +2380 above (3 or large) and
    // no removal (the far radius NaN) and no free entry, or a free
    // entry and every slot word busy (above 0); the three traffic entries
    // (+2160) in use (578, 2626, 0x842, 1 or random; their vehicles 0 to 6)
    // or not; the slot words near 216,000, 0, -1, negative or random; the
    // second player or none; the radii inside, between and beyond the
    // distances, now and then NaN; the vehicles losing theirs with a box
    // shape of their own (only with --native_heap) or none; the race data's
    // other flags random. Compared: the manager, the vehicles' +340, +344
    // (by name), +6148, which shapes were freed and the frame's temporaries
    // (+96 to +271 as floats).
    const Block bodies(7 * 512);
    if (!bodies.address) {
      ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
      return;
    }
    Results results("RaceManager::StreamTraffic (sub_821724F8)");
    const uint32_t frame = sp - 464;
    uint32_t removed = 0, listed = 0, counted = 0, wraps = 0, scanned = 0;
    for (uint64_t seed = 0; seed < 400; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 211 + 139);
        auto globals = saved();
        common(r);
        tr::Route route;
        route_up(r, route);
        WriteRandom(h.base, manager.address, 2400, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        WriteRandom(h.base, bodies.address, bodies.bytes, r);
        const uint32_t kind = uint32_t(seed % 5);
        guest::StoreU32(h.base, manager.address + 2364, r.Below(5));
        const uint32_t player = manager.address + 4 + 28 * r.Below(20);
        guest::StoreU32(h.base, player + 20, r.Below(7));
        uint32_t second = 0;
        if (r.Below(2) == 0) {
          second = manager.address + 4 + 28 * (20 + r.Below(20));
          guest::StoreU32(h.base, second + 20, r.Below(7));
        }
        bool all_used = kind == 4 && r.Below(2) == 0;
        for (uint32_t t = 0; t < 3; t++) {
          const uint32_t entry = manager.address + 2160 + 28 * t;
          const uint32_t flags[] = {578, 2626, 0x842, 1, uint32_t(r.Next()) | 1};
          const bool used = all_used || r.Below(3) != 0;
          guest::StoreU32(h.base, entry, used ? flags[r.Below(5)] : 0);
          if (used) {
            guest::StoreU32(h.base, entry + 20, r.Below(7));
          }
        }
        for (uint32_t k = 0; k < 7; k++) {
          uint32_t word;
          if (kind == 4) {
            const uint32_t busy[] = {1, 2, 59, 1000, 215999, 216000, 0x7FFFFFFFu,
                                     (uint32_t(r.Next()) & 0x7FFFFFFFu) | 1};
            word = busy[r.Below(8)];
          } else {
            const uint32_t slots[] = {215999, 216000, 216001, 0, 0xFFFFFFFFu,
                                      0x80000000u, 1, uint32_t(r.Next())};
            word = slots[r.Below(8)];
          }
          guest::StoreU32(h.base, manager.address + 2336 + 4 * k, word);
        }
        const uint32_t ticks_below[] = {0, 1, 88, 89};
        const uint32_t ticks_at[] = {90, 91, 1000, 0xFFFFFFFFu};
        guest::StoreU32(h.base, manager.address + 2388,
                        kind == 2 ? ticks_below[r.Below(4)] : ticks_at[r.Below(4)]);
        const uint32_t not_above[] = {0, 0xFFFFFFFFu, 0x80000000u};
        const uint32_t above[] = {3, 0x7FFFFFFFu};
        guest::StoreU32(h.base, manager.address + 2380,
                        kind == 3   ? not_above[r.Below(3)]
                        : kind == 4 ? above[r.Below(2)]
                                    : uint32_t(r.Next()));
        uint32_t flags = uint32_t(r.Next()) & ~0x6000u;
        if (kind == 0) {
          flags |= 0x4000 | (r.Below(3) == 0 ? 0x2000 : 0);
        } else if (kind == 1) {
          flags |= 0x2000;
        }
        guest::StoreU32(h.base, race.address + 0x59AE4, flags);
        std::vector<uint32_t> shapes;
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          uint32_t shape = 0;
          if (hooks::NativeHeapActive() && r.Below(3) == 0) {
            shape = memory::Alloc(496, 0);
            car_objects::BuildBox(h, shape, r);
            shapes.push_back(shape);
          }
          guest::StoreU32(h.base, vehicle + 336, bodies.address + 512 * i);
          guest::StoreU32(h.base, vehicle + 340, 0);
          guest::StoreU32(h.base, vehicle + 344, shape);
          tr::RandomPosition(h.base, vehicle + 5236, route, r);
          for (uint32_t w = 0; w < 4; w++) {
            guest::StoreU32(h.base, bodies.address + 512 * i + 208 + 4 * w,
                            test_route::RandomFloat(r, 150.0f));
          }
        }
        const float inners[] = {-1.0f, 0.5f, 20.0f, 60.0f, 150.0f, 1.0e9f};
        const float inner = r.Below(16) == 0 ? std::bit_cast<float>(0x7FC00000u)
                                             : inners[r.Below(6)];
        const float outers[] = {-1.0f, 10.0f, 80.0f, 200.0f, 1.0e9f, inner};
        float outer = r.Below(16) == 0 ? std::bit_cast<float>(0xFFC00001u) : outers[r.Below(6)];
        if (kind == 4) {
          // NaN: no entry is beyond it (an infinite distance would be beyond
          // 1e9, freeing an entry and its slot for a new car).
          outer = std::bit_cast<float>(r.Below(2) == 0 ? 0x7FC00000u : 0xFFC00001u);
        }
        if (v == 1) {
          for (uint32_t t = 0; t < 3; t++) {
            const uint32_t entry = manager.address + 2160 + 28 * t;
            if (guest::LoadU32(h.base, entry) != 0 &&
                int32_t(guest::LoadU32(h.base, manager.address + 2336 +
                                                   4 * guest::LoadU32(h.base, entry + 20))) >=
                    216000) {
              wraps++;
            }
          }
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager.address;
        t.r4.u64 = player;
        t.r5.u64 = second;
        t.f1.f64 = double(inner);
        t.f2.f64 = double(outer);
        const uint32_t ticks = guest::LoadU32(h.base, manager.address + 2388);
        (v == 0 ? Original<__imp__sub_821724F8> : hooks::RaceWorldStreamTraffic)(t, h.base);
        if (v == 1) {
          for (uint32_t i = 0; i < 3; i++) {
            removed += guest::LoadU32(h.base, manager.address + 2160 + 28 * i + 20) ==
                               0xFFFFFFFFu
                           ? 1
                           : 0;
          }
          listed += guest::LoadU32(h.base, manager.address + 2376);
          const uint32_t after = guest::LoadU32(h.base, manager.address + 2388);
          counted += after == ticks + 1 ? 1 : 0;
          scanned += after == 0 && ticks != 0 ? 1 : 0;
        }
        const auto name = [&](uint32_t w) {
          for (size_t i = 0; i < shapes.size(); i++) {
            if (w == shapes[i]) {
              return uint32_t(0xB10C0000u + i);
            }
          }
          return w;
        };
        out[v].Bytes(Read(h.base, manager.address, 2400));
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t vehicle = vehicles.address + 6272 * i;
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 340)));
          out[v].Word(name(guest::LoadU32(h.base, vehicle + 344)));
          out[v].Word(guest::LoadU32(h.base, vehicle + 6148));
        }
        for (uint32_t s : shapes) {
          out[v].Word(memory::IsHeapBlock(s) ? 1 : 0);
          if (memory::IsHeapBlock(s)) {
            memory::Free(s);
          }
        }
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Floats(h.base, frame + 96, 32);
        out[v].Bytes(Read(h.base, frame + 224, 20));
        out[v].Floats(h.base, frame + 244, 7);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: StreamTraffic listed {} traffic cars, {} entries without "
                 "a vehicle after, {} slot words at 216,000 or more, {} cases counted +2388, {} "
                 "scanned for a free entry",
                 listed, removed, wraps, counted, scanned);
  }
  {
    // The streaming by the race's kind: RaceManager::StreamNear,
    // StreamCruise, StreamTwoPlayers and Stream over the random world of the
    // groups above, their callees (StreamOpponents, StreamTraffic,
    // MoveUnseen) run for real (RaceManager::GiveVehicle through its
    // stand-in on both sides; no new traffic car: +2388 below 90). The
    // manager's entries (2 to 24; the player's, flagged 0x4 (now and then
    // 0x10) with a vehicle, first or later; the first two always with
    // vehicles, the two players StreamTwoPlayers takes) with the
    // StreamOpponents and MoveUnseen groups' flags, +12 and records' items
    // (the route's floats finite and the positions on its sections and
    // slices: a NaN key
    // can put a player past the fourth, taking its vehicle, which the
    // callees after read; the callees' own groups keep those); the slot words; the traffic entries in use or not;
    // the race data's flags (+0x59AE4: 0x20, 0x40, 0x2000, 0x4000 or not;
    // +20 MoveUnseen's masks); [0x826D82E8]'s floats +3012 and +3984 below,
    // equal, above or NaN; the vehicles' speeds (+48) around the speeds at
    // which the far radii reach their caps ([0x82003384], and [0x820099CC]
    // for the traffic's), large, negative or NaN; Stream's kinds 0 to 8,
    // above 8 and random. Compared: the manager (the entries' +12 as
    // floats), the vehicles' +340, +344 (by name), +6148, which shapes were
    // freed, the stand-in's log, the drivers, the input records and the
    // temporaries of the last callee's frame (StreamTraffic's for StreamNear
    // and StreamTwoPlayers, MoveUnseen's for StreamCruise: the callees run
    // after it put the originals' register saves in the frames above).
    const Block bodies(7 * 512);
    const Block racer_data(8192);
    if (!bodies.address || !racer_data.address) {
      ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
      return;
    }
    constexpr uint32_t kRacerData = 0x826D82E8;
    namespace ops = car_physics::ops;
    const double opponent_scale = ops::Lfs(h.base, 0x820021F4);
    const double traffic_scale = ops::Lfs(h.base, 0x820021E8);
    const double far_base = ops::Lfs(h.base, 0x82008914);
    const double cap = ops::Lfs(h.base, 0x82003384);
    const double near_cap = ops::Lfs(h.base, 0x820099CC);
    // The speeds at which the opponents' far radius reaches its cap and the
    // traffic's reaches the opponents' near cap.
    const float at_cap = float((cap - far_base) / opponent_scale);
    const float at_near_cap = float((near_cap - far_base) / traffic_scale);
    enum Which { kNear, kCruise, kTwo, kStream };
    for (const Which which : {kNear, kCruise, kTwo, kStream}) {
      const char* names[] = {"RaceManager::StreamNear (sub_82172E60)",
                             "RaceManager::StreamCruise (sub_82174200)",
                             "RaceManager::StreamTwoPlayers (sub_82172D20)",
                             "RaceManager::Stream (sub_82174338)"};
      Results results(names[which]);
      uint32_t capped = 0, both_capped = 0, second_ahead = 0, listed = 0;
      uint32_t kinds[11] = {};
      for (uint64_t seed = 0; seed < (which == kStream ? 330u : 150u); seed++) {
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          Random r(seed * 227 + 157 + 10000 * uint64_t(which));
          auto globals = saved();
          SavedBytes data_saved(h.base, {{kRacerData, 4}});
          common(r);
          tr::Route route;
          route_up(r, route);
          // The route's floats finite: a NaN key (an infinity's difference)
          // sorts a player past the fourth, taking its vehicle, which the
          // callees after read (the callees' own groups keep them).
          const auto finite = [&](uint32_t address) {
            if ((guest::LoadU32(h.base, address) & 0x7F800000u) == 0x7F800000u) {
              guest::StoreFloat(h.base, address, 1.0f + float(r.Below(64)));
            }
          };
          finite(area.address + tr::kFile + 8);
          for (uint32_t k = 0; k < 8; k++) {
            const uint32_t at = route.address[k];
            const uint32_t n = route.slices[k];
            for (uint32_t i = 0; i < 6; i++) {
              finite(at + 4 + 4 * i);
            }
            for (uint32_t j = 0; j < n; j++) {
              for (uint32_t i = 0; i < 84; i += 4) {
                if (i != 32 && i != 56) {
                  finite(at + 124 + 84 * j + i);
                }
              }
            }
            for (uint32_t i = 0; i < 24 * n; i += 4) {
              finite(at + 124 + 84 * n + i);
            }
          }
          // A position on one of the route's sections and slices.
          const auto on_route = [&](uint32_t address) {
            const uint32_t section = r.Below(route.sections);
            guest::StoreU16(h.base, address + 0, uint16_t(section));
            guest::StoreU16(h.base, address + 2, uint16_t(r.Below(route.Slices(section))));
          };
          WriteRandom(h.base, manager.address, 2400, r);
          WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
          WriteRandom(h.base, bodies.address, bodies.bytes, r);
          WriteRandom(h.base, racer_data.address, racer_data.bytes, r);
          // Stream's kind: 0 to 8 in turn, then above 8 and random.
          uint32_t kind = 0;
          if (which == kStream) {
            const uint32_t k = uint32_t(seed % 11);
            kind = k < 9 ? k : (k == 9 ? 9 + r.Below(4) : uint32_t(r.Next()));
            kinds[k] += v == 1 ? 1 : 0;
          }
          guest::StoreU32(h.base, race.address + 0xA3AF4, kind);
          const uint32_t count = 2 + r.Below(23);
          guest::StoreU32(h.base, manager.address + 2368, count);
          guest::StoreU32(h.base, manager.address + 2364, r.Below(4));
          guest::StoreU32(h.base, manager.address + 2372,
                          r.Below(2) == 0 ? 4 : r.Below(8));
          // The players: one (flagged 0x4) for StreamNear and StreamCruise,
          // the first two (0x4 and 0x8) for StreamTwoPlayers; the others
          // without 0x4 or 0x8 (keys 0.0: more than four would take a
          // player's vehicle, which the callees after then read).
          const bool two = which == kTwo || (which == kStream && kind == 5);
          const uint32_t player = two || r.Below(2) == 0 ? 0 : r.Below(count);
          for (uint32_t i = 0; i < count; i++) {
            const uint32_t entry = manager.address + 4 + 28 * i;
            const uint32_t flags[] = {0x1, 0x41, 0x21, 0x60, 0x80, 0x10, 0x400, 0x0,
                                      0x1, 0x41, 0x101, 0x40, 0x1, 0x41, 0x81, 0x401};
            uint32_t f = r.Below(8) == 0 ? uint32_t(r.Next()) & ~0xCu : flags[r.Below(16)];
            if (i == player || (two && i < 2)) {
              f = (two && i == 1 ? 0x48 : 0x44) | (r.Below(3) == 0 ? 0x10 : 0);
            }
            guest::StoreU32(h.base, entry, f);
            on_route(entry + 8);
            const uint32_t floats[] = {0x00000000u, 0x3F800000u, 0x41200000u, 0x447A0000u,
                                       0xBF800000u, 0x7FC00000u,
                                       test_route::RandomFloat(r, 50.0f)};
            guest::StoreU32(h.base, entry + 12, floats[r.Below(7)]);
            const bool vehicle = i == player || i < 2 || (f & 0x40) != 0 || r.Below(3) != 0;
            guest::StoreU32(h.base, entry + 20, vehicle ? r.Below(7) : 0xFFFFFFFFu);
            const uint32_t number = r.Below(14);
            guest::StoreU32(h.base, entry + 24, number);
            const uint32_t ids[] = {0, 1, 2, 3, 0xFFFFFFFFu, 0xFFFF0002u, 2};
            guest::StoreU32(h.base, race.address + 3936 * number + 3960, ids[r.Below(7)]);
          }
          for (uint32_t k = 0; k < 7; k++) {
            const uint32_t slots[] = {1, 1, 2, 0x80000000u, 0, 0xFFFFFFFFu, 215999, 216000,
                                      uint32_t(r.Next())};
            guest::StoreU32(h.base, manager.address + 2336 + 4 * k, slots[r.Below(9)]);
          }
          for (uint32_t t = 0; t < 3; t++) {
            const uint32_t entry = manager.address + 2160 + 28 * t;
            const uint32_t flags[] = {578, 2626, 0x842, 1, uint32_t(r.Next()) | 1};
            const bool used = r.Below(3) != 0;
            guest::StoreU32(h.base, entry, used ? flags[r.Below(5)] : 0);
            if (used) {
              guest::StoreU32(h.base, entry + 20, r.Below(7));
            }
          }
          const uint32_t ticks[] = {0, 1, 88, 89};
          guest::StoreU32(h.base, manager.address + 2388, ticks[r.Below(4)]);
          uint32_t flags = uint32_t(r.Next()) & ~0x6060u;
          flags |= (r.Below(4) == 0 ? 0x4000 : 0) | (r.Below(4) == 0 ? 0x2000 : 0);
          const uint32_t ahead_flags[] = {0, 0x20, 0x40, 0x60};
          flags |= ahead_flags[r.Below(4)];
          guest::StoreU32(h.base, race.address + 0x59AE4, flags);
          const uint32_t masks[] = {0, 0x10, 0x20, 0x30, uint32_t(r.Next())};
          guest::StoreU32(h.base, race.address + 20, masks[r.Below(5)]);
          const uint32_t positions[] = {0x00000000u, 0x3F800000u, 0x40000000u, 0x7FC00000u,
                                        test_route::RandomFloat(r, 100.0f)};
          const uint32_t ahead_at = positions[r.Below(5)];
          guest::StoreU32(h.base, racer_data.address + 3012, ahead_at);
          guest::StoreU32(h.base, racer_data.address + 3984,
                          r.Below(4) == 0 ? ahead_at : positions[r.Below(5)]);
          guest::StoreU32(h.base, kRacerData, racer_data.address);
          std::vector<uint32_t> shapes;
          const float speeds[] = {0.0f,
                                  5.0f,
                                  30.0f,
                                  at_cap * 0.98f,
                                  at_cap * 1.02f,
                                  at_near_cap * 0.98f,
                                  at_near_cap * 1.02f,
                                  std::max(at_cap, at_near_cap) * 1.5f,
                                  1.0e6f,
                                  0.25f,
                                  std::bit_cast<float>(0x7FC00000u)};
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t vehicle = vehicles.address + 6272 * i;
            uint32_t shape = 0;
            if (hooks::NativeHeapActive() && r.Below(3) == 0) {
              shape = memory::Alloc(496, 0);
              car_objects::BuildBox(h, shape, r);
              shapes.push_back(shape);
            }
            guest::StoreU32(h.base, vehicle + 336, bodies.address + 512 * i);
            guest::StoreU32(h.base, vehicle + 340, 0);
            guest::StoreU32(h.base, vehicle + 344, shape);
            on_route(vehicle + 5236);
            for (uint32_t w = 0; w < 4; w++) {
              guest::StoreU32(h.base, bodies.address + 512 * i + 208 + 4 * w,
                              test_route::RandomFloat(r, 150.0f));
            }
            guest::StoreU32(h.base, vehicle + 6168, r.Below(3) == 0 ? 1 + r.Below(4) : 0);
            guest::StoreFloat(h.base, vehicle + 48, speeds[r.Below(11)]);
          }
          // The branches the radii take (counted on the native side, with the
          // originals' float steps).
          const auto speed_of = [&](uint32_t entry) {
            return ops::Lfs(h.base, vehicles.address +
                                        6272 * guest::LoadU32(h.base, entry + 20) + 48);
          };
          const auto first_flagged = [&] {
            for (uint32_t i = 0; i < count; i++) {
              if ((guest::LoadU32(h.base, manager.address + 4 + 28 * i) & 0x4) != 0) {
                return manager.address + 4 + 28 * i;
              }
            }
            return 0u;
          };
          if (v == 1) {
            double far_opp, far_traffic;
            if (two) {
              const bool second = (flags & 0x60) != 0 &&
                                  ops::Lfs(h.base, racer_data.address + 3012) <
                                      ops::Lfs(h.base, racer_data.address + 3984);
              second_ahead += second ? 1 : 0;
              double s = speed_of(manager.address + (second ? 32 : 4));
              const double o = speed_of(manager.address + (second ? 4 : 32));
              if (s < o) {
                s = o;
              }
              far_opp = ops::Fadds(ops::Fmuls(s, opponent_scale), far_base);
              far_traffic = ops::Fadds(ops::Fmuls(s, traffic_scale), far_base);
            } else {
              const double s = speed_of(first_flagged());
              far_opp = ops::Fmadds(s, opponent_scale, far_base);
              far_traffic = ops::Fmadds(s, traffic_scale, far_base);
            }
            capped += far_opp > cap ? 1 : 0;
            both_capped += far_opp > cap && far_traffic > near_cap ? 1 : 0;
          }
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = manager.address;
          GiveVehicleStandIn stand_in;
          switch (which) {
            case kNear:
              (v == 0 ? Original<__imp__sub_82172E60> : hooks::RaceWorldStreamNear)(t, h.base);
              break;
            case kCruise:
              (v == 0 ? Original<__imp__sub_82174200> : hooks::RaceWorldStreamCruise)(t, h.base);
              break;
            case kTwo:
              (v == 0 ? Original<__imp__sub_82172D20> : hooks::RaceWorldStreamTwoPlayers)(
                  t, h.base);
              break;
            case kStream:
              (v == 0 ? Original<__imp__sub_82174338> : hooks::RaceWorldStream)(t, h.base);
              break;
          }
          if (v == 1) {
            listed += guest::LoadU32(h.base, manager.address + 2364);
          }
          out[v].Word(uint32_t(g_give_log.size()));
          for (uint32_t w : g_give_log) {
            out[v].Word(w);
          }
          const auto name = [&](uint32_t w) {
            for (size_t i = 0; i < shapes.size(); i++) {
              if (w == shapes[i]) {
                return uint32_t(0xB10C0000u + i);
              }
            }
            return w;
          };
          for (uint32_t i = 0; i < 80; i++) {
            const uint32_t entry = manager.address + 4 + 28 * i;
            out[v].Bytes(Read(h.base, entry, 12));
            out[v].Float(guest::LoadU32(h.base, entry + 12));
            out[v].Bytes(Read(h.base, entry + 16, 12));
          }
          out[v].Bytes(Read(h.base, manager.address, 4));
          out[v].Bytes(Read(h.base, manager.address + 2244, 2400 - 2244));
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t vehicle = vehicles.address + 6272 * i;
            out[v].Word(name(guest::LoadU32(h.base, vehicle + 340)));
            out[v].Word(name(guest::LoadU32(h.base, vehicle + 344)));
            out[v].Word(guest::LoadU32(h.base, vehicle + 6148));
          }
          for (uint32_t s : shapes) {
            out[v].Word(memory::IsHeapBlock(s) ? 1 : 0);
            if (memory::IsHeapBlock(s)) {
              memory::Free(s);
            }
          }
          drivers_out(out[v]);
          // The last callee's temporaries.
          const bool cruise = which == kCruise || (which == kStream && (kind == 2 || kind == 3));
          const bool none = which == kStream && kind > 8;
          if (cruise) {
            out[v].Bytes(Read(h.base, sp - 160 - 272 + 80, 32));
          } else if (!none) {
            const uint32_t frame = sp - (two ? 128 : 160) - 464;
            out[v].Bytes(Read(h.base, frame + 80, 16));
            out[v].Floats(h.base, frame + 96, 32);
            out[v].Bytes(Read(h.base, frame + 224, 20));
            out[v].Floats(h.base, frame + 244, 7);
          }
        }
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
      results.Report();
      if (which == kStream) {
        ITC_LOG_INFO("harness: race_world: Stream's kinds 0 to 8: {} {} {} {} {} {} {} {} {} "
                     "cases, above 8 {}, random {}",
                     kinds[0], kinds[1], kinds[2], kinds[3], kinds[4], kinds[5], kinds[6],
                     kinds[7], kinds[8], kinds[9], kinds[10]);
      }
      ITC_LOG_INFO("harness: race_world: {}: {} cases with the far radii capped ({} both), {} "
                   "with the second player ahead, {} racers listed",
                   names[which], capped, both_capped, second_ahead, listed);
    }
  }
}

// --- The racers made, placed and settled: RaceWorld::OnLoaded,
// RaceStart::Place and PlaceRolling over a race's start world built from the
// existing cases' pieces: the course (the collision world harness's mesh and
// route, the route not marked loaded, so TrackCollision::Tick skips the
// world's update and only reads the racers' contacts), the car physics'
// fake world (its free objects the most racers a case gives vehicles to),
// the racer records' car as the vehicle harness's Vehicle::SetUp cases make
// it (vehicle_cases::FillSetUpCase; record 0's copied to the others; cases
// whose car data the gearing's set-up would read out of its tables with
// are left out, as there, by a first run of the car loader on a zeroed
// vehicle), the race data (zeroed: no traffic, no event; the stages, mode,
// kind, flags and the records' flags, start positions, speeds, items and
// distances), player 0's save, the data tables' items and the racers' input
// records. The seven vehicles, the AI drivers and the manager are made by
// the functions themselves (Vehicles::Create, AiDrivers::Create,
// RaceManager::Create through the allocation layer: each side frees them
// after, so the other gets the same blocks). AiDriver::DriveCar and
// AiDrivers::UpdateAll answer through stand-ins on both sides (their own
// harnesses check them): the calls' arguments, stack pointers and return
// addresses logged and compared, DriveCar's record filled with words from
// them. Vehicle::SetUp, ResetForCar, PlaceOnRoute, Vehicle::Tick,
// TrackCollision::Tick, AddRacers, ListRacers and NoteMarkedRacer run for
// real. Place's and PlaceRolling's cases start from the world OnLoaded's
// original leaves, then change the stage, mode, kind, items, the save and
// the start's globals, and flag entries 0x400. Compared exactly: the
// vehicles, their bodies and shapes, the fake world, the manager, the
// drivers, the input records, the records' fields, the race data's flags,
// the marked racer, the stand-ins' log and the frames' temporaries. ---

std::vector<uint32_t> g_start_log;
uint32_t g_start_calls = 0;
uint32_t StartMix(uint32_t x) {
  x ^= x >> 16;
  x *= 0x7FEB352Du;
  x ^= x >> 15;
  x *= 0x846CA68Bu;
  return x ^ (x >> 16);
}
void StartDriveCar(PPCContext& ctx, uint8_t* base) {
  g_start_log.insert(g_start_log.end(), {0xD0000000u, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                         ctx.r1.u32, uint32_t(ctx.lr)});
  // Moderate inputs: the steering, brake and throttle words small, the kind
  // byte +17 2 (an AI driver's), as DriveCar leaves it.
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, ctx.r3.u32 + 4 * i,
                    StartMix(g_start_calls * 7 + i + ctx.r4.u32 * 131 + ctx.r5.u32 * 977) &
                        0x3E00003Fu);
  }
  guest::StoreU8(base, ctx.r3.u32 + 17, 2);
  ctx.f1.f64 = double(g_start_calls);
  g_start_calls++;
}
void StartUpdateAll(PPCContext& ctx, uint8_t* base) {
  g_start_log.insert(g_start_log.end(), {0xD2000000u, ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = StartMix(g_start_calls++);
}
// While alive, the stand-ins answer and the log starts empty.
struct StartStandIns {
  StartStandIns() {
    hooks::g_drive_car_stand_in = StartDriveCar;
    hooks::g_update_all_stand_in = StartUpdateAll;
    Reset();
  }
  ~StartStandIns() {
    hooks::g_drive_car_stand_in = nullptr;
    hooks::g_update_all_stand_in = nullptr;
  }
  static void Reset() {
    g_start_log.clear();
    g_start_calls = 0;
  }
};

// sub_821743A8's stand-in for RaceWorld::Tick's cases (the original, never
// seen in play, would move cars to recorded places from random words): its
// arguments, stack pointer and return address logged with the start's
// stand-ins'.
void StandInRecorded(PPCContext& ctx, uint8_t*) {
  g_start_log.insert(g_start_log.end(),
                     {0xD3000000u, ctx.r3.u32, ctx.r1.u32, uint32_t(ctx.lr)});
}
struct RecordedStandIn {
  RecordedStandIn() { hooks::g_recorded_stand_in = StandInRecorded; }
  ~RecordedStandIn() { hooks::g_recorded_stand_in = nullptr; }
};

void StartCases(Context& h) {
  constexpr uint32_t kStartObject = 0x826C7820, kStartPosition = 0x826C6B0C;
  const Block world(car_objects::WorldBytes());
  const Block region(car_objects::VehicleRegionBytes());
  const Block course(TestCourseBytes());
  const Block table(4 * 0x10000);
  const Block race(0xA4000);
  const Block tables(0x40 + 4 * 3008);
  const Block save(8192);
  const Block inputs(8 * 32);
  const Block object(0x4000);
  if (!world.address || !region.address || !course.address || !table.address ||
      !race.address || !tables.address || !save.address || !inputs.address || !object.address) {
    ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
    return;
  }
  FillTestCourseTable(h.base, course.address, table.address);
  const uint32_t sp = CallContext(h).r1.u32;
  const uint32_t loaded_frame = sp - 352;
  const uint32_t setup_frame = loaded_frame - 224;
  const uint32_t items = tables.address + 0x40;
  const uint32_t records = race.address + 48;
  const uint32_t stages = race.address + 0x59620;
  const auto globals = [&] {
    auto ranges = car_objects::UpdateGlobals();
    ranges.insert(ranges.end(), {{0x826C2D80, 0x100},
                                 {0x826C5B00, 0x80},
                                 {0x826C6070, 0x10},
                                 {kMarkedRacer, 16},
                                 {rw::kManager, 4},
                                 {kStartObject, 8},
                                 {kStartPosition, 8}});
    return SavedBytes(h.base, ranges);
  };
  // A random position on the course (2 sections of 6 slices).
  const auto position = [](Random& r) { return (r.Below(2) << 16) | r.Below(6); };
  // The world for one side; false when the case is left out (`check`: the
  // loader's first run). `racers` gets the records' count.
  const auto build = [&](Random& r, uint64_t seed, bool check, uint32_t* count) {
    WriteRandom(h.base, course.address, course.bytes, r);
    BuildTestCourse(h.base, course.address, table.address, r);
    guest::StoreU32(h.base, track_route::kRoute + 8, 0);
    std::memset(h.base + race.address, 0, race.bytes);
    std::vector<uint32_t> given;
    vehicle_cases::FillSetUpCase(h, world.address, region.address, records, setup_frame,
                                 loaded_frame, seed, false, r, given);
    if (check) {
      std::memset(h.base + region.address, 0, region.bytes);
      const bool sane = vehicle_cases::SetUpCaseSane(h, region.address, records, setup_frame);
      vehicle_cases::FreeSetUpCase(given);
      return sane;
    }
    vehicle_cases::FreeSetUpCase(given);
    // The world's free objects: the most racers with vehicles.
    uint32_t free_objects = 0;
    const uint32_t w = guest::LoadU32(h.base, kWorldPointer);
    for (uint32_t o = guest::LoadU32(h.base, w + 24); o != 0 && free_objects < 8;
         o = guest::LoadU32(h.base, o + 228)) {
      free_objects++;
    }
    const uint32_t n = 1 + r.Below(std::max(1u, std::min(free_objects, 4u)));
    *count = n;
    guest::StoreU32(h.base, records + 0x595E0, n);
    const uint32_t player = r.Below(n);
    for (uint32_t i = 0; i < n; i++) {
      const uint32_t record = records + 3936 * i;
      if (i != 0) {
        std::memcpy(h.base + record, h.base + records, 3936);
      }
      const uint32_t flags[] = {0x2, 0x2, 0x2, 0x80, 0x0, 0x4, 0x102};
      guest::StoreU32(h.base, record + 3888, i == player ? 0x1 : flags[r.Below(7)]);
      guest::StoreU32(h.base, record + 3904, position(r));
      guest::StoreFloat(h.base, record + 3908, float(r.Below(120)) * 0.5f);
      guest::StoreU32(h.base, record + 3912, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(4));
      guest::StoreU32(h.base, record + 3916, 0xFFFFFFFFu);
      guest::StoreFloat(h.base, record + 3920, float(r.Below(1000)) * 0.25f);
    }
    const uint32_t stage = r.Below(3);
    guest::StoreU32(h.base, stages + 804, stage);
    guest::StoreU32(h.base, stages + 800, 3);
    for (uint32_t s = 0; s < 3; s++) {
      guest::StoreU32(h.base, stages + 80 * s + 0, r.Below(5));
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(h.base, stages + 80 * s + 44 + 8 * i,
                        r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(n));
      }
    }
    const uint32_t modes[] = {0, 1, 6, 6};
    guest::StoreU32(h.base, race.address + 0xA3AF0, modes[r.Below(4)]);
    const uint32_t kinds[] = {0, 1, 2, 3, 8};
    guest::StoreU32(h.base, race.address + 0xA3AF4, kinds[r.Below(5)]);
    guest::StoreU32(h.base, race.address + 0x59AE4, uint32_t(r.Next()) & ~0x2200u);
    // The items' branch kinds (+36) and AiDriver::Setup's table (+120).
    WriteRandom(h.base, tables.address, tables.bytes, r);
    guest::StoreU32(h.base, tables.address + 16, items);
    for (uint32_t k = 0; k < 4; k++) {
      const uint8_t branch[] = {0, 1, 2, 3, 0xFF, 0xFE};
      guest::StoreU8(h.base, items + 3008 * k + 36, branch[r.Below(6)]);
      for (uint32_t i = 0; i < 7; i++) {
        guest::StoreU16(h.base, items + 3008 * k + 120 + 2 * i,
                        uint16_t(int32_t(r.Below(300)) - 100));
      }
    }
    WriteRandom(h.base, inputs.address, inputs.bytes, r);
    for (uint32_t k = 0; k < 8; k++) {
      guest::StoreU32(h.base, kRacerInputs + 4 * k, inputs.address + 32 * k);
    }
    WriteRandom(h.base, save.address, save.bytes, r);
    const uint32_t backs[] = {1, 1, 0, 1000, 2};
    guest::StoreU32(h.base, save.address + 6100, backs[r.Below(5)]);
    guest::StoreU32(h.base, kSaves, save.address);
    guest::StoreU32(h.base, data_tables::kTables, tables.address);
    guest::StoreU32(h.base, rw::kRaceData, race.address);
    guest::StoreU32(h.base, rw::kRaceEvent, 0);
    guest::StoreU32(h.base, rw::kManager, 0);
    return true;
  };
  // What a side leaves.
  const auto outputs = [&](Outputs& out, uint32_t count, uint32_t frame, uint32_t bytes) {
    const uint32_t vehicles = guest::LoadU32(h.base, kVehicles);
    const uint32_t drivers = guest::LoadU32(h.base, ai_drivers::kDrivers);
    const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
    out.Word(vehicles);
    out.Word(drivers);
    out.Word(manager);
    if (vehicles != 0) {
      out.Bytes(Read(h.base, vehicles, kVehiclesBytes));
      for (uint32_t i = 0; i < 7; i++) {
        for (uint32_t at : {336u, 340u, 344u}) {
          const uint32_t p = guest::LoadU32(h.base, vehicles + 6272 * i + at);
          if (p != 0 && p >= 0x10000) {
            out.Bytes(Read(h.base, p, 512));
          }
        }
      }
    }
    out.Bytes(Read(h.base, world.address, world.bytes));
    if (manager != 0) {
      out.Bytes(Read(h.base, manager, 2400 + 2 * 1200));
      out.Bytes(Read(h.base, manager + 360000, 64));
    }
    if (drivers != 0) {
      out.Bytes(Read(h.base, drivers, kDriversBytes));
    }
    out.Bytes(Read(h.base, inputs.address, inputs.bytes));
    for (uint32_t i = 0; i < count; i++) {
      out.Bytes(Read(h.base, records + 3936 * i + 3888, 48));
    }
    out.Word(guest::LoadU32(h.base, race.address + 0x59AE4));
    out.Bytes(Read(h.base, kMarkedRacer, 12));
    out.Bytes(Read(h.base, frame + 80, bytes));
    out.Word(uint32_t(g_start_log.size()));
    for (uint32_t w : g_start_log) {
      out.Word(w);
    }
  };
  // Frees what the functions made.
  const auto teardown = [&] {
    PPCContext t = CallContext(h);
    t.fpscr.disableFlushModeUnconditional();
    if (guest::LoadU32(h.base, kVehicles) != 0 && !vehicle::VehiclesDestroy(h.base)) {
      __imp__sub_820C3B78(t, h.base);
    }
    uint64_t r3 = 0;
    t = CallContext(h);
    if (guest::LoadU32(h.base, ai_drivers::kDrivers) != 0 && !ai_drivers::Destroy(h.base, &r3)) {
      __imp__sub_8213B2B8(t, h.base);
    }
    const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
    if (manager != 0 && !memory::crt::OperatorDelete(manager)) {
      t = CallContext(h);
      t.r3.u64 = manager;
      __imp__sub_825A70E8(t, h.base);
    }
    // The shape records' copies (+36, +40) the fake world's objects took
    // (AddObject), freed so the other side's blocks come from the same
    // free ranges.
    const uint32_t w = guest::LoadU32(h.base, kWorldPointer);
    for (uint32_t i = 0; i < guest::LoadU32(h.base, w + 12) && i < 8; i++) {
      const uint32_t o = guest::LoadU32(h.base, w + 16) + 320 * i;
      for (uint32_t at : {36u, 40u}) {
        const uint32_t copy = guest::LoadU32(h.base, o + at);
        if (copy != 0 && memory::IsHeapBlock(copy)) {
          memory::Free(copy);
        }
      }
    }
  };
  // RaceWorld::OnLoaded's original, as the place cases' starting world.
  const auto loaded = [&] {
    PPCContext t = CallContext(h);
    t.fpscr.disableFlushModeUnconditional();
    t.r3.u64 = h.data;
    Original<__imp__sub_82173A90>(t, h.base);
  };
  uint32_t racers = 0, ticks = 0, kind8 = 0, long_settles = 0;
  {
    Results results("RaceWorld::OnLoaded (sub_82173A90)");
    uint32_t left_out = 0;
    uint32_t held = 0;
    for (uint64_t seed = 0; seed < 150; seed++) {
      Outputs out[2];
      bool skip = false;
      uint32_t count = 0;
      for (int v = -1; v < 2 && !skip; v++) {
        Random r(seed * 179 + 103);
        auto saved = globals();
        StartStandIns stand_ins;
        if (!build(r, seed, v < 0, &count)) {
          skip = true;
          left_out++;
          continue;
        }
        if (v < 0) {
          continue;
        }
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = h.data;
        (v == 0 ? Original<__imp__sub_82173A90> : hooks::RaceWorldOnLoaded)(t, h.base);
        if (v == 1) {
          const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
          racers += manager != 0 ? guest::LoadU32(h.base, manager + 2364) : 0;
          held += (guest::LoadU32(h.base, race.address + 0x59AE4) & 0x2000) != 0 ? 1 : 0;
        }
        outputs(out[v], count, loaded_frame, 160);
        teardown();
      }
      if (!skip) {
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: RaceWorld::OnLoaded: {} racers with vehicles, {} cases "
                 "held, {} cases left out (car data the gearing's set-up would read out of its "
                 "tables with)",
                 racers, held, left_out);
  }
  // The changes Place's and PlaceRolling's cases make to the loaded world.
  const auto vary = [&](Random& r, uint32_t count) {
    const uint32_t stage = guest::LoadU32(h.base, stages + 804);
    guest::StoreU32(h.base, stages + 80 * stage + 0, r.Below(5));
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreU32(h.base, stages + 80 * stage + 44 + 8 * i,
                      r.Below(5) == 0 ? 0xFFFFFFFFu : r.Below(count));
    }
    const uint32_t modes[] = {0, 1, 1, 6};
    guest::StoreU32(h.base, race.address + 0xA3AF0, modes[r.Below(4)]);
    const uint32_t kinds[] = {0, 1, 2, 3, 8, 8};
    guest::StoreU32(h.base, race.address + 0xA3AF4, kinds[r.Below(6)]);
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(h.base, records + 3936 * i + 3912,
                      r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(4));
    }
    const uint32_t backs[] = {1, 1, 0, 1000, 2};
    guest::StoreU32(h.base, save.address + 6100, backs[r.Below(5)]);
    // The kind 8 start: an object's (+9624, its +7244) or the halfwords.
    guest::StoreU32(h.base, kStartObject + 4, r.Below(2) == 0 ? 0 : uint32_t(r.Next()) | 1);
    guest::StoreU32(h.base, object.address + 9624, object.address + 0x200);
    guest::StoreU32(h.base, object.address + 0x200 + 7244, position(r));
    guest::StoreU16(h.base, kStartPosition + 2, uint16_t(r.Below(2)));
    guest::StoreU16(h.base, kStartPosition + 4, uint16_t(r.Below(6)));
    // Entries flagged 0x400 (the rolling start's).
    const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
    for (uint32_t i = 0; i < guest::LoadU32(h.base, manager + 2368) && i < 80; i++) {
      const uint32_t entry = manager + 4 + 28 * i;
      if (r.Below(2) == 0) {
        guest::StoreU32(h.base, entry, guest::LoadU32(h.base, entry) | 0x400);
      }
    }
  };
  for (const bool rolling : {false, true}) {
    Results results(rolling ? "RaceStart::PlaceRolling (sub_821662E0)"
                            : "RaceStart::Place (sub_82165E20)");
    for (uint64_t seed = 0; seed < 150; seed++) {
      Outputs out[2];
      bool skip = false;
      uint32_t count = 0;
      for (int v = -1; v < 2 && !skip; v++) {
        Random r(seed * 181 + 107 + (rolling ? 5000 : 0));
        auto saved = globals();
        StartStandIns stand_ins;
        if (!build(r, seed + 1000, v < 0, &count)) {
          skip = true;
          continue;
        }
        if (v < 0) {
          continue;
        }
        loaded();
        vary(r, count);
        // Place's cases: now and then no player (the record's flag 0x1
        // cleared; PlaceRolling reads its entry, always there).
        if (!rolling && r.Below(5) == 0) {
          for (uint32_t i = 0; i < count; i++) {
            const uint32_t at = records + 3936 * i + 3888;
            guest::StoreU32(h.base, at, guest::LoadU32(h.base, at) & ~0x1u);
          }
        }
        const uint32_t settle = r.Below(3) == 0 ? 0 : 1;
        const uint32_t back = r.Below(3) == 0 ? 1 : 0;
        StartStandIns::Reset();
        WriteRandom(h.base, sp - 512, 512, r);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = object.address;
        t.r4.u64 = settle;
        t.r5.u64 = back;
        if (rolling) {
          (v == 0 ? Original<__imp__sub_821662E0> : hooks::RaceWorldPlaceRolling)(t, h.base);
        } else {
          (v == 0 ? Original<__imp__sub_82165E20> : hooks::RaceWorldPlace)(t, h.base);
        }
        if (v == 1) {
          ticks += settle;
          kind8 += guest::LoadU32(h.base, race.address + 0xA3AF4) == 8 ? 1 : 0;
          long_settles += settle != 0 && guest::LoadU32(h.base, race.address + 0xA3AF0) == 6 &&
                                  guest::LoadU32(h.base, sp - 400 + 160) != 1 && !rolling
                              ? 1
                              : 0;
        }
        // Place's frame temporaries (+80 to +240) and the words it stores in
        // its caller's frame (+20, +28); for PlaceRolling, its own
        // temporaries (+80 and +112) and Place's words in its frame (Place's
        // own frame is below it, where PlaceRolling's later callees' frames
        // go: not compared).
        const uint32_t place_sp = rolling ? sp - 272 : sp;
        outputs(out[v], count, rolling ? sp - 272 : sp - 400, rolling ? 52 : 160);
        out[v].Bytes(Read(h.base, place_sp + 16, 16));
        teardown();
      }
      if (!skip) {
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
    }
    results.Report();
  }
  ITC_LOG_INFO("harness: race_world: RaceStart::Place and PlaceRolling: {} cases settled ({} "
               "for 300 ticks), {} of kind 8",
               ticks, long_settles, kind8);
  {
    // RaceStates::RollingStartTick (src/race_states/, its own harness's
    // other cases in harnesses/race_states.cpp) on the same world as
    // PlaceRolling's cases, the race object its machine: the fades done or
    // not (their counts at 0x826A50DC and 0x826A50E4, put back after), the
    // race data's flag 0x100 set or not, the dispatcher's ring (a block of
    // its own) with a random count and start. Its original with
    // --native_race_states off; RaceManager::Refresh and
    // RaceStart::PlaceRolling are their adapters on both sides. Compared as
    // PlaceRolling's (its frame 112 bytes down, PlaceRolling's below it),
    // with the ring and the machine's +9612.
    Results results("RaceStates::RollingStartTick (sub_82166E50)");
    const Block dispatcher(1024);
    uint32_t placed = 0;
    for (uint64_t seed = 0; seed < 20 && dispatcher.address; seed++) {
      Outputs out[2];
      bool skip = false;
      uint32_t count = 0;
      for (int v = -1; v < 2 && !skip; v++) {
        Random r(seed * 191 + 113);
        auto saved = globals();
        const auto fades = SavedBytes(h.base, {{0x826A50DC, 16}});
        StartStandIns stand_ins;
        if (!build(r, seed + 3000, v < 0, &count)) {
          skip = true;
          continue;
        }
        if (v < 0) {
          continue;
        }
        loaded();
        vary(r, count);
        const bool done = r.Below(4) != 0;
        guest::StoreU32(h.base, 0x826A50DC, done ? 0 : r.Below(2));
        guest::StoreU32(h.base, 0x826A50E4, done ? 0 : 1);
        const uint32_t flags = race.address + 0x59AE4;
        guest::StoreU32(h.base, flags, (guest::LoadU32(h.base, flags) & ~0x100u) |
                                           (r.Below(2) ? 0x100u : 0u));
        WriteRandom(h.base, dispatcher.address, 1024, r);
        guest::StoreU32(h.base, dispatcher.address + 804, r.Below(50));
        guest::StoreU32(h.base, dispatcher.address + 808, r.Below(50));
        guest::StoreU32(h.base, object.address + 9612, uint32_t(r.Next()));
        StartStandIns::Reset();
        WriteRandom(h.base, sp - 512, 512, r);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = object.address;
        t.r4.u64 = dispatcher.address;
        if (v == 0) {
          ITC_FLAG(native_race_states) = false;
          __imp__sub_82166E50(t, h.base);
          ITC_FLAG(native_race_states) = true;
        } else {
          hooks::RaceStatesRollingStartTick(t, h.base);
          placed += done ? 1 : 0;
        }
        outputs(out[v], count, sp - 112 - 272, 52);
        out[v].Bytes(Read(h.base, dispatcher.address, 1024));
        out[v].Word(guest::LoadU32(h.base, object.address + 9612));
        teardown();
      }
      if (!skip) {
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: RollingStartTick placed the racers in {} cases", placed);
  }
  // RaceManager::StreamOpponents on the loaded world, where
  // RaceManager::GiveVehicle's original (never seen in play) can give a
  // vehicle for real: most cases first take a racer's vehicle away as the
  // function would (its original Vehicle::RemoveFromWorld, its slot word 0,
  // +20 -1, flag 0x40 cleared; both sides alike), so a slot and a world
  // object are free, or, with only the player's and a world object free,
  // add an entry without a vehicle beside the player; now and then entries flagged 0x20, 0x8, 0x10, 0x400 or
  // 0x80; +2364 0 (as RaceManager::StreamCruise leaves it); the radii wide
  // or narrow. Compared: what the place cases compare, with the frame's
  // temporaries (the vectors and the sorted list's keys as floats) and the
  // input record.
  {
    Results results("RaceManager::StreamOpponents, vehicles given (sub_82172118)");
    const uint32_t frame = sp - 1920;
    uint32_t given = 0, taken = 0, added = 0;
    for (uint64_t seed = 0; seed < 200; seed++) {
      Outputs out[2];
      bool skip = false;
      uint32_t count = 0;
      for (int v = -1; v < 2 && !skip; v++) {
        Random r(seed * 199 + 131);
        auto saved = globals();
        StartStandIns stand_ins;
        if (!build(r, seed + 3000, v < 0, &count)) {
          skip = true;
          continue;
        }
        if (v < 0) {
          continue;
        }
        loaded();
        const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
        const uint32_t vehicles = guest::LoadU32(h.base, kVehicles);
        const uint32_t entries = guest::LoadU32(h.base, manager + 2368);
        uint32_t player = 0;
        std::vector<uint32_t> others;
        for (uint32_t i = 0; i < entries && i < 80; i++) {
          const uint32_t entry = manager + 4 + 28 * i;
          if (player == 0 && (guest::LoadU32(h.base, entry) & 0x4) != 0) {
            player = entry;
          } else if (guest::LoadU32(h.base, entry + 20) != 0xFFFFFFFFu) {
            others.push_back(entry);
          }
        }
        if (player == 0 || guest::LoadU32(h.base, player + 20) == 0xFFFFFFFFu) {
          // Both sides alike: the case is left out.
          skip = true;
          teardown();
          continue;
        }
        // The fake world's free objects left.
        uint32_t free_objects = 0;
        {
          const uint32_t w = guest::LoadU32(h.base, kWorldPointer);
          for (uint32_t o = guest::LoadU32(h.base, w + 24); o != 0 && free_objects < 8;
               o = guest::LoadU32(h.base, o + 228)) {
            free_objects++;
          }
        }
        if (others.empty() && free_objects != 0 && entries < 80 && r.Below(4) != 0) {
          // An entry without a vehicle beside the player (its record and
          // position), as AddRacers makes one: flags 1, +4 its number, +12
          // 0.0, +16 0, +20 -1.
          const uint32_t entry = manager + 4 + 28 * entries;
          guest::StoreU32(h.base, entry + 0, 1);
          guest::StoreU32(h.base, entry + 4, entries);
          guest::StoreU32(h.base, entry + 8, guest::LoadU32(h.base, player + 8));
          guest::StoreU32(h.base, entry + 12, 0);
          guest::StoreU32(h.base, entry + 16, 0);
          guest::StoreU32(h.base, entry + 20, 0xFFFFFFFFu);
          guest::StoreU32(h.base, entry + 24, guest::LoadU32(h.base, player + 24));
          guest::StoreU32(h.base, manager + 2368, entries + 1);
          added += v == 1 ? 1 : 0;
        } else if (!others.empty() && r.Below(4) != 0) {
          const uint32_t entry = others[r.Below(uint32_t(others.size()))];
          const uint32_t which = guest::LoadU32(h.base, entry + 20);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = vehicles + 6272 * which;
          __imp__sub_820C20E0(t, h.base);
          guest::StoreU32(h.base, manager + 2336 + 4 * which, 0);
          guest::StoreU32(h.base, entry + 20, 0xFFFFFFFFu);
          guest::StoreU32(h.base, entry, guest::LoadU32(h.base, entry) & ~0x40u);
          taken += v == 1 ? 1 : 0;
        }
        for (uint32_t i = 0; i < guest::LoadU32(h.base, manager + 2368) && i < 80; i++) {
          const uint32_t entry = manager + 4 + 28 * i;
          if (entry != player && r.Below(6) == 0) {
            const uint32_t flags[] = {0x20, 0x8, 0x10, 0x400, 0x80};
            guest::StoreU32(h.base, entry, guest::LoadU32(h.base, entry) | flags[r.Below(5)]);
          }
        }
        // Both sections on one branch (BranchesMeet then answers 1), now and
        // then.
        if (r.Below(4) != 0) {
          const uint32_t file = guest::LoadU32(h.base, track_route::kRoute);
          const uint32_t sections = guest::LoadU32(h.base, file + 12);
          const uint8_t branch = guest::LoadU8(h.base, guest::LoadU32(h.base, sections) + 3);
          guest::StoreU8(h.base, guest::LoadU32(h.base, sections + 4) + 3, branch);
        }
        guest::StoreU32(h.base, manager + 2364, 0);
        uint32_t empty_before = 0;
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t slot = guest::LoadU32(h.base, manager + 2336 + 4 * k);
          empty_before += slot == 0 || slot == 0xFFFFFFFFu ? 1 : 0;
        }
        StartStandIns::Reset();
        WriteRandom(h.base, sp - 2048, 2048, r);
        const float spawns[] = {1.0e9f, 1.0e9f, 30.0f, 0.5f};
        const float despawns[] = {2.0e9f, 2.0e9f, 60.0f, 0.25f};
        const uint32_t radius = r.Below(4);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager;
        t.r4.u64 = player;
        t.f1.f64 = double(spawns[radius]);
        t.f2.f64 = double(despawns[radius]);
        (v == 0 ? Original<__imp__sub_82172118> : hooks::RaceWorldStreamOpponents)(t, h.base);
        if (v == 1) {
          uint32_t empty_after = 0;
          for (uint32_t k = 0; k < 7; k++) {
            const uint32_t slot = guest::LoadU32(h.base, manager + 2336 + 4 * k);
            empty_after += slot == 0 || slot == 0xFFFFFFFFu ? 1 : 0;
          }
          given += empty_after < empty_before ? 1 : 0;
        }
        outputs(out[v], count, frame, 16);
        out[v].Floats(h.base, frame + 96, 12);
        out[v].Bytes(Read(h.base, frame + 144, 20));
        for (uint32_t k = 0; k < 100; k++) {
          const uint32_t node = frame + 176 + 16 * k;
          out[v].Word(guest::LoadU32(h.base, node + 0));
          out[v].Float(guest::LoadU32(h.base, node + 4));
          out[v].Word(guest::LoadU32(h.base, node + 8));
          out[v].Word(guest::LoadU32(h.base, node + 12));
        }
        out[v].Bytes(Read(h.base, frame + 1776, 12));
        teardown();
      }
      if (!skip) {
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: StreamOpponents on the loaded world: {} cases took a "
                 "racer's vehicle first, {} added an entry without one, {} gave vehicles",
                 taken, added, given);
  }
  // RaceManager::StreamTraffic on the loaded world, where a new traffic
  // car's vehicle is set up for real (Vehicle::SetUp or ResetForCar,
  // PlaceOnRoute, the physics step, VehicleStatus::Update, AiDriver::Setup):
  // +2380 1 to 3, +2388 90 (now and then 89), the three traffic entries free,
  // or the second in use with a racer's vehicle taken over (as the
  // function's own would be; now and then beyond the far radius, so it
  // loses it); a world object freed first when none is (a racer's vehicle
  // taken away, its original Vehicle::RemoveFromWorld, as the
  // StreamOpponents cases do); the free slot words 0 or -1 (Vehicle::SetUp:
  // the body it replaces freed after each side, so both get the same
  // blocks); the traffic records (+2244) and numbers (+16, the race data's
  // +0xA3AE0) small; the race data's flags 0x1000 or not (0x2000, 0x4000
  // cleared); the player's car's +6168, speed (+48) and +6224; the second
  // player (another racer, or the player's own entry) or none; the racers' bodies moved 5,000 away (x) or not (a racer
  // near the spot refuses it); the racers' list (+2308) the entries with a
  // vehicle, as RaceManager::StreamOpponents leaves it; the radii.
  // Compared: what the StreamOpponents
  // cases compare, with the frame's temporaries (+96 to +271 as floats, the
  // input record at +224 exactly).
  {
    Results results("RaceManager::StreamTraffic, traffic given (sub_821724F8)");
    const uint32_t frame = sp - 464;
    uint32_t given = 0, set_up = 0, kept = 0, lost = 0, refused = 0, seconds = 0;
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      bool skip = false;
      uint32_t count = 0;
      for (int v = -1; v < 2 && !skip; v++) {
        Random r(seed * 223 + 151);
        auto saved = globals();
        StartStandIns stand_ins;
        if (!build(r, seed + 4000, v < 0, &count)) {
          skip = true;
          continue;
        }
        if (v < 0) {
          continue;
        }
        loaded();
        const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
        const uint32_t vehicles = guest::LoadU32(h.base, kVehicles);
        const uint32_t entries = guest::LoadU32(h.base, manager + 2368);
        uint32_t player = 0;
        std::vector<uint32_t> others;
        for (uint32_t i = 0; i < entries && i < 77; i++) {
          const uint32_t entry = manager + 4 + 28 * i;
          if (player == 0 && (guest::LoadU32(h.base, entry) & 0x4) != 0) {
            player = entry;
          } else if (guest::LoadU32(h.base, entry + 20) != 0xFFFFFFFFu) {
            others.push_back(entry);
          }
        }
        if (player == 0 || guest::LoadU32(h.base, player + 20) == 0xFFFFFFFFu) {
          // Both sides alike: the case is left out.
          skip = true;
          teardown();
          continue;
        }
        uint32_t free_objects = 0;
        {
          const uint32_t w = guest::LoadU32(h.base, kWorldPointer);
          for (uint32_t o = guest::LoadU32(h.base, w + 24); o != 0 && free_objects < 8;
               o = guest::LoadU32(h.base, o + 228)) {
            free_objects++;
          }
        }
        for (uint32_t t = 0; t < 3; t++) {
          guest::StoreU32(h.base, manager + 2160 + 28 * t, 0);
          guest::StoreU32(h.base, manager + 2160 + 28 * t + 20, 0xFFFFFFFFu);
        }
        // A racer's vehicle taken over by the second traffic entry.
        uint32_t second = 0;
        if (!others.empty() && r.Below(2) == 0) {
          const size_t pick = r.Below(uint32_t(others.size()));
          const uint32_t entry = others[pick];
          others.erase(others.begin() + std::ptrdiff_t(pick));
          const uint32_t traffic = manager + 2160 + 28;
          guest::StoreU32(h.base, traffic + 0, 578);
          guest::StoreU32(h.base, traffic + 20, guest::LoadU32(h.base, entry + 20));
          guest::StoreU32(h.base, entry + 20, 0xFFFFFFFFu);
          guest::StoreU32(h.base, entry, guest::LoadU32(h.base, entry) & ~0x40u);
          kept += v == 1 ? 1 : 0;
        } else if (free_objects == 0 && !others.empty()) {
          const uint32_t entry = others.back();
          others.pop_back();
          const uint32_t which = guest::LoadU32(h.base, entry + 20);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          t.r3.u64 = vehicles + 6272 * which;
          __imp__sub_820C20E0(t, h.base);
          guest::StoreU32(h.base, manager + 2336 + 4 * which, 0);
          guest::StoreU32(h.base, entry + 20, 0xFFFFFFFFu);
          guest::StoreU32(h.base, entry, guest::LoadU32(h.base, entry) & ~0x40u);
          free_objects++;
        }
        // The second player: another racer, or the player's own entry with
        // none other.
        if (r.Below(3) == 0) {
          second = others.empty() ? player : others[r.Below(uint32_t(others.size()))];
        }
        // The racers' list as RaceManager::StreamOpponents leaves it before
        // (the entries with a vehicle; +2364, +2372 their count).
        {
          uint32_t n = 0;
          for (uint32_t i = 0; i < entries && i < 77; i++) {
            const uint32_t entry = manager + 4 + 28 * i;
            if (guest::LoadU32(h.base, entry + 20) != 0xFFFFFFFFu && n < 7) {
              guest::StoreU32(h.base, manager + 2308 + 4 * n, entry);
              n++;
            }
          }
          guest::StoreU32(h.base, manager + 2364, n);
          guest::StoreU32(h.base, manager + 2372, n);
        }
        guest::StoreU32(h.base, manager + 2380, 1 + r.Below(3));
        // Without a free world object (a lone player), no new car.
        guest::StoreU32(h.base, manager + 2388,
                        free_objects == 0 ? 0 : (r.Below(6) == 0 ? 89 : 90));
        for (uint32_t k = 0; k < 16; k++) {
          guest::StoreU32(h.base, manager + 2244 + 4 * k, r.Below(count));
        }
        guest::StoreU32(h.base, race.address + 0xA3AE0, r.Below(50));
        for (uint32_t t = 0; t < 3; t++) {
          guest::StoreU32(h.base, manager + 2160 + 28 * t + 16, r.Below(100));
        }
        // The free slots: 0 (as RaceManager::Create leaves them) or -1.
        std::vector<std::pair<uint32_t, uint32_t>> bodies;
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t slot = manager + 2336 + 4 * k;
          if (guest::LoadU32(h.base, slot) == 0 && r.Below(3) == 0) {
            guest::StoreU32(h.base, slot, 0xFFFFFFFFu);
          }
          bodies.push_back({k, guest::LoadU32(h.base, vehicles + 6272 * k + 336)});
        }
        const uint32_t at = race.address + 0x59AE4;
        guest::StoreU32(h.base, at, (guest::LoadU32(h.base, at) & ~0x7000u) |
                                        (r.Below(3) == 0 ? 0x1000 : 0));
        const uint32_t car = vehicles + 6272 * guest::LoadU32(h.base, player + 20);
        guest::StoreU32(h.base, car + 6168, r.Below(3) == 0 ? 1 : 0);
        const float speeds[] = {0.0f, 5.0f, 15.0f, 40.0f};
        guest::StoreFloat(h.base, car + 48, speeds[r.Below(4)]);
        const float sides[] = {-1.0f, 0.0f, 1.0f};
        guest::StoreFloat(h.base, car + 6224, sides[r.Below(3)]);
        if (r.Below(2) == 0) {
          for (uint32_t i = 0; i < 7; i++) {
            const uint32_t body = guest::LoadU32(h.base, vehicles + 6272 * i + 336);
            if (body != 0) {
              guest::StoreFloat(h.base, body + 208, guest::LoadFloat(h.base, body + 208) + 5000.0f);
            }
          }
        }
        const float inners[] = {10.0f, 30.0f, 60.0f};
        const float outers[] = {0.5f, 100.0f, 1.0e9f};
        const float inner = inners[r.Below(3)];
        const float outer = outers[r.Below(3)];
        StartStandIns::Reset();
        WriteRandom(h.base, sp - 2048, 2048, r);
        const uint32_t traffic_before = guest::LoadU32(h.base, manager + 2160 + 28 + 20);
        const uint32_t ticks_before = guest::LoadU32(h.base, manager + 2388);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = manager;
        t.r4.u64 = player;
        t.r5.u64 = second;
        t.f1.f64 = double(inner);
        t.f2.f64 = double(outer);
        std::vector<uint32_t> slot_words;
        for (uint32_t k = 0; k < 7; k++) {
          slot_words.push_back(guest::LoadU32(h.base, manager + 2336 + 4 * k));
        }
        (v == 0 ? Original<__imp__sub_821724F8> : hooks::RaceWorldStreamTraffic)(t, h.base);
        // The bodies Vehicle::SetUp replaced, freed.
        for (const auto& [k, old] : bodies) {
          const uint32_t now = guest::LoadU32(h.base, vehicles + 6272 * k + 336);
          if (now != old && old != 0 && memory::IsHeapBlock(old)) {
            memory::Free(old);
          }
        }
        if (v == 1) {
          bool gave = false;
          for (uint32_t k = 0; k < 7; k++) {
            const uint32_t word = guest::LoadU32(h.base, manager + 2336 + 4 * k);
            if ((slot_words[k] == 0 || slot_words[k] == 0xFFFFFFFFu) && word == 1) {
              given++;
              gave = true;
              set_up += slot_words[k] == 0xFFFFFFFFu ? 1 : 0;
            }
          }
          refused += !gave && ticks_before == 90 &&
                             guest::LoadU32(h.base, manager + 2388) == 0
                         ? 1
                         : 0;
          seconds += second != 0 ? 1 : 0;
          lost += traffic_before != 0xFFFFFFFFu &&
                          guest::LoadU32(h.base, manager + 2160 + 28 + 20) == 0xFFFFFFFFu
                      ? 1
                      : 0;
        }
        outputs(out[v], count, frame, 16);
        out[v].Floats(h.base, frame + 96, 32);
        out[v].Bytes(Read(h.base, frame + 224, 20));
        out[v].Floats(h.base, frame + 244, 7);
        teardown();
      }
      if (!skip) {
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: StreamTraffic on the loaded world: {} traffic cars given "
                 "({} by Vehicle::SetUp), {} spots refused (a racer near), {} cases with a second "
                 "player, {} with a traffic car taken over ({} lost it)",
                 given, set_up, refused, seconds, kept, lost);
  }
  // RaceWorld::Tick on the loaded world: its callees run for real
  // (RaceManager::Stream by the race's kind with StreamOpponents,
  // StreamTraffic and MoveUnseen, RaceManager::TickVehicles,
  // TrackCollision::Tick, RaceManager::RefreshVehicles), AiDrivers::UpdateAll
  // through its stand-in, as the start's cases, and sub_821743A8 (never seen
  // in play) through one of its own (its arguments, stack pointer and return
  // address logged). The world's +9608 set or not; the race data's flags
  // 0x20 or not; the byte at 0x826C6B0C 0, 1, 2 or 255, the word at
  // 0x826C7824 0 or not; the racer records' flags as the start's cases make
  // them (each with 0x1 or 0x80 has an entry); [0x826D82E8] a block whose
  // words +2972 + 972 x the record numbers are cleared; the kind 0 to 9
  // (5 only with the first two entries holding vehicles; StreamTraffic's new
  // car now and then: +2388 90 with a world object to spare, the bodies
  // Vehicle::SetUp replaces freed after each side). Compared: what the start's cases compare, with the
  // [0x826D82E8] block and the stand-ins' log.
  {
    const Block tick_world(0x2600);
    const Block racer_data(0x2000);
    if (!tick_world.address || !racer_data.address) {
      ITC_LOG_ERROR("harness: race_world: no block from the allocation layer");
      return;
    }
    constexpr uint32_t kRacerData = 0x826D82E8;
    Results results("RaceWorld::Tick (sub_82174BC0)");
    const uint32_t frame = sp - 128;
    uint32_t cleared = 0, recorded = 0, streamed = 0, set = 0;
    for (uint64_t seed = 0; seed < 160; seed++) {
      Outputs out[2];
      bool skip = false;
      uint32_t count = 0;
      for (int v = -1; v < 2 && !skip; v++) {
        Random r(seed * 229 + 163);
        auto saved = globals();
        SavedBytes data_saved(h.base, {{kRacerData, 4}});
        StartStandIns stand_ins;
        RecordedStandIn recorded_stand_in;
        if (!build(r, seed + 5000, v < 0, &count)) {
          skip = true;
          continue;
        }
        if (v < 0) {
          continue;
        }
        loaded();
        const uint32_t manager = guest::LoadU32(h.base, rw::kManager);
        const uint32_t vehicles = guest::LoadU32(h.base, kVehicles);
        const uint32_t entries = guest::LoadU32(h.base, manager + 2368);
        uint32_t kind = r.Below(10);
        if (kind == 5 && (entries < 2 || guest::LoadU32(h.base, manager + 4 + 20) == 0xFFFFFFFFu ||
                          guest::LoadU32(h.base, manager + 32 + 20) == 0xFFFFFFFFu)) {
          kind = 1;
        }
        guest::StoreU32(h.base, race.address + 0xA3AF4, kind);
        WriteRandom(h.base, tick_world.address, tick_world.bytes, r);
        // +9608, the flag 0x20, the byte and the word in turn by the seed.
        guest::StoreU32(h.base, tick_world.address + 9608, seed % 5 == 0 ? 0 : 1 + r.Below(3));
        const uint32_t at = race.address + 0x59AE4;
        guest::StoreU32(h.base, at, (guest::LoadU32(h.base, at) & ~0x6020u) |
                                        (seed % 7 != 0 ? 0x20 : 0));
        const uint8_t bytes[] = {0, 1, 2, 255, 0, 1};
        guest::StoreU8(h.base, kStartPosition, bytes[seed % 6]);
        guest::StoreU32(h.base, kStartObject + 4,
                        (seed / 6) % 2 == 0 ? 0 : uint32_t(r.Next()) | 1);
        WriteRandom(h.base, racer_data.address, racer_data.bytes, r);
        guest::StoreU32(h.base, kRacerData, racer_data.address);
        // Traffic now and then: +2380 1 to 3 and +2388 90, only with a world
        // object free after the entries without a vehicle get theirs (a new
        // car needs one: StreamTraffic's group's precondition).
        uint32_t free_objects = 0;
        {
          const uint32_t w = guest::LoadU32(h.base, kWorldPointer);
          for (uint32_t o = guest::LoadU32(h.base, w + 24); o != 0 && free_objects < 8;
               o = guest::LoadU32(h.base, o + 228)) {
            free_objects++;
          }
        }
        uint32_t without = 0;
        for (uint32_t i = 0; i < entries && i < 80; i++) {
          without += guest::LoadU32(h.base, manager + 4 + 28 * i + 20) == 0xFFFFFFFFu ? 1 : 0;
        }
        if (free_objects > without && r.Below(2) == 0) {
          guest::StoreU32(h.base, manager + 2380, 1 + r.Below(3));
          guest::StoreU32(h.base, manager + 2388, 90);
          for (uint32_t k = 0; k < 16; k++) {
            guest::StoreU32(h.base, manager + 2244 + 4 * k, r.Below(count));
          }
        }
        std::vector<std::pair<uint32_t, uint32_t>> bodies;
        for (uint32_t k = 0; k < 7; k++) {
          bodies.push_back({k, guest::LoadU32(h.base, vehicles + 6272 * k + 336)});
        }
        StartStandIns::Reset();
        WriteRandom(h.base, sp - 4096, 4096, r);
        const std::vector<uint8_t> before = Read(h.base, racer_data.address, racer_data.bytes);
        PPCContext t = CallContext(h);
        t.fpscr.disableFlushModeUnconditional();
        t.r3.u64 = tick_world.address;
        (v == 0 ? Original<__imp__sub_82174BC0> : hooks::RaceWorldTick)(t, h.base);
        for (const auto& [k, old] : bodies) {
          const uint32_t now = guest::LoadU32(h.base, vehicles + 6272 * k + 336);
          if (now != old && old != 0 && memory::IsHeapBlock(old)) {
            memory::Free(old);
          }
        }
        if (v == 1) {
          set += guest::LoadU32(h.base, tick_world.address + 9608) != 0 ? 1 : 0;
          cleared += Read(h.base, racer_data.address, racer_data.bytes) != before ? 1 : 0;
          recorded += std::count(g_start_log.begin(), g_start_log.end(), 0xD3000000u);
          streamed += kind <= 8 ? 1 : 0;
        }
        outputs(out[v], count, frame, 0);
        out[v].Bytes(Read(h.base, racer_data.address, racer_data.bytes));
        out[v].Bytes(Read(h.base, tick_world.address, tick_world.bytes));
        teardown();
      }
      if (!skip) {
        results.Check(out[0], out[1], fmt::format("seed {}", seed));
      }
    }
    results.Report();
    ITC_LOG_INFO("harness: race_world: RaceWorld::Tick: {} cases with +9608 set, {} cleared "
                 "[0x826D82E8]'s words, {} called sub_821743A8's stand-in, {} streamed",
                 set, cleared, recorded, streamed);
  }
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
  Placements(h);
  Create(h);
  Tables(h, manager, vehicles);
  VehicleLoops(h, manager);
  TrackCollisionTicks(h, manager, vehicles);
  Inits(h);
  ReleasesForStage(h, manager, vehicles);
  AddDistances(h);
  ManagerLists(h, manager, vehicles);
  if (hooks::NativeHeapActive()) {
    ResetWorlds(h, manager, vehicles);
    ResetVehicleCases(h, manager);
    Unloads(h);
    StartCases(h);
  } else {
    ITC_LOG_INFO("harness: race_world: the resets, Unload and the start skipped (--native_heap "
                 "off)");
  }
}

}  // namespace harness
