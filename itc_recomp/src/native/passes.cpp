#include "passes.h"
#include "frame_costs.h"
#include "log_enabled.h"
#include "disk_cache.h"

#include "generated_gamma_comp.h"
#include "generated_id_frag.h"
#include "xbox_gamma_table.h"
#include "generated_resolve_frag.h"
#include "generated_resolve_ms_frag.h"
#include "generated_resolve_vert.h"

#include "buffers.h"
#include "guest.h"
#include "renderer.h"
#include "shaders.h"
#include "state.h"

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
#include <rex/graphics/pipeline/shader/spirv_translator.h>
#include <rex/graphics/register_file.h>
#include <rex/graphics/registers.h>
#include <rex/graphics/util/draw.h>
#include <rex/graphics/xenos.h>
#include <rex/cvar.h>
#include <rex/hash.h>
#include <rex/logging.h>

REXCVAR_DECLARE(std::string, log_level);

REXCVAR_DEFINE_STRING(xbox_gamma, "off", "ITC",
                      "Show the frame through the display gamma table the game sets for a kind "
                      "of display, as the Xbox 360's video output does (native renderer): off "
                      "(a PC monitor, sRGB: no change), hdtv (darker mid-tones and shadows), "
                      "power (lighter shadows)");

namespace native {

namespace graphics = rex::graphics;
namespace reg = rex::graphics::reg;
namespace xenos = rex::graphics::xenos;
namespace draw_util = rex::graphics::draw_util;
using Translator = graphics::SpirvShaderTranslator;
using graphics::XE_GPU_REG_RB_ALPHA_REF;
using graphics::XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031;
using graphics::XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0;
using graphics::XE_GPU_REG_SQ_CONTEXT_MISC;
using graphics::XE_GPU_REG_SQ_PROGRAM_CNTL;
using graphics::XE_GPU_REG_VGT_INDX_OFFSET;
using graphics::XE_GPU_REG_VGT_MAX_VTX_INDX;
using graphics::XE_GPU_REG_VGT_MIN_VTX_INDX;

namespace {

// Every depth surface (24_8 and 24_8_FLOAT) is drawn as this.
constexpr VkFormat kDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

// The main view, which is shown: its surface (pitch 1040, 2x MSAA, 7e3
// colour, drawn as RGBA16F) and the part of it drawn (its viewport).
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

// Per frame in flight: the draws' constants, in blocks of this size (about
// 7,000 draws each); a frame that fills its blocks gets another, kept for
// later frames.
constexpr VkDeviceSize kConstantBlockSize = 64u << 20;
// Fixed descriptor ranges of the constant buffers (set 1), so one set per
// frame serves every draw through dynamic offsets.
constexpr uint32_t kFloatConstantsRange = 256 * 16;
constexpr uint32_t kBoolLoopConstantsRange = 40 * 4;
constexpr uint32_t kFetchConstantsRange = 32 * 6 * 4;

// Past guest memory, each mirror holds data of ours that the translated
// shaders and draws read like guest memory: built-in index buffers, then
// the frame's inline vertices (an inline draw's vertex fetch constant
// points there; the translator doesn't mask the address).
constexpr uint32_t kBuiltinPrimitives = 4096;
// Rectangle lists as two-triangle strips (the translator's
// kRectangleListAsTriangleStrip): 32-bit, rectangle << 2 | corner, with
// restarts between rectangles, as the plugin's built-in index buffer.
constexpr uint32_t kRectangleIndices = kPhysicalMemorySize;
constexpr uint32_t RectangleIndexCount(uint32_t rectangles) {
  return 4 * rectangles + (std::max(rectangles, 1u) - 1);
}
// Quad lists as triangle lists, 32-bit: (v0, v1, v2), (v0, v2, v3) a quad.
constexpr uint32_t kQuadIndices = kRectangleIndices + 4 * RectangleIndexCount(kBuiltinPrimitives);
constexpr uint32_t kInlineVertices = kQuadIndices + 4 * 6 * kBuiltinPrimitives;
constexpr uint32_t kInlineVerticesSize = 32u << 20;
constexpr VkDeviceSize kMirrorSize = VkDeviceSize(kInlineVertices) + kInlineVerticesSize;

// An inline draw's vertices: vertex fetch constant 95 (stream 0), as the
// library's BeginVertices (sub_825BD8F0) writes it into the command buffer
// instead of the shadow, with 93 and 94 as [1, 0] (the same register write)
// and VGT_INDX_OFFSET 0; endianness 8-in-32.
constexpr uint32_t kInlineFetchConstant = 95;

// A draw: command 18's item (an indexed draw) or an inline one.
struct DrawCall {
  uint32_t type;
  // Command 18: the item's base vertex << 16 | start index, and its
  // primitive count.
  uint32_t base_start = 0, primitive_count = 0;
  // Inline vertices: their count, copied to vertex_address in the mirror.
  bool inline_vertices = false;
  uint32_t vertex_count = 0, vertex_address = 0;
  // With log_tick_repeats: a hash of the inline vertices (their address in
  // the mirror changes from frame to frame).
  uint64_t inline_hash = 0;
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

  bool operator==(const PipelineKey&) const = default;
};
// Hashed and compared as a whole (no padding to hold stray bytes).
static_assert(sizeof(PipelineKey) == 3 * sizeof(void*) + 18 * sizeof(uint32_t));
struct PipelineKeyHash {
  size_t operator()(const PipelineKey& key) const { return size_t(XXH3_64bits(&key, sizeof(key))); }
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
    uint64_t hash = XXH3_64bits(words, count * sizeof(uint64_t));
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
  VkImageAspectFlags aspects() const {
    return depth ? kDepthStencilAspects : VK_IMAGE_ASPECT_COLOR_BIT;
  }
};

// A draw as worked out, for RecordDraw to record: the dynamic state, the
// pipeline, the texture sets and the draw call.
struct DrawEmit {
  VkViewport viewport;
  VkRect2D scissor;
  float bias[2];  // constant, slope
  float blend_constants[4];
  VkPipeline pipeline;
  VkPipelineLayout layout;
  VkDescriptorSet textures[2];
  bool indexed;  // vkCmdDrawIndexed (else vkCmdDraw)
  VkIndexType index_type;
  VkDeviceSize index_offset;
  uint32_t first_index, count;
  bool depth_biased, depth_equal, inline_vertices;
  uint32_t type;
};

// Draws translated once a tick (--native_renderer_tick_draws, step 1 in
// docs/tick-replay.md): what a draw was worked out from, cheap to gather at
// every draw, and with it what it came to on the tick's first frame.
// Render-only frames give a draw whose key equals the stored one at the
// same place the stored result, packing only its constants afresh.
struct TickDrawKey {
  uint32_t call[5];      // type, base | start, primitive count, inline, vertex count
  uint32_t surfaces[6];  // colour and depth objects, each's +24 and +28
  uint32_t shaders[10];  // vertex object, +48, +600, +24, declaration id, a patched
                         // copy used; pixel object, +24, +64; tiling
  uint32_t indices[3];   // the index buffer object, its +0 and +24
  uint32_t scissor[5];   // device +11600, the rectangle
  uint64_t registers;    // a hash of the register blocks ReadRegisters fills
  uint64_t generations[2];  // the texture cache's (views, samplers)
  bool operator==(const TickDrawKey&) const = default;
};

// Texture descriptor sets (sets 2 and 3) in each of a frame's texture
// pools, and their descriptors; a frame that fills its pools gets another,
// kept for later frames.
constexpr uint32_t kTextureSetsPerPool = 8192;
constexpr uint32_t kTextureDescriptorsPerPool = 32768;

// The lowest and highest of `count` indices as the GPU reads them, leaving
// out the restart index (all ones) when `skip_restart`; {UINT32_MAX, 0} if
// there are none. One plain loop per format, so the compiler vectorises it
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
// both objects, their microcode's address and size, the id the vertex
// shader is patched for and the bound declaration's, and the registers and
// flags the interface depends on. Measured over races, a key never came back
// with other microcode: the game's Direct3D changes a vertex shader's only by
// patching it, which changes the id it's patched for. Translations are never
// freed, so their pointers stay good. An open-addressed table, at most half
// full; never emptied (a session meets ~600 keys).
class ShaderLookupTable {
 public:
  struct Key {
    uint32_t words[12];
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
    for (int i = 0; i < 12; i += 2) {
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
  // The main view's surfaces (colour and depth): pitch 1040, 2x MSAA.
  static bool IsMainViewSurface(const Surface& surface) {
    return surface.pitch == kMainViewPitch && surface.msaa == 1;
  }
  // What's shown and the main view's drawn part, scaled.
  VkExtent2D shown_extent{kShownWidth, kShownHeight};
  VkExtent2D main_view_extent{kMainViewWidth, kMainViewHeight};

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
    // Each range copied in (address << 32 | size): the memcpy write
    // sequence (Buffers_Sequence) when it was copied.
    std::unordered_map<uint64_t, uint64_t> copied;
  };
  std::vector<GuestMirror> mirrors;
  // With mirror_by_tick (two mirrors; "Hypothesis to test: the mirrors
  // updated once a tick" in docs/native-renderer.md): draws read the front
  // mirror, swapped only as a logic pass's list starts replaying; memcpy's
  // writes are applied at frame starts to the other mirror, which no frame
  // in flight reads, and each mirror keeps the writes it hasn't had yet.
  // Inline vertices get a part of their region per frame slot.
  bool mirror_by_tick = false, mirror_to_both = false;
  uint8_t* guest_base = nullptr;
  uint32_t front_mirror = 0;
  std::vector<std::pair<uint32_t, uint32_t>> mirror_pending[2], mirror_taken;
  // The writes the front mirror hasn't had, sorted by start, to count drawn
  // ranges it's missing data of (memcpy being the only writer).
  std::vector<std::pair<uint32_t, uint32_t>> front_missing;
  uint32_t front_missing_longest = 0;
  struct MirrorByTickStats {
    uint64_t frames = 0, swaps = 0, writes = 0, bytes = 0;
    uint64_t stale[2] = {}, stale_bytes[2] = {};  // [render-only list]
    uint64_t stale_lines = 200;
  } by_tick;
  // The mirror this frame's draws read.
  GuestMirror& draw_mirror() { return mirrors[mirror_by_tick ? front_mirror : frame]; }
  // Whether draws work out their vertex and index ranges and pass them to
  // Mirror: always, except with mirror_by_tick and none of the mirror's
  // diagnostics (then the mirror needs nothing from draws).
  bool MirrorPerDraw() const {
    return !mirror_by_tick || check_mirror || check_mirror_frames || check_late_writes ||
           check_clobbers || verify_mirror || log_index_ranges ||
           Buffers_Logging() || Buffers_LogPhysicalWrites();
  }
  uint32_t inline_slot_size() const {
    return mirror_by_tick ? kInlineVerticesSize / frames_in_flight : kInlineVerticesSize;
  }
  void MirrorByTickFrameStart(uint8_t* base);
  std::unordered_set<uint64_t> mirrored;  // address << 32 | size, this frame
  uint32_t inline_offset = 0;  // this frame's inline vertices so far
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
    uint32_t offsets[5];
    VkViewport viewport;
    VkRect2D scissor;
    VkDeviceSize index_offset;
    VkIndexType index_type;
    uint32_t index_count;
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
  bool no_index_clamp = false;  // debugging aid
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
  uint32_t vertex_shader_patches = 0;  // g_vertex_shader_patches at the last draw
  std::vector<std::string> declaration_lines;
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
  uint8_t* last_base = nullptr;
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
  // What DrawIndexRange learned from Buffers_NewestWrite for the draw's
  // index range (kNewestUnknown if it didn't ask), for Mirror.
  uint64_t draw_index_newest = kNewestUnknown;
  std::pair<uint32_t, uint32_t> DrawIndexRange(uint8_t* base, uint32_t address, uint32_t count,
                                               bool index_32bit, uint32_t endian,
                                               bool skip_restart);
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
  void CountIndexRange(uint32_t address, uint32_t count, bool index_32bit, uint32_t endian,
                       bool skip_restart, uint32_t low, uint32_t high);
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
  // With log_shader_lookups: how much of the shader lookup repeats, to
  // judge keying it by what the draw reads from the shader objects instead
  // of hashing the microcode (TakeShaderLookups describes it). A stage's
  // key is its object, microcode address and size, and for the vertex
  // shader the id it's patched for, the bound declaration's and whether the
  // patched copy is drawn. A key met before (this session) whose microcode
  // now hashes differently is stale: a cache by the key would have given
  // the wrong shader. Split by whether a patch (g_vertex_shader_patches)
  // came between.
  bool log_shader_lookups = false;
  struct ShaderKey {
    uint32_t object, microcode, size, patched_for, declaration_id, copy;
  };
  struct ShaderSeen {
    uint64_t hash;
    uint32_t patches;  // g_vertex_shader_patches when last seen
  };
  std::unordered_map<uint64_t, ShaderSeen> shaders_seen[2];  // by key hash; vertex, pixel
  std::unordered_set<uint64_t> lookups_seen;  // whole lookups' key hashes
  struct ShaderLookupCounts {
    uint64_t frames = 0, draws = 0, copies = 0, lookup_same = 0, lookup_seen = 0;
    uint64_t same[2] = {}, seen[2] = {}, stale[2] = {}, stale_patched[2] = {};
    ShaderKey stale_sample[2] = {};
    uint64_t stale_hashes[2][2] = {};  // the sample's hash before and now
    uint32_t patches = 0;  // g_vertex_shader_patches when last taken
  } lookup_counts;
  struct LastLookup {
    uint64_t key[2], lookup;
    bool valid;
  } last_lookup = {};
  void CountShaderLookup(const ShaderKey keys[2], const uint64_t hashes[2],
                         uint32_t program_control, uint32_t context_misc, bool point,
                         bool rectangle_list);
  void LogGlow();
  // Debugging aids: draws of this pixel shader (hash) are skipped; with
  // record_draws, each draw is described in a line (written with a dump).
  uint64_t skip_pixel_shader = 0;
  bool record_draws = false;
  // With log_glow: per frame, the glow history's cross-fade draws (pixel
  // shader kGlowCrossFade) and the texture they read, and 256x144 resolves
  // without a clear (the history written back); logged once a second by
  // whether a logic pass recorded the frame.
  bool log_glow = false;
  // With log_tick_repeats (docs/tick-replay.md): each draw's parts hashed,
  // and each render-only frame's draws compared with its tick's first
  // frame's, draw by draw.
  enum DrawPart {
    kPartTarget, kPartPipeline, kPartTextures, kPartVertexFetch, kPartIndices, kPartDynamic,
    kPartInline, kPartSystem, kPartVertexFloats, kPartPixelFloats, kPartBoolLoop, kDrawParts
  };
  static constexpr uint32_t kStructureParts = 6;  // kPartTarget to kPartDynamic
  struct DrawParts {
    uint64_t hash[kDrawParts];
    uint64_t pixel;  // the pixel shader's hash, for the samples
    uint32_t pass;   // the frame's resolves before it
  };
  bool log_tick_repeats = false;
  std::vector<DrawParts> frame_parts, tick_parts;
  bool tick_parts_valid = false;
  struct TickRepeatCounts {
    uint64_t logic_frames = 0, frames = 0, same_count = 0, structure_same = 0,
             all_but_floats_same = 0, all_same = 0, draws = 0, count_delta = 0;
    uint64_t differ[kDrawParts] = {};  // draws, aligned, in frames with the same count
    uint64_t structure_differ = 0;     // draws with any structural part differing
    std::vector<std::string> samples;
  } tick_repeats;
  struct RecordedState;
  // Records the draw (binding the targets), its state calls left out where
  // they repeat the last draw's (skip_repeated_state); returns what the
  // command buffer then holds.
  RecordedState RecordDraw(VkCommandBuffer commands, Target* color, Target* depth,
                           const DrawEmit& e, const uint32_t offsets[5]);
  void NoteDrawParts(const DrawCall& call, const Target* color, const Target* depth,
                     const RecordedState& now, const TranslatedShader* vertex,
                     const TranslatedShader* pixel, uint64_t index_start, uint32_t index_count,
                     uint32_t first_index, const void* system, size_t system_size,
                     const uint32_t packed_bytes[2]);
  void CompareTickRepeats();
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

  // Targets by surface (colour or depth, EDRAM base, pitch, samples, the
  // format they're drawn in), and the ones being drawn into now.
  std::map<std::array<uint32_t, 6>, Target> targets;
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
    // For the resolve list's log: the draws since the frame's previous
    // resolve (the pass's), and the source target's guest size and samples.
    uint32_t draws;
    VkExtent2D source;
    VkSampleCountFlagBits samples;
  };
  std::vector<Resolved> frame_resolves;
  uint64_t frames = 0;  // for logging the resolve list now and then
  uint32_t frame_draws = 0, draws_at_resolve = 0;  // draws this frame, and by its last resolve
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

  // --native_renderer_tick_draws (TickDrawKey): on a frame replaying a
  // logic pass's list (the tick's first), each draw's key and result into
  // tick_capture, kept as tick_list at the frame's end; on the frames after
  // it, a draw whose key equals tick_list's at its place reuses its result.
  // Texture sets made while capturing come from tick rings, kept until
  // three ticks later (no frame in flight can still use them by then).
  struct TickDraw {
    TickDrawKey key;
    uint32_t texture_mask = 0;  // the texture fetch constants its shaders read
    uint64_t textures_hash = 0;  // a hash of their words
    bool eligible = false;  // false: failed, or something not kept
    bool drawn = false;     // false: invisible (nothing to draw)
    Target* color = nullptr;
    Target* depth = nullptr;
    const TranslatedShader* vertex = nullptr;
    const TranslatedShader* pixel = nullptr;
    Translator::SystemConstants system;
    DrawEmit emit;
    bool main_view_draw = false;
  };
  bool tick_draws = false;          // the option
  bool tick_draws_active = false;   // this frame: on, with no diagnostic in the way
  bool tick_capturing = false;      // this frame is a tick's first
  std::vector<TickDraw> tick_capture, tick_list;
  uint32_t draw_calls = 0;  // Draw calls this frame, failed and invisible ones included
  static constexpr uint32_t kTickRings = 3;
  Ring tick_rings[kTickRings];
  uint32_t tick_ring = 0;
  struct TickDrawCounts {
    uint64_t captured_frames = 0, captured = 0, frames = 0, matched = 0, reused = 0, invisible = 0,
             no_entry = 0, ineligible = 0, key_differs = 0, textures_differ = 0;
    uint64_t key_parts[8] = {};  // which part of the key differed
    uint64_t checked = 0, check_mismatches = 0;
    std::vector<std::string> check_samples;
  } tick_counts;
  void MakeTickKey(uint8_t* base, uint32_t device, const DrawCall& call, TickDrawKey& key);
  static uint64_t TextureWordsHash(const uint32_t* fetch, uint32_t mask);
  // On a render-only frame: draws the stored result for this draw if its
  // key matches; false to work it out in full.
  const TickDraw* FindTickDraw(const TickDrawKey& key);
  void DrawFromTick(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                    const TickDraw& d);
  // With check_tick_draws: a draw that matched its stored one is worked
  // out in full as well, and the two compared.
  bool check_tick_draws = false;
  const TickDraw* tick_expected = nullptr;
  void CheckTickDraw(const TickDraw& d, bool drawn, Target* color, Target* depth,
                     const TranslatedShader* vertex, const TranslatedShader* pixel,
                     const void* system, const DrawEmit* e);
  // The vertex and pixel float constants the shaders read, packed, into
  // the ring (offsets[1], offsets[2]); packed_bytes gets each's size.
  bool PlaceFloatConstants(uint8_t* base, uint32_t device, const TranslatedShader* vertex,
                           const TranslatedShader* pixel, uint32_t offsets[5],
                           uint32_t packed_bytes[2]);

  VkDescriptorPool pool = VK_NULL_HANDLE;
  VkDescriptorSetLayout shared_memory_layout = VK_NULL_HANDLE;
  VkDescriptorSetLayout constants_layout = VK_NULL_HANDLE;

  // Textures, and white 1x1 placeholders of each declared type for what
  // can't be had yet.
  TextureCache textures;
  Image dummy_2d_array, dummy_3d, dummy_cube;
  // Zero (transparent black) ones for a texture of another dimension than
  // the shader declares: the plugin binds a null descriptor, which reads
  // as zero.
  Image zero_2d_array, zero_3d, zero_cube;
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
  rex::graphics::RegisterFile regs;
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
    uint32_t offsets[5];  // the constants' dynamic offsets
    bool indexed;  // index buffer below bound
    VkBuffer index_buffer;
    VkDeviceSize index_offset;
    VkIndexType index_type;
  } recorded;
  // The constants each block (system, vertex floats, pixel floats,
  // bool/loop, fetch) last had written into the constant block in use: a
  // copy of the bytes and where they went, so a draw whose block holds the
  // same bytes points at them instead of writing them again
  // (reuse_constants). Forgotten when the frame or the block changes.
  struct LastConstants {
    bool valid[5] = {};
    uint32_t offsets[5];
    uint32_t sizes[5];  // bytes written
    alignas(16) uint8_t bytes[5][kFloatConstantsRange];
  } last_constants;
  bool reuse_constants = true;
  bool log_constant_changes = false;
  bool log_register_reads = false;
  // ReadRegisters into regs, counted with log_register_reads.
  void ReadState(uint8_t* base, uint32_t device) {
    if (log_register_reads) {
      ReadRegisters(base, device, regs, stats.register_reads);
    } else {
      ReadRegisters(base, device, regs);
    }
  }
  // Points offset at a copy of the block's size bytes in the constant
  // block in use: the last one written if the same (and reuse_constants),
  // or a new one, reserving range bytes. With copied, the bytes are
  // already in last_constants.bytes[block] and differ from the last ones
  // written unless same.
  bool PlaceConstants(uint32_t block, const void* bytes, uint32_t size, uint32_t range,
                      uint32_t& offset, bool copied = false, bool same = false);

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
  // Draws the source's rectangle (left, top, right, bottom; the whole
  // destination if null) into the resolve's destination texture at the
  // point (x, y; 0, 0 if null).
  bool ResolveInto(VkCommandBuffer commands, uint8_t* base, Target& source, uint32_t destination,
                   const int32_t* rect, const int32_t* point, uint32_t flags);
  // A resolve as the library's Resolve (sub_822AFA58) takes it: flags as
  // the XDK's D3DRESOLVE, from the device's bound surfaces; then the clears
  // its flags ask for (0x100 colour, 0x200 depth and stencil), if
  // apply_clears.
  void Resolve(VkCommandBuffer commands, uint8_t* base, uint32_t device, uint32_t flags,
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
  void EndRendering(VkCommandBuffer commands);
  bool CreateGuestMemoryMirror();
  bool CreateGuestMemoryMirror(GuestMirror& mirror);
  // `newest`: Buffers_NewestWrite's answer for exactly this range if the
  // caller has just asked it (kNewestUnknown otherwise), to not ask twice.
  static constexpr uint64_t kNewestUnknown = ~uint64_t(0);
  void Mirror(uint8_t* base, uint32_t address, uint32_t size, bool index,
              uint64_t newest = kNewestUnknown);
  void CheckClobbers(uint8_t* base, uint32_t address, uint32_t size, bool index);
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
  VkDescriptorSet GetTextureSet(uint8_t* base, const TranslatedShader* shader,
                                VkDescriptorSetLayout layout, uint32_t stage);
  VkDescriptorSet BuildTextureSet(uint8_t* base, const TranslatedShader* shader,
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
  void Draw(VkCommandBuffer commands, uint8_t* base, uint32_t device, const DrawCall& call);
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
  info.size = kMirrorSize;
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
  if (vk::vkBindBufferMemory(device, mirror.buffer, mirror.memory, 0) != VK_SUCCESS ||
      vk::vkMapMemory(device, mirror.memory, 0, VK_WHOLE_SIZE, 0,
                      reinterpret_cast<void**>(&mirror.mapped)) != VK_SUCCESS) {
    return false;
  }
  // The built-in index buffers.
  uint32_t* rectangles = reinterpret_cast<uint32_t*>(mirror.mapped + kRectangleIndices);
  for (uint32_t i = 0; i < kBuiltinPrimitives; i++) {
    if (i) {
      *rectangles++ = UINT32_MAX;
    }
    for (uint32_t j = 0; j < 4; j++) {
      *rectangles++ = i << 2 | j;
    }
  }
  uint32_t* quads = reinterpret_cast<uint32_t*>(mirror.mapped + kQuadIndices);
  for (uint32_t i = 0; i < kBuiltinPrimitives; i++) {
    for (uint32_t j : {0u, 1u, 2u, 0u, 2u, 3u}) {
      *quads++ = 4 * i + j;
    }
  }
  return true;
}

void Passes::State::Mirror(uint8_t* base, uint32_t address, uint32_t size, bool index,
                           uint64_t newest) {
  if (!size || address >= kPhysicalMemorySize) {
    return;
  }
  if (size > kPhysicalMemorySize - address) {
    size = kPhysicalMemorySize - address;
    newest = kNewestUnknown;  // asked of a larger range
  }
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
  if (log_index_ranges) {
    index_range_counts.mirror_calls++;
  }
  if (!mirrored.insert(key).second) {
    if (log_index_ranges) {
      index_range_counts.mirror_repeats++;
    }
    return;  // already copied this frame
  }
  if (Buffers_Logging()) {
    Buffers_OnMirror(address, size, index);
  }
  if (Buffers_LogPhysicalWrites()) {
    Buffers_OnDrawnRange(address, size, index);
  }
  if (check_late_writes) {
    // Hashed again at the end of the frame.
    copied_ranges.push_back({address, size, XXH3_64bits(PhysicalHostAddress(base, address), size),
                             index, current_vertex_shader});
  }
  if (check_clobbers) {
    CheckClobbers(base, address, size, index);
  }
  if (check_mirror_frames) {
    uint64_t hash = XXH3_64bits(PhysicalHostAddress(base, address), size);
    auto [it, added] = range_history.try_emplace(key, RangeHistory{hash, 0, index});
    if (added) {
      mirror_new_bytes += size;
    } else if (it->second.hash != hash) {
      it->second.hash = hash;
      it->second.changes++;
      mirror_changed_bytes += size;
    }
    if (!index) {
      mirror_vertex_bytes += size;
    }
  }
  if (mirror_by_tick) {
    // The mirror has memcpy's writes as of the last logic pass's list (or,
    // with mirror_to_both, as of this frame's start). With verify_mirror,
    // the drawn ranges it misses a write of (memcpy being the only writer;
    // comparing with guest memory slows frames below the tick).
    if (!verify_mirror) {
      return;
    }
    stats.mirror_checked++;
    uint32_t end = address + size;
    auto it = std::lower_bound(
        front_missing.begin(), front_missing.end(),
        std::make_pair(address > front_missing_longest ? address - front_missing_longest : 0, 0u));
    for (; it != front_missing.end() && it->first < end; ++it) {
      if (it->second <= address) {
        continue;
      }
      bool render_only = !Renderer_ReplayingLogicList();
      by_tick.stale[render_only]++;
      by_tick.stale_bytes[render_only] += size;
      stats.mirror_stale++;
      stats.mirror_stale_bytes += size;
      if (by_tick.stale_lines) {
        by_tick.stale_lines--;
        REXLOG_INFO("native renderer: mirror by tick: frame {}, {} list: {} range {:08X} ({} "
                    "bytes) misses a write to {:08X}-{:08X}",
                    by_tick.frames, render_only ? "a render-only pass's" : "a logic pass's",
                    index ? "index" : "vertex", address, size, it->first, it->second);
      }
      break;
    }
    return;
  }
  // Only what memcpy has written since this mirror last got the range (see
  // "Buffers" in docs/native-renderer.md): vertex and index data is
  // written by the library's memcpy and nothing else.
  GuestMirror& mirror = mirrors[frame];
  if (!copy_all) {
    auto [it, added] = mirror.copied.try_emplace(key, 0);
    if (log_index_ranges && !added) {
      // No write taken since the copy (Buffers_NewestWrite takes the
      // pending ones): the answer is known without the lookup.
      index_range_counts.newest_asked++;
      Buffers_NewestWrite(address, size);
      if (Buffers_Sequence() == it->second) {
        index_range_counts.nothing_taken++;
      }
    }
    if (!added && (newest != kNewestUnknown ? newest : Buffers_NewestWrite(address, size)) <=
                      it->second) {
      stats.mirror_skipped_bytes += size;
      // Slow (the mirror is uncached memory: ~20 fps in a race), but
      // checks every range every frame.
      if (verify_mirror &&
          (stats.mirror_checked++,
           std::memcmp(mirror.mapped + address, PhysicalHostAddress(base, address), size))) {
        stats.mirror_stale++;
        stats.mirror_stale_bytes += size;
        if (!stats.mirror_stale_sample_size) {
          stats.mirror_stale_sample_address = address;
          stats.mirror_stale_sample_size = size;
          stats.mirror_stale_sample_index = index;
        }
      }
      return;
    }
    it->second = Buffers_Sequence();
  }
  std::memcpy(mirror.mapped + address, PhysicalHostAddress(base, address), size);
  stats.mirrored_bytes += size;
}

void Passes::State::MirrorByTickFrameStart(uint8_t* base) {
  by_tick.frames++;
  mirror_taken.clear();
  Buffers_TakeMirrorWrites(mirror_taken);
  for (auto& pending : mirror_pending) {
    pending.insert(pending.end(), mirror_taken.begin(), mirror_taken.end());
  }
  // The back mirror: no frame in flight reads it. It was last read by the
  // frame before it stopped being the front, which, with two frames in
  // flight, is done by now (this frame slot's fence has been waited on) as
  // long as the front changed in an earlier frame than this one.
  uint32_t back = front_mirror ^ 1;
  auto apply = [&](uint32_t m) {
    for (auto [start, end] : mirror_pending[m]) {
      end = std::min(end, kPhysicalMemorySize);
      if (start < end) {
        std::memcpy(mirrors[m].mapped + start, PhysicalHostAddress(base, start), end - start);
        by_tick.writes++;
        by_tick.bytes += end - start;
      }
    }
    mirror_pending[m].clear();
  };
  apply(back);
  // With mirror_to_both, the front too: frames in flight read it, but no
  // write has been seen landing in data a frame drew around it
  // (--native_renderer_log_physical_writes).
  if (mirror_to_both) {
    apply(front_mirror);
  }
  if (Renderer_ReplayingLogicList()) {
    front_mirror = back;
    by_tick.swaps++;
  }
  front_missing.clear();
  front_missing_longest = 0;
  for (auto [start, end] : verify_mirror ? mirror_pending[front_mirror]
                                          : std::vector<std::pair<uint32_t, uint32_t>>{}) {
    end = std::min(end, kPhysicalMemorySize);
    if (start < end) {
      front_missing.emplace_back(start, end);
      front_missing_longest = std::max(front_missing_longest, end - start);
    }
  }
  std::sort(front_missing.begin(), front_missing.end());
}

std::pair<uint32_t, uint32_t> Passes::State::DrawIndexRange(uint8_t* base, uint32_t address,
                                                            uint32_t count, bool index_32bit,
                                                            uint32_t endian, bool skip_restart) {
  if (!cache_index_ranges) {
    return IndexRange(PhysicalHostAddress(base, address), count, index_32bit, endian,
                      skip_restart);
  }
  uint64_t key = IndexRangeCache::Key(address, count, index_32bit, endian, skip_restart);
  IndexRangeCache::Entry& entry = index_cache.Find(key);
  std::pair<uint32_t, uint32_t> range;
  if (entry.key && entry.frame != mirror_frames) {
    draw_index_newest = Buffers_NewestWrite(address, count * (index_32bit ? 4 : 2));
  }
  if (entry.key && (entry.frame == mirror_frames || draw_index_newest <= entry.sequence)) {
    entry.frame = mirror_frames;
    range = {entry.low, entry.high};
    return range;
  }
  // Writes not yet taken get later sequences, so the entry is scanned again
  // once they are.
  uint64_t sequence = Buffers_Sequence();
  range = IndexRange(PhysicalHostAddress(base, address), count, index_32bit, endian,
                     skip_restart);
  if (!entry.key) {
    index_cache.Added();
  }
  entry = {key, sequence, mirror_frames, range.first, range.second};
  return range;
}

void Passes::State::CountIndexRange(uint32_t address, uint32_t count, bool index_32bit,
                                    uint32_t endian, bool skip_restart, uint32_t low,
                                    uint32_t high) {
  IndexRangeCounts& c = index_range_counts;
  c.scans++;
  c.indices += count;
  // A draw's count is far below 2^28, leaving room for the format.
  uint64_t key = (uint64_t(address) << 32) | (count & 0x0FFFFFFFu) |
                 (uint64_t(index_32bit) << 28) | (uint64_t(endian & 3) << 29) |
                 (uint64_t(skip_restart) << 31);
  uint64_t newest = Buffers_NewestWrite(address, count * (index_32bit ? 4 : 2));
  auto [it, added] = index_ranges_seen.try_emplace(
      key, IndexRangeSeen{Buffers_Sequence(), low, high, mirror_frames});
  if (added) {
    c.first++;
    c.distinct++;
    return;
  }
  IndexRangeSeen& seen = it->second;
  if (seen.frame == mirror_frames) {
    c.same_frame++;
  } else {
    c.distinct++;
  }
  if (newest > seen.sequence) {
    c.written++;
  } else {
    c.unchanged++;
    c.unchanged_indices += count;
    if (seen.low != low || seen.high != high) {
      if (!c.mismatched++) {
        uint32_t sample[] = {address, count, seen.low, seen.high, low, high};
        std::copy(std::begin(sample), std::end(sample), c.mismatch_sample);
      }
    }
  }
  seen = IndexRangeSeen{Buffers_Sequence(), low, high, mirror_frames};
}

void Passes::State::CheckClobbers(uint8_t* base, uint32_t address, uint32_t size, bool index) {
  // Earlier ranges overlapping this one: the mirror holds what their draws
  // copied (the last copy of each byte); bytes differing from guest memory
  // now are about to be replaced under them.
  clobber_copies++;
  uint32_t end = address + size;
  uint32_t from = address > clobber_longest ? address - clobber_longest : 0;
  const uint8_t* mirror = draw_mirror().mapped;
  const uint8_t* guest = PhysicalHostAddress(base, 0);
  for (auto it = clobber_ranges.lower_bound(from);
       it != clobber_ranges.end() && it->first < end; ++it) {
    uint32_t lo = std::max(address, it->first), hi = std::min(end, it->second.end);
    if (lo >= hi || !std::memcmp(mirror + lo, guest + lo, hi - lo)) {
      continue;
    }
    uint32_t differing = 0, first = 0;
    for (uint32_t a = lo; a < hi; a++) {
      if (mirror[a] != guest[a]) {
        first = differing++ ? first : a;
      }
    }
    clobbers++;
    clobbered_bytes += differing;
    clobbers_by_shaders[{it->second.vertex_shader, it->second.pixel_shader, current_vertex_shader,
                         current_pixel_shader}]++;
    if (clobber_sample.empty()) {
      clobber_sample = fmt::format(
          "{} {:08X}-{:08X} of vs {:016X} ps {:016X} by {} {:08X}-{:08X} of vs {:016X} ps "
          "{:016X}: {} bytes differ from {:08X}",
          it->second.index ? "index" : "vertex", it->first, it->second.end,
          it->second.vertex_shader, it->second.pixel_shader, index ? "index" : "vertex", address,
          end, current_vertex_shader, current_pixel_shader, differing, first);
    }
  }
  clobber_ranges.insert({address, {end, index, current_vertex_shader, current_pixel_shader}});
  clobber_longest = std::max(clobber_longest, size);
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
  VkDescriptorPoolSize size{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 5};
  VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_info.maxSets = 1;
  pool_info.poolSizeCount = 1;
  pool_info.pPoolSizes = &size;
  if (vk::vkCreateDescriptorPool(device, &pool_info, nullptr, &block.pool) != VK_SUCCESS) {
    return false;
  }
  VkDescriptorSetAllocateInfo set_alloc{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  set_alloc.descriptorPool = block.pool;
  set_alloc.descriptorSetCount = 1;
  set_alloc.pSetLayouts = &constants_layout;
  if (vk::vkAllocateDescriptorSets(device, &set_alloc, &block.set) != VK_SUCCESS) {
    return false;
  }
  const uint32_t ranges[5] = {uint32_t(sizeof(Translator::SystemConstants)),
                              kFloatConstantsRange, kFloatConstantsRange,
                              kBoolLoopConstantsRange, kFetchConstantsRange};
  VkDescriptorBufferInfo buffers[5];
  VkWriteDescriptorSet writes[5];
  for (uint32_t i = 0; i < 5; i++) {
    buffers[i] = {block.buffer, 0, ranges[i]};
    writes[i] = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    writes[i].dstSet = block.set;
    writes[i].dstBinding = i;
    writes[i].descriptorCount = 1;
    writes[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    writes[i].pBufferInfo = &buffers[i];
  }
  vk::vkUpdateDescriptorSets(device, 5, writes, 0, nullptr);
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

  for (Ring& ring : rings) {
    if (!CreateConstantBlock(ring)) {
      return false;
    }
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
  for (auto* set : {&dummy_2d_array, &zero_2d_array}) {
    Image& array = set[0];
    Image& volume = set == &dummy_2d_array ? dummy_3d : zero_3d;
    Image& cube = set == &dummy_2d_array ? dummy_cube : zero_cube;
    if (!CreateImage(array, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                     VK_FORMAT_R8G8B8A8_UNORM, {1, 1, 1}, 1, kUsage, VK_IMAGE_ASPECT_COLOR_BIT) ||
        !CreateImage(volume, VK_IMAGE_TYPE_3D, VK_IMAGE_VIEW_TYPE_3D, VK_FORMAT_R8G8B8A8_UNORM,
                     {1, 1, 1}, 1, kUsage, VK_IMAGE_ASPECT_COLOR_BIT) ||
        !CreateImage(cube, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_CUBE, VK_FORMAT_R8G8B8A8_UNORM,
                     {1, 1, 1}, 6, kUsage, VK_IMAGE_ASPECT_COLOR_BIT,
                     VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT)) {
      return false;
    }
  }
  // Clear them to white or zero once, and leave them readable.
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
  VkClearColorValue white{{1.0f, 1.0f, 1.0f, 1.0f}}, zero{};
  struct ToClear {
    VkImage image;
    uint32_t layers;
    const VkClearColorValue* value;
  };
  for (auto [image, layers, value] :
       {ToClear{dummy_2d_array.image, 1, &white}, ToClear{dummy_3d.image, 1, &white},
        ToClear{dummy_cube.image, 6, &white}, ToClear{zero_2d_array.image, 1, &zero},
        ToClear{zero_3d.image, 1, &zero}, ToClear{zero_cube.image, 6, &zero}}) {
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
    vk::vkCmdClearColorImage(commands, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, value, 1,
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

VkDescriptorSet Passes::State::GetTextureSet(uint8_t* base, const TranslatedShader* shader,
                                               VkDescriptorSetLayout layout, uint32_t stage) {
  LastTextureSet& last = last_texture_sets[stage];
  const uint32_t* fetch = &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0];
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
  VkDescriptorSet set = BuildTextureSet(base, shader, layout);
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

VkDescriptorSet Passes::State::BuildTextureSet(uint8_t* base, const TranslatedShader* shader,
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
        &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 6 * texture.fetch_constant];
    // The signed binding gets the same (unsigned) view: it's read only for
    // components whose sign is signed, which this game's textures don't
    // have (logged if they do).
    // The translator binds a signed and an unsigned view of each texture:
    // both get the same.
    size_t same = 0;
    while (same < i && (shader->textures[same].fetch_constant != texture.fetch_constant ||
                        shader->textures[same].view_type != texture.view_type)) {
      same++;
    }
    if (same < i) {
      // Field by field: copied whole, the 8-byte read of its layout and
      // padding waited on the 4-byte store of the layout just made.
      infos[i] = {VK_NULL_HANDLE, infos[same].imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
      continue;
    }
    // Counted as asked of the texture cache (not the copies above).
    if (log_texture_bindings) {
      uint32_t extra[4] = {uint32_t(texture.view_type), 0, 0, 0};
      CountBinding(true, texture.fetch_constant, fetch, extra);
    }
    VkImageView view = hide_cubes && texture.view_type == VK_IMAGE_VIEW_TYPE_CUBE
                           ? VK_NULL_HANDLE
                           : textures.GetView(base, fetch, texture.view_type,
                                                              texture.fetch_constant);
    bool mismatched = false;
    if (uint64_t total = textures.mismatched_total(); total != mismatched_seen) {
      mismatched = true;
      mismatched_seen = total;
    }
    // Which shaders sample a texture of another dimension than declared.
    if (mismatched && mismatch_lines.size() < 20 && InfoLogEnabled()) {
      std::string key = fmt::format(
          "{} shader {:016X} fetch constant {} declares view type {}; texture {:08X} format {} "
          "dimension {} {}x{}",
          shader->stage == ShaderStage::kVertex ? "vertex" : "pixel", shader->hash,
          texture.fetch_constant, int(texture.view_type), fetch[1] & 0xFFFFF000u, fetch[1] & 0x3F,
          (fetch[5] >> 9) & 3, (fetch[2] & 0x1FFF) + 1, ((fetch[2] >> 13) & 0x1FFF) + 1);
      if (mismatch_lines.insert(key).second) {
        REXLOG_INFO("native renderer: dimension mismatch: {} (frame {})", key, frames);
      }
    }
    if (!view && mismatched) {
      view = texture.view_type == VK_IMAGE_VIEW_TYPE_3D     ? zero_3d.view
             : texture.view_type == VK_IMAGE_VIEW_TYPE_CUBE ? zero_cube.view
                                                            : zero_2d_array.view;
    } else if (!view) {
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
    if (log_texture_bindings) {
      uint32_t extra[4] = {binding.mag_filter, binding.min_filter, binding.mip_filter,
                           binding.aniso_filter};
      CountBinding(false, binding.fetch_constant, fetch, extra);
    }
    VkSampler texture_sampler = textures.GetSampler(
        fetch, {binding.fetch_constant, binding.mag_filter, binding.min_filter, binding.mip_filter,
                binding.aniso_filter});
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
  Ring& ring = tick_capturing ? tick_rings[tick_ring] : rings[frame];
  if (ring.texture_pools.empty() && !CreateTexturePool(ring)) {
    return VK_NULL_HANDLE;
  }
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
      REXLOG_INFO("native renderer: frame {} texture sets: pool {} added ({} sets each)",
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
  uint64_t hash = XXH3_64bits(words, sizeof(words));
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

void Passes::State::CountShaderLookup(const ShaderKey keys[2], const uint64_t hashes[2],
                                      uint32_t program_control, uint32_t context_misc,
                                      bool point, bool rectangle_list) {
  ShaderLookupCounts& c = lookup_counts;
  LastLookup& last = last_lookup;
  c.draws++;
  c.copies += keys[0].copy;
  uint64_t key_hashes[2];
  for (int stage = 0; stage < 2; stage++) {
    key_hashes[stage] = XXH3_64bits(&keys[stage], sizeof(ShaderKey));
    c.same[stage] += last.valid && last.key[stage] == key_hashes[stage];
    auto [it, added] =
        shaders_seen[stage].try_emplace(key_hashes[stage], ShaderSeen{hashes[stage], 0});
    if (!added) {
      c.seen[stage]++;
      if (it->second.hash != hashes[stage]) {
        bool patched = it->second.patches != g_vertex_shader_patches;
        c.stale[stage]++;
        c.stale_patched[stage] += patched;
        if (!patched && !c.stale_sample[stage].object) {
          c.stale_sample[stage] = keys[stage];
          c.stale_hashes[stage][0] = it->second.hash;
          c.stale_hashes[stage][1] = hashes[stage];
        }
        it->second.hash = hashes[stage];
      }
    }
    it->second.patches = g_vertex_shader_patches;
  }
  // The whole lookup: both stages' keys and what the interface depends on.
  uint64_t lookup[4] = {key_hashes[0], key_hashes[1],
                        uint64_t(program_control) << 32 | context_misc,
                        uint64_t(point) | uint64_t(rectangle_list) << 1};
  uint64_t lookup_hash = XXH3_64bits(lookup, sizeof(lookup));
  c.lookup_same += last.valid && last.lookup == lookup_hash;
  c.lookup_seen += !lookups_seen.insert(lookup_hash).second;
  last = {{key_hashes[0], key_hashes[1]}, lookup_hash, true};
}

void Passes::State::CountDrawBindings(const TranslatedShader* vertex,
                                      const TranslatedShader* pixel) {
  TextureBindingCounts& c = binding_counts;
  const uint32_t* fetch = &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0];
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
  constexpr VkDeviceSize kDrawConstants = sizeof(Translator::SystemConstants) +
                                          2 * kFloatConstantsRange + kBoolLoopConstantsRange +
                                          kFetchConstantsRange;
  if (ring_offset + kDrawConstants + 5 * alignment <= kConstantBlockSize) {
    return true;
  }
  Ring& ring = rings[frame];
  if (ring.block + 1 == ring.blocks.size()) {
    if (!CreateConstantBlock(ring)) {
      return false;
    }
    REXLOG_INFO("native renderer: frame {} constants: block {} added ({} MB each)", frame,
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
  // Surfaces drawn while tiling get targets of their own, as large as the
  // tiles together (their EDRAM wouldn't hold them).
  std::array<uint32_t, 6> key{uint32_t(depth), surface.edram_base, surface.pitch, surface.msaa,
                              uint32_t(format), tiling ? 1u : 0u};
  auto [it, added] = targets.try_emplace(key);
  Target& t = it->second;
  if (!added) {
    return t.image.image ? &t : nullptr;
  }
  t.depth = depth;
  t.surface = surface;
  t.format = format;
  t.samples = SampleCount(surface.msaa);
  t.guest_extent = SurfaceExtent(surface, depth);
  if (tiling) {
    t.guest_extent = {std::max(t.guest_extent.width, tiling_extent.width),
                      std::max(t.guest_extent.height, tiling_extent.height)};
  }
  t.scale = IsMainViewSurface(surface) ? main_view_scale : scale;
  t.extent = ScaledBy(t.guest_extent, t.scale);
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
  REXLOG_INFO("native renderer: {} target: EDRAM base {}, pitch {}, MSAA {}, format {}; {}x{}{} "
              "({}x{} drawn)",
              depth ? "depth" : "colour", surface.edram_base, surface.pitch, surface.msaa,
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
  REXLOG_INFO("native renderer: --xbox_gamma={}: the Xbox 360's gamma table for that display is "
              "applied to the frame",
              REXCVAR_GET(xbox_gamma));
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
  if (vk::vkCreateGraphicsPipelines(context.device, pipeline_cache, 1, &info, nullptr,
                                    &it->second) != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: couldn't create a resolve pipeline for format {}",
                 int(format));
    it->second = VK_NULL_HANDLE;
  }
  return it->second;
}

bool Passes::State::ResolveInto(VkCommandBuffer commands, uint8_t* base, Target& source,
                                uint32_t destination, const int32_t* rect,
                                const int32_t* point, uint32_t flags) {
  // The destination texture's fetch constant (+28..+48), its addresses
  // made physical as SetTexture makes them.
  uint32_t fetch[6];
  for (uint32_t k = 0; k < 6; k++) {
    fetch[k] = guest::LoadU32(base, destination + 28 + 4 * k);
  }
  fetch[1] = (fetch[1] & 0xFFFu) | GpuPhysicalAddress(fetch[1] & 0xFFFFF000u);
  fetch[5] = (fetch[5] & 0xFFFu) | GpuPhysicalAddress(fetch[5] & 0xFFFFF000u);
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
    REXLOG_INFO("native renderer: resolve from {} target (EDRAM base {}, pitch {}, MSAA {}, "
                "format {}) rect {},{}-{},{} to {:08X} at {},{}, flags {:08X}",
                source.depth ? "depth" : "colour", source.surface.edram_base,
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
  // The exponent bias: flags bits 26-31, signed (likely; 0xF4000000 is -3
  // on the main view's HDR copies).
  // Red and blue swapped as the library's Resolve sets RB_COPY_DEST_INFO's
  // copy_dest_swap: when the destination's swizzle takes X from another
  // component than the first (bits 1-3 of fetch word 3), so sampling it
  // through the swizzle swaps them back.
  bool swap = !source.depth && ((fetch[3] >> 1) & 7) != 0;
  struct {
    int32_t offset[2];
    int32_t samples;
    float scale;
    int32_t swap_red_blue;
  } constants{{host_left - host_x, host_top - host_y},
              source.depth ? 1 : int32_t(source.samples),
              std::ldexp(1.0f, int32_t(flags) >> 26), swap ? 1 : 0};
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
  resolved.draws = frame_draws - draws_at_resolve;
  draws_at_resolve = frame_draws;
  resolved.source = source.guest_extent;
  resolved.samples = source.samples;
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

Passes::Passes() : state_(std::make_unique<State>()) {
  std::memset(state_->regs.values, 0, sizeof(state_->regs.values));  // see ReadRegisters
}
Passes::~Passes() = default;

bool Passes::Init(const DeviceContext& context, uint8_t* base, uint32_t frames_in_flight,
                  float resolution_scale) {
  State& s = *state_;
  s.context = context;
  s.scale = resolution_scale > 0.0f ? resolution_scale : 1.0f;
  s.main_view_scale =
      s.scale == 1.0f ? 1.0f : s.scale * float(kShownWidth) / float(kMainViewWidth);
  s.shown_extent = s.Scaled(VkExtent2D{kShownWidth, kShownHeight});
  s.main_view_extent = State::ScaledBy(VkExtent2D{kMainViewWidth, kMainViewHeight},
                                       s.main_view_scale);
  REXLOG_INFO("native renderer: resolution scale {}: frame {}x{}, 3D view {}x{} (scale {})",
              s.scale, s.shown_extent.width, s.shown_extent.height, s.main_view_extent.width,
              s.main_view_extent.height, s.main_view_scale);
  s.guest_base = base;
  s.frames_in_flight = frames_in_flight;
  const std::string& log_level = REXCVAR_GET(log_level);
  s.collect_interfaces = log_level == "trace" || log_level == "debug" || log_level == "info";
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &s.memory_properties);
  if (!s.CreateGuestMemoryMirror()) {
    return false;
  }
  const std::string& gamma = REXCVAR_GET(xbox_gamma);
  if (gamma == "hdtv") {
    s.gamma_values = kXboxGammaHdtv;
  } else if (gamma == "power") {
    s.gamma_values = kXboxGammaPower;
  } else if (gamma != "off") {
    REXLOG_WARN("native renderer: --xbox_gamma={}: not off, hdtv or power; off", gamma);
  }
  if (!s.CreateImage(s.shown, VK_IMAGE_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D, kShownFormat,
                     {s.shown_extent.width, s.shown_extent.height, 1}, 1,
                     VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                         (s.gamma_values ? VK_IMAGE_USAGE_STORAGE_BIT : 0),
                     VK_IMAGE_ASPECT_COLOR_BIT) ||
      (s.gamma_values && !s.CreateGammaResources()) || !s.CreateRings() || !s.CreateDescriptors() || !s.CreateDummyTextures() ||
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
  if (std::filesystem::path directory = DiskCacheDirectory(); !directory.empty()) {
    s.shaders->OpenDiskCache(directory / "shaders.bin");
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
    REXLOG_INFO("native renderer: pipeline disk cache {}: {}", pipeline_cache_file.string(),
                matches        ? fmt::format("{} KB loaded", data.size() / 1024)
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
    REXLOG_WARN("native renderer: pipeline disk cache: couldn't write {}",
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
  s.frame_draws = s.draws_at_resolve = 0;
  s.draw_calls = 0;
  // Draws translated once a tick: off with the diagnostics that need every
  // draw worked out (or that the reused draws don't feed).
  s.tick_draws_active = s.tick_draws && !s.MirrorPerDraw() && !s.record_draws &&
                        !s.watch_draws && !s.skip_pixel_shader && !s.log_glow &&
                        !s.check_mirror_frames && !s.log_tick_repeats &&
                        !s.log_constant_changes && !s.log_texture_bindings &&
                        !s.log_shader_lookups;
  s.tick_capturing = s.tick_draws_active && Renderer_ReplayingLogicList();
  if (s.tick_capturing) {
    s.tick_capture.clear();
    s.tick_ring = (s.tick_ring + 1) % State::kTickRings;
    State::Ring& tick = s.tick_rings[s.tick_ring];
    for (size_t i = 0; i <= tick.texture_pool && i < tick.texture_pools.size(); i++) {
      vk::vkResetDescriptorPool(s.context.device, tick.texture_pools[i], 0);
    }
    tick.texture_pool = 0;
    s.tick_counts.captured_frames++;
  } else if (s.tick_draws_active) {
    s.tick_counts.frames++;
  } else {
    s.tick_list.clear();
  }
  s.mirrored.clear();
  s.used_mirrored.clear();
  s.mirror_frames++;
  if (s.mirror_by_tick) {
    Buffers_KeepSegments(s.MirrorPerDraw());
    s.MirrorByTickFrameStart(s.guest_base);
  }
  if (Buffers_LogPhysicalWrites()) {
    Buffers_OnFrameStart([&s](uint32_t start, uint32_t end) { return s.textures.Holds(start, end); });
  }
  if (s.log_texture_bindings) {
    s.binding_counts.frames++;
  }
  if (s.log_shader_lookups) {
    s.lookup_counts.frames++;
  }
  if (s.log_index_ranges) {
    // Writes taken since the last frame's start (memcpy's, into the
    // physical views).
    Buffers_NewestWrite(0, 0);
    State::IndexRangeCounts& c = s.index_range_counts;
    c.frames++;
    c.writes += Buffers_Sequence() - c.frame_sequence;
    c.frame_sequence = Buffers_Sequence();
  }
  s.inline_offset = 0;
  s.mirrored_hashes.clear();
  s.clobber_ranges.clear();
  s.clobber_longest = 0;
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

bool Passes::State::PlaceFloatConstants(uint8_t* base, uint32_t device,
                                        const TranslatedShader* vertex,
                                        const TranslatedShader* pixel, uint32_t offsets[5],
                                        uint32_t packed_bytes[2]) {
  for (uint32_t stage = 0; stage < 2; stage++) {
    const TranslatedShader* shader = stage == 0 ? vertex : pixel;
    uint32_t source = device + (stage == 0 ? kDeviceVertexFloatConstants
                                           : kDevicePixelFloatConstants);
    // Only the registers the shader reads, packed in order, into the last
    // copy, noting whether any differ: run by run of consecutive registers
    // (testing each of the 256 bits in turn was ~14% of the render thread,
    // mostly mispredicted branches).
    uint32_t* copy = reinterpret_cast<uint32_t*>(last_constants.bytes[1 + stage]);
    bool same = last_constants.valid[1 + stage];
    uint32_t packed = 0;
    for (uint32_t w = 0; shader && w < 4; w++) {
      uint64_t bits = shader->float_constants[w];
      while (bits) {
        uint32_t first = std::countr_zero(bits);
        uint32_t count = std::countr_one(bits >> first);
        bits = count == 64 ? 0 : bits & ~(((uint64_t(1) << count) - 1) << first);
        if (guest::CopyU32s(base, source + 16 * (64 * w + first), copy + 4 * packed,
                            4 * count)) {
          same = false;
        }
        packed += count;
      }
    }
    packed_bytes[stage] = 16 * packed;
    if (!PlaceConstants(1 + stage, copy, 16 * packed, kFloatConstantsRange, offsets[1 + stage],
                        true, same)) {
      return false;
    }
  }
  return true;
}

Passes::State::RecordedState Passes::State::RecordDraw(VkCommandBuffer commands, Target* color,
                                                     Target* depth, const DrawEmit& e,
                                                     const uint32_t offsets[5]) {
  State& s = *this;
  s.BeginRendering(commands, color, depth);
  if (color && color->surface.pitch == kMainViewPitch && color->samples == VK_SAMPLE_COUNT_2_BIT &&
      color->format == VK_FORMAT_R16G16B16A16_SFLOAT) {
    s.main_view = color;
  }
  s.stats.depth_biased += e.depth_biased;
  s.stats.depth_equal += e.depth_equal;
  VkDescriptorSet sets[4] = {s.draw_mirror().set, s.constant_block().set, e.textures[0],
                             e.textures[1]};
  State::RecordedState now{};
  now.valid = true;
  now.viewport = e.viewport;
  now.scissor = e.scissor;
  now.bias[0] = e.bias[0];
  now.bias[1] = e.bias[1];
  std::memcpy(now.blend_constants, e.blend_constants, sizeof(e.blend_constants));
  now.pipeline = e.pipeline;
  now.layout = e.layout;
  std::memcpy(now.sets, sets, sizeof(sets));
  std::memcpy(now.offsets, offsets, sizeof(now.offsets));
  now.indexed = e.indexed;
  now.index_buffer = s.draw_mirror().buffer;
  now.index_type = e.index_type;
  now.index_offset = e.index_offset;

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
      uint32_t differ = 0;
      for (int i = 0; i < 5; i++) {
        if (last.offsets[i] != now.offsets[i]) {
          s.stats.offset_differs[i]++;
          differ |= 1u << i;
        }
      }
      s.stats.only_fetch_offset_differs += differ == 1u << 4;
    }
  }
  bool skip = s.skip_repeated_state;
  if (!(skip && repeated[0])) {
    vk::vkCmdSetViewport(commands, 0, 1, &now.viewport);
  }
  if (!(skip && repeated[1])) {
    vk::vkCmdSetScissor(commands, 0, 1, &now.scissor);
  }
  if (!(skip && repeated[2])) {
    vk::vkCmdSetDepthBias(commands, now.bias[0], 0.0f, now.bias[1]);
  }
  if (!(skip && repeated[3])) {
    vk::vkCmdSetBlendConstants(commands, now.blend_constants);
  }
  if (!(skip && repeated[4])) {
    vk::vkCmdBindPipeline(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, now.pipeline);
  }
  if (!(skip && repeated[8])) {
    vk::vkCmdBindDescriptorSets(commands, VK_PIPELINE_BIND_POINT_GRAPHICS, now.layout, 0, 4,
                                now.sets, 5, now.offsets);
  }
  if (now.indexed && !(skip && repeated[6])) {
    vk::vkCmdBindIndexBuffer(commands, now.index_buffer, now.index_offset, now.index_type);
  }
  if (e.indexed) {
    vk::vkCmdDrawIndexed(commands, e.count, 1, e.first_index, 0, 0);
  } else {
    vk::vkCmdDraw(commands, e.count, 1, 0, 0);
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
  s.frame_draws++;
  if (e.inline_vertices) {
    s.stats.inline_draws++;
    s.stats.inline_by_type[e.type & 15]++;
  }
  return now;
}

uint64_t Passes::State::TextureWordsHash(const uint32_t* fetch, uint32_t mask) {
  uint32_t words[32 * 6];
  uint32_t n = 0;
  for (; mask; mask &= mask - 1) {
    std::memcpy(words + n, fetch + 6 * std::countr_zero(mask), 24);
    n += 6;
  }
  return XXH3_64bits(words, 4 * n);
}

void Passes::State::MakeTickKey(uint8_t* base, uint32_t device, const DrawCall& call,
                                TickDrawKey& key) {
  key.call[0] = call.type;
  key.call[1] = call.base_start;
  key.call[2] = call.primitive_count;
  key.call[3] = call.inline_vertices;
  key.call[4] = call.vertex_count;
  uint32_t color_object = guest::LoadU32(base, device + kDeviceRenderTarget0);
  uint32_t depth_object = guest::LoadU32(base, device + kDeviceDepthStencil);
  key.surfaces[0] = color_object;
  key.surfaces[1] = color_object ? guest::LoadU32(base, color_object + 24) : 0;
  key.surfaces[2] = color_object ? guest::LoadU32(base, color_object + 28) : 0;
  key.surfaces[3] = depth_object;
  key.surfaces[4] = depth_object ? guest::LoadU32(base, depth_object + 24) : 0;
  key.surfaces[5] = depth_object ? guest::LoadU32(base, depth_object + 28) : 0;
  uint32_t vertex_object = guest::LoadU32(base, device + kDeviceVertexShader);
  uint32_t pixel_object = guest::LoadU32(base, device + kDevicePixelShader);
  uint32_t declaration = guest::LoadU32(base, device + kDeviceVertexDeclaration);
  uint32_t declaration_id = declaration ? guest::LoadU32(base, declaration + 48) : 0;
  key.shaders[0] = vertex_object;
  key.shaders[1] = vertex_object ? guest::LoadU32(base, vertex_object + 48) : 0;
  key.shaders[2] = vertex_object ? guest::LoadU32(base, vertex_object + 600) : 0;
  key.shaders[3] = vertex_object ? guest::LoadU32(base, vertex_object + 24) : 0;
  key.shaders[4] = declaration_id;
  key.shaders[5] = g_loaded_vertex_shader.object == vertex_object &&
                   g_loaded_vertex_shader.declaration_id == declaration_id;
  key.shaders[6] = pixel_object;
  key.shaders[7] = pixel_object ? guest::LoadU32(base, pixel_object + 24) : 0;
  key.shaders[8] = pixel_object ? guest::LoadU32(base, pixel_object + 64) : 0;
  key.shaders[9] = tiling;
  uint32_t index_buffer = call.inline_vertices ? 0 : guest::LoadU32(base, device + kDeviceIndexBuffer);
  key.indices[0] = index_buffer;
  key.indices[1] = index_buffer ? guest::LoadU32(base, index_buffer) : 0;
  key.indices[2] = index_buffer ? guest::LoadU32(base, index_buffer + 24) : 0;
  key.scissor[0] = guest::LoadU32(base, device + kDeviceScissorEnable);
  for (uint32_t i = 0; i < 4; i++) {
    key.scissor[1 + i] = guest::LoadU32(base, device + kDeviceScissorRect + 4 * i);
  }
  // The register blocks ReadRegisters fills (all state but constants).
  static constexpr uint32_t kBlocks[][2] = {{0x2000, 16}, {0x2100, 21}, {0x2180, 5},
                                             {0x2200, 12}, {0x2280, 21}, {0x2300, 38},
                                             {0x2380, 4}};
  uint32_t words[117];
  uint32_t n = 0;
  for (const auto& [first, count] : kBlocks) {
    std::memcpy(words + n, &regs.values[first], 4 * count);
    n += count;
  }
  key.registers = XXH3_64bits(words, sizeof(words));
  key.generations[0] = textures.generation();
  key.generations[1] = textures.sampler_generation();
}

const Passes::State::TickDraw* Passes::State::FindTickDraw(const TickDrawKey& key) {
  TickDrawCounts& c = tick_counts;
  uint32_t index = draw_calls - 1;
  if (index >= tick_list.size()) {
    c.no_entry++;
    return nullptr;
  }
  const TickDraw& d = tick_list[index];
  if (!d.eligible) {
    c.ineligible++;
    return nullptr;
  }
  if (!(d.key == key)) {
    c.key_differs++;
    const TickDrawKey& k = d.key;
    auto differs = [](const auto& a, const auto& b) { return !!std::memcmp(&a, &b, sizeof(a)); };
    c.key_parts[0] += differs(k.call, key.call);
    c.key_parts[1] += differs(k.surfaces, key.surfaces);
    c.key_parts[2] += differs(k.shaders, key.shaders);
    c.key_parts[3] += differs(k.indices, key.indices);
    c.key_parts[4] += differs(k.scissor, key.scissor);
    c.key_parts[5] += k.registers != key.registers;
    c.key_parts[6] += differs(k.generations, key.generations);
    return nullptr;
  }
  if (TextureWordsHash(&regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0], d.texture_mask) !=
      d.textures_hash) {
    c.textures_differ++;
    return nullptr;
  }
  c.matched++;
  return &d;
}

void Passes::State::DrawFromTick(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                                 const TickDraw& d) {
  TickDrawCounts& c = tick_counts;
  const uint32_t* fetch = &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0];
  if (!d.drawn) {
    c.invisible++;
    return;  // invisible on the tick's frame too
  }
  uint32_t offsets[5];
  uint32_t packed_bytes[2];
  if (!ReserveDrawConstants() ||
      !PlaceConstants(0, &d.system, sizeof(d.system), sizeof(d.system), offsets[0]) ||
      !PlaceFloatConstants(base, device, d.vertex, d.pixel, offsets, packed_bytes) ||
      !PlaceConstants(3, &regs.values[XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031],
                      kBoolLoopConstantsRange, kBoolLoopConstantsRange, offsets[3]) ||
      !PlaceConstants(4, fetch, kFetchConstantsRange, kFetchConstantsRange, offsets[4])) {
    stats.failed++;
    stats.failed_by[4]++;
    return;
  }
  stats.constants_draws++;
  current_vertex_shader = d.vertex->hash;
  current_pixel_shader = d.pixel ? d.pixel->hash : 0;
  last_base = base;
  if (d.main_view_draw && d.pixel &&
      std::find(frame_pixel_shaders.begin(), frame_pixel_shaders.end(), d.pixel->hash) ==
          frame_pixel_shaders.end()) {
    frame_pixel_shaders.push_back(d.pixel->hash);
  }
  RecordDraw(commands, d.color, d.depth, d.emit, offsets);
  c.reused++;
}

void Passes::State::CheckTickDraw(const TickDraw& d, bool drawn, Target* color, Target* depth,
                                  const TranslatedShader* vertex,
                                  const TranslatedShader* pixel, const void* system,
                                  const DrawEmit* e) {
  TickDrawCounts& c = tick_counts;
  c.checked++;
  std::string differs;
  auto note = [&](bool same, const char* what) {
    if (!same) {
      differs += fmt::format("{}{}", differs.empty() ? "" : ", ", what);
    }
  };
  note(d.drawn == drawn, "drawn");
  if (d.drawn && drawn) {
    auto same = [](const auto& a, const auto& b) { return !std::memcmp(&a, &b, sizeof(a)); };
    note(d.color == color && d.depth == depth, "targets");
    note(d.vertex == vertex && d.pixel == pixel, "shaders");
    note(!std::memcmp(&d.system, system, sizeof(d.system)), "system constants");
    note(d.emit.pipeline == e->pipeline && d.emit.layout == e->layout, "pipeline");
    note(same(d.emit.viewport, e->viewport), "viewport");
    note(same(d.emit.scissor, e->scissor), "scissor");
    note(same(d.emit.bias, e->bias), "depth bias");
    note(same(d.emit.blend_constants, e->blend_constants), "blend constants");
    note(d.emit.indexed == e->indexed && d.emit.index_type == e->index_type &&
             d.emit.index_offset == e->index_offset && d.emit.first_index == e->first_index &&
             d.emit.count == e->count,
         "draw call");
  }
  if (!differs.empty()) {
    c.check_mismatches++;
    if (c.check_samples.size() < 4) {
      c.check_samples.push_back(fmt::format("draw {}: {}", draw_calls - 1, differs));
    }
  }
}

void Passes::State::Draw(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                         const DrawCall& call) {
  State& s = *this;
  auto& regs = s.regs;
  // Draws translated once a tick (TickDrawKey).
  TickDraw* capture = nullptr;
  if (s.tick_draws_active) {
    s.draw_calls++;
    TickDrawKey tick_key;
    s.MakeTickKey(base, device, call, tick_key);
    if (s.tick_capturing) {
      capture = &s.tick_capture.emplace_back();
      capture->key = tick_key;
    } else if (const TickDraw* d = s.FindTickDraw(tick_key)) {
      if (!s.check_tick_draws) {
        s.DrawFromTick(commands, base, device, *d);
        return;
      }
      s.tick_expected = d;
    }
  }
  // (Cleared when this draw is done, whichever way it ends.)
  struct ExpectedGuard {
    const TickDraw*& expected;
    ~ExpectedGuard() { expected = nullptr; }
  } expected_guard{s.tick_expected};
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
  // The base vertex the GPU adds to each index: VGT_INDX_OFFSET, as the
  // plugin takes it (the item's upper half is the library's argument, which
  // it may already have applied to the vertex buffers' addresses).
  int32_t item_base_vertex = int32_t(base_start) >> 16;
  int32_t base_vertex = int32_t(regs.values[XE_GPU_REG_VGT_INDX_OFFSET]);
  if (!call.inline_vertices && base_vertex != item_base_vertex) {
    s.stats.base_vertex_differs++;
    if (!s.stats.base_vertex_sample[0] && !s.stats.base_vertex_sample[1]) {
      s.stats.base_vertex_sample = {uint32_t(item_base_vertex), uint32_t(base_vertex)};
    }
  }
  uint32_t start_index = base_start & 0xFFFF;
  // Inline draws: the vertices in order (indices only to expand rectangle
  // and quad lists).
  uint32_t index_count = call.inline_vertices ? call.vertex_count
                                              : primitive_count * primitive.a + primitive.b;

  // Shaders, read through physical memory as the GPU reads them.
  uint32_t program_control = regs.values[XE_GPU_REG_SQ_PROGRAM_CNTL];
  uint32_t vertex_object = guest::LoadU32(base, device + kDeviceVertexShader);
  uint32_t pixel_object = guest::LoadU32(base, device + kDevicePixelShader);
  if (!vertex_object) {
    s.stats.failed++;
    s.stats.failed_by[1]++;
    return;
  }
  uint32_t vertex_address = guest::LoadU32(base, vertex_object + 48);
  const uint8_t* vertex_ucode = PhysicalHostAddress(base, GpuPhysicalAddress(vertex_address));
  uint32_t vertex_size = guest::LoadU32(base, vertex_object + 600);
  uint32_t declaration = guest::LoadU32(base, device + kDeviceVertexDeclaration);
  uint32_t declaration_id = declaration ? guest::LoadU32(base, declaration + 48) : 0;
  uint32_t patched_for = guest::LoadU32(base, vertex_object + 24);
  bool patched = g_vertex_shader_patches != s.vertex_shader_patches;
  s.vertex_shader_patches = g_vertex_shader_patches;
  // The object's microcode is patched for another declaration: the GPU runs
  // the copy the library patched in the command buffer instead.
  const LoadedVertexShader& loaded = g_loaded_vertex_shader;
  bool vertex_copy = false;
  if (declaration_id != patched_for && loaded.object == vertex_object &&
      loaded.declaration_id == declaration_id && loaded.microcode.size() == vertex_size) {
    vertex_ucode = loaded.microcode.data();
    patched_for = declaration_id;
    vertex_copy = true;
    s.stats.vertex_shader_copies++;
  }
  if (declaration_id != patched_for) {
    s.stats.declaration_mismatches++;
    s.stats.declaration_mismatches_patched += patched;
    if (s.declaration_lines.size() < 6) {
      s.declaration_lines.push_back(fmt::format(
          "vs object {:08X} patched for {} ({:016X}), bound declaration {:08X} id {}; this "
          "draw patched {}; last patch: object {:08X} declaration {:08X} id {}",
          vertex_object, patched_for, XXH3_64bits(vertex_ucode, vertex_size), declaration,
          declaration_id, patched, g_last_patch[0], g_last_patch[1], g_last_patch[2]));
    }
    if (!s.stats.declaration_sample[0] && !s.stats.declaration_sample[1]) {
      s.stats.declaration_sample = {patched_for, declaration_id};
    }
  }
  uint32_t pixel_address = pixel_object ? guest::LoadU32(base, pixel_object + 24) : 0;
  const uint8_t* pixel_ucode =
      pixel_object ? PhysicalHostAddress(base, GpuPhysicalAddress(pixel_address)) : nullptr;
  uint32_t pixel_size = pixel_object ? guest::LoadU32(base, pixel_object + 64) : 0;
  uint32_t context_misc = regs.values[XE_GPU_REG_SQ_CONTEXT_MISC];
  // The lookup in full: each microcode hashed once (the cache's lookups take
  // the hash), which interpolators connect them (an unconnected one the
  // pixel shader reads would be undefined), and the translations.
  auto look_up = [&] {
    ShaderLookup r;
    r.vertex_hash = MicrocodeHash(vertex_ucode, vertex_size);
    r.pixel_hash = pixel_ucode ? MicrocodeHash(pixel_ucode, pixel_size) : 0;
    r.interface = s.shaders->GetInterface(vertex_ucode, vertex_size, r.vertex_hash, pixel_ucode,
                                          pixel_size, r.pixel_hash, program_control,
                                          context_misc);
    r.interface.param_gen_point = type == 1;  // point list
    r.interface.rectangle_list = rectangle_list;
    r.vertex = s.shaders->Get(ShaderStage::kVertex, vertex_ucode, vertex_size, r.vertex_hash,
                              program_control & 0x3F, r.interface);
    if (pixel_object) {
      r.pixel = s.shaders->Get(ShaderStage::kPixel, pixel_ucode, pixel_size, r.pixel_hash,
                               (program_control >> 8) & 0x3F, r.interface);
    }
    return r;
  };
  // Kept by what the draw read from the shader objects (ShaderLookupTable),
  // except for the patched copy (its contents change at one address) and
  // with the diagnostic that needs the hashes worked out.
  ShaderLookup lookup;
  if (s.cache_shader_lookups && !vertex_copy && !s.log_shader_lookups) {
    ShaderLookupTable::Key key = {{vertex_object, vertex_address, vertex_size, patched_for,
                                   declaration_id, pixel_object, pixel_address, pixel_size,
                                   program_control, context_misc,
                                   uint32_t(type == 1) | uint32_t(rectangle_list) << 1, 0}};
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
  if (s.log_shader_lookups) {
    State::ShaderKey keys[2] = {
        {vertex_object, vertex_address, vertex_size, patched_for, declaration_id, vertex_copy},
        {pixel_object, pixel_address, pixel_size, 0, 0, 0}};
    uint64_t hashes[2] = {lookup.vertex_hash, pixel_hash};
    s.CountShaderLookup(keys, hashes, program_control, context_misc, type == 1, rectangle_list);
  }
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
        r.vertex_fetches.push_back({k, regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k],
                                    regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k + 1]});
      }
    }
    r.render_state = {regs.values[0x2200], regs.values[0x2201], regs.values[0x2202],
                      regs.values[0x2208], regs.values[0x2104], regs.values[0x2105],
                      regs.values[0x2106], regs.values[0x2107], regs.values[0x2108],
                      regs.values[0x210E]};
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

  if (log_glow && pixel && pixel->hash == kGlowCrossFade) {
    glow_frame.cross_fades++;
    glow_frame.constants.clear();
    if (call.inline_vertices) {
      // Its weight: the vertices' colour alpha (the shader outputs the
      // history times the colour, and the colour's alpha).
      uint8_t* vertices = draw_mirror().mapped;
      glow_frame.constants += fmt::format(
          "vertex colour {:.4g} {:.4g} {:.4g} {:.4g}; ",
          guest::LoadFloat(vertices, call.vertex_address + 16),
          guest::LoadFloat(vertices, call.vertex_address + 20),
          guest::LoadFloat(vertices, call.vertex_address + 24),
          guest::LoadFloat(vertices, call.vertex_address + 28));
    }
    for (uint32_t c = 0; c < 256; c++) {
      if (pixel->float_constants[c >> 6] & (uint64_t(1) << (c & 63))) {
        uint32_t at = device + kDevicePixelFloatConstants + 16 * c;
        glow_frame.constants += fmt::format(
            "c{} {:.4g} {:.4g} {:.4g} {:.4g}; ", c, guest::LoadFloat(base, at),
            guest::LoadFloat(base, at + 4), guest::LoadFloat(base, at + 8),
            guest::LoadFloat(base, at + 12));
      }
    }
    for (const ShaderTexture& texture : pixel->textures) {
      glow_frame.cross_fade_texture =
          regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 6 * texture.fetch_constant + 1] &
          0x1FFFF000u;
      break;
    }
  }

  current_vertex_shader = vertex->hash;
  current_pixel_shader = pixel ? pixel->hash : 0;
  last_base = base;

  // Index buffer: +0 bit 31 = 32-bit, bits 29-30 = endianness; +24 address.
  bool index_32bit = false;
  uint32_t index_endian = 0, index_address = 0;
  if (!call.inline_vertices) {
    uint32_t index_buffer = guest::LoadU32(base, device + kDeviceIndexBuffer);
    if (!index_buffer) {
      s.stats.failed++;
      s.stats.failed_by[3]++;
      return;
    }
    uint32_t index_word = guest::LoadU32(base, index_buffer);
    index_32bit = index_word >> 31;
    index_endian = (index_word >> 29) & 3;
    index_address = GpuPhysicalAddress(guest::LoadU32(base, index_buffer + 24)) +
                    start_index * (index_32bit ? 4 : 2);
  }

  // Viewport and scissor.
  reg::RB_DEPTHCONTROL depth_control = draw_util::GetNormalizedDepthControl(regs);
  draw_util::ViewportInfo viewport_info;
  draw_util::GetHostViewportInfo(regs, 1, 1, false, 8192, 8192, true, depth_control, false,
                                 false, false, viewport_info);
  // Scissor: the window scissor registers aren't in the shadow's flushed
  // blocks, so take it as the library keeps it: enabled by render state 200
  // (device +11600), rectangle (left, top, right, bottom) at +12416 from
  // SetScissorRect; otherwise the whole target.
  draw_util::Scissor scissor{{0, 0}, {guest_extent.width, guest_extent.height}};
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
  uint32_t scissor_right = std::min(scissor.offset[0] + scissor.extent[0], guest_extent.width);
  uint32_t scissor_bottom = std::min(scissor.offset[1] + scissor.extent[1], guest_extent.height);
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
    if (capture) {
      capture->eligible = !vertex_copy;
      capture->drawn = false;
    }
    if (s.tick_expected) {
      s.CheckTickDraw(*s.tick_expected, false, color, depth, vertex, pixel, nullptr, nullptr);
    }
    return;  // nothing visible
  }

  uint32_t max_vertex = 0, min_vertex = UINT32_MAX;
  s.draw_index_newest = State::kNewestUnknown;
  if (call.inline_vertices) {
    min_vertex = 0;
    max_vertex = call.vertex_count - 1;
  }
  bool mirror_per_draw = s.MirrorPerDraw();
  if (!call.inline_vertices && index_count && mirror_per_draw) {
    bool skip_restart = primitive.strip && regs.Get<reg::PA_SU_SC_MODE_CNTL>().multi_prim_ib_ena;
    auto [low, high] = s.DrawIndexRange(base, index_address, index_count, index_32bit,
                                        index_endian, skip_restart);
    if (s.log_index_ranges) {
      s.CountIndexRange(index_address, index_count, index_32bit, index_endian, skip_restart, low,
                        high);
    }
    if (low <= high) {
      // After the base vertex, as the GPU reads them. Adding it to the
      // extremes keeps their order unless the sum wraps (a negative base
      // vertex under some index), where each index is taken again.
      int64_t low_sum = int64_t(int32_t(low)) + base_vertex;
      int64_t high_sum = int64_t(int32_t(high)) + base_vertex;
      if (low_sum >= 0 && high_sum <= INT32_MAX && int32_t(low) >= 0 && int32_t(high) >= 0) {
        min_vertex = uint32_t(low_sum);
        max_vertex = uint32_t(high_sum);
      } else {
        for (uint32_t i = 0; i < index_count; i++) {
          auto [index, unused] = IndexRange(PhysicalHostAddress(base, index_address) +
                                                (index_32bit ? 4 : 2) * i,
                                            1, index_32bit, index_endian, skip_restart);
          if (index == UINT32_MAX && !unused) {
            continue;  // the restart index
          }
          max_vertex = std::max(max_vertex, uint32_t(int32_t(index) + base_vertex));
          min_vertex = std::min(min_vertex, uint32_t(int32_t(index) + base_vertex));
        }
      }
    }
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
  if (system.vertex_index_max <= system.vertex_index_min || call.inline_vertices) {
    system.vertex_index_min = 0;
    system.vertex_index_max = xenos::kVertexIndexMask;
  }
  // Indices the clamp changes (the registers may be another draw's).
  if (index_count && (max_vertex > system.vertex_index_max ||
                      min_vertex < system.vertex_index_min)) {
    s.stats.clamped_draws++;
    if (!s.stats.clamped_sample[1]) {
      s.stats.clamped_sample = {min_vertex, max_vertex, system.vertex_index_min,
                                system.vertex_index_max};
    }
  }
  if (s.no_index_clamp) {
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
  // bool/loop, fetch. A block the same as the last one written points at it.
  static_assert(sizeof(system) <= kFloatConstantsRange);
  uint32_t offsets[5];
  uint32_t packed_bytes[2] = {};
  if (!s.ReserveDrawConstants() ||
      !s.PlaceConstants(0, &system, sizeof(system), sizeof(system), offsets[0])) {
    s.stats.failed++;
    s.stats.failed_by[4]++;
    return;
  }
  if (!s.PlaceFloatConstants(base, device, vertex, pixel, offsets, packed_bytes)) {
    s.stats.failed++;
    s.stats.failed_by[4]++;
    return;
  }
  // Why the fetch block changed (log_constant_changes): in the fetch
  // constants the shaders read (textures, vertices), or only in others.
  const uint32_t* fetch = &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0];
  if (s.log_constant_changes && s.last_constants.valid[4] &&
      s.last_constants.sizes[4] == kFetchConstantsRange) {
    const uint32_t* last = reinterpret_cast<const uint32_t*>(s.last_constants.bytes[4]);
    if (std::memcmp(last, fetch, kFetchConstantsRange)) {
      s.stats.fetch_changed++;
      bool texture_differs = false, vertex_differs = false, vertex_address_only = true;
      for (const TranslatedShader* shader : {vertex, pixel}) {
        if (!shader) {
          continue;
        }
        for (const ShaderTexture& texture : shader->textures) {
          uint32_t i = 6 * texture.fetch_constant;
          texture_differs |= !!std::memcmp(last + i, fetch + i, 24);
        }
        for (const ShaderSampler& sampler : shader->samplers) {
          uint32_t i = 6 * sampler.fetch_constant;
          texture_differs |= !!std::memcmp(last + i, fetch + i, 24);
        }
      }
      for (uint32_t k = 0; vertex && k < 96; k++) {
        if (!(vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31)))) {
          continue;
        }
        if (last[2 * k + 1] != fetch[2 * k + 1]) {
          vertex_differs = true;
          vertex_address_only = false;
        } else if (last[2 * k] != fetch[2 * k]) {
          vertex_differs = true;
        }
      }
      s.stats.fetch_changed_texture += texture_differs;
      s.stats.fetch_changed_vertex += vertex_differs;
      s.stats.fetch_changed_vertex_address += vertex_differs && vertex_address_only;
      s.stats.fetch_changed_unread += !texture_differs && !vertex_differs;
    }
  }
  if (!s.PlaceConstants(3, &regs.values[XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031],
                        kBoolLoopConstantsRange, kBoolLoopConstantsRange, offsets[3]) ||
      !s.PlaceConstants(4, fetch, kFetchConstantsRange, kFetchConstantsRange, offsets[4])) {
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
  VkDescriptorSet vertex_textures = s.GetTextureSet(base, vertex, vertex_layout, 0);
  VkDescriptorSet pixel_textures = s.GetTextureSet(base, pixel, pixel_layout, 1);
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
  auto mode = regs.Get<reg::PA_SU_SC_MODE_CNTL>();
  key.primitive_restart = (primitive.strip && mode.multi_prim_ib_ena) || rectangle_list;
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
  // As the plugin normalizes it: nothing without a pixel shader, for a
  // target the pixel shader doesn't write, or in depth-only EDRAM mode.
  // (With the raw mask, those draws wrote Vulkan's undefined fragment output:
  // the rainbow effect on buildings, from ~230 main view draws a frame
  // without a pixel shader.)
  key.write_mask =
      draw_util::GetNormalizedColorMask(regs, pixel ? pixel->writes_color_targets : 0) & 0xF;
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
                &regs.values[XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031],
                &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0]);
  }
  if (!pipeline) {
    s.stats.failed++;
    return;
  }

  // The draw's indices and vertex buffers, into the mirror (an inline
  // draw's vertices are there already). A vertex fetch constant is two
  // words: the address (word 0, low 2 bits the type) and the size in words
  // (word 1, bits 2-25).
  if (!call.inline_vertices && mirror_per_draw) {
    s.Mirror(base, index_address, index_count * (index_32bit ? 4 : 2), true,
             s.draw_index_newest);
  }
  // The highest vertex the draw uses (indices as the GPU reads them, after
  // the base vertex): a vertex buffer is copied up to it even past the size
  // its fetch constant gives, as the GPU reads whatever is there.
  for (uint32_t k = 0; k < 96 && !call.inline_vertices && mirror_per_draw; k++) {
    if (!(vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31)))) {
      continue;
    }
    uint32_t word0 = regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k];
    uint32_t word1 = regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0 + 2 * k + 1];
    uint32_t size = ((word1 >> 2) & 0xFFFFFFu) * 4;
    for (const auto& [fetch_constant, stride_words] : vertex->vertex_strides) {
      uint64_t needed = (uint64_t(max_vertex) + 1) * stride_words * 4;
      if (fetch_constant == k && needed > size && needed < (64u << 20)) {
        s.stats.vertex_overruns++;
        if (!s.stats.vertex_overrun_sample[1]) {
          s.stats.vertex_overrun_sample = {word0 & 0x1FFFFFFCu, size, uint32_t(needed),
                                           max_vertex, stride_words};
        }
        size = uint32_t(needed);
      }
    }
    if (s.check_mirror_frames) {
      // The bytes this draw's vertices span in this buffer.
      uint64_t begin = UINT64_MAX, end = 0;
      for (const auto& [fetch_constant, stride_words] : vertex->vertex_strides) {
        if (fetch_constant == k && min_vertex <= max_vertex) {
          begin = std::min(begin, uint64_t(min_vertex) * stride_words * 4);
          end = std::max(end, (uint64_t(max_vertex) + 1) * stride_words * 4);
        }
      }
      if (begin >= end) {
        begin = 0;
        end = size;  // stride unknown: the whole buffer
      }
      end = std::min<uint64_t>(end, size);
      if (begin < end) {
        uint32_t address = (word0 & 0x1FFFFFFCu) + uint32_t(begin);
        uint32_t length = uint32_t(end - begin);
        if (s.used_mirrored.insert((uint64_t(address) << 32) | length).second) {
          s.used_vertex_bytes += length;
        }
      }
    }
    s.Mirror(base, word0 & 0x1FFFFFFCu, size, false);
  }

  // Draw.
  // Both scaled from the guest's pixels (the NDC scale and offset are
  // ratios, unchanged), by the targets' scale (colour and depth share one:
  // the main view's surfaces are both scaled alike).
  float draw_scale = color ? color->scale : depth->scale;
  VkViewport viewport{float(viewport_info.xy_offset[0]) * draw_scale,
                      float(viewport_info.xy_offset[1]) * draw_scale,
                      float(viewport_info.xy_extent[0]) * draw_scale,
                      float(viewport_info.xy_extent[1]) * draw_scale,
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
  DrawEmit e{};
  if (depth) {
    // As the plugin sets it for its host render targets: the guest's depth
    // is float24 (24_8_FLOAT) or unorm24, the slope in 1/16 pixels.
    draw_util::GetPreferredFacePolygonOffset(regs, primitive.polygonal, slope, constant);
    constant *= depth->surface.format == uint32_t(xenos::DepthRenderTargetFormat::kD24S8)
                    ? draw_util::kD3D10PolygonOffsetFactorUnorm24
                    : draw_util::kD3D10PolygonOffsetFactorFloat24;
    // Per pixel: scaled pixels are smaller, so the slope (as the plugin
    // scales it).
    slope *= xenos::kPolygonOffsetScaleSubpixelUnit * std::max(draw_scale, 1.0f);
    e.depth_biased = constant != 0.0f || slope != 0.0f;
    e.depth_equal = key.z_enable && key.z_func == VK_COMPARE_OP_EQUAL;
  }
  e.viewport = viewport;
  e.scissor = scissor_rect;
  e.bias[0] = constant;
  e.bias[1] = slope;
  // RB_BLEND_RED/GREEN/BLUE/ALPHA, for the constant blend factors.
  std::memcpy(e.blend_constants, &regs.values[0x2105], sizeof(e.blend_constants));
  e.pipeline = pipeline;
  e.layout = key.layout;
  e.textures[0] = vertex_textures;
  e.textures[1] = pixel_textures;
  // Indexed draws bind the mirror at offset 0 and start at their indices
  // with firstIndex, so consecutive draws share the binding (unless an
  // address isn't a multiple of the index size).
  e.indexed = rectangle_list || quad_list || !call.inline_vertices;
  e.index_type = rectangle_list || quad_list || index_32bit ? VK_INDEX_TYPE_UINT32
                                                            : VK_INDEX_TYPE_UINT16;
  uint32_t index_bytes = e.index_type == VK_INDEX_TYPE_UINT32 ? 4 : 2;
  uint64_t index_start = rectangle_list ? kRectangleIndices
                         : quad_list    ? kQuadIndices
                                        : index_address;
  e.first_index = 0;
  e.index_offset = 0;
  if (s.skip_repeated_state && index_start % index_bytes == 0) {
    e.first_index = uint32_t(index_start / index_bytes);
  } else {
    e.index_offset = index_start;
  }
  e.count = rectangle_list ? RectangleIndexCount(index_count / 3)
            : quad_list    ? index_count / 4 * 6
                           : index_count;
  e.inline_vertices = call.inline_vertices;
  e.type = type;
  if (s.tick_expected) {
    s.CheckTickDraw(*s.tick_expected, true, color, depth, vertex, pixel, &system, &e);
  }
  State::RecordedState now = s.RecordDraw(commands, color, depth, e, offsets);
  uint32_t first_index = e.first_index;
  const VkDescriptorSet* sets = now.sets;
  if (capture) {
    capture->eligible = !vertex_copy;
    capture->drawn = true;
    capture->color = color;
    capture->depth = depth;
    capture->vertex = vertex;
    capture->pixel = pixel;
    capture->system = system;
    capture->emit = e;
    capture->main_view_draw = main_view_draw;
    uint32_t mask = 0;
    for (const TranslatedShader* shader : {vertex, pixel}) {
      for (size_t i = 0; shader && i < shader->textures.size(); i++) {
        mask |= 1u << (shader->textures[i].fetch_constant & 31);
      }
      for (size_t i = 0; shader && i < shader->samplers.size(); i++) {
        mask |= 1u << (shader->samplers[i].fetch_constant & 31);
      }
    }
    capture->texture_mask = mask;
    capture->textures_hash =
        TextureWordsHash(&regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0], mask);
    // A texture set not kept (a placeholder for a texture not loaded yet,
    // see GetTextureSet) is asked again each frame.
    if (!s.last_texture_sets[0].set || !s.last_texture_sets[1].set) {
      capture->eligible = false;
    }
  }
  if (s.log_tick_repeats) {
    s.NoteDrawParts(call, color, depth, now, vertex, pixel, index_start, index_count, first_index,
                    &system, sizeof(system), packed_bytes);
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
    id.index_offset = index_address;
    id.index_type = index_32bit ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16;
    id.index_count = index_count;
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
  raster.depthBiasEnable = VK_TRUE;
  raster.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo multisample{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineDepthStencilStateCreateInfo depth{
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
  depth.depthTestEnable = key.z_enable;
  depth.depthWriteEnable = key.z_write;
  depth.depthCompareOp = VkCompareOp(key.z_func);
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
    reversed |= draw.key.z_enable && (draw.key.z_func == VK_COMPARE_OP_GREATER ||
                                      draw.key.z_func == VK_COMPARE_OP_GREATER_OR_EQUAL);
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
                                draw.sets, 5, draw.offsets);
    vk::vkCmdPushConstants(commands, layout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 4, &draw.number);
    vk::vkCmdBindIndexBuffer(commands, draw_mirror().buffer, draw.index_offset, draw.index_type);
    vk::vkCmdDrawIndexed(commands, draw.index_count, 1, 0, 0, 0);
  }
  vk::vkCmdEndRendering(commands);
  recorded.valid = false;
  Transition(commands, id_image.image, VK_IMAGE_ASPECT_COLOR_BIT, color_layout,
             VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  id_drawn = true;
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
  s.ReadState(base, device);
  for (uint32_t i = 0; i < arg1; i++) {
    uint32_t item = payload + 12 * i;
    DrawCall call;
    call.type = guest::LoadU32(base, item);
    call.base_start = guest::LoadU32(base, item + 4);
    call.primitive_count = guest::LoadU32(base, item + 8);
    s.Draw(commands, base, device, call);
  }
}

void Passes::InlineDraw(VkCommandBuffer commands, uint8_t* base, uint32_t type, uint32_t count,
                        uint32_t stride, uint32_t data) {
  State& s = *state_;
  uint32_t size = count * stride;
  uint32_t offset = (s.inline_offset + 15) & ~15u;
  if (!count || !stride || offset + size > s.inline_slot_size()) {
    s.stats.failed++;
    s.stats.failed_by[7]++;
    return;
  }
  s.inline_offset = offset + size;
  DrawCall call;
  call.type = type;
  call.inline_vertices = true;
  call.vertex_count = count;
  call.vertex_address =
      kInlineVertices + (s.mirror_by_tick ? s.frame * s.inline_slot_size() : 0) + offset;
  std::memcpy(s.draw_mirror().mapped + call.vertex_address, base + data, size);
  if (s.log_tick_repeats) {
    call.inline_hash = XXH3_64bits(base + data, size) ^ (uint64_t(stride) << 32);
  }
  s.stats.mirrored_bytes += size;
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  s.ReadState(base, device);
  // What BeginVertices wrote to the GPU instead of the shadow.
  uint32_t* fetch = &s.regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0];
  fetch[2 * (kInlineFetchConstant - 2)] = 1;
  fetch[2 * (kInlineFetchConstant - 2) + 1] = 0;
  fetch[2 * (kInlineFetchConstant - 1)] = 1;
  fetch[2 * (kInlineFetchConstant - 1) + 1] = 0;
  fetch[2 * kInlineFetchConstant] = call.vertex_address | 3;
  fetch[2 * kInlineFetchConstant + 1] =
      0x10000000u | ((size / 4) << 2) | uint32_t(xenos::Endian::k8in32);
  s.regs.values[XE_GPU_REG_VGT_INDX_OFFSET] = 0;
  s.Draw(commands, base, device, call);
  // Read them from the shadow again next time.
  static_assert((kInlineFetchConstant - 2) / 3 == kInlineFetchConstant / 3);
  MarkFetchConstantsDirty(1ull << (63 - kInlineFetchConstant / 3));
  MarkRegistersDirty(XE_GPU_REG_VGT_INDX_OFFSET, 1ull << 63);
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
  // docs/gpu-functions.md); its flags are State::Resolve's.
  for (uint32_t i = 0; i < arg1; i++) {
    uint32_t item = payload + 68 * i;
    uint32_t fields = guest::LoadU32(base, item);
    uint32_t flags = guest::LoadU32(base, item + 4);
    int32_t rect[4], point[2];
    for (uint32_t k = 0; k < 4; k++) {
      rect[k] = int32_t(guest::LoadU32(base, item + 8 + 4 * k));
    }
    point[0] = int32_t(guest::LoadU32(base, item + 28));
    point[1] = int32_t(guest::LoadU32(base, item + 32));
    // The clear colour's 4 bytes, as floats (as the handler converts them).
    float color[4] = {};
    for (uint32_t c = 0; (fields & 8) && c < 4; c++) {
      color[c] = float(guest::LoadU8(base, item + 44 + c)) / 255.0f;
    }
    uint32_t z = guest::LoadU32(base, item + 48);
    float clear_z;
    std::memcpy(&clear_z, &z, 4);
    s.Resolve(commands, base, device, flags, (fields & 1) ? rect : nullptr,
              (fields & 2) ? guest::LoadU32(base, item + 24) : 0, (fields & 4) ? point : nullptr,
              guest::LoadU32(base, item + 36) || guest::LoadU32(base, item + 40), color, clear_z,
              guest::LoadU32(base, item + 52), true);
  }
}

void Passes::State::Resolve(VkCommandBuffer commands, uint8_t* base, uint32_t device,
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
  uint32_t color_object = guest::LoadU32(base, device + kDeviceRenderTarget0 + 4 * (flags & 3));
  uint32_t depth_object = guest::LoadU32(base, device + kDeviceDepthStencil);
  uint32_t source_object = from_depth ? depth_object : color_object;
  Target* source = source_object ? GetTarget(commands, base, source_object, from_depth) : nullptr;
  if (source && destination && !level_or_slice &&
      ResolveInto(commands, base, *source, destination, rect, point, flags)) {
    stats.resolves++;
  } else {
    stats.resolves_skipped++;
  }
  if (!apply_clears) {
    return;
  }
  if ((flags & 0x100) && color_object) {
    if (Target* target = GetTarget(commands, base, color_object, false)) {
      VkClearValue value{};
      for (uint32_t c = 0; clear_color && c < 4; c++) {
        value.color.float32[c] = clear_color[c];
      }
      ClearTarget(commands, *target, value);
    }
  }
  if ((flags & 0x200) && depth_object) {
    if (Target* target = GetTarget(commands, base, depth_object, true)) {
      VkClearValue value{};
      value.depthStencil.depth = clear_z;
      value.depthStencil.stencil = clear_stencil & 0xFF;
      ClearTarget(commands, *target, value);
    }
  }
}

void Passes::BeginTiling(VkCommandBuffer commands, uint8_t* base, uint32_t payload) {
  State& s = *state_;
  uint32_t device = guest::LoadU32(base, kDevicePointer);
  // Command 32's item (16 bytes): +0 flags, +4 tile count, +8 the tiles'
  // rectangles (16 bytes each: left, top, right, bottom). The handler gives
  // BeginTiling a clear colour and Z of the float at 0x82000A34, stencil 0.
  uint32_t flags = guest::LoadU32(base, payload);
  uint32_t count = guest::LoadU32(base, payload + 4);
  uint32_t rects = guest::LoadU32(base, payload + 8);
  uint32_t width = 0, height = 0;
  std::string tiles;
  for (uint32_t i = 0; rects && i < count && i < 16; i++) {
    int32_t r[4];
    for (uint32_t k = 0; k < 4; k++) {
      r[k] = int32_t(guest::LoadU32(base, rects + 16 * i + 4 * k));
    }
    width = std::max(width, uint32_t(std::max(r[2], 0)));
    height = std::max(height, uint32_t(std::max(r[3], 0)));
    tiles += fmt::format(" {},{}-{},{}", r[0], r[1], r[2], r[3]);
  }
  uint32_t clear_bits = guest::LoadU32(base, 0x82000A34);
  float clear;
  std::memcpy(&clear, &clear_bits, 4);
  if (s.logged_tilings.insert(uint64_t(rects) << 32 | count).second) {
    REXLOG_INFO("native renderer: begin tiling: flags {:X}, {} tiles:{}; clear colour and Z {}",
                flags, count, tiles, clear);
  }
  s.EndRendering(commands);
  s.tiling = true;
  s.tiling_extent = {std::min(width, 8192u), std::min(height, 8192u)};
  s.stats.tilings++;
  // Each tile starts cleared to these.
  if (uint32_t object = guest::LoadU32(base, device + kDeviceRenderTarget0)) {
    if (Target* target = s.GetTarget(commands, base, object, false)) {
      VkClearValue value{};
      value.color = {{clear, clear, clear, clear}};
      s.ClearTarget(commands, *target, value);
    }
  }
  if (uint32_t object = guest::LoadU32(base, device + kDeviceDepthStencil)) {
    if (Target* target = s.GetTarget(commands, base, object, true)) {
      VkClearValue value{};
      value.depthStencil = {clear, 0};
      s.ClearTarget(commands, *target, value);
    }
  }
}

void Passes::TiledResolve(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                          uint32_t flags, uint32_t rect_address, uint32_t destination,
                          uint32_t point_address, uint32_t clear_color_address, float clear_z,
                          uint32_t tile, uint32_t tiles) {
  State& s = *state_;
  int32_t rect[4], point[2];
  for (uint32_t k = 0; rect_address && k < 4; k++) {
    rect[k] = int32_t(guest::LoadU32(base, rect_address + 4 * k));
  }
  for (uint32_t k = 0; point_address && k < 2; k++) {
    point[k] = int32_t(guest::LoadU32(base, point_address + 4 * k));
  }
  float color[4] = {};
  for (uint32_t c = 0; clear_color_address && c < 4; c++) {
    uint32_t bits = guest::LoadU32(base, clear_color_address + 4 * c);
    std::memcpy(&color[c], &bits, 4);
  }
  if (s.logged_tiled_resolves.insert(uint64_t(destination) << 8 | tile).second) {
    REXLOG_INFO("native renderer: tiled resolve {} of {}: flags {:08X}, rect {},{}-{},{}, to "
                "{:08X} at {},{}",
                tile + 1, tiles, flags, rect_address ? rect[0] : -1, rect_address ? rect[1] : -1,
                rect_address ? rect[2] : -1, rect_address ? rect[3] : -1, destination,
                point_address ? point[0] : -1, point_address ? point[1] : -1);
  }
  // The library clears the target after each tile but the last for the
  // next tile; the whole surface is drawn at once here, so only the last
  // tile's clears are made.
  s.Resolve(commands, base, device, flags, rect_address ? rect : nullptr, destination,
            point_address ? point : nullptr, false, color, clear_z, 0, tile + 1 >= tiles);
  s.stats.tiled_resolves++;
}

void Passes::EndTiling() {
  state_->tiling = false;
}

void Passes::FrontBufferResolve(VkCommandBuffer commands, uint8_t* base, uint32_t device,
                                uint32_t front_buffer) {
  State& s = *state_;
  uint32_t object = guest::LoadU32(base, device + kDeviceRenderTarget0);
  Target* target = object ? s.GetTarget(commands, base, object, false) : nullptr;
  // The front buffer's size, from its fetch constant's word 2 (2D: width - 1
  // in bits 0-12, height - 1 in bits 13-25).
  uint32_t size = front_buffer ? guest::LoadU32(base, front_buffer + 28 + 8) : 0;
  s.front_extent = {(size & 0x1FFF) + 1, ((size >> 13) & 0x1FFF) + 1};
  s.front = target;
  if (!s.logged_front) {
    s.logged_front = true;
    Surface surface = object ? ReadSurface(base, object) : Surface{};
    uint32_t format = front_buffer ? guest::LoadU32(base, front_buffer + 28 + 4) & 0x3F : 0;
    REXLOG_INFO("native renderer: front buffer resolve: device {:08X} (ours {:08X}), render "
                "target 0 {:08X} (pitch {}, MSAA {}, format {}, EDRAM base {}), into {:08X}: "
                "{}x{} format {}",
                device, guest::LoadU32(base, kDevicePointer), object, surface.pitch, surface.msaa,
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
      glow_addresses[logic] += fmt::format("{:08X} ", address);
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
    REXLOG_INFO("native renderer: glow: logic frames {} (cross-fades {}, history resolves {}, "
                "reading the history {}), render-only frames {} ({}, {}, {}); last read "
                "{:08X}, last history {:08X}; 256x144 resolves of a logic frame: {}, of a "
                "render-only frame: {}; frames: {}",
                glow_counts[1].frames, glow_counts[1].cross_fades,
                glow_counts[1].history_resolves, glow_counts[1].reads_history,
                glow_counts[0].frames, glow_counts[0].cross_fades,
                glow_counts[0].history_resolves, glow_counts[0].reads_history,
                glow_frame.cross_fade_texture, glow_frame.history_address, glow_addresses[1],
                glow_addresses[0], glow_pattern);
    REXLOG_INFO("native renderer: glow cross-fade constants: logic frame: {}render-only "
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

void Passes::set_log_register_reads(bool log) {
  state_->log_register_reads = log;
}

void Passes::State::NoteDrawParts(const DrawCall& call, const Target* color, const Target* depth,
                                  const RecordedState& now, const TranslatedShader* vertex,
                                  const TranslatedShader* pixel, uint64_t index_start,
                                  uint32_t index_count, uint32_t first_index, const void* system,
                                  size_t system_size, const uint32_t packed_bytes[2]) {
  DrawParts& d = frame_parts.emplace_back();
  const uint32_t* fetch = &regs.values[XE_GPU_REG_SHADER_CONSTANT_FETCH_00_0];
  auto hash = [](const void* data, size_t size) { return XXH3_64bits(data, size); };
  const void* targets[2] = {color, depth};
  d.hash[kPartTarget] = hash(targets, sizeof(targets));
  d.hash[kPartPipeline] = hash(&now.pipeline, sizeof(now.pipeline));
  // The texture fetch constants the shaders read (textures and samplers).
  std::vector<uint32_t> words;
  for (const TranslatedShader* shader : {vertex, pixel}) {
    for (size_t i = 0; shader && i < shader->textures.size(); i++) {
      uint32_t k = shader->textures[i].fetch_constant;
      words.push_back(k);
      words.insert(words.end(), fetch + 6 * k, fetch + 6 * k + 6);
    }
    for (size_t i = 0; shader && i < shader->samplers.size(); i++) {
      uint32_t k = shader->samplers[i].fetch_constant;
      words.push_back(k);
      words.insert(words.end(), fetch + 6 * k, fetch + 6 * k + 6);
    }
  }
  d.hash[kPartTextures] = hash(words.data(), 4 * words.size());
  // The vertex fetch constants it reads; an inline draw's own (93-95: its
  // vertices' place in the mirror) left out.
  words.clear();
  for (uint32_t k = 0; vertex && k < 96; k++) {
    if ((vertex->vertex_fetch_constants[k >> 5] & (1u << (k & 31))) &&
        !(call.inline_vertices && k >= kInlineFetchConstant - 2)) {
      words.insert(words.end(), {k, fetch[2 * k], fetch[2 * k + 1]});
    }
  }
  d.hash[kPartVertexFetch] = hash(words.data(), 4 * words.size());
  uint64_t indices[5] = {call.type, call.inline_vertices ? 0 : index_start, index_count,
                         call.inline_vertices ? 0 : first_index, uint64_t(now.index_type)};
  d.hash[kPartIndices] = hash(indices, sizeof(indices));
  d.hash[kPartDynamic] = hash(&now.viewport, sizeof(now.viewport)) ^
                         3 * hash(&now.scissor, sizeof(now.scissor)) ^
                         5 * hash(now.bias, sizeof(now.bias)) ^
                         7 * hash(now.blend_constants, sizeof(now.blend_constants));
  d.hash[kPartInline] = call.inline_hash;
  d.hash[kPartSystem] = hash(system, system_size);
  d.hash[kPartVertexFloats] = hash(last_constants.bytes[1], packed_bytes[0]);
  d.hash[kPartPixelFloats] = hash(last_constants.bytes[2], packed_bytes[1]);
  d.hash[kPartBoolLoop] =
      hash(&regs.values[XE_GPU_REG_SHADER_CONSTANT_BOOL_000_031], kBoolLoopConstantsRange);
  d.pixel = pixel ? pixel->hash : 0;
  d.pass = uint32_t(frame_resolves.size());
}

void Passes::State::CompareTickRepeats() {
  TickRepeatCounts& c = tick_repeats;
  if (Renderer_ReplayingLogicList()) {
    // A tick's first frame: what the frames after it are compared with.
    tick_parts.swap(frame_parts);
    tick_parts_valid = true;
    c.logic_frames++;
    frame_parts.clear();
    return;
  }
  if (!tick_parts_valid) {
    frame_parts.clear();
    return;
  }
  static constexpr const char* kPartNames[kDrawParts] = {
      "target", "pipeline", "textures", "vertex fetch", "indices", "dynamic state",
      "inline vertices", "system constants", "vertex floats", "pixel floats", "bool/loop"};
  c.frames++;
  c.draws += frame_parts.size();
  const std::vector<DrawParts>& t = tick_parts;
  const std::vector<DrawParts>& f = frame_parts;
  if (f.size() != t.size()) {
    c.count_delta += f.size() > t.size() ? f.size() - t.size() : t.size() - f.size();
  } else {
    c.same_count++;
  }
  // Aligned draw by draw up to the shorter; the first divergence sampled.
  size_t n = std::min(f.size(), t.size());
  bool structure_same = f.size() == t.size(), but_floats_same = structure_same,
       all_same = structure_same;
  bool sampled = false;
  for (size_t i = 0; i < n; i++) {
    uint32_t mask = 0;
    for (uint32_t k = 0; k < kDrawParts; k++) {
      if (f[i].hash[k] != t[i].hash[k]) {
        mask |= 1u << k;
        if (f.size() == t.size()) {
          c.differ[k]++;
        }
      }
    }
    bool structure = mask & ((1u << kStructureParts) - 1);
    c.structure_differ += structure && f.size() == t.size();
    structure_same &= !structure;
    but_floats_same &= !(mask & ~((1u << kPartVertexFloats) | (1u << kPartPixelFloats)));
    all_same &= !mask;
    if (structure && !sampled && c.samples.size() < 4) {
      sampled = true;
      std::string parts;
      for (uint32_t k = 0; k < kDrawParts; k++) {
        if (mask & (1u << k)) {
          parts += fmt::format("{}{}", parts.empty() ? "" : ", ", kPartNames[k]);
        }
      }
      c.samples.push_back(fmt::format(
          "draw {} of {} (tick's {}), pass {} (tick's {}), ps {:016X} (tick's {:016X}): {}", i,
          f.size(), t.size(), f[i].pass, t[i].pass, f[i].pixel, t[i].pixel, parts));
    }
  }
  if (!sampled && f.size() != t.size() && c.samples.size() < 4) {
    c.samples.push_back(fmt::format("{} draws against the tick's {}, the same up to there",
                                    f.size(), t.size()));
  }
  c.structure_same += structure_same;
  c.all_but_floats_same += but_floats_same;
  c.all_same += all_same;
  frame_parts.clear();
}

void Passes::set_tick_draws(bool on, bool check) {
  state_->tick_draws = on;
  state_->check_tick_draws = check;
}

std::string Passes::TakeTickDraws() {
  State::TickDrawCounts& c = state_->tick_counts;
  uint64_t asked = c.matched + c.no_entry + c.ineligible + c.key_differs + c.textures_differ;
  double a = double(std::max<uint64_t>(asked, 1));
  std::string text = fmt::format(
      "tick draws: {} ticks captured ({:.1f} draws each), {} frames after them: of {:.1f} draws "
      "a frame, reused {:.1f}% (invisible {:.1f}%), worked out because: no draw stored at its "
      "place {:.2f}%, the stored one not kept {:.2f}%, key differs {:.2f}% (call {}, surfaces "
      "{}, shaders {}, indices {}, scissor {}, registers {}, texture generations {}), texture "
      "fetch constants differ {:.2f}%",
      c.captured_frames, double(c.captured) / double(std::max<uint64_t>(c.captured_frames, 1)),
      c.frames, a / double(std::max<uint64_t>(c.frames, 1)), 100.0 * c.matched / a,
      100.0 * c.invisible / a, 100.0 * c.no_entry / a, 100.0 * c.ineligible / a,
      100.0 * c.key_differs / a, c.key_parts[0], c.key_parts[1], c.key_parts[2], c.key_parts[3],
      c.key_parts[4], c.key_parts[5], c.key_parts[6], 100.0 * c.textures_differ / a);
  if (state_->check_tick_draws) {
    text += fmt::format("; checked {}, differing from the full path {}", c.checked,
                        c.check_mismatches);
    for (const std::string& sample : c.check_samples) {
      text += "; e.g. " + sample;
    }
  }
  c = {};
  return text;
}

void Passes::set_log_tick_repeats(bool log) {
  state_->log_tick_repeats = log;
}

std::string Passes::TakeTickRepeats() {
  State::TickRepeatCounts& c = state_->tick_repeats;
  static constexpr const char* kPartNames[State::kDrawParts] = {
      "target", "pipeline", "textures", "vertex fetch", "indices", "dynamic", "inline",
      "system", "vertex floats", "pixel floats", "bool/loop"};
  double frames = double(std::max<uint64_t>(c.frames, 1));
  double draws = double(std::max<uint64_t>(c.draws, 1));
  std::string differ;
  for (uint32_t k = 0; k < State::kDrawParts; k++) {
    differ += fmt::format("{}{} {:.2f}%", k ? ", " : "", kPartNames[k], 100.0 * c.differ[k] / draws);
  }
  std::string text = fmt::format(
      "tick repeats: {} logic and {} render-only frames ({:.1f} draws a frame); of the "
      "render-only frames, the tick's draw count {:.1f}% (else {:.1f} draws apart on average), "
      "every draw's target, pipeline, textures, vertex fetch, indices and dynamic state the "
      "tick's {:.1f}%, all but the float constants {:.1f}%, everything {:.1f}%; draws differing "
      "from the tick's (frames of its count): structure {:.2f}%, by part: {}",
      c.logic_frames, c.frames, c.draws / frames, 100.0 * c.same_count / frames,
      c.frames > c.same_count ? double(c.count_delta) / double(c.frames - c.same_count) : 0.0,
      100.0 * c.structure_same / frames, 100.0 * c.all_but_floats_same / frames,
      100.0 * c.all_same / frames, 100.0 * c.structure_differ / draws, differ);
  for (const std::string& sample : c.samples) {
    text += "; e.g. " + sample;
  }
  c = {};
  return text;
}

void Passes::set_cache_index_ranges(bool cache) {
  state_->cache_index_ranges = cache;
}

void Passes::set_log_index_ranges(bool log) {
  state_->log_index_ranges = log;
}

std::string Passes::TakeIndexRanges() {
  State::IndexRangeCounts& c = state_->index_range_counts;
  double frames = double(std::max<uint64_t>(c.frames, 1));
  double scans = double(std::max<uint64_t>(c.scans, 1));
  double asked = double(std::max<uint64_t>(c.newest_asked, 1));
  std::string text = fmt::format(
      "index ranges: {:.1f} scans a frame ({:.0f} indices), {:.1f} distinct; of the scans, "
      "first seen {:.1f}%, seen earlier this frame {:.1f}%, written by memcpy since last "
      "scanned {:.1f}%, unchanged {:.1f}% ({:.0f} indices a frame), unchanged with another "
      "result {}; memcpy writes taken {:.1f} a frame; mirror: {:.1f} calls a frame, {:.1f}% "
      "already copied this frame, {:.1f} asked whether written, {:.1f}% of those with no "
      "write taken since the copy; {} ranges known",
      c.scans / frames, c.indices / frames, c.distinct / frames, 100.0 * c.first / scans,
      100.0 * c.same_frame / scans, 100.0 * c.written / scans, 100.0 * c.unchanged / scans,
      c.unchanged_indices / frames, c.mismatched, c.writes / frames, c.mirror_calls / frames,
      100.0 * c.mirror_repeats / double(std::max<uint64_t>(c.mirror_calls, 1)),
      c.newest_asked / frames, 100.0 * c.nothing_taken / asked,
      state_->index_ranges_seen.size());
  if (c.mismatched) {
    const uint32_t* m = c.mismatch_sample;
    text += fmt::format(" (e.g. {:08X} x{}: {}-{} kept, {}-{} scanned)", m[0], m[1], m[2], m[3],
                        m[4], m[5]);
  }
  uint64_t frame_sequence = c.frame_sequence;
  c = {};
  c.frame_sequence = frame_sequence;
  return text;
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
  std::string text = fmt::format(
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

void Passes::set_log_shader_lookups(bool log) {
  state_->log_shader_lookups = log;
}

std::string Passes::TakeShaderLookups() {
  State& s = *state_;
  State::ShaderLookupCounts& c = s.lookup_counts;
  double frames = double(std::max<uint64_t>(c.frames, 1));
  auto share = [](uint64_t n, uint64_t of) { return 100.0 * double(n) / double(std::max<uint64_t>(of, 1)); };
  std::string text = fmt::format(
      "shader lookups: {:.1f} draws a frame ({:.1f} with the patched copy); the same keys as "
      "the last draw: vertex {:.1f}%, pixel {:.1f}%, whole lookup {:.1f}%; seen before: vertex "
      "{:.1f}%, pixel {:.1f}%, whole lookup {:.1f}%; stale (seen, microcode changed): vertex {} "
      "({} with a patch between), pixel {} ({} with a patch between); {} vertex shader patches; "
      "keys this session: vertex {}, pixel {}, whole lookups {}",
      c.draws / frames, c.copies / frames, share(c.same[0], c.draws), share(c.same[1], c.draws),
      share(c.lookup_same, c.draws), share(c.seen[0], c.draws), share(c.seen[1], c.draws),
      share(c.lookup_seen, c.draws), c.stale[0], c.stale_patched[0], c.stale[1],
      c.stale_patched[1], g_vertex_shader_patches - c.patches, s.shaders_seen[0].size(),
      s.shaders_seen[1].size(), s.lookups_seen.size());
  static const char* const kStage[2] = {"vertex", "pixel"};
  for (int stage = 0; stage < 2; stage++) {
    const State::ShaderKey& k = c.stale_sample[stage];
    if (k.object) {
      text += fmt::format(
          "; stale {} sample without a patch: object {:08X} microcode {:08X} size {} patched for "
          "{} declaration {} copy {}, hash {:016X} then {:016X}",
          kStage[stage], k.object, k.microcode, k.size, k.patched_for, k.declaration_id, k.copy,
          c.stale_hashes[stage][0], c.stale_hashes[stage][1]);
    }
  }
  c = {};
  c.patches = g_vertex_shader_patches;
  return text;
}

void Passes::set_copy_all(bool copy_all) {
  state_->copy_all = copy_all;
}

void Passes::set_mirror_to_both(bool to_both) {
  state_->mirror_to_both = to_both;
}

void Passes::set_mirror_by_tick(bool by_tick) {
  State& s = *state_;
  if (by_tick && s.mirrors.size() != 2) {
    REXLOG_WARN("native renderer: mirror by tick needs two mirrors, not {}: off",
                s.mirrors.size());
    return;
  }
  s.mirror_by_tick = by_tick;
  if (by_tick) {
    Buffers_CollectMirrorWrites();
  }
}

std::string Passes::TakeMirrorByTick() {
  State& s = *state_;
  if (!s.mirror_by_tick) {
    return {};
  }
  State::MirrorByTickStats& b = s.by_tick;
  std::string text = fmt::format(
      "{} frames, {} swaps; {} writes applied ({:.1f} MB); drawn ranges missing a write: {} in "
      "logic passes' lists ({:.2f} MB), {} in render-only passes' ({:.2f} MB)",
      b.frames, b.swaps, b.writes, b.bytes / 1048576.0, b.stale[0], b.stale_bytes[0] / 1048576.0,
      b.stale[1], b.stale_bytes[1] / 1048576.0);
  b.frames = b.swaps = b.writes = b.bytes = 0;
  b.stale[0] = b.stale[1] = b.stale_bytes[0] = b.stale_bytes[1] = 0;
  return text;
}

void Passes::set_verify_mirror(bool verify) {
  state_->verify_mirror = verify;
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
  double frames = double(std::max<uint64_t>(s.mirror_frames, 1));
  std::string text = fmt::format(
      "per frame {:.2f} MB copied with changed contents, {:.2f} MB of ranges new, vertex "
      "buffers {:.2f} MB copied but {:.2f} MB within the draws' vertices; {} of {} ranges "
      "changed ({:.1f} MB)",
      s.mirror_changed_bytes / 1048576.0 / frames, s.mirror_new_bytes / 1048576.0 / frames,
      s.mirror_vertex_bytes / 1048576.0 / frames, s.used_vertex_bytes / 1048576.0 / frames,
      changed.size(), s.range_history.size(), bytes / 1048576.0);
  s.mirror_frames = s.mirror_changed_bytes = s.mirror_new_bytes = s.mirror_vertex_bytes =
      s.used_vertex_bytes = 0;
  for (size_t i = 0; i < changed.size() && i < 12; i++) {
    text += fmt::format("; {} {:08X} {} bytes x{}", changed[i].second.index ? "index" : "vertex",
                        uint32_t(changed[i].first >> 32), uint32_t(changed[i].first),
                        changed[i].second.changes);
  }
  return text;
}

namespace {

// The float constant registers a shader reads, in packed order.
std::vector<uint32_t> FloatRegisters(const TranslatedShader* shader) {
  std::vector<uint32_t> registers;
  for (uint32_t i = 0; shader && i < 256; i++) {
    if (shader->float_constants[i >> 6] & (uint64_t(1) << (i & 63))) {
      registers.push_back(i);
    }
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
        text += fmt::format(" c{}.{} {:.4g}->{:.4g}", (*float_registers)[i / 4], "xyzw"[i % 4], a,
                            b);
      } else {
        text += fmt::format(" word {} {:08X}->{:08X}", i, before[i], after[i]);
      }
    }
  }
  return changed ? fmt::format("; {} ({} words):{}", name, changed, text) : std::string();
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
  now.system.assign(system, system + sizeof(Translator::SystemConstants) / 4);
  std::vector<uint32_t> vertex_registers = FloatRegisters(vertex);
  std::vector<uint32_t> pixel_registers = FloatRegisters(pixel);
  now.vertex_floats.assign(vertex_floats, vertex_floats + 4 * vertex_registers.size());
  now.pixel_floats.assign(pixel_floats, pixel_floats + 4 * pixel_registers.size());
  now.bool_loop.assign(bool_loop, bool_loop + kBoolLoopConstantsRange / 4);
  // Only the fetch constants the shaders read (the space is shared: texture
  // slot n is vertex buffers 3n-3n+2); unused ones are zeroed.
  now.fetch.assign(kFetchConstantsRange / 4, 0);
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
        watch_lines.push_back(fmt::format("vs {:016X} ps {:016X} indices {:08X} x{} #{}{}",
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
    text += fmt::format(" ps {:016X} param gen in {:04X};", hash, interpolators);
  }
  for (const auto& [hash, masks] : s.interpolators_by_shader) {
    text += fmt::format(" ps {:016X} interpolators {:08X};", hash, masks);
  }
  s.param_gen_by_shader.clear();
  s.interpolators_by_shader.clear();
  return text;
}

void Passes::set_no_index_clamp(bool off) {
  state_->no_index_clamp = off;
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

void Passes::set_check_late_writes(bool check) {
  state_->check_late_writes = check;
}

std::string Passes::TakeLateWrites() {
  State& s = *state_;
  std::string text = fmt::format("{} of {} copied ranges changed by the end of the frame",
                                 s.late_written, s.late_checked);
  if (!s.late_sample.empty()) {
    text += ", e.g. " + s.late_sample + "; by vertex shader:";
    for (const auto& [shader, count] : s.late_by_shader) {
      text += fmt::format(" {:016X} x{}", shader, count);
    }
  }
  s.late_written = s.late_checked = 0;
  s.late_by_shader.clear();
  s.late_sample.clear();
  return text;
}

void Passes::set_check_clobbers(bool check) {
  state_->check_clobbers = check;
}

std::vector<std::string> Passes::TakeDeclarationLines() {
  return std::exchange(state_->declaration_lines, {});
}

std::string Passes::TakeClobbers() {
  State& s = *state_;
  std::string text =
      fmt::format("{} of {} copies changed bytes an earlier copy this frame put in the mirror "
                  "({} bytes)",
                  s.clobbers, s.clobber_copies, s.clobbered_bytes);
  if (!s.clobber_sample.empty()) {
    text += ", e.g. " + s.clobber_sample + "; by shaders (earlier vs/ps, then vs/ps):";
    std::vector<std::pair<uint64_t, std::array<uint64_t, 4>>> pairs;
    for (const auto& [shaders, count] : s.clobbers_by_shaders) {
      pairs.push_back({count, shaders});
    }
    std::sort(pairs.rbegin(), pairs.rend());
    for (size_t i = 0; i < std::min<size_t>(pairs.size(), 12); i++) {
      const auto& [count, k] = pairs[i];
      text += fmt::format(" {:08X}/{:08X} by {:08X}/{:08X} x{};", k[0] >> 32, k[1] >> 32,
                          k[2] >> 32, k[3] >> 32, count);
    }
  }
  s.clobbers = s.clobbered_bytes = s.clobber_copies = 0;
  s.clobbers_by_shaders.clear();
  s.clobber_sample.clear();
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
  if (Buffers_LogPhysicalWrites()) {
    Buffers_OnFrameEnd();
  }
  s.last_frame_resolves = s.frame_resolves.size();
  if (s.tick_capturing) {
    s.tick_counts.captured += s.tick_capture.size();
    s.tick_list.swap(s.tick_capture);
  }
  if (s.log_tick_repeats) {
    s.CompareTickRepeats();
  }
  if (s.log_glow) {
    s.LogGlow();
  }
  if (s.check_late_writes && s.last_base) {
    uint8_t* base = s.last_base;
    for (const State::CopiedRange& r : s.copied_ranges) {
      s.late_checked++;
      if (XXH3_64bits(PhysicalHostAddress(base, r.address), r.size) != r.hash) {
        s.late_written++;
        s.late_by_shader[r.vertex_shader]++;
        if (s.late_sample.empty()) {
          s.late_sample = fmt::format("{} {:08X} ({} bytes) of vs {:016X}",
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
      list += fmt::format("{}{}: {:08X} format {} {}x{} flags {:X} after {} draws from {}x{} x{}",
                          i ? "; " : "", i, r.address, r.format, r.extent.width,
                          r.extent.height, r.flags, r.draws, r.source.width, r.source.height,
                          uint32_t(r.samples));
    }
    REXLOG_INFO("native renderer: this frame's resolves: {}; then {} draws ({} in all)", list,
                s.frame_draws - s.draws_at_resolve, s.frame_draws);
  }
  VkImage shown = s.shown.image;
  Transition(commands, shown, VK_IMAGE_ASPECT_COLOR_BIT, s.shown_layout,
             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  VkExtent2D extent = s.main_view_extent;
  if (show_resolve >= 0 && size_t(show_resolve) < s.frame_resolves.size()) {
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
    VkClearColorValue colour{{0.0f, 0.05f, 0.15f, 1.0f}};
    VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vk::vkCmdClearColorImage(commands, shown, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &colour, 1,
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
    if (dump_resolves && s.last_base) {
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
  for (size_t i = 0; with_resolves && i < frame_resolves.size(); i++) {
    const Resolved& r = frame_resolves[i];
    add(fmt::format("resolve_{:02}_{:08X}_format{}", i, r.address, r.format), r.host_format,
        r.extent);
    images.push_back(r.image);
  }
  size_t resolve_end = images.size();
  if (id_drawn) {
    add("draw_ids", VK_FORMAT_R32_UINT, main_view_extent);
    images.push_back(id_image.image);
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
    if (resolve) {
      Transition(commands, image, VK_IMAGE_ASPECT_COLOR_BIT, layout,
                 VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
    }
    VkBufferImageCopy copy{};
    copy.bufferOffset = dumped[i].offset;
    copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
    copy.imageExtent = {dumped[i].extent.width, dumped[i].extent.height, 1};
    vk::vkCmdCopyImageToBuffer(commands, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dump_buffer,
                               1, &copy);
    if (resolve) {
      Transition(commands, image, VK_IMAGE_ASPECT_COLOR_BIT, layout,
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
    if (!TextureCache::DecodeGuest(last_base, r.fetch, d.cpu, d.format, width, height)) {
      continue;
    }
    d.name = fmt::format("guest_resolve_{:02}_{:08X}_format{}", i, r.address, r.format);
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
      REXLOG_ERROR("native renderer: couldn't write {}", path);
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
  REXLOG_INFO("native renderer: dumped {} images into {}", s.dumped.size(), directory);
  if (!s.recorded_draws.empty()) {
    if (FILE* file = std::fopen((directory + "/draws.txt").c_str(), "w")) {
      for (size_t n = 0; n < s.recorded_draws.size(); n++) {
        const State::RecordedDraw& r = s.recorded_draws[n];
        std::string line = fmt::format(
            "draw {}: {} target pitch {} MSAA {} format {}, colour {}, depth {}; vs {:016X} ps "
            "{:016X}; primitive {} x{}{}",
            n, r.main_view ? "main view" : "other", r.surface.pitch, r.surface.msaa,
            r.surface.format, r.has_color, r.has_depth, r.vertex, r.pixel, r.type, r.count,
            r.inline_vertices ? " inline" : "");
        const auto& rs = r.render_state;
        float blend[4], alpha_ref;
        std::memcpy(blend, &rs[5], 16);
        std::memcpy(&alpha_ref, &rs[9], 4);
        line += fmt::format(
            "; depthcontrol {:08X} blendcontrol {:08X} colorcontrol {:08X} modecontrol {:08X} "
            "color mask {:08X} blend constant {:.3g} {:.3g} {:.3g} {:.3g} alpha ref {:.3g}",
            rs[0], rs[1], rs[2], rs[3], rs[4], blend[0], blend[1], blend[2], blend[3], alpha_ref);
        for (const auto& v : r.vertex_fetches) {
          line += fmt::format("; vfetch {}: {:08X} {:08X} (address {:08X}, {} bytes)", v[0], v[1],
                              v[2], v[1] & 0x1FFFFFFCu, ((v[2] >> 2) & 0xFFFFFFu) * 4);
        }
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
  stats.shaders_from_disk = state_->shaders->from_disk();
  uint32_t pipelines = state_->stats.pipelines, targets = state_->stats.targets;
  state_->stats = {};
  state_->stats.pipelines = pipelines;
  state_->stats.targets = targets;
  return stats;
}

}  // namespace native
