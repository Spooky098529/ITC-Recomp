// The vertex and index buffers draws use, and where their data comes from
// (--native_renderer_log_buffers; see buffers.cpp).

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

namespace native {

bool Buffers_Logging();

// The library's memcpy (sub_825A7410) for native callers: the bytes copied
// as its override copies them natively (c_runtime::Memcpy), with the
// override's work around it (the logging), `caller`
// standing for the call's return address in the logs. False, with nothing
// done, where the override would run the original: --native_memcpy off, or
// ranges that may share memory.
bool Buffers_Memcpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
                    uint32_t caller);
// Whether Buffers_Memcpy copies at all (--native_memcpy): for native callers
// checking before they change anything.
bool Buffers_NativeMemcpy();

// Render thread, before a command handler runs: the buffer objects commands
// 11 and 12 bind.
void Buffers_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t count);

}  // namespace native
