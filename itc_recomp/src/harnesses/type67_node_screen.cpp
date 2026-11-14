// The node screen (src/type67_screens/node_screen.cpp;
// docs/functions/type67-screens.md) against the recompiled functions, run
// under the type67_screens harness (Type67Screens calls Type67NodeScreen).
// The originals run with --native_type67_screens off (their calls to each
// other through the adapters, so the originals), the plain C++ through the
// register steps (hooks/type67_screens.h).
//
// The world, as the color list's (type67_color_list.cpp): the loading
// screen's UI movie world (the leaves' +108 made 1 to 7 so SetText finds a
// text record; the screen's movie id 16 to 19, not in the movie table, so
// UiMovies::Find answers the word at 0x826C5CDC, pointed at movie 0), and a
// block of our own: the screen (random words; its paired node table at +0,
// laid as the node tables' cases lay one; its node, child, named animation
// and string words in the movie world's ranges; the page block's page -1 or
// 0 to 5 and step -1 or 0 to 2; the first-call word 0 or 1), its two menu
// lists (+10132 the courses: 10 items, 5 shown, selection 0 to 4, scroll 0
// to 5; +10204 the toggle: selection 0 or 1; the players' action mappers;
// their item blocks ours, or 0 or pages of the allocation layer's for the
// destructors), the screen's vtable with its slots from stand-ins (the empty
// handler sub_82106540; sub_820CC470, which stores r4 at r3 + 20, so a
// call's r3 and r4 show, except slot 20, called with the r4 SetArrows
// leaves, which SetIcons doesn't read; sub_82090780, which answers r3's
// +80; slot 4 Type6ListScreen::ItemInfo or NodeScreen::ItemInfo, so the info
// LoadItem reads is written; slots 1, 2, 20, 21 and 22, and 15 where the
// lists' item blocks are 0 or pages, sometimes Type6NodeScreen's own: the
// native side runs them as plain C++), a fake sound manager, the screen
// fades' words (counts 0 or small), the page and step tables' .data words (each guard's
// bit 0 clear, the table random, or set with the table holding its
// entries), and player 0's save data pointed so that its course records
// (+0x1D52C) are a block of ours: 22 records of random words with their
// entry chains (+44 the first node, +48 the count, 0 to 10) in range.
// Movie 15 (the id Type6NodeScreen's icons, scroll text and marks find)
// must not be in the movie table either. UiMovie::Run's update
// (sub_8211D2F8) and ApplyAnimation (sub_825A08E0) are stood in for,
// recorded and answered from the case's generator.
// Compared exactly (the cursor's child +16 is an integer's conversion,
// never a NaN): the recorded calls, the block, the records, the movie
// world, the globals (the tables and guards among them), the stack below
// and above the call (the originals' register saves left out: the
// function's own and those of the callees the native side runs as plain
// C++; in frames that call SetText, +48 to +79), r1 and r3. The deleting
// destructor with bit 0 set runs on blocks of operator new's, compared by
// r3 and whether the block is handed out again.
//
// The panel screens' quit dialog (Type6PanelScreen's Draw, DrawList and
// AskQuit, Type7PanelScreen's AskQuit and its Draw and DrawList with it)
// runs in this file too, for the movie world's text nodes: a panel world
// laid over the same block (LayPanel).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/type67_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <tuple>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type67_screens);

namespace harness {

void LayNodeTable(uint8_t* base, uint32_t table, Random& r, bool paired);

namespace {

namespace lw = loading_world;
namespace rs = race_screens;
namespace ts = type67_screens;

constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kEmpty = 0x82106540, kSetR4 = 0x820CC470, kGetter = 0x82090780;
constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x40;
// The page table (0x82816078), its guard, the step table and its guard.
constexpr uint32_t kTables = 0x82816078, kTablesBytes = 0x50;
constexpr uint32_t kPages = 0x82816078, kPageGuard = 0x828160A8;
constexpr uint32_t kSteps = 0x828160AC, kStepGuard = 0x828160C4;
// Player 0's save data slot.
constexpr uint32_t kSaveSlot = 0x826C5B58;
constexpr uint32_t kCourseRecords = 0x1D52C, kRecordsBytes = 0x2400;

// The block: the screen, then (as the color list's harness) the mappers'
// vtables and mappers, the sound manager, the screen's vtable, the lists'
// item blocks and the dispatcher.
constexpr uint32_t kVtablesAt = 0x2C00;
constexpr uint32_t kMappersAt = 0x2D00;
constexpr uint32_t kManagerAt = 0x3500;
constexpr uint32_t kManagerVtableAt = 0x3580;
constexpr uint32_t kVtableAt = 0x3800;
constexpr uint32_t kItemsAt = 0x3900;
constexpr uint32_t kDispatcherAt = 0x3B00;
constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kLists[2] = {10132, 10204};
constexpr uint32_t kScreenBytes = 10552;

constexpr uint32_t kBelow = 1024;  // the stack read below the call's r1
constexpr uint32_t kAbove = 256;   // and above it

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kMissingMovie, 4},    {rs::kMapperEntries, 32},        {rs::kControllerEntries, 16},
    {kFades, kFadesBytes}, {menu_sounds::kSoundManager, 4}, {kTables, kTablesBytes},
    {kSaveSlot, 4},
};

uint64_t Seed(uint64_t z) {
  z += 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
}

struct Recorded {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<Recorded>* g_recorded = nullptr;
Random g_answers;

bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_recorded == nullptr || (function != 0x8211D2F8 && function != 0x825A08E0)) {
    return false;
  }
  g_recorded->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = function == 0x8211D2F8 ? g_answers.Below(2) : g_answers.Next();
  return true;
}

// What r4 (and r5) hold and the lists' item blocks.
enum class Kind {
  kAny,       // r4 the dispatcher (or any word), item blocks ours
  kNoDelete,  // r4 flags with bit 0 clear, item blocks 0 or pages
  kRelease,   // item blocks 0 or pages
  kRecords,   // r4 a course (0 to 10), r5 the traffic (0 or 1)
  kArrows,    // r4 0 or 1 (any word sometimes), r5 0, 1 or any word
  kInit,      // item blocks 0 or pages, the new ones compared and freed
  kDispatcher,  // r4 the dispatcher
  kIndex,     // r4 0 to 9
  kPanel,     // the panel screen's world (LayPanel), r4 the dispatcher
};

bool Inits(Kind kind) { return kind == Kind::kInit; }

// The stack ranges left out, as [sp - from, sp - to).
using Ranges = std::vector<std::pair<uint32_t, uint32_t>>;

struct Function {
  const char* name;
  uint32_t address;
  int cases;
  Kind kind;
  Ranges skip;  // the register saves left out
  GuestFunction recompiled, native;
};

uint32_t Pick(Random& r, std::initializer_list<uint32_t> targets) {
  return *(targets.begin() + r.Below(uint32_t(targets.size())));
}

// An action id: mostly the menus' (151 to 180), sometimes any word.
uint32_t AnyAction(Random& r) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30);
}

// The tables' entries (as the originals build them).
constexpr uint32_t kPageEntries[6] = {kEmpty, kEmpty, 0x82259D80, kEmpty, 0x822590E8, 0x82259328};
constexpr uint32_t kStepEntries[3] = {0x82259EC0, 0x82259680, 0x82259750};

void LayTables(uint8_t* base, Random& r) {
  lw::WorldRandom(base, kTables, kTablesBytes, r);
  for (const auto& [guard, table, entries, count] :
       {std::tuple{kPageGuard, kPages, kPageEntries, 6u},
        std::tuple{kStepGuard, kSteps, kStepEntries, 3u}}) {
    const uint32_t word = uint32_t(r.Next()) & ~1u;
    if (r.Below(2) == 0) {
      guest::StoreU32(base, guard, word);
      continue;
    }
    guest::StoreU32(base, guard, word | 1);
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, table + 8 * i, entries[i]);
      guest::StoreU32(base, table + 8 * i + 4, 0);
    }
  }
}

// The course records: 22 records of random words, each with its node chain
// (+0 to +43: a node's halfwords the next node, 0 to 10, and the entry, 0 to
// 11), its first node (+44, 0 to 10), its count (+48, 0 to 10, sometimes
// more), its entries' string offsets (+52, 0 to 30) and minutes (+72's high
// halfword, 0 to 70 with the top bit sometimes set).
void LayRecords(uint8_t* base, uint32_t records, Random& r) {
  lw::WorldRandom(base, records, kRecordsBytes, r);
  for (uint32_t k = 0; k < 22; k++) {
    const uint32_t record = records + 20 + 408 * k;
    for (uint32_t j = 0; j < 11; j++) {
      guest::StoreU16(base, record + 4 * j, uint16_t(r.Below(11)));
      guest::StoreU16(base, record + 4 * j + 2, uint16_t(r.Below(12)));
    }
    guest::StoreU16(base, record + 44, uint16_t(r.Below(11)));
    guest::StoreU16(base, record + 48, uint16_t(r.Below(6) == 0 ? 11 + r.Below(4) : r.Below(11)));
    for (uint32_t e = 0; e < 12; e++) {
      guest::StoreU32(base, record + 28 * e + 52, r.Below(31));
      guest::StoreU16(base, record + 28 * e + 72,
                      uint16_t((r.Below(4) == 0 ? 0x8000 : 0) | r.Below(71)));
    }
  }
}

// The panel screens' world (Type6PanelScreen's Draw, DrawList and AskQuit,
// Type7PanelScreen's with AskQuit): the screen a block of random words, its
// vtable stand-ins, the quit dialog's state +9656 (0 to 3, mostly 0) and
// +11700 (0 or 1), its car list (+9644) at +0x3600 with stand-ins for its
// vtable (+0x3A00) and its +80 0 or random (the getter's answers), the
// dispatcher's ring's count and start small, the fade counts 0 or small.
void LayPanel(uint8_t* base, uint32_t block, Random& r) {
  constexpr uint32_t kListAt = 0x3600, kListVtableAt = 0x3A00;
  const uint32_t s = block;
  guest::StoreU32(base, s + 9656, r.Below(2) == 0 ? 0 : r.Below(4));
  guest::StoreU32(base, s + 11700, r.Below(2));
  guest::StoreU32(base, s + 9644, block + kListAt);
  guest::StoreU32(base, block + kListAt, block + kListVtableAt);
  guest::StoreU32(base, block + kListAt + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, block + kListVtableAt + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, s, block + kVtableAt);
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, block + kVtableAt + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, block + kDispatcherAt + 204 + 600, r.Below(50));
  guest::StoreU32(base, block + kDispatcherAt + 204 + 604, r.Below(60));
  lw::WorldRandom(base, kFades, kFadesBytes, r);
  for (const uint32_t at : {mode_tick::kFadeInCounts, mode_tick::kFadeOutCounts}) {
    for (uint32_t layer = 0; layer < 2; layer++) {
      guest::StoreU32(base, at + 4 * layer, r.Below(2) == 0 ? 0 : 1 + r.Below(5));
    }
  }
}

void LayBlock(uint8_t* base, uint32_t block, uint32_t records, Random& r, Kind kind,
              std::vector<uint32_t>& made) {
  lw::WorldRandom(base, block, kBlockBytes, r);
  if (kind == Kind::kPanel) {
    LayPanel(base, block, r);
    return;
  }
  const uint32_t s = block;
  auto sub = [&r] { return r.Below(3) == 0 ? kNone : r.Below(20); };
  auto child = [&r] { return r.Below(6) == 0 ? kNone : r.Below(60); };
  auto named = [&r] { return r.Below(6) == 0 ? kNone : r.Below(48); };
  // The paired node table at +0.
  LayNodeTable(base, s, r, true);
  guest::StoreU32(base, s + 10100, r.Below(8) == 0 ? 0x80000000u | r.Below(4) : 16 + r.Below(4));
  for (const uint32_t at : {10276u}) guest::StoreU32(base, s + at, sub());
  for (const uint32_t at : {10396u, 10400u, 10404u, 10408u, 10412u, 10544u}) {
    guest::StoreU32(base, s + at, child());
  }
  // Step2's cursor (the node +10280 and its child +10548), looked up with no
  // check: always one.
  for (const uint32_t at : {10280u, 10548u}) guest::StoreU32(base, s + at, r.Below(60));
  for (uint32_t i = 0; i < 30; i++) guest::StoreU32(base, s + 10416 + 4 * i, child());
  // The arrows' values, stored as a found node's +108 (its children's first
  // index): small, as a node's (a wild index makes later lookups through the
  // node read outside the movie world).
  for (const uint32_t at : {10536u, 10540u}) {
    guest::StoreU32(base, s + at, r.Below(4) == 0 ? kNone - r.Below(3) : 1 + r.Below(7));
  }
  // The named animations, +10284 to +10392.
  for (uint32_t at = 10284; at <= 10392; at += 4) guest::StoreU32(base, s + at, named());
  // The page block and the screen's words.
  guest::StoreU32(base, s + 10104, r.Below(6) == 0 ? kNone : r.Below(6));
  guest::StoreU32(base, s + 10108, r.Below(6));
  guest::StoreU32(base, s + 10112, r.Below(6));
  guest::StoreU32(base, s + 10116, r.Below(4) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, s + 10120, r.Below(2) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, s + 10128, r.Below(2));
  // The lists (as the list part's harness shapes one).
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t list = s + kLists[k];
    const bool toggle = k == 1;
    guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
    guest::StoreU32(base, list + rs::kListCount, toggle ? 2 : 10);
    guest::StoreU32(base, list + rs::kListShown, toggle ? 2 : 5);
    guest::StoreU32(base, list + rs::kListScroll, toggle ? 0 : r.Below(6));
    guest::StoreU32(base, list + rs::kListScrollBefore, toggle ? 0 : r.Below(6));
    if (r.Below(3) == 0) {
      guest::StoreU32(base, list + rs::kListScrollBefore,
                      guest::LoadU32(base, list + rs::kListScroll));
    }
    const uint32_t n = toggle ? 2 : 5;
    guest::StoreU32(base, list + rs::kListSelection, r.Below(n));
    guest::StoreU32(base, list + rs::kListSelectionBefore, r.Below(n));
    for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
      guest::StoreU32(base, list + field, AnyAction(r));
    }
    guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
    guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
    guest::StoreU32(base, list + rs::kListWrapped, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    uint32_t items = block + kItemsAt + 0x40 * k;
    if (kind == Kind::kNoDelete || kind == Kind::kRelease || Inits(kind)) {
      items = 0;
      if (r.Below(4) != 0) {
        items = memory::AllocPages(16, 4096);
        lw::WorldRandom(base, items, 16, r);
        made.push_back(items);
      }
    }
    guest::StoreU32(base, list + rs::kListBlock, items);
  }
  // The players' mappers.
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t vtable = block + kVtablesAt + 0x40 * c;
    const uint32_t mapper = block + kMappersAt + 0x200 * c;
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, rs::kPressedSlot);
    guest::StoreU32(base, vtable + 48, rs::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  // The fake sound manager.
  const uint32_t manager = block + kManagerAt, manager_vtable = block + kManagerVtableAt;
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, manager_vtable + 52, 0x820CE9F8);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
  // The screen's vtable: stand-ins; slot 4 an ItemInfo; slot 20 without the
  // r4 recorder.
  const uint32_t vtable = block + kVtableAt;
  guest::StoreU32(base, s, vtable);
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, vtable + 16, Pick(r, {0x82237008, 0x82259998}));
  guest::StoreU32(base, vtable + 80, Pick(r, {kEmpty, kGetter}));
  // Type6NodeScreen's own slots the node screen calls, or a stand-in: 1
  // (NodeScreen::SetUpTexts), 2 (SetText), 20 (SetIcons, which doesn't read
  // r4), 21 (SetScrollText), 22 (ShowMarks); 15 (SetUpMenus) only where the
  // lists' item blocks are 0 or pages (its MenuList::Init frees the old).
  guest::StoreU32(base, vtable + 4, Pick(r, {0x82247FE0, kEmpty, kSetR4, kGetter}));
  guest::StoreU32(base, vtable + 8, Pick(r, {0x822370A0, kEmpty, kSetR4, kGetter}));
  guest::StoreU32(base, vtable + 80, Pick(r, {0x82237160, kEmpty, kGetter}));
  guest::StoreU32(base, vtable + 84, Pick(r, {0x82237228, kEmpty, kSetR4, kGetter}));
  guest::StoreU32(base, vtable + 88, Pick(r, {0x822372A0, kEmpty, kSetR4, kGetter}));
  if (Inits(kind)) {
    guest::StoreU32(base, vtable + 60, Pick(r, {0x82236F00, kEmpty, kSetR4, kGetter}));
  }
  // The screen fades: counts 0 mostly.
  lw::WorldRandom(base, kFades, kFadesBytes, r);
  for (const uint32_t at : {mode_tick::kFadeInCounts, mode_tick::kFadeOutCounts}) {
    for (uint32_t layer = 0; layer < 2; layer++) {
      guest::StoreU32(base, at + 4 * layer, r.Below(2) == 0 ? 0 : 1 + r.Below(5));
    }
  }
  LayTables(base, r);
  LayRecords(base, records, r);
  guest::StoreU32(base, kSaveSlot, records - kCourseRecords);
}

void Run(Context& h, const lw::MovieWorld& w, uint32_t block, uint32_t records,
         const Function& f) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
  for (int c = 0; c < f.cases; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed((uint64_t(f.address) << 16) ^ uint64_t(c)));
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) saved.push_back(Read(base, g.address, g.bytes));
      lw::ShapeMovieWorld(base, w, r);
      for (uint32_t k = 0; k < lw::kLeaves; k++) {
        guest::StoreU32(base, w.leaf(k) + 108, 1 + r.Below(7));
      }
      guest::StoreU32(base, kMissingMovie, w.movie(0));
      std::vector<uint32_t> made;
      LayBlock(base, block, records, r, f.kind, made);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      lw::WorldRandom(base, sp - kBelow, kBelow + kAbove, r);
      const uint64_t high = r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0;
      t.r3.u64 = high | block;
      t.r4.u64 = r.Below(2) == 0 ? block + kDispatcherAt : r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      if (f.kind == Kind::kNoDelete) t.r4.u64 &= ~1ull;
      if (f.kind == Kind::kRecords) {
        t.r4.u64 = r.Below(11);
        t.r5.u64 = r.Below(2);
      }
      if (f.kind == Kind::kDispatcher || f.kind == Kind::kPanel) {
        t.r4.u64 = block + kDispatcherAt;
      }
      if (f.kind == Kind::kIndex) t.r4.u64 = r.Below(10);
      std::vector<uint32_t> old_items;
      for (const uint32_t at : kLists) {
        old_items.push_back(guest::LoadU32(base, block + at + rs::kListBlock));
      }
      if (f.kind == Kind::kArrows) {
        t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(2);
        t.r5.u64 = r.Below(6) == 0 ? r.Next() : r.Below(2);
      }
      t.lr = 0x82000000 + 4 * r.Below(1024);
      g_answers = Random(r.Next());
      std::vector<Recorded> recorded;
      g_recorded = &recorded;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = StandIn;
      if (v == 0) {
        ITC_FLAG(native_type67_screens) = false;
        f.recompiled(t, base);
        ITC_FLAG(native_type67_screens) = true;
      } else {
        f.native(t, base);
      }
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      g_recorded = nullptr;
      out[v].Word(uint32_t(recorded.size()));
      for (const Recorded& call : recorded) {
        for (const uint32_t word : {call.function, call.lr, call.r1, uint32_t(call.r3),
                                    uint32_t(call.r3 >> 32), uint32_t(call.r4),
                                    uint32_t(call.r4 >> 32)}) {
          out[v].Word(word);
        }
      }
      // MenuList::Init's new item blocks: compared (whether one was made,
      // its words) and freed, their words in the lists replaced.
      if (Inits(f.kind)) {
        for (uint32_t k = 0; k < 2; k++) {
          const uint32_t list = block + kLists[k];
          const uint32_t now = guest::LoadU32(base, list + rs::kListBlock);
          out[v].Word(now == old_items[k] ? 1 : 0);
          if (now != 0 && now != old_items[k] && memory::IsPages(now)) {
            out[v].Bytes(Read(base, now, 4 * guest::LoadU32(base, list + rs::kListCount)));
            memory::FreePages(now);
            guest::StoreU32(base, list + rs::kListBlock, 0xB10C0000 + k);
          }
        }
      }
      // The item blocks the destructors free: whether still the layer's.
      for (const uint32_t items : made) {
        const bool still = memory::IsPages(items);
        out[v].Word(still ? 1 : 0);
        if (still) memory::FreePages(items);
      }
      out[v].Bytes(Read(base, block, kBlockBytes));
      out[v].Bytes(Read(base, records, kRecordsBytes));
      out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
      for (const Global& g : kGlobals) out[v].Bytes(Read(base, g.address, g.bytes));
      std::vector<uint8_t> stack = Read(base, sp - kBelow, kBelow + kAbove);
      for (const auto& [from, to] : f.skip) {
        for (uint32_t i = kBelow - from; i < kBelow - to; i++) stack[i] = 0;
      }
      out[v].Bytes(stack);
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The deleting destructor with bit 0 of r4 set: a block of operator new's
// (its lists' item blocks 0) destroyed and deleted, compared by r3 and
// whether operator new hands the block out again.
void Deleting(Context& h, const char* name, uint32_t kAddress, GuestFunction recompiled,
              GuestFunction native) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{}, deleting (sub_{:08X})", name, kAddress));
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed(uint64_t(kAddress) << 20 ^ uint64_t(c)));
      const auto object = memory::crt::OperatorNew(base, kScreenBytes);
      if (!object) {
        ITC_LOG_ERROR("harness: type67_screens: operator new failed");
        return;
      }
      lw::WorldRandom(base, *object, kScreenBytes, r);
      for (const uint32_t at : kLists) {
        guest::StoreU32(base, *object + at + rs::kListBlock, 0);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = *object;
      t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type67_screens) = false;
        recompiled(t, base);
        ITC_FLAG(native_type67_screens) = true;
      } else {
        native(t, base);
      }
      out[v].Word(t.r3.u32 == *object ? 1 : 0);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const auto probe = memory::crt::OperatorNew(base, kScreenBytes);
      out[v].Word(probe && *probe == *object ? 1 : 0);
      if (probe) memory::crt::OperatorDelete(*probe);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The panel functions without adapters (only the native Draws and DrawLists
// call them): their plain C++ with the adapters' calls.
void NativeType6DrawList(PPCContext& t, uint8_t* b) {
  const ts::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = ts::Type6PanelScreenDrawList(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
}
void NativeType6AskQuit(PPCContext& t, uint8_t* b) {
  const ts::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = ts::Type6PanelScreenAskQuit(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
}
void NativeType7AskQuit(PPCContext& t, uint8_t* b) {
  const ts::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = ts::Type7PanelScreenAskQuit(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
}
void NativeType7DrawList(PPCContext& t, uint8_t* b) {
  const ts::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = ts::Type7PanelScreenDrawList(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
}

}  // namespace

void Type67NodeScreen(Context& h) {
  uint8_t* const base = h.base;
  // 15: Type6NodeScreen's slots that find movie 15 itself.
  for (const uint32_t id : {15u, 16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR(
          "harness: type67_screens: movie {} is in the table; the node screen skipped", id);
      return;
    }
  }
  const lw::MovieWorld w;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  const uint32_t records = memory::Alloc(kRecordsBytes, 0);
  if (!w.ok() || block == 0 || records == 0) {
    ITC_LOG_ERROR("harness: type67_screens: no blocks for the node screen");
    if (block != 0) memory::Free(block);
    if (records != 0) memory::Free(records);
    return;
  }
  const lw::SavedGlobals saved(base);
  lw::BuildStringTable(base, w);

  // The register saves: the function's own (its frame's top bytes) and
  // those of the callees the native side runs as plain C++ (at their frames'
  // tops below it); in the frames that call SetText, +48 to +79 (its
  // varargs).
  auto saves = [](std::initializer_list<std::pair<uint32_t, uint32_t>> list) {
    return Ranges(list);
  };
  using K = Kind;
  const Function functions[] = {
      {"NodeScreen::Page4", 0x822590E8, 4, K::kAny, {}, __imp__sub_822590E8,
       hooks::NodeScreenPage4},
      {"NodeScreen::Page5", 0x82259328, 4, K::kAny, saves({{16, 0}}), __imp__sub_82259328,
       hooks::NodeScreenPage5},
      {"NodeScreen::SetArrows", 0x82259368, 8, K::kArrows, saves({{40, 0}}),
       __imp__sub_82259368, hooks::NodeScreenSetArrows},
      {"NodeScreen::LoadItem", 0x82259050, 6, K::kAny, saves({{24, 0}}), __imp__sub_82259050,
       hooks::NodeScreenLoadItem},
      {"NodeScreen::ShowRecords", 0x82259410, 12, K::kRecords, saves({{56, 0}, {112, 80}, {200, 160}}),
       __imp__sub_82259410, hooks::NodeScreenShowRecords},
      {"NodeScreen::ShowSelection", 0x82259A78, 8, K::kAny, saves({{32, 0}, {168, 128}}),
       __imp__sub_82259A78, hooks::NodeScreenShowSelection},
      {"NodeScreen::OnToggleMoved", 0x82259B70, 8, K::kAny, saves({{40, 0}, {168, 128}}),
       __imp__sub_82259B70, hooks::NodeScreenOnToggleMoved},
      {"NodeScreen::Refresh", 0x822599B0, 8, K::kAny,
       saves({{24, 0}, {188, 112}, {224, 192}, {312, 272}}), __imp__sub_822599B0,
       hooks::NodeScreenRefresh},
      {"NodeScreen::Show", 0x82259CA0, 8, K::kInit,
       saves({{32, 0}, {80, 48}, {176, 128}, {320, 240}, {380, 320}, {468, 400}}),
       __imp__sub_82259CA0, hooks::NodeScreenShow},
      {"NodeScreen::Page2", 0x82259D80, 12, K::kAny,
       saves({{32, 0}, {168, 112}, {224, 192}, {280, 240}, {312, 272}}), __imp__sub_82259D80,
       hooks::NodeScreenPage2},
      {"NodeScreen::Step0", 0x82259EC0, 8, K::kAny,
       saves({{32, 0}, {172, 128}, {328, 256}}), __imp__sub_82259EC0,
       hooks::NodeScreenStep0},
      {"NodeScreen::Step1", 0x82259680, 6, K::kAny, saves({{24, 0}}), __imp__sub_82259680,
       hooks::NodeScreenStep1},
      {"NodeScreen::Step2", 0x82259750, 8, K::kAny,
       saves({{80, 0}, {208, 176}, {236, 224}, {376, 304}}), __imp__sub_82259750,
       hooks::NodeScreenStep2},
      {"NodeScreen::Draw", 0x8225A1B8, 10, K::kAny,
       saves({{24, 0}, {192, 160}, {328, 272}, {384, 352}, {472, 400}}), __imp__sub_8225A1B8,
       hooks::NodeScreenDraw},
      {"NodeScreen::Update", 0x8225A2C0, 10, K::kAny,
       saves({{32, 0}, {224, 144}, {352, 272}, {380, 368}, {520, 400}}), __imp__sub_8225A2C0,
       hooks::NodeScreenUpdate},
      {"NodeScreen::~NodeScreen", 0x82259948, 6, K::kRelease, saves({{16, 0}}),
       __imp__sub_82259948, hooks::NodeScreenDestruct},
      {"NodeScreen::DeletingDestructor", 0x8225A168, 4, K::kNoDelete,
       saves({{24, 0}, {128, 112}}), __imp__sub_8225A168, hooks::NodeScreenDeletingDestruct},
      {"Type6NodeScreen::SetUpMenus", 0x82236F00, 12, K::kInit,
       saves({{48, 0}, {208, 144}, {256, 192}, {344, 304}}),
       __imp__sub_82236F00, hooks::Type6NodeScreenSetUpMenus},
      {"Type6NodeScreen::SetText", 0x822370A0, 8, K::kIndex, saves({{32, 0}, {80, 48}}),
       __imp__sub_822370A0, hooks::Type6NodeScreenSetText},
      {"Type6NodeScreen::Back", 0x82237110, 4, K::kDispatcher, {}, __imp__sub_82237110,
       hooks::Type6NodeScreenBack},
      {"Type6NodeScreen::SetIcons", 0x82237160, 6, K::kAny, saves({{16, 0}, {136, 112}}),
       __imp__sub_82237160, hooks::Type6NodeScreenSetIcons},
      {"Type6NodeScreen::SetScrollText", 0x82237228, 6, K::kAny, saves({{16, 0}, {64, 32}}),
       __imp__sub_82237228, hooks::Type6NodeScreenSetScrollText},
      {"Type6NodeScreen::ShowMarks", 0x822372A0, 8, K::kIndex, saves({{32, 0}}),
       __imp__sub_822372A0, hooks::Type6NodeScreenShowMarks},
      {"NodeScreen::SetUpTexts", 0x82247FE0, 12, K::kAny,
       saves({{40, 0}, {80, 48}, {144, 128}}), __imp__sub_82247FE0,
       hooks::NodeScreenSetUpTexts},
      {"Type6NodeScreen::~Type6NodeScreen", 0x82236EA0, 6, K::kRelease, saves({{16, 0}}),
       __imp__sub_82236EA0, hooks::Type6NodeScreenDestruct},
      {"Type6NodeScreen::DeletingDestructor", 0x82237048, 4, K::kNoDelete,
       saves({{24, 0}, {128, 112}}), __imp__sub_82237048,
       hooks::Type6NodeScreenDeletingDestruct},
      // The panel screens' quit dialog (the panel world, LayPanel).
      {"Type6PanelScreen::AskQuit", 0x82230BA8, 6, K::kPanel, saves({{32, 0}, {80, 48}}),
       __imp__sub_82230BA8, NativeType6AskQuit},
      {"Type7PanelScreen::AskQuit", 0x82240518, 6, K::kPanel, saves({{32, 0}, {80, 48}}),
       __imp__sub_82240518, NativeType7AskQuit},
      {"Type6PanelScreen::DrawList", 0x82230EF0, 8, K::kPanel,
       saves({{24, 0}, {144, 112}, {192, 160}}),
       __imp__sub_82230EF0, NativeType6DrawList},
      {"Type6PanelScreen::Draw", 0x82230F40, 12, K::kPanel,
       saves({{24, 0}, {144, 112}, {192, 160}}),
       __imp__sub_82230F40, hooks::Type6PanelScreenDraw},
      {"Type7PanelScreen::DrawList, with AskQuit", 0x82240908, 8, K::kPanel,
       saves({{24, 0}, {144, 112}, {192, 160}}),
       __imp__sub_82240908, NativeType7DrawList},
      {"Type7PanelScreen::Draw, with AskQuit", 0x82240958, 8, K::kPanel,
       saves({{24, 0}, {144, 112}, {192, 160}}),
       __imp__sub_82240958, hooks::Type7PanelScreenDraw},
  };
  for (const Function& f : functions) {
    Run(h, w, block, records, f);
  }
  Deleting(h, "NodeScreen::DeletingDestructor", 0x8225A168, __imp__sub_8225A168,
           hooks::NodeScreenDeletingDestruct);
  Deleting(h, "Type6NodeScreen::DeletingDestructor", 0x82237048, __imp__sub_82237048,
           hooks::Type6NodeScreenDeletingDestruct);
  memory::Free(block);
  memory::Free(records);
  saved.Restore();
}

}  // namespace harness
