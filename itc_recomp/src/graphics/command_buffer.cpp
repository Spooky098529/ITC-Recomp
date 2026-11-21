// The graphics library's command buffer (docs/functions/command-buffer.md):
// see command_buffer.h.

#include "graphics/command_buffer.h"

#include "guest.h"
#include "schema/command_buffer.h"
#include "schema/device_state.h"

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
  const schema::Device d(base, device);
  const uint32_t generation = d.wrap_generation();
  const uint32_t write = d.write_position();
  const uint32_t fence = d.fence();
  const uint32_t progress = d.progress_block().address;
  const uint32_t read_position = (generation & 3) | write;
  d.set_tail_generation(generation);
  d.set_tail_position(write);
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
  if (d.tap().address == 0 && (d.flags() & 0x2)) {
    d.progress_block().set_fence_done(fence);
    d.progress_block().set_read_position(read_position);
  }
  d.set_fence(fence + 2);
  return position + 40;
}

void SaveSegment(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229DBF0 Device::SaveSegment");
  const schema::Device d(base, device);
  const uint32_t words[6] = {d.write_position(), d.segment_end(), d.segment_start(),
                             d.wrap_cursor(),    d.data_cursor(), d.wrap_generation()};
  // The six saved words, +13056..+13076 in order.
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, d.saved_write_position_address() + 4 * i, words[i]);
  }
}

void RestoreSegment(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229E0C0 Device::RestoreSegment");
  const schema::Device d(base, device);
  const uint32_t end = d.saved_segment_end();
  d.set_write_position(d.saved_write_position());
  d.set_segment_start(d.saved_segment_start());
  d.set_wrap_cursor(d.saved_wrap_cursor());
  d.set_data_cursor(d.saved_data_cursor());
  d.set_segment_end(end);
  d.set_wrap_generation(d.saved_generation());
  d.set_soft_end(end - 160);
}

void CloseChain(uint8_t* base, uint32_t chain) {
  ITC_NATIVE_RUNNING("sub_8229DFB8 CommandChain::Close");
  const schema::CommandChain c(base, chain);
  const uint32_t cursor = c.cursor();
  const uint32_t block = c.block().address;
  schema::CommandChainBlock(base, block).set_content_end(PhysicalAddress(cursor) + kEndMarker);
  guest::StoreU32(base, c.cursor(), kEndMarker);
  c.set_end(0);
}

uint32_t WriteCallback(uint8_t* base, uint32_t device, uint32_t position, uint32_t flags,
                       uint32_t function, uint32_t argument) {
  ITC_NATIVE_RUNNING("sub_8229E9D0 Device::WriteCallback");
  const schema::Device d(base, device);
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
  put(PhysicalAddress(d.callback_block()) | 2);
  put(cpus);
  put(0xFFFFFFFFu);
  put(256);
  put(kInterrupt);
  put(cpus);
  put(0x45E);  // CALLBACK_ACK
  put(cpus);
  put(kWaitRegMem);
  put(19);
  put(PhysicalAddress(d.callback_block()) | 2);
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
  const schema::Device d(base, device);
  const uint32_t words = size >> 2;
  uint32_t cursor = d.data_cursor();
  if (kind & 1) {
    // Data.
    if (cursor == 0) {
      cursor = d.segment_end() + 60;
    }
    const uint32_t mask = ~(alignment - 1);
    const uint32_t bytes = words << 2;
    const uint32_t generation = d.wrap_generation();
    uint32_t start = (cursor + alignment - 1) & mask;
    uint32_t end = bytes + start;
    if (end <= d.command_buffer_end()) {
      const uint32_t tiling = d.tiling_end();
      if (tiling != 0) {
        const int32_t apart = int32_t(d.tiling_generation() - generation);
        if (apart == -1 || (apart == 0 && tiling < end)) {
          return 0;
        }
      }
      d.set_data_cursor(end);
      return start;
    }
    start = (d.wrap_cursor() + alignment - 1) & mask;
    end = bytes + start;
    const uint32_t tiling = d.tiling_end();
    if (tiling != 0) {
      const int32_t apart = int32_t(d.tiling_generation() - generation - 1);
      if (apart == -1 || (apart == 0 && tiling < end)) {
        return 0;
      }
    }
    if (end > d.segment_start()) {
      return 0;
    }
    d.set_wrap_cursor(end);
    return start;
  }
  // A segment.
  if (cursor == 0) {
    cursor = d.write_position() + 4;
  }
  uint32_t start = (cursor + 31) & ~31u;
  const uint32_t wanted = words != 0 ? words : 54;
  uint32_t segment_words = d.segment_size() >> 2;
  if (wanted > segment_words) {
    segment_words = wanted;
  }
  const uint32_t bytes = segment_words << 2;
  uint32_t end = bytes + start;
  if (end > d.command_buffer_end()) {
    end = d.command_buffer_end();
  }
  auto room = [&] { return int32_t(end - start) >> 2; };
  if (room() < int32_t(wanted)) {
    // Wrapped to the buffer's start.
    const uint32_t generation = d.wrap_generation();
    start = (d.wrap_cursor() + 31) & ~31u;
    end = bytes + start;
    d.set_wrap_cursor(d.command_buffer_start());
    d.set_wrap_generation(generation + 1);
    d.set_flags2(d.flags2() | 8);
  }
  const uint32_t tiling = d.tiling_end();
  if (tiling != 0) {
    const uint32_t tiling_generation = d.tiling_generation();
    const uint32_t generation = d.wrap_generation();
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
  const schema::Device d(base, device);
  if (d.flags() & 0x40) {
    return 0;
  }
  uint32_t size = words << 2;
  const uint32_t space = NextSpace(base, device, 1, size, alignment);
  const uint32_t data = d.segment_data() + size;
  d.set_segment_data(data);
  if (data > d.segment_data_budget() && d.tiling_end() == 0) {
    // The segment cut short: the next writer ends it.
    const uint32_t wrap_cursor = d.wrap_cursor();
    const uint32_t buffer_start = d.command_buffer_start();
    const uint32_t write = d.write_position();
    d.set_soft_end(write);
    d.set_segment_end(write + 160);
    if (wrap_cursor != buffer_start) {
      d.set_data_cursor(d.command_buffer_end());
    }
  }
  if (space == 0) {
    d.set_flags(d.flags() | 0x40);
  }
  return space;
}

uint32_t WriteDefaultDrawRegisters(uint8_t* base, uint32_t device, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_822AF978 Device::WriteDefaultDrawRegisters");
  const schema::Device d(base, device);
  d.set_render_dirty(d.render_dirty() | (0x400000 | 0x200000));
  d.set_shader_dirty(d.shader_dirty() | (0x80 | 0x40 | 0x20));
  d.set_other_dirty(d.other_dirty() | 0x80000);
  d.set_render_dirty(d.render_dirty() | 0x2000000);
  return position;
}

uint32_t NewBlock(uint8_t* base, uint32_t chain) {
  ITC_NATIVE_RUNNING("sub_8229EB10 CommandChain::NewBlock");
  // 0xC0000000 + the physical address (the original subtracts 0x40000000).
  auto gpu = [](uint32_t address) { return PhysicalAddress(address) - 0x40000000u; };
  const schema::CommandChain c(base, chain);
  const uint32_t old_block = c.block().address;
  if (old_block != 0) {
    schema::CommandChainBlock(base, old_block).set_content_end(gpu(c.cursor()));
  }
  const uint32_t device = c.device();
  const schema::Device d(base, device);
  uint32_t block = Reserve(base, device, 4224, 128);
  if (block == 0) {
    block = d.dummy_segment().address;
    c.set_first(0);
  } else {
    schema::CommandChainBlock(base, block).set_content_end(gpu(block + 4));
    const uint32_t cursor = c.cursor();
    if (cursor != 0) {
      guest::StoreU32(base, cursor, gpu(block));
    }
    // The data cache flush of the block (sub_822AC470): Dropped.
  }
  c.set_block(block);
  c.set_cursor(block + 4);
  c.set_end(block + 4220);
  return block + 4;
}

uint32_t InitChain(uint8_t* base, uint32_t chain, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_8229ED60 CommandChain::Init");
  const schema::CommandChain c(base, chain);
  c.set_device(device);
  c.set_cursor(0);
  c.set_end(0);
  c.set_block(0);
  const uint32_t cursor = NewBlock(base, chain);
  c.set_first(c.block().address);
  return cursor;
}

bool Recording(uint8_t* base, uint32_t device) {
  const schema::Device d(base, device);
  return (d.tiling_flags() & 0x80) != 0 || d.recorder().address != 0;
}

uint32_t EndSegment(uint8_t* base, uint32_t device, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229EF48 Device::EndSegment");
  const schema::Device d(base, device);
  // The pending coherency flush block (sub_8229E5D0): Dropped, none.
  KickSegment(base, device, kick);
  if (guest::LoadU32(base, kTrainingFailed) != 0 && !(d.flags() & 0x2)) {
    WaitForFence(base, device, d.fence() - 2, 0, kick);
    d.set_flags(d.flags() | 0x2);
  }
  return d.write_position();
}

uint32_t KickSegment(uint8_t* base, uint32_t device, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229EDA8 Device::KickSegment");
  const schema::Device d(base, device);
  uint32_t r3 = device;
  const uint32_t start = d.segment_start();
  uint32_t end = d.write_position() + 4;
  if (!(d.flags() & 0x40)) {
    if (d.tiling_end() != 0) {
      // Predicated tiling: the segment appended to the chain, kicked per
      // tile by the tiling worker.
      const uint32_t words = uint32_t(int32_t(end - start) >> 2);
      if (words != 0) {
        const uint32_t chain = d.segment_chain_first_address();
        const schema::CommandChain c(base, chain);
        r3 = chain;
        uint32_t cursor = c.cursor();
        if (cursor + 8 > c.end()) {
          cursor = NewBlock(base, chain);
          r3 = cursor;
        }
        guest::StoreU32(base, cursor + 4, PhysicalAddress(start));
        guest::StoreU32(base, cursor, words | 0x81000000u);
        c.set_cursor(cursor + 8);
      }
    } else {
      const uint32_t tail = WriteSegmentTail(base, device, end);
      end = kick(device, tail, PhysicalAddress(start), uint32_t(int32_t(tail - start) >> 2), 0,
                 d.segment_chain_first_address());
      r3 = end;
    }
  }
  const uint32_t next = (end + 31) & ~31u;
  if (next > d.soft_end()) {
    d.set_write_position(end - 4);
    return NextSegment(base, device, 0, kick);
  }
  d.set_segment_start(next);
  d.set_write_position(next - 4);
  if (d.tap().address != 0) {
    const uint32_t cursor = d.data_cursor();
    if (cursor != 0) {
      d.set_data_cursor((cursor + 4095) & ~4095u);
    }
  }
  return r3;
}

uint32_t NextSegment(uint8_t* base, uint32_t device, uint32_t words, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229EC00 Device::NextSegment");
  const schema::Device d(base, device);
  auto out_of_space = [&] {
    d.set_flags(d.flags() | 0x40);
  };
  uint32_t write;
  uint32_t end;
  if (d.flags() & 0x40) {
    out_of_space();
    write = d.dummy_segment().address;
    end = write + 4800;
    d.set_write_position(write);
    d.set_segment_end(end);
    d.set_soft_end(end - 160);
    return 0;
  }
  uint32_t size = words != 0 ? (words + 14) << 2 : 0;
  const uint32_t segment = NextSpace(base, device, 2, size, 32);
  if (segment == 0) {
    out_of_space();
    write = d.dummy_segment().address;
    end = write + 4800;
  } else {
    write = segment - 4;
    d.set_segment_start(segment);
    d.set_data_cursor(0);
    d.set_segment_data(0);
    end = (((size >> 2) - 14) << 2) + write;
  }
  d.set_write_position(write);
  d.set_segment_end(end);
  d.set_soft_end(end - 160);
  // The frame throttle: the last Swap's fence.
  const uint32_t fence =
      (d.options() & 1) ? d.throttle_fence2_address() : d.throttle_fence_address();
  WaitForFence(base, device, guest::LoadU32(base, fence), 3, kick);
  guest::StoreU32(base, fence, 0);
  return segment;
}

uint32_t WaitForFence(uint8_t* base, uint32_t device, uint32_t fence, uint32_t code,
                      const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229E908 Device::WaitForFence");
  const schema::Device d(base, device);
  (void)code;
  if (fence == 0) {
    return device;
  }
  const uint32_t open = d.fence();
  const uint32_t done =
      d.progress_block().fence_done();
  if (open - fence >= open - done) {
    return device;
  }
  uint32_t r3 = device;
  if (fence == open) {
    if (d.tiling_end() != 0) {
      return device;
    }
    r3 = EndSegment(base, device, kick);
  }
  // Still pending: the original waits for the GPU here (Dropped).
  return r3;
}

uint32_t ReserveWords(uint8_t* base, uint32_t device, uint32_t words, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229F148 Device::ReserveWords");
  const schema::Device d(base, device);
  const uint32_t bytes = words << 2;
  auto short_of_room = [&] {
    return d.write_position() + bytes >
           d.segment_end();
  };
  if (short_of_room()) {
    EndSegment(base, device, kick);
    if (short_of_room() && NextSegment(base, device, words, kick) == 0) {
      return 0;
    }
  }
  return d.write_position();
}

uint32_t InsertCallback(uint8_t* base, uint32_t device, uint32_t flags, uint32_t function,
                        uint32_t argument, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229F228 Device::InsertCallback");
  const schema::Device d(base, device);
  uint32_t position = d.write_position();
  if (position > d.soft_end()) {
    position = EndSegment(base, device, kick);
  }
  position = WriteCallback(base, device, position, flags, function, argument);
  d.set_write_position(position);
  return position;
}

uint32_t WriteIdlePackets(uint8_t* base, uint32_t device, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229F0E0 Device::WriteIdlePackets");
  const schema::Device d(base, device);
  if (d.write_position() > d.soft_end()) {
    EndSegment(base, device, kick);
  }
  // The WAIT_UNTIL and register 0xD04's packets: Dropped.
  return d.write_position();
}

uint32_t WaitForIdle(uint8_t* base, uint32_t device, const Kick& kick) {
  ITC_NATIVE_RUNNING("sub_8229F1B0 Device::WaitForIdle");
  const schema::Device d(base, device);
  if (d.write_position() > d.soft_end()) {
    EndSegment(base, device, kick);
  }
  // The WAIT_UNTIL packet: Dropped.
  const uint32_t r3 = WaitForFence(base, device, d.fence(), 4, kick);
  // The spin until no kick is pending (+10868, always 0): Dropped.
  return r3;
}

}  // namespace graphics::command_buffer
