// The car scene natively: adapters in front of src/scene/
// (docs/functions/scene.md), with the register steps the harness shares
// (hooks/scene.h).

#include "hooks/scene.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "car_models/vinyl.h"
#include "hooks/accessories.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "scene/scene.h"

ITC_FLAG_DECLARE(bool, native_accessories);
ITC_FLAG_DECLARE(bool, native_car_models);

ITC_FLAG_BOOL(native_scene, true, "ITC",
              "Run the car scene (docs/functions/scene.md: SceneSetUp::SceneSetUp sub_820D0D50, "
              "with --native_heap FreePagesIn sub_820D09A0, ScenePools::StartTeardownA "
              "sub_820C3C80, CarScene::StartTeardown sub_820C4D88 (Accessories::ReleaseAll's "
              "plain C++ where its adapter would run it), CarScene::SetInteriorWord sub_820C8950, "
              "with --native_heap ScenePools::DestroyA sub_820C3CF8 (the pool objects' "
              "destructors guest calls through their vtables) and UpdateA sub_820C3DA0 "
              "(Texture::Free's plain C++ where its adapter would run it), with --native_heap "
              "and the models' lifetime SceneAssets::SceneAssets sub_820C5008, Reset "
              "sub_820C4A70 and ~SceneAssets sub_820C5168, CarScene::CarScene sub_820C89A8 and "
              "~CarScene sub_820C9110, List::DeleteAll sub_82593FF8 and ~List sub_82594188 (the "
              "nodes' destructors guest calls)) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=scene)");

namespace hooks {

namespace {

// A guest function through the function table (its override when it has
// one), from `r1` with `lr`, r3 and r4 set; r1 and lr put back.
void GuestCall(PPCContext& ctx, uint8_t* base, uint32_t function, uint32_t r1, uint32_t lr,
               uint64_t r3, uint64_t r4) {
  const uint64_t stack = ctx.r1.u64, link = ctx.lr;
  ctx.r1.u64 = r1;
  ctx.r3.u64 = r3;
  ctx.r4.u64 = r4;
  ctx.lr = lr;
  ctx.ctr.u64 = function;
  REX_CALL_INDIRECT_FUNC(function);
  ctx.r1.u64 = stack;
  ctx.lr = link;
}

// The pool objects' deleting destructors: slot 0 of the object's vtable,
// r4 = the flags, from `frame` with `lr`.
scene::DestroyCall Destroyer(PPCContext& ctx, uint8_t* base, uint32_t lr) {
  return [&ctx, base, lr](uint32_t object, uint32_t flags, uint32_t frame) {
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object));
    GuestCall(ctx, base, function, frame, lr, object, flags);
  };
}

}  // namespace

void SceneSetUpConstruct(PPCContext& ctx, uint8_t* base) {
  scene::SetUpConstruct(base, ctx.r3.u32);
  ctx.r11.u64 = 0;
}

void SceneFreePagesIn(PPCContext& ctx, uint8_t* base) {
  if (hooks::NativeHeapActive()) {
    if (const auto r3 = scene::FreePagesIn(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820D09A0(ctx, base);
}

void SceneStartTeardownA(PPCContext& ctx, uint8_t* base) {
  scene::StartTeardownA(base);
}

void SceneStartTeardown(PPCContext& ctx, uint8_t* base) {
  // ReleaseAll runs only when the scene exists.
  const uint32_t set = guest::LoadU32(base, accessories::kSet);
  if (guest::LoadU32(base, scene::kScene) != 0 &&
      (set == 0 || !ITC_FLAG(native_accessories) || !AccessoriesNativeLoads() ||
       !ModelsNativeLifetime() || !accessories::SetReleasable(base, set))) {
    __imp__sub_820C4D88(ctx, base);
    return;
  }
  const auto r3 = scene::StartTeardown(base, [&](uint32_t set) -> uint64_t {
    return accessories::SetReleaseAll(base, set).value_or(set);
  });
  if (r3) {
    ctx.fpscr.disableFlushMode();
    ctx.r3.u64 = *r3;
  }
}

void SceneSetInteriorWord(PPCContext& ctx, uint8_t* base) {
  if (const auto r3 = scene::SetInteriorWord(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *r3;
  }
}

void SceneDestroyA(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !scene::DestroyAFreeable(base, ctx.r3.u32)) {
    __imp__sub_820C3CF8(ctx, base);
    return;
  }
  scene::DestroyA(base, ctx.r3.u32, ctx.r1.u32, [&](uint32_t object, uint32_t frame) {
    const uint32_t function = guest::LoadU32(base, guest::LoadU32(base, object));
    GuestCall(ctx, base, function, frame, 0x820C3D60, object, 0);
  });
  ctx.r3.u64 = 0;
}

void SceneAssetsConstruct(PPCContext& ctx, uint8_t* base) {
  if (!ModelsNativeLifetime() || !scene::AssetsConstruct(base, ctx.r3.u32)) {
    __imp__sub_820C5008(ctx, base);
  }
}

void SceneCarSceneConstruct(PPCContext& ctx, uint8_t* base) {
  if (!ModelsNativeLifetime() || !scene::CarSceneConstruct(base, ctx.r3.u32)) {
    __imp__sub_820C89A8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
}

void SceneAssetsReset(PPCContext& ctx, uint8_t* base) {
  if (ModelsNativeLifetime()) {
    if (const auto r3 = scene::AssetsReset(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820C4A70(ctx, base);
}

void SceneAssetsDestruct(PPCContext& ctx, uint8_t* base) {
  if (ModelsNativeLifetime()) {
    if (const auto r3 = scene::AssetsDestruct(base, ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820C5168(ctx, base);
}

void SceneListDeleteAll(PPCContext& ctx, uint8_t* base) {
  scene::ListDeleteAll(base, ctx.r3.u32, ctx.r1.u32, Destroyer(ctx, base, 0x8259402C));
  ctx.r3.u64 = 0;
}

void SceneListDestruct(PPCContext& ctx, uint8_t* base) {
  scene::ListDestruct(base, ctx.r3.u32, ctx.r1.u32, Destroyer(ctx, base, 0x8259402C));
  ctx.r3.u64 = 0;
}

void SceneCarSceneDestruct(PPCContext& ctx, uint8_t* base) {
  if (!ModelsNativeLifetime() ||
      !scene::CarSceneDestructible(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820C9110(ctx, base);
    return;
  }
  const auto r3 = scene::CarSceneDestruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32,
                                          Destroyer(ctx, base, 0x8259402C));
  ctx.r3.u64 = *r3;
}

void SceneUpdateA(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !scene::UpdateANative(base)) {
    __imp__sub_820C3DA0(ctx, base);
    return;
  }
  const auto texture_free = [&](uint32_t header_in, uint32_t data_in,
                                uint32_t frame) -> uint32_t {
    if (ITC_FLAG(native_car_models) && car_models::TextureFreeable(base, header_in)) {
      return car_models::TextureFree(base, header_in, data_in);
    }
    const uint64_t stack = ctx.r1.u64, link = ctx.lr;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = header_in;
    ctx.r4.u64 = data_in;
    ctx.lr = 0x820C3E1C;
    sub_820ECFC8(ctx, base);
    ctx.r1.u64 = stack;
    ctx.lr = link;
    return ctx.r3.u32;
  };
  const uint64_t r3_in = ctx.r3.u64;
  const auto r3 = scene::UpdateA(base, ctx.r1.u32, Destroyer(ctx, base, 0x820C3D60), texture_free);
  ctx.r3.u64 = r3 ? *r3 : r3_in;
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off.

#define ITC_SCENE(address, step)       \
  extern "C" REX_FUNC(sub_##address) { \
    REX_FUNC_PROLOGUE();               \
    if (!ITC_FLAG(native_scene)) {     \
      __imp__sub_##address(ctx, base); \
      return;                          \
    }                                  \
    hooks::step(ctx, base);            \
  }

ITC_SCENE(820D0D50, SceneSetUpConstruct)
ITC_SCENE(820D09A0, SceneFreePagesIn)
ITC_SCENE(820C4D88, SceneStartTeardown)
ITC_SCENE(820C8950, SceneSetInteriorWord)
ITC_SCENE(820C3DA0, SceneUpdateA)
ITC_SCENE(820C89A8, SceneCarSceneConstruct)
ITC_SCENE(820C4A70, SceneAssetsReset)
ITC_SCENE(820C9110, SceneCarSceneDestruct)
// No adapters (their callers in play are native; the register steps above
// are the harness's): ScenePools::StartTeardownA sub_820C3C80, DestroyA
// sub_820C3CF8, SceneAssets::SceneAssets sub_820C5008 and ~SceneAssets
// sub_820C5168, List::DeleteAll sub_82593FF8 and ~List sub_82594188.
