// The titled list screen (src/type67_screens/titled_list.cpp,
// docs/functions/type67-screens.md) against the recompiled functions, run
// under the type67_screens harness (Type67Screens calls Type67TitledList).
// The originals run with --native_type67_screens off (their calls to each
// other through the adapters, so the originals), the plain C++ through the
// register steps (hooks/type67_screens.h).
//
// The world: the loading screen's UI movie world (loading_world.h, as the
// list helpers' cases use it: the leaves' +108 made 1 to 7 so SetText finds
// a text record; the screen's movie id 16 to 19, not in the movie table, so
// UiMovies::Find answers the word at 0x826C5CDC, pointed at movie 0), and a
// block of our own: the screen (random words, its node, child and named
// animation words in the movie world's ranges, the page block's page -1 or 0
// to 8 and step 0 to 2, the first-call word 0 or 1), its three menu lists
// (+10144, +10216, +10288: shaped as the list part's harness shapes one,
// with the players' action mappers, their item blocks ours, or 0 or pages
// of the allocation layer's for the destructors), its node table (+10360,
// as the node tables' cases lay one, paired), the list part (+9676, ours or
// 0) and the screen's vtable with their slots from stand-ins (the empty
// handler sub_82106540; sub_820CC470, which stores r4 at r3 + 20, so a
// call's r3 and r4 show; sub_82090780, which answers r3's +80; slot 25 the
// screens' ItemInfo functions), a fake sound manager (as the type 2
// dialog's harness makes it), the screen fades' words (counts 0 or small),
// and the page and step tables' .data words: each guard's bit 0 clear (the
// table random, built by the call) or set (the table holding its entries,
// as a call before would leave it). UiMovie::Run's update (sub_8211D2F8)
// and ApplyAnimation (sub_825A08E0) are stood in for, recorded and answered
// from the case's generator. Compared exactly (the one float, the cursor's
// child +16 OnItemMoved stores, is an integer's conversion, never a NaN, and
// is compared with the movie world): the recorded calls, the block, the movie world, the globals
// (the tables and guards among them), the stack below and above the call
// (the originals' register saves left out: the function's own and those of
// the callees the native side runs as plain C++), r1 and r3. The deleting destructors with
// bit 0 set run on blocks of operator new's, compared by r3 and whether the
// block is handed out again.

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

// The node tables' layout (type67_screen_tables.cpp).
void LayNodeTable(uint8_t* base, uint32_t table, Random& r, bool paired);

namespace {

namespace ts = type67_screens;
namespace lw = loading_world;
namespace rs = race_screens;

constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kEmpty = 0x82106540, kSetR4 = 0x820CC470, kGetter = 0x82090780;
constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x40;
constexpr uint32_t kTables = 0x82815F88, kTablesBytes = 0x68;

// The block's layout (the screen at its start, up to its node table's end,
// 0x2B78).
constexpr uint32_t kVtablesAt = 0x2C00;  // the mappers' vtables (0x40 each)
constexpr uint32_t kMappersAt = 0x2D00;  // 4 mappers of 0x200
constexpr uint32_t kManagerAt = 0x3500;  // the fake sound manager
constexpr uint32_t kManagerVtableAt = 0x3580;
constexpr uint32_t kPartAt = 0x3600;     // the list part (+9676), its vtable +0x80
constexpr uint32_t kVtableAt = 0x3800;   // the screen's vtable (32 slots)
constexpr uint32_t kItemsAt = 0x3900;    // the lists' item blocks (0x40 each)
constexpr uint32_t kDispatcherAt = 0x3A00;
constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kLists[3] = {10144, 10216, 10288};

constexpr uint32_t kBelow = 1024;  // the stack read below the call's r1
constexpr uint32_t kAbove = 256;   // and above it

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kMissingMovie, 4},     {rs::kMapperEntries, 32},          {rs::kControllerEntries, 16},
    {kFades, kFadesBytes},  {menu_sounds::kSoundManager, 4},   {kTables, kTablesBytes},
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

// What r4 holds and the lists' item blocks.
enum class Kind {
  kAny,       // r4 the dispatcher (or the caller's word), item blocks ours
  kNoDelete,  // r4 flags with bit 0 clear, item blocks 0 or pages
  kRelease,   // item blocks 0 or pages
  kInit,      // item blocks 0 or pages, the new ones compared and freed
  kDispatcher,  // r4 the dispatcher
  kRow,       // r4 0 to 2 (the +10144 list's selection)
  kMarks,     // r4 0 to 3
};

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
constexpr uint32_t kPageEntries[9] = {kEmpty,     kEmpty, 0x82257148, 0x822572F0, 0x82257360,
                                      kEmpty,     kEmpty, 0x82257490, 0x82255818};
constexpr uint32_t kStepEntries[3] = {0x82257D48, 0x82257FF8, 0x822580E0};

void LayTables(uint8_t* base, Random& r) {
  lw::WorldRandom(base, kTables, kTablesBytes, r);
  for (const auto& [guard, table, entries, count] :
       {std::tuple{ts::kTitledPageGuard, ts::kTitledPages, kPageEntries, 9u},
        std::tuple{ts::kTitledStepGuard, ts::kTitledSteps, kStepEntries, 3u}}) {
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

void LayBlock(uint8_t* base, uint32_t block, Random& r, Kind kind, std::vector<uint32_t>& made) {
  lw::WorldRandom(base, block, kBlockBytes, r);
  const uint32_t s = block;
  auto sub = [&r] { return r.Below(3) == 0 ? kNone : r.Below(20); };
  auto child = [&r] { return r.Below(6) == 0 ? kNone : r.Below(60); };
  auto named = [&r] { return r.Below(6) == 0 ? kNone : r.Below(48); };
  guest::StoreU32(base, s + 9680, r.Below(8) == 0 ? 0x80000000u | r.Below(4) : 16 + r.Below(4));
  // The list helpers' words (ShowSelection, SetArrows, the toggle, the
  // scroll text).
  for (const uint32_t at : {9724u, 9728u, 9740u}) guest::StoreU32(base, s + at, sub());
  for (const uint32_t at : {9752u, 9756u, 9760u, 9764u, 9768u, 9816u}) {
    guest::StoreU32(base, s + at, child());
  }
  for (const uint32_t at : {9800u, 9804u}) {
    guest::StoreU32(base, s + at, r.Below(4) == 0 ? kNone - r.Below(3) : uint32_t(r.Next()));
  }
  guest::StoreU32(base, s + 9784, r.Below(5) == 0 ? (r.Below(2) == 0 ? 0 : kNone) : r.Below(5));
  guest::StoreU32(base, s + 9772, 0);
  guest::StoreU32(base, s + 9776, 0);
  // The cursor's node and child (OnItemMoved).
  guest::StoreU32(base, s + 9744, r.Below(60));
  guest::StoreU32(base, s + 9828, r.Below(60));
  for (uint32_t at = 9832; at <= 9932; at += 4) guest::StoreU32(base, s + at, named());
  for (uint32_t i = 0; i < 30; i++) guest::StoreU32(base, s + 9936 + 4 * i, named());
  guest::StoreU32(base, s + 10820, r.Below(5) == 0 ? kNone - r.Below(2) : r.Below(450));
  if (r.Below(4) == 0) guest::StoreU32(base, s + 10140, guest::LoadU32(base, s + 10820));
  // The page block and the screen's words.
  guest::StoreU32(base, s + 9644, r.Below(6) == 0 ? kNone : r.Below(9));
  guest::StoreU32(base, s + 9648, r.Below(9));
  guest::StoreU32(base, s + 9652, r.Below(9));
  guest::StoreU32(base, s + 9656, r.Below(3));
  guest::StoreU32(base, s + 9660, r.Below(2) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, s + 9668, r.Below(2));
  guest::StoreU32(base, s + 9672, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, s + 9688, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, s + 9696, r.Below(2));
  guest::StoreU32(base, s + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  // The lists (as the list part's harness shapes one).
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t list = s + kLists[k];
    const bool toggle = kLists[k] == 10288;
    guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
    guest::StoreU32(base, list + rs::kListCount, r.Below(4) == 0 ? r.Below(12) : (toggle ? 2 : 10));
    guest::StoreU32(base, list + rs::kListShown, r.Below(4) == 0 ? r.Below(12) : 5);
    guest::StoreU32(base, list + rs::kListScroll, r.Below(4) == 0 ? kNone : r.Below(5));
    guest::StoreU32(base, list + rs::kListScrollBefore, r.Below(4) == 0 ? kNone : r.Below(5));
    if (r.Below(3) == 0) {
      guest::StoreU32(base, list + rs::kListScrollBefore,
                      guest::LoadU32(base, list + rs::kListScroll));
    }
    const uint32_t n = toggle ? 3 : 10;
    guest::StoreU32(base, list + rs::kListSelection,
                    r.Below(8) == 0 ? kNone - r.Below(2) : r.Below(n));
    guest::StoreU32(base, list + rs::kListSelectionBefore,
                    r.Below(8) == 0 ? kNone - r.Below(2) : r.Below(n));
    for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
      guest::StoreU32(base, list + field, AnyAction(r));
    }
    guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
    guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
    guest::StoreU32(base, list + rs::kListWrapped, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    uint32_t items = block + kItemsAt + 0x40 * k;
    if (kind == Kind::kNoDelete || kind == Kind::kRelease || kind == Kind::kInit) {
      items = 0;
      if (r.Below(4) != 0) {
        items = memory::AllocPages(16, 4096);
        lw::WorldRandom(base, items, 16, r);
        made.push_back(items);
      }
    }
    guest::StoreU32(base, list + rs::kListBlock, items);
  }
  LayNodeTable(base, s + 10360, r, true);
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
  // The list part, its vtable's slots from the stand-ins.
  const uint32_t part = block + kPartAt, part_vtable = part + 0x80;
  guest::StoreU32(base, part, part_vtable);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, part_vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, part + 4, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, part + 8, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, part + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, s + 9676, r.Below(4) == 0 ? 0 : part);
  // The screen's vtable.
  const uint32_t vtable = block + kVtableAt;
  guest::StoreU32(base, s, vtable);
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, vtable + 100, Pick(r, {0x82256FE8, 0x82237008, kSetR4}));
  // Type6ListScreen::SetUpMenus' slot 2 (SetItemTexts or a stand-in).
  // Its slot 25 the screen's own ItemInfo (play's: the info LoadItem reads
  // filled, not its frame's leftovers).
  if (kind == Kind::kInit) {
    guest::StoreU32(base, vtable + 8, Pick(r, {0x822336C0, kEmpty, kSetR4}));
    guest::StoreU32(base, vtable + 100, 0x82237008);
  }
  // The type 6 list screen's list part (+10824): its inner's (+20) list's
  // (+40) block 0 or pages for the destructors.
  if (kind == Kind::kNoDelete || kind == Kind::kRelease) {
    uint32_t items = 0;
    if (r.Below(2) == 0) {
      items = memory::AllocPages(16, 4096);
      made.push_back(items);
    }
    guest::StoreU32(base, s + 10824 + 20 + 40 + rs::kListBlock, items);
  }
  // The screen fades: counts 0 mostly.
  lw::WorldRandom(base, kFades, kFadesBytes, r);
  for (const uint32_t at : {mode_tick::kFadeInCounts, mode_tick::kFadeOutCounts}) {
    for (uint32_t layer = 0; layer < 2; layer++) {
      guest::StoreU32(base, at + 4 * layer, r.Below(2) == 0 ? 0 : 1 + r.Below(5));
    }
  }
  LayTables(base, r);
}

void Run(Context& h, const lw::MovieWorld& w, uint32_t block, const Function& f) {
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
      LayBlock(base, block, r, f.kind, made);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      lw::WorldRandom(base, sp - kBelow, kBelow + kAbove, r);
      const uint64_t high = r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0;
      t.r3.u64 = high | block;
      t.r4.u64 = r.Below(2) == 0 ? block + kDispatcherAt : r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      if (f.kind == Kind::kNoDelete) t.r4.u64 &= ~1ull;
      if (f.kind == Kind::kDispatcher) t.r4.u64 = block + kDispatcherAt;
      if (f.kind == Kind::kRow) t.r4.u64 = r.Below(3);
      if (f.kind == Kind::kMarks) t.r4.u64 = r.Below(4);
      std::vector<uint32_t> old_items;
      for (const uint32_t at : kLists) {
        old_items.push_back(guest::LoadU32(base, block + at + rs::kListBlock));
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
      if (f.kind == Kind::kInit) {
        for (uint32_t k = 0; k < 3; k++) {
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

// The deleting destructors with bit 0 of r4 set: a block of operator new's
// (its lists' item blocks 0) destroyed and deleted, compared by r3 and
// whether operator new hands the block out again.
void Deleting(Context& h) {
  uint8_t* const base = h.base;
  constexpr uint32_t kBytes = 11200;
  struct Case {
    const char* name;
    uint32_t address;
    GuestFunction recompiled, native;
  };
  const Case cases[] = {
      {"TitledListScreen::DeletingDestructor, deleting", 0x82257990, __imp__sub_82257990,
       hooks::TitledListScreenDeletingDestruct},
      {"NodeListScreen::DeletingDestructor, deleting", 0x82257940, __imp__sub_82257940,
       hooks::NodeListScreenDeletingDestruct},
      {"Type6ListScreen::DeletingDestructor, deleting", 0x82233478, __imp__sub_82233478,
       hooks::Type6ListScreenDeletingDestruct},
  };
  for (const Case& k : cases) {
    Results results(fmt::format("{} (sub_{:08X})", k.name, k.address));
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(Seed(uint64_t(k.address) << 20 ^ uint64_t(c)));
        const auto object = memory::crt::OperatorNew(base, kBytes);
        if (!object) {
          ITC_LOG_ERROR("harness: type67_screens: operator new failed");
          return;
        }
        lw::WorldRandom(base, *object, kBytes, r);
        for (const uint32_t at : kLists) {
          guest::StoreU32(base, *object + at + rs::kListBlock, 0);
        }
        guest::StoreU32(base, *object + 10824 + 20 + 40 + rs::kListBlock, 0);
        PPCContext t = CallContext(h);
        t.r3.u64 = *object;
        t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
        t.lr = 0x82000000 + 4 * r.Below(1024);
        if (v == 0) {
          ITC_FLAG(native_type67_screens) = false;
          k.recompiled(t, base);
          ITC_FLAG(native_type67_screens) = true;
        } else {
          k.native(t, base);
        }
        out[v].Word(t.r3.u32 == *object ? 1 : 0);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        const auto probe = memory::crt::OperatorNew(base, kBytes);
        out[v].Word(probe && *probe == *object ? 1 : 0);
        if (probe) memory::crt::OperatorDelete(*probe);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// Type6ListScreen::ItemInfo (r4 the item: 0 to 7 mostly, else 8 to 11 or
// any word) and NodeScreen::ItemInfo: r5 an info in a block of random words;
// the block and r3 (both halves, random) compared.
void ItemInfos(Context& h, uint32_t block) {
  uint8_t* const base = h.base;
  struct Case {
    const char* name;
    uint32_t address;
    GuestFunction recompiled, native;
  };
  const Case cases[] = {
      {"Type6ListScreen::ItemInfo", 0x82237008, __imp__sub_82237008,
       hooks::Type6ListScreenItemInfo},
      {"NodeScreen::ItemInfo", 0x82259998, __imp__sub_82259998, hooks::NodeScreenItemInfo},
  };
  for (const Case& k : cases) {
    Results results(fmt::format("{} (sub_{:08X})", k.name, k.address));
    for (int c = 0; c < 12; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(Seed(uint64_t(k.address) << 16 ^ uint64_t(c)));
        lw::WorldRandom(base, block, 0x100, r);
        PPCContext t = CallContext(h);
        t.r3.u64 = r.Next();
        t.r4.u64 = r.Below(4) == 0 ? (r.Below(2) == 0 ? 8 + r.Below(4) : r.Next()) : r.Below(8);
        t.r5.u64 = block + 4 * r.Below(48);
        if (v == 0) {
          ITC_FLAG(native_type67_screens) = false;
          k.recompiled(t, base);
          ITC_FLAG(native_type67_screens) = true;
        } else {
          k.native(t, base);
        }
        out[v].Bytes(Read(base, block, 0x100));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void Type67TitledList(Context& h) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR(
          "harness: type67_screens: movie {} is in the table; the titled list skipped", id);
      return;
    }
  }
  const lw::MovieWorld w;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  if (!w.ok() || block == 0) {
    ITC_LOG_ERROR("harness: type67_screens: no blocks for the titled list");
    if (block != 0) memory::Free(block);
    return;
  }
  const lw::SavedGlobals saved(base);
  lw::BuildStringTable(base, w);

  // The register saves: the function's own (its frame's top bytes) and
  // those of the callees the native side runs as plain C++ (at their frames'
  // tops below it: the pages and steps, the list helpers, the node table's
  // Refresh and theirs, ~NodeListScreen); in the frames of SetScrollText,
  // which calls SetText, +48 to +79 (its varargs, which the native side
  // passes 0 for).
  auto saves = [](std::initializer_list<std::pair<uint32_t, uint32_t>> list) {
    return Ranges(list);
  };
  using K = Kind;
  const Function functions[] = {
      {"TitledListScreen::Reset", 0x82257558, 4, K::kAny, {}, __imp__sub_82257558,
       hooks::TitledListScreenReset},
      {"TitledListScreen::Page7", 0x82257490, 8, K::kAny, {}, __imp__sub_82257490,
       hooks::TitledListScreenPage7},
      {"ListScreen::LeavePage", 0x82255818, 8, K::kAny, {}, __imp__sub_82255818,
       hooks::ListScreenLeavePage},
      {"TitledListScreen::Page3", 0x822572F0, 8, K::kAny, saves({{16, 0}}), __imp__sub_822572F0,
       hooks::TitledListScreenPage3},
      {"TitledListScreen::Page4", 0x82257360, 16, K::kAny, saves({{24, 0}}), __imp__sub_82257360,
       hooks::TitledListScreenPage4},
      {"TitledListScreen::Page2", 0x82257148, 24, K::kAny,
       saves({{32, 0}, {168, 112}, {280, 240}}), __imp__sub_82257148,
       hooks::TitledListScreenPage2},
      {"TitledListScreen::OnItemMoved", 0x82257B78, 16, K::kAny,
       saves({{80, 0}, {208, 176}, {376, 304}}), __imp__sub_82257B78,
       hooks::TitledListScreenOnItemMoved},
      {"TitledListScreen::Step0", 0x82257D48, 16, K::kAny,
       saves({{32, 0}, {168, 128}, {208, 176}, {328, 256}}), __imp__sub_82257D48,
       hooks::TitledListScreenStep0},
      {"TitledListScreen::Step1", 0x82257FF8, 12, K::kAny, saves({{24, 0}}), __imp__sub_82257FF8,
       hooks::TitledListScreenStep1},
      {"TitledListScreen::Step2", 0x822580E0, 12, K::kAny,
       saves({{24, 0}, {192, 112}, {320, 288}, {488, 416}}), __imp__sub_822580E0,
       hooks::TitledListScreenStep2},
      {"TitledListScreen::Draw", 0x822579E8, 24, K::kAny,
       saves({{48, 0}, {240, 208}, {376, 320}, {488, 448}}), __imp__sub_822579E8,
       hooks::TitledListScreenDraw},
      {"TitledListScreen::Update", 0x82258998, 16, K::kAny,
       saves({{8, 0}, {144, 112}, {320, 224}, {440, 368}, {600, 528}}), __imp__sub_82258998,
       hooks::TitledListScreenUpdate},
      {"NodeListScreen::~NodeListScreen", 0x82257000, 8, K::kRelease, saves({{16, 0}}),
       __imp__sub_82257000, hooks::NodeListScreenDestruct},
      {"TitledListScreen::~TitledListScreen", 0x82257138, 8, K::kRelease, saves({{16, 0}}),
       __imp__sub_82257138, hooks::TitledListScreenDestruct},
      {"TitledListScreen::DeletingDestructor", 0x82257990, 4, K::kNoDelete,
       saves({{24, 0}, {128, 112}}),
       __imp__sub_82257990, hooks::TitledListScreenDeletingDestruct},
      {"NodeListScreen::DeletingDestructor", 0x82257940, 4, K::kNoDelete,
       saves({{24, 0}, {128, 112}}),
       __imp__sub_82257940, hooks::NodeListScreenDeletingDestruct},
      // The type 6 list screen's own slots and destructors (type6_list.cpp).
      {"Type6ListScreen::SetItemTexts", 0x822336C0, 8, K::kRow, saves({{32, 0}, {80, 48}}),
       __imp__sub_822336C0, hooks::Type6ListScreenSetItemTexts},
      {"Type6ListScreen::ShowMarks", 0x82233730, 8, K::kMarks, saves({{32, 0}}),
       __imp__sub_82233730, hooks::Type6ListScreenShowMarks},
      {"Type6ListScreen::Back", 0x822337B0, 8, K::kDispatcher, saves({{16, 0}, {120, 112}}),
       __imp__sub_822337B0, hooks::Type6ListScreenBack},
      // SetUpMenus: LoadItem's saves, and SetItemTexts' varargs words (its
      // frame's +48 to +79, below SetUpMenus' 144 bytes).
      {"Type6ListScreen::SetUpMenus", 0x82233378, 12, K::kInit,
       saves({{40, 0}, {160, 144}, {224, 192}}),
       __imp__sub_82233378, hooks::Type6ListScreenSetUpMenus},
      // The destructors: ~NodeListScreen's and ~ListPartInner's saves.
      {"Type6ListScreen::~Type6ListScreen", 0x822332D8, 8, K::kRelease,
       saves({{16, 0}, {112, 96}, {208, 192}}),
       __imp__sub_822332D8, hooks::Type6ListScreenDestruct},
      {"Type6ListScreen::DeletingDestructor", 0x82233478, 4, K::kNoDelete,
       saves({{24, 0}, {128, 112}, {224, 208}, {320, 304}}), __imp__sub_82233478, hooks::Type6ListScreenDeletingDestruct},
  };
  for (const Function& f : functions) {
    Run(h, w, block, f);
  }
  Deleting(h);
  ItemInfos(h, block);
  memory::Free(block);
  saved.Restore();
}

}  // namespace harness
