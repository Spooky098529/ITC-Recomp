// The tuning screen's pages (src/tuning_pages/,
// docs/functions/tuning-pages.md) against the recompiled functions. Each
// case fills a world of its own (a block from the allocation layer holding
// the screen, its decal images at +10048, and the save data the screen's
// +18904 points at) with random words from the case's seed, plus the stack
// below the call's stack pointer and the caller's area above it; shapes the
// fields the function tests (the state, the category and item around their
// ranges, the picks, the table rows, the garage list); runs the original
// (with --native_tuning_pages off) or the register steps
// (hooks/tuning_pages.h); and compares the blocks, the stack, the caller's
// area, r1 and all of r3 with harness::Outputs (nothing here is a float, so
// all exactly). The screen runs only in the front end, which attract races
// don't reach: these cases are its check.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_models/vinyl.h"
#include "graphics/resources.h"
#include "hooks/tuning_pages.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_tuning_pages);

namespace harness {

// The input pages' cases over a movie world (tuning_pages_movie.cpp).
void TuningPagesMovie(Context& h);

namespace {

constexpr uint32_t kScreenBytes = 0xA000;  // the screen (to +40960: the picks' tables with any byte)
constexpr uint32_t kSaveBytes = 0x1A000;   // the save data (to +0x19924)
constexpr uint32_t kStackBytes = 2048;     // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;     // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

// Movie 13's (and 14's) block: the movie (+0, its +28 the texture table),
// the table's 200 words (+0x100), the texture entries they point at (+0x500,
// 8 bytes each), the movie table's pairs (+0xC00; movie 2 too), a pool of 40
// nodes (+0x1000, each child entry another of the pool; the live scene the
// first).
constexpr uint32_t kMovieBytes = 0xB000;
constexpr uint32_t kNodesAt = 0x1000;
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kTableAt = 0x100;
constexpr uint32_t kSlotsAt = 0x500;
constexpr uint32_t kPairsAt = 0xC00;
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kFades = 0x826A50AC;  // the screen fades' words (to 0x826A50E8)
constexpr uint32_t kFadesBytes = 0x3C;
constexpr uint32_t kQueueAt = 0x9F00;    // the command queue [+10036] in the screen's block

// The car parameters [0x826C2E48] (+0) and the one part table their nine
// pointers name (+0x100: car ids 0 to 25 and levels to 255 stay inside).
constexpr uint32_t kParamsBytes = 0x4000;
constexpr uint32_t kCarParams = 0x826C2E48;
constexpr uint32_t kSaveDataPointer = 0x826C5B58;  // SaveData::Of(0)
constexpr uint32_t kMenuAt = 0x9E00;  // the front-end menu [+10040] in the screen's block

struct World {
  World()
      : screen_block(kScreenBytes), save(kSaveBytes), movie(kMovieBytes), params(kParamsBytes) {}
  bool ok() const {
    return screen_block.address != 0 && save.address != 0 && movie.address != 0 &&
           params.address != 0;
  }
  uint32_t screen() const { return screen_block.address; }
  Block screen_block;
  Block save;
  Block movie;
  Block params;
  uint8_t* base = nullptr;
};

// The globals a case points into its world (or writes), put back after.
struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kFades, kFadesBytes},
    {kCarParams, 4},
    {kSaveDataPointer, 4},
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// A small signed word in [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The fields the pages' functions test: the state 0 to 14 mostly, the
// category -1 to 7, the item 0 to 11, the picks small (now and then any
// byte), the table rows and the wheel fields small (the word indices grow
// with them, and any word would read far outside the world), the garage
// list as the screen's harness lays it.
void ShapeScreen(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 9652, Around(r, 0, 14));
  guest::StoreU32(base, s + 9656, Around(r, -1, 7));
  guest::StoreU8(base, s + 9660, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(12)));
  for (uint32_t k = 9661; k <= 9665; k++) {
    guest::StoreU8(base, s + k, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(6)));
  }
  guest::StoreU32(base, s + 33900, r.Below(6));
  guest::StoreU32(base, s + 33540, Around(r, 0, 2));
  guest::StoreU32(base, s + 33548, r.Below(10));
  guest::StoreU32(base, s + 33564, r.Below(6));
  guest::StoreU32(base, s + 33572, r.Below(5));
  guest::StoreU32(base, s + 25204, r.Below(5) == 0 ? 17 + r.Below(4) : r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
  guest::StoreU32(base, s + 25168, Around(r, 0, 1));
  guest::StoreU32(base, s + 25172, Around(r, 0, 1));
  guest::StoreU32(base, s + 30724, Around(r, -1, 3));
  // The state history's depth (its words +9756 up to the byte itself at
  // +10008; deeper pushes would write over the queue's pointer).
  guest::StoreU8(base, s + 10008, uint8_t(r.Below(60)));
  // The decal images' states: 0 to 2, now and then any word.
  for (uint32_t i = 0; i < 200; i++) {
    guest::StoreU32(base, s + 10048 + 36 * i + 8, r.Below(3) == 0 ? Around(r, 0, 2) : 2);
  }
  // Category 5's item 6 (the vinyls) now and then.
  if (r.Below(3) == 0) {
    guest::StoreU32(base, s + 9656, 5);
    guest::StoreU8(base, s + 9660, 6);
  }
  // The save data's garage list: 51 nodes of 4 bytes from +6120 (the head
  // halfword at +204), each link and the head a node, each slot 0 to 3; the
  // walk [+0x19920] steps; the cars' ids (+224) 0 to 25, now and then any
  // word for InchListEnd (LoadPageLayouts' table read stays near).
  const uint32_t save = w.save.address, nodes = save + 6120;
  guest::StoreU32(base, s + 18904, save);
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, nodes + 1968 * k + 224, r.Below(26));
  }
}

// Movie 13 and 14 (one movie named by both) and its texture table, the
// movie table pointed at its pairs (now and then not found: UiMovies::Find
// answers the missing movie's word, the same movie); the screen's command
// queue; the screen fades' counts 0 most of the time.
void ShapeMovie(uint8_t* base, const World& w, Random& r) {
  const uint32_t movie = w.movie.address;
  guest::StoreU32(base, movie + 28, movie + kTableAt);
  for (uint32_t k = 0; k < 200; k++) {
    guest::StoreU32(base, movie + kTableAt + 4 * k, movie + kSlotsAt + 8 * r.Below(200));
  }
  guest::StoreU32(base, movie + kPairsAt, 13);
  guest::StoreU32(base, movie + kPairsAt + 4, movie);
  guest::StoreU32(base, movie + kPairsAt + 8, 14);
  guest::StoreU32(base, movie + kPairsAt + 12, movie);
  guest::StoreU32(base, movie + kPairsAt + 16, 2);
  guest::StoreU32(base, movie + kPairsAt + 20, movie);
  guest::StoreU32(base, ui_movies::kMovieTable, movie + kPairsAt);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 3);
  const auto node = [&](uint32_t n) { return movie + kNodesAt + kNodeBytes * n; };
  guest::StoreU32(base, movie + 16, node(0));
  for (uint32_t n = 0; n < kNodeCount; n++) {
    guest::StoreU32(base, node(n) + 108, r.Below(20));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node(n) + 128 + 8 * e, node(r.Below(kNodeCount)));
    }
  }
  guest::StoreU32(base, kMissingMovie, movie);
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 10036, s + kQueueAt);
  for (uint32_t i = 0; i < kFadesBytes; i += 4) {
    guest::StoreU32(base, kFades + i, r.Below(4) == 0 ? uint32_t(r.Next()) : 0);
  }
}

// The car parameters: their nine part tables' pointers at the one table.
void ShapeParams(uint8_t* base, const World& w) {
  const uint32_t params = w.params.address;
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, params + 16 + 4 * i, params + 0x100);
  }
  guest::StoreU32(base, kCarParams, params);
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// Stack bytes left out of the comparison besides the top register saves:
// [from, to) bytes below the call's stack pointer (a callee's register
// saves, which its native version doesn't write).
using Holes = std::vector<std::pair<uint32_t, uint32_t>>;

// `saves` the original's register saves' bytes below the stack pointer.
void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, uint32_t saves, const Holes& holes,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    std::vector<std::vector<uint8_t>> saved;
    for (const Global& g : kGlobals) {
      saved.push_back(Read(h.base, g.address, g.bytes));
    }
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    WriteRandom(h.base, w.movie.address, w.movie.bytes, r);
    WriteRandom(h.base, w.params.address, w.params.bytes, r);
    ShapeScreen(h.base, w, r);
    ShapeMovie(h.base, w, r);
    ShapeParams(h.base, w);
    t.r3.u64 = w.screen();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    if (v == 0) {
      const bool flag = ITC_FLAG(native_tuning_pages);
      ITC_FLAG(native_tuning_pages) = false;
      recompiled(t, h.base);
      ITC_FLAG(native_tuning_pages) = flag;
    } else {
      native(t, h.base);
    }
    for (const auto& [from, to] : holes) {
      for (uint32_t a = sp - to; a < sp - from; a++) {
        guest::StoreU8(h.base, a, 0);
      }
    }
    out[v].Bytes(Read(h.base, w.screen_block.address, w.screen_block.bytes));
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, w.movie.address, w.movie.bytes));
    out[v].Bytes(Read(h.base, w.params.address, w.params.bytes));
    for (size_t i = 0; i < saved.size(); i++) {
      out[v].Bytes(Read(h.base, kGlobals[i].address, kGlobals[i].bytes));
      Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
    }
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, uint32_t saves = 0,
           const Holes& holes = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, saves, holes,
         fmt::format("case {}", c));
  }
  results.Report();
}

const SetupFn kScreenOnly = [](PPCContext&, Random&, const World&) {};

// A word with a signaling NaN made quiet (the originals' lfs and stfs quiet
// it, the host's float copies may not; not the game's data).
uint32_t Quiet(uint32_t word) {
  if ((word & 0x7F800000u) == 0x7F800000u && (word & 0x007FFFFFu) != 0) {
    word |= 0x00400000u;
  }
  return word;
}
void QuietRange(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, Quiet(guest::LoadU32(base, address + i)));
  }
}

// The vinyls: the screen's records and the garage cars' decal records
// without signaling NaNs, now and then small ints and plain floats (and
// negative angles) in them.
const SetupFn kVinyls = [](PPCContext&, Random& r, const World& w) {
  const uint32_t s = w.screen();
  QuietRange(w.base, s + 30880, 31220 - 30880);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t c = w.save.address + 6120 + 1968 * k + 212;
    QuietRange(w.base, c + 96, 192);
    if (r.Below(2) == 0) {
      for (uint32_t i = 0; i < 6; i++) {
        const uint32_t record = c + 96 + 32 * i;
        guest::StoreFloat(w.base, record + 0, float(int32_t(r.Below(720)) - 360));
        guest::StoreFloat(w.base, record + 4, float(int32_t(r.Below(720)) - 360));
        guest::StoreFloat(w.base, record + 8, float(r.Below(1000)) / 4.0f - 100.0f);
        guest::StoreFloat(w.base, record + 12, float(r.Below(1000)) / 8.0f);
        guest::StoreFloat(w.base, record + 16, float(r.Below(100)) / 16.0f);
      }
    }
  }
  if (r.Below(2) == 0) {
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t record = s + 30880 + 24 * i;
      guest::StoreU32(w.base, record + 0, Around(r, -300, 300));
      guest::StoreU32(w.base, record + 4, Around(r, -300, 300));
      guest::StoreFloat(w.base, record + 8, float(r.Below(100)) / 16.0f);
      guest::StoreFloat(w.base, record + 12, float(int32_t(r.Below(720)) - 360));
      guest::StoreU32(w.base, record + 16, Around(r, 0, 1));
    }
  }
};

// The confirm dialogs' yes answers: the state (+9652) one of the answers'
// (now and then any), the category, item and type their branches test
// (now and then other ones), the paint and light kit's fields, the vinyl and
// sticker editors' page, layer and area small; the front-end menu in the
// screen's block; the history at least 10 deep with each state the answers
// pop to (3, 4, 15, 19, 23, 24, 53, 56) somewhere below its depth; the
// screen's saved and placed sticker records without signaling NaNs (the
// originals' lfs and stfs quiet them, the host's float copies may not);
// SaveData::Of(0) the world's save data, its +70 below 15 (Career::PickNext's
// gate skips: past it, never seen in play, its original would run on the
// world's random words).
void ShapeDialog(PPCContext& t, Random& r, const World& w, bool stickers) {
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 10040, s + kMenuAt);
  static constexpr uint32_t kStates[] = {9, 11, 11, 11, 12, 12, 13, 13, 14, 14, 14, 37, 37,
                                         50, 51, 52};
  static constexpr uint32_t kStickerStates[] = {50, 50, 51, 52, 52};
  uint32_t state = stickers ? kStickerStates[r.Below(5)] : kStates[r.Below(16)];
  if (r.Below(10) == 0) {
    state = Around(r, 0, 64);
  }
  if (state == 35 || state == 36) {
    state = 37;  // FitSelectedCar's world is car_makers'
  }
  guest::StoreU32(base, s + 9652, state);
  // (category, item, type): the picks the state's branches test, now and
  // then any.
  struct Pick {
    int32_t category;
    uint8_t item, type;
  };
  static constexpr Pick k11[] = {{2, 0, 0}, {5, 0, 0}, {5, 2, 0}, {6, 3, 0}, {6, 1, 0},
                                 {4, 3, 1}, {1, 1, 0}, {0, 2, 0}, {3, 0, 2}};
  static constexpr Pick k12[] = {{5, 4, 0}, {5, 5, 0}, {5, 5, 0}, {5, 6, 0}, {4, 4, 0}};
  static constexpr Pick k14[] = {{4, 3, 1}, {4, 9, 0}, {4, 3, 0}, {4, 9, 1}, {5, 5, 0},
                                 {5, 4, 0}, {6, 3, 0}, {6, 3, 0}, {6, 2, 0}};
  static constexpr Pick k37[] = {{5, 6, 0}, {5, 7, 0}, {5, 2, 0}, {4, 6, 0}};
  Pick pick{int32_t(r.Below(7)), uint8_t(r.Below(10)), uint8_t(r.Below(3))};
  if (r.Below(6) != 0) {
    switch (state) {
      case 11: pick = k11[r.Below(9)]; break;
      case 12:
      case 13: pick = k12[r.Below(5)]; break;
      case 14: pick = k14[r.Below(9)]; break;
      case 37: pick = k37[r.Below(4)]; break;
      default: break;
    }
  }
  if (r.Below(10) == 0) {
    pick.category = int32_t(Around(r, -1, 7));
  }
  guest::StoreU32(base, s + 9656, uint32_t(pick.category));
  guest::StoreU8(base, s + 9660, pick.item);
  guest::StoreU8(base, s + 9662, pick.type);
  guest::StoreU32(base, s + 33540, r.Below(8) == 0 ? Around(r, 0, 3) : r.Below(2));
  guest::StoreU32(base, s + 33576, r.Below(8));
  guest::StoreU32(base, s + 25196, r.Below(2) == 0 ? 0 : 1 + r.Below(3));
  guest::StoreU32(base, s + 30500, r.Below(8) == 0 ? Around(r, 0, 3) : r.Below(2));
  guest::StoreU32(base, s + 30492, r.Below(2) == 0 ? 3 : uint32_t(int32_t(r.Below(5)) - 1));
  guest::StoreU32(base, s + 30724, r.Below(3));
  guest::StoreU32(base, s + 30732, r.Below(3));
  if (state >= 50 && state <= 52) {
    guest::StoreU32(base, s + 30724, r.Below(8));
    guest::StoreU32(base, s + 30732, r.Below(5));
  }
  // The history.
  const uint32_t depth = 10 + r.Below(50);
  guest::StoreU8(base, s + 10008, uint8_t(depth));
  static constexpr uint32_t kTargets[] = {3, 4, 15, 19, 23, 24, 53, 56};
  std::vector<uint32_t> slots(depth);
  for (uint32_t i = 0; i < depth; i++) {
    slots[i] = i;
  }
  for (const uint32_t target : kTargets) {
    const uint32_t k = r.Below(uint32_t(slots.size()));
    guest::StoreU32(base, s + 9756 + 4 * slots[k], target);
    slots.erase(slots.begin() + k);
  }
  // The stickers' records: the saved ones and the placed ones
  // (SaveAreaDecals copies them over the saved).
  QuietRange(base, s + 31240, 32200 - 31240);
  QuietRange(base, s + 32360, 33320 - 32360);
  if (r.Below(2) == 0) {
    for (uint32_t i = 0; i < 40; i++) {
      const uint32_t record = s + 31240 + 24 * i;
      guest::StoreU32(base, record + 0, Around(r, -300, 300));
      guest::StoreU32(base, record + 4, Around(r, -300, 300));
      guest::StoreFloat(base, record + 8, float(int32_t(r.Below(720)) - 360));
      guest::StoreFloat(base, record + 12, float(r.Below(100)) / 16.0f);
      guest::StoreU32(base, record + 16, Around(r, 0, 1));
    }
  }
  guest::StoreU32(base, kSaveDataPointer, w.save.address);
  guest::StoreU16(base, w.save.address + 70, uint16_t(int16_t(int32_t(r.Below(18)) - 3)));
  if (r.Below(4) == 0) {
    t.r3.u64 |= r.Next() << 32;
  }
}

// DecalImages::FreeTexture: entry i 0 to 60 (now and then any word, with
// upper bits); its texture held by a kept slot (mode 1), by another of
// entries 33 to 50 (mode 2), or by nothing (mode 0: a real header of the
// layer's pages, or 0, and real physical data, or 0, freed through
// Texture::Free on both sides). The allocations' addresses differ between
// the two runs, so words equal to them (in the blocks and r3) are compared as
// tokens; whatever is left alive is freed after.
void FreeTextures(Context& h, const World& w) {
  Results results("DecalImages::FreeTexture");
  for (int c = 0; c < 60; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
      const uint32_t images = w.screen() + 10048;
      const uint32_t mode = r.Below(3);
      uint64_t i = mode == 2 ? 33 + r.Below(18) : r.Below(61);
      if (r.Below(10) == 0) {
        i |= r.Next() << 32;
      }
      const uint32_t entry = images + 36 * uint32_t(i);
      uint32_t header = 0, data = 0;
      if (mode == 0) {
        header = r.Below(5) == 0 ? 0 : memory::AllocPages(52, 4096);
        data = r.Below(4) == 0 ? 0 : memory::AllocPhysical(0x1000, 0, 0xFFFFFFFF, 0, 0x404);
        guest::StoreU32(h.base, entry + 28, header);
        guest::StoreU32(h.base, entry + 32, data);
      } else {
        const uint32_t texture = uint32_t(r.Next());
        guest::StoreU32(h.base, entry + 28, texture);
        if (mode == 1) {
          guest::StoreU32(h.base, images + 7236 + 8 * r.Below(200), texture);
        } else {
          uint32_t j = 33 + r.Below(18);
          if (j == uint32_t(i)) {
            j = j == 50 ? 33 : j + 1;
          }
          guest::StoreU32(h.base, images + 36 * j + 28, texture);
        }
      }
      t.r3.u64 = images;
      t.r4.u64 = i;
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_tuning_pages);
        ITC_FLAG(native_tuning_pages) = false;
        __imp__sub_821FE6A0(t, h.base);
        ITC_FLAG(native_tuning_pages) = flag;
      } else {
        hooks::TuningPagesFreeTexture(t, h.base);
      }
      // The allocations as tokens.
      auto token = [&](uint32_t word) -> uint32_t {
        if (word != 0 && word == header) return 0xA110C001u;
        if (word != 0 && word == data) return 0xA110C002u;
        return word;
      };
      for (uint32_t a = w.screen(); a < w.screen() + kScreenBytes; a += 4) {
        guest::StoreU32(h.base, a, token(guest::LoadU32(h.base, a)));
      }
      out[v].Bytes(Read(h.base, w.screen_block.address, w.screen_block.bytes));
      out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes + kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(token(t.r3.u32));
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      const bool header_alive = header != 0 && memory::IsPages(header);
      out[v].Word(header_alive);
      if (header_alive) {
        memory::FreePages(header);
      }
      // The data is still allocated unless Texture::Free cleared its word.
      const bool data_alive = data != 0 && guest::LoadU32(h.base, entry + 32) == 0xA110C002u;
      out[v].Word(data_alive);
      if (data_alive) {
        memory::FreePhysical(data);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// Allocations (whose addresses differ between the two runs) compared as
// which one and where in it.
struct Known {
  uint32_t address;
  uint32_t bytes;
};

void Address(Outputs& out, uint32_t value, const std::vector<Known>& known) {
  for (uint32_t i = 0; i < known.size(); i++) {
    if (known[i].address != 0 && value - known[i].address < known[i].bytes) {
      out.Word(1 + i);
      out.Word(value - known[i].address);
      return;
    }
  }
  out.Word(0);
  out.Word(value);
}

void Addresses(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
               const std::vector<Known>& known) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    Address(out, guest::LoadU32(base, address + i), known);
  }
}

// A texture's data bytes as Texture::Create sizes it.
uint32_t TextureBytes(uint8_t* base, uint32_t width, uint32_t height) {
  return graphics::XGSetTextureHeader(base, width, height, 1, 2, car_models::kSheetFormat, 0, 0, 0,
                                      0, 0, 0);
}

// A TGA file as the archive gives it (the layer's pages): uncompressed 24-
// or 32-bit, `w` x `hh`, its other header bytes and pixels random (as
// car_models' harness makes them).
uint32_t MakeTga(uint8_t* base, uint32_t w, uint32_t hh, uint32_t bits, Random& r,
                 uint32_t* size) {
  *size = 18 + w * hh * (bits / 8);
  const uint32_t buffer = memory::AllocPages(*size, 4096);
  WriteRandom(base, buffer, (*size + 3) & ~3u, r);
  guest::StoreU8(base, buffer + 2, 2);
  guest::StoreU8(base, buffer + 12, uint8_t(w));
  guest::StoreU8(base, buffer + 13, uint8_t(w >> 8));
  guest::StoreU8(base, buffer + 14, uint8_t(hh));
  guest::StoreU8(base, buffer + 15, uint8_t(hh >> 8));
  guest::StoreU8(base, buffer + 16, uint8_t(bits));
  return buffer;
}

void ZeroHoles(uint8_t* base, uint32_t sp, const Holes& holes) {
  for (const auto& [from, to] : holes) {
    for (uint32_t a = sp - to; a < sp - from; a++) {
      guest::StoreU8(base, a, 0);
    }
  }
}

// DecalImages::Update (with Texture::FromTga under it): 0 to 4 entries
// loaded with +20 set (real TGA files, 1 to 48 pixels a side, 24 or 32
// bits, made once a case and shared by both runs), the others with +20 0,
// or +8 not 2 (now and then any word), or both. The textures made on each
// side (headers of the layer's pages, physical data) and the last call's
// pixels compared as tokens, then freed; the screen's block, the textures'
// headers and data bytes, the stack (but for the saves: Update's r30, r31
// and lr 24 bytes, FromTga's __savegprlr_28 40 bytes below its frame,
// Resource::Release's r31 below FromTga's), the caller's area, r1 and r3.
void UpdateCases(Context& h, const World& w) {
  Results results("DecalImages::Update");
  const Holes holes = {{112, 152}, {280, 288}};
  for (int c = 0; c < 30; c++) {
    Random& g = h.random;
    struct Live {
      uint32_t entry, file, size, width, height;
    };
    std::vector<Live> live;
    const uint32_t count = c < 3 ? 0 : g.Below(5);
    for (uint32_t k = 0; k < count; k++) {
      uint32_t entry = g.Below(200);
      bool taken = false;
      for (const Live& l : live) taken |= l.entry == entry;
      if (taken) continue;
      Live l{entry, 0, 0, 1 + g.Below(48), 1 + g.Below(48)};
      l.file = MakeTga(h.base, l.width, l.height, g.Below(2) ? 32 : 24, g, &l.size);
      live.push_back(l);
    }
    const uint64_t seed = g.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
      const uint32_t images = w.screen() + 10048;
      for (uint32_t i = 0; i < 200; i++) {
        const uint32_t e = images + 36 * i;
        const uint32_t pick = r.Below(3);
        if (pick != 1) {
          guest::StoreU32(h.base, e + 20, 0);
        }
        if (pick != 0) {
          uint32_t state = Around(r, -1, 3);
          guest::StoreU32(h.base, e + 8, state == 2 ? 3 : state);
        }
        if (pick == 2 && r.Below(2) == 0) {
          guest::StoreU32(h.base, e + 8, 2);
        }
      }
      for (const Live& l : live) {
        const uint32_t e = images + 36 * l.entry;
        guest::StoreU32(h.base, e + 8, 2);
        guest::StoreU32(h.base, e + 20, uint32_t(r.Next()) | 1);
        guest::StoreU32(h.base, e + 12, l.file);
        guest::StoreU32(h.base, e + 16, l.size);
      }
      t.r3.u64 = images;
      if (r.Below(6) == 0) {
        t.r3.u64 |= r.Next() << 32;
      }
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_tuning_pages);
        ITC_FLAG(native_tuning_pages) = false;
        __imp__sub_821FE628(t, h.base);
        ITC_FLAG(native_tuning_pages) = flag;
      } else {
        hooks::TuningPagesUpdateImages(t, h.base);
      }
      ZeroHoles(h.base, sp, holes);
      std::vector<Known> known;
      std::vector<std::pair<uint32_t, uint32_t>> made;
      for (const Live& l : live) {
        const uint32_t e = images + 36 * l.entry;
        const uint32_t header = guest::LoadU32(h.base, e + 28);
        const uint32_t data = guest::LoadU32(h.base, e + 32);
        const uint32_t bytes = TextureBytes(h.base, l.width, l.height);
        known.push_back({header, 52});
        known.push_back({data, bytes});
        known.push_back({data - 0x40000000u, bytes});
        known.push_back({data + 0x40000000u, bytes});
        made.push_back({header, data});
      }
      // The last call's pixels (freed), at FromTga's frame + 80.
      if (!live.empty()) {
        const uint32_t pixels = guest::LoadU32(h.base, sp - 112 - 160 + 80);
        const Live& last = *std::max_element(
            live.begin(), live.end(), [](const Live& a, const Live& b) { return a.entry < b.entry; });
        known.push_back({pixels, last.width * last.height * 4});
      }
      Addresses(out[v], h.base, w.screen_block.address, w.screen_block.bytes, known);
      for (size_t k = 0; k < live.size(); k++) {
        Addresses(out[v], h.base, made[k].first, 52, known);
        out[v].Bytes(Read(h.base, made[k].second, TextureBytes(h.base, live[k].width,
                                                               live[k].height)));
      }
      Addresses(out[v], h.base, sp - kStackBytes, kStackBytes - 24, known);
      Addresses(out[v], h.base, sp, kCallerBytes, known);
      out[v].Word(t.r1.u32);
      Address(out[v], t.r3.u32, known);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (const auto& [header, data] : made) {
        const bool alive = memory::IsPages(header);
        out[v].Word(alive);
        if (alive) memory::FreePages(header);
        memory::FreePhysical(data);
      }
      for (const Live& l : live) {
        out[v].Word(memory::IsPages(l.file));
      }
    }
    for (const Live& l : live) {
      if (memory::IsPages(l.file)) memory::FreePages(l.file);
    }
    results.Check(out[0], out[1], fmt::format("case {} ({} made)", c, live.size()));
  }
  results.Report();
}

// DecalImages::UnloadAll (with DecalImages::Unload under it, and the
// restores and FreeTexture before it): a quarter of the entries loaded (+8
// 2) with a real file buffer (the layer's pages) and a texture of their own
// (a real header, and real physical data or 0; or none), a kept texture's
// (whose slot frees it) or, for entries 33 to 50, another of those
// entries'; the others in another state with random words. The kept
// textures 0 and a random word (one of them 0), or real ones. The movies'
// world as the restores' cases. The allocations compared as tokens, and
// whether each is still alive (the pages by the layer, the physical data by
// a word still holding it), then freed. The stack but for the saves: the
// restores' and Unload's (112 to 144 below, RestoreStickerMovie's callees'
// 208 to 264 and 320 to 344), UnloadAll's own 24 bytes.
void UnloadAllCases(Context& h, const World& w) {
  Results results("DecalImages::UnloadAll");
  const Holes holes = {{112, 144}, {208, 264}, {320, 344}};
  for (int c = 0; c < 30; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(h.base, g.address, g.bytes));
      }
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
      WriteRandom(h.base, w.movie.address, w.movie.bytes, r);
      ShapeMovie(h.base, w, r);
      const uint32_t images = w.screen() + 10048;
      std::vector<Known> known;
      std::vector<uint32_t> pages, physical;
      auto page = [&](uint32_t bytes) {
        const uint32_t a = memory::AllocPages(bytes, 4096);
        pages.push_back(a);
        known.push_back({a, bytes});
        return a;
      };
      auto data = [&]() {
        const uint32_t a = memory::AllocPhysical(0x1000, 0, 0xFFFFFFFF, 0, 0x404);
        physical.push_back(a);
        known.push_back({a, 0x1000});
        return a;
      };
      // The kept textures: one word 0, or both real (a tenth of them).
      bool kept_used[200] = {};
      for (uint32_t k = 0; k < 200; k++) {
        const uint32_t kept = images + 7236 + 8 * k;
        if (r.Below(10) == 0) {
          guest::StoreU32(h.base, kept, page(52));
          guest::StoreU32(h.base, kept + 4, data());
          kept_used[k] = true;
        } else {
          const uint32_t zero = r.Below(3);
          if (zero != 1) guest::StoreU32(h.base, kept, 0);
          if (zero != 0) guest::StoreU32(h.base, kept + 4, 0);
        }
      }
      bool loaded[200], assigned[200] = {};
      for (uint32_t i = 0; i < 200; i++) {
        loaded[i] = r.Below(4) == 0;
        const uint32_t e = images + 36 * i;
        if (!loaded[i]) {
          const uint32_t state = Around(r, -1, 3);
          guest::StoreU32(h.base, e + 8, state == 2 ? 1 : state);
        } else {
          guest::StoreU32(h.base, e + 8, 2);
          guest::StoreU32(h.base, e + 12, page(1 + r.Below(8192)));
        }
      }
      for (uint32_t i = 0; i < 200; i++) {
        if (!loaded[i] || assigned[i]) continue;
        const uint32_t e = images + 36 * i;
        const uint32_t mode = r.Below(4);
        uint32_t header = 0, physical_data = 0;
        if (mode == 1 || (mode == 3 && (i < 33 || i > 50))) {
          header = page(52);
          physical_data = r.Below(4) == 0 ? 0 : data();
        } else if (mode == 2) {
          uint32_t k = r.Below(200);
          while (kept_used[k]) k = (k + 1) % 200;
          kept_used[k] = true;
          header = page(52);
          physical_data = r.Below(4) == 0 ? 0 : data();
          guest::StoreU32(h.base, images + 7236 + 8 * k, header);
          guest::StoreU32(h.base, images + 7236 + 8 * k + 4, physical_data);
        } else if (mode == 3) {
          header = page(52);
          physical_data = r.Below(4) == 0 ? 0 : data();
          uint32_t j = 33 + r.Below(18);
          if (j == i) j = j == 50 ? 33 : j + 1;
          // Only an entry not yet given a texture (after this one).
          if (j > i && !assigned[j]) {
            assigned[j] = true;
            guest::StoreU32(h.base, images + 36 * j + 28, header);
            guest::StoreU32(h.base, images + 36 * j + 32, physical_data);
          }
        }
        guest::StoreU32(h.base, e + 28, header);
        guest::StoreU32(h.base, e + 32, physical_data);
      }
      t.r3.u64 = images;
      if (r.Below(6) == 0) {
        t.r3.u64 |= r.Next() << 32;
      }
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_tuning_pages);
        ITC_FLAG(native_tuning_pages) = false;
        __imp__sub_82208738(t, h.base);
        ITC_FLAG(native_tuning_pages) = flag;
      } else {
        hooks::TuningPagesUnloadAll(t, h.base);
      }
      ZeroHoles(h.base, sp, holes);
      Addresses(out[v], h.base, w.screen_block.address, w.screen_block.bytes, known);
      out[v].Bytes(Read(h.base, w.movie.address, w.movie.bytes));
      for (size_t i = 0; i < saved.size(); i++) {
        out[v].Bytes(Read(h.base, kGlobals[i].address, kGlobals[i].bytes));
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
      Addresses(out[v], h.base, sp - kStackBytes, kStackBytes - 24, known);
      Addresses(out[v], h.base, sp, kCallerBytes, known);
      out[v].Word(t.r1.u32);
      Address(out[v], t.r3.u32, known);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (const uint32_t a : pages) {
        const bool alive = memory::IsPages(a);
        out[v].Word(alive);
        if (alive) memory::FreePages(a);
      }
      for (const uint32_t a : physical) {
        bool alive = false;
        for (uint32_t at = images; at < images + 8836; at += 4) {
          alive |= guest::LoadU32(h.base, at) == a;
        }
        out[v].Word(alive);
        if (alive) memory::FreePhysical(a);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The decal images' loads (LoadDecalImages, Request, Load): Archive::Load
// recorded (harness::ArchiveLoads: its calls' r3 to r8 compared, answered
// with random serials or -1), VirtualAlloc's pages real (both runs allocate
// them in the same order): the pages compared as tokens by the order they
// first appear in the recorded calls (and in entries' +12), their first 64
// bytes compared (cleared), then freed. The screen's block, the stack but
// for `holes` and the top `saves` bytes, the caller's area, r1 and all of
// r3.
using LoadSetup = std::function<void(PPCContext&, Random&, const World&)>;

void LoadGroup(Context& h, const World& w, const char* name, int cases, const LoadSetup& setup,
               GuestFunction recompiled, GuestFunction native, uint32_t saves,
               const Holes& holes) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      WriteRandom(h.base, w.screen_block.address, w.screen_block.bytes, r);
      const uint32_t images = w.screen() + 10048;
      // The entries' states: 0, 1 (loading), 2 (loaded) or any word; their
      // ids sensible (163 to 581: a decal image's).
      for (uint32_t i = 0; i < 200; i++) {
        const uint32_t e = images + 36 * i;
        const uint32_t kind = r.Below(4);
        guest::StoreU32(h.base, e + 8, kind == 3 ? uint32_t(r.Next()) : kind);
        guest::StoreU32(h.base, e + 0, 163 + r.Below(419));
      }
      t.r3.u64 = w.screen();
      std::vector<int64_t> serials;
      for (int k = 0; k < 260; k++) {
        serials.push_back(r.Below(5) == 0 ? -1 : int64_t(r.Below(1000)));
      }
      setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      ArchiveLoads loads(serials);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_tuning_pages);
        ITC_FLAG(native_tuning_pages) = false;
        recompiled(t, h.base);
        ITC_FLAG(native_tuning_pages) = flag;
      } else {
        native(t, h.base);
      }
      ZeroHoles(h.base, sp, holes);
      // The pages, in the order the loads name them.
      std::vector<uint32_t> pages;
      auto token = [&](uint32_t word) -> uint32_t {
        if (word == 0 || !memory::IsPages(word)) return word;
        for (uint32_t k = 0; k < pages.size(); k++) {
          if (pages[k] == word) return 0xA110C000u + k;
        }
        pages.push_back(word);
        return 0xA110C000u + uint32_t(pages.size() - 1);
      };
      const std::vector<uint32_t>& calls = loads.calls();
      out[v].Word(uint32_t(calls.size()));
      for (size_t k = 0; k < calls.size(); k++) {
        out[v].Word(k % 6 == 1 ? token(calls[k]) : calls[k]);
      }
      for (uint32_t i = 0; i < 200; i++) {
        const uint32_t e = images + 36 * i;
        guest::StoreU32(h.base, e + 12, token(guest::LoadU32(h.base, e + 12)));
      }
      out[v].Bytes(Read(h.base, w.screen_block.address, w.screen_block.bytes));
      out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(uint32_t(pages.size()));
      for (const uint32_t a : pages) {
        out[v].Bytes(Read(h.base, a, 64));
        memory::FreePages(a);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void LoadCases(Context& h, const World& w) {
  // LoadDecalImages: the vinyls (category 5's item 6) four times in five,
  // else another category or item (the stickers' 200 loads); the layer ids
  // +31168 to +31188 small, now and then -1 (the "none" ids 162 and 188).
  // Its saves (__savegprlr_28: 40 bytes) and Request's or Load's (r31 and
  // lr, 16 bytes below its frame) out.
  LoadGroup(
      h, w, "TuningScreen::LoadDecalImages", 25,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t s = w.screen();
        const bool vinyls = r.Below(5) != 0;
        guest::StoreU32(w.base, s + 9656, vinyls ? 5 : Around(r, -1, 7));
        guest::StoreU8(w.base, s + 9660, uint8_t(vinyls ? 6 : r.Below(9)));
        for (uint32_t k = 0; k < 6; k++) {
          guest::StoreU32(w.base, s + 31168 + 4 * k,
                          r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(30));
        }
        if (r.Below(4) == 0) {
          t.r3.u64 |= r.Next() << 32;
        }
      },
      __imp__sub_821FEE10, hooks::TuningPagesLoadDecalImages, 40, {{128, 144}});
  // Request: entries in each of the id swaps' ranges (0 to 20, 21 to 50,
  // 51 to 79, 80 to 108, 109 to 120; now and then any word, with upper
  // bits), the id the range's "none" id half the time (now and then another
  // range's), else a decal image's (the entry's +0 for the entries keeping
  // their id: a decal image's).
  const LoadSetup request = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.screen() + 10048;
    static constexpr uint32_t kFrom[] = {0, 21, 51, 80, 109, 121};
    static constexpr uint32_t kNone[] = {162, 188, 382, 382, 162};
    const uint32_t range = r.Below(5);
    uint64_t i = kFrom[range] + r.Below(kFrom[range + 1] - kFrom[range]);
    t.r4.u64 = r.Below(2) == 0 ? (r.Below(5) == 0 ? kNone[r.Below(5)] : kNone[range])
                               : 163 + r.Below(419);
    if (r.Below(8) == 0) {
      t.r4.u64 |= r.Next() << 32;
    }
    if (r.Below(10) == 0) {
      i = r.Next();
      // Inside the block, now and then with upper bits.
      i = (uint32_t(i) % 200) | (r.Below(2) == 0 ? (i & ~0xFFFFFFFFull) : 0);
    }
    t.r5.u64 = i;
    // The entry's state 0 two times in three, else 0 to 2.
    guest::StoreU32(w.base, w.screen() + 10048 + 36 * uint32_t(i % 200) + 8,
                    r.Below(3) == 0 ? r.Below(3) : 0);
  };
  LoadGroup(h, w, "DecalImages::Request", 80, request, __imp__sub_821FE830,
            hooks::TuningPagesRequestImage, 16, {});
  LoadGroup(h, w, "DecalImages::Load", 30, request, __imp__sub_821F6D40,
            hooks::TuningPagesLoadImage, 16, {});
}

}  // namespace

void TuningPages(Context& h) {
  World w;
  w.base = h.base;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_pages: no memory for the world");
    return;
  }
  // The pages' counts and owned flags.
  Group(h, w, "TuningScreen::LevelCount", 30, kScreenOnly, __imp__sub_821CC728,
        hooks::TuningPagesLevelCount);
  Group(h, w, "TuningScreen::TypeCount", 30, kScreenOnly, __imp__sub_821CC7B8,
        hooks::TuningPagesTypeCount);
  Group(h, w, "TuningScreen::PageCursor", 30, kScreenOnly, __imp__sub_821CCD38,
        hooks::TuningPagesPageCursor);
  Group(h, w, "TuningScreen::PageItemCount", 30, kScreenOnly, __imp__sub_821CCDB0,
        hooks::TuningPagesPageItemCount);
  Group(h, w, "TuningScreen::IsPickOwned", 50, kScreenOnly, __imp__sub_821CCE30,
        hooks::TuningPagesIsPickOwned);
  Group(h, w, "TuningScreen::MarkOwned", 50, kScreenOnly, __imp__sub_821CD498,
        hooks::TuningPagesMarkOwned);
  // TestAndSetWheelFlag: wheel numbers -2 to 180, now and then any word
  // (with upper bits).
  Group(
      h, w, "TuningScreen::TestAndSetWheelFlag", 40,
      [](PPCContext& t, Random& r, const World&) {
        t.r4.u64 = r.Below(8) == 0 ? r.Next() : uint64_t(int64_t(Around(r, -2, 180)));
      },
      __imp__sub_821CDF98, hooks::TuningPagesTestAndSetWheelFlag);
  Group(h, w, "TuningScreen::SetWheelListEnd", 30, kScreenOnly, __imp__sub_821E7DB0,
        hooks::TuningPagesSetWheelListEnd);
  // InchListEnd: the selected car's id 0 to 25, now and then any word.
  Group(
      h, w, "TuningScreen::InchListEnd", 40,
      [](PPCContext&, Random& r, const World& w) {
        if (r.Below(6) == 0) {
          const uint32_t nodes = w.save.address + 6120;
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(w.base, nodes + 1968 * k + 224, uint32_t(r.Next()));
          }
        }
      },
      __imp__sub_821EF288, hooks::TuningPagesInchListEnd);

  // The vinyl editor's set-up.
  Group(h, w, "TuningScreen::SetUpVinylBusyIcon", 10, kScreenOnly, __imp__sub_821FE560,
        hooks::TuningPagesSetUpVinylBusyIcon);
  // AllLoaded: r3 the decal images; none loading a third of the time.
  Group(
      h, w, "DecalImages::AllLoaded", 30,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.screen() + 10048;
        if (r.Below(3) == 0) {
          for (uint32_t i = 0; i < 200; i++) {
            const uint32_t state = w.screen() + 10048 + 36 * i + 8;
            if (guest::LoadU32(w.base, state) == 1) {
              guest::StoreU32(w.base, state, 2);
            }
          }
        }
      },
      __imp__sub_821FE5D8, hooks::TuningPagesAllLoaded);
  Group(h, w, "TuningScreen::KeepListTextures", 20, kScreenOnly, __imp__sub_821FE7D8,
        hooks::TuningPagesKeepListTextures);
  Group(h, w, "TuningScreen::SetEditorLimits", 30, kScreenOnly, __imp__sub_821FE420,
        hooks::TuningPagesSetEditorLimits);
  Group(h, w, "TuningScreen::LoadPageLayouts", 20, kScreenOnly, __imp__sub_822045F8,
        hooks::TuningPagesLoadPageLayouts);

  // The decal images' textures (r30, r31 and lr's saves 24 bytes,
  // __savegprlr_29's 32, r31 and lr's 16). r3 the images (now and then with
  // upper bits), the textures 0 to 199 (the movie's table).
  FreeTextures(h, w);
  // The update and the unload (allocating, as FreeTexture's cases).
  UpdateCases(h, w);
  UnloadAllCases(h, w);

  // Movie 2's nodes (r30, r31 and lr's saves 24 bytes; r31 and lr's 16).
  Group(
      h, w, "TuningScreen::ShowPricePanel", 30,
      [](PPCContext&, Random& r, const World& w) {
        constexpr uint32_t kStates[] = {16, 9, 10, 11, 14, 5, 15, 17};
        guest::StoreU32(w.base, w.screen() + 9652, kStates[r.Below(8)]);
        guest::StoreU32(w.base, w.screen() + 25204, r.Below(3) == 0 ? 0 : r.Below(17));
      },
      __imp__sub_821D9E68, hooks::TuningPagesShowPricePanel, 24);
  Group(
      h, w, "TuningScreen::SetBrandLogo", 30,
      [](PPCContext&, Random& r, const World& w) {
        guest::StoreU32(w.base, w.screen() + 25204, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(18));
      },
      __imp__sub_821EED98, hooks::TuningPagesSetBrandLogo, 16);
  // HsvToRgb: r4 the out (four floats), r5 the in (h, s, v) in the screen's
  // block; h 0 to 1.25 mostly (now and then negative, infinite, a quiet
  // NaN), s 0.0 or -0.0 now and then, v 0 to 1.
  Group(
      h, w, "Color::HsvToRgb", 60,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t in = w.screen() + 0x200, out = w.screen() + 0x100;
        t.r4.u64 = out;
        t.r5.u64 = in;
        constexpr uint32_t kOdd[] = {0xBF000000u, 0x7F800000u, 0xFF800000u, 0x7FC00000u,
                                     0x3F800000u, 0x00000000u};
        guest::StoreU32(w.base, in + 0,
                        r.Below(6) == 0 ? kOdd[r.Below(6)]
                                        : std::bit_cast<uint32_t>(float(r.Below(1250)) / 1000.0f));
        const uint32_t sat = r.Below(5);
        guest::StoreU32(w.base, in + 4,
                        sat == 0 ? 0u : sat == 1 ? 0x80000000u
                                               : std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f));
        guest::StoreU32(w.base, in + 8, std::bit_cast<uint32_t>(float(r.Below(1001)) / 1000.0f));
        QuietRange(w.base, in + 12, 4);
      },
      __imp__sub_8224AB50, hooks::TuningPagesHsvToRgb);

  // The vinyls' records (LoadVinyls' __savegprlr_27: 48 bytes).
  Group(h, w, "TuningScreen::StoreVinyls", 40, kVinyls, __imp__sub_82203F20,
        hooks::TuningPagesStoreVinyls);
  Group(h, w, "TuningScreen::LoadVinyls", 40, kVinyls, __imp__sub_822040A8,
        hooks::TuningPagesLoadVinyls, 48);
  auto restore = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.screen() + 10048;
    if (r.Below(6) == 0) {
      t.r3.u64 |= r.Next() << 32;
    }
    t.r4.u64 = Around(r, -1, 4);
    t.r5.u64 = r.Below(200);
  };
  Group(h, w, "DecalImages::RestoreVinylTexture", 30, restore, __imp__sub_82204698,
        hooks::TuningPagesRestoreVinylTexture, 24);
  Group(h, w, "DecalImages::RestoreStickerTexture", 30, restore, __imp__sub_821F89B8,
        hooks::TuningPagesRestoreStickerTexture, 24);
  Group(
      h, w, "DecalImages::BindVinylTexture", 40,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.screen() + 10048;
        t.r4.u64 = Around(r, -2, 45);
        t.r5.u64 = Around(r, -1, 2);
      },
      __imp__sub_82204730, hooks::TuningPagesBindVinylTexture, 32);
  // The restores: RestoreMovieTexture's __savegprlr_29 (32 bytes) with its
  // callees' (24, 112 bytes below); RestoreVinylMovie's (32) with
  // RestoreStickerTexture's (24, 128 below); RestoreStickerMovie's r31 and
  // lr (16) with RestoreMovieTexture's (32, 96 below) and its callees' (24,
  // 208 below).
  auto images = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = w.screen() + 10048;
    if (r.Below(6) == 0) {
      t.r3.u64 |= r.Next() << 32;
    }
  };
  Group(
      h, w, "DecalImages::RestoreMovieTexture", 30,
      [images](PPCContext& t, Random& r, const World& w) {
        images(t, r, w);
        t.r4.u64 = Around(r, -1, 4);
        t.r5.u64 = r.Below(200);
      },
      __imp__sub_822081A0, hooks::TuningPagesRestoreMovieTexture, 32, {{112, 136}});
  Group(h, w, "DecalImages::RestoreVinylMovie", 20, images, __imp__sub_822081D8,
        hooks::TuningPagesRestoreVinylMovie, 32, {{128, 152}});
  Group(h, w, "DecalImages::RestoreStickerMovie", 20, images, __imp__sub_821F6DD8,
        hooks::TuningPagesRestoreStickerMovie, 16, {{96, 128}, {208, 232}});
  Group(h, w, "TuningScreen::BindVinylTextures", 20, kScreenOnly, __imp__sub_82203D40,
        hooks::TuningPagesBindVinylTextures, 16);
  // EnterEditor: the fades' counts 0 and the images loaded most of the
  // time, category 5's items 6 and 7 mostly; BindVinylTextures' saves (its
  // frame 96 bytes below) out too.
  Group(
      h, w, "TuningScreen::EnterEditor", 40,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t s = w.screen();
        if (r.Below(4) != 0) {
          guest::StoreU32(w.base, kFades + 0x30, 0);
          guest::StoreU32(w.base, kFades + 0x38, 0);
        }
        if (r.Below(4) != 0) {
          for (uint32_t i = 0; i < 200; i++) {
            if (guest::LoadU32(w.base, s + 10048 + 36 * i + 8) == 1) {
              guest::StoreU32(w.base, s + 10048 + 36 * i + 8, 2);
            }
          }
        }
        if (r.Below(5) != 0) {
          guest::StoreU32(w.base, s + 9656, 5);
          guest::StoreU8(w.base, s + 9660, uint8_t(r.Below(4) == 0 ? r.Below(9) : 6 + r.Below(2)));
        }
        guest::StoreU32(w.base, s + 9652, r.Below(3) == 0 ? Around(r, 20, 62) : 60);
        if (r.Below(4) == 0) {
          t.r3.u64 |= r.Next() << 32;
        }
      },
      __imp__sub_821D84F8, hooks::TuningPagesEnterEditor, 16, {{96, 112}});

  // The current choices: the body kit [+36588] 0 a quarter of the time,
  // else a level to 255 (r31 and lr's saves 16 bytes).
  Group(
      h, w, "TuningScreen::SetAeroAvailability", 40,
      [](PPCContext&, Random& r, const World& w) {
        guest::StoreU32(w.base, w.screen() + 36588, r.Below(4) == 0 ? 0 : r.Below(256));
      },
      __imp__sub_821DE160, hooks::TuningPagesSetAeroAvailability, 16);
  // SetCurrentChoice: categories -1 to 7 and items 0 to 10 (now and then any
  // byte), the indices [+33900] and [+33576] small, the wheels' +25196 0
  // half the time, else 1 to 3; r3 now and then with upper bits (lr's save
  // 8 bytes; SetAeroAvailability's r31 and lr, 96 bytes below, out too).
  Group(
      h, w, "TuningScreen::SetCurrentChoice", 120,
      [](PPCContext& t, Random& r, const World& w) {
        const uint32_t s = w.screen();
        guest::StoreU32(w.base, s + 9656, Around(r, -1, 7));
        guest::StoreU8(w.base, s + 9660, uint8_t(r.Below(10) == 0 ? r.Next() : r.Below(11)));
        // A third of the time the paths with calls or more stores: the body
        // kit, the wheels, category 3's item 0, the interior's item 3.
        if (r.Below(3) == 0) {
          static constexpr uint8_t kPicks[4][2] = {{4, 9}, {5, 0}, {3, 0}, {6, 3}};
          const auto& pick = kPicks[r.Below(4)];
          guest::StoreU32(w.base, s + 9656, pick[0]);
          guest::StoreU8(w.base, s + 9660, pick[1]);
        }
        if (r.Below(4) == 0) {
          guest::StoreU8(w.base, s + 9661 + r.Below(4), 0);
        }
        guest::StoreU32(w.base, s + 33900, r.Below(8));
        guest::StoreU32(w.base, s + 33576, r.Below(8));
        guest::StoreU32(w.base, s + 25196, r.Below(2) == 0 ? 0 : 1 + r.Below(3));
        if (r.Below(6) == 0) {
          t.r3.u64 |= r.Next() << 32;
        }
      },
      __imp__sub_821E1DB0, hooks::TuningPagesSetCurrentChoice, 8, {{96, 112}});

  // The confirm dialogs' yes answers (states 35 and 36, which fit the car on
  // the stand, are car_makers' cases: its world has the stand). Their
  // saves: OnDialogYes' __savegprlr_29 (32 bytes), OnStickerDialogYes' r31
  // and lr (16); below them, the callees' saves their plain C++ doesn't
  // write: SetCurrentChoice's lr and OnStickerDialogYes' r31 and lr (112 to
  // 128 below OnDialogYes' r1), SetAeroAvailability's and StoreStickers'
  // r31 and lr (208 to 224; SaveAreaDecals' saves there are overwritten by
  // StoreStickers' conversions and saves).
  Group(
      h, w, "TuningScreen::OnStickerDialogYes", 60,
      [](PPCContext& t, Random& r, const World& w) { ShapeDialog(t, r, w, true); },
      __imp__sub_821FA230, hooks::TuningPagesOnStickerDialogYes, 16, {{96, 112}});
  Group(
      h, w, "TuningScreen::OnDialogYes", 600,
      [](PPCContext& t, Random& r, const World& w) { ShapeDialog(t, r, w, false); },
      __imp__sub_821E2660, hooks::TuningPagesOnDialogYes, 32, {{112, 128}, {208, 224}});

  // The decal images' loads.
  LoadCases(h, w);
  // The input pages over movie 2 (tuning_pages_movie.cpp).
  TuningPagesMovie(h);
}

}  // namespace harness
