// The race overlays' camera-facing sprites as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/sprites.md).
// The sprite lists live on the overlays' guest stacks and are read by
// recompiled recorders, so these work on guest memory (level 1 of "The
// boundary rule"). Adapters: src/hooks/sprites.cpp.

#pragma once

#include <cstdint>

namespace sprites {

// A sprite list's corners (500 sprites of 4) and a single sprite's.
constexpr uint32_t kListCorners = 2000;
constexpr uint32_t kSpriteCorners = 4;

// SpriteList::Reset (sub_821298C0) and Sprite::Reset (sub_8211EB60), the
// same code for `corners` corners: at `sprite`, the identity matrix (+0,
// written to the 16-byte aligned rows as the game's vector stores do, after
// w = 1.0 at +12, +28, +44 and +60), +64, +68 and +72 cleared, and w = 1.0 in
// each corner (16 bytes apart from +80, the float at +92), whatever the
// count will be.
void ResetSprites(uint8_t* base, uint32_t sprite, uint32_t corners);

// Overlay::LayerFor (sub_82126630): the render queue layer the overlays
// submit their sprites with for a camera pass index: 2 -> 491, 3 -> 53,
// anything else 270.
uint32_t LayerFor(uint32_t pass_index);

// Vec4::Equal (sub_82123300): whether all four components are equal (as
// floats: -0 equals 0, NaN equals nothing).
bool Equal4(const float a[4], const float b[4]);

// Trail::TurnSide (sub_82129940): which way a trail turns about the vertical
// from direction a to direction b: 0 if they're equal (Equal4), 1 if their
// cross product's y (as the game computes it, -(a.x b.z - a.z b.x), flushing
// denormals to zero) is above `zero` (the game's 0.0), else -1.
int32_t TurnSide(const float a[4], const float b[4], float zero);

}  // namespace sprites
