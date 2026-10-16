// The native renderer (--native_renderer): draws the game's render commands
// with Vulkan in its own window. See docs/native-renderer.md.
//
// Milestone 1: the renderer owns a window, device and swapchain and is fed
// every replayed command; each frame it clears and presents.

#pragma once

#include <cstdint>

namespace native {

bool Renderer_Enabled();

// Main thread, as a pass's command list is handed to the render thread
// (before Renderer::WaitForFrame): whether a logic pass recorded it. Render
// thread, as a frame's replay starts: takes the next list's. Then whether
// the list being replayed came from a logic pass (--native_renderer_mirror_by_tick).
void Renderer_OnListHandedOver(bool logic_pass);
void Renderer_OnReplayStart();
bool Renderer_ReplayingLogicList();
// Since the last call: frames replayed with no list handed over, and the
// most lists waiting at a frame's start.
void Renderer_TakeListStats(uint64_t& missing, uint64_t& most);

// Exclusive mode (milestone 6): the emulated GPU gets no work; see
// gpu_link.cpp.
bool Renderer_Exclusive();

// The display's 256-entry gamma table as the game last wrote it
// (DC_LUT_30_COLOR words; exclusive mode only, see gpu_link.cpp): copied
// into `table` and true if it changed since the last call.
bool GammaRamp_Take(uint32_t (&table)[256]);

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

// Render thread, after each frame's replay and present.
void Renderer_EndFrame();

}  // namespace native
