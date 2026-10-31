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

#include <bit>
#include <cstring>
#include <limits>
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

// --- The makers and teardowns ---

// The screens' globals (0x826D84D0 to 0x826D851F and 0x82814E10 to
// 0x82814E1F), saved around each run.
constexpr uint32_t kScreenGlobals = 0x826D84D0;
constexpr uint32_t kScreenGlobalsBytes = 0x50;
constexpr uint32_t kUnseenGlobals = 0x82814E10;
constexpr uint32_t kUnseenGlobalsBytes = 0x10;

struct Maker {
  const char* name;
  GuestFunction recompiled, native;
  // Each global the maker sets and the bytes of its block compared.
  std::vector<std::pair<uint32_t, uint32_t>> blocks;
  uint32_t frame, saves;
};

// A maker run: the globals 0 or random first; compared: each global set
// (to the layer's pages, by whether they are and their bytes), which one r3
// is, the frame and r1. The blocks are freed after.
void Makers(Context& h) {
  const std::vector<Maker> makers = {
      {"PauseMenu::Create (sub_8220D7E8)", __imp__sub_8220D7E8,
       hooks::RaceScreensPauseMenuCreate,
       {{rs::kPauseScreen, 4}, {rs::kPauseData, 160}, {rs::kPauseList, 72}, {rs::kPausePages, 28}},
       96, 8},
      {"PauseMenu4::Create (sub_82210868)", __imp__sub_82210868,
       hooks::RaceScreensPauseMenu4Create,
       {{rs::kPause4Screen, 4}, {rs::kPause4Data, 80}, {rs::kPause4List, 72}, {rs::kPause4Pages, 28}},
       96, 8},
      {"AfterRaceMenu::Create (sub_8221B1F0)", __imp__sub_8221B1F0,
       hooks::RaceScreensAfterRaceMenuCreate,
       {{rs::kAfterScreen, 4}, {rs::kAfterData, 24}, {rs::kAfterPages, 28}, {rs::kAfterList, 72}},
       96, 8},
      {"RaceDialog::Create (sub_82217D70)", __imp__sub_82217D70,
       hooks::RaceScreensRaceDialogCreate,
       {{rs::kDialogList, 72}, {rs::kDialogPages, 28}, {rs::kDialogData, 40}}, 96, 16},
      {"CareerResults::Create (sub_82219138)", __imp__sub_82219138,
       hooks::RaceScreensCareerResultsCreate, {{rs::kCareerResults, 212}}, 96, 8},
      {"RaceResults::Create (sub_8221F390)", __imp__sub_8221F390,
       hooks::RaceScreensRaceResultsCreate, {{rs::kRaceResults, 52}}, 96, 8},
      {"SplitResults::Create (sub_8221FA30)", __imp__sub_8221FA30,
       hooks::RaceScreensSplitResultsCreate, {{rs::kSplitResults, 40}}, 96, 8},
      {"Kind6Menu::Create (sub_8221BDD0)", __imp__sub_8221BDD0, hooks::RaceScreensKind6MenuCreate,
       {{rs::kKind6Menu, 360}, {rs::kKind6Word, 0}}, 96, 8},
  };
  for (const Maker& maker : makers) {
    Results results(maker.name);
    for (int c = 0; c < 10; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        SavedBytes saved(h.base, {{kScreenGlobals, kScreenGlobalsBytes},
                                  {kUnseenGlobals, kUnseenGlobalsBytes}});
        WriteRandom(h.base, kScreenGlobals, kScreenGlobalsBytes, r);
        WriteRandom(h.base, kUnseenGlobals, kUnseenGlobalsBytes, r);
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(h.base, sp - 1024, 1024 + kCallerBytes, r);
        ITC_FLAG(native_race_screens) = v == 1;
        (v == 0 ? maker.recompiled : maker.native)(t, h.base);
        ITC_FLAG(native_race_screens) = true;
        uint32_t which = 0xFFFFFFFF;
        for (size_t k = 0; k < maker.blocks.size(); k++) {
          const auto& [global, bytes] = maker.blocks[k];
          const uint32_t block = guest::LoadU32(h.base, global);
          if (bytes == 0) {
            out[v].Word(block);
            continue;
          }
          out[v].Word(block != 0 ? 1 : 0);
          out[v].Word(memory::IsPages(block) ? 1 : 0);
          if (block != 0) {
            out[v].Bytes(Read(h.base, block, bytes));
          }
          if (t.r3.u32 == block) {
            which = uint32_t(k);
          }
        }
        out[v].Word(which);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Bytes(Read(h.base, sp - maker.frame, maker.frame - maker.saves));
        out[v].Bytes(Read(h.base, sp, kCallerBytes));
        out[v].Word(t.r1.u32);
        for (const auto& [global, bytes] : maker.blocks) {
          const uint32_t block = guest::LoadU32(h.base, global);
          if (bytes != 0 && memory::IsPages(block)) {
            memory::FreePages(block);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

struct Teardown {
  const char* name;
  GuestFunction recompiled, native;
  std::vector<uint32_t> globals;  // the blocks freed
  uint32_t list;                  // the menu list's global, or 0
  uint32_t frame, saves;
  bool checks;                    // whether an unset first global prints instead
};

// A teardown run: the blocks the layer's pages (the menu list's block, +68,
// 0 or pages), now and then (where the teardown checks) the first global 0;
// compared: the globals, which blocks were freed, the frames (the
// teardown's and MenuList::Release's), r1 and r3. Blocks left are freed.
void Teardowns(Context& h) {
  const std::vector<Teardown> teardowns = {
      {"PauseMenu::Destroy (sub_8220C4C8)", __imp__sub_8220C4C8,
       hooks::RaceScreensPauseMenuDestroy,
       {rs::kPauseScreen, rs::kPauseData, rs::kPauseList, rs::kPausePages}, rs::kPauseList, 128,
       40, true},
      {"PauseMenu4::Destroy (sub_8220FC98)", __imp__sub_8220FC98,
       hooks::RaceScreensPauseMenu4Destroy,
       {rs::kPause4Screen, rs::kPause4Data, rs::kPause4List, rs::kPause4Pages}, rs::kPause4List,
       128, 40, true},
      {"AfterRaceMenu::Destroy (sub_8221A3E8)", __imp__sub_8221A3E8,
       hooks::RaceScreensAfterRaceMenuDestroy,
       {rs::kAfterScreen, rs::kAfterData, rs::kAfterList, rs::kAfterPages}, rs::kAfterList, 128,
       40, true},
      {"RaceDialog::Destroy (sub_82217CD0)", __imp__sub_82217CD0,
       hooks::RaceScreensRaceDialogDestroy, {rs::kDialogList, rs::kDialogPages, rs::kDialogData},
       rs::kDialogList, 112, 32, false},
      {"CareerResults::Destroy (sub_82219170)", __imp__sub_82219170,
       hooks::RaceScreensCareerResultsDestroy, {rs::kCareerResults}, 0, 96, 16, true},
      {"RaceResults::Destroy (sub_8221F3C8)", __imp__sub_8221F3C8,
       hooks::RaceScreensRaceResultsDestroy, {rs::kRaceResults}, 0, 96, 16, true},
      {"SplitResults::Destroy (sub_8221FA68)", __imp__sub_8221FA68,
       hooks::RaceScreensSplitResultsDestroy, {rs::kSplitResults}, 0, 96, 16, true},
      {"Kind6Menu::Destroy (sub_8221BE30)", __imp__sub_8221BE30,
       hooks::RaceScreensKind6MenuDestroy, {rs::kKind6Menu}, 0, 96, 16, true},
      {"AfterRaceMenu::Close (sub_8221A498)", __imp__sub_8221A498,
       hooks::RaceScreensAfterRaceMenuClose, {}, rs::kAfterList, 0, 0, false},
  };
  for (const Teardown& teardown : teardowns) {
    Results results(teardown.name);
    for (int c = 0; c < 20; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        SavedBytes saved(h.base, {{kScreenGlobals, kScreenGlobalsBytes},
                                  {kUnseenGlobals, kUnseenGlobalsBytes}});
        WriteRandom(h.base, kScreenGlobals, kScreenGlobalsBytes, r);
        WriteRandom(h.base, kUnseenGlobals, kUnseenGlobalsBytes, r);
        std::vector<uint32_t> blocks;
        for (const uint32_t global : teardown.globals) {
          const uint32_t block = memory::AllocPages(0x100, 4096);
          WriteRandom(h.base, block, 0x100, r);
          guest::StoreU32(h.base, global, block);
          blocks.push_back(block);
        }
        uint32_t list_pages = 0;
        uint32_t own_list = 0;
        if (teardown.list != 0) {
          uint32_t list = guest::LoadU32(h.base, teardown.list);
          if (teardown.globals.empty()) {
            own_list = memory::AllocPages(0x100, 4096);
            WriteRandom(h.base, own_list, 0x100, r);
            guest::StoreU32(h.base, teardown.list, own_list);
            list = own_list;
          }
          if (r.Below(3) != 0) {
            list_pages = memory::AllocPages(0x1000, 4096);
          }
          guest::StoreU32(h.base, list + 68, list_pages);
        }
        const bool unset = teardown.checks && r.Below(4) == 0;
        if (unset) {
          guest::StoreU32(h.base, teardown.globals[0], 0);
        }
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(h.base, sp - 1024, 1024 + kCallerBytes, r);
        ITC_FLAG(native_race_screens) = v == 1;
        (v == 0 ? teardown.recompiled : teardown.native)(t, h.base);
        ITC_FLAG(native_race_screens) = true;
        for (const uint32_t global : teardown.globals) {
          out[v].Word(guest::LoadU32(h.base, global));
        }
        for (const uint32_t block : blocks) {
          out[v].Word(memory::IsPages(block) ? 1 : 0);
        }
        out[v].Word(list_pages != 0 && memory::IsPages(list_pages) ? 1 : 0);
        if (own_list != 0) {
          out[v].Word(guest::LoadU32(h.base, own_list + 68));
        }
        out[v].Bytes(Read(h.base, sp - teardown.frame, teardown.frame - teardown.saves));
        if (teardown.list != 0) {
          // MenuList::Release's frame below the teardown's.
          out[v].Bytes(Read(h.base, sp - teardown.frame - 96, 80));
        }
        out[v].Bytes(Read(h.base, sp, kCallerBytes));
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        for (const uint32_t block : blocks) {
          if (memory::IsPages(block)) {
            memory::FreePages(block);
          }
        }
        for (const uint32_t block : {list_pages, own_list}) {
          if (block != 0 && memory::IsPages(block)) {
            memory::FreePages(block);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The pause menu's data and the answers ---

constexpr uint32_t kSaveDataPointers = 0x826C5B58;
constexpr uint32_t kVehiclesPointer = 0x826C2E34;
constexpr uint32_t kSaveBytes = 0x1F85C + 0x100;
constexpr uint32_t kVehicleBytes = 6272;

// A float: mostly ordinary values in `range`, sometimes an edge.
uint32_t AnyFloatBits(Random& r, float range) {
  switch (r.Below(12)) {
    case 0:
      return std::bit_cast<uint32_t>(std::numeric_limits<float>::quiet_NaN());
    case 1:
      return std::bit_cast<uint32_t>(std::numeric_limits<float>::infinity());
    case 2:
      return std::bit_cast<uint32_t>(-std::numeric_limits<float>::infinity());
    case 3:
      return std::bit_cast<uint32_t>(-0.0f);
    case 4:
      return uint32_t(r.Next());
    default:
      return std::bit_cast<uint32_t>(
          (float(r.Below(1u << 20)) / float(1u << 20) * 2.0f - 0.5f) * range);
  }
}

void Data(Context& h) {
  const Block save(kSaveBytes);
  const Block vehicle(kVehicleBytes);
  const Block data(0x400);  // the pause menu's (+0), the menu after the race's (+0x100),
                            // the dialog's (+0x200)
  // A stack of our own for LoadData's 6,176-byte frame (the calls run with
  // their stack pointer at +kDataStackTop).
  constexpr uint32_t kDataStackBytes = 0x3000;
  constexpr uint32_t kDataStackTop = 0x2800;
  const Block stack(kDataStackBytes);
  if (!save.address || !vehicle.address || !data.address || !stack.address) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t frame, saves;
    int cases;
  };
  const std::vector<Function> functions = {
      {"PauseMenu::LoadData (sub_8220F840)", __imp__sub_8220F840,
       hooks::RaceScreensPauseMenuLoadData, 6176, 40, 300},
      {"PauseMenu::SaveOptions (sub_8220D6A0)", __imp__sub_8220D6A0,
       hooks::RaceScreensPauseMenuSaveOptions, 96, 8, 50},
      {"PauseMenu::Close (sub_8220D890)", __imp__sub_8220D890, hooks::RaceScreensPauseMenuClose,
       96, 8, 20},
      {"PauseMenu::SaveHudOptions (sub_8220D740)", __imp__sub_8220D740,
       hooks::RaceScreensPauseMenuSaveHudOptions, 96, 8, 50},
      {"PauseMenu::AnswerPage (sub_8220C5D0)", __imp__sub_8220C5D0,
       hooks::RaceScreensPauseMenuAnswerPage, 0, 0, 20},
      {"AfterRaceMenu::AnswerPage (sub_8221A720)", __imp__sub_8221A720,
       hooks::RaceScreensAfterRaceMenuAnswerPage, 0, 0, 20},
      {"RaceDialog::Answer (sub_82217D40)", __imp__sub_82217D40,
       hooks::RaceScreensRaceDialogAnswer, 0, 0, 40},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < f.cases; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        SavedBytes saved(h.base, {{kScreenGlobals, kScreenGlobalsBytes},
                                  {kSaveDataPointers, 4},
                                  {kVehiclesPointer, 4}});
        WriteRandom(h.base, save.address, save.bytes, r);
        WriteRandom(h.base, vehicle.address, vehicle.bytes, r);
        WriteRandom(h.base, data.address, data.bytes, r);
        guest::StoreU32(h.base, kSaveDataPointers, save.address);
        guest::StoreU32(h.base, kVehiclesPointer, vehicle.address);
        guest::StoreU32(h.base, rs::kPauseData, data.address);
        guest::StoreU32(h.base, rs::kAfterData, data.address + 0x100);
        guest::StoreU32(h.base, rs::kDialogData, data.address + 0x200);
        // The pause menu's kind (+132) 0 to 4; the stats' win streak (the
        // copy's halfword +5970) small, negative or above 999; the day
        // (+6100) small, negative or large; the vehicle's two floats.
        guest::StoreU32(h.base, data.address + 132,
                        r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
        const uint16_t streaks[4] = {uint16_t(r.Below(1000)), uint16_t(0x8000 | r.Below(0x8000)),
                                     uint16_t(1000 + r.Below(5000)), 999};
        guest::StoreU16(h.base, save.address + 48 + 5970, streaks[r.Below(4)]);
        const uint32_t days[4] = {r.Below(400), uint32_t(-int32_t(r.Below(50))),
                                  uint32_t(r.Next()), 1000};
        guest::StoreU32(h.base, save.address + 6100, days[r.Below(4)]);
        guest::StoreU32(h.base, vehicle.address + 6248, AnyFloatBits(r, 400.0f));
        guest::StoreU32(h.base, vehicle.address + 6244, AnyFloatBits(r, 400.0f));
        guest::StoreU32(h.base, data.address + 0x200, r.Below(3));
        PPCContext t = CallContext(h);
        t.r1.u64 = stack.address + kDataStackTop;
        const uint32_t sp = t.r1.u32;
        WriteRandom(h.base, stack.address, stack.bytes, r);
        ITC_FLAG(native_race_screens) = v == 1;
        (v == 0 ? f.recompiled : f.native)(t, h.base);
        ITC_FLAG(native_race_screens) = true;
        out[v].Bytes(Read(h.base, data.address, data.bytes));
        out[v].Bytes(Read(h.base, save.address + 0x1F85C, 0x100));
        if (f.frame > f.saves) {
          out[v].Bytes(Read(h.base, sp - f.frame, f.frame - f.saves));
        }
        out[v].Bytes(Read(h.base, sp, kCallerBytes));
        out[v].Word(t.r1.u32);
        out[v].Word(t.r3.u32 == save.address ? 1 : t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void RaceScreens(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  PagesAndLists(h, w);
  Makers(h);
  Teardowns(h);
  Data(h);
}

}  // namespace harness
