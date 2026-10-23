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

}  // namespace model_animation
