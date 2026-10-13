#include "renderer.h"

#include "shaders.h"
#include "vulkan_functions.h"

#include "guest.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <future>
#include <memory>
#include <string>
#include <vector>
#include <rex/logging.h>
#include <rex/cvar.h>

REXCVAR_DEFINE_BOOL(native_renderer, false, "ITC",
                    "Draw the game's render commands with our own Vulkan renderer, in a "
                    "second window (in development; see docs/native-renderer.md)");
REXCVAR_DEFINE_BOOL(native_renderer_validation, false, "ITC",
                    "Enable the Vulkan validation layer for the native renderer, if installed");

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

// The graphics library's device and the fields read here (see
// docs/gpu-functions.md).
constexpr uint32_t kDevicePointer = 0x8296BD90;
constexpr uint32_t kDevicePixelShader = 12432, kDeviceVertexShader = 12436;
constexpr uint32_t kDeviceProgramControl = 10400;  // SQ_PROGRAM_CNTL
constexpr uint32_t kVertexShaderUcode = 48, kVertexShaderUcodeSize = 600;
constexpr uint32_t kPixelShaderUcode = 24, kPixelShaderUcodeSize = 64;

// The GPU's physical address for a guest address, as the graphics library
// converts it for the GPU (e.g. IM_LOAD in sub_822AED18, SetTexture).
uint32_t GpuPhysicalAddress(uint32_t address) {
  return (address & 0x1FFFFFFFu) + ((((address >> 20) + 0x200u) & 0x1000u));
}

// Guest physical memory is one 512 MB mapping, 4 GB above the virtual base
// (rex::memory::Memory: physical_membase = mapping base + 4 GB).
const uint8_t* PhysicalHostAddress(uint8_t* base, uint32_t physical) {
  return base + 0x100000000ull + physical;
}

// Host address of what the GPU reads at a guest address.
const uint8_t* HostAddress(uint8_t* base, uint32_t address) {
  return PhysicalHostAddress(base, GpuPhysicalAddress(address));
}

class Renderer {
 public:
  // Render thread. Returns false if the renderer can't run.
  bool Init();
  void OnCommand(uint32_t id, uint32_t arg1);
  void AfterDraw(uint8_t* base);
  void EndFrame();

 private:
  bool CreateWindowAndInstance();
  bool CreateDevice();
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

  ShaderCache shaders_;
  uint64_t draws_without_shaders_ = 0;

  // This frame's commands (milestone 1: counted only).
  uint32_t frame_commands_ = 0, frame_draws_ = 0;

  // Logged every 5 s.
  uint64_t frames_presented_ = 0, frames_skipped_ = 0, commands_ = 0, draws_ = 0;
  Clock::time_point start_ = Clock::now(), last_log_ = Clock::now();
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

  float priority = 1.0f;
  VkDeviceQueueCreateInfo queue_info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
  queue_info.queueFamilyIndex = queue_family_;
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = &priority;
  const char* extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  VkDeviceCreateInfo info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
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

bool Renderer::Init() {
  return CreateWindowAndInstance() && CreateDevice() && CreateSwapchain();
}

void Renderer::OnCommand(uint32_t id, uint32_t arg1) {
  frame_commands_++;
  if (IsDraw(id)) {
    frame_draws_ += arg1;  // the handler makes one draw per item
  }
}

void Renderer::AfterDraw(uint8_t* base) {
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  uint32_t program_control = guest::LoadU32(base, device + kDeviceProgramControl);
  uint32_t vertex_shader = guest::LoadU32(base, device + kDeviceVertexShader);
  uint32_t pixel_shader = guest::LoadU32(base, device + kDevicePixelShader);
  const TranslatedShader* vertex = nullptr;
  const TranslatedShader* pixel = nullptr;
  if (vertex_shader) {
    vertex = shaders_.Get(
        ShaderStage::kVertex,
        HostAddress(base, guest::LoadU32(base, vertex_shader + kVertexShaderUcode)),
        guest::LoadU32(base, vertex_shader + kVertexShaderUcodeSize), program_control & 0x3F);
  }
  if (pixel_shader) {
    pixel = shaders_.Get(
        ShaderStage::kPixel,
        HostAddress(base, guest::LoadU32(base, pixel_shader + kPixelShaderUcode)),
        guest::LoadU32(base, pixel_shader + kPixelShaderUcodeSize),
        (program_control >> 8) & 0x3F);
  }
  if (!vertex || (pixel_shader && !pixel)) {
    draws_without_shaders_++;
  }
}

void Renderer::EndFrame() {
  commands_ += frame_commands_;
  draws_ += frame_draws_;
  uint32_t draws = frame_draws_;
  frame_commands_ = frame_draws_ = 0;

  if ((swapchain_stale_ || !swapchain_) && !CreateSwapchain()) {
    frames_skipped_++;
    LogStats();
    return;
  }
  Frame& frame = frames_[frame_index_];
  vk::vkWaitForFences(device_, 1, &frame.done, VK_TRUE, UINT64_MAX);
  uint32_t image_index;
  VkResult result = vk::vkAcquireNextImageKHR(device_, swapchain_, 100'000'000,
                                              frame.image_ready, VK_NULL_HANDLE, &image_index);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_TIMEOUT || result == VK_NOT_READY) {
    swapchain_stale_ = result == VK_ERROR_OUT_OF_DATE_KHR;
    frames_skipped_++;
    LogStats();
    return;
  }
  if (result == VK_SUBOPTIMAL_KHR) {
    swapchain_stale_ = true;  // present this one, recreate next frame
  } else if (result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: vkAcquireNextImageKHR: {}", int(result));
    frames_skipped_++;
    return;
  }
  vk::vkResetFences(device_, 1, &frame.done);

  // Milestone 1: clear to a colour that shows the commands arrive: red with
  // the frame's draws (full at 3,000), blue pulsing with time.
  float seconds = std::chrono::duration<float>(Clock::now() - start_).count();
  VkClearColorValue colour{{std::min(draws / 3000.0f, 1.0f), 0.1f,
                            0.3f + 0.2f * std::sin(seconds * 2.0f), 1.0f}};
  VkImage image = swapchain_images_[image_index];
  VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

  VkCommandBuffer commands = frame.commands;
  vk::vkResetCommandBuffer(commands, 0);
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vk::vkBeginCommandBuffer(commands, &begin);
  VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange = range;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &barrier);
  vk::vkCmdClearColorImage(commands, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &colour, 1,
                           &range);
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = 0;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &barrier);
  vk::vkEndCommandBuffer(commands);

  VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit.waitSemaphoreCount = 1;
  submit.pWaitSemaphores = &frame.image_ready;
  submit.pWaitDstStageMask = &wait_stage;
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = &commands;
  submit.signalSemaphoreCount = 1;
  submit.pSignalSemaphores = &render_done_[image_index];
  vk::vkQueueSubmit(queue_, 1, &submit, frame.done);

  VkPresentInfoKHR present{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
  present.waitSemaphoreCount = 1;
  present.pWaitSemaphores = &render_done_[image_index];
  present.swapchainCount = 1;
  present.pSwapchains = &swapchain_;
  present.pImageIndices = &image_index;
  result = vk::vkQueuePresentKHR(queue_, &present);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    swapchain_stale_ = true;
  }
  frame_index_ = (frame_index_ + 1) % kFramesInFlight;
  frames_presented_++;
  LogStats();
}

void Renderer::LogStats() {
  auto now = Clock::now();
  if (now - last_log_ < std::chrono::seconds(5)) {
    return;
  }
  double seconds = std::chrono::duration<double>(now - last_log_).count();
  uint64_t frames = std::max<uint64_t>(frames_presented_ + frames_skipped_, 1);
  REXLOG_INFO("native renderer: {:.0f} frames/s presented, {} skipped; per frame {:.0f} "
              "commands, {:.0f} draws; shaders translated {}, failed {}; draws without "
              "shaders {}",
              frames_presented_ / seconds, frames_skipped_, double(commands_) / frames,
              double(draws_) / frames, shaders_.translated(), shaders_.failed(),
              draws_without_shaders_);
  frames_presented_ = frames_skipped_ = commands_ = draws_ = 0;
  last_log_ = now;
}

// Render thread only. Null until the first frame, and if it can't run.
Renderer* g_renderer = nullptr;
bool g_failed = false;

Renderer* GetRenderer() {
  if (!g_renderer && !g_failed) {
    g_renderer = new Renderer();
    if (!g_renderer->Init()) {
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

void Renderer_OnCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                        uint32_t arg2) {
  if (!Renderer_Enabled()) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->OnCommand(id, arg1);
  }
}

void Renderer_AfterCommand(uint8_t* base, uint32_t id, uint32_t payload, uint32_t arg1,
                           uint32_t arg2) {
  if (!Renderer_Enabled() || !IsDraw(id)) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->AfterDraw(base);
  }
}

void Renderer_EndFrame() {
  if (!Renderer_Enabled()) {
    return;
  }
  if (Renderer* renderer = GetRenderer()) {
    renderer->EndFrame();
  }
}

}  // namespace native
