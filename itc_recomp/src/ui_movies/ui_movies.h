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
#include <initializer_list>
#include <optional>
#include <vector>

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

// Tags, as the words read from guest memory.
constexpr uint32_t kTagTrm = 0x54524D00;  // "TRM\0", a chunk list's end
constexpr uint32_t kTagSce = 0x53434500;  // "SCE\0", a scene

// The globals UiMovie::SetRenderer fills: the renderer object the library
// hands its chunks to (its vtable's slot 4), and the command list it records
// into.
constexpr uint32_t kRenderer = 0x82970C18;
constexpr uint32_t kCommandList = 0x82970C1C;

// sub_8259D5C8: from the chunk at `chunk` (not itself tested), walks the
// chunks (+4 the offset to the next, +0 its tag) to the first tagged `tag`;
// at the end (`TRM`) returns 0, unless `tag` is `TRM` (then the end).
uint32_t FindChunk(uint8_t* base, uint32_t chunk, uint32_t tag);

// sub_825A0EB0: a movie file's scene (file + [file + 4]) when tagged `SCE`,
// else 0.
uint32_t FileScene(uint8_t* base, uint32_t file);

// sub_825A0CD0: an `XMD` file's entry i (file + [file + 16 + 4 i]) when i is
// below its count (+12, unsigned), else 0.
uint32_t XmdEntry(uint8_t* base, uint32_t file, uint32_t i);

// sub_8259D798 (UiMovie::SetRenderer): stores the renderer and the command
// list in their globals.
void SetRenderer(uint8_t* base, uint32_t renderer, uint32_t list);

// sub_8259D930: a draw context reset: the identity matrix (+0..+60), no clip
// rectangle (+64..+76 = 0), the color (+80..+92) all the float at
// 0x82000A30 (1.0), +96..+108 = 0.
void ResetDrawContext(uint8_t* base, uint32_t context);

// Loading (files.cpp): the movie file (`X2D`, version `005`) and the `XMD`
// file (version `001`) are fixed up in place once loaded.
//
// sub_825A0C60: whether `file` starts with `XMD\0` `001\0`.
bool IsXmd(uint8_t* base, uint32_t file);
// sub_825A0CF8: 1 when the XMD file is 16-byte aligned and IsXmd, else 0.
// The original then byte-swaps its +8, +12 and entry words unless word +0
// reads `XMD\0`, which IsXmd has just made sure of: the swap can't run, and
// isn't ported.
uint32_t CheckXmd(uint8_t* base, uint32_t file);
// sub_825A0E40: whether a movie file holds `X2D\0` `005\0` at +8.
bool IsMovieFile(uint8_t* base, uint32_t file);
// sub_825A0ED8: a movie file's table entry `index` from its animations'
// start ([file + 16]): the word at file + 32 + 8 ([file + 16] + index).
uint32_t FileAnimation(uint8_t* base, uint32_t file, uint32_t index);
// sub_8259D688: a scene's child entries ([+124] of them, 8 bytes from +128)
// from offsets to pointers: those in the animation range ([+116], [+120] of
// them) through the file's table (FileAnimation), the others relative to the
// scene; recursing into children tagged `SCE`. Returns 1.
uint32_t FixUpScene(uint8_t* base, uint32_t scene, uint32_t file);
// sub_825A6570: an animation's entries ([+56], 8 bytes from +64) from
// offsets to pointers (relative to it), then each sub-animation's +16 ([+52]
// of them from index [+48]) through the file's table. Returns 1.
uint32_t FixUpAnimation(uint8_t* base, uint32_t animation, uint32_t file);
// sub_825A0EF0 (UiMovie::FixUpFile): 0 unless 16-byte aligned and
// IsMovieFile; else the table's [+24] entries (8 bytes from +32) from
// offsets to pointers, the scene at file + [+4] fixed up (0 passed on when
// it isn't tagged `SCE`, as the original does), and each of its [+20]
// animations; 1.
uint32_t FixUpFile(uint8_t* base, uint32_t file);

// The keys' interpolation (keys.cpp, built with -ffp-model=strict and
// -ffp-contract=off: the translated code's doubles, rounded to single at the
// same steps, std::fma where it has fmadds). k0 and k1 are the two keys
// (32 bytes: +4 the time, +8 the value, +12 and +16 the second key's
// tangent angles), t the time between them (0 to 1, from the caller's f1).
//
// sub_8259FA20: an integer track's eased value, (1 - s) [k0 + 8] + s
// [k1 + 8] with s a cubic in t from k1's +12 and +16 and the float at
// 0x820021E4, truncated (fctiwz).
uint32_t EaseInteger(uint8_t* base, uint32_t k0, uint32_t k1, double t);
// sub_8259FB30: a float track's value, a cubic between the keys with
// tangents tan(k1 + 16) and tan(k1 + 12) scaled by the time between the keys
// and the floats at 0x82002A38 and 0x820021E4. nullopt, with nothing done,
// where tan (sub_825A8A50) would go to its error paths or the rounding mode
// isn't nearest (tan sets nearest and puts the mode back): the original
// runs there.
std::optional<double> CubicFloat(uint8_t* base, uint32_t k0, uint32_t k1, double t);
// sub_8259FC18: the same cubic on integer values, truncated (fctiwz).
std::optional<uint32_t> CubicInteger(uint8_t* base, uint32_t k0, uint32_t k1, double t);

// The recorders (recording.cpp), on the command list `list` (the global at
// 0x82970C1C). Each records a chunk and returns its first slot, which the
// caller hands to the renderer (vtable slot 4); nothing is done (nullopt)
// where the original's command list calls would print a message (another
// chunk open, the list full), which the caller leaves to the original.
//
// Whether chunks with these allocations (slot counts, in order) fit, one
// after the other, as the command list's OpenChunk and AllocateSlots check.
bool ChunksFit(uint8_t* base, uint32_t list, std::initializer_list<std::vector<uint32_t>> chunks);
// sub_8259DAD0 (textured) and sub_8259E098 (untextured): a quad or
// primitive built by the caller (+68 the blend mode, +80 the vertex count,
// +84 the texture, +88, +92 and +96 the positions, colors and texture
// coordinates, 16 bytes a vertex, +100 clipped, +104..+110 the clip
// rectangle, +112 the texture stage flag): the clip (command 31 with the
// rectangle, or 1 and 200), the texture (command 4 and +84), the blend
// states by mode, the texture stage, 13, 6, the vertices (command 5) and
// 15, then 36.
std::optional<uint32_t> RecordQuad(uint8_t* base, uint32_t list, uint32_t quad, bool textured);
// sub_8259D7B0's two chunks (4 slots each; check them with ChunksFit
// first): the clip set to the whole screen (command 31, 0, 0, 1280, 720,
// then 36), and commands 1 and 200, then 36.
extern const std::vector<uint32_t> kResetChunkSlots;
uint32_t RecordClipReset(uint8_t* base, uint32_t list);
uint32_t RecordStateReset(uint8_t* base, uint32_t list);

}  // namespace ui_movies
