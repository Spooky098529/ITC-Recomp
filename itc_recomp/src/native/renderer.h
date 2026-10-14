// The native renderer (--native_renderer): draws the game's render commands
// with Vulkan in its own window. See docs/native-renderer.md.
//
// Milestone 1: the renderer owns a window, device and swapchain and is fed
// every replayed command; each frame it clears and presents.

#pragma once

#include <cstdint>

namespace native {

bool Renderer_Enabled();

// Render thread, before a command handler runs (see the table in
// docs/gpu-functions.md): r3 = payload, r4 = arg1, r5 = arg2.
void Renderer_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                        uint32_t arg2);

// Render thread, after a command handler has run. For draws, the device's
// register shadow then holds everything the draw used (some of it is filled
// in by the draw call itself; see docs/gpu-functions.md).
void Renderer_AfterCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                           uint32_t arg2);

// Render thread, at the library's End (sub_825BDDA0) of an inline vertex
// draw a command made (BeginVertices, sub_825BD8F0): type, vertex count,
// stride in bytes and where the vertices were copied (a guest address).
void Renderer_InlineDraw(uint8_t* base, uint32_t type, uint32_t count, uint32_t stride,
                         uint32_t data);

// Render thread, at the frame's end (sub_8258D998, outside the command
// list): the game resolves render target 0 of `device` into the front buffer
// texture `front_buffer`.
void Renderer_FrontBufferResolve(uint8_t* base, uint32_t device, uint32_t front_buffer);

// Render thread, in command 33 (EndTiling): each Resolve (sub_822AFA58)
// call, with its arguments (guest addresses), tile of tiles.
void Renderer_TiledResolve(uint8_t* base, uint32_t device, uint32_t flags, uint32_t rect,
                           uint32_t destination, uint32_t point, uint32_t clear_color,
                           float clear_z, uint32_t tile, uint32_t tiles);

// A DrawVertices (sub_825BE300) outside the command list, after the
// library's call: device, primitive type, start vertex, vertex count.
void Renderer_DirectDraw(uint8_t* base, uint32_t device, uint32_t type, uint32_t start,
                         uint32_t count);

// Render thread, after each frame's replay and present.
void Renderer_EndFrame();

}  // namespace native
