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

}  // namespace scenery_lists
