// The native renderer (--native_renderer): draws the game's render commands
// with Vulkan in its own window. See docs/native-renderer.md.
//
// Milestone 1: the renderer owns a window, device and swapchain and is fed
// every replayed command; each frame it clears and presents.

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace native {

bool Renderer_Enabled();

// Main thread, as a pass's command list is handed to the render thread
// (before Renderer::WaitForFrame): the pass's kind (GameMessages_PassKind:
// 0 logic, 1 render-only, 2 render-only and not drawn). Render thread, as a
// frame's replay starts: takes the next list's. Then whether the list being
// replayed came from a logic pass (--native_renderer_mirror_by_tick), and
// whether the frame is one that wasn't drawn, to be drawn from the tick's
// stored list (--native_renderer_tick_replay).
// With the kind, the command list ranges the main thread's rebuilt calls
// wrote in the pass, in order (GameMessages_TakeRebuiltRanges).
void Renderer_OnListHandedOver(int kind, std::vector<std::pair<uint32_t, uint32_t>> ranges = {});
// Render thread: those of the list being replayed.
const std::vector<std::pair<uint32_t, uint32_t>>& Renderer_ReplayingRanges();
void Renderer_OnReplayStart();
bool Renderer_ReplayingLogicList();
bool Renderer_ReplayingStoredFrame();
// Render thread, in place of the game's frame (its replay, front buffer
// resolve and present) on a frame that wasn't drawn: the tick's stored list
// again, with this frame's blends.
void Renderer_DrawStoredFrame(uint8_t* base);
// Any thread (--native_renderer_tick_replay): whether passes after a tick's
// first render-only pass may go undrawn; on, and the last tick's stored
// list was made.
bool Renderer_TickReplayReady();
// Render thread: the kind of the list being replayed (as handed over), and
// whether a tick's stored list was made (or not) for the frames after it.
int Renderer_ReplayingKind();
void Renderer_SetTickReplayReady(bool ready);
// Since the last call: frames replayed with no list handed over, and the
// most lists waiting at a frame's start.
void Renderer_TakeListStats(uint64_t& missing, uint64_t& most);

// Exclusive mode (milestone 6): the emulated GPU gets no work; see
// gpu_link.cpp.
bool Renderer_Exclusive();

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
