// The race event (src/race_event/, docs/functions/race-event.md) against the
// recompiled functions, with the career's calls into it
// (Career::EventQuery, EventLocation, DayBlock::BestStreak, SetUpEvent). Each case points
// the game's globals at blocks of its own (a save data block in the players'
// slots, the data tables' items, the car specs, the race data, the event,
// the racers' manager, a course's route), shapes the fields the functions
// read from the case's seed, runs the original (with --native_race_event off)
// or the register steps (hooks/race_event.h), and compares what they leave:
// the event, its list and groups (pointers by role: the allocation layer
// gives each run its own), the pools, the manager, the scratch area and r3.
// The globals and rand's seed are put back after each run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "hooks/career.h"
#include "hooks/race_event.h"
#include "memory/heap.h"
#include "race_event/race_event.h"
#include "test_route.h"
#include "track_route/track_route.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_event);
ITC_FLAG_DECLARE(bool, native_career);

namespace harness {

namespace {

namespace re = race_event;
namespace tr = test_route;

constexpr uint32_t kSaveBytes = 0x1F978;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kItems = 400;
constexpr uint32_t kItemBytes = 3008;
constexpr uint32_t kSpecBytes = 8432;
constexpr uint32_t kRaceBytes = 0xA3B00;
constexpr uint32_t kSeed = 0x82638188;  // rand's
constexpr uint32_t kGarage = 6120;

// The misc block's parts.
constexpr uint32_t kEventAt = 0;
constexpr uint32_t kListAt = 0x200;
constexpr uint32_t kEntriesAt = 0x1000;  // 0x200 a group
constexpr uint32_t kPoolsAt = 0x4000;
constexpr uint32_t kManagerAt = 0x6000;
constexpr uint32_t kMiscBytes = 0x8000;

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

// Words mostly 0, else small, now and then any.
void WriteSparse(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t k = r.Below(8);
    guest::StoreU32(base, address + i,
                    k < 4 ? 0 : k < 7 ? r.Below(10) : uint32_t(r.Next()));
  }
}

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

struct World {
  World()
      : save(kSaveBytes),
        tables(0x40 + kItems * kItemBytes),
        specs(0x40 + 4 * kSpecBytes),
        race(kRaceBytes),
        misc(kMiscBytes),
        area(tr::kRouteBytes + 0x100),
        table(4 * tr::kTableEntries) {}
  bool ok() const {
    return save.address && tables.address && specs.address && race.address && misc.address &&
           area.address && table.address;
  }
  uint32_t item(uint32_t id) const { return tables.address + 0x40 + kItemBytes * id; }
  uint32_t event() const { return misc.address + kEventAt; }
  uint32_t list() const { return misc.address + kListAt; }
  uint32_t entries(uint32_t g) const { return misc.address + kEntriesAt + 0x200 * g; }
  uint32_t pools() const { return misc.address + kPoolsAt; }
  uint32_t manager() const { return misc.address + kManagerAt; }
  Block save, tables, specs, race, misc, area, table;
  tr::Route route;
};

// --- Shaping ---

// A condition (20 bytes) of a kind from `kinds` (or any of 0 to 67, now and
// then outside), its arguments shaped by the kind.
void ShapeCondition(uint8_t* base, uint32_t c, Random& r, int kind = -1) {
  if (kind < 0) {
    kind = r.Below(32) == 0 ? int(r.Below(0x10000)) : int(r.Below(68));
    // Kind 40 (EventCondition::Power, the recompiled test) only where a case
    // asks: its car copy and power curve follow the random specs' pointers.
    if (kind == 40) {
      kind = 41;
    }
  }
  guest::StoreU16(base, c + 0, uint16_t(r.Below(5) == 0 ? 0 : 1));
  guest::StoreU16(base, c + 2, uint16_t(kind));
  for (uint32_t k = 0; k < 4; k++) {
    uint32_t a;
    switch (r.Below(8)) {
      case 0:
      case 1:
        a = 0xFFFFFFFF;  // unused
        break;
      case 2:
        a = r.Below(400);
        break;
      case 3:
        a = uint32_t(r.Next());
        break;
      default:
        a = r.Below(12);
        break;
    }
    if (kind == 3 && a == 0 && r.Below(2) == 0) {
      a = 1 + r.Below(7);  // division by 0 now and then only
    }
    guest::StoreU32(base, c + 4 + 4 * k, a);
  }
}

// The items' fields the functions read (rivals only above the item's id,
// so the rival walks end; race kind 3 rare, so they stay short).
void ShapeItems(uint8_t* base, const World& w, Random& r) {
  for (uint32_t id = 0; id < kItems; id++) {
    const uint32_t item = w.item(id);
    guest::StoreU16(base, item + 0, uint16_t(r.Below(32) == 0 ? r.Below(400) : id));
    guest::StoreU8(base, item + 34, uint8_t(r.Below(16) == 0 ? 0xFF : r.Below(50)));
    guest::StoreU8(base, item + 35, uint8_t(r.Below(10) == 0 ? 0xFF : r.Below(6)));
    guest::StoreU8(base, item + 36, uint8_t(r.Below(10)));
    guest::StoreU8(base, item + 37, uint8_t(r.Below(8) == 0 ? 0xF0 + r.Below(16) : r.Below(4)));
    guest::StoreU16(base, item + 40, uint16_t(r.Below(8) == 0 ? r.Next() : r.Below(8)));
    guest::StoreU16(base, item + 42, uint16_t(r.Below(6)));
    guest::StoreU8(base, item + 46, uint8_t(r.Below(3)));
    guest::StoreU8(base, item + 47, uint8_t(r.Below(3) == 0 ? 1 + r.Below(3) : 0));
    // Mostly cheap kinds (0, 61, 67, the save's halfwords), so the unlock
    // states vary.
    for (uint32_t c : {48u, 68u, 100u}) {
      const int cheap[] = {0, 0, 61, 67, 6, 7, 8, 17, 21, 31};
      ShapeCondition(base, item + c, r, r.Below(3) == 0 ? -1 : cheap[r.Below(10)]);
    }
    guest::StoreU16(base, item + 88, uint16_t(r.Below(10) == 0 ? 3 : r.Below(3)));
    guest::StoreU8(base, item + 91, uint8_t(r.Below(3) == 0 ? 1 : 0));
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t rival = id + 1 + r.Below(20);
      guest::StoreU16(base, item + 92 + 2 * k,
                      uint16_t(r.Below(3) == 0 && rival < kItems ? rival : 0xFFFF));
    }
  }
}

void ShapeSave(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.save.address;
  WriteSparse(base, s, 0x2000, r);
  WriteSparse(base, s + kGarage, 4 * 1968 + 2400, r);
  WriteSparse(base, s + re::kItemRecords, 12 * kItems, r);
  for (uint32_t id = 0; id < kItems; id++) {
    for (uint32_t k = 0; k < 8; k += 2) {
      guest::StoreU16(base, s + re::kItemRecords + 12 * id + k,
                      uint16_t(r.Below(3) == 0 ? 1 + r.Below(5) : 0));
    }
  }
  guest::StoreU32(base, s + re::kDay, r.Below(8) == 0 ? 999 + r.Below(5) : r.Below(400));
  guest::StoreU32(base, s + 6092, r.Below(4) == 0 ? uint32_t(r.Next()) : (1u << r.Below(7)) - 1);
  guest::StoreU32(base, s + 6056, r.Below(4));
  guest::StoreU32(base, s + 6108, 10 + r.Below(12));
  guest::StoreU16(base, s + 6328, uint16_t(r.Below(8)));
  guest::StoreU32(base, s + 6024, std::bit_cast<uint32_t>(float(r.Below(40)) * 0.5f));
  guest::StoreU32(base, s + 5952, tr::RandomFloat(r, 20.0f));
  guest::StoreU32(base, s + 4908, std::bit_cast<uint32_t>(float(r.Below(40)) - 3.0f));
  guest::StoreU32(base, s + 6032, tr::RandomFloat(r, 3e9f));
  guest::StoreU16(base, s + 72 + 4830, uint16_t(r.Below(6)));
  // The garage's list: nodes of {next, car} from +0, the head at +204, the
  // count at +208; cars 0 to 3 (their ids, +224, 0 to 3).
  const uint32_t g = s + kGarage;
  for (uint32_t n = 0; n < 50; n++) {
    guest::StoreU16(base, g + 4 * n, uint16_t(r.Below(50)));
    guest::StoreU16(base, g + 4 * n + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, g + 204, uint16_t(r.Below(50)));
  guest::StoreU16(base, g + 208, uint16_t(r.Below(6)));
  guest::StoreU32(base, s + re::kSelectedCar, r.Below(4));
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(base, g + 1968 * c + 224, r.Below(6) == 0 ? 0xFFFFFFFF : r.Below(4));
    guest::StoreU32(base, g + 1968 * c + 2104, tr::RandomFloat(r, 12.0f));
  }
  guest::StoreU32(base, s + 0x1AD10, uint32_t(r.Next()));
  guest::StoreU8(base, s + 0x1F878, uint8_t(r.Next()));
}

void ShapeSpecs(uint8_t* base, const World& w, Random& r) {
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t spec = w.specs.address + 0x40 + kSpecBytes * i + 640;
    guest::StoreU8(base, spec + 68, uint8_t(r.Below(3) == 0 ? 7 : r.Below(9)));
    guest::StoreU8(base, spec + 129, uint8_t(r.Below(12)));
    guest::StoreU8(base, spec + 131, uint8_t(r.Below(3)));
    guest::StoreU32(base, spec + 88, 1985 + r.Below(20));
    guest::StoreU32(base, spec + 92, r.Below(6));
    guest::StoreU32(base, spec + 96, r.Below(2));
    guest::StoreU32(base, spec + 120, r.Below(2));
  }
}

void ShapeRace(uint8_t* base, const World& w, Random& r) {
  const uint32_t race = w.race.address;
  WriteSparse(base, race, 0x8000, r);
  guest::StoreU32(base, race + 20, r.Below(4) == 0 ? uint32_t(r.Next()) : 0x4u << r.Below(4));
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, race + 48 + 3936 * k + 3912, r.Below(400));
  }
  const uint32_t spec = race + 48 + 1888;
  guest::StoreU8(base, spec + 68, uint8_t(r.Below(9)));
  guest::StoreU32(base, spec + 88, 1985 + r.Below(20));
}

// The racers' manager: +2368 the count, 28-byte entries from +4 (+0 flags,
// +8 a route position, +24 the racer's index).
void ShapeManager(uint8_t* base, const World& w, Random& r) {
  const uint32_t m = w.manager();
  WriteRandom(base, m, 4 + 28 * 8, r);
  guest::StoreU32(base, m + 2368, r.Below(8) == 0 ? 0 : 1 + r.Below(7));
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, m + 4 + 28 * i + 24, r.Below(7));
    tr::RandomPosition(base, m + 4 + 28 * i + 8, w.route, r);
  }
}

// An event of the case's own (in the misc block): its list and groups in
// the misc block too (or none).
void ShapeEvent(uint8_t* base, const World& w, Random& r) {
  const uint32_t e = w.event();
  WriteSparse(base, e, 36, r);
  guest::StoreU32(base, e + 0, r.Below(3) == 0 ? 0 : 1);
  guest::StoreU16(base, e + 30, uint16_t(r.Below(4)));
  guest::StoreU16(base, e + 34, uint16_t(r.Below(3) == 0 ? 6 : r.Below(10)));
  const uint32_t n = r.Below(30);
  guest::StoreU16(base, e + 20, uint16_t(n));
  guest::StoreU32(base, e + 156, w.list());
  // All 30 entries a case may point at (the list's n and past it).
  for (uint32_t i = 0; i < 30; i++) {
    const uint32_t entry = w.list() + 12 * i;
    guest::StoreU32(base, entry + 0, r.Below(kItems));
    guest::StoreU32(base, entry + 4, uint32_t(r.Next()));
    guest::StoreU32(base, entry + 8, r.Below(3) == 0 ? 1 : 0);
  }
  for (uint32_t g = 0; g < 10; g++) {
    const uint32_t group = e + 36 + 12 * g;
    const uint32_t open = r.Below(9), locked = r.Below(9);
    guest::StoreU16(base, group + 0, uint16_t(open));
    guest::StoreU16(base, group + 2, uint16_t(locked));
    guest::StoreU32(base, group + 4, w.entries(g));
    guest::StoreU32(base, group + 8, w.entries(g) + 4 * open);
    for (uint32_t i = 0; i < open + locked; i++) {
      guest::StoreU16(base, w.entries(g) + 4 * i,
                      uint16_t(r.Below(16) == 0 ? 0xFFFF : r.Below(kItems)));
      guest::StoreU16(base, w.entries(g) + 4 * i + 2, uint16_t(n == 0 ? 0 : r.Below(n)));
    }
  }
}

// Pools for the 10 locations (counts small).
void ShapePools(uint8_t* base, const World& w, Random& r) {
  for (uint32_t l = 0; l < 10; l++) {
    const uint32_t p = w.pools() + 608 * l;
    WriteRandom(base, p, 608, r);
    const uint32_t counts[] = {r.Below(12), r.Below(12), r.Below(20)};
    const uint32_t at[] = {0, 202, 404};
    for (uint32_t k = 0; k < 3; k++) {
      guest::StoreU16(base, p + at[k], uint16_t(counts[k]));
      for (uint32_t i = 0; i < counts[k]; i++) {
        guest::StoreU16(base, p + at[k] + 2 + 2 * i, uint16_t(r.Below(kItems)));
      }
    }
  }
}

// --- Running a case ---

using SetupFn = std::function<void(Random&, const World&, PPCContext&)>;
using CaptureFn = std::function<void(Outputs&, const World&, PPCContext&)>;

// The original with --native_race_event (and --native_career) off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_race_event) = false;
  ITC_FLAG(native_career) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_race_event) = true;
  ITC_FLAG(native_career) = true;
}

void Case(Context& h, Results& results, World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const CaptureFn& capture,
          const std::string& what, bool r3) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    Globals globals(h.base, {kSaveSlots, kSaveSlots + 4, kSaveSlots + 8, kSaveSlots + 12,
                             re::kRaceData, data_tables::kTables, car_specs::kParams,
                             re::kEvent, re::kManager, track_route::kRoute, kSeed});
    PPCContext t = CallContext(h);
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    for (uint32_t p = 0; p < 4; p++) {
      guest::StoreU32(h.base, kSaveSlots + 4 * p, w.save.address);
    }
    guest::StoreU32(h.base, w.tables.address + 4, r.Below(16) == 0 ? 1 : 0);
    guest::StoreU32(h.base, w.tables.address + 8, kItems);
    guest::StoreU32(h.base, w.tables.address + 16, w.tables.address + 0x40);
    guest::StoreU32(h.base, data_tables::kTables, w.tables.address);
    guest::StoreU32(h.base, w.specs.address + 12, w.specs.address + 0x40);
    // No car specs only where a case's setup says (EventCondition::Power's
    // Car::Refresh reads them unchecked: a null global faults, the original
    // too).
    guest::StoreU32(h.base, car_specs::kParams, w.specs.address);
    guest::StoreU32(h.base, re::kRaceData, w.race.address);
    // No event only where a case's setup says (EventCondition::SectionRival
    // reads the event without a check: a null one faults, the original too).
    guest::StoreU32(h.base, re::kEvent, w.event());
    guest::StoreU32(h.base, re::kManager, r.Below(8) == 0 ? 0 : w.manager());
    w.route = tr::BuildRoute(h.base, w.area.address, w.table.address, r);
    guest::StoreU32(h.base, track_route::kRoute, w.area.address + tr::kFile);
    guest::StoreU32(h.base, kSeed, uint32_t(r.Next()));
    ShapeItems(h.base, w, r);
    ShapeSave(h.base, w, r);
    ShapeSpecs(h.base, w, r);
    ShapeRace(h.base, w, r);
    ShapeManager(h.base, w, r);
    ShapeEvent(h.base, w, r);
    ShapePools(h.base, w, r);
    setup(r, w, t);
    (v == 0 ? recompiled : native)(t, h.base);
    capture(out[v], w, t);
    out[v].Word(guest::LoadU32(h.base, kSeed));
    if (r3) {
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
  }
  results.Check(out[0], out[1], what);
}

// `r3`: false for the functions returning nothing (the originals' r3 is
// what their last callee left, and no caller reads it).
void Group(Context& h, World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const CaptureFn& capture,
           bool r3 = true) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, capture,
         fmt::format("case {}", c), r3);
  }
  results.Report();
}

// Pages from the allocation layer for an event's list or a group's entries,
// filled at random.
uint32_t RandomPages(uint8_t* base, uint32_t bytes, Random& r) {
  const uint32_t pages = memory::AllocPages(bytes, 4096);
  WriteRandom(base, pages, bytes, r);
  return pages;
}

// The event's list and groups in pages of their own, or none.
void PagedEvent(uint8_t* base, uint32_t e, Random& r) {
  guest::StoreU32(base, e + 156, r.Below(2) == 0 ? 0 : RandomPages(base, 12 * 40, r));
  for (uint32_t g = 0; g < 10; g++) {
    const uint32_t group = e + 36 + 12 * g;
    if (r.Below(2) == 0) {
      guest::StoreU32(base, group + 4, 0);
      guest::StoreU32(base, group + 8, 0);
    } else {
      const uint32_t entries = RandomPages(base, 4 * 32, r);
      guest::StoreU32(base, group + 4, entries);
      guest::StoreU32(base, group + 8, entries + 4 * r.Below(9));
    }
  }
}

// The event at `e` with its pointers by role (whether set, the locked part's
// offset), its list and groups' entries; the layer's pages among them then
// freed (each run made its own).
void CaptureEvent(Outputs& out, uint8_t* base, uint32_t e) {
  std::vector<uint8_t> bytes = Read(base, e, 160);
  const auto clear = [&](uint32_t offset) { std::memset(bytes.data() + offset, 0, 4); };
  const uint32_t list = guest::LoadU32(base, e + 156);
  out.Word(list != 0 ? 1 : 0);
  clear(156);
  for (uint32_t g = 0; g < 10; g++) {
    const uint32_t entries = guest::LoadU32(base, e + 40 + 12 * g);
    out.Word(entries != 0 ? 1 : 0);
    out.Word(guest::LoadU32(base, e + 44 + 12 * g) - entries);
    clear(40 + 12 * g);
    clear(44 + 12 * g);
  }
  out.Bytes(bytes);
  if (list != 0) {
    const int32_t n = int16_t(guest::LoadU16(base, e + 22)) + int16_t(guest::LoadU16(base, e + 24));
    if (n > 0 && n < 300) {
      out.Bytes(Read(base, list, 12 * uint32_t(n)));
    }
    if (memory::IsPages(list)) {
      memory::FreePages(list);
    }
  }
  for (uint32_t g = 0; g < 10; g++) {
    const uint32_t group = e + 36 + 12 * g;
    const uint32_t entries = guest::LoadU32(base, group + 4);
    if (entries == 0) {
      continue;
    }
    const int32_t n = int16_t(guest::LoadU16(base, group + 0)) +
                      int16_t(guest::LoadU16(base, group + 2));
    if (n > 0 && n < 300) {
      out.Bytes(Read(base, entries, 4 * uint32_t(n)));
    }
    if (memory::IsPages(entries)) {
      memory::FreePages(entries);
    }
  }
}

// --- The native sides with no adapter of their own ---

void NativeBestStreak(PPCContext& t, uint8_t* base) {
  t.r3.u64 = career::DayBlockBestStreak(base, t.r3.u32);
}

// The cases' registers.
uint32_t Scratch(Context& h, uint32_t offset) { return h.data + offset; }

// No event now and then (for the functions that check).
void MaybeNoEvent(uint8_t* base, Random& r) {
  if (r.Below(6) == 0) {
    guest::StoreU32(base, re::kEvent, 0);
  }
}

void Conditions(Context& h, World& w) {
  uint8_t* const base = h.base;
  const CaptureFn none = [](Outputs&, const World&, PPCContext&) {};
  const auto condition_args = [&h, base](Random& r, const World& w, PPCContext& t) {
    const uint32_t c = Scratch(h, 0x100), level = Scratch(h, 0x140);
    ShapeCondition(base, c, r, r.Below(10) == 0 ? -1 : int(r.Below(68)));
    if (guest::LoadU16(base, c + 2) != 65) {
      MaybeNoEvent(base, r);
    }
    if (guest::LoadU16(base, c + 2) != 40 && r.Below(4) == 0) {
      guest::StoreU32(base, car_specs::kParams, 0);  // the race car's paths
    }
    guest::StoreU16(base, level, uint16_t(r.Below(6)));
    t.r3.u64 = c;
    t.r4.u64 = level;
  };
  Group(h, w, "EventCondition::Met (sub_82142820, its tests)", 1400, condition_args,
        Original<__imp__sub_82142820>, hooks::RaceEventConditionMet, none);
  Group(h, w, "EventCondition::State (sub_82142C00)", 300, condition_args,
        Original<__imp__sub_82142C00>, hooks::RaceEventConditionState, none);
  const auto block_args = [base](Random& r, const World& w, PPCContext& t) {
    t.r3.u64 = w.item(r.Below(kItems)) + 40;
    (void)base;
  };
  Group(h, w, "Unlock::All (sub_82142C80)", 150, block_args, Original<__imp__sub_82142C80>,
        hooks::RaceEventUnlockAll, none);
  Group(h, w, "Unlock::Any (sub_82142CE0)", 150, block_args, Original<__imp__sub_82142CE0>,
        hooks::RaceEventUnlockAny, none);
  const auto item_args = [](Random& r, const World& w, PPCContext& t) {
    t.r3.u64 = w.item(r.Below(kItems));
  };
  Group(h, w, "Unlock::State (sub_82142EC0)", 200, item_args, Original<__imp__sub_82142EC0>,
        hooks::RaceEventUnlockState, none);
  Group(h, w, "Unlock::StateKind2 (sub_82142D40)", 100, item_args,
        Original<__imp__sub_82142D40>, hooks::RaceEventUnlockStateKind2, none);
  Group(h, w, "Unlock::StateKind5 (sub_82142E18)", 100, item_args,
        Original<__imp__sub_82142E18>, hooks::RaceEventUnlockStateKind5, none);
  Group(
      h, w, "Unlock::HasSectionCondition (sub_82140308)", 64,
      [base](Random& r, const World& w, PPCContext& t) {
        const uint32_t item = w.item(r.Below(kItems));
        if (r.Below(2) == 0) {
          guest::StoreU16(base, item + (r.Below(2) == 0 ? 50 : 70), 65);
        }
        t.r3.u64 = item + 40;
      },
      Original<__imp__sub_82140308>, hooks::RaceEventHasSectionCondition, none);
  Group(
      h, w, "EventCondition::CategoryMask (sub_82140340)", 100,
      [&h, base](Random& r, const World&, PPCContext& t) {
        guest::StoreU16(base, Scratch(h, 0x100), uint16_t(r.Below(6)));
        guest::StoreU32(base, Scratch(h, 0x104), r.Below(8) == 0 ? r.Below(40) : r.Below(10));
        guest::StoreU32(base, Scratch(h, 0x108),
                        r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8));
        t.r3.u64 = Scratch(h, 0x100);
        t.r4.u64 = Scratch(h, 0x104);
        t.r5.u64 = Scratch(h, 0x108);
      },
      Original<__imp__sub_82140340>, hooks::RaceEventCategoryMask, none);
  Group(
      h, w, "EventCondition::FloatAtLeast (sub_82140400)", 100,
      [&h, base](Random& r, const World&, PPCContext& t) {
        ShapeCondition(base, Scratch(h, 0x100), r, 11);
        guest::StoreU32(base, Scratch(h, 0x140), r.Below(2) == 0
                                                     ? tr::RandomFloat(r, 12.0f)
                                                     : std::bit_cast<uint32_t>(
                                                           float(r.Below(12))));
        t.r3.u64 = Scratch(h, 0x100);
        t.r4.u64 = Scratch(h, 0x140);
      },
      Original<__imp__sub_82140400>, hooks::RaceEventFloatAtLeast, none);
  Group(
      h, w, "Items::Available (sub_821435C8)", 200,
      [](Random& r, const World&, PPCContext& t) { t.r3.u64 = r.Below(kItems); },
      Original<__imp__sub_821435C8>, hooks::RaceEventItemsAvailable, none);
  Group(
      h, w, "Items::Beaten (sub_82143528)", 200,
      [](Random& r, const World& w, PPCContext& t) {
        t.r3.u64 = w.save.address + re::kItemRecords;
        t.r4.u64 = w.item(r.Below(kItems));
      },
      Original<__imp__sub_82143528>, hooks::RaceEventItemsBeaten, none);
  Group(
      h, w, "DayBlock::BestStreak (sub_82110588)", 16,
      [](Random& r, const World& w, PPCContext& t) {
        t.r3.u64 = w.save.address + 4 * r.Below(1000);
      },
      Original<__imp__sub_82110588>, NativeBestStreak, none);
}

void Events(Context& h, World& w) {
  uint8_t* const base = h.base;
  // The misc block (event, list, groups, pools, manager) and the scratch area.
  const CaptureFn misc = [&h, base](Outputs& out, const World& w, PPCContext&) {
    out.Bytes(Read(base, w.misc.address, w.misc.bytes));
    out.Bytes(Read(base, h.data, kScratchBytes));
  };
  const CaptureFn event = [&h, base](Outputs& out, const World& w, PPCContext&) {
    CaptureEvent(out, base, w.event());
    out.Bytes(Read(base, w.misc.address + 160, w.misc.bytes - 160));
    out.Bytes(Read(base, h.data, kScratchBytes));
  };
  const auto event_r3 = [](Random&, const World& w, PPCContext& t) { t.r3.u64 = w.event(); };
  Group(h, w, "RaceEvent::RaceEvent (sub_82142F20)", 8, event_r3, Original<__imp__sub_82142F20>,
        hooks::RaceEventConstruct, misc, false);
  Group(
      h, w, "RaceEvent::SetMode (sub_82142F68)", 32,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        guest::StoreU32(base, Scratch(h, 0x100), r.Below(4));
        guest::StoreU32(base, Scratch(h, 0x104), r.Below(20));
        t.r3.u64 = w.event();
        t.r4.u64 = Scratch(h, 0x100);
        t.r5.u64 = Scratch(h, 0x104);
      },
      Original<__imp__sub_82142F68>, hooks::RaceEventSetMode, misc, false);
  // Alloc: counts from the event (some 0), its pointers none.
  const auto alloc_args = [base](Random& r, const World& w, PPCContext& t) {
    const uint32_t e = w.event();
    guest::StoreU16(base, e + 22, uint16_t(r.Below(4) == 0 ? 0 : r.Below(20)));
    guest::StoreU16(base, e + 24, uint16_t(r.Below(4) == 0 ? 0 : r.Below(20)));
    guest::StoreU32(base, e + 156, 0);
    for (uint32_t g = 0; g < 10; g++) {
      guest::StoreU32(base, e + 40 + 12 * g, 0);
      guest::StoreU32(base, e + 44 + 12 * g, 0);
    }
    t.r3.u64 = e;
  };
  Group(h, w, "RaceEvent::Alloc (sub_82143620)", 40, alloc_args,
        Original<__imp__sub_82143620>, hooks::RaceEventAlloc, event);
  Group(
      h, w, "EventGroup::Alloc (sub_821433A8)", 40,
      [alloc_args](Random& r, const World& w, PPCContext& t) {
        alloc_args(r, w, t);
        t.r3.u64 = w.event() + 36 + 12 * r.Below(10);
      },
      Original<__imp__sub_821433A8>, hooks::RaceEventGroupAlloc, event);
  const auto free_args = [base](Random& r, const World& w, PPCContext& t) {
    PagedEvent(base, w.event(), r);
    t.r3.u64 = w.event();
  };
  Group(h, w, "RaceEvent::Free (sub_821436C8)", 32, free_args, Original<__imp__sub_821436C8>,
        hooks::RaceEventFree, event, false);
  Group(
      h, w, "EventGroup::Free (sub_82143358)", 32,
      [free_args](Random& r, const World& w, PPCContext& t) {
        free_args(r, w, t);
        t.r3.u64 = w.event() + 36 + 12 * r.Below(10);
      },
      Original<__imp__sub_82143358>, hooks::RaceEventGroupFree, event, false);
  // The pools.
  const auto pools_item = [](Random& r, const World& w, PPCContext& t) {
    t.r3.u64 = w.pools() + 608 * r.Below(10);
    const uint32_t id = r.Below(kItems);
    t.r4.u64 = w.item(id);
    t.r5.u64 = w.save.address + re::kItemRecords + 12 * id;
    t.r6.u64 = w.save.address + re::kItemRecords;
  };
  Group(h, w, "EventPools::AddItem (sub_82144950)", 200, pools_item,
        Original<__imp__sub_82144950>, hooks::RaceEventPoolsAddItem, misc, false);
  Group(h, w, "EventPools::AddRivals (sub_82144458)", 100, pools_item,
        Original<__imp__sub_82144458>, hooks::RaceEventPoolsAddRivals, misc, false);
  Group(
      h, w, "EventPools::AddRivalsOf (sub_821440F8)", 100,
      [base, pools_item](Random& r, const World& w, PPCContext& t) {
        pools_item(r, w, t);
        if (r.Below(2) == 0) {
          guest::StoreU16(base, t.r4.u32 + 88, 3);
        }
        t.r4.u64 = t.r4.u32 + (r.Below(2) == 0 ? 0 : 92 + 2 * r.Below(3));
      },
      Original<__imp__sub_821440F8>, hooks::RaceEventPoolsAddRivalsOf, misc, false);
  Group(
      h, w, "EventPools::Trim (sub_821444D0)", 200,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        guest::StoreU16(base, Scratch(h, 0x100),
                        uint16_t(r.Below(8) == 0 ? r.Below(0x10000) : r.Below(14)));
        t.r3.u64 = w.pools() + 608 * r.Below(10);
        t.r4.u64 = Scratch(h, 0x100);
      },
      Original<__imp__sub_821444D0>, hooks::RaceEventPoolsTrim, misc, false);
  Group(
      h, w, "EventPools::CopyIds (sub_82143420)", 64,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        const uint32_t at[] = {0, 202, 404};
        guest::StoreU32(base, Scratch(h, 0x100), r.Below(30));
        guest::StoreU32(base, Scratch(h, 0x104), r.Below(2));
        t.r3.u64 = w.pools() + 608 * r.Below(10) + at[r.Below(3)];
        t.r4.u64 = w.entries(r.Below(10));
        t.r5.u64 = w.list();
        t.r6.u64 = Scratch(h, 0x100);
        t.r7.u64 = Scratch(h, 0x104);
      },
      Original<__imp__sub_82143420>, hooks::RaceEventPoolsCopyIds, misc, false);
  Group(
      h, w, "EventPools::ToGroup (sub_82144170)", 64,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        guest::StoreU32(base, Scratch(h, 0x100), r.Below(30));
        t.r3.u64 = w.pools() + 608 * r.Below(10);
        t.r4.u64 = w.event() + 36 + 12 * r.Below(10);
        t.r5.u64 = w.list();
        t.r6.u64 = Scratch(h, 0x100);
      },
      Original<__imp__sub_82144170>, hooks::RaceEventPoolsToGroup, misc, false);
  Group(
      h, w, "EventPools::ToGroupNoList (sub_821441D8)", 64,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        guest::StoreU32(base, Scratch(h, 0x100), r.Below(30));
        t.r3.u64 = w.pools() + 608 * r.Below(10);
        t.r4.u64 = w.event() + 36 + 12 * r.Below(10);
        t.r5.u64 = Scratch(h, 0x100);
      },
      Original<__imp__sub_821441D8>, hooks::RaceEventPoolsToGroupNoList, misc, false);
  // RaceEvent::Init and Make: the event's old list and groups the layer's
  // pages (or none).
  const auto init_args = [&h, base](Random& r, const World& w, PPCContext& t) {
    PagedEvent(base, w.event(), r);
    guest::StoreU32(base, Scratch(h, 0x100), r.Below(8) == 0 ? r.Below(16) : r.Below(4));
    guest::StoreU32(base, Scratch(h, 0x104), r.Below(20));
    t.r3.u64 = w.event();
    t.r4.u64 = Scratch(h, 0x100);
    t.r5.u64 = Scratch(h, 0x104);
    t.r6.u64 = Scratch(h, 0x108);
  };
  Group(h, w, "RaceEvent::Init (sub_82144A50)", 60, init_args, Original<__imp__sub_82144A50>,
        hooks::RaceEventInit, event);
  Group(
      h, w, "RaceEvent::Make (sub_82144E58)", 60,
      [&h, base, init_args](Random& r, const World& w, PPCContext& t) {
        init_args(r, w, t);
        if (r.Below(2) == 0) {
          guest::StoreU32(base, re::kEvent, 0);
        }
        t.r3.u64 = Scratch(h, 0x100);
        t.r4.u64 = Scratch(h, 0x104);
        t.r5.u64 = Scratch(h, 0x108);
      },
      Original<__imp__sub_82144E58>, hooks::RaceEventMake,
      [&h, base](Outputs& out, const World& w, PPCContext&) {
        const uint32_t e = guest::LoadU32(base, re::kEvent);
        out.Word(e == 0 ? 0 : e == w.event() ? 1 : 2);
        if (e != 0) {
          CaptureEvent(out, base, e);
          if (e != w.event()) {
            memory::Free(e);
          }
        }
        if (e != w.event()) {
          CaptureEvent(out, base, w.event());
        }
        out.Bytes(Read(base, h.data, kScratchBytes));
      });
  Group(
      h, w, "RaceEvent::Destroy (sub_82144DF0)", 32,
      [base](Random& r, const World& w, PPCContext&) {
        MaybeNoEvent(base, r);
        if (guest::LoadU32(base, re::kEvent) != 0) {
          const uint32_t e = memory::Alloc(160, 0);
          WriteRandom(base, e, 160, r);
          PagedEvent(base, e, r);
          guest::StoreU32(base, re::kEvent, e);
        }
        (void)w;
      },
      Original<__imp__sub_82144DF0>, hooks::RaceEventDestroy,
      [base](Outputs& out, const World&, PPCContext&) {
        out.Word(guest::LoadU32(base, re::kEvent));
      },
      false);
}

void Groups(Context& h, World& w) {
  uint8_t* const base = h.base;
  const CaptureFn misc = [&h, base](Outputs& out, const World& w, PPCContext&) {
    out.Bytes(Read(base, w.misc.address, w.misc.bytes));
    out.Bytes(Read(base, h.data, kScratchBytes));
  };
  const auto group_args = [](Random& r, const World& w, PPCContext& t) {
    t.r3.u64 = w.event() + 36 + 12 * r.Below(10);
    t.r4.u64 = w.list();
  };
  Group(h, w, "EventGroup::PromoteFlagged (sub_82143C70)", 100, group_args,
        Original<__imp__sub_82143C70>, hooks::RaceEventGroupPromoteFlagged, misc, false);
  Group(h, w, "EventGroup::SortOpen (sub_82143DC8)", 100, group_args,
        Original<__imp__sub_82143DC8>, hooks::RaceEventGroupSortOpen, misc, false);
  Group(h, w, "EventGroup::PromoteUnlocked (sub_82143F90)", 100, group_args,
        Original<__imp__sub_82143F90>, hooks::RaceEventGroupPromoteUnlocked, misc, false);
  Group(
      h, w, "RaceEvent::Refresh (sub_82144300)", 64,
      [](Random&, const World& w, PPCContext& t) { t.r3.u64 = w.event(); },
      Original<__imp__sub_82144300>, hooks::RaceEventRefresh, misc);
  Group(
      h, w, "RaceEvent::RefreshCurrent (sub_82144720)", 32,
      [base](Random& r, const World&, PPCContext&) { MaybeNoEvent(base, r); },
      Original<__imp__sub_82144720>,
      hooks::RaceEventRefreshCurrent, misc);
  Group(
      h, w, "RaceEvent::UpdateLocation (sub_82144740)", 64,
      [&h, base](Random& r, const World&, PPCContext& t) {
        guest::StoreU32(base, Scratch(h, 0x100), r.Below(10));
        t.r3.u64 = Scratch(h, 0x100);
        MaybeNoEvent(base, r);
      },
      Original<__imp__sub_82144740>, hooks::RaceEventUpdateLocation, misc, false);
  Group(
      h, w, "RaceEvent::GroupItem (sub_821431C0)", 64,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        guest::StoreU32(base, Scratch(h, 0x100), r.Below(13));
        guest::StoreU32(base, Scratch(h, 0x104), r.Below(8) == 0 ? 0xFFFFFFFF : r.Below(10));
        t.r3.u64 = w.event();
        t.r4.u64 = Scratch(h, 0x100);
        t.r5.u64 = Scratch(h, 0x104);
      },
      Original<__imp__sub_821431C0>, hooks::RaceEventGroupItem, misc);
  Group(
      h, w, "Career::EventQuery (sub_82111638)", 32,
      [base](Random& r, const World&, PPCContext& t) {
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : 0;
        MaybeNoEvent(base, r);
      },
      Original<__imp__sub_82111638>, hooks::CareerEventQuery, misc);
  Group(
      h, w, "Career::EventLocation (sub_82111650)", 32,
      [base](Random& r, const World& w, PPCContext& t) {
        t.r3.u64 = w.save.address;
        MaybeNoEvent(base, r);
      },
      Original<__imp__sub_82111650>, hooks::CareerEventLocation, misc, false);
}

// The list's items made to place: +46 1, both conditions met (kind 0, all),
// kind 65 now and then; racers found for some.
void ShapePlacing(uint8_t* base, const World& w, Random& r) {
  const uint32_t e = w.event();
  guest::StoreU32(base, e + 0, r.Below(4) == 0 ? 0 : 1);
  for (uint32_t k = 4; k <= 16; k += 4) {
    guest::StoreU32(base, e + k, r.Below(4) == 0 ? 0 : 1);
  }
  guest::StoreU16(base, w.save.address + 72 + 4830, uint16_t(r.Below(6)));
  const int32_t n = int16_t(guest::LoadU16(base, e + 20));
  for (int32_t i = 0; i < n; i++) {
    const uint32_t entry = w.list() + 12 * uint32_t(i);
    const uint32_t id = guest::LoadU32(base, entry + 0);
    const uint32_t item = w.item(id);
    if (r.Below(2) == 0) {
      guest::StoreU8(base, item + 46, 1);
      guest::StoreU8(base, item + 35, uint8_t(r.Below(2)));
      guest::StoreU8(base, item + 47, 0);
      ShapeCondition(base, item + 48, r, r.Below(2) == 0 ? 65 : 0);
      guest::StoreU16(base, item + 48, 1);
      ShapeCondition(base, item + 68, r, 0);
    }
    if (r.Below(2) == 0) {
      const uint32_t racer = r.Below(7);
      guest::StoreU32(base, w.race.address + 48 + 3936 * racer + 3912,
                      uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 0)))));
    }
  }
}

void Placing(Context& h, World& w) {
  uint8_t* const base = h.base;
  const CaptureFn misc = [&h, base](Outputs& out, const World& w, PPCContext&) {
    out.Bytes(Read(base, w.misc.address, w.misc.bytes));
    out.Bytes(Read(base, h.data, kScratchBytes));
  };
  Group(
      h, w, "RaceEvent::FindRacer (sub_82142FC8)", 100,
      [&h, base](Random& r, const World& w, PPCContext& t) {
        ShapePlacing(base, w, r);
        t.r3.u64 = w.list() + 12 * r.Below(30);
        t.r4.u64 = Scratch(h, 0x100);
      },
      Original<__imp__sub_82142FC8>, hooks::RaceEventFindRacer, misc);
  const auto position_args = [&h, base](Random& r, const World& w, PPCContext& t) {
    ShapePlacing(base, w, r);
    tr::RandomPosition(base, Scratch(h, 0x100), w.route, r);
    guest::StoreU32(base, Scratch(h, 0x104), r.Below(3) == 0 ? 0 : 1);
    t.r3.u64 = w.event();
    t.r4.u64 = Scratch(h, 0x100);
    t.r5.u64 = Scratch(h, 0x104);
  };
  Group(h, w, "RaceEvent::SectionRival (sub_82143210)", 150, position_args,
        Original<__imp__sub_82143210>, hooks::RaceEventSectionRival, misc);
  Group(
      h, w, "RaceEvent::SectionRivalCurrent (sub_821434A0)", 64,
      [base, position_args](Random& r, const World& w, PPCContext& t) {
        position_args(r, w, t);
        t.r3.u64 = t.r4.u64;
        MaybeNoEvent(base, r);
      },
      Original<__imp__sub_821434A0>, hooks::RaceEventSectionRivalCurrent, misc);
  Group(h, w, "RaceEvent::PlaceUnlocked (sub_82143740)", 150, position_args,
        Original<__imp__sub_82143740>, hooks::RaceEventPlaceUnlocked, misc);
  Group(
      h, w, "RaceEvent::PlaceUnlockedCurrent (sub_821442D8)", 64,
      [base, position_args](Random& r, const World& w, PPCContext& t) {
        position_args(r, w, t);
        t.r3.u64 = t.r4.u64;
        t.r4.u64 = t.r5.u64;
        MaybeNoEvent(base, r);
      },
      Original<__imp__sub_821442D8>, hooks::RaceEventPlaceUnlockedCurrent, misc);
}

// Career::SetUpEvent: tables 0 to 7 of 56-byte entries in a block of their
// own (up to 12 entries each, then the end), the save data's table words,
// table 0's done bits and list, the offers; the race data reset by
// RaceData::ResetGlobal; the event made as in RaceEvent::Make's cases. Its
// frame (below r1, 121,200 bytes) filled at random before each run and
// compared from +80 to its entries' end; below it (its callees' frames,
// where the originals save registers and Condition::Test copies items) not.
constexpr uint32_t kTableBytes = 0x400;  // a table's entries

// Returns table 0's entries.
uint32_t ShapeSetUpTables(uint8_t* base, uint32_t tables, Random& r) {
  uint32_t entries0 = 0;
  for (uint32_t t = 0; t < 8; t++) {
    const uint32_t table = tables + kTableBytes * t;
    const uint32_t n = r.Below(4) == 0 ? 0 : r.Below(13);
    for (uint32_t i = 0; i < n; i++) {
      const uint32_t e = table + 56 * i;
      WriteRandom(base, e, 56, r);
      guest::StoreU32(base, e + 0, r.Below(4) == 0 ? r.Below(13) : r.Below(6));
      uint32_t mask = uint32_t(r.Next());
      switch (r.Below(4)) {
        case 0:
          mask &= 0x40;  // the stage bits only: table 0's skip
          break;
        case 1:
          mask &= ~0x7F80u;  // no mode or stage bits
          break;
        case 2:
          mask |= 0x7FFFu;
          break;
        default:
          break;
      }
      guest::StoreU32(base, e + 4, mask);
      guest::StoreU32(base, e + 20, r.Below(2));
      for (uint32_t k = 0; k < 3; k++) {
        const uint32_t c = e + 24 + 8 * k;
        // Kinds 7 to 9 (Condition::Test's item counts) rare: each copies
        // the 400 items.
        uint32_t kind = r.Below(40) == 0 ? 7 + r.Below(3) : r.Below(7);
        if (r.Below(5) == 0) {
          kind = 0xFFFFFFFFu;
        }
        guest::StoreU32(base, c, kind);
        guest::StoreU32(base, c + 4,
                        r.Below(10) == 0 ? 0xFFFFFFFFu : r.Below(kind == 5 || kind == 6 ? 60 : 9));
      }
    }
    guest::StoreU32(base, table + 56 * n, 0xFFFFFFFFu);
    if (t == 0) {
      entries0 = n;
    }
  }
  return entries0;
}

void SetUpEvents(Context& h, World& w) {
  uint8_t* const base = h.base;
  const Block tables(8 * kTableBytes);
  if (!tables.address) {
    ITC_LOG_ERROR("harness: race_event: no block for the tables");
    return;
  }
  const uint32_t frame_bytes = career::kSetUpEventFrame;
  const uint32_t compared = 1280 + 13 * 6 * 192 * 8 - 80;
  uint64_t runs = 0;
  Group(
      h, w, "Career::SetUpEvent (sub_82114268)", 64,
      [&h, base, &tables, frame_bytes, &runs](Random& shared, const World& w, PPCContext& t) {
        // A generator of the case's own (the same for its two runs): the
        // cases' streams are the shared one's shifted, and fall into step
        // when their draws' counts differ.
        const uint64_t c = runs++ / 2;
        Random r((shared.Next() ^ (c * 0x9E3779B97F4A7C15ull)) * 0x2545F4914F6CDD1Dull + c);
        const uint32_t save = w.save.address;
        const uint32_t entries0 = ShapeSetUpTables(base, tables.address, r);
        for (uint32_t k = 0; k < 8; k++) {
          guest::StoreU32(base, w.tables.address + 20 + 4 * k, tables.address + kTableBytes * k);
        }
        for (uint32_t i = 0; i < 0x600 / 4; i++) {
          const uint32_t k = r.Below(6);
          guest::StoreU32(base, save + 0x1B278 + 4 * i, k < 3 ? k : k == 3 ? 2 : uint32_t(r.Next()));
        }
        // Table 0's done bits only over its entries: the walk skips a done
        // entry without testing for the end, so past the end it would read
        // whatever follows the table (as the original would).
        const uint64_t done = (uint64_t(r.Next()) & uint64_t(r.Next())) &
                              ((uint64_t(1) << entries0) - 1);
        guest::StoreU32(base, save + 0x1CA7C, uint32_t(done));
        guest::StoreU32(base, save + 0x1CA7C + 4, uint32_t(done >> 32));
        guest::StoreU32(base, save + 0x1C778 + 768, r.Below(20));
        for (uint32_t l = 0; l < 6; l++) {
          guest::StoreU32(base, save + 0x1AD20 + 100 * l + 96, r.Below(8));
        }
        const uint32_t days[] = {1, 1, 0, 2, 7, 1000};
        guest::StoreU32(base, save + re::kDay, r.Below(2) == 0 ? days[r.Below(6)] : r.Below(400));
        guest::StoreU32(base, save + 6092, (1u << r.Below(8)) - 1);
        const uint32_t locations[] = {14, 15, 16, 17, 18, 19, 48, 5, 20, 0xFFFFFFFFu};
        // The event as in RaceEvent::Make's cases.
        PagedEvent(base, w.event(), r);
        if (r.Below(2) == 0) {
          guest::StoreU32(base, re::kEvent, 0);
        }
        const uint32_t sp = t.r1.u32;
        WriteRandom(base, sp - frame_bytes, frame_bytes, r);
        WriteRandom(base, sp, h.data - sp, r);
        t.r3.u64 = save;
        t.r4.u64 = r.Below(8) == 0 ? r.Below(8) : r.Below(4);
        t.r5.u64 = r.Below(4) == 0 ? locations[6 + r.Below(4)] : locations[r.Below(6)];
      },
      Original<__imp__sub_82114268>, hooks::CareerSetUpEvent,
      [&h, base, frame_bytes, compared](Outputs& out, const World& w, PPCContext&) {
        const uint32_t e = guest::LoadU32(base, re::kEvent);
        out.Word(e == 0 ? 0 : e == w.event() ? 1 : 2);
        if (e != 0) {
          CaptureEvent(out, base, e);
          if (e != w.event()) {
            memory::Free(e);
          }
        }
        if (e != w.event()) {
          CaptureEvent(out, base, w.event());
        }
        out.Bytes(Read(base, w.save.address, w.save.bytes));
        out.Bytes(Read(base, w.race.address, w.race.bytes));
        out.Bytes(Read(base, h.data, kScratchBytes));
        const uint32_t sp = CallContext(h).r1.u32;
        out.Bytes(Read(base, sp, h.data - sp));
        out.Bytes(Read(base, sp - frame_bytes + 80, compared));
      });
}

}  // namespace

void RaceEvent(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_event: no block from the allocation layer");
    return;
  }
  tr::FillTable(h.base, w.table.address, w.area.address);
  WriteRandom(h.base, w.save.address, w.save.bytes, h.random);
  WriteRandom(h.base, w.tables.address, w.tables.bytes, h.random);
  WriteRandom(h.base, w.specs.address, w.specs.bytes, h.random);
  WriteRandom(h.base, w.race.address, w.race.bytes, h.random);
  Conditions(h, w);
  Events(h, w);
  Groups(h, w);
  Placing(h, w);
  SetUpEvents(h, w);
}

}  // namespace harness
