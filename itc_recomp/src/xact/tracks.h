// XACT's tracks, track instances and events as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/xact-tracks.md):
// the layer of XACT, the sound library linked into the game (translated as
// the game's code: the user's call, 2026-10-23), between a sound and the
// waves that play its wave bank entries (src/xact/waves.h). The tracks,
// instances and events are guest objects shared with the recompiled code
// (the sounds' methods, the waves, XAudio's thread running the engine's
// audio callback), so these work on guest memory. Calls through the objects'
// vtables go to the plain C++ here while the slot holds the native function,
// else through the guest call (the waves' and voices' methods, whose
// adapters run their own plain C++). Adapters: src/hooks/xact_tracks.cpp.

#pragma once

#include "xact/waves.h"

#include <cstdint>

namespace xact {

// --- The track instance (XACTTrackInstance, 64 bytes, vtable 0x82076AD8:
// +4/+8 its node, +12 the engine, +16 the track, +20 the events, byte +24
// their count, +28 the start time, byte +32 the flags, +36 the play-wave
// event, +40 the state, byte +44 events not fired, byte +45 fired not done,
// +48 the helper list's head (+52/+56 linked to it), +60 stopped at once) ---

constexpr uint32_t kTrackInstanceVtable = 0x82076AD8;
constexpr uint32_t kListBaseVtable = 0x82075344;  // XACT's base vtable

// XACTTrackInstance::XACTTrackInstance (sub_825D57D0): the fields set, the
// state 1 (created), the helper list empty.
void TrackInstanceConstruct(uint8_t* base, uint32_t instance, uint32_t engine, uint32_t track);
// XACTTrackInstance::CheckDone (sub_825D55E0): the state 6 (done) when no
// event is left to fire or finish and the helper list is empty.
void TrackInstanceCheckDone(uint8_t* base, uint32_t instance);

// The instance's forwarders to its play-wave event (+36), tail calls from
// `sp` (the caller's stack pointer): SetVolume (sub_825D5678, the event's
// slot +64 with f1), IsBusy (sub_825D5780, slot +44), Pause (sub_825D57A8,
// slot +88 with `pause`). Each returns the slot's r3, or 0 without an event.
uint32_t TrackInstanceSetVolume(uint8_t* base, uint32_t instance, double volume, uint32_t sp,
                                const GuestCall& call);
uint32_t TrackInstanceIsBusy(uint8_t* base, uint32_t instance, uint32_t sp,
                             const GuestCall& call);
uint32_t TrackInstancePause(uint8_t* base, uint32_t instance, uint32_t pause, uint32_t sp,
                            const GuestCall& call);
// The track's (+24 its instance): Pause (sub_825D05A8) and IsBusy
// (sub_825D0590), the instance's above, or 0 without one.
uint32_t TrackPause(uint8_t* base, uint32_t track, uint32_t pause, uint32_t sp,
                    const GuestCall& call);
uint32_t TrackIsBusy(uint8_t* base, uint32_t track, uint32_t sp, const GuestCall& call);

// --- The events (XACTEvent, 48 bytes, vtable 0x82076A10: +4/+8 the node in
// the sound's queue, +12 the engine, +16 the record, +20 the fire time, +24
// the done time, +28 the end, +32 the time offset, +36 the previous event,
// byte +40 the flags (0x08 uncounted, 0x10 varied, 0x20 queued, 0x40 done,
// 0x80 fired), +44 the instance) ---

constexpr uint32_t kEventVtable = 0x82076A10;

// XACTEvent::XACTEvent (sub_825D1760): the fields set; byte +40 0x08 when
// `uncounted` is set, else 0.
void EventConstruct(uint8_t* base, uint32_t event, uint32_t engine, uint32_t instance,
                    uint32_t record, uint32_t previous, uint32_t uncounted);
// XACTEvent::~XACTEvent (sub_825D0EF0): XACT's base vtable put back.
void EventDestruct(uint8_t* base, uint32_t event);
// XACTEvent::Fire (sub_825D1358, the base's slot +4): the first time, the
// fired bit and (counted) the instance's byte +45 up; the event's slot +16
// (SetQueued) with 0, from `frame` (this function's, 96 bytes); (counted)
// the instance's byte +44 down. Returns SetQueued's r3.
uint32_t EventFire(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call);

// --- The play-wave event (XACTPlayWaveEvent, 72 bytes, vtable 0x82076CE8:
// +48 a flag, byte +52 the loop count, +56 the variation record, +60 the
// wave, +64 the play time, +68 the stop time) ---

constexpr uint32_t kPlayWaveEventVtable = 0x82076CE8;
constexpr uint32_t kPlayWaveSetQueued = 0x825D8530;  // slot +16
constexpr uint32_t kPlayWaveSetWave = 0x82483208;    // slot +104
constexpr uint32_t kGetWord60 = 0x82090770;          // slot +56: the wave (+60)

// XACTPlayWaveEvent::XACTPlayWaveEvent (sub_825D82E8): the base (counted),
// the vtable, the loop count from the record's byte +11, +56 = `variation`,
// the rest 0. Returns the event.
uint32_t PlayWaveConstruct(uint8_t* base, uint32_t event, uint32_t engine, uint32_t instance,
                           uint32_t record, uint32_t previous, uint32_t variation);
// SetQueued (sub_825D8530, slot +16): byte +40's 0x20 = (`queued` != 0) and
// 0x40 = (the wave's +60 is 0), the wave read unchecked.
void PlayWaveSetQueued(uint8_t* base, uint32_t event, uint32_t queued);
// GetLoopCount (sub_825D8358, slot +48): byte +52.
uint32_t PlayWaveGetLoopCount(uint8_t* base, uint32_t event);
// SetWave (sub_82483208, slot +104): +60 = `wave`. The wave-variation
// event's (sub_825D7B78, slot +104): +72 = `wave`.
void PlayWaveSetWave(uint8_t* base, uint32_t event, uint32_t wave);
void WaveVariationSetWave(uint8_t* base, uint32_t event, uint32_t wave);
// GetVariationRecord (sub_825D8920, slot +92): the record + 12 for type 4,
// + 16 for type 5, else 0.
uint32_t PlayWaveGetVariationRecord(uint8_t* base, uint32_t event);
// ForgetWave (sub_825D8360, slot +28): own slot +104 with 0, a tail call
// from `sp`. Returns its r3.
uint32_t PlayWaveForgetWave(uint8_t* base, uint32_t event, uint32_t sp, const GuestCall& call);

// The slots asking the wave (own slot +56) first, each from `frame`, its own
// (96 bytes; 112 for Pause and SetVolume): GetWaveBank (sub_825D84C0, slot
// +52: the wave's +36, or 0), IsPrepared (sub_825D76A0, slot +20: the wave's
// state +40 is 2 or more), IsPlaying (sub_825D7708, slot +24: 3 to 5), IsBusy
// (sub_825D81E8, slot +44: the wave's slot +20, or 0), Pause (sub_825D8268,
// slot +88: the wave's slot +36 with `pause`, or 0), SetVolume
// (sub_825D7FE0, slot +64: XACTWave::SetVolume with f1, when there's a wave;
// r3 0 without one).
uint32_t PlayWaveGetWaveBank(uint8_t* base, uint32_t event, uint32_t frame,
                             const GuestCall& call);
uint32_t PlayWaveIsPrepared(uint8_t* base, uint32_t event, uint32_t frame,
                            const GuestCall& call);
uint32_t PlayWaveIsPlaying(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call);
uint32_t PlayWaveIsBusy(uint8_t* base, uint32_t event, uint32_t frame, const GuestCall& call);
uint32_t PlayWavePause(uint8_t* base, uint32_t event, uint32_t pause, uint32_t frame,
                       const GuestCall& call);
uint32_t PlayWaveSetVolume(uint8_t* base, uint32_t event, double volume, uint32_t frame,
                           const GuestCall& call);

// XACTWave::SetVolume (sub_825D1CC8): the wave's voice (+44), when set, gets
// its slot +4 with f1, a tail call from `sp`. Returns its r3, or 0 without a
// voice.
uint32_t WaveSetVolume(uint8_t* base, uint32_t wave, double volume, uint32_t sp,
                       const GuestCall& call);

}  // namespace xact
