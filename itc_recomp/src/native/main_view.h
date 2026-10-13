// Milestone 2 of the native renderer (docs/native-renderer.md): draws the
// race's main view (the 1024x576, 2x MSAA, 7e3 render target; see "A race
// frame's passes" in docs/gpu-functions.md), textured, into a target of
// our own, which the renderer copies to its window.

#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "device_context.h"
#include "textures.h"
#include "vulkan_functions.h"

namespace native {

class MainView {
 public:
  MainView();
  ~MainView();

  // base: the guest memory base (guest physical memory is imported from it).
  bool Init(const DeviceContext& context, uint8_t* base, uint32_t frames_in_flight);

  // Render thread. Before a frame's first draw.
  void BeginFrame(uint32_t frame_index);
  // After a draw command's library call (the device's shadow is complete).
  void AfterDraw(VkCommandBuffer commands, uint8_t* base, uint32_t id, uint32_t payload,
                 uint32_t arg1);
  // Ends the frame's drawing; leaves the target in TRANSFER_SRC_OPTIMAL.
  void EndFrame(VkCommandBuffer commands);
  // The frame's texture uploads, to be submitted before its commands; null
  // if none.
  VkCommandBuffer EndUploads();

  VkImage target() const;
  VkExtent2D target_extent() const;

  // Logged by the renderer.
  struct Stats {
    uint64_t drawn = 0, other_passes = 0, unsupported = 0, failed = 0, mirrored_bytes = 0;
    uint32_t pipelines = 0, shaders_translated = 0, shaders_failed = 0;
    // Draws skipped as having an empty viewport or scissor, and one example:
    // viewport offset and extent, scissor offset and extent (bit 31 of the
    // first word set when filled).
    uint64_t invisible = 0;
    std::array<uint32_t, 8> invisible_sample{};
    TextureCache::Stats textures;
  };
  Stats TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
};

}  // namespace native
