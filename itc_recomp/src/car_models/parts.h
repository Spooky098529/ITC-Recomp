// The car's part loads as plain C++ (B7 in docs/rexglue-removal.md;
// docs/functions/car-models.md): the wheels (scene pool 5) with their rims'
// and brakes' asset ids, the cockpit body's set (scene pool 2) and the
// interior's parts (scene pool 3). Guest data shared with the recompiled code, so
// these work on guest memory with the originals' loads and stores in their
// order. Adapters: src/hooks/car_models.cpp.

#pragma once

#include "archive/archive.h"

#include <cstdint>
#include <functional>
#include <optional>

namespace car_models {

// The callback the wheels' loads hand Archive::Load (not mapped).
constexpr uint32_t kOnWheelPartLoaded = 0x820D8D20;

// sub_820D8BA8 (the rims): the front's (+44) and rear's (+60) wheel number
// n: -1 gives, for the car's block b ([[+24] + 32]) up to 19 (signed), the
// XPR2 587 + 3b and the rim 588 + 3b, else 695 and 696; n gives 695 + 3n and
// 696 + 3n. The XPR2s into +2144 and +2168, the rims into +1168 and +1192.
void WheelsRims(uint8_t* base, uint32_t wheels);

// sub_820D8C50 (the brakes): as the rims with 589 + 3b and 697 + 3n for the
// part; for traffic (+28 = 3), wheel 20 + [[+3264]] for both ends.
void WheelsBrakes(uint8_t* base, uint32_t wheels);

// sub_820D9110 (the wheels' set-up): the 156-byte set-up copied to +28
// (memcpy, native::Buffers_Memcpy), the rims or (by [[+3268] + 8] not 0) the
// brakes, then for the front and rear (i 0, 1) through LoadSlot::LoadShared
// with the wheels and kOnWheelPartLoaded: the tire 582, or 584 when +144 +
// 4i isn't 0, into +192 + 24i; the part into +1168 + 24i and the XPR2 into
// +2144 + 24i (their own +0's ids); the calipers 887 into +2280 + 24i.
// Returns the last LoadShared's r3; nullopt, with nothing done, where the
// copy can't be native (the original runs).
std::optional<uint32_t> WheelsSetUp(uint8_t* base, uint32_t wheels, uint32_t setup,
                                    const archive::LoadCall& load);

// The callbacks the cockpit body's and the interior's loads hand
// Archive::Load: sub_820D6528 and Interior::OnPartLoaded (sub_820D7578).
constexpr uint32_t kOnCockpitPartLoaded = 0x820D6528;
constexpr uint32_t kOnInteriorPartLoaded = 0x820D7578;
// The format sub_820D67E8 prints the block with when the node file is empty.
constexpr uint32_t kCockpitNodeFormat = 0x82005024;

// CarInterior::LoadSetCBody (sub_820D67E8): the 144-byte set-up copied to
// +28 (memcpy, native::Buffers_Memcpy); with b the car's block ([[+24] +
// 32]): LoadSlot::Load of the node file 19928 + 55b into +176 (when it
// loads nothing, sprintf of b into a buffer in the frame at stack - 1152 +
// 80, with the format at kCockpitNodeFormat; nothing reads it); the body
// 19926 + 55b into the shared load buffer ([0x826C2E44] + 5056, read before
// +400 = id): +408 the size (not compared), +404 = Archive::Load(id, the
// buffer, kOnCockpitPartLoaded, the object), +420 = 2; then six parts with
// LoadSlot::LoadShared into +536 + 24i: body + 3 + [+28], + 16 + [+32], + 19
// + [+36], + 31 + [+40], + 36 + [+44], + 35. Returns the last LoadShared's
// r3; nullopt, with nothing done, where the copy can't be native (the
// original runs). Only while --native_heap routes VirtualAlloc to the layer.
std::optional<uint32_t> CockpitLoadSetC(uint8_t* base, uint32_t object, uint32_t setup,
                                        uint32_t stack, uint32_t pcr,
                                        const archive::LoadCall& load);

// CarInterior::LoadParts (sub_820D7730): the six assets [[+24] + 32] x
// `stride` + `first` + `offset` + i (signed 32-bit product) through
// LoadSlot::LoadShared into +160 + 24i, with the object and
// kOnInteriorPartLoaded. Returns the last LoadShared's r3.
uint32_t InteriorLoadParts(uint8_t* base, uint32_t object, uint32_t first, uint32_t stride,
                           uint32_t offset, const archive::LoadCall& load);

// The accessories' set's vtable slot +4 (sub_820F0EB8, probably
// Accessories::Load; recompiled): r3 = the set, r4, r5; returns its r3.
// `which` is the call site: 0 set b's (returning to 0x820D7A40), 1 set a's
// and the cockpit's (0x820D7A94).
using AccessoryLoadCall =
    std::function<uint32_t(uint32_t set, uint32_t r4, uint32_t r5, int which)>;

// CarInterior::Load (sub_820D79B0): the accessories' set (sub_820F0E50,
// read first), the 120-byte set-up copied to +28 (memcpy,
// native::Buffers_Memcpy); by the model set s = [[+3092] + 8] and the
// cockpit flag +3088: s = 1 InteriorLoadParts(11061, 197, 190), or (19926,
// 55, 49) with the cockpit, then the set's slot +4 with [+140], [+144]; s = 0
// (891, 226, 219) or the cockpit's, then the slot +4 with (i, 0) for i 0 to
// 2, each answer's +808 = 0; other sets nothing. Returns the last slot +4
// call's r3 (memcpy's, the destination, for other sets); nullopt, with
// nothing done, where the copy can't be native (the original runs).
std::optional<uint32_t> InteriorLoad(uint8_t* base, uint32_t object, uint32_t setup,
                                     const archive::LoadCall& load,
                                     const AccessoryLoadCall& accessory_load);

// --- The first set's pools' set-ups (ScenePools::SetUp and the loaders it
// reaches with rows in scene-models.md) ---

// The callbacks the loaders hand Archive::Load: CarParts::LoadSetA's
// (sub_820D6C10) and LoadSetB's (sub_820D6C88), pool 4's (sub_820D69B8) and
// TrafficCar::Load's (sub_820D8430).
constexpr uint32_t kOnPartsLoadedA = 0x820D6C10;
constexpr uint32_t kOnPartsLoadedB = 0x820D6C88;
constexpr uint32_t kOnPool4Loaded = 0x820D69B8;
constexpr uint32_t kOnTrafficLoaded = 0x820D8430;
// The format TrafficCar::Load prints the car's number with when its load
// gives nothing.
constexpr uint32_t kTrafficFormat = 0x820050BC;

// CarParts::LoadSetA (sub_820D7090): 216 slots from +96 (24 bytes each),
// each with +0 = the asset 894 + 226 b + i (b the car's block, [[+24] +
// 32]) and through LoadSlot::LoadShared with the object and
// kOnPartsLoadedA.
void CarPartsLoadSetA(uint8_t* base, uint32_t object, const archive::LoadCall& load);
// CarParts::LoadSetB (sub_820D70F0): 17 slots from +96 through
// LoadSlot::LoadShared with kOnPartsLoadedB: the asset 11061 + 197 b plus,
// by the slot, 0 [+28] + 3, 1 7 [+28] + [+32] + 16, 2 [+36] + 107, 3 [+40]
// + 120, 4 [+44] + 133 (none when [+44] is 13), 5 [+48] + 146, 6 [+52] +
// 154, 8 [+56] + 157, 10 [+60] + 169, 11 [+64] + 173, 12 [+68] + 178, 13
// 177, 15 [+76] + 184, 16 189; slots 7, 9 and 14 none.
void CarPartsLoadSetB(uint8_t* base, uint32_t object, const archive::LoadCall& load);
// CarParts::Load (sub_820D7510): the 60-byte set-up copied to +28 (memcpy,
// native::Buffers_Memcpy), then by the scene's set [[+106000] + 8] 0
// LoadSetA, 1 LoadSetB, else nothing. False, with nothing done, where the
// copy can't be native (the original runs).
bool CarPartsLoad(uint8_t* base, uint32_t object, uint32_t setup, const archive::LoadCall& load);
// Pool 4's set-up (sub_820D6B48): the 52-byte record copied to +28
// (memcpy), then one asset by the scene's set ([[+576] + 8]): 0 1116 + 226
// b, else 11257 + 197 b, into the slot at +80 through LoadSlot::LoadShared
// with the object and kOnPool4Loaded. False, with nothing done, where the
// copy can't be native.
bool Pool4Load(uint8_t* base, uint32_t object, uint32_t record, const archive::LoadCall& load);
// TrafficCar::Load (sub_820D8A08): the car record copied to +32
// (CarRecord::Copy), +7392 = [0x826C2E40], LoadSlot::Load of 22529 + the
// record's +0 into +3744 with the object and kOnTrafficLoaded; when that
// gives 0, sprintf of the number with kTrafficFormat into a buffer in its
// frame (stack - 1136 + 80; nothing reads it; the varargs' other words 0);
// then Texture::Create (0x18280086, 16 x 16) into +3728 and +3732, from its
// frame. `stack` is its r1 at entry, `pcr` r13. False, with nothing done,
// where CarRecord::Copy's plain C++ declines (the original runs). Only while
// --native_heap routes the game's heap to the layer.
bool TrafficCarLoad(uint8_t* base, uint32_t object, uint32_t record, uint32_t stack, uint32_t pcr,
                    const archive::LoadCall& load);

// A set-up called through the guest (its adapter, or its original): r3, r4,
// the call's r1 and return address; returns r3.
using SetUpCall =
    std::function<uint32_t(uint32_t function, uint32_t r3, uint32_t r4, uint32_t r1, uint32_t lr)>;

// What ScenePools::SetUp may run natively: the car models' set-ups'
// plain C++ (--native_car_models), the loaders above (--native_scene_models)
// and what needs the allocation layer (--native_heap).
struct PoolSetUps {
  bool car_models;
  bool loaders;
  bool heap;
};

// ScenePools::SetUp (sub_820C4338): the pool's object `index` (List::At on
// the first set's +56 + 12 pool); 0 unless it's in use (+16 = 3), else the
// object, after its set-up with `record` by the pool: 0
// CarModel::LoadExterior, 1 CarParts::Load, 2 CarInterior::LoadSetCBody, 3
// CarInterior::Load, 4 Pool4Load, 5 the wheels' set-up, 6 TrafficCar::Load,
// above none. Their plain C++ where `native` allows it and they agree, else
// through `call` from its 112-byte frame (stack - 112) with the original's
// return addresses (CarInterior::Load always: its own calls need the guest).
uint32_t ScenePoolsSetUp(uint8_t* base, uint32_t pools, uint32_t pool, uint32_t index,
                         uint32_t record, uint32_t stack, uint32_t pcr,
                         const archive::LoadCall& load, const PoolSetUps& native,
                         const SetUpCall& call);

}  // namespace car_models
