#include "audio/xma_packets.h"

namespace audio::xma {

namespace {

uint32_t LoadBe32(const uint8_t* p) {
  return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
}

// A frame's length: the 15 bits at `bit` from the packet's start (read as
// the game does: the word at bit / 8, shifted by bit % 8).
uint32_t FrameLength(const uint8_t* packet, uint32_t bit) {
  return (LoadBe32(packet + (bit >> 3)) << (bit & 7)) >> 17;
}

}  // namespace

PacketHeader ReadPacketHeader(const uint8_t* packet) {
  const uint32_t word = LoadBe32(packet);
  const uint32_t offset = (word >> 11) & 0x7FFF;
  return {word >> 26, offset + 32, offset != 0x7FFF, word & 0xFF};
}

uint32_t CountFrames(const uint8_t* data, size_t size) {
  uint32_t frames = 0;
  for (size_t at = 0; at + kPacketSize <= size;) {
    const PacketHeader header = ReadPacketHeader(data + at);
    frames += header.frame_count;
    at += kPacketSize * (1 + header.packets_to_skip);
  }
  return frames;
}

uint32_t BitOffsetOfSample(const uint8_t* data, uint32_t size, uint32_t stream, uint32_t sample) {
  const uint32_t target = sample / kSamplesPerFrame;
  uint32_t at = stream * uint32_t(kPacketSize);
  uint32_t frames = 0;
  while (at < size) {
    const uint32_t word = LoadBe32(data + at);
    const uint32_t count = word >> 26;
    if (target < frames + count) {
      uint32_t bit = ((word >> 11) & 0x7FFF) + 32;
      for (uint32_t i = frames; i < target; i++) {
        bit += FrameLength(data + at, bit);
      }
      return at * 8 + bit;
    }
    frames += count;
    at += uint32_t(kPacketSize) * ((word & 0xFF) + 1);
  }
  return 0;
}

std::optional<uint32_t> FrameAtBitOffset(const uint8_t* data, uint32_t size, uint32_t stream,
                                         uint32_t bit) {
  uint32_t at = stream * uint32_t(kPacketSize);
  uint32_t frames = 0;
  while (at < size) {
    const uint32_t word = LoadBe32(data + at);
    const uint32_t count = word >> 26;
    if (bit < (at + uint32_t(kPacketSize)) * 8 && count) {
      uint32_t frame_bit = ((word >> 11) & 0x7FFF) + 32;
      for (uint32_t i = 0; i < count; i++) {
        if (at * 8 + frame_bit == bit) {
          return frames + i;
        }
        frame_bit += FrameLength(data + at, frame_bit);
      }
    }
    frames += count;
    at += uint32_t(kPacketSize) * ((word & 0xFF) + 1);
  }
  return std::nullopt;
}

}  // namespace audio::xma
