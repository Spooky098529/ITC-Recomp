// The UI movies' vectors and objects as plain C++ (game.h: "The movies'
// vectors and objects"; docs/functions/ui-movies-game.md).

#include "ui_movies/game.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_string/std_string.h"
#include "ui_movies/ui_movies.h"

#include <cstdlib>
#include <unordered_set>

namespace ui_movies {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// operator new[] and delete[] once their checks said yes: a logged abort
// when they refuse anyway.
uint32_t NewArray(uint8_t* base, uint32_t size) {
  const auto block = memory::crt::OperatorNewArray(base, size);
  if (!block) {
    ITC_LOG_ERROR("UI movies: operator new[]({}) failed after its checks", size);
    std::abort();
  }
  return *block;
}
void DeleteArray(uint32_t block) {
  if (!memory::crt::OperatorDeleteArray(block)) {
    ITC_LOG_ERROR("UI movies: operator delete[]({:08X}) failed after its checks", block);
    std::abort();
  }
}

constexpr uint32_t kArray = 0, kCapacity = 4, kCount = 8;

// The size sub_8211B160 (8-byte items) and sub_8211A788 (words) ask for a
// capacity: -1 past what 32 bits hold, as the originals.
uint32_t ArrayBytes(uint32_t capacity, uint32_t item) {
  return capacity > 0xFFFFFFFFu / item ? 0xFFFFFFFFu : capacity * item;
}

// The append's capacity change (the doubled capacity, when the count has
// reached the capacity and doubling changes it), or none.
std::optional<uint32_t> Regrow(uint8_t* base, uint32_t vector) {
  const uint32_t capacity = Load(base, vector + kCapacity);
  if (Load(base, vector + kCount) < capacity || capacity * 2 == capacity) {
    return std::nullopt;
  }
  return capacity * 2;
}

// sub_8211B160 and sub_8211A788 for a new capacity: a new array (cleared
// for pairs), the items that fit copied (the old capacity or the new, the
// smaller), the old array freed. Returns the old array (r3 as operator
// delete[] leaves it).
uint32_t Reserve(uint8_t* base, uint32_t vector, uint32_t capacity, uint32_t item, bool clear) {
  const uint32_t block = NewArray(base, ArrayBytes(capacity, item));
  if (clear) {
    for (uint32_t i = 0; int32_t(i) < int32_t(capacity); i++) {
      Store(base, block + 8 * i, 0);
      Store(base, block + 8 * i + 4, 0);
    }
  }
  for (uint32_t i = 0;; i++) {
    const uint32_t old_capacity = Load(base, vector + kCapacity);
    if (i >= (old_capacity > capacity ? capacity : old_capacity)) {
      break;
    }
    const uint32_t from = Load(base, vector + kArray) + item * i;
    for (uint32_t k = 0; k < item; k += 4) {
      Store(base, block + item * i + k, Load(base, from + k));
    }
  }
  const uint32_t old = Load(base, vector + kArray);
  DeleteArray(old);
  Store(base, vector + kArray, block);
  Store(base, vector + kCapacity, capacity);
  return old;
}

// Whether the append's capacity change (if any) runs natively.
bool RegrowFits(uint8_t* base, uint32_t vector, uint32_t item) {
  const auto capacity = Regrow(base, vector);
  return !capacity || (CanAllocate(base, ArrayBytes(*capacity, item)) &&
                       CanDelete(Load(base, vector + kArray)));
}

}  // namespace

bool CanAllocate(uint8_t* base, uint32_t size) {
  return size <= 0xFFFFF000u && memory::crt::GetProcessHeap(base) != 0;
}

bool CanDelete(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

std::optional<uint32_t> MakePairs(uint8_t* base, uint32_t vector) {
  if (!CanAllocate(base, 512)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211B0E8 (the movie table's vector made)");
  Store(base, vector + kCapacity, 64);
  const uint32_t block = NewArray(base, 512);
  for (uint32_t i = 0; i < 64; i++) {
    Store(base, block + 8 * i, 0);
    Store(base, block + 8 * i + 4, 0);
  }
  Store(base, vector + kArray, block);
  return vector;
}

std::optional<uint32_t> PairsAppend(uint8_t* base, uint32_t vector, uint32_t key,
                                    uint32_t value) {
  if (!RegrowFits(base, vector, 8)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211B458 (a pair appended)");
  uint32_t r3 = vector;
  if (const auto capacity = Regrow(base, vector)) {
    ITC_NATIVE_RUNNING("sub_8211B160 (the pairs' capacity changed)");
    r3 = Reserve(base, vector, *capacity, 8, true);
  }
  const uint32_t count = Load(base, vector + kCount);
  const uint32_t at = Load(base, vector + kArray) + 8 * count;
  Store(base, vector + kCount, count + 1);
  Store(base, at, key);
  Store(base, at + 4, value);
  return r3;
}

std::optional<uint32_t> PairsSet(uint8_t* base, uint32_t vector, uint32_t key, uint32_t value) {
  const uint32_t count = Load(base, vector + kCount);
  const uint32_t array = Load(base, vector + kArray);
  for (uint32_t i = 0; i < count; i++) {
    if (Load(base, array + 8 * i) == key) {
      ITC_NATIVE_RUNNING("sub_8211C168 (a pair set)");
      Store(base, Load(base, vector + kArray) + 8 * i + 4, value);
      return vector;
    }
  }
  if (!RegrowFits(base, vector, 8)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211C168 (a pair set)");
  return PairsAppend(base, vector, key, value);
}

std::optional<uint32_t> MakeWords(uint8_t* base, uint32_t vector) {
  if (!CanAllocate(base, 2048)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211AE18 (a words vector made)");
  Store(base, vector + kCapacity, 512);
  Store(base, vector + kArray, NewArray(base, 2048));
  Store(base, vector + kCount, 0);
  return vector;
}

std::optional<uint32_t> WordsAppend(uint8_t* base, uint32_t vector, uint32_t word) {
  if (!RegrowFits(base, vector, 4)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211AE68 (a word appended)");
  uint32_t r3 = vector;
  if (const auto capacity = Regrow(base, vector)) {
    ITC_NATIVE_RUNNING("sub_8211A788 (the words' capacity changed)");
    r3 = Reserve(base, vector, *capacity, 4, false);
  }
  const uint32_t count = Load(base, vector + kCount);
  const uint32_t array = Load(base, vector + kArray);
  Store(base, vector + kCount, count + 1);
  Store(base, array + 4 * count, word);
  return r3;
}

namespace {

// A free the checks said would run natively: a logged abort when it
// doesn't.
void Require(bool done, const char* what, uint32_t block) {
  if (!done) {
    ITC_LOG_ERROR("UI movies: {}({:08X}) failed after its checks", what, block);
    std::abort();
  }
}

uint32_t NamedEntry(uint32_t movie, uint32_t i) {
  return movie + kMovieNamed + kNamedBytes * i;
}

// The block a named animation's string frees (_Tidy with `built`): its
// pointer while the capacity is 16 or more, else none.
std::optional<uint32_t> StringBlock(uint8_t* base, uint32_t string) {
  if (Load(base, string + std_string::kCapacity) < std_string::kInPlace) {
    return std::nullopt;
  }
  return Load(base, string + std_string::kText);
}

}  // namespace

std::optional<uint32_t> ConstructMovie(uint8_t* base, uint32_t movie) {
  if (!CanAllocate(base, 2048) || !CanAllocate(base, 65552)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211E1C0 UiMovie::UiMovie");
  MakeWords(base, movie + kMovieTextures);
  {
    ITC_NATIVE_RUNNING("sub_8211B018 (the text list made)");
    const uint32_t list = movie + kMovieTextList;
    Store(base, list + kCapacity, 512);
    const uint32_t block = NewArray(base, 65552);
    Store(base, block, 512);
    const uint32_t items = block + 16;
    for (uint32_t i = 0; i < 512; i++) {
      MakeTextItem(base, items + 128 * i);
    }
    Store(base, list + kArray, items);
    Store(base, list + kCount, 0);
  }
  ResetDrawContext(base, movie + 64);
  for (uint32_t i = 0; i < kNamedCount; i++) {
    ITC_NATIVE_RUNNING("sub_8211D210 (a named animation made)");
    const uint32_t entry = NamedEntry(movie, i);
    std_string::Construct(base, entry);
    Store(base, entry + 28, 0);
    Store(base, entry + 32, 0);
  }
  Store(base, movie + kMovieNamedUsed, 0);
  for (uint32_t i = 0; i < 128; i++) {
    Store(base, movie + kMovieTexts + 8 * i, 0);
    Store(base, movie + kMovieTexts + 8 * i + 4, 0);
  }
  Store(base, movie + kMovieTexts + 1024, 0);
  return movie;
}

std::optional<uint32_t> DestroyMovie(uint8_t* base, uint32_t movie) {
  std::unordered_set<uint32_t> blocks;
  auto deletable = [&](uint32_t block) {
    return CanDelete(block) && (block == 0 || blocks.insert(block).second);
  };
  for (uint32_t i = 0; i < kNamedCount; i++) {
    const auto block = StringBlock(base, NamedEntry(movie, i));
    if (block && !deletable(*block)) {
      return std::nullopt;
    }
  }
  const uint32_t items = Load(base, movie + kMovieTextList);
  if ((items != 0 && !deletable(items - 16)) || !deletable(Load(base, movie + kMovieTextures))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211E160 UiMovie::~UiMovie");
  {
    ITC_NATIVE_RUNNING("sub_8211D1B8 (the named animations destroyed)");
    for (uint32_t i = kNamedCount; i-- > 0;) {
      const uint32_t entry = NamedEntry(movie, i);
      Require(std_string::Tidy(base, entry, true, 0), "a named animation's _Tidy", entry);
    }
  }
  // The draw context's destructor (sub_82106540) is empty.
  if (items != 0) {
    ITC_NATIVE_RUNNING("sub_8211AF90 (the text items' deleting destructor)");
    const uint32_t block = items - 16;
    const uint32_t count = Load(base, block);
    uint32_t item = items + (count << 7);
    for (int32_t k = int32_t(count - 1); k >= 0; k--) {
      item -= 128;
      ClearTextItem(base, item);
    }
    DeleteArray(block);
  }
  const uint32_t textures = Load(base, movie + kMovieTextures);
  DeleteArray(textures);
  return textures;
}

std::optional<uint32_t> FreeTexts(uint8_t* base, uint32_t movie) {
  const uint32_t count = Load(base, movie + kMovieTextCount);
  std::unordered_set<uint32_t> records;
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t record = Load(base, movie + kMovieTexts + 4 + 8 * k);
    if (!memory::InRange(Load(base, record)) || !CanDelete(record) ||
        (record != 0 && !records.insert(record).second)) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_8211AC90 (a movie's texts freed)");
  uint32_t r3 = movie;
  for (uint32_t k = 0; k < Load(base, movie + kMovieTextCount); k++) {
    const uint32_t record = Load(base, movie + kMovieTexts + 4 + 8 * k);
    // VirtualFree(buffer, 0, MEM_RELEASE), as its adapter frees the layer's
    // pages.
    memory::FreePages(Load(base, record));
    Require(memory::crt::OperatorDelete(record), "operator delete", record);
    r3 = record;
  }
  Store(base, movie + kMovieTextCount, 0);
  return r3;
}

}  // namespace ui_movies
