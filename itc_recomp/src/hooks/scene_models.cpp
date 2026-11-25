// The scene models' parts natively: adapters in front of src/scene_models/
// (docs/functions/scene-models.md), with the register steps the harness
// shares (hooks/scene_models.h). Each leaves r3 as the original does.

#include "lifted/originals.h"
#include "hooks/scene_models.h"
#include "hooks/native_entry.h"

#include "base/flags.h"
#include "hooks/car_models.h"
#include "hooks/heap.h"
#include "hooks/stand_models.h"
#include "scene_models/apply.h"
#include "scene_models/scene_models.h"

ITC_FLAG_BOOL(native_scene_models, true, "ITC",
              "Run the scene models' parts (docs/functions/scene-models.md: sub_820F0E50 (the "
              "accessories' set), the sound loads' callback sub_82175EE0, sub_820E7378 (no adapter), "
              "sub_820E70B8, SceneParts::Link sub_820DF1A8, sub_820DF1C0, SceneInterior::Link "
              "sub_820E37D0, sub_820E3658 (no adapter), CarModel::SetRecord sub_820D3C70, CarRecord::Copy "
              "sub_820D7EE8, the records' defaults sub_820D45B0, sub_820D3B38, CarRecord::Reset "
              "sub_820D0A98 with sub_820D0A20 (no adapter: only CarRecord::Reset calls it), sub_820D3558, sub_820D3840, sub_820D3900 and "
              "sub_820D34E0, SceneModel::Accessory sub_820D97B8, SceneModel::Update sub_820DA180 (no adapter: from "
              "CarScene::Update's plain C++ under --native_scene; Build's step, the accessory's branch, never seen in play, guest calls), SceneModelRecord::Copy sub_820DB290, "
              "SceneModelRecord::SceneModelRecord sub_820DB240, sub_820D34B8, SceneModelRecord::Reset sub_820D9810, SceneModel::ApplyRecord sub_820DA758 (the original for the race's path) with SceneParts::Apply sub_820E08B0 and SceneInterior::Apply sub_820E4258, SceneModel::ReloadDecals sub_820D9D48 (these four without adapters: their native callers call them; their callees through their adapters; checked by --run_harnesses=load_poll and car_models), and with "
              "--native_heap the car model's pages sub_820C3CA0; in src/hooks/car_models.cpp, the second "
              "set's set-ups and ScenePools::SetUpB sub_820C5B28 (its pool 0 case sub_820C5B98 "
              "merged) with SceneCockpit::SetUp sub_820DBE80 and SceneModel::SetUp sub_820DB458 (no "
              "adapters: only SetUpB calls them) and SceneModel::Build sub_820D9870, checked by "
              "--run_harnesses=car_models) natively "
              "instead of through the translated code (the same writes; checked by "
              "--run_harnesses=scene_models)");

namespace hooks {

namespace sm = scene_models;

ITC_REGS_STEP(SceneModelsAccessories) {
  ctx.r3.u64 = sm::Accessories();
}

ITC_REGS_STEP(SceneModelsAccessory) {
  ctx.r3.u64 = sm::Accessory(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsOnSoundLoaded) {
  sm::OnSoundLoaded(ctx.r6.u32);
}

ITC_REGS_STEP(SceneModelsWheelsSetRecord) {
  sm::WheelsSetRecord(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneModelsSetCarModel) {
  sm::SetCarModel(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneModelsPartsLink) {
  sm::PartsLink(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneModelsPartsSetRecord) {
  sm::PartsSetRecord(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneModelsInteriorLink) {
  sm::InteriorLink(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneModelsSetWord11452) {
  sm::SetWord11452(ctx.r3.u32, ctx.r4.u32);
}

ITC_REGS_STEP(SceneModelsCarModelSetRecord) {
  if (!sm::CarModelSetRecord(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820D3C70(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 32;
}

ITC_REGS_STEP(SceneModelsCarRecordCopy) {
  if (!sm::CarRecordCopy(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820D7EE8(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ctx.r3.u32;
}

ITC_REGS_STEP(SceneModelsSceneModelRecordCopy) {
  if (!sm::SceneModelRecordCopy(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820DB290(ctx);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ctx.r3.u32;
}

ITC_REGS_STEP(SceneModelsSceneModelRecordConstruct) {
  ctx.fpscr.disableFlushMode();
  sm::SceneModelRecordConstruct(ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u32;
}

ITC_REGS_STEP(SceneModelsCockpitRecordReset) {
  ctx.r3.u64 = sm::CockpitRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsCarRecordSetW) {
  ctx.fpscr.disableFlushMode();
  sm::CarRecordSetW(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsRecord12Reset) {
  ctx.fpscr.disableFlushMode();
  sm::Record12Reset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsLayerReset) {
  ctx.fpscr.disableFlushMode();
  sm::LayerReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsCarRecordReset) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sm::CarRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsPartsRecordReset) {
  sm::PartsRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsInteriorRecordReset) {
  sm::InteriorRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsWheelsRecordReset) {
  ctx.fpscr.disableFlushMode();
  sm::WheelsRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsPool4RecordReset) {
  ctx.fpscr.disableFlushMode();
  sm::Pool4RecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsSceneModelRecordReset) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sm::SceneModelRecordReset(ctx.r3.u32);
}

ITC_REGS_STEP(SceneModelsAllocPages) {
  ctx.r3.u64 = sm::AllocPages(ctx.r4.u32, ctx.r5.u32);
}

ITC_REGS_STEP(SceneModelsApplyRecord) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::ApplyCall call = StandModelsGuestCall(ctx);
  const auto r3 = sm::SceneModelApplyRecord(call, ctx.r3.u32, ctx.r1.u32);
  ctx.lr = lr;
  if (!r3) {
    lifted::original::c820DA758(ctx);
    return;
  }
  ctx.r3.u64 = *r3;
}

ITC_REGS_STEP(SceneModelsReloadDecals) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::ApplyCall call = StandModelsGuestCall(ctx);
  const uint64_t r3 = sm::SceneModelReloadDecals(call, ctx.r3.u32, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(SceneModelsPartsApply) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::ApplyCall call = StandModelsGuestCall(ctx);
  const uint64_t r3 = sm::ScenePartsApply(call, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}

ITC_REGS_STEP(SceneModelsInteriorApply) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  const sm::ApplyCall call = StandModelsGuestCall(ctx);
  const uint64_t r3 = sm::SceneInteriorApply(call, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32);
  ctx.lr = lr;
  ctx.r3.u64 = r3;
}


ITC_REGS_STEP(SceneModelUpdate) {
  const uint64_t lr = ctx.lr;
  const auto call = StandModelsGuestCall(ctx);
  sm::ModelUpdateCalls calls;
  calls.build = [&](uint32_t model, uint32_t frame) -> uint64_t {
    // SceneModel::Build's register step (its own fallbacks), from the frame.
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = frame;
    ctx.r3.u64 = model;
    ctx.lr = 0x820DA1C4;
    if (ITC_FLAG(native_scene_models)) {
      SceneModelBuild(ctx);
    } else {
      lifted::original::c820D9870(ctx);
    }
    ctx.r1.u64 = stack;
    return ctx.r3.u64;
  };
  // The accessory's branch, never seen in play: guest calls, Matrix::Multiply
  // through its override (frame interpolation's note), SetAnchor and Step
  // the originals.
  calls.multiply = [&](uint32_t out, uint32_t a, uint32_t b, uint32_t frame) {
    call(0x8209BD50, {out, a, b}, frame, 0x820DA2E8);
  };
  calls.set_anchor = [&](uint32_t accessory, uint32_t matrix, uint32_t frame) {
    call(0x820EF418, {accessory, matrix}, frame, 0x820DA2F4);
  };
  calls.step = [&](uint32_t accessory, uint32_t frame) {
    return call(0x820EF250, {accessory, 0}, frame, 0x820DA300);
  };
  const uint64_t r3_in = ctx.r3.u64;
  const auto r3 = sm::ModelUpdate(ctx.r3.u32, ctx.r1.u32, calls);
  ctx.lr = lr;
  ctx.r3.u64 = r3.value_or(r3_in);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_SCENE_MODELS(address, step)     \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_scene_models)) { \
    hooks::step(ctx);                 \
  }

ITC_SCENE_MODELS(82175EE0, SceneModelsOnSoundLoaded)
ITC_SCENE_MODELS(820E70B8, SceneModelsSetCarModel)
ITC_SCENE_MODELS(820DF1A8, SceneModelsPartsLink)
ITC_SCENE_MODELS(820DF1C0, SceneModelsPartsSetRecord)
ITC_SCENE_MODELS(820E37D0, SceneModelsInteriorLink)
ITC_SCENE_MODELS(820D3C70, SceneModelsCarModelSetRecord)
ITC_SCENE_MODELS(820D7EE8, SceneModelsCarRecordCopy)
ITC_SCENE_MODELS(820D45B0, SceneModelsCarRecordSetW)
ITC_SCENE_MODELS(820D3B38, SceneModelsRecord12Reset)
ITC_SCENE_MODELS(820D0A98, SceneModelsCarRecordReset)
// sub_820D0A20 has no adapter: only CarRecord::Reset calls it, and its plain
// C++ calls LayerReset's (SceneModelsLayerReset is the harness's register
// steps).
ITC_SCENE_MODELS(820D3558, SceneModelsPartsRecordReset)
ITC_SCENE_MODELS(820D3840, SceneModelsInteriorRecordReset)
ITC_SCENE_MODELS(820DB290, SceneModelsSceneModelRecordCopy)
ITC_SCENE_MODELS(820DB240, SceneModelsSceneModelRecordConstruct)
ITC_SCENE_MODELS(820D34B8, SceneModelsCockpitRecordReset)
ITC_SCENE_MODELS(820D9810, SceneModelsSceneModelRecordReset)
ITC_SCENE_MODELS(820D3900, SceneModelsWheelsRecordReset)
ITC_SCENE_MODELS(820D34E0, SceneModelsPool4RecordReset)
// SceneModel::ApplyRecord sub_820DA758 and ReloadDecals sub_820D9D48 have
// no adapters (removed, B7 loop, 2026-11-06): their callers in play (the
// garage stage's re-applies and setters, car_makers and stand_models) are
// native and call their plain C++, the race's path of ApplyRecord the
// original; their register steps above are the harness's.
// sub_820F0E50 (the accessories' set) and SceneModel::Accessory sub_820D97B8
// have no adapters (removed, B7 loop, 2026-11-06): their callers in play are
// native and call their plain C++.
// SceneModel::Update sub_820DA180 has no adapter: only sub_820C5A60 calls
// it, whose plain C++ (src/scene/scene.cpp, under CarScene::Update's) calls
// its register step above.
// SceneParts::Apply sub_820E08B0 and SceneInterior::Apply sub_820E4258 have
// no adapters: only SceneModel::ApplyRecord calls them, and its plain C++
// calls theirs (their register steps above are the harness's).

// The car model's pages: only while --native_heap routes VirtualAlloc to the
// allocation layer.
ITC_NATIVE_ENTRY_ALWAYS(820C3CA0) {
  
  if (!ITC_FLAG(native_scene_models) || !hooks::NativeHeapActive()) {
    lifted::original::c820C3CA0(ctx);
    return;
  }
  hooks::SceneModelsAllocPages(ctx);
}
