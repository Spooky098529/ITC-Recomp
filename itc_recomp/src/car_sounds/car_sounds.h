// The car sounds natively (docs/functions/car-sounds.md): the game's own
// sounds for the cars over XAudio's voices, their wave bank views and their
// objects' set-up. Plain C++ over the guest layout; the adapters are in
// src/hooks/car_sounds.cpp (behind --native_car_sounds). XAudio's entry
// points are called through their plain C++ (src/xaudio/), each in a frame
// below the caller's (xaudio::FrameBelow), with the registers set as the
// original sets them before the call.

#pragma once

#include "car_sounds/x3daudio.h"
#include "xaudio/voices.h"

#include <cstdint>
#include <optional>

namespace car_sounds {

// The voice holder's vtable (the 44-byte base of every sound part) and the
// engine layer's.
constexpr uint32_t kHolderVtable = 0x82010B5C;
constexpr uint32_t kLayerVtable = 0x82008018;
constexpr uint32_t kCarSoundBytes = 1264;

// --- The wave bank views (24 bytes over an XWB in memory) ---

// sub_8210C9D8 WaveBankView::Init: the view over `bank`; returns 0 (r3).
uint32_t WaveBankViewInit(uint8_t* base, uint32_t view, uint32_t bank);
// sub_8210C850 WaveBankView::Format: wave `wave`'s PCM format into `out`
// (+0 the sample type, +4 the channels, +8 the rate); 1 (nothing written)
// past the count or for a format other than PCM, else 0.
uint32_t WaveBankFormat(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);
// sub_8210C920 WaveBankView::Length: the entry's +12 into `out`; 1 past the
// count, else 0.
uint32_t WaveBankLength(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);
// sub_8210C958 WaveBankView::Data: the entry's +8 plus the wave data's
// start into `out`; 1 past the count, else 0.
uint32_t WaveBankData(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);
// sub_8210C998 WaveBankView::Entry: the wave's 24-byte entry copied into
// `out` (XMemCpy); 0. Nullopt, with nothing copied, where XMemCpy's
// original must run.
std::optional<uint32_t> WaveBankEntry(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out);

// --- The car sound objects ---

// sub_8210CB48 CarSounds::SetBanks: the car's sound parameters
// [[0x82616318 + 16 i] + 4 j] into the car sound's banks.
constexpr uint32_t kSoundParameters = 0x82616318;
void SetBanks(uint8_t* base, uint32_t sound, uint32_t i, uint32_t j);
// sub_8210CA28 TireSound::Amount: the tires' part's +80 and +76 stepped by
// `f1` (a double, as the register holds it).
void TireSoundAmount(uint8_t* base, uint32_t part, double f1);
// sub_8210CBA0 EngineSound::EngineSound: the engine part's two layers.
void EngineSoundConstruct(uint8_t* base, uint32_t engine);
// sub_8210CC18 CarSound::CarSound: the engine and the ten holders; returns
// the car sound.
uint32_t CarSoundConstruct(uint8_t* base, uint32_t sound);

// sub_8210DCA8 CarSounds::Destroy (the vector deleting destructor): the
// vtables put back, for the array (flags bit 1; the count at r3 - 16) or one
// car sound. Returns r3 as the original leaves it (the array's block or the
// object) and, in `to_free`, the block operator delete (`array`: delete[])
// is to free (flags bit 0), or 0.
struct Destroyed {
  uint32_t r3;
  uint32_t to_free;
  bool array;
};
Destroyed CarSoundsDestroy(uint8_t* base, uint32_t sound, uint32_t flags);

// sub_8210DDC8 CarSound::SetUp: the car sound's X3DAudio listener (+4),
// emitter (+52, its curves and channel azimuths), DSP settings (+176 the
// matrix `matrix`, 24 bytes from operator new[] that the adapter makes
// first; one source and six destination channels) and the floats +1192 to
// +1244 from the game's constants; the vectors +368 and +384 stored as stvx
// does (at their addresses rounded down to 16). Returns the matrix (r3).
uint32_t SetUp(uint8_t* base, uint32_t sound, uint32_t matrix);

// sub_82110258 SoundGlobals::Init, in two parts around its call to
// Music::Play (the adapter's guest call): the music player kept, the
// volumes and the scene models' sound picks set, and bank pair 2's two
// physical buffers made (Memory::AllocPhysicalEx's plain C++, so only while
// the memory blocks are native); then X3DAudioInitialize's plain C++ for
// 5.1 into the handle at 0x826C5C90, whose r3 it returns.
constexpr uint32_t kMusicName = 0x820080FC;
constexpr uint32_t kX3DAudioHandle = 0x826C5C90;
void SoundGlobalsInit(uint8_t* base, uint32_t music_player);
uint32_t SoundGlobalsInit3D(uint8_t* base);

// sub_8210F820 CarSound::Update3D (car_sound_3d.cpp), every tick from
// CarSound::Update for each car: r3 = the car sound, r4 = Update's case
// flag, r5 = the vehicle (all three whole registers). The camera pass
// (0x826BAC00, or its +592 when the race data's +0xA3AF0 is 4 and
// [0x826C2CE8]+8 is set): the listener's position (+28, and +384) its +400,
// its front (+4) its +464 and +472 with y 0; the emitter's position (+80,
// and +368) the vehicle's rigid body's (+336, RigidBody::Position,
// Vec4::Copy), or the camera's when the car is the one at the camera (view
// mode [[0x826C2CE8]]+1860 1 and r4 1; in race mode +0xA3AF4 8 also the
// vehicle's +6148 bit 0x1 or 0x80 by [0x826C7824]); both velocities (+40,
// +92) from the old +384 and +368 (vectors, flush to zero on) times
// [0x82001FD8] and [0x820021E4]; then X3DAudioCalculate's plain C++ (the
// handle 0x826C5C90, flags 0x3D, the DSP settings +176); the output maps
// +272 (the matrix times [0x82616C38], or [0x82002A10] times it for the car
// at the camera) and +224 (+200, the reverb level, times it), their
// descriptors +320 to +356; +1260 passed close; +1256 = +204. `sp` is the
// caller's stack pointer: the original's 288-byte frame (its prologue's
// saves, the back chain, its locals) and Calculate's below it written as
// the originals leave them. `saves` holds the caller's registers the
// prologues save. Returns the r3 the original leaves (Calculate's).
struct Update3DSaves {
  uint32_t lr;
  uint64_t r22, r23, r24, r25, r26, r27, r28, r29, r30, r31;
  uint64_t f23, f24, f25, f26, f27, f28, f29, f30, f31;  // the bits
  uint8_t v126[16], v127[16];                            // as PPCContext holds them
};
uint64_t Update3D(uint8_t* base, uint64_t sound, uint64_t r4, uint64_t vehicle, uint32_t sp,
                  const Update3DSaves& saves, const x3daudio::TanCall& tan);

// --- The voice holders (car_sound_voices.cpp) ---
//
// A voice holder (the 44-byte base of every sound part): four XAudio source
// voices at +4 to +16, their count at +20, which were playing at +24 to +36.
// `frame` is the function's own frame (the original's size, given with each)
// and `call` the guest call for XAudio's calls into the recompiled objects
// and the kernel; each returns the r3 the original leaves.
using xaudio::GuestCall;
using xaudio::Thread;

// sub_8210D158 CarSound::SetPitch (a tail call, no frame: `sp` the caller's
// stack pointer): (f1 - [0x8200801C]) x [0x82008020], at least [0x82002A24]
// (r7 set) or [0x82001BE0], or that floor straight when f1 is
// [0x82000A34]; the voice in `slot`'s frequency ratio (XAudio's
// SetFrequencyRatio) with it.
uint32_t SetPitch(uint8_t* base, uint32_t holder, uint32_t slot, double f1, uint32_t r7,
                  uint32_t sp, const GuestCall& call);
// sub_8210D080 CarSound::SetPitchScaled (96 bytes): as SetPitch with the
// difference also times r9 (as a float) x [0x82008024], the floor for r7 0
// that times [0x82001BE0]; then + f3 - [0x82007FC8].
uint32_t SetPitchScaled(uint8_t* base, uint32_t holder, uint32_t slot, double f1, double f3,
                        uint32_t r7, uint32_t r9, uint32_t frame, const GuestCall& call);
// sub_8210D1D8 CarSound::IsPlaying (112 bytes): 1 when the voice in `slot`
// exists and its state's (GetState, at the frame's +80) first byte has bit
// 0; 0 for the slot -1.
uint32_t IsPlaying(uint8_t* base, uint32_t holder, uint32_t slot, uint32_t frame,
                   const GuestCall& call);
// sub_8210D378 CarSound::UpdateVoices (112 bytes): `output` (r4, the whole
// register) stored at the caller's frame's +24 (the frame's +136); each
// voice whose state has bit 2 stopped, released and cleared, each playing
// one (bit 0) given the output matrix at the frame's +136.
uint32_t UpdateVoices(uint8_t* base, uint32_t holder, uint64_t output, uint32_t frame,
                      const GuestCall& call);
// sub_8210D320 CarSound::Clear (128 bytes): each voice stopped, released and
// cleared; +20 = 0.
uint32_t Clear(uint8_t* base, uint32_t holder, uint32_t frame, const GuestCall& call);
// sub_8210D240 CarSound::Pause (112 bytes): each playing voice stopped and
// noted (+24 + 4i = 1, else 0).
uint32_t Pause(uint8_t* base, uint32_t holder, uint32_t frame, const GuestCall& call);
// sub_8210D2C0 CarSound::Resume (112 bytes): each voice noted started again.
uint32_t Resume(uint8_t* base, uint32_t holder, const Thread& thread, uint32_t frame,
                const GuestCall& call);
// sub_8210E2B8 CarSound::PauseAll and sub_8210E350 ResumeAll (112 bytes
// each): Pause or Resume on the car sound's 12 holders.
uint32_t PauseAll(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call);
uint32_t ResumeAll(uint8_t* base, uint32_t sound, const Thread& thread, uint32_t frame,
                   const GuestCall& call);

}  // namespace car_sounds
