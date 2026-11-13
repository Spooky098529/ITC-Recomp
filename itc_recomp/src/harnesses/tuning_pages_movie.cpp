// The tuning screen's input pages over movie 2 (src/tuning_pages/pages.cpp,
// docs/functions/tuning-pages.md) against the recompiled ones, part of the
// tuning_pages harness. Its world is the tuning screen's movie world
// (src/harnesses/tuning_screen_pages.cpp), general enough for the pages
// still to come: one block holding the movie (ids 2, 13 and 14 naming it;
// its +28 texture table the decal images' restores write through), a pool
// of 40 TXT nodes (each child of a node another of the pool, their
// animation entries the 8 animations, as the lists' pages need them), the
// named animations' scene root and 8 animations of no tracks, the screen
// (its command queue at [+10036], the front-end menu at [+10040], the
// decal images at +10048 left unloaded: no entry in state 2, no kept
// texture), the four players' action mappers with their vtables (slots 8,
// 10 and 12 mostly ActionMapper::Held, Pressed and Fired) and the mapper
// entries' globals pointed at them, a sound manager whose PlayCue2D is a
// recording leaf (MenuSound::Play), the screen fades' globals (the counts
// 0 most of the time); the save data (its garage list) in a block of its
// own; a string table of our own (letters, newlines and page breaks, no
// argument codes: the varargs native callers pass as 0 aren't read); the
// car scene [0x826C2E44] with pool 7's two models in use (their +32 the
// 4,244-byte records SceneModelRecord::Copy takes) and the stand holder
// [0x826D83E0] picking one of them; for the pages that load the decal
// images, Archive::Load recorded (harness::ArchiveLoads) and the pages
// VirtualAlloc gives compared as tokens and freed. Each
// case fills it with random words from its seed, lays it, points the
// globals into it (put back after), runs the original (with
// --native_tuning_pages off) or the register steps (hooks/tuning_pages.h),
// and compares the text records SetText made (freed), the globals, the
// blocks, the stack below r1 (without the original's register saves and
// the callees' saves its native version doesn't write), the caller's area,
// r1 and all of r3, exactly (nothing here is a float).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/tuning_pages.h"
#include "hooks/tuning_screen.h"
#include "tuning_pages/tuning_pages.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_tuning_pages);

namespace harness {

namespace {

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
constexpr uint32_t kVtablesAt = 0x1FC00;     // the 4 mappers' vtables (0x40 each)
constexpr uint32_t kMappersAt = 0x1FD00;     // the 4 mappers (0x200 each)
constexpr uint32_t kMenuAt = 0x20500;        // the front-end menu [+10040] (0x100)
constexpr uint32_t kTexturesAt = 0x20600;    // the movie's +28 table (200 words)
constexpr uint32_t kSlotsAt = 0x20A00;       // the textures' entries (200 of 8 bytes)
constexpr uint32_t kManagerAt = 0x21100;     // the sound manager (0x180)
constexpr uint32_t kManagerVtableAt = 0x21280;  // its vtable (0x40)
constexpr uint32_t kSceneAt = 0x21400;       // the car scene (pool 7's list at +5028)
constexpr uint32_t kModelsAt = 0x22800;      // pool 7's two models (0x1200 each)
constexpr uint32_t kModelBytes = 0x1200;
constexpr uint32_t kHolderAt = 0x24C00;      // [0x826D83E0]'s word, the holder, the stage
constexpr uint32_t kBlockBytes = 0x24C80;
constexpr uint32_t kSaveBytes = 0x1A000;

constexpr uint32_t kStackBytes = 2048;
constexpr uint32_t kCallerBytes = 256;
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kFades = 0x826A50AC;  // the screen fades' words (to 0x826A50E8)
constexpr uint32_t kFadeIn = 0x826A50DC;
constexpr uint32_t kFadeOut = 0x826A50E4;
constexpr uint32_t kSoundManager = 0x826C5C80;
constexpr uint32_t kCarScene = 0x826C2E44;
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kVinylWaits = 0x826D84A4;  // to 0x826D84C0
constexpr uint32_t kLayersWait = 0x826D84B8;  // InputVinylLayers' load wait
constexpr uint32_t kActionsWait = 0x826D84BC;  // InputLayerActions' load wait
constexpr uint32_t kOrderCount = 0x826D84C0;   // InputLayerOrder's frame count
// A leaf that stores r4's low byte, r5 and r6 at r3 + 320, 332 and 336 (the
// menu sounds' harness's recorder): the sound manager's slot 10, PlayCue2D.
constexpr uint32_t kRecordR4R6 = 0x820A8420;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"

// The mappers' read slots.
constexpr uint32_t kHeld = 0x8210BE70;
constexpr uint32_t kPressed = 0x8210BF40;
constexpr uint32_t kFired = 0x8210C010;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kMapperEntries, 32},      // the players' action mapper entries
    {kControllerEntries, 16},  // their entries' indices
    {kFades, 0x3C},
    {kSoundManager, 4},
    {kCarScene, 4},
    {kStandHolder, 4},
    {kVinylWaits, 32},  // the vinyl editor's wait words
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
  uint32_t menu() const { return at(kMenuAt); }
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  uint32_t save;
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;
};

// The movie (the block's words random already): its node pool (each node's
// animation index +116 60 to 63 and its entries 60 to 111 the 8 animations,
// for UiMovie::Play, Finish and Loop by index on the scene and its
// sub-scenes), the named animations' root and animations, its +28 texture
// table, the three ids' pairs.
void LayMovie(const World& w, Random& r) {
  uint8_t* base = w.base;
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(60));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(20));
    guest::StoreU32(base, node + 116, 60 + r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      const uint32_t value = e < 60 ? w.node(r.Below(kNodeCount))
                                    : w.at(kAnimationsAt) + 64 * r.Below(8);
      guest::StoreU32(base, node + 128 + 8 * e, value);
    }
  }
  const uint32_t root = w.at(kRootAt);
  ui_movies::InitAnimationLists(base, movie);
  guest::StoreU32(base, movie + 16, w.node(0));
  guest::StoreU32(base, movie + 20, root);
  guest::StoreU32(base, movie + 24, root);
  // The named animations (all 1,024, below the animation lists at +0x90B4)
  // over the root and one of the 8 animations.
  for (uint32_t k = 0; k < 1024; k++) {
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
  // The texture table: 200 words naming the textures' entries.
  guest::StoreU32(base, movie + 28, w.at(kTexturesAt));
  for (uint32_t k = 0; k < 200; k++) {
    guest::StoreU32(base, w.at(kTexturesAt) + 4 * k, w.at(kSlotsAt) + 8 * r.Below(200));
  }
  // The pairs: ids 2, 13 and 14, the same movie.
  for (uint32_t k = 0; k < 3; k++) {
    static constexpr uint32_t kIds[] = {2, 13, 14};
    guest::StoreU32(base, w.at(kPairsAt) + 8 * k, kIds[k]);
    guest::StoreU32(base, w.at(kPairsAt) + 8 * k + 4, movie);
  }
}

// The four players' mappers and their vtables (slot 8 mostly Held, 10
// mostly Pressed, 12 mostly Fired, else another read slot), their words
// +448 to +471 with none, few or many bits, the entries' +4 1 now and then,
// each player's entry index 0 to 3.
void LayMappers(const World& w, Random& r) {
  uint8_t* base = w.base;
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(base, kControllerEntries + 4 * c, r.Below(4));
    const uint32_t vtable = w.at(kVtablesAt) + 0x40 * c, mapper = w.at(kMappersAt) + 0x200 * c;
    guest::StoreU32(base, kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, mapper, vtable);
    static constexpr uint32_t kSlots[] = {kHeld, kPressed, kFired};
    guest::StoreU32(base, vtable + 32, r.Below(5) == 0 ? kSlots[r.Below(3)] : kHeld);
    guest::StoreU32(base, vtable + 40, r.Below(5) == 0 ? kSlots[r.Below(3)] : kPressed);
    guest::StoreU32(base, vtable + 48, r.Below(5) == 0 ? kSlots[r.Below(3)] : kFired);
    const uint32_t density = r.Below(4);
    for (uint32_t k = 0; k < 6; k++) {
      uint32_t bits = 0;
      if (density != 0) {
        bits = uint32_t(r.Next());
        for (uint32_t d = 1; d < density + 1; d++) {
          bits &= uint32_t(r.Next());
        }
      }
      guest::StoreU32(base, mapper + 448 + 4 * k, bits);
    }
  }
}

// The front-end menu: the cursor, the range and the list's rows small, the
// flags 0 or 1 (now and then any word), the player 0 to 3.
void LayMenu(const World& w, Random& r) {
  uint8_t* base = w.base;
  const uint32_t menu = w.menu();
  const uint32_t first = r.Below(3), last = first + r.Below(7);
  guest::StoreU32(base, menu + 0, r.Below(10));
  guest::StoreU32(base, menu + 4, r.Below(10));
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0, 1};
  guest::StoreU32(base, menu + 8, r.Below(6) == 0 ? uint32_t(r.Next()) : kSteps[r.Below(3)]);
  guest::StoreU32(base, menu + 12, first);
  guest::StoreU32(base, menu + 16, last);
  for (const uint32_t flag : {20u, 24u, 28u, 32u, 40u, 44u}) {
    guest::StoreU32(base, menu + flag, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2));
  }
  guest::StoreU32(base, menu + 36, kSteps[r.Below(3)]);
  guest::StoreU32(base, menu + 48, uint32_t(int32_t(r.Below(12)) - 2));
  guest::StoreU32(base, menu + 52, r.Below(10));
  guest::StoreU32(base, menu + 56, r.Below(3));
  guest::StoreU32(base, menu + 60, r.Below(10));
  guest::StoreU32(base, menu + 64, r.Below(4));
}

void MakeWorld(const World& w, Random& r) {
  uint8_t* base = w.base;
  WriteRandom(base, w.block, kBlockBytes, r);
  WriteRandom(base, w.save, kSaveBytes, r);
  LayMovie(w, r);
  LayMappers(w, r);
  LayMenu(w, r);
  // The car scene's pool 7: two models in use, the stand holder's index
  // (the stage's +8) 0 to 2 (2 past the list: its first).
  const uint32_t scene = w.at(kSceneAt);
  guest::StoreU32(base, scene + 5028, w.at(kModelsAt));
  for (uint32_t i = 0; i < 2; i++) {
    const uint32_t model = w.at(kModelsAt) + kModelBytes * i;
    guest::StoreU32(base, model + 8, i == 0 ? model + kModelBytes : 0);
    guest::StoreU32(base, model + 16, 3);
  }
  const uint32_t holder = w.at(kHolderAt);
  guest::StoreU32(base, holder + 0, holder + 0x10);
  guest::StoreU32(base, holder + 0x10, holder + 0x20);
  guest::StoreU32(base, holder + 0x28, r.Below(3));
  // The sound manager: its vtable's slot 10 the recording leaf.
  guest::StoreU32(base, w.at(kManagerAt), w.at(kManagerVtableAt));
  guest::StoreU32(base, w.at(kManagerVtableAt) + 40, kRecordR4R6);
  // The screen: its save data, queue and menu; the category -1 to 7 (2 and
  // 4 more often), the item 0 to 9, the state history's depth below 60
  // (deeper pushes write over the queue's pointer); the decal images
  // unloaded (no entry in state 2, the kept textures 0).
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 18904, w.save);
  guest::StoreU32(base, s + 10036, w.at(kQueueAt));
  guest::StoreU32(base, s + 10040, w.menu());
  guest::StoreU32(base, w.at(kQueueAt) + 16, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(65));
  static constexpr int32_t kCategories[] = {-1, 0, 1, 2, 2, 3, 4, 4, 5, 6, 7};
  guest::StoreU32(base, s + 9656, uint32_t(kCategories[r.Below(11)]));
  guest::StoreU8(base, s + 9660, uint8_t(r.Below(10)));
  guest::StoreU8(base, s + 10008, uint8_t(r.Below(60)));
  guest::StoreU32(base, s + 9652, r.Below(63));
  const uint32_t images = s + 10048;
  for (uint32_t i = 0; i < 200; i++) {
    const uint32_t state = guest::LoadU32(base, images + 36 * i + 8);
    if (state == 2) {
      guest::StoreU32(base, images + 36 * i + 8, r.Below(2));
    }
    guest::StoreU32(base, images + 7236 + 8 * i, 0);
    guest::StoreU32(base, images + 7236 + 8 * i + 4, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  }
}

void PointGlobals(const World& w, Random& r) {
  uint8_t* base = w.base;
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 16);
  // Now and then the pairs not there (UiMovies::Find answers the missing
  // movie, the same movie).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 3);
  guest::StoreU32(base, kMissingMovie, w.at(kMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
  guest::StoreU32(base, kSoundManager, w.at(kManagerAt));
  guest::StoreU32(base, kCarScene, w.at(kSceneAt));
  guest::StoreU32(base, kStandHolder, w.at(kHolderAt));
  // The fades' words random, the counts 0 most of the time.
  for (uint32_t i = 0; i < 0x3C; i += 4) {
    guest::StoreU32(base, kFades + i, r.Below(4) == 0 ? uint32_t(r.Next()) : 0);
  }
  if (r.Below(4) != 0) {
    guest::StoreU32(base, kFadeIn, 0);
    guest::StoreU32(base, kFadeOut, 0);
  }
}

// The text records SetText made: their words and buffers compared, freed,
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

using Setup = std::function<void(PPCContext&, Random&, const World&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  uint32_t saves;  // the original's register saves below r1
  // [from, to) bytes below r1 left out: callees' saves the native version
  // doesn't write.
  std::vector<std::pair<uint32_t, uint32_t>> holes = {};
  Setup setup = {};  // after the world, the globals and the stack
  // Archive::Load recorded (random serials or -1) and the decal images'
  // pages compared as tokens, then freed.
  bool loads = false;
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
      MakeWorld(w, r);
      PointGlobals(w, r);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = w.screen();
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      if (spec.setup) {
        spec.setup(t, r, w);
      }
      std::optional<ArchiveLoads> loads;
      if (spec.loads) {
        std::vector<int64_t> serials;
        for (int k = 0; k < 260; k++) {
          serials.push_back(r.Below(5) == 0 ? -1 : int64_t(r.Below(1000)));
        }
        loads.emplace(serials);
      }
      if (v == 0) {
        const bool native = ITC_FLAG(native_tuning_pages);
        ITC_FLAG(native_tuning_pages) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_tuning_pages) = native;
      } else {
        spec.native(t, h.base);
      }
      for (const auto& [from, to] : spec.holes) {
        for (uint32_t a = sp - to; a < sp - from; a++) {
          guest::StoreU8(h.base, a, 0);
        }
      }
      // The pages, in the order the loads and the entries name them.
      std::vector<uint32_t> pages;
      if (loads) {
        auto token = [&](uint32_t word) -> uint32_t {
          if (word == 0 || !memory::IsPages(word)) return word;
          for (uint32_t k = 0; k < pages.size(); k++) {
            if (pages[k] == word) return 0xA110C000u + k;
          }
          pages.push_back(word);
          return 0xA110C000u + uint32_t(pages.size() - 1);
        };
        const std::vector<uint32_t>& calls = loads->calls();
        out[v].Word(uint32_t(calls.size()));
        for (size_t k = 0; k < calls.size(); k++) {
          out[v].Word(k % 6 == 1 ? token(calls[k]) : calls[k]);
        }
        const uint32_t images = w.screen() + 10048;
        for (uint32_t i = 0; i < 200; i++) {
          guest::StoreU32(h.base, images + 36 * i + 12,
                          token(guest::LoadU32(h.base, images + 36 * i + 12)));
        }
        loads.reset();
      }
      MovieTextRecords(h.base, w.at(kMovieAt), out[v]);
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      out[v].Bytes(Read(h.base, w.save, kSaveBytes));
      out[v].Bytes(Read(h.base, sp - kStackBytes, kStackBytes - spec.saves));
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(uint32_t(t.lr));
      out[v].Word(uint32_t(pages.size()));
      for (const uint32_t a : pages) {
        out[v].Bytes(Read(h.base, a, 64));
        memory::FreePages(a);
      }
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// r3's high word now and then set.
void HighWord(PPCContext& t, Random& r) {
  if (r.Below(4) == 0) {
    t.r3.u64 |= r.Next() << 32;
  }
}

// The wheel pages' fields: the part row +25196 outside 0 to 2 (up and down
// still take every branch from -2, -1, 3 and 4; PreviewWheels then writes no
// garage car and calls no PreviewWheel), the wraps +25200 and +25236, +9708,
// the inch row +25228 and its end +25232 small, the money +25116 any word,
// the brand +25204 and model +25220 in range and the other words
// IsPickOwned and GetPrice index by small; the save data's garage list
// short (GarageCarOf reads, no writes).
void WheelFields(PPCContext&, Random& r, const World& w, bool any_row) {
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  static constexpr int32_t kRows[] = {-2, -1, 3, 4};
  uint32_t row = r.Below(5) == 0 ? (uint32_t(r.Next()) | 0x10) : uint32_t(kRows[r.Below(4)]);
  if (any_row && r.Below(2) == 0) {
    row = r.Below(3);
  }
  guest::StoreU32(base, s + 25196, row);
  auto flag = [&] { return r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2); };
  guest::StoreU32(base, s + 25200, flag());
  guest::StoreU32(base, s + 25236, flag());
  guest::StoreU32(base, s + 9708, r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  guest::StoreU32(base, s + 25228, uint32_t(int32_t(r.Below(14)) - 2));
  guest::StoreU32(base, s + 25232, uint32_t(int32_t(r.Below(13)) - 1));
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 25116, r.Below(5000));
  }
  guest::StoreU32(base, s + 25204, r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
  guest::StoreU32(base, s + 33900, r.Below(4));
  guest::StoreU32(base, s + 33540, r.Below(3));
  guest::StoreU32(base, s + 33548, r.Below(50));
  guest::StoreU32(base, s + 33564, r.Below(5));
  guest::StoreU32(base, s + 33572, r.Below(5));
  guest::StoreU32(base, w.save + 0x19920, 0);
  guest::StoreU16(base, w.save + 6120 + 204, uint16_t(r.Below(50)));
  // A few of the pages' named animations already started (now and then
  // moved to the looping list), so their Finish calls have work to do.
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t k = r.Below(12); k > 0; k--) {
    // 388 to 403 (the part rows, bars and picks), 966, 19 to 31 (the inch
    // rows and bars), 62.
    const uint32_t pick = r.Below(31);
    const uint32_t named = pick < 16 ? 388 + pick : pick < 29 ? pick + 3 : pick == 29 ? 966 : 62;
    ui_movies::PlayNamed(base, movie, named, r.Below(2), 0, 0, 0);
    if (r.Below(3) == 0) {
      ui_movies::MoveNamedAnimation(base, movie, named, true);
    }
  }
  // Most cases one action asked by the pages answered on the player's
  // mapper (its pressed +456 or fired +464 bit, by the id - 151), the
  // entry not blocked; else the mappers as laid.
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 159, 158};
  const uint32_t pick = r.Below(8);
  if (pick < 6) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    const uint32_t bit = 1u << (kActions[pick] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pick >= 4 ? bit : 0);
    guest::StoreU32(base, mapper + 464, pick < 4 ? bit : 0);
  }
}

// A word with a signaling NaN made quiet (the originals' lfs and stfs quiet
// it, the host's float copies may not; not the game's data: the kVinyls
// precedent in tuning_pages.cpp).
void QuietRange(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    uint32_t word = guest::LoadU32(base, address + i);
    if ((word & 0x7F800000u) == 0x7F800000u && (word & 0x007FFFFFu) != 0) {
      word |= 0x00400000u;
    }
    guest::StoreU32(base, address + i, word);
  }
}

// State 4's fields (InputItems and MoveItem): the category and item picked
// from the pages InputItems' A goes to three times in four (the levels,
// types, door, wheels, paint, light kits, vinyl and sticker editors,
// accessory, and items that go nowhere), else any; the aero list's rows
// +25100 and +25104 -1 to 8 (now and then the wraps' pairs 7, 0 and 0, 7),
// the aero items' availability, +9708, the item count +9676, the items'
// bits +9688 mostly all set; the editors' world: the save data's garage
// list as tuning_pages.cpp's ShapeScreen lays it (the cars' ids below 26,
// their decal ids -1 to 29), the part set, the sticker numbers, the vinyl
// layers' kinds and mirror index small (StoreToGarageCar's tables), the
// vinyl records without signaling NaNs; a few named animations playing;
// the menu's player 0 now and then; most cases one action answered on the
// player's mapper (up, down, A three times as often, B).
void ItemFields(PPCContext& t, Random& world_random, const World& w) {
  // The cases' generators are seeded with consecutive outputs of the
  // harness's xorshift, so their streams are one draw apart, and the
  // world's draws of varying counts often bring two cases' streams back in
  // step: this one is seeded from the save block's first word (drawn before
  // any of those) mixed by splitmix64's finalizer.
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save)) << 32 |
                guest::LoadU32(w.base, w.save + 4)) +
               0x9E3779B97F4A7C15ull;
  world_random.Next();
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  struct Pick {
    int32_t category;
    uint8_t item;
  };
  static constexpr Pick kPicks[] = {{0, 0}, {0, 2}, {1, 4}, {1, 1}, {3, 1}, {4, 4}, {4, 2},
                                    {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
                                    {5, 6}, {5, 7}, {5, 7}, {6, 0}, {6, 2}, {6, 3}, {6, 4},
                                    {2, 0}, {7, 1}, {5, 8}, {4, 0}, {4, 9}};
  if (r.Below(4) != 0) {
    const Pick pick = kPicks[r.Below(sizeof(kPicks) / sizeof(kPicks[0]))];
    guest::StoreU32(base, s + 9656, uint32_t(pick.category));
    guest::StoreU8(base, s + 9660, pick.item);
  }
  int32_t before = int32_t(r.Below(10)) - 1, now = int32_t(r.Below(10)) - 1;
  if (r.Below(4) == 0) {
    const bool down = r.Below(2) == 0;
    before = down ? 0 : 7;
    now = down ? 7 : 0;
  }
  guest::StoreU32(base, s + 25104, uint32_t(before));
  guest::StoreU32(base, s + 25100, uint32_t(now));
  for (uint32_t k = 0; k < 10; k++) {
    guest::StoreU32(base, s + 25128 + 4 * k,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(3) == 0 ? 0 : 1);
  }
  guest::StoreU32(base, s + 9708, r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  guest::StoreU8(base, s + 9676, uint8_t(r.Below(11)));
  guest::StoreU32(base, s + 9688, r.Below(5) == 0 ? uint32_t(r.Next()) : 0xFFFFFFFFu);
  // The garage list: 51 nodes, each link and the head a node, each slot 0
  // to 3, the walk 0 to 5 steps; the cars' ids and decal ids small.
  const uint32_t save = w.save, nodes = save + 6120;
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t c = nodes + 1968 * k + 212;
    guest::StoreU32(base, c + 12, r.Below(26));
    QuietRange(base, c + 96, 192);
    for (uint32_t n = 0; n < 6; n++) {
      guest::StoreU32(base, c + 96 + 32 * n + 24, uint32_t(int32_t(r.Below(31)) - 1));
    }
  }
  QuietRange(base, s + 30880, 31220 - 30880);
  guest::StoreU32(base, s + 33900, r.Below(4));
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, s + 33800 + 4 * i,
                    r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(int32_t(r.Below(260)) - 100));
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t layer = s + 30580 + 4 * i;
    guest::StoreU32(base, layer + 0, r.Below(3));
    guest::StoreU32(base, layer + 80, r.Below(3));
    guest::StoreU32(base, layer + 64, r.Below(8));
  }
  // A few of the items' animations playing (now and then moved to the
  // looping list).
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t k = r.Below(8); k > 0; k--) {
    const uint32_t pick = r.Below(13);
    const uint32_t named = pick < 10 ? 401 + pick : pick == 10 ? 966 : pick == 11 ? 967 : 1013;
    ui_movies::PlayNamed(base, movie, named, r.Below(2), 0, 0, 0);
    if (r.Below(3) == 0) {
      ui_movies::MoveNamedAnimation(base, movie, named, true);
    }
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, w.menu() + 64, 0);
  }
  // The movie's file scene (+24) the live one: the editors' UiMovie::ResetPose
  // (movies 13 and 14 are this movie) then copies the live nodes over
  // themselves (the root's animations would overwrite their child entries).
  guest::StoreU32(base, movie + 24, w.node(0));
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 159, 159, 159, 158};
  const uint32_t pick = r.Below(10);
  if (pick < 8) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    const uint32_t bit = 1u << (kActions[pick] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pick >= 4 ? bit : 0);
    guest::StoreU32(base, mapper + 464, pick < 4 ? bit : 0);
  }
  // With A, B or nothing asked, most of the time the menu stays (its
  // player 0, the cursor inside its range, nothing fired on the mapper), so
  // InputItems' A and B run.
  if (pick >= 4 && r.Below(5) != 0) {
    const uint32_t menu = w.menu();
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    guest::StoreU32(base, guest::LoadU32(base, kMapperEntries + 8 * c) + 464, 0);
    guest::StoreU32(base, menu + 64, 0);
    const uint32_t first = guest::LoadU32(base, menu + 12), last = guest::LoadU32(base, menu + 16);
    guest::StoreU32(base, menu + 0, first + r.Below(last - first + 1));
  }
  // The garage car's sticker records (LoadStickers' GarageDecal reads)
  // without signaling NaNs (the originals' lfs and stfs quiet them, the
  // host's float copies may not: the kVinyls precedent in tuning_pages.cpp).
  for (uint32_t a = 0; a < 8; a++) {
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t g = tuning_pages::GarageDecal(base, w.screen(), a, k);
      if (g != 0) {
        QuietRange(base, g, 32);
      }
    }
  }
  HighWord(t, r);
}

// State 8's fields (InputOptions and MoveOption): the category and item
// one of the option lists most of the time (the tire, category 3 with its
// items 0 to 2; the bonnet, category 4 item 4; the door, category 5 item
// 1), else as the world laid them; the option +9664 0 to 9, the list's
// count +9680 1 to 10, the tire's level +9661 0 to 9 (its current choice,
// the item's word GetCurrentChoice reads, the level half the time); the
// menu's step (+8) -1, 0 or 1 (now and then any word) and wrap (+40); +9708;
// the words IsPickOwned and GetPrice index by small, the money any word
// half the time; the state history's word below its depth one of the pages'
// states most of the time; a few of the lists' named animations playing;
// most cases one action answered on the player's mapper (up, down, A three
// times as often, B), with A, B or nothing asked the menu staying most of
// the time (its player 0, the cursor in range, nothing fired) so A and B run.
void OptionFields(PPCContext& t, Random& world_random, const World& w) {
  // Seeded as ItemFields' (its comment): the save block's first words mixed
  // by splitmix64's finalizer.
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save)) << 32 |
                guest::LoadU32(w.base, w.save + 4)) +
               0x9E3779B97F4A7C15ull;
  world_random.Next();
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  const uint32_t list = r.Below(5);
  if (list < 2) {
    guest::StoreU32(base, s + 9656, 3);
    guest::StoreU8(base, s + 9660, uint8_t(r.Below(3)));
  } else if (list == 2) {
    guest::StoreU32(base, s + 9656, 4);
    guest::StoreU8(base, s + 9660, 4);
  } else if (list == 3) {
    guest::StoreU32(base, s + 9656, 5);
    guest::StoreU8(base, s + 9660, 1);
  }
  guest::StoreU8(base, s + 9664, uint8_t(r.Below(10)));
  guest::StoreU8(base, s + 9680, uint8_t(1 + r.Below(10)));
  const uint32_t level = r.Below(10);
  guest::StoreU8(base, s + 9661, uint8_t(level));
  static constexpr uint32_t kTireChoices[] = {35820, 35948, 36076};
  const uint32_t item = guest::LoadU8(base, s + 9660);
  if (item < 3 && r.Below(2) == 0) {
    guest::StoreU32(base, s + kTireChoices[item], level);
  }
  // The pick not owned half the time (IsPickOwned's word for the lists 0),
  // so A goes to the price check (the money short or the buy).
  if (r.Below(2) == 0) {
    const int32_t category = int32_t(guest::LoadU32(base, s + 9656));
    const uint32_t option = guest::LoadU8(base, s + 9664);
    uint32_t owned = 0;
    if (category == 3 && item < 3) {
      owned = kTireChoices[item] + 8 + 12 * level + 4 * option;
    } else if (category == 4 && item == 4) {
      owned = 36380 + 8 * guest::LoadU8(base, s + 9662) + 4 * guest::LoadU8(base, s + 9663);
    } else if (category == 5 && item == 1) {
      owned = 36616 + 4 * option;
    }
    if (owned != 0) {
      guest::StoreU32(base, s + owned, 0);
    }
  }
  const uint32_t menu = w.menu();
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0, 1};
  guest::StoreU32(base, menu + 8, r.Below(8) == 0 ? uint32_t(r.Next()) : kSteps[r.Below(3)]);
  guest::StoreU32(base, menu + 40, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, s + 9708, r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 25116, r.Below(5000));
  }
  guest::StoreU32(base, s + 25196, r.Below(3));
  guest::StoreU32(base, s + 25204, r.Below(17));
  guest::StoreU32(base, s + 25220, r.Below(38));
  guest::StoreU32(base, s + 33900, r.Below(4));
  guest::StoreU32(base, s + 33540, r.Below(3));
  guest::StoreU32(base, s + 33548, r.Below(50));
  guest::StoreU32(base, s + 33564, r.Below(5));
  guest::StoreU32(base, s + 33572, r.Below(5));
  // The state B pops back to.
  const uint32_t depth = guest::LoadU8(base, s + 10008);
  if (depth != 0 && r.Below(5) != 0) {
    static constexpr uint32_t kStates[] = {3, 4, 5, 6, 7, 8};
    guest::StoreU32(base, s + (depth - 1 + 2439) * 4, kStates[r.Below(6)]);
  }
  // A few of the lists' named animations playing (now and then moved to
  // the looping list).
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t k = r.Below(10); k > 0; k--) {
    static constexpr uint32_t kFirst[] = {844, 783, 675, 489, 428, 658, 609, 619};
    const uint32_t pick = r.Below(11);
    const uint32_t named = pick < 8      ? kFirst[pick] + r.Below(10)
                           : pick == 8  ? 854 + r.Below(2)
                           : pick == 9  ? (r.Below(2) == 0 ? 685 + r.Below(2) : 499 + r.Below(2))
                                        : (r.Below(2) == 0 ? 629 : 973);
    ui_movies::PlayNamed(base, movie, named, r.Below(2), 0, 0, 0);
    if (r.Below(3) == 0) {
      ui_movies::MoveNamedAnimation(base, movie, named, true);
    }
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, menu + 64, 0);
  }
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 159, 159, 159, 158};
  const uint32_t pick = r.Below(10);
  if (pick < 8) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    const uint32_t bit = 1u << (kActions[pick] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pick >= 4 ? bit : 0);
    guest::StoreU32(base, mapper + 464, pick < 4 ? bit : 0);
  }
  if (pick >= 4 && r.Below(5) != 0) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    guest::StoreU32(base, guest::LoadU32(base, kMapperEntries + 8 * c) + 464, 0);
    guest::StoreU32(base, menu + 64, 0);
    const uint32_t first = guest::LoadU32(base, menu + 12), last = guest::LoadU32(base, menu + 16);
    guest::StoreU32(base, menu + 0, first + r.Below(last - first + 1));
  }
  HighWord(t, r);
}

// State 5's fields (MoveLevel), seeded as ItemFields (its comment): the
// category and item one of the level lists most of the time (the short
// ones, the long ones, the tire's), else one with no list or as the world
// laid them; the level +9661 0 to 9 (0 and 1 more often: the A path's
// level checks), the level count +9677 1 to 10 (above and below 8; 9 or
// 10 most of the time for the long lists); the
// long list's rows +25100 and +25104 -1 to 8 (the wraps' pairs 7, 0 and 0,
// 7 often); the menu's step (+8) -1, 0 or 1 (now and then any word) and
// wrap (+40); +9708; the level's IsItemAvailable word 0 a third of the
// time, and its GetCurrentChoice word the level half the time (three
// times in four for the tire's); a few of the
// lists' named animations playing; most cases A or B answered on the
// player's mapper (the entry blocked now and then).
void LevelFields(PPCContext& t, Random& world_random, const World& w) {
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save)) << 32 |
                guest::LoadU32(w.base, w.save + 4)) +
               0x9E3779B97F4A7C15ull;
  world_random.Next();
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  struct Pick {
    int32_t category;
    uint8_t item;
  };
  static constexpr Pick kShort[] = {{0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 1},
                                    {1, 2}, {1, 3}, {1, 3}, {2, 0}, {2, 5}};
  static constexpr Pick kLong[] = {{0, 0}, {1, 4}};
  static constexpr Pick kNone[] = {{0, 4}, {1, 5}, {4, 4}, {5, 1}, {-1, 0}, {7, 2}};
  const uint32_t kind = r.Below(10);
  Pick pick{-2, 0};
  if (kind < 4) {
    pick = kShort[r.Below(10)];
  } else if (kind < 6) {
    pick = kLong[r.Below(2)];
  } else if (kind < 8) {
    pick = {3, uint8_t(r.Below(4))};
  } else if (kind < 9) {
    pick = kNone[r.Below(6)];
  }
  if (pick.category != -2) {
    guest::StoreU32(base, s + 9656, uint32_t(pick.category));
    guest::StoreU8(base, s + 9660, pick.item);
  }
  const uint32_t level = r.Below(4) == 0 ? r.Below(2) : r.Below(10);
  guest::StoreU8(base, s + 9661, uint8_t(level));
  // The long lists' rows move only with more than 8 levels: 9 or 10 most of
  // the time there.
  const bool long_list = (kind == 4 || kind == 5) && r.Below(4) != 0;
  guest::StoreU8(base, s + 9677, uint8_t(long_list ? 9 + r.Below(2) : 1 + r.Below(10)));
  int32_t before = int32_t(r.Below(10)) - 1, now = int32_t(r.Below(10)) - 1;
  if (r.Below(3) == 0) {
    const bool down = r.Below(2) == 0;
    before = down ? 0 : 7;
    now = down ? 7 : 0;
  }
  guest::StoreU32(base, s + 25104, uint32_t(before));
  guest::StoreU32(base, s + 25100, uint32_t(now));
  const uint32_t menu = w.menu();
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0, 1};
  guest::StoreU32(base, menu + 8, r.Below(10) == 0 ? uint32_t(r.Next()) : kSteps[r.Below(3)]);
  guest::StoreU32(base, menu + 40, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2));
  guest::StoreU32(base, s + 9708, r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) : 1);
  const uint32_t parts = r.Below(4);
  guest::StoreU32(base, s + 33900, parts);
  // The level's words: IsItemAvailable's (by category and item, + 4 x the
  // level) and GetCurrentChoice's (the item's word).
  const int32_t category = int32_t(guest::LoadU32(base, s + 9656));
  const uint32_t item = guest::LoadU8(base, s + 9660);
  uint32_t available = 0, choice = 0;
  if (category == 0) {
    static constexpr uint32_t kAvailable[] = {34104, 34664, 34992, 35084};
    static constexpr uint32_t kChoice[] = {34304, 34804, 35020, 35140};
    if (item < 4) {
      available = kAvailable[item] + (item == 0 ? 40 : item == 1 ? 28 : 0) * parts;
      choice = kChoice[item] + (item < 2 ? 4 * parts : 0);
    }
  } else if (category == 1) {
    static constexpr uint32_t kAvailable[] = {35200, 35288, 35376, 35464, 35568};
    static constexpr uint32_t kChoice[] = {35228, 35316, 35404, 35492, 35608};
    if (item < 5) {
      available = kAvailable[item];
      choice = kChoice[item];
    }
  } else if (category == 2) {
    available = 35680;
    choice = 35708;
  } else if (category == 3) {
    static constexpr uint32_t kChoice[] = {35820, 35948, 36076};
    available = 35800;
    choice = item < 3 ? kChoice[item] : 0;
  }
  if (available != 0) {
    guest::StoreU32(base, s + available + 4 * level,
                    r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) | 1 : 1);
  }
  if (choice != 0 && r.Below(category == 3 ? 4 : 2) != 0) {
    guest::StoreU32(base, s + choice, level);
  }
  // A few of the lists' named animations playing (now and then moved to
  // the looping list).
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t k = r.Below(12); k > 0; k--) {
    static constexpr uint32_t kFirst[] = {489, 428, 632, 571, 501, 444, 455, 644, 587, 598, 415};
    static constexpr uint32_t kSingle[] = {499, 500, 642, 643, 1013, 465, 608, 972, 973, 974};
    const uint32_t pick_named = r.Below(14);
    const uint32_t named = pick_named < 11 ? kFirst[pick_named] + r.Below(10) : kSingle[r.Below(10)];
    ui_movies::PlayNamed(base, movie, named, r.Below(2), 0, 0, 0);
    if (r.Below(3) == 0) {
      ui_movies::MoveNamedAnimation(base, movie, named, true);
    }
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, menu + 64, 0);
  }
  static constexpr uint32_t kActions[] = {159, 159, 159, 158, 158, 153};
  const uint32_t action = r.Below(8);
  if (action < 6) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    const uint32_t bit = 1u << (kActions[action] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, bit);
    guest::StoreU32(base, mapper + 464, 0);
  }
  HighWord(t, r);
}

// State 5's page's fields (InputLevels): LevelFields' (above), then, from a
// generator of its own (the save block's next two words mixed as ItemFields
// mixes its first two), the pick's owned word (IsPickOwned's, by category,
// item, part set, level and option +9664 0 to 2) 0 half the time, so A goes
// to the price check; the money small half the time (the money short or the
// buy); the state history's word below its depth one of the pages' states
// most of the time (B's Pop, PageItemCount and PageCursor); with A or B
// answered (the entry not blocked), the menu staying most of the time (its
// player 0, its range 0 to 0..9 with the cursor in it, nothing fired) so A
// and B run.
void LevelInputFields(PPCContext& t, Random& world_random, const World& w) {
  LevelFields(t, world_random, w);
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save + 8)) << 32 |
                guest::LoadU32(w.base, w.save + 12)) +
               0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU8(base, s + 9664, uint8_t(r.Below(3)));
  if (r.Below(2) == 0) {
    const int32_t category = int32_t(guest::LoadU32(base, s + 9656));
    const uint32_t item = guest::LoadU8(base, s + 9660);
    const uint32_t level = guest::LoadU8(base, s + 9661);
    const uint32_t parts = guest::LoadU32(base, s + 33900);
    uint32_t owned = 0;
    if (category == 0 && item < 4) {
      static constexpr uint32_t kOwned[] = {34324, 34824, 35028, 35144};
      owned = kOwned[item] + (item == 0 ? 40 : item == 1 ? 28 : 0) * parts + 4 * level;
    } else if (category == 1 && item < 5) {
      static constexpr uint32_t kOwned[] = {35232, 35320, 35408, 35500, 35612};
      owned = kOwned[item] + 4 * level;
    } else if (category == 2) {
      owned = 35712 + 4 * level;
    } else if (category == 3 && item < 3) {
      static constexpr uint32_t kOwned[] = {35828, 35956, 36084};
      owned = kOwned[item] + 12 * level + 4 * guest::LoadU8(base, s + 9664);
    }
    if (owned != 0) {
      guest::StoreU32(base, s + owned, 0);
    }
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 25116, r.Below(5000));
  }
  const uint32_t depth = guest::LoadU8(base, s + 10008);
  if (depth != 0 && r.Below(5) != 0) {
    static constexpr uint32_t kStates[] = {3, 4, 5, 6, 7, 8};
    guest::StoreU32(base, s + (depth - 1 + 2439) * 4, kStates[r.Below(6)]);
  }
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  const uint32_t pressed = guest::LoadU32(base, mapper + 456);
  const bool a_or_b = (pressed & ((1u << (159 - 151)) | (1u << (158 - 151)))) != 0;
  if (a_or_b && r.Below(5) != 0) {
    const uint32_t menu = w.menu();
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, 0);
    guest::StoreU32(base, mapper + 464, 0);
    guest::StoreU32(base, menu + 64, 0);
    const uint32_t last = r.Below(10);
    guest::StoreU32(base, menu + 12, 0);
    guest::StoreU32(base, menu + 16, last);
    guest::StoreU32(base, menu + 0, r.Below(last + 1));
  }
}

// State 6's fields (MoveType), seeded as ItemFields (its comment): the
// category and item one of the type lists most of the time (G1, G2, G3),
// else one with no list (category 1 item 3 for the A path's G1 rows), or as
// the world laid them; the state +9652 6, or 5 now and then (category 0's
// IsItemAvailable answers only there); the type +9662 and the level +9661 0
// to 9 (0 and 1 more often), the type count +9678 1 to 10 (above and below
// 8); the menu's step (+8) -1, 0 or 1 (now and then any word) and wrap
// (+40); the availability words IsItemAvailable reads for the type and the
// level (category 4 item 9, category 0 item 2, category 1 item 3) 0 a third
// of the time; the word +36368 and GetCurrentChoice's words (category 6 item
// 0, category 0 item 2) the type or the level half the time; a few of the
// lists' named animations playing; most cases A or B answered on the
// player's mapper (the entry blocked now and then).
void TypeFields(PPCContext& t, Random& world_random, const World& w) {
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save)) << 32 |
                guest::LoadU32(w.base, w.save + 4)) +
               0x9E3779B97F4A7C15ull;
  world_random.Next();
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  struct Pick {
    int32_t category;
    uint8_t item;
  };
  static constexpr Pick kG2[] = {{4, 0}, {4, 3}, {4, 4}, {4, 4}, {4, 9}, {4, 9},
                                 {4, 7}, {5, 3}, {6, 0}, {6, 0}, {6, 1}};
  static constexpr Pick kG3[] = {{5, 2}, {6, 2}};
  static constexpr Pick kNone[] = {{1, 3}, {1, 3}, {0, 0}, {1, 1}, {5, 1},
                                   {6, 3}, {2, 0}, {-1, 0}, {7, 2}};
  const uint32_t kind = r.Below(10);
  Pick pick{-2, 0};
  if (kind < 2) {
    pick = {0, 2};
  } else if (kind < 5) {
    pick = kG2[r.Below(11)];
  } else if (kind < 7) {
    pick = kG3[r.Below(2)];
  } else if (kind < 9) {
    pick = kNone[r.Below(9)];
  }
  if (pick.category != -2) {
    guest::StoreU32(base, s + 9656, uint32_t(pick.category));
    guest::StoreU8(base, s + 9660, pick.item);
  }
  guest::StoreU32(base, s + 9652, r.Below(4) == 0 ? 5 : 6);
  const uint32_t type = r.Below(4) == 0 ? r.Below(2) : r.Below(10);
  const uint32_t level = r.Below(4) == 0 ? r.Below(2) : r.Below(10);
  guest::StoreU8(base, s + 9662, uint8_t(type));
  guest::StoreU8(base, s + 9661, uint8_t(level));
  guest::StoreU8(base, s + 9678, uint8_t(1 + r.Below(10)));
  const uint32_t menu = w.menu();
  static constexpr uint32_t kSteps[] = {0xFFFFFFFFu, 0, 1};
  guest::StoreU32(base, menu + 8, r.Below(10) == 0 ? uint32_t(r.Next()) : kSteps[r.Below(3)]);
  guest::StoreU32(base, menu + 40, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(2));
  const uint32_t parts = r.Below(4);
  guest::StoreU32(base, s + 33900, parts);
  // IsItemAvailable's words: category 4 item 9 (+36576), category 0 item 2
  // (+34992, in state 5), category 1 item 3 (+35464); + 4 x the type or the
  // level.
  auto availability = [&] {
    return r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? uint32_t(r.Next()) | 1 : 1;
  };
  for (const uint32_t at : {36576u, 34992u, 35464u}) {
    guest::StoreU32(base, s + at + 4 * type, availability());
    guest::StoreU32(base, s + at + 4 * level, availability());
  }
  // The word +36368 and GetCurrentChoice's (+36708 category 6 item 0, +35020
  // category 0 item 2).
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 36368, type);
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 36708, type);
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 35020, level);
  }
  // A few of the lists' named animations playing (now and then moved to
  // the looping list).
  const uint32_t movie = w.at(kMovieAt);
  for (uint32_t k = r.Below(12); k > 0; k--) {
    static constexpr uint32_t kFirst[] = {532, 489, 428, 501, 444, 455, 515, 466, 476};
    static constexpr uint32_t kSingle[] = {542, 543, 499, 500, 1013, 465, 486,
                                           990, 973, 975, 1012, 1};
    const uint32_t pick_named = r.Below(12);
    const uint32_t named = pick_named < 9 ? kFirst[pick_named] + r.Below(10) : kSingle[r.Below(12)];
    ui_movies::PlayNamed(base, movie, named, r.Below(2), 0, 0, 0);
    if (r.Below(3) == 0) {
      ui_movies::MoveNamedAnimation(base, movie, named, true);
    }
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, menu + 64, 0);
  }
  static constexpr uint32_t kActions[] = {159, 159, 159, 158, 158, 153};
  const uint32_t action = r.Below(8);
  if (action < 6) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    const uint32_t bit = 1u << (kActions[action] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, bit);
    guest::StoreU32(base, mapper + 464, 0);
  }
  HighWord(t, r);
}

// State 6's page's fields (InputTypes): TypeFields' (above), then, from a
// generator of its own (the save block's words +16 and +20 mixed as
// ItemFields mixes its first two), the list half the time one of the
// A path's (the bonnet's types, category 4 item 4; the interior's, category
// 6 item 0; state 14's, category 4 items 3 and 9; the horn, category 5
// item 3; their types half the time) or B's; the bonnet's list byte +9663 0 to 2; +25172 1 half the
// time; the menu's cursor 0 to 5 most of the time (the interior's new type
// 1 to 4 or not); the pick's owned word (IsPickOwned's, by category, item
// and type) 0 half the time, so A goes to the price check; the money small
// half the time; the state history's word below its depth one of the
// pages' states most of the time (B's Pop, PageItemCount and PageCursor);
// action 157 pressed now and then (with A or B or alone; half of those on
// the horn's list, types 0 to 5); with A, B or 157
// answered (the entry not blocked), the menu staying most of the time.
void TypeInputFields(PPCContext& t, Random& world_random, const World& w) {
  TypeFields(t, world_random, w);
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save + 16)) << 32 |
                guest::LoadU32(w.base, w.save + 20)) +
               0x9E3779B97F4A7C15ull;
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  struct Pick {
    int32_t category;
    uint8_t item;
  };
  static constexpr Pick kPicks[] = {{4, 4}, {4, 4}, {6, 0}, {6, 0}, {4, 3}, {4, 3}, {4, 3},
                                    {4, 3}, {4, 9}, {4, 9}, {5, 3}, {5, 3}, {5, 2}, {6, 1},
                                    {6, 2}, {0, 2}, {4, 0}, {4, 7}, {0, 1}, {1, 3}, {6, 3},
                                    {5, 4}};
  if (r.Below(2) == 0) {
    const Pick pick = kPicks[r.Below(22)];
    guest::StoreU32(base, s + 9656, uint32_t(pick.category));
    guest::StoreU8(base, s + 9660, pick.item);
    // The types the A paths branch on half the time: state 14's (category
    // 4 item 3 type 1, item 9 type 0), the interior's 1 to 4.
    if (r.Below(2) == 0) {
      if (pick.category == 4 && pick.item == 3) {
        guest::StoreU8(base, s + 9662, 1);
      } else if (pick.category == 4 && pick.item == 9) {
        guest::StoreU8(base, s + 9662, 0);
      } else if (pick.category == 6 && pick.item == 0) {
        guest::StoreU8(base, s + 9662, uint8_t(1 + r.Below(4)));
      }
    }
  }
  guest::StoreU8(base, s + 9663, uint8_t(r.Below(3)));
  guest::StoreU32(base, s + 25172, r.Below(2) == 0 ? 1 : r.Below(3));
  const uint32_t menu = w.menu();
  if (r.Below(5) != 0) {
    guest::StoreU32(base, menu + 0, r.Below(6));
  }
  if (r.Below(2) == 0) {
    const int32_t category = int32_t(guest::LoadU32(base, s + 9656));
    const uint32_t item = guest::LoadU8(base, s + 9660);
    const uint32_t type = guest::LoadU8(base, s + 9662);
    const uint32_t level = guest::LoadU8(base, s + 9661);
    uint32_t owned = 0;
    if (category == 0 && item == 2) {
      owned = 35028 + 4 * level;
    } else if (category == 4 && item == 4) {
      owned = 36380 + 8 * type + 4 * guest::LoadU8(base, s + 9663);
    } else if (category == 4 && item <= 9) {
      static constexpr uint32_t kOwned[] = {36168, 36212, 36256, 36304, 0,
                                            36440, 36476, 36512, 36548, 36592};
      owned = kOwned[item] + 4 * type;
    } else if (category == 5 && (item == 2 || item == 3)) {
      owned = (item == 2 ? 36640 : 36672) + 4 * type;
    } else if (category == 6 && item <= 2) {
      static constexpr uint32_t kOwned[] = {36716, 36756, 36800};
      owned = kOwned[item] + 4 * type;
    }
    if (owned != 0) {
      guest::StoreU32(base, s + owned, 0);
    }
  }
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 25116, r.Below(5000));
  }
  const uint32_t depth = guest::LoadU8(base, s + 10008);
  if (depth != 0 && r.Below(5) != 0) {
    static constexpr uint32_t kStates[] = {3, 4, 5, 6, 7, 8};
    guest::StoreU32(base, s + (depth - 1 + 2439) * 4, kStates[r.Below(6)]);
  }
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  uint32_t pressed = guest::LoadU32(base, mapper + 456);
  if (r.Below(4) == 0) {
    pressed = (r.Below(2) == 0 ? pressed : 0) | (1u << (157 - 151));
    guest::StoreU32(base, mapper + 456, pressed);
    if (r.Below(2) == 0) {
      // The horn's list, its types 0 to 5.
      guest::StoreU32(base, s + 9656, 5);
      guest::StoreU8(base, s + 9660, 3);
      guest::StoreU8(base, s + 9662, uint8_t(r.Below(6)));
    }
  }
  const uint32_t asked = (1u << (159 - 151)) | (1u << (158 - 151)) | (1u << (157 - 151));
  if ((pressed & asked) != 0 && r.Below(5) != 0) {
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, 0);
    guest::StoreU32(base, mapper + 464, 0);
    guest::StoreU32(base, menu + 64, 0);
    guest::StoreU32(base, menu + 8, 0);
    const uint32_t last = r.Below(10);
    guest::StoreU32(base, menu + 12, 0);
    guest::StoreU32(base, menu + 16, last);
    if (r.Below(3) == 0) {
      guest::StoreU32(base, menu + 0, r.Below(last + 1));
    }
  }
}

// State 59's fields (Leave and StoreOwnedParts): the owned words each 0
// half the time (else as laid: any word), the tires' words 0, 1 or any, the
// part set [+33900] 0 to 7 (5 to 7 now and then: none of the power items'
// bits), +25172 0 or any, the wheels' flags 0, 1 or any; StoreToGarageCar's
// world as ItemFields lays it (the save data's garage list, the sticker
// numbers, the vinyl layers' kinds and mirror index small); r4 the object
// with the message ring, in the save block (its count and start any word,
// small now and then). Seeded as ItemFields (its comment).
void OwnedFields(PPCContext& t, Random& world_random, const World& w) {
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save)) << 32 |
                guest::LoadU32(w.base, w.save + 4)) +
               0x9E3779B97F4A7C15ull;
  world_random.Next();
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  auto mix = [&](uint32_t from, uint32_t count) {
    for (uint32_t k = 0; k < count; k++) {
      if (r.Below(2) == 0) {
        guest::StoreU32(base, s + from + 4 * k, 0);
      }
    }
  };
  // The wheels' flags (TestAndSetWheelFlag's words, by brand).
  static constexpr uint32_t kBases[18] = {0,   1,   39,  47,  72,  99,  116, 123, 124,
                                          126, 135, 144, 147, 162, 163, 165, 170, 179};
  for (uint32_t b = 0; b < 17; b++) {
    for (uint32_t k = 0; k < kBases[b + 1] - kBases[b]; k++) {
      const uint32_t pick = r.Below(4);
      guest::StoreU32(base, s + 27856 + 152 * b + 4 * k,
                      pick < 2 ? 0 : pick == 2 ? 1 : uint32_t(r.Next()));
    }
  }
  // The power items 0 and 1 of every part set 0 to 4 (and past them).
  mix(34324, 10 * 8);
  mix(34824, 7 * 8);
  static constexpr std::pair<uint32_t, uint32_t> kRanges[] = {
      {35028, 7}, {35144, 7}, {35232, 7}, {35320, 7}, {35408, 7}, {35500, 7}, {35612, 10},
      {35712, 7}, {36168, 5}, {36212, 5}, {36256, 5}, {36304, 6}, {36380, 10}, {36440, 4},
      {36476, 4}, {36512, 4}, {36548, 4}, {36592, 3}, {36616, 2}, {36672, 4}, {36716, 5},
      {36756, 4}, {36800, 6}, {33624, 4}, {33820, 20}};
  for (const auto& [from, count] : kRanges) {
    mix(from, count);
  }
  for (uint32_t i = 0; i < 15; i++) {
    const uint32_t pick = r.Below(4);
    guest::StoreU32(base, s + 35828 + 4 * i, pick < 2 ? 0 : pick == 2 ? 1 : uint32_t(r.Next()));
  }
  guest::StoreU32(base, s + 33900, r.Below(4) == 0 ? 5 + r.Below(3) : r.Below(5));
  guest::StoreU32(base, s + 25172, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  // StoreToGarageCar's world (ItemFields').
  const uint32_t save = w.save, nodes = save + 6120;
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, nodes + 1968 * k + 212 + 12, r.Below(26));
  }
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, s + 33800 + 4 * i,
                    r.Below(4) == 0 ? 0xFFFFFFFFu : uint32_t(int32_t(r.Below(260)) - 100));
  }
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t layer = s + 30580 + 4 * i;
    guest::StoreU32(base, layer + 0, r.Below(3));
    guest::StoreU32(base, layer + 80, r.Below(3));
    guest::StoreU32(base, layer + 64, r.Below(8));
  }
  // The message ring's object.
  const uint32_t object = save + 0x400;
  if (r.Below(2) == 0) {
    guest::StoreU32(base, object + 804, r.Below(60));
    guest::StoreU32(base, object + 808, r.Below(60));
  }
  t.r4.u64 = object | (r.Below(4) == 0 ? r.Next() << 32 : 0);
  HighWord(t, r);
}

// State 16's fields (InputWheels, AnimateWheels and the list's rows), seeded
// as ItemFields (its comment): WheelFields' (the part row +25196 any for the
// movie side and the rows; outside 0 to 2 for the page, so PreviewWheels
// writes no garage car and calls no PreviewWheel), then the list's last
// index +25224 -1 to 8 (1, 2 and 4 and above most), the cursor +25220 -1 to
// n + 1, the brand +25204 0 to 16 (0 and 16 more often, now and then -1 or
// 17), the tabs' row +25212 -1 to 4 and the old row +25216 0 to 3 (the
// wraps' pairs 3, 0 and 0, 3 often), +9716 any, the front and rear wheels
// +27836 and +27840 now and then a wheel the rows show; the state 16 most
// of the time; the state history's word below its depth 15 most of the
// time; the garage list as ItemFields lays it (InchListEnd, PreviewWheels'
// reads); a few of the page's named animations playing; most cases one
// action answered on the player's mapper (left, right, up, down, A, B), the
// entry not blocked mostly.
void WheelsFields(PPCContext& t, Random& world_random, const World& w, bool page) {
  uint64_t z = (uint64_t(guest::LoadU32(w.base, w.save)) << 32 |
                guest::LoadU32(w.base, w.save + 4)) +
               0x9E3779B97F4A7C15ull;
  world_random.Next();
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
  Random r(z ^ (z >> 31));
  WheelFields(t, r, w, !page);
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  static constexpr int32_t kLasts[] = {-1, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8};
  const int32_t last = kLasts[r.Below(12)];
  guest::StoreU32(base, s + 25224, uint32_t(last));
  const int32_t top = last < 0 ? 1 : last + 2;
  guest::StoreU32(base, s + 25220, uint32_t(int32_t(r.Below(uint32_t(top) + 1)) - 1));
  static constexpr int32_t kBrands[] = {-1, 0, 0, 16, 16, 17};
  const uint32_t brand_pick = r.Below(12);
  guest::StoreU32(base, s + 25204,
                  brand_pick < 6 ? uint32_t(kBrands[brand_pick]) : 1 + r.Below(15));
  uint32_t row = uint32_t(int32_t(r.Below(6)) - 1), old = r.Below(4);
  if (r.Below(3) == 0) {
    const bool down = r.Below(2) == 0;
    row = down ? 0 : 3;
    old = down ? 3 : 0;
  }
  guest::StoreU32(base, s + 25212, row);
  guest::StoreU32(base, s + 25216, old);
  guest::StoreU32(base, s + 9716, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
  for (const uint32_t wheel : {27836u, 27840u}) {
    if (r.Below(2) == 0) {
      guest::StoreU32(base, s + wheel, r.Below(60));
    }
  }
  if (r.Below(4) != 0) {
    guest::StoreU32(base, s + 9652, 16);
  }
  const uint32_t depth = guest::LoadU8(base, s + 10008);
  if (depth != 0 && r.Below(5) != 0) {
    guest::StoreU32(base, s + (depth - 1 + 2439) * 4, 15);
  }
  // The scene's lookups pinned: the rows write a wheel's number into each
  // picture's +108 and SetText the text's id into its node's +108 (both a
  // node's child index), so the scene (node 0), sub-scene 1 (node 1), the
  // rows (1, 14 to 19: nodes 2 to 7), the brand logo and the tabs' bar (-1,
  // 17 and 1, 9: node 38) and the price panel (-1, 19: node 39) keep their
  // children's indices at 0, and the nodes those two write into are leaves,
  // looked up from no node: the rows' children 1 to 5 (nodes 8 to 37), the
  // logo's and bar's child 0 (node 12), the price texts (19, 2 to 4: nodes
  // 13 to 15) and the part name (1, 20: node 16).
  auto child = [&](uint32_t node, uint32_t i, uint32_t to) {
    guest::StoreU32(base, w.node(node) + 108, 0);
    guest::StoreU32(base, w.node(node) + 128 + 8 * i, w.node(to));
  };
  child(0, 1, 1);
  child(0, 17, 38);
  child(0, 19, 39);
  child(1, 9, 38);
  child(1, 20, 16);
  child(38, 0, 12);
  for (uint32_t i = 2; i <= 4; i++) {
    child(39, i, 11 + i);
  }
  for (uint32_t k = 0; k < 6; k++) {
    child(1, 14 + k, 2 + k);
    for (uint32_t i = 1; i <= 5; i++) {
      child(2 + k, i, 8 + 5 * k + (i - 1));
    }
  }
  // The garage list (ItemFields').
  const uint32_t save = w.save, nodes = save + 6120;
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, nodes + 1968 * k + 212 + 12, r.Below(26));
  }
  // A few of the page's named animations playing (now and then moved to
  // the looping list): the tabs' highlights 20 to 23, the bars 30 and 31,
  // the slides 58, 59, 63 and 64, 69, 70, the part picks 415 to 417, 1008,
  // 1012.
  const uint32_t movie = w.at(kMovieAt);
  static constexpr uint32_t kNamed[] = {20, 21, 22, 23, 30, 31, 58,  59,  63,   64,
                                        69, 70, 415, 416, 417, 1008, 1012};
  for (uint32_t k = r.Below(10); k > 0; k--) {
    const uint32_t named = kNamed[r.Below(sizeof(kNamed) / sizeof(kNamed[0]))];
    ui_movies::PlayNamed(base, movie, named, r.Below(2), 0, 0, 0);
    if (r.Below(3) == 0) {
      ui_movies::MoveNamedAnimation(base, movie, named, true);
    }
  }
  static constexpr uint32_t kActions[] = {155, 169, 156, 170, 153, 167, 154, 168, 159, 158};
  const uint32_t pick = r.Below(12);
  if (pick < 10) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    const uint32_t bit = 1u << (kActions[pick] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pick >= 8 ? bit : 0);
    guest::StoreU32(base, mapper + 464, pick < 8 ? bit : 0);
  }
  HighWord(t, r);
}

// The vinyl editor's decal edits (states 29 to 31, InputDecalPosition,
// Angle, Scale and their movie sides): the mappers' slot 11 (+44) the read
// slot sub_8210BFA8 (TuningScreen::HighlightSelected's release check; the
// world's random word otherwise); the page +30724 0 or 1 mostly (now and then
// 2 or -1: nothing placed), the layer +30732 0 to 2; x and y (+30832,
// +30836) at, beside or inside the page's limits; the angle +30840 and the
// scale +30844 at, beside or between their ends, now and then any word; the
// pages' layouts (+30800 to +30828: int origins, float scales) small; the
// history at least 1 deep; one action most of the time (a move fired, A or
// B pressed), with 164 held now and then.
void DecalEditFields(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(base, w.at(kVtablesAt) + 0x40 * c + 44, 0x8210BFA8);
  }
  const int32_t pages[] = {0, 0, 1, 1, 2, -1};
  guest::StoreU32(base, s + 30724, uint32_t(pages[r.Below(6)]));
  guest::StoreU32(base, s + 30732, r.Below(3));
  static constexpr int32_t kEnds[] = {-512, -256, -128, 128, 256, 512};
  auto coordinate = [&] {
    switch (r.Below(4)) {
      case 0: return kEnds[r.Below(6)];
      case 1: return kEnds[r.Below(6)] + int32_t(r.Below(3)) - 1;
      case 2: return int32_t(r.Below(1025)) - 512;
      default: return int32_t(r.Next());
    }
  };
  guest::StoreU32(base, s + 30832, uint32_t(coordinate()));
  guest::StoreU32(base, s + 30836, uint32_t(coordinate()));
  static constexpr float kAngles[] = {0.0f, 0.5f, 1.0f, 358.0f, 358.5f, 359.0f, 180.0f, -1.0f};
  static constexpr float kScales[] = {0.1f, 0.105f, 0.11f, 1.0f, 1.995f, 2.0f, 1.99f, 0.05f};
  guest::StoreFloat(base, s + 30840, r.Below(8) == 0 ? float(r.Below(36000)) / 100.0f
                                                     : kAngles[r.Below(8)]);
  guest::StoreFloat(base, s + 30844, r.Below(8) == 0 ? float(r.Below(250)) / 100.0f
                                                     : kScales[r.Below(8)]);
  if (r.Below(10) == 0) {
    guest::StoreU32(base, s + (r.Below(2) == 0 ? 30840 : 30844), uint32_t(r.Next()));
  }
  for (const uint32_t at : {30800u, 30804u, 30816u, 30820u}) {
    guest::StoreU32(base, s + at, uint32_t(int32_t(r.Below(1000)) - 500));
  }
  for (const uint32_t at : {30808u, 30812u, 30824u, 30828u}) {
    guest::StoreFloat(base, s + at, float(r.Below(400)) / 100.0f);
  }
  guest::StoreU8(base, s + 10008, uint8_t(1 + r.Below(58)));
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 155, 169, 156, 170, 159, 159, 158, 158};
  const uint32_t pick = r.Below(14);
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  if (pick < 12) {
    const uint32_t bit = 1u << (kActions[pick] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pick >= 8 ? bit : 0);
    guest::StoreU32(base, mapper + 464, pick < 8 ? bit : 0);
  }
  guest::StoreU32(base, mapper + 448, r.Below(4) == 0 ? 1u << (164 - 151) : 0);
  HighWord(t, r);
}

// The layer's edit menu (InputLayerEdit, MoveLayerEdit): the decal edits'
// fields (above), the cursor +30748 -1 to 7 (now and then any word), the
// last row +30752 0 to 6, the flip +30848 0, 1 or any, the category and
// item the vinyls' mostly; the history with 24 below the top and now and
// then 28 at the top (cancel's pops find them; the walks don't go below the
// history's start), +9708 0 or 1; the action now and then B fired (the
// movie side's slot 12 on 158).
void LayerEditFields(PPCContext& t, Random& r, const World& w) {
  DecalEditFields(t, r, w);
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 30748,
                  r.Below(10) == 0 ? uint32_t(r.Next()) : uint32_t(int32_t(r.Below(9)) - 1));
  guest::StoreU32(base, s + 30752, r.Below(7));
  guest::StoreU32(base, s + 30848, r.Below(6) == 0 ? uint32_t(r.Next()) : r.Below(2));
  if (r.Below(5) != 0) {
    guest::StoreU32(base, s + 9656, 5);
    guest::StoreU8(base, s + 9660, 6);
  }
  const uint32_t depth = 3 + r.Below(40);
  guest::StoreU8(base, s + 10008, uint8_t(depth));
  guest::StoreU32(base, s + 9756 + 4 * r.Below(depth - 2), 24);
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 9756 + 4 * (depth - 1), 28);
  }
  guest::StoreU32(base, s + 9708, r.Below(3) == 0 ? 0 : 1);
  if (r.Below(8) == 0) {
    const uint32_t c = guest::LoadU32(base, kControllerEntries);
    const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
    guest::StoreU32(base, mapper + 464, 1u << (158 - 151));
  }
}

// The vinyl page's layers (InputVinylLayers, MoveVinylLayers): the page
// +30724 -1 to 2 (the layer ids index it unchecked), the cursor +30732 -1 to
// 5 (now and then any word without the wait), the button left +30736 3 or 4 (now and then
// 0 to 2), the layers' saved ids +31192 + 12 x page + 4c -1 half the time,
// the picked image +30764 around both lists, +9708 0 or 1; the load wait 0
// half the time, else 1 to 5 (now and then -1); the picker windows'
// entries (12 to 20, 33 to 41) with no texture (0 header and data:
// Texture::Free frees nothing) or held by the kept slot of their index; now and then no
// entry loading (the wait's end); the action now and then one move, A or
// B.
void VinylLayerFields(PPCContext& t, Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 30724, uint32_t(int32_t(r.Below(4)) - 1));
  const uint32_t cursor = uint32_t(int32_t(r.Below(7)) - 1);
  const uint32_t any = uint32_t(r.Next());
  guest::StoreU32(base, s + 30736, r.Below(5) == 0 ? r.Below(3) : 3 + r.Below(2));
  for (uint32_t k = 0; k < 12; k++) {
    guest::StoreU32(base, s + 31180 + 4 * k, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(200));
  }
  guest::StoreU32(base, s + 30764, uint32_t(int32_t(r.Below(210)) - 8));
  guest::StoreU32(base, s + 9708, r.Below(3) == 0 ? 0 : 1);
  const uint32_t wait = r.Below(2) == 0 ? 0 : r.Below(10) == 0 ? 0xFFFFFFFFu : 1 + r.Below(5);
  guest::StoreU32(base, kLayersWait, wait);
  // (Past the wait the cursor indexes the movie's textures: any word only
  // without it.)
  guest::StoreU32(base, s + 30732, wait == 0 && r.Below(10) == 0 ? any : cursor);
  const uint32_t images = s + 10048;
  for (const uint32_t first : {12u, 33u}) {
    for (uint32_t i = first; i < first + 9; i++) {
      const uint32_t e = images + 36 * i;
      if (r.Below(2) == 0) {
        guest::StoreU32(base, e + 28, 0);
        guest::StoreU32(base, e + 32, 0);
      } else {
        // Kept slot i (distinct: a slot taken twice would leave an entry
        // unheld, its random texture freed).
        guest::StoreU32(base, images + 7236 + 8 * i, guest::LoadU32(base, e + 28));
      }
    }
  }
  if (r.Below(2) == 0) {
    for (uint32_t i = 0; i < 200; i++) {
      if (guest::LoadU32(base, images + 36 * i + 8) == 1) {
        guest::StoreU32(base, images + 36 * i + 8, 0);
      }
    }
  }
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 155, 169, 156, 170, 159, 159, 158, 158};
  const uint32_t pick = r.Below(15);
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  if (pick < 12) {
    const uint32_t bit = 1u << (kActions[pick] - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pick >= 8 ? bit : 0);
    guest::StoreU32(base, mapper + 464, pick < 8 ? bit : 0);
  }
  HighWord(t, r);
}

// The image picker (MoveVinylImages; InputVinylImages): the layers' fields
// (above), with 164 now and then held and the mappers' slot 11 (+44) the
// read slot sub_8210BFA8 (as the decal edits').
void VinylImageFields(PPCContext& t, Random& r, const World& w) {
  VinylLayerFields(t, r, w);
  for (uint32_t c = 0; c < 4; c++) {
    guest::StoreU32(w.base, w.at(kVtablesAt) + 0x40 * c + 44, 0x8210BFA8);
  }
  const uint32_t c = guest::LoadU32(w.base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(w.base, kMapperEntries + 8 * c);
  guest::StoreU32(w.base, mapper + 448, r.Below(4) == 0 ? 1u << (164 - 151) : 0);
}

// The image picker's page (InputVinylImages): the image picker's fields
// (above) with the layer +30732 -1 to 5 (it indexes the entries freed and
// the movie's texture table), entries 0 to 50 with no texture (0 header and
// data) or held by the kept slot of their index (each distinct: two
// entries sharing a kept slot would leave one unheld and Texture::Free run
// on random words), and the kept slots 6 to 14 FreeKept frees held or with
// no texture; the four wait words [0x826D84B4], [0x826D84B0], [0x826D84A8]
// and [0x826D84AC] 0 most of the time, else small; the picked image +30764
// around 0, the list's end +30796 (4, 193 or 198) and between, the group
// +30768 -1 to 12, the state +9652 24 to 26 mostly; the action one of the
// moves, 164 held, the group's 161 and 162, A (159 pressed, now and then
// with 163 fired) or B, now and then none.
void VinylPickFields(PPCContext& t, Random& r, const World& w) {
  VinylImageFields(t, r, w);
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 30732, uint32_t(int32_t(r.Below(7)) - 1));
  const uint32_t images = s + 10048;
  for (uint32_t i = 0; i < 51; i++) {
    const uint32_t e = images + 36 * i;
    if (r.Below(2) == 0) {
      guest::StoreU32(base, e + 28, 0);
      guest::StoreU32(base, e + 32, 0);
      guest::StoreU32(base, images + 7236 + 8 * i, 0);
      guest::StoreU32(base, images + 7240 + 8 * i, 0);
    } else {
      guest::StoreU32(base, images + 7236 + 8 * i, guest::LoadU32(base, e + 28));
    }
  }
  static constexpr uint32_t kWaits[] = {0x826D84B4, 0x826D84B0, 0x826D84A8, 0x826D84AC};
  for (const uint32_t at : kWaits) {
    guest::StoreU32(base, at, r.Below(3) != 0 ? 0 : r.Below(8) == 0 ? 0xFFFFFFFFu : 1 + r.Below(3));
  }
  static constexpr uint32_t kLasts[] = {4, 193, 198};
  const uint32_t last = kLasts[r.Below(3)];
  guest::StoreU32(base, s + 30796, last);
  int32_t pick;
  switch (r.Below(5)) {
    case 0: pick = int32_t(r.Below(3)) - 1; break;
    case 1: pick = int32_t(last) + int32_t(r.Below(3)) - 1; break;
    case 2: pick = int32_t(r.Below(last + 1)); break;
    case 3: pick = int32_t(r.Below(210)) - 8; break;
    default: pick = int32_t(r.Below(5)); break;
  }
  guest::StoreU32(base, s + 30764, uint32_t(pick));
  guest::StoreU32(base, s + 30768, uint32_t(int32_t(r.Below(14)) - 1));
  guest::StoreU32(base, s + 9652, r.Below(5) == 0 ? r.Below(63) : 24 + r.Below(3));
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  static constexpr uint32_t kFired[] = {155, 169, 156, 170, 161, 162};
  const uint32_t action = r.Below(12);
  uint32_t pressed = 0, fired = 0;
  if (action < 6) {
    fired = 1u << (kFired[action] - 151);
  } else if (action < 9) {
    pressed = 1u << (159 - 151);
    if (r.Below(2) == 0) {
      fired = 1u << (163 - 151);
    }
  } else if (action < 11) {
    pressed = 1u << (158 - 151);
  }
  if (action < 11) {
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 456, pressed);
    guest::StoreU32(base, mapper + 464, fired);
  }
}

// The operate select (InputLayerActions, MoveLayerActions,
// BeginLayerOrder): the image picker's fields (above: the page -1 to 2, the
// layers' saved ids, the picked image, the picker windows' entries with no
// texture or held by a distinct kept slot, now and then none loading, the
// mappers' slot 11 laid) with the layer +30732 0 to 2 (it indexes the
// movie's texture table), the cursor +30740 -1 to 4 (now and then any word;
// 2 half the time with the wait set), the wrap +30744 0 or 1, the vinyls
// (+9656 5, byte +9660 6) most of the time; the load wait [0x826D84BC] 0
// mostly, else 2 (its loads) or 1 to 5 or -1, now and then an entry
// loading; the action now and then one of up, down, A, B
// or 164 held (held now and then with the others too).
void LayerActionFields(PPCContext& t, Random& r, const World& w) {
  VinylImageFields(t, r, w);
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 30732, r.Below(3));
  const uint32_t wait = r.Below(3) != 0   ? 0
                        : r.Below(8) == 0 ? 0xFFFFFFFFu
                        : r.Below(2) == 0 ? 2
                                          : 1 + r.Below(5);
  guest::StoreU32(base, kActionsWait, wait);
  if (r.Below(3) == 0) {
    // An entry loading (the wait's end waits for it).
    guest::StoreU32(base, s + 10048 + 36 * r.Below(200) + 8, 1);
  }
  uint32_t cursor = uint32_t(int32_t(r.Below(6)) - 1);
  if (r.Below(10) == 0) {
    cursor = uint32_t(r.Next());
  } else if (wait != 0 && r.Below(2) == 0) {
    cursor = 2;
  }
  guest::StoreU32(base, s + 30740, cursor);
  guest::StoreU32(base, s + 30744, r.Below(2));
  if (r.Below(5) != 0) {
    guest::StoreU32(base, s + 9656, 5);
    guest::StoreU8(base, s + 9660, 6);
  }
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 159, 159, 159, 158, 158, 164};
  const uint32_t pick = r.Below(12);
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  if (pick < 10) {
    const uint32_t action = kActions[pick];
    const uint32_t bit = 1u << (action - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 448,
                    action == 164 || r.Below(8) == 0 ? 1u << (164 - 151) : 0);
    guest::StoreU32(base, mapper + 456, action == 159 || action == 158 ? bit : 0);
    guest::StoreU32(base, mapper + 464, action < 158 || action > 159 ? bit : 0);
  }
}

// The layer order (InputLayerOrder, MoveLayerOrder, ShowOrderNodes,
// SwapOrderTextures, SwapLayerRecords, EndLayerOrder): the image picker's
// fields (above: the page -1 to 2, the vinyls most of the time, 164 now and
// then held) with the history's depth 3 to 42 and a 24 below its top, the
// layer +30732 0 to 2 (now and then 3 or 4: the named animations' index), the position
// +30756 -1 to 3 and the last +30792 0 to 3 (the work list's words they
// index stay inside the screen), the move's way +30760 0 to 3, the work
// list's ids +31216 + 4k -1 or an image and indices +31228 + 4k 0 to 2
// (they index the image entries), the frame count [0x826D84C0] 0 half the
// time, else 9, 12 (their next frames the work), 1 to 14 or -1; the action
// now and then one of up, down, A, B or 164 held (held now and then with
// the others too); SwapLayerRecords' r4 and r5 -1 to 3 (now and then with
// a high word).
void LayerOrderFields(PPCContext& t, Random& r, const World& w) {
  VinylImageFields(t, r, w);
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 30732, r.Below(6) == 0 ? 3 + r.Below(2) : r.Below(3));
  // The history: a 24 below its top (the count's end pops down to it; the
  // original loops until it finds one), now and then 28 on top.
  const uint32_t depth = 3 + r.Below(40);
  guest::StoreU8(base, s + 10008, uint8_t(depth));
  guest::StoreU32(base, s + 9756 + 4 * r.Below(depth - 2), 24);
  if (r.Below(2) == 0) {
    guest::StoreU32(base, s + 9756 + 4 * (depth - 1), 28);
  }
  guest::StoreU32(base, s + 30756, uint32_t(int32_t(r.Below(5)) - 1));
  guest::StoreU32(base, s + 30792, r.Below(4));
  guest::StoreU32(base, s + 30760, r.Below(4));
  for (uint32_t k = 0; k < 3; k++) {
    guest::StoreU32(base, s + 31216 + 4 * k, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(200));
    guest::StoreU32(base, s + 31228 + 4 * k, r.Below(3));
  }
  static constexpr uint32_t kCounts[] = {9, 12, 9, 12, 0xFFFFFFFFu};
  const uint32_t count = r.Below(2) == 0 ? 0
                         : r.Below(3) == 0 ? 1 + r.Below(14)
                                           : kCounts[r.Below(5)];
  guest::StoreU32(base, kOrderCount, count);
  static constexpr uint32_t kActions[] = {153, 167, 154, 168, 159, 159, 158, 158, 164};
  const uint32_t pick = r.Below(11);
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  if (pick < 9) {
    const uint32_t action = kActions[pick];
    const uint32_t bit = 1u << (action - 151);
    guest::StoreU32(base, kMapperEntries + 8 * c + 4, r.Below(8) == 0 ? 1 : 0);
    guest::StoreU32(base, mapper + 448,
                    action == 164 || r.Below(8) == 0 ? 1u << (164 - 151) : 0);
    guest::StoreU32(base, mapper + 456, action == 159 || action == 158 ? bit : 0);
    guest::StoreU32(base, mapper + 464, action < 158 || action > 159 ? bit : 0);
  }
  auto around = [&r] {
    uint64_t v = uint64_t(int64_t(int32_t(r.Below(5)) - 1));
    if (r.Below(2) == 0) {
      v &= 0xFFFFFFFFu;
    }
    if (r.Below(6) == 0) {
      v += r.Next() << 32;
    }
    return v;
  };
  t.r4.u64 = around();
  t.r5.u64 = around();
}

// The sticker editor's group helpers (SetGroupImage, SetImageGroup): r4 the
// group -2 to 13 or the image -5 to 200 around the bounds, now and then any
// doubleword.
void GroupFields(PPCContext& t, Random& r, const World& w) {
  (void)w;
  HighWord(t, r);
  switch (r.Below(4)) {
    case 0: t.r4.u64 = uint64_t(int64_t(int32_t(r.Below(16)) - 2)); break;
    case 1: t.r4.u64 = uint64_t(int64_t(int32_t(r.Below(206)) - 5)); break;
    case 2: {
      static constexpr int32_t kBounds[] = {43, 78, 106, 114, 142, 156, 165, 175, 180, 187, 192};
      t.r4.u64 = uint64_t(int64_t(kBounds[r.Below(11)] + int32_t(r.Below(3)) - 1));
      break;
    }
    default: t.r4.u64 = r.Next(); break;
  }
}

}  // namespace

// TuningScreen::HandleInput's cases for one of `states`' page: the page's
// own fields (`fields`, as its group lays them), then +9728 set (0 one case
// in eight: DecalImages::Update only) and the queue's +0 the state. Its r30,
// r31 and lr (24 bytes); DecalImages::Update's saves (112 to 136 below, the
// decal images unloaded: nothing made) and the page's own saves and holes
// (`page_saves`, `page_holes`) 112 deeper, which the native versions don't
// write.
Spec HandleInputSpec(const char* name, int cases, std::vector<uint32_t> states,
                     uint32_t page_saves,
                     const std::vector<std::pair<uint32_t, uint32_t>>& page_holes,
                     const Setup& fields, bool loads = false) {
  std::vector<std::pair<uint32_t, uint32_t>> holes = {{112, 112 + std::max(24u, page_saves)}};
  for (const auto& [from, to] : page_holes) {
    holes.push_back({from + 112, to + 112});
  }
  return {name,
          cases,
          __imp__sub_821E6950,
          hooks::TuningPagesHandleInput,
          24,
          holes,
          [fields, states](PPCContext& t, Random& r, const World& w) {
            if (fields) {
              fields(t, r, w);
            }
            const uint32_t state = states[r.Below(uint32_t(states.size()))];
            const uint32_t s = w.screen();
            guest::StoreU32(w.base, s + 9728,
                            r.Below(8) == 0 ? 0 : (r.Below(2) == 0 ? 1 : uint32_t(r.Next()) | 1));
            guest::StoreU32(w.base, guest::LoadU32(w.base, s + 10036), state);
          },
          loads};
}

// The sticker editor's world (docs/functions/sticker-editor.md), over the
// pages' world: the sticker menu [+10044] the front-end menu; the area
// +30724 0 to 7 and the slot +30732 0 to 6; the placed stickers' ids (+33320,
// 40 words) -1 to 198 (-1 a fifth of the time) and records (+32360, 40 of 24
// bytes), the edited decal D (+30832) and its backup (+33512): x and y small
// ints, the angle and the scale floats (now and then any word), the flip 0
// or 1, a word; the solo and preview holds (+33500, +33504) and the garage
// car's flag +33508 0 or 1; the save data's owned bits (+0x199A8, 7 words)
// with at least one of the 199 stickers owned (the steps over them loop
// forever otherwise, which play never gives), few or many; the garage list
// as ItemFields lays it (the cars' ids below 26: the layouts' table); the
// editor's other words (+30740, +30748, +30756, +30764, +30768, the order
// list +33480, the picker's wait +33536) small; the holds 163 and 164 now
// and then held on the player's mapper.
void StickerFields(PPCContext&, Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t s = w.screen();
  guest::StoreU32(base, s + 10044, w.menu());
  guest::StoreU32(base, s + 30724, r.Below(8));
  guest::StoreU32(base, s + 30732, r.Below(7));
  auto id = [&] { return r.Below(5) == 0 ? 0xFFFFFFFFu : r.Below(199); };
  for (uint32_t k = 0; k < 40; k++) {
    guest::StoreU32(base, s + 33320 + 4 * k, id());
  }
  auto record = [&](uint32_t at) {
    guest::StoreU32(base, at + 0, uint32_t(int32_t(r.Below(601)) - 300));
    guest::StoreU32(base, at + 4, uint32_t(int32_t(r.Below(601)) - 300));
    guest::StoreFloat(base, at + 8, float(r.Below(36000)) / 100.0f);
    guest::StoreFloat(base, at + 12, float(r.Below(250)) / 100.0f);
    if (r.Below(10) == 0) {
      guest::StoreU32(base, at + 8 + 4 * r.Below(2), uint32_t(r.Next()));
    }
    guest::StoreU32(base, at + 16, r.Below(2));
    guest::StoreU32(base, at + 20, uint32_t(r.Next()));
  };
  for (uint32_t k = 0; k < 40; k++) {
    record(s + 32360 + 24 * k);
  }
  record(s + 30832);
  record(s + 33512);
  QuietRange(base, s + 32360, 40 * 24);
  QuietRange(base, s + 30832, 24);
  QuietRange(base, s + 33512, 24);
  for (const uint32_t at : {33500u, 33504u, 33508u}) {
    guest::StoreU32(base, s + at, r.Below(2));
  }
  const uint32_t density = r.Below(3);
  for (uint32_t k = 0; k < 7; k++) {
    uint32_t bits = uint32_t(r.Next());
    for (uint32_t d = 0; d < density; d++) {
      bits &= uint32_t(r.Next());
    }
    guest::StoreU32(base, w.save + 0x199A8 + 4 * k, bits);
  }
  const uint32_t owned = r.Below(199);
  const uint32_t word = w.save + 0x199A8 + 4 * (owned >> 5);
  guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (owned & 31)));
  const uint32_t nodes = w.save + 6120;
  for (uint32_t k = 0; k < 51; k++) {
    guest::StoreU16(base, nodes + 4 * k, uint16_t(r.Below(51)));
    guest::StoreU16(base, nodes + 4 * k + 2, uint16_t(r.Below(4)));
  }
  guest::StoreU16(base, nodes + 204, uint16_t(r.Below(51)));
  guest::StoreU32(base, w.save + 0x19920, r.Below(6));
  for (uint32_t k = 0; k < 4; k++) {
    guest::StoreU32(base, nodes + 1968 * k + 224, r.Below(26));
  }
  for (const uint32_t at : {30740u, 30748u, 30756u, 30764u, 30768u}) {
    guest::StoreU32(base, s + at, r.Below(8));
  }
  for (uint32_t k = 0; k < 5; k++) {
    guest::StoreU32(base, s + 33480 + 4 * k, r.Below(4) == 0 ? r.Below(5) : k);
  }
  guest::StoreU32(base, s + 33536, r.Below(3) == 0 ? r.Below(4) : 0);
  const uint32_t c = guest::LoadU32(base, kControllerEntries);
  const uint32_t mapper = guest::LoadU32(base, kMapperEntries + 8 * c);
  if (r.Below(2) == 0) {
    const uint32_t bit = 1u << ((r.Below(2) == 0 ? 163 : 164) - 151);
    guest::StoreU32(base, mapper + 448, guest::LoadU32(base, mapper + 448) | bit);
  }
}

// The sticker editor's cases (sticker_pages.cpp): its helpers over
// StickerFields' world.
void StickerCases(Context& h, const World& w) {
  // UpdateSoloHold and UpdatePreviewHold: r31 and lr saved (16 bytes).
  Run(h, w,
      {"TuningScreen::UpdateSoloHold", 60, __imp__sub_821F7298, hooks::TuningPagesUpdateSoloHold,
       16, {}, StickerFields});
  Run(h, w,
      {"TuningScreen::UpdatePreviewHold", 60, __imp__sub_821F7348,
       hooks::TuningPagesUpdatePreviewHold, 16, {}, StickerFields});
  // StepUnlockedSticker: r4 an id (-1 to 198, now and then any word), r5 a
  // step count -8 to 8 (now and then 0); leaf.
  Run(h, w,
      {"TuningScreen::StepUnlockedSticker", 150, __imp__sub_821F73F8,
       hooks::TuningPagesStepUnlockedSticker, 0, {},
       [](PPCContext& t, Random& r, const World& w) {
         StickerFields(t, r, w);
         t.r4.u64 = r.Below(8) == 0 ? r.Next() : uint64_t(int64_t(int32_t(r.Below(200)) - 1));
         t.r5.u64 = uint64_t(int64_t(int32_t(r.Below(17)) - 8));
         HighWord(t, r);
       }});
  // ShowSlotNode: r4 an id (-1 to 198), r5 a slot 0 to 4, r6 the current 0
  // to 6; __savegprlr_28: 40 bytes.
  Run(h, w,
      {"TuningScreen::ShowSlotNode", 100, __imp__sub_821F8070, hooks::TuningPagesShowSlotNode, 40,
       {}, [](PPCContext& t, Random& r, const World& w) {
         StickerFields(t, r, w);
         t.r4.u64 = uint64_t(int64_t(int32_t(r.Below(200)) - 1));
         t.r5.u64 = r.Below(5);
         t.r6.u64 = r.Below(7);
       }});
  // PlaceStickerNode: r4 an id (-1 to 198), r5 a slot 0 to 4, r6 a placed
  // record or D; __savegprlr_27: 48 bytes.
  Run(h, w,
      {"TuningScreen::PlaceStickerNode", 150, __imp__sub_821F8160,
       hooks::TuningPagesPlaceStickerNode, 48, {}, [](PPCContext& t, Random& r, const World& w) {
         StickerFields(t, r, w);
         t.r4.u64 = uint64_t(int64_t(int32_t(r.Below(200)) - 1));
         t.r5.u64 = r.Below(5);
         t.r6.u64 = r.Below(4) == 0 ? w.screen() + 30832 : w.screen() + 32360 + 24 * r.Below(40);
       }});
  // DecalImages::SetSlot: r3 the images, r4 the sub-scene (-1, 29 or 30), r5
  // and r6 nodes, r7 and r8 texture slots below 200, r9 an entry -1 to 198,
  // r10 a flag (0, 1, now and then any word); __savegprlr_23: 80 bytes.
  Run(h, w,
      {"DecalImages::SetSlot", 100, __imp__sub_821F8868, hooks::TuningPagesSetSlot, 80, {},
       [](PPCContext& t, Random& r, const World& w) {
         StickerFields(t, r, w);
         static constexpr uint32_t kSubs[] = {0xFFFFFFFFu, 29, 30};
         t.r3.u64 = w.screen() + 10048;
         t.r4.u64 = int64_t(int32_t(kSubs[r.Below(3)]));
         t.r5.u64 = r.Below(40);
         t.r6.u64 = r.Below(40);
         t.r7.u64 = r.Below(200);
         t.r8.u64 = r.Below(200);
         t.r9.u64 = uint64_t(int64_t(int32_t(r.Below(200)) - 1));
         t.r10.u64 = r.Below(6) == 0 ? r.Next() : r.Below(2);
       }});
  // SetStickerEditTexts: r30, r31 and lr saved (24 bytes); the frame's
  // parameter area (+48 to +79, the varargs SetText stores) left out: the
  // original leaves r7 to r10 as the calls before left them, the native
  // version passes 0 (the precedent: only dead frame words differ); and
  // SetText's saves of r28 to r31 below the frame (where it runs the
  // original), the native version's registers not the original's.
  Run(h, w,
      {"TuningScreen::SetStickerEditTexts", 60, __imp__sub_821F7F20,
       hooks::TuningPagesSetStickerEditTexts, 24, {{32, 64}, {112, 152}}, StickerFields});
  // ShowSlotImage and ShowStickerListImage: tail calls into SetSlot, whose
  // saves (80 bytes) are the top of the stack; r4 an id -1 to 198, r5 a slot
  // or an index 0 to 4.
  for (const auto& [name, recompiled, native] :
       {std::tuple{"TuningScreen::ShowSlotImage", __imp__sub_821FA408,
                   hooks::TuningPagesShowSlotImage},
        std::tuple{"TuningScreen::ShowStickerListImage", __imp__sub_821FA460,
                   hooks::TuningPagesShowStickerListImage}}) {
    Run(h, w,
        {name, 80, recompiled, native, 80, {}, [](PPCContext& t, Random& r, const World& w) {
           StickerFields(t, r, w);
           t.r4.u64 = uint64_t(int64_t(int32_t(r.Below(200)) - 1));
           t.r5.u64 = r.Below(5);
         }});
  }
  // The movie sides: r4 show (0, 1, now and then any word); their saves
  // and, below their frames, the callees' saves (the native callees don't
  // write them) and the varargs SetText leaves in their parameter areas.
  auto show = [](PPCContext& t, Random& r, const World& w) {
    StickerFields(t, r, w);
    t.r4.u64 = r.Below(3) == 0 ? 0 : r.Below(4) == 0 ? r.Next() : 1;
  };
  // AnimateStickerAreas: below its frame, GarageStage::Command's callees
  // (ApplyPreset's) save the caller's registers deep down (seen at 408 and
  // 764 below r1: the original's k in r29), dead words the native version's
  // registers fill differently; left out with SetSlot's saves.
  Run(h, w,
      {"TuningScreen::AnimateStickerAreas", 100, __imp__sub_821FA508,
       hooks::TuningPagesAnimateStickerAreas, 56, {{144, 800}}, show});
  Run(h, w,
      {"TuningScreen::AnimateStickerOperate", 100, __imp__sub_821F7518,
       hooks::TuningPagesAnimateStickerOperate, 56, {{64, 96}, {144, 184}}, show});
  Run(h, w,
      {"TuningScreen::AnimateStickerSlots", 100, __imp__sub_821F8A50,
       hooks::TuningPagesAnimateStickerSlots, 56, {{144, 184}}, show});
  Run(h, w,
      {"TuningScreen::AnimateStickerEditMenu", 100, __imp__sub_821F8D68,
       hooks::TuningPagesAnimateStickerEditMenu, 64, {{80, 112}, {160, 224}, {272, 312}}, show});
  Run(h, w,
      {"TuningScreen::ShowStickerOrder", 100, __imp__sub_821FBA40,
       hooks::TuningPagesShowStickerOrder, 48, {{144, 224}}, show});
  Run(h, w,
      {"TuningScreen::ShowStickerPicker", 100, __imp__sub_821FBB60,
       hooks::TuningPagesShowStickerPicker, 64, {{80, 112}, {160, 240}}, show});
}

void TuningPagesMovie(Context& h) {
  World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: tuning_pages: no memory for the pages' movie world");
    return;
  }
  // Menu::StepList: r3 the menu (its high word now and then set); the
  // mappers, the menu's fields and the player (above). __savegprlr_27: 48
  // bytes.
  Run(h, w,
      {"Menu::StepList", 60, __imp__sub_821CE1D0, hooks::TuningPagesStepList, 48, {},
       [](PPCContext& t, Random& r, const World& w) {
         t.r3.u64 = w.menu();
         HighWord(t, r);
       }});
  // LeaveEditor: the fades' counts (above; one set now and then, for the
  // early return), r3's high word; __savegprlr_28: 40 bytes.
  Run(h, w,
      {"TuningScreen::LeaveEditor", 40, __imp__sub_821D8670, hooks::TuningPagesLeaveEditor, 40,
       {}, [](PPCContext& t, Random& r, const World& w) {
         if (r.Below(4) == 0) {
           guest::StoreU32(w.base, r.Below(2) == 0 ? kFadeIn : kFadeOut, 1 + r.Below(30));
         }
         HighWord(t, r);
       }});
  // InputCategories: the menu, the mappers and the category (above); r4 any
  // word (not read). Its r30, r31 and lr (24 bytes); with B, UnloadAll's
  // (112 to 136 below) and its callees' (RestoreVinylMovie's and
  // RestoreStickerMovie's, Unload's) saves, which their plain C++ doesn't
  // write; MoveCategory's (r29 to r31 and lr, 112 to 144 below: its adapter
  // went, B7 loop, 2026-11-11, so the recompiled side runs its original).
  Run(h, w,
      {"TuningScreen::InputCategories", 80, __imp__sub_821D6AD0,
       hooks::TuningPagesInputCategories, 24,
       {{112, 144}, {224, 256}, {320, 376}, {432, 456}},
       [](PPCContext& t, Random& r, const World&) {
         t.r4.u64 = r.Next();
         HighWord(t, r);
       }});
  // BackToItems: the fades' counts (one set now and then), r3's high word, r4 any word (not
  // read); a 4 put on the history below its depth (or, with depth 0, past
  // the wrap at bytes 64 to 69). Its r31 and lr (16 bytes); UnloadAll's
  // saves (96 to 120 below) and its callees' (as InputCategories').
  Run(h, w,
      {"TuningScreen::BackToItems", 60, __imp__sub_821D87C0, hooks::TuningPagesBackToItems, 16,
       {{96, 120}, {208, 240}, {304, 360}, {416, 440}},
       [](PPCContext& t, Random& r, const World& w) {
         const uint32_t s = w.screen();
         const uint32_t depth = guest::LoadU8(w.base, s + 10008);
         const uint32_t at = depth == 0 ? 64 + r.Below(6) : r.Below(depth);
         guest::StoreU32(w.base, s + (at + 2439) * 4, 4);
         // A fade running now and then (one count or the other set).
         if (r.Below(6) == 0) {
           guest::StoreU32(w.base, r.Below(2) == 0 ? kFadeIn : kFadeOut, 1 + r.Below(30));
         }
         t.r4.u64 = r.Next();
         HighWord(t, r);
       }});
  // AnimateWheelParts and AnimateInches: the mappers (above), the fields
  // (the part row any); __savegprlr_29: 32 bytes.
  Run(h, w,
      {"TuningScreen::AnimateWheelParts", 120, __imp__sub_821E8CE0,
       hooks::TuningPagesAnimateWheelParts, 32, {},
       [](PPCContext& t, Random& r, const World& w) { WheelFields(t, r, w, true); }});
  Run(h, w,
      {"TuningScreen::AnimateInches", 120, __imp__sub_821E9110, hooks::TuningPagesAnimateInches,
       32, {}, [](PPCContext& t, Random& r, const World& w) { WheelFields(t, r, w, true); }});
  // InputWheelParts: __savegprlr_27: 48 bytes; AnimateWheelParts' saves
  // (128 to 160 below).
  Run(h, w,
      {"TuningScreen::InputWheelParts", 100, __imp__sub_821EE390,
       hooks::TuningPagesInputWheelParts, 48, {{128, 160}},
       [](PPCContext& t, Random& r, const World& w) { WheelFields(t, r, w, false); }});
  // InputInches: its r30, r31 and lr (24 bytes); AnimateInches' saves (112
  // to 144 below).
  Run(h, w,
      {"TuningScreen::InputInches", 100, __imp__sub_821EEEA8, hooks::TuningPagesInputInches, 24,
       {{112, 144}},
       [](PPCContext& t, Random& r, const World& w) { WheelFields(t, r, w, false); }});
  // MoveItem: state 4's fields (above); __savegprlr_29: 32 bytes.
  Run(h, w,
      {"TuningScreen::MoveItem", 300, __imp__sub_821D0CC0, hooks::TuningPagesMoveItem, 32, {},
       ItemFields});
  // InputItems: its saves (__savegprlr_28: 40 bytes); StepList's,
  // MoveItem's and LoadVinyls' (128 to 176 below: their plain C++ doesn't
  // write them, LoadDecalImages' too) and LoadDecalImages' Request's or
  // Load's (r31 and lr, 256 to 272 below); StoreToGarageCar's
  // (__savegprlr_25, 128 to 192 below: its adapter went, B7 loop,
  // 2026-11-11, so the recompiled side runs its original); ScenePools::
  // GetB's back chain (236 to 240 below: its adapter went, B7 loop,
  // 2026-11-12; its saves fall in StoreToGarageCar's); Archive::Load
  // recorded.
  Run(h, w,
      {"TuningScreen::InputItems", 500, __imp__sub_821DFD48, hooks::TuningPagesInputItems, 40,
       {{128, 224}, {236, 240}, {256, 272}, {304, 336}}, ItemFields, true});
  // The sticker editor's set-up under InputItems (the item fields' world:
  // the garage list, the cars' decal records): ResetStickerEditor a leaf;
  // RestoreAreaDecals' __savegprlr_29 (32 bytes), r4 the area 0 to 7 (now
  // and then with a high word: the original's 64-bit adds); LoadStickers'
  // __savegprlr_26 and __savefpr_27 (96 bytes) and RestoreAreaDecals' saves
  // below its frame (176 to 208), which its plain C++ doesn't write.
  Run(h, w,
      {"TuningScreen::ResetStickerEditor", 100, __imp__sub_821F8440,
       hooks::TuningPagesResetStickerEditor, 0, {}, ItemFields});
  Run(h, w,
      {"TuningScreen::RestoreAreaDecals", 100, __imp__sub_821F6C90,
       hooks::TuningPagesRestoreAreaDecals, 32, {},
       [](PPCContext& t, Random& r, const World& w) {
         ItemFields(t, r, w);
         t.r4.u64 = r.Below(8);
       }});
  Run(h, w,
      {"TuningScreen::LoadStickers", 200, __imp__sub_821F86C0, hooks::TuningPagesLoadStickers, 96,
       {{176, 208}}, ItemFields});
  // MoveOption: state 8's fields (above); __savegprlr_28: 40 bytes.
  Run(h, w,
      {"TuningScreen::MoveOption", 300, __imp__sub_821D5610, hooks::TuningPagesMoveOption, 40, {},
       OptionFields});
  // InputOptions: its r30, r31 and lr (24 bytes); MoveOption's saves (112 to
  // 152 below: its plain C++ doesn't write them).
  Run(h, w,
      {"TuningScreen::InputOptions", 400, __imp__sub_821D8160, hooks::TuningPagesInputOptions, 24,
       {{112, 152}}, OptionFields});
  // MoveLevel: state 5's fields (above); __savegprlr_28: 40 bytes.
  Run(h, w,
      {"TuningScreen::MoveLevel", 800, __imp__sub_821D1A18, hooks::TuningPagesMoveLevel, 40, {},
       LevelFields});
  // MoveType: state 6's fields (above); __savegprlr_28: 40 bytes.
  Run(h, w,
      {"TuningScreen::MoveType", 800, __imp__sub_821D32C8, hooks::TuningPagesMoveType, 40, {},
       TypeFields});
  // InputLevels: its r30, r31 and lr (24 bytes); StepList's and MoveLevel's
  // saves (112 to 160 below: their plain C++ doesn't write them).
  Run(h, w,
      {"TuningScreen::InputLevels", 800, __imp__sub_821D6E20, hooks::TuningPagesInputLevels, 24,
       {{112, 160}}, LevelInputFields});
  // InputTypes: its r30, r31 and lr (24 bytes); MoveType's saves (112 to
  // 160 below: its plain C++ doesn't write them).
  Run(h, w,
      {"TuningScreen::InputTypes", 800, __imp__sub_821D74A0, hooks::TuningPagesInputTypes, 24,
       {{112, 160}}, TypeInputFields});
  // StoreOwnedParts: state 59's fields (above); its r30, r31 and lr (24
  // bytes).
  Run(h, w,
      {"TuningScreen::StoreOwnedParts", 300, __imp__sub_821DD408,
       hooks::TuningPagesStoreOwnedParts, 24, {}, OwnedFields});
  // Leave: its r30, r31 and lr (24 bytes); StoreToGarageCar's saves (112 to
  // 176 below: its plain C++ doesn't write them) but StoreOwnedParts' table
  // (144 to 160), which both write over them.
  Run(h, w,
      {"TuningScreen::Leave", 400, __imp__sub_821E25B0, hooks::TuningPagesLeave, 24,
       {{112, 144}, {160, 176}}, OwnedFields});
  // The wheel list's rows after a step right or left: r23 to r31 and lr
  // saved (80 bytes); HidePartMarks' and ShowPartMarks' saves (176 to 216
  // below: their originals run on the recompiled side since their adapters
  // went, 2026-11-07, and their plain C++ doesn't write them).
  Run(h, w,
      {"TuningScreen::FillPartRowsRight", 150, __imp__sub_821F40B8,
       hooks::TuningPagesFillPartRowsRight, 80, {{176, 216}},
       [](PPCContext& t, Random& r, const World& w) { WheelsFields(t, r, w, false); }});
  Run(h, w,
      {"TuningScreen::FillPartRowsLeft", 150, __imp__sub_821F4768,
       hooks::TuningPagesFillPartRowsLeft, 80, {{176, 216}},
       [](PPCContext& t, Random& r, const World& w) { WheelsFields(t, r, w, false); }});
  // AnimateWheels: __savegprlr_29 (32 bytes); FillPartRowsRight's and
  // Left's saves (128 to 208 below), which their plain C++ doesn't write.
  // Since the adapters of FillPartRows, SetPartName, PlayGroupAnimation and
  // the part marks went (2026-11-07), their originals run on the recompiled
  // side: also HidePartMarks' and ShowPartMarks' saves under FillPartRows
  // (304 to 344) and SetPartName's frame's +48 to +79 (896 to 928: SetText's
  // parameter area, the r7 to r10 its calls leave unset, passed as 0
  // natively, the user's call of 2026-10-30).
  Run(h, w,
      {"TuningScreen::AnimateWheels", 300, __imp__sub_821F60E0, hooks::TuningPagesAnimateWheels,
       32, {{128, 208}, {304, 344}, {896, 928}},
       [](PPCContext& t, Random& r, const World& w) { WheelsFields(t, r, w, false); }});
  // InputWheels: __savegprlr_29 (32 bytes); its frame's +48 to +79
  // (SetText's parameter area: the r7 to r10 the calls leave unset, passed
  // as 0 natively, the user's call of 2026-10-30), AnimateWheels' saves (128
  // to 160 below) and FillPartRowsRight's and Left's (256 to 336); as
  // AnimateWheels' own (2026-11-07), 128 deeper: the part marks' saves (432
  // to 472) and SetPartName's SetText parameter area (1024 to 1056).
  Run(h, w,
      {"TuningScreen::InputWheels", 600, __imp__sub_821F6688, hooks::TuningPagesInputWheels, 32,
       {{48, 80}, {128, 160}, {256, 336}, {432, 472}, {1024, 1056}},
       [](PPCContext& t, Random& r, const World& w) { WheelsFields(t, r, w, true); }});
  // The decal edits' movie sides: __savegprlr_29 (32 bytes).
  Run(h, w,
      {"TuningScreen::MoveDecalPosition", 200, __imp__sub_82200DC0,
       hooks::TuningPagesMoveDecalPosition, 32, {}, DecalEditFields});
  Run(h, w,
      {"TuningScreen::MoveDecalAngle", 150, __imp__sub_822012E8, hooks::TuningPagesMoveDecalAngle,
       32, {}, DecalEditFields});
  Run(h, w,
      {"TuningScreen::MoveDecalScale", 150, __imp__sub_82201620, hooks::TuningPagesMoveDecalScale,
       32, {}, DecalEditFields});
  // Their pages: r30, r31 and lr (24 bytes); the movie side's saves and
  // HighlightSelected's (its r29 to r31, lr, f30, f31: 48 bytes) and
  // memcpy's r3 (112 to 160 below: the guest's hold the original's
  // registers, which the native version doesn't set; the movie side's plain
  // C++ doesn't write them).
  Run(h, w,
      {"TuningScreen::InputDecalPosition", 300, __imp__sub_82206A50,
       hooks::TuningPagesInputDecalPosition, 24, {{112, 160}}, DecalEditFields});
  Run(h, w,
      {"TuningScreen::InputDecalAngle", 200, __imp__sub_82206F48,
       hooks::TuningPagesInputDecalAngle, 24, {{112, 160}}, DecalEditFields});
  Run(h, w,
      {"TuningScreen::InputDecalScale", 200, __imp__sub_82207320,
       hooks::TuningPagesInputDecalScale, 24, {{112, 160}}, DecalEditFields});
  // The layer's edit menu: MoveLayerEdit's __savegprlr_29 (32 bytes);
  // InputLayerEdit's r30, r31 and lr (24) with its callees' saves as the
  // decal edits' (112 to 160 below).
  Run(h, w,
      {"TuningScreen::MoveLayerEdit", 300, __imp__sub_82200760, hooks::TuningPagesMoveLayerEdit,
       32, {}, LayerEditFields});
  Run(h, w,
      {"TuningScreen::InputLayerEdit", 400, __imp__sub_82205110, hooks::TuningPagesInputLayerEdit,
       24, {{112, 160}}, LayerEditFields});
  // The items' highlights (tuning_screen's plain C++): the decal edits'
  // fields (the page 0 or 1 mostly, now and then 2 or -1; the layer 0 to
  // 2), r4 0 or 1 (now and then any word), HighlightItem's r5 -1 to 3 (now
  // and then any word). Their saves: r28 to r31, lr, f30 and f31 (56
  // bytes), r29 to r31, lr, f30 and f31 (48).
  Run(h, w,
      {"TuningScreen::HighlightItem", 120, __imp__sub_82203400, hooks::TuningPagesHighlightItem,
       56, {}, [](PPCContext& t, Random& r, const World& w) {
         DecalEditFields(t, r, w);
         t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(2);
         t.r5.u64 = r.Below(6) == 0 ? r.Next() : uint64_t(int64_t(r.Below(5)) - 1);
       }});
  Run(h, w,
      {"TuningScreen::HighlightSelected", 120, __imp__sub_82203570,
       hooks::TuningPagesHighlightSelected, 48, {},
       [](PPCContext& t, Random& r, const World& w) {
         DecalEditFields(t, r, w);
         t.r4.u64 = r.Below(6) == 0 ? r.Next() : r.Below(2);
         if (r.Below(6) == 0) {
           guest::StoreU32(w.base, w.screen() + 30732, uint32_t(r.Next()));
         }
       }});
  // The vinyl page's layers: MoveVinylLayers' __savegprlr_29 (32 bytes);
  // InputVinylLayers' __savegprlr_22 (88) with its callees' saves its
  // native version doesn't write: MoveVinylLayers', RestoreStickerTexture's
  // and LoadWindow's (176 to 232 below), Reload's under LoadWindow (320 to
  // 336); Archive::Load recorded.
  Run(h, w,
      {"TuningScreen::MoveVinylLayers", 300, __imp__sub_821FFB20,
       hooks::TuningPagesMoveVinylLayers, 32, {}, VinylLayerFields});
  Run(h, w,
      {"TuningScreen::InputVinylLayers", 400, __imp__sub_82208CE0,
       hooks::TuningPagesInputVinylLayers, 88, {{176, 232}, {320, 336}}, VinylLayerFields, true});
  // The image picker's movie side: __savegprlr_29 (32 bytes).
  Run(h, w,
      {"TuningScreen::MoveVinylImages", 200, __imp__sub_82202530,
       hooks::TuningPagesMoveVinylImages, 32, {}, VinylImageFields});
  // Its page: __savegprlr_14 (152 bytes) with its callees' saves its native
  // version doesn't write: MoveVinylImages' (32), Scroll's (40),
  // LoadWindow's (56), Reload's (16) and BindVinylTexture's (32) below its
  // 240-byte frame (240 to 296 below), Reload's under Scroll's and
  // LoadWindow's frames (368 to 400); Archive::Load recorded.
  Run(h, w,
      {"TuningScreen::InputVinylImages", 500, __imp__sub_822058F8,
       hooks::TuningPagesInputVinylImages, 152, {{240, 296}, {368, 400}}, VinylPickFields, true});
  // The operate select: BeginLayerOrder a leaf (no saves);
  // MoveLayerActions' __savegprlr_29 (32 bytes); InputLayerActions'
  // __savegprlr_22 (88) with its callees' saves its native version doesn't
  // write: MoveLayerActions', HighlightSelected's, RestoreStickerTexture's
  // and LoadWindow's (176 to 232 below), Reload's under LoadWindow (320 to
  // 336); Archive::Load recorded.
  Run(h, w,
      {"TuningScreen::BeginLayerOrder", 100, __imp__sub_821FE370,
       hooks::TuningPagesBeginLayerOrder, 0, {}, LayerActionFields});
  Run(h, w,
      {"TuningScreen::MoveLayerActions", 300, __imp__sub_82200230,
       hooks::TuningPagesMoveLayerActions, 32, {}, LayerActionFields});
  Run(h, w,
      {"TuningScreen::InputLayerActions", 500, __imp__sub_82209738,
       hooks::TuningPagesInputLayerActions, 88, {{176, 232}, {320, 336}}, LayerActionFields,
       true});
  // The sticker editor's group helpers: leaves.
  Run(h, w,
      {"TuningScreen::SetGroupImage", 100, __imp__sub_821FD5C0, hooks::TuningPagesSetGroupImage,
       0, {}, GroupFields});
  Run(h, w,
      {"TuningScreen::SetImageGroup", 100, __imp__sub_821FD658, hooks::TuningPagesSetImageGroup,
       0, {}, GroupFields});
  // The layer order: EndLayerOrder a leaf; SwapLayerRecords' and
  // SwapOrderTextures' __savegprlr_29 (32 bytes); ShowOrderNodes'
  // __savegprlr_28 (48); MoveLayerOrder's r30, r31 and lr (24);
  // InputLayerOrder's __savegprlr_23 (80) with its callees' saves its native
  // version doesn't write (176 to 224 below: ShowOrderNodes',
  // MoveLayerOrder's, SwapLayerRecords', SwapOrderTextures' and
  // ShowOptionMarks').
  Run(h, w,
      {"TuningScreen::EndLayerOrder", 100, __imp__sub_821FE3C8, hooks::TuningPagesEndLayerOrder,
       0, {}, LayerOrderFields});
  Run(h, w,
      {"TuningScreen::SwapLayerRecords", 150, __imp__sub_821FD518,
       hooks::TuningPagesSwapLayerRecords, 32, {}, LayerOrderFields});
  Run(h, w,
      {"TuningScreen::SwapOrderTextures", 150, __imp__sub_82207F38,
       hooks::TuningPagesSwapOrderTextures, 32, {}, LayerOrderFields});
  Run(h, w,
      {"TuningScreen::ShowOrderNodes", 150, __imp__sub_82203A30, hooks::TuningPagesShowOrderNodes,
       48, {}, LayerOrderFields});
  Run(h, w,
      {"TuningScreen::MoveLayerOrder", 200, __imp__sub_82201DB8, hooks::TuningPagesMoveLayerOrder,
       24, {}, LayerOrderFields});
  Run(h, w,
      {"TuningScreen::InputLayerOrder", 800, __imp__sub_8220A1C8,
       hooks::TuningPagesInputLayerOrder, 80, {{176, 224}}, LayerOrderFields});
  // TuningScreen::Command21 (tuning_screen's plain C++, the update's
  // command 21): the layer order's fields with the layer +30732 and the
  // position +30756 0 to 2 (up to two moves either way). __savegprlr_27 (48
  // bytes); its callees' saves (144 to 224 below: SwapLayerRecords',
  // SwapOrderTextures', Command20's and LayOutItems', their plain C++ not
  // writing them).
  Run(h, w,
      {"TuningScreen::Command21", 300, __imp__sub_822087C8, hooks::TuningCommand21, 48,
       {{144, 224}}, [](PPCContext& t, Random& r, const World& w) {
         LayerOrderFields(t, r, w);
         // The layer and the position 0 to 2, as play leaves them: the moves
         // stay on the work list's three words (0 to 2), so Command20's
         // named animations are the layers' (a position of 3 reads the
         // sticker records past the list as ids: wild animation indices).
         guest::StoreU32(w.base, w.screen() + 30732, r.Below(3));
         guest::StoreU32(w.base, w.screen() + 30756, r.Below(3));
       }});
  // TuningScreen::HandleInput (its piece sub_821E69B8 merged): the states
  // with no page, the empty ones and the piece's (with the screen's own
  // fields), then each page this world holds with its group's fields (the
  // pages' saves and holes as their groups give them). Not here: the pages
  // never seen in play it calls through the guest (7, 18 to 22, 38 to 46,
  // 49, 53 to 58: the same function on both sides) and the vinyl view, the
  // vinyl editor's menu and EnterEditor (23, 34, 60: their worlds are
  // car_makers' and tuning_pages.cpp's; HandleInput only calls them).
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (no page)", 120,
                      {0, 0, 0, 1, 2, 9, 10, 11, 12, 13, 14, 32, 33, 35, 36, 37, 47, 48, 50, 51,
                       52, 63, 64, 200, 0xFFFFFFFFu, 0x80000000u},
                      0, {}, [](PPCContext& t, Random& r, const World&) {
                        t.r4.u64 = r.Next();
                        HighWord(t, r);
                      }));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (3)", 40, {3}, 24,
                      {{112, 144}, {224, 256}, {320, 376}, {432, 456}},
                      [](PPCContext& t, Random& r, const World&) {
                        t.r4.u64 = r.Next();
                        HighWord(t, r);
                      }));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (4)", 100, {4}, 40,
                      {{128, 224}, {236, 240}, {256, 272}, {304, 336}}, ItemFields, true));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (5)", 100, {5}, 24, {{112, 160}},
                      LevelInputFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (6)", 100, {6}, 24, {{112, 160}},
                      TypeInputFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (8)", 100, {8}, 24, {{112, 152}}, OptionFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (15)", 50, {15}, 48, {{128, 160}},
                      [](PPCContext& t, Random& r, const World& w) { WheelFields(t, r, w, false); }));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (16)", 100, {16}, 32,
                      {{48, 80}, {128, 160}, {256, 336}, {432, 472}, {1024, 1056}},
                      [](PPCContext& t, Random& r, const World& w) { WheelsFields(t, r, w, true); }));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (17)", 50, {17}, 24, {{112, 144}},
                      [](PPCContext& t, Random& r, const World& w) { WheelFields(t, r, w, false); }));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (24)", 80, {24}, 88, {{176, 232}, {320, 336}},
                      VinylLayerFields, true));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (25)", 80, {25}, 88, {{176, 232}, {320, 336}},
                      LayerActionFields, true));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (26)", 80, {26}, 24, {{112, 160}},
                      LayerEditFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (27)", 100, {27}, 80, {{176, 224}},
                      LayerOrderFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (28)", 80, {28}, 152, {{240, 296}, {368, 400}},
                      VinylPickFields, true));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (29)", 60, {29}, 24, {{112, 160}},
                      DecalEditFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (30)", 40, {30}, 24, {{112, 160}},
                      DecalEditFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (31)", 40, {31}, 24, {{112, 160}},
                      DecalEditFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (59)", 80, {59}, 24, {{112, 144}, {160, 176}},
                      OwnedFields));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (61)", 40, {61}, 40, {},
                      [](PPCContext& t, Random& r, const World& w) {
                        if (r.Below(4) == 0) {
                          guest::StoreU32(w.base, r.Below(2) == 0 ? kFadeIn : kFadeOut,
                                          1 + r.Below(30));
                        }
                        HighWord(t, r);
                      }));
  Run(h, w,
      HandleInputSpec("TuningScreen::HandleInput (62)", 40, {62}, 16,
                      {{96, 120}, {208, 240}, {304, 360}, {416, 440}},
                      [](PPCContext& t, Random& r, const World& w) {
                        const uint32_t s = w.screen();
                        const uint32_t depth = guest::LoadU8(w.base, s + 10008);
                        const uint32_t at = depth == 0 ? 64 + r.Below(6) : r.Below(depth);
                        guest::StoreU32(w.base, s + (at + 2439) * 4, 4);
                        if (r.Below(6) == 0) {
                          guest::StoreU32(w.base, r.Below(2) == 0 ? kFadeIn : kFadeOut,
                                          1 + r.Below(30));
                        }
                        t.r4.u64 = r.Next();
                        HighWord(t, r);
                      }));
  StickerCases(h, w);
}

}  // namespace harness
