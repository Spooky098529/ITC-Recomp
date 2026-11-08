// The options screen (src/options_screen/, docs/functions/options-screen.md)
// against the recompiled functions. Each case fills a world of our own with
// random words from the case's seed (the settings block, the front-end menu,
// the page block), plus the stack below the call's stack pointer and the
// caller's area above it; points the screen's globals at the world; shapes
// the fields the function tests; runs the original (with
// --native_options_screen off) or the register steps
// (hooks/options_screen.h); and compares the world, the originals' frames
// (their register saves left out), the caller's area, r1 and r3, all
// exactly (nothing here is a float). The globals are put back after each
// run.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/options_screen.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "options_screen/options_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_options_screen);

namespace harness {

namespace {

namespace os = options_screen;

constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kGlobals = os::kPage;  // the screen's seven globals
constexpr uint32_t kGlobalsBytes = 28;

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

// A small signed word around [lo, hi], now and then any word.
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  if (r.Below(16) == 0) {
    return uint32_t(r.Next());
  }
  return uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
}

// The UI movie world, built as the race screens' harness builds it: two
// movies (ids 7, or 9 where a case wants movie 7 missing, and 14 in the
// movie table; the word UiMovies::Find answers for a missing one movie 7's
// block) with empty animation lists, live, saved and file scenes one tree's
// root, their first named animations over the root and one of 8
// animations of no tracks (512 of them: the options' named animations
// reach 455); the root's child index (+108) 0 to 3, its
// entries sub-scenes up to 60 and animations after (its animation index,
// +116, 60); the 16 sub-scenes' child index 0 to 3, their entries leaves up
// to 64 and animations after (+116 64); the 32 leaves TXT nodes
// (UiMovie::SetText's assert), their children's range (+100 below 16, +104
// 0 to 6), their text id and child index (+108) 0 to 8 and their entries
// leaves; each movie's texts ids 1 to 7 (records of 4 KB buffers of letters
// and page breaks; the text count 8; the missing text another record) and
// the string table of letters only (no argument codes: the user's call,
// 2026-10-30). The callees run for real on both sides (the originals' and
// ours reach the same overrides of the UI movie calls); UiMovie::Run's
// update (sub_8211D2F8) is stood in for, recorded and answered with 0 or a
// word from the case's generator.
constexpr uint32_t kMovieHeaderBytes = 0xA000;
constexpr uint32_t kMovieCount = 2;
constexpr uint32_t kTreeNodeBytes = 0x400;
constexpr uint32_t kSubScenes = 16, kLeaves = 32, kAnimations = 8;
constexpr uint32_t kRootAt = kMovieCount * kMovieHeaderBytes;
constexpr uint32_t kSubScenesAt = kRootAt + kTreeNodeBytes;
constexpr uint32_t kLeavesAt = kSubScenesAt + kSubScenes * kTreeNodeBytes;
constexpr uint32_t kAnimationsAt = kLeavesAt + kLeaves * kTreeNodeBytes;
constexpr uint32_t kPairsAt = kAnimationsAt + kAnimations * 64;
constexpr uint32_t kTexturesAt = kPairsAt + 0x40;
constexpr uint32_t kMovieWorldBytes = kTexturesAt + 0x100;
constexpr uint32_t kTextsPerMovie = 8;
constexpr uint32_t kTextBuffersAt = 0x1000;
constexpr uint32_t kTextWorldBytes = kTextBuffersAt + kMovieCount * kTextsPerMovie * 0x1000;
constexpr uint32_t kStringWorldBytes = 0x10000;
constexpr uint32_t kMissingText = 0x826C5CD8;  // TextMap::Find's answer when not found
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTagTxt = 0x54585400;
constexpr uint32_t kTagSce = 0x53434500;

// The world: the seven blocks the screen's globals point at (0x400 bytes
// each: the page block, the menu, the settings, then four more); player 0's
// save data (its options at +0x1F85C); a misc block (the machine at +0, its
// words up to +9616; the dispatcher at +0x2600, its ring at +204; the fake
// sound manager at +0x3000 and its vtable at +0x3400; the movie table's
// pairs at +0x3800, unused now); the UI movie world.
constexpr uint32_t kBlockBytes = 0x400;
constexpr uint32_t kSaveBytes = 0x20000;
constexpr uint32_t kMiscBytes = 0x4000;
constexpr uint32_t kSaveDataPointers = 0x826C5B58;
constexpr uint32_t kMissingMovie = 0x826C5CDC;  // UiMovies::Find's answer when not found
// The getter sub_82090770 (r3 = +60), the sound manager's slots 29 and 30.
constexpr uint32_t kGet60 = 0x82090770;
struct World {
  World()
      : blocks(7 * kBlockBytes),
        save(kSaveBytes),
        misc(kMiscBytes),
        movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes) {}
  bool ok() const {
    return blocks.address != 0 && save.address != 0 && misc.address != 0 &&
           movies.address != 0 && texts.address != 0 && strings.address != 0;
  }
  uint32_t movie(uint32_t m) const { return movies.address + kMovieHeaderBytes * m; }
  uint32_t root() const { return movies.address + kRootAt; }
  uint32_t sub(uint32_t k) const { return movies.address + kSubScenesAt + kTreeNodeBytes * k; }
  uint32_t leaf(uint32_t k) const { return movies.address + kLeavesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  uint32_t machine() const { return misc.address; }
  uint32_t dispatcher() const { return misc.address + 0x2600; }
  uint32_t manager() const { return misc.address + 0x3000; }
  uint32_t options() const { return save.address + os::kSaveOptions; }
  uint32_t block(uint32_t global) const {
    return blocks.address + (global - kGlobals) / 4 * kBlockBytes;
  }
  uint32_t page() const { return block(os::kPage); }
  uint32_t menu() const { return block(os::kMenu); }
  uint32_t settings() const { return block(os::kSettings); }
  Block blocks;
  Block save;
  Block misc;
  Block movies;
  Block texts;
  Block strings;
};

// The globals a case points at its world, put back after.
const std::vector<std::pair<uint32_t, uint32_t>> kSavedGlobals = {
    {kGlobals, kGlobalsBytes},
    {kSaveDataPointers, 4},
    {os::kModeType, 4},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4}};

// Whether a case's movie table leaves movie 7 out (its pair's id 9): for
// the functions that delete it (UiMovies::Delete would free our block).
bool g_movie7_missing = false;

// The string table: 17 files' pointers to one container of entries enough
// for the widest file (the bounds at 0x82007E18), each one of 8 strings of
// letters (no argument codes), as the race screens' harness builds it.
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
    ITC_LOG_ERROR("harness: options_screen: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x4F7074696F6E73ull);  // "Options"
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
      guest::StoreU32(base, movie + field, w.root());
    }
    guest::StoreU32(base, movie + 28, w.movies.address + kTexturesAt);
    for (uint32_t k = 0; k < 512; k++) {  // the options' named animations reach 455
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, w.root());
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
  guest::StoreU32(base, w.root(), kTagSce);
  guest::StoreU32(base, w.root() + 100, 0);  // the pose resets walk children 0 to 3
  guest::StoreU32(base, w.root() + 104, r.Below(5));
  guest::StoreU32(base, w.root() + 108, r.Below(4));
  guest::StoreU32(base, w.root() + 116, 60);
  for (uint32_t i = 0; i < kEntries; i++) {
    guest::StoreU32(base, w.root() + 128 + 8 * i,
                    i < 60 ? w.sub(r.Below(kSubScenes)) : w.animation(r.Below(kAnimations)));
  }
  for (uint32_t k = 0; k < kSubScenes; k++) {
    const uint32_t sub = w.sub(k);
    guest::StoreU32(base, sub, kTagSce);
    guest::StoreU32(base, sub + 100, r.Below(16));
    guest::StoreU32(base, sub + 104, r.Below(7));
    guest::StoreU32(base, sub + 108, r.Below(4));
    guest::StoreU32(base, sub + 116, 64);
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, sub + 128 + 8 * i,
                      i < 64 ? w.leaf(r.Below(kLeaves)) : w.animation(r.Below(kAnimations)));
    }
  }
  for (uint32_t k = 0; k < kLeaves; k++) {
    const uint32_t leaf = w.leaf(k);
    guest::StoreU32(base, leaf, kTagTxt);
    guest::StoreU32(base, leaf + 100, r.Below(16));
    guest::StoreU32(base, leaf + 104, r.Below(8) == 0 ? 0 : r.Below(7));
    guest::StoreU32(base, leaf + 108, r.Below(9));
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, leaf + 128 + 8 * i, w.leaf(r.Below(kLeaves)));
    }
  }
  const uint32_t pairs = w.movies.address + kPairsAt;
  guest::StoreU32(base, pairs, g_movie7_missing ? 9 : 7);
  guest::StoreU32(base, pairs + 4, w.movie(0));
  guest::StoreU32(base, pairs + 8, 14);
  guest::StoreU32(base, pairs + 12, w.movie(1));
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, kMovieCount);
  guest::StoreU32(base, kMissingMovie, w.movie(0));
  guest::StoreU32(base, kStringTable, w.strings.address);
}

// UiMovie::Run's update calls, kept and answered.
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall>* g_updates = nullptr;
Random g_update_answers;

bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_updates == nullptr || function != 0x8211D2F8) {
    return false;
  }
  g_updates->push_back({uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_update_answers.Below(2) == 0 ? 0 : g_update_answers.Next();
  return true;
}

// The mode type: 2, 4, 6, 7 or 8, now and then another.
uint32_t ModeType(Random& r) {
  static constexpr uint32_t kTypes[6] = {2, 4, 6, 7, 8, 3};
  return kTypes[r.Below(6)];
}

void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.blocks.address, w.blocks.bytes, r);
  WriteRandom(base, w.misc.address, w.misc.bytes, r);
  // Only the save data's parts the functions read or write (+0 to +0x2000
  // and the options), not all of it, and the movie's flag.
  WriteRandom(base, w.save.address, 0x2000, r);
  WriteRandom(base, w.options() - 0x40, 0x200, r);
  for (uint32_t g = kGlobals; g < kGlobals + kGlobalsBytes; g += 4) {
    guest::StoreU32(base, g, w.block(g));
  }
  guest::StoreU32(base, kSaveDataPointers, w.save.address);
  guest::StoreU32(base, os::kModeType, ModeType(r));
  // The sound manager: slots 29 and 30 the getter; its +60 -1 now and then.
  const uint32_t manager = w.manager(), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60, r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(r.Next()));
  guest::StoreU32(base, vtable + menu_sounds::kGetCategorySlot, kGet60);
  guest::StoreU32(base, vtable + menu_sounds::kSetCategoryVolumeSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
  // The UI movies; movie 7's loaded flag 0 or 1.
  ShapeMovieWorld(base, w, r);
  guest::StoreU32(base, w.movie(0) + ui_movies::kMovieLoaded, r.Below(2));
  // The machine's words: +9608, +9612 0 or 1 or any.
  for (const uint32_t field : {9608u, 9612u}) {
    guest::StoreU32(base, w.machine() + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  // The dispatcher's ring: its count and start small.
  guest::StoreU32(base, w.dispatcher() + 204 + 600, r.Below(50));
  guest::StoreU32(base, w.dispatcher() + 204 + 604, r.Below(60));
}

// UiMovies::Open's and Delete's calls, recorded on both sides (their
// adapters hand them to the start-up recorder) and answered 1: Delete would
// free our movie block.
std::vector<uint32_t> g_opens;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (address != 0x8211E808 && address != 0x8211E258) return false;
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens.push_back(address);
  g_opens.insert(g_opens.end(), {ctx.r3.u32, address == 0x8211E258 ? 0u : ctx.r4.u32, ctx.r1.u32,
                                 uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read (the allocated
// blocks compared and freed).
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

// The original's frames compared, as [sp - from, sp - to) ranges (its
// register saves left out).
using Frames = std::vector<std::pair<uint32_t, uint32_t>>;

void Case(Context& h, Results& results, const World& w, uint64_t seed, const SetupFn& setup,
          GuestFunction recompiled, GuestFunction native, const Frames& frames,
          const std::string& what, const AfterFn& after) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Random r(seed);
    SavedBytes saved(h.base, kSavedGlobals);
    PPCContext t = CallContext(h);
    const uint32_t sp = t.r1.u32;
    WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
    ShapeWorld(h.base, w, r);
    t.r3.u64 = r.Next();
    t.r4.u64 = r.Next();
    setup(t, r, w);
    t.lr = 0x82000000 | (r.Below(0x10000) << 2);
    g_opens.clear();
    const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
    hooks::g_start_up_recorder = RecordOpen;
    g_update_answers = Random(r.Next());
    std::vector<UpdateCall> updates;
    g_updates = &updates;
    const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
        ui_movies_hooks::g_update_callee_stand_in;
    ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
    ITC_FLAG(native_options_screen) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_options_screen) = true;
    ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
    g_updates = nullptr;
    hooks::g_start_up_recorder = saved_recorder;
    if (after) {
      after(t, w, out[v]);
    }
    for (const uint32_t word : g_opens) out[v].Word(word);
    out[v].Word(uint32_t(updates.size()));
    for (const UpdateCall& u : updates) {
      out[v].Word(u.lr);
      out[v].Word(u.r1);
      for (const uint64_t x : {u.r3, u.r4}) {
        out[v].Word(uint32_t(x >> 32));
        out[v].Word(uint32_t(x));
      }
    }
    out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
    out[v].Bytes(Read(h.base, w.texts.address, w.texts.bytes));
    out[v].Bytes(Read(h.base, w.blocks.address, w.blocks.bytes));
    out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
    out[v].Bytes(Read(h.base, w.save.address, 0x2000));
    out[v].Bytes(Read(h.base, w.options() - 0x40, 0x200));
    for (const auto& [address, bytes] : kSavedGlobals) {
      out[v].Bytes(Read(h.base, address, bytes));
    }
    for (const auto& [from, to] : frames) {
      out[v].Bytes(Read(h.base, sp - from, from - to));
    }
    out[v].Bytes(Read(h.base, sp, kCallerBytes));
    out[v].Word(t.r1.u32);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Frames& frames = {},
           const AfterFn& after = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    Case(h, results, w, h.random.Next(), setup, recompiled, native, frames,
         fmt::format("case {}", c), after);
  }
  results.Report();
}

// --- The controls' tables and the settings' leaves ---

void Leaves(Context& h, const World& w) {
  // The tables: every code from -2 to 33, then any words.
  const auto code = [](PPCContext& t, Random& r, const World&) {
    t.r3.u64 = r.Below(4) == 0 ? r.Next() : uint64_t(int64_t(Around(r, -2, 33)) & 0xFFFFFFFF) |
                                                (uint64_t(r.Next()) << 32);
  };
  Group(h, w, "Controls::ButtonIndex (sub_821BA950)", 120, code, __imp__sub_821BA950,
        hooks::OptionsButtonIndex);
  Group(h, w, "Controls::ButtonCode (sub_821BAA18)", 120, code, __imp__sub_821BAA18,
        hooks::OptionsButtonCode);
  Group(h, w, "Controls::AnalogIndex (sub_821BAAE0)", 120, code, __imp__sub_821BAAE0,
        hooks::OptionsAnalogIndex);
  Group(h, w, "Controls::AnalogPicture (sub_821BAD08)", 120, code, __imp__sub_821BAD08,
        hooks::OptionsAnalogPicture);
  // The edits: values 0 to 5 (clashes likely), now and then any word; the
  // item 0 to 11 (or 0 to 5), now and then -1 to 16 (or 10): the frame's
  // neighbors, not any word (the original reads at sp - 48 + 4 item).
  const auto edits = [&h](const World& w, Random& r, uint32_t from, uint32_t to) {
    for (uint32_t f = from; f <= to; f += 4) {
      guest::StoreU32(h.base, w.settings() + f, r.Below(10) == 0 ? uint32_t(r.Next()) : r.Below(6));
    }
  };
  Group(
      h, w, "Options::IsButtonTaken (sub_821BA7E0)", 150,
      [&](PPCContext& t, Random& r, const World& w) {
        edits(w, r, 284, 328);
        t.r3.u64 = r.Below(8) == 0 ? uint64_t(int64_t(r.Below(18)) - 1) : r.Below(12);
      },
      __imp__sub_821BA7E0, hooks::OptionsIsButtonTaken, {{48, 0}});
  Group(
      h, w, "Options::IsAnalogTaken (sub_821BAB58)", 150,
      [&](PPCContext& t, Random& r, const World& w) {
        edits(w, r, 380, 400);
        t.r3.u64 = r.Below(8) == 0 ? uint64_t(int64_t(r.Below(12)) - 1) : r.Below(6);
      },
      __imp__sub_821BAB58, hooks::OptionsIsAnalogTaken, {{32, 0}});
  // The steps: the menu's side -1, 0, 1 or any, its cursor around the
  // items, the OK/Default word 0, 1 or any.
  const auto step = [&h](int32_t last) {
    return [&h, last](PPCContext&, Random& r, const World& w) {
      const uint32_t sides[4] = {0xFFFFFFFFu, 0, 1, uint32_t(r.Next())};
      guest::StoreU32(h.base, w.menu() + 36, sides[r.Below(4)]);
      guest::StoreU32(h.base, w.menu() + 0,
                      r.Below(2) == 0 ? uint32_t(last) : Around(r, -1, last + 1));
      guest::StoreU32(h.base, w.settings() + 660, r.Below(5) == 0 ? uint32_t(r.Next()) : r.Below(2));
    };
  };
  Group(h, w, "Options::StepButtons (sub_821BA550)", 120, step(12), __imp__sub_821BA550,
        hooks::OptionsStepButtons);
  Group(h, w, "Options::StepAnalog (sub_821BABD8)", 120, step(6), __imp__sub_821BABD8,
        hooks::OptionsStepAnalog);
  Group(
      h, w, "Settings::Reset (sub_821B9DB8)", 30,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.settings(); },
      __imp__sub_821B9DB8, hooks::OptionsSettingsReset, {{112, 24}});
}

// --- The settings' read and write, and the machine's handlers ---

void Handlers(Context& h, const World& w) {
  // The settings: the controls' slots +92 to +232 indexes -2 to 19, now and
  // then any; the volumes and sliders any words (their low bytes written).
  const auto slots = [&h](const World& w, Random& r) {
    for (uint32_t f = 92; f <= 232; f += 4) {
      guest::StoreU32(h.base, w.settings() + f,
                      r.Below(10) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(22)) - 2));
    }
  };
  // The save's records: codes 0 to 33, now and then any word.
  const auto records = [&h](const World& w, Random& r) {
    for (uint32_t f = 44; f < 284; f += 4) {
      guest::StoreU32(h.base, w.options() + f,
                      r.Below(10) == 0 ? uint32_t(r.Next()) : r.Below(34));
    }
    // The day: small, negative, or past 999.
    const uint32_t days[4] = {r.Below(400), uint32_t(-int32_t(r.Below(50))), 900 + r.Below(300),
                              uint32_t(r.Next())};
    guest::StoreU32(h.base, w.save.address + 6100, days[r.Below(4)]);
  };
  Group(
      h, w, "Settings::Read (sub_821BBF00)", 120,
      [&](PPCContext&, Random& r, const World& w) {
        records(w, r);
        // The context +4: 1 half the time (type 2's career info read).
        if (r.Below(2) == 0) {
          guest::StoreU32(h.base, os::kModeType, 2);
        }
      },
      __imp__sub_821BBF00, hooks::OptionsSettingsRead, {{272, 56}});
  Group(
      h, w, "Settings::Write (sub_821BAE10)", 120,
      [&](PPCContext&, Random& r, const World& w) { slots(w, r); }, __imp__sub_821BAE10,
      hooks::OptionsSettingsWrite, {{176, 32}});
  Group(
      h, w, "Options::OnClosed (sub_821BCE38)", 80,
      [&](PPCContext& t, Random& r, const World& w) {
        slots(w, r);
        t.r3.u64 = w.machine();
        t.r4.u64 = w.dispatcher();
      },
      __imp__sub_821BCE38, hooks::OptionsOnClosed, {{112, 24}, {288, 144}});
  Group(
      h, w, "Options::WaitForMovie (sub_821BBBA0)", 80,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.machine(); }, __imp__sub_821BBBA0,
      hooks::OptionsWaitForMovie, {{96, 16}});
  Group(
      h, w, "Options::OpenMovie (sub_821B9F88)", 10, [](PPCContext&, Random&, const World&) {},
      __imp__sub_821B9F88, hooks::OptionsOpenMovie);
  // OnEnter allocates over the layer, as its adapter runs it: the seven
  // blocks compared and freed.
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_ERROR("harness: options_screen: the native heap isn't active; OnEnter skipped");
    return;
  }
  Group(
      h, w, "Options::OnEnter (sub_821BB9A8)", 40,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.machine(); }, __imp__sub_821BB9A8,
      hooks::OptionsOnEnter, {{128, 40}},
      [&h](PPCContext& t, const World& w, Outputs& out) {
        static constexpr uint32_t kBytes[7] = {28, 68, 716, 28, 28, 68, 28};
        out.Word(t.r3.u32 == guest::LoadU32(h.base, 0x826D8488));
        for (uint32_t i = 0; i < 7; i++) {
          const uint32_t slot = kGlobals + 4 * i;
          const uint32_t block = guest::LoadU32(h.base, slot);
          if (block == 0 || !memory::IsPages(block)) {
            out.Word(0xDEAD0000 | (block != 0));
            continue;
          }
          out.Bytes(Read(h.base, block, kBytes[i]));
          memory::FreePages(block);
          guest::StoreU32(h.base, slot, w.block(slot));
        }
        t.r3.u64 = 0;
      });
}

// --- The handlers on the movie ---

void MovieHandlers(Context& h, const World& w) {
  const auto machine = [](PPCContext& t, Random&, const World& w) {
    t.r3.u64 = w.machine();
    t.r4.u64 = w.dispatcher();
  };
  // Close and Leave delete movie 7 (recorded). Leave's movie missing now and
  // then (out of the table, the missing word 0: none finished or deleted).
  Group(
      h, w, "Options::Close (sub_821BCD28)", 80,
      [&](PPCContext& t, Random& r, const World& w) {
        machine(t, r, w);
        guest::StoreU32(h.base, w.machine() + 9616, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
      },
      __imp__sub_821BCD28, hooks::OptionsClose, {{112, 24}});
  Group(
      h, w, "Options::Leave (sub_821BCF08)", 80,
      [&](PPCContext& t, Random& r, const World& w) {
        machine(t, r, w);
        if (r.Below(3) == 0) {
          guest::StoreU32(h.base, w.movies.address + kPairsAt, 9);
          guest::StoreU32(h.base, kMissingMovie, 0);
        }
      },
      __imp__sub_821BCF08, hooks::OptionsLeave, {{112, 16}});
  Group(
      h, w, "Options::ClosePage (sub_821BD330)", 60,
      [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.dispatcher(); },
      __imp__sub_821BD330, hooks::OptionsClosePage);
  // The steps: r3 the page block's step word (+24), 0 or 1; the counter
  // +684 around 0 to 12.
  const auto step = [&h](PPCContext& t, Random& r, const World& w) {
    guest::StoreU32(h.base, w.page() + 24, r.Below(2));
    guest::StoreU32(h.base, w.settings() + 684, Around(r, -1, 12));
    t.r3.u64 = w.page() + 24;
  };
  Group(h, w, "Options::Wait (sub_821BD3C8)", 80, step, __imp__sub_821BD3C8, hooks::OptionsWait,
        {{112, 24}});
  // The steps on the movie: the step word 0 or 1, the menu's cursor 0 to 5
  // (the animations' indexes stay in the world's nodes), the context 0 to
  // 6, OK/Default 0 or 1, the first button edit 0 to 16 (the picture's
  // table index).
  const auto movie_step = [&h, step](PPCContext& t, Random& r, const World& w) {
    step(t, r, w);
    guest::StoreU32(h.base, w.menu() + 0, r.Below(6));
    guest::StoreU32(h.base, w.settings() + 4, r.Below(7));
    guest::StoreU32(h.base, w.settings() + 660, r.Below(2));
    guest::StoreU32(h.base, w.settings() + 284, r.Below(17));
  };
  struct StepCase {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t frame, saves;
  };
  const StepCase kSteps[] = {
      {"Options::OpenCategories (sub_821BD468)", __imp__sub_821BD468,
       hooks::OptionsOpenCategories, 128, 32},
      {"Options::ReturnToCategories (sub_821BD5B0)", __imp__sub_821BD5B0,
       hooks::OptionsReturnToCategories, 128, 32},
      {"Options::EnterHud (sub_821BD728)", __imp__sub_821BD728, hooks::OptionsEnterHud, 128, 32},
      {"Options::ChangeHudValue (sub_821BD800)", __imp__sub_821BD800, hooks::OptionsChangeHudValue,
       112, 24},
      {"Options::EnterSound (sub_821BD908)", __imp__sub_821BD908, hooks::OptionsEnterSound, 112,
       24},
      {"Options::EnterSliders (sub_821BD9A0)", __imp__sub_821BD9A0, hooks::OptionsEnterSliders,
       112, 24},
      {"Options::EnterButtons (sub_821BDAD0)", __imp__sub_821BDAD0, hooks::OptionsEnterButtons, 208,
       32},
  };
  for (const StepCase& c : kSteps) {
    Group(h, w, c.name, 80, movie_step, c.recompiled, c.native, {{c.frame, c.saves}});
  }
}

// --- The node helpers ---
//
// The frames compared without the words the varargs SetText and
// SetNodeText store from their unset r7 to r10 (+48 to +79 of the frame:
// passed as 0 by ours, the user's call, 2026-10-30).

void NodeHelpers(Context& h, const World& w) {
  // A small value or any word.
  const auto value = [](Random& r, int32_t lo, int32_t hi) -> uint64_t {
    return r.Below(6) == 0 ? r.Next() : uint64_t(int64_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1)))));
  };
  Group(
      h, w, "Options::SetCategoryArrow (sub_821BE218)", 60,
      [&](PPCContext& t, Random& r, const World&) { t.r3.u64 = value(r, -200, 200); },
      __imp__sub_821BE218, hooks::OptionsSetCategoryArrow, {{112, 24}});
  Group(
      h, w, "Options::ShowVolumeNumber (sub_821BE518)", 60,
      [&](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.leaf(r.Below(kLeaves));
        t.r4.u64 = value(r, -1, 17);
      },
      __imp__sub_821BE518, hooks::OptionsShowVolumeNumber, {{128, 80}, {48, 32}});
  Group(
      h, w, "Options::ShowMusicName (sub_821BE930)", 40,
      [&](PPCContext& t, Random& r, const World&) { t.r3.u64 = value(r, 0, 6); },
      __imp__sub_821BE930, hooks::OptionsShowMusicName, {{112, 64}, {32, 16}});
  Group(
      h, w, "Options::ShowSlider (sub_821BE988)", 60,
      [&](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.leaf(r.Below(kLeaves));
        t.r4.u64 = value(r, -12, 12);
      },
      __imp__sub_821BE988, hooks::OptionsShowSlider, {{128, 80}, {48, 40}});
  Group(
      h, w, "Options::ShowSliderMark (sub_821BEA48)", 60,
      [&](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.leaf(r.Below(kLeaves));
        t.r4.u64 = value(r, -12, 12);
      },
      __imp__sub_821BEA48, hooks::OptionsShowSliderMark, {{112, 16}});
  Group(
      h, w, "Options::ShowButtonPicture (sub_821BF0F0)", 40,
      [&](PPCContext& t, Random& r, const World&) { t.r3.u64 = value(r, -2, 20); },
      __imp__sub_821BF0F0, hooks::OptionsShowButtonPicture, {{128, 32}});
  Group(
      h, w, "Options::ShowAnalogPicture (sub_821BF630)", 40,
      [&](PPCContext& t, Random& r, const World&) { t.r3.u64 = value(r, -2, 14); },
      __imp__sub_821BF630, hooks::OptionsShowAnalogPicture, {{128, 32}});
  Group(
      h, w, "Options::ResetNode (sub_821BFA68)", 60,
      [&](PPCContext& t, Random& r, const World&) {
        t.r3.u64 = r.Below(3) == 0 ? ~uint64_t(0) : r.Below(6);
        t.r4.u64 = r.Below(10);
      },
      __imp__sub_821BFA68, hooks::OptionsResetNode, {{128, 40}});
}

}  // namespace

void OptionsScreen(Context& h) {
  World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: options_screen: no memory for the world");
    return;
  }
  BuildStringTable(h.base, w);
  Leaves(h, w);
  Handlers(h, w);
  MovieHandlers(h, w);
  NodeHelpers(h, w);
}

}  // namespace harness
