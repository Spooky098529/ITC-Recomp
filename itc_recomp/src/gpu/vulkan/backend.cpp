#include "gpu/vulkan/backend.h"

#include <SDL3/SDL_vulkan.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <format>
#include <memory>

#include "base/log.h"
#include "base/hash.h"
#include "native/disk_cache.h"
#include "native/frame_costs.h"
#include "native/gpu_memory.h"
#include "native/vulkan_types.h"
#include "native/generated_gamma_comp.h"
#include "native/generated_id_frag.h"
#include "native/generated_movie_frag.h"
#include "native/generated_resolve_frag.h"
#include "native/generated_resolve_ms_frag.h"
#include "native/generated_resolve_vert.h"

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

// Texture descriptor sets (sets 2 and 3) in each of a frame's texture
// pools, and their descriptors; a frame that fills its pools gets another,
// kept for later frames.
constexpr uint32_t kTextureSetsPerPool = 8192;
constexpr uint32_t kTextureDescriptorsPerPool = 32768;

// This frame's texture descriptor sets by their contents: an open-addressed
// table of indices into the entries, the keys' words in one buffer, all
// kept between frames (the string-keyed map it replaced allocated a node
// and a key per set, and hashed and compared bytes: ~3% of the render
// thread with the pipelines' keys).
class FrameSetCache {
 public:
  // Returns the set slot for the key (words), and whether it was added.
  std::pair<VkDescriptorSet*, bool> FindOrAdd(const uint64_t* words, uint32_t count) {
    if (entries_.size() * 2 >= table_.size()) {
      Grow();
    }
    uint64_t hash = base::Hash64(words, count * sizeof(uint64_t));
    size_t mask = table_.size() - 1;
    for (size_t i = size_t(hash) & mask;; i = (i + 1) & mask) {
      int32_t index = table_[i];
      if (index < 0) {
        table_[i] = int32_t(entries_.size());
        entries_.push_back({hash, uint32_t(words_.size()), count, VK_NULL_HANDLE});
        words_.insert(words_.end(), words, words + count);
        return {&entries_.back().set, true};
      }
      Entry& e = entries_[index];
      if (e.hash == hash && e.count == count &&
          std::equal(words, words + count, words_.begin() + e.offset)) {
        return {&e.set, false};
      }
    }
  }

  void Clear() {
    entries_.clear();
    words_.clear();
    std::fill(table_.begin(), table_.end(), -1);
  }

 private:
  struct Entry {
    uint64_t hash;
    uint32_t offset, count;
    VkDescriptorSet set;
  };

  void Grow() {
    table_.assign(std::max<size_t>(table_.size() * 2, 8192), -1);
    size_t mask = table_.size() - 1;
    for (size_t k = 0; k < entries_.size(); k++) {
      size_t i = size_t(entries_[k].hash) & mask;
      while (table_[i] >= 0) {
        i = (i + 1) & mask;
      }
      table_[i] = int32_t(k);
    }
  }

  std::vector<int32_t> table_;
  std::vector<Entry> entries_;
  std::vector<uint64_t> words_;
};

Backend::Backend(const Options& options)
    : options_(options), texture_sets_(std::make_unique<FrameSetCache>()) {
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

bool Backend::Init() { return CreateDevice() && CreateSwapchain() && CreatePassResources(); }

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
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    if (vk::vkCreateCommandPool(device_, &pool_info, nullptr, &frame.upload_pool) != VK_SUCCESS) {
      return false;
    }
    alloc.commandPool = frame.upload_pool;
    if (vk::vkAllocateCommandBuffers(device_, &alloc, &frame.uploads) != VK_SUCCESS ||
        !AddTexturePool(frame)) {
      return false;
    }
  }
  // Set 0: empty (it held the vertex arena as a storage buffer until the
  // vertex shaders took vertex attributes; kept so the other sets keep
  // their numbers). Set 1: the constant blocks.
  VkDescriptorSetLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  if (vk::vkCreateDescriptorSetLayout(device_, &layout_info, nullptr, &empty_layout_) !=
      VK_SUCCESS) {
    return false;
  }
  constexpr VkShaderStageFlags kBoth = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  VkDescriptorSetLayoutBinding constants[4];
  for (uint32_t i = 0; i < 4; i++) {
    constants[i] = {i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, kBoth, nullptr};
  }
  layout_info.bindingCount = 4;
  layout_info.pBindings = constants;
  if (vk::vkCreateDescriptorSetLayout(device_, &layout_info, nullptr, &constants_layout_) !=
      VK_SUCCESS) {
    return false;
  }
  // Set 0's one set.
  VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1};
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &size;
  VkDescriptorPool pool;
  if (vk::vkCreateDescriptorPool(device_, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  set_alloc.descriptorPool = pool;
  set_alloc.descriptorSetCount = 1;
  set_alloc.pSetLayouts = &empty_layout_;
  return vk::vkAllocateDescriptorSets(device_, &set_alloc, &empty_set_) == VK_SUCCESS;
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
  if (!uploading_) {
    // (Uploads asked for before the first frame stay for it.)
    vk::vkResetCommandPool(device_, frame.upload_pool, 0);
  }
  texture_sets_->Clear();
  for (size_t i = 0; i <= frame.texture_pool && i < frame.texture_pools.size(); i++) {
    vk::vkResetDescriptorPool(device_, frame.texture_pools[i], 0);
  }
  frame.texture_pool = 0;
  vk::vkResetDescriptorPool(device_, frame.pass_pool, 0);
  MaybeSaveDiskCache();
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
  if (uploading_) {
    if (uploaded_buffers_) {
      // The buffers' ranges are read by the frame's vertex input and shaders.
      VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_INDEX_READ_BIT | VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
      vk::vkCmdPipelineBarrier(frame.uploads, VK_PIPELINE_STAGE_TRANSFER_BIT,
                               VK_PIPELINE_STAGE_VERTEX_INPUT_BIT |
                                   VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                                   VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                               0, 1, &barrier, 0, nullptr, 0, nullptr);
    }
    vk::vkEndCommandBuffer(frame.uploads);
    before_.insert(before_.begin(), frame.uploads);
    uploading_ = uploaded_buffers_ = false;
  }
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

void Backend::OpenDiskCache(const std::filesystem::path& directory) {
  if (!directory.empty()) {
    pipeline_cache_file_ = directory / "pipelines.bin";
  }
  std::vector<uint8_t> data;
  if (!pipeline_cache_file_.empty()) {
    if (std::FILE* in = native::OpenFile(pipeline_cache_file_, "rb")) {
      uint8_t buffer[65536];
      while (size_t n = std::fread(buffer, 1, sizeof(buffer), in)) {
        data.insert(data.end(), buffer, buffer + n);
      }
      std::fclose(in);
    }
  }
  // Only this device and driver's data (the header says whose it is).
  VkPipelineCacheHeaderVersionOne header{};
  bool matches = data.size() >= sizeof(header);
  if (matches) {
    std::memcpy(&header, data.data(), sizeof(header));
    matches = header.headerSize >= sizeof(header) &&
              header.headerVersion == VK_PIPELINE_CACHE_HEADER_VERSION_ONE &&
              header.vendorID == context_.vendor_id && header.deviceID == context_.device_id &&
              std::equal(std::begin(header.pipelineCacheUUID), std::end(header.pipelineCacheUUID),
                         context_.pipeline_cache_uuid.begin());
  }
  VkPipelineCacheCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO};
  if (matches) {
    info.initialDataSize = data.size();
    info.pInitialData = data.data();
  }
  if (vk::vkCreatePipelineCache(device_, &info, nullptr, &pipeline_cache_) != VK_SUCCESS) {
    pipeline_cache_ = VK_NULL_HANDLE;
  }
  pipeline_cache_saved_ = std::chrono::steady_clock::now();
  if (!pipeline_cache_file_.empty()) {
    ITC_LOG_INFO("native renderer: pipeline disk cache {}: {}", pipeline_cache_file_.string(),
                 matches        ? std::format("{} KB loaded", data.size() / 1024)
                 : data.empty() ? std::string("new")
                               : std::string("another device's or driver's; starting over"));
  }
}

void Backend::MaybeSaveDiskCache() {
  const size_t made = pipelines_.size() + id_pipelines_.size() + resolve_pipelines_.size() +
                      movie_pipelines_.size();
  auto now = std::chrono::steady_clock::now();
  if (!pipeline_cache_ || pipeline_cache_file_.empty() || made == pipeline_cache_saved_pipelines_ ||
      now - pipeline_cache_saved_ < std::chrono::seconds(10)) {
    return;
  }
  pipeline_cache_saved_pipelines_ = made;
  pipeline_cache_saved_ = now;
  size_t size = 0;
  if (vk::vkGetPipelineCacheData(device_, pipeline_cache_, &size, nullptr) != VK_SUCCESS) {
    return;
  }
  std::vector<uint8_t> data(size);
  if (vk::vkGetPipelineCacheData(device_, pipeline_cache_, &size, data.data()) !=
      VK_SUCCESS) {
    return;
  }
  // Written aside and renamed over, so an exit mid-write leaves the old file.
  std::filesystem::path temporary = pipeline_cache_file_;
  temporary += ".tmp";
  std::FILE* out = native::OpenFile(temporary, "wb");
  bool written = out && std::fwrite(data.data(), 1, size, out) == size;
  if (out) {
    written = std::fclose(out) == 0 && written;
  }
  std::error_code error;
  if (written) {
    std::filesystem::rename(temporary, pipeline_cache_file_, error);
  }
  if (!written || error) {
    ITC_LOG_WARN("native renderer: pipeline disk cache: couldn't write {}",
                 pipeline_cache_file_.string());
  }
}

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
  const uint32_t layers = desc.layers;
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

Buffer* Backend::CreateBuffer(const BufferDesc& desc) {
  auto object = std::make_unique<BufferObject>();
  BufferObject& o = *object;
  o.desc = desc;
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = desc.size;
  info.usage = ((desc.usage & kBufferVertex) ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : 0) |
               ((desc.usage & kBufferIndex) ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT : 0) |
               ((desc.usage & kBufferUniform) ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : 0) |
               ((desc.usage & kBufferStorage) ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : 0) |
               ((desc.usage & kBufferCopySource) ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT : 0) |
               ((desc.usage & kBufferCopyDest) ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0);
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device_, &info, nullptr, &o.buffer) != VK_SUCCESS) {
    return nullptr;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device_, o.buffer, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = FindMemoryType(
      memory_properties_, requirements.memoryTypeBits,
      desc.host_visible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
                        : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device_, &alloc, nullptr, &o.memory) != VK_SUCCESS ||
      vk::vkBindBufferMemory(device_, o.buffer, o.memory, 0) != VK_SUCCESS ||
      (desc.host_visible &&
       vk::vkMapMemory(device_, o.memory, 0, VK_WHOLE_SIZE, 0,
                       reinterpret_cast<void**>(&o.mapped)) != VK_SUCCESS)) {
    DestroyBuffer(reinterpret_cast<Buffer*>(object.release()));
    return nullptr;
  }
  o.memory_size = alloc.allocationSize;
  return reinterpret_cast<Buffer*>(object.release());
}

void Backend::DestroyBuffer(Buffer* buffer) {
  if (!buffer) {
    return;
  }
  BufferObject* o = &Object(buffer);
  if (o->buffer) {
    vk::vkDestroyBuffer(device_, o->buffer, nullptr);
  }
  if (o->memory) {
    vk::vkFreeMemory(device_, o->memory, nullptr);  // unmapped with it
  }
  delete o;
}

VkCommandBuffer Backend::Uploads() {
  VkCommandBuffer commands = frames_[frame_index_].uploads;
  if (!uploading_) {
    VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vk::vkBeginCommandBuffer(commands, &begin);
    uploading_ = true;
  }
  return commands;
}

void Backend::UploadImage(Buffer* staging, Image* image, const ImageUpload* regions,
                          uint32_t count) {
  ImageObject& o = Object(image);
  VkCommandBuffer commands = Uploads();
  // From what earlier frames' shaders read (or nothing, never uploaded).
  const bool fresh = o.layout == VK_IMAGE_LAYOUT_UNDEFINED;
  VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.oldLayout = fresh ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = o.image;
  barrier.subresourceRange = {o.aspects, 0, o.desc.levels, 0, o.desc.layers};
  vk::vkCmdPipelineBarrier(commands,
                           fresh ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
                                 : VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                                       VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
  std::vector<VkBufferImageCopy> copies(count);
  for (uint32_t i = 0; i < count; i++) {
    const ImageUpload& r = regions[i];
    VkBufferImageCopy& copy = copies[i];
    copy.bufferOffset = r.offset;
    copy.bufferRowLength = r.row_texels;
    copy.bufferImageHeight = r.rows;
    copy.imageSubresource = {o.aspects, r.level, r.layer, 1};
    copy.imageExtent = {r.width, r.height, 1};
  }
  vk::vkCmdCopyBufferToImage(commands, Handle(staging), o.image,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, count, copies.data());
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                           0, 0, nullptr, 0, nullptr, 1, &barrier);
  o.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

void Backend::UploadBuffer(Buffer* staging, Buffer* buffer, const BufferUpload& region) {
  VkCommandBuffer commands = Uploads();
  if (!uploaded_buffers_) {
    // Earlier frames' draws may still read ranges copied again in place:
    // the copies wait for them.
    VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 1, &barrier, 0, nullptr, 0,
                             nullptr);
    uploaded_buffers_ = true;
  }
  VkBufferCopy copy{region.source_offset, region.dest_offset, region.size};
  vk::vkCmdCopyBuffer(commands, Handle(staging), Handle(buffer), 1, &copy);
}

Shader* Backend::CreateShader(const ShaderDesc& desc) {
  auto object = std::make_unique<ShaderObject>();
  ShaderObject& o = *object;
  VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
  info.codeSize = desc.spirv_words * 4;
  info.pCode = desc.spirv;
  if (vk::vkCreateShaderModule(device_, &info, nullptr, &o.module) != VK_SUCCESS) {
    return nullptr;
  }
  // The texture set layout, by stage, the image types and the sampler count.
  const VkShaderStageFlags stage = desc.stage == ShaderStage::kVertex
                                       ? VK_SHADER_STAGE_VERTEX_BIT
                                       : VK_SHADER_STAGE_FRAGMENT_BIT;
  auto view_type = [](ImageType type) {
    return type == ImageType::k3D     ? VK_IMAGE_VIEW_TYPE_3D
           : type == ImageType::kCube ? VK_IMAGE_VIEW_TYPE_CUBE
           : type == ImageType::k2D   ? VK_IMAGE_VIEW_TYPE_2D
                                      : VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  };
  std::string key(1, char(stage));
  for (ImageType type : desc.textures) {
    key += char('0' + view_type(type));
  }
  key += ':' + std::to_string(desc.samplers);
  VkDescriptorSetLayout& layout = texture_layouts_[key];
  if (!layout) {
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    for (uint32_t i = 0; i < desc.textures.size(); i++) {
      bindings.push_back({i, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, stage, nullptr});
    }
    for (uint32_t i = 0; i < desc.samplers; i++) {
      bindings.push_back({uint32_t(desc.textures.size() + i), VK_DESCRIPTOR_TYPE_SAMPLER, 1,
                          stage, nullptr});
    }
    VkDescriptorSetLayoutCreateInfo layout_info{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    layout_info.bindingCount = uint32_t(bindings.size());
    layout_info.pBindings = bindings.data();
    if (vk::vkCreateDescriptorSetLayout(device_, &layout_info, nullptr, &layout) != VK_SUCCESS) {
      layout = VK_NULL_HANDLE;
    }
  }
  o.textures = layout;
  for (const VertexBinding& b : desc.bindings) {
    o.bindings.push_back({b.binding, b.stride,
                          b.per_instance ? VK_VERTEX_INPUT_RATE_INSTANCE
                                         : VK_VERTEX_INPUT_RATE_VERTEX});
  }
  for (const VertexAttribute& a : desc.attributes) {
    o.attributes.push_back({a.location, a.binding, vk_types::Format(a.format), a.offset});
  }
  return reinterpret_cast<Shader*>(object.release());
}

VkDescriptorSetLayout Backend::TexturesLayout(Shader* shader, ShaderStage stage) {
  if (shader) {
    return Object(shader).textures;
  }
  const VkShaderStageFlags flags =
      stage == ShaderStage::kVertex ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
  VkDescriptorSetLayout& layout = texture_layouts_[std::string(1, char(flags))];
  if (!layout) {
    VkDescriptorSetLayoutCreateInfo info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    if (vk::vkCreateDescriptorSetLayout(device_, &info, nullptr, &layout) != VK_SUCCESS) {
      layout = VK_NULL_HANDLE;
    }
  }
  return layout;
}

size_t Backend::DescHash::operator()(const PipelineDesc& desc) const {
  return size_t(base::Hash64(&desc, sizeof(desc)));
}

Pipeline* Backend::GetPipeline(const PipelineDesc& key, bool* made) {
  auto [it, added] = pipelines_.try_emplace(key);
  if (!added) {
    PipelineObject* o = it->second.get();
    return o && o->pipeline ? reinterpret_cast<Pipeline*>(o) : nullptr;
  }
  if (made) {
    *made = true;
  }
  auto object = std::make_unique<PipelineObject>();
  PipelineObject& o = *object;
  o.sets[0] = empty_layout_;
  o.sets[1] = constants_layout_;
  o.sets[2] = TexturesLayout(key.vertex, ShaderStage::kVertex);
  o.sets[3] = TexturesLayout(key.pixel, ShaderStage::kPixel);
  if (!key.vertex || !o.sets[2] || !o.sets[3]) {
    return nullptr;  // (kept as made: not tried again)
  }
  VkPipelineLayout& layout = pipeline_layouts_[{o.sets[2], o.sets[3]}];
  if (!layout) {
    VkPipelineLayoutCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    info.setLayoutCount = 4;
    info.pSetLayouts = o.sets;
    vk::vkCreatePipelineLayout(device_, &info, nullptr, &layout);
  }
  o.layout = layout;
  if (!layout) {
    return nullptr;
  }
  native::ScopedCost cost(native::g_frame_costs.pipeline_ns);
  native::g_frame_costs.pipelines++;
  o.pipeline = MakeGraphicsPipeline(key, key.pixel ? Object(key.pixel).module : VK_NULL_HANDLE,
                                    layout, true);
  it->second = std::move(object);
  return it->second->pipeline ? reinterpret_cast<Pipeline*>(it->second.get()) : nullptr;
}

VkPipeline Backend::MakeGraphicsPipeline(const PipelineDesc& key, VkShaderModule pixel,
                                         VkPipelineLayout layout, bool blend_constants) {
  const ShaderObject& vertex = Object(key.vertex);
  VkPipelineShaderStageCreateInfo stages[2] = {};
  uint32_t stage_count = 0;
  stages[stage_count] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
  stages[stage_count].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[stage_count].module = vertex.module;
  stages[stage_count++].pName = "main";
  if (pixel) {
    stages[stage_count] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    stages[stage_count].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[stage_count].module = pixel;
    stages[stage_count++].pName = "main";
  }
  VkPipelineVertexInputStateCreateInfo vertex_input{
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
  vertex_input.vertexBindingDescriptionCount = uint32_t(vertex.bindings.size());
  vertex_input.pVertexBindingDescriptions = vertex.bindings.data();
  vertex_input.vertexAttributeDescriptionCount = uint32_t(vertex.attributes.size());
  vertex_input.pVertexAttributeDescriptions = vertex.attributes.data();
  VkPipelineInputAssemblyStateCreateInfo input_assembly{
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
  input_assembly.topology = VkPrimitiveTopology(key.topology);  // the same order
  input_assembly.primitiveRestartEnable = key.primitive_restart;
  VkPipelineViewportStateCreateInfo viewport{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
  viewport.viewportCount = 1;
  viewport.scissorCount = 1;
  VkPipelineRasterizationStateCreateInfo raster{
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
  raster.polygonMode = VK_POLYGON_MODE_FILL;
  raster.cullMode = vk_types::CullMode(CullMode(key.cull_mode));
  raster.frontFace = key.front_clockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
  raster.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample.rasterizationSamples = VkSampleCountFlagBits(key.samples);
  VkPipelineDepthStencilStateCreateInfo depth{
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
  depth.depthTestEnable = key.z_enable;
  depth.depthWriteEnable = key.z_write;
  depth.depthCompareOp = vk_types::CompareOp(CompareOp(key.z_func));
  VkPipelineColorBlendAttachmentState attachment{};
  attachment.blendEnable = key.blend_enable;
  attachment.srcColorBlendFactor = vk_types::BlendFactor(BlendFactor(key.src_color));
  attachment.dstColorBlendFactor = vk_types::BlendFactor(BlendFactor(key.dst_color));
  attachment.colorBlendOp = vk_types::BlendOp(BlendOp(key.color_op));
  attachment.srcAlphaBlendFactor = vk_types::BlendFactor(BlendFactor(key.src_alpha));
  attachment.dstAlphaBlendFactor = vk_types::BlendFactor(BlendFactor(key.dst_alpha));
  attachment.alphaBlendOp = vk_types::BlendOp(BlendOp(key.alpha_op));
  attachment.colorWriteMask = key.write_mask;
  VkPipelineColorBlendStateCreateInfo blend{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
  VkFormat color_format = vk_types::Format(Format(key.color_format));
  bool has_color = color_format != VK_FORMAT_UNDEFINED;
  blend.attachmentCount = has_color ? 1 : 0;
  blend.pAttachments = &attachment;
  // Depth bias is set per draw.
  raster.depthBiasEnable = key.depth;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
                                     VK_DYNAMIC_STATE_DEPTH_BIAS, VK_DYNAMIC_STATE_BLEND_CONSTANTS};
  VkPipelineDynamicStateCreateInfo dynamic{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic.dynamicStateCount = blend_constants ? 4 : 3;
  dynamic.pDynamicStates = dynamic_states;
  VkPipelineRenderingCreateInfo rendering{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
  rendering.colorAttachmentCount = has_color ? 1 : 0;
  rendering.pColorAttachmentFormats = &color_format;
  rendering.depthAttachmentFormat =
      key.depth ? vk_types::Format(Format::kD32FloatS8Uint) : VK_FORMAT_UNDEFINED;
  rendering.stencilAttachmentFormat = rendering.depthAttachmentFormat;

  VkGraphicsPipelineCreateInfo info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
  info.pNext = &rendering;
  info.stageCount = stage_count;
  info.pStages = stages;
  info.pVertexInputState = &vertex_input;
  info.pInputAssemblyState = &input_assembly;
  info.pViewportState = &viewport;
  info.pRasterizationState = &raster;
  info.pMultisampleState = &multisample;
  info.pDepthStencilState = &depth;
  info.pColorBlendState = &blend;
  info.pDynamicState = &dynamic;
  info.layout = layout;
  VkPipeline pipeline = VK_NULL_HANDLE;
  if (VkResult result =
          vk::vkCreateGraphicsPipelines(device_, pipeline_cache_, 1, &info, nullptr, &pipeline);
      result != VK_SUCCESS) {
    ITC_LOG_ERROR("native renderer: vkCreateGraphicsPipelines: {}", int(result));
    return VK_NULL_HANDLE;
  }
  native::gpu_memory::Created(native::gpu_memory::Object::kPipeline);
  return pipeline;
}

bool Backend::AddTexturePool(Frame& frame) {
  VkDescriptorPoolSize sizes[] = {
      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, kTextureDescriptorsPerPool},
      {VK_DESCRIPTOR_TYPE_SAMPLER, kTextureDescriptorsPerPool},
  };
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = kTextureSetsPerPool;
  pool_info.poolSizeCount = 2;
  pool_info.pPoolSizes = sizes;
  VkDescriptorPool pool;
  if (vk::vkCreateDescriptorPool(device_, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  native::gpu_memory::Created(native::gpu_memory::Object::kDescriptorPool);
  frame.texture_pools.push_back(pool);
  return true;
}

TextureSet* Backend::MakeTextureSet(Shader* shader, ShaderStage stage, View* const* views,
                                    uint32_t view_count, Sampler* const* samplers,
                                    uint32_t sampler_count, bool* found) {
  VkDescriptorSetLayout layout = TexturesLayout(shader, stage);
  std::vector<VkDescriptorImageInfo>& infos = set_infos_;
  infos.resize(view_count + sampler_count);
  for (uint32_t i = 0; i < view_count; i++) {
    infos[i] = {VK_NULL_HANDLE, Handle(views[i]), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
  }
  for (uint32_t i = 0; i < sampler_count; i++) {
    infos[view_count + i] = {Handle(samplers[i]), VK_NULL_HANDLE, VK_IMAGE_LAYOUT_UNDEFINED};
  }
  // One set per distinct contents a frame, keyed by the layout and each
  // binding's sampler, view and layout (the fields, not the structs' bytes,
  // which have padding).
  std::vector<uint64_t>& key = set_key_;
  key.clear();
  key.push_back(uint64_t(reinterpret_cast<uintptr_t>(layout)));
  for (const VkDescriptorImageInfo& info : infos) {
    key.push_back(uint64_t(reinterpret_cast<uintptr_t>(info.sampler)));
    key.push_back(uint64_t(reinterpret_cast<uintptr_t>(info.imageView)));
    key.push_back(uint64_t(info.imageLayout));
  }
  auto [set, added] = texture_sets_->FindOrAdd(key.data(), uint32_t(key.size()));
  if (found) {
    *found = !added;
  }
  if (!added) {
    return reinterpret_cast<TextureSet*>(*set);
  }
  Frame& frame = frames_[frame_index_];
  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  alloc.descriptorPool = frame.texture_pools[frame.texture_pool];
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &layout;
  if (vk::vkAllocateDescriptorSets(device_, &alloc, set) != VK_SUCCESS) {
    // The pool in use is full: on to the next, made if needed.
    *set = VK_NULL_HANDLE;
    frame.texture_pool++;
    if (frame.texture_pool == frame.texture_pools.size()) {
      if (!AddTexturePool(frame)) {
        frame.texture_pool--;
        return nullptr;
      }
      ITC_LOG_INFO("native renderer: frame {} texture sets: pool {} added ({} sets each)",
                   frame_index_, frame.texture_pools.size(), kTextureSetsPerPool);
    }
    alloc.descriptorPool = frame.texture_pools[frame.texture_pool];
    if (vk::vkAllocateDescriptorSets(device_, &alloc, set) != VK_SUCCESS) {
      *set = VK_NULL_HANDLE;
      return nullptr;
    }
  }
  std::vector<VkWriteDescriptorSet>& writes = set_writes_;
  writes.assign(infos.size(), {});
  for (size_t i = 0; i < infos.size(); i++) {
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = *set;
    writes[i].dstBinding = uint32_t(i);
    writes[i].descriptorCount = 1;
    writes[i].descriptorType =
        i < view_count ? VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE : VK_DESCRIPTOR_TYPE_SAMPLER;
    writes[i].pImageInfo = &infos[i];
  }
  if (!writes.empty()) {
    vk::vkUpdateDescriptorSets(device_, uint32_t(writes.size()), writes.data(), 0, nullptr);
  }
  return reinterpret_cast<TextureSet*>(*set);
}

bool Backend::DescribeConstants(Buffer* buffer, const uint32_t ranges[4]) {
  VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 5};
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &size;
  VkDescriptorPool pool;
  if (vk::vkCreateDescriptorPool(device_, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  native::gpu_memory::Created(native::gpu_memory::Object::kDescriptorPool);
  VkDescriptorSet set;
  VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  set_alloc.descriptorPool = pool;
  set_alloc.descriptorSetCount = 1;
  set_alloc.pSetLayouts = &constants_layout_;
  if (vk::vkAllocateDescriptorSets(device_, &set_alloc, &set) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorBufferInfo buffers[4];
  VkWriteDescriptorSet writes[4];
  for (uint32_t i = 0; i < 4; i++) {
    buffers[i] = {Handle(buffer), 0, ranges[i]};
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = set;
    writes[i].dstBinding = i;
    writes[i].descriptorCount = 1;
    writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writes[i].pBufferInfo = &buffers[i];
  }
  vk::vkUpdateDescriptorSets(device_, 4, writes, 0, nullptr);
  constants_sets_[buffer] = set;
  return true;
}

void Backend::BindResources(Pipeline* pipeline, TextureSet* vertex, TextureSet* pixel,
                            Buffer* constants, const uint32_t offsets[4]) {
  // Without textures: the empty set (an empty layout's, compatible).
  VkDescriptorSet sets[4] = {empty_set_, constants_sets_[constants],
                             vertex ? Handle(vertex) : empty_set_,
                             pixel ? Handle(pixel) : empty_set_};
  vk::vkCmdBindDescriptorSets(commands(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                              Object(pipeline).layout, 0, 4, sets, 4, offsets);
}

void Backend::BeginPass(const PassTarget& color, const PassTarget& depth, const Rect& area) {
  auto attachment = [](const PassTarget& target, VkImageLayout layout) {
    VkRenderingAttachmentInfo a{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
    a.imageView = Object(target.image).view;
    a.imageLayout = layout;
    a.loadOp = target.load == LoadOp::kLoad    ? VK_ATTACHMENT_LOAD_OP_LOAD
               : target.load == LoadOp::kClear ? VK_ATTACHMENT_LOAD_OP_CLEAR
                                               : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    a.storeOp = target.store == StoreOp::kStore ? VK_ATTACHMENT_STORE_OP_STORE
                                                : VK_ATTACHMENT_STORE_OP_DONT_CARE;
    return a;
  };
  VkRenderingAttachmentInfo color_attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  VkRenderingAttachmentInfo depth_attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  info.renderArea = {{area.x, area.y}, {area.width, area.height}};
  info.layerCount = 1;
  if (color.image) {
    color_attachment = attachment(color, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    std::memcpy(color_attachment.clearValue.color.uint32, color.clear_color,
                sizeof(color.clear_color));
    info.colorAttachmentCount = 1;
    info.pColorAttachments = &color_attachment;
  }
  if (depth.image) {
    depth_attachment = attachment(depth, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    depth_attachment.clearValue.depthStencil = {depth.clear_depth, depth.clear_stencil};
    info.pDepthAttachment = &depth_attachment;
    info.pStencilAttachment = &depth_attachment;
  }
  vk::vkCmdBeginRendering(commands(), &info);
}

void Backend::EndPass() { vk::vkCmdEndRendering(commands()); }

void Backend::SetViewport(const Viewport& v) {
  VkViewport viewport{v.x, v.y, v.width, v.height, v.min_depth, v.max_depth};
  vk::vkCmdSetViewport(commands(), 0, 1, &viewport);
}

void Backend::SetScissor(const Rect& r) {
  VkRect2D scissor{{r.x, r.y}, {r.width, r.height}};
  vk::vkCmdSetScissor(commands(), 0, 1, &scissor);
}

void Backend::SetDepthBias(float constant, float slope) {
  vk::vkCmdSetDepthBias(commands(), constant, 0.0f, slope);
}

void Backend::SetBlendConstants(const float constants[4]) {
  vk::vkCmdSetBlendConstants(commands(), constants);
}

void Backend::BindPipeline(Pipeline* pipeline) {
  bound_pipeline_ = &Object(pipeline);
  vk::vkCmdBindPipeline(commands(), VK_PIPELINE_BIND_POINT_GRAPHICS, Object(pipeline).pipeline);
}

void Backend::BindVertexBuffers(Buffer* buffer, uint32_t count, const uint64_t* offsets) {
  VkBuffer buffers[32];
  count = std::min<uint32_t>(count, 32);
  std::fill(buffers, buffers + count, Handle(buffer));
  vk::vkCmdBindVertexBuffers(commands(), 0, count, buffers, offsets);
}

void Backend::BindIndexBuffer(Buffer* buffer, uint64_t offset, bool index_32bit) {
  vk::vkCmdBindIndexBuffer(commands(), Handle(buffer), offset,
                           index_32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16);
}

void Backend::Draw(uint32_t vertices, uint32_t instances, uint32_t first_vertex,
                   uint32_t first_instance) {
  vk::vkCmdDraw(commands(), vertices, instances, first_vertex, first_instance);
}

void Backend::DrawIndexed(uint32_t indices, uint32_t instances, uint32_t first_index,
                          int32_t base_vertex, uint32_t first_instance) {
  vk::vkCmdDrawIndexed(commands(), indices, instances, first_index, base_vertex, first_instance);
}

void Backend::ClearColor(Image* image, const uint32_t bits[4]) {
  const ImageObject& o = Object(image);
  VkClearColorValue value;
  std::memcpy(value.uint32, bits, sizeof(value.uint32));
  VkImageSubresourceRange range{o.aspects, 0, 1, 0, 1};
  vk::vkCmdClearColorImage(commands(), o.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value, 1,
                           &range);
}

void Backend::ClearDepthStencil(Image* image, float depth, uint32_t stencil, bool clear_depth,
                                bool clear_stencil) {
  const ImageObject& o = Object(image);
  VkClearDepthStencilValue value{depth, stencil};
  VkImageSubresourceRange range{
      VkImageAspectFlags((clear_depth ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) |
                         (clear_stencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0)),
      0, 1, 0, 1};
  vk::vkCmdClearDepthStencilImage(commands(), o.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  &value, 1, &range);
}

void Backend::CopyImage(Image* source, Image* dest, uint32_t width, uint32_t height) {
  const ImageObject& from = Object(source);
  VkImageCopy region{};
  region.srcSubresource = {from.aspects, 0, 0, 1};
  region.dstSubresource = region.srcSubresource;
  region.extent = {width, height, 1};
  vk::vkCmdCopyImage(commands(), from.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                     Object(dest).image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void Backend::BlitImage(Image* source, const Rect& from, Image* dest, const Rect& to,
                        Filter filter) {
  VkImageBlit blit{};
  blit.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
  blit.srcOffsets[0] = {from.x, from.y, 0};
  blit.srcOffsets[1] = {from.x + int32_t(from.width), from.y + int32_t(from.height), 1};
  blit.dstSubresource = blit.srcSubresource;
  blit.dstOffsets[0] = {to.x, to.y, 0};
  blit.dstOffsets[1] = {to.x + int32_t(to.width), to.y + int32_t(to.height), 1};
  vk::vkCmdBlitImage(commands(), Object(source).image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                     Object(dest).image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
                     vk_types::Filter(filter));
}

void Backend::ResolveImage(Image* source, Image* dest, uint32_t width, uint32_t height) {
  VkImageResolve region{};
  region.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
  region.dstSubresource = region.srcSubresource;
  region.extent = {width, height, 1};
  vk::vkCmdResolveImage(commands(), Object(source).image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                        Object(dest).image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void Backend::CopyBufferToImage(Buffer* buffer, Image* image, const ImageUpload* regions,
                                uint32_t count) {
  const ImageObject& o = Object(image);
  std::vector<VkBufferImageCopy> copies(count);
  for (uint32_t i = 0; i < count; i++) {
    const ImageUpload& r = regions[i];
    copies[i] = {};
    copies[i].bufferOffset = r.offset;
    copies[i].bufferRowLength = r.row_texels;
    copies[i].bufferImageHeight = r.rows;
    copies[i].imageSubresource = {o.aspects, r.level, r.layer, 1};
    copies[i].imageExtent = {r.width, r.height, 1};
  }
  vk::vkCmdCopyBufferToImage(commands(), Handle(buffer), o.image,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, count, copies.data());
}

void Backend::CopyImageToBuffer(Image* image, Buffer* buffer, uint64_t offset, uint32_t width,
                                uint32_t height) {
  const ImageObject& o = Object(image);
  VkBufferImageCopy copy{};
  copy.bufferOffset = offset;
  copy.imageSubresource = {o.depth_view ? VkImageAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT) : o.aspects,
                           0, 0, 1};
  copy.imageExtent = {width, height, 1};
  vk::vkCmdCopyImageToBuffer(commands(), o.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                             Handle(buffer), 1, &copy);
}

View* Backend::CreateView(Image* image, const ViewDesc& desc) {
  const ImageObject& o = Object(image);
  auto component = [](Swizzle swizzle) {
    switch (swizzle) {
      case Swizzle::kZero: return VK_COMPONENT_SWIZZLE_ZERO;
      case Swizzle::kOne: return VK_COMPONENT_SWIZZLE_ONE;
      default: return VkComponentSwizzle(VK_COMPONENT_SWIZZLE_R + uint32_t(swizzle));
    }
  };
  VkImageViewCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  info.image = o.image;
  info.viewType = desc.type == ImageType::k3D        ? VK_IMAGE_VIEW_TYPE_3D
                  : desc.type == ImageType::kCube    ? VK_IMAGE_VIEW_TYPE_CUBE
                  : desc.type == ImageType::k2DArray ? VK_IMAGE_VIEW_TYPE_2D_ARRAY
                                                     : VK_IMAGE_VIEW_TYPE_2D;
  info.format = o.format;
  info.components = {component(desc.swizzle[0]), component(desc.swizzle[1]),
                     component(desc.swizzle[2]), component(desc.swizzle[3])};
  info.subresourceRange = {desc.depth_only ? VkImageAspectFlags(VK_IMAGE_ASPECT_DEPTH_BIT)
                                           : o.aspects,
                           0, o.desc.levels, 0, o.desc.layers};
  VkImageView view;
  if (vk::vkCreateImageView(device_, &info, nullptr, &view) != VK_SUCCESS) {
    return nullptr;
  }
  return reinterpret_cast<View*>(view);
}

void Backend::DestroyView(View* view) {
  vk::vkDestroyImageView(device_, Handle(view), nullptr);
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

// The resolve and movie passes.

namespace {
// Their sets a frame (one sampled image each for a resolve, three for a
// movie).
constexpr uint32_t kResolvesPerFrame = 256;
constexpr uint32_t kMoviesPerFrame = 16;
}  // namespace

bool Backend::CreatePassResources() {
  auto module = [&](const uint32_t* code, size_t size, VkShaderModule& out) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = size;
    info.pCode = code;
    return vk::vkCreateShaderModule(device_, &info, nullptr, &out) == VK_SUCCESS;
  };
  auto layouts = [&](uint32_t images, uint32_t push_bytes, VkDescriptorSetLayout& set_layout,
                     VkPipelineLayout& layout) {
    VkDescriptorSetLayoutBinding bindings[3];
    for (uint32_t i = 0; i < images; i++) {
      bindings[i] = {i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
                     nullptr};
    }
    VkDescriptorSetLayoutCreateInfo set_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    set_info.bindingCount = images;
    set_info.pBindings = bindings;
    if (vk::vkCreateDescriptorSetLayout(device_, &set_info, nullptr, &set_layout) != VK_SUCCESS) {
      return false;
    }
    VkPushConstantRange push{VK_SHADER_STAGE_FRAGMENT_BIT, 0, push_bytes};
    VkPipelineLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    layout_info.setLayoutCount = 1;
    layout_info.pSetLayouts = &set_layout;
    layout_info.pushConstantRangeCount = 1;
    layout_info.pPushConstantRanges = &push;
    return vk::vkCreatePipelineLayout(device_, &layout_info, nullptr, &layout) == VK_SUCCESS;
  };
  if (!layouts(1, 20, resolve_set_layout_, resolve_layout_) ||
      !layouts(3, 16, movie_set_layout_, movie_layout_) ||
      !module(k_resolve_vert, sizeof(k_resolve_vert), resolve_vertex_) ||
      !module(k_resolve_frag, sizeof(k_resolve_frag), resolve_pixel_) ||
      !module(k_resolve_ms_frag, sizeof(k_resolve_ms_frag), resolve_pixel_ms_) ||
      !module(k_movie_frag, sizeof(k_movie_frag), movie_pixel_)) {
    return false;
  }
  SamplerDesc sampler;
  sampler.address[0] = sampler.address[1] = sampler.address[2] = AddressMode::kClampToEdge;
  resolve_sampler_ = Handle(CreateSampler(sampler));
  // Bilinear, as the movie library's sampler states.
  sampler.mag_filter = sampler.min_filter = Filter::kLinear;
  movie_sampler_ = Handle(CreateSampler(sampler));
  if (!resolve_sampler_ || !movie_sampler_) {
    return false;
  }
  for (Frame& frame : frames_) {
    VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                              kResolvesPerFrame + 3 * kMoviesPerFrame};
    VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    pool_info.maxSets = kResolvesPerFrame + kMoviesPerFrame;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &size;
    if (vk::vkCreateDescriptorPool(device_, &pool_info, nullptr, &frame.pass_pool) !=
        VK_SUCCESS) {
      return false;
    }
  }
  return true;
}

VkDescriptorSet Backend::PassSet(VkDescriptorSetLayout layout) {
  VkDescriptorSet set = VK_NULL_HANDLE;
  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  alloc.descriptorPool = frames_[frame_index_].pass_pool;
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &layout;
  if (vk::vkAllocateDescriptorSets(device_, &alloc, &set) != VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  return set;
}

VkPipeline Backend::MakeFullscreenPipeline(VkShaderModule pixel, VkPipelineLayout layout,
                                           VkFormat format, VkSampleCountFlagBits samples) {
  VkPipelineShaderStageCreateInfo stages[2] = {};
  for (VkPipelineShaderStageCreateInfo& stage : stages) {
    stage = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    stage.pName = "main";
  }
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = resolve_vertex_;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = pixel;
  VkPipelineVertexInputStateCreateInfo vertex_input{
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
  VkPipelineInputAssemblyStateCreateInfo input_assembly{
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkPipelineViewportStateCreateInfo viewport{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
  viewport.viewportCount = 1;
  viewport.scissorCount = 1;
  VkPipelineRasterizationStateCreateInfo raster{
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
  raster.polygonMode = VK_POLYGON_MODE_FILL;
  raster.cullMode = VK_CULL_MODE_NONE;
  raster.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample_state{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample_state.rasterizationSamples = samples;
  VkPipelineColorBlendAttachmentState attachment{};
  attachment.colorWriteMask = 0xF;
  VkPipelineColorBlendStateCreateInfo blend{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
  blend.attachmentCount = 1;
  blend.pAttachments = &attachment;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamic{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic.dynamicStateCount = 2;
  dynamic.pDynamicStates = dynamic_states;
  VkPipelineRenderingCreateInfo rendering{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
  rendering.colorAttachmentCount = 1;
  rendering.pColorAttachmentFormats = &format;
  VkGraphicsPipelineCreateInfo info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
  info.pNext = &rendering;
  info.stageCount = 2;
  info.pStages = stages;
  info.pVertexInputState = &vertex_input;
  info.pInputAssemblyState = &input_assembly;
  info.pViewportState = &viewport;
  info.pRasterizationState = &raster;
  info.pMultisampleState = &multisample_state;
  info.pColorBlendState = &blend;
  info.pDynamicState = &dynamic;
  info.layout = layout;
  VkPipeline pipeline = VK_NULL_HANDLE;
  if (vk::vkCreateGraphicsPipelines(device_, pipeline_cache_, 1, &info, nullptr, &pipeline) !=
      VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  return pipeline;
}

bool Backend::DrawResolve(const ResolveDraw& draw) {
  ImageObject& source = Object(draw.source);
  ImageObject& dest = Object(draw.dest);
  const bool multisample = source.desc.samples > 1;
  auto [it, added] = resolve_pipelines_.try_emplace({dest.format, multisample}, VK_NULL_HANDLE);
  if (added) {
    it->second = MakeFullscreenPipeline(multisample ? resolve_pixel_ms_ : resolve_pixel_,
                                        resolve_layout_, dest.format, VK_SAMPLE_COUNT_1_BIT);
    if (!it->second) {
      ITC_LOG_ERROR("native renderer: couldn't create a resolve pipeline for format {}",
                    int(dest.format));
    }
  }
  const VkPipeline pipeline = it->second;
  if (!pipeline) {
    return false;
  }
  const VkDescriptorSet set = PassSet(resolve_set_layout_);
  if (!set) {
    return false;
  }
  // A depth image's depth alone, in the layout its sampled use left it.
  VkDescriptorImageInfo image_info{resolve_sampler_, source.depth_view ? source.depth_view
                                                                       : source.view,
                                   source.layout};
  VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
  write.dstSet = set;
  write.descriptorCount = 1;
  write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  write.pImageInfo = &image_info;
  vk::vkUpdateDescriptorSets(device_, 1, &write, 0, nullptr);
  PassTarget target;
  target.image = draw.dest;
  BeginPass(target, {}, draw.area);
  const VkCommandBuffer commands = this->commands();
  SetViewport({float(draw.area.x), float(draw.area.y), float(draw.area.width),
               float(draw.area.height), 0.0f, 1.0f});
  SetScissor(draw.area);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, resolve_layout_, 0, 1,
                              &set, 0, nullptr);
  struct {
    int32_t offset[2];
    int32_t samples;
    float scale;
    int32_t swap_red_blue;
  } constants{{draw.offset[0], draw.offset[1]}, draw.samples, draw.scale,
              draw.swap_red_blue ? 1 : 0};
  vk::vkCmdPushConstants(commands, resolve_layout_, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(constants), &constants);
  vk::vkCmdDraw(commands, 3, 1, 0, 0);
  EndPass();
  bound_pipeline_ = nullptr;
  return true;
}

bool Backend::DrawMovie(Image* const planes[3], Image* target_image, const Rect& area) {
  ImageObject& target = Object(target_image);
  const VkSampleCountFlagBits samples = VkSampleCountFlagBits(target.desc.samples);
  auto [it, added] = movie_pipelines_.try_emplace({target.format, samples}, VK_NULL_HANDLE);
  if (added) {
    it->second = MakeFullscreenPipeline(movie_pixel_, movie_layout_, target.format, samples);
    if (!it->second) {
      ITC_LOG_ERROR("native renderer: couldn't create a movie pipeline for format {}",
                    int(target.format));
    }
  }
  const VkPipeline pipeline = it->second;
  if (!pipeline) {
    return false;
  }
  const VkDescriptorSet set = PassSet(movie_set_layout_);
  if (!set) {
    return false;
  }
  VkDescriptorImageInfo images[3];
  VkWriteDescriptorSet writes[3];
  for (uint32_t i = 0; i < 3; i++) {
    images[i] = {movie_sampler_, Object(planes[i]).view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = set;
    writes[i].dstBinding = i;
    writes[i].descriptorCount = 1;
    writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writes[i].pImageInfo = &images[i];
  }
  vk::vkUpdateDescriptorSets(device_, 3, writes, 0, nullptr);
  PassTarget pass_target;
  pass_target.image = target_image;
  BeginPass(pass_target, {}, area);
  const VkCommandBuffer commands = this->commands();
  SetViewport({float(area.x), float(area.y), float(area.width), float(area.height), 0.0f, 1.0f});
  SetScissor(area);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, movie_layout_, 0, 1,
                              &set, 0, nullptr);
  const float constants[4] = {float(area.x), float(area.y), float(area.width),
                              float(area.height)};
  vk::vkCmdPushConstants(commands, movie_layout_, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(constants), constants);
  vk::vkCmdDraw(commands, 3, 1, 0, 0);
  EndPass();
  bound_pipeline_ = nullptr;
  return true;
}

// The gamma pass (gamma.comp).

bool Backend::ApplyGamma(Image* image, Extent2D size, const uint16_t table[256]) {
  if (gamma_failed_) {
    return false;
  }
  const VkCommandBuffer commands = this->commands();
  if (!gamma_pipeline_) {
    gamma_failed_ = true;  // until made
    gamma_table_ = CreateBuffer({256 * sizeof(uint32_t), kBufferStorage | kBufferCopyDest, false});
    if (!gamma_table_) {
      return false;
    }
    native::gpu_memory::Allocated(native::gpu_memory::Kind::kOther, MemorySize(gamma_table_));
    VkDescriptorSetLayoutBinding bindings[2] = {
        {0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr},
        {1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr}};
    VkDescriptorSetLayoutCreateInfo set_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    set_info.bindingCount = 2;
    set_info.pBindings = bindings;
    VkPushConstantRange push{VK_SHADER_STAGE_COMPUTE_BIT, 0, 8};
    VkPipelineLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    layout_info.setLayoutCount = 1;
    layout_info.pSetLayouts = &gamma_set_layout_;
    layout_info.pushConstantRangeCount = 1;
    layout_info.pPushConstantRanges = &push;
    VkDescriptorPoolSize sizes[2] = {{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1},
                                     {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1}};
    VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    pool_info.maxSets = 1;
    pool_info.poolSizeCount = 2;
    pool_info.pPoolSizes = sizes;
    if (vk::vkCreateDescriptorSetLayout(device_, &set_info, nullptr, &gamma_set_layout_) !=
            VK_SUCCESS ||
        vk::vkCreatePipelineLayout(device_, &layout_info, nullptr, &gamma_layout_) != VK_SUCCESS ||
        vk::vkCreateDescriptorPool(device_, &pool_info, nullptr, &gamma_pool_) != VK_SUCCESS) {
      return false;
    }
    VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    set_alloc.descriptorPool = gamma_pool_;
    set_alloc.descriptorSetCount = 1;
    set_alloc.pSetLayouts = &gamma_set_layout_;
    if (vk::vkAllocateDescriptorSets(device_, &set_alloc, &gamma_set_) != VK_SUCCESS) {
      return false;
    }
    VkShaderModuleCreateInfo module_info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    module_info.codeSize = sizeof(k_gamma_comp);
    module_info.pCode = k_gamma_comp;
    VkShaderModule module;
    if (vk::vkCreateShaderModule(device_, &module_info, nullptr, &module) != VK_SUCCESS) {
      return false;
    }
    VkComputePipelineCreateInfo info{VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};
    info.stage = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    info.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    info.stage.module = module;
    info.stage.pName = "main";
    info.layout = gamma_layout_;
    const VkResult result =
        vk::vkCreateComputePipelines(device_, pipeline_cache_, 1, &info, nullptr, &gamma_pipeline_);
    vk::vkDestroyShaderModule(device_, module, nullptr);
    if (result != VK_SUCCESS) {
      gamma_pipeline_ = VK_NULL_HANDLE;
      return false;
    }
    gamma_failed_ = false;
  }
  if (image != gamma_image_) {
    // Written while no frame in flight reads it: once (the renderer's
    // shown image lives as long as the backend).
    gamma_image_ = image;
    VkDescriptorImageInfo image_info{VK_NULL_HANDLE, Object(image).view, VK_IMAGE_LAYOUT_GENERAL};
    VkDescriptorBufferInfo table_info{Handle(gamma_table_), 0, VK_WHOLE_SIZE};
    VkWriteDescriptorSet writes[2] = {{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET},
                                      {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET}};
    writes[0].dstSet = writes[1].dstSet = gamma_set_;
    writes[0].dstBinding = 0;
    writes[0].descriptorCount = 1;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    writes[0].pImageInfo = &image_info;
    writes[1].dstBinding = 1;
    writes[1].descriptorCount = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[1].pBufferInfo = &table_info;
    vk::vkUpdateDescriptorSets(device_, 2, writes, 0, nullptr);
  }
  if (!gamma_uploaded_) {  // the first frame: the table into its buffer
    uint32_t entries[256];
    std::copy(table, table + 256, entries);
    vk::vkCmdUpdateBuffer(commands, Handle(gamma_table_), 0, sizeof(entries), entries);
    VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 1, &barrier, 0, nullptr, 0,
                             nullptr);
    gamma_uploaded_ = true;
  }
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_COMPUTE, gamma_pipeline_);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_COMPUTE, gamma_layout_, 0, 1,
                              &gamma_set_, 0, nullptr);
  const int32_t extent[2] = {int32_t(size.width), int32_t(size.height)};
  vk::vkCmdPushConstants(commands, gamma_layout_, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(extent),
                         extent);
  vk::vkCmdDispatch(commands, (size.width + 7) / 8, (size.height + 7) / 8, 1);
  return true;
}

// The draw ID pass (id.frag).

Pipeline* Backend::GetIdPipeline(const PipelineDesc& desc) {
  PipelineDesc key = desc;
  key.pixel = nullptr;
  key.blend_enable = key.src_color = key.dst_color = key.color_op = 0;
  key.src_alpha = key.dst_alpha = key.alpha_op = key.write_mask = 0;
  key.color_format = key.samples = 0;
  key.depth = 1;
  auto [it, added] = id_pipelines_.try_emplace(key);
  if (!added) {
    PipelineObject* o = it->second.get();
    return o && o->pipeline ? reinterpret_cast<Pipeline*>(o) : nullptr;
  }
  if (!id_pixel_) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = sizeof(k_id_frag);
    info.pCode = k_id_frag;
    if (vk::vkCreateShaderModule(device_, &info, nullptr, &id_pixel_) != VK_SUCCESS) {
      id_pixel_ = VK_NULL_HANDLE;
      return nullptr;
    }
  }
  auto object = std::make_unique<PipelineObject>();
  PipelineObject& o = *object;
  o.sets[0] = empty_layout_;
  o.sets[1] = constants_layout_;
  o.sets[2] = TexturesLayout(key.vertex, ShaderStage::kVertex);
  o.sets[3] = TexturesLayout(nullptr, ShaderStage::kPixel);
  if (!key.vertex || !o.sets[2] || !o.sets[3]) {
    return nullptr;  // (kept as made: not tried again)
  }
  // The draw's layout with the number as a push constant.
  VkPipelineLayout& layout = id_layouts_[o.sets[2]];
  if (!layout) {
    VkPushConstantRange push{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 4};
    VkPipelineLayoutCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    info.setLayoutCount = 4;
    info.pSetLayouts = o.sets;
    info.pushConstantRangeCount = 1;
    info.pPushConstantRanges = &push;
    vk::vkCreatePipelineLayout(device_, &info, nullptr, &layout);
  }
  o.layout = layout;
  if (!layout) {
    return nullptr;
  }
  PipelineDesc build = key;
  build.write_mask = 0xF;
  build.color_format = uint32_t(Format::kR32Uint);
  build.samples = 1;
  o.pipeline = MakeGraphicsPipeline(build, id_pixel_, layout, false);
  it->second = std::move(object);
  return it->second->pipeline ? reinterpret_cast<Pipeline*>(it->second.get()) : nullptr;
}

void Backend::SetDrawId(uint32_t id) {
  vk::vkCmdPushConstants(commands(), bound_pipeline_->layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 4,
                         &id);
}

}  // namespace gpu::vulkan
