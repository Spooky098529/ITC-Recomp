// The graphics library's command buffer (docs/functions/command-buffer.md):
// see command_buffer.h.

#include "graphics/command_buffer.h"

#include "guest.h"

#include "base/log.h"

namespace graphics::command_buffer {

namespace {

constexpr uint32_t kInvalidateState = 0xC0003B00u;  // INVALIDATE_STATE, one word
constexpr uint32_t kEventWriteShd = 0xC0025800u;    // EVENT_WRITE_SHD, three words
constexpr uint32_t kEndMarker = 0xC0000000u;

}  // namespace

uint32_t WriteSegmentTail(uint8_t* base, uint32_t device, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8229E768 Device::WriteSegmentTail");
  const uint32_t generation = guest::LoadU32(base, device + kGeneration);
  const uint32_t write = guest::LoadU32(base, device + kWritePosition);
  const uint32_t fence = guest::LoadU32(base, device + kFence);
  const uint32_t progress = guest::LoadU32(base, device + kProgressBlock);
  const uint32_t read_position = (generation & 3) | write;
  guest::StoreU32(base, device + kTailGeneration, generation);
  guest::StoreU32(base, device + kTailPosition, write);
  // The stores in the original's order.
  guest::StoreU32(base, position + 12, 3);
  guest::StoreU32(base, position + 28, 3);
  guest::StoreU32(base, position + 0, kInvalidateState);
  guest::StoreU32(base, position + 4, 0x7FFF);
  guest::StoreU32(base, position + 8, kEventWriteShd);
  guest::StoreU32(base, position + 24, kEventWriteShd);
  guest::StoreU32(base, position + 20, read_position);
  guest::StoreU32(base, position + 36, fence);
  guest::StoreU32(base, position + 32, PhysicalAddress(progress) | 2);
  guest::StoreU32(base, position + 16, PhysicalAddress(progress + 4) | 2);
  if (guest::LoadU32(base, device + kTailsDirect) == 0 &&
      (guest::LoadU8(base, device + kFlags) & 0x2)) {
    guest::StoreU32(base, guest::LoadU32(base, device + kProgressBlock), fence);
    guest::StoreU32(base, guest::LoadU32(base, device + kProgressBlock) + 4, read_position);
  }
  guest::StoreU32(base, device + kFence, fence + 2);
  return position + 40;
}

void SaveSegment(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229DBF0 Device::SaveSegment");
  const uint32_t words[6] = {
      guest::LoadU32(base, device + kWritePosition), guest::LoadU32(base, device + kSegmentEnd),
      guest::LoadU32(base, device + kSegmentStart),  guest::LoadU32(base, device + kWrapCursor),
      guest::LoadU32(base, device + kDataCursor),    guest::LoadU32(base, device + kGeneration)};
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, device + kSavedSegment + 4 * i, words[i]);
  }
}

void CloseChain(uint8_t* base, uint32_t chain) {
  ITC_NATIVE_RUNNING("sub_8229DFB8 CommandChain::Close");
  const uint32_t cursor = guest::LoadU32(base, chain + 8);
  const uint32_t block = guest::LoadU32(base, chain + 4);
  guest::StoreU32(base, block, PhysicalAddress(cursor) + kEndMarker);
  guest::StoreU32(base, guest::LoadU32(base, chain + 8), kEndMarker);
  guest::StoreU32(base, chain + 12, 0);
}

}  // namespace graphics::command_buffer
