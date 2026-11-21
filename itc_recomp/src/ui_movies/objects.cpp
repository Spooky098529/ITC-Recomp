// The UI movies' vectors and objects as plain C++ (game.h: "The movies'
// vectors and objects"; docs/functions/ui-movies-game.md).

#include "ui_movies/game.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "schema/ui_movies_game.h"
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

using schema::UiMovie;
using schema::UiMoviePair;
using schema::UiMoviePairVector;
using schema::UiMovieText;
using schema::UiMovieWordVector;
using Globals = schema::UiMoviesGameGlobals;

// The pair and word vectors share their layout (+0 the array, +4 the
// capacity, +8 the count): the shared steps view them as word vectors.
// The size sub_8211B160 (8-byte items) and sub_8211A788 (words) ask for a
// capacity: -1 past what 32 bits hold, as the originals.
uint32_t ArrayBytes(uint32_t capacity, uint32_t item) {
  return capacity > 0xFFFFFFFFu / item ? 0xFFFFFFFFu : capacity * item;
}

// The append's capacity change (the doubled capacity, when the count has
// reached the capacity and doubling changes it), or none.
std::optional<uint32_t> Regrow(uint8_t* base, uint32_t vector) {
  const UiMovieWordVector v(base, vector);
  const uint32_t capacity = v.capacity();
  if (v.count() < capacity || capacity * 2 == capacity) {
    return std::nullopt;
  }
  return capacity * 2;
}

// sub_8211B160 and sub_8211A788 for a new capacity: a new array (cleared
// for pairs), the items that fit copied (the old capacity or the new, the
// smaller), the old array freed. Returns the old array (r3 as operator
// delete[] leaves it).
uint32_t Reserve(uint8_t* base, uint32_t vector, uint32_t capacity, uint32_t item, bool clear) {
  const UiMovieWordVector v(base, vector);
  const uint32_t block = NewArray(base, ArrayBytes(capacity, item));
  if (clear) {
    for (uint32_t i = 0; int32_t(i) < int32_t(capacity); i++) {
      const UiMoviePair pair(base, block + 8 * i);
      pair.set_key(0);
      pair.set_value(0);
    }
  }
  for (uint32_t i = 0;; i++) {
    const uint32_t old_capacity = v.capacity();
    if (i >= (old_capacity > capacity ? capacity : old_capacity)) {
      break;
    }
    const uint32_t from = v.words() + item * i;
    for (uint32_t k = 0; k < item; k += 4) {
      Store(base, block + item * i + k, Load(base, from + k));
    }
  }
  const uint32_t old = v.words();
  DeleteArray(old);
  v.set_words(block);
  v.set_capacity(capacity);
  return old;
}

// Whether the append's capacity change (if any) runs natively.
bool RegrowFits(uint8_t* base, uint32_t vector, uint32_t item) {
  const auto capacity = Regrow(base, vector);
  return !capacity || (CanAllocate(base, ArrayBytes(*capacity, item)) &&
                       CanDelete(UiMovieWordVector(base, vector).words()));
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
  const UiMoviePairVector v(base, vector);
  v.set_capacity(64);
  const uint32_t block = NewArray(base, 512);
  for (uint32_t i = 0; i < 64; i++) {
    const UiMoviePair pair(base, block + 8 * i);
    pair.set_key(0);
    pair.set_value(0);
  }
  v.set_pairs(block);
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
  const UiMoviePairVector v(base, vector);
  const uint32_t count = v.count();
  const UiMoviePair at(base, v.pairs().address + 8 * count);
  v.set_count(count + 1);
  at.set_key(key);
  at.set_value(value);
  return r3;
}

std::optional<uint32_t> PairsSet(uint8_t* base, uint32_t vector, uint32_t key, uint32_t value) {
  const UiMoviePairVector v(base, vector);
  const uint32_t count = v.count();
  const uint32_t array = v.pairs().address;
  for (uint32_t i = 0; i < count; i++) {
    if (UiMoviePair(base, array + 8 * i).key() == key) {
      ITC_NATIVE_RUNNING("sub_8211C168 (a pair set)");
      UiMoviePair(base, v.pairs().address + 8 * i).set_value(value);
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
  const UiMovieWordVector v(base, vector);
  v.set_capacity(512);
  v.set_words(NewArray(base, 2048));
  v.set_count(0);
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
  const UiMovieWordVector v(base, vector);
  const uint32_t count = v.count();
  const uint32_t array = v.words();
  v.set_count(count + 1);
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

schema::UiMovieNamedAnimation NamedEntry(uint8_t* base, uint32_t movie, uint32_t i) {
  return schema::UiMovieNamedAnimation(
      base, UiMovie(base, movie).named_animations_address() + kNamedBytes * i);
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
  const UiMovie m(base, movie);
  MakeWords(base, m.textures_address());
  {
    ITC_NATIVE_RUNNING("sub_8211B018 (the text list made)");
    const schema::UiMovieDrawTextList list(base, m.text_items_address());
    list.set_capacity(512);
    const uint32_t block = NewArray(base, 65552);
    schema::UiMovieTextItemArray(base, block).set_count(512);
    const uint32_t items = block + 16;
    for (uint32_t i = 0; i < 512; i++) {
      MakeTextItem(base, items + 128 * i);
    }
    list.set_items(items);
    list.set_count(0);
  }
  ResetDrawContext(base, m.draw_context_address());
  for (uint32_t i = 0; i < kNamedCount; i++) {
    ITC_NATIVE_RUNNING("sub_8211D210 (a named animation made)");
    const schema::UiMovieNamedAnimation entry = NamedEntry(base, movie, i);
    std_string::Construct(base, entry.name_address());
    entry.set_scene(0);
    entry.set_animation(0);
  }
  m.set_named_count(0);
  for (uint32_t i = 0; i < 128; i++) {
    const UiMoviePair pair(base, m.texts_address() + 8 * i);
    pair.set_key(0);
    pair.set_value(0);
  }
  m.set_text_count(0);
  return movie;
}

std::optional<uint32_t> DestroyMovie(uint8_t* base, uint32_t movie) {
  std::unordered_set<uint32_t> blocks;
  auto deletable = [&](uint32_t block) {
    return CanDelete(block) && (block == 0 || blocks.insert(block).second);
  };
  const UiMovie m(base, movie);
  for (uint32_t i = 0; i < kNamedCount; i++) {
    const auto block = StringBlock(base, NamedEntry(base, movie, i).name_address());
    if (block && !deletable(*block)) {
      return std::nullopt;
    }
  }
  const uint32_t items = m.text_items().address;
  if ((items != 0 && !deletable(items - 16)) || !deletable(m.textures())) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211E160 UiMovie::~UiMovie");
  {
    ITC_NATIVE_RUNNING("sub_8211D1B8 (the named animations destroyed)");
    for (uint32_t i = kNamedCount; i-- > 0;) {
      const uint32_t entry = NamedEntry(base, movie, i).name_address();
      Require(std_string::Tidy(base, entry, true, 0), "a named animation's _Tidy", entry);
    }
  }
  // The draw context's destructor (sub_82106540) is empty.
  if (items != 0) {
    ITC_NATIVE_RUNNING("sub_8211AF90 (the text items' deleting destructor)");
    const uint32_t block = items - 16;
    const uint32_t count = schema::UiMovieTextItemArray(base, block).count();
    uint32_t item = items + (count << 7);
    for (int32_t k = int32_t(count - 1); k >= 0; k--) {
      item -= 128;
      ClearTextItem(base, item);
    }
    DeleteArray(block);
  }
  const uint32_t textures = m.textures();
  DeleteArray(textures);
  return textures;
}

std::optional<uint32_t> FreeTexts(uint8_t* base, uint32_t movie) {
  const UiMovie m(base, movie);
  const uint32_t count = m.text_count();
  std::unordered_set<uint32_t> records;
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t record = UiMoviePair(base, m.texts_address() + 8 * k).value();
    if (!memory::InRange(UiMovieText(base, record).buffer().address) || !CanDelete(record) ||
        (record != 0 && !records.insert(record).second)) {
      return std::nullopt;
    }
  }
  ITC_NATIVE_RUNNING("sub_8211AC90 (a movie's texts freed)");
  uint32_t r3 = movie;
  for (uint32_t k = 0; k < m.text_count(); k++) {
    const uint32_t record = UiMoviePair(base, m.texts_address() + 8 * k).value();
    // VirtualFree(buffer, 0, MEM_RELEASE), as its adapter frees the layer's
    // pages.
    memory::FreePages(UiMovieText(base, record).buffer().address);
    Require(memory::crt::OperatorDelete(record), "operator delete", record);
    r3 = record;
  }
  m.set_text_count(0);
  return r3;
}

std::optional<uint32_t> GetText(uint8_t* base, uint32_t movie, uint32_t id_address) {
  const UiMovie m(base, movie);
  const schema::UiMovieWord id_word(base, id_address);
  uint32_t id = id_word.value();
  const bool new_id = id == 0;
  if (new_id) {
    id = m.text_count() + 1;
  }
  const uint32_t map = m.texts_address();
  uint32_t record = schema::UiMovieWord(base, TextMapFindKey(base, map, id)).value();
  if (record == 0 && !CanAllocate(base, 28)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211B660 UiMovie::GetText");
  if (new_id) {
    id_word.set_value(id);
  }
  if (record != 0) {
    return record;
  }
  const auto made = memory::crt::OperatorNew(base, 28);
  Require(made.has_value(), "operator new", 28);
  record = *made;
  const UiMovieText text(base, record);
  const uint32_t buffer = memory::AllocPages(4096, 4096);
  text.set_buffer(buffer);
  guest::ForEachHostPart(base, buffer, 4096,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
  text.set_scroll_speed_word(Globals::zero(base));
  text.set_scrolling(0);
  text.set_scroll_counter(0);
  text.set_scroll_delay(0);
  text.set_page(0);
  text.set_first_line(0);
  return record;
}

uint32_t AddNamedAnimations(uint8_t* base, uint32_t stack, uint32_t movie, uint32_t scene) {
  ITC_NATIVE_RUNNING("sub_8211E378 (a scene's named animations added)");
  constexpr uint32_t kFrame = 240;
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
    require(std_string::Append(base, name, Globals::name_separator_address(), 1), "append");
    const uint32_t animation_name = schema::UiMovieFileAnimation(base, animation).name_address();
    require(std_string::Append(base, name, animation_name, length(animation_name)), "append");
    require(std_string::ConstructFrom(base, copy, name, 0, 0xFFFFFFFFu), "string(const string&)");
    const UiMovie m(base, movie);
    const uint32_t used = m.named_count();
    Store(base, copy + 28, scene);
    Store(base, copy + 32, animation);
    m.set_named_count(used + 1);
    const schema::UiMovieNamedAnimation entry = NamedEntry(base, movie, used);
    {
      ITC_NATIVE_RUNNING("sub_8211E318 (a named animation copied)");
      require(std_string::AssignFrom(base, entry.name_address(), copy, 0, 0xFFFFFFFFu), "assign");
      entry.set_scene(Load(base, copy + 28));
      entry.set_animation(Load(base, copy + 32));
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
  TextMapSetKey(base, UiMovie(base, movie).texts_address(), Load(base, id_address), *record);
  return UiMovie(base, movie).texts_address();
}

// The same with the node given.
template <typename Change>
std::optional<uint32_t> ChangeNodeText(uint8_t* base, uint32_t movie, uint32_t node,
                                       Change change) {
  const uint32_t id_address = node + 108;
  const auto record = GetText(base, movie, id_address);
  if (!record) {
    return std::nullopt;
  }
  change(*record);
  TextMapSetKey(base, UiMovie(base, movie).texts_address(), Load(base, id_address), *record);
  return UiMovie(base, movie).texts_address();
}

constexpr uint32_t kTxtTag = 0x54585400;  // "TXT\0"

// Whether the record's next page is there: its page (+20) + 1 against its
// buffer's page count, signed.
bool HasNextPage(uint8_t* base, uint32_t record) {
  const uint32_t pages =
      strings_and_font::StringsPageCount(base, UiMovieText(base, record).buffer().address);
  return int32_t(UiMovieText(base, record).page() + 1) <= int32_t(pages);
}

}  // namespace

std::optional<uint32_t> ClearText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                  uint32_t child) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C470 UiMovie::ClearText");
    Zero(base, UiMovieText(base, record).buffer().address, 4096);
    UiMovieText(base, record).set_scroll_counter(0);
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
  Zero(base, UiMovieText(base, *record).buffer().address, 4096);
  UiMovieText(base, *record).set_page(0);
  strings_and_font::StringsFormatV(base, calls, stack - 128, Load(base, kStringTable),
                                   UiMovieText(base, *record).buffer().address, id & 0xFFFF, args);
  TextMapSetKey(base, UiMovie(base, movie).texts_address(), Load(base, id_address), *record);
  return UiMovie(base, movie).texts_address();
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
  Zero(base, UiMovieText(base, *record).buffer().address, 4096);
  UiMovieText(base, *record).set_page(0);
  strings_and_font::StringsFormatV(base, calls, stack - 128, Load(base, kStringTable),
                                   UiMovieText(base, *record).buffer().address, id & 0xFFFF, args);
  TextMapSetKey(base, UiMovie(base, movie).texts_address(), Load(base, id_address), *record);
  return UiMovie(base, movie).texts_address();
}

namespace {
// SetNumberText's and SetNodeNumberText's change: the buffer cleared, then
// string 8 with r6 = 0 when the value's low word is 0, else 9 with the
// value (all 64 bits, as the originals' mr r6); r7 to r10, the memset's
// leftovers in the originals, passed 0 (dead frame words).
void FormatNumber(uint8_t* base, const strings_and_font::FormatCalls& calls, uint32_t stack,
                  uint32_t record, uint64_t value) {
  constexpr uint32_t kStringTable = 0x826C5C34;
  Zero(base, UiMovieText(base, record).buffer().address, 4096);
  const bool zero = uint32_t(value) == 0;
  const uint64_t varargs[5] = {zero ? 0 : value, 0, 0, 0, 0};
  strings_and_font::StringsFormat(base, calls, stack - 128, Load(base, kStringTable),
                                  UiMovieText(base, record).buffer().address, zero ? 8 : 9,
                                  varargs);
}
}  // namespace

std::optional<uint32_t> SetNumberText(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                      uint32_t stack, uint32_t movie, uint32_t sub_scene,
                                      uint32_t child, uint64_t value) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C740 UiMovie::SetNumberText");
    FormatNumber(base, calls, stack, record, value);
  });
}

std::optional<uint32_t> SetNodeNumberText(uint8_t* base,
                                          const strings_and_font::FormatCalls& calls,
                                          uint32_t stack, uint32_t movie, uint32_t node,
                                          uint64_t value) {
  return ChangeNodeText(base, movie, node, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211CA18 UiMovie::SetNodeNumberText");
    FormatNumber(base, calls, stack, record, value);
  });
}

std::optional<uint32_t> SetNodeTextV(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                     uint32_t stack, uint32_t movie, uint32_t node, uint32_t id,
                                     uint32_t args) {
  constexpr uint32_t kStringTable = 0x826C5C34;
  return ChangeNodeText(base, movie, node, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211CBD8 UiMovie::SetNodeTextV");
    Zero(base, UiMovieText(base, record).buffer().address, 4096);
    strings_and_font::StringsFormatV(base, calls, stack - 144, Load(base, kStringTable),
                                     UiMovieText(base, record).buffer().address, id & 0xFFFF, args);
  });
}

std::optional<uint32_t> SetNodeCharText(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                        uint32_t stack, uint32_t movie, uint32_t node,
                                        uint32_t text) {
  constexpr uint32_t kCharsToText = 0x82108A70;  // the 8-bit string's conversion
  constexpr uint32_t kFont = 0x826C5C00;
  if (Load(base, node) != kTxtTag) {
    return std::nullopt;
  }
  return ChangeNodeText(base, movie, node, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C8A0 UiMovie::SetNodeCharText");
    Zero(base, UiMovieText(base, record).buffer().address, 4096);
    calls.call(calls.user, kCharsToText, Load(base, kFont),
               UiMovieText(base, record).buffer().address, text, stack - 128, 0x8211C908);
  });
}

std::optional<uint32_t> SetNameText(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                    uint32_t stack, uint32_t movie, uint32_t node, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_8211CAA0 UiMovie::SetNameText");
  const schema::UiMovieNameRecord record(base, name);
  const auto index = [](uint16_t halfword) { return int32_t(int16_t(halfword)); };
  // The original's checks: below `bound` signed, then the halfword's
  // unsigned value below 32768 (or at least 0, signed), as its branches.
  const auto in_range = [](int32_t i, int32_t bound) { return i < bound && i >= 0; };
  const uint32_t kind = record.kind();
  if (kind == 5) {
    const int32_t i = index(record.index());
    if (!in_range(i, 71)) {
      return movie;
    }
    // r6 to r10 the lookup's leftovers in the original: passed 0.
    const uint64_t varargs[5] = {};
    return SetNodeText(base, calls, stack - 128, movie, node, uint32_t(i + 5837), varargs);
  }
  int32_t first = -1, second = -1;
  if (kind == 0) {
    const int32_t i = index(record.index());
    first = record.flag() != 0 ? i + 5477 : i + 5657;
    if (!in_range(i, 90)) {
      first = -1;
    }
  } else if (kind == 1 || kind == 4) {
    const int32_t i = index(record.index());
    first = i + (kind == 1 ? 5908 : 5463);
    if (!in_range(i, kind == 1 ? 4 : 105)) {
      first = -1;
    }
  }
  const int32_t second_kind = int32_t(record.second_kind());
  if (second_kind == 2) {
    const int32_t i = index(record.second_index());
    second = record.second_flag() != 0 ? i + 5567 : i + 5747;
    if (!in_range(i, 90)) {
      second = -1;
    }
  } else if (second_kind == 3) {
    const int32_t i = index(record.second_index());
    second = i + 5912;
    if (!in_range(i, 6)) {
      second = -1;
    }
  }
  if (first == -1 || second == -1) {
    return movie;
  }
  // String 532 with the two ids; r8 to r10, unset, passed 0.
  const uint64_t varargs[5] = {uint64_t(int64_t(first)), uint64_t(int64_t(second)), 0, 0, 0};
  return SetNodeText(base, calls, stack - 128, movie, node, 532, varargs);
}

std::optional<uint32_t> SetChildNameText(uint8_t* base,
                                         const strings_and_font::FormatCalls& calls,
                                         uint32_t stack, uint32_t movie, uint32_t sub_scene,
                                         uint32_t child, uint32_t name) {
  ITC_NATIVE_RUNNING("sub_8211D930 UiMovie::SetChildNameText");
  const uint32_t node = FindNode(base, movie, sub_scene, child);
  return SetNameText(base, calls, stack - 112, movie, node, name);
}

std::optional<uint32_t> SetTextScrollSpeed(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                           uint32_t child, uint32_t delay, double speed) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C4F0 (a text's scroll delay and speed)");
    UiMovieText(base, record).set_scroll_delay(delay);
    UiMovieText(base, record).set_scroll_speed(float(speed));
  });
}

std::optional<uint32_t> ScrollText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                   uint32_t child, uint32_t scrolling) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C570 UiMovie::ScrollText");
    const uint32_t speed = Globals::scroll_speed(base);
    UiMovieText(base, record).set_scrolling(scrolling);
    UiMovieText(base, record).set_scroll_delay(45);
    UiMovieText(base, record).set_scroll_speed_word(speed);
  });
}

std::optional<uint32_t> ResetTextScroll(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                        uint32_t child) {
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C5F8 (a text's scroll counter reset)");
    UiMovieText(base, record).set_scroll_counter(0);
  });
}

std::optional<uint32_t> SetFirstLine(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                     uint32_t child, uint32_t line) {
  if (Load(base, FindNode(base, movie, sub_scene, child)) != kTxtTag) {
    return std::nullopt;
  }
  return ChangeText(base, movie, sub_scene, child, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211C370 UiMovie::SetFirstLine");
    UiMovieText(base, record).set_first_line(line);
  });
}

std::optional<uint32_t> FirstLine(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                  uint32_t child) {
  const uint32_t node = FindNode(base, movie, sub_scene, child);
  if (Load(base, node) != kTxtTag) {
    return std::nullopt;
  }
  const auto record = GetText(base, movie, node + 108);
  if (!record) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211C3F8 UiMovie::FirstLine");
  return UiMovieText(base, *record).first_line();
}

uint32_t CountLines(uint8_t* base, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_821087D8 Strings::CountLines");
  uint32_t lines = 1;
  for (;; text += 2) {
    const uint16_t c = guest::LoadU16(base, text);
    if (c == 0 || c == 0xFFD1) {
      return lines;
    }
    if (c == 10) {
      lines = (lines + 1) & 0xFFFF;
    }
  }
}

std::optional<uint32_t> LineCount(uint8_t* base, uint32_t stack, uint32_t movie,
                                  uint32_t sub_scene, uint32_t child) {
  const uint32_t id_address = FindNode(base, movie, sub_scene, child) + 108;
  if (Load(base, id_address) == 0) {
    ITC_NATIVE_RUNNING("sub_8211C680 UiMovie::LineCount");
    return 0;
  }
  const uint32_t buffer = stack - 4208 + 80;
  Zero(base, buffer, 4096);
  if (!GetText(base, movie, id_address)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8211C680 UiMovie::LineCount");
  const uint32_t id = Load(base, id_address);
  const uint32_t page = TextField(base, movie, id, 20);
  const uint32_t text = TextString(base, movie, id);
  strings_and_font::StringsCopyPage(base, buffer, text, int32_t(page), 0);
  return CountLines(base, buffer) & 0xFFFF;
}

std::optional<uint32_t> ScrollNodeText(uint8_t* base, uint32_t movie, uint32_t node,
                                       uint32_t scrolling) {
  return ChangeNodeText(base, movie, node, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211CCC0 UiMovie::ScrollNodeText");
    const uint32_t speed = Globals::scroll_speed(base);
    UiMovieText(base, record).set_scrolling(scrolling);
    UiMovieText(base, record).set_scroll_delay(45);
    UiMovieText(base, record).set_scroll_speed_word(speed);
  });
}

std::optional<uint32_t> ResetNodeTextScroll(uint8_t* base, uint32_t movie, uint32_t node) {
  return ChangeNodeText(base, movie, node, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211CD28 UiMovie::ResetNodeTextScroll");
    UiMovieText(base, record).set_scroll_counter(0);
  });
}

std::optional<uint32_t> ClearNodeText(uint8_t* base, uint32_t movie, uint32_t node) {
  return ChangeNodeText(base, movie, node, [&](uint32_t record) {
    ITC_NATIVE_RUNNING("sub_8211CC50 UiMovie::ClearNodeText");
    Zero(base, UiMovieText(base, record).buffer().address, 4096);
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
      UiMovieText(base, record).set_page(UiMovieText(base, record).page() + 1);
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
  if (schema::UiMovieWord(base, PairsFindKey(base, kMovieTable, id)).value() != 0) {
    return 0;
  }
  const uint32_t movie = calls.make();
  Store(base, frame + 80, movie);
  InitAnimationLists(base, movie);
  UiMovie(base, movie).set_id(id);
  MovieLoad(base, movie, asset, load);
  Store(base, frame + 84, id);
  calls.set(frame + 84, frame + 80);
  return 1;
}

uint32_t UnloadMovie(uint8_t* base, uint32_t movie, uint32_t frame, const UnloadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8211B540 UiMovie::Unload");
  const UiMovie m(base, movie);
  for (uint32_t i = 0; i < 3; i++) {
    calls.virtual_free(i, Load(base, m.file_0_address() + 4 * i));
  }
  m.set_live_scene(0);
  m.set_saved_scene(0);
  m.set_file_scene(0);
  const UiMovieWordVector textures(base, m.textures_address());
  while (m.texture_count() != 0) {
    const uint32_t count = textures.count() - 1;
    textures.set_count(count);
    const uint32_t entry = schema::UiMovieWord(base, textures.words() + (count << 2)).value();
    if (entry != 0) {
      Store(base, entry + 0, 0);
      Store(base, entry + 4, 0);
      Store(base, entry + 8, 0);
      calls.free(entry);
    }
  }
  const schema::UiMovieDrawTextList items(base, m.text_items_address());
  while (m.text_item_count() != 0) {
    const uint32_t count = items.count() - 1;
    const uint32_t item = (count << 7) + items.items().address;
    items.set_count(count);
    calls.copy_item(frame + 80, item);
    ClearTextItem(base, frame + 80);
  }
  if (m.texture_set() != 0) {
    calls.release_xpr(m.texture_set_address());
  }
  m.set_texture_set(0);
  m.set_loaded(0);
  return calls.free_texts(movie);
}

uint32_t DeleteMovie(uint8_t* base, uint32_t id, uint32_t frame, const DeleteCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8211E258 UiMovies::Delete");
  Store(base, frame + 80, id);
  const uint32_t entry = PairsFindKey(base, kMovieTable, id);
  const uint32_t movie = schema::UiMovieWord(base, entry).value();
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

uint32_t OnLoaded(uint8_t* base, uint32_t archive, uint32_t movie, uint32_t frame,
                  const OnLoadedCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8211E500 UiMovie::OnLoaded");
  CheckXmd(base, archive);
  const uint32_t file = XmdEntry(base, archive, 0);
  const uint32_t end = FindChunk(base, file, kTagTrm);
  const uint32_t size = Load(base, end + 4) - file + end;
  // The three file copies (+0, +4, +8) and their scenes (+16, +20, +24), by
  // index.
  const UiMovie m(base, movie);
  const uint32_t files = m.file_0_address(), scenes = m.live_scene_address();
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, files + 4 * i, memory::AllocPages(size, 4096));
  }
  for (uint32_t i = 0; i < 3; i++) {
    calls.copy(i, Load(base, files + 4 * i), file, size);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FixUpFile(base, Load(base, files + 4 * i));
  }
  for (uint32_t i = 0; i < 3; i++) {
    Store(base, scenes + 4 * i, FileScene(base, Load(base, files + 4 * i)));
  }
  for (uint32_t i = 0; i < 3; i++) {
    calls.layers(i, movie, Load(base, scenes + 4 * i));
  }
  ClearTextIds(base, movie);
  calls.free_texts(movie);
  m.set_named_count(0);
  calls.add_named(movie, m.live_scene());
  if (calls.get_size(archive) != 1) {
    const uint32_t textures =
        schema::UiMovieXmdEntry(base, XmdEntry(base, archive, 1)).texture_count();
    const uint32_t xpr = calls.make(0, 24);
    if (xpr != 0) {
      calls.xpr_construct(xpr);
    }
    m.set_texture_set(xpr);
    calls.xpr_create(m.texture_set(), XmdEntry(base, archive, 1));
    for (uint32_t i = 0; i < textures; i++) {
      const uint32_t entry = calls.make(1, 12);
      if (entry != 0) {
        Store(base, entry + 0, 0);
        Store(base, entry + 4, 0);
        Store(base, entry + 8, 0);
      }
      Store(base, frame + 80, entry);
      const uint32_t texture = uint32_t(calls.get_texture(m.texture_set(), i));
      Store(base, entry + 0, texture);
      calls.level_desc(texture, frame + 96);
      Store(base, entry + 4, Load(base, frame + 120));
      Store(base, entry + 8, Load(base, frame + 124));
      calls.append(m.textures_address(), frame + 80);
    }
  }
  const uint32_t r3 = calls.virtual_free(archive);
  m.set_loaded(1);
  return r3;
}

uint32_t DeleteAll(uint8_t* base, uint32_t frame, const DeleteCalls& calls, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_8211E2C8 UiMovies::DeleteAll");
  for (uint32_t i = 0; i < Globals::movie_table_count(base); i++) {
    r3 =
        DeleteMovie(base, UiMoviePair(base, Globals::movie_table_pairs(base).address + 8 * i).key(),
                    frame - 128, calls);
  }
  return r3;
}

}  // namespace ui_movies
