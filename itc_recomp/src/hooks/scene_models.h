// The scene models' register steps (src/scene_models/,
// docs/functions/scene-models.md), shared by the adapters
// (src/hooks/scene_models.cpp, behind --native_scene_models) and the harness
// (without the flag). Each leaves r3 as the original does, and runs the
// original where the plain C++ declines.

#pragma once

#include "guest.h"

namespace hooks {

// sub_820F0E50: r3 = the accessories' set.
void SceneModelsAccessories(PPCContext& ctx, uint8_t* base);
// sub_82175EE0: r6 = the context (the slot); r3 left.
void SceneModelsOnSoundLoaded(PPCContext& ctx, uint8_t* base);
// sub_820E7378, sub_820E70B8, SceneParts::Link sub_820DF1A8, sub_820DF1C0,
// SceneInterior::Link sub_820E37D0, sub_820E3658: r3 = the object, r4 = the
// value or the car model; r3 left.
void SceneModelsWheelsSetRecord(PPCContext& ctx, uint8_t* base);
void SceneModelsSetCarModel(PPCContext& ctx, uint8_t* base);
void SceneModelsPartsLink(PPCContext& ctx, uint8_t* base);
void SceneModelsPartsSetRecord(PPCContext& ctx, uint8_t* base);
void SceneModelsInteriorLink(PPCContext& ctx, uint8_t* base);
void SceneModelsSetWord11452(PPCContext& ctx, uint8_t* base);
// CarModel::SetRecord sub_820D3C70: r3 = the car model, r4 = the record; r3
// = the model + 32 (memcpy's).
void SceneModelsCarModelSetRecord(PPCContext& ctx, uint8_t* base);
// CarRecord::Copy sub_820D7EE8: r3 = to, r4 = from; r3 = to.
void SceneModelsCarRecordCopy(PPCContext& ctx, uint8_t* base);
// The records' defaults: r3 = the record; r3 left (memset's return, the
// same), except CarRecord::Reset's (32).
void SceneModelsCarRecordSetW(PPCContext& ctx, uint8_t* base);
void SceneModelsRecord12Reset(PPCContext& ctx, uint8_t* base);
// sub_820D0A20: no adapter (only CarRecord::Reset calls it); the harness's
// steps.
void SceneModelsLayerReset(PPCContext& ctx, uint8_t* base);
void SceneModelsCarRecordReset(PPCContext& ctx, uint8_t* base);
void SceneModelsPartsRecordReset(PPCContext& ctx, uint8_t* base);
void SceneModelsInteriorRecordReset(PPCContext& ctx, uint8_t* base);
void SceneModelsWheelsRecordReset(PPCContext& ctx, uint8_t* base);
void SceneModelsPool4RecordReset(PPCContext& ctx, uint8_t* base);
// sub_820C3CA0: r4 = out, r5 = the size; r3 = the pages (VirtualAlloc's).
void SceneModelsAllocPages(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
