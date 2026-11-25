// The world-space sprites' recorders (docs/functions/sprites.md) as plain
// C++: a single sprite (the marker above opponents) and a sprite list (the
// light trails), each recorded as quads into the command list at
// [0x826C5D60] and handed to the render queue at [0x826C5D5C]. Their command
// list calls (OpenChunk, AllocateSlots, the constants and vertices'
// allocations, CloseChunk), the matrix product and the queue go through
// the dispatch table (hooks/lifted_call.h), so their overrides and wraps
// (frame interpolation's AllocateSlots) still run. Render-path code:
// checked by frame dumps. Their wraps in src/frame_pacing/per_frame_logic.cpp
// (the corners blended between ticks) stay.

#pragma once

#include <cstdint>

namespace sprites {

// Sprite::Record (sub_821281A8): r3 = the sprite (+0 its matrix, +68 and +72
// the blend and depth kinds, +80 its four corners, +144 the corners' colors,
// +208 to +220 the texture coordinates, +224 a state word), r4 = the camera
// pass (+144 its view-projection), r5 = the render queue's layer. From its
// 272-byte frame below `sp`; returns the queue call's r3.
uint64_t SpriteRecord(uint32_t sp, uint64_t r13, uint32_t sprite, uint32_t camera,
                      uint32_t layer);

// SpriteList::Record (sub_82128750): r3 = the list (as a sprite, with up to
// 500 sprites: count at +0x109F0, the state word at +0x109F4, corners at +80
// + 64i, colors at +32080 + 64i, texture coordinate choices at +64080 + 4i
// and +66080 + 4i), r4 = the camera pass, r5 = the layer. From its 368-byte
// frame below `sp`; returns the queue call's r3.
uint64_t SpriteListRecord(uint32_t sp, uint64_t r13, uint32_t list,
                          uint32_t camera, uint32_t layer);

}  // namespace sprites
