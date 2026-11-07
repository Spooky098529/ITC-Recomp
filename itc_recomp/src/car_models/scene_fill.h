// The car scene's set-up from a record as plain C++ (docs/functions/car-models.md,
// CarScene::SetUp sub_820C8DE8): the record copied into the scene, the paint
// palette, the load buffer, the shared assets' loads, the second set's
// eight pools filled and the first set's made (ScenePools::CreateA). Over
// the allocation layer (its VirtualAllocs are the layer's pages), so it
// runs natively only while the adapter's checks hold. Adapter:
// src/hooks/car_models.cpp.

#pragma once

#include "archive/archive.h"
#include "scene_loaders/pools.h"

#include <cstdint>

namespace car_models {

// Its frame (stwu) and the second set's objects' sizes, by pool (0 the car
// model to 7 the scene model).
constexpr uint32_t kSceneSetUpFrame = 144;
constexpr uint32_t kSecondSetBytes[8] = {3744, 37296, 4320, 11680, 13168, 16800, 9808, 5120};

// CarScene::SetUp (sub_820C8DE8): r3 = the scene, r4 = a 48-byte record, r5
// a word kept at the scene's +5044. The scene's +0 = 3; +4 = rec +0, +8 =
// rec +4, +12 = rec +8, +16 = rec +12 + rec +16 + rec +32, +20 = rec +16,
// +24 = rec +20, +28 = rec +24 + rec +32 + rec +28, +32 = rec +32 + rec +28,
// +36 = rec +32, +40 = rec +36, +44 = rec +40 (in the original's order);
// PaintPalette::Init (its frame below this one's); +5056, +5060 = the
// archive's shared load buffer and its size ([0x826A5134], [0x826A5130])
// when rec +36 is 0, else the scene's +40, +44; CarScene::LoadAssets (`pcr`
// and `load` its own); then for pools 0 to 7 (counts at +8, +12, +20, +16,
// +24, +28, +36, +32), each object: VirtualAlloc(0, size, MEM_COMMIT,
// read-write), its constructor's plain C++ with the pool's list (+4944 + 12
// x pool) and this frame, its +20 = the index (at address 20 when the
// allocation fails, as the original); then ScenePools::CreateA on the first
// set (+5040) with the record (`create` its constructors' fallbacks).
// `sp` is the caller's stack pointer. Returns CreateA's r3.
uint32_t SceneSetUp(uint8_t* base, uint32_t scene, uint32_t record, uint32_t r5, uint32_t sp,
                    uint32_t pcr, const archive::LoadCall& load,
                    const scene_loaders::CreateCalls& create);

}  // namespace car_models
