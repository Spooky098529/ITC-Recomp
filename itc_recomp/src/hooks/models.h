// The model class's adapters (hooks/models.cpp): what the harness shares.

#pragma once

#include "guest.h"

namespace hooks {

// Whether the memory blocks' lock is the host mutex (models/memory_blocks.h)
// for this run: --native_models as it was the first time this was asked
// (fixed for the run, so no user takes the game's critical section while
// another takes the mutex, even while a harness switches the flag).
bool ModelsHostLock();

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
