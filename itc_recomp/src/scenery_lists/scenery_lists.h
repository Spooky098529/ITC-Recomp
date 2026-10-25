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

#include <array>
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
// The render queue the draws hand their entries to (its vtable slot 4).
constexpr uint32_t kRenderQueue = 0x8296B91C;

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

// Model::RecordShapeDepth (sub_825960D0, probably): the depth-only record
// of shape `shape`'s `group` materials: command 10, the depth vertex
// declaration (VertexDeclarations::ForDepth of the shape's +260), the
// stream (11; the shape's +320, or with +428 = 1 its +412; stride +264),
// pixel c43 from 0x8265ED10, the index buffer (22) when +428 is 1 and
// [+408]'s +20 isn't 0, then each material (render states 100 and 56,
// command 12 with &+352, the draw 18 from +348 and +340), skipping those
// whose +404 doesn't share `mask` (when not 0) and those with +388 set
// unless `with_flagged`'s low byte is; command 36, the chunk closed.
// Returns the chunk's first slot, or 0 (nothing recorded) when the group
// has no materials (the count +8 or +12 of the shape's +396 block).
std::optional<uint32_t> RecordShapeDepth(uint8_t* base, uint32_t model, uint32_t shape,
                                         int32_t group, uint32_t mask, uint32_t with_flagged);

// What Model::DrawShapeDepth leaves for its adapter: r3 as the original
// leaves it when nothing goes to the render queue, else the queue entry,
// its sort key and depth (the queue's vtable slot 4 is a guest call).
struct DepthDraw {
  uint32_t r3 = 0;
  bool submit = false;
  uint32_t entry = 0;
  uint32_t key = 0;
  double depth = 0;
};

// Model::DrawShapeDepth (sub_82596D48, probably): nothing when the model's
// +28 is 0 or the shape isn't visible (IsShapeVisible, its product at
// `frame` - 96); then Recorder::RecordObject (with the interpolation's hooks
// and `context`, the caller's return address and registers its wrapper
// would keep), a 2-slot entry (+2 = 3, +4 the constants, +18 = 4, +20 =
// RecordShapeDepth(group, mask, with_flagged)) in a chunk of its own, for
// the render queue with the key [0x8296B8FC] and minus the constants'
// +72, when the group (`group`'s low byte: 0 or 1) has materials. nullopt,
// with nothing done, where a recorder would decline (a chunk open on
// another list, or no room for all the slots).
std::optional<DepthDraw> DrawShapeDepth(uint8_t* base, uint32_t model, uint32_t view_projection,
                                        uint32_t world, uint32_t shape, uint32_t group,
                                        uint32_t mask, uint32_t with_flagged, uint32_t frame,
                                        const std::array<uint32_t, 9>& context);

// --- The scenery model's shapes (model_draws.cpp) ---

// The guest calls the draws make: `call` runs `function` with r3 up to r10 =
// `args` from a frame at `frame` (r1) with the return address `lr`, and
// returns r3; `submit` hands an entry to the render queue
// ([0x8296B91C]'s vtable slot 4: r4 = the entry, r5 = the key, f1 = the
// depth) the same way; both return r3 and leave flush-to-zero off.
struct DrawCalls {
  uint32_t (*call)(void* user, uint32_t function, const uint32_t* args, int count,
                   uint32_t frame, uint32_t lr) = nullptr;
  uint32_t (*submit)(void* user, uint32_t entry, uint32_t key, double depth, uint32_t frame,
                     uint32_t lr) = nullptr;
  void* user = nullptr;
};

// SceneryModel::RecordShape's frame (its caller's r1 less this).
constexpr uint32_t kRecordShapeFrame = 304;

// SceneryModel::RecordShape (sub_82146BD0): the shape's group `group`
// materials recorded (docs/functions/scenery-lists.md has the commands),
// the model's vtable slot 1 (with &the blend index at `frame` + 80) and the
// texture provider's slots 5 and 6 guest calls from `frame`, the
// originals' stack temporaries (+80, +96 to +127) written there as the
// original does. Returns 0 when the group has no materials, else the
// chunk's first slot. nullopt, with nothing done, where the original must
// run: a vertex declaration the plain C++ leaves to it, a material
// animation (+396 not -1, never seen in play), more than 4 textures on a
// material, a chunk open on another list, or no room for its slots.
std::optional<uint32_t> SceneryRecordShape(uint8_t* base, const DrawCalls& calls, uint32_t frame,
                                           uint32_t model, uint32_t shape, int32_t group,
                                           uint32_t set_shaders, uint32_t c76, uint32_t range,
                                           uint32_t pair_offset);

// SceneryModel::DrawShape's frame.
constexpr uint32_t kSceneryDrawShapeFrame = 208;

// SceneryModel::DrawShape (sub_82147D88): nothing when the model's +28 is
// 0 or the shape isn't visible (IsShapeVisible, its product at `frame` -
// 96); then a chunk with two 3-slot entries, RecordObject (inside the
// interpolation's hooks, with the context its wrapper keeps: the return
// address 0x82147E10 and r24-r31 = `caller_r24`, the world matrix,
// 0x82960000, the shape, the view-projection, the second and first entries
// and the model) and RecordSceneLights, each group with materials recorded
// (SceneryRecordShape with the shaders) and handed to the render queue
// (keys [0x8296B90C] with minus the constants' +72 and [0x8296B900] with
// it), and with `depth_too` a 2-slot entry for group 0's RecordShapeDepth
// (key [0x8296B908]). Returns r3 as the original leaves it when nothing
// went to the queue (the model for +28 = 0, 0 when not visible); nullopt,
// with nothing done, where a recorder would decline.
struct SceneryDraw {
  bool queued = false;  // r3 is the last queue call's (also in `r3`)
  uint32_t r3 = 0;
};
std::optional<SceneryDraw> SceneryDrawShape(uint8_t* base, const DrawCalls& calls,
                                            uint32_t frame, uint32_t model,
                                            uint32_t view_projection, uint32_t world,
                                            uint32_t shape, uint32_t depth_too, uint32_t c76,
                                            uint32_t range, uint32_t pair_offset,
                                            uint32_t caller_r24);

// --- The generic model's shapes (model_draws.cpp) ---

// Model::RecordShape's frame.
constexpr uint32_t kModelRecordShapeFrame = 304;

// Model::RecordShape (sub_82595550, docs/functions/models.md): as
// SceneryRecordShape without the course's texture animations, the shared
// pair's offset and the samplers' states 12/16/20/36 around the materials:
// the arguments saved in the caller's frame (`frame` + 332 to + 364), the
// header slot at `frame` + 92, the put-back constants' addresses at + 84 and
// + 88; per material its shaders (with `set_shaders`), vertex c9-c13, the
// model's vtable slot 1 (&the blend index at `frame` + 80), pixel c63-c67
// (c64 the material's +80 with w the float at +92, c65 its +16 with w the
// float at +88), render state 3 from the table at 0x8265EEB4, 96 and 100
// for group 0, 56, the textures (the provider's slot 5) with sampler states
// 0 and 4, the environment textures (slots 6 and 5, when neither id is
// -1), vertex c86 from `range` (through `frame` + 112) and pixel c76 (`c76`'s
// words), the draw, the constants put back. Returns 0 when the group has no
// materials, else the chunk's first slot. nullopt, with nothing done, where
// the original must run: a vertex declaration the plain C++ leaves to it,
// a material animation (+396 not -1), more than 4 textures on a material,
// a chunk open on another list, or no room for its slots.
std::optional<uint32_t> ModelRecordShape(uint8_t* base, const DrawCalls& calls, uint32_t frame,
                                         uint32_t model, uint32_t shape, int32_t group,
                                         uint32_t set_shaders, uint32_t c76, uint32_t range);

// Model::DrawShape's frame.
constexpr uint32_t kModelDrawShapeFrame = 208;

// Model::DrawShape (sub_82596B18): as SceneryDrawShape with ModelRecordShape
// for the groups, the interpolation's context of its own (the return address
// 0x82596BA0 and r24-r31 = `caller_r24`, the world matrix, the shape, the
// view-projection, 0x82970000, the second and first entries, the model) and
// its queue calls' return addresses. r3 as the original leaves it: the
// model for +28 = 0, 0 when not visible, else the last queue call's, or
// the lights' header when no group has materials, or the command list after
// the depth entry's chunk when group 0 has none. nullopt, with nothing done,
// where a recorder would decline.
std::optional<SceneryDraw> ModelDrawShape(uint8_t* base, const DrawCalls& calls, uint32_t frame,
                                          uint32_t model, uint32_t view_projection,
                                          uint32_t world, uint32_t shape, uint32_t depth_too,
                                          uint32_t c76, uint32_t range, uint32_t caller_r24);

// PointList::Draw's frame.
constexpr uint32_t kPointListDrawFrame = 464;

// PointList::Draw (sub_82148058): the camera pass's view-projection (+144)
// copied to `frame` + 240 (memcpy); the list's first model (its draw state
// reset, +48 = the list's +612, +436 = its +608) drawn at entry `entry`'s
// points of groups +616 to 2, the second model at group 3's: each point
// with y at least the float at 0x82006808 gets an identity turned about y
// by w x the float at 0x82002948 and by the float at 0x82002A50
// (Matrix::RotateY), moved to the point (Matrix::MultiplyTranslation),
// and the model's shape 0 drawn (SceneryDrawShape, the range from
// 0x82002814 and 0x82006804). Its stack temporaries (+80 to +255) are
// written in `frame` as the original's, which its callees read.
// `caller_r24` is its caller's r24 (the draws' context while the first
// groups are skipped). Returns the r3 the original leaves; nullopt, with
// nothing done, where the memcpy can't be made natively or a draw would
// decline (then the original runs).
std::optional<uint32_t> PointListDraw(uint8_t* base, const DrawCalls& calls, uint32_t frame,
                                      uint32_t list, uint32_t entry, uint32_t pass,
                                      uint32_t caller_r24, uint32_t r3);

}  // namespace scenery_lists
