// The car list (src/car_list/, docs/functions/car-list.md) against the
// recompiled functions. Each case fills a world of our own with random
// words from the case's seed (mixed with splitmix64: the harness
// generator's consecutive seeds are correlated): the objects block (the
// panel at +0, its two panel groups at +0x200 and +0x240, the C panel's
// vtable at +0x300, a record at +0x400, the colors' list at +0x800 and its
// records at +0x900), a car list entry (8,448 bytes), the stack below the
// call's stack pointer and the caller's area above it; shapes the fields
// the function tests (the indices small or -1, the groups' movie ids 7 or
// 14); runs the original (with --native_car_list off) or the register steps
// (hooks/car_list.h); and compares the world, the movie world, the
// original's frame (its register saves and the varargs words SetText and
// SetNodeText store, +40 to +79, left out), the caller's area, r1 and r3,
// all exactly (the floats the functions store are converted from integers
// or copied as the originals' lfs and stfs do).
//
// The UI movie world: two movies (ids 7 and 14 in the movie table; the word
// UiMovies::Find answers for a missing one movie 7's) with empty animation
// lists, their live, saved and file scenes one tree's root and their 900
// named animations over it, each one of 8 animations of no tracks; the
// tree's 48 nodes all TXT nodes (UiMovie::SetText's assert) whose child
// index (+108, also their text id) is 0 to 8 and whose entries (from -1)
// are nodes of the tree, their children's range +100 below 16 and +104 0 to 6; each
// movie's texts ids 1 to 7 and the string table of letters only (no
// argument codes: the user's call, 2026-10-30). The callees run for real on
// both sides (the originals' and ours reach the same overrides of the UI
// movie calls).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_list/car_list.h"
#include "hooks/car_list.h"
#include "memory/heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_list);

namespace harness {

namespace {

constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

class SavedBytes {
 public:
  SavedBytes(uint8_t* base, std::vector<std::pair<uint32_t, uint32_t>> ranges)
      : base_(base), ranges_(std::move(ranges)) {
    for (const auto& [address, bytes] : ranges_) {
      saved_.push_back(Read(base_, address, bytes));
    }
  }
  ~SavedBytes() {
    for (size_t i = 0; i < ranges_.size(); i++) {
      std::memcpy(base_ + ranges_[i].first, saved_[i].data(), ranges_[i].second);
    }
  }

 private:
  uint8_t* base_;
  std::vector<std::pair<uint32_t, uint32_t>> ranges_;
  std::vector<std::vector<uint8_t>> saved_;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

Random Mixed(uint64_t seed) {
  uint64_t z = seed + 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return Random(z ^ (z >> 31));
}

// The movie world.
constexpr uint32_t kMovieHeaderBytes = 0xA000;
constexpr uint32_t kMovieCount = 2;
constexpr uint32_t kTreeNodeBytes = 0x800;  // 240 entries: RowPanel::SetRow stores a logo frame (up to 79) at a node's +108
constexpr uint32_t kNodes = 48, kAnimations = 8;
constexpr uint32_t kNodesAt = kMovieCount * kMovieHeaderBytes;
constexpr uint32_t kAnimationsAt = kNodesAt + kNodes * kTreeNodeBytes;
constexpr uint32_t kPairsAt = kAnimationsAt + kAnimations * 64;
constexpr uint32_t kTexturesAt = kPairsAt + 0x40;
constexpr uint32_t kMovieWorldBytes = kTexturesAt + 0x100;
constexpr uint32_t kTextsPerMovie = 8;
constexpr uint32_t kTextBuffersAt = 0x1000;
constexpr uint32_t kTextWorldBytes = kTextBuffersAt + kMovieCount * kTextsPerMovie * 0x1000;
constexpr uint32_t kStringWorldBytes = 0x10000;
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTagTxt = 0x54585400;

// The objects block.
constexpr uint32_t kObjectsBytes = 0x1000;
constexpr uint32_t kGroupA = 0x200, kGroupB = 0x240, kVtable = 0x300, kRecord = 0x400;
constexpr uint32_t kColors = 0x800, kColorRecords = 0x900;
// A car list (128 entry pointers, +512 the count) at +0xA04, the two words
// before it entry pointers too (RowPanel::Move reads the word before the
// scroll's entry).
constexpr uint32_t kList = 0xA04;
constexpr uint32_t kEntryBytes = 8448;
constexpr uint32_t kSetBytes = 0x1000;

struct World {
  World()
      : objects(kObjectsBytes),
        entry(kEntryBytes),
        movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes),
        set(kSetBytes) {}
  bool ok() const {
    return objects.address != 0 && entry.address != 0 && movies.address != 0 &&
           texts.address != 0 && strings.address != 0 && set.address != 0;
  }
  uint32_t b() const { return set.address + 4; }
  uint32_t movie(uint32_t m) const { return movies.address + kMovieHeaderBytes * m; }
  uint32_t node(uint32_t k) const { return movies.address + kNodesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  uint32_t panel() const { return objects.address; }
  uint32_t at(uint32_t offset) const { return objects.address + offset; }
  Block objects;
  Block entry;
  Block movies;
  Block texts;
  Block strings;
  Block set;  // a PanelSet at +0 (its base b at +4)
};

const std::vector<std::pair<uint32_t, uint32_t>> kSavedGlobals = {
    {ui_movies::kMovieTable, 12}, {kMissingMovie, 4}, {kMissingText, 4}, {kStringTable, 4},
    {car_list::kTwoPlayers, 4},   {car_list::kPowerGraph, 4},
    {car_list::kFadeWait, 4},     {0x826A50AC, 64}};  // the screen fades' words

// The string table: 17 files' pointers to one container of entries enough
// for the widest file (the bounds at 0x82007E18), each one of 8 strings of
// letters (no argument codes), as the options screen's harness builds it.
void BuildStringTable(uint8_t* base, const World& w) {
  constexpr uint32_t kBounds = 0x82007E18;
  uint32_t span = 1;
  for (uint32_t i = 0; i < 17; i++) {
    const int32_t from = int32_t(guest::LoadU32(base, kBounds + 4 * i));
    const int32_t to = int32_t(guest::LoadU32(base, kBounds + 4 * i + 4));
    if (to > from && uint32_t(to - from) > span) {
      span = uint32_t(to - from);
    }
  }
  const uint32_t at = w.strings.address, container = at + 0x100;
  if (container + 4 + 4 * span + 8 * 64 > at + w.strings.bytes) {
    ITC_LOG_ERROR("harness: car_list: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x4361724C697374ull);  // "CarList"
  const uint32_t text = container + 4 + 4 * span;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t length = r.Below(30);
    for (uint32_t c = 0; c < length; c++) {
      guest::StoreU16(base, text + 64 * k + 2 * c, uint16_t('A' + r.Below(26)));
    }
    guest::StoreU16(base, text + 64 * k + 2 * length, 0);
  }
  for (uint32_t i = 0; i < span; i++) {
    guest::StoreU32(base, container + 4 + 4 * i,
                    __builtin_bswap32(text + 64 * r.Below(8) - container));
  }
}

void ShapeMovieWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.movies.address, w.movies.bytes, r);
  WriteRandom(base, w.texts.address, w.texts.bytes, r);
  for (uint32_t m = 0; m < kMovieCount; m++) {
    const uint32_t movie = w.movie(m);
    ui_movies::InitAnimationLists(base, movie);
    for (const uint32_t field : {16u, 20u, 24u}) {
      guest::StoreU32(base, movie + field, w.node(0));
    }
    guest::StoreU32(base, movie + 28, w.movies.address + kTexturesAt);
    for (uint32_t k = 0; k < 900; k++) {
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, w.node(0));
      guest::StoreU32(base, named + 32, w.animation(r.Below(kAnimations)));
    }
    for (uint32_t k = 0; k < kTextsPerMovie; k++) {
      const uint32_t record = w.texts.address + 0x100 * m + 32 * k;
      const uint32_t buffer = w.texts.address + kTextBuffersAt + 0x1000 * (kTextsPerMovie * m + k);
      guest::StoreU32(base, record, buffer);
      guest::StoreU32(base, record + 20, r.Below(5) - 1);
      const uint32_t length = r.Below(60);
      for (uint32_t c = 0; c < length; c++) {
        guest::StoreU16(base, buffer + 2 * c,
                        r.Below(6) == 0 ? 0xFFD1 : uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, buffer + 2 * length, 0);
      if (k + 1 < kTextsPerMovie) {
        guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k, k + 1);
        guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k + 4, record);
      }
    }
    guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, kTextsPerMovie - 1);
    guest::StoreU32(base, movie + ui_movies::kMovieTextCount, kTextsPerMovie);
  }
  guest::StoreU32(base, kMissingText, w.texts.address + 32 * (kTextsPerMovie - 1));
  for (uint32_t k = 0; k < kAnimations; k++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, w.animation(k) + i, 0);
    }
    guest::StoreFloat(base, w.animation(k) + 12, float(r.Below(100)) / 4.0f);
  }
  constexpr uint32_t kEntries = (kTreeNodeBytes - 128) / 8;
  for (uint32_t k = 0; k < kNodes; k++) {
    const uint32_t node = w.node(k);
    guest::StoreU32(base, node, kTagTxt);
    guest::StoreU32(base, node + 100, r.Below(16));
    guest::StoreU32(base, node + 104, r.Below(8) == 0 ? 0 : r.Below(7));
    guest::StoreU32(base, node + 108, r.Below(9));
    // Entry -1 (+120) a node too: a -1 index with +108 0 reads it.
    guest::StoreU32(base, node + 120, w.node(r.Below(kNodes)));
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, node + 128 + 8 * i, w.node(r.Below(kNodes)));
    }
  }
  const uint32_t pairs = w.movies.address + kPairsAt;
  guest::StoreU32(base, pairs, 7);
  guest::StoreU32(base, pairs + 4, w.movie(0));
  guest::StoreU32(base, pairs + 8, 14);
  guest::StoreU32(base, pairs + 12, w.movie(1));
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, kMovieCount);
  guest::StoreU32(base, kMissingMovie, w.movie(0));
  guest::StoreU32(base, kStringTable, w.strings.address);
}

// An index: 0 to 63, now and then -1.
uint32_t Index(Random& r) { return r.Below(6) == 0 ? 0xFFFFFFFFu : r.Below(64); }

// The world's shape: the objects random, then every panel word an index;
// the groups' ids 7 or 14, their +12 a sub-scene (0 to 63, now and then
// -1) and +32 an index; the C panel's vtable (slots 1 and 2 SetStats and
// SetCarName, now and then the stand-in sub_820CC470, which stores r4 at
// r3 + 20); the colors' list (count 0 to 13, its records' pointers); the
// entry's car id 0 to 79 and the spec's bytes +769 0 to 9.
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.set.address, w.set.bytes, r);
  WriteRandom(base, w.objects.address, w.objects.bytes, r);
  WriteRandom(base, w.entry.address, w.entry.bytes, r);
  for (uint32_t i = 0; i < 0x200; i += 4) {
    guest::StoreU32(base, w.panel() + i, Index(r));
  }
  for (const uint32_t g : {kGroupA, kGroupB}) {
    guest::StoreU32(base, w.at(g) + 4, r.Below(2) == 0 ? 7 : 14);
    guest::StoreU32(base, w.at(g) + 12, Index(r));
    guest::StoreU32(base, w.at(g) + 32, Index(r));
  }
  constexpr uint32_t kStandIn = 0x820CC470;
  guest::StoreU32(base, w.at(kVtable) + 4, r.Below(4) == 0 ? kStandIn : car_list::kSetStats);
  guest::StoreU32(base, w.at(kVtable) + 8, r.Below(4) == 0 ? kStandIn : car_list::kSetCarName);
  const uint32_t count = r.Below(14);
  guest::StoreU32(base, w.at(kColors) + 52, count);
  for (uint32_t i = 0; i < 13; i++) {
    guest::StoreU32(base, w.at(kColors) + 4 * i, w.at(kColorRecords) + 32 * i);
  }
  for (uint32_t i = 0; i <= 129; i++) {
    guest::StoreU32(base, w.at(kList) - 8 + 4 * i, w.entry.address);
  }
  guest::StoreU32(base, w.at(kList) + 512, r.Below(21));
  guest::StoreU32(base, car_list::kTwoPlayers, 0);
  guest::StoreU32(base, car_list::kPowerGraph, w.at(kRecord));
  guest::StoreU32(base, w.entry.address + 8432, r.Below(80));
  guest::StoreU8(base, w.entry.address + 769, uint8_t(r.Below(10)));
  ShapeMovieWorld(base, w, r);
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read (blocks the run
// allocated or freed compared by kind and put back).
using AfterFn = std::function<void(const World&, Outputs&)>;

// A function's frame: its size and the register saves at its top.
struct Frame {
  uint32_t size, saves;
};

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, Frame frame, const std::string& what,
          const AfterFn& after) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r = Mixed(seed);
    SavedBytes saved(h.base, kSavedGlobals);
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    ShapeWorld(h.base, w, r);
    t.r3.u64 = w.panel() | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
    for (PPCRegister* reg : {&t.r4, &t.r5, &t.r6, &t.r7}) {
      reg->u64 = r.Next();
    }
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    ITC_FLAG(native_car_list) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_list) = true;
    if (after) {
      after(w, out[v]);
    }
    out[v].Bytes(Read(h.base, w.set.address, w.set.bytes));
    out[v].Bytes(Read(h.base, w.objects.address, w.objects.bytes));
    out[v].Bytes(Read(h.base, w.entry.address, w.entry.bytes));
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.texts.address, w.texts.bytes));
    for (const auto& [address, bytes] : kSavedGlobals) {
      out[v].Bytes(Read(h.base, address, bytes));
    }
    if (frame.size != 0) {
      const uint32_t bottom = sp - frame.size;
      out[v].Bytes(Read(h.base, bottom, 40));
      out[v].Bytes(Read(h.base, bottom + 80, frame.size - 80 - frame.saves));
    } else {
      out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes));  // a leaf: none written
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, Frame frame,
           const SetupFn& setup, GuestFunction recompiled, GuestFunction native,
           const AfterFn& after = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, frame,
         fmt::format("case {}", c), after);
  }
  results.Report();
}

// The panel's group pointer at `at` (A or B).
void SetGroup(uint8_t* base, const World& w, Random& r, uint32_t at) {
  guest::StoreU32(base, w.panel() + at, w.at(r.Below(2) == 0 ? kGroupA : kGroupB));
}

// r4 a small signed word (0 to 15, now and then -1 or any word).
uint64_t Small(Random& r) {
  switch (r.Below(8)) {
    case 0:
      return 0xFFFFFFFFFFFFFFFFull;
    case 1:
      return r.Next();
    default:
      return r.Below(16);
  }
}

void Panels(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const int n = 40;
  const auto group = [base](uint32_t at) {
    return [base, at](PPCContext&, Random& r, const World& w) { SetGroup(base, w, r, at); };
  };
  // PanelGroup::SetCaption: r3 a group, r4 a string id below 2,000.
  Group(
      h, w, "PanelGroup::SetCaption (sub_8224EF18)", n, {112, 32},
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.at(r.Below(2) == 0 ? kGroupA : kGroupB);
        t.r4.u64 = r.Below(2000);
      },
      __imp__sub_8224EF18, hooks::CarListSetCaption);
  Group(h, w, "PanelPart::FinishAll (sub_82249CF0)", 16, {96, 8}, group(4), __imp__sub_82249CF0,
        hooks::CarListPartFinishAll);
  Group(
      h, w, "TabPanel::SetCursorY (sub_8224FD20)", n, {112, 24},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : 40 * r.Below(3);
      },
      __imp__sub_8224FD20, hooks::CarListTabSetCursorY);
  Group(h, w, "TabPanel::SetButtons (sub_8224FD90)", n, {112, 24}, group(4),
        __imp__sub_8224FD90, hooks::CarListTabSetButtons);
  // The rows: r4 the row (0 to 7, now and then -1), r5 the entry.
  Group(
      h, w, "RowPanel::SetRow (sub_8224F018)", 80, {128, 48},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 0);
        t.r4.u64 = r.Below(6) == 0 ? 0xFFFFFFFFFFFFFFFFull : r.Below(8);
        // The rows' nodes (+20 to +48, +60) not negative: without one the
        // original reads a child of node 0.
        for (uint32_t at = 20; at <= 60; at += 4) {
          guest::StoreU32(base, w.panel() + at, r.Below(64));
        }
        t.r5.u64 = w.entry.address;
        // The car ids the logo's frame maps.
        static constexpr uint32_t kIds[] = {0, 2, 34, 35, 43, 46, 47, 56, 57, 71, 72, 79};
        guest::StoreU32(base, w.entry.address + 8432,
                        r.Below(2) == 0 ? kIds[r.Below(12)] : r.Below(80));
      },
      __imp__sub_8224F018, hooks::CarListRowSetRow);
  Group(
      h, w, "RowPanel::SetArrows (sub_8224F680)", n, {128, 48},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 0);
        t.r4.u64 = Small(r);
        t.r5.u64 = r.Below(2) == 0 ? guest::LoadU32(base, w.panel() + 4) + uint32_t(t.r4.u64)
                                   : Small(r);
      },
      __imp__sub_8224F680, hooks::CarListRowSetArrows);
  Group(
      h, w, "RowPanel::Unhighlight (sub_8224F558)", n, {112, 24},
      [base](PPCContext&, Random& r, const World& w) {
        SetGroup(base, w, r, 0);
        guest::StoreU32(base, w.panel() + 156, r.Below(2));
      },
      __imp__sub_8224F558, hooks::CarListRowUnhighlight);
  Group(h, w, "RowPanel::Hide (sub_8224F4C8)", n, {112, 24}, group(0), __imp__sub_8224F4C8,
        hooks::CarListRowHide);
  // The colors.
  Group(h, w, "ColorPanel::Show (sub_8224D010)", n, {112, 24}, group(4), __imp__sub_8224D010,
        hooks::CarListColorShow);
  Group(
      h, w, "ColorPanel::Select (sub_8224D118)", n, {112, 32},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = r.Below(13);
      },
      __imp__sub_8224D118, hooks::CarListColorSelect);
  Group(
      h, w, "ColorPanel::SetArrows (sub_8224D2D0)", n, {128, 40},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = r.Below(3) == 0 ? r.Next() : r.Below(2);
        t.r5.u64 = r.Below(3) == 0 ? t.r4.u64 : r.Below(2);
      },
      __imp__sub_8224D2D0, hooks::CarListColorSetArrows);
  Group(
      h, w, "ColorPanel::ShowSwatches (sub_8224D358)", n, {128, 40},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = Small(r);
      },
      __imp__sub_8224D358, hooks::CarListColorShowSwatches);
  Group(
      h, w, "ColorPanel::PlaceCursor (sub_8224D3E0)", n, {128, 32},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = Small(r);
      },
      __imp__sub_8224D3E0, hooks::CarListColorPlaceCursor);
  Group(
      h, w, "ColorPanel::SetCount (sub_8224D458)", n, {96, 16},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        guest::StoreU32(base, w.panel() + 88, r.Below(2));
        t.r4.u64 = Small(r);
      },
      __imp__sub_8224D458, hooks::CarListColorSetCount);
  Group(
      h, w, "ColorPanel::SetSwatch (sub_8224D4A0)", n, {128, 40},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = r.Below(8);
        t.r5.u64 = w.at(kColorRecords) + 32 * r.Below(13);
      },
      __imp__sub_8224D4A0, hooks::CarListColorSetSwatch);
  Group(
      h, w, "ColorPanel::SetSwatches (sub_8224D518)", n, {128, 40},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = w.at(kColors);
        t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(13);
        // The swatches drawn (each node's +104) stay few: the count 0 to 8.
        guest::StoreU32(base, w.at(kColors) + 52, r.Below(9));
      },
      __imp__sub_8224D518, hooks::CarListColorSetSwatches);
  // The car's info.
  Group(h, w, "CarPanel::SetTexts (sub_8224D7F0)", n, {112, 32}, group(12), __imp__sub_8224D7F0,
        hooks::CarListCarSetTexts);
  Group(
      h, w, "CarPanel::SetButtons (sub_8224D860)", n, {144, 56},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        for (PPCRegister* reg : {&t.r4, &t.r5, &t.r6, &t.r7}) {
          reg->u64 = r.Below(3) == 0 ? 0xFFFFFFFFFFFFFFFFull
                                     : (r.Below(4) == 0 ? r.Next() : r.Below(8));
        }
      },
      __imp__sub_8224D860, hooks::CarListCarSetButtons);
  Group(
      h, w, "CarPanel::PlaceCursor (sub_8224D9A0)", n, {128, 32},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        t.r4.u64 = Small(r);
      },
      __imp__sub_8224D9A0, hooks::CarListCarPlaceCursor);
  Group(
      h, w, "CarPanel::Select (sub_8224DCF0)", n, {112, 32},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        guest::StoreU32(base, w.panel() + 8, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(10));
        t.r4.u64 = r.Below(3) == 0 ? (r.Next() & 0xFFFFFFFF00000000ull) : r.Below(2);
      },
      __imp__sub_8224DCF0, hooks::CarListCarSelect);
  Group(h, w, "CarPanel::StopArrows (sub_8224DF30)", n, {112, 24}, group(12),
        __imp__sub_8224DF30, hooks::CarListCarStopArrows);
  Group(
      h, w, "CarPanel::SetName (sub_8224DED8)", n, {112, 24},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        guest::StoreU32(base, w.panel() + 316, r.Below(2));
        t.r4.u64 = w.entry.address;
      },
      __imp__sub_8224DED8, hooks::CarListCarSetName);
  // SetStats: the record's ids small, its values any words, +24 a float
  // (any bits).
  const auto stats_record = [base](Random& r, uint32_t record) {
    guest::StoreU32(base, record + 0, r.Below(5));
    guest::StoreU32(base, record + 4, r.Below(5));
    if (r.Below(2) == 0) {
      guest::StoreFloat(base, record + 24, float(r.Below(100000)) / 7.0f);
    }
  };
  Group(
      h, w, "CarPanel::SetStats (sub_8224E288)", 80, {144, 48},
      [base, stats_record](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        stats_record(r, w.at(kRecord));
        if (r.Below(2) == 0) {
          guest::StoreU32(base, w.at(kRecord) + 8, r.Below(400));
        }
        t.r4.u64 = w.at(kRecord);
      },
      __imp__sub_8224E288, hooks::CarListCarSetStats);
  Group(
      h, w, "CarPanel::SetCarName (sub_822417E8)", n, {128, 40},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        t.r4.u64 = r.Below(80);
        t.r5.u64 = r.Below(10);
      },
      __imp__sub_822417E8, hooks::CarListCarSetCarName);
  Group(
      h, w, "Panel7C::ShowArrows (sub_82241868)", n, {96, 16},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        t.r4.u64 = r.Below(3) == 0 ? (r.Next() & 0xFFFFFFFF00000000ull) : r.Below(2);
      },
      __imp__sub_82241868, hooks::CarListPanel7CShowArrows);
  // ShowStats: the C panel's vtable at +0, its +4 0 (CarPanel::SetPrice never
  // ran in play), the entry's floats any bits or small.
  Group(
      h, w, "CarPanel::ShowStats (sub_8224E920)", 80, {144, 24},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        guest::StoreU32(base, w.panel() + 0, w.at(kVtable));
        guest::StoreU32(base, w.panel() + 4, 0);
        for (const uint32_t at : {652u, 656u, 660u, 664u}) {
          if (r.Below(2) == 0) {
            guest::StoreFloat(base, w.entry.address + at, float(r.Below(100000)) / 3.0f);
          }
        }
        t.r4.u64 = w.entry.address;
      },
      __imp__sub_8224E920, hooks::CarListCarShowStats);
}

// A small word around 0 (-2 to 3), now and then any word.
uint64_t Around(Random& r) {
  return r.Below(8) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(6)) - 2));
}

void MorePanels(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const int n = 40;
  Group(
      h, w, "TabPanel::SetTab (sub_822501C8)", n, {144, 56},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        guest::StoreU32(base, w.panel() + 140, r.Below(2));
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(4);
      },
      __imp__sub_822501C8, hooks::CarListTabSetTab);
  Group(
      h, w, "TabPanel::Move (sub_8224FF88)", 80, {160, 72},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        guest::StoreU32(base, w.panel() + 140, r.Below(2));
        for (const uint32_t at : {12u, 16u, 20u}) {
          guest::StoreU32(base, w.panel() + at, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
        }
        t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(4);
        t.r5.u64 = r.Below(4) == 0 ? r.Next() : t.r4.u64 + Around(r);
        t.r6.u64 = Around(r);
      },
      __imp__sub_8224FF88, hooks::CarListTabMove);
  // The rows over the world's list: the rows shown (+4) 0 to 8, the scroll
  // 0 to 10, the item 0 to 9.
  const auto rows = [base](PPCContext& t, Random& r, const World& w) {
    SetGroup(base, w, r, 0);
    guest::StoreU32(base, w.panel() + 4, r.Below(9));
    guest::StoreU32(base, w.panel() + 156, r.Below(2));
    // The rows' nodes not negative (as RowPanel::SetRow's cases).
    for (uint32_t at = 20; at <= 60; at += 4) {
      guest::StoreU32(base, w.panel() + at, r.Below(64));
    }
    t.r4.u64 = w.at(kList);
    t.r5.u64 = r.Below(11);
  };
  Group(
      h, w, "RowPanel::Fill (sub_8224F758)", 60, {192, 104},
      [rows](PPCContext& t, Random& r, const World& w) {
        rows(t, r, w);
        t.r6.u64 = r.Below(10);
      },
      __imp__sub_8224F758, hooks::CarListRowFill);
  Group(h, w, "RowPanel::Refresh (sub_8224F888)", n, {128, 48}, rows, __imp__sub_8224F888,
        hooks::CarListRowRefresh);
  Group(
      h, w, "RowPanel::Move (sub_8224F8F8)", 80, {160, 80},
      [rows](PPCContext& t, Random& r, const World& w) {
        rows(t, r, w);
        t.r7.u64 = t.r5.u64;  // the scroll
        t.r4.u64 = r.Below(8);
        t.r5.u64 = r.Below(8);
        t.r6.u64 = r.Below(4) == 0 ? (r.Next() & 0xFFFFFFFF00000000ull) : Around(r);
        t.r8.u64 = r.Next();
        t.r9.u64 = w.at(kList);
      },
      __imp__sub_8224F8F8, hooks::CarListRowMove);
  Group(
      h, w, "ColorPanel::Move (sub_8224D580)", 80, {160, 80},
      [base](PPCContext& t, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        t.r4.u64 = r.Below(13);
        t.r5.u64 = r.Below(13);
        t.r6.u64 = r.Below(4) == 0 ? (r.Next() & 0xFFFFFFFF00000000ull) : Around(r);
        t.r7.u64 = r.Below(13);
        t.r8.u64 = Around(r);
        t.r9.u64 = r.Below(3) == 0 ? (r.Next() & 0xFFFFFFFF00000000ull) : r.Below(2);
        t.r10.u64 = w.at(kColors);
        guest::StoreU32(base, w.at(kColors) + 52, r.Below(9));
      },
      __imp__sub_8224D580, hooks::CarListColorMove);
  // PowerGraph::SetEntry: the record at the world's +0x408; the byte +708 0
  // to 9, the floats any bits or small.
  Group(
      h, w, "PowerGraph::SetEntry (sub_82250368)", n, {0, 0},
      [base](PPCContext& t, Random& r, const World& w) {
        guest::StoreU8(base, w.entry.address + 708, uint8_t(r.Below(10)));
        for (const uint32_t at : {652u, 656u, 660u, 664u}) {
          if (r.Below(2) == 0) {
            guest::StoreFloat(base, w.entry.address + at, float(r.Below(100000)) / 3.0f);
          }
        }
        t.r3.u64 = w.entry.address | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
        t.r4.u64 = r.Below(4) == 0 ? r.Next() : r.Below(3);
      },
      __imp__sub_82250368, hooks::CarListPowerGraphSetEntry);
}

// PanelSet's leaves on the objects block (r3 its start, its words random).
void Leaves(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const int n = 24;
  const auto words = [base](PPCContext& t, Random& r, const World& w) {
    for (const uint32_t at : {12u, 16u, 20u}) {
      guest::StoreU32(base, w.panel() + at, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
    }
    t.r4.u64 = r.Next();
  };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } leaves[] = {
      {"PanelSet::Reset (sub_822496C8)", __imp__sub_822496C8, hooks::CarListSetReset},
      {"PanelSet::Picked (sub_82249848)", __imp__sub_82249848, hooks::CarListSetPicked},
      {"PanelSet::WentBack (sub_82249870)", __imp__sub_82249870, hooks::CarListSetWentBack},
      {"PanelSet::WantsPowerGraph (sub_82249898)", __imp__sub_82249898,
       hooks::CarListSetWantsPowerGraph},
      {"PanelSet::SetOpenKind (sub_8224B6D8)", __imp__sub_8224B6D8, hooks::CarListSetOpenKind},
      {"PanelSet::SetPreviousTab (sub_8224B6E8)", __imp__sub_8224B6E8,
       hooks::CarListSetPreviousTab},
      {"PanelSet::SetPreviousEntry (sub_8224B6F0)", __imp__sub_8224B6F0,
       hooks::CarListSetPreviousEntry},
      {"PanelSet::SetPreviousColor (sub_8224B6F8)", __imp__sub_8224B6F8,
       hooks::CarListSetPreviousColor},
      {"PanelSetBase::PickedPage (sub_82248EF0)", __imp__sub_82248EF0, hooks::CarListPickedPage},
  };
  for (const auto& leaf : leaves) {
    Group(h, w, leaf.name, n, {0, 0}, words, leaf.recompiled, leaf.native);
  }
  // SaveData: one player (SaveData::Of(0)) or two (the pointer at
  // 0x826C5B5C plus 120,108 x the player).
  Group(
      h, w, "PanelSetBase::SaveData (sub_82249668)", n, {96, 16},
      [base](PPCContext&, Random& r, const World& w) {
        guest::StoreU32(base, car_list::kTwoPlayers, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
        guest::StoreU32(base, w.panel() + 1952, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
      },
      __imp__sub_82249668, hooks::CarListSaveData);
}

// --- PanelSet's pages and steps ---

// Blocks a case allocated (the layer's pages) and the slots holding them,
// and the menus' blocks (+68) a run may make: compared by whether they're
// still pages after, freed, and their words replaced by markers.
std::vector<std::pair<uint32_t, uint32_t>> g_blocks;

void TrackBlock(uint8_t* base, uint32_t slot) {
  const uint32_t block = memory::AllocPages(64, 4096);
  guest::StoreU32(base, slot, block);
  g_blocks.emplace_back(slot, block);
}

void SettleBlocks(uint8_t* base, const World& w, Outputs& out) {
  uint32_t marker = 0xB10C0000;
  for (const auto& [slot, block] : g_blocks) {
    const bool pages = block != 0 && memory::IsPages(block);
    out.Word(pages ? 1 : 0);
    if (pages) memory::FreePages(block);
    if (guest::LoadU32(base, slot) == block) guest::StoreU32(base, slot, marker);
    marker++;
  }
  g_blocks.clear();
  for (const uint32_t menu : {60u, 132u}) {
    const uint32_t slot = w.b() + menu + 68;
    const uint32_t block = guest::LoadU32(base, slot);
    if (block == 0 || (block & 0xFFFF0000u) == 0xB10C0000u) continue;
    const bool pages = memory::IsPages(block);
    out.Word(pages ? 1 : 0);
    if (pages) memory::FreePages(block);
    guest::StoreU32(base, slot, 0xB10CFFFF);
  }
}

// The set's shape: its panel E the world's panel (+0 a group, +4 the rows
// shown 0 to 8, its rows' nodes +20 to +60 not negative), its panels F, D
// and C one panel at the objects' +0x480 (its words indices, +4 and +12 a
// group), each now and then 0 where the original tests the pointer (E
// always set: Open and ToTabs write its +152); the list +1820 the world's
// list, the two menus' words small (+68, the block, 0), the page words -1
// to 8, the flags 0 or 1, the confirm panel (+1892) 0 or a block in the
// objects' +0xE00.
constexpr uint32_t kOtherPanel = 0x480;

void ShapeSet(uint8_t* base, const World& w, Random& r, bool nulls) {
  g_blocks.clear();
  const uint32_t b = w.b();
  SetGroup(base, w, r, 0);
  guest::StoreU32(base, w.panel() + 4, r.Below(9));
  for (uint32_t at = 20; at <= 60; at += 4) {
    guest::StoreU32(base, w.panel() + at, r.Below(64));
  }
  const uint32_t other = w.at(kOtherPanel);
  for (uint32_t i = 0; i < 0x180; i += 4) {
    guest::StoreU32(base, other + i, Index(r));
  }
  for (const uint32_t at : {4u, 12u}) {
    guest::StoreU32(base, other + at, w.at(r.Below(2) == 0 ? kGroupA : kGroupB));
  }
  for (const uint32_t at : {1896u, 1904u, 1908u}) {
    guest::StoreU32(base, b + at, nulls && r.Below(6) == 0 ? 0 : other);
  }
  guest::StoreU32(base, b + 1900, w.panel());
  guest::StoreU32(base, b + 1820, w.at(kList));
  for (const uint32_t menu : {60u, 132u}) {
    for (uint32_t i = 0; i < 72; i += 4) {
      guest::StoreU32(base, b + menu + i, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(9));
    }
    guest::StoreU32(base, b + menu + 68, 0);
  }
  guest::StoreU32(base, b + 64, r.Below(9));  // the rows' scroll
  for (const uint32_t at : {32u, 36u, 40u, 44u, 48u, 1932u}) {
    guest::StoreU32(base, b + at, r.Below(10) - 1);
  }
  for (const uint32_t at : {1936u, 1940u, 1944u, 1948u}) {
    guest::StoreU32(base, b + at, r.Below(2));
  }
  guest::StoreU32(base, b + 1880, r.Below(3));
  guest::StoreU32(base, b + 232, r.Below(3));
  guest::StoreU32(base, b + 1892, r.Below(2) == 0 ? 0 : w.at(0xE00));
  guest::StoreU32(base, w.set.address + 1932, r.Below(3));  // the open kind
  guest::StoreU32(base, car_list::kFadeWait, r.Below(2));
}

void Steps(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const int n = 30;
  const auto set = [base](PPCContext& t, Random& r, const World& w) {
    ShapeSet(base, w, r, true);
    t.r3.u64 = w.b() | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
  };
  const AfterFn settle = [base](const World& w, Outputs& out) { SettleBlocks(base, w, out); };
  Group(h, w, "PanelSetBase::ToTabs (sub_82249558)", n, {128, 24}, set, __imp__sub_82249558,
        hooks::CarListToTabs, settle);
  Group(
      h, w, "PanelSet::Open (sub_82249730)", n, {112, 24},
      [set](PPCContext& t, Random& r, const World& w) {
        set(t, r, w);
        t.r3.u64 -= 4;  // the set
      },
      __imp__sub_82249730, hooks::CarListOpen, settle);
  // Free: each list 0 to 3 entries of the layer's pages, the menus' blocks
  // pages or 0.
  const auto lists = [base, set](PPCContext& t, Random& r, const World& w) {
    set(t, r, w);
    const uint32_t b = w.b();
    const uint32_t list_at[4] = {272, 788, 1304, 1824};
    const uint32_t count_at[4] = {784, 1300, 1816, 1876};
    for (int i = 0; i < 4; i++) {
      const uint32_t count = r.Below(4);
      guest::StoreU32(base, b + count_at[i], count);
      for (uint32_t k = 0; k < count; k++) TrackBlock(base, b + list_at[i] + 4 * k);
    }
    for (const uint32_t menu : {60u, 132u}) {
      if (r.Below(2) == 0) TrackBlock(base, b + menu + 68);
    }
  };
  Group(h, w, "PanelSetBase::Free (sub_8224B538)", n, {112, 32}, lists, __imp__sub_8224B538,
        hooks::CarListBaseFree, settle);
  Group(
      h, w, "PanelSet::Free (sub_8224B880)", 12, {112, 32},
      [lists](PPCContext& t, Random& r, const World& w) {
        lists(t, r, w);
        t.r3.u64 -= 4;
      },
      __imp__sub_8224B880, hooks::CarListSetFree, settle);
  // HandleInput: the pages -1, 5, 7 (BackPage7, the original through the
  // table) and 8.
  const auto input = [base, set](PPCContext& t, Random& r, const World& w) {
    set(t, r, w);
    static constexpr uint32_t kPages[4] = {0xFFFFFFFFu, 5, 7, 8};
    guest::StoreU32(base, w.b() + 32, kPages[r.Below(4)]);
  };
  Group(h, w, "PanelSetBase::HandleInput (sub_8224BF30)", n, {176, 48}, input,
        __imp__sub_8224BF30, hooks::CarListBaseHandleInput, settle);
  Group(
      h, w, "PanelSet::HandleInput (sub_8224CA28)", 12, {176, 48},
      [input](PPCContext& t, Random& r, const World& w) {
        input(t, r, w);
        t.r3.u64 -= 4;
      },
      __imp__sub_8224CA28, hooks::CarListSetHandleInput, settle);
  Group(h, w, "PanelSetBase::BackPage8 (sub_8224A3F8)", n, {96, 16}, set, __imp__sub_8224A3F8,
        hooks::CarListBackPage8, settle);
  // The steps: r4 the first-pass word (b + 56) 0 or 1.
  const auto step = [base, set](PPCContext& t, Random& r, const World& w) {
    set(t, r, w);
    guest::StoreU32(base, w.b() + 56, r.Below(3) == 0 ? 1 : 0);
    t.r4.u64 = w.b() + 56;
  };
  const struct {
    const char* name;
    Frame frame;
    GuestFunction recompiled, native;
  } steps[] = {
      {"PanelSetBase::Step2 (sub_82248F00)", {128, 48}, __imp__sub_82248F00, hooks::CarListStep2},
      {"PanelSetBase::Step3 (sub_8224A718)", {112, 32}, __imp__sub_8224A718, hooks::CarListStep3},
      {"PanelSetBase::Step7 (sub_82249098)", {144, 64}, __imp__sub_82249098, hooks::CarListStep7},
      {"PanelSetBase::Step11 (sub_8224A7E0)", {112, 32}, __imp__sub_8224A7E0,
       hooks::CarListStep11},
      {"PanelSetBase::Step18 (sub_82249258)", {112, 24}, __imp__sub_82249258,
       hooks::CarListStep18},
      {"PanelSetBase::Step26 (sub_8224A8E0)", {112, 32}, __imp__sub_8224A8E0,
       hooks::CarListStep26},
  };
  for (const auto& s : steps) {
    Group(h, w, s.name, 40, s.frame, step, s.recompiled, s.native, settle);
  }
}

}  // namespace

void CarList(Context& h) {
  const World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: car_list: no block from the allocation layer");
    return;
  }
  BuildStringTable(h.base, w);
  Panels(h, w);
  MorePanels(h, w);
  Leaves(h, w);
  Steps(h, w);
}

}  // namespace harness
