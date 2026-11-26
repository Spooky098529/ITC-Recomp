// Runs a translated game shader and its port with the same inputs on a
// Vulkan device (lavapipe works) and writes both results, for
// check_ports.py to compare.
//
//   vkdiff <job file> <result A> <result B>
//
// The job (written by check_ports.py, little-endian 32-bit words):
//   stage (0 vertex, 1 pixel), width, height
//   SPIR-V A (word count, words), SPIR-V B (word count, words)
//   interpolators: how many locations connect the stages (pixel: the test's
//     vertex shader writes 16, at 0-15; vertex: the test's geometry shader
//     reads this many)
//   textures: count, then each: view (1 2D array, 3 cube), signed, size,
//     mip levels, then RGBA8 texels for each layer and level
//   sampler count
//   system constants (512 bytes), float constants (vec4 count, data),
//   bool and loop constants (160 bytes), fetch constants (768 bytes)
//   shared memory (word count, words)
//   vertex shaders: the vertex count (one point each); pixel shaders: the
//   test vertex shader's 3 vertices' 16 interpolators, at shared memory word 0.
// A result is width x height RGBA32F texels: pixel shaders, the color; vertex
// shaders, a row per vertex: the position, then the interpolators.

#include <vulkan/vulkan.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

#define CHECK(x)                                                        \
  do {                                                                  \
    VkResult r_ = (x);                                                  \
    if (r_ != VK_SUCCESS) {                                             \
      std::fprintf(stderr, "%s:%d: %s = %d\n", __FILE__, __LINE__, #x, r_); \
      std::exit(2);                                                     \
    }                                                                   \
  } while (0)

#include "harness_shaders.h"  // build.sh: k_pixel_test_vert, k_vertex_test_geom, k_vertex_test_frag

namespace {

struct Reader {
  std::vector<uint32_t> w;
  size_t at = 0;
  uint32_t U() { return w.at(at++); }
  std::vector<uint32_t> Words(size_t n) {
    std::vector<uint32_t> r(w.begin() + at, w.begin() + at + n);
    at += n;
    return r;
  }
  std::vector<uint8_t> Bytes(size_t n) {
    std::vector<uint8_t> r(n);
    std::memcpy(r.data(), &w[at], n);
    at += (n + 3) / 4;
    return r;
  }
};

VkInstance instance;
VkPhysicalDevice physical;
VkDevice device;
VkQueue queue;
VkCommandPool pool;
VkPhysicalDeviceMemoryProperties memory_properties;

uint32_t MemoryType(uint32_t bits, VkMemoryPropertyFlags flags) {
  for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
    if ((bits >> i & 1) && (memory_properties.memoryTypes[i].propertyFlags & flags) == flags) {
      return i;
    }
  }
  std::fprintf(stderr, "no memory type\n");
  std::exit(2);
}

struct Buffer {
  VkBuffer buffer;
  VkDeviceMemory memory;
  void* mapped;
};
Buffer MakeBuffer(VkDeviceSize size, VkBufferUsageFlags usage, const void* data) {
  Buffer b;
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = size;
  info.usage = usage;
  CHECK(vkCreateBuffer(device, &info, nullptr, &b.buffer));
  VkMemoryRequirements req;
  vkGetBufferMemoryRequirements(device, b.buffer, &req);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = req.size;
  alloc.memoryTypeIndex = MemoryType(req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  CHECK(vkAllocateMemory(device, &alloc, nullptr, &b.memory));
  CHECK(vkBindBufferMemory(device, b.buffer, b.memory, 0));
  CHECK(vkMapMemory(device, b.memory, 0, size, 0, &b.mapped));
  if (data) {
    std::memcpy(b.mapped, data, size);
  } else {
    std::memset(b.mapped, 0, size);
  }
  return b;
}

VkCommandBuffer Begin() {
  VkCommandBufferAllocateInfo info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
  info.commandPool = pool;
  info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  info.commandBufferCount = 1;
  VkCommandBuffer cb;
  CHECK(vkAllocateCommandBuffers(device, &info, &cb));
  VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  CHECK(vkBeginCommandBuffer(cb, &begin));
  return cb;
}
void Submit(VkCommandBuffer cb) {
  CHECK(vkEndCommandBuffer(cb));
  VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submit.commandBufferCount = 1;
  submit.pCommandBuffers = &cb;
  CHECK(vkQueueSubmit(queue, 1, &submit, VK_NULL_HANDLE));
  CHECK(vkQueueWaitIdle(queue));
  vkFreeCommandBuffers(device, pool, 1, &cb);
}

void Barrier(VkCommandBuffer cb, VkImage image, VkImageLayout from, VkImageLayout to,
             uint32_t levels, uint32_t layers) {
  VkImageMemoryBarrier b{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  b.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
  b.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
  b.oldLayout = from;
  b.newLayout = to;
  b.srcQueueFamilyIndex = b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  b.image = image;
  b.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, levels, 0, layers};
  vkCmdPipelineBarrier(cb, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                       0, 0, nullptr, 0, nullptr, 1, &b);
}

struct Texture {
  VkImage image;
  VkImageView view;
};
Texture MakeTexture(uint32_t view_kind, bool is_signed, uint32_t size, uint32_t levels,
                    const std::vector<uint8_t>& texels) {
  bool cube = view_kind == 3;
  uint32_t layers = cube ? 6 : 1;
  VkFormat format = is_signed ? VK_FORMAT_R8G8B8A8_SNORM : VK_FORMAT_R8G8B8A8_UNORM;
  VkImageCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  info.flags = cube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
  info.imageType = VK_IMAGE_TYPE_2D;
  info.format = format;
  info.extent = {size, size, 1};
  info.mipLevels = levels;
  info.arrayLayers = layers;
  info.samples = VK_SAMPLE_COUNT_1_BIT;
  info.tiling = VK_IMAGE_TILING_OPTIMAL;
  info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  Texture t;
  CHECK(vkCreateImage(device, &info, nullptr, &t.image));
  VkMemoryRequirements req;
  vkGetImageMemoryRequirements(device, t.image, &req);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = req.size;
  alloc.memoryTypeIndex = MemoryType(req.memoryTypeBits, 0);
  VkDeviceMemory memory;
  CHECK(vkAllocateMemory(device, &alloc, nullptr, &memory));
  CHECK(vkBindImageMemory(device, t.image, memory, 0));
  Buffer staging = MakeBuffer(texels.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, texels.data());
  VkCommandBuffer cb = Begin();
  Barrier(cb, t.image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, levels,
          layers);
  VkDeviceSize offset = 0;
  for (uint32_t layer = 0; layer < layers; layer++) {
    for (uint32_t level = 0; level < levels; level++) {
      uint32_t s = size >> level ? size >> level : 1;
      VkBufferImageCopy copy{};
      copy.bufferOffset = offset;
      copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, level, layer, 1};
      copy.imageExtent = {s, s, 1};
      vkCmdCopyBufferToImage(cb, staging.buffer, t.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                             &copy);
      offset += VkDeviceSize(s) * s * 4;
    }
  }
  Barrier(cb, t.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, levels, layers);
  Submit(cb);
  VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  view.image = t.image;
  view.viewType = cube ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  view.format = format;
  view.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, levels, 0, layers};
  CHECK(vkCreateImageView(device, &view, nullptr, &t.view));
  return t;
}

VkShaderModule Module(const std::vector<uint32_t>& words) {
  VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
  info.codeSize = words.size() * 4;
  info.pCode = words.data();
  VkShaderModule m;
  CHECK(vkCreateShaderModule(device, &info, nullptr, &m));
  return m;
}
template <size_t N>
VkShaderModule Module(const uint32_t (&words)[N]) {
  return Module(std::vector<uint32_t>(words, words + N));
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 4) {
    std::fprintf(stderr, "usage: vkdiff <job> <result A> <result B>\n");
    return 1;
  }
  Reader job;
  {
    std::ifstream in(argv[1], std::ios::binary);
    std::vector<char> bytes((std::istreambuf_iterator<char>(in)), {});
    job.w.resize(bytes.size() / 4);
    std::memcpy(job.w.data(), bytes.data(), job.w.size() * 4);
  }
  bool pixel = job.U() == 1;
  uint32_t width = job.U(), height = job.U();
  std::vector<uint32_t> spirv[2];
  for (auto& s : spirv) {
    s = job.Words(job.U());
  }
  uint32_t interpolators = job.U();
  struct TextureDesc {
    uint32_t view, is_signed, size, levels;
    std::vector<uint8_t> texels;
  };
  std::vector<TextureDesc> texture_descs(job.U());
  for (auto& t : texture_descs) {
    t.view = job.U();
    t.is_signed = job.U();
    t.size = job.U();
    t.levels = job.U();
    size_t bytes = 0;
    for (uint32_t level = 0; level < t.levels; level++) {
      uint32_t s = t.size >> level ? t.size >> level : 1;
      bytes += size_t(s) * s * 4;
    }
    t.texels = job.Bytes(bytes * (t.view == 3 ? 6 : 1));
  }
  uint32_t sampler_count = job.U();
  std::vector<uint8_t> system = job.Bytes(512);
  uint32_t float_count = job.U();
  std::vector<uint8_t> floats = job.Bytes(float_count * 16);
  std::vector<uint8_t> bool_loop = job.Bytes(160);
  std::vector<uint8_t> fetch = job.Bytes(768);
  std::vector<uint32_t> memory = job.Words(job.U());
  uint32_t vertex_count = pixel ? 3 : job.U();

  // The device.
  VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app.apiVersion = VK_API_VERSION_1_3;
  VkInstanceCreateInfo instance_info{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
  instance_info.pApplicationInfo = &app;
  CHECK(vkCreateInstance(&instance_info, nullptr, &instance));
  uint32_t count = 1;
  vkEnumeratePhysicalDevices(instance, &count, &physical);
  vkGetPhysicalDeviceMemoryProperties(physical, &memory_properties);
  float priority = 1;
  VkDeviceQueueCreateInfo queue_info{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = &priority;
  VkPhysicalDeviceVulkan12Features features12{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
  features12.scalarBlockLayout = VK_TRUE;
  features12.uniformBufferStandardLayout = VK_TRUE;
  VkPhysicalDeviceFeatures2 features{VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  features.pNext = &features12;
  features.features.geometryShader = VK_TRUE;
  features.features.shaderClipDistance = VK_TRUE;
  features.features.shaderCullDistance = VK_TRUE;
  features.features.robustBufferAccess = VK_TRUE;
  features.features.vertexPipelineStoresAndAtomics = VK_TRUE;
  features.features.fragmentStoresAndAtomics = VK_TRUE;
  VkDeviceCreateInfo device_info{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  device_info.pNext = &features;
  device_info.queueCreateInfoCount = 1;
  device_info.pQueueCreateInfos = &queue_info;
  const char* extensions[] = {"VK_EXT_shader_demote_to_helper_invocation"};
  device_info.enabledExtensionCount = 1;
  device_info.ppEnabledExtensionNames = extensions;
  CHECK(vkCreateDevice(physical, &device_info, nullptr, &device));
  vkGetDeviceQueue(device, 0, 0, &queue);
  VkCommandPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
  CHECK(vkCreateCommandPool(device, &pool_info, nullptr, &pool));

  // The resources.
  Buffer shared = MakeBuffer(memory.size() * 4, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, memory.data());
  Buffer uniforms[5] = {
      MakeBuffer(512, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, system.data()),
      MakeBuffer(floats.size(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, floats.data()),
      MakeBuffer(floats.size(), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, floats.data()),
      MakeBuffer(160, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, bool_loop.data()),
      MakeBuffer(768, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, fetch.data()),
  };
  VkDeviceSize uniform_sizes[5] = {512, floats.size(), floats.size(), 160, 768};
  std::vector<Texture> textures;
  for (auto& t : texture_descs) {
    textures.push_back(MakeTexture(t.view, t.is_signed, t.size, t.levels, t.texels));
  }
  VkSamplerCreateInfo sampler_info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  sampler_info.magFilter = sampler_info.minFilter = VK_FILTER_LINEAR;
  sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  sampler_info.addressModeU = sampler_info.addressModeV = sampler_info.addressModeW =
      VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_info.maxLod = 16;
  VkSampler sampler;
  CHECK(vkCreateSampler(device, &sampler_info, nullptr, &sampler));

  // The descriptor sets: shared memory, the constants, vertex textures and
  // pixel textures, as the renderer's.
  VkDescriptorSetLayout layouts[4];
  {
    VkDescriptorSetLayoutBinding b{0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_ALL,
                                   nullptr};
    VkDescriptorSetLayoutCreateInfo info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    info.bindingCount = 1;
    info.pBindings = &b;
    CHECK(vkCreateDescriptorSetLayout(device, &info, nullptr, &layouts[0]));
    VkDescriptorSetLayoutBinding u[5];
    for (uint32_t i = 0; i < 5; i++) {
      u[i] = {i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_ALL, nullptr};
    }
    info.bindingCount = 5;
    info.pBindings = u;
    CHECK(vkCreateDescriptorSetLayout(device, &info, nullptr, &layouts[1]));
    for (int stage = 0; stage < 2; stage++) {
      std::vector<VkDescriptorSetLayoutBinding> t;
      bool used = (stage == 1) == pixel;
      if (used) {
        for (uint32_t i = 0; i < textures.size(); i++) {
          t.push_back({i, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_ALL, nullptr});
        }
        for (uint32_t i = 0; i < sampler_count; i++) {
          t.push_back({uint32_t(textures.size()) + i, VK_DESCRIPTOR_TYPE_SAMPLER, 1,
                       VK_SHADER_STAGE_ALL, nullptr});
        }
      }
      info.bindingCount = uint32_t(t.size());
      info.pBindings = t.data();
      CHECK(vkCreateDescriptorSetLayout(device, &info, nullptr, &layouts[2 + stage]));
    }
  }
  VkDescriptorPoolSize sizes[] = {{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1},
                                  {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 5},
                                  {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 64},
                                  {VK_DESCRIPTOR_TYPE_SAMPLER, 64}};
  VkDescriptorPoolCreateInfo dp{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  dp.maxSets = 4;
  dp.poolSizeCount = 4;
  dp.pPoolSizes = sizes;
  VkDescriptorPool descriptor_pool;
  CHECK(vkCreateDescriptorPool(device, &dp, nullptr, &descriptor_pool));
  VkDescriptorSet sets[4];
  VkDescriptorSetAllocateInfo ds{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  ds.descriptorPool = descriptor_pool;
  ds.descriptorSetCount = 4;
  ds.pSetLayouts = layouts;
  CHECK(vkAllocateDescriptorSets(device, &ds, sets));
  std::vector<VkWriteDescriptorSet> writes;
  std::vector<VkDescriptorBufferInfo> buffer_infos(6);
  std::vector<VkDescriptorImageInfo> image_infos(textures.size() + sampler_count);
  buffer_infos[0] = {shared.buffer, 0, VK_WHOLE_SIZE};
  writes.push_back({VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, sets[0], 0, 0, 1,
                    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, nullptr, &buffer_infos[0], nullptr});
  for (uint32_t i = 0; i < 5; i++) {
    buffer_infos[1 + i] = {uniforms[i].buffer, 0, uniform_sizes[i]};
    writes.push_back({VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, sets[1], i, 0, 1,
                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nullptr, &buffer_infos[1 + i], nullptr});
  }
  VkDescriptorSet texture_set = sets[pixel ? 3 : 2];
  for (uint32_t i = 0; i < textures.size(); i++) {
    image_infos[i] = {VK_NULL_HANDLE, textures[i].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    writes.push_back({VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, texture_set, i, 0, 1,
                      VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, &image_infos[i], nullptr, nullptr});
  }
  for (uint32_t i = 0; i < sampler_count; i++) {
    uint32_t n = uint32_t(textures.size()) + i;
    image_infos[n] = {sampler, VK_NULL_HANDLE, VK_IMAGE_LAYOUT_UNDEFINED};
    writes.push_back({VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr, texture_set, n, 0, 1,
                      VK_DESCRIPTOR_TYPE_SAMPLER, &image_infos[n], nullptr, nullptr});
  }
  vkUpdateDescriptorSets(device, uint32_t(writes.size()), writes.data(), 0, nullptr);
  VkPushConstantRange push{VK_SHADER_STAGE_ALL, 0, 16};
  VkPipelineLayoutCreateInfo pl{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
  pl.setLayoutCount = 4;
  pl.pSetLayouts = layouts;
  pl.pushConstantRangeCount = 1;
  pl.pPushConstantRanges = &push;
  VkPipelineLayout pipeline_layout;
  CHECK(vkCreatePipelineLayout(device, &pl, nullptr, &pipeline_layout));

  // The render target and its readback.
  VkImageCreateInfo rt_info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  rt_info.imageType = VK_IMAGE_TYPE_2D;
  rt_info.format = VK_FORMAT_R32G32B32A32_SFLOAT;
  rt_info.extent = {width, height, 1};
  rt_info.mipLevels = rt_info.arrayLayers = 1;
  rt_info.samples = VK_SAMPLE_COUNT_1_BIT;
  rt_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  VkImage rt;
  CHECK(vkCreateImage(device, &rt_info, nullptr, &rt));
  VkMemoryRequirements req;
  vkGetImageMemoryRequirements(device, rt, &req);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = req.size;
  alloc.memoryTypeIndex = MemoryType(req.memoryTypeBits, 0);
  VkDeviceMemory rt_memory;
  CHECK(vkAllocateMemory(device, &alloc, nullptr, &rt_memory));
  CHECK(vkBindImageMemory(device, rt, rt_memory, 0));
  VkImageViewCreateInfo rt_view_info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  rt_view_info.image = rt;
  rt_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  rt_view_info.format = rt_info.format;
  rt_view_info.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
  VkImageView rt_view;
  CHECK(vkCreateImageView(device, &rt_view_info, nullptr, &rt_view));
  VkAttachmentDescription attachment{};
  attachment.format = rt_info.format;
  attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  attachment.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
  VkAttachmentReference ref{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &ref;
  VkRenderPassCreateInfo rp{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
  rp.attachmentCount = 1;
  rp.pAttachments = &attachment;
  rp.subpassCount = 1;
  rp.pSubpasses = &subpass;
  VkRenderPass render_pass;
  CHECK(vkCreateRenderPass(device, &rp, nullptr, &render_pass));
  VkFramebufferCreateInfo fb{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
  fb.renderPass = render_pass;
  fb.attachmentCount = 1;
  fb.pAttachments = &rt_view;
  fb.width = width;
  fb.height = height;
  fb.layers = 1;
  VkFramebuffer framebuffer;
  CHECK(vkCreateFramebuffer(device, &fb, nullptr, &framebuffer));
  Buffer readback = MakeBuffer(VkDeviceSize(width) * height * 16, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                               nullptr);

  for (int which = 0; which < 2; which++) {
    VkShaderModule tested = Module(spirv[which]);
    std::vector<VkPipelineShaderStageCreateInfo> stages;
    auto stage = [&](VkShaderStageFlagBits s, VkShaderModule m) {
      VkPipelineShaderStageCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
      info.stage = s;
      info.module = m;
      info.pName = "main";
      stages.push_back(info);
    };
    if (pixel) {
      stage(VK_SHADER_STAGE_VERTEX_BIT, Module(k_pixel_test_vert));
      stage(VK_SHADER_STAGE_FRAGMENT_BIT, tested);
    } else {
      stage(VK_SHADER_STAGE_VERTEX_BIT, tested);
      stage(VK_SHADER_STAGE_GEOMETRY_BIT, Module(k_vertex_test_geom));
      stage(VK_SHADER_STAGE_FRAGMENT_BIT, Module(k_vertex_test_frag));
    }
    VkPipelineVertexInputStateCreateInfo vi{VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
    VkPipelineInputAssemblyStateCreateInfo ia{VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
    ia.topology = pixel ? VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST : VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    VkViewport viewport{0, 0, float(width), float(height), 0, 1};
    VkRect2D scissor{{0, 0}, {width, height}};
    VkPipelineViewportStateCreateInfo vp{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
    vp.viewportCount = vp.scissorCount = 1;
    vp.pViewports = &viewport;
    vp.pScissors = &scissor;
    VkPipelineRasterizationStateCreateInfo rs{VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_NONE;
    rs.lineWidth = 1;
    VkPipelineMultisampleStateCreateInfo ms{VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPipelineColorBlendAttachmentState blend{};
    blend.colorWriteMask = 0xF;
    VkPipelineColorBlendStateCreateInfo cb_state{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
    cb_state.attachmentCount = 1;
    cb_state.pAttachments = &blend;
    VkGraphicsPipelineCreateInfo gp{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    gp.stageCount = uint32_t(stages.size());
    gp.pStages = stages.data();
    gp.pVertexInputState = &vi;
    gp.pInputAssemblyState = &ia;
    gp.pViewportState = &vp;
    gp.pRasterizationState = &rs;
    gp.pMultisampleState = &ms;
    gp.pColorBlendState = &cb_state;
    gp.layout = pipeline_layout;
    gp.renderPass = render_pass;
    VkPipeline pipeline;
    CHECK(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &gp, nullptr, &pipeline));

    VkCommandBuffer cb = Begin();
    VkClearValue clear{};
    clear.color.float32[0] = -12345.0f;  // marks pixels a shader didn't write
    VkRenderPassBeginInfo begin{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    begin.renderPass = render_pass;
    begin.framebuffer = framebuffer;
    begin.renderArea = scissor;
    begin.clearValueCount = 1;
    begin.pClearValues = &clear;
    vkCmdBeginRenderPass(cb, &begin, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 4, sets, 0,
                            nullptr);
    uint32_t constants[4] = {width, height, interpolators, 0};
    vkCmdPushConstants(cb, pipeline_layout, VK_SHADER_STAGE_ALL, 0, 16, constants);
    vkCmdDraw(cb, vertex_count, 1, 0, 0);
    vkCmdEndRenderPass(cb);
    VkBufferImageCopy copy{};
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {width, height, 1};
    vkCmdCopyImageToBuffer(cb, rt, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, readback.buffer, 1, &copy);
    Submit(cb);
    std::ofstream(argv[2 + which], std::ios::binary)
        .write(static_cast<const char*>(readback.mapped), std::streamsize(width) * height * 16);
    vkDestroyPipeline(device, pipeline, nullptr);
  }
  return 0;
}
