// The build screen (src/build_screen/, docs/functions/build-screen.md)
// against the recompiled functions. Each case fills a world of its own (one
// block from the allocation layer) with random words from the case's seed:
// the screen data (its 56 groups, each with a build list whose 400 nodes'
// links and slots all stay in range, so any walk does; the shown groups,
// the screen's build list, the kept selections), the two menu lists, the
// transition machine, 64 build records the lists' slots point at, movie 6
// with a pool of 40 nodes (each child of a node another of the pool) and
// the movie table's pair naming it, and a garage stage; points the screen's
// globals into it (put back after); shapes the fields each function tests;
// runs the original (with --native_build_screen off, so what it calls of
// the screen's is the original too) or the register steps
// (hooks/build_screen.h); and compares the block, the function's frame (and
// the leaves' words below it) without the register saves, the caller's
// area above r1, r1 and r3, all exactly (nothing here is a float but the
// stage's light records, which are copied as words). MenuList::Init runs as
// its adapter on both sides; the flags blocks it makes are compared by
// their words and freed.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "build_screen/build_screen.h"
#include "hooks/build_screen.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_build_screen);

namespace harness {

namespace {

namespace bs = build_screen;

// The block's layout.
constexpr uint32_t kScreenAt = 0x0;          // the screen data (to 0x32668)
constexpr uint32_t kListAt = 0x32700;        // [0x826D8440]
constexpr uint32_t kGroupListAt = 0x32780;   // [0x826D8444]
constexpr uint32_t kMachineAt = 0x32800;     // [0x826D843C] (64 bytes)
constexpr uint32_t kBuildsAt = 0x32900;      // 64 build records of 52 bytes
constexpr uint32_t kBuildCount = 64;
constexpr uint32_t kPairsAt = 0x33600;       // the movie table's pairs
constexpr uint32_t kHolderAt = 0x33700;      // a garage stage's holder
constexpr uint32_t kStageAt = 0x33800;       // the stage (6,144 bytes)
constexpr uint32_t kMovieAt = 0x35000;       // movie 6 (0xA000 bytes)
constexpr uint32_t kNodesAt = 0x3F000;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimRootAt = 0x49000;    // the transitions' scene root (0x400)
constexpr uint32_t kAnimationsAt = 0x49400;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kBlockBytes = 0x49800;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;  // a text id not in the map: its record
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {bs::kMachine, 16},  // the machine, the lists and the screen
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {0x826A50AC, 64},  // the screen fades
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), strings(memory::Alloc(kStringsBytes, 0)) {
    if (strings != 0) {
      MakeStrings();
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(strings);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && strings != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  // A string table of our own (for the text setters, [0x826C5C34] pointed
  // at it): its 18 files all one container of 8,192 entries over 16
  // strings of letters, newlines and page breaks, so any id below 8,192
  // has a string; no argument codes (the varargs the native callers pass
  // as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;

 private:
  void MakeStrings() {
    Random r(0x4275696C64ull);  // "Build"
    const uint32_t container = strings + 128, text = strings + 128 + 4 + 4 * 8192;
    for (uint32_t k = 0; k < 18; k++) {
      guest::StoreU32(base, strings + 4 * k, container);
    }
    guest::StoreU32(base, container, __builtin_bswap32(8192));
    for (uint32_t k = 0; k < 8192; k++) {
      const uint32_t at = text + 64 * (k % 16);
      guest::StoreU32(base, container + 4 + 4 * k, __builtin_bswap32(at - container));
    }
    for (uint32_t k = 0; k < 16; k++) {
      const uint32_t at = text + 64 * k;
      const uint32_t length = r.Below(30);
      for (uint32_t c = 0; c < length; c++) {
        uint16_t unit = uint16_t('a' + r.Below(26));
        if (r.Below(10) == 0) {
          unit = r.Below(2) == 0 ? 10 : 0xFFD1;
        }
        guest::StoreU16(base, at + 2 * c, unit);
      }
      guest::StoreU16(base, at + 2 * length, 0);
    }
  }
};

// A string id below 8,192 (every one has a string in our table).
uint32_t StringId(Random& r) { return r.Below(8192); }

// A small count: mostly 0 to `hi`, now and then negative or any halfword.
uint16_t SmallCount(Random& r, uint32_t hi) {
  switch (r.Below(12)) {
    case 0: return uint16_t(0x8000 + r.Below(0x8000));
    case 1: return 0;
    default: return uint16_t(r.Below(hi + 1));
  }
}

// A build list at `list`: every node's link and slot in 0 to 399, the head
// and free head too, the count `count`, every slot a build record.
void MakeBuildList(uint8_t* base, const World& w, uint32_t list, Random& r, uint16_t count) {
  for (uint32_t i = 0; i < 400; i++) {
    guest::StoreU16(base, list + 4 * i, uint16_t(r.Below(400)));
    guest::StoreU16(base, list + 4 * i + 2, uint16_t(r.Below(400)));
    guest::StoreU32(base, list + bs::kListSlots + 4 * i,
                    w.at(kBuildsAt) + 52 * r.Below(kBuildCount));
  }
  guest::StoreU16(base, list + bs::kListFree, uint16_t(r.Below(400)));
  guest::StoreU16(base, list + bs::kListHead, uint16_t(r.Below(400)));
  guest::StoreU16(base, list + bs::kListCount, count);
}

// A menu list: its count, scroll, shown and selection small, its flags
// block 0 (MenuList::Init frees the old one).
void MakeMenuList(uint8_t* base, uint32_t list, Random& r) {
  guest::StoreU32(base, list + 0, r.Below(20));
  guest::StoreU32(base, list + 4, r.Below(12) - 2);
  guest::StoreU32(base, list + 12, r.Below(12));
  guest::StoreU32(base, list + 16, r.Below(14) - 2);
  guest::StoreU32(base, list + 64, r.Below(2));
  guest::StoreU32(base, list + 68, 0);
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  const uint32_t screen = w.at(kScreenAt);
  for (uint32_t g = 0; g < bs::kGroupCount; g++) {
    const uint32_t group = screen + bs::kGroups + bs::kGroupBytes * g;
    guest::StoreU32(base, group + 0, r.Below(40));
    guest::StoreU32(base, group + 12, r.Below(14) - 1);
    guest::StoreU32(base, group + 36, r.Below(5));
    guest::StoreU32(base, group + bs::kGroupCategory, r.Below(8) == 0 ? r.Below(8) : r.Below(5));
    MakeBuildList(base, w, group + bs::kGroupList_, r, SmallCount(r, 8));
  }
  MakeBuildList(base, w, screen + bs::kBuilds, r, SmallCount(r, 12));
  guest::StoreU32(base, screen + bs::kCategory, r.Below(5));
  const uint32_t shown = r.Below(4) == 0 ? r.Below(57) : r.Below(14);
  guest::StoreU32(base, screen + bs::kShownCount, shown);
  for (uint32_t i = 0; i < 56; i++) {
    guest::StoreU32(base, screen + bs::kShownGroups + 4 * i,
                    screen + bs::kGroups + bs::kGroupBytes * r.Below(bs::kGroupCount));
  }
  for (uint32_t k = bs::kGroupSelection; k <= bs::kStandScroll; k += 4) {
    guest::StoreU32(base, screen + k, r.Below(12) - 2);
  }
  for (uint32_t i = 0; i < kBuildCount; i++) {
    const uint32_t build = w.at(kBuildsAt) + 52 * i;
    // Its +0 a string's index (+4650 and the like in our table), +8 a kind
    // 0 to 3, +20 ValueText's value (now and then below 0 or past 9).
    guest::StoreU32(base, build + 0, r.Below(3000));
    guest::StoreU32(base, build + 8, r.Below(4));
    guest::StoreU32(base, build + 20, r.Below(14) - 1);
    if (r.Below(2) == 0) {
      guest::StoreU32(base, build + 36, guest::LoadU32(base, build + 32));
    }
  }
  MakeMenuList(base, w.at(kListAt), r);
  MakeMenuList(base, w.at(kGroupListAt), r);
  // Movie 6: its live scene node 0, each node's children the pool's.
  const uint32_t movie = w.at(kMovieAt);
  guest::StoreU32(base, movie + 16, w.at(kNodesAt));
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.at(kNodesAt) + kNodeBytes * n;
    guest::StoreU32(base, node + 0, kTxtTag);
    // +100 and +104 (the prompts' and digits' children) in the pool's range
    // too, as nodes other than the ones a case shapes are reached once a
    // function has stored into +108 (each node's first child's index).
    guest::StoreU32(base, node + 100, r.Below(60));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.at(kNodesAt) + kNodeBytes * r.Below(kNodeCount));
    }
  }
  // The texts' map empty, the text count small.
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, w.at(kPairsAt), 6);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  guest::StoreU32(base, w.at(kHolderAt), w.at(kStageAt));
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, bs::kMachine, w.at(kMachineAt));
  guest::StoreU32(base, bs::kList, w.at(kListAt));
  guest::StoreU32(base, bs::kGroupList, w.at(kGroupListAt));
  guest::StoreU32(base, bs::kScreen, w.at(kScreenAt));
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
}

// The text records GetText made (stored in the movie's map by the
// setters): their words and buffers compared, freed, their map words 0.
void TextRecords(uint8_t* base, const World& w, Outputs& out) {
  const uint32_t map = w.at(kMovieAt) + ui_movies::kMovieTexts;
  const uint32_t pairs = guest::LoadU32(base, map + 1024);
  for (uint32_t k = 0; k < pairs && k < 128; k++) {
    const uint32_t value = guest::LoadU32(base, map + 8 * k + 4);
    if (value != 0 && memory::IsHeapBlock(value)) {
      const uint32_t buffer = guest::LoadU32(base, value);
      out.Word(k);
      out.Bytes(Read(base, value + 4, 24));
      out.Word(memory::IsPages(buffer) ? 1 : 0);
      if (memory::IsPages(buffer)) {
        out.Bytes(Read(base, buffer, 4096));
        memory::FreePages(buffer);
      }
      memory::crt::OperatorDelete(value);
      guest::StoreU32(base, map + 8 * k + 4, 0);
    }
  }
}

// The menu lists' flags blocks MenuList::Init made: their words compared,
// freed, +68 0 again.
void ListBlocks(uint8_t* base, const World& w, Outputs& out) {
  for (const uint32_t list : {w.at(kListAt), w.at(kGroupListAt)}) {
    const uint32_t block = guest::LoadU32(base, list + 68);
    out.Word(block != 0 ? 1 : 0);
    if (block != 0 && memory::IsPages(block)) {
      const uint32_t count = guest::LoadU32(base, list + 0);
      out.Bytes(Read(base, block, 4 * (count < 64 ? count : 64)));
      memory::FreePages(block);
    }
    guest::StoreU32(base, list + 68, 0);
  }
}

// UiMovie::Run's update (sub_8211D2F8, its jump), stood in for while the
// transitions' cases run: each call's return address, stack pointer (from
// the case's), r3 and r4 kept and answered 0 or 1 from the case's
// generator (the update would draw the movie's scene).
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall> g_updates;
Random g_answers;
uint32_t g_case_sp = 0;

bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8) {
    return false;
  }
  g_updates.push_back({uint32_t(ctx.lr), g_case_sp - ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Below(2);
  return true;
}

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  std::function<void(PPCContext&, Random&, const World&)> setup;
  bool r3_is_block = false;  // r3 a flags block MenuList::Init made
};

void Run(Context& h, const World& w, const Spec& spec) {
  Results results(spec.name);
  for (int c = 0; c < spec.cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(h.base, g.address, g.bytes));
      }
      MakeWorld(h.base, w, r);
      PointGlobals(h.base, w, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      if (v == 0) {
        const bool native = ITC_FLAG(native_build_screen);
        ITC_FLAG(native_build_screen) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_build_screen) = native;
      } else {
        spec.native(t, h.base);
      }
      out[v].Word(uint32_t(g_updates.size()));
      for (const UpdateCall& call : g_updates) {
        out[v].Word(call.lr);
        out[v].Word(call.r1);
        out[v].Word(uint32_t(call.r3 >> 32));
        out[v].Word(uint32_t(call.r3));
        out[v].Word(uint32_t(call.r4 >> 32));
        out[v].Word(uint32_t(call.r4));
      }
      ListBlocks(h.base, w, out[v]);
      TextRecords(h.base, w, out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      if (spec.r3_is_block) {
        out[v].Word(t.r3.u32 != 0 ? 1 : 0);
      } else {
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void None(PPCContext&, Random&, const World&) {}

void Lists(Context& h, const World& w) {
  // Frames: the back chain and the words up to the register saves (96-byte
  // frames save r31 and lr, 112-byte ones r30 too, savegprlr r29 on).
  Run(h, w, {"BuildScreen::KeepGroupList (sub_8219BA50)", 100, __imp__sub_8219BA50,
             hooks::BuildScreenKeepGroupList, {{-96, -16}}, None});
  Run(h, w, {"BuildScreen::KeepBuildList (sub_8219BB20)", 100, __imp__sub_8219BB20,
             hooks::BuildScreenKeepBuildList, {{-96, -16}}, None});
  Run(h, w, {"BuildScreen::KeepStandList (sub_8219BBF0)", 100, __imp__sub_8219BBF0,
             hooks::BuildScreenKeepStandList, {{-96, -16}}, None});
  Run(h, w, {"BuildScreen::RestoreBuildList (sub_8219BB80)", 100, __imp__sub_8219BB80,
             hooks::BuildScreenRestoreBuildList, {{-112, -24}}, None});
  Run(h, w, {"BuildScreen::RestoreStandList (sub_8219BC50)", 100, __imp__sub_8219BC50,
             hooks::BuildScreenRestoreStandList, {{-112, -24}}, None});
  // InitGroupLists: its frame and RestoreGroupList's under it.
  Run(h, w, {"BuildScreen::InitGroupLists (sub_8219BF08; sub_8219BAB0)", 150,
             __imp__sub_8219BF08, hooks::BuildScreenInitGroupLists,
             {{-128 - 112, -128 - 24}, {-128, -32}}, None});
  Run(h, w, {"BuildScreen::InitStandList (sub_8219C078)", 150, __imp__sub_8219C078,
             hooks::BuildScreenInitStandList, {{-112, -8}},
             [](PPCContext&, Random& r, const World& w) {
               // A count MenuList::Init can make a block for.
               guest::StoreU16(w.base, w.at(kScreenAt) + bs::kBuildCount, uint16_t(r.Below(13)));
             },
             true});
  // ValueText: mostly 0 to 9, now and then below 0 or past the table (the
  // words above it, up to the caller's area).
  Run(h, w, {"BuildScreen::ValueText (sub_8219BCC0)", 150, __imp__sub_8219BCC0,
             hooks::BuildScreenValueText, {{-48, 0}},
             [](PPCContext& t, Random& r, const World&) {
               switch (r.Below(6)) {
                 case 0: t.r3.u64 = 0xFFFFFFFF00000000ull | (0x80000000u + r.Below(1000)); break;
                 case 1: t.r3.u64 = 10 + r.Below(30); break;
                 case 2: t.r3.u64 = (r.Next() << 32) | r.Below(10); break;
                 default: t.r3.u64 = r.Below(10);
               }
             }});
}

void BuildLists(Context& h, const World& w) {
  // Insert: a group's list or the screen's, a position within the count
  // (or a few past it), the pointer a build's address word.
  Run(h, w, {"BuildList::Insert (sub_8219C230; sub_8219C1C8)", 200, __imp__sub_8219C230,
             hooks::BuildListInsert, {{-112, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t screen = w.at(kScreenAt);
               t.r3.u64 = r.Below(4) == 0 ? screen + bs::kBuilds
                                          : screen + bs::kGroups + bs::kGroupList_ +
                                                bs::kGroupBytes * r.Below(bs::kGroupCount);
               t.r4.u64 = w.at(kBuildsAt) + 52 * r.Below(kBuildCount) + 4 * r.Below(13);
               t.r5.u64 = r.Below(16);
               if (r.Below(4) == 0) {
                 t.r5.u64 |= r.Next() << 32;
               }
             }});
  Run(h, w, {"BuildScreen::MarkGroups (sub_8219EC50)", 60, __imp__sub_8219EC50,
             hooks::BuildScreenMarkGroups, {}, None});
  Run(h, w, {"BuildScreen::FilterGroups (sub_8219CC70)", 150, __imp__sub_8219CC70,
             hooks::BuildScreenFilterGroups, {{-192, -96}}, None});
}

void Stage(Context& h, const World& w) {
  // SetDark: its frame (r30, r31 and lr saved) and SetMode's vectors below
  // it (its savegprlr saves above them left out); dark 0 or 1, now and then
  // another mode.
  Run(h, w, {"GarageStage::SetDark (sub_8217D0C8; sub_8217B7C8)", 100, __imp__sub_8217D0C8,
             hooks::GarageStageSetDark, {{-112 - 208, -112 - 96}, {-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.at(kHolderAt);
               t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(2);
             }});
}

void Rows(Context& h, const World& w) {
  auto arrows = [](uint32_t rows) {
    return [rows](PPCContext& t, Random& r, const World&) {
      t.r3.u64 = r.Below(3) == 0 ? 0 : r.Below(20);
      t.r4.u64 = r.Below(2) == 0 ? t.r3.u64 + rows : r.Below(30);
      if (r.Below(6) == 0) {
        t.r3.u64 |= r.Next() << 32;
      }
    };
  };
  Run(h, w, {"BuildScreen::GroupArrows (sub_8219D1B0)", 100, __imp__sub_8219D1B0,
             hooks::BuildScreenGroupArrows, {{-128, -40}}, arrows(11)});
  Run(h, w, {"BuildScreen::BuildArrows (sub_8219D4B8)", 100, __imp__sub_8219D4B8,
             hooks::BuildScreenBuildArrows, {{-128, -40}}, arrows(6)});
  Run(h, w, {"BuildScreen::StandArrows (sub_8219D7B0)", 100, __imp__sub_8219D7B0,
             hooks::BuildScreenStandArrows, {{-96, -8}},
             [](PPCContext&, Random& r, const World& w) {
               if (r.Below(2) == 0) {
                 guest::StoreU16(w.base, w.at(kScreenAt) + bs::kBuildCount, 1);
               }
             }});
  Run(h, w, {"BuildScreen::ShowBuildMark (sub_8219D440)", 100, __imp__sub_8219D440,
             hooks::BuildScreenShowBuildMark, {{-112, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t build = w.at(kBuildsAt) + 52 * r.Below(kBuildCount);
               if (r.Below(2) == 0) {
                 guest::StoreU32(w.base, build + 16, 0);
               }
               t.r3.u64 = build;
             }});
  // SetPrompts: each of r3 to r8 -1 now and then; the slots' +100 (the
  // icon's and label's children) in the pool's range.
  Run(h, w, {"BuildScreen::SetPrompts (sub_8219DA58)", 150, __imp__sub_8219DA58,
             hooks::BuildScreenSetPrompts, {{-176, -56}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t movie = w.at(kMovieAt);
               for (uint32_t k = 0; k < 3; k++) {
                 guest::StoreU32(w.base, ui_movies::FindNode(w.base, movie, 2, k) + 100,
                                 r.Below(60));
               }
               PPCRegister* regs[6] = {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7, &t.r8};
               for (PPCRegister* reg : regs) {
                 reg->u64 = r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(12);
                 if (r.Below(8) == 0) {
                   reg->u64 |= r.Next() << 32;
                 }
               }
             }});
  // SetHelp: a string id. SetNodeText's parameter area in its frame
  // (+40 to +79: the original's register leftovers, 0 natively, the user's
  // call, 2026-10-30) left out, and its register saves.
  Run(h, w, {"BuildScreen::SetHelp (sub_8219DB50)", 100, __imp__sub_8219DB50,
             hooks::BuildScreenSetHelp, {{-128, -88}, {-48, -32}},
             [](PPCContext& t, Random& r, const World&) { t.r3.u64 = StringId(r); }});
  // ShowNumber: 100 now and then, mostly 0 to 999, else any word; the
  // digits node's first child and count in the pool's range.
  Run(h, w, {"BuildScreen::ShowNumber (sub_8219D090; sub_8219BD30)", 200, __imp__sub_8219D090,
             hooks::BuildScreenShowNumber, {{-176, -88}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t digits = ui_movies::FindNode(w.base, w.at(kMovieAt), 12, 3);
               guest::StoreU32(w.base, digits + 100, r.Below(40));
               guest::StoreU32(w.base, digits + 104, r.Below(8) == 0 ? 0 : r.Below(8));
               switch (r.Below(6)) {
                 case 0: t.r3.u64 = 100; break;
                 case 1: t.r3.u64 = r.Next(); break;
                 case 2: t.r3.u64 = r.Below(10); break;
                 default: t.r3.u64 = r.Below(1000);
               }
             }});
  // DrawGroupRows: the rows from the list's scroll over the shown groups;
  // each row's frame below (SetNodeText's parameter area and the saves left
  // out).
  Run(h, w, {"BuildScreen::DrawGroupRows (sub_8219FA28; sub_8219CE08)", 100,
             __imp__sub_8219FA28, hooks::BuildScreenDrawGroupRows,
             {{-112, -24}, {-256, -216}, {-176, -168}}, None});
}

// Movie 6 for the transitions (as the car makers' harness lays it for the
// busy marker): its animation lists set up, its live scene a root of its
// own, its first 128 named animations over that root and one of 8
// animations of no tracks.
void AnimationWorld(uint8_t* base, const World& w, Random& r) {
  const uint32_t movie = w.at(kMovieAt), root = w.at(kAnimRootAt);
  ui_movies::InitAnimationLists(base, movie);
  guest::StoreU32(base, movie + 16, root);
  guest::StoreU32(base, movie + 20, root);
  guest::StoreU32(base, movie + 24, root);
  for (uint32_t k = 0; k < 128; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, root);
    guest::StoreU32(base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(8));
  }
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  guest::StoreU32(base, root + 100, 0);
  guest::StoreU32(base, root + 104, 4);
  guest::StoreU32(base, root + 108, r.Below(4));
  guest::StoreU32(base, root + 116, 70);
  for (uint32_t i = 0; i < 94; i++) {
    guest::StoreU32(base, root + 128 + 8 * i, w.at(kAnimationsAt) + 64 * r.Below(8));
  }
}

void Transitions(Context& h, const World& w) {
  // r3 the machine's +24: 0 (the set-up runs) or 1; the selection small.
  auto step = [](PPCContext& t, Random& r, const World& w) {
    AnimationWorld(w.base, w, r);
    guest::StoreU32(w.base, w.at(kMachineAt) + 24, r.Below(3) == 0 ? 1 : 0);
    guest::StoreU32(w.base, w.at(kListAt) + 16, r.Below(11));
    t.r3.u64 = w.at(kMachineAt) + 24;
  };
  struct T {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    std::vector<Range> stack;
  };
  const T transitions[] = {
      {"BuildScreen::HighlightGroups (sub_8219C528)", __imp__sub_8219C528,
       hooks::BuildScreenHighlightGroupsA, {{-128, -32}}},
      {"BuildScreen::HighlightGroups2 (sub_8219C6D0)", __imp__sub_8219C6D0,
       hooks::BuildScreenHighlightGroupsB, {{-128, -32}}},
      {"BuildScreen::HighlightBuilds (sub_8219C8F8)", __imp__sub_8219C8F8,
       hooks::BuildScreenHighlightBuilds, {{-128, -32}}},
      {"BuildScreen::HighlightStand (sub_8219CA08)", __imp__sub_8219CA08,
       hooks::BuildScreenHighlightStand, {{-112, -24}}},
      {"BuildScreen::HighlightStand2 (sub_8219CB18)", __imp__sub_8219CB18,
       hooks::BuildScreenHighlightStand2, {{-112, -24}}},
      {"BuildScreen::HighlightDetails (sub_8219CBD8)", __imp__sub_8219CBD8,
       hooks::BuildScreenHighlightDetails, {{-112, -24}}},
  };
  ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
  for (const T& t : transitions) {
    Run(h, w, {t.name, 60, t.recompiled, t.native, t.stack, step});
  }
  // FadeOut: the fades' counts set or not before.
  Run(h, w, {"BuildScreen::FadeOut (sub_8219C850)", 60, __imp__sub_8219C850,
             hooks::BuildScreenFadeOut, {{-112, -24}},
             [step](PPCContext& t, Random& r, const World& w) {
               step(t, r, w);
               guest::StoreU32(w.base, 0x826A50E4, r.Below(2));
               guest::StoreU32(w.base, 0x826A50DC, r.Below(2));
             }});
  ui_movies_hooks::g_update_callee_stand_in = nullptr;
}

// The groups' build counts not negative (CopyBuilds walks an unsigned
// count), the kept selection and scroll small (SetUpBuilds' shown group).
void Counts(PPCContext&, Random& r, const World& w) {
  for (uint32_t g = 0; g < bs::kGroupCount; g++) {
    const uint32_t count =
        w.at(kScreenAt) + bs::kGroups + bs::kGroupBytes * g + bs::kGroupBuildCount;
    if ((guest::LoadU16(w.base, count) & 0x8000) != 0) {
      guest::StoreU16(w.base, count, uint16_t(r.Below(9)));
    }
  }
  guest::StoreU32(w.base, w.at(kScreenAt) + bs::kGroupSelection, r.Below(6));
  guest::StoreU32(w.base, w.at(kScreenAt) + bs::kGroupScroll, r.Below(6));
}

void Builds(Context& h, const World& w) {
  Run(h, w, {"BuildScreen::InitBuildList (sub_8219BFE8)", 100, __imp__sub_8219BFE8,
             hooks::BuildScreenInitBuildList, {{-112, -8}, {-224, -136}},
             [](PPCContext&, Random& r, const World& w) {
               guest::StoreU16(w.base, w.at(kScreenAt) + bs::kBuildCount, uint16_t(r.Below(13)));
             }});
  auto group_index = [](PPCContext& t, Random& r, const World& w) {
    Counts(t, r, w);
    t.r3.u64 = r.Below(bs::kGroupCount);
  };
  Run(h, w, {"BuildScreen::CopyBuilds (sub_821A0128; BuildList::Insert)", 100,
             __imp__sub_821A0128, hooks::BuildScreenCopyBuilds, {{-144, -48}, {-256, -176}},
             group_index});
  Run(h, w, {"BuildScreen::ListBuilds (sub_821A0D38)", 100, __imp__sub_821A0D38,
             hooks::BuildScreenListBuilds, {{-192, -96}, {-336, -240}, {-448, -368}},
             group_index});
  // The rows: SetNodeText's parameter area (+40 to +79: the first call's
  // r6 is set, but the later calls store their register leftovers over it,
  // 0 natively) left out.
  Run(h, w, {"BuildScreen::DrawBuildRows (sub_8219D3E8; sub_8219D248)", 100, __imp__sub_8219D3E8,
             hooks::BuildScreenDrawBuildRows, {{-112, -24}, {-272, -232}, {-192, -184}}, None});
  Run(h, w, {"BuildScreen::DrawStandRow (sub_8219D550)", 150, __imp__sub_8219D550,
             hooks::BuildScreenDrawStandRow, {{-160, -120}, {-80, -72}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(2);
               t.r4.u64 = r.Below(16);
             }});
  // ShowGroup: SetText's parameter area (+48 to +79) left out.
  Run(h, w, {"BuildScreen::ShowGroup (sub_8219CF08)", 150, __imp__sub_8219CF08,
             hooks::BuildScreenShowGroup, {{-112, -64}, {-32, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.at(kScreenAt) + bs::kGroups + bs::kGroupBytes * r.Below(bs::kGroupCount);
             }});
  Run(h, w, {"BuildScreen::SetUpBuilds (sub_821A1008)", 100, __imp__sub_821A1008,
             hooks::BuildScreenSetUpBuilds, {{-128, -40}}, Counts});
}

}  // namespace

void BuildScreen(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: build_screen: no block from the allocation layer");
    return;
  }
  Lists(h, w);
  BuildLists(h, w);
  Stage(h, w);
  Rows(h, w);
  Transitions(h, w);
  Builds(h, w);
}

}  // namespace harness
