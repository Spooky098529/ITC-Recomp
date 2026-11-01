// The vertex shader's fetch instructions patched for a vertex declaration,
// as the graphics library's sub_822AE340 does at the draw's shader load
// (see "Vertex declaration" in docs/gpu-functions.md), in plain C++. A
// Xenos vertex shader reads its own vertices: each vfetch names the fetch
// constant (the stream), the offset, the stride and the format, which the
// compiled shader doesn't know until a declaration is bound. The SDK's
// translator reads them from the microcode, so the conventional renderer
// (docs/conventional-renderer.md) patches a copy for each (shader,
// declaration, strides) it draws with.

#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace native {

// The library's tables in the image: a mask of the components each
// destination select keeps (0x82014224, 8 halfwords), and each vertex
// format's size in dwords (0x82014238, 64 bytes; probably).
struct VertexFetchTables {
  uint16_t select_masks[8];
  uint8_t format_sizes[64];
};
VertexFetchTables LoadVertexFetchTables(uint8_t* base);

// A declaration element (12 bytes from the declaration's +52).
struct DeclarationElement {
  uint16_t stream, offset;
  uint32_t type;
  uint8_t method, usage, usage_index;
};

// The vertex shader object's fetches: +60 their count, +64 a word each
// (bits 0-11 the instruction's index in the microcode, 12-15 the usage,
// 16-19 the usage index, 20-21 the group marks), +192 each one's template
// instruction (3 words).
struct VertexFetches {
  std::span<const uint32_t> info;
  std::span<const uint32_t> templates;
};

// Writes each fetch's patched instruction into `microcode` (big-endian
// words, as in guest memory; `size` bytes): its stream's fetch constant
// (95 - stream), offset, format and the stream's stride (`strides`: a byte
// a stream, the stride in dwords, as the device keeps them at +12272), then
// the marked groups sorted and joined into mini-fetches. A fetch with no
// element of its usage reads stream 0. Returns false (nothing written past
// `size`) if an instruction's index lies outside the microcode.
bool PatchVertexFetches(const VertexFetchTables& tables, const VertexFetches& fetches,
                        std::span<const DeclarationElement> elements, const uint8_t strides[16],
                        uint8_t* microcode, size_t size);

// The inputs read from the game's objects (a vertex shader object and a
// vertex declaration), for the guest objects the replay binds.
struct GuestVertexFetchInputs {
  uint32_t info[64];
  uint32_t templates[3 * 64];
  DeclarationElement elements[64];
  uint32_t fetch_count = 0, element_count = 0;
  VertexFetches fetches() const {
    return {{info, fetch_count}, {templates, 3 * size_t(fetch_count)}};
  }
  std::span<const DeclarationElement> declaration() const { return {elements, element_count}; }
};
// False when the counts are past what's kept (logged by the caller).
bool ReadGuestVertexFetchInputs(uint8_t* base, uint32_t shader, uint32_t declaration,
                                GuestVertexFetchInputs& out);

}  // namespace native
