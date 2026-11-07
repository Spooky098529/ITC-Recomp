// The tuning screen (src/tuning_screen/, docs/functions/tuning-screen.md)
// against the recompiled functions. Each case fills a world of its own (a
// block from the allocation layer holding the screen, 37,000 bytes) with
// random words from the case's seed, plus the stack below the call's stack
// pointer and the caller's area above it; shapes the fields the function
// tests (the state, the category and item around their ranges, the picks,
// the table rows); runs the original (with --native_tuning_screen off, so
// what it calls of the screen's is the original too) or the register steps
// (hooks/tuning_screen.h); and compares the block, the stack below the
// original's register saves, the caller's area, r1 and all of r3, exactly
// (nothing here is a float). The screen runs only in the front end, which
// attract races don't reach: these cases are its check.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_screen.h"
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

constexpr uint32_t kScreenBytes = 0x9400;  // the screen (to +37888)
constexpr uint32_t kBlockBytes = 0xA000;
constexpr uint32_t kSaveBytes = 0x1A000;   // the save data (to +0x19924)
constexpr uint32_t kStackBytes = 6144;  // below the call's stack pointer (AeroPartValue's frame is 4,400)
constexpr uint32_t kCallerBytes = 256;  // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// Movie 2's block: the movie (+0, its live scene the first node), the movie
// table's pairs (+0x100) and a pool of 40 nodes (+0x400), each child entry
// of a node another of the pool.
constexpr uint32_t kMovieBytes = 0xC000;
constexpr uint32_t kPairsAt = 0x100;
constexpr uint32_t kNodesAt = 0x400;
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kMissingMovie = 0x826C5CDC;

struct World {
  World() : block(kBlockBytes), save(kSaveBytes), movie(kMovieBytes) {}
  bool ok() const { return block.address != 0 && save.address != 0 && movie.address != 0; }
  uint32_t node(uint32_t n) const { return movie.address + kNodesAt + kNodeBytes * n; }
  uint32_t screen() const { return block.address; }
  Block block;
  Block save;  // the save data the screen's +18904 points at
  Block movie;
  uint8_t* base = nullptr;
};

// A small signed word in [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The fields the screen's functions test: the state mostly 5, the category
// -1 to 7, the item 0 to 11, the picks small (now and then any byte), the
// table rows small.
void ShapeScreen(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + ts::kState, r.Below(3) == 0 ? Around(r, 3, 58) : 5);
  guest::StoreU32(base, s + ts::kCategory, Around(r, -1, 7));
  guest::StoreU8(base, s + ts::kItem, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(12)));
  for (uint32_t k = ts::kLevel; k <= ts::kOption; k++) {
    guest::StoreU8(base, s + k, uint8_t(r.Below(8) == 0 ? r.Next() : r.Below(6)));
  }
  // The row stays small: the tables' index grows with it, and any word would
  // read far outside the world.
  guest::StoreU32(base, s + ts::kPartSet, r.Below(6));
  guest::StoreU32(base, s + 33540, Around(r, 0, 2));
  guest::StoreU32(base, s + 33548, r.Below(10));
  guest::StoreU32(base, s + 33564, r.Below(6));
  guest::StoreU32(base, s + 33572, r.Below(5));
  guest::StoreU32(base, s + 25196, r.Below(16));
  guest::StoreU32(base, s + 25204, r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
  // The debug pages: the front-end menu at +10040 (its +0 the cursor), the
  // line counts, the decal area (the debug lists' index grows with it).
  guest::StoreU32(base, s + 10040, s + 0x9300);
  guest::StoreU32(base, s + 0x9300, r.Below(12));
  guest::StoreU8(base, s + 9677, uint8_t(r.Below(8)));
  guest::StoreU8(base, s + 9678, uint8_t(r.Below(8)));
  guest::StoreU32(base, s + 30724, r.Below(8));
  // The save data's garage list: 51 nodes of 4 bytes from +6120 (the head
  // halfword at +204), each link and the head a node, each slot 0 to 3; the
  // walk [+0x19920] steps; the cars' ids (+12) around the 23 known.
  const uint32_t save = w.save.address, nodes = save + 6120;
  guest::StoreU32(base, s + ts::kSaveData, save);
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t c = nodes + 1968 * k + 212;
    guest::StoreU32(base, c + 12, r.Below(26));
    if (r.Below(3) == 0) {
      guest::StoreU16(base, c + 4, 0xFFFF);
    }
  }
  // StoreToGarageCar's and EquipChoice's choices.
  guest::StoreU32(base, s + 35496, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(7));
  guest::StoreU32(base, s + 35608, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(8));
  guest::StoreU32(base, s + 35948, Around(r, 0, 1));
  guest::StoreU32(base, s + 36076, Around(r, 0, 1));
  guest::StoreU32(base, s + 25172, Around(r, 0, 1));
  guest::StoreU32(base, s + 36300, Around(r, -1, 7));
  guest::StoreU32(base, s + 36372, Around(r, 0, 2));
  guest::StoreU32(base, s + 36376, Around(r, 0, 2));
  guest::StoreU32(base, s + 36824, Around(r, 0, 1));
  guest::StoreU32(base, s + 36708, Around(r, 0, 5));
  guest::StoreU32(base, s + 36712, Around(r, 0, 4));
  guest::StoreU32(base, s + 33616, Around(r, 0, 4));
  guest::StoreU32(base, s + 33620, Around(r, 0, 6));
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t layer = s + 30580 + 4 * i;
    guest::StoreU32(base, layer + 0, Around(r, 0, 2));
    guest::StoreU32(base, layer + 80, Around(r, 0, 2));
    // The mirror table's index stays 0 to 7: past it the original reads its
    // own register saves, which the native version doesn't write.
    guest::StoreU32(base, layer + 64, r.Below(8));
    if (r.Below(2) == 0) {
      for (uint32_t f : {16u, 32u, 48u, 112u}) {
        guest::StoreU32(base, layer + f, uint32_t(int32_t(r.Below(721)) - 360));
      }
    }
  }
  // The stickers: -1 for none, or a number whose tens stay below 16 (past
  // that the original's tens table reaches its register saves).
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, s + 33800 + 4 * i,
                    r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(int32_t(r.Below(260)) - 100));
  }
  // Category 5's item 6 (the stickers) now and then.
  if (r.Below(4) == 0) {
    guest::StoreU32(base, s + ts::kCategory, 5);
    guest::StoreU8(base, s + ts::kItem, 6);
  }
}

// Movie 2 and its nodes, and the movie table pointed at its pair (now and
// then not found: UiMovies::Find answers the missing movie's word, the same
// movie).
void ShapeMovie(uint8_t* base, const World& w, Random& r) {
  const uint32_t movie = w.movie.address;
  guest::StoreU32(base, movie + 16, w.node(0));
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 108, r.Below(20));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
  }
  guest::StoreU32(base, movie + kPairsAt, 2);
  guest::StoreU32(base, movie + kPairsAt + 4, movie);
  guest::StoreU32(base, ui_movies::kMovieTable, movie + kPairsAt);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 1);
  guest::StoreU32(base, kMissingMovie, movie);
}

// The globals a case points into its world, put back after.
class SavedGlobals {
 public:
  explicit SavedGlobals(uint8_t* base)
      : base_(base), table_(Read(base, ui_movies::kMovieTable, 12)),
        missing_(Read(base, kMissingMovie, 4)) {}
  ~SavedGlobals() {
    Write(base_, ui_movies::kMovieTable, table_.data(), 12);
    Write(base_, kMissingMovie, missing_.data(), 4);
  }
  SavedGlobals(const SavedGlobals&) = delete;
  SavedGlobals& operator=(const SavedGlobals&) = delete;

 private:
  uint8_t* base_;
  std::vector<uint8_t> table_;
  std::vector<uint8_t> missing_;
};

// A small index for r4 (all 64 bits), now and then with upper bits.
uint64_t SmallIndex(Random& r, uint32_t hi) {
  uint64_t v = r.Below(hi + 1);
  if (r.Below(6) == 0) {
    v |= r.Next() << 32;
  }
  return v;
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;

// Stack bytes left out of the comparison besides the top register saves:
// [from, to) bytes below the call's stack pointer (a callee's register
// saves, which its native version doesn't write; a varargs callee's stores
// into its caller's parameter area).
using Holes = std::vector<std::pair<uint32_t, uint32_t>>;

// `saves` the original's register saves' bytes below the stack pointer, left
// out of the comparison.
void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, uint32_t saves,
          const std::string& what, const Holes& holes = {}) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    WriteRandom(h.base, w.block.address, w.block.bytes, r);
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    WriteRandom(h.base, w.movie.address, w.movie.bytes, r);
    SavedGlobals globals(h.base);
    ShapeScreen(h.base, w, r);
    ShapeMovie(h.base, w, r);
    t.r3.u64 = w.screen();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    if (v == 0) {
      const bool flag = ITC_FLAG(native_tuning_screen);
      ITC_FLAG(native_tuning_screen) = false;
      recompiled(t, h.base);
      ITC_FLAG(native_tuning_screen) = flag;
    } else {
      native(t, h.base);
    }
    for (const auto& [from, to] : holes) {
      for (uint32_t a = sp - to; a < sp - from; a++) {
        guest::StoreU8(h.base, a, 0);
      }
    }
    out[v].Bytes(Read(h.base, w.block.address, w.block.bytes));
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, w.movie.address, w.movie.bytes));
    out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - saves));
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, uint32_t saves,
           const Holes& holes = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, saves,
         fmt::format("case {}", c), holes);
  }
  results.Report();
}

const SetupFn kScreenOnly = [](PPCContext&, Random&, const World&) {};

}  // namespace

void TuningScreen(Context& h) {
  World w;
  w.base = h.base;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_screen: no memory for the world");
    return;
  }
  Group(h, w, "TuningScreen::CategoryItemCount", 24, kScreenOnly, __imp__sub_821CC6C0,
        hooks::TuningCategoryItemCount, 0);
  Group(
      h, w, "TuningScreen::IsItemAvailable", 40,
      [](PPCContext& t, Random& r, const World&) { t.r4.u64 = SmallIndex(r, 12); },
      __imp__sub_821CDB18, hooks::TuningIsItemAvailable, 0);
  Group(
      h, w, "TuningScreen::WheelNumber", 30,
      [](PPCContext& t, Random& r, const World&) {
        t.r4.u64 = r.Below(5) == 0 ? r.Next() : r.Below(18);
        t.r5.u64 = r.Below(3) == 0 ? r.Next() : r.Below(40);
      },
      __imp__sub_821CDED8, hooks::TuningWheelNumber, 0);
  Group(h, w, "TuningScreen::GetPrice", 40, kScreenOnly, __imp__sub_821CD160,
        hooks::TuningGetPrice, 0);
  Group(h, w, "TuningScreen::GetCurrentChoice", 40, kScreenOnly, __imp__sub_821CD868,
        hooks::TuningGetCurrentChoice, 0);
  Group(
      h, w, "TuningScreen::GetChoiceFlag", 40,
      [](PPCContext& t, Random& r, const World&) { t.r4.u64 = SmallIndex(r, 10); },
      __imp__sub_821CDC38, hooks::TuningGetChoiceFlag, 0);

  // The garage car: StoreToGarageCar's register saves (__savegprlr_25)
  // left out; its floats are stored with float instructions but compared
  // exactly (the products of ints, never NaN).
  Group(h, w, "TuningScreen::StoreToGarageCar", 40, kScreenOnly, __imp__sub_821DCC28,
        hooks::TuningStoreToGarageCar, 64);
  Group(
      h, w, "TuningScreen::EquipChoice", 40,
      [](PPCContext&, Random& r, const World& w) {
        // States 5 to 8 most of the time.
        guest::StoreU32(w.base, w.screen() + ts::kState, r.Below(6) == 0 ? Around(r, 3, 12)
                                                                          : 5 + r.Below(4));
      },
      __imp__sub_821DF448, hooks::TuningEquipChoice, 0);

  // AeroPartValue: the types 0 to 3 (r4 -1, the callers' type 0, reads a
  // word the original never writes as a pointer), the items 0 to 9, with
  // upper bits now and then; its register saves (__savegprlr_25) left out.
  Group(
      h, w, "TuningScreen::AeroPartValue", 40,
      [](PPCContext& t, Random& r, const World&) {
        t.r4.u64 = SmallIndex(r, 3);
        t.r5.u64 = SmallIndex(r, 9);
      },
      __imp__sub_821DE2D8, hooks::TuningAeroPartValue, 64);

  // The part list's marks (__savegprlr_28's saves left out). ShowPartMarks:
  // a row node of the pool, a wheel, and the screen's front and rear wheel
  // numbers matching it half the time each.
  Group(h, w, "TuningScreen::HidePartMarks", 24, kScreenOnly, __imp__sub_821EE6D0,
        hooks::TuningHidePartMarks, 40);
  Group(
      h, w, "TuningScreen::ShowPartMarks", 40,
      [](PPCContext& t, Random& r, const World& w) {
        t.r4.u64 = w.node(r.Below(kNodeCount));
        t.r5.u64 = r.Below(8) == 0 ? r.Next() : r.Below(18);
        t.r6.u64 = r.Below(8) == 0 ? r.Next() : r.Below(40);
        const uint32_t number = uint32_t(ts::WheelNumber(t.r5.u64, t.r6.u64));
        guest::StoreU32(w.base, w.screen() + 25196, Around(r, 0, 3));
        for (uint32_t field : {27836u, 27840u}) {
          if (r.Below(2) == 0) {
            guest::StoreU32(w.base, w.screen() + field, number);
          }
        }
      },
      __imp__sub_821EE760, hooks::TuningShowPartMarks, 40);

  // Movie 2's node setters (savegprlr_29's, r31 and lr's, savegprlr_22's
  // saves left out). The states and categories they test, the tire's and
  // the interior's mostly; the tire's choices equal half the time.
  auto tire = [](PPCContext&, Random& r, const World& w) {
    const uint32_t s = w.screen();
    constexpr uint32_t kStates[] = {5, 8, 9, 10, 11, 6, 7};
    guest::StoreU32(w.base, s + ts::kState, kStates[r.Below(7)]);
    guest::StoreU32(w.base, s + ts::kCategory, r.Below(4) == 0 ? Around(r, -1, 7) : 3);
    guest::StoreU8(w.base, s + ts::kItem, uint8_t(r.Below(4)));
    for (const uint32_t pair : {35948u, 35952u}) {
      guest::StoreU32(w.base, s + pair, r.Below(4));
      guest::StoreU32(w.base, s + pair + 128,
                      r.Below(2) == 0 ? guest::LoadU32(w.base, s + pair) : r.Below(4));
    }
    guest::StoreU8(w.base, s + ts::kLevel, uint8_t(r.Below(4)));
  };
  Group(
      h, w, "TuningScreen::ShowInteriorPicture", 20,
      [](PPCContext& t, Random& r, const World& w) {
        t.r4.u64 = r.Below(3) == 0 ? 0 : (r.Below(4) == 0 ? r.Next() : r.Below(5));
        t.r5.u64 = r.Below(4);
        guest::StoreU8(w.base, w.screen() + ts::kType, uint8_t(r.Below(6)));
      },
      __imp__sub_821DA3F8, hooks::TuningShowInteriorPicture, 32);
  Group(h, w, "TuningScreen::HideTireMarks", 15, kScreenOnly, __imp__sub_821DFB20,
        hooks::TuningHideTireMarks, 32);
  Group(
      h, w, "TuningScreen::HideNode19", 25,
      [](PPCContext&, Random& r, const World& w) {
        const uint32_t s = w.screen();
        constexpr uint32_t kStates[] = {5, 6, 8, 54, 56, 9, 10, 11, 14, 15, 7, 16};
        guest::StoreU32(w.base, s + ts::kState, kStates[r.Below(12)]);
        guest::StoreU32(w.base, s + ts::kCategory, r.Below(7));
        guest::StoreU8(w.base, s + ts::kItem, uint8_t(r.Below(5)));
        for (uint32_t k = ts::kLevel; k <= ts::kOption; k++) {
          guest::StoreU8(w.base, s + k, uint8_t(r.Below(3)));
        }
        guest::StoreU32(w.base, s + 25172, r.Below(2));
        guest::StoreU32(w.base, s + 33548, r.Below(2));
        guest::StoreU32(w.base, s + 33564, 3 + r.Below(3));
      },
      __imp__sub_821DFBE8, hooks::TuningHideNode19, 16);
  // ShowTireMarks: HideTireMarks' saves (its frame 128 bytes below) out too.
  Group(h, w, "TuningScreen::ShowTireMarks", 25, tire, __imp__sub_821E22E8,
        hooks::TuningShowTireMarks, 88, {{176, 176 + 32}});

  // The debug state handlers: `saves` is each original's register saves
  // (__savegprlr_N's 8 x (32 - N) + 8 bytes, or its inline stores).
  struct Handler {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t saves;
  };
  const Handler handlers[] = {
      {"TuningScreen::DebugMainMenu", __imp__sub_821CBE08, hooks::TuningDebugMainMenu, 32},
      {"TuningScreen::DebugLevelSelect", __imp__sub_821CBEE8, hooks::TuningDebugLevelSelect, 40},
      {"TuningScreen::DebugTypeSelect", __imp__sub_821CBFC0, hooks::TuningDebugTypeSelect, 40},
      {"TuningScreen::DebugOptionSelect", __imp__sub_821CC248, hooks::TuningDebugOptionSelect,
       16},
      {"TuningScreen::DebugConfirmBuy", __imp__sub_821CC3A8, hooks::TuningDebugConfirmBuy, 8},
      {"TuningScreen::DebugConfirmEquip", __imp__sub_821CC488, hooks::TuningDebugConfirmEquip,
       8},
      {"TuningScreen::DebugCategoryMenu", __imp__sub_821CFCE8, hooks::TuningDebugCategoryMenu,
       152},
      {"TuningScreen::DebugWheelPartSelect", __imp__sub_821E8068,
       hooks::TuningDebugWheelPartSelect, 40},
      {"TuningScreen::DebugWheelKindSelect", __imp__sub_821E8128,
       hooks::TuningDebugWheelKindSelect, 104},
      {"TuningScreen::DebugWheelInchSelect", __imp__sub_821E8298,
       hooks::TuningDebugWheelInchSelect, 32},
      {"TuningScreen::DebugDecalTopMenu", __imp__sub_821FD710, hooks::TuningDebugDecalTopMenu,
       32},
      {"TuningScreen::DebugLayerSelect", __imp__sub_821FD880, hooks::TuningDebugLayerSelect, 56},
      {"TuningScreen::DebugLayerEdit", __imp__sub_821FDCB8, hooks::TuningDebugLayerEdit, 48},
      {"TuningScreen::DebugLayerSet", __imp__sub_821FE080, hooks::TuningDebugLayerSet, 24},
      {"TuningScreen::DebugEditPosition", __imp__sub_821FE100, hooks::TuningDebugEditPosition, 0},
      {"TuningScreen::DebugEditView", __imp__sub_821FE178, hooks::TuningDebugEditView, 0},
      {"TuningScreen::DebugPaintConfirm", __imp__sub_821FE190, hooks::TuningDebugPaintConfirm,
       32},
  };
  for (const Handler& handler : handlers) {
    Group(h, w, handler.name, 20, kScreenOnly, handler.recompiled, handler.native,
          handler.saves);
  }
}

}  // namespace harness
