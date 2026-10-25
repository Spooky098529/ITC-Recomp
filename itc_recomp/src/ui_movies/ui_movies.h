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

#include "base/log.h"
#include "c_runtime/exit.h"

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
inline uint32_t NodeTrack(uint32_t list, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_825A6508 NodeTrack");
  return list + 16 * (i + 1);
}
// sub_825A6518: a key list's key i (32 bytes from +16).
inline uint32_t Key(uint32_t list, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_825A6518 Key");
  return list + 32 * i + 16;
}
// sub_825A6528: an animation's node track list i (from index [+32]).
inline uint32_t NodeTrackList(uint8_t* base, uint32_t animation, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_825A6528 NodeTrackList");
  return Entry(base, animation, 64, 32, i);
}
// sub_825A6540: an animation's key list i (from index [+40]).
inline uint32_t KeyList(uint8_t* base, uint32_t animation, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_825A6540 KeyList");
  return Entry(base, animation, 64, 40, i);
}
// sub_825A6558: an animation's sub-animation i (from index [+48]).
inline uint32_t SubAnimation(uint8_t* base, uint32_t animation, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_825A6558 SubAnimation");
  return Entry(base, animation, 64, 48, i);
}

// sub_8259D600: the offset of a primitive's vertex `vertex` when it has
// `textures` texture entries (16 * (textures + 2 * (vertex + 4))): the
// indirect tracks' extra offset.
inline uint32_t VertexOffset(uint32_t textures, uint32_t vertex) {
  ITC_NATIVE_RUNNING("sub_8259D600 VertexOffset");
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

// UiMovie::RestorePose (sub_82119B80; ui_movies.cpp): puts a movie's live
// tree back to its rest pose. `live` and `rest` are the two scenes; the
// children walked are `group`'s range (+100 the first, +104 the count),
// last first, or the live scene's when `group` or `rest_group` is 0 (as the
// original tests r6 and r7). For each pair of children (Child at the same
// index in each scene) whose live one is tagged GRP, PIC, PRM, SCE, TFM or
// TXT: +12..+80 copied from the rest one, then by tag: a group recursed
// into (same scenes, its range), a scene recursed into as the scenes, a
// picture's +100..+136, a text's +100, +104, +116..+136, a primitive's
// +100..+108 and its first [rest + 116] vertices (+0..+28 each). Returns
// the r3 the original leaves (its callers may pass it on): the last child
// pair's (rest child + 28, the rest primitive's last vertex, a recursion's
// result, or the rest child when its tag isn't one of those), or `movie`
// when nothing is walked.
uint32_t RestorePose(uint8_t* base, uint32_t movie, uint32_t live, uint32_t rest, uint32_t group,
                     uint32_t rest_group);

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

// For ApplyTrack: a key function's original, called through the translated
// code from ApplyTrack's original frame (the adapter's, hooks/ui_movies.cpp)
// with r3 = k0, r4 = k1, f1 = t; returns its r3 and f1. For sub_8259FB30 and
// sub_8259FC18 where CubicFloat and CubicInteger give nullopt, and
// sub_8259FD40 and sub_8259FAA8 (type 2's cubic and eased keys), which
// never ran in play and aren't ported.
struct KeyResult {
  uint64_t r3;
  double f1;
};
using GuestKeyFunction = KeyResult (*)(void* user, uint32_t function, uint32_t k0, uint32_t k1,
                                       double t);
struct KeyCall {
  GuestKeyFunction guest = nullptr;
  void* user = nullptr;
};

// sub_825A0548 UiMovie::ApplyTrack: one track's value at t between keys k0
// and k1 (0 for none: k0 the stand-in key at frame + 112, k1 then k0)
// written into the node: the descriptor from TrackProperty (called as the
// original calls it, `atexit` its atexit fallback), the target node + its
// offset (+ VertexOffset([node + 112], [track + 12]) when indirect), set or
// added by the descriptor's byte +8; type 0 a float, 1 and 2 integers
// (signed and unsigned), 3 and up nothing. The value by the second key's
// kind (+0): 0 step, 1 linear, 2 cubic, 3 eased, others zero. `frame` = the
// original's frame (r1 - 224): the stand-in key is written there as the
// original writes it (+112 0, +116 zero, +120 0 or the target's value when
// the track sets it, +124 and +128 zero); its other stores (+80, +88, +96,
// the saved registers) aren't written. Built as keys.cpp.
void ApplyTrack(uint8_t* base, uint32_t node, uint32_t track, double t, uint32_t k0,
                uint32_t k1, uint32_t frame, const c_runtime::AtexitCall& atexit,
                const KeyCall& keys);

// For ApplyAnimation: one track applied (UiMovie::ApplyTrack, sub_825A0548,
// called through its override, which keeps --per_tick_ui_movie_tracks'
// skips and --log_pause_movie's log), from the original's frame `frame` (the
// level's r1 - 224) with r3 = node, r4 = track, f1 = t, r5 = k0, r6 = k1.
using GuestTrackFunction = void (*)(void* user, uint32_t frame, uint32_t node, uint32_t track,
                                    double t, uint32_t k0, uint32_t k1);
struct TrackCall {
  GuestTrackFunction call = nullptr;
  void* user = nullptr;
};

// sub_825A08E0 UiMovie::ApplyAnimation (animation.cpp, built as keys.cpp):
// for each node track list (NodeTrackList, [animation + 36] of them) and
// each child of `node` with its id (+8), each track's keys around `time` (k
// at or before the track's local time, time - its start +8, and `time`
// itself before the next; before the first key none and it, at or after
// the last it and none) and its t, clamped to 0..1 (0 for keys at the same
// time), applied through `apply`; then each sub-animation (SubAnimation,
// [animation + 52]) from its start (+24) at the time mapped from +24..+28
// onto +32..+36, recursively on `node` (id +8 = 0) or its children with
// that id, from the frame 224 bytes below. Returns 1 while playing (with
// `loop`, time above 0; else time below the length +12), else 0. `frame` =
// the original's frame (r1 - 224); none of its stores are made.
uint32_t ApplyAnimation(uint8_t* base, uint32_t node, uint32_t animation, double time,
                        uint32_t loop, uint32_t frame, const TrackCall& apply);

// The drawing walk (draw.cpp).
//
// UiMovie::ApplyTransform (sub_8259E6C8): the node's transform applied to
// the draw context: the clip (the node's rectangle +40..+52 offset by the
// context's +32 and +36 as integers, kept at +64 or intersected with the
// context's, sub_8259D730 merged), the color (+80..+92), and the matrix
// (Matrix::Multiply, local first, each noted for frame interpolation with the
// original's return address): translated by the position plus the pivot,
// turned (Matrix::RotateZ), scaled (+108 set when a factor isn't 1.0), and
// translated back. `frame` is the original's 352-byte frame (the local
// matrices at +176 and +240, the angle at +80, the turn's at -80). Returns
// the r3 the original leaves (the context, or its clip when no matrix was
// multiplied). Flush-to-zero off on entry (the adapter's job).
uint32_t ApplyTransform(uint8_t* base, uint32_t context, uint32_t node, uint32_t frame);

// sub_8259D9F8: a draw context copied from its parent's: the matrix
// (Matrix::Copy, noted for frame interpolation with the original's return
// address; the w column set to 1.0 first), then +64..+108 (the clip, the
// color, +96..+108). Returns the context.
uint32_t CopyDrawContext(uint8_t* base, uint32_t context, uint32_t parent);
// sub_8259E620: a queued text item copied: +0, the matrix at +16
// (Matrix::Copy, noted), +80 (as a float), +84..+112. Returns `to`.
uint32_t CopyQueuedText(uint8_t* base, uint32_t to, uint32_t from);
// Whether a text list (+0 the items, +4 the capacity, +8 the count) is full.
bool TextListFull(uint8_t* base, uint32_t list);
// sub_8259F700: the item appended to the list (the count raised first,
// then CopyQueuedText into items + 128 x count); `r3` its slot. False, with
// nothing done, when the list is full (its grow, sub_8259EC58, never seen in
// play, is the original's).
bool AppendText(uint8_t* base, uint32_t list, uint32_t item, uint32_t* r3);
// UiMovie::QueueText (sub_8259F770): ApplyTransform, then with a text list
// (+100) a text item built at `frame` + 96 (`frame` the original's 272-byte
// frame): MakeTextItem, the node, the matrix (Matrix::Copy, noted), the
// depth (the layer, unsigned, times the float at 0x82071520), the color and
// the clip; appended, then ClearTextItem. `r3` as the original leaves it.
// False, with nothing done, when the list is full (the original runs).
bool QueueText(uint8_t* base, uint32_t context, uint32_t node, uint32_t layer, uint32_t frame,
               uint32_t* r3);

// A quad or primitive built for sub_8259EBE8: r3 = `quad` (on the
// builder's frame), r4 = `key`, f1 = `depth`.
struct QuadCall {
  uint32_t quad;
  uint32_t key;
  double depth;
};
// UiMovie::DrawPicture (sub_8259ED40)'s work before its call of
// sub_8259EBE8: the picture's transform applied (ApplyTransform), its quad
// built at `frame` + 112 (the original's 1,088-byte frame: positions at
// +240, colors +496, texture coordinates +752): the corners (0, 0) to
// (+100, +104) transformed by the context (Vec4::Transform through
// debug::Vec4Transform), the context's color as integers, the texture's
// coordinates (+116..+128 over its size, else 0 to 1; +132 and +136 flip
// them), the clip; the key +72 and the depth (the layer times the float at
// 0x82071520).
// Its caller checks first that the picture has a texture (PictureTextureOf
// below not 0): without one, the original reads its size at guest address
// 4, which faults (never so in play), and the original is left to run.
QuadCall BuildPicture(uint8_t* base, uint32_t context, uint32_t picture, uint32_t layer,
                      uint32_t frame);
// The picture's texture: with +140 set, the context's table entry for its
// texture entry [+108]; else 0.
uint32_t PictureTextureOf(uint8_t* base, uint32_t context, uint32_t picture);
// UiMovie::DrawPrimitive (sub_8259F348)'s work before its call of
// sub_8259EBE8: the same for a primitive's [+116] vertices, its quad at
// `frame` + 96 (the original's 1,152-byte frame: positions +272, colors
// +528, texture coordinates +784), the vertices' colors times the
// context's (squared with +100, which is also the quad's blend mode).
QuadCall BuildPrimitive(uint8_t* base, uint32_t context, uint32_t primitive, uint32_t layer,
                        uint32_t frame);

// UiMovie::DrawNode (sub_8259F848): ApplyTransform on the node, then its
// children [+100] to [+100] + [+104] - 1 of `scene` (Child), last first
// ([+100] read again each time), each skipped when invisible (+80 = 0) or
// either scale (+28, +32) is near zero, else the context copied to the
// level's frame (CopyDrawContext at `stack` - 272 + 80) and, by tag: GRP
// drawn again (the same scene), SCE drawn as its own scene, PIC and PRM
// through `call` (they hand their quads to the renderer through a guest
// call), TXT through QueueText (`call` where it would grow the list or the
// layer doesn't fit 32 bits), the layer `layer` + [child + 72] (64-bit, as
// the original adds it). `stack` is the r1 the original is called with;
// each level's frame is 272 bytes below it, the callees called with r1 at
// that frame. Returns the r3 the original leaves.
enum class DrawCall { kPicture, kPrimitive, kText };
using DrawNodeCall = uint32_t (*)(void* user, DrawCall kind, uint32_t context, uint32_t scene,
                                  uint32_t node, uint64_t layer, uint32_t stack,
                                  uint32_t return_address);
uint32_t DrawNode(uint8_t* base, uint32_t context, uint32_t scene, uint32_t node, uint64_t layer,
                  uint32_t stack, DrawNodeCall call, void* user);

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

// --- The tracks' properties (track_properties.cpp) ---

// The 64 12-byte descriptors (+0 the node offset, +4 the type: 0 float, 1
// and 2 integer, byte +8 set/add, byte +9 indirect).
constexpr uint32_t kTrackProperties = 0x82970C28;

// sub_8259FE68 UiMovie::TrackProperty: r3 = the property id; returns its
// descriptor's address (id x 12 on the table, the full 64 bits). The first
// call (guard bit 0 of 0x82970F28) clears the table and registers its
// clean-up 0x825DB3F0 with atexit (through `call` when the native one
// can't); the first with byte 0x82970C20 clear fills the 48 descriptors and
// sets it. `frame` = the original's frame (r1 - 128): the +8 words' last two
// bytes are its +90 and +91, as the original leaves them.
uint64_t TrackProperty(uint8_t* base, uint64_t id, uint32_t frame,
                       const c_runtime::AtexitCall& call);

}  // namespace ui_movies
