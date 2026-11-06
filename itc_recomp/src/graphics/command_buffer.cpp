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

uint32_t NextSpace(uint8_t* base, uint32_t device, uint32_t kind, uint32_t& size,
                   uint32_t alignment) {
  ITC_NATIVE_RUNNING("sub_8229E100 Device::NextSpace");
  auto load = [&](uint32_t field) { return guest::LoadU32(base, device + field); };
  auto store = [&](uint32_t field, uint32_t value) { guest::StoreU32(base, device + field, value); };
  const uint32_t words = size >> 2;
  uint32_t cursor = load(kDataCursor);
  if (kind & 1) {
    // Data.
    if (cursor == 0) {
      cursor = load(kSegmentEnd) + 60;
    }
    const uint32_t mask = ~(alignment - 1);
    const uint32_t bytes = words << 2;
    const uint32_t generation = load(kGeneration);
    uint32_t start = (cursor + alignment - 1) & mask;
    uint32_t end = bytes + start;
    if (end <= load(kBufferEnd)) {
      const uint32_t tiling = load(kTilingEnd);
      if (tiling != 0) {
        const int32_t apart = int32_t(load(kTilingGeneration) - generation);
        if (apart == -1 || (apart == 0 && tiling < end)) {
          return 0;
        }
      }
      store(kDataCursor, end);
      return start;
    }
    start = (load(kWrapCursor) + alignment - 1) & mask;
    end = bytes + start;
    const uint32_t tiling = load(kTilingEnd);
    if (tiling != 0) {
      const int32_t apart = int32_t(load(kTilingGeneration) - generation - 1);
      if (apart == -1 || (apart == 0 && tiling < end)) {
        return 0;
      }
    }
    if (end > load(kSegmentStart)) {
      return 0;
    }
    store(kWrapCursor, end);
    return start;
  }
  // A segment.
  if (cursor == 0) {
    cursor = load(kWritePosition) + 4;
  }
  uint32_t start = (cursor + 31) & ~31u;
  const uint32_t wanted = words != 0 ? words : 54;
  uint32_t segment_words = load(kSegmentSize) >> 2;
  if (wanted > segment_words) {
    segment_words = wanted;
  }
  const uint32_t bytes = segment_words << 2;
  uint32_t end = bytes + start;
  if (end > load(kBufferEnd)) {
    end = load(kBufferEnd);
  }
  auto room = [&] { return int32_t(end - start) >> 2; };
  if (room() < int32_t(wanted)) {
    // Wrapped to the buffer's start.
    const uint32_t generation = load(kGeneration);
    start = (load(kWrapCursor) + 31) & ~31u;
    end = bytes + start;
    store(kWrapCursor, load(kBufferStart));
    store(kGeneration, generation + 1);
    guest::StoreU8(base, device + kFlags2, guest::LoadU8(base, device + kFlags2) | 8);
  }
  const uint32_t tiling = load(kTilingEnd);
  if (tiling != 0) {
    const uint32_t tiling_generation = load(kTilingGeneration);
    const uint32_t generation = load(kGeneration);
    const int32_t apart = int32_t(tiling_generation - generation);
    if (apart == -1 || (apart == 0 && tiling < end)) {
      end = tiling;
      if (generation != tiling_generation || room() < int32_t(wanted)) {
        return 0;
      }
    }
  }
  size = (end - start) & ~3u;
  return start;
}

uint32_t Reserve(uint8_t* base, uint32_t device, uint32_t words, uint32_t alignment) {
  ITC_NATIVE_RUNNING("sub_8229E308 Device::AllocateFromCommandBuffer");
  if (guest::LoadU8(base, device + kFlags) & 0x40) {
    return 0;
  }
  uint32_t size = words << 2;
  const uint32_t space = NextSpace(base, device, 1, size, alignment);
  const uint32_t data = guest::LoadU32(base, device + kSegmentData) + size;
  guest::StoreU32(base, device + kSegmentData, data);
  if (data > guest::LoadU32(base, device + kSegmentDataBudget) &&
      guest::LoadU32(base, device + kTilingEnd) == 0) {
    // The segment cut short: the next writer ends it.
    const uint32_t wrap_cursor = guest::LoadU32(base, device + kWrapCursor);
    const uint32_t buffer_start = guest::LoadU32(base, device + kBufferStart);
    const uint32_t write = guest::LoadU32(base, device + kWritePosition);
    guest::StoreU32(base, device + kSoftEnd, write);
    guest::StoreU32(base, device + kSegmentEnd, write + 160);
    if (wrap_cursor != buffer_start) {
      guest::StoreU32(base, device + kDataCursor, guest::LoadU32(base, device + kBufferEnd));
    }
  }
  if (space == 0) {
    guest::StoreU8(base, device + kFlags, guest::LoadU8(base, device + kFlags) | 0x40);
  }
  return space;
}

uint32_t WriteDefaultDrawRegisters(uint8_t* base, uint32_t device, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_822AF978 Device::WriteDefaultDrawRegisters");
  auto mark = [&](uint32_t field, uint64_t bits) {
    REX_STORE_U64(device + field, REX_LOAD_U64(device + field) | bits);
  };
  mark(24, 0x400000 | 0x200000);
  mark(16, 0x80 | 0x40 | 0x20);
  mark(32, 0x80000);
  mark(24, 0x2000000);
  return position;
}

uint32_t NewBlock(uint8_t* base, uint32_t chain) {
  ITC_NATIVE_RUNNING("sub_8229EB10 CommandChain::NewBlock");
  // 0xC0000000 + the physical address (the original subtracts 0x40000000).
  auto gpu = [](uint32_t address) { return PhysicalAddress(address) - 0x40000000u; };
  const uint32_t old_block = guest::LoadU32(base, chain + kChainBlock);
  if (old_block != 0) {
    guest::StoreU32(base, old_block, gpu(guest::LoadU32(base, chain + kChainCursor)));
  }
  const uint32_t device = guest::LoadU32(base, chain + kChainDevice);
  uint32_t block = Reserve(base, device, 4224, 128);
  if (block == 0) {
    block = guest::LoadU32(base, device + kDummySegment);
    guest::StoreU32(base, chain + kChainFirst, 0);
  } else {
    guest::StoreU32(base, block, gpu(block + 4));
    const uint32_t cursor = guest::LoadU32(base, chain + kChainCursor);
    if (cursor != 0) {
      guest::StoreU32(base, cursor, gpu(block));
    }
    // The data cache flush of the block (sub_822AC470): Dropped.
  }
  guest::StoreU32(base, chain + kChainBlock, block);
  guest::StoreU32(base, chain + kChainCursor, block + 4);
  guest::StoreU32(base, chain + kChainEnd, block + 4220);
  return block + 4;
}

uint32_t InitChain(uint8_t* base, uint32_t chain, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229ED60 CommandChain::Init");
  guest::StoreU32(base, chain + kChainDevice, device);
  guest::StoreU32(base, chain + kChainCursor, 0);
  guest::StoreU32(base, chain + kChainEnd, 0);
  guest::StoreU32(base, chain + kChainBlock, 0);
  const uint32_t cursor = NewBlock(base, chain);
  guest::StoreU32(base, chain + kChainFirst, guest::LoadU32(base, chain + kChainBlock));
  return cursor;
}

bool Recording(uint8_t* base, uint32_t device) {
  return (guest::LoadU8(base, device + kRecording) & 0x80) != 0 ||
         guest::LoadU32(base, device + kRecorder) != 0;
}

uint32_t EndSegment(uint8_t* base, uint32_t device, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229EF48 Device::EndSegment");
  // The pending coherency flush block (sub_8229E5D0): Dropped, none.
  KickSegment(base, device, kick);
  if (guest::LoadU32(base, kTrainingFailed) != 0 &&
      !(guest::LoadU8(base, device + kFlags) & 0x2)) {
    WaitForFence(base, device, guest::LoadU32(base, device + kFence) - 2, 0, kick);
    guest::StoreU8(base, device + kFlags, guest::LoadU8(base, device + kFlags) | 0x2);
  }
  return guest::LoadU32(base, device + kWritePosition);
}

uint32_t KickSegment(uint8_t* base, uint32_t device, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229EDA8 Device::KickSegment");
  auto load = [&](uint32_t field) { return guest::LoadU32(base, device + field); };
  auto store = [&](uint32_t field, uint32_t value) { guest::StoreU32(base, device + field, value); };
  uint32_t r3 = device;
  const uint32_t start = load(kSegmentStart);
  uint32_t end = load(kWritePosition) + 4;
  if (!(guest::LoadU8(base, device + kFlags) & 0x40)) {
    if (load(kTilingEnd) != 0) {
      // Predicated tiling: the segment appended to the chain, kicked per
      // tile by the tiling worker.
      const uint32_t words = uint32_t(int32_t(end - start) >> 2);
      if (words != 0) {
        const uint32_t chain = device + kSegmentChain;
        r3 = chain;
        uint32_t cursor = guest::LoadU32(base, chain + kChainCursor);
        if (cursor + 8 > guest::LoadU32(base, chain + kChainEnd)) {
          cursor = NewBlock(base, chain);
          r3 = cursor;
        }
        guest::StoreU32(base, cursor + 4, PhysicalAddress(start));
        guest::StoreU32(base, cursor, words | 0x81000000u);
        store(kSegmentChain + kChainCursor, cursor + 8);
      }
    } else {
      const uint32_t tail = WriteSegmentTail(base, device, end);
      end = kick(device, tail, PhysicalAddress(start), uint32_t(int32_t(tail - start) >> 2), 0,
                 device + kSegmentChain);
      r3 = end;
    }
  }
  const uint32_t next = (end + 31) & ~31u;
  if (next > load(kSoftEnd)) {
    store(kWritePosition, end - 4);
    return NextSegment(base, device, 0, kick);
  }
  store(kSegmentStart, next);
  store(kWritePosition, next - 4);
  if (load(kTailsDirect) != 0) {
    const uint32_t cursor = load(kDataCursor);
    if (cursor != 0) {
      store(kDataCursor, (cursor + 4095) & ~4095u);
    }
  }
  return r3;
}

uint32_t NextSegment(uint8_t* base, uint32_t device, uint32_t words, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229EC00 Device::NextSegment");
  auto load = [&](uint32_t field) { return guest::LoadU32(base, device + field); };
  auto store = [&](uint32_t field, uint32_t value) { guest::StoreU32(base, device + field, value); };
  auto out_of_space = [&] {
    guest::StoreU8(base, device + kFlags, guest::LoadU8(base, device + kFlags) | 0x40);
  };
  uint32_t write;
  uint32_t end;
  if (guest::LoadU8(base, device + kFlags) & 0x40) {
    out_of_space();
    write = load(kDummySegment);
    end = write + 4800;
    store(kWritePosition, write);
    store(kSegmentEnd, end);
    store(kSoftEnd, end - 160);
    return 0;
  }
  uint32_t size = words != 0 ? (words + 14) << 2 : 0;
  const uint32_t segment = NextSpace(base, device, 2, size, 32);
  if (segment == 0) {
    out_of_space();
    write = load(kDummySegment);
    end = write + 4800;
  } else {
    write = segment - 4;
    store(kSegmentStart, segment);
    store(kDataCursor, 0);
    store(kSegmentData, 0);
    end = (((size >> 2) - 14) << 2) + write;
  }
  store(kWritePosition, write);
  store(kSegmentEnd, end);
  store(kSoftEnd, end - 160);
  // The frame throttle: the last Swap's fence.
  const uint32_t fence = (load(kThrottleSelect) & 1) ? kThrottleFence2 : kThrottleFence;
  WaitForFence(base, device, load(fence), 3, kick);
  store(fence, 0);
  return segment;
}

uint32_t WaitForFence(uint8_t* base, uint32_t device, uint32_t fence, uint32_t code,
                      const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229E908 Device::WaitForFence");
  (void)code;
  if (fence == 0) {
    return device;
  }
  const uint32_t open = guest::LoadU32(base, device + kFence);
  const uint32_t done = guest::LoadU32(base, guest::LoadU32(base, device + kProgressBlock));
  if (open - fence >= open - done) {
    return device;
  }
  uint32_t r3 = device;
  if (fence == open) {
    if (guest::LoadU32(base, device + kTilingEnd) != 0) {
      return device;
    }
    r3 = EndSegment(base, device, kick);
  }
  // Still pending: the original waits for the GPU here (Dropped).
  return r3;
}

}  // namespace graphics::command_buffer
