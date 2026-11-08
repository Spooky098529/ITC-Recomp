// The type 2 screens (src/type2_screens/, docs/functions/type2-screens.md)
// against the recompiled functions. Each case fills a world of its own (one
// block from the allocation layer) with random words from the case's seed:
// a screen machine, the mode with its message ring, the page object, the
// menu list, the screen's data, UI movie 26 with a pool of 40 nodes (each
// child of a node another of the pool) and the movie table's pair naming
// it, as the car dealer's harness lays them; points the globals into it
// (put back after); shapes the fields each function tests; runs the
// original (with --native_type2_screens off, so what it calls of the
// screens' is the original too) or the register steps
// (hooks/type2_screens.h); and compares the block, the function's frame
// without its register saves, the caller's area above r1, r1 and r3, all
// exactly (nothing here is a float). UiMovies::Open's and Delete's calls
// are recorded on both sides (the start-up recorder) and answered 1; the
// blocks VirtualAlloc makes (the layer's pages) are compared by content
// where their pointers were stored, then freed. GetLocalTime answers a
// made-up time (time_hooks::SetFakeNow) while a case runs. The text
// records the setters make are compared and freed, and the menu list's
// flags blocks MenuList::Init makes; the frames are compared without the
// words the varargs UiMovie::SetText and SetNodeText calls store from r7 or
// r6 on (the originals' leftovers, 0 in ours: the user's call for the
// parking area, 2026-10-30).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_specs/car_specs.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/type2_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "models/memory_blocks.h"
#include "models/xpr.h"
#include "race_screens/race_screens.h"
#include "type2_screens/type2_screens.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type2_screens);

namespace time_hooks {
void SetFakeNow(uint64_t now);
void ClearFakeNow();
}  // namespace time_hooks

namespace harness {

namespace {

namespace t2 = type2_screens;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;         // the screen machine (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;         // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2A00;        // the page object (28 bytes)
constexpr uint32_t kListAt = 0x2A40;         // the menu list (72)
constexpr uint32_t kDataAt = 0x2B00;         // the screen's data (512)
constexpr uint32_t kWordsAt = 0x2D00;        // a function's argument words (64)
constexpr uint32_t kPairsAt = 0x2D80;        // the movie table's pairs
constexpr uint32_t kMovieAt = 0x3000;        // the movie (0xA100 bytes)
constexpr uint32_t kNodesAt = 0xD400;        // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x17400;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the movie's named animations shaped
constexpr uint32_t kMenuWorldAt = 0x17800;   // the menus' mappers, vtables, sounds
constexpr uint32_t kStageAt = 0x18400;       // a garage stage (6,144 bytes)
constexpr uint32_t kHolderAt = 0x19C00;      // [0x826D83E0] points here: +0 the stage
constexpr uint32_t kSheetDataAt = 0x19C80;   // the spec sheet's data (1,968 bytes)
constexpr uint32_t kGroupListAt = 0x2A90;    // the build screen's group list (72)
constexpr uint32_t kHubAt = 0x1A480;         // the hub's world (0x800 bytes)
constexpr uint32_t kBlockBytes = 0x1AC80;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kRandSeed = 0x82638188;  // the game's rand
constexpr uint32_t kSaveSlots = 0x826C5B58;  // the players' save data
constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kStandHolder = 0x826D83E0;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {t2::kPlatePages, 12},  // the plate editor's page object, list and data
    {t2::kSheetPages, 12},  // the spec sheet's
    {t2::kBuildPages, 16},  // the build screen's
    {kSaveSlots, 16},
    {kStandHolder, 4},
    {t2::kHubPages, 16},  // the hub's page object, menu, data and counter
    {data_tables::kTables, 4},
    {car_specs::kParams, 4},
    {t2::kRecordScreen, 4},
    {0x826364EC, 4},  // the front-end menu to show on return (OnDialogDone clears it)
    {0x826A50AC, 64},  // the screen fades
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kRandSeed, 4},
    {kStringTable, 4},
    {race_screens::kMapperEntries, 32},
    {race_screens::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), strings(memory::Alloc(kStringsBytes, 0)),
        save(memory::Alloc(kSaveDataBytes, 0)), screen(memory::Alloc(t2::kBuildScreenBytes, 0)) {
    if (strings != 0) {
      MakeStrings();
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(strings);
    memory::Free(save);
    memory::Free(screen);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && strings != 0 && save != 0 && screen != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t data() const { return at(kDataAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  // A string table of our own ([0x826C5C34] pointed at it): its 18 files
  // all one container of 8,192 entries over 16 strings of letters, so any id
  // below 8,192 has a string; no argument codes (the varargs the native
  // callers pass as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;
  uint32_t save;    // player 0's save data (all four slots point at it)
  uint32_t screen;  // the build screen's data ([0x826D8448])

 private:
  void MakeStrings() {
    Random r(0x5479706532ull);  // "Type2"
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
        guest::StoreU16(base, at + 2 * c, uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, at + 2 * length, 0);
    }
  }
};

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  // The page object's words small or -1.
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, w.at(kPagesAt) + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(22));
  }
  // The menu list's selection (+16) small.
  guest::StoreU32(base, w.at(kListAt) + 16, r.Below(5));
  // The plate editor's data: the column (+52) small, the field (+56) 0 to 7.
  guest::StoreU32(base, w.data() + 52, r.Below(4));
  guest::StoreU32(base, w.data() + 56, r.Below(8));
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.at(kModeAt) + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.at(kModeAt) + 204 + 604, r.Below(60));
  // The movie: its live scene node 0, each node's children the pool's; its
  // loaded flag set or not.
  const uint32_t movie = w.at(kMovieAt);
  ui_movies::InitAnimationLists(base, movie);
  for (const uint32_t field : {16u, 20u, 24u}) {
    guest::StoreU32(base, movie + field, w.node(0));
  }
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    // +100 below 40, so a child index stays in the node's table (112
    // entries) after a function stores a character (below 40) into a
    // node's +108 (FindNode's first child's index).
    guest::StoreU32(base, node + 100, r.Below(40));
    guest::StoreU32(base, node + 104, r.Below(12));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  // The named animations (+176, 36 bytes each): over the root, each one of
  // the 8 animations of no tracks (a duration at +12).
  for (uint32_t k = 0; k < kNamed; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, w.node(0));
    guest::StoreU32(base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
  }
  for (uint32_t k = 0; k < kAnimations; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  // The root's own children none (+104 0), so UiMovie::ResetPose's walks
  // copy nothing; FindNode reads the root's table, not its count.
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, w.at(kPairsAt), t2::kPlateMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  guest::StoreU32(base, kRandSeed, uint32_t(r.Next()));
  // The machine's words: +9608, +9612, +9616 0 or 1 or any.
  for (const uint32_t field : {t2::kMachineLoaded, t2::kMachineReenter, t2::kMachineDelete}) {
    guest::StoreU32(base, w.at(kMachineAt) + field,
                    r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, t2::kPlatePages, w.at(kPagesAt));
  guest::StoreU32(base, t2::kPlateList, w.at(kListAt));
  guest::StoreU32(base, t2::kPlateData, w.data());
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
  guest::StoreU32(base, t2::kSheetPages, w.at(kPagesAt));
  guest::StoreU32(base, t2::kSheetMenu, w.at(kListAt));
  guest::StoreU32(base, t2::kSheetData, w.at(kSheetDataAt));
  guest::StoreU32(base, t2::kBuildPages, w.at(kPagesAt));
  guest::StoreU32(base, t2::kBuildList, w.at(kListAt));
  guest::StoreU32(base, t2::kBuildGroupList, w.at(kGroupListAt));
  guest::StoreU32(base, t2::kBuildScreen, w.screen);
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, w.save);
  }
  guest::StoreU32(base, kStandHolder, w.at(kHolderAt));
  guest::StoreU32(base, w.at(kHolderAt), w.at(kStageAt));
}

// The text records the setters made (stored in the movie's map): their
// words and buffers compared, freed, their map words 0.
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

// The pointer word at `slot` names a block the side allocated (the layer's
// pages): its first `bytes` compared, freed, the word replaced by 1 (0 when
// none, the word itself when not pages).
void Allocated(uint8_t* base, Outputs& out, uint32_t slot, uint32_t bytes) {
  const uint32_t block = guest::LoadU32(base, slot);
  if (block == 0 || !memory::IsPages(block)) {
    out.Word(block);
    return;
  }
  out.Word(1);
  out.Bytes(Read(base, block, bytes));
  memory::FreePages(block);
  guest::StoreU32(base, slot, 1);
}

// UiMovies::Open's and Delete's calls, recorded on both sides and answered
// 1 (Delete would free our movie); other addresses to the recorder set
// before the run, when there's one.
constexpr uint32_t kMoviesOpen = t2::kMoviesOpen, kMoviesDelete = t2::kMoviesDelete;
std::vector<uint32_t> g_opens;
hooks::StartUpRecorder g_chained = nullptr;
// Archive::Load's calls (the hub's notice images), recorded and answered a
// serial: their buffers and records compared and freed after the run.
struct LoadCall {
  uint32_t id, buffer, callback, record, r7, r8, r1, lr;
};
std::vector<LoadCall> g_loads;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address == t2::kArchiveLoad) {
    g_loads.push_back({ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32,
                       ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 0x100 + g_loads.size();
    return true;
  }
  if (address != kMoviesOpen && address != kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
                                 uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

// The layer's pages a case made (blocks the function frees): after the
// run, whether each is still pages (then freed).
std::vector<uint32_t> g_made;
uint32_t MakePages(uint8_t* base, uint32_t bytes, Random& r) {
  const uint32_t block = memory::AllocPages(bytes, 4096);
  WriteRandom(base, block, std::min(bytes, 64u), r);
  g_made.push_back(block);
  return block;
}

// UiMovie::Run's update (sub_8211D2F8), stood in for: each call's return
// address, stack pointer (from the case's), r3 and r4 kept and answered 0
// or 1 from the case's generator (the update would draw the movie).
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

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read.
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
  AfterFn after = nullptr;
  bool big = false;  // the save data and the build screen's data compared too
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
      g_made.clear();
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
      g_opens.clear();
      g_loads.clear();
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordOpen;
      if (v == 0) {
        const bool native = ITC_FLAG(native_type2_screens);
        ITC_FLAG(native_type2_screens) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_type2_screens) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      time_hooks::ClearFakeNow();
      out[v].Word(uint32_t(g_updates.size()));
      for (const UpdateCall& call : g_updates) {
        out[v].Word(call.lr);
        out[v].Word(call.r1);
        out[v].Word(uint32_t(call.r3 >> 32));
        out[v].Word(uint32_t(call.r3));
        out[v].Word(uint32_t(call.r4 >> 32));
        out[v].Word(uint32_t(call.r4));
      }
      out[v].Word(uint32_t(g_opens.size()));
      for (const uint32_t word : g_opens) {
        out[v].Word(word);
      }
      // The loads: their words, the buffers (zeroed) and the records
      // compared and freed, the records' list slots in the data marked.
      out[v].Word(uint32_t(g_loads.size()));
      for (size_t k = 0; k < g_loads.size(); k++) {
        const LoadCall& load = g_loads[k];
        for (const uint32_t word : {load.id, load.callback, load.r7, load.r8, load.r1, load.lr}) {
          out[v].Word(word);
        }
        for (const uint32_t block : {load.buffer, load.record}) {
          const bool pages = memory::IsPages(block);
          out[v].Word(pages ? 1 : 0);
          if (pages) {
            out[v].Bytes(Read(h.base, block, block == load.record ? 16 : 64));
            memory::FreePages(block);
          }
        }
        for (uint32_t i = 0; i < 2 * 46; i++) {
          const uint32_t slot = w.data() + (i < 46 ? 32 + 4 * i : 220 + 4 * (i - 46));
          if (guest::LoadU32(h.base, slot) == load.record) {
            guest::StoreU32(h.base, slot, 0xF00D0000u + uint32_t(k));
          }
        }
      }
      g_loads.clear();
      if (spec.after) {
        spec.after(t, w, out[v]);
      }
      out[v].Word(uint32_t(g_made.size()));
      for (const uint32_t block : g_made) {
        const bool pages = memory::IsPages(block);
        out[v].Word(pages ? 1 : 0);
        if (pages) {
          memory::FreePages(block);
        }
      }
      g_made.clear();
      TextRecords(h.base, w, out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      if (spec.big) {
        out[v].Bytes(Read(h.base, w.save, kSaveDataBytes));
        out[v].Bytes(Read(h.base, w.screen, t2::kBuildScreenBytes));
      }
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void None(PPCContext&, Random&, const World&) {}
void Machine(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kMachineAt); }
void MachineAndMode(PPCContext& t, Random&, const World& w) {
  t.r3.u64 = w.at(kMachineAt);
  t.r4.u64 = w.at(kModeAt);
}
void Mode(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kModeAt); }

// Sixteen argument words at kWordsAt: 0 to `hi`, now and then `special`
// (or any, with `any`: not for words the function stores into a node's
// +108, which FindNode reads as its first child's index).
void Words(PPCContext& t, Random& r, const World& w, uint32_t hi, uint32_t special,
           bool any = false) {
  for (uint32_t i = 0; i < 16; i++) {
    const uint32_t pick = r.Below(6);
    guest::StoreU32(w.base, w.at(kWordsAt) + 4 * i,
                    pick == 0   ? special
                    : pick == 1 ? (any ? uint32_t(r.Next()) : special)
                                : r.Below(hi + 1));
  }
  t.r3.u64 = w.at(kWordsAt);
}

void PlateEditor(Context& h, const World& w) {
  Run(h, w, {"Screen::LeaveToHub (sub_821D6A20)", 60, __imp__sub_821D6A20,
             hooks::ScreenLeaveToHub, {}, MachineAndMode});
  if (hooks::NativeHeapActive()) {
    // Create: the three blocks VirtualAlloc made compared by content.
    Run(h, w, {"PlateEditor::Create (sub_82198210)", 60, __imp__sub_82198210,
               hooks::PlateCreate, {{-112, -24}}, Machine,
               [&h](PPCContext& t, const World&, Outputs& out) {
                 // r3 the data block's address (the last VirtualAlloc's).
                 out.Word(t.r3.u32 == guest::LoadU32(h.base, t2::kPlateData) ? 1 : 0);
                 t.r3.u64 = 0;
                 Allocated(h.base, out, t2::kPlatePages, 28);
                 Allocated(h.base, out, t2::kPlateList, 72);
                 Allocated(h.base, out, t2::kPlateData, 76);
               }});
    // Destroy: the three blocks the layer's pages, their globals cleared.
    Run(h, w, {"PlateEditor::Destroy (sub_82198118)", 60, __imp__sub_82198118,
               hooks::PlateDestroy, {{-112, -32}},
               [](PPCContext&, Random& r, const World& w) {
                 guest::StoreU32(w.base, t2::kPlatePages, MakePages(w.base, 28, r));
                 guest::StoreU32(w.base, t2::kPlateList, MakePages(w.base, 72, r));
                 guest::StoreU32(w.base, t2::kPlateData, MakePages(w.base, 76, r));
               }});
  }
  Run(h, w, {"PlateEditor::OpenMovie (sub_82198180)", 20, __imp__sub_82198180,
             hooks::PlateOpenMovie, {}, None});
  Run(h, w, {"PlateEditor::WaitForLoad (sub_821982F0)", 100, __imp__sub_821982F0,
             hooks::PlateWaitForLoad, {{-96, -16}}, Machine});
  Run(h, w, {"PlateEditor::Close (sub_82198498)", 60, __imp__sub_82198498, hooks::PlateClose,
             {{-112, -16}}, MachineAndMode});
  Run(h, w, {"PlateEditor::OnMessage32 (sub_82198190)", 60, __imp__sub_82198190,
             hooks::PlateOnMessage32, {},
             [](PPCContext&, Random& r, const World& w) {
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), 4);
               }
             }});
  Run(h, w, {"PlateEditor::LeavePage (sub_82198988)", 60, __imp__sub_82198988,
             hooks::PlateLeavePage, {}, Mode});
  Run(h, w, {"PlateEditor::CharLineKind (sub_821981C8)", 200, __imp__sub_821981C8,
             hooks::PlateCharLineKind, {},
             [](PPCContext& t, Random& r, const World& w) {
               Words(t, r, w, 12, 10, true);
               // The field 0 to 7, 3 often (CharLineKind's 2).
               guest::StoreU32(w.base, w.data() + 56, r.Below(3) == 0 ? 3 : r.Below(8));
               t.r3.u64 = w.at(kWordsAt) + 4;
             }});
  // RandomPlate: the plate at the data's +24 (byte +0 and +12 on left as
  // they are); its frame but the saves (r29 on) compared.
  Run(h, w, {"PlateEditor::RandomPlate (sub_82198360)", 60, __imp__sub_82198360,
             hooks::PlateRandom, {{-128, -32}},
             [](PPCContext& t, Random&, const World& w) { t.r4.u64 = w.data() + 24; }});
  // LoadPlate: with the data's +48 set or not; RandomPlate's frame under
  // its own.
  auto load = [](PPCContext&, Random& r, const World& w) {
    guest::StoreU32(w.base, w.data() + 48, r.Below(2));
  };
  Run(h, w, {"PlateEditor::LoadPlate (sub_82198420; sub_82198360)", 60, __imp__sub_82198420,
             hooks::PlateLoad, {{-96, -16}, {-224, -128}}, load});
  Run(h, w, {"PlateEditor::ResetMovie (sub_821989E8)", 60, __imp__sub_821989E8,
             hooks::PlateResetMovie, {{-96, -8}}, None});
  // Start: LoadPlate's and ResetMovie's frames at the same place under it
  // (ResetMovie's id at +80 over the upper half of LoadPlate's r31 save, its
  // lower half left out), RandomPlate's under them.
  Run(h, w, {"PlateEditor::Start (sub_82199BB8; sub_82198420, sub_821989E8)", 60,
             __imp__sub_82199BB8, hooks::PlateStart, {{-96, -8}, {-192, -108}, {-320, -224}},
             [load](PPCContext& t, Random& r, const World& w) {
               load(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), 0);
               }
             }});
  Run(h, w, {"PlateEditor::ShowRegion (sub_82199898)", 60, __imp__sub_82199898,
             hooks::PlateShowRegion, {{-112, -16}},
             [](PPCContext& t, Random& r, const World&) { t.r3.u64 = r.Below(14); }});
  Run(h, w, {"PlateEditor::ShowDigits (sub_821998F0)", 100, __imp__sub_821998F0,
             hooks::PlateShowDigits, {{-144, -48}},
             [](PPCContext& t, Random& r, const World& w) { Words(t, r, w, 10, 10); }});
  Run(h, w, {"PlateEditor::ShowChars (sub_82199990)", 100, __imp__sub_82199990,
             hooks::PlateShowChars, {{-144, -48}},
             [](PPCContext& t, Random& r, const World& w) { Words(t, r, w, 38, 37); }});
  Run(h, w, {"PlateEditor::MarkField (sub_82199AA8)", 100, __imp__sub_82199AA8,
             hooks::PlateMarkField, {{-128, -40}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(4) == 0 ? 0xFFFFFFFFFFFFFFFFull : r.Below(12);
             }});
  Run(h, w, {"PlateEditor::FinishRegionAnim (sub_821992D8)", 60, __imp__sub_821992D8,
             hooks::PlateFinishRegionAnim, {{-112, -16}}, None});
  Run(h, w, {"PlateEditor::FinishDigitAnim (sub_82199410)", 60, __imp__sub_82199410,
             hooks::PlateFinishDigitAnim, {{-112, -16}}, None});
}

// The cells' and menus' world: the plate (the data's +4: the region 0 to
// 13, the digits 0 to 10, the characters 0 to 37, now and then past them),
// the field (+56) 0 to 6, the rows (+64, +68) 1 to 16, the list's scroll
// (+4) -1 to 6, its flags block (+68) none or the layer's pages
// (MenuList::Init frees it and makes another, compared after).
void CellsWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t data = w.data();
  guest::StoreU8(base, data + 4 + 1, uint8_t(r.Below(16)));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU8(base, data + 4 + 2 + i, uint8_t(r.Below(12)));
  }
  for (uint32_t i = 0; i < 7; i++) {
    guest::StoreU8(base, data + 4 + 5 + i, uint8_t(r.Below(40)));
  }
  guest::StoreU32(base, data + 56, r.Below(7));
  guest::StoreU32(base, data + 64, 1 + r.Below(16));
  guest::StoreU32(base, data + 68, 1 + r.Below(16));
  guest::StoreU32(base, w.at(kListAt) + 4, uint32_t(int32_t(r.Below(8)) - 1));
  guest::StoreU32(base, w.at(kListAt) + 68, r.Below(2) == 0 ? 0 : MakePages(base, 64, r));
}
// Node (2, 2) given 3 children, as in the game: ShowDigits reads as many
// words as it has children, past the three digits its callers write into
// their frames (the saves: a register's value stored into a node's +108,
// where FindNode reads its first child's index).
void ThreeDigits(const World& w) {
  const uint32_t node = ui_movies::FindNode(w.base, w.at(kMovieAt), 2, 2);
  guest::StoreU32(w.base, node + 104, 3);
}
// Every node at most 3 children (the transitions' callees may move which
// node is (2, 2) before ShowDigits runs), the root none.
void SmallNodes(Random& r, const World& w) {
  for (uint32_t n = 1; n < kNodeCount; n++) {
    guest::StoreU32(w.base, w.node(n) + 104, r.Below(4));
  }
}
void ListBlock(PPCContext&, const World& w, Outputs& out) {
  const uint32_t list = w.at(kListAt);
  Allocated(w.base, out, list + 68, 4 * std::min(guest::LoadU32(w.base, list + 0), 128u));
}

void PlateCells(Context& h, const World& w) {
  auto cells = [](PPCContext&, Random& r, const World& w) { CellsWorld(r, w); };
  // The cells: SetNodeText's parameter area (+40 to +79) left out.
  Run(h, w, {"PlateEditor::ShowRegionCells (sub_821990D0)", 100, __imp__sub_821990D0,
             hooks::PlateShowRegionCells, {{-176, -136}, {-96, -80}}, cells});
  Run(h, w, {"PlateEditor::ShowCharCells (sub_82199488)", 100, __imp__sub_82199488,
             hooks::PlateShowCharCells, {{-176, -136}, {-96, -88}}, cells});
  // ShowDigitCells: SetText's parameter area (+48 to +79) left out.
  Run(h, w, {"PlateEditor::ShowDigitCells (sub_82199350)", 100, __imp__sub_82199350,
             hooks::PlateShowDigitCells, {{-128, -80}, {-48, -40}}, cells});
  Run(h, w, {"PlateEditor::ShowCharLine (sub_82199688)", 100, __imp__sub_82199688,
             hooks::PlateShowCharLine, {{-128, -32}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(5) == 0 ? r.Next() : r.Below(4);
             }});
  // ShowPlate: ShowChars' frame (and ShowRegion's back chain) under its own.
  // ShowDigits reads as many words as node (2, 2) has children (3 in the
  // game): the frame's +92, between the digits and the characters, unwritten
  // (the stack's word), small here, as a large one stored into a node's
  // +108 sends FindNode out of the pool.
  Run(h, w, {"PlateEditor::ShowPlate (sub_82199A20; sub_82199898)", 100, __imp__sub_82199A20,
             hooks::PlateShowPlate, {{-144, -8}, {-288, -192}},
             [](PPCContext& t, Random& r, const World& w) {
               CellsWorld(r, w);
               guest::StoreU32(w.base, t.r1.u32 - 144 + 92, r.Below(12));
               if (r.Below(2) == 0) {
                 ThreeDigits(w);
               }
               t.r3.u64 = w.data() + 4;
             }});
  // The menus' set-ups: their frames (MenuList::Init's stack words) without
  // the saves.
  Run(h, w, {"PlateEditor::SetUpRegionMenu (sub_82199C10; sub_821990D0, sub_82199AA8)", 100,
             __imp__sub_82199C10, hooks::PlateSetUpRegionMenu, {{-128, -24}}, cells, ListBlock});
  Run(h, w, {"PlateEditor::SetUpNumberMenu (sub_82199CC8; sub_82199350)", 100,
             __imp__sub_82199CC8, hooks::PlateSetUpNumberMenu, {{-128, -24}},
             [](PPCContext&, Random& r, const World& w) {
               CellsWorld(r, w);
               guest::StoreU32(w.base, w.data() + 56, r.Below(3));
             },
             ListBlock});
  Run(h, w, {"PlateEditor::SetUpCharsMenu (sub_82199D88; sub_82199488)", 100,
             __imp__sub_82199D88, hooks::PlateSetUpCharsMenu, {{-128, -24}}, cells, ListBlock});
}

// The transitions: r3 the started word's address (the page object's +24:
// 0, 1 or any), the cells' world; UiMovie::Run's update stood in (answered
// 0 or 1); their frames without the saves.
// The menus' world, as the car dealer's harness lays it: the four
// controllers' action mappers (their read slots ActionMapper::Pressed and
// Fired, now and then another function; their bits random), the list's
// player, counts, scroll and selection around each other, its actions
// (151 to 180 or any), its wrap and move words; the sound manager (its
// PlayCue2D slot sub_82090770).
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}
void MenuWorld(Random& r, const World& w) {
  namespace rs = race_screens;
  uint8_t* base = w.base;
  const uint32_t at = w.at(kMenuWorldAt);
  const uint32_t mappers = at, vtables = at + 0x800;
  const uint32_t manager = at + 0xB00, manager_vtable = at + 0xB80;
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t mapper = mappers + 0x200 * c, vtable = vtables + 0x40 * c;
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, r.Below(5) == 0 ? 0x8210BE70 : rs::kPressedSlot);
    guest::StoreU32(base, vtable + 48, r.Below(5) == 0 ? 0x8210BE70 : rs::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  const uint32_t list = w.at(kListAt);
  guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
  const int32_t count = int32_t(r.Below(13));
  guest::StoreU32(base, list + rs::kListCount, uint32_t(count));
  guest::StoreU32(base, list + rs::kListShown, Around(r, 0, count + 1));
  guest::StoreU32(base, list + rs::kListScroll, Around(r, -1, count));
  guest::StoreU32(base, list + rs::kListScrollBefore, Around(r, -1, count));
  guest::StoreU32(base, list + rs::kListSelection, Around(r, -2, count + 1));
  for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
    guest::StoreU32(base, list + field, r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30));
  }
  guest::StoreU32(base, list + rs::kListWraps, r.Below(3) == 0 ? 0 : r.Below(4));
  guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, list + rs::kListSide, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, list + rs::kListBlock, at + 0x900);
  for (uint32_t k = 0; k < 64; k++) {
    guest::StoreU32(base, at + 0x900 + 4 * k, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, 0x82090770);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// The pages: the menus' world, the field (+56) 0 to 6 (2 and 6 often: the
// last digit and character), the column (+52) small; r3 the mode. Their
// frames without the saves.
void PlatePages(Context& h, const World& w) {
  auto page = [](PPCContext& t, Random& r, const World& w) {
    MenuWorld(r, w);
    const uint32_t pick = r.Below(4);
    guest::StoreU32(w.base, w.data() + 56, pick == 0 ? 2 : pick == 1 ? 6 : r.Below(7));
    t.r3.u64 = w.at(kModeAt);
  };
  Run(h, w, {"PlateEditor::RegionPage (sub_82199ED0)", 300, __imp__sub_82199ED0,
             hooks::PlateRegionPage, {{-112, -24}}, page});
  Run(h, w, {"PlateEditor::NumberPage (sub_8219A138)", 300, __imp__sub_8219A138,
             hooks::PlateNumberPage, {{-112, -24}}, page});
  Run(h, w, {"PlateEditor::CharsPage (sub_82198540)", 300, __imp__sub_82198540,
             hooks::PlateCharsPage, {{-112, -24}}, page});
  // UpdatePage: the page -1 or 0 to 4 (3, ConfirmPage, never seen in play,
  // the original on both sides), the data's +72 21 or another state.
  Run(h, w, {"PlateEditor::UpdatePage (sub_8219B050)", 300, __imp__sub_8219B050,
             hooks::PlateUpdatePage, {{-128, -16}},
             [page](PPCContext& t, Random& r, const World& w) {
               page(t, r, w);
               guest::StoreU32(w.base, w.at(kPagesAt), uint32_t(int32_t(r.Below(6)) - 1));
               guest::StoreU32(w.base, w.data() + 72, r.Below(2) == 0 ? 21 : r.Below(12));
               t.r3.u64 = w.at(kMachineAt);
               t.r4.u64 = w.at(kModeAt);
             }});
}

// InitStates: the machine empty or holding random transitions with keys of
// small numbers (states below 12, messages below 52), so some match the
// Init's (Add replaces those); its frame (but its saves, r27 on) compared.
void InitStates(Context& h, const World& w) {
  auto machine = [](PPCContext& t, Random& r, const World& w) {
    namespace gm = game_modes;
    const uint32_t machine = w.at(kMachineAt);
    if (r.Below(2) == 0) {
      gm::StateMachineConstruct(w.base, machine);
      guest::StoreU32(w.base, machine + gm::kTransitionCount, 0);
    } else {
      const uint32_t count = 8 + r.Below(160);
      guest::StoreU32(w.base, machine + gm::kTransitionCount, count);
      for (uint32_t i = 0; i < gm::kTransitions; i++) {
        const uint32_t at = machine + gm::kTransitionBytes * i;
        guest::StoreU32(w.base, at, r.Below(12));
        guest::StoreU32(w.base, at + 4, r.Below(52));
        guest::StoreU32(w.base, at + 8, r.Below(8) == 0 ? 1 : 0);
        guest::StoreU32(w.base, at + 12, 0);
      }
    }
    t.r3.u64 = machine;
  };
  Run(h, w, {"Type2Hub::InitStates (sub_82190B18)", 24, __imp__sub_82190B18,
             hooks::HubInitStates, {{-1552, -48}}, machine});
  Run(h, w, {"PowerGraphScreen2::InitStates (sub_821B90D8)", 24, __imp__sub_821B90D8,
             hooks::SheetInitStates, {{-912, -48}}, machine});
  Run(h, w, {"BuildScreen::InitStates (sub_821A16C0)", 24, __imp__sub_821A16C0,
             hooks::BuildInitStates, {{-912, -48}}, machine});
  Run(h, w, {"PlateEditor::InitStates (sub_8219B2E8)", 24, __imp__sub_8219B2E8,
             hooks::PlateInitStates, {{-912, -48}}, machine});
}

void PlateTransitions(Context& h, const World& w) {
  auto step = [](PPCContext& t, Random& r, const World& w) {
    CellsWorld(r, w);
    SmallNodes(r, w);
    const uint32_t at = w.at(kPagesAt) + 24;
    guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
    t.r3.u64 = at;
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    int32_t frame, saves;
  } kCases[] = {
      {"PlateEditor::Transition0 (sub_8219A3A8; sub_82199C10, sub_82199A20)", __imp__sub_8219A3A8,
       hooks::PlateTransition0, 112, 24},
      {"PlateEditor::Transition1 (sub_82198A40)", __imp__sub_82198A40, hooks::PlateTransition1,
       128, 32},
      {"PlateEditor::Transition4 (sub_82198C20)", __imp__sub_82198C20, hooks::PlateTransition4,
       112, 24},
      {"PlateEditor::Transition5 (sub_8219A5E8; sub_82199CC8, sub_821998F0)", __imp__sub_8219A5E8,
       hooks::PlateTransition5, 144, 32},
      {"PlateEditor::Transition6 (sub_82198D00)", __imp__sub_82198D00, hooks::PlateTransition6,
       128, 32},
      {"PlateEditor::Transition8 (sub_8219A820; sub_82199CC8, sub_821998F0)", __imp__sub_8219A820,
       hooks::PlateTransition8, 144, 32},
      {"PlateEditor::Transition10 (sub_8219AA00; sub_82199D88, sub_82199688)",
       __imp__sub_8219AA00, hooks::PlateTransition10, 128, 32},
      {"PlateEditor::Transition11 (sub_82198DC8)", __imp__sub_82198DC8, hooks::PlateTransition11,
       128, 32},
      {"PlateEditor::Transition13 (sub_8219AC88; sub_821981C8, sub_82199990)",
       __imp__sub_8219AC88, hooks::PlateTransition13, 160, 32},
      {"PlateEditor::Transition19 (sub_82198FB8)", __imp__sub_82198FB8, hooks::PlateTransition19,
       160, 72},
  };
  for (const auto& c : kCases) {
    Run(h, w, {c.name, 80, c.recompiled, c.native, {{-c.frame, -c.saves}}, step, ListBlock});
  }
  // Update: the running transition -1, -2 (none: done) or one of those seen
  // in play (the others' worlds aren't laid), its frame without the saves
  // (r18 on).
  Run(h, w, {"PlateEditor::Update (sub_8219B108)", 200, __imp__sub_8219B108, hooks::PlateUpdate,
             {{-304, -120}},
             [step](PPCContext& t, Random& r, const World& w) {
               static constexpr uint32_t kSeen[10] = {0, 1, 4, 5, 6, 8, 10, 11, 13, 19};
               step(t, r, w);
               MenuWorld(r, w);
               guest::StoreU32(w.base, w.at(kListAt) + 68, 0);
               const uint32_t pick = r.Below(6);
               guest::StoreU32(w.base, w.at(kPagesAt) + 12,
                               pick == 0   ? 0xFFFFFFFFu
                               : pick == 1 ? 0xFFFFFFFEu
                                           : kSeen[r.Below(10)]);
               guest::StoreU32(w.base, w.at(kPagesAt) + 16,
                               r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(20));
             },
             ListBlock});
}

// The spec sheet's and the build screen's machines' handlers.
void SheetAndBuild(Context& h, const World& w) {
  if (hooks::NativeHeapActive()) {
    Run(h, w, {"PowerGraphScreen::Create (sub_821B8120)", 60, __imp__sub_821B8120,
               hooks::SheetCreate, {{-112, -24}}, Machine,
               [&h](PPCContext& t, const World&, Outputs& out) {
                 out.Word(t.r3.u32 == guest::LoadU32(h.base, t2::kSheetData) ? 1 : 0);
                 t.r3.u64 = 0;
                 Allocated(h.base, out, t2::kSheetPages, 28);
                 Allocated(h.base, out, t2::kSheetMenu, 68);
                 Allocated(h.base, out, t2::kSheetData, 1968);
               }});
    Run(h, w, {"PowerGraphScreen::Destroy (sub_821B80B8)", 60, __imp__sub_821B80B8,
               hooks::SheetDestroy, {{-112, -32}},
               [](PPCContext&, Random& r, const World& w) {
                 guest::StoreU32(w.base, t2::kSheetPages, MakePages(w.base, 28, r));
                 guest::StoreU32(w.base, t2::kSheetMenu, MakePages(w.base, 68, r));
                 guest::StoreU32(w.base, t2::kSheetData, MakePages(w.base, 1968, r));
               }});
    // BuildScreen::Create: the screen data's first words and its +0x32668
    // compared (the rest is the layer's zeroed pages).
    Run(h, w, {"BuildScreen::Create (sub_8219BD80)", 60, __imp__sub_8219BD80,
               hooks::BuildCreate, {{-112, -24}}, Machine,
               [&h](PPCContext& t, const World&, Outputs& out) {
                 out.Word(t.r3.u32 == guest::LoadU32(h.base, t2::kBuildScreen) ? 1 : 0);
                 t.r3.u64 = 0;
                 const uint32_t screen = guest::LoadU32(h.base, t2::kBuildScreen);
                 if (screen != 0 && memory::IsPages(screen)) {
                   out.Word(guest::LoadU32(h.base, screen + 0x32668));
                 }
                 Allocated(h.base, out, t2::kBuildPages, 28);
                 Allocated(h.base, out, t2::kBuildList, 72);
                 Allocated(h.base, out, t2::kBuildGroupList, 72);
                 Allocated(h.base, out, t2::kBuildScreen, 64);
               }});
    // BuildScreen::Destroy: the four blocks the layer's pages (the list's
    // +68 block one too, or none: MenuList::Release frees it).
    Run(h, w, {"BuildScreen::Destroy (sub_8219B8C8)", 60, __imp__sub_8219B8C8,
               hooks::BuildDestroy, {{-112, -32}},
               [](PPCContext&, Random& r, const World& w) {
                 uint8_t* base = w.base;
                 const uint32_t list = MakePages(base, 72, r);
                 guest::StoreU32(base, list + 68, r.Below(2) == 0 ? 0 : MakePages(base, 64, r));
                 guest::StoreU32(base, t2::kBuildPages, MakePages(base, 28, r));
                 guest::StoreU32(base, t2::kBuildList, list);
                 guest::StoreU32(base, t2::kBuildGroupList, MakePages(base, 72, r));
                 guest::StoreU32(base, t2::kBuildScreen, MakePages(base, t2::kBuildScreenBytes, r));
               }});
  }
  Run(h, w, {"PowerGraphScreen::OpenMovie (sub_82250328)", 20, __imp__sub_82250328,
             hooks::SheetOpenMovie, {}, None});
  Run(h, w, {"PowerGraphScreen::WaitForLoad (sub_821B8900)", 100, __imp__sub_821B8900,
             hooks::SheetWaitForLoad, {{-96, -16}}, Machine});
  Run(h, w, {"PowerGraphScreen::Close (sub_821B8320)", 60, __imp__sub_821B8320,
             hooks::SheetClose, {{-96, -16}}, MachineAndMode});
  Run(h, w, {"BuildScreen::OpenMovie (sub_8219B950)", 20, __imp__sub_8219B950,
             hooks::BuildOpenMovie, {}, None});
  Run(h, w, {"BuildScreen::WaitForLoad (sub_8219BE98)", 100, __imp__sub_8219BE98,
             hooks::BuildWaitForLoad, {{-96, -16}}, Machine});
  Run(h, w, {"BuildScreen::Unload (sub_8219C298)", 60, __imp__sub_8219C298, hooks::BuildUnload,
             {{-112, -16}}, Machine});
  Run(h, w, {"BuildScreen::Close (sub_8219C300)", 60, __imp__sub_8219C300, hooks::BuildClose,
             {{-112, -16}}, MachineAndMode});
  // SaveBuildStates and LeavePage: the screen data and the save data random,
  // compared (the stage in the block).
  auto screen = [](PPCContext& t, Random& r, const World& w) {
    WriteRandom(w.base, w.screen, t2::kBuildScreenBytes, r);
    WriteRandom(w.base, w.save, kSaveDataBytes, r);
    t.r3.u64 = w.at(kModeAt);
  };
  Run(h, w, {"BuildScreen::SaveBuildStates (sub_8219C100)", 20, __imp__sub_8219C100,
             hooks::BuildSaveBuildStates, {{-96, -8}}, screen, nullptr, true});
  Run(h, w, {"BuildScreen::LeavePage (sub_8219C3A8; sub_8219C100)", 20, __imp__sub_8219C3A8,
             hooks::BuildLeavePage, {{-96, -16}}, screen, nullptr, true});
  Run(h, w, {"BuildScreen::DetailsPage (sub_8219B960)", 300, __imp__sub_8219B960,
             hooks::BuildDetailsPage, {{-96, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               MenuWorld(r, w);
               if (r.Below(2) == 0) {
                 guest::StoreU32(w.base, w.at(kListAt) + 8, guest::LoadU32(w.base, w.at(kListAt) + 4));
               }
               t.r3.u64 = w.at(kModeAt);
             }});
  // UpdatePage: the level -1, 3 (DetailsPage) or 4 (LeavePage); the others'
  // worlds (the groups and builds) are the build screen harness's.
  Run(h, w, {"BuildScreen::UpdatePage (sub_8219FA80)", 200, __imp__sub_8219FA80,
             hooks::BuildUpdatePage, {{-112, -8}},
             [screen](PPCContext& t, Random& r, const World& w) {
               MenuWorld(r, w);
               screen(t, r, w);
               const uint32_t pick = r.Below(3);
               guest::StoreU32(w.base, w.at(kPagesAt), pick == 0 ? 0xFFFFFFFFu : 2 + pick);
               t.r3.u64 = w.at(kMachineAt);
               t.r4.u64 = w.at(kModeAt);
             },
             nullptr, true});
}

// The hub's world: the movie table's pairs 0 (the movie), 9 and 10 (two
// stubs whose loaded flags are in the hub's area), the movie's object (+28)
// there, the menu's cursor, the data's image lists (counts 0 to 4, records
// of the area, loaded or not), its notice (+432 a table 0 to 3, +436 -1 or
// 0 to 7) over a 56-byte table the data tables' global points at, the load
// counter, the stage's +0, and player 0's save data (random): its notice lists (counts 0 to 8,
// entries read or not), name, money and day.
void HubWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t hub = w.at(kHubAt), data = w.data(), save = w.save;
  const uint32_t pairs = w.at(kPairsAt);
  guest::StoreU32(base, t2::kHubPages, w.at(kPagesAt));
  guest::StoreU32(base, t2::kHubMenu, w.at(kListAt));
  guest::StoreU32(base, t2::kHubData, data);
  WriteRandom(base, save, kSaveDataBytes, r);
  const uint32_t stubs[2] = {hub + 0x3C0 - ui_movies::kMovieLoaded,
                             hub + 0x3C4 - ui_movies::kMovieLoaded};
  guest::StoreU32(base, pairs + 0, t2::kHubMovie);
  guest::StoreU32(base, pairs + 4, w.at(kMovieAt));
  guest::StoreU32(base, pairs + 8, 9);
  guest::StoreU32(base, pairs + 12, stubs[0]);
  guest::StoreU32(base, pairs + 16, 10);
  guest::StoreU32(base, pairs + 20, stubs[1]);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 3);
  for (const uint32_t stub : stubs) {
    guest::StoreU32(base, stub + ui_movies::kMovieLoaded, r.Below(4) == 0 ? 0 : 1);
  }
  guest::StoreU32(base, w.at(kMovieAt) + ui_movies::kMovieLoaded, r.Below(4) == 0 ? 0 : 1);
  guest::StoreU32(base, w.at(kMovieAt) + 28, hub);
  // The stage's +0 (GarageStage::SetPlates writes its +6092) at a word of
  // the area.
  guest::StoreU32(base, w.at(kStageAt), hub + 0x3C8 - 6092);
  // Its +16 set (GarageStage::MakeModel then makes nothing).
  guest::StoreU32(base, hub + 0x3C8 - 6092 + 16, 1 + r.Below(4));
  guest::StoreU32(base, w.at(kListAt) + 0, r.Below(6) == 0 ? 0xFFFFFFFFu : r.Below(10));
  for (const uint32_t list : {32u, 220u}) {
    guest::StoreU32(base, data + list + 184, r.Below(5));
    for (uint32_t i = 0; i < 46; i++) {
      guest::StoreU32(base, data + list + 4 * i, hub + 0x40 + 16 * r.Below(16));
    }
  }
  for (uint32_t k = 0; k < 16; k++) {
    guest::StoreU32(base, hub + 0x40 + 16 * k, r.Below(5) == 0 ? 0 : 1);
    guest::StoreU32(base, hub + 0x40 + 16 * k + 4, r.Below(12));
  }
  guest::StoreU32(base, data + 432, r.Below(4));
  guest::StoreU32(base, data + 436, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(8));
  guest::StoreU32(base, data_tables::kTables, hub + 0x140);
  for (uint32_t k = 5; k < 9; k++) {
    guest::StoreU32(base, hub + 0x140 + 4 * k, hub + 0x180);
  }
  // The table's entries: chains that end inside it (+8 the step to the
  // next, 0 at the end), +16 an image index below 64 or negative.
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t entry = hub + 0x180 + 56 * k;
    guest::StoreU32(base, entry + 8, k < 7 && r.Below(2) == 0 ? 1 + r.Below(7 - k) : 0);
    guest::StoreU32(base, entry + 16, r.Below(5) == 0 ? uint32_t(-int32_t(1 + r.Below(9))) : r.Below(64));
  }
  // The image bits (+408, +412) sparse.
  guest::StoreU32(base, data + 408, uint32_t(r.Next() & r.Next() & r.Next()));
  guest::StoreU32(base, data + 412, uint32_t(r.Next() & r.Next() & r.Next()) & 0x3FFF);
  guest::StoreU32(base, t2::kHubCounter, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(100));
  for (const uint32_t list : {0x1C778u, 0x1CA84u}) {
    for (uint32_t i = 0; i < 12; i++) {
      guest::StoreU32(base, save + list + 12 * i, r.Below(2));
      guest::StoreU32(base, save + list + 12 * i + 4, r.Below(4));
      guest::StoreU32(base, save + list + 12 * i + 8, r.Below(8));
    }
  }
  guest::StoreU32(base, save + 0x1CA78, r.Below(9));
  guest::StoreU32(base, save + 0x1D504, r.Below(9));
  // The name record (UiMovie::SetNameText's kinds and indexes).
  guest::StoreU32(base, save + 8 + 4, r.Below(6));
  guest::StoreU16(base, save + 8 + 8, uint16_t(r.Below(110)));
  guest::StoreU16(base, save + 8 + 10, uint16_t(r.Below(2)));
  guest::StoreU32(base, save + 8 + 12, r.Below(6));
  guest::StoreU16(base, save + 8 + 16, uint16_t(r.Below(110)));
  guest::StoreU16(base, save + 8 + 18, uint16_t(r.Below(2)));
  // The garage (+6120): its slot links (halfwords at +4h, below 60, and
  // +4h + 2, the slots below 50), the head at +204, the steps
  // (+0x19920) below 4, then the count (+6328).
  for (uint32_t k = 0; k < 64; k++) {
    guest::StoreU16(base, save + 6120 + 4 * k, uint16_t(r.Below(60)));
    guest::StoreU16(base, save + 6120 + 4 * k + 2, uint16_t(r.Below(50)));
  }
  guest::StoreU32(base, save + 0x19920, r.Below(4));
  guest::StoreU16(base, save + 6328, uint16_t(r.Below(4) == 0 ? 0 : 1 + r.Below(59)));
  // The garage cars' ids (+224, below 20) over the car specs ([0x826C2E48]
  // the build screen's data block: Params::GetCarSpec's +771 byte read).
  for (uint32_t k = 0; k < 50; k++) {
    guest::StoreU32(base, save + 6120 + 1968 * k + 224, r.Below(20));
  }
  guest::StoreU32(base, car_specs::kParams, w.screen);
  for (uint32_t k = 0; k < 20; k++) {
    guest::StoreU8(base, w.screen + 72 + 8432 * k + 771, uint8_t(r.Below(3)));
  }
  const uint32_t pick = r.Below(4);
  guest::StoreU32(base, save + 6100,
                  pick == 0 ? uint32_t(r.Next()) : pick == 1 ? 990 + r.Below(20) : r.Below(999));
  guest::StoreU32(base, save + 6116, uint32_t(r.Next()));
}

// The hub's movie world: the hub's, the nodes small (at most 3 children,
// their animations' base +116 below 8: UiMovie::Play's index), the fades'
// counts 0 or not.
void HubMovieWorld(Random& r, const World& w) {
  HubWorld(r, w);
  SmallNodes(r, w);
  for (uint32_t n = 0; n < kNodeCount; n++) {
    guest::StoreU32(w.base, w.node(n) + 116, r.Below(8));
  }
  for (const uint32_t count : {0x826A50E4u, 0x826A50DCu}) {
    guest::StoreU32(w.base, count, r.Below(2) == 0 ? 0 : r.Below(30));
  }
}

void HubMovie(Context& h, const World& w) {
  auto movie = [](PPCContext&, Random& r, const World& w) { HubMovieWorld(r, w); };
  // UpdatePage: the page -1, 1 or 2 (the empty function), 3 (Choose) or 5
  // (RecordPage); the others' worlds (MenuPage's, NoticePage's) aren't laid.
  Run(h, w, {"Type2Hub::UpdatePage (sub_82190498)", 200, __imp__sub_82190498,
             hooks::HubUpdatePage, {{-128, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               HubMovieWorld(r, w);
               static constexpr uint32_t kPages[5] = {0xFFFFFFFFu, 1, 2, 3, 5};
               guest::StoreU32(w.base, w.at(kPagesAt), kPages[r.Below(5)]);
               t.r3.u64 = w.at(kMachineAt);
               t.r4.u64 = w.at(kModeAt);
             }});
  // The varargs UiMovie::SetText calls' parameter area (+48 to +79 of the
  // caller's frame: r7 to r10, the originals' leftovers) left out.
  Run(h, w, {"Type2Hub::SetRecordText (sub_8218F4D8)", 60, __imp__sub_8218F4D8,
             hooks::HubSetRecordText, {{-96, -48}, {-16, -8}}, movie});
  Run(h, w, {"Type2Hub::RecordPage (sub_821900B8; sub_8218F4D8)", 60, __imp__sub_821900B8,
             hooks::HubRecordPage, {{-96, -16}, {-192, -144}, {-112, -104}},
             [](PPCContext& t, Random& r, const World& w) {
               HubMovieWorld(r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), 0xFFFFFFFFu);
               }
               t.r3.u64 = w.at(kModeAt);
             }});
  Run(h, w, {"Type2Hub::ShowBusy (sub_8218EA90)", 40, __imp__sub_8218EA90, hooks::HubShowBusy,
             {{-112, -16}}, movie});
  Run(h, w, {"Type2Hub::HideBusy (sub_8218EB10)", 40, __imp__sub_8218EB10, hooks::HubHideBusy,
             {{-112, -16}}, movie});
  Run(h, w, {"Type2Hub::ShowNoticeImage (sub_8218F3F8)", 100, __imp__sub_8218F3F8,
             hooks::HubShowNoticeImage, {{-112, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               HubMovieWorld(r, w);
               t.r3.u64 = r.Below(14);
             }});
  // ShowNotice: ShowNoticeImage's frame under its own.
  Run(h, w, {"Type2Hub::ShowNotice (sub_821903D0; sub_8218F3F8)", 100, __imp__sub_821903D0,
             hooks::HubShowNotice, {{-128, -80}, {-48, -40}, {-240, -144}}, movie});
  // FillMenu: SetPrompts' frame under its own (but its saves).
  Run(h, w, {"Type2Hub::FillMenu (sub_8218FB20; sub_8218F530)", 100, __imp__sub_8218FB20,
             hooks::HubFillMenu, {{-176, -128}, {-96, -40}, {-368, -240}}, movie});
  // The transitions: r3 the page object's +24 (0, 1 or any); their own
  // frames without the saves (Transition2's SetText parameter area left
  // out); the callees' frames are their own cases'.
  auto step = [](PPCContext& t, Random& r, const World& w) {
    HubMovieWorld(r, w);
    const uint32_t at = w.at(kPagesAt) + 24;
    guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
    // The menu's cursor (0 to 7), its old one (+4) and way (+8: 1, -1 or
    // another).
    const uint32_t menu = w.at(kListAt);
    guest::StoreU32(w.base, menu + 0, r.Below(8));
    guest::StoreU32(w.base, menu + 4, r.Below(8));
    const uint32_t way = r.Below(3);
    guest::StoreU32(w.base, menu + 8, way == 0 ? 1 : way == 1 ? 0xFFFFFFFFu : r.Below(4));
    guest::StoreU32(w.base, w.data() + 12, r.Below(2));
    t.r3.u64 = at;
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    std::vector<Range> stack;
  } kCases[] = {
      {"Type2Hub::Transition0 (sub_821901C0; sub_8218FB20)", __imp__sub_821901C0,
       hooks::HubTransition0, {{-112, -24}}},
      {"Type2Hub::Transition1 (sub_8218EC78)", __imp__sub_8218EC78, hooks::HubTransition1,
       {{-128, -32}}},
      {"Type2Hub::Transition2 (sub_8218EE48)", __imp__sub_8218EE48, hooks::HubTransition2,
       {{-144, -96}, {-64, -48}}},
      {"Type2Hub::Transition3 (sub_8218ED98)", __imp__sub_8218ED98, hooks::HubTransition3,
       {{-112, -24}}},
      {"Type2Hub::Transition5 (sub_82190548; sub_8218F530, sub_821903D0)", __imp__sub_82190548,
       hooks::HubTransition5, {{-112, -24}}},
      {"Type2Hub::Transition6 (sub_8218F098)", __imp__sub_8218F098, hooks::HubTransition6,
       {{-112, -24}}},
      {"Type2Hub::Transition7 (sub_821902C8; sub_8218FB20)", __imp__sub_821902C8,
       hooks::HubTransition7, {{-112, -24}}},
  };
  for (const auto& c : kCases) {
    Run(h, w, {c.name, 100, c.recompiled, c.native, c.stack, step});
  }
}

// The hub's menu world: the movie world, the menus' (MenuWorld's mappers
// and sound manager) with the hub's front-end menu's fields (at the list's
// place): the cursor 0 to 8 or -1, moved (+8) 0 mostly, first 0, last 7,
// its steps and side small, the player 0 to 3; the dialog's step (+24) and
// answer (+28) 0 to 3; the player's record's screen ([0x826D84CC]) in the
// hub's area.
void HubMenuWorld(Random& r, const World& w) {
  namespace mh = menu_helpers;
  HubMovieWorld(r, w);
  MenuWorld(r, w);
  uint8_t* base = w.base;
  const uint32_t menu = w.at(kListAt);
  guest::StoreU32(base, menu + mh::kCursor, r.Below(10) == 0 ? 0xFFFFFFFFu : r.Below(9));
  guest::StoreU32(base, menu + mh::kPrevious, r.Below(8));
  guest::StoreU32(base, menu + mh::kMoved, r.Below(4) == 0 ? (r.Below(2) == 0 ? 1 : 0xFFFFFFFFu) : 0);
  guest::StoreU32(base, menu + mh::kFirst, 0);
  guest::StoreU32(base, menu + mh::kLast, 7);
  guest::StoreU32(base, menu + mh::kWraps, r.Below(2));
  guest::StoreU32(base, menu + mh::kUpDownLocked, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + mh::kSidesStep, r.Below(3));
  guest::StoreU32(base, menu + mh::kShouldersStep, r.Below(3));
  guest::StoreU32(base, menu + mh::kSide, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + mh::kWrapped, r.Below(2));
  guest::StoreU32(base, menu + mh::kPlayer, r.Below(4));
  guest::StoreU32(base, w.data() + 24, r.Below(5));
  guest::StoreU32(base, w.data() + 28, r.Below(5));
  guest::StoreU32(base, w.data() + 20, r.Below(3));
  guest::StoreU32(base, t2::kRecordScreen, w.at(kHubAt) + 0x400);
}

// The player's record's floats (+12, +20, +24 of [0x826D84CC] + 8: stored
// with stfs) compared as floats, then zeroed for the block's comparison.
void RecordFloats(PPCContext&, const World& w, Outputs& out) {
  const uint32_t record = w.at(kHubAt) + 0x400 + 8;
  for (const uint32_t at : {12u, 20u, 24u}) {
    out.Floats(w.base, record + at, 1);
    guest::StoreU32(w.base, record + at, 0);
  }
}

void HubPages(Context& h, const World& w) {
  auto menu = [](PPCContext& t, Random& r, const World& w) {
    HubMenuWorld(r, w);
    t.r3.u64 = w.at(kModeAt);
    t.r4.u64 = w.at(kModeAt);
  };
  Run(h, w, {"Type2Hub::FillPlayerRecord (sub_8218DA48)", 100, __imp__sub_8218DA48,
             hooks::HubFillPlayerRecord, {{-6176, -32}}, menu, RecordFloats});
  Run(h, w, {"Type2Hub::NoticePage (sub_8218E8F8; sub_8218DBD0, sub_8218DD78)", 300,
             __imp__sub_8218E8F8, hooks::HubNoticePage, {{-112, -24}}, menu, nullptr, true});
  // OnDialogDone: SetText's parameter area left out.
  Run(h, w, {"Type2Hub::OnDialogDone (sub_8218F800; sub_8218F530, sub_8218DD78)", 300,
             __imp__sub_8218F800, hooks::HubOnDialogDone, {{-128, -80}, {-48, -32}},
             [menu](PPCContext& t, Random& r, const World& w) {
               menu(t, r, w);
               t.r3.u64 = r.Next();
             },
             nullptr, true});
  Run(h, w, {"Type2Hub::MenuPage (sub_8218FCC8; sub_8218DA48, sub_8218F530)", 300,
             __imp__sub_8218FCC8, hooks::HubMenuPage, {{-2112, -2064}, {-2032, -40}}, menu,
             RecordFloats});
}

void Hub(Context& h, const World& w) {
  auto hub = [](PPCContext&, Random& r, const World& w) { HubWorld(r, w); };
  auto machine = [hub](PPCContext& t, Random& r, const World& w) {
    hub(t, r, w);
    t.r3.u64 = w.at(kMachineAt);
    t.r4.u64 = w.at(kModeAt);
  };
  auto data = [hub](PPCContext& t, Random& r, const World& w) {
    hub(t, r, w);
    t.r3.u64 = w.data();
  };
  Run(h, w, {"Type2Hub::OnYes (sub_8218D6A8)", 20, __imp__sub_8218D6A8, hooks::HubOnYes, {}, hub});
  Run(h, w, {"Type2Hub::OnNo (sub_8218D6C0)", 20, __imp__sub_8218D6C0, hooks::HubOnNo, {}, hub});
  Run(h, w, {"Type2Hub::ImagesLoadedA (sub_8218E2D8)", 100, __imp__sub_8218E2D8,
             hooks::HubImagesLoadedA, {}, data});
  Run(h, w, {"Type2Hub::ImagesLoadedB (sub_8218E320)", 100, __imp__sub_8218E320,
             hooks::HubImagesLoadedB, {}, data});
  Run(h, w, {"Type2Hub::WaitForLoad (sub_8218E420; sub_8218E2D8, sub_8218E320)", 300,
             __imp__sub_8218E420, hooks::HubWaitForLoad, {{-112, -24}}, machine});
  if (hooks::NativeHeapActive()) {
    // FreeImage: the record's two blocks the layer's pages.
    Run(h, w, {"Type2Hub::FreeImage (sub_8218D650)", 40, __imp__sub_8218D650,
               hooks::HubFreeImage, {{-96, -16}},
               [](PPCContext& t, Random& r, const World& w) {
                 const uint32_t record = w.at(kHubAt) + 0x340;
                 guest::StoreU32(w.base, record + 8, MakePages(w.base, 24, r));
                 guest::StoreU32(w.base, record + 12, MakePages(w.base, 12, r));
                 t.r3.u64 = record;
               }});
  }
  Run(h, w, {"Type2Hub::OpenSelected (sub_8218E5B0)", 100, __imp__sub_8218E5B0,
             hooks::HubOpenSelected, {}, machine});
  Run(h, w, {"Type2Hub::Reenter (sub_8218D9B8)", 100, __imp__sub_8218D9B8, hooks::HubReenter,
             {},
             [machine](PPCContext& t, Random& r, const World& w) {
               machine(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), 0);
               }
             }});
  Run(h, w, {"Type2Hub::Choose (sub_8218E768; sub_8218E7A4)", 100, __imp__sub_8218E768,
             hooks::HubChoose, {},
             [machine](PPCContext& t, Random& r, const World& w) {
               machine(t, r, w);
               t.r3.u64 = w.at(kModeAt);
               t.r4.u64 = w.at(kMachineAt);
             }});
  Run(h, w, {"Type2Hub::Unload (sub_8218E530)", 60, __imp__sub_8218E530, hooks::HubUnload,
             {{-128, -40}}, machine});
  Run(h, w, {"Type2Hub::Close (sub_8218E688)", 60, __imp__sub_8218E688, hooks::HubClose,
             {{-112, -24}}, machine});
  // SetPrompts: each pair's values -1 or small (stored into nodes' +108).
  auto prompts = [hub](PPCContext& t, Random& r, const World& w) {
    hub(t, r, w);
    SmallNodes(r, w);
    uint64_t* regs[6] = {&t.r3.u64, &t.r4.u64, &t.r5.u64, &t.r6.u64, &t.r7.u64, &t.r8.u64};
    for (uint64_t* reg : regs) {
      *reg = r.Below(4) == 0 ? 0xFFFFFFFFFFFFFFFFull : r.Below(12);
    }
  };
  Run(h, w, {"Type2Hub::SetPrompts (sub_8218F530)", 200, __imp__sub_8218F530,
             hooks::HubSetPrompts, {{-192, -64}}, prompts});
  Run(h, w, {"Type2Hub::Back (sub_8218F698; sub_8218F530)", 100, __imp__sub_8218F698,
             hooks::HubBack, {{-192, -64}},
             [prompts](PPCContext& t, Random& r, const World& w) {
               prompts(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), 0);
               }
             }});
  // SetUpStatus: SetText's parameter area past its r7 (+56 to +79, the
  // original's leftovers) left out.
  auto status = [hub](PPCContext& t, Random& r, const World& w) {
    hub(t, r, w);
    SmallNodes(r, w);
  };
  Run(h, w, {"Type2Hub::SetUpStatus (sub_8218E810)", 100, __imp__sub_8218E810,
             hooks::HubSetUpStatus, {{-128, -72}, {-48, -40}}, status, nullptr, true});
  Run(h, w, {"Type2Hub::NextNotice (sub_8218DBD0)", 200, __imp__sub_8218DBD0,
             hooks::HubNextNotice, {{-96, -16}}, hub, nullptr, true});
  Run(h, w, {"Type2Hub::NextNotice2 (sub_8218DD78)", 200, __imp__sub_8218DD78,
             hooks::HubNextNotice2, {{-96, -8}}, hub, nullptr, true});
  // Start: SetUpStatus's frame under its own (the same words), NextNotice's
  // and NextNotice2's back chains over it.
  Run(h, w, {"Type2Hub::Start (sub_8218F6E0; sub_8218E810, sub_8218DBD0, sub_8218DD78)", 200,
             __imp__sub_8218F6E0, hooks::HubStart, {{-112, -24}, {-240, -184}, {-160, -152}},
             [status](PPCContext& t, Random& r, const World& w) {
               status(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), r.Below(2) == 0 ? 4 : 5);
               }
             },
             nullptr, true});
  Run(h, w, {"Type2Hub::MarkNoticeImages (sub_8218DCC0)", 200, __imp__sub_8218DCC0,
             hooks::HubMarkNoticeImages, {{-160, -56}}, hub});
  if (!hooks::NativeHeapActive()) {
    return;
  }
  // Create: the three blocks compared by content (the data's first 448
  // bytes), MarkNoticeImages' frame under its own.
  Run(h, w, {"Type2Hub::Create (sub_8218E078; sub_8218DCC0)", 100, __imp__sub_8218E078,
             hooks::HubCreate, {{-128, -40}, {-288, -184}}, machine,
             [&h](PPCContext&, const World&, Outputs& out) {
               Allocated(h.base, out, t2::kHubPages, 28);
               Allocated(h.base, out, t2::kHubMenu, 68);
               Allocated(h.base, out, t2::kHubData, 448);
             }});
  // FreeImages and Destroy: the image lists (counts 0 to 3) of records in
  // the layer's pages, each with its two blocks.
  auto images = [](Random& r, uint32_t data, const World& w) {
    for (const uint32_t list : {32u, 220u}) {
      const uint32_t count = r.Below(4);
      guest::StoreU32(w.base, data + list + 184, count);
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t record = MakePages(w.base, 16, r);
        guest::StoreU32(w.base, record + 8, MakePages(w.base, 12, r));
        guest::StoreU32(w.base, record + 12, MakePages(w.base, 24, r));
        guest::StoreU32(w.base, data + list + 4 * i, record);
      }
    }
  };
  Run(h, w, {"Type2Hub::FreeImages (sub_8218DE88; sub_8218D650)", 100, __imp__sub_8218DE88,
             hooks::HubFreeImages, {{-112, -32}, {-208, -128}},
             [hub, images](PPCContext& t, Random& r, const World& w) {
               hub(t, r, w);
               images(r, w.data(), w);
               t.r3.u64 = w.data();
             }});
  Run(h, w, {"Type2Hub::Destroy (sub_8218E1A8; sub_8218DE88)", 100, __imp__sub_8218E1A8,
             hooks::HubDestroy, {{-128, -40}, {-240, -160}, {-336, -256}},
             [machine, images](PPCContext& t, Random& r, const World& w) {
               machine(t, r, w);
               uint8_t* base = w.base;
               const uint32_t data = MakePages(base, 448, r);
               WriteRandom(base, data, 448, r);
               images(r, data, w);
               guest::StoreU32(base, t2::kHubPages, MakePages(base, 28, r));
               guest::StoreU32(base, t2::kHubMenu, MakePages(base, 68, r));
               guest::StoreU32(base, t2::kHubData, data);
             }});
  // The notice images' loads: Archive::Load recorded (no load queued).
  Run(h, w, {"Type2Hub::LoadNoticeImage (sub_8218DF38)", 40, __imp__sub_8218DF38,
             hooks::HubLoadNoticeImage, {{-144, -56}},
             [data](PPCContext& t, Random& r, const World& w) {
               data(t, r, w);
               for (const uint32_t list : {32u, 220u}) {
                 guest::StoreU32(w.base, w.data() + list + 184, r.Below(8));
               }
               t.r4.u64 = r.Below(46);
             }});
  Run(h, w, {"Type2Hub::LoadNoticeImages (sub_8218E240; sub_8218DF38)", 40,
             __imp__sub_8218E240, hooks::HubLoadNoticeImages, {{-112, -24}, {-256, -168}}, hub});
  // OpenMovie: the selected garage car copied into its frame (compared).
  Run(h, w, {"Type2Hub::OpenMovie (sub_8218E368; sub_8218E240)", 60, __imp__sub_8218E368,
             hooks::HubOpenMovie, {{-2064, -8}, {-2176, -2088}, {-2320, -2232}}, hub});
  // ImageLoaded: an XPR2 file of 1 to 4 records in the layer's pages (the
  // callback frees it), as the location screen's harness makes them; the
  // XPR's and the texture record's words compared (the texture and the
  // XPR's +16 as offsets in its header block), then the blocks freed.
  Run(h, w, {"Type2Hub::ImageLoaded (sub_8218D8E8)", 60, __imp__sub_8218D8E8,
             hooks::HubImageLoaded, {{-144, -24}},
             [hub](PPCContext& t, Random& r, const World& w) {
               hub(t, r, w);
               uint8_t* base = w.base;
               const uint32_t records = 1 + r.Below(4);
               const uint32_t textures_at = 4 + 16 * records;
               const uint32_t header_bytes = textures_at + 52 * records + 4 * r.Below(16);
               const uint32_t data_bytes = 1 + r.Below(4000);
               const uint32_t bytes = 12 + header_bytes + data_bytes;
               const uint32_t buffer = MakePages(base, bytes, r);
               WriteRandom(base, buffer, bytes & ~3u, r);
               guest::StoreU32(base, buffer + 0, 0x58505232);
               guest::StoreU32(base, buffer + 4, header_bytes);
               guest::StoreU32(base, buffer + 8, data_bytes);
               const uint32_t header = buffer + 12;
               guest::StoreU32(base, header, records);
               for (uint32_t i = 0; i < records; i++) {
                 const uint32_t record = header + 4 + 16 * i;
                 const bool texture = i == 0 || r.Below(2) == 0;
                 guest::StoreU32(base, record,
                                 (texture ? 0x54580000u : 0x56420000u) | r.Below(0x10000));
                 guest::StoreU32(base, record + 4, textures_at + 52 * i);
               }
               t.r4.u64 = buffer;
               t.r6.u64 = w.at(kHubAt) + 0x340;
             },
             [&h](PPCContext&, const World& w, Outputs& out) {
               uint8_t* base = h.base;
               const uint32_t image = w.at(kHubAt) + 0x340;
               const uint32_t texture = guest::LoadU32(base, image + 8);
               const uint32_t xpr = guest::LoadU32(base, image + 12);
               out.Word(memory::IsPages(texture) ? 1 : 0);
               out.Word(memory::IsPages(xpr) ? 1 : 0);
               if (!memory::IsPages(texture) || !memory::IsPages(xpr)) {
                 return;
               }
               const uint32_t header = guest::LoadU32(base, xpr + 0);
               const uint32_t data = guest::LoadU32(base, xpr + 8);
               out.Word(guest::LoadU32(base, xpr + 4));
               out.Word(guest::LoadU32(base, xpr + 12));
               out.Word(guest::LoadU32(base, xpr + 16) - header);
               out.Word(guest::LoadU32(base, xpr + 20));
               out.Word(guest::LoadU32(base, texture + 0) - header);
               out.Word(guest::LoadU32(base, texture + 4));
               out.Word(guest::LoadU32(base, texture + 8));
               memory::FreePages(header - models::kBlockHeaderBytes);
               memory::crt::XMemFree(data - models::kBlockHeaderBytes, models::kXprDataAllocation);
               memory::FreePages(texture);
               memory::FreePages(xpr);
               guest::StoreU32(base, image + 8, 1);
               guest::StoreU32(base, image + 12, 1);
             }});
  HubMovie(h, w);
  HubPages(h, w);
}

}  // namespace

void Type2Screens(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type2_screens: no block from the allocation layer");
    return;
  }
  InitStates(h, w);
  PlateEditor(h, w);
  if (hooks::NativeHeapActive()) {
    PlateCells(h, w);
    PlateTransitions(h, w);
    PlatePages(h, w);
    SheetAndBuild(h, w);
    Hub(h, w);
  }
}

}  // namespace harness
