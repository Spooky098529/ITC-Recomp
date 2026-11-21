#include "ui_movies/game.h"

#include "base/log.h"
#include "memory/heap.h"

#include "ui_movies/ui_movies.h"

#include "c_runtime/strings.h"
#include "schema/ui_movies_game.h"

#include <optional>

namespace ui_movies {

namespace {
using schema::UiMovie;
using schema::UiMovieAnimationLink;
using schema::UiMovieAnimationList;
using schema::UiMovieAnimationSlot;
using Globals = schema::UiMoviesGameGlobals;

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
  return FindChild(base, UiMovie(base, movie).live_scene(), sub_scene, child);
}

void SetColor(uint8_t* base, uint32_t node, uint32_t color) {
  const uint32_t from = Globals::colors_address() + 12 * color;
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
  uint32_t chunk = UiMovie(base, movie).live_scene();
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
  return UiMovie(base, movie).textures() + 4 * schema::UiMoviePictureTexture(base, entry).texture();
}

uint32_t SwapTexture(uint8_t* base, uint32_t movie, uint32_t i, uint32_t texture) {
  const schema::UiMovieWord slot(base, UiMovie(base, movie).textures() + 4 * i);
  const uint32_t old = slot.value();
  slot.set_value(texture);
  return old;
}

uint32_t ResetPose(uint8_t* base, uint32_t movie) {
  const UiMovie m(base, movie);
  RestorePose(base, movie, m.live_scene(), m.file_scene(), 0, 0);
  return RestorePose(base, movie, m.saved_scene(), m.file_scene(), 0, 0);
}

uint32_t ResetNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  // The floats among +12..+80 (+36, +68) go through registers (lfs, stfs),
  // which leave their words as they were (checked by the harness's odd
  // floats): every field is copied as a word.
  const UiMovie m(base, movie);
  if (sub_scene != 0xFFFFFFFFu) {
    const uint32_t file_sub = Child(base, m.file_scene(), sub_scene);
    const uint32_t live_sub = Child(base, m.live_scene(), sub_scene);
    const uint32_t saved_sub = Child(base, m.saved_scene(), sub_scene);
    const uint32_t from = Child(base, file_sub, child);
    const uint32_t live = Child(base, live_sub, child);
    const uint32_t saved = Child(base, saved_sub, child);
    CopyWords(base, live, from, 12, 80);
    CopyWords(base, saved, from, 12, 80);
    RestorePose(base, movie, live_sub, file_sub, live, from);
    return RestorePose(base, movie, saved_sub, file_sub, saved, from);
  }
  const uint32_t from = Child(base, m.file_scene(), child);
  const uint32_t live = Child(base, m.live_scene(), child);
  const uint32_t saved = Child(base, m.saved_scene(), child);
  CopyWords(base, live, from, 12, 80);
  CopyWords(base, saved, from, 12, 80);
  RestorePose(base, movie, m.live_scene(), m.file_scene(), live, from);
  return RestorePose(base, movie, m.saved_scene(), m.file_scene(), saved, from);
}

namespace {
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
  const schema::UiMovieQuad q(base, quad);
  StoreIdentity(base, q.matrix_address());
  q.set_unknown_64(0);
  q.set_blend_mode(0);
  q.set_unknown_72(2);
  return quad;
}

uint32_t MakeTextItem(uint8_t* base, uint32_t item) {
  const schema::UiMovieTextItem t(base, item);
  const uint32_t zero = Globals::zero(base);
  t.set_depth(zero);
  t.set_node(0);
  t.set_red(zero);
  t.set_green(zero);
  t.set_blue(zero);
  t.set_alpha(zero);
  StoreIdentity(base, t.matrix_address());
  return item;
}

uint32_t ClearTextItem(uint8_t* base, uint32_t item) {
  const schema::UiMovieTextItem t(base, item);
  t.set_depth(Globals::zero(base));
  t.set_node(0);
  StoreIdentity(base, t.matrix_address());
  return t.matrix_address();
}

bool CopyTextItem(uint8_t* base, uint32_t to, uint32_t from) {
  if (c_runtime::MayShare(to, 116, from, 116)) {
    return false;
  }
  const schema::UiMovieTextItem t(base, to), f(base, from);
  t.set_node(f.node());
  // The matrix (+16) to the clip rectangle's last word (+112), word by word.
  CopyWords(base, to, from, 16, 112);
  return true;
}

void InitAnimationLists(uint8_t* base, uint32_t movie) {
  const UiMovie m(base, movie);
  m.set_file_0(0);
  m.set_file_1(0);
  m.set_file_2(0);
  m.set_texture_set(0);
  m.set_live_scene(0);
  m.set_saved_scene(0);
  m.set_file_scene(0);
  m.set_loaded(0);
  for (uint32_t address : {m.queue_address(), m.looping_address()}) {
    const UiMovieAnimationList list(base, address);
    const uint32_t links = list.links_address();
    for (uint32_t k = 0; k < 32; k++) {
      const UiMovieAnimationLink link(base, links + 4 * k);
      link.set_slot(uint16_t(k));
      link.set_next(uint16_t(k + 1));
    }
    UiMovieAnimationLink(base, links + 4 * 31).set_next(0xFFFF);
    list.set_free(0);
    list.set_unknown_130(0xFFFF);
    list.set_first(0xFFFF);
    list.set_unknown_134(0xFFFF);
    list.set_count(0);
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
  return position == 0 ? UiMovieAnimationList(base, list).first_address()
                       : LinkAt(base, list, position - 1);
}

// The list's count, sign-extended (the original's lha).
uint32_t CountOf(uint8_t* base, uint32_t list) {
  return uint32_t(int32_t(int16_t(UiMovieAnimationList(base, list).count())));
}
}  // namespace

uint32_t LinkAt(uint8_t* base, uint32_t list, uint32_t steps) {
  uint32_t link = Link(list, UiMovieAnimationList(base, list).first());
  for (; steps != 0; steps--) {
    link = Link(list, UiMovieAnimationLink(base, link).next());
  }
  return link;
}

uint32_t SlotOf(uint8_t* base, uint32_t list, uint32_t link) {
  return UiMovieAnimationList(base, list).slots_address() +
         40 * uint32_t(int32_t(int16_t(UiMovieAnimationLink(base, link).slot())));
}

void LinkFree(uint8_t* base, uint32_t list, int32_t position) {
  ITC_NATIVE_RUNNING("sub_8211B230 LinkFree");
  const UiMovieAnimationList l(base, list);
  const uint32_t before = Before(base, list, uint32_t(position));
  const uint16_t free = l.free();
  const UiMovieAnimationLink link(base, Link(list, free));
  l.set_free(link.next());
  link.set_next(free);
  link.set_next(UiMovieAnimationLink(base, before).next());
  UiMovieAnimationLink(base, before).set_next(free);
  l.set_count(uint16_t(l.count() + 1));
}

void RemoveAt(uint8_t* base, uint32_t list, uint32_t position) {
  const uint32_t before = Before(base, list, position);
  const UiMovieAnimationLink first(base, before);
  const uint16_t index = first.next();
  const UiMovieAnimationLink link(base, Link(list, index));
  const uint16_t next = link.next();
  link.set_next(index);
  first.set_next(next);
  const UiMovieAnimationList l(base, list);
  const uint16_t freed = link.next();
  const uint16_t free = l.free();
  l.set_free(freed);
  link.set_next(free);
  l.set_count(uint16_t(l.count() - 1));
}

namespace {
// The first slot of `list` that `match` accepts.
template <typename Match>
std::optional<uint32_t> FindSlot(uint8_t* base, uint32_t list, Match match) {
  const uint32_t count = CountOf(base, list);
  uint32_t link = Link(list, UiMovieAnimationList(base, list).first());
  for (uint32_t i = 0; i < count; i++) {
    if (i != 0) {
      link = Link(list, UiMovieAnimationLink(base, link).next());
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
  const UiMovie m(base, movie);
  for (uint32_t list : {m.looping_address(), m.queue_address()}) {
    if (const std::optional<uint32_t> slot = FindSlot(base, list, match)) {
      return UiMovieAnimationSlot(base, *slot).time();
    }
  }
  return Globals::no_time(base);
}
}  // namespace

uint32_t AnimationTime(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index) {
  return FindTime(base, movie, [&](uint32_t slot) {
    const UiMovieAnimationSlot s(base, slot);
    return s.sub_scene() == sub_scene && s.index() == index;
  });
}

uint32_t NamedAnimationTime(uint8_t* base, uint32_t movie, uint32_t index) {
  return FindTime(base, movie,
                  [&](uint32_t slot) { return UiMovieAnimationSlot(base, slot).named() == index; });
}

uint32_t IsNamedActive(uint8_t* base, uint32_t movie, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211BC18 UiMovie::IsNamedActive");
  const UiMovie m(base, movie);
  for (uint32_t list : {m.looping_address(), m.queue_address()}) {
    if (FindSlot(base, list, [&](uint32_t slot) {
          return UiMovieAnimationSlot(base, slot).named() == index;
        })) {
      return 1;
    }
  }
  return 0;
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
  const uint32_t position = CountOf(base, list);
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
  const UiMovie m(base, movie);
  const uint32_t from = to_looping ? m.queue_address() : m.looping_address();
  const uint32_t to = to_looping ? m.looping_address() : m.queue_address();
  const uint32_t count = CountOf(base, from);
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

Moved UnloopNamed(uint8_t* base, uint32_t movie, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211DE30 UiMovie::UnloopNamed");
  return MoveNamedAnimation(base, movie, index, false);
}

namespace {

// The position of the first slot of `list` that `match` accepts (by its
// words), walked as FindSlot does.
template <typename Match>
std::optional<uint32_t> FindPosition(uint8_t* base, uint32_t list, Match match) {
  const uint32_t count = CountOf(base, list);
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
  const UiMovie m(base, movie);
  slot[5] = guest::LoadU32(
      base, slot[7] != 0 ? schema::UiMovieFileAnimation(base, animation).length_address()
                         : Globals::zero_address());
  Append(base, m.queue_address(), slot);
  StoreSlot(base, m.last_started_address(), slot);
  return m.last_started_address();
}
}  // namespace

uint32_t Play(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index,
              uint32_t chained, uint32_t reversed, uint32_t w32, uint32_t paused) {
  const UiMovie m(base, movie);
  uint32_t scene = m.live_scene();
  if (sub_scene != 0xFFFFFFFFu) {
    scene = Child(base, scene, sub_scene);
  }
  const uint32_t animation = SceneAnimation(base, scene, index);
  if (FindPosition(base, m.looping_address(), [&](uint32_t slot) {
        const UiMovieAnimationSlot s(base, slot);
        return s.sub_scene() == sub_scene && s.index() == index;
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
  const UiMovie m(base, movie);
  if (const std::optional<uint32_t> i = FindPosition(base, m.looping_address(), [&](uint32_t slot) {
        return UiMovieAnimationSlot(base, slot).named() == named;
      })) {
    return *i;
  }
  const schema::UiMovieNamedAnimation entry(base, m.named_animations_address() + 36 * named);
  const uint32_t animation = entry.animation();
  return Start(base, movie,
               {0xFFFFFFFFu, 0xFFFFFFFFu, named, entry.scene(), animation, 0, chained, reversed,
                w32, paused},
               animation);
}

Moved LoopLast(uint8_t* base, uint32_t movie) {
  const UiMovieAnimationSlot last(base, UiMovie(base, movie).last_started_address());
  const uint32_t named = last.named();
  if (named != 0xFFFFFFFFu) {
    return MoveNamedAnimation(base, movie, named, true);
  }
  return MoveAnimation(base, movie, last.sub_scene(), last.index(), true);
}

uint32_t FinishTime(uint8_t* base, uint32_t slot) {
  const UiMovieAnimationSlot s(base, slot);
  const uint32_t reversed = s.reversed();
  return guest::LoadU32(
      base, reversed != 0 ? Globals::zero_address()
                          : schema::UiMovieFileAnimation(base, s.animation()).length_address());
}

namespace {
template <typename Match>
std::optional<Finishing> FindQueued(uint8_t* base, uint32_t movie, Match match) {
  const uint32_t queue = UiMovie(base, movie).queue_address();
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
    const UiMovieAnimationSlot s(base, slot);
    return s.sub_scene() == sub_scene && s.index() == index;
  });
}

std::optional<Finishing> FindFinishingNamed(uint8_t* base, uint32_t movie, uint32_t named) {
  ITC_NATIVE_RUNNING("sub_8211DF20 UiMovie::FinishNamed");
  return FindQueued(base, movie, [&](uint32_t slot) {
    return UiMovieAnimationSlot(base, slot).named() == named;
  });
}

namespace {
// The index of the first pair keyed `key` among `count` from `pairs`.
std::optional<uint32_t> FindKey(uint8_t* base, uint32_t pairs, uint32_t count, uint32_t key) {
  for (uint32_t i = 0; i < count; i++) {
    if (schema::UiMoviePair(base, pairs + 8 * i).key() == key) {
      return i;
    }
  }
  return std::nullopt;
}
}  // namespace

uint32_t TextMapFindKey(uint8_t* base, uint32_t map, uint32_t key) {
  const uint32_t count = schema::UiMovieTextMap(base, map).count();
  if (count == 0) {
    return Globals::text_none_address();
  }
  const std::optional<uint32_t> i = FindKey(base, map, count, key);
  return i ? schema::UiMoviePair(base, map + 8 * *i).value_address() : Globals::text_none_address();
}

uint32_t TextMapFind(uint8_t* base, uint32_t map, uint32_t key_address) {
  return TextMapFindKey(base, map, schema::UiMovieWord(base, key_address).value());
}

void TextMapSetKey(uint8_t* base, uint32_t map, uint32_t key, uint32_t value) {
  const schema::UiMovieTextMap texts(base, map);
  const uint32_t count = texts.count();
  if (count != 0) {
    if (const std::optional<uint32_t> i = FindKey(base, map, count, key)) {
      schema::UiMoviePair(base, map + 8 * *i).set_value(value);
      return;
    }
  }
  texts.set_count(count + 1);
  const schema::UiMoviePair pair(base, map + 8 * count);
  pair.set_key(key);
  pair.set_value(value);
}

void TextMapSet(uint8_t* base, uint32_t map, uint32_t key_address, uint32_t value_address) {
  // Both read before any store, as the original's paths do (the found one
  // reads the value just before its only store).
  TextMapSetKey(base, map, schema::UiMovieWord(base, key_address).value(),
                schema::UiMovieWord(base, value_address).value());
}

uint32_t PairsFindKey(uint8_t* base, uint32_t vector, uint32_t key) {
  const schema::UiMoviePairVector v(base, vector);
  const uint32_t count = v.count();
  if (count == 0) {
    return Globals::movie_none_address();
  }
  const std::optional<uint32_t> i = FindKey(base, v.pairs().address, count, key);
  return i ? schema::UiMoviePair(base, v.pairs().address + 8 * *i).value_address()
           : Globals::movie_none_address();
}

uint32_t PairsFind(uint8_t* base, uint32_t vector, uint32_t key_address) {
  return PairsFindKey(base, vector, schema::UiMovieWord(base, key_address).value());
}

uint32_t IsLoaded(uint8_t* base, uint32_t id) {
  const uint32_t movie = schema::UiMovieWord(base, PairsFindKey(base, kMovieTable, id)).value();
  return UiMovie(base, movie).loaded() != 0 ? 1 : 0;
}

namespace {
// A text's record ([the value TextMapFindKey gives]; 0 for none).
uint32_t TextRecord(uint8_t* base, uint32_t movie, uint32_t id) {
  return schema::UiMovieWord(base, TextMapFindKey(base, UiMovie(base, movie).texts_address(), id))
      .value();
}
}  // namespace

uint32_t TextString(uint8_t* base, uint32_t movie, uint32_t id) {
  const uint32_t text = TextRecord(base, movie, id);
  return text == 0 ? Globals::empty_string_address()
                   : schema::UiMovieText(base, text).buffer().address;
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
  const schema::UiMovieText t(base, text);
  t.set_scroll_counter(t.scroll_counter() + 1);
  TextMapSetKey(base, UiMovie(base, movie).texts_address(), id, text);
}

void ClearScreenFlag(uint8_t* base) {
  Globals::set_screen_flag(base, 0);
}

void ResetMovieTable(uint8_t* base, uint32_t renderer, uint32_t list) {
  SetRenderer(base, renderer, list);
  for (uint32_t i = 0; i < Globals::movie_table_count(base); i++) {
    schema::UiMoviePair(base, Globals::movie_table_pairs(base).address + 8 * i)
        .set_key(0xFFFFFFFFu);
    schema::UiMoviePair(base, Globals::movie_table_pairs(base).address + 8 * i).set_value(0);
  }
  Globals::set_unknown_826C5CA4(base, 0);
  Globals::set_unknown_826C5CAC(base, 0);
  Globals::set_unknown_826C5CA8(base, 0);
  Globals::set_unknown_826C5CC0(base, 0);
  Globals::set_unknown_826C5CB4(base, 0);
  Globals::set_unknown_826C5CB8(base, 0);
  Globals::set_unknown_826C5CBC(base, 0);
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

int64_t MovieLoad(uint8_t* base, uint32_t movie, uint32_t id, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_8211E798 UiMovie::Load");
  const uint32_t file = memory::AllocPages(archive::AssetSize(base, id), 4096);
  return load(id, file, 0x8211E500, movie);
}

}  // namespace ui_movies
