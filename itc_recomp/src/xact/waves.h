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

}  // namespace xact
