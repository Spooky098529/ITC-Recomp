// The user info screen (src/user_info/, docs/functions/user-info.md) against
// the recompiled functions, laid as the race menu's harness lays its world
// (race_menu.cpp). Each case fills a world of its own (one block from the
// allocation layer) with random words from the case's seed: the screen's
// machine, the mode with its message ring, the page object, the menu, the
// data, movie 10 over a pool of 40 nodes (each child of a node another of
// the pool, all TXTs; each node's first animation entry and the entries
// after the children the 8 animations of no tracks) and the movie table's
// pair naming it, the players' action mappers and the sound manager, with
// player 0's save data and a string table of our own; points the globals
// into it (put back after); shapes the fields each function tests; runs the
// original (with --native_user_info off, so what it calls of the screen's
// is the original too) or the register steps (hooks/user_info.h); and
// compares the block, the save data, the globals, the function's frame
// without its register saves (and without the words the varargs
// UiMovie::SetText calls store from r7 on: the originals' leftovers, 0 in
// ours, the user's call for the parking area, 2026-10-30), the caller's
// area above r1, r1 and r3. UiMovie::Run's update is stood in for (each
// call's return address, stack pointer and arguments kept, answered 0 or 1
// from the case's generator); UiMovies::Open's and Delete's calls are
// recorded on both sides and answered 1; the blocks VirtualAlloc makes (the
// layer's pages) are compared by content and freed; the text records the
// setters make are compared and freed. Nothing here is a float but the
// SetText doubles' frame words, which are copies of what the original
// passes (compared exactly: the same bits on both sides).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/ui_movies.h"
#include "hooks/user_info.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "ui_movies/game.h"
#include "user_info/user_info.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_user_info);

namespace harness {

namespace {

namespace ui = user_info;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;         // the machine (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;         // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2A00;        // [0x826D84C4] (28 bytes)
constexpr uint32_t kMenuAt = 0x2A40;         // [0x826D84C8] (68 bytes)
constexpr uint32_t kDataAt = 0x2AC0;         // [0x826D84CC] (60 bytes)
constexpr uint32_t kPairsAt = 0x2B00;        // the movie table's pairs
constexpr uint32_t kMenuWorldAt = 0x2C00;    // the sound manager and its vtable
constexpr uint32_t kMovieAt = 0x3400;        // movie 10 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0xD600;        // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x17600;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the movie's named animations shaped
constexpr uint32_t kMappersAt = 0x17A00;     // 4 action mappers of 0x200
constexpr uint32_t kMapperVtablesAt = 0x18200;  // their vtables (0x40 each)
constexpr uint32_t kBlockBytes = 0x18400;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kSaveSlots = 0x826C5B58;  // the players' save data
constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kFrameCount = 0x826C5B90;  // PlayTime::Add's frame count

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ui::kPages, 12},  // the page object, the menu and the data
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kSaveSlots, 16},
    {kFrameCount, 4},
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
  uint32_t data() const { return at(kDataAt); }
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
    Random r(0x55736572ull);  // "User"
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
    guest::StoreU32(base, w.at(kPagesAt) + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(2));
  }
  // The data's next state (+4) 21 or small.
  guest::StoreU32(base, w.data() + 4, r.Below(2) == 0 ? 21 : r.Below(12));
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.mode() + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.mode() + 204 + 604, r.Below(60));
  // The nodes: all TXTs, each node's children the pool's (their index +108
  // small).
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(10));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  // The scenes' animations (UiMovie::Play's, of an index added to the
  // scene's first): the 8 of no tracks. Each node's first animation entry
  // (+116) 40 to 47, its entries 40 to 63 (past the children the texts'
  // nodes are found among) those, as the race menu's harness has them.
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
  // Movie 10: its live scene node 0, the named animations over the root;
  // its loaded flag set or not.
  const uint32_t movie = w.at(kMovieAt);
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
  // The root's own children none (+104 0); FindNode reads the root's table,
  // not its count.
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, w.at(kPairsAt), ui::kMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  // Player 0's save data: random words.
  WriteRandom(base, w.save, kSaveDataBytes, r);
  guest::StoreU32(base, kFrameCount, uint32_t(r.Next()));
  // The machine's words: +9608, +9612 0 or 1 or any.
  for (const uint32_t field : {ui::kMachineLoaded, ui::kMachineReenter}) {
    guest::StoreU32(base, w.machine() + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, ui::kPages, w.at(kPagesAt));
  guest::StoreU32(base, ui::kMenu, w.at(kMenuAt));
  guest::StoreU32(base, ui::kData, w.data());
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
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
  if (address != ui::kMoviesOpen && address != ui::kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == ui::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
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
        const bool native = ITC_FLAG(native_user_info);
        ITC_FLAG(native_user_info) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_user_info) = native;
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

// The sound manager ([0x826C5C80]): its PlayCue2D slot (Sound::Play's) the
// getter sub_82090770.
void SoundManager(const World& w) {
  uint8_t* base = w.base;
  const uint32_t manager = w.at(kMenuWorldAt), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x82090770);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// The page's input, as the race menu's harness shapes it: the players'
// action mappers (each controller's entry 0 to 3, each entry's mapper one
// of ours, its slots 10 and 12 ActionMapper::Pressed and Fired, now and then
// Held; blocked now and then; a few bits set, now and then only pressed
// ones, so the menu doesn't move and 158 and 164 are read), the menu (items
// 0 to 7, as Create makes it) and the sound manager.
void PageInput(Random& r, const World& w) {
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
    const bool pressed_only = r.Below(3) == 0;
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      pressed_only ? (k == 2 ? uint32_t(r.Next()) : 0)
                                   : uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  const uint32_t menu = w.at(kMenuAt);
  guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
  guest::StoreU32(base, menu + menu_helpers::kFirst, 0);
  guest::StoreU32(base, menu + menu_helpers::kLast, 7);
  guest::StoreU32(base, menu + menu_helpers::kCursor, r.Below(8));
  guest::StoreU32(base, menu + menu_helpers::kPrevious, r.Below(8));
  for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                           menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
    guest::StoreU32(base, menu + o, r.Below(2));
  }
  guest::StoreU32(base, menu + menu_helpers::kMoved, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + menu_helpers::kSide, 0);
  SoundManager(w);
}

// A transition's started word (the page object's +24): 0, 1 or any.
void Started(PPCContext& t, Random& r, const World& w) {
  const uint32_t at = w.at(kPagesAt) + 24;
  guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
  t.r3.u64 = at;
}

void InitStates(Context& h, const World& w) {
  // The machine empty or holding random transitions with keys of small
  // numbers, some the Init's (Add replaces those); its frame but its saves
  // (r27 on) compared.
  Run(h, w, {"UserInfo::InitStates (sub_8220B408)", 24, __imp__sub_8220B408,
             hooks::UserInfoInitStates, {{-912, -48}},
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
  Run(h, w, {"UserInfo::Create (sub_8220AA50)", 60, __imp__sub_8220AA50, hooks::UserInfoCreate,
             {{-112, -24}}, Machine,
             [&h](PPCContext& t, const World&, Outputs& out) {
               // r3 the data's address (the last VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(h.base, ui::kData) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(h.base, out, ui::kPages, 28);
               Allocated(h.base, out, ui::kMenu, 68);
               Allocated(h.base, out, ui::kData, 60);
             }});
  // Destroy: the three blocks the layer's pages (whether each was freed
  // compared).
  Run(h, w, {"UserInfo::Destroy (sub_8220A9D8)", 60, __imp__sub_8220A9D8,
             hooks::UserInfoDestroy, {{-112, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               guest::StoreU32(base, ui::kPages, MakePages(base, 28, r));
               guest::StoreU32(base, ui::kMenu, MakePages(base, 68, r));
               guest::StoreU32(base, ui::kData, MakePages(base, 60, r));
             }});
  Run(h, w, {"UserInfo::OpenMovie (sub_8220AA40)", 20, __imp__sub_8220AA40,
             hooks::UserInfoOpenMovie, {}, Machine});
  Run(h, w, {"UserInfo::WaitForLoad (sub_8220B070)", 100, __imp__sub_8220B070,
             hooks::UserInfoWaitForLoad, {{-96, -16}}, Machine});
  // Reenter: the page 0 or not.
  Run(h, w, {"UserInfo::Reenter (sub_8220AB60)", 100, __imp__sub_8220AB60,
             hooks::UserInfoReenter, {},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), 0);
               }
             }});
  Run(h, w, {"UserInfo::Close (sub_8220ABB8)", 60, __imp__sub_8220ABB8, hooks::UserInfoClose,
             {{-112, -16}}, Machine});
}

void Pages(Context& h, const World& w) {
  // SetPlayTimeText: the hours (the data's +44 after PlayTime::Add) up to
  // and past 9999 by the save's play time (+48 + 6032). SetText's parameter
  // area (+48 to +79) left out.
  auto time = [](Random& r, const World& w) {
    guest::StoreU32(w.base, w.save + 48 + 6032,
                    r.Below(2) == 0 ? 9990 + r.Below(20) : uint32_t(r.Next()));
  };
  Run(h, w, {"UserInfo::SetPlayTimeText (sub_8220AFC0)", 100, __imp__sub_8220AFC0,
             hooks::UserInfoSetPlayTimeText, {{-112, -64}, {-32, -16}},
             [time](PPCContext&, Random& r, const World& w) { time(r, w); }});
  // MenuPage: the menu's input (PageInput); SetPlayTimeText's frame under
  // its own (without SetText's parameter area and its r31 save).
  Run(h, w, {"UserInfo::MenuPage (sub_8220B0D8; sub_8220AFC0)", 300, __imp__sub_8220B0D8,
             hooks::UserInfoMenuPage, {{-112, -24}, {-224, -176}, {-144, -128}},
             [time](PPCContext& t, Random& r, const World& w) {
               PageInput(r, w);
               time(r, w);
               t.r3.u64 = w.mode();
             }});
  // UpdatePage: the page -1, 0 or 1 (MenuPage, the empty function), r4 the
  // mode; only its own frame compared.
  Run(h, w, {"UserInfo::UpdatePage (sub_8220B260)", 200, __imp__sub_8220B260,
             hooks::UserInfoUpdatePage, {{-96, -8}},
             [time](PPCContext& t, Random& r, const World& w) {
               PageInput(r, w);
               time(r, w);
               Machine(t, r, w);
               guest::StoreU32(w.base, w.at(kPagesAt), uint32_t(int32_t(r.Below(3)) - 1));
             }});
}

void Transitions(Context& h, const World& w) {
  // SetTexts: its frame without SetText's parameter area (+48 to +79 but
  // the last call's r7 and r8, +48 to +63) and the saves (f31, r29 on).
  Run(h, w, {"UserInfo::SetTexts (sub_8220AD08)", 100, __imp__sub_8220AD08,
             hooks::UserInfoSetTexts, {{-192, -128}, {-112, -40}},
             [](PPCContext&, Random&, const World&) {}});
  // Opening: SetTexts' frame under its own.
  Run(h, w, {"UserInfo::Opening (sub_8220B1C8; sub_8220AD08)", 100, __imp__sub_8220B1C8,
             hooks::UserInfoOpening, {{-112, -24}, {-304, -240}, {-224, -152}}, Started});
  Run(h, w, {"UserInfo::Closing (sub_8220AC60)", 100, __imp__sub_8220AC60,
             hooks::UserInfoClosing, {{-112, -24}}, Started});
  // Update: the running transition -1, 0, 1 or another negative, the queued
  // one -1 or 0 to 1, the started word 0 or not, the data's +4 21 or a
  // state; only its own frame compared (the transitions' frames below it
  // hold the recompiled ones' register saves).
  Run(h, w, {"UserInfo::Update (sub_8220B2C0)", 300, __imp__sub_8220B2C0, hooks::UserInfoUpdate,
             {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               const uint32_t pages = w.at(kPagesAt);
               const uint32_t pick = r.Below(5);
               guest::StoreU32(base, pages + 12,
                               pick == 0   ? 0xFFFFFFFFu
                               : pick == 1 ? uint32_t(-int32_t(2 + r.Below(5)))
                                           : r.Below(2));
               guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(2));
               guest::StoreU32(base, pages + 24, r.Below(3) == 0 ? 1 : 0);
             }});
}

}  // namespace

void UserInfo(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: user_info: no block from the allocation layer");
    return;
  }
  InitStates(h, w);
  if (hooks::NativeHeapActive()) {
    Handlers(h, w);
    Pages(h, w);
    Transitions(h, w);
  }
}

}  // namespace harness
