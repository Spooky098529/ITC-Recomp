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
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <functional>
#include <optional>
#include <string>
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
  HighWord(t, r);
}

}  // namespace

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
  // write.
  Run(h, w,
      {"TuningScreen::InputCategories", 80, __imp__sub_821D6AD0,
       hooks::TuningPagesInputCategories, 24,
       {{112, 136}, {224, 256}, {320, 376}, {432, 456}},
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
  // Load's (r31 and lr, 256 to 272 below); Archive::Load recorded.
  Run(h, w,
      {"TuningScreen::InputItems", 500, __imp__sub_821DFD48, hooks::TuningPagesInputItems, 40,
       {{128, 176}, {256, 272}}, ItemFields, true});
}

}  // namespace harness
