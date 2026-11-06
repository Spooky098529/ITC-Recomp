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

// --- The second set's set-ups (ScenePools::SetUpB's cases with rows in
// scene-models.md): each copies its record into the second set's object,
// claims the first set's pool of the same number ([0x826C2E40]: ClaimA,
// GetA), and sets that up with ScenePools::SetUp's plain C++ (`setup`'s
// fields as its adapter passes them), from the original's frame (`stack`
// its r1). nullopt, with nothing done but the stores before the copy (which
// the original makes the same again), where the copy can't be native (the
// original runs). Each returns its r3 as the original leaves it. ---

// A guest call with r3 to r5 from frame r1 and return address lr; returns r3.
using GuestCall = std::function<uint32_t(uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5,
                                         uint32_t r1, uint32_t lr)>;

// What the set-ups pass ScenePools::SetUp.
struct SecondSetUps {
  uint32_t pcr;
  const archive::LoadCall& load;
  PoolSetUps native;
  const GuestCall& guest;
};

// The first set's pools.
constexpr uint32_t kFirstSet = 0x826C2E40;

// SceneCarModel::SetUp (sub_820DB818): +3728 = the scene ([0x826C2E44]),
// the car record copied to +32 (CarRecord::Copy), pool 0 claimed and set up
// with +32; +24 = the set-up's object. 112-byte frame.
std::optional<uint32_t> SceneCarModelSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                           uint32_t stack, const SecondSetUps& setups);
// SceneParts::SetUp (sub_820DF1D0): the 60-byte record copied to +108
// (memcpy), pool 1 claimed (ClaimA, GetA) into +104, its +24 = the car
// model's ([[+24] + 24]), set up with +108; +104 = the set-up's object.
// 128-byte frame.
std::optional<uint32_t> ScenePartsSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups);
// Pool 4's set-up (sub_820DCAA0): as SceneParts' with 52 bytes to +32,
// pool 4, the object at +28.
std::optional<uint32_t> ScenePool4SetUp(uint8_t* base, uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups);
// SceneInterior::SetUp (sub_820E37F0): +11668 = the scene, the 120-byte
// record copied to +40 (memcpy), pool 3 claimed into +36, its +3088 = 1 when
// +11656 isn't 0 (the cockpit), its +24 = the car model's, set up with +40
// (CarInterior::Load, through the guest as ScenePools::SetUp always calls
// it); +36 = the set-up's object. 128-byte frame.
std::optional<uint32_t> SceneInteriorSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                           uint32_t stack, const SecondSetUps& setups);
// SceneTraffic::SetUp (sub_820E5998): +9796 = the scene, the 3,696-byte
// record copied to +32 (memcpy), pool 6 claimed into +9760 and set up with
// `record` (the caller's); the set-up's object not stored. 128-byte frame.
std::optional<uint32_t> SceneTrafficSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                          uint32_t stack, const SecondSetUps& setups);
// The wheels' set-up (sub_820E70C0): the 156-byte record copied to +32
// (memcpy); +112 to +152 from the table at 0x820044B0 (44-byte entries) by
// the car's block ([[+24] + 24] + 32), or for traffic (+32 = 3, with +100
// 0) 45 + [[+16776]] (+120 only with +100 0; +132 and +136 through float
// loads and stores; +148 and +152 the same word); the 156 bytes copied
// back over `record` (memcpy); with both wheel numbers (+48, +64) -1, a car
// model and a block from 18, sub_820D39D8(block, +48, +64) (never seen in
// play: through `guest`); +148 (+152) = whether the front (rear) number,
// when not negative, isn't 190; pool 5 claimed into +28, its +24 = the car
// model's, or with none its +3264 = +16776; set up with +32. 160-byte frame.
std::optional<uint32_t> SceneWheelsSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                         uint32_t stack, const SecondSetUps& setups);

// --- ScenePools::SetUpB and the set-ups above the others (each calls it
// again for its parts: SceneCockpit::SetUp for pool 3, SceneModel::Build
// for pools 0 to 6) ---

// ScenePools::SetUpB (sub_820C5B28): the second set's (`scene`, [0x826C2E44])
// pool object `index` (List::At on +4944 + 12 pool); 0 unless it's in use
// (+16 = 3), else the object, after its set-up with `record` by the pool
// (pool 0's split-off case sub_820C5B98 merged): 0 SceneCarModel::SetUp, 1
// SceneParts::SetUp, 2 SceneCockpit::SetUp, 3 SceneInterior::SetUp, 4 pool
// 4's, 5 the wheels', 6 SceneTraffic::SetUp, 7 SceneModel::SetUp, above
// none; their plain C++ from its 112-byte frame below `stack`, or, where one
// declines, the function through `setups.guest` (the adapter, or the
// original for those without one) with the original's return address.
uint32_t ScenePoolsSetUpB(uint8_t* base, uint32_t scene, uint32_t pool, uint32_t index,
                          uint32_t record, uint32_t stack, const SecondSetUps& setups);
// SceneCockpit::SetUp (sub_820DBE80): +4316 = the scene; the 144-byte record
// copied to +36 (memcpy); pool 2 of the first set claimed (ClaimA, GetA)
// into +32, its +24 = the car model's ([[+24] + 24]), set up with +36 (its
// result not stored); pool 3 of the second set claimed (ClaimB, GetB) into
// +28, its +11656 = 1 and +11612 = 1 (sub_820E3658 on +160), linked to the
// car model (SceneInterior::Link), its +28 = [+32] + 208 and +32 = [+32] +
// 496; returns ScenePools::SetUpB(scene, 3, that index, +60). nullopt with
// only +4316 written where the copy can't be native. 128-byte frame.
std::optional<uint32_t> SceneCockpitSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                          uint32_t stack, const SecondSetUps& setups);
// SceneModel::SetUp (sub_820DB458): pool 7, `record` a 4,244-byte scene
// model record: +5024 = 1, the record copied to +32 (SceneModelRecord::Copy;
// its original through `setups.guest` where the plain C++ declines), +4068
// = its kind, block 5's +3824 = 1 and +3828 = 0, +4916 = 0x8200296C's float
// (the scene's +4 0) or 0x82001FBC's; with the scene's +4 1 and the game
// mode ([0x826032DC]) not 5, the two sound assets of the model's slot i
// (+20) asked for (the slot's globals at 0x826D8350, 0x826D8390, 0x826D83AC,
// 0x826C5C48, 0x826C5C64 set, its two buffers cleared, Archive::Load of the
// ids of 0x82616318's entry with the callback sub_82175EE0); then +5040 = 1
// with +5044 set (built later), else SceneModel::Build. Returns its r3
// (Build's, or the last word it loaded). nullopt, with nothing done, where
// the record overlaps the copy or the parts record's +44 would take the
// tail sub_820DB53C (3 or 4 for kinds 1 and 2 below block 18, never seen in
// play): the original runs. 144-byte frame.
std::optional<uint32_t> SceneModelSetUp(uint8_t* base, uint32_t object, uint32_t record,
                                        uint32_t stack, const SecondSetUps& setups);
// SceneModel::Build (sub_820D9870): the model's parts claimed in the second
// set (+5116) and set up (ClaimB, GetB, SetUpB), each result stored back:
// kind 3 (traffic, +32): +5048 = -1, +5080 = 1, pool 6 into +5092 with +48,
// pool 5 into +5100 (its +16776 = +48) with +4068; other kinds pool 0 into
// +5048 (+48), pool 1 into +5060 (linked, +37248 = +32; +3744), pool 3 into
// +5068 (linked; +3804), pool 4 into +5084 (+24 the car model; +4224), pool
// 5 into +5100 (the same; +4068); then for kind 2 with the scene's +4 1 the
// cockpit's record at +3924 from the interior's (+3764, + 5 from 6; +3768;
// +3772; +3784, + 7 from 4; +3776; +3728; its 120 bytes from +3804 to +3948
// by memcpy) and pool 2 into +5076 (its +24 the car model), else +5080 = 1.
// Returns the last SetUpB's answer (the original's r3). 128-byte frame.
uint32_t SceneModelBuild(uint8_t* base, uint32_t model, uint32_t stack,
                         const SecondSetUps& setups);

}  // namespace car_models

