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
constexpr uint32_t kWaitRegMem = 0xC0043C01u;  // WAIT_REG_MEM, five words
constexpr uint32_t kInterrupt = 0xC0005401u;   // INTERRUPT, one word

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

uint32_t WriteCallback(uint8_t* base, uint32_t device, uint32_t position, uint32_t flags,
                       uint32_t function, uint32_t argument) {
  ITC_NATIVE_RUNNING("sub_8229E9D0 Device::WriteCallback");
  uint32_t at = position;
  auto put = [&](uint32_t word) {
    at += 4;
    guest::StoreU32(base, at, word);
  };
  uint32_t cpus = (flags >> 24) & 0x3F;
  if ((flags & 0xC0FFFFFFu) == 0) {
    put(0x5C8);  // WAIT_UNTIL
    put(0x20000);
  }
  put(0x1057C);  // CALLBACK_ADDRESS, CALLBACK_CONTEXT
  put(function);
  put(argument);
  if (cpus == 0) {
    cpus = 4;
  }
  put(0x578);  // SCRATCH_REG0
  put(cpus);
  put(kWaitRegMem);
  put(19);
  put(PhysicalAddress(guest::LoadU32(base, device + kCallbackBlock)) | 2);
  put(cpus);
  put(0xFFFFFFFFu);
  put(256);
  put(kInterrupt);
  put(cpus);
  put(0x45E);  // CALLBACK_ACK
  put(cpus);
  put(kWaitRegMem);
  put(19);
  put(PhysicalAddress(guest::LoadU32(base, device + kCallbackBlock)) | 2);
  put(0);
  put(0xFFFFFFFFu);
  put(256);
  put(0x57C);  // CALLBACK_ADDRESS
  put(0x0BADF00Du);
  return at;
}

}  // namespace graphics::command_buffer
