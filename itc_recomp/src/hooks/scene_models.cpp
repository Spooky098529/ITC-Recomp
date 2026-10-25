// The scene models' parts natively: adapters in front of src/scene_models/
// (docs/functions/scene-models.md), with the register steps the harness
// shares (hooks/scene_models.h). Each leaves r3 as the original does.

#include "hooks/scene_models.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "scene_models/scene_models.h"

ITC_FLAG_BOOL(native_scene_models, true, "ITC",
              "Run the scene models' parts (docs/functions/scene-models.md: sub_820F0E50 (the "
              "accessories' set), the sound loads' callback sub_82175EE0, sub_820E7378, "
              "sub_820E70B8, SceneParts::Link sub_820DF1A8, sub_820DF1C0, SceneInterior::Link "
              "sub_820E37D0, sub_820E3658, CarModel::SetRecord sub_820D3C70, CarRecord::Copy "
              "sub_820D7EE8, the records' defaults sub_820D45B0, sub_820D3B38, CarRecord::Reset "
              "sub_820D0A98 with sub_820D0A20 (no adapter: only CarRecord::Reset calls it), sub_820D3558, sub_820D3840, sub_820D3900 and "
              "sub_820D34E0, SceneModel::Accessory sub_820D97B8, SceneModelRecord::Copy sub_820DB290, "
              "SceneModelRecord::SceneModelRecord sub_820DB240, sub_820D34B8, and with "
              "--native_heap the car model's pages sub_820C3CA0) natively "
              "instead of through the translated code (the same writes; checked by "
              "--run_harnesses=scene_models)");

namespace hooks {

namespace sm = scene_models;

void SceneModelsAccessories(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sm::Accessories(base);
}

void SceneModelsAccessory(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sm::Accessory(base, ctx.r3.u32);
}

void SceneModelsOnSoundLoaded(PPCContext& ctx, uint8_t* base) {
  sm::OnSoundLoaded(base, ctx.r6.u32);
}

void SceneModelsWheelsSetRecord(PPCContext& ctx, uint8_t* base) {
  sm::WheelsSetRecord(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneModelsSetCarModel(PPCContext& ctx, uint8_t* base) {
  sm::SetCarModel(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneModelsPartsLink(PPCContext& ctx, uint8_t* base) {
  sm::PartsLink(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneModelsPartsSetRecord(PPCContext& ctx, uint8_t* base) {
  sm::PartsSetRecord(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneModelsInteriorLink(PPCContext& ctx, uint8_t* base) {
  sm::InteriorLink(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneModelsSetWord11452(PPCContext& ctx, uint8_t* base) {
  sm::SetWord11452(base, ctx.r3.u32, ctx.r4.u32);
}

void SceneModelsCarModelSetRecord(PPCContext& ctx, uint8_t* base) {
  if (!sm::CarModelSetRecord(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820D3C70(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 32;
}

void SceneModelsCarRecordCopy(PPCContext& ctx, uint8_t* base) {
  if (!sm::CarRecordCopy(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820D7EE8(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ctx.r3.u32;
}

void SceneModelsSceneModelRecordCopy(PPCContext& ctx, uint8_t* base) {
  if (!sm::SceneModelRecordCopy(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820DB290(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = ctx.r3.u32;
}

void SceneModelsSceneModelRecordConstruct(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sm::SceneModelRecordConstruct(base, ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u32;
}

void SceneModelsCockpitRecordReset(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sm::CockpitRecordReset(base, ctx.r3.u32);
}

void SceneModelsCarRecordSetW(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sm::CarRecordSetW(base, ctx.r3.u32);
}

void SceneModelsRecord12Reset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sm::Record12Reset(base, ctx.r3.u32);
}

void SceneModelsLayerReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sm::LayerReset(base, ctx.r3.u32);
}

void SceneModelsCarRecordReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = sm::CarRecordReset(base, ctx.r3.u32);
}

void SceneModelsPartsRecordReset(PPCContext& ctx, uint8_t* base) {
  sm::PartsRecordReset(base, ctx.r3.u32);
}

void SceneModelsInteriorRecordReset(PPCContext& ctx, uint8_t* base) {
  sm::InteriorRecordReset(base, ctx.r3.u32);
}

void SceneModelsWheelsRecordReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sm::WheelsRecordReset(base, ctx.r3.u32);
}

void SceneModelsPool4RecordReset(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  sm::Pool4RecordReset(base, ctx.r3.u32);
}

void SceneModelsAllocPages(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = sm::AllocPages(base, ctx.r4.u32, ctx.r5.u32);
}

}  // namespace hooks

// Defines an adapter: the flag, else the original.
#define ITC_SCENE_MODELS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_scene_models)) {   \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

ITC_SCENE_MODELS(820F0E50, SceneModelsAccessories)
ITC_SCENE_MODELS(82175EE0, SceneModelsOnSoundLoaded)
ITC_SCENE_MODELS(820E7378, SceneModelsWheelsSetRecord)
ITC_SCENE_MODELS(820E70B8, SceneModelsSetCarModel)
ITC_SCENE_MODELS(820DF1A8, SceneModelsPartsLink)
ITC_SCENE_MODELS(820DF1C0, SceneModelsPartsSetRecord)
ITC_SCENE_MODELS(820E37D0, SceneModelsInteriorLink)
ITC_SCENE_MODELS(820E3658, SceneModelsSetWord11452)
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
ITC_SCENE_MODELS(820D97B8, SceneModelsAccessory)
ITC_SCENE_MODELS(820DB290, SceneModelsSceneModelRecordCopy)
ITC_SCENE_MODELS(820DB240, SceneModelsSceneModelRecordConstruct)
ITC_SCENE_MODELS(820D34B8, SceneModelsCockpitRecordReset)
ITC_SCENE_MODELS(820D3900, SceneModelsWheelsRecordReset)
ITC_SCENE_MODELS(820D34E0, SceneModelsPool4RecordReset)

// The car model's pages: only while --native_heap routes VirtualAlloc to the
// allocation layer.
extern "C" REX_FUNC(sub_820C3CA0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_scene_models) || !hooks::NativeHeapActive()) {
    __imp__sub_820C3CA0(ctx, base);
    return;
  }
  hooks::SceneModelsAllocPages(ctx, base);
}
