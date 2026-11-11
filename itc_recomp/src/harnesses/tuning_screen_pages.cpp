// The tuning screen's functions over movies 2 and 13's animations and texts
// (src/tuning_screen/pages.cpp, docs/functions/tuning-screen.md) against the
// recompiled ones, part of the tuning_screen harness. Its world, laid as the
// build screen's harness lays movie 6 (src/harnesses/build_screen.cpp): one
// block holding the movie (both ids' pairs naming it), a pool of 40 TXT
// nodes (each child of a node another of the pool), the transitions' scene
// root and 8 animations of no tracks (the named animations over them, the
// animation lists set up), the screen and its command queue (and for
// MoveCategory a front-end menu and four action mappers with their vtables,
// the mapper entries' globals pointed at them, used by UpdateItems too; for
// ShowTypes the car parameters [0x826C2E48] and their part tables); the save
// data (its garage list for ShowTypes and FillPartRows) in a block of its own; a string table of our own (letters, newlines and
// page breaks, no argument codes: the varargs native callers pass as 0
// aren't read). Each case fills it with random words from its seed, lays
// it, points the globals into it (put back after), runs the original (with
// --native_tuning_screen off) or the register steps (hooks/tuning_screen.h),
// and compares the text records SetText made (their words and buffers,
// freed), the globals, the blocks, the stack below r1 (without the
// original's register saves and the parameter areas the varargs callees
// store into), the caller's area, r1 and all of r3, exactly. Its movie
// and string table are also laid for car_makers.cpp's DecalCommand and
// SetCarStats cases (LayTextMovie and the functions after it).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_screen.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_tuning_screen);

namespace harness {

namespace {

namespace ts = tuning_screen;

// The block's layout.
constexpr uint32_t kMovieAt = 0x0;           // the movie (0xA100 bytes)
constexpr uint32_t kPairsAt = 0xA200;        // the movie table's pairs
constexpr uint32_t kNodesAt = 0xB000;        // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kRootAt = 0x15000;        // the named animations' scene root (0x400)
constexpr uint32_t kAnimationsAt = 0x15400;  // 8 animations of no tracks (64 bytes)
constexpr uint32_t kZeroAt = 0x15600;        // words past the animations: 0
constexpr uint32_t kScreenAt = 0x16000;      // the screen (0x9400)
constexpr uint32_t kQueueAt = kScreenAt + 0x9380;  // the command queue [+10036]
constexpr uint32_t kTableAt = 0x1F400;       // Command23's movie +28 table (0x300)
constexpr uint32_t kTargetsAt = 0x1F700;     // its pointers' words (0x100)
constexpr uint32_t kDispatcherAt = 0x1F800;  // the dialogs' dispatcher (its ring at +204)
constexpr uint32_t kVtablesAt = 0x1FC00;    // MoveCategory's 4 mappers' vtables (0x40 each)
constexpr uint32_t kMappersAt = 0x1FD00;    // the 4 mappers (0x200 each)
constexpr uint32_t kMenuAt = 0x20500;       // the front-end menu [+10040] (0x100)
constexpr uint32_t kParamsAt = 0x20600;     // ShowTypes' car parameters [0x826C2E48] (0x100)
constexpr uint32_t kPartTablesAt = 0x20700;  // their part tables (0x900)
constexpr uint32_t kBlockBytes = 0x21000;
constexpr uint32_t kTextMovieBytes = kScreenAt;  // LayTextMovie's block: the movie to the zeros
constexpr uint32_t kSaveBytes = 0x1A000;

constexpr uint32_t kStackBytes = 2048;
constexpr uint32_t kCallerBytes = 256;
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {0x826BAB80, 32},  // the players' action mapper entries
    {0x826BABA4, 16},  // their entries' indices
    {0x826C2E48, 4},   // the car parameters (ShowTypes)
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// The string table at `strings`: its 18 files one container of 8,192
// entries over 16 strings, so any id below 8,192 has a string.
void Strings(uint8_t* base, uint32_t strings) {
  Random r(0x54756E65ull);  // "Tune"
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
      uint16_t unit = uint16_t('a' + r.Below(26));
      if (r.Below(10) == 0) {
        unit = r.Below(2) == 0 ? 10 : 0xFFD1;
      }
      guest::StoreU16(base, at + 2 * c, unit);
    }
    guest::StoreU16(base, at + 2 * length, 0);
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), save(memory::Alloc(kSaveBytes, 0)),
        strings(memory::Alloc(kStringsBytes, 0)) {
    if (strings != 0) {
      Strings(base, strings);
    }
  }
  ~World() {
    memory::Free(allocation);
    memory::Free(save);
    memory::Free(strings);
  }
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  bool ok() const { return allocation != 0 && save != 0 && strings != 0; }
  uint32_t at(uint32_t offset) const { return block + offset; }
  uint32_t screen() const { return at(kScreenAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  uint32_t save;
  // The string table: its 18 files one container of 8,192 entries over 16
  // strings, so any id below 8,192 has a string.
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;
};

uint32_t At(uint32_t block, uint32_t offset) { return block + offset; }
uint32_t NodeAt(uint32_t block, uint32_t n) { return block + kNodesAt + kNodeBytes * n; }

// The movie in `block` (the block's words random already): its node pool,
// the named animations' root and animations, both ids' pairs.
void LayMovie(uint8_t* base, uint32_t block, Random& r) {
  const uint32_t movie = At(block, kMovieAt);
  // The nodes: TXT tags, their first child's index small, their children
  // the pool's.
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = NodeAt(block, n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(60));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(20));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, NodeAt(block, r.Below(kNodeCount)));
    }
  }
  // The movie: its animation lists set up, its scenes the root, its 128
  // named animations over the root and one of the 8 animations.
  const uint32_t root = At(block, kRootAt);
  ui_movies::InitAnimationLists(base, movie);
  guest::StoreU32(base, movie + 16, NodeAt(block, 0));
  guest::StoreU32(base, movie + 20, root);
  guest::StoreU32(base, movie + 24, root);
  for (uint32_t k = 0; k < 128; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, root);
    guest::StoreU32(base, named + 32, At(block, kAnimationsAt) + 64 * r.Below(8));
  }
  for (uint32_t i = kZeroAt; i < kScreenAt; i += 4) {
    guest::StoreU32(base, At(block, i), 0);
  }
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t animation = At(block, kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  guest::StoreU32(base, root + 100, 0);
  guest::StoreU32(base, root + 104, 4);
  guest::StoreU32(base, root + 108, r.Below(4));
  guest::StoreU32(base, root + 116, 70);
  for (uint32_t i = 0; i < 94; i++) {
    guest::StoreU32(base, root + 128 + 8 * i, At(block, kAnimationsAt) + 64 * r.Below(8));
  }
  // The texts' map empty, the text count small.
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  // Both ids' pairs.
  guest::StoreU32(base, At(block, kPairsAt), 2);
  guest::StoreU32(base, At(block, kPairsAt) + 4, movie);
  guest::StoreU32(base, At(block, kPairsAt) + 8, 13);
  guest::StoreU32(base, At(block, kPairsAt) + 12, movie);
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  WriteRandom(base, w.save, kSaveBytes, r);
  LayMovie(base, w.block, r);
  // The screen: its save data, its queue, the fields the cases test small.
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + ts::kSaveData, w.save);
  guest::StoreU32(base, s + 10036, w.at(kQueueAt));
  guest::StoreU32(base, w.at(kQueueAt) + 16, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(65));
  guest::StoreU32(base, s + 25204, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(18));
  guest::StoreU32(base, s + 25208, r.Below(2) == 0 ? 11 : r.Below(14));
  guest::StoreU32(base, s + 25220, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(10));
  guest::StoreU32(base, s + 25224, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(10));
  for (const uint32_t f : {9716u, 9720u, 9724u}) {
    guest::StoreU32(base, s + f, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
  constexpr uint32_t kStates[] = {23, 24, 5, 15};
  guest::StoreU32(base, s + ts::kState, kStates[r.Below(4)]);
}

void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 16);
  // Now and then the pairs not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 2);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
}

// The text records GetText made: their words and buffers compared, freed,
// their map words 0.
void MovieTextRecords(uint8_t* base, uint32_t movie, Outputs& out) {
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

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using Setup = std::function<void(PPCContext&, Random&, const World&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1
  Setup setup = {};          // after the world, the globals and the stack
  bool f1 = false;           // f1 compared too (as a double)
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
      t.r3.u64 = w.screen();
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (spec.setup) {
        spec.setup(t, r, w);
      }
      if (v == 0) {
        const bool native = ITC_FLAG(native_tuning_screen);
        ITC_FLAG(native_tuning_screen) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_tuning_screen) = native;
      } else {
        spec.native(t, h.base);
      }
      MovieTextRecords(h.base, w.at(kMovieAt), out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      out[v].Bytes(Read(h.base, w.save, kSaveBytes));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(uint32_t(t.lr));
      if (spec.f1) {
        out[v].Double(t.f1.u64);
      }
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// The named animations past the first 128 (to 1,023, the last below the
// animation lists at +0x90B4) over the root and the 8 animations, as
// MakeWorld lays the first.
void BlockNamedAnimations(uint8_t* base, uint32_t block, Random& r) {
  const uint32_t movie = At(block, kMovieAt);
  for (uint32_t k = 128; k < 1024; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, At(block, kRootAt));
    guest::StoreU32(base, named + 32, At(block, kAnimationsAt) + 64 * r.Below(8));
  }
}
void NamedAnimations(const World& w, Random& r) { BlockNamedAnimations(w.base, w.block, r); }

// Movies 16 and 19 (the dialogs') named by the pairs too, when they're there.
void DialogPairs(const World& w) {
  const uint32_t pairs = w.at(kPairsAt), movie = w.at(kMovieAt);
  guest::StoreU32(w.base, pairs + 16, 16);
  guest::StoreU32(w.base, pairs + 20, movie);
  guest::StoreU32(w.base, pairs + 24, 19);
  guest::StoreU32(w.base, pairs + 28, movie);
  if (guest::LoadU32(w.base, ui_movies::kMovieTable + 8) != 0) {
    guest::StoreU32(w.base, ui_movies::kMovieTable + 8, 4);
  }
}

// The dialogs: r4 the dispatcher (its ring's count and start random words),
// r3's high word now and then set, the state one of `states` mostly.
template <size_t N>
Setup Dialog(const uint32_t (&states)[N]) {
  return [&states](PPCContext& t, Random& r, const World& w) {
    DialogPairs(w);
    t.r4.u64 = w.at(kDispatcherAt);
    if (r.Below(4) == 0) {
      t.r3.u64 |= r.Next() << 32;
    }
    const uint32_t state = r.Below(4) == 0 ? r.Below(40) : states[r.Below(N)];
    guest::StoreU32(w.base, w.screen() + ts::kState, state);
  };
}
// The scene's own animations (UiMovie::Play, Finish and Loop by index, the
// live scene node 0 and its children the pool's): each node's animation
// index +116 100 to 103, its entries 100 to 111 (past the children the
// cases reach) the 8 animations.
void SceneAnimations(const World& w, Random& r) {
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(w.base, node + 116, 100 + r.Below(4));
    for (uint32_t e = 100; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(w.base, node + 128 + 8 * e, w.at(kAnimationsAt) + 64 * r.Below(8));
    }
  }
}

// The scene's animations for the lists' pages (UiMovie::Play and Finish by
// index up to 34 on the scene, 13 on sub-scene 27): each node's animation
// index +116 60 to 63, its entries 60 to 111 (past the children the cases
// reach) the 8 animations.
void ListAnimations(const World& w, Random& r) {
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(w.base, node + 116, 60 + r.Below(4));
    for (uint32_t e = 60; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(w.base, node + 128 + 8 * e, w.at(kAnimationsAt) + 64 * r.Below(8));
    }
  }
}

// A small word: 0 to `below` - 1.
uint32_t Small(Random& r, uint32_t below) { return r.Below(below); }

// The lists' pages: their animations, the category 0 to 6 (now and then 7),
// the item 0 to 4, the level, type and option bytes 0 to 7.
void ListPage(const World& w, Random& r) {
  ListAnimations(w, r);
  NamedAnimations(w, r);
  const uint32_t s = w.screen();
  guest::StoreU32(w.base, s + ts::kCategory, r.Below(8) == 0 ? 7 : r.Below(7));
  guest::StoreU8(w.base, s + ts::kItem, uint8_t(Small(r, 5)));
  guest::StoreU8(w.base, s + ts::kLevel, uint8_t(Small(r, 8)));
  guest::StoreU8(w.base, s + ts::kType, uint8_t(Small(r, 8)));
  guest::StoreU8(w.base, s + ts::kOption, uint8_t(Small(r, 8)));
}

// The tire's two pairs of words (`a` and `b`, `c` and `d`) small, equal half
// the time each.
void TirePairs(const World& w, Random& r, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  const uint32_t s = w.screen();
  const uint32_t first = Small(r, 6), second = Small(r, 6);
  guest::StoreU32(w.base, s + a, first);
  guest::StoreU32(w.base, s + b, r.Below(2) == 0 ? first : Small(r, 6));
  guest::StoreU32(w.base, s + c, second);
  guest::StoreU32(w.base, s + d, r.Below(2) == 0 ? second : Small(r, 6));
}

// MoveCategory's world: the menu at [+10040] (its step -1, 0, 1 or another
// word, its wrap 0, 1 or any word), the four players' mappers with their
// vtables (slot 10 mostly ActionMapper::Pressed, else Fired or Held), their
// words +448 to +471 with none, few or many bits, the entries' +4 1 now and
// then, the player's entry index 0 to 3.
void MoveCategoryWorld(const World& w, Random& r) {
  ListAnimations(w, r);
  const uint32_t s = w.screen();
  guest::StoreU32(w.base, s + ts::kCategory, r.Below(8));
  const uint32_t menu = w.at(kMenuAt);
  guest::StoreU32(w.base, s + 10040, menu);
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0, 1};
  guest::StoreU32(w.base, menu + 8, r.Below(6) == 0 ? uint32_t(r.Next()) : kSteps[r.Below(3)]);
  const uint32_t wrapped = r.Below(2);
  guest::StoreU32(w.base, menu + 40, wrapped == 0 ? 0 : (r.Below(2) == 0 ? 1 : uint32_t(r.Next())));
  guest::StoreU32(w.base, 0x826BABA4, r.Below(4));
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t vtable = w.at(kVtablesAt) + 0x40 * c, mapper = w.at(kMappersAt) + 0x200 * c;
    guest::StoreU32(w.base, 0x826BAB80 + 8 * c, mapper);
    guest::StoreU32(w.base, 0x826BAB80 + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(w.base, mapper, vtable);
    static constexpr uint32_t kSlots[] = {0x8210C010, 0x8210BE70};  // Fired, Held
    guest::StoreU32(w.base, vtable + 40, r.Below(5) == 0 ? kSlots[r.Below(2)] : 0x8210BF40);
    const uint32_t density = r.Below(4);
    for (uint32_t k = 0; k < 6; k++) {
      uint32_t bits = 0;
      if (density != 0) {
        bits = uint32_t(r.Next());
        for (uint32_t d = 1; d < density + 1; d++) {
          bits &= uint32_t(r.Next());
        }
      }
      guest::StoreU32(w.base, mapper + 448 + 4 * k, bits);
    }
  }
}

// UpdateItems' world: the four players' mappers as MoveCategory's, their
// slot 8 mostly ActionMapper::Held (else Pressed or Fired) and slot 11 mostly
// sub_8210BFA8 (else Pressed); states 27 and 28 mostly, the set +30724 0 or
// 1 mostly, the item +30732 and +30756 0 to 2.
void UpdateItemsWorld(const World& w, Random& r) {
  MoveCategoryWorld(w, r);
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t vtable = w.at(kVtablesAt) + 0x40 * c;
    static constexpr uint32_t kSlots[] = {0x8210C010, 0x8210BF40};  // Fired, Pressed
    guest::StoreU32(w.base, vtable + 32, r.Below(4) == 0 ? kSlots[r.Below(2)] : 0x8210BE70);
    guest::StoreU32(w.base, vtable + 44, r.Below(4) == 0 ? 0x8210BF40 : 0x8210BFA8);
  }
  const uint32_t s = w.screen();
  static constexpr uint32_t kItemStates[] = {27, 28, 28, 27, 28, 26, 29, 5};
  guest::StoreU32(w.base, s + ts::kState, kItemStates[r.Below(8)]);
  guest::StoreU32(w.base, s + 30724, r.Below(5) == 0 ? r.Below(4) : r.Below(2));
  guest::StoreU32(w.base, s + 30732, r.Below(3));
  guest::StoreU32(w.base, s + 30756, r.Below(3));
}

// The save data's garage list (at +6120: the head +204, links of 4 bytes,
// +0 the next and +2 the slot, 0 to 7 and 0 to 3), walked 0 to 3 steps
// ([save + 0x19920]); each slot's car id (+224 of its 1,968-byte record)
// below `ids`.
void GarageList(const World& w, Random& r, uint32_t ids) {
  const uint32_t list = w.save + 6120;
  guest::StoreU32(w.base, w.save + 0x19920, r.Below(4));
  guest::StoreU16(w.base, list + 204, uint16_t(r.Below(8)));
  for (uint32_t k = 0; k < 8; k++) {
    guest::StoreU16(w.base, list + 4 * k, uint16_t(r.Below(8)));
    guest::StoreU16(w.base, list + 4 * k + 2, uint16_t(r.Below(4)));
  }
  for (uint32_t slot = 0; slot < 4; slot++) {
    guest::StoreU32(w.base, list + 1968 * slot + 224, r.Below(ids));
  }
}

// ShowTypes' world: the lists' pages (above), power, aero, exterior and
// interior mostly with the items their pages show half the time, aero's
// items 0 to 10, the byte +9678 0 to 12, [+36368] and [+36588] small; the
// garage list (car ids 0 to 3) and the car parameters [0x826C2E48] in the
// block, their 9 tables' pointers at one table area of random words.
void ShowTypesWorld(const World& w, Random& r) {
  ListPage(w, r);
  const uint32_t s = w.screen();
  static constexpr uint32_t kCategories[] = {0, 4, 4, 5, 6, 0, 4, 5, 6, 1, 3};
  const uint32_t category = kCategories[r.Below(11)];
  guest::StoreU32(w.base, s + ts::kCategory, category);
  uint32_t item = r.Below(5);
  if (category == 4) {
    item = r.Below(2) == 0 ? 3 : r.Below(11);
  } else if (r.Below(2) == 0) {
    static constexpr uint32_t kItems[] = {2, 2, 3, 0, 1};
    item = category == 0 ? 2 : category == 5 ? kItems[r.Below(3)] : kItems[2 + r.Below(3)];
  }
  guest::StoreU8(w.base, s + ts::kItem, uint8_t(item));
  guest::StoreU8(w.base, s + 9678, uint8_t(r.Below(13)));
  guest::StoreU32(w.base, s + 36368, Small(r, 8));
  guest::StoreU32(w.base, s + 36588, r.Below(3) == 0 ? 0 : Small(r, 4));
  GarageList(w, r, 4);
  const uint32_t params = w.at(kParamsAt);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(w.base, params + 16 + 4 * i, w.at(kPartTablesAt));
  }
  guest::StoreU32(w.base, 0x826C2E48, params);
}

// SetStateTexts' world: states 15 to 17 mostly, +25228 0 to 7, the group
// +25204 0 or 16 now and then, else 1 to 15, +25212 so the tabs' table
// index (+25204 - +25212 + j) stays -3 to 16; the looping list given slots
// naming 1007 and 62 (time -1.0 or another) now and then.
void StateTextsWorld(const World& w, Random& r) {
  NamedAnimations(w, r);
  const uint32_t s = w.screen();
  static constexpr uint32_t kTextStates[] = {15, 16, 17, 16, 15, 17, 16, 4};
  guest::StoreU32(w.base, s + ts::kState, kTextStates[r.Below(8)]);
  guest::StoreU32(w.base, s + 25228, r.Below(8));
  const uint32_t group = r.Below(3) == 0 ? (r.Below(2) == 0 ? 0 : 16) : 1 + r.Below(15);
  guest::StoreU32(w.base, s + 25204, group);
  guest::StoreU32(w.base, s + 25212, uint32_t(int32_t(group) - int32_t(r.Below(17)) + 3));
  // State 15's textures: nodes (3, 1) and (3, 2) given PIC or PRM tags
  // (now and then left TXT) when they aren't the help text's node (10, 0)
  // or the tabs' and arrows' (sub-scene 1's), which SetText and SetNodeText
  // need as TXT.
  const uint32_t movie = w.at(kMovieAt);
  if (guest::LoadU32(w.base, s + ts::kState) == 15) {
    std::vector<uint32_t> texts = {ui_movies::FindNode(w.base, movie, 10, 0)};
    for (const uint32_t child : {1u, 2u}) {
      const uint32_t node = ui_movies::FindNode(w.base, movie, 3, child);
      bool text = false;
      for (const uint32_t t : texts) {
        text = text || t == node;
      }
      if (!text && r.Below(4) != 0) {
        guest::StoreU32(w.base, node, r.Below(2) == 0 ? 0x50494300u : 0x50524D00u);
      }
    }
  }
  const uint32_t looping = movie + ui_movies::kMovieLooping;
  for (const uint32_t named : {1007u, 62u}) {
    if (r.Below(3) == 0) {
      ui_movies::Slot slot{};
      slot[2] = named;
      slot[5] = r.Below(2) == 0 ? 0xBF800000u : uint32_t(r.Next());
      ui_movies::Append(w.base, looping, slot);
    }
  }
}

// FillPartRows' world: n = +25224 -1 to 8, the cursor +25220 0 to n, the
// group +25204 0 a third of the time, else 1 to 16; the wheel side +25196 0
// to 3; the garage list (car ids 0 to 25, for the group 0's picture).
void PartRowsWorld(const World& w, Random& r) {
  const uint32_t s = w.screen();
  const int32_t last = int32_t(r.Below(10)) - 1;
  guest::StoreU32(w.base, s + 25224, uint32_t(last));
  guest::StoreU32(w.base, s + 25220, last <= 0 ? 0 : r.Below(uint32_t(last) + 1));
  guest::StoreU32(w.base, s + 25204, r.Below(3) == 0 ? 0 : 1 + r.Below(16));
  guest::StoreU32(w.base, s + 25196, r.Below(4));
  GarageList(w, r, 26);
}

// Command8's world: FillPartRows' (above) and the named 1007 and 1011.
void Command8World(const World& w, Random& r) {
  NamedAnimations(w, r);
  PartRowsWorld(w, r);
}

// RefreshMenu's world: the lists' pages (their animations and the named
// ones), its states mostly (now and then another below 60), the fields its
// blocks read small (the tabs' table index +25204 - +25212 + j kept -3 to
// 16, the aero items' words at +25128 0 a third of the time, the flags
// +9720 and +9724 1 a third of the time), and mostly one to three slots in
// the looping list naming the state's triggers, at the time its block waits
// for two times in three, else at a random one.
void RefreshMenuWorld(const World& w, Random& r) {
  ListPage(w, r);
  const uint32_t s = w.screen();
  static constexpr uint32_t kMenuStates[] = {16, 15, 4, 3, 17, 57, 58, 56, 53, 16};
  const uint32_t state = r.Below(10) == 0 ? r.Below(60) : kMenuStates[r.Below(10)];
  guest::StoreU32(w.base, s + ts::kState, state);
  const uint32_t group = r.Below(3) == 0 ? (r.Below(2) == 0 ? 0 : 16) : 1 + r.Below(15);
  guest::StoreU32(w.base, s + 25204, r.Below(4) == 0 ? 11 : group);
  const uint32_t tab_group = guest::LoadU32(w.base, s + 25204);
  guest::StoreU32(w.base, s + 25212, uint32_t(int32_t(tab_group) - int32_t(r.Below(17)) + 3));
  guest::StoreU32(w.base, s + 25196, r.Below(4));
  guest::StoreU32(w.base, s + 25100, r.Below(4));
  guest::StoreU8(w.base, s + ts::kItem, uint8_t(r.Below(10)));
  guest::StoreU8(w.base, s + 9676, uint8_t(r.Below(11)));
  if (r.Below(3) == 0) {
    guest::StoreU32(w.base, s + ts::kCategory, r.Below(2) == 0 ? 4 : 6);
  }
  guest::StoreU32(w.base, s + 36588, r.Below(2) == 0 ? 0 : 1 + r.Below(3));
  for (int32_t k = -4; k < 18; k++) {
    guest::StoreU32(w.base, s + 25128 + 4 * k, r.Below(3) == 0 ? 0 : 1 + r.Below(2));
  }
  guest::StoreU32(w.base, s + 25228, r.Below(8));
  guest::StoreU32(w.base, s + 25232, r.Below(4) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(7));
  guest::StoreU32(w.base, s + 33576, r.Below(5));
  guest::StoreU32(w.base, s + 33564, r.Below(2) == 0 ? 4 : r.Below(6));
  guest::StoreU32(w.base, s + 33540, r.Below(2) == 0 ? 1 : r.Below(3));
  if (r.Below(2) == 0) {
    guest::StoreU32(w.base, s + ts::kCategory, 6);
    guest::StoreU8(w.base, s + ts::kItem, 3);
  }
  for (uint32_t k = 0; k < 5; k++) {
    guest::StoreU32(w.base, s + 25176 + 4 * k, r.Below(2));
  }
  for (const uint32_t f : {9720u, 9724u}) {
    if (r.Below(3) == 0) {
      guest::StoreU32(w.base, s + f, 1);
    }
  }
  // The triggers: (named index, or the scene's animation index + 0x10000;
  // the time's address).
  struct Trigger {
    uint32_t what;
    uint32_t at;
  };
  constexpr uint32_t C = 0x82002CAC;
  static const std::vector<Trigger> kNone = {};
  static const std::vector<Trigger> k16 = {{1007, 0x820021E8}, {62, C}};
  static const std::vector<Trigger> k15 = {{964, C}, {1008, C}};
  static const std::vector<Trigger> k4 = {{0x10001, C}, {0x10002, C}, {966, C}, {1004, C},
                                          {972, C},     {973, C},     {974, C}, {975, C},
                                          {982, C}};
  static const std::vector<Trigger> k3 = {{0x10000, 0x82000C34}, {966, C}, {967, C}, {973, C}};
  static const std::vector<Trigger> k17 = {{61, C}};
  static const std::vector<Trigger> k57 = {{143, C}};
  static const std::vector<Trigger> k56 = {{1005, C}, {144, 0x82002C78}};
  static const std::vector<Trigger> k53 = {{1001, C}, {1003, C}, {1006, 0x82002C78}};
  const std::vector<Trigger>& triggers = state == 16   ? k16
                                         : state == 15 ? k15
                                         : state == 4  ? k4
                                         : state == 3  ? k3
                                         : state == 17 ? k17
                                         : state == 57 || state == 58 ? k57
                                         : state == 56 ? k56
                                         : state == 53 ? k53
                                                       : kNone;
  if (triggers.empty()) {
    return;
  }
  const uint32_t looping = w.at(kMovieAt) + ui_movies::kMovieLooping;
  const uint32_t count = r.Below(5) == 0 ? 0 : 1 + r.Below(3);
  for (uint32_t k = 0; k < count; k++) {
    const Trigger& t = triggers[r.Below(uint32_t(triggers.size()))];
    ui_movies::Slot slot{};
    if (t.what >= 0x10000) {
      slot[0] = t.what - 0x10000;
      slot[1] = 0xFFFFFFFFu;
      slot[2] = 0xFFFFFFFFu;
    } else {
      slot[0] = 0x7777;
      slot[2] = t.what;
    }
    slot[5] = r.Below(3) != 0 ? guest::LoadU32(w.base, t.at) : uint32_t(r.Next());
    ui_movies::Append(w.base, looping, slot);
  }
}

// RefreshMovie13's world: its states mostly (23 to 28 and 34, now and
// then another below 60), the sets +30724 and the picks +30740 and +30748
// small (now and then any word), the name record at +20792 one that names
// strings two times in three (kinds 0, 1, 4 or 5 and 2 or 3, indexes in
// range), the movie's +28 table pointing into the block (state 24's
// pointers), the floats +30840 and +30844 mostly ordinary.
void Movie13World(const World& w, Random& r) {
  const uint32_t s = w.screen();
  static constexpr uint32_t kStates[] = {23, 24, 25, 26, 27, 28, 34, 28};
  guest::StoreU32(w.base, s + ts::kState, r.Below(8) == 0 ? r.Below(60) : kStates[r.Below(8)]);
  guest::StoreU32(w.base, s + 30724, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(w.base, s + 30740, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
  guest::StoreU32(w.base, s + 30748,
                  r.Below(6) == 0 ? uint32_t(-int32_t(r.Below(3))) : r.Below(8));
  guest::StoreU32(w.base, s + 30764, r.Below(6));
  guest::StoreU32(w.base, s + 30768, r.Below(6));
  guest::StoreU32(w.base, s + 30848, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  for (const uint32_t at : {30840u, 30844u}) {
    guest::StoreFloat(w.base, s + at, (float(r.Below(200000)) - 100000.0f) / 64.0f);
  }
  if (r.Below(8) == 0) {
    guest::StoreU32(w.base, s + 30840, uint32_t(r.Next()));  // any word: NaNs, the ends
  }
  if (r.Below(3) != 0) {
    const uint32_t name = s + 20792;
    const uint32_t firsts[] = {0, 1, 4, 5};
    guest::StoreU32(w.base, name + 4, firsts[r.Below(4)]);
    guest::StoreU16(w.base, name + 8, uint16_t(r.Below(4)));
    guest::StoreU8(w.base, name + 10, uint8_t(r.Below(2)));
    guest::StoreU32(w.base, name + 12, 2 + r.Below(2));
    guest::StoreU16(w.base, name + 16, uint16_t(r.Below(6)));
    guest::StoreU8(w.base, name + 18, uint8_t(r.Below(2)));
  }
  guest::StoreU32(w.base, w.at(kMovieAt) + 28, w.at(kTableAt));
  for (const uint32_t slot : {700u, 708u, 712u, 724u, 732u, 736u}) {
    guest::StoreU32(w.base, w.at(kTableAt) + slot, w.at(kTargetsAt) + 4 * r.Below(64));
  }
}

constexpr uint32_t kShowDialogStates[] = {9, 10, 11, 12, 13, 14};
constexpr uint32_t kCommand30States[] = {35, 36, 37};

}  // namespace

uint32_t LayTextMovie(uint8_t* base, uint32_t block, Random& r) {
  WriteRandom(base, block, kTextMovieBytes, r);
  LayMovie(base, block, r);
  BlockNamedAnimations(base, block, r);
  return At(block, kMovieAt);
}

void MakeTextStrings(uint8_t* base, uint32_t strings) { Strings(base, strings); }

// The lists' pages' scene animations (ListAnimations) over LayTextMovie's
// node pool in `block` (TuningScreen::Update's cases, tuning_refresh.cpp).
void TextMovieListAnimations(uint8_t* base, uint32_t block, Random& r) {
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = NodeAt(block, n);
    guest::StoreU32(base, node + 116, 60 + r.Below(4));
    for (uint32_t e = 60; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, At(block, kAnimationsAt) + 64 * r.Below(8));
    }
  }
}

const std::vector<std::pair<uint32_t, uint32_t>>& TextMovieGlobals() {
  static const std::vector<std::pair<uint32_t, uint32_t>> globals = {
      {ui_movies::kMovieTable, 12}, {kMissingMovie, 4}, {kMissingText, 4}, {kStringTable, 4}};
  return globals;
}

void PointTextMovieGlobals(uint8_t* base, uint32_t block, uint32_t strings) {
  guest::StoreU32(base, ui_movies::kMovieTable, At(block, kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 16);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, 2);
  guest::StoreU32(base, kMissingMovie, At(block, kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, strings);
}

void TextMovieRecords(uint8_t* base, uint32_t movie, Outputs& out) {
  MovieTextRecords(base, movie, out);
}

void TuningScreenPages(Context& h) {
  World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_screen: no memory for the pages' world");
    return;
  }
  // The frames without their register saves (r30, r31 and lr: 24 bytes;
  // savegprlr_29: 32), the SetText callers' without their parameter area
  // (+48 to +79, the varargs SetText stores: the user's call for the unset
  // ones, 2026-10-30).
  Run(h, w, {"TuningScreen::UpdateListCounter", 20, __imp__sub_821EED00,
             hooks::TuningUpdateListCounter, {{-112, -112 + 48}, {-112 + 80, -24}}});
  Run(h, w, {"TuningScreen::SetPartName", 20, __imp__sub_821F2068, hooks::TuningSetPartName,
             {{-848, -848 + 48}, {-848 + 80, -24}}});
  Run(h, w, {"TuningScreen::PlayGroupAnimation", 25, __imp__sub_821EE880,
             hooks::TuningPlayGroupAnimation, {{-112, -24}}});
  Run(h, w, {"TuningScreen::Command31", 15, __imp__sub_82202880, hooks::TuningCommand31,
             {{-112, -24}}});
  Run(h, w, {"TuningScreen::Command9", 20, __imp__sub_821E9050, hooks::TuningCommand9,
             {{-128, -32}}});
  // savegprlr_28: 40 bytes.
  Run(h, w, {"TuningScreen::ShowDialog", 25, __imp__sub_821D6230, hooks::TuningShowDialog,
             {{-128, -128 + 48}, {-128 + 80, -40}}, Dialog(kShowDialogStates)});
  Run(h, w, {"TuningScreen::Command30", 20, __imp__sub_82202750, hooks::TuningCommand30,
             {{-128, -128 + 48}, {-128 + 80, -32}}, Dialog(kCommand30States)});
  // Command7: the wheel x mostly 0 to 3, now and then to 599 (415 + x a
  // named animation).
  Run(h, w, {"TuningScreen::Command7", 20, __imp__sub_821E8B68, hooks::TuningCommand7,
             {{-128, -32}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               guest::StoreU32(w.base, w.screen() + 25196,
                               r.Below(4) == 0 ? r.Below(600) : r.Below(4));
             }});
  // Command23: the movie's +28 table pointing into the block, the set 0 to
  // 2, the pick 0 to 3.
  Run(h, w, {"TuningScreen::Command23", 20, __imp__sub_82202368, hooks::TuningCommand23,
             {{-112, -24}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               guest::StoreU32(w.base, w.at(kMovieAt) + 28, w.at(kTableAt));
               for (uint32_t k = 0; k < 6; k++) {
                 guest::StoreU32(w.base, w.at(kTableAt) + 640 + 4 * k,
                                 w.at(kTargetsAt) + 4 * r.Below(64));
                 guest::StoreU32(w.base, w.at(kTableAt) + 672 + 4 * k,
                                 w.at(kTargetsAt) + 4 * r.Below(64));
               }
               guest::StoreU32(w.base, w.screen() + 30724, r.Below(3));
               guest::StoreU32(w.base, w.screen() + 9640, r.Below(4));
             }});
  // Command18: the slot's word 28 half the time, +30748 mostly small (130
  // on a named animation).
  Run(h, w, {"TuningScreen::Command18", 20, __imp__sub_82200600, hooks::TuningCommand18,
             {{-128, -32}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               const uint32_t s = w.screen();
               const uint32_t slot = r.Below(256);
               guest::StoreU8(w.base, s + 10008, uint8_t(slot));
               if (r.Below(2) == 0) {
                 guest::StoreU32(w.base, s + 9752 + 4 * slot, 28);
               }
               guest::StoreU32(w.base, s + 30748, r.Below(4) == 0 ? r.Below(890) : r.Below(8));
             }});
  // Command25: PlaceStateNodes' states now and then, its rows 0 to 7.
  Run(h, w, {"TuningScreen::Command25", 20, __imp__sub_82204888, hooks::TuningCommand25,
             {{-128, -128 + 48}, {-128 + 80, -32}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               const uint32_t s = w.screen();
               if (r.Below(2) == 0) {
                 guest::StoreU32(w.base, s + ts::kState, 29 + r.Below(3));
               }
               guest::StoreU32(w.base, s + 30724, r.Below(8));
             }});
  // PlayIntro: the scene's and sub-scene 9's animation 0.
  Run(h, w, {"TuningScreen::PlayIntro", 20, __imp__sub_821D06B0, hooks::TuningPlayIntro,
             {{-128, -32}}, [](PPCContext&, Random& r, const World& w) {
               SceneAnimations(w, r);
             }});
  // OpenItems: aero (+9656 4) half the time, the named 976, 1012 and 1013.
  Run(h, w, {"TuningScreen::OpenItems", 25, __imp__sub_821D0A48, hooks::TuningOpenItems,
             {{-128, -32}}, [](PPCContext&, Random& r, const World& w) {
               SceneAnimations(w, r);
               NamedAnimations(w, r);
               guest::StoreU32(w.base, w.screen() + 9656, r.Below(2) == 0 ? 4 : r.Below(8));
             }});
  // Command15: +30724 mostly small (6 on a named animation), now and then
  // negative.
  Run(h, w, {"TuningScreen::Command15", 20, __imp__sub_821FF1D0, hooks::TuningCommand15,
             {{-128, -128 + 48}, {-128 + 80, -32}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               const uint32_t v = r.Below(6) == 0 ? uint32_t(-int32_t(r.Below(5))) : r.Below(8);
               guest::StoreU32(w.base, w.screen() + 30724, v);
             }});
  // Command22: the set 0 to 2, +30732 0 to 2, the word it picks -1 half the
  // time; the movie's +28 table pointing into the block.
  Run(h, w, {"TuningScreen::Command22", 25, __imp__sub_82202090, hooks::TuningCommand22,
             {{-128, -128 + 48}, {-128 + 80, -32}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               guest::StoreU32(w.base, w.at(kMovieAt) + 28, w.at(kTableAt));
               for (uint32_t k = 0; k < 6; k++) {
                 guest::StoreU32(w.base, w.at(kTableAt) + 640 + 4 * k,
                                 w.at(kTargetsAt) + 4 * r.Below(64));
                 guest::StoreU32(w.base, w.at(kTableAt) + 672 + 4 * k,
                                 w.at(kTargetsAt) + 4 * r.Below(64));
               }
               const uint32_t s = w.screen();
               const uint32_t set = r.Below(3), pick = r.Below(3);
               guest::StoreU32(w.base, s + 30724, set);
               guest::StoreU32(w.base, s + 30732, pick);
               if (r.Below(2) == 0) {
                 guest::StoreU32(w.base, s + 31192 + 4 * (3 * set + pick), 0xFFFFFFFFu);
               }
             }});
  // MoveCategory: the menu's step and wrap, the player's mapper (above).
  Run(h, w, {"TuningScreen::MoveCategory", 25, __imp__sub_821D07D8, hooks::TuningMoveCategory,
             {{-128, -32}}, [](PPCContext&, Random& r, const World& w) {
               MoveCategoryWorld(w, r);
             }});
  // MarkPick: states 4 and 53 mostly, [+33548] 0 to 4 (3 the far marks);
  // r28 to r31, f30 and f31 saved (56 bytes).
  Run(h, w, {"TuningScreen::MarkPick", 25, __imp__sub_821D6420, hooks::TuningMarkPick,
             {{-144, -56}}, [](PPCContext&, Random& r, const World& w) {
               ListPage(w, r);
               const uint32_t s = w.screen();
               static constexpr uint32_t kMarkStates[] = {4, 4, 53, 5};
               guest::StoreU32(w.base, s + ts::kState, kMarkStates[r.Below(4)]);
               guest::StoreU32(w.base, s + 33548, r.Below(3) == 0 ? 3 : Small(r, 5));
             }});
  // Command16: +30732 0 to 6, the set 0 half the time.
  Run(h, w, {"TuningScreen::Command16", 20, __imp__sub_821FF6F0, hooks::TuningCommand16,
             {{-128, -128 + 48}, {-128 + 80, -32}}, [](PPCContext&, Random& r, const World& w) {
               NamedAnimations(w, r);
               const uint32_t s = w.screen();
               guest::StoreU32(w.base, s + 30732, r.Below(7));
               guest::StoreU32(w.base, s + 30724, r.Below(2) == 0 ? 0 : r.Below(3));
             }});
  // ShowOptions: tire, aero and exterior mostly (items 4 and 1 half the
  // time), the tire's pairs, [+36612] small; r26 to r31 and f31 saved (64
  // bytes).
  Run(h, w, {"TuningScreen::ShowOptions", 25, __imp__sub_821D5028, hooks::TuningShowOptions,
             {{-160, -64}}, [](PPCContext&, Random& r, const World& w) {
               ListPage(w, r);
               const uint32_t s = w.screen();
               const uint32_t category = r.Below(4) == 0 ? r.Below(8) : 3 + r.Below(3);
               guest::StoreU32(w.base, s + ts::kCategory, category);
               if (r.Below(2) == 0) {
                 guest::StoreU8(w.base, s + ts::kItem, category == 4 ? 4 : category == 5 ? 1 : 0);
               }
               TirePairs(w, r, 35952, 36080, 35948, 36076);
               guest::StoreU32(w.base, s + 36612, Small(r, 8));
             }});
  // ShowLevels: power, power train, body and tire mostly (the extended
  // levels' items half the time), the byte +9677 0 to 12, +25100 0 to 7, the
  // tire's pair.
  Run(h, w, {"TuningScreen::ShowLevels", 25, __imp__sub_821D12C0, hooks::TuningShowLevels,
             {{-160, -64}}, [](PPCContext&, Random& r, const World& w) {
               ListPage(w, r);
               const uint32_t s = w.screen();
               const uint32_t category = r.Below(4) == 0 ? r.Below(8) : r.Below(4);
               guest::StoreU32(w.base, s + ts::kCategory, category);
               if (r.Below(2) == 0) {
                 guest::StoreU8(w.base, s + ts::kItem, category == 1 || category == 2 ? 4 : 0);
               }
               guest::StoreU8(w.base, s + 9677, uint8_t(r.Below(13)));
               guest::StoreU32(w.base, s + 25100, Small(r, 8));
               TirePairs(w, r, 35948, 36076, 35952, 36080);
             }});
  // UpdateItems: the mappers' slots 8 and 11, the states and the set
  // (above); r29 to r31 saved (32 bytes).
  Run(h, w, {"TuningScreen::UpdateItems", 25, __imp__sub_822036E8, hooks::TuningUpdateItems,
             {{-128, -32}}, [](PPCContext& t, Random& r, const World& w) {
               UpdateItemsWorld(w, r);
               if (r.Below(4) == 0) {
                 t.r3.u64 |= r.Next() << 32;
               }
             }});
  // ShowTypes: the pages and the aero parts (above); r27 to r31 and f31
  // saved (56 bytes), SetText's parameter area left out.
  Run(h, w, {"TuningScreen::ShowTypes", 25, __imp__sub_821D2A80, hooks::TuningShowTypes,
             {{-320, -320 + 48}, {-320 + 80, -56}}, [](PPCContext&, Random& r, const World& w) {
               ShowTypesWorld(w, r);
             }});
  // SetStateTexts: r27 to r31 and f31 saved (56 bytes); SetNodeText's and
  // SetText's parameter area (+40 to +79) left out.
  Run(h, w, {"TuningScreen::SetStateTexts", 25, __imp__sub_821F1DE0,
             hooks::TuningSetStateTexts, {{-224, -224 + 40}, {-224 + 80, -56}},
             [](PPCContext&, Random& r, const World& w) { StateTextsWorld(w, r); }});
  // RefreshMenu: its world (above); r25 to r31 and f31 saved (72 bytes);
  // SetNodeText's and SetText's parameter area (+40 to +79) left out; f1
  // compared.
  Run(h, w, {"TuningScreen::RefreshMenu", 40, __imp__sub_821D8978, hooks::TuningRefreshMenu,
             {{-240, -240 + 40}, {-240 + 80, -72}},
             [](PPCContext&, Random& r, const World& w) { RefreshMenuWorld(w, r); }, true});
  // Command8: FillPartRows' world and the named 1007 and 1011; r29 to r31
  // saved (32 bytes); r3's high word now and then set.
  Run(h, w, {"TuningScreen::Command8", 25, __imp__sub_821F5F70, hooks::TuningCommand8,
             {{-128, -32}}, [](PPCContext& t, Random& r, const World& w) {
               Command8World(w, r);
               if (r.Below(4) == 0) {
                 t.r3.u64 |= r.Next() << 32;
               }
             }});
  // FillPartRows: r23 to r31 saved (80 bytes); r3's high word now and then
  // set.
  Run(h, w, {"TuningScreen::FillPartRows", 25, __imp__sub_821F4BF8, hooks::TuningFillPartRows,
             {{-176, -80}}, [](PPCContext& t, Random& r, const World& w) {
               PartRowsWorld(w, r);
               if (r.Below(4) == 0) {
                 t.r3.u64 |= r.Next() << 32;
               }
             }});
  // RefreshMovie13 (last, so the groups before keep their seeds): its world
  // (above); r29 to r31 saved (32 bytes); SetNodeText's and SetText's
  // parameter area (+40 to +79) left out; f1 compared.
  Run(h, w, {"TuningScreen::RefreshMovie13", 40, __imp__sub_822076E8,
             hooks::TuningRefreshMovie13, {{-128, -128 + 40}, {-128 + 80, -32}},
             [](PPCContext&, Random& r, const World& w) { Movie13World(w, r); }, true});
}

}  // namespace harness
