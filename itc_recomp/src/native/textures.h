// Milestone 3 of the native renderer (docs/native-renderer.md): textures
// from the texture fetch constants, read from guest memory, untiled and
// byte-swapped into Vulkan images, and samplers. Our own code (layout,
// untiling and format mapping as the 360's GPU stores textures; see
// "Textures" in docs/gpu-functions.md), not the SDK's texture cache.

#pragma once

#include <cstdint>
#include <memory>

#include "device_context.h"
#include "vulkan_functions.h"

namespace native {

// A shader's sampler binding: the fetch constant and the instruction's
// filter overrides (xenos::TextureFilter, AnisoFilter values; 3 and 7 mean
// "from the fetch constant").
struct SamplerOverrides {
  uint32_t fetch_constant;
  uint32_t mag_filter, min_filter, mip_filter, aniso_filter;
};

class TextureCache {
 public:
  TextureCache();
  ~TextureCache();

  bool Init(const DeviceContext& context, uint32_t frames_in_flight);

  // Render thread. After the frame's fence has been waited on.
  void BeginFrame(uint32_t frame_index);

  // The fetch constant's texture as a view of the given type, with the
  // fetch constant's swizzle; loaded (or reloaded, if its memory changed)
  // as needed. Null if the texture can't be had (unsupported format or
  // dimension, view type mismatch, staging full): use a placeholder.
  // fetch: the 6 words, in host order.
  VkImageView GetView(uint8_t* base, const uint32_t fetch[6], VkImageViewType view_type);
  VkSampler GetSampler(const uint32_t fetch[6], const SamplerOverrides& overrides);
  // The post-swizzle signedness of each component, 2 bits each (the
  // translator's texture_swizzled_signs byte).
  static uint8_t SwizzledSigns(const uint32_t fetch[6]);

  // A resolve's destination texture (fetch: its fetch constant, addresses
  // physical): an image of its size and format, made on first use, which
  // draws sampling a texture at its base address then get instead of
  // guest memory (the emulated GPU doesn't write resolves back). Left in
  // SHADER_READ_ONLY_OPTIMAL, or UNDEFINED while `fresh`. False if its
  // format can't be a colour attachment.
  struct ResolveDestination {
    VkImage image;
    VkImageView view;  // all components as is, for drawing into
    VkFormat format;
    uint32_t width, height;
    bool fresh;
  };
  bool GetResolveDestination(const uint32_t fetch[6], ResolveDestination& out);

  // A debugging aid: makes GetView give nothing (so placeholders) for some
  // textures: from guest memory (all, or of one format), or resolve
  // destinations (all, or at one address).
  enum class Hide { kNone, kMemory, kMemoryFormat, kResolved, kResolvedAddress };
  // A debugging aid: samplers use only each texture's first level.
  void set_base_level_only(bool base_only) { base_level_only_ = base_only; }
  // A debugging aid: hash every texture used again each frame (not every
  // kVerifyInterval frames, a few a frame), to see whether textures replaced
  // in memory are what's shown wrong meanwhile.
  void set_verify_every_frame(bool every) { verify_every_frame_ = every; }
  void set_debug_hide(Hide hide, uint32_t value = 0) {
    debug_hide_ = hide;
    debug_hide_value_ = value;
  }

  // This frame's uploads, ended, to be submitted before its drawing; null if
  // none.
  VkCommandBuffer EndUploads();

  struct Stats {
    uint64_t loaded = 0, reloaded = 0, loaded_bytes = 0, unsupported = 0, mismatched = 0,
             deferred = 0;
    // Lookups answered with a resolve destination; of those, ones whose
    // fetch constant has another size or format than the resolve made.
    uint64_t resolved_hits = 0, resolved_mismatched = 0;
    // Textures whose memory changed within 240 frames of loading them (still
    // being streamed in when first used), and an example: its address and
    // how many frames after loading.
    uint64_t changed_after_load = 0;
    // Textures past their first frames found changed, and an example: its
    // address and how many frames since it was last checked.
    uint64_t changed_later = 0;
    uint32_t changed_later_sample = UINT32_MAX, changed_later_frames = 0;
    uint32_t changed_after_load_sample = UINT32_MAX, changed_after_load_frames = 0;
    uint32_t textures = 0, samplers = 0;
    uint32_t unsupported_sample = UINT32_MAX;  // format << 8 | dimension
    // A texture of another dimension than the shader declares: declared view
    // type << 16 | format << 8 | dimension, and its size.
    uint32_t mismatched_sample = UINT32_MAX, mismatched_width = 0, mismatched_height = 0;
  };
  Stats TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
  Hide debug_hide_ = Hide::kNone;
  bool base_level_only_ = false;
  bool verify_every_frame_ = false;
  uint32_t debug_hide_value_ = 0;
};

}  // namespace native
