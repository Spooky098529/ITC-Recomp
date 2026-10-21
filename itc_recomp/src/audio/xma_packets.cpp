#include "audio/xma_packets.h"

namespace audio::xma {

PacketHeader ReadPacketHeader(const uint8_t* packet) {
  const uint32_t word = uint32_t(packet[0]) << 24 | uint32_t(packet[1]) << 16 |
                        uint32_t(packet[2]) << 8 | packet[3];
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

}  // namespace audio::xma
