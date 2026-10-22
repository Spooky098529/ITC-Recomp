// The UI movie library as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/ui-movies.md). Movies are guest objects
// (loaded files whose offsets were turned into pointers) that recompiled
// code reads and writes, so these work on guest memory (level 1 of "The
// boundary rule"). Adapters: src/hooks/ui_movies.cpp.
//
// The arithmetic is 32-bit, as the original's addresses are: everything
// wraps at 2^32.

#pragma once

#include "guest.h"

#include <cstdint>

namespace ui_movies {

// A node's or an animation's 8-byte entries start at +128 (nodes) or +64
// (animations); each kind of entry starts at an index the object holds.
inline uint32_t Entry(uint8_t* base, uint32_t object, uint32_t first_entry,
                      uint32_t index_field, uint32_t i) {
  const uint32_t index = guest::LoadU32(base, object + index_field) + i;
  return guest::LoadU32(base, object + first_entry + 8 * index);
}

// sub_825A6508: a node track list's track i (16 bytes from +16).
inline uint32_t NodeTrack(uint32_t list, uint32_t i) { return list + 16 * (i + 1); }
// sub_825A6518: a key list's key i (32 bytes from +16).
inline uint32_t Key(uint32_t list, uint32_t i) { return list + 32 * i + 16; }
// sub_825A6528: an animation's node track list i (from index [+32]).
inline uint32_t NodeTrackList(uint8_t* base, uint32_t animation, uint32_t i) {
  return Entry(base, animation, 64, 32, i);
}
// sub_825A6540: an animation's key list i (from index [+40]).
inline uint32_t KeyList(uint8_t* base, uint32_t animation, uint32_t i) {
  return Entry(base, animation, 64, 40, i);
}
// sub_825A6558: an animation's sub-animation i (from index [+48]).
inline uint32_t SubAnimation(uint8_t* base, uint32_t animation, uint32_t i) {
  return Entry(base, animation, 64, 48, i);
}

// sub_8259D600: the offset of a primitive's vertex `vertex` when it has
// `textures` texture entries (16 * (textures + 2 * (vertex + 4))): the
// indirect tracks' extra offset.
inline uint32_t VertexOffset(uint32_t textures, uint32_t vertex) {
  return 16 * (textures + 2 * (vertex + 4));
}
// sub_8259D618: a primitive's texture entry i (16 bytes from +128).
inline uint32_t PrimitiveTexture(uint32_t primitive, uint32_t i) {
  return primitive + 16 * (i + 8);
}
// sub_8259D628: a primitive's vertex i (32 bytes after its [+112] texture
// entries).
inline uint32_t PrimitiveVertex(uint8_t* base, uint32_t primitive, uint32_t i) {
  return primitive + VertexOffset(guest::LoadU32(base, primitive + 112), i);
}
// sub_8259D648: a picture's texture entry i (16 bytes from +144).
inline uint32_t PictureTexture(uint32_t picture, uint32_t i) { return picture + 16 * (i + 9); }
// sub_8259D658 (UiMovie::Child): a node's child i (from index [+108]).
inline uint32_t Child(uint8_t* base, uint32_t node, uint32_t i) {
  return Entry(base, node, 128, 108, i);
}
// sub_8259D670: a scene's animation i (from index [+116]).
inline uint32_t SceneAnimation(uint8_t* base, uint32_t scene, uint32_t i) {
  return Entry(base, scene, 128, 116, i);
}

}  // namespace ui_movies
