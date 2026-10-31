// The other in-race screens (src/race_screens/, docs/functions/race-screens.md)
// against the recompiled functions. Each case fills a world of our own with
// random words from the case's seed (a menu list and its block of item
// words, a page object, the action mappers with their vtables), plus the
// stack below the call's stack pointer and the caller's area above it;
// shapes the fields the function tests; runs the original (with
// --native_race_screens off) or the register steps (hooks/race_screens.h);
// and compares the world, the originals' frames (their register saves left
// out), the caller's area, r1 and r3, all exactly (nothing here is a float).
// The globals the cases point elsewhere are put back after each run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/race_screens.h"
#include "memory/heap.h"
#include "race_screens/race_screens.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_screens);

namespace harness {

namespace {

namespace rs = race_screens;

constexpr uint32_t kMiscBytes = 0x2000;
constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kBlockWords = 64;

// The mapper read slots a vtable may hold: Pressed, Fired (native read
// slots, their adapters), and Held (through the vtable from the plain C++).
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
  uint32_t list() const { return misc.address; }
  uint32_t pages() const { return misc.address + 0x80; }
  uint32_t block() const { return misc.address + 0x100; }
  uint32_t vtable(uint32_t i) const { return misc.address + 0x300 + 0x40 * i; }
  uint32_t mapper(uint32_t i) const { return misc.address + 0x400 + 0x200 * i; }
  Block misc;  // the menu list (+0), the page object (+0x80), the list's block (+0x100),
               // the mappers' vtables (+0x300) and the mappers (+0x400)
};

// An action id: mostly the menus' (151 to 180), sometimes any word.
uint32_t AnyAction(Random& r) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30);
}

// A small signed word around [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The list's controller (+60) 0 to 3, each controller's entry 0 to 3, each
// entry's mapper one of ours (its vtable's slots 10 and 12 Pressed and
// Fired, now and then Held) and +4 now and then 1; the list's count (+0)
// 0 to 12, scroll, shown and selection around it, the action ids, the
// wrap word and the block of item words (half of them 0).
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  const uint32_t list = w.list();
  guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, w.mapper(c));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, w.mapper(c), w.vtable(c));
    guest::StoreU32(base, w.vtable(c) + 40, r.Below(5) == 0 ? kHeldSlot : rs::kPressedSlot);
    guest::StoreU32(base, w.vtable(c) + 48, r.Below(5) == 0 ? kHeldSlot : rs::kFiredSlot);
  }
  const int32_t count = int32_t(r.Below(13));
  guest::StoreU32(base, list + rs::kListCount, uint32_t(count));
  guest::StoreU32(base, list + rs::kListShown, Around(r, 0, count + 1));
  guest::StoreU32(base, list + rs::kListScroll, Around(r, -1, count));
  guest::StoreU32(base, list + rs::kListSelection, Around(r, -2, count + 1));
  for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
    guest::StoreU32(base, list + field, AnyAction(r));
  }
  guest::StoreU32(base, list + rs::kListWraps, r.Below(3) == 0 ? 0 : r.Below(4));
  guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, list + rs::kListBlock, w.block());
  for (uint32_t i = 0; i < kBlockWords; i++) {
    guest::StoreU32(base, w.block() + 4 * i, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
  // The mappers' words (+448 to +471): a few bits set, so the slots answer
  // both ways.
  for (uint32_t c = 0; c < 4; c++) {
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, w.mapper(c) + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// The original's frames compared, as [sp - from, sp - to) ranges (its
// register saves left out).
using Frames = std::vector<std::pair<uint32_t, uint32_t>>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Frames& frames,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{rs::kMapperEntries, 32}, {rs::kControllerEntries, 16}});
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.misc.address, w.misc.bytes, r);
    ShapeWorld(h.base, w, r);
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    ITC_FLAG(native_race_screens) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_race_screens) = true;
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    out[v].Bytes(Read(h.base, rs::kMapperEntries, 32));
    for (const auto& [from, to] : frames) {
      out[v].Bytes(Read(h.base, sp - from, from - to));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Frames& frames = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, frames,
         fmt::format("case {}", c));
  }
  results.Report();
}

// A page object's words: pages 0 to 7, transitions -1 to 20, now and then
// any word.
void ShapePages(uint8_t* base, const World& w, Random& r) {
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, w.pages() + 4 * k, Around(r, -1, k == 3 || k == 4 ? 20 : 7));
  }
}

// --- The page object and the menu list ---

void PagesAndLists(Context& h, const World& w) {
  Group(
      h, w, "ScreenPages::Init (sub_8220BA98)", 50,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.pages();
        t.r4.u64 = Around(r, -1, 7);
      },
      __imp__sub_8220BA98, hooks::RaceScreensPagesInit);
  Group(
      h, w, "ScreenPages::Go (sub_8220BA68)", 100,
      [&h](PPCContext& t, Random& r, const World& w) {
        ShapePages(h.base, w, r);
        t.r3.u64 = w.pages();
        t.r4.u64 = Around(r, -1, 7);
        t.r5.u64 = r.Below(3) == 0 ? ~uint64_t(0) : Around(r, -1, 20);
        t.r6.u64 = Around(r, -1, 20);
      },
      __imp__sub_8220BA68, hooks::RaceScreensPagesGo);
  Group(
      h, w, "ScreenPages::Finish (sub_8220BA28)", 100,
      [&h](PPCContext& t, Random& r, const World& w) {
        ShapePages(h.base, w, r);
        t.r3.u64 = w.pages();
        t.r4.u64 = r.Below(3) == 0 ? 0 : Around(r, -1, 2);
      },
      __imp__sub_8220BA28, hooks::RaceScreensPagesFinish);
  Group(
      h, w, "MenuList::Selection (sub_8220BC20)", 30,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.list(); },
      __imp__sub_8220BC20, hooks::RaceScreensListSelection);
  Group(
      h, w, "MenuList::SetSelection (sub_8220BC30)", 30,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.list();
        t.r4.u64 = Around(r, -1, 12);
      },
      __imp__sub_8220BC30, hooks::RaceScreensListSetSelection);
  Group(
      h, w, "MenuList::SetEnabled (sub_8220BC00)", 50,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.list();
        t.r4.u64 = r.Below(kBlockWords);
        t.r5.u64 = r.Below(2);
      },
      __imp__sub_8220BC00, hooks::RaceScreensListSetEnabled);
  Group(
      h, w, "MenuList::IsEnabled (sub_8220BC10)", 50,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.list();
        t.r4.u64 = r.Below(kBlockWords);
      },
      __imp__sub_8220BC10, hooks::RaceScreensListIsEnabled);
  Group(
      h, w, "MenuList::Fit (sub_8220BC40)", 400,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.list();
        t.r4.u64 = r.Below(2) == 0 ? 0 : (r.Below(4) == 0 ? r.Next() : 1);
      },
      __imp__sub_8220BC40, hooks::RaceScreensListFit);
  Group(
      h, w, "MenuList::Fired (sub_8220C020)", 400,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.list();
        t.r4.u64 = r.Below(4) == 0 ? 152 + r.Below(20) : AnyAction(r);
      },
      __imp__sub_8220C020, hooks::RaceScreensListFired, {{96, 16}});
  // Update: Fired's frames below its own (from each of its calls).
  Group(
      h, w, "MenuList::Update (sub_8220C1E0)", 800,
      [&h](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.list();
        // The directions' ids (+36 to +48) often the menus' own (down 154 or
        // 168, up 153 or 167, left 155 or 169, right 156 or 170), so the
        // moves run.
        if (r.Below(4) != 0) {
          const uint32_t ids[4] = {154, 153, 155, 156};
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, w.list() + rs::kListDown + 4 * k,
                            ids[k] + (r.Below(2) == 0 ? 0 : 14));
          }
        }
      },
      __imp__sub_8220C1E0, hooks::RaceScreensListUpdate, {{144, 56}, {240, 160}});
  const Frames pressed = {{96, 16}};
  Group(
      h, w, "MenuList::Accepted (sub_8220BD50)", 200,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.list(); }, __imp__sub_8220BD50,
      hooks::RaceScreensListAccepted, pressed);
  Group(
      h, w, "MenuList::Cancelled (sub_8220BDE0)", 200,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.list(); }, __imp__sub_8220BDE0,
      hooks::RaceScreensListCancelled, pressed);
  Group(
      h, w, "MenuList::Pressed152 (sub_8220BE70)", 200,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.list(); }, __imp__sub_8220BE70,
      hooks::RaceScreensListPressed152, pressed);
  Group(
      h, w, "MenuList::Pressed166 (sub_8220BF90)", 200,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.list(); }, __imp__sub_8220BF90,
      hooks::RaceScreensListPressed166, pressed);
}

}  // namespace

void RaceScreens(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  PagesAndLists(h, w);
}

}  // namespace harness
