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
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/tuning_machine.h"
#include "memory/heap.h"
#include "save_data/save_data.h"
#include "tuning_machine/tuning_machine.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <utility>
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
    {car_specs::kParams, 4},
    {save_data::kSlots, 4},
};

// Player 0's save data (the screen's +18904; its garage list at +6120, the
// selected car's index at +0x19920) and the car specs' object (+12 the 72
// specs, a spare 0x400 past them for the paint entries read past the last).
constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kSpecCount = 72;
constexpr uint32_t kSpecBytes = 8432;
constexpr uint32_t kSpecsBytes = 0x40 + kSpecCount * kSpecBytes + 0x400;
// The parameters' tables Car::Refresh, GetCarParts and SetAeroAvailability
// read: the nine aero part tables (each its stride per car, a spare car
// before the first and after the last) and 8 wheels of 48 bytes.
constexpr uint32_t kPartStrides[9] = {280, 280, 280, 280, 168, 168, 84, 108, 72};
constexpr uint32_t kPartsBytes = 1720 * (kSpecCount + 2) + 8 * 48;

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), save(memory::Alloc(kSaveDataBytes, 0)),
        specs(memory::Alloc(kSpecsBytes, 0)), parts(memory::Alloc(kPartsBytes, 0)) {
    if (specs != 0 && parts != 0) {
      // The car specs' object: +12 the 72 specs (random words once); +8 the
      // wheels, +16 to +48 the aero part tables (random words once).
      Random r(0x5370656373ull);  // "Specs"
      WriteRandom(base, specs, kSpecsBytes, r);
      WriteRandom(base, parts, kPartsBytes, r);
      guest::StoreU32(base, specs + 12, specs + 0x40);
      uint32_t at = parts;
      for (uint32_t k = 0; k < 9; k++) {
        guest::StoreU32(base, specs + 16 + 4 * k, at + kPartStrides[k]);
        at += kPartStrides[k] * (kSpecCount + 2);
      }
      guest::StoreU32(base, specs + 8, at);
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(save);
    memory::Free(specs);
    memory::Free(parts);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && save != 0 && specs != 0 && parts != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t screen() const { return at(kScreenAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  uint32_t save;
  uint32_t specs;
  uint32_t parts;
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

// TuningScreen::SetUp's callees: player 0's save with its garage shaped as
// the garage screen's harness shapes it (50 nodes, `count` linked from the
// head), the selected car one of them; the selected car's record random,
// its tested words over every branch, in and out of range.
void ShapeSave(uint8_t* base, const World& w, Random& r) {
  const uint32_t garage = w.save + career::kGarage;
  WriteRandom(base, garage, 212 + 50 * 1968, r);
  uint32_t order[50];
  for (uint32_t i = 0; i < 50; i++) {
    order[i] = i;
  }
  for (uint32_t i = 49; i > 0; i--) {
    std::swap(order[i], order[r.Below(i + 1)]);
  }
  const uint32_t count = 1 + r.Below(12);
  const auto link = [&](uint32_t head_at, uint32_t from, uint32_t to) {
    guest::StoreU16(base, garage + head_at, uint16_t(from < to ? order[from] : 0xFFFF));
    for (uint32_t i = from; i < to; i++) {
      guest::StoreU16(base, garage + 4 * order[i], uint16_t(i + 1 < to ? order[i + 1] : 0xFFFF));
      guest::StoreU16(base, garage + 4 * order[i] + 2, uint16_t(r.Below(50)));
    }
  };
  link(204, 0, count);
  link(200, count, 50);
  guest::StoreU16(base, garage + 208, uint16_t(count));
  const uint32_t selected = r.Below(count);
  guest::StoreU32(base, w.save + career::kSelectedCar, selected);
  // The selected car's record (c = the slot's +212).
  uint32_t node = garage + 4 * order[0];
  for (uint32_t k = 0; k < selected; k++) {
    node = garage + 4 * order[k + 1];
  }
  const uint32_t c = garage + 1968 * guest::LoadU16(base, node + 2) + 212;
  auto pick = [&](uint32_t below) -> uint32_t {
    switch (r.Below(8)) {
      case 0: return uint32_t(r.Next());
      case 1: return 0xFFFFFFFFu - r.Below(3);
      default: return r.Below(below);
    }
  };
  static constexpr uint32_t kIds[] = {8, 10, 4, 7, 12, 14, 0, 71};
  guest::StoreU32(base, c + 12, r.Below(3) == 0 ? kIds[r.Below(8)] : r.Below(kSpecCount));
  // The paint: -2 (the car's own) often, else an entry of the spec's
  // (-4 to 15).
  guest::StoreU16(base, c + 0,
                  uint16_t(r.Below(3) == 0 ? 0xFFFE : uint16_t(int16_t(r.Below(20)) - 4)));
  for (const uint32_t field : {1656u, 1664u, 1672u, 1668u, 1676u, 1680u, 1684u, 1688u, 1696u,
                               1700u, 1708u, 1712u, 1720u, 1728u, 1732u, 1736u, 1740u, 1744u,
                               1748u, 1752u, 1760u, 1768u, 1772u, 1776u, 1780u, 1784u, 1792u,
                               1796u, 1808u, 1812u}) {
    guest::StoreU32(base, c + field, pick(8));
  }
  guest::StoreU32(base, c + 1692, pick(8));
  guest::StoreU32(base, c + 1704, r.Below(3) == 0 ? 0 : pick(4));
  guest::StoreU32(base, c + 1716, r.Below(3) == 0 ? 0 : pick(4));
  guest::StoreU32(base, c + 1724, r.Below(3) == 0 ? 0 : pick(4));
  guest::StoreU32(base, c + 1756, uint32_t(int32_t(r.Below(15)) - 2) ^ (r.Below(8) == 0 ? 0x10000u : 0));
  guest::StoreU32(base, c + 1764, pick(4));
  guest::StoreU32(base, c + 1800, pick(7));
  guest::StoreU32(base, c + 1804, pick(6));
  guest::StoreU32(base, c + 1816, pick(6));
  guest::StoreU32(base, c + 1820, pick(7));
  // The vinyl layers: kinds 0, 1, 2 to 11 or others; their angles' floats
  // small, any word now and then (NaNs, infinities, denormals).
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t e = c + 1588 + 20 * i;
    guest::StoreU32(base, e + 0, r.Below(4) == 0 ? pick(14) : r.Below(3));
    for (uint32_t k = 8; k <= 16; k += 4) {
      switch (r.Below(6)) {
        case 0: guest::StoreU32(base, e + k, uint32_t(r.Next())); break;
        case 1: {
          static constexpr uint32_t kOdd[] = {0x7FC00000u, 0xFFC00001u, 0x7F800000u, 0xFF800000u,
                                              0x00000001u, 0x80400000u, 0x4F000000u, 0xCF000001u};
          guest::StoreU32(base, e + k, kOdd[r.Below(8)]);
          break;
        }
        default: guest::StoreFloat(base, e + k, float(int32_t(r.Below(2001)) - 1000) / 997.0f);
      }
    }
  }
  // The mirror table's index (the first layer's +4): 0 to 7, now and then
  // below (the frame's words under the table; past +108 are the register
  // saves).
  guest::StoreU32(base, c + 1592,
                  r.Below(5) == 0 ? uint32_t(-int32_t(r.Below(40))) : r.Below(8));
  guest::StoreU8(base, c + 1568, uint8_t(r.Next()));
  // The sticker layers: kinds 1, 2 or others, units 0 to 3, their +8 0 to 8
  // (the table's index + 1; 5 to 8 the mirror table's words), now and then
  // below.
  for (uint32_t i = 0; i < 5; i++) {
    const uint32_t e = c + 1824 + 12 * i;
    guest::StoreU32(base, e + 0, r.Below(5) == 0 ? pick(4) : 1 + r.Below(2));
    guest::StoreU32(base, e + 4, r.Below(5) == 0 ? pick(5) : r.Below(3));
    guest::StoreU32(base, e + 8,
                    r.Below(6) == 0 ? uint32_t(-int32_t(r.Below(40))) : r.Below(9));
  }
  // The screen: its save, the part set (0 to 4 in play), +25172 0 or 1 or
  // any.
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 18904, w.save);
  guest::StoreU32(base, s + 33900, r.Below(6) == 0 ? r.Below(200) : r.Below(5));
  guest::StoreU32(base, s + 25172, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, car_specs::kParams, w.specs);
}

void SetUpCallees(Context& h, const World& w) {
  // The save's garage compared (neither writes it).
  const auto after = [&h](PPCContext&, const World& w, Outputs& out) {
    out.Bytes(Read(h.base, w.save + career::kGarage, 212 + 50 * 1968));
    out.Word(guest::LoadU32(h.base, w.save + career::kSelectedCar));
  };
  Run(h, w, {"TuningScreen::LoadInteriorSlots (sub_821DE280)", 60, __imp__sub_821DE280,
             hooks::TuningMachineLoadInteriorSlots, {},
             [](PPCContext& t, Random& r, const World& w) {
               ShapeSave(w.base, w, r);
               Screen(t, r, w);
             },
             after});
  // LoadCurrentChoices: its frame but its saves (r23 on, -80) compared;
  // the vinyl angles' floats truncated to words (compared exactly).
  Run(h, w, {"TuningScreen::LoadCurrentChoices (sub_821DA4B0; sub_820C9E98)", 200,
             __imp__sub_821DA4B0, hooks::TuningMachineLoadCurrentChoices, {{-192, -80}},
             [](PPCContext& t, Random& r, const World& w) {
               ShapeSave(w.base, w, r);
               Screen(t, r, w);
             },
             after});
}

// A bitset of `words` words at `at`: all clear, all set, a few bits, many
// or random, by `mode` (5: each word its own mode).
void FillBits(uint8_t* base, uint32_t at, uint32_t words, uint32_t mode, Random& r) {
  for (uint32_t k = 0; k < words; k++) {
    const uint32_t m = mode == 5 ? r.Below(5) : mode;
    uint32_t word = 0;
    switch (m) {
      case 0: word = 0; break;
      case 1: word = 0xFFFFFFFFu; break;
      case 2: word = r.Below(3) == 0 ? 1u << r.Below(32) : 0; break;
      case 3: word = uint32_t(r.Next()) | uint32_t(r.Next()); break;
      default: word = uint32_t(r.Next()); break;
    }
    guest::StoreU32(base, at + 4 * k, word);
  }
}

// LoadOwnedParts: the save shaped as for the other callees, the selected
// car's +1904 bitset (13 words, bits 0 to 409) and the save's +0x1996C (14
// words, bits 95 to 438) with few or many bits, the wheels' bits (189 to
// 367) now and then all clear (r3 then the tires' leftover); the part set
// [+33900] 0 to 4, now and then out of range; [+25172] 0, 1 or any. Its
// frame but its saves (r26 on, -56) compared; the save compared.
void OwnedParts(Context& h, const World& w) {
  Run(h, w, {"TuningScreen::LoadOwnedParts (sub_821DBAB0; sub_821CDF98)", 240,
             __imp__sub_821DBAB0, hooks::TuningMachineLoadOwnedParts, {{-160, -56}},
             [](PPCContext& t, Random& r, const World& w) {
               ShapeSave(w.base, w, r);
               Screen(t, r, w);
               const uint32_t s = w.screen();
               const uint32_t car = tuning_screen::SelectedGarageCar(w.base, s) + 212 + 1904;
               FillBits(w.base, car, 13, r.Below(6), r);
               const uint32_t bits = w.save + 0x1996C;
               FillBits(w.base, bits, 14, r.Below(6), r);
               if (r.Below(4) == 0) {
                 FillBits(w.base, bits + 5 * 4, 7, 0, r);  // bits 160 to 383
               }
               uint32_t set = r.Below(5);
               switch (r.Below(6)) {
                 case 0: set = 5 + r.Below(20); break;
                 case 1: set = uint32_t(r.Next()); break;
                 default: break;
               }
               guest::StoreU32(w.base, s + 33900, set);
               guest::StoreU32(w.base, s + 25172,
                               r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
             },
             [&h](PPCContext&, const World& w, Outputs& out) {
               out.Bytes(Read(h.base, w.save, kSaveDataBytes));
             }});
}

// SetPrices: the save shaped as for the other callees, the selected car's
// price (its +1900) 0, small, large, negative, near the int limits, past
// single's exact integers or any word; the part set [+33900] 0 to 4, now
// and then out of range (-847 to 98 keep its rows in the screen; -334 puts
// a 28-byte row's first word on +25172, read after); [+25172] 0, 1 or any.
// Its words below sp but its saves (r17 on, -128) compared, the save too.
// The prices are words truncated by fctiwz, compared exactly.
void Prices(Context& h, const World& w) {
  Run(h, w, {"TuningScreen::SetPrices (sub_821DAD58)", 240, __imp__sub_821DAD58,
             hooks::TuningMachineSetPrices, {{-256, -128}},
             [](PPCContext& t, Random& r, const World& w) {
               ShapeSave(w.base, w, r);
               Screen(t, r, w);
               const uint32_t s = w.screen();
               uint32_t price = 0;
               switch (r.Below(9)) {
                 case 0: price = 0; break;
                 case 1: price = r.Below(100); break;
                 case 2: price = 1000 * r.Below(200); break;
                 case 3: price = r.Below(3000000); break;
                 case 4: price = (1u << 24) + r.Below(1u << 30); break;
                 case 5: price = uint32_t(-int32_t(r.Below(5000000))); break;
                 case 6: price = 0x7FFFFFFFu - r.Below(200); break;
                 case 7: price = 0x80000000u + r.Below(200); break;
                 default: price = uint32_t(r.Next()); break;
               }
               guest::StoreU32(w.base, tuning_screen::SelectedGarageCar(w.base, s) + 2112, price);
               uint32_t set = r.Below(5);
               switch (r.Below(8)) {
                 case 0: set = 5 + r.Below(94); break;
                 case 1: set = uint32_t(-1 - int32_t(r.Below(847))); break;
                 case 2: set = uint32_t(-334); break;
                 default: break;
               }
               guest::StoreU32(w.base, s + 33900, set);
               guest::StoreU32(w.base, s + 25172,
                               r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
             },
             [&h](PPCContext&, const World& w, Outputs& out) {
               out.Bytes(Read(h.base, w.save, kSaveDataBytes));
             }});
}

// LoadUnlockedParts: the screen's +18904 at the save, its unlocked bitset
// (+0x19930, 7 words: bits 0 to 223, the function reads 0 to 188) clear,
// full, sparse, dense, random or each word its own; the part set [+33900]
// 0 to 4, now and then out of range (nothing written then). A leaf without
// a frame: the words below sp compared all the same, the save too.
void UnlockedParts(Context& h, const World& w) {
  Run(h, w, {"TuningScreen::LoadUnlockedParts (sub_821CFFC8)", 240, __imp__sub_821CFFC8,
             hooks::TuningMachineLoadUnlockedParts, {{-256, 0}},
             [](PPCContext& t, Random& r, const World& w) {
               Screen(t, r, w);
               const uint32_t s = w.screen();
               guest::StoreU32(w.base, s + 18904, w.save);
               FillBits(w.base, w.save + 0x19930, 7, r.Below(6), r);
               uint32_t set = r.Below(5);
               switch (r.Below(6)) {
                 case 0: set = 5 + r.Below(20); break;
                 case 1: set = uint32_t(r.Next()); break;
                 default: break;
               }
               guest::StoreU32(w.base, s + 33900, set);
             },
             [&h](PPCContext&, const World& w, Outputs& out) {
               out.Bytes(Read(h.base, w.save, kSaveDataBytes));
             }});
}

// TuningScreen::SetUp with its callees: the save random, its garage shaped
// as for the callees, the selected car's words Car::Refresh and
// SetAeroAvailability read in range (Refresh's tables, as car_specs'
// harness lays them: the id below 72 and often 8 or 10, the engine stage,
// the tires' types and grades, the wheels, the aero levels, the body kit
// 0 to 3); the car's spec's bytes the set-up tests (+708 and +709, the
// record's +68 and +69: 7 or not, 0, 1 or others; +770, the record's +130;
// the word +760, the record's +120, 0 or not; +771 the parts of car 0 or
// its own) and table 82's two records' first words 0 or not (CountBodyKits
// 0, 1 or 2); the day around 999, negative or any; the money any. The stack
// below the harness's random fill made random too (the frame is 4,128
// bytes). Its frame but its saves (r27 on, -48) compared, and SetPrices'
// words below it (frame - 160 to - 128; the callees' saves above them, and
// the frames below, are written by the originals only); the save too.
void SetUpScreen(Context& h, const World& w) {
  Run(h, w, {"TuningScreen::SetUp (sub_821E1B48; its five callees)", 100, __imp__sub_821E1B48,
             hooks::TuningMachineSetUp,
             {{-int32_t(tm::kSetUpFrame), -48},
              {-int32_t(tm::kSetUpFrame) - 160, -int32_t(tm::kSetUpFrame) - 128}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               WriteRandom(base, t.r1.u32 - 8192, 8192 - kStackBytes, r);
               WriteRandom(base, w.save, kSaveDataBytes, r);
               ShapeSave(base, w, r);
               Screen(t, r, w);
               guest::StoreU32(base, save_data::kSlots, w.save);
               int32_t day = 0;
               switch (r.Below(6)) {
                 case 0: day = 999 + int32_t(r.Below(5)) - 2; break;
                 case 1: day = -int32_t(r.Below(30)); break;
                 case 2: day = int32_t(r.Next()); break;
                 default: day = int32_t(r.Below(1200)); break;
               }
               guest::StoreU32(base, w.save + 6100, uint32_t(day));
               const uint32_t s = w.screen();
               const uint32_t c = tuning_screen::SelectedGarageCar(base, s) + 212;
               auto set = [&](uint32_t offset, uint32_t value) {
                 guest::StoreU32(base, c + offset, value);
               };
               static constexpr uint32_t kIds[] = {8, 10};
               const uint32_t id = r.Below(3) == 0 ? kIds[r.Below(2)] : r.Below(kSpecCount);
               set(12, id);
               set(1656, r.Below(16));
               for (const uint32_t offset : {1712u, 1720u}) {
                 const uint32_t type = r.Below(5);
                 set(offset, type);
                 set(offset + 4, type == 0 && r.Below(2) == 0 ? 0 : 1 + r.Below(5));
               }
               set(1728, r.Below(3) == 0 ? 0 : 1 + r.Below(8));
               set(1736, r.Below(3) == 0 ? 0 : 1 + r.Below(8));
               set(1732, r.Below(3) == 0 ? 0 : uint32_t(int32_t(r.Below(7)) - 2));
               set(1740, r.Below(3) == 0 ? 0 : uint32_t(int32_t(r.Below(7)) - 2));
               set(1744, r.Below(11));
               set(1748, r.Below(11));
               set(1752, r.Below(11));
               set(1756, uint32_t(int32_t(r.Below(12)) - 1));
               set(1760, r.Below(7));
               set(1764, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               set(1768, r.Below(4));
               set(1780, r.Below(4));
               set(1784, r.Below(4));
               const uint32_t spec = w.specs + 0x40 + kSpecBytes * id;
               static constexpr uint8_t kKinds[] = {0, 1, 2, 5};
               guest::StoreU8(base, spec + 709, kKinds[r.Below(4)]);
               guest::StoreU8(base, spec + 708, r.Below(2) == 0 ? 7 : uint8_t(r.Below(12)));
               guest::StoreU8(base, spec + 770, uint8_t(r.Next()));
               guest::StoreU8(base, spec + 771, r.Below(4) == 0 ? 1 : 0);
               guest::StoreU32(base, spec + 760, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               const uint32_t kits = guest::LoadU32(base, w.specs + 16 + 4 * 8) + 72 * id;
               for (const uint32_t offset : {0u, 36u}) {
                 guest::StoreU32(base, kits + offset, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               }
             },
             [&h](PPCContext&, const World& w, Outputs& out) {
               out.Bytes(Read(h.base, w.save, kSaveDataBytes));
             }});
}

// The no answers and the notice's close (never seen in play): the queue
// and the category menu pointed at words of the screen's own (+36000 and
// +36100); the state history (256 words from +9756) small states, each
// state a PopTo can look for (3, 4, 15, 19, 24, 25, 39, 40, 53, 56) placed
// in it so every walk ends; its depth byte any; the state 9 to 14, 35 to
// 37, 50 to 52 or another; the category 2, 4, 5, 6 or another, its item
// byte 0 to 9, the bytes and words the branches test (+9662, +30500,
// +33540) 0, 1 or 2; PageItemCount's and PageCursor's bytes (+9660 to
// +9680) small. The screen, the frames (their saves left out) and r3
// compared.
void DialogNo(Context& h, const World& w) {
  auto shape = [](PPCContext& t, Random& r, const World& w) {
    uint8_t* base = w.base;
    const uint32_t s = w.screen();
    guest::StoreU32(base, s + tm::kQueue, s + 36000);
    guest::StoreU32(base, s + tm::kMenu, s + 36100);
    // The history below its depth (11 to 60: its words stop short of the
    // depth byte +10008 and the blocks' pointers), each target at a
    // distinct index under it.
    const uint32_t depth = 11 + r.Below(50);
    uint32_t order[60];
    for (uint32_t i = 0; i < depth; i++) {
      order[i] = i;
      guest::StoreU32(base, s + 9756 + 4 * i, r.Below(64));
    }
    for (uint32_t i = depth - 1; i > 0; i--) {
      std::swap(order[i], order[r.Below(i + 1)]);
    }
    uint32_t k = 0;
    for (const uint32_t target : {3u, 4u, 15u, 19u, 24u, 25u, 39u, 40u, 53u, 56u}) {
      guest::StoreU32(base, s + 9756 + 4 * order[k++], target);
    }
    guest::StoreU8(base, s + 10008, uint8_t(depth));
    static constexpr uint32_t kStates[] = {9, 10, 11, 12, 13, 14, 35, 36, 37, 50, 51, 52};
    guest::StoreU32(base, s + 9652, r.Below(8) == 0 ? r.Below(64) : kStates[r.Below(12)]);
    static constexpr uint32_t kCategories[] = {2, 4, 5, 6};
    guest::StoreU32(base, s + 9656, r.Below(8) == 0 ? r.Below(8) : kCategories[r.Below(4)]);
    for (uint32_t at = 9660; at <= 9680; at++) {
      guest::StoreU8(base, s + at, uint8_t(r.Below(at == 9660 ? 10 : 12)));
    }
    guest::StoreU8(base, s + 9662, uint8_t(r.Below(3)));
    for (const uint32_t at : {30500u, 33540u}) {
      guest::StoreU32(base, s + at, r.Below(3));
    }
    Screen(t, r, w);
  };
  Run(h, w, {"TuningMachine::OnDialogNo (sub_821CEA88; sub_821F7988)", 400, __imp__sub_821CEA88,
             hooks::TuningMachineOnDialogNo, {{-96, -8}, {-192, -104}}, shape});
  Run(h, w, {"TuningMachine::StickerDialogNo (sub_821F7988)", 100, __imp__sub_821F7988,
             hooks::TuningMachineStickerDialogNo, {{-96, -8}}, shape});
  Run(h, w, {"TuningMachine::OnNoticeClosed (sub_821CF8B8)", 200, __imp__sub_821CF8B8,
             hooks::TuningMachineOnNoticeClosed, {{-96, -8}},
             [shape](PPCContext& t, Random& r, const World& w) {
               shape(t, r, w);
               if (r.Below(4) != 0) {
                 guest::StoreU32(w.base, w.screen() + 9652, 10);
               }
             }});
}

}  // namespace

void TuningMachine(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_machine: no block from the allocation layer");
    return;
  }
  Init(h, w);
  SetUpCallees(h, w);
  OwnedParts(h, w);
  Prices(h, w);
  UnlockedParts(h, w);
  SetUpScreen(h, w);
  DialogNo(h, w);
  if (hooks::NativeHeapActive()) {
    Handlers(h, w);
  }
}

}  // namespace harness
