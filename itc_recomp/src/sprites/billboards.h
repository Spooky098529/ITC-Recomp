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

}  // namespace sprites
