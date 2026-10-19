#include "renderer.h"
#include "frame_costs.h"
#include "log_enabled.h"

#include "buffers.h"
#include "gpu_memory.h"
#include "app_window.h"
#include "state.h"

#include "passes.h"
#include "shaders.h"
#include "vulkan_functions.h"

#include "guest.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <deque>
#include <mutex>
#include <SDL3/SDL_vulkan.h>
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
#include <rex/logging.h>
#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(log_gpu_memory, false, "ITC",
                    "Log every 5 s the GPU memory the native renderer holds, by what it's for, "
                    "and its counts of views, samplers, descriptor pools and pipelines (to find "
                    "what grows from race to race)");
REXCVAR_DEFINE_BOOL(native_renderer, false, "ITC",
                    "Draw the game's render commands with our own Vulkan renderer, in a "
                    "second window (in development; see docs/native-renderer.md)");
REXCVAR_DEFINE_BOOL(native_renderer_exclusive, false, "ITC",
                    "With native_renderer: hand the emulated GPU no work, drawing only in our "
                    "window (milestone 6; see docs/native-renderer.md)");
REXCVAR_DEFINE_BOOL(native_renderer_validation, false, "ITC",
                    "Enable the Vulkan validation layer for the native renderer, if installed");
REXCVAR_DEFINE_INT32(native_renderer_show_resolve, -1, "ITC",
                     "Show the frame's nth resolve destination in the native renderer's window "
                     "instead of the main view (a debugging aid; -1 for the main view)");

REXCVAR_DEFINE_BOOL(native_renderer_serialize, false, "ITC",
                    "Wait for the GPU to finish each frame before recording the next (a "
                    "debugging aid: rules out frames in flight sharing data)");
REXCVAR_DEFINE_BOOL(native_renderer_check_mirror, false, "ITC",
                    "Log the vertex and index ranges whose contents change between frames "
                    "(a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_skip_repeated_state, true, "ITC",
                    "Don't record a draw's viewport, scissor, depth bias, blend constants, "
                    "pipeline or index buffer again when the command buffer already holds them");
REXCVAR_DEFINE_BOOL(native_renderer_reuse_constants, true, "ITC",
                    "Point a draw at the constants the last draw wrote, block by block, when "
                    "they're the same, instead of writing them again");
REXCVAR_DEFINE_BOOL(native_renderer_log_constant_changes, false, "ITC",
                    "Log why draws' constants aren't reused: what differs in the fetch block, "
                    "and which blocks' offsets differ when the descriptor sets repeat");
REXCVAR_DEFINE_BOOL(native_renderer_log_register_reads, false, "ITC",
                    "Log how often the device's register shadow is read a frame, how many words "
                    "are copied (those marked dirty) and what changed since the last read");
REXCVAR_DEFINE_BOOL(native_renderer_cache_index_ranges, true, "ITC",
                    "Keep each draw's lowest and highest index until memcpy writes its index "
                    "range, instead of scanning the indices every draw");
REXCVAR_DEFINE_BOOL(native_renderer_log_index_ranges, false, "ITC",
                    "Log how often draws' index ranges are scanned again unchanged since memcpy "
                    "last wrote them, and how the guest memory mirror's checks go");
REXCVAR_DEFINE_BOOL(native_renderer_log_texture_bindings, false, "ITC",
                    "Log how often draws' texture bindings repeat the last ones (fetch constants, "
                    "views, samplers, descriptor sets) and what would empty a cache of them");
REXCVAR_DEFINE_BOOL(native_renderer_cache_shader_lookups, true, "ITC",
                    "Keep draws' shader lookups by what they read from the shader objects "
                    "instead of hashing the shaders' microcode every draw");
REXCVAR_DEFINE_BOOL(native_renderer_log_shader_lookups, false, "ITC",
                    "Log how often draws' shader lookups repeat the last ones and whether a key "
                    "of the shader objects' fields would ever give a shader whose microcode "
                    "changed");
REXCVAR_DEFINE_BOOL(native_renderer_reuse_texture_sets, true, "ITC",
                    "Give a draw the last draw's texture descriptor set when its shader and the "
                    "fetch constants it reads are unchanged, and no texture changed");
REXCVAR_DEFINE_BOOL(native_renderer_copy_all, false, "ITC",
                    "Copy every vertex and index range into the mirror every frame, not only "
                    "those the library's memcpy wrote since (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_mirror_by_tick, false, "ITC",
                    "Draws read a guest memory mirror updated only as a logic pass's list starts "
                    "replaying, the two mirrors alternating (an experiment; see 'Hypothesis to "
                    "test: the mirrors updated once a tick' in docs/native-renderer.md)");
REXCVAR_DEFINE_BOOL(native_renderer_mirror_to_both, false, "ITC",
                    "With --native_renderer_mirror_by_tick: memcpy's writes go into both mirrors "
                    "at each frame's start, the one draws read included (an experiment)");
REXCVAR_DEFINE_BOOL(native_renderer_verify_mirror, false, "ITC",
                    "Compare each vertex and index range not copied into the mirror with guest "
                    "memory every frame, logging those that differ (a debugging aid; slow, ~20 "
                    "fps in a race). With --native_renderer_mirror_by_tick: each drawn range "
                    "looked up in the writes the mirror hasn't had instead (fast)");
REXCVAR_DEFINE_BOOL(native_renderer_watch_draws, false, "ITC",
                    "Log what changes in the main view's draws' inputs between frames: with the "
                    "camera still, only flickering draws should (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_hide_cubes, false, "ITC",
                    "Give cube maps the white placeholder (a debugging aid)");
REXCVAR_DEFINE_STRING(native_renderer_cycle, "", "ITC",
                      "Directory: when a file named 'now' appears in it, leave out the main "
                      "view's draws of each pixel shader in turn for 3 s, saving a frame of each "
                      "(a debugging aid that keeps frames recorded at their usual speed)");
REXCVAR_DEFINE_BOOL(native_renderer_check_late_writes, false, "ITC",
                    "Log vertex and index ranges written after we copied them, by the end of "
                    "the frame (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_check_clobbers, false, "ITC",
                    "Log copies into the mirror that change bytes an earlier draw's copy put "
                    "there in the same frame (a debugging aid)");
REXCVAR_DEFINE_STRING(native_renderer_watch_pixel_shader, "", "ITC",
                      "With native_renderer_watch_draws: only the draws of this pixel shader "
                      "(hash in hex)");
REXCVAR_DEFINE_BOOL(motion_blur, true, "ITC",
                    "Draw the game's velocity-based motion blur (pixel shader "
                    "7EB16456AAD56CBA); false leaves the frame unblurred");
REXCVAR_DEFINE_STRING(native_renderer_skip_pixel_shader, "", "ITC",
                      "Leave out the draws of this pixel shader (hash in hex; a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_log_glow, false, "ITC",
                    "Log the glow history's cross-fades and write-backs once a second, by logic "
                    "and render-only frames (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_base_level_only, false, "ITC",
                    "Sample only each texture's first level (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_verify_textures, false, "ITC",
                    "Hash every texture used each frame, logging and reloading those whose "
                    "memory changed with no memcpy write recorded (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_no_index_clamp, false, "ITC",
                    "Don't clamp indices to VGT_MIN/MAX_VTX_INDX (a debugging aid)");
REXCVAR_DEFINE_INT32(native_renderer_dump_every, 0, "ITC",
                     "With native_renderer_dump: dump only the frame itself (shown image, draw "
                     "IDs, draws.txt) every this many seconds of a race (a debugging aid)");
REXCVAR_DEFINE_STRING(native_renderer_dump, "", "ITC",
                      "Directory to dump frames into, as PFM files, with draws.txt describing "
                      "each draw: once 10 s into a race, and again whenever a file named 'now' "
                      "appears in it (a debugging aid; see docs/native-renderer.md)");
REXCVAR_DEFINE_INT32(width, 0, "ITC",
                     "Window width in pixels (in fullscreen, the drawing width); 0: 1280, or "
                     "from height at 16:9");
REXCVAR_DEFINE_INT32(height, 0, "ITC",
                     "Window height in pixels (in fullscreen, the drawing height); 0: 720, or "
                     "from width at 16:9");
// The SDK's (it also makes the SDK's own window, hidden in exclusive mode,
// fullscreen); ours follows it.
REXCVAR_DECLARE(bool, fullscreen);

namespace native {
namespace {

using Clock = std::chrono::steady_clock;

constexpr uint32_t kFramesInFlight = 2;
constexpr uint32_t kDefaultWidth = 1280, kDefaultHeight = 720;

// The window's size from --width and --height (one alone keeps 16:9).
VkExtent2D WindowSize() {
  int32_t width = REXCVAR_GET(width), height = REXCVAR_GET(height);
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
  bool sized = REXCVAR_GET(width) > 0 || REXCVAR_GET(height) > 0;
  VkExtent2D size = (REXCVAR_GET(fullscreen) && sized) || !pixels.width || !pixels.height
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

// Where a frame of `source`'s shape goes in `target`: as large as fits,
// centered (black bars on the sides or above and below).
VkOffset3D FitOffsets(VkExtent2D source, VkExtent2D target, VkOffset3D& end) {
  uint64_t width = target.width, height = target.height;
  if (uint64_t(target.width) * source.height > uint64_t(target.height) * source.width) {
    width = (uint64_t(target.height) * source.width + source.height / 2) / source.height;
  } else {
    height = (uint64_t(target.width) * source.height + source.width / 2) / source.width;
  }
  VkOffset3D start{int32_t((target.width - width) / 2), int32_t((target.height - height) / 2), 0};
  end = {start.x + int32_t(width), start.y + int32_t(height), 1};
  return start;
}

// Draw commands (see docs/gpu-functions.md).
bool IsDraw(uint32_t id) {
  return id == 15 || id == 16 || id == 17 || id == 18 || id == 19 || id == 27;
}

// The window and Vulkan instance, created on the main thread (SDL's window
// calls belong there; the SDK runs SDL's event loop on it).
struct WindowSetup {
  bool validation = false;
  SDL_Window* window = nullptr;
  VkExtent2D display{};  // its screen's size in pixels
  float density = 1.0f;  // pixels per unit of the desktop's
  VkInstance instance = VK_NULL_HANDLE;
  VkSurfaceKHR surface = VK_NULL_HANDLE;
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
  SDL_WindowFlags flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                          SDL_WINDOW_HIGH_PIXEL_DENSITY |
                          (REXCVAR_GET(fullscreen) ? SDL_WINDOW_FULLSCREEN : 0);
  setup->window =
      SDL_CreateWindow("ITC native renderer", int(size.width), int(size.height), flags);
  if (!setup->window) {
    return finish(fmt::format("SDL_CreateWindow: {}", SDL_GetError()));
  }
  if (const SDL_DisplayMode* mode =
          SDL_GetDesktopDisplayMode(SDL_GetDisplayForWindow(setup->window))) {
    setup->display = {uint32_t(std::lround(mode->w * mode->pixel_density)),
                      uint32_t(std::lround(mode->h * mode->pixel_density))};
  }
  // The window's size is given in pixels: in the desktop's (scaled) units,
  // it's that over the pixel density.
  if (float density = SDL_GetWindowPixelDensity(setup->window);
      density > 0.0f && density != 1.0f && !REXCVAR_GET(fullscreen)) {
    SDL_SetWindowSize(setup->window, int(std::lround(size.width / density)),
                      int(std::lround(size.height / density)));
    SDL_SyncWindow(setup->window);
  }
  setup->density = SDL_GetWindowPixelDensity(setup->window);
  g_window_id = SDL_GetWindowID(setup->window);
  SDL_AddEventWatch(WatchWindowEvents, nullptr);
  AppWindow_AdoptOurWindow(setup->window);
  auto get_instance_proc_addr =
      reinterpret_cast<PFN_vkGetInstanceProcAddr>(SDL_Vulkan_GetVkGetInstanceProcAddr());
  if (!get_instance_proc_addr || !vk::LoadGlobal(get_instance_proc_addr)) {
    return finish(fmt::format("no Vulkan loader: {}", SDL_GetError()));
  }
  Uint32 extension_count = 0;
  const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extension_count);
  uint32_t api_version = VK_API_VERSION_1_1;
  vk::vkEnumerateInstanceVersion(&api_version);
  VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app.pApplicationName = "ITC native renderer";
  app.apiVersion = std::min<uint32_t>(api_version, VK_API_VERSION_1_3);
  const char* validation_layer = "VK_LAYER_KHRONOS_validation";
  VkInstanceCreateInfo info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  info.pApplicationInfo = &app;
  info.enabledExtensionCount = extension_count;
  info.ppEnabledExtensionNames = extensions;
  if (setup->validation) {
    info.enabledLayerCount = 1;
    info.ppEnabledLayerNames = &validation_layer;
  }
  VkResult result = vk::vkCreateInstance(&info, nullptr, &setup->instance);
  if (result == VK_ERROR_LAYER_NOT_PRESENT) {
    REXLOG_WARN("native renderer: validation layer not installed, continuing without it");
    info.enabledLayerCount = 0;
    result = vk::vkCreateInstance(&info, nullptr, &setup->instance);
  }
  if (result != VK_SUCCESS) {
    return finish(fmt::format("vkCreateInstance: {}", int(result)));
  }
  if (!vk::LoadInstance(setup->instance)) {
    return finish("missing instance functions");
  }
  if (!SDL_Vulkan_CreateSurface(setup->window, setup->instance, nullptr, &setup->surface)) {
    return finish(fmt::format("SDL_Vulkan_CreateSurface: {}", SDL_GetError()));
  }
  finish("");
}

class Renderer {
 public:
  // Render thread. Returns false if the renderer can't run.
  bool Init(uint8_t* base);
  void OnCommand(uint32_t id, uint32_t arg1);
  void AfterCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1);
  void InlineDraw(uint8_t* base, uint32_t type, uint32_t count, uint32_t stride, uint32_t data);
  void TiledResolve(uint8_t* base, uint32_t device, uint32_t flags, uint32_t rect,
                    uint32_t destination, uint32_t point, uint32_t clear_color, float clear_z,
                    uint32_t tile, uint32_t tiles) {
    passes_.TiledResolve(BeginFrame(), base, device, flags, rect, destination, point, clear_color,
                         clear_z, tile, tiles);
  }
  void FrontBufferResolve(uint8_t* base, uint32_t device, uint32_t front_buffer) {
    passes_.FrontBufferResolve(BeginFrame(), base, device, front_buffer);
  }
  void EndFrame();

 private:
  bool CreateWindowAndInstance();
  bool CreateDevice();
  // Opens this frame's command buffer, once its previous use is done.
  VkCommandBuffer BeginFrame();
  bool CreateSwapchain();
  void DestroySwapchain();
  void LogStats();

  SDL_Window* window_ = nullptr;
  VkExtent2D display_{};  // the window's screen, in pixels
  VkInstance instance_ = VK_NULL_HANDLE;
  VkSurfaceKHR surface_ = VK_NULL_HANDLE;
  VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;
  uint32_t queue_family_ = 0;
  VkQueue queue_ = VK_NULL_HANDLE;

  VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
  VkFormat swapchain_format_ = VK_FORMAT_UNDEFINED;
  VkExtent2D swapchain_extent_{};
  std::vector<VkImage> swapchain_images_;
  std::vector<VkSemaphore> render_done_;  // one per swapchain image
  bool swapchain_stale_ = false;

  struct Frame {
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer commands = VK_NULL_HANDLE;
    VkFence done = VK_NULL_HANDLE;
    VkSemaphore image_ready = VK_NULL_HANDLE;
  };
  std::array<Frame, kFramesInFlight> frames_{};
  uint32_t frame_index_ = 0;

  DeviceContext context_{};
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

bool Renderer::CreateWindowAndInstance() {
  static Uint32 wakeup_event = SDL_RegisterEvents(1);
  auto setup = std::make_shared<WindowSetup>();
  setup->validation = REXCVAR_GET(native_renderer_validation);
  auto future = setup->done.get_future();
  // Queued for the main thread, which runs it as it pumps events; an event
  // of our own wakes it (the SDK's loop ignores event types it doesn't know).
  if (!SDL_RunOnMainThread(CreateWindowOnMainThread, new std::shared_ptr<WindowSetup>(setup),
                           false)) {
    REXLOG_ERROR("native renderer: SDL_RunOnMainThread: {}", SDL_GetError());
    return false;
  }
  SDL_Event wake{};
  wake.type = wakeup_event;
  SDL_PushEvent(&wake);
  if (future.wait_for(std::chrono::seconds(10)) != std::future_status::ready) {
    REXLOG_ERROR("native renderer: the main thread didn't create the window");
    return false;
  }
  if (!setup->error.empty()) {
    REXLOG_ERROR("native renderer: {}", setup->error);
    return false;
  }
  window_ = setup->window;
  display_ = setup->display;
  REXLOG_INFO("native renderer: window {}x{}{}, screen {}x{}, pixel density {}",
              WindowSize().width, WindowSize().height,
              REXCVAR_GET(fullscreen) ? " (fullscreen)" : "", display_.width, display_.height,
              setup->density);
  instance_ = setup->instance;
  surface_ = setup->surface;
  return true;
}

bool Renderer::CreateDevice() {
  uint32_t count = 0;
  vk::vkEnumeratePhysicalDevices(instance_, &count, nullptr);
  std::vector<VkPhysicalDevice> devices(count);
  vk::vkEnumeratePhysicalDevices(instance_, &count, devices.data());
  // A queue that draws and presents; discrete GPUs first.
  int best_score = -1;
  for (VkPhysicalDevice candidate : devices) {
    VkPhysicalDeviceProperties properties;
    vk::vkGetPhysicalDeviceProperties(candidate, &properties);
    uint32_t family_count = 0;
    vk::vkGetPhysicalDeviceQueueFamilyProperties(candidate, &family_count, nullptr);
    std::vector<VkQueueFamilyProperties> families(family_count);
    vk::vkGetPhysicalDeviceQueueFamilyProperties(candidate, &family_count, families.data());
    for (uint32_t i = 0; i < family_count; i++) {
      VkBool32 present = VK_FALSE;
      vk::vkGetPhysicalDeviceSurfaceSupportKHR(candidate, i, surface_, &present);
      if (!(families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) || !present) {
        continue;
      }
      int score = properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2 : 1;
      if (score > best_score) {
        best_score = score;
        physical_device_ = candidate;
        queue_family_ = i;
      }
      break;
    }
  }
  if (!physical_device_) {
    REXLOG_ERROR("native renderer: no GPU can draw to the window");
    return false;
  }
  VkPhysicalDeviceProperties properties;
  vk::vkGetPhysicalDeviceProperties(physical_device_, &properties);
  REXLOG_INFO("native renderer: using {}", properties.deviceName);

  // Dynamic rendering is core in Vulkan 1.3.
  if (properties.apiVersion < VK_API_VERSION_1_3) {
    REXLOG_ERROR("native renderer: needs Vulkan 1.3");
    return false;
  }

  // Features: what the GPU has, of what the translated shaders can use.
  VkPhysicalDeviceVulkan13Features supported13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
  VkPhysicalDeviceVulkan12Features supported12{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
  supported13.pNext = &supported12;
  VkPhysicalDeviceFeatures2 supported{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  supported.pNext = &supported13;
  vk::vkGetPhysicalDeviceFeatures2(physical_device_, &supported);
  if (!supported13.dynamicRendering) {
    REXLOG_ERROR("native renderer: needs dynamic rendering");
    return false;
  }
  VkPhysicalDeviceVulkan13Features enabled13{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
  enabled13.dynamicRendering = VK_TRUE;
  enabled13.shaderDemoteToHelperInvocation = supported13.shaderDemoteToHelperInvocation;
  VkPhysicalDeviceVulkan12Features enabled12{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
  enabled12.samplerMirrorClampToEdge = supported12.samplerMirrorClampToEdge;
  enabled13.pNext = &enabled12;
  VkPhysicalDeviceFeatures2 enabled{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  enabled.pNext = &enabled13;
  enabled.features.shaderClipDistance = supported.features.shaderClipDistance;
  enabled.features.shaderCullDistance = supported.features.shaderCullDistance;
  enabled.features.fullDrawIndexUint32 = supported.features.fullDrawIndexUint32;
  enabled.features.independentBlend = supported.features.independentBlend;
  enabled.features.samplerAnisotropy = supported.features.samplerAnisotropy;
  enabled.features.imageCubeArray = supported.features.imageCubeArray;

  float priority = 1.0f;
  VkDeviceQueueCreateInfo queue_info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
  queue_info.queueFamilyIndex = queue_family_;
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = &priority;
  const char* extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  VkDeviceCreateInfo info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  info.pNext = &enabled;
  info.queueCreateInfoCount = 1;
  info.pQueueCreateInfos = &queue_info;
  info.enabledExtensionCount = 1;
  info.ppEnabledExtensionNames = extensions;
  if (VkResult result = vk::vkCreateDevice(physical_device_, &info, nullptr, &device_);
      result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: vkCreateDevice: {}", int(result));
    return false;
  }
  if (!vk::LoadDevice(device_)) {
    return false;
  }
  vk::vkGetDeviceQueue(device_, queue_family_, 0, &queue_);

  context_.physical_device = physical_device_;
  context_.device = device_;
  context_.queue_family = queue_family_;
  context_.queue = queue_;
  context_.max_storage_buffer_range = properties.limits.maxStorageBufferRange;
  context_.min_uniform_buffer_offset_alignment =
      uint32_t(properties.limits.minUniformBufferOffsetAlignment);
  context_.clip_distance = enabled.features.shaderClipDistance;
  context_.cull_distance = enabled.features.shaderCullDistance;
  context_.demote_to_helper_invocation = enabled13.shaderDemoteToHelperInvocation;
  context_.sampler_anisotropy = enabled.features.samplerAnisotropy;
  context_.max_sampler_anisotropy = properties.limits.maxSamplerAnisotropy;
  context_.vendor_id = properties.vendorID;
  context_.device_id = properties.deviceID;
  std::copy(std::begin(properties.pipelineCacheUUID), std::end(properties.pipelineCacheUUID),
            context_.pipeline_cache_uuid.begin());
  context_.sampler_mirror_clamp_to_edge = enabled12.samplerMirrorClampToEdge;

  for (Frame& frame : frames_) {
    VkCommandPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family_;
    vk::vkCreateCommandPool(device_, &pool_info, nullptr, &frame.pool);
    VkCommandBufferAllocateInfo alloc{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    alloc.commandPool = frame.pool;
    alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc.commandBufferCount = 1;
    vk::vkAllocateCommandBuffers(device_, &alloc, &frame.commands);
    VkFenceCreateInfo fence_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    vk::vkCreateFence(device_, &fence_info, nullptr, &frame.done);
    VkSemaphoreCreateInfo semaphore_info{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    vk::vkCreateSemaphore(device_, &semaphore_info, nullptr, &frame.image_ready);
  }
  return true;
}

bool Renderer::CreateSwapchain() {
  VkSurfaceCapabilitiesKHR caps;
  vk::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_, surface_, &caps);
  VkExtent2D extent = caps.currentExtent;
  if (extent.width == UINT32_MAX) {
    // The window system leaves the size to us (Wayland).
    VkExtent2D size = WindowSize();
    int width = int(size.width), height = int(size.height);
    SDL_GetWindowSizeInPixels(window_, &width, &height);
    extent = {std::clamp<uint32_t>(width, caps.minImageExtent.width, caps.maxImageExtent.width),
              std::clamp<uint32_t>(height, caps.minImageExtent.height,
                                   caps.maxImageExtent.height)};
  }
  if (extent.width == 0 || extent.height == 0) {
    return false;  // minimized
  }

  uint32_t count = 0;
  vk::vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_, surface_, &count, nullptr);
  std::vector<VkSurfaceFormatKHR> formats(count);
  vk::vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_, surface_, &count, formats.data());
  VkSurfaceFormatKHR format = formats[0];
  for (const VkSurfaceFormatKHR& candidate : formats) {
    if (candidate.format == VK_FORMAT_B8G8R8A8_UNORM ||
        candidate.format == VK_FORMAT_R8G8B8A8_UNORM) {
      format = candidate;
      break;
    }
  }
  // Never wait for vblank: the render thread is the game's, and the frame
  // rate is paced elsewhere.
  vk::vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_, surface_, &count, nullptr);
  std::vector<VkPresentModeKHR> modes(count);
  vk::vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_, surface_, &count, modes.data());
  VkPresentModeKHR mode = VK_PRESENT_MODE_FIFO_KHR;
  for (VkPresentModeKHR wanted : {VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR}) {
    if (std::find(modes.begin(), modes.end(), wanted) != modes.end()) {
      mode = wanted;
      break;
    }
  }

  VkSwapchainCreateInfoKHR info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
  info.surface = surface_;
  info.minImageCount = std::max(caps.minImageCount, 3u);
  if (caps.maxImageCount) {
    info.minImageCount = std::min(info.minImageCount, caps.maxImageCount);
  }
  info.imageFormat = format.format;
  info.imageColorSpace = format.colorSpace;
  info.imageExtent = extent;
  info.imageArrayLayers = 1;
  info.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  info.preTransform = caps.currentTransform;
  info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  info.presentMode = mode;
  info.clipped = VK_TRUE;
  info.oldSwapchain = swapchain_;
  VkSwapchainKHR swapchain;
  if (VkResult result = vk::vkCreateSwapchainKHR(device_, &info, nullptr, &swapchain);
      result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: vkCreateSwapchainKHR: {}", int(result));
    return false;
  }
  DestroySwapchain();
  swapchain_ = swapchain;
  swapchain_format_ = format.format;
  swapchain_extent_ = extent;
  vk::vkGetSwapchainImagesKHR(device_, swapchain_, &count, nullptr);
  swapchain_images_.resize(count);
  vk::vkGetSwapchainImagesKHR(device_, swapchain_, &count, swapchain_images_.data());
  render_done_.resize(count);
  for (VkSemaphore& semaphore : render_done_) {
    VkSemaphoreCreateInfo semaphore_info{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    vk::vkCreateSemaphore(device_, &semaphore_info, nullptr, &semaphore);
  }
  swapchain_stale_ = false;
  REXLOG_INFO("native renderer: swapchain {}x{}, {} images, present mode {}", extent.width,
              extent.height, count, int(mode));
  return true;
}

void Renderer::DestroySwapchain() {
  if (!swapchain_) {
    return;
  }
  vk::vkDeviceWaitIdle(device_);
  for (VkSemaphore semaphore : render_done_) {
    vk::vkDestroySemaphore(device_, semaphore, nullptr);
  }
  render_done_.clear();
  swapchain_images_.clear();
  vk::vkDestroySwapchainKHR(device_, swapchain_, nullptr);
  swapchain_ = VK_NULL_HANDLE;
}

bool Renderer::Init(uint8_t* base) {
  if (!CreateWindowAndInstance() || !CreateDevice() || !CreateSwapchain() ||
      !passes_.Init(context_, base, kFramesInFlight, ResolutionScale(swapchain_extent_))) {
    return false;
  }
  passes_.set_mirror_by_tick(REXCVAR_GET(native_renderer_mirror_by_tick));
  passes_.set_mirror_to_both(REXCVAR_GET(native_renderer_mirror_to_both));
  return true;
}

void Renderer::OnCommand(uint32_t id, uint32_t arg1) {
  frame_commands_++;
  if (IsDraw(id)) {
    frame_draws_ += arg1;  // the handler makes one draw per item
  }
}

VkCommandBuffer Renderer::BeginFrame() {
  Frame& frame = frames_[frame_index_];
  if (!recording_) {
    vk::vkWaitForFences(device_, 1, &frame.done, VK_TRUE, UINT64_MAX);
    if (REXCVAR_GET(native_renderer_serialize)) {
      vk::vkQueueWaitIdle(queue_);
    }
    vk::vkResetCommandBuffer(frame.commands, 0);
    VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vk::vkBeginCommandBuffer(frame.commands, &begin);
    passes_.BeginFrame(frame_index_);
    // A dump starts 10 s into a race, or when asked for.
    const std::string& dump_root = REXCVAR_GET(native_renderer_dump);
    if (!dump_root.empty() && !dumping_) {
      std::error_code error;
      bool asked = std::filesystem::remove(dump_root + "/now", error);
      // With dump_every: a frame every that many seconds of a race.
      int32_t every = REXCVAR_GET(native_renderer_dump_every);
      bool in_race = passes_.frame_resolve_count() >= 20;
      bool periodic = every > 0 && in_race && Clock::now() - last_dump_ >= std::chrono::seconds(every);
      if (asked || periodic ||
          (every <= 0 && dumps_ == 0 && in_race && ++race_frames_ >= 600)) {
        last_dump_ = Clock::now();
        dumping_ = true;
        dump_step_ = 0;
        dump_steps_.clear();
        dump_directory_ = fmt::format("{}/{}", dump_root, dumps_++);
        std::filesystem::create_directories(dump_directory_, error);
        REXLOG_INFO("native renderer: dumping into {}", dump_directory_);
      }
    }
    // The cycle: started on request, a step every 3 s.
    const std::string& cycle_root = REXCVAR_GET(native_renderer_cycle);
    if (!cycle_root.empty()) {
      std::error_code error;
      if (cycle_steps_.empty() && std::filesystem::remove(cycle_root + "/now", error)) {
        cycle_steps_.push_back({"none", 0});
        for (uint64_t hash : passes_.main_view_pixel_shaders()) {
          cycle_steps_.push_back({fmt::format("skip_ps_{:016X}", hash), hash});
        }
        cycle_steps_.push_back({"skip_no_pixel_shader", Passes::kSkipNoPixelShader});
        cycle_steps_.push_back({"none_again", 0});
        cycle_step_ = 0;
        cycle_step_start_ = Clock::now();
        cycle_captured_ = false;
        cycle_directory_ = fmt::format("{}/{}", cycle_root, cycles_++);
        std::filesystem::create_directories(cycle_directory_, error);
        REXLOG_INFO("native renderer: cycling {} steps into {}", cycle_steps_.size(),
                    cycle_directory_);
      }
      if (!cycle_steps_.empty()) {
        passes_.set_skip_pixel_shader(cycle_steps_[cycle_step_].skip);
      }
    }
    if (const std::string& skip = REXCVAR_GET(native_renderer_skip_pixel_shader); !skip.empty()) {
      passes_.set_skip_pixel_shader(std::strtoull(skip.c_str(), nullptr, 16));
    }
    passes_.set_skip_motion_blur(!REXCVAR_GET(motion_blur));
    // Its first frame describes each draw (cheaply: the effect being looked
    // for went away while frames were recorded slowly).
    passes_.set_record_draws(dumping_ && dump_step_ == 0);
    passes_.set_check_mirror_frames(REXCVAR_GET(native_renderer_check_mirror));
    passes_.set_copy_all(REXCVAR_GET(native_renderer_copy_all));
    passes_.set_skip_repeated_state(REXCVAR_GET(native_renderer_skip_repeated_state));
    passes_.set_reuse_constants(REXCVAR_GET(native_renderer_reuse_constants));
    passes_.set_log_constant_changes(REXCVAR_GET(native_renderer_log_constant_changes));
    passes_.set_log_register_reads(REXCVAR_GET(native_renderer_log_register_reads));
    passes_.set_log_index_ranges(REXCVAR_GET(native_renderer_log_index_ranges));
    passes_.set_log_texture_bindings(REXCVAR_GET(native_renderer_log_texture_bindings));
    passes_.set_log_shader_lookups(REXCVAR_GET(native_renderer_log_shader_lookups));
    passes_.set_cache_shader_lookups(REXCVAR_GET(native_renderer_cache_shader_lookups));
    passes_.set_reuse_texture_sets(REXCVAR_GET(native_renderer_reuse_texture_sets));
    passes_.set_log_glow(REXCVAR_GET(native_renderer_log_glow));
    passes_.set_cache_index_ranges(REXCVAR_GET(native_renderer_cache_index_ranges));
    passes_.set_verify_mirror(REXCVAR_GET(native_renderer_verify_mirror));
    passes_.set_check_late_writes(REXCVAR_GET(native_renderer_check_late_writes));
    passes_.set_check_clobbers(REXCVAR_GET(native_renderer_check_clobbers));
    passes_.set_watch_draws(
        REXCVAR_GET(native_renderer_watch_draws),
        std::strtoull(REXCVAR_GET(native_renderer_watch_pixel_shader).c_str(), nullptr, 16));
    passes_.set_hide_cubes(REXCVAR_GET(native_renderer_hide_cubes));
    passes_.set_no_index_clamp(REXCVAR_GET(native_renderer_no_index_clamp));
    passes_.set_base_level_only(REXCVAR_GET(native_renderer_base_level_only));
    passes_.set_verify_textures(REXCVAR_GET(native_renderer_verify_textures));
    recording_ = true;
  }
  return frame.commands;
}

void Renderer::InlineDraw(uint8_t* base, uint32_t type, uint32_t count, uint32_t stride,
                          uint32_t data) {
  passes_.InlineDraw(BeginFrame(), base, type, count, stride, data);
}

void Renderer::AfterCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1) {
  // Commands 15 and 19 draw inline vertices, drawn at the library's End
  // (Renderer_InlineDraw).
  if (IsDraw(id) && id != 15 && id != 19) {
    passes_.AfterDraw(BeginFrame(), base, id, payload, arg1);
  } else if (id == 25) {
    passes_.AfterResolve(BeginFrame(), base, payload, arg1);
  } else if (id == 26) {
    passes_.AfterClear(BeginFrame(), base, payload, arg1);
  } else if (id == 32) {
    // One item of 16 bytes per tiling begun.
    for (uint32_t i = 0; i < arg1; i++) {
      passes_.BeginTiling(BeginFrame(), base, payload + 16 * i);
    }
  } else if (id == 33) {
    passes_.EndTiling();
  }
}

void Renderer::EndFrame() {
  commands_ += frame_commands_;
  draws_ += frame_draws_;
  uint32_t frame_draws = frame_draws_;
  frame_commands_ = frame_draws_ = 0;

  VkCommandBuffer commands = BeginFrame();
  // A dump: the frame as is (with the resolves), then frames with something
  // left out (see dump_steps_).
  bool dump = dumping_;
  // The cycle saves a frame 2 s into each step (the pause after it is over
  // by the next).
  bool cycle_capture = !cycle_steps_.empty() && !cycle_captured_ &&
                       Clock::now() - cycle_step_start_ >= std::chrono::seconds(2);
  VkExtent2D source =
      dump ? passes_.EndFrame(commands, REXCVAR_GET(native_renderer_show_resolve), true,
                              dump_step_ ? dump_steps_[dump_step_ - 1].name : "shown",
                              dump_step_ == 0 && REXCVAR_GET(native_renderer_dump_every) <= 0)
      : cycle_capture
          ? passes_.EndFrame(commands, REXCVAR_GET(native_renderer_show_resolve), true,
                             fmt::format("{:02}_{}", cycle_step_, cycle_steps_[cycle_step_].name),
                             false)
          : passes_.EndFrame(commands, REXCVAR_GET(native_renderer_show_resolve), false);
  Frame& frame = frames_[frame_index_];

  // Copy what's shown onto a swapchain image, if one can be had now.
  bool acquired = false;
  uint32_t image_index = 0;
  if (g_window_resized.exchange(false)) {
    swapchain_stale_ = true;
  }
  if ((!swapchain_stale_ && swapchain_) || CreateSwapchain()) {
    VkResult result = vk::vkAcquireNextImageKHR(device_, swapchain_, 100'000'000,
                                                frame.image_ready, VK_NULL_HANDLE, &image_index);
    if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
      acquired = true;
      swapchain_stale_ = result == VK_SUBOPTIMAL_KHR;  // present it, recreate next frame
    } else if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      swapchain_stale_ = true;
    } else if (result != VK_TIMEOUT && result != VK_NOT_READY) {
      REXLOG_ERROR("native renderer: vkAcquireNextImageKHR: {}", int(result));
    }
  }
  if (acquired) {
    VkImage image = swapchain_images_[image_index];
    VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);
    // The frame keeps its shape: black bars where the window's differs.
    VkImageBlit blit{};
    blit.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    blit.srcOffsets[1] = {int32_t(source.width), int32_t(source.height), 1};
    blit.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    blit.dstOffsets[0] = FitOffsets(source, swapchain_extent_, blit.dstOffsets[1]);
    if (blit.dstOffsets[0].x || blit.dstOffsets[0].y ||
        blit.dstOffsets[1].x != int32_t(swapchain_extent_.width) ||
        blit.dstOffsets[1].y != int32_t(swapchain_extent_.height)) {
      VkClearColorValue black{{0.0f, 0.0f, 0.0f, 1.0f}};
      VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
      vk::vkCmdClearColorImage(commands, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &black, 1,
                               &range);
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.oldLayout = barrier.newLayout;
      vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                               VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                               &barrier);
    }
    vk::vkCmdBlitImage(commands, passes_.shown(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = 0;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);
  }
  vk::vkEndCommandBuffer(commands);
  recording_ = false;

  vk::vkResetFences(device_, 1, &frame.done);
  VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  if (acquired) {
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &frame.image_ready;
    submit.pWaitDstStageMask = &wait_stage;
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &render_done_[image_index];
  }
  // Texture uploads first, then the frame.
  VkCommandBuffer command_buffers[2];
  submit.commandBufferCount = 0;
  if (VkCommandBuffer uploads = passes_.EndUploads()) {
    command_buffers[submit.commandBufferCount++] = uploads;
  }
  command_buffers[submit.commandBufferCount++] = commands;
  submit.pCommandBuffers = command_buffers;
  if (VkResult result = vk::vkQueueSubmit(queue_, 1, &submit, frame.done); result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: vkQueueSubmit: {}", int(result));
  }
  if (acquired) {
    VkPresentInfoKHR present{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present.waitSemaphoreCount = 1;
    present.pWaitSemaphores = &render_done_[image_index];
    present.swapchainCount = 1;
    present.pSwapchains = &swapchain_;
    present.pImageIndices = &image_index;
    VkResult result = vk::vkQueuePresentKHR(queue_, &present);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
      swapchain_stale_ = true;
    }
    frames_presented_++;
    // A frame much longer than usual (a hitch the per-second fps hides),
    // with what it spent on work that comes and goes (frame_costs.h).
    auto now = Clock::now();
    if (auto ms = std::chrono::duration<double, std::milli>(now - last_present_).count();
        ms >= 40 && frames_presented_ > 1) {
      const FrameCosts& c = g_frame_costs;
      REXLOG_INFO(
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
    vk::vkQueueWaitIdle(queue_);
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
      REXLOG_INFO("native renderer: cycle done");
    }
  }
  if (dump) {
    vk::vkQueueWaitIdle(queue_);
    passes_.WriteDump(dump_directory_);
    if (dump_step_ == 0 && REXCVAR_GET(native_renderer_dump_every) <= 0) {
      // Each pixel shader the main view was drawn with, left out; then all
      // textures from memory, and all resolve destinations, hidden.
      using Hide = TextureCache::Hide;
      for (uint64_t hash : passes_.main_view_pixel_shaders()) {
        dump_steps_.push_back({fmt::format("skip_ps_{:016X}", hash), Hide::kNone, 0, hash});
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
  frame_index_ = (frame_index_ + 1) % kFramesInFlight;
  LogStats();
}

void Renderer::LogStats() {
  auto now = Clock::now();
  if (now - last_log_ < std::chrono::seconds(5)) {
    return;
  }
  if (!InfoLogEnabled()) {
    // Nothing would be printed: leave the counters to run on.
    last_log_ = now;
    return;
  }
  double seconds = std::chrono::duration<double>(now - last_log_).count();
  uint64_t frames = std::max<uint64_t>(frames_presented_ + frames_skipped_, 1);
  Passes::Stats view = passes_.TakeStats();
  REXLOG_INFO("native renderer: {:.0f} frames/s presented, {} skipped; per frame {:.0f} "
              "commands, {:.0f} draws; {:.0f} drawn, {:.0f} unsupported, {:.0f} failed; {:.1f} "
              "MB copied a frame ({:.1f} MB unchanged, not copied); {} pipelines; shaders "
              "translated {}, failed {}, from the disk cache {}",
              frames_presented_ / seconds, frames_skipped_, double(commands_) / frames,
              double(draws_) / frames, double(view.drawn) / frames,
              double(view.unsupported) / frames, double(view.failed) / frames,
              view.mirrored_bytes / 1048576.0 / frames,
              view.mirror_skipped_bytes / 1048576.0 / frames, view.pipelines,
              view.shaders_translated, view.shaders_failed, view.shaders_from_disk);
  if (view.failed) {
    static constexpr const char* kFailedDrawReasons[] = {
        "no target", "no vertex shader", "shader translation", "no index buffer",
        "constants ring full", "texture set layout", "texture set",
        "inline vertex space full"};
    std::string reasons;
    for (size_t i = 0; i < view.failed_by.size(); i++) {
      if (view.failed_by[i]) {
        reasons += fmt::format(" {} {:.1f};", kFailedDrawReasons[i],
                               double(view.failed_by[i]) / frames);
      }
    }
    REXLOG_INFO("native renderer: failed draws a frame by reason:{}", reasons);
  }
  if (REXCVAR_GET(log_gpu_memory)) {
    REXLOG_INFO("native renderer: GPU memory: {}", gpu_memory::Describe());
  }
  if (std::string by_tick = passes_.TakeMirrorByTick(); !by_tick.empty()) {
    uint64_t missing, most;
    Renderer_TakeListStats(missing, most);
    REXLOG_INFO("native renderer: mirror by tick: {}; lists replayed without a hand-over {}, "
                "most waiting {}",
                by_tick, missing, most);
  }
  if (REXCVAR_GET(native_renderer_verify_mirror)) {
    REXLOG_INFO("native renderer: mirror check: of {} ranges not copied and checked, {} "
                "differed from guest memory ({:.1f} MB){}",
                view.mirror_checked, view.mirror_stale, view.mirror_stale_bytes / 1048576.0,
                view.mirror_stale ? fmt::format(", e.g. {} {:08X} ({} bytes)",
                                                view.mirror_stale_sample_index ? "index" : "vertex",
                                                view.mirror_stale_sample_address,
                                                view.mirror_stale_sample_size)
                                  : std::string());
  }
  REXLOG_INFO("native renderer: {} targets; per frame {:.1f} clears ({:.1f} with rects), {:.1f} "
              "resolves, {:.1f} skipped; {:.1f} draws with targets 1-3 bound, {:.1f} with "
              "mismatched sample counts, {:.1f} with a depth bias, {:.1f} with an equal depth "
              "test; {} resolves with unknown flags (e.g. {:08X}); {} tilings, {} tiled resolves",
              view.targets, double(view.clears) / frames, double(view.clear_rects) / frames,
              double(view.resolves) / frames, double(view.resolves_skipped) / frames,
              double(view.extra_targets) / frames, double(view.mismatched_samples) / frames,
              double(view.depth_biased) / frames, double(view.depth_equal) / frames,
              view.resolve_unknown_flags, view.resolve_flags_sample, view.tilings,
              view.tiled_resolves);
  frames_presented_ = frames_skipped_ = commands_ = draws_ = 0;
  const TextureCache::Stats& t = view.textures;
  REXLOG_INFO("native renderer: textures: {} ({} samplers); {} loaded, {} reloaded, {:.1f} MB "
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
  REXLOG_INFO("native renderer: textures: {} memcpy writes, {} textures marked written ({:.1f} "
              "ms marking)",
              t.writes, t.marked, mark_ns_ / 1e6);
  mark_ns_ = 0;
  if (t.unrecorded) {
    REXLOG_INFO("native renderer: {} textures changed in memory with no write recorded, e.g. "
                "{:08X}, {} frames after it was first loaded",
                t.unrecorded, t.unrecorded_sample, t.unrecorded_frames);
  }
  if (t.mismatched_sample != UINT32_MAX) {
    REXLOG_INFO("native renderer: e.g. a texture of format {}, dimension {}, {}x{} where the "
                "shader declares view type {}",
                (t.mismatched_sample >> 8) & 0xFF, t.mismatched_sample & 0xFF, t.mismatched_width,
                t.mismatched_height, t.mismatched_sample >> 16);
  }
  REXLOG_INFO("native renderer: {:.1f} draws a frame with PsParamGen;{}",
              double(view.param_gen_draws) / frames, passes_.TakeInterfaces());
  if (view.clamped_draws) {
    const auto& c = view.clamped_sample;
    REXLOG_INFO("native renderer: {:.1f} draws a frame have indices outside VGT_MIN/MAX_VTX_INDX, "
                "e.g. {}-{} against {}-{}",
                double(view.clamped_draws) / frames, c[0], c[1], c[2], c[3]);
  }
  if (view.inline_draws) {
    const auto& t = view.inline_by_type;
    REXLOG_INFO("native renderer: {:.1f} inline vertex draws a frame (triangle lists {:.1f}, "
                "strips {:.1f}, rectangle lists {:.1f}, quad lists {:.1f})",
                double(view.inline_draws) / frames, double(t[4]) / frames, double(t[6]) / frames,
                double(t[8]) / frames, double(t[13]) / frames);
  }
  if (view.state_compared) {
    const auto& r = view.state_repeated;
    auto share = [&](uint64_t n) { return 100.0 * double(n) / double(view.state_compared); };
    REXLOG_INFO("native renderer: of {:.1f} draws a frame following another draw, repeating its "
                "viewport {:.0f}%, scissor {:.0f}%, depth bias {:.0f}%, blend constants {:.0f}%, "
                "pipeline {:.0f}%, descriptor sets {:.0f}% (with offsets {:.0f}%), index buffer "
                "binding {:.0f}% (buffer and type {:.0f}%)",
                double(view.state_compared) / frames, share(r[0]), share(r[1]), share(r[2]),
                share(r[3]), share(r[4]), share(r[5]), share(r[8]), share(r[6]), share(r[7]));
    const auto& o = view.offset_differs;
    if (view.offsets_differ) {
      REXLOG_INFO("native renderer: descriptor sets repeated with other offsets for {:.0f}% of "
                  "draws: system {:.0f}%, vertex floats {:.0f}%, pixel floats {:.0f}%, "
                  "bool/loop {:.0f}%, fetch {:.0f}% (fetch alone {:.0f}%)",
                  share(view.offsets_differ), share(o[0]), share(o[1]), share(o[2]),
                  share(o[3]), share(o[4]), share(view.only_fetch_offset_differs));
    }
  }
  if (const RegisterReadStats& r = view.register_reads; r.calls) {
    auto share = [&](uint64_t n) { return 100.0 * double(n) / double(r.calls); };
    const auto& b = r.blocks_changed;
    REXLOG_INFO("native renderer: register reads: {:.1f} a frame, of {:.0f} words a frame {:.0f} "
                "copied (dirty), {:.1f} changed; {:.0f}% changed anything; changed by block: "
                "2000 {:.0f}%, 2100 {:.0f}%, 2180 {:.0f}%, 2200 {:.0f}%, 2280 {:.0f}%, 2300 "
                "{:.0f}%, 2380 {:.0f}%, fetch {:.0f}%, bool {:.0f}%, loop {:.0f}%",
                double(r.calls) / frames, double(r.words) / frames,
                double(r.words_copied) / frames, double(r.words_changed) / frames,
                share(r.calls_changed), share(b[0]), share(b[1]), share(b[2]), share(b[3]),
                share(b[4]), share(b[5]), share(b[6]), share(b[7]), share(b[8]), share(b[9]));
  }
  if (view.constants_draws) {
    const auto& r = view.constants_reused;
    auto share = [&](uint64_t n) { return 100.0 * double(n) / double(view.constants_draws); };
    REXLOG_INFO("native renderer: of {:.1f} draws a frame given constants, reusing the last "
                "written system {:.0f}%, vertex floats {:.0f}%, pixel floats {:.0f}%, bool/loop "
                "{:.0f}%, fetch {:.0f}%; {:.0f} KB written a frame",
                double(view.constants_draws) / frames, share(r[0]), share(r[1]), share(r[2]),
                share(r[3]), share(r[4]), double(view.constant_bytes) / frames / 1024);
    if (view.fetch_changed) {
      REXLOG_INFO("native renderer: fetch block differed from the last written for {:.0f}% of "
                  "draws: in a texture fetch constant read {:.0f}%, a vertex fetch constant "
                  "read {:.0f}% (address only {:.0f}%), only in unread ones {:.0f}%",
                  share(view.fetch_changed), share(view.fetch_changed_texture),
                  share(view.fetch_changed_vertex), share(view.fetch_changed_vertex_address),
                  share(view.fetch_changed_unread));
    }
  }
  if (view.vertex_shader_copies) {
    REXLOG_INFO("native renderer: {:.1f} draws a frame got their vertex shader from the library's "
                "patched copy in the command buffer",
                double(view.vertex_shader_copies) / frames);
  }
  if (view.declaration_mismatches) {
    REXLOG_INFO("native renderer: {:.1f} draws a frame had a vertex shader patched for another "
                "vertex declaration than the bound one ({:.1f} patched in their library call), "
                "e.g. {} (bound {})",
                double(view.declaration_mismatches) / frames,
                double(view.declaration_mismatches_patched) / frames, view.declaration_sample[0],
                view.declaration_sample[1]);
  }
  for (const std::string& line : passes_.TakeDeclarationLines()) {
    REXLOG_INFO("native renderer: declaration mismatch: {}", line);
  }
  if (view.base_vertex_differs) {
    REXLOG_INFO("native renderer: {:.1f} draws a frame have a base vertex in their item other "
                "than VGT_INDX_OFFSET, e.g. {} and {}",
                double(view.base_vertex_differs) / frames, int32_t(view.base_vertex_sample[0]),
                int32_t(view.base_vertex_sample[1]));
  }
  if (view.vertex_overruns) {
    const auto& v = view.vertex_overrun_sample;
    REXLOG_INFO("native renderer: {:.1f} draws a frame read vertices past their fetch "
                "constant's size, e.g. {:08X}: {} bytes, {} needed (vertex {}, stride {} words)",
                double(view.vertex_overruns) / frames, v[0], v[1], v[2], v[3], v[4]);
  }
  if (REXCVAR_GET(native_renderer_watch_draws)) {
    uint64_t changes, watched;
    std::vector<std::string> lines = passes_.TakeWatchLines(changes, watched);
    REXLOG_INFO("native renderer: {} of {} main view draws changed inputs since the previous "
                "frame", changes, watched);
    for (const std::string& line : lines) {
      REXLOG_INFO("native renderer: changed: {}", line);
    }
  }
  if (REXCVAR_GET(native_renderer_check_clobbers)) {
    REXLOG_INFO("native renderer: {}", passes_.TakeClobbers());
  }
  if (REXCVAR_GET(native_renderer_check_late_writes)) {
    REXLOG_INFO("native renderer: {}", passes_.TakeLateWrites());
  }
  if (REXCVAR_GET(native_renderer_check_mirror)) {
    REXLOG_INFO("native renderer: between frames, {}", passes_.TakeMirrorChanges());
  }
  if (REXCVAR_GET(native_renderer_log_index_ranges)) {
    REXLOG_INFO("native renderer: {}", passes_.TakeIndexRanges());
  }
  if (REXCVAR_GET(native_renderer_log_texture_bindings)) {
    REXLOG_INFO("native renderer: {}", passes_.TakeTextureBindings());
  }
  if (REXCVAR_GET(native_renderer_log_shader_lookups)) {
    REXLOG_INFO("native renderer: {}", passes_.TakeShaderLookups());
  }
  if (view.invisible) {
    const auto& v = view.invisible_sample;
    REXLOG_INFO("native renderer: {:.0f} draws a frame skipped as invisible, e.g. "
                "viewport {},{} {}x{}, scissor {},{} {}x{}",
                double(view.invisible) / frames, v[0] & 0x7FFFFFFF, v[1], v[2], v[3], v[4], v[5],
                v[6], v[7]);
  }
  last_log_ = now;
}

// Render thread only. Null until the first frame, and if it can't run.
Renderer* g_renderer = nullptr;
bool g_failed = false;

// Guest memory's base, from the command hooks (Renderer_EndFrame has none).
uint8_t* g_base = nullptr;

// Kept out of line: inlined, Init's stack frame was set up on every call to
// GetRenderer (several per replayed command), ~2% of the render thread.
[[gnu::noinline]] void StartRenderer() {
  g_renderer = new Renderer();
  if (!g_base || !g_renderer->Init(g_base)) {
    REXLOG_ERROR("native renderer: couldn't start; continuing without it");
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

bool Renderer_Enabled() {
  return REXCVAR_GET(native_renderer);
}

bool Renderer_Exclusive() {
  return REXCVAR_GET(native_renderer) && REXCVAR_GET(native_renderer_exclusive);
}

void Renderer_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                        uint32_t arg2) {
  if (!Renderer_Enabled()) {
    return;
  }
  g_base = base;
  Buffers_OnCommand(base, id, payload, arg1);
  if (Renderer* renderer = GetRenderer()) {
    renderer->OnCommand(id, arg1);
  }
}

void Renderer_AfterCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                           uint32_t arg2) {
  if (!Renderer_Enabled() || !(IsDraw(id) || id == 25 || id == 26 || id == 32 || id == 33)) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->AfterCommand(base, id, payload, arg1);
  }
}

void Renderer_InlineDraw(uint8_t* base, uint32_t type, uint32_t count, uint32_t stride,
                         uint32_t data) {
  if (!Renderer_Enabled()) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->InlineDraw(base, type, count, stride, data);
  }
}

void Renderer_TiledResolve(uint8_t* base, uint32_t device, uint32_t flags, uint32_t rect,
                           uint32_t destination, uint32_t point, uint32_t clear_color,
                           float clear_z, uint32_t tile, uint32_t tiles) {
  if (!Renderer_Enabled()) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->TiledResolve(base, device, flags, rect, destination, point, clear_color, clear_z,
                           tile, tiles);
  }
}

void Renderer_FrontBufferResolve(uint8_t* base, uint32_t device, uint32_t front_buffer) {
  if (!Renderer_Enabled()) {
    return;
  }
  g_base = base;
  if (Renderer* renderer = GetRenderer()) {
    renderer->FrontBufferResolve(base, device, front_buffer);
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
  if (!Renderer_Enabled() || !g_base) {
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
                                           "mirrors",  "constants", "other"};
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
    out += fmt::format("{} {:.1f} MB ({}); ", kKinds[i], bytes / 1048576.0,
                       totals.allocations[i].load(std::memory_order_relaxed));
  }
  out = fmt::format("{:.1f} MB: ", total / 1048576.0) + out;
  for (size_t i = 0; i < size_t(Object::kCount); i++) {
    out += fmt::format("{}{} {}", i ? ", " : "", kObjects[i],
                       totals.objects[i].load(std::memory_order_relaxed));
  }
  return out;
}

}  // namespace native::gpu_memory
