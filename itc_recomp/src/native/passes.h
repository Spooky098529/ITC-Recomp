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

#include "buffer_table.h"
#include "device_context.h"
#include "render/device.h"
#include "render/pass_plan.h"
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
  // Predicated tiling's end (command 33): see InterfaceBeginTiling.
  void EndTiling();
  // The conventional renderer (render/device.h, docs/conventional-renderer.md
  // step 1): draws, clears, resolves and tiling (see "Predicated tiling" in
  // docs/gpu-functions.md) from our interface's state, with the translator's
  // registers made from it (interface_registers.h). set_interface, before
  // the first frame, is the state everything binds from (FrontBufferResolve
  // and DrawMovie take render target 0 from it too).
  void set_interface(render::State* state);
  // Textures made, written and freed (render/resources.h), between lists:
  // the texture cache's table by handle.
  void TextureCreated(uint32_t handle);
  void TextureWritten(uint32_t handle);
  void TextureDestroyed(uint32_t handle);
  // Buffers made, written and freed (render/resources.h, step 2b): the
  // buffer table (buffer_table.h).
  void BufferCreated(uint32_t handle, const render::BufferData& data);
  void BufferWritten(uint32_t handle);
  void BufferDestroyed(uint32_t handle);
  // The frame's pass plan (render/pass_plan.h), before its list is
  // replayed: each resolve is checked against it (step 3).
  void PassPlanned(const render::PassPlan& plan);
  void InterfaceDraw(VkCommandBuffer commands, uint8_t* base, render::State& state,
                     const render::DrawIndexed& draw);
  void InterfaceInlineDraw(VkCommandBuffer commands, uint8_t* base, render::State& state,
                           const render::DrawInline& draw);
  void InterfaceClear(VkCommandBuffer commands, uint8_t* base, render::State& state,
                      const render::Clear& clear);
  void InterfaceResolve(VkCommandBuffer commands, uint8_t* base, render::State& state,
                        const render::Resolve& resolve);
  void InterfaceBeginTiling(VkCommandBuffer commands, uint8_t* base, render::State& state,
                            uint32_t width, uint32_t height, float clear);
  // A movie's frame (docs/native-movies.md), drawn over render target 0 of
  // `device` at the place of the command that drew it (command 28): the Y
  // plane (width x height) and the half-size U and V, rows with no padding,
  // stretched over the target's top-left 1280x720 guest pixels, as the
  // movie library drew them.
  struct MoviePlanes {
    uint32_t width = 0, height = 0;
    const uint8_t* y = nullptr;
    const uint8_t* u = nullptr;
    const uint8_t* v = nullptr;
  };
  void DrawMovie(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                 const MoviePlanes& planes);
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
  // Debugging aids: skip the draws of a pixel shader (0: none); describe
  // each draw of the frames recorded, written with the next dump
  // (draws.txt); the pixel shaders the last frame drew the main view with.
  void set_skip_pixel_shader(uint64_t hash);
  // Leave out the game's motion blur (its pixel shader's draws; --motion_blur).
  void set_skip_motion_blur(bool skip);
  // For set_skip_pixel_shader: skip the draws without a pixel shader.
  static constexpr uint64_t kSkipNoPixelShader = 1;
  void set_record_draws(bool record);
  // A diagnostic: with record_draws, also note the frame's passes, clears
  // and resolves, written with the dump (passes.txt).
  void set_log_passes(bool log);
  // A diagnostic: log the glow history's cross-fades and write-backs once a
  // second, by logic and render-only frames.
  void set_log_glow(bool log);
  std::vector<uint64_t> main_view_pixel_shaders() const;
  void set_skip_repeated_state(bool skip);
  void set_reuse_constants(bool reuse);
  // Counts why constants aren't reused (Stats::fetch_changed and
  // offset_differs; a diagnostic, off by default).
  void set_log_constant_changes(bool log);
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
  // The same for the buffer arena's copies (step 2b), submitted first.
  VkCommandBuffer EndBufferUploads();

  // Logged by the renderer.
  struct Stats {
    uint64_t drawn = 0, unsupported = 0, failed = 0;
    // Failed draws by reason (kFailedDrawReasons in renderer.cpp).
    std::array<uint64_t, 9> failed_by{};
    uint32_t pipelines = 0, shaders_translated = 0, shaders_failed = 0, targets = 0;
    uint32_t shaders_from_disk = 0;
    // Draws skipped as having an empty viewport or scissor, and one example:
    // viewport offset and extent, scissor offset and extent (bit 31 of the
    // first word set when filled).
    uint64_t invisible = 0;
    std::array<uint32_t, 8> invisible_sample{};
    // Draws with render targets 1-3 bound to another surface than 0 (only 0
    // is drawn), or color and depth of different sample counts.
    uint64_t extra_targets = 0, mismatched_samples = 0;
    // Movie frames drawn (DrawMovie).
    uint64_t movie_draws = 0;
    // Draws with a depth bias, and with an "equal" depth test.
    uint64_t depth_biased = 0, depth_equal = 0;
    // Draws whose item's base vertex differs from VGT_INDX_OFFSET, and an
    // example (item's, register's).
    uint64_t base_vertex_differs = 0;
    // Draws with PsParamGen.
    uint64_t param_gen_draws = 0;
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
    std::array<uint32_t, 2> base_vertex_sample{};
    uint64_t clears = 0, clear_rects = 0;
    // Resolves done; skipped (no source or destination, a level or slice
    // other than 0, a destination format we can't draw into); flags with
    // bits we don't know, and an example.
    uint64_t resolves = 0, resolves_skipped = 0, resolve_unknown_flags = 0;
    // Predicated tilings begun, and the resolves their ends made.
    uint64_t tilings = 0, tiled_resolves = 0;
    // The pass plan (step 3): resolves by planned kind (render::ResolveKind),
    // resolves not as planned (another source or destination, or past the
    // plan's end), and lists planned without a whole walk.
    std::array<uint64_t, 4> planned_kinds{};
    uint64_t plan_mismatches = 0, plans_incomplete = 0;
    // Passes drawn straight into their texture, and of those, ones whose
    // first draw had no clear before it.
    uint64_t drawn_into = 0, uncleared_starts = 0;
    uint32_t resolve_flags_sample = 0;
    TextureCache::Stats textures;
    BufferTable::Stats buffers;
    // The buffer arena's (step 2b), and draws whose vertex shader fetches
    // from a stream with no buffer bound.
    BufferArena::Stats arena;
    uint64_t arena_unbound = 0;
  };
  Stats TakeStats();

 private:
  struct State;
  std::unique_ptr<State> state_;
};

}  // namespace native
