// The game's side of the UI movies (UiMovie::, over the library in
// ui_movies.h) as plain C++ (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/ui-movies-game.md). Movies are guest objects that
// recompiled code shares, so these work on guest memory. Adapters:
// src/hooks/ui_movies_game.cpp.
//
// The arithmetic is 32-bit, as the original's addresses are: everything
// wraps at 2^32.

#pragma once

#include "guest.h"

#include <cstdint>

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

}  // namespace ui_movies
