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
#include <optional>

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

// sub_822A9FD8 VertexShader::CopyHeader and sub_822A9EA8
// PixelShader::CopyHeader (probably): `record` is ParseFile's; the file's
// header size (the file's +4) read, the object's 592 (or 56) bytes cleared,
// then the file's header copied after them (XMemCpy, sub_82295890). False,
// with nothing written, where the original must run: the record inside the
// cleared bytes, or a copy XMemCpy hands to the original.
bool CopyVertexHeader(uint8_t* base, uint32_t object, uint32_t record);
bool CopyPixelHeader(uint8_t* base, uint32_t object, uint32_t record);

// What the creates call back where the native path hands back to the
// original: VertexShader::Init's original (sub_822A8740, where
// VertexShaderInit declines).
struct CreateCalls {
  void (*init)(void* user, uint32_t object, uint32_t microcode) = nullptr;
  void* user = nullptr;
};

// The attributes the creates allocate with (XMemAlloc, sub_822976F8): the
// object from the heap, the microcode in physical memory.
constexpr uint32_t kObjectAttributes = 0x24800000;
constexpr uint32_t kMicrocodeAttributes = 0xB5800000;
// The flag the creates set in the object's +0 (as the library's creates do).
constexpr uint32_t kCreatedFlag = 0x00100000;

// sub_822AA018 VertexShader::Create: the object XMemAlloc(56 or 592 (by the
// magic's low 6 bits) + the header size, kObjectAttributes), its 592 bytes
// cleared and the header copied after them (XMemCpy); the microcode (the
// file's +8 bytes from the file + the header size) into XMemAlloc(size,
// kMicrocodeAttributes) through memcpy (sub_825A7410, recorded for the
// renderer: native::Buffers_Memcpy), the object freed (XMemFree) and 0
// returned when that fails; then VertexShader::Init, +0 |= kCreatedFlag, the
// object returned (0 when its allocation fails). nullopt, with nothing done,
// where the original must run: a header copy XMemCpy hands to the original,
// or memcpy's copy that wouldn't be native (--native_memcpy off). Only over
// the allocation layer (the adapter's condition).
std::optional<uint32_t> VertexShaderCreate(uint8_t* base, uint32_t file, const CreateCalls& calls);

// sub_822A9EE8 PixelShader::Create: as VertexShaderCreate with 56 bytes
// cleared, the header after them, and the fields set inline as
// PixelShaderInit's (the 56 bytes cleared again, +24 = the microcode, +0 =
// 7, +4 = 1, +20 = 0xFFFF0000), +0 |= kCreatedFlag.
std::optional<uint32_t> PixelShaderCreate(uint8_t* base, uint32_t file);

// sub_82594D20 Mesh::LoadShaders (models.md): `section` = the model file's
// shader section: +0 the pair count (to the model's +420); none, nothing
// more. Else the pairs' block (16 bytes a pair) from Memory::AllocVirtual to
// the model's +24, and from +32 a pair after another (+4 the vertex file's
// size, +8 the pixel file's, the vertex file at +32, the pixel file after
// it, the next pair after that). Each shader: its file parsed, its object
// from Memory::AllocVirtual (the pair's +0, +4), its header copied after the
// fields, its microcode into Memory::AllocPhysicalEx(size, 0, -1, 32, 0x404)
// (the pair's +8, +12) through memcpy (recorded for the renderer:
// native::Buffers_Memcpy), then its Init. A pair whose microcode block is 0
// gets the original's debug messages (logged; never seen: the layer aborts
// when out of memory). False, with nothing done, where the original must
// run: memcpy's copy that wouldn't be native (--native_memcpy off), or a
// header copy XMemCpy hands to the original. Only over the allocation layer
// with the memory blocks' host lock (the adapter's condition). Where
// VertexShaderInit declines, `calls.init` (the original).
bool LoadShaders(uint8_t* base, uint32_t model, uint32_t section, const CreateCalls& calls);

}  // namespace shader_objects
