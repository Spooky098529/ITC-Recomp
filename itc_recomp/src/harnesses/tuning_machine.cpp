// The tuning screen's machine (src/tuning_machine/,
// docs/functions/tuning-machine.md) against the recompiled functions, laid
// as the garage screen's harness lays its world (garage_screen.cpp). Each
// case fills a world of its own (one block from the allocation layer) with
// random words from the case's seed: the screen (the machine itself), the
// mode with its message ring, one movie (ids 2, 13, 14 and 16 naming it; a
// pool of 40 nodes, its named animations over 8 animations of no tracks,
// its +28 texture table) and the movie table's pairs; points the globals
// into it (put back after); shapes the fields each function tests; runs the
// original (with --native_tuning_machine off) or the register steps
// (hooks/tuning_machine.h); and compares the block, the function's frame
// without its register saves, the caller's area above r1, r1 and r3.
// UiMovies::Open's and Delete's calls and UiMovie::SetText's are recorded
// on both sides (r3 to r6, r1 and lr; SetText's r7 to r10 are the
// original's leftovers, 0 in ours, the user's call for the parking area,
// 2026-10-30) and answered 1 (SetText the movie's texts' map); the blocks
// VirtualAlloc made are compared by content and freed; the blocks
// VirtualFree is given are the layer's pages (whether each was freed
// compared). Nothing here is a float but Create's two constants, copied as
// words (compared exactly).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/tuning_machine.h"
#include "memory/heap.h"
#include "tuning_machine/tuning_machine.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_tuning_machine);

namespace harness {

namespace {

namespace tm = tuning_machine;

// The block's layout.
constexpr uint32_t kScreenAt = 0x0;          // the screen (0x9400 bytes)
constexpr uint32_t kModeAt = 0x9400;         // the mode (its ring at +204)
constexpr uint32_t kPairsAt = 0x9800;        // the movie table's pairs
constexpr uint32_t kTexturesAt = 0x9900;     // the movie's +28 table (200 words)
constexpr uint32_t kSlotsAt = 0x9C40;        // the textures' entries (200 of 8 bytes)
constexpr uint32_t kMovieAt = 0xA400;        // the movie (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x14800;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x1E800;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // the named animations shaped
constexpr uint32_t kBlockBytes = 0x1EC00;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
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
  uint32_t screen() const { return at(kScreenAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
};

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.at(kModeAt) + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.at(kModeAt) + 204 + 604, r.Below(60));
  // The movie: its live scene node 0, each node's children the pool's, all
  // TXTs; the named animations over the root, each one of the 8 animations
  // of no tracks; its +28 texture table naming the textures' entries; its
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
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
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
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, movie + 28, w.at(kTexturesAt));
  for (uint32_t k = 0; k < 200; k++) {
    guest::StoreU32(base, w.at(kTexturesAt) + 4 * k, w.at(kSlotsAt) + 8 * r.Below(200));
  }
  static constexpr uint32_t kIds[] = {2, 13, 14, 16};
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, w.at(kPairsAt) + 8 * k, kIds[k]);
    guest::StoreU32(base, w.at(kPairsAt) + 8 * k + 4, movie);
  }
  // The machine's words: +9608, +9612, +9616 0 or 1 or any; the state
  // +9652 11 often.
  const uint32_t s = w.screen();
  for (const uint32_t field : {tm::kMachineLoaded, tm::kMachineReenter, tm::kMachineDelete}) {
    guest::StoreU32(base, s + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  guest::StoreU32(base, s + 9652, r.Below(2) == 0 ? 11 : r.Below(63));
  // The decal images unloaded (no entry in state 2, no kept texture), so
  // DecalImages::UnloadAll frees nothing.
  const uint32_t images = s + tm::kImages;
  for (uint32_t i = 0; i < 200; i++) {
    if (guest::LoadU32(base, images + 36 * i + 8) == 2) {
      guest::StoreU32(base, images + 36 * i + 8, r.Below(2));
    }
    guest::StoreU32(base, images + 7236 + 8 * i, 0);
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pairs not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 4);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
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

// UiMovies::Open's and Delete's and UiMovie::SetText's calls, recorded on
// both sides: Open and Delete answered 1 (Delete would free our movie),
// SetText the movie's texts' map (its r3).
std::vector<uint32_t> g_calls;
uint32_t g_case_sp = 0;
bool Record(PPCContext& ctx, uint32_t address) {
  if (address != tm::kMoviesOpen && address != tm::kMoviesDelete && address != tm::kSetText) {
    return false;
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  const bool delete_call = address == tm::kMoviesDelete;
  const bool set_text = address == tm::kSetText;
  g_calls.insert(g_calls.end(),
                 {address, ctx.r3.u32, delete_call ? 0u : ctx.r4.u32,
                  set_text ? ctx.r5.u32 : 0u, set_text ? ctx.r6.u32 : 0u, g_case_sp - ctx.r1.u32,
                  uint32_t(ctx.lr)});
  ctx.r3.u64 = set_text ? ctx.r3.u64 + ui_movies::kMovieTexts : 1;
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
      g_calls.clear();
      g_case_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = Record;
      if (v == 0) {
        const bool native = ITC_FLAG(native_tuning_machine);
        ITC_FLAG(native_tuning_machine) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_tuning_machine) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      out[v].Word(uint32_t(g_calls.size()));
      for (const uint32_t word : g_calls) {
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

// r3 the screen, with a random high word now and then.
void Screen(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | w.screen();
}

// Init: the machine empty or holding random transitions with keys of small
// numbers (states below 12, messages below 52), so some match the Init's
// (Add replaces those); its frame (but its saves, r27 on) compared.
void Init(Context& h, const World& w) {
  Run(h, w, {"TuningMachine::Init (sub_821E7568)", 24, __imp__sub_821E7568,
             hooks::TuningMachineInit, {{-1232, -48}},
             [](PPCContext& t, Random& r, const World& w) {
               namespace gm = game_modes;
               const uint32_t machine = w.screen();
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
  // ZeroRecord16 and DecalImages::Reset: leaves (r4 a record in the
  // screen; r3 the decal images).
  Run(h, w, {"TuningMachine::ZeroRecord16 (sub_821FE540)", 20, __imp__sub_821FE540,
             hooks::TuningMachineZeroRecord16, {},
             [](PPCContext& t, Random& r, const World& w) {
               t.r4.u64 = w.screen() + 30800 + 4 * r.Below(16);
             }});
  Run(h, w, {"DecalImages::Reset (sub_821FE590)", 20, __imp__sub_821FE590,
             hooks::TuningMachineResetImages, {},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = (r.Below(4) == 0 ? (r.Next() << 32) : 0) | (w.screen() + tm::kImages);
             }});
  // Create: the three blocks VirtualAlloc made compared by content and
  // freed; its saves (r26 on) left out.
  Run(h, w, {"TuningMachine::Create (sub_821CE4C8; sub_821FE540, sub_821FE590)", 60,
             __imp__sub_821CE4C8, hooks::TuningMachineCreate, {{-144, -56}}, Screen,
             [&h](PPCContext&, const World& w, Outputs& out) {
               const uint32_t s = w.screen();
               Allocated(h.base, out, s + tm::kQueue, 28);
               Allocated(h.base, out, s + tm::kMenu, 68);
               Allocated(h.base, out, s + tm::kMenu2, 68);
             }});
  Run(h, w, {"TuningMachine::OpenMovies (sub_821CBDB0)", 20, __imp__sub_821CBDB0,
             hooks::TuningMachineOpenMovies, {{-96, -16}}, Screen});
  // WaitForMovies: the movie loaded or not (above; one movie stands for
  // the three).
  Run(h, w, {"TuningMachine::WaitForMovies (sub_821CE9E8)", 100, __imp__sub_821CE9E8,
             hooks::TuningMachineWaitForMovies, {{-112, -24}}, Screen});
  Run(h, w, {"TuningMachine::OnDialogClosed (sub_821D8898)", 100, __imp__sub_821D8898,
             hooks::TuningMachineOnDialogClosed, {{-128, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               Screen(t, r, w);
               t.r4.u64 = w.at(kModeAt);
             }});
  Run(h, w, {"TuningMachine::Unload (sub_821D69A0)", 60, __imp__sub_821D69A0,
             hooks::TuningMachineUnload, {{-112, -16}}, Screen});
  Run(h, w, {"TuningMachine::Close (sub_821D6A78)", 60, __imp__sub_821D6A78,
             hooks::TuningMachineClose, {},
             [](PPCContext& t, Random& r, const World& w) {
               Screen(t, r, w);
               t.r4.u64 = w.at(kModeAt);
             }});
  // Destroy: the queue and the two menus the layer's pages (whether each
  // was freed compared); the decal images unloaded (above). Its saves and
  // UnloadAll's (and its callees') below its frame left out.
  Run(h, w, {"TuningMachine::Destroy (sub_821CBD40)", 60, __imp__sub_821CBD40,
             hooks::TuningMachineDestroy, {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               const uint32_t s = w.screen();
               guest::StoreU32(base, s + tm::kQueue, MakePages(base, 28, r));
               guest::StoreU32(base, s + tm::kMenu, MakePages(base, 68, r));
               guest::StoreU32(base, s + tm::kMenu2, MakePages(base, 68, r));
               Screen(t, r, w);
             }});
  // SavePlaceholders (with SavePlaceholders14) and SavePlaceholders14 alone:
  // r3 the decal images.
  const auto images = [](PPCContext& t, Random&, const World& w) {
    t.r3.u64 = w.screen() + tm::kImages;
  };
  Run(h, w, {"DecalImages::SavePlaceholders (sub_82208130; sub_821F8950)", 40,
             __imp__sub_82208130, hooks::TuningMachineSavePlaceholders, {{-112, -16}},
             images});
  Run(h, w, {"DecalImages::SavePlaceholders14 (sub_821F8950)", 40, __imp__sub_821F8950,
             hooks::TuningMachineSavePlaceholders14, {{-112, -16}}, images});
}

}  // namespace

void TuningMachine(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_machine: no block from the allocation layer");
    return;
  }
  Init(h, w);
  if (hooks::NativeHeapActive()) {
    Handlers(h, w);
  }
}

}  // namespace harness
