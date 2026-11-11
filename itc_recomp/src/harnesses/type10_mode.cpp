// The type 10 mode, the replay theater (src/type10_mode/,
// docs/functions/type10-mode.md), against the recompiled functions, laid as
// the title screen's harness lays its world (title_screen.cpp). Each case
// fills a world of its own (one block from the allocation layer) with random
// words from the case's seed: the type 10 block (the theater's machine at
// +0, the save screen's at +9640), the mode with its message ring, the
// theater's three blocks, the save layer's header with up to 20 replay
// records, the save screen's data, movie 45 over a pool of 40 nodes (each
// child of a node another of the pool, all TXTs) and the movie table's pair
// naming it, the players' action mappers and the sound manager, with a
// string table of our own; points the globals into it (put back after);
// shapes the fields each function tests; runs the original (with
// --native_type10_mode off, so what it calls of the mode's is the original
// too) or the register steps (hooks/type10_mode.h); and compares the world,
// the globals, the function's frame without its register saves (and without
// the text setters' parameter area, +48 to +79, where their unset varargs
// land: the user's call of 2026-10-30), the caller's area above r1, r1 and
// r3. UiMovie::Run's update is stood in for (each call's return address,
// stack pointer and arguments kept, answered 0 or 1 from the case's
// generator); UiMovies::Open's and Delete's calls, the screen start's and
// Strings::Load's are recorded on both sides and answered 1; the blocks
// VirtualAlloc makes (the layer's pages) are compared by content and freed.
// The cursor's float is stored by both from the same integer and compared
// in the world's bytes.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/type10_mode.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "type10_mode/type10_mode.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_type10_mode);

namespace harness {

namespace {

namespace t10 = type10_mode;

// The block's layout.
constexpr uint32_t kTypeBlockAt = 0x0;       // the type 10 block (19,300 bytes)
constexpr uint32_t kModeAt = 0x4C00;         // the mode (852 bytes; its ring at +240)
constexpr uint32_t kPagesAt = 0x5000;        // [0x828161E0] (28 bytes)
constexpr uint32_t kListAt = 0x5040;         // [0x828161E4] (72 bytes)
constexpr uint32_t kDataAt = 0x50C0;         // [0x828161E8] (16 bytes)
constexpr uint32_t kHeaderAt = 0x5100;       // [0x828161F0] (24 bytes)
constexpr uint32_t kSaveDataAt = 0x5140;     // [0x826D8454] (64 bytes)
constexpr uint32_t kPairsAt = 0x5200;        // the movie table's pair
constexpr uint32_t kMenuWorldAt = 0x5300;    // the sound manager and its vtable (+0x400)
constexpr uint32_t kListBlockAt = 0x5900;    // the list's item words
constexpr uint32_t kAnswerAt = 0x5A00;       // the delegate functions' answer
constexpr uint32_t kRecordsAt = 0x5C00;      // the replays' 36-byte records
constexpr uint32_t kRecords = 20;
constexpr uint32_t kReplayWordAt = 0x6400;   // the kept replay's +0x2B600 (nonzero)
constexpr uint32_t kMovieAt = 0x6800;        // movie 45 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x10C00;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x1AC00;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the movie's named animations shaped
constexpr uint32_t kMappersAt = 0x1B000;     // 4 action mappers of 0x200
constexpr uint32_t kMapperVtablesAt = 0x1B800;  // their vtables (0x40 each)
constexpr uint32_t kBlockBytes = 0x1BA00;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kLoadingCurrent = 0x82815E64;  // LoadingScreen::Run(-1)'s movie (3: none)

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {t10::kPages, 12},  // the page object, the list and the data
    {t10::kSaveBuffers, 4},
    {t10::kSaveScreenData, 4},
    {t10::kEntered, 4},
    {t10::kPadBlocked0, 8},
    {t10::kKeptReplay, 4},
    {t10::kRaceData, 4},
    {t10::kRecordings, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kFades, 64},
    {kLoadingCurrent, 4},
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
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint32_t machine() const { return at(kTypeBlockAt); }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t dispatcher() const { return mode() + t10::kDispatcher; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  // A string table of our own ([0x826C5C34] pointed at it): its 18 files
  // all one container of 8,192 entries over 16 strings of letters, so any id
  // below 8,192 has a string; no argument codes (the varargs the native
  // callers pass as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;

 private:
  void MakeStrings() {
    Random r(0x5265706C6179ull);  // "Replay"
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

// The movie over the pool: its live scene node 0, the named animations over
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
  // The page object's words small or -1.
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, w.at(kPagesAt) + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(7));
  }
  // The data's words: +4 (what was asked) 0 to 4 or any, +8 and +12 0 or
  // not.
  const uint32_t data = w.at(kDataAt);
  guest::StoreU32(base, data + 4, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
  guest::StoreU32(base, data + 8, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, data + 12, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  // The mode's ring and the dispatcher's (the same ring, at +240): count
  // and start small; no handlers.
  guest::StoreU32(base, w.mode() + 240 + 600, r.Below(40));
  guest::StoreU32(base, w.mode() + 240 + 604, r.Below(60));
  guest::StoreU32(base, w.dispatcher() + 200, 0);
  // The nodes: all TXTs, each node's children the pool's (their index +108
  // small; the prompts' +100 small).
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(30));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  // The scenes' animations: the 8 of no tracks. Each node's first animation
  // entry (+116) 40 to 47, its entries 40 to 63 those (never a wild entry).
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
  MakeMovie(base, w, w.at(kMovieAt), r);
  // The root's own children none (+104 0).
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, w.at(kPairsAt), t10::kMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, w.at(kMovieAt));
  // The save layer's header: the records and their count (0 to 20).
  const uint32_t header = w.at(kHeaderAt);
  guest::StoreU32(base, header + 12, w.at(kRecordsAt));
  guest::StoreU32(base, header + 20, r.Below(4) == 0 ? r.Below(10) : r.Below(kRecords + 1));
  for (uint32_t k = 0; k < kRecords + 12; k++) {
    const uint32_t record = w.at(kRecordsAt) + 36 * k;
    guest::StoreU32(base, record + 16,
                    r.Below(3) == 0 ? uint32_t(r.Next()) : (r.Below(70) << 16) | r.Below(0x10000));
    guest::StoreU32(base, record + 20, r.Below(6));
    guest::StoreU32(base, record + 24, r.Below(70));
    guest::StoreU32(base, record + 28,
                    r.Below(4) == 0 ? 0 : r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(100));
    guest::StoreU32(base, record + 32, r.Below(8));
  }
  // The save screen's data: +56 0, 1 or any.
  guest::StoreU32(base, w.at(kSaveDataAt) + 56, r.Below(3) == 2 ? uint32_t(r.Next()) : r.Below(2));
  // The fades' words: their counts 0 or not.
  WriteRandom(base, kFades, 64, r);
  for (const uint32_t count : {t10::kFadeCount0, t10::kFadeCount1}) {
    guest::StoreU32(base, count, r.Below(2) == 0 ? 0 : r.Below(30));
  }
  // The pad blocked or not, how the title is entered.
  guest::StoreU32(base, t10::kPadBlocked0, r.Below(3) == 0 ? 1 : 0);
  guest::StoreU32(base, t10::kPadBlocked1, r.Below(3) == 0 ? 1 : 0);
  guest::StoreU32(base, t10::kEntered, r.Below(4));
  // The machines' words: +9604 a state, +9608, +9612, +9616 0 or 1 or any;
  // the shown index 0 or 1.
  static constexpr uint32_t kStates[] = {1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 20};
  for (const uint32_t at : {0u, t10::kSaveScreenAt}) {
    const uint32_t machine = w.machine() + at;
    guest::StoreU32(base, machine + t10::kMachineState,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kStates[r.Below(std::size(kStates))]);
    for (const uint32_t field :
         {t10::kMachineLoaded, t10::kMachineReenter, t10::kMachineDelete}) {
      guest::StoreU32(base, machine + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
    }
  }
  guest::StoreU32(base, w.machine() + t10::kShown, r.Below(2));
  // The kept replay's word Replay::SetUpRace tests (nonzero: it returns at
  // once).
  guest::StoreU32(base, w.at(kReplayWordAt), 1 + r.Below(1000));
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, t10::kPages, w.at(kPagesAt));
  guest::StoreU32(base, t10::kList, w.at(kListAt));
  guest::StoreU32(base, t10::kData, w.at(kDataAt));
  guest::StoreU32(base, t10::kSaveBuffers, w.at(kHeaderAt));
  guest::StoreU32(base, t10::kSaveScreenData, w.at(kSaveDataAt));
  guest::StoreU32(base, t10::kKeptReplay, w.at(kReplayWordAt) - 0x2B600);
  guest::StoreU32(base, t10::kRaceData, w.at(kAnswerAt) + 0x100);
  guest::StoreU32(base, t10::kRecordings, w.at(kAnswerAt) + 0x140);
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie, ours).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
  guest::StoreU32(base, kLoadingCurrent, 3);
}

// UiMovies::Open's and Delete's calls, the screen start's and
// Strings::Load's, recorded on both sides and answered 1 (Delete would free
// our movie). Other addresses go to the recorder set before the run, when
// there's one.
std::vector<uint32_t> g_opens;
hooks::StartUpRecorder g_chained = nullptr;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address == t10::kScreenStart || address == t10::kStringsLoad) {
    // The start reads r3 only (its r4 a leftover).
    g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                   address == t10::kScreenStart ? 0u : ctx.r4.u32, ctx.r1.u32,
                                   uint32_t(ctx.lr)});
    return true;
  }
  if (address != t10::kMoviesOpen && address != t10::kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == t10::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
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

// The text records the setters made (stored in the movie's map): their
// words and buffers compared, freed, their map words 0.
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

// The sound manager ([0x826C5C80], in the menus' world): PlayCue2D (slot 10,
// Sound::Play's) the getter.
void SoundManager(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t manager = w.at(kMenuWorldAt), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60,
                  r.Below(2) == 0 ? uint32_t(r.Next()) & ~0xFFu : uint32_t(r.Next()));
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
        const bool native = ITC_FLAG(native_type10_mode);
        ITC_FLAG(native_type10_mode) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_type10_mode) = native;
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
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
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

// The menus' input, as the title screen's harness shapes it: the players'
// action mappers (each controller's entry 0 to 3, each entry's mapper one
// of ours, its slots 10 and 12 Pressed and Fired, now and then Held;
// blocked now and then; a few bits set) and the menu list over the
// replays (its directions often the menus' own ids, its block of item words
// in the world), and the sound manager.
constexpr uint32_t kHeldSlot = 0x8210BE70;
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
  const uint32_t count = 1 + r.Below(8);
  guest::StoreU32(base, list + rs::kListCount, count);
  guest::StoreU32(base, list + rs::kListScroll, 0);
  guest::StoreU32(base, list + rs::kListScrollBefore, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, list + rs::kListShown, count);
  guest::StoreU32(base, list + rs::kListSelection, r.Below(count));
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
  SoundManager(r, w);
}

// The list shaped for the transitions and the list's helpers: the
// selection, previous selection and scroll within the replays (the records
// read stay in the world), its move word -1, 0 or 1, wrapped or not; its
// block 0 (MenuList::Init makes one: compared and freed after).
void ListWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t list = w.at(kListAt);
  const uint32_t count = guest::LoadU32(base, w.at(kHeaderAt) + 20);
  const uint32_t scroll = count > 9 && r.Below(3) != 0 ? r.Below(count - 8) : 0;
  guest::StoreU32(base, list + 4, scroll);
  guest::StoreU32(base, list + 8, r.Below(2) == 0 ? scroll : r.Below(count + 1));
  guest::StoreU32(base, list + 16, r.Below(10));
  guest::StoreU32(base, list + 20, r.Below(10));
  guest::StoreU32(base, list + 24, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(2));
  guest::StoreU32(base, list + 32, r.Below(2));
  guest::StoreU32(base, list + 68, 0);
}

// r3 the machine (now and then with high bits), r4 the dispatcher.
void Machine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.machine() | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = w.dispatcher();
}
// r3 the dispatcher.
void Dispatcher(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.dispatcher(); }
// r3 the started word (the page object's +24): 0 mostly, 1 or any.
void Started(PPCContext& t, Random& r, const World& w) {
  ListWorld(r, w);
  const uint32_t at = w.at(kPagesAt) + 24;
  guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
  t.r3.u64 = at;
}
// The list's block MenuList::Init made (20 words at most) compared and freed.
void ListBlock(PPCContext&, const World& w, Outputs& out) {
  Allocated(w.base, out, w.at(kListAt) + 68, 80);
}

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

void Theater(Context& h, const World& w) {
  // InitStates: its frame but its saves (r27 on) compared.
  Run(h, w, {"ReplayTheater::InitStates (sub_8228AC48)", 24, __imp__sub_8228AC48,
             hooks::ReplayTheaterInitStates, {{-1296, -48}}, InitMachine});
  // Create: the three blocks VirtualAlloc made compared by content.
  Run(h, w, {"ReplayTheater::Create (sub_82289408)", 60, __imp__sub_82289408,
             hooks::ReplayTheaterCreate, {{-112, -24}}, Machine,
             [](PPCContext& t, const World& w, Outputs& out) {
               // r3 the data's address (the last VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(w.base, t10::kData) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(w.base, out, t10::kPages, 28);
               Allocated(w.base, out, t10::kList, 72);
               Allocated(w.base, out, t10::kData, 16);
             }});
  // Destroy: the three blocks the layer's pages (whether each was freed
  // compared).
  Run(h, w, {"ReplayTheater::Destroy (sub_822894E8)", 60, __imp__sub_822894E8,
             hooks::ReplayTheaterDestroy, {{-128, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               const uint32_t data = MakePages(base, 16, r);
               guest::StoreU32(base, data + 8, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               guest::StoreU32(base, t10::kPages, MakePages(base, 28, r));
               guest::StoreU32(base, t10::kList, MakePages(base, 72, r));
               guest::StoreU32(base, t10::kData, data);
             }});
  Run(h, w, {"ReplayTheater::OpenMovie (sub_822891F8)", 20, __imp__sub_822891F8,
             hooks::ReplayTheaterOpenMovie, {}, Machine});
  Run(h, w, {"ReplayTheater::WaitForMovie (sub_82289570)", 100, __imp__sub_82289570,
             hooks::ReplayTheaterWaitForMovie, {{-96, -16}}, Machine});
  // Start: SaveBuffers::Create finds the header there (nothing made); its
  // frame, StartPage's and SetPrompts' below without their saves.
  Run(h, w, {"ReplayTheater::Start (sub_8228AA58; sub_8228A508, sub_8228A3B8)", 100,
             __imp__sub_8228AA58, hooks::ReplayTheaterStart,
             {{-96, -8}, {-192, -104}, {-368, -248}}, Machine});
  Run(h, w, {"ReplayTheater::StartPage (sub_8228A508; sub_8228A3B8)", 60, __imp__sub_8228A508,
             hooks::ReplayTheaterStartPage, {{-96, -8}, {-272, -152}}, Machine});
  // Draw: the page -1 or 1 to 5 (0, ListPage, has its own cases); only its
  // own frame compared.
  Run(h, w, {"ReplayTheater::Draw (sub_8228A868)", 150, __imp__sub_8228A868,
             hooks::ReplayTheaterDraw, {{-112, -8}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               const uint32_t pick = r.Below(6);
               guest::StoreU32(w.base, w.at(kPagesAt), pick == 0 ? 0xFFFFFFFFu : pick);
             }});
  // Update: the running transition -1, another negative, or 0 to 6, the
  // queued one -1 or small, the started word 0 or not, the page 3 or not;
  // only its own frame compared.
  Run(h, w, {"ReplayTheater::Update (sub_8228A8F8)", 400, __imp__sub_8228A8F8,
             hooks::ReplayTheaterUpdate, {{-144, -16}},
             [](PPCContext&, Random& r, const World& w) {
               uint8_t* base = w.base;
               ListWorld(r, w);
               const uint32_t pages = w.at(kPagesAt);
               const uint32_t pick = r.Below(4);
               guest::StoreU32(base, pages + 12,
                               pick == 0   ? 0xFFFFFFFFu
                               : pick == 1 ? uint32_t(-int32_t(2 + r.Below(5)))
                                           : r.Below(7));
               guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(6));
               guest::StoreU32(base, pages + 24, r.Below(3) == 0 ? 1 : 0);
               guest::StoreU32(base, pages, r.Below(3) == 0 ? 3 : r.Below(6));
             }});
  // The save screen's replies: what was asked 0 to 4 or any (set by the
  // world), interrupted or not; OnDone's and OnRetry's BuildList with the
  // list's block made after.
  Run(h, w, {"ReplayTheater::OnDone (sub_8228AAB0)", 200, __imp__sub_8228AAB0,
             hooks::ReplayTheaterOnDone, {{-96, -8}},
             [](PPCContext& t, Random& r, const World& w) {
               ListWorld(r, w);
               Machine(t, r, w);
             },
             ListBlock});
  Run(h, w, {"ReplayTheater::OnFailed (sub_82289208)", 150, __imp__sub_82289208,
             hooks::ReplayTheaterOnFailed, {}, Machine});
  Run(h, w, {"ReplayTheater::OnRetry (sub_8228ABE0)", 100, __imp__sub_8228ABE0,
             hooks::ReplayTheaterOnRetry, {{-96, -8}},
             [](PPCContext& t, Random& r, const World& w) {
               ListWorld(r, w);
               Machine(t, r, w);
             },
             ListBlock});
  Run(h, w, {"ReplayTheater::OnBack (sub_82289308)", 60, __imp__sub_82289308,
             hooks::ReplayTheaterOnBack, {}, Machine});
  Run(h, w, {"ReplayTheater::OnInterrupt (sub_82289358)", 60, __imp__sub_82289358,
             hooks::ReplayTheaterOnInterrupt, {}, Machine});
  Run(h, w, {"ReplayTheater::OnResume (sub_822893A8)", 60, __imp__sub_822893A8,
             hooks::ReplayTheaterOnResume, {}, Machine});
  Run(h, w, {"ReplayTheater::Unload (sub_82289600)", 60, __imp__sub_82289600,
             hooks::ReplayTheaterUnload, {{-112, -16}}, Machine});
  Run(h, w, {"ReplayTheater::Wait (sub_82289668)", 40, __imp__sub_82289668,
             hooks::ReplayTheaterWait, {}, Machine});
  // Leave: the save layer's header and its buffers the layer's pages or 0
  // (whether each was freed compared).
  Run(h, w, {"ReplayTheater::Leave (sub_822896C0)", 60, __imp__sub_822896C0,
             hooks::ReplayTheaterLeave, {{-112, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               const uint32_t header = MakePages(base, 24, r);
               for (const uint32_t field : {0u, 4u, 8u, 12u}) {
                 guest::StoreU32(base, header + field,
                                 r.Below(3) == 0 ? 0 : MakePages(base, 64, r));
               }
               guest::StoreU32(base, t10::kSaveBuffers, header);
             }});
}

void Pages(Context& h, const World& w) {
  // ListPage: the menus' input; its frame without its saves (r25 on).
  Run(h, w, {"ReplayTheater::ListPage (sub_82289768)", 400, __imp__sub_82289768,
             hooks::ReplayTheaterListPage, {{-144, -64}},
             [](PPCContext& t, Random& r, const World& w) {
               MenuInput(r, w);
               Dispatcher(t, r, w);
             }});
  Run(h, w, {"ReplayTheater::AskList (sub_82289958)", 60, __imp__sub_82289958,
             hooks::ReplayTheaterAskList, {}, Dispatcher});
  Run(h, w, {"ReplayTheater::AskLoad (sub_822899E0)", 60, __imp__sub_822899E0,
             hooks::ReplayTheaterAskLoad, {}, Dispatcher});
  Run(h, w, {"ReplayTheater::LeavePage (sub_82289A70)", 40, __imp__sub_82289A70,
             hooks::ReplayTheaterLeavePage, {}, Dispatcher});
  // WaitPage: the save screen's +56 0, 1 or any; SetPrompts' frame (its
  // tail call's) without its saves.
  Run(h, w, {"ReplayTheater::WaitPage (sub_8228A4B0; sub_8228A3B8)", 100, __imp__sub_8228A4B0,
             hooks::ReplayTheaterWaitPage, {{-176, -56}}, Dispatcher});
  // SetPrompts: each pair's values -1 or small.
  Run(h, w, {"ReplayTheater::SetPrompts (sub_8228A3B8)", 150, __imp__sub_8228A3B8,
             hooks::ReplayTheaterSetPrompts, {{-176, -56}},
             [](PPCContext& t, Random& r, const World&) {
               for (PPCRegister* reg : {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7, &t.r8}) {
                 reg->u64 = r.Below(3) == 0 ? 0xFFFFFFFFull : r.Below(4);
               }
             }});
}

void Transitions(Context& h, const World& w) {
  struct Case {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    std::vector<Range> stack;
  };
  const Case cases[] = {
      {"ReplayTheater::Intro (sub_82289AC0)", __imp__sub_82289AC0, hooks::ReplayTheaterIntro,
       {{-112, -24}}},
      {"ReplayTheater::FadeOut (sub_82289BD0)", __imp__sub_82289BD0, hooks::ReplayTheaterFadeOut,
       {{-112, -24}}},
      {"ReplayTheater::FadeInList (sub_82289CC0)", __imp__sub_82289CC0,
       hooks::ReplayTheaterFadeInList, {{-112, -24}}},
      {"ReplayTheater::ShowCursor (sub_82289D70)", __imp__sub_82289D70,
       hooks::ReplayTheaterShowCursor, {{-128, -32}}},
      // MoveCursor: without SetNodeText's parameter area and its saves (r23
      // on, f31); ShowRecord's and ShowListPage's frames have their own
      // cases.
      {"ReplayTheater::MoveCursor (sub_8228A658)", __imp__sub_8228A658,
       hooks::ReplayTheaterMoveCursor, {{-176, -128}, {-96, -88}}},
      {"ReplayTheater::HideCursor (sub_82289E08)", __imp__sub_82289E08,
       hooks::ReplayTheaterHideCursor, {{-112, -24}}},
      {"ReplayTheater::PickReplay (sub_82289EB8)", __imp__sub_82289EB8,
       hooks::ReplayTheaterPickReplay, {{-112, -24}}},
  };
  for (const Case& c : cases) {
    Run(h, w, {c.name, 200, c.recompiled, c.native, c.stack, Started, ListBlock});
  }
  auto list = [](PPCContext&, Random& r, const World& w) { ListWorld(r, w); };
  // BuildList: its frame without its saves (r29 on, f31).
  Run(h, w, {"ReplayTheater::BuildList (sub_8228A560)", 200, __imp__sub_8228A560,
             hooks::ReplayTheaterBuildList, {{-144, -40}}, list, ListBlock});
  // ShowListPage: without the setters' parameter area and its saves (r23
  // on).
  Run(h, w, {"ReplayTheater::ShowListPage (sub_82289F48)", 200, __imp__sub_82289F48,
             hooks::ReplayTheaterShowListPage, {{-176, -128}, {-96, -80}}, list});
  // ShowRecord: r3 a record (now and then 0), its +28 0 now and then; its
  // frame without the setters' parameter area and its saves (r28 on).
  Run(h, w, {"ReplayTheater::ShowRecord (sub_8228A0D8)", 300, __imp__sub_8228A0D8,
             hooks::ReplayTheaterShowRecord, {{-192, -144}, {-112, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = r.Below(8) == 0 ? 0 : w.at(kRecordsAt) + 36 * r.Below(kRecords);
             }});
}

// --- The mode: the mode the world's (its dispatcher without handlers, so
// GameMessages::Dispatch takes the queued messages and delivers none), its
// block the world's type 10 block with the two machines' states and loaded
// words picked, the answer in the world. ---

void ModeWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t mode = w.mode();
  guest::StoreU32(base, mode + t10::kBlock, w.machine());
  guest::StoreU32(base, mode + 840, r.Below(4));
  guest::StoreU32(base, mode + 844, r.Below(200));
  // Leaving's: now and then both machines in a leaving state.
  if (r.Below(3) == 0) {
    static constexpr uint32_t kLeft[] = {6, 11, 14, 12, 20};
    for (const uint32_t at : {0u, t10::kSaveScreenAt}) {
      guest::StoreU32(base, w.machine() + at + t10::kMachineState, kLeft[r.Below(5)]);
    }
  }
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
  Run(h, w, {"ModeType10::FindShown (sub_822888F0)", 60, __imp__sub_822888F0,
             hooks::ModeType10FindShown, {},
             [](PPCContext& t, Random& r, const World& w) {
               ModeWorld(r, w);
               t.r3.u64 = w.machine() | (uint64_t(r.Below(2)) << 32);
             }});
  Run(h, w, {"ModeType10::WaitForLoads (sub_82288B70)", 100, __imp__sub_82288B70,
             hooks::ModeType10WaitForLoads, {{-112, -16}}, Delegate});
  Run(h, w, {"ModeType10::NextMode (sub_82288E68)", 300, __imp__sub_82288E68,
             hooks::ModeType10NextMode, {{-128, -40}}, Delegate});
  Run(h, w, {"ModeType10::LeaveScreen (sub_82288DA0)", 60, __imp__sub_82288DA0,
             hooks::ModeType10LeaveScreen, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType10::WaitForShown (sub_82288CA0)", 100, __imp__sub_82288CA0,
             hooks::ModeType10WaitForShown, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType10::Leaving (sub_822890D8)", 100, __imp__sub_822890D8,
             hooks::ModeType10Leaving, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType9::NextModeAnswer (sub_8225D638)", 20, __imp__sub_8225D638,
             hooks::ModeType10NextModeAnswer, {}, Delegate});
  Run(h, w, {"ModeType10::Interrupt (sub_82288A68)", 150, __imp__sub_82288A68,
             hooks::ModeType10Interrupt, {{-96, -16}}, Interface});
  Run(h, w, {"ModeType9::ForceExit (sub_8225D828)", 60, __imp__sub_8225D828,
             hooks::ModeType10ForceExit, {{-16, -8}}, Interface});
  Run(h, w, {"ModeType10::Resume (sub_82288B20)", 20, __imp__sub_82288B20,
             hooks::ModeType10Resume, {}, Interface});
  Run(h, w, {"ReplayTheater::OnList (sub_822893E8)", 20, __imp__sub_822893E8,
             hooks::ReplayTheaterOnList, {}, Interface});
  // Init: the mode random words (its dispatcher's count 0 to 7); the UI
  // movies' start and Strings::Load recorded; the block VirtualAlloc made
  // compared by content and freed, its addresses in it and in the mode made
  // offsets; its frame's +80 to +87 compared (the machines' Inits' frames
  // below hold the machines' addresses).
  Run(h, w, {"ModeType10::Init (sub_82288940)", 12, __imp__sub_82288940, hooks::ModeType10Init,
             {{-112 + 80, -112 + 88}},
             [](PPCContext& t, Random& r, const World& w) {
               guest::StoreU32(w.base, w.dispatcher() + 200, r.Below(8));
               t.r3.u64 = w.mode();
             },
             [](PPCContext&, const World& w, Outputs& out) {
               uint8_t* base = w.base;
               const uint32_t block = guest::LoadU32(base, w.mode() + t10::kBlock);
               const bool made = memory::IsPages(block);
               out.Word(made ? 1 : 0);
               if (!made) {
                 return;
               }
               NormalizePointers(base, block, t10::kBlockBytes, block, t10::kBlockBytes);
               out.Bytes(Read(base, block, t10::kBlockBytes));
               NormalizePointers(base, w.mode(), 1024, block, t10::kBlockBytes);
               memory::FreePages(block);
               guest::StoreU32(base, w.mode() + t10::kBlock, 1);
             }});
}

}  // namespace

void Type10Mode(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type10_mode: no block from the allocation layer");
    return;
  }
  if (hooks::NativeHeapActive()) {
    Theater(h, w);
    Pages(h, w);
    Transitions(h, w);
    Mode(h, w);
  }
}

}  // namespace harness
