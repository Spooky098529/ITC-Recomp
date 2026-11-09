// The garage screen (src/garage_screen/, docs/functions/garage-screen.md)
// against the recompiled functions, laid as the car dealer's harness lays
// its world (car_dealer.cpp). Each case fills a world of its own (one block
// from the allocation layer) with random words from the case's seed: the
// screen machine, the mode with its message ring, the page object, the two
// menu lists, the screen's data, movie 5 with a pool of 40 nodes (each child
// of a node another of the pool, all TXTs) and the movie table's pair naming
// it, the menus' mappers and the sound manager, and the stand (a garage
// stage and the car scene, as the car dealer's harness lays it), with player
// 0's save data (its garage shaped as the career's harness shapes it), a
// string table of our own and the car specs; points the globals into it
// (put back after); shapes the fields each function tests; runs the
// original (with --native_garage_screen off, so what it calls of the
// screen's is the original too) or the register steps
// (hooks/garage_screen.h); and compares the block, the save data, the
// function's frame without its register saves (and without the words the
// varargs UiMovie::SetText and SetNodeText calls store from r7 on: the
// originals' leftovers, 0 in ours, the user's call for the parking area,
// 2026-10-30), the caller's area above r1, r1 and r3. The current car's
// marker (SetUpLists) is a float made from an integer, compared as a float
// (harness::Outputs). UiMovie::Run's update is stood in for (each call's
// return address, stack pointer and arguments kept, answered 0 or 1 from
// the case's generator); UiMovies::Open's and Delete's calls are recorded on
// both sides and answered 1; the flags blocks MenuList::Init makes (the
// layer's pages) are compared by content and freed; the text records the
// setters make are compared and freed.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "career/career.h"
#include "car_specs/car_specs.h"
#include "garage_screen/garage_screen.h"
#include "hooks/app_start_up.h"
#include "hooks/garage_screen.h"
#include "hooks/heap.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_garage_screen);

namespace harness {

namespace {

namespace gs = garage_screen;

// The block's layout.
constexpr uint32_t kMachineAt = 0x0;         // the screen machine (9,620 bytes)
constexpr uint32_t kModeAt = 0x2600;         // the mode (its ring at +204)
constexpr uint32_t kPagesAt = 0x2A00;        // [0x826D8420]
constexpr uint32_t kListAt = 0x2A40;         // [0x826D8424]
constexpr uint32_t kList2At = 0x2AC0;        // [0x826D8428]
constexpr uint32_t kDataAt = 0x2B40;         // [0x826D842C] (104 bytes)
constexpr uint32_t kPairsAt = 0x3080;        // the movie table's pairs
constexpr uint32_t kHolderAt = 0x30C0;       // the stage's holder
constexpr uint32_t kHolderWordAt = 0x30D0;   // [0x826D83E0] points here
constexpr uint32_t kStageAt = 0x3400;        // the garage stage (6,144 bytes)
constexpr uint32_t kSceneAt = 0x4C00;        // the car scene (5,536 bytes)
constexpr uint32_t kSceneModelAt = 0x6200;   // its one pool 7 scene model
constexpr uint32_t kMovieAt = 0x8000;        // movie 5 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x12400;       // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x1C400;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;             // movie 5's named animations shaped
constexpr uint32_t kMenuWorldAt = 0x1C800;   // the menus' mappers, vtables, blocks, sounds
constexpr uint32_t kBlockBytes = 0x1D400;

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
constexpr uint32_t kFades = 0x826A50AC;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {gs::kStandHolder, 8},  // the holder, ReloadModel's count
    {gs::kPages, 16},       // the page object, the two lists, the data
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {0x826C2E40, 8},  // the first pool set and the car scene
    {kSaveSlots, 16},
    {car_specs::kParams, 4},
    {kFades, 64},
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
      // The car specs' object: +12 the 72 specs (random words once).
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
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t data() const { return at(kDataAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint32_t garage() const { return save + career::kGarage; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  // A string table of our own ([0x826C5C34] pointed at it): its 18 files
  // all one container of 8,192 entries over 16 strings of letters, so any id
  // below 8,192 has a string; no argument codes (the varargs the native
  // callers pass as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;
  uint32_t save;   // player 0's save data (all four slots and the data's +36 point at it)
  uint32_t specs;  // the car specs' object ([0x826C2E48])

 private:
  void MakeStrings() {
    Random r(0x476172616765ull);  // "Garage"
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

// Player 0's garage as the career's harness shapes it: `count` of 50 nodes
// linked from the head, the rest on the free list, each slot 0 to 49; each
// car's id (the record's +224) 0 to 71 (now and then one of the logo's
// special ids).
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
  static constexpr uint32_t kSpecial[] = {2, 34, 35, 43, 46, 47, 52, 56, 57, 71};
  for (uint32_t slot = 0; slot < 50; slot++) {
    guest::StoreU32(base, garage + 212 + 1968 * slot + 12,
                    r.Below(4) == 0 ? kSpecial[r.Below(10)] : r.Below(kSpecCount));
  }
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  // The page object's words small or -1.
  for (uint32_t k = 0; k < 7; k++) {
    guest::StoreU32(base, w.at(kPagesAt) + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(18));
  }
  // The mode's ring: its count and start small.
  guest::StoreU32(base, w.at(kModeAt) + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.at(kModeAt) + 204 + 604, r.Below(60));
  // Movie 5: its live scene node 0, each node's children the pool's (their
  // index +108 small), all TXTs; its loaded flag set or not.
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
  // copy nothing; FindNode reads the root's table, not its count.
  guest::StoreU32(base, w.node(0) + 104, 0);
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, w.at(kPairsAt), gs::kMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  guest::StoreU32(base, w.at(kHolderAt), w.at(kStageAt));
  // Player 0's save data: random words; its garage shaped (0 to 10 cars),
  // the current car 0 to 11.
  WriteRandom(base, w.save, kSaveDataBytes, r);
  ShapeGarage(base, w.garage(), r, r.Below(11));
  guest::StoreU32(base, w.save + career::kSelectedCar, r.Below(12));
  // The data: the save (+36), the selected row (+44) and scroll (+48)
  // small.
  const uint32_t data = w.data();
  guest::StoreU32(base, data + 36, w.save);
  guest::StoreU32(base, data + 44, r.Below(4));
  guest::StoreU32(base, data + 48, r.Below(4));
  // The fades' words: their counts (+48, +56) 0 or not.
  WriteRandom(base, kFades, 64, r);
  for (const uint32_t count : {0x826A50E4u, 0x826A50DCu}) {
    guest::StoreU32(base, count, r.Below(2) == 0 ? 0 : r.Below(30));
  }
  // The machine's words: +9608, +9612, +9616 0 or 1 or any.
  for (const uint32_t field : {gs::kMachineLoaded, gs::kMachineReenter, gs::kMachineDelete}) {
    guest::StoreU32(base, w.at(kMachineAt) + field,
                    r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
}

// Points the globals into the world.
void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, gs::kStandHolder, w.at(kHolderWordAt));
  guest::StoreU32(base, gs::kPages, w.at(kPagesAt));
  guest::StoreU32(base, gs::kList, w.at(kListAt));
  guest::StoreU32(base, gs::kList2, w.at(kList2At));
  guest::StoreU32(base, gs::kData, w.data());
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
// 1 (Delete would free our movie). Other addresses go to the recorder set
// before the run, when there's one.
std::vector<uint32_t> g_opens;
hooks::StartUpRecorder g_chained = nullptr;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != gs::kMoviesOpen && address != gs::kMoviesDelete) {
    return g_chained != nullptr && g_chained(ctx, address);
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.insert(g_opens.end(), {address, ctx.r3.u32,
                                 address == gs::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
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
        const bool native = ITC_FLAG(native_garage_screen);
        ITC_FLAG(native_garage_screen) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_garage_screen) = native;
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

// The menus' world (as the car dealer's harness lays it): the two lists'
// controller (+60) 0 to 3, each controller's entry 0 to 3, each entry's
// mapper one of ours (its vtable's slots 10 and 12 Pressed and Fired, now
// and then Held) with a few bits set in its words (+448 to +471); each
// list's count (+0) 0 to 12, scroll, shown and selection around it, the
// action ids, the wrap word, the move word 0 mostly and the block of item
// words (half of them 0); the sound manager's slot 10 (MenuSound::Play's
// PlayCue2D) the getter sub_82090770.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

void MenuWorld(Random& r, const World& w) {
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
  const uint32_t lists[2] = {w.at(kListAt), w.at(kList2At)};
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
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, 0x82090770);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// The lists' flag blocks (+68): 0 or the layer's pages (MenuList::Init
// frees the old one and allocates a new one); after the run, the new ones
// compared by content (4 bytes an item) and freed.
void MenuBlocks(Random& r, const World& w) {
  for (const uint32_t list : {w.at(kListAt), w.at(kList2At)}) {
    guest::StoreU32(w.base, list + 68, r.Below(2) == 0 ? 0 : MakePages(w.base, 64, r));
  }
}
void FreeMenuBlocks(PPCContext&, const World& w, Outputs& out) {
  for (const uint32_t list : {w.at(kListAt), w.at(kList2At)}) {
    const uint32_t count = std::min(guest::LoadU32(w.base, list + 0), 128u);
    Allocated(w.base, out, list + 68, 4 * count);
  }
}

// The rows' world: the list's scroll (+4) 0 to the count less 4 (so the
// rows walked stay among the linked cars); with `negative`, now and then
// the garage's count 0xFFF0 and up (negative: no rows; not for
// MenuList::Init, which would allocate the count's words).
void RowsWorld(Random& r, const World& w, bool negative = false) {
  uint8_t* base = w.base;
  const int32_t count = int16_t(guest::LoadU16(base, w.save + 6328));
  guest::StoreU32(base, w.at(kListAt) + 4, count > 4 ? r.Below(uint32_t(count - 3)) : 0);
  if (negative && r.Below(8) == 0) {
    guest::StoreU16(base, w.save + 6328, uint16_t(0xFFF0 + r.Below(16)));
  }
}

// The started word (the page object's +24): 0 mostly, 1 or any.
void Started(PPCContext& t, Random& r, const World& w) {
  const uint32_t step = w.at(kPagesAt) + 24;
  const uint32_t words[3] = {0, 1, uint32_t(r.Next())};
  guest::StoreU32(w.base, step, words[r.Below(4) == 0 ? 1 + r.Below(2) : 0]);
  t.r3.u64 = step;
}

void Helpers(Context& h, const World& w) {
  // SetCarRow: a node of the pool, a garage car (its id shaped by
  // ShapeGarage); SetNodeText's parameter area from +40 (its varargs, r6
  // on: the original's leftovers) left out.
  Run(h, w, {"GarageScreen::SetCarRow (sub_82193180)", 150, __imp__sub_82193180,
             hooks::GarageScreenSetCarRow, {{-128, -88}, {-48, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.node(r.Below(kNodeCount));
               t.r4.u64 = w.garage() + 212 + 1968 * r.Below(50);
             }});
  Run(h, w, {"GarageScreen::FillRows (sub_82194CD8; sub_82193180)", 150, __imp__sub_82194CD8,
             hooks::GarageScreenFillRows, {{-144, -56}},
             [](PPCContext&, Random& r, const World& w) { RowsWorld(r, w, true); }});
  Run(h, w, {"GarageScreen::SetPrompts (sub_821939A0)", 100, __imp__sub_821939A0,
             hooks::GarageScreenSetPrompts, {{-176, -56}},
             [](PPCContext& t, Random& r, const World&) {
               for (PPCRegister* reg : {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7, &t.r8}) {
                 reg->u64 = r.Below(3) == 0 ? 0xFFFFFFFFFFFFFFFFull : Small(r, 4);
               }
             }});
  // SetHelpLine: SetText's parameter area (+48 to +79: r7 on, the
  // original's leftovers) left out.
  Run(h, w, {"GarageScreen::SetHelpLine (sub_82193A90)", 100, __imp__sub_82193A90,
             hooks::GarageScreenSetHelpLine, {{-112, -64}, {-32, -16}},
             [](PPCContext& t, Random& r, const World&) { t.r3.u64 = r.Below(8192); }});
}

void Transitions(Context& h, const World& w) {
  // SetUpLists: the menus' world with their flag blocks, the rows' world,
  // the data's +44 small.
  Run(h, w, {"GarageScreen::SetUpLists (sub_821956C0; sub_82194CD8, sub_821939A0, "
             "sub_82193A90)",
             150, __imp__sub_821956C0, hooks::GarageScreenSetUpLists, {{-176, -64}},
             [](PPCContext&, Random& r, const World& w) {
               MenuWorld(r, w);
               MenuBlocks(r, w);
               RowsWorld(r, w);
               guest::StoreU32(w.base, w.data() + 44, uint32_t(Small(r, 4)));
               guest::StoreU32(w.base, w.data() + 48,
                               guest::LoadU32(w.base, w.at(kListAt) + 4));
             },
             FreeMenuBlocks});
  // Transition0: SetUpLists' world, the started word.
  Run(h, w, {"GarageScreen::Transition0 (sub_82195BA0; sub_821956C0)", 150, __imp__sub_82195BA0,
             hooks::GarageScreenTransition0, {{-112, -24}},
             [](PPCContext& t, Random& r, const World& w) {
               MenuWorld(r, w);
               MenuBlocks(r, w);
               RowsWorld(r, w);
               Started(t, r, w);
             },
             FreeMenuBlocks});
  auto plain = [](PPCContext& t, Random& r, const World& w) {
    MenuWorld(r, w);
    Started(t, r, w);
  };
  Run(h, w, {"GarageScreen::Transition1 (sub_82192D60)", 100, __imp__sub_82192D60,
             hooks::GarageScreenTransition1, {{-128, -32}}, plain});
  Run(h, w, {"GarageScreen::Transition3 (sub_82192E20)", 100, __imp__sub_82192E20,
             hooks::GarageScreenTransition3, {{-112, -24}}, plain});
}

}  // namespace

void GarageScreen(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: garage_screen: no block from the allocation layer");
    return;
  }
  Helpers(h, w);
  if (hooks::NativeHeapActive()) {
    Transitions(h, w);
  }
}

}  // namespace harness
