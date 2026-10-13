#include "main_view.h"

#include "guest.h"
#include "shaders.h"
#include "state.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <rex/graphics/pipeline/shader/spirv_translator.h>
#include <rex/graphics/register_file.h>
#include <rex/graphics/registers.h>
#include <rex/graphics/util/draw.h>
#include <rex/graphics/xenos.h>
#include <rex/logging.h>

namespace native {

namespace graphics = rex::graphics;
namespace reg = rex::graphics::reg;
namespace xenos = rex::graphics::xenos;
namespace draw_util = rex::graphics::draw_util;
using Translator = graphics::SpirvShaderTranslator;
using graphics::XE_GPU_REG_RB_ALPHA_REF;
using graphics::XE_GPU_REG_RB_COLOR_MASK;
using graphics::XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031;
using graphics::XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0;
using graphics::XE_GPU_REG_SQ_PROGRAM_CNTL;
using graphics::XE_GPU_REG_VGT_MAX_VTX_INDX;
using graphics::XE_GPU_REG_VGT_MIN_VTX_INDX;

namespace {

// Our target: the main view's size (its viewport is 0,0 1024x576).
constexpr uint32_t kTargetWidth = 1024, kTargetHeight = 576;
constexpr VkFormat kColorFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
constexpr VkFormat kDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

// Per frame in flight: the draws' constants.
constexpr VkDeviceSize kConstantRingSize = 32u << 20;
// Fixed descriptor ranges of the constant buffers (set 1), so one set per
// frame serves every draw through dynamic offsets.
constexpr uint32_t kFloatConstantsRange = 256 * 16;
constexpr uint32_t kBoolLoopConstantsRange = 40 * 4;
constexpr uint32_t kFetchConstantsRange = 32 * 6 * 4;

// Command 18's item: [primitive type, base vertex << 16 | start index,
// primitive count]; the index count is count * a + b (the table at
// 0x82000950, see docs/gpu-functions.md).
struct PrimitiveInfo {
  VkPrimitiveTopology topology;
  uint32_t a, b;
  bool strip;
  bool polygonal;
};

bool GetPrimitiveInfo(uint32_t type, PrimitiveInfo& out) {
  switch (type) {
    case 1:
      out = {VK_PRIMITIVE_TOPOLOGY_POINT_LIST, 1, 0, false, false};
      return true;
    case 2:
      out = {VK_PRIMITIVE_TOPOLOGY_LINE_LIST, 2, 0, false, false};
      return true;
    case 3:
      out = {VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, 1, 1, true, false};
      return true;
    case 4:
      out = {VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 3, 0, false, true};
      return true;
    case 5:
      out = {VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN, 1, 2, true, true};
      return true;
    case 6:
      out = {VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, 1, 2, true, true};
      return true;
    default:
      return false;  // rectangle (8) and quad (13) lists: not yet
  }
}

// Xenos blend factors and ops (as the plugin maps them).
VkBlendFactor BlendFactor(uint32_t factor) {
  static const VkBlendFactor kMap[17] = {
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_FACTOR_ONE,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_FACTOR_SRC_COLOR,
      VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
      VK_BLEND_FACTOR_SRC_ALPHA,
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
      VK_BLEND_FACTOR_DST_COLOR,
      VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
      VK_BLEND_FACTOR_DST_ALPHA,
      VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
      VK_BLEND_FACTOR_CONSTANT_COLOR,
      VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
      VK_BLEND_FACTOR_CONSTANT_ALPHA,
      VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
      VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
  };
  return factor < 17 ? kMap[factor] : VK_BLEND_FACTOR_ZERO;
}

VkBlendOp BlendOp(uint32_t op) {
  static const VkBlendOp kMap[5] = {VK_BLEND_OP_ADD, VK_BLEND_OP_SUBTRACT, VK_BLEND_OP_MIN,
                                    VK_BLEND_OP_MAX, VK_BLEND_OP_REVERSE_SUBTRACT};
  return op < 5 ? kMap[op] : VK_BLEND_OP_ADD;
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

// Everything that selects a pipeline. Zeroed before filling, so it can be
// compared as bytes.
struct PipelineKey {
  VkShaderModule vertex;
  VkShaderModule pixel;
  VkPipelineLayout layout;
  uint32_t topology, primitive_restart;
  uint32_t cull_mode, front_clockwise;
  uint32_t z_enable, z_write, z_func;
  uint32_t blend_enable, src_color, dst_color, color_op, src_alpha, dst_alpha, alpha_op;
  uint32_t write_mask;
};

struct Image {
  VkImage image = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  VkImageView view = VK_NULL_HANDLE;
};

// Texture descriptor sets (sets 2 and 3) a frame can allocate, and their
// descriptors.
constexpr uint32_t kTextureSetsPerFrame = 4096;
constexpr uint32_t kTextureDescriptorsPerFrame = 16384;

}  // namespace

struct MainView::State {
  DeviceContext context{};
  VkPhysicalDeviceMemoryProperties memory_properties{};
  uint32_t frames_in_flight = 0;

  // A mirror of guest physical memory at the same offsets: the shaders'
  // storage buffer (set 0) and the index buffer. The driver won't import
  // guest memory itself (a shared memory-file mapping), so each draw's
  // index and vertex ranges are copied in, once a frame.
  VkBuffer guest_memory = VK_NULL_HANDLE;
  VkDeviceMemory guest_memory_allocation = VK_NULL_HANDLE;
  uint8_t* guest_memory_mapped = nullptr;
  std::unordered_set<uint64_t> mirrored;  // address << 32 | size, this frame

  Image color, depth;
  bool rendering = false;

  struct Ring {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapped = nullptr;
    VkDescriptorSet constants_set = VK_NULL_HANDLE;
    // This frame's texture descriptor sets, reset with the frame.
    VkDescriptorPool texture_pool = VK_NULL_HANDLE;
  };
  std::vector<Ring> rings;
  uint32_t frame = 0;
  VkDeviceSize ring_offset = 0;

  VkDescriptorPool pool = VK_NULL_HANDLE;
  VkDescriptorSetLayout shared_memory_layout = VK_NULL_HANDLE;
  VkDescriptorSetLayout constants_layout = VK_NULL_HANDLE;
  VkDescriptorSet shared_memory_set = VK_NULL_HANDLE;

  // Textures, and white 1x1 placeholders of each declared type for what
  // can't be had yet.
  TextureCache textures;
  Image dummy_2d_array, dummy_3d, dummy_cube;
  VkSampler sampler = VK_NULL_HANDLE;
  // Texture set layouts by the shader's bindings; this frame's sets by
  // their contents.
  std::unordered_map<std::string, VkDescriptorSetLayout> texture_layouts;
  std::unordered_map<std::string, VkDescriptorSet> texture_sets;
  std::unordered_map<std::string, VkPipelineLayout> pipeline_layouts;
  std::unordered_map<std::string, VkPipeline> pipelines;

  std::unique_ptr<ShaderCache> shaders;
  rex::graphics::RegisterFile regs;
  Stats stats;

  bool CreateImage(Image& out, VkImageType type, VkImageViewType view_type, VkFormat format,
                   VkExtent3D extent, uint32_t layers, VkImageUsageFlags usage,
                   VkImageAspectFlags aspect, VkImageCreateFlags flags = 0);
  bool CreateGuestMemoryMirror();
  void Mirror(uint8_t* base, uint32_t address, uint32_t size);
  bool CreateRings();
  bool CreateDescriptors();
  bool CreateDummyTextures();
  VkDescriptorSetLayout GetTextureLayout(const TranslatedShader* shader,
                                         VkShaderStageFlags stage);
  VkDescriptorSet GetTextureSet(uint8_t* base, const TranslatedShader* shader,
                                VkDescriptorSetLayout layout);
  VkPipelineLayout GetPipelineLayout(VkDescriptorSetLayout vertex, VkDescriptorSetLayout pixel);
  VkShaderModule GetModule(TranslatedShader* shader);
  VkPipeline GetPipeline(const PipelineKey& key);
  bool Reserve(VkDeviceSize size, uint32_t& offset, uint8_t*& data);
  void BeginRendering(VkCommandBuffer commands, float clear_depth);
};

bool MainView::State::CreateImage(Image& out, VkImageType type, VkImageViewType view_type,
                                  VkFormat format, VkExtent3D extent, uint32_t layers,
                                  VkImageUsageFlags usage, VkImageAspectFlags aspect,
                                  VkImageCreateFlags flags) {
  VkDevice device = context.device;
  VkImageCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  info.flags = flags;
  info.imageType = type;
  info.format = format;
  info.extent = extent;
  info.mipLevels = 1;
  info.arrayLayers = layers;
  info.samples = VK_SAMPLE_COUNT_1_BIT;
  info.tiling = VK_IMAGE_TILING_OPTIMAL;
  info.usage = usage;
  info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (vk::vkCreateImage(device, &info, nullptr, &out.image) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetImageMemoryRequirements(device, out.image, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = FindMemoryType(memory_properties, requirements.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &out.memory) != VK_SUCCESS ||
      vk::vkBindImageMemory(device, out.image, out.memory, 0) != VK_SUCCESS) {
    return false;
  }
  VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  view.image = out.image;
  view.viewType = view_type;
  view.format = format;
  view.subresourceRange = {aspect, 0, 1, 0, layers};
  return vk::vkCreateImageView(device, &view, nullptr, &out.view) == VK_SUCCESS;
}

bool MainView::State::CreateGuestMemoryMirror() {
  VkDevice device = context.device;
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = kPhysicalMemorySize;
  info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &info, nullptr, &guest_memory) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, guest_memory, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex =
      FindMemoryType(memory_properties, requirements.memoryTypeBits,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX) {
    REXLOG_ERROR("native renderer: no host-visible memory for the guest memory mirror");
    return false;
  }
  if (VkResult result = vk::vkAllocateMemory(device, &alloc, nullptr, &guest_memory_allocation);
      result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: allocating the guest memory mirror failed: {}", int(result));
    return false;
  }
  if (vk::vkBindBufferMemory(device, guest_memory, guest_memory_allocation, 0) != VK_SUCCESS ||
      vk::vkMapMemory(device, guest_memory_allocation, 0, VK_WHOLE_SIZE, 0,
                      reinterpret_cast<void**>(&guest_memory_mapped)) != VK_SUCCESS) {
    return false;
  }
  REXLOG_INFO("native renderer: guest memory mirror of {} MB", kPhysicalMemorySize >> 20);
  return true;
}

void MainView::State::Mirror(uint8_t* base, uint32_t address, uint32_t size) {
  if (!size || address >= kPhysicalMemorySize) {
    return;
  }
  size = std::min(size, kPhysicalMemorySize - address);
  uint64_t key = (uint64_t(address) << 32) | size;
  if (!mirrored.insert(key).second) {
    return;  // already copied this frame
  }
  std::memcpy(guest_memory_mapped + address, PhysicalHostAddress(base, address), size);
  stats.mirrored_bytes += size;
}

bool MainView::State::CreateRings() {
  VkDevice device = context.device;
  rings.resize(frames_in_flight);
  for (Ring& ring : rings) {
    VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    info.size = kConstantRingSize;
    info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vk::vkCreateBuffer(device, &info, nullptr, &ring.buffer) != VK_SUCCESS) {
      return false;
    }
    VkMemoryRequirements requirements;
    vk::vkGetBufferMemoryRequirements(device, ring.buffer, &requirements);
    VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc.allocationSize = requirements.size;
    alloc.memoryTypeIndex =
        FindMemoryType(memory_properties, requirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (alloc.memoryTypeIndex == UINT32_MAX ||
        vk::vkAllocateMemory(device, &alloc, nullptr, &ring.memory) != VK_SUCCESS ||
        vk::vkBindBufferMemory(device, ring.buffer, ring.memory, 0) != VK_SUCCESS ||
        vk::vkMapMemory(device, ring.memory, 0, VK_WHOLE_SIZE, 0,
                        reinterpret_cast<void**>(&ring.mapped)) != VK_SUCCESS) {
      return false;
    }
    VkDescriptorPoolSize sizes[] = {
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, kTextureDescriptorsPerFrame},
        {VK_DESCRIPTOR_TYPE_SAMPLER, kTextureDescriptorsPerFrame},
    };
    VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    pool_info.maxSets = kTextureSetsPerFrame;
    pool_info.poolSizeCount = 2;
    pool_info.pPoolSizes = sizes;
    if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &ring.texture_pool) !=
        VK_SUCCESS) {
      return false;
    }
  }
  return true;
}

bool MainView::State::CreateDescriptors() {
  VkDevice device = context.device;
  VkDescriptorPoolSize sizes[] = {
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 4},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 5 * frames_in_flight},
  };
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 1 + frames_in_flight;
  pool_info.poolSizeCount = 2;
  pool_info.pPoolSizes = sizes;
  if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  constexpr VkShaderStageFlags kBoth = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

  // Set 0: guest memory.
  VkDescriptorSetLayoutBinding shared{0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, kBoth, nullptr};
  VkDescriptorSetLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  layout_info.bindingCount = 1;
  layout_info.pBindings = &shared;
  if (vk::vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &shared_memory_layout) !=
      VK_SUCCESS) {
    return false;
  }
  // Set 1: system, vertex float, pixel float, bool/loop and fetch constants.
  VkDescriptorSetLayoutBinding constants[5];
  for (uint32_t i = 0; i < 5; i++) {
    constants[i] = {i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, kBoth, nullptr};
  }
  layout_info.bindingCount = 5;
  layout_info.pBindings = constants;
  if (vk::vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &constants_layout) !=
      VK_SUCCESS) {
    return false;
  }

  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  alloc.descriptorPool = pool;
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &shared_memory_layout;
  if (vk::vkAllocateDescriptorSets(device, &alloc, &shared_memory_set) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorBufferInfo shared_buffer{guest_memory, 0, VK_WHOLE_SIZE};
  VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
  write.dstSet = shared_memory_set;
  write.dstBinding = 0;
  write.descriptorCount = 1;
  write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  write.pBufferInfo = &shared_buffer;
  vk::vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);

  const uint32_t ranges[5] = {uint32_t(sizeof(Translator::SystemConstants)),
                              kFloatConstantsRange, kFloatConstantsRange,
                              kBoolLoopConstantsRange, kFetchConstantsRange};
  for (Ring& ring : rings) {
    alloc.pSetLayouts = &constants_layout;
    if (vk::vkAllocateDescriptorSets(device, &alloc, &ring.constants_set) != VK_SUCCESS) {
      return false;
    }
    VkDescriptorBufferInfo buffers[5];
    VkWriteDescriptorSet writes[5];
    for (uint32_t i = 0; i < 5; i++) {
      buffers[i] = {ring.buffer, 0, ranges[i]};
      writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
      writes[i].dstSet = ring.constants_set;
      writes[i].dstBinding = i;
      writes[i].descriptorCount = 1;
      writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
      writes[i].pBufferInfo = &buffers[i];
    }
    vk::vkUpdateDescriptorSets(device, 5, writes, 0, nullptr);
  }

  VkSamplerCreateInfo sampler_info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  sampler_info.magFilter = VK_FILTER_LINEAR;
  sampler_info.minFilter = VK_FILTER_LINEAR;
  sampler_info.addressModeU = sampler_info.addressModeV = sampler_info.addressModeW =
      VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_info.maxLod = VK_LOD_CLAMP_NONE;
  return vk::vkCreateSampler(device, &sampler_info, nullptr, &sampler) == VK_SUCCESS;
}

bool MainView::State::CreateDummyTextures() {
  constexpr VkImageUsageFlags kUsage =
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  if (!CreateImage(dummy_2d_array, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                   VK_FORMAT_R8G8B8A8_UNORM, {1, 1, 1}, 1, kUsage, VK_IMAGE_ASPECT_COLOR_BIT) ||
      !CreateImage(dummy_3d, VK_IMAGE_TYPE_3D, VK_IMAGE_VIEW_TYPE_3D, VK_FORMAT_R8G8B8A8_UNORM,
                   {1, 1, 1}, 1, kUsage, VK_IMAGE_ASPECT_COLOR_BIT) ||
      !CreateImage(dummy_cube, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_CUBE,
                   VK_FORMAT_R8G8B8A8_UNORM, {1, 1, 1}, 6, kUsage, VK_IMAGE_ASPECT_COLOR_BIT,
                   VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)) {
    return false;
  }
  // Clear them to white once, and leave them readable.
  VkDevice device = context.device;
  VkCommandPool command_pool;
  VkCommandPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
  pool_info.queueFamilyIndex = context.queue_family;
  if (vk::vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS) {
    return false;
  }
  VkCommandBuffer commands;
  VkCommandBufferAllocateInfo alloc{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
  alloc.commandPool = command_pool;
  alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc.commandBufferCount = 1;
  vk::vkAllocateCommandBuffers(device, &alloc, &commands);
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vk::vkBeginCommandBuffer(commands, &begin);
  VkClearColorValue white{{1.0f, 1.0f, 1.0f, 1.0f}};
  for (auto [image, layers] : {std::pair{dummy_2d_array.image, 1u}, std::pair{dummy_3d.image, 1u},
                               std::pair{dummy_cube.image, 6u}}) {
    VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, layers};
    VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange = range;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);
    vk::vkCmdClearColorImage(commands, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &white, 1,
                             &range);
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                                 VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                             0, 0, nullptr, 0, nullptr, 1, &barrier);
  }
  vk::vkEndCommandBuffer(commands);
  VkFence fence;
  VkFenceCreateInfo fence_info{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
  vk::vkCreateFence(device, &fence_info, nullptr, &fence);
  VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = &commands;
  vk::vkQueueSubmit(context.queue, 1, &submit, fence);
  vk::vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
  vk::vkDestroyFence(device, fence, nullptr);
  vk::vkDestroyCommandPool(device, command_pool, nullptr);
  return true;
}

VkDescriptorSetLayout MainView::State::GetTextureLayout(const TranslatedShader* shader,
                                                       VkShaderStageFlags stage) {
  // Keyed by stage, the image types and the sampler count.
  std::string key(1, char(stage));
  if (shader) {
    for (const ShaderTexture& texture : shader->textures) {
      key += char('0' + texture.view_type);
    }
    key += ':' + std::to_string(shader->samplers.size());
  }
  VkDescriptorSetLayout& layout = texture_layouts[key];
  if (layout) {
    return layout;
  }
  std::vector<VkDescriptorSetLayoutBinding> bindings;
  if (shader) {
    for (uint32_t i = 0; i < shader->textures.size(); i++) {
      bindings.push_back({i, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, stage, nullptr});
    }
    for (uint32_t i = 0; i < shader->samplers.size(); i++) {
      bindings.push_back({uint32_t(shader->textures.size() + i), VK_DESCRIPTOR_TYPE_SAMPLER, 1,
                          stage, nullptr});
    }
  }
  VkDescriptorSetLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  layout_info.bindingCount = uint32_t(bindings.size());
  layout_info.pBindings = bindings.data();
  if (vk::vkCreateDescriptorSetLayout(context.device, &layout_info, nullptr, &layout) !=
      VK_SUCCESS) {
    layout = VK_NULL_HANDLE;
  }
  return layout;
}

VkDescriptorSet MainView::State::GetTextureSet(uint8_t* base, const TranslatedShader* shader,
                                               VkDescriptorSetLayout layout) {
  // The views and samplers for the shader's bindings, from the current
  // fetch constants; placeholders for what can't be had.
  size_t texture_count = shader ? shader->textures.size() : 0;
  size_t sampler_count = shader ? shader->samplers.size() : 0;
  std::vector<VkDescriptorImageInfo> infos(texture_count + sampler_count);
  for (size_t i = 0; i < texture_count; i++) {
    const ShaderTexture& texture = shader->textures[i];
    const uint32_t* fetch =
        &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 6 * texture.fetch_constant];
    // The signed binding gets the same (unsigned) view: it's read only for
    // components whose sign is signed, which this game's textures don't
    // have (logged if they do).
    VkImageView view = textures.GetView(base, fetch, texture.view_type);
    if (!view) {
      view = texture.view_type == VK_IMAGE_VIEW_TYPE_3D     ? dummy_3d.view
             : texture.view_type == VK_IMAGE_VIEW_TYPE_CUBE ? dummy_cube.view
                                                            : dummy_2d_array.view;
    }
    infos[i] = {VK_NULL_HANDLE, view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
  }
  for (size_t i = 0; i < sampler_count; i++) {
    const ShaderSampler& binding = shader->samplers[i];
    const uint32_t* fetch =
        &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 6 * binding.fetch_constant];
    VkSampler texture_sampler = textures.GetSampler(
        fetch, {binding.fetch_constant, binding.mag_filter, binding.min_filter, binding.mip_filter,
                binding.aniso_filter});
    infos[texture_count + i] = {texture_sampler ? texture_sampler : sampler, VK_NULL_HANDLE,
                                VK_IMAGE_LAYOUT_UNDEFINED};
  }

  // One set per distinct contents a frame.
  std::string key(reinterpret_cast<const char*>(&layout), sizeof(layout));
  key.append(reinterpret_cast<const char*>(infos.data()), infos.size() * sizeof(infos[0]));
  auto [it, added] = texture_sets.try_emplace(key, VK_NULL_HANDLE);
  if (!added) {
    return it->second;
  }
  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  alloc.descriptorPool = rings[frame].texture_pool;
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &layout;
  if (vk::vkAllocateDescriptorSets(context.device, &alloc, &it->second) != VK_SUCCESS) {
    it->second = VK_NULL_HANDLE;
    return VK_NULL_HANDLE;
  }
  std::vector<VkWriteDescriptorSet> writes(infos.size());
  for (size_t i = 0; i < infos.size(); i++) {
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = it->second;
    writes[i].dstBinding = uint32_t(i);
    writes[i].descriptorCount = 1;
    writes[i].descriptorType =
        i < texture_count ? VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE : VK_DESCRIPTOR_TYPE_SAMPLER;
    writes[i].pImageInfo = &infos[i];
  }
  if (!writes.empty()) {
    vk::vkUpdateDescriptorSets(context.device, uint32_t(writes.size()), writes.data(), 0, nullptr);
  }
  return it->second;
}

VkPipelineLayout MainView::State::GetPipelineLayout(VkDescriptorSetLayout vertex,
                                                    VkDescriptorSetLayout pixel) {
  std::string key(reinterpret_cast<const char*>(&vertex), sizeof(vertex));
  key.append(reinterpret_cast<const char*>(&pixel), sizeof(pixel));
  VkPipelineLayout& layout = pipeline_layouts[key];
  if (!layout) {
    VkDescriptorSetLayout sets[4] = {shared_memory_layout, constants_layout, vertex, pixel};
    VkPipelineLayoutCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    info.setLayoutCount = 4;
    info.pSetLayouts = sets;
    vk::vkCreatePipelineLayout(context.device, &info, nullptr, &layout);
  }
  return layout;
}

VkShaderModule MainView::State::GetModule(TranslatedShader* shader) {
  if (!shader) {
    return VK_NULL_HANDLE;
  }
  if (!shader->module) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = shader->spirv.size() * 4;
    info.pCode = shader->spirv.data();
    vk::vkCreateShaderModule(context.device, &info, nullptr, &shader->module);
  }
  return shader->module;
}

VkPipeline MainView::State::GetPipeline(const PipelineKey& key) {
  std::string key_bytes(reinterpret_cast<const char*>(&key), sizeof(key));
  auto [it, added] = pipelines.try_emplace(key_bytes, VK_NULL_HANDLE);
  if (!added) {
    return it->second;
  }
  VkPipelineShaderStageCreateInfo stages[2] = {};
  uint32_t stage_count = 0;
  stages[stage_count] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
  stages[stage_count].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[stage_count].module = key.vertex;
  stages[stage_count++].pName = "main";
  if (key.pixel) {
    stages[stage_count] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    stages[stage_count].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[stage_count].module = key.pixel;
    stages[stage_count++].pName = "main";
  }
  VkPipelineVertexInputStateCreateInfo vertex_input{
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
  VkPipelineInputAssemblyStateCreateInfo input_assembly{
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
  input_assembly.topology = VkPrimitiveTopology(key.topology);
  input_assembly.primitiveRestartEnable = key.primitive_restart;
  VkPipelineViewportStateCreateInfo viewport{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
  viewport.viewportCount = 1;
  viewport.scissorCount = 1;
  VkPipelineRasterizationStateCreateInfo raster{
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
  raster.polygonMode = VK_POLYGON_MODE_FILL;
  raster.cullMode = key.cull_mode;
  raster.frontFace = key.front_clockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
  raster.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineDepthStencilStateCreateInfo depth{
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
  depth.depthTestEnable = key.z_enable;
  depth.depthWriteEnable = key.z_write;
  depth.depthCompareOp = VkCompareOp(key.z_func);  // Xenos' order is Vulkan's
  VkPipelineColorBlendAttachmentState attachment{};
  attachment.blendEnable = key.blend_enable;
  attachment.srcColorBlendFactor = VkBlendFactor(key.src_color);
  attachment.dstColorBlendFactor = VkBlendFactor(key.dst_color);
  attachment.colorBlendOp = VkBlendOp(key.color_op);
  attachment.srcAlphaBlendFactor = VkBlendFactor(key.src_alpha);
  attachment.dstAlphaBlendFactor = VkBlendFactor(key.dst_alpha);
  attachment.alphaBlendOp = VkBlendOp(key.alpha_op);
  attachment.colorWriteMask = key.write_mask;
  VkPipelineColorBlendStateCreateInfo blend{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
  blend.attachmentCount = 1;
  blend.pAttachments = &attachment;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamic{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic.dynamicStateCount = 2;
  dynamic.pDynamicStates = dynamic_states;
  VkFormat color_format = kColorFormat;
  VkPipelineRenderingCreateInfo rendering{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
  rendering.colorAttachmentCount = 1;
  rendering.pColorAttachmentFormats = &color_format;
  rendering.depthAttachmentFormat = kDepthFormat;
  rendering.stencilAttachmentFormat = kDepthFormat;

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
  info.layout = key.layout;
  if (VkResult result = vk::vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &info,
                                                      nullptr, &it->second);
      result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: vkCreateGraphicsPipelines: {}", int(result));
    it->second = VK_NULL_HANDLE;
  } else {
    stats.pipelines++;
  }
  return it->second;
}

bool MainView::State::Reserve(VkDeviceSize size, uint32_t& offset, uint8_t*& data) {
  VkDeviceSize alignment = std::max<VkDeviceSize>(context.min_uniform_buffer_offset_alignment, 16);
  VkDeviceSize start = (ring_offset + alignment - 1) / alignment * alignment;
  if (start + size > kConstantRingSize) {
    return false;
  }
  ring_offset = start + size;
  offset = uint32_t(start);
  data = rings[frame].mapped + start;
  return true;
}

void MainView::State::BeginRendering(VkCommandBuffer commands, float clear_depth) {
  VkImageMemoryBarrier barriers[2] = {};
  for (VkImageMemoryBarrier& barrier : barriers) {
    barrier = {VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  }
  barriers[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  barriers[0].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  barriers[0].image = color.image;
  barriers[0].subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
  barriers[1].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  barriers[1].newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  barriers[1].image = depth.image;
  barriers[1].subresourceRange = {VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 0, 1,
                                  0, 1};
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                           VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                               VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                           0, 0, nullptr, 0, nullptr, 2, barriers);
  VkRenderingAttachmentInfo color_attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  color_attachment.imageView = color.view;
  color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  VkRenderingAttachmentInfo depth_attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  depth_attachment.imageView = depth.view;
  depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depth_attachment.clearValue.depthStencil = {clear_depth, 0};
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  info.renderArea = {{0, 0}, {kTargetWidth, kTargetHeight}};
  info.layerCount = 1;
  info.colorAttachmentCount = 1;
  info.pColorAttachments = &color_attachment;
  info.pDepthAttachment = &depth_attachment;
  info.pStencilAttachment = &depth_attachment;
  vk::vkCmdBeginRendering(commands, &info);
  rendering = true;
}

MainView::MainView() : state_(std::make_unique<State>()) {}
MainView::~MainView() = default;

bool MainView::Init(const DeviceContext& context, uint8_t* base, uint32_t frames_in_flight) {
  State& s = *state_;
  s.context = context;
  s.frames_in_flight = frames_in_flight;
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &s.memory_properties);
  if (!s.CreateGuestMemoryMirror()) {
    return false;
  }
  if (!s.CreateImage(s.color, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, kColorFormat,
                     {kTargetWidth, kTargetHeight, 1}, 1,
                     VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                         VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                     VK_IMAGE_ASPECT_COLOR_BIT) ||
      !s.CreateImage(s.depth, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, kDepthFormat,
                     {kTargetWidth, kTargetHeight, 1}, 1,
                     VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                     VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)) {
    REXLOG_ERROR("native renderer: couldn't create the main view's target");
    return false;
  }
  if (!s.CreateRings() || !s.CreateDescriptors() || !s.CreateDummyTextures() ||
      !s.textures.Init(context, frames_in_flight)) {
    REXLOG_ERROR("native renderer: couldn't create the main view's resources");
    return false;
  }
  ShaderFeatures features;
  features.max_storage_buffer_range = context.max_storage_buffer_range;
  features.clip_distance = context.clip_distance;
  features.cull_distance = context.cull_distance;
  features.demote_to_helper_invocation = context.demote_to_helper_invocation;
  s.shaders = std::make_unique<ShaderCache>(features);
  return true;
}

void MainView::BeginFrame(uint32_t frame_index) {
  state_->frame = frame_index;
  state_->ring_offset = 0;
  state_->rendering = false;
  state_->mirrored.clear();
  state_->texture_sets.clear();
  vk::vkResetDescriptorPool(state_->context.device, state_->rings[frame_index].texture_pool, 0);
  state_->textures.BeginFrame(frame_index);
}

VkCommandBuffer MainView::EndUploads() {
  return state_->textures.EndUploads();
}

void MainView::AfterDraw(VkCommandBuffer commands, uint8_t* base, uint32_t id, uint32_t payload,
                         uint32_t arg1) {
  State& s = *state_;
  auto& regs = s.regs;
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  ReadRegisters(base, device, regs);

  // Only the main view for now.
  auto surface_info = regs.Get<reg::RB_SURFACE_INFO>();
  auto color_info = regs.Get<reg::RB_COLOR_INFO>(reg::RB_COLOR_INFO::rt_register_indices[0]);
  // Its format switches per draw between 2_10_10_10_FLOAT (3) and the
  // high-precision blending variant (12), by render states 308-320.
  bool main_view_format =
      color_info.color_format == xenos::ColorRenderTargetFormat::k_2_10_10_10_FLOAT ||
      color_info.color_format == xenos::ColorRenderTargetFormat::k_2_10_10_10_FLOAT_AS_16_16_16_16;
  if (surface_info.surface_pitch != 1040 || surface_info.msaa_samples != xenos::MsaaSamples::k2X ||
      !main_view_format) {
    s.stats.other_passes++;
    return;
  }
  // Only single indexed draws for now (command 18 with one item).
  if (id != 18 || arg1 != 1) {
    s.stats.unsupported++;
    return;
  }
  uint32_t type = guest::LoadU32(base, payload);
  uint32_t base_start = guest::LoadU32(base, payload + 4);
  uint32_t primitive_count = guest::LoadU32(base, payload + 8);
  PrimitiveInfo primitive;
  if (!GetPrimitiveInfo(type, primitive)) {
    s.stats.unsupported++;
    return;
  }
  int32_t base_vertex = int32_t(base_start) >> 16;
  uint32_t start_index = base_start & 0xFFFF;
  uint32_t index_count = primitive_count * primitive.a + primitive.b;

  // Shaders, read through physical memory as the GPU reads them.
  uint32_t program_control = regs.values[XE_GPU_REG_SQ_PROGRAM_CNTL];
  uint32_t vertex_object = guest::LoadU32(base, device + kDeviceVertexShader);
  uint32_t pixel_object = guest::LoadU32(base, device + kDevicePixelShader);
  if (!vertex_object) {
    s.stats.failed++;
    return;
  }
  TranslatedShader* vertex = s.shaders->Get(
      ShaderStage::kVertex,
      PhysicalHostAddress(base, GpuPhysicalAddress(guest::LoadU32(base, vertex_object + 48))),
      guest::LoadU32(base, vertex_object + 600), program_control & 0x3F);
  TranslatedShader* pixel = nullptr;
  if (pixel_object) {
    pixel = s.shaders->Get(
        ShaderStage::kPixel,
        PhysicalHostAddress(base, GpuPhysicalAddress(guest::LoadU32(base, pixel_object + 24))),
        guest::LoadU32(base, pixel_object + 64), (program_control >> 8) & 0x3F);
  }
  if (!vertex || (pixel_object && !pixel)) {
    s.stats.failed++;
    return;
  }

  // Index buffer: +0 bit 31 = 32-bit, bits 29-30 = endianness; +24 address.
  uint32_t index_buffer = guest::LoadU32(base, device + kDeviceIndexBuffer);
  if (!index_buffer) {
    s.stats.failed++;
    return;
  }
  uint32_t index_word = guest::LoadU32(base, index_buffer);
  bool index_32bit = index_word >> 31;
  uint32_t index_endian = (index_word >> 29) & 3;
  uint32_t index_address = GpuPhysicalAddress(guest::LoadU32(base, index_buffer + 24)) +
                           start_index * (index_32bit ? 4 : 2);

  // Viewport and scissor.
  reg::RB_DEPTHCONTROL depth_control = draw_util::GetNormalizedDepthControl(regs);
  draw_util::ViewportInfo viewport_info;
  draw_util::GetHostViewportInfo(regs, 1, 1, false, 8192, 8192, true, depth_control, false,
                                 false, false, viewport_info);
  // Scissor: the window scissor registers aren't in the shadow's flushed
  // blocks, so take it as the library keeps it: enabled by render state 200
  // (device +11600), rectangle (left, top, right, bottom) at +12416 from
  // SetScissorRect; otherwise the whole target.
  draw_util::Scissor scissor{{0, 0}, {kTargetWidth, kTargetHeight}};
  if (guest::LoadU32(base, device + kDeviceScissorEnable)) {
    int32_t left = int32_t(guest::LoadU32(base, device + kDeviceScissorRect));
    int32_t top = int32_t(guest::LoadU32(base, device + kDeviceScissorRect + 4));
    int32_t right = int32_t(guest::LoadU32(base, device + kDeviceScissorRect + 8));
    int32_t bottom = int32_t(guest::LoadU32(base, device + kDeviceScissorRect + 12));
    scissor.offset[0] = uint32_t(std::max(left, 0));
    scissor.offset[1] = uint32_t(std::max(top, 0));
    scissor.extent[0] = uint32_t(std::max(right - int32_t(scissor.offset[0]), 0));
    scissor.extent[1] = uint32_t(std::max(bottom - int32_t(scissor.offset[1]), 0));
  }
  uint32_t scissor_right = std::min(scissor.offset[0] + scissor.extent[0], kTargetWidth);
  uint32_t scissor_bottom = std::min(scissor.offset[1] + scissor.extent[1], kTargetHeight);
  if (!viewport_info.xy_extent[0] || !viewport_info.xy_extent[1] ||
      scissor.offset[0] >= scissor_right || scissor.offset[1] >= scissor_bottom) {
    s.stats.invisible++;
    if (!s.stats.invisible_sample[0]) {  // one example per log line
      s.stats.invisible_sample = {viewport_info.xy_offset[0], viewport_info.xy_offset[1],
                                  viewport_info.xy_extent[0], viewport_info.xy_extent[1],
                                  scissor.offset[0], scissor.offset[1], scissor.extent[0],
                                  scissor.extent[1]};
      s.stats.invisible_sample[0] |= 0x80000000u;  // mark as set
    }
    return;  // nothing visible
  }

  // System constants (as the plugin's UpdateSystemConstantValues, for what
  // these draws use).
  Translator::SystemConstants system{};
  uint32_t flags = 0;
  auto vte = regs.Get<reg::PA_CL_VTE_CNTL>();
  if (vte.vtx_xy_fmt) {
    flags |= Translator::kSysFlag_XYDividedByW;
  }
  if (vte.vtx_z_fmt) {
    flags |= Translator::kSysFlag_ZDividedByW;
  }
  if (vte.vtx_w0_fmt) {
    flags |= Translator::kSysFlag_WNotReciprocal;
  }
  if (primitive.polygonal) {
    flags |= Translator::kSysFlag_PrimitivePolygonal;
  } else if (type == 2 || type == 3) {
    flags |= Translator::kSysFlag_PrimitiveLine;
  }
  auto color_control = regs.Get<reg::RB_COLORCONTROL>();
  uint32_t alpha_function = color_control.alpha_test_enable ? uint32_t(color_control.alpha_func)
                                                            : 7;  // always
  flags |= alpha_function << Translator::kSysFlag_AlphaPassIfLess_Shift;
  system.flags = flags;
  system.vertex_index_endian = xenos::Endian(index_endian);
  system.vertex_base_index = base_vertex;
  system.vertex_index_min = regs.values[XE_GPU_REG_VGT_MIN_VTX_INDX];
  system.vertex_index_max = regs.values[XE_GPU_REG_VGT_MAX_VTX_INDX];
  if (system.vertex_index_max <= system.vertex_index_min) {
    system.vertex_index_min = 0;
    system.vertex_index_max = xenos::kVertexIndexMask;
  }
  for (uint32_t i = 0; i < 3; i++) {
    system.ndc_scale[i] = viewport_info.ndc_scale[i];
    system.ndc_offset[i] = viewport_info.ndc_offset[i];
  }
  system.alpha_test_reference = regs.Get<float>(XE_GPU_REG_RB_ALPHA_REF);
  // Each sampled texture's post-swizzle signs, a byte per fetch constant.
  for (const TranslatedShader* shader : {vertex, pixel}) {
    if (!shader) {
      continue;
    }
    for (const ShaderTexture& texture : shader->textures) {
      uint32_t index = texture.fetch_constant;
      uint8_t signs = TextureCache::SwizzledSigns(
          &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 6 * index]);
      system.texture_swizzled_signs[index >> 2] |= uint32_t(signs) << (8 * (index & 3));
    }
  }
  for (uint32_t i = 0; i < 4; i++) {
    auto info = regs.Get<reg::RB_COLOR_INFO>(reg::RB_COLOR_INFO::rt_register_indices[i]);
    int32_t bias = info.color_exp_bias;
    uint32_t scale_bits = uint32_t(0x3F800000 + (bias << 23));
    std::memcpy(&system.color_exp_bias[i], &scale_bits, 4);
  }

  // Constants into this frame's ring: system, vertex float, pixel float,
  // bool/loop, fetch.
  uint32_t offsets[5];
  uint8_t* data;
  if (!s.Reserve(sizeof(system), offsets[0], data)) {
    s.stats.failed++;
    return;
  }
  std::memcpy(data, &system, sizeof(system));
  for (uint32_t stage = 0; stage < 2; stage++) {
    const TranslatedShader* shader = stage == 0 ? vertex : pixel;
    uint32_t source = device + (stage == 0 ? kDeviceVertexFloatConstants
                                           : kDevicePixelFloatConstants);
    if (!s.Reserve(kFloatConstantsRange, offsets[1 + stage], data)) {
      s.stats.failed++;
      return;
    }
    if (!shader) {
      continue;
    }
    // Only the registers the shader reads, packed in order.
    uint32_t packed = 0;
    for (uint32_t i = 0; i < 256; i++) {
      if (!(shader->float_constants[i >> 6] & (uint64_t(1) << (i & 63)))) {
        continue;
      }
      for (uint32_t c = 0; c < 4; c++) {
        uint32_t value = guest::LoadU32(base, source + 16 * i + 4 * c);
        std::memcpy(data + 16 * packed + 4 * c, &value, 4);
      }
      packed++;
    }
  }
  if (!s.Reserve(kBoolLoopConstantsRange, offsets[3], data)) {
    s.stats.failed++;
    return;
  }
  std::memcpy(data, &regs.values[XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031], kBoolLoopConstantsRange);
  if (!s.Reserve(kFetchConstantsRange, offsets[4], data)) {
    s.stats.failed++;
    return;
  }
  std::memcpy(data, &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0], kFetchConstantsRange);

  // Pipeline and textures.
  VkDescriptorSetLayout vertex_layout = s.GetTextureLayout(vertex, VK_SHADER_STAGE_VERTEX_BIT);
  VkDescriptorSetLayout pixel_layout = s.GetTextureLayout(pixel, VK_SHADER_STAGE_FRAGMENT_BIT);
  if (!vertex_layout || !pixel_layout) {
    s.stats.failed++;
    return;
  }
  VkDescriptorSet vertex_textures = s.GetTextureSet(base, vertex, vertex_layout);
  VkDescriptorSet pixel_textures = s.GetTextureSet(base, pixel, pixel_layout);
  if (!vertex_textures || !pixel_textures) {
    s.stats.failed++;
    return;
  }
  PipelineKey key;
  std::memset(&key, 0, sizeof(key));
  key.vertex = s.GetModule(vertex);
  key.pixel = s.GetModule(pixel);
  key.layout = s.GetPipelineLayout(vertex_layout, pixel_layout);
  key.topology = primitive.topology;
  auto mode = regs.Get<reg::PA_SU_SC_MODE_CNTL>();
  key.primitive_restart = primitive.strip && mode.multi_prim_ib_ena;
  key.cull_mode = (mode.cull_front ? VK_CULL_MODE_FRONT_BIT : 0) |
                  (mode.cull_back ? VK_CULL_MODE_BACK_BIT : 0);
  key.front_clockwise = mode.face != 0;
  key.z_enable = depth_control.z_enable;
  key.z_write = depth_control.z_write_enable;
  key.z_func = uint32_t(depth_control.zfunc);
  auto blend = regs.Get<reg::RB_BLENDCONTROL>(reg::RB_BLENDCONTROL::rt_register_indices[0]);
  key.src_color = BlendFactor(uint32_t(blend.color_srcblend));
  key.dst_color = BlendFactor(uint32_t(blend.color_destblend));
  key.color_op = BlendOp(uint32_t(blend.color_comb_fcn));
  key.src_alpha = BlendFactor(uint32_t(blend.alpha_srcblend));
  key.dst_alpha = BlendFactor(uint32_t(blend.alpha_destblend));
  key.alpha_op = BlendOp(uint32_t(blend.alpha_comb_fcn));
  key.blend_enable =
      !(key.src_color == VK_BLEND_FACTOR_ONE && key.dst_color == VK_BLEND_FACTOR_ZERO &&
        key.color_op == VK_BLEND_OP_ADD && key.src_alpha == VK_BLEND_FACTOR_ONE &&
        key.dst_alpha == VK_BLEND_FACTOR_ZERO && key.alpha_op == VK_BLEND_OP_ADD);
  key.write_mask = regs.values[XE_GPU_REG_RB_COLOR_MASK] & 0xF;
  if (!key.vertex || (pixel && !key.pixel) || !key.layout) {
    s.stats.failed++;
    return;
  }
  VkPipeline pipeline = s.GetPipeline(key);
  if (!pipeline) {
    s.stats.failed++;
    return;
  }

  // The draw's indices and vertex buffers, into the mirror. A vertex fetch
  // constant is two words: the address (word 0, low 2 bits the type) and
  // the size in words (word 1, bits 2-25).
  s.Mirror(base, index_address, index_count * (index_32bit ? 4 : 2));
  for (uint32_t k = 0; k < 96; k++) {
    if (!(vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31)))) {
      continue;
    }
    uint32_t word0 = regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k];
    uint32_t word1 = regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k + 1];
    s.Mirror(base, word0 & 0x1FFFFFFCu, ((word1 >> 2) & 0xFFFFFFu) * 4);
  }

  // Draw.
  if (!s.rendering) {
    // Reversed depth (greater, greater or equal) clears to 0.
    s.BeginRendering(commands, key.z_func == 4 || key.z_func == 6 ? 0.0f : 1.0f);
  }
  VkViewport viewport{float(viewport_info.xy_offset[0]), float(viewport_info.xy_offset[1]),
                      float(viewport_info.xy_extent[0]), float(viewport_info.xy_extent[1]),
                      viewport_info.z_min, viewport_info.z_max};
  VkRect2D scissor_rect{{int32_t(scissor.offset[0]), int32_t(scissor.offset[1])},
                        {scissor_right - scissor.offset[0], scissor_bottom - scissor.offset[1]}};
  vk::vkCmdSetViewport(commands, 0, 1, &viewport);
  vk::vkCmdSetScissor(commands, 0, 1, &scissor_rect);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  VkDescriptorSet sets[4] = {s.shared_memory_set, s.rings[s.frame].constants_set,
                             vertex_textures, pixel_textures};
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, key.layout, 0, 4, sets,
                              5, offsets);
  vk::vkCmdBindIndexBuffer(commands, s.guest_memory, index_address,
                           index_32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16);
  vk::vkCmdDrawIndexed(commands, index_count, 1, 0, 0, 0);
  s.stats.drawn++;
}

void MainView::EndFrame(VkCommandBuffer commands) {
  State& s = *state_;
  VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = s.color.image;
  barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
  if (s.rendering) {
    vk::vkCmdEndRendering(commands);
    s.rendering = false;
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1,
                             &barrier);
    return;
  }
  // No main view this frame (menus): a dark blue target.
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
  VkClearColorValue colour{{0.0f, 0.05f, 0.15f, 1.0f}};
  vk::vkCmdClearColorImage(commands, s.color.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &colour,
                           1, &barrier.subresourceRange);
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

VkImage MainView::target() const {
  return state_->color.image;
}

VkExtent2D MainView::target_extent() const {
  return {kTargetWidth, kTargetHeight};
}

MainView::Stats MainView::TakeStats() {
  Stats stats = state_->stats;
  stats.textures = state_->textures.TakeStats();
  stats.shaders_translated = state_->shaders->translated();
  stats.shaders_failed = state_->shaders->failed();
  uint32_t pipelines = state_->stats.pipelines;
  state_->stats = {};
  state_->stats.pipelines = pipelines;
  return stats;
}

}  // namespace native
