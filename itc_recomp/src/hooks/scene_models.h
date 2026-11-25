// The scene models' register steps (src/scene_models/,
// docs/functions/scene-models.md), shared by the adapters
// (src/hooks/scene_models.cpp, behind --native_scene_models) and the harness
// (without the flag). Each leaves r3 as the original does, and runs the
// original where the plain C++ declines.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

namespace hooks {

// sub_820F0E50: r3 = the accessories' set.
void SceneModelsAccessories(lifted::Regs& ctx);
// SceneModel::Accessory sub_820D97B8: r3 = the interior's loading object; r3
// = the accessory or 0.
void SceneModelsAccessory(lifted::Regs& ctx);
// sub_82175EE0: r6 = the context (the slot); r3 left.
void SceneModelsOnSoundLoaded(lifted::Regs& ctx);
// sub_820E7378, sub_820E70B8, SceneParts::Link sub_820DF1A8, sub_820DF1C0,
// SceneInterior::Link sub_820E37D0, sub_820E3658: r3 = the object, r4 = the
// value or the car model; r3 left.
void SceneModelsWheelsSetRecord(lifted::Regs& ctx);
void SceneModelsSetCarModel(lifted::Regs& ctx);
void SceneModelsPartsLink(lifted::Regs& ctx);
void SceneModelsPartsSetRecord(lifted::Regs& ctx);
void SceneModelsInteriorLink(lifted::Regs& ctx);
void SceneModelsSetWord11452(lifted::Regs& ctx);
// CarModel::SetRecord sub_820D3C70: r3 = the car model, r4 = the record; r3
// = the model + 32 (memcpy's).
void SceneModelsCarModelSetRecord(lifted::Regs& ctx);
// CarRecord::Copy sub_820D7EE8: r3 = to, r4 = from; r3 = to.
void SceneModelsCarRecordCopy(lifted::Regs& ctx);
// SceneModelRecord::Copy sub_820DB290: r3 = to, r4 = from; r3 = to.
void SceneModelsSceneModelRecordCopy(lifted::Regs& ctx);
// The records' defaults: r3 = the record; r3 left (memset's return, the
// same), except CarRecord::Reset's (32).
void SceneModelsCarRecordSetW(lifted::Regs& ctx);
void SceneModelsRecord12Reset(lifted::Regs& ctx);
// sub_820D0A20: no adapter (only CarRecord::Reset calls it); the harness's
// steps.
void SceneModelsLayerReset(lifted::Regs& ctx);
void SceneModelsCarRecordReset(lifted::Regs& ctx);
// SceneModelRecord::SceneModelRecord sub_820DB240: r3 = the record (left).
void SceneModelsSceneModelRecordConstruct(lifted::Regs& ctx);
// sub_820D34B8: r3 = the record; r3 = the record + 24.
void SceneModelsCockpitRecordReset(lifted::Regs& ctx);
void SceneModelsPartsRecordReset(lifted::Regs& ctx);
void SceneModelsInteriorRecordReset(lifted::Regs& ctx);
void SceneModelsWheelsRecordReset(lifted::Regs& ctx);
void SceneModelsPool4RecordReset(lifted::Regs& ctx);
// SceneModelRecord::Reset sub_820D9810: r3 = the record; r3 = the record +
// 4192.
void SceneModelsSceneModelRecordReset(lifted::Regs& ctx);
// sub_820C3CA0: r4 = out, r5 = the size; r3 = the pages (VirtualAlloc's).
void SceneModelsAllocPages(lifted::Regs& ctx);
// The record re-applies (src/scene_models/apply.cpp), their callees through
// the guest (hooks::StandModelsGuestCall): SceneModel::ApplyRecord
// sub_820DA758 (r3 a scene model; the original for the race's path, the
// scene's +4 set), SceneModel::ReloadDecals sub_820D9D48 (r3 a scene model),
// and, without adapters (only ApplyRecord calls them; the harness's steps),
// SceneParts::Apply sub_820E08B0 (r3 the parts, r4 a record) and
// SceneInterior::Apply sub_820E4258 (r3 an interior, r4 a record). Each
// leaves r3 as the original does and lr put back.
void SceneModelsApplyRecord(lifted::Regs& ctx);
void SceneModelsReloadDecals(lifted::Regs& ctx);
void SceneModelsPartsApply(lifted::Regs& ctx);
void SceneModelsInteriorApply(lifted::Regs& ctx);

// SceneModel::Update sub_820DA180 (no adapter: CarScene::UpdateModels' plain
// C++ calls this step): r3 a scene model; Build's register step
// (with --native_scene_models) from its 240-byte frame, the accessory's
// branch (never seen in play) guest calls. r3 as the original leaves it.
void SceneModelUpdate(lifted::Regs& ctx);

}  // namespace hooks
