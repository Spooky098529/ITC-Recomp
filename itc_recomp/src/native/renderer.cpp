#include <format>
#include "renderer.h"
#include "render/state.h"
#include "frame_costs.h"
#include "base/log.h"

#include "buffers.h"
#include "gpu_memory.h"
#include "app_window.h"
#include "state.h"

#include "passes.h"
#include "shaders.h"
#include "vulkan_functions.h"
#include "gpu/vulkan/backend.h"

#include "guest.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <deque>
#include <mutex>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <vector>
#include "base/log.h"
#include "base/flags.h"
#include "memory/mapping.h"

ITC_FLAG_BOOL(log_gpu_memory, false, "ITC",
              "Log every 5 s the GPU memory the native renderer holds, by what it's for, "
              "and its counts of views, samplers, descriptor pools and pipelines (to find "
              "what grows from race to race)");
ITC_FLAG_BOOL(log_memory_use, false, "ITC",
              "Log every 5 s the resident memory by block (the heap, the image, the "
              "physical memory: touched pages) and the process's resident set, with the "
              "renderer's GPU memory (docs/pc-renderer.md, step 0)");
ITC_FLAG_BOOL(native_renderer_validation, false, "ITC",
              "Enable the Vulkan validation layer for the native renderer, if installed");
ITC_FLAG_INT32(native_renderer_show_resolve, -1, "ITC",
               "Show the frame's nth resolve destination in the native renderer's window "
               "instead of the main view (a debugging aid; -1 for the main view)");

ITC_FLAG_BOOL(native_renderer_serialize, false, "ITC",
              "Wait for the GPU to finish each frame before recording the next (a "
              "debugging aid: rules out frames in flight sharing data)");
ITC_FLAG_BOOL(native_renderer_skip_repeated_state, true, "ITC",
              "Don't record a draw's viewport, scissor, depth bias, blend constants, "
              "pipeline or index buffer again when the command buffer already holds them");
ITC_FLAG_BOOL(native_renderer_reuse_constants, true, "ITC",
              "Point a draw at the constants the last draw wrote, block by block, when "
              "they're the same, instead of writing them again");
ITC_FLAG_BOOL(native_renderer_log_constant_changes, false, "ITC",
              "Log why draws' constants aren't reused: what differs in the fetch block, "
              "and which blocks' offsets differ when the descriptor sets repeat");
ITC_FLAG_BOOL(native_renderer_log_texture_bindings, false, "ITC",
              "Log how often draws' texture bindings repeat the last ones (fetch constants, "
              "views, samplers, descriptor sets) and what would empty a cache of them");
ITC_FLAG_BOOL(native_renderer_cache_shader_lookups, true, "ITC",
              "Keep draws' shader lookups by what they read from the shader objects "
              "instead of hashing the shaders' microcode every draw");
ITC_FLAG_BOOL(native_renderer_reuse_texture_sets, true, "ITC",
              "Give a draw the last draw's texture descriptor set when its shader and the "
              "fetch constants it reads are unchanged, and no texture changed");
ITC_FLAG_BOOL(native_renderer_watch_draws, false, "ITC",
              "Log what changes in the main view's draws' inputs between frames: with the "
              "camera still, only flickering draws should (a debugging aid)");
ITC_FLAG_BOOL(native_renderer_hand_over_resolves, true, "ITC",
              "Hand a pass's image to its resolve's destination texture when the resolve is a "
              "whole-image copy from a single-sample target, instead of copying "
              "(docs/pc-renderer.md, step 4)");
ITC_FLAG_BOOL(native_renderer_hide_cubes, false, "ITC",
              "Give cube maps the white placeholder (a debugging aid)");
ITC_FLAG_STRING(native_renderer_cycle, "", "ITC",
                "Directory: when a file named 'now' appears in it, leave out the main "
                "view's draws of each pixel shader in turn for 3 s, saving a frame of each "
                "(a debugging aid that keeps frames recorded at their usual speed)");
ITC_FLAG_STRING(native_renderer_watch_pixel_shader, "", "ITC",
                "With native_renderer_watch_draws: only the draws of this pixel shader "
                "(hash in hex)");
ITC_FLAG_BOOL(motion_blur, true, "ITC",
              "Draw the game's velocity-based motion blur (pixel shader "
              "7EB16456AAD56CBA); false leaves the frame unblurred");
ITC_FLAG_STRING(native_renderer_skip_pixel_shader, "", "ITC",
                "Leave out the draws of this pixel shader (hash in hex; a debugging aid)");
ITC_FLAG_BOOL(native_renderer_log_glow, false, "ITC",
              "Log the glow history's cross-fades and write-backs once a second, by logic "
              "and render-only frames (a debugging aid)");
ITC_FLAG_BOOL(native_renderer_base_level_only, false, "ITC",
              "Sample only each texture's first level (a debugging aid)");
ITC_FLAG_BOOL(native_renderer_verify_textures, false, "ITC",
              "Hash every texture used each frame, logging and reloading those whose "
              "memory changed with no memcpy write recorded (a debugging aid)");
ITC_FLAG_INT32(native_renderer_dump_every, 0, "ITC",
               "With native_renderer_dump: dump only the frame itself (shown image, draw "
               "IDs, draws.txt) every this many seconds of a race (a debugging aid)");
ITC_FLAG_STRING(native_renderer_dump, "", "ITC",
                "Directory to dump frames into, as PFM files, with draws.txt describing "
                "each draw: once 10 s into a race, and again whenever a file named 'now' "
                "appears in it (a debugging aid; see docs/native-renderer.md)");
ITC_FLAG_INT32(width, 0, "ITC",
               "Window width in pixels (in fullscreen, the drawing width); 0: 1280, or "
               "from height at 16:9");
ITC_FLAG_INT32(height, 0, "ITC",
               "Window height in pixels (in fullscreen, the drawing height); 0: 720, or "
               "from width at 16:9");
// The SDK's (it also makes the SDK's own window, hidden in exclusive mode,
// fullscreen); ours follows it.
// The SDK's until A4, same name and default (itc.toml sets it false).
ITC_FLAG_BOOL(fullscreen, true, "UI/Window", "Start the window in fullscreen mode");

namespace native {
namespace {

using Clock = std::chrono::steady_clock;

constexpr uint32_t kFramesInFlight = 2;
constexpr uint32_t kDefaultWidth = 1280, kDefaultHeight = 720;

// The window's size from --width and --height (one alone keeps 16:9).
VkExtent2D WindowSize() {
  int32_t width = ITC_FLAG(width), height = ITC_FLAG(height);
  if (width <= 0 && height <= 0) {
    return {kDefaultWidth, kDefaultHeight};
  }
  if (width <= 0) {
    width = (height * 16 + 8) / 9;
  } else if (height <= 0) {
    height = (width * 9 + 8) / 16;
  }
  return {uint32_t(width), uint32_t(height)};
}

// How many times the game's own resolution (1280x720 frame, 1024x576 3D
// view) is drawn (see "Resolution" in docs/native-renderer.md): in
// fullscreen with --width or --height, the largest that fits them;
// otherwise the largest that fits the window's pixels (`pixels`, the
// swapchain's size).
float ResolutionScale(VkExtent2D pixels) {
  bool sized = ITC_FLAG(width) > 0 || ITC_FLAG(height) > 0;
  VkExtent2D size = (ITC_FLAG(fullscreen) && sized) || !pixels.width || !pixels.height
                        ? WindowSize()
                        : pixels;
  return std::min(float(size.width) / kDefaultWidth, float(size.height) / kDefaultHeight);
}

// Set (from the main thread's event loop) when our window's size in pixels
// changes: the swapchain is made again (Wayland never reports it out of
// date).
std::atomic<bool> g_window_resized{false};
SDL_WindowID g_window_id = 0;
bool SDLCALL WatchWindowEvents(void*, SDL_Event* event) {
  if (event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED &&
      event->window.windowID == g_window_id) {
    g_window_resized = true;
  }
  return true;
}

// Draw commands (see docs/gpu-functions.md).
bool IsDraw(uint32_t id) {
  return id == 15 || id == 16 || id == 17 || id == 18 || id == 19 || id == 27;
}

// The window, created on the main thread (SDL's window calls belong there;
// the main thread runs SDL's event loop), with what the backend makes there.
struct WindowSetup {
  gpu::Backend* backend = nullptr;
  SDL_Window* window = nullptr;
  VkExtent2D display{};  // its screen's size in pixels
  float density = 1.0f;  // pixels per unit of the desktop's
  std::string error;
  std::promise<void> done;
};

void SDLCALL CreateWindowOnMainThread(void* data) {
  auto& setup = *static_cast<std::shared_ptr<WindowSetup>*>(data);
  auto finish = [&](std::string error) {
    setup->error = std::move(error);
    setup->done.set_value();
    delete static_cast<std::shared_ptr<WindowSetup>*>(data);
  };
  VkExtent2D size = WindowSize();
  // Fullscreen: borderless, at the desktop's resolution (no mode change).
  // High pixel density: real pixels under the desktop's scaling, not a
  // smaller surface the compositor stretches (2648x1490 for a 3840x2160
  // screen at 145%).
  SDL_WindowFlags flags = setup->backend->WindowFlags() | SDL_WINDOW_RESIZABLE |
                          SDL_WINDOW_HIGH_PIXEL_DENSITY |
                          (ITC_FLAG(fullscreen) ? SDL_WINDOW_FULLSCREEN : 0);
  setup->window =
      SDL_CreateWindow("ITC native renderer", int(size.width), int(size.height), flags);
  if (!setup->window) {
    return finish(std::format("SDL_CreateWindow: {}", SDL_GetError()));
  }
  if (const SDL_DisplayMode* mode =
          SDL_GetDesktopDisplayMode(SDL_GetDisplayForWindow(setup->window))) {
    setup->display = {uint32_t(std::lround(mode->w * mode->pixel_density)),
                      uint32_t(std::lround(mode->h * mode->pixel_density))};
  }
  // The window's size is given in pixels: in the desktop's (scaled) units,
  // it's that over the pixel density.
  if (float density = SDL_GetWindowPixelDensity(setup->window);
      density > 0.0f && density != 1.0f && !ITC_FLAG(fullscreen)) {
    SDL_SetWindowSize(setup->window, int(std::lround(size.width / density)),
                      int(std::lround(size.height / density)));
    SDL_SyncWindow(setup->window);
  }
  setup->density = SDL_GetWindowPixelDensity(setup->window);
  g_window_id = SDL_GetWindowID(setup->window);
  SDL_AddEventWatch(WatchWindowEvents, nullptr);
  AppWindow_AdoptOurWindow(setup->window);
  finish(setup->backend->AttachWindow(setup->window));
}

class Renderer {
 public:
  // Render thread. Returns false if the renderer can't run.
  bool Init();
  void OnCommand(uint32_t id, uint32_t arg1);
  void InlineDraw(uint32_t type, uint32_t count, uint32_t stride, uint32_t data);
  void DrawIndexed(uint32_t type, uint32_t base_start, uint32_t primitive_count);
  // The passes, in a frame being recorded (begun if it isn't).
  Passes& passes() {
    BeginFrame();
    return passes_;
  }
  void FrontBufferResolve(uint32_t device, uint32_t front_buffer) {
    passes().FrontBufferResolve(device, front_buffer);
  }
  void DrawMovie(uint32_t device, const Passes::MoviePlanes& planes) {
    passes().DrawMovie(device, planes);
  }
  void EndFrame();

 private:
  bool OpenWindow();
  // Begins this frame's recording (once its slot's previous frame is
  // done), if it isn't begun.
  void BeginFrame();
  void LogStats();

  SDL_Window* window_ = nullptr;
  VkExtent2D display_{};  // the window's screen, in pixels
  std::unique_ptr<gpu::vulkan::Backend> backend_;
  uint32_t frame_index_ = 0;  // the frame's slot
  Passes passes_;
  bool recording_ = false;  // this frame's command buffer is open
  // The dump's frames after the first: what each hides.
  struct DumpStep {
    std::string name;
    TextureCache::Hide hide;
    uint32_t value;
    uint64_t skip_pixel_shader;
  };
  bool dumping_ = false;
  // The pixel shader cycle (native_renderer_cycle).
  struct CycleStep {
    std::string name;
    uint64_t skip;
  };
  std::vector<CycleStep> cycle_steps_;
  size_t cycle_step_ = 0;
  Clock::time_point cycle_step_start_;
  bool cycle_captured_ = false;
  uint32_t cycles_ = 0;
  std::string cycle_directory_;
  uint32_t dumps_ = 0;
  std::string dump_directory_;
  std::vector<DumpStep> dump_steps_;
  uint32_t dump_step_ = 0;
  uint32_t race_frames_ = 0;
  Clock::time_point last_dump_ = Clock::now();

  // This frame's commands (milestone 1: counted only).
  uint32_t frame_commands_ = 0, frame_draws_ = 0;

  // Logged every 5 s.
  uint64_t frames_presented_ = 0, frames_skipped_ = 0, commands_ = 0, draws_ = 0;
  // Texture loading and hashing (frame_costs.h) since the last stats: total
  // and the frame that spent the most.
  int64_t texture_ns_ = 0, texture_ns_max_ = 0, mark_ns_ = 0;
  Clock::time_point start_ = Clock::now(), last_log_ = Clock::now();
  Clock::time_point last_present_ = Clock::now();
};

bool Renderer::OpenWindow() {
  static Uint32 wakeup_event = SDL_RegisterEvents(1);
  auto setup = std::make_shared<WindowSetup>();
  setup->backend = backend_.get();
  auto future = setup->done.get_future();
  // Queued for the main thread, which runs it as it pumps events; an event
  // of our own wakes it (the SDK's loop ignores event types it doesn't know).
  if (!SDL_RunOnMainThread(CreateWindowOnMainThread, new std::shared_ptr<WindowSetup>(setup),
                           false)) {
    ITC_LOG_ERROR("native renderer: SDL_RunOnMainThread: {}", SDL_GetError());
    return false;
  }
  SDL_Event wake{};
  wake.type = wakeup_event;
  SDL_PushEvent(&wake);
  if (future.wait_for(std::chrono::seconds(10)) != std::future_status::ready) {
    ITC_LOG_ERROR("native renderer: the main thread didn't create the window");
    return false;
  }
  if (!setup->error.empty()) {
    ITC_LOG_ERROR("native renderer: {}", setup->error);
    return false;
  }
  window_ = setup->window;
  display_ = setup->display;
  ITC_LOG_INFO("native renderer: window {}x{}{}, screen {}x{}, pixel density {}",
               WindowSize().width, WindowSize().height,
               ITC_FLAG(fullscreen) ? " (fullscreen)" : "", display_.width, display_.height,
               setup->density);
  return true;
}

bool Renderer::Init() {
  gpu::vulkan::Backend::Options options;
  options.frames_in_flight = kFramesInFlight;
  options.validation = ITC_FLAG(native_renderer_validation);
  options.requested_size = {WindowSize().width, WindowSize().height};
  backend_ = std::make_unique<gpu::vulkan::Backend>(options);
  if (!OpenWindow() || !backend_->Init()) {
    return false;
  }
  const gpu::Extent2D output = backend_->OutputSize();
  if (!passes_.Init(*backend_, kFramesInFlight,
                    ResolutionScale({output.width, output.height}))) {
    return false;
  }
  return true;
}

void Renderer::OnCommand(uint32_t id, uint32_t arg1) {
  frame_commands_++;
  if (IsDraw(id)) {
    frame_draws_ += arg1;  // the handler makes one draw per item
  }
}

void Renderer::BeginFrame() {
  if (!recording_) {
    frame_index_ = backend_->BeginFrame();
    if (ITC_FLAG(native_renderer_serialize)) {
      backend_->WaitIdle();
    }
    passes_.BeginFrame(frame_index_);
    // A dump starts 10 s into a race, or when asked for.
    const std::string& dump_root = ITC_FLAG(native_renderer_dump);
    if (!dump_root.empty() && !dumping_) {
      std::error_code error;
      bool asked = std::filesystem::remove(dump_root + "/now", error);
      // With dump_every: a frame every that many seconds of a race.
      int32_t every = ITC_FLAG(native_renderer_dump_every);
      bool in_race = passes_.frame_resolve_count() >= 20;
      bool periodic = every > 0 && in_race && Clock::now() - last_dump_ >= std::chrono::seconds(every);
      if (asked || periodic ||
          (every <= 0 && dumps_ == 0 && in_race && ++race_frames_ >= 600)) {
        last_dump_ = Clock::now();
        dumping_ = true;
        dump_step_ = 0;
        dump_steps_.clear();
        dump_directory_ = std::format("{}/{}", dump_root, dumps_++);
        std::filesystem::create_directories(dump_directory_, error);
        ITC_LOG_INFO("native renderer: dumping into {}", dump_directory_);
      }
    }
    // The cycle: started on request, a step every 3 s.
    const std::string& cycle_root = ITC_FLAG(native_renderer_cycle);
    if (!cycle_root.empty()) {
      std::error_code error;
      if (cycle_steps_.empty() && std::filesystem::remove(cycle_root + "/now", error)) {
        cycle_steps_.push_back({"none", 0});
        for (uint64_t hash : passes_.main_view_pixel_shaders()) {
          cycle_steps_.push_back({std::format("skip_ps_{:016X}", hash), hash});
        }
        cycle_steps_.push_back({"skip_no_pixel_shader", Passes::kSkipNoPixelShader});
        cycle_steps_.push_back({"none_again", 0});
        cycle_step_ = 0;
        cycle_step_start_ = Clock::now();
        cycle_captured_ = false;
        cycle_directory_ = std::format("{}/{}", cycle_root, cycles_++);
        std::filesystem::create_directories(cycle_directory_, error);
        ITC_LOG_INFO("native renderer: cycling {} steps into {}", cycle_steps_.size(),
                     cycle_directory_);
      }
      if (!cycle_steps_.empty()) {
        passes_.set_skip_pixel_shader(cycle_steps_[cycle_step_].skip);
      }
    }
    if (const std::string& skip = ITC_FLAG(native_renderer_skip_pixel_shader); !skip.empty()) {
      passes_.set_skip_pixel_shader(std::strtoull(skip.c_str(), nullptr, 16));
    }
    passes_.set_skip_motion_blur(!ITC_FLAG(motion_blur));
    // Its first frame describes each draw (cheaply: the effect being looked
    // for went away while frames were recorded slowly).
    passes_.set_record_draws(dumping_ && dump_step_ == 0);
    passes_.set_skip_repeated_state(ITC_FLAG(native_renderer_skip_repeated_state));
    passes_.set_reuse_constants(ITC_FLAG(native_renderer_reuse_constants));
    passes_.set_log_constant_changes(ITC_FLAG(native_renderer_log_constant_changes));
    passes_.set_log_texture_bindings(ITC_FLAG(native_renderer_log_texture_bindings));
    passes_.set_cache_shader_lookups(ITC_FLAG(native_renderer_cache_shader_lookups));
    passes_.set_reuse_texture_sets(ITC_FLAG(native_renderer_reuse_texture_sets));
    passes_.set_log_glow(ITC_FLAG(native_renderer_log_glow));
    passes_.set_watch_draws(
        ITC_FLAG(native_renderer_watch_draws),
        std::strtoull(ITC_FLAG(native_renderer_watch_pixel_shader).c_str(), nullptr, 16));
    passes_.set_hide_cubes(ITC_FLAG(native_renderer_hide_cubes));
    passes_.set_hand_over_resolves(ITC_FLAG(native_renderer_hand_over_resolves));
    passes_.set_base_level_only(ITC_FLAG(native_renderer_base_level_only));
    passes_.set_verify_textures(ITC_FLAG(native_renderer_verify_textures));
    recording_ = true;
  }
}

void Renderer::InlineDraw(uint32_t type, uint32_t count, uint32_t stride,
                          uint32_t data) {
  passes().InlineDraw(type, count, stride, data);
}

void Renderer::DrawIndexed(uint32_t type, uint32_t base_start, uint32_t primitive_count) {
  passes().DrawIndexed(type, base_start, primitive_count);
}

void Renderer::EndFrame() {
  commands_ += frame_commands_;
  draws_ += frame_draws_;
  uint32_t frame_draws = frame_draws_;
  frame_commands_ = frame_draws_ = 0;

  BeginFrame();
  // A dump: the frame as is (with the resolves), then frames with something
  // left out (see dump_steps_).
  bool dump = dumping_;
  // The cycle saves a frame 2 s into each step (the pause after it is over
  // by the next).
  bool cycle_capture = !cycle_steps_.empty() && !cycle_captured_ &&
                       Clock::now() - cycle_step_start_ >= std::chrono::seconds(2);
  VkExtent2D source =
      dump ? passes_.EndFrame(ITC_FLAG(native_renderer_show_resolve), true,
                              dump_step_ ? dump_steps_[dump_step_ - 1].name : "shown",
                              dump_step_ == 0 && ITC_FLAG(native_renderer_dump_every) <= 0)
      : cycle_capture
          ? passes_.EndFrame(ITC_FLAG(native_renderer_show_resolve), true,
                             std::format("{:02}_{}", cycle_step_, cycle_steps_[cycle_step_].name),
                             false)
          : passes_.EndFrame(ITC_FLAG(native_renderer_show_resolve), false);
  // Shown, after the frame's uploads.
  if (g_window_resized.exchange(false)) {
    backend_->WindowResized();
  }
  passes_.EndUploads();
  recording_ = false;
  if (backend_->EndFrame(passes_.shown(), {source.width, source.height})) {
    frames_presented_++;
    // A frame much longer than usual (a hitch the per-second fps hides),
    // with what it spent on work that comes and goes (frame_costs.h).
    auto now = Clock::now();
    if (auto ms = std::chrono::duration<double, std::milli>(now - last_present_).count();
        ms >= 40 && frames_presented_ > 1) {
      const FrameCosts& c = g_frame_costs;
      ITC_LOG_INFO(
          "native renderer: long frame: {:.0f} ms since the last present ({} draws): "
          "{} textures loaded ({:.1f} MB) in {:.1f} ms (hashing {:.1f} ms), {} shaders "
          "translated in {:.1f} ms, {} pipelines made in {:.1f} ms",
          ms, frame_draws, c.textures, c.texture_bytes / 1048576.0, c.texture_ns / 1e6,
          c.hash_ns / 1e6,
          c.shaders, c.shader_ns / 1e6, c.pipelines, c.pipeline_ns / 1e6);
    }
    texture_ns_ += g_frame_costs.texture_ns + g_frame_costs.hash_ns;
    mark_ns_ += g_frame_costs.mark_ns;
    texture_ns_max_ =
        std::max(texture_ns_max_, g_frame_costs.texture_ns + g_frame_costs.hash_ns);
    g_frame_costs = {};
    last_present_ = now;
  } else {
    frames_skipped_++;
  }
  if (cycle_capture) {
    backend_->WaitIdle();
    passes_.WriteDump(cycle_directory_);
    cycle_captured_ = true;
  }
  if (!cycle_steps_.empty() && Clock::now() - cycle_step_start_ >= std::chrono::seconds(3)) {
    cycle_step_++;
    cycle_step_start_ = Clock::now();
    cycle_captured_ = false;
    if (cycle_step_ >= cycle_steps_.size()) {
      cycle_steps_.clear();
      passes_.set_skip_pixel_shader(0);
      ITC_LOG_INFO("native renderer: cycle done");
    }
  }
  if (dump) {
    backend_->WaitIdle();
    passes_.WriteDump(dump_directory_);
    if (dump_step_ == 0 && ITC_FLAG(native_renderer_dump_every) <= 0) {
      // Each pixel shader the main view was drawn with, left out; then all
      // textures from memory, and all resolve destinations, hidden.
      using Hide = TextureCache::Hide;
      for (uint64_t hash : passes_.main_view_pixel_shaders()) {
        dump_steps_.push_back({std::format("skip_ps_{:016X}", hash), Hide::kNone, 0, hash});
      }
      dump_steps_.push_back({"hide_memory", Hide::kMemory, 0, 0});
      dump_steps_.push_back({"hide_resolved", Hide::kResolved, 0, 0});
    }
    dump_step_++;
    if (dump_step_ <= dump_steps_.size()) {
      const DumpStep& step = dump_steps_[dump_step_ - 1];
      passes_.set_debug_hide(step.hide, step.value);
      passes_.set_skip_pixel_shader(step.skip_pixel_shader);
    } else {
      passes_.set_debug_hide(TextureCache::Hide::kNone);
      passes_.set_skip_pixel_shader(0);
      dumping_ = false;
    }
  }
  LogStats();
}

void Renderer::LogStats() {
  auto now = Clock::now();
  if (now - last_log_ < std::chrono::seconds(5)) {
    return;
  }
  if (!base::InfoLogEnabled()) {
    // Nothing would be printed: leave the counters to run on.
    last_log_ = now;
    return;
  }
  double seconds = std::chrono::duration<double>(now - last_log_).count();
  uint64_t frames = std::max<uint64_t>(frames_presented_ + frames_skipped_, 1);
  Passes::Stats view = passes_.TakeStats();
  ITC_LOG_INFO("native renderer: {:.0f} frames/s presented, {} skipped; per frame {:.0f} "
               "commands, {:.0f} draws; {:.0f} drawn, {:.0f} unsupported, {:.0f} failed; {:.1f} "
               "MB copied a frame ({:.1f} MB unchanged, not copied); {} pipelines; shaders "
               "from their GLSL {}",
               frames_presented_ / seconds, frames_skipped_, double(commands_) / frames,
               double(draws_) / frames, double(view.drawn) / frames,
               double(view.unsupported) / frames, double(view.failed) / frames,
               view.mirrored_bytes / 1048576.0 / frames,
               view.mirror_skipped_bytes / 1048576.0 / frames, view.pipelines,
               view.shaders_ported);
  if (view.failed) {
    static constexpr const char* kFailedDrawReasons[] = {
        "no target", "no vertex shader", "shader translation", "no index buffer",
        "constants ring full", "texture set layout", "texture set",
        "inline vertex space full"};
    std::string reasons;
    for (size_t i = 0; i < view.failed_by.size(); i++) {
      if (view.failed_by[i]) {
        reasons += std::format(" {} {:.1f};", kFailedDrawReasons[i],
                               double(view.failed_by[i]) / frames);
      }
    }
    ITC_LOG_INFO("native renderer: failed draws a frame by reason:{}", reasons);
  }
  if (ITC_FLAG(log_gpu_memory) || ITC_FLAG(log_memory_use)) {
    ITC_LOG_INFO("native renderer: GPU memory: {}", gpu_memory::Describe());
  }
  if (ITC_FLAG(log_memory_use)) {
    ITC_LOG_INFO("memory use: {}", memory::DescribeResident());
  }
  ITC_LOG_INFO("native renderer: {}", passes_.TakeArenaStats());
  ITC_LOG_INFO("native renderer: {} targets; per frame {:.1f} clears ({:.1f} with rects), {:.1f} "
               "resolves ({:.1f} handed over, {:.1f} targets copied back), {:.1f} skipped; {:.1f} "
               "draws with targets 1-3 bound, {:.1f} with "
               "mismatched sample counts, {:.1f} with a depth bias, {:.1f} with an equal depth "
               "test; {} resolves with unknown flags (e.g. {:08X}); {} tilings, {} tiled resolves",
               view.targets, double(view.clears) / frames, double(view.clear_rects) / frames,
               double(view.resolves) / frames, double(view.resolves_handed) / frames,
               double(view.resolves_copied_back) / frames, double(view.resolves_skipped) / frames,
               double(view.extra_targets) / frames, double(view.mismatched_samples) / frames,
               double(view.depth_biased) / frames, double(view.depth_equal) / frames,
               view.resolve_unknown_flags, view.resolve_flags_sample, view.tilings,
               view.tiled_resolves);
  frames_presented_ = frames_skipped_ = commands_ = draws_ = 0;
  const TextureCache::Stats& t = view.textures;
  ITC_LOG_INFO("native renderer: textures: {} ({} samplers); {} loaded, {} reloaded, {:.1f} MB "
               "(loading and hashing {:.1f} ms, at most {:.1f} ms a frame); "
               "{} deferred (staging full), {} unsupported (e.g. format {} dimension {}), {} "
               "of another dimension than declared; {:.0f} lookups a frame got a resolve "
               "destination ({:.0f} of another size or format)",
               t.textures, t.samplers, t.loaded, t.reloaded, t.loaded_bytes / 1048576.0,
               texture_ns_ / 1e6, texture_ns_max_ / 1e6,
               t.deferred, t.unsupported,
               t.unsupported_sample == UINT32_MAX ? -1 : int(t.unsupported_sample >> 8),
               t.unsupported_sample == UINT32_MAX ? -1 : int(t.unsupported_sample & 0xFF),
               t.mismatched, double(t.resolved_hits) / frames,
               double(t.resolved_mismatched) / frames);
  texture_ns_ = texture_ns_max_ = 0;
  ITC_LOG_INFO("native renderer: textures: {} memcpy writes, {} textures marked written ({:.1f} "
               "ms marking)",
               t.writes, t.marked, mark_ns_ / 1e6);
  mark_ns_ = 0;
  if (t.unrecorded) {
    ITC_LOG_INFO("native renderer: {} textures changed in memory with no write recorded, e.g. "
                 "{:08X}, {} frames after it was first loaded",
                 t.unrecorded, t.unrecorded_sample, t.unrecorded_frames);
  }
  if (t.mismatched_sample != UINT32_MAX) {
    ITC_LOG_INFO("native renderer: e.g. a texture of format {}, dimension {}, {}x{} where the "
                 "shader declares view type {}",
                 (t.mismatched_sample >> 8) & 0xFF, t.mismatched_sample & 0xFF, t.mismatched_width,
                 t.mismatched_height, t.mismatched_sample >> 16);
  }
  ITC_LOG_INFO("native renderer: {:.1f} draws a frame with PsParamGen;{}",
               double(view.param_gen_draws) / frames, passes_.TakeInterfaces());
  if (view.inline_draws) {
    const auto& t = view.inline_by_type;
    ITC_LOG_INFO("native renderer: {:.1f} inline vertex draws a frame (triangle lists {:.1f}, "
                 "strips {:.1f}, rectangle lists {:.1f}, quad lists {:.1f})",
                 double(view.inline_draws) / frames, double(t[4]) / frames, double(t[6]) / frames,
                 double(t[8]) / frames, double(t[13]) / frames);
  }
  if (view.state_compared) {
    const auto& r = view.state_repeated;
    auto share = [&](uint64_t n) { return 100.0 * double(n) / double(view.state_compared); };
    ITC_LOG_INFO("native renderer: of {:.1f} draws a frame following another draw, repeating its "
                 "viewport {:.0f}%, scissor {:.0f}%, depth bias {:.0f}%, blend constants {:.0f}%, "
                 "pipeline {:.0f}%, descriptor sets {:.0f}% (with offsets {:.0f}%), index buffer "
                 "binding {:.0f}% (buffer and type {:.0f}%)",
                 double(view.state_compared) / frames, share(r[0]), share(r[1]), share(r[2]),
                 share(r[3]), share(r[4]), share(r[5]), share(r[8]), share(r[6]), share(r[7]));
    const auto& o = view.offset_differs;
    if (view.offsets_differ) {
      ITC_LOG_INFO("native renderer: descriptor sets repeated with other offsets for {:.0f}% of "
                   "draws: system {:.0f}%, vertex floats {:.0f}%, pixel floats {:.0f}%, "
                   "bool/loop {:.0f}%",
                   share(view.offsets_differ), share(o[0]), share(o[1]), share(o[2]),
                   share(o[3]));
    }
  }
  if (view.constants_draws) {
    const auto& r = view.constants_reused;
    auto share = [&](uint64_t n) { return 100.0 * double(n) / double(view.constants_draws); };
    ITC_LOG_INFO("native renderer: of {:.1f} draws a frame given constants, reusing the last "
                 "written system {:.0f}%, vertex floats {:.0f}%, pixel floats {:.0f}%, bool/loop "
                 "{:.0f}%; {:.0f} KB written a frame",
                 double(view.constants_draws) / frames, share(r[0]), share(r[1]), share(r[2]),
                 share(r[3]), double(view.constant_bytes) / frames / 1024);
  }
  if (view.vertex_overruns) {
    const auto& v = view.vertex_overrun_sample;
    ITC_LOG_INFO("native renderer: {:.1f} draws a frame read vertices past their fetch "
                 "constant's size, e.g. {:08X}: {} bytes, {} needed (vertex {}, stride {} words)",
                 double(view.vertex_overruns) / frames, v[0], v[1], v[2], v[3], v[4]);
  }
  if (ITC_FLAG(native_renderer_watch_draws)) {
    uint64_t changes, watched;
    std::vector<std::string> lines = passes_.TakeWatchLines(changes, watched);
    ITC_LOG_INFO("native renderer: {} of {} main view draws changed inputs since the previous "
                 "frame", changes, watched);
    for (const std::string& line : lines) {
      ITC_LOG_INFO("native renderer: changed: {}", line);
    }
  }
  if (ITC_FLAG(native_renderer_log_texture_bindings)) {
    ITC_LOG_INFO("native renderer: {}", passes_.TakeTextureBindings());
  }
  if (view.invisible) {
    const auto& v = view.invisible_sample;
    ITC_LOG_INFO("native renderer: {:.0f} draws a frame skipped as invisible, e.g. "
                 "viewport {},{} {}x{}, scissor {},{} {}x{}",
                 double(view.invisible) / frames, v[0] & 0x7FFFFFFF, v[1], v[2], v[3], v[4], v[5],
                 v[6], v[7]);
  }
  last_log_ = now;
}

// Render thread only. Null until the first frame, and if it can't run.
Renderer* g_renderer = nullptr;
bool g_failed = false;

// Set by the command hooks: the renderer starts after the first of them
// (Renderer_EndFrame does nothing before).
bool g_commands_seen = false;

// Kept out of line: inlined, Init's stack frame was set up on every call to
// GetRenderer (several per replayed command), ~2% of the render thread.
[[gnu::noinline]] void StartRenderer() {
  g_renderer = new Renderer();
  if (!g_commands_seen || !g_renderer->Init()) {
    ITC_LOG_ERROR("native renderer: couldn't start; continuing without it");
    g_failed = true;
    g_renderer = nullptr;  // leaked: may hold objects already made
  }
}

Renderer* GetRenderer() {
  if (!g_renderer && !g_failed) {
    StartRenderer();
  }
  return g_renderer;
}

}  // namespace

void Renderer_OnCommand(uint32_t id, uint32_t payload, uint32_t arg1,
                        uint32_t arg2) {
  g_commands_seen = true;
  Buffers_OnCommand(id, payload, arg1);
  if (Renderer* renderer = GetRenderer()) {
    renderer->OnCommand(id, arg1);
  }
}


}  // namespace native

void render::DrawIndexed(uint32_t type, uint32_t base_start, uint32_t primitive_count) {
  if (native::Renderer* renderer = native::GetRenderer()) {
    renderer->DrawIndexed(type, base_start, primitive_count);
  }
}

void render::Clear(uint32_t flags, uint32_t color, float z, uint32_t stencil, bool rects) {
  if (native::Renderer* renderer = native::GetRenderer()) {
    renderer->passes().Clear(flags, color, z, stencil, rects);
  }
}

void render::Resolve(uint32_t flags, const int32_t* rect, uint32_t destination,
                     const int32_t* point, bool level_or_slice, const float clear_color[4],
                     float clear_z, uint32_t clear_stencil) {
  if (native::Renderer* renderer = native::GetRenderer()) {
    renderer->passes().Resolve(flags, rect, destination, point,
                               level_or_slice, clear_color, clear_z, clear_stencil);
  }
}

void render::TilingBegun(uint32_t width, uint32_t height, float clear) {
  if (native::Renderer* renderer = native::GetRenderer()) {
    renderer->passes().BeginTiling(width, height, clear);
  }
}

void render::TilingEnded(uint32_t flags, uint32_t destination, float clear) {
  if (native::Renderer* renderer = native::GetRenderer()) {
    renderer->passes().EndTiling(flags, destination, clear);
  }
}

void render::DrawUP(uint32_t type, uint32_t count, uint32_t stride, uint32_t data) {
  if (native::Renderer* renderer = native::GetRenderer()) {
    renderer->InlineDraw(type, count, stride, data);
  }
}

namespace native {

void Renderer_FrontBufferResolve(uint32_t device, uint32_t front_buffer) {
  g_commands_seen = true;
  if (Renderer* renderer = GetRenderer()) {
    renderer->FrontBufferResolve(device, front_buffer);
  }
}

void Renderer_DrawMovie(uint32_t device, uint32_t width, uint32_t height,
                        const uint8_t* y, const uint8_t* u, const uint8_t* v) {
  g_commands_seen = true;
  if (Renderer* renderer = GetRenderer()) {
    renderer->DrawMovie(device, {width, height, y, u, v});
  }
}

namespace {
// Lists handed over and not yet replayed: whether a logic pass recorded
// each (any thread, under g_lists_mutex); and the one being replayed
// (render thread).
std::mutex g_lists_mutex;
std::deque<bool> g_lists;
bool g_replaying_logic_list = true;
uint64_t g_lists_missing = 0, g_lists_queued_most = 0;
}  // namespace

void Renderer_OnListHandedOver(bool logic_pass) {
  std::lock_guard lock(g_lists_mutex);
  g_lists.push_back(logic_pass);
}

void Renderer_OnReplayStart() {
  std::lock_guard lock(g_lists_mutex);
  g_lists_queued_most = std::max<uint64_t>(g_lists_queued_most, g_lists.size());
  if (g_lists.empty()) {
    // Not handed over by WaitForFrame: taken as a logic pass's.
    g_lists_missing++;
    g_replaying_logic_list = true;
    return;
  }
  g_replaying_logic_list = g_lists.front();
  g_lists.pop_front();
}

bool Renderer_ReplayingLogicList() {
  return g_replaying_logic_list;
}

void Renderer_TakeListStats(uint64_t& missing, uint64_t& most) {
  std::lock_guard lock(g_lists_mutex);
  missing = g_lists_missing;
  most = g_lists_queued_most;
  g_lists_missing = g_lists_queued_most = 0;
}

void Renderer_EndFrame() {
  if (!g_commands_seen) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->EndFrame();
  }
}

}  // namespace native

namespace native::gpu_memory {

std::string Describe() {
  static constexpr const char* kKinds[] = {"textures", "resolved", "images",   "staging",
                                           "arena",    "constants", "other"};
  static constexpr const char* kObjects[] = {"views", "samplers", "descriptor pools",
                                             "pipelines"};
  static_assert(std::size(kKinds) == size_t(Kind::kCount));
  static_assert(std::size(kObjects) == size_t(Object::kCount));
  Totals& totals = Get();
  int64_t total = 0;
  std::string out;
  for (size_t i = 0; i < size_t(Kind::kCount); i++) {
    int64_t bytes = totals.bytes[i].load(std::memory_order_relaxed);
    total += bytes;
    out += std::format("{} {:.1f} MB ({}); ", kKinds[i], bytes / 1048576.0,
                       totals.allocations[i].load(std::memory_order_relaxed));
  }
  out = std::format("{:.1f} MB: ", total / 1048576.0) + out;
  for (size_t i = 0; i < size_t(Object::kCount); i++) {
    out += std::format("{}{} {}", i ? ", " : "", kObjects[i],
                       totals.objects[i].load(std::memory_order_relaxed));
  }
  return out;
}

}  // namespace native::gpu_memory
