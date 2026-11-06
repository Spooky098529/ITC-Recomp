// The car scene natively: adapters in front of src/scene/
// (docs/functions/scene.md), with the register steps the harness shares
// (hooks/scene.h).

#include "hooks/scene.h"

#include "accessories/accessories.h"
#include "base/flags.h"
#include "car_models/vinyl.h"
#include "hooks/accessories.h"
#include "hooks/archive.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "scene/scene.h"

#include <optional>

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
              "nodes' destructors guest calls), CarScene::InitLighting sub_820C8528, with --native_heap "
              "and the models' lifetime SceneAssets::Release sub_820C4C80 (the original "
              "VirtualFree for a TGA pixels' word that isn't the layer's) and CarScene::Create "
              "sub_820C8A50 (over ScenePools::MakeA's, PaintPalette::Init's and the accessories' "
              "plain C++), with --native_heap and the archive's queue CarScene::LoadAssets sub_820C8B40, with the models' lifetime CarScene::OnFileLoaded "
              "sub_820C52A0 (Model::Load's, XPR::Create's and TGA::CreateImage's originals "
              "where their plain C++ declines, the providers' slot +16 through their vtables "
              "unless it's TextureProvider::Set), CarScene::LoadsDone sub_820C5798 (the second set's "
              "finishers through their overrides)) natively instead of through the "
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

void SceneLoadsDone(PPCContext& ctx, uint8_t* base) {
  const uint64_t lr = ctx.lr;
  ctx.r3.u64 = scene::LoadsDone(
      base, ctx.r3.u32, ctx.r1.u32,
      [&](uint32_t function, uint32_t frame, uint32_t return_address, uint32_t r3, uint32_t r4) {
        GuestCall(ctx, base, function, frame, return_address, r3, r4);
        return ctx.r3.u64;
      });
  ctx.lr = lr;
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

void SceneInitLighting(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = scene::InitLighting(base, ctx.r3.u32, ctx.r1.u32);
}

void SceneAssetsRelease(PPCContext& ctx, uint8_t* base) {
  if (ModelsNativeLifetime()) {
    const auto r3 = scene::AssetsRelease(base, ctx.r3.u32, [&](uint32_t at) {
      // FreePagesIn's original, from Release's 128-byte frame.
      const uint64_t stack = ctx.r1.u64, link = ctx.lr;
      ctx.r1.u64 = ctx.r1.u32 - 128;
      guest::StoreU32(base, ctx.r1.u32, uint32_t(stack));
      ctx.r3.u64 = at;
      ctx.lr = 0x820C4CDC;
      __imp__sub_820D09A0(ctx, base);
      ctx.r1.u64 = stack;
      ctx.lr = link;
    });
    if (r3) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  __imp__sub_820C4C80(ctx, base);
}

void SceneCreate(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !ModelsNativeLifetime() || !scene::CreateNative(base)) {
    __imp__sub_820C8A50(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = scene::Create(base, ctx.r1.u32);
}

void SceneLoadAssets(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive() || !NativeArchiveQueue()) {
    __imp__sub_820C8B40(ctx, base);
    return;
  }
  ctx.r3.u64 = scene::LoadAssets(base, ctx.r3.u32, ctx.r13.u32, NativeArchiveLoad(base));
}

void SceneOnFileLoaded(PPCContext& ctx, uint8_t* base) {
  const uint32_t request = ctx.r3.u32, data = ctx.r4.u32, scene_at = ctx.r6.u32;
  if (!ModelsNativeLifetime() || !scene::FileLoadedNative(base, request, scene_at)) {
    __imp__sub_820C52A0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr, r3_in = ctx.r3.u64;
  std::optional<uint64_t> r3;
  // The guest calls from the original's frame (128 bytes).
  guest_call::InFrame(ctx, base, 128, 0x820C52D0, [&](uint32_t) {
    hooks::ModelLoadGuest model_guest{ctx, base};
    scene::FileLoadedCalls calls;
    calls.model_load = model_guest.Calls();
    calls.model_load_original = [&](uint32_t model, uint32_t file, uint32_t xpr) {
      ctx.r3.u64 = model;
      ctx.r4.u64 = file;
      ctx.r5.u64 = xpr;
      ctx.lr = 0x820C5480;
      __imp__sub_82596920(ctx, base);
    };
    calls.xpr_create = [&](uint32_t xpr, uint32_t file) {
      ctx.r3.u64 = xpr;
      ctx.r4.u64 = file;
      ctx.lr = 0x820C52D0;
      __imp__sub_82596F68(ctx, base);
    };
    calls.provider_set = [&](uint32_t provider, uint32_t xpr) -> uint64_t {
      ctx.r3.u64 = provider;
      ctx.r4.u64 = 0;
      ctx.r5.u64 = xpr;
      ctx.lr = 0x820C5340;
      REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, provider) + 16));
      return ctx.r3.u64;
    };
    calls.tga_create = [&](uint32_t header, uint32_t size, uint32_t buffer_at, uint32_t pixels) {
      ctx.r3.u64 = header;
      ctx.r4.u64 = size;
      ctx.r5.u64 = buffer_at;
      ctx.r6.u64 = pixels;
      ctx.lr = 0x820C54F0;
      __imp__sub_820ED020(ctx, base);
    };
    r3 = scene::FileLoaded(base, request, data, scene_at, stack, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3 ? *r3 : r3_in;
  ctx.fpscr.disableFlushMode();
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
ITC_SCENE(820C9110, SceneCarSceneDestruct)
ITC_SCENE(820C4C80, SceneAssetsRelease)
ITC_SCENE(820C8A50, SceneCreate)
ITC_SCENE(820C8B40, SceneLoadAssets)
ITC_SCENE(820C52A0, SceneOnFileLoaded)
ITC_SCENE(820C5798, SceneLoadsDone)
// No adapters (their callers in play are native; the register steps above
// are the harness's): ScenePools::StartTeardownA sub_820C3C80, DestroyA
// sub_820C3CF8, SceneAssets::SceneAssets sub_820C5008 and ~SceneAssets
// sub_820C5168, List::DeleteAll sub_82593FF8 and ~List sub_82594188,
// CarScene::InitLighting sub_820C8528, CarScene::CarScene sub_820C89A8 and
// SceneAssets::Reset sub_820C4A70.
