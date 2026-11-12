// The type 9 mode, the career's end of the day (src/todays_result/,
// docs/functions/todays-result.md), against the recompiled functions, laid
// as the replay theater's harness lays its world (type10_mode.cpp). Each
// case fills a world of its own (one block from the allocation layer) with
// random words from the case's seed: the type 9 block (today's result's
// machine at +0, today's event's at +9640), the mode with its message ring
// and player 0's save data (a block of its own, [0x826C5B58] pointed at
// it), the fields each function tests shaped to their play ranges; points
// the globals into it (put back after); runs the original (with
// --native_todays_result off, so what it calls of the mode's is the
// original too) or the register steps (hooks/todays_result.h); and compares
// the world, the save data's words the functions read or write, the
// globals, the function's frame without its register saves, the caller's
// area above r1, r1 and r3.

#include "harness.h"

#include "loading_world.h"

#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "archive/archive.h"
#include "data_tables/data_tables.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "hooks/todays_result.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "menu_helpers/menu_helpers.h"
#include "menu_sounds/menu_sounds.h"
#include "ui_movies/game.h"
#include "memory/heap.h"
#include "race_loading/race_loading.h"
#include "todays_result/todays_result.h"

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_todays_result);

namespace time_hooks {
void SetFakeNow(uint64_t now);
void ClearFakeNow();
}  // namespace time_hooks

namespace harness {

namespace {

namespace tr = todays_result;

// The block's layout.
constexpr uint32_t kTypeBlockAt = 0x0;      // the type 9 block (19,300 bytes)
constexpr uint32_t kModeAt = 0x4C00;        // the mode (852 bytes; its ring at +240)
constexpr uint32_t kAnswerAt = 0x4F80;      // the delegate functions' answer
constexpr uint32_t kResultPagesAt = 0x5000; // [0x828160C8] (28 bytes)
constexpr uint32_t kResultMenuAt = 0x5040;  // [0x828160CC] (68 bytes)
constexpr uint32_t kResultDataAt = 0x5100;  // [0x828160D0] (68 bytes asked, 88 used)
constexpr uint32_t kEventPagesAt = 0x5200;  // [0x828160D4] (28 bytes)
constexpr uint32_t kEventMenuAt = 0x5240;   // [0x828160D8] (68 bytes)
constexpr uint32_t kEventDataAt = 0x5300;   // [0x828160DC] (68 bytes asked, 412 used)
constexpr uint32_t kPairsAt = 0x5500;       // the movie table's pairs (27 and 0)
constexpr uint32_t kMenuWorldAt = 0x5600;   // the sound manager and its vtable (+0x400)
constexpr uint32_t kMappersAt = 0x5C00;     // 4 action mappers of 0x200
constexpr uint32_t kMapperVtablesAt = 0x6400;  // their vtables (0x40 each)
constexpr uint32_t kResultMovieAt = 0x6800;    // movie 27 (0xA100 bytes)
constexpr uint32_t kEventMovieAt = 0x10C00;    // movie 0 (0xA100 bytes)
constexpr uint32_t kNodesAt = 0x1B000;         // 40 nodes of 0x400
constexpr uint32_t kNodeBytes = 0x400;
constexpr uint32_t kNodeCount = 40;
constexpr uint32_t kAnimationsAt = 0x25000;  // 8 animations of no tracks (64 bytes each)
constexpr uint32_t kAnimations = 8;
constexpr uint32_t kNamed = 200;  // the movies' named animations shaped
constexpr uint32_t kTablesAt = 0x25200;      // the data tables' object ([0x826C5CA0], 0x80)
constexpr uint32_t kTable0At = 0x25280;      // its tables 0 to 8 (32 56-byte entries)
constexpr uint32_t kTableEntries = 32;
constexpr uint32_t kTable40At = 0x25A00;     // its 40-byte table
constexpr uint32_t kTocAt = 0x25A80;         // BUILD.TOC's entries for the images' assets
constexpr uint32_t kRecordsAt = 0x26080;     // image records (16 bytes each, 92)
constexpr uint32_t kMovieObjectAt = 0x26680; // the movies' +28 object (64 bytes)
constexpr uint32_t kBlockBytes = 0x26800;

// Player 0's save data (a block of its own): its first 0x1800 bytes (the
// header, the stats record) and from +0x1C700 (the unlocks just made, the
// movie queued) random and compared.
constexpr uint32_t kSaveBytes = 0x1D600;
constexpr uint32_t kSaveHeadBytes = 0x1800;
constexpr uint32_t kSaveTailAt = 0x1C700;
constexpr uint32_t kSaveTable = 0x826C5B58;

constexpr uint32_t kStackBytes = 2048;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kFadesBytes = 64;

struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr uint32_t kMissingMovie = 0x826C5CDC;
constexpr uint32_t kMissingText = 0x826C5CD8;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kRaceEvent = 0x826C6078;
constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"
constexpr Global kGlobals[] = {
    {kSaveTable, 4},
    {kFades, kFadesBytes},
    {race_loading::kLoadingCurrent, 4},
    {tr::kResultPages, 24},  // both screens' three blocks
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kRaceEvent, 4},
    {data_tables::kTables, 4},
    {archive::kBuildToc, 4},
    {tr::kBootMovies, 32},
    {menu_helpers::kMapperEntries, 32},
    {menu_helpers::kControllerEntries, 16},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
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
  uint32_t node(uint32_t n) const { return at(kNodesAt) + kNodeBytes * n; }
  uint32_t result() const { return at(kTypeBlockAt); }
  uint32_t event() const { return at(kTypeBlockAt) + tr::kEventAt; }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t dispatcher() const { return mode() + tr::kDispatcher; }
  uint8_t* base;
  uint32_t allocation;
  uint32_t block;
  uint32_t save;
  // A string table of our own ([0x826C5C34] pointed at it): its 18 files
  // all one container of 8,192 entries over 16 strings of letters, so any id
  // below 8,192 has a string; no argument codes (the varargs the native
  // callers pass as 0 aren't read).
  static constexpr uint32_t kStringsBytes = 0x9000;
  uint32_t strings;

 private:
  void MakeStrings() {
    Random r(0x546F646179ull);  // "Today"
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
        guest::StoreU16(base, at + 2 * c, uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, at + 2 * length, 0);
    }
  }
};

// A movie over the pool: its live scene node 0, the named animations over
// the root (each one of the 8 animations of no tracks); its loaded flag set
// or not.
void MakeMovie(uint8_t* base, const World& w, uint32_t movie, Random& r) {
  ui_movies::InitAnimationLists(base, movie);
  for (const uint32_t field : {16u, 20u, 24u}) {
    guest::StoreU32(base, movie + field, w.node(0));
  }
  for (uint32_t k = 0; k < kNamed; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, w.node(0));
    guest::StoreU32(base, named + 32, w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
  }
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, 0);
  guest::StoreU32(base, movie + 0x9FCC, r.Below(7));
  guest::StoreU32(base, movie + 40960, r.Below(3));
  guest::StoreU32(base, movie + ui_movies::kMovieLoaded, r.Below(2));
  guest::StoreU32(base, movie + 28, w.at(kMovieObjectAt));
}

// A name record (SetChildNameText's: +4 a kind, +8 a 16-bit index, +10 a
// flag; +12 a second kind, +16 its index, +18 its flag) at `name`, the
// kinds 0 to 5 and the indexes mostly in range (as the garage screen's
// harness makes it).
void NameRecord(uint8_t* base, uint32_t name, Random& r) {
  guest::StoreU32(base, name + 4, r.Below(6));
  guest::StoreU16(base, name + 8, uint16_t(r.Below(4) == 0 ? r.Next() : r.Below(110)));
  guest::StoreU16(base, name + 10, uint16_t(r.Below(2)));
  guest::StoreU32(base, name + 12, r.Below(6));
  guest::StoreU16(base, name + 16, uint16_t(r.Below(4) == 0 ? r.Next() : r.Below(110)));
  guest::StoreU16(base, name + 18, uint16_t(r.Below(2)));
}

// The states the machines take in play (and a random word now and then).
constexpr uint32_t kStates[] = {0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 13, 20};

// The save data's words Kind reads: the unlocks just made's count (0 or 1
// to 8: the save holds 64 at most, and the cases shape the first 64) and
// the movie queued (-1, 0 to 8, or a random word).
void ShapeSave(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, w.save + tr::kUnlockCount, r.Below(3) == 0 ? 0 : 1 + r.Below(8));
  const uint32_t movie = r.Below(5);
  guest::StoreU32(base, w.save + tr::kQueuedMovie,
                  movie == 0 ? 0xFFFFFFFFu : movie == 4 ? uint32_t(r.Next()) : r.Below(9));
}

// The sound manager ([0x826C5C80]): PlayCue2D (slot 10, MenuSound::Play's)
// the getter sub_82090770 (r3 = +60), as the replay theater's harness uses
// it.
void SoundManager(uint8_t* base, const World& w, Random& r) {
  const uint32_t manager = w.at(kMenuWorldAt), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, manager + 60,
                  r.Below(2) == 0 ? uint32_t(r.Next()) & ~0xFFu : uint32_t(r.Next()));
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, 0x82090770);
  // StopCue (slot 13) and IsCuePlaying (slot 14, its low byte) the getter
  // too.
  guest::StoreU32(base, vtable + 52, 0x82090770);
  guest::StoreU32(base, vtable + 56, 0x82090770);
}

// The menus' input, as the menu helpers' harness shapes it: each
// controller's entry 0 to 3, each entry's mapper one of ours (its slots 10
// and 12 Pressed and Fired, now and then Held), blocked now and then, a few
// bits set; the menus' player 0 to 3 and their last step's move and side
// mostly 0.
constexpr uint32_t kHeldSlot = 0x8210BE70;
void MenuInput(uint8_t* base, const World& w, Random& r) {
  namespace mh = menu_helpers;
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t mapper = w.at(kMappersAt) + 0x200 * c;
    const uint32_t vtable = w.at(kMapperVtablesAt) + 0x40 * c;
    guest::StoreU32(base, mh::kControllerEntries + 4 * c, r.Below(4));
    guest::StoreU32(base, mh::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, mh::kMapperEntries + 8 * c + 4, r.Below(5) == 0 ? 1 : r.Below(3));
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + mh::kPressedOffset,
                    r.Below(5) == 0 ? kHeldSlot : mh::kPressedSlot);
    guest::StoreU32(base, vtable + mh::kFiredOffset, r.Below(5) == 0 ? kHeldSlot : mh::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  for (const uint32_t menu : {w.at(kResultMenuAt), w.at(kEventMenuAt)}) {
    guest::StoreU32(base, menu + mh::kPlayer, r.Below(4));
    guest::StoreU32(base, menu + mh::kMoved, r.Below(4) == 0 ? r.Below(3) - 1 : 0);
    guest::StoreU32(base, menu + mh::kSide, r.Below(4) == 0 ? r.Below(3) - 1 : 0);
  }
}

void MakeWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.block, kBlockBytes, r);
  WriteRandom(base, w.save, kSaveHeadBytes, r);
  WriteRandom(base, w.save + kSaveTailAt, kSaveBytes - kSaveTailAt, r);
  // The page objects' words small or -1.
  for (const uint32_t pages : {w.at(kResultPagesAt), w.at(kEventPagesAt)}) {
    for (uint32_t k = 0; k < 7; k++) {
      guest::StoreU32(base, pages + 4 * k, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(3));
    }
  }
  // The nodes: all TXTs, each node's children the pool's (their index +108
  // small).
  for (uint32_t n = 0; n < kNodeCount; n++) {
    const uint32_t node = w.node(n);
    guest::StoreU32(base, node + 0, kTxtTag);
    guest::StoreU32(base, node + 100, r.Below(30));
    guest::StoreU32(base, node + 104, r.Below(8));
    guest::StoreU32(base, node + 108, r.Below(4));
    for (uint32_t e = 0; e < (kNodeBytes - 128) / 8; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.node(r.Below(kNodeCount)));
    }
    // The scenes' animations: the 8 of no tracks (never a wild entry).
    guest::StoreU32(base, node + 116, 40 + r.Below(8));
    for (uint32_t e = 40; e < 64; e++) {
      guest::StoreU32(base, node + 128 + 8 * e, w.at(kAnimationsAt) + 64 * r.Below(kAnimations));
    }
  }
  for (uint32_t k = 0; k < kAnimations; k++) {
    const uint32_t animation = w.at(kAnimationsAt) + 64 * k;
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, animation + i, 0);
    }
    guest::StoreFloat(base, animation + 12, float(r.Below(100)) / 4.0f);
  }
  MakeMovie(base, w, w.at(kResultMovieAt), r);
  MakeMovie(base, w, w.at(kEventMovieAt), r);
  // The root's own children none (+104 0).
  guest::StoreU32(base, w.node(0) + 104, 0);
  // The pairs sorted by id (0, then 27).
  guest::StoreU32(base, w.at(kPairsAt), 0);
  guest::StoreU32(base, w.at(kPairsAt) + 4, w.at(kEventMovieAt));
  guest::StoreU32(base, w.at(kPairsAt) + 8, tr::kResultMovie);
  guest::StoreU32(base, w.at(kPairsAt) + 12, w.at(kResultMovieAt));
  SoundManager(base, w, r);
  MenuInput(base, w, r);
  // The save's day (+6100): around 0, 999 and the week's edges, negative
  // now and then; its next pick's name record (+8).
  static constexpr int32_t kDays[] = {0, 1, 6, 7, 998, 999, 1000, 5000, -1, -7, -8};
  const int32_t day = r.Below(2) == 0 ? kDays[r.Below(11)] : int32_t(r.Below(1500));
  guest::StoreU32(base, w.save + 6100, uint32_t(day));
  NameRecord(base, w.save + 8, r);
  // Today's event's data: the image lists (counts 0 to 46, their records in
  // the world, loaded mostly), the images' bits (within the 46), the unlock
  // shown (its table 0 to 3, its entry -1 or in the table), movies queued
  // or not.
  const uint32_t data = w.at(kEventDataAt);
  for (const uint32_t list : {data + 4, data + 192}) {
    const uint32_t count = r.Below(4) == 0 ? 0 : 1 + r.Below(r.Below(2) == 0 ? 4 : 46);
    guest::StoreU32(base, list + 184, count);
    for (uint32_t k = 0; k < count; k++) {
      const uint32_t record = w.at(kRecordsAt) + 16 * r.Below(92);
      guest::StoreU32(base, list + 4 * k, record);
    }
  }
  for (uint32_t k = 0; k < 92; k++) {
    guest::StoreU32(base, w.at(kRecordsAt) + 16 * k, r.Below(8) == 0 ? 0 : 1);
  }
  const uint32_t density = r.Below(4);
  uint32_t bits[2] = {0, 0};
  for (uint32_t k = 0; k < density * 2; k++) {
    const uint32_t image = r.Below(tr::kImages);
    bits[image >> 5] |= 1u << (image & 31);
  }
  guest::StoreU32(base, data + 380, bits[0]);
  guest::StoreU32(base, data + 384, bits[1]);
  guest::StoreU32(base, data + 392, r.Below(4));
  guest::StoreU32(base, data + 396, r.Below(3) == 0 ? 0xFFFFFFFFu : r.Below(kTableEntries));
  guest::StoreU32(base, data + 408, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
  // The save's unlocks just made (their count ShapeSave's), each shown or
  // not, its table 0 to 3 and entry in the table.
  const uint32_t unlocks = w.save + tr::kUnlocks;
  for (uint32_t k = 0; k < 64; k++) {
    guest::StoreU32(base, unlocks + 12 * k, r.Below(3) == 0 ? 1 : 0);
    guest::StoreU32(base, unlocks + 12 * k + 4, r.Below(4));
    guest::StoreU32(base, unlocks + 12 * k + 8, r.Below(kTableEntries));
  }
  // The tables' 56-byte entries: +8 the offset to the entry's next page (0
  // mostly, forward and within the table), +12 a title, +16 an image (-1 or
  // 0 to 45); entry 0's +0 -1 (table 0's end for Progress::CheckUnlocks).
  for (uint32_t e = 0; e < kTableEntries; e++) {
    const uint32_t entry = w.at(kTable0At) + 56 * e;
    guest::StoreU32(base, entry + 8,
                    e + 4 < kTableEntries && r.Below(3) == 0 ? 1 + r.Below(3) : 0);
    guest::StoreU32(base, entry + 12, r.Below(60));
    guest::StoreU32(base, entry + 16, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(tr::kImages));
  }
  guest::StoreU32(base, w.at(kTable0At), 0xFFFFFFFFu);
  // BUILD.TOC's entries for the images (ids 22696 to 22786): their sizes
  // small, unpacked (+8) or stored (+4) only.
  for (uint32_t id = tr::kImageAssetA; id < tr::kImageAssetB + tr::kImages; id++) {
    const uint32_t entry = w.at(kTocAt) + 16 * (id - tr::kImageAssetA);
    const uint32_t size = 16 + 4 * r.Below(256);
    const bool stored = r.Below(4) == 0;
    guest::StoreU32(base, entry + 4, size);
    guest::StoreU32(base, entry + 8, stored ? 0 : size);
  }
  // The stats' counts (the record's +12 to +22, its day block's the same at
  // +24 + 12 to 22): 0 now and then (no percentage), the races 0 now and
  // then too.
  for (const uint32_t counts : {w.save + 48 + 12, w.save + 48 + 24 + 12}) {
    for (uint32_t k = 0; k < 6; k += 2) {
      guest::StoreU16(base, counts + k, uint16_t(r.Below(3) == 0 ? 0 : r.Below(400)));
    }
    guest::StoreU16(base, counts + 10, uint16_t(r.Below(4) == 0 ? 0 : r.Below(800)));
  }
  // The mode's ring (the dispatcher's, at +240): count and start small; no
  // handlers.
  guest::StoreU32(base, w.mode() + tr::kRing + 600, r.Below(40));
  guest::StoreU32(base, w.mode() + tr::kRing + 604, r.Below(60));
  guest::StoreU32(base, w.dispatcher() + 200, 0);
  guest::StoreU32(base, w.mode() + tr::kBlock, w.result());
  // The machines' words: +9604 a state, +9608, +9612, +9616 0 or 1 or any;
  // the shown index 0 or 27 (the only ones the mode's functions store).
  for (const uint32_t machine : {w.result(), w.event()}) {
    guest::StoreU32(base, machine + tr::kMachineState,
                    r.Below(8) == 0 ? uint32_t(r.Next()) : kStates[r.Below(std::size(kStates))]);
    for (const uint32_t field : {tr::kMachineLoaded, tr::kMachineReenter, tr::kMachineDelete}) {
      guest::StoreU32(base, machine + field, r.Below(3) == 0 ? uint32_t(r.Next()) : r.Below(2));
    }
  }
  guest::StoreU32(base, w.result() + tr::kShown, r.Below(2) == 0 ? 0 : tr::kResultIndex);
  // Leaving's: now and then both machines in a leaving state.
  if (r.Below(3) == 0) {
    static constexpr uint32_t kLeft[] = {6, 13, 14, 12, 20};
    for (const uint32_t machine : {w.result(), w.event()}) {
      guest::StoreU32(base, machine + tr::kMachineState, kLeft[r.Below(5)]);
    }
  }
  ShapeSave(base, w, r);
  // The fades' words.
  WriteRandom(base, kFades, kFadesBytes, r);
}

void PointGlobals(uint8_t* base, const World& w, Random& r) {
  guest::StoreU32(base, kSaveTable, w.save);
  guest::StoreU32(base, tr::kResultPages, w.at(kResultPagesAt));
  guest::StoreU32(base, tr::kResultMenu, w.at(kResultMenuAt));
  guest::StoreU32(base, tr::kResultData, w.at(kResultDataAt));
  guest::StoreU32(base, tr::kResultPages + 12, w.at(kEventPagesAt));
  guest::StoreU32(base, tr::kResultPages + 16, w.at(kEventMenuAt));
  guest::StoreU32(base, tr::kResultPages + 20, w.at(kEventDataAt));
  guest::StoreU32(base, ui_movies::kMovieTable, w.at(kPairsAt));
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  // Now and then the pairs not there (UiMovies::Find answers the missing
  // movie, ours).
  guest::StoreU32(base, ui_movies::kMovieTable + 8, r.Below(8) == 0 ? 0 : 2);
  guest::StoreU32(base, kMissingMovie, w.at(r.Below(2) == 0 ? kResultMovieAt : kEventMovieAt));
  guest::StoreU32(base, kMissingText, 0);
  guest::StoreU32(base, kStringTable, w.strings);
  // The data tables (Progress::CheckUnlocks', recompiled, reads them): table
  // 0 and the 40-byte table both empty (their first entries the ends), so
  // it walks no unlocks.
  guest::StoreU32(base, data_tables::kTables, w.at(kTablesAt));
  for (uint32_t table = 0; table < 9; table++) {
    guest::StoreU32(base, w.at(kTablesAt) + 20 + 4 * table, w.at(kTable0At));
  }
  guest::StoreU32(base, archive::kBuildToc, w.at(kTocAt) - 16 * (tr::kImageAssetA + 1));
  guest::StoreU32(base, w.at(kTablesAt) + 56, w.at(kTable40At));
  guest::StoreU32(base, w.at(kTable40At) + 16, 0xFFFFFFFFu);
  // No race event (RaceEvent::Destroy does nothing).
  guest::StoreU32(base, kRaceEvent, 0);
  guest::StoreU32(base, menu_sounds::kSoundManager, w.at(kMenuWorldAt));
  // LoadingScreen::Run and Finish(-1): the current movie 3 (none: they do
  // nothing).
  guest::StoreU32(base, race_loading::kLoadingCurrent, 3);
}

// UiMovie::Run's update (sub_8211D2F8), stood in for: each call's return
// address, stack pointer (from the case's), r3 and r4 kept and answered 0
// or 1 from the case's generator (the update would draw the movie).
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall> g_updates;
Random g_answers;
uint32_t g_case_sp = 0;
bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (function != 0x8211D2F8) {
    return false;
  }
  g_updates.push_back({uint32_t(ctx.lr), g_case_sp - ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_answers.Below(2);
  return true;
}

// A stack range compared, relative to the call's r1: [from, to).
struct Range {
  int32_t from;
  int32_t to;
};

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// Run after the function, before the outputs are read.
using AfterFn = std::function<void(PPCContext&, const World&, Outputs&)>;

struct Spec {
  const char* name;
  int cases;
  GuestFunction recompiled;
  GuestFunction native;
  std::vector<Range> stack;  // compared below r1 (frames without their saves)
  SetupFn setup;
  AfterFn after = nullptr;
  // The loading screen's world shaped (loading_world.h) and compared:
  // LoadingScreen::Run and Finish(-1) on its current movie.
  bool loading = false;
};

// The loading screen's world, made once (Run's cases shape it when their
// spec asks).
const loading_world::MovieWorld* g_loading = nullptr;
void ShapeLoading(uint8_t* base, Random& r) {
  namespace lw = loading_world;
  lw::ShapeMovieWorld(base, *g_loading, r);
  lw::BuildStringTable(base, *g_loading);
  guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
  guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                    r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
  }
}
void TakeLoading(uint8_t* base, Outputs& out) {
  out.Bytes(Read(base, g_loading->movies.address, g_loading->movies.bytes));
  out.Bytes(Read(base, g_loading->texts.address, g_loading->texts.bytes));
  out.Word(guest::LoadU32(base, race_loading::kLoadingShown));
  out.Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
}

// The save data's random parts compared.
void SaveWords(uint8_t* base, const World& w, Outputs& out) {
  out.Bytes(Read(base, w.save, kSaveHeadBytes));
  out.Bytes(Read(base, w.save + kSaveTailAt, kSaveBytes - kSaveTailAt));
}

// The calls recorded on both sides (their adapters hand them to the
// harnesses' recorder first) and answered 1: the UI movies' start,
// Strings::Load, UiMovies::Open and Delete (Delete would free our movie)
// and Career::NextDay (the career's harness checks it). Their address, r3,
// r4 (0 where the callee doesn't read it), r1 by its offset from the case's
// and lr.
std::vector<uint32_t> g_calls;
uint32_t g_record_sp = 0;
// Archive::Load's buffers (the layer's pages the side allocated): compared
// and freed after the case.
std::vector<uint32_t> g_loads;
bool Record(PPCContext& ctx, uint32_t address) {
  if (address == tr::kArchiveLoad) {
    const uint32_t buffer = ctx.r4.u32, record = ctx.r6.u32;
    const bool pages = memory::IsPages(buffer);
    if (pages) {
      g_loads.push_back(buffer);
    }
    g_calls.insert(g_calls.end(),
                   {address, ctx.r3.u32, pages ? 1u : buffer, ctx.r5.u32,
                    memory::IsPages(record) ? 1u : record, ctx.r7.u32, ctx.r8.u32,
                    g_record_sp - ctx.r1.u32, uint32_t(ctx.lr)});
    ctx.r3.u64 = 1;
    return true;
  }
  const bool reads_r4 = address == tr::kStringsLoad || address == tr::kMoviesOpen;
  if (address != tr::kScreenStart && address != tr::kCareerNextDay && !reads_r4 &&
      address != tr::kMoviesDelete) {
    return false;
  }
  g_calls.insert(g_calls.end(), {address, ctx.r3.u32, reads_r4 ? ctx.r4.u32 : 0u,
                                 g_record_sp - ctx.r1.u32, uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

// The layer's pages a case made (blocks the function frees): after the
// run, whether each is still pages (then freed).
std::vector<uint32_t> g_made;
uint32_t MakePages(uint8_t* base, uint32_t bytes, Random& r) {
  const uint32_t block = memory::AllocPages(bytes, 4096);
  WriteRandom(base, block, std::min(bytes, 64u), r);
  g_made.push_back(block);
  return block;
}

// The pointer word at `slot` names a block the side allocated (the layer's
// pages): its first `bytes` compared, freed, the word replaced by 1 (0 when
// none, the word itself when not pages).
void Allocated(uint8_t* base, Outputs& out, uint32_t slot, uint32_t bytes) {
  const uint32_t block = guest::LoadU32(base, slot);
  if (block == 0 || !memory::IsPages(block)) {
    out.Word(block);
    return;
  }
  out.Word(1);
  out.Bytes(Read(base, block, bytes));
  memory::FreePages(block);
  guest::StoreU32(base, slot, 1);
}

// The text records the setters made (stored in the movie's map): their
// words and buffers compared, freed, their map words 0.
void TextRecords(uint8_t* base, uint32_t movie, Outputs& out) {
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
      const loading_world::SavedGlobals loading(h.base);
      MakeWorld(h.base, w, r);
      PointGlobals(h.base, w, r);
      if (spec.loading) {
        ShapeLoading(h.base, r);
      }
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = r.Next();
      t.r4.u64 = r.Next();
      t.r5.u64 = r.Next();
      spec.setup(t, r, w);
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_calls.clear();
      g_record_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = Record;
      g_updates.clear();
      g_answers = Random(r.Next());
      g_case_sp = sp;
      const ui_movies_hooks::UpdateCalleeStandIn stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      if (v == 0) {
        const bool native = ITC_FLAG(native_todays_result);
        ITC_FLAG(native_todays_result) = false;
        spec.recompiled(t, h.base);
        ITC_FLAG(native_todays_result) = native;
      } else {
        spec.native(t, h.base);
      }
      ui_movies_hooks::g_update_callee_stand_in = stand_in;
      hooks::g_start_up_recorder = recorder;
      out[v].Word(uint32_t(g_calls.size()));
      for (const uint32_t word : g_calls) {
        out[v].Word(word);
      }
      for (const uint32_t buffer : g_loads) {
        out[v].Bytes(Read(h.base, buffer, 64));
        memory::FreePages(buffer);
      }
      g_loads.clear();
      out[v].Word(uint32_t(g_updates.size()));
      for (const UpdateCall& call : g_updates) {
        out[v].Word(call.lr);
        out[v].Word(call.r1);
        out[v].Word(uint32_t(call.r3 >> 32));
        out[v].Word(uint32_t(call.r3));
        out[v].Word(uint32_t(call.r4 >> 32));
        out[v].Word(uint32_t(call.r4));
      }
      if (spec.after) {
        spec.after(t, w, out[v]);
      }
      out[v].Word(uint32_t(g_made.size()));
      for (const uint32_t block : g_made) {
        const bool pages = memory::IsPages(block);
        out[v].Word(pages ? 1 : 0);
        if (pages) {
          memory::FreePages(block);
        }
      }
      g_made.clear();
      TextRecords(h.base, w.at(kResultMovieAt), out[v]);
      TextRecords(h.base, w.at(kEventMovieAt), out[v]);
      SaveWords(h.base, w, out[v]);
      if (spec.loading) {
        TakeLoading(h.base, out[v]);
      }
      for (const Global& g : kGlobals) {
        out[v].Bytes(Read(h.base, g.address, g.bytes));
      }
      out[v].Bytes(Read(h.base, w.block, kBlockBytes));
      for (const Range& range : spec.stack) {
        out[v].Bytes(Read(h.base, sp + range.from, uint32_t(range.to - range.from)));
      }
      out[v].Bytes(Read(h.base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      for (size_t i = 0; i < saved.size(); i++) {
        Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
      loading.Restore();
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- The mode ---

// r3 the answer, r4 the mode.
void Delegate(PPCContext& t, Random&, const World& w) {
  t.r3.u64 = w.at(kAnswerAt);
  t.r4.u64 = w.mode();
}

// The machine empty or holding random transitions with keys of small
// numbers, some the Init's (Add replaces those).
void InitMachine(PPCContext& t, Random& r, uint8_t* base, uint32_t machine) {
  namespace gm = game_modes;
  if (r.Below(2) == 0) {
    gm::StateMachineConstruct(base, machine);
    guest::StoreU32(base, machine + gm::kTransitionCount, 0);
  } else {
    const uint32_t count = 8 + r.Below(160);
    guest::StoreU32(base, machine + gm::kTransitionCount, count);
    for (uint32_t i = 0; i < gm::kTransitions; i++) {
      const uint32_t at = machine + gm::kTransitionBytes * i;
      guest::StoreU32(base, at, r.Below(12));
      guest::StoreU32(base, at + 4, r.Below(20));
      guest::StoreU32(base, at + 8, r.Below(8) == 0 ? 1 : 0);
      guest::StoreU32(base, at + 12, 0);
    }
  }
  t.r3.u64 = machine;
}

void Mode(Context& h, const World& w) {
  Run(h, w, {"TodaysResult::InitStates (sub_8225B358)", 24, __imp__sub_8225B358,
             hooks::TodaysResultInitStates, {{-976, -48}},
             [](PPCContext& t, Random& r, const World& w) {
               InitMachine(t, r, w.base, w.result());
             }});
  Run(h, w, {"TodaysEvent::InitStates (sub_8225CF30)", 24, __imp__sub_8225CF30,
             hooks::TodaysEventInitStates, {{-976, -48}},
             [](PPCContext& t, Random& r, const World& w) {
               InitMachine(t, r, w.base, w.event());
             }});
  Run(h, w, {"ModeType9::FindShown (sub_8225D658)", 60, __imp__sub_8225D658,
             hooks::ModeType9FindShown, {},
             [](PPCContext& t, Random& r, const World& w) {
               t.r3.u64 = w.result() | (uint64_t(r.Below(2)) << 32);
             }});
  Run(h, w, {"TodaysEvent::Kind (sub_8225BA48)", 100, __imp__sub_8225BA48,
             hooks::TodaysEventKind, {{-96, -8}}, [](PPCContext&, Random&, const World&) {}});
  Run(h, w, {"ModeType9::WaitForLoads (sub_8225D918)", 150, __imp__sub_8225D918,
             hooks::ModeType9WaitForLoads, {{-128, -40}}, Delegate, nullptr, true});
  Run(h, w, {"ModeType9::NextMode (sub_8225DC38)", 300, __imp__sub_8225DC38,
             hooks::ModeType9NextMode, {{-144, -56}}, Delegate});
  Run(h, w, {"ModeType9::LeaveScreen (sub_8225DB70)", 60, __imp__sub_8225DB70,
             hooks::ModeType9LeaveScreen, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType9::WaitForShown (sub_8225DA68)", 100, __imp__sub_8225DA68,
             hooks::ModeType9WaitForShown, {{-112, -16}}, Delegate});
  Run(h, w, {"ModeType9::Leaving (sub_8225DE78)", 150, __imp__sub_8225DE78,
             hooks::ModeType9Leaving, {{-128, -32}}, Delegate});
  Run(h, w, {"ModeType9::IsLoaded (sub_8225D6A8)", 40, __imp__sub_8225D6A8,
             hooks::ModeType9IsLoaded, {},
             [](PPCContext& t, Random&, const World& w) { t.r3.u64 = w.mode() + 32; }});
}

// --- Today's result ---

// r3 the machine (now and then with high bits), r4 the dispatcher.
void ResultMachine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.result() | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = w.dispatcher();
}
// r3 the dispatcher.
void Dispatcher(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.dispatcher(); }
// r3 the started word (the page object's +24): 0 mostly, 1 or any.
void ResultStarted(PPCContext& t, Random& r, const World& w) {
  const uint32_t at = w.at(kResultPagesAt) + 24;
  guest::StoreU32(w.base, at, r.Below(3) == 0 ? 1 : r.Below(5) == 0 ? uint32_t(r.Next()) : 0);
  t.r3.u64 = at;
}
void Nothing(PPCContext&, Random&, const World&) {}

void Result(Context& h, const World& w) {
  // Create: the three blocks VirtualAlloc made compared by content.
  Run(h, w, {"TodaysResult::Create (sub_8225A7A0)", 60, __imp__sub_8225A7A0,
             hooks::TodaysResultCreate, {{-112, -24}}, ResultMachine,
             [](PPCContext& t, const World& w, Outputs& out) {
               // r3 the data's address (the last VirtualAlloc's).
               out.Word(t.r3.u32 == guest::LoadU32(w.base, tr::kResultData) ? 1 : 0);
               t.r3.u64 = 0;
               Allocated(w.base, out, tr::kResultPages, 28);
               Allocated(w.base, out, tr::kResultMenu, 68);
               Allocated(w.base, out, tr::kResultData, 68);
             }});
  // Destroy: the three blocks the layer's pages (whether each was freed
  // compared), the data's +4 0 or not.
  Run(h, w, {"TodaysResult::Destroy (sub_8225A8A8)", 60, __imp__sub_8225A8A8,
             hooks::TodaysResultDestroy, {{-128, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               ResultMachine(t, r, w);
               const uint32_t data = MakePages(base, 68, r);
               guest::StoreU32(base, data + 4, r.Below(2) == 0 ? 0 : uint32_t(r.Next()));
               guest::StoreU32(base, tr::kResultPages, MakePages(base, 28, r));
               guest::StoreU32(base, tr::kResultMenu, MakePages(base, 68, r));
               guest::StoreU32(base, tr::kResultData, data);
             }});
  Run(h, w, {"TodaysResult::OpenMovie (sub_8225A510)", 20, __imp__sub_8225A510,
             hooks::TodaysResultOpenMovie, {}, ResultMachine});
  Run(h, w, {"TodaysResult::WaitForMovie (sub_8225A930)", 100, __imp__sub_8225A930,
             hooks::TodaysResultWaitForMovie, {{-96, -16}}, ResultMachine});
  // Start: only its own frame (ReadDay's, SetUpMovie's and SetTexts' have
  // their own cases).
  Run(h, w, {"TodaysResult::Start (sub_8225B300; sub_8225A520, sub_8225B258, sub_8225ADE0)",
             60, __imp__sub_8225B300, hooks::TodaysResultStart, {{-96, -8}}, ResultMachine});
  Run(h, w, {"TodaysResult::ReadDay (sub_8225A520)", 150, __imp__sub_8225A520,
             hooks::TodaysResultReadDay, {{-11040, -48}}, Nothing});
  Run(h, w, {"TodaysResult::SetUpMovie (sub_8225B258)", 60, __imp__sub_8225B258,
             hooks::TodaysResultSetUpMovie, {{-128, -32}}, Nothing});
  // SetTexts: without the setters' parameter area (+48 to +79) and its
  // saves.
  Run(h, w, {"TodaysResult::SetTexts (sub_8225ADE0)", 100, __imp__sub_8225ADE0,
             hooks::TodaysResultSetTexts, {{-112, -64}, {-32, -24}}, Nothing});
  // Draw: the page -1, 0 or 1 (the table's two); only its own frame.
  Run(h, w, {"TodaysResult::Draw (sub_8225B0C8)", 150, __imp__sub_8225B0C8,
             hooks::TodaysResultDraw, {{-96, -8}},
             [](PPCContext& t, Random& r, const World& w) {
               ResultMachine(t, r, w);
               const uint32_t pick = r.Below(3);
               guest::StoreU32(w.base, w.at(kResultPagesAt), pick == 2 ? 0xFFFFFFFFu : pick);
             }});
  // Update: the running transition -1, another negative, or 0 to 2, the
  // queued one -1 or small, the started word 0 or not, the page 1 or not;
  // only its own frame.
  Run(h, w, {"TodaysResult::Update (sub_8225B128)", 300, __imp__sub_8225B128,
             hooks::TodaysResultUpdate, {{-128, -16}},
             [](PPCContext&, Random& r, const World& w) {
               uint8_t* base = w.base;
               const uint32_t pages = w.at(kResultPagesAt);
               const uint32_t pick = r.Below(4);
               guest::StoreU32(base, pages + 12,
                               pick == 0   ? 0xFFFFFFFFu
                               : pick == 1 ? uint32_t(-int32_t(2 + r.Below(5)))
                                           : r.Below(3));
               guest::StoreU32(base, pages + 16, r.Below(2) == 0 ? 0xFFFFFFFFu : r.Below(3));
               guest::StoreU32(base, pages + 24, r.Below(3) == 0 ? 1 : 0);
               guest::StoreU32(base, pages, r.Below(3) == 0 ? 1 : r.Below(2));
             }});
  Run(h, w, {"TodaysResult::WaitPage (sub_8225A9B0)", 300, __imp__sub_8225A9B0,
             hooks::TodaysResultWaitPage, {{-96, -8}}, Dispatcher});
  // LeavePage: the save's movie -1 or not; QueueMovie's frame below without
  // its lr save.
  Run(h, w, {"TodaysResult::LeavePage (sub_8225AB30; sub_8225A720)", 150, __imp__sub_8225AB30,
             hooks::TodaysResultLeavePage, {{-96, -8}, {-192, -104}}, Dispatcher});
  Run(h, w, {"TodaysResult::QueueMovie (sub_8225A720)", 60, __imp__sub_8225A720,
             hooks::TodaysResultQueueMovie, {{-96, -8}}, Nothing});
  struct Case {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Case transitions[] = {
      {"TodaysResult::Intro (sub_8225ABD8)", __imp__sub_8225ABD8, hooks::TodaysResultIntro},
      {"TodaysResult::ShowLoop (sub_8225AC88)", __imp__sub_8225AC88,
       hooks::TodaysResultShowLoop},
      {"TodaysResult::FadeOut (sub_8225AD20)", __imp__sub_8225AD20, hooks::TodaysResultFadeOut},
  };
  for (const Case& c : transitions) {
    Run(h, w, {c.name, 150, c.recompiled, c.native, {{-112, -24}}, ResultStarted});
  }
  Run(h, w, {"TodaysResult::Unload (sub_8225AA20)", 60, __imp__sub_8225AA20,
             hooks::TodaysResultUnload, {{-112, -16}}, ResultMachine});
  Run(h, w, {"TodaysResult::Unloaded (sub_8225A9A0)", 20, __imp__sub_8225A9A0,
             hooks::TodaysResultUnloaded, {}, ResultMachine});
  Run(h, w, {"TodaysResult::Leave (sub_8225AA88)", 60, __imp__sub_8225AA88,
             hooks::TodaysResultLeave, {{-112, -16}}, ResultMachine});
}

// --- Today's event ---

// r3 the machine (now and then with high bits), r4 the dispatcher.
void EventMachine(PPCContext& t, Random& r, const World& w) {
  t.r3.u64 = w.event() | (uint64_t(r.Below(2)) << 32);
  t.r4.u64 = w.dispatcher();
}
// r3 the data.
void EventData(PPCContext& t, Random&, const World& w) { t.r3.u64 = w.at(kEventDataAt); }

// The image lists' records the layer's pages (each record's +8 and +12
// pages too, or 0 now and then), for FreeImages and Destroy.
void PagesLists(Random& r, const World& w) {
  uint8_t* base = w.base;
  const uint32_t data = w.at(kEventDataAt);
  for (const uint32_t list : {data + 4, data + 192}) {
    const uint32_t count = r.Below(3) == 0 ? 0 : 1 + r.Below(6);
    guest::StoreU32(base, list + 184, count);
    for (uint32_t k = 0; k < count; k++) {
      const uint32_t record = MakePages(base, 16, r);
      for (const uint32_t field : {8u, 12u}) {
        guest::StoreU32(base, record + field, r.Below(6) == 0 ? 0 : MakePages(base, 24, r));
      }
      guest::StoreU32(base, list + 4 * k, record);
    }
  }
}

// The records the side allocated in the image lists (the layer's pages):
// compared, freed, their list words 1.
void ImageRecords(PPCContext&, const World& w, Outputs& out) {
  uint8_t* base = w.base;
  const uint32_t data = w.at(kEventDataAt);
  for (const uint32_t list : {data + 4, data + 192}) {
    const uint32_t count = guest::LoadU32(base, list + 184);
    for (uint32_t k = 0; k < count && k < 46; k++) {
      const uint32_t record = guest::LoadU32(base, list + 4 * k);
      if (memory::IsPages(record)) {
        out.Bytes(Read(base, record, 16));
        memory::FreePages(record);
        guest::StoreU32(base, list + 4 * k, 1);
      }
    }
  }
}

void EmptyLists(PPCContext&, Random&, const World& w) {
  guest::StoreU32(w.base, w.at(kEventDataAt) + 188, 0);
  guest::StoreU32(w.base, w.at(kEventDataAt) + 376, 0);
}

void Event(Context& h, const World& w) {
  // Create: the three blocks VirtualAlloc made compared by content (the
  // data's 412 bytes used); Kind 0 or not by the save.
  Run(h, w, {"TodaysEvent::Create (sub_8225BE50; sub_8225BAB8)", 150, __imp__sub_8225BE50,
             hooks::TodaysEventCreate, {{-112, -32}}, EventMachine,
             [](PPCContext& t, const World& w, Outputs& out) {
               out.Word(t.r3.u32 == guest::LoadU32(w.base, tr::kEventData) ? 2 : t.r3.u32);
               t.r3.u64 = 0;
               Allocated(w.base, out, tr::kEventPages, 28);
               Allocated(w.base, out, tr::kEventMenu, 68);
               Allocated(w.base, out, tr::kEventData, 412);
             }});
  Run(h, w, {"TodaysEvent::MarkImages (sub_8225BAB8)", 150, __imp__sub_8225BAB8,
             hooks::TodaysEventMarkImages, {{-160, -56}}, Nothing});
  // OpenMovie, LoadImages, LoadImagePair: the records and buffers they make
  // compared and freed; only their own frames.
  // (The lists empty, as Create leaves them: the images' 46 bits fill them
  // at most.)
  Run(h, w, {"TodaysEvent::OpenMovie (sub_8225C130; sub_8225C008, sub_8225BD10)", 60,
             __imp__sub_8225C130, hooks::TodaysEventOpenMovie, {{-96, -8}}, EmptyLists,
             ImageRecords});
  Run(h, w, {"TodaysEvent::LoadImages (sub_8225C008; sub_8225BD10)", 100, __imp__sub_8225C008,
             hooks::TodaysEventLoadImages, {{-112, -24}}, EmptyLists, ImageRecords});
  Run(h, w, {"TodaysEvent::LoadImagePair (sub_8225BD10)", 100, __imp__sub_8225BD10,
             hooks::TodaysEventLoadImagePair, {{-144, -56}},
             [](PPCContext& t, Random& r, const World& w) {
               // The lists' counts at most 45 (room for one more).
               uint8_t* base = w.base;
               for (const uint32_t count : {w.at(kEventDataAt) + 188, w.at(kEventDataAt) + 376}) {
                 guest::StoreU32(base, count, r.Below(46));
               }
               t.r3.u64 = w.at(kEventDataAt);
               t.r4.u64 = r.Below(tr::kImages);
             },
             ImageRecords});
  Run(h, w, {"TodaysEvent::WaitForMovie (sub_8225C160)", 150, __imp__sub_8225C160,
             hooks::TodaysEventWaitForMovie, {{-96, -16}}, EventMachine});
  Run(h, w, {"TodaysEvent::ImagesLoadedA (sub_8225C0A0)", 100, __imp__sub_8225C0A0,
             hooks::TodaysEventImagesLoadedA, {}, EventData});
  Run(h, w, {"TodaysEvent::ImagesLoadedB (sub_8225C0E8)", 100, __imp__sub_8225C0E8,
             hooks::TodaysEventImagesLoadedB, {}, EventData});
  // Start: only its own frame (SetUpTexts' and NextUnlockEntry's have their
  // own cases).
  Run(h, w, {"TodaysEvent::Start (sub_8225C990; sub_8225C648, sub_8225BB70)", 150,
             __imp__sub_8225C990, hooks::TodaysEventStart, {{-112, -16}}, EventMachine});
  // SetUpTexts: without SetText's parameter area (+48 to +79, where its
  // unset varargs land: the user's call of 2026-10-30) and its saves.
  Run(h, w, {"TodaysEvent::SetUpTexts (sub_8225C648)", 100, __imp__sub_8225C648,
             hooks::TodaysEventSetUpTexts, {{-128, -80}, {-48, -40}}, Nothing});
  Run(h, w, {"TodaysEvent::NextUnlockEntry (sub_8225BB70)", 200, __imp__sub_8225BB70,
             hooks::TodaysEventNextUnlockEntry, {{-96, -16}}, Nothing});
  Run(h, w, {"TodaysEvent::Unload (sub_8225C1F8)", 100, __imp__sub_8225C1F8,
             hooks::TodaysEventUnload, {{-128, -32}}, EventMachine});
  Run(h, w, {"TodaysEvent::Unloaded (sub_8225C278)", 60, __imp__sub_8225C278,
             hooks::TodaysEventUnloaded, {{-144, -48}}, EventMachine});
  Run(h, w, {"TodaysEvent::Leave (sub_8225C348)", 100, __imp__sub_8225C348,
             hooks::TodaysEventLeave, {{-112, -24}}, EventMachine});
  // Destroy and FreeImages: the lists' records and their images the layer's
  // pages (whether each was freed compared).
  Run(h, w, {"TodaysEvent::Destroy (sub_8225BF78; sub_8225BC60)", 100, __imp__sub_8225BF78,
             hooks::TodaysEventDestroy, {{-128, -40}},
             [](PPCContext& t, Random& r, const World& w) {
               uint8_t* base = w.base;
               EventMachine(t, r, w);
               PagesLists(r, w);
               const uint32_t data = MakePages(base, 412, r);
               Write(base, data, Read(base, w.at(kEventDataAt), 412).data(), 412);
               guest::StoreU32(base, tr::kEventPages, MakePages(base, 28, r));
               guest::StoreU32(base, tr::kEventMenu, MakePages(base, 68, r));
               guest::StoreU32(base, tr::kEventData, data);
             }});
  Run(h, w, {"TodaysEvent::FreeImages (sub_8225BC60)", 100, __imp__sub_8225BC60,
             hooks::TodaysEventFreeImages, {{-112, -32}},
             [](PPCContext& t, Random& r, const World& w) {
               PagesLists(r, w);
               EventData(t, r, w);
             }});
}

// --- ModeType9::Init: the mode random words (its dispatcher's count 0 to
// 7), the UI movies' start and Strings::Load recorded (their adapters'
// recorder), LoadingScreen::Show over the loading screen's world
// (loading_world.h, a made-up time for the tip), the screen fades' words
// saved and put back. Compared: the recorded calls, the mode, the block
// VirtualAlloc made (compared by content and freed, its addresses in it and
// in the mode made offsets), the save data's words Kind reads, the loading
// world, the fades, the frame's +80 to +87 (the machines' Inits' frames
// below hold the machines' addresses) and r3. ---

// The words of [at, at + bytes) that point into [lo, lo + span) made their
// offsets with a mark (the block VirtualAlloc made differs by side).
void NormalizePointers(uint8_t* base, uint32_t at, uint32_t bytes, uint32_t lo, uint32_t span) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, at + i);
    if (word >= lo && word < lo + span) {
      guest::StoreU32(base, at + i, 0xB10C0000u ^ (word - lo));
    }
  }
}

void Init(Context& h, const World& w) {
  namespace lw = loading_world;
  uint8_t* const base = h.base;
  const lw::MovieWorld& movies = *g_loading;
  Results results("ModeType9::Init (sub_8225D6D0)");
  for (int c = 0; c < 12; c++) {
    const uint64_t seed = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed);
      std::vector<std::vector<uint8_t>> saved;
      for (const Global& g : kGlobals) {
        saved.push_back(Read(base, g.address, g.bytes));
      }
      const lw::SavedGlobals loading(base);
      lw::ShapeMovieWorld(base, movies, r);
      lw::BuildStringTable(base, movies);
      guest::StoreU32(base, race_loading::kLoadingCurrent, r.Below(3));
      guest::StoreU32(base, race_loading::kLoadingShown, uint32_t(r.Next()));
      for (uint32_t i = 0; i < 3; i++) {
        guest::StoreU32(base, race_loading::kLoadingAnimations + 4 * i,
                        r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
      }
      WriteRandom(base, w.block, kBlockBytes, r);
      ShapeSave(base, w, r);
      WriteRandom(base, kFades, kFadesBytes, r);
      guest::StoreU32(base, kSaveTable, w.save);
      guest::StoreU32(base, w.dispatcher() + 200, r.Below(8));
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(base, sp - kStackBytes, kStackBytes + kCallerBytes, r);
      t.r3.u64 = w.mode();
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
      g_calls.clear();
      g_record_sp = sp;
      const hooks::StartUpRecorder recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = Record;
      if (v == 0) {
        const bool native = ITC_FLAG(native_todays_result);
        ITC_FLAG(native_todays_result) = false;
        __imp__sub_8225D6D0(t, base);
        ITC_FLAG(native_todays_result) = native;
      } else {
        hooks::ModeType9Init(t, base);
      }
      hooks::g_start_up_recorder = recorder;
      time_hooks::ClearFakeNow();
      out[v].Word(uint32_t(g_calls.size()));
      for (const uint32_t word : g_calls) {
        out[v].Word(word);
      }
      const uint32_t block = guest::LoadU32(base, w.mode() + tr::kBlock);
      const bool made = memory::IsPages(block);
      out[v].Word(made ? 1 : 0);
      if (made) {
        NormalizePointers(base, block, tr::kBlockBytes, block, tr::kBlockBytes);
        out[v].Bytes(Read(base, block, tr::kBlockBytes));
        NormalizePointers(base, w.mode(), 1024, block, tr::kBlockBytes);
        memory::FreePages(block);
        guest::StoreU32(base, w.mode() + tr::kBlock, 1);
      }
      SaveWords(base, w, out[v]);
      out[v].Bytes(Read(base, w.block, kBlockBytes));
      out[v].Bytes(Read(base, movies.movies.address, movies.movies.bytes));
      out[v].Bytes(Read(base, movies.texts.address, movies.texts.bytes));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingShown));
      out[v].Word(guest::LoadU32(base, race_loading::kLoadingCurrent));
      out[v].Bytes(Read(base, kFades, kFadesBytes));
      out[v].Bytes(Read(base, sp - 112 + 80, 8));
      out[v].Bytes(Read(base, sp, kCallerBytes));
      out[v].Word(t.r1.u32);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      loading.Restore();
      for (size_t i = 0; i < saved.size(); i++) {
        Write(base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void TodaysResult(Context& h) {
  const World w(h.base);
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: todays_result: no block from the allocation layer");
    return;
  }
  const loading_world::MovieWorld loading;
  if (!loading.ok()) {
    ITC_LOG_ERROR("harness: todays_result: no loading world");
    return;
  }
  g_loading = &loading;
  if (hooks::NativeHeapActive()) {
    Mode(h, w);
    Init(h, w);
    Result(h, w);
    Event(h, w);
  }
  g_loading = nullptr;
}

}  // namespace harness
