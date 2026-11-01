// The second set's objects' lifetimes as plain C++
// (docs/functions/second-set.md): each object's reset (its record's
// defaults, its point block's reset, its words cleared), the releases that
// hand its first-set object back (ScenePools::ReleaseA), and the scene
// model's block. Guest data shared with the recompiled code, so these work on
// guest memory with the originals' stores in their order (the saved
// registers' slots in their frames aren't written). `stack` is the guest r1
// at entry; each returns r3 as the original leaves it. Only where memcpy is
// native, with --native_material_passes for the parts' (PartsPoints::Reset),
// and where ReleaseA's plain C++ runs for the releases (the adapters'
// checks). Adapters: src/hooks/second_set.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace second_set {

constexpr uint32_t kSceneModelBlockResetFrameBytes = 352;

// SceneCarModel::Release (sub_820DB6C0): when +24 (the first set's car
// model) is set, ScenePools::ReleaseA([0x826C2E40], 0, its +20);
// CarRecord::Reset(+32); +24 and +3728 = 0. Returns CarRecord::Reset's r3;
// nullopt, with nothing done, where ReleaseA's plain C++ declines.
std::optional<uint32_t> SceneCarModelRelease(uint8_t* base, uint32_t object, uint32_t stack);
// SceneParts::Reset (sub_820E0510): the parts record's defaults (+108),
// PartsPoints::Reset(+176), +24, +28, +100, +104, +37280 = 0, +37284 = 0
// then the scene, memset(+32, 0, 68). Returns memset's r3 (+32).
uint32_t ScenePartsReset(uint8_t* base, uint32_t object, uint32_t stack);
// SceneCockpit::Reset (sub_820DBE20): +24, +28, +32, +4312 = 0, +4316 =
// the scene; the cockpit record's defaults (+36); CockpitPoints::Reset(+192);
// +4272 = +36, +4288 to +4311 = 0. Returns CockpitPoints::Reset's r3.
uint32_t SceneCockpitReset(uint8_t* base, uint32_t object, uint32_t stack);
// SceneInterior::Reset (sub_820E3748): +24 to +36 and +11656 to +11664 =
// 0; the interior record's defaults (+40); InteriorPoints::Reset(+160);
// +11668 = the scene; the six words at +11632 = 0. Returns
// InteriorPoints::Reset's r3.
uint32_t SceneInteriorReset(uint8_t* base, uint32_t object, uint32_t stack);
// SceneLights::Reset (sub_820DCA40): +24, +28, +13152, +13156 = 0; the pool
// 4 record's defaults (+32); LightPoints::Reset(+96); +13160 = the scene;
// the block's +0, +48, +4384, +8720 = the record (+32). Returns
// LightPoints::Reset's r3.
uint32_t SceneLightsReset(uint8_t* base, uint32_t object, uint32_t stack);
// SceneTraffic::Reset (sub_820E5930): CarRecord::Reset(+32);
// TrafficPoints::Reset(+3728); +9764, +9760, +9792 = 0 and the six words at
// +9768 = 0. Returns TrafficPoints::Reset's r3.
uint32_t SceneTrafficReset(uint8_t* base, uint32_t object, uint32_t stack);

// SceneWheels::Reset (sub_820E88A8): +24, +28 and +16768 to +16780 = 0,
// +16784 = the scene; the wheels record's defaults (+32);
// WheelPoints::Reset(+192). Returns WheelPoints::Reset's r3.
uint32_t SceneWheelsReset(uint8_t* base, uint32_t object, uint32_t stack);

// The releases: when the word at `first` (the first set's object) is set,
// ScenePools::ReleaseA([0x826C2E40], the pool, its +20); then the reset.
// Return the reset's r3; nullopt, with nothing done, where ReleaseA's plain
// C++ declines. SceneParts::Release (sub_820E0590, +104, pool 1),
// SceneInterior::Release (sub_820E3890, +36, pool 3), SceneLights::Release
// (sub_820DCB20, +28, pool 4), SceneWheels::Release (sub_820E89B0, +28, pool
// 5) and SceneTraffic::Release (sub_820E5A18, +9760, pool 6).
std::optional<uint32_t> ScenePartsRelease(uint8_t* base, uint32_t object, uint32_t stack);
std::optional<uint32_t> SceneInteriorRelease(uint8_t* base, uint32_t object, uint32_t stack);
std::optional<uint32_t> SceneLightsRelease(uint8_t* base, uint32_t object, uint32_t stack);
std::optional<uint32_t> SceneWheelsRelease(uint8_t* base, uint32_t object, uint32_t stack);
std::optional<uint32_t> SceneTrafficRelease(uint8_t* base, uint32_t object, uint32_t stack);

// What ScenePools::ReleaseB's plain C++ hands back to the adapters.
struct ReleaseCalls {
  // Whether the release of `pool` runs natively (the adapters' checks:
  // native memcpy, ReleaseA's plain C++ for the pool, the parts' with
  // --native_material_passes).
  std::function<bool(uint32_t pool)> native;
  // ScenePools::ReleaseB's original (r3 the set, r4 the pool, r5 the index)
  // from the frame at `sp` with `lr`, where the plain C++ declines (nothing
  // done); its r3.
  std::function<uint64_t(uint32_t set, uint32_t pool, uint32_t index, uint32_t sp, uint32_t lr)>
      release_b;
};

// ScenePools::ReleaseB (sub_820C5C88): r3 = the second set, r4 = a pool, r5
// = an index: the object (List::At on +4944 + 12 x pool); when its +16 is 3,
// its pool's release (0 SceneCarModel, 1 SceneParts, 2 SceneCockpit, 3
// SceneInterior, 4 SceneLights, 5 SceneWheels, 6 SceneTraffic, 7
// SceneModel; nothing past 7), then +16 = 0. Returns the release's r3 (the
// object otherwise). The original (calls.release_b, from `stack` with `lr`)
// where the pool's release isn't native or declines with nothing done.
uint64_t ReleaseB(uint8_t* base, uint32_t set, uint32_t pool, uint32_t index, uint32_t stack,
                  uint32_t lr, const ReleaseCalls& calls);
// SceneCockpit::Release (sub_820DBF88): when +32 (the first set's cockpit
// body) is set, ScenePools::ReleaseA([0x826C2E40], 2, its +20); when +28
// (its interior) is set, ReleaseB(+4316, 3, its +20); then
// SceneCockpit::Reset. Returns the reset's r3; nullopt, with nothing done,
// where ReleaseA's plain C++ declines.
std::optional<uint32_t> SceneCockpitRelease(uint8_t* base, uint32_t object, uint32_t stack,
                                            const ReleaseCalls& calls);
// SceneModel::Release (sub_820DA5D0): when +5044 is 0, ReleaseB(+5116, the
// pool, the part's +20) on the car model (+5048, pool 0; not for kind 3 at
// +32), the parts (+5060, 1), the interior (+5068, 3), pool 4 (+5084), the
// wheels (+5100, 5), the cockpit (+5076, 2) and the traffic body (+5092, 6)
// that are set; then SceneModelRecord::Reset(+32), SceneModelBlock::Reset
// (+4288), +5024 to +5112 = 0 but +5044 = 3, +5116 = the scene. Returns
// SceneModelBlock::Reset's r3.
uint32_t SceneModelRelease(uint8_t* base, uint32_t object, uint32_t stack,
                           const ReleaseCalls& calls);

// The constructors: PoolNode::PoolNode(the object, `list`), the vtable, +16
// = 0 and +20 = -1 (in the original's order), the records' and point
// blocks' constructors, then the reset. Return the object.
// SceneCarModel::SceneCarModel (sub_820DB770, vtable 0x82005130; the car
// record's w's and defaults at +32, +24 and +3728 = 0, no reset),
// SceneParts::SceneParts (sub_820E2F18, 0x82006024), SceneCockpit::SceneCockpit
// (sub_820DC3E8, 0x8200527C), SceneInterior::SceneInterior (sub_820E5340,
// 0x820062E4), SceneLights::SceneLights (sub_820DE030, 0x820056E0),
// SceneWheels::SceneWheels (sub_820ECE40, 0x82006638; the records at +76 and
// +88 reset first), SceneTraffic::SceneTraffic (sub_820E6E70, 0x82006424; the
// car record's w's at +32) and SceneModel::SceneModel (sub_820DB330, the base
// vtable 0x82004F90; the record and block constructed and reset, +5024 to
// +5112 = 0, +5116 = the scene).
uint32_t SceneCarModelConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t ScenePartsConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t SceneCockpitConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t SceneInteriorConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t SceneLightsConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t SceneWheelsConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t SceneTrafficConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);
uint32_t SceneModelConstruct(uint8_t* base, uint32_t object, uint32_t list, uint32_t stack);

// The destructors: the class's vtable, its release, its point block's
// destructor, the pool objects' base vtable (0x82004F90),
// PoolNode::~PoolNode. False where the release declines (only the vtable
// word written, which the original writes again the same). The object is the
// r3 they leave. ~SceneCarModel (sub_820DB720, no point block), ~SceneParts
// (sub_820E2E10), ~SceneInterior (sub_820E5178), ~SceneLights (sub_820DDE00)
// and ~SceneWheels (sub_820ECDD0); ~SceneTraffic (sub_820E69F0) doesn't
// release, as the original (its +9760 object isn't handed back).
bool SceneCarModelDestruct(uint8_t* base, uint32_t object, uint32_t stack);
bool ScenePartsDestruct(uint8_t* base, uint32_t object, uint32_t stack);
bool SceneInteriorDestruct(uint8_t* base, uint32_t object, uint32_t stack);
bool SceneLightsDestruct(uint8_t* base, uint32_t object, uint32_t stack);
bool SceneWheelsDestruct(uint8_t* base, uint32_t object, uint32_t stack);
void SceneTrafficDestruct(uint8_t* base, uint32_t object);
// The deleting destructors (vtable slot 0, which ScenePools::DestroyB calls
// with r4 = 0): the destructor; operator delete when bit 0 of `flags` is
// set, which these leave to the original (false, nothing done: never in
// play). r3 = the object. SceneCarModel's (sub_820DB7C8), SceneParts'
// (sub_820E2F78), SceneInterior's (sub_820E53A0), SceneLights' (sub_820DE090),
// SceneWheels' (sub_820ECEB8), SceneTraffic's (sub_820E6ED8) and the pool
// objects' base class's (sub_820DB400, the scene model's: the base vtable and
// PoolNode::~PoolNode only, nothing of its own released).
bool SceneCarModelDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);
bool ScenePartsDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);
bool SceneInteriorDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);
bool SceneLightsDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);
bool SceneWheelsDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);
bool SceneTrafficDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);
bool PoolObjectDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack);

// SceneCockpit::~SceneCockpit (sub_820DC2F8): the vtable (0x8200527C),
// SceneCockpit::Release, CockpitPoints::~CockpitPoints(+192), the base
// vtable, PoolNode::~PoolNode; false where the release declines (only the
// vtable word written). Its deleting destructor (sub_820DC448, vtable slot
// 0): operator delete when bit 0 of `flags` is set, left to the original
// (false, nothing done). The object is the r3 they leave.
bool SceneCockpitDestruct(uint8_t* base, uint32_t object, uint32_t stack,
                          const ReleaseCalls& calls);
bool SceneCockpitDeletingDestruct(uint8_t* base, uint32_t object, uint32_t flags, uint32_t stack,
                                  const ReleaseCalls& calls);

// SceneModelBlock::SceneModelBlock (sub_820DA108): 1.0 into the w's of the
// matrix at +16, the four at +80 and the four at +336, and +668, +684. Leaf;
// r3 left.
void SceneModelBlockConstruct(uint8_t* base, uint32_t block);
// SceneModelBlock::Reset (sub_820D9F50): +8, +12, +0 (8 bytes) = 0; the
// identity (built at the frame's +80) into +16; +592 = 0; for each of the
// four matrices at +80 + 64k: the identity into +336 + 64k (from the
// frame's +80) and +80 + 64k (from the frame's +144), +612 + 4k = 0.0,
// +596 + 4k = 2; +640 = 0.0; +628, +632, +636 = [0x8200296C], [0x82002A10],
// [0x82002208]; +656 to +684 and +692 to +700 = 0.0; +688, +724, +728 = 0;
// +732 = 1. Returns the last memcpy's r3 (+272).
uint32_t SceneModelBlockReset(uint8_t* base, uint32_t block, uint32_t stack);

}  // namespace second_set
