// The scene's loading objects natively: adapters in front of
// src/scene_loaders/ (docs/functions/scene-loaders.md), with the register
// steps the harness shares (hooks/scene_loaders.h).

#include "hooks/scene_loaders.h"

#include "base/flags.h"
#include "hooks/heap.h"
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
              "RecordName sub_82596F40, no adapters)) natively instead of through the "
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
