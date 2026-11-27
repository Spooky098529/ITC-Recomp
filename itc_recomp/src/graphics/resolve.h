// Device::Resolve (sub_822AFA58, docs/functions/functions.md) as plain C++
// over the device in guest memory: a render target (or the depth buffer)
// copied into a texture, with an optional clear. Our renderer does the copy
// from the wraps' calls (command 25's items, command 33's tiles, the front
// buffer's resolve), so what's kept is what the game and our renderer read
// back from the device and the texture: the copy registers in the register
// shadow (+10648 to +10660, +10692) and the clear color and depth words
// (sub_822AF438, sub_822AF8A0, lifted), the dirty masks' flush with the
// native renderer's marks the dropped packet writers' overrides make
// (src/native/draw_packets.cpp) and the masks set after it, the flags
// +10809 and +10810, the default draw registers' marks
// (Device::WriteDefaultDrawRegisters), the segment ends where the original
// checks for them, the data space it reserves (its out-of-space return
// kept), the destination texture's fences (+8, +12) and the fence list
// (+13168, its flush sub_822AD790). Dropped (the graphics rule): its GPU
// packets (the copy's draw, the EDRAM copy controls, the cache events, the
// predicated tiling's per-tile copies, the 60 bytes of fixed registers it
// memcpys in), the six floats of the copy's rectangle written into the
// data space, and Device::SetScreenScissor (sub_822A0F80, already Dropped:
// only packets); the write position +40 is left where it was, as the
// dropped writers leave it. The adapter is the wrap in
// src/hooks/replay_commands.cpp (--native_resolve).
#pragma once

#include <cstdint>

namespace graphics::resolve {

// r3 device, r4 flags, r5 the source rectangle (0: the whole target), r6
// the destination texture, r7 the destination point (0: 0x8204A9F4's), r8
// the level, r9 the slice, r10 the clear color (4 floats, 0: zeros), f1 the
// clear depth; the stack's +92 and +100 (sp the caller's frame) the clear
// helpers' arguments. r3 not kept (no caller reads it).
void Resolve(uint32_t sp, uint64_t r13, uint32_t device, uint32_t flags,
             uint32_t rect, uint32_t texture, uint32_t point, uint32_t level, uint32_t slice,
             uint32_t color, double clear_depth);

}  // namespace graphics::resolve
