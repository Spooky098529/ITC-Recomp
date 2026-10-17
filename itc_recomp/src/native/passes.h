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
#include "state.h"
#include "textures.h"
#include "vulkan_functions.h"

namespace native {

class Passes {
 public:
  Passes();
  ~Passes();

  // base: the guest memory base. resolution_scale: every target and resolve
  // destination is drawn this many times its guest size (see "Resolution"
  // in docs/native-renderer.md).
  bool Init(const DeviceContext& context, uint8_t* base, uint32_t frames_in_flight,
            float resolution_scale);

  // Render thread. Before a frame's first command.
  void BeginFrame(uint32_t frame_index);
  // After a draw command's library call (the device's shadow is complete).
  void AfterDraw(VkCommandBuffer commands, uint8_t* base, uint32_t id, uint32_t payload,
                 uint32_t arg1);
  // A draw of vertices given inline (DrawPrimitiveUP, command 19, and the
  // helper object's draws, command 15): at the library's End
  // (sub_825BDDA0), when the vertices have been copied to `data` (a guest
  // address) and the device's shadow holds the draw's state. type: the
  // primitive type; count: vertices; stride in bytes.
  void InlineDraw(VkCommandBuffer commands, uint8_t* base, uint32_t type, uint32_t count,
                  uint32_t stride, uint32_t data);
  // After a clear (26) or resolve (25) command's library call.
  void AfterClear(VkCommandBuffer commands, uint8_t* base, uint32_t payload, uint32_t arg1);
  void AfterResolve(VkCommandBuffer commands, uint8_t* base, uint32_t payload, uint32_t arg1);
  // Predicated tiling (see "Predicated tiling" in docs/gpu-functions.md):
  // after command 32 (BeginTiling); each Resolve the library's EndTiling
  // (command 33) makes, tile of tiles, with its arguments (guest addresses
  // of the rectangle, point and clear colour, 0 if none); after command 33.
  void BeginTiling(VkCommandBuffer commands, uint8_t* base, uint32_t payload);
  void TiledResolve(VkCommandBuffer commands, uint8_t* base, uint32_t device, uint32_t flags,
                    uint32_t rect, uint32_t destination, uint32_t point, uint32_t clear_color,
                    float clear_z, uint32_t tile, uint32_t tiles);
  void EndTiling();
  // The frame's end (sub_8258D998, outside the command list): the game
  // resolves render target 0 of `device` whole into a front buffer texture
  // (`front_buffer`, its fetch constant at +28). That target is then shown.
  void FrontBufferResolve(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                          uint32_t front_buffer);
  // Ends the frame's drawing and puts what's to be shown into the image
  // `shown()` returns, in TRANSFER_SRC_OPTIMAL: the frame resolved to the
  // front buffer (else the main view, as in frames without one), or with
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
  // A diagnostic: log the glow history's cross-fades and write-backs once a
  // second, by logic and render-only frames.
  void set_log_glow(bool log);
  std::vector<uint64_t> main_view_pixel_shaders() const;
  // A debugging aid: hash each vertex and index range every frame, keeping
  // how often each changes between frames; TakeMirrorChanges describes the
  // ranges that changed since it was last called (the most changed first).
  void set_check_mirror_frames(bool check);
  // Debugging aids: copy every vertex and index range into the mirror every
  // frame (as before memcpy's writes were tracked); compare each range not
  // copied with guest memory (Stats::mirror_stale; slow).
  void set_copy_all(bool copy_all);
  void set_skip_repeated_state(bool skip);
  void set_reuse_constants(bool reuse);
  // Counts why constants aren't reused (Stats::fetch_changed and
  // offset_differs; a diagnostic, off by default).
  void set_log_constant_changes(bool log);
  void set_log_register_reads(bool log);
  // A diagnostic: how often each draw's index range is scanned again
  // unchanged, and how the mirror's checks go; TakeIndexRanges describes
  // them since it was last called.
  // Keep draws' index ranges' lowest and highest index until memcpy writes
  // them (on by default).
  void set_cache_index_ranges(bool cache);
  void set_log_index_ranges(bool log);
  std::string TakeIndexRanges();
  // A diagnostic (docs/tick-replay.md): whether each render-only frame's
  // draws are the same as its tick's first frame's, part by part (target,
  // pipeline, textures, vertex fetch, indices, dynamic state, inline
  // vertices, each constant block); TakeTickRepeats describes it since it
  // was last called.
  void set_log_tick_repeats(bool log);
  // --native_renderer_tick_replay (docs/tick-replay.md, step 2): a tick's
  // first render-only frame is stored as it's drawn (capturing_frame;
  // NoteUpload gets the frame's constant uploads as recorded), and
  // DrawStoredFrame draws it again in place of a frame the main thread
  // didn't draw, with the frame's blends. TakeStoredFrames describes it
  // since it was last called.
  void set_tick_replay(bool on);
  bool capturing_frame() const;
  void NoteUpload(uint8_t* base, uint32_t payload, uint32_t arg1, uint32_t packed);
  void DrawStoredFrame(VkCommandBuffer commands, uint8_t* base);
  std::string TakeStoredFrames();
  // Draws translated once a tick (docs/tick-replay.md, step 1): on the
  // frames after a tick's first, a draw made from the same state as the one
  // at its place on that frame reuses what it came to, packing only its
  // constants again; with check, such a draw is worked out in full too and
  // the two compared. TakeTickDraws describes it since it was last called.
  void set_tick_draws(bool on, bool check);
  std::string TakeTickDraws();
  std::string TakeTickRepeats();
  // A diagnostic: how much of texture binding repeats from draw to draw
  // (to judge caching it); TakeTextureBindings describes it since it was
  // last called.
  void set_log_texture_bindings(bool log);
  // Reuse each stage's last texture set when nothing it was built from
  // changed (on by default).
  void set_reuse_texture_sets(bool reuse);
  std::string TakeTextureBindings();
  // A diagnostic: how much of the shader lookup repeats from draw to draw,
  // and whether a key of what draws read from the shader objects would
  // ever give a shader whose microcode changed (to judge caching by it);
  // TakeShaderLookups describes it since it was last called.
  // Keep draws' shader lookups by what they read from the shader objects
  // instead of hashing the microcode (on by default).
  void set_cache_shader_lookups(bool cache);
  void set_log_shader_lookups(bool log);
  std::string TakeShaderLookups();
  // Fixed at start (--native_renderer_mirror_by_tick): draws read a mirror
  // updated only as a logic pass's list starts replaying (an experiment);
  // TakeMirrorByTick describes it since it was last called.
  void set_mirror_by_tick(bool by_tick);
  // With it (--native_renderer_mirror_to_both): memcpy's writes also go into
  // the front mirror at each frame's start.
  void set_mirror_to_both(bool to_both);
  std::string TakeMirrorByTick();
  void set_verify_mirror(bool verify);
  // A debugging aid: hash each copied vertex and index range again at the
  // end of the frame; TakeLateWrites describes those that changed.
  void set_check_late_writes(bool check);
  std::string TakeLateWrites();
  // A debugging aid: find copies into the mirror that change bytes an
  // earlier draw's copy this frame put there (that draw then reads the new
  // ones); TakeClobbers describes them.
  void set_check_clobbers(bool check);
  std::string TakeClobbers();
  // Examples of draws counted in Stats::declaration_mismatches.
  std::vector<std::string> TakeDeclarationLines();
  std::string TakeMirrorChanges();
  // A debugging aid: compare each main view draw's inputs with the same
  // draw's in the previous frame; TakeWatchLines gives how many changed (of
  // how many watched) and descriptions of the first since the last call.
  void set_watch_draws(bool watch, uint64_t pixel_shader = 0);
  // A debugging aid: cube maps get the white placeholder.
  void set_hide_cubes(bool hide);
  void set_no_index_clamp(bool off);
  // Which pixel shaders got PsParamGen, and with which interpolators.
  std::string TakeInterfaces();
  void set_base_level_only(bool base_only);
  // A debugging aid: the texture cache's set_verify_every_frame.
  void set_verify_textures(bool every_frame);
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
    // Failed draws by reason (kFailedDrawReasons in renderer.cpp).
    std::array<uint64_t, 8> failed_by{};
    // Ranges not copied (unchanged since this mirror got them); with
    // set_verify_mirror, those whose contents differ from guest memory, and
    // an example.
    uint64_t mirror_skipped_bytes = 0, mirror_checked = 0, mirror_stale = 0,
             mirror_stale_bytes = 0;
    uint32_t mirror_stale_sample_address = 0, mirror_stale_sample_size = 0;
    bool mirror_stale_sample_index = false;
    // With set_check_mirror: ranges used again in a frame with other
    // contents, and an example.
    uint64_t mirror_rewritten = 0, mirror_rewritten_bytes = 0;
    uint32_t mirror_rewritten_sample_address = 0, mirror_rewritten_sample_size = 0;
    uint32_t pipelines = 0, shaders_translated = 0, shaders_failed = 0, targets = 0;
    uint32_t shaders_from_disk = 0;
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
    // Draws with indices outside VGT_MIN/MAX_VTX_INDX (the shader clamps
    // them), and an example: lowest and highest index, the registers.
    uint64_t clamped_draws = 0, param_gen_draws = 0;
    // Draws whose vertex shader's microcode, when read, was patched for
    // another vertex declaration than the bound one, and an example (the
    // shader's declaration id, the bound one's).
    // Of those, draws whose library call patched a vertex shader.
    uint64_t declaration_mismatches = 0, declaration_mismatches_patched = 0;
    // Draws given the vertex shader copy the library patched in the
    // command buffer (the object's own being patched for another declaration).
    uint64_t vertex_shader_copies = 0;
    // Inline vertex draws drawn (also counted in drawn), and by primitive
    // type (4, 6, 8 rectangle list, 13 quad list).
    uint64_t inline_draws = 0;
    std::array<uint64_t, 16> inline_by_type{};
    // Draws compared with the draw before them (nothing else recorded in
    // between), and how many repeated each of its state calls: viewport,
    // scissor, depth bias, blend constants, pipeline, descriptor sets (their
    // dynamic offsets aside), index buffer binding, the index buffer and
    // type alone (another offset), and descriptor sets with their offsets.
    uint64_t state_compared = 0;
    std::array<uint64_t, 9> state_repeated{};
    // Draws given constants, and how many pointed at the last copy of each
    // block (system, vertex floats, pixel floats, bool/loop, fetch) instead
    // of writing it; the bytes of constants written.
    uint64_t constants_draws = 0, constant_bytes = 0;
    std::array<uint64_t, 5> constants_reused{};
    // Draws whose fetch block differed from the last one written, and of
    // those, how many differed in a texture fetch constant their shaders
    // read, in a vertex fetch constant they read (and in its address
    // alone), or in neither (only unread slots).
    // With log_constant_changes: draws repeating the last draw's
    // descriptor sets but not their offsets: how many had each block's offset differ, and the fetch
    // block's alone.
    std::array<uint64_t, 5> offset_differs{};
    uint64_t offsets_differ = 0, only_fetch_offset_differs = 0;
    uint64_t fetch_changed = 0, fetch_changed_texture = 0, fetch_changed_vertex = 0,
             fetch_changed_vertex_address = 0, fetch_changed_unread = 0;
    std::array<uint32_t, 2> declaration_sample{};
    std::array<uint32_t, 4> clamped_sample{};
    std::array<uint32_t, 2> base_vertex_sample{};
    std::array<uint32_t, 5> vertex_overrun_sample{};
    uint64_t clears = 0, clear_rects = 0;
    // Resolves done; skipped (no source or destination, a level or slice
    // other than 0, a destination format we can't draw into); flags with
    // bits we don't know, and an example.
    uint64_t resolves = 0, resolves_skipped = 0, resolve_unknown_flags = 0;
    // Predicated tilings begun, and the resolves their ends made.
    uint64_t tilings = 0, tiled_resolves = 0;
    uint32_t resolve_flags_sample = 0;
    // With log_register_reads.
    RegisterReadStats register_reads;
    TextureCache::Stats textures;
  };
  Stats TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
};

}  // namespace native
