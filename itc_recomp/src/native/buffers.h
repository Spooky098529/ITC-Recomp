// The vertex and index buffers draws use, and where their data comes from
// (--native_renderer_log_buffers; see buffers.cpp).

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace native {

bool Buffers_Logging();

// Render thread, before a command handler runs: the buffer objects commands
// 11 and 12 bind.
void Buffers_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count);

// Render thread: the sequence number of the newest write the library's
// memcpy made into [physical, physical + size), 0 if none; and the newest
// sequence number of all. A range read when the sequence was S needs reading
// again only if Buffers_NewestWrite returns more than S.
uint64_t Buffers_NewestWrite(uint32_t physical, uint32_t size);
uint64_t Buffers_Sequence();
// Render thread: what the library's memcpy wrote into [physical, physical +
// size): bytes covered, the first segment's caller, and the newest write's
// sequence and caller.
struct Buffers_Coverage {
  uint64_t bytes = 0, newest = 0;
  uint32_t newest_caller = 0, first_caller = 0;
};
Buffers_Coverage Buffers_CoverageOf(uint32_t physical, uint32_t size);
// Render thread: the ranges [start, end) memcpy has written into the
// physical views since the last call, appended to `out` (the texture cache
// marks the textures they overlap as changed).
void Buffers_TakeNewWrites(std::vector<std::pair<uint32_t, uint32_t>>& out);

// Render thread, with logging: a vertex or index range (physical) copied
// into the mirror, the first time in a frame.
void Buffers_OnMirror(uint32_t physical, uint32_t size, bool index);

}  // namespace native
