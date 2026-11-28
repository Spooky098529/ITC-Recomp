// The scene's loading objects natively: adapters in front of
// src/scene_loaders/ (docs/functions/scene-loaders.md), with the register
// steps the harness shares (hooks/scene_loaders.h).

#include "hooks/scene_loaders.h"
#include "lifted/names.h"
#include "lifted/entries.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_models/plate.h"
#include "hooks/accessories.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "native/buffers.h"
#include "scene_loaders/file_loaded.h"
#include "scene_loaders/loads.h"
#include "scene_loaders/model_point.h"
#include "scene_loaders/objects.h"
#include "scene_loaders/pools.h"
#include "schema/models.h"
#include "schema/scene_loaders.h"
#include "lifted/sdk.h"

ITC_FLAG_DECLARE(bool, native_material_passes);
ITC_FLAG_DECLARE(bool, native_models);
ITC_FLAG_DECLARE(bool, native_accessories);
ITC_FLAG_DECLARE(bool, native_car_models);

ITC_FLAG_BOOL(native_scene_loaders, true, "ITC",
              "Run the scene's loading objects (docs/functions/scene-loaders.md: with "
              "--native_heap ScenePools::MakeA sub_820C3C40 and sub_820D0960; PoolNode::PoolNode "
              "sub_82594050 and ~PoolNode sub_825940B0; CarModel::IsLoaded sub_820D3C28, "
              "Pool4Object::IsLoaded sub_820D6A08 and SetRecord sub_820D6A60, CarParts::Part "
              "sub_820D6E10 (with SlotOf sub_820D6D48, no adapter), IsLoaded sub_820D7258 and "
              "WalkMaterials sub_820D6EB8 (with sub_820D2270, no adapter), with --native_material_passes "
              "the paint walks sub_820D6F20, sub_820D7388, sub_820D5FC8, sub_820D6328, sub_820D7690, "
              "sub_820D7840 and sub_820D7BF8 (over "
              "PartModel::ApplyPalette's and SetPaint's plain C++), with --native_models "
              "CarModel::SelectedTexture sub_820D4588 (over XPR::GetTexture's), Cockpit::BindTextures "
              "sub_820D6108 and TrafficCar::BindTextures sub_820D7D38 (over XPR::GetTexture's, "
              "Model::SetVinylTextures' and sprintf's), with native memcpy ModelPoint::Find "
              "sub_820D02D0 (Skeleton::UpdateMatrices' original where its plain C++ declines), "
              "Interior::IsLoaded "
              "sub_820D7790 (with Accessories::AllLoaded sub_820F07D8, no adapter); NameMatches "
              "sub_820CE800 and XPR::FindByName sub_820D09B8 (with XPR::Count sub_82596F20 and "
              "RecordName sub_82596F40, no adapters); with --native_heap and the memory blocks' host lock "
              "CarModelLoads::CarModelLoads sub_820D3DE0, ~CarModelLoads sub_820D3E40 and Reset sub_820D3B58 (no adapter: only CarModel::Reset calls it), "
              "CarModel::Reset sub_820D3E98, CarPartsLoads::CarPartsLoads sub_820D6F88 (no adapter: only CarParts::CarParts calls it) and "
              "~CarPartsLoads sub_820D6FE8, CarParts::CarParts sub_820D7428 and Release "
              "sub_820D6D00, Pool4Object::Reset sub_820D6948 (no adapter: its callers here call it), CockpitLoads::CockpitLoads sub_820D6010 (no "
              "adapter: only Cockpit::Cockpit calls it) and ~CockpitLoads sub_820D6090, "
              "Cockpit::Cockpit sub_820D63A0 and Release sub_820D6238, Interior::Interior "
              "sub_820D78B8, WheelsLoads::WheelsLoads sub_820D8F60 (no adapter: only Wheels::Wheels calls it), Reset sub_820D8AA8 (no "
              "adapter: only Wheels::Reset calls it) and ~WheelsLoads sub_820D9010 (no adapter: only ~Wheels calls it), Wheels::Reset "
              "sub_820D8B58 (no adapter: its callers here call it), TrafficLoads::TrafficLoads sub_820D7C40 and ~TrafficLoads "
              "sub_820D7CC0, TrafficCar::Reset sub_820D7AB0, Pool4Object::Pool4Object sub_820D6A70, Release "
              "sub_820D6A20, ~Pool4Object sub_820D6AD0 (no adapter: only the deleting destructor calls it) and its deleting destructor sub_820D6BC0, "
              "Wheels::Wheels sub_820D9570, Release sub_820D8E68, ~Wheels sub_820D90B8 (no adapter: only the deleting destructor calls it) and its "
              "deleting destructor sub_820D95E8, CarModel::CarModel sub_820D5890, "
              "~CarModel sub_820D4750, ~CarParts sub_820D7038, ~Cockpit sub_820D64D0 and their "
              "deleting destructors sub_820D5988, sub_820D74C0, sub_820D6798, "
              "TrafficCar::TrafficCar sub_820D8908, Release sub_820D7B90, ~TrafficCar sub_820D7E90 "
              "and its deleting destructor sub_820D89B8, ScenePools::CreateA sub_820C3FD8 (the "
              "constructors' originals called where their plain C++ declines), "
              "Interior::OnPartLoaded sub_820D7578 (Model::Load's original where its plain C++ "
              "declines), Cockpit::OnFileLoaded sub_820D6528 (Model::Load's, XPR::Create's, "
              "Skeleton::Bind's and UpdateMatrices' originals where their plain C++ declines, "
              "the texture provider's slot +16 through its vtable), with native memcpy "
              "TrafficCar::OnFileLoaded sub_820D8430 (the same, and MakePlate's Resource::Release); with those and the accessories' release native Interior::Release "
              "sub_820D75F8, ~Interior sub_820D76D8, its deleting destructor sub_820D7960 and "
              "ScenePools::ReleaseA sub_820C4488 (pool 0 with --native_car_models)) natively instead of through the "
              "translated code (the same writes; checked by --run_harnesses=scene_loaders); "
              "the ones only these native callers reach (the members' constructors, resets and "
              "destructors, the destructors under the deleting ones) have no adapters");

namespace hooks {

namespace sl = scene_loaders;

ITC_REGS_STEP(SceneLoadersMakeA) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820C3C40(ctx);
    return;
  }
  ctx.r3.u64 = sl::MakeA();
}

ITC_REGS_STEP(SceneLoadersAllocPages) {
  if (!hooks::NativeHeapActive()) {
    lifted::original::c820D0960(ctx);
    return;
  }
  ctx.r3.u64 = sl::AllocPages(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneLoadersPoolNodeConstruct) {
  sl::PoolNodeConstruct(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneLoadersPoolNodeDestruct) {
  sl::PoolNodeDestruct(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersCarModelIsLoaded) {
  ctx.r3.u64 = sl::CarModelIsLoaded(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersPool4IsLoaded) {
  ctx.r3.u64 = sl::Pool4IsLoaded(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersPartsIsLoaded) {
  ctx.r3.u64 = sl::PartsIsLoaded(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersInteriorIsLoaded) {
  ctx.r3.u64 = sl::InteriorIsLoaded(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersAccessoriesAllLoaded) {
  ctx.r3.u64 = sl::AccessoriesAllLoaded(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersPool4SetRecord) {
  if (!sl::Pool4SetRecord(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820D6A60(ctx);
    return;
  }
  ctx.r3.u64 = schema::Pool4Object(ctx.r3.u32).record_address();
}

ITC_REGS_STEP(SceneLoadersPartsSlotOf) {
  ctx.r3.u64 = sl::PartsSlotOf(ctx.r3.u64, ctx.r4.u32, ctx.r5.u64, ctx.r6.u64);
}

ITC_REGS_STEP(SceneLoadersPartsPart) {
  ctx.r3.u64 = sl::PartsPart(ctx.r3.u32, ctx.r4.s32, ctx.r5.u32, ctx.r6.u32);
}

ITC_REGS_STEP(SceneLoadersPartWalkMaterials) {
  ctx.r3.u64 = sl::PartWalkMaterials(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersPartsWalkMaterials) {
  ctx.r3.u64 = sl::PartsWalkMaterials(ctx.r3.u32);
}

namespace {

// The paint walks' callees' flag: the original when it's off.
template <typename Walk>
void PaintWalk(lifted::Regs& ctx, lifted::RegsFunction* original,
               Walk walk) {
  if (!ITC_FLAG(native_material_passes)) {
    lifted::Sdk(ctx, original);
    return;
  }
  const uint64_t regs[6] = {ctx.r4.u64, ctx.r5.u64, ctx.r6.u64,
                            ctx.r7.u64, ctx.r8.u64, ctx.r9.u64};
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = walk(ctx.r3.u32, ctx.r3.u64, regs, ctx.r1.u32);
}

}  // namespace

ITC_REGS_STEP(SceneLoadersPartsApplyPalette) {
  PaintWalk(ctx, lifted::original::c820D6F20,
            [&](uint32_t object, uint64_t r3, const uint64_t*, uint32_t sp) {
              return sl::PartsApplyPalette(object, r3, sp);
            });
}

ITC_REGS_STEP(SceneLoadersPartsSetPaint) {
  PaintWalk(ctx, lifted::original::c820D7388,
            [&](uint32_t object, uint64_t r3, const uint64_t* regs, uint32_t sp) {
              return sl::PartsSetPaint(object, r3, regs, sp);
            });
}

ITC_REGS_STEP(SceneLoadersCockpitApplyPalette) {
  PaintWalk(ctx, lifted::original::c820D5FC8,
            [&](uint32_t object, uint64_t r3, const uint64_t*, uint32_t sp) {
              return sl::CockpitApplyPalette(object, r3, sp);
            });
}

ITC_REGS_STEP(SceneLoadersCockpitSetPaint) {
  PaintWalk(ctx, lifted::original::c820D6328,
            [&](uint32_t object, uint64_t r3, const uint64_t* regs, uint32_t sp) {
              return sl::CockpitSetPaint(object, r3, regs, sp);
            });
}

ITC_REGS_STEP(SceneLoadersInteriorApplyPalette) {
  PaintWalk(ctx, lifted::original::c820D7690,
            [&](uint32_t object, uint64_t r3, const uint64_t*, uint32_t sp) {
              return sl::InteriorApplyPalette(object, r3, sp);
            });
}

ITC_REGS_STEP(SceneLoadersInteriorSetPaint) {
  PaintWalk(ctx, lifted::original::c820D7840,
            [&](uint32_t object, uint64_t r3, const uint64_t* regs, uint32_t sp) {
              return sl::InteriorSetPaint(object, r3, regs, sp);
            });
}

ITC_REGS_STEP(SceneLoadersTrafficApplyPalette) {
  PaintWalk(ctx, lifted::original::c820D7BF8,
            [&](uint32_t object, uint64_t r3, const uint64_t*, uint32_t sp) {
              return sl::TrafficApplyPalette(object, r3, sp);
            });
}

namespace {

// The interior's release's callees native: the models' lifetime and the
// accessories' release.
bool InteriorNative() {
  return hooks::ModelsNativeLifetime() && ITC_FLAG(native_accessories) &&
         hooks::AccessoriesNativeLoads();
}

}  // namespace

ITC_REGS_STEP(SceneLoadersInteriorRelease) {
  if (InteriorNative()) {
    ctx.fpscr.disableFlushMode();
    if (const auto r3 = sl::InteriorRelease(ctx.r3.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c820D75F8(ctx);
}

ITC_REGS_STEP(SceneLoadersInteriorDestruct) {
  if (InteriorNative()) {
    ctx.fpscr.disableFlushMode();
    if (sl::InteriorDestruct(ctx.r3.u32)) {
      return;
    }
  }
  lifted::original::c820D76D8(ctx);
}

ITC_REGS_STEP(SceneLoadersInteriorDeletingDestruct) {
  if (InteriorNative()) {
    ctx.fpscr.disableFlushMode();
    if (sl::InteriorDeletingDestruct(ctx.r3.u32, ctx.r4.u32)) {
      return;
    }
  }
  lifted::original::c820D7960(ctx);
}

bool SceneLoadersReleaseANative(uint32_t pool) {
  return InteriorNative() && (pool != 0 || ITC_FLAG(native_car_models));
}

ITC_REGS_STEP(SceneLoadersReleaseA) {
  const uint32_t pool = ctx.r4.u32;
  if (SceneLoadersReleaseANative(pool)) {
    ctx.fpscr.disableFlushMode();
    if (const auto r3 = sl::ReleaseA(ctx.r3.u32, pool, ctx.r5.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c820C4488(ctx);
}

ITC_REGS_STEP(SceneLoadersCarModelSelectedTexture) {
  if (!ITC_FLAG(native_models)) {
    lifted::original::c820D4588(ctx);
    return;
  }
  ctx.r3.u64 = sl::CarModelSelectedTexture(ctx.r3.u32);
}

// The texture binds: their callees' plain C++ (XPR::GetTexture's and
// Model::SetVinylTextures' under --native_models), or the original.
ITC_REGS_STEP(SceneLoadersCockpitBindTextures) {
  if (ITC_FLAG(native_models)) {
    if (const auto r3 = sl::CockpitBindTextures(ctx.r3.u32, ctx.r1.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c820D6108(ctx);
}

ITC_REGS_STEP(SceneLoadersTrafficBindTextures) {
  if (ITC_FLAG(native_models)) {
    if (const auto r3 = sl::TrafficBindTextures(ctx.r3.u32, ctx.r1.u32)) {
      ctx.r3.u64 = *r3;
      return;
    }
  }
  lifted::original::c820D7D38(ctx);
}

ITC_REGS_STEP(SceneLoadersModelPointFind) {
  if (!ITC_FLAG(native_models) || !native::Buffers_NativeMemcpy()) {
    lifted::original::c820D02D0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t point = ctx.r3.u32, model = ctx.r4.u32, stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  uint32_t r3 = 0;
  // Skeleton::UpdateMatrices' original from the original's frame (320
  // bytes), with its return address.
  guest_call::InFrame(ctx, sl::kModelPointFrameBytes, 0x820D03EC, [&](uint32_t) {
    r3 = sl::ModelPointFind(point, model, stack, [&](uint32_t skeleton) {
      ctx.r3.u64 = skeleton;
      ctx.lr = 0x820D03EC;
      lifted::original::c825998A0(ctx);
    });
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  if (r3 == 1) {
    ctx.fpscr.enableFlushMode();  // as Matrix::Inverse's adapter leaves it
  } else {
    ctx.fpscr.disableFlushMode();
  }
}

namespace {

// The file-loaded callbacks' guest calls, each with the original's return
// address (the callback's frame made by the caller).
struct FileLoadedReturns {
  uint32_t model_load, xpr_create, provider_set, skeleton_bind, update_matrices;
};

sl::FileLoadedCalls MakeFileLoadedCalls(lifted::Regs& ctx, hooks::ModelLoadGuest& model_guest,
                                        const FileLoadedReturns& at) {
  sl::FileLoadedCalls calls;
  calls.model_load = model_guest.Calls();
  calls.model_load_original = [&ctx, at](uint32_t model, uint32_t file) {
    ctx.r3.u64 = model;
    ctx.r4.u64 = file;
    ctx.r5.u64 = 0;
    ctx.lr = at.model_load;
    lifted::original::c82596920(ctx);
  };
  calls.xpr_create = [&ctx, at](uint32_t xpr, uint32_t file) {
    ctx.r3.u64 = xpr;
    ctx.r4.u64 = file;
    ctx.lr = at.xpr_create;
    lifted::original::c82596F68(ctx);
  };
  calls.provider_set = [&ctx, at](uint32_t provider, uint32_t xpr) -> uint64_t {
    ctx.r3.u64 = provider;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = xpr;
    ctx.lr = at.provider_set;
    // The provider's vtable's slot 4, TextureProvider::Set (models.md).
    lifted::CallFromContext(ctx, schema::TextureProvider(provider).vtable().set());
    return ctx.r3.u64;
  };
  calls.skeleton_bind = [&ctx, at](uint32_t skeleton, uint32_t file) {
    ctx.r3.u64 = skeleton;
    ctx.r4.u64 = file;
    ctx.r5.u64 = 0;
    ctx.lr = at.skeleton_bind;
    lifted::original::c82599A20(ctx);
  };
  calls.update_matrices = [&ctx, at](uint32_t skeleton) {
    ctx.r3.u64 = skeleton;
    ctx.lr = at.update_matrices;
    lifted::original::c825998A0(ctx);
  };
  calls.plate_release = [&ctx](uint32_t surface) {
    // From MakePlate's frame, below the callback's.
    guest_call::InFrame(ctx, car_models::kPlateFrameBytes, 0x820ED8E4, [&](uint32_t) {
      ctx.r3.u64 = surface;
      lifted::Enter(ctx, game::Resource::Release);
    });
  };
  return calls;
}

}  // namespace

ITC_REGS_STEP(SceneLoadersCockpitOnFileLoaded) {
  const uint32_t request = ctx.r3.u32, buffer = ctx.r4.u32, cockpit = ctx.r6.u32;
  if (!hooks::ModelsNativeLifetime() || !sl::CockpitFileLoadedNative(request, cockpit)) {
    lifted::original::c820D6528(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  uint64_t r3 = 0;
  // The guest calls from the original's frame (272 bytes).
  guest_call::InFrame(ctx, sl::kCockpitLoadedFrameBytes, 0x820D6738, [&](uint32_t) {
    hooks::ModelLoadGuest model_guest{ctx};
    const sl::FileLoadedCalls calls = MakeFileLoadedCalls(
        ctx, model_guest, {0x820D6738, 0x820D6768, 0x820D678C, 0x820D65A0, 0x820D65B8});
    r3 = sl::CockpitFileLoaded(request, buffer, cockpit, stack, calls);
  });
  ctx.lr = lr;
  ctx.r3.u64 = r3;
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(SceneLoadersTrafficOnFileLoaded) {
  const uint32_t request = ctx.r3.u32, traffic = ctx.r6.u32;
  if (!hooks::ModelsNativeLifetime() || !sl::TrafficFileLoadedNative(request, traffic)) {
    lifted::original::c820D8430(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint32_t stack = ctx.r1.u32;
  const uint64_t lr = ctx.lr;
  std::optional<uint64_t> r3;
  // The guest calls from the original's frame (1,360 bytes).
  guest_call::InFrame(ctx, sl::kTrafficLoadedFrameBytes, 0x820D88E0, [&](uint32_t) {
    hooks::ModelLoadGuest model_guest{ctx};
    const sl::FileLoadedCalls calls = MakeFileLoadedCalls(
        ctx, model_guest, {0x820D88E0, 0x820D84B0, 0x820D84CC, 0x820D854C, 0x820D8564});
    r3 = sl::TrafficFileLoaded(request, traffic, stack, calls);
  });
  ctx.lr = lr;
  if (!r3) {
    ctx.r3.u64 = request;  // sprintf declined: nothing done
    lifted::original::c820D8430(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
  ctx.fpscr.disableFlushMode();
}

ITC_REGS_STEP(SceneLoadersNameMatches) {
  ctx.r3.u64 = sl::NameMatches(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneLoadersXprCount) {
  ctx.r3.u64 = sl::XprCount(ctx.r3.u32);
}

ITC_REGS_STEP(SceneLoadersXprRecordName) {
  ctx.r3.u64 = sl::XprRecordName(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneLoadersXprFindByName) {
  ctx.r3.s64 = sl::XprFindByName(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneLoadersCarModelLoadsConstruct) {
  if (!hooks::ModelsNativeLifetime() || !sl::CarModelLoadsConstruct(ctx.r3.u32)) {
    lifted::original::c820D3DE0(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();  // as Skeleton::Skeleton leaves it
}

ITC_REGS_STEP(SceneLoadersCarModelLoadsDestruct) {
  if (!hooks::ModelsNativeLifetime() || !sl::CarModelLoadsDestruct(ctx.r3.u32)) {
    lifted::original::c820D3E40(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = schema::CarModelLoads(ctx.r3.u32).skeleton_address();
}

// The resets that free: the plain C++, or the original where it refuses.
#define ITC_SCENE_LOADERS_RESET(name, address, function)                  \
  ITC_REGS_STEP(name) {                             \
    if (hooks::ModelsNativeLifetime()) {                                  \
      ctx.fpscr.disableFlushMode();                                       \
      if (const auto r3 = sl::function(ctx.r3.u32)) {               \
        ctx.r3.u64 = *r3;                                                 \
        return;                                                           \
      }                                                                   \
    }                                                                     \
    lifted::original::c##address(ctx);                                      \
  }

ITC_SCENE_LOADERS_RESET(SceneLoadersCarModelLoadsReset, 820D3B58, CarModelLoadsReset)
ITC_SCENE_LOADERS_RESET(SceneLoadersCarModelReset, 820D3E98, CarModelReset)

ITC_SCENE_LOADERS_RESET(SceneLoadersCarPartsRelease, 820D6D00, CarPartsRelease)
ITC_SCENE_LOADERS_RESET(SceneLoadersCarPartsLoadsDestruct, 820D6FE8, CarPartsLoadsDestruct)
ITC_SCENE_LOADERS_RESET(SceneLoadersPool4Reset, 820D6948, Pool4Reset)

ITC_REGS_STEP(SceneLoadersCarPartsLoadsConstruct) {
  if (hooks::ModelsNativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (sl::CarPartsLoadsConstruct(ctx.r3.u32)) {
      return;
    }
  }
  lifted::original::c820D6F88(ctx);
}

ITC_REGS_STEP(SceneLoadersCarPartsConstruct) {
  if (hooks::ModelsNativeLifetime()) {
    ctx.fpscr.disableFlushMode();
    if (sl::CarPartsConstruct(ctx.r3.u32, ctx.r4.u32)) {
      return;
    }
  }
  lifted::original::c820D7428(ctx);
}

ITC_SCENE_LOADERS_RESET(SceneLoadersCockpitRelease, 820D6238, CockpitRelease)
ITC_SCENE_LOADERS_RESET(SceneLoadersCockpitLoadsDestruct, 820D6090, CockpitLoadsDestruct)

// The constructors: the plain C++ (r3 left), or the original where it
// refuses.
#define ITC_SCENE_LOADERS_CONSTRUCT(name, address, call)  \
  ITC_REGS_STEP(name) {              \
    if (hooks::ModelsNativeLifetime()) {                   \
      ctx.fpscr.disableFlushMode();                        \
      if (sl::call) {                                      \
        return;                                            \
      }                                                    \
    }                                                      \
    lifted::original::c##address(ctx);                       \
  }

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCockpitLoadsConstruct, 820D6010,
                            CockpitLoadsConstruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCockpitConstruct, 820D63A0,
                            CockpitConstruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersInteriorConstruct, 820D78B8,
                            InteriorConstruct(ctx.r3.u32, ctx.r4.u32))

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsLoadsConstruct, 820D8F60,
                            WheelsLoadsConstruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersTrafficLoadsConstruct, 820D7C40,
                            TrafficLoadsConstruct(ctx.r3.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsLoadsReset, 820D8AA8, WheelsLoadsReset)
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsLoadsDestruct, 820D9010, WheelsLoadsDestruct)
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsReset, 820D8B58, WheelsReset)
ITC_SCENE_LOADERS_RESET(SceneLoadersTrafficLoadsDestruct, 820D7CC0, TrafficLoadsDestruct)
ITC_SCENE_LOADERS_RESET(SceneLoadersTrafficReset, 820D7AB0, TrafficReset)

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersPool4Construct, 820D6A70,
                            Pool4Construct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersPool4Release, 820D6A20, Pool4Release)
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersPool4Destruct, 820D6AD0, Pool4Destruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersPool4DeletingDestruct, 820D6BC0,
                            Pool4DeletingDestruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsConstruct, 820D9570,
                            WheelsConstruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersWheelsRelease, 820D8E68, WheelsRelease)
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsDestruct, 820D90B8, WheelsDestruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersWheelsDeletingDestruct, 820D95E8,
                            WheelsDeletingDestruct(ctx.r3.u32, ctx.r4.u32))

ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCarModelConstruct, 820D5890,
                            CarModelConstruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCarModelDestruct, 820D4750,
                            CarModelDestruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCarModelDeletingDestruct, 820D5988,
                            CarModelDeletingDestruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCarPartsDestruct, 820D7038,
                            CarPartsDestruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCarPartsDeletingDestruct, 820D74C0,
                            CarPartsDeletingDestruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCockpitDestruct, 820D64D0,
                            CockpitDestruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersCockpitDeletingDestruct, 820D6798,
                            CockpitDeletingDestruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersTrafficConstruct, 820D8908,
                            TrafficConstruct(ctx.r3.u32, ctx.r4.u32))
ITC_SCENE_LOADERS_RESET(SceneLoadersTrafficRelease, 820D7B90, TrafficRelease)
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersTrafficDestruct, 820D7E90,
                            TrafficDestruct(ctx.r3.u32))
ITC_SCENE_LOADERS_CONSTRUCT(SceneLoadersTrafficDeletingDestruct, 820D89B8,
                            TrafficDeletingDestruct(ctx.r3.u32, ctx.r4.u32))

namespace {

// ScenePools::CreateA's constructors' originals, from its frame (160 bytes)
// with each call's return address.
struct CreateAGuest {
  lifted::Regs& ctx;
  static void Construct(void* user, uint32_t pool, uint32_t object, uint32_t list) {
    using Original = void (*)(lifted::Regs&);
    static constexpr Original kOriginals[7] = {
        ITC_REGS_FN(lifted::original::c820D5890), ITC_REGS_FN(lifted::original::c820D7428), ITC_REGS_FN(lifted::original::c820D63A0), ITC_REGS_FN(lifted::original::c820D78B8),
        ITC_REGS_FN(lifted::original::c820D6A70), ITC_REGS_FN(lifted::original::c820D9570), ITC_REGS_FN(lifted::original::c820D8908)};
    static constexpr uint32_t kReturns[7] = {0x820C42A4, 0x820C4278, 0x820C4248, 0x820C421C,
                                             0x820C41F0, 0x820C41C4, 0x820C4198};
    auto& c = *static_cast<CreateAGuest*>(user);
    guest_call::InFrame(c.ctx, 160, kReturns[pool], [&](uint32_t) {
      c.ctx.r3.u64 = object;
      c.ctx.r4.u64 = list;
      kOriginals[pool](c.ctx);
    });
  }
};

}  // namespace

namespace {

// Interior::OnPartLoaded's guest call: Model::Load's original from a frame
// as the callback's (96 bytes).
struct PartLoadedGuest {
  lifted::Regs& ctx;
  static void Load(void* user, uint32_t model, uint32_t file) {
    auto& c = *static_cast<PartLoadedGuest*>(user);
    guest_call::InFrame(c.ctx, 96, 0x820D75D0, [&](uint32_t) {
      c.ctx.r3.u64 = model;
      c.ctx.r4.u64 = file;
      c.ctx.r5.u64 = 0;
      lifted::original::c82596920(c.ctx);
    });
  }
};

}  // namespace

ITC_REGS_STEP(SceneLoadersInteriorOnPartLoaded) {
  if (!hooks::ModelsNativeLifetime()) {
    lifted::original::c820D7578(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t request = ctx.r3.u32, interior = ctx.r6.u32;
  PartLoadedGuest guest{ctx};
  hooks::ModelLoadGuest model_guest{ctx};
  sl::PartLoadedCalls calls;
  calls.model_load = model_guest.Calls();
  calls.load = &PartLoadedGuest::Load;
  calls.user = &guest;
  const auto r3 = sl::InteriorOnPartLoaded(request, interior, calls);
  ctx.lr = lr;
  if (!r3) {
    lifted::original::c820D7578(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(SceneLoadersCreateA) {
  if (!hooks::ModelsNativeLifetime()) {
    lifted::original::c820C3FD8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  const uint32_t set = ctx.r3.u32, record = ctx.r4.u32;
  ctx.r3.u64 = SceneLoadersCreateAWith(ctx, [&](const sl::CreateCalls& calls) {
    return sl::CreateA(set, record, calls);
  });
  ctx.lr = lr;
}

uint32_t SceneLoadersCreateAWith(
    lifted::Regs& ctx, const std::function<uint32_t(const scene_loaders::CreateCalls&)>& run) {
  CreateAGuest guest{ctx};
  sl::CreateCalls calls;
  calls.construct = &CreateAGuest::Construct;
  calls.user = &guest;
  return run(calls);
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. CarParts::SlotOf, sub_820D2270, Accessories::AllLoaded, XPR::Count
// and XPR::RecordName have none: each has one caller, made native here, whose
// plain C++ calls theirs; nor do CarModelLoads::Reset (only CarModel::Reset
// calls it) and CarPartsLoads::CarPartsLoads (only CarParts::CarParts): their
// register steps stay for the harness; nor do CockpitLoads::CockpitLoads
// (only Cockpit::Cockpit) and WheelsLoads::Reset (only Wheels::Reset); nor,
// since their callers are native, Pool4Object::Reset, ~Pool4Object,
// WheelsLoads::WheelsLoads, Wheels::Reset, ~Wheels, ~WheelsLoads,
// CarModel::Reset, CarModelLoads::CarModelLoads and ~CarModelLoads,
// ~CarModel, ~CarParts, ~CarPartsLoads, ~Cockpit, ~CockpitLoads,
// TrafficLoads::TrafficLoads, TrafficCar::Reset, ~TrafficCar,
// ~TrafficLoads, and the seven constructors ScenePools::CreateA calls.

// No adapters either (their callers in play are native: ScenePools::ReleaseA
// and the destructors call their plain C++; the steps above are the
// harness's): CarParts::Release sub_820D6D00, Cockpit::Release sub_820D6238,
// Pool4Object::Release sub_820D6A20, Wheels::Release sub_820D8E68,
// TrafficCar::Release sub_820D7B90, Interior::Release sub_820D75F8 and
// ~Interior sub_820D76D8 (the B7 loop, 2026-11-01).

#define ITC_SCENE_LOADERS(address, step)   \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_scene_loaders)) { \
    hooks::step(ctx);                \
  }

ITC_SCENE_LOADERS(820C3C40, SceneLoadersMakeA)
ITC_SCENE_LOADERS(820C3FD8, SceneLoadersCreateA)
ITC_SCENE_LOADERS(820D7578, SceneLoadersInteriorOnPartLoaded)
ITC_SCENE_LOADERS(820D0960, SceneLoadersAllocPages)
ITC_SCENE_LOADERS(82594050, SceneLoadersPoolNodeConstruct)
ITC_SCENE_LOADERS(825940B0, SceneLoadersPoolNodeDestruct)
ITC_SCENE_LOADERS(820D3C28, SceneLoadersCarModelIsLoaded)
ITC_SCENE_LOADERS(820D6A08, SceneLoadersPool4IsLoaded)
ITC_SCENE_LOADERS(820D6A60, SceneLoadersPool4SetRecord)
ITC_SCENE_LOADERS(820D6E10, SceneLoadersPartsPart)
ITC_SCENE_LOADERS(820D7258, SceneLoadersPartsIsLoaded)
ITC_SCENE_LOADERS(820D6EB8, SceneLoadersPartsWalkMaterials)
ITC_SCENE_LOADERS(820D6F20, SceneLoadersPartsApplyPalette)
ITC_SCENE_LOADERS(820D7388, SceneLoadersPartsSetPaint)
ITC_SCENE_LOADERS(820D5FC8, SceneLoadersCockpitApplyPalette)
ITC_SCENE_LOADERS(820D6328, SceneLoadersCockpitSetPaint)
ITC_SCENE_LOADERS(820D4588, SceneLoadersCarModelSelectedTexture)
ITC_SCENE_LOADERS(820D7690, SceneLoadersInteriorApplyPalette)
ITC_SCENE_LOADERS(820D7840, SceneLoadersInteriorSetPaint)
ITC_SCENE_LOADERS(820D7BF8, SceneLoadersTrafficApplyPalette)
ITC_SCENE_LOADERS(820D6108, SceneLoadersCockpitBindTextures)
ITC_SCENE_LOADERS(820D7D38, SceneLoadersTrafficBindTextures)
ITC_SCENE_LOADERS(820D02D0, SceneLoadersModelPointFind)
ITC_SCENE_LOADERS(820D6528, SceneLoadersCockpitOnFileLoaded)
ITC_SCENE_LOADERS(820D8430, SceneLoadersTrafficOnFileLoaded)
ITC_SCENE_LOADERS(820D7960, SceneLoadersInteriorDeletingDestruct)
ITC_SCENE_LOADERS(820C4488, SceneLoadersReleaseA)
ITC_SCENE_LOADERS(820D7790, SceneLoadersInteriorIsLoaded)
ITC_SCENE_LOADERS(820CE800, SceneLoadersNameMatches)
ITC_SCENE_LOADERS(820D09B8, SceneLoadersXprFindByName)
ITC_SCENE_LOADERS(820D6BC0, SceneLoadersPool4DeletingDestruct)
ITC_SCENE_LOADERS(820D95E8, SceneLoadersWheelsDeletingDestruct)
ITC_SCENE_LOADERS(820D5988, SceneLoadersCarModelDeletingDestruct)
ITC_SCENE_LOADERS(820D74C0, SceneLoadersCarPartsDeletingDestruct)
ITC_SCENE_LOADERS(820D6798, SceneLoadersCockpitDeletingDestruct)
ITC_SCENE_LOADERS(820D89B8, SceneLoadersTrafficDeletingDestruct)

// For the fallback originals (docs/missing-functions.md): they call these by
// address (no dispatch entry). Each runs its register step above (the plain
// C++, or the original where it declines), leaving r3 as the step does.
#define ITC_SCENE_LOADERS_DIRECT(address, step) \
  ITC_DIRECT_ENTRY(address) { hooks::step(ctx); }

ITC_SCENE_LOADERS_DIRECT(820D3B58, SceneLoadersCarModelLoadsReset)
ITC_SCENE_LOADERS_DIRECT(820D3DE0, SceneLoadersCarModelLoadsConstruct)
ITC_SCENE_LOADERS_DIRECT(820D3E40, SceneLoadersCarModelLoadsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D3E98, SceneLoadersCarModelReset)
ITC_SCENE_LOADERS_DIRECT(820D4750, SceneLoadersCarModelDestruct)
ITC_SCENE_LOADERS_DIRECT(820D5890, SceneLoadersCarModelConstruct)
ITC_SCENE_LOADERS_DIRECT(820D6010, SceneLoadersCockpitLoadsConstruct)
ITC_SCENE_LOADERS_DIRECT(820D6090, SceneLoadersCockpitLoadsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D6238, SceneLoadersCockpitRelease)
ITC_SCENE_LOADERS_DIRECT(820D63A0, SceneLoadersCockpitConstruct)
ITC_SCENE_LOADERS_DIRECT(820D64D0, SceneLoadersCockpitDestruct)
ITC_SCENE_LOADERS_DIRECT(820D6948, SceneLoadersPool4Reset)
ITC_SCENE_LOADERS_DIRECT(820D6A20, SceneLoadersPool4Release)
ITC_SCENE_LOADERS_DIRECT(820D6A70, SceneLoadersPool4Construct)
ITC_SCENE_LOADERS_DIRECT(820D6AD0, SceneLoadersPool4Destruct)
ITC_SCENE_LOADERS_DIRECT(820D6D00, SceneLoadersCarPartsRelease)
ITC_SCENE_LOADERS_DIRECT(820D6F88, SceneLoadersCarPartsLoadsConstruct)
ITC_SCENE_LOADERS_DIRECT(820D6FE8, SceneLoadersCarPartsLoadsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D7038, SceneLoadersCarPartsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D7428, SceneLoadersCarPartsConstruct)
ITC_SCENE_LOADERS_DIRECT(820D75F8, SceneLoadersInteriorRelease)
ITC_SCENE_LOADERS_DIRECT(820D76D8, SceneLoadersInteriorDestruct)
ITC_SCENE_LOADERS_DIRECT(820D78B8, SceneLoadersInteriorConstruct)
ITC_SCENE_LOADERS_DIRECT(820D7AB0, SceneLoadersTrafficReset)
ITC_SCENE_LOADERS_DIRECT(820D7B90, SceneLoadersTrafficRelease)
ITC_SCENE_LOADERS_DIRECT(820D7C40, SceneLoadersTrafficLoadsConstruct)
ITC_SCENE_LOADERS_DIRECT(820D7CC0, SceneLoadersTrafficLoadsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D7E90, SceneLoadersTrafficDestruct)
ITC_SCENE_LOADERS_DIRECT(820D8908, SceneLoadersTrafficConstruct)
ITC_SCENE_LOADERS_DIRECT(820D8AA8, SceneLoadersWheelsLoadsReset)
ITC_SCENE_LOADERS_DIRECT(820D8B58, SceneLoadersWheelsReset)
ITC_SCENE_LOADERS_DIRECT(820D8E68, SceneLoadersWheelsRelease)
ITC_SCENE_LOADERS_DIRECT(820D8F60, SceneLoadersWheelsLoadsConstruct)
ITC_SCENE_LOADERS_DIRECT(820D9010, SceneLoadersWheelsLoadsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D90B8, SceneLoadersWheelsDestruct)
ITC_SCENE_LOADERS_DIRECT(820D9570, SceneLoadersWheelsConstruct)
