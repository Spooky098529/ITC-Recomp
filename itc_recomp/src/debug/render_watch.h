#pragma once

#include <cstdint>

namespace debug {

// Step 0 of docs/conventional-renderer.md: two page-protection watches,
// diagnostics only (off by default, slow when on).
//
// --watch_register_shadow: game code outside the render thread's replay
// touching the graphics device's register shadow and the fields around it
// (the device at [0x8296BD90], +0 to +12440). The main thread waits for
// each frame's replay to finish (AfterHandOver), the shadow's pages are
// open while the render thread replays (OnDrawStart) and protected
// otherwise (OnDrawEnd), so every access outside the replay faults and is
// noted.
//
// --watch_gpu_data_reads: CPU accesses to the vertex, index and texture
// data draws have used (OnGpuData, from the renderer), through the guest's
// virtual views of physical memory (0xA0000000, 0xC0000000, 0xE0000000).
// The renderer reads through the physical view and isn't seen. A write
// (memcpy loading new data, a file read: BeforeHostWrite) stops watching
// that page until a draw uses it again.
//
// Both log "render watch: <module>+0x<offset> ..." lines every 5 s;
// tools/resolve_watch_reads.py turns them into functions.

// Main thread, around the list's hand-over to the render thread
// (Renderer::SignalTick, sub_82091A58).
uint64_t RenderWatch_BeforeHandOver();
void RenderWatch_AfterHandOver(uint64_t token);
// Render thread: the replay's device (RenderQueue::Replay; [0x8296BD90]
// holds it only during the replay).
void RenderWatch_OnReplay(uint32_t device);
// Render thread, around Renderer_DrawFrame.
void RenderWatch_OnDrawStart(uint8_t* base);
void RenderWatch_OnDrawEnd(uint8_t* base);

// The renderer: a draw used [physical, physical + size).
enum class GpuData : uint8_t { kVertex = 1, kIndex = 2, kTexture = 4 };
bool RenderWatch_WatchingGpuData();
void RenderWatch_OnGpuData(uint32_t physical, uint32_t size, GpuData kind);
// Host code about to write guest memory in a way a fault can't catch (a
// read() into it fails with EFAULT instead): the pages are dropped first.
void RenderWatch_BeforeHostWrite(const uint8_t* host, uint64_t size);

}  // namespace debug
