// The scene's loading objects natively: adapters in front of
// src/scene_loaders/ (docs/functions/scene-loaders.md), with the register
// steps the harness shares (hooks/scene_loaders.h).

#include "hooks/scene_loaders.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/objects.h"
#include "scene_loaders/pools.h"

ITC_FLAG_BOOL(native_scene_loaders, true, "ITC",
              "Run the scene's loading objects (docs/functions/scene-loaders.md: with "
              "--native_heap ScenePools::MakeA sub_820C3C40 and sub_820D0960; PoolNode::PoolNode "
              "sub_82594050 and ~PoolNode sub_825940B0; CarModel::IsLoaded sub_820D3C28, "
              "Pool4Object::IsLoaded sub_820D6A08 and SetRecord sub_820D6A60, CarParts::Part "
              "sub_820D6E10 (with SlotOf sub_820D6D48, no adapter), IsLoaded sub_820D7258 and "
              "WalkMaterials sub_820D6EB8 (with sub_820D2270, no adapter), Interior::IsLoaded "
              "sub_820D7790 (with Accessories::AllLoaded sub_820F07D8, no adapter); NameMatches "
              "sub_820CE800 and XPR::FindByName sub_820D09B8 (with XPR::Count sub_82596F20 and "
              "RecordName sub_82596F40, no adapters); with --native_heap and the memory blocks' host lock "
              "CarModelLoads::CarModelLoads sub_820D3DE0, ~CarModelLoads sub_820D3E40 and Reset sub_820D3B58, "
              "CarModel::Reset sub_820D3E98, CarPartsLoads::CarPartsLoads sub_820D6F88 and "
              "~CarPartsLoads sub_820D6FE8, CarParts::CarParts sub_820D7428 and Release "
              "sub_820D6D00, Pool4Object::Reset sub_820D6948) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=scene_loaders)");

namespace hooks {

namespace sl = scene_loaders;

void SceneLoadersMakeA(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820C3C40(ctx, base);
    return;
  }
  ctx.r3.u64 = sl::MakeA(base);
}

void SceneLoadersAllocPages(PPCContext& ctx, uint8_t* base) {
  if (!hooks::NativeHeapActive()) {
    __imp__sub_820D0960(ctx, base);
    return;
  }
  ctx.r3.u64 = sl::AllocPages(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneLoadersPoolNodeConstruct(PPCContext& ctx, uint8_t* base) {
  sl::PoolNodeConstruct(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneLoadersPoolNodeDestruct(PPCContext& ctx, uint8_t* base) {
  sl::PoolNodeDestruct(base, ctx.r3.u32);
}

void SceneLoadersCarModelIsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::CarModelIsLoaded(base, ctx.r3.u32);
}

void SceneLoadersPool4IsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::Pool4IsLoaded(base, ctx.r3.u32);
}

void SceneLoadersPartsIsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::PartsIsLoaded(base, ctx.r3.u32);
}

void SceneLoadersInteriorIsLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::InteriorIsLoaded(base, ctx.r3.u32);
}

void SceneLoadersAccessoriesAllLoaded(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::AccessoriesAllLoaded(base, ctx.r3.u32);
}

void SceneLoadersPool4SetRecord(PPCContext& ctx, uint8_t* base) {
  if (!sl::Pool4SetRecord(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820D6A60(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 28;
}

void SceneLoadersPartsSlotOf(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::PartsSlotOf(ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, ctx.r6.u64);
}

void SceneLoadersPartsPart(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::PartsPart(base, ctx.r3.u32, ctx.r4.s32, ctx.r5.u32, ctx.r6.u32);
}

void SceneLoadersPartWalkMaterials(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::PartWalkMaterials(base, ctx.r3.u32);
}

void SceneLoadersPartsWalkMaterials(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::PartsWalkMaterials(base, ctx.r3.u32);
}

void SceneLoadersNameMatches(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::NameMatches(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneLoadersXprCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::XprCount(base, ctx.r3.u32);
}

void SceneLoadersXprRecordName(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sl::XprRecordName(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneLoadersXprFindByName(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = sl::XprFindByName(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneLoadersCarModelLoadsConstruct(PPCContext& ctx, uint8_t* base) {
  if (!hooks::ModelsNativeLifetime() || !sl::CarModelLoadsConstruct(base, ctx.r3.u32)) {
    __imp__sub_820D3DE0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as Skeleton::Skeleton leaves it
}

void SceneLoadersCarModelLoadsDestruct(PPCContext& ctx, uint8_t* base) {
  if (!hooks::ModelsNativeLifetime() || !sl::CarModelLoadsDestruct(base, ctx.r3.u32)) {
    __imp__sub_820D3E40(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ctx.r3.u32 + sl::kCarModelLoadsSkeleton;
}

// The resets that free: the plain C++, or the original where it refuses.
#define ITC_SCENE_LOADERS_RESET(name, address, function)                  \
  void name(PPCContext& ctx, uint8_t* base) {                             \
    if (hooks::ModelsNativeLifetime()) {                                  \
      ctx.fpscr.disableFlushMode();                                       \
      if (const auto r3 = sl::function(base, ctx.r3.u32)) {               \
        ctx.r3.u64 = *r3;                                                 \
        return;                                                           \
      }                                                                   \
    }                                                                     \
    __imp__sub_##address(ctx, base);                                      \
  }

ITC_SCENE_LOADERS_RESET(SceneLoadersCarModelLoadsReset, 820D3B58, CarModelLoadsReset)
ITC_SCENE_LOADERS_RESET(SceneLoadersCarModelReset, 820D3E98, CarModelReset)

ITC_SCENE_LOADERS_RESET(SceneLoadersCarPartsRelease, 820D6D00, CarPartsRelease)
ITC_SCENE_LOADERS_RESET(SceneLoadersCarPartsLoadsDestruct, 820D6FE8, CarPartsLoadsDestruct)
ITC_SCENE_LOADERS_RESET(SceneLoadersPool4Reset, 820D6948, Pool4Reset)

void SceneLoadersCarPartsLoadsConstruct(PPCContext& ctx, uint8_t* base) {
  if (hooks::ModelsNativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (sl::CarPartsLoadsConstruct(base, ctx.r3.u32)) {
      return;
    }
  }
  __imp__sub_820D6F88(ctx, base);
}

void SceneLoadersCarPartsConstruct(PPCContext& ctx, uint8_t* base) {
  if (hooks::ModelsNativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (sl::CarPartsConstruct(base, ctx.r3.u32, ctx.r4.u32)) {
      return;
    }
  }
  __imp__sub_820D7428(ctx, base);
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. CarParts::SlotOf, sub_820D2270, Accessories::AllLoaded, XPR::Count
// and XPR::RecordName have none: each has one caller, made native here, whose
// plain C++ calls theirs.

#define ITC_SCENE_LOADERS(address, step)   \
  extern "C" REX_FUNC(sub_##address) {     \
    REX_FUNC_PROLOGUE();                   \
    if (!ITC_FLAG(native_scene_loaders)) { \
      __imp__sub_##address(ctx, base);     \
      return;                              \
    }                                      \
    hooks::step(ctx, base);                \
  }

ITC_SCENE_LOADERS(820C3C40, SceneLoadersMakeA)
ITC_SCENE_LOADERS(820D0960, SceneLoadersAllocPages)
ITC_SCENE_LOADERS(82594050, SceneLoadersPoolNodeConstruct)
ITC_SCENE_LOADERS(825940B0, SceneLoadersPoolNodeDestruct)
ITC_SCENE_LOADERS(820D3C28, SceneLoadersCarModelIsLoaded)
ITC_SCENE_LOADERS(820D6A08, SceneLoadersPool4IsLoaded)
ITC_SCENE_LOADERS(820D6A60, SceneLoadersPool4SetRecord)
ITC_SCENE_LOADERS(820D6E10, SceneLoadersPartsPart)
ITC_SCENE_LOADERS(820D7258, SceneLoadersPartsIsLoaded)
ITC_SCENE_LOADERS(820D6EB8, SceneLoadersPartsWalkMaterials)
ITC_SCENE_LOADERS(820D7790, SceneLoadersInteriorIsLoaded)
ITC_SCENE_LOADERS(820CE800, SceneLoadersNameMatches)
ITC_SCENE_LOADERS(820D09B8, SceneLoadersXprFindByName)
ITC_SCENE_LOADERS(820D3DE0, SceneLoadersCarModelLoadsConstruct)
ITC_SCENE_LOADERS(820D3E40, SceneLoadersCarModelLoadsDestruct)
ITC_SCENE_LOADERS(820D3B58, SceneLoadersCarModelLoadsReset)
ITC_SCENE_LOADERS(820D3E98, SceneLoadersCarModelReset)
ITC_SCENE_LOADERS(820D6F88, SceneLoadersCarPartsLoadsConstruct)
ITC_SCENE_LOADERS(820D7428, SceneLoadersCarPartsConstruct)
ITC_SCENE_LOADERS(820D6D00, SceneLoadersCarPartsRelease)
ITC_SCENE_LOADERS(820D6FE8, SceneLoadersCarPartsLoadsDestruct)
ITC_SCENE_LOADERS(820D6948, SceneLoadersPool4Reset)
