#include <format>
#include "passes.h"
#include <cstdlib>
#include "vertex_arena.h"
#include "shader_ports.h"
#include "graphics/shader_patch.h"
#include "render/state.h"
#include "native/vulkan_types.h"
#include "gpu/vulkan/backend.h"
#include "frame_costs.h"
#include "base/log.h"
#include "disk_cache.h"

#include "generated_gamma_comp.h"
#include "generated_id_frag.h"
#include "xbox_gamma_table.h"
#include "generated_movie_frag.h"
#include "generated_resolve_frag.h"
#include "generated_resolve_ms_frag.h"
#include "generated_resolve_vert.h"

#include "buffers.h"
#include "gpu_memory.h"
#include "guest.h"
#include "renderer.h"
#include "shaders.h"
#include "state.h"
#include "draw_registers.h"
#include "draw_constants.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "xenos_types.h"
#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"

ITC_FLAG_DECLARE(std::string, log_level);

ITC_FLAG_STRING(xbox_gamma, "off", "ITC",
                "Show the frame through the display gamma table the game sets for a kind "
                "of display, as the Xbox 360's video output does (native renderer): off "
                "(a PC monitor, sRGB: no change), hdtv (darker mid-tones and shadows), "
                "power (lighter shadows)");

namespace native {


namespace {

// Every depth surface (24_8 and 24_8_FLOAT) is drawn as this.
constexpr VkFormat kDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

// The main view, which is shown: its surface (pitch 1040, 2x MSAA, 7e3
// color, drawn as RGBA16F) and the part of it drawn (its viewport).
constexpr uint32_t kMainViewPitch = 1040;
// The glow's history cross-fade: blends last frame's glow (a 256x144
// texture) over this frame's downsample (see "The bloom" in
// docs/native-renderer.md's known issues).
constexpr uint64_t kGlowCrossFade = 0xE0D070FB58A4EA03ull;
constexpr uint32_t kMainViewWidth = 1024, kMainViewHeight = 576;
// What's shown is copied into an image of this size.
constexpr uint32_t kShownWidth = 1280, kShownHeight = 720;
constexpr VkFormat kShownFormat = VK_FORMAT_R16G16B16A16_SFLOAT;

// EDRAM: 2048 tiles of 80x16 samples at 32 bits a sample.

// Device fields: the bound surfaces (render targets 0-3, then depth).
constexpr uint32_t kDeviceDepthStencil = 12200;

// A surface object's words (see "Resources" in docs/gpu-functions.md):
// +24 RB_SURFACE_INFO (pitch in bits 0-13, MSAA in bits 16-17), +28
// RB_COLOR_INFO or RB_DEPTH_INFO (EDRAM base in bits 0-11, format in bits
// 16-19).
// A render target as the game made it (graphics::CreateSurface): +24 the
// pitch and samples, +28 the format (and its EDRAM base, which we don't
// use: docs/pc-renderer.md, step 4), +36 the size (width - 1 from bit 18,
// height - 1 from bit 3).
struct Surface {
  uint32_t pitch, msaa, edram_base, format, width, height;
};
Surface ReadSurface(uint32_t object) {
  uint32_t surface_info = guest::LoadU32(object + 24);
  uint32_t info = guest::LoadU32(object + 28);
  uint32_t size = guest::LoadU32(object + 36);
  return {surface_info & 0x3FFF, (surface_info >> 16) & 3, info & 0xFFF, (info >> 16) & 0xF,
          (size >> 18) + 1, ((size >> 3) & 0x7FFF) + 1};
}

// How a color format is drawn. 7e3 and its high-precision blending
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

// The components a color format has (0 for others).
uint32_t FormatComponents(VkFormat format) {
  switch (format) {
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R16_SFLOAT:
    case VK_FORMAT_R32_SFLOAT:
      return 1;
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R16G16_SFLOAT:
    case VK_FORMAT_R32G32_SFLOAT:
      return 2;
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_R16G16B16A16_UNORM:
    case VK_FORMAT_R16G16B16A16_SFLOAT:
    case VK_FORMAT_R32G32B32A32_SFLOAT:
      return 4;
    default:
      return 0;
  }
}

bool IsUnorm(VkFormat format) {
  switch (format) {
    case VK_FORMAT_R8_UNORM:
    case VK_FORMAT_R16_UNORM:
    case VK_FORMAT_R8G8_UNORM:
    case VK_FORMAT_R16G16_UNORM:
    case VK_FORMAT_R8G8B8A8_UNORM:
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
    case VK_FORMAT_R16G16B16A16_UNORM:
      return true;
    default:
      return false;
  }
}

VkSampleCountFlagBits SampleCount(uint32_t msaa) {
  return msaa == 2 ? VK_SAMPLE_COUNT_4_BIT : msaa == 1 ? VK_SAMPLE_COUNT_2_BIT
                                                       : VK_SAMPLE_COUNT_1_BIT;
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

// Per frame in flight: the draws' constants, in blocks of this size (about
// 1,750 draws each, fewer reused); a frame that fills its blocks gets
// another, kept for later frames. Host-visible, so pinned system memory on
// a discrete GPU: 16 MB since docs/pc-renderer.md (64 before).
constexpr VkDeviceSize kConstantBlockSize = 16u << 20;
// Fixed descriptor ranges of the constant buffers (set 1), so one set per
// frame serves every draw through dynamic offsets.
constexpr uint32_t kFloatConstantsRange = 256 * 16;
constexpr uint32_t kBoolLoopConstantsRange = sizeof(render::BoolLoopConstants);
// The constant blocks: system, vertex floats, pixel floats, bool/loop.
constexpr uint32_t kConstantBlocks = 4;
// The fetch constants the draw reads (textures 0-31, 6 words each; vertex
// fetch constants 2 words each, the same space).
constexpr uint32_t kFetchConstantsBytes = 32 * 6 * 4;

// The built-in index lists (rectangle and quad lists) live in the vertex
// arena (vertex_arena.h), with the frame's inline vertices.
constexpr uint32_t kQuadIndices = VertexArena::kQuadIndices;

// An inline draw's vertices: vertex fetch constant 95 (stream 0), as the
// library's BeginVertices (sub_825BD8F0) writes it into the command buffer
// instead of the shadow, with 93 and 94 as [1, 0] (the same register write)
// and VGT_INDX_OFFSET 0; endianness 8-in-32.
constexpr uint32_t kInlineFetchConstant = 95;

// A vertex shader's vertex input (docs/pc-renderer.md, step 2): a fetch's
// attributes at locations 3n to 3n + 2 (xe_interface.glsl's XE_VF_IN), so
// up to 8 fetches; a rectangle list's shaders (the screen quads') have at
// most 4, three bindings each.
constexpr uint32_t kMaxVertexFetches = 8;
constexpr uint32_t kMaxVertexBindings = 3 * kMaxVertexFetches;

// The attribute format holding a fetch's raw words.
VkFormat RawWordsFormat(uint32_t words) {
  switch (words) {
    case 1:
      return VK_FORMAT_R32_UINT;
    case 2:
      return VK_FORMAT_R32G32_UINT;
    case 3:
      return VK_FORMAT_R32G32B32_UINT;
    default:
      return VK_FORMAT_R32G32B32A32_UINT;
  }
}

// A draw: command 18's item (an indexed draw) or an inline one.
struct DrawCall {
  uint32_t type;
  // Command 18: the item's base vertex << 16 | start index, and its
  // primitive count.
  uint32_t base_start = 0, primitive_count = 0;
  // Inline vertices: their count, and their offset in the vertex arena.
  bool inline_vertices = false;
  uint32_t vertex_count = 0, vertex_address = 0;
  uint32_t inline_stride = 0;  // bytes
};

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
    case 8:  // rectangle list: inline draws only, as two-triangle strips
      out = {VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, 3, 0, false, true};
      return true;
    case 13:  // quad list: inline draws only, as a triangle list
      out = {VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 4, 0, false, true};
      return true;
    default:
      return false;
  }
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
  // cull_mode, z_func and the blend fields are gpu:: values (src/gpu/types.h).
  // The targets: color format (UNDEFINED for none), depth or not, samples.
  uint32_t color_format, depth, samples;

  bool operator==(const PipelineKey&) const = default;
};
// Hashed and compared as a whole (no padding to hold stray bytes).
static_assert(sizeof(PipelineKey) == 3 * sizeof(void*) + 18 * sizeof(uint32_t));
struct PipelineKeyHash {
  size_t operator()(const PipelineKey& key) const {
    return size_t(base::Hash64(&key, sizeof(key)));
  }
};
struct HandlePairHash {
  size_t operator()(const std::pair<VkDescriptorSetLayout, VkDescriptorSetLayout>& key) const {
    return std::hash<const void*>()(key.first) ^
           (std::hash<const void*>()(key.second) * 0x9E3779B97F4A7C15ull);
  }
};

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
  VkExtent2D guest_extent;  // the surface's, in the guest's pixels
  VkExtent2D extent;        // the image's: guest_extent times scale
  float scale;              // the resolution scale it's drawn at
  Image image;               // view: for drawing (depth and stencil)
  VkImageView sample_view;   // for resolving (depth only, for depth)
  VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;
  // Its image was handed to a resolve destination (HandOver) and it hasn't
  // been cleared since: what it held is in that image, copied back before
  // it's drawn into or read (RestoreContents) while the destination holds
  // it.
  VkImage handed_image = VK_NULL_HANDLE;
  uint32_t handed_fetch[6] = {};
  float handed_scale = 1.0f;
  VkImageAspectFlags aspects() const {
    return depth ? kDepthStencilAspects : VK_IMAGE_ASPECT_COLOR_BIT;
  }
};

// Texture descriptor sets (sets 2 and 3) in each of a frame's texture
// pools, and their descriptors; a frame that fills its pools gets another,
// kept for later frames.
constexpr uint32_t kTextureSetsPerPool = 8192;
constexpr uint32_t kTextureDescriptorsPerPool = 32768;

// The lowest and highest of `count` indices as the GPU reads them, leaving
// out the restart index (all ones) when `skip_restart`; {UINT32_MAX, 0} if
// there are none. One plain loop per format, so the compiler vectorizes it
// (scanning an index at a time with the format checks inside was ~20% of
// the render thread).
template <typename T, typename Swap>
std::pair<uint32_t, uint32_t> IndexRangeOf(const uint8_t* data, uint32_t count, bool skip_restart,
                                           Swap swap) {
  constexpr T kRestart = T(~T(0));
  T low = kRestart, high = 0;
  if (skip_restart) {
    // The restart index is the type's largest value, so it never lowers the
    // minimum; it's left out of the maximum.
    for (uint32_t i = 0; i < count; i++) {
      T value;
      std::memcpy(&value, data + sizeof(T) * i, sizeof(T));
      value = swap(value);
      low = std::min(low, value);
      high = std::max(high, value == kRestart ? T(0) : value);
    }
    if (low == kRestart) {
      return {UINT32_MAX, 0};  // only restarts
    }
  } else {
    for (uint32_t i = 0; i < count; i++) {
      T value;
      std::memcpy(&value, data + sizeof(T) * i, sizeof(T));
      value = swap(value);
      low = std::min(low, value);
      high = std::max(high, value);
    }
  }
  if (!count) {
    return {UINT32_MAX, 0};
  }
  return {low, high};
}

std::pair<uint32_t, uint32_t> IndexRange(const uint8_t* data, uint32_t count, bool index_32bit,
                                         uint32_t endian, bool skip_restart) {
  if (!index_32bit) {
    if (endian == uint32_t(xenos::Endian::k8in16)) {
      return IndexRangeOf<uint16_t>(data, count, skip_restart,
                                    [](uint16_t v) { return uint16_t(__builtin_bswap16(v)); });
    }
    return IndexRangeOf<uint16_t>(data, count, skip_restart, [](uint16_t v) { return v; });
  }
  switch (xenos::Endian(endian)) {
    case xenos::Endian::k8in32:
      return IndexRangeOf<uint32_t>(data, count, skip_restart,
                                    [](uint32_t v) { return __builtin_bswap32(v); });
    case xenos::Endian::k8in16:
      return IndexRangeOf<uint32_t>(data, count, skip_restart, [](uint32_t v) {
        return ((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu);
      });
    case xenos::Endian::k16in32:
      return IndexRangeOf<uint32_t>(data, count, skip_restart,
                                    [](uint32_t v) { return (v << 16) | (v >> 16); });
    default:
      return IndexRangeOf<uint32_t>(data, count, skip_restart, [](uint32_t v) { return v; });
  }
}

// Draws' index ranges (IndexRange's result) by address, count and format,
// so a range drawn again needn't be scanned: index data is written by the
// library's memcpy and nothing else (measured with
// --native_renderer_log_index_ranges), so a result holds while
// Buffers_NewestWrite says the range wasn't written since its scan. That's
// asked once a frame per range (as the mirror does), and the ~60% of draws
// repeating a range already drawn this frame only look it up. An
// open-addressed table (a multiply and a probe), emptied when it fills.
class IndexRangeCache {
 public:
  struct Entry {
    uint64_t key;  // see Key; 0 for an empty slot
    uint64_t sequence;  // Buffers_Sequence when scanned
    uint64_t frame;     // the frame it was last found unwritten in
    uint32_t low, high;
  };

  static uint64_t Key(uint32_t address, uint32_t count, bool index_32bit, uint32_t endian,
                      bool skip_restart) {
    // A draw's count is far below 2^28, leaving room for the format; the
    // address is a physical one (below 2^29), so a key is never 0.
    return (uint64_t(address | 0x80000000u) << 32) | (count & 0x0FFFFFFFu) |
           (uint64_t(index_32bit) << 28) | (uint64_t(endian & 3) << 29) |
           (uint64_t(skip_restart) << 31);
  }

  // The entry for `key`, or an empty slot to fill (its key 0).
  Entry& Find(uint64_t key) {
    if (size_ * 2 >= slots_.size()) {
      Reset(slots_.size() < kMaxSlots ? slots_.size() * 2 : slots_.size());
    }
    size_t mask = slots_.size() - 1;
    size_t i = (key * 0x9E3779B97F4A7C15ull) >> shift_;
    while (slots_[i].key && slots_[i].key != key) {
      i = (i + 1) & mask;
    }
    return slots_[i];
  }

  // After filling the slot Find returned.
  void Added() { size_++; }

 private:
  static constexpr size_t kMaxSlots = size_t(1) << 16;
  void Reset(size_t slots) {
    slots = std::max<size_t>(slots, 4096);
    shift_ = 64 - std::countr_zero(slots);
    slots_.assign(slots, Entry{});
    size_ = 0;
  }
  std::vector<Entry> slots_;
  size_t size_ = 0;
  uint32_t shift_ = 64;
};

// What a draw's shader lookup finds: the translations, how they connect,
// and the microcodes' hashes (for the diagnostics keyed by them).
struct ShaderLookup {
  TranslatedShader* vertex = nullptr;
  TranslatedShader* pixel = nullptr;
  ShaderCache::Interface interface;
  uint64_t vertex_hash = 0, pixel_hash = 0;
};

// Shader lookups kept by what the draw reads from the shader objects instead
// of the microcode's hash (see "Shader lookup" in docs/native-renderer.md):
// both objects, their microcode's address and size, the bound declaration
// (its address and id) and the stream strides the vertex fetches take their
// fields from, and the registers and flags the interface depends on (the
// microcode itself isn't changed since the library's patch stopped running,
// docs/pc-renderer.md, step 2). Shaders are never freed, so their pointers
// stay good. An open-addressed table, at most half
// full; never emptied (a session meets ~600 keys).
class ShaderLookupTable {
 public:
  struct Key {
    uint32_t words[16];
    bool operator==(const Key& other) const {
      return std::memcmp(words, other.words, sizeof(words)) == 0;
    }
  };

  // The key's answer, or null.
  const ShaderLookup* Find(const Key& key) const {
    if (slots_.empty()) {
      return nullptr;
    }
    size_t mask = slots_.size() - 1;
    for (size_t i = HashOf(key) & mask;; i = (i + 1) & mask) {
      const Slot& slot = slots_[i];
      if (!slot.used) {
        return nullptr;
      }
      if (slot.key == key) {
        return &slot.value;
      }
    }
  }

  // After Find gave null.
  void Add(const Key& key, const ShaderLookup& value) {
    if ((size_ + 1) * 2 > slots_.size()) {
      Grow();
    }
    Place(key, value);
    size_++;
  }

 private:
  struct Slot {
    Key key;
    bool used;
    ShaderLookup value;
  };
  static size_t HashOf(const Key& key) {
    uint64_t h = 0;
    for (int i = 0; i < 16; i += 2) {
      uint64_t word;
      std::memcpy(&word, &key.words[i], 8);
      h = (h ^ word) * 0x9E3779B97F4A7C15ull;
    }
    // Mixed down (MurmurHash3's finalizer), as multiplying carries bits
    // only upward.
    h ^= h >> 33;
    h *= 0xFF51AFD7ED558CCDull;
    h ^= h >> 33;
    h *= 0xC4CEB9FE1A85EC53ull;
    h ^= h >> 33;
    return size_t(h);
  }
  void Place(const Key& key, const ShaderLookup& value) {
    size_t mask = slots_.size() - 1;
    size_t i = HashOf(key) & mask;
    while (slots_[i].used) {
      i = (i + 1) & mask;
    }
    slots_[i] = {key, true, value};
  }
  void Grow() {
    std::vector<Slot> old = std::move(slots_);
    slots_.assign(std::max<size_t>(old.size() * 2, 1024), Slot{});
    for (const Slot& slot : old) {
      if (slot.used) {
        Place(slot.key, slot.value);
      }
    }
  }
  std::vector<Slot> slots_;
  size_t size_ = 0;
};

}  // namespace

struct Passes::State {
  gpu::vulkan::Backend* backend = nullptr;
  DeviceContext context{};
  VkPhysicalDeviceMemoryProperties memory_properties{};
  uint32_t frames_in_flight = 0;

  // Resolution scaling (see "Resolution" in docs/native-renderer.md): every
  // target and resolve destination is this many times its guest size, and
  // positions in the guest's pixels (viewports, scissors, resolve
  // rectangles) are scaled and rounded to whole pixels. Shaders are
  // translated unscaled: none of this game's reads the pixel position or
  // fetches in texels.
  float scale = 1.0f;
  // Above or below 1, the 3D view (1024x576, upscaled to the 1280x720 frame
  // by the game) is drawn 1.25 times larger again, as are the textures it's
  // resolved into, so the game's upscale becomes a copy one to one.
  float main_view_scale = 1.0f;
  static int32_t ScaledBy(int32_t guest, float by) { return int32_t(std::lround(guest * by)); }
  static uint32_t ScaledBy(uint32_t guest, float by) {
    return uint32_t(std::lround(guest * by));
  }
  static VkExtent2D ScaledBy(VkExtent2D guest, float by) {
    return {std::max(ScaledBy(guest.width, by), 1u), std::max(ScaledBy(guest.height, by), 1u)};
  }
  VkExtent2D Scaled(VkExtent2D guest) const { return ScaledBy(guest, scale); }
  // The main view's surfaces (color and depth): pitch 1040, 2x MSAA.
  static bool IsMainViewSurface(const Surface& surface) {
    return surface.pitch == kMainViewPitch && surface.msaa == 1;
  }
  // What's shown and the main view's drawn part, scaled.
  VkExtent2D shown_extent{kShownWidth, kShownHeight};
  VkExtent2D main_view_extent{kMainViewWidth, kMainViewHeight};

  // The vertex and index data draws read (set 0 and the index buffer):
  // device-local, the ranges draws use copied in (docs/pc-renderer.md
  // step 1).
  VertexArena arena;
  VkDescriptorSet arena_set = VK_NULL_HANDLE;
  // With check_mirror: each range's hash when copied, to find ranges whose
  // contents change within a frame (draws after the change get the first).
  bool check_mirror = false;
  // With watch_draws: each main view draw's inputs, by the draw (shaders,
  // index address, primitive count, and which of the frame's draws with
  // those it is), to log what changes between frames when the camera
  // stands still.
  bool watch_draws = false;
  // The draw ID pass (with record_draws): each main view draw drawn again
  // after the frame, with its vertex shader, geometry, constants and depth
  // state, writing its number in recorded_draws into an R32_UINT image.
  struct IdDraw {
    uint32_t number;
    VkShaderModule vertex;
    VkPipelineLayout layout;
    VkDescriptorSet sets[4];
    uint32_t offsets[kConstantBlocks];
    VkViewport viewport;
    VkRect2D scissor;
    VkDeviceSize index_offset;
    VkIndexType index_type;
    uint32_t index_count;
    int32_t base_vertex;
    uint32_t vertex_bindings;
    VkDeviceSize vertex_offsets[kMaxVertexBindings];
    PipelineKey key;
    float bias_constant, bias_slope;
  };
  std::vector<IdDraw> id_draws;
  std::unordered_map<VkPipelineLayout, VkPipelineLayout> id_layouts;
  std::unordered_map<PipelineKey, VkPipeline, PipelineKeyHash> id_pipelines;
  VkShaderModule id_pixel = VK_NULL_HANDLE;
  Image id_image, id_depth;
  bool id_drawn = false;  // this frame, for the dump
  VkPipeline GetIdPipeline(const IdDraw& draw);
  void DrawIds(VkCommandBuffer commands);
  uint64_t watch_pixel_shader = 0;  // only its draws, if set
  bool hide_cubes = false;
  uint64_t mismatched_seen = 0;
  std::unordered_set<std::string> mismatch_lines;
  // Pixel shaders given PsParamGen (interpolators, a bit each), and the
  // interpolator masks each was translated with (bits 16-31: with
  // SQ_PROGRAM_CNTL's param_gen set).
  std::map<uint64_t, uint32_t> param_gen_by_shader;
  std::map<uint64_t, uint64_t> interpolators_by_shader;  // placeholders for cube maps (debugging aid)
  // Whether to fill the two maps above: they're only logged (at info level),
  // and cost two shader hashes a draw.
  bool collect_interfaces = true;
  struct WatchedDraw {
    uint64_t frame = 0;
    std::vector<uint32_t> system, vertex_floats, pixel_floats, bool_loop, fetch;
    std::string key;
  };
  std::map<std::tuple<uint64_t, uint64_t, uint32_t, uint32_t, uint32_t>, WatchedDraw> watched;
  std::map<std::tuple<uint64_t, uint64_t, uint32_t, uint32_t>, uint32_t> watched_this_frame;
  uint64_t watched_changes = 0, watched_draws = 0;
  std::vector<std::string> watch_lines;
  void WatchDraw(const TranslatedShader* vertex, const TranslatedShader* pixel,
                 uint32_t index_address, uint32_t primitive_count, const PipelineKey& key,
                 const uint32_t* system, const uint32_t* vertex_floats,
                 const uint32_t* pixel_floats, const uint32_t* bool_loop, const uint32_t* fetch);
  std::unordered_map<uint64_t, uint64_t> mirrored_hashes;
  // With check_mirror_frames: each range's hash in the frame it was last
  // copied, and how often it changed since the last log.
  bool check_mirror_frames = false;
  // Copy every range every frame, as before memcpy's writes were tracked;
  // compare each range not copied with guest memory (debugging aids).
  bool copy_all = false, verify_mirror = false;
  // Don't record state calls that repeat what the command buffer holds.
  bool skip_repeated_state = true;
  // With check_late_writes: the ranges copied this frame and their hashes
  // then, to find ranges written after we copied them (the emulated GPU
  // reads them much later).
  bool check_late_writes = false;
  struct CopiedRange {
    uint32_t address, size;
    uint64_t hash;
    bool index;
    uint64_t vertex_shader;
  };
  std::vector<CopiedRange> copied_ranges;
  uint64_t current_vertex_shader = 0, current_pixel_shader = 0;
  // With check_clobbers: the ranges copied this frame, by start (end,
  // index or not, the shaders of the draw that copied it), to find copies
  // that change bytes an earlier draw's copy put in the mirror (the GPU
  // draws the whole frame after its copies, so that draw gets the new
  // bytes). Counted per pair of shaders: earlier draw's, then this one's.
  bool check_clobbers = false;
  struct ClobberRange {
    uint32_t end;
    bool index;
    uint64_t vertex_shader, pixel_shader;
  };
  std::multimap<uint32_t, ClobberRange> clobber_ranges;
  uint32_t clobber_longest = 0;
  uint64_t clobbers = 0, clobbered_bytes = 0, clobber_copies = 0;
  std::map<std::array<uint64_t, 4>, uint64_t> clobbers_by_shaders;
  std::string clobber_sample;
  bool drew_any = false;  // a draw has run (late writes and resolve dumps wait on one)
  uint64_t late_written = 0, late_checked = 0;
  std::map<uint64_t, uint64_t> late_by_shader;  // vertex shader: count
  std::string late_sample;
  struct RangeHistory {
    uint64_t hash;
    uint32_t changes;
    bool index;
  };
  std::unordered_map<uint64_t, RangeHistory> range_history;
  // Also with check_mirror_frames, a frame at a time: bytes copied whose
  // contents differ from that range's last copy, and bytes of ranges never
  // copied before; and what copying only each draw's vertices (from its
  // lowest to its highest) would copy, each range once a frame.
  uint64_t mirror_frames = 0, mirror_changed_bytes = 0, mirror_new_bytes = 0,
           mirror_vertex_bytes = 0, used_vertex_bytes = 0;
  std::unordered_set<uint64_t> used_mirrored;
  IndexRangeCache index_cache;
  bool cache_index_ranges = true;
  // IndexRange's result for the draw's indices, from index_cache when it
  // holds.
  // With log_index_ranges: each draw's index range (address, count and
  // format) as last scanned, to see how often a scan could be reused (the
  // range not written by memcpy since) and whether the reused result would
  // be the same; and how often the mirror's question to Buffers_NewestWrite
  // comes with no write taken since the range was copied.
  bool log_index_ranges = false;
  struct IndexRangeSeen {
    uint64_t sequence;  // Buffers_Sequence when scanned
    uint32_t low, high;
    uint64_t frame;  // mirror_frames when last seen
  };
  std::unordered_map<uint64_t, IndexRangeSeen> index_ranges_seen;
  struct IndexRangeCounts {
    uint64_t frames = 0, scans = 0, indices = 0, first = 0, same_frame = 0, written = 0,
             unchanged = 0, unchanged_indices = 0, mismatched = 0, distinct = 0,
             mirror_calls = 0, mirror_repeats = 0, newest_asked = 0, nothing_taken = 0,
             writes = 0;
    uint64_t frame_sequence = 0;  // Buffers_Sequence at the frame's start
    uint32_t mismatch_sample[6] = {};  // address, count, cached low/high, scanned low/high
  } index_range_counts;
  // With log_texture_bindings: how much of texture binding repeats, to
  // judge caching it (TakeTextureBindings describes it). A binding repeats
  // its fetch constant's last when the words (and view type, or filter
  // overrides) are the same with no texture cache generation between;
  // "seen" counts those met earlier in the same generation (any fetch
  // constant). A draw repeats the last when both shaders and the fetch
  // constants they read do.
  bool log_texture_bindings = false;
  struct TextureBindingCounts {
    uint64_t frames = 0, draws = 0, draws_textured = 0, draws_same = 0, draws_same_words = 0,
             vertex_same = 0, pixel_same = 0, views = 0, view_same_slot = 0,
             view_same_slot_words = 0, view_seen = 0, samplers = 0, sampler_same_slot = 0,
             sampler_seen = 0, sets = 0, sets_found = 0;
    uint64_t generation = 0;  // the texture cache's when last taken
  } binding_counts;
  struct LastBinding {
    uint32_t words[10];  // the fetch constant, then the view type or overrides
    uint64_t generation;
    bool valid;
  };
  LastBinding last_view[32] = {}, last_sampler[32] = {};
  std::unordered_map<uint64_t, uint64_t> views_seen;  // by key hash: generation
  std::unordered_set<uint64_t> samplers_seen;
  struct LastDraw {
    const TranslatedShader* vertex = nullptr;
    const TranslatedShader* pixel = nullptr;
    uint64_t generation = 0;
    uint32_t fetch[192] = {};
    bool valid = false;
  } last_draw;
  void CountBinding(bool view, uint32_t slot, const uint32_t* fetch, const uint32_t extra[4]);
  void CountDrawBindings(const TranslatedShader* vertex, const TranslatedShader* pixel);
  void LogGlow();
  // Debugging aids: draws of this pixel shader (hash) are skipped; with
  // record_draws, each draw is described in a line (written with a dump).
  uint64_t skip_pixel_shader = 0;
  bool skip_motion_blur = false;  // --motion_blur=false
  // A resolve that is a whole-image copy from a single-sample color target
  // hands the target's image to the destination texture instead
  // (docs/pc-renderer.md, step 4; --native_renderer_hand_over_resolves).
  bool hand_over_resolves = true;
  // The depth format can be sampled with linear filtering (the depth
  // resolves' readers' samplers may ask for it).
  bool depth_filterable = false;
  uint32_t logged_new_target_images = 0;
  std::unordered_set<uint64_t> logged_copy_backs;
  bool CreateTargetImage(Target& t);
  void RestoreContents(VkCommandBuffer commands, Target& t);
  bool record_draws = false;
  // With log_glow: per frame, the glow history's cross-fade draws (pixel
  // shader kGlowCrossFade) and the texture they read, and 256x144 resolves
  // without a clear (the history written back); logged once a second by
  // whether a logic pass recorded the frame.
  bool log_glow = false;
  struct GlowFrame {
    uint32_t cross_fades = 0, history_resolves = 0, cross_fade_texture = 0,
             history_address = 0;
    std::vector<uint32_t> addresses;  // the 256x144 resolves without a clear
    std::string constants;  // the cross-fade's pixel float constants
  } glow_frame;
  std::array<std::string, 2> glow_addresses;  // an example frame's, by kind
  std::array<std::string, 2> glow_constants;  // the last cross-fade's, by kind
  struct GlowCounts {
    uint64_t frames = 0, cross_fades = 0, history_resolves = 0, reads_history = 0;
  };
  std::array<GlowCounts, 2> glow_counts{};  // render-only, logic
  std::string glow_pattern;
  std::chrono::steady_clock::time_point glow_last_log{};
  struct RecordedDraw {
    bool main_view, has_color, has_depth, inline_vertices;
    Surface surface;
    uint64_t vertex, pixel;
    uint32_t type, count;
    // RB_DEPTHCONTROL, RB_BLENDCONTROL0, RB_COLORCONTROL, RB_MODECONTROL,
    // RB_COLOR_MASK, RB_BLEND_RED/GREEN/BLUE/ALPHA, RB_ALPHA_REF.
    std::array<uint32_t, 10> render_state;
    // The vertex fetch constants the vertex shader reads: index, then words.
    std::vector<std::array<uint32_t, 3>> vertex_fetches;
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

  // Targets by surface (color or depth, EDRAM base, pitch, samples, the
  // format they're drawn in), and the ones being drawn into now.
  std::map<std::array<uint32_t, 7>, Target> targets;
  Target* bound_color = nullptr;
  Target* bound_depth = nullptr;
  VkExtent2D render_extent{};
  bool rendering = false;
  Target* main_view = nullptr;  // drawn into this frame
  // The target resolved to the front buffer this frame, and the front
  // buffer's size.
  Target* front = nullptr;
  VkExtent2D front_extent{};
  bool logged_front = false;
  std::unordered_set<uint64_t> logged_tilings, logged_tiled_resolves;

  // What's shown, and this frame's resolve destinations (for showing one).
  Image shown;
  VkImageLayout shown_layout = VK_IMAGE_LAYOUT_UNDEFINED;
  // --xbox_gamma: a gamma table (xbox_gamma_table.h) applied to `shown` in
  // place (gamma.comp).
  const uint16_t* gamma_values = nullptr;
  VkBuffer gamma_table = VK_NULL_HANDLE;
  VkDeviceMemory gamma_table_memory = VK_NULL_HANDLE;
  VkDescriptorSetLayout gamma_set_layout = VK_NULL_HANDLE;
  VkPipelineLayout gamma_layout = VK_NULL_HANDLE;
  VkPipeline gamma_pipeline = VK_NULL_HANDLE;
  VkDescriptorPool gamma_pool = VK_NULL_HANDLE;
  VkDescriptorSet gamma_set = VK_NULL_HANDLE;
  bool gamma_ready = false;  // the table has been uploaded
  bool CreateGammaResources();
  void ApplyGamma(VkCommandBuffer commands);
  struct Resolved {
    VkImage image;
    VkExtent2D extent;
    uint32_t address, format, flags;
    VkFormat host_format;
    uint32_t fetch[6];  // the destination's fetch constant, addresses physical
    // A target's image handed over (at the target's scale): held by the
    // destination only until a later hand-over gives it back to a target.
    bool handed = false;
    float scale = 1.0f;
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
  };
  std::vector<Resolved> frame_resolves;
  // The resolve's image still in SHADER_READ_ONLY_OPTIMAL as its texture's.
  bool Held(const Resolved& r) const {
    return !r.handed || textures.HoldsHanded(r.fetch, r.scale, r.image);
  }
  uint64_t frames = 0;  // for logging the resolve list now and then
  size_t last_frame_resolves = 0;

  // A dump being read back: the images' data, one after another.
  struct Dumped {
    std::string name;
    VkFormat format;
    VkExtent2D extent;
    VkDeviceSize offset;
    std::vector<uint8_t> cpu;  // if not empty, the data (not in the buffer)
  };
  std::vector<Dumped> dumped;
  VkBuffer dump_buffer = VK_NULL_HANDLE;
  VkDeviceMemory dump_memory = VK_NULL_HANDLE;
  void RecordDump(VkCommandBuffer commands, VkExtent2D shown_extent, const std::string& prefix,
                  bool with_resolves);
  void DumpGuestResolves();
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

  // Movies' frames (DrawMovie): their own pixel shader with the resolve's
  // vertex shader; per frame in flight, the three planes and the buffer
  // they're uploaded through. Made at the first frame, for its size.
  struct MovieUpload {
    Image planes[3];
    VkImageLayout layouts[3] = {VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_UNDEFINED};
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    uint8_t* mapped = nullptr;
    VkDescriptorSet set = VK_NULL_HANDLE;
  };
  std::vector<MovieUpload> movie_uploads;
  VkExtent2D movie_extent{};
  bool movie_failed = false;
  VkDescriptorSetLayout movie_set_layout = VK_NULL_HANDLE;
  VkPipelineLayout movie_layout = VK_NULL_HANDLE;
  VkDescriptorPool movie_pool = VK_NULL_HANDLE;
  VkShaderModule movie_pixel = VK_NULL_HANDLE;
  VkSampler movie_sampler = VK_NULL_HANDLE;
  std::map<std::pair<VkFormat, VkSampleCountFlagBits>, VkPipeline> movie_pipelines;

  struct ConstantBlock {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint8_t* mapped = nullptr;
    VkDescriptorPool pool = VK_NULL_HANDLE;
    VkDescriptorSet set = VK_NULL_HANDLE;
  };
  struct Ring {
    std::vector<ConstantBlock> blocks;
    size_t block = 0;  // the one in use this frame
    // This frame's texture descriptor sets, reset with the frame.
    std::vector<VkDescriptorPool> texture_pools;
    size_t texture_pool = 0;  // the one in use this frame
  };
  std::vector<Ring> rings;
  uint32_t frame = 0;
  VkDeviceSize ring_offset = 0;  // in the block in use

  VkDescriptorPool pool = VK_NULL_HANDLE;
  VkDescriptorSetLayout shared_memory_layout = VK_NULL_HANDLE;
  VkDescriptorSetLayout constants_layout = VK_NULL_HANDLE;

  // Textures, and white 1x1 placeholders of each declared type for what
  // can't be had yet.
  TextureCache textures;
  gpu::Image* dummy_2d_array = nullptr;
  gpu::Image* dummy_3d = nullptr;
  gpu::Image* dummy_cube = nullptr;
  // Zero (transparent black) ones for a texture of another dimension than
  // the shader declares: the plugin binds a null descriptor, which reads
  // as zero.
  gpu::Image* zero_2d_array = nullptr;
  gpu::Image* zero_3d = nullptr;
  gpu::Image* zero_cube = nullptr;
  VkSampler sampler = VK_NULL_HANDLE;
  // Texture set layouts by the shader's bindings; this frame's sets by
  // their contents.
  std::unordered_map<std::string, VkDescriptorSetLayout> texture_layouts;
  // Each shader's layout (shaders are never freed, and have one stage).
  std::unordered_map<const TranslatedShader*, VkDescriptorSetLayout> shader_texture_layouts;
  FrameSetCache texture_sets;
  // GetTextureSet's buffers, kept to save allocating them every draw.
  std::vector<VkDescriptorImageInfo> set_infos;
  std::vector<uint64_t> set_key;
  std::vector<VkWriteDescriptorSet> set_writes;
  std::unordered_map<std::pair<VkDescriptorSetLayout, VkDescriptorSetLayout>, VkPipelineLayout,
                     HandlePairHash>
      pipeline_layouts;
  std::unordered_map<PipelineKey, VkPipeline, PipelineKeyHash> pipelines;

  std::unique_ptr<ShaderCache> shaders;
  // The driver's compiled pipelines, kept in the disk cache (disk_cache.h):
  // loaded at start and saved at most every 10 s after new pipelines.
  VkPipelineCache pipeline_cache = VK_NULL_HANDLE;
  std::filesystem::path pipeline_cache_file;
  size_t pipeline_cache_saved_pipelines = 0;
  std::chrono::steady_clock::time_point pipeline_cache_saved;
  void OpenPipelineCache();
  void MaybeSavePipelineCache();
  // The draw's state, from the device's shadow (ReadState).
  DrawRegisters regs;
  Stats stats;
  // The state the last draw recorded into the frame's command buffer, to
  // count the state calls that repeat it (stats.state_repeated). Forgotten
  // at each frame's start and after anything else that sets state (a
  // resolve's draw, the ID pass).
  struct RecordedState {
    bool valid = false;
    VkViewport viewport;
    VkRect2D scissor;
    float bias[2];
    float blend_constants[4];
    VkPipeline pipeline;
    VkPipelineLayout layout;
    VkDescriptorSet sets[4];
    uint32_t offsets[kConstantBlocks];  // the constants' dynamic offsets
    bool indexed;  // index buffer below bound
    VkBuffer index_buffer;
    VkDeviceSize index_offset;
    VkIndexType index_type;
    // The vertex buffers bound (the arena at these offsets).
    uint32_t vertex_bindings;
    VkDeviceSize vertex_offsets[kMaxVertexBindings];
  } recorded;
  // Each vertex shader module's vertex input: a binding a fetch (three for
  // a rectangle list, at instance rate), its attributes the raw words.
  struct VertexInput {
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;
  };
  std::unordered_map<VkShaderModule, VertexInput> vertex_inputs;
  // The constants each block (system, vertex floats, pixel floats,
  // bool/loop) last had written into the constant block in use: a
  // copy of the bytes and where they went, so a draw whose block holds the
  // same bytes points at them instead of writing them again
  // (reuse_constants). Forgotten when the frame or the block changes.
  struct LastConstants {
    bool valid[kConstantBlocks] = {};
    uint32_t offsets[kConstantBlocks];
    uint32_t sizes[kConstantBlocks];  // bytes written
    alignas(16) uint8_t bytes[kConstantBlocks][kFloatConstantsRange];
  } last_constants;
  bool reuse_constants = true;
  bool log_constant_changes = false;
  void ReadState(uint32_t) {
    // The fetch constants from the renderer's own state (render/state.h):
    // the sampler stages' texture descriptors (constants 0-25, 6 words
    // each), and each stream's vertex fetch constant (95 - stream, 2
    // words, in the same space).
    std::memset(regs.fetch, 0, sizeof(regs.fetch));
    for (uint32_t stage = 0; stage < 26; stage++) {
      std::memcpy(&regs.fetch[6 * stage], render::TextureDescriptor(stage), 24);
    }
    const render::Bindings& bound = render::BoundResources();
    for (uint32_t stream = 0; stream < 16; stream++) {
      regs.fetch[2 * (95 - stream)] = bound.stream_fetch[stream][0];
      regs.fetch[2 * (95 - stream) + 1] = bound.stream_fetch[stream][1];
    }
  }
  // Points offset at a copy of the block's size bytes in the constant
  // block in use: the last one written if the same (and reuse_constants),
  // or a new one, reserving range bytes. With copied, the bytes are
  // already in last_constants.bytes[block] and differ from the last ones
  // written unless same.
  bool PlaceConstants(uint32_t block, const void* bytes, uint32_t size, uint32_t range,
                      uint32_t& offset, bool copied = false, bool same = false);

  // An image from the backend, its memory counted (gpu_memory.h).
  gpu::Image* NewImage(const gpu::ImageDesc& desc, const void* initial = nullptr) {
    gpu::Image* image = backend->CreateImage(desc, initial);
    if (image) {
      gpu_memory::Allocated(gpu_memory::Kind::kImage, backend->MemorySize(image));
    }
    return image;
  }
  static VkImageView View(gpu::Image* image) {
    return gpu::vulkan::Backend::Object(image).view;
  }
  bool CreateImage(Image& out, VkImageType type, VkImageViewType view_type, VkFormat format,
                   VkExtent3D extent, uint32_t layers, VkImageUsageFlags usage,
                   VkImageAspectFlags aspect, VkImageCreateFlags flags = 0,
                   VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
  // The bound surface's target, made (and cleared) on first use; null if
  // its format isn't drawn.
  Target* GetTarget(VkCommandBuffer commands, uint32_t object, bool depth);
  // Clears the whole target (aspects: all if 0).
  void ClearTarget(VkCommandBuffer commands, Target& target, const VkClearValue& value,
                   VkImageAspectFlags aspects = 0);
  // Draws the source's rectangle (left, top, right, bottom; the whole
  // destination if null) into the resolve's destination texture at the
  // point (x, y; 0, 0 if null).
  bool ResolveInto(VkCommandBuffer commands, Target& source, uint32_t destination,
                   const int32_t* rect, const int32_t* point, uint32_t flags);
  // A resolve as the library's Resolve (sub_822AFA58) takes it: flags as
  // the XDK's D3DRESOLVE, from the device's bound surfaces; then the clears
  // its flags ask for (0x100 color, 0x200 depth and stencil), if
  // apply_clears.
  void Resolve(VkCommandBuffer commands, uint32_t device, uint32_t flags,
               const int32_t* rect, uint32_t destination, const int32_t* point,
               bool level_or_slice, const float* clear_color, float clear_z,
               uint32_t clear_stencil, bool apply_clears);
  // Predicated tiling (commands 32 and 33; see "Predicated tiling" in
  // docs/gpu-functions.md): the surfaces are drawn whole, not tile by tile,
  // so targets made while tiling are as large as the tiles together.
  bool tiling = false;
  VkExtent2D tiling_extent{};
  bool CreateResolveResources();
  VkPipeline GetResolvePipeline(VkFormat format, bool multisample);
  // A pipeline drawing the resolve's viewport-covering triangle with
  // `pixel`, into one color attachment, no blending.
  VkPipeline MakeFullscreenPipeline(VkShaderModule pixel, VkPipelineLayout layout,
                                    VkFormat format, VkSampleCountFlagBits samples);
  bool CreateMovieResources(VkExtent2D extent);
  VkPipeline GetMoviePipeline(VkFormat format, VkSampleCountFlagBits samples);
  void EndRendering(VkCommandBuffer commands);
  bool CreateRings();
  bool CreateConstantBlock(Ring& ring);
  bool CreateTexturePool(Ring& ring);
  // Makes room for a draw's constants in one block, moving to the next
  // block (made if needed) when the one in use can't hold them.
  bool ReserveDrawConstants();
  ConstantBlock& constant_block() { return rings[frame].blocks[rings[frame].block]; }
  bool CreateDescriptors();
  bool CreateDummyTextures();
  VkDescriptorSetLayout GetTextureLayout(const TranslatedShader* shader,
                                         VkShaderStageFlags stage);
  // The texture set for the shader's bindings from the current fetch
  // constants (stage: 0 vertex, 1 pixel): the last one for the stage again
  // when nothing it was built from changed (reuse_texture_sets), else
  // BuildTextureSet's.
  VkDescriptorSet GetTextureSet(const TranslatedShader* shader,
                                VkDescriptorSetLayout layout, uint32_t stage);
  VkDescriptorSet BuildTextureSet(const TranslatedShader* shader,
                                  VkDescriptorSetLayout layout);
  // Each stage's last texture set and what it was built from: the shader,
  // the layout, the fetch constants the shader reads (a bit each, and their
  // words), and the texture cache's generations. Null set: none to reuse
  // (forgotten each frame, as sets are; not kept when an answer it was
  // built from wasn't kept by the texture cache, e.g. a placeholder).
  struct LastTextureSet {
    const TranslatedShader* shader = nullptr;
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    uint64_t generation = 0, sampler_generation = 0;
    uint32_t fetch_mask = 0;
    uint32_t fetch[32 * 6];
    VkDescriptorSet set = VK_NULL_HANDLE;
  } last_texture_sets[2];
  bool reuse_texture_sets = true;
  // Draws' shader lookups kept by what they read from the shader objects
  // (cache_shader_lookups; ShaderLookupTable), the last draw's checked
  // first.
  bool cache_shader_lookups = true;
  ShaderLookupTable shader_lookups;
  ShaderLookupTable::Key last_shader_key = {};
  ShaderLookup last_shader_lookup;
  bool last_shader_valid = false;
  VkPipelineLayout GetPipelineLayout(VkDescriptorSetLayout vertex, VkDescriptorSetLayout pixel);
  VkShaderModule GetModule(TranslatedShader* shader);
  VkPipeline GetPipeline(const PipelineKey& key);
  bool Reserve(VkDeviceSize size, uint32_t& offset, uint8_t*& data);
  void BeginRendering(VkCommandBuffer commands, Target* color, Target* depth);
  void Draw(VkCommandBuffer commands, uint32_t device, const DrawCall& call);
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
  gpu_memory::Allocated(gpu_memory::Kind::kImage, alloc.allocationSize);
  VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  view.image = out.image;
  view.viewType = view_type;
  view.format = format;
  view.subresourceRange = {aspect, 0, 1, 0, layers};
  return vk::vkCreateImageView(device, &view, nullptr, &out.view) == VK_SUCCESS;
}

bool Passes::State::CreateRings() {
  rings.resize(frames_in_flight);
  for (Ring& ring : rings) {
    if (!CreateTexturePool(ring)) {
      return false;
    }
  }
  return true;
}

bool Passes::State::CreateTexturePool(Ring& ring) {
  VkDescriptorPoolSize sizes[] = {
      {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, kTextureDescriptorsPerPool},
      {VK_DESCRIPTOR_TYPE_SAMPLER, kTextureDescriptorsPerPool},
  };
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = kTextureSetsPerPool;
  pool_info.poolSizeCount = 2;
  pool_info.pPoolSizes = sizes;
  VkDescriptorPool pool;
  if (vk::vkCreateDescriptorPool(context.device, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  gpu_memory::Created(gpu_memory::Object::kDescriptorPool);
  ring.texture_pools.push_back(pool);
  return true;
}

// Called once constants_layout exists (CreateDescriptors).
bool Passes::State::CreateConstantBlock(Ring& ring) {
  VkDevice device = context.device;
  ConstantBlock block;
  VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  info.size = kConstantBlockSize;
  info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &info, nullptr, &block.buffer) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, block.buffer, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex =
      FindMemoryType(memory_properties, requirements.memoryTypeBits,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &block.memory) != VK_SUCCESS) {
    vk::vkDestroyBuffer(device, block.buffer, nullptr);
    return false;
  }
  if (vk::vkBindBufferMemory(device, block.buffer, block.memory, 0) != VK_SUCCESS ||
      vk::vkMapMemory(device, block.memory, 0, VK_WHOLE_SIZE, 0,
                      reinterpret_cast<void**>(&block.mapped)) != VK_SUCCESS) {
    vk::vkDestroyBuffer(device, block.buffer, nullptr);
    vk::vkFreeMemory(device, block.memory, nullptr);
    return false;
  }
  gpu_memory::Allocated(gpu_memory::Kind::kConstants, alloc.allocationSize);
  VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 5};
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &size;
  if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &block.pool) != VK_SUCCESS) {
    return false;
  }
  gpu_memory::Created(gpu_memory::Object::kDescriptorPool);
  VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  set_alloc.descriptorPool = block.pool;
  set_alloc.descriptorSetCount = 1;
  set_alloc.pSetLayouts = &constants_layout;
  if (vk::vkAllocateDescriptorSets(device, &set_alloc, &block.set) != VK_SUCCESS) {
    return false;
  }
  const uint32_t ranges[kConstantBlocks] = {uint32_t(sizeof(DrawConstants)),
                                            kFloatConstantsRange, kFloatConstantsRange,
                                            kBoolLoopConstantsRange};
  VkDescriptorBufferInfo buffers[kConstantBlocks];
  VkWriteDescriptorSet writes[kConstantBlocks];
  for (uint32_t i = 0; i < kConstantBlocks; i++) {
    buffers[i] = {block.buffer, 0, ranges[i]};
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = block.set;
    writes[i].dstBinding = i;
    writes[i].descriptorCount = 1;
    writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writes[i].pBufferInfo = &buffers[i];
  }
  vk::vkUpdateDescriptorSets(device, kConstantBlocks, writes, 0, nullptr);
  ring.blocks.push_back(block);
  return true;
}

bool Passes::State::CreateDescriptors() {
  VkDevice device = context.device;
  VkDescriptorPoolSize sizes[] = {
      {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, frames_in_flight},
  };
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = frames_in_flight;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = sizes;
  if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &pool) != VK_SUCCESS) {
    return false;
  }
  constexpr VkShaderStageFlags kBoth = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

  // Set 0: empty (it held the vertex arena as a storage buffer until the
  // vertex shaders took vertex attributes; kept so the other sets keep
  // their numbers).
  VkDescriptorSetLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  if (vk::vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &shared_memory_layout) !=
      VK_SUCCESS) {
    return false;
  }
  // Set 1: system, vertex float, pixel float and bool/loop constants.
  VkDescriptorSetLayoutBinding constants[kConstantBlocks];
  for (uint32_t i = 0; i < kConstantBlocks; i++) {
    constants[i] = {i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, kBoth, nullptr};
  }
  layout_info.bindingCount = kConstantBlocks;
  layout_info.pBindings = constants;
  if (vk::vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &constants_layout) !=
      VK_SUCCESS) {
    return false;
  }

  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  alloc.descriptorPool = pool;
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &shared_memory_layout;
  if (vk::vkAllocateDescriptorSets(device, &alloc, &arena_set) != VK_SUCCESS) {
    return false;
  }

  for (Ring& ring : rings) {
    if (!CreateConstantBlock(ring)) {
      return false;
    }
  }

  gpu::SamplerDesc sampler_desc;
  sampler_desc.mag_filter = sampler_desc.min_filter = gpu::Filter::kLinear;
  sampler_desc.max_lod = VK_LOD_CLAMP_NONE;
  sampler = gpu::vulkan::Backend::Handle(backend->CreateSampler(sampler_desc));
  return sampler != VK_NULL_HANDLE;
}

bool Passes::State::CreateDummyTextures() {
  // White or zero, ready to sample.
  const uint8_t white[6][4] = {{255, 255, 255, 255}, {255, 255, 255, 255}, {255, 255, 255, 255},
                               {255, 255, 255, 255}, {255, 255, 255, 255}, {255, 255, 255, 255}};
  const uint8_t zero[6][4] = {};
  for (bool is_white : {true, false}) {
    const void* texels = is_white ? &white : &zero;
    gpu::ImageDesc desc;
    desc.format = gpu::Format::kR8G8B8A8Unorm;
    desc.usage = gpu::kImageSampled | gpu::kImageCopyDest;
    desc.type = gpu::ImageType::k2DArray;
    gpu::Image* array = NewImage(desc, texels);
    desc.type = gpu::ImageType::k3D;
    gpu::Image* volume = NewImage(desc, texels);
    desc.type = gpu::ImageType::kCube;
    gpu::Image* cube = NewImage(desc, texels);
    if (!array || !volume || !cube) {
      return false;
    }
    (is_white ? dummy_2d_array : zero_2d_array) = array;
    (is_white ? dummy_3d : zero_3d) = volume;
    (is_white ? dummy_cube : zero_cube) = cube;
  }
  return true;
}

VkDescriptorSetLayout Passes::State::GetTextureLayout(const TranslatedShader* shader,
                                                       VkShaderStageFlags stage) {
  VkDescriptorSetLayout* cached = nullptr;
  if (shader) {
    cached = &shader_texture_layouts[shader];
    if (*cached) {
      return *cached;
    }
  }
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
    if (cached) {
      *cached = layout;
    }
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
  if (cached) {
    *cached = layout;
  }
  return layout;
}

VkDescriptorSet Passes::State::GetTextureSet(const TranslatedShader* shader,
                                               VkDescriptorSetLayout layout, uint32_t stage) {
  LastTextureSet& last = last_texture_sets[stage];
  const uint32_t* fetch = regs.fetch;
  uint64_t generation = textures.generation();
  uint64_t sampler_generation = textures.sampler_generation();
  if (reuse_texture_sets && last.set && last.shader == shader && last.layout == layout &&
      last.generation == generation && last.sampler_generation == sampler_generation) {
    bool same = true;
    for (uint32_t mask = last.fetch_mask; mask && same; mask &= mask - 1) {
      uint32_t i = 6 * uint32_t(std::countr_zero(mask));
      same = !std::memcmp(last.fetch + i, fetch + i, 24);
    }
    if (same) {
      return last.set;
    }
  }
  uint64_t unkept = textures.unkept_answers();
  VkDescriptorSet set = BuildTextureSet(shader, layout);
  if (shader != last.shader) {
    last.shader = shader;
    last.fetch_mask = 0;
    if (shader) {
      for (const ShaderTexture& texture : shader->textures) {
        last.fetch_mask |= 1u << (texture.fetch_constant & 31);
      }
      for (const ShaderSampler& sampler : shader->samplers) {
        last.fetch_mask |= 1u << (sampler.fetch_constant & 31);
      }
    }
  }
  last.layout = layout;
  last.generation = generation;
  last.sampler_generation = sampler_generation;
  for (uint32_t mask = last.fetch_mask; mask; mask &= mask - 1) {
    uint32_t i = 6 * uint32_t(std::countr_zero(mask));
    std::memcpy(last.fetch + i, fetch + i, 24);
  }
  last.set = textures.unkept_answers() == unkept && !(hide_cubes && shader) ? set : VK_NULL_HANDLE;
  return set;
}

VkDescriptorSet Passes::State::BuildTextureSet(const TranslatedShader* shader,
                                                 VkDescriptorSetLayout layout) {
  // The views and samplers for the shader's bindings, from the current
  // fetch constants; placeholders for what can't be had.
  size_t texture_count = shader ? shader->textures.size() : 0;
  size_t sampler_count = shader ? shader->samplers.size() : 0;
  std::vector<VkDescriptorImageInfo>& infos = set_infos;
  infos.assign(texture_count + sampler_count, {});
  for (size_t i = 0; i < texture_count; i++) {
    const ShaderTexture& texture = shader->textures[i];
    const uint32_t* fetch =
        &regs.fetch[6 * texture.fetch_constant];
    if (log_texture_bindings) {
      uint32_t extra[4] = {uint32_t(texture.view_type), 0, 0, 0};
      CountBinding(true, texture.fetch_constant, fetch, extra);
    }
    VkImageView view = hide_cubes && texture.view_type == VK_IMAGE_VIEW_TYPE_CUBE
                           ? VK_NULL_HANDLE
                           : textures.GetView(fetch, texture.view_type,
                                                              texture.fetch_constant);
    bool mismatched = false;
    if (uint64_t total = textures.mismatched_total(); total != mismatched_seen) {
      mismatched = true;
      mismatched_seen = total;
    }
    // Which shaders sample a texture of another dimension than declared.
    if (mismatched && mismatch_lines.size() < 20 && base::InfoLogEnabled()) {
      std::string key = std::format(
          "{} shader {:016X} fetch constant {} declares view type {}; texture {:08X} format {} "
          "dimension {} {}x{}",
          shader->stage == ShaderStage::kVertex ? "vertex" : "pixel", shader->hash,
          texture.fetch_constant, int(texture.view_type), fetch[1] & 0xFFFFF000u, fetch[1] & 0x3F,
          (fetch[5] >> 9) & 3, (fetch[2] & 0x1FFF) + 1, ((fetch[2] >> 13) & 0x1FFF) + 1);
      if (mismatch_lines.insert(key).second) {
        ITC_LOG_INFO("native renderer: dimension mismatch: {} (frame {})", key, frames);
      }
    }
    if (!view && mismatched) {
      view = View(texture.view_type == VK_IMAGE_VIEW_TYPE_3D     ? zero_3d
                  : texture.view_type == VK_IMAGE_VIEW_TYPE_CUBE ? zero_cube
                                                                 : zero_2d_array);
    } else if (!view) {
      view = View(texture.view_type == VK_IMAGE_VIEW_TYPE_3D     ? dummy_3d
                  : texture.view_type == VK_IMAGE_VIEW_TYPE_CUBE ? dummy_cube
                                                                 : dummy_2d_array);
    }
    infos[i] = {VK_NULL_HANDLE, view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
  }
  for (size_t i = 0; i < sampler_count; i++) {
    const ShaderSampler& binding = shader->samplers[i];
    const uint32_t* fetch =
        &regs.fetch[6 * binding.fetch_constant];
    if (log_texture_bindings) {
      uint32_t extra[4] = {binding.mag_filter, binding.min_filter, binding.mip_filter,
                           binding.aniso_filter};
      CountBinding(false, binding.fetch_constant, fetch, extra);
    }
    VkSampler texture_sampler =
        textures.GetSampler(render::StageSampler(binding.fetch_constant), fetch);
    infos[texture_count + i] = {texture_sampler ? texture_sampler : sampler, VK_NULL_HANDLE,
                                VK_IMAGE_LAYOUT_UNDEFINED};
  }

  // One set per distinct contents a frame, keyed by the layout and each
  // binding's sampler, view and layout (the fields, not the structs' bytes,
  // which have padding).
  std::vector<uint64_t>& key = set_key;
  key.clear();
  key.push_back(uint64_t(reinterpret_cast<uintptr_t>(layout)));
  for (const VkDescriptorImageInfo& info : infos) {
    key.push_back(uint64_t(reinterpret_cast<uintptr_t>(info.sampler)));
    key.push_back(uint64_t(reinterpret_cast<uintptr_t>(info.imageView)));
    key.push_back(uint64_t(info.imageLayout));
  }
  auto [set, added] = texture_sets.FindOrAdd(key.data(), uint32_t(key.size()));
  if (log_texture_bindings) {
    binding_counts.sets++;
    binding_counts.sets_found += !added;
  }
  if (!added) {
    return *set;
  }
  VkDescriptorSetAllocateInfo alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  Ring& ring = rings[frame];
  alloc.descriptorPool = ring.texture_pools[ring.texture_pool];
  alloc.descriptorSetCount = 1;
  alloc.pSetLayouts = &layout;
  if (vk::vkAllocateDescriptorSets(context.device, &alloc, set) != VK_SUCCESS) {
    // The pool in use is full: on to the next, made if needed.
    *set = VK_NULL_HANDLE;
    ring.texture_pool++;
    if (ring.texture_pool == ring.texture_pools.size()) {
      if (!CreateTexturePool(ring)) {
        ring.texture_pool--;
        return VK_NULL_HANDLE;
      }
      ITC_LOG_INFO("native renderer: frame {} texture sets: pool {} added ({} sets each)",
                   frame, ring.texture_pools.size(), kTextureSetsPerPool);
    }
    alloc.descriptorPool = ring.texture_pools[ring.texture_pool];
    if (vk::vkAllocateDescriptorSets(context.device, &alloc, set) != VK_SUCCESS) {
      *set = VK_NULL_HANDLE;
      return VK_NULL_HANDLE;
    }
  }
  std::vector<VkWriteDescriptorSet>& writes = set_writes;
  writes.assign(infos.size(), {});
  for (size_t i = 0; i < infos.size(); i++) {
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = *set;
    writes[i].dstBinding = uint32_t(i);
    writes[i].descriptorCount = 1;
    writes[i].descriptorType =
        i < texture_count ? VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE : VK_DESCRIPTOR_TYPE_SAMPLER;
    writes[i].pImageInfo = &infos[i];
  }
  if (!writes.empty()) {
    vk::vkUpdateDescriptorSets(context.device, uint32_t(writes.size()), writes.data(), 0, nullptr);
  }
  return *set;
}

void Passes::State::CountBinding(bool view, uint32_t slot, const uint32_t* fetch,
                                 const uint32_t extra[4]) {
  TextureBindingCounts& c = binding_counts;
  uint64_t generation = textures.generation();
  uint32_t words[10];
  std::memcpy(words, fetch, 24);
  std::memcpy(words + 6, extra, 16);
  LastBinding& last = (view ? last_view : last_sampler)[slot & 31];
  bool same_words = last.valid && !std::memcmp(last.words, words, sizeof(words));
  // Samplers don't depend on textures: no generation.
  bool same = same_words && (!view || last.generation == generation);
  std::memcpy(last.words, words, sizeof(words));
  last.generation = generation;
  last.valid = true;
  uint64_t hash = base::Hash64(words, sizeof(words));
  if (view) {
    c.views++;
    c.view_same_slot += same;
    c.view_same_slot_words += same_words;
    auto [it, added] = views_seen.try_emplace(hash, generation);
    c.view_seen += !added && it->second == generation;
    it->second = generation;
    if (views_seen.size() > (1u << 20)) {
      views_seen.clear();
    }
  } else {
    c.samplers++;
    c.sampler_same_slot += same;
    c.sampler_seen += !samplers_seen.insert(hash).second;
  }
}

void Passes::State::CountDrawBindings(const TranslatedShader* vertex,
                                      const TranslatedShader* pixel) {
  TextureBindingCounts& c = binding_counts;
  const uint32_t* fetch = regs.fetch;
  uint64_t generation = textures.generation();
  LastDraw& last = last_draw;
  // Whether a stage's shader and the fetch constants it reads are as at the
  // last draw.
  auto stage_same = [&](const TranslatedShader* shader, const TranslatedShader* last_shader) {
    if (!last.valid || shader != last_shader) {
      return false;
    }
    if (!shader) {
      return true;
    }
    for (const ShaderTexture& texture : shader->textures) {
      uint32_t i = 6 * (texture.fetch_constant & 31);
      if (std::memcmp(last.fetch + i, fetch + i, 24)) {
        return false;
      }
    }
    for (const ShaderSampler& sampler : shader->samplers) {
      uint32_t i = 6 * (sampler.fetch_constant & 31);
      if (std::memcmp(last.fetch + i, fetch + i, 24)) {
        return false;
      }
    }
    return true;
  };
  bool vertex_same = stage_same(vertex, last.vertex);
  bool pixel_same = stage_same(pixel, last.pixel);
  bool same_generation = last.generation == generation;
  c.draws++;
  c.draws_textured += (vertex && (!vertex->textures.empty() || !vertex->samplers.empty())) ||
                      (pixel && (!pixel->textures.empty() || !pixel->samplers.empty()));
  c.draws_same_words += vertex_same && pixel_same;
  c.draws_same += vertex_same && pixel_same && same_generation;
  c.vertex_same += vertex_same && same_generation;
  c.pixel_same += pixel_same && same_generation;
  last.vertex = vertex;
  last.pixel = pixel;
  last.generation = generation;
  std::memcpy(last.fetch, fetch, sizeof(last.fetch));
  last.valid = true;
}

VkPipelineLayout Passes::State::GetPipelineLayout(VkDescriptorSetLayout vertex,
                                                    VkDescriptorSetLayout pixel) {
  VkPipelineLayout& layout = pipeline_layouts[{vertex, pixel}];
  if (!layout) {
    VkDescriptorSetLayout sets[4] = {shared_memory_layout, constants_layout, vertex, pixel};
    VkPipelineLayoutCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    info.setLayoutCount = 4;
    info.pSetLayouts = sets;
    vk::vkCreatePipelineLayout(context.device, &info, nullptr, &layout);
    // The same with the ID pass's push constant (its sets stay compatible).
    VkPushConstantRange push{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 4};
    info.pushConstantRangeCount = 1;
    info.pPushConstantRanges = &push;
    vk::vkCreatePipelineLayout(context.device, &info, nullptr, &id_layouts[layout]);
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
    if (shader->module && shader->stage == ShaderStage::kVertex) {
      VertexInput& input = vertex_inputs[shader->module];
      const auto& fetches = shader->vertex_fetches;
      for (uint32_t n = 0; n < fetches.size() && n < kMaxVertexFetches; n++) {
        uint32_t stride = 4 * fetches[n].stride_words;
        VkFormat format = RawWordsFormat(fetches[n].words);
        for (uint32_t g = 0; g < 3; g++) {
          uint32_t binding = shader->rectangle_list ? 3 * n + g : n;
          if (shader->rectangle_list || g == 0) {
            input.bindings.push_back(
                {binding, shader->rectangle_list ? 3 * stride : stride,
                 shader->rectangle_list ? VK_VERTEX_INPUT_RATE_INSTANCE
                                        : VK_VERTEX_INPUT_RATE_VERTEX});
          }
          input.attributes.push_back({3 * n + g, binding, format, 0});
        }
      }
    }
  }
  return shader->module;
}

VkPipeline Passes::State::GetPipeline(const PipelineKey& key) {
  auto [it, added] = pipelines.try_emplace(key, VK_NULL_HANDLE);
  if (!added) {
    return it->second;
  }
  ScopedCost cost(g_frame_costs.pipeline_ns);
  g_frame_costs.pipelines++;
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
  if (auto input = vertex_inputs.find(key.vertex); input != vertex_inputs.end()) {
    vertex_input.vertexBindingDescriptionCount = uint32_t(input->second.bindings.size());
    vertex_input.pVertexBindingDescriptions = input->second.bindings.data();
    vertex_input.vertexAttributeDescriptionCount = uint32_t(input->second.attributes.size());
    vertex_input.pVertexAttributeDescriptions = input->second.attributes.data();
  }
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
  raster.cullMode = vk_types::CullMode(gpu::CullMode(key.cull_mode));
  raster.frontFace = key.front_clockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
  raster.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample.rasterizationSamples = VkSampleCountFlagBits(key.samples);
  VkPipelineDepthStencilStateCreateInfo depth{
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
  depth.depthTestEnable = key.z_enable;
  depth.depthWriteEnable = key.z_write;
  depth.depthCompareOp = vk_types::CompareOp(gpu::CompareOp(key.z_func));
  VkPipelineColorBlendAttachmentState attachment{};
  attachment.blendEnable = key.blend_enable;
  attachment.srcColorBlendFactor = vk_types::BlendFactor(gpu::BlendFactor(key.src_color));
  attachment.dstColorBlendFactor = vk_types::BlendFactor(gpu::BlendFactor(key.dst_color));
  attachment.colorBlendOp = vk_types::BlendOp(gpu::BlendOp(key.color_op));
  attachment.srcAlphaBlendFactor = vk_types::BlendFactor(gpu::BlendFactor(key.src_alpha));
  attachment.dstAlphaBlendFactor = vk_types::BlendFactor(gpu::BlendFactor(key.dst_alpha));
  attachment.alphaBlendOp = vk_types::BlendOp(gpu::BlendOp(key.alpha_op));
  attachment.colorWriteMask = key.write_mask;
  VkPipelineColorBlendStateCreateInfo blend{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
  bool has_color = key.color_format != VK_FORMAT_UNDEFINED;
  blend.attachmentCount = has_color ? 1 : 0;
  blend.pAttachments = &attachment;
  // Depth bias is set per draw.
  raster.depthBiasEnable = key.depth;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
                                     VK_DYNAMIC_STATE_DEPTH_BIAS, VK_DYNAMIC_STATE_BLEND_CONSTANTS};
  VkPipelineDynamicStateCreateInfo dynamic{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic.dynamicStateCount = 4;
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
  if (VkResult result = vk::vkCreateGraphicsPipelines(context.device, pipeline_cache, 1, &info,
                                                      nullptr, &it->second);
      result != VK_SUCCESS) {
    ITC_LOG_ERROR("native renderer: vkCreateGraphicsPipelines: {}", int(result));
    it->second = VK_NULL_HANDLE;
  } else {
    stats.pipelines++;
    gpu_memory::Created(gpu_memory::Object::kPipeline);
  }
  return it->second;
}

bool Passes::State::Reserve(VkDeviceSize size, uint32_t& offset, uint8_t*& data) {
  VkDeviceSize alignment = std::max<VkDeviceSize>(context.min_uniform_buffer_offset_alignment, 16);
  VkDeviceSize start = (ring_offset + alignment - 1) / alignment * alignment;
  if (start + size > kConstantBlockSize) {
    return false;
  }
  ring_offset = start + size;
  offset = uint32_t(start);
  data = constant_block().mapped + start;
  return true;
}

bool Passes::State::ReserveDrawConstants() {
  VkDeviceSize alignment = std::max<VkDeviceSize>(context.min_uniform_buffer_offset_alignment, 16);
  constexpr VkDeviceSize kDrawConstants =
      sizeof(DrawConstants) + 2 * kFloatConstantsRange + kBoolLoopConstantsRange;
  if (ring_offset + kDrawConstants + kConstantBlocks * alignment <= kConstantBlockSize) {
    return true;
  }
  Ring& ring = rings[frame];
  if (ring.block + 1 == ring.blocks.size()) {
    if (!CreateConstantBlock(ring)) {
      return false;
    }
    ITC_LOG_INFO("native renderer: frame {} constants: block {} added ({} MB each)", frame,
                 ring.blocks.size(), kConstantBlockSize >> 20);
  }
  ring.block++;
  ring_offset = 0;
  std::fill(std::begin(last_constants.valid), std::end(last_constants.valid), false);
  return true;
}

bool Passes::State::PlaceConstants(uint32_t block, const void* bytes, uint32_t size,
                                   uint32_t range, uint32_t& offset, bool copied, bool same) {
  LastConstants& last = last_constants;
  uint8_t* copy = last.bytes[block];
  if (!copied) {
    same = last.valid[block] && last.sizes[block] == size && !std::memcmp(copy, bytes, size);
  }
  // (A block with nothing to read can point anywhere.)
  if (reuse_constants && last.valid[block] &&
      (size == 0 || (same && last.sizes[block] == size))) {
    offset = last.offsets[block];
    stats.constants_reused[block]++;
    return true;
  }
  uint8_t* data;
  if (!Reserve(range, offset, data)) {
    last.valid[block] = false;
    return false;
  }
  if (!copied) {
    std::memcpy(copy, bytes, size);
  }
  std::memcpy(data, copy, size);
  last.valid[block] = true;
  last.offsets[block] = offset;
  last.sizes[block] = size;
  stats.constant_bytes += size;
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
  if (color) {
    RestoreContents(commands, *color);
  }
  if (depth) {
    RestoreContents(commands, *depth);
  }
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

Target* Passes::State::GetTarget(VkCommandBuffer commands, uint32_t object,
                                 bool depth) {
  Surface surface = ReadSurface(object);
  VkFormat format = depth ? kDepthFormat : ColorFormat(surface.format);
  if (format == VK_FORMAT_UNDEFINED || !surface.pitch) {
    return nullptr;
  }
  // A target an image, by the surface object the game made it as, its size,
  // samples and format. Surfaces drawn while tiling get targets of their
  // own, as large as the tiles together.
  std::array<uint32_t, 7> key{uint32_t(depth), object, surface.width, surface.height,
                              surface.msaa, uint32_t(format), tiling ? 1u : 0u};
  auto [it, added] = targets.try_emplace(key);
  Target& t = it->second;
  if (!added) {
    return t.image.image ? &t : nullptr;
  }
  t.depth = depth;
  t.surface = surface;
  t.format = format;
  t.samples = SampleCount(surface.msaa);
  t.guest_extent = {std::clamp(surface.width, 1u, 8192u), std::clamp(surface.height, 1u, 8192u)};
  if (tiling) {
    t.guest_extent = {std::max(t.guest_extent.width, tiling_extent.width),
                      std::max(t.guest_extent.height, tiling_extent.height)};
  }
  t.scale = IsMainViewSurface(surface) ? main_view_scale : scale;
  t.extent = ScaledBy(t.guest_extent, t.scale);
  if (!CreateTargetImage(t)) {
    ITC_LOG_ERROR("native renderer: couldn't create a {}x{} target", t.extent.width,
                  t.extent.height);
    t.image = {};
    return nullptr;
  }
  ITC_LOG_INFO("native renderer: {} target: surface {:08X}, pitch {}, MSAA {}, format {}; {}x{}{} "
               "({}x{} drawn)",
               depth ? "depth" : "color", object, surface.pitch, surface.msaa,
               surface.format, t.guest_extent.width, t.guest_extent.height,
               tiling ? " (tiled)" : "", t.extent.width, t.extent.height);
  stats.targets++;
  VkClearValue initial{};
  initial.depthStencil = {1.0f, 0};
  if (!depth) {
    initial.color = {{0.0f, 0.0f, 0.0f, 0.0f}};
  }
  ClearTarget(commands, t, initial);
  return &t;
}

bool Passes::State::CreateTargetImage(Target& t) {
  VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                            VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                            (t.depth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
                                     : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
  if (!CreateImage(t.image, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, t.format,
                   {t.extent.width, t.extent.height, 1}, 1, usage, t.aspects(), 0, t.samples)) {
    return false;
  }
  t.sample_view = t.image.view;
  if (t.depth) {
    // Its depth alone, for resolving and sampling.
    VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view.image = t.image.image;
    view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view.format = t.format;
    view.subresourceRange = {VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1};
    vk::vkCreateImageView(context.device, &view, nullptr, &t.sample_view);
  }
  return true;
}

void Passes::State::RestoreContents(VkCommandBuffer commands, Target& t) {
  if (!t.handed_image) {
    return;
  }
  VkImage source = t.handed_image;
  t.handed_image = VK_NULL_HANDLE;
  if (!textures.HoldsHanded(t.handed_fetch, t.handed_scale, source)) {
    return;  // gone (a race's end): as after a clear
  }
  EndRendering(commands);
  // The destination keeps its image in SHADER_READ_ONLY_OPTIMAL.
  VkImageLayout source_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  Transition(commands, source, t.aspects(), source_layout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  Transition(commands, t.image.image, t.aspects(), t.layout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  VkImageCopy region{};
  region.srcSubresource = {t.aspects(), 0, 0, 1};
  region.dstSubresource = region.srcSubresource;
  region.extent = {t.extent.width, t.extent.height, 1};
  vk::vkCmdCopyImage(commands, source, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, t.image.image,
                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  Transition(commands, source, t.aspects(), source_layout,
             VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  recorded.valid = false;
  stats.resolves_copied_back++;
  if (logged_copy_backs.insert(uint64_t(t.surface.pitch) << 32 | t.handed_fetch[1]).second) {
    ITC_LOG_INFO("native renderer: target (pitch {}, format {}) drawn or read again without a "
                 "clear after handing its image to {:08X}: copied back",
                 t.surface.pitch, t.surface.format, t.handed_fetch[1] & 0xFFFFF000u);
  }
}

void Passes::State::ClearTarget(VkCommandBuffer commands, Target& target,
                                const VkClearValue& value, VkImageAspectFlags aspects) {
  EndRendering(commands);
  if (!aspects || aspects == target.aspects()) {
    target.handed_image = VK_NULL_HANDLE;  // all of it cleared
  }
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
  VkPushConstantRange push{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 20};
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
  gpu::SamplerDesc sampler_desc;
  sampler_desc.address[0] = sampler_desc.address[1] = sampler_desc.address[2] =
      gpu::AddressMode::kClampToEdge;
  resolve_sampler = gpu::vulkan::Backend::Handle(backend->CreateSampler(sampler_desc));
  if (!resolve_sampler) {
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

bool Passes::State::CreateGammaResources() {
  VkDevice device = context.device;
  VkBufferCreateInfo buffer_info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  buffer_info.size = 256 * sizeof(uint32_t);
  buffer_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  if (vk::vkCreateBuffer(device, &buffer_info, nullptr, &gamma_table) != VK_SUCCESS) {
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetBufferMemoryRequirements(device, gamma_table, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = FindMemoryType(memory_properties, requirements.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &gamma_table_memory) != VK_SUCCESS ||
      vk::vkBindBufferMemory(device, gamma_table, gamma_table_memory, 0) != VK_SUCCESS) {
    return false;
  }
  gpu_memory::Allocated(gpu_memory::Kind::kOther, alloc.allocationSize);
  VkDescriptorSetLayoutBinding bindings[2] = {
      {0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr},
      {1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr}};
  VkDescriptorSetLayoutCreateInfo set_info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  set_info.bindingCount = 2;
  set_info.pBindings = bindings;
  if (vk::vkCreateDescriptorSetLayout(device, &set_info, nullptr, &gamma_set_layout) !=
      VK_SUCCESS) {
    return false;
  }
  VkPushConstantRange push{VK_SHADER_STAGE_COMPUTE_BIT, 0, 8};
  VkPipelineLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
  layout_info.setLayoutCount = 1;
  layout_info.pSetLayouts = &gamma_set_layout;
  layout_info.pushConstantRangeCount = 1;
  layout_info.pPushConstantRanges = &push;
  if (vk::vkCreatePipelineLayout(device, &layout_info, nullptr, &gamma_layout) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorPoolSize sizes[2] = {{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1},
                                   {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1}};
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = 2;
  pool_info.pPoolSizes = sizes;
  if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &gamma_pool) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  set_alloc.descriptorPool = gamma_pool;
  set_alloc.descriptorSetCount = 1;
  set_alloc.pSetLayouts = &gamma_set_layout;
  if (vk::vkAllocateDescriptorSets(device, &set_alloc, &gamma_set) != VK_SUCCESS) {
    return false;
  }
  // `shown` is only ever written by transfers and this pass, and lives as
  // long as the set: the set is written once.
  VkDescriptorImageInfo image_info{VK_NULL_HANDLE, shown.view, VK_IMAGE_LAYOUT_GENERAL};
  VkDescriptorBufferInfo table_info{gamma_table, 0, VK_WHOLE_SIZE};
  VkWriteDescriptorSet writes[2] = {{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET},
                                    {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET}};
  writes[0].dstSet = writes[1].dstSet = gamma_set;
  writes[0].dstBinding = 0;
  writes[0].descriptorCount = 1;
  writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
  writes[0].pImageInfo = &image_info;
  writes[1].dstBinding = 1;
  writes[1].descriptorCount = 1;
  writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  writes[1].pBufferInfo = &table_info;
  vk::vkUpdateDescriptorSets(device, 2, writes, 0, nullptr);
  VkShaderModuleCreateInfo module_info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
  module_info.codeSize = sizeof(k_gamma_comp);
  module_info.pCode = k_gamma_comp;
  VkShaderModule module;
  if (vk::vkCreateShaderModule(device, &module_info, nullptr, &module) != VK_SUCCESS) {
    return false;
  }
  VkComputePipelineCreateInfo info{VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};
  info.stage = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
  info.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
  info.stage.module = module;
  info.stage.pName = "main";
  info.layout = gamma_layout;
  VkResult result =
      vk::vkCreateComputePipelines(device, pipeline_cache, 1, &info, nullptr, &gamma_pipeline);
  vk::vkDestroyShaderModule(device, module, nullptr);
  if (result != VK_SUCCESS) {
    gamma_pipeline = VK_NULL_HANDLE;
    return false;
  }
  ITC_LOG_INFO("native renderer: --xbox_gamma={}: the Xbox 360's gamma table for that display is "
               "applied to the frame",
               ITC_FLAG(xbox_gamma));
  return true;
}

void Passes::State::ApplyGamma(VkCommandBuffer commands) {
  if (!gamma_ready) {  // the first frame: the table into its buffer
    uint32_t table[256];
    std::copy(gamma_values, gamma_values + 256, table);
    vk::vkCmdUpdateBuffer(commands, gamma_table, 0, sizeof(table), table);
    VkMemoryBarrier barrier{VK_STRUCTURE_TYPE_MEMORY_BARRIER};
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 1, &barrier, 0, nullptr, 0,
                             nullptr);
    gamma_ready = true;
  }
  Transition(commands, shown.image, VK_IMAGE_ASPECT_COLOR_BIT, shown_layout,
             VK_IMAGE_LAYOUT_GENERAL);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_COMPUTE, gamma_pipeline);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_COMPUTE, gamma_layout, 0, 1,
                              &gamma_set, 0, nullptr);
  int32_t extent[2] = {int32_t(shown_extent.width), int32_t(shown_extent.height)};
  vk::vkCmdPushConstants(commands, gamma_layout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(extent),
                         extent);
  vk::vkCmdDispatch(commands, (shown_extent.width + 7) / 8, (shown_extent.height + 7) / 8, 1);
}

VkPipeline Passes::State::GetResolvePipeline(VkFormat format, bool multisample) {
  auto [it, added] = resolve_pipelines.try_emplace({format, multisample}, VK_NULL_HANDLE);
  if (!added) {
    return it->second;
  }
  it->second = MakeFullscreenPipeline(multisample ? resolve_pixel_ms : resolve_pixel,
                                      resolve_layout, format, VK_SAMPLE_COUNT_1_BIT);
  if (!it->second) {
    ITC_LOG_ERROR("native renderer: couldn't create a resolve pipeline for format {}",
                  int(format));
  }
  return it->second;
}

VkPipeline Passes::State::MakeFullscreenPipeline(VkShaderModule pixel, VkPipelineLayout layout,
                                                 VkFormat format, VkSampleCountFlagBits samples) {
  VkPipelineShaderStageCreateInfo stages[2] = {};
  for (VkPipelineShaderStageCreateInfo& stage : stages) {
    stage = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    stage.pName = "main";
  }
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = resolve_vertex;
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
  if (vk::vkCreateGraphicsPipelines(context.device, pipeline_cache, 1, &info, nullptr,
                                    &pipeline) != VK_SUCCESS) {
    return VK_NULL_HANDLE;
  }
  return pipeline;
}

bool Passes::State::ResolveInto(VkCommandBuffer commands, Target& source,
                                uint32_t destination, const int32_t* rect,
                                const int32_t* point, uint32_t flags) {
  // The destination texture's fetch constant (+28..+48), its addresses
  // made physical as SetTexture makes them.
  uint32_t fetch[6];
  for (uint32_t k = 0; k < 6; k++) {
    fetch[k] = guest::LoadU32(destination + 28 + 4 * k);
  }
  fetch[1] = (fetch[1] & 0xFFFu) | GpuPhysicalAddress(fetch[1] & 0xFFFFF000u);
  fetch[5] = (fetch[5] & 0xFFFu) | GpuPhysicalAddress(fetch[5] & 0xFFFFF000u);
  RestoreContents(commands, source);
  TextureCache::ResolveDestination dest;
  if (!textures.GetResolveDestination(fetch, source.scale, dest)) {
    return false;
  }
  // Source rectangle (left, top, right, bottom) and destination point.
  int32_t left = 0, top = 0, right = int32_t(dest.width), bottom = int32_t(dest.height);
  if (rect) {
    left = rect[0];
    top = rect[1];
    right = rect[2];
    bottom = rect[3];
  }
  int32_t x = point ? point[0] : 0, y = point ? point[1] : 0;
  left = std::max(left, 0);
  top = std::max(top, 0);
  right = std::min(right, int32_t(source.guest_extent.width));
  bottom = std::min(bottom, int32_t(source.guest_extent.height));
  int32_t width = std::min(right - left, int32_t(dest.width) - x);
  int32_t height = std::min(bottom - top, int32_t(dest.height) - y);
  uint64_t log_key = uint64_t(reinterpret_cast<uintptr_t>(&source)) * 31 + (fetch[1] >> 12);
  if (logged_resolves.insert(log_key).second) {
    ITC_LOG_INFO("native renderer: resolve from {} target (EDRAM base {}, pitch {}, MSAA {}, "
                 "format {}) rect {},{}-{},{} to {:08X} at {},{}, flags {:08X}",
                 source.depth ? "depth" : "color", source.surface.edram_base,
                 source.surface.pitch, source.surface.msaa, source.surface.format, left, top, right,
                 bottom, fetch[1] & 0xFFFFF000u, x, y, flags);
  }
  if (width <= 0 || height <= 0 || x < 0 || y < 0) {
    return false;
  }
  // In the images' pixels: each edge scaled and rounded, the same in the
  // source and the destination (made at the source's scale), so the copy
  // stays one to one.
  float source_scale = source.scale, dest_scale = dest.scale;
  int32_t host_x = ScaledBy(x, dest_scale), host_y = ScaledBy(y, dest_scale);
  int32_t host_left = ScaledBy(left, source_scale), host_top = ScaledBy(top, source_scale);
  int32_t host_width =
      std::min({ScaledBy(x + width, dest_scale) - host_x,
                ScaledBy(left + width, source_scale) - host_left,
                int32_t(dest.host_width) - host_x, int32_t(source.extent.width) - host_left});
  int32_t host_height =
      std::min({ScaledBy(y + height, dest_scale) - host_y,
                ScaledBy(top + height, source_scale) - host_top,
                int32_t(dest.host_height) - host_y, int32_t(source.extent.height) - host_top});
  if (host_width <= 0 || host_height <= 0) {
    return false;
  }
  // Red and blue swapped as the library's Resolve sets RB_COPY_DEST_INFO's
  // copy_dest_swap: when the destination's swizzle takes X from another
  // component than the first (bits 1-3 of fetch word 3), so sampling it
  // through the swizzle swaps them back.
  bool swap = !source.depth && ((fetch[3] >> 1) & 7) != 0;
  // The exponent bias: flags bits 26-31, signed (likely; 0xF4000000 is -3
  // on the main view's HDR copies).
  int32_t bias = int32_t(flags) >> 26;
  // A whole-image copy from a single-sample color target: the pass has drawn
  // into its texture. The target's image is handed to the destination, and
  // what the copy did is left to the readers (the views' swizzle, the
  // reader scale); the target draws next into the image the destination
  // held (or a new one).
  bool whole = host_x == 0 && host_y == 0 && host_left == 0 && host_top == 0 &&
               uint32_t(host_width) == dest.host_width &&
               uint32_t(host_height) == dest.host_height &&
               dest.host_width == source.extent.width && dest.host_height == source.extent.height;
  // A depth target's image is sampled as depth (the copy is the depth as
  // is, into an R32F texture), where its format can be filtered.
  bool color_hand_over = !source.depth &&
                         FormatComponents(source.format) == FormatComponents(dest.native_format) &&
                         FormatComponents(source.format) != 0;
  bool depth_hand_over = source.depth && dest.native_format == VK_FORMAT_R32_SFLOAT &&
                         bias == 0 && depth_filterable;
  if (hand_over_resolves && whole && !dest.copied && source.samples == VK_SAMPLE_COUNT_1_BIT &&
      (color_hand_over || depth_hand_over)) {
    float reader_scale = 0.0f;
    float factor = std::ldexp(1.0f, bias);
    if (IsUnorm(dest.native_format)) {
      // What the normalized format kept: 0-1 (nothing to do for a
      // normalized target and no bias).
      if (bias != 0 || !IsUnorm(source.format)) {
        reader_scale = factor;
      }
    } else if (bias != 0) {
      reader_scale = -factor;
    }
    EndRendering(commands);
    Transition(commands, source.image.image, source.aspects(), source.layout,
               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    TextureCache::HandedImage handed{source.image.image, source.image.memory, source.image.view,
                                     source.format};
    if (source.depth) {
      handed.aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
      handed.depth_view = source.sample_view;
    }
    TextureCache::HandedImage back =
        textures.HandOver(fetch, source.scale, handed, swap, reader_scale);
    if (back.image) {
      source.image = {back.image, back.memory, back.view};
      source.sample_view = source.depth ? back.depth_view : back.view;
      source.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    } else {
      if (logged_new_target_images < 40) {
        logged_new_target_images++;
        ITC_LOG_INFO("native renderer: hand-over to {:08X} ({}x{}, held its own {} format {}) "
                     "from target {:08X} format {}: a new target image",
                     fetch[1] & 0xFFFFF000u, dest.host_width, dest.host_height,
                     dest.handed ? "no:" : "yes:", int(dest.format), source.surface.pitch,
                     int(source.format));
      }
      source.image = {};
      source.layout = VK_IMAGE_LAYOUT_UNDEFINED;
      if (!CreateTargetImage(source)) {
        ITC_LOG_ERROR("native renderer: couldn't create a {}x{} target", source.extent.width,
                      source.extent.height);
      }
      stats.targets++;
    }
    source.handed_image = handed.image;
    std::copy(fetch, fetch + 6, source.handed_fetch);
    source.handed_scale = source.scale;
    recorded.valid = false;
    stats.resolves_handed++;
    Resolved& resolved = frame_resolves.emplace_back();
    resolved = {handed.image, {dest.host_width, dest.host_height}, fetch[1] & 0xFFFFF000u,
                fetch[1] & 0x3F, flags,
                source.depth ? VK_FORMAT_R32_SFLOAT : source.format};
    std::copy(fetch, fetch + 6, resolved.fetch);
    resolved.handed = true;
    resolved.scale = source.scale;
    resolved.aspect = source.depth ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    return true;
  }
  if (!dest.copied && !textures.RestoreResolveDestination(fetch, source.scale, dest)) {
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
  VkRect2D area{{host_x, host_y}, {uint32_t(host_width), uint32_t(host_height)}};
  VkRenderingAttachmentInfo attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  attachment.imageView = dest.view;
  attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  info.renderArea = area;
  info.layerCount = 1;
  info.colorAttachmentCount = 1;
  info.pColorAttachments = &attachment;
  vk::vkCmdBeginRendering(commands, &info);
  VkViewport viewport{float(host_x), float(host_y), float(host_width), float(host_height), 0.0f,
                      1.0f};
  vk::vkCmdSetViewport(commands, 0, 1, &viewport);
  vk::vkCmdSetScissor(commands, 0, 1, &area);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, resolve_layout, 0, 1,
                              &set, 0, nullptr);
  struct {
    int32_t offset[2];
    int32_t samples;
    float scale;
    int32_t swap_red_blue;
  } constants{{host_left - host_x, host_top - host_y},
              source.depth ? 1 : int32_t(source.samples),
              std::ldexp(1.0f, bias), swap ? 1 : 0};
  vk::vkCmdPushConstants(commands, resolve_layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(constants), &constants);
  vk::vkCmdDraw(commands, 3, 1, 0, 0);
  vk::vkCmdEndRendering(commands);
  recorded.valid = false;
  Transition(commands, dest.image, VK_IMAGE_ASPECT_COLOR_BIT, dest_layout,
             VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  Resolved& resolved = frame_resolves.emplace_back();
  resolved = {dest.image, {dest.host_width, dest.host_height}, fetch[1] & 0xFFFFF000u,
              fetch[1] & 0x3F, flags, dest.format};
  std::copy(fetch, fetch + 6, resolved.fetch);
  if (log_glow && dest.width == 256 && dest.height == 144 && !(flags & 0x100)) {
    uint32_t address = resolved.address & 0x1FFFF000u;
    glow_frame.addresses.push_back(address);
    if (address == glow_frame.cross_fade_texture) {
      // The texture the cross-fade reads: the history.
      glow_frame.history_resolves++;
      glow_frame.history_address = address;
    }
  }
  return true;
}

bool Passes::State::CreateMovieResources(VkExtent2D extent) {
  VkDevice device = context.device;
  if (!movie_layout) {
    VkDescriptorSetLayoutBinding bindings[3];
    for (uint32_t i = 0; i < 3; i++) {
      bindings[i] = {i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1,
                     VK_SHADER_STAGE_FRAGMENT_BIT, nullptr};
    }
    VkDescriptorSetLayoutCreateInfo set_info{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    set_info.bindingCount = 3;
    set_info.pBindings = bindings;
    VkPushConstantRange push{VK_SHADER_STAGE_FRAGMENT_BIT, 0, 16};
    VkPipelineLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    layout_info.setLayoutCount = 1;
    layout_info.pSetLayouts = &movie_set_layout;
    layout_info.pushConstantRangeCount = 1;
    layout_info.pPushConstantRanges = &push;
    VkShaderModuleCreateInfo module_info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    module_info.codeSize = sizeof(k_movie_frag);
    module_info.pCode = k_movie_frag;
    // Bilinear, as the movie library's sampler states.
    gpu::SamplerDesc sampler_desc;
    sampler_desc.mag_filter = sampler_desc.min_filter = gpu::Filter::kLinear;
    sampler_desc.address[0] = sampler_desc.address[1] = sampler_desc.address[2] =
        gpu::AddressMode::kClampToEdge;
    movie_sampler = gpu::vulkan::Backend::Handle(backend->CreateSampler(sampler_desc));
    VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                              3 * frames_in_flight};
    VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    pool_info.maxSets = frames_in_flight;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &size;
    if (vk::vkCreateDescriptorSetLayout(device, &set_info, nullptr, &movie_set_layout) !=
            VK_SUCCESS ||
        vk::vkCreatePipelineLayout(device, &layout_info, nullptr, &movie_layout) != VK_SUCCESS ||
        vk::vkCreateShaderModule(device, &module_info, nullptr, &movie_pixel) != VK_SUCCESS ||
        !movie_sampler ||
        vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &movie_pool) != VK_SUCCESS) {
      return false;
    }
  }
  // Another size than the last movie's (none in the game: all are
  // 1280x720): the old planes are left, as the renderer leaves its other
  // images at exit.
  movie_uploads.assign(frames_in_flight, {});
  movie_extent = extent;
  const VkExtent2D sizes[3] = {extent,
                               {extent.width / 2, extent.height / 2},
                               {extent.width / 2, extent.height / 2}};
  const VkDeviceSize staging_size = VkDeviceSize(extent.width) * extent.height * 3 / 2;
  for (MovieUpload& upload : movie_uploads) {
    for (uint32_t i = 0; i < 3; i++) {
      if (!CreateImage(upload.planes[i], VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D,
                       VK_FORMAT_R8_UNORM, {sizes[i].width, sizes[i].height, 1}, 1,
                       VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                       VK_IMAGE_ASPECT_COLOR_BIT, 0, VK_SAMPLE_COUNT_1_BIT)) {
        return false;
      }
    }
    VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    info.size = staging_size;
    info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vk::vkCreateBuffer(device, &info, nullptr, &upload.staging) != VK_SUCCESS) {
      return false;
    }
    VkMemoryRequirements requirements;
    vk::vkGetBufferMemoryRequirements(device, upload.staging, &requirements);
    VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc.allocationSize = requirements.size;
    alloc.memoryTypeIndex =
        FindMemoryType(memory_properties, requirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (alloc.memoryTypeIndex == UINT32_MAX ||
        vk::vkAllocateMemory(device, &alloc, nullptr, &upload.staging_memory) != VK_SUCCESS ||
        vk::vkBindBufferMemory(device, upload.staging, upload.staging_memory, 0) != VK_SUCCESS ||
        vk::vkMapMemory(device, upload.staging_memory, 0, VK_WHOLE_SIZE, 0,
                        reinterpret_cast<void**>(&upload.mapped)) != VK_SUCCESS) {
      return false;
    }
    gpu_memory::Allocated(gpu_memory::Kind::kOther, alloc.allocationSize);
    VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    set_alloc.descriptorPool = movie_pool;
    set_alloc.descriptorSetCount = 1;
    set_alloc.pSetLayouts = &movie_set_layout;
    if (vk::vkAllocateDescriptorSets(device, &set_alloc, &upload.set) != VK_SUCCESS) {
      return false;
    }
    VkDescriptorImageInfo images[3];
    VkWriteDescriptorSet writes[3];
    for (uint32_t i = 0; i < 3; i++) {
      images[i] = {movie_sampler, upload.planes[i].view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
      writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
      writes[i].dstSet = upload.set;
      writes[i].dstBinding = i;
      writes[i].descriptorCount = 1;
      writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      writes[i].pImageInfo = &images[i];
    }
    vk::vkUpdateDescriptorSets(device, 3, writes, 0, nullptr);
  }
  return true;
}

VkPipeline Passes::State::GetMoviePipeline(VkFormat format, VkSampleCountFlagBits samples) {
  auto [it, added] = movie_pipelines.try_emplace({format, samples}, VK_NULL_HANDLE);
  if (added) {
    it->second = MakeFullscreenPipeline(movie_pixel, movie_layout, format, samples);
    if (!it->second) {
      ITC_LOG_ERROR("native renderer: couldn't create a movie pipeline for format {}",
                    int(format));
    }
  }
  return it->second;
}

void Passes::DrawMovie(VkCommandBuffer commands, uint32_t device,
                       const MoviePlanes& planes) {
  State& s = *state_;
  if (s.movie_failed || !planes.width || !planes.height) {
    return;
  }
  if (planes.width != s.movie_extent.width || planes.height != s.movie_extent.height) {
    if (!s.CreateMovieResources({planes.width, planes.height})) {
      ITC_LOG_ERROR("native renderer: couldn't make the movies' resources; movies aren't drawn");
      s.movie_failed = true;
      return;
    }
    ITC_LOG_INFO("native renderer: movies drawn from {}x{} planes", planes.width, planes.height);
  }
  uint32_t object = render::BoundTargets().color[0];
  Target* target = object ? s.GetTarget(commands, object, false) : nullptr;
  if (!target) {
    return;
  }
  VkPipeline pipeline = s.GetMoviePipeline(target->format, target->samples);
  if (!pipeline) {
    return;
  }
  State::MovieUpload& upload = s.movie_uploads[s.frame];
  // The planes into this frame's buffer, then into its images.
  const VkExtent2D sizes[3] = {{planes.width, planes.height},
                               {planes.width / 2, planes.height / 2},
                               {planes.width / 2, planes.height / 2}};
  const uint8_t* sources[3] = {planes.y, planes.u, planes.v};
  VkDeviceSize offsets[3];
  VkDeviceSize offset = 0;
  for (uint32_t i = 0; i < 3; i++) {
    offsets[i] = offset;
    VkDeviceSize bytes = VkDeviceSize(sizes[i].width) * sizes[i].height;
    std::memcpy(upload.mapped + offset, sources[i], bytes);
    offset += bytes;
  }
  s.EndRendering(commands);
  for (uint32_t i = 0; i < 3; i++) {
    Transition(commands, upload.planes[i].image, VK_IMAGE_ASPECT_COLOR_BIT, upload.layouts[i],
               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    VkBufferImageCopy copy{};
    copy.bufferOffset = offsets[i];
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {sizes[i].width, sizes[i].height, 1};
    vk::vkCmdCopyBufferToImage(commands, upload.staging, upload.planes[i].image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
    Transition(commands, upload.planes[i].image, VK_IMAGE_ASPECT_COLOR_BIT, upload.layouts[i],
               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  }
  // The library's rectangle: the target's top-left 1280x720 guest pixels.
  int32_t width = std::min(State::ScaledBy(1280, target->scale), int32_t(target->extent.width));
  int32_t height = std::min(State::ScaledBy(720, target->scale), int32_t(target->extent.height));
  if (width <= 0 || height <= 0) {
    return;
  }
  s.RestoreContents(commands, *target);
  Transition(commands, target->image.image, target->aspects(), target->layout,
             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  VkRect2D area{{0, 0}, {uint32_t(width), uint32_t(height)}};
  VkRenderingAttachmentInfo attachment{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  attachment.imageView = target->image.view;
  attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
  attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  info.renderArea = area;
  info.layerCount = 1;
  info.colorAttachmentCount = 1;
  info.pColorAttachments = &attachment;
  vk::vkCmdBeginRendering(commands, &info);
  VkViewport viewport{0.0f, 0.0f, float(width), float(height), 0.0f, 1.0f};
  vk::vkCmdSetViewport(commands, 0, 1, &viewport);
  vk::vkCmdSetScissor(commands, 0, 1, &area);
  vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, s.movie_layout, 0, 1,
                              &upload.set, 0, nullptr);
  float constants[4] = {0.0f, 0.0f, float(width), float(height)};
  vk::vkCmdPushConstants(commands, s.movie_layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                         sizeof(constants), constants);
  vk::vkCmdDraw(commands, 3, 1, 0, 0);
  vk::vkCmdEndRendering(commands);
  s.recorded.valid = false;
  s.stats.movie_draws++;
}

Passes::Passes() : state_(std::make_unique<State>()) {}
Passes::~Passes() = default;

bool Passes::Init(gpu::vulkan::Backend& backend, uint32_t frames_in_flight,
                  float resolution_scale) {
  State& s = *state_;
  s.backend = &backend;
  const DeviceContext& context = backend.context();
  s.context = context;
  s.scale = resolution_scale > 0.0f ? resolution_scale : 1.0f;
  s.main_view_scale =
      s.scale == 1.0f ? 1.0f : s.scale * float(kShownWidth) / float(kMainViewWidth);
  s.shown_extent = s.Scaled(VkExtent2D{kShownWidth, kShownHeight});
  s.main_view_extent = State::ScaledBy(VkExtent2D{kMainViewWidth, kMainViewHeight},
                                       s.main_view_scale);
  ITC_LOG_INFO("native renderer: resolution scale {}: frame {}x{}, 3D view {}x{} (scale {})",
               s.scale, s.shown_extent.width, s.shown_extent.height, s.main_view_extent.width,
               s.main_view_extent.height, s.main_view_scale);
  s.frames_in_flight = frames_in_flight;
  const std::string& log_level = ITC_FLAG(log_level);
  s.collect_interfaces = log_level == "trace" || log_level == "debug" || log_level == "info";
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &s.memory_properties);
  if (!s.arena.Init(context, frames_in_flight)) {
    return false;
  }
  const std::string& gamma = ITC_FLAG(xbox_gamma);
  if (gamma == "hdtv") {
    s.gamma_values = kXboxGammaHdtv;
  } else if (gamma == "power") {
    s.gamma_values = kXboxGammaPower;
  } else if (gamma != "off") {
    ITC_LOG_WARN("native renderer: --xbox_gamma={}: not off, hdtv or power; off", gamma);
  }
  if (!s.CreateImage(s.shown, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, kShownFormat,
                     {s.shown_extent.width, s.shown_extent.height, 1}, 1,
                     VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                         (s.gamma_values ? VK_IMAGE_USAGE_STORAGE_BIT : 0),
                     VK_IMAGE_ASPECT_COLOR_BIT) ||
      (s.gamma_values && !s.CreateGammaResources()) || !s.CreateRings() || !s.CreateDescriptors() || !s.CreateDummyTextures() ||
      !s.CreateResolveResources() || !s.textures.Init(backend, frames_in_flight)) {
    ITC_LOG_ERROR("native renderer: couldn't create the passes' resources");
    return false;
  }
  VkFormatProperties depth_properties{};
  vk::vkGetPhysicalDeviceFormatProperties(context.physical_device, kDepthFormat,
                                          &depth_properties);
  s.depth_filterable = depth_properties.optimalTilingFeatures &
                       VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT;
  ITC_LOG_INFO("native renderer: depth targets {} handed to their resolves' textures (linear "
               "filtering of the depth format {})",
               s.depth_filterable ? "can be" : "can't be",
               s.depth_filterable ? "supported" : "not supported");
  s.shaders = std::make_unique<ShaderCache>();
  if (std::filesystem::path directory = DiskCacheDirectory(); !directory.empty()) {
    s.pipeline_cache_file = directory / "pipelines.bin";
  }
  s.OpenPipelineCache();
  return true;
}

void Passes::State::OpenPipelineCache() {
  std::vector<uint8_t> data;
  if (!pipeline_cache_file.empty()) {
    if (std::FILE* in = OpenFile(pipeline_cache_file, "rb")) {
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
              header.vendorID == context.vendor_id && header.deviceID == context.device_id &&
              std::equal(std::begin(header.pipelineCacheUUID), std::end(header.pipelineCacheUUID),
                         context.pipeline_cache_uuid.begin());
  }
  VkPipelineCacheCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO};
  if (matches) {
    info.initialDataSize = data.size();
    info.pInitialData = data.data();
  }
  if (vk::vkCreatePipelineCache(context.device, &info, nullptr, &pipeline_cache) != VK_SUCCESS) {
    pipeline_cache = VK_NULL_HANDLE;
  }
  pipeline_cache_saved = std::chrono::steady_clock::now();
  if (!pipeline_cache_file.empty()) {
    ITC_LOG_INFO("native renderer: pipeline disk cache {}: {}", pipeline_cache_file.string(),
                 matches        ? std::format("{} KB loaded", data.size() / 1024)
                 : data.empty() ? std::string("new")
                               : std::string("another device's or driver's; starting over"));
  }
}

void Passes::State::MaybeSavePipelineCache() {
  size_t made = pipelines.size() + resolve_pipelines.size() + id_pipelines.size();
  auto now = std::chrono::steady_clock::now();
  if (!pipeline_cache || pipeline_cache_file.empty() || made == pipeline_cache_saved_pipelines ||
      now - pipeline_cache_saved < std::chrono::seconds(10)) {
    return;
  }
  pipeline_cache_saved_pipelines = made;
  pipeline_cache_saved = now;
  size_t size = 0;
  if (vk::vkGetPipelineCacheData(context.device, pipeline_cache, &size, nullptr) != VK_SUCCESS) {
    return;
  }
  std::vector<uint8_t> data(size);
  if (vk::vkGetPipelineCacheData(context.device, pipeline_cache, &size, data.data()) !=
      VK_SUCCESS) {
    return;
  }
  // Written aside and renamed over, so an exit mid-write leaves the old file.
  std::filesystem::path temporary = pipeline_cache_file;
  temporary += ".tmp";
  std::FILE* out = OpenFile(temporary, "wb");
  bool written = out && std::fwrite(data.data(), 1, size, out) == size;
  if (out) {
    written = std::fclose(out) == 0 && written;
  }
  std::error_code error;
  if (written) {
    std::filesystem::rename(temporary, pipeline_cache_file, error);
  }
  if (!written || error) {
    ITC_LOG_WARN("native renderer: pipeline disk cache: couldn't write {}",
                 pipeline_cache_file.string());
  }
}

void Passes::BeginFrame(uint32_t frame_index) {
  State& s = *state_;
  s.MaybeSavePipelineCache();
  s.frame = frame_index;
  s.ring_offset = 0;
  s.rings[frame_index].block = 0;
  s.rendering = false;
  s.recorded.valid = false;
  s.bound_color = s.bound_depth = nullptr;
  std::fill(std::begin(s.last_constants.valid), std::end(s.last_constants.valid), false);
  s.main_view = nullptr;
  s.front = nullptr;
  s.frame_resolves.clear();
  Buffers_KeepSegments(false);
  s.arena.BeginFrame(frame_index);
  if (Buffers_LogPhysicalWrites()) {
    Buffers_OnFrameStart([&s](uint32_t start, uint32_t end) { return s.textures.Holds(start, end); });
  }
  if (s.log_texture_bindings) {
    s.binding_counts.frames++;
  }
  s.id_draws.clear();
  s.id_drawn = false;
  s.watched_this_frame.clear();
  s.frame_pixel_shaders.clear();
  s.texture_sets.Clear();
  for (State::LastTextureSet& last : s.last_texture_sets) {
    last.set = VK_NULL_HANDLE;
  }
  auto& ring = s.rings[frame_index];
  for (size_t i = 0; i <= ring.texture_pool && i < ring.texture_pools.size(); i++) {
    vk::vkResetDescriptorPool(s.context.device, ring.texture_pools[i], 0);
  }
  ring.texture_pool = 0;
  vk::vkResetDescriptorPool(s.context.device, s.resolve_pools[frame_index], 0);
  s.textures.BeginFrame(frame_index);
}

VkCommandBuffer Passes::EndUploads() {
  return state_->textures.EndUploads();
}

VkCommandBuffer Passes::EndBufferUploads() {
  return state_->arena.EndUploads();
}

std::string Passes::TakeArenaStats() {
  return state_->arena.TakeStats();
}

void Passes::State::Draw(VkCommandBuffer commands, uint32_t device,
                         const DrawCall& call) {
  State& s = *this;
  auto& regs = s.regs;
  // The bound surfaces: render target 0 and depth (others aren't drawn).
  uint32_t color_object = render::BoundTargets().color[0];
  uint32_t depth_object = render::BoundTargets().depth;
  for (uint32_t i = 1; i < 4; i++) {
    // The same surface in another slot (seen: target 1 = target 0) adds
    // nothing.
    if (uint32_t object = render::BoundTargets().color[i];
        object && object != color_object) {
      if (!s.logged_extra_targets) {
        s.logged_extra_targets = true;
        Surface extra = ReadSurface(object);
        Surface first = color_object ? ReadSurface(color_object) : Surface{};
        ITC_LOG_INFO("native renderer: render target {} bound ({:08X}: EDRAM base {}, pitch {}, "
                     "MSAA {}, format {}) with target 0 {:08X} (EDRAM base {}, pitch {}, MSAA {}, "
                     "format {})",
                     i, object, extra.edram_base, extra.pitch, extra.msaa, extra.format,
                     color_object, first.edram_base, first.pitch, first.msaa, first.format);
      }
      s.stats.extra_targets++;
      break;
    }
  }
  Target* color = color_object ? s.GetTarget(commands, color_object, false) : nullptr;
  Target* depth = depth_object ? s.GetTarget(commands, depth_object, true) : nullptr;
  if (!color && !depth) {
    s.stats.failed++;
    s.stats.failed_by[0]++;
    return;
  }
  if (color && depth && color->samples != depth->samples) {
    s.stats.mismatched_samples++;
    return;
  }
  // The area both can be drawn in, in the guest's pixels and the images'.
  VkExtent2D guest_extent = color ? color->guest_extent : depth->guest_extent;
  VkExtent2D extent = color ? color->extent : depth->extent;
  if (color && depth) {
    guest_extent = {std::min(color->guest_extent.width, depth->guest_extent.width),
                    std::min(color->guest_extent.height, depth->guest_extent.height)};
    extent = {std::min(color->extent.width, depth->extent.width),
              std::min(color->extent.height, depth->extent.height)};
  }

  uint32_t type = call.type;
  uint32_t base_start = call.base_start;
  uint32_t primitive_count = call.primitive_count;
  PrimitiveInfo primitive;
  bool rectangle_list = type == 8, quad_list = type == 13;
  if (!GetPrimitiveInfo(type, primitive) ||
      ((rectangle_list || quad_list) && !call.inline_vertices)) {
    s.stats.unsupported++;
    return;
  }
  // The base vertex added to each index: the draw's own (the item's upper
  // half, as the library's DrawIndexedVertices puts it in VGT_INDX_OFFSET:
  // never another in the runs that compared them); 0 for inline draws.
  int32_t base_vertex = call.inline_vertices ? 0 : int32_t(base_start) >> 16;
  uint32_t start_index = base_start & 0xFFFF;
  // Inline draws: the vertices in order (indices only to expand rectangle
  // and quad lists).
  uint32_t index_count = call.inline_vertices ? call.vertex_count
                                              : primitive_count * primitive.a + primitive.b;

  // Shaders, read through physical memory as the GPU reads them.
  const render::Bindings& bound = render::BoundResources();
  uint32_t program_control, context_misc;
  render::ShaderControls(program_control, context_misc);
  uint32_t vertex_object = bound.vertex_shader;
  uint32_t pixel_object = bound.pixel_shader;
  if (!vertex_object) {
    s.stats.failed++;
    s.stats.failed_by[1]++;
    return;
  }
  uint32_t vertex_address = guest::LoadU32(vertex_object + 48);
  const uint8_t* vertex_ucode = PhysicalHostAddress(GpuPhysicalAddress(vertex_address));
  uint32_t vertex_size = guest::LoadU32(vertex_object + 600);
  uint32_t declaration = bound.declaration;
  // The fetches take their fields from the declaration and the stream
  // strides (the library's patch of the microcode no longer runs), unless
  // the shader is one the library doesn't patch (its +592 bit 6).
  bool fields_from_declaration =
      declaration && !(guest::LoadU32(vertex_object + 592) & 0x40);
  // Each stream's stride in words; an inline draw's stream 0 its own.
  uint8_t stream_strides[16];
  std::memcpy(stream_strides, bound.strides, sizeof(stream_strides));
  if (call.inline_vertices) {
    stream_strides[0] = uint8_t(call.inline_stride >> 2);
  }
  uint32_t strides[4] = {};
  if (fields_from_declaration) {
    std::memcpy(strides, stream_strides, sizeof(strides));
  }
  uint32_t pixel_address = pixel_object ? guest::LoadU32(pixel_object + 24) : 0;
  const uint8_t* pixel_ucode =
      pixel_object ? PhysicalHostAddress(GpuPhysicalAddress(pixel_address)) : nullptr;
  uint32_t pixel_size = pixel_object ? guest::LoadU32(pixel_object + 64) : 0;
  // The lookup in full: each microcode hashed once (the cache's lookups take
  // the hash), which interpolators connect them (an unconnected one the
  // pixel shader reads would be undefined), and the translations.
  auto look_up = [&] {
    ShaderLookup r;
    r.vertex_hash = MicrocodeHash(vertex_ucode, vertex_size);
    r.pixel_hash = pixel_ucode ? MicrocodeHash(pixel_ucode, pixel_size) : 0;
    // The file's microcode with the fetches' fields made for the
    // declaration, for its GLSL (shader_ports.h): the object keeps its
    // fetches' records (+60, +64) and file instructions (+192).
    VertexFetchRemap remap;
    uint32_t fetch_count = guest::LoadU32(vertex_object + 60);
    bool remapped = false;
    if (fetch_count <= 32) {
      uint32_t records[32];
      std::array<uint32_t, 3> made[32];
      for (uint32_t j = 0; j < fetch_count; j++) {
        records[j] = guest::LoadU32(vertex_object + 64 + 4 * j);
        uint32_t original[3];
        for (uint32_t w = 0; w < 3; w++) {
          original[w] = guest::LoadU32(vertex_object + 192 + 12 * j + 4 * w);
        }
        if (fields_from_declaration) {
          graphics::shader_patch::Fetch fetch = graphics::shader_patch::FetchFromDeclaration(
              records[j], original, declaration, stream_strides);
          made[j] = {fetch.words[0], fetch.words[1], fetch.words[2]};
        } else {
          made[j] = {original[0], original[1], original[2]};
        }
      }
      remapped = FitVertexFetches(vertex_ucode, vertex_size, fetch_count, records,
                                  guest::Pointer(vertex_object + 192), made, remap);
    }
    if (remapped) {
      // The shader as fitted, for the caches.
      r.vertex_hash = MicrocodeHash(remap.ucode.data(), vertex_size) ^
                      base::Hash64(remap.swizzles.data(),
                                   remap.swizzles.size() * sizeof(remap.swizzles[0]));
    }
    r.interface = s.shaders->GetInterface(vertex_ucode, vertex_size, r.vertex_hash, pixel_ucode,
                                          pixel_size, r.pixel_hash, program_control,
                                          context_misc, remapped ? &remap : nullptr);
    r.interface.param_gen_point = type == 1;  // point list
    r.interface.rectangle_list = rectangle_list;
    r.vertex = s.shaders->Get(ShaderStage::kVertex, vertex_ucode, vertex_size, r.vertex_hash,
                              r.interface, remapped ? &remap : nullptr);
    if (pixel_object) {
      r.pixel = s.shaders->Get(ShaderStage::kPixel, pixel_ucode, pixel_size, r.pixel_hash,
                               r.interface);
    }
    return r;
  };
  // Kept by what the draw read from the shader objects and the
  // declaration (ShaderLookupTable).
  ShaderLookup lookup;
  if (s.cache_shader_lookups) {
    ShaderLookupTable::Key key = {{vertex_object, vertex_address, vertex_size,
                                   uint32_t(bound.declaration_hash >> 32),
                                   uint32_t(bound.declaration_hash), strides[0], strides[1], strides[2],
                                   strides[3], pixel_object, pixel_address, pixel_size,
                                   program_control, context_misc,
                                   uint32_t(type == 1) | uint32_t(rectangle_list) << 1 |
                                       uint32_t(fields_from_declaration) << 2,
                                   0}};
    const ShaderLookup* kept = nullptr;
    bool last = s.last_shader_valid && key == s.last_shader_key;
    if (last) {
      kept = &s.last_shader_lookup;
    } else {
      kept = s.shader_lookups.Find(key);
    }
    if (kept) {
      lookup = *kept;
    } else {
      lookup = look_up();
      // Failed translations aren't kept.
      if (lookup.vertex && (!pixel_object || lookup.pixel)) {
        s.shader_lookups.Add(key, lookup);
      }
    }
    s.last_shader_key = key;
    s.last_shader_lookup = lookup;
    s.last_shader_valid = lookup.vertex && (!pixel_object || lookup.pixel);
  } else {
    lookup = look_up();
  }
  const ShaderCache::Interface& interface = lookup.interface;
  uint64_t pixel_hash = lookup.pixel_hash;
  if (pixel_ucode && interface.param_gen_interpolator < 16) {
    s.stats.param_gen_draws++;
    if (s.collect_interfaces) {
      s.param_gen_by_shader[pixel_hash] |=
          1u << interface.param_gen_interpolator;
    }
  }
  if (pixel_ucode && s.collect_interfaces) {
    s.interpolators_by_shader[pixel_hash] |=
        uint64_t(interface.interpolator_mask) << 16 * (program_control >> 18 & 1);
  }
  TranslatedShader* vertex = lookup.vertex;
  TranslatedShader* pixel = lookup.pixel;
  if (!vertex || (pixel_object && !pixel)) {
    s.stats.failed++;
    s.stats.failed_by[2]++;
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
    // The game's velocity-based motion blur: leaving its one draw out
    // leaves the frame as drawn, unblurred.
    constexpr uint64_t kMotionBlur = 0x7EB16456AAD56CBAull;
    if (skip_motion_blur && pixel->hash == kMotionBlur) {
      return;
    }
  } else if (skip_pixel_shader == kSkipNoPixelShader) {
    return;
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
    r.count = call.inline_vertices ? call.vertex_count : primitive_count;
    r.inline_vertices = call.inline_vertices;
    for (uint32_t k = 0; k < 96; k++) {
      if (vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31))) {
        r.vertex_fetches.push_back({k, regs.fetch[2 * k],
                                    regs.fetch[2 * k + 1]});
      }
    }
    // The render states in the registers' layouts the dump prints them in.
    const render::RenderStates& rs = render::States();
    r.render_state = {uint32_t(rs.z_enable) << 1 | uint32_t(rs.z_write) << 2 |
                          uint32_t(rs.z_func) << 4,
                      rs.blend[0].fields,
                      uint32_t(rs.alpha_func) | uint32_t(rs.alpha_test) << 3,
                      pixel_object ? 4u : 5u,
                      rs.color_write[0],
                      std::bit_cast<uint32_t>(rs.blend_constants[0]),
                      std::bit_cast<uint32_t>(rs.blend_constants[1]),
                      std::bit_cast<uint32_t>(rs.blend_constants[2]),
                      std::bit_cast<uint32_t>(rs.blend_constants[3]),
                      std::bit_cast<uint32_t>(rs.alpha_ref)};
    for (const TranslatedShader* shader : {vertex, pixel}) {
      for (size_t i = 0; shader && i < shader->textures.size(); i++) {
        const ShaderTexture& texture = shader->textures[i];
        RecordedDraw::Texture& t = r.textures.emplace_back();
        t.pixel = shader == pixel;
        t.texture = texture;
        std::memcpy(t.fetch,
                    &regs.fetch[6 * texture.fetch_constant],
                    sizeof(t.fetch));
      }
    }
  }

  if (log_glow && pixel && pixel->hash == kGlowCrossFade) {
    glow_frame.cross_fades++;
    glow_frame.constants.clear();
    for (uint32_t c = 0; c < 256; c++) {
      if (pixel->float_constants[c >> 6] & (uint64_t(1) << (c & 63))) {
        uint32_t at = device + kDevicePixelFloatConstants + 16 * c;
        glow_frame.constants += std::format(
            "c{} {:.4g} {:.4g} {:.4g} {:.4g}; ", c, guest::LoadFloat(at),
            guest::LoadFloat(at + 4), guest::LoadFloat(at + 8),
            guest::LoadFloat(at + 12));
      }
    }
    for (const ShaderTexture& texture : pixel->textures) {
      glow_frame.cross_fade_texture =
          regs.fetch[6 * texture.fetch_constant + 1] &
          0x1FFFF000u;
      break;
    }
  }

  current_vertex_shader = vertex->hash;
  current_pixel_shader = pixel ? pixel->hash : 0;
  drew_any = true;

  // Index buffer: +0 bit 31 = 32-bit, bits 29-30 = endianness; +24 address.
  bool index_32bit = false;
  uint32_t index_endian = 0, index_address = 0;
  if (!call.inline_vertices) {
    uint32_t index_buffer = bound.index_buffer;
    if (!index_buffer) {
      s.stats.failed++;
      s.stats.failed_by[3]++;
      return;
    }
    uint32_t index_word = guest::LoadU32(index_buffer);
    index_32bit = index_word >> 31;
    index_endian = (index_word >> 29) & 3;
    index_address = GpuPhysicalAddress(guest::LoadU32(index_buffer + 24)) +
                    start_index * (index_32bit ? 4 : 2);
  }

  // Viewport and scissor.
  bool z_enable = false, z_write = false;
  uint32_t z_func = 0;
  const render::RenderStates& states = render::States();
  // No depth drawn (RB_MODECONTROL), or a test that always passes and
  // doesn't write: off.
  {  // Depth is drawn with a pixel shader or without (depth only).
    z_enable = states.z_enable;
    z_write = states.z_write;
    z_func = uint32_t(states.z_func);
    if (z_enable && !z_write && states.z_func == gpu::CompareOp::kAlways) {
      z_enable = false;
    }
  }
  // D3D's viewport clamped to the target drawn into (a tiled target is the
  // tiles' whole size).
  render::Viewport set_viewport = render::BoundTargets().viewport;
  {
    uint64_t right = std::min<uint64_t>(uint64_t(set_viewport.x) + set_viewport.width,
                                        guest_extent.width);
    uint64_t bottom = std::min<uint64_t>(uint64_t(set_viewport.y) + set_viewport.height,
                                         guest_extent.height);
    set_viewport.width = right > set_viewport.x ? uint32_t(right - set_viewport.x) : 0;
    set_viewport.height = bottom > set_viewport.y ? uint32_t(bottom - set_viewport.y) : 0;
  }
  const DrawRegisters::Viewport viewport_info =
      HostViewport(set_viewport, states.viewport_enable, states.half_pixel_offset);
  // Scissor: the window scissor registers aren't in the shadow's flushed
  // blocks, so take it as the library keeps it: enabled by render state 200
  // (device +11600), rectangle (left, top, right, bottom) at +12416 from
  // SetScissorRect; otherwise the whole target.
  struct {
    uint32_t offset[2], extent[2];
  } scissor{{0, 0}, {guest_extent.width, guest_extent.height}};
  if (states.scissor_enable) {
    int32_t left = states.scissor[0];
    int32_t top = states.scissor[1];
    int32_t right = states.scissor[2];
    int32_t bottom = states.scissor[3];
    scissor.offset[0] = uint32_t(std::max(left, 0));
    scissor.offset[1] = uint32_t(std::max(top, 0));
    scissor.extent[0] = uint32_t(std::max(right - int32_t(scissor.offset[0]), 0));
    scissor.extent[1] = uint32_t(std::max(bottom - int32_t(scissor.offset[1]), 0));
  }
  uint32_t scissor_right = std::min(scissor.offset[0] + scissor.extent[0], guest_extent.width);
  uint32_t scissor_bottom = std::min(scissor.offset[1] + scissor.extent[1], guest_extent.height);
  if (!viewport_info.extent[0] || !viewport_info.extent[1] ||
      scissor.offset[0] >= scissor_right || scissor.offset[1] >= scissor_bottom) {
    s.stats.invisible++;
    if (!s.stats.invisible_sample[0]) {  // one example per log line
      s.stats.invisible_sample = {viewport_info.offset[0], viewport_info.offset[1],
                                  viewport_info.extent[0], viewport_info.extent[1],
                                  scissor.offset[0], scissor.offset[1], scissor.extent[0],
                                  scissor.extent[1]};
      s.stats.invisible_sample[0] |= 0x80000000u;  // mark as set
    }
    return;  // nothing visible
  }

  // The draw's own constants (draw_constants.h).
  DrawConstants system{};
  uint32_t flags = 0;
  if (primitive.polygonal) {
    flags |= kFlagPrimitivePolygonal;
  } else if (type == 2 || type == 3) {
    flags |= kFlagPrimitiveLine;
  }
  uint32_t alpha_function = states.alpha_test ? uint32_t(states.alpha_func) : 7;  // 7: always
  flags |= alpha_function << kFlagAlphaTestShift;
  system.flags = flags;
  for (uint32_t i = 0; i < 3; i++) {
    system.ndc_scale[i] = viewport_info.ndc_scale[i];
    system.ndc_offset[i] = viewport_info.ndc_offset[i];
  }
  system.alpha_test_reference = states.alpha_ref;
  // Each sampled texture's reader scale. The shaders sample a texture's
  // view as it is: signed, biased or gamma components and an exponent bias
  // in the texture aren't applied (none of the game's textures has them in
  // the attract race or the pad replay; logged once each if one does). Its
  // LOD bias is the sampler's (render::SamplerState).
  for (const TranslatedShader* shader : {vertex, pixel}) {
    if (!shader) {
      continue;
    }
    for (const ShaderTexture& texture : shader->textures) {
      uint32_t index = texture.fetch_constant;
      const uint32_t* words = &regs.fetch[6 * index];
      system.texture_scale[index & 31] = s.textures.ReaderScale(words);
      uint8_t signs = TextureCache::SwizzledSigns(words);
      int32_t exponent = int32_t(words[3] << 13) >> 26;
      if (signs || exponent) [[unlikely]] {
        static std::unordered_set<uint64_t> logged;
        if (logged.insert(shader->hash ^ (uint64_t(words[1]) << 32) ^ signs).second) {
          ITC_LOG_ERROR("native renderer: {} shader {:016X} samples texture {:08X} (format {}) "
                        "with signs {:02X} or exponent bias {}, which the shaders don't apply",
                        shader == vertex ? "vertex" : "pixel", shader->hash,
                        words[1] & 0xFFFFF000u, words[1] & 63, signs, exponent);
        }
      }
    }
  }
  // Each bound target's exponent bias, from its surface's color info (+28,
  // bits 20-25).
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t surface = render::BoundTargets().color[i];
    uint32_t info = surface ? guest::LoadU32(surface + 28) : 0;
    int32_t bias = int32_t(info << 6) >> 26;
    uint32_t scale_bits = uint32_t(0x3F800000 + (bias << 23));
    std::memcpy(&system.color_exp_bias[i], &scale_bits, 4);
  }

  // Constants into this frame's ring: system, vertex float, pixel float,
  // bool/loop, fetch. A block the same as the last one written points at it.
  static_assert(sizeof(system) <= kFloatConstantsRange);
  uint32_t offsets[kConstantBlocks];
  if (!s.ReserveDrawConstants() ||
      !s.PlaceConstants(0, &system, sizeof(system), sizeof(system), offsets[0])) {
    s.stats.failed++;
    s.stats.failed_by[4]++;
    return;
  }
  for (uint32_t stage = 0; stage < 2; stage++) {
    const TranslatedShader* shader = stage == 0 ? vertex : pixel;
    const float(*source)[4] = render::ShaderConstants().floats[stage];
    // The registers the shader is given, from its lowest to its highest,
    // into the last copy, noting whether any differ.
    uint32_t* copy = reinterpret_cast<uint32_t*>(s.last_constants.bytes[1 + stage]);
    bool same = s.last_constants.valid[1 + stage];
    const uint32_t packed = shader ? shader->float_count : 0;
    if (packed) {
      const float* from = source[shader->float_first];
      if (!same || std::memcmp(copy, from, 16 * packed)) {
        std::memcpy(copy, from, 16 * packed);
        same = false;
      }
    }
    if (!s.PlaceConstants(1 + stage, copy, 16 * packed, kFloatConstantsRange,
                          offsets[1 + stage], true, same)) {
      s.stats.failed++;
      s.stats.failed_by[4]++;
      return;
    }
  }
  // The draw's vertex streams and indices in the vertex arena: each stream
  // the vertex shader reads gets its range's arena offset in place of the
  // address (word 0, low 2 bits the type; word 1 bits 2-25 the size in
  // words). An inline draw's stream points into the arena already.
  uint32_t arena_fetch[kFetchConstantsBytes / 4];
  std::memcpy(arena_fetch, regs.fetch,
              kFetchConstantsBytes);
  if (!call.inline_vertices) {
    // The streams' vertex fetch constants as bound (95 - stream).
    for (uint32_t stream = 0; stream < 16; stream++) {
      arena_fetch[2 * (95 - stream)] = bound.stream_fetch[stream][0];
      arena_fetch[2 * (95 - stream) + 1] = bound.stream_fetch[stream][1];
    }
  }
  uint32_t index_offset = 0;
  if (!call.inline_vertices) {
    for (uint32_t k = 0; k < 96; k++) {
      if (!(vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31)))) {
        continue;
      }
      uint32_t word0 = arena_fetch[2 * k];
      uint32_t size = ((arena_fetch[2 * k + 1] >> 2) & 0xFFFFFFu) * 4;
      // A buffer the arena holds by its object, else placed by address, in
      // the host's byte order (the fetch constant's endian mode).
      uint32_t offset = UINT32_MAX;
      if (k >= 80) {
        if (uint32_t object = bound.stream_buffer[95 - k]) {
          offset = s.arena.HeldBuffer(object, word0 & 0x1FFFFFFCu, arena_fetch[2 * k + 1] & 3);
        }
      }
      if (offset == UINT32_MAX) {
        offset = s.arena.Place(word0 & 0x1FFFFFFCu, size, arena_fetch[2 * k + 1] & 3);
      }
      if (offset == UINT32_MAX) {
        s.stats.failed++;
        s.stats.failed_by[7]++;
        return;
      }
      arena_fetch[2 * k] = offset | (word0 & 3);
    }
    // In the host's byte order, as the vertex input fetches by them: held by
    // its object, else placed by address.
    index_offset = s.arena.HeldBuffer(bound.index_buffer, index_address, index_endian);
    if (index_offset == UINT32_MAX) {
      index_offset =
          s.arena.Place(index_address, index_count * (index_32bit ? 4 : 2), index_endian);
    }
    if (index_offset == UINT32_MAX) {
      s.stats.failed++;
      s.stats.failed_by[7]++;
      return;
    }
  }
  // The vertex buffers: each fetch's stream in the arena, bound at its
  // element (the fetch's offset in the vertex); a rectangle list's three
  // times, a vertex apart.
  VkDeviceSize vertex_offsets[kMaxVertexBindings];
  uint32_t vertex_bindings = 0;
  if (vertex->vertex_fetches.size() > kMaxVertexFetches) {
    s.stats.failed++;
    s.stats.failed_by[7]++;
    return;
  }
  for (const TranslatedShader::VertexFetch& f : vertex->vertex_fetches) {
    int64_t at = int64_t(arena_fetch[2 * f.fetch_constant] & 0x1FFFFFFCu) +
                 4 * int64_t(f.offset_words);
    if (at < 0) {
      s.stats.failed++;
      s.stats.failed_by[7]++;
      return;
    }
    for (uint32_t g = 0; g < (rectangle_list ? 3u : 1u); g++) {
      vertex_offsets[vertex_bindings++] = VkDeviceSize(at) + 4 * f.stride_words * g;
    }
  }
  if (!s.PlaceConstants(3, &render::ShaderConstants().bool_loop,
                        kBoolLoopConstantsRange, kBoolLoopConstantsRange, offsets[3])) {
    s.stats.failed++;
    s.stats.failed_by[4]++;
    return;
  }
  s.stats.constants_draws++;

  // Pipeline and textures.
  VkDescriptorSetLayout vertex_layout = s.GetTextureLayout(vertex, VK_SHADER_STAGE_VERTEX_BIT);
  VkDescriptorSetLayout pixel_layout = s.GetTextureLayout(pixel, VK_SHADER_STAGE_FRAGMENT_BIT);
  if (!vertex_layout || !pixel_layout) {
    s.stats.failed++;
    s.stats.failed_by[5]++;
    return;
  }
  if (s.log_texture_bindings) {
    s.CountDrawBindings(vertex, pixel);
  }
  VkDescriptorSet vertex_textures = s.GetTextureSet(vertex, vertex_layout, 0);
  VkDescriptorSet pixel_textures = s.GetTextureSet(pixel, pixel_layout, 1);
  if (!vertex_textures || !pixel_textures) {
    s.stats.failed++;
    s.stats.failed_by[6]++;
    return;
  }
  PipelineKey key;
  std::memset(&key, 0, sizeof(key));
  key.vertex = s.GetModule(vertex);
  key.pixel = s.GetModule(pixel);
  key.layout = s.GetPipelineLayout(vertex_layout, pixel_layout);
  key.topology = primitive.topology;
  key.primitive_restart = primitive.strip && states.primitive_restart;
  key.cull_mode = uint32_t(states.cull);
  key.front_clockwise = states.front_clockwise;
  if (depth) {
    key.z_enable = z_enable;
    key.z_write = z_write;
    key.z_func = z_func;
  }
  key.color_format = color ? color->format : VK_FORMAT_UNDEFINED;
  key.depth = depth != nullptr;
  key.samples = color ? color->samples : depth->samples;
  const render::BlendState& blend = states.blend[0];
  key.src_color = uint32_t(blend.src_color);
  key.dst_color = uint32_t(blend.dst_color);
  key.color_op = uint32_t(blend.color_op);
  key.src_alpha = uint32_t(blend.src_alpha);
  key.dst_alpha = uint32_t(blend.dst_alpha);
  key.alpha_op = uint32_t(blend.alpha_op);
  key.blend_enable =
      !(blend.src_color == gpu::BlendFactor::kOne && blend.dst_color == gpu::BlendFactor::kZero &&
        blend.color_op == gpu::BlendOp::kAdd && blend.src_alpha == gpu::BlendFactor::kOne &&
        blend.dst_alpha == gpu::BlendFactor::kZero && blend.alpha_op == gpu::BlendOp::kAdd);
  // Each bound target's color format, from its surface's color info.
  uint32_t target_formats[4];
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t surface = render::BoundTargets().color[i];
    target_formats[i] = surface ? guest::LoadU32(surface + 28) >> 16 & 0xF : 0;
  }
  // As the plugin normalizes it: nothing without a pixel shader, for a
  // target the pixel shader doesn't write, or in depth-only EDRAM mode.
  // (With the raw mask, those draws wrote Vulkan's undefined fragment output:
  // the rainbow effect on buildings, from ~230 main view draws a frame
  // without a pixel shader.)
  key.write_mask =
      regs.NormalizedColorMask(pixel ? pixel->writes_color_targets : 0, states.color_write,
                               pixel_object != 0, target_formats) &
      0xF;
  if (!key.vertex || (pixel && !key.pixel) || !key.layout) {
    s.stats.failed++;
    return;
  }
  VkPipeline pipeline = s.GetPipeline(key);
  if (s.watch_draws && main_view_draw &&
      (!s.watch_pixel_shader || (pixel && pixel->hash == s.watch_pixel_shader))) {
    s.WatchDraw(vertex, pixel, index_address, primitive_count, key,
                reinterpret_cast<const uint32_t*>(s.constant_block().mapped + offsets[0]),
                reinterpret_cast<const uint32_t*>(s.constant_block().mapped + offsets[1]),
                reinterpret_cast<const uint32_t*>(s.constant_block().mapped + offsets[2]),
                reinterpret_cast<const uint32_t*>(&render::ShaderConstants().bool_loop),
                regs.fetch);
  }
  if (!pipeline) {
    s.stats.failed++;
    return;
  }

  // Draw.
  s.BeginRendering(commands, color, depth);
  if (color && color->surface.pitch == kMainViewPitch && color->samples == VK_SAMPLE_COUNT_2_BIT &&
      color->format == VK_FORMAT_R16G16B16A16_SFLOAT) {
    s.main_view = color;
  }
  // Both scaled from the guest's pixels (the NDC scale and offset are
  // ratios, unchanged), by the targets' scale (color and depth share one:
  // the main view's surfaces are both scaled alike).
  float draw_scale = color ? color->scale : depth->scale;
  VkViewport viewport{float(viewport_info.offset[0]) * draw_scale,
                      float(viewport_info.offset[1]) * draw_scale,
                      float(viewport_info.extent[0]) * draw_scale,
                      float(viewport_info.extent[1]) * draw_scale,
                      viewport_info.z_min,
                      viewport_info.z_max};
  uint32_t scissor_left = State::ScaledBy(scissor.offset[0], draw_scale);
  uint32_t scissor_top = State::ScaledBy(scissor.offset[1], draw_scale);
  uint32_t scissor_right_scaled = State::ScaledBy(scissor_right, draw_scale);
  uint32_t scissor_bottom_scaled = State::ScaledBy(scissor_bottom, draw_scale);
  VkRect2D scissor_rect{
      {int32_t(scissor_left), int32_t(scissor_top)},
      {std::min(scissor_right_scaled, extent.width) - std::min(scissor_left, extent.width),
       std::min(scissor_bottom_scaled, extent.height) - std::min(scissor_top, extent.height)}};
  // Set for every pipeline (it's dynamic in all of them), used with depth.
  float slope = 0.0f, constant = 0.0f;
  if (depth) {
    // As the plugin sets it for its host render targets: the guest's depth
    // is float24 (24_8_FLOAT) or unorm24, the slope in 1/16 pixels.
    // Both faces take the same bias (D3D's DepthBias and SlopeScaleDepthBias),
    // on when either isn't 0; lines take none.
    if (primitive.polygonal &&
        (states.depth_bias_slope != 0.0f || states.depth_bias_constant != 0.0f) &&
        states.cull != gpu::CullMode::kFrontAndBack) {
      slope = states.depth_bias_slope;
      constant = states.depth_bias_constant;
    }
    // D3D10's factors: 2^24 - 1 for unorm24 (D24S8, format 0), 2^24 for float24.
    constant *= depth->surface.format == 0 ? float((1u << 24) - 1) : float(1u << 24);
    // Per pixel: scaled pixels are smaller, so the slope (as the plugin
    // scales it).
    slope *= xenos::kPolygonOffsetScaleSubpixelUnit * std::max(draw_scale, 1.0f);
    if (constant != 0.0f || slope != 0.0f) {
      s.stats.depth_biased++;
    }
    if (key.z_enable && gpu::CompareOp(key.z_func) == gpu::CompareOp::kEqual) {
      s.stats.depth_equal++;
    }
  }
  // RB_BLEND_RED/GREEN/BLUE/ALPHA, for the constant blend factors.
  float blend_constants[4];
  std::memcpy(blend_constants, states.blend_constants, sizeof(blend_constants));
  VkDescriptorSet sets[4] = {s.arena_set, s.constant_block().set,
                             vertex_textures, pixel_textures};

  // The state this draw needs. Indexed draws bind the mirror at offset 0
  // and start at their indices with firstIndex, so consecutive draws share
  // the binding (unless an address isn't a multiple of the index size).
  State::RecordedState now{};
  now.valid = true;
  now.viewport = viewport;
  now.scissor = scissor_rect;
  now.bias[0] = constant;
  now.bias[1] = slope;
  std::memcpy(now.blend_constants, blend_constants, sizeof(blend_constants));
  now.pipeline = pipeline;
  now.layout = key.layout;
  std::memcpy(now.sets, sets, sizeof(sets));
  std::memcpy(now.offsets, offsets, sizeof(offsets));
  now.indexed = quad_list || !call.inline_vertices;
  now.index_buffer = s.arena.buffer();
  now.index_type = quad_list || index_32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
  uint32_t index_bytes = now.index_type == VK_INDEX_TYPE_UINT32 ? 4 : 2;
  uint64_t index_start = quad_list ? kQuadIndices : index_offset;
  now.vertex_bindings = vertex_bindings;
  std::copy(vertex_offsets, vertex_offsets + vertex_bindings, now.vertex_offsets);
  uint32_t first_index = 0;
  if (s.skip_repeated_state && index_start % index_bytes == 0) {
    first_index = uint32_t(index_start / index_bytes);
  } else {
    now.index_offset = index_start;
  }

  // Which state calls repeat what the command buffer already holds (the
  // previous draw's, with nothing else recorded since); with
  // skip_repeated_state they aren't recorded again.
  const State::RecordedState& last = s.recorded;
  auto same = [](const auto& a, const auto& b) { return !std::memcmp(&a, &b, sizeof(a)); };
  bool repeated[9] = {};
  bool both_indexed = last.valid && now.indexed && last.indexed;
  if (last.valid) {
    repeated[0] = same(last.viewport, now.viewport);
    repeated[1] = same(last.scissor, now.scissor);
    repeated[2] = same(last.bias, now.bias);
    repeated[3] = same(last.blend_constants, now.blend_constants);
    repeated[4] = last.pipeline == now.pipeline;
    repeated[5] = last.layout == now.layout && same(last.sets, now.sets);
    repeated[6] = both_indexed && last.index_buffer == now.index_buffer &&
                  last.index_offset == now.index_offset && last.index_type == now.index_type;
    repeated[7] = both_indexed && last.index_buffer == now.index_buffer &&
                  last.index_type == now.index_type;
    repeated[8] = repeated[5] && same(last.offsets, now.offsets);
    s.stats.state_compared++;
    for (int i = 0; i < 9; i++) {
      s.stats.state_repeated[i] += repeated[i];
    }
    if (s.log_constant_changes && repeated[5] && !repeated[8]) {
      s.stats.offsets_differ++;
      for (uint32_t i = 0; i < kConstantBlocks; i++) {
        if (last.offsets[i] != now.offsets[i]) {
          s.stats.offset_differs[i]++;
        }
      }
    }
  }
  bool skip = s.skip_repeated_state;
  if (!(skip && repeated[0])) {
    vk::vkCmdSetViewport(commands, 0, 1, &viewport);
  }
  if (!(skip && repeated[1])) {
    vk::vkCmdSetScissor(commands, 0, 1, &scissor_rect);
  }
  if (!(skip && repeated[2])) {
    vk::vkCmdSetDepthBias(commands, constant, 0.0f, slope);
  }
  if (!(skip && repeated[3])) {
    vk::vkCmdSetBlendConstants(commands, blend_constants);
  }
  if (!(skip && repeated[4])) {
    vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  }
  if (!(skip && repeated[8])) {
    vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, key.layout, 0, 4, sets,
                                kConstantBlocks, offsets);
  }
  if (now.indexed && !(skip && repeated[6])) {
    vk::vkCmdBindIndexBuffer(commands, now.index_buffer, now.index_offset, now.index_type);
  }
  bool vertex_repeated = last.valid && last.vertex_bindings == vertex_bindings &&
                         std::equal(vertex_offsets, vertex_offsets + vertex_bindings,
                                    last.vertex_offsets);
  if (vertex_bindings && !(skip && vertex_repeated)) {
    VkBuffer buffers[kMaxVertexBindings];
    std::fill(buffers, buffers + vertex_bindings, s.arena.buffer());
    vk::vkCmdBindVertexBuffers(commands, 0, vertex_bindings, buffers, vertex_offsets);
  }
  // The base vertex is the vertex input's (VGT_INDX_OFFSET; 0 for inline
  // draws). A rectangle list is an instance a rectangle, four vertices each
  // (the vertex shader makes the fourth corner).
  if (rectangle_list) {
    vk::vkCmdDraw(commands, 4, index_count / 3, 0, 0);
  } else if (quad_list) {
    vk::vkCmdDrawIndexed(commands, index_count / 4 * 6, 1, first_index, base_vertex, 0);
  } else if (call.inline_vertices) {
    vk::vkCmdDraw(commands, index_count, 1, uint32_t(base_vertex), 0);
  } else {
    vk::vkCmdDrawIndexed(commands, index_count, 1, first_index, base_vertex, 0);
  }
  if (!now.indexed && last.valid) {
    // A draw without indices leaves the earlier binding in place.
    now.indexed = last.indexed;
    now.index_buffer = last.index_buffer;
    now.index_offset = last.index_offset;
    now.index_type = last.index_type;
  }
  s.recorded = now;
  s.stats.drawn++;
  if (call.inline_vertices) {
    s.stats.inline_draws++;
    s.stats.inline_by_type[type & 15]++;
  }
  // (The draw ID pass redraws indexed draws only.)
  if (s.record_draws && main_view_draw && !call.inline_vertices) {
    IdDraw& id = s.id_draws.emplace_back();
    id.number = uint32_t(s.recorded_draws.size() - 1);
    id.vertex = key.vertex;
    id.layout = key.layout;
    std::copy(sets, sets + 4, id.sets);
    std::copy(offsets, offsets + 5, id.offsets);
    id.viewport = viewport;
    id.scissor = scissor_rect;
    id.index_offset = index_offset;
    id.index_type = index_32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
    id.index_count = index_count;
    id.base_vertex = base_vertex;
    id.vertex_bindings = vertex_bindings;
    std::copy(vertex_offsets, vertex_offsets + vertex_bindings, id.vertex_offsets);
    id.key = key;
    id.bias_constant = constant;
    id.bias_slope = slope;
  }
}

VkPipeline Passes::State::GetIdPipeline(const IdDraw& draw) {
  PipelineKey key = draw.key;
  key.pixel = VK_NULL_HANDLE;
  key.blend_enable = key.src_color = key.dst_color = key.color_op = 0;
  key.src_alpha = key.dst_alpha = key.alpha_op = key.write_mask = 0;
  key.color_format = key.samples = 0;
  auto [it, added] = id_pipelines.try_emplace(key, VK_NULL_HANDLE);
  if (!added) {
    return it->second;
  }
  if (!id_pixel) {
    VkShaderModuleCreateInfo info{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    info.codeSize = sizeof(k_id_frag);
    info.pCode = k_id_frag;
    vk::vkCreateShaderModule(context.device, &info, nullptr, &id_pixel);
  }
  VkPipelineShaderStageCreateInfo stages[2] = {};
  for (VkPipelineShaderStageCreateInfo& stage : stages) {
    stage = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    stage.pName = "main";
  }
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = draw.vertex;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = id_pixel;
  VkPipelineVertexInputStateCreateInfo vertex_input{
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
  if (auto input = vertex_inputs.find(draw.vertex); input != vertex_inputs.end()) {
    vertex_input.vertexBindingDescriptionCount = uint32_t(input->second.bindings.size());
    vertex_input.pVertexBindingDescriptions = input->second.bindings.data();
    vertex_input.vertexAttributeDescriptionCount = uint32_t(input->second.attributes.size());
    vertex_input.pVertexAttributeDescriptions = input->second.attributes.data();
  }
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
  raster.cullMode = vk_types::CullMode(gpu::CullMode(key.cull_mode));
  raster.frontFace = key.front_clockwise ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
  raster.depthBiasEnable = VK_TRUE;
  raster.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineDepthStencilStateCreateInfo depth{
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
  depth.depthTestEnable = key.z_enable;
  depth.depthWriteEnable = key.z_write;
  depth.depthCompareOp = vk_types::CompareOp(gpu::CompareOp(key.z_func));
  VkPipelineColorBlendAttachmentState attachment{};
  attachment.colorWriteMask = 0xF;
  VkPipelineColorBlendStateCreateInfo blend{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
  blend.attachmentCount = 1;
  blend.pAttachments = &attachment;
  VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
                                     VK_DYNAMIC_STATE_DEPTH_BIAS};
  VkPipelineDynamicStateCreateInfo dynamic{VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic.dynamicStateCount = 3;
  dynamic.pDynamicStates = dynamic_states;
  VkFormat color_format = VK_FORMAT_R32_UINT;
  VkPipelineRenderingCreateInfo rendering{VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
  rendering.colorAttachmentCount = 1;
  rendering.pColorAttachmentFormats = &color_format;
  rendering.depthAttachmentFormat = kDepthFormat;
  rendering.stencilAttachmentFormat = kDepthFormat;
  VkGraphicsPipelineCreateInfo info{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
  info.pNext = &rendering;
  info.stageCount = 2;
  info.pStages = stages;
  info.pVertexInputState = &vertex_input;
  info.pInputAssemblyState = &input_assembly;
  info.pViewportState = &viewport;
  info.pRasterizationState = &raster;
  info.pMultisampleState = &multisample;
  info.pDepthStencilState = &depth;
  info.pColorBlendState = &blend;
  info.pDynamicState = &dynamic;
  info.layout = id_layouts[draw.layout];
  if (vk::vkCreateGraphicsPipelines(context.device, pipeline_cache, 1, &info, nullptr,
                                    &it->second) != VK_SUCCESS) {
    it->second = VK_NULL_HANDLE;
  }
  return it->second;
}

void Passes::State::DrawIds(VkCommandBuffer commands) {
  if (!id_image.image &&
      (!CreateImage(id_image, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R32_UINT,
                    {main_view_extent.width, main_view_extent.height, 1}, 1,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                    VK_IMAGE_ASPECT_COLOR_BIT) ||
       !CreateImage(id_depth, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, kDepthFormat,
                    {main_view_extent.width, main_view_extent.height, 1}, 1,
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, kDepthStencilAspects))) {
    return;
  }
  EndRendering(commands);
  VkImageLayout color_layout = VK_IMAGE_LAYOUT_UNDEFINED, depth_layout = VK_IMAGE_LAYOUT_UNDEFINED;
  Transition(commands, id_image.image, VK_IMAGE_ASPECT_COLOR_BIT, color_layout,
             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
  Transition(commands, id_depth.image, kDepthStencilAspects, depth_layout,
             VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
  // Depth cleared to far: 0 for the game's reversed depth (greater tests).
  bool reversed = false;
  for (const IdDraw& draw : id_draws) {
    const gpu::CompareOp z_func = gpu::CompareOp(draw.key.z_func);
    reversed |= draw.key.z_enable && (z_func == gpu::CompareOp::kGreater ||
                                      z_func == gpu::CompareOp::kGreaterOrEqual);
  }
  VkRenderingAttachmentInfo color{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  color.imageView = id_image.view;
  color.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color.clearValue.color.uint32[0] = UINT32_MAX;
  VkRenderingAttachmentInfo depth{VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO};
  depth.imageView = id_depth.view;
  depth.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  depth.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depth.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depth.clearValue.depthStencil = {reversed ? 0.0f : 1.0f, 0};
  VkRenderingInfo info{VK_STRUCTURE_TYPE_RENDERING_INFO};
  info.renderArea = {{0, 0}, main_view_extent};
  info.layerCount = 1;
  info.colorAttachmentCount = 1;
  info.pColorAttachments = &color;
  info.pDepthAttachment = &depth;
  info.pStencilAttachment = &depth;
  vk::vkCmdBeginRendering(commands, &info);
  for (const IdDraw& draw : id_draws) {
    VkPipeline pipeline = GetIdPipeline(draw);
    if (!pipeline) {
      continue;
    }
    VkRect2D scissor = draw.scissor;
    scissor.extent.width = std::min<uint32_t>(
        scissor.extent.width,
        uint32_t(std::max(0, int32_t(main_view_extent.width) - scissor.offset.x)));
    scissor.extent.height = std::min<uint32_t>(
        scissor.extent.height,
        uint32_t(std::max(0, int32_t(main_view_extent.height) - scissor.offset.y)));
    vk::vkCmdSetViewport(commands, 0, 1, &draw.viewport);
    vk::vkCmdSetScissor(commands, 0, 1, &scissor);
    vk::vkCmdSetDepthBias(commands, draw.bias_constant, 0.0f, draw.bias_slope);
    vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    VkPipelineLayout layout = id_layouts[draw.layout];
    vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 4,
                                draw.sets, kConstantBlocks, draw.offsets);
    vk::vkCmdPushConstants(commands, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 4, &draw.number);
    vk::vkCmdBindIndexBuffer(commands, arena.buffer(), draw.index_offset, draw.index_type);
    if (draw.vertex_bindings) {
      VkBuffer buffers[kMaxVertexBindings];
      std::fill(buffers, buffers + draw.vertex_bindings, arena.buffer());
      vk::vkCmdBindVertexBuffers(commands, 0, draw.vertex_bindings, buffers,
                                 draw.vertex_offsets);
    }
    vk::vkCmdDrawIndexed(commands, draw.index_count, 1, 0, draw.base_vertex, 0);
  }
  vk::vkCmdEndRendering(commands);
  recorded.valid = false;
  Transition(commands, id_image.image, VK_IMAGE_ASPECT_COLOR_BIT, color_layout,
             VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  id_drawn = true;
}

void Passes::DrawIndexed(VkCommandBuffer commands, uint32_t type, uint32_t base_start,
                         uint32_t primitive_count) {
  State& s = *state_;
  uint32_t device = guest::LoadU32(kDevicePointer);
  s.ReadState(device);
  DrawCall call;
  call.type = type;
  call.base_start = base_start;
  call.primitive_count = primitive_count;
  s.Draw(commands, device, call);
}

void Passes::InlineDraw(VkCommandBuffer commands, uint32_t type, uint32_t count,
                        uint32_t stride, uint32_t data) {
  State& s = *state_;
  uint32_t size = count * stride;
  // In the host's byte order: inline vertices are 8-in-32, as BeginVertices'
  // fetch constant has them.
  uint32_t offset = count && stride
                        ? s.arena.PlaceInline(guest::Pointer(data), size,
                                              uint32_t(xenos::Endian::k8in32))
                        : UINT32_MAX;
  if (offset == UINT32_MAX) {
    s.stats.failed++;
    s.stats.failed_by[7]++;
    return;
  }
  DrawCall call;
  call.type = type;
  call.inline_vertices = true;
  call.vertex_count = count;
  call.vertex_address = offset;
  call.inline_stride = stride;
  s.stats.mirrored_bytes += size;
  uint32_t device = guest::LoadU32(kDevicePointer);
  s.ReadState(device);
  // What BeginVertices wrote to the GPU instead of the shadow.
  uint32_t* fetch = s.regs.fetch;
  fetch[2 * (kInlineFetchConstant - 2)] = 1;
  fetch[2 * (kInlineFetchConstant - 2) + 1] = 0;
  fetch[2 * (kInlineFetchConstant - 1)] = 1;
  fetch[2 * (kInlineFetchConstant - 1) + 1] = 0;
  fetch[2 * kInlineFetchConstant] = call.vertex_address | 3;
  fetch[2 * kInlineFetchConstant + 1] =
      0x10000000u | ((size / 4) << 2) | uint32_t(xenos::Endian::k8in32);
  s.Draw(commands, device, call);
}

void Passes::Clear(VkCommandBuffer commands, uint32_t flags, uint32_t color, float z,
                   uint32_t stencil, bool rects) {
  State& s = *state_;
  // Flags: bits 0-3 render targets 0-3, 0x10 Z, 0x20 stencil; color a
  // D3DCOLOR. Rects aren't kept: the whole target is cleared.
  s.stats.clears++;
  if (rects) {
    s.stats.clear_rects++;
  }
  if (flags & 0xF) {
    VkClearValue value{};
    value.color = {{float((color >> 16) & 0xFF) / 255.0f, float((color >> 8) & 0xFF) / 255.0f,
                    float(color & 0xFF) / 255.0f, float(color >> 24) / 255.0f}};
    if (flags & 0xE) {
      s.stats.extra_targets++;
    }
    if (uint32_t object = render::BoundTargets().color[0]; object && (flags & 1)) {
      if (Target* target = s.GetTarget(commands, object, false)) {
        s.ClearTarget(commands, *target, value);
      }
    }
  }
  if (flags & 0x30) {
    uint32_t object = render::BoundTargets().depth;
    Target* target = object ? s.GetTarget(commands, object, true) : nullptr;
    if (target) {
      VkClearValue value{};
      value.depthStencil.depth = z;
      value.depthStencil.stencil = stencil & 0xFF;
      s.ClearTarget(commands, *target, value,
                    ((flags & 0x10) ? VK_IMAGE_ASPECT_DEPTH_BIT : 0) |
                        ((flags & 0x20) ? VK_IMAGE_ASPECT_STENCIL_BIT : 0));
    }
  }
}

void Passes::Resolve(VkCommandBuffer commands, uint32_t flags, const int32_t* rect,
                     uint32_t destination, const int32_t* point, bool level_or_slice,
                     const float* clear_color, float clear_z, uint32_t clear_stencil) {
  State& s = *state_;
  s.Resolve(commands, guest::LoadU32(kDevicePointer), flags, rect, destination, point,
            level_or_slice, clear_color, clear_z, clear_stencil, true);
}

void Passes::State::Resolve(VkCommandBuffer commands, uint32_t device,
                            uint32_t flags, const int32_t* rect, uint32_t destination,
                            const int32_t* point, bool level_or_slice, const float* clear_color,
                            float clear_z, uint32_t clear_stencil, bool apply_clears) {
  // Flags as the XDK's D3DRESOLVE ones, likely: bits 0-1 render target, 0x4
  // depth instead, 0x100 then clear the render target, 0x200 clear depth
  // and stencil, bits 26-31 exponent bias.
  constexpr uint32_t kKnownFlags = 0xFC000000u | 0x300 | 0x7;
  if (flags & ~kKnownFlags) {
    stats.resolve_unknown_flags++;
    stats.resolve_flags_sample = flags;
  }
  bool from_depth = flags & 4;
  uint32_t color_object = render::BoundTargets().color[flags & 3];
  uint32_t depth_object = render::BoundTargets().depth;
  uint32_t source_object = from_depth ? depth_object : color_object;
  Target* source = source_object ? GetTarget(commands, source_object, from_depth) : nullptr;
  if (source && destination && !level_or_slice &&
      ResolveInto(commands, *source, destination, rect, point, flags)) {
    stats.resolves++;
  } else {
    stats.resolves_skipped++;
  }
  if (!apply_clears) {
    return;
  }
  if ((flags & 0x100) && color_object) {
    if (Target* target = GetTarget(commands, color_object, false)) {
      VkClearValue value{};
      for (uint32_t c = 0; clear_color && c < 4; c++) {
        value.color.float32[c] = clear_color[c];
      }
      ClearTarget(commands, *target, value);
    }
  }
  if ((flags & 0x200) && depth_object) {
    if (Target* target = GetTarget(commands, depth_object, true)) {
      VkClearValue value{};
      value.depthStencil.depth = clear_z;
      value.depthStencil.stencil = clear_stencil & 0xFF;
      ClearTarget(commands, *target, value);
    }
  }
}

void Passes::BeginTiling(VkCommandBuffer commands, uint32_t width, uint32_t height,
                         float clear) {
  State& s = *state_;
  if (s.logged_tilings.insert(uint64_t(width) << 32 | height).second) {
    ITC_LOG_INFO("native renderer: begin tiling: {}x{}; clear color and Z {}", width, height,
                 clear);
  }
  s.EndRendering(commands);
  s.tiling = true;
  s.tiling_extent = {std::min(width, 8192u), std::min(height, 8192u)};
  s.stats.tilings++;
  // Each tile starts cleared to these.
  if (uint32_t object = render::BoundTargets().color[0]) {
    if (Target* target = s.GetTarget(commands, object, false)) {
      VkClearValue value{};
      value.color = {{clear, clear, clear, clear}};
      s.ClearTarget(commands, *target, value);
    }
  }
  if (uint32_t object = render::BoundTargets().depth) {
    if (Target* target = s.GetTarget(commands, object, true)) {
      VkClearValue value{};
      value.depthStencil = {clear, 0};
      s.ClearTarget(commands, *target, value);
    }
  }
}

void Passes::EndTiling(VkCommandBuffer commands, uint32_t flags, uint32_t destination,
                       float clear) {
  State& s = *state_;
  // The tiled target drawn whole: one resolve of all of it (the library's
  // EndTiling resolves each tile to its place, the same rectangle in all),
  // then the clears its flags ask for (the library's last tile's: it
  // clears the target after each tile for the next).
  const int32_t rect[4] = {0, 0, int32_t(s.tiling_extent.width),
                           int32_t(s.tiling_extent.height)};
  const int32_t point[2] = {0, 0};
  const float color[4] = {clear, clear, clear, clear};
  if (s.logged_tiled_resolves.insert(uint64_t(destination) << 32 | flags).second) {
    ITC_LOG_INFO("native renderer: end tiling: flags {:08X}, {}x{} to {:08X}", flags,
                 rect[2], rect[3], destination);
  }
  s.Resolve(commands, guest::LoadU32(kDevicePointer), flags, rect, destination, point, false,
            color, clear, 0, true);
  s.stats.tiled_resolves++;
  s.tiling = false;
}

void Passes::FrontBufferResolve(VkCommandBuffer commands, uint32_t device,
                                uint32_t front_buffer) {
  State& s = *state_;
  uint32_t object = render::BoundTargets().color[0];
  Target* target = object ? s.GetTarget(commands, object, false) : nullptr;
  // The front buffer's size, from its fetch constant's word 2 (2D: width - 1
  // in bits 0-12, height - 1 in bits 13-25).
  uint32_t size = front_buffer ? guest::LoadU32(front_buffer + 28 + 8) : 0;
  s.front_extent = {(size & 0x1FFF) + 1, ((size >> 13) & 0x1FFF) + 1};
  s.front = target;
  if (!s.logged_front) {
    s.logged_front = true;
    Surface surface = object ? ReadSurface(object) : Surface{};
    uint32_t format = front_buffer ? guest::LoadU32(front_buffer + 28 + 4) & 0x3F : 0;
    ITC_LOG_INFO("native renderer: front buffer resolve: device {:08X} (ours {:08X}), render "
                 "target 0 {:08X} (pitch {}, MSAA {}, format {}, EDRAM base {}), into {:08X}: "
                 "{}x{} format {}",
                 device, guest::LoadU32(kDevicePointer), object, surface.pitch, surface.msaa,
                 surface.format, surface.edram_base, front_buffer, s.front_extent.width,
                 s.front_extent.height, format);
  }
}

void Passes::State::LogGlow() {
  bool logic = Renderer_ReplayingLogicList();
  GlowCounts& counts = glow_counts[logic];
  counts.frames++;
  counts.cross_fades += glow_frame.cross_fades;
  counts.history_resolves += glow_frame.history_resolves;
  counts.reads_history += glow_frame.cross_fades && glow_frame.history_resolves &&
                          glow_frame.cross_fade_texture == glow_frame.history_address;
  if (glow_frame.cross_fades) {
    glow_constants[logic] = glow_frame.constants;
  }
  if (glow_addresses[logic].empty()) {
    for (uint32_t address : glow_frame.addresses) {
      glow_addresses[logic] += std::format("{:08X} ", address);
    }
  }
  // A frame: L (logic) or r, then x if it cross-faded, h if it wrote the
  // history back.
  if (glow_pattern.size() < 120) {
    glow_pattern += logic ? 'L' : 'r';
    if (glow_frame.cross_fades) {
      glow_pattern += 'x';
    }
    if (glow_frame.history_resolves) {
      glow_pattern += 'h';
    }
    glow_pattern += ' ';
  }
  auto now = std::chrono::steady_clock::now();
  if (now - glow_last_log >= std::chrono::seconds(1)) {
    glow_last_log = now;
    ITC_LOG_INFO("native renderer: glow: logic frames {} (cross-fades {}, history resolves {}, "
                 "reading the history {}), render-only frames {} ({}, {}, {}); last read "
                 "{:08X}, last history {:08X}; 256x144 resolves of a logic frame: {}, of a "
                 "render-only frame: {}; frames: {}",
                 glow_counts[1].frames, glow_counts[1].cross_fades,
                 glow_counts[1].history_resolves, glow_counts[1].reads_history,
                 glow_counts[0].frames, glow_counts[0].cross_fades,
                 glow_counts[0].history_resolves, glow_counts[0].reads_history,
                 glow_frame.cross_fade_texture, glow_frame.history_address, glow_addresses[1],
                 glow_addresses[0], glow_pattern);
    ITC_LOG_INFO("native renderer: glow cross-fade constants: logic frame: {}render-only "
                 "frame: {}",
                 glow_constants[1], glow_constants[0]);
    glow_counts = {};
    glow_addresses = {};
    glow_pattern.clear();
  }
  glow_frame.cross_fades = glow_frame.history_resolves = 0;
  glow_frame.history_address = 0;
  glow_frame.addresses.clear();
}

void Passes::set_log_glow(bool log) {
  state_->log_glow = log;
}

void Passes::set_skip_pixel_shader(uint64_t hash) {
  state_->skip_pixel_shader = hash;
}

void Passes::set_skip_motion_blur(bool skip) {
  state_->skip_motion_blur = skip;
}

void Passes::set_record_draws(bool record) {
  state_->record_draws = record;
}

std::vector<uint64_t> Passes::main_view_pixel_shaders() const {
  return state_->last_pixel_shaders;
}

void Passes::set_skip_repeated_state(bool skip) {
  state_->skip_repeated_state = skip;
}

void Passes::set_reuse_constants(bool reuse) {
  state_->reuse_constants = reuse;
}

void Passes::set_log_constant_changes(bool log) {
  state_->log_constant_changes = log;
}

void Passes::set_reuse_texture_sets(bool reuse) {
  state_->reuse_texture_sets = reuse;
}

void Passes::set_log_texture_bindings(bool log) {
  state_->log_texture_bindings = log;
}

std::string Passes::TakeTextureBindings() {
  State& s = *state_;
  State::TextureBindingCounts& c = s.binding_counts;
  TextureCache::GenerationEvents e = s.textures.TakeGenerationEvents();
  double frames = double(std::max<uint64_t>(c.frames, 1));
  auto share = [](uint64_t n, uint64_t of) { return 100.0 * double(n) / double(std::max<uint64_t>(of, 1)); };
  uint64_t generations = s.textures.generation() - c.generation;
  std::string text = std::format(
      "texture bindings: {:.1f} draws a frame ({:.1f} with textures or samplers), repeating the "
      "last draw's shaders and the fetch constants they read {:.1f}% ({:.1f}% ignoring "
      "generations; vertex stage {:.1f}%, pixel {:.1f}%); {:.1f} views a frame, the same as the "
      "fetch constant's last {:.1f}% ({:.1f}% ignoring generations), seen this generation "
      "{:.1f}%; {:.1f} samplers a frame, the same as the fetch constant's last {:.1f}%, seen "
      "before {:.1f}%; {:.1f} sets a frame, found this frame {:.1f}%; in {} frames, {} "
      "generations (frames with textures marked written {}, resolves changing an address's "
      "image {}), {} textures made",
      c.draws / frames, c.draws_textured / frames, share(c.draws_same, c.draws),
      share(c.draws_same_words, c.draws), share(c.vertex_same, c.draws),
      share(c.pixel_same, c.draws), c.views / frames, share(c.view_same_slot, c.views),
      share(c.view_same_slot_words, c.views), share(c.view_seen, c.views), c.samplers / frames,
      share(c.sampler_same_slot, c.samplers), share(c.sampler_seen, c.samplers), c.sets / frames,
      share(c.sets_found, c.sets), c.frames, generations, e.marked_frames,
      e.resolve_changes, e.created);
  c = {};
  c.generation = s.textures.generation();
  return text;
}

void Passes::set_cache_shader_lookups(bool cache) {
  state_->cache_shader_lookups = cache;
}

namespace {

// The float constant registers a shader reads, in packed order.
// The registers a shader is given, in its block's order.
std::vector<uint32_t> FloatRegisters(const TranslatedShader* shader) {
  std::vector<uint32_t> registers;
  for (uint32_t i = 0; shader && i < shader->float_count; i++) {
    registers.push_back(shader->float_first + i);
  }
  return registers;
}

// "name: word 3 a -> b, ..." for the words that differ (at most 4 shown).
std::string DescribeChange(const char* name, const std::vector<uint32_t>& before,
                           const std::vector<uint32_t>& after,
                           const std::vector<uint32_t>* float_registers) {
  std::string text;
  uint32_t shown = 0, changed = 0;
  for (size_t i = 0; i < before.size() && i < after.size(); i++) {
    if (before[i] == after[i]) {
      continue;
    }
    if (float_registers) {
      // The camera moves in its last digits even standing still: ignore
      // changes under 0.1%.
      float a, b;
      std::memcpy(&a, &before[i], 4);
      std::memcpy(&b, &after[i], 4);
      if (std::abs(a - b) <= 1e-3f * std::max(1.0f, std::abs(a))) {
        continue;
      }
    }
    changed++;
    if (shown++ < 4) {
      float a, b;
      std::memcpy(&a, &before[i], 4);
      std::memcpy(&b, &after[i], 4);
      if (float_registers) {
        text += std::format(" c{}.{} {:.4g}->{:.4g}", (*float_registers)[i / 4], "xyzw"[i % 4], a,
                            b);
      } else {
        text += std::format(" word {} {:08X}->{:08X}", i, before[i], after[i]);
      }
    }
  }
  return changed ? std::format("; {} ({} words):{}", name, changed, text) : std::string();
}

}  // namespace

void Passes::State::WatchDraw(const TranslatedShader* vertex, const TranslatedShader* pixel,
                              uint32_t index_address, uint32_t primitive_count,
                              const PipelineKey& key, const uint32_t* system,
                              const uint32_t* vertex_floats, const uint32_t* pixel_floats,
                              const uint32_t* bool_loop, const uint32_t* fetch) {
  uint64_t ps = pixel ? pixel->hash : 0;
  uint32_t occurrence =
      watched_this_frame[{vertex->hash, ps, index_address, primitive_count}]++;
  WatchedDraw now;
  now.frame = frames;
  now.system.assign(system, system + sizeof(DrawConstants) / 4);
  std::vector<uint32_t> vertex_registers = FloatRegisters(vertex);
  std::vector<uint32_t> pixel_registers = FloatRegisters(pixel);
  now.vertex_floats.assign(vertex_floats, vertex_floats + 4 * vertex_registers.size());
  now.pixel_floats.assign(pixel_floats, pixel_floats + 4 * pixel_registers.size());
  now.bool_loop.assign(bool_loop, bool_loop + kBoolLoopConstantsRange / 4);
  // Only the fetch constants the shaders read (the space is shared: texture
  // slot n is vertex buffers 3n-3n+2); unused ones are zeroed.
  now.fetch.assign(kFetchConstantsBytes / 4, 0);
  for (const TranslatedShader* shader : {vertex, pixel}) {
    for (size_t i = 0; shader && i < shader->textures.size(); i++) {
      uint32_t slot = shader->textures[i].fetch_constant;
      std::copy(fetch + 6 * slot, fetch + 6 * slot + 6, now.fetch.begin() + 6 * slot);
    }
  }
  for (uint32_t k = 0; k < 96; k++) {
    if (vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31))) {
      std::copy(fetch + 2 * k, fetch + 2 * k + 2, now.fetch.begin() + 2 * k);
    }
  }
  now.key.assign(reinterpret_cast<const char*>(&key), sizeof(key));
  watched_draws++;
  WatchedDraw& last = watched[{vertex->hash, ps, index_address, primitive_count, occurrence}];
  if (last.frame + 1 == frames) {
    std::string change =
        DescribeChange("system", last.system, now.system, nullptr) +
        DescribeChange("vertex floats", last.vertex_floats, now.vertex_floats,
                       &vertex_registers) +
        DescribeChange("pixel floats", last.pixel_floats, now.pixel_floats, &pixel_registers) +
        DescribeChange("bool/loop", last.bool_loop, now.bool_loop, nullptr) +
        DescribeChange("fetch (used)", last.fetch, now.fetch, nullptr) +
        (last.key != now.key ? std::string("; pipeline") : std::string());
    if (!change.empty()) {
      watched_changes++;
      if (watch_lines.size() < (watch_pixel_shader ? 200u : 40u)) {
        watch_lines.push_back(std::format("vs {:016X} ps {:016X} indices {:08X} x{} #{}{}",
                                          vertex->hash, ps, index_address, primitive_count,
                                          occurrence, change));
      }
    }
  }
  last = std::move(now);
}

void Passes::set_verify_textures(bool every_frame) {
  state_->textures.set_verify_every_frame(every_frame);
}

void Passes::set_base_level_only(bool base_only) {
  state_->textures.set_base_level_only(base_only);
}

std::string Passes::TakeInterfaces() {
  State& s = *state_;
  std::string text;
  for (const auto& [hash, interpolators] : s.param_gen_by_shader) {
    text += std::format(" ps {:016X} param gen in {:04X};", hash, interpolators);
  }
  for (const auto& [hash, masks] : s.interpolators_by_shader) {
    text += std::format(" ps {:016X} interpolators {:08X};", hash, masks);
  }
  s.param_gen_by_shader.clear();
  s.interpolators_by_shader.clear();
  return text;
}


void Passes::set_hand_over_resolves(bool hand_over) {
  state_->hand_over_resolves = hand_over;
}

void Passes::set_hide_cubes(bool hide) {
  state_->hide_cubes = hide;
}

void Passes::set_watch_draws(bool watch, uint64_t pixel_shader) {
  state_->watch_draws = watch;
  state_->watch_pixel_shader = pixel_shader;
}

std::vector<std::string> Passes::TakeWatchLines(uint64_t& changes, uint64_t& draws) {
  State& s = *state_;
  changes = s.watched_changes;
  draws = s.watched_draws;
  s.watched_changes = s.watched_draws = 0;
  return std::exchange(s.watch_lines, {});
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
  if (Buffers_LogPhysicalWrites()) {
    Buffers_OnFrameEnd();
  }
  s.last_frame_resolves = s.frame_resolves.size();
  if (s.log_glow) {
    s.LogGlow();
  }
  if (s.check_late_writes && s.drew_any) {
    for (const State::CopiedRange& r : s.copied_ranges) {
      s.late_checked++;
      if (base::Hash64(PhysicalHostAddress(r.address), r.size) != r.hash) {
        s.late_written++;
        s.late_by_shader[r.vertex_shader]++;
        if (s.late_sample.empty()) {
          s.late_sample = std::format("{} {:08X} ({} bytes) of vs {:016X}",
                                      r.index ? "index" : "vertex", r.address, r.size,
                                      r.vertex_shader);
        }
      }
    }
  }
  s.copied_ranges.clear();
  s.last_pixel_shaders = s.frame_pixel_shaders;
  // The frame's resolves, now and then, to pick one to show.
  if (s.frames++ % 1200 == 600 && !s.frame_resolves.empty()) {
    std::string list;
    for (size_t i = 0; i < s.frame_resolves.size(); i++) {
      const State::Resolved& r = s.frame_resolves[i];
      list += std::format("{}{}: {:08X} format {} {}x{} flags {:X}", i ? "; " : "", i, r.address,
                          r.format, r.extent.width, r.extent.height, r.flags);
    }
    ITC_LOG_INFO("native renderer: this frame's resolves: {}", list);
  }
  VkImage shown = s.shown.image;
  Transition(commands, shown, VK_IMAGE_ASPECT_COLOR_BIT, s.shown_layout,
             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  VkExtent2D extent = s.main_view_extent;
  if (show_resolve >= 0 && size_t(show_resolve) < s.frame_resolves.size() &&
      s.Held(s.frame_resolves[show_resolve]) &&
      s.frame_resolves[show_resolve].aspect == VK_IMAGE_ASPECT_COLOR_BIT) {
    // A resolve destination, as is.
    const State::Resolved& resolved = s.frame_resolves[show_resolve];
    extent = {std::min(resolved.extent.width, s.shown_extent.width),
              std::min(resolved.extent.height, s.shown_extent.height)};
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
  } else if (Target* front = s.front) {
    // The frame as the game resolves it to the front buffer.
    s.EndRendering(commands);
    s.RestoreContents(commands, *front);
    Transition(commands, front->image.image, front->aspects(), front->layout,
               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    VkExtent2D front_extent = s.Scaled(s.front_extent);
    extent = {std::min({front_extent.width, front->extent.width, s.shown_extent.width}),
              std::min({front_extent.height, front->extent.height, s.shown_extent.height})};
    if (front->samples == VK_SAMPLE_COUNT_1_BIT) {
      VkImageBlit blit{};
      blit.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
      blit.srcOffsets[1] = {int32_t(extent.width), int32_t(extent.height), 1};
      blit.dstSubresource = blit.srcSubresource;
      blit.dstOffsets[1] = blit.srcOffsets[1];
      vk::vkCmdBlitImage(commands, front->image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, shown,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_NEAREST);
    } else if (front->format == kShownFormat) {  // a resolve doesn't convert
      VkImageResolve region{};
      region.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
      region.dstSubresource = region.srcSubresource;
      region.extent = {extent.width, extent.height, 1};
      vk::vkCmdResolveImage(commands, front->image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                            shown, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }
  } else if (Target* main = s.main_view) {
    // The main view's drawn part, its samples averaged.
    Transition(commands, main->image.image, main->aspects(), main->layout,
               VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    extent = {std::min(s.main_view_extent.width, main->extent.width),
              std::min(s.main_view_extent.height, main->extent.height)};
    VkImageResolve region{};
    region.srcSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    region.dstSubresource = region.srcSubresource;
    region.extent = {extent.width, extent.height, 1};
    vk::vkCmdResolveImage(commands, main->image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, shown,
                          VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  } else {
    // No main view this frame (menus): dark blue.
    VkClearColorValue color{{0.0f, 0.05f, 0.15f, 1.0f}};
    VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vk::vkCmdClearColorImage(commands, shown, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1,
                             &range);
  }
  if (s.gamma_pipeline) {
    s.ApplyGamma(commands);
  }
  Transition(commands, shown, VK_IMAGE_ASPECT_COLOR_BIT, s.shown_layout,
             VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  if (dump) {
    if (!s.id_draws.empty()) {
      s.DrawIds(commands);
    }
    s.RecordDump(commands, extent, dump_prefix, dump_resolves);
    if (dump_resolves && s.drew_any) {
      s.DumpGuestResolves();
    }
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
    case VK_FORMAT_R32_UINT:
      std::memcpy(&w, p, 4);
      out[0] = out[1] = out[2] = w == UINT32_MAX ? -1.0f : float(w);
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
  std::vector<VkImage> images = {shown.image};
  std::vector<VkImageAspectFlags> aspects = {VK_IMAGE_ASPECT_COLOR_BIT};
  for (size_t i = 0; with_resolves && i < frame_resolves.size(); i++) {
    const Resolved& r = frame_resolves[i];
    if (!Held(r)) {
      continue;  // a handed image back with its target since
    }
    add(std::format("resolve_{:02}_{:08X}_format{}", i, r.address, r.format), r.host_format,
        r.extent);
    images.push_back(r.image);
    aspects.push_back(r.aspect);
  }
  size_t resolve_end = images.size();
  if (id_drawn) {
    add("draw_ids", VK_FORMAT_R32_UINT, main_view_extent);
    images.push_back(id_image.image);
    aspects.push_back(VK_IMAGE_ASPECT_COLOR_BIT);
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
    VkImage image = images[i];
    VkImageLayout layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    bool resolve = i && i < resolve_end;  // in SHADER_READ_ONLY_OPTIMAL
    // A depth image's both aspects change layout; its depth is copied.
    VkImageAspectFlags transition_aspects =
        aspects[i] == VK_IMAGE_ASPECT_DEPTH_BIT ? kDepthStencilAspects : aspects[i];
    if (resolve) {
      Transition(commands, image, transition_aspects, layout,
                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    }
    VkBufferImageCopy copy{};
    copy.bufferOffset = dumped[i].offset;
    copy.imageSubresource = {aspects[i], 0, 0, 1};
    copy.imageExtent = {dumped[i].extent.width, dumped[i].extent.height, 1};
    vk::vkCmdCopyImageToBuffer(commands, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dump_buffer,
                               1, &copy);
    if (resolve) {
      Transition(commands, image, transition_aspects, layout,
                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
  }
}

// Each of the frame's resolve destinations as guest memory holds it at
// the end of the frame (the last resolve to each address): with
// --readback_resolve=full, the emulated GPU's result, to compare ours with.
void Passes::State::DumpGuestResolves() {
  std::unordered_set<uint32_t> done;
  for (size_t i = frame_resolves.size(); i-- > 0;) {
    const Resolved& r = frame_resolves[i];
    if (!done.insert(r.address).second) {
      continue;
    }
    Dumped d;
    uint32_t width, height;
    if (!TextureCache::DecodeGuest(r.fetch, d.cpu, d.format, width, height)) {
      continue;
    }
    d.name = std::format("guest_resolve_{:02}_{:08X}_format{}", i, r.address, r.format);
    d.extent = {width, height};
    d.offset = 0;
    dumped.push_back(std::move(d));
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
      ITC_LOG_ERROR("native renderer: couldn't write {}", path);
      continue;
    }
    // PFM: rows bottom to top, little-endian floats.
    std::fprintf(file, "PF\n%u %u\n-1.0\n", d.extent.width, d.extent.height);
    uint32_t bpp = BytesPerPixel(d.format);
    std::vector<float> row(size_t(d.extent.width) * 3);
    const uint8_t* source = d.cpu.empty() ? data + d.offset : d.cpu.data();
    for (uint32_t y = d.extent.height; y-- > 0;) {
      for (uint32_t x = 0; x < d.extent.width; x++) {
        ToRgb(d.format, source + (size_t(y) * d.extent.width + x) * bpp, &row[3 * x]);
      }
      std::fwrite(row.data(), sizeof(float), row.size(), file);
    }
    std::fclose(file);
  }
  ITC_LOG_INFO("native renderer: dumped {} images into {}", s.dumped.size(), directory);
  if (!s.recorded_draws.empty()) {
    if (FILE* file = std::fopen((directory + "/draws.txt").c_str(), "w")) {
      for (size_t n = 0; n < s.recorded_draws.size(); n++) {
        const State::RecordedDraw& r = s.recorded_draws[n];
        std::string line = std::format(
            "draw {}: {} target pitch {} MSAA {} format {}, color {}, depth {}; vs {:016X} ps "
            "{:016X}; primitive {} x{}{}",
            n, r.main_view ? "main view" : "other", r.surface.pitch, r.surface.msaa,
            r.surface.format, r.has_color, r.has_depth, r.vertex, r.pixel, r.type, r.count,
            r.inline_vertices ? " inline" : "");
        const auto& rs = r.render_state;
        float blend[4], alpha_ref;
        std::memcpy(blend, &rs[5], 16);
        std::memcpy(&alpha_ref, &rs[9], 4);
        line += std::format(
            "; depthcontrol {:08X} blendcontrol {:08X} colorcontrol {:08X} modecontrol {:08X} "
            "color mask {:08X} blend constant {:.3g} {:.3g} {:.3g} {:.3g} alpha ref {:.3g}",
            rs[0], rs[1], rs[2], rs[3], rs[4], blend[0], blend[1], blend[2], blend[3], alpha_ref);
        for (const auto& v : r.vertex_fetches) {
          line += std::format("; vfetch {}: {:08X} {:08X} (address {:08X}, {} bytes)", v[0], v[1],
                              v[2], v[1] & 0x1FFFFFFCu, ((v[2] >> 2) & 0xFFFFFFu) * 4);
        }
        for (const State::RecordedDraw::Texture& t : r.textures) {
          const uint32_t* f = t.fetch;
          line += std::format(
              "; {} tex {}: {:08X} format {} {}x{} dim {} declared {} words {:08X} {:08X} "
              "{:08X} {:08X} {:08X} {:08X}",
              t.pixel ? "ps" : "vs", t.texture.fetch_constant,
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
  stats.shaders_ported = state_->shaders->ported();
  uint32_t pipelines = state_->stats.pipelines, targets = state_->stats.targets;
  state_->stats = {};
  state_->stats.pipelines = pipelines;
  state_->stats.targets = targets;
  return stats;
}

}  // namespace native
