// The career's location screen (src/location_screen/,
// docs/functions/location-screen.md) against the recompiled functions. Each
// case fills a world of our own with random words from the case's seed: a
// screen block (the screen, its page object and four menus, pointed at by
// 0x82814E48-0x82814E5C), a save data block (the players' slots pointed at
// it), the data tables' object (its items, 56-byte and 20-byte tables), a
// race data block (its racer records) and a race event (its groups), plus
// the scratch area, the caller's area above r1 and the frames the functions
// write; shapes the fields the function tests; runs the original (with
// --native_location_screen off) or the register steps
// (hooks/location_screen.h); and compares the blocks, the frames and r3 (all
// 64 bits). The globals the cases point elsewhere are put back after each
// run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "hooks/location_screen.h"
#include "location_screen/location_screen.h"
#include "memory/heap.h"
#include "race_event/race_event.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_location_screen);

namespace harness {

namespace {

namespace ls = location_screen;

constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kItems = 400;
constexpr uint32_t kItemBytes = 3008;
constexpr uint32_t kTables56 = 8;        // 56-byte tables
constexpr uint32_t kEntries56 = 64;      // entries in each
constexpr uint32_t kEntries20 = 256;     // the 20-byte table's entries
constexpr uint32_t kRecordBytes = 3936;  // a racer record
constexpr uint32_t kRecordCount = 0x49FE0;
constexpr uint32_t kRaceBytes = ls::kRacerRecords + kRecordCount + 16;
constexpr uint32_t kScreenBytes = 0x1000;  // the screen (912 bytes), then the rest
constexpr uint32_t kEventBytes = 0x400;    // the event (160 bytes), then its lists

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

// The screen block's parts: the screen at +0, the page object, the menus.
constexpr uint32_t kPagesAt = 0x400, kMenuAt = 0x440, kListMenuAt = 0x490,
                   kOpponentMenuAt = 0x4E0, kConfirmMenuAt = 0x530;
// The pictures (16 bytes each, then 12-byte texture records) at +0x600.
constexpr uint32_t kPicturesAt = 0x600;

struct World {
  World()
      : save(kSaveDataBytes),
        tables(0x40 + kItems * kItemBytes + kTables56 * kEntries56 * 56 + kEntries20 * 20),
        race(kRaceBytes),
        screen(kScreenBytes),
        event(kEventBytes) {}
  bool ok() const {
    return save.address && tables.address && race.address && screen.address && event.address;
  }
  uint32_t items() const { return tables.address + 0x40; }
  uint32_t item(uint32_t id) const { return items() + kItemBytes * id; }
  uint32_t table56(uint32_t t) const {
    return items() + kItems * kItemBytes + kEntries56 * 56 * t;
  }
  uint32_t table20() const { return table56(kTables56); }
  uint32_t records() const { return race.address + ls::kRacerRecords; }
  Block save;
  Block tables;
  Block race;
  Block screen;
  Block event;
};

struct Spec {
  // Ranges below r1 (offsets from r1, negative) the function writes into its
  // frame, compared too.
  std::vector<std::pair<int32_t, uint32_t>> frame_ranges;
};

using SetupFn = std::function<void(Random&, const World&)>;
using ArgsFn = std::function<void(PPCContext&, Random&, const World&)>;

// The world's fields every case shapes: the tables' object, the pointers.
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.tables.address, 0x40, r);
  guest::StoreU32(base, w.tables.address + 8, kItems);
  guest::StoreU32(base, w.tables.address + 16, w.items());
  for (uint32_t t = 0; t < kTables56; t++) {
    guest::StoreU32(base, w.tables.address + 4 * (t + 5), w.table56(t));
  }
  guest::StoreU32(base, w.tables.address + 52, w.table20());
  // The 20-byte table: +0 small (the keys tested), +8 an item id, +4 the
  // next (0 often); the last entry ends it.
  for (uint32_t i = 0; i < kEntries20; i++) {
    const uint32_t entry = w.table20() + 20 * i;
    guest::StoreU32(base, entry + 0, i + 1 == kEntries20 ? 0xFFFFFFFFu : r.Below(14));
    guest::StoreU32(base, entry + 8, r.Below(8));
    guest::StoreU32(base, entry + 4, r.Below(3) == 0 ? 0 : r.Below(5));
  }
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, w.save.address);
  }
  guest::StoreU32(base, data_tables::kTables, w.tables.address);
  guest::StoreU32(base, ls::kRaceData, w.race.address);
  guest::StoreU32(base, race_event::kEvent, w.event.address);
  guest::StoreU32(base, ls::kPages, w.screen.address + kPagesAt);
  guest::StoreU32(base, ls::kMenu, w.screen.address + kMenuAt);
  guest::StoreU32(base, ls::kScreen, w.screen.address);
  guest::StoreU32(base, ls::kListMenu, w.screen.address + kListMenuAt);
  guest::StoreU32(base, ls::kOpponentMenu, w.screen.address + kOpponentMenuAt);
  guest::StoreU32(base, ls::kConfirmMenu, w.screen.address + kConfirmMenuAt);
  // The racer records' count: 0 to 4.
  guest::StoreU32(base, w.records() + kRecordCount, r.Below(5));
  // The event's groups (kinds 4 to 9 at +84 + 12 (kind - 4)): a count of 0
  // to 6 and a list of item ids (halfwords every 4 bytes) after the event.
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t group = w.event.address + 84 + 12 * k;
    const uint32_t list = w.event.address + 0x100 + 0x20 * k;
    guest::StoreU16(base, group, uint16_t(r.Below(7)));
    guest::StoreU32(base, group + 4, list);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU16(base, list + 4 * i, uint16_t(r.Below(kItems)));
    }
  }
}

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          const ArgsFn& args, GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kSaveSlots, 16},
                              {data_tables::kTables, 4},
                              {ls::kRaceData, 4},
                              {race_event::kEvent, 4},
                              {ls::kPages, 24}});
    PPCContext t = CallContext(h);
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, t.r1.u32, h.data - t.r1.u32, r);
    for (const auto& [offset, bytes] : spec.frame_ranges) {
      WriteRandom(h.base, t.r1.u32 + offset, bytes, r);
    }
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    WriteRandom(h.base, w.screen.address, w.screen.bytes, r);
    WriteRandom(h.base, w.event.address, w.event.bytes, r);
    WriteRandom(h.base, w.records(), 4 * kRecordBytes, r);
    ShapeWorld(h.base, w, r);
    setup(r, w);
    args(t, r, w);
    ITC_FLAG(native_location_screen) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_location_screen) = true;
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, w.screen.address, w.screen.bytes));
    out[v].Bytes(Read(h.base, w.event.address, w.event.bytes));
    out[v].Bytes(Read(h.base, w.records(), 4 * kRecordBytes));
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

// A small value now and then, else any.
uint64_t SmallOrAny(Random& r) {
  switch (r.Below(4)) {
    case 0:
      return r.Below(16);
    case 1:
      return uint64_t(-int64_t(r.Below(16)));
    case 2:
      return r.Below(20000);
    default:
      return r.Next();
  }
}

// The two picture lists: up to 6 pictures each (16 bytes, +4 the id 0 to 9,
// +8 a texture record), the defaults random.
void ShapePictures(uint8_t* base, const World& w, Random& r) {
  const uint32_t screen = w.screen.address;
  for (uint32_t list = 0; list < 2; list++) {
    const uint32_t count = r.Below(7);
    guest::StoreU32(base, screen + (list == 0 ? 136 : 404), r.Below(16) == 0 ? 0 : count);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t picture = screen + kPicturesAt + 0x100 * list + 16 * i;
      guest::StoreU32(base, screen + (list == 0 ? 8 : 276) + 4 * i, picture);
      guest::StoreU32(base, picture + 4, r.Below(10));
    }
  }
  // Ring A: 32 ids from +140, +268 of them from +272 (any start).
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, screen + 140 + 4 * i, r.Below(40));
  }
  guest::StoreU32(base, screen + 268, r.Below(33));
  guest::StoreU32(base, screen + 272, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(32));
}

void Helpers(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn none = [](Random&, const World&) {};
  Group(h, w, "LocationScreen::SceneryConfig (sub_8228F0A0)", 200, none,
        [](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = SmallOrAny(r);
          t.r4.u64 = SmallOrAny(r);
        },
        __imp__sub_8228F0A0, hooks::LocationScreenSceneryConfig);
  Group(h, w, "LocationScreen::KindText (sub_82227690)", 200, none,
        [](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = r.Below(4) == 0 ? SmallOrAny(r) : r.Below(9);
          t.r4.u64 = SmallOrAny(r);
        },
        __imp__sub_82227690, hooks::LocationScreenKindText);
  const SetupFn pictures = [base](Random& r, const World& w) { ShapePictures(base, w, r); };
  const ArgsFn id = [](PPCContext& t, Random& r, const World&) {
    t.r3.u64 = r.Below(8) == 0 ? r.Next() : r.Below(40);
  };
  Group(h, w, "LocationScreen::IsPictureQueued (sub_822291E8)", 300, pictures, id,
        __imp__sub_822291E8, hooks::LocationScreenIsPictureQueued);
  Group(h, w, "LocationScreen::PictureA (sub_82229108)", 300, pictures, id, __imp__sub_82229108,
        hooks::LocationScreenPictureA);
  Group(h, w, "LocationScreen::PictureB (sub_82229158)", 300, pictures, id, __imp__sub_82229158,
        hooks::LocationScreenPictureB);
  Group(h, w, "LocationScreen::OnMessage14 (sub_82227158)", 200,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + kMenuAt,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(6));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227158,
        hooks::LocationScreenOnMessage14);
}

void Lists(Context& h, const World& w) {
  uint8_t* const base = h.base;
  Group(h, w, "LocationScreen::HasMoreUnlocks (sub_822281D8)", 200,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.save.address + ls::kUnlockCount,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8));
          guest::StoreU32(base, w.screen.address + 868,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_822281D8,
        hooks::LocationScreenHasMoreUnlocks);
  Group(h, w, "LocationScreen::NextUnlockEntry (sub_82227618)", 300,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 876, r.Below(kTables56));
          guest::StoreU32(base, w.screen.address + 872, r.Below(kEntries56 - 8));
          for (uint32_t t = 0; t < kTables56; t++) {
            for (uint32_t i = 0; i < kEntries56; i++) {
              const uint32_t next = r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next())
                                                                         : r.Below(5);
              guest::StoreU32(base, w.table56(t) + 56 * i + 8, next);
            }
          }
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227618,
        hooks::LocationScreenNextUnlockEntry);
  // The item's halfword +88 (4, 5 or another).
  const Spec item_frame{{{-int32_t(ls::kItemFrame) + 80, 3024}}};
  Group(h, w, "LocationScreen::OpponentHasCard (sub_82227708)", 300,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 804,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : 3 + r.Below(8));
          guest::StoreU32(base, w.screen.address + 800,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(8) - 1);
          for (uint32_t id = 0; id < kItems; id++) {
            guest::StoreU16(base, w.item(id) + 88, uint16_t(r.Below(4) == 0 ? r.Next() : 3 + r.Below(4)));
          }
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227708,
        hooks::LocationScreenOpponentHasCard, item_frame);
  // Racer 0's item (+980), the human racer's (the last record's) +980 and
  // +3924, the item's +88 (3 or not), the item records' halfword +2, the
  // 20-byte entries' keys (the case's +872 an offset from the found one).
  Group(h, w, "LocationScreen::NextResultText (sub_822271F8)", 400,
        [base](Random& r, const World& w) {
          for (uint32_t k = 0; k < 4; k++) {
            const uint32_t record = w.records() + kRecordBytes * k;
            guest::StoreU32(base, record + 980, r.Below(8));
            guest::StoreU32(base, record + 3924, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2) + 1);
          }
          for (uint32_t id = 0; id < 8; id++) {
            guest::StoreU16(base, w.item(id) + 88, uint16_t(r.Below(2) == 0 ? 3 : r.Below(6)));
            guest::StoreU16(base, w.save.address + ls::kItemRecords + 12 * id + 2,
                            uint16_t(r.Below(3)));
          }
          guest::StoreU32(base, w.screen.address + 872, r.Below(4));
          // At least one record so racer 0 is there; the human's may be
          // record 0 too.
          guest::StoreU32(base, w.records() + kRecordCount, 1 + r.Below(4));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_822271F8,
        hooks::LocationScreenNextResultText, {{{-int32_t(ls::kItemFrame) + 80, 3008}}});
}

}  // namespace

void LocationScreen(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: location_screen: no block from the allocation layer");
    return;
  }
  {
    Random r(0x10CA7E);
    WriteRandom(h.base, w.tables.address, w.tables.bytes, r);
  }
  Helpers(h, w);
  Lists(h, w);
}

}  // namespace harness
