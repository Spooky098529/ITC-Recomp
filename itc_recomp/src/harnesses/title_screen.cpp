// The title screen (src/title_screen/, docs/functions/title-screen.md)
// against the recompiled functions, laid as the race menu's harness lays its
// world (race_menu.cpp). Each case fills a world of its own (one block from
// the allocation layer) with random words from the case's seed: the
// machine, the mode with its message ring, the title load's block, the title
// screen's four blocks, movies 46 and 0 over a pool of 40 nodes (each child
// of a node another of the pool, all TXTs) and the movie table's pairs
// naming them, the players' action mappers and the sound manager, with
// player 0's save data and a string table of our own; points the globals
// into it (put back after); shapes the fields each function tests; runs the
// original (with --native_title_screen off, so what it calls of the title
// screen's is the original too) or the register steps
// (hooks/title_screen_native.h); and compares the block, the save data, the
// globals, the function's frame without its register saves, the caller's
// area above r1, r1 and r3. UiMovie::Run's update is stood in for (each
// call's return address, stack pointer and arguments kept, answered 0 or 1
// from the case's generator); UiMovies::Open's and Delete's calls are
// recorded on both sides and answered 1; the blocks VirtualAlloc makes (the
// layer's pages) are compared by content and freed. Nothing here is a float.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/title_screen_native.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "title_screen/title_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_title_screen);

namespace harness {

namespace {

namespace ts = title_screen;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;         // the machine (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;         // the mode (its ring at +204)
constexpr uint32_t kLoadBlockAt = 0x2A00;    // [0x82814E28] (68 bytes)
constexpr uint32_t kPagesAt = 0x2A80;        // [0x82814E2C] (28 bytes)
constexpr uint32_t kListAt = 0x2AC0;         // [0x82814E30] (72 bytes)
constexpr uint32_t kWordsAt = 0x2B40;        // [0x82814E34] (68 bytes)
constexpr uint32_t kMenuAt = 0x2BC0;         // [0x82814E38] (68 bytes)
constexpr uint32_t kPairsAt = 0x2C00;        // the movie table's pairs
constexpr uint32_t kMenuWorldAt = 0x2D00;    // the sound manager and its vtable
constexpr uint32_t kMovie46At = 0x3800;      // movie 46 (0xA100 bytes)
constexpr uint32_t kMovie0At = 0xDA00;       // movie 0 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x17C00;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x21C00;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the movies' named animations shaped
constexpr uint32_t kMappersAt = 0x22000;     // 4 action mappers of 0x200
constexpr uint32_t kMapperVtablesAt = 0x22800;  // their vtables (0x40 each)
constexpr uint32_t kBlockBytes = 0x22A00;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kSaveSlots = 0x826C5B58;  // the players' save data
constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kFades = 0x826A50AC;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ts::kLoadBlock, 20},  // the title load's and the title screen's blocks
    {ts::kEntered, 4},
    {ts::kPadBlocked0, 8},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kSaveSlots, 16},
    {kFades, 64},
    {menu_helpers::kMapperEntries, 32},
    {menu_helpers::kControllerEntries, 16},
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
        save(memory::Alloc(kSaveDataBytes, 0)) {
    if (strings != 0) {
      MakeStrings();
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(strings);
    memory::Free(save);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && strings != 0 && save != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint32_t machine() const { return at(kMachineAt); }
  uint32_t mode() const { return at(kModeAt); }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  // A string table of our own ([0x826C5C34] pointed at it): its 18 files
  // all one container of 8,192 entries over 16 strings of letters, so any id
  // below 8,192 has a string; no argument codes (the varargs the native
  // callers pass as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;
  uint32_t save;  // player 0's save data (all four slots point at it)

 private:
  void MakeStrings() {
    Random r(0x5469746C65ull);  // "Title"
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

// One movie over the pool: its live scene node 0, the named animations over
// the root (each one of the 8 animations of no tracks); its loaded flag set
// or not.
void MakeMovie(uint8_t* base, const World& w, uint32_t movie, Random& r) {
  ui_movies::InitAnimationLists(base, movie);
  for (const uint32_t field : {16u, 20u, 24u}) {
    guest::StoreU32(base, movie + field, w.node(0));
  }
  for (uint32_t k = 0; k < kNamed; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, w.node(0));
    guest::StoreU32(base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
  }
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + 40960, r.Below(3));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  // The title load's block: its transition (+8) -1 or 0 to 12, its page
  // (+12) -1, 4 or 5 (the debug pages read the pad), its started word 0, 1
  // or any.
  const uint32_t load = w.at(kLoadBlockAt);
  guest::StoreU32(base, load + 8, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(13));
  guest::StoreU32(base, load + 12, r.Below(3) == 0 ? 0xFFFFFFFFu : 4 + r.Below(2));
  guest::StoreU32(base, load + 16,
                  r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
  // The title screen's page object's words small or -1.
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, w.at(kPagesAt) + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(6));
  }
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.mode() + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.mode() + 204 + 604, r.Below(60));
  // The nodes: all TXTs, each node's children the pool's (their index +108
  // small).
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(60));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  // The scenes' animations: the 8 of no tracks. Each node's first animation
  // entry (+116) 40 to 47, its entries 40 to 63 those (as the race menu's
  // harness has them: never a wild entry).
  for (uint32_t n = 0; n < kNodeCount; n++) {
    guest::StoreU32(base, w.node(n) + 116, 40 + r.Below(8));
    for (uint32_t e = 40; e < 64; e++) {
      guest::StoreU32(base, w.node(n) + 128 + 8 * e,
                      w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
    }
  }
  for (uint32_t k = 0; k < kAnimations; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  MakeMovie(base, w, w.at(kMovie46At), r);
  MakeMovie(base, w, w.at(kMovie0At), r);
  // The root's own children none (+104 0), so UiMovie::ResetPose's walks
  // copy nothing; FindNode reads the root's table, not its count.
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, w.at(kPairsAt), ts::kTitleMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, w.at(kMovie0At));
  guest::StoreU32(base, w.at(kPairsAt) + 8, ts::kLoadMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 12, w.at(kMovie46At));
  // Player 0's save data: random words.
  WriteRandom(base, w.save, kSaveDataBytes, r);
  // The fades' words: their counts (+48, +56) 0 or not.
  WriteRandom(base, kFades, 64, r);
  for (const uint32_t count : {0x826A50E4u, 0x826A50DCu}) {
    guest::StoreU32(base, count, r.Below(2) == 0 ? 0 : r.Below(30));
  }
  // The pad blocked or not, how the title was entered.
  guest::StoreU32(base, ts::kPadBlocked0, r.Below(3) == 0 ? 1 : 0);
  guest::StoreU32(base, ts::kPadBlocked1, r.Below(3) == 0 ? 1 : 0);
  guest::StoreU32(base, ts::kEntered, r.Below(4));
  // The machine's words: +9608, +9612, +9616 0 or 1 or any.
  for (const uint32_t field : {ts::kMachineLoaded, ts::kMachineReenter, ts::kMachineDelete}) {
    guest::StoreU32(base, w.machine() + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, ts::kLoadBlock, w.at(kLoadBlockAt));
  guest::StoreU32(base, ts::kPages, w.at(kPagesAt));
  guest::StoreU32(base, ts::kList, w.at(kListAt));
  guest::StoreU32(base, ts::kWords, w.at(kWordsAt));
  guest::StoreU32(base, ts::kMenu, w.at(kMenuAt));
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pairs not there (UiMovies::Find answers the missing
  // movie, movie 0).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 2);
  guest::StoreU32(base, kMissingMovie, w.at(kMovie0At));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, w.save);
  }
}

// UiMovies::Open's and Delete's calls, recorded on both sides and answered
// 1 (Delete would free our movie). Other addresses go to the recorder set
// before the run, when there's one.
std::vector<uint32_t> g_opens;
hooks::StartUpRecorder g_chained = nullptr;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address == ts::kBootDataLoad) {
    // BootData::Load (TitleLoad::Start's): recorded, r3 left (the harnesses
    // run before the boot's data is loaded, which it would load).
    g_opens.insert(g_opens.end(), {address, ctx.r3.u32, ctx.r1.u32, uint32_t(ctx.lr)});
    return true;
  }
  if (address != ts::kMoviesOpen && address != ts::kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == ts::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
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

// The text records the setters made (stored in movie 0's map): their
// words and buffers compared, freed, their map words 0 (as the race menu's
// harness does).
void TextRecords(uint8_t* base, uint32_t movie, Outputs& out) {
  const uint32_t map = movie + ui_movies::kMovieTexts;
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

// The getter sub_82090770 (r3 = +60), as the race menu's harness uses it.
constexpr uint32_t kGet60 = 0x82090770;

// The sound manager ([0x826C5C80], in the menus' world): IsCuePlaying (slot
// 14) and PlayCue2D (slot 10, Sound::Play's) the getter (its +60's low byte
// 0 half the time).
void SoundManager(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t manager = w.at(kMenuWorldAt), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60,
                  r.Below(2) == 0 ? uint32_t(r.Next()) & ~0xFFu : uint32_t(r.Next()));
  guest::StoreU32(base, vtable + ts::kIsCuePlayingSlot, kGet60);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
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
// Run after the function, before the outputs are read (the allocated blocks
// compared and freed).
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
  AfterFn after = nullptr;
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
      t.r7.u64 = r.Next();
      t.r8.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_opens.clear();
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordOpen;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      if (v == 0) {
        const bool native = ITC_FLAG(native_title_screen);
        ITC_FLAG(native_title_screen) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_title_screen) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
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
      TextRecords(h.base, w.at(kMovie0At), out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      out[v].Bytes(Read(h.base, w.save, kSaveDataBytes));
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

// r3 the machine (now and then with high bits), r4 the mode.
void Machine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.machine() | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = w.mode();
}

// r3 the mode.
void Mode(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.mode(); }

// The machine empty or holding random transitions with keys of small
// numbers, some the Init's (Add replaces those).
void InitMachine(PPCContext& t, Random& r, const World& w) {
  namespace gm = game_modes;
  const uint32_t machine = w.machine();
  if (r.Below(2) == 0) {
    gm::StateMachineConstruct(w.base, machine);
    guest::StoreU32(w.base, machine + gm::kTransitionCount, 0);
  } else {
    const uint32_t count = 8 + r.Below(160);
    guest::StoreU32(w.base, machine + gm::kTransitionCount, count);
    for (uint32_t i = 0; i < gm::kTransitions; i++) {
      const uint32_t at = machine + gm::kTransitionBytes * i;
      guest::StoreU32(w.base, at, r.Below(12));
      guest::StoreU32(w.base, at + 4, r.Below(20));
      guest::StoreU32(w.base, at + 8, r.Below(8) == 0 ? 1 : 0);
      guest::StoreU32(w.base, at + 12, 0);
    }
  }
  t.r3.u64 = machine;
}

void Inits(Context& h, const World& w) {
  // Their frames but their saves (r27 on) compared.
  Run(h, w, {"TitleLoad::Init (sub_822213B0)", 24, __imp__sub_822213B0, hooks::TitleLoadInit,
             {{-912, -48}}, InitMachine});
  Run(h, w, {"TitleScreen::Init (sub_82224270)", 24, __imp__sub_82224270,
             hooks::TitleScreenInit, {{-1232, -48}}, InitMachine});
}

void TitleLoad(Context& h, const World& w) {
  // Create: the block VirtualAlloc made compared by content.
  Run(h, w, {"TitleLoad::Create (sub_82220BA8)", 60, __imp__sub_82220BA8, hooks::TitleLoadCreate,
             {{-112, -24}}, Machine,
             [&h](PPCContext& t, const World&, Outputs& out) {
               // r3 the block's address (VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(h.base, ts::kLoadBlock) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(h.base, out, ts::kLoadBlock, 68);
             }});
  // Destroy: the block the layer's pages (whether it was freed compared).
  Run(h, w, {"TitleLoad::Destroy (sub_82220468)", 60, __imp__sub_82220468,
             hooks::TitleLoadDestroy, {{-96, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, ts::kLoadBlock, MakePages(w.base, 68, r));
             }});
  Run(h, w, {"TitleLoad::OpenMovie (sub_822204B0)", 20, __imp__sub_822204B0,
             hooks::TitleLoadOpenMovie, {}, Machine});
  Run(h, w, {"TitleLoad::WaitForMovie (sub_82220C18)", 100, __imp__sub_82220C18,
             hooks::TitleLoadWaitForMovie, {{-96, -16}}, Machine});
  // Start: BootData::Load recorded (RecordOpen).
  Run(h, w, {"TitleLoad::Start (sub_82220C88)", 40, __imp__sub_82220C88, hooks::TitleLoadStart,
             {{-96, -8}}, Machine});
  Run(h, w, {"TitleLoad::Unload (sub_82220EF8)", 60, __imp__sub_82220EF8,
             hooks::TitleLoadUnload, {{-112, -16}}, Machine});
  Run(h, w, {"TitleLoad::OpenTitle (sub_82220F60)", 40, __imp__sub_82220F60,
             hooks::TitleLoadOpenTitle, {}, Machine});
  // Leave: now and then no movie found (the pairs not there, the missing
  // movie 0).
  Run(h, w, {"TitleLoad::Leave (sub_82220FC8)", 60, __imp__sub_82220FC8, hooks::TitleLoadLeave,
             {{-112, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, ui_movies::kMovieTable + 8, 0);
                 guest::StoreU32(w.base, kMissingMovie, 0);
               }
             }});
  Run(h, w, {"TitleLoad::PostClose (sub_82221078)", 40, __imp__sub_82221078,
             hooks::TitleLoadPostClose, {}, Mode});
  Run(h, w, {"TitleLoad::WaitForArchive (sub_822204C0)", 20, __imp__sub_822204C0,
             hooks::TitleLoadWaitForArchive, {{-96, -8}}, Mode});
  // PlayLogo: r3 the block's started word.
  Run(h, w, {"TitleLoad::PlayLogo (sub_822210C8)", 100, __imp__sub_822210C8,
             hooks::TitleLoadPlayLogo, {{-112, -24}},
             [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kLoadBlockAt) + 16; }});
  // Draw: the page -1, 4 or 5; only its own frame compared.
  Run(h, w, {"TitleLoad::Draw (sub_82221198)", 100, __imp__sub_82221198, hooks::TitleLoadDraw,
             {{-112, -8}}, Machine});
  // Update: the transition -1 or 0 to 12 (the debug sequence's steps and
  // PlayLogo); its frame without its saves (r24 on).
  Run(h, w, {"TitleLoad::Update (sub_82221228)", 200, __imp__sub_82221228,
             hooks::TitleLoadUpdate, {{-208, -72}}, Machine});
}

// The title's words: +4 (the main menu's item) 0 to 9, +16 (from the boot)
// 0 or not.
void Words(Random& r, const World& w) {
  guest::StoreU32(w.base, w.at(kWordsAt) + 4, r.Below(10));
  guest::StoreU32(w.base, w.at(kWordsAt) + 16, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
}

void ScreenHandlers(Context& h, const World& w) {
  // Create: the four blocks VirtualAlloc made compared by content.
  Run(h, w, {"TitleScreen::Create (sub_82221D98)", 60, __imp__sub_82221D98,
             hooks::TitleScreenCreate, {{-112, -40}}, Machine,
             [&h](PPCContext& t, const World&, Outputs& out) {
               // r3 the title's words' address (the last VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(h.base, ts::kWords) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(h.base, out, ts::kPages, 28);
               Allocated(h.base, out, ts::kList, 72);
               Allocated(h.base, out, ts::kMenu, 68);
               Allocated(h.base, out, ts::kWords, 68);
             }});
  // Destroy: the four blocks the layer's pages, the list's +68 0 or pages
  // (whether each was freed compared); the item 0 to 9, the list's
  // selection 2 or not.
  Run(h, w, {"TitleScreen::Destroy (sub_82221EC0)", 100, __imp__sub_82221EC0,
             hooks::TitleScreenDestroy, {{-128, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               const uint32_t list = MakePages(base, 72, r);
               guest::StoreU32(base, list + 16, r.Below(2) == 0 ? 2 : r.Below(7));
               guest::StoreU32(base, list + 68, r.Below(2) == 0 ? 0 : MakePages(base, 64, r));
               const uint32_t words = MakePages(base, 68, r);
               guest::StoreU32(base, words + 4, r.Below(10));
               guest::StoreU32(base, ts::kPages, MakePages(base, 28, r));
               guest::StoreU32(base, ts::kList, list);
               guest::StoreU32(base, ts::kMenu, MakePages(base, 68, r));
               guest::StoreU32(base, ts::kWords, words);
             }});
  Run(h, w, {"TitleScreen::OpenMovie (sub_82221990)", 20, __imp__sub_82221990,
             hooks::TitleScreenOpenMovie, {}, Machine});
  Run(h, w, {"TitleScreen::WaitForMovie (sub_82221FC8)", 100, __imp__sub_82221FC8,
             hooks::TitleScreenWaitForMovie, {{-96, -16}}, Machine});
  // SetUp: the movie's seven texts; its frame without SetText's parameter
  // area (+48 to +79: r7 on, the original's leftovers) and its saves.
  Run(h, w, {"TitleScreen::SetUp (sub_82222A50)", 100, __imp__sub_82222A50,
             hooks::TitleScreenSetUp, {{-176, -128}, {-96, -40}},
             [](PPCContext&, Random& r, const World& w) { Words(r, w); }});
  // Start: how the title was entered 0 to 3, the sound manager; its frame
  // and SetUp's below it, without SetText's parameter area and SetUp's
  // saves.
  Run(h, w, {"TitleScreen::Start (sub_82223AB8; sub_82222A50)", 100, __imp__sub_82223AB8,
             hooks::TitleScreenStart, {{-112, -16}, {-288, -240}, {-208, -152}},
             [](PPCContext&, Random& r, const World& w) {
               Words(r, w);
               SoundManager(r, w);
             }});
  Run(h, w, {"TitleScreen::Unload (sub_822224D0)", 60, __imp__sub_822224D0,
             hooks::TitleScreenUnload, {{-112, -24}}, Machine});
  // OpenNext: the list's selection 6 or not.
  Run(h, w, {"TitleScreen::OpenNext (sub_82222538)", 60, __imp__sub_82222538,
             hooks::TitleScreenOpenNext, {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.at(kListAt) + 16, r.Below(2) == 0 ? 6 : r.Below(8));
             }});
  // Leave: now and then no movie found.
  Run(h, w, {"TitleScreen::Leave (sub_822225D8)", 60, __imp__sub_822225D8,
             hooks::TitleScreenLeave, {{-112, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, ui_movies::kMovieTable + 8, 0);
                 guest::StoreU32(w.base, kMissingMovie, 0);
               }
             }});
  Run(h, w, {"TitleScreen::OnGameDataLoaded (sub_822219A0)", 40, __imp__sub_822219A0,
             hooks::TitleScreenOnGameDataLoaded, {}, Machine});
  Run(h, w, {"TitleScreen::OnInterrupt (sub_82221A78)", 10, __imp__sub_82221A78,
             hooks::TitleScreenOnInterrupt, {}, Machine});
  Run(h, w, {"TitleScreen::OnResume (sub_82221A90)", 10, __imp__sub_82221A90,
             hooks::TitleScreenOnResume, {}, Machine,
             [&h](PPCContext&, const World&, Outputs& out) {
               out.Bytes(Read(h.base, ts::kSystemChecks, 16));
             }});
  Run(h, w, {"TitleScreen::LoadGameData (sub_82222688)", 40, __imp__sub_82222688,
             hooks::TitleScreenLoadGameData, {}, Mode});
  // LeavePage: the item 0 to 9.
  Run(h, w, {"TitleScreen::LeavePage (sub_822229D0)", 60, __imp__sub_822229D0,
             hooks::TitleScreenLeavePage, {},
             [](PPCContext& t, Random& r, const World& w) {
               Words(r, w);
               t.r3.u64 = w.mode();
               t.r4.u64 = w.machine();
             }});
  // Draw: the page -1, 1 or 5 (LoadGameData, LeavePage); only its own frame
  // compared.
  Run(h, w, {"TitleScreen::Draw (sub_82223A18)", 100, __imp__sub_82223A18,
             hooks::TitleScreenDraw, {{-128, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               Words(r, w);
               t.r3.u64 = w.machine() | (uint64_t(r.Below(2)) << 32);
               t.r4.u64 = w.mode();
               const uint32_t pick = r.Below(3);
               guest::StoreU32(w.base, w.at(kPagesAt), pick == 0 ? 0xFFFFFFFFu : pick == 1 ? 1 : 5);
             }});
  // Update: the running transition -1, another negative, 1 or 3
  // (PressStartLoop, LeavePressStart: the movie's calls only), the queued
  // one -1 or small, the started word 0 or not; only its own frame compared
  // (r19 on saved).
  Run(h, w, {"TitleScreen::Update (sub_822240B0)", 300, __imp__sub_822240B0,
             hooks::TitleScreenUpdateTransitions, {{-288, -112}},
             [](PPCContext&, Random& r, const World& w) {
               uint8_t* base = w.base;
               const uint32_t pages = w.at(kPagesAt);
               const uint32_t pick = r.Below(4);
               guest::StoreU32(base, pages + 12,
                               pick == 0   ? 0xFFFFFFFFu
                               : pick == 1 ? uint32_t(-int32_t(2 + r.Below(5)))
                                           : (r.Below(2) == 0 ? 1 : 3));
               guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(6));
               guest::StoreU32(base, pages + 24, r.Below(3) == 0 ? 1 : 0);
             }});
}

}  // namespace

void TitleScreen(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: title_screen: no block from the allocation layer");
    return;
  }
  Inits(h, w);
  if (hooks::NativeHeapActive()) {
    TitleLoad(h, w);
    ScreenHandlers(h, w);
  }
}

}  // namespace harness
