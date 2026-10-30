// The career's progress (src/career/, docs/functions/career.md) against the
// recompiled functions. Each case fills a save data block (the players'
// slot 0 pointed at it), the data tables' items (a table object of our own
// in [0x826C5CA0]) where the function reads them, the scratch area and the
// caller's area above the stack pointer with random words from the case's
// seed, shapes the fields the function tests, runs the original (with
// --native_career off, so its callees are the originals too) or the
// register steps (hooks/career.h), and compares the save block, the scratch
// area, the 1,024 bytes above r1 (the caller's frame, where the originals
// store their doubleword arguments) and r3 (all 64 bits): exactly, except
// the floats the functions compute (harness::Outputs' floats). The globals
// the cases point elsewhere (the save slots, the data tables, the car
// specs, the play time's counter) are put back after each run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "hooks/career.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_career);

namespace harness {

namespace {

namespace cr = career;

constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kItems = 400;
constexpr uint32_t kItemBytes = 3008;
constexpr uint32_t kRaceDataBytes = 0xA3B00;
constexpr uint32_t kRaceDataSlot = 0x826C5B2C;
constexpr uint32_t kTable0 = 0x1000;   // in the race block: the cases' table 0
constexpr uint32_t kTable40 = 0x2000;  // and their 40-byte table
constexpr uint32_t kResults = 0x59AE8;            // the race data's result records
constexpr uint32_t kResultCount = 0x59AE8 + 0x49FE0;
constexpr uint32_t kRandSeed = 0x82638188;  // rand's (Car::SetModel's picks)

// A block of guest memory for the cases, freed at the end.
struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

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

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

// The cases' memory: a save data block, the data tables' object (+0x40 the
// items), the car specs' object (+0x40 four specs).
struct World {
  World()
      : save(kSaveDataBytes),
        tables(0x40 + kItems * kItemBytes),
        specs(0x40 + 4 * 8432),
        race(kRaceDataBytes) {}
  bool ok() const { return save.address && tables.address && specs.address && race.address; }
  uint32_t items() const { return tables.address + 0x40; }
  uint32_t item(uint32_t id) const { return items() + kItemBytes * id; }
  Block save;
  Block tables;
  Block specs;
  Block race;  // a race data block (read only by the cases so far)
};

// What a case writes before both runs and compares after them.
struct Spec {
  // Float fields of the save block (offsets), compared as floats.
  std::vector<uint32_t> save_floats;
  // Ranges below r1 (offsets from r1, negative) the function writes into its
  // frame, compared too.
  std::vector<std::pair<int32_t, uint32_t>> frame_ranges;
};

void RecordSave(Outputs& out, uint8_t* base, const World& w, const std::vector<uint32_t>& floats) {
  uint32_t at = 0;
  for (uint32_t offset : floats) {
    if (offset > at) {
      out.Bytes(Read(base, w.save.address + at, offset - at));
    }
    out.Float(guest::LoadU32(base, w.save.address + offset));
    at = offset + 4;
  }
  out.Bytes(Read(base, w.save.address + at, w.save.bytes - at));
}

// The item fields the functions test (+35, +36, +42) given small values,
// once per case from its generator.
void ShapeItems(uint8_t* base, const World& w, Random& r) {
  for (uint32_t id = 0; id < kItems; id++) {
    const uint32_t item = w.item(id);
    guest::StoreU8(base, item + 35, uint8_t(r.Below(8) == 0 ? 0xFF : r.Below(7)));
    guest::StoreU8(base, item + 36, uint8_t(r.Below(3)));
    guest::StoreU16(base, item + 42, uint16_t(r.Below(6)));
  }
}

// The item records' halfwords mostly 0, else small.
void ShapeRecords(uint8_t* base, const World& w, Random& r, uint32_t zero_in) {
  for (uint32_t id = 0; id < kItems; id++) {
    const uint32_t record = w.save.address + cr::kItemRecords + 12 * id;
    for (uint32_t k = 0; k < 6; k += 2) {
      guest::StoreU16(base, record + k, uint16_t(r.Below(zero_in) == 0 ? r.Below(1000) : 0));
    }
  }
}

using SetupFn = std::function<void(Random&, const World&)>;
using ArgsFn = std::function<void(PPCContext&, Random&, const World&)>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          const ArgsFn& args, GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kSaveSlots, 16},
                              {kRaceDataSlot, 4},
                              {data_tables::kTables, 4},
                              {car_specs::kParams, 4},
                              {cr::kPlayTimeFrames, 4},
                              {kRandSeed, 4}});
    PPCContext t = CallContext(h);
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, t.r1.u32, h.data - t.r1.u32, r);
    for (const auto& [offset, bytes] : spec.frame_ranges) {
      WriteRandom(h.base, t.r1.u32 + offset, bytes, r);
    }
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    WriteRandom(h.base, w.tables.address, 0x40, r);
    guest::StoreU32(h.base, w.tables.address + 8, kItems);
    guest::StoreU32(h.base, w.tables.address + 16, w.items());
    WriteRandom(h.base, w.specs.address, 0x40, r);
    guest::StoreU32(h.base, w.specs.address + 12, w.specs.address + 0x40);
    for (uint32_t p = 0; p < 4; p++) {
      guest::StoreU32(h.base, kSaveSlots + 4 * p, w.save.address);
    }
    guest::StoreU32(h.base, data_tables::kTables, w.tables.address);
    guest::StoreU32(h.base, w.tables.address + 20, w.race.address + kTable0);
    guest::StoreU32(h.base, w.tables.address + 56, w.race.address + kTable40);
    guest::StoreU32(h.base, kRaceDataSlot, w.race.address);
    guest::StoreU32(h.base, car_specs::kParams, w.specs.address);
    guest::StoreU32(h.base, cr::kPlayTimeFrames, uint32_t(r.Next()));
    setup(r, w);
    args(t, r, w);
    ITC_FLAG(native_career) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_career) = true;
    RecordSave(out[v], h.base, w, spec.save_floats);
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    const uint32_t r1 = CallContext(h).r1.u32;
    out[v].Bytes(Read(h.base, r1, h.data - r1));
    for (const auto& [offset, bytes] : spec.frame_ranges) {
      out[v].Bytes(Read(h.base, r1 + offset, bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           const ArgsFn& args, GuestFunction recompiled, GuestFunction native,
           const Spec& spec = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, args, recompiled, native, spec,
         fmt::format("case {}", c));
  }
  results.Report();
}

// A random pointer into the save block, 4-aligned, with `room` bytes after it.
uint32_t InSave(const World& w, Random& r, uint32_t room) {
  return w.save.address + 4 * r.Below((w.save.bytes - room) / 4);
}

// A small value now and then, else any.
uint64_t SmallOrAny(Random& r) {
  switch (r.Below(4)) {
    case 0:
      return r.Below(16);
    case 1:
      return uint64_t(-int64_t(r.Below(16)));
    case 2:
      return r.Below(20000000);
    default:
      return r.Next();
  }
}

void Accessors(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn none = [](Random&, const World&) {};
  const ArgsFn save_r3 = [](PPCContext& t, Random&, const World& w) {
    t.r3.u64 = w.save.address;
  };
  const ArgsFn block_r3 = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = InSave(w, r, 6100);
    t.r4.u64 = SmallOrAny(r);
  };
  Group(h, w, "PlayTime::Mark (sub_82111628)", 8, none, save_r3, __imp__sub_82111628,
        hooks::CareerPlayTimeMark);
  Group(h, w, "DayBlock::WinStreak (sub_82110580)", 8, none, block_r3, __imp__sub_82110580,
        hooks::CareerDayBlockWinStreak);
  Group(h, w, "DayBlock::AddWord4832 (sub_82110590)", 16, none, block_r3, __imp__sub_82110590,
        hooks::CareerDayBlockAddWord4832);
  Group(h, w, "DayBlock::Word4832 (sub_821105A0)", 8, none, block_r3, __imp__sub_821105A0,
        hooks::CareerDayBlockWord4832);
  Group(h, w, "Stats::WinStreak (sub_82110B10)", 8, none, block_r3, __imp__sub_82110B10,
        hooks::CareerStatsWinStreak);
  Group(
      h, w, "Stats::AddPoints (sub_82110B18)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = InSave(w, r, 6100);
        t.r4.u64 = SmallOrAny(r);
        if (r.Below(2) == 0) {
          // Near the cap.
          guest::StoreU32(base, t.r3.u32 + 6024, 9999990 + r.Below(20));
        }
      },
      __imp__sub_82110B18, hooks::CareerStatsAddPoints);
  Group(
      h, w, "ItemFlags::Set (sub_82112860)", 32, none,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address;
        t.r4.u64 = r.Below(4) == 0 ? SmallOrAny(r) : 0;
        t.r5.u64 = r.Below(8) == 0 ? uint64_t(-int64_t(r.Below(40))) : r.Below(400);
      },
      __imp__sub_82112860, hooks::CareerItemFlagsSet);
  Group(
      h, w, "Car::SpecWord744 (sub_821104B0)", 16, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = InSave(w, r, 64);
        guest::StoreU32(base, t.r3.u32 + 12, r.Below(4));
        WriteRandom(base, w.specs.address + 0x40, 4 * 8432, r);
      },
      __imp__sub_821104B0, hooks::CareerCarSpecWord744);
  // The garage: its free list and head as a list of up to 8 nodes over 8
  // slots, the selected car below the count.
  Group(
      h, w, "Career::PickFromCar (sub_82115CC0)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        const uint32_t save = w.save.address;
        const uint32_t garage = save + cr::kGarage;
        const uint32_t count = 1 + r.Below(8);
        uint32_t order[8] = {0, 1, 2, 3, 4, 5, 6, 7};
        for (uint32_t i = 7; i > 0; i--) {
          std::swap(order[i], order[r.Below(i + 1)]);
        }
        guest::StoreU16(base, garage + 204, uint16_t(order[0]));
        for (uint32_t i = 0; i < count; i++) {
          const uint32_t node = garage + 4 * order[i];
          guest::StoreU16(base, node, uint16_t(i + 1 < count ? order[i + 1] : 0xFFFF));
          guest::StoreU16(base, node + 2, uint16_t(r.Below(8)));
        }
        guest::StoreU32(base, save + cr::kSelectedCar, r.Below(count));
        t.r3.u64 = save;
      },
      __imp__sub_82115CC0, hooks::CareerPickFromCar);
  Group(
      h, w, "RunningMean::Merge (sub_82111CB0)", 64, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address + 4800;
        const uint32_t counts[] = {0, 1, 2, 7, 100, 0xFFFFFFFFu, 0xFFFFFFFEu, 0x80000000u};
        guest::StoreU32(base, t.r3.u32 + 4, counts[r.Below(8)]);
        if (r.Below(2) == 0) {
          guest::StoreFloat(base, t.r3.u32, float(int32_t(r.Below(2000)) - 1000) / 8.0f);
        }
        const float mean = float(int32_t(r.Below(2000)) - 1000) / 16.0f;
        const uint32_t mean_bits = r.Below(8) == 0 ? uint32_t(r.Next()) : std::bit_cast<uint32_t>(mean);
        t.r4.u64 = (uint64_t(mean_bits) << 32) | counts[r.Below(8)];
      },
      __imp__sub_82111CB0, hooks::CareerRunningMeanMerge, {.save_floats = {4800}});
  Group(
      h, w, "Stage::Of (sub_821121F8)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address;
        guest::StoreU32(base, w.save.address + 6092, (1u << r.Below(8)) - 1);
      },
      __imp__sub_821121F8, hooks::CareerStageOf);
  Group(
      h, w, "Entry56::Allows (sub_82111BA8)", 128, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = InSave(w, r, 64);
        guest::StoreU32(base, t.r3.u32, r.Below(4) == 0 ? 4 : r.Below(6));
        uint32_t mask = uint32_t(r.Next());
        if (r.Below(3) == 0) {
          mask &= ~0x380u;
        }
        if (r.Below(3) == 0) {
          mask &= ~0x7C00u;
        }
        guest::StoreU32(base, t.r3.u32 + 4, mask);
        const uint32_t locations[] = {48, 14, 15, 16, 17, 18, 19, 13, 20, 0, 0xFFFFFFFFu};
        t.r4.u64 = r.Below(5) == 4 ? uint64_t(-1) : r.Below(5);
        t.r5.u64 = locations[r.Below(11)];
        t.r6.u64 = r.Below(8) == 0 ? r.Below(40) : r.Below(5);
        t.r7.u64 = r.Below(2);
      },
      __imp__sub_82111BA8, hooks::CareerEntry56Allows);
  Group(
      h, w, "Items::AnyWonKind3Stage1 (sub_82111780)", 32,
      [base](Random& r, const World& w) {
        ShapeItems(base, w, r);
        ShapeRecords(base, w, r, 1 + r.Below(40));
      },
      [](PPCContext&, Random&, const World&) {}, __imp__sub_82111780,
      hooks::CareerAnyWonKind3Stage1);
}


// --- A race's outcome against an item ---

cr::Opponent ReadOpponent(uint8_t* base, uint32_t at) {
  return {guest::LoadU32(base, at), guest::LoadU32(base, at + 4), guest::LoadU32(base, at + 8)};
}

// The register steps of the functions without adapters (r4 = the Opponent
// in guest memory, as the originals read it).
void NativeCounterAddWin(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::CounterAddWin(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeCounterAddLoss(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::CounterAddLoss(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeCounterAddOutcome3(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::CounterAddOutcome3(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeWeekCounterAddLoss(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::WeekCounterAddLoss(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeOpponentsAddWin(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::OpponentsAddWin(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeOpponentsAddLoss(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::OpponentsAddLoss(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeOpponentsAddOutcome3(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::OpponentsAddOutcome3(base, t.r3.u32, ReadOpponent(base, t.r4.u32));
}
void NativeStatsAddWin(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::StatsAddWin(base, t.r3.u32, ReadOpponent(base, t.r4.u32), t.r5.u32);
}
void NativeStatsAddLoss(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::StatsAddLoss(base, t.r3.u32, ReadOpponent(base, t.r4.u32), t.r5.u32);
}
void NativeStatsAddOutcome3(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::StatsAddOutcome3(base, t.r3.u32, ReadOpponent(base, t.r4.u32), t.r5.u32);
}
void NativeItemRecordsAddOutcome3(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::ItemRecordsAddOutcome3(base, t.r3.u32, t.r4.u32, t.r1.u32);
}
void NativeResultRecordFill(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::ResultRecordFill(base, t.r3.u32, t.r4.u32);
}
void NativeDayLogPush(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = cr::DayLogPush(base, t.r3.u32, t.r4.u32);
}
void NativeGarageListInsert(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::GarageListInsert(base, t.r3.u32, t.r4.u32);
}

// A day block's opponents' list (stats +24 +24, count at +1624): up to 40
// ids, now and then the case's own.
void ShapeOpponentList(uint8_t* base, uint32_t day, uint32_t id, Random& r) {
  const uint32_t count = r.Below(4) == 0 ? 0 : r.Below(40);
  guest::StoreU32(base, day + 1624, count);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, day + 24 + 4 * i, r.Below(3) == 0 ? id : r.Below(400));
  }
}

// An Opponent in the scratch area (+0x800): an id, its record in the save
// block, its item; the record's halfwords 0 or not, the item's kind.
uint32_t WriteOpponent(uint8_t* base, Random& r, const World& w, uint32_t data) {
  const uint32_t id = r.Below(kItems);
  const uint32_t at = data + 0x800;
  const uint32_t record = w.save.address + cr::kItemRecords + 12 * id;
  guest::StoreU32(base, at, id);
  guest::StoreU32(base, at + 4, record);
  guest::StoreU32(base, at + 8, w.item(id));
  for (uint32_t k = 0; k < 6; k += 2) {
    guest::StoreU16(base, record + k, uint16_t(r.Below(2) == 0 ? 0 : r.Below(1200)));
  }
  const uint8_t kinds[] = {0, 1, 2, 3, 4, 5, 31, 32, 95, 0xFF, 0x80};
  guest::StoreU8(base, w.item(id) + 35, kinds[r.Below(11)]);
  return at;
}

// Halfwords near the caps now and then.
void NearCaps(uint8_t* base, Random& r, uint32_t address, uint32_t halfwords) {
  for (uint32_t i = 0; i < halfwords; i++) {
    const uint16_t values[] = {0, 1, 998, 999, 1000, 9998, 9999, 10000, 0x8000, 0xFFFF};
    if (r.Below(3) == 0) {
      guest::StoreU16(base, address + 2 * i, values[r.Below(10)]);
    }
  }
}

void Outcomes(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const uint32_t data = h.data;
  const SetupFn none = [](Random&, const World&) {};
  const ArgsFn counter_args = [base, data](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.save.address + 4 * r.Below(2000);
    NearCaps(base, r, t.r3.u32, 30);
    t.r4.u64 = WriteOpponent(base, r, w, data);
  };
  Group(h, w, "Counter::AddWin (sub_82111E10)", 48, none, counter_args, __imp__sub_82111E10,
        NativeCounterAddWin);
  Group(h, w, "Counter::AddLoss (sub_82111E80)", 48, none, counter_args, __imp__sub_82111E80,
        NativeCounterAddLoss);
  Group(h, w, "Counter::AddOutcome3 (sub_82111EF0)", 48, none, counter_args,
        __imp__sub_82111EF0, NativeCounterAddOutcome3);
  Group(h, w, "WeekCounter::AddLoss (sub_82111F60)", 48, none, counter_args,
        __imp__sub_82111F60, NativeWeekCounterAddLoss);
  const ArgsFn opponents_args = [base, data](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.save.address + cr::kStats + 24;
    NearCaps(base, r, t.r3.u32, 12);
    t.r4.u64 = WriteOpponent(base, r, w, data);
    const uint32_t id = guest::LoadU32(base, t.r4.u32);
    NearCaps(base, r, t.r3.u32 + 1628 + 8 * id, 3);
    ShapeOpponentList(base, t.r3.u32, id, r);
  };
  Group(h, w, "Opponents::AddWin (sub_821134D0)", 48, none, opponents_args, __imp__sub_821134D0,
        NativeOpponentsAddWin);
  Group(h, w, "Opponents::AddLoss (sub_82113580)", 48, none, opponents_args,
        __imp__sub_82113580, NativeOpponentsAddLoss);
  Group(h, w, "Opponents::AddOutcome3 (sub_82113628)", 48, none, opponents_args,
        __imp__sub_82113628, NativeOpponentsAddOutcome3);
  const auto shape_stats = [base](Random& r, uint32_t stats, uint32_t id, uint32_t mode) {
    NearCaps(base, r, stats, 12);
    NearCaps(base, r, stats + 24, 12);
    NearCaps(base, r, stats + 4852, 3);
    NearCaps(base, r, stats + 4864, 30);
    NearCaps(base, r, stats + 4924, 12);
    NearCaps(base, r, stats + 5968, 3);
    NearCaps(base, r, stats + 24 + 1628 + 8 * id, 3);
    NearCaps(base, r, stats + 5796 + 14 * mode, 7);
    ShapeOpponentList(base, stats + 24, id, r);
  };
  const ArgsFn stats_args = [base, data, shape_stats](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.save.address + cr::kStats;
    t.r4.u64 = WriteOpponent(base, r, w, data);
    t.r5.u64 = r.Below(5);
    shape_stats(r, t.r3.u32, guest::LoadU32(base, t.r4.u32), t.r5.u32);
  };
  Group(h, w, "Stats::AddWin (sub_821139B8)", 48, none, stats_args, __imp__sub_821139B8,
        NativeStatsAddWin);
  Group(h, w, "Stats::AddLoss (sub_82113AB8)", 48, none, stats_args, __imp__sub_82113AB8,
        NativeStatsAddLoss);
  Group(h, w, "Stats::AddOutcome3 (sub_82113B50)", 48, none, stats_args, __imp__sub_82113B50,
        NativeStatsAddOutcome3);
  const ArgsFn records_args = [base, data, shape_stats](PPCContext& t, Random& r,
                                                        const World& w) {
    const uint32_t save = w.save.address;
    t.r3.u64 = save;
    const uint32_t at = WriteOpponent(base, r, w, data);
    t.r4.u64 = guest::LoadU32(base, at);
    const uint32_t mode = r.Below(5);
    guest::StoreU32(base, save + 6104, mode);
    shape_stats(r, save + cr::kStats, t.r4.u32, mode);
    NearCaps(base, r, save + cr::kItemRecords + 12 * t.r4.u32, 3);
  };
  const Spec frame{.frame_ranges = {{-int32_t(cr::kItemRecordsFrame) + 80, 12}}};
  Group(h, w, "ItemRecords::AddWin (sub_82114B88)", 48, none, records_args, __imp__sub_82114B88,
        hooks::CareerItemRecordsAddWin, frame);
  Group(h, w, "ItemRecords::AddLoss (sub_82114B08)", 48, none, records_args,
        __imp__sub_82114B08, hooks::CareerItemRecordsAddLoss, frame);
  Group(h, w, "ItemRecords::AddOutcome3 (sub_82114A88)", 48, none, records_args,
        __imp__sub_82114A88, NativeItemRecordsAddOutcome3, frame);
}

// --- The last results, the day log ---

// The race data's last result record: the count 1 to 77 (none would read
// from address 0 + 812, unmapped, in both), its racer index small, the
// record random.
void ShapeResults(uint8_t* base, const World& w, Random& r) {
  const uint32_t count = 1 + r.Below(77);
  guest::StoreU32(base, w.race.address + kResultCount, count);
  if (count != 0) {
    const uint32_t last = w.race.address + kResults + 3936 * (count - 1);
    WriteRandom(base, last, 3936, r);
    guest::StoreU32(base, last + 4, r.Below(77));
    guest::StoreU32(base, last + 3924, r.Below(3));
  }
  WriteRandom(base, w.race.address, 64, r);
}

void LastResults(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn none = [](Random&, const World&) {};
  Group(
      h, w, "ResultRecord::Fill (sub_82110850)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        ShapeResults(base, w, r);
        t.r3.u64 = InSave(w, r, 64);
        t.r4.u64 = w.race.address;
      },
      __imp__sub_82110850, NativeResultRecordFill);
  Group(
      h, w, "Results::Push (sub_82111FC8)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        ShapeResults(base, w, r);
        t.r3.u64 = w.save.address + cr::kStats + 4948;
        const uint32_t counts[] = {0, 1, 14, 15, 16, 0xFFFFFFFFu};
        guest::StoreU32(base, t.r3.u32, counts[r.Below(6)]);
        t.r4.u64 = w.race.address;
      },
      __imp__sub_82111FC8, hooks::CareerResultsPush);
  const uint32_t stats = cr::kStats;
  Group(
      h, w, "DayLog::Push (sub_82112028)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address + cr::kStats + 5672;
        const uint32_t counts[] = {0, 1, 9, 10, 11, 0xFFFFFFFFu};
        guest::StoreU32(base, t.r3.u32, counts[r.Below(6)]);
        t.r4.u64 = w.save.address + cr::kStats + 24;
      },
      __imp__sub_82112028, NativeDayLogPush, {.save_floats = {stats + 5672 + 8}});
  Group(
      h, w, "Stats::StartDay (sub_82112088)", 32, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address + cr::kStats;
        t.r4.u64 = r.Below(4) == 0 ? 0 : r.Below(8);
        guest::StoreU32(base, t.r3.u32 + 5672, r.Below(12));
      },
      __imp__sub_82112088, hooks::CareerStatsStartDay, {.save_floats = {stats + 5672 + 8}});
}

// --- The garage's list ---

// 50 nodes: `count` linked from the head, the rest on the free list, each
// slot 0 to 49.
void ShapeGarage(uint8_t* base, uint32_t garage, Random& r, uint32_t count) {
  uint32_t order[50];
  for (uint32_t i = 0; i < 50; i++) {
    order[i] = i;
  }
  for (uint32_t i = 49; i > 0; i--) {
    std::swap(order[i], order[r.Below(i + 1)]);
  }
  const auto link = [&](uint32_t head_at, uint32_t from, uint32_t to) {
    guest::StoreU16(base, garage + head_at, uint16_t(from < to ? order[from] : 0xFFFF));
    for (uint32_t i = from; i < to; i++) {
      guest::StoreU16(base, garage + 4 * order[i], uint16_t(i + 1 < to ? order[i + 1] : 0xFFFF));
      guest::StoreU16(base, garage + 4 * order[i] + 2, uint16_t(r.Below(50)));
    }
  };
  link(204, 0, count);
  link(200, count, 50);
  guest::StoreU16(base, garage + 208, uint16_t(count));
}

void Garage(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const uint32_t data = h.data;
  const SetupFn none = [](Random&, const World&) {};
  Group(
      h, w, "GarageList::Insert (sub_82113468)", 48, none,
      [base](PPCContext& t, Random& r, const World& w) {
        const uint32_t garage = w.save.address + cr::kGarage;
        const uint32_t count = r.Below(49);
        ShapeGarage(base, garage, r, count);
        t.r3.u64 = garage;
        t.r4.u64 = r.Below(count + 1) | (r.Below(4) == 0 ? 0xABCD0000u : 0);
      },
      __imp__sub_82113468, NativeGarageListInsert);
  Group(
      h, w, "Garage::InsertCar (sub_821154E0)", 48, none,
      [base, data](PPCContext& t, Random& r, const World& w) {
        const uint32_t garage = w.save.address + cr::kGarage;
        const uint32_t count = r.Below(49);
        ShapeGarage(base, garage, r, count);
        t.r3.u64 = garage;
        t.r4.u64 = data + 16;
        t.r5.u64 = r.Below(count + 1);
      },
      __imp__sub_821154E0, hooks::CareerGarageInsertCar);
  // Car::SetModelAndClass and Garage::Add: the four specs at random, their
  // byte +708 0 to 9 (above 7, the frame's word the original reads unwritten:
  // filled at random above and below r1 alike for both runs).
  const auto specs = [base](Random& r, const World& w) {
    WriteRandom(base, w.specs.address + 0x40, 4 * 8432, r);
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t spec = w.specs.address + 0x40 + 8432 * i;
      guest::StoreU8(base, spec + 708, uint8_t(r.Below(10)));
    }
    guest::StoreU32(base, kRandSeed, uint32_t(r.Next()));
  };
  Group(
      h, w, "Car::SetModelAndClass (sub_821104D8)", 48, specs,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address + cr::kGarage + 212 + 1968 * r.Below(4);
        t.r4.u64 = r.Below(4);
        guest::StoreU32(base, t.r3.u32 + 1884, r.Below(3) == 0 ? 3 : r.Below(6));
      },
      __imp__sub_821104D8, hooks::CareerCarSetModelAndClass,
      {.frame_ranges = {{-int32_t(cr::kSetModelAndClassFrame) + 80, 4}}});
  Group(
      h, w, "Garage::Add (sub_82116CE0)", 48, specs,
      [base, data](PPCContext& t, Random& r, const World& w) {
        const uint32_t save = w.save.address;
        ShapeGarage(base, save + cr::kGarage, r, r.Below(49));
        t.r3.u64 = save;
        t.r4.u64 = data + 16;
        guest::StoreU32(base, data + 16 + 12, r.Below(4));
        guest::StoreU32(base, data + 16 + 1884, r.Below(3) == 0 ? 3 : r.Below(6));
        if (r.Below(3) == 0) {
          // The spec words +744 alike now and then (no +1900 write).
          for (uint32_t i = 0; i < 4; i++) {
            guest::StoreU32(base, w.specs.address + 0x40 + 8432 * i + 744, 7);
          }
        }
      },
      __imp__sub_82116CE0, hooks::CareerGarageAdd,
      {.frame_ranges = {{-int32_t(cr::kGarageAddFrame + cr::kSetModelAndClassFrame) + 80, 4}}});
}

// --- The unlocks' conditions and the rewards ---

void Unlocks(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const uint32_t data = h.data;
  Group(
      h, w, "Condition::Test (sub_82112950, with sub_821129E0)", 240,
      [base](Random& r, const World& w) {
        ShapeItems(base, w, r);
        ShapeRecords(base, w, r, 1 + r.Below(6));
      },
      [base, data](PPCContext& t, Random& r, const World& w) {
        const uint32_t save = w.save.address;
        const uint32_t kind = r.Below(12) == 0 ? 10 + r.Below(4) : r.Below(10);
        int32_t value;
        switch (kind) {
          case 3:
          case 4:
            value = int32_t(r.Below(9));
            break;
          case 5:
          case 6:
            value = int32_t(r.Below(190));
            break;
          case 7:
          case 8:
          case 9:
            value = int32_t(r.Below(r.Below(2) == 0 ? 8 : 200));
            break;
          default:
            value = int32_t(r.Below(kItems));
        }
        if (r.Below(10) == 0) {
          value = -1 - int32_t(r.Below(100));
        }
        guest::StoreU32(base, data + 0x900, kind);
        guest::StoreU32(base, data + 0x904, uint32_t(value));
        guest::StoreU32(base, save + 6100, r.Below(4) == 0 ? uint32_t(-int32_t(r.Below(30)))
                                                           : r.Below(2000));
        for (uint32_t i = 0; i < 192; i++) {
          if (r.Below(3) == 0) {
            guest::StoreU32(base, save + 0x1BE78 + 4 * i, r.Below(4));
            guest::StoreU32(base, save + 0x1C178 + 4 * i, r.Below(4));
          }
        }
        const uint32_t locations[] = {0xFFFFFFFFu, 0, 0, 1, 2, 4, 7, 8, 20, 0xFFFFFFFEu};
        t.r3.u64 = data + 0x900;
        t.r4.u64 = locations[r.Below(10)];
        uint32_t mask = uint32_t(r.Next());
        if (r.Below(2) == 0) {
          mask &= ~0x7C00u;
        }
        t.r5.u64 = mask;
      },
      __imp__sub_82112950, hooks::CareerConditionTest,
      {.frame_ranges = {{-int32_t(cr::kConditionTestFrame) + 80, 3008}}});
  Group(
      h, w, "Rewards::Grant (sub_82114C08, with sub_82114C7C)", 320,
      [](Random&, const World&) {},
      [base](PPCContext& t, Random& r, const World& w) {
        const uint32_t save = w.save.address;
        t.r3.u64 = save;
        t.r4.u64 = r.Below(8) == 0 ? uint64_t(-int64_t(1 + r.Below(4))) : r.Below(120);
        t.r5.u64 = r.Below(16) == 0 ? 42 + r.Below(4) : r.Below(42);
        t.r6.u64 = r.Below(6) == 0 ? uint64_t(-int64_t(1 + r.Below(500))) : r.Below(40);
        guest::StoreU32(base, save + 0x1D504, r.Below(60));
        if (r.Below(2) == 0) {
          // Most of the unlock sets' bits clear.
          for (uint32_t i = 0; i < 0x30; i += 4) {
            guest::StoreU32(base, save + 0x19924 + 4 * i,
                            guest::LoadU32(base, save + 0x19924 + 4 * i) & uint32_t(r.Next()) &
                                uint32_t(r.Next()));
          }
        }
      },
      __imp__sub_82114C08, hooks::CareerRewardsGrant);
}

// --- A race's result ---

// A float now and then 0, else a number of either sign.
uint32_t SomeFloat(Random& r) {
  if (r.Below(6) == 0) {
    return 0;
  }
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(200001)) - 100000) / 64.0f);
}

void NativeMergeTwo(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = cr::RunningMeanMergeTwo(base, t.r3.u32, t.r4.u64, t.r1.u32);
}

// The save data's running means and bests the result goes into (floats
// and small counts), and the race data's last result.
void ShapeRaceResult(uint8_t* base, const World& w, Random& r) {
  const uint32_t save = w.save.address;
  for (uint32_t at : {5928u, 5936u, 5944u, 5960u, 5968u, 4936u, 4940u, 5952u}) {
    guest::StoreU32(base, save + at, SomeFloat(r));
    guest::StoreU32(base, save + at + 4, r.Below(4) == 0 ? 0 : r.Below(100));
  }
  for (uint32_t at : {6024u, 6028u, 6032u, 6036u}) {
    guest::StoreU32(base, save + at, SomeFloat(r));
  }
  guest::StoreU32(base, save + 6104, r.Below(5));
  guest::StoreU32(base, save + 4996, r.Below(16));
  ShapeResults(base, w, r);
  const uint32_t race = w.race.address;
  const uint32_t modes[] = {2, 3, 2, 3, 0, 1, 4, 0xFFFFFFFFu};
  guest::StoreU32(base, race + 0xA3AF4, modes[r.Below(8)]);
  const uint32_t result =
      race + kResults + 3936 * (guest::LoadU32(base, race + kResultCount) - 1);
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, result + 8 + 972 * k, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(kItems));
  }
  guest::StoreU32(base, result + 3924, r.Below(4));
  guest::StoreU32(base, result + 3920, r.Below(5));
  guest::StoreU32(base, result + 784, r.Below(2));
  for (uint32_t at : {788u, 808u, 812u, 816u}) {
    guest::StoreU32(base, result + at,
                    r.Below(5) == 0 ? uint32_t(-int32_t(r.Below(50))) : r.Below(5000));
  }
  guest::StoreU32(base, result + 796, SomeFloat(r));
  guest::StoreU32(base, result + 800, r.Below(4) == 0 ? 0 : r.Below(20));
  for (uint32_t at : {804u, 820u, 828u}) {
    guest::StoreU32(base, result + at, SomeFloat(r));
  }
  // The stage list (+0x59620): the current stage (+804) and its type.
  const uint32_t stages = race + 0x59620;
  const uint32_t current = r.Below(10);
  guest::StoreU32(base, stages + 804, current);
  guest::StoreU32(base, stages + 80 * current, r.Below(3) == 0 ? 1 : 0);
  ShapeItems(base, w, r);
}

void RaceResult(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn none = [](Random&, const World&) {};
  Group(
      h, w, "RunningMean::MergeTwo (sub_82112148)", 48, none,
      [base](PPCContext& t, Random& r, const World& w) {
        const uint32_t stats = w.save.address + cr::kStats;
        for (uint32_t at : {4892u, 5904u}) {
          guest::StoreU32(base, stats + at, SomeFloat(r));
          guest::StoreU32(base, stats + at + 4, r.Below(4) == 0 ? 0 : r.Below(100));
        }
        t.r3.u64 = stats;
        t.r4.u64 = (uint64_t(SomeFloat(r)) << 32) | (r.Below(4) == 0 ? 0 : r.Below(100));
      },
      __imp__sub_82112148, NativeMergeTwo,
      {.save_floats = {48 + 4892, 48 + 5904},
       .frame_ranges = {{-int32_t(cr::kMergeTwoFrame) + 24, 8}}});
  const int32_t frame = -int32_t(cr::kAddRaceResultFrame);
  Group(
      h, w, "Stats::AddRaceResult (sub_82115190)", 160,
      [base](Random& r, const World& w) { ShapeRaceResult(base, w, r); },
      [](PPCContext& t, Random&, const World& w) {
        t.r3.u64 = w.save.address;
        t.r4.u64 = w.race.address;
      },
      __imp__sub_82115190, hooks::CareerAddRaceResult,
      {.save_floats = {4936, 4940, 5928, 5936, 5944, 5952, 5960, 5968, 6024, 6028, 6032, 6036},
       .frame_ranges = {{frame + 24, 8},
                        {frame + 80, 16},
                        {frame + 96, 3008},
                        {frame - int32_t(cr::kMergeTwoFrame) + 24, 8},
                        {frame - int32_t(cr::kItemRecordsFrame) + 80, 12}}});
  Group(
      h, w, "Career::PickNext (sub_821159B0, its gate)", 48, none,
      [base](PPCContext& t, Random& r, const World& w) {
        const uint32_t save = w.save.address;
        t.r3.u64 = save;
        t.r4.u64 = r.Below(2);
        if (r.Below(2) == 0) {
          guest::StoreU16(base, save + 70, uint16_t(int16_t(int32_t(r.Below(20)) - 5)));
        } else {
          guest::StoreU16(base, save + 70, uint16_t(15 + r.Below(500)));
          guest::StoreU32(base, save + 6100, uint32_t(2 - int32_t(r.Below(1000))));
        }
      },
      __imp__sub_821159B0, hooks::CareerPickNext);
}

// --- The progress after a race, the day ---

void NativeAnyWonKind3Stage2(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::AnyWonKind3Stage2(base);
}
void NativeAllWonKind3(PPCContext& t, uint8_t* base) { t.r3.u64 = cr::AllWonKind3(base); }
void NativeAllWonKind4Stage2(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::AllWonKind4Stage2(base);
}
void NativeAllWonKind4Stage3(PPCContext& t, uint8_t* base) {
  t.r3.u64 = cr::AllWonKind4Stage3(base);
}
void NativeAllWonKind5(PPCContext& t, uint8_t* base) { t.r3.u64 = cr::AllWonKind5(base); }
void NativeLocationsUnlock(PPCContext& t, uint8_t* base) {
  cr::LocationsUnlock(base, t.r3.u32, t.r4.u32);
}
void NativeLocationsUnlock2(PPCContext& t, uint8_t* base) {
  cr::LocationsUnlock2(base, t.r3.u32, t.r4.u32);
}
void NativeCloseDay(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = cr::StatsCloseDay(base, t.r3.u32, t.r1.u32);
}

// The day's sums CloseDay divides and means (small, a divisor of 0 and the
// overflowing division now and then).
void ShapeDaySums(uint8_t* base, uint32_t stats, Random& r) {
  for (uint32_t at : {5928u, 5936u, 5944u}) {
    guest::StoreU32(base, stats + at, SomeFloat(r));
    guest::StoreU32(base, stats + at + 4, r.Below(4) == 0 ? 0 : r.Below(100));
  }
  guest::StoreU32(base, stats + 5952, r.Below(500));
  const uint32_t divisors[] = {1, 2, 7, 30, 0, 0xFFFFFFFFu};
  guest::StoreU32(base, stats + 5956, divisors[r.Below(6)]);
  if (r.Below(8) == 0) {
    // x 100 wraps to INT_MIN, over -1: the division's other trap.
    guest::StoreU32(base, stats + 5952, 0x20000000u);
    guest::StoreU32(base, stats + 5956, 0xFFFFFFFFu);
  }
  guest::StoreU32(base, stats + 5960, r.Below(4) == 0 ? uint32_t(-int32_t(r.Below(9))) : r.Below(99999));
  guest::StoreU32(base, stats + 5964, r.Below(99999));
  guest::StoreU16(base, stats + 6028, uint16_t(r.Below(2) == 0 ? 9998 + r.Below(3) : r.Below(100)));
}

// A table-0 entry or a 40-byte entry's three conditions.
void WriteConditions(uint8_t* base, uint32_t at, Random& r, bool first_valid) {
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t c = at + 8 * k;
    const uint32_t kinds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFFFFFFFFu, 0xFFFFFFFFu};
    uint32_t kind = kinds[r.Below(12)];
    if (k == 0 && first_valid && kind == 0xFFFFFFFFu) {
      kind = r.Below(10);
    }
    guest::StoreU32(base, c, kind);
    uint32_t value = kind >= 7 && kind <= 9 ? r.Below(6) : r.Below(kind == 5 || kind == 6 ? 60 : 9);
    if (r.Below(10) == 0) {
      value = 0xFFFFFFFFu;
    }
    guest::StoreU32(base, c + 4, value);
  }
}

void ShapeAfterRace(uint8_t* base, const World& w, Random& r) {
  const uint32_t save = w.save.address;
  const uint32_t race = w.race.address;
  ShapeItems(base, w, r);
  ShapeRecords(base, w, r, 1 + r.Below(3));
  WriteRandom(base, race + 0xA3AD0, 16, r);
  guest::StoreU32(base, race + 0x59AE4, uint32_t(r.Next()));
  const uint32_t locations[] = {48, 48, 14, 16, 19, 20, 5};
  guest::StoreU32(base, save + 6112, locations[r.Below(7)]);
  guest::StoreU32(base, save + 6108, locations[r.Below(7)]);
  guest::StoreU32(base, save + 6104, r.Below(4));
  guest::StoreU32(base, save + 6092, (1u << r.Below(8)) - 1);
  guest::StoreU32(base, save + 6100, r.Below(50));
  for (uint32_t i = 0; i < 2; i++) {
    guest::StoreU32(base, save + 0x1CA7C + 4 * i, uint32_t(r.Next()) & uint32_t(r.Next()));
    guest::StoreU32(base, save + 0x1D508 + 4 * i, uint32_t(r.Next()) & uint32_t(r.Next()));
  }
  guest::StoreU32(base, save + 0x1CA78, r.Below(20));
  guest::StoreU32(base, save + 0x1D504, r.Below(20));
  // Table 0: up to 40 entries, then the end (-1).
  const uint32_t entries0 = r.Below(40);
  for (uint32_t i = 0; i < entries0; i++) {
    const uint32_t e = race + kTable0 + 56 * i;
    guest::StoreU32(base, e + 0, r.Below(5));
    guest::StoreU32(base, e + 4, uint32_t(r.Next()) | (r.Below(2) == 0 ? 0 : 0x7FF0u));
    guest::StoreU32(base, e + 20, r.Below(2));
    WriteConditions(base, e + 24, r, false);
  }
  guest::StoreU32(base, race + kTable0 + 56 * entries0, 0xFFFFFFFFu);
  // The 40-byte table: up to 40 entries, then the end (+16 = -1).
  const uint32_t entries40 = r.Below(40);
  for (uint32_t i = 0; i < entries40; i++) {
    const uint32_t e = race + kTable40 + 40 * i;
    guest::StoreU32(base, e + 0, uint32_t(r.Next()));
    guest::StoreU32(base, e + 4, r.Below(42));
    guest::StoreU32(base, e + 8, r.Below(40));
    guest::StoreU32(base, e + 12, r.Below(2));
    WriteConditions(base, e + 16, r, true);
  }
  guest::StoreU32(base, race + kTable40 + 40 * entries40 + 16, 0xFFFFFFFFu);
}

void Progress(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const ArgsFn no_args = [](PPCContext&, Random&, const World&) {};
  const SetupFn some_won = [base](Random& r, const World& w) {
    ShapeItems(base, w, r);
    const uint32_t zero_in[] = {1, 1, 2, 60};
    ShapeRecords(base, w, r, zero_in[r.Below(4)]);
  };
  Group(h, w, "Items::AnyWonKind3Stage2 (sub_82111818)", 32, some_won, no_args,
        __imp__sub_82111818, NativeAnyWonKind3Stage2);
  Group(h, w, "Items::AllWonKind3 (sub_821118B0)", 32, some_won, no_args, __imp__sub_821118B0,
        NativeAllWonKind3);
  Group(h, w, "Items::AllWonKind4Stage2 (sub_82111940)", 32, some_won, no_args,
        __imp__sub_82111940, NativeAllWonKind4Stage2);
  Group(h, w, "Items::AllWonKind4Stage3 (sub_821119D0)", 32, some_won, no_args,
        __imp__sub_821119D0, NativeAllWonKind4Stage3);
  Group(h, w, "Items::AllWonKind5 (sub_82111A60)", 32, some_won, no_args, __imp__sub_82111A60,
        NativeAllWonKind5);
  const SetupFn none = [](Random&, const World&) {};
  const ArgsFn bit_args = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.save.address;
    t.r4.u64 = r.Below(8) == 0 ? uint64_t(-int64_t(1 + r.Below(8))) : r.Below(64);
  };
  Group(h, w, "Locations::Unlock (sub_82111D20)", 32, none, bit_args, __imp__sub_82111D20,
        NativeLocationsUnlock);
  Group(h, w, "Locations::Unlock2 (sub_82111D98)", 32, none, bit_args, __imp__sub_82111D98,
        NativeLocationsUnlock2);
  const int32_t close = -int32_t(cr::kCloseDayFrame);
  Group(
      h, w, "Stats::CloseDay (sub_82113898)", 64, none,
      [base](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.save.address + cr::kStats;
        ShapeDaySums(base, t.r3.u32, r);
      },
      __imp__sub_82113898, NativeCloseDay,
      {.save_floats = {48 + 5928, 48 + 5936, 48 + 5944},
       .frame_ranges = {{close + 24, 8}, {close + 80, 16}}});
  const int32_t next_close = -int32_t(cr::kNextDayFrame) + close;
  Group(
      h, w, "Career::NextDay (sub_82116AF0)", 64, none,
      [base](PPCContext& t, Random& r, const World& w) {
        const uint32_t save = w.save.address;
        t.r3.u64 = save;
        ShapeGarage(base, save + cr::kGarage, r, r.Below(20));
        ShapeDaySums(base, save + cr::kStats, r);
        guest::StoreU32(base, save + 6100, r.Below(30));
        // Career::PickNext's gate holds (past it, never seen in play).
        guest::StoreU16(base, save + 70, uint16_t(r.Below(15)));
        guest::StoreU32(base, save + cr::kStats + 5672, r.Below(12));
      },
      __imp__sub_82116AF0, hooks::CareerNextDay,
      // Stats::CloseDay's running mean at its frame's +80 is left out: the
      // original's later callees (Career::PickNext, Stats::StartDay) save
      // their registers over it, which native code doesn't do.
      {.save_floats = {48 + 5672 + 8, 48 + 5928, 48 + 5936, 48 + 5944},
       .frame_ranges = {{next_close + 24, 8}}});
  Group(
      h, w, "Progress::AfterRace (sub_82116588)", 160,
      [base](Random& r, const World& w) { ShapeAfterRace(base, w, r); },
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.save.address; },
      __imp__sub_82116588, hooks::CareerAfterRace,
      // Condition::Test's last item copy, without the words at r1 - 272 and
      // r1 - 256: the back chains the original's stage tests (112-byte
      // frames) and Stage::Of (96 bytes) store there below
      // Progress::AfterRace's frame, which native code doesn't.
      {.frame_ranges = {{-int32_t(cr::kAfterRaceFrame) - int32_t(cr::kConditionTestFrame) + 80,
                         2976},
                        {-272 + 4, 12},
                        {-256 + 4, 12}}});
}

}  // namespace

void Career(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: career: no block from the allocation layer");
    return;
  }
  {
    // The items once (the cases shape the fields they test).
    Random r(0xCA4EE4);
    WriteRandom(h.base, w.tables.address, w.tables.bytes, r);
  }
  {
    Random r(0xCA4EE5);
    WriteRandom(h.base, w.race.address, w.race.bytes, r);
  }
  Accessors(h, w);
  Outcomes(h, w);
  LastResults(h, w);
  Garage(h, w);
  Unlocks(h, w);
  RaceResult(h, w);
  Progress(h, w);
}

}  // namespace harness
