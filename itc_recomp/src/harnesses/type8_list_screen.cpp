// The type 8 list screen's pages, steps, tables, slots and destructors
// (src/type8_screens/list_screen.cpp, docs/functions/type8-screens.md)
// against the recompiled functions, run under the type8_screens harness
// (Type8Screens calls Type8ListScreen). The originals run with
// --native_type8_screens off, the plain C++ through the register steps
// (hooks/type8_screens.h; DimList and PairedNodeTable::Clear, which have no
// adapters, through their plain C++ with the adapters' calls).
//
// The world, as the titled list's harness (type67_titled_list.cpp) lays
// it: the loading screen's UI movie world (the leaves' +108 made 1 to 7 so
// SetText finds a text record; the screen's movie id 16 to 19, not in the
// movie table, so UiMovies::Find answers the word at 0x826C5CDC, pointed at
// movie 0), and a block of our own: the screen (random words, its node,
// child, string and named animation words in the movie world's ranges, the
// page block's page -1 or 0 to 9 and step 0 to 5, the first-call word 0 or
// 1), its three menu lists in the ranges play gives (the kinds +10144: 3
// items; the courses +10216: 8, 6 shown, scroll 0 to 2; the toggle +10288:
// 2), with the players' action mappers, their item blocks ours, or 0 or
// pages of the allocation layer's for the destructors and SetUpMenus; its
// node table (+10360, as the node tables' cases lay one, paired; its
// vtable's slot 1 the empty handler or sub_82090780), a stand-in list part
// (+9676, or 0) and the screen's vtable with their slots from stand-ins (the
// empty handler sub_82106540; sub_820CC470, which stores r4 at r3 + 20;
// sub_82090780, which answers r3's +80) and now and then the screen's own
// (SetItemTexts, ItemInfo, ShowMarks, SetLabels), a fake sound manager
// (PlayCue2D and StopCue stand-ins storing their arguments, IsCuePlaying
// sub_82090780 answering its +80), the screen fades' words (counts 0 or
// small), and the tables' .data words: each guard's bit 0 clear (the table
// random, built by the call) or set (the table holding its entries).
// UiMovie::Run's update (sub_8211D2F8) and ApplyAnimation (sub_825A08E0)
// are stood in for, recorded and answered from the case's generator. For
// the destructors, the real list part at +10844 with its inner's four menus'
// item blocks 0 or pages. Compared exactly (no float math): the recorded
// calls, the new and freed item blocks, the block, the movie world, the
// globals (the tables and guards among them), the function's own frame (its
// back chain and the words above its outgoing area, its register saves
// left out; callees' frames are their own harnesses'), or for frameless
// ones the stack below the call, the caller's area, r1 and r3. The
// deleting destructors with bit 0 set run on blocks of operator new's,
// compared by r3 and whether the block is handed out again.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/type67_screens.h"
#include "hooks/type8_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "race_screens/race_screens.h"
#include "type67_screens/type67_screens.h"
#include "type8_screens/type8_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <tuple>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type8_screens);

namespace harness {

// The node tables' layout (type67_screen_tables.cpp).
void LayNodeTable(uint8_t* base, uint32_t table, Random& r, bool paired);

namespace {

namespace t8 = type8_screens;
namespace lw = loading_world;
namespace rs = race_screens;

constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kEmpty = 0x82106540, kSetR4 = 0x820CC470, kGetter = 0x82090780;
constexpr uint32_t kFades = 0x826A50AC, kFadesBytes = 0x40;
constexpr uint32_t kTables = t8::kListBasePages, kTablesBytes = 0x88;

// The block's layout (the screen at its start, 11,368 bytes with its part).
constexpr uint32_t kVtablesAt = 0x2D00;  // the mappers' vtables (0x40 each)
constexpr uint32_t kMappersAt = 0x2E00;  // 4 mappers of 0x200
constexpr uint32_t kManagerAt = 0x3600;  // the fake sound manager
constexpr uint32_t kManagerVtableAt = 0x3680;
constexpr uint32_t kPartAt = 0x3800;     // the stand-in list part, its vtable +0x80
constexpr uint32_t kVtableAt = 0x3900;   // the screen's vtable (32 slots)
constexpr uint32_t kNodeVtableAt = 0x3980;
constexpr uint32_t kItemsAt = 0x3A00;    // the lists' item blocks (0x40 each)
constexpr uint32_t kDispatcherAt = 0x3B00;
constexpr uint32_t kBlockBytes = 0x4000;
constexpr uint32_t kLists[3] = {10144, 10216, 10288};
constexpr uint32_t kCounts[3] = {3, 8, 2};
// The real list part's inner's four menus (the destructors).
constexpr uint32_t kPartMenus[4] = {10864 + 40 + 12, 10864 + 40 + 84, 10864 + 272 + 12,
                                    10864 + 272 + 84};

constexpr uint32_t kBelow = 1024;  // the stack read below the call's r1
constexpr uint32_t kAbove = 256;   // and above it

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kMissingMovie, 4},    {rs::kMapperEntries, 32},        {rs::kControllerEntries, 16},
    {kFades, kFadesBytes}, {menu_sounds::kSoundManager, 4}, {kTables, kTablesBytes},
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

// What r3 and r4 hold and the lists' item blocks.
enum class Kind {
  kAny,       // r4 the dispatcher (or the caller's word), item blocks ours
  kRelease,   // item blocks 0 or pages (the part's menus too)
  kNoDelete,  // the same, r4 flags with bit 0 clear
  kInit,      // item blocks 0 or pages, the new ones compared and freed
  kRow,       // r4 0 to 2 (the course list's scroll)
  kMarks,     // r4 -1 or 0 to 3
  kDim,       // r4 0 or 1, now and then any word
  kTable,     // r3 the node table (+10360)
};

struct Function {
  const char* name;
  uint32_t address;
  int cases;
  Kind kind;
  uint32_t frame, saves;  // the function's frame and its register saves (0: none)
  GuestFunction recompiled;
  std::function<void(PPCContext&, uint8_t*)> native;
  // Whether the stack below a frameless call is compared (not for a tail
  // call into functions with register saves).
  bool below = true;
};

uint32_t Pick(Random& r, std::initializer_list<uint32_t> targets) {
  return *(targets.begin() + r.Below(uint32_t(targets.size())));
}

// An action id: mostly the menus' (151 to 180), sometimes any word.
uint32_t AnyAction(Random& r) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30);
}

// The tables' entries (as the originals build them).
constexpr uint32_t kPageEntries[10] = {kEmpty,     kEmpty,     0x82257588, 0x82257768, 0x82258A40,
                                       0x82258AD0, 0x822578D8, kEmpty,     0x82257900, 0x82255818};
constexpr uint32_t kStepEntries[6] = {0x82258158, 0x82258648, 0x82258BE0,
                                      0x82258438, 0x822580E0, 0x82258578};

void LayTables(uint8_t* base, Random& r) {
  lw::WorldRandom(base, kTables, kTablesBytes, r);
  for (const auto& [guard, table, entries, count] :
       {std::tuple{t8::kListBasePageGuard, t8::kListBasePages, kPageEntries, 10u},
        std::tuple{t8::kListBaseStepGuard, t8::kListBaseSteps, kStepEntries, 6u}}) {
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

// An item block for a menu MenuList::Release frees or Init replaces: 0 or
// pages of the allocation layer's.
uint32_t MadeItems(uint8_t* base, Random& r, std::vector<uint32_t>& made) {
  if (r.Below(4) == 0) return 0;
  const uint32_t items = memory::AllocPages(16, 4096);
  lw::WorldRandom(base, items, 16, r);
  made.push_back(items);
  return items;
}

void LayBlock(uint8_t* base, uint32_t block, Random& r, Kind kind, std::vector<uint32_t>& made) {
  lw::WorldRandom(base, block, kBlockBytes, r);
  const uint32_t s = block;
  auto sub = [&r] { return r.Below(3) == 0 ? kNone : r.Below(20); };
  auto child = [&r] { return r.Below(6) == 0 ? kNone : r.Below(60); };
  auto named = [&r] { return r.Below(6) == 0 ? kNone : r.Below(48); };
  auto text = [&r] { return r.Below(5) == 0 ? kNone - r.Below(2) : r.Below(450); };
  guest::StoreU32(base, s + 9680, r.Below(8) == 0 ? 0x80000000u | r.Below(4) : 16 + r.Below(4));
  // The list helpers' words (ShowSelection, SetArrows, the toggle, the
  // scroll text), the cursors (PlaceColorCursor), the item texts' sub-scene
  // and the nodes DimList shows.
  for (const uint32_t at : {9724u, 9728u, 9736u, 9740u, 9744u}) guest::StoreU32(base, s + at, sub());
  for (const uint32_t at : {9752u, 9756u, 9760u, 9764u, 9768u, 9808u, 9816u, 9820u, 9824u,
                            9828u}) {
    guest::StoreU32(base, s + at, child());
  }
  for (const uint32_t at : {9800u, 9804u}) {
    guest::StoreU32(base, s + at, r.Below(4) == 0 ? kNone - r.Below(3) : uint32_t(r.Next()));
  }
  guest::StoreU32(base, s + 9784, r.Below(5) == 0 ? (r.Below(2) == 0 ? 0 : kNone) : r.Below(5));
  guest::StoreU32(base, s + 9772, 0);
  guest::StoreU32(base, s + 9776, 0);
  for (uint32_t at = 9832; at <= 9932; at += 4) guest::StoreU32(base, s + at, named());
  for (uint32_t i = 0; i < 30; i++) guest::StoreU32(base, s + 9936 + 4 * i, named());
  for (uint32_t at = 10820; at <= 10840; at += 4) guest::StoreU32(base, s + at, text());
  if (r.Below(4) == 0) guest::StoreU32(base, s + 10140, guest::LoadU32(base, s + 10820));
  // The page block and the screen's words.
  guest::StoreU32(base, s + 9644, r.Below(6) == 0 ? kNone : r.Below(10));
  guest::StoreU32(base, s + 9648, r.Below(10));
  guest::StoreU32(base, s + 9652, r.Below(10));
  guest::StoreU32(base, s + 9656, r.Below(6));
  guest::StoreU32(base, s + 9660, r.Below(2) == 0 ? kNone : r.Below(6));
  guest::StoreU32(base, s + 9668, r.Below(2));
  guest::StoreU32(base, s + 9672, r.Below(3) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, s + 9688, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, s + 9696, r.Below(2));
  guest::StoreU32(base, s + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  const bool items_made = kind == Kind::kRelease || kind == Kind::kNoDelete || kind == Kind::kInit;
  // The lists (as the list part's harness shapes one), in play's ranges.
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t list = s + kLists[k], n = kCounts[k];
    guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
    guest::StoreU32(base, list + rs::kListCount, n);
    guest::StoreU32(base, list + rs::kListShown, k == 1 ? 6 : n);
    const uint32_t scroll = k == 1 ? r.Below(3) : 0;
    guest::StoreU32(base, list + rs::kListScroll, scroll);
    guest::StoreU32(base, list + rs::kListScrollBefore, r.Below(3) == 0 ? scroll : (k == 1 ? r.Below(3) : 0));
    const uint32_t shown = k == 1 ? 6 : n;
    guest::StoreU32(base, list + rs::kListSelection, r.Below(shown));
    guest::StoreU32(base, list + rs::kListSelectionBefore,
                    r.Below(8) == 0 ? kNone - r.Below(2) : r.Below(shown));
    for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
      guest::StoreU32(base, list + field, AnyAction(r));
    }
    guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
    guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
    guest::StoreU32(base, list + rs::kListWrapped, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    const uint32_t items = items_made ? MadeItems(base, r, made) : block + kItemsAt + 0x40 * k;
    guest::StoreU32(base, list + rs::kListBlock, items);
  }
  // The node table, its vtable ours (slot 1 a stand-in).
  LayNodeTable(base, s + 10360, r, true);
  guest::StoreU32(base, s + 10360, block + kNodeVtableAt);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, block + kNodeVtableAt + 4 * i, Pick(r, {kEmpty, kGetter}));
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
  // The fake sound manager: PlayCue2D (slot 10) and StopCue (slot 13)
  // storing their arguments, IsCuePlaying (slot 14) answering its +80.
  const uint32_t manager = block + kManagerAt, manager_vtable = block + kManagerVtableAt;
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, manager_vtable + 52, 0x820CE9F8);
  guest::StoreU32(base, manager_vtable + 56, kGetter);
  guest::StoreU32(base, manager + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
  // The stand-in list part, its vtable's slots from the stand-ins.
  const uint32_t part = block + kPartAt, part_vtable = part + 0x80;
  guest::StoreU32(base, part, part_vtable);
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, part_vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, part + 4, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, part + 8, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, part + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, s + 9676, r.Below(4) == 0 ? 0 : part);
  // The screen's vtable: stand-ins, and now and then the screen's own
  // slots 2 (SetItemTexts), 25 (ItemInfo), 26 (ShowMarks), 27 (SetLabels).
  const uint32_t vtable = block + kVtableAt;
  guest::StoreU32(base, s, vtable);
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, vtable + 8, Pick(r, {0x8223F5D8, kEmpty, kSetR4}));
  guest::StoreU32(base, vtable + 100, Pick(r, {0x82256FE8, 0x8223F598, kSetR4}));
  guest::StoreU32(base, vtable + 104, Pick(r, {0x8223F6C8, kEmpty, kSetR4}));
  // Slot 27's call leaves r4 as the calls before it left it: no stand-in
  // that reads it.
  guest::StoreU32(base, vtable + 108, Pick(r, {0x8223F648, kEmpty, kGetter}));
  // SetUpMenus' item infos are the screen's own (play's: the info LoadItem
  // reads filled, not its frame's leftovers).
  if (kind == Kind::kInit) guest::StoreU32(base, vtable + 100, 0x8223F598);
  // The real list part's inner's menus for the destructors.
  if (kind == Kind::kRelease || kind == Kind::kNoDelete) {
    for (const uint32_t at : kPartMenus) {
      guest::StoreU32(base, s + at + rs::kListBlock, MadeItems(base, r, made));
    }
  }
  // The screen fades: counts 0 mostly.
  lw::WorldRandom(base, kFades, kFadesBytes, r);
  for (const uint32_t at : {mode_tick::kFadeInCounts, mode_tick::kFadeOutCounts}) {
    for (uint32_t layer = 0; layer < 2; layer++) {
      guest::StoreU32(base, at + 4 * layer, r.Below(2) == 0 ? 0 : 1 + r.Below(5));
    }
  }
  // The dispatcher's ring: its count and start small.
  guest::StoreU32(base, block + kDispatcherAt + 204 + 600, r.Below(50));
  guest::StoreU32(base, block + kDispatcherAt + 204 + 604, r.Below(60));
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
      t.r3.u64 = high | (f.kind == Kind::kTable ? block + 10360 : block);
      t.r4.u64 = r.Below(2) == 0 ? block + kDispatcherAt : r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      if (f.kind == Kind::kNoDelete) t.r4.u64 &= ~1ull;
      if (f.kind == Kind::kRow) t.r4.u64 = r.Below(3);
      if (f.kind == Kind::kMarks) t.r4.u64 = r.Below(5) == 0 ? ~0ull : r.Below(4);
      if (f.kind == Kind::kDim) t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
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
        ITC_FLAG(native_type8_screens) = false;
        f.recompiled(t, base);
        ITC_FLAG(native_type8_screens) = true;
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
      if (f.frame != 0) {
        const uint32_t bottom = sp - f.frame;
        out[v].Bytes(Read(base, bottom, 4));
        out[v].Bytes(Read(base, bottom + 80, f.frame - 80 - f.saves));
      } else if (f.below) {
        out[v].Bytes(Read(base, sp - kBelow, kBelow));
      }
      out[v].Bytes(Read(base, sp, kAbove));
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
// (its lists' and the part's menus' item blocks 0) destroyed and deleted,
// compared by r3 and whether operator new hands the block out again.
void Deleting(Context& h) {
  uint8_t* const base = h.base;
  constexpr uint32_t kBytes = 11368;
  struct Case {
    const char* name;
    uint32_t address;
    GuestFunction recompiled, native;
  };
  const Case cases[] = {
      {"Type8ListBase::DeletingDestructor, deleting", 0x82257B20, __imp__sub_82257B20,
       hooks::Type8ListBaseDeletingDestruct},
      {"Type8ListScreen::DeletingDestructor, deleting", 0x8223F388, __imp__sub_8223F388,
       hooks::Type8ListScreenDeletingDestruct},
  };
  for (const Case& k : cases) {
    Results results(fmt::format("{} (sub_{:08X})", k.name, k.address));
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(Seed(uint64_t(k.address) << 20 ^ uint64_t(c)));
        const auto object = memory::crt::OperatorNew(base, kBytes);
        if (!object) {
          ITC_LOG_ERROR("harness: type8_screens: operator new failed");
          return;
        }
        lw::WorldRandom(base, *object, kBytes, r);
        for (const uint32_t at : kLists) {
          guest::StoreU32(base, *object + at + rs::kListBlock, 0);
        }
        for (const uint32_t at : kPartMenus) {
          guest::StoreU32(base, *object + at + rs::kListBlock, 0);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = *object;
        t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
        t.lr = 0x82000000 + 4 * r.Below(1024);
        if (v == 0) {
          ITC_FLAG(native_type8_screens) = false;
          k.recompiled(t, base);
          ITC_FLAG(native_type8_screens) = true;
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

// The functions without adapters: their plain C++ with the adapters' calls.
void NativeDimList(PPCContext& t, uint8_t* b) {
  const t8::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = t8::ListBaseDimList(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
}
void NativeClear(PPCContext& t, uint8_t* b) {
  const t8::Calls c = hooks::Type67ScreensCalls(t, b);
  t.r3.u64 = t8::PairedNodeTableClear(b, t.r3.u64, t.r1.u32, c);
}

}  // namespace

void Type8ListScreen(Context& h) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR(
          "harness: type8_screens: movie {} is in the table; the list screen skipped", id);
      return;
    }
  }
  const lw::MovieWorld w;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  if (!w.ok() || block == 0) {
    ITC_LOG_ERROR("harness: type8_screens: no blocks for the list screen");
    if (block != 0) memory::Free(block);
    return;
  }
  const lw::SavedGlobals saved(base);
  lw::BuildStringTable(base, w);
  using K = Kind;
  const Function functions[] = {
      {"PairedNodeTable::Clear", 0x82248A20, 8, K::kTable, 128, 32, __imp__sub_82248A20,
       NativeClear},
      {"Type8ListBase::DimList", 0x822587C8, 10, K::kDim, 128, 40, __imp__sub_822587C8,
       NativeDimList},
      {"Type8ListScreen::SetItemTexts", 0x8223F5D8, 6, K::kRow, 128, 40, __imp__sub_8223F5D8,
       hooks::Type8ListSetItemTexts},
      {"Type8ListScreen::ShowMarks", 0x8223F6C8, 8, K::kMarks, 128, 32, __imp__sub_8223F6C8,
       hooks::Type8ListShowMarks},
      {"Type8ListScreen::SetLabels", 0x8223F648, 4, K::kAny, 112, 16, __imp__sub_8223F648,
       hooks::Type8ListSetLabels},
      {"Type8ListScreen::SetUpMenus", 0x8223F3F0, 10, K::kInit, 176, 40, __imp__sub_8223F3F0,
       hooks::Type8ListSetUpMenus},
      {"Type8ListBase::Page2", 0x82257588, 16, K::kAny, 112, 24, __imp__sub_82257588,
       hooks::Type8ListPage2},
      {"Type8ListBase::Page3", 0x82257768, 16, K::kAny, 112, 24, __imp__sub_82257768,
       hooks::Type8ListPage3},
      {"Type8ListBase::Page4", 0x82258A40, 8, K::kAny, 96, 16, __imp__sub_82258A40,
       hooks::Type8ListPage4},
      {"Type8ListBase::Page5", 0x82258AD0, 12, K::kAny, 96, 16, __imp__sub_82258AD0,
       hooks::Type8ListPage5},
      {"Type8ListBase::Page6", 0x822578D8, 4, K::kAny, 0, 0, __imp__sub_822578D8,
       hooks::Type8ListPage6},
      {"Type8ListBase::Page8", 0x82257900, 6, K::kAny, 0, 0, __imp__sub_82257900,
       hooks::Type8ListPage8},
      {"Type8ListBase::Step0", 0x82258158, 12, K::kAny, 128, 40, __imp__sub_82258158,
       hooks::Type8ListStep0},
      {"Type8ListBase::Step1", 0x82258648, 10, K::kAny, 144, 48, __imp__sub_82258648,
       hooks::Type8ListStep1},
      {"Type8ListBase::Step2", 0x82258BE0, 10, K::kAny, 128, 32, __imp__sub_82258BE0,
       hooks::Type8ListStep2},
      {"Type8ListBase::Step3", 0x82258438, 10, K::kAny, 128, 32, __imp__sub_82258438,
       hooks::Type8ListStep3},
      {"Type8ListBase::Draw", 0x82258D60, 16, K::kAny, 224, 56, __imp__sub_82258D60,
       hooks::Type8ListDraw},
      {"Type8ListBase::Update", 0x82258EB0, 12, K::kAny, 160, 24, __imp__sub_82258EB0,
       hooks::Type8ListUpdate},
      {"Type8ListBase::~Type8ListBase", 0x82257548, 6, K::kRelease, 0, 0, __imp__sub_82257548,
       hooks::Type8ListBaseDestruct, false},
      {"Type8ListBase::DeletingDestructor", 0x82257B20, 4, K::kNoDelete, 112, 24,
       __imp__sub_82257B20, hooks::Type8ListBaseDeletingDestruct},
      {"Type8ListScreen::DeletingDestructor", 0x8223F388, 6, K::kNoDelete, 112, 24,
       __imp__sub_8223F388, hooks::Type8ListScreenDeletingDestruct},
  };
  for (const Function& f : functions) {
    Run(h, w, block, f);
  }
  Deleting(h);
  memory::Free(block);
  saved.Restore();
}

}  // namespace harness
