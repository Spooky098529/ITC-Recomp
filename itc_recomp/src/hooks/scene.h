// The car scene's register steps (src/scene/, docs/functions/scene.md),
// shared by the adapters (src/hooks/scene.cpp, behind --native_scene) and
// the harness (without the flag). Each leaves r3 as the original does and
// runs the original where the plain C++ refuses.

#pragma once

#include "guest.h"

namespace hooks {

// SceneSetUp::SceneSetUp sub_820D0D50: r3 = the record (left).
void SceneSetUpConstruct(PPCContext& ctx, uint8_t* base);
// FreePagesIn sub_820D09A0: r3 = where the pages' address is; VirtualFree's
// r3. Natively only while --native_heap routes VirtualFree to the layer.
void SceneFreePagesIn(PPCContext& ctx, uint8_t* base);
// ScenePools::StartTeardownA sub_820C3C80 (r3 left).
void SceneStartTeardownA(PPCContext& ctx, uint8_t* base);
// CarScene::StartTeardown sub_820C4D88: Accessories::ReleaseAll's plain C++
// where its adapter would run it (--native_accessories, the layer, the
// models' lifetime, SetReleasable), else the original.
void SceneStartTeardown(PPCContext& ctx, uint8_t* base);
// CarScene::SetInteriorWord sub_820C8950: r3 = the scene, r4 = the word.
void SceneSetInteriorWord(PPCContext& ctx, uint8_t* base);
// ScenePools::DestroyA sub_820C3CF8: r3 = the first set; the destructors
// guest calls. Natively only over the layer where DestroyAFreeable.
void SceneDestroyA(PPCContext& ctx, uint8_t* base);
// CarScene::LoadsDone sub_820C5798: r3 = the scene; r3 = 0 or 1.
void SceneLoadsDone(PPCContext& ctx, uint8_t* base);

// SceneAssets::SceneAssets sub_820C5008 (r3 the assets, left) and
// CarScene::CarScene sub_820C89A8 (r3 the scene, left): natively where the
// models' lifetime is native.
void SceneAssetsConstruct(PPCContext& ctx, uint8_t* base);
void SceneCarSceneConstruct(PPCContext& ctx, uint8_t* base);
// SceneAssets::Reset sub_820C4A70 and ~SceneAssets sub_820C5168: r3 the
// assets; the r3 the original leaves.
void SceneAssetsReset(PPCContext& ctx, uint8_t* base);
void SceneAssetsDestruct(PPCContext& ctx, uint8_t* base);
// List::DeleteAll sub_82593FF8 and List::~List sub_82594188: r3 the list;
// the nodes' deleting destructors guest calls.
void SceneListDeleteAll(PPCContext& ctx, uint8_t* base);
void SceneListDestruct(PPCContext& ctx, uint8_t* base);
// CarScene::~CarScene sub_820C9110: r3 the scene, r4 the flags.
void SceneCarSceneDestruct(PPCContext& ctx, uint8_t* base);
// ScenePools::UpdateA sub_820C3DA0: over the layer where UpdateANative;
// Texture::Free's plain C++ where its adapter would run it, else the
// original from UpdateA's frame.
void SceneUpdateA(PPCContext& ctx, uint8_t* base);
// CarScene::InitLighting sub_820C8528: r3 = the scene + 5248.
void SceneInitLighting(PPCContext& ctx, uint8_t* base);
// SceneAssets::Release sub_820C4C80: r3 = the assets; FreePagesIn's
// original for a pixels' word that isn't the layer's pages.
void SceneAssetsRelease(PPCContext& ctx, uint8_t* base);
// CarScene::Create sub_820C8A50: over the layer with the models' lifetime.
void SceneCreate(PPCContext& ctx, uint8_t* base);
// CarScene::LoadAssets sub_820C8B40: r3 = the scene; over the layer with
// the archive's queue native.
void SceneLoadAssets(PPCContext& ctx, uint8_t* base);
// CarScene::OnFileLoaded sub_820C52A0: r3 = the request, r4 = the data, r6
// = the scene; its guest calls from the original's 128-byte frame.
void SceneOnFileLoaded(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
