// Milestone 3 of the native renderer (docs/native-renderer.md): textures
// from the texture fetch constants, read from guest memory, untiled and
// byte-swapped into Vulkan images, and samplers. Our own code (layout,
// untiling and format mapping as the 360's GPU stores textures; see
// "Textures" in docs/gpu-functions.md), not the SDK's texture cache.

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "device_context.h"
#include "render/state.h"
#include "vulkan_functions.h"

namespace gpu::vulkan {
class Backend;
}

namespace native {

class TextureCache {
 public:
  TextureCache();
  ~TextureCache();

  bool Init(gpu::vulkan::Backend& backend, uint32_t frames_in_flight);

  // Render thread. After the frame's fence has been waited on.
  void BeginFrame(uint32_t frame_index);

  // The fetch constant's texture as a view of the given type, with the
  // fetch constant's swizzle; loaded as needed, and reloaded after the
  // library's memcpy writes its memory. Null if the texture can't be had (unsupported format or
  // dimension, view type mismatch, staging full): use a placeholder.
  // fetch: the 6 words, in host order. Both answer from a table of earlier
  // answers by the same words while the generation (below) holds
  // (--native_renderer_cache_texture_bindings), so Stats count only the
  // calls that weren't answered from it. Before the table, each fetch
  // constant's last answer (fetch_constant: its index, 0-31).
  VkImageView GetView(const uint32_t fetch[6], VkImageViewType view_type,
                      uint32_t fetch_constant);
  // A sampler as the stage's state describes it (render::StageSampler), for
  // the texture in `fetch` (its level range; a cube map clamps to the edge).
  VkSampler GetSampler(const render::SamplerState& state, const uint32_t fetch[6]);
  // The post-swizzle signedness of each component, 2 bits each (the
  // translator's swizzled signs byte; the shaders no longer apply them:
  // a texture with signs is logged).
  static uint8_t SwizzledSigns(const uint32_t fetch[6]);

  // A resolve's destination texture (fetch: its fetch constant, addresses
  // physical): an image of its size and format, made on first use at
  // `scale` times its size (the resolution scale of the target first
  // resolved into it; see "Resolution" in docs/native-renderer.md), which
  // draws sampling a texture at its base address then get instead of
  // guest memory (the emulated GPU doesn't write resolves back). Left in
  // SHADER_READ_ONLY_OPTIMAL, or UNDEFINED while `fresh`. False if its
  // format can't be a color attachment.
  struct ResolveDestination {
    gpu::Image* object;  // its image (Use: undefined while fresh)
    VkImage image;
    VkImageView view;  // all components as is, for drawing into
    VkFormat format;
    uint32_t width, height;            // the guest's
    uint32_t host_width, host_height;  // the image's (scaled)
    float scale;                       // the scale it was made at
    bool fresh;
    // It holds a pass's image handed over (HandOver), not its own.
    bool handed;
    VkFormat native_format;  // its own image's format
    // A resolve has copied into it (RestoreResolveDestination): it isn't
    // handed images (a partial copy keeps what's outside its rectangle).
    bool copied;
  };
  bool GetResolveDestination(const uint32_t fetch[6], float scale, ResolveDestination& out);

  // A pass's image handed to a resolve destination in place of a copy
  // (docs/pc-renderer.md, step 4): the pass has drawn into its texture.
  // The image, its memory and its view (all of it, in `format`) become the
  // destination's (GetResolveDestination made it first), left in
  // SHADER_READ_ONLY_OPTIMAL by the caller. What the copy did is left to
  // the readers: red and blue exchanged by the views' swizzle
  // (swap_red_blue), and reader_scale (ReaderScale). Returns the image the
  // destination held when it's of `format` (a pass's image handed over
  // before: the target draws into it next), else retires it and returns
  // none.
  struct HandedImage {
    // The backend's image (its image, memory and views below are its).
    gpu::Image* object = nullptr;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkFormat format = VK_FORMAT_UNDEFINED;
    // A depth target's (its depth sampled, as the resolve's 24_8 copy
    // holds): the aspect its views take, and its depth-only view.
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageView depth_view = VK_NULL_HANDLE;
  };
  HandedImage HandOver(const uint32_t fetch[6], float scale, const HandedImage& image,
                       bool swap_red_blue, float reader_scale);
  // Before a resolve copies into the destination: marked as copied into
  // from now on, and if it holds a handed image, made its own again (a new
  // image of its format); `out` as from GetResolveDestination.
  bool RestoreResolveDestination(const uint32_t fetch[6], float scale, ResolveDestination& out);
  // Whether the destination still holds `image` (handed over).
  bool HoldsHanded(const uint32_t fetch[6], float scale, const gpu::Image* image) const;
  // What a draw's sampling of the texture applies (xe_interface.glsl's
  // xe_texture_result): 0 nothing; above 0 times it, clamped to 0-1 (what
  // the resolve's exponent bias and a normalized destination did); below 0
  // times its negation.
  float ReaderScale(const uint32_t fetch[6]) const;

  // Diagnostics: whether [start, end) (physical) overlaps a texture held.
  bool Holds(uint32_t start, uint32_t end) const;
  // A debugging aid: the texture's first level (first layer) decoded from
  // guest memory into the host format's bytes, uncompressed formats only.
  // With --readback_resolve=full, a resolve destination's memory holds what
  // the emulated GPU resolved.
  static bool DecodeGuest(const uint32_t fetch[6], std::vector<uint8_t>& out,
                          VkFormat& format, uint32_t& width, uint32_t& height);

  // A debugging aid: makes GetView give nothing (so placeholders) for some
  // textures: from guest memory (all, or of one format), or resolve
  // destinations (all, or at one address).
  enum class Hide { kNone, kMemory, kMemoryFormat, kResolved, kResolvedAddress };
  // A debugging aid: samplers use only each texture's first level.
  void set_base_level_only(bool base_only) {
    if (base_only != base_level_only_) {
      sampler_generation_++;
    }
    base_level_only_ = base_only;
  }
  // A debugging aid: hash every texture used each frame, to check that the
  // library's memcpy is how textures' memory changes (a change without a
  // recorded write is counted and reloaded).
  void set_verify_every_frame(bool every) { verify_every_frame_ = every; }
  void set_debug_hide(Hide hide, uint32_t value = 0) {
    if (hide != debug_hide_ || value != debug_hide_value_) {
      generation_++;
    }
    debug_hide_ = hide;
    debug_hide_value_ = value;
  }

  // Bumped whenever GetView's result for some fetch constant may change
  // without its words changing, or a call may have work to do (a texture
  // marked written, to reload; a resolve changing which image an address
  // samples; the debug hide), so results kept from before must be asked
  // again. Not for first loads or new textures: those happen only on a
  // fetch constant's first use.
  uint64_t generation() const { return generation_; }
  // The same for samplers (bumped when base_level_only changes).
  uint64_t sampler_generation() const { return sampler_generation_; }
  // GetView answers not kept in the table so far (null, a
  // reload deferred, or the table off): a caller keeping what it built from
  // answers can tell whether all of them would be asked again.
  uint64_t unkept_answers() const { return unkept_answers_; }
  // What bumped it, and textures made, since the last call (a diagnostic).
  struct GenerationEvents {
    uint64_t marked_frames = 0, resolve_changes = 0, created = 0;
  };
  GenerationEvents TakeGenerationEvents();

  // XPR files' events, at the frame's start (TextureCache_XprLoaded).
  void ProcessXprs();

  struct Stats {
    uint64_t loaded = 0, reloaded = 0, loaded_bytes = 0, unsupported = 0, mismatched = 0,
             deferred = 0;
    // Lookups answered with a resolve destination; of those, ones whose
    // fetch constant has another size or format than the resolve made.
    uint64_t resolved_hits = 0, resolved_mismatched = 0;
    // memcpy's writes into the physical views, and textures they marked to be
    // reloaded.
    uint64_t writes = 0, marked = 0;
    // With verify_every_frame: textures whose memory changed with no write
    // recorded, and an example: its address and how many frames after it was
    // first loaded.
    uint64_t unrecorded = 0;
    uint32_t unrecorded_sample = UINT32_MAX, unrecorded_frames = 0;
    uint32_t textures = 0, samplers = 0;
    uint32_t unsupported_sample = UINT32_MAX;  // format << 8 | dimension
    // A texture of another dimension than the shader declares: declared view
    // type << 16 | format << 8 | dimension, and its size.
    uint32_t mismatched_sample = UINT32_MAX, mismatched_width = 0, mismatched_height = 0;
  };
  Stats TakeStats();
  // Lookups so far that found a texture of another dimension than declared
  // (not reset by TakeStats).
  uint64_t mismatched_total() const;

 private:
  struct State;
  // GetView worked out in full; `cacheable`: whether the
  // answer may be kept until the generation changes.
  VkImageView LookUpView(const uint32_t fetch[6], VkImageViewType view_type,
                         bool& cacheable);
  // The sampler made from `desc`, kept by its fields.
  VkSampler SamplerFor(const gpu::SamplerDesc& desc);
  std::unique_ptr<State> state_;
  bool cache_bindings_ = true;
  bool log_writes_ = false;
  uint64_t sampler_generation_ = 0;
  uint64_t unkept_answers_ = 0;
  Hide debug_hide_ = Hide::kNone;
  bool base_level_only_ = false;
  bool verify_every_frame_ = false;
  uint32_t debug_hide_value_ = 0;
  uint64_t generation_ = 0;
  uint64_t races_seen_ = 0;
  GenerationEvents events_;
};

// The game left a race (main thread): the texture cache frees its loaded
// textures at its next frame.
void TextureCache_RaceLeft();
// An XPR file loaded (its data block, guest address and size, and each
// texture's fetch constant as its header has it), and released: its
// textures are uploaded at a frame's start and its data's pages given back
// (docs/pc-renderer.md, step 2b). Any thread.
void TextureCache_XprLoaded(uint32_t data, uint32_t size,
                            std::vector<std::array<uint32_t, 6>> fetches);
void TextureCache_XprReleased(uint32_t data);

}  // namespace native
