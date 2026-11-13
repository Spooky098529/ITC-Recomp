// The list and node screens' node tables (src/type67_screens/node_tables.cpp,
// docs/functions/type67-screens.md) against the recompiled functions, run
// under the type67_screens harness (Type67Screens calls Type67ScreenTables).
// The originals run with --native_type67_screens off (their calls to each
// other through the adapters, so the originals), the plain C++ with the
// calls hooks::Type67ScreensCalls makes.
//
// The world: the loading screen's UI movie world (loading_world.h: three
// movies over one live scene of TXT leaves, named animations 0 to 255, text
// records for ids 1 to 7, the string table of letters), the leaves' +108
// (their text id, and the base of their children's index) made 1 to 7 so
// SetText finds a record; the table a block of random words with its node,
// child, named animation and pair words in the ranges the screens use, its
// movie id 16 to 19 (not in the movie table: UiMovies::Find answers the word
// at 0x826C5CDC, pointed at movie 0 for the case). FinishNamed, PlayNamed,
// LoopNamed and SetText run for real on both sides (their adapters). Compared
// with harness::Outputs, all exactly (no float math here): the table block,
// the movie world (movies, texts), the back chain and the id word (+80) of
// the function's own frame (made before its checks) (the frames below it hold the originals' register
// saves, and SetText's varargs r7 to r10 the native side passes 0 for, the
// user's call of 2026-10-30), and r3 (both halves).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "harnesses/loading_world.h"
#include "hooks/type67_screens.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "type67_screens/type67_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>

ITC_FLAG_DECLARE(bool, native_type67_screens);

namespace harness {

namespace {

namespace ts = type67_screens;
namespace lw = loading_world;

constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kTableBytes = 0x300, kInfo = 0x280;
constexpr uint32_t kNone = 0xFFFFFFFFu;

uint64_t Seed(uint64_t z) {
  z += 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return z ^ (z >> 31);
}

// The table's words (see the top); `paired` puts +452 to shifts (up to 40).
void LayTable(uint8_t* base, uint32_t t, Random& r, bool paired) {
  lw::WorldRandom(base, t, kTableBytes, r);
  auto item = [&r] { return r.Below(6) == 0 ? kNone : r.Below(27); };
  auto named = [&r] { return r.Below(6) == 0 ? kNone : r.Below(48); };
  auto node = [&r] { return r.Below(6) == 0 ? kNone : r.Below(60); };
  auto set = [&r] { return r.Below(3) == 0 ? kNone : r.Below(20); };
  auto shift = [&r] { return r.Below(4) == 0 ? 32 + r.Below(9) : r.Below(4); };
  auto color = [&r] { return r.Below(5) == 0 ? kNone : r.Below(3); };
  guest::StoreU32(base, t + 4, 16 + r.Below(4));
  guest::StoreU32(base, t + 8, set());
  guest::StoreU32(base, t + 12, set());
  for (uint32_t i = 0; i < 27; i++) guest::StoreU32(base, t + 16 + 4 * i, named());
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, t + 124 + 4 * i, named());
  for (uint32_t i = 0; i < 27; i++) guest::StoreU32(base, t + 156 + 4 * i, node());
  for (uint32_t i = 0; i < 8; i++) guest::StoreU32(base, t + 264 + 4 * i, node());
  for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, t + 296 + 4 * i, node());
  guest::StoreU32(base, t + 420, r.Below(3) == 0 ? kNone - r.Below(3) : r.Below(40));
  guest::StoreU32(base, t + 424, r.Below(3) == 0 ? kNone : r.Below(60));
  for (const uint32_t at : {428u, 432u, 436u, 440u}) guest::StoreU32(base, t + at, item());
  if (r.Below(4) == 0) guest::StoreU32(base, t + 432, guest::LoadU32(base, t + 428));
  if (r.Below(4) == 0) guest::StoreU32(base, t + 436, guest::LoadU32(base, t + 428));
  for (const uint32_t at : {444u, 448u}) {
    guest::StoreU32(base, t + at, r.Below(5) == 0 ? kNone : r.Below(8));
  }
  if (r.Below(4) == 0) guest::StoreU32(base, t + 448, guest::LoadU32(base, t + 444));
  guest::StoreU32(base, t + 452, paired ? shift() : color());
  guest::StoreU32(base, t + 456, color());
  guest::StoreU32(base, t + 460, shift());
  guest::StoreU32(base, t + 468, shift());
  guest::StoreU32(base, t + 476, r.Below(3) == 0 ? kNone : r.Below(5));
  for (uint32_t i = 0; i < 18; i++) guest::StoreU32(base, t + 480 + 4 * i, named());
}

// A case's registers beyond r3: r4 to r6.
using Registers = std::function<void(Random&, PPCContext&, uint32_t table)>;
using NativeCall = std::function<uint64_t(PPCContext&, uint8_t*, const ts::Calls&)>;

struct Function {
  const char* name;
  uint32_t address;
  int cases;
  uint32_t frame;  // the function's frame's bytes (0: none)
  bool paired;
  Registers registers;
  GuestFunction recompiled;
  NativeCall native;
};

void Run(Context& h, const lw::MovieWorld& w, uint32_t table, const Function& f) {
  uint8_t* const base = h.base;
  Results results(fmt::format("{} (sub_{:08X})", f.name, f.address));
  for (int c = 0; c < f.cases; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed((uint64_t(f.address) << 16) ^ uint64_t(c)));
      lw::ShapeMovieWorld(base, w, r);
      for (uint32_t k = 0; k < lw::kLeaves; k++) {
        guest::StoreU32(base, w.leaf(k) + 108, 1 + r.Below(7));
      }
      guest::StoreU32(base, kMissingMovie, w.movie(0));
      LayTable(base, table, r, f.paired);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      lw::WorldRandom(base, sp - 0x800, 0x800, r);
      t.r3.u64 = (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0) | table;
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      f.registers(r, t, table);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type67_screens) = false;
        f.recompiled(t, base);
        ITC_FLAG(native_type67_screens) = true;
      } else {
        const ts::Calls calls = hooks::Type67ScreensCalls(t, base);
        t.r3.u64 = f.native(t, base, calls);
      }
      out[v].Bytes(Read(base, table, kTableBytes));
      out[v].Bytes(Read(base, w.movies.address, w.movies.bytes));
      out[v].Bytes(Read(base, w.texts.address, w.texts.bytes));
      if (f.frame != 0) {
        out[v].Word(guest::LoadU32(base, sp - f.frame));
        // +80 is the id's word in the 128- to 160-byte frames (in the
        // 96-byte one, a register save).
        if (f.frame >= 128) out[v].Word(guest::LoadU32(base, sp - f.frame + 80));
      }
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// NodeTable::DeletingDestructor with bit 0 of r4 set: a block of operator
// new's destroyed and deleted, compared by r3 and whether operator new hands
// the block out again.
void Deleting(Context& h) {
  uint8_t* const base = h.base;
  Results results("NodeTable::DeletingDestructor, deleting (sub_82247E88)");
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(Seed(0x82247E88ull << 20 ^ uint64_t(c)));
      const auto block = memory::crt::OperatorNew(base, 580);
      if (!block) {
        ITC_LOG_ERROR("harness: type67_screens: operator new failed");
        return;
      }
      lw::WorldRandom(base, *block, 580, r);
      PPCContext t = CallContext(h);
      t.r3.u64 = *block;
      t.r4.u64 = (r.Next() & ~3ull) | (r.Below(2) == 0 ? 1 : 3);
      t.lr = 0x82000000 + 4 * r.Below(1024);
      if (v == 0) {
        ITC_FLAG(native_type67_screens) = false;
        __imp__sub_82247E88(t, base);
        ITC_FLAG(native_type67_screens) = true;
      } else {
        const ts::Calls calls = hooks::Type67ScreensCalls(t, base);
        t.r3.u64 = ts::NodeTableDeletingDestruct(base, t.r3.u64, t.r4.u64, t.r1.u32, calls);
      }
      out[v].Word(t.r3.u32 == *block ? 1 : 0);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const auto probe = memory::crt::OperatorNew(base, 580);
      out[v].Word(probe && *probe == *block ? 1 : 0);
      if (probe) memory::crt::OperatorDelete(*probe);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

// The table's words, for the titled list's cases (type67_titled_list.cpp).
void LayNodeTable(uint8_t* base, uint32_t table, Random& r, bool paired) {
  LayTable(base, table, r, paired);
}

void Type67ScreenTables(Context& h) {
  uint8_t* const base = h.base;
  for (const uint32_t id : {16u, 17u, 18u, 19u}) {
    if (ui_movies::PairsFindKey(base, ui_movies::kMovieTable, id) != kMissingMovie) {
      ITC_LOG_ERROR("harness: type67_screens: movie {} is in the table; the node tables skipped",
                    id);
      return;
    }
  }
  const lw::MovieWorld w;
  const uint32_t table = memory::Alloc(kTableBytes, 0);
  if (!w.ok() || table == 0) {
    ITC_LOG_ERROR("harness: type67_screens: no blocks for the node tables");
    if (table != 0) memory::Free(table);
    return;
  }
  const lw::SavedGlobals saved(base);
  const uint32_t saved_missing = guest::LoadU32(base, kMissingMovie);
  lw::BuildStringTable(base, w);

  const Registers none = [](Random&, PPCContext&, uint32_t) {};
  auto item = [](Random& r) -> uint64_t {
    return r.Below(8) == 0 ? (r.Below(2) == 0 ? 27 + r.Below(10) : r.Next()) : r.Below(27);
  };
  const Registers show = [item](Random& r, PPCContext& t, uint32_t) {
    t.r4.u64 = item(r);
    t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
    t.r6.u64 = r.Below(2) == 0 ? r.Below(16) : r.Next();
  };
  const Registers marks = [item](Random& r, PPCContext& t, uint32_t) {
    t.r4.u64 = item(r);
    t.r5.u64 = r.Below(2) == 0 ? r.Below(16) : r.Next();
  };
  const Registers colors = [item](Random& r, PPCContext& t, uint32_t) {
    t.r4.u64 = item(r);
    t.r5.u64 = r.Below(5);
  };
  const Registers which = [](Random& r, PPCContext& t, uint32_t) {
    static constexpr uint64_t kWhich[] = {0, 1, 0xFFFFFFFFFFFFFFFFull, 2, 7};
    t.r4.u64 = r.Below(6) == 0 ? r.Next() : kWhich[r.Below(5)];
  };
  const Registers finish = [](Random& r, PPCContext& t, uint32_t) {
    t.r4.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
  };
  const Registers no_delete = [](Random& r, PPCContext& t, uint32_t) {
    t.r4.u64 = r.Next() & ~1ull;
  };
  const Function functions[] = {
      {"NodeTable::~NodeTable", 0x82247DC0, 4, 0, false, none, __imp__sub_82247DC0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) { return ts::NodeTableDestruct(b, t.r3.u64); }},
      {"NodeTable::DeletingDestructor", 0x82247E88, 4, 96, false, no_delete, __imp__sub_82247E88,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::NodeTableDeletingDestruct(b, t.r3.u64, t.r4.u64, t.r1.u32, c);
       }},
      {"NodeTable::SetPairs", 0x82247CB8, 4, 0, false,
       [](Random&, PPCContext& t, uint32_t table) { t.r4.u64 = table + kInfo; },
       __imp__sub_82247CB8,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::NodeTableSetPairs(b, t.r3.u64, t.r4.u32);
       }},
      {"PairedNodeTable::SavePairs", 0x82247CE0, 4, 0, true, none, __imp__sub_82247CE0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::PairedNodeTableSavePairs(b, t.r3.u64);
       }},
      {"ColorNodeTable::Clear", 0x82247DD0, 4, 0, false, none, __imp__sub_82247DD0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) { return ts::ColorNodeTableClear(b, t.r3.u64); }},
      {"ColorNodeTable::Set", 0x82247E00, 8, 0, false,
       [base](Random& r, PPCContext& t, uint32_t table) {
         t.r4.u64 = table + kInfo;
         if (r.Below(2) == 0) guest::StoreU32(base, table + kInfo + 8, 1);
       },
       __imp__sub_82247E00,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::ColorNodeTableSet(b, t.r3.u64, t.r4.u32);
       }},
      {"ColorNodeTable::SavePairs", 0x82247E50, 4, 0, false, none, __imp__sub_82247E50,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::ColorNodeTableSavePairs(b, t.r3.u64);
       }},
      {"NodeTable::Show", 0x82248300, 16, 128, false, show, __imp__sub_82248300,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::NodeTableShow(b, t.r3.u64, t.r4.u32, t.r5.u32, t.r1.u32);
       }},
      {"NodeTable::ShowNode", 0x82247ED0, 16, 160, false, show, __imp__sub_82247ED0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::NodeTableShowNode(b, t.r3.u64, t.r4.u32, t.r5.u32, t.r6.u32, t.r1.u32);
       }},
      {"NodeTable::SetMarks", 0x822480F0, 12, 144, false, marks, __imp__sub_822480F0,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::NodeTableSetMarks(b, t.r3.u64, t.r4.u32, t.r5.u32, t.r1.u32);
       }},
      {"ColorNodeTable::SetColors", 0x82248198, 12, 144, false, colors, __imp__sub_82248198,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::ColorNodeTableSetColors(b, t.r3.u64, t.r4.u32, t.r5.u32, t.r1.u32);
       }},
      {"ColorNodeTable::ClearColors", 0x82248260, 8, 128, false, colors, __imp__sub_82248260,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::ColorNodeTableClearColors(b, t.r3.u64, t.r4.u32, t.r1.u32);
       }},
      {"ColorNodeTable::MarkSelected", 0x822487B8, 12, 144, false, which, __imp__sub_822487B8,
       [](PPCContext& t, uint8_t* b, const ts::Calls&) {
         return ts::ColorNodeTableMarkSelected(b, t.r3.u64, t.r4.u32, t.r1.u32);
       }},
      {"ColorNodeTable::Refresh", 0x822483B0, 24, 128, false, finish, __imp__sub_822483B0,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::ColorNodeTableRefresh(b, t.r3.u64, t.r4.u32, t.r1.u32, c);
       }},
      {"ColorNodeTable::Reset", 0x82248650, 12, 128, false, none, __imp__sub_82248650,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::ColorNodeTableReset(b, t.r3.u64, t.r1.u32, c);
       }},
      {"PairedNodeTable::Refresh", 0x82248800, 24, 128, true, none, __imp__sub_82248800,
       [](PPCContext& t, uint8_t* b, const ts::Calls& c) {
         return ts::PairedNodeTableRefresh(b, t.r3.u64, t.r1.u32, c);
       }},
  };
  for (const Function& f : functions) {
    Run(h, w, table, f);
  }
  Deleting(h);
  memory::Free(table);
  guest::StoreU32(base, kMissingMovie, saved_missing);
  saved.Restore();
}

}  // namespace harness
