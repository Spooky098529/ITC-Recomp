// The GPU data arena (docs/conventional-renderer.md, "Step 2b", part 2):
// one device-local buffer that draws read vertices from (set 0, by the
// offsets the fetch constants are given) and bind as their index buffer,
// in place of the guest memory mirrors. It holds the buffers made through
// the interface (each its own space, given by BufferTable), fixed data
// (the built-in index lists) and a region for the frame's inline vertices.
//
// The CPU never writes memory the GPU reads: data goes into this frame's
// staging, and the frame's upload command buffer (submitted before its
// drawing) copies it into the arena after a barrier on earlier
// submissions' reads, so a write in place waits for the frames in flight
// that draw from it. That's why one arena is enough where the mirrors
// needed one per frame in flight. Space freed is reused only after the
// frames in flight.

#pragma once

#include <cstdint>
#include <map>
#include <vector>

#include "device_context.h"
#include "vulkan_functions.h"

namespace native {

class BufferArena {
 public:
  // The buffers' space, then the fixed data, then the inline vertices.
  static constexpr uint32_t kSpaceSize = 128u << 20;
  static constexpr uint32_t kInlineSize = 16u << 20;

  // `fixed_bytes`: the fixed data's size (at fixed_base()).
  bool Init(const DeviceContext& context, uint32_t frames_in_flight, uint32_t fixed_bytes);
  VkBuffer buffer() const { return buffer_; }
  uint32_t fixed_base() const { return kSpaceSize; }
  uint32_t inline_base() const { return kSpaceSize + fixed_bytes_; }

  // Render thread, once the frame's slot is free (its fence passed).
  void BeginFrame(uint32_t frame_index);
  // Space for `size` bytes; false when the space is full (logged once).
  bool Allocate(uint32_t size, uint32_t& offset);
  // Given back after the frames in flight.
  void Free(uint32_t offset, uint32_t size);
  // `size` bytes from `data` to `offset`, before this frame's drawing;
  // false when this frame's staging is full.
  bool Upload(uint32_t offset, const void* data, uint32_t size);
  // This frame's copies, ended, to be submitted before its drawing; null if
  // none.
  VkCommandBuffer EndUploads();

  struct Stats {
    uint64_t uploads = 0, upload_bytes = 0, staging_full = 0, allocation_failed = 0;
    // The space in use now (rounded), and the most since the last call.
    uint64_t used = 0, most_used = 0;
  };
  Stats TakeStats();

 private:
  static constexpr VkDeviceSize kStagingSize = 64u << 20;
  static constexpr uint32_t kAlignment = 256;

  struct Frame {
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    uint8_t* staging_mapped = nullptr;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer uploads = VK_NULL_HANDLE;
  };
  struct Retired {
    uint64_t frame_number;
    uint32_t offset, size;
  };

  void Release(uint32_t offset, uint32_t size);

  DeviceContext context_{};
  VkBuffer buffer_ = VK_NULL_HANDLE;
  VkDeviceMemory memory_ = VK_NULL_HANDLE;
  uint32_t fixed_bytes_ = 0;
  std::vector<Frame> frames_;
  uint32_t frame_ = 0;
  uint64_t frame_number_ = 0;
  VkDeviceSize staging_used_ = 0;
  std::vector<VkBufferCopy> copies_;
  // Free space by offset (merged with its neighbors), and space freed but
  // maybe still read by a frame in flight, oldest first.
  std::map<uint32_t, uint32_t> free_;
  std::vector<Retired> retired_;
  bool full_logged_ = false;
  Stats stats_;
};

}  // namespace native
