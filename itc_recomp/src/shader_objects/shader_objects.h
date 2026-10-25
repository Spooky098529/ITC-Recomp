// The graphics library's shader objects as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/shader-objects.md and models.md, the shader file in
// docs/known-data.md): a shader file parsed, its object's fields set. The
// objects are guest data the renderer reads to find a draw's shaders, so
// these work on guest memory with the originals' stores in their order.
// Adapters: src/hooks/shader_objects.cpp (and src/hooks/models.cpp for the
// file parse, a models.md row).

#pragma once

#include <cstdint>

namespace shader_objects {

// A vertex shader object's fields (592 bytes, its file header copied after
// them) and a pixel shader's (56).
constexpr uint32_t kVertexShaderBytes = 592;
constexpr uint32_t kPixelShaderBytes = 56;

// sub_822A8568 ShaderFile::Parse (probably): `record` (16 bytes) = the file,
// the object's size (592 when the magic's low 6 bits are set, else 56) plus
// the header size (the file's +4), the microcode (the file + the header
// size) and its size (the file's +8). The record is cleared first.
void ParseFile(uint8_t* base, uint32_t file, uint32_t record);

// sub_822A8740 VertexShader::Init: the 592 bytes cleared, +0 = 6, +4 = 1,
// +20 = 0xFFFF0000, +24 = -1, +48 = the microcode; the header's fetch table
// (h = the object + 592 + [object + 612]): +60 = h's +28 (the count), and
// for each word i at h + 4 x (h's +24 + 10), +64 + 4i = the word and the 12
// bytes at the microcode + 12 x (its low 12 bits) copied to +192 + 12i
// (memcpy, sub_825A7410). False, with nothing written, where the original
// must run: an object in the physical views (memcpy's override records
// copies there for the renderer), a count over 32, or a table or a copy
// whose bytes may share memory with the fields (the original's order would
// then decide).
bool VertexShaderInit(uint8_t* base, uint32_t object, uint32_t microcode);

// sub_822A85B8 PixelShader::Init: the 56 bytes cleared, +0 = 7, +4 = 1,
// +20 = 0xFFFF0000, +24 = the microcode.
void PixelShaderInit(uint8_t* base, uint32_t object, uint32_t microcode);

}  // namespace shader_objects
