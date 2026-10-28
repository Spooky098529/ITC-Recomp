// The graphics library's model animation as plain C++
// (docs/functions/model-animation.md): the track sets a model holds for its
// part file's sections (+304 the float4s, +324 and +344 floats, +364 32-byte
// values, +384 208-byte values), their tracks and curves (20-byte keys),
// Model::Animate, and the skeleton class's small pieces. The data is guest
// memory shared with the recompiled code, so these work on guest memory with
// the originals' loads and stores in their order, and their float steps as
// the translated code does them (car_physics/float_ops.h; built with
// -ffp-model=strict and -ffp-contract=off, flush-to-zero off: the adapters'
// job). Adapters: src/hooks/model_animation.cpp, --native_model_animation.
//
// The functions that free or allocate go through the C runtime's heap over
// the allocation layer (memory/c_runtime_heap.h), so their adapters call
// them only while --native_heap routes the game's heap to it. They return
// false, with nothing done, where the original would take a path left to
// it: a block the layer didn't give (free's error path), a size operator
// new refuses (the new handler, std::bad_alloc).

#pragma once

#include <cstdint>

namespace model_animation {

// --- The track sets (20 bytes: +0 the section, +4 its track count's
// address, +8 its tracks, +12 the default start time, +16 the values). ---

// TrackSet::Clear (sub_825A6670): +0, +4, +8 = 0, +12 = 0.0.
void TrackSetClear(uint8_t* base, uint32_t set);
// TrackSet::TrackSet (sub_825A1B70): Clear, +16 = 0; returns the set.
uint32_t TrackSetConstruct(uint8_t* base, uint32_t set);
// TrackSet::View (sub_825A6690): binds the section (0: as Clear).
void TrackSetView(uint8_t* base, uint32_t set, uint32_t section);
// TrackSet::Count (sub_825A66D8): the track count, 0 with no section.
uint32_t TrackSetCount(uint8_t* base, uint32_t set);
// TrackSet::StartTime and EndTime (sub_825A1F08, sub_825A1F50): the track's
// +72 or +76, the section's +4 or +8 for track -1; 0.0 with no section.
double TrackSetStartTime(uint8_t* base, uint32_t set, uint32_t track);
double TrackSetEndTime(uint8_t* base, uint32_t set, uint32_t track);
// TrackSet::Track (sub_825A2000): the 80-byte track; 0 with no section.
uint32_t TrackSetTrack(uint8_t* base, uint32_t set, uint32_t index);
// TrackSet::Channels (sub_825A2030): the track's first channel (section +
// its +68); 0 with no section.
uint32_t TrackSetChannels(uint8_t* base, uint32_t set, uint32_t index);

// Curve::Sample (sub_825A1F98): the keys' curve at `time`.
double CurveSample(uint8_t* base, uint32_t keys, uint32_t count, double time);
// Key::Interpolate (sub_825A66F8): between two keys by the first's flags (2
// linear, 8 stepped, else cubic Hermite).
double KeyInterpolate(uint8_t* base, uint32_t key0, uint32_t key1, double time);

// TrackSet::SampleFloat4 (sub_825A1C88): value `index` (16 bytes) = (0, 0,
// 1, its time), then each channel sampled at its time into its component.
void TrackSetSampleFloat4(uint8_t* base, uint32_t set, uint32_t index);
// TrackSet::Advance (sub_825A1D68): value `index`'s time (+12) + step,
// looped between the track's start and end times; 1 when it looped.
uint32_t TrackSetAdvance(uint8_t* base, uint32_t set, uint32_t index, double step);
// sub_825A0FE0 (the +364 set's sampling; never ran in play): value `index`
// (32 bytes): +20 the mask of the components written, each channel sampled
// at its time (+16) into its component.
void TrackSetSample32(uint8_t* base, uint32_t set, uint32_t index);
// sub_825A10A8 (never ran in play): TrackSetAdvance for 32-byte values
// (the time at +16).
uint32_t TrackSetAdvance32(uint8_t* base, uint32_t set, uint32_t index, double step);
// Model::Animate (sub_82594B60): the +304 set's tracks sampled and advanced
// by 1.0, then the +364 set's. Returns the original's r3 (the last count).
uint32_t ModelAnimate(uint8_t* base, uint32_t model);

// TrackSet::ReleaseArray (sub_825A11D0): +16 freed as an array with a
// count header (operator delete[] on +16 - 16), +16 = 0.
bool TrackSetReleaseArray(uint8_t* base, uint32_t set);
// TrackSet::Release (sub_825A1BA8): +16 freed (no header), +16 = 0.
bool TrackSetRelease(uint8_t* base, uint32_t set);
// TrackSet::SetupFloat4 (sub_825A1E08), Setup32 (sub_825A1218): the old
// values freed, the section bound, its track count of 16- or 32-byte values
// allocated with a count header, each +12 = 1.0, then each one's time (+12
// or +16) = the section's start time.
bool TrackSetSetupFloat4(uint8_t* base, uint32_t set, uint32_t section);
bool TrackSetSetup32(uint8_t* base, uint32_t set, uint32_t section);
// TrackSet::SetupFloat (sub_825A1BE8): as SetupFloat4 with 4-byte values,
// no header and no 1.0.
bool TrackSetSetupFloat(uint8_t* base, uint32_t set, uint32_t section);
// TrackSet::Setup208 (sub_825A1A38): natively only for a section with no
// tracks (in play, every one): the old values freed and the section bound.
// False, with nothing done, when it has tracks (their set-up,
// sub_825A6608 and sub_825A1938, never ran in play: the original's).
bool TrackSetSetup208(uint8_t* base, uint32_t set, uint32_t section);

// --- The skeleton (the base class: +0 the vtable 0x8206CE60, +16 a matrix,
// +80 the node file's view, +84 its node records (160 bytes), +128 the
// instances' cursors (20 bytes each), +132, +136 (144-byte node records),
// +144 and +148 (the nodes' 64-byte matrices) arrays, +156 the root's
// index; the skeleton: the vtable 0x8206CE50, +176 a float4 a node). ---

// SkeletonBase::SkeletonBase (sub_8259A140): the vtable, +16 the identity,
// +80 = 0 (XACTWaveEntry::XACTWaveEntry, folded), +128 to +160 = 0; returns
// it.
uint32_t SkeletonBaseConstruct(uint8_t* base, uint32_t skeleton);
// Skeleton::Skeleton (sub_82599758): the base, +176 = 0, the vtable;
// returns it.
uint32_t SkeletonConstruct(uint8_t* base, uint32_t skeleton);
// SkeletonBase::Free (sub_8259A228): +128 freed (operator delete[]), +136,
// +144 and +132 (count headers: less 16), all four cleared.
bool SkeletonBaseFree(uint8_t* base, uint32_t skeleton);
// SkeletonBase::~SkeletonBase (sub_8259A9D0): the vtable, then Free.
bool SkeletonBaseDestruct(uint8_t* base, uint32_t skeleton);
// Skeleton::~Skeleton (sub_825999C8): the vtable, +176 freed and cleared,
// then the base's destructor.
bool SkeletonDestruct(uint8_t* base, uint32_t skeleton);
// Skeleton::FreeNodeValues (sub_825997A0): +176 freed and cleared.
bool SkeletonFreeNodeValues(uint8_t* base, uint32_t skeleton);

// For native callers checking before they free (nothing changed): whether
// TrackSetReleaseArray (`array`) or TrackSetRelease would free the set's
// values natively, and whether SkeletonDestruct would.
bool TrackSetReleasable(uint8_t* base, uint32_t set, bool array);
bool SkeletonDestructible(uint8_t* base, uint32_t skeleton);

// sub_825A21A8 NodeFile::View (probably; models.md): the view's +0 = the
// file, +4 to +32 its +16 to +44 made pointers, +36 = +32's + 16, +40 its +48
// made a pointer, +44 = its +4 - its +8.
void NodeFileView(uint8_t* base, uint32_t view, uint32_t file);
// sub_8259A9E0 SkeletonBase::SetUp (probably; models.md): Free, +160 = the
// instance count (1 when not above 0), +128 that many cursors (operator
// new[], each SkeletonCursor::Clear), the node file's view at +80; with
// nodes ([file + 4]): +136 a 144-byte entry a node (+12, +28, +44, +60 =
// 1.0, +64 to +127 and +132 = 0), and +148 `matrices`, or when that's 0
// +132 and +148 a 64-byte matrix a node (+12, +28, +44, +60 = 1.0); with
// [file + 8] entries, +144 a 64-byte entry each (+12, +28, +44 = 1.0, +52 =
// 1); +140 = +136 + 144 x [file + 12]. Arrays have a 16-byte count header
// (operator new[]). False, with nothing done, where the original must run:
// a block Free can't free natively, or a size operator new[] would refuse.
bool SkeletonSetUp(uint8_t* base, uint32_t skeleton, uint32_t file, uint32_t matrices,
                   int32_t instances);
// sub_82599A20 Skeleton::Bind (probably; models.md): SetUp with one
// instance, +176 freed (operator delete[]) and made again (16 bytes a node,
// none without nodes), each node's x, y, z = 0.0 (by Skeleton::NodeCount).
// False, with nothing done, where the original must run (SetUp's, or a +176
// the layer didn't give, or a size operator new[] would refuse).
bool SkeletonBind(uint8_t* base, uint32_t skeleton, uint32_t file, uint32_t matrices);

// SkeletonCursor::Clear (sub_825A2068): +0, +4, +8, +16 = 0, +12 = 0.0.
void SkeletonCursorClear(uint8_t* base, uint32_t cursor);
// SkeletonCursor::View (sub_825A2090): as TrackSet::View, +16 = 0.
void SkeletonCursorView(uint8_t* base, uint32_t cursor, uint32_t section);
// Skeleton::LoadNodeValues (sub_825997E0): with +176, instance 0's cursor
// bound to the section and each entry's node float4 = (+12, +16, +12).
void SkeletonLoadNodeValues(uint8_t* base, uint32_t skeleton, uint32_t section);
// Skeleton::NodeCount (sub_82599880): the node file's count; 0 without
// +176.
uint32_t SkeletonNodeCount(uint8_t* base, uint32_t skeleton);

// What UpdateMatrices did: nothing (no +176), the matrices multiplied
// (Matrix::Multiply leaves flush-to-zero on, as the translated code), or
// handed back to the original with nothing done (a node of type 8 or 16:
// sub_8259B748 and sub_8259BE40, never in play, stay recompiled).
enum class MatricesResult { kNothing, kMultiplied, kOriginal };
// sub_8259C3A8 (models.md): the root's matrix × +16, then in node order
// each node's matrix × its parent's and its +92's translation row copied
// in, then the bones' (+132 flag 0x80) × theirs; each multiply noted for
// interpolation as Matrix::Multiply's adapter does.
MatricesResult SkeletonUpdateNodeMatrices(uint8_t* base, uint32_t skeleton);
// Skeleton::UpdateMatrices (sub_825998A0): with +176, the above.
MatricesResult SkeletonUpdateMatrices(uint8_t* base, uint32_t skeleton);
// sub_825A20E8 (never ran in play): a node channel's keys sampled at
// `time` from the key the cursor word at `cursor` holds, which it moves.
double CursorCurveSample(uint8_t* base, uint32_t keys, uint32_t cursor, uint32_t count,
                         double time);
// Skeleton::ApplyAnimation (sub_825998B8): each entry of instance 0's
// cursor: its node's channels sampled at the node's time into its record.
void SkeletonApplyAnimation(uint8_t* base, uint32_t skeleton);
// sub_82599AF0 Skeleton::ResetPose (models.md): with +176, for each node
// (NodeCount, asked again after each) its 144-byte record (+136) gets the
// 16 bytes at +88, then +92, then +96 (16 bytes a node each), and with its
// node record's (+84, 160 bytes) +132 bit 0x40 that record's +64; then
// Skeleton::ApplyAnimation. Whether it reached ApplyAnimation (the adapter
// leaves flush-to-zero off then, as ApplyAnimation's).
bool SkeletonResetPose(uint8_t* base, uint32_t skeleton);

// --- Beside it in the library. ---

// PrimitiveDrawer::PrimitiveDrawer (sub_825A1378): the vtable 0x820721D4,
// +4 = operator new(4). False, with only the vtable written, when operator
// new would take its error path (the original's, from the start).
bool PrimitiveDrawerConstruct(uint8_t* base, uint32_t drawer);
// sub_825A18D8: a 12-byte record's +0 = 0.0, +4, +8 = 0.
void RecordClear(uint8_t* base, uint32_t record);

}  // namespace model_animation
