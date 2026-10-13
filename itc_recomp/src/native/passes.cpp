#include "passes.h"

#include "generated_resolve_frag.h"
#include "generated_resolve_ms_frag.h"
#include "generated_resolve_vert.h"

#include "guest.h"
#include "shaders.h"
#include "state.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <rex/graphics/pipeline/shader/spirv_translator.h>
#include <rex/graphics/register_file.h>
#include <rex/graphics/registers.h>
#include <rex/graphics/util/draw.h>
#include <rex/graphics/xenos.h>
#include <rex/hash.h>
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
using graphics::XE_GPU_REG_SQ_CONTEXT_MISC;
using graphics::XE_GPU_REG_SQ_PROGRAM_CNTL;
using graphics::XE_GPU_REG_VGT_MAX_VTX_INDX;
using graphics::XE_GPU_REG_VGT_MIN_VTX_INDX;

namespace {

// Every depth surface (24_8 and 24_8_FLOAT) is drawn as this.
constexpr VkFormat kDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

// The main view, which is shown: its surface (pitch 1040, 2x MSAA, 7e3
// colour, drawn as RGBA16F) and the part of it drawn (its viewport).
constexpr uint32_t kMainViewPitch = 1040;
constexpr uint32_t kMainViewWidth = 1024, kMainViewHeight = 576;
// What's shown is copied into an image of this size.
constexpr uint32_t kShownWidth = 1280, kShownHeight = 720;
constexpr VkFormat kShownFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

// EDRAM: 2048 tiles of 80x16 samples at 32 bits a sample.
constexpr uint32_t kEdramTiles = 2048;

// Device fields: the bound surfaces (render targets 0-3, then depth).
constexpr uint32_t kDeviceDepthStencil = 12200;

// A surface object's words (see "Resources" in docs/gpu-functions.md):
// +24 RB_SURFACE_INFO (pitch in bits 0-13, MSAA in bits 16-17), +28
// RB_COLOR_INFO or RB_DEPTH_INFO (EDRAM base in bits 0-11, format in bits
// 16-19).
struct Surface {
  uint32_t pitch, msaa, edram_base, format;
};
Surface ReadSurface(uint8_t* base, uint32_t object) {
  uint32_t surface_info = guest::LoadU32(base, object + 24);
  uint32_t info = guest::LoadU32(base, object + 28);
  return {surface_info & 0x3FFF, (surface_info >> 16) & 3, info & 0xFFF, (info >> 16) & 0xF};
}

// How a colour format is drawn. 7e3 and its high-precision blending
// variant (which draws switch between, see render states 308-320) share
// one; the fixed-point -32..32 formats are drawn as floats.
VkFormat ColorFormat(uint32_t format) {
  switch (xenos::ColorRenderTargetFormat(format)) {
    case xenos::ColorRenderTargetFormat::k_8_8_8_8:
    case xenos::ColorRenderTargetFormat::k_8_8_8_8_GAMMA:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case xenos::ColorRenderTargetFormat::k_2_10_10_10:
    case xenos::ColorRenderTargetFormat::k_2_10_10_10_AS_10_10_10_10:
      return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    case xenos::ColorRenderTargetFormat::k_2_10_10_10_FLOAT:
    case xenos::ColorRenderTargetFormat::k_2_10_10_10_FLOAT_AS_16_16_16_16:
    case xenos::ColorRenderTargetFormat::k_16_16_16_16:
    case xenos::ColorRenderTargetFormat::k_16_16_16_16_FLOAT:
      return VK_FORMAT_R16G16B16A16_SFLOAT;
    case xenos::ColorRenderTargetFormat::k_16_16:
    case xenos::ColorRenderTargetFormat::k_16_16_FLOAT:
      return VK_FORMAT_R16G16_SFLOAT;
    case xenos::ColorRenderTargetFormat::k_32_FLOAT:
      return VK_FORMAT_R32_SFLOAT;
    case xenos::ColorRenderTargetFormat::k_32_32_FLOAT:
      return VK_FORMAT_R32G32_SFLOAT;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

VkSampleCountFlagBits SampleCount(uint32_t msaa) {
  return msaa == 2 ? VK_SAMPLE_COUNT_4_BIT : msaa == 1 ? VK_SAMPLE_COUNT_2_BIT
                                                       : VK_SAMPLE_COUNT_1_BIT;
}

// Each surface gets an image of its pitch, as tall as the EDRAM from its
// base holds (2x MSAA doubles the samples down, 4x also across).
VkExtent2D SurfaceExtent(const Surface& surface, bool depth) {
  uint32_t samples_x = surface.msaa == 2 ? 2 : 1, samples_y = surface.msaa ? 2 : 1;
  bool wide = !depth && xenos::IsColorRenderTargetFormat64bpp(
                            xenos::ColorRenderTargetFormat(surface.format));
  uint32_t tiles_across = std::max((surface.pitch * samples_x + 79) / 80, 1u) * (wide ? 2 : 1);
  uint32_t tile_rows = (kEdramTiles - std::min(surface.edram_base, kEdramTiles - 1)) / tiles_across;
  uint32_t height = tile_rows * 16 / samples_y;
  return {std::max(surface.pitch, 1u), std::clamp(height, 16u, 4096u)};
}

constexpr VkAccessFlags kAllAccess = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;

// Changes an image's layout, waiting for everything before.
void Transition(VkCommandBuffer commands, VkImage image, VkImageAspectFlags aspect,
                VkImageLayout& layout, VkImageLayout wanted) {
  if (layout == wanted) {
    return;
  }
  VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  barrier.srcAccessMask = kAllAccess;
  barrier.dstAccessMask = kAllAccess;
  barrier.oldLayout = layout;
  barrier.newLayout = wanted;
  barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange = {aspect, 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS};
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                           VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1,
                           &barrier);
  layout = wanted;
}

constexpr VkImageAspectFlags kDepthStencilAspects =
    VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

// Resolve draws a frame can make (a descriptor set each).
constexpr uint32_t kResolvesPerFrame = 256;

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
  // The targets: colour format (UNDEFINED for none), depth or not, samples.
  uint32_t color_format, depth, samples;
};

struct Image {
  VkImage image = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  VkImageView view = VK_NULL_HANDLE;
};

// A surface's image: what's drawn while it's bound, and resolved from.
struct Target {
  bool depth;
  Surface surface;
  VkFormat format;
  VkSampleCountFlagBits samples;
  VkExtent2D extent;
  Image image;               // view: for drawing (depth and stencil)
  VkImageView sample_view;   // for resolving (depth only, for depth)
  VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
  VkImageAspectFlags aspects() const {
    return depth ? kDepthStencilAspects : VK_IMAGE_ASPECT_COLOR_BIT;
  }
};

// Texture descriptor sets (sets 2 and 3) a frame can allocate, and their
// descriptors.
constexpr uint32_t kTextureSetsPerFrame = 4096;
constexpr uint32_t kTextureDescriptorsPerFrame = 16384;

}  // namespace

struct Passes::State {
  DeviceContext context{};
  VkPhysicalDeviceMemoryProperties memory_properties{};
  uint32_t frames_in_flight = 0;

  // Mirrors of guest physical memory at the same offsets: the shaders'
  // storage buffer (set 0) and the index buffer. The driver won't import
  // guest memory itself (a shared memory-file mapping), so each draw's
  // index and vertex ranges are copied in, once a frame. One per frame in
  // flight: with one shared, the next frame's copies overwrote vertices the
  // GPU was still drawing (buildings showing a rainbow texture).
  struct GuestMirror {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapped = nullptr;
    VkDescriptorSet set = VK_NULL_HANDLE;
  };
  std::vector<GuestMirror> mirrors;
  std::unordered_set<uint64_t> mirrored;  // address << 32 | size, this frame
  // With check_mirror: each range's hash when copied, to find ranges whose
  // contents change within a frame (draws after the change get the first).
  bool check_mirror = false;
  std::unordered_map<uint64_t, uint64_t> mirrored_hashes;
  // With check_mirror_frames: each range's hash in the frame it was last
  // copied, and how often it changed since the last log.
  bool check_mirror_frames = false;
  struct RangeHistory {
    uint64_t hash;
    uint32_t changes;
    bool index;
  };
  std::unordered_map<uint64_t, RangeHistory> range_history;
  // Debugging aids: draws of this pixel shader (hash) are skipped; with
  // record_draws, each draw is described in a line (written with a dump).
  uint64_t skip_pixel_shader = 0;
  bool record_draws = false;
  struct RecordedDraw {
    bool main_view, has_color, has_depth;
    Surface surface;
    uint64_t vertex, pixel;
    uint32_t type, count;
    struct Texture {
      bool pixel;
      ShaderTexture texture;
      uint32_t fetch[6];
    };
    std::vector<Texture> textures;
  };
  std::vector<RecordedDraw> recorded_draws;
  // The pixel shaders drawn into the main view this frame and the last.
  std::vector<uint64_t> frame_pixel_shaders, last_pixel_shaders;

  // Targets by surface (colour or depth, EDRAM base, pitch, samples, the
  // format they're drawn in), and the ones being drawn into now.
  std::map<std::array<uint32_t, 5>, Target> targets;
  Target* bound_color = nullptr;
  Target* bound_depth = nullptr;
  VkExtent2D render_extent{};
  bool rendering = false;
  Target* main_view = nullptr;  // drawn into this frame

  // What's shown, and this frame's resolve destinations (for showing one).
  Image shown;
  VkImageLayout shown_layout = VK_IMAGE_LAYOUT_UNDEFINED;
  struct Resolved {
    VkImage image;
    VkExtent2D extent;
    uint32_t address, format, flags;
    VkFormat host_format;
  };
  std::vector<Resolved> frame_resolves;
  uint64_t frames = 0;  // for logging the resolve list now and then
  size_t last_frame_resolves = 0;

  // A dump being read back: the images' data, one after another.
  struct Dumped {
    std::string name;
    VkFormat format;
    VkExtent2D extent;
    VkDeviceSize offset;
  };
  std::vector<Dumped> dumped;
  VkBuffer dump_buffer = VK_NULL_HANDLE;
  VkDeviceMemory dump_memory = VK_NULL_HANDLE;
  void RecordDump(VkCommandBuffer commands, VkExtent2D shown_extent, const std::string& prefix,
                  bool with_resolves);
  bool logged_extra_targets = false;

  // Resolves: drawn with our own shaders (src/native/glsl), sampling the
  // source target; pipelines by destination format and multisampling.
  VkDescriptorSetLayout resolve_set_layout = VK_NULL_HANDLE;
  VkPipelineLayout resolve_layout = VK_NULL_HANDLE;
  VkShaderModule resolve_vertex = VK_NULL_HANDLE, resolve_pixel = VK_NULL_HANDLE,
                 resolve_pixel_ms = VK_NULL_HANDLE;
  VkSampler resolve_sampler = VK_NULL_HANDLE;
  std::map<std::pair<VkFormat, bool>, VkPipeline> resolve_pipelines;
  std::vector<VkDescriptorPool> resolve_pools;  // per frame in flight
  // Source and destination pairs logged so far.
  std::unordered_set<uint64_t> logged_resolves;

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
                   VkImageAspectFlags aspect, VkImageCreateFlags flags = 0,
                   VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
  // The bound surface's target, made (and cleared) on first use; null if
  // its format isn't drawn.
  Target* GetTarget(VkCommandBuffer commands, uint8_t* base, uint32_t object, bool depth);
  // Clears the whole target (aspects: all if 0).
  void ClearTarget(VkCommandBuffer commands, Target& target, const VkClearValue& value,
                   VkImageAspectFlags aspects = 0);
  // Draws the source's rectangle into the resolve's destination texture.
  bool ResolveInto(VkCommandBuffer commands, uint8_t* base, Target& source, uint32_t destination,
                   uint32_t item, uint32_t fields, uint32_t flags);
  bool CreateResolveResources();
  VkPipeline GetResolvePipeline(VkFormat format, bool multisample);
  void EndRendering(VkCommandBuffer commands);
  bool CreateGuestMemoryMirror();
  bool CreateGuestMemoryMirror(GuestMirror& mirror);
  void Mirror(uint8_t* base, uint32_t address, uint32_t size, bool index);
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
  void BeginRendering(VkCommandBuffer commands, Target* color, Target* depth);
  void Draw(VkCommandBuffer commands, uint8_t* base, uint32_t device, uint32_t item);
};

bool Passes::State::CreateImage(Image& out, VkImageType type, VkImageViewType view_type,
                                  VkFormat format, VkExtent3D extent, uint32_t layers,
                                  VkImageUsageFlags usage, VkImageAspectFlags aspect,
                                  VkImageCreateFlags flags, VkSampleCountFlagBits samples) {
  VkDevice device = context.device;
  VkImageCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  info.flags = flags;
  info.imageType = type;
  info.format = format;
  info.extent = extent;
  info.mipLevels = 1;
  info.arrayLayers = layers;
  info.samples = samples;
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

bool Passes::State::CreateGuestMemoryMirror() {
  mirrors.resize(frames_in_flight);
  for (GuestMirror& mirror : mirrors) {
    if (!CreateGuestMemoryMirror(mirror)) {
      return false;
    }
  }
  REXLOG_INFO("native renderer: {} guest memory mirrors of {} MB", mirrors.size(),
              kPhysicalMemorySize >> 20);
  return true;
}

bool Passes::State::CreateGuestMemoryMirror(GuestMirror& mirror) {
  VkDevice device = context.device;
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = kPhysicalMemorySize;
  info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &info, nullptr, &mirror.buffer) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, mirror.buffer, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex =
      FindMemoryType(memory_properties, requirements.memoryTypeBits,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX) {
    REXLOG_ERROR("native renderer: no host-visible memory for the guest memory mirror");
    return false;
  }
  if (VkResult result = vk::vkAllocateMemory(device, &alloc, nullptr, &mirror.memory);
      result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: allocating the guest memory mirror failed: {}", int(result));
    return false;
  }
  return vk::vkBindBufferMemory(device, mirror.buffer, mirror.memory, 0) == VK_SUCCESS &&
         vk::vkMapMemory(device, mirror.memory, 0, VK_WHOLE_SIZE, 0,
                         reinterpret_cast<void**>(&mirror.mapped)) == VK_SUCCESS;
}

void Passes::State::Mirror(uint8_t* base, uint32_t address, uint32_t size, bool index) {
  if (!size || address >= kPhysicalMemorySize) {
    return;
  }
  size = std::min(size, kPhysicalMemorySize - address);
  uint64_t key = (uint64_t(address) << 32) | size;
  if (check_mirror) {
    uint64_t hash = XXH3_64bits(PhysicalHostAddress(base, address), size);
    auto [it, added] = mirrored_hashes.try_emplace(key, hash);
    if (!added && it->second != hash) {
      it->second = hash;
      stats.mirror_rewritten++;
      stats.mirror_rewritten_bytes += size;
      if (!stats.mirror_rewritten_sample_size) {
        stats.mirror_rewritten_sample_address = address;
        stats.mirror_rewritten_sample_size = size;
      }
    }
  }
  if (!mirrored.insert(key).second) {
    return;  // already copied this frame
  }
  if (check_mirror_frames) {
    uint64_t hash = XXH3_64bits(PhysicalHostAddress(base, address), size);
    auto [it, added] = range_history.try_emplace(key, RangeHistory{hash, 0, index});
    if (!added && it->second.hash != hash) {
      it->second.hash = hash;
      it->second.changes++;
    }
  }
  std::memcpy(mirrors[frame].mapped + address, PhysicalHostAddress(base, address), size);
  stats.mirrored_bytes += size;
}

bool Passes::State::CreateRings() {
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

bool Passes::State::CreateDescriptors() {
  VkDevice device = context.device;
  VkDescriptorPoolSize sizes[] = {
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, frames_in_flight},
      {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 5 * frames_in_flight},
  };
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 2 * frames_in_flight;
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
  for (GuestMirror& mirror : mirrors) {
    if (vk::vkAllocateDescriptorSets(device, &alloc, &mirror.set) != VK_SUCCESS) {
      return false;
    }
    VkDescriptorBufferInfo shared_buffer{mirror.buffer, 0, VK_WHOLE_SIZE};
    VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    write.dstSet = mirror.set;
    write.dstBinding = 0;
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    write.pBufferInfo = &shared_buffer;
    vk::vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
  }

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

bool Passes::State::CreateDummyTextures() {
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

VkDescriptorSetLayout Passes::State::GetTextureLayout(const TranslatedShader* shader,
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

VkDescriptorSet Passes::State::GetTextureSet(uint8_t* base, const TranslatedShader* shader,
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

VkPipelineLayout Passes::State::GetPipelineLayout(VkDescriptorSetLayout vertex,
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

VkShaderModule Passes::State::GetModule(TranslatedShader* shader) {
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

VkPipeline Passes::State::GetPipeline(const PipelineKey& key) {
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
  multisample.rasterizationSamples = VkSampleCountFlagBits(key.samples);
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
  bool has_color = key.color_format != VK_FORMAT_UNDEFINED;
  blend.attachmentCount = has_color ? 1 : 0;
  blend.pAttachments = &attachment;
  // Depth bias is set per draw.
  raster.depthBiasEnable = key.depth;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
                                     VK_DYNAMIC_STATE_DEPTH_BIAS};
  VkPipelineDynamicStateCreateInfo dynamic{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic.dynamicStateCount = 3;
  dynamic.pDynamicStates = dynamic_states;
  VkFormat color_format = VkFormat(key.color_format);
  VkPipelineRenderingCreateInfo rendering{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
  rendering.colorAttachmentCount = has_color ? 1 : 0;
  rendering.pColorAttachmentFormats = &color_format;
  rendering.depthAttachmentFormat = key.depth ? kDepthFormat : VK_FORMAT_UNDEFINED;
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

bool Passes::State::Reserve(VkDeviceSize size, uint32_t& offset, uint8_t*& data) {
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

void Passes::State::EndRendering(VkCommandBuffer commands) {
  if (rendering) {
    vk::vkCmdEndRendering(commands);
    rendering = false;
  }
}

void Passes::State::BeginRendering(VkCommandBuffer commands, Target* color, Target* depth) {
  if (rendering && bound_color == color && bound_depth == depth) {
    return;
  }
  EndRendering(commands);
  VkRenderingAttachmentInfo color_attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  VkRenderingAttachmentInfo depth_attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  VkExtent2D extent{UINT32_MAX, UINT32_MAX};
  if (color) {
    Transition(commands, color->image.image, color->aspects(), color->layout,
               VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    color_attachment.imageView = color->image.view;
    color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    info.colorAttachmentCount = 1;
    info.pColorAttachments = &color_attachment;
    extent = color->extent;
  }
  if (depth) {
    Transition(commands, depth->image.image, depth->aspects(), depth->layout,
               VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    depth_attachment.imageView = depth->image.view;
    depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    info.pDepthAttachment = &depth_attachment;
    info.pStencilAttachment = &depth_attachment;
    extent = {std::min(extent.width, depth->extent.width),
              std::min(extent.height, depth->extent.height)};
  }
  info.renderArea = {{0, 0}, extent};
  info.layerCount = 1;
  vk::vkCmdBeginRendering(commands, &info);
  bound_color = color;
  bound_depth = depth;
  render_extent = extent;
  rendering = true;
}

Target* Passes::State::GetTarget(VkCommandBuffer commands, uint8_t* base, uint32_t object,
                                 bool depth) {
  Surface surface = ReadSurface(base, object);
  VkFormat format = depth ? kDepthFormat : ColorFormat(surface.format);
  if (format == VK_FORMAT_UNDEFINED || !surface.pitch) {
    return nullptr;
  }
  std::array<uint32_t, 5> key{uint32_t(depth), surface.edram_base, surface.pitch, surface.msaa,
                              uint32_t(format)};
  auto [it, added] = targets.try_emplace(key);
  Target& t = it->second;
  if (!added) {
    return t.image.image ? &t : nullptr;
  }
  t.depth = depth;
  t.surface = surface;
  t.format = format;
  t.samples = SampleCount(surface.msaa);
  t.extent = SurfaceExtent(surface, depth);
  VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                            VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                            (depth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                                   : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
  if (!CreateImage(t.image, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, format,
                   {t.extent.width, t.extent.height, 1}, 1, usage, t.aspects(), 0, t.samples)) {
    REXLOG_ERROR("native renderer: couldn't create a {}x{} target", t.extent.width,
                 t.extent.height);
    t.image = {};
    return nullptr;
  }
  t.sample_view = t.image.view;
  if (depth) {
    VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view.image = t.image.image;
    view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view.format = format;
    view.subresourceRange = {VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1};
    vk::vkCreateImageView(context.device, &view, nullptr, &t.sample_view);
  }
  REXLOG_INFO("native renderer: {} target: EDRAM base {}, pitch {}, MSAA {}, format {}; {}x{}",
              depth ? "depth" : "colour", surface.edram_base, surface.pitch, surface.msaa,
              surface.format, t.extent.width, t.extent.height);
  stats.targets++;
  VkClearValue initial{};
  initial.depthStencil = {1.0f, 0};
  if (!depth) {
    initial.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
  }
  ClearTarget(commands, t, initial);
  return &t;
}

void Passes::State::ClearTarget(VkCommandBuffer commands, Target& target,
                                const VkClearValue& value, VkImageAspectFlags aspects) {
  EndRendering(commands);
  Transition(commands, target.image.image, target.aspects(), target.layout,
             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  VkImageSubresourceRange range{aspects ? aspects : target.aspects(), 0, 1, 0, 1};
  if (target.depth) {
    vk::vkCmdClearDepthStencilImage(commands, target.image.image,
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &value.depthStencil, 1,
                                    &range);
  } else {
    vk::vkCmdClearColorImage(commands, target.image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                             &value.color, 1, &range);
  }
}

bool Passes::State::CreateResolveResources() {
  VkDevice device = context.device;
  VkDescriptorSetLayoutBinding binding{0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
                                       VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
  VkDescriptorSetLayoutCreateInfo set_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  set_info.bindingCount = 1;
  set_info.pBindings = &binding;
  if (vk::vkCreateDescriptorSetLayout(device, &set_info, nullptr, &resolve_set_layout) !=
      VK_SUCCESS) {
    return false;
  }
  VkPushConstantRange push{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 16};
  VkPipelineLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
  layout_info.setLayoutCount = 1;
  layout_info.pSetLayouts = &resolve_set_layout;
  layout_info.pushConstantRangeCount = 1;
  layout_info.pPushConstantRanges = &push;
  if (vk::vkCreatePipelineLayout(device, &layout_info, nullptr, &resolve_layout) != VK_SUCCESS) {
    return false;
  }
  auto module = [&](const uint32_t* code, size_t size, VkShaderModule& out) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = size;
    info.pCode = code;
    return vk::vkCreateShaderModule(device, &info, nullptr, &out) == VK_SUCCESS;
  };
  if (!module(k_resolve_vert, sizeof(k_resolve_vert), resolve_vertex) ||
      !module(k_resolve_frag, sizeof(k_resolve_frag), resolve_pixel) ||
      !module(k_resolve_ms_frag, sizeof(k_resolve_ms_frag), resolve_pixel_ms)) {
    return false;
  }
  VkSamplerCreateInfo sampler_info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  sampler_info.addressModeU = sampler_info.addressModeV = sampler_info.addressModeW =
      VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  if (vk::vkCreateSampler(device, &sampler_info, nullptr, &resolve_sampler) != VK_SUCCESS) {
    return false;
  }
  resolve_pools.resize(frames_in_flight);
  for (VkDescriptorPool& pool : resolve_pools) {
    VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, kResolvesPerFrame};
    VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    pool_info.maxSets = kResolvesPerFrame;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &size;
    if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &pool) != VK_SUCCESS) {
      return false;
    }
  }
  return true;
}

VkPipeline Passes::State::GetResolvePipeline(VkFormat format, bool multisample) {
  auto [it, added] = resolve_pipelines.try_emplace({format, multisample}, VK_NULL_HANDLE);
  if (!added) {
    return it->second;
  }
  VkPipelineShaderStageCreateInfo stages[2] = {};
  for (VkPipelineShaderStageCreateInfo& stage : stages) {
    stage = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    stage.pName = "main";
  }
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = resolve_vertex;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = multisample ? resolve_pixel_ms : resolve_pixel;
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
  multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
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
  info.layout = resolve_layout;
  if (vk::vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &info, nullptr,
                                    &it->second) != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: couldn't create a resolve pipeline for format {}",
                 int(format));
    it->second = VK_NULL_HANDLE;
  }
  return it->second;
}

bool Passes::State::ResolveInto(VkCommandBuffer commands, uint8_t* base, Target& source,
                                uint32_t destination, uint32_t item, uint32_t fields,
                                uint32_t flags) {
  // The destination texture's fetch constant (+28..+48), its addresses
  // made physical as SetTexture makes them.
  uint32_t fetch[6];
  for (uint32_t k = 0; k < 6; k++) {
    fetch[k] = guest::LoadU32(base, destination + 28 + 4 * k);
  }
  fetch[1] = (fetch[1] & 0xFFFu) | GpuPhysicalAddress(fetch[1] & 0xFFFFF000u);
  fetch[5] = (fetch[5] & 0xFFFu) | GpuPhysicalAddress(fetch[5] & 0xFFFFF000u);
  TextureCache::ResolveDestination dest;
  if (!textures.GetResolveDestination(fetch, dest)) {
    return false;
  }
  // Source rectangle (left, top, right, bottom) and destination point.
  int32_t left = 0, top = 0, right = int32_t(dest.width), bottom = int32_t(dest.height);
  if (fields & 1) {
    left = int32_t(guest::LoadU32(base, item + 8));
    top = int32_t(guest::LoadU32(base, item + 12));
    right = int32_t(guest::LoadU32(base, item + 16));
    bottom = int32_t(guest::LoadU32(base, item + 20));
  }
  int32_t x = 0, y = 0;
  if (fields & 4) {
    x = int32_t(guest::LoadU32(base, item + 28));
    y = int32_t(guest::LoadU32(base, item + 32));
  }
  left = std::max(left, 0);
  top = std::max(top, 0);
  right = std::min(right, int32_t(source.extent.width));
  bottom = std::min(bottom, int32_t(source.extent.height));
  int32_t width = std::min(right - left, int32_t(dest.width) - x);
  int32_t height = std::min(bottom - top, int32_t(dest.height) - y);
  uint64_t log_key = uint64_t(reinterpret_cast<uintptr_t>(&source)) * 31 + (fetch[1] >> 12);
  if (logged_resolves.insert(log_key).second) {
    REXLOG_INFO("native renderer: resolve from {} target (EDRAM base {}, pitch {}, MSAA {}, "
                "format {}) rect {},{}-{},{} to {:08X} at {},{}, flags {:08X}",
                source.depth ? "depth" : "colour", source.surface.edram_base,
                source.surface.pitch, source.surface.msaa, source.surface.format, left, top, right,
                bottom, fetch[1] & 0xFFFFF000u, x, y, flags);
  }
  if (width <= 0 || height <= 0 || x < 0 || y < 0) {
    return false;
  }
  VkPipeline pipeline = GetResolvePipeline(dest.format, source.samples != VK_SAMPLE_COUNT_1_BIT);
  if (!pipeline) {
    return false;
  }
  VkDescriptorSet set;
  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  alloc.descriptorPool = resolve_pools[frame];
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &resolve_set_layout;
  if (vk::vkAllocateDescriptorSets(context.device, &alloc, &set) != VK_SUCCESS) {
    return false;
  }
  VkImageLayout read_layout = source.depth ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
                                           : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  VkDescriptorImageInfo image_info{resolve_sampler, source.sample_view, read_layout};
  VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
  write.dstSet = set;
  write.descriptorCount = 1;
  write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  write.pImageInfo = &image_info;
  vk::vkUpdateDescriptorSets(context.device, 1, &write, 0, nullptr);

  EndRendering(commands);
  Transition(commands, source.image.image, source.aspects(), source.layout, read_layout);
  VkImageLayout dest_layout =
      dest.fresh ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  Transition(commands, dest.image, VK_IMAGE_ASPECT_COLOR_BIT, dest_layout,
             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  VkRect2D rect{{x, y}, {uint32_t(width), uint32_t(height)}};
  VkRenderingAttachmentInfo attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  attachment.imageView = dest.view;
  attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  info.renderArea = rect;
  info.layerCount = 1;
  info.colorAttachmentCount = 1;
  info.pColorAttachments = &attachment;
  vk::vkCmdBeginRendering(commands, &info);
  VkViewport viewport{float(x), float(y), float(width), float(height), 0.0f, 1.0f};
  vk::vkCmdSetViewport(commands, 0, 1, &viewport);
  vk::vkCmdSetScissor(commands, 0, 1, &rect);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, resolve_layout, 0, 1,
                              &set, 0, nullptr);
  // The exponent bias: flags bits 26-31, signed (likely; 0xF4000000 is -3
  // on the main view's HDR copies).
  struct {
    int32_t offset[2];
    int32_t samples;
    float scale;
  } constants{{left - x, top - y},
              source.depth ? 1 : int32_t(source.samples),
              std::ldexp(1.0f, int32_t(flags) >> 26)};
  vk::vkCmdPushConstants(commands, resolve_layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(constants), &constants);
  vk::vkCmdDraw(commands, 3, 1, 0, 0);
  vk::vkCmdEndRendering(commands);
  Transition(commands, dest.image, VK_IMAGE_ASPECT_COLOR_BIT, dest_layout,
             VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  frame_resolves.push_back({dest.image, {dest.width, dest.height}, fetch[1] & 0xFFFFF000u,
                            fetch[1] & 0x3F, flags, dest.format});
  return true;
}

Passes::Passes() : state_(std::make_unique<State>()) {}
Passes::~Passes() = default;

bool Passes::Init(const DeviceContext& context, uint8_t* base, uint32_t frames_in_flight) {
  State& s = *state_;
  s.context = context;
  s.frames_in_flight = frames_in_flight;
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &s.memory_properties);
  if (!s.CreateGuestMemoryMirror()) {
    return false;
  }
  if (!s.CreateImage(s.shown, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, kShownFormat,
                     {kShownWidth, kShownHeight, 1}, 1,
                     VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                     VK_IMAGE_ASPECT_COLOR_BIT) ||
      !s.CreateRings() || !s.CreateDescriptors() || !s.CreateDummyTextures() ||
      !s.CreateResolveResources() || !s.textures.Init(context, frames_in_flight)) {
    REXLOG_ERROR("native renderer: couldn't create the passes' resources");
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

void Passes::BeginFrame(uint32_t frame_index) {
  State& s = *state_;
  s.frame = frame_index;
  s.ring_offset = 0;
  s.rendering = false;
  s.bound_color = s.bound_depth = nullptr;
  s.main_view = nullptr;
  s.frame_resolves.clear();
  s.mirrored.clear();
  s.mirrored_hashes.clear();
  s.frame_pixel_shaders.clear();
  s.texture_sets.clear();
  vk::vkResetDescriptorPool(s.context.device, s.rings[frame_index].texture_pool, 0);
  vk::vkResetDescriptorPool(s.context.device, s.resolve_pools[frame_index], 0);
  s.textures.BeginFrame(frame_index);
}

VkCommandBuffer Passes::EndUploads() {
  return state_->textures.EndUploads();
}

void Passes::State::Draw(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                         uint32_t item) {
  State& s = *this;
  auto& regs = s.regs;
  // The bound surfaces: render target 0 and depth (others aren't drawn).
  uint32_t color_object = guest::LoadU32(base, device + kDeviceRenderTarget0);
  uint32_t depth_object = guest::LoadU32(base, device + kDeviceDepthStencil);
  for (uint32_t i = 1; i < 4; i++) {
    // The same surface in another slot (seen: target 1 = target 0) adds
    // nothing.
    if (uint32_t object = guest::LoadU32(base, device + kDeviceRenderTarget0 + 4 * i);
        object && object != color_object) {
      if (!s.logged_extra_targets) {
        s.logged_extra_targets = true;
        Surface extra = ReadSurface(base, object);
        Surface first = color_object ? ReadSurface(base, color_object) : Surface{};
        REXLOG_INFO("native renderer: render target {} bound ({:08X}: EDRAM base {}, pitch {}, "
                    "MSAA {}, format {}) with target 0 {:08X} (EDRAM base {}, pitch {}, MSAA {}, "
                    "format {})",
                    i, object, extra.edram_base, extra.pitch, extra.msaa, extra.format,
                    color_object, first.edram_base, first.pitch, first.msaa, first.format);
      }
      s.stats.extra_targets++;
      break;
    }
  }
  Target* color = color_object ? s.GetTarget(commands, base, color_object, false) : nullptr;
  Target* depth = depth_object ? s.GetTarget(commands, base, depth_object, true) : nullptr;
  if (!color && !depth) {
    s.stats.failed++;
    return;
  }
  if (color && depth && color->samples != depth->samples) {
    s.stats.mismatched_samples++;
    return;
  }
  // The area both can be drawn in.
  VkExtent2D extent = color ? color->extent : depth->extent;
  if (color && depth) {
    extent = {std::min(color->extent.width, depth->extent.width),
              std::min(color->extent.height, depth->extent.height)};
  }

  // Command 18's item: [primitive type, base vertex << 16 | start index,
  // primitive count].
  uint32_t type = guest::LoadU32(base, item);
  uint32_t base_start = guest::LoadU32(base, item + 4);
  uint32_t primitive_count = guest::LoadU32(base, item + 8);
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
  const uint8_t* vertex_ucode =
      PhysicalHostAddress(base, GpuPhysicalAddress(guest::LoadU32(base, vertex_object + 48)));
  uint32_t vertex_size = guest::LoadU32(base, vertex_object + 600);
  const uint8_t* pixel_ucode =
      pixel_object
          ? PhysicalHostAddress(base, GpuPhysicalAddress(guest::LoadU32(base, pixel_object + 24)))
          : nullptr;
  uint32_t pixel_size = pixel_object ? guest::LoadU32(base, pixel_object + 64) : 0;
  // Which interpolators connect them (an unconnected one the pixel shader
  // reads would be undefined).
  ShaderCache::Interface interface = s.shaders->GetInterface(
      vertex_ucode, vertex_size, pixel_ucode, pixel_size, program_control,
      regs.values[XE_GPU_REG_SQ_CONTEXT_MISC]);
  interface.param_gen_point = type == 1;  // point list
  TranslatedShader* vertex = s.shaders->Get(ShaderStage::kVertex, vertex_ucode, vertex_size,
                                            program_control & 0x3F, interface);
  TranslatedShader* pixel = nullptr;
  if (pixel_object) {
    pixel = s.shaders->Get(ShaderStage::kPixel, pixel_ucode, pixel_size,
                           (program_control >> 8) & 0x3F, interface);
  }
  if (!vertex || (pixel_object && !pixel)) {
    s.stats.failed++;
    return;
  }
  bool main_view_draw = color && color->surface.pitch == kMainViewPitch &&
                        color->samples == VK_SAMPLE_COUNT_2_BIT;
  if (pixel) {
    if (main_view_draw && std::find(frame_pixel_shaders.begin(), frame_pixel_shaders.end(),
                                    pixel->hash) == frame_pixel_shaders.end()) {
      frame_pixel_shaders.push_back(pixel->hash);
    }
    if (pixel->hash == skip_pixel_shader) {
      return;  // debugging aid
    }
  }
  if (record_draws) {
    // Raw, to keep recording at its usual speed (the effect being looked for
    // depends on timing); formatted with the dump.
    RecordedDraw& r = recorded_draws.emplace_back();
    r.main_view = main_view_draw;
    r.surface = color ? color->surface : depth->surface;
    r.has_color = color != nullptr;
    r.has_depth = depth != nullptr;
    r.vertex = vertex->hash;
    r.pixel = pixel ? pixel->hash : 0;
    r.type = type;
    r.count = primitive_count;
    for (const TranslatedShader* shader : {vertex, pixel}) {
      for (size_t i = 0; shader && i < shader->textures.size(); i++) {
        const ShaderTexture& texture = shader->textures[i];
        RecordedDraw::Texture& t = r.textures.emplace_back();
        t.pixel = shader == pixel;
        t.texture = texture;
        std::memcpy(t.fetch,
                    &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 6 * texture.fetch_constant],
                    sizeof(t.fetch));
      }
    }
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
  draw_util::Scissor scissor{{0, 0}, {extent.width, extent.height}};
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
  uint32_t scissor_right = std::min(scissor.offset[0] + scissor.extent[0], extent.width);
  uint32_t scissor_bottom = std::min(scissor.offset[1] + scissor.extent[1], extent.height);
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
  if (depth) {
    key.z_enable = depth_control.z_enable;
    key.z_write = depth_control.z_write_enable;
    key.z_func = uint32_t(depth_control.zfunc);
  }
  key.color_format = color ? color->format : VK_FORMAT_UNDEFINED;
  key.depth = depth != nullptr;
  key.samples = color ? color->samples : depth->samples;
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
  s.Mirror(base, index_address, index_count * (index_32bit ? 4 : 2), true);
  for (uint32_t k = 0; k < 96; k++) {
    if (!(vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31)))) {
      continue;
    }
    uint32_t word0 = regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k];
    uint32_t word1 = regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k + 1];
    s.Mirror(base, word0 & 0x1FFFFFFCu, ((word1 >> 2) & 0xFFFFFFu) * 4, false);
  }

  // Draw.
  s.BeginRendering(commands, color, depth);
  if (color && color->surface.pitch == kMainViewPitch && color->samples == VK_SAMPLE_COUNT_2_BIT &&
      color->format == VK_FORMAT_R16G16B16A16_SFLOAT) {
    s.main_view = color;
  }
  VkViewport viewport{float(viewport_info.xy_offset[0]), float(viewport_info.xy_offset[1]),
                      float(viewport_info.xy_extent[0]), float(viewport_info.xy_extent[1]),
                      viewport_info.z_min, viewport_info.z_max};
  VkRect2D scissor_rect{{int32_t(scissor.offset[0]), int32_t(scissor.offset[1])},
                        {scissor_right - scissor.offset[0], scissor_bottom - scissor.offset[1]}};
  vk::vkCmdSetViewport(commands, 0, 1, &viewport);
  vk::vkCmdSetScissor(commands, 0, 1, &scissor_rect);
  // Set for every pipeline (it's dynamic in all of them), used with depth.
  float slope = 0.0f, constant = 0.0f;
  if (depth) {
    // As the plugin sets it for its host render targets: the guest's depth
    // is float24 (24_8_FLOAT) or unorm24, the slope in 1/16 pixels.
    draw_util::GetPreferredFacePolygonOffset(regs, primitive.polygonal, slope, constant);
    constant *= depth->surface.format == uint32_t(xenos::DepthRenderTargetFormat::kD24S8)
                    ? draw_util::kD3D10PolygonOffsetFactorUnorm24
                    : draw_util::kD3D10PolygonOffsetFactorFloat24;
    slope *= xenos::kPolygonOffsetScaleSubpixelUnit;
    if (constant != 0.0f || slope != 0.0f) {
      s.stats.depth_biased++;
    }
    if (key.z_enable && key.z_func == VK_COMPARE_OP_EQUAL) {
      s.stats.depth_equal++;
    }
  }
  vk::vkCmdSetDepthBias(commands, constant, 0.0f, slope);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  VkDescriptorSet sets[4] = {s.mirrors[s.frame].set, s.rings[s.frame].constants_set,
                             vertex_textures, pixel_textures};
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, key.layout, 0, 4, sets,
                              5, offsets);
  vk::vkCmdBindIndexBuffer(commands, s.mirrors[s.frame].buffer, index_address,
                           index_32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16);
  vk::vkCmdDrawIndexed(commands, index_count, 1, 0, 0, 0);
  s.stats.drawn++;
}

void Passes::AfterDraw(VkCommandBuffer commands, uint8_t* base, uint32_t id, uint32_t payload,
                       uint32_t arg1) {
  State& s = *state_;
  // Only indexed draws for now (command 18, 12 bytes an item).
  if (id != 18) {
    s.stats.unsupported += arg1;
    return;
  }
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  ReadRegisters(base, device, s.regs);
  for (uint32_t i = 0; i < arg1; i++) {
    s.Draw(commands, base, device, payload + 12 * i);
  }
}

void Passes::AfterClear(VkCommandBuffer commands, uint8_t* base, uint32_t payload,
                        uint32_t arg1) {
  State& s = *state_;
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  // Command 26's item (280 bytes): +4 rect count (rects from +12), +8 flags
  // (bits 0-3 render targets 0-3, 0x10 Z, 0x20 stencil), +268 colour
  // (D3DCOLOR), +272 Z, +276 stencil. Rects aren't kept: the whole target
  // is cleared.
  for (uint32_t i = 0; i < arg1; i++) {
    uint32_t item = payload + 280 * i;
    uint32_t flags = guest::LoadU32(base, item + 8);
    s.stats.clears++;
    if (guest::LoadU32(base, item + 4)) {
      s.stats.clear_rects++;
    }
    if (flags & 0xF) {
      uint32_t color = guest::LoadU32(base, item + 268);
      VkClearValue value{};
      value.color = {{float((color >> 16) & 0xFF) / 255.0f, float((color >> 8) & 0xFF) / 255.0f,
                      float(color & 0xFF) / 255.0f, float(color >> 24) / 255.0f}};
      if (flags & 0xE) {
        s.stats.extra_targets++;
      }
      if (uint32_t object = guest::LoadU32(base, device + kDeviceRenderTarget0); object && (flags & 1)) {
        if (Target* target = s.GetTarget(commands, base, object, false)) {
          s.ClearTarget(commands, *target, value);
        }
      }
    }
    if (flags & 0x30) {
      uint32_t object = guest::LoadU32(base, device + kDeviceDepthStencil);
      Target* target = object ? s.GetTarget(commands, base, object, true) : nullptr;
      if (target) {
        VkClearValue value{};
        uint32_t z = guest::LoadU32(base, item + 272);
        std::memcpy(&value.depthStencil.depth, &z, 4);
        value.depthStencil.stencil = guest::LoadU32(base, item + 276) & 0xFF;
        s.ClearTarget(commands, *target, value,
                      ((flags & 0x10) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) |
                          ((flags & 0x20) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0));
      }
    }
  }
}

void Passes::AfterResolve(VkCommandBuffer commands, uint8_t* base, uint32_t payload,
                          uint32_t arg1) {
  State& s = *state_;
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  // Command 25's item (68 bytes; see "Pass command items" in
  // docs/gpu-functions.md). Flags as the XDK's D3DRESOLVE ones, likely:
  // bits 0-1 render target, 0x4 depth instead, 0x100 then clear the render
  // target, 0x200 clear depth and stencil, bits 26-31 exponent bias.
  constexpr uint32_t kKnownFlags = 0xFC000000u | 0x300 | 0x7;
  for (uint32_t i = 0; i < arg1; i++) {
    uint32_t item = payload + 68 * i;
    uint32_t fields = guest::LoadU32(base, item);
    uint32_t flags = guest::LoadU32(base, item + 4);
    if (flags & ~kKnownFlags) {
      s.stats.resolve_unknown_flags++;
      s.stats.resolve_flags_sample = flags;
    }
    bool from_depth = flags & 4;
    uint32_t color_object = guest::LoadU32(base, device + kDeviceRenderTarget0 + 4 * (flags & 3));
    uint32_t depth_object = guest::LoadU32(base, device + kDeviceDepthStencil);
    uint32_t source_object = from_depth ? depth_object : color_object;
    Target* source =
        source_object ? s.GetTarget(commands, base, source_object, from_depth) : nullptr;
    uint32_t destination = (fields & 2) ? guest::LoadU32(base, item + 24) : 0;
    bool level_or_slice = guest::LoadU32(base, item + 36) || guest::LoadU32(base, item + 40);
    if (source && destination && !level_or_slice &&
        s.ResolveInto(commands, base, *source, destination, item, fields, flags)) {
      s.stats.resolves++;
    } else {
      s.stats.resolves_skipped++;
    }
    if ((flags & 0x100) && color_object) {
      if (Target* target = s.GetTarget(commands, base, color_object, false)) {
        VkClearValue value{};
        if (fields & 8) {
          for (uint32_t c = 0; c < 4; c++) {
            value.color.float32[c] = float(guest::LoadU8(base, item + 44 + c)) / 255.0f;
          }
        }
        s.ClearTarget(commands, *target, value);
      }
    }
    if ((flags & 0x200) && depth_object) {
      if (Target* target = s.GetTarget(commands, base, depth_object, true)) {
        VkClearValue value{};
        uint32_t z = guest::LoadU32(base, item + 48);
        std::memcpy(&value.depthStencil.depth, &z, 4);
        value.depthStencil.stencil = guest::LoadU32(base, item + 52) & 0xFF;
        s.ClearTarget(commands, *target, value);
      }
    }
  }
}

void Passes::set_skip_pixel_shader(uint64_t hash) {
  state_->skip_pixel_shader = hash;
}

void Passes::set_record_draws(bool record) {
  state_->record_draws = record;
}

std::vector<uint64_t> Passes::main_view_pixel_shaders() const {
  return state_->last_pixel_shaders;
}

void Passes::set_check_mirror_frames(bool check) {
  state_->check_mirror_frames = check;
}

std::string Passes::TakeMirrorChanges() {
  State& s = *state_;
  // The ranges that changed, the most changed first.
  std::vector<std::pair<uint64_t, State::RangeHistory>> changed;
  uint64_t bytes = 0;
  for (auto& [key, history] : s.range_history) {
    if (history.changes) {
      changed.emplace_back(key, history);
      bytes += key & 0xFFFFFFFF;
      history.changes = 0;
    }
  }
  std::sort(changed.begin(), changed.end(),
            [](const auto& a, const auto& b) { return a.second.changes > b.second.changes; });
  std::string text = fmt::format("{} of {} ranges changed ({:.1f} MB)", changed.size(),
                                 s.range_history.size(), bytes / 1048576.0);
  for (size_t i = 0; i < changed.size() && i < 12; i++) {
    text += fmt::format("; {} {:08X} {} bytes x{}", changed[i].second.index ? "index" : "vertex",
                        uint32_t(changed[i].first >> 32), uint32_t(changed[i].first),
                        changed[i].second.changes);
  }
  return text;
}

void Passes::set_check_mirror(bool check) {
  state_->check_mirror = check;
}

void Passes::set_debug_hide(TextureCache::Hide hide, uint32_t value) {
  state_->textures.set_debug_hide(hide, value);
}

std::vector<uint32_t> Passes::resolve_addresses() const {
  std::vector<uint32_t> addresses;
  for (const State::Resolved& r : state_->frame_resolves) {
    if (std::find(addresses.begin(), addresses.end(), r.address) == addresses.end()) {
      addresses.push_back(r.address);
    }
  }
  return addresses;
}

VkExtent2D Passes::EndFrame(VkCommandBuffer commands, int32_t show_resolve, bool dump,
                            const std::string& dump_prefix, bool dump_resolves) {
  State& s = *state_;
  s.EndRendering(commands);
  s.last_frame_resolves = s.frame_resolves.size();
  s.last_pixel_shaders = s.frame_pixel_shaders;
  // The frame's resolves, now and then, to pick one to show.
  if (s.frames++ % 1200 == 600 && !s.frame_resolves.empty()) {
    std::string list;
    for (size_t i = 0; i < s.frame_resolves.size(); i++) {
      const State::Resolved& r = s.frame_resolves[i];
      list += fmt::format("{}{}: {:08X} format {} {}x{} flags {:X}", i ? "; " : "", i, r.address,
                          r.format, r.extent.width, r.extent.height, r.flags);
    }
    REXLOG_INFO("native renderer: this frame's resolves: {}", list);
  }
  VkImage shown = s.shown.image;
  Transition(commands, shown, VK_IMAGE_ASPECT_COLOR_BIT, s.shown_layout,
             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  VkExtent2D extent{kMainViewWidth, kMainViewHeight};
  if (show_resolve >= 0 && size_t(show_resolve) < s.frame_resolves.size()) {
    // A resolve destination, as is.
    const State::Resolved& resolved = s.frame_resolves[show_resolve];
    extent = {std::min(resolved.extent.width, kShownWidth),
              std::min(resolved.extent.height, kShownHeight)};
    VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    Transition(commands, resolved.image, VK_IMAGE_ASPECT_COLOR_BIT, layout,
               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    VkImageBlit blit{};
    blit.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    blit.srcOffsets[1] = {int32_t(extent.width), int32_t(extent.height), 1};
    blit.dstSubresource = blit.srcSubresource;
    blit.dstOffsets[1] = blit.srcOffsets[1];
    vk::vkCmdBlitImage(commands, resolved.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, shown,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_NEAREST);
    Transition(commands, resolved.image, VK_IMAGE_ASPECT_COLOR_BIT, layout,
               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  } else if (Target* main = s.main_view) {
    // The main view's drawn part, its samples averaged.
    Transition(commands, main->image.image, main->aspects(), main->layout,
               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    extent = {std::min(kMainViewWidth, main->extent.width),
              std::min(kMainViewHeight, main->extent.height)};
    VkImageResolve region{};
    region.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    region.dstSubresource = region.srcSubresource;
    region.extent = {extent.width, extent.height, 1};
    vk::vkCmdResolveImage(commands, main->image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, shown,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  } else {
    // No main view this frame (menus): dark blue.
    VkClearColorValue colour{{0.0f, 0.05f, 0.15f, 1.0f}};
    VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vk::vkCmdClearColorImage(commands, shown, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &colour, 1,
                             &range);
  }
  Transition(commands, shown, VK_IMAGE_ASPECT_COLOR_BIT, s.shown_layout,
             VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  if (dump) {
    s.RecordDump(commands, extent, dump_prefix, dump_resolves);
  }
  return extent;
}

size_t Passes::frame_resolve_count() const {
  return state_->last_frame_resolves;
}

namespace {

uint32_t BytesPerPixel(VkFormat format) {
  switch (format) {
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32_SFLOAT:
      return 8;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
      return 16;
    case VK_FORMAT_R8_UNORM:
      return 1;
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SFLOAT:
      return 2;
    default:
      return 4;
  }
}

float HalfToFloat(uint16_t h) {
  uint32_t sign = uint32_t(h >> 15) << 31, exponent = (h >> 10) & 0x1F, mantissa = h & 0x3FF;
  float value;
  if (exponent == 0) {
    value = std::ldexp(float(mantissa), -24);
  } else if (exponent == 31) {
    value = mantissa ? NAN : INFINITY;
  } else {
    value = std::ldexp(float(mantissa | 0x400), int(exponent) - 25);
  }
  return sign ? -value : value;
}

// A pixel as RGB floats (missing components 0).
void ToRgb(VkFormat format, const uint8_t* p, float out[3]) {
  out[0] = out[1] = out[2] = 0.0f;
  uint16_t h[4];
  uint32_t w;
  float f;
  switch (format) {
    case VK_FORMAT_R8G8B8A8_UNORM:
      for (int c = 0; c < 3; c++) out[c] = p[c] / 255.0f;
      break;
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
      std::memcpy(&w, p, 4);
      for (int c = 0; c < 3; c++) out[c] = float((w >> (10 * c)) & 0x3FF) / 1023.0f;
      break;
    case VK_FORMAT_R16G16B16A16_UNORM:
      std::memcpy(h, p, 8);
      for (int c = 0; c < 3; c++) out[c] = h[c] / 65535.0f;
      break;
    case VK_FORMAT_R16G16_UNORM:
      std::memcpy(h, p, 4);
      for (int c = 0; c < 2; c++) out[c] = h[c] / 65535.0f;
      break;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
      std::memcpy(h, p, 8);
      for (int c = 0; c < 3; c++) out[c] = HalfToFloat(h[c]);
      break;
    case VK_FORMAT_R16G16_SFLOAT:
      std::memcpy(h, p, 4);
      for (int c = 0; c < 2; c++) out[c] = HalfToFloat(h[c]);
      break;
    case VK_FORMAT_R32_SFLOAT:
      std::memcpy(&f, p, 4);
      out[0] = out[1] = out[2] = f;
      break;
    default:
      break;
  }
}

}  // namespace

void Passes::State::RecordDump(VkCommandBuffer commands, VkExtent2D shown_extent,
                               const std::string& prefix, bool with_resolves) {
  dumped.clear();
  VkDeviceSize size = 0;
  auto add = [&](std::string name, VkFormat format, VkExtent2D extent) {
    dumped.push_back({std::move(name), format, extent, size});
    size += (VkDeviceSize(extent.width) * extent.height * BytesPerPixel(format) + 15) & ~15ull;
  };
  add(prefix, kShownFormat, shown_extent);
  for (size_t i = 0; with_resolves && i < frame_resolves.size(); i++) {
    const Resolved& r = frame_resolves[i];
    add(fmt::format("resolve_{:02}_{:08X}_format{}", i, r.address, r.format), r.host_format,
        r.extent);
  }
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = size;
  info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  VkMemoryRequirements requirements;
  if (vk::vkCreateBuffer(context.device, &info, nullptr, &dump_buffer) != VK_SUCCESS) {
    dumped.clear();
    return;
  }
  vk::vkGetBufferMemoryRequirements(context.device, dump_buffer, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex =
      FindMemoryType(memory_properties, requirements.memoryTypeBits,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (vk::vkAllocateMemory(context.device, &alloc, nullptr, &dump_memory) != VK_SUCCESS ||
      vk::vkBindBufferMemory(context.device, dump_buffer, dump_memory, 0) != VK_SUCCESS) {
    dumped.clear();
    return;
  }
  for (size_t i = 0; i < dumped.size(); i++) {
    VkImage image = i == 0 ? shown.image : frame_resolves[i - 1].image;
    VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    if (i) {
      Transition(commands, image, VK_IMAGE_ASPECT_COLOR_BIT, layout,
                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    }
    VkBufferImageCopy copy{};
    copy.bufferOffset = dumped[i].offset;
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {dumped[i].extent.width, dumped[i].extent.height, 1};
    vk::vkCmdCopyImageToBuffer(commands, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dump_buffer,
                               1, &copy);
    if (i) {
      Transition(commands, image, VK_IMAGE_ASPECT_COLOR_BIT, layout,
                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
  }
}

void Passes::WriteDump(const std::string& directory) {
  State& s = *state_;
  if (s.dumped.empty()) {
    return;
  }
  uint8_t* data = nullptr;
  vk::vkMapMemory(s.context.device, s.dump_memory, 0, VK_WHOLE_SIZE, 0,
                  reinterpret_cast<void**>(&data));
  for (const State::Dumped& d : s.dumped) {
    std::string path = directory + "/" + d.name + ".pfm";
    FILE* file = std::fopen(path.c_str(), "wb");
    if (!file) {
      REXLOG_ERROR("native renderer: couldn't write {}", path);
      continue;
    }
    // PFM: rows bottom to top, little-endian floats.
    std::fprintf(file, "PF\n%u %u\n-1.0\n", d.extent.width, d.extent.height);
    uint32_t bpp = BytesPerPixel(d.format);
    std::vector<float> row(size_t(d.extent.width) * 3);
    for (uint32_t y = d.extent.height; y-- > 0;) {
      for (uint32_t x = 0; x < d.extent.width; x++) {
        ToRgb(d.format, data + d.offset + (size_t(y) * d.extent.width + x) * bpp, &row[3 * x]);
      }
      std::fwrite(row.data(), sizeof(float), row.size(), file);
    }
    std::fclose(file);
  }
  REXLOG_INFO("native renderer: dumped {} images into {}", s.dumped.size(), directory);
  if (!s.recorded_draws.empty()) {
    if (FILE* file = std::fopen((directory + "/draws.txt").c_str(), "w")) {
      for (size_t n = 0; n < s.recorded_draws.size(); n++) {
        const State::RecordedDraw& r = s.recorded_draws[n];
        std::string line = fmt::format(
            "draw {}: {} target pitch {} MSAA {} format {}, colour {}, depth {}; vs {:016X} ps "
            "{:016X}; primitive {} x{}",
            n, r.main_view ? "main view" : "other", r.surface.pitch, r.surface.msaa,
            r.surface.format, r.has_color, r.has_depth, r.vertex, r.pixel, r.type, r.count);
        for (const State::RecordedDraw::Texture& t : r.textures) {
          const uint32_t* f = t.fetch;
          line += fmt::format(
              "; {} tex {}{}: {:08X} format {} {}x{} dim {} declared {} words {:08X} {:08X} "
              "{:08X} {:08X} {:08X} {:08X}",
              t.pixel ? "ps" : "vs", t.texture.fetch_constant, t.texture.is_signed ? "s" : "",
              f[1] & 0xFFFFF000u, f[1] & 0x3F, (f[2] & 0x1FFF) + 1, ((f[2] >> 13) & 0x1FFF) + 1,
              (f[5] >> 9) & 3, int(t.texture.view_type), f[0], f[1], f[2], f[3], f[4], f[5]);
        }
        std::fprintf(file, "%s\n", line.c_str());
      }
      std::fclose(file);
    }
    s.recorded_draws.clear();
  }
  vk::vkDestroyBuffer(s.context.device, s.dump_buffer, nullptr);
  vk::vkFreeMemory(s.context.device, s.dump_memory, nullptr);
  s.dump_buffer = VK_NULL_HANDLE;
  s.dump_memory = VK_NULL_HANDLE;
  s.dumped.clear();
}

VkImage Passes::shown() const {
  return state_->shown.image;
}

Passes::Stats Passes::TakeStats() {
  Stats stats = state_->stats;
  stats.textures = state_->textures.TakeStats();
  stats.shaders_translated = state_->shaders->translated();
  stats.shaders_failed = state_->shaders->failed();
  uint32_t pipelines = state_->stats.pipelines, targets = state_->stats.targets;
  state_->stats = {};
  state_->stats.pipelines = pipelines;
  state_->stats.targets = targets;
  return stats;
}

}  // namespace native
