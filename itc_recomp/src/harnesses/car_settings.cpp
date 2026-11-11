// The car settings screen (src/car_settings/, docs/functions/car-settings.md)
// against the recompiled functions, laid as the race menu's harness lays its
// world (race_menu.cpp). Each case fills a world of its own (one block from
// the allocation layer) with random words from the case's seed: the
// machine, the mode with its message ring, movie 3 (its animation lists set
// up empty, its loaded flag 0 or 1) and the movie table's pair naming it
// (now and then none: UiMovies::Find answers the missing movie, our movie or
// 0); points the globals into it (put back after); shapes the fields each
// function tests; runs the original (with --native_car_settings off) or the
// register steps (hooks/car_settings.h); and compares the block, the
// globals, the function's frame without its register saves, the caller's
// area above r1, r1 and r3. UiMovies::Open's and Delete's calls are recorded
// on both sides and answered 1; the blocks VirtualAlloc makes (the layer's
// pages) are compared by content and freed, the blocks Destroy frees made
// as the layer's pages (whether each is still pages compared).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_settings/car_settings.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/car_settings.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_settings);

namespace harness {

namespace {

namespace cs = car_settings;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;     // the machine (its transitions, +9604 to +9612)
constexpr uint32_t kModeAt = 0x2800;     // the mode (its ring at +204)
constexpr uint32_t kPairsAt = 0x2C00;    // the movie table's pair
constexpr uint32_t kMovieAt = 0x3000;    // movie 3 (0xA100 bytes)
constexpr uint32_t kBlockBytes = 0xD200;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {cs::kPages, 12},  // the page object, the menu and the data
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {cs::kModeType, 4},
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
  uint32_t machine() const { return at(kMachineAt); }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t movie() const { return at(kMovieAt); }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
};

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.mode() + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.mode() + 204 + 604, r.Below(60));
  // Movie 3: its animation lists empty (FinishAll walks them), its loaded
  // flag 0 or 1.
  ui_movies::InitAnimationLists(base, w.movie());
  guest::StoreU32(base, w.movie() + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, w.at(kPairsAt), cs::kMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, w.movie());
  // The machine's words: +9608 and +9612 0 or 1 or any.
  for (const uint32_t field : {cs::kMachineLoaded, cs::kMachineReenter}) {
    guest::StoreU32(base, w.machine() + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, cs::kPages, 12, r);
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, w.at(kPairsAt) + 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie: ours; 0 only in Close's cases, which test it).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(6) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, w.movie());
  // The mode's type: 2, 5 or another.
  static constexpr uint32_t kTypes[] = {2, 5, 0, 1, 3, 6, 7};
  guest::StoreU32(base, cs::kModeType, kTypes[r.Below(7)]);
}

// UiMovies::Open's and Delete's calls, recorded on both sides and answered
// 1 (Delete would free our movie). Other addresses go to the recorder set
// before the run, when there's one.
std::vector<uint32_t> g_opens;
hooks::StartUpRecorder g_chained = nullptr;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != cs::kMoviesOpen && address != cs::kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == cs::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
                                 uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

// The layer's pages a case made (blocks the function frees): after the
// run, whether each is still pages (then freed).
std::vector<uint32_t> g_made;
uint32_t MakePages(uint8_t* base, uint32_t bytes, Random& r) {
  const uint32_t block = memory::AllocPages(bytes, 4096);
  WriteRandom(base, block, bytes, r);
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
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_opens.clear();
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      g_chained = recorder;
      hooks::g_start_up_recorder = RecordOpen;
      if (v == 0) {
        const bool native = ITC_FLAG(native_car_settings);
        ITC_FLAG(native_car_settings) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_car_settings) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      g_chained = nullptr;
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

// r3 the machine (now and then with high bits), r4 the mode.
void Machine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.machine() | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = w.mode();
}

// The machine's transitions: constructed empty, or a count of random
// keys (some found again, replaced).
void Transitions(PPCContext& t, Random& r, const World& w) {
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
  t.r3.u64 = machine | (uint64_t(r.Below(2)) << 32);
}

void Inits(Context& h, const World& w) {
  Run(h, w, {"CarSettings::Init (sub_821B6FC0)", 24, __imp__sub_821B6FC0, hooks::CarSettingsInit,
             {{-int32_t(cs::kInitFrame), -48}}, Transitions});
  Run(h, w, {"CarSettings::InitType5 (sub_821B7878)", 24, __imp__sub_821B7878,
             hooks::CarSettingsInitType5, {{-int32_t(cs::kInitType5Frame), -48}}, Transitions});
}

void Handlers(Context& h, const World& w) {
  // Create: the three blocks VirtualAlloc made compared by content.
  Run(h, w, {"CarSettings::Create (sub_821AA2F8)", 60, __imp__sub_821AA2F8,
             hooks::CarSettingsCreate, {{-112, -24}}, Machine,
             [&h](PPCContext& t, const World&, Outputs& out) {
               // r3 the data's address (the last VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(h.base, cs::kData) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(h.base, out, cs::kPages, cs::kPagesBytes);
               Allocated(h.base, out, cs::kMenu, cs::kMenuBytes);
               Allocated(h.base, out, cs::kData, cs::kDataBytes);
             }});
  // OpenMovie: a tail call (no frame of its own).
  Run(h, w, {"CarSettings::OpenMovie (sub_821A8DD0)", 20, __imp__sub_821A8DD0,
             hooks::CarSettingsOpenMovie, {}, Machine});
  Run(h, w, {"CarSettings::WaitForMovie (sub_821AA490)", 100, __imp__sub_821AA490,
             hooks::CarSettingsWaitForMovie, {{-96, -16}}, Machine});
  // Close: the missing movie 0 or ours (Find answers it when the pair isn't
  // there).
  Run(h, w, {"CarSettings::Close (sub_821AAF70)", 100, __imp__sub_821AAF70,
             hooks::CarSettingsClose, {{-112, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               Machine(t, r, w);
               if (r.Below(2) == 0) {
                 guest::StoreU32(w.base, kMissingMovie, 0);
               }
             }});
  // Destroy: the three blocks the layer's pages (whether each was freed
  // compared), the data's +576 1 or not.
  Run(h, w, {"CarSettings::Destroy (sub_821AA408)", 60, __imp__sub_821AA408,
             hooks::CarSettingsDestroy, {{-128, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               Machine(t, r, w);
               guest::StoreU32(base, cs::kPages, MakePages(base, cs::kPagesBytes, r));
               guest::StoreU32(base, cs::kMenu, MakePages(base, cs::kMenuBytes, r));
               const uint32_t data = MakePages(base, cs::kDataBytes, r);
               guest::StoreU32(base, cs::kData, data);
               guest::StoreU32(base, data + 576,
                               r.Below(2) == 0   ? 1
                               : r.Below(3) == 0 ? uint32_t(r.Next())
                                                 : 0);
             }});
}

}  // namespace

void CarSettings(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: car_settings: no block from the allocation layer");
    return;
  }
  Inits(h, w);
  if (hooks::NativeHeapActive()) {
    Handlers(h, w);
  }
}

}  // namespace harness
