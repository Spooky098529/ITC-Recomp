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
#include <unordered_map>
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
constexpr uint32_t kSaveBytes = 0x4000;
constexpr uint32_t kBigSaveBytes = 0x1A000;
constexpr uint32_t kStandBytes = 0x8000;

struct World {
  World()
      : objects(kObjectsBytes),
        entry(kEntryBytes),
        movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes),
        set(kSetBytes),
        save(kSaveBytes),
        big_save(kBigSaveBytes),
        menus(0x1000),
        stand(kStandBytes) {}
  bool ok() const {
    return objects.address != 0 && entry.address != 0 && movies.address != 0 &&
           texts.address != 0 && strings.address != 0 && set.address != 0 &&
           save.address != 0 && big_save.address != 0 && menus.address != 0 &&
           stand.address != 0;
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
  Block save;  // the two players' save data ([0x826C5B5C], player 0)
  Block big_save;  // a whole save data (Build reads its +104740)
  Block menus;  // the players' mappers (+0x100 vtables, +0x200 the mappers), the sound manager (+0xC00)
  Block stand;  // the stand's world (StandWorld)
};

const std::vector<std::pair<uint32_t, uint32_t>> kSavedGlobals = {
    {ui_movies::kMovieTable, 12}, {kMissingMovie, 4}, {kMissingText, 4}, {kStringTable, 4},
    {car_list::kTwoPlayers, 4},   {car_list::kPowerGraph, 4},
    {car_list::kFadeWait, 4},     {0x826A50AC, 64},  // the screen fades' words
    {car_list::kPairSaveData, 4}, {0x826C2E48, 4},  // Params ([0x826C2E48])
    // The pages': the mapper and controller entries, the sound manager, the
    // players' save data pointers, the power graph's shown word.
    {0x826BAB80, 32}, {0x826BABA4, 16}, {0x826C5C80, 4}, {0x826C5B58, 4}, {0x82815EC0, 4},
    // The update's: the stand's holders and reload count, the car scene and
    // the first pool set, the two-player stand's wait word.
    {car_list::kStandHolders, 8}, {0x826C2E40, 8}, {car_list::kStandWait, 4}};

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
  WriteRandom(base, w.save.address, w.save.bytes, r);
  guest::StoreU32(base, car_list::kPairSaveData, w.save.address);
  guest::StoreU32(base, w.entry.address + 8432, r.Below(80));
  guest::StoreU8(base, w.entry.address + 769, uint8_t(r.Below(10)));
  ShapeMovieWorld(base, w, r);
}

// Blocks a run allocated that r3 may hold, by the marker the after function
// gave them (their addresses may differ between the two runs).
std::unordered_map<uint32_t, uint32_t> g_r3_markers;

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
    g_r3_markers.clear();
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_list) = true;
    if (after) {
      after(w, out[v]);
    }
    out[v].Bytes(Read(h.base, w.set.address, w.set.bytes));
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, w.objects.address, w.objects.bytes));
    out[v].Bytes(Read(h.base, w.entry.address, w.entry.bytes));
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.texts.address, w.texts.bytes));
    out[v].Bytes(Read(h.base, w.stand.address, w.stand.bytes));
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
    const auto marker = g_r3_markers.find(t.r3.u32);
    out[v].Word(marker != g_r3_markers.end() ? marker->second : t.r3.u32);
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

// Both movies' animation lists given 0 to 5 used slots each, as
// UiMovie::PlayNamed fills them (-1, -1, the named index, its entry's scene
// and animation, a time of 0.0, chained), their named indices the panel's
// words at `offsets` (read after the setup's shaping) or any index, so the
// panels' answers (UiMovie::IsNamedActive) find some.
void FillLists(uint8_t* base, const World& w, Random& r, const std::vector<uint32_t>& offsets) {
  for (uint32_t m = 0; m < kMovieCount; m++) {
    const uint32_t movie = w.movie(m);
    for (const uint32_t list : {ui_movies::kMovieQueue, ui_movies::kMovieLooping}) {
      const uint32_t used = r.Below(6);
      for (uint32_t k = 0; k < used; k++) {
        uint32_t named = Index(r);
        if (offsets.size() != 0 && r.Below(3) != 0) {
          named = guest::LoadU32(base, w.panel() + offsets[r.Below(uint32_t(offsets.size()))]);
        }
        const uint32_t entry = movie + 176 + 36 * (named < 900 ? named : 0);
        const ui_movies::Slot slot = {0xFFFFFFFFu, 0xFFFFFFFFu, named,
                                      guest::LoadU32(base, entry + 28),
                                      guest::LoadU32(base, entry + 32), 0, 1, 0, 0, 0};
        ui_movies::Append(base, movie + list, slot);
      }
    }
  }
}

// The panels' answers (UiMovie::IsNamedActive over the lists FillLists
// fills): r3 the panel with its group at the offset each reads.
void PanelAnswers(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const int n = 60;
  const auto answer = [base](uint32_t at, std::initializer_list<uint32_t> offsets) {
    const std::vector<uint32_t> named(offsets);
    return [base, at, named](PPCContext&, Random& r, const World& w) {
      SetGroup(base, w, r, at);
      if (at == 0) {
        guest::StoreU32(base, w.panel() + 152, r.Below(2));
      }
      FillLists(base, w, r, named);
    };
  };
  Group(h, w, "TabPanel::IsShown (sub_8224FCE0)", n, {96, 16}, answer(4, {116}),
        __imp__sub_8224FCE0, hooks::CarListTabIsShown);
  Group(h, w, "RowPanel::IsShown (sub_8224F5E8)", n, {96, 16}, answer(0, {76, 80}),
        __imp__sub_8224F5E8, hooks::CarListRowIsShown);
  Group(h, w, "RowPanel::IsHidden (sub_8224F640)", n, {96, 16}, answer(0, {88}),
        __imp__sub_8224F640, hooks::CarListRowIsHidden);
  Group(h, w, "CarPanel::IsShown (sub_8224E108)", n, {128, 40}, answer(12, {40, 44}),
        __imp__sub_8224E108, hooks::CarListCarIsShown);
  Group(h, w, "ColorPanel::IsShown (sub_8224D1F0)", n, {96, 16}, answer(4, {16}),
        __imp__sub_8224D1F0, hooks::CarListColorIsShown);
  Group(h, w, "ColorPanel::IsIdle (sub_8224D288)", n, {112, 32}, answer(4, {36, 40}),
        __imp__sub_8224D288, hooks::CarListColorIsIdle);
}

// The panels' shows, selections and moves (the lists filled as for the
// answers, so the loops they start find some running).
void PanelShows(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const int n = 60;
  Group(
      h, w, "TabPanel::Show (sub_8224FE50)", n, {112, 24},
      [base](PPCContext&, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        FillLists(base, w, r, {116, 124, 132, 136});
      },
      __imp__sub_8224FE50, hooks::CarListTabShow);
  Group(
      h, w, "TabPanel::Select (sub_8224FB20)", n, {112, 32},
      [base](PPCContext&, Random& r, const World& w) {
        SetGroup(base, w, r, 4);
        guest::StoreU32(base, w.panel() + 8, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(4));
        for (const uint32_t at : {12u, 16u, 20u}) {
          guest::StoreU32(base, w.panel() + at, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
        }
        FillLists(base, w, r, {128, 132});
      },
      __imp__sub_8224FB20, hooks::CarListTabSelect);
  Group(
      h, w, "RowPanel::Show (sub_8224F1D8)", n, {112, 24},
      [base](PPCContext&, Random& r, const World& w) {
        SetGroup(base, w, r, 0);
        guest::StoreU32(base, w.panel() + 152, r.Below(2));
        guest::StoreU32(base, w.panel() + 8, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(8));
        FillLists(base, w, r, {76, 80, 112, 116});
      },
      __imp__sub_8224F1D8, hooks::CarListRowShow);
  Group(
      h, w, "RowPanel::Select (sub_8224F340)", n, {112, 24},
      [base](PPCContext&, Random& r, const World& w) {
        SetGroup(base, w, r, 0);
        guest::StoreU32(base, w.panel() + 8, r.Below(8));
        FillLists(base, w, r, {112, 116});
      },
      __imp__sub_8224F340, hooks::CarListRowSelect);
  // The C panel: the highlighted action (+8) -1 or 0 to 9 (its word at +68
  // on), the actions r4 and r5 0 to 9, the direction around 0.
  Group(
      h, w, "CarPanel::Show (sub_8224DA20)", n, {128, 48},
      [base](PPCContext&, Random& r, const World& w) {
        SetGroup(base, w, r, 12);
        guest::StoreU32(base, w.panel() + 8, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(10));
        FillLists(base, w, r, {68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 40, 44, 124, 128});
      },
      __imp__sub_8224DA20, hooks::CarListCarShow);
  const auto swap = [base](PPCContext& t, Random& r, const World& w) {
    SetGroup(base, w, r, 12);
    FillLists(base, w, r, {68, 72, 76, 80, 84, 88, 92, 96, 100, 104});
    t.r4.u64 = r.Below(10);
    t.r5.u64 = r.Below(10);
    t.r6.u64 = Around(r);
  };
  Group(h, w, "CarPanel::Move (sub_8224DDF8)", n, {128, 48}, swap, __imp__sub_8224DDF8,
        hooks::CarListCarMove);
  Group(h, w, "CarPanel::Pick (sub_8224E050)", n, {128, 40}, swap, __imp__sub_8224E050,
        hooks::CarListCarPick);
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

// The pages' world (after ShapeSet): the four players' mappers (their
// slots 10 and 12 ActionMapper::Pressed and Fired, their action bits few or
// many), the sound manager (MenuSound::Play's slot 10 a getter), the menu at
// b +60 as MenuList::Init leaves it (1 to 4 items, the selection and scroll
// small, the move word now and then set, the ids 154, 153, 155, 156, 159,
// 158, the player 0 to 3, its enabled words a block of the layer's pages,
// mostly 1), the three lists 0 to 4 entries (the world's entry), the tab,
// entry and color picked before small, b +236 and +240 small, the car not
// loaded (+24 not 1: ColorsPage's ShowStandCar is load_poll's), E's +156 0
// or 1, C always set, the power graph not shown ([0x82815EC0] 0: the
// set-ups end there), the players' save data the world's (a garage of
// link nodes from its +204 in the block, slots 0 to 2).
void ShapeMenu(uint8_t* base, const World& w, Random& r) {
  constexpr uint32_t kGet60 = 0x82090770;
  const uint32_t m = w.menus.address;
  WriteRandom(base, m, w.menus.bytes, r);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t vt = m + 0x100 + 0x40 * k, mapper = m + 0x200 + 0x200 * k;
    guest::StoreU32(base, 0x826BABA4 + 4 * k, r.Below(4));
    guest::StoreU32(base, 0x826BAB80 + 8 * k, mapper);
    guest::StoreU32(base, 0x826BAB80 + 8 * k + 4, r.Below(5) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper, vt);
    guest::StoreU32(base, vt + 40, 0x8210BF40);
    guest::StoreU32(base, vt + 48, 0x8210C010);
    for (uint32_t i = 0; i < 6; i++) {
      guest::StoreU32(base, mapper + 448 + 4 * i,
                      r.Below(3) == 0 ? 0 : uint32_t(r.Next() & r.Next()));
    }
  }
  const uint32_t manager = m + 0xC00;
  guest::StoreU32(base, manager, m + 0xD00);
  guest::StoreU32(base, m + 0xD00 + 40, kGet60);
  guest::StoreU32(base, 0x826C5C80, manager);
  const uint32_t b = w.b(), list = b + 60;
  const uint32_t count = 1 + r.Below(4);
  guest::StoreU32(base, list + 0, count);
  const uint32_t scroll = r.Below(2);
  guest::StoreU32(base, list + 4, scroll);
  guest::StoreU32(base, list + 8, r.Below(3) == 0 ? r.Below(2) : scroll);
  guest::StoreU32(base, list + 12, count);
  guest::StoreU32(base, list + 16, r.Below(count));
  guest::StoreU32(base, list + 20, r.Below(count));
  guest::StoreU32(base, list + 24, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, list + 28, 0);
  guest::StoreU32(base, list + 32, r.Below(2));
  static constexpr uint32_t kIds[6] = {154, 153, 155, 156, 159, 158};
  for (uint32_t i = 0; i < 6; i++) guest::StoreU32(base, list + 36 + 4 * i, kIds[i]);
  guest::StoreU32(base, list + 60, r.Below(4));
  guest::StoreU32(base, list + 64, r.Below(2));
  TrackBlock(base, list + 68);
  const uint32_t enabled = guest::LoadU32(base, list + 68);
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, enabled + 4 * i, r.Below(4) == 0 ? 0 : 1);
  }
  for (const uint32_t at : {272u, 788u, 1304u}) {
    guest::StoreU32(base, b + at + 512, r.Below(5));
    for (uint32_t i = 0; i < 4; i++) guest::StoreU32(base, b + at + 4 * i, w.entry.address);
  }
  guest::StoreU32(base, b + 1880, r.Below(3));
  guest::StoreU32(base, b + 1884, r.Below(4));
  guest::StoreU32(base, b + 236, r.Below(4));
  guest::StoreU32(base, b + 240, r.Below(4));
  guest::StoreU32(base, b + 24, r.Below(2) == 0 ? 0 : 2);
  guest::StoreU32(base, b + 16, r.Below(2));
  guest::StoreU32(base, b + 1952, 0);
  guest::StoreU32(base, w.panel() + 156, r.Below(2));
  guest::StoreU32(base, b + 1908, w.at(kOtherPanel));
  guest::StoreU32(base, b + 1912, r.Below(2) == 0 ? 0 : w.at(0xE40));
  guest::StoreU32(base, car_list::kTwoPlayers, r.Below(2));
  guest::StoreU32(base, 0x82815EC0, 0);
  guest::StoreU32(base, 0x826C5B58, w.save.address);
  const uint32_t garage = w.save.address + 6120;
  uint32_t node = 1000 + r.Below(100);
  guest::StoreU16(base, garage + 204, uint16_t(node));
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t next = node + 2 + r.Below(3);
    guest::StoreU16(base, garage + 4 * node, uint16_t(next));
    guest::StoreU16(base, garage + 4 * node + 2, uint16_t(r.Below(3)));
    node = next;
  }
}

// The pages' after: the menu's enabled block (its words, while it's the
// layer's pages) and the mappers' block, then SettleBlocks.
void SettleMenu(uint8_t* base, const World& w, Outputs& out) {
  const uint32_t list = w.b() + 60;
  const uint32_t block = guest::LoadU32(base, list + 68);
  if (block != 0 && memory::IsPages(block)) {
    const uint32_t count = guest::LoadU32(base, list);
    out.Bytes(Read(base, block, 4 * (count < 16 ? count : 16)));
  }
  out.Bytes(Read(base, w.menus.address, w.menus.bytes));
  SettleBlocks(base, w, out);
}

void Pages(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const auto shape = [base](PPCContext& t, Random& r, const World& w) {
    ShapeSet(base, w, r, true);
    ShapeMenu(base, w, r);
    t.r3.u64 = w.b() | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
  };
  const AfterFn settle = [base](const World& w, Outputs& out) { SettleMenu(base, w, out); };
  Group(h, w, "PanelSetBase::TabsPage (sub_82249EB8)", 60, {144, 40}, shape, __imp__sub_82249EB8,
        hooks::CarListTabsPage, settle);
  Group(h, w, "PanelSetBase::RowsPage (sub_8224A178)", 60, {112, 32}, shape, __imp__sub_8224A178,
        hooks::CarListRowsPage, settle);
  Group(h, w, "PanelSetBase::ColorsPage (sub_8224AEA8)", 60, {112, 32}, shape,
        __imp__sub_8224AEA8, hooks::CarListColorsPage, settle);
  Group(h, w, "PanelSetBase::PowerGraphPage (sub_8224BDC0)", 60, {112, 24}, shape,
        __imp__sub_8224BDC0, hooks::CarListPowerGraphPage, settle);
  // HandleInput over the pages it runs natively now (0, 1, 3, 6) and the
  // others in its cases (-1, 5, 7, 8).
  const auto input = [base, shape](PPCContext& t, Random& r, const World& w) {
    shape(t, r, w);
    static constexpr uint32_t kPages[8] = {0xFFFFFFFFu, 0, 1, 3, 5, 6, 7, 8};
    guest::StoreU32(base, w.b() + 32, kPages[r.Below(8)]);
  };
  Group(h, w, "PanelSetBase::HandleInput, the pages (sub_8224BF30)", 60, {176, 48}, input,
        __imp__sub_8224BF30, hooks::CarListBaseHandleInput, settle);
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

// The set-ups: the tab (+232) 0 to 2 (2 the garage's count, with two
// players: the world's save data, the player +1952 0), the rows
// shown (+1916) 0 to 10, the selection and scroll (+236, +240) small, the
// garage's count (+6328) 0 to 20. SetUpCarPage's garage tab (its
// ShowCarStats, never seen in play, the original over a garage car) isn't
// in the cases.
void SetUps(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const AfterFn settle = [base](const World& w, Outputs& out) { SettleBlocks(base, w, out); };
  const auto shape = [base](PPCContext& t, Random& r, const World& w, uint32_t tabs) {
    ShapeSet(base, w, r, true);
    const uint32_t b = w.b();
    guest::StoreU32(base, b + 232, r.Below(tabs));
    guest::StoreU32(base, b + 1916, r.Below(11));
    guest::StoreU32(base, b + 236, r.Below(6));
    guest::StoreU32(base, b + 240, r.Below(6));
    guest::StoreU32(base, b + 1952, 0);
    // Tab 2 with two players only: player 0's save data may be 0 when the
    // harness runs (an attract race's).
    guest::StoreU32(base, car_list::kTwoPlayers,
                    guest::LoadU32(base, b + 232) == 2 ? 1 : r.Below(2));
    guest::StoreU16(base, w.save.address + 6328, uint16_t(r.Below(21)));
    t.r3.u64 = b | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
  };
  Group(
      h, w, "PanelSetBase::SetUpRows (sub_8224A978)", 60, {144, 48},
      [shape](PPCContext& t, Random& r, const World& w) { shape(t, r, w, 3); },
      __imp__sub_8224A978, hooks::CarListSetUpRows, settle);
  Group(
      h, w, "PanelSetBase::SetUpCarPage (sub_8224C560)", 60, {128, 32},
      [shape, base](PPCContext& t, Random& r, const World& w) {
        shape(t, r, w, 2);
        // C's vtable (+0): slots 1 and 2 SetStats and SetCarName; its +4 0
        // (SetPrice never ran in play).
        const uint32_t c = guest::LoadU32(base, w.b() + 1908);
        if (c != 0) {
          guest::StoreU32(base, c + 0, w.at(kVtable));
          guest::StoreU32(base, c + 4, 0);
        }
      },
      __imp__sub_8224C560, hooks::CarListSetUpCarPage, settle);
}

// Build: two players (the save data the world's whole one at
// [0x826C5B5C], player 0), the set built (+4) 0, 2 or 1 (1: nothing done)
// or now and then one player's word 0; when built, each list 0 to 3 entries
// of the layer's pages; the unlocked bits about one in eight; the garage 0
// to 5 cars, a chain of link nodes from its +204 (in the save's +86,120 on)
// over 8 records whose ids (+224) are cars below 72. The entries a run makes
// are compared by their bytes, freed and their slots (and r3) given
// markers.
void Builds(Context& h, const World& w) {
  uint8_t* const base = h.base;
  // Params ([0x826C2E48]) of our own: its +12 the 72 specs (random, their
  // byte +771 0 or 1, one in ten 2), as the attract race's harnesses run
  // before the game's are loaded.
  constexpr uint32_t kSpecBytes = 8432;
  const Block params(16 + 72 * kSpecBytes);
  if (params.address == 0) {
    ITC_LOG_ERROR("harness: car_list: no block for Build's specs");
    return;
  }
  {
    Random r(0x4275696C64ull);  // "Build"
    WriteRandom(base, params.address, params.bytes, r);
    guest::StoreU32(base, params.address + 12, params.address + 16);
    for (uint32_t id = 0; id < 72; id++) {
      const uint32_t kind = r.Below(10);
      guest::StoreU8(base, params.address + 16 + kSpecBytes * id + 771,
                     uint8_t(kind == 0 ? 2 : kind % 2));
    }
  }
  const uint32_t specs_global = params.address;
  const auto shape = [base, specs_global](PPCContext& t, Random& r, const World& w) {
    guest::StoreU32(base, 0x826C2E48, specs_global);
    ShapeSet(base, w, r, true);
    const uint32_t b = w.b();
    static constexpr uint32_t kBuilt[4] = {0, 2, 1, 0};
    const uint32_t built = kBuilt[r.Below(4)];
    guest::StoreU32(base, b + 4, built);
    guest::StoreU32(base, car_list::kTwoPlayers,
                    built == 0 && r.Below(3) == 0 ? uint32_t(r.Next()) : 1);
    guest::StoreU32(base, b + 1952, 0);
    if (built != 0) {
      const uint32_t list_at[3] = {272, 788, 1304};
      for (int i = 0; i < 3; i++) {
        const uint32_t count = r.Below(4);
        guest::StoreU32(base, b + list_at[i] + 512, count);
        for (uint32_t k = 0; k < count; k++) TrackBlock(base, b + list_at[i] + 4 * k);
      }
    } else {
      for (const uint32_t at : {784u, 1300u, 1816u}) guest::StoreU32(base, b + at, r.Below(129));
    }
    const uint32_t save = w.big_save.address;
    WriteRandom(base, save, w.big_save.bytes, r);
    guest::StoreU32(base, car_list::kPairSaveData, save);
    for (uint32_t i = 0; i < 3; i++) {
      guest::StoreU32(base, save + 104740 + 4 * i,
                      uint32_t(r.Next() & r.Next() & r.Next()));
    }
    const uint32_t garage = save + 6120;
    const uint32_t cars = r.Below(6);
    guest::StoreU16(base, save + 6328, uint16_t(cars));
    uint32_t node = 20000 + r.Below(100);
    guest::StoreU16(base, garage + 204, uint16_t(node));
    for (uint32_t k = 0; k < 6; k++) {
      const uint32_t next = node + 2 + r.Below(3);
      guest::StoreU16(base, garage + 4 * node, uint16_t(next));
      guest::StoreU16(base, garage + 4 * node + 2, uint16_t(r.Below(8)));
      node = next;
    }
    for (uint32_t slot = 0; slot < 8; slot++) {
      guest::StoreU32(base, garage + 1968 * slot + 224, r.Below(72));
    }
    t.r3.u64 = b | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
  };
  const AfterFn after = [base](const World& w, Outputs& out) {
    uint32_t marker = 0xE0000000;
    for (const uint32_t list : {272u, 788u, 1304u}) {
      const uint32_t at = w.b() + list;
      const uint32_t count = guest::LoadU32(base, at + 512);
      for (uint32_t i = 0; i < count && i < 128; i++, marker++) {
        const uint32_t entry = guest::LoadU32(base, at + 4 * i);
        if (entry == 0 || !memory::IsPages(entry)) continue;
        out.Bytes(Read(base, entry, kEntryBytes));
        memory::FreePages(entry);
        guest::StoreU32(base, at + 4 * i, marker);
        g_r3_markers[entry] = marker;
      }
    }
    SettleBlocks(base, w, out);
  };
  Group(h, w, "PanelSetBase::Build (sub_8224C698)", 40, {160, 72}, shape, __imp__sub_8224C698,
        hooks::CarListBaseBuild, after);
  Group(
      h, w, "PanelSet::Build (sub_8224CAD8)", 12, {160, 72},
      [shape](PPCContext& t, Random& r, const World& w) {
        shape(t, r, w);
        t.r3.u64 -= 4;  // the set
      },
      __imp__sub_8224CAD8, hooks::CarListSetBuild, after);
}


// --- PanelSet's update ---
//
// The stand's world (in the world's stand block; as the type 6 and 7 modes'
// and GarageStage::ReloadModel's harnesses lay it): [0x826D83E0] pointing at
// three holder words, each holder's stage (+0x100, +0x1A00, +0x3300) with
// its garage camera not set up (+112 0: GarageStage::Draw does nothing, as
// its callers' cases in type67_modes.cpp; Draw's own cases draw), +4 -1 or
// not, +8 0 (the car scene's one pool 7 model), +16 set or not, the draw
// word +6056 0, 1 or any word, +6052 and +6088 random; the car scene
// ([0x826C2E44] at +0x4C00; +4 0, a teardown counting down or its models
// updating, the accessory skipped) with that model (+0x6000, a car with no
// parts, its build deferred: SceneModel::PollLoad answers 1 unless +5028 or
// +5040 is set), the first pool set none, the reload count 0 to 2 (so
// GarageStage::ReloadModel answers 0: the branch after it, which makes the
// car object and calls GarageStage::SetCar, isn't laid, as in the type 6
// and 7 modes' cases: SetCar's scene model world is the load_poll
// harness's; its Car::Init, SetModel and Refresh are ShowStandCar's, covered
// there). The set's stand words: +20 and +28 0 or 1, +24 0, 1 or another,
// the row +236, +240 small (the world's list, its entry's car id below 80),
// group B (+1956) 0 or a group, its animations +1960 to +1968 indices, the
// player +1952 0 or 1; the two-player word 0 or 1 and the wait word
// [0x826379B8] 0 or 1.
constexpr uint32_t kStandStages[3] = {0x100, 0x1A00, 0x3300};
constexpr uint32_t kStandScene = 0x4C00, kStandModel = 0x6000;
constexpr uint32_t kDeepStackBytes = 0x8000;  // below the call's stack pointer

void StandWorld(uint8_t* base, const World& w, Random& r) {
  const uint32_t s = w.stand.address;
  WriteRandom(base, s, w.stand.bytes, r);
  guest::StoreU32(base, car_list::kStandHolders, s);
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t holder = s + 0x40 * (i + 1), stage = s + kStandStages[i];
    guest::StoreU32(base, s + 4 * i, holder);
    guest::StoreU32(base, holder, stage);
    guest::StoreU32(base, stage + 4, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(4));
    guest::StoreU32(base, stage + 8, 0);
    guest::StoreU32(base, stage + 16, r.Below(3) == 0 ? 0 : 1 + r.Below(2));
    guest::StoreU32(base, stage + 112, 0);
    const uint32_t word = r.Below(4);
    guest::StoreU32(base, stage + 6056, word < 2 ? word : uint32_t(r.Next()));
  }
  const uint32_t scene = s + kStandScene, model = s + kStandModel;
  guest::StoreU32(base, 0x826C2E44, scene);
  guest::StoreU32(base, 0x826C2E40, 0);
  guest::StoreU32(base, 0x826D83E4, r.Below(3));
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
  // The set's stand words.
  const uint32_t b = w.b();
  guest::StoreU32(base, b + 20, r.Below(2));
  const uint32_t loaded = r.Below(4);
  guest::StoreU32(base, b + 24, loaded < 2 ? loaded : uint32_t(r.Next()));
  guest::StoreU32(base, b + 28, r.Below(2));
  guest::StoreU32(base, b + 236, r.Below(4));
  guest::StoreU32(base, b + 240, r.Below(4));
  guest::StoreU32(base, b + 1820, w.at(kList));
  guest::StoreU32(base, b + 1956, r.Below(5) == 0 ? 0 : w.at(r.Below(2) == 0 ? kGroupA : kGroupB));
  for (const uint32_t at : {1960u, 1964u, 1968u}) {
    guest::StoreU32(base, b + at, Index(r));
  }
  guest::StoreU32(base, b + 1952, r.Below(2));
  guest::StoreU32(base, car_list::kTwoPlayers, r.Below(3) == 0 ? 1 : 0);
  guest::StoreU32(base, car_list::kStandWait, r.Below(2));
}

void Updates(Context& h, const World& w) {
  uint8_t* const base = h.base;
  // The deep stack (UpdateStand's frame and its callees') random too.
  const auto deep = [base](PPCContext& t, Random& r) {
    WriteRandom(base, t.r1.u32 - kDeepStackBytes, kDeepStackBytes - kStackBytes, r);
  };
  const auto stand = [base, deep](PPCContext& t, Random& r, const World& w) {
    deep(t, r);
    StandWorld(base, w, r);
    t.r3.u64 = w.b() | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
  };
  Group(h, w, "GarageStages::DrawAll (sub_821807D8)", 60, {160, 40}, stand, __imp__sub_821807D8,
        hooks::CarListDrawAll);
  Group(h, w, "PanelSetBase::UpdateStand (sub_8224C008)", 80, {9952, 88}, stand,
        __imp__sub_8224C008, hooks::CarListUpdateStand);
  // Update: the set as the steps' cases shape it, the step -1, below -1, or
  // one of the steps made native (their worlds the steps' cases'), the
  // stand's world after.
  const auto update = [base, deep](PPCContext& t, Random& r, const World& w) {
    deep(t, r);
    ShapeSet(base, w, r, true);
    guest::StoreU32(base, w.b() + 56, r.Below(3) == 0 ? 1 : 0);
    StandWorld(base, w, r);
    static constexpr uint32_t kSteps[9] = {0xFFFFFFFFu, 0xFFFFFFFEu, 0xFFFFFFF0u, 2, 3, 7, 11,
                                           18, 26};
    guest::StoreU32(base, w.b() + 44, kSteps[r.Below(9)]);
    t.r3.u64 = w.b() | (r.Below(4) == 0 ? 0xFFFFFFFF00000000ull : 0);
  };
  const AfterFn settle = [base](const World& w, Outputs& out) { SettleBlocks(base, w, out); };
  Group(h, w, "PanelSetBase::Update (sub_8224CD00)", 80, {368, 152}, update, __imp__sub_8224CD00,
        hooks::CarListBaseUpdate, settle);
  Group(
      h, w, "PanelSet::Update (sub_8224CF18)", 30, {368, 152},
      [update](PPCContext& t, Random& r, const World& w) {
        update(t, r, w);
        t.r3.u64 -= 4;  // the set
      },
      __imp__sub_8224CF18, hooks::CarListSetUpdate, settle);
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
  PanelAnswers(h, w);
  PanelShows(h, w);
  Leaves(h, w);
  Steps(h, w);
  SetUps(h, w);
  Builds(h, w);
  Pages(h, w);
  Updates(h, w);
}

}  // namespace harness
