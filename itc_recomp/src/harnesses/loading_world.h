// The loading screen's UI movie world for harnesses (from the race's
// loading harness, src/harnesses/race_loading.cpp, which describes it): the
// three movies the loading screen's words point to, their scenes and
// texts, the string table of letters, and the globals the cases set, saved
// and put back. Shared by the race_loading and type67_modes harnesses.

#pragma once

#include "harness.h"

#include "base/log.h"
#include "memory/heap.h"
#include "race_loading/race_loading.h"
#include "ui_movies/game.h"

#include <iterator>

namespace harness::loading_world {

namespace rl = race_loading;

struct WorldBlock {
  explicit WorldBlock(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~WorldBlock() { memory::Free(address); }
  WorldBlock(const WorldBlock&) = delete;
  WorldBlock& operator=(const WorldBlock&) = delete;
  uint32_t bytes;
  uint32_t address;
};

inline void WorldRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

constexpr uint32_t kMovieHeaderBytes = 0xA000;
constexpr uint32_t kMovieCount = 3;
constexpr uint32_t kTreeNodeBytes = 0x400;
constexpr uint32_t kLeaves = 32, kAnimations = 8;
constexpr uint32_t kRootAt = kMovieCount * kMovieHeaderBytes;
constexpr uint32_t kLeavesAt = kRootAt + kTreeNodeBytes;
constexpr uint32_t kTrmAt = kLeavesAt + kLeaves * kTreeNodeBytes;
constexpr uint32_t kAnimationsAt = kTrmAt + 0x100;
constexpr uint32_t kTexturesAt = kAnimationsAt + kAnimations * 64;
constexpr uint32_t kMovieWorldBytes = kTexturesAt + 0x100;
constexpr uint32_t kTextsPerMovie = 8;
constexpr uint32_t kTextBuffersAt = 0x1000;
constexpr uint32_t kTextWorldBytes = kTextBuffersAt + kMovieCount * kTextsPerMovie * 0x1000;
constexpr uint32_t kStringWorldBytes = 0x10000;
constexpr uint32_t kMissingText = 0x826C5CD8;  // TextMap::Find's answer when not found
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTagTxt = 0x54585400;
constexpr uint32_t kTagSce = 0x53434500;
constexpr uint32_t kTagTrm = 0x54524D00;
constexpr uint32_t kTagPrm = 0x50524D00;
constexpr uint32_t kTagPic = 0x50494300;

struct MovieWorld {
  MovieWorld() : movies(kMovieWorldBytes), texts(kTextWorldBytes), strings(kStringWorldBytes) {}
  bool ok() const { return movies.address && texts.address && strings.address; }
  uint32_t movie(uint32_t m) const { return movies.address + kMovieHeaderBytes * m; }
  uint32_t root() const { return movies.address + kRootAt; }
  uint32_t leaf(uint32_t k) const { return movies.address + kLeavesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  WorldBlock movies;
  WorldBlock texts;
  WorldBlock strings;
};

// The string table: 17 files' pointers to one container of entries enough
// for the widest file (the bounds at 0x82007E18), each one of 8 strings of
// letters (no argument codes), as the in-race screens' harness builds it.
inline void BuildStringTable(uint8_t* base, const MovieWorld& w) {
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
    ITC_LOG_ERROR("harness: race_loading: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x4C6F6164ull);  // "Load"
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

inline void ShapeMovieWorld(uint8_t* base, const MovieWorld& w, Random& r) {
  WorldRandom(base, w.movies.address, w.movies.bytes, r);
  WorldRandom(base, w.texts.address, w.texts.bytes, r);
  for (uint32_t m = 0; m < kMovieCount; m++) {
    const uint32_t movie = w.movie(m);
    ui_movies::InitAnimationLists(base, movie);
    for (const uint32_t field : {16u, 20u, 24u}) {
      guest::StoreU32(base, movie + field, w.root());
    }
    guest::StoreU32(base, movie + 28, w.movies.address + kTexturesAt);
    for (uint32_t k = 0; k < 256; k++) {
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, w.root());
      guest::StoreU32(base, named + 32, w.animation(r.Below(kAnimations)));
    }
    // The texts: ids 1 to 7, each a record of a 4 KB buffer of its own.
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
  // The root and its leaves, one chunk after another (+4 the size), then the
  // TRM chunk.
  constexpr uint32_t kEntries = (kTreeNodeBytes - 128) / 8;
  guest::StoreU32(base, w.root(), kTagSce);
  guest::StoreU32(base, w.root() + 4, kTreeNodeBytes);
  guest::StoreU32(base, w.root() + 100, 0);  // the pose resets walk children 0 to 3
  guest::StoreU32(base, w.root() + 104, r.Below(5));
  guest::StoreU32(base, w.root() + 108, r.Below(4));
  guest::StoreU32(base, w.root() + 116, kEntries);
  for (uint32_t i = 0; i < kEntries; i++) {
    guest::StoreU32(base, w.root() + 128 + 8 * i, w.leaf(r.Below(kLeaves)));
  }
  for (uint32_t k = 0; k < kLeaves; k++) {
    const uint32_t leaf = w.leaf(k);
    // TXT (SetText's lookups need them); the cases make some PRM or PIC.
    guest::StoreU32(base, leaf, kTagTxt);
    guest::StoreU32(base, leaf + 4, kTreeNodeBytes);
    guest::StoreU32(base, leaf + 100, r.Below(16));
    guest::StoreU32(base, leaf + 104, r.Below(8) == 0 ? 0 : r.Below(7));
    guest::StoreU32(base, leaf + 108, r.Below(9));
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, leaf + 128 + 8 * i, w.leaf(r.Below(kLeaves)));
    }
  }
  guest::StoreU32(base, w.movies.address + kTrmAt, kTagTrm);
  guest::StoreU32(base, kStringTable, w.strings.address);
  for (uint32_t m = 0; m < kMovieCount; m++) {
    guest::StoreU32(base, rl::kLoadingMovies + 4 * m, w.movie(m));
  }
}

// The loading screen's globals the cases set (the shown word, the current
// movie, the three movies; the three animation words; the string table and
// the missing text; the system request and sub_82293358's two words), put
// back after each side.
struct SavedGlobals {
  static constexpr uint32_t kAddresses[] = {
      rl::kLoadingShown, rl::kLoadingCurrent, rl::kLoadingMovies, rl::kLoadingMovies + 4,
      rl::kLoadingMovies + 8, rl::kLoadingAnimations, rl::kLoadingAnimations + 4,
      rl::kLoadingAnimations + 8, kStringTable, kMissingText, rl::kSystemRequest,
      0x826C7958, 0x826C7944};
  uint32_t words[std::size(kAddresses)];
  explicit SavedGlobals(uint8_t* base) : base_(base) {
    for (size_t i = 0; i < std::size(kAddresses); i++) {
      words[i] = guest::LoadU32(base, kAddresses[i]);
    }
  }
  void Restore() const {
    for (size_t i = 0; i < std::size(kAddresses); i++) {
      guest::StoreU32(base_, kAddresses[i], words[i]);
    }
  }

 private:
  uint8_t* base_;
};

enum class Kind { kShow, kRun, kFinish, kSetFrames };

}  // namespace harness::loading_world
