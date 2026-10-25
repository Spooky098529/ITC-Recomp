// XMA2 data as the game's banks hold it (B2 in docs/rexglue-removal.md):
// 2 KB packets, each with a 32-bit header (big-endian) followed by frames;
// a frame starts with its length in bits (15 bits) and decodes to 512 samples
// a channel. The game's own walk over this is sub_8246A5F8
// (docs/functions/audio.md). Plain C++, no guest memory.

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

namespace audio::xma {

constexpr size_t kPacketSize = 2048;
constexpr uint32_t kSamplesPerFrame = 512;
constexpr uint32_t kSamplesPerSubframe = 128;

struct PacketHeader {
  uint32_t frame_count;      // frames starting in this packet (top 6 bits)
  uint32_t first_frame_bit;  // bit offset of the first frame from the packet's start
  bool has_frame_start;      // false when no frame starts here (offset 0x7FFF)
  uint32_t packets_to_skip;  // packets of other streams after this one (low 8 bits)
};

PacketHeader ReadPacketHeader(const uint8_t* packet);

// Frames in a single-stream buffer of whole packets.
uint32_t CountFrames(const uint8_t* data, size_t size);

// The bit offset (from the data's start) of the frame holding `sample`
// (frame sample / 512) of `stream`, as sub_8246A5F8 works it out: the
// stream's packets from packet `stream`, skipping other streams' packets,
// then that packet's frames by their 15-bit lengths. 0 past the end.
uint32_t BitOffsetOfSample(const uint8_t* data, uint32_t size, uint32_t stream, uint32_t sample);

// The other way: the index of the stream's frame starting at `bit`, if one
// does.
std::optional<uint32_t> FrameAtBitOffset(const uint8_t* data, uint32_t size, uint32_t stream,
                                         uint32_t bit);

}  // namespace audio::xma
