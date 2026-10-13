// Diagnostics (--log_replay_commands): how often the render thread replays
// each render command (the table at 0x8265EDE0; see docs/gpu-functions.md),
// with sample arguments for the commands not identified yet.

#pragma once

#include <cstdint>

namespace debug {

// Render thread, before a command handler runs: r3 = payload, r4 = arg1,
// r5 = arg2.
void ReplayStats_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                           uint32_t arg2);

// Render thread, after each frame's replay. Logs every 2 s.
void ReplayStats_OnFrame();

}  // namespace debug
