// The vertex and index buffers draws use, and where their data comes from
// (--native_renderer_log_buffers; see buffers.cpp).

#pragma once

#include <cstdint>

namespace native {

bool Buffers_Logging();

// Render thread, before a command handler runs: the buffer objects commands
// 11 and 12 bind.
void Buffers_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count);

// Render thread: a vertex or index range (physical) copied into the mirror,
// the first time in a frame.
void Buffers_OnMirror(uint32_t physical, uint32_t size, bool index);

}  // namespace native
