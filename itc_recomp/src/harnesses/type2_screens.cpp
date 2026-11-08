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
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/type2_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
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
constexpr uint32_t kBlockBytes = 0x18400;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kRandSeed = 0x82638188;  // the game's rand

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {t2::kPlatePages, 12},  // the plate editor's page object, list and data
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
bool RecordOpen(PPCContext& ctx, uint32_t address) {
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
  }
}

}  // namespace harness
