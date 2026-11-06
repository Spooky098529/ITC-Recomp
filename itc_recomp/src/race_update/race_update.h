// The race's update natively (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/race-update.md): the race scene's (`[0x826D82D4]`)
// update in its machine's state 38, its scenery load in state 1, and what
// they call. Plain C++ over guest memory (the scene, the cars and the scene
// lights' arrays are shared with the recompiled code), with the originals'
// loads and stores in their order. Adapters: src/hooks/race_update.cpp.

#pragma once

#include <cstdint>

namespace race_update {

// The scene lights' arrays RaceLights::UploadDirectional fills (16 bytes a
// slot): the directions (normalized), and the entries' +32 and +48.
constexpr uint32_t kDirections = 0x8296B950;
constexpr uint32_t kSecond = 0x8296B980;
constexpr uint32_t kThird = 0x8296B9B0;
// RaceLights::Update's (0, 0, 0, 1).
constexpr uint32_t kOrigin = 0x8265ED00;
// RaceLights::Update's frame (its r1 less this).
constexpr uint32_t kLightsFrame = 112;

// sub_8215B330 RaceScene::VaryCar (probably): a seed from the racer index,
// the vehicle index and the racer's +16 (`pick`), then the car's +1568
// bytes it varies by it (+1580 only when the car's word +12 is 64).
// Returns nothing: r3 is the car, as the original leaves it.
void VaryCar(uint8_t* base, uint32_t car, int32_t racer, int32_t vehicle, int32_t pick);

// sub_8215C698 RaceLights::UploadDirectional (probably): `entry`'s +16
// normalized into kDirections + 16 n, +32 into kSecond + 16 n and +48 into
// kThird + 16 n (in that order: the arrays overlap past slot 2). `stack` is
// its r1: the normalized vector also left at stack - 16 (the 1.0 it stores
// at stack - 4 first is overwritten there).
void UploadDirectional(uint8_t* base, uint32_t entry, uint32_t n, uint32_t stack);

// sub_8215D008 RaceLights::Update (probably): kOrigin = (0, 0, 0, 1) (also
// left at the frame's +80), then each of the 11 entries (+16 + 96 i) of kind
// 1 through UploadDirectional with n counting them. `frame` is its r1 less
// kLightsFrame. Returns n.
uint32_t LightsUpdate(uint8_t* base, uint32_t set, uint32_t frame);

}  // namespace race_update
