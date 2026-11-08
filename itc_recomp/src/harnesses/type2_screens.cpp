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
// made-up time (time_hooks::SetFakeNow) while a case runs.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/type2_screens.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "type2_screens/type2_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
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
constexpr uint32_t kBlockBytes = 0x17800;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
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
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u) {}
  ~World() { memory::Free(allocation); }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t data() const { return at(kDataAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
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
    guest::StoreU32(base, node + 100, r.Below(60));
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
      time_hooks::ClearFakeNow();
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

}  // namespace

void Type2Screens(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: type2_screens: no block from the allocation layer");
    return;
  }
  PlateEditor(h, w);
}

}  // namespace harness
