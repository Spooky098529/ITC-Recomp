// The car scene natively: adapters in front of src/scene/
// (docs/functions/scene.md), with the register steps the harness shares
// (hooks/scene.h).

#include "hooks/scene.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "hooks/accessories.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "scene/scene.h"

ITC_FLAG_DECLARE(bool, native_accessories);

ITC_FLAG_BOOL(native_scene, true, "ITC",
              "Run the car scene (docs/functions/scene.md: SceneSetUp::SceneSetUp sub_820D0D50, "
              "with --native_heap FreePagesIn sub_820D09A0, ScenePools::StartTeardownA "
              "sub_820C3C80, CarScene::StartTeardown sub_820C4D88 (Accessories::ReleaseAll's "
              "plain C++ where its adapter would run it), CarScene::SetInteriorWord sub_820C8950, "
              "with --native_heap ScenePools::DestroyA sub_820C3CF8 (the pool objects' "
              "destructors guest calls through their vtables)) natively instead of through the "
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
ITC_SCENE(820C3C80, SceneStartTeardownA)
ITC_SCENE(820C4D88, SceneStartTeardown)
ITC_SCENE(820C8950, SceneSetInteriorWord)
ITC_SCENE(820C3CF8, SceneDestroyA)
