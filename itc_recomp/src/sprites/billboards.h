// The light glows' billboards (the light groups) as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/sprites.md).
// The material and its lights live on the guest stack and in level data, and
// the vertices go to the game's vertex ring, all read by recompiled code, so
// these work on guest memory (level 1 of "The boundary rule"). Adapters:
// src/hooks/billboards.cpp, and the vertex builder's in
// src/interpolation/blend_state.cpp (its camera blend around it).

#pragma once

#include <cstdint>

namespace car_scene_draw {
struct SceneDrawCalls;
}

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

// The recording (src/sprites/billboard_record.cpp). Each takes `stack`, the
// guest r1 at entry, and keeps the original's frame below it (its back chain
// and temporaries; its guest calls made from it through `calls` with the
// original's return addresses). Flush-to-zero off at entry and exit. The
// adapters (src/hooks/billboards.cpp, the wrap in
// src/debug/frame_state_diff.cpp) run them only with --native_billboards and
// --native_matrix_math on and the command list's diagnostics off.

// Billboards::RecordState (sub_8209C1A0): the material's render states by
// its alpha mode (+0) and blend mode (+4), command 7 with vertex c0 x4 = the
// camera pass's (`[set]`) view (+16) x projection (+80) through an identity
// on its 368-byte frame (both multiplies noted for frame interpolation), the
// shaders at 0x826A50A0 and 0x826A50A4, the texture (+12) in stage 0 or none
// in stages 0 to 3. Returns the last slot (the original's r3).
uint32_t RecordBillboardState(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                              uint32_t stack, uint32_t set, uint32_t list, uint32_t material);
// Billboards::Record (sub_8209CC38): a chunk on the render manager's command
// list with RecordState's states, the vertices (BuildVertices inside frame
// interpolation's camera blend), three render states and command 36,
// handed to the render queue (layer `set` + 4, depth 0.0) when any vertex
// was counted. Returns the original's r3 (the list, or the queue call's).
uint64_t RecordBillboards(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                          uint32_t stack, uint32_t set, uint32_t material);
// Billboards::Draw (sub_8209CDB0): 0 when the material has no lights (+32),
// else Record (inside its wrap's checks) and 1.
uint64_t DrawBillboards(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                        uint32_t stack, uint32_t set, uint32_t material);

// The light groups' draw (sub_82148658, LightGroups::Draw): for each of the
// groups (`groups` + 0 the count, the first at +16: its flags +0, texture id
// +4, light count +44, lights from +60, 48 bytes each), a material on its
// 192-byte frame (+80): alpha mode 1024 (flag 0x200), else 64 (0x10) or 32;
// blend mode 2 (0x40) or 4; with `xpr`, the texture from the XPR by the id or
// its animation's current frame in `animations`; Draw. Returns the last
// Draw's r3 (`set` with no groups).
uint64_t DrawLightGroups(uint8_t* base, const car_scene_draw::SceneDrawCalls& calls,
                         uint32_t stack, uint32_t set, uint32_t groups, uint32_t xpr,
                         uint32_t animations);

}  // namespace sprites
