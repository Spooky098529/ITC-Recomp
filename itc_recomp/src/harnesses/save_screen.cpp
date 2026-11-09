// The save screen (src/save_screen/, docs/functions/save-screen.md) against
// the recompiled functions. Each case fills a world of its own (blocks from
// the allocation layer) with random words from the case's seed: the page
// block, the menu list with its players' mappers (as the race screens'
// harness lays a menu list's), the screen's data, a mode with its message
// ring, UI movie 44 (as the front end's screen helpers' harness lays it:
// its named animations over a tree of nodes, its texts and the string
// table of letters only, no argument codes) and the movie table's pair
// naming it, the save system's interface (its slots and their enumerated
// saves, as the save interface's harness lays them), player 0's save data
// and the save buffers' header with its game data's buffer; points the
// globals into it (put back after); shapes the fields each function tests;
// runs the original (with --native_save_screen off, so what it calls of the
// screen's is the original too) or the register steps (hooks/save_screen.h);
// and compares the blocks, the globals, the function's frame without its
// register saves, the caller's area above r1, r1, r3 and UiMovie::Run's
// update calls (stood in for, recorded and answered), all exactly (nothing
// here is a float).

#include "harness.h"

#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"
#include "hooks/app_start_up.h"
#include "hooks/save_screen.h"
#include "game_modes/game_modes.h"
#include "hooks/ui_movies.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "race_screens/race_screens.h"
#include "save_screen/save_screen.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <algorithm>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_save_screen);

namespace harness {

namespace {

namespace ss = save_screen;
namespace rs = race_screens;

constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it

// The getter sub_82090770 (r3 = +60): the sound manager's PlayCue2D.
constexpr uint32_t kGet60 = 0x82090770;
constexpr uint32_t kMissingText = 0x826C5CD8;   // TextMap::Find's answer when not found
constexpr uint32_t kMissingMovie = 0x826C5CDC;  // UiMovies::Find's answer when not found
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTagTxt = 0x54585400;
constexpr uint32_t kTagSce = 0x53434500;
constexpr uint32_t kSaveInterfaceGlobal = 0x826BDC90;
constexpr uint32_t kSaveBuffers = 0x828161F0;
constexpr uint32_t kSaveDataSlots = 0x826C5B58;  // the players' save data (SaveData::Of)
constexpr uint32_t kGameDataBytes = 0x1F978;

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

// 0 to `n` - 1 from the high bits of the next word (Random::Below takes
// the low bits, which consecutive draws of the xorshift share: back-to-back
// picks would hardly ever reach some combinations).
uint32_t Pick(Random& r, uint32_t n) { return uint32_t((r.Next() >> 32) % n); }

// A case's seed (splitmix64's finalizer): the harness's seeds are its
// generator's consecutive states, and a generator seeded with one state
// draws the next case's sequence shifted by one word.
uint64_t MixSeed(uint64_t x) {
  x = (x ^ (x >> 30)) * 0xBF58476D1CE4E5B9ull;
  x = (x ^ (x >> 27)) * 0x94D049BB133111EBull;
  return x ^ (x >> 31);
}

// A small word: 0 to `n` - 1 mostly, now and then any.
uint32_t Small(Random& r, uint32_t n) {
  return Pick(r, 8) == 0 ? uint32_t(r.Next()) : Pick(r, n);
}
// One of `values`, now and then any word.
uint32_t OneOf(Random& r, std::initializer_list<uint32_t> values) {
  const std::vector<uint32_t> list(values);
  return Pick(r, 8) == 0 ? uint32_t(r.Next()) : list[Pick(r, uint32_t(list.size()))];
}
uint32_t Around(Random& r, int32_t lo, int32_t hi) {
  return uint32_t(lo + int32_t(Pick(r, uint32_t(hi - lo + 1))));
}

// The screen's block: the page block (+0), the menu list (+0x40), the data
// (+0x100; the steps' started word at +0x180), the mode (+0x200, its ring
// at +204), the menu world (+0x600: four mappers of 0x200, their vtables
// +0x800, the list's item block +0x900, the sound manager +0xB00 and its
// vtable +0xB80).
constexpr uint32_t kPagesAt = 0x0, kListAt = 0x40, kDataAt = 0x100, kStepAt = 0x180;
constexpr uint32_t kModeAt = 0x200, kMenuWorldAt = 0x600, kScreenBytes = 0x1400;

// Movie 44 (as the front end's screen helpers' harness lays its movies).
constexpr uint32_t kMovieHeaderBytes = 0xA100;
constexpr uint32_t kTreeNodeBytes = 0x400;
constexpr uint32_t kSubScenes = 16, kLeaves = 32, kAnimations = 8;
constexpr uint32_t kRootAt = kMovieHeaderBytes;
constexpr uint32_t kSubScenesAt = kRootAt + kTreeNodeBytes;
constexpr uint32_t kLeavesAt = kSubScenesAt + kSubScenes * kTreeNodeBytes;
constexpr uint32_t kAnimationsAt = kLeavesAt + kLeaves * kTreeNodeBytes;
constexpr uint32_t kPairsAt = kAnimationsAt + kAnimations * 64;
constexpr uint32_t kTexturesAt = kPairsAt + 0x40;
constexpr uint32_t kMovieWorldBytes = kTexturesAt + 0x100;
constexpr uint32_t kTexts = 8;
constexpr uint32_t kTextBuffersAt = 0x1000;
constexpr uint32_t kTextWorldBytes = kTextBuffersAt + kTexts * 0x1000;
constexpr uint32_t kStringWorldBytes = 0x10000;

// The save system's interface (as the save interface's harness lays it):
// the interface (+0), its +36 interface (+0x40), the devices (+0x100), the
// users (+0x200), six 1,000-byte slots (+0x800: the users' count, 0 to 5,
// is a slot too) and four enumerated saves of 308 bytes (+0x2000).
constexpr uint32_t kSaveObject36 = 0x40, kSaveDevices = 0x100, kSaveUsers = 0x200;
constexpr uint32_t kSaveSlots = 0x800, kSaveRecords = 0x2000, kSaveWorldBytes = 0x2800;

constexpr uint32_t kReplayBlockBytes = 0x13D000;
constexpr uint32_t kSaveData1Bytes = 0x3B000;
uint8_t* base_of_world = nullptr;  // the guest base, for the World's set-up

struct World {
  World()
      : screen(kScreenBytes),
        movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes),
        save(kSaveWorldBytes),
        save_data(kGameDataBytes),
        save_data1(kSaveData1Bytes),
        buffers(0x1000),
        game_data(kGameDataBytes),
        profile(0x400),
        kept(kReplayBlockBytes),
        replay(kReplayBlockBytes) {
    if (replay.address != 0) {
      Random r(0x5265706C6179ull);  // "Replay"
      for (uint32_t i = 0; i < replay.bytes; i += 4) {
        guest::StoreU32(base_of_world, replay.address + i, uint32_t(r.Next()));
      }
    }
  }
  bool ok() const {
    return screen.address && movies.address && texts.address && strings.address &&
           save.address && save_data.address && save_data1.address && buffers.address &&
           game_data.address &&
           profile.address && kept.address && replay.address;
  }
  uint32_t at(uint32_t offset) const { return screen.address + offset; }
  uint32_t pages() const { return at(kPagesAt); }
  uint32_t list() const { return at(kListAt); }
  uint32_t data() const { return at(kDataAt); }
  uint32_t step() const { return at(kStepAt); }
  uint32_t mode() const { return at(kModeAt); }
  uint32_t movie() const { return movies.address; }
  uint32_t root() const { return movies.address + kRootAt; }
  uint32_t sub(uint32_t k) const { return movies.address + kSubScenesAt + kTreeNodeBytes * k; }
  uint32_t leaf(uint32_t k) const { return movies.address + kLeavesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  Block screen;
  Block movies;
  Block texts;
  Block strings;
  Block save;
  Block save_data;   // player 0's ([0x826C5B58]; 2 and 3 too)
  // Player 1's ([0x826C5B5C]): the kind 6 pages copy 0x1D52C bytes between
  // its two halves.
  Block save_data1;
  // The save buffers' header ([0x828161F0]: +0 the game data, +4 the
  // replay, +12 the records at +0x100, +16 the list entries at +0x200, +20
  // their count).
  Block buffers;
  Block game_data;  // its +0
  // The profile reader ([0x826C6AD0], +4 the results at +0x100).
  Block profile;
  // The kept replay ([0x826C5B50]) and the replay at the header's +4
  // (Replay::CopyFrom's), random words once.
  Block kept;
  Block replay;
};

const std::vector<std::pair<uint32_t, uint32_t>> kSavedGlobals = {
    {ss::kPages, 12},  // the page block, the menu list, the data
    {ss::kModeType, 4},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {kStringTable, 4},
    {kSaveInterfaceGlobal, 4},
    {kSaveBuffers, 4},
    {kSaveDataSlots, 16},
    {rs::kMapperEntries, 32},
    {rs::kControllerEntries, 16},
    {ss::kProfileRead, 4},
    {ss::kProfileWanted, 4},
    {ss::kProfileReader, 4},
    {ss::kKeptReplay, 4},
};

// The string table: 18 files' pointers to one container of entries enough
// for the widest file (the bounds at 0x82007E18), each one of 8 strings of
// letters (no argument codes), as the front end's helpers' harness builds it.
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
    ITC_LOG_ERROR("harness: save_screen: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x5361766553637265ull);  // "SaveScre"
  const uint32_t text = container + 4 + 4 * span;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t length = Pick(r, 30);
    for (uint32_t c = 0; c < length; c++) {
      guest::StoreU16(base, text + 64 * k + 2 * c, uint16_t('A' + Pick(r, 26)));
    }
    guest::StoreU16(base, text + 64 * k + 2 * length, 0);
  }
  for (uint32_t i = 0; i < span; i++) {
    guest::StoreU32(base, container + 4 + 4 * i,
                    __builtin_bswap32(text + 64 * Pick(r, 8) - container));
  }
}

void ShapeMovieWorld(uint8_t* base, const World& w, Random& r) {
  WriteRandom(base, w.movies.address, w.movies.bytes, r);
  WriteRandom(base, w.texts.address, w.texts.bytes, r);
  const uint32_t movie = w.movie();
  ui_movies::InitAnimationLists(base, movie);
  for (const uint32_t field : {16u, 20u, 24u}) {
    guest::StoreU32(base, movie + field, w.root());
  }
  guest::StoreU32(base, movie + 28, w.movies.address + kTexturesAt);
  for (uint32_t k = 0; k < 64; k++) {
    const uint32_t named = movie + 176 + 36 * k;
    guest::StoreU32(base, named + 28, w.root());
    guest::StoreU32(base, named + 32, w.animation(Pick(r, kAnimations)));
  }
  for (uint32_t k = 0; k < kTexts; k++) {
    const uint32_t record = w.texts.address + 32 * k;
    const uint32_t buffer = w.texts.address + kTextBuffersAt + 0x1000 * k;
    guest::StoreU32(base, record, buffer);
    guest::StoreU32(base, record + 20, Pick(r, 5) - 1);
    const uint32_t length = Pick(r, 60);
    for (uint32_t c = 0; c < length; c++) {
      guest::StoreU16(base, buffer + 2 * c, Pick(r, 6) == 0 ? 0xFFD1 : uint16_t('a' + Pick(r, 26)));
    }
    guest::StoreU16(base, buffer + 2 * length, 0);
    if (k + 1 < kTexts) {
      guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k, k + 1);
      guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k + 4, record);
    }
  }
  guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, kTexts - 1);
  guest::StoreU32(base, movie + ui_movies::kMovieTextCount, kTexts);
  guest::StoreU32(base, kMissingText, w.texts.address + 32 * (kTexts - 1));
  for (uint32_t k = 0; k < kAnimations; k++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, w.animation(k) + i, 0);
    }
    guest::StoreFloat(base, w.animation(k) + 12, float(Pick(r, 100)) / 4.0f);
  }
  constexpr uint32_t kEntries = (kTreeNodeBytes - 128) / 8;
  guest::StoreU32(base, w.root(), kTagSce);
  guest::StoreU32(base, w.root() + 100, 0);
  guest::StoreU32(base, w.root() + 104, Pick(r, 5));
  guest::StoreU32(base, w.root() + 108, Pick(r, 4));
  guest::StoreU32(base, w.root() + 116, 60);
  for (uint32_t i = 0; i < kEntries; i++) {
    guest::StoreU32(base, w.root() + 128 + 8 * i,
                    i < 60 ? w.sub(Pick(r, kSubScenes)) : w.animation(Pick(r, kAnimations)));
  }
  for (uint32_t k = 0; k < kSubScenes; k++) {
    const uint32_t sub = w.sub(k);
    guest::StoreU32(base, sub, kTagSce);
    guest::StoreU32(base, sub + 100, Pick(r, 16));
    guest::StoreU32(base, sub + 104, Pick(r, 7));
    guest::StoreU32(base, sub + 108, Pick(r, 4));
    guest::StoreU32(base, sub + 116, 64);
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, sub + 128 + 8 * i,
                      i < 64 ? w.leaf(Pick(r, kLeaves)) : w.animation(Pick(r, kAnimations)));
    }
  }
  for (uint32_t k = 0; k < kLeaves; k++) {
    const uint32_t leaf = w.leaf(k);
    guest::StoreU32(base, leaf, kTagTxt);
    guest::StoreU32(base, leaf + 100, Pick(r, 16));
    guest::StoreU32(base, leaf + 104, Pick(r, 8) == 0 ? 0 : Pick(r, 7));
    guest::StoreU32(base, leaf + 108, Pick(r, 9));
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, leaf + 128 + 8 * i, w.leaf(Pick(r, kLeaves)));
    }
  }
  const uint32_t pairs = w.movies.address + kPairsAt;
  guest::StoreU32(base, pairs, 44);
  guest::StoreU32(base, pairs + 4, movie);
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, 1);
  guest::StoreU32(base, kMissingMovie, movie);
  guest::StoreU32(base, kStringTable, w.strings.address);
}

// The save system's interface linked over random words: the users (a count
// of 0 to 5, controllers 0 to 3, signed in or not), the slots (a state the
// pages test, 0 to 11 or any; their request words 0 or 1, all 0 half the
// time; the device word +36 0 half the time; +992 0 to 5; their enumerated
// saves the four records, 0 to 4 of them), the records' file names
// SystemData.bin, another or random.
void ShapeSaveWorld(uint8_t* base, uint32_t block, Random& r) {
  WriteRandom(base, block, kSaveWorldBytes, r);
  guest::StoreU32(base, block, 0x82000E38);
  guest::StoreU32(base, block + 8, block + kSaveObject36);
  guest::StoreU32(base, block + kSaveObject36, 0x82000AFC);
  guest::StoreU32(base, block + kSaveObject36 + 88, block + kSaveDevices);
  guest::StoreU32(base, block + kSaveObject36 + 92, block + kSaveUsers);
  guest::StoreU32(base, block + kSaveDevices, 0x82002198);
  guest::StoreU32(base, block + kSaveDevices + 44, block + kSaveSlots);
  guest::StoreU32(base, block + kSaveUsers, 0x82000FA0);
  guest::StoreU32(base, block + kSaveUsers + 216, Pick(r, 6));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, block + kSaveUsers + 200 + 4 * i, Pick(r, 4));
    guest::StoreU32(base, block + kSaveUsers + 8 + 40 * i, Pick(r, 3) == 0 ? 0 : Pick(r, 5));
  }
  const bool idle = Pick(r, 2) == 0;
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t slot = block + kSaveSlots + 1000 * k;
    guest::StoreU32(base, slot + 988, Pick(r, 4) == 0 ? Small(r, 12) : OneOf(r, {1, 2, 3, 4, 7}));
    guest::StoreU32(base, slot + 36, Pick(r, 2) == 0 ? 0 : uint32_t(r.Next()));
    guest::StoreU32(base, slot + 992, Small(r, 6));
    if (Pick(r, 2) == 0) {  // the free bytes: a little or a lot
      guest::StoreU32(base, slot + 40, 0);
      guest::StoreU32(base, slot + 44, Pick(r, 2) == 0 ? Pick(r, 0x40000) : 0x100000);
    }
    for (uint32_t word = 960; word <= 980; word += 4) {
      guest::StoreU32(base, slot + word, idle ? 0 : Pick(r, 2));
    }
    guest::StoreU32(base, slot + 80, block + kSaveRecords);
    guest::StoreU32(base, slot + 984, Pick(r, 5));
  }
  for (uint32_t e = 0; e < 4; e++) {
    const uint32_t save = block + kSaveRecords + 308 * e;
    const uint32_t pick = Pick(r, 3);
    const char* name = pick == 0 ? "SystemData.bin" : "rex";
    const uint32_t length = uint32_t(std::strlen(name)) + 1;
    if (pick == 2) {
      for (uint32_t i = 0; i < 20; i++) {
        guest::StoreU8(base, save + 264 + i, uint8_t('a' + Pick(r, 26)));
      }
      guest::StoreU8(base, save + 284, 0);
    } else {
      Write(base, save + 264, name, length);
    }
  }
  guest::StoreU32(base, kSaveInterfaceGlobal, block);
}

// The menu list (as the race screens' harness lays one): its player's
// controller (+60) 0 to 3, each controller's entry 0 to 3, each entry's
// mapper one of ours (its vtable's slots 10 and 12 Pressed and Fired, now
// and then Held) with a few bits set in its words (+448 to +471); the count
// (+0) 0 to 12, the scroll, shown and selection around it (the selection
// one of `selections` when given), the action ids, the wrap word, the move
// word 0 mostly and the block of item words (half of them 0); the sound
// manager's slot 10 (MenuSound::Play's PlayCue2D) the getter.
void MenuWorld(uint8_t* base, const World& w, Random& r,
               const std::vector<uint32_t>& selections = {}) {
  const uint32_t at = w.at(kMenuWorldAt);
  const uint32_t mappers = at, vtables = at + 0x800, block = at + 0x900;
  const uint32_t manager = at + 0xB00, manager_vtable = at + 0xB80;
  for (uint32_t c = 0; c < 4; c++) {
    const uint32_t mapper = mappers + 0x200 * c, vtable = vtables + 0x40 * c;
    guest::StoreU32(base, rs::kControllerEntries + 4 * c, Pick(r, 4));
    guest::StoreU32(base, rs::kMapperEntries + 8 * c, mapper);
    guest::StoreU32(base, rs::kMapperEntries + 8 * c + 4, Pick(r, 5) == 0 ? 1 : Pick(r, 3));
    guest::StoreU32(base, mapper, vtable);
    guest::StoreU32(base, vtable + 40, Pick(r, 5) == 0 ? 0x8210BE70 : rs::kPressedSlot);
    guest::StoreU32(base, vtable + 48, Pick(r, 5) == 0 ? 0x8210BE70 : rs::kFiredSlot);
    for (uint32_t k = 0; k < 6; k++) {
      guest::StoreU32(base, mapper + 448 + 4 * k,
                      uint32_t(r.Next()) & uint32_t(r.Next()) & uint32_t(r.Next()));
    }
  }
  const uint32_t list = w.list();
  guest::StoreU32(base, list + rs::kListPlayer, Pick(r, 4));
  const int32_t count = int32_t(Pick(r, 13));
  guest::StoreU32(base, list + rs::kListCount, uint32_t(count));
  guest::StoreU32(base, list + rs::kListShown, Around(r, 0, count + 1));
  guest::StoreU32(base, list + rs::kListScroll, Around(r, -1, count));
  guest::StoreU32(base, list + rs::kListSelection, Around(r, -2, count + 1));
  guest::StoreU32(base, list + 20, Around(r, -2, count + 1));
  for (uint32_t field = rs::kListDown; field <= rs::kListBack; field += 4) {
    guest::StoreU32(base, list + field, Pick(r, 8) == 0 ? uint32_t(r.Next()) : 151 + Pick(r, 30));
  }
  guest::StoreU32(base, list + rs::kListWraps, Pick(r, 3) == 0 ? 0 : Pick(r, 4));
  guest::StoreU32(base, list + rs::kListMove, Pick(r, 3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, list + rs::kListBlock, block);
  for (uint32_t k = 0; k < 64; k++) {
    guest::StoreU32(base, block + 4 * k, Pick(r, 2) == 0 ? 0 : uint32_t(r.Next()));
  }
  if (!selections.empty()) {
    guest::StoreU32(base, list + rs::kListSelection,
                    selections[Pick(r, uint32_t(selections.size()))]);
  }
  guest::StoreU32(base, manager, manager_vtable);
  guest::StoreU32(base, manager_vtable + menu_sounds::kPlayCue2DSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
}

// The list's player's mapper made to answer the accept action (+52) alone
// (its entry's +4 0, slot 10 Pressed, only the action's bit of +456 set),
// so MenuList::Accepted answers 1 unless the update moves.
void ForceAccept(uint8_t* base, const World& w, Random& r) {
  const uint32_t list = w.list();
  const uint32_t action = 151 + Pick(r, 30);
  guest::StoreU32(base, list + rs::kListAccept, action);
  const uint32_t player = guest::LoadU32(base, list + rs::kListPlayer);
  const uint32_t controller = guest::LoadU32(base, rs::kControllerEntries + 4 * player);
  const uint32_t entry = rs::kMapperEntries + 8 * controller;
  guest::StoreU32(base, entry + 4, 0);
  const uint32_t mapper = guest::LoadU32(base, entry);
  guest::StoreU32(base, guest::LoadU32(base, mapper) + 40, rs::kPressedSlot);
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(base, mapper + 448 + 4 * k, 0);
  }
  guest::StoreU32(base, mapper + 456, 1u << (action - 151));
  guest::StoreU32(base, list + rs::kListMove, 0);
}

// A few named animations of movie 44 played (UiMovie::PlayNamed through its
// override, the same on both sides), so the finishes find them.
void PlaySome(uint8_t* base, const World& w, const PPCContext& from, Random& r) {
  const uint32_t count = Pick(r, 6);
  for (uint32_t i = 0; i < count; i++) {
    PPCContext t = from;
    t.r1.u64 = from.r1.u64 - 0x200;
    t.r3.u64 = w.movie();
    const uint32_t ids[10] = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15};
    t.r4.u64 = ids[Pick(r, 10)];
    t.r5.u64 = 0;
    t.r6.u64 = 0;
    t.r7.u64 = 0;
    t.r8.u64 = 0;
    sub_8211DAC8(t, base);
  }
}

void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  ShapeMovieWorld(base, w, r);
  WriteRandom(base, w.screen.address, w.screen.bytes, r);
  WriteRandom(base, w.save_data.address, w.save_data.bytes, r);
  WriteRandom(base, w.buffers.address, w.buffers.bytes, r);
  WriteRandom(base, w.game_data.address, w.game_data.bytes, r);
  ShapeSaveWorld(base, w.save.address, r);
  MenuWorld(base, w, r);
  guest::StoreU32(base, ss::kPages, w.pages());
  guest::StoreU32(base, ss::kList, w.list());
  guest::StoreU32(base, ss::kData, w.data());
  guest::StoreU32(base, ss::kModeType, OneOf(r, {2, 4, 6, 8, 10, 0}));
  WriteRandom(base, w.save_data1.address, w.save_data1.bytes, r);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, kSaveDataSlots + 4 * i, i == 1 ? w.save_data1.address : w.save_data.address);
  }
  // The save buffers: the game data, the replay, 1 to 4 records and their
  // list entries (each entry's +300 its record's +28, the names short: with
  // no entry found, Replays::Load, never seen in play, reads the name at
  // address 42).
  const uint32_t buffers = w.buffers.address;
  guest::StoreU32(base, kSaveBuffers, buffers);
  guest::StoreU32(base, buffers + 0, w.game_data.address);
  guest::StoreU32(base, buffers + 4, w.replay.address);
  guest::StoreU32(base, buffers + 12, buffers + 0x100);
  guest::StoreU32(base, buffers + 16, buffers + 0x200);
  const uint32_t records = 1 + Pick(r, 4);
  guest::StoreU32(base, buffers + 20, records);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t record = buffers + 0x100 + 36 * k, entry = buffers + 0x200 + 304 * k;
    guest::StoreU32(base, entry + 300, guest::LoadU32(base, record + 28));
    guest::StoreU8(base, entry + 20, 0);
    guest::StoreU16(base, entry + 42 + 40, 0);
  }
  guest::StoreU32(base, ss::kKeptReplay, w.kept.address);
  std::memset(base + w.kept.address, 0xAB, w.kept.bytes);
  // The profile: wanted or not, its read pending (997) or not, its results'
  // words +632, +712 and +752 0 or not and +672 0 to 4 (or any).
  guest::StoreU32(base, ss::kProfileWanted, Pick(r, 3) == 0 ? 0 : 1);
  guest::StoreU32(base, ss::kProfileRead, Pick(r, 4) == 0 ? 997 : Small(r, 4));
  WriteRandom(base, w.profile.address, w.profile.bytes, r);
  guest::StoreU32(base, ss::kProfileReader, w.profile.address);
  guest::StoreU32(base, w.profile.address + 4, w.profile.address + 0x100);
  for (const uint32_t field : {632u, 712u, 752u}) {
    guest::StoreU32(base, w.profile.address + 0x100 + field, Pick(r, 2) == 0 ? 0 : uint32_t(r.Next()));
  }
  guest::StoreU32(base, w.profile.address + 0x100 + 672, Small(r, 5));
  // The page block: the page (+0) 0 to 26 or -1, now and then any.
  guest::StoreU32(base, w.pages(), Pick(r, 4) == 0 ? 0xFFFFFFFFu : Small(r, 27));
  // The data: the answer (+4) 0 to 3, the state (+16) and the next (+20) -1
  // to 3, the wait (+24) around the waits' 45 and 90.
  const uint32_t data = w.data();
  guest::StoreU32(base, data + 4, Small(r, 4));
  guest::StoreU32(base, data + 8, Pick(r, 2) == 0 ? 0 : uint32_t(r.Next()));
  guest::StoreU32(base, data + 40, Pick(r, records));
  guest::StoreU32(base, data + 60, Pick(r, 3) == 0 ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, data + 16, Small(r, 5) - 1);
  guest::StoreU32(base, data + 20, Small(r, 5) - 1);
  const uint32_t waits[6] = {0, 43, 44, 45, 88, 89};
  guest::StoreU32(base, data + 24, Pick(r, 8) == 0 ? uint32_t(r.Next()) : waits[Pick(r, 6)]);
  // The mode's ring: its count and start small, now and then any.
  guest::StoreU32(base, w.mode() + 204 + 600, Pick(r, 8) == 0 ? uint32_t(r.Next()) : Pick(r, 50));
  guest::StoreU32(base, w.mode() + 204 + 604, Pick(r, 8) == 0 ? uint32_t(r.Next()) : Pick(r, 60));
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

using SetupFn = std::function<void(PPCContext&, Random&, const World&)>;
// The stack compared, as [sp - from, sp - to) ranges.
using Frames = std::vector<std::pair<uint32_t, uint32_t>>;
// What a group compares after the run beyond the world (blocks the function
// made or freed), before the world is read.
using AfterFn = std::function<void(uint8_t*, const World&, Outputs&)>;

// UiMovies::Open's and Delete's calls, recorded on both sides and answered
// 1 (Delete would free our movie).
std::vector<uint32_t>* g_opens = nullptr;
bool RecordOpen(PPCContext& ctx, uint32_t address) {
  if (g_opens == nullptr || (address != ss::kMoviesOpen && address != ss::kMoviesDelete)) {
    return false;
  }
  // Delete reads only r3: its r4 (a leftover) isn't compared.
  g_opens->insert(g_opens->end(), {address, ctx.r3.u32,
                                   address == ss::kMoviesDelete ? 0u : ctx.r4.u32, ctx.r1.u32,
                                   uint32_t(ctx.lr)});
  ctx.r3.u64 = 1;
  return true;
}

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Frames& frames,
           const AfterFn& after = {}) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = MixSeed(h.random.Next());
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
      t.r5.u64 = r.Next();
      t.r6.u64 = r.Next();
      t.r7.u64 = r.Next();
      t.r8.u64 = r.Next();
      t.r9.u64 = r.Next();
      t.r10.u64 = r.Next();
      setup(t, r, w);
      PlaySome(h.base, w, t, r);
      t.lr = 0x82000000 | (Pick(r, 0x10000) << 2);
      g_update_answers = Random(r.Next());
      std::vector<UpdateCall> updates;
      g_updates = &updates;
      const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      std::vector<uint32_t> opens;
      g_opens = &opens;
      const hooks::StartUpRecorder saved_recorder = hooks::g_start_up_recorder;
      hooks::g_start_up_recorder = RecordOpen;
      ITC_FLAG(native_save_screen) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_save_screen) = true;
      hooks::g_start_up_recorder = saved_recorder;
      g_opens = nullptr;
      ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
      g_updates = nullptr;
      for (const uint32_t word : opens) {
        out[v].Word(word);
      }
      if (after) {
        after(h.base, w, out[v]);
      }
      // The lists Replays::BuildList made (the layer's pages): their
      // records and entries by the count, freed, the pointers cleared.
      const uint32_t buffers = w.buffers.address;
      const uint32_t count = guest::LoadU32(h.base, buffers + 20);
      for (const auto& [field, bytes] : {std::pair{12u, 36u}, std::pair{16u, 304u}}) {
        const uint32_t list = guest::LoadU32(h.base, buffers + field);
        const bool made = list != 0 && memory::IsPages(list);
        out[v].Word(made ? 1 : 0);
        if (made) {
          out[v].Bytes(Read(h.base, list, bytes * std::min(count, 100u)));
          memory::FreePages(list);
          guest::StoreU32(h.base, buffers + field, 0);
        }
      }
      out[v].Word(uint32_t(base::Hash64(h.base + w.kept.address, w.kept.bytes)));
      out[v].Word(uint32_t(updates.size()));
      for (const UpdateCall& u : updates) {
        out[v].Word(u.lr);
        out[v].Word(u.r1);
        for (const uint64_t x : {u.r3, u.r4}) {
          out[v].Word(uint32_t(x >> 32));
          out[v].Word(uint32_t(x));
        }
      }
      for (const Block* b : {&w.screen, &w.movies, &w.texts, &w.save, &w.save_data,
                             &w.save_data1, &w.buffers, &w.game_data}) {
        out[v].Bytes(Read(h.base, b->address, b->bytes));
      }
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
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// --- The cases ---

// The dialog steps: r3 the started word (0 mostly, 1 or any), the state
// (+16) -1 to 3; the list's selection and previous selection small.
void Steps(Context& h, const World& w) {
  auto setup = [&h](PPCContext& t, Random& r, const World& w) {
    const uint32_t words[3] = {0, 1, uint32_t(r.Next())};
    guest::StoreU32(h.base, w.step(), Pick(r, 3) != 0 ? 0 : words[Pick(r, 3)]);
    guest::StoreU32(h.base, w.list() + 16, Small(r, 4));
    guest::StoreU32(h.base, w.list() + 20, Small(r, 4));
    t.r3.u64 = w.step();
  };
  Group(h, w, "SaveScreen::MoveSelection (sub_821A5118)", 200, setup, __imp__sub_821A5118,
        hooks::SaveScreenMoveSelection, {{128, 40}});
  Group(h, w, "SaveScreen::CloseDialog (sub_821A5038)", 200, setup, __imp__sub_821A5038,
        hooks::SaveScreenCloseDialog, {{128, 32}});
}

// The pages: r3 the mode; the message (+12) one the page tests (or any);
// the selection 0 or 1 mostly; the list made to accept half the time; the
// wait (+24) one below `wait` (the page's count, 0 for none) half the time.
SetupFn PageSetup(uint8_t* base, std::vector<uint32_t> messages, uint32_t wait = 0) {
  return [base, messages, wait](PPCContext& t, Random& r, const World& w) {
    if (wait != 0 && Pick(r, 2) == 0) {
      guest::StoreU32(base, w.data() + 24, wait - 1);
    }
    const uint32_t message = Pick(r, 8) == 0 ? uint32_t(r.Next())
                                             : messages[Pick(r, uint32_t(messages.size()))];
    guest::StoreU32(base, w.data() + 12, message);
    guest::StoreU32(base, w.list() + 16, Pick(r, 4) == 0 ? Small(r, 4) : Pick(r, 2));
    if (Pick(r, 2) == 0) {
      ForceAccept(base, w, r);
    }
    t.r3.u64 = Pick(r, 8) == 0 ? (uint64_t(r.Next()) << 32) | w.mode() : w.mode();
  };
}

void KindZeroPages(Context& h, const World& w) {
  Group(h, w, "SaveScreen::Page0 (sub_821A28E0)", 400,
        PageSetup(h.base, {1, 4, 5, 6, 11, 0, 2}), __imp__sub_821A28E0, hooks::SaveScreenPage0,
        {{128, 40}});
  Group(h, w, "SaveScreen::Page1 (sub_821A2AF8)", 300,
        PageSetup(h.base, {2, 3, 7, 9, 10, 0}, 45), __imp__sub_821A2AF8, hooks::SaveScreenPage1,
        {{96, 16}});
  Group(h, w, "SaveScreen::Page2 (sub_821A2C98)", 400, PageSetup(h.base, {0, 8, 1}, 90),
        __imp__sub_821A2C98, hooks::SaveScreenPage2, {{96, 16}});
  Group(h, w, "SaveScreen::Page3 (sub_821A2EE8)", 200, PageSetup(h.base, {0, 11}),
        __imp__sub_821A2EE8, hooks::SaveScreenPage3, {{96, 8}});
}

void EndPages(Context& h, const World& w) {
  const SetupFn setup = PageSetup(h.base, {0});
  Group(h, w, "SaveScreen::Page4 (sub_821A4D30)", 100, setup, __imp__sub_821A4D30,
        hooks::SaveScreenPage4, {});
  Group(h, w, "SaveScreen::Page9 (sub_821A4DE0)", 100, setup, __imp__sub_821A4DE0,
        hooks::SaveScreenPage9, {});
  Group(h, w, "SaveScreen::Page16 (sub_821A4E60)", 100, setup, __imp__sub_821A4E60,
        hooks::SaveScreenPage16, {});
  Group(h, w, "SaveScreen::Page25 (sub_821A4FD8)", 100, setup, __imp__sub_821A4FD8,
        hooks::SaveScreenPage25, {});
}

// The players' presses for kind 7's pick: each controller's mapper (as
// MenuWorld lays them) given the bit of the action its player tests (159 or
// 158 for player 0, 152 for the others) now and then.
void Presses(uint8_t* base, Random& r) {
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t entry =
        rs::kMapperEntries + 8 * guest::LoadU32(base, rs::kControllerEntries + 4 * i);
    const uint32_t mapper = guest::LoadU32(base, entry);
    if (Pick(r, 3) == 0) {
      const uint32_t action = i == 0 ? (Pick(r, 2) == 0 ? 159 : 158) : 152;
      guest::StoreU32(base, mapper + 456,
                      guest::LoadU32(base, mapper + 456) | (1u << (action - 151)));
    }
  }
}

void KindOnePages(Context& h, const World& w) {
  // StoreSettings: no arguments; 224-byte frame, saves 48 and lr.
  Group(h, w, "SaveScreen::StoreSettings (sub_821A24B0)", 300,
        [](PPCContext&, Random&, const World&) {}, __imp__sub_821A24B0,
        hooks::SaveScreenStoreSettings, {{224, 56}});
  // Page6 with StoreSettings' frame below its own.
  Group(h, w, "SaveScreen::Page6 (sub_821A32C0)", 400,
        PageSetup(h.base, {0, 5, 6, 7, 11, 1}, 45), __imp__sub_821A32C0, hooks::SaveScreenPage6,
        {{96, 16}, {320, 152}});
  Group(h, w, "SaveScreen::Page7 (sub_821A33E8)", 500, PageSetup(h.base, {10, 9, 4, 0}, 90),
        __imp__sub_821A33E8, hooks::SaveScreenPage7, {{96, 16}});
  Group(h, w, "SaveScreen::Page8 (sub_821A3728)", 300, PageSetup(h.base, {0}),
        __imp__sub_821A3728, hooks::SaveScreenPage8, {{96, 8}});
}

void ReplayPages(Context& h, const World& w) {
  Group(h, w, "SaveScreen::Page12 (sub_821A3A08)", 400,
        PageSetup(h.base, {12, 6, 5, 3, 1, 0}), __imp__sub_821A3A08, hooks::SaveScreenPage12,
        {{128, 48}});
  Group(h, w, "SaveScreen::Page13 (sub_821A3C60)", 400,
        PageSetup(h.base, {2, 4, 8, 9, 10, 11, 0}, 45), __imp__sub_821A3C60,
        hooks::SaveScreenPage13, {{96, 16}});
  // Page14: the header's lists 0 half the time (Replays::BuildList makes
  // new ones, compared and freed).
  const SetupFn page14 = PageSetup(h.base, {0, 7, 1}, 90);
  Group(
      h, w, "SaveScreen::Page14 (sub_821A67E8)", 500,
      [&h, page14](PPCContext& t, Random& r, const World& w) {
        page14(t, r, w);
        // The header's lists: none (with the message 0 only: the list's
        // build; the message 7 reads a record from them), or copies of the
        // shaped ones in the layer's pages (Replays::BuildList frees the old
        // ones).
        const bool none = guest::LoadU32(h.base, w.data() + 12) == 0 && Pick(r, 2) == 0;
        for (const auto& [field, bytes] : {std::pair{12u, 4u * 36}, std::pair{16u, 4u * 304}}) {
          uint32_t list = 0;
          if (!none) {
            list = memory::AllocPages(bytes, 4096);
            if (list != 0) {
              std::memcpy(h.base + list, h.base + guest::LoadU32(h.base, w.buffers.address + field),
                          bytes);
            }
          }
          guest::StoreU32(h.base, w.buffers.address + field, list);
        }
      },
      __imp__sub_821A67E8, hooks::SaveScreenPage14, {{96, 16}});
  Group(h, w, "SaveScreen::Page15 (sub_821A3DE8)", 300, PageSetup(h.base, {0}),
        __imp__sub_821A3DE8, hooks::SaveScreenPage15, {{96, 8}});
  const SetupFn page24 = PageSetup(h.base, {0});
  Group(
      h, w, "SaveScreen::Page24 (sub_821A4AC0)", 300,
      [&h, page24](PPCContext& t, Random& r, const World& w) {
        page24(t, r, w);
        Presses(h.base, r);
      },
      __imp__sub_821A4AC0, hooks::SaveScreenPage24, {{128, 48}});
}

// The menu list's item block (+68): 0, or a copy of the shaped one in the
// layer's pages (MenuList::Init and Release free the old one).
void ItemBlock(uint8_t* base, const World& w, Random& r) {
  uint32_t block = 0;
  if (Pick(r, 2) == 0) {
    block = memory::AllocPages(0x100, 4096);
    if (block != 0) {
      std::memcpy(base + block, base + w.at(kMenuWorldAt) + 0x900, 0x100);
    }
  }
  guest::StoreU32(base, w.list() + 68, block);
}
// After MenuList::Init: its item block, whether made, its words, freed.
void FreeItemBlock(uint8_t* base, const World& w, Outputs& out) {
  const uint32_t items = guest::LoadU32(base, w.list() + 68);
  const bool made = items != 0 && memory::IsPages(items);
  out.Word(made ? 1 : 0);
  if (made) {
    out.Bytes(Read(base, items, 2 * 4));
    memory::FreePages(items);
  }
  guest::StoreU32(base, w.list() + 68, 0);
}

// A global's block made by the function (the layer's pages): its bytes
// compared, freed, and the global put back to the world's.
void MadeBlock(uint8_t* base, uint32_t global, uint32_t bytes, uint32_t world, Outputs& out) {
  const uint32_t block = guest::LoadU32(base, global);
  const bool made = block != world && block != 0 && memory::IsPages(block);
  out.Word(made ? 1 : 0);
  if (made) {
    out.Bytes(Read(base, block, bytes));
    memory::FreePages(block);
  }
  guest::StoreU32(base, global, world);
}

// The save buffers' header SaveBuffers::Create made when [0x828161F0] was 0:
// its words, its buffers' first bytes, all freed, the global put back.
void MadeBuffers(uint8_t* base, const World& w, Outputs& out) {
  const uint32_t header = guest::LoadU32(base, ss::kSaveBuffers);
  const bool made = header != w.buffers.address && header != 0 && memory::IsPages(header);
  out.Word(made ? 1 : 0);
  if (made) {
    for (uint32_t field = 0; field < 24; field += 4) {
      const uint32_t word = guest::LoadU32(base, header + field);
      if (field < 12 && word != 0 && memory::IsPages(word)) {
        out.Bytes(Read(base, word, 256));
        memory::FreePages(word);
      } else {
        out.Word(word);
      }
    }
    memory::FreePages(header);
  }
  guest::StoreU32(base, ss::kSaveBuffers, w.buffers.address);
}

// The layer's pages a case gave the function (to free, or to use where it
// may free them): after the run, whether each is still pages, then its
// bytes' hash, and freed here.
std::vector<std::pair<uint32_t, uint32_t>> g_given;
uint32_t GivePages(uint8_t* base, uint32_t from, uint32_t bytes) {
  const uint32_t block = memory::AllocPages(bytes, 4096);
  if (block != 0) {
    if (from != 0) {
      std::memcpy(base + block, base + from, bytes);
    }
    g_given.push_back({block, bytes});
  }
  return block;
}
bool Given(uint32_t block) {
  for (const auto& [address, bytes] : g_given) {
    if (address == block) {
      return true;
    }
  }
  return false;
}
void CheckGiven(uint8_t* base, Outputs& out) {
  for (const auto& [block, bytes] : g_given) {
    const bool left = memory::IsPages(block);
    out.Word(left ? 1 : 0);
    if (left) {
      out.Word(uint32_t(base::Hash64(base + block, bytes)));
      memory::FreePages(block);
    }
  }
  g_given.clear();
}

void Handlers(Context& h, const World& w) {
  // r3 the machine (the screen block's +0x1000 less +9604, so its words
  // +9604 to +9612 are the block's), r4 the mode; +9608 and +9612 0 or 1.
  auto machine = [&h](PPCContext& t, Random& r, const World& w) {
    const uint32_t m = w.at(0x1000) - ss::kMachineState;
    guest::StoreU32(h.base, m + ss::kMachineLoaded, Pick(r, 4) == 0 ? uint32_t(r.Next()) : Pick(r, 2));
    guest::StoreU32(h.base, m + ss::kMachineAtOnce, Pick(r, 4) == 0 ? uint32_t(r.Next()) : Pick(r, 2));
    t.r3.u64 = m;
    t.r4.u64 = w.mode();
  };
  Group(h, w, "SaveScreen::OnEnter (sub_821A2608)", 100, machine, __imp__sub_821A2608,
        hooks::SaveScreenOnEnter, {{112, 24}}, [](uint8_t* base, const World& w, Outputs& out) {
          MadeBlock(base, ss::kPages, 28, w.pages(), out);
          MadeBlock(base, ss::kList, 72, w.list(), out);
          MadeBlock(base, ss::kData, 68, w.data(), out);
        });
  Group(h, w, "SaveScreen::OpenMovie (sub_821A1D10)", 50, machine, __imp__sub_821A1D10,
        hooks::SaveScreenOpenMovie, {});
  // WaitMovie: movie 44 loaded or not (its +0x9FD8... the table's pair kept).
  Group(h, w, "SaveScreen::WaitMovie (sub_821A26F0)", 200, machine, __imp__sub_821A26F0,
        hooks::SaveScreenWaitMovie, {{96, 16}});
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } kOpens[] = {
      {"SaveScreen::OpenKind0 (sub_821A2760)", __imp__sub_821A2760, hooks::SaveScreenOpenKind0},
      {"SaveScreen::OpenKind1 (sub_821A2790)", __imp__sub_821A2790, hooks::SaveScreenOpenKind1},
      {"SaveScreen::OpenKind2 (sub_821A2820)", __imp__sub_821A2820, hooks::SaveScreenOpenKind2},
      {"SaveScreen::OpenKind3 (sub_821A27F0)", __imp__sub_821A27F0, hooks::SaveScreenOpenKind3},
      {"SaveScreen::OpenKind4 (sub_821A2850)", __imp__sub_821A2850, hooks::SaveScreenOpenKind4},
      {"SaveScreen::OpenKind5 (sub_821A2880)", __imp__sub_821A2880, hooks::SaveScreenOpenKind5},
      {"SaveScreen::OpenKind6 (sub_821A27C0)", __imp__sub_821A27C0, hooks::SaveScreenOpenKind6},
      {"SaveScreen::OpenKind7 (sub_821A28B0)", __imp__sub_821A28B0, hooks::SaveScreenOpenKind7},
  };
  for (const auto& open : kOpens) {
    Group(h, w, open.name, 40, machine, open.recompiled, open.native, {});
  }
  Group(h, w, "SaveScreen::Cancel (sub_821A1D20)", 20, machine, __imp__sub_821A1D20,
        hooks::SaveScreenCancel, {});
  // Show: the kind 0 to 7 (or any); the header's buffers none now and then
  // (SaveBuffers::Create makes them); the item block 0 or pages.
  auto show = [&h, machine](PPCContext& t, Random& r, const World& w) {
    machine(t, r, w);
    guest::StoreU32(h.base, w.data() + 36, Small(r, 8));
    if (Pick(r, 4) == 0) {
      guest::StoreU32(h.base, ss::kSaveBuffers, 0);
    }
    ItemBlock(h.base, w, r);
  };
  const AfterFn show_after = [](uint8_t* base, const World& w, Outputs& out) {
    FreeItemBlock(base, w, out);
    MadeBuffers(base, w, out);
  };
  Group(h, w, "SaveScreen::Show (sub_821A6608)", 400, show, __imp__sub_821A6608,
        hooks::SaveScreenShow, {{96, 8}}, show_after);
  Group(h, w, "SaveScreen::ShowKind2 (sub_821A1D38)", 100, show, __imp__sub_821A1D38,
        hooks::SaveScreenShowKind2, {{128, 24}}, show_after);
  Group(h, w, "SaveScreen::ShowKind7 (sub_821A2138)", 100, show, __imp__sub_821A2138,
        hooks::SaveScreenShowKind7, {{128, 24}}, show_after);
  Group(h, w, "SaveScreen::Leave (sub_821A4C58)", 100, machine, __imp__sub_821A4C58,
        hooks::SaveScreenLeave, {{112, 16}});
  // OnLeft: the three blocks copies of the shaped ones in the layer's pages
  // (it frees them), the item block 0 or pages.
  Group(
      h, w, "SaveScreen::OnLeft (sub_821A1CA0)", 100,
      [&h, machine](PPCContext& t, Random& r, const World& w) {
        machine(t, r, w);
        g_given.clear();
        for (const auto& [global, bytes] :
             {std::pair{ss::kPages, 28u}, std::pair{ss::kList, 72u}, std::pair{ss::kData, 68u}}) {
          guest::StoreU32(h.base, global, GivePages(h.base, guest::LoadU32(h.base, global), bytes));
        }
        const uint32_t items = Pick(r, 2) == 0 ? GivePages(h.base, 0, 0x100) : 0;
        guest::StoreU32(h.base, guest::LoadU32(h.base, ss::kList) + 68, items);
      },
      __imp__sub_821A1CA0, hooks::SaveScreenOnLeft, {{112, 32}},
      [](uint8_t* base, const World& w, Outputs& out) {
        // Freed by the function: any still pages are freed here (counted).
        CheckGiven(base, out);
        for (const auto& [global, world] : {std::pair{ss::kPages, w.pages()},
                                            std::pair{ss::kList, w.list()},
                                            std::pair{ss::kData, w.data()}}) {
          const uint32_t block = guest::LoadU32(base, global);
          out.Word(block);
          guest::StoreU32(base, global, world);
        }
      });
}

// The Inits (as the type 6 and 7 modes' harness runs theirs): the machine
// empty or holding random transitions already (fewer than 200 less the
// Init's) with keys of small numbers, some the Init adds (so some Adds
// replace); the original's frame filled with the same random words for both
// (the words it never writes go into the transitions); the machine's block,
// the frame (but its saved registers), the scratch area and r3 (both
// halves) compared.
GuestFunction RecompiledInit(uint32_t address) {
  switch (address) {
    case 0x821A6E18: return __imp__sub_821A6E18;
    case 0x821A7470: return __imp__sub_821A7470;
    case 0x821A7AC8: return __imp__sub_821A7AC8;
    default: return nullptr;
  }
}

void Inits(Context& h) {
  namespace gm = game_modes;
  const Block object(0x2A00);
  if (object.address == 0) {
    ITC_LOG_ERROR("harness: save_screen: no block for the Inits");
    return;
  }
  for (const ss::InitInfo& info : ss::kInits) {
    Results results(fmt::format("{} (sub_{:08X})", info.name, info.address));
    const uint32_t saves = (32 - info.first_saved) * 8 + 8;
    const uint32_t frame = CallContext(h).r1.u32 - info.frame;
    for (uint64_t seed = 0; seed < 24; seed++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(MixSeed(seed * 151 + info.address));
        WriteRandom(h.base, h.data, kScratchBytes, r);
        WriteRandom(h.base, object.address, object.bytes, r);
        WriteRandom(h.base, frame, info.frame, r);
        const uint32_t machine = object.address;
        if (seed % 2 == 0) {
          gm::StateMachineConstruct(h.base, machine);
          guest::StoreU32(h.base, machine + gm::kTransitionCount, 0);
        } else {
          const uint32_t count = 8 + Pick(r, 160);
          guest::StoreU32(h.base, machine + gm::kTransitionCount, count);
          for (uint32_t i = 0; i < gm::kTransitions; i++) {
            const uint32_t at = machine + gm::kTransitionBytes * i;
            guest::StoreU32(h.base, at, Pick(r, 12));
            guest::StoreU32(h.base, at + 4, Pick(r, 50));
            guest::StoreU32(h.base, at + 8, Pick(r, 8) == 0 ? 1 : 0);
            guest::StoreU32(h.base, at + 12, 0);
          }
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = object.address;
        if (v == 0) {
          ITC_FLAG(native_save_screen) = false;
          RecompiledInit(info.address)(t, h.base);
          ITC_FLAG(native_save_screen) = true;
        } else {
          hooks::SaveScreenInit(t, h.base, info.address);
        }
        out[v].Bytes(Read(h.base, object.address, object.bytes));
        out[v].Bytes(Read(h.base, frame, info.frame - saves));
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("seed {}", seed));
    }
    results.Report();
  }
}

void UpdateAndDraw(Context& h, const World& w) {
  // Update: the running step -1, 0 to 6 or another negative, the queued -1
  // or 0 to 6, the started word 0 or 1; the message and the dialog's state
  // the steps test; the item block 0 or pages (ShowMessage's MenuList::Init).
  Group(
      h, w, "SaveScreen::Update (sub_821A64A0)", 500,
      [&h](PPCContext& t, Random& r, const World& w) {
        const uint32_t pages = w.pages();
        const uint32_t steps[9] = {0xFFFFFFFFu, 0, 1, 2, 3, 4, 5, 6, 0xFFFFFFFEu};
        guest::StoreU32(h.base, pages + 12, steps[Pick(r, 9)]);
        guest::StoreU32(h.base, pages + 16, Pick(r, 3) == 0 ? 0xFFFFFFFFu : Pick(r, 7));
        guest::StoreU32(h.base, pages + 24, Pick(r, 2));
        guest::StoreU32(h.base, w.data() + 12, Small(r, 18));
        guest::StoreU32(h.base, w.data() + 16, Small(r, 5) - 1);
        guest::StoreU32(h.base, w.list() + 16, Small(r, 3));
        ItemBlock(h.base, w, r);
        t.r3.u64 = r.Next();
      },
      __imp__sub_821A64A0, hooks::SaveScreenUpdate, {{144, 16}},
      [](uint8_t* base, const World& w, Outputs& out) { FreeItemBlock(base, w, out); });
  // Draw: r3 the machine, r4 the mode; the page any of the 27 (those
  // seen in play more often) or -1 (the pages for the player at the data's
  // +44 given 0 or 1); a step running or not; the cancel 0 or 1 with the kind 0
  // to 8; the next state 21 or another; the save buffers' header, its
  // buffers and lists as the layer's pages (SaveBuffers::Destroy frees
  // them); the pages' fields as their groups shape them.
  Group(
      h, w, "SaveScreen::Draw (sub_821A6AF0)", 500,
      [&h](PPCContext& t, Random& r, const World& w) {
        uint8_t* base = h.base;
        const uint32_t m = w.at(0x1000) - ss::kMachineState;
        t.r3.u64 = m;
        t.r4.u64 = w.mode();
        const uint32_t pages[22] = {0, 1, 2, 3, 4, 6, 7, 8, 9, 12, 13, 14, 15, 16, 24, 25, 26,
                                    0xFFFFFFFFu, 0xFFFFFFFFu, 0, 6, 12};
        guest::StoreU32(base, w.pages() + 0, Pick(r, 4) == 0 ? Pick(r, 27) : pages[Pick(r, 22)]);
        guest::StoreU32(base, w.pages() + 12, Pick(r, 2) == 0 ? 0xFFFFFFFFu : Pick(r, 7));
        guest::StoreU32(base, w.data() + 28, Pick(r, 2));
        guest::StoreU32(base, w.data() + 36, Small(r, 9));
        guest::StoreU32(base, w.data() + 32, Pick(r, 2) == 0 ? 21 : Small(r, 10));
        guest::StoreU32(base, w.data() + 12, Small(r, 13));
        guest::StoreU32(base, w.data() + 44, Pick(r, 2));
        const uint32_t waits[4] = {44, 89, 0, 90};
        guest::StoreU32(base, w.data() + 24, waits[Pick(r, 4)]);
        guest::StoreU32(base, w.list() + 16, Pick(r, 2));
        if (Pick(r, 2) == 0) {
          ForceAccept(base, w, r);
        }
        Presses(base, r);
        ItemBlock(base, w, r);
        g_given.clear();
        const uint32_t from = w.buffers.address;
        const uint32_t header = GivePages(base, from, 24);
        guest::StoreU32(base, header + 0, GivePages(base, w.game_data.address, kGameDataBytes));
        guest::StoreU32(base, header + 4, GivePages(base, 0, kReplayBlockBytes));
        guest::StoreU32(base, header + 8, Pick(r, 2) == 0 ? 0 : GivePages(base, 0, 64));
        guest::StoreU32(base, header + 12, GivePages(base, from + 0x100, 4 * 36));
        guest::StoreU32(base, header + 16, GivePages(base, from + 0x200, 4 * 304));
        guest::StoreU32(base, ss::kSaveBuffers, header);
      },
      __imp__sub_821A6AF0, hooks::SaveScreenDraw, {{368, 152}},
      [](uint8_t* base, const World& w, Outputs& out) {
        FreeItemBlock(base, w, out);
        // The header (0 once SaveBuffers::Destroy ran) and the lists
        // Replays::BuildList made in it, compared and freed; then what's
        // left of the given pages.
        const uint32_t header = guest::LoadU32(base, ss::kSaveBuffers);
        out.Word(header == 0 ? 0 : 1);
        if (header != 0 && memory::IsPages(header)) {
          const uint32_t count = guest::LoadU32(base, header + 20);
          out.Word(count);
          for (const auto& [field, bytes] : {std::pair{12u, 36u}, std::pair{16u, 304u}}) {
            const uint32_t list = guest::LoadU32(base, header + field);
            const bool made = list != 0 && !Given(list) && memory::IsPages(list);
            out.Word(made ? 1 : 0);
            if (made) {
              out.Bytes(Read(base, list, bytes * std::min(count, 13u)));
              memory::FreePages(list);
            }
            guest::StoreU32(base, header + field, 0);
          }
        }
        CheckGiven(base, out);
        guest::StoreU32(base, ss::kSaveBuffers, w.buffers.address);
      });
}

}  // namespace

void SaveScreen(Context& h) {
  Inits(h);
  base_of_world = h.base;
  const World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: save_screen: no block from the allocation layer");
    return;
  }
  BuildStringTable(h.base, w);
  Steps(h, w);
  KindZeroPages(h, w);
  EndPages(h, w);
  KindOnePages(h, w);
  ReplayPages(h, w);
  Handlers(h, w);
  UpdateAndDraw(h, w);
}

}  // namespace harness
