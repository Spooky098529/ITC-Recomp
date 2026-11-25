// The model class's adapters (hooks/models.cpp): what the harness shares.

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"
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
  lifted::Regs& ctx;
  models::LoadCalls Calls();
};

// The register steps of the materials' and shared shaders' accessors
// (without the flag): Model::MaterialCount (sub_82594AD8: r3 = the model, r4
// = the shape, r5 = the group), Model::Material (sub_82594B20: r5 = the
// index, r6 = the group), the shared shaders (sub_825896D8, sub_82589700: r3
// = the pair). (The vertex declaration's, sub_82589728, are the harness's:
// its adapter is gone.)
void ModelsMaterialCount(lifted::Regs& ctx);
void ModelsMaterial(lifted::Regs& ctx);
void ModelsSharedVertexShader(lifted::Regs& ctx);
void ModelsSharedPixelShader(lifted::Regs& ctx);
// sub_825896C0 (r3, r4 the tables; r3 left).
void ModelsSetSharedShaders(lifted::Regs& ctx);

// A guest call of `function` (r3 the provider, r4 the slot, r5 the index;
// lr the caller's, r1 its frame) made natively when it's the texture
// provider's slot 3 (TextureProvider::Get) or slots 5 to 7 and
// models::CallGet or CallXprTexture answers: r3 as the adapter leaves it.
// False where the caller makes the guest call.
bool TextureProviderSlotDirect(lifted::Regs& ctx, uint32_t function);

}  // namespace hooks
