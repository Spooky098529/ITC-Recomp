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
