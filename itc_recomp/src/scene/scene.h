// The car scene as plain C++ (B7 in docs/rexglue-removal.md; the functions
// are in docs/functions/scene.md): the 5,536-byte object at [0x826C2E44]
// that holds the cars' models wherever cars are shown, its shared assets
// (scene + 64, 4,880 bytes), its set-up record (scene + 4) and the first
// pool set's teardown helpers. Guest data shared with the recompiled code,
// so these work on guest memory with the originals' loads and stores in
// their order; the originals' saved registers (in their own frames) aren't
// written: nothing reads them after the return. The guest calls they can't
// make themselves (the pool objects' destructors through their vtables) are
// handed back to the adapters (src/hooks/scene.cpp).

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace scene {

constexpr uint32_t kScene = 0x826C2E44;     // [it] the scene
constexpr uint32_t kFirstSet = 0x826C2E40;  // [it] the first pool set
constexpr uint32_t kSceneBytes = 5536;
constexpr uint32_t kAssets = 64;  // the shared assets in the scene
constexpr uint32_t kAssetsBytes = 4880;

// --- The small pieces ---

// SceneSetUp::SceneSetUp (sub_820D0D50): +36 and +40 = 0.
void SetUpConstruct(uint8_t* base, uint32_t record);

// FreePagesIn (sub_820D09A0): VirtualFree([at], 0, MEM_RELEASE), a tail
// call; returns VirtualFree's 1. nullopt, with nothing done, where the word
// isn't pages the allocation layer gave (VirtualFree's adapter would go to
// the original; 0 too).
std::optional<uint32_t> FreePagesIn(uint8_t* base, uint32_t at);

// ScenePools::StartTeardownA (sub_820C3C80): the first set's (not checked)
// +4 = 4 and +0 = 2, the pointer read again for each.
void StartTeardownA(uint8_t* base);

// CarScene::StartTeardown (sub_820C4D88): when the scene exists, +0 = 4 and
// +48 = 2 (the pointer read again), StartTeardownA, then
// `release_all(the accessories' set)` (Accessories::ReleaseAll, the r3 it
// leaves). The r3 it leaves: release_all's, or nullopt (r3 as given) when
// the scene is gone.
std::optional<uint64_t> StartTeardown(uint8_t* base,
                                      const std::function<uint64_t(uint32_t set)>& release_all);

// CarScene::SetInteriorWord (sub_820C8950): for each scene model in pool 7
// (+5028, its count +5036; List::At) whose interior is ready (+5072), the
// interior's (+5068) +11616 = `word`. Returns the r3 it leaves: the last
// List::At's, or nullopt (r3 as given) for an empty count.
std::optional<uint32_t> SetInteriorWord(uint8_t* base, uint32_t scene, uint32_t word);

// ScenePools::DestroyA (sub_820C3CF8): for each of the first set's seven
// pools (+56 + 12 x pool), while List::At(list, 0) finds an object:
// `destroy(object, frame)` (its vtable slot 0 with r4 = 0, a guest call
// from the 128-byte frame below `sp`, whose back chain is stored), then
// VirtualFree(object, 0, MEM_RELEASE) (the allocation layer's pages). r3 is
// left 0 (the last List::At's). Only where DestroyAFreeable (the adapter's
// check).
void DestroyA(uint8_t* base, uint32_t set, uint32_t sp,
              const std::function<void(uint32_t object, uint32_t frame)>& destroy);
// Whether DestroyA frees natively: every object in the seven lists (linked
// by +8) the allocation layer's pages, and each list's walk ends (at most
// 4,096 nodes).
bool DestroyAFreeable(uint8_t* base, uint32_t set);

}  // namespace scene
