// The list screens' shared helpers, item loaders and handlers
// (src/type67_screens/list_helpers.cpp, docs/functions/type67-screens.md)
// against the recompiled functions, run under the type67_screens harness
// (Type67Screens calls Type67ListHelpers). The originals run with
// --native_type67_screens off (their calls to each other through the
// adapters, so the originals), the plain C++ through the register steps
// (hooks/type67_screens.h).
//
// The world: the loading screen's UI movie world (loading_world.h, as the
// node tables' and the list part's cases use it: the leaves' +108 made 1 to
// 7 so SetText finds a text record; the screen's movie id 16 to 19, not in
// the movie table, so UiMovies::Find answers the word at 0x826C5CDC, pointed
// at movie 0), and a block of our own: the screen (random words, its node
// and child words in the ranges the movie world has, its named animations
// -1 or 0 to 47, the node arrays at +9772 and +9776 ours or 0, the node table
// at +9672 and the list part at +9676 ours or 0), a menu list (its selection
// and the one before), and the screen's, the node table's and the list part's
// vtables, each slot picked per case from stand-ins (the empty handler
// sub_82106540; sub_820CC470, which stores r4 at r3 + 20, so a call's r3 and
// r4 show; sub_82090780, which answers r3's +80) and, for slot 25, the
// screens' ItemInfo functions (NodeListScreen's, ours; Type6ListScreen's and
// Type7ListScreen's, recompiled). UiMovie::Run's update (sub_8211D2F8) and
// ApplyAnimation (sub_825A08E0) are stood in for, recorded and answered from
// the case's generator. FinishNamed, PlayNamed, LoopNamed, ResetPose,
// SetText, ClearText, ScrollText and sub_8211C5F8 run for real on both sides.
// Compared exactly (nothing here is a float): the recorded calls, the block,
// the movie world, the missing movie's word, the stack below and above the
// call (the originals' register saves left out, and in frames that call
// SetText the parameter words its varargs go to, which the native side
// passes 0 for: the user's call of 2026-10-30), r1 and r3. The deleting
// destructor with bit 0 set runs on blocks of operator new's, compared by r3
// and whether the block is handed out again.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/type67_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type67_screens);

namespace harness {

namespace {

namespace ts = type67_screens;
namespace lw = loading_world;

constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kEmpty = 0x82106540, kSetR4 = 0x820CC470, kGetter = 0x82090780;

// The block's layout (the screen at its start).
constexpr uint32_t kListAt = 0x2C00;     // the menu list
constexpr uint32_t kArraysAt = 0x2D00;   // the node arrays (+9772, +9776)
constexpr uint32_t kTableAt = 0x2E00;    // the node table (+9672), its vtable +0x80
constexpr uint32_t kPartAt = 0x2F00;     // the list part (+9676), its vtable +0x80
constexpr uint32_t kDispatcherAt = 0x3200;
constexpr uint32_t kVtableAt = 0x3800;   // the screen's vtable (32 slots)
constexpr uint32_t kBlockBytes = 0x4000;

constexpr uint32_t kBelow = 1024;  // the stack compared below the call's r1
constexpr uint32_t kAbove = 256;   // and above it

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

// The stack ranges left out, as [sp - from, sp - to).
using Ranges = std::vector<std::pair<uint32_t, uint32_t>>;

// What r4 and r5 hold beyond random words.
enum class Args {
  kAny,       // the dispatcher in r4
  kList,      // r4 the menu list (a selection mostly 0 to 9)
  kToggle,    // r4 the menu list (a selection mostly 0 to 2)
  kArrows,    // r4 and r5 mostly 0 or 1
  kScroll,    // r4 a string id (now and then +10140), r5 mostly 0 or 1
  kItem,      // r4 an item mostly 0 to 9, r5 an index mostly 0 to 4
  kNoDelete,  // r4 flags with bit 0 clear
  kInfo,      // r5 a word of the block's
};

struct Function {
  const char* name;
  uint32_t address;
  int cases;
  Args args;
  Ranges skip;
  GuestFunction recompiled, native;
};

uint32_t Pick(Random& r, std::initializer_list<uint32_t> targets) {
  return *(targets.begin() + r.Below(uint32_t(targets.size())));
}

void LayBlock(uint8_t* base, uint32_t block, Random& r) {
  lw::WorldRandom(base, block, kBlockBytes, r);
  const uint32_t s = block;
  auto sub = [&r] { return r.Below(3) == 0 ? kNone : r.Below(20); };
  auto child = [&r] { return r.Below(6) == 0 ? kNone : r.Below(60); };
  auto named = [&r] { return r.Below(6) == 0 ? kNone : r.Below(48); };
  guest::StoreU32(base, s + 9680, r.Below(8) == 0 ? 0x80000000u | r.Below(4) : 16 + r.Below(4));
  for (const uint32_t at : {9724u, 9728u, 9740u}) guest::StoreU32(base, s + at, sub());
  guest::StoreU32(base, s + 9732, r.Below(4) == 0 ? kNone : r.Below(20));
  for (const uint32_t at : {9748u, 9752u, 9756u, 9760u, 9764u, 9768u, 9816u}) {
    guest::StoreU32(base, s + at, child());
  }
  guest::StoreU32(base, s + 9812, r.Below(54));
  for (const uint32_t at : {9800u, 9804u}) {
    guest::StoreU32(base, s + at, r.Below(4) == 0 ? kNone - r.Below(3) : uint32_t(r.Next()));
  }
  guest::StoreU32(base, s + 9784, r.Below(5) == 0 ? (r.Below(2) == 0 ? 0 : kNone) : r.Below(5));
  for (uint32_t k = 0; k < 2; k++) {
    const uint32_t array = block + kArraysAt + 0x40 * k;
    for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, array + 4 * i, child());
    guest::StoreU32(base, s + 9772 + 4 * k, r.Below(4) == 0 ? 0 : array);
  }
  guest::StoreU32(base, s + 9788, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  for (const uint32_t at : {9892u, 9896u, 9900u}) guest::StoreU32(base, s + at, named());
  for (uint32_t i = 0; i < 30; i++) guest::StoreU32(base, s + 9936 + 4 * i, named());
  guest::StoreU32(base, s + 9712, r.Below(4) == 0 ? kNone : r.Below(60));
  for (const uint32_t at : {9716u, 9720u}) {
    guest::StoreU32(base, s + at, r.Below(3) == 0 ? 0 : r.Below(60));
  }
  guest::StoreU32(base, s + 9696, r.Below(2));
  guest::StoreU32(base, s + 9656, r.Below(2) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, s + 9660, r.Below(2) == 0 ? kNone : r.Below(5));
  guest::StoreU32(base, s + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  // The menu list: its selection and the one before.
  const uint32_t list = block + kListAt;
  guest::StoreU32(base, list + 16, r.Below(6) == 0 ? kNone - r.Below(2) : r.Below(10));
  guest::StoreU32(base, list + 20, r.Below(6) == 0 ? kNone - r.Below(2) : r.Below(10));
  // The node table and the list part, their vtables' slots from the
  // stand-ins.
  for (const uint32_t at : {kTableAt, kPartAt}) {
    const uint32_t object = block + at, vtable = object + 0x80;
    guest::StoreU32(base, object, vtable);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU32(base, vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
    }
    guest::StoreU32(base, object + 80, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
  guest::StoreU32(base, s + 9672, r.Below(3) == 0 ? 0 : block + kTableAt);
  guest::StoreU32(base, s + 9676, r.Below(3) == 0 ? 0 : block + kPartAt);
  // The screen's vtable.
  const uint32_t vtable = block + kVtableAt;
  guest::StoreU32(base, s, vtable);
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, vtable + 4 * i, Pick(r, {kEmpty, kSetR4, kGetter}));
  }
  guest::StoreU32(base, vtable + 100, Pick(r, {0x82256FE8, 0x82237008, 0x82242FA0, kSetR4}));
}

void Run(Context& h, const lw::MovieWorld& w, uint32_t block, const Function& f) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
  for (int c = 0; c < f.cases; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed((uint64_t(f.address) << 16) ^ uint64_t(c)));
      const uint32_t saved_missing = guest::LoadU32(base, kMissingMovie);
      lw::ShapeMovieWorld(base, w, r);
      for (uint32_t k = 0; k < lw::kLeaves; k++) {
        guest::StoreU32(base, w.leaf(k) + 108, 1 + r.Below(7));
      }
      guest::StoreU32(base, kMissingMovie, w.movie(0));
      LayBlock(base, block, r);
      const uint32_t list = block + kListAt;
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      lw::WorldRandom(base, sp - kBelow, kBelow + kAbove, r);
      const uint64_t high = r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0;
      t.r3.u64 = high | block;
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      auto small = [&r](uint32_t n) -> uint64_t {
        return r.Below(6) == 0 ? (r.Below(2) == 0 ? r.Next() : kNone - r.Below(3)) : r.Below(n);
      };
      switch (f.args) {
        case Args::kAny:
          t.r4.u64 = block + kDispatcherAt;
          break;
        case Args::kList:
          t.r4.u64 = list;
          break;
        case Args::kToggle:
          t.r4.u64 = list;
          guest::StoreU32(base, list + 16, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(3));
          break;
        case Args::kArrows:
          t.r4.u64 = small(2);
          t.r5.u64 = small(2);
          break;
        case Args::kScroll:
          t.r4.u64 = r.Below(5) == 0 ? kNone - r.Below(2) : r.Below(450);
          if (r.Below(4) == 0) guest::StoreU32(base, block + 10140, uint32_t(t.r4.u64));
          t.r5.u64 = small(2);
          break;
        case Args::kItem:
          t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(10);
          // An index 0 to 4: past 4, ColorListScreen::LoadItem's read (the
          // info's +20 + 4 r5, when below the info's +8) reaches the
          // original's register saves (+120 on), which the native side
          // doesn't write; in play the count is the item's colors (2 at
          // most).
          t.r5.u64 = r.Below(5) == 0 ? kNone - r.Below(4) : r.Below(5);
          break;
        case Args::kNoDelete:
          t.r4.u64 &= ~1ull;
          break;
        case Args::kInfo:
          t.r5.u64 = block + 0x3000 + 4 * r.Below(16);
          break;
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
      out[v].Bytes(Read(base, block, kBlockBytes));
      out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
      out[v].Word(guest::LoadU32(base, kMissingMovie));
      std::vector<uint8_t> stack = Read(base, sp - kBelow, kBelow + kAbove);
      for (const auto& [from, to] : f.skip) {
        for (uint32_t i = kBelow - from; i < kBelow - to; i++) stack[i] = 0;
      }
      out[v].Bytes(stack);
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      guest::StoreU32(base, kMissingMovie, saved_missing);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// ListScreen::DeletingDestructor with bit 0 of r4 set: a block of operator
// new's destroyed and deleted, compared by r3 and whether operator new hands
// the block out again.
void Deleting(Context& h) {
  uint8_t* const base = h.base;
  constexpr uint32_t kBytes = 11200;
  Results results("ListScreen::DeletingDestructor, deleting (sub_82252248)");
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed(0x82252248ull << 20 ^ uint64_t(c)));
      const auto object = memory::crt::OperatorNew(base, kBytes);
      if (!object) {
        ITC_LOG_ERROR("harness: type67_screens: operator new failed");
        return;
      }
      lw::WorldRandom(base, *object, kBytes, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = *object;
      t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type67_screens) = false;
        __imp__sub_82252248(t, base);
        ITC_FLAG(native_type67_screens) = true;
      } else {
        hooks::ListScreenDeletingDestruct(t, base);
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

}  // namespace

void Type67ListHelpers(Context& h) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR(
          "harness: type67_screens: movie {} is in the table; the list helpers skipped", id);
      return;
    }
  }
  const lw::MovieWorld w;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  if (!w.ok() || block == 0) {
    ITC_LOG_ERROR("harness: type67_screens: no blocks for the list helpers");
    if (block != 0) memory::Free(block);
    return;
  }
  const lw::SavedGlobals saved(base);
  lw::BuildStringTable(base, w);

  // The register saves: the function's own (its frame's top bytes) and
  // those of the callees the native side runs as plain C++ (at their
  // frames' tops below it); in frames that call SetText, +48 to +79.
  auto saves = [](std::initializer_list<std::pair<uint32_t, uint32_t>> list) {
    return Ranges(list);
  };
  using A = Args;
  const Function functions[] = {
      {"ListScreen::~ListScreen", 0x82251EB8, 4, A::kAny, {}, __imp__sub_82251EB8,
       hooks::ListScreenDestruct},
      {"ListScreen::DeletingDestructor", 0x82252248, 4, A::kNoDelete, saves({{16, 0}}),
       __imp__sub_82252248, hooks::ListScreenDeletingDestruct},
      {"NodeListScreen::ItemInfo", 0x82256FE8, 4, A::kInfo, {}, __imp__sub_82256FE8,
       hooks::NodeListScreenItemInfo},
      {"NodeListScreen::LoadItem", 0x82257060, 16, A::kItem, saves({{16, 0}}),
       __imp__sub_82257060, hooks::NodeListScreenLoadItem},
      {"ColorListScreen::LoadItem", 0x82255238, 16, A::kItem, saves({{24, 0}}),
       __imp__sub_82255238, hooks::ColorListScreenLoadItem},
      {"ListScreen::ShowSelection", 0x822518B0, 24, A::kList, saves({{56, 0}}),
       __imp__sub_822518B0, hooks::ListScreenShowSelection},
      {"ListScreen::LoopItemNode", 0x82251A20, 12, A::kList, saves({{24, 0}}),
       __imp__sub_82251A20, hooks::ListScreenLoopItemNode},
      {"ListScreen::LoopSecondNode", 0x82251B80, 12, A::kList, saves({{24, 0}}),
       __imp__sub_82251B80, hooks::ListScreenLoopSecondNode},
      {"ListScreen::LoopThirdNode", 0x82251AD0, 12, A::kList, saves({{24, 0}}),
       __imp__sub_82251AD0, hooks::ListScreenLoopThirdNode},
      {"ListScreen::SetScrollText", 0x82251D00, 16, A::kScroll, saves({{40, 0}, {80, 48}}),
       __imp__sub_82251D00, hooks::ListScreenSetScrollText},
      {"ListScreen::ClearScrollText", 0x82251DA0, 8, A::kAny, saves({{16, 0}}),
       __imp__sub_82251DA0, hooks::ListScreenClearScrollText},
      {"ListScreen::SetArrows", 0x82251E10, 16, A::kArrows, saves({{40, 0}}),
       __imp__sub_82251E10, hooks::ListScreenSetArrows},
      {"ListScreen::OnToggleMoved", 0x82252068, 16, A::kToggle, saves({{40, 0}, {168, 128}}),
       __imp__sub_82252068, hooks::ListScreenOnToggleMoved},
      {"ListScreen::ShowToggle", 0x82252198, 12, A::kToggle, saves({{32, 0}, {168, 128}}),
       __imp__sub_82252198, hooks::ListScreenShowToggle},
      {"ListScreen::SetIcons", 0x82251C30, 8, A::kAny, saves({{24, 0}}), __imp__sub_82251C30,
       hooks::ListScreenSetIcons},
      {"ListScreen::SetUpMovie", 0x82251EC8, 16, A::kAny, saves({{24, 0}, {152, 112}}),
       __imp__sub_82251EC8, hooks::ListScreenSetUpMovie},
      {"ListScreen::Show", 0x82251F98, 16, A::kAny,
       saves({{32, 0}, {80, 48}, {152, 128}, {280, 240}}), __imp__sub_82251F98,
       hooks::ListScreenShow},
      {"ListScreen::Draw", 0x82251520, 8, A::kAny, saves({{24, 0}}), __imp__sub_82251520,
       hooks::ListScreenDraw},
      {"ListScreen::Update", 0x822517B8, 24, A::kAny, saves({{24, 0}}), __imp__sub_822517B8,
       hooks::ListScreenUpdate},
      {"ListScreen::Suspend", 0x82251580, 4, A::kAny, {}, __imp__sub_82251580,
       hooks::ListScreenSuspend},
      {"ListScreen::Resume", 0x82251590, 4, A::kAny, {}, __imp__sub_82251590,
       hooks::ListScreenResume},
      {"ListScreen::Open", 0x822517A0, 4, A::kAny, {}, __imp__sub_822517A0,
       hooks::ListScreenOpen},
  };
  for (const Function& f : functions) {
    Run(h, w, block, f);
  }
  Deleting(h);
  memory::Free(block);
  saved.Restore();
}

}  // namespace harness
