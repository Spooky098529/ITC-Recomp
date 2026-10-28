// The race's states and the race's progress (src/race_states/,
// docs/functions/race-states.md) against the recompiled functions. The
// originals run with --native_race_states off (their callees' own flags as
// they are: those were checked by their harnesses), the plain C++ through
// the register steps (hooks/race_states.h). The inputs go in the scratch
// area, the stack below it and blocks from the allocation layer: a
// progress (its pointer at 0x826D82E8), a manager (0x826D82F4), the seven
// vehicles (0x826C2E34), the course's route (test_route.h, at 0x826C5B10)
// and the race data (0x826C5B2C) where a function reads them, with the
// game's globals put back after each side. Compared with harness::Outputs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/test_route.h"
#include "hooks/race_states.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_states/race_states.h"
#include "race_world/race_world.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <vector>

ITC_FLAG_DECLARE(bool, native_race_states);

namespace harness {

namespace {

namespace rs = race_states;
namespace rw = race_world;
namespace tr = test_route;

constexpr uint32_t kVehicles = 0x826C2E34;
constexpr uint32_t kProgressBytes = 7248;
constexpr uint32_t kVehiclesBytes = 7 * 6272;
constexpr uint32_t kManagerBytes = 0x60000;

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

// The original with --native_race_states off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_race_states) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_race_states) = true;
}

// The game's globals a case changes, put back when it ends.
class Globals {
 public:
  Globals(uint8_t* base, std::vector<uint32_t> addresses) : base_(base), addresses_(addresses) {
    for (uint32_t a : addresses_) {
      saved_.push_back(guest::LoadU32(base_, a));
    }
  }
  ~Globals() {
    for (size_t i = 0; i < addresses_.size(); i++) {
      guest::StoreU32(base_, addresses_[i], saved_[i]);
    }
  }

 private:
  uint8_t* base_;
  std::vector<uint32_t> addresses_;
  std::vector<uint32_t> saved_;
};

// A float word: now and then NaN, an infinity or a zero.
uint32_t RandomFloat(Random& r, float range) {
  return tr::RandomFloat(r, range);
}

// --- The race's progress ---

void Progress(Context& h, const Block& progress, const Block& vehicles, const Block& manager) {
  {
    Results results("RaceProgress::Create (sub_821698B0)");
    for (uint64_t seed = 0; seed < 4; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Globals globals(h.base, {rs::kProgress});
        PPCContext t = CallContext(h);
        (v == 0 ? Original<__imp__sub_821698B0> : hooks::RaceStatesProgressCreate)(t, h.base);
        const uint32_t block = guest::LoadU32(h.base, rs::kProgress);
        out[v].Word(block == t.r3.u32 ? 1 : 0);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(block != 0 ? 1 : 0);
        if (block != 0) {
          for (uint32_t k = 0; k < 4; k++) {
            out[v].Bytes(Read(h.base, block + 2188 + 792 + 972 * k, 8));
          }
          memory::crt::OperatorDelete(block);
        }
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceProgress::Reset (sub_821648E0)");
    for (uint64_t seed = 0; seed < 12; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 41 + 5);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 1024, 1024, r);
        t.r3.u64 = progress.address;
        (v == 0 ? Original<__imp__sub_821648E0> : hooks::RaceStatesProgressReset)(t, h.base);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceProgress::SetChecks (sub_821649B8)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 43 + 9);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = progress.address;
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(8);
        t.r5.u64 = r.Below(8) == 0 ? r.Next() : r.Below(6);
        t.r6.u64 = r.Below(2) == 0 ? 0 : r.Next();
        (v == 0 ? Original<__imp__sub_821649B8> : hooks::RaceStatesSetChecks)(t, h.base);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The slots' entries in the scratch area (now and then none); the
    // manager with only the player's entry, flagged 0x4 and 0x40 with a
    // vehicle (its slot word 1) on a random route, no traffic in use, so
    // RaceManager::Refresh keeps it listed.
    Results results("RaceProgress::ReleaseRacers (sub_82163440)");
    const Block area(tr::kRouteBytes + 0x100);
    const Block table(4 * tr::kTableEntries);
    const Block race(0x60000);
    if (!area.address || !table.address || !race.address) {
      ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
      return;
    }
    tr::FillTable(h.base, table.address, area.address);
    for (uint64_t seed = 0; seed < 24; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 47 + 13);
        Globals globals(h.base,
                        {rw::kManager, kVehicles, track_route::kRoute, rw::kRaceData});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, area.address, area.bytes, r);
        const tr::Route route = tr::BuildRoute(h.base, area.address, table.address, r);
        guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
        guest::StoreU32(h.base, race.address + 0x59AE4, uint32_t(r.Next()));
        guest::StoreU32(h.base, rw::kRaceData, race.address);
        WriteRandom(h.base, manager.address, 2400, r);
        WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
        guest::StoreU32(h.base, manager.address + 2368, 1);
        guest::StoreU32(h.base, manager.address + 4, 0x44);
        tr::RandomPosition(h.base, manager.address + 12, route, r);
        guest::StoreU32(h.base, manager.address + 24, 0);
        guest::StoreU32(h.base, manager.address + 28, 0);
        for (uint32_t k = 0; k < 7; k++) {
          guest::StoreU32(h.base, manager.address + 2336 + 4 * k, k == 0 ? 1 : 0);
        }
        for (uint32_t k = 0; k < 3; k++) {
          guest::StoreU32(h.base, manager.address + 2160 + 28 * k, 0);
        }
        tr::RandomPosition(h.base, vehicles.address + 5236, route, r);
        guest::StoreU32(h.base, rw::kManager, manager.address);
        guest::StoreU32(h.base, kVehicles, vehicles.address);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        const uint32_t flags[] = {0x1000, 0x3410, 0x7FFF, 0x20, 0, 0x1400};
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t slot = progress.address + 48 + 536 * i;
          const uint32_t entry = h.data + 16 * i;
          guest::StoreU32(h.base, slot, r.Below(4) == 0 ? 0 : entry);
          guest::StoreU32(h.base, entry,
                          r.Below(3) == 0 ? uint32_t(r.Next()) : flags[r.Below(6)]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = progress.address;
        (v == 0 ? Original<__imp__sub_82163440> : hooks::RaceStatesReleaseRacers)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 64));
        out[v].Bytes(Read(h.base, manager.address, 2400));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceProgress::CountTime (sub_82162E08)");
    for (uint64_t seed = 0; seed < 32; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 53 + 17);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        const uint32_t index = r.Below(4);
        const uint32_t ticks[] = {0, 29, 1799, 107999, 0x7FFFFFFF, 0xFFFFFFFF};
        guest::StoreU32(h.base, progress.address + 2200 + 972 * index,
                        r.Below(2) == 0 ? ticks[r.Below(6)] : r.Below(200000));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 4 * r.Below(64);
        t.r4.u64 = progress.address;
        t.r5.u64 = index;
        (v == 0 ? Original<__imp__sub_82162E08> : hooks::RaceStatesCountTime)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 512));
        out[v].Bytes(Read(h.base, progress.address + 2188, 4 * 972));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The slot's entry in the scratch area with a vehicle (0 to 6); the
    // vehicle's +6228 and speed (+48) random floats (zeros, NaNs and
    // infinities among them); the time a packed time (either sign).
    Results results("RaceProgress::TickFraction (sub_82163140)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 59 + 23);
        Globals globals(h.base, {kVehicles});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 256, 256, r);
        const uint32_t index = r.Below(4);
        const uint32_t entry = h.data + 64;
        guest::StoreU32(h.base, progress.address + 48 + 536 * index, entry);
        const uint32_t vehicle = r.Below(7);
        guest::StoreU32(h.base, entry + 20, vehicle);
        guest::StoreU32(h.base, kVehicles, vehicles.address);
        const uint32_t car = vehicles.address + 6272 * vehicle;
        guest::StoreU32(h.base, car + 6228, RandomFloat(r, 2.0f));
        guest::StoreU32(h.base, car + 48, r.Below(8) == 0 ? 0 : RandomFloat(r, 90.0f));
        const uint32_t time = h.data + 128;
        guest::StoreU32(h.base, time, r.Below(4) == 0 ? uint32_t(r.Next())
                                                      : (uint32_t(r.Below(2)) << 31) |
                                                            (r.Below(30) << 16) |
                                                            (r.Below(60) << 10) | r.Below(1000));
        t.r3.u64 = progress.address;
        t.r4.u64 = index;
        t.r5.u64 = time;
        (v == 0 ? Original<__imp__sub_82163140> : hooks::RaceStatesTickFraction)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, time));
        out[v].Word(guest::LoadU32(h.base, t.r1.u32 - 112 + 80));  // fctiwz's word
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The entries' +824 keys (some equal, NaNs and infinities among them),
    // the count 0 to 4 (now and then below 0); the list in the frame.
    Results results("RaceProgress::Rank (sub_82162D80)");
    const uint32_t keys[] = {0x00000000, 0x80000000, 0x3F800000, 0x42C80000,
                             0x7F800000, 0xFF800000, 0x7FC00000, 0x44FA0000};
    for (uint64_t seed = 0; seed < 32; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 61 + 29);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 1728;
        WriteRandom(h.base, frame, 1728, r);
        guest::StoreU32(h.base, progress.address + 2184,
                        seed % 16 == 0 ? uint32_t(-int32_t(r.Below(2))) : r.Below(5));
        for (uint32_t i = 0; i < 4; i++) {
          guest::StoreU32(h.base, progress.address + 2188 + 972 * i + 824,
                          r.Below(2) == 0 ? keys[r.Below(8)] : RandomFloat(r, 5000.0f));
        }
        t.r3.u64 = progress.address;
        (v == 0 ? Original<__imp__sub_82162D80> : hooks::RaceStatesRank)(t, h.base);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Bytes(Read(h.base, frame + 80, 1616));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The route's sections and positions, over a random route ---

void Sections(Context& h) {
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  if (!area.address || !table.address) {
    ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  // Two sections: now and then the same, or the second one of the first's
  // links (its forward links 0 to 2 or the ones beside it, 6 to 9), or of
  // a link beside it.
  const auto pick = [&](Random& r, const tr::Route& route, uint32_t& a, uint32_t& b) {
    a = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(route.sections);
    const uint32_t record = route.Address(a & 0xFFFF);
    switch (r.Below(5)) {
      case 0:
        b = a;
        break;
      case 1:
        b = uint32_t(int32_t(int16_t(guest::LoadU16(h.base, record + 28 + 4 * r.Below(10)))));
        break;
      case 2: {
        const int16_t side = int16_t(guest::LoadU16(h.base, record + 52 + 4 * r.Below(4)));
        const uint32_t other = route.Address(uint32_t(side) & 0xFFFF);
        b = uint32_t(int32_t(int16_t(guest::LoadU16(h.base, other + 28 + 4 * r.Below(3)))));
        break;
      }
      default:
        b = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(route.sections);
        break;
    }
  };
  {
    Results results("RouteSections::Beside (sub_82162F78)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 67 + 31);
        Globals globals(h.base, {track_route::kRoute});
        WriteRandom(h.base, area.address, area.bytes, r);
        const tr::Route route = tr::BuildRoute(h.base, area.address, table.address, r);
        guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 64, 64, r);
        uint32_t a = 0, b = 0;
        pick(r, route, a, b);
        t.r3.u64 = a | (r.Below(4) == 0 ? r.Next() << 32 : 0);
        t.r4.u64 = b;
        (v == 0 ? Original<__imp__sub_82162F78> : hooks::RaceStatesSectionsBeside)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, t.r1.u32 - 64, 64));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RouteSections::Before (sub_82162E70)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 71 + 37);
        Globals globals(h.base, {track_route::kRoute});
        WriteRandom(h.base, area.address, area.bytes, r);
        const tr::Route route = tr::BuildRoute(h.base, area.address, table.address, r);
        guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 64, 64, r);
        uint32_t a = 0, b = 0;
        pick(r, route, a, b);
        t.r3.u64 = a | (r.Below(4) == 0 ? r.Next() << 32 : 0);
        t.r4.u64 = b;
        (v == 0 ? Original<__imp__sub_82162E70> : hooks::RaceStatesSectionsBefore)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, t.r1.u32 - 64, 64));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RoutePosition::NotAfter (sub_821622A8)");
    for (uint64_t seed = 0; seed < 100; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 73 + 41);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        const uint32_t a = h.data, b = h.data + 8;
        const uint16_t values[] = {0, 1, 2, 0x7FFF, 0x8000, 0xFFFF};
        for (uint32_t at : {a, a + 2, b, b + 2}) {
          guest::StoreU16(h.base, at, values[r.Below(6)]);
        }
        if (r.Below(2) == 0) {
          guest::StoreU16(h.base, b, guest::LoadU16(h.base, a));
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = a;
        t.r4.u64 = b;
        (v == 0 ? Original<__imp__sub_821622A8> : hooks::RaceStatesPositionNotAfter)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

}  // namespace

void RaceStates(Context& h) {
  const Block progress(kProgressBytes);
  const Block vehicles(kVehiclesBytes);
  const Block manager(kManagerBytes);
  if (!progress.address || !vehicles.address || !manager.address) {
    ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
    return;
  }
  Progress(h, progress, vehicles, manager);
  Sections(h);
}

}  // namespace harness
