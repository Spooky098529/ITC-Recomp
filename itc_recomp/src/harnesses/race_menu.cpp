// The race menu (src/race_menu/, docs/functions/race-menu.md) against the
// recompiled functions, laid as the garage screen's harness lays its world
// (garage_screen.cpp). Each case fills a world of its own (one block from
// the allocation layer) with random words from the case's seed: the race
// menu's machine, the mode with its message ring, the two page objects, the
// menu, movies 1 and 16 over a pool of 40 nodes (each child of a node
// another of the pool, all TXTs) and the movie table's pairs naming them,
// the players' action mappers and the sound manager, with player 0's save data and a
// string table of our own; points the globals into it (put back after);
// shapes the fields each function tests; runs the original (with
// --native_race_menu off, so what it calls of the menu's is the original
// too) or the register steps (hooks/race_menu.h); and compares the block,
// the save data, the globals, the function's frame without its register
// saves, the caller's area above r1, r1 and r3. UiMovie::Run's update is
// stood in for (each call's return address, stack pointer and arguments
// kept, answered 0 or 1 from the case's generator); UiMovies::Open's and
// Delete's calls are recorded on both sides and answered 1; the blocks
// VirtualAlloc makes (the layer's pages) are compared by content and freed;
// the text records the setters make are compared and freed.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/race_menu.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "race_menu/race_menu.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_menu);

namespace harness {

namespace {

namespace rm = race_menu;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;         // the machine (10,160 bytes)
constexpr uint32_t kModeAt = 0x2800;         // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2C00;        // [0x826D8404]
constexpr uint32_t kPages2At = 0x2C40;       // [0x826D8408]
constexpr uint32_t kMenuAt = 0x2C80;         // [0x826D840C] (68 bytes)
constexpr uint32_t kPairsAt = 0x2D00;        // the movie table's pairs
constexpr uint32_t kMenuWorldAt = 0x3000;    // the mappers, vtables, sound manager
constexpr uint32_t kMovieAt = 0x4000;        // movie 1 (0xA100 bytes)
constexpr uint32_t kMovie16At = 0xE200;      // movie 16 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x18400;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x22400;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the movies' named animations shaped
constexpr uint32_t kMappersAt = 0x22800;      // 4 action mappers of 0x200
constexpr uint32_t kMapperVtablesAt = 0x23000;  // their vtables (0x40 each)
constexpr uint32_t kBlockBytes = 0x23200;

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
    {rm::kPages, 12},  // the two page objects and the menu
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kSaveSlots, 16},
    {kFades, 64},
    {rm::kBootList, 32},
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
    Random r(0x5261636520ull);  // "Race "
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
  // The page objects' words small or -1.
  for (const uint32_t pages : {w.at(kPagesAt), w.at(kPages2At)}) {
    for (uint32_t k = 0; k < 7; k++) {
      guest::StoreU32(base, pages + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(13));
    }
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
  for (uint32_t k = 0; k < kAnimations; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  MakeMovie(base, w, w.at(kMovieAt), r);
  MakeMovie(base, w, w.at(kMovie16At), r);
  // The root's own children none (+104 0), so UiMovie::ResetPose's walks
  // copy nothing; FindNode reads the root's table, not its count.
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, w.at(kPairsAt), 1);
  guest::StoreU32(base, w.at(kPairsAt) + 4, w.at(kMovieAt));
  guest::StoreU32(base, w.at(kPairsAt) + 8, 16);
  guest::StoreU32(base, w.at(kPairsAt) + 12, w.at(kMovie16At));
  // Player 0's save data: random words.
  WriteRandom(base, w.save, kSaveDataBytes, r);
  // The fades' words: their counts (+48, +56) 0 or not.
  WriteRandom(base, kFades, 64, r);
  for (const uint32_t count : {0x826A50E4u, 0x826A50DCu}) {
    guest::StoreU32(base, count, r.Below(2) == 0 ? 0 : r.Below(30));
  }
  WriteRandom(base, rm::kBootList, 32, r);
  // The machine's words: +9608, +9612, +9616 0 or 1 or any.
  for (const uint32_t field : {rm::kMachineLoaded, rm::kMachineReenter, rm::kMachineDelete}) {
    guest::StoreU32(base, w.machine() + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, rm::kPages, w.at(kPagesAt));
  guest::StoreU32(base, rm::kPages2, w.at(kPages2At));
  guest::StoreU32(base, rm::kMenu, w.at(kMenuAt));
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pairs not there (UiMovies::Find answers the missing
  // movie, movie 1).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 2);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
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
  if (address != rm::kMoviesOpen && address != rm::kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == rm::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
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

// The text records the setters made (stored in movie 1's and 16's maps): their words
// and buffers compared, freed, their map words 0 (as the garage screen's
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
        const bool native = ITC_FLAG(native_race_menu);
        ITC_FLAG(native_race_menu) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_race_menu) = native;
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
      TextRecords(h.base, w.at(kMovieAt), out[v]);
      TextRecords(h.base, w.at(kMovie16At), out[v]);
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

// The progress bits (+10148): one of the shapes the code tests, or any.
uint32_t ProgressBits(Random& r) {
  static constexpr uint32_t kBits[] = {0, 0x20, 0x4, 0x8, 0x10, 0x3C, 0x14, 0x1};
  return r.Below(4) == 0 ? uint32_t(r.Next()) : kBits[r.Below(8)];
}

void Init(Context& h, const World& w) {
  Run(h, w, {"RaceMenu::Init (sub_8218CE10)", 24, __imp__sub_8218CE10, hooks::RaceMenuInit,
             {{-1232, -48}},
             [](PPCContext& t, Random& r, const World& w) {
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
                   guest::StoreU32(w.base, at + 4, r.Below(52));
                   guest::StoreU32(w.base, at + 8, r.Below(8) == 0 ? 1 : 0);
                   guest::StoreU32(w.base, at + 12, 0);
                 }
               }
               t.r3.u64 = machine;
             }});
}

void Handlers(Context& h, const World& w) {
  // Create: the three blocks VirtualAlloc made compared by content.
  Run(h, w, {"RaceMenu::Create (sub_82188E20)", 60, __imp__sub_82188E20, hooks::RaceMenuCreate,
             {{-144, -56}}, Machine,
             [&h](PPCContext& t, const World&, Outputs& out) {
               // r3 the menu's address (the last VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(h.base, rm::kMenu) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(h.base, out, rm::kPages, 28);
               Allocated(h.base, out, rm::kPages2, 28);
               Allocated(h.base, out, rm::kMenu, 68);
             }});
  Run(h, w, {"RaceMenu::OpenMovie (sub_821889E0)", 20, __imp__sub_821889E0,
             hooks::RaceMenuOpenMovie, {{-96, -16}}, Machine});
  Run(h, w, {"RaceMenu::WaitForMovie (sub_821890D8)", 100, __imp__sub_821890D8,
             hooks::RaceMenuWaitForMovie, {{-112, -24}}, Machine});
  Run(h, w, {"RaceMenu::OnYes (sub_821895F0)", 40, __imp__sub_821895F0, hooks::RaceMenuOnYes,
             {{-112, -24}}, Machine});
  Run(h, w, {"RaceMenu::OnNo (sub_82189648)", 40, __imp__sub_82189648, hooks::RaceMenuOnNo,
             {{-112, -24}}, Machine});
  // OnBack: the page before the question 4, 5 or another.
  Run(h, w, {"RaceMenu::OnBack (sub_82188938)", 100, __imp__sub_82188938,
             hooks::RaceMenuOnBack, {},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.machine() + 9644, 3 + r.Below(4));
             }});
  // OnDialogClosed: answered or not.
  Run(h, w, {"RaceMenu::OnDialogClosed (sub_821896A0)", 100, __imp__sub_821896A0,
             hooks::RaceMenuOnDialogClosed, {{-128, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.machine() + 9788, r.Below(2));
             }});
  Run(h, w, {"RaceMenu::Unload (sub_8218AF08)", 60, __imp__sub_8218AF08, hooks::RaceMenuUnload,
             {{-128, -32}}, Machine});
  Run(h, w, {"RaceMenu::LeaveToHub (sub_8218AF80)", 40, __imp__sub_8218AF80,
             hooks::RaceMenuLeaveToHub, {}, Machine});
  // Destroy: the three blocks the layer's pages (whether each was freed
  // compared); the day 2 or not, +9792 1 or not.
  Run(h, w, {"RaceMenu::Destroy (sub_82188FE0)", 60, __imp__sub_82188FE0,
             hooks::RaceMenuDestroy, {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               guest::StoreU32(base, rm::kPages, MakePages(base, 28, r));
               guest::StoreU32(base, rm::kPages2, MakePages(base, 28, r));
               guest::StoreU32(base, rm::kMenu, MakePages(base, 68, r));
               guest::StoreU32(base, w.machine() + 10136, r.Below(2) == 0 ? 2 : r.Below(5));
               guest::StoreU32(base, w.machine() + 9792, r.Below(3));
             }});
  // Back: the history's depth 0 to 11.
  Run(h, w, {"RaceMenu::Back (sub_82188A20)", 60, __imp__sub_82188A20, hooks::RaceMenuBack, {},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU8(w.base, w.machine() + 9776, uint8_t(r.Below(12)));
             }});
  Run(h, w, {"RaceMenu::SetEventRange (sub_82189158)", 60, __imp__sub_82189158,
             hooks::RaceMenuSetEventRange, {},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.machine() + 10148, ProgressBits(r));
             }});
}

// A name record (the machine's +10112): its kinds and indexes small or any.
void NameRecord(uint8_t* base, uint32_t name, Random& r) {
  guest::StoreU32(base, name + 4, r.Below(6));
  guest::StoreU16(base, name + 8, uint16_t(r.Below(4) == 0 ? r.Next() : r.Below(110)));
  guest::StoreU16(base, name + 10, uint16_t(r.Below(2)));
  guest::StoreU32(base, name + 12, r.Below(6));
  guest::StoreU16(base, name + 16, uint16_t(r.Below(4) == 0 ? r.Next() : r.Below(110)));
  guest::StoreU16(base, name + 18, uint16_t(r.Below(2)));
}

// A small value or -1.
uint32_t SmallOrNone(Random& r, uint32_t below) {
  return r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(below);
}

// Refresh's words: pages 0 to 10 (now and then any), the mode 0 to 2, the
// list's cursor within 12 rows of its top, its first and last rows at the
// top and two below it or not, the scroll flag 0 to 3, the variant, the two
// tables' pairs small or -1 (ids below the string table's 8,192), the name
// record and Refresh's frame's words from +80 (`sp` its caller's r1: the
// event names past the list, left as they were) small.
void RefreshWords(Random& r, const World& w, uint32_t sp) {
  uint8_t* base = w.base;
  const uint32_t m = w.machine();
  const uint32_t page = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(11);
  guest::StoreU32(base, m + rm::kPage, r.Below(2) == 0 ? 3 + r.Below(3) : page);
  guest::StoreU32(base, m + 9820, r.Below(3));
  const uint32_t top = r.Below(3);
  const uint32_t row = r.Below(13);
  const uint32_t cursor = top + row;
  guest::StoreU32(base, m + 9972, top);
  guest::StoreU32(base, m + 9828, cursor);
  const uint32_t first_choices[] = {row, cursor, r.Below(15)};
  const uint32_t last_choices[] = {row + 2, cursor, r.Below(15)};
  guest::StoreU32(base, m + 9840, first_choices[r.Below(3)]);
  guest::StoreU32(base, m + 9844, last_choices[r.Below(3)]);
  guest::StoreU32(base, m + 9832,
                  r.Below(2) == 0 ? guest::LoadU32(base, m + 9840 + 4 * r.Below(2))
                                  : r.Below(15));
  guest::StoreU32(base, m + 9836, r.Below(4));
  guest::StoreU32(base, m + 9724, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, m + 10148, ProgressBits(r));
  for (uint32_t k = 0; k < 15; k++) {
    guest::StoreU32(base, m + 9852 + 8 * k, SmallOrNone(r, 11));
    guest::StoreU32(base, m + 9856 + 8 * k, r.Below(4) == 0 ? uint32_t(r.Next())
                                                            : SmallOrNone(r, 4));
    guest::StoreU32(base, m + 9980 + 8 * k, r.Below(20));
    guest::StoreU32(base, m + 9984 + 8 * k, SmallOrNone(r, 20));
  }
  NameRecord(base, m + 10112, r);
  guest::StoreU32(base, m + 10132, r.Below(2) == 0 ? r.Below(100000) : r.Next());
  guest::StoreU32(base, m + 10136, r.Below(1000));
  guest::StoreU32(base, m + 10140, r.Below(7));
  const uint32_t frame = sp - 224;
  for (uint32_t k = 80; k < 176; k += 4) {
    guest::StoreU32(base, frame + k, r.Below(600));
  }
}

void Pages(Context& h, const World& w) {
  // SetUpEvents: the progress bits' shapes; its frame's table words
  // (+80 to +199) compared.
  Run(h, w, {"RaceMenu::SetUpEvents (sub_821891C8)", 80, __imp__sub_821891C8,
             hooks::RaceMenuSetUpEvents, {{-288, -88}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.machine() + 10148, ProgressBits(r));
             }});
  // Refresh: its words (RefreshWords). SetText's parameter area (+48 to +79:
  // r7 on, the original's leftovers) left out.
  Run(h, w, {"RaceMenu::Refresh (sub_8218BC18)", 300, __imp__sub_8218BC18,
             hooks::RaceMenuRefresh, {{-224, -176}, {-144, -64}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               RefreshWords(r, w, t.r1.u32);
             }});
}

// The getter sub_82090770 (r3 = +60) and the leaf recorder sub_820A8420
// (r4's low byte, r5 and r6 at +320, +332 and +336), as the front-end
// screen helpers' harness uses them.
constexpr uint32_t kGet60 = 0x82090770;
constexpr uint32_t kRecordR4R6 = 0x820A8420;
constexpr uint32_t kStopCueSlot = 52;       // 13
constexpr uint32_t kIsCuePlayingSlot = 56;  // 14

// The sound manager ([0x826C5C80], in the menus' world): IsCuePlaying (slot
// 14) and PlayCue2D (slot 10, Sound::Play's) the getter (its +60's low byte
// 0 half the time: both cues playing or neither), StopCue (slot 13) the
// recorder.
void SoundManager(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t manager = w.at(kMenuWorldAt), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60,
                  r.Below(2) == 0 ? uint32_t(r.Next()) & ~0xFFu : uint32_t(r.Next()));
  guest::StoreU32(base, vtable + kIsCuePlayingSlot, kGet60);
  guest::StoreU32(base, vtable + kStopCueSlot, kRecordR4R6);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// The save's day (+6100): 0 to 999 mostly, around 999, above it or any
// (negative too: the weekday is a signed remainder).
uint32_t Day(Random& r) {
  switch (r.Below(5)) {
    case 0: return 995 + r.Below(10);
    case 1: return 1000 + r.Below(100000);
    case 2: return uint32_t(r.Next());
    default: return r.Below(1000);
  }
}

void Start(Context& h, const World& w) {
  // Start: player 0's save data shaped (the progress bits, the day, the
  // money, the name record), the sound manager. Its frame (+0 to +111) and
  // Refresh's below it (as Refresh's harness compares it, from Start's
  // frame, without SetUpEvents' register saves from -264 left there)
  // compared; the callees' frames under them (FindNode's, which the native
  // code doesn't make) not. The recorder's r6 word (+336: StopCue reads r3
  // to r5 only, r6 the original's leftover) left out.
  Run(h, w, {"RaceMenu::Start (sub_8218C9C8)", 200, __imp__sub_8218C9C8, hooks::RaceMenuStart,
             {{-176, -64}, {-400, -352}, {-320, -264}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               guest::StoreU32(base, w.save + 0x199E0, ProgressBits(r));
               guest::StoreU32(base, w.save + 6100, Day(r));
               guest::StoreU32(base, w.save + 6116,
                               r.Below(2) == 0 ? r.Below(100000) : uint32_t(r.Next()));
               NameRecord(base, w.save + 8, r);
               SoundManager(r, w);
             },
             [](PPCContext&, const World& w, Outputs&) {
               guest::StoreU32(w.base, w.at(kMenuWorldAt) + 336, 0);
             }});
}

// The players' action mappers (as the garage screen's harness lays them):
// each controller's entry 0 to 3, each entry's mapper one of ours (its
// vtable's slots 10 and 12 ActionMapper::Pressed and Fired, now and then
// Held), blocked (+4 1) now and then, a few bits set in its words (+448 to
// +471).
void Mappers(Random& r, const World& w) {
  uint8_t* base = w.base;
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t mapper = w.at(kMappersAt) + 0x200 * c;
    const uint32_t vtable = w.at(kMapperVtablesAt) + 0x40 * c;
    guest::StoreU32(base, menu_helpers::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * c + 4,
                    r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + menu_helpers::kPressedOffset,
                    r.Below(5) == 0 ? 0x8210BE70 : menu_helpers::kPressedSlot);
    guest::StoreU32(base, vtable + menu_helpers::kFiredOffset,
                    r.Below(5) == 0 ? 0x8210BE70 : menu_helpers::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
}

// 0 or 1 mostly, now and then any.
uint32_t Flag(Random& r) { return r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2); }

// The transitions' words: the first-time flag (+9812), the mode (+9820)
// and the list's top (+9972) small, the progress bits, the end flag (+9796).
void TransitionWords(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  Machine(t, r, w);
  guest::StoreU32(base, w.machine() + 9812, Flag(r));
  guest::StoreU32(base, w.machine() + 9820, r.Below(3));
  guest::StoreU32(base, w.machine() + 9972, r.Below(3));
  guest::StoreU32(base, w.machine() + 10148, ProgressBits(r));
  guest::StoreU32(base, w.machine() + 9796, Flag(r));
}

void Transitions(Context& h, const World& w) {
  // Each frame compared up to its register saves (the callees' frames
  // below it, which the native code's plain C++ lookups don't make, not).
  Run(h, w, {"RaceMenu::Transition0 (sub_82189740)", 60, __imp__sub_82189740,
             hooks::RaceMenuTransition0, {{-128, -32}}, TransitionWords});
  Run(h, w, {"RaceMenu::Transition3 (sub_82189C28)", 100, __imp__sub_82189C28,
             hooks::RaceMenuTransition3, {{-176, -88}}, TransitionWords});
  Run(h, w, {"RaceMenu::Transition5 (sub_8218A7F0)", 60, __imp__sub_8218A7F0,
             hooks::RaceMenuTransition5, {{-128, -32}}, TransitionWords});
  Run(h, w, {"RaceMenu::Transition12 (sub_8218AE48)", 40, __imp__sub_8218AE48,
             hooks::RaceMenuTransition12, {{-112, -16}}, TransitionWords});
  // AskConfirm: player 0's save +0x1F870 0 or not. SetText's parameter area
  // (+48 to +79: r7 on, the original's leftovers) left out.
  Run(h, w, {"RaceMenu::AskConfirm (sub_8218ACE8)", 80, __imp__sub_8218ACE8,
             hooks::RaceMenuAskConfirm, {{-128, -80}, {-48, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.save + 0x1F870,
                               r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
             }});
  Run(h, w, {"RaceMenu::DebugKindPage (sub_8218BB68)", 20, __imp__sub_8218BB68,
             hooks::RaceMenuDebugKindPage, {{-128, -32}}, Machine});
  // DebugEventPage: the list's count (+9720) 0 or small, its entries (+9656)
  // small (the original reads its frame's strings by them).
  Run(h, w, {"RaceMenu::DebugEventPage (sub_82188A98)", 40, __imp__sub_82188A98,
             hooks::RaceMenuDebugEventPage, {{-240, -96}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.machine() + 9720, r.Below(3) == 0 ? 0 : r.Below(17));
               for (uint32_t k = 0; k < 16; k++) {
                 guest::StoreU32(w.base, w.machine() + 9656 + 4 * k, r.Below(15));
               }
             }});
  // DebugVariantPage: the menu's +8 0 or not.
  Run(h, w, {"RaceMenu::DebugVariantPage (sub_82188C00)", 40, __imp__sub_82188C00,
             hooks::RaceMenuDebugVariantPage, {{-128, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               guest::StoreU32(w.base, w.at(kMenuAt) + 8, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
             }});
  // AnimateVariantPage: the mappers, the variant (+10100) and its flag
  // (+10104) 0 to 2, the cursor's row and its second pair word small or -1.
  Run(h, w, {"RaceMenu::AnimateVariantPage (sub_8218A928)", 200, __imp__sub_8218A928,
             hooks::RaceMenuAnimateVariantPage, {{-128, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               Mappers(r, w);
               const uint32_t m = w.machine();
               guest::StoreU32(base, m + 10100, r.Below(3));
               guest::StoreU32(base, m + 10104, r.Below(3));
               guest::StoreU32(base, m + 9828, r.Below(15));
               guest::StoreU32(base, m + 9972, r.Below(3));
               for (uint32_t k = 0; k < 15; k++) {
                 guest::StoreU32(base, m + 9984 + 8 * k, SmallOrNone(r, 4));
               }
             }});
}

// Movie 1's animations Update times: up to three slots (the scene's -1, 1,
// 4 or 2) in its looping list or queue, their times the floats Update tests
// or not.
void UpdateSlots(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t movie = w.at(kMovieAt);
  static constexpr uint32_t kTimes[] = {0x82001FB0, 0x82001FD8};
  for (uint32_t k = r.Below(4); k > 0; k--) {
    ui_movies::Slot slot{};
    static constexpr uint32_t kIndexes[] = {1, 4, 2, 6};
    slot[0] = kIndexes[r.Below(4)];
    slot[1] = r.Below(6) == 0 ? 8 : 0xFFFFFFFFu;
    slot[2] = 0xFFFFFFFFu;
    slot[3] = w.node(0);
    slot[4] = w.at(kAnimationsAt) + 64 * r.Below(kAnimations);
    slot[5] = r.Below(3) == 0 ? std::bit_cast<uint32_t>(float(r.Below(40)) / 4.0f)
                              : guest::LoadU32(base, kTimes[r.Below(2)]);
    slot[6] = r.Below(2);
    slot[7] = r.Below(2);
    ui_movies::Append(base,
                      movie + (r.Below(2) ? ui_movies::kMovieQueue : ui_movies::kMovieLooping),
                      slot);
  }
}

void Update(Context& h, const World& w) {
  // Update: the callees' words (Refresh's, the transitions', AskConfirm's,
  // the debug pages', AnimateVariantPage's mappers), the page (+9640) 0 to
  // 10 or any, +9780 and +9796 0, 1 or any, movie 1's timed slots, the page
  // object's transition (+12) -1, 0 to 13 or any, its +24 0 or not and +16
  // small or -1. Only Update's own frame words compared (up to its f31 and
  // register saves; the callees' frames below it hold the recompiled
  // callees' register saves).
  Run(h, w, {"RaceMenu::Update (sub_8218C678)", 400, __imp__sub_8218C678, hooks::RaceMenuUpdate,
             {{-176, -88}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               const uint32_t m = w.machine();
               RefreshWords(r, w, t.r1.u32 - 176);
               Mappers(r, w);
               UpdateSlots(r, w);
               guest::StoreU32(base, m + 9812, Flag(r));
               guest::StoreU32(base, m + 10100, r.Below(3));
               guest::StoreU32(base, m + 10104, r.Below(3));
               guest::StoreU32(base, m + 9796, Flag(r));
               guest::StoreU32(base, m + 9780, Flag(r));
               guest::StoreU32(base, m + 9720, r.Below(3) == 0 ? 0 : r.Below(17));
               for (uint32_t k = 0; k < 16; k++) {
                 guest::StoreU32(base, m + 9656 + 4 * k, r.Below(15));
               }
               const uint32_t page = r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(11);
               guest::StoreU32(base, m + rm::kPage, r.Below(2) == 0 ? 3 + r.Below(3) : page);
               guest::StoreU32(base, w.save + 0x1F870, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               guest::StoreU32(base, w.at(kMenuAt) + 8, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               const uint32_t pages = w.at(kPagesAt);
               const uint32_t transition = r.Below(5) == 0   ? 0xFFFFFFFFu
                                           : r.Below(8) == 0 ? uint32_t(r.Next())
                                                             : r.Below(14);
               guest::StoreU32(base, pages + 12, transition);
               guest::StoreU32(base, pages + 24, r.Below(3) == 0 ? Flag(r) : 0);
               guest::StoreU32(base, pages + 16, SmallOrNone(r, 13));
             }});
}

// The menu (as menu_helpers' harness shapes it): items 0 to 2, and the
// mappers' bits now and then only pressed ones (+456), so the menu doesn't
// move and the pages' A and B are read.
void PageInput(Random& r, const World& w) {
  uint8_t* base = w.base;
  Mappers(r, w);
  for (uint32_t c = 0; c < 4; c++) {
    if (r.Below(3) != 0) {
      continue;
    }
    const uint32_t mapper = w.at(kMappersAt) + 0x200 * c;
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k, k == 2 ? uint32_t(r.Next()) : 0);
    }
  }
  const uint32_t menu = w.at(kMenuAt);
  guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
  guest::StoreU32(base, menu + menu_helpers::kFirst, 0);
  guest::StoreU32(base, menu + menu_helpers::kLast, 2);
  guest::StoreU32(base, menu + menu_helpers::kCursor, r.Below(3));
  guest::StoreU32(base, menu + menu_helpers::kPrevious, r.Below(3));
  for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                           menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
    guest::StoreU32(base, menu + o, r.Below(2));
  }
  guest::StoreU32(base, menu + menu_helpers::kMoved, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + menu_helpers::kSide, 0);
  SoundManager(r, w);
}

// The kind page's words: the mode (+9820) and the one before it (+9824) 0
// to 2 or now and then any, the history's depth (+9776) 0 to 11.
void KindWords(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  Machine(t, r, w);
  PageInput(r, w);
  const uint32_t m = w.machine();
  guest::StoreU32(base, m + 9820, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3));
  guest::StoreU32(base, m + 9824, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3));
  guest::StoreU8(base, m + 9776, uint8_t(r.Below(12)));
}

// The variant page's words: the choice (+10100) and its flag (+10104) 0 to
// 2, the cursor's row and its second pair word small or -1, the list's top.
void VariantWords(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  Machine(t, r, w);
  PageInput(r, w);
  const uint32_t m = w.machine();
  guest::StoreU32(base, m + 10100, r.Below(3));
  guest::StoreU32(base, m + 10104, r.Below(3));
  guest::StoreU32(base, m + 9828, r.Below(15));
  guest::StoreU32(base, m + 9972, r.Below(3));
  for (uint32_t k = 0; k < 15; k++) {
    guest::StoreU32(base, m + 9984 + 8 * k, SmallOrNone(r, 4));
  }
}

// The event page's words: the list's ends (+9840 first, +9844 last) within
// 0 to 14, the cursor (+9828) at an end, one past it, between them, -1 or
// 15 (the list's scroll past its ends), the
// old cursor (+9832) an end or any row, the visible row (+9972) and the old
// (+9976) 0 to 2 (now and then -1 or 3), the event count (+9848) and the
// cursor kept (+9716) around each other, the scroll flag (+9836) 0 to 3, the
// rows' badges (+9856 + 8 x row) 0 to 2 (now and then -1, 3 or any), the
// history's depth, player 0's save +0x1F870 0 or not. The scenes'
// animations (UiMovie::Play's, then Finish's, of the same index) the 8 of no
// tracks: each node's first animation entry (+116) 40 to 47, its entries 40
// to 63 (past the children the sub-scenes are found among) those.
void EventWords(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  Machine(t, r, w);
  PageInput(r, w);
  for (uint32_t n = 0; n < kNodeCount; n++) {
    guest::StoreU32(base, w.node(n) + 116, 40 + r.Below(8));
    for (uint32_t e = 40; e < 64; e++) {
      guest::StoreU32(base, w.node(n) + 128 + 8 * e,
                      w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
    }
  }
  const uint32_t m = w.machine();
  const uint32_t first = r.Below(15);
  const uint32_t last = r.Below(4) == 0 ? first : first + r.Below(15 - first);
  const uint32_t cursors[] = {first, last, first - 1, last + 1, first + r.Below(last - first + 1),
                              r.Below(15), 0xFFFFFFFFu, 15};
  const uint32_t cursor = cursors[r.Below(8)];
  guest::StoreU32(base, m + 9840, first);
  guest::StoreU32(base, m + 9844, last);
  guest::StoreU32(base, m + 9828, cursor);
  const uint32_t olds[] = {first, last, cursor, r.Below(15)};
  guest::StoreU32(base, m + 9832, olds[r.Below(4)]);
  auto row = [&] { return r.Below(6) == 0 ? (r.Below(2) == 0 ? 0xFFFFFFFFu : 3u) : r.Below(3); };
  guest::StoreU32(base, m + 9972, row());
  guest::StoreU32(base, m + 9976, row());
  const uint32_t kept = r.Below(16);
  guest::StoreU32(base, m + 9716, kept);
  guest::StoreU32(base, m + 9848, r.Below(3) == 0 ? kept : r.Below(16));
  guest::StoreU32(base, m + 9836, r.Below(4));
  // (Rows -1 and 15 read +9848 and +9976.)
  for (uint32_t k = 0; k < 15; k++) {
    const uint32_t badge = r.Below(6) == 0 ? (r.Below(2) == 0 ? uint32_t(r.Next()) : 3u)
                                           : r.Below(3);
    guest::StoreU32(base, m + 9856 + 8 * k, r.Below(8) == 0 ? 0xFFFFFFFFu : badge);
  }
  guest::StoreU8(base, m + 9776, uint8_t(r.Below(12)));
  guest::StoreU32(base, w.save + 0x1F870, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
}

void DrawPages(Context& h, const World& w) {
  // Each page's frame up to its register saves and its animation's frame
  // below it up to that one's (the callees' frames under them not).
  Run(h, w, {"RaceMenu::AnimateKindPage (sub_821898C0)", 200, __imp__sub_821898C0,
             hooks::RaceMenuAnimateKindPage, {{-128, -32}}, KindWords});
  Run(h, w, {"RaceMenu::KindPage (sub_8218AFD8)", 300, __imp__sub_8218AFD8,
             hooks::RaceMenuKindPage, {{-112, -24}}, KindWords});
  Run(h, w, {"RaceMenu::VariantPage (sub_8218B750)", 300, __imp__sub_8218B750,
             hooks::RaceMenuVariantPage, {{-112, -24}}, VariantWords});
  Run(h, w, {"RaceMenu::AnimateEventPage (sub_82189FE8)", 300, __imp__sub_82189FE8,
             hooks::RaceMenuAnimateEventPage, {{-128, -32}}, EventWords});
  // EventPage: SetText's parameter area (+48 to +79: r7 on, the original's
  // leftovers) left out.
  Run(h, w, {"RaceMenu::EventPage (sub_8218B2C8)", 400, __imp__sub_8218B2C8,
             hooks::RaceMenuEventPage, {{-160, -112}, {-80, -64}}, EventWords});
}

}  // namespace

void RaceMenu(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: race_menu: no block from the allocation layer");
    return;
  }
  Init(h, w);
  if (hooks::NativeHeapActive()) {
    Handlers(h, w);
    Pages(h, w);
    Start(h, w);
    Transitions(h, w);
    Update(h, w);
    DrawPages(h, w);
  }
}

}  // namespace harness
