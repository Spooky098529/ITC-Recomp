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

}  // namespace hooks
