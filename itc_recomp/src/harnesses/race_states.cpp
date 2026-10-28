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
#include "hooks/heap.h"
#include "hooks/race_states.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_states/race_states.h"
#include "race_world/race_world.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
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

// --- The race's checks ---

// Time::Sub has no adapter (its only caller, CheckLap, is native): its
// register step here.
void NativeTimeSub(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = rs::TimeSub(base, ctx.r3.u32, ctx.r4.u32);
}

uint32_t RandomTime(Random& r) {
  return r.Below(4) == 0 ? uint32_t(r.Next())
                         : (uint32_t(r.Below(2)) << 31) | (r.Below(30) << 16) |
                               (r.Below(60) << 10) | r.Below(1000);
}

// The slots' manager entries in the scratch area (+0 the flags, +20 the
// vehicle 0 to 6), the vehicles' flags +6148 among 0x1, 0x2, 0x80, 0x81,
// 0x40 and random.
void SetUpSlots(Context& h, const Block& progress, const Block& vehicles, Random& r) {
  guest::StoreU32(h.base, kVehicles, vehicles.address);
  const uint32_t car_flags[] = {0x1, 0x2, 0x80, 0x81, 0x40, 0x0, 0x3};
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t entry = h.data + 32 * i;
    guest::StoreU32(h.base, progress.address + 48 + 536 * i, entry);
    const uint32_t flags[] = {0x4, 0x8, 0x0, 0x41, 0x4004};
    guest::StoreU32(h.base, entry, r.Below(4) == 0 ? uint32_t(r.Next()) : flags[r.Below(5)]);
    guest::StoreU32(h.base, entry + 20, r.Below(7));
  }
  for (uint32_t k = 0; k < 7; k++) {
    const uint32_t car = vehicles.address + 6272 * k;
    guest::StoreU32(h.base, car + 6148,
                    r.Below(6) == 0 ? uint32_t(r.Next()) : car_flags[r.Below(7)]);
  }
}

void Checks(Context& h, const Block& progress, const Block& vehicles) {
  {
    Results results("Time::Sub (sub_82162230)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 79 + 43);
        WriteRandom(h.base, h.data, kScratchBytes, r);
        guest::StoreU32(h.base, h.data, RandomTime(r));
        guest::StoreU32(h.base, h.data + 4, RandomTime(r));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        t.r4.u64 = h.data + 4;
        (v == 0 ? __imp__sub_82162230 : NativeTimeSub)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The vehicle's collision flags +6152 and strength +6160 (around 230,
    // NaNs among them), the slot's holds (+44, +40) 0 to 2, the checks
    // +7224 with 0x200, 0x400 or neither, the lap (+6096 set or not, E
    // +832 0 to 3) and its counts near 999.
    Results results("RaceProgress::CountHits (sub_82164F08)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 83 + 47);
        Globals globals(h.base, {kVehicles});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        SetUpSlots(h, progress, vehicles, r);
        const uint32_t index = r.Below(4);
        const uint32_t slot = progress.address + 536 * index;
        const uint32_t e = progress.address + 972 * index;
        const uint32_t car = vehicles.address +
                             6272 * guest::LoadU32(h.base, h.data + 32 * index + 20);
        guest::StoreU32(h.base, car + 6152, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(4));
        const uint32_t strengths[] = {0x43660000, 0x43670000, 0x43650000, 0x7FC00000, 0};
        guest::StoreU32(h.base, car + 6160,
                        r.Below(2) == 0 ? strengths[r.Below(5)] : RandomFloat(r, 500.0f));
        guest::StoreU32(h.base, slot + 44, r.Below(3));
        guest::StoreU32(h.base, slot + 40, r.Below(3));
        const uint32_t checks[] = {0x200, 0x400, 0x600, 0x0, 0x417};
        guest::StoreU32(h.base, progress.address + 7224, checks[r.Below(5)]);
        guest::StoreU32(h.base, progress.address + 6096, r.Below(2));
        const uint32_t lap = r.Below(4);
        guest::StoreU32(h.base, e + 3020, lap);
        guest::StoreU32(h.base, e + 76 * lap + 2276, r.Below(2) ? 998 + r.Below(3) : r.Below(5));
        guest::StoreU32(h.base, e + 76 * lap + 2280, r.Below(2) ? 998 + r.Below(3) : r.Below(5));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 512;
        t.r4.u64 = progress.address;
        t.r5.u64 = index;
        (v == 0 ? Original<__imp__sub_82164F08> : hooks::RaceStatesCountHits)(t, h.base);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Bytes(Read(h.base, h.data + 512, 8));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The vehicle's flags (human or not), its SP (E +784) against a quarter
    // of its start (+788), its collision object (+340, in the scratch area)
    // with a contact (+240) or none: the contact's object word (+0) with its
    // flags' top byte (+4) 0 or not, its strength (+64); the progress's
    // factors +7232 and +7236.
    Results results("RaceProgress::Damage (sub_82164228)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 89 + 53);
        Globals globals(h.base, {kVehicles});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        SetUpSlots(h, progress, vehicles, r);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 160;
        WriteRandom(h.base, frame, 160, r);
        const uint32_t index = r.Below(4);
        const uint32_t e = progress.address + 972 * index;
        const uint32_t car = vehicles.address +
                             6272 * guest::LoadU32(h.base, h.data + 32 * index + 20);
        const uint32_t sps[] = {12000, 3001, 3000, 2999, 100, 0, 0xFFFFFF00u};
        guest::StoreU32(h.base, e + 2972, sps[r.Below(7)]);
        guest::StoreU32(h.base, e + 2976, r.Below(2) ? 12000 : sps[r.Below(7)]);
        const uint32_t object = h.data + 1024;
        const uint32_t contact = h.data + 1536;
        guest::StoreU32(h.base, car + 340, object);
        guest::StoreU32(h.base, object + 240, r.Below(4) == 0 ? 0 : contact);
        guest::StoreU32(h.base, contact + 0, r.Below(2) ? 0 : uint32_t(r.Next()) | 1);
        guest::StoreU32(h.base, contact + 4, r.Below(2) ? r.Below(0x1000000) : uint32_t(r.Next()));
        guest::StoreU32(h.base, contact + 64, RandomFloat(r, 3000.0f));
        guest::StoreU32(h.base, progress.address + 7232, RandomFloat(r, 4.0f));
        guest::StoreU32(h.base, progress.address + 7236, RandomFloat(r, 4.0f));
        t.r3.u64 = h.data + 512;
        t.r4.u64 = progress.address;
        t.r5.u64 = index;
        (v == 0 ? Original<__imp__sub_82164228> : hooks::RaceStatesDamage)(t, h.base);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Bytes(Read(h.base, h.data + 512, 8));
        out[v].Bytes(Read(h.base, frame + 80, 16));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // 0 to 4 entries (now and then below 0): their distances (+824, some
    // equal, NaNs among them), SP (+784, some at 0 or below), slots' places
    // (+568) 0 to 5, the slots' entries' flags 0x4 and 0x8, the vehicles'
    // flags as SetUpSlots.
    Results results("RaceProgress::DrainSp (sub_821650A0)");
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 97 + 59);
        Globals globals(h.base, {kVehicles});
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        SetUpSlots(h, progress, vehicles, r);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 224;
        WriteRandom(h.base, frame, 224, r);
        guest::StoreU32(h.base, progress.address + 2184,
                        seed % 16 == 0 ? uint32_t(-int32_t(r.Below(2))) : r.Below(5));
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t e = progress.address + 2188 + 972 * i;
          guest::StoreU32(h.base, e + 824,
                          r.Below(8) == 0 ? RandomFloat(r, 500.0f)
                                          : std::bit_cast<uint32_t>(float(r.Below(400))));
          const uint32_t sps[] = {12000, 1, 0, 0xFFFFFFF0u, 500};
          guest::StoreU32(h.base, e + 784, r.Below(2) ? r.Below(14000) : sps[r.Below(5)]);
          guest::StoreU32(h.base, progress.address + 40 + 536 * i + 568, r.Below(6));
        }
        t.r3.u64 = h.data + 512;
        t.r4.u64 = progress.address;
        (v == 0 ? Original<__imp__sub_821650A0> : hooks::RaceStatesDrainSp)(t, h.base);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Bytes(Read(h.base, h.data, 128));
        out[v].Bytes(Read(h.base, h.data + 512, 8));
        out[v].Bytes(Read(h.base, frame + 72, 12));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// A position on the route: one of its sections and a slice within it (the
// walks of RoutePosition::MapToSection read the slices' centers from the
// section's end to the slice: a slice past it would walk 65,536 of them).
void ValidPosition(uint8_t* base, uint32_t address, const tr::Route& route, Random& r) {
  const uint32_t section = r.Below(route.sections);
  guest::StoreU16(base, address + 0, uint16_t(section));
  guest::StoreU16(base, address + 2, uint16_t(r.Below(route.Slices(section))));
}

// The route checks over a random route: the vehicle's track position (its
// identity +4) a random position; CheckLap's course route (+6076, 0 to 22,
// whose sections the route's table maps to the test sections), its lap
// (+6096 set or not, E +832 0 to 2) with 0 to 5 laps done and their times,
// the laps' distances (+7160) and E +824 below or above them; CheckFinish's
// finish (+6088) a random position, +7156 against E +824.
void RouteChecks(Context& h, const Block& progress, const Block& vehicles) {
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  if (!area.address || !table.address) {
    ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  const auto set_up = [&](Random& r, tr::Route& route) {
    WriteRandom(h.base, area.address, area.bytes, r);
    route = tr::BuildRoute(h.base, area.address, table.address, r);
    guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, progress.address, kProgressBytes, r);
    SetUpSlots(h, progress, vehicles, r);
  };
  {
    // The position on the section itself, on another of the same kind and
    // slice count, sharing a back link, or elsewhere (a walk).
    Results results("RoutePosition::MapToSection (sub_82163930)");
    for (uint64_t seed = 0; seed < 160; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 101 + 61);
        Globals globals(h.base, {track_route::kRoute, kVehicles});
        tr::Route route;
        set_up(r, route);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 128, 128, r);
        const uint32_t position = h.data + 2048;
        ValidPosition(h.base, position, route, r);
        uint32_t section = r.Below(route.sections);
        if (r.Below(6) == 0) {
          section = guest::LoadU16(h.base, position);
        } else if (r.Below(4) == 0) {
          section = uint32_t(r.Next());
        }
        if (r.Below(3) == 0) {
          // A back link shared.
          const uint32_t a = route.Address(section & 0xFFFF);
          const uint32_t p = route.Address(guest::LoadU16(h.base, position));
          guest::StoreU16(h.base, p + 28 + 4 * (3 + r.Below(3)),
                          guest::LoadU16(h.base, a + 28 + 4 * (3 + r.Below(3))));
        }
        t.r3.u64 = section;
        t.r4.u64 = position;
        (v == 0 ? Original<__imp__sub_82163930> : hooks::RaceStatesMapToSection)(t, h.base);
        out[v].Word(guest::LoadU32(h.base, position));
        out[v].Bytes(Read(h.base, t.r1.u32 - 96, 64));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceProgress::CheckLap (sub_82164090)");
    uint32_t laps = 0;
    for (uint64_t seed = 0; seed < 48; seed++) {
      Outputs out[2];
      // A first run of the original (v -1, not compared) gives the next
      // lap's position in the course route's copy; two cases in three then
      // put the car on it (its slice 0 to 2 past it) and far enough along.
      uint32_t lap_position = 0xFFFFFFFFu;
      for (int v = -1; v < 2; v++) {
        Random r(seed * 103 + 67);
        Globals globals(h.base, {track_route::kRoute, kVehicles});
        tr::Route route;
        set_up(r, route);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 544;
        WriteRandom(h.base, frame - 32, 576, r);
        const uint32_t index = r.Below(4);
        const uint32_t e = progress.address + 972 * index;
        const uint32_t car = vehicles.address +
                             6272 * guest::LoadU32(h.base, h.data + 32 * index + 20);
        ValidPosition(h.base, car + 5236, route, r);
        guest::StoreU32(h.base, progress.address + 6076, r.Below(23));
        const uint32_t by_lap = r.Below(2);
        guest::StoreU32(h.base, progress.address + 6096, by_lap);
        const uint32_t lap = by_lap ? r.Below(3) : 0;  // the record the function reads
        guest::StoreU32(h.base, e + 3020, lap);
        const uint32_t record = e + 76 * lap + 2208;
        const uint32_t done = r.Below(8) == 0 ? 4 + r.Below(3) : r.Below(4);
        guest::StoreU32(h.base, record + 60, done);
        for (uint32_t k = 0; k < 8; k++) {
          guest::StoreU32(h.base, record + 4 * k, RandomTime(r));
        }
        guest::StoreU32(h.base, e + 2204, RandomTime(r));
        for (uint32_t k = 0; k < 15; k++) {
          guest::StoreU32(h.base, progress.address + 7160 + 4 * k,
                          std::bit_cast<uint32_t>(float(r.Below(4000))));
        }
        guest::StoreU32(h.base, e + 3012, r.Below(8) == 0 ? RandomFloat(r, 5000.0f)
                                                          : std::bit_cast<uint32_t>(
                                                                float(r.Below(4000))));
        guest::StoreU32(h.base, car + 48, RandomFloat(r, 90.0f));
        guest::StoreU32(h.base, car + 6228, RandomFloat(r, 2.0f));
        const uint32_t past = r.Below(3);
        if (v >= 0 && seed % 3 != 0 && lap_position != 0xFFFFFFFFu) {
          guest::StoreU16(h.base, car + 5236, uint16_t(lap_position >> 16));
          guest::StoreU16(h.base, car + 5238, uint16_t(lap_position + past));
          guest::StoreU32(h.base, e + 3012, std::bit_cast<uint32_t>(4000.0f));
        }
        t.r3.u64 = h.data + 512;
        t.r4.u64 = progress.address;
        t.r5.u64 = index;
        (v <= 0 ? Original<__imp__sub_82164090> : hooks::RaceStatesCheckLap)(t, h.base);
        if (v < 0) {
          const uint32_t next = done + 1;
          lap_position = next < 5 ? guest::LoadU32(h.base, frame + 96 + 4 * next) : 0xFFFFFFFFu;
          continue;
        }
        laps += v == 1 && guest::LoadU32(h.base, record + 60) != done ? 1 : 0;
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Bytes(Read(h.base, h.data + 512, 8));
        out[v].Bytes(Read(h.base, frame + 80, 8));
        out[v].Bytes(Read(h.base, frame + 96, 184));
        out[v].Bytes(Read(h.base, frame + 288, 184));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_states: CheckLap recorded a lap in {} of 48 cases", laps);
  }
  {
    Results results("RaceProgress::CheckFinish (sub_82164E18)");
    uint32_t finished = 0;
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 107 + 71);
        Globals globals(h.base, {track_route::kRoute, kVehicles});
        tr::Route route;
        set_up(r, route);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 144;
        WriteRandom(h.base, frame - 32, 176, r);
        const uint32_t index = r.Below(4);
        const uint32_t e = progress.address + 972 * index;
        const uint32_t car = vehicles.address +
                             6272 * guest::LoadU32(h.base, h.data + 32 * index + 20);
        ValidPosition(h.base, car + 5236, route, r);
        if (r.Below(2) == 0) {
          guest::StoreU32(h.base, progress.address + 6088, guest::LoadU32(h.base, car + 5236));
        } else {
          ValidPosition(h.base, progress.address + 6088, route, r);
        }
        guest::StoreU32(h.base, progress.address + 7156,
                        std::bit_cast<uint32_t>(float(r.Below(4000))));
        guest::StoreU32(h.base, e + 3012, r.Below(8) == 0 ? RandomFloat(r, 5000.0f)
                                                          : std::bit_cast<uint32_t>(
                                                                float(r.Below(4000))));
        guest::StoreU32(h.base, e + 2204, RandomTime(r));
        guest::StoreU32(h.base, car + 48, RandomFloat(r, 90.0f));
        guest::StoreU32(h.base, car + 6228, RandomFloat(r, 2.0f));
        t.r3.u64 = h.data + 512;
        t.r4.u64 = progress.address;
        t.r5.u64 = index;
        (v == 0 ? Original<__imp__sub_82164E18> : hooks::RaceStatesCheckFinish)(t, h.base);
        finished += v == 1 && guest::LoadU32(h.base, h.data + 512) == 1 ? 1 : 0;
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
        out[v].Bytes(Read(h.base, h.data + 512, 8));
        out[v].Bytes(Read(h.base, frame + 80, 4));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_states: CheckFinish finished {} of 64 cases", finished);
  }
}

// --- The race's progress along the route ---

// A whole random route (every section 8 slices, the links' slices within
// them: TrackRoute::PathDistance's slice steps end only where they meet)
// whose links name its own sections or none. FollowRoute's cases: the
// slot's manager entry and the car's input record in the scratch area, the
// vehicle's track position on the route, the slot's list of sections (0 to
// 6, or 255 and 256: full) and the race's (+6120, 0 to 6, or 255 and 256)
// from the car's section, its links and random sections, the start (+7152)
// the car's section or another; the speeds, the distance a frame and the
// distance past the slice random floats with NaNs.
void FollowRouteCases(Context& h, const Block& progress, const Block& vehicles,
                      const Block& manager) {
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  if (!area.address || !table.address) {
    ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  std::vector<uint32_t> words = {track_route::kRoute, kVehicles};
  for (uint32_t k = 0; k < 8; k++) {
    words.push_back(0x826C5B30 + 4 * k);
  }
  const auto set_up = [&](Random& r, tr::Route& route) {
    WriteRandom(h.base, area.address, area.bytes, r);
    route = tr::BuildRoute(h.base, area.address, table.address, r, true);
    for (uint32_t s = 0; s < 8; s++) {
      for (uint32_t i = 0; i < 12; i++) {
        const uint32_t link = route.address[s] + 28 + 4 * i;
        if (guest::LoadU16(h.base, link) != 0xFFFF) {
          guest::StoreU16(h.base, link, uint16_t(r.Below(route.sections)));
        }
      }
    }
    guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, progress.address, kProgressBytes, r);
    WriteRandom(h.base, vehicles.address, vehicles.bytes, r);
    SetUpSlots(h, progress, vehicles, r);
    for (uint32_t k = 0; k < 8; k++) {
      guest::StoreU32(h.base, 0x826C5B30 + 4 * k, h.data + 1024 + 24 * k);
      for (uint32_t i = 0; i < 20; i += 2) {
        guest::StoreU16(h.base, h.data + 1024 + 24 * k + i, r.Below(3) ? 0 : uint16_t(r.Next()));
      }
    }
  };
  // A section near `section`: itself, one of its links or any.
  const auto near = [&](const tr::Route& route, uint32_t section, Random& r) {
    const uint32_t pick = r.Below(4);
    if (pick == 0) {
      return section;
    }
    if (pick < 3) {
      const int16_t link =
          int16_t(guest::LoadU16(h.base, route.Address(section) + 28 + 4 * r.Below(12)));
      if (link != -1) {
        return uint32_t(uint16_t(link));
      }
    }
    return r.Below(route.sections);
  };
  {
    Results results("sub_82163000 (two sections joined)");
    for (uint64_t seed = 0; seed < 48; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 131 + 89);
        Globals globals(h.base, words);
        tr::Route route;
        set_up(r, route);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 128, 128, r);
        t.r3.u64 = r.Below(route.sections);
        t.r4.u64 = near(route, near(route, t.r3.u32, r), r);
        t.r5.u64 = h.data + 512;
        t.r6.u64 = h.data + 516;
        (v == 0 ? __imp__sub_82163000 : hooks::RaceStatesSectionsJoin)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + 512, 8));
        out[v].Bytes(Read(h.base, t.r1.u32 - 96, 36));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceProgress::FollowRoute (sub_82163C50)");
    uint32_t off_route = 0, along = 0;
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 137 + 97);
        Globals globals(h.base, words);
        tr::Route route;
        set_up(r, route);
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 240;
        WriteRandom(h.base, frame, 240, r);
        const uint32_t index = r.Below(4);
        const uint32_t slot = progress.address + 40 + 536 * index;
        const uint32_t e = progress.address + 972 * index;
        const uint32_t car = vehicles.address +
                             6272 * guest::LoadU32(h.base, h.data + 32 * index + 20);
        ValidPosition(h.base, car + 5236, route, r);
        const uint32_t section = guest::LoadU16(h.base, car + 5236);
        const uint32_t counts[] = {0, 1, 1, 2, 3, 4, 6, 255, 256};
        const uint32_t n = counts[r.Below(9)];
        guest::StoreU32(h.base, slot + 524, n);
        uint32_t s = r.Below(2) ? section : r.Below(route.sections);
        for (uint32_t k = 0; k < std::min(n, 256u); k++) {
          guest::StoreU16(h.base, slot + 12 + 2 * k, uint16_t(s));
          s = near(route, s, r);
        }
        if (n > 0 && r.Below(2)) {
          guest::StoreU16(h.base, slot + 10 + 2 * std::min(n, 256u),
                          uint16_t(near(route, section, r)));
        }
        const uint32_t race_n = counts[r.Below(9)];
        guest::StoreU32(h.base, progress.address + 6632, race_n);
        for (uint32_t k = 0; k < std::min(race_n, 256u); k++) {
          guest::StoreU16(h.base, progress.address + 6120 + 2 * k,
                          uint16_t(near(route, section, r)));
        }
        guest::StoreU16(h.base, progress.address + 7152,
                        uint16_t(r.Below(2) ? section : r.Below(route.sections)));
        guest::StoreU16(h.base, progress.address + 7154, uint16_t(r.Below(8)));
        guest::StoreU8(h.base, car + 2424, r.Below(2) ? 0 : uint8_t(r.Next()));
        guest::StoreU32(h.base, car + 48, RandomFloat(r, 90.0f));
        guest::StoreU32(h.base, car + 6228, RandomFloat(r, 2.0f));
        guest::StoreU32(h.base, e + 2980, RandomFloat(r, 200.0f));
        guest::StoreU32(h.base, e + 2984, r.Below(4) == 0 ? uint32_t(-1) : r.Below(1000));
        guest::StoreU32(h.base, e + 2988, RandomFloat(r, 300.0f));
        guest::StoreU32(h.base, e + 3004, RandomFloat(r, 50.0f));
        t.r3.u64 = h.data + 600;
        t.r4.u64 = progress.address;
        t.r5.u64 = index;
        (v == 0 ? Original<__imp__sub_82163C50> : hooks::RaceStatesFollowRoute)(t, h.base);
        if (v == 1) {
          off_route += guest::LoadU32(h.base, h.data + 600) == 1 ? 1 : 0;
          along += guest::LoadU32(h.base, h.data + 600) == 0 &&
                           guest::LoadU32(h.base, e + 3012) != 0
                       ? 1
                       : 0;
        }
        // The entry's floats (+792 the mean, +800 the top speed, +816 the
        // kilometers, +824 the distance) as floats, the rest exact.
        uint32_t at = progress.address;
        for (uint32_t f : {2980u, 2988u, 3004u, 3012u}) {
          out[v].Bytes(Read(h.base, at, e + f - at));
          out[v].Floats(h.base, e + f, 1);
          at = e + f + 4;
        }
        out[v].Bytes(Read(h.base, at, progress.address + kProgressBytes - at));
        out[v].Bytes(Read(h.base, h.data + 600, 8));
        out[v].Bytes(Read(h.base, frame + 80, 36));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_states: FollowRoute off the route in {}, along in {} of 64",
                 off_route, along);
  }
  {
    // RaceTick: 0 to 4 racers (now and then below 0), each with a car on the
    // route, its list (1 to 3 sections) from its section, its contact (Damage), its lap
    // record and the laps' distances (CheckLap, CheckFinish); the checks
    // +7220 and +7224 as SetChecks sets them or random (+7220's 0x8, which
    // the original runs, left out); the race's ticks and the ring. No AI
    // drivers (0x826C6074 0) and no racers in the manager: AiDrivers::ClearAll
    // stops at once (its own harness checks it).
    Results results("RaceStates::RaceTick (sub_82167610)");
    const Block machine(10240);
    const Block dispatcher(1024);
    if (!machine.address || !dispatcher.address) {
      ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
      return;
    }
    std::vector<uint32_t> tick_words = words;
    tick_words.push_back(rw::kManager);
    tick_words.push_back(0x826C6074);
    uint32_t ended = 0;
    for (uint64_t seed = 0; seed < 48; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 139 + 101);
        Globals globals(h.base, tick_words);
        tr::Route route;
        set_up(r, route);
        WriteRandom(h.base, manager.address, 2400, r);
        guest::StoreU32(h.base, manager.address + 2364, 0);
        guest::StoreU32(h.base, rw::kManager, manager.address);
        guest::StoreU32(h.base, 0x826C6074, 0);
        WriteRandom(h.base, machine.address + 9600, 64, r);
        guest::StoreU32(h.base, machine.address + 9624, progress.address);
        WriteRandom(h.base, dispatcher.address, 1024, r);
        guest::StoreU32(h.base, dispatcher.address + 804, r.Below(50));
        guest::StoreU32(h.base, dispatcher.address + 808, r.Below(50));
        PPCContext t = CallContext(h);
        const uint32_t frame = t.r1.u32 - 208;
        WriteRandom(h.base, frame, 208, r);
        const uint32_t p = progress.address;
        guest::StoreU32(h.base, p + 2184, seed % 16 == 0 ? uint32_t(-1) : r.Below(5));
        const uint32_t checks_a[] = {0x3, 0x11, 0x13, 0x0};
        const uint32_t checks_b[] = {0x417, 0x481, 0x0, 0x695};
        guest::StoreU32(h.base, p + 7220,
                        r.Below(4) == 0 ? uint32_t(r.Next()) & ~0x8u : checks_a[r.Below(4)]);
        guest::StoreU32(h.base, p + 7224,
                        r.Below(4) == 0 ? uint32_t(r.Next()) : checks_b[r.Below(4)]);
        guest::StoreU32(h.base, p + 6076, r.Below(23));
        guest::StoreU32(h.base, p + 6096, r.Below(2));
        guest::StoreU32(h.base, p + 6116, r.Below(100000));
        guest::StoreU16(h.base, p + 7152, uint16_t(r.Below(route.sections)));
        guest::StoreU16(h.base, p + 7154, uint16_t(r.Below(8)));
        guest::StoreU32(h.base, p + 7156, std::bit_cast<uint32_t>(float(r.Below(4000))));
        guest::StoreU32(h.base, p + 7232, RandomFloat(r, 4.0f));
        guest::StoreU32(h.base, p + 7236, RandomFloat(r, 4.0f));
        guest::StoreU32(h.base, p + 6632, r.Below(4));
        for (uint32_t k = 0; k < 15; k++) {
          guest::StoreU32(h.base, p + 7160 + 4 * k, std::bit_cast<uint32_t>(float(r.Below(4000))));
        }
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t slot = p + 40 + 536 * i;
          const uint32_t e = p + 972 * i;
          const uint32_t car = vehicles.address +
                               6272 * guest::LoadU32(h.base, h.data + 32 * i + 20);
          ValidPosition(h.base, car + 5236, route, r);
          const uint32_t section = guest::LoadU16(h.base, car + 5236);
          // 1 to 3 sections (StartRoute lists the first before the race:
          // with none, FollowRoute may write over the slot's manager entry).
          const uint32_t n = 1 + r.Below(3);
          guest::StoreU32(h.base, slot + 524, n);
          uint32_t sec = r.Below(2) ? section : r.Below(route.sections);
          for (uint32_t k = 0; k < n; k++) {
            guest::StoreU16(h.base, slot + 12 + 2 * k, uint16_t(sec));
            sec = near(route, sec, r);
          }
          guest::StoreU32(h.base, slot + 40, r.Below(3));
          guest::StoreU32(h.base, slot + 44, r.Below(3));
          guest::StoreU32(h.base, slot + 568, r.Below(6));
          guest::StoreU8(h.base, car + 2424, r.Below(2) ? 0 : uint8_t(r.Next()));
          guest::StoreU32(h.base, car + 48, RandomFloat(r, 90.0f));
          guest::StoreU32(h.base, car + 6228, RandomFloat(r, 2.0f));
          guest::StoreU32(h.base, car + 6152, r.Below(4));
          guest::StoreU32(h.base, car + 6160, RandomFloat(r, 400.0f));
          const uint32_t object = h.data + 1280 + 64 * i;
          const uint32_t contact = h.data + 1536 + 128 * i;
          guest::StoreU32(h.base, car + 340, object - 240);
          guest::StoreU32(h.base, object, r.Below(3) == 0 ? 0 : contact);
          guest::StoreU32(h.base, contact + 0, r.Below(2) ? 0 : uint32_t(r.Next()) | 1);
          guest::StoreU32(h.base, contact + 4, r.Below(0x1000000));
          guest::StoreU32(h.base, contact + 64, RandomFloat(r, 3000.0f));
          const uint32_t sps[] = {12000, 3001, 1, 0, 0xFFFFFF00u};
          guest::StoreU32(h.base, e + 2972, r.Below(2) ? r.Below(14000) : sps[r.Below(5)]);
          guest::StoreU32(h.base, e + 2976, 12000);
          guest::StoreU32(h.base, e + 2980, RandomFloat(r, 200.0f));
          guest::StoreU32(h.base, e + 2984, r.Below(1000));
          guest::StoreU32(h.base, e + 2988, RandomFloat(r, 300.0f));
          guest::StoreU32(h.base, e + 3004, RandomFloat(r, 50.0f));
          guest::StoreU32(h.base, e + 3020, r.Below(3));
          for (uint32_t lap = 0; lap < 3; lap++) {
            const uint32_t record = e + 2208 + 76 * lap;
            guest::StoreU32(h.base, record + 60, r.Below(4));
            for (uint32_t k = 0; k < 8; k++) {
              guest::StoreU32(h.base, record + 4 * k, RandomTime(r));
            }
            guest::StoreU32(h.base, record + 68, r.Below(1000));
            guest::StoreU32(h.base, record + 72, r.Below(1000));
          }
          guest::StoreU32(h.base, e + 2204, RandomTime(r));
          guest::StoreU32(h.base, e + 2200, r.Below(100000));
        }
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher.address;
        (v == 0 ? Original<__imp__sub_82167610> : hooks::RaceStatesRaceTick)(t, h.base);
        if (v == 1) {
          ended += guest::LoadU32(h.base, p + 6104) != 0 ? 1 : 0;
        }
        uint32_t at = p;
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t e = p + 972 * i;
          for (uint32_t f : {2980u, 2988u, 3004u, 3012u}) {
            out[v].Bytes(Read(h.base, at, e + f - at));
            out[v].Floats(h.base, e + f, 1);
            at = e + f + 4;
          }
        }
        out[v].Bytes(Read(h.base, at, p + kProgressBytes - at));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Bytes(Read(h.base, machine.address + 9600, 64));
        out[v].Bytes(Read(h.base, dispatcher.address, 1024));
        out[v].Bytes(Read(h.base, frame + 16, 32));
        for (uint32_t d : {48u, 56u}) {
          out[v].Double(uint64_t(guest::LoadU32(h.base, frame + d)) << 32 |
                        guest::LoadU32(h.base, frame + d + 4));
        }
        out[v].Bytes(Read(h.base, frame + 64, 72));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_states: RaceTick ended the race in {} of 48 cases", ended);
  }
}

// --- The state machine's handlers: the machine and the dispatcher in
// blocks of their own, the race data's flags and kind, the screen fades'
// words (0x826A50AC to 0x826A50EB) and [0x826C6B04] put back after each
// side ---

void Handlers(Context& h, const Block& progress, const Block& vehicles, const Block& manager) {
  const Block machine(10240);
  const Block dispatcher(1024);
  const Block race(0xA4000);
  if (!machine.address || !dispatcher.address || !race.address) {
    ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
    return;
  }
  constexpr uint32_t kFades = 0x826A50AC;
  const std::vector<uint32_t> fade_words = [] {
    std::vector<uint32_t> words;
    for (uint32_t a = kFades; a < kFades + 64; a += 4) {
      words.push_back(a);
    }
    words.push_back(0x826C6B04);
    words.push_back(rw::kRaceData);
    words.push_back(rw::kManager);
    words.push_back(kVehicles);
    words.push_back(rs::kProgress);
    words.push_back(track_route::kRoute);
    return words;
  }();
  // One case's common inputs: the machine (its +9612 to +9640 random, +9608
  // 0 or not, +9616 around 90, +9624 the progress), the ring's count and
  // start, the fades done or not (the alpha 0 or not), the race data's
  // flags and kind (5 or not).
  const auto common = [&](Random& r) {
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, machine.address + 9600, 64, r);
    guest::StoreU32(h.base, machine.address + 9608, r.Below(2) ? 0 : uint32_t(r.Next()));
    guest::StoreU32(h.base, machine.address + 9616, r.Below(2) ? 89 + r.Below(4) : r.Below(200));
    guest::StoreU32(h.base, machine.address + 9624, progress.address);
    WriteRandom(h.base, dispatcher.address, 1024, r);
    guest::StoreU32(h.base, dispatcher.address + 804, r.Below(50));
    guest::StoreU32(h.base, dispatcher.address + 808, r.Below(r.Below(4) == 0 ? 1000 : 50));
    const bool done = r.Below(3) != 0;
    guest::StoreU32(h.base, kFades + 48, done ? 0 : r.Below(2));    // 0x826A50DC
    guest::StoreU32(h.base, kFades + 56, done ? 0 : 1);             // 0x826A50E4
    guest::StoreU32(h.base, kFades + 40, r.Below(2) ? 0 : 255);     // the alpha
    guest::StoreU32(h.base, race.address + 0x59AE4, uint32_t(r.Next()));
    guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(3) == 0 ? 5 : r.Below(8));
    guest::StoreU32(h.base, rw::kRaceData, race.address);
    WriteRandom(h.base, progress.address + 6100, 16, r);
  };
  const auto outputs = [&](Outputs& o) {
    o.Bytes(Read(h.base, machine.address + 9600, 64));
    o.Bytes(Read(h.base, dispatcher.address, 1024));
    o.Bytes(Read(h.base, kFades, 64));
    o.Word(guest::LoadU32(h.base, 0x826C6B04));
    o.Word(guest::LoadU32(h.base, race.address + 0x59AE4));
    o.Bytes(Read(h.base, progress.address + 6100, 16));
  };
  struct Simple {
    const char* name;
    GuestFunction original;
    GuestFunction step;
  };
  const Simple simple[] = {
      {"RaceStates::FadeInFree (sub_82163398)", __imp__sub_82163398, hooks::RaceStatesFadeInFree},
      {"RaceStates::FadeOut (sub_82162310)", __imp__sub_82162310, hooks::RaceStatesFadeOut},
      {"RaceStates::EndWhenFaded (sub_82168F50)", __imp__sub_82168F50,
       hooks::RaceStatesEndWhenFaded},
      {"RaceStates::OnRollingStart (sub_821632C8)", __imp__sub_821632C8,
       hooks::RaceStatesOnRollingStart},
      {"RaceStates::ClearStep (sub_82163348)", __imp__sub_82163348, hooks::RaceStatesClearStep},
      {"RaceStates::OnQuit (sub_82163378)", __imp__sub_82163378, hooks::RaceStatesOnQuit},
      {"RaceStates::OnReplay (sub_821647B0)", __imp__sub_821647B0, hooks::RaceStatesOnReplay},
      {"RaceStates::QuitTick (sub_82168500)", __imp__sub_82168500, hooks::RaceStatesQuitTick},
      {"RaceStates::AfterRaceTick (sub_82167970)", __imp__sub_82167970,
       hooks::RaceStatesAfterRaceTick},
  };
  for (const Simple& f : simple) {
    Results results(f.name);
    for (uint64_t seed = 0; seed < 24; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 109 + 73);
        Globals globals(h.base, fade_words);
        common(r);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher.address;
        if (v == 0) {
          ITC_FLAG(native_race_states) = false;
          f.original(t, h.base);
          ITC_FLAG(native_race_states) = true;
        } else {
          f.step(t, h.base);
        }
        outputs(out[v]);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // A manager with no racers: AiDrivers::StartRanked lists none.
    Results results("RaceStates::OnStandingStart (sub_821643B0)");
    for (uint64_t seed = 0; seed < 8; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 113 + 79);
        Globals globals(h.base, fade_words);
        common(r);
        WriteRandom(h.base, manager.address, 2400, r);
        guest::StoreU32(h.base, manager.address + 2364, 0);
        guest::StoreU32(h.base, rw::kManager, manager.address);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        (v == 0 ? Original<__imp__sub_821643B0> : hooks::RaceStatesOnStandingStart)(t, h.base);
        outputs(out[v]);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The machine's words and the whole progress (RaceProgress::Reset).
    Results results("RaceStates::Begin (sub_82164DC0)");
    for (uint64_t seed = 0; seed < 8; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 127 + 83);
        Globals globals(h.base, fade_words);
        common(r);
        WriteRandom(h.base, progress.address, kProgressBytes, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher.address;
        (v == 0 ? Original<__imp__sub_82164DC0> : hooks::RaceStatesBegin)(t, h.base);
        outputs(out[v]);
        out[v].Bytes(Read(h.base, progress.address, kProgressBytes));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // A progress block from operator new, freed by both sides (the second
    // gets the same block again).
    Results results("RaceStates::Unload (sub_82163920)");
    for (uint64_t seed = 0; seed < 4; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Globals globals(h.base, fade_words);
        const auto block = memory::crt::OperatorNew(h.base, kProgressBytes);
        const uint32_t p = block ? *block : 0;
        guest::StoreU32(h.base, rs::kProgress, seed == 0 ? 0 : p);
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        (v == 0 ? Original<__imp__sub_82163920> : hooks::RaceStatesUnload)(t, h.base);
        const bool freed = p == 0 || !memory::IsHeapBlock(p);
        out[v].Word(freed ? 1 : 0);
        if (!freed) {
          memory::crt::OperatorDelete(p);
        }
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    // The racers (the manager's +2364, 0 to 7) on a random route: the
    // vehicles' flags (0x81 or not), their track positions; +9616 at 90 or
    // above.
    Results results("RaceStates::FreeStartTick (sub_82168B80)");
    const Block area(tr::kRouteBytes + 0x100);
    const Block table(4 * tr::kTableEntries);
    if (!area.address || !table.address) {
      return;
    }
    tr::FillTable(h.base, table.address, area.address);
    for (uint64_t seed = 0; seed < 48; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 127 + 83);
        Globals globals(h.base, fade_words);
        common(r);
        WriteRandom(h.base, area.address, area.bytes, r);
        const tr::Route route = tr::BuildRoute(h.base, area.address, table.address, r);
        guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
        WriteRandom(h.base, manager.address, 2400, r);
        guest::StoreU32(h.base, manager.address + 2364, seed % 12 == 0 ? 0 : r.Below(8));
        guest::StoreU32(h.base, rw::kManager, manager.address);
        guest::StoreU32(h.base, kVehicles, vehicles.address);
        guest::StoreU32(h.base, machine.address + 9616, r.Below(4) == 0 ? r.Below(91) : 91 + r.Below(3));
        for (uint32_t k = 0; k < 7; k++) {
          const uint32_t car = vehicles.address + 6272 * k;
          const uint32_t flags[] = {0x1, 0x80, 0x0, 0x40, 0x81};
          guest::StoreU32(h.base, car + 6148, flags[r.Below(5)]);
          tr::RandomPosition(h.base, car + 5236, route, r);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher.address;
        (v == 0 ? Original<__imp__sub_82168B80> : hooks::RaceStatesFreeStartTick)(t, h.base);
        outputs(out[v]);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The racers at a standing start: a race data block (1 to 6 racer
// records, the player's flagged 0x1 at a random place or none; the current
// stage's four slots, some -1), the manager's entries (their positions on
// a random route of 1 to 8 sections, so many share or link sections), its
// racers' list (0 to 6), the vehicles' track positions, collision objects
// and flags; no AI drivers ([0x826C6074] 0, so ClearAll and ResetAll stop
// at once) ---

void Racers(Context& h, const Block& vehicles, const Block& manager) {
  const Block race(0xA4000);
  const Block machine(10240);
  const Block dispatcher(1024);
  const Block area(tr::kRouteBytes + 0x100);
  const Block table(4 * tr::kTableEntries);
  if (!race.address || !machine.address || !dispatcher.address || !area.address ||
      !table.address) {
    ITC_LOG_ERROR("harness: race_states: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, table.address, area.address);
  constexpr uint32_t kDriversTable = 0x826C6074;
  const std::vector<uint32_t> words = {rw::kRaceData, rw::kManager, kVehicles,
                                       track_route::kRoute, kDriversTable};
  const auto set_up = [&](Random& r) {
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, area.address, area.bytes, r);
    const tr::Route route = tr::BuildRoute(h.base, area.address, table.address, r);
    guest::StoreU32(h.base, track_route::kRoute, area.address + tr::kFile);
    const uint32_t records = race.address + 48;
    const uint32_t n = 1 + r.Below(6);
    guest::StoreU32(h.base, records + 0x595E0, n);
    const uint32_t player = r.Below(6) == 0 ? 99 : r.Below(n);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU32(h.base, records + 3888 + 3936 * i,
                      (uint32_t(r.Next()) & ~0x1u) | (i == player ? 1u : 0u));
    }
    const uint32_t stages = race.address + 0x59620;
    guest::StoreU32(h.base, stages + 804, 0);
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(h.base, stages + 44 + 8 * k, r.Below(5) == 0 ? 0xFFFFFFFFu : r.Below(n));
    }
    guest::StoreU32(h.base, rw::kRaceData, race.address);
    WriteRandom(h.base, manager.address, 2400, r);
    guest::StoreU32(h.base, manager.address + 2368, n);
    for (uint32_t i = 0; i < n; i++) {
      const uint32_t entry = manager.address + 4 + 28 * i;
      ValidPosition(h.base, entry + 8, route, r);
      guest::StoreU32(h.base, entry + 20, i % 7);
      guest::StoreU32(h.base, entry + 24, i);
    }
    const uint32_t racers = r.Below(8) == 0 ? 0 : 1 + r.Below(n);
    guest::StoreU32(h.base, manager.address + 2364, racers);
    for (uint32_t j = 0; j < 7; j++) {
      guest::StoreU32(h.base, manager.address + 2308 + 4 * j,
                      manager.address + 4 + 28 * (j < n ? j : 0));
    }
    guest::StoreU32(h.base, rw::kManager, manager.address);
    guest::StoreU32(h.base, kVehicles, vehicles.address);
    guest::StoreU32(h.base, kDriversTable, 0);
    for (uint32_t k = 0; k < 7; k++) {
      const uint32_t car = vehicles.address + 6272 * k;
      ValidPosition(h.base, car + 5236, route, r);
      const uint32_t object = h.data + 1024 + 256 * k;
      guest::StoreU32(h.base, car + 340, object);
      guest::StoreU32(h.base, object + 240, r.Below(5) == 0 ? uint32_t(r.Next()) | 1 : 0);
      guest::StoreU32(h.base, car + 6152, r.Below(5) == 0 ? r.Below(8) : r.Below(4) << 3);
    }
  };
  {
    Results results("RaceStates::RacersNear (sub_82165AB0)");
    uint32_t near = 0;
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 131 + 89);
        Globals globals(h.base, words);
        set_up(r);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 208, 208, r);
        (v == 0 ? Original<__imp__sub_82165AB0> : hooks::RaceStatesRacersNear)(t, h.base);
        near += v == 1 ? t.r3.u32 : 0;
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, t.r1.u32 - 208 + 80, 44));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_states: RacersNear answered 1 in {} of 64 cases", near);
  }
  {
    Results results("RaceStates::RacersClose (sub_82165C78)");
    uint32_t close = 0;
    for (uint64_t seed = 0; seed < 64; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 137 + 97);
        Globals globals(h.base, words);
        set_up(r);
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 208, 208, r);
        (v == 0 ? Original<__imp__sub_82165C78> : hooks::RaceStatesRacersClose)(t, h.base);
        close += v == 1 ? t.r3.u32 : 0;
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Floats(h.base, t.r1.u32 - 208 + 80, 12);
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
    ITC_LOG_INFO("harness: race_states: RacersClose answered 1 in {} of 64 cases", close);
  }
  {
    Results results("RaceStates::RacersStill (sub_82164A90)");
    for (uint64_t seed = 0; seed < 48; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 139 + 101);
        Globals globals(h.base, words);
        set_up(r);
        const uint32_t count = h.data + 512;
        const uint32_t counts[] = {0, 28, 29, 30, 31, 0xFFFFFFFFu};
        guest::StoreU32(h.base, count, counts[r.Below(6)]);
        PPCContext t = CallContext(h);
        t.r3.u64 = count;
        (v == 0 ? Original<__imp__sub_82164A90> : hooks::RaceStatesRacersStill)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(guest::LoadU32(h.base, count));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
  {
    Results results("RaceStates::CountdownTick (sub_821674A0)");
    for (uint64_t seed = 0; seed < 48; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 149 + 103);
        Globals globals(h.base, words);
        set_up(r);
        WriteRandom(h.base, machine.address + 9600, 64, r);
        guest::StoreU32(h.base, machine.address + 9616, r.Below(2) ? 118 + r.Below(4) : r.Below(200));
        WriteRandom(h.base, dispatcher.address, 1024, r);
        guest::StoreU32(h.base, dispatcher.address + 804, r.Below(50));
        guest::StoreU32(h.base, dispatcher.address + 808, r.Below(50));
        guest::StoreU32(h.base, race.address + 0x59AE4, uint32_t(r.Next()));
        PPCContext t = CallContext(h);
        WriteRandom(h.base, t.r1.u32 - 512, 512, r);
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher.address;
        (v == 0 ? Original<__imp__sub_821674A0> : hooks::RaceStatesCountdownTick)(t, h.base);
        out[v].Bytes(Read(h.base, machine.address + 9600, 64));
        out[v].Bytes(Read(h.base, dispatcher.address, 1024));
        out[v].Word(guest::LoadU32(h.base, race.address + 0x59AE4));
        out[v].Bytes(Read(h.base, t.r1.u32 - 112 - 208 + 80, 44));
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
  Checks(h, progress, vehicles);
  RouteChecks(h, progress, vehicles);
  FollowRouteCases(h, progress, vehicles, manager);
  Handlers(h, progress, vehicles, manager);
  Racers(h, vehicles, manager);
}

}  // namespace harness
