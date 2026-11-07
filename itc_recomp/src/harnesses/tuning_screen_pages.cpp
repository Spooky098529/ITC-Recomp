// The tuning screen's functions over movies 2 and 13's animations and texts
// (src/tuning_screen/pages.cpp, docs/functions/tuning-screen.md) against the
// recompiled ones, part of the tuning_screen harness. Its world, laid as the
// build screen's harness lays movie 6 (src/harnesses/build_screen.cpp): one
// block holding the movie (both ids' pairs naming it), a pool of 40 TXT
// nodes (each child of a node another of the pool), the transitions' scene
// root and 8 animations of no tracks (the named animations over them, the
// animation lists set up), the screen and its command queue; the save data
// in a block of its own; a string table of our own (letters, newlines and
// page breaks, no argument codes: the varargs native callers pass as 0
// aren't read). Each case fills it with random words from its seed, lays
// it, points the globals into it (put back after), runs the original (with
// --native_tuning_screen off) or the register steps (hooks/tuning_screen.h),
// and compares the text records SetText made (their words and buffers,
// freed), the globals, the blocks, the stack below r1 (without the
// original's register saves and the parameter areas the varargs callees
// store into), the caller's area, r1 and all of r3, exactly.

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
constexpr uint32_t kBlockBytes = 0x1FC00;
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
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

struct World {
  explicit World(uint8_t* base)
      : base(base), allocation(memory::Alloc(kBlockBytes + 16, 0)),
        block((allocation + 15) & ~15u), save(memory::Alloc(kSaveBytes, 0)),
        strings(memory::Alloc(kStringsBytes, 0)) {
    if (strings != 0) {
      MakeStrings();
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

 private:
  void MakeStrings() {
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
};

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  WriteRandom(base, w.save, kSaveBytes, r);
  const uint32_t movie = w.at(kMovieAt);
  // The nodes: TXT tags, their first child's index small, their children
  // the pool's.
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(60));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(20));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  // The movie: its animation lists set up, its scenes the root, its 128
  // named animations over the root and one of the 8 animations.
  const uint32_t root = w.at(kRootAt);
  ui_movies::InitAnimationLists(base, movie);
  guest::StoreU32(base, movie + 16, w.node(0));
  guest::StoreU32(base, movie + 20, root);
  guest::StoreU32(base, movie + 24, root);
  for (uint32_t k = 0; k < 128; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, root);
    guest::StoreU32(base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(8));
  }
  for (uint32_t i = kZeroAt; i < kScreenAt; i += 4) {
    guest::StoreU32(base, w.at(i), 0);
  }
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
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
    guest::StoreU32(base, root + 128 + 8 * i, w.at(kAnimationsAt) + 64 * r.Below(8));
  }
  // The texts' map empty, the text count small.
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  // Both ids' pairs.
  guest::StoreU32(base, w.at(kPairsAt), 2);
  guest::StoreU32(base, w.at(kPairsAt) + 4, movie);
  guest::StoreU32(base, w.at(kPairsAt) + 8, 13);
  guest::StoreU32(base, w.at(kPairsAt) + 12, movie);
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
      TextRecords(h.base, w, out[v]);
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
void NamedAnimations(const World& w, Random& r) {
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t k = 128; k < 1024; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(w.base, named + 28, w.at(kRootAt));
    guest::StoreU32(w.base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(8));
  }
}

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

constexpr uint32_t kShowDialogStates[] = {9, 10, 11, 12, 13, 14};
constexpr uint32_t kCommand30States[] = {35, 36, 37};

}  // namespace

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
}

}  // namespace harness
