// The XMA frame walk (src/hooks/xma.cpp, audio/xma_packets.h) against the
// recompiled sub_8246A5F8 (sample to bit offset), on made-up XMA data:
// packets with random frame counts, first-frame offsets and frame lengths,
// some of them other streams' packets to skip. The other XMA helpers answer
// from our own decoding and differ from the recompiled ones by design: an
// attract race and the user's listening check them.

#include "harness.h"

#include "audio/xma_packets.h"

#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <fmt/format.h>

#include <vector>

namespace harness {

namespace {

constexpr uint32_t kPackets = 24;
constexpr uint32_t kPacketBits = 2048 * 8;

void PutBits(std::vector<uint8_t>& data, uint32_t bit, uint32_t value, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t b = bit + i;
    const uint8_t mask = uint8_t(0x80 >> (b & 7));
    if ((value >> (count - 1 - i)) & 1) {
      data[b >> 3] |= mask;
    } else {
      data[b >> 3] &= uint8_t(~mask);
    }
  }
}

// Packets of `streams` interleaved streams; returns stream 0's frame count.
uint32_t MakeData(Random& random, std::vector<uint8_t>& data, uint32_t streams) {
  data.assign(kPackets * 2048, 0);
  for (auto& b : data) {
    b = uint8_t(random.Next());
  }
  uint32_t frames = 0;
  for (uint32_t p = 0; p < kPackets;) {
    const uint32_t skip = streams > 1 ? random.Below(streams) : 0;
    const uint32_t count = random.Below(12);
    const uint32_t first = random.Below(400);
    uint32_t bit = 32 + first;
    for (uint32_t f = 0; f < count; f++) {
      const uint32_t length = 200 + random.Below(1000);
      if (bit + length + 32 >= kPacketBits) {
        break;
      }
      PutBits(data, p * kPacketBits + bit, length, 15);
      bit += length;
    }
    const uint32_t header = count << 26 | first << 11 | skip;
    for (uint32_t i = 0; i < 4; i++) {
      data[p * 2048 + i] = uint8_t(header >> (24 - 8 * i));
    }
    frames += count;
    p += 1 + skip;
  }
  return frames;
}

}  // namespace

void Xma(Context& h) {
  Results results("XMA frame walk (sub_8246A5F8)");
  auto* memory = rex::system::kernel_state()->memory();
  const uint32_t guest_data = memory->SystemHeapAlloc(kPackets * 2048);
  std::vector<uint8_t> data;
  for (uint32_t set = 0; set < 8; set++) {
    const uint32_t streams = 1 + set % 3;
    const uint32_t frames = MakeData(h.random, data, streams);
    Write(h.base, guest_data, data.data(), uint32_t(data.size()));
    for (uint32_t i = 0; i < 64; i++) {
      const uint32_t sample = i < 4 ? i * 511 : h.random.Below((frames + 4) * 512);
      const uint32_t stream = h.random.Below(2);
      Both(h, results, [] {},
           [&](PPCContext& t) {
             t.r3.u64 = guest_data;
             t.r4.u64 = uint32_t(data.size());
             t.r5.u64 = stream;
             t.r6.u64 = sample;
           },
           __imp__sub_8246A5F8, sub_8246A5F8, h.data, 0,
           fmt::format("{} streams, stream {}, sample {}", streams, stream, sample));
    }
  }
  memory->SystemHeapFree(guest_data);
  results.Report();
}

}  // namespace harness
