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
#include <vector>

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
  // dump_prefix: the shown image's file name; dump_resolves: the resolves
  // too.
  VkExtent2D EndFrame(VkCommandBuffer commands, int32_t show_resolve, bool dump,
                      const std::string& dump_prefix = "shown", bool dump_resolves = true);
  // For the next frames: the texture cache's debug_hide (see TextureCache).
  void set_debug_hide(TextureCache::Hide hide, uint32_t value = 0);
  // A debugging aid: hash each vertex and index range again at every use,
  // counting ranges whose contents changed within a frame (stats).
  void set_check_mirror(bool check);
  // Debugging aids: skip the draws of a pixel shader (0: none); describe
  // each draw of the frames recorded, written with the next dump
  // (draws.txt); the pixel shaders the last frame drew the main view with.
  void set_skip_pixel_shader(uint64_t hash);
  // For set_skip_pixel_shader: skip the draws without a pixel shader.
  static constexpr uint64_t kSkipNoPixelShader = 1;
  void set_record_draws(bool record);
  std::vector<uint64_t> main_view_pixel_shaders() const;
  // A debugging aid: hash each vertex and index range every frame, keeping
  // how often each changes between frames; TakeMirrorChanges describes the
  // ranges that changed since it was last called (the most changed first).
  void set_check_mirror_frames(bool check);
  // A debugging aid: hash each copied vertex and index range again at the
  // end of the frame; TakeLateWrites describes those that changed.
  void set_check_late_writes(bool check);
  std::string TakeLateWrites();
  std::string TakeMirrorChanges();
  // A debugging aid: compare each main view draw's inputs with the same
  // draw's in the previous frame; TakeWatchLines gives how many changed (of
  // how many watched) and descriptions of the first since the last call.
  void set_watch_draws(bool watch, uint64_t pixel_shader = 0);
  // A debugging aid: cube maps get the white placeholder.
  void set_hide_cubes(bool hide);
  void set_base_level_only(bool base_only);
  std::vector<std::string> TakeWatchLines(uint64_t& changes, uint64_t& draws);
  // The distinct addresses of the last frame's resolve destinations.
  std::vector<uint32_t> resolve_addresses() const;
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
    // With set_check_mirror: ranges used again in a frame with other
    // contents, and an example.
    uint64_t mirror_rewritten = 0, mirror_rewritten_bytes = 0;
    uint32_t mirror_rewritten_sample_address = 0, mirror_rewritten_sample_size = 0;
    uint32_t pipelines = 0, shaders_translated = 0, shaders_failed = 0, targets = 0;
    // Draws skipped as having an empty viewport or scissor, and one example:
    // viewport offset and extent, scissor offset and extent (bit 31 of the
    // first word set when filled).
    uint64_t invisible = 0;
    std::array<uint32_t, 8> invisible_sample{};
    // Draws with render targets 1-3 bound to another surface than 0 (only 0
    // is drawn), or colour and depth of different sample counts.
    uint64_t extra_targets = 0, mismatched_samples = 0;
    // Draws with a depth bias, and with an "equal" depth test.
    uint64_t depth_biased = 0, depth_equal = 0;
    // Draws reading a vertex buffer past the size its fetch constant gives,
    // and an example: address, size, size needed, highest vertex, stride
    // in words.
    uint64_t vertex_overruns = 0;
    // Draws whose item's base vertex differs from VGT_INDX_OFFSET, and an
    // example (item's, register's).
    uint64_t base_vertex_differs = 0;
    std::array<uint32_t, 2> base_vertex_sample{};
    std::array<uint32_t, 5> vertex_overrun_sample{};
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
