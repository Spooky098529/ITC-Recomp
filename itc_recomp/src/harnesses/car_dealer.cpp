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
#include "car_specs/car_specs.h"
#include "career/career.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/car_dealer.h"
#include "hooks/heap.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
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
constexpr uint32_t kAnimationsAt = 0x28C00;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // movie 4's named animations shaped
constexpr uint32_t kMenuWorldAt = 0x29000;   // the menus' mappers, vtables, blocks, sounds
constexpr uint32_t kBlockBytes = 0x29C00;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr uint32_t kSaveSlots = 0x826C5B58;    // the players' save data
constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kSpecCount = 72;
constexpr uint32_t kSpecBytes = 8432;
constexpr uint32_t kRandSeed = 0x82638188;     // rand's (Car::SetModel's picks)

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
    {kSaveSlots, 16},
    {car_specs::kParams, 4},
    {kRandSeed, 4},
    {0x826A50AC, 64},  // the screen fades
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
        block((allocation + 15) & ~15u), strings(memory::Alloc(kStringsBytes, 0)),
        save(memory::Alloc(kSaveDataBytes, 0)),
        specs(memory::Alloc(0x40 + kSpecCount * kSpecBytes, 0)) {
    if (strings != 0) {
      MakeStrings();
    }
    if (specs != 0) {
      // The car specs' object: +12 the 72 specs (random words once; the
      // cases shape the bytes the functions test).
      Random r(0x5370656373ull);  // "Specs"
      WriteRandom(base, specs, 0x40 + kSpecCount * kSpecBytes, r);
      guest::StoreU32(base, specs + 12, specs + 0x40);
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(strings);
    memory::Free(save);
    memory::Free(specs);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && strings != 0 && save != 0 && specs != 0; }
  uint32_t spec(uint32_t id) const { return specs + 0x40 + kSpecBytes * id; }
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
  uint32_t save;   // player 0's save data (all four slots point at it)
  uint32_t specs;  // the car specs' object ([0x826C2E48])

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
  // (over the live and saved roots' children) copy nothing; FindNode reads
  // the root's table, not its count.
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, w.at(kPairsAt), 4);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  guest::StoreU32(base, w.at(kHolderAt), w.at(kStageAt));
  // Player 0's save data: random words; the garage's count (+6328) small.
  WriteRandom(base, w.save, kSaveDataBytes, r);
  guest::StoreU16(base, w.save + 6328, uint16_t(r.Below(6)));
  guest::StoreU32(base, kRandSeed, uint32_t(r.Next()));
  // The machine's words: +9608, +9612, +9616 0 or 1 or any.
  for (const uint32_t field : {cd::kMachineLoaded, cd::kMachineReenter, cd::kMachineDelete}) {
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
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, w.save);
  }
  guest::StoreU32(base, car_specs::kParams, w.specs);
}

// The stand's world for GarageStage::ReloadModel (as the build screen's
// harness lays it): the car scene ([0x826C2E44]; +4 0, no sound loads; a
// teardown counting down or its models updating) with one pool 7 scene
// model in use (a car with no parts, its build deferred), the first set
// none, the reload count 0 to 5, the stage's +16 set or not.
void StandWorld(uint8_t* base, Random& r, const World& w, bool drawless = false) {
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
  if (drawless) {
    // As the car list's harness lays the stand for its callers: the garage
    // camera not set up (+112 0: GarageStage::Draw does nothing; its own
    // cases draw), +4 -1 or not (GarageStage::IsBusy), the reload count 0
    // to 2 (GarageStage::ReloadModel answers 0: the branch after it, the
    // stand's car made by ShowCarById, is the car makers' and load_poll's
    // harnesses', not laid here).
    guest::StoreU32(base, stage + 112, 0);
    guest::StoreU32(base, stage + 4, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(4));
    guest::StoreU32(base, 0x826D83E4, r.Below(3));
  }
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
// 1 (Delete would free our movie).
std::vector<uint32_t> g_opens;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != cd::kMoviesOpen && address != cd::kMoviesDelete) return false;
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == cd::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
                                 uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

// The layer's pages a case made (entries the function frees): after the
// run, whether each is still pages (then freed); words naming them in the
// world replaced by their index first (Normalize), as the two runs' blocks
// may lie at different addresses.
std::vector<uint32_t> g_made;
uint32_t MakePages(uint8_t* base, uint32_t bytes, Random& r) {
  const uint32_t block = memory::AllocPages(bytes, 4096);
  WriteRandom(base, block, std::min(bytes, 64u), r);
  g_made.push_back(block);
  return block;
}
void Normalize(uint8_t* base, uint32_t from, uint32_t words) {
  for (uint32_t i = 0; i < words; i++) {
    const uint32_t word = guest::LoadU32(base, from + 4 * i);
    for (size_t k = 0; k < g_made.size(); k++) {
      if (word == g_made[k]) {
        guest::StoreU32(base, from + 4 * i, 0x100 + uint32_t(k));
        break;
      }
    }
  }
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
      hooks::g_start_up_recorder = RecordOpen;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      if (v == 0) {
        const bool native = ITC_FLAG(native_car_dealer);
        ITC_FLAG(native_car_dealer) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_car_dealer) = native;
      } else {
        spec.native(t, h.base);
      }
      hooks::g_start_up_recorder = recorder;
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
      TextRecords(h.base, w, out[v]);
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

// The lists' and the colors' entries as the layer's pages (0 to 4 each).
void PagedEntries(PPCContext&, Random& r, const World& w) {
  const uint32_t data = w.data();
  for (const auto& [list, count] : {std::pair{104u, 616u}, {620u, 1132u}, {1140u, 1192u}}) {
    const uint32_t n = r.Below(5);
    for (uint32_t i = 0; i < n; i++) {
      guest::StoreU32(w.base, data + list + 4 * i, MakePages(w.base, 64, r));
    }
    guest::StoreU32(w.base, data + count, n);
  }
}
void NormalizeEntries(PPCContext&, const World& w, Outputs&) {
  for (const uint32_t list : {104u, 620u, 1140u}) {
    Normalize(w.base, w.data() + list, 8);
  }
}

// Player 0's garage as the career's harness shapes it: `count` of 50 nodes
// linked from the head, the rest on the free list, each slot 0 to 49.
void ShapeGarage(uint8_t* base, uint32_t garage, Random& r, uint32_t count) {
  uint32_t order[50];
  for (uint32_t i = 0; i < 50; i++) {
    order[i] = i;
  }
  for (uint32_t i = 49; i > 0; i--) {
    std::swap(order[i], order[r.Below(i + 1)]);
  }
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
}

// The entries BuildLists made (the layer's pages appended to the lists):
// compared (their 8,436 bytes written), freed, their words 1; r3 (the last
// memcpy's destination) 1 when it was one.
void FreeBuilt(PPCContext& t, const World& w, Outputs& out) {
  uint8_t* base = w.base;
  out.Word(memory::IsPages(t.r3.u32) ? 1 : 0);
  t.r3.u64 = memory::IsPages(t.r3.u32) ? 1 : t.r3.u64;
  for (const uint32_t list : {104u, 620u}) {
    const uint32_t n = std::min(guest::LoadU32(base, w.data() + list + 512), 128u);
    for (uint32_t i = 0; i < n; i++) {
      const uint32_t at = w.data() + list + 4 * i;
      const uint32_t entry = guest::LoadU32(base, at);
      if (entry != 0 && memory::IsPages(entry)) {
        out.Bytes(Read(base, entry, 8436));
        memory::FreePages(entry);
        guest::StoreU32(base, at, 1);
      }
    }
  }
}

// BuildLists' world: the save's bit set of ids (0 to 71) and the specs'
// byte +771 (0, 1 or another) at random, the day (any, or 0 or more for
// Start: FillStatus stores day % 7 at a node's child index), the garage's
// count, the lists' counts small.
void ListsWorld(Random& r, const World& w, bool day_positive) {
  uint8_t* base = w.base;
  for (uint32_t k = 0; k < 3; k++) {
    const uint32_t bits = r.Below(4) == 0 ? 0 : uint32_t(r.Next() & r.Next());
    guest::StoreU32(base, w.save + 0x19924 + 4 * k, bits);
  }
  for (uint32_t id = 0; id < kSpecCount; id++) {
    const uint32_t kinds[4] = {0, 1, 2, 0xFF};
    guest::StoreU8(base, w.spec(id) + 771, uint8_t(kinds[r.Below(4)]));
  }
  const int32_t days[] = {0, 6, 7, 999, 1000, -8};
  const int32_t day = r.Below(2) == 0 ? days[r.Below(6)] : int32_t(r.Below(2000));
  guest::StoreU32(base, w.save + 6100, uint32_t(day_positive && day < 0 ? -day : day));
  guest::StoreU16(base, w.save + 6328,
                  uint16_t(r.Below(4) == 0 ? 0xFFF0 + r.Below(16) : r.Below(8)));
  guest::StoreU32(base, w.data() + 616, r.Below(3));
  guest::StoreU32(base, w.data() + 1132, r.Below(3));
}

// Buy's world: the garage as the career's harness shapes it (fewer than 49
// cars), the selected car's id one of the four specs Car::SetModelAndClass
// reads (their byte +708 0 to 9), Career::PickNext's gate closed (the
// stats' races below 15 or the day below 3), the data's +4 and +24 0 or 1.
void BuyWorld(Random& r, const World& w) {
  uint8_t* base = w.base;
  ShapeGarage(base, w.save + career::kGarage, r, r.Below(49));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU8(base, w.spec(i) + 708, uint8_t(r.Below(10)));
  }
  const uint32_t data = w.data();
  for (uint32_t k = 0; k < kEntryCount; k++) {
    guest::StoreU32(base, w.entry(k) + 8432, r.Below(4));
  }
  guest::StoreU32(base, data + 4, r.Below(2));
  guest::StoreU32(base, data + 24, r.Below(2));
  if (r.Below(2) == 0) {
    guest::StoreU16(base, w.save + 70, uint16_t(int16_t(int32_t(r.Below(20)) - 5)));
  } else {
    guest::StoreU16(base, w.save + 70, uint16_t(15 + r.Below(500)));
    guest::StoreU32(base, w.save + 6100, uint32_t(2 - int32_t(r.Below(1000))));
  }
  guest::StoreU32(base, w.save + career::kSelectedCar, r.Below(4));
}

void Lifecycle(Context& h, const World& w) {
  auto machine = [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kMachineAt); };
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Run(h, w, {"CarDealer::Unload (sub_821816B8)", 80, __imp__sub_821816B8, hooks::CarDealerUnload,
             {{-128, -32}},
             [machine](PPCContext& t, Random& r, const World& w) {
               PagedEntries(t, r, w);
               machine(t, r, w);
             },
             NormalizeEntries});
  Run(h, w, {"CarDealer::Close (sub_82181838)", 80, __imp__sub_82181838, hooks::CarDealerClose,
             {{-112, -24}},
             [machine](PPCContext& t, Random& r, const World& w) {
               PagedEntries(t, r, w);
               machine(t, r, w);
               t.r4.u64 = w.at(kModeAt);
             },
             NormalizeEntries});
  // Destroy: the page object, both menus and the data the layer's pages (the
  // list menu's +68 block one too, or none), their globals cleared.
  Run(h, w, {"CarDealer::Destroy (sub_82180E70)", 80, __imp__sub_82180E70,
             hooks::CarDealerDestroy, {{-128, -40}},
             [machine](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               const uint32_t pages = MakePages(base, 28, r);
               const uint32_t list = MakePages(base, 72, r);
               const uint32_t colors = MakePages(base, 72, r);
               const uint32_t data = MakePages(base, 64, r);
               guest::StoreU32(base, list + 68, r.Below(2) == 0 ? 0 : MakePages(base, 64, r));
               guest::StoreU32(base, data + 8, r.Below(3) == 0 ? 1 : 0);
               guest::StoreU32(base, data + 4, r.Below(2));
               guest::StoreU32(base, cd::kPages, pages);
               guest::StoreU32(base, cd::kListMenu, list);
               guest::StoreU32(base, cd::kColorMenu, colors);
               guest::StoreU32(base, cd::kData, data);
               machine(t, r, w);
             },
             [](PPCContext&, const World& w, Outputs&) {
               Normalize(w.base, cd::kStandHolder, 6);
             }});
  // BuildLists: the save's bit set of ids (0 to 71) and the specs' byte
  // +771 (0, 1 or another) at random, the lists' counts small; the entries
  // VirtualAlloc made compared (their 8,436 bytes written) and freed.
  Run(h, w, {"CarDealer::BuildLists (sub_821813B0)", 60, __imp__sub_821813B0,
             hooks::CarDealerBuildLists, {{-128, -48}},
             [](PPCContext&, Random& r, const World& w) { ListsWorld(r, w, false); },
             FreeBuilt});
  // Buy: BuyWorld.
  Run(h, w, {"CarDealer::Buy (sub_82182E68)", 80, __imp__sub_82182E68, hooks::CarDealerBuy,
             {{-2112, -48}},
             [](PPCContext&, Random& r, const World& w) { BuyWorld(r, w); }});
  // OnYes: the dialog's step 2 or another.
  Run(h, w, {"CarDealer::OnYes (sub_82181968)", 80, __imp__sub_82181968, hooks::CarDealerOnYes,
             {{-112, -24}},
             [](PPCContext&, Random& r, const World& w) {
               guest::StoreU32(w.base, w.data() + 36, r.Below(2) == 0 ? 2 : r.Below(5));
             }});
  // UpdatePage: the page -1, 4 or 5 (the leaves' handlers: Page4's plain
  // C++ through its adapter, Page5 the original), r4 the mode.
  Run(h, w, {"CarDealer::UpdatePage (sub_82184AF0)", 60, __imp__sub_82184AF0,
             hooks::CarDealerUpdatePage, {{-112, -8}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t pages[3] = {0xFFFFFFFFu, 4, 5};
               guest::StoreU32(w.base, w.at(kPagesAt), pages[r.Below(3)]);
               t.r4.u64 = w.at(kModeAt);
             }});
}

// The entry fields ShowCarInfo reads as floats (+652, +660): now and then
// a NaN (quiet or signaling), an infinity, a huge or a negative value.
uint32_t FloatBits(Random& r) {
  static constexpr uint32_t kSpecial[] = {0x7FC00000u, 0x7F800001u, 0xFFA00000u, 0x7F800000u,
                                          0xFF800000u, 0x4F000000u, 0xCF000001u, 0x4F7FFFFFu,
                                          0xBF800000u, 0x00000001u};
  if (r.Below(3) == 0) {
    return kSpecial[r.Below(10)];
  }
  return std::bit_cast<uint32_t>(float(r.Below(40000)) / 7.0f - 100.0f);
}

void Info(Context& h, const World& w) {
  // ShowCarInfo: the index small; the entries' float fields shaped.
  auto shape_entries = [](Random& r, const World& w) {
    for (uint32_t k = 0; k < kEntryCount; k++) {
      guest::StoreU32(w.base, w.entry(k) + 652, FloatBits(r));
      guest::StoreU32(w.base, w.entry(k) + 660, FloatBits(r));
      guest::StoreU8(w.base, w.entry(k) + 709, uint8_t(r.Below(2) == 0 ? 0 : r.Below(256)));
    }
  };
  Run(h, w, {"CarDealer::ShowCarInfo (sub_82182620; sub_821824A8, sub_82182388, sub_82182418)",
             100, __imp__sub_82182620, hooks::CarDealerShowCarInfo, {{-128, -16}},
             [shape_entries](PPCContext& t, Random& r, const World& w) {
               shape_entries(r, w);
               t.r3.u64 = r.Below(6);
             }});
  // The setters: their parameter areas (+48 to +79, or +56 on where the
  // last SetText's r7 is set) left out (the originals' leftovers).
  Run(h, w, {"CarDealer::SetSpecTexts (sub_821824A8)", 100, __imp__sub_821824A8,
             hooks::CarDealerSetSpecTexts, {{-112, -64}, {-32, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t record = w.at(kColorsAt);
               WriteRandom(w.base, record, 24, r);
               guest::StoreU32(w.base, record + 0, r.Below(5));
               guest::StoreU32(w.base, record + 4, r.Below(5));
               guest::StoreU32(w.base, record + 8, r.Below(2) == 0 ? r.Below(3000) : r.Next());
               guest::StoreU32(w.base, record + 20, FloatBits(r));
               t.r3.u64 = record;
             }});
  Run(h, w, {"CarDealer::SetNameTexts (sub_82182388)", 100, __imp__sub_82182388,
             hooks::CarDealerSetNameTexts, {{-112, -64}, {-32, -24}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(80);
               t.r4.u64 = r.Below(256);
             }});
  Run(h, w, {"CarDealer::SetPriceTexts (sub_82182418)", 100, __imp__sub_82182418,
             hooks::CarDealerSetPriceTexts, {{-112, -56}, {-32, -24}},
             [](PPCContext& t, Random& r, const World&) {
               t.r3.u64 = r.Below(2) == 0 ? r.Below(100000) : r.Next();
             }});
  Run(h, w, {"CarDealer::ShowSwatches (sub_821826C0; sub_82181270)", 100, __imp__sub_821826C0,
             hooks::CarDealerShowSwatches, {{-144, -56}},
             [](PPCContext&, Random& r, const World& w) {
               guest::StoreU32(w.base, w.data() + 1192, uint32_t(int32_t(r.Below(14)) - 2));
               guest::StoreU32(w.base, w.at(kListMenuAt) + 4, r.Below(4));
             }});
  Run(h, w, {"CarDealer::SetSwatchColor (sub_82181270)", 60, __imp__sub_82181270,
             hooks::CarDealerSetSwatchColor, {{-96, -16}},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.node(r.Below(kNodeCount));
               t.r4.u64 = w.at(kColorsAt) + 32 * r.Below(16);
             }});
  // FillStatus: SetText's parameter area from +56 (its r7 set) left out;
  // SetPrompts' frame below (its saves left out).
  Run(h, w, {"CarDealer::FillStatus (sub_82183AC8; sub_82182B30)", 100, __imp__sub_82183AC8,
             hooks::CarDealerFillStatus, {{-112, -56}, {-32, -24}, {-288, -168}},
             [](PPCContext&, Random& r, const World& w) {
               // The weekday (+88, stored at node (15, 11)'s +108, its first
               // child's index) 0 to 6, so the walks after stay in the pool.
               guest::StoreU32(w.base, w.data() + 4, r.Below(2));
               guest::StoreU32(w.base, w.data() + 88, r.Below(7));
             }});
  // Leave: the garage not empty (the first car's path, StartFirstRace, is
  // race_event's).
  Run(h, w, {"CarDealer::Leave (sub_821817C0)", 60, __imp__sub_821817C0, hooks::CarDealerLeave,
             {},
             [](PPCContext& t, Random&, const World& w) {
               guest::StoreU32(w.base, w.data() + 4, 0);
               t.r3.u64 = w.at(kMachineAt);
               t.r4.u64 = w.at(kModeAt);
             }});
  // Transition3: r3 the started word (0, 1 or any), the fades' counts 0 or
  // not.
  Run(h, w, {"CarDealer::Transition3 (sub_82181D68)", 100, __imp__sub_82181D68,
             hooks::CarDealerTransition3, {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               const uint32_t step = w.at(kPagesAt) + 24;
               const uint32_t words[3] = {0, 1, uint32_t(r.Next())};
               guest::StoreU32(w.base, step, words[r.Below(3)]);
               WriteRandom(w.base, 0x826A50AC, 64, r);
               for (const uint32_t count : {0x826A50E4u, 0x826A50DCu}) {
                 guest::StoreU32(w.base, count, r.Below(2) == 0 ? 0 : r.Below(30));
               }
               t.r3.u64 = step;
             }});
}

void Updates(Context& h, const World& w) {
  // The busy marker's three (112-byte frames, r31 saved).
  Run(h, w, {"CarDealer::ShowBusy (sub_82181558)", 40, __imp__sub_82181558,
             hooks::CarDealerShowBusy, {{-112, -16}}, None});
  Run(h, w, {"CarDealer::HideBusy (sub_821815D8)", 40, __imp__sub_821815D8,
             hooks::CarDealerHideBusy, {{-112, -16}}, None});
  Run(h, w, {"CarDealer::HideBusyNow (sub_82181640)", 40, __imp__sub_82181640,
             hooks::CarDealerHideBusyNow, {{-112, -16}}, None});
  // Update: the running transition -1, another negative or 3 (Transition3's
  // plain C++ through its adapter: the fades' counts 0 or not), the queued
  // one -1 or small, the page 4 or another; the stand drawless (as the car
  // list's harness lays it); the data's +20, +28 and +32 0 or not.
  Run(h, w, {"CarDealer::Update (sub_82185600; sub_82181558, sub_821815D8, sub_82181640)", 200,
             __imp__sub_82185600, hooks::CarDealerUpdate, {{-320, -136}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               StandWorld(base, r, w, true);
               const uint32_t pages = w.at(kPagesAt);
               const uint32_t running[3] = {0xFFFFFFFFu, 0xFFFFFFF0u + r.Below(15), 3};
               guest::StoreU32(base, pages + 12, running[r.Below(3)]);
               guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(22));
               guest::StoreU32(base, pages + 0, r.Below(3) == 0 ? 4 : r.Below(6));
               for (const uint32_t count : {0x826A50E4u, 0x826A50DCu}) {
                 guest::StoreU32(base, count, r.Below(2) == 0 ? 0 : r.Below(30));
               }
               guest::StoreU32(base, w.data() + 20, r.Below(2));
               guest::StoreU32(base, w.data() + 28, r.Below(4) == 0 ? 0 : 1);
               guest::StoreU32(base, w.data() + 32, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               t.r3.u64 = w.at(kMachineAt);
             }});
  if (!hooks::NativeHeapActive()) {
    return;
  }
  // Start: BuildLists' world (the day 0 or more) and the drawless stand;
  // the entries it made compared and freed.
  Run(h, w, {"CarDealer::Start (sub_82184B80; sub_821813B0, sub_82183AC8)", 60,
             __imp__sub_82184B80, hooks::CarDealerStart, {{-96, -16}},
             [](PPCContext&, Random& r, const World& w) {
               ListsWorld(r, w, true);
               StandWorld(w.base, r, w, true);
             },
             FreeBuilt});
  // OnDialogDone: the step 0 to 3 or another, the answer 0 or 1, Buy's
  // world; ShowDriveDialog (the original) on movie 4 (16 isn't in the
  // table: the missing movie's word is 4's).
  Run(h, w, {"CarDealer::OnDialogDone (sub_82183160; sub_82182E68)", 200, __imp__sub_82183160,
             hooks::CarDealerOnDialogDone, {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               BuyWorld(r, w);
               guest::StoreU32(w.base, w.data() + 36, r.Below(6) == 0 ? r.Below(9) : r.Below(4));
               guest::StoreU32(w.base, w.data() + 16, r.Below(2));
               t.r4.u64 = w.at(kModeAt);
             }});
}

// The menus' world (as the race screens' harness lays a menu list's): the
// two menus' controller (+60) 0 to 3, each controller's entry 0 to 3, each
// entry's mapper one of ours (its vtable's slots 10 and 12 Pressed and
// Fired, now and then Held) with a few bits set in its words (+448 to
// +471); each menu's count (+0) 0 to 12, scroll, shown and selection
// around it (the first menu's selection `selections` when given), the
// action ids, the wrap word, the move word 0 mostly and the block of item
// words (half of them 0); the sound manager's slot 10 (MenuSound::Play's
// PlayCue2D) the getter sub_82090770. The scroll, shown and selection stay
// near the count (never any word: MenuList::Update's walk over the items
// from a wild selection reads past any block, the original's too).
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

void MenuWorld(Random& r, const World& w, const std::vector<uint32_t>& selections = {}) {
  namespace rs = race_screens;
  uint8_t* base = w.base;
  const uint32_t at = w.at(kMenuWorldAt);
  const uint32_t mappers = at, vtables = at + 0x800, blocks = at + 0x900;
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
  const uint32_t lists[2] = {w.at(kListMenuAt), w.at(kColorMenuAt)};
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t list = lists[i], block = blocks + 0x100 * i;
    guest::StoreU32(base, list + rs::kListPlayer, r.Below(4));
    const int32_t count = int32_t(r.Below(13));
    guest::StoreU32(base, list + rs::kListCount, uint32_t(count));
    guest::StoreU32(base, list + rs::kListShown, Around(r, 0, count + 1));
    guest::StoreU32(base, list + rs::kListScroll, Around(r, -1, count));
    guest::StoreU32(base, list + rs::kListSelection, Around(r, -2, count + 1));
    for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
      guest::StoreU32(base, list + field,
                      r.Below(8) == 0 ? uint32_t(r.Next()) : 151 + r.Below(30));
    }
    guest::StoreU32(base, list + rs::kListWraps, r.Below(3) == 0 ? 0 : r.Below(4));
    guest::StoreU32(base, list + rs::kListMove, r.Below(3) == 0 ? uint32_t(r.Next()) : 0);
    guest::StoreU32(base, list + rs::kListBlock, block);
    for (uint32_t k = 0; k < 64; k++) {
      guest::StoreU32(base, block + 4 * k, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    }
  }
  if (!selections.empty()) {
    guest::StoreU32(base, lists[0] + rs::kListSelection,
                    selections[r.Below(uint32_t(selections.size()))]);
  }
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, 0x82090770);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

void Pages(Context& h, const World& w) {
  auto mode = [](PPCContext& t, const World& w) { t.r3.u64 = w.at(kModeAt); };
  Run(h, w, {"CarDealer::Page0 (sub_82180F90)", 300, __imp__sub_82180F90, hooks::CarDealerPage0,
             {{-96, -16}},
             [mode](PPCContext& t, Random& r, const World& w) {
               MenuWorld(r, w);
               for (const uint32_t count : {616u, 1132u}) {
                 guest::StoreU32(w.base, w.data() + count, r.Below(3));
               }
               mode(t, w);
             }});
  Run(h, w, {"CarDealer::Page1 (sub_82183428)", 300, __imp__sub_82183428, hooks::CarDealerPage1,
             {{-112, -24}},
             [mode](PPCContext& t, Random& r, const World& w) {
               MenuWorld(r, w);
               guest::StoreU32(w.base, w.data() + 4, r.Below(2));
               mode(t, w);
             }});
  // Page2: the first menu's selection 0, 2 or 3 (1, the spec sheet, goes to
  // SetUpSpecSheet: never seen in play, the original, its world the car
  // makers'), the garage's count and size, the price and money around each
  // other, the list +44 0 or 1, the selected car 43, 2 or another (the
  // press swaps them: ReloadStandModel over the stand's world);
  // SetText's parameter area (+48 to +79) left out.
  Run(h, w, {"CarDealer::Page2 (sub_82183600)", 400, __imp__sub_82183600, hooks::CarDealerPage2,
             {{-144, -96}, {-64, -48}},
             [mode](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               MenuWorld(r, w, {0, 2, 3});
               StandWorld(base, r, w);
               const uint32_t data = w.data();
               guest::StoreU32(base, data + 96, r.Below(10));
               guest::StoreU32(base, data + 100, r.Below(10));
               guest::StoreU32(base, data + 84, r.Below(2000));
               guest::StoreU32(base, data + 44, r.Below(2));
               for (uint32_t k = 0; k < kEntryCount; k++) {
                 const uint32_t ids[4] = {43, 2, 7, 70};
                 guest::StoreU32(base, w.entry(k) + 8432, ids[r.Below(4)]);
                 guest::StoreU32(base, w.entry(k) + 744, r.Below(2000));
               }
               mode(t, w);
             }});
  Run(h, w, {"CarDealer::Page3 (sub_82181A10)", 300, __imp__sub_82181A10, hooks::CarDealerPage3,
             {{-112, -24}},
             [mode](PPCContext& t, Random& r, const World& w) {
               MenuWorld(r, w);
               mode(t, w);
             }});
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
  Lifecycle(h, w);
  Info(h, w);
  Updates(h, w);
  Pages(h, w);
}

}  // namespace harness
