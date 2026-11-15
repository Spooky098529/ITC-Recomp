// The graphics library's draws that don't take an index buffer the game
// made (docs/functions/functions.md: BeginVertices sub_825BD8F0,
// EndVertices sub_825BDDA0, BeginIndexedVertices sub_825BDDC0, DrawVertices
// sub_825BE300) as plain C++ over the device in guest memory. What our
// renderer and the game read is kept: the dirty state's flush (the masks at
// device +0 to +32 cleared, the shader load, the native renderer's dirty
// marks the dropped packet writers make, src/native/draw_packets.cpp), the
// segment's end when the write position has passed the soft end, the space
// reserved for the caller's vertices and indices, and the fields End and
// the callers read. Dropped (the graphics rule): the GPU packets (the
// vertex fetch constant, DRAW_INDX, the inline index and vertex packets)
// and predicated tiling's draw chain records, which nothing reads (our kick
// looks only for fences and interrupts; the chains are never kicked,
// docs/functions/predicated-tiling.md). The write position +40 is left
// where it was, as the dropped writers leave it. The adapters are the wraps
// in src/native/replay_hooks.cpp (--native_inline_draws).
#pragma once

#include <cstdint>

namespace graphics::inline_draws {

// BeginVertices (sub_825BD8F0): `count` vertices of `stride` bytes. Returns
// the space reserved for them (16-byte aligned, in the command buffer's
// data space), or 0 when none is left.
uint32_t BeginVertices(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t device, uint32_t count,
                       uint32_t stride);

// EndVertices (sub_825BDDA0): the write position +40 set to where Begin
// left it (+13092). Its tail call to the empty sub_82106540 (without a
// recording command buffer, +10808 bit 0x80) does nothing and is left out.
void EndVertices(uint8_t* base, uint32_t device);

// BeginIndexedVertices (sub_825BDDC0): `vertex_count` vertices of `stride`
// bytes and `index_count` indices (32-bit with `format` bit 0x4, else
// 16-bit). The index space goes to [index_out], the vertex space to the
// word at sp + 84 points to. Returns 0, or 0x8007000E (both 0) when either
// can't be reserved.
uint32_t BeginIndexedVertices(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t device,
                              uint32_t vertex_count, uint32_t index_count, uint32_t format,
                              uint32_t stride, uint32_t index_out);

// DrawVertices (sub_825BE300): `count` vertices from `start` of primitive
// `type`, cut into draws of at most 65,535 vertices (the type's pair at
// 0x82000950) as the original's packets are. r3 not kept (its callers
// don't read it).
void DrawVertices(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t device, uint32_t type,
                  uint32_t start, uint32_t count);

}  // namespace graphics::inline_draws
