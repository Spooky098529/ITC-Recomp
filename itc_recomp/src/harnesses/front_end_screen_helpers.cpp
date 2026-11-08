// The front end's screen helpers (src/front_end_screen_helpers/,
// docs/functions/front-end-screen-helpers.md) against the recompiled
// functions. Two worlds:
//
// - The modes' releases: a mode of the case's (its block at +848 the
//   allocation layer's pages), the stand holder and viewports as
//   StandMode::Release's cases make them (src/harnesses/type67_modes.cpp),
//   the string table and the movie table of the case's, and a fake sound
//   manager whose IsCuePlaying (slot 14) is the getter sub_82090770 (r3 =
//   its +60, the low byte 0 half the time) and StopCue (slot 13) the leaf
//   recorder sub_820A8420 (r4's low byte to +320, r5 to +332, r6 to +336).
// - The UI movie functions: the options screen's harness's movie world
//   (four movies: ids 44, 7, 4 and 18), the string table of letters only (no
//   argument codes: the user's call, 2026-10-30), the save screen's data
//   ([0x826D8454]), the front end's car data ([0x826D83F4]) with its car
//   entries, a mode's dispatcher ring, the sound manager's slot 10
//   (MenuSound::Play's) the getter; UiMovie::Run's update stood in for,
//   recorded and answered.
//
// Compared: the worlds, the originals' frames (their register saves left
// out, and the words the varargs SetNodeText calls store from r6 on: the
// originals' leftovers, 0 in ours, the user's call for the parking area,
// 2026-10-30), the caller's area, r1 and r3, all exactly (nothing here is a
// float).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "cameras/cameras.h"
#include "front_end_screen_helpers/front_end_screen_helpers.h"
#include "hooks/front_end_screen_helpers.h"
#include "hooks/heap.h"
#include "hooks/ui_movies.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "strings_and_font/strings_and_font.h"
#include "type67_modes/type67_modes.h"
#include "ui_movies/game.h"

#include <fmt/format.h>

#include <cstring>
#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_front_end_screen_helpers);

namespace harness {

namespace {

namespace fe = front_end_screen_helpers;

constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
constexpr uint32_t kCallerBytes = 256;  // above it

// The getter sub_82090770 (r3 = +60) and the leaf recorder sub_820A8420.
constexpr uint32_t kGet60 = 0x82090770;
constexpr uint32_t kRecordR4R6 = 0x820A8420;

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

// --- The modes' releases ---

struct ReleaseInfo {
  const char* name;
  GuestFunction recompiled;
  bool (*native)(PPCContext&, uint8_t*);
  uint32_t frame, saves;
};

void Releases(Context& h) {
  namespace sf = strings_and_font;
  namespace t67 = type67_modes;
  uint8_t* const base = h.base;
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const ReleaseInfo releases[6] = {
      {"ModeType2::Release (sub_82196EC0)", __imp__sub_82196EC0,
       hooks::FrontEndModeType2Release, 144, 32},
      {"ModeType4::Release (sub_82224AB8)", __imp__sub_82224AB8,
       hooks::FrontEndModeType4Release, 112, 16},
      {"ModeType5::Release (sub_8222FB90)", __imp__sub_8222FB90,
       hooks::FrontEndModeType5Release, 128, 32},
      {"ModeType9::Release (sub_8225D590)", __imp__sub_8225D590,
       hooks::FrontEndModeType9Release, 128, 32},
      {"ModeType10::Release (sub_82288898)", __imp__sub_82288898,
       hooks::FrontEndModeType10Release, 96, 16},
      {"ModeType12::Release (sub_82277DB0)", __imp__sub_82277DB0,
       hooks::FrontEndModeType12Release, 96, 16},
  };
  const Block mode(0x400), table(sf::kStringsBytes), misc(0x800);
  if (!mode.address || !table.address || !misc.address) {
    ITC_LOG_ERROR("harness: front_end_screen_helpers: no block from the allocation layer");
    return;
  }
  const SavedBytes saved(base, {{t67::kStandHolder, 4},
                                {cameras::kViewports, 4},
                                {fe::kStringTable, 4},
                                {fe::kSoundManager, 4},
                                {ui_movies::kMovieTable, 12}});
  for (const ReleaseInfo& info : releases) {
    Results results(info.name);
    for (int c = 0; c < 40; c++) {
      const uint64_t seed = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed);
        std::vector<uint32_t> made;
        auto pages = [&](uint32_t bytes) {
          const uint32_t block = memory::AllocPages(bytes, 4096);
          WriteRandom(base, block, bytes < 64 ? bytes : 64, r);
          made.push_back(block);
          return block;
        };
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(base, sp - 512, 512 + 256, r);
        WriteRandom(base, mode.address, mode.bytes, r);
        guest::StoreU32(base, mode.address + fe::kModeBlock, pages(0x2000));
        const uint32_t viewports = pages(12);
        guest::StoreU32(base, viewports + 0, pages(1872));
        guest::StoreU32(base, viewports + 4, pages(1872));
        guest::StoreU32(base, cameras::kViewports, viewports);
        const uint32_t holder = pages(12);
        guest::StoreU32(base, t67::kStandHolder, holder);
        for (uint32_t i = 0; i < 3; i++) {
          uint32_t slot = 0;
          if (r.Below(2) != 0) {
            slot = pages(64);
            guest::StoreU32(base, slot, 0);
          }
          guest::StoreU32(base, holder + 4 * i, slot);
        }
        // The sound manager: IsCuePlaying the getter (its +60's low byte 0
        // half the time), StopCue the recorder.
        WriteRandom(base, misc.address, misc.bytes, r);
        const uint32_t manager = misc.address, vtable = misc.address + 0x400;
        guest::StoreU32(base, manager, vtable);
        guest::StoreU32(base, manager + 60,
                        r.Below(2) == 0 ? uint32_t(r.Next()) & ~0xFFu : uint32_t(r.Next()));
        guest::StoreU32(base, vtable + fe::kIsCuePlayingSlot, kGet60);
        guest::StoreU32(base, vtable + fe::kStopCueSlot, kRecordR4R6);
        guest::StoreU32(base, fe::kSoundManager, manager);
        WriteRandom(base, table.address, table.bytes, r);
        uint32_t files[sf::kStringFiles];
        uint32_t kinds[sf::kStringFiles];
        for (uint32_t f = 0; f < sf::kStringFiles; f++) {
          kinds[f] = r.Below(3);
          uint32_t count = r.Below(4);
          files[f] = kinds[f] == 0   ? 0
                     : kinds[f] == 1 ? memory::AllocPages(64, 4096)
                                     : uint32_t(r.Next()) | 1;
          if (kinds[f] == 2 && count == 1) {
            count = 2;
          }
          guest::StoreU32(base, table.address + 4 * f, files[f]);
          guest::StoreU32(base, table.address + 68 + 4 * f, r.Below(2));
          guest::StoreU32(base, table.address + 4236 + 4 * f, count);
        }
        guest::StoreU32(base, fe::kStringTable, table.address);
        const uint32_t capacity = 1 + r.Below(4);
        const uint32_t count = r.Below(capacity + 1);
        const uint32_t array = memory::crt::OperatorNewArray(base, 8 * capacity).value_or(0);
        WriteRandom(base, array, 8 * capacity, r);
        for (uint32_t k = 0; k < count; k++) {
          const uint32_t key = k > 0 && r.Below(4) == 0 ? 100 + r.Below(k) : 100 + k;
          guest::StoreU32(base, array + 8 * k, key);
          guest::StoreU32(base, array + 8 * k + 4, 0);
        }
        guest::StoreU32(base, ui_movies::kMovieTable, array);
        guest::StoreU32(base, ui_movies::kMovieTable + 4, capacity);
        guest::StoreU32(base, ui_movies::kMovieTable + 8, count);
        t.r3.u64 = mode.address;
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        if (v == 0) {
          ITC_FLAG(native_front_end_screen_helpers) = false;
          info.recompiled(t, base);
          ITC_FLAG(native_front_end_screen_helpers) = true;
        } else if (!info.native(t, base)) {
          ITC_LOG_ERROR("harness: front_end_screen_helpers: {} refused", info.name);
        }
        // The recorder's r6 (+336) left out: StopCue (AudioManager::StopCue
        // sub_82118A90, sound-cues.md) reads only r3 to r5, and the
        // originals' r6 is a leftover of their earlier calls (0 in ours).
        guest::StoreU32(base, manager + 336, 0);
        out[v].Word(uint32_t(made.size()));
        for (const uint32_t block : made) {
          out[v].Word(memory::IsPages(block));
          if (memory::IsPages(block)) memory::FreePages(block);
        }
        out[v].Bytes(Read(base, mode.address, mode.bytes));
        out[v].Bytes(Read(base, misc.address, misc.bytes));
        out[v].Word(guest::LoadU32(base, t67::kStandHolder));
        out[v].Word(guest::LoadU32(base, cameras::kViewports));
        for (uint32_t f = 0; f < sf::kStringFiles; f++) {
          const uint32_t now = guest::LoadU32(base, table.address + 4 * f);
          out[v].Word(now == files[f] ? 1 : now);
          if (kinds[f] == 1) {
            const bool left = memory::IsPages(files[f]);
            out[v].Word(left ? 1 : 0);
            if (left) {
              memory::FreePages(files[f]);
            }
          }
        }
        out[v].Bytes(Read(base, table.address + 68, table.bytes - 68));
        const uint32_t now = guest::LoadU32(base, ui_movies::kMovieTable);
        out[v].Word(now == array ? 1 : now);
        out[v].Bytes(Read(base, ui_movies::kMovieTable + 4, 8));
        out[v].Bytes(Read(base, array, 8 * capacity));
        out[v].Bytes(Read(base, sp - info.frame, info.frame - info.saves));
        out[v].Bytes(Read(base, sp, 256));
        out[v].Word(t.r1.u32);
        const uint32_t r3 = t.r3.u32;
        out[v].Word(r3 == table.address              ? 1
                    : r3 == ui_movies::kMovieTable   ? 2
                    : r3 >= array && r3 < array + 64 ? 0x100 + (r3 - array)
                                                     : r3);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        if (array != 0 && memory::IsHeapBlock(array)) memory::crt::OperatorDeleteArray(array);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The UI movie world (the options screen's harness's, four movies) ---

constexpr uint32_t kMovieHeaderBytes = 0xA100;  // +0xA000 FillDetail's word
constexpr uint32_t kMovieCount = 4;
constexpr uint32_t kMovieIds[kMovieCount] = {44, 7, 4, 18};
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
constexpr uint32_t kMissingText = 0x826C5CD8;   // TextMap::Find's answer when not found
constexpr uint32_t kMissingMovie = 0x826C5CDC;  // UiMovies::Find's answer when not found
constexpr uint32_t kTagTxt = 0x54585400;
constexpr uint32_t kTagSce = 0x53434500;

// The car entries: 4 of 0x2100 bytes (+744, +769, +8432 read).
constexpr uint32_t kEntryBytes = 0x2100;
constexpr uint32_t kEntryCount = 4;

struct World {
  World()
      : movies(kMovieWorldBytes),
        texts(kTextWorldBytes),
        strings(kStringWorldBytes),
        misc(0x2000),
        entries(kEntryCount * kEntryBytes) {}
  bool ok() const {
    return movies.address && texts.address && strings.address && misc.address &&
           entries.address;
  }
  uint32_t movie(uint32_t m) const { return movies.address + kMovieHeaderBytes * m; }
  uint32_t root() const { return movies.address + kRootAt; }
  uint32_t sub(uint32_t k) const { return movies.address + kSubScenesAt + kTreeNodeBytes * k; }
  uint32_t leaf(uint32_t k) const { return movies.address + kLeavesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  // The misc block: the save screen's data (+0), the car data (+0x100, its
  // entries' pointers at +0x700), the mode (+0x800, its ring at +204), the
  // sound manager (+0x1400) and its vtable (+0x1800).
  uint32_t save_data() const { return misc.address; }
  uint32_t car_data() const { return misc.address + 0x100; }
  uint32_t car_list() const { return misc.address + 0x700; }
  uint32_t mode() const { return misc.address + 0x800; }
  uint32_t manager() const { return misc.address + 0x1400; }
  uint32_t entry(uint32_t k) const { return entries.address + kEntryBytes * k; }
  Block movies;
  Block texts;
  Block strings;
  Block misc;
  Block entries;
};

const std::vector<std::pair<uint32_t, uint32_t>> kSavedGlobals = {
    {fe::kSaveData, 4},
    {fe::kCarData, 4},
    {menu_sounds::kSoundManager, 4},
    {menu_sounds::kEffectsVolume, 8},
    {menu_sounds::kMusicVolume, 4},
    {ui_movies::kMovieTable, 12},
    {kMissingMovie, 4},
    {kMissingText, 4},
    {fe::kStringTable, 4}};

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
    ITC_LOG_ERROR("harness: front_end_screen_helpers: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x48656C70657273ull);  // "Helpers"
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
    for (uint32_t k = 0; k < 64; k++) {
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
  guest::StoreU32(base, w.root() + 100, 0);
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
  for (uint32_t m = 0; m < kMovieCount; m++) {
    guest::StoreU32(base, pairs + 8 * m, kMovieIds[m]);
    guest::StoreU32(base, pairs + 8 * m + 4, w.movie(m));
  }
  guest::StoreU32(base, ui_movies::kMovieTable, pairs);
  guest::StoreU32(base, ui_movies::kMovieTable + 4, 8);
  guest::StoreU32(base, ui_movies::kMovieTable + 8, kMovieCount);
  guest::StoreU32(base, kMissingMovie, w.movie(0));
  guest::StoreU32(base, fe::kStringTable, w.strings.address);
}

void ShapeWorld(uint8_t* base, const World& w, Random& r) {
  ShapeMovieWorld(base, w, r);
  WriteRandom(base, w.misc.address, w.misc.bytes, r);
  WriteRandom(base, w.entries.address, w.entries.bytes, r);
  // The save screen's data: +12 and +16 0 half the time.
  guest::StoreU32(base, fe::kSaveData, w.save_data());
  for (const uint32_t field : {12u, 16u}) {
    guest::StoreU32(base, w.save_data() + field,
                    r.Below(2) == 0 ? 0 : (r.Below(2) == 0 ? 1 : uint32_t(r.Next())));
  }
  // The car data: the list's index (+48, +52) within the 8 pointers, the
  // entries' pointers (+1136) each one of the four entries; their ids
  // (+8432) around the logo's cases (-5 to 120).
  guest::StoreU32(base, fe::kCarData, w.car_data());
  guest::StoreU32(base, w.car_data() + 48, r.Below(4));
  guest::StoreU32(base, w.car_data() + 52, r.Below(4));
  guest::StoreU32(base, w.car_data() + 1136, w.car_list());
  for (uint32_t i = 0; i < 8; i++) {
    guest::StoreU32(base, w.car_list() + 4 * i, w.entry(r.Below(kEntryCount)));
  }
  for (uint32_t k = 0; k < kEntryCount; k++) {
    static constexpr int32_t kIds[8] = {2, 43, 71, 35, 47, 56, 57, 46};
    const int32_t id = r.Below(3) == 0 ? kIds[r.Below(8)] : int32_t(r.Below(126)) - 5;
    guest::StoreU32(base, w.entry(k) + 8432, uint32_t(id));
  }
  // The mode's dispatcher ring: its count and start small, now and then any.
  guest::StoreU32(base, w.mode() + 204 + 600, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(50));
  guest::StoreU32(base, w.mode() + 204 + 604, r.Below(8) == 0 ? uint32_t(r.Next()) : r.Below(60));
  // The sound manager: slot 10 (MenuSound::Play's PlayCue2D) the getter.
  const uint32_t manager = w.manager(), vtable = manager + 0x400;
  guest::StoreU32(base, manager, vtable);
  guest::StoreU32(base, vtable + menu_sounds::kPlayCue2DSlot, kGet60);
  guest::StoreU32(base, menu_sounds::kSoundManager, manager);
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

void Group(Context& h, const World& w, const char* name, int cases, const SetupFn& setup,
           GuestFunction recompiled, GuestFunction native, const Frames& frames) {
  Results results(name);
  for (int c = 0; c < cases; c++) {
    const uint64_t seed = h.random.Next();
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
      t.lr = 0x82000000 | (r.Below(0x10000) << 2);
      g_update_answers = Random(r.Next());
      std::vector<UpdateCall> updates;
      g_updates = &updates;
      const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
          ui_movies_hooks::g_update_callee_stand_in;
      ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
      ITC_FLAG(native_front_end_screen_helpers) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_front_end_screen_helpers) = true;
      ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
      g_updates = nullptr;
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
      out[v].Bytes(Read(h.base, w.misc.address, w.misc.bytes));
      out[v].Bytes(Read(h.base, w.entries.address, w.entries.bytes));
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

// The string id: one the callers pass, or any below 2000.
void StringId(PPCContext& t, Random& r, std::initializer_list<uint32_t> ids) {
  const std::vector<uint32_t> list(ids);
  t.r3.u64 = r.Below(4) == 0 ? r.Below(2000) : list[r.Below(uint32_t(list.size()))];
}

void Dialogs(Context& h, const World& w) {
  // The text setters: 144-byte frames, their register saves (56 or 48
  // bytes) and SetNodeText's r6 to r10 stores (+40 to +79) left out.
  const Frames text26 = {{144, 104}, {64, 56}};
  const Frames text27 = {{144, 104}, {64, 48}};
  const Frames note = {{112, 24}};
  Group(
      h, w, "SaveScreen::SetText (sub_821A51C8)", 100,
      [](PPCContext& t, Random& r, const World&) {
        StringId(t, r, {62, 61, 71, 72, 65, 66, 63, 64, 67, 70, 102, 59});
      },
      __imp__sub_821A51C8, hooks::FrontEndSaveScreenSetText, text26);
  Group(
      h, w, "SaveScreen::SetNote (sub_821A52C0)", 100,
      [](PPCContext& t, Random& r, const World&) { StringId(t, r, {62, 69, 70, 61}); },
      __imp__sub_821A52C0, hooks::FrontEndSaveScreenSetNote, note);
  Group(
      h, w, "SaveScreen::SetMessage (sub_821A5360)", 100,
      [](PPCContext& t, Random& r, const World&) { StringId(t, r, {107}); }, __imp__sub_821A5360,
      hooks::FrontEndSaveScreenSetMessage, text26);
  Group(
      h, w, "Options::SetSaveDialogText (sub_821C0D70)", 100,
      [](PPCContext& t, Random& r, const World&) { StringId(t, r, {62, 83, 69, 70, 84, 104}); },
      __imp__sub_821C0D70, hooks::FrontEndOptionsSetSaveDialogText, text27);
  Group(
      h, w, "Options::SetSaveDialogNote (sub_821C0E50)", 100,
      [](PPCContext& t, Random& r, const World&) { StringId(t, r, {62, 69, 70}); },
      __imp__sub_821C0E50, hooks::FrontEndOptionsSetSaveDialogNote, note);
  Group(
      h, w, "Options::SetCategory6DialogText (sub_821C0BF0)", 100,
      [](PPCContext& t, Random& r, const World&) { StringId(t, r, {62, 61, 71, 72, 65}); },
      __imp__sub_821C0BF0, hooks::FrontEndOptionsSetCategory6DialogText, text27);
  Group(
      h, w, "Options::SetCategory6DialogNote (sub_821C0CD0)", 100,
      [](PPCContext& t, Random& r, const World&) { StringId(t, r, {62, 69, 70}); },
      __imp__sub_821C0CD0, hooks::FrontEndOptionsSetCategory6DialogNote, note);
  // ShowNotice: r3 the step word (in the misc block, 0 or 1 or any); its
  // frame's +24 to +79 left out (SetMessage's varargs stores: UiMovies::
  // Find's leftovers in the original, 0 in ours), and SetMessage's frame
  // below it (its +40 to +79 left out as the setters').
  Group(
      h, w, "SaveScreen::ShowNotice (sub_821A63B0)", 200,
      [&h](PPCContext& t, Random& r, const World& w) {
        const uint32_t step = w.save_data() + 0x80;
        const uint32_t words[3] = {0, 1, uint32_t(r.Next())};
        guest::StoreU32(h.base, step, words[r.Below(3)]);
        t.r3.u64 = step;
      },
      __imp__sub_821A63B0, hooks::FrontEndSaveScreenShowNotice,
      {{144, 120}, {64, 32}, {288, 248}, {208, 200}});
}

void CarCards(Context& h, const World& w) {
  // Fill: r3 a leaf (TXT nodes all), r4 one of the entries.
  Group(
      h, w, "CarCard::Fill (sub_821821C8)", 200,
      [](PPCContext& t, Random& r, const World& w) {
        t.r3.u64 = w.leaf(r.Below(kLeaves));
        t.r4.u64 = w.entry(r.Below(kEntryCount));
      },
      __imp__sub_821821C8, hooks::FrontEndCarCardFill, {{128, 88}, {48, 32}});
  Group(
      h, w, "CarCard::FillDetail (sub_821827E0)", 200,
      [](PPCContext&, Random&, const World&) {}, __imp__sub_821827E0,
      hooks::FrontEndCarCardFillDetail, {{192, 152}, {112, 96}});
  // Open: r4 the mode; FillDetail's frame below Open's.
  Group(
      h, w, "CarDetail::Open (sub_821830C0)", 200,
      [](PPCContext& t, Random&, const World& w) { t.r4.u64 = w.mode(); }, __imp__sub_821830C0,
      hooks::FrontEndCarDetailOpen, {{112, 24}, {304, 264}, {224, 208}});
}

}  // namespace

void FrontEndScreenHelpers(Context& h) {
  Releases(h);
  const World w;
  if (!w.ok()) {
    ITC_LOG_ERROR("harness: front_end_screen_helpers: no block from the allocation layer");
    return;
  }
  BuildStringTable(h.base, w);
  Dialogs(h, w);
  CarCards(h, w);
}

}  // namespace harness
