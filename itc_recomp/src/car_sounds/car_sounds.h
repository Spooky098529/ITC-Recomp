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

// --- Making and starting the voices (car_sound_voices.cpp) ---

// What making a voice needs beside the guest call: the thread's words and
// whether XAudio's default allocator may run natively (the native heap).
struct Voices {
  Thread thread;
  bool native_heap;
};

// The submix voice every car's voices play into.
constexpr uint32_t kSubmix = 0x826C5C44;
// The car sounds' array (the count 7 at [-16]).
constexpr uint32_t kSounds = 0x826C5C3C;

// sub_8210CE38 CarSound::AddVoice (496 bytes): in the holder's first free
// slot (none: -1), a source voice of wave 0's format (WaveBankView::Format
// into the description at the frame's +304, its outputs at +144: the null
// voice and the submix, each with the six-channel map at +80) made by
// XAudioCreateSourceVoice, then wave `wave`'s buffer submitted (the packet
// at +208: the data, the length, with `loop` (r6) the loop region from the
// entry copied to +176, in bytes for 8- or 16-bit samples); +20 up by one.
// Returns the slot.
uint32_t AddVoice(uint8_t* base, uint32_t holder, uint32_t view, uint32_t wave, uint32_t loop,
                  const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210E3E8 CarSound::PlayOnce (128 bytes): AddVoice (`loop` r8), then
// the voice started, its volume `volume` (f1) and pitch `pitch` (f2,
// SetPitch with r7 0). Returns the slot or -1.
uint32_t PlayOnce(uint8_t* base, uint32_t holder, uint32_t view, uint32_t wave, uint32_t loop,
                  double volume, double pitch, const Voices& voices, uint32_t frame,
                  const GuestCall& call);
// sub_8210D410 EngineLayer::Start (112 bytes): +4 to +16 and the 24 bytes
// at +20 cleared, four looping voices of waves 0-3 from `view` into the
// holder at +44.
uint32_t EngineLayerStart(uint8_t* base, uint32_t layer, uint32_t view, const Voices& voices,
                          uint32_t frame, const GuestCall& call);
// sub_8210D6C0 EngineSound::Start (112 bytes): layer +84 from [views],
// layer +0 from [views + 4] (r4 `index` as the original passes it); +168
// = [0x82000A34], +172 = [0x82003384].
uint32_t EngineSoundStart(uint8_t* base, uint32_t engine, uint64_t index, uint32_t views,
                          const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210D7F8 TireSound::Start (128 bytes): +68 the index's low byte, +64
// the view, +72 and +80 [0x82000A34], +70 and +76 0, two looping voices of
// wave 0 into the holder at +24, then its state words.
uint32_t TireSoundStart(uint8_t* base, uint32_t part, uint32_t index, uint32_t view,
                        const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210E030 CarSound::Start (128 bytes): the engine (two views at
// `views`), the tires and each part's bank `view` and index (+404), their
// state words; the roll noise's looping voice of wave 1 (+792).
uint32_t CarSoundStart(uint8_t* base, uint32_t sound, uint32_t views, uint32_t view,
                       const Voices& voices, uint32_t frame, const GuestCall& call);

// sub_8210E218 CarSound::Stop (112 bytes): Clear on the 12 holders, +1248
// = 0.
uint32_t Stop(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call);
// sub_8210F7C8 CarSounds::StopAll (112 bytes): Stop on each of the seven at
// [0x826C5C3C] (on 0 each time when the array is 0, as the original).
uint32_t StopAll(uint8_t* base, uint32_t frame, const GuestCall& call);
// sub_8210E170 CarSound::Release (112 bytes): Clear on the 12 holders, the
// matrix (+176) freed (operator delete[]'s plain C++, its original through
// `call` where that refuses), +1248 = 0.
uint32_t Release(uint8_t* base, uint32_t sound, uint32_t frame, const GuestCall& call);
// sub_8210F750 CarSounds::Release (112 bytes): Release on each of the
// seven, the array destroyed (CarSounds::Destroy with 3, its frees through
// operator delete[]'s plain C++) and the submix voice released.
uint32_t CarSoundsRelease(uint8_t* base, uint32_t frame, const GuestCall& call);

// sub_8210FE00 CarSounds::Init (368 bytes): the seven car sounds (operator
// new[] of 8,864 bytes, the count 7 at +0, each made by CarSound::CarSound)
// into [0x826C5C3C], cleared (8,848 bytes) and each made again and set up
// (+1252 [0x82007FC8], +404 its index, +400, +0 and +1248 0, SetUp with its
// matrix from operator new[]); [0x826C5C40] = 0; the six-channel 48 kHz
// submix voice (XAudioCreateSubmixVoice: the description at the frame's
// +224, its map at +208 and +96, its outputs at +144 and +256) into
// [0x826C5C44] and its effect parameters (SetEffectParameters r4 0, r5 1,
// r6 3, the block 0x82616638 at the frame's +80). Operator new[] through
// its plain C++, the guest's through `call` where that refuses. Returns
// SetEffectParameters' r3.
uint32_t CarSoundsInit(uint8_t* base, const Voices& voices, uint32_t frame,
                       const GuestCall& call);

// --- The parts' updates (car_sound_parts.cpp), from CarSound::Update ---
//
// `part` is the part's holder (r3), `vehicle` the vehicle (r4), `output` the
// DSP output word (r5, the whole register) UpdateVoices is given; each
// returns the r3 the original leaves. One-shot waves through PlayOnce with
// the holder's view (+40).

// sub_8210E778 (144 bytes; probably the impact): with the vehicle's +6152
// bit 0, the strength of the collision object's (+340) first contact of
// mask 0xFFFFFF; wave 7 + its level (strength x [0x82008054], at most
// [0x820021E8], fctidz into the frame's +80) when it passes [0x82002A0C]
// with +64 -1, or [0x82001FC0] with +64 at least 0; then +48 the ticks,
// +60 the strength (fctiwz), +64 0.
uint32_t HitImpact(uint8_t* base, uint32_t part, uint32_t vehicle, const Voices& voices,
                   uint32_t frame, const GuestCall& call);
// sub_8210E938 (192 bytes; probably the scrape): in contact (+6152 bit 1
// without bit 0), the first contact of mask 0xFF000000's strength times the
// speed; the looping wave 10 (+56 its slot) played or its volume set, and
// wave 7 + the level above [0x82001FC0]; out of contact, the looping voice
// stopped after two ticks (+52).
uint32_t HitScrape(uint8_t* base, uint32_t part, uint32_t vehicle, const Voices& voices,
                   uint32_t frame, const GuestCall& call);
// sub_8210EB90 (96 bytes): wave 9 on the vehicle's +6152 bit 2, +48 = 32.
uint32_t HitWave9(uint8_t* base, uint32_t part, uint32_t vehicle, const Voices& voices,
                  uint32_t frame, const GuestCall& call);
// sub_8210FD78 HitSounds::Update (112 bytes): the three above while +48 is 0
// (else +48 down by one), +64 counted up to 32 (then -1), UpdateVoices.
uint32_t HitSoundsUpdate(uint8_t* base, uint32_t part, uint64_t vehicle, uint64_t output,
                         const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210EBF0 (112 bytes; the part at +736): nothing while the vehicle's
// +2252 is 0; wave 4 when its byte +2948 sets and +48 is 0 (+52 the slot,
// +48 32 ticks), its volume +2952 by the car's specs (+5712: byte +2019,
// word +1656); +48 counted down while the byte is clear; UpdateVoices.
uint32_t Part736Update(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                       const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210ECF0 (128 bytes; the part at +840): wave 6 on the vehicle's byte
// +2504, wave 18 on +2960 when the byte +45 is 0 (+45 = 2, down to 0 a
// tick); UpdateVoices.
uint32_t Part840Update(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                       const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210EEE0 (144 bytes; the part at +888): the looping wave 17 started on
// the vehicle's byte +2424 (+48 1, +52 the slot), its volume faded in (+60)
// and out (+56) by [0x82007FC8] a tick, stopped at 0.0; UpdateVoices.
uint32_t Part888Update(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                       const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210EDB0 WindSound::Update (128 bytes): wave 5 once (+48 1) while a
// halfword at the vehicle's +5410 + 96i (i 0-3) has bit 0 and the speed
// passes [0x8200257C], its volume by the speed and the view mode
// ([[0x826C2CE8]]+1860); +48 back to 0 when none has; UpdateVoices.
uint32_t WindSoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                         const Voices& voices, uint32_t frame, const GuestCall& call);
// sub_8210F088 PassBySound::Update (112 bytes): nothing without a vehicle;
// wave 15 when `passed` (r6) and the vehicle's +6148 & 0x81 is clear;
// UpdateVoices.
uint32_t PassBySoundUpdate(uint8_t* base, uint32_t part, uint32_t vehicle, uint64_t output,
                           uint32_t passed, const Voices& voices, uint32_t frame,
                           const GuestCall& call);

}  // namespace car_sounds
