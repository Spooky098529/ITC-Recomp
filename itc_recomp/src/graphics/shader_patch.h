// The graphics library's vertex shader patcher as plain C++
// (sub_822AE340, docs/functions/functions.md): the shader's vertex fetch
// instructions rewritten for the bound vertex declaration. Our renderer
// translates the patched microcode, so the work is kept whole (nothing in it
// is a packet). Adapter: src/native/replay_hooks.cpp, inside its wrap, which
// keeps the patched copy for the renderer (--native_replay_commands).

#pragma once

#include <cstdint>

namespace graphics::shader_patch {

// sub_822AE340, in its 592-byte frame below `sp` (the fetch records are
// built at the frame's +96, and its first pass reads the halfword at the
// frame's +80, which the original never writes: whatever the stack holds
// there, as for the original). `shader` the vertex shader object: +60 the
// fetch count, +64 a word a fetch (the low 12 bits its instruction's index
// in the microcode, bits 12-15 and 16-19 its usage and usage index, bits
// 20-21 its group marks), +192 the fetch instructions (12 bytes each).
// `microcode` where the patched instructions go (12 x the index from the
// start); `declaration` the vertex declaration (+24 its element count, +52
// its 12-byte elements: +0 the stream, +2 the offset, +4 the type, +9 the
// usage, +10 the usage index); `streams` a byte a stream. For each fetch,
// the element with its usage and index is found and the instruction's
// stream, offset, format, stride and swizzle set from it (a fetch without
// one reads stream 95's first byte as a constant); the fetches of each group
// are then sorted and adjacent ones merged into one mini-fetch, as the
// original does, and the records copied over the microcode's instructions
// (memcpy's copy, 12 bytes each, through `copy`). Returns r3 as the original
// leaves it: the last instruction's address, or the shader with no fetches.
using Copy = void (*)(void* context, uint32_t destination, uint32_t source, uint32_t size);
uint32_t PatchVertexShader(uint8_t* base, uint32_t shader, uint32_t microcode,
                           uint32_t declaration, uint32_t streams, uint32_t sp, Copy copy,
                           void* context);

}  // namespace graphics::shader_patch
