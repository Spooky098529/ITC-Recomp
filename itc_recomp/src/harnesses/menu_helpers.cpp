// The front end's menu helpers (src/menu_helpers/,
// docs/functions/menu-helpers.md) against the recompiled functions. Each case
// fills a world of our own with random words from the case's seed (a menu,
// the four players' action mappers with their vtables), plus the stack below
// the call's stack pointer and the caller's area above it; shapes the fields
// the helpers test (the cursor around the first and last items, the wrap and
// the three step switches, the player, the mapper entries and the mappers'
// pressed and fired bits, the vtables' slots 10 and 12 now and then
// ActionMapper::Held or swapped); runs the original or the register steps
// (hooks/menu_helpers.h); and compares the world, the mapper and controller
// entries, the stack below the original's register saves, the caller's area,
// r1 and r3, all exactly (nothing here is a float). The mapper slots run as
// their adapters (--native_action_mappers as set) on both sides, so neither
// leaves a slot frame. The globals the cases point elsewhere are put back
// after each run.

#include "harness.h"

#include "base/log.h"
#include "hooks/menu_helpers.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace mh = menu_helpers;

constexpr uint32_t kMiscBytes = 0x1000;
constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kHeldSlot = 0x8210BE70;

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

struct World {
  World() : misc(kMiscBytes) {}
  bool ok() const { return misc.address != 0; }
  uint32_t menu() const { return misc.address; }
  uint32_t vtable(uint32_t i) const { return misc.address + 0x100 + 0x40 * i; }
  uint32_t mapper(uint32_t i) const { return misc.address + 0x200 + 0x200 * i; }
  Block misc;  // the menu (+0, 68 bytes), the mappers' vtables (+0x100) and the mappers (+0x200)
};

// A small signed word in [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// 0 half the time, else 1 or any word.
uint32_t Switch(Random& r) {
  if (r.Below(2) == 0) {
    return 0;
  }
  return r.Below(2) == 0 ? 1 : uint32_t(r.Next());
}

// A read slot: mostly the expected one, now and then Held or the other.
uint32_t Slot(Random& r, uint32_t expected) {
  switch (r.Below(10)) {
    case 0:
      return kHeldSlot;
    case 1:
      return expected == mh::kPressedSlot ? mh::kFiredSlot : mh::kPressedSlot;
    default:
      return expected;
  }
}

void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  const uint32_t menu = w.menu();
  guest::StoreU32(base, menu + mh::kPlayer, r.Below(4));
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(base, mh::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, mh::kMapperEntries + 8 * c, w.mapper(c));
    guest::StoreU32(base, mh::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, w.mapper(c), w.vtable(c));
    guest::StoreU32(base, w.vtable(c) + mh::kPressedOffset, Slot(r, mh::kPressedSlot));
    guest::StoreU32(base, w.vtable(c) + mh::kFiredOffset, Slot(r, mh::kFiredSlot));
    // The mappers' words (+448 to +471): none, few or many bits set, so the
    // tries answer both ways and the first fired varies.
    const uint32_t density = r.Below(4);
    for (uint32_t k = 0; k < 6; k++) {
      uint32_t bits = 0;
      if (density != 0) {
        bits = uint32_t(r.Next());
        for (uint32_t d = 1; d < density + 1; d++) {
          bits &= uint32_t(r.Next());
        }
      }
      guest::StoreU32(base, w.mapper(c) + 448 + 4 * k, bits);
    }
  }
  const int32_t first = int32_t(r.Below(5));
  const int32_t last = r.Below(8) == 0 ? first - int32_t(r.Below(3)) : first + int32_t(r.Below(9));
  guest::StoreU32(base, menu + mh::kFirst, Around(r, first, first));
  guest::StoreU32(base, menu + mh::kLast, Around(r, last, last));
  guest::StoreU32(base, menu + mh::kCursor, Around(r, first - 3, last + 3));
  guest::StoreU32(base, menu + mh::kWraps, Switch(r));
  guest::StoreU32(base, menu + mh::kUpDownLocked, Switch(r));
  guest::StoreU32(base, menu + mh::kSidesStep, Switch(r));
  guest::StoreU32(base, menu + mh::kShouldersStep, Switch(r));
  // For Menu::Pressed: the last step's move and side mostly 0.
  guest::StoreU32(base, menu + mh::kMoved, r.Below(3) == 0 ? Around(r, -1, 1) : 0);
  guest::StoreU32(base, menu + mh::kSide, r.Below(3) == 0 ? Around(r, -1, 1) : 0);
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// `saves` the original's register saves' bytes below the stack pointer, left
// out of the comparison.
void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, uint32_t saves,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{mh::kMapperEntries, 32}, {mh::kControllerEntries, 16}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    ShapeWorld(h.base, w, r);
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    out[v].Bytes(Read(h.base, mh::kMapperEntries, 32));
    out[v].Bytes(Read(h.base, mh::kControllerEntries, 16));
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, uint32_t saves) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, saves,
         fmt::format("case {}", c));
  }
  results.Report();
}

// An action id: mostly the menus' (151 to 180), sometimes any word, with
// random upper bits now and then (only the low word is an id).
uint64_t AnyAction(Random& r) {
  uint64_t id = r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30);
  if (r.Below(4) == 0) {
    id |= r.Next() << 32;
  }
  return id;
}

}  // namespace

void MenuHelpers(Context& h) {
  const World w;
  if (!w.ok()) {
    ITC_LOG_WARN("harness: menu_helpers: no memory for the world");
    return;
  }
  // Menu::Step: r27 to r31 and lr saved in [sp - 48, sp).
  Group(
      h, w, "Menu::Step (sub_82186568)", 1000,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.menu(); }, __imp__sub_82186568,
      hooks::MenuStep, 48);
  // Menu::Pressed: lr saved at sp - 8.
  Group(
      h, w, "Menu::Pressed (sub_821864C8)", 500,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.menu();
        t.r4.u64 = AnyAction(r);
      },
      __imp__sub_821864C8, hooks::MenuPressed, 8);
}

}  // namespace harness
