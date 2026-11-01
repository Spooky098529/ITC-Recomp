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

// --- The shared assets (scene + 64) ---

// The assets' members, at their offsets in the assets: the XPR objects, the
// texture providers and the part models (the layout in scene.md's intro).
constexpr uint32_t kAssetsXprs[13] = {24, 96, 120, 208, 276, 344, 976, 1044, 4584,
                                      4704, 4728, 4752, 4776};
constexpr uint32_t kAssetsProviders[11] = {144, 164, 232, 300, 368, 1000, 1068,
                                           4800, 4820, 4840, 4860};
constexpr uint32_t kAssetsModels[6] = {416, 1184, 1648, 2112, 2576, 4096};

// SceneAssets::SceneAssets (sub_820C5008): XPR::XPR, TextureProvider's
// constructor (64 slots), PartModel::PartModel and TgaHeader::Clear on the
// members, in the original's order (the load slots untouched). Returns the
// assets. False, with nothing done, where a member's plain C++ would refuse
// (a provider's storage operator new can't give natively); only where the
// models' lifetime is native (the adapter's check).
bool AssetsConstruct(uint8_t* base, uint32_t assets);

// SceneAssets::Reset (sub_820C4A70): LoadSlot::Reset on every slot,
// XPR::Release on every XPR, the providers' slot 2 (TextureProvider::ClearAll),
// Model::Release on the part models, the TGA pixels' words (+948, +3592 +
// 4i) = 0, in the layout's order. Returns the r3 it leaves (the last
// provider, +4860); nullopt, with nothing done, where AssetsResettable says
// no. Only where the models' lifetime is native.
std::optional<uint32_t> AssetsReset(uint8_t* base, uint32_t assets);
// Whether AssetsReset runs natively (nothing changed): each provider's slot
// 2 TextureProvider::ClearAll, room in the pending frees for every block it
// may put off.
bool AssetsResettable(uint8_t* base, uint32_t assets);

// SceneAssets::~SceneAssets (sub_820C5168): what the constructor made, last
// first (the TGA headers' empty destructor left out). Returns the r3 it
// leaves (XPR::~XPR's on +24); nullopt, with nothing done, where a member's
// plain C++ would refuse. Only where the models' lifetime is native.
std::optional<uint32_t> AssetsDestruct(uint8_t* base, uint32_t assets);
bool AssetsDestructible(uint8_t* base, uint32_t assets);

// --- The scene ---

// CarScene::InitLighting (sub_820C8528): r3 = the scene + 5248. Its float4s
// (the image's constants and 12 cos calls on the doubles at 0x82002FE0,
// 0x82002FD8, 0x82002FC8 and 0x82002FB8) built in its 288-byte frame below
// `sp` (left as the original leaves it) and copied into +0 to +271 as
// doublewords, then the float at 0x8200256C into +272. Returns the r3 it
// leaves (the doubleword at the frame's +96).
uint64_t InitLighting(uint8_t* base, uint32_t block, uint32_t sp);

// SceneAssets::Release (sub_820C4C80): LoadSlot::ReleaseBuffer on the
// slots and FreePagesIn on the TGA pixels' words (scene.md's row), in its
// order, Model::Release(+4096), then SceneAssets::Reset. `free_pages_in(at)`
// for a pixels' word that isn't the layer's pages (0 too: the original's
// VirtualFree, a guest call). Returns the r3 it leaves (Reset's); nullopt,
// with nothing done, where AssetsReleasable says no.
std::optional<uint32_t> AssetsRelease(uint8_t* base, uint32_t assets,
                                      const std::function<void(uint32_t at)>& free_pages_in);
bool AssetsReleasable(uint8_t* base, uint32_t assets);

// CarScene::Create (sub_820C8A50): the scene's pages (VirtualAlloc(5536),
// the layer's) into [0x826C2E44], CarScene::CarScene on them when given;
// the scene's +0 = 0, [0x82601854] = 167, [0x82601858] = 270,
// ScenePools::MakeA, +5040 = [0x826C2E40], InitLighting(+5248),
// SceneAssets::Reset(+64), +5044, +5048, +5052 = 0, PaintPalette::Init,
// Accessories::Create, then Accessories::SetScale(the set, {x, x, x, 1.0})
// with x the float at 0x82002FE8 built at its 128-byte frame's +80 below
// `sp`. Returns the r3 it leaves (the accessories' set). Only where
// CreateNative (the adapter's check, with the layer and the models'
// lifetime).
uint64_t Create(uint8_t* base, uint32_t sp);
bool CreateNative(uint8_t* base);

// CarScene::CarScene (sub_820C89A8): SceneSetUp::SceneSetUp (+4),
// SceneAssets::SceneAssets (+64), List::Init on the eight lists at +4944 +
// 12i, then the float at 0x82000A30 (1.0) into +5180 + 16i (five), +5260 +
// 16i (eight) and +5388 + 16i (nine). Returns the scene; false where
// AssetsConstruct refuses (nothing done).
bool CarSceneConstruct(uint8_t* base, uint32_t scene);

// The pool objects' deleting destructors (vtable slot 0 with r4 = `flags`),
// a guest call from `frame` (its back chain stored), from the return address
// the caller gives.
using DestroyCall = std::function<void(uint32_t object, uint32_t flags, uint32_t frame)>;

// List::DeleteAll (sub_82593FF8), through List::~List (sub_82594188, a
// jump): while the list's first node (+0) isn't 0, its slot 0 with r4 = 1,
// from the 96-byte frame below `sp`. r3 is left 0.
void ListDeleteAll(uint8_t* base, uint32_t list, uint32_t sp, const DestroyCall& destroy);
// List::~List (sub_82594188): a jump to DeleteAll (the same frame).
void ListDestruct(uint8_t* base, uint32_t list, uint32_t sp, const DestroyCall& destroy);

// CarScene::~CarScene (sub_820C9110): List::~List on the eight lists, last
// first (from the 128-byte frame below `sp`), SceneAssets::~SceneAssets
// (+64), the set-up record's empty destructor left out, then operator
// delete when `flags`' bit 0 is set. Returns the scene; nullopt, with
// nothing done, where CarSceneDestructible says no.
std::optional<uint32_t> CarSceneDestruct(uint8_t* base, uint32_t scene, uint32_t flags,
                                         uint32_t sp, const DestroyCall& destroy);
bool CarSceneDestructible(uint8_t* base, uint32_t scene, uint32_t flags);

// ScenePools::UpdateA (sub_820C3DA0): the first set's update (scene.md's
// row): the deferred texture releases counted down and freed
// (Texture::Free), or in a teardown (+4 = 4) the count (+0) down and at 0
// DestroyA, every pending release freed, FreePagesIn(+52), the set's pages
// freed and [0x826C2E40] = 0. `sp` the guest r1 at entry (DestroyA's frame
// below its 128-byte one). Returns the r3 it leaves, or nullopt (r3 as
// given). Only where UpdateANative (the adapter's check).
//
// Texture::Free's calls go through `texture_free(header_in, data_in,
// frame)` (its plain C++ where its adapter would run it, else the original
// from UpdateA's frame), answering the r3 it leaves: the pool objects'
// destructors may put off releases of their own before the teardown frees
// them.
using TextureFreeCall =
    std::function<uint32_t(uint32_t header_in, uint32_t data_in, uint32_t frame)>;
std::optional<uint32_t> UpdateA(uint8_t* base, uint32_t sp, const DestroyCall& destroy,
                                const TextureFreeCall& texture_free);
// Whether UpdateA runs natively (nothing changed): no first set, no
// teardown due, or DestroyAFreeable with the records' block (+52) and the
// set the allocation layer's pages.
bool UpdateANative(uint8_t* base);

}  // namespace scene
