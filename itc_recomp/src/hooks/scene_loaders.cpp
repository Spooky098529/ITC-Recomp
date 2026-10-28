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
              "CarModelLoads::CarModelLoads sub_820D3DE0, ~CarModelLoads sub_820D3E40 and Reset sub_820D3B58 (no adapter: only CarModel::Reset calls it), "
              "CarModel::Reset sub_820D3E98, CarPartsLoads::CarPartsLoads sub_820D6F88 (no adapter: only CarParts::CarParts calls it) and "
              "~CarPartsLoads sub_820D6FE8, CarParts::CarParts sub_820D7428 and Release "
              "sub_820D6D00, Pool4Object::Reset sub_820D6948, CockpitLoads::CockpitLoads sub_820D6010 (no "
              "adapter: only Cockpit::Cockpit calls it) and ~CockpitLoads sub_820D6090, "
              "Cockpit::Cockpit sub_820D63A0 and Release sub_820D6238, Interior::Interior "
              "sub_820D78B8, WheelsLoads::WheelsLoads sub_820D8F60, Reset sub_820D8AA8 (no "
              "adapter: only Wheels::Reset calls it) and ~WheelsLoads sub_820D9010, Wheels::Reset "
              "sub_820D8B58, TrafficLoads::TrafficLoads sub_820D7C40 and ~TrafficLoads "
              "sub_820D7CC0, TrafficCar::Reset sub_820D7AB0, Pool4Object::Pool4Object sub_820D6A70, Release "
              "sub_820D6A20, ~Pool4Object sub_820D6AD0 and its deleting destructor sub_820D6BC0, "
              "Wheels::Wheels sub_820D9570, Release sub_820D8E68, ~Wheels sub_820D90B8 and its "
              "deleting destructor sub_820D95E8) natively instead of through the "
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

ITC_SCENE_LOADERS_RESET(SceneLoadersCockpitRelease, 820D6238, CockpitRelease)
ITC_SCENE_LOADERS_RESET(SceneLoadersCockpitLoadsDestruct, 820D6090, CockpitLoadsDestruct)

// The constructors: the plain C++ (r3 left), or the original where it
// refuses.
#define ITC_SCENE_LOADERS_CONSTRUCT(name, address, call)  \
  void name(PPCContext& ctx, uint8_t* base) {              \
    if (hooks::ModelsNativeLifetime()) {                   \
      ctx.fpscr.disableFlushMode();                        \
      if (sl::call) {                                      \
        return;                                            \
      }                                                    \
    }                                                      \
    __imp__sub_##address(ctx, base);                       \
  }

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCockpitLoadsConstruct, 820D6010,
                            CockpitLoadsConstruct(base, ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCockpitConstruct, 820D63A0,
                            CockpitConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersInteriorConstruct, 820D78B8,
                            InteriorConstruct(base, ctx.r3.u32, ctx.r4.u32))

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsLoadsConstruct, 820D8F60,
                            WheelsLoadsConstruct(base, ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersTrafficLoadsConstruct, 820D7C40,
                            TrafficLoadsConstruct(base, ctx.r3.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsLoadsReset, 820D8AA8, WheelsLoadsReset)
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsLoadsDestruct, 820D9010, WheelsLoadsDestruct)
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsReset, 820D8B58, WheelsReset)
ITC_SCENE_LOADERS_RESET(SceneLoadersTrafficLoadsDestruct, 820D7CC0, TrafficLoadsDestruct)
ITC_SCENE_LOADERS_RESET(SceneLoadersTrafficReset, 820D7AB0, TrafficReset)

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersPool4Construct, 820D6A70,
                            Pool4Construct(base, ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersPool4Release, 820D6A20, Pool4Release)
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersPool4Destruct, 820D6AD0, Pool4Destruct(base, ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersPool4DeletingDestruct, 820D6BC0,
                            Pool4DeletingDestruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsConstruct, 820D9570,
                            WheelsConstruct(base, ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsRelease, 820D8E68, WheelsRelease)
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsDestruct, 820D90B8, WheelsDestruct(base, ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsDeletingDestruct, 820D95E8,
                            WheelsDeletingDestruct(base, ctx.r3.u32, ctx.r4.u32))

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. CarParts::SlotOf, sub_820D2270, Accessories::AllLoaded, XPR::Count
// and XPR::RecordName have none: each has one caller, made native here, whose
// plain C++ calls theirs; nor do CarModelLoads::Reset (only CarModel::Reset
// calls it) and CarPartsLoads::CarPartsLoads (only CarParts::CarParts): their
// register steps stay for the harness; nor do CockpitLoads::CockpitLoads
// (only Cockpit::Cockpit) and WheelsLoads::Reset (only Wheels::Reset).

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
ITC_SCENE_LOADERS(820D3E98, SceneLoadersCarModelReset)
ITC_SCENE_LOADERS(820D7428, SceneLoadersCarPartsConstruct)
ITC_SCENE_LOADERS(820D6D00, SceneLoadersCarPartsRelease)
ITC_SCENE_LOADERS(820D6FE8, SceneLoadersCarPartsLoadsDestruct)
ITC_SCENE_LOADERS(820D6948, SceneLoadersPool4Reset)
ITC_SCENE_LOADERS(820D63A0, SceneLoadersCockpitConstruct)
ITC_SCENE_LOADERS(820D6238, SceneLoadersCockpitRelease)
ITC_SCENE_LOADERS(820D6090, SceneLoadersCockpitLoadsDestruct)
ITC_SCENE_LOADERS(820D78B8, SceneLoadersInteriorConstruct)
ITC_SCENE_LOADERS(820D8F60, SceneLoadersWheelsLoadsConstruct)
ITC_SCENE_LOADERS(820D9010, SceneLoadersWheelsLoadsDestruct)
ITC_SCENE_LOADERS(820D8B58, SceneLoadersWheelsReset)
ITC_SCENE_LOADERS(820D7C40, SceneLoadersTrafficLoadsConstruct)
ITC_SCENE_LOADERS(820D7CC0, SceneLoadersTrafficLoadsDestruct)
ITC_SCENE_LOADERS(820D7AB0, SceneLoadersTrafficReset)
ITC_SCENE_LOADERS(820D6A70, SceneLoadersPool4Construct)
ITC_SCENE_LOADERS(820D6A20, SceneLoadersPool4Release)
ITC_SCENE_LOADERS(820D6AD0, SceneLoadersPool4Destruct)
ITC_SCENE_LOADERS(820D6BC0, SceneLoadersPool4DeletingDestruct)
ITC_SCENE_LOADERS(820D9570, SceneLoadersWheelsConstruct)
ITC_SCENE_LOADERS(820D8E68, SceneLoadersWheelsRelease)
ITC_SCENE_LOADERS(820D90B8, SceneLoadersWheelsDestruct)
ITC_SCENE_LOADERS(820D95E8, SceneLoadersWheelsDeletingDestruct)
