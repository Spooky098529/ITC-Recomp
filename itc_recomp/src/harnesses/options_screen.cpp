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
#include "base/hash.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/options_screen.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "options_screen/options_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_options_screen);
ITC_FLAG_DECLARE(bool, native_options_load_save);

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
// animations of no tracks (900 of them: the options' named animations
// reach 872, below the texts' map at +0x9BCC); the root's child index (+108) 0 to 3, its
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
// The getter sub_82090770 (r3 = +60), the sound manager's slots 10, 29 and
// 30.
constexpr uint32_t kGet60 = 0x82090770;
struct World {
  World()
      : blocks(7 * kBlockBytes),
        save(kSaveBytes),
        misc(kMiscBytes),
        movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes),
        mappers(0x1000) {}
  bool ok() const {
    return blocks.address != 0 && save.address != 0 && misc.address != 0 &&
           movies.address != 0 && texts.address != 0 && strings.address != 0 &&
           mappers.address != 0;
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
  Block mappers;
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
    {kStringTable, 4},
    {menu_helpers::kMapperEntries, 32},
    {menu_helpers::kControllerEntries, 16},
    {0x826367E8 + 1972, 4},
    {0x826371D0 + 1972, 4},
    {0x826364EC, 4},
    {0x826A50AC, 64},  // the screen fades' words (Options::FadeIn's)
    {0x826BDC90, 4},   // the save system's interface (the load and save pages')
    {0x828161F0, 4}};  // the save buffers' header

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
    for (uint32_t k = 0; k < 900; k++) {  // the options' named animations reach 872
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
  // Slot 10 (PlayCue2D, MenuSound::Play's) the getter too.
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, kGet60);
  // Slots 5 (ReleaseBank) and 13 (StopCue), Options::OnLeft's, and 14
  // (IsCuePlaying), Options::SetUpMovie's, too.
  guest::StoreU32(base, vtable + 20, kGet60);
  guest::StoreU32(base, vtable + 52, kGet60);
  guest::StoreU32(base, vtable + 56, kGet60);
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
    ITC_FLAG(native_options_load_save) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_options_screen) = true;
    ITC_FLAG(native_options_load_save) = true;
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


// --- The second migration's page helpers ---

void PageHelpers(Context& h, const World& w) {
  // A small value around [lo, hi] or any word.
  const auto edit = [](Random& r, int32_t lo, int32_t hi) {
    return r.Below(8) == 0 ? uint32_t(r.Next()) : uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1))));
  };
  const auto edits = [&h, edit](const World& w, Random& r, uint32_t from, uint32_t to, int32_t lo,
                                int32_t hi) {
    for (uint32_t f = from; f <= to; f += 4) {
      guest::StoreU32(h.base, w.settings() + f, edit(r, lo, hi));
    }
  };
  Group(
      h, w, "Options::ShowVolumeBar (sub_821BE5A8)", 60,
      [&](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.leaf(r.Below(kLeaves));
        t.r4.u64 = r.Below(6) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(19)) - 1));
      },
      __imp__sub_821BE5A8, hooks::OptionsShowVolumeBar, {{112, 32}});
  // ShowVolumeNumbers: ShowVolumeNumber's frame below its own (its varargs
  // words and register saves left out).
  Group(
      h, w, "Options::ShowVolumeNumbers (sub_821BE820)", 60,
      [&](PPCContext&, Random& r, const World& w) { edits(w, r, 252, 256, -1, 17); },
      __imp__sub_821BE820, hooks::OptionsShowVolumeNumbers, {{112, 24}, {240, 192}, {160, 144}});
  Group(
      h, w, "Options::ShowSliderNumbers (sub_821BECA8)", 60,
      [&](PPCContext&, Random& r, const World& w) { edits(w, r, 260, 268, -12, 12); },
      __imp__sub_821BECA8, hooks::OptionsShowSliderNumbers, {{144, 32}, {272, 224}, {192, 184}});
  Group(
      h, w, "Options::ShowSliderMarks (sub_821BED38)", 60,
      [&](PPCContext&, Random& r, const World& w) { edits(w, r, 260, 268, -12, 12); },
      __imp__sub_821BED38, hooks::OptionsShowSliderMarks, {{144, 32}, {256, 160}});
  Group(
      h, w, "Options::ShowHudValues (sub_821BE290)", 80,
      [&](PPCContext&, Random& r, const World& w) {
        edits(w, r, 236, 248, 0, 1);
        edits(w, r, 660, 660, 0, 1);
      },
      __imp__sub_821BE290, hooks::OptionsShowHudValues, {{208, 88}});
  // The steps: the menu's side -1, 0, 1 or any, its cursor 0 to 4 (now and
  // then any), the edits around their bounds.
  const auto side = [&h, edit](const World& w, Random& r) {
    const uint32_t sides[4] = {0xFFFFFFFFu, 1, 0, uint32_t(r.Next())};
    guest::StoreU32(h.base, w.menu() + 36, sides[r.Below(4)]);
    guest::StoreU32(h.base, w.menu() + 0, edit(r, -1, 4));
    guest::StoreU32(h.base, w.settings() + 660, edit(r, 0, 1));
  };
  Group(
      h, w, "Options::StepSound (sub_821BE5F8)", 150,
      [&](PPCContext&, Random& r, const World& w) {
        side(w, r);
        edits(w, r, 252, 256, -1, 16);
        edits(w, r, 688, 688, -1, 17);
      },
      __imp__sub_821BE5F8, hooks::OptionsStepSound);
  Group(
      h, w, "Options::StepSliders (sub_821BEAC8)", 150,
      [&](PPCContext&, Random& r, const World& w) {
        side(w, r);
        edits(w, r, 260, 268, -11, 11);
      },
      __imp__sub_821BEAC8, hooks::OptionsStepSliders, {{112, 16}});

  // The lists: the edits 0 to `hi` only (stored into the picture nodes'
  // +108, the world's child index: any word would send later lookups out of
  // the world). The clashes: the edits 0 to 5 (clashes likely), now and then
  // any word. The lists' frames without SetNodeText's varargs words (+40 to
  // +79: r6 to r10, unset by the original, passed as 0 by ours); the
  // clashes' with IsButtonTaken's (IsAnalogTaken's) copy below them.
  const auto small = [&h](const World& w, Random& r, uint32_t from, uint32_t to, uint32_t hi) {
    for (uint32_t f = from; f <= to; f += 4) {
      guest::StoreU32(h.base, w.settings() + f, r.Below(hi + 1));
    }
  };
  Group(
      h, w, "Options::ShowButtonList (sub_821BEFE8)", 60,
      [&](PPCContext&, Random& r, const World& w) { small(w, r, 284, 328, 16); },
      __imp__sub_821BEFE8, hooks::OptionsShowButtonList, {{208, 168}, {128, 56}});
  Group(
      h, w, "Options::ShowAnalogList (sub_821BF520)", 60,
      [&](PPCContext&, Random& r, const World& w) { small(w, r, 380, 400, 10); },
      __imp__sub_821BF520, hooks::OptionsShowAnalogList, {{208, 168}, {128, 48}});
  Group(
      h, w, "Options::ShowButtonClashes (sub_821BF158)", 80,
      [&](PPCContext&, Random& r, const World& w) { edits(w, r, 284, 328, 0, 5); },
      __imp__sub_821BF158, hooks::OptionsShowButtonClashes, {{304, 40}});
  Group(
      h, w, "Options::ShowAnalogClashes (sub_821BF698)", 80,
      [&](PPCContext&, Random& r, const World& w) { edits(w, r, 380, 400, 0, 3); },
      __imp__sub_821BF698, hooks::OptionsShowAnalogClashes, {{288, 40}});
  // The steps: r3 the step word (0 or 1); the analog edit -2 to 20; the
  // dialog menu's cursors 0 to 3; the dialogs' states 0 to 3 or any.
  const auto dialog_step = [&h, edit](PPCContext& t, Random& r, const World& w) {
    guest::StoreU32(h.base, w.page() + 24, r.Below(2));
    t.r3.u64 = w.page() + 24;
    guest::StoreU32(h.base, w.settings() + 380, edit(r, -2, 20));
    const uint32_t menu = w.block(0x826D8484);
    guest::StoreU32(h.base, menu + 0, r.Below(4));
    guest::StoreU32(h.base, menu + 4, r.Below(4));
    for (const uint32_t f : {620u, 640u}) {
      guest::StoreU32(h.base, w.settings() + f, edit(r, 0, 3));
    }
  };
  Group(h, w, "Options::EnterAnalog (sub_821BDE88)", 80, dialog_step, __imp__sub_821BDE88,
        hooks::OptionsEnterAnalog, {{176, 32}});
  Group(h, w, "Options::MoveSaveDialogCursor (sub_821C09B0)", 60, dialog_step,
        __imp__sub_821C09B0, hooks::OptionsMoveSaveDialogCursor, {{128, 32}});
  Group(h, w, "Options::MoveCategory6DialogCursor (sub_821C0B50)", 60, dialog_step,
        __imp__sub_821C0B50, hooks::OptionsMoveCategory6DialogCursor, {{128, 32}});
  Group(h, w, "Options::ChangeSaveDialog (sub_821C08B0)", 100, dialog_step, __imp__sub_821C08B0,
        hooks::OptionsChangeSaveDialog, {{128, 32}});
  Group(h, w, "Options::ChangeCategory6Dialog (sub_821C0A50)", 100, dialog_step,
        __imp__sub_821C0A50, hooks::OptionsChangeCategory6Dialog, {{128, 32}});
}


// --- The machine's dispatch and the page helpers ---

// The players' action mappers (Menu::Pressed's), as the type 6 and 7 modes'
// harness shapes them, and the options' menu's fields Pressed reads.
void ShapeMappers(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.mappers.address, w.mappers.bytes, r);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t vt = w.mappers.address + 0x100 + 0x40 * k;
    const uint32_t mapper = w.mappers.address + 0x200 + 0x200 * k;
    guest::StoreU32(base, menu_helpers::kControllerEntries + 4 * k, r.Below(4));
    guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k, mapper);
    guest::StoreU32(base, menu_helpers::kMapperEntries + 8 * k + 4, r.Below(5) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper, vt);
    guest::StoreU32(base, vt + menu_helpers::kPressedOffset, menu_helpers::kPressedSlot);
    guest::StoreU32(base, vt + menu_helpers::kFiredOffset, menu_helpers::kFiredSlot);
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t bits = r.Below(3) == 0 ? 0 : uint32_t(r.Next() & r.Next());
      guest::StoreU32(base, mapper + 448 + 4 * i, bits);
    }
  }
  const uint32_t menu = w.menu();
  guest::StoreU32(base, menu + menu_helpers::kPlayer, r.Below(4));
  guest::StoreU32(base, menu + menu_helpers::kMoved, r.Below(4) == 0 ? 1 : 0);
  guest::StoreU32(base, menu + menu_helpers::kSide, r.Below(4) == 0 ? 1 : 0);
}

void Dispatch(Context& h, const World& w) {
  const auto pick = [](Random& r, std::initializer_list<int32_t> values) {
    const int32_t* v = values.begin();
    return uint32_t(v[r.Below(uint32_t(values.size()))]);
  };
  // Pages 9 and 10: the cancel word 0, 1 or any; the sub-pages' page among
  // those the world runs (the save pages' 4 and 5, the notes' 5) and
  // negatives; the options' page around 9 and 10.
  const auto save_pages = [&h, pick](const World& w, Random& r) {
    ShapeMappers(h.base, w, r);
    guest::StoreU32(h.base, w.settings() + 708, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
    guest::StoreU32(h.base, w.block(0x826D847C) + 0, pick(r, {-1, -3, 4, 5, 5}));
    guest::StoreU32(h.base, w.block(0x826D8488) + 0, pick(r, {-1, -2, 5, 5}));
    guest::StoreU32(h.base, w.page() + 0, pick(r, {9, 10, 3, -1}));
  };
  Group(
      h, w, "Options::SavePage (sub_821C6F58)", 100,
      [&](PPCContext& t, Random& r, const World& w) {
        save_pages(w, r);
        t.r3.u64 = w.dispatcher();
      },
      __imp__sub_821C6F58, hooks::OptionsSavePage, {{128, 8}});
  Group(
      h, w, "Options::Category6Page (sub_821BD228)", 100,
      [&](PPCContext& t, Random& r, const World& w) {
        save_pages(w, r);
        t.r3.u64 = w.dispatcher();
        t.r4.u64 = w.dispatcher();
      },
      __imp__sub_821BD228, hooks::OptionsCategory6Page, {{112, 8}});
  // Draw: no page, the close (14), or pages 9 and 10 (theirs as above).
  Group(
      h, w, "Options::Draw (sub_821C80F0)", 100,
      [&](PPCContext& t, Random& r, const World& w) {
        save_pages(w, r);
        guest::StoreU32(h.base, w.page() + 0, pick(r, {-1, 14, 9, 10}));
        t.r3.u64 = w.machine();
        t.r4.u64 = w.dispatcher();
      },
      __imp__sub_821C80F0, hooks::OptionsDraw, {{224, 80}});
  // Update: the step none (-1, with the page 14 or not), other negatives,
  // or one the world runs (the native steps); the queued step -1 or one;
  // the steps' fields as their own cases shape them.
  Group(
      h, w, "Options::Update (sub_821C81E8)", 200,
      [&](PPCContext&, Random& r, const World& w) {
        const uint32_t p = w.page();
        guest::StoreU32(h.base, p + 12,
                        pick(r, {-1, -1, -2, -7, 0, 4, 5, 9, 11, 13, 17, 25, 33, 44, 45, 47, 48}));
        guest::StoreU32(h.base, p + 16, pick(r, {-1, -1, 3, 9}));
        guest::StoreU32(h.base, p + 0, pick(r, {14, 2, 0}));
        guest::StoreU32(h.base, p + 24, r.Below(2));
        guest::StoreU32(h.base, w.settings() + 684, Around(r, -1, 12));
        guest::StoreU32(h.base, w.menu() + 0, r.Below(6));
        guest::StoreU32(h.base, w.settings() + 4, r.Below(7));
        guest::StoreU32(h.base, w.settings() + 660, r.Below(2));
        guest::StoreU32(h.base, w.settings() + 284, r.Below(17));
        guest::StoreU32(h.base, w.settings() + 380, r.Below(19));
        const uint32_t menu = w.block(0x826D8484);
        guest::StoreU32(h.base, menu + 0, r.Below(4));
        guest::StoreU32(h.base, menu + 4, r.Below(4));
        for (const uint32_t f : {620u, 640u}) {
          guest::StoreU32(h.base, w.settings() + f, r.Below(4));
        }
      },
      __imp__sub_821C81E8, hooks::OptionsUpdate, {{480, 152}});
  // StepHud: the side -1, 1, 0 or any; the cursor 0 to 4; the values 0, 1
  // or any. ShowHudValues' frame below its own.
  Group(
      h, w, "Options::StepHud (sub_821BE3E0)", 150,
      [&](PPCContext&, Random& r, const World& w) {
        const uint32_t sides[4] = {0xFFFFFFFFu, 1, 0, uint32_t(r.Next())};
        guest::StoreU32(h.base, w.menu() + 36, sides[r.Below(4)]);
        guest::StoreU32(h.base, w.menu() + 0, r.Below(5));
        for (const uint32_t f : {236u, 240u, 244u, 248u, 660u}) {
          guest::StoreU32(h.base, w.settings() + f,
                          r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
        }
      },
      __imp__sub_821BE3E0, hooks::OptionsStepHud, {{112, 8}, {320, 200}});
  Group(
      h, w, "Options::ShowVolumeBars (sub_821BE8A8)", 60,
      [&](PPCContext&, Random& r, const World& w) {
        for (const uint32_t f : {252u, 256u}) {
          guest::StoreU32(h.base, w.settings() + f, Around(r, -1, 17));
        }
      },
      __imp__sub_821BE8A8, hooks::OptionsShowVolumeBars, {{112, 24}, {224, 144}});
  // ShowPrompts: each frame -1 or 0 to 8 (they land in the world's child
  // indexes).
  Group(
      h, w, "Options::ShowPrompts (sub_821C1068)", 100,
      [&](PPCContext& t, Random& r, const World&) {
        PPCRegister* regs[6] = {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7, &t.r8};
        for (PPCRegister* reg : regs) {
          reg->u64 = r.Below(4) == 0 ? ~uint64_t(0) : r.Below(9);
        }
      },
      __imp__sub_821C1068, hooks::OptionsShowPrompts, {{176, 56}});
  // Settings::Reload: the context 0 to 4 or any (1 runs ShowCareerInfo, the
  // original, on both sides). Settings::Read's frame below its own.
  Group(
      h, w, "Settings::Reload (sub_821C0FF0)", 100,
      [&](PPCContext&, Random& r, const World& w) {
        guest::StoreU32(h.base, w.settings() + 4, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(5));
        for (uint32_t f = 44; f < 284; f += 4) {
          guest::StoreU32(h.base, w.options() + f, r.Below(34));
        }
      },
      __imp__sub_821C0FF0, hooks::OptionsSettingsReload, {{96, 8}, {368, 152}});
  // OnLeft frees the seven blocks: each case gives it seven of the
  // allocation layer's pages (as OnEnter's), compared freed after.
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_ERROR("harness: options_screen: the native heap isn't active; OnLeft skipped");
    return;
  }
  Group(
      h, w, "Options::OnLeft (sub_821B9E70)", 40,
      [&](PPCContext&, Random&, const World&) {
        for (uint32_t i = 0; i < 7; i++) {
          guest::StoreU32(h.base, kGlobals + 4 * i, memory::AllocPages(64, 4096));
        }
      },
      __imp__sub_821B9E70, hooks::OptionsOnLeft, {{160, 64}},
      [&h](PPCContext&, const World&, Outputs& out) {
        for (uint32_t i = 0; i < 7; i++) {
          out.Word(guest::LoadU32(h.base, kGlobals + 4 * i));
        }
      });
}


// --- The category page, the movie's set-up and the cursor steps ---

void CursorSteps(Context& h, const World& w) {
  // The options' menu for Menu::Step: six items, its flags 0 or 1.
  const auto menu = [&h](const World& w, Random& r) {
    ShapeMappers(h.base, w, r);
    const uint32_t m = w.menu();
    guest::StoreU32(h.base, m + menu_helpers::kFirst, 0);
    guest::StoreU32(h.base, m + menu_helpers::kLast, 5);
    guest::StoreU32(h.base, m + menu_helpers::kCursor, r.Below(6));
    for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                             menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
      guest::StoreU32(h.base, m + o, r.Below(2));
    }
  };
  Group(
      h, w, "Options::CategoryPage (sub_821BC670)", 150,
      [&](PPCContext&, Random& r, const World& w) {
        menu(w, r);
        guest::StoreU32(h.base, w.page() + 0, r.Below(4) == 0 ? 14 : r.Below(3) - 1);
      },
      __imp__sub_821BC670, hooks::OptionsCategoryPage, {{96, 16}});
  // SetUpMovie: the context 0 to 6 or any; type 2's name record's kinds 0
  // to 5 and small indexes, the day and weekday small.
  const auto set_up = [&h](const World& w, Random& r) {
    guest::StoreU32(h.base, w.settings() + 4, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(7));
    const uint32_t name = w.settings() + 12;
    guest::StoreU32(h.base, name + 4, r.Below(6));
    guest::StoreU32(h.base, name + 8, r.Below(100) << 16);
    guest::StoreU32(h.base, name + 12, r.Below(6));
    guest::StoreU32(h.base, name + 16, r.Below(100) << 16);
    guest::StoreU32(h.base, w.settings() + 32, r.Below(2000));
    guest::StoreU32(h.base, w.settings() + 36, r.Below(7));
    guest::StoreU32(h.base, w.settings() + 40, r.Below(1000));
  };
  Group(
      h, w, "Options::SetUpMovie (sub_821BCFF0)", 100,
      [&](PPCContext&, Random& r, const World& w) { set_up(w, r); }, __imp__sub_821BCFF0,
      hooks::OptionsSetUpMovie, {{160, 104}, {80, 56}});
  // Show: Settings::Read's records (codes 0 to 33) and the mode type (its
  // context) as ShapeWorld picks; r4 the dispatcher. SetUpMovie's and
  // Read's frames below its own.
  Group(
      h, w, "Options::Show (sub_821C1158)", 100,
      [&](PPCContext& t, Random& r, const World& w) {
        for (uint32_t f = 44; f < 284; f += 4) {
          guest::StoreU32(h.base, w.options() + f, r.Below(34));
        }
        guest::StoreU32(h.base, w.save.address + 6100, r.Below(1200));
        guest::StoreU32(h.base, w.page() + 0, r.Below(3) == 0 ? 0 : r.Below(15));
        t.r4.u64 = w.dispatcher();
      },
      __imp__sub_821C1158, hooks::OptionsShow, {{112, 24}, {272, 216}, {192, 168}, {384, 272}});
  // The cursor steps: the step word 0 or 1; the cursor and the cursor
  // before 0 to 5 (the last row 4, 2 or 3 by page), the direction -1, 0, 1
  // or any, the menu's last item 0 to 5; the context 0 to 4; OK/Default 0,
  // 1 or any; the volumes and the track small. The help line's SetText
  // varargs words (+48 to +79 of the frame) left out.
  // ChangeSoundValue's callees' frames below its own: the second
  // ShowVolumeNumber's lower words (its varargs and register saves left
  // out) and ShowMusicName's movie id (the rest of that area the first
  // callee's register saves).
  const auto cursor_step = [&h](PPCContext& t, Random& r, const World& w) {
    guest::StoreU32(h.base, w.page() + 24, r.Below(2));
    t.r3.u64 = w.page() + 24;
    const uint32_t m = w.menu();
    guest::StoreU32(h.base, m + 0, r.Below(6));
    guest::StoreU32(h.base, m + 4, r.Below(6));
    const uint32_t directions[4] = {0xFFFFFFFFu, 0, 1, uint32_t(r.Next())};
    guest::StoreU32(h.base, m + 8, directions[r.Below(4)]);
    guest::StoreU32(h.base, m + 16, r.Below(6));
    guest::StoreU32(h.base, w.settings() + 4, r.Below(5));
    guest::StoreU32(h.base, w.settings() + 660, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
    for (const uint32_t f : {252u, 256u}) {
      guest::StoreU32(h.base, w.settings() + f, r.Below(17));
    }
    guest::StoreU32(h.base, w.settings() + 688, r.Below(7));
  };
  Group(h, w, "Options::MoveCategoryCursor (sub_821C2BE0)", 150, cursor_step,
        __imp__sub_821C2BE0, hooks::OptionsMoveCategoryCursor,
        {{160, 112}, {80, 64}, {272, 184}});
  Group(h, w, "Options::MoveHudCursor (sub_821C2E28)", 150, cursor_step, __imp__sub_821C2E28,
        hooks::OptionsMoveHudCursor, {{160, 112}, {80, 64}});
  Group(h, w, "Options::MoveSoundCursor (sub_821C3008)", 150, cursor_step, __imp__sub_821C3008,
        hooks::OptionsMoveSoundCursor, {{144, 96}, {64, 56}});
  Group(h, w, "Options::ChangeSoundValue (sub_821C3278)", 100, cursor_step, __imp__sub_821C3278,
        hooks::OptionsChangeSoundValue, {{128, 40}, {256, 208}, {160, 156}});
  Group(h, w, "Options::MoveSlidersCursor (sub_821C3398)", 150, cursor_step,
        __imp__sub_821C3398, hooks::OptionsMoveSlidersCursor, {{144, 96}, {64, 56}});
  // ChangeSlidersValue: ShowSliderNumbers' frame below its own.
  Group(
      h, w, "Options::ChangeSlidersValue (sub_821C35F0)", 100,
      [&](PPCContext& t, Random& r, const World& w) {
        cursor_step(t, r, w);
        guest::StoreU32(h.base, w.menu() + 0, r.Below(5));
        for (const uint32_t f : {260u, 264u, 268u}) {
          guest::StoreU32(h.base, w.settings() + f, uint32_t(int32_t(r.Below(21)) - 10));
        }
      },
      __imp__sub_821C35F0, hooks::OptionsChangeSlidersValue, {{112, 24}, {256, 144}});
  // The leaves: the cursor 0 to the last row (now and then past it), the
  // first row shown 0 to 2, the edits and the current settings 0 to 16 (or
  // 10: they land in the world's child indexes), the counter around 0 to 12.
  // Below their frames the clashes' (with the list's varargs and register
  // saves they leave unwritten left out) and the taken check's copy.
  const auto leave = [&h](uint32_t last, uint32_t edits, uint32_t current, uint32_t count,
                          uint32_t hi) {
    return [&h, last, edits, current, count, hi](PPCContext& t, Random& r, const World& w) {
      guest::StoreU32(h.base, w.page() + 24, r.Below(2));
      t.r3.u64 = w.page() + 24;
      guest::StoreU32(h.base, w.menu() + 0, r.Below(8) == 0 ? last + 1 : r.Below(last + 1));
      guest::StoreU32(h.base, w.menu() + 48, r.Below(3));
      for (uint32_t i = 0; i < count; i++) {
        guest::StoreU32(h.base, w.settings() + edits + 4 * i, r.Below(hi + 1));
        guest::StoreU32(h.base, w.settings() + current + 4 * i, r.Below(hi + 1));
      }
      guest::StoreU32(h.base, w.settings() + 660, r.Below(2));
      guest::StoreU32(h.base, w.settings() + 684, Around(r, -1, 12));
    };
  };
  Group(h, w, "Options::LeaveButtons (sub_821C2778)", 100, leave(12, 284, 92, 12, 16),
        __imp__sub_821C2778, hooks::OptionsLeaveButtons, {{336, 40}, {640, 504}, {496, 380}});
  Group(h, w, "Options::LeaveAnalog (sub_821C2A10)", 100, leave(6, 380, 188, 6, 10),
        __imp__sub_821C2A10, hooks::OptionsLeaveAnalog, {{240, 40}, {528, 408}, {400, 284}});
}


// --- The category pages (the third migration) ---

// HasButtonClash and HasAnalogClash have no adapters (never seen in play,
// called only by their pages' plain C++): their register steps here.
void NativeHasButtonClash(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = os::HasButtonClash(base, ctx.r1.u32);
}
void NativeHasAnalogClash(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = os::HasAnalogClash(base, ctx.r1.u32);
}

// A generator of its own for a case's shaping: the cases' seeds follow one
// another in the harness's xorshift stream (each case's stream the one
// before shifted by a word), so the words a case shapes from its own are
// mixed first (splitmix64's finalizer) to keep neighboring cases apart.
Random Mixed(Random& r) {
  uint64_t z = r.Next() + 0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return Random(z ^ (z >> 31));
}

// `count` edits from `at`: half the time distinct values below `hi` (one
// pair made equal now and then), else any of them below `hi`.
void ShapeEdits(uint8_t* base, uint32_t at, uint32_t count, uint32_t hi, Random& r) {
  if (r.Below(2) == 0) {
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, at + 4 * i, r.Below(hi));
    }
    return;
  }
  std::vector<uint32_t> values(hi);
  for (uint32_t i = 0; i < hi; i++) values[i] = i;
  for (uint32_t i = hi - 1; i > 0; i--) std::swap(values[i], values[r.Below(i + 1)]);
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(base, at + 4 * i, values[i]);
  }
  if (r.Below(3) == 0) {
    const uint32_t a = r.Below(count), b = r.Below(count);
    guest::StoreU32(base, at + 4 * b, guest::LoadU32(base, at + 4 * a));
  }
}

void CategoryPages(Context& h, const World& w) {
  Group(
      h, w, "Options::HasButtonClash (sub_821BA890)", 150,
      [&](PPCContext&, Random& from, const World& w) {
        Random r = Mixed(from);
        ShapeEdits(h.base, w.settings() + 284, 12, 17, r);
      },
      __imp__sub_821BA890, NativeHasButtonClash, {{48, 0}});
  Group(
      h, w, "Options::HasAnalogClash (sub_821BAC78)", 150,
      [&](PPCContext&, Random& from, const World& w) {
        Random r = Mixed(from);
        ShapeEdits(h.base, w.settings() + 380, 6, 11, r);
      },
      __imp__sub_821BAC78, NativeHasAnalogClash, {{32, 0}});
  // The pages: the options' menu for Menu::Step (items 0 to `last`, the
  // cursor among them, its flags 0 or 1; the mappers as ShapeMappers makes
  // them); the page block's page this page, -1 or another; OK/Default 0, 1
  // or any; the edits, the current and the default settings.
  const auto page = [&h](uint32_t number, uint32_t last,
                         std::function<void(const World&, Random&)> fields) {
    return [&h, number, last, fields](PPCContext&, Random& from, const World& w) {
      Random r = Mixed(from);
      ShapeMappers(h.base, w, r);
      // Each mapper's fired word (+464: Menu::Step's moves, bits 2 to 5 and
      // 16 to 19 up, down, left, right) one move, none or any bits, and its
      // pressed word (+456: back 158 bit 7, accept 159 bit 8) one, both,
      // none or any, so every path is taken.
      for (uint32_t k = 0; k < 4; k++) {
        const uint32_t mapper = w.mappers.address + 0x200 + 0x200 * k;
        const uint32_t fired[8] = {0, 1u << 2, 1u << 3, 1u << 4, 1u << 5, 1u << 18, 1u << 19,
                                   uint32_t(r.Next() & r.Next())};
        const uint32_t pressed[5] = {0, 1u << 7, 1u << 8, (1u << 7) | (1u << 8), uint32_t(r.Next())};
        guest::StoreU32(h.base, mapper + 464, fired[r.Below(8)]);
        guest::StoreU32(h.base, mapper + 456, pressed[r.Below(5)]);
      }
      const uint32_t m = w.menu();
      guest::StoreU32(h.base, m + menu_helpers::kFirst, 0);
      guest::StoreU32(h.base, m + menu_helpers::kLast, last);
      guest::StoreU32(h.base, m + menu_helpers::kCursor,
                      r.Below(3) == 0 ? last : r.Below(last + 1));
      for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                               menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
        guest::StoreU32(h.base, m + o, r.Below(2));
      }
      const uint32_t pages[3] = {number, 0xFFFFFFFFu, r.Below(15)};
      guest::StoreU32(h.base, w.page() + 0, pages[r.Below(3)]);
      guest::StoreU32(h.base, w.settings() + 660,
                      r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2));
      fields(w, r);
    };
  };
  const auto words = [&h](const World& w, Random& r, std::initializer_list<uint32_t> offsets,
                          uint32_t count, int32_t lo, int32_t hi) {
    for (const uint32_t at : offsets) {
      for (uint32_t i = 0; i < count; i++) {
        guest::StoreU32(h.base, w.settings() + at + 4 * i, uint32_t(lo + int32_t(r.Below(uint32_t(hi - lo + 1)))));
      }
    }
  };
  Group(h, w, "Options::HudPage (sub_821C1270)", 200,
        page(1, 4, [&](const World& w, Random& r) { words(w, r, {44, 236, 428}, 4, 0, 1); }),
        __imp__sub_821C1270, hooks::OptionsHudPage, {{96, 16}});
  Group(h, w, "Options::SlidersPage (sub_821C1888)", 200,
        page(3, 3, [&](const World& w, Random& r) { words(w, r, {68, 260, 452}, 3, -10, 10); }),
        __imp__sub_821C1888, hooks::OptionsSlidersPage, {{96, 16}});
  Group(h, w, "Options::ButtonsPage (sub_821C1D08)", 200,
        page(5, 12,
             [&](const World& w, Random& r) {
               ShapeEdits(h.base, w.settings() + 284, 12, 17, r);
               words(w, r, {476}, 12, 0, 16);
             }),
        __imp__sub_821C1D08, hooks::OptionsButtonsPage, {{112, 24}});
  Group(h, w, "Options::AnalogPage (sub_821C21A8)", 200,
        page(7, 6,
             [&](const World& w, Random& r) {
               ShapeEdits(h.base, w.settings() + 380, 6, 11, r);
               words(w, r, {572}, 6, 0, 10);
             }),
        __imp__sub_821C21A8, hooks::OptionsAnalogPage, {{96, 16}});
  // The sound page: the volumes (edits, current, defaults) around 0 to 15,
  // the track picked and the one previewed 0 to 16 (the same half the
  // time), the menu music's cue 0 to 3. Its register saves (r28 to r31 and
  // lr, 40 bytes) left out.
  Group(h, w, "Options::SoundPage (sub_821C1498)", 250,
        page(2, 3,
             [&](const World& w, Random& r) {
               for (const uint32_t f : {60u, 64u, 252u, 256u, 444u, 448u}) {
                 guest::StoreU32(h.base, w.settings() + f, Around(r, -1, 16));
               }
               const uint32_t track = r.Below(17);
               guest::StoreU32(h.base, w.settings() + 688, track);
               guest::StoreU32(h.base, w.settings() + 692,
                               r.Below(2) == 0 ? track : Around(r, -1, 16));
               guest::StoreU32(h.base, w.settings() + 704, r.Below(4));
             }),
        __imp__sub_821C1498, hooks::OptionsSoundPage, {{160, 40}});
  // The sound page's leave: the state +700 0 to 2 or another, the menu
  // music's cue 0 to 3; the sound manager's slot 8 (IsBankSlotEmpty) the
  // getter too. Its register saves (r29 to r31 and lr) left out.
  Group(
      h, w, "Options::LeaveSoundPage (sub_821BA380)", 150,
      [&](PPCContext&, Random& from, const World& w) {
        Random r = Mixed(from);
        const uint32_t states[5] = {0, 1, 2, 2, uint32_t(r.Next())};
        guest::StoreU32(h.base, w.settings() + 700, states[r.Below(5)]);
        guest::StoreU32(h.base, w.settings() + 704, r.Below(4));
        guest::StoreU32(h.base, w.manager() + 0x400 + 32, kGet60);
        guest::StoreU32(h.base, w.page() + 0, r.Below(2) == 0 ? 13 : 0xFFFFFFFFu);
      },
      __imp__sub_821BA380, hooks::OptionsLeaveSoundPage, {{160, 32}});
  // The set-ups: the context 0 to 4, the category cursor 0 to 6, the music
  // tracks' flag 1, 0 or any; the current settings as the pages' cases
  // shape them (the buttons 0 to 16, the analog inputs 0 to 10: they land
  // in the world's child indexes). Their frames' words below +48 and from
  // +80 compared up to their register saves (the movie's text calls store
  // their varargs words at +48 to +79).
  const auto set_up = [&h, words](const World& w, Random& from) {
    Random r = Mixed(from);
    guest::StoreU32(h.base, w.settings() + 4, r.Below(5));
    guest::StoreU32(h.base, w.settings() + 664, r.Below(7));
    guest::StoreU32(h.base, w.settings() + 8, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
    words(w, r, {44}, 4, 0, 1);
    words(w, r, {68}, 3, -10, 10);
    words(w, r, {60}, 2, 0, 15);
    ShapeEdits(h.base, w.settings() + 92, 12, 17, r);
    ShapeEdits(h.base, w.settings() + 188, 6, 11, r);
  };
  const auto set_up_case = [set_up](PPCContext&, Random& r, const World& w) { set_up(w, r); };
  Group(h, w, "Options::SetUpCategoryPage (sub_821C4BE0)", 100, set_up_case, __imp__sub_821C4BE0,
        hooks::OptionsSetUpCategoryPage, {{144, 96}, {64, 48}});
  Group(h, w, "Options::SetUpHudPage (sub_821C4DB0)", 100, set_up_case, __imp__sub_821C4DB0,
        hooks::OptionsSetUpHudPage, {{144, 96}, {64, 48}});
  Group(h, w, "Options::SetUpSoundPage (sub_821C4FD0)", 100, set_up_case, __imp__sub_821C4FD0,
        hooks::OptionsSetUpSoundPage, {{128, 80}, {48, 32}});
  Group(h, w, "Options::SetUpSlidersPage (sub_821C51D0)", 100, set_up_case, __imp__sub_821C51D0,
        hooks::OptionsSetUpSlidersPage, {{128, 80}, {48, 40}});
  Group(h, w, "Options::SetUpButtonsPage (sub_821C5580)", 100, set_up_case, __imp__sub_821C5580,
        hooks::OptionsSetUpButtonsPage, {{128, 80}, {48, 32}});
  Group(h, w, "Options::SetUpAnalogPage (sub_821C57D0)", 100, set_up_case, __imp__sub_821C57D0,
        hooks::OptionsSetUpAnalogPage, {{128, 80}, {48, 32}});
  // The steps over the set-ups: the step word 0 or 1, the menu's cursor 0
  // to 5, OK/Default 0, 1 or any, the page 0 or another; the fades' counts
  // 0 or any (FadeIn's answer).
  const auto close_step = [&h, set_up](PPCContext& t, Random& from, const World& w) {
    Random r = Mixed(from);
    set_up(w, r);
    guest::StoreU32(h.base, w.page() + 24, r.Below(2));
    t.r3.u64 = w.page() + 24;
    guest::StoreU32(h.base, w.menu() + 0, r.Below(6));
    guest::StoreU32(h.base, w.settings() + 660, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
    guest::StoreU32(h.base, w.page() + 0, r.Below(2) == 0 ? 0 : r.Below(15));
    for (const uint32_t g : {0x826A50DCu, 0x826A50E4u}) {
      guest::StoreU32(h.base, g, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
    }
  };
  Group(h, w, "Options::FadeIn (sub_821C7080)", 100, close_step, __imp__sub_821C7080,
        hooks::OptionsFadeIn, {{112, 24}});
  Group(h, w, "Options::CloseHud (sub_821C75E0)", 100, close_step, __imp__sub_821C75E0,
        hooks::OptionsCloseHud, {{112, 24}});
  Group(h, w, "Options::CloseSound (sub_821C7710)", 100, close_step, __imp__sub_821C7710,
        hooks::OptionsCloseSound, {{112, 24}});
  Group(h, w, "Options::CloseSliders (sub_821C7868)", 100, close_step, __imp__sub_821C7868,
        hooks::OptionsCloseSliders, {{112, 24}});
  Group(h, w, "Options::CloseButtons (sub_821C7B08)", 100, close_step, __imp__sub_821C7B08,
        hooks::OptionsCloseButtons, {{112, 24}});
  Group(h, w, "Options::CloseAnalog (sub_821C7D18)", 100, close_step, __imp__sub_821C7D18,
        hooks::OptionsCloseAnalog, {{112, 24}});
}

// --- The type 2 and 4 modes' Inits (as the type 6 and 7 modes' harness
// checks theirs) ---

GuestFunction RecompiledInit(uint32_t address) {
  switch (address) {
    case 0x821C9E00: return __imp__sub_821C9E00;
    case 0x821CA458: return __imp__sub_821CA458;
    case 0x821C8578: return __imp__sub_821C8578;
    case 0x821C8A60: return __imp__sub_821C8A60;
    default: return nullptr;
  }
}

// Each Init on a machine block: half the cases a new machine, half one of
// random transitions whose keys are small numbers (states below 12,
// messages below 50, so some match the Init's: Add replaces those). The
// machine, the frame from its back chain up to the register saves and r3
// (64 bits) compared.
void Inits(Context& h) {
  namespace gm = game_modes;
  const Block machine(0x2600);
  if (machine.address == 0) {
    ITC_LOG_ERROR("harness: options_screen: no memory for the Inits' machine");
    return;
  }
  for (const os::InitInfo& info : os::kInits) {
    Results results(fmt::format("{} (sub_{:08X})", info.name, info.address));
    const uint32_t saves = (32 - info.first_saved) * 8 + 8;
    const uint32_t sp = CallContext(h).r1.u32;
    const uint32_t frame = sp - info.frame;
    for (uint64_t seed = 0; seed < 24; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 151 + info.address);
        r = Mixed(r);
        WriteRandom(h.base, machine.address, machine.bytes, r);
        WriteRandom(h.base, frame, info.frame, r);
        if (seed % 2 == 0) {
          gm::StateMachineConstruct(h.base, machine.address);
          guest::StoreU32(h.base, machine.address + gm::kTransitionCount, 0);
        } else {
          const uint32_t count = 8 + r.Below(160);
          guest::StoreU32(h.base, machine.address + gm::kTransitionCount, count);
          for (uint32_t i = 0; i < gm::kTransitions; i++) {
            const uint32_t at = machine.address + gm::kTransitionBytes * i;
            guest::StoreU32(h.base, at, r.Below(12));
            guest::StoreU32(h.base, at + 4, r.Below(50));
            guest::StoreU32(h.base, at + 8, r.Below(8) == 0 ? 1 : 0);
            guest::StoreU32(h.base, at + 12, 0);
          }
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = machine.address;
        if (v == 0) {
          ITC_FLAG(native_options_screen) = false;
          RecompiledInit(info.address)(t, h.base);
          ITC_FLAG(native_options_screen) = true;
        } else {
          hooks::OptionsInit(t, h.base, info.address);
        }
        out[v].Bytes(Read(h.base, machine.address, machine.bytes));
        out[v].Bytes(Read(h.base, frame, info.frame - saves));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

// --- The load and save pages (options_load_save,
// docs/functions/options-load-save.md) ---
//
// Beside the options' world: the save system's interface linked over random
// words (as the save screen's harness lays it: the users, six slots and four
// enumerated saves), the save buffers' header and its game data's buffer.
// The callees run for real on both sides (the save layer, Menu::Step and
// Pressed, MenuSound::Play, the UI movie calls, Settings::Reload). Compared
// beyond the options' world: the interface, the header, the game data's
// buffer and all of player 0's save data (their hashes for the big ones).

constexpr uint32_t kSaveObject36 = 0x40, kSaveDevices = 0x100, kSaveUsers = 0x200;
constexpr uint32_t kSaveSlots = 0x800, kSaveRecords = 0x2000, kSaveWorldBytes = 0x2800;

struct SaveSide {
  SaveSide() : save(kSaveWorldBytes), header(0x100), game_data(os::kGameDataBytes) {}
  bool ok() const { return save.address != 0 && header.address != 0 && game_data.address != 0; }
  Block save;
  Block header;
  Block game_data;
};

// A pick below `n` from the generator's high bits (its low bits follow
// one another too closely for the joint picks the pages test).
uint32_t Hi(Random& r, uint32_t n) { return uint32_t((r.Next() >> 32) % n); }

uint32_t Small(Random& r, uint32_t n) { return Hi(r, 8) == 0 ? uint32_t(r.Next()) : Hi(r, n); }
uint32_t OneOf(Random& r, std::initializer_list<uint32_t> values) {
  const uint32_t* list = values.begin();
  return Hi(r, 8) == 0 ? uint32_t(r.Next()) : list[r.Below(uint32_t(values.size()))];
}

void ShapeSaveSide(uint8_t* base, const SaveSide& s, Random& r) {
  const uint32_t block = s.save.address;
  WriteRandom(base, block, kSaveWorldBytes, r);
  guest::StoreU32(base, block, 0x82000E38);
  guest::StoreU32(base, block + 8, block + kSaveObject36);
  guest::StoreU32(base, block + kSaveObject36, 0x82000AFC);
  guest::StoreU32(base, block + kSaveObject36 + 88, block + kSaveDevices);
  guest::StoreU32(base, block + kSaveObject36 + 92, block + kSaveUsers);
  guest::StoreU32(base, block + kSaveDevices, 0x82002198);
  guest::StoreU32(base, block + kSaveDevices + 44, block + kSaveSlots);
  guest::StoreU32(base, block + kSaveUsers, 0x82000FA0);
  guest::StoreU32(base, block + kSaveUsers + 216, Hi(r, 6));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, block + kSaveUsers + 200 + 4 * i, Hi(r, 4));
    guest::StoreU32(base, block + kSaveUsers + 8 + 40 * i, Hi(r, 3) == 0 ? 0 : Hi(r, 5));
  }
  const bool idle = Hi(r, 2) == 0;
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t slot = block + kSaveSlots + 1000 * k;
    guest::StoreU32(base, slot + 988, Hi(r, 4) == 0 ? Small(r, 12) : OneOf(r, {1, 2, 3, 4, 7, 8}));
    guest::StoreU32(base, slot + 36, Hi(r, 2) == 0 ? 0 : uint32_t(r.Next()));
    guest::StoreU32(base, slot + 992, Small(r, 6));
    if (Hi(r, 2) == 0) {  // the free bytes: a little or a lot
      guest::StoreU32(base, slot + 40, 0);
      guest::StoreU32(base, slot + 44, Hi(r, 2) == 0 ? Hi(r, 0x40000) : 0x100000);
    }
    for (uint32_t word = 960; word <= 980; word += 4) {
      guest::StoreU32(base, slot + word, idle ? 0 : Hi(r, 2));
    }
    guest::StoreU32(base, slot + 80, block + kSaveRecords);
    guest::StoreU32(base, slot + 984, Hi(r, 5));
  }
  for (uint32_t e = 0; e < 4; e++) {
    const uint32_t save = block + kSaveRecords + 308 * e;
    const uint32_t pick = Hi(r, 3);
    if (pick == 2) {
      for (uint32_t i = 0; i < 20; i++) {
        guest::StoreU8(base, save + 264 + i, uint8_t('a' + Hi(r, 26)));
      }
      guest::StoreU8(base, save + 284, 0);
    } else {
      const char* name = pick == 0 ? "SystemData.bin" : "rex";
      Write(base, save + 264, name, uint32_t(std::strlen(name)) + 1);
    }
  }
  guest::StoreU32(base, 0x826BDC90, block);
  // The header: +0 the game data's buffer (its halfword +6328 0 now and
  // then: the load's check), the others 0.
  WriteRandom(base, s.header.address, s.header.bytes, r);
  for (uint32_t f = 0; f < 24; f += 4) {
    guest::StoreU32(base, s.header.address + f, 0);
  }
  guest::StoreU32(base, s.header.address, s.game_data.address);
  WriteRandom(base, s.game_data.address, 0x2000, r);
  if (Hi(r, 3) == 0) {
    guest::StoreU16(base, s.game_data.address + 6328, 0);
  }
  guest::StoreU32(base, os::kSaveBuffers, s.header.address);
}

// The dialog's menu ([0x826D8484], two items) for Menu::Step and Pressed:
// the mappers as ShapeMappers makes them, each one's moves and presses one,
// both, none or any (accept 159, back 158); the menu's player, flags and
// cursor; half the time the player's mapper made to accept (not blocked, no
// move, 159 pressed) so the answers' kinds are reached.
void ShapeDialogMenu(uint8_t* base, const World& w, Random& r) {
  ShapeMappers(base, w, r);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t mapper = w.mappers.address + 0x200 + 0x200 * k;
    const uint32_t fired[6] = {0, 1u << 2, 1u << 3, 1u << 18, 1u << 19,
                               uint32_t(r.Next() & r.Next())};
    const uint32_t pressed[5] = {0, 1u << 7, 1u << 8, (1u << 7) | (1u << 8), uint32_t(r.Next())};
    guest::StoreU32(base, mapper + 464, Hi(r, 2) == 0 ? 0 : fired[Hi(r, 6)]);
    guest::StoreU32(base, mapper + 456, pressed[Hi(r, 5)]);
  }
  const uint32_t m = w.block(os::kDialogMenu);
  const uint32_t player = Hi(r, 4);
  guest::StoreU32(base, m + menu_helpers::kPlayer, player);
  guest::StoreU32(base, m + menu_helpers::kFirst, 0);
  guest::StoreU32(base, m + menu_helpers::kLast, 1);
  guest::StoreU32(base, m + menu_helpers::kCursor, Hi(r, 2));
  for (const uint32_t o : {menu_helpers::kWraps, menu_helpers::kUpDownLocked,
                           menu_helpers::kSidesStep, menu_helpers::kShouldersStep}) {
    guest::StoreU32(base, m + o, Hi(r, 2));
  }
  if (Hi(r, 2) == 0) {
    const uint32_t entry = guest::LoadU32(base, menu_helpers::kControllerEntries + 4 * player);
    const uint32_t at = menu_helpers::kMapperEntries + 8 * entry;
    guest::StoreU32(base, at + 4, 0);
    const uint32_t mapper = guest::LoadU32(base, at);
    guest::StoreU32(base, mapper + 464, 0);
    guest::StoreU32(base, mapper + 456, 1u << 8);
  }
}

// A case's own generator for the load and save pages' shaping: the cases'
// streams are one stream shifted (their seeds follow one another), so after
// ShapeWorld's varying use two cases can stand at the same place in it;
// the first two stack words below the call's (the stream's first two
// words, one place apart for each case) mixed instead.
Random CaseRandom(uint8_t* base, const PPCContext& t) {
  const uint32_t at = t.r1.u32 - kStackBytes;
  uint64_t z = ((uint64_t(guest::LoadU32(base, at)) << 32) | guest::LoadU32(base, at + 4)) +
               0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  return Random(z ^ (z >> 31));
}

void OptionsLoadSaveCases(Context& h, const World& w, const SaveSide& side) {
  // The settings' words the pages read: the kinds (+628, +648) 0 to 13 (or
  // -1 or any), the counters (+632, +652) just below or at their 45 and 90
  // (or any), the next states; the context 0 to 4 (Settings::Reload's), the
  // save's records codes 0 to 33 (Settings::Read's).
  const auto shape = [&h, &w, &side](Random& r) {
    ShapeDialogMenu(h.base, w, r);
    ShapeSaveSide(h.base, side, r);
    const uint32_t o = w.settings();
    for (const uint32_t f : {628u, 648u}) {
      guest::StoreU32(h.base, o + f, Hi(r, 10) == 0 ? OneOf(r, {0xFFFFFFFFu, 13}) : Hi(r, 13));
    }
    const uint32_t counts[6] = {0, 43, 44, 45, 88, 89};
    for (const uint32_t f : {632u, 652u}) {
      guest::StoreU32(h.base, o + f, Hi(r, 10) == 0 ? uint32_t(r.Next()) : counts[Hi(r, 6)]);
    }
    guest::StoreU32(h.base, o + 4, r.Below(5));
    for (uint32_t f = 44; f < 284; f += 4) {
      guest::StoreU32(h.base, w.options() + f, r.Below(34));
    }
    // The page blocks' pages: the options' around 9 and 10, the sub-pages'
    // 0 to 6 or -1.
    static constexpr int32_t kPages[5] = {9, 10, -1, 0, 3};
    guest::StoreU32(h.base, w.page() + 0, uint32_t(kPages[r.Below(5)]));
    for (const uint32_t g : {os::kLoadPages, os::kSavePages}) {
      guest::StoreU32(h.base, w.block(g) + 0, uint32_t(int32_t(r.Below(8)) - 1));
    }
  };
  // After: the save side's blocks, player 0's whole save data.
  const AfterFn after = [&h, &w, &side](PPCContext&, const World&, Outputs& out) {
    out.Bytes(Read(h.base, side.save.address, side.save.bytes));
    out.Bytes(Read(h.base, side.header.address, side.header.bytes));
    out.Word(uint32_t(base::Hash64(h.base + side.game_data.address, side.game_data.bytes)));
    out.Word(uint32_t(base::Hash64(h.base + w.save.address, w.save.bytes)));
  };
  const auto plain = [&h, shape](PPCContext& t, Random&, const World&) {
    Random r = CaseRandom(h.base, t);
    shape(r);
  };
  // The leaves: r3 the kind 0 to 13 (or any), r4 the next 0 to 3 (or any).
  const auto dialog = [&h, shape](PPCContext& t, Random&, const World&) {
    Random r = CaseRandom(h.base, t);
    shape(r);
    t.r3.u64 = r.Below(8) == 0 ? r.Next() : r.Below(14);
    t.r4.u64 = r.Below(8) == 0 ? r.Next() : (uint64_t(r.Next()) << 32) * r.Below(2) + r.Below(4);
  };
  Group(h, w, "LoadPage::SetDialog (sub_821BB858)", 100, dialog, __imp__sub_821BB858,
        hooks::LoadPageSetDialog, {}, after);
  Group(h, w, "SavePage::SetDialog (sub_821BB708)", 100, dialog, __imp__sub_821BB708,
        hooks::SavePageSetDialog, {}, after);
  Group(h, w, "LoadPage::Close (sub_821B9FB0)", 40, plain, __imp__sub_821B9FB0,
        hooks::LoadPageClose, {}, after);
  Group(h, w, "SavePage::Close (sub_821BA508)", 40, plain, __imp__sub_821BA508,
        hooks::SavePageClose, {}, after);
  Group(h, w, "OptionsSavePages::Cancel (sub_821B9FF8)", 20, plain, __imp__sub_821B9FF8,
        hooks::SavePagesCancel, {}, after);
  // The sub-pages.
  Group(h, w, "LoadPage::CheckDevice (sub_821BBC10)", 150, plain, __imp__sub_821BBC10,
        hooks::LoadPageCheckDevice, {{96, 8}}, after);
  Group(h, w, "SavePage::CheckDevice (sub_821BBCC0)", 150, plain, __imp__sub_821BBCC0,
        hooks::SavePageCheckDevice, {{96, 8}}, after);
  Group(h, w, "LoadPage::DeviceAnswer (sub_821BBC68)", 150, plain, __imp__sub_821BBC68,
        hooks::LoadPageDeviceAnswer, {{96, 8}}, after);
  Group(h, w, "LoadPage::Answer (sub_821BC7B0)", 400, plain, __imp__sub_821BC7B0,
        hooks::LoadPageAnswer, {{112, 24}}, after);
  Group(h, w, "SavePage::Answer (sub_821BCA60)", 400, plain, __imp__sub_821BCA60,
        hooks::SavePageAnswer, {{96, 16}}, after);
  Group(h, w, "LoadPage::Note (sub_821BC940)", 300, plain, __imp__sub_821BC940,
        hooks::LoadPageNote, {{112, 24}}, after);
  Group(h, w, "SavePage::Note (sub_821BCC10)", 300, plain, __imp__sub_821BCC10,
        hooks::SavePageNote, {{112, 24}}, after);
  // The waits: the kind 0, 1 to 8 or the others; the counter at 89 most
  // of the time (the checks run).
  const auto wait = [&h, &w, shape](PPCContext& t, Random&, const World&) {
    Random r = CaseRandom(h.base, t);
    shape(r);
    for (const uint32_t f : {632u, 652u}) {
      if (Hi(r, 4) != 0) guest::StoreU32(h.base, w.settings() + f, 89);
    }
    for (const uint32_t f : {628u, 648u}) {
      const uint32_t kinds[5] = {0, 0, 8, Hi(r, 9), Hi(r, 14)};
      guest::StoreU32(h.base, w.settings() + f, kinds[Hi(r, 5)]);
    }
  };
  Group(h, w, "LoadPage::Wait (sub_821C2620)", 400, wait, __imp__sub_821C2620,
        hooks::LoadPageWait, {{96, 16}}, after);
  Group(h, w, "SavePage::Wait (sub_821BBD18)", 400, wait, __imp__sub_821BBD18,
        hooks::SavePageWait, {{96, 16}}, after);
  // The leaves: r3 the step word (0 or 1); the dialog menu's cursor 0 to 3;
  // the category page's set-up as the closes' cases shape it; the header the
  // layer's pages with no buffers (SaveBuffers::Destroy frees it: whether
  // it's still there compared, then freed).
  const auto leave = [&h, &w, shape](PPCContext& t, Random&, const World&) {
    Random r = CaseRandom(h.base, t);
    shape(r);
    guest::StoreU32(h.base, w.page() + 24, r.Below(2));
    t.r3.u64 = w.page() + 24;
    guest::StoreU32(h.base, w.block(os::kDialogMenu) + 0, r.Below(4));
    guest::StoreU32(h.base, w.settings() + 664, r.Below(7));
    guest::StoreU32(h.base, w.settings() + 8, r.Below(2));
    const uint32_t header = memory::AllocPages(24, 4096);
    for (uint32_t f = 0; header != 0 && f < 24; f += 4) {
      guest::StoreU32(h.base, header + f, 0);
    }
    guest::StoreU32(h.base, os::kSaveBuffers, header);
  };
  const AfterFn leave_after = [&h, after](PPCContext& t, const World& w, Outputs& out) {
    const uint32_t header = guest::LoadU32(h.base, os::kSaveBuffers);
    out.Word(header == 0 ? 0 : 1);
    if (header != 0 && memory::IsPages(header)) {
      memory::FreePages(header);
    }
    guest::StoreU32(h.base, os::kSaveBuffers, 0);
    after(t, w, out);
  };
  Group(h, w, "LoadPage::Leave (sub_821C6468)", 150, leave, __imp__sub_821C6468,
        hooks::LoadPageLeave, {{128, 32}}, leave_after);
  Group(h, w, "SavePage::Leave (sub_821C6550)", 150, leave, __imp__sub_821C6550,
        hooks::SavePageLeave, {{128, 32}}, leave_after);
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
  PageHelpers(h, w);
  Dispatch(h, w);
  CursorSteps(h, w);
  CategoryPages(h, w);
  Inits(h);
}

void OptionsLoadSave(Context& h) {
  World w;
  SaveSide side;
  if (!w.ok() || !side.ok()) {
    ITC_LOG_ERROR("harness: options_load_save: no memory for the world");
    return;
  }
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_ERROR("harness: options_load_save: the native heap isn't active");
    return;
  }
  BuildStringTable(h.base, w);
  OptionsLoadSaveCases(h, w, side);
}

}  // namespace harness
