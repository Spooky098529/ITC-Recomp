// The light glows' billboards (the light groups) as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/sprites.md).
// The material and its lights live on the guest stack and in level data, and
// the vertices go to the game's vertex ring, all read by recompiled code, so
// these work on guest memory (level 1 of "The boundary rule"). Adapters:
// src/hooks/billboards.cpp, and the vertex builder's in
// src/interpolation/blend_state.cpp (its camera blend around it).

#pragma once

#include <cstdint>

namespace sprites {

// BillboardMaterial::Init (sub_8209F5A0): alpha mode +0 = 1, blend mode
// +4 = 1, +8, +12 (texture), +16, +32 (light count) and +36 (lights) = 0.
void InitBillboardMaterial(uint8_t* base, uint32_t material);

// BillboardMaterial::SetLights (sub_8209F5C8): +32 = count, +36 = lights,
// unless count is 0.
void SetBillboardLights(uint8_t* base, uint32_t material, uint32_t count, uint32_t lights);

// Allocates `count` 16-byte slots on the command list being built; returns
// the first (0 when the list is full). `return_address` is the original
// call's, for the allocator's diagnostics.
struct SlotAllocator {
  uint32_t (*allocate)(void* user, uint32_t count, uint32_t return_address);
  void* user;
  uint32_t operator()(uint32_t count, uint32_t return_address) const {
    return allocate(user, count, return_address);
  }
};

// Billboards::BuildVertices (sub_8209C6E0): for the material's lights (+32
// count, 48 bytes each at +36: position +0, color +16-+28 as floats 0-255
// in r, g, b, a order, width +32, height +36), a camera-facing quad of four
// 28-byte vertices (position, 1.0, ARGB, u, v) in the game's vertex ring,
// for each light with a size, within the camera pass's draw distance (+548,
// capped) and in front of its eye (+400, looking at +416, flattened in y),
// while the ring's count (0x826A509C) leaves room. Records a command 10 and
// the ring's handle, then, when anything is counted, a command 19
// (DrawPrimitiveUP: type 13 for alpha modes 2-2048 that are powers of two,
// with 4 a light, else type 2 with 1 a light; minus 4 per light skipped),
// adds that count to the ring's and returns it. `set` + 0 is the camera
// pass. The vector steps run flushing denormals to zero, as the
// translated code's do.
uint32_t BuildBillboardVertices(uint8_t* base, uint32_t set, uint32_t material,
                                const SlotAllocator& slots);

}  // namespace sprites
