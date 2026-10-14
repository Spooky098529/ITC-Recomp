#include "renderer.h"
#include "frame_costs.h"
#include "log_enabled.h"

#include "buffers.h"
#include "app_window.h"
#include "state.h"

#include "passes.h"
#include "shaders.h"
#include "vulkan_functions.h"

#include "guest.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <array>
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
REXCVAR_DEFINE_BOOL(native_renderer_copy_all, false, "ITC",
                    "Copy every vertex and index range into the mirror every frame, not only "
                    "those the library's memcpy wrote since (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_verify_mirror, false, "ITC",
                    "Compare each vertex and index range not copied into the mirror with guest "
                    "memory every frame, logging those that differ (a debugging aid; slow, ~20 "
                    "fps in a race)");
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
REXCVAR_DEFINE_STRING(native_renderer_skip_pixel_shader, "", "ITC",
                      "Leave out the draws of this pixel shader (hash in hex; a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_base_level_only, false, "ITC",
                    "Sample only each texture's first level (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_verify_textures, false, "ITC",
                    "Hash every texture used again each frame, reloading those whose memory "
                    "changed (a debugging aid)");
REXCVAR_DEFINE_BOOL(native_renderer_no_index_clamp, false, "ITC",
                    "Don't clamp indices to VGT_MIN/MAX_VTX_INDX (a debugging aid)");
REXCVAR_DEFINE_INT32(native_renderer_dump_every, 0, "ITC",
                     "With native_renderer_dump: dump only the frame itself (shown image, draw "
                     "IDs, draws.txt) every this many seconds of a race (a debugging aid)");
REXCVAR_DEFINE_STRING(native_renderer_dump, "", "ITC",
                      "Directory to dump frames into, as PFM files, with draws.txt describing "
                      "each draw: once 10 s into a race, and again whenever a file named 'now' "
                      "appears in it (a debugging aid; see docs/native-renderer.md)");

namespace native {
namespace {

using Clock = std::chrono::steady_clock;

constexpr uint32_t kFramesInFlight = 2;
constexpr uint32_t kDefaultWidth = 1280, kDefaultHeight = 720;

// Draw commands (see docs/gpu-functions.md).
bool IsDraw(uint32_t id) {
  return id == 15 || id == 16 || id == 17 || id == 18 || id == 19 || id == 27;
}

// The window and Vulkan instance, created on the main thread (SDL's window
// calls belong there; the SDK runs SDL's event loop on it).
struct WindowSetup {
  bool validation = false;
  SDL_Window* window = nullptr;
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
  setup->window = SDL_CreateWindow("ITC native renderer", kDefaultWidth, kDefaultHeight,
                                   SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
  if (!setup->window) {
    return finish(fmt::format("SDL_CreateWindow: {}", SDL_GetError()));
  }
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
  int64_t texture_ns_ = 0, texture_ns_max_ = 0;
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
    int width = kDefaultWidth, height = kDefaultHeight;
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
  return CreateWindowAndInstance() && CreateDevice() && CreateSwapchain() &&
         passes_.Init(context_, base, kFramesInFlight);
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
    // Its first frame describes each draw (cheaply: the effect being looked
    // for went away while frames were recorded slowly).
    passes_.set_record_draws(dumping_ && dump_step_ == 0);
    passes_.set_check_mirror_frames(REXCVAR_GET(native_renderer_check_mirror));
    passes_.set_copy_all(REXCVAR_GET(native_renderer_copy_all));
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
    VkImageBlit blit{};
    blit.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    blit.srcOffsets[1] = {int32_t(source.width), int32_t(source.height), 1};
    blit.dstSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    blit.dstOffsets[1] = {int32_t(swapchain_extent_.width), int32_t(swapchain_extent_.height), 1};
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
  if (t.changed_after_load) {
    REXLOG_INFO("native renderer: {} textures changed soon after loading, e.g. {:08X} after {} "
                "frames",
                t.changed_after_load, t.changed_after_load_sample, t.changed_after_load_frames);
  }
  if (t.changed_later) {
    REXLOG_INFO("native renderer: {} textures changed in memory after their first frames, e.g. "
                "{:08X}, {} frames after it was last checked",
                t.changed_later, t.changed_later_sample, t.changed_later_frames);
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

Renderer* GetRenderer() {
  if (!g_renderer && !g_failed) {
    g_renderer = new Renderer();
    if (!g_base || !g_renderer->Init(g_base)) {
      REXLOG_ERROR("native renderer: couldn't start; continuing without it");
      g_failed = true;
      g_renderer = nullptr;  // leaked: may hold objects already made
    }
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

void Renderer_EndFrame() {
  if (!Renderer_Enabled() || !g_base) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->EndFrame();
  }
}

}  // namespace native
