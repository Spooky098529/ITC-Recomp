// The list screens' list part and its inner part (src/type67_screens/list_part.cpp,
// docs/functions/type67-screens.md) against the recompiled functions, run
// under the type67_screens harness (Type67Screens calls Type67ListPart). The
// originals run with --native_type67_screens off (their calls to each other
// through the adapters, so the originals), the plain C++ through the
// register steps (hooks/type67_screens.h).
//
// The world: the loading screen's UI movie world (loading_world.h, as the
// node tables' cases use it: the leaves' +108 made 1 to 7 so SetText finds a
// text record; the part's movie id 16 to 19, not in the movie table, so
// UiMovies::Find answers the word at 0x826C5CDC, pointed at movie 0), and a
// block of our own: the part (its real vtable, +16 its inner part at +20 or
// now and then 0, +12 the movie id), the inner part (its real vtable, the
// page -1 or 0 to 2, the step -1 or 0 to 2, the queued step, the first-call
// word, the state 0 to 4, the leave word), its MenuList (+40: two items
// mostly, the selection mostly 0 or 1, the players' action mappers with
// ActionMapper::Pressed and Fired in their vtables, its item block ours, or
// for the functions that free it, 0 or pages from the allocation layer), a
// fake sound manager (as the type 2 dialog's harness makes it), and player
// 0's save data pointer put where its options' +20 is a word of ours.
// UiMovie::Run's update (sub_8211D2F8) and ApplyAnimation (sub_825A08E0)
// are stood in for, recorded and answered from the case's generator.
// Compared exactly (nothing here is a float): the recorded calls, the
// block, the movie world, the globals, the stack below and above the call
// (the originals' register saves left out, and in ListPartInner::Open's
// frame the parameter words SetText's varargs go to, which the native side
// passes 0 for: the user's call of 2026-10-30), the item blocks made or
// freed, and r3. The deleting destructors with bit 0 set run on blocks of
// operator new's, compared by r3 and whether the block is handed out again.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/type67_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
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
namespace rs = race_screens;

constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kSaveData = 0x826C5B58;  // the players' save data pointers
constexpr uint32_t kOptionsWord = 0x1F870;
constexpr uint32_t kNone = 0xFFFFFFFFu;

// The block's layout.
constexpr uint32_t kPartAt = 0x0;      // the part (140 bytes), its inner at +20
constexpr uint32_t kItemsAt = 0x100;   // the list's item block (16 words)
constexpr uint32_t kOptionsAt = 0x180;  // player 0's options' +20
constexpr uint32_t kVtablesAt = 0x200;  // the mappers' vtables (0x40 each)
constexpr uint32_t kMappersAt = 0x400;  // 4 mappers of 0x200
constexpr uint32_t kManagerAt = 0xC00;  // the fake sound manager
constexpr uint32_t kManagerVtableAt = 0xC80;
constexpr uint32_t kBlockBytes = 0x1000;

constexpr uint32_t kBelow = 1024;  // the stack compared below the call's r1
constexpr uint32_t kAbove = 256;   // and above it

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kMissingMovie, 4},
    {kSaveData, 4},
    {rs::kMapperEntries, 32},
    {rs::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
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

// What the item block (the list's +68) holds: ours (functions that read
// it), or 0 or a block of the allocation layer's pages (functions that free
// it or make a new one).
enum class Items { kOurs, kPages };

// The stack ranges left out, as [sp - from, sp - to).
using Ranges = std::vector<std::pair<uint32_t, uint32_t>>;

struct Function {
  const char* name;
  uint32_t address;
  int cases;
  bool inner;  // r3 the inner part (else the part)
  Items items;
  Ranges skip;
  GuestFunction recompiled, native;
  // r4 (and r5) beyond the defaults: the movie id for the inner's Open,
  // Input and steps; flags with bit 0 clear for the deleting destructors.
  enum class R4 { kAny, kMovie, kNoDelete } r4 = R4::kAny;
};

// An action id: mostly the menus' (151 to 180), sometimes any word.
uint32_t AnyAction(Random& r) {
  return r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30);
}

void LayBlock(uint8_t* base, uint32_t block, Random& r, Items items, std::vector<uint32_t>& made) {
  lw::WorldRandom(base, block, kBlockBytes, r);
  const uint32_t part = block + kPartAt, inner = part + 20, list = inner + 40;
  guest::StoreU32(base, part, ts::kListPartVtable);
  guest::StoreU32(base, part + 12, 16 + r.Below(4));
  guest::StoreU32(base, part + 16, r.Below(5) == 0 ? 0 : inner);
  guest::StoreU32(base, inner, ts::kListPartInnerVtable);
  auto small = [&r](uint32_t n) { return r.Below(4) == 0 ? kNone : r.Below(n); };
  guest::StoreU32(base, inner + 4, r.Below(5) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, inner + 8, small(3));
  guest::StoreU32(base, inner + 12, small(3));
  guest::StoreU32(base, inner + 16, r.Below(4) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, inner + 20, r.Below(2) == 0 ? kNone : r.Below(3));
  guest::StoreU32(base, inner + 24, small(2));
  guest::StoreU32(base, inner + 28, r.Below(2));
  guest::StoreU32(base, inner + 112, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
  guest::StoreU32(base, inner + 116, r.Below(2));
  // The list (as the in-race screens' harness shapes one): two items
  // mostly.
  guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
  const uint32_t count = r.Below(4) == 0 ? r.Below(4) : 2;
  guest::StoreU32(base, list + rs::kListCount, count);
  guest::StoreU32(base, list + rs::kListShown, r.Below(4) == 0 ? r.Below(4) : 2);
  guest::StoreU32(base, list + rs::kListScroll, r.Below(4) == 0 ? kNone : r.Below(2));
  guest::StoreU32(base, list + rs::kListSelection, r.Below(6) == 0 ? r.Below(4) : r.Below(2));
  guest::StoreU32(base, list + rs::kListSelectionBefore,
                  r.Below(6) == 0 ? r.Below(4) : r.Below(2));
  for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
    guest::StoreU32(base, list + field, AnyAction(r));
  }
  guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
  guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  uint32_t items_block = block + kItemsAt;
  if (items == Items::kPages) {
    items_block = 0;
    if (r.Below(4) != 0) {
      items_block = memory::AllocPages(16, 4096);
      lw::WorldRandom(base, items_block, 16, r);
      made.push_back(items_block);
    }
  }
  guest::StoreU32(base, list + rs::kListBlock, items_block);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, block + kItemsAt + 4 * i, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
  // Player 0's options' +20.
  guest::StoreU32(base, block + kOptionsAt, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, kSaveData, block + kOptionsAt - kOptionsWord);
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
  const uint32_t manager = block + kManagerAt, vtable = block + kManagerVtableAt;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x820A8420);
  guest::StoreU32(base, vtable + 52, 0x820CE9F8);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
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
      LayBlock(base, block, r, f.items, made);
      const uint32_t part = block + kPartAt, inner = part + 20, list = inner + 40;
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      lw::WorldRandom(base, sp - kBelow, kBelow + kAbove, r);
      const uint64_t high = r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0;
      t.r3.u64 = high | (f.inner ? inner : part);
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      switch (f.r4) {
        case Function::R4::kMovie:
          t.r4.u64 = 16 + r.Below(4);
          break;
        case Function::R4::kNoDelete:
          t.r4.u64 &= ~1ull;
          break;
        case Function::R4::kAny:
          break;
      }
      t.lr = 0x82000000 + 4 * r.Below(1024);
      const uint32_t old_items = guest::LoadU32(base, list + rs::kListBlock);
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
      // The item blocks: a new one's words compared and freed (its word in
      // the list 1), the old one whether still the layer's.
      if (f.items == Items::kPages) {
        const uint32_t now = guest::LoadU32(base, list + rs::kListBlock);
        out[v].Word(now == old_items ? 1 : 0);
        out[v].Word(now == 0 ? 0 : 1);
        if (now != 0 && memory::IsPages(now)) {
          out[v].Bytes(Read(base, now, 4 * guest::LoadU32(base, list + rs::kListCount)));
          memory::FreePages(now);
          guest::StoreU32(base, list + rs::kListBlock, 1);
        }
        for (const uint32_t old : made) {
          if (old == now) continue;
          const bool still = memory::IsPages(old);
          out[v].Word(still ? 1 : 0);
          if (still) memory::FreePages(old);
        }
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
// (the part, 140 bytes, with its inner at +20, or an inner, 120) destroyed
// and deleted (its list's item block 0 or the layer's pages), compared by
// r3, whether the item block was freed and whether operator new hands the
// block out again.
void Deleting(Context& h) {
  uint8_t* const base = h.base;
  struct Case {
    const char* name;
    uint32_t address;
    uint32_t bytes;
    uint32_t inner_at;  // where the inner part (and its list) is
    GuestFunction recompiled, native;
  };
  const Case cases[] = {
      {"ListPartBase::DeletingDestructor, deleting", 0x82252610, 140, 20, __imp__sub_82252610,
       hooks::ListPartBaseDeletingDestruct},
      {"ListPartInnerBase::DeletingDestructor, deleting", 0x82252658, 120, 0,
       __imp__sub_82252658, hooks::ListPartInnerBaseDeletingDestruct},
      {"ListPartInner::DeletingDestructor, deleting", 0x822529B0, 120, 0, __imp__sub_822529B0,
       hooks::ListPartInnerDeletingDestruct},
      {"ListPart::DeletingDestructor, deleting", 0x82252A38, 140, 20, __imp__sub_82252A38,
       hooks::ListPartDeletingDestruct},
  };
  for (const Case& k : cases) {
    Results results(fmt::format("{} (sub_{:08X})", k.name, k.address));
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(Seed(uint64_t(k.address) << 20 ^ uint64_t(c)));
        const auto object = memory::crt::OperatorNew(base, k.bytes);
        if (!object) {
          ITC_LOG_ERROR("harness: type67_screens: operator new failed");
          return;
        }
        lw::WorldRandom(base, *object, k.bytes, r);
        const uint32_t inner = *object + k.inner_at;
        guest::StoreU32(base, *object, k.inner_at != 0 ? ts::kListPartVtable : ts::kListPartInnerVtable);
        if (k.inner_at != 0) guest::StoreU32(base, inner, ts::kListPartInnerVtable);
        const uint32_t items = r.Below(4) == 0 ? 0 : memory::AllocPages(16, 4096);
        guest::StoreU32(base, inner + 40 + rs::kListBlock, items);
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
        const bool still = items != 0 && memory::IsPages(items);
        out[v].Word(still ? 1 : 0);
        if (still) memory::FreePages(items);
        const auto probe = memory::crt::OperatorNew(base, k.bytes);
        out[v].Word(probe && *probe == *object ? 1 : 0);
        if (probe) memory::crt::OperatorDelete(*probe);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

}  // namespace

void Type67ListPart(Context& h) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR("harness: type67_screens: movie {} is in the table; the list part skipped",
                    id);
      return;
    }
  }
  const lw::MovieWorld w;
  const uint32_t block = memory::Alloc(kBlockBytes, 0);
  if (!w.ok() || block == 0) {
    ITC_LOG_ERROR("harness: type67_screens: no blocks for the list part");
    if (block != 0) memory::Free(block);
    return;
  }
  const lw::SavedGlobals saved(base);
  lw::BuildStringTable(base, w);

  using R4 = Function::R4;
  const Items ours = Items::kOurs, pages = Items::kPages;
  // The register saves: the function's own (its frame's top `n` bytes) and
  // those of the callees the native side runs as plain C++ (at their
  // frames' tops below it).
  auto saves = [](std::initializer_list<std::pair<uint32_t, uint32_t>> list) {
    return Ranges(list);
  };
  const Function functions[] = {
      {"ListPartBase::~ListPartBase", 0x82252420, 4, false, ours, {}, __imp__sub_82252420,
       hooks::ListPartBaseDestruct},
      {"ListPartInnerBase::~ListPartInnerBase", 0x82252600, 4, true, ours, {},
       __imp__sub_82252600, hooks::ListPartInnerBaseDestruct},
      {"ListPartBase::DeletingDestructor", 0x82252610, 4, false, ours, saves({{16, 0}}),
       __imp__sub_82252610, hooks::ListPartBaseDeletingDestruct, R4::kNoDelete},
      {"ListPartInnerBase::DeletingDestructor", 0x82252658, 4, true, ours, saves({{16, 0}}),
       __imp__sub_82252658, hooks::ListPartInnerBaseDeletingDestruct, R4::kNoDelete},
      {"ListPartInner::~ListPartInner", 0x822526F8, 8, true, pages, saves({{16, 0}}),
       __imp__sub_822526F8, hooks::ListPartInnerDestruct},
      {"ListPartInner::DeletingDestructor", 0x822529B0, 8, true, pages,
       saves({{24, 0}, {128, 112}}), __imp__sub_822529B0, hooks::ListPartInnerDeletingDestruct,
       R4::kNoDelete},
      {"ListPart::~ListPart", 0x82252968, 8, false, pages, saves({{16, 0}, {112, 96}}),
       __imp__sub_82252968, hooks::ListPartDestruct},
      {"ListPart::DeletingDestructor", 0x82252A38, 8, false, pages,
       saves({{24, 0}, {128, 112}, {224, 208}}), __imp__sub_82252A38,
       hooks::ListPartDeletingDestruct, R4::kNoDelete},
      {"ListPartInner::CanLeave", 0x822528C8, 8, true, ours, {}, __imp__sub_822528C8,
       hooks::ListPartInnerCanLeave},
      {"ListPartInner::Page1", 0x82252878, 4, true, ours, {}, __imp__sub_82252878,
       hooks::ListPartInnerPage1},
      {"ListPartInner::Page2", 0x822528A8, 4, true, ours, {}, __imp__sub_822528A8,
       hooks::ListPartInnerPage2},
      {"ListPartInner::IsIdle", 0x82252748, 8, true, ours, saves({{8, 0}}), __imp__sub_82252748,
       hooks::ListPartInnerIsIdle},
      {"ListPartInner::Close", 0x82252740, 8, true, pages, {}, __imp__sub_82252740,
       hooks::ListPartInnerClose},
      {"ListPartInner::Page0", 0x82252778, 24, true, ours, saves({{48, 0}}), __imp__sub_82252778,
       hooks::ListPartInnerPage0},
      {"ListPartInner::Step0", 0x82252BB8, 16, true, ours, saves({{40, 0}}), __imp__sub_82252BB8,
       hooks::ListPartInnerStep0, R4::kMovie},
      {"ListPartInner::Step1", 0x82252C98, 16, true, ours, saves({{32, 0}}), __imp__sub_82252C98,
       hooks::ListPartInnerStep1, R4::kMovie},
      {"ListPartInner::Step2", 0x82252D38, 12, true, ours, saves({{40, 0}}), __imp__sub_82252D38,
       hooks::ListPartInnerStep2, R4::kMovie},
      // Open: its frame's +48 to +79 (SetText's varargs) left out too.
      {"ListPartInner::Open", 0x82252A88, 12, true, pages, saves({{40, 0}, {144 - 48, 144 - 80}}),
       __imp__sub_82252A88, hooks::ListPartInnerOpen, R4::kMovie},
      {"ListPartInner::Pick", 0x82252A00, 16, true, ours, saves({{48, 0}}), __imp__sub_82252A00,
       hooks::ListPartInnerPick},
      {"ListPartInner::Input", 0x82252DF0, 24, true, ours, saves({{32, 0}, {168, 128}}),
       __imp__sub_82252DF0, hooks::ListPartInnerInput, R4::kMovie},
      {"ListPartBase::Open", 0x82252430, 12, false, pages,
       saves({{16, 0}, {136, 96}, {96 + 144 - 48, 96 + 144 - 80}}), __imp__sub_82252430,
       hooks::ListPartOpen},
      {"ListPartBase::Pick", 0x82252490, 16, false, ours, saves({{16, 0}, {144, 96}}),
       __imp__sub_82252490, hooks::ListPartPick},
      {"ListPartBase::Input", 0x82252500, 16, false, ours, saves({{32, 0}, {168, 128}}),
       __imp__sub_82252500, hooks::ListPartInput},
      {"ListPartBase::Close", 0x82252530, 8, false, pages, saves({{16, 0}}), __imp__sub_82252530,
       hooks::ListPartClose},
      {"ListPartBase::IsIdle", 0x82252590, 8, false, ours, saves({{8, 0}}), __imp__sub_82252590,
       hooks::ListPartIsIdle},
      {"ListPartBase::CanLeave", 0x822525B8, 8, false, ours, {}, __imp__sub_822525B8,
       hooks::ListPartCanLeave},
  };
  for (const Function& f : functions) {
    Run(h, w, block, f);
  }
  Deleting(h);
  memory::Free(block);
  saved.Restore();
}

}  // namespace harness
