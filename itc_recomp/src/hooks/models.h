// The model class's adapters (hooks/models.cpp): what the harness shares.

#pragma once

#include "guest.h"
#include "models/model_load.h"

namespace hooks {

// Whether the memory blocks' lock is the host mutex (models/memory_blocks.h)
// for this run: --native_models as it was the first time this was asked
// (fixed for the run, so no user takes the game's critical section while
// another takes the mutex, even while a harness switches the flag).
bool ModelsHostLock();

// Whether the model's functions that allocate or free run natively:
// --native_models, over the allocation layer, with the host lock. Native
// callers of Model::Model and Model::Load check it as their adapters do.
bool ModelsNativeLifetime();

// The guest calls Model::Load's plain C++ makes where a callee's declines,
// each from a 512-byte frame below r1 (room for the original's own frames
// and a native caller's).
struct ModelLoadGuest {
  PPCContext& ctx;
  uint8_t* base;
  models::LoadCalls Calls();
};

// The register steps of the materials' and shared shaders' accessors
// (without the flag): Model::MaterialCount (sub_82594AD8: r3 = the model, r4
// = the shape, r5 = the group), Model::Material (sub_82594B20: r5 = the
// index, r6 = the group), the shared shaders (sub_825896D8, sub_82589700: r3
// = the pair) and the vertex declaration (sub_82589728: r3 to r5 the stream
// types; the original where the plain C++ declines).
void ModelsMaterialCount(PPCContext& ctx, uint8_t* base);
void ModelsMaterial(PPCContext& ctx, uint8_t* base);
void ModelsSharedVertexShader(PPCContext& ctx, uint8_t* base);
void ModelsSharedPixelShader(PPCContext& ctx, uint8_t* base);
void ModelsVertexDeclaration(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
