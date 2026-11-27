#include "gpu/vulkan/backend.h"

#include <SDL3/SDL_vulkan.h>

#include <algorithm>
#include <cstring>
#include <format>
#include <memory>

#include "base/log.h"
#include "native/vulkan_types.h"

namespace gpu::vulkan {

namespace vk = native::vk;

namespace {

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

uint32_t FindMemoryType(const VkPhysicalDeviceMemoryProperties& properties, uint32_t type_bits,
                        VkMemoryPropertyFlags wanted) {
  for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
    if ((type_bits & (1u << i)) &&
        (properties.memoryTypes[i].propertyFlags & wanted) == wanted) {
      return i;
    }
  }
  return UINT32_MAX;
}

// Bytes a texel (a 4x4 block for the compressed formats).
uint32_t TexelBytes(Format format) {
  switch (format) {
    case Format::kR8Unorm: return 1;
    case Format::kR8G8Unorm: case Format::kR16Unorm: case Format::kR16Float: return 2;
    case Format::kR16G16B16A16Unorm: case Format::kR16G16B16A16Float: case Format::kR32G32Float:
    case Format::kR32G32Uint: case Format::kD32FloatS8Uint: case Format::kBC1: return 8;
    case Format::kR32G32B32Uint: return 12;
    case Format::kR32G32B32A32Float: case Format::kR32G32B32A32Uint: case Format::kBC2:
    case Format::kBC3: case Format::kBC5: return 16;
    case Format::kUndefined: return 0;
    default: return 4;
  }
}

bool IsCompressed(Format format) {
  return format == Format::kBC1 || format == Format::kBC2 || format == Format::kBC3 ||
         format == Format::kBC5;
}

}  // namespace

Backend::Backend(const Options& options) : options_(options) {
  frames_.resize(options.frames_in_flight);
}

Backend::~Backend() = default;

std::string Backend::AttachWindow(SDL_Window* window) {
  window_ = window;
  auto get_instance_proc_addr =
      reinterpret_cast<PFN_vkGetInstanceProcAddr>(SDL_Vulkan_GetVkGetInstanceProcAddr());
  if (!get_instance_proc_addr || !vk::LoadGlobal(get_instance_proc_addr)) {
    return std::format("no Vulkan loader: {}", SDL_GetError());
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
  if (options_.validation) {
    info.enabledLayerCount = 1;
    info.ppEnabledLayerNames = &validation_layer;
  }
  VkResult result = vk::vkCreateInstance(&info, nullptr, &instance_);
  if (result == VK_ERROR_LAYER_NOT_PRESENT) {
    ITC_LOG_WARN("native renderer: validation layer not installed, continuing without it");
    info.enabledLayerCount = 0;
    result = vk::vkCreateInstance(&info, nullptr, &instance_);
  }
  if (result != VK_SUCCESS) {
    return std::format("vkCreateInstance: {}", int(result));
  }
  if (!vk::LoadInstance(instance_)) {
    return "missing instance functions";
  }
  if (!SDL_Vulkan_CreateSurface(window, instance_, nullptr, &surface_)) {
    return std::format("SDL_Vulkan_CreateSurface: {}", SDL_GetError());
  }
  return {};
}

bool Backend::Init() { return CreateDevice() && CreateSwapchain(); }

bool Backend::CreateDevice() {
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
    ITC_LOG_ERROR("native renderer: no GPU can draw to the window");
    return false;
  }
  VkPhysicalDeviceProperties properties;
  vk::vkGetPhysicalDeviceProperties(physical_device_, &properties);
  ITC_LOG_INFO("native renderer: using {}", properties.deviceName);

  // Dynamic rendering is core in Vulkan 1.3.
  if (properties.apiVersion < VK_API_VERSION_1_3) {
    ITC_LOG_ERROR("native renderer: needs Vulkan 1.3");
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
    ITC_LOG_ERROR("native renderer: needs dynamic rendering");
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
    ITC_LOG_ERROR("native renderer: vkCreateDevice: {}", int(result));
    return false;
  }
  if (!vk::LoadDevice(device_)) {
    return false;
  }
  vk::vkGetDeviceQueue(device_, queue_family_, 0, &queue_);
  vk::vkGetPhysicalDeviceMemoryProperties(physical_device_, &memory_properties_);

  context_.physical_device = physical_device_;
  context_.device = device_;
  context_.queue_family = queue_family_;
  context_.queue = queue_;
  // The vertex shaders take up to 24 attributes and bindings (a fetch's
  // three, passes.cpp's kMaxVertexBindings; Vulkan promises 16).
  if (properties.limits.maxVertexInputAttributes < 24 ||
      properties.limits.maxVertexInputBindings < 24) {
    ITC_LOG_ERROR("native renderer: the device takes {} vertex attributes and {} bindings; the "
                  "game's shaders may need 24",
                  properties.limits.maxVertexInputAttributes,
                  properties.limits.maxVertexInputBindings);
  }
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

bool Backend::CreateSwapchain() {
  VkSurfaceCapabilitiesKHR caps;
  vk::vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_, surface_, &caps);
  VkExtent2D extent = caps.currentExtent;
  if (extent.width == UINT32_MAX) {
    // The window system leaves the size to us (Wayland).
    int width = int(options_.requested_size.width), height = int(options_.requested_size.height);
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
    ITC_LOG_ERROR("native renderer: vkCreateSwapchainKHR: {}", int(result));
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
  ITC_LOG_INFO("native renderer: swapchain {}x{}, {} images, present mode {}", extent.width,
               extent.height, count, int(mode));
  return true;
}

void Backend::DestroySwapchain() {
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

uint32_t Backend::BeginFrame() {
  Frame& frame = frames_[frame_index_];
  vk::vkWaitForFences(device_, 1, &frame.done, VK_TRUE, UINT64_MAX);
  vk::vkResetCommandBuffer(frame.commands, 0);
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vk::vkBeginCommandBuffer(frame.commands, &begin);
  return frame_index_;
}

bool Backend::EndFrame(Image* shown, Extent2D size) {
  Frame& frame = frames_[frame_index_];
  VkCommandBuffer commands = frame.commands;
  const VkExtent2D source{size.width, size.height};

  // Copy what's shown onto a swapchain image, if one can be had now.
  bool acquired = false;
  uint32_t image_index = 0;
  if (window_resized_.exchange(false)) {
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
      ITC_LOG_ERROR("native renderer: vkAcquireNextImageKHR: {}", int(result));
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
    vk::vkCmdBlitImage(commands, Object(shown).image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
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
  // The uploads first, then the frame.
  before_.push_back(commands);
  submit.commandBufferCount = uint32_t(before_.size());
  submit.pCommandBuffers = before_.data();
  if (VkResult result = vk::vkQueueSubmit(queue_, 1, &submit, frame.done); result != VK_SUCCESS) {
    ITC_LOG_ERROR("native renderer: vkQueueSubmit: {}", int(result));
  }
  before_.clear();
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
  }
  frame_index_ = (frame_index_ + 1) % uint32_t(frames_.size());
  return acquired;
}

void Backend::WaitIdle() { vk::vkQueueWaitIdle(queue_); }

template <typename Record>
bool Backend::RunNow(Record record) {
  VkCommandPool pool;
  VkCommandPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
  pool_info.queueFamilyIndex = queue_family_;
  if (vk::vkCreateCommandPool(device_, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  VkCommandBuffer commands;
  VkCommandBufferAllocateInfo alloc{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
  alloc.commandPool = pool;
  alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc.commandBufferCount = 1;
  vk::vkAllocateCommandBuffers(device_, &alloc, &commands);
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vk::vkBeginCommandBuffer(commands, &begin);
  record(commands);
  vk::vkEndCommandBuffer(commands);
  VkFence fence;
  VkFenceCreateInfo fence_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
  vk::vkCreateFence(device_, &fence_info, nullptr, &fence);
  VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = &commands;
  vk::vkQueueSubmit(queue_, 1, &submit, fence);
  vk::vkWaitForFences(device_, 1, &fence, VK_TRUE, UINT64_MAX);
  vk::vkDestroyFence(device_, fence, nullptr);
  vk::vkDestroyCommandPool(device_, pool, nullptr);
  return true;
}

Image* Backend::CreateImage(const ImageDesc& desc, const void* initial) {
  auto object = std::make_unique<ImageObject>();
  ImageObject& o = *object;
  o.desc = desc;
  o.format = vk_types::Format(desc.format);
  const bool depth = desc.format == Format::kD32FloatS8Uint;
  o.aspects = depth ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
                    : VK_IMAGE_ASPECT_COLOR_BIT;
  const uint32_t layers = desc.type == ImageType::kCube ? 6 : desc.layers;
  VkImageCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  info.flags = desc.type == ImageType::kCube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
  info.imageType = desc.type == ImageType::k3D ? VK_IMAGE_TYPE_3D : VK_IMAGE_TYPE_2D;
  info.format = o.format;
  info.extent = {desc.width, desc.height, desc.type == ImageType::k3D ? desc.depth : 1};
  info.mipLevels = desc.levels;
  info.arrayLayers = layers;
  info.samples = VkSampleCountFlagBits(desc.samples);
  info.tiling = VK_IMAGE_TILING_OPTIMAL;
  info.usage = ((desc.usage & kImageSampled) ? VK_IMAGE_USAGE_SAMPLED_BIT : 0) |
               ((desc.usage & kImageColorTarget) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0) |
               ((desc.usage & kImageDepthTarget) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                                                 : 0) |
               ((desc.usage & kImageCopySource) ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0) |
               ((desc.usage & kImageCopyDest) || initial ? VK_IMAGE_USAGE_TRANSFER_DST_BIT : 0) |
               ((desc.usage & kImageStorage) ? VK_IMAGE_USAGE_STORAGE_BIT : 0);
  info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (vk::vkCreateImage(device_, &info, nullptr, &o.image) != VK_SUCCESS) {
    return nullptr;
  }
  VkMemoryRequirements requirements;
  vk::vkGetImageMemoryRequirements(device_, o.image, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = FindMemoryType(memory_properties_, requirements.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device_, &alloc, nullptr, &o.memory) != VK_SUCCESS) {
    vk::vkDestroyImage(device_, o.image, nullptr);
    return nullptr;
  }
  o.memory_size = alloc.allocationSize;
  VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  view.image = o.image;
  view.viewType = desc.type == ImageType::k3D        ? VK_IMAGE_VIEW_TYPE_3D
                  : desc.type == ImageType::kCube    ? VK_IMAGE_VIEW_TYPE_CUBE
                  : desc.type == ImageType::k2DArray ? VK_IMAGE_VIEW_TYPE_2D_ARRAY
                                                     : VK_IMAGE_VIEW_TYPE_2D;
  view.format = o.format;
  view.subresourceRange = {o.aspects, 0, desc.levels, 0, layers};
  if (vk::vkBindImageMemory(device_, o.image, o.memory, 0) != VK_SUCCESS ||
      vk::vkCreateImageView(device_, &view, nullptr, &o.view) != VK_SUCCESS) {
    DestroyImage(reinterpret_cast<Image*>(object.release()));
    return nullptr;
  }
  if (depth) {
    view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (vk::vkCreateImageView(device_, &view, nullptr, &o.depth_view) != VK_SUCCESS) {
      DestroyImage(reinterpret_cast<Image*>(object.release()));
      return nullptr;
    }
  }
  if (initial) {
    // Through a staging buffer, then left ready to sample.
    const uint32_t blocks_wide = IsCompressed(desc.format) ? (desc.width + 3) / 4 : desc.width;
    const uint32_t blocks_high = IsCompressed(desc.format) ? (desc.height + 3) / 4 : desc.height;
    const VkDeviceSize layer_bytes = VkDeviceSize(blocks_wide) * blocks_high *
                                     (desc.type == ImageType::k3D ? desc.depth : 1) *
                                     TexelBytes(desc.format);
    VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    buffer_info.size = layer_bytes * layers;
    buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    void* mapped = nullptr;
    bool ok = vk::vkCreateBuffer(device_, &buffer_info, nullptr, &staging) == VK_SUCCESS;
    if (ok) {
      vk::vkGetBufferMemoryRequirements(device_, staging, &requirements);
      alloc.allocationSize = requirements.size;
      alloc.memoryTypeIndex = FindMemoryType(
          memory_properties_, requirements.memoryTypeBits,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      ok = alloc.memoryTypeIndex != UINT32_MAX &&
           vk::vkAllocateMemory(device_, &alloc, nullptr, &staging_memory) == VK_SUCCESS &&
           vk::vkBindBufferMemory(device_, staging, staging_memory, 0) == VK_SUCCESS &&
           vk::vkMapMemory(device_, staging_memory, 0, VK_WHOLE_SIZE, 0, &mapped) == VK_SUCCESS;
    }
    if (ok) {
      std::memcpy(mapped, initial, size_t(layer_bytes * layers));
      ok = RunNow([&](VkCommandBuffer commands) {
        VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = o.image;
        barrier.subresourceRange = {o.aspects, 0, desc.levels, 0, layers};
        vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                                 &barrier);
        VkBufferImageCopy copy{};
        copy.imageSubresource = {o.aspects, 0, 0, layers};
        copy.imageExtent = info.extent;
        vk::vkCmdCopyBufferToImage(commands, staging, o.image,
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                 0, 0, nullptr, 0, nullptr, 1, &barrier);
      });
    }
    if (staging) {
      vk::vkDestroyBuffer(device_, staging, nullptr);
    }
    if (staging_memory) {
      vk::vkFreeMemory(device_, staging_memory, nullptr);
    }
    if (!ok) {
      DestroyImage(reinterpret_cast<Image*>(object.release()));
      return nullptr;
    }
    o.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  }
  return reinterpret_cast<Image*>(object.release());
}

void Backend::DestroyImage(Image* image) {
  if (!image) {
    return;
  }
  ImageObject* o = &Object(image);
  if (o->depth_view) {
    vk::vkDestroyImageView(device_, o->depth_view, nullptr);
  }
  if (o->view) {
    vk::vkDestroyImageView(device_, o->view, nullptr);
  }
  if (o->image) {
    vk::vkDestroyImage(device_, o->image, nullptr);
  }
  if (o->memory) {
    vk::vkFreeMemory(device_, o->memory, nullptr);
  }
  delete o;
}

void Backend::Use(Image* image, Usage usage) {
  ImageObject& o = Object(image);
  const VkImageLayout wanted = vk_types::Layout(usage);
  if (o.layout == wanted || usage == Usage::kUndefined) {
    // Undefined: forgotten (the next use starts from nothing; no barrier).
    o.layout = wanted;
    return;
  }
  // Waiting for everything before.
  constexpr VkAccessFlags kAllAccess = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  barrier.srcAccessMask = kAllAccess;
  barrier.dstAccessMask = kAllAccess;
  barrier.oldLayout = o.layout;
  barrier.newLayout = wanted;
  barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = o.image;
  barrier.subresourceRange = {o.aspects, 0, VK_REMAINING_MIP_LEVELS, 0,
                              VK_REMAINING_ARRAY_LAYERS};
  vk::vkCmdPipelineBarrier(commands(), VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &barrier);
  o.layout = wanted;
}

Sampler* Backend::CreateSampler(const SamplerDesc& desc) {
  VkSamplerCreateInfo info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  info.magFilter = vk_types::Filter(desc.mag_filter);
  info.minFilter = vk_types::Filter(desc.min_filter);
  info.mipmapMode = vk_types::MipMode(desc.mip_mode);
  VkSamplerAddressMode* modes[3] = {&info.addressModeU, &info.addressModeV, &info.addressModeW};
  for (uint32_t i = 0; i < 3; i++) {
    VkSamplerAddressMode mode = vk_types::AddressMode(desc.address[i]);
    if (mode == VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE &&
        !context_.sampler_mirror_clamp_to_edge) {
      mode = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    }
    *modes[i] = mode;
  }
  if (desc.max_anisotropy >= 1.0f && context_.sampler_anisotropy) {
    info.anisotropyEnable = VK_TRUE;
    info.maxAnisotropy = std::min(desc.max_anisotropy, context_.max_sampler_anisotropy);
    info.magFilter = info.minFilter = VK_FILTER_LINEAR;
  }
  info.mipLodBias = desc.mip_lod_bias;
  info.minLod = desc.min_lod;
  info.maxLod = desc.max_lod;
  info.borderColor = vk_types::BorderColor(desc.border);
  VkSampler sampler;
  if (vk::vkCreateSampler(device_, &info, nullptr, &sampler) != VK_SUCCESS) {
    return nullptr;
  }
  return reinterpret_cast<Sampler*>(sampler);
}

}  // namespace gpu::vulkan
