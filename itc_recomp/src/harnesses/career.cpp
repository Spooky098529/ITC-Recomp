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
  World() : save(kSaveDataBytes), tables(0x40 + kItems * kItemBytes), specs(0x40 + 4 * 8432) {}
  bool ok() const { return save.address && tables.address && specs.address; }
  uint32_t items() const { return tables.address + 0x40; }
  uint32_t item(uint32_t id) const { return items() + kItemBytes * id; }
  Block save;
  Block tables;
  Block specs;
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
                              {data_tables::kTables, 4},
                              {car_specs::kParams, 4},
                              {cr::kPlayTimeFrames, 4}});
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
  Accessors(h, w);
}

}  // namespace harness
