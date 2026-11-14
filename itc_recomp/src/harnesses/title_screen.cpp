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
#include "race_screens/race_screens.h"
#include "title_screen/title_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_title_screen);
ITC_FLAG_DECLARE(bool, multiplayer_menu_items);

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
    {0x826D8478, 4},  // the options' settings (ModeType4::Interrupt's +712)
    {0x826BABA4, 20},  // the players' controllers and the fifth word
    {0x826C7938, 4},   // the profile read asked for
    {0x826D8454, 4},   // the controller pick's block (+52 its count)
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
        save(memory::Alloc(kSaveDataBytes, 0)), screen(memory::Alloc(kScreenBytes, 0)) {
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
  // The type 4 mode's block (+848) for the delegate functions' cases.
  static constexpr uint32_t kScreenBytes = 48224;
  uint32_t screen;

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
  if (address == ts::kBootDataLoad || address == 0x82180CE8 || address == 0x82116AF0) {
    // BootData::Load (TitleLoad::Start's), CarDealer::StartFirstRace and
    // Career::NextDay (CareerMenu's): recorded, r3 left (the harnesses run
    // before the boot's data is loaded, which the first would load; the
    // second sets up a race; the third went round in the harness's save).
    g_opens.insert(g_opens.end(), {address, ctx.r3.u32, ctx.r1.u32, uint32_t(ctx.lr)});
    return true;
  }
  if (address == 0x8211AD00 || address == 0x8210A8D0) {
    // The UI movies' start and Strings::Load (ModeType4::Init's): recorded,
    // r3 left. The start reads r3 only (its r4 a leftover).
    g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                   address == 0x8211AD00 ? 0u : ctx.r4.u32, ctx.r1.u32,
                                   uint32_t(ctx.lr)});
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

// The menus' input, as the race screens' harness shapes a list: the
// players' action mappers (each controller's entry 0 to 3, each entry's
// mapper one of ours, its slots 10 and 12 Pressed and Fired, now and then
// Held; blocked now and then; a few bits set), the menu list (7 items, its
// directions often the menus' own ids, its block of item words in the
// world), the front-end menu (items 0 to 1) and the sound manager.
constexpr uint32_t kHeldSlot = 0x8210BE70;
constexpr uint32_t kListBlockAt = 0x2D00 + 0x600;  // in the menus' world: 8 item words
void MenuInput(Random& r, const World& w) {
  namespace rs = race_screens;
  uint8_t* base = w.base;
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t mapper = w.at(kMappersAt) + 0x200 * c;
    const uint32_t vtable = w.at(kMapperVtablesAt) + 0x40 * c;
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, r.Below(5) == 0 ? kHeldSlot : rs::kPressedSlot);
    guest::StoreU32(base, vtable + 48, r.Below(5) == 0 ? kHeldSlot : rs::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  const uint32_t list = w.at(kListAt);
  const uint32_t count = r.Below(4) == 0 ? 2 + r.Below(2) : 7;
  guest::StoreU32(base, list + rs::kListCount, count);
  guest::StoreU32(base, list + rs::kListScroll, 0);
  guest::StoreU32(base, list + rs::kListShown, count);
  guest::StoreU32(base, list + rs::kListSelection, r.Below(count + 1));
  guest::StoreU32(base, list + rs::kListSelectionBefore, r.Below(8));
  guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? 1 : 0);
  const uint32_t ids[6] = {154, 153, 155, 156, 159, 158};
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(base, list + rs::kListDown + 4 * k,
                    r.Below(6) == 0 ? 151 + r.Below(30) : ids[k] + (k < 4 && r.Below(2) ? 14 : 0));
  }
  guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
  guest::StoreU32(base, list + rs::kListWraps, r.Below(2));
  guest::StoreU32(base, list + rs::kListBlock, w.at(kListBlockAt));
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, w.at(kListBlockAt) + 4 * i, r.Below(4) == 0 ? 0 : 1);
  }
  const uint32_t menu = w.at(kMenuAt);
  guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
  guest::StoreU32(base, menu + menu_helpers::kFirst, 0);
  guest::StoreU32(base, menu + menu_helpers::kLast, 1);
  guest::StoreU32(base, menu + menu_helpers::kCursor, r.Below(2));
  guest::StoreU32(base, menu + menu_helpers::kPrevious, r.Below(2));
  for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                           menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
    guest::StoreU32(base, menu + o, r.Below(2));
  }
  guest::StoreU32(base, menu + menu_helpers::kMoved, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + menu_helpers::kSide, 0);
  SoundManager(r, w);
}

// The title's words, the list's selections and moves, the menu's and player
// 0's save shaped for the transitions and the menus' set-ups; the list's
// block 0 (MenuList::Init makes one: compared and freed after).
void TransitionWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t words = w.at(kWordsAt), list = w.at(kListAt), menu = w.at(kMenuAt);
  guest::StoreU32(base, words + 4, r.Below(10));
  for (const uint32_t field : {8u, 12u, 28u, 40u}) {
    guest::StoreU32(base, words + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  guest::StoreU32(base, words + 20, r.Below(3));
  guest::StoreU32(base, list + 16, r.Below(8));
  guest::StoreU32(base, list + 20, r.Below(8));
  guest::StoreU32(base, list + 24, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(2));
  guest::StoreU32(base, list + 32, r.Below(2));
  guest::StoreU32(base, list + 68, 0);
  guest::StoreU32(base, menu + 0, r.Below(2));
  guest::StoreU32(base, menu + 4, r.Below(2));
  guest::StoreU32(base, menu + 8, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(2));
  guest::StoreU32(base, w.save + 6092, uint32_t(r.Next()));
  guest::StoreU32(base, w.save + 6100,
                  r.Below(3) == 0 ? 0 : r.Below(3) == 0 ? 995 + r.Below(10) : uint32_t(r.Next()));
  guest::StoreU16(base, w.save + 6328, r.Below(3) == 0 ? 0 : uint16_t(r.Next()));
}

// r3 the started word (the page object's +24): 0 mostly, 1 or any.
void Started(PPCContext& t, Random& r, const World& w) {
  TransitionWorld(r, w);
  const uint32_t at = w.at(kPagesAt) + 24;
  guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
  t.r3.u64 = at;
}

// The list's block MenuList::Init made (7 words at most) compared and freed.
void ListBlock(PPCContext&, const World& w, Outputs& out) {
  Allocated(w.base, out, w.at(kListAt) + 68, 28);
}

void Transitions(Context& h, const World& w) {
  struct Case {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    int32_t frame;
    int32_t saves;
  };
  const Case cases[] = {
      {"TitleScreen::Intro (sub_82222B60)", __imp__sub_82222B60, hooks::TitleScreenIntro, 112, 24},
      {"TitleScreen::PressStartLoop (sub_82222BF8)", __imp__sub_82222BF8,
       hooks::TitleScreenPressStartLoop, 112, 24},
      {"TitleScreen::Transition2 (sub_82222C90)", __imp__sub_82222C90,
       hooks::TitleScreenTransition2, 112, 24},
      {"TitleScreen::LeavePressStart (sub_82222D40)", __imp__sub_82222D40,
       hooks::TitleScreenLeavePressStart, 112, 24},
      {"TitleScreen::ShowMainMenu (sub_82223B78)", __imp__sub_82223B78,
       hooks::TitleScreenShowMainMenu, 128, 40},
      {"TitleScreen::ShowMainMenuAtOnce (sub_82223C38)", __imp__sub_82223C38,
       hooks::TitleScreenShowMainMenuAtOnce, 128, 40},
      {"TitleScreen::HighlightMain (sub_82222DD0)", __imp__sub_82222DD0,
       hooks::TitleScreenHighlightMain, 112, 24},
      {"TitleScreen::MoveMainCursor (sub_82222E88)", __imp__sub_82222E88,
       hooks::TitleScreenMoveMainCursor, 160, 64},
      {"TitleScreen::Transition9 (sub_82223050)", __imp__sub_82223050,
       hooks::TitleScreenTransition9, 160, 64},
      {"TitleScreen::Transition10 (sub_822231B0)", __imp__sub_822231B0,
       hooks::TitleScreenTransition10, 160, 64},
      {"TitleScreen::ShowCareerMenu (sub_82223D70)", __imp__sub_82223D70,
       hooks::TitleScreenShowCareerMenu, 144, 48},
      {"TitleScreen::HighlightCareer (sub_82223328)", __imp__sub_82223328,
       hooks::TitleScreenHighlightCareer, 128, 32},
      {"TitleScreen::MoveCareerCursor (sub_82223428)", __imp__sub_82223428,
       hooks::TitleScreenMoveCareerCursor, 144, 56},
      {"TitleScreen::BackFromCareer (sub_82223E58)", __imp__sub_82223E58,
       hooks::TitleScreenBackFromCareer, 128, 32},
      {"TitleScreen::ShowMenu4 (sub_82223F30)", __imp__sub_82223F30, hooks::TitleScreenShowMenu4,
       128, 32},
      {"TitleScreen::BackFromMenu4 (sub_82223FD8)", __imp__sub_82223FD8,
       hooks::TitleScreenBackFromMenu4, 128, 32},
      {"TitleScreen::MoveMenu4Cursor (sub_822235D0)", __imp__sub_822235D0,
       hooks::TitleScreenMoveMenu4Cursor, 144, 48},
  };
  // Each transition's own frame without its saves compared (the set-ups'
  // frames below them have their own cases).
  for (const Case& c : cases) {
    Run(h, w, {c.name, 150, c.recompiled, c.native, {{-c.frame, -c.saves}}, Started, ListBlock});
  }
  // The set-ups: their frames without their saves (and without SetText's
  // parameter area, +48 to +79: r7 on, the originals' leftovers).
  auto setup = [](PPCContext&, Random& r, const World& w) { TransitionWorld(r, w); };
  Run(h, w, {"TitleScreen::SetUpMainMenu (sub_82223700)", 150, __imp__sub_82223700,
             hooks::TitleScreenSetUpMainMenu, {{-144, -40}}, setup, ListBlock});
  Run(h, w, {"TitleScreen::SetUpCareerMenu (sub_82223840)", 150, __imp__sub_82223840,
             hooks::TitleScreenSetUpCareerMenu, {{-144, -96}, {-64, -40}}, setup, ListBlock});
  Run(h, w, {"TitleScreen::SetUpMenu4 (sub_82223940)", 60, __imp__sub_82223940,
             hooks::TitleScreenSetUpMenu4, {{-128, -80}, {-48, -32}}, setup, ListBlock});
}

void MenuPages(Context& h, const World& w) {
  // The pages: the menus' input (MenuInput), the title's words; their own
  // frames without their saves.
  auto input = [](PPCContext& t, Random& r, const World& w) {
    TransitionWorld(r, w);
    MenuInput(r, w);
    t.r3.u64 = w.mode();
  };
  // MainMenu with --multiplayer_menu_items on (the original's behavior), then
  // off: the bypass (the user's calls, 2026-11-14) answers items 3 (Xbox
  // Live) and 4 (split screen) as an input the menu ignores (no sound, no
  // change, r3 0) where the original leaves for their modes, so the cases
  // accepting either differ from the original.
  // Random input accepts item 3 or 4 rarely, so each setting also gets
  // cases that do: the list's seven items with the cursor on 3 or 4, no
  // move, every mapper bit set but the directions' ids out of the mappers'
  // range (no move from MenuList::Update), the accept id 159, no mapper
  // blocked.
  auto multiplayer_item = [input](PPCContext& t, Random& r, const World& w) {
    namespace rs = race_screens;
    input(t, r, w);
    uint8_t* base = w.base;
    for (uint32_t c = 0; c < 4; c++) {
      const uint32_t mapper = w.at(kMappersAt) + 0x200 * c;
      for (uint32_t k = 0; k < 6; k++) {
        guest::StoreU32(base, mapper + 448 + 4 * k, 0xFFFFFFFFu);
      }
      guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, 0);
    }
    const uint32_t list = w.at(kListAt);
    guest::StoreU32(base, list + rs::kListCount, 7);
    guest::StoreU32(base, list + rs::kListShown, 7);
    guest::StoreU32(base, list + rs::kListSelection, 3 + r.Below(2));
    guest::StoreU32(base, list + rs::kListMove, 0);
    for (uint32_t k = 0; k < 4; k++) {
      guest::StoreU32(base, list + rs::kListDown + 4 * k, 200);
    }
    guest::StoreU32(base, list + rs::kListDown + 16, 159);
  };
  const bool multiplayer = ITC_FLAG(multiplayer_menu_items);
  ITC_FLAG(multiplayer_menu_items) = true;
  Run(h, w, {"TitleScreen::MainMenu (sub_82222700)", 400, __imp__sub_82222700,
             hooks::TitleScreenMainMenu, {{-144, -32}}, input});
  Run(h, w, {"TitleScreen::MainMenu (sub_82222700), item 3 or 4 accepted", 100,
             __imp__sub_82222700, hooks::TitleScreenMainMenu, {{-144, -32}}, multiplayer_item});
  ITC_FLAG(multiplayer_menu_items) = false;
  Run(h, w,
      {"TitleScreen::MainMenu (sub_82222700), multiplayer items bypassed (expected to differ)",
       400, __imp__sub_82222700, hooks::TitleScreenMainMenu, {{-144, -32}}, input});
  Run(h, w,
      {"TitleScreen::MainMenu (sub_82222700), item 3 or 4 accepted, bypassed (expected to "
       "differ)",
       100, __imp__sub_82222700, hooks::TitleScreenMainMenu, {{-144, -32}}, multiplayer_item});
  ITC_FLAG(multiplayer_menu_items) = multiplayer;
  // CareerMenu: player 0's save a new game's (Progress::Clear run on it
  // first, for Progress::Restart's garage), its day shaped after.
  Run(h, w, {"TitleScreen::CareerMenu (sub_82221AC0)", 400, __imp__sub_82221AC0,
             hooks::TitleScreenCareerMenu, {{-128, -24}},
             [&h, input](PPCContext& t, Random& r, const World& w) {
               PPCContext c = CallContext(h);
               c.r3.u64 = w.save;
               __imp__sub_82113BD8(c, w.base);
               input(t, r, w);
               guest::StoreU32(w.base, w.save + 6100, r.Below(3) == 0 ? 0 : 1 + r.Below(30));
             }});
  Run(h, w, {"TitleScreen::Menu4 (sub_82222378)", 200, __imp__sub_82222378,
             hooks::TitleScreenMenu4, {{-112, -24}}, input});
}

// --- The mode: the mode the world's (its dispatcher without handlers, so
// GameMessages::Dispatch takes the queued messages and delivers none; its
// ring's count and start small), its block the world's screen block with
// the five machines' states and loaded words picked, the answer in the
// world. ---

constexpr uint32_t kAnswerAt = 0x3400;
constexpr uint32_t kMachineOffsets[5] = {28920, 38560, 0, 9640, 19280};

void ModeWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t mode = w.mode();
  WriteRandom(base, w.screen, World::kScreenBytes, r);
  guest::StoreU32(base, mode + 36 + 200, 0);  // no handlers
  guest::StoreU32(base, mode + 840, r.Below(4));
  guest::StoreU32(base, mode + 844, r.Below(200));
  guest::StoreU32(base, mode + 848, w.screen);
  static constexpr uint32_t kStates[] = {4, 5, 6, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1};
  for (const uint32_t at : kMachineOffsets) {
    guest::StoreU32(base, w.screen + at + 9604,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kStates[r.Below(std::size(kStates))]);
    guest::StoreU32(base, w.screen + at + 9608, r.Below(4) == 0 ? 0 : r.Below(3));
  }
  // Leaving's: now and then every machine in a leaving state.
  if (r.Below(3) == 0) {
    static constexpr uint32_t kLeft[] = {6, 11, 14, 12, 20};
    for (const uint32_t at : kMachineOffsets) {
      guest::StoreU32(base, w.screen + at + 9604, kLeft[r.Below(5)]);
    }
  }
  guest::StoreU32(base, w.screen + 48200, r.Below(5));
  guest::StoreU32(base, 0x826D8478, w.mode());
  guest::StoreU32(base, w.mode() + 712, r.Below(2));
}

// r3 the answer, r4 the mode.
void Delegate(PPCContext& t, Random& r, const World& w) {
  ModeWorld(r, w);
  t.r3.u64 = w.at(kAnswerAt);
  t.r4.u64 = w.mode();
}
// r3 the mode + 32 (r4 a number for ForceExit).
void Interface(PPCContext& t, Random& r, const World& w) {
  ModeWorld(r, w);
  t.r3.u64 = w.mode() + 32;
  t.r4.u64 = r.Below(6);
}
// r3 the block (now and then with high bits), r4 an index.
void BlockArgs(PPCContext& t, Random& r, const World& w) {
  ModeWorld(r, w);
  t.r3.u64 = w.screen | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = r.Below(4) == 0 ? uint64_t(r.Next()) : r.Below(7);
}
// The screen block compared.
void ScreenBlock(PPCContext&, const World& w, Outputs& out) {
  out.Bytes(Read(w.base, w.screen, World::kScreenBytes));
}

// The words of [at, at + bytes) that point into [lo, lo + span) made their
// offsets with a mark (the block VirtualAlloc made differs by side).
void NormalizePointers(uint8_t* base, uint32_t at, uint32_t bytes, uint32_t lo, uint32_t span) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, at + i);
    if (word >= lo && word < lo + span) {
      guest::StoreU32(base, at + i, 0xB10C0000u ^ (word - lo));
    }
  }
}

void Mode(Context& h, const World& w) {
  Run(h, w, {"ModeType4::MachineOf (sub_82224B40)", 60, __imp__sub_82224B40,
             hooks::ModeType4MachineOf, {}, BlockArgs});
  Run(h, w, {"ModeType4::AllLoaded (sub_82224BC0)", 60, __imp__sub_82224BC0,
             hooks::ModeType4AllLoaded, {}, BlockArgs});
  Run(h, w, {"ModeType4::FindShown (sub_82224C10)", 60, __imp__sub_82224C10,
             hooks::ModeType4FindShown, {{-96, -8}}, BlockArgs, ScreenBlock});
  Run(h, w, {"ModeType4::Restart (sub_82225098)", 100, __imp__sub_82225098,
             hooks::ModeType4Restart, {{-112, -16}}, Delegate, ScreenBlock});
  Run(h, w, {"ModeType4::NextMode (sub_82225390)", 200, __imp__sub_82225390,
             hooks::ModeType4NextMode, {{-144, -56}, {-240, -152}}, Delegate, ScreenBlock});
  Run(h, w, {"ModeType4::LeaveScreen (sub_822252C8)", 60, __imp__sub_822252C8,
             hooks::ModeType4LeaveScreen, {{-128, -32}, {-224, -136}}, Delegate, ScreenBlock});
  Run(h, w, {"ModeType4::WaitForLeave (sub_822251D0)", 100, __imp__sub_822251D0,
             hooks::ModeType4WaitForLeave, {{-112, -16}, {-208, -120}}, Delegate, ScreenBlock});
  Run(h, w, {"ModeType4::Leaving (sub_82225698)", 100, __imp__sub_82225698,
             hooks::ModeType4Leaving, {{-128, -32}}, Delegate, ScreenBlock});
  Run(h, w, {"ModeType4::NextModeAnswer (sub_82224B98)", 20, __imp__sub_82224B98,
             hooks::ModeType4NextModeAnswer, {}, Delegate});
  Run(h, w, {"ModeType4::Interrupt (sub_82224E20)", 100, __imp__sub_82224E20,
             hooks::ModeType4Interrupt, {{-96, -8}}, Interface});
  Run(h, w, {"ModeType4::ForceExit (sub_82224ED0)", 60, __imp__sub_82224ED0,
             hooks::ModeType4ForceExit, {{-32, -16}}, Interface, ScreenBlock});
  Run(h, w, {"ModeType4::Resume (sub_82225048)", 20, __imp__sub_82225048, hooks::ModeType4Resume,
             {}, Interface});
  // Init: the mode random words (its dispatcher's count 0 to 7); the UI
  // movies' start and Strings::Load recorded; the block VirtualAlloc made
  // compared by content and freed, its addresses in it, in the mode and in
  // r3 made offsets; its frame's +80 to +87 compared (the machines' Inits'
  // frames below hold the machines' addresses).
  Run(h, w, {"ModeType4::Init (sub_82224C70)", 12, __imp__sub_82224C70, hooks::ModeType4Init,
             {{-112 + 80, -112 + 88}},
             [](PPCContext& t, Random& r, const World& w) {
               guest::StoreU32(w.base, w.mode() + 36 + 200, r.Below(8));
               t.r3.u64 = w.mode();
             },
             [](PPCContext& t, const World& w, Outputs& out) {
               uint8_t* base = w.base;
               const uint32_t block = guest::LoadU32(base, w.mode() + 848);
               const bool made = memory::IsPages(block);
               out.Word(made ? 1 : 0);
               if (!made) {
                 return;
               }
               NormalizePointers(base, block, 48220, block, 48220);
               out.Bytes(Read(base, block, 48220));
               NormalizePointers(base, w.mode(), 1024, block, 48220);
               if (t.r3.u32 >= block && t.r3.u32 < block + 48220) {
                 t.r3.u64 = 0xB10C0000u ^ (t.r3.u32 - block);
               }
               memory::FreePages(block);
               guest::StoreU32(base, w.mode() + 848, 1);
             }});
}

// --- "Press start": the menus' input (the four players' mappers), the
// title's idle count below the attract race's 600 (SetUpAttractRace stays
// recompiled and needs the race data's world, which the harnesses, run
// before the boot's data load, don't have: its 600 is seen in every attract
// race), the controllers' words 0 to 4 or any. ---

void PressStart(Context& h, const World& w) {
  Run(h, w, {"Players::AssignController (sub_8209F6B8)", 100, __imp__sub_8209F6B8,
             hooks::PlayersAssignController, {},
             [](PPCContext& t, Random& r, const World& w) {
               for (uint32_t k = 0; k < 5; k++) {
                 guest::StoreU32(w.base, 0x826BABA4 + 4 * k,
                                 r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
               }
               t.r3.u64 = r.Below(4);
               t.r4.u64 = r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(5);
             }});
  Run(h, w, {"TitleScreen::OnControllerPicked (sub_822219E0)", 100, __imp__sub_822219E0,
             hooks::TitleScreenOnControllerPicked, {},
             [](PPCContext& t, Random& r, const World& w) {
               guest::StoreU32(w.base, 0x826D8454, w.at(kAnswerAt) + 0x40);
               guest::StoreU32(w.base, w.at(kAnswerAt) + 0x40 + 52,
                               r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(6));
               for (uint32_t k = 0; k < 5; k++) {
                 guest::StoreU32(w.base, 0x826BABA4 + 4 * k, r.Below(5));
               }
               Machine(t, r, w);
             }});
  // TitleScreen::Update (the step, without the wrap): the players' input
  // (MenuInput), the idle count 0 to 597 or negative; its frame without its
  // saves.
  Run(h, w, {"TitleScreen::Update (sub_822221A0)", 400, __imp__sub_822221A0,
             hooks::TitleScreenPressStart, {{-144, -56}},
             [](PPCContext& t, Random& r, const World& w) {
               TransitionWorld(r, w);
               MenuInput(r, w);
               guest::StoreU32(w.base, w.at(kWordsAt) + 36,
                               r.Below(4) == 0 ? uint32_t(-int32_t(r.Below(1000))) : r.Below(598));
               t.r3.u64 = w.mode();
               t.r4.u64 = w.machine();
             }});
}

// --- The title load's debug sequence: the pages over the menus' input,
// the steps with the block's tick count around their limits. ---

void Debug(Context& h, const World& w) {
  struct Case {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Case pages[] = {
      {"TitleLoad::DebugPage0 (sub_82220CD8)", __imp__sub_82220CD8, hooks::TitleLoadDebugPage<0>},
      {"TitleLoad::DebugPage1 (sub_82220D60)", __imp__sub_82220D60, hooks::TitleLoadDebugPage<1>},
      {"TitleLoad::DebugPage2 (sub_82220DE8)", __imp__sub_82220DE8, hooks::TitleLoadDebugPage<2>},
      {"TitleLoad::DebugPage3 (sub_82220E70)", __imp__sub_82220E70, hooks::TitleLoadDebugPage<3>},
  };
  for (const Case& c : pages) {
    Run(h, w, {c.name, 100, c.recompiled, c.native, {{-96, -8}},
               [](PPCContext& t, Random& r, const World& w) {
                 MenuInput(r, w);
                 t.r3.u64 = w.mode() | (uint64_t(r.Below(2)) << 32);
               }});
  }
  const Case steps[] = {
      {"TitleLoad::DebugStep0 (sub_822204F8)", __imp__sub_822204F8, hooks::TitleLoadDebugStep<0>},
      {"TitleLoad::DebugStep1 (sub_82220580)", __imp__sub_82220580, hooks::TitleLoadDebugStep<1>},
      {"TitleLoad::DebugStep2 (sub_82220610)", __imp__sub_82220610, hooks::TitleLoadDebugStep<2>},
      {"TitleLoad::DebugStep3 (sub_82220698)", __imp__sub_82220698, hooks::TitleLoadDebugStep<3>},
      {"TitleLoad::DebugStep4 (sub_82220720)", __imp__sub_82220720, hooks::TitleLoadDebugStep<4>},
      {"TitleLoad::DebugStep5 (sub_822207B0)", __imp__sub_822207B0, hooks::TitleLoadDebugStep<5>},
      {"TitleLoad::DebugStep6 (sub_82220840)", __imp__sub_82220840, hooks::TitleLoadDebugStep<6>},
      {"TitleLoad::DebugStep7 (sub_822208D0)", __imp__sub_822208D0, hooks::TitleLoadDebugStep<7>},
      {"TitleLoad::DebugStep8 (sub_82220960)", __imp__sub_82220960, hooks::TitleLoadDebugStep<8>},
      {"TitleLoad::DebugStep9 (sub_822209F0)", __imp__sub_822209F0, hooks::TitleLoadDebugStep<9>},
      {"TitleLoad::DebugStep10 (sub_82220A80)", __imp__sub_82220A80,
       hooks::TitleLoadDebugStep<10>},
      {"TitleLoad::DebugStep11 (sub_82220B10)", __imp__sub_82220B10,
       hooks::TitleLoadDebugStep<11>},
  };
  // Their frames without their saves (r28 or r29 on, or r31 for step 11).
  for (const Case& c : steps) {
    Run(h, w, {c.name, 80, c.recompiled, c.native, {{-128, -40}},
               [](PPCContext& t, Random& r, const World& w) {
                 static constexpr uint32_t kLimits[] = {10, 120, 180, 240};
                 const uint32_t load = w.at(kLoadBlockAt);
                 const uint32_t limit = kLimits[r.Below(4)];
                 guest::StoreU32(w.base, load + 4,
                                 r.Below(4) == 0 ? uint32_t(r.Next()) : limit - 2 + r.Below(4));
                 t.r3.u64 = load + 16;
               }});
  }
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
    Transitions(h, w);
    MenuPages(h, w);
    Mode(h, w);
    PressStart(h, w);
    Debug(h, w);
  }
}

}  // namespace harness
