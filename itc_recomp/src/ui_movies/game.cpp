#include "ui_movies/game.h"

#include "base/log.h"
#include "memory/heap.h"

#include "ui_movies/ui_movies.h"

#include "c_runtime/strings.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies.h"
#include "schema/ui_movies_game.h"

#include <optional>
#include <vector>

namespace ui_movies {

namespace {
using schema::UiMovie;
using schema::UiMovieAnimationLink;
using schema::UiMovieAnimationList;
using schema::UiMovieAnimationSlot;
using schema::UiMovieChunk;
using schema::UiMovieNode;
using Globals = schema::UiMoviesGameGlobals;

constexpr uint32_t kTagPic = 0x50494300;  // "PIC\0"
constexpr uint32_t kTagPrm = 0x50524D00;  // "PRM\0"
constexpr uint32_t kTagTxt = 0x54585400;  // "TXT\0"

void CopyWords(uint32_t to, uint32_t from, uint32_t first, uint32_t last) {
  for (uint32_t offset = first; offset <= last; offset += 4) {
    guest::StoreU32(to + offset, guest::LoadU32(from + offset));
  }
}
}  // namespace

uint32_t FindChild(uint32_t scene, uint32_t sub_scene, uint32_t child) {
  if (sub_scene != 0xFFFFFFFFu) {
    scene = Child(scene, sub_scene);
  }
  return Child(scene, child);
}

uint32_t FindNode(uint32_t movie, uint32_t sub_scene, uint32_t child) {
  return FindChild(UiMovie(movie).live_scene(), sub_scene, child);
}

void SetColor(uint32_t node, uint32_t color) {
  const uint32_t from = Globals::colors_address() + 12 * color;
  for (uint32_t i = 0; i < 12; i += 4) {
    guest::StoreU32(UiMovieNode(node).red_address() + i, guest::LoadU32(from + i));
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

void SetLayer(uint32_t node, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8211A660 SetLayer");
  UiMovieNode(node).set_layer_offset(Layer(n));
}

void ClearTextIds(uint32_t movie) {
  uint32_t chunk = UiMovie(movie).live_scene();
  while (UiMovieChunk(chunk).tag() != kTagTrm) {
    chunk += UiMovieChunk(chunk).next();
    const UiMovieNode node(chunk);
    if (node.tag() == kTagTxt) {
      node.set_texture_entry(0);  // a text node's text id
    }
  }
}

uint32_t SetNodeTexture(uint32_t node, uint32_t texture) {
  const UiMovieNode n(node);
  const uint32_t tag = n.tag();
  if (tag == kTagPrm) {
    guest::StoreU32(n.height_address(), texture);  // a primitive's texture entry
  } else if (tag == kTagPic) {
    n.set_texture_entry(texture);
  }
  return node;
}

uint32_t PictureTextureSlot(uint32_t movie, uint32_t picture) {
  const uint32_t entry = PictureTexture(picture, UiMovieNode(picture).texture_entry());
  return UiMovie(movie).textures() + 4 * schema::UiMoviePictureTexture(entry).texture();
}

uint32_t SwapTexture(uint32_t movie, uint32_t i, uint32_t texture) {
  const schema::UiMovieWord slot(UiMovie(movie).textures() + 4 * i);
  const uint32_t old = slot.value();
  slot.set_value(texture);
  return old;
}

uint32_t ResetPose(uint32_t movie) {
  const UiMovie m(movie);
  RestorePose(movie, m.live_scene(), m.file_scene(), 0, 0);
  return RestorePose(movie, m.saved_scene(), m.file_scene(), 0, 0);
}

uint32_t ResetNode(uint32_t movie, uint32_t sub_scene, uint32_t child) {
  // The floats among +12..+80 (+36, +68) go through registers (lfs, stfs),
  // which leave their words as they were (checked by the harness's odd
  // floats): every field is copied as a word.
  const UiMovie m(movie);
  if (sub_scene != 0xFFFFFFFFu) {
    const uint32_t file_sub = Child(m.file_scene(), sub_scene);
    const uint32_t live_sub = Child(m.live_scene(), sub_scene);
    const uint32_t saved_sub = Child(m.saved_scene(), sub_scene);
    const uint32_t from = Child(file_sub, child);
    const uint32_t live = Child(live_sub, child);
    const uint32_t saved = Child(saved_sub, child);
    CopyWords(live, from, 12, 80);
    CopyWords(saved, from, 12, 80);
    RestorePose(movie, live_sub, file_sub, live, from);
    return RestorePose(movie, saved_sub, file_sub, saved, from);
  }
  const uint32_t from = Child(m.file_scene(), child);
  const uint32_t live = Child(m.live_scene(), child);
  const uint32_t saved = Child(m.saved_scene(), child);
  CopyWords(live, from, 12, 80);
  CopyWords(saved, from, 12, 80);
  RestorePose(movie, m.live_scene(), m.file_scene(), live, from);
  return RestorePose(movie, m.saved_scene(), m.file_scene(), saved, from);
}

namespace {
void StoreIdentity(uint32_t matrix) {
  constexpr uint32_t kOneBits = 0x3F800000;
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      guest::StoreU32(matrix + 16 * row + 4 * column, row == column ? kOneBits : 0);
    }
  }
}
}  // namespace

uint32_t MakeQuad(uint32_t quad) {
  const schema::UiMovieQuad q(quad);
  StoreIdentity(q.matrix_address());
  q.set_unknown_64(0);
  q.set_blend_mode(0);
  q.set_unknown_72(2);
  return quad;
}

uint32_t MakeTextItem(uint32_t item) {
  const schema::UiMovieTextItem t(item);
  const uint32_t zero = Globals::zero();
  t.set_depth(zero);
  t.set_node(0);
  t.set_red(zero);
  t.set_green(zero);
  t.set_blue(zero);
  t.set_alpha(zero);
  StoreIdentity(t.matrix_address());
  return item;
}

uint32_t ClearTextItem(uint32_t item) {
  const schema::UiMovieTextItem t(item);
  t.set_depth(Globals::zero());
  t.set_node(0);
  StoreIdentity(t.matrix_address());
  return t.matrix_address();
}

bool CopyTextItem(uint32_t to, uint32_t from) {
  if (c_runtime::MayShare(to, 116, from, 116)) {
    return false;
  }
  const schema::UiMovieTextItem t(to), f(from);
  t.set_node(f.node());
  // The matrix (+16) to the clip rectangle's last word (+112), word by word.
  CopyWords(to, from, 16, 112);
  return true;
}

void InitAnimationLists(uint32_t movie) {
  const UiMovie m(movie);
  m.set_file_0(0);
  m.set_file_1(0);
  m.set_file_2(0);
  m.set_texture_set(0);
  m.set_live_scene(0);
  m.set_saved_scene(0);
  m.set_file_scene(0);
  m.set_loaded(0);
  for (uint32_t address : {m.queue_address(), m.looping_address()}) {
    const UiMovieAnimationList list(address);
    const uint32_t links = list.links_address();
    for (uint32_t k = 0; k < 32; k++) {
      const UiMovieAnimationLink link(links + 4 * k);
      link.set_slot(uint16_t(k));
      link.set_next(uint16_t(k + 1));
    }
    UiMovieAnimationLink(links + 4 * 31).set_next(0xFFFF);
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
uint32_t Before(uint32_t list, uint32_t position) {
  return position == 0 ? UiMovieAnimationList(list).first_address()
                       : LinkAt(list, position - 1);
}

// The list's count, sign-extended (the original's lha).
uint32_t CountOf(uint32_t list) {
  return uint32_t(int32_t(int16_t(UiMovieAnimationList(list).count())));
}
}  // namespace

uint32_t LinkAt(uint32_t list, uint32_t steps) {
  uint32_t link = Link(list, UiMovieAnimationList(list).first());
  for (; steps != 0; steps--) {
    link = Link(list, UiMovieAnimationLink(link).next());
  }
  return link;
}

uint32_t SlotOf(uint32_t list, uint32_t link) {
  return UiMovieAnimationList(list).slots_address() +
         40 * uint32_t(int32_t(int16_t(UiMovieAnimationLink(link).slot())));
}

void LinkFree(uint32_t list, int32_t position) {
  ITC_NATIVE_RUNNING("sub_8211B230 LinkFree");
  const UiMovieAnimationList l(list);
  const uint32_t before = Before(list, uint32_t(position));
  const uint16_t free = l.free();
  const UiMovieAnimationLink link(Link(list, free));
  l.set_free(link.next());
  link.set_next(free);
  link.set_next(UiMovieAnimationLink(before).next());
  UiMovieAnimationLink(before).set_next(free);
  l.set_count(uint16_t(l.count() + 1));
}

void RemoveAt(uint32_t list, uint32_t position) {
  const uint32_t before = Before(list, position);
  const UiMovieAnimationLink first(before);
  const uint16_t index = first.next();
  const UiMovieAnimationLink link(Link(list, index));
  const uint16_t next = link.next();
  link.set_next(index);
  first.set_next(next);
  const UiMovieAnimationList l(list);
  const uint16_t freed = link.next();
  const uint16_t free = l.free();
  l.set_free(freed);
  link.set_next(free);
  l.set_count(uint16_t(l.count() - 1));
}

namespace {
// The first slot of `list` that `match` accepts.
template <typename Match>
std::optional<uint32_t> FindSlot(uint32_t list, Match match) {
  const uint32_t count = CountOf(list);
  uint32_t link = Link(list, UiMovieAnimationList(list).first());
  for (uint32_t i = 0; i < count; i++) {
    if (i != 0) {
      link = Link(list, UiMovieAnimationLink(link).next());
    }
    const uint32_t slot = SlotOf(list, link);
    if (match(slot)) {
      return slot;
    }
  }
  return std::nullopt;
}

template <typename Match>
uint32_t FindTime(uint32_t movie, Match match) {
  const UiMovie m(movie);
  for (uint32_t list : {m.looping_address(), m.queue_address()}) {
    if (const std::optional<uint32_t> slot = FindSlot(list, match)) {
      return UiMovieAnimationSlot(*slot).time();
    }
  }
  return Globals::no_time();
}
}  // namespace

uint32_t AnimationTime(uint32_t movie, uint32_t sub_scene, uint32_t index) {
  return FindTime(movie, [&](uint32_t slot) {
    const UiMovieAnimationSlot s(slot);
    return s.sub_scene() == sub_scene && s.index() == index;
  });
}

uint32_t NamedAnimationTime(uint32_t movie, uint32_t index) {
  return FindTime(movie,
                  [&](uint32_t slot) { return UiMovieAnimationSlot(slot).named() == index; });
}

uint32_t IsNamedActive(uint32_t movie, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211BC18 UiMovie::IsNamedActive");
  const UiMovie m(movie);
  for (uint32_t list : {m.looping_address(), m.queue_address()}) {
    if (FindSlot(list, [&](uint32_t slot) {
          return UiMovieAnimationSlot(slot).named() == index;
        })) {
      return 1;
    }
  }
  return 0;
}

Slot LoadSlot(uint32_t address) {
  Slot slot;
  for (uint32_t k = 0; k < slot.size(); k++) {
    slot[k] = guest::LoadU32(address + 4 * k);
  }
  return slot;
}

void StoreSlot(uint32_t address, const Slot& slot) {
  for (uint32_t k = 0; k < slot.size(); k++) {
    guest::StoreU32(address + 4 * k, slot[k]);
  }
}

uint32_t Append(uint32_t list, const Slot& slot) {
  const uint32_t position = CountOf(list);
  LinkFree(list, int16_t(uint16_t(position)));
  const uint32_t to = SlotOf(list, LinkAt(list, position));
  StoreSlot(to, slot);
  return to;
}

namespace {
// The original's loop: each slot copied out by position (AnimationList::Get)
// and tested; the first match removed and appended to the other list.
template <typename Match>
Moved Move(uint32_t movie, bool to_looping, Match match) {
  const UiMovie m(movie);
  const uint32_t from = to_looping ? m.queue_address() : m.looping_address();
  const uint32_t to = to_looping ? m.looping_address() : m.queue_address();
  const uint32_t count = CountOf(from);
  Moved moved;
  for (uint32_t i = 0; i < count; i++) {
    moved.looked = true;
    const Slot slot = LoadSlot(SlotOf(from, LinkAt(from, i)));
    if (match(slot)) {
      RemoveAt(from, i);
      moved.slot = Append(to, slot);
      return moved;
    }
  }
  return moved;
}
}  // namespace

Moved MoveAnimation(uint32_t movie, uint32_t sub_scene, uint32_t index,
                    bool to_looping) {
  ITC_NATIVE_RUNNING("sub_8211DBE0 UiMovie::MoveAnimation");
  return Move(movie, to_looping,
              [&](const Slot& slot) { return slot[1] == sub_scene && slot[0] == index; });
}

Moved MoveNamedAnimation(uint32_t movie, uint32_t index, bool to_looping) {
  ITC_NATIVE_RUNNING("sub_8211DCE8 UiMovie::MoveNamedAnimation");
  return Move(movie, to_looping, [&](const Slot& slot) { return slot[2] == index; });
}

Moved UnloopNamed(uint32_t movie, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211DE30 UiMovie::UnloopNamed");
  return MoveNamedAnimation(movie, index, false);
}

namespace {

// The position of the first slot of `list` that `match` accepts (by its
// words), walked as FindSlot does.
template <typename Match>
std::optional<uint32_t> FindPosition(uint32_t list, Match match) {
  const uint32_t count = CountOf(list);
  for (uint32_t i = 0; i < count; i++) {
    if (match(SlotOf(list, LinkAt(list, i)))) {
      return i;
    }
  }
  return std::nullopt;
}

// The start shared by Play and PlayNamed: the slot appended to the queue
// and copied to the last started. Returns the latter's address. The time's
// word is copied as it is: the original's lfs and stfs leave it so, a
// signaling NaN too (as the recompiled code does, checked by the harness).
uint32_t Start(uint32_t movie, Slot slot, uint32_t animation) {
  const UiMovie m(movie);
  slot[5] = guest::LoadU32(slot[7] != 0 ? schema::UiMovieFileAnimation(animation).length_address()
                         : Globals::zero_address());
  Append(m.queue_address(), slot);
  StoreSlot(m.last_started_address(), slot);
  return m.last_started_address();
}
}  // namespace

uint32_t Play(uint32_t movie, uint32_t sub_scene, uint32_t index,
              uint32_t chained, uint32_t reversed, uint32_t w32, uint32_t paused) {
  const UiMovie m(movie);
  uint32_t scene = m.live_scene();
  if (sub_scene != 0xFFFFFFFFu) {
    scene = Child(scene, sub_scene);
  }
  const uint32_t animation = SceneAnimation(scene, index);
  if (FindPosition(m.looping_address(), [&](uint32_t slot) {
        const UiMovieAnimationSlot s(slot);
        return s.sub_scene() == sub_scene && s.index() == index;
      })) {
    return animation;
  }
  return Start(movie,
               {index, sub_scene, 0xFFFFFFFFu, scene, animation, 0, chained, reversed, w32, paused},
               animation);
}

uint32_t PlayNamed(uint32_t movie, uint32_t named, uint32_t chained,
                   uint32_t reversed, uint32_t w32, uint32_t paused) {
  if (int32_t(named) < 0) {
    return movie;
  }
  const UiMovie m(movie);
  if (const std::optional<uint32_t> i = FindPosition(m.looping_address(), [&](uint32_t slot) {
        return UiMovieAnimationSlot(slot).named() == named;
      })) {
    return *i;
  }
  const schema::UiMovieNamedAnimation entry(m.named_animations_address() + 36 * named);
  const uint32_t animation = entry.animation();
  return Start(movie,
               {0xFFFFFFFFu, 0xFFFFFFFFu, named, entry.scene(), animation, 0, chained, reversed,
                w32, paused},
               animation);
}

Moved LoopLast(uint32_t movie) {
  const UiMovieAnimationSlot last(UiMovie(movie).last_started_address());
  const uint32_t named = last.named();
  if (named != 0xFFFFFFFFu) {
    return MoveNamedAnimation(movie, named, true);
  }
  return MoveAnimation(movie, last.sub_scene(), last.index(), true);
}

uint32_t FinishTime(uint32_t slot) {
  const UiMovieAnimationSlot s(slot);
  const uint32_t reversed = s.reversed();
  return guest::LoadU32(reversed != 0 ? Globals::zero_address()
                          : schema::UiMovieFileAnimation(s.animation()).length_address());
}

namespace {
template <typename Match>
std::optional<Finishing> FindQueued(uint32_t movie, Match match) {
  const uint32_t queue = UiMovie(movie).queue_address();
  const std::optional<uint32_t> position = FindPosition(queue, match);
  if (!position) {
    return std::nullopt;
  }
  const uint32_t slot = SlotOf(queue, LinkAt(queue, *position));
  return Finishing{*position, slot, FinishTime(slot)};
}
}  // namespace

std::optional<Finishing> FindFinishing(uint32_t movie, uint32_t sub_scene,
                                       uint32_t index) {
  ITC_NATIVE_RUNNING("sub_8211DE38 UiMovie::Finish");
  return FindQueued(movie, [&](uint32_t slot) {
    const UiMovieAnimationSlot s(slot);
    return s.sub_scene() == sub_scene && s.index() == index;
  });
}

std::optional<Finishing> FindFinishingNamed(uint32_t movie, uint32_t named) {
  ITC_NATIVE_RUNNING("sub_8211DF20 UiMovie::FinishNamed");
  return FindQueued(movie, [&](uint32_t slot) {
    return UiMovieAnimationSlot(slot).named() == named;
  });
}

namespace {
// The index of the first pair keyed `key` among `count` from `pairs`.
std::optional<uint32_t> FindKey(uint32_t pairs, uint32_t count, uint32_t key) {
  for (uint32_t i = 0; i < count; i++) {
    if (schema::UiMoviePair(pairs + 8 * i).key() == key) {
      return i;
    }
  }
  return std::nullopt;
}
}  // namespace

uint32_t TextMapFindKey(uint32_t map, uint32_t key) {
  const uint32_t count = schema::UiMovieTextMap(map).count();
  if (count == 0) {
    return Globals::text_none_address();
  }
  const std::optional<uint32_t> i = FindKey(map, count, key);
  return i ? schema::UiMoviePair(map + 8 * *i).value_address() : Globals::text_none_address();
}

uint32_t TextMapFind(uint32_t map, uint32_t key_address) {
  return TextMapFindKey(map, schema::UiMovieWord(key_address).value());
}

void TextMapSetKey(uint32_t map, uint32_t key, uint32_t value) {
  const schema::UiMovieTextMap texts(map);
  const uint32_t count = texts.count();
  if (count != 0) {
    if (const std::optional<uint32_t> i = FindKey(map, count, key)) {
      schema::UiMoviePair(map + 8 * *i).set_value(value);
      return;
    }
  }
  texts.set_count(count + 1);
  const schema::UiMoviePair pair(map + 8 * count);
  pair.set_key(key);
  pair.set_value(value);
}

void TextMapSet(uint32_t map, uint32_t key_address, uint32_t value_address) {
  // Both read before any store, as the original's paths do (the found one
  // reads the value just before its only store).
  TextMapSetKey(map, schema::UiMovieWord(key_address).value(),
                schema::UiMovieWord(value_address).value());
}

uint32_t PairsFindKey(uint32_t vector, uint32_t key) {
  const schema::UiMoviePairVector v(vector);
  const uint32_t count = v.count();
  if (count == 0) {
    return Globals::movie_none_address();
  }
  const std::optional<uint32_t> i = FindKey(v.pairs().address, count, key);
  return i ? schema::UiMoviePair(v.pairs().address + 8 * *i).value_address()
           : Globals::movie_none_address();
}

uint32_t PairsFind(uint32_t vector, uint32_t key_address) {
  return PairsFindKey(vector, schema::UiMovieWord(key_address).value());
}

uint32_t IsLoaded(uint32_t id) {
  const uint32_t movie = schema::UiMovieWord(PairsFindKey(kMovieTable, id)).value();
  return UiMovie(movie).loaded() != 0 ? 1 : 0;
}

namespace {
// A text's record ([the value TextMapFindKey gives]; 0 for none).
uint32_t TextRecord(uint32_t movie, uint32_t id) {
  return schema::UiMovieWord(TextMapFindKey(UiMovie(movie).texts_address(), id))
      .value();
}
}  // namespace

uint32_t TextString(uint32_t movie, uint32_t id) {
  const uint32_t text = TextRecord(movie, id);
  return text == 0 ? Globals::empty_string_address()
                   : schema::UiMovieText(text).buffer().address;
}

uint32_t TextField(uint32_t movie, uint32_t id, uint32_t offset) {
  // One print per native function sharing this body (sub_8211B770's +20
  // still has its adapter).
  if (offset == 4) ITC_NATIVE_RUNNING("sub_8211B9B8 (a text's +4)");
  if (offset == 24) ITC_NATIVE_RUNNING("sub_8211B7B8 (a text's +24)");
  const uint32_t text = TextRecord(movie, id);
  return text == 0 ? 0 : uint32_t(schema::UiMovieText(text).load_at(offset, 4));
}

void AdvanceTextScroll(uint32_t movie, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8211B940 (a text's scroll counter)");
  const uint32_t text = TextRecord(movie, id);
  const schema::UiMovieText t(text);
  t.set_scroll_counter(t.scroll_counter() + 1);
  TextMapSetKey(UiMovie(movie).texts_address(), id, text);
}

void ClearScreenFlag() {
  Globals::set_screen_flag(0);
}

void ResetMovieTable(uint32_t renderer, uint32_t list) {
  SetRenderer(renderer, list);
  for (uint32_t i = 0; i < Globals::movie_table_count(); i++) {
    schema::UiMoviePair(Globals::movie_table_pairs().address + 8 * i)
        .set_key(0xFFFFFFFFu);
    schema::UiMoviePair(Globals::movie_table_pairs().address + 8 * i).set_value(0);
  }
  Globals::set_unknown_826C5CA4(0);
  Globals::set_unknown_826C5CAC(0);
  Globals::set_unknown_826C5CA8(0);
  Globals::set_unknown_826C5CC0(0);
  Globals::set_unknown_826C5CB4(0);
  Globals::set_unknown_826C5CB8(0);
  Globals::set_unknown_826C5CBC(0);
}

namespace {
constexpr uint32_t kTagGrp = 0x47525000;  // "GRP\0"
constexpr uint32_t kBuckets = 64, kBucketWords = 65;

// A group's children: the first index at +100 and the count at +104 (the
// picture's width and height words, read as integers).
uint32_t FirstChild(uint32_t group) {
  return guest::LoadU32(UiMovieNode(group).width_address());
}
uint32_t ChildCount(uint32_t group) {
  return guest::LoadU32(UiMovieNode(group).height_address());
}

// Calls `visit(child)` for a group's children, last first, as the original
// walks them (the first index read again each time).
template <typename Visit>
void ForChildren(uint32_t scene, uint32_t group, Visit visit) {
  for (int32_t i = int32_t(ChildCount(group) + FirstChild(group) - 1);
       i >= int32_t(FirstChild(group)); i--) {
    if (!visit(Child(scene, uint32_t(i)))) {
      return;
    }
  }
}

bool LayersFitAt(uint32_t scene, uint32_t group, uint32_t depth) {
  if (depth > 32) {
    return false;
  }
  const int64_t last =
      int32_t(ChildCount(group) + FirstChild(group) - 1);
  if (last - int64_t(int32_t(FirstChild(group))) + 1 > int64_t(kBuckets)) {
    return false;
  }
  bool fits = true;
  ForChildren(scene, group, [&](uint32_t child) {
    const UiMovieNode node(child);
    const uint32_t tag = node.tag();
    if (int32_t(node.layer_offset()) < 0 ||
        (tag == kTagGrp && !LayersFitAt(scene, child, depth + 1)) ||
        (tag == kTagSce && !LayersFitAt(child, child, depth + 1))) {
      fits = false;
    }
    return fits;
  });
  return fits;
}
}  // namespace

bool LayersFit(uint32_t scene, uint32_t group) {
  return LayersFitAt(scene, group, 0);
}

uint32_t AssignLayers(uint32_t stack, uint32_t movie, uint32_t scene,
                      uint32_t group, uint32_t layer, bool by_bucket) {
  // The buckets the original keeps in its 16,800-byte frame (+80: 64 of 64
  // words and a count each), which only this function reads: host words in
  // the same layout (LayersFit keeps each bucket within its 64), on the heap
  // as the recursion goes up to 33 deep. `stack` is no longer needed.
  (void)stack;
  std::vector<uint32_t> buckets(kBuckets * kBucketWords);
  for (uint32_t b = 0; b < kBuckets; b++) {
    buckets[kBucketWords * b + 64] = 0;
  }
  uint32_t result = movie;
  ForChildren(scene, group, [&](uint32_t child) {
    result = child;
    const int32_t offset = int32_t(UiMovieNode(child).layer_offset());
    const uint32_t bucket = kBucketWords * uint32_t(offset < 63 ? offset : 63);
    const uint32_t count = buckets[bucket + 64];
    buckets[bucket + 64] = count + 1;
    buckets[bucket + count] = child;
    return true;
  });
  for (uint32_t b = 0; b < kBuckets; b++) {
    const uint32_t bucket = kBucketWords * b;
    const uint32_t count = buckets[bucket + 64];
    for (uint32_t j = 0; j < count; j++) {
      const uint32_t node = buckets[bucket + j];
      if (by_bucket) {
        layer = b;
      }
      switch (UiMovieNode(node).tag()) {
        case kTagGrp:
          result = AssignLayers(stack, movie, scene, node, layer, false);
          break;
        case kTagSce:
          result = AssignLayers(stack, movie, node, node, layer, false);
          break;
        case kTagPic:
        case kTagPrm:
        case kTagTxt:
          SetLayer(node, layer);
          result = movie;
          break;
        default:
          break;
      }
    }
  }
  return result;
}

int64_t MovieLoad(uint32_t movie, uint32_t id, const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_8211E798 UiMovie::Load");
  // A blob (E5 step 5: the file in PC memory, in the guest's byte order) of
  // the pages the original's VirtualAlloc gave: the archive reads whole
  // 2,048-byte sectors into it, past the asset's size.
  const uint32_t size = (archive::AssetSize(id) + 4095u) & ~4095u;
  const uint32_t file = schema::UiMovieArchiveFile::New(size).address;
  return load(id, file, 0x8211E500, movie);
}

}  // namespace ui_movies
