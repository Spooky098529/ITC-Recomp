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

}  // namespace hooks
