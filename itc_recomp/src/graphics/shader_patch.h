// What the graphics library's vertex shader patcher (sub_822AE340,
// docs/functions/functions.md) makes of one vertex fetch for a vertex
// declaration, as plain C++ (docs/pc-renderer.md, step 2): the renderer
// takes each fetch's fields from the declaration and the stream strides
// with it, so the library's patch of the shader's microcode doesn't run.
//
// The patcher (its first loop) builds a fetch instruction a fetch from the
// shader file's and the declaration's element with the fetch's usage and
// usage index: the element's stream (fetch constant 95 - stream), offset,
// format, signedness and component order, and the stream's stride; a fetch
// without one reads stream 95's first byte as a constant. It then sorts
// each group of fetches and merges neighbors into mini-fetches, which only
// changes how the GPU fetches the same values: left out here.

#pragma once

#include <cstdint>

namespace graphics::shader_patch {

struct Fetch {
  uint32_t words[3];
};

// The fetch's instruction for the declaration. usage_word: the shader
// object's record for the fetch (its +64 + 4n: bits 12-15 and 16-19 the
// usage and usage index); original: the file's instruction (the object's
// +192 + 12n), as host words; declaration: the vertex declaration (+24 its
// element count, +52 its 12-byte elements: +0 the stream, +2 the offset,
// +4 the type, +9 the usage, +10 the usage index); strides: each stream's
// stride in words.
Fetch FetchFromDeclaration(uint32_t usage_word, const uint32_t original[3], uint32_t declaration,
                           const uint8_t strides[16]);

}  // namespace graphics::shader_patch
