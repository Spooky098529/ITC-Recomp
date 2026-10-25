// The course set's lists of models and the model draws as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/scenery-lists.md, the fields and globals in
// docs/known-data.md): the lists' release, the race's picks of their
// entries (the race data's flags, the course's layout and the race mode),
// the scenery model's draw state, the animation sets' current frames and the
// texture provider's slots 2 and 4. The data is guest data shared with the
// recompiled code, so these work on guest memory, with the originals' loads
// and stores in their order. Adapters: src/hooks/scenery_lists.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace scenery_lists {

// The race data (its +4 the course, +20 flags, +24 the +2124 list's
// entries' flags, +0x59620 the stage list, +0x59AE4 flags, +0xA3AF0 and
// +0xA3AF4 the race mode).
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kStages = 0x59620;
constexpr uint32_t kRaceFlags = 0x59AE4;
constexpr uint32_t kRaceKind = 0xA3AF0;
constexpr uint32_t kRaceMode = 0xA3AF4;

// --- The lists ---

// The guest calls ModelList::Release makes: Model::Release (sub_82594970)
// and the model's vtable slot 0 with r4 = 1, each returning what it leaves
// in r3.
struct ReleaseCalls {
  uint32_t (*release)(void* user, uint32_t model) = nullptr;
  uint32_t (*destroy)(void* user, uint32_t model) = nullptr;
  void* user = nullptr;
};

// ModelList::Release (sub_82147FB8): for each of the list's +0 models in
// list order (the List64 at +4: the node walk from its head +260, the data
// slot +268 + 4 x slot), Model::Release, then, when not null, its vtable
// slot 0. The count is read again after each model. Returns the r3 the
// original leaves: `list` when there are no models, else the last call's.
uint32_t ModelListRelease(uint8_t* base, uint32_t list, const ReleaseCalls& calls);

// SceneryModel::ResetDrawState (sub_82146AA8, probably): +436 = 0, +432 = 1,
// +440 = 0.
void ResetDrawState(uint8_t* base, uint32_t model);

// AnimationSet::CurrentFrame (sub_82146B80): the current frame's 8-byte
// entry of animation `index` in `set`, or 0 when the current frame isn't
// below the frame count (signed).
uint32_t AnimationCurrentFrame(uint8_t* base, uint32_t set, uint32_t index);

// --- The race's picks ---

// ModelList::IsBEntryOn (sub_82147850, probably): 1 when the race data's
// +24 flag for `entry` is set, else 0.
uint32_t IsBEntryOn(uint8_t* base, int32_t entry);

// Race::CourseLayout (sub_8228F408, with the part the recompiler split off,
// sub_8228F484): -1 without race data, else by the race mode, the course,
// the flags and the current stage's type (Stages::Type).
int32_t CourseLayout(uint8_t* base);

// ModelList::IsCEntryOn (sub_82147958, probably, with the switch case the
// recompiler split off, sub_82147A20): by the race kind and mode, the
// current stage's type and the course's layout.
uint32_t IsCEntryOn(uint8_t* base, uint32_t entry);

// PointList::IsEntryOn (sub_82147B18, probably): by the race data's flags,
// kind, mode and course.
uint32_t PointListIsEntryOn(uint8_t* base, int32_t entry);

// PassDescription::SetBillboardPass (sub_82148650, probably), with its tail
// call sub_8209BD40 merged: +0 = `pass`, +4 = `word28`.
void SetBillboardPass(uint8_t* base, uint32_t description, uint32_t pass, uint32_t word28);

// VertexDeclarations::ForDepth (sub_82589970, probably): [0x8296B8D0] for
// stream type 11, else [0x8296B8CC].
uint32_t DepthVertexDeclaration(uint8_t* base, int32_t stream_type);

// --- The texture provider (vtable 0x8206CCB4) ---

// TextureProvider::ClearAll (sub_82594328, slot 2): every slot from the
// vector's first (+8) up to its end (+12, read again after each store) = 0.
void TextureProviderClearAll(uint8_t* base, uint32_t provider);

// TextureProvider::Set (sub_82594718, slot 4): the slot = `value` when the
// vector has storage and the slot is below its capacity and its size (the
// only path seen in play). False, with nothing written, otherwise: the
// original's reserve, debug prints and resize (never ran) run there.
bool TextureProviderSet(uint8_t* base, uint32_t provider, uint32_t slot, uint32_t value);

// --- The model draws (model_draws.cpp, built with strict floats) ---
//
// The recorders work on the command list at [0x8296B918]: each opens a
// chunk (or finds the list's open), takes its slots and closes the chunk at
// the end. They return nullopt, with nothing done, where the original's
// command list calls would print a message (a chunk open on another list,
// or no room for their slots): the original runs there.

constexpr uint32_t kRecordList = 0x8296B918;

// Model::IsShapeVisible (sub_825951E0): `world` x `view_projection` into
// `product` (64 bytes of guest memory: the original's frame, r1 - 96, where
// the interpolation notes it), then the shape's eight bounding box corners
// (+96 of shape `shape`, 544 bytes each from the model's +40) transformed
// and divided by w, counted against the near plane and the floats at
// 0x82002B90 and 0x8206CDC4 as the translation's vector and scalar steps;
// 0 when all eight are past one plane, else 1.
uint32_t IsShapeVisible(uint8_t* base, uint32_t model, uint32_t view_projection, uint32_t world,
                        uint32_t shape, uint32_t product);

// Recorder::RecordObject (sub_825953D0): the header (7, 9, 0x00010009),
// c0-c3 = world x view-projection (Matrix::Multiply, noted for
// interpolation with the original's return address), c4-c7 the world
// matrix (Matrix::Copy, noted), c8 the 16 bytes at 0x8265ECE0, command 36,
// the chunk closed. Returns the header slot. The interpolation's hooks
// around it (BlendState_BeforeRecordObject and After, the caller's
// context) are its adapter's.
std::optional<uint32_t> RecordObject(uint8_t* base, uint32_t view_projection, uint32_t world);

// Recorder::RecordSceneLights (sub_82595490, probably): the header (7, 63,
// 0x0101003F), c0-c3 the view at 0x8265ECA0 (Matrix::Copy, noted), c4 the
// 16 bytes at 0x8265ECE0, c5-c39 SceneLightsRecordDirections, c40-c62
// SceneLightsRecordProjectors, command 36, the chunk closed. Returns the
// header slot.
std::optional<uint32_t> RecordSceneLights(uint8_t* base);

// SceneLights::RecordDirections (sub_8258B980, probably): at `out`, 3
// vectors from 0x8296B950 normalized (the translation's estimate and Newton
// step; a zero length gives its zero dot product), 8 from 0x8296B9E0 and 8
// from 0x8296BA60 copied, 8 from 0x8296BBE0 normalized, 8 from 0x8296BC60
// copied.
void SceneLightsRecordDirections(uint8_t* base, uint32_t out);
// SceneLights::RecordProjectors (sub_82589B58, probably): at `out`, the
// x, y, z at 0x8265ED00 with w 1.0, 12 floats from 0x8296B980, 12 from
// 0x8296B9B0, 32 from 0x8296BAE0 and 32 from 0x8296BB60 (lfs and stfs).
void SceneLightsRecordProjectors(uint8_t* base, uint32_t out);

}  // namespace scenery_lists
