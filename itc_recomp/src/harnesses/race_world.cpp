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
#include "memory/heap.h"
#include "race_world/race_world.h"
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
}

}  // namespace harness
