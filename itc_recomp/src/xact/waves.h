// XACT's voice class and waves as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/xact-waves.md): where XACT, the sound
// library linked into the game (translated as the game's code: the user's
// call, 2026-10-23), meets XAudio below it. The voices and waves are guest
// objects shared with the recompiled code (XACT's tracks and events, the
// renderer, XAudio's thread running the engine's audio callback), so these
// work on guest memory. XAudio's entry points are called through their
// plain C++ (src/xaudio/voices.h), each in a frame below the caller's
// (xaudio::FrameBelow) with the registers set as the original sets them;
// XAudio's locks are taken there as the original takes them. The objects
// still recompiled (the owner's vtable, the renderer's slots, the voice's
// methods not made native) are called through the guest call. Adapters:
// src/hooks/xact_waves.cpp.

#pragma once

#include "xact/xact.h"
#include "xaudio/voices.h"

#include <cstdint>
#include <optional>

namespace xact {

using xaudio::GuestCall;
using xaudio::Thread;

// --- The voice (XACTVoice, 24 bytes, vtable 0x82075B98: +4 the XAudio
// source voice, +8 the reverb submix, byte +12 the channels, +16 the reverb
// send level, +20 a flag) ---

constexpr uint32_t kVoiceVtable = 0x82075B98;
constexpr uint32_t kVoiceAttributes = 0x2084802A;  // XMemAlloc's
constexpr uint32_t kVoiceDeletingDestructor = 0x825CA448;

// XACTVoice::Init (sub_825C9B60): XAudioCreateSourceVoice (its plain C++)
// on `description` into +4; on success byte +12 = the description's byte
// +4, and with `reverb` set +8 = the holder's ([holder]) slot +4, +16 =
// [level] and the voice's slot +32 (SetOutputs) with 0, both through the
// guest call. `frame` is this function's (128 bytes). Returns the last r3.
uint32_t VoiceInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t holder,
                   uint32_t level, uint32_t reverb, const Thread& thread, uint32_t frame,
                   const GuestCall& call);
// XACTVoice::Create (sub_825C9C40): 24 bytes (XMemAlloc 0x2084802A; none:
// [out] 0 and E_OUTOFMEMORY), set up and initialized (VoiceInit, a frame
// below); on a failure deleted through its slot +56 (the deleting
// destructor's plain C++ while the slot holds it) and [out] 0, else [out] the
// voice. `frame` is this function's (144 bytes). Allocates through the
// native layer: call it only while hooks::NativeHeapActive().
uint32_t VoiceCreate(uint8_t* base, uint32_t description, uint32_t holder, uint32_t level,
                     uint32_t reverb, uint32_t out, const Thread& thread, uint32_t frame,
                     const GuestCall& call);
// XACTVoice::GetState (sub_825C9DB8, slot +0): XAudioVoice::GetState on +4
// into the frame's +80; on success [out] = 1 when its bit 0 (playing) is
// clear, else 0. `frame` is this function's (112 bytes). Returns XAudio's r3.
uint32_t VoiceGetState(uint8_t* base, uint32_t voice, uint32_t out, uint32_t frame,
                       const GuestCall& call);
// The tail calls on +4, with the XAudio entry point's frame below `sp`
// (the caller's stack pointer): SetVolume (sub_825C9BE8, slot +4, f1),
// SetPitch (sub_825C9BF0, slot +8, f1 the frequency ratio), Start
// (sub_825C9BF8, slot +16, `deferred` != 0 as XAudio's flag), Stop
// (sub_825C9C10, slot +20, `now` != 0 as XAudio's flag), FlushPackets
// (sub_825C9C28, slot +24). Each returns the entry point's r3.
uint32_t VoiceSetVolume(uint8_t* base, uint32_t voice, double volume, uint32_t sp,
                        const GuestCall& call);
uint32_t VoiceSetPitch(uint8_t* base, uint32_t voice, double ratio, uint32_t sp,
                       const GuestCall& call);
uint32_t VoiceStart(uint8_t* base, uint32_t voice, uint32_t deferred, const Thread& thread,
                    uint32_t sp, const GuestCall& call);
uint32_t VoiceStop(uint8_t* base, uint32_t voice, uint32_t now, uint32_t sp,
                   const GuestCall& call);
uint32_t VoiceFlushPackets(uint8_t* base, uint32_t voice, uint32_t sp, const GuestCall& call);
// XACTVoice::SubmitPacket (sub_825C9E08, slot +12): XACT's 40-byte packet
// at `packet` copied into XAudio's 32 bytes at the frame's +80 (its words
// +0, +4, +8 (255 becomes -1), +20 to +36), then the codec's slot +36
// (sub_8245A108) on +4 with it and (`flag` != 0). `frame` is this
// function's (128 bytes). Returns its r3.
uint32_t VoiceSubmitPacket(uint8_t* base, uint32_t voice, uint32_t packet, uint32_t flag,
                           uint32_t frame, const GuestCall& call);
// XACTVoice::~XACTVoice, deleting (sub_825CA448, slot +56): the vtable put
// back, XAudioVoice::Release on +4 when set, and with `flags` bit 0
// XMemFree(this, 0x2084802A). `frame` is this function's (112 bytes).
// Returns the voice. Frees through the native layer: call it only while
// hooks::NativeHeapActive().
uint32_t VoiceDeletingDestruct(uint8_t* base, uint32_t voice, uint32_t flags, uint32_t frame,
                               const GuestCall& call);

// --- The waves (XACTWave: +0 the vtable, +4/+8 the node in the wave
// bank's wave list (+240), +12 the listener interface, +16 stopped at once,
// +20 the engine, byte +24 the loop count, byte +25 the flags, +28 and +32
// the request words, +36 the wave bank, +40 the state, +44 the voice, +48
// the owner, 16-bit +52 the wave index, +56 the next wave, +60 prepared;
// the in-memory wave's +64 to +92 its packets' context) ---

constexpr uint32_t kWaveVtable = 0x82076A38;
constexpr uint32_t kWaveListenerVtable = 0x82076A70;
constexpr uint32_t kInMemoryWaveVtable = 0x82076A74;
constexpr uint32_t kListenerBaseVtable = 0x8203DF00;
constexpr uint32_t kWaveBankAddListener = 0x825C8690;  // the bank's +4 slot +0

// XACTWave::XACTWave (sub_825D23F0): the fields set (byte +25 kept to its
// 0x20 bit) and the wave registered with the bank's +4 interface (its slot
// +0, AddListener with the wave's +12): AddListener's plain C++ while the
// slot holds it and `native_heap`, else through the guest call. `frame` is
// this function's (112 bytes). Returns the wave.
uint32_t WaveConstruct(uint8_t* base, uint32_t wave, uint32_t engine, uint32_t bank,
                       uint32_t index, uint32_t owner, bool native_heap, uint32_t frame,
                       const GuestCall& call);
// XACTInMemoryWave::XACTInMemoryWave (sub_825D37A8): the base (a frame
// below), its vtables, +64 to +92 zeroed, +64 and +80 the wave. `frame` is
// this function's (96 bytes). Returns the wave.
uint32_t InMemoryWaveConstruct(uint8_t* base, uint32_t wave, uint32_t engine, uint32_t bank,
                               uint32_t index, uint32_t owner, bool native_heap, uint32_t frame,
                               const GuestCall& call);
// XACTTrack::GetVolume (sub_825D0268): f1 = +56 x +40 x XACTSound::GetVolume
// of the sound (+20), each product rounded to single; r3 as GetVolume leaves
// it. nullopt where GetVolume's plain C++ declines (nothing changed).
std::optional<CategoryVolume> TrackGetVolume(uint8_t* base, uint32_t track);
// XACTTrack::GetPitch (sub_825D02B0): f1 = +60 + +44 + XACTSound::GetPitch
// of the sound, each sum rounded to single; r3 the sound.
CategoryVolume TrackGetPitch(uint8_t* base, uint32_t track);
// XACTWave::GetStartPosition (sub_825D2170): the start in samples to
// [start] and the loop count left to the byte at `count` for an offset of
// `ms` into the wave's entry (the accessor at `entry`); the count is the
// byte's only while byte +25's bit 0x20 is clear, else 0. A finite count folds
// an offset inside the loops into one pass, less the passes skipped, one
// past them lands after them with the count 0, 255 folds it into the loop.
// Returns 0, or 0x8AC70019 for an offset past the duration ([start] 0).
// nullopt, with nothing changed, where the original traps (a zero loop
// length with the count 255 and the offset past the loop start).
std::optional<uint32_t> WaveGetStartPosition(uint8_t* base, uint32_t wave, uint32_t entry,
                                             uint32_t ms, uint32_t start, uint32_t count);
// XACTWave::RequestNext (sub_825D1C30): +32 exchanged with `value`
// atomically (the original's lwarx/stwcx. loop, as a compare-and-swap on the
// big-endian word, as XAudio's AddStat does).
void WaveRequestNext(uint8_t* base, uint32_t wave, uint32_t value);
// XACTEngine::RequestCommit (sub_825D1C78): the engine's critical section
// (+144) entered, +184 = 1, left; the game's lock, its imports through the
// guest call at their thunks (the user's call, 2026-10-29). Returns
// RtlLeaveCriticalSection's r3.
uint32_t EngineRequestCommit(uint8_t* base, uint32_t engine, const GuestCall& call);

// The voice's methods these call: the plain C++ while the voice's slot holds
// the native function (XACTVoice's vtable), else through the guest call.
constexpr uint32_t kVoiceStart = 0x825C9BF8;         // slot +16
constexpr uint32_t kVoiceStop = 0x825C9C10;          // slot +20
constexpr uint32_t kVoiceFlushPackets = 0x825C9C28;  // slot +24

// XACTWave::SetVoice (sub_825D1DF8): +44 = `voice`; with a voice and an
// owner, the cue's (`[[[[owner + 44] + 16] + 20] + 48]`) outputs: +348 set,
// the voice's slot +28 with the cue + 340, else +316 set, slot +32 with the
// cue + 308; then +72 set, the renderer's ([engine + 12]) slot +24 and the
// voice's slot +48 with the cue's 16-bit +68, that r3's low 16 bits and
// the cue's +64; all through the guest call. `frame` is this function's
// (112 bytes). Returns the last r3.
uint32_t WaveSetVoice(uint8_t* base, uint32_t wave, uint32_t voice, uint32_t frame,
                      const GuestCall& call);
// XACTInMemoryWave::Play (sub_825D3938, slot +8): with an owner, its slot
// +100 (byte +25 bit 0x20 set and a loop count) and +96; unless paused
// (bit 0x80) the voice's Start with `deferred` (a failure returned at
// once), RequestCommit when deferred and bit 0x2 set; then bit 0x40 set and
// the voice's FlushPackets. `frame` is this function's (112 bytes). Returns
// the last r3.
uint32_t InMemoryWavePlay(uint8_t* base, uint32_t wave, uint32_t deferred, const Thread& thread,
                          uint32_t frame, const GuestCall& call);
// XACTWave::Pause (sub_825D1ED8, slot +36): nothing (0) when the paused bit
// (byte +25 bit 0x80, as 0 or 1) is `pause` (compared as a word); else the
// bit set to (`pause` != 0), and with a voice: pausing stops it at once
// (slot +20 with 1); resuming a started wave (0x2) starts it deferred (slot
// +16 with 1; a failure returned), RequestCommit, and flushes again (slot
// +24) when it had been (0x40). `frame` is this function's (112 bytes).
// Returns 0 or the voice call's r3.
uint32_t WavePause(uint8_t* base, uint32_t wave, uint32_t pause, const Thread& thread,
                   uint32_t frame, const GuestCall& call);
// XACTWave::PrepareNext (sub_825D2778, slot +24): when byte +25 has 0x8
// (cleared) and the loop count byte +24 isn't 0: the owner's slot +108 (a
// bank byte at the frame's +80, a 16-bit wave index at +82), the cue's
// sound bank's (+296) wave bank of that byte (XACTSoundBank::GetWaveBank's
// plain C++ where it answers and `native_heap`, else through the guest
// call), its slot +40 (CreateWave, the new wave to the frame's +84), the new
// wave's slot +4 (Prepare) with the count less one (255 kept), whether any
// is left, and 0; on success +56 = the new wave, linked at the tail of the
// bank's wave list (+240); else it's deleted (slot +0 with 1). `frame` is
// this function's (112 bytes). Returns the last r3 (the wave when nothing
// is to do).
uint32_t WavePrepareNext(uint8_t* base, uint32_t wave, bool native_heap, uint32_t frame,
                         const GuestCall& call);

// XACTInMemoryWave::Update (sub_825D3890, slot +24): PrepareNext's plain
// C++ (a frame below), then a finished wave (byte +25 bit 0x10) deletes
// itself (its slot +0 with 1); else a stopped wave (state 5) with a voice
// gives the voice back (the renderer's slot +12) and clears it (SetVoice's
// plain C++ with 0). `frame` is this function's (96 bytes). Returns the
// last r3.
uint32_t InMemoryWaveUpdate(uint8_t* base, uint32_t wave, bool native_heap, uint32_t frame,
                            const GuestCall& call);

}  // namespace xact
