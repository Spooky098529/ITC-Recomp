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
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_world);

namespace harness {

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
}

}  // namespace harness
