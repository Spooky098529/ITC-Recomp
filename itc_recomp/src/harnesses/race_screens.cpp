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
#include "data_tables/data_tables.h"
#include "hooks/race_hud_machine.h"
#include "hooks/race_hud_object.h"
#include "hooks/race_screens.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <bit>
#include <cstring>
#include <limits>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_screens);
ITC_FLAG_DECLARE(bool, native_race_hud_machine);

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

// --- The pages' dispatch and the pause menu's steps ---

// A world for the screens' pages: each screen global (0x826D84D0 on) a
// block of its own (0x200 bytes) of random words; the pause menu's screen
// word its movie, whose live scene (+16) is a pool node; each pool node's
// child index (+108) 0 to 3 and its child entries (+128 on) pool nodes.
constexpr uint32_t kPageNodes = 16;
constexpr uint32_t kPageNodeBytes = 0x400;
constexpr uint32_t kPageBlocks = 20;  // the 20 globals 0x826D84D0 to 0x826D851C
constexpr uint32_t kPageBlockBytes = 0x200;

struct PageWorld {
  PageWorld()
      : blocks(kPageBlocks * kPageBlockBytes), movie(0x100), nodes(kPageNodes * kPageNodeBytes) {}
  bool ok() const { return blocks.address && movie.address && nodes.address; }
  uint32_t block(uint32_t global) const {
    return blocks.address + (global - kScreenGlobals) / 4 * kPageBlockBytes;
  }
  uint32_t node(uint32_t i) const { return nodes.address + kPageNodeBytes * i; }
  Block blocks;
  Block movie;
  Block nodes;
};

void ShapePages(uint8_t* base, const PageWorld& w, Random& r) {
  WriteRandom(base, w.blocks.address, w.blocks.bytes, r);
  WriteRandom(base, w.movie.address, w.movie.bytes, r);
  WriteRandom(base, w.nodes.address, w.nodes.bytes, r);
  for (uint32_t g = kScreenGlobals; g < kScreenGlobals + kScreenGlobalsBytes; g += 4) {
    guest::StoreU32(base, g, w.block(g));
  }
  guest::StoreU32(base, w.block(rs::kPauseScreen), w.movie.address);
  guest::StoreU32(base, w.movie.address + 16, w.node(r.Below(kPageNodes)));
  for (uint32_t k = 0; k < kPageNodes; k++) {
    guest::StoreU32(base, w.node(k) + 108, r.Below(4));
    for (uint32_t i = 128; i < kPageNodeBytes; i += 8) {
      guest::StoreU32(base, w.node(k) + i, w.node(r.Below(kPageNodes)));
    }
  }
}

using PageSetupFn = std::function<void(PPCContext&, Random&, const PageWorld&)>;

void PageGroup(Context& h, const PageWorld& w, const char* name, int cases,
               const PageSetupFn& setup, GuestFunction recompiled, GuestFunction native,
               uint32_t frame, uint32_t saves) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      SavedBytes saved(h.base, {{kScreenGlobals, kScreenGlobalsBytes},
                                {kUnseenGlobals, kUnseenGlobalsBytes}});
      ShapePages(h.base, w, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      ITC_FLAG(native_race_screens) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_race_screens) = true;
      out[v].Bytes(Read(h.base, w.blocks.address, w.blocks.bytes));
      out[v].Bytes(Read(h.base, kScreenGlobals, kScreenGlobalsBytes));
      out[v].Bytes(Read(h.base, sp - frame, frame - saves));
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void Pages(Context& h) {
  const PageWorld w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  auto page_of = [&h](uint32_t pages, std::initializer_list<int32_t> choices, Random& r) {
    const int32_t* list = choices.begin();
    guest::StoreU32(h.base, pages, uint32_t(list[r.Below(uint32_t(choices.size()))]));
  };
  // The pages whose handlers the cases run: none (-1) and the answers (the
  // pause menu's 5, the menu after the race's 7, the dialog's 5, the
  // results' 0 and 4); the dialog's other pages only with its +36 1 (the
  // Go to 5 leaves no page).
  PageGroup(
      h, w, "PauseMenu::Update (sub_8220F638)", 60,
      [&](PPCContext&, Random& r, const PageWorld& w) {
        page_of(w.block(rs::kPausePages), {-1, 5}, r);
      },
      __imp__sub_8220F638, hooks::RaceScreensPauseMenuUpdate, 112, 8);
  PageGroup(
      h, w, "CareerResults::Update (sub_8221A090)", 60,
      [&](PPCContext& t, Random& r, const PageWorld& w) {
        t.r3.u64 = w.block(rs::kCareerResults);
        page_of(w.block(rs::kCareerResults) + 184, {-1, 0, 4}, r);
      },
      __imp__sub_8221A090, hooks::RaceScreensCareerResultsUpdate, 112, 8);
  PageGroup(
      h, w, "AfterRaceMenu::Update (sub_8221BD20)", 60,
      [&](PPCContext&, Random& r, const PageWorld& w) {
        page_of(w.block(rs::kAfterPages), {-1, 7}, r);
      },
      __imp__sub_8221BD20, hooks::RaceScreensAfterRaceMenuUpdate, 128, 8);
  PageGroup(
      h, w, "RaceDialog::Update (sub_822185B0)", 120,
      [&](PPCContext&, Random& r, const PageWorld& w) {
        const uint32_t pages = w.block(rs::kDialogPages);
        const uint32_t data = w.block(rs::kDialogData);
        page_of(pages, {-1, 5, 0, 1, 2, 3, 4}, r);
        const int32_t page = int32_t(guest::LoadU32(h.base, pages));
        guest::StoreU32(h.base, pages + 12, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3));
        guest::StoreU32(h.base, data + 36, r.Below(2));
        guest::StoreU32(h.base, data + 0, r.Below(3));
        if (page != -1 && page != 5) {
          guest::StoreU32(h.base, data + 36, 1);
        } else if (page == -1 && int32_t(guest::LoadU32(h.base, pages + 12)) != -1) {
          // Skips the Go: the page stays -1.
        }
      },
      __imp__sub_822185B0, hooks::RaceScreensRaceDialogUpdate, 128, 24);
  PageGroup(
      h, w, "CareerResults::Start (sub_822191D0)", 20,
      [](PPCContext&, Random&, const PageWorld&) {}, __imp__sub_822191D0,
      hooks::RaceScreensCareerResultsStart, 96, 8);
  // The steps: the list's side (+28) -1, 0, 1 or any word, its selection
  // (+16; the scroll +4 0) 0 to 4; the values near their bounds.
  auto steps = [&h](PPCContext&, Random& r, const PageWorld& w) {
    const uint32_t list = w.block(rs::kPauseList);
    const uint32_t sides[4] = {0xFFFFFFFFu, 0, 1, uint32_t(r.Next())};
    guest::StoreU32(h.base, list + rs::kListSide, sides[r.Below(4)]);
    guest::StoreU32(h.base, list + rs::kListSelection, r.Below(5));
    const uint32_t data = w.block(rs::kPauseData);
    for (const uint32_t field : {52u, 56u}) {
      guest::StoreU32(h.base, data + field, uint32_t(int32_t(r.Below(19)) - 2));
    }
    for (const uint32_t field : {60u, 64u, 68u}) {
      guest::StoreU32(h.base, data + field, uint32_t(int32_t(r.Below(25)) - 12));
    }
    guest::StoreU32(h.base, data + 144, r.Below(3));
  };
  PageGroup(h, w, "PauseMenu::StepVolume (sub_8220CED0)", 200, steps, __imp__sub_8220CED0,
            hooks::RaceScreensPauseMenuStepVolume, 96, 16);
  PageGroup(h, w, "PauseMenu::StepAdjust (sub_8220D248)", 200, steps, __imp__sub_8220D248,
            hooks::RaceScreensPauseMenuStepAdjust, 96, 16);
}

// --- The screens' transitions and node helpers (a UI movie world) ---
//
// The world the transitions' callees read, built as the UI movies' own
// harnesses (location_screen, race_hud_object) build it: the screens' globals
// each a block of its own (as the pages' world), four movies (ids 4, 14, 6
// and 9 in the movie table, the pause menu's screen word movie 4) with
// empty animation lists, live, saved and file scenes one tree's root, their
// 256 first named animations over the root and one of 8 animations of no
// tracks; the root's child index (+108) 0 to 3, its entries sub-scenes up
// to 60 and animations after (its animation index, +116, 60); the 16
// sub-scenes' child index 0 to 3, their entries leaves up to 64 and
// animations after (+116 64); the 32 leaves TXT nodes (UiMovie::SetText's
// assert), their children's range (+100 below 16, +104 0 to 6), their
// text id and child index (+108) 0 to 8 and their entries leaves; each
// movie's texts ids 1 to 7 (records of 4 KB buffers of letters and page
// breaks; the text count 8; the missing text another record) and the
// string table of letters only (no argument codes: the user's call,
// 2026-10-30). The callees run for real on both sides (the originals' and
// ours reach the same overrides of the UI movie calls); UiMovie::Run's update
// (sub_8211D2F8) is the UI movies' stand-in's, recorded and answered with 0
// or a word from the case's generator.
constexpr uint32_t kMovieHeaderBytes = 0xA000;
constexpr uint32_t kMovieCount = 4;
constexpr uint32_t kMovieIdsOf[kMovieCount] = {4, 14, 6, 9};
constexpr uint32_t kTreeNodeBytes = 0x400;
constexpr uint32_t kSubScenes = 16, kLeaves = 32, kAnimations = 8;
constexpr uint32_t kRootAt = kMovieCount * kMovieHeaderBytes;
constexpr uint32_t kSubScenesAt = kRootAt + kTreeNodeBytes;
constexpr uint32_t kLeavesAt = kSubScenesAt + kSubScenes * kTreeNodeBytes;
constexpr uint32_t kAnimationsAt = kLeavesAt + kLeaves * kTreeNodeBytes;
constexpr uint32_t kPairsAt = kAnimationsAt + kAnimations * 64;
constexpr uint32_t kTexturesAt = kPairsAt + 0x40;
constexpr uint32_t kMovieWorldBytes = kTexturesAt + 0x100;
constexpr uint32_t kTextsPerMovie = 8;
constexpr uint32_t kTextBuffersAt = 0x1000;
constexpr uint32_t kTextWorldBytes = kTextBuffersAt + kMovieCount * kTextsPerMovie * 0x1000;
constexpr uint32_t kStringWorldBytes = 0x10000;
constexpr uint32_t kMissingText = 0x826C5CD8;   // TextMap::Find's answer when not found
constexpr uint32_t kMissingMovie = 0x826C5CDC;  // UiMovies::Find's answer when not found
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTagTxt = 0x54585400;
constexpr uint32_t kTagSce = 0x53434500;

// The save system's interface ([0x826BDC90]) and the replay list
// ([0x828161F0]), pointed at a world of the case's own where the confirm
// dialog's cases reach them (SaveSlots::RequestDevice, Replays::Delete).
constexpr uint32_t kSaveInterfaceGlobal = 0x826BDC90;
constexpr uint32_t kReplayList = 0x828161F0;

// A stack of its own for PauseMenu::Open (LoadData's 6,176-byte frame
// below it), the stack pointer at +kOwnStackTop.
constexpr uint32_t kOwnStackBytes = 0x3000;
constexpr uint32_t kOwnStackTop = 0x2800;

struct MovieWorld {
  MovieWorld()
      : blocks(kPageBlocks * kPageBlockBytes),
        movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes),
        misc(0x1000),
        save(kSaveBytes),
        vehicle(kVehicleBytes),
        stack(kOwnStackBytes) {}
  bool ok() const {
    return blocks.address && movies.address && texts.address && strings.address &&
           misc.address && save.address && vehicle.address && stack.address;
  }
  uint32_t block(uint32_t global) const {
    return blocks.address + (global - kScreenGlobals) / 4 * kPageBlockBytes;
  }
  uint32_t movie(uint32_t m) const { return movies.address + kMovieHeaderBytes * m; }
  uint32_t root() const { return movies.address + kRootAt; }
  uint32_t sub(uint32_t k) const { return movies.address + kSubScenesAt + kTreeNodeBytes * k; }
  uint32_t leaf(uint32_t k) const { return movies.address + kLeavesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  Block blocks;
  Block movies;
  Block texts;
  Block strings;
  Block misc;     // the sound manager (+0) and its vtable (+0x400); the
                  // menu lists' item words (+0x800)
  Block save;     // player 0's save data (PauseMenu::LoadData)
  Block vehicle;  // Vehicle::Get(0)
  Block stack;
};

// The string table: 17 files' pointers to one container of entries enough
// for the widest file (the bounds at 0x82007E18), each one of 8 strings of
// letters (no argument codes), as the location screen's harness builds it.
void BuildStringTable(uint8_t* base, const MovieWorld& w) {
  constexpr uint32_t kBounds = 0x82007E18;
  uint32_t span = 1;
  for (uint32_t i = 0; i < 17; i++) {
    const int32_t from = int32_t(guest::LoadU32(base, kBounds + 4 * i));
    const int32_t to = int32_t(guest::LoadU32(base, kBounds + 4 * i + 4));
    if (to > from && uint32_t(to - from) > span) {
      span = uint32_t(to - from);
    }
  }
  const uint32_t at = w.strings.address, container = at + 0x100;
  if (container + 4 + 4 * span + 8 * 64 > at + w.strings.bytes) {
    ITC_LOG_ERROR("harness: race_screens: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x5061757365ull);  // "Pause"
  const uint32_t text = container + 4 + 4 * span;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t length = r.Below(30);
    for (uint32_t c = 0; c < length; c++) {
      guest::StoreU16(base, text + 64 * k + 2 * c, uint16_t('A' + r.Below(26)));
    }
    guest::StoreU16(base, text + 64 * k + 2 * length, 0);
  }
  for (uint32_t i = 0; i < span; i++) {
    guest::StoreU32(base, container + 4 + 4 * i,
                    __builtin_bswap32(text + 64 * r.Below(8) - container));
  }
}

void ShapeMovieWorld(uint8_t* base, const MovieWorld& w, Random& r) {
  WriteRandom(base, w.blocks.address, w.blocks.bytes, r);
  WriteRandom(base, w.movies.address, w.movies.bytes, r);
  WriteRandom(base, w.texts.address, w.texts.bytes, r);
  for (uint32_t g = kScreenGlobals; g < kScreenGlobals + kScreenGlobalsBytes; g += 4) {
    guest::StoreU32(base, g, w.block(g));
  }
  guest::StoreU32(base, w.block(rs::kPauseScreen), w.movie(0));
  guest::StoreU32(base, w.block(rs::kAfterScreen), w.movie(3));
  for (uint32_t m = 0; m < kMovieCount; m++) {
    const uint32_t movie = w.movie(m);
    ui_movies::InitAnimationLists(base, movie);
    for (const uint32_t field : {16u, 20u, 24u}) {
      guest::StoreU32(base, movie + field, w.root());
    }
    guest::StoreU32(base, movie + 28, w.movies.address + kTexturesAt);
    for (uint32_t k = 0; k < 256; k++) {
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, w.root());
      guest::StoreU32(base, named + 32, w.animation(r.Below(kAnimations)));
    }
    // The texts: ids 1 to 7, each a record of a 4 KB buffer of its own.
    for (uint32_t k = 0; k < kTextsPerMovie; k++) {
      const uint32_t record = w.texts.address + 0x100 * m + 32 * k;
      const uint32_t buffer = w.texts.address + kTextBuffersAt + 0x1000 * (kTextsPerMovie * m + k);
      guest::StoreU32(base, record, buffer);
      guest::StoreU32(base, record + 20, r.Below(5) - 1);
      const uint32_t length = r.Below(60);
      for (uint32_t c = 0; c < length; c++) {
        guest::StoreU16(base, buffer + 2 * c,
                        r.Below(6) == 0 ? 0xFFD1 : uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, buffer + 2 * length, 0);
      if (k + 1 < kTextsPerMovie) {
        guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k, k + 1);
        guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k + 4, record);
      }
    }
    guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, kTextsPerMovie - 1);
    guest::StoreU32(base, movie + ui_movies::kMovieTextCount, kTextsPerMovie);
  }
  guest::StoreU32(base, kMissingText, w.texts.address + 32 * (kTextsPerMovie - 1));
  for (uint32_t k = 0; k < kAnimations; k++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, w.animation(k) + i, 0);
    }
    guest::StoreFloat(base, w.animation(k) + 12, float(r.Below(100)) / 4.0f);
  }
  constexpr uint32_t kEntries = (kTreeNodeBytes - 128) / 8;
  guest::StoreU32(base, w.root(), kTagSce);
  guest::StoreU32(base, w.root() + 100, 0);  // the pose resets walk children 0 to 3
  guest::StoreU32(base, w.root() + 104, r.Below(5));
  guest::StoreU32(base, w.root() + 108, r.Below(4));
  guest::StoreU32(base, w.root() + 116, 60);
  for (uint32_t i = 0; i < kEntries; i++) {
    guest::StoreU32(base, w.root() + 128 + 8 * i,
                    i < 60 ? w.sub(r.Below(kSubScenes)) : w.animation(r.Below(kAnimations)));
  }
  for (uint32_t k = 0; k < kSubScenes; k++) {
    const uint32_t sub = w.sub(k);
    guest::StoreU32(base, sub, kTagSce);
    guest::StoreU32(base, sub + 100, r.Below(16));
    guest::StoreU32(base, sub + 104, r.Below(7));
    guest::StoreU32(base, sub + 108, r.Below(4));
    guest::StoreU32(base, sub + 116, 64);
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, sub + 128 + 8 * i,
                      i < 64 ? w.leaf(r.Below(kLeaves)) : w.animation(r.Below(kAnimations)));
    }
  }
  for (uint32_t k = 0; k < kLeaves; k++) {
    const uint32_t leaf = w.leaf(k);
    guest::StoreU32(base, leaf, kTagTxt);
    guest::StoreU32(base, leaf + 100, r.Below(16));
    guest::StoreU32(base, leaf + 104, r.Below(8) == 0 ? 0 : r.Below(7));
    guest::StoreU32(base, leaf + 108, r.Below(9));
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, leaf + 128 + 8 * i, w.leaf(r.Below(kLeaves)));
    }
  }
  const uint32_t pairs = w.movies.address + kPairsAt;
  for (uint32_t m = 0; m < kMovieCount; m++) {
    guest::StoreU32(base, pairs + 8 * m, kMovieIdsOf[m]);
    guest::StoreU32(base, pairs + 8 * m + 4, w.movie(m));
  }
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, kMovieCount);
  guest::StoreU32(base, kMissingMovie, w.movie(0));
  guest::StoreU32(base, kStringTable, w.strings.address);
  // The pause menu's data: the kind (+132) 0 to 8, the toggle (+144) 0 to 2,
  // the counter (+152) around 0 to 12, the edits (+36 to +48) 0, 1 or any;
  // its list's selection and the one before (+16, +20) 0 to 7, now and then
  // up to 20; the step words (+24 of each page object) 0 or 1.
  const uint32_t data = w.block(rs::kPauseData);
  guest::StoreU32(base, data + 132, r.Below(9));
  guest::StoreU32(base, data + 144, r.Below(3));
  guest::StoreU32(base, data + 152, Around(r, -1, 12));
  for (const uint32_t field : {36u, 40u, 44u, 48u}) {
    guest::StoreU32(base, data + field, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  for (const uint32_t list : {rs::kPauseList, rs::kAfterList, rs::kDialogList}) {
    for (const uint32_t field : {rs::kListSelection, rs::kListSelectionBefore}) {
      guest::StoreU32(base, w.block(list) + field, r.Below(6) == 0 ? r.Below(21) : r.Below(8));
    }
  }
  // Their last move (+24) -1, 0, 1 or any word, their item words (+68) the
  // misc block's (half of them 0).
  WriteRandom(base, w.misc.address, w.misc.bytes, r);
  for (const uint32_t list : {rs::kPauseList, rs::kAfterList, rs::kDialogList}) {
    const uint32_t moves[4] = {0xFFFFFFFFu, 0, 1, uint32_t(r.Next())};
    guest::StoreU32(base, w.block(list) + rs::kListMove, moves[r.Below(4)]);
    guest::StoreU32(base, w.block(list) + rs::kListBlock, w.misc.address + 0x800);
  }
  for (uint32_t i = 0; i < 64; i++) {
    guest::StoreU32(base, w.misc.address + 0x800 + 4 * i, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
  for (const uint32_t pages : {rs::kPausePages, rs::kAfterPages, rs::kDialogPages}) {
    guest::StoreU32(base, w.block(pages) + 24, r.Below(2));
  }
  // The sound manager fake (the race HUD object's harness's): its vtable's
  // slot 10 (PlayCue2D) the leaf recorder sub_820A8420 (r4's low byte to
  // +320, r5 to +332, r6 to +336) or the getter sub_82090770 (r3 = +60).
  constexpr uint32_t kRecordR4R6 = 0x820A8420, kGet60 = 0x82090770;
  const uint32_t manager = w.misc.address, vtable = w.misc.address + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot,
                  r.Below(3) == 0 ? kGet60 : kRecordR4R6);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
  // Player 0's save data and the car (PauseMenu::LoadData), as the data's
  // cases shape them: the win streak (the stats' halfword +5970 of the copy
  // from +48), the day (+6100), the car's two floats.
  WriteRandom(base, w.save.address, w.save.bytes, r);
  WriteRandom(base, w.vehicle.address, w.vehicle.bytes, r);
  guest::StoreU32(base, kSaveDataPointers, w.save.address);
  guest::StoreU32(base, kVehiclesPointer, w.vehicle.address);
  const uint16_t streaks[4] = {uint16_t(r.Below(1000)), uint16_t(0x8000 | r.Below(0x8000)),
                               uint16_t(1000 + r.Below(5000)), 999};
  guest::StoreU16(base, w.save.address + 48 + 5970, streaks[r.Below(4)]);
  const uint32_t days[4] = {r.Below(400), uint32_t(-int32_t(r.Below(50))), uint32_t(r.Next()),
                            1000};
  guest::StoreU32(base, w.save.address + 6100, days[r.Below(4)]);
  guest::StoreU32(base, w.vehicle.address + 6248, AnyFloatBits(r, 400.0f));
  guest::StoreU32(base, w.vehicle.address + 6244, AnyFloatBits(r, 400.0f));
}

// UiMovie::Run's update calls, kept and answered.
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall>* g_updates = nullptr;
Random g_update_answers;

bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_updates == nullptr || function != 0x8211D2F8) {
    return false;
  }
  g_updates->push_back({uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_update_answers.Below(2) == 0 ? 0 : g_update_answers.Next();
  return true;
}

using MovieSetupFn = std::function<void(PPCContext&, Random&, const MovieWorld&)>;

// What a case compares of the function's frame: [sp - frame, sp - saves)
// less `skip` (offsets from the frame's bottom: SetText's varargs stores).
struct FrameSpec {
  uint32_t frame = 0, saves = 0;
  std::pair<uint32_t, uint32_t> skip = {0, 0};
  bool own_stack = false;  // the world's stack (PauseMenu::Open)
  // The pause menu's list moved to a menu list world of the first section's
  // (World: its list, the action mappers and their vtables, ShapeWorld) with
  // 5 items shown from 0 and the selection 0 to 4, for the pages that run
  // MenuList::Update and its tests; that world compared too.
  const World* list_world = nullptr;
  uint32_t list_global = race_screens::kPauseList;  // the global it's moved to
  // Run after the function, before the outputs are read (frees, and words
  // made comparable), with the case's outputs to add to.
  std::function<void(Outputs&)> after;
};

void MovieGroup(Context& h, const MovieWorld& w, const char* name, int cases,
                const MovieSetupFn& setup, GuestFunction recompiled, GuestFunction native,
                const FrameSpec& spec) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      SavedBytes saved(h.base, {{kScreenGlobals, kScreenGlobalsBytes},
                                {kUnseenGlobals, kUnseenGlobalsBytes},
                                {ui_movies::kMovieTable, 12},
                                {kMissingMovie, 4},
                                {kMissingText, 4},
                                {kStringTable, 4},
                                {menu_sounds::kSoundManager, 4},
                                {kSaveDataPointers, 4},
                                {kVehiclesPointer, 4},
                                {rs::kMapperEntries, 32},
                                {rs::kControllerEntries, 16},
                                {kSaveInterfaceGlobal, 4},
                                {kReplayList, 4}});
      ShapeMovieWorld(h.base, w, r);
      if (spec.list_world != nullptr) {
        const World& lw = *spec.list_world;
        WriteRandom(h.base, lw.misc.address, lw.misc.bytes, r);
        ShapeWorld(h.base, lw, r);
        guest::StoreU32(h.base, lw.list() + rs::kListCount, 5);
        guest::StoreU32(h.base, lw.list() + rs::kListShown, 5);
        guest::StoreU32(h.base, lw.list() + rs::kListScroll, 0);
        guest::StoreU32(h.base, lw.list() + rs::kListSelection, r.Below(5));
        guest::StoreU32(h.base, lw.list() + rs::kListSelectionBefore, r.Below(5));
        guest::StoreU32(h.base, lw.list() + rs::kListSide,
                        r.Below(3) == 0 ? 0 : (r.Below(2) == 0 ? 1 : 0xFFFFFFFFu));
        guest::StoreU32(h.base, spec.list_global, lw.list());
      }
      PPCContext t = CallContext(h);
      if (spec.own_stack) {
        WriteRandom(h.base, w.stack.address, w.stack.bytes, r);
        t.r1.u64 = w.stack.address + kOwnStackTop;
      }
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_update_answers = Random(r.Next());
      std::vector<UpdateCall> updates;
      g_updates = &updates;
      const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      ITC_FLAG(native_race_screens) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_race_screens) = true;
      ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
      g_updates = nullptr;
      if (spec.after) {
        spec.after(out[v]);
      }
      out[v].Word(uint32_t(updates.size()));
      for (const UpdateCall& u : updates) {
        out[v].Word(u.lr);
        out[v].Word(u.r1);
        for (const uint64_t x : {u.r3, u.r4}) {
          out[v].Word(uint32_t(x >> 32));
          out[v].Word(uint32_t(x));
        }
      }
      out[v].Bytes(Read(h.base, w.blocks.address, w.blocks.bytes));
      out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(h.base, w.texts.address, w.texts.bytes));
      out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
      if (spec.list_world != nullptr) {
        out[v].Bytes(Read(h.base, spec.list_world->misc.address, spec.list_world->misc.bytes));
        out[v].Bytes(Read(h.base, rs::kMapperEntries, 32));
      }
      out[v].Bytes(Read(h.base, kScreenGlobals, kScreenGlobalsBytes));
      const uint32_t bottom = sp - spec.frame, top = sp - spec.saves;
      if (spec.skip.second > spec.skip.first) {
        out[v].Bytes(Read(h.base, bottom, spec.skip.first));
        out[v].Bytes(Read(h.base, bottom + spec.skip.second, top - bottom - spec.skip.second));
      } else if (top > bottom) {
        out[v].Bytes(Read(h.base, bottom, top - bottom));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The transitions: r3 a page object's step word.
void StepWord(PPCContext& t, Random& r, const MovieWorld& w, uint32_t pages) {
  t.r3.u64 = w.block(pages) + 24;
  (void)r;
}

// The race HUD machine's pause menu openers (race-hud-machine.md), whose
// callee PauseMenu::Open runs for real in this world: the machine (+9612
// the kind -1 to 6, now and then any word; +9608 the holder; +9616 any),
// the holder (+8 the replay bit 0 or 1, +1012 the pause menu's screen
// word), the race data's +0xA3AF0 (the race kind 0 to 2) and +0xA3AF4 (0,
// 1 or any), the save data's day (+6100) 1 half the time (IsFirstDay).
// Kinds 4 and 6 without the replay bit (the original runs there: kind 4's
// and the stage's openers, never seen in play) are left out. The originals
// run with --native_race_screens off (PauseMenu::Open's original), ours
// with Open's plain C++.
void Openers(Context& h, const MovieWorld& w) {
  constexpr uint32_t kRaceData = 0x826C5B2C;
  const Block machine(9620 + 12);
  const Block holder(0x420);
  const Block race(0xA3B00);
  if (!machine.address || !holder.address || !race.address) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  auto shape = [&](PPCContext& t, Random& r, const MovieWorld& w, bool racing) {
    WriteRandom(h.base, machine.address, machine.bytes, r);
    WriteRandom(h.base, holder.address, holder.bytes, r);
    int32_t kind = r.Below(10) == 0 ? int32_t(r.Next()) : int32_t(r.Below(8)) - 1;
    const uint32_t bit = r.Below(2);
    if (kind == 4 || (kind == 6 && (!racing || bit == 0))) {
      kind = 1;
    }
    guest::StoreU32(h.base, machine.address + 9612, uint32_t(kind));
    guest::StoreU32(h.base, machine.address + 9608, holder.address);
    guest::StoreU32(h.base, holder.address + 8, (uint32_t(r.Next()) & ~1u) | bit);
    guest::StoreU32(h.base, holder.address + 1012, w.block(rs::kPauseScreen));
    guest::StoreU32(h.base, kRaceData, race.address);
    guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(3));
    guest::StoreU32(h.base, race.address + 0xA3AF4,
                    r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
    if (r.Below(2) == 0) {
      guest::StoreU32(h.base, w.save.address + 6100, 1);
    }
    t.r3.u64 = machine.address;
  };
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  FrameSpec open{112, 16, {0, 0}, true};
  open.after = [&](Outputs& out) {
    out.Bytes(Read(h.base, machine.address, machine.bytes));
    out.Bytes(Read(h.base, holder.address, holder.bytes));
  };
  MovieGroup(
      h, w, "RaceHudMachine::OpenPause (sub_8216D600)", 150,
      [&](PPCContext& t, Random& r, const MovieWorld& w) { shape(t, r, w, false); },
      __imp__sub_8216D600, hooks::RaceHudMachineOpenPause, open);
  // OpenRacePause's career path runs OpenCareerPause's 96-byte frame first
  // (Open's below it, its register saves at sp - 112 to sp - 96): the
  // frame's lowest 16 bytes (Open's back chain on the other paths) left
  // out.
  FrameSpec racing = open;
  racing.skip = {0, 16};
  MovieGroup(
      h, w, "RaceHudMachine::OpenRacePause (sub_8216DA40)", 200,
      [&](PPCContext& t, Random& r, const MovieWorld& w) { shape(t, r, w, true); },
      __imp__sub_8216DA40, hooks::RaceHudMachineOpenRacePause, racing);
  FrameSpec career{96, 16, {0, 0}, true};
  career.after = open.after;
  MovieGroup(
      h, w, "RaceHudMachine::OpenCareerPause (sub_8216D8C8)", 100,
      [&](PPCContext& t, Random& r, const MovieWorld& w) { shape(t, r, w, false); },
      __imp__sub_8216D8C8, hooks::RaceHudMachineOpenCareerPause, career);
  guest::StoreU32(h.base, kRaceData, saved_race);
}

// The career's results' opener and texts, and the race HUD machine's
// results at the race's end (race-hud-machine.md), whose callees
// (CareerResults::Open with SetTexts, AfterRaceMenu::Open) run for real in
// this world, the originals with --native_race_screens off and ours with
// their plain C++.
//
// SetTexts and Open: the screen [0x826D8500]'s block (the kind +0 0 to 3 or
// any, the values +4 to +28 and +164 small, either sign, or any; the floats
// +12 and +24 any) with its movie (+180) movie 6's; Open's source block one
// of our own (random words shaped as those), compared. SetTexts' frame's +48
// to +79 left out (SetText stores its r7 to r10 there: the float's double,
// then leftovers in the original and 0 in ours, the user's call for the
// parking area, 2026-10-30).
//
// AwardPrize and OnRaceEnd: the machine (+9612 the kind, 1 for AwardPrize
// without the replay bit, where SaveData::Of(0) is read; +9608 the holder),
// the holder (+8 the replay bit 0 or 1, +1020 the menu after the race's
// screen, +1024 the career's results' block), the race data (its +0xA3AF0
// race kind 0 to 2, +0xA3AF4 mode 0 or not; the stage list's current stage
// +804, count +800 and word +808; the racer records' count 1 to 3 and the
// last one's entries: +0 the count 0 to 4, their item ids -1 or 370 to 409,
// the floats +828, +844 and +852 on (0.0 now and then) with the halfword
// +968 0 to 17 (more would write the pairs past the block, onto its own
// count), +3920 1, 3 or another, +3924 1 to 3 or another), player 0's save
// data (the day's win streak +72 + 4828 around the factors' bounds, the
// day +6100 1 half the time, the money +6116 and points +48 + 6024 small
// or near 9,999,999 or any; the items' records +0x199F8, 12 bytes each) and
// the data tables' items (370 to 409; their bytes +35 and +37 random).
// OnRaceEnd's kinds 2, 4, 6 and 1 with the mode 0 (the results never seen
// in play, ShowResults, ShowSplitResults and ShowKind6Results: the
// originals, whose worlds this one doesn't hold) are left out. Both run on
// a stack of the world's (their callees' frames below theirs). The menu
// after the race's list's item block (+68, 0 before) compared and freed.
// The originals of AwardPrize and OnRaceEnd with --native_race_hud_machine
// off as well, so their callees' originals run (OpponentPrize, ItemBonus,
// and OnRaceEnd's AwardPrize).
template <GuestFunction original>
void WithoutMachine(PPCContext& ctx, uint8_t* base) {
  ITC_FLAG(native_race_hud_machine) = false;
  original(ctx, base);
  ITC_FLAG(native_race_hud_machine) = true;
}

void Results(Context& h, const MovieWorld& w) {
  constexpr uint32_t kRaceData = 0x826C5B2C;
  constexpr uint32_t kFirstItem = 370, kItems = 40;
  const Block machine(9620 + 12);
  const Block holder(0x420);
  const Block race(0xA3B00);
  const Block source(0x100);
  const Block items(3008 * kItems + 0x40);
  if (!machine.address || !holder.address || !race.address || !source.address ||
      !items.address) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  // A screen block's words: the kind, the values, the two floats.
  auto values = [&h](Random& r, uint32_t at) {
    guest::StoreU32(h.base, at + 0, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
    for (const uint32_t field : {4u, 8u, 16u, 20u, 28u, 164u}) {
      const uint32_t picks[4] = {r.Below(100000), uint32_t(-int32_t(r.Below(100000))),
                                 uint32_t(r.Next()), 0};
      guest::StoreU32(h.base, at + field, picks[r.Below(4)]);
    }
    for (const uint32_t field : {12u, 24u}) {
      guest::StoreU32(h.base, at + field, AnyFloatBits(r, 100.0f));
    }
  };
  auto screen = [&](Random& r, const MovieWorld& w) {
    const uint32_t at = w.block(rs::kCareerResults);
    values(r, at);
    guest::StoreU32(h.base, at + 180, w.movie(2));
    return at;
  };
  FrameSpec set_texts{128, 40, {48, 80}, true};
  MovieGroup(
      h, w, "CareerResults::SetTexts (sub_822197A8)", 150,
      [&](PPCContext& t, Random& r, const MovieWorld& w) { t.r3.u64 = screen(r, w); },
      __imp__sub_822197A8, hooks::RaceScreensCareerResultsSetTexts, set_texts);
  FrameSpec open{112, 24, {0, 0}, true};
  open.after = [&](Outputs& out) { out.Bytes(Read(h.base, source.address, source.bytes)); };
  MovieGroup(
      h, w, "CareerResults::Open (sub_8221A1F0)", 150,
      [&](PPCContext& t, Random& r, const MovieWorld& w) {
        t.r3.u64 = screen(r, w);
        WriteRandom(h.base, source.address, source.bytes, r);
        values(r, source.address);
        t.r4.u64 = source.address;
      },
      __imp__sub_8221A1F0, hooks::RaceScreensCareerResultsOpen, open);

  // The race HUD machine's results.
  auto shape = [&](PPCContext& t, Random& r, const MovieWorld& w, bool award) {
    WriteRandom(h.base, machine.address, machine.bytes, r);
    WriteRandom(h.base, holder.address, holder.bytes, r);
    WriteRandom(h.base, race.address + 0x59600, 0x4A500, r);
    WriteRandom(h.base, items.address, items.bytes, r);
    const uint32_t bit = r.Below(4) == 0 ? 1 : 0;
    int32_t kind = 1;
    if (!award) {
      const int32_t kinds[7] = {1, 1, 1, -1, 0, 3, 5};
      kind = r.Below(8) == 0 ? int32_t(r.Next()) : kinds[r.Below(7)];
      if (kind == 2 || kind == 4 || kind == 6) {
        kind = 1;
      }
    } else if (bit == 1 && r.Below(2) == 0) {
      kind = int32_t(r.Below(8));
    }
    guest::StoreU32(h.base, machine.address + 9612, uint32_t(kind));
    guest::StoreU32(h.base, machine.address + 9608, holder.address);
    guest::StoreU32(h.base, holder.address + 8, (uint32_t(r.Next()) & ~1u) | bit);
    guest::StoreU32(h.base, holder.address + 1020, w.block(rs::kAfterScreen));
    guest::StoreU32(h.base, holder.address + 1024, w.block(rs::kCareerResults));
    guest::StoreU32(h.base, w.block(rs::kAfterList) + rs::kListBlock, 0);
    guest::StoreU32(h.base, kRaceData, race.address);
    guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(3));
    // OnRaceEnd's kind 1 with the mode 0 runs ShowResults: left out.
    const bool mode_zero = r.Below(3) == 0 && award;
    guest::StoreU32(h.base, race.address + 0xA3AF4,
                    mode_zero ? 0 : (r.Below(4) == 0 ? uint32_t(r.Next()) | 1 : 1 + r.Below(8)));
    const uint32_t stages = race.address + 0x59620;
    guest::StoreU32(h.base, stages + 800, r.Below(6));
    guest::StoreU32(h.base, stages + 804, r.Below(6));
    guest::StoreU32(h.base, stages + 808, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    const uint32_t records = race.address + 0x59AE8;
    const uint32_t count = 1 + r.Below(3);
    guest::StoreU32(h.base, records + 0x49FE0, count);
    const uint32_t result = records + 3936 * (count - 1);
    const uint32_t entries = r.Below(5);
    guest::StoreU32(h.base, result + 0, entries);
    for (uint32_t i = 0; i + 1 < entries; i++) {
      guest::StoreU32(h.base, result + 980 + 972 * i,
                      r.Below(5) == 0 ? 0xFFFFFFFFu : kFirstItem + r.Below(kItems));
    }
    auto any_float = [&r](float range) {
      return r.Below(5) == 0 ? 0u : AnyFloatBits(r, range);
    };
    guest::StoreU32(h.base, result + 828, any_float(1.0e7f));
    guest::StoreU32(h.base, result + 844, any_float(100.0f));
    for (uint32_t k = 0; k < 17; k++) {
      guest::StoreU32(h.base, result + 852 + 8 * k, any_float(100.0f));
    }
    guest::StoreU16(h.base, result + 968, uint16_t(r.Below(18)));
    const uint32_t outcomes[5] = {1, 1, 3, 0, uint32_t(r.Next())};
    guest::StoreU32(h.base, result + 3920, outcomes[r.Below(5)]);
    const uint32_t places[5] = {1, 1, 2, 3, r.Below(2) == 0 ? 0 : uint32_t(r.Next())};
    guest::StoreU32(h.base, result + 3924, places[r.Below(5)]);
    const uint32_t save = w.save.address;
    const uint16_t streaks[5] = {uint16_t(r.Below(25)), uint16_t(r.Below(25)),
                                 uint16_t(r.Below(0x10000)), 20, 3};
    guest::StoreU16(h.base, save + 72 + 4828, streaks[r.Below(5)]);
    guest::StoreU32(h.base, save + 6100, r.Below(2) == 0 ? 1 : r.Below(4));
    for (const uint32_t field : {6116u, 48u + 6024u}) {
      const uint32_t picks[4] = {r.Below(1000000), 9999999 - r.Below(1000), uint32_t(r.Next()),
                                 0};
      guest::StoreU32(h.base, save + field, picks[r.Below(4)]);
    }
    for (uint32_t i = 0; i < kItems; i++) {
      const uint32_t record = save + 0x199F8 + 12 * (kFirstItem + i);
      guest::StoreU16(h.base, record + 2, r.Below(2) == 0 ? 1 : uint16_t(r.Below(4)));
    }
    const uint32_t tables = items.address + 3008 * kItems;
    guest::StoreU32(h.base, data_tables::kTables, tables);
    guest::StoreU32(h.base, tables + 16, items.address - 3008 * kFirstItem);
    t.r3.u64 = machine.address;
  };
  const uint32_t saved_race = guest::LoadU32(h.base, kRaceData);
  const uint32_t saved_tables = guest::LoadU32(h.base, data_tables::kTables);
  auto after = [&](Outputs& out) {
    out.Bytes(Read(h.base, machine.address, machine.bytes));
    out.Bytes(Read(h.base, holder.address, holder.bytes));
    out.Bytes(Read(h.base, race.address + 0x59600, 0x4A500));
    out.Bytes(Read(h.base, w.save.address, w.save.bytes));
    const uint32_t list = w.block(rs::kAfterList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 4 * 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  // __savegprlr_26 and f30, f31: the 72 bytes below sp.
  FrameSpec award{336, 72, {0, 0}, true};
  award.after = after;
  MovieGroup(
      h, w, "RaceHudMachine::AwardPrize (sub_8216E150)", 300,
      [&](PPCContext& t, Random& r, const MovieWorld& w) { shape(t, r, w, true); },
      WithoutMachine<__imp__sub_8216E150>, hooks::RaceHudMachineAwardPrize, award);
  FrameSpec end{96, 16, {0, 0}, true};
  end.after = after;
  MovieGroup(
      h, w, "RaceHudMachine::OnRaceEnd (sub_8216E5B0)", 200,
      [&](PPCContext& t, Random& r, const MovieWorld& w) { shape(t, r, w, false); },
      WithoutMachine<__imp__sub_8216E5B0>, hooks::RaceHudMachineOnRaceEnd, end);
  guest::StoreU32(h.base, kRaceData, saved_race);
  guest::StoreU32(h.base, data_tables::kTables, saved_tables);
}

// The menu after the race's pages and transitions: its data's kind (+0) 0
// to 7, item count (+4) 0 to 4, flag (+8) 0, 1 or any, selection (+16) 0
// to 3; its list's selections as the pause menu's. Init's item block (+68,
// 0 before) compared and freed as SetUpOptions' cases do.
void AfterRace(Context& h, const MovieWorld& w) {
  auto data = [&h](Random& r, const MovieWorld& w) {
    const uint32_t d = w.block(rs::kAfterData);
    guest::StoreU32(h.base, d + 0, r.Below(8));
    guest::StoreU32(h.base, d + 4, r.Below(5));
    guest::StoreU32(h.base, d + 8, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
    guest::StoreU32(h.base, d + 16, r.Below(4));
  };
  auto after_step = [&](PPCContext& t, Random& r, const MovieWorld& w) {
    data(r, w);
    StepWord(t, r, w, rs::kAfterPages);
  };
  MovieGroup(
      h, w, "AfterRaceMenu::ConfirmPage (sub_8221A6C8)", 120,
      [&h](PPCContext&, Random& r, const MovieWorld& w) {
        const uint32_t pages = w.block(rs::kDialogPages);
        const uint32_t dialog = w.block(rs::kDialogData);
        const int32_t choices[7] = {-1, 5, 0, 1, 2, 3, 4};
        const int32_t page = choices[r.Below(7)];
        guest::StoreU32(h.base, pages, uint32_t(page));
        guest::StoreU32(h.base, pages + 12, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3));
        guest::StoreU32(h.base, dialog + 36, page != -1 && page != 5 ? 1 : r.Below(2));
        guest::StoreU32(h.base, dialog + 0, r.Below(3));
      },
      __imp__sub_8221A6C8, hooks::RaceScreensAfterRaceMenuConfirmPage, {96, 8});
  MovieGroup(h, w, "AfterRaceMenu::Intro (sub_8221A730)", 100, after_step, __imp__sub_8221A730,
             hooks::RaceScreensAfterRaceMenuIntro, {96, 16});
  MovieGroup(h, w, "AfterRaceMenu::Show (sub_8221A7A8)", 100, after_step, __imp__sub_8221A7A8,
             hooks::RaceScreensAfterRaceMenuShow, {112, 24});
  MovieGroup(h, w, "AfterRaceMenu::CloseMain (sub_8221A950)", 100, after_step,
             __imp__sub_8221A950, hooks::RaceScreensAfterRaceMenuCloseMain, {112, 24});
  MovieGroup(h, w, "AfterRaceMenu::Move (sub_8221AA20)", 100, after_step, __imp__sub_8221AA20,
             hooks::RaceScreensAfterRaceMenuMove, {112, 32});
  MovieGroup(
      h, w, "AfterRaceMenu::ShowItems (sub_8221AC00)", 150,
      [&](PPCContext&, Random& r, const MovieWorld& w) { data(r, w); }, __imp__sub_8221AC00,
      hooks::RaceScreensAfterRaceMenuShowItems, {128, 48, {48, 80}});
  MovieGroup(
      h, w, "AfterRaceMenu::ColorItems (sub_8221B768)", 100,
      [&](PPCContext& t, Random& r, const MovieWorld& w) {
        data(r, w);
        t.r3.u64 = w.block(rs::kAfterScreen);
      },
      __imp__sub_8221B768, hooks::RaceScreensAfterRaceMenuColorItems, {128, 48});
  MovieGroup(
      h, w, "AfterRaceMenu::Start (sub_8221B860)", 60,
      [&](PPCContext&, Random& r, const MovieWorld& w) { data(r, w); }, __imp__sub_8221B860,
      hooks::RaceScreensAfterRaceMenuStart, {96, 8});
  // Init's block: compared, freed, +68 0 again.
  auto init_block = [&](Outputs& out) {
    const uint32_t list = w.block(rs::kAfterList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 4 * 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  FrameSpec set_up{144, 40};
  set_up.after = init_block;
  MovieGroup(
      h, w, "AfterRaceMenu::SetUpItems (sub_8221B5C8)", 150,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        data(r, w);
        guest::StoreU32(h.base, w.block(rs::kAfterList) + rs::kListBlock, 0);
      },
      __imp__sub_8221B5C8, hooks::RaceScreensAfterRaceMenuSetUpItems, set_up);
  FrameSpec open{128, 40};
  open.after = init_block;
  MovieGroup(
      h, w, "AfterRaceMenu::Open (sub_8221BA68)", 150,
      [&](PPCContext& t, Random& r, const MovieWorld& w) {
        data(r, w);
        guest::StoreU32(h.base, w.block(rs::kAfterList) + rs::kListBlock, 0);
        t.r3.u64 = w.block(rs::kAfterScreen);
        t.r4.u64 = r.Below(8);
        t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
      },
      __imp__sub_8221BA68, hooks::RaceScreensAfterRaceMenuOpen, open);
}

// The confirm dialog's pages, transitions and draw.
//
// Question's callees reach SaveSlots::RequestDevice (the save system's
// interface) and Replays::Delete (the replay list): both pointed at a block
// of the case's own, built as the save interface's harness builds its
// world (the interface, its +36 object, the save devices, the users and
// three 1,000-byte slots, with the game's vtables), so the game's own
// objects aren't touched; the replay list's 0 to 2 entries never match the
// dialog's replay (+4), so SaveInterface::Delete isn't reached. The block
// is compared. The list moved to a menu list world (MenuList::Update and
// its tests) as the options page's cases do.
constexpr uint32_t kSaveWorldBytes = 0x3000;
constexpr uint32_t kSaveObject36 = 0x40, kSaveDevices = 0x100, kSaveUsers = 0x200;
constexpr uint32_t kSaveSlots = 0x800, kReplayHeader = 0x2000, kReplayEntries = 0x2040;

void ShapeSaveWorld(uint8_t* base, uint32_t block, Random& r) {
  WriteRandom(base, block, kSaveWorldBytes, r);
  guest::StoreU32(base, block, 0x82000E38);
  guest::StoreU32(base, block + 8, block + kSaveObject36);
  guest::StoreU32(base, block + kSaveObject36, 0x82000AFC);
  guest::StoreU32(base, block + kSaveObject36 + 88, block + kSaveDevices);
  guest::StoreU32(base, block + kSaveObject36 + 92, block + kSaveUsers);
  guest::StoreU32(base, block + kSaveDevices, 0x82002198);
  guest::StoreU32(base, block + kSaveDevices + 44, block + kSaveSlots);
  guest::StoreU32(base, block + kSaveUsers, 0x82000FA0);
  guest::StoreU32(base, block + kSaveUsers + 216, r.Below(6));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, block + kSaveUsers + 200 + 4 * i, r.Below(4));
    guest::StoreU32(base, block + kSaveUsers + 8 + 40 * i, r.Below(3) == 0 ? 0 : r.Below(5));
  }
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t slot = block + kSaveSlots + 1000 * k;
    guest::StoreU32(base, slot + 988, r.Below(2) == 0 ? 3 : r.Below(12));
    for (uint32_t word = 960; word <= 980; word += 4) {
      guest::StoreU32(base, slot + word, r.Below(2));
    }
  }
  guest::StoreU32(base, kSaveInterfaceGlobal, block);
}

void Dialog(Context& h, const MovieWorld& w) {
  const Block save(kSaveWorldBytes);
  const World list_world;
  if (save.address == 0 || !list_world.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  // The data: the mode (+12) the question's and the save flow's (1, 4, 5,
  // 6, 8, 13) or any small one, the animation's (+16) 0 to 3 (now and then
  // any word) and the next (+20), the replay (+4).
  auto data = [&h](Random& r, const MovieWorld& w) {
    const uint32_t d = w.block(rs::kDialogData);
    const uint32_t modes[8] = {1, 4, 5, 6, 8, 13, r.Below(16), r.Below(16)};
    guest::StoreU32(h.base, d + 12, modes[r.Below(8)]);
    guest::StoreU32(h.base, d + 16, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
    guest::StoreU32(h.base, d + 20, r.Below(4));
    guest::StoreU32(h.base, d + 4, r.Below(100));
  };
  FrameSpec question{112, 32};
  question.list_world = &list_world;
  question.list_global = rs::kDialogList;
  question.after = [&](Outputs& out) {
    out.Bytes(Read(h.base, save.address, kSaveWorldBytes));
  };
  MovieGroup(
      h, w, "RaceDialog::Question (sub_82217F40)", 400,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        data(r, w);
        ShapeSaveWorld(h.base, save.address, r);
        const uint32_t replay = guest::LoadU32(h.base, w.block(rs::kDialogData) + 4);
        const uint32_t count = r.Below(3);
        guest::StoreU32(h.base, save.address + kReplayHeader + 16, save.address + kReplayEntries);
        guest::StoreU32(h.base, save.address + kReplayHeader + 20, count);
        for (uint32_t k = 0; k < count; k++) {
          guest::StoreU32(h.base, save.address + kReplayEntries + 304 * k + 300,
                          replay + 1 + k);
        }
        guest::StoreU32(h.base, kReplayList, save.address + kReplayHeader);
      },
      __imp__sub_82217F40, hooks::RaceScreensRaceDialogQuestion, question);
  auto step = [&](PPCContext& t, Random& r, const MovieWorld& w) {
    data(r, w);
    StepWord(t, r, w, rs::kDialogPages);
  };
  MovieGroup(h, w, "RaceDialog::OpenAnim (sub_822186A8)", 150, step, __imp__sub_822186A8,
             hooks::RaceScreensRaceDialogOpenAnim, {128, 32});
  MovieGroup(h, w, "RaceDialog::Move (sub_82218788)", 100, step, __imp__sub_82218788,
             hooks::RaceScreensRaceDialogMove, {128, 32});
  // ShowMessage: the step word 0 or 1 (the world's), the message (+12) the
  // question's (13) mostly, now and then any word or 0 to 11 (the jump
  // table's messages, never seen in play: the original on both sides), the
  // mode (+16) 0 to 3 or any (data's), the selection to set (+28) 0 to 2 or
  // any; the dialog list's item block (+68) 0 first, MenuList::Init's then
  // compared and freed.
  FrameSpec show{160, 48};
  show.after = [&](Outputs& out) {
    const uint32_t list = w.block(rs::kDialogList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 2 * 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  MovieGroup(
      h, w, "RaceDialog::ShowMessage (sub_82218C10)", 200,
      [&](PPCContext& t, Random& r, const MovieWorld& w) {
        data(r, w);
        StepWord(t, r, w, rs::kDialogPages);
        const uint32_t d = w.block(rs::kDialogData);
        const uint32_t messages[6] = {13, 13, 13, 13, uint32_t(r.Next()), r.Below(12)};
        guest::StoreU32(h.base, d + 12, messages[r.Below(6)]);
        guest::StoreU32(h.base, d + 28, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(3));
        guest::StoreU32(h.base, w.block(rs::kDialogList) + rs::kListBlock, 0);
      },
      __imp__sub_82218C10, hooks::RaceScreensRaceDialogShowMessage, show);
  // Draw: the running transition (+12) none (-1), OpenAnim's (1), Move's (2)
  // or another negative one (ShowMessage's, 0, never seen in play, left out:
  // the original on both sides), the queued one (+16) -1 or 0 to 2.
  MovieGroup(
      h, w, "RaceDialog::Draw (sub_82218F30)", 200,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        data(r, w);
        const uint32_t pages = w.block(rs::kDialogPages);
        const uint32_t transitions[5] = {0xFFFFFFFFu, 1, 2, uint32_t(-2 - int32_t(r.Below(50))),
                                         0xFFFFFFFFu};
        guest::StoreU32(h.base, pages + 12, transitions[r.Below(5)]);
        guest::StoreU32(h.base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3));
        guest::StoreU32(h.base, pages + 4, r.Below(8));
      },
      __imp__sub_82218F30, hooks::RaceScreensRaceDialogDraw, {128, 16});
  // SetText: the string id one its callers pass (62, 75, 77, 1954 to 1956)
  // or any below 2000, the varargs random words (the strings have no
  // argument codes). The frame's +40 to +79 left out: UiMovie::SetNodeText
  // stores its r6 to r10 there, leftovers of SetNodeTextV in the original
  // and 0 in ours (the user's call for the parking area, 2026-10-30).
  FrameSpec set_text{144, 48};
  set_text.skip = {40, 80};
  MovieGroup(
      h, w, "RaceDialog::SetText (sub_82218838)", 100,
      [&](PPCContext& t, Random& r, const MovieWorld&) {
        const uint32_t ids[7] = {62, 75, 77, 1954, 1955, 1956, r.Below(2000)};
        t.r3.u64 = ids[r.Below(7)];
        t.r4.u64 = r.Next();
        t.r5.u64 = r.Next();
        t.r6.u64 = r.Next();
        t.r7.u64 = r.Next();
        t.r8.u64 = r.Next();
        t.r9.u64 = r.Next();
        t.r10.u64 = r.Next();
      },
      __imp__sub_82218838, hooks::RaceScreensRaceDialogSetText, set_text);
  // Open: the mode 0 to 5 or any up to 7, the player 0, 1 or any word. The
  // frame's +32 to +79 left out: SetText stores r5 to r10 there, the
  // caller's leftovers in the original and 0 in ours (the user's call for
  // the parking area, 2026-10-30).
  FrameSpec open{128, 32};
  open.skip = {32, 80};
  MovieGroup(
      h, w, "RaceDialog::Open (sub_82218B30)", 100,
      [&](PPCContext& t, Random& r, const MovieWorld&) {
        t.r3.u64 = r.Below(8);
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
      },
      __imp__sub_82218B30, hooks::RaceScreensRaceDialogOpen, open);
}

// The two main pages (PauseMenu::MainPage, AfterRaceMenu::MainPage), then
// the pause menu's set-ups and adjust page: the pages' lists moved to a
// menu list world (MenuList::Update and its tests) as the options page's
// cases do, the items read from the image's tables by the data's kind (0
// to 8, the world's) and the selection.
//
// The pause menu's: the item count (+136) 0 to 6; the race data a block of
// the case's own (the current stage +804 of the stage list at +0x59620, 0 to 9, its first word 1 or
// any; the race kind +0xA3AF0 6 or 0 to 7) and the word at 0x826C7824 0 or
// any, for the end-race item 4 (Stages::Copy into the frame's +80).
//
// The menu after the race's: its data's kind (+0) 0 to 7; the replay's save
// (item 2) runs SaveBuffers::Create (the replay list's header set, so it
// makes nothing) and RaceDialog::StartSave (the original on both sides)
// over the save world the dialog's cases build; the dialog list's item
// block (+68) 0 first, StartSave's MenuList::Init's then compared and freed.
void MainPages(Context& h, const MovieWorld& w) {
  const World list_world;
  const Block race(0xA3B00);
  const Block save(kSaveWorldBytes);
  if (!list_world.ok() || !race.address || !save.address) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  const uint32_t saved_race = guest::LoadU32(h.base, rs::kRaceData);
  const uint32_t saved_word = guest::LoadU32(h.base, rs::kStageWord);
  FrameSpec pause{208, 48};
  pause.list_world = &list_world;
  MovieGroup(
      h, w, "PauseMenu::MainPage (sub_8220D898)", 400,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        // The item count (+136) 0 to 6, as SetUpMain leaves it: the
        // original's debug texts read the item table up to it (a wild count
        // reads wild addresses).
        guest::StoreU32(h.base, w.block(rs::kPauseData) + 136, r.Below(7));
        const uint32_t stages = race.address + 0x59620;
        WriteRandom(h.base, stages, 80 * 10 + 808, r);
        const uint32_t stage = r.Below(10);
        guest::StoreU32(h.base, stages + 804, stage);
        guest::StoreU32(h.base, stages + 80 * stage, r.Below(2) == 0 ? 1 : uint32_t(r.Next()));
        guest::StoreU32(h.base, race.address + 0xA3AF0, r.Below(2) == 0 ? 6 : r.Below(8));
        guest::StoreU32(h.base, rs::kRaceData, race.address);
        guest::StoreU32(h.base, rs::kStageWord, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
      },
      __imp__sub_8220D898, hooks::RaceScreensPauseMenuMainPage, pause);
  guest::StoreU32(h.base, rs::kRaceData, saved_race);
  guest::StoreU32(h.base, rs::kStageWord, saved_word);
  FrameSpec after{112, 32};
  after.list_world = &list_world;
  after.list_global = rs::kAfterList;
  after.after = [&](Outputs& out) {
    out.Bytes(Read(h.base, save.address, kSaveWorldBytes));
    const uint32_t list = w.block(rs::kDialogList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  MovieGroup(
      h, w, "AfterRaceMenu::MainPage (sub_8221A4A8)", 400,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        guest::StoreU32(h.base, w.block(rs::kAfterData), r.Below(8));
        ShapeSaveWorld(h.base, save.address, r);
        guest::StoreU32(h.base, kReplayList, save.address + kReplayHeader);
        guest::StoreU32(h.base, w.block(rs::kDialogList) + rs::kListBlock, 0);
      },
      __imp__sub_8221A4A8, hooks::RaceScreensAfterRaceMenuMainPage, after);
  // The pause menu's set-ups and the adjust page: the volumes (+52, +56) 0
  // to 15 or any, the three values and their saved and default copies (+24
  // to +32, +60 to +68, +96 to +104) -10 to 10 or any, the toggle (+144) 0
  // to 2, the selection to set (+148) 0 to 7, the disabling flag (+156) 0,
  // 1 or any; the career's numbers (+108 to +128) as the world leaves them.
  auto values = [&h](Random& r, const MovieWorld& w) {
    const uint32_t data = w.block(rs::kPauseData);
    for (const uint32_t field : {52u, 56u}) {
      guest::StoreU32(h.base, data + field, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(16));
    }
    for (const uint32_t field : {24u, 28u, 32u, 60u, 64u, 68u, 96u, 100u, 104u}) {
      guest::StoreU32(h.base, data + field,
                      r.Below(6) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(21)) - 10));
    }
    guest::StoreU32(h.base, data + 144, r.Below(3));
    guest::StoreU32(h.base, data + 148, r.Below(8));
    guest::StoreU32(h.base, data + 156, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  };
  const MovieSetupFn set_values = [&](PPCContext&, Random& r, const MovieWorld& w) {
    values(r, w);
  };
  // ShowCareer: the frame's +48 to +79 left out (SetText stores its r7 to
  // r10 there, leftovers in the original and 0 in ours for strings 2009 to
  // 2011: the user's call for the parking area, 2026-10-30).
  MovieGroup(h, w, "PauseMenu::ShowCareer (sub_8220D418)", 150, set_values, __imp__sub_8220D418,
             hooks::RaceScreensPauseMenuShowCareer, {128, 32, {48, 80}});
  // SetUpMain: MenuList::Init frees the list's old item block (+68, 0 here)
  // and makes one: its words compared, then it's freed and +68 put to 0.
  FrameSpec set_up{144, 40};
  set_up.after = [&](Outputs& out) {
    const uint32_t list = w.block(rs::kPauseList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 6 * 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  MovieGroup(
      h, w, "PauseMenu::SetUpMain (sub_8220E418)", 200,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        values(r, w);
        guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListBlock, 0);
      },
      __imp__sub_8220E418, hooks::RaceScreensPauseMenuSetUpMain, set_up);
  MovieGroup(h, w, "PauseMenu::ShowVolumesB (sub_8220E810)", 100, set_values, __imp__sub_8220E810,
             hooks::RaceScreensPauseMenuShowVolumesB, {112, 24});
  MovieGroup(h, w, "PauseMenu::ShowAdjustB (sub_8220E908)", 100, set_values, __imp__sub_8220E908,
             hooks::RaceScreensPauseMenuShowAdjustB, {128, 32});
  FrameSpec adjust{112, 24};
  adjust.list_world = &list_world;
  MovieGroup(h, w, "PauseMenu::AdjustPage (sub_8220EBD8)", 400, set_values, __imp__sub_8220EBD8,
             hooks::RaceScreensPauseMenuAdjustPage, adjust);
}

// The dialog's page object and data as its draw's cases shape them (its
// running transition none, OpenAnim's, Move's or another negative one).
void ShapeDialogPages(uint8_t* base, Random& r, const MovieWorld& w) {
  const uint32_t pages = w.block(rs::kDialogPages);
  const uint32_t transitions[4] = {0xFFFFFFFFu, 1, 2, uint32_t(-2 - int32_t(r.Below(50)))};
  guest::StoreU32(base, pages + 12, transitions[r.Below(4)]);
  guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3));
  guest::StoreU32(base, w.block(rs::kDialogData) + 16, r.Below(4));
}

// The draws and the career's results.
//
// The screens' draws run the transitions of this file that are native (the
// others, never seen in play or not yet native, are left out: they'd be the
// originals on both sides), a negative transition other than -1 and none;
// the pages the confirm dialog's draw follows; the dialog's page object as
// its draw's cases shape it. The career's results: its screen block (212
// bytes in the global's block) with the kind (+0) 0 to 3, now and then any
// word, +20 around 0, +24 a float around 0.0, the movie (+180) the world's
// id 6, its page object at +184 and step word +208.
void Draws(Context& h, const MovieWorld& w) {
  const World list_world;
  if (!list_world.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  // MenuList::Init's block (OpenOptions' SetUpOptions): +68 0 first, then
  // compared and freed.
  auto pause_block = [&](Outputs& out) {
    const uint32_t list = w.block(rs::kPauseList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 5 * 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  FrameSpec open_options{112, 24};
  open_options.after = pause_block;
  MovieGroup(
      h, w, "PauseMenu::OpenOptions (sub_8220EEF0)", 120,
      [&h](PPCContext& t, Random& r, const MovieWorld& w) {
        guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListBlock, 0);
        StepWord(t, r, w, rs::kPausePages);
      },
      __imp__sub_8220EEF0, hooks::RaceScreensPauseMenuOpenOptions, open_options);
  FrameSpec pause_draw{320, 144};
  pause_draw.after = pause_block;
  // The pause list's item block (+68) 0 only for OpenOptions (6), whose
  // MenuList::Init frees it; the other transitions read the world's item
  // words (ShowMain's ColorItems: with the block 0 the original read guest
  // address 0, a harness gap met once the cases' seeds moved, 2026-11-08).
  const MovieSetupFn pause_draw_setup = [&h](PPCContext& t, Random& r, const MovieWorld& w) {
    const uint32_t pages = w.block(rs::kPausePages);
    const uint32_t transitions[16] = {0xFFFFFFFFu, 0xFFFFFFFFu, uint32_t(-2 - int32_t(r.Below(9))),
                                      0, 2, 3, 4, 5, 6, 7, 8, 9, 12, 17, 18, 19};
    const uint32_t transition = transitions[r.Below(16)];
    guest::StoreU32(h.base, pages + 12, transition);
    if (transition == 6) {
      guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListBlock, 0);
    }
    guest::StoreU32(h.base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(21));
    guest::StoreU32(h.base, pages + 4, r.Below(3) == 0 ? 4 : r.Below(6));
    guest::StoreU32(h.base, pages + 0, r.Below(3) == 0 ? 4 : r.Below(6));
    ShapeDialogPages(h.base, r, w);
    t.r3.u64 = w.block(rs::kPauseScreen);
  };
  MovieGroup(h, w, "PauseMenu::Draw (sub_8220FB10)", 300, pause_draw_setup, __imp__sub_8220FB10,
             hooks::RaceScreensPauseMenuDraw, pause_draw);
  const MovieSetupFn after_draw_setup = [&h](PPCContext& t, Random& r, const MovieWorld& w) {
    const uint32_t d = w.block(rs::kAfterData);
    guest::StoreU32(h.base, d + 0, r.Below(8));
    guest::StoreU32(h.base, d + 4, r.Below(5));
    const uint32_t pages = w.block(rs::kAfterPages);
    const uint32_t transitions[7] = {0xFFFFFFFFu, 0xFFFFFFFFu,
                                     uint32_t(-2 - int32_t(r.Below(9))), 0, 1, 3, 4};
    guest::StoreU32(h.base, pages + 12, transitions[r.Below(7)]);
    guest::StoreU32(h.base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(10));
    guest::StoreU32(h.base, pages + 4, r.Below(8));
    guest::StoreU32(h.base, pages + 0, r.Below(8));
    ShapeDialogPages(h.base, r, w);
    t.r3.u64 = w.block(rs::kAfterScreen);
  };
  MovieGroup(h, w, "AfterRaceMenu::Draw (sub_8221BB48)", 200, after_draw_setup,
             __imp__sub_8221BB48, hooks::RaceScreensAfterRaceMenuDraw, {176, 56});
  // The race HUD object's tail calls to these draws (race-hud-object.md):
  // r3 a holder of the case's own (its words random) whose +1012 and +1020
  // are the screens the draws' cases pass; the holder compared.
  const Block holder(0x420);
  if (!holder.address) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  auto of_holder = [&h, &holder](const MovieSetupFn& setup) -> MovieSetupFn {
    return [&h, &holder, setup](PPCContext& t, Random& r, const MovieWorld& w) {
      setup(t, r, w);
      WriteRandom(h.base, holder.address, holder.bytes, r);
      guest::StoreU32(h.base, holder.address + 1012, w.block(rs::kPauseScreen));
      guest::StoreU32(h.base, holder.address + 1020, w.block(rs::kAfterScreen));
      t.r3.u64 = holder.address;
    };
  };
  FrameSpec held_pause = pause_draw;
  held_pause.after = [&](Outputs& out) {
    pause_block(out);
    out.Bytes(Read(h.base, holder.address, holder.bytes));
  };
  // The pause menu's list's items (+68) the world's block (ShapeMovieWorld's
  // item words) but for OpenOptions (6), whose MenuList::Init frees the old
  // block: ShowMain's ColorItems reads them on its first call.
  const MovieSetupFn pause_items_setup = [&h, &pause_draw_setup](PPCContext& t, Random& r,
                                                                const MovieWorld& w) {
    pause_draw_setup(t, r, w);
    if (guest::LoadU32(h.base, w.block(rs::kPausePages) + 12) != 6) {
      guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListBlock, w.misc.address + 0x800);
    }
  };
  MovieGroup(h, w, "RaceScreens::DrawPauseMenu (sub_82211D08)", 150, of_holder(pause_items_setup),
             __imp__sub_82211D08, hooks::RaceHudObjectDrawPauseMenu, held_pause);
  FrameSpec held_after{176, 56};
  held_after.after = [&](Outputs& out) { out.Bytes(Read(h.base, holder.address, holder.bytes)); };
  MovieGroup(h, w, "RaceScreens::DrawAfterRaceMenu (sub_82211D10)", 150,
             of_holder(after_draw_setup), __imp__sub_82211D10,
             hooks::RaceHudObjectDrawAfterRaceMenu, held_after);
  auto career = [&h](Random& r, const MovieWorld& w) {
    const uint32_t screen = w.block(rs::kCareerResults);
    guest::StoreU32(h.base, screen + 0, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
    guest::StoreU32(h.base, screen + 20, Around(r, -2, 2));
    guest::StoreU32(h.base, screen + 24,
                    r.Below(3) == 0 ? 0 : AnyFloatBits(r, 1000.0f) ^ (r.Below(2) << 31));
    guest::StoreU32(h.base, screen + 180, w.movie(2));
    guest::StoreU32(h.base, screen + 208, r.Below(2));
    return screen;
  };
  auto career_step = [&](PPCContext& t, Random& r, const MovieWorld& w) {
    t.r3.u64 = career(r, w) + 208;
  };
  MovieGroup(h, w, "CareerResults::Intro (sub_82219210)", 100, career_step, __imp__sub_82219210,
             hooks::RaceScreensCareerResultsIntro, {96, 16});
  MovieGroup(h, w, "CareerResults::Show (sub_822192B0)", 100, career_step, __imp__sub_822192B0,
             hooks::RaceScreensCareerResultsShow, {96, 16});
  MovieGroup(h, w, "CareerResults::Close1 (sub_822193D8)", 100, career_step,
             __imp__sub_822193D8, hooks::RaceScreensCareerResultsClose1, {96, 16});
  // Page1: player 0's mapper from a menu list world of the first section's.
  FrameSpec page1{112, 32};
  page1.list_world = &list_world;
  MovieGroup(
      h, w, "CareerResults::Page1 (sub_82219A78)", 300,
      [&](PPCContext&, Random& r, const MovieWorld& w) { career(r, w); }, __imp__sub_82219A78,
      hooks::RaceScreensCareerResultsPage1, page1);
  // Draw: the running transition (+196) none (-1), another negative one, or
  // the native ones (0 Intro, 1 Show, 6 Close1); the queued one (+200) -1 or
  // 0 to 8.
  MovieGroup(
      h, w, "CareerResults::Draw (sub_8221A2F8)", 200,
      [&](PPCContext& t, Random& r, const MovieWorld& w) {
        const uint32_t screen = career(r, w);
        const uint32_t transitions[6] = {0xFFFFFFFFu, 0xFFFFFFFFu,
                                         uint32_t(-2 - int32_t(r.Below(9))), 0, 1, 6};
        guest::StoreU32(h.base, screen + 196, transitions[r.Below(6)]);
        guest::StoreU32(h.base, screen + 200, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(9));
        guest::StoreU32(h.base, screen + 188, r.Below(5));
        t.r3.u64 = screen;
      },
      __imp__sub_8221A2F8, hooks::RaceScreensCareerResultsDraw, {144, 24});
}

void Transitions(Context& h) {
  const MovieWorld w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  BuildStringTable(h.base, w);
  auto pause_step = [](PPCContext& t, Random& r, const MovieWorld& w) {
    StepWord(t, r, w, rs::kPausePages);
  };
  struct Transition {
    const char* name;
    GuestFunction recompiled, native;
    FrameSpec spec;
  };
  const std::vector<Transition> transitions = {
      {"PauseMenu::RunTen (sub_8220C5E0)", __imp__sub_8220C5E0, hooks::RaceScreensPauseMenuRunTen,
       {96, 16}},
      {"PauseMenu::BackToMain (sub_8220C668)", __imp__sub_8220C668,
       hooks::RaceScreensPauseMenuBackToMain, {112, 32}},
      {"PauseMenu::CloseMain (sub_8220C700)", __imp__sub_8220C700,
       hooks::RaceScreensPauseMenuCloseMain, {112, 24}},
      {"PauseMenu::MoveMain (sub_8220C7B8)", __imp__sub_8220C7B8,
       hooks::RaceScreensPauseMenuMoveMain, {112, 32}},
      {"PauseMenu::ShowOptionsRow (sub_8220C868)", __imp__sub_8220C868,
       hooks::RaceScreensPauseMenuShowOptionsRow, {112, 32}},
      {"PauseMenu::ToggleOptions (sub_8220C930)", __imp__sub_8220C930,
       hooks::RaceScreensPauseMenuToggleOptions, {112, 24}},
      {"PauseMenu::ShowVolumeRow (sub_8220CA28)", __imp__sub_8220CA28,
       hooks::RaceScreensPauseMenuShowVolumeRow, {112, 24}},
      {"PauseMenu::ShowAdjustRow (sub_8220CAB0)", __imp__sub_8220CAB0,
       hooks::RaceScreensPauseMenuShowAdjustRow, {112, 24}},
  };
  for (const Transition& tr : transitions) {
    MovieGroup(h, w, tr.name, 100, pause_step, tr.recompiled, tr.native, tr.spec);
  }
  // ConfirmPage: the dialog's pages as RaceDialog::Update's cases shape them
  // (none, or its answer page 5 directly or after the Go to it).
  MovieGroup(
      h, w, "PauseMenu::ConfirmPage (sub_8220C578)", 120,
      [&h](PPCContext&, Random& r, const MovieWorld& w) {
        const uint32_t pages = w.block(rs::kDialogPages);
        const uint32_t data = w.block(rs::kDialogData);
        const int32_t choices[7] = {-1, 5, 0, 1, 2, 3, 4};
        const int32_t page = choices[r.Below(7)];
        guest::StoreU32(h.base, pages, uint32_t(page));
        guest::StoreU32(h.base, pages + 12, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3));
        guest::StoreU32(h.base, data + 36, page != -1 && page != 5 ? 1 : r.Below(2));
        guest::StoreU32(h.base, data + 0, r.Below(3));
      },
      __imp__sub_8220C578, hooks::RaceScreensPauseMenuConfirmPage, {96, 8});
  // The node helpers.
  MovieGroup(
      h, w, "PauseMenu::ShowItems (sub_8220CB38)", 150,
      [](PPCContext&, Random&, const MovieWorld&) {}, __imp__sub_8220CB38,
      hooks::RaceScreensPauseMenuShowItems, {128, 48, {48, 80}});
  MovieGroup(
      h, w, "PauseMenu::SetArrowA (sub_8220CBF8)", 100,
      [](PPCContext& t, Random& r, const MovieWorld&) {
        const uint64_t values[4] = {0, 120, uint64_t(int64_t(-int32_t(r.Below(1000)))), r.Next()};
        t.r3.u64 = values[r.Below(4)];
      },
      __imp__sub_8220CBF8, hooks::RaceScreensPauseMenuSetArrowA, {112, 24});
  MovieGroup(
      h, w, "PauseMenu::ShowOptions (sub_8220CC60)", 150,
      [](PPCContext&, Random&, const MovieWorld&) {}, __imp__sub_8220CC60,
      hooks::RaceScreensPauseMenuShowOptions, {192, 88});
  MovieGroup(
      h, w, "PauseMenu::ShowVolumes (sub_8220D0B8)", 100,
      [&h](PPCContext&, Random& r, const MovieWorld& w) {
        for (const uint32_t field : {52u, 56u}) {
          guest::StoreU32(h.base, w.block(rs::kPauseData) + field, r.Below(16));
        }
      },
      __imp__sub_8220D0B8, hooks::RaceScreensPauseMenuShowVolumes, {112, 24});
  // ShowSlider: r3 a leaf, r4 the value -10 to 10, now and then any word
  // (SetNodeText's unset r7 to r10 stored at its frame's +48 to +79 left out).
  MovieGroup(
      h, w, "PauseMenu::ShowSlider (sub_8220D130)", 150,
      [](PPCContext& t, Random& r, const MovieWorld& w) {
        t.r3.u64 = w.leaf(r.Below(kLeaves));
        t.r4.u64 = r.Below(6) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(21)) - 10));
      },
      __imp__sub_8220D130, hooks::RaceScreensPauseMenuShowSlider, {128, 40, {48, 80}});
  MovieGroup(
      h, w, "PauseMenu::RestoreNodes (sub_8220D5D0)", 150,
      [](PPCContext& t, Random& r, const MovieWorld&) {
        t.r3.u64 = r.Below(3) == 0 ? ~uint64_t(0) : r.Below(16);
        t.r4.u64 = r.Below(16);
      },
      __imp__sub_8220D5D0, hooks::RaceScreensPauseMenuRestoreNodes, {128, 40});
  MovieGroup(h, w, "PauseMenu::MoveOptions (sub_8220DD98)", 200, pause_step,
             __imp__sub_8220DD98, hooks::RaceScreensPauseMenuMoveOptions, {144, 64});
  MovieGroup(h, w, "PauseMenu::MoveAdjust (sub_8220E1F8)", 200, pause_step,
             __imp__sub_8220E1F8, hooks::RaceScreensPauseMenuMoveAdjust, {144, 56});
  MovieGroup(h, w, "PauseMenu::ShowMain (sub_8220EE28)", 100, pause_step, __imp__sub_8220EE28,
             hooks::RaceScreensPauseMenuShowMain, {112, 32});
  MovieGroup(
      h, w, "PauseMenu::ColorItems (sub_8220E700)", 100,
      [](PPCContext&, Random&, const MovieWorld&) {}, __imp__sub_8220E700,
      hooks::RaceScreensPauseMenuColorItems, {128, 48});
  MovieGroup(
      h, w, "PauseMenu::ShowAdjust (sub_8220E888)", 100,
      [&h](PPCContext&, Random& r, const MovieWorld& w) {
        for (const uint32_t field : {60u, 64u, 68u}) {
          guest::StoreU32(h.base, w.block(rs::kPauseData) + field,
                          r.Below(6) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(21)) - 10));
        }
      },
      __imp__sub_8220E888, hooks::RaceScreensPauseMenuShowAdjust, {128, 32});
  // Open: r3 the screen word, r4 the kind 0 to 4 (now and then any word),
  // r5 the flag.
  MovieGroup(
      h, w, "PauseMenu::Open (sub_8220FA88)", 150,
      [](PPCContext& t, Random& r, const MovieWorld& w) {
        t.r3.u64 = w.block(rs::kPauseScreen);
        t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(5);
        t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
      },
      __imp__sub_8220FA88, hooks::RaceScreensPauseMenuOpen, {112, 16, {0, 0}, true});
  // The options page: the edits (+36 to +48, +144) and the saved ones (+0 to
  // +12, +72 to +84) 0, 1 or any; the list's selection 0 to 4.
  auto edits = [&h](Random& r, const MovieWorld& w) {
    const uint32_t data = w.block(rs::kPauseData);
    for (const uint32_t field : {0u, 4u, 8u, 12u, 36u, 40u, 44u, 48u, 72u, 76u, 80u, 84u}) {
      guest::StoreU32(h.base, data + field, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
    }
    guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListSelection, r.Below(5));
  };
  MovieGroup(
      h, w, "PauseMenu::ToggleOption (sub_8220CDA0)", 200,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        edits(r, w);
        const uint32_t sides[4] = {0xFFFFFFFFu, 0, 1, uint32_t(r.Next())};
        guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListSide, sides[r.Below(4)]);
      },
      __imp__sub_8220CDA0, hooks::RaceScreensPauseMenuToggleOption, {128, 16});
  const World list_world;
  if (!list_world.ok()) {
    ITC_LOG_ERROR("harness: race_screens: no block from the allocation layer");
    return;
  }
  FrameSpec options{112, 24};
  options.list_world = &list_world;
  MovieGroup(
      h, w, "PauseMenu::OptionsPage (sub_8220DB98)", 400,
      [&](PPCContext&, Random& r, const MovieWorld& w) { edits(r, w); },
      __imp__sub_8220DB98, hooks::RaceScreensPauseMenuOptionsPage, options);
  // SetUpOptions: MenuList::Init frees the list's old item block (+68, 0
  // here) and makes one of its own: its words compared, then it's freed and
  // +68 put to 0.
  FrameSpec set_up{144, 40, {40, 80}};
  set_up.after = [&](Outputs& out) {
    const uint32_t list = w.block(rs::kPauseList);
    const uint32_t block = guest::LoadU32(h.base, list + rs::kListBlock);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      out.Bytes(Read(h.base, block, 5 * 4));
      memory::FreePages(block);
    }
    guest::StoreU32(h.base, list + rs::kListBlock, 0);
  };
  MovieGroup(
      h, w, "PauseMenu::SetUpOptions (sub_8220E530)", 100,
      [&](PPCContext&, Random& r, const MovieWorld& w) {
        edits(r, w);
        guest::StoreU32(h.base, w.block(rs::kPauseList) + rs::kListBlock, 0);
      },
      __imp__sub_8220E530, hooks::RaceScreensPauseMenuSetUpOptions, set_up);
  MovieGroup(h, w, "PauseMenu::SetAdjust (sub_8220F1B8)", 100, pause_step,
             __imp__sub_8220F1B8, hooks::RaceScreensPauseMenuSetAdjust, {112, 24});
  Openers(h, w);
  Results(h, w);
  AfterRace(h, w);
  Dialog(h, w);
  Draws(h, w);
  MainPages(h, w);
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
  Pages(h);
  Transitions(h);
}

}  // namespace harness
