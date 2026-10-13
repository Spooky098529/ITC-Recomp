// Milestones 2-4 of the native renderer (docs/native-renderer.md): draws
// every pass of a frame (see "A race frame's passes" in
// docs/gpu-functions.md), textured, each into a target of our own per
// bound surface; clears them, and turns resolves into draws into the
// destination textures' images, which later draws sample.

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include "device_context.h"
#include "textures.h"
#include "vulkan_functions.h"

namespace native {

class Passes {
 public:
  Passes();
  ~Passes();

  // base: the guest memory base.
  bool Init(const DeviceContext& context, uint8_t* base, uint32_t frames_in_flight);

  // Render thread. Before a frame's first command.
  void BeginFrame(uint32_t frame_index);
  // After a draw command's library call (the device's shadow is complete).
  void AfterDraw(VkCommandBuffer commands, uint8_t* base, uint32_t id, uint32_t payload,
                 uint32_t arg1);
  // After a clear (26) or resolve (25) command's library call.
  void AfterClear(VkCommandBuffer commands, uint8_t* base, uint32_t payload, uint32_t arg1);
  void AfterResolve(VkCommandBuffer commands, uint8_t* base, uint32_t payload, uint32_t arg1);
  // Ends the frame's drawing and puts what's to be shown into the image
  // `shown()` returns, in TRANSFER_SRC_OPTIMAL: the main view, or with
  // show_resolve >= 0 that resolve of the frame's destination (a debugging
  // aid). Returns the size of what was put there.
  // dump: also copy the frame's resolve destinations and what's shown
  // into memory, for WriteDump once the frame is done (a debugging aid).
  VkExtent2D EndFrame(VkCommandBuffer commands, int32_t show_resolve, bool dump);
  // After a frame that dumped has finished on the GPU: writes each image as
  // a PFM file into the directory (tools/pfm_to_png.py makes PNGs of them).
  void WriteDump(const std::string& directory);
  // How many resolves the last frame made (a race frame makes about 25).
  size_t frame_resolve_count() const;
  VkImage shown() const;
  // The frame's texture uploads, to be submitted before its commands; null
  // if none.
  VkCommandBuffer EndUploads();

  // Logged by the renderer.
  struct Stats {
    uint64_t drawn = 0, unsupported = 0, failed = 0, mirrored_bytes = 0;
    uint32_t pipelines = 0, shaders_translated = 0, shaders_failed = 0, targets = 0;
    // Draws skipped as having an empty viewport or scissor, and one example:
    // viewport offset and extent, scissor offset and extent (bit 31 of the
    // first word set when filled).
    uint64_t invisible = 0;
    std::array<uint32_t, 8> invisible_sample{};
    // Draws with render targets 1-3 bound to another surface than 0 (only 0
    // is drawn), or colour and depth of different sample counts.
    uint64_t extra_targets = 0, mismatched_samples = 0;
    uint64_t clears = 0, clear_rects = 0;
    // Resolves done; skipped (no source or destination, a level or slice
    // other than 0, a destination format we can't draw into); flags with
    // bits we don't know, and an example.
    uint64_t resolves = 0, resolves_skipped = 0, resolve_unknown_flags = 0;
    uint32_t resolve_flags_sample = 0;
    TextureCache::Stats textures;
  };
  Stats TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
};

}  // namespace native
