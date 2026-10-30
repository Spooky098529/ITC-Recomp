// The UI movies' vectors and objects as plain C++ (game.h: "The movies'
// vectors and objects"; docs/functions/ui-movies-game.md).

#include "ui_movies/game.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "std_string/std_string.h"
#include "strings_and_font/strings_and_font.h"
#include "ui_movies/ui_movies.h"

#include <cstdlib>
#include <cstring>
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

std::optional<uint32_t> GetText(uint8_t* base, uint32_t movie, uint32_t id_address) {
  uint32_t id = Load(base, id_address);
  const bool new_id = id == 0;
  if (new_id) {
    id = Load(base, movie + kMovieTextCount) + 1;
  }
  const uint32_t map = movie + kMovieTexts;
  uint32_t record = Load(base, TextMapFindKey(base, map, id));
  if (record == 0 && !CanAllocate(base, 28)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211B660 UiMovie::GetText");
  if (new_id) {
    Store(base, id_address, id);
  }
  if (record != 0) {
    return record;
  }
  const auto made = memory::crt::OperatorNew(base, 28);
  Require(made.has_value(), "operator new", 28);
  record = *made;
  const uint32_t buffer = memory::AllocPages(4096, 4096);
  Store(base, record, buffer);
  guest::ForEachHostPart(base, buffer, 4096,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
  Store(base, record + 16, Load(base, 0x82000A34));
  for (uint32_t offset : {4u, 8u, 12u, 20u, 24u}) {
    Store(base, record + offset, 0);
  }
  return record;
}

uint32_t AddNamedAnimations(uint8_t* base, uint32_t stack, uint32_t movie, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8211E378 (a scene's named animations added)");
  constexpr uint32_t kFrame = 240;
  constexpr uint32_t kSeparator = 0x82001DE8;
  const uint32_t frame = stack - kFrame;
  const uint32_t name = frame + 96, copy = frame + 128;
  uint32_t r3 = movie;
  for (uint32_t i = 0; i < Load(base, scene + 112); i++) {
    const uint32_t child = Child(base, scene, i);
    r3 = child;
    if (Load(base, child) == kTagSce) {
      r3 = AddNamedAnimations(base, frame, movie, child);
    }
  }
  auto length = [&](uint32_t text) {
    uint32_t end = text;
    while (guest::LoadU8(base, end) != 0) {
      end++;
    }
    return end - text;
  };
  auto require = [&](bool done, const char* what) { Require(done, what, name); };
  for (uint32_t i = 0; i < Load(base, scene + 120); i++) {
    const uint32_t animation = SceneAnimation(base, scene, i);
    std_string::Construct(base, name);
    require(std_string::Append(base, name, scene + 84, length(scene + 84)), "append");
    require(std_string::Append(base, name, kSeparator, 1), "append");
    require(std_string::Append(base, name, animation + 16, length(animation + 16)), "append");
    require(std_string::ConstructFrom(base, copy, name, 0, 0xFFFFFFFFu), "string(const string&)");
    const uint32_t used = Load(base, movie + kMovieNamedUsed);
    Store(base, copy + 28, scene);
    Store(base, copy + 32, animation);
    Store(base, movie + kMovieNamedUsed, used + 1);
    const uint32_t entry = NamedEntry(movie, used);
    {
      ITC_NATIVE_RUNNING("sub_8211E318 (a named animation copied)");
      require(std_string::AssignFrom(base, entry, copy, 0, 0xFFFFFFFFu), "assign");
      Store(base, entry + 28, Load(base, copy + 28));
      Store(base, entry + 32, Load(base, copy + 32));
    }
    require(std_string::Tidy(base, copy, true, 0), "_Tidy");
    const auto block = StringBlock(base, name);
    require(std_string::Tidy(base, name, true, 0), "_Tidy");
    r3 = block ? *block : name;
  }
  return r3;
}

namespace {

void Zero(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

// The setters' steps: the node's record, `change(record)`, then the record
// stored for the node's id.
template <typename Change>
std::optional<uint32_t> ChangeText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                   uint32_t child, Change change) {
  const uint32_t id_address = FindNode(base, movie, sub_scene, child) + 108;
  const auto record = GetText(base, movie, id_address);
  if (!record) {
    return std::nullopt;
  }
  change(*record);
  TextMapSetKey(base, movie + kMovieTexts, Load(base, id_address), *record);
  return movie + kMovieTexts;
}

// Whether the record's next page is there: its page (+20) + 1 against its
// buffer's page count, signed.
bool HasNextPage(uint8_t* base, uint32_t record) {
  const uint32_t pages = strings_and_font::StringsPageCount(base, Load(base, record));
  return int32_t(Load(base, record + 20) + 1) <= int32_t(pages);
}

}  // namespace

std::optional<uint32_t> ClearText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                  uint32_t child) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C470 UiMovie::ClearText");
    Zero(base, Load(base, record), 4096);
    Store(base, record + 8, 0);
  });
}

std::optional<uint32_t> SetText(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                uint32_t stack, uint32_t movie, uint32_t sub_scene,
                                uint32_t child, uint32_t id, const uint64_t varargs[4]) {
  constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"
  constexpr uint32_t kStringTable = 0x826C5C34;
  const uint32_t node = FindNode(base, movie, sub_scene, child);
  if (Load(base, node) != kTagTxt) {
    return std::nullopt;
  }
  const uint32_t args = stack + 48;
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, args + 8 * i, uint32_t(varargs[i] >> 32));
    Store(base, args + 8 * i + 4, uint32_t(varargs[i]));
  }
  const uint32_t id_address = node + 108;
  const auto record = GetText(base, movie, id_address);
  if (!record) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211C1F8 UiMovie::SetText");
  Zero(base, Load(base, *record), 4096);
  Store(base, *record + 20, 0);
  strings_and_font::StringsFormatV(base, calls, stack - 128, Load(base, kStringTable),
                                   Load(base, *record), id & 0xFFFF, args);
  TextMapSetKey(base, movie + kMovieTexts, Load(base, id_address), *record);
  return movie + kMovieTexts;
}

std::optional<uint32_t> SetNodeText(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                    uint32_t stack, uint32_t movie, uint32_t node, uint32_t id,
                                    const uint64_t varargs[5]) {
  constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"
  constexpr uint32_t kStringTable = 0x826C5C34;
  if (Load(base, node) != kTagTxt) {
    return std::nullopt;
  }
  const uint32_t args = stack + 40;
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, args + 8 * i, uint32_t(varargs[i] >> 32));
    Store(base, args + 8 * i + 4, uint32_t(varargs[i]));
  }
  const uint32_t id_address = node + 108;
  const auto record = GetText(base, movie, id_address);
  if (!record) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211C7E8 UiMovie::SetNodeText");
  Zero(base, Load(base, *record), 4096);
  Store(base, *record + 20, 0);
  strings_and_font::StringsFormatV(base, calls, stack - 128, Load(base, kStringTable),
                                   Load(base, *record), id & 0xFFFF, args);
  TextMapSetKey(base, movie + kMovieTexts, Load(base, id_address), *record);
  return movie + kMovieTexts;
}

std::optional<uint32_t> SetTextScrollSpeed(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                           uint32_t child, uint32_t delay, double speed) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C4F0 (a text's scroll delay and speed)");
    Store(base, record + 12, delay);
    guest::StoreFloat(base, record + 16, float(speed));
  });
}

std::optional<uint32_t> ScrollText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                   uint32_t child, uint32_t scrolling) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C570 UiMovie::ScrollText");
    const uint32_t speed = Load(base, 0x820021F8);
    Store(base, record + 4, scrolling);
    Store(base, record + 12, 45);
    Store(base, record + 16, speed);
  });
}

std::optional<uint32_t> ResetTextScroll(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                        uint32_t child) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C5F8 (a text's scroll counter reset)");
    Store(base, record + 8, 0);
  });
}

std::optional<uint32_t> RevealText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                   uint32_t child) {
  bool stepped = false;
  const auto r3 = ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211D880 UiMovie::RevealText");
    ITC_NATIVE_RUNNING("sub_8211C930 (a text's next page)");
    stepped = HasNextPage(base, record);
    if (stepped) {
      Store(base, record + 20, Load(base, record + 20) + 1);
    }
  });
  if (!r3) {
    return std::nullopt;
  }
  return stepped ? 1u : 0u;
}

std::optional<uint32_t> HasMoreText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                    uint32_t child) {
  const uint32_t id_address = FindNode(base, movie, sub_scene, child) + 108;
  const auto record = GetText(base, movie, id_address);
  if (!record) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211D8D8 (whether a text has another page; sub_8211C9C0)");
  return HasNextPage(base, *record) ? 1u : 0u;
}

// --- The movie table's open and delete ---

uint32_t OpenMovie(uint8_t* base, uint32_t id, uint32_t asset, uint32_t frame,
                   const archive::LoadCall& load, const OpenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8211E808 UiMovies::Open");
  Store(base, frame + 80, id);
  if (Load(base, PairsFindKey(base, kMovieTable, id)) != 0) {
    return 0;
  }
  const uint32_t movie = calls.make();
  Store(base, frame + 80, movie);
  InitAnimationLists(base, movie);
  Store(base, movie + 0x9FD4, id);
  MovieLoad(base, movie, asset, load);
  Store(base, frame + 84, id);
  calls.set(frame + 84, frame + 80);
  return 1;
}

uint32_t UnloadMovie(uint8_t* base, uint32_t movie, uint32_t frame, const UnloadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8211B540 UiMovie::Unload");
  for (uint32_t i = 0; i < 3; i++) {
    calls.virtual_free(i, Load(base, movie + 4 * i));
  }
  Store(base, movie + 16, 0);
  Store(base, movie + 20, 0);
  Store(base, movie + 24, 0);
  const uint32_t textures = movie + kMovieTextures;
  while (Load(base, movie + 36) != 0) {
    const uint32_t count = Load(base, textures + kCount) - 1;
    Store(base, textures + kCount, count);
    const uint32_t entry = Load(base, Load(base, textures + kArray) + (count << 2));
    if (entry != 0) {
      Store(base, entry + 0, 0);
      Store(base, entry + 4, 0);
      Store(base, entry + 8, 0);
      calls.free(entry);
    }
  }
  const uint32_t items = movie + kMovieTextList;
  while (Load(base, movie + 48) != 0) {
    const uint32_t count = Load(base, items + kCount) - 1;
    const uint32_t item = (count << 7) + Load(base, items + kArray);
    Store(base, items + kCount, count);
    calls.copy_item(frame + 80, item);
    ClearTextItem(base, frame + 80);
  }
  const uint32_t xpr = movie + 12;
  if (Load(base, xpr) != 0) {
    calls.release_xpr(xpr);
  }
  Store(base, xpr, 0);
  Store(base, movie + kMovieLoaded, 0);
  return calls.free_texts(movie);
}

uint32_t DeleteMovie(uint8_t* base, uint32_t id, uint32_t frame, const DeleteCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8211E258 UiMovies::Delete");
  Store(base, frame + 80, id);
  const uint32_t entry = PairsFindKey(base, kMovieTable, id);
  const uint32_t movie = Load(base, entry);
  if (movie == 0) {
    return entry;
  }
  UnloadMovie(base, movie, frame - 240, calls.unload);
  calls.destroy(movie);
  calls.free(movie);
  Store(base, frame + 84, id);
  Store(base, frame + 80, 0);
  return calls.set(frame + 84, frame + 80);
}

}  // namespace ui_movies
