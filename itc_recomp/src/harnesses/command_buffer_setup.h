// The command buffer harness's set-up (src/harnesses/command_buffer.cpp),
// shared with the predicated tiling's harness: a device, blocks of the
// harness's own, and the segment flow's fields over a command buffer in
// physical memory.

#pragma once

#include "harness.h"

#include <cstdint>

namespace harness::command_buffer_setup {

constexpr uint32_t kDeviceBytes = 20480;   // through +20440
constexpr uint32_t kCommandBytes = 8192;   // the command buffer's words
constexpr uint32_t kProgressBytes = 64;

constexpr uint32_t kRingBytes = 0x20000;  // a command buffer of its own (physical memory)
constexpr uint32_t kRingProgress = kRingBytes;  // its progress block, after it
constexpr uint32_t kRingAllocation = kRingBytes + 4096;

struct Blocks {
  uint32_t device = 0;
  uint32_t commands = 0;
  uint32_t progress = 0;
  uint32_t ring = 0;
};

// Every word of `bytes` at `address` random.
void RandomWords(uint8_t* base, uint32_t address, uint32_t bytes, Random& r);

// The segment flow's inputs over the ring (see command_buffer.cpp): out of
// space in 1 case of 8, the open segment's words NOPs, the segment chain in
// the ring's first 8 KB. Returns the open fence.
uint32_t FlowFields(Context& h, const Blocks& b, Random& r, int c, bool full);

// The throttle's fences (+13540, +13544): done, 0, or the open one when
// `may_open`.
void ThrottleFences(Context& h, const Blocks& b, Random& r, uint32_t open, bool may_open);

// The "training failed" global set in some cases where it's safe.
void TrainingFailed(Context& h, const Blocks& b, Random& r, uint32_t open);

}  // namespace harness::command_buffer_setup
