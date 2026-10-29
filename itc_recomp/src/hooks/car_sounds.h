// The car sounds' register steps (src/car_sounds/,
// docs/functions/car-sounds.md), shared by the adapters
// (src/hooks/car_sounds.cpp, behind --native_car_sounds) and the harness
// (without the flag). Each takes the original's registers, leaves r3 as the
// original does, and runs the original (`__imp__`) where the plain C++
// can't.

#pragma once

#include "guest.h"

namespace hooks {

// The wave bank views: r3 = the view, r4 = the wave (the bank for Init),
// r5 = the out block.
void CarSoundsWaveBankViewInit(PPCContext& ctx, uint8_t* base);  // sub_8210C9D8
void CarSoundsWaveBankFormat(PPCContext& ctx, uint8_t* base);    // sub_8210C850
void CarSoundsWaveBankLength(PPCContext& ctx, uint8_t* base);    // sub_8210C920
void CarSoundsWaveBankData(PPCContext& ctx, uint8_t* base);      // sub_8210C958
void CarSoundsWaveBankEntry(PPCContext& ctx, uint8_t* base);     // sub_8210C998

// r3 = a car sound, r4 and r5 the indices (r3 left).
void CarSoundsSetBanks(PPCContext& ctx, uint8_t* base);  // sub_8210CB48
// r3 = the tires' part, f1 (r3 left).
void CarSoundsTireSoundAmount(PPCContext& ctx, uint8_t* base);  // sub_8210CA28
// r3 = the engine part (r3 left).
void CarSoundsEngineSoundConstruct(PPCContext& ctx, uint8_t* base);  // sub_8210CBA0
// r3 = a car sound; returns it.
void CarSoundsCarSoundConstruct(PPCContext& ctx, uint8_t* base);  // sub_8210CC18
// r3 = the array or a car sound, r4 = the flags; operator delete through its
// plain C++ (the guest's where that refuses).
void CarSoundsDestroy(PPCContext& ctx, uint8_t* base);  // sub_8210DCA8

// r3 = a car sound; returns the matrix from operator new[] (its plain C++,
// the guest's where that refuses).
void CarSoundsSetUp(PPCContext& ctx, uint8_t* base);  // sub_8210DDC8
// r3 = the music player; r3 = X3DAudioInitialize's. Music::Play through its
// register step from a 112-byte frame like the original's. Only while the
// memory blocks are native (CarSoundsGlobalsNative), else the original.
bool CarSoundsGlobalsNative();
void CarSoundsGlobalsInit(PPCContext& ctx, uint8_t* base);  // sub_82110258

// X3DAudio (car_sounds/x3daudio.h); each turns the host's flush to zero off
// first, as the original's float instructions do. The vectors: r3 = a,
// r4 = b, r5 = the out vector (r3 left); Length and Dot into f1.
void CarSoundsX3DVecSub(PPCContext& ctx, uint8_t* base);     // sub_824819E0
void CarSoundsX3DVecLength(PPCContext& ctx, uint8_t* base);  // sub_82481998
void CarSoundsX3DVecDot(PPCContext& ctx, uint8_t* base);     // sub_824819B8
void CarSoundsX3DVecCross(PPCContext& ctx, uint8_t* base);   // sub_82481A18
// r3 = the curve, f1 = the scaler, f2 = the distance; f1 (r3 left).
void CarSoundsX3DEvaluateCurve(PPCContext& ctx, uint8_t* base);  // sub_82480818
// r3 = the handle, r4 = the emitter, r5 = the flags, r6 = the DSP settings:
// the original's 128-byte frame below r1 written as it leaves it (the back
// chain, the saved lr and r31, the default curves); r3 the last curve
// evaluated, as the original leaves it.
void CarSoundsX3DCurves(PPCContext& ctx, uint8_t* base);  // sub_82480918
// r3 = the handle, r5 = the DSP settings, r6 = the source channel, f1 = the
// azimuth: the original's 192-byte frame below r1 written as it leaves it
// (the back chain, the saved lr, r28 to r31 and f31, the azimuth tables);
// r3 as the original leaves it (tan's when its tangent law ran). tan through
// its adapter where c_runtime::Tan can't stand in for it.
void CarSoundsX3DPan(PPCContext& ctx, uint8_t* base);  // sub_82480F80
// r3 = the handle, r4 = the listener, r5 = the emitter, r6 = the DSP
// settings: the original's 304-byte frame below r1 (its prologue's saves,
// the back chain, its locals) and Pan's below that written as the original
// leaves them; r3 as the original leaves it.
void CarSoundsX3DMatrix(PPCContext& ctx, uint8_t* base);  // sub_82481390

// r3 = the handle, r4 = the listener, r5 = the emitter, r6 = the flags,
// r7 = the DSP settings: the original's 160-byte frame below r1 (its
// prologue's saves, the back chain, the vector at +88) written as it leaves
// it; r3 as the original leaves it.
void CarSoundsX3DCone(PPCContext& ctx, uint8_t* base);  // sub_82480BA8
// r3 = the handle, r4 = the listener, r5 = the emitter, r6 = the DSP
// settings: the original's 144-byte frame below r1 as it leaves it; r3 as
// the original leaves it.
void CarSoundsX3DDoppler(PPCContext& ctx, uint8_t* base);  // sub_82480E90
// r3 = the handle, r4 = the listener, r5 = the emitter, r6 = the flags,
// r7 = the DSP settings: the original's 144-byte frame and its callees'
// below it as the originals leave them; r3 as the original leaves it. The
// original itself (`__imp__`) when the flags ask for 0x40 or 0x2
// (EmitterAngle, Delay: never seen in play).
void CarSoundsX3DCalculate(PPCContext& ctx, uint8_t* base);  // sub_82481880

// r3 = a car sound, r4 = CarSound::Update's case flag, r5 = the vehicle: the
// original's 288-byte frame (its prologue's saves, the back chain, its
// locals) and X3DAudioCalculate's below it written as the originals leave
// them; r3 as the original leaves it (Calculate's).
void CarSoundsUpdate3D(PPCContext& ctx, uint8_t* base);  // sub_8210F820

// The voice holders (car_sounds.h): r3 = the holder (the car sound for
// PauseAll and ResumeAll), r4 = the slot (UpdateVoices: the output), as the
// originals take them; XAudio's entry points through their plain C++, in the
// original's frame (hooks::XAudioFramed).
void CarSoundsSetPitch(PPCContext& ctx, uint8_t* base);        // sub_8210D158 (f1, r7)
void CarSoundsSetPitchScaled(PPCContext& ctx, uint8_t* base);  // sub_8210D080 (f1, f3, r7, r9)
void CarSoundsIsPlaying(PPCContext& ctx, uint8_t* base);       // sub_8210D1D8
void CarSoundsUpdateVoices(PPCContext& ctx, uint8_t* base);    // sub_8210D378
void CarSoundsClear(PPCContext& ctx, uint8_t* base);           // sub_8210D320
void CarSoundsPause(PPCContext& ctx, uint8_t* base);           // sub_8210D240
void CarSoundsResume(PPCContext& ctx, uint8_t* base);          // sub_8210D2C0
void CarSoundsPauseAll(PPCContext& ctx, uint8_t* base);        // sub_8210E2B8
void CarSoundsResumeAll(PPCContext& ctx, uint8_t* base);       // sub_8210E350

// Making and starting the voices: r3 = the holder or part (the car sound
// for Start), r4 = the view (the index for the parts' Start), r5 = the wave
// (the views), r6 (PlayOnce: r8) = loop, f1 and f2 PlayOnce's volume and
// pitch; XAudioCreateSourceVoice with the native heap's default allocator
// while --native_heap routes the heap.
void CarSoundsAddVoice(PPCContext& ctx, uint8_t* base);          // sub_8210CE38
void CarSoundsPlayOnce(PPCContext& ctx, uint8_t* base);          // sub_8210E3E8
void CarSoundsEngineLayerStart(PPCContext& ctx, uint8_t* base);  // sub_8210D410
void CarSoundsEngineSoundStart(PPCContext& ctx, uint8_t* base);  // sub_8210D6C0
void CarSoundsTireSoundStart(PPCContext& ctx, uint8_t* base);    // sub_8210D7F8
void CarSoundsCarSoundStart(PPCContext& ctx, uint8_t* base);     // sub_8210E030
// r3 = the car sound (none for StopAll and CarSounds::Release).
void CarSoundsStop(PPCContext& ctx, uint8_t* base);              // sub_8210E218
void CarSoundsStopAll(PPCContext& ctx, uint8_t* base);           // sub_8210F7C8
void CarSoundsRelease(PPCContext& ctx, uint8_t* base);           // sub_8210E170
void CarSoundsReleaseAll(PPCContext& ctx, uint8_t* base);        // sub_8210F750

// No registers in (r3 SetEffectParameters').
void CarSoundsInit(PPCContext& ctx, uint8_t* base);  // sub_8210FE00

// The parts' updates: r3 = the part, r4 = the vehicle, r5 = the DSP output
// (PassBySound: r6 the pass flag); PlayOnce's voices with the native heap's
// default allocator while --native_heap routes the heap.
void CarSoundsHitImpact(PPCContext& ctx, uint8_t* base);        // sub_8210E778
void CarSoundsHitScrape(PPCContext& ctx, uint8_t* base);        // sub_8210E938
void CarSoundsHitWave9(PPCContext& ctx, uint8_t* base);         // sub_8210EB90
void CarSoundsHitSoundsUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210FD78
void CarSoundsPart736Update(PPCContext& ctx, uint8_t* base);    // sub_8210EBF0
void CarSoundsPart840Update(PPCContext& ctx, uint8_t* base);    // sub_8210ECF0
void CarSoundsPart888Update(PPCContext& ctx, uint8_t* base);    // sub_8210EEE0
void CarSoundsWindSoundUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210EDB0
void CarSoundsPassBySoundUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210F088

// The engine: EngineLayer::Update (r3 the layer, f1, f2, f4, f5, r7 the
// output), UpdatePitched (r3 the engine part, f1-f4, r6 the output) and
// EngineSound::Update (r3 the engine part, r4 the vehicle, r5 the output,
// f1, f2).
void CarSoundsEngineLayerUpdate(PPCContext& ctx, uint8_t* base);         // sub_8210D478
void CarSoundsEngineLayerUpdatePitched(PPCContext& ctx, uint8_t* base);  // sub_8210D718
void CarSoundsEngineSoundUpdate(PPCContext& ctx, uint8_t* base);         // sub_8210E470

// The tires: TireLayer::Update (r3 the part, r4 the layer, r5 active, r7 the
// output, f1) and TireSound::Update, and RollSound::Update (each r3 the part,
// r4 the vehicle, r5 the output).
void CarSoundsTireLayerUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210D890
void CarSoundsTireSoundUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210E5C0
void CarSoundsRollSoundUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210DB60

// The horn (r3 the part, r4 the vehicle, r5 the output, f1 the car sound's
// +204) and the turbo (r3, r4, r5).
void CarSoundsHornSoundUpdate(PPCContext& ctx, uint8_t* base);   // sub_8210F118
void CarSoundsTurboSoundUpdate(PPCContext& ctx, uint8_t* base);  // sub_8210F4E8

}  // namespace hooks
