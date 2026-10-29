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
#include <optional>

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

// --- The track (XACTTrack, 64 bytes, vtable 0x820765F8: +4/+8 its node in
// the sound's track list, +12 the engine, +16 its record, +20 the sound, +24
// the instance, +28 the next instance, byte +32 the flags, +36 a pending
// transition event, +40 the volume, +44 the pitch, byte +48 the index, +52
// the RPC entries, +56 the RPC volume factor, +60 the RPC pitch) ---

constexpr uint32_t kTrackVtable = 0x820765F8;
constexpr uint32_t kTrackAttributes = 0x20848004;      // XMemAlloc's
constexpr uint32_t kPlayWaveAttributes = 0x2084800D;   // the play-wave event's
constexpr uint32_t kWaveEventBaseVtable = 0x82076B58;  // the wave event base's

// XACTTrack::SetVolume (sub_825D0BF8): +40 = `volume` (0.0 below it); with
// an instance, its SetVolume with XACTSound::GetVolume x +40 x +56, each
// product rounded to single. `frame` is this function's (112 bytes).
// Returns the last r3. nullopt, with only +40 written (the original writes
// it the same), where GetVolume's plain C++ declines.
std::optional<uint32_t> TrackSetVolume(uint8_t* base, uint32_t track, double volume,
                                       uint32_t frame, const GuestCall& call);
// XACTTrack::UpdateVolume (sub_825D0428): with an instance, its SetVolume
// with +40 x GetVolume x +56. `frame` is this function's (112 bytes). r3 as
// the original leaves it; nullopt, with nothing written, where GetVolume's
// plain C++ declines.
std::optional<uint32_t> TrackUpdateVolume(uint8_t* base, uint32_t track, uint32_t frame,
                                          const GuestCall& call);
// XACTTrack::XACTTrack (sub_825D0CE0): the fields set, then SetVolume (a
// frame below `frame`, this function's 96 bytes; the original through the
// guest call where its plain C++ declines) with 1.0, or the gain of the
// record's volume byte (the table at 0x820761F8). Returns the track.
uint32_t TrackConstruct(uint8_t* base, uint32_t track, uint32_t engine, uint32_t sound,
                        uint32_t record, uint32_t index, uint32_t frame, const GuestCall& call);
// XACTTrack::Update (sub_825D0E30): a pending transition event (+36) to
// sub_825D0B08 (never seen in play: through the guest call) and +36 = 0;
// then the empty sub_82106540 on the instance (nothing done). `frame` is
// this function's (96 bytes). Returns the last r3 (the instance).
uint32_t TrackUpdate(uint8_t* base, uint32_t track, uint32_t frame, const GuestCall& call);
// XACTTrack::CreateRpcs (sub_825D06B8) and StartRpcs (sub_825D0800): with
// no RPCs (the sound's record's byte +0 without 0x4) nothing to do: 0, and
// the sound for StartRpcs (r3 as the originals leave them). nullopt, with
// nothing written, with RPCs (never in play: the original runs).
std::optional<uint32_t> TrackCreateRpcs(uint8_t* base, uint32_t track);
std::optional<uint32_t> TrackStartRpcs(uint8_t* base, uint32_t track);
// XACTTrack::~XACTTrack (sub_825D0110): the vtable; the next instance (+28)
// and the instance (+24) deleted (slot +0 with 1, through the guest call);
// the RPC entries (+52, never in play) let go through the guest call as the
// original does; XACT's base vtable. `frame` is this function's (96 bytes).
// Returns the last r3.
uint32_t TrackDestruct(uint8_t* base, uint32_t track, uint32_t frame, const GuestCall& call);
// XACTTrack's deleting destructor (sub_825D0978): ~XACTTrack (a frame below
// `frame`, this function's 112 bytes), then XMemFree(0x20848004) with
// `flags` bit 0. Returns the track. Frees through the native layer: call it
// only while hooks::NativeHeapActive().
uint32_t TrackDeletingDestruct(uint8_t* base, uint32_t track, uint32_t flags, uint32_t frame,
                               const GuestCall& call);

// XACTPlayWaveEvent's deleting destructor (sub_825D8660, slot +0): the
// vtable, the wave (+60) deleted (its slot +0 with 1, through the guest
// call), the wave event base's vtable, ~XACTEvent, and XMemFree(0x2084800D)
// with `flags` bit 0. `frame` is this function's (112 bytes). Returns the
// event. Frees through the native layer: call it only while
// hooks::NativeHeapActive().
uint32_t PlayWaveDeletingDestruct(uint8_t* base, uint32_t event, uint32_t flags,
                                  uint32_t frame, const GuestCall& call);
// ApplyVariation (sub_825D7558, slot +96): own slot +92 (the variation
// record); with one, sub_825D1478 (never seen in play: through the guest
// call). `frame` is this function's (96 bytes). Returns the last r3.
uint32_t PlayWaveApplyVariation(uint8_t* base, uint32_t event, uint32_t frame,
                                const GuestCall& call);
// GetDuration (sub_825D8870, slot +12): the cue's sound bank's wave bank of
// the record's byte +10 (XACTSoundBank::GetWaveBank's plain C++ where it
// answers and `native_heap`, else through the guest call); 0 without one;
// 0x7FFFFFFF when the loop count (the record's byte +11) is 255; else the
// wave's durations (XACTWaveBank::GetWaveDurations's plain C++ where it
// answers, else through the guest call; to the frame's +84 and +80): the
// count x the loop length + the duration in ms, or 0 on a failure. `frame`
// is this function's (112 bytes).
uint32_t PlayWaveGetDuration(uint8_t* base, uint32_t event, bool native_heap, uint32_t frame,
                             const GuestCall& call);

// --- Preparing: the play-wave event's Prepare, the simple sounds' one-track
// prepare ---

constexpr uint32_t kTrackInstanceAttributes = 0x20848008;  // XMemAlloc's
constexpr uint32_t kEventArrayAttributes = 0x20840001;     // the instance's events

// XACTPlayWaveEvent::Prepare (sub_825D86F8, slot +8): the record's time
// (bits 5-28 of word +0) against `offset`: +32 the offset (no seek) when
// it's later, else the time (the seek the rest); the cue's sound bank's wave
// bank (as GetDuration; none: XACTENGINE_E_NOWAVEBANK, not prepared:
// 0x8AC70016); its slot +40 (CreateWave, the wave to the frame's +80, 0
// first), the wave's slot +4 (Prepare) with own slot +48 (the loop count),
// 0 and the seek, own slot +104 (SetWave), the wave linked at the tail of
// the bank's list (+240). A failure deletes the wave (its slot +0 with 1).
// `frame` is this function's (128 bytes). Returns the last result.
uint32_t PlayWavePrepare(uint8_t* base, uint32_t event, uint32_t offset, bool native_heap,
                         uint32_t frame, const GuestCall& call);
// XACTTrackInstance::PrepareSimple (sub_825D53D8): state 2, one event: a
// 4-byte array (XMemAlloc 0x20840001) at +20, a play-wave event (72 bytes,
// 0x2084800D, its constructor's plain C++) in it and at +36, its slot +8
// (Prepare) with `offset` and, on success, its slot +20 (IsPrepared): state
// 3 when prepared. Without memory: the count 0, the event deleted (its slot
// +0), the array freed, E_OUTOFMEMORY. `frame` is this function's (128
// bytes). Returns the result. Allocates through the native layer: call it
// only while hooks::NativeHeapActive().
uint32_t TrackInstancePrepareSimple(uint8_t* base, uint32_t instance, uint32_t record,
                                    uint32_t offset, uint32_t frame, const GuestCall& call);
// XACTTrack::PrepareSimple (sub_825D09D0): CreateRpcs (its plain C++, else
// the original through the guest call), an instance (XMemAlloc 64
// 0x20848008, its constructor's plain C++) at +24 (E_OUTOFMEMORY without
// one); unless the RPCs failed, the instance's PrepareSimple and on success
// StartRpcs (likewise); on a failure the instance deleted (its slot +0 with
// 1, through the guest call) and +24 = 0. `frame` is this function's (128
// bytes). Returns the result. Allocates through the native layer: call it
// only while hooks::NativeHeapActive().
uint32_t TrackPrepareSimple(uint8_t* base, uint32_t track, uint32_t record, uint32_t offset,
                            uint32_t frame, const GuestCall& call);

// --- Cancelling, stopping and deleting instances ---

// XACTTrackInstance::CancelEvents (sub_825D5520): under XACT's lock (the
// engine's +12 critical section at +144, entered and left through its
// imports as the original does: the user's call, 2026-10-29), each queued
// event (byte +40 bit 0x20) and then each helper (the list +48, until it's
// empty) handed to the sound's sub_825CA6F0 (never seen in play: through the
// guest call). `frame` is this function's (112 bytes). Returns the queued
// events' count.
uint32_t TrackInstanceCancelEvents(uint8_t* base, uint32_t instance, uint32_t frame,
                                   const GuestCall& call);
// XACTTrackInstance::~XACTTrackInstance (sub_825D5838): the vtable, its
// node unlinked; with events, CancelEvents (a frame below), each deleted
// (slot +0 with 1, through the guest call) and the array freed (XMemFree
// 0x20840001); the helpers deleted until the list is empty; state 4 or 5
// becomes 6; XACT's base vtable at +48 and +0. `frame` is this function's
// (112 bytes). Returns the last r3. Frees through the native layer: call it
// only while hooks::NativeHeapActive().
uint32_t TrackInstanceDestruct(uint8_t* base, uint32_t instance, uint32_t frame,
                               const GuestCall& call);
// Its deleting destructor (sub_825D5E58, slot +0): the above (a frame below
// `frame`, this function's 112 bytes), XMemFree(0x20848008) with `flags`
// bit 0. Returns the instance. Native layer only, as above.
uint32_t TrackInstanceDeletingDestruct(uint8_t* base, uint32_t instance, uint32_t flags,
                                       uint32_t frame, const GuestCall& call);
// XACTTrackInstance::Stop (sub_825D5D18): nothing when done (6), or stopping
// (5) unless this is the first immediate stop; state 5, an immediate stop
// +60 = 1 and CancelEvents; a playing play-wave event (its slot +24) gets
// its slot +60 (Stop) with `immediate` (through the guest call); an
// immediate stop that cancelled nothing ends in state 6. `frame` is this
// function's (112 bytes). Returns 0.
uint32_t TrackInstanceStop(uint8_t* base, uint32_t instance, uint32_t immediate, uint32_t frame,
                           const GuestCall& call);
// XACTTrack::Stop (sub_825D01D8): with an instance, immediate when asked or
// when its play-wave event is missing or not playing, the instance's Stop;
// a next instance (+28) to sub_825C1548 with the engine (never seen in
// play: through the guest call) and +28 = 0. `frame` is this function's
// (112 bytes). Returns Stop's result (0 without an instance).
uint32_t TrackStop(uint8_t* base, uint32_t track, uint32_t immediate, uint32_t frame,
                   const GuestCall& call);

// --- Making events ---

// XACTEvent::Create (sub_825D0F00): by the record's type (word +0, low 5
// bits): the play-wave kinds 1, 4 (no variation) and 2, 5 (the word at the
// record + 12 as the variation) as 72 bytes (XMemAlloc 0x2084800D) through
// the play-wave event's constructor's plain C++, the event to [out]; 20
// makes nothing; above 20 XACTENGINE_E_INVALIDEVENTTYPE (0x8AC70011). Then
// E_OUTOFMEMORY when [out] is 0, else 0. nullopt, with nothing written, for
// the other types (never seen in play: the original runs). Allocates through
// the native layer: call it only while hooks::NativeHeapActive().
std::optional<uint32_t> EventCreate(uint8_t* base, uint32_t engine, uint32_t instance,
                                    uint32_t record, uint32_t previous, uint32_t out,
                                    const GuestCall& call);

// --- Preparing a complex sound's track ---

// XACTTrackInstance::Prepare (sub_825D5A48): state 2; the track's event
// records walked through the cue's sound bank's slots +24 (FirstEvent, the
// iterator at the frame's +88 holding `key`, the count back in its byte +8)
// and +28 (NextEvent), their plain C++ while the slots hold them; an array
// of count words (XMemAlloc 0x20840001) at +20; for each record its event
// (XACTEvent::Create's plain C++, else the original through the guest call;
// the previous event the array's entry at the record's byte +6, or none
// with bit 0x20000000); a marker (9) before `offset` dropped, else its slot
// +8 (Prepare) with `offset`; a play-wave kind (1 to 6) becomes +36; kept
// events in the array. XACTENGINE_E_SEEKTIMEBEYONDCUEEND drops the event
// (the result when nothing is kept); any other failure deletes every event
// (slot +0 with 1, through the guest call), frees the array and clears +36.
// At the end the play-wave event's slot +20 (IsPrepared) and state 3.
// `frame` is this function's (192 bytes). Returns the result. Allocates
// through the native layer: call it only while hooks::NativeHeapActive().
uint32_t TrackInstancePrepare(uint8_t* base, uint32_t instance, uint32_t key, uint32_t offset,
                              uint32_t frame, const GuestCall& call);
// XACTTrack::CreateInstance (sub_825D02F8): an instance (XMemAlloc 64
// 0x20848008, its constructor's plain C++), its Prepare with `key` and
// `offset`; on success the instance to [out]; on a failure E_OUTOFMEMORY or
// Prepare's result, the instance deleted (its slot +0 with 1, through the
// guest call). `frame` is this function's (128 bytes). Native layer only,
// as above.
uint32_t TrackCreateInstance(uint8_t* base, uint32_t track, uint32_t key, uint32_t out,
                             uint32_t offset, uint32_t frame, const GuestCall& call);
// XACTTrack::Prepare (sub_825D0AA0): CreateRpcs (its plain C++, else the
// original through the guest call); when it succeeds, CreateInstance into
// the frame's +80 (0 first) and on success +24 = the instance and StartRpcs
// (likewise). `frame` is this function's (128 bytes). Returns CreateRpcs'
// failure or CreateInstance's result. Native layer only, as above.
uint32_t TrackPrepare(uint8_t* base, uint32_t track, uint32_t key, uint32_t offset,
                      uint32_t frame, const GuestCall& call);

}  // namespace xact
