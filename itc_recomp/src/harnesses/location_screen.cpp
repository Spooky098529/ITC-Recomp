// The career's location screen (src/location_screen/,
// docs/functions/location-screen.md) against the recompiled functions. Each
// case fills a world of our own with random words from the case's seed: a
// screen block (the screen, its page object and four menus, pointed at by
// 0x82814E48-0x82814E5C), a save data block (the players' slots pointed at
// it), the data tables' object (its items, 56-byte and 20-byte tables), a
// race data block (its racer records) and a race event (its groups), plus
// the scratch area, the caller's area above r1 and the frames the functions
// write; shapes the fields the function tests; runs the original (with
// --native_location_screen off) or the register steps
// (hooks/location_screen.h); and compares the blocks, the frames and r3 (all
// 64 bits). The globals the cases point elsewhere are put back after each
// run.

#include "harness.h"

#include "archive/archive.h"
#include "base/flags.h"
#include "base/log.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/location_screen.h"
#include "hooks/ui_movies.h"
#include "location_screen/location_screen.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "race_event/race_event.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_location_screen);

namespace harness {

namespace {

namespace ls = location_screen;

constexpr uint32_t kSaveDataBytes = 0x1F978;
constexpr uint32_t kSaveSlots = 0x826C5B58;
constexpr uint32_t kItems = 400;
constexpr uint32_t kItemBytes = 3008;
constexpr uint32_t kTables56 = 8;        // 56-byte tables
constexpr uint32_t kEntries56 = 64;      // entries in each
constexpr uint32_t kEntries20 = 256;     // the 20-byte table's entries
constexpr uint32_t kRecordBytes = 3936;  // a racer record
constexpr uint32_t kRecordCount = 0x49FE0;
constexpr uint32_t kRaceBytes = ls::kRacerRecords + kRecordCount + 16;
constexpr uint32_t kScreenBytes = 0x1000;  // the screen (912 bytes), then the rest
constexpr uint32_t kEventBytes = 0x400;    // the event (160 bytes), then its lists
// The movies' block: movie 21 at +0, movie 30 at +0xA000, the live scene at
// +0x14400 (0x400), 16 sub-scenes (0x400 each) from +0x14800, 32 leaf
// nodes (0x400 each) from +0x18800, the movie table's pairs at +0x20800, 8
// animations (64 bytes) at +0x20900, the two movies' textures (16 words) at
// +0x20B00, a dispatcher at +0x21000.
constexpr uint32_t kMoviesBytes = 0x22000;
constexpr uint32_t kMovie30At = 0xA000, kSceneAt = 0x14400, kSubScenesAt = 0x14800,
                   kLeavesAt = 0x18800, kPairsAt = 0x20800, kAnimationsAt = 0x20900,
                   kTexturesAt = 0x20B00, kDispatcherAt = 0x21000;
// The texts block (records, then their buffers) and the string table's.
constexpr uint32_t kTextsBytes = 0x21000;
constexpr uint32_t kStringsBytes = 0x10000;
constexpr uint32_t kMissingText = 0x826C5CD8;  // TextMap::Find's answer when not found
constexpr uint32_t kStringTable = 0x826C5C34;
// UpdateView's globals: the lights' arrays (0x8296B950 to 0x8296BBE0) and
// 0x8265ED00.
constexpr uint32_t kLights = 0x8296B950, kLightsBytes = 0x290;
constexpr uint32_t kViewCopy = 0x8265ED00;
constexpr uint32_t kMissingMovie = 0x826C5CDC;  // UiMovies::Find's answer when not found

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

// The screen block's parts: the screen at +0, the page object, the menus.
constexpr uint32_t kPagesAt = 0x400, kMenuAt = 0x440, kListMenuAt = 0x490,
                   kOpponentMenuAt = 0x4E0, kConfirmMenuAt = 0x530;
// The pictures (16 bytes each, then 12-byte texture records) at +0x600.
constexpr uint32_t kPicturesAt = 0x600;

struct World {
  World()
      : save(kSaveDataBytes),
        tables(0x40 + kItems * kItemBytes + kTables56 * kEntries56 * 56 + kEntries20 * 20),
        race(kRaceBytes),
        screen(kScreenBytes),
        event(kEventBytes),
        movies(kMoviesBytes),
        texts(kTextsBytes),
        strings(kStringsBytes) {}
  bool ok() const {
    return save.address && tables.address && race.address && screen.address && event.address &&
           movies.address && texts.address && strings.address;
  }
  uint32_t items() const { return tables.address + 0x40; }
  uint32_t item(uint32_t id) const { return items() + kItemBytes * id; }
  uint32_t table56(uint32_t t) const {
    return items() + kItems * kItemBytes + kEntries56 * 56 * t;
  }
  uint32_t table20() const { return table56(kTables56); }
  uint32_t records() const { return race.address + ls::kRacerRecords; }
  Block save;
  Block tables;
  Block race;
  Block screen;
  Block event;
  Block movies;  // movies 21 and 30, their scene tree, the movie table's pairs
  Block texts;   // the movies' text records and buffers
  Block strings;  // the string table for the texts' cases
};

struct Spec {
  // Ranges below r1 (offsets from r1, negative) the function writes into its
  // frame, compared too.
  std::vector<std::pair<int32_t, uint32_t>> frame_ranges;
  // Run after the function, before the outputs are read (frees, and words
  // made comparable), with the case's outputs to add to.
  std::function<void(PPCContext&, Outputs&)> after;
  // The movies' nodes all TXT, with the texts and string table (ShapeTexts).
  bool texts = false;
};

// UiMovie::Run's calls (the update it jumps to, sub_8211D2F8), kept and
// answered with 0 or 1 from the case's generator.
struct Call {
  uint32_t function, lr, r1;
  uint64_t r3, r4;
};
std::vector<Call>* g_calls = nullptr;
Random g_answers;

bool StandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_calls == nullptr || function != 0x8211D2F8) {
    return false;
  }
  g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Below(2);
  return true;
}

// Career::EventQuery's and Race::SetUp's register steps and Archive::Load's
// adapter (hooks::g_start_up_recorder) kept the same way: answered with a
// word from the case's generator (Archive::Load with a serial or -1), r5 and
// r6 kept for Archive::Load (in r4's place: its r4 the pages, freed after).
struct Load {
  uint32_t destination, size;
};
std::vector<Load>* g_loads = nullptr;
bool Recorder(PPCContext& ctx, uint32_t function) {
  if (g_calls == nullptr ||
      (function != 0x82111638 && function != 0x82115D98 && function != 0x8209DE48)) {
    return false;
  }
  if (function == 0x8209DE48) {
    g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64,
                        uint64_t(ctx.r5.u32) << 32 | ctx.r6.u32});
    if (g_loads != nullptr) {
      g_loads->push_back({ctx.r4.u32, 0});
    }
    ctx.r3.u64 = g_answers.Below(3) == 0 ? ~0ull : g_answers.Below(100);
    return true;
  }
  g_calls->push_back({function, uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Next();
  return true;
}

void Record(Outputs& out, const std::vector<Call>& calls) {
  out.Word(uint32_t(calls.size()));
  for (const Call& call : calls) {
    out.Word(call.function);
    out.Word(call.lr);
    out.Word(call.r1);
    out.Word(uint32_t(call.r3 >> 32));
    out.Word(uint32_t(call.r3));
    out.Word(uint32_t(call.r4 >> 32));
    out.Word(uint32_t(call.r4));
  }
}

// The movies: both with empty animation lists, live scene the tree's root
// (and their saved and file scenes, +20 and +24, the same: a pose reset
// copies the root's children 0 to 3 onto themselves); the root's child index
// (+108) 0 to 3 and 70 child entries from it (sub-scenes), its animation
// index (+116) 70 and 24 animations after it; the sub-scenes' child index 0
// to 3 and 96 entries (leaves); the leaves' tags PIC, PRM or other (the
// sub-scenes' SCE, so SetNodeTexture leaves their index), or with `texts`
// every node a TXT; each leaf's picture texture entries (+144 + 16 k, as
// PictureTexture reads them for an index of k) 0 to 15, into the movies'
// textures (+28) of 16 words; the first 128 named animations of each movie
// over the root and one of 8 animations of no tracks (+12 the length); the
// movie table at the pairs {21, 30}, or only one (then the missing word
// names the movie 21).
void ShapeMovies(uint8_t* base, const World& w, Random& r, bool texts) {
  const uint32_t at = w.movies.address;
  const uint32_t movie21 = at, movie30 = at + kMovie30At;
  const uint32_t root = at + kSceneAt;
  constexpr uint32_t kTagTxt = 0x54585400;
  for (uint32_t m = 0; m < 2; m++) {
    const uint32_t movie = m == 0 ? movie21 : movie30;
    ui_movies::InitAnimationLists(base, movie);
    guest::StoreU32(base, movie + 16, root);
    guest::StoreU32(base, movie + 20, root);
    guest::StoreU32(base, movie + 24, root);
    guest::StoreU32(base, movie + 28, at + kTexturesAt + 64 * m);
    for (uint32_t k = 0; k < 128; k++) {
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, root);
      guest::StoreU32(base, named + 32, at + kAnimationsAt + 64 * r.Below(8));
    }
  }
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t animation = at + kAnimationsAt + 64 * k;
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
    const uint32_t entry = root + 128 + 8 * i;
    guest::StoreU32(base, entry, i < 70 ? at + kSubScenesAt + 0x400 * r.Below(16)
                                        : at + kAnimationsAt + 64 * r.Below(8));
  }
  if (texts) {
    guest::StoreU32(base, root, kTagTxt);
  }
  for (uint32_t k = 0; k < 16; k++) {
    const uint32_t sub = at + kSubScenesAt + 0x400 * k;
    guest::StoreU32(base, sub, texts ? kTagTxt : 0x53434500);  // "SCE\0"
    guest::StoreU32(base, sub + 108, r.Below(4));
    for (uint32_t i = 0; i < 96; i++) {
      guest::StoreU32(base, sub + 128 + 8 * i, at + kLeavesAt + 0x400 * r.Below(32));
    }
    if (texts) {
      guest::StoreU32(base, sub + 108, r.Below(4));
    }
  }
  constexpr uint32_t kTags[] = {0x50494300, 0x50524D00, kTagTxt};  // PIC, PRM, TXT
  for (uint32_t k = 0; k < 32; k++) {
    const uint32_t leaf = at + kLeavesAt + 0x400 * k;
    guest::StoreU32(base, leaf,
                    texts ? kTagTxt : r.Below(4) == 3 ? uint32_t(r.Next()) : kTags[r.Below(3)]);
    guest::StoreFloat(base, leaf + 12, float(r.Below(100)) / 4.0f);
    if (texts) {
      guest::StoreU32(base, leaf + 108, r.Below(16));
    }
    for (uint32_t i = 144; i < 0x400; i += 16) {
      guest::StoreU32(base, leaf + i, r.Below(16));
    }
  }
  const uint32_t pairs = at + kPairsAt;
  guest::StoreU32(base, pairs + 0, 21);
  guest::StoreU32(base, pairs + 4, movie21);
  guest::StoreU32(base, pairs + 8, 30);
  guest::StoreU32(base, pairs + 12, movie30);
  const uint32_t which = r.Below(4);  // 0: only 30 (21 missing), 1: only 21, else both
  guest::StoreU32(base, ui_movies::kMovieTable, which == 0 ? pairs + 8 : pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, which < 2 ? 1 : 2);
  guest::StoreU32(base, kMissingMovie, movie21);
}

// The texts (with `texts`): each movie's texts' map (+0x9BCC) holds ids 1
// to 14, records of the texts block's (32 bytes each: +0 a 4 KB buffer of
// its own of UTF-16 letters with page breaks (0xFFD1), ended by 0; +20 the
// page -1 to 3; the rest random), the count 14; the word at 0x826C5CD8 (an
// id not there) another record, so none is made. The string table
// (0x826C5C34, put back after) one container of strings of letters only (no
// argument codes: the user's call, 2026-10-30), for every id of every file.
void ShapeTexts(uint8_t* base, const World& w, Random& r) {
  const uint32_t at = w.texts.address;
  for (uint32_t m = 0; m < 2; m++) {
    const uint32_t movie = w.movies.address + (m == 0 ? 0 : kMovie30At);
    for (uint32_t k = 0; k < 16; k++) {
      const uint32_t record = at + 0x200 * m + 32 * k;
      const uint32_t buffer = at + 0x1000 + 0x1000 * (16 * m + k);
      guest::StoreU32(base, record, buffer);
      guest::StoreU32(base, record + 20, r.Below(5) - 1);
      const uint32_t length = r.Below(60);
      for (uint32_t c = 0; c < length; c++) {
        guest::StoreU16(base, buffer + 2 * c,
                        r.Below(6) == 0 ? 0xFFD1 : uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, buffer + 2 * length, 0);
      if (k < 14) {
        guest::StoreU32(base, movie + 0x9BCC + 8 * k, k + 1);
        guest::StoreU32(base, movie + 0x9BCC + 8 * k + 4, record);
      }
    }
    guest::StoreU32(base, movie + 0x9BCC + 1024, 14);
  }
  guest::StoreU32(base, kMissingText, at + 32 * 15);
  guest::StoreU32(base, kStringTable, w.strings.address);
}

using SetupFn = std::function<void(Random&, const World&)>;
using ArgsFn = std::function<void(PPCContext&, Random&, const World&)>;

// The world's fields every case shapes: the tables' object, the pointers.
void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.tables.address, 0x40, r);
  guest::StoreU32(base, w.tables.address + 8, kItems);
  guest::StoreU32(base, w.tables.address + 16, w.items());
  for (uint32_t t = 0; t < kTables56; t++) {
    guest::StoreU32(base, w.tables.address + 4 * (t + 5), w.table56(t));
  }
  guest::StoreU32(base, w.tables.address + 52, w.table20());
  // The 20-byte table: +0 small (the keys tested), +8 an item id, +4 the
  // next (0 often); the last entry ends it.
  for (uint32_t i = 0; i < kEntries20; i++) {
    const uint32_t entry = w.table20() + 20 * i;
    guest::StoreU32(base, entry + 0, i + 1 == kEntries20 ? 0xFFFFFFFFu : r.Below(14));
    guest::StoreU32(base, entry + 8, r.Below(8));
    guest::StoreU32(base, entry + 4, r.Below(3) == 0 ? 0 : r.Below(5));
  }
  for (uint32_t p = 0; p < 4; p++) {
    guest::StoreU32(base, kSaveSlots + 4 * p, w.save.address);
  }
  guest::StoreU32(base, data_tables::kTables, w.tables.address);
  guest::StoreU32(base, ls::kRaceData, w.race.address);
  guest::StoreU32(base, race_event::kEvent, w.event.address);
  guest::StoreU32(base, ls::kPages, w.screen.address + kPagesAt);
  guest::StoreU32(base, ls::kMenu, w.screen.address + kMenuAt);
  guest::StoreU32(base, ls::kScreen, w.screen.address);
  guest::StoreU32(base, ls::kListMenu, w.screen.address + kListMenuAt);
  guest::StoreU32(base, ls::kOpponentMenu, w.screen.address + kOpponentMenuAt);
  guest::StoreU32(base, ls::kConfirmMenu, w.screen.address + kConfirmMenuAt);
  // The racer records' count: 0 to 4.
  guest::StoreU32(base, w.records() + kRecordCount, r.Below(5));
  // The event's groups (kinds 4 to 9 at +84 + 12 (kind - 4)): a count of 0
  // to 6 and a list of item ids (halfwords every 4 bytes) after the event.
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t group = w.event.address + 84 + 12 * k;
    const uint32_t list = w.event.address + 0x100 + 0x20 * k;
    guest::StoreU16(base, group, uint16_t(r.Below(7)));
    guest::StoreU32(base, group + 4, list);
    for (uint32_t i = 0; i < 8; i++) {
      guest::StoreU16(base, list + 4 * i, uint16_t(r.Below(kItems)));
    }
  }
}

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          const ArgsFn& args, GuestFunction recompiled, GuestFunction native, const Spec& spec,
          const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, {{kSaveSlots, 16},
                              {data_tables::kTables, 4},
                              {ls::kRaceData, 4},
                              {race_event::kEvent, 4},
                              {ls::kPages, 24},
                              {ui_movies::kMovieTable, 12},
                              {kMissingMovie, 4},
                              {kMissingText, 4},
                              {kStringTable, 4},
                              {kLights, kLightsBytes},
                              {kViewCopy, 16},
                              {0x826A50AC, 0x40}});
    PPCContext t = CallContext(h);
    WriteRandom(h.base, h.data, kScratchBytes, r);
    WriteRandom(h.base, t.r1.u32, h.data - t.r1.u32, r);
    for (const auto& [offset, bytes] : spec.frame_ranges) {
      WriteRandom(h.base, t.r1.u32 + offset, bytes, r);
    }
    WriteRandom(h.base, w.save.address, w.save.bytes, r);
    WriteRandom(h.base, w.screen.address, w.screen.bytes, r);
    WriteRandom(h.base, w.event.address, w.event.bytes, r);
    WriteRandom(h.base, w.records(), 4 * kRecordBytes, r);
    WriteRandom(h.base, w.movies.address, w.movies.bytes, r);
    ShapeWorld(h.base, w, r);
    WriteRandom(h.base, w.texts.address, w.texts.bytes, r);
    ShapeMovies(h.base, w, r, spec.texts);
    if (spec.texts) {
      ShapeTexts(h.base, w, r);
    }
    setup(r, w);
    args(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    g_answers = Random(r.Next());
    std::vector<Call> calls;
    g_calls = &calls;
    ui_movies_hooks::g_update_callee_stand_in = StandIn;
    const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
    hooks::g_start_up_recorder = Recorder;
    ITC_FLAG(native_location_screen) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_location_screen) = true;
    hooks::g_start_up_recorder = saved_recorder;
    ui_movies_hooks::g_update_callee_stand_in = nullptr;
    g_calls = nullptr;
    if (spec.after) {
      spec.after(t, out[v]);
    }
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.texts.address, w.texts.bytes));
    out[v].Bytes(Read(h.base, kLights, kLightsBytes));
    out[v].Bytes(Read(h.base, kViewCopy, 16));
    out[v].Bytes(Read(h.base, 0x826A50AC, 0x40));
    Record(out[v], calls);
    out[v].Word(uint32_t(t.lr));
    out[v].Word(t.r1.u32);
    out[v].Bytes(Read(h.base, w.save.address, w.save.bytes));
    out[v].Bytes(Read(h.base, w.screen.address, w.screen.bytes));
    out[v].Bytes(Read(h.base, w.event.address, w.event.bytes));
    out[v].Bytes(Read(h.base, w.records(), 4 * kRecordBytes));
    out[v].Bytes(Read(h.base, w.race.address + 0x59AE0, 8));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    const uint32_t r1 = CallContext(h).r1.u32;
    out[v].Bytes(Read(h.base, r1, h.data - r1));
    for (const auto& [offset, bytes] : spec.frame_ranges) {
      out[v].Bytes(Read(h.base, r1 + offset, bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           const ArgsFn& args, GuestFunction recompiled, GuestFunction native,
           const Spec& spec = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, args, recompiled, native, spec,
         fmt::format("case {}", c));
  }
  results.Report();
}

// A small value now and then, else any.
uint64_t SmallOrAny(Random& r) {
  switch (r.Below(4)) {
    case 0:
      return r.Below(16);
    case 1:
      return uint64_t(-int64_t(r.Below(16)));
    case 2:
      return r.Below(20000);
    default:
      return r.Next();
  }
}

// The two picture lists: up to 6 pictures each (16 bytes, +4 the id 0 to 9,
// +8 a texture record), the defaults random.
void ShapePictures(uint8_t* base, const World& w, Random& r) {
  const uint32_t screen = w.screen.address;
  for (uint32_t list = 0; list < 2; list++) {
    const uint32_t count = r.Below(7);
    guest::StoreU32(base, screen + (list == 0 ? 136 : 404), r.Below(16) == 0 ? 0 : count);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t picture = screen + kPicturesAt + 0x100 * list + 16 * i;
      guest::StoreU32(base, screen + (list == 0 ? 8 : 276) + 4 * i, picture);
      guest::StoreU32(base, picture + 4, r.Below(10));
    }
  }
  // Ring A: 32 ids from +140, +268 of them from +272 (any start).
  for (uint32_t i = 0; i < 32; i++) {
    guest::StoreU32(base, screen + 140 + 4 * i, r.Below(40));
  }
  guest::StoreU32(base, screen + 268, r.Below(33));
  guest::StoreU32(base, screen + 272, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(32));
}

void Helpers(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn none = [](Random&, const World&) {};
  Group(h, w, "LocationScreen::SceneryConfig (sub_8228F0A0)", 200, none,
        [](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = SmallOrAny(r);
          t.r4.u64 = SmallOrAny(r);
        },
        __imp__sub_8228F0A0, hooks::LocationScreenSceneryConfig);
  Group(h, w, "LocationScreen::KindText (sub_82227690)", 200, none,
        [](PPCContext& t, Random& r, const World&) {
          t.r3.u64 = r.Below(4) == 0 ? SmallOrAny(r) : r.Below(9);
          t.r4.u64 = SmallOrAny(r);
        },
        __imp__sub_82227690, hooks::LocationScreenKindText);
  const SetupFn pictures = [base](Random& r, const World& w) { ShapePictures(base, w, r); };
  const ArgsFn id = [](PPCContext& t, Random& r, const World&) {
    t.r3.u64 = r.Below(8) == 0 ? r.Next() : r.Below(40);
  };
  Group(h, w, "LocationScreen::IsPictureQueued (sub_822291E8)", 300, pictures, id,
        __imp__sub_822291E8, hooks::LocationScreenIsPictureQueued);
  Group(h, w, "LocationScreen::PictureA (sub_82229108)", 300, pictures, id, __imp__sub_82229108,
        hooks::LocationScreenPictureA);
  Group(h, w, "LocationScreen::PictureB (sub_82229158)", 300, pictures, id, __imp__sub_82229158,
        hooks::LocationScreenPictureB);
  Group(h, w, "LocationScreen::OnMessage14 (sub_82227158)", 200,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + kMenuAt,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(6));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227158,
        hooks::LocationScreenOnMessage14);
}

void Lists(Context& h, const World& w) {
  uint8_t* const base = h.base;
  Group(h, w, "LocationScreen::HasMoreUnlocks (sub_822281D8)", 200,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.save.address + ls::kUnlockCount,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8));
          guest::StoreU32(base, w.screen.address + 868,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(8));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_822281D8,
        hooks::LocationScreenHasMoreUnlocks);
  Group(h, w, "LocationScreen::NextUnlockEntry (sub_82227618)", 300,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 876, r.Below(kTables56));
          guest::StoreU32(base, w.screen.address + 872, r.Below(kEntries56 - 8));
          for (uint32_t t = 0; t < kTables56; t++) {
            for (uint32_t i = 0; i < kEntries56; i++) {
              const uint32_t next = r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next())
                                                                         : r.Below(5);
              guest::StoreU32(base, w.table56(t) + 56 * i + 8, next);
            }
          }
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227618,
        hooks::LocationScreenNextUnlockEntry);
  // The item's halfword +88 (4, 5 or another).
  const Spec item_frame{{{-int32_t(ls::kItemFrame) + 80, 3024}}};
  Group(h, w, "LocationScreen::OpponentHasCard (sub_82227708)", 300,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 804,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : 3 + r.Below(8));
          guest::StoreU32(base, w.screen.address + 800,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(8) - 1);
          for (uint32_t id = 0; id < kItems; id++) {
            guest::StoreU16(base, w.item(id) + 88, uint16_t(r.Below(4) == 0 ? r.Next() : 3 + r.Below(4)));
          }
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227708,
        hooks::LocationScreenOpponentHasCard, item_frame);
  // Racer 0's item (+980), the human racer's (the last record's) +980 and
  // +3924, the item's +88 (3 or not), the item records' halfword +2, the
  // 20-byte entries' keys (the case's +872 an offset from the found one).
  Group(h, w, "LocationScreen::NextResultText (sub_822271F8)", 400,
        [base](Random& r, const World& w) {
          for (uint32_t k = 0; k < 4; k++) {
            const uint32_t record = w.records() + kRecordBytes * k;
            guest::StoreU32(base, record + 980, r.Below(8));
            guest::StoreU32(base, record + 3924, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2) + 1);
          }
          for (uint32_t id = 0; id < 8; id++) {
            guest::StoreU16(base, w.item(id) + 88, uint16_t(r.Below(2) == 0 ? 3 : r.Below(6)));
            guest::StoreU16(base, w.save.address + ls::kItemRecords + 12 * id + 2,
                            uint16_t(r.Below(3)));
          }
          guest::StoreU32(base, w.screen.address + 872, r.Below(4));
          // At least one record so racer 0 is there; the human's may be
          // record 0 too.
          guest::StoreU32(base, w.records() + kRecordCount, 1 + r.Below(4));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_822271F8,
        hooks::LocationScreenNextResultText, {{{-int32_t(ls::kItemFrame) + 80, 3008}}});
}

void Movies(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn none = [](Random&, const World&) {};
  const ArgsFn no_args = [](PPCContext&, Random&, const World&) {};
  // The frame below r1: +80 the movie id (SetCursor and ListNode: the
  // doubleword); the register saves above it not compared.
  const Spec frame112{{{-112 + 80, 8}}};
  const Spec frame96{{{-96 + 80, 8}}};
  const Spec frame128{{{-128 + 80, 8}}};
  Group(h, w, "LocationScreen::ShowPictures (sub_8222A060)", 300, none, no_args,
        __imp__sub_8222A060, hooks::LocationScreenShowPictures, frame112);
  Group(h, w, "LocationScreen::HidePictures (sub_8222A150)", 300, none, no_args,
        __imp__sub_8222A150, hooks::LocationScreenHidePictures, frame112);
  Group(h, w, "LocationScreen::SetCarColor (sub_8222AC18)", 300,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 892,
                          r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(3));
        },
        no_args, __imp__sub_8222AC18, hooks::LocationScreenSetCarColor, frame96);
  const ArgsFn value = [](PPCContext& t, Random& r, const World&) {
    t.r3.u64 = r.Below(3) == 0 ? r.Next() : r.Below(200);
  };
  Group(h, w, "LocationScreen::SetCursor (sub_8222BFE8)", 300, none, value, __imp__sub_8222BFE8,
        hooks::LocationScreenSetCursor, frame112);
  Group(h, w, "LocationScreen::ListNode (sub_8222C058)", 300, none, value, __imp__sub_8222C058,
        hooks::LocationScreenListNode, frame112);
  Group(h, w, "LocationScreen::ShowArrows (sub_8222C0C8)", 300,
        [base](Random& r, const World& w) {
          const uint32_t count = r.Below(5);
          guest::StoreU32(base, w.screen.address + 808, r.Below(8) == 0 ? uint32_t(r.Next()) : count);
          guest::StoreU32(base, w.screen.address + 800,
                          r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(count + 1));
        },
        no_args, __imp__sub_8222C0C8, hooks::LocationScreenShowArrows, frame112);
  Group(h, w, "LocationScreen::RunCardMovie (sub_822291A8)", 200, none, no_args,
        __imp__sub_822291A8, hooks::LocationScreenRunCardMovie, frame96);
  // The step word in the screen block; the fades' words 0 or not.
  const SetupFn fades = [base](Random& r, const World& w) {
    guest::StoreU32(base, w.screen.address + 0x700, r.Below(2) == 0 ? 0 : r.Below(3));
    for (uint32_t k = 0; k < 3; k++) {
      guest::StoreU32(base, 0x826A50DC + 4 * k, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    }
  };
  const ArgsFn step = [](PPCContext& t, Random&, const World& w) {
    t.r3.u64 = w.screen.address + 0x700;
  };
  Group(h, w, "LocationScreen::Transition1 (sub_8222AD50)", 300, fades, step,
        __imp__sub_8222AD50, hooks::LocationScreenTransition1, frame112);
  Group(h, w, "LocationScreen::Transition18 (sub_8222BE60)", 300, fades, step,
        __imp__sub_8222BE60, hooks::LocationScreenTransition18, frame128);
}

// The screen's state: Reenter on a machine (the movies block's start: its
// +9604 and +9608), Reset and ResetView on the screen, ApplyQueuedPage and
// UpdatePage on the page object (+0 the page, +4 the next, +12 the
// transition) with +880 and +884 queued; UpdatePage's pages -1, 7 (it posts
// message 6 to the dispatcher, in the movies block at kDispatcherAt) or 10 (the
// empty function), the others not run here (they run the menus' input);
// its frame's handler table compared.
void State(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const ArgsFn no_args = [](PPCContext&, Random&, const World&) {};
  auto small_or_none = [](Random& r) {
    return r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(16);
  };
  Group(h, w, "ScreenMachine::Reenter (sub_822895E0)", 200,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.movies.address + 9608, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
        },
        [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.movies.address; },
        __imp__sub_822895E0, hooks::LocationScreenReenter);
  Group(h, w, "LocationScreen::Reset (sub_822279A8, with sub_82227818)", 100,
        [](Random&, const World&) {},
        [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.screen.address; },
        __imp__sub_822279A8, hooks::LocationScreenReset);
  Group(h, w, "LocationScreen::ResetView (sub_82227818)", 100, [](Random&, const World&) {},
        [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.screen.address + 560; },
        __imp__sub_82227818, hooks::LocationScreenResetView);
  const SetupFn queued = [base, small_or_none](Random& r, const World& w) {
    const uint32_t screen = w.screen.address;
    guest::StoreU32(base, screen + 880, r.Below(3) == 0 ? 0 : small_or_none(r));
    guest::StoreU32(base, screen + 884, small_or_none(r));
    const uint32_t pages = screen + kPagesAt;
    guest::StoreU32(base, pages + 0, r.Below(3) == 0 ? guest::LoadU32(base, screen + 880)
                                                     : small_or_none(r));
    guest::StoreU32(base, pages + 4, small_or_none(r));
    guest::StoreU32(base, pages + 12, small_or_none(r));
  };
  Group(h, w, "LocationScreen::ApplyQueuedPage (sub_8222C9B0)", 300, queued, no_args,
        __imp__sub_8222C9B0, hooks::LocationScreenApplyQueuedPage, {{{-96 - 112 + 80, 8}}});
  Group(h, w, "LocationScreen::UpdatePage (sub_8222EC08)", 300,
        [base, queued](Random& r, const World& w) {
          queued(r, w);
          const uint32_t pages = w.screen.address + kPagesAt;
          constexpr uint32_t kPages[] = {0xFFFFFFFFu, 7, 10};
          const uint32_t page = kPages[r.Below(3)];
          guest::StoreU32(base, pages + 0, page);
          if (r.Below(2) == 0) {
            guest::StoreU32(base, pages + 4, page);
          }
          guest::StoreU32(base, w.screen.address + 900, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
          const uint32_t ring = w.movies.address + kDispatcherAt + 204;
          guest::StoreU32(base, ring + 600, r.Below(50));
          guest::StoreU32(base, ring + 604, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(50));
        },
        [](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = r.Next();
          t.r4.u64 = w.movies.address + kDispatcherAt;
        },
        __imp__sub_8222EC08, hooks::LocationScreenUpdatePage, {{{-240 + 80, 64}}});
}

// InitStates on a machine at the movies block's start: emptied
// (StateMachine::StateMachine, count and state 0), and in half the cases
// set up once already by the original (so every Add replaces); the frame
// (random words, the same for both) compared but the register saves.
void InitStates(Context& h, const World& w) {
  uint8_t* const base = h.base;
  Group(h, w, "LocationScreen::InitStates (sub_8222F258)", 20,
        [&h, base](Random& r, const World& w) {
          const uint32_t machine = w.movies.address;
          game_modes::StateMachineConstruct(base, machine);
          guest::StoreU32(base, machine + game_modes::kTransitionCount, 0);
          guest::StoreU32(base, machine + game_modes::kState, r.Below(12));
          if (r.Below(2) == 0) {
            PPCContext t = CallContext(h);
            t.r3.u64 = machine;
            __imp__sub_8222F258(t, base);
          }
        },
        [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.movies.address; },
        __imp__sub_8222F258, hooks::LocationScreenInitStates,
        {{{-int32_t(ls::kInitStatesFrame) + 80, 1232}}});
}

// --- The second migration's ---

// Page9 and Page11 on a machine at the movies block's start (r4), r3 any:
// Career::EventQuery and Race::SetUp recorded (Recorder); Page11's picked
// opponent from the event's groups and its item copied to its frame.
void RacePages(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn opponent = [base](Random& r, const World& w) {
    guest::StoreU32(base, w.screen.address + 804, r.Below(8) == 0 ? r.Below(4) : 4 + r.Below(6));
    guest::StoreU32(base, w.screen.address + 800, r.Below(7));
  };
  const ArgsFn machine = [](PPCContext& t, Random& r, const World& w) {
    t.r3.u64 = r.Next();
    t.r4.u64 = w.movies.address;
  };
  Group(h, w, "LocationScreen::Page9 (sub_822280D8)", 100, opponent, machine,
        __imp__sub_822280D8, hooks::LocationScreenPage9, {{{-112 + 80, 8}}});
  Group(h, w, "LocationScreen::Page11 (sub_82228138)", 200, opponent, machine,
        __imp__sub_82228138, hooks::LocationScreenPage11, {{{-3136 + 80, 3024}}});
}

// NextOpponentText: the picked opponent's item (+0 its id, 0 to 9; +88 3
// or not; +92 three halfwords, -1 or ids below the item's index, so the chain
// ends), the
// items' availability (+46 2, or a condition of kind 0, 67 or past 67:
// none of the guest tests), the items' records (+0, +2, +4 0 or 1), +872.
void Opponents(Context& h, const World& w) {
  uint8_t* const base = h.base;
  Group(h, w, "LocationScreen::NextOpponentText (sub_82227430)", 400,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 804, 4 + r.Below(6));
          guest::StoreU32(base, w.screen.address + 800, r.Below(6));
          for (uint32_t id = 0; id < kItems; id++) {
            const uint32_t item = w.item(id);
            const uint32_t own = r.Below(10);
            guest::StoreU16(base, item + 0, uint16_t(own));
            // Items 0 to 9 name only lower items: the chain ends.
            const uint32_t limit = id < 10 ? id : 10;
            guest::StoreU8(base, item + 46, uint8_t(r.Below(4) == 0 ? 2 : r.Below(2)));
            constexpr uint16_t kKinds[] = {0, 67, 68, 0x8000};
            guest::StoreU16(base, item + 102, kKinds[r.Below(4)]);
            guest::StoreU16(base, item + 88, uint16_t(r.Below(2) == 0 ? 3 : r.Below(6)));
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU16(base, item + 92 + 2 * k,
                              uint16_t(limit == 0 || r.Below(2) == 0 ? 0xFFFF : r.Below(limit)));
            }
          }
          for (uint32_t id = 0; id < 10; id++) {
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU16(base, w.save.address + ls::kItemRecords + 12 * id + 2 * k,
                              uint16_t(r.Below(2)));
            }
          }
          guest::StoreU32(base, w.screen.address + 872, r.Below(4));
        },
        [](PPCContext&, Random&, const World&) {}, __imp__sub_82227430,
        hooks::LocationScreenNextOpponentText,
        {{{-int32_t(ls::kNextOpponentTextFrame) + 80, 6016}}});
}

// The pictures. LoadPicture: an asset id of the screen's (22696 on), the
// list 0, 1 or another; Archive::Load recorded, the pages (VirtualAlloc's)
// compared (their first 256 bytes, cleared) and freed. Picture::Free: a
// picture in the screen block with a texture record (operator new(12)) or
// none, and an XPR slot with a cleared XPR (operator new(24)) or none.
// FreePictures: both lists with 0 to 4 such pictures (VirtualAlloc's pages);
// the lists' pointers compared as their indices.
void Pictures(Context& h, const World& w) {
  uint8_t* const base = h.base;
  std::vector<Load> loads;
  // No locals in these frames: their +80 on hold the saved registers.
  Spec load_spec{{},
                 [base, &loads](PPCContext& t, Outputs& out) {
                   // The pages: the load's destination, or r3 for another list.
                   uint32_t pages = loads.empty() ? t.r3.u32 : loads.back().destination;
                   out.Bytes(Read(base, pages, 256));
                   memory::FreePages(pages);
                   loads.clear();
                 }};
  g_loads = &loads;
  Group(h, w, "LocationScreen::LoadPicture (sub_8222C270)", 100, [](Random&, const World&) {},
        [base](PPCContext& t, Random& r, const World& w) {
          t.r3.u64 = w.screen.address;
          // An asset of a sensible size (the TOC's: no load is made).
          uint32_t id;
          do {
            id = 22696 + r.Below(90);
          } while (archive::AssetSize(base, id) - 1 >= 0x400000);
          t.r4.u64 = id;
          t.r5.u64 = r.Below(4) == 0 ? r.Next() : r.Below(2);
        },
        __imp__sub_8222C270, hooks::LocationScreenLoadPicture, load_spec);
  g_loads = nullptr;

  // Picture::Free: the picture at the screen block's +0x700 (+8 a record,
  // +12 an XPR or 0); r3 compared as which block it names.
  struct Blocks {
    uint32_t record = 0, xpr = 0;
  };
  Blocks blocks;
  const SetupFn picture = [base, &blocks](Random& r, const World& w) {
    const uint32_t at = w.screen.address + 0x700;
    blocks = {};
    if (r.Below(4) != 0) {
      blocks.record = memory::crt::OperatorNew(base, 12).value_or(0);
    }
    if (r.Below(2) == 0) {
      blocks.xpr = memory::crt::OperatorNew(base, 24).value_or(0);
      for (uint32_t i = 0; blocks.xpr != 0 && i < 24; i += 4) {
        guest::StoreU32(base, blocks.xpr + i, 0);
      }
    }
    guest::StoreU32(base, at + 8, blocks.record);
    guest::StoreU32(base, at + 12, blocks.xpr);
  };
  const auto which = [&blocks](uint64_t r3) -> uint32_t {
    if (blocks.record != 0 && r3 == blocks.record) {
      return 1;
    }
    if (blocks.xpr != 0 && r3 == blocks.xpr) {
      return 2;
    }
    return 0;
  };
  Spec free_spec{{}, [&which](PPCContext& t, Outputs& out) {
                   const uint32_t kind = which(t.r3.u64);
                   out.Word(kind);
                   if (kind != 0) {
                     t.r3.u64 = 0;
                   }
                 }};
  Group(h, w, "Picture::Free (sub_82228438)", 200, picture,
        [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.screen.address + 0x700; },
        __imp__sub_82228438, hooks::LocationScreenPictureFree, free_spec);

  // FreePictures: each list's pictures (pages) and their blocks.
  std::vector<uint32_t> pictures;
  const SetupFn lists = [base, &pictures](Random& r, const World& w) {
    pictures.clear();
    for (uint32_t list = 0; list < 2; list++) {
      const uint32_t at = w.screen.address + (list == 0 ? 8 : 276);
      const uint32_t count = r.Below(5);
      guest::StoreU32(base, at + 128, count);
      for (uint32_t i = 0; i < count; i++) {
        const uint32_t p = memory::AllocPages(16, 4096);
        pictures.push_back(p);
        guest::StoreU32(base, at + 4 * i, p);
        guest::StoreU32(base, p + 0, 1);
        guest::StoreU32(base, p + 4, r.Below(10));
        guest::StoreU32(base, p + 8,
                        r.Below(4) == 0 ? 0 : memory::crt::OperatorNew(base, 12).value_or(0));
        uint32_t xpr = 0;
        if (r.Below(2) == 0) {
          xpr = memory::crt::OperatorNew(base, 24).value_or(0);
          for (uint32_t k = 0; xpr != 0 && k < 24; k += 4) {
            guest::StoreU32(base, xpr + k, 0);
          }
        }
        guest::StoreU32(base, p + 12, xpr);
      }
    }
  };
  Spec lists_spec{{}, [base, &w, &pictures](PPCContext&, Outputs&) {
                    for (uint32_t list = 0; list < 2; list++) {
                      const uint32_t at = w.screen.address + (list == 0 ? 8 : 276);
                      for (uint32_t i = 0; i < 32; i++) {
                        const uint32_t word = guest::LoadU32(base, at + 4 * i);
                        for (uint32_t k = 0; k < pictures.size(); k++) {
                          if (pictures[k] == word) {
                            guest::StoreU32(base, at + 4 * i, 0xF00D0000u + k);
                          }
                        }
                      }
                    }
                  }};
  Group(h, w, "LocationScreen::FreePictures (sub_82228498)", 200, lists,
        [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.screen.address; },
        __imp__sub_82228498, hooks::LocationScreenFreePictures, lists_spec);
}

// --- The transitions that set texts ---

// The string table for the texts' cases: 17 files' pointers to one
// container of `count` entries (enough for the widest file by the bounds at
// 0x82007E18), each one of 8 strings of letters (no argument codes).
void BuildStrings(uint8_t* base, const World& w) {
  constexpr uint32_t kBounds = 0x82007E18;
  uint32_t span = 1;
  for (uint32_t i = 0; i < 17; i++) {
    const int32_t from = int32_t(guest::LoadU32(base, kBounds + 4 * i));
    const int32_t to = int32_t(guest::LoadU32(base, kBounds + 4 * i + 4));
    if (to > from && uint32_t(to - from) > span) {
      span = uint32_t(to - from);
    }
  }
  ITC_LOG_INFO("harness: location_screen: strings to {}, files of up to {}",
               int32_t(guest::LoadU32(base, kBounds + 4 * 17)), span);
  const uint32_t at = w.strings.address, container = at + 0x100;
  const uint32_t strings = container + 4 + 4 * span;
  if (strings + 8 * 64 > at + w.strings.bytes) {
    ITC_LOG_ERROR("harness: location_screen: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x5061726B696E67ull);  // "Parking"
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

// The screen's and the save data's fields the texts' functions read: the
// location (+852, 0 to 5) and its record (save + 0x1AD20 + 100 x it: +96 a
// count 0 to 8, entries' +4 a 56-byte table and +8 an entry in it), the
// mode (save +6104), the unlock list (save + 0x1C778, 12-byte entries the
// same; its count 0 to 6 at +768) and +868, the pictures (ShapePictures),
// +892 (0, 1 or any), the picked opponent (+804, +800) and the items' +88
// (4, 5 or another: OpponentHasCard), the menus' cursors (+0, +4 0 to 4;
// +8 -1, 0 or 1), the 56-byte entries' +12 and +16 small, the flags words
// ListEntry reads (save + 4 (192 table + index + 27614)) 2 or not; the step
// word at the screen block's +0x700 (0 or 1), the fades' words 0 or not.
void ShapeScreen(uint8_t* base, const World& w, Random& r) {
  const uint32_t screen = w.screen.address, save = w.save.address;
  ShapePictures(base, w, r);
  guest::StoreU32(base, screen + 852, r.Below(6));
  guest::StoreU32(base, save + 6104, r.Below(4));
  auto entries = [&](uint32_t list, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, list + 12 * i + 4, r.Below(kTables56));
      guest::StoreU32(base, list + 12 * i + 8, r.Below(kEntries56));
    }
  };
  for (uint32_t location = 0; location < 6; location++) {
    const uint32_t record = save + 0x1AD20 + 100 * location;
    guest::StoreU32(base, record + 96, r.Below(9));
    entries(record, 8);
  }
  const uint32_t unlocks = save + 0x1C778;
  guest::StoreU32(base, unlocks + 768, r.Below(7));
  entries(unlocks, 6);
  guest::StoreU32(base, screen + 868, r.Below(8));
  for (uint32_t t = 0; t < kTables56; t++) {
    for (uint32_t i = 0; i < kEntries56; i++) {
      const uint32_t entry = w.table56(t) + 56 * i;
      guest::StoreU32(base, entry + 12, r.Below(20));
      guest::StoreU32(base, entry + 16, r.Below(10));
      guest::StoreU32(base, save + 4 * (192 * t + i + 27614), r.Below(2) == 0 ? 2 : r.Below(4));
    }
  }
  guest::StoreU32(base, screen + 892, r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, screen + 804, 4 + r.Below(6));
  guest::StoreU32(base, screen + 800, r.Below(6));
  for (uint32_t id = 0; id < kItems; id++) {
    guest::StoreU16(base, w.item(id) + 88, uint16_t(3 + r.Below(4)));
  }
  for (uint32_t menu : {kMenuAt, kListMenuAt, kOpponentMenuAt, kConfirmMenuAt}) {
    const uint32_t items = menu == kConfirmMenuAt ? 3 : menu == kListMenuAt ? 8 : 5;
    guest::StoreU32(base, screen + menu + 0, r.Below(items));
    guest::StoreU32(base, screen + menu + 4, r.Below(items));
    guest::StoreU32(base, screen + menu + 8, r.Below(3) - 1);
  }
  guest::StoreU32(base, screen + 0x700, r.Below(3) == 0 ? 1 : 0);
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, 0x826A50DC + 4 * k, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
}

void Texts(Context& h, const World& w) {
  uint8_t* const base = h.base;
  BuildStrings(base, w);
  const SetupFn screen = [base](Random& r, const World& w) { ShapeScreen(base, w, r); };
  const ArgsFn none = [](PPCContext&, Random&, const World&) {};
  const ArgsFn step = [](PPCContext& t, Random&, const World& w) {
    t.r3.u64 = w.screen.address + 0x700;
  };
  auto texts = [](int32_t frame, uint32_t locals) {
    Spec spec{{{-frame + 80, locals}}};
    spec.texts = true;
    return spec;
  };
  Group(h, w, "LocationScreen::ResetMovie (sub_8222CD60)", 200, screen, none,
        __imp__sub_8222CD60, hooks::LocationScreenResetMovie, texts(192, 80));
  Group(h, w, "LocationScreen::Transition29 (sub_8222CFF0)", 200, screen, step,
        __imp__sub_8222CFF0, hooks::LocationScreenTransition29, texts(112, 8));
  Group(h, w, "LocationScreen::ShowUnlockEntry (sub_8222A978)", 300, screen, none,
        __imp__sub_8222A978, hooks::LocationScreenShowUnlockEntry, texts(128, 8));
  Group(h, w, "LocationScreen::Transition25 (sub_8222D1A8)", 300, screen, step,
        __imp__sub_8222D1A8, hooks::LocationScreenTransition25, texts(112, 8));
  Group(h, w, "LocationScreen::ShowListEntry (sub_8222B048)", 300, screen, none,
        __imp__sub_8222B048, hooks::LocationScreenShowListEntry, texts(288, 128));
  Group(h, w, "LocationScreen::Transition2 (sub_8222D610)", 300, screen, step,
        __imp__sub_8222D610, hooks::LocationScreenTransition2, texts(160, 40));
  Group(h, w, "LocationScreen::Transition14 (sub_8222E6E0)", 300, screen, step,
        __imp__sub_8222E6E0, hooks::LocationScreenTransition14, texts(176, 48));
  Group(h, w, "LocationScreen::Transition16 (sub_8222E8D8)", 300, screen, step,
        __imp__sub_8222E8D8, hooks::LocationScreenTransition16, texts(160, 32));
}

// After a race (Transition23, Transition33): the racer records (1 to 4;
// racer 0's and the human's (the last) item 0 to 9, the human's +3924 1 or
// not), the items' +88 (3 or not) and category byte +35 (2 to 6), their
// records' +2 (0 to 2); the 20-byte entries' +12 and +16 small (texts and
// pictures). Transition7: the 56-byte entries' +48 (0 or 1) and +52 (an
// item id, or -1) for ItemFlags::Set. ShowOpponentPictures: as
// NextOpponentText's (Opponents) with 0 to 6 opponents. UpdateView: the
// location and the opponent menu's cursor (0 to 5).
void ShapeResults(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, w.records() + kRecordCount, 1 + r.Below(4));
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t record = w.records() + kRecordBytes * k;
    guest::StoreU32(base, record + 980, r.Below(10));
    guest::StoreU32(base, record + 3924, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2) + 1);
  }
  for (uint32_t id = 0; id < kItems; id++) {
    guest::StoreU16(base, w.item(id) + 88, uint16_t(r.Below(2) == 0 ? 3 : r.Below(6)));
    guest::StoreU8(base, w.item(id) + 35, uint8_t(2 + r.Below(5)));
  }
  for (uint32_t id = 0; id < 10; id++) {
    guest::StoreU16(base, w.save.address + ls::kItemRecords + 12 * id + 2, uint16_t(r.Below(3)));
  }
  for (uint32_t i = 0; i < kEntries20; i++) {
    guest::StoreU32(base, w.table20() + 20 * i + 12, r.Below(20));
    guest::StoreU32(base, w.table20() + 20 * i + 16, r.Below(10));
  }
  for (uint32_t t = 0; t < kTables56; t++) {
    for (uint32_t i = 0; i < kEntries56; i++) {
      const uint32_t entry = w.table56(t) + 56 * i;
      guest::StoreU32(base, entry + 48, r.Below(4) == 0 ? 1 : 0);
      guest::StoreU32(base, entry + 52, r.Below(8) == 0 ? 0xFFFFFFFFu : r.Below(kItems));
    }
  }
  guest::StoreU32(base, w.screen.address + 872, r.Below(4));
}

void AfterRace(Context& h, const World& w) {
  uint8_t* const base = h.base;
  const SetupFn results = [base](Random& r, const World& w) {
    ShapeScreen(base, w, r);
    ShapeResults(base, w, r);
  };
  const ArgsFn none = [](PPCContext&, Random&, const World&) {};
  const ArgsFn step = [](PPCContext& t, Random&, const World& w) {
    t.r3.u64 = w.screen.address + 0x700;
  };
  auto texts = [](int32_t frame, uint32_t locals) {
    Spec spec{{{-frame + 80, locals}}};
    spec.texts = true;
    return spec;
  };
  Group(h, w, "LocationScreen::Transition23 (sub_8222A470)", 300, results, step,
        __imp__sub_8222A470, hooks::LocationScreenTransition23, texts(3168, 3024));
  Group(h, w, "LocationScreen::Transition33 (sub_8222A718)", 300, results, step,
        __imp__sub_8222A718, hooks::LocationScreenTransition33, texts(3168, 3024));
  Group(h, w, "LocationScreen::Transition7 (sub_8222AE00)", 300, results, step,
        __imp__sub_8222AE00, hooks::LocationScreenTransition7, texts(288, 128));
  Group(h, w, "LocationScreen::ShowOpponentPictures (sub_8222B200)", 300,
        [base](Random& r, const World& w) {
          ShapeResults(base, w, r);
          guest::StoreU32(base, w.screen.address + 808, r.Below(7));
          guest::StoreU32(base, w.screen.address + 804, 4 + r.Below(6));
          for (uint32_t id = 0; id < kItems; id++) {
            const uint32_t item = w.item(id);
            const uint32_t limit = id < 10 ? id : 10;
            guest::StoreU16(base, item + 0, uint16_t(r.Below(10)));
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU16(base, item + 92 + 2 * k,
                              uint16_t(limit == 0 || r.Below(2) == 0 ? 0xFFFF : r.Below(limit)));
            }
          }
          for (uint32_t id = 0; id < 10; id++) {
            for (uint32_t k = 0; k < 3; k++) {
              guest::StoreU16(base, w.save.address + ls::kItemRecords + 12 * id + 2 * k,
                              uint16_t(r.Below(2)));
            }
          }
        },
        none, __imp__sub_8222B200, hooks::LocationScreenShowOpponentPictures,
        {{{-6224 + 80, 6072}}});
  Group(h, w, "LocationScreen::UpdateView (sub_82227E20)", 100,
        [base](Random& r, const World& w) {
          guest::StoreU32(base, w.screen.address + 852, r.Below(6));
          guest::StoreU32(base, w.screen.address + kOpponentMenuAt, r.Below(6));
          for (uint32_t i = 0; i < kLightsBytes; i += 4) {
            guest::StoreU32(base, kLights + i, uint32_t(r.Next()));
          }
        },
        none, __imp__sub_82227E20, hooks::LocationScreenUpdateView, {{{-304 + 80, 192}}});
}

}  // namespace

void LocationScreen(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: location_screen: no block from the allocation layer");
    return;
  }
  {
    Random r(0x10CA7E);
    WriteRandom(h.base, w.tables.address, w.tables.bytes, r);
  }
  Helpers(h, w);
  Lists(h, w);
  Movies(h, w);
  State(h, w);
  InitStates(h, w);
  RacePages(h, w);
  Opponents(h, w);
  Pictures(h, w);
  Texts(h, w);
  AfterRace(h, w);
}

}  // namespace harness
