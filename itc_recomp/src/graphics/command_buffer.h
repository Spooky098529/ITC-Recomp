// The graphics library's command buffer (docs/functions/command-buffer.md):
// the segments its packet writers fill, the data space reserved beside
// them, the segments' tails (the fences and read positions our kick applies,
// src/native/gpu_link.cpp), the callback packets and predicated tiling's
// chains of blocks. Plain C++ over the device in guest memory, which
// recompiled packet writers share; the adapters are in
// src/hooks/command_buffer.cpp (--native_command_buffer).

#pragma once

#include <cstdint>

namespace graphics::command_buffer {

// The device's fields used here.
constexpr uint32_t kWritePosition = 40;    // the last word written (pre-increment)
constexpr uint32_t kSegmentEnd = 44;
constexpr uint32_t kProgressBlock = 10768;  // the fences and read position
constexpr uint32_t kCallbackBlock = 10772;
constexpr uint32_t kFence = 10780;          // the open segment's fence
constexpr uint32_t kFlags = 10809;          // bit 1 the GPU hung, bit 6 out of space
constexpr uint32_t kTailPosition = 12968;   // the last tail's position and generation
constexpr uint32_t kTailGeneration = 12972;
constexpr uint32_t kSavedSegment = 13056;   // SaveSegment's six words
constexpr uint32_t kDataCursor = 13516;
constexpr uint32_t kGeneration = 13520;
constexpr uint32_t kWrapCursor = 13524;
constexpr uint32_t kSegmentStart = 13528;
constexpr uint32_t kTailsDirect = 19980;    // set: the hung GPU's progress not written

// The address the GPU sees for a CPU address (the physical address, the
// 0xE0000000 view's 4 KB offset added).
inline uint32_t PhysicalAddress(uint32_t address) {
  return (address & 0x1FFFFFFFu) + ((((address >> 20) & 0xFFFu) + 0x200u) & 0x1000u);
}

// Device::WriteSegmentTail (sub_8229E768): the 10 words ending a segment at
// `position` (the invalidate and two EVENT_WRITE_SHD: the read position with
// the generation's low bits and the fence into the progress block), the
// position and generation kept at +12968 and +12972, and both written into
// the progress block when the GPU hung (+10809 bit 1, +19980 0); the fence
// +10780 advanced by 2. Returns position + 40.
uint32_t WriteSegmentTail(uint8_t* base, uint32_t device, uint32_t position);

// Device::SaveSegment (sub_8229DBF0): the write position, the end, the
// segment's start, the wrap cursor, the data cursor and the generation
// copied to +13056..+13076.
void SaveSegment(uint8_t* base, uint32_t device);

// CommandChain::Close (sub_8229DFB8): the current block's first word gets
// where its content ends (the cursor, 0xC0000000 + its physical address),
// the cursor the end marker 0xC0000000, and the block's end +12 is cleared.
void CloseChain(uint8_t* base, uint32_t chain);

// The callback packets (sub_8229E9D0) at `position` (pre-increment): a
// WAIT_UNTIL first unless `flags` has bits outside the CPUs (24-29); then
// CALLBACK_ADDRESS and CALLBACK_CONTEXT = `function`, `argument`, the CPUs
// (0: 4) into SCRATCH_REG0, a WAIT_REG_MEM on the callback block, the
// INTERRUPT our kick runs (src/native/gpu_link.cpp), CALLBACK_ACK, a
// WAIT_REG_MEM for the callbacks done and CALLBACK_ADDRESS = 0BADF00D.
// Returns the last word written.
uint32_t WriteCallback(uint8_t* base, uint32_t device, uint32_t position, uint32_t flags,
                       uint32_t function, uint32_t argument);

}  // namespace graphics::command_buffer
