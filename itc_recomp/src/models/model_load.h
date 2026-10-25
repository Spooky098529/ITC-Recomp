// The model's load as plain C++ (docs/functions/models.md: Model::Load,
// Mesh::Parse; the layout in docs/known-data.md): a part file parsed into
// the model (its shaders, its header block, its shapes' pointers, the
// skeleton and track sets, its vertex data in physical memory, the shapes'
// buffers relocated), its texture set made and handed to the texture
// provider, the skinned shapes' weights normalized. The model is guest data
// the recompiled code and the renderer share, so these work on guest memory
// with the originals' loads and stores in their order. Only over the
// allocation layer with the memory blocks' host lock (the adapter's
// condition). Built with strict floats (the weights). Adapter:
// src/hooks/models.cpp.

#pragma once

#include <cstdint>

namespace models {

// The guest calls the load makes where a callee's plain C++ hands back to
// its original: `init` VertexShader::Init's original (shader_objects'
// CreateCalls), `call` a function by its address with r3 to r5 (Skeleton::Bind,
// the track sets' set-ups, the texture provider's slot 4), from a frame as
// the caller's with `return_address`.
struct LoadCalls {
  void (*init)(void* user, uint32_t object, uint32_t microcode) = nullptr;
  void (*call)(void* user, uint32_t function, uint32_t r3, uint32_t r4, uint32_t r5,
               uint32_t return_address) = nullptr;
  void* user = nullptr;
};

// sub_825963F0 Mesh::Parse: `file` a part file (version 0x00150000; any other
// gets the original's debug message, logged). See models.md's row.
void MeshParse(uint8_t* base, uint32_t model, uint32_t file, const LoadCalls& calls);

// Whether MeshParse and ModelLoad run natively (checked before anything is
// done): memcpy's copies native (--native_memcpy), and each of the file's
// shader header copies XMemCpy makes natively.
bool LoadFits(uint8_t* base, uint32_t file);

// sub_82596920 Model::Load: Model::Release, Mesh::Parse, then with `xpr` (an
// XPR2 file or 0) XPR::Create on +56 and the texture provider's (+80) slot 4
// with slot 0 = +56, +48 = +80; then each skinned shape's (+428 = 1)
// weights quantized and the last of each vertex made 1 minus the others.
// False, with nothing done, where the original must run (Model::Release's,
// or LoadFits').
bool ModelLoad(uint8_t* base, uint32_t model, uint32_t file, uint32_t xpr,
               const LoadCalls& calls);

}  // namespace models
