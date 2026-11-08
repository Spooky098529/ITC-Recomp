// The car dealer (src/car_dealer/, docs/functions/car-dealer.md) against the
// recompiled functions. Each case fills a world of its own (one block from
// the allocation layer) with random words from the case's seed: the screen
// machine, the mode with its message ring, the page object, the two menus,
// the dealer's data with its two lists and colors, 6 car entries the lists
// point at, the spec sheet's object, movie 4 with a pool of 40 nodes (each
// child of a node another of the pool) and the movie table's pair naming
// it, and the stand (a garage stage and the car scene for
// GarageStage::ReloadModel, as the build screen's harness lays it); points
// the globals into it (put back after); shapes the fields each function
// tests; runs the original (with --native_car_dealer off, so what it calls
// of the dealer's is the original too) or the register steps
// (hooks/car_dealer.h); and compares the block, the function's frame
// without its register saves (and without the words the varargs
// UiMovie::SetText calls store from r7 on: the originals' leftovers, 0 in
// ours, the user's call for the parking area, 2026-10-30), the caller's
// area above r1, r1 and r3, all exactly (the floats here are integers
// converted, compared as words). UiMovies::Open's calls are recorded on
// both sides (the start-up recorder) and answered 1; the blocks
// VirtualAlloc makes (the layer's pages) are compared by content where
// their pointers were stored, then freed; the text records the setters make
// are compared and freed.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_dealer/car_dealer.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/car_dealer.h"
#include "hooks/heap.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_dealer);

namespace harness {

namespace {

namespace cd = car_dealer;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;         // the screen machine (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;         // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2A00;        // [0x826D83E8]
constexpr uint32_t kListMenuAt = 0x2A40;     // [0x826D83EC]
constexpr uint32_t kColorMenuAt = 0x2AC0;    // [0x826D83F0]
constexpr uint32_t kLimitsAt = 0x2B40;       // [0x826D8438]
constexpr uint32_t kDataAt = 0x2B80;         // [0x826D83F4] (1,196 bytes)
constexpr uint32_t kPairsAt = 0x3080;        // the movie table's pairs
constexpr uint32_t kHolderAt = 0x30C0;       // the stage's holder
constexpr uint32_t kHolderWordAt = 0x30D0;   // [0x826D83E0] points here
constexpr uint32_t kColorsAt = 0x3100;       // 16 color records of 32 bytes
constexpr uint32_t kStageAt = 0x3400;        // the garage stage (6,144 bytes)
constexpr uint32_t kSceneAt = 0x4C00;        // the car scene (5,536 bytes)
constexpr uint32_t kSceneModelAt = 0x6200;   // its one pool 7 scene model
constexpr uint32_t kMovieAt = 0x8000;        // movie 4 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x12400;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kEntriesAt = 0x1C400;     // 6 car entries of 8,448 bytes
constexpr uint32_t kEntryBytes = 0x2100;
constexpr uint32_t kEntryCount = 6;
constexpr uint32_t kBlockBytes = 0x28C00;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {cd::kStandHolder, 24},  // the holder, ReloadModel's count, the pages, menus, data
    {cd::kLimits, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {0x826C2E40, 8},  // the first pool set and the car scene
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
  uint32_t entry(uint32_t k) const { return at(kEntriesAt) + kEntryBytes * k; }
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
    Random r(0x4465616C6572ull);  // "Dealer"
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
  // The dealer's data: the selection (+48, +52) small, the two lists (+104,
  // +620; counts +616, +1132) of the entries, the shown list (+1136) one of
  // them, the colors (+1140, count +1192) the color records.
  const uint32_t data = w.data();
  guest::StoreU32(base, data + 48, r.Below(3));
  guest::StoreU32(base, data + 52, r.Below(3));
  for (const uint32_t list : {104u, 620u}) {
    for (uint32_t i = 0; i < 128; i++) {
      guest::StoreU32(base, data + list + 4 * i, w.entry(r.Below(kEntryCount)));
    }
    guest::StoreU32(base, data + list + 512, r.Below(8));
  }
  guest::StoreU32(base, data + 1136, data + (r.Below(2) == 0 ? 104 : 620));
  for (uint32_t i = 0; i < 13; i++) {
    guest::StoreU32(base, data + 1140 + 4 * i, w.at(kColorsAt) + 32 * r.Below(16));
  }
  guest::StoreU32(base, data + 1192, r.Below(14));
  // The entries: their ids (+8432) small (CarModels::BlockOf's table), the
  // spec words CheckLimits tests around its bounds.
  for (uint32_t k = 0; k < kEntryCount; k++) {
    const uint32_t entry = w.entry(k);
    guest::StoreU32(base, entry + 8432, r.Below(80));
    const uint32_t bounds[4][2] = {{648, 2000}, {716, 4700}, {720, 2000}, {724, 1700}};
    for (const auto& [field, bound] : bounds) {
      guest::StoreU32(base, entry + field,
                      r.Below(3) == 0 ? uint32_t(r.Next()) : bound - 2 + r.Below(5));
    }
  }
  // The page object's words small or -1.
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, w.at(kPagesAt) + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(22));
  }
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.at(kModeAt) + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.at(kModeAt) + 204 + 604, r.Below(60));
  // Movie 4: its live scene node 0, each node's children the pool's; its
  // loaded flag set or not.
  const uint32_t movie = w.at(kMovieAt);
  guest::StoreU32(base, movie + 16, w.node(0));
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
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, w.at(kPairsAt), 4);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  guest::StoreU32(base, w.at(kHolderAt), w.at(kStageAt));
  // The machine's words: +9608, +9612 0 or 1 or any.
  for (const uint32_t field : {cd::kMachineLoaded, cd::kMachineReenter}) {
    guest::StoreU32(base, w.at(kMachineAt) + field,
                    r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, cd::kStandHolder, w.at(kHolderWordAt));
  guest::StoreU32(base, cd::kPages, w.at(kPagesAt));
  guest::StoreU32(base, cd::kListMenu, w.at(kListMenuAt));
  guest::StoreU32(base, cd::kColorMenu, w.at(kColorMenuAt));
  guest::StoreU32(base, cd::kData, w.data());
  guest::StoreU32(base, cd::kLimits, w.at(kLimitsAt));
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pair not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
  guest::StoreU32(base, w.at(kHolderWordAt), w.at(kHolderAt));
}

// The stand's world for GarageStage::ReloadModel (as the build screen's
// harness lays it): the car scene ([0x826C2E44]; +4 0, no sound loads; a
// teardown counting down or its models updating) with one pool 7 scene
// model in use (a car with no parts, its build deferred), the first set
// none, the reload count 0 to 5, the stage's +16 set or not.
void StandWorld(uint8_t* base, Random& r, const World& w) {
  const uint32_t scene = w.at(kSceneAt), model = w.at(kSceneModelAt);
  const uint32_t stage = w.at(kStageAt);
  guest::StoreU32(base, 0x826C2E44, scene);
  guest::StoreU32(base, 0x826C2E40, 0);
  guest::StoreU32(base, 0x826D83E4, r.Below(6));
  guest::StoreU32(base, scene + 0, r.Below(2) == 0 ? 4 : r.Below(4));
  guest::StoreU32(base, scene + 48, 3 + r.Below(4));
  guest::StoreU32(base, scene + 4, 0);
  guest::StoreU32(base, scene + 5048, 1);
  guest::StoreU32(base, scene + 5028, model);
  guest::StoreU32(base, scene + 5036, 1);
  guest::StoreU32(base, model + 8, 0);
  guest::StoreU32(base, model + 16, 3);
  guest::StoreU32(base, model + 20, r.Below(6));
  guest::StoreU32(base, model + 32, 1);
  guest::StoreU32(base, model + 5028, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, model + 5040, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, model + 5044, 3 + r.Below(3));
  for (const uint32_t at : {5048u, 5060u, 5068u, 5076u, 5084u, 5100u}) {
    guest::StoreU32(base, model + at, 0);
  }
  guest::StoreU32(base, model + 5116, scene);
  guest::StoreU32(base, stage + 8, 0);
  guest::StoreU32(base, stage + 16, r.Below(2));
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

// UiMovies::Open's calls, recorded on both sides and answered 1.
std::vector<uint32_t> g_opens;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != cd::kMoviesOpen) return false;
  g_opens.insert(g_opens.end(), {ctx.r3.u32, ctx.r4.u32, ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
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
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = RecordOpen;
      if (v == 0) {
        const bool native = ITC_FLAG(native_car_dealer);
        ITC_FLAG(native_car_dealer) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_car_dealer) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
      out[v].Word(uint32_t(g_opens.size()));
      for (const uint32_t word : g_opens) {
        out[v].Word(word);
      }
      if (spec.after) {
        spec.after(t, w, out[v]);
      }
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

// A small argument: mostly 0 to `n` - 1, now and then -1 or any.
uint64_t Small(Random& r, uint32_t n) {
  switch (r.Below(8)) {
    case 0: return 0xFFFFFFFFFFFFFFFFull;
    case 1: return r.Next();
    default: return r.Below(n);
  }
}

// InitStates: the machine empty or holding random transitions with keys of
// small numbers (states below 12, messages below 52), so some match the
// Init's (Add replaces those); its frame (but its saves, r25 on) compared.
void InitStates(Context& h, const World& w) {
  Run(h, w, {"CarDealer::InitStates (sub_82185920)", 24, __imp__sub_82185920,
             hooks::CarDealerInitStates, {{-1632, -64}},
             [](PPCContext& t, Random& r, const World& w) {
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
             }});
}

void Handlers(Context& h, const World& w) {
  auto machine = [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kMachineAt); };
  if (hooks::NativeHeapActive()) {
    // Create: the four blocks VirtualAlloc made compared by content.
    Run(h, w, {"CarDealer::Create (sub_82180D68)", 60, __imp__sub_82180D68,
               hooks::CarDealerCreate, {{-112, -24}}, machine,
               [&h](PPCContext& t, const World&, Outputs& out) {
                 // r3 the data block's address (the last VirtualAlloc's).
                 out.Word(t.r3.u32 == guest::LoadU32(h.base, cd::kData) ? 1 : 0);
                 t.r3.u64 = 0;
                 Allocated(h.base, out, cd::kPages, 28);
                 Allocated(h.base, out, cd::kListMenu, 72);
                 Allocated(h.base, out, cd::kColorMenu, 72);
                 Allocated(h.base, out, cd::kData, 1196);
               }});
  }
  Run(h, w, {"CarDealer::OpenMovie (sub_82180C08)", 20, __imp__sub_82180C08,
             hooks::CarDealerOpenMovie, {}, None});
  Run(h, w, {"CarDealer::WaitForLoad (sub_82180F20)", 100, __imp__sub_82180F20,
             hooks::CarDealerWaitForLoad, {{-96, -16}}, machine});
  Run(h, w, {"CarDealer::AfterDetail (sub_82180C48)", 100, __imp__sub_82180C48,
             hooks::CarDealerAfterDetail, {},
             [](PPCContext&, Random& r, const World& w) {
               guest::StoreU32(w.base, w.data() + 12, r.Below(3) == 0 ? 1 : 0);
               guest::StoreU32(w.base, w.data() + 44, r.Below(2));
               if (r.Below(3) == 0) {
                 guest::StoreU32(w.base, w.at(kPagesAt), r.Below(2) == 0 ? 2 : 0xFFFFFFFFu);
               }
             }});
  Run(h, w, {"CarDealer::Page4 (sub_82181B98)", 50, __imp__sub_82181B98, hooks::CarDealerPage4,
             {}, [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kModeAt); }});
}

void Helpers(Context& h, const World& w) {
  Run(h, w, {"CarDealer::CheckLimits (sub_821812B8)", 100, __imp__sub_821812B8,
             hooks::CarDealerCheckLimits, {}, None});
  Run(h, w, {"CarDealer::ReloadStandModel (sub_82181330)", 60, __imp__sub_82181330,
             hooks::CarDealerReloadStandModel, {{-96, -16}},
             [](PPCContext&, Random& r, const World& w) { StandWorld(w.base, r, w); }});
  auto bar = [](PPCContext& t, Random& r, const World&) {
    t.r3.u64 = r.Below(4) == 0 ? r.Next() : r.Below(400);
  };
  Run(h, w, {"CarDealer::SetKindBar (sub_82182158)", 60, __imp__sub_82182158,
             hooks::CarDealerSetKindBar, {{-112, -24}}, bar});
  Run(h, w, {"CarDealer::SetColorBar (sub_82182770)", 60, __imp__sub_82182770,
             hooks::CarDealerSetColorBar, {{-112, -24}}, bar});
  Run(h, w, {"CarDealer::SetScrollArrows (sub_82182DD0)", 100, __imp__sub_82182DD0,
             hooks::CarDealerSetScrollArrows, {{-128, -40}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(3) == 0 ? 0 : Small(r, 8);
               t.r4.u64 = r.Below(2) == 0 ? t.r3.u64 + 4 : Small(r, 12);
             }});
  Run(h, w, {"CarDealer::SetHeader (sub_82182C28)", 60, __imp__sub_82182C28,
             hooks::CarDealerSetHeader, {{-128, -40}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = Small(r, 4);
               t.r4.u64 = Small(r, 4);
             }});
  Run(h, w, {"CarDealer::SetPrompts (sub_82182B30)", 100, __imp__sub_82182B30,
             hooks::CarDealerSetPrompts, {{-176, -56}},
             [](PPCContext& t, Random& r, const World&) {
               for (PPCRegister* reg : {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7, &t.r8}) {
                 reg->u64 = r.Below(3) == 0 ? 0xFFFFFFFFFFFFFFFFull : Small(r, 4);
               }
             }});
  // HelpLine::Set: SetText's parameter area (+48 to +79: r7 on, the
  // original's leftovers) left out.
  Run(h, w, {"HelpLine::Set (sub_82199B38)", 100, __imp__sub_82199B38, hooks::HelpLineSet,
             {{-112, -64}, {-32, -16}},
             [](PPCContext& t, Random& r, const World&) { t.r3.u64 = r.Below(8192); }});
}

}  // namespace

void CarDealer(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: car_dealer: no block from the allocation layer");
    return;
  }
  InitStates(h, w);
  Handlers(h, w);
  Helpers(h, w);
}

}  // namespace harness
