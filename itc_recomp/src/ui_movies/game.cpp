#include "ui_movies/game.h"

#include "base/log.h"

#include "ui_movies/ui_movies.h"

#include "c_runtime/strings.h"

#include <optional>

namespace ui_movies {

namespace {
constexpr uint32_t kColors = 0x82008410;  // 12 bytes a color: r, g, b
constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"

void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t first, uint32_t last) {
  for (uint32_t offset = first; offset <= last; offset += 4) {
    guest::StoreU32(base, to + offset, guest::LoadU32(base, from + offset));
  }
}
}  // namespace

uint32_t FindChild(uint8_t* base, uint32_t scene, uint32_t sub_scene, uint32_t child) {
  if (sub_scene != 0xFFFFFFFFu) {
    scene = Child(base, scene, sub_scene);
  }
  return Child(base, scene, child);
}

uint32_t FindNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  return FindChild(base, guest::LoadU32(base, movie + kMovieLive), sub_scene, child);
}

void SetColor(uint8_t* base, uint32_t node, uint32_t color) {
  const uint32_t from = kColors + 12 * color;
  for (uint32_t i = 0; i < 12; i += 4) {
    guest::StoreU32(base, node + 56 + i, guest::LoadU32(base, from + i));
  }
}

uint32_t Layer(uint32_t n) {
  if (n <= 11) {
    const uint32_t layer = (n + 62) * 2;
    return layer < 147 ? layer : 147;
  }
  if (n <= 43) {
    const uint32_t layer = (n + 291) * 2;
    return layer < 667 ? layer : 667;
  }
  return 667;
}

void SetLayer(uint8_t* base, uint32_t node, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8211A660 SetLayer");
  guest::StoreU32(base, node + 72, Layer(n));
}

void ClearTextIds(uint8_t* base, uint32_t movie) {
  uint32_t chunk = guest::LoadU32(base, movie + kMovieLive);
  while (guest::LoadU32(base, chunk) != kTagTrm) {
    chunk += guest::LoadU32(base, chunk + 4);
    if (guest::LoadU32(base, chunk) == kTagTxt) {
      guest::StoreU32(base, chunk + 108, 0);
    }
  }
}

uint32_t SetNodeTexture(uint8_t* base, uint32_t node, uint32_t texture) {
  const uint32_t tag = guest::LoadU32(base, node);
  if (tag == kTagPrm) {
    guest::StoreU32(base, node + 104, texture);
  } else if (tag == kTagPic) {
    guest::StoreU32(base, node + 108, texture);
  }
  return node;
}

uint32_t PictureTextureSlot(uint8_t* base, uint32_t movie, uint32_t picture) {
  const uint32_t entry = PictureTexture(picture, guest::LoadU32(base, picture + 108));
  return guest::LoadU32(base, movie + kMovieTextures) + 4 * guest::LoadU32(base, entry);
}

uint32_t SwapTexture(uint8_t* base, uint32_t movie, uint32_t i, uint32_t texture) {
  const uint32_t slot = guest::LoadU32(base, movie + kMovieTextures) + 4 * i;
  const uint32_t old = guest::LoadU32(base, slot);
  guest::StoreU32(base, slot, texture);
  return old;
}

uint32_t ResetPose(uint8_t* base, uint32_t movie) {
  RestorePose(base, movie, guest::LoadU32(base, movie + kMovieLive),
              guest::LoadU32(base, movie + kMovieFile), 0, 0);
  return RestorePose(base, movie, guest::LoadU32(base, movie + kMovieSaved),
                     guest::LoadU32(base, movie + kMovieFile), 0, 0);
}

uint32_t ResetNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  // The floats among +12..+80 (+36, +68) go through registers (lfs, stfs),
  // which leave their words as they were (checked by the harness's odd
  // floats): every field is copied as a word.
  if (sub_scene != 0xFFFFFFFFu) {
    const uint32_t file_sub = Child(base, guest::LoadU32(base, movie + kMovieFile), sub_scene);
    const uint32_t live_sub = Child(base, guest::LoadU32(base, movie + kMovieLive), sub_scene);
    const uint32_t saved_sub = Child(base, guest::LoadU32(base, movie + kMovieSaved), sub_scene);
    const uint32_t from = Child(base, file_sub, child);
    const uint32_t live = Child(base, live_sub, child);
    const uint32_t saved = Child(base, saved_sub, child);
    CopyWords(base, live, from, 12, 80);
    CopyWords(base, saved, from, 12, 80);
    RestorePose(base, movie, live_sub, file_sub, live, from);
    return RestorePose(base, movie, saved_sub, file_sub, saved, from);
  }
  const uint32_t from = Child(base, guest::LoadU32(base, movie + kMovieFile), child);
  const uint32_t live = Child(base, guest::LoadU32(base, movie + kMovieLive), child);
  const uint32_t saved = Child(base, guest::LoadU32(base, movie + kMovieSaved), child);
  CopyWords(base, live, from, 12, 80);
  CopyWords(base, saved, from, 12, 80);
  RestorePose(base, movie, guest::LoadU32(base, movie + kMovieLive),
              guest::LoadU32(base, movie + kMovieFile), live, from);
  return RestorePose(base, movie, guest::LoadU32(base, movie + kMovieSaved),
                     guest::LoadU32(base, movie + kMovieFile), saved, from);
}

namespace {
constexpr uint32_t kZero = 0x82000A34;  // the float 0.0

void StoreIdentity(uint8_t* base, uint32_t matrix) {
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(base, matrix + 16 * row + 4 * column, row == column ? kOneBits : 0);
    }
  }
}
}  // namespace

uint32_t MakeQuad(uint8_t* base, uint32_t quad) {
  StoreIdentity(base, quad);
  guest::StoreU32(base, quad + 64, 0);
  guest::StoreU32(base, quad + 68, 0);
  guest::StoreU32(base, quad + 72, 2);
  return quad;
}

uint32_t MakeTextItem(uint8_t* base, uint32_t item) {
  const uint32_t zero = guest::LoadU32(base, kZero);
  guest::StoreU32(base, item + 80, zero);
  guest::StoreU32(base, item, 0);
  for (uint32_t offset = 84; offset <= 96; offset += 4) {
    guest::StoreU32(base, item + offset, zero);
  }
  StoreIdentity(base, item + 16);
  return item;
}

uint32_t ClearTextItem(uint8_t* base, uint32_t item) {
  guest::StoreU32(base, item + 80, guest::LoadU32(base, kZero));
  guest::StoreU32(base, item, 0);
  StoreIdentity(base, item + 16);
  return item + 16;
}

bool CopyTextItem(uint8_t* base, uint32_t to, uint32_t from) {
  if (c_runtime::MayShare(to, 116, from, 116)) {
    return false;
  }
  guest::StoreU32(base, to, guest::LoadU32(base, from));
  CopyWords(base, to, from, 16, 112);
  return true;
}

void InitAnimationLists(uint8_t* base, uint32_t movie) {
  for (uint32_t offset = 0; offset <= 24; offset += 4) {
    guest::StoreU32(base, movie + offset, 0);
  }
  guest::StoreU32(base, movie + kMovieLoaded, 0);
  for (uint32_t list : {movie + kMovieQueue, movie + kMovieLooping}) {
    for (uint32_t k = 0; k < 32; k++) {
      guest::StoreU16(base, list + 4 * k + 2, uint16_t(k));
      guest::StoreU16(base, list + 4 * k, uint16_t(k + 1));
    }
    guest::StoreU16(base, list + 124, 0xFFFF);
    guest::StoreU16(base, list + 128, 0);
    guest::StoreU16(base, list + 130, 0xFFFF);
    guest::StoreU16(base, list + 132, 0xFFFF);
    guest::StoreU16(base, list + 134, 0xFFFF);
    guest::StoreU16(base, list + 136, 0);
  }
}

namespace {
// A link's address from its 16-bit index (sign-extended, as the original's
// lha).
uint32_t Link(uint32_t list, uint16_t index) {
  return list + 4 * uint32_t(int32_t(int16_t(index)));
}

// The link before position `position` (the list's first field, +132, for
// 0): where a link is put in or taken out.
uint32_t Before(uint8_t* base, uint32_t list, uint32_t position) {
  return position == 0 ? list + 132 : LinkAt(base, list, position - 1);
}
}  // namespace

uint32_t LinkAt(uint8_t* base, uint32_t list, uint32_t steps) {
  uint32_t link = Link(list, guest::LoadU16(base, list + 132));
  for (; steps != 0; steps--) {
    link = Link(list, guest::LoadU16(base, link));
  }
  return link;
}

uint32_t SlotOf(uint8_t* base, uint32_t list, uint32_t link) {
  return list + 140 + 40 * uint32_t(int32_t(int16_t(guest::LoadU16(base, link + 2))));
}

void LinkFree(uint8_t* base, uint32_t list, int32_t position) {
  ITC_NATIVE_RUNNING("sub_8211B230 LinkFree");
  const uint32_t before = Before(base, list, uint32_t(position));
  const uint16_t free = guest::LoadU16(base, list + 128);
  const uint32_t link = Link(list, free);
  guest::StoreU16(base, list + 128, guest::LoadU16(base, link));
  guest::StoreU16(base, link, free);
  guest::StoreU16(base, link, guest::LoadU16(base, before));
  guest::StoreU16(base, before, free);
  guest::StoreU16(base, list + 136, uint16_t(guest::LoadU16(base, list + 136) + 1));
}

void RemoveAt(uint8_t* base, uint32_t list, uint32_t position) {
  const uint32_t before = Before(base, list, position);
  const uint16_t index = guest::LoadU16(base, before);
  const uint32_t link = Link(list, index);
  const uint16_t next = guest::LoadU16(base, link);
  guest::StoreU16(base, link, index);
  guest::StoreU16(base, before, next);
  const uint16_t freed = guest::LoadU16(base, link);
  const uint16_t free = guest::LoadU16(base, list + 128);
  guest::StoreU16(base, list + 128, freed);
  guest::StoreU16(base, link, free);
  guest::StoreU16(base, list + 136, uint16_t(guest::LoadU16(base, list + 136) - 1));
}

namespace {
constexpr uint32_t kNoTime = 0x82001BE0;

// The first slot of `list` that `match` accepts.
template <typename Match>
std::optional<uint32_t> FindSlot(uint8_t* base, uint32_t list, Match match) {
  const uint32_t count = uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 136))));
  uint32_t link = Link(list, guest::LoadU16(base, list + 132));
  for (uint32_t i = 0; i < count; i++) {
    if (i != 0) {
      link = Link(list, guest::LoadU16(base, link));
    }
    const uint32_t slot = SlotOf(base, list, link);
    if (match(slot)) {
      return slot;
    }
  }
  return std::nullopt;
}

template <typename Match>
uint32_t FindTime(uint8_t* base, uint32_t movie, Match match) {
  for (uint32_t list : {movie + kMovieLooping, movie + kMovieQueue}) {
    if (const std::optional<uint32_t> slot = FindSlot(base, list, match)) {
      return guest::LoadU32(base, *slot + 20);
    }
  }
  return guest::LoadU32(base, kNoTime);
}
}  // namespace

uint32_t AnimationTime(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index) {
  return FindTime(base, movie, [&](uint32_t slot) {
    return guest::LoadU32(base, slot + 4) == sub_scene && guest::LoadU32(base, slot) == index;
  });
}

uint32_t NamedAnimationTime(uint8_t* base, uint32_t movie, uint32_t index) {
  return FindTime(base, movie,
                  [&](uint32_t slot) { return guest::LoadU32(base, slot + 8) == index; });
}

Slot LoadSlot(uint8_t* base, uint32_t address) {
  Slot slot;
  for (uint32_t k = 0; k < slot.size(); k++) {
    slot[k] = guest::LoadU32(base, address + 4 * k);
  }
  return slot;
}

void StoreSlot(uint8_t* base, uint32_t address, const Slot& slot) {
  for (uint32_t k = 0; k < slot.size(); k++) {
    guest::StoreU32(base, address + 4 * k, slot[k]);
  }
}

uint32_t Append(uint8_t* base, uint32_t list, const Slot& slot) {
  const uint32_t position = uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 136))));
  LinkFree(base, list, int16_t(uint16_t(position)));
  const uint32_t to = SlotOf(base, list, LinkAt(base, list, position));
  StoreSlot(base, to, slot);
  return to;
}

namespace {
// The original's loop: each slot copied out by position (AnimationList::Get)
// and tested; the first match removed and appended to the other list.
template <typename Match>
Moved Move(uint8_t* base, uint32_t movie, bool to_looping, Match match) {
  const uint32_t from = movie + (to_looping ? kMovieQueue : kMovieLooping);
  const uint32_t to = movie + (to_looping ? kMovieLooping : kMovieQueue);
  const uint32_t count = uint32_t(int32_t(int16_t(guest::LoadU16(base, from + 136))));
  Moved moved;
  for (uint32_t i = 0; i < count; i++) {
    moved.looked = true;
    const Slot slot = LoadSlot(base, SlotOf(base, from, LinkAt(base, from, i)));
    if (match(slot)) {
      RemoveAt(base, from, i);
      moved.slot = Append(base, to, slot);
      return moved;
    }
  }
  return moved;
}
}  // namespace

Moved MoveAnimation(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index,
                    bool to_looping) {
  ITC_NATIVE_RUNNING("sub_8211DBE0 UiMovie::MoveAnimation");
  return Move(base, movie, to_looping,
              [&](const Slot& slot) { return slot[1] == sub_scene && slot[0] == index; });
}

Moved MoveNamedAnimation(uint8_t* base, uint32_t movie, uint32_t index, bool to_looping) {
  ITC_NATIVE_RUNNING("sub_8211DCE8 UiMovie::MoveNamedAnimation");
  return Move(base, movie, to_looping, [&](const Slot& slot) { return slot[2] == index; });
}

namespace {

// The position of the first slot of `list` that `match` accepts (by its
// words), walked as FindSlot does.
template <typename Match>
std::optional<uint32_t> FindPosition(uint8_t* base, uint32_t list, Match match) {
  const uint32_t count = uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 136))));
  for (uint32_t i = 0; i < count; i++) {
    if (match(SlotOf(base, list, LinkAt(base, list, i)))) {
      return i;
    }
  }
  return std::nullopt;
}

// The start shared by Play and PlayNamed: the slot appended to the queue
// and copied to the last started. Returns the latter's address. The time's
// word is copied as it is: the original's lfs and stfs leave it so, a
// signaling NaN too (as the recompiled code does, checked by the harness).
uint32_t Start(uint8_t* base, uint32_t movie, Slot slot, uint32_t animation) {
  slot[5] = guest::LoadU32(base, slot[7] != 0 ? animation + 12 : kZero);
  Append(base, movie + kMovieQueue, slot);
  StoreSlot(base, movie + kMovieLastStarted, slot);
  return movie + kMovieLastStarted;
}
}  // namespace

uint32_t Play(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index,
              uint32_t chained, uint32_t reversed, uint32_t w32, uint32_t paused) {
  uint32_t scene = guest::LoadU32(base, movie + kMovieLive);
  if (sub_scene != 0xFFFFFFFFu) {
    scene = Child(base, scene, sub_scene);
  }
  const uint32_t animation = SceneAnimation(base, scene, index);
  if (FindPosition(base, movie + kMovieLooping, [&](uint32_t slot) {
        return guest::LoadU32(base, slot + 4) == sub_scene && guest::LoadU32(base, slot) == index;
      })) {
    return animation;
  }
  return Start(base, movie,
               {index, sub_scene, 0xFFFFFFFFu, scene, animation, 0, chained, reversed, w32, paused},
               animation);
}

uint32_t PlayNamed(uint8_t* base, uint32_t movie, uint32_t named, uint32_t chained,
                   uint32_t reversed, uint32_t w32, uint32_t paused) {
  if (int32_t(named) < 0) {
    return movie;
  }
  if (const std::optional<uint32_t> i = FindPosition(base, movie + kMovieLooping, [&](uint32_t slot) {
        return guest::LoadU32(base, slot + 8) == named;
      })) {
    return *i;
  }
  const uint32_t entry = movie + 36 * named;
  const uint32_t animation = guest::LoadU32(base, entry + 208);
  return Start(base, movie,
               {0xFFFFFFFFu, 0xFFFFFFFFu, named, guest::LoadU32(base, entry + 204), animation, 0,
                chained, reversed, w32, paused},
               animation);
}

Moved LoopLast(uint8_t* base, uint32_t movie) {
  const uint32_t named = guest::LoadU32(base, movie + kMovieLastStarted + 8);
  if (named != 0xFFFFFFFFu) {
    return MoveNamedAnimation(base, movie, named, true);
  }
  return MoveAnimation(base, movie, guest::LoadU32(base, movie + kMovieLastStarted + 4),
                       guest::LoadU32(base, movie + kMovieLastStarted), true);
}

uint32_t FinishTime(uint8_t* base, uint32_t slot) {
  const uint32_t reversed = guest::LoadU32(base, slot + 28);
  return guest::LoadU32(base, reversed != 0 ? kZero : guest::LoadU32(base, slot + 16) + 12);
}

namespace {
template <typename Match>
std::optional<Finishing> FindQueued(uint8_t* base, uint32_t movie, Match match) {
  const uint32_t queue = movie + kMovieQueue;
  const std::optional<uint32_t> position = FindPosition(base, queue, match);
  if (!position) {
    return std::nullopt;
  }
  const uint32_t slot = SlotOf(base, queue, LinkAt(base, queue, *position));
  return Finishing{*position, slot, FinishTime(base, slot)};
}
}  // namespace

std::optional<Finishing> FindFinishing(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                       uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211DE38 UiMovie::Finish");
  return FindQueued(base, movie, [&](uint32_t slot) {
    return guest::LoadU32(base, slot + 4) == sub_scene && guest::LoadU32(base, slot) == index;
  });
}

std::optional<Finishing> FindFinishingNamed(uint8_t* base, uint32_t movie, uint32_t named) {
  ITC_NATIVE_RUNNING("sub_8211DF20 UiMovie::FinishNamed");
  return FindQueued(base, movie,
                    [&](uint32_t slot) { return guest::LoadU32(base, slot + 8) == named; });
}

namespace {
constexpr uint32_t kTextMapNone = 0x826C5CD8;
constexpr uint32_t kPairsNone = 0x826C5CDC;

// The index of the first pair keyed `key` among `count` from `pairs`.
std::optional<uint32_t> FindKey(uint8_t* base, uint32_t pairs, uint32_t count, uint32_t key) {
  for (uint32_t i = 0; i < count; i++) {
    if (guest::LoadU32(base, pairs + 8 * i) == key) {
      return i;
    }
  }
  return std::nullopt;
}
}  // namespace

uint32_t TextMapFindKey(uint8_t* base, uint32_t map, uint32_t key) {
  const uint32_t count = guest::LoadU32(base, map + 1024);
  if (count == 0) {
    return kTextMapNone;
  }
  const std::optional<uint32_t> i = FindKey(base, map, count, key);
  return i ? map + 8 * *i + 4 : kTextMapNone;
}

uint32_t TextMapFind(uint8_t* base, uint32_t map, uint32_t key_address) {
  return TextMapFindKey(base, map, guest::LoadU32(base, key_address));
}

void TextMapSetKey(uint8_t* base, uint32_t map, uint32_t key, uint32_t value) {
  const uint32_t count = guest::LoadU32(base, map + 1024);
  if (count != 0) {
    if (const std::optional<uint32_t> i = FindKey(base, map, count, key)) {
      guest::StoreU32(base, map + 8 * *i + 4, value);
      return;
    }
  }
  guest::StoreU32(base, map + 1024, count + 1);
  guest::StoreU32(base, map + 8 * count, key);
  guest::StoreU32(base, map + 8 * count + 4, value);
}

void TextMapSet(uint8_t* base, uint32_t map, uint32_t key_address, uint32_t value_address) {
  // Both read before any store, as the original's paths do (the found one
  // reads the value just before its only store).
  TextMapSetKey(base, map, guest::LoadU32(base, key_address),
                guest::LoadU32(base, value_address));
}

uint32_t PairsFindKey(uint8_t* base, uint32_t vector, uint32_t key) {
  const uint32_t count = guest::LoadU32(base, vector + 8);
  if (count == 0) {
    return kPairsNone;
  }
  const std::optional<uint32_t> i = FindKey(base, guest::LoadU32(base, vector), count, key);
  return i ? guest::LoadU32(base, vector) + 8 * *i + 4 : kPairsNone;
}

uint32_t PairsFind(uint8_t* base, uint32_t vector, uint32_t key_address) {
  return PairsFindKey(base, vector, guest::LoadU32(base, key_address));
}

uint32_t IsLoaded(uint8_t* base, uint32_t id) {
  const uint32_t movie = guest::LoadU32(base, PairsFindKey(base, kMovieTable, id));
  return guest::LoadU32(base, movie + kMovieLoaded) != 0 ? 1 : 0;
}

namespace {
constexpr uint32_t kEmptyString = 0x82008434;

// A text's record ([the value TextMapFindKey gives]; 0 for none).
uint32_t TextRecord(uint8_t* base, uint32_t movie, uint32_t id) {
  return guest::LoadU32(base, TextMapFindKey(base, movie + kMovieTexts, id));
}
}  // namespace

uint32_t TextString(uint8_t* base, uint32_t movie, uint32_t id) {
  const uint32_t text = TextRecord(base, movie, id);
  return text == 0 ? kEmptyString : guest::LoadU32(base, text);
}

uint32_t TextField(uint8_t* base, uint32_t movie, uint32_t id, uint32_t offset) {
  // One print per native function sharing this body (sub_8211B770's +20
  // still has its adapter).
  if (offset == 4) ITC_NATIVE_RUNNING("sub_8211B9B8 (a text's +4)");
  if (offset == 24) ITC_NATIVE_RUNNING("sub_8211B7B8 (a text's +24)");
  const uint32_t text = TextRecord(base, movie, id);
  return text == 0 ? 0 : guest::LoadU32(base, text + offset);
}

void AdvanceTextScroll(uint8_t* base, uint32_t movie, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8211B940 (a text's scroll counter)");
  const uint32_t text = TextRecord(base, movie, id);
  guest::StoreU32(base, text + 8, guest::LoadU32(base, text + 8) + 1);
  TextMapSetKey(base, movie + kMovieTexts, id, text);
}

void ClearScreenFlag(uint8_t* base) {
  guest::StoreU32(base, 0x826C5CB0, 0);
}

void ResetMovieTable(uint8_t* base, uint32_t renderer, uint32_t list) {
  SetRenderer(base, renderer, list);
  for (uint32_t i = 0; i < guest::LoadU32(base, kMovieTable + 8); i++) {
    guest::StoreU32(base, guest::LoadU32(base, kMovieTable) + 8 * i, 0xFFFFFFFFu);
    guest::StoreU32(base, guest::LoadU32(base, kMovieTable) + 8 * i + 4, 0);
  }
  for (uint32_t global : {0x826C5CA4u, 0x826C5CACu, 0x826C5CA8u, 0x826C5CC0u, 0x826C5CB4u,
                          0x826C5CB8u, 0x826C5CBCu}) {
    guest::StoreU32(base, global, 0);
  }
}

namespace {
constexpr uint32_t kTagGrp = 0x47525000;  // "GRP\0"
constexpr uint32_t kLayersFrame = 16800;
constexpr uint32_t kBuckets = 64, kBucketBytes = 260;

// Calls `visit(child)` for a group's children, last first, as the original
// walks them (the first index read again each time).
template <typename Visit>
void ForChildren(uint8_t* base, uint32_t scene, uint32_t group, Visit visit) {
  for (int32_t i = int32_t(guest::LoadU32(base, group + 104) + guest::LoadU32(base, group + 100) - 1);
       i >= int32_t(guest::LoadU32(base, group + 100)); i--) {
    if (!visit(Child(base, scene, uint32_t(i)))) {
      return;
    }
  }
}

bool LayersFitAt(uint8_t* base, uint32_t scene, uint32_t group, uint32_t depth) {
  if (depth > 32) {
    return false;
  }
  const int64_t last =
      int32_t(guest::LoadU32(base, group + 104) + guest::LoadU32(base, group + 100) - 1);
  if (last - int64_t(int32_t(guest::LoadU32(base, group + 100))) + 1 > int64_t(kBuckets)) {
    return false;
  }
  bool fits = true;
  ForChildren(base, scene, group, [&](uint32_t child) {
    const uint32_t tag = guest::LoadU32(base, child);
    if (int32_t(guest::LoadU32(base, child + 72)) < 0 ||
        (tag == kTagGrp && !LayersFitAt(base, scene, child, depth + 1)) ||
        (tag == kTagSce && !LayersFitAt(base, child, child, depth + 1))) {
      fits = false;
    }
    return fits;
  });
  return fits;
}
}  // namespace

bool LayersFit(uint8_t* base, uint32_t scene, uint32_t group) {
  return LayersFitAt(base, scene, group, 0);
}

uint32_t AssignLayers(uint8_t* base, uint32_t stack, uint32_t movie, uint32_t scene,
                      uint32_t group, uint32_t layer, bool by_bucket) {
  const uint32_t frame = stack - kLayersFrame;
  const uint32_t buckets = frame + 80;
  for (uint32_t b = 0; b < kBuckets; b++) {
    guest::StoreU32(base, buckets + kBucketBytes * b + 256, 0);
  }
  uint32_t result = movie;
  ForChildren(base, scene, group, [&](uint32_t child) {
    result = child;
    const int32_t offset = int32_t(guest::LoadU32(base, child + 72));
    const uint32_t bucket = buckets + kBucketBytes * uint32_t(offset < 63 ? offset : 63);
    const uint32_t count = guest::LoadU32(base, bucket + 256);
    guest::StoreU32(base, bucket + 256, count + 1);
    guest::StoreU32(base, bucket + 4 * count, child);
    return true;
  });
  for (uint32_t b = 0; b < kBuckets; b++) {
    const uint32_t bucket = buckets + kBucketBytes * b;
    const uint32_t count = guest::LoadU32(base, bucket + 256);
    for (uint32_t j = 0; j < count; j++) {
      const uint32_t node = guest::LoadU32(base, bucket + 4 * j);
      if (by_bucket) {
        layer = b;
      }
      switch (guest::LoadU32(base, node)) {
        case kTagGrp:
          result = AssignLayers(base, frame, movie, scene, node, layer, false);
          break;
        case kTagSce:
          result = AssignLayers(base, frame, movie, node, node, layer, false);
          break;
        case kTagPic:
        case kTagPrm:
        case kTagTxt:
          SetLayer(base, node, layer);
          result = movie;
          break;
        default:
          break;
      }
    }
  }
  return result;
}

}  // namespace ui_movies
