// The game's side of the UI movies (UiMovie::, over the library in
// ui_movies.h) as plain C++ (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/ui-movies-game.md). Movies are guest objects that
// recompiled code shares, so these work on guest memory. Adapters:
// src/hooks/ui_movies_game.cpp.
//
// The arithmetic is 32-bit, as the original's addresses are: everything
// wraps at 2^32.

#pragma once

#include "archive/archive.h"
#include "guest.h"
#include "strings_and_font/strings_and_font.h"

#include <array>
#include <cstdint>
#include <functional>
#include <optional>

namespace ui_movies {

// A movie's fields: its three scenes (the live one that's animated and
// drawn, the pose saved between updates, the file's own) and its textures'
// array.
constexpr uint32_t kMovieLive = 16;
constexpr uint32_t kMovieSaved = 20;
constexpr uint32_t kMovieFile = 24;
constexpr uint32_t kMovieTextures = 28;

// The node lookups the game's setters share: a child of the scene `scene`,
// or of its sub-scene `sub_scene` when that isn't -1.
uint32_t FindChild(uint8_t* base, uint32_t scene, uint32_t sub_scene, uint32_t child);

// sub_8211A578 (UiMovie::FindNode): the live scene's node (FindChild).
uint32_t FindNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child);

// sub_8211A630: color `color` of the table at 0x82008410 (12 bytes each)
// copied to the node's +56..+64.
void SetColor(uint8_t* base, uint32_t node, uint32_t color);

// sub_8211A660: the node's +72 set to the game's layer for number n
// (Layer).
uint32_t Layer(uint32_t n);
void SetLayer(uint8_t* base, uint32_t node, uint32_t n);

// sub_8211A6C0: every `TXT` chunk after the live scene's, up to `TRM`, gets
// its text id (+108) cleared.
void ClearTextIds(uint8_t* base, uint32_t movie);

// sub_8211A708 (UiMovie::SetNodeTexture): a primitive's +104 or a
// picture's +108 set to `texture` (other tags left). Returns the node.
uint32_t SetNodeTexture(uint8_t* base, uint32_t node, uint32_t texture);

// The texture word a picture's first texture entry names in the movie's
// array ([movie + 28] + 4 [PictureTexture(picture, [picture + 108])]).
uint32_t PictureTextureSlot(uint8_t* base, uint32_t movie, uint32_t picture);

// sub_8211AB90 (UiMovie::SwapTexture): texture i of the movie set to
// `texture`; returns the old one.
uint32_t SwapTexture(uint8_t* base, uint32_t movie, uint32_t i, uint32_t texture);

// sub_8211ABA8 (UiMovie::ResetPose): the file's pose copied over the live
// and the saved scenes (RestorePose with no group). Returns what the second
// RestorePose returns.
uint32_t ResetPose(uint8_t* base, uint32_t movie);

// sub_8211A1E0 (UiMovie::ResetNode): a node's +12..+80 copied from the
// file's scene to the live and saved ones, then its subtree (RestorePose
// with the node as the group). Returns what the second RestorePose returns.
uint32_t ResetNode(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child);

// The quads' and text items' set-up (the identity matrix copied in with
// memcpy in the original, after stores of 1.0 that it overwrites).
//
// sub_8211A948 (Quad::Quad): the identity at +0, +64 and +68 (the blend
// mode) = 0, +72 = 2. Returns the quad.
uint32_t MakeQuad(uint8_t* base, uint32_t quad);
// sub_8211A818 (TextItem::TextItem): +0 = 0, the identity at +16, +80..+96
// = the float at 0x82000A34 (0.0). Returns the item.
uint32_t MakeTextItem(uint8_t* base, uint32_t item);
// sub_8211A8C8 (TextItem::~TextItem): +80 = the float at 0x82000A34, +0 =
// 0, the identity at +16. Returns item + 16 (memcpy's return, left in r3).
uint32_t ClearTextItem(uint8_t* base, uint32_t item);
// sub_8211AED0: a text item copied: +0, the matrix at +16 (memcpy), +80,
// +84..+96 and +100..+112 (+116..+127 aren't). False, with nothing done,
// where the two items might share memory (c_runtime::MayShare): the order
// of the original's stores decides there, and it runs.
bool CopyTextItem(uint8_t* base, uint32_t to, uint32_t from);

// The animation lists (two a movie: the queue at +0x90B4 and the looping
// animations at +0x9640): 32 links of 4 bytes (+0 the next link, +2 its
// slot; 16-bit, -1 ending a chain), the free chain's first at +128, the
// list's first at +132 (+130 and +134 set to -1, not read here), the count
// at +136 (16-bit), and 32 slots of 40 bytes from +140 (+0 an animation's
// index, +4 its sub-scene's or -1, +8 a named animation's index or -1, +20
// the time). A link's slot never changes; linking moves links between the
// chains.
constexpr uint32_t kMovieQueue = 0x90B4;
constexpr uint32_t kMovieLooping = 0x9640;
constexpr uint32_t kMovieLoaded = 0x9FD0;

// sub_8211A9E8: a movie's +0..+24 and loaded flag (+0x9FD0) cleared, and
// both lists set up empty (link k: next k + 1, slot k; the free chain from
// 0, link 31's next -1).
void InitAnimationLists(uint8_t* base, uint32_t movie);
// The link `steps` links after the list's first.
uint32_t LinkAt(uint8_t* base, uint32_t list, uint32_t steps);
// A link's slot: list + 140 + 40 [link + 2].
uint32_t SlotOf(uint8_t* base, uint32_t list, uint32_t link);
// sub_8211B230: the free chain's first link linked in at `position` (the
// original's 16-bit r4, sign-extended), the count + 1. No check for a full
// list.
void LinkFree(uint8_t* base, uint32_t list, int32_t position);
// sub_8211B080 (AnimationList::RemoveAt): the link at `position` unlinked
// onto the free chain, the count - 1.
void RemoveAt(uint8_t* base, uint32_t list, uint32_t position);
// sub_8211BA00 (UiMovie::AnimationTime) and sub_8211BB18
// (UiMovie::NamedAnimationTime): the time (+20, a float's word) of the
// first slot naming the animation (`index`, `sub_scene` or, by name,
// `index` at +8), in the looping list, then the queue; else the float at
// 0x82001BE0.
uint32_t AnimationTime(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index);
uint32_t NamedAnimationTime(uint8_t* base, uint32_t movie, uint32_t index);

// A list's 40-byte slot as its ten words, and the slot of the last
// animation started (+0x9FD8: +0 index, +4 sub-scene, +8 named index).
using Slot = std::array<uint32_t, 10>;
constexpr uint32_t kMovieLastStarted = 0x9FD8;
Slot LoadSlot(uint8_t* base, uint32_t address);
void StoreSlot(uint8_t* base, uint32_t address, const Slot& slot);
// AnimationList::Insert (sub_8211B4D0) at the end of the list (its count,
// 16-bit, sign-extended, as the original's callers pass it), from a slot
// that can't share the list's memory. Returns the slot written.
uint32_t Append(uint8_t* base, uint32_t list, const Slot& slot);

// UiMovie::Update (sub_8211D2F8, docs/functions/ui-movies.md)'s own work;
// the adapter (src/hooks/ui_movies.cpp) makes its calls. Its frame (1,616
// bytes) holds a local animation list at +80 (the slots moved off the queue
// this update, applied after drawing and dropped).
constexpr uint32_t kUpdateFrame = 1616;
constexpr uint32_t kUpdateLocal = 80;
// The local list set up: link k's next k + 1 and slot k, link 31's next -1,
// the free chain from 0, no slots used (+130, +132 and +134 set to -1).
void UpdateBegin(uint8_t* base, uint32_t local);
// A queued slot's time (+20) stepped by the float at 0x820021E8, back when
// reversed (+28), and kept within 0 and its animation's length ([+16] +
// 12).
void StepQueuedTime(uint8_t* base, uint32_t slot);
// A looping slot's time stepped forward, wrapped: below 0 to the length,
// past the length to 0.
void StepLoopingTime(uint8_t* base, uint32_t slot);
// The list's first link unlinked onto the free chain and the count down by
// one, with the original's loads and stores in their order: the queue's
// (its first read here), and the local list's (its first, `first`, and its
// link read before the slot's animation was applied).
void UnlinkQueueFirst(uint8_t* base, uint32_t list);
void UnlinkLocalFirst(uint8_t* base, uint32_t list, uint32_t link, uint16_t first);

// sub_8211DBE0 (UiMovie::MoveAnimation) and sub_8211DCE8
// (UiMovie::MoveNamedAnimation): the first slot naming the animation (its
// +4 `sub_scene` and +0 `index`, or by name its +8 `index`) in the queue
// (`to_looping`) or the looping list, copied out, removed (RemoveAt) and
// appended to the other list. Returns the slot appended to, or nothing when
// no slot matched; `looked` says whether the list had slots (the original's
// r3 differs: a copy in its frame then, the movie otherwise).
struct Moved {
  std::optional<uint32_t> slot;
  bool looked = false;
};
Moved MoveAnimation(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index,
                    bool to_looping);
Moved MoveNamedAnimation(uint8_t* base, uint32_t movie, uint32_t index, bool to_looping);
// sub_8211DDE8 (UiMovie::LoopLast): the last started animation moved to the
// looping list, by name when its +8 isn't -1.
Moved LoopLast(uint8_t* base, uint32_t movie);

// The time a finished animation's slot is applied at: the float word at
// 0x82000A34 (0.0) when reversed (+28), else its animation's length ([+16]
// + 12).
uint32_t FinishTime(uint8_t* base, uint32_t slot);
// sub_8211DE38 (UiMovie::Finish)'s and sub_8211DF20 (UiMovie::FinishNamed)'s
// search, after their move of the animation to the queue (MoveAnimation or
// MoveNamedAnimation, not to the looping list): the first queued slot with
// its +4 `sub_scene` and +0 `index`, or by name its +8 `named` (the count
// read then, 16-bit, sign-extended and compared unsigned), its position and
// FinishTime. The adapters apply it there (UiMovie::ApplyAnimation on +12
// and +16, r6 = 0) and remove it (RemoveAt): the live scene keeps its end
// pose.
struct Finishing {
  uint32_t position = 0, slot = 0, time = 0;
};
std::optional<Finishing> FindFinishing(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                       uint32_t index);
std::optional<Finishing> FindFinishingNamed(uint8_t* base, uint32_t movie, uint32_t named);

// sub_8211D978 (UiMovie::Play): animation `index` of the live scene (or of
// its sub-scene `sub_scene` when that isn't -1; SceneAnimation) started:
// unless the looping list has a slot with its +4 `sub_scene` and +0
// `index`, a slot {index, sub_scene, -1, the scene, the animation, the
// time, chained, reversed, w32, paused} appended to the queue and copied to
// the last started (+0x9FD8). The time is 0.0 (the float at 0x82000A34),
// or the animation's length (+12) when `reversed` isn't 0. Returns the r3
// the original leaves: the animation when the looping list had it, else
// the last started slot's address.
uint32_t Play(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t index,
              uint32_t chained, uint32_t reversed, uint32_t w32, uint32_t paused);
// sub_8211DAC8 (UiMovie::PlayNamed): the same for named animation `named`
// (nothing for a negative one; the looping list's +8 tested): the slot {-1,
// -1, named, then the entry's +28 and +32 (movie + 176 + 36 named), the
// time from the latter's +12, ...}. Returns the r3 the original leaves: the
// movie for a negative `named`, the matching slot's position in the
// looping list, else the last started slot's address.
uint32_t PlayNamed(uint8_t* base, uint32_t movie, uint32_t named, uint32_t chained,
                   uint32_t reversed, uint32_t w32, uint32_t paused);

// The maps of {key, value} word pairs. A movie's texts (+0x9BCC): 128 pairs
// in place, the count at +1024. The movie table (0x826C5CCC) and the like:
// +0 the pairs, +4 the capacity, +8 the count.
constexpr uint32_t kMovieTable = 0x826C5CCC;
//
// sub_8211B310 (TextMap::Find): the value's address for the key at
// `key_address`, or 0x826C5CD8 when it's not there.
uint32_t TextMapFind(uint8_t* base, uint32_t map, uint32_t key_address);
// sub_8211B370 (TextMap::Set): the value at `value_address` stored for the
// key at `key_address`, appended when it's not there (the count stored
// first, then the pair; no check against the 128 pairs).
void TextMapSet(uint8_t* base, uint32_t map, uint32_t key_address, uint32_t value_address);
// sub_8211B3F0 (UiMovies::Find): the value's address for the key at
// `key_address`, or 0x826C5CDC when it's not there.
uint32_t PairsFind(uint8_t* base, uint32_t vector, uint32_t key_address);
// The same three with the key (and value) as words.
uint32_t TextMapFindKey(uint8_t* base, uint32_t map, uint32_t key);
void TextMapSetKey(uint8_t* base, uint32_t map, uint32_t key, uint32_t value);
uint32_t PairsFindKey(uint8_t* base, uint32_t vector, uint32_t key);

// sub_8211BE98 (UiMovies::IsLoaded): 1 when the movie with id `id` in the
// movie table has its loaded flag (+0x9FD0) set, else 0. No check that the
// movie exists: an id not in the table reads the word at 0x826C5CDC as the
// movie, as the original does.
uint32_t IsLoaded(uint8_t* base, uint32_t id);

// A movie's texts by id (the map at +0x9BCC): records of +0 the UTF-16
// buffer, +4 scrolling, +8 the scroll counter, +20 the characters shown,
// +24 passed to the formatting. An id not in the map reads the word at
// 0x826C5CD8 as the record, as the originals do.
constexpr uint32_t kMovieTexts = 0x9BCC;
// sub_8211B720 (UiMovie::TextString): the text's buffer, or the empty string
// at 0x82008434 when the record is 0.
uint32_t TextString(uint8_t* base, uint32_t movie, uint32_t id);
// sub_8211B9B8 (+4), sub_8211B770 (+20), sub_8211B7B8 (+24): the record's
// word at `offset`, or 0 when the record is 0.
uint32_t TextField(uint8_t* base, uint32_t movie, uint32_t id, uint32_t offset);
// sub_8211B940: the record's +8 plus one, then the record stored again for
// the id (TextMapSetKey: appended when the id wasn't there). No check for a
// record of 0 (the original's neither).
void AdvanceTextScroll(uint8_t* base, uint32_t movie, uint32_t id);

// sub_8211B800 (UiMovie::ScrollOffset): a scrolling text's x for the draw
// parameters at `params` (+0 x, +8 the origin's x, +28 the scale, +68 the
// gap, +120 the text id): the start x as Font::SetParams works it out; while
// the text's scroll counter (+8) is below its delay (+12), x itself; after
// it, the start less the ticks past the delay times the speed (+16), moved
// on by the text's width (Font::TextWidth on [0x826C5C00]) plus the gap
// until it's not left of the start less the width (a step of 0 or less
// there loops forever, as the original). Returns f1.
double ScrollOffset(uint8_t* base, uint32_t movie, uint32_t params);

// UiMovie::DrawTexts (sub_8211CD90): r3 = movie, r4 = the text list (movie +
// 40: +0 the items, 128 bytes each, +8 the count), r5 = 1 to draw without
// advancing. Each item, last first, is drawn from the original's 4,752-byte
// frame (`frame`, its r1), where its callees read what it builds: the item
// copied to +368, the draw parameters at +112 (124 bytes), the text at +496
// (4,096 bytes), and the original's other stores (+80..+108, +240..+367).
constexpr uint32_t kDrawTextsFrame = 4752;
constexpr uint32_t kDrawTextsParams = 112;
constexpr uint32_t kDrawTextsItem = 368;
constexpr uint32_t kDrawTextsBuffer = 496;
// One item's start: the text buffer cleared (memset), the count down by
// one; returns the item to copy to +368 (sub_8211AED0: CopyTextItem, or the
// original where they may share memory).
uint32_t DrawTextsPop(uint8_t* base, uint32_t frame, uint32_t list);
// The copied item's drawing up to Text::Draw: its origin transformed by its
// matrix (Vec4::Transform), the node's (+0) scale and clip rectangle set,
// the draw parameters built, a scrolling text's x (ScrollOffset) and its
// counter stepped unless `draw_only` (sub_8211B940), and the text's page
// copied into the buffer (Strings::CopyPage). Text::Draw (r4 = the
// parameters, r5 = the buffer) and TextItem::~TextItem on the copy follow,
// the adapter's.
void DrawTextsItem(uint8_t* base, uint32_t frame, uint32_t movie, bool draw_only);

// sub_8211AD00's work after its renderer calls: the UI movie library's
// renderer and command list set (SetRenderer), every entry of the movie
// table marked free (key -1, value 0; nothing freed), and the screen's
// globals 0x826C5CA4..0x826C5CAC, 0x826C5CC0 and 0x826C5CB4..0x826C5CBC
// cleared. (0x826C5CB0 is cleared before the calls: ClearScreenFlag.)
void ClearScreenFlag(uint8_t* base);
void ResetMovieTable(uint8_t* base, uint32_t renderer, uint32_t list);

// sub_8211BD10: the layers of a group's nodes, by their layer offsets
// (+72): its children ([group + 100] on, [group + 104] of them, last first;
// Child of `scene`) sorted into 64 buckets (an offset above 63 in the last),
// then bucket by bucket in order: a group recursed into (the same scene), a
// scene recursed into as the scene and the group, a picture's, primitive's
// or text's layer set (SetLayer) to `layer`, or with `by_bucket` to its
// bucket's number. The buckets are kept where the original keeps them, in
// its 16,800-byte frame below `stack` (the caller's r1): 260 bytes each from
// +80 (64 nodes, then the count at +256), the recursions' frames below.
// Returns the r3 the original leaves: the movie after a layer set, a
// recursion's, else the last child looked up (`movie` when none).
//
// LayersFit first: the original's buckets overflow into each other or
// outside its frame (where its own saved registers are) with more than 64
// children or a negative offset, and that isn't followed: false, with
// nothing done, for a tree (read as it is: the layers set are all above 63)
// with a group of more than 64 children, a negative offset, or deeper than
// 32 groups; the original runs there.
bool LayersFit(uint8_t* base, uint32_t scene, uint32_t group);
uint32_t AssignLayers(uint8_t* base, uint32_t stack, uint32_t movie, uint32_t scene,
                      uint32_t group, uint32_t layer, bool by_bucket);

// --- The movies' vectors and objects (objects.cpp) ---
//
// Blocks come from operator new and new[] and go back with operator delete
// and delete[] (the C runtime heap's plain C++, memory/c_runtime_heap.h): the
// adapters call these only while --native_heap routes the game's heap to the
// allocation layer, and each function's checks come first, with nothing
// done (nullopt or false) where the original must run (a size malloc
// refuses, a block the layer didn't give).

// Whether operator new or new[] gives `size` bytes natively (malloc's
// checks), and whether operator delete or delete[] frees `block` natively (0
// or a block the layer gave).
bool CanAllocate(uint8_t* base, uint32_t size);
bool CanDelete(uint32_t block);

// The vectors of 8-byte {key, value} pairs (the movie table at 0x826C5CCC)
// and of words (a movie's textures, +28): +0 the array, +4 the capacity, +8
// the count.
//
// sub_8211B0E8: capacity 64, 512 bytes (operator new[]) with the 64 pairs
// cleared when given (+0 = 0 otherwise); the count isn't written (the movie
// table is a static). nullopt where the original must run.
std::optional<uint32_t> MakePairs(uint8_t* base, uint32_t vector);
// sub_8211B458 (with sub_8211B160, the capacity changed): the pair appended
// at the count, the capacity doubled first when the count has reached it (a
// new array, cleared, the pairs that fit copied, the old one freed; nothing
// when the doubled capacity is the same). Returns the r3 the original
// leaves: the old array when it was freed, else the vector.
std::optional<uint32_t> PairsAppend(uint8_t* base, uint32_t vector, uint32_t key, uint32_t value);
// sub_8211C168: the value set for the key (compared as signed words) when
// it's there, else the pair appended (PairsAppend). Returns r3 as the
// original leaves it (the vector, or PairsAppend's).
std::optional<uint32_t> PairsSet(uint8_t* base, uint32_t vector, uint32_t key, uint32_t value);
// sub_8211AE18: capacity 512, 2 KB (operator new[], not checked for 0), the
// count 0. nullopt where the original must run.
std::optional<uint32_t> MakeWords(uint8_t* base, uint32_t vector);
// sub_8211AE68 (with sub_8211A788, the capacity changed: the new array not
// cleared): the word appended, as PairsAppend.
std::optional<uint32_t> WordsAppend(uint8_t* base, uint32_t vector, uint32_t word);

// A movie's named animations: 1,024 entries of 36 bytes from +176 (a
// std::string, then +28 the scene and +32 the animation), the count at
// +0x90B0 (176 + 0x9000). A movie's texts: the map at +0x9BCC (kMovieTexts)
// of 28-byte records (+0 a 4 KB buffer from VirtualAlloc), their count at
// +0x9FCC. Its text list at +40 (+0 the items, 128 bytes each after a
// 16-byte count; +4 the capacity, +8 the count) and draw context at +64.
constexpr uint32_t kMovieNamed = 176;
constexpr uint32_t kNamedBytes = 36;
constexpr uint32_t kNamedCount = 1024;
constexpr uint32_t kMovieNamedUsed = kMovieNamed + 0x9000;
constexpr uint32_t kMovieTextCount = 0x9FCC;
constexpr uint32_t kMovieTextList = 40;

// sub_8211E1C0 (UiMovie::UiMovie): the textures (MakeWords on +28), the text
// list (sub_8211B018: capacity 512, 512 items made with MakeTextItem after
// their count, 512, in a block from operator new[]; its count, +48, 0), the
// draw context reset (ResetDrawContext), the 1,024 named animations made
// (sub_8211D210: an empty string, as its copy of an empty temporary leaves
// it, and +28, +32 = 0), the named count 0, and the texts' 128 pairs and
// their count cleared. Returns the movie; nullopt where the original must
// run.
std::optional<uint32_t> ConstructMovie(uint8_t* base, uint32_t movie);
// sub_8211E160 (UiMovie::~UiMovie): the named animations' strings
// destroyed last first (sub_8211D1B8: _Tidy(entry, true, 0)), the text
// items destroyed last first and their block freed (sub_8211AF90 with 3,
// when +40 isn't 0), then the textures' array freed (operator delete[]).
// Returns the r3 the original leaves (the textures' array); nullopt, with
// nothing done, where a free wouldn't run natively.
std::optional<uint32_t> DestroyMovie(uint8_t* base, uint32_t movie);
// sub_8211AC90: each text record ([+0x9FCC] of them, the values from the
// texts' map) freed: its buffer (VirtualFree, MEM_RELEASE: the allocation
// layer's pages) and itself (operator delete); the count 0. Returns the r3
// the original leaves (the last record, or the movie when there were
// none); nullopt, with nothing done, where a free wouldn't run natively.
std::optional<uint32_t> FreeTexts(uint8_t* base, uint32_t movie);

// sub_8211B660 (UiMovie::GetText): r4's node id (a TXT node's +108, at
// `id_address`) set to [movie + 0x9FCC] + 1 when 0, then its text record
// (TextMapFindKey; an id not there reads the word at 0x826C5CD8); when that
// is 0, one is made (not stored in the map: the caller does): 28 bytes from
// operator new, +0 a 4 KB buffer from VirtualAlloc (the layer's pages, as
// its adapter: MEM_COMMIT, 4 KB aligned) cleared (memset), +16 the float
// word at 0x82000A34 (0.0), +4..+12, +20 and +24 = 0. Returns the record;
// nullopt, with nothing done, where operator new wouldn't run natively.
std::optional<uint32_t> GetText(uint8_t* base, uint32_t movie, uint32_t id_address);

// sub_8211E378: the scene's named animations added to the movie's (+176,
// the count at +0x90B0, no check against the 1,024): its SCE children's
// first (recursed into), then for each animation (SceneAnimation) the name
// "<scene's +84><the byte at 0x82001DE8><the animation's +16>" built in
// std::strings in the original's 240-byte frame below `stack` (its r1: +96
// the name, +128 its copy; the recursions' frames below), then copied into
// the next entry (sub_8211E318: AssignFrom the copy, +28 the scene, +32 the
// animation) and both temporaries destroyed (_Tidy). The strings through
// std_string's plain C++ (a logged abort if one refuses after the
// adapter's checks: the process heap there). Returns the r3 the original
// leaves (the movie, the last child looked up or a recursion's, or the
// name's _Tidy: its block when it was on the heap, else the name).
uint32_t AddNamedAnimations(uint8_t* base, uint32_t stack, uint32_t movie, uint32_t scene);

// sub_8211BEE8 (effect.cpp): a primitive node drawn over the full frame's
// texture, from the original's 1,248-byte frame (`frame`, its r1): the quad
// at +160 (blend mode 4, key 597), its positions at +384, colors +640 and
// texture coordinates +896, a scale matrix at +320 (the identity built at
// +96 first), the level description at +288. The adapter calls
// CameraPasses::RenderTarget(0) between the two parts and hands the quad to
// sub_8259EBE8 (r4 = 597, f1 = the depth EffectQuad returns).
constexpr uint32_t kEffectFrame = 1248;
constexpr uint32_t kEffectQuad = 160;
constexpr uint32_t kEffectKey = 597;
// Before RenderTarget: the positions' w (the float at 0x82000A30, 1.0) in
// all 16, the quad made (MakeQuad), the identity, and the scale matrix (the
// identity with the float at 0x82001070 at its +0 and +20).
void EffectBegin(uint8_t* base, uint32_t frame);
// After it: the texture's level 0 description (TextureLevelDesc), the
// quad's vertex count (the primitive's +116), texture and arrays; for each
// vertex (PrimitiveVertex) its x and y plus the primitive's +12 and +16 (z
// the float at 0x82000A34, w 1.0) through the scale matrix
// (debug::Vec4Transform), its +24 and +28 over the texture's width and
// height, and the primitive's color (+56..+64) and alpha (+68 times the
// float at 0x82000C44, as an integer); with any vertex, the clip cleared
// (+100..+111). Blend mode 4, +72 = 0. Returns the depth (the float at
// 0x82000A34). The original's integer temporaries at +80 and +88 written as
// it leaves them.
double EffectQuad(uint8_t* base, uint32_t frame, uint32_t primitive, uint32_t texture);

// The text setters: r3 = movie, r4 = a sub-scene's index or -1, r5 = a
// child's index: the node (FindNode), its text record (GetText, which may
// make one), the change, then the record stored for the node's id
// (TextMapSetKey). Each returns the r3 the original leaves (the movie's
// texts' map, as TextMap::Set leaves it); nullopt, with nothing done, where
// GetText would run the original.
//
// sub_8211C470 (UiMovie::ClearText): the buffer's 4 KB cleared (memset)
// and +8 = 0.
std::optional<uint32_t> ClearText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                  uint32_t child);
// sub_8211C4F0: +12 = `delay`, +16 = `speed` (f1) as stfs stores it.
std::optional<uint32_t> SetTextScrollSpeed(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                           uint32_t child, uint32_t delay, double speed);
// sub_8211C570 (UiMovie::ScrollText): +4 = `scrolling`, +12 = 45, +16 the
// float word at 0x820021F8 (copied).
std::optional<uint32_t> ScrollText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                   uint32_t child, uint32_t scrolling);
// sub_8211C5F8: the scroll counter (+8) = 0.
std::optional<uint32_t> ResetTextScroll(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                        uint32_t child);
// sub_8211D880 (UiMovie::RevealText, with sub_8211C930): when the record's
// page (+20) + 1 is at most its buffer's page count (Strings::PageCount,
// compared signed), the page + 1 and 1, else 0; the record stored either
// way. Returns r3 (1 or 0).
std::optional<uint32_t> RevealText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                   uint32_t child);
// sub_8211D8D8 (with sub_8211C9C0): the same test with no step and nothing
// stored: 1 while there's another page, else 0.
std::optional<uint32_t> HasMoreText(uint8_t* base, uint32_t movie, uint32_t sub_scene,
                                    uint32_t child);

// sub_8211C1F8 UiMovie::SetText: the node (FindNode) must be a TXT (else
// nullopt: the original loops forever there, an assert); `varargs` (its r7
// to r10) stored in the caller's parameter area (`stack` + 48 to + 79) and
// the va_list from there; its text record (GetText, which may make one)
// cleared (its buffer's 4 KB, memset) with +20 = 0, string `id` (16-bit)
// formatted into the buffer (Strings::FormatV from its 128-byte frame below
// `stack`, with `calls` for FormatV's guest calls), then the record stored
// for the node's id (TextMapSetKey). Returns movie + kMovieTexts (the r3
// TextMap::Set leaves); nullopt, with nothing done but the varargs' stores
// (the original makes them again), where GetText would run the original.
std::optional<uint32_t> SetText(uint8_t* base, const strings_and_font::FormatCalls& calls,
                                uint32_t stack, uint32_t movie, uint32_t sub_scene,
                                uint32_t child, uint32_t id, const uint64_t varargs[4]);

// sub_8211E798 UiMovie::Load: the asset's size (Archive::AssetSize) in pages
// (VirtualAlloc's, AllocPages), then load(id, them, sub_8211E500, the movie).
// Returns the serial (r3).
int64_t MovieLoad(uint8_t* base, uint32_t movie, uint32_t id, const archive::LoadCall& load);

}  // namespace ui_movies

namespace ui_movies {

// --- The movie table's open and delete (objects.cpp) ---
//
// Their callees run natively where their own checks say so; the adapter
// (src/hooks/ui_movies_game.cpp, hooks::UiMoviesOpenCalls and the rest)
// hands the others to the originals from frames like the originals'.

// sub_8211E808 UiMovies::Open's calls: `make` is operator new(40,976) and,
// when it gave a block, UiMovie::UiMovie on it (the movie, or 0); `set` is
// sub_8211C168 on the movie table with the key at `key_address` and the
// value at `value_address` (the frame's +84 and +80).
struct OpenCalls {
  std::function<uint32_t()> make;
  std::function<void(uint32_t key_address, uint32_t value_address)> set;
};
// sub_8211E808 UiMovies::Open: the movie table's entry for `id` (the key
// stored at the frame's +80 first; `frame` is the original's r1, 128
// bytes); when there's a movie, 0. Else the movie made (calls.make; stored
// at +80), its animation lists set up (InitAnimationLists, passed 0 when
// the allocation failed, as the original), `id` at its +0x9FD4, `asset`
// loaded into it (MovieLoad), `id` at +84 and the pair set (calls.set).
// Returns 1.
uint32_t OpenMovie(uint8_t* base, uint32_t id, uint32_t asset, uint32_t frame,
                   const archive::LoadCall& load, const OpenCalls& calls);

// sub_8211B540 UiMovie::Unload's calls (from its 240-byte frame):
// VirtualFree(address, 0, MEM_RELEASE) (`which` 0-2, the three copies),
// operator delete, the text item copy sub_8211AED0, XPR::Release of a slot
// sub_82108860 and the texts freed sub_8211AC90 (its r3 returned).
struct UnloadCalls {
  std::function<void(uint32_t which, uint32_t address)> virtual_free;
  std::function<void(uint32_t block)> free;
  std::function<void(uint32_t to, uint32_t from)> copy_item;
  std::function<void(uint32_t slot)> release_xpr;
  std::function<uint32_t(uint32_t movie)> free_texts;
};
// sub_8211B540 UiMovie::Unload: the three file copies (+0, +4, +8) freed,
// the scenes (+16, +20, +24) cleared; the textures popped (+28: the count
// decremented first, each entry's three words cleared and the entry freed
// when not 0); the queued text items popped (+40: each copied to the
// frame's +80, then ClearTextItem there); the texture set (+12) released
// when set and cleared, the loaded flag (+0x9FD0) cleared, the texts freed.
// `frame` is the original's r1. Returns r3 as the original leaves it (the
// texts' free's).
uint32_t UnloadMovie(uint8_t* base, uint32_t movie, uint32_t frame, const UnloadCalls& calls);

// sub_8211E258 UiMovies::Delete's calls: Unload's (from its frame below
// Delete's), UiMovie::~UiMovie, operator delete and the pair set (as
// OpenCalls::set; its r3 returned).
struct DeleteCalls {
  UnloadCalls unload;
  std::function<void(uint32_t movie)> destroy;
  std::function<void(uint32_t movie)> free;
  std::function<uint32_t(uint32_t key_address, uint32_t value_address)> set;
};
// sub_8211E258 UiMovies::Delete: the movie table's entry for `id` (the key
// at the frame's +80; `frame` is the original's r1, 128 bytes); when
// there's a movie: unloaded (UnloadMovie, its frame 240 bytes below),
// destroyed, freed, then `id` at +84, 0 at +80 and the pair set to 0.
// Returns r3 as the original leaves it (the entry's address when there was
// none, else the set's).
uint32_t DeleteMovie(uint8_t* base, uint32_t id, uint32_t frame, const DeleteCalls& calls);

}  // namespace ui_movies
