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

}  // namespace ui_movies
