#include "textures.h"
#include "frame_costs.h"

#include "buffers.h"
#include "state.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <bit>
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <rex/graphics/xenos.h>
#include <rex/cvar.h>
#include <rex/hash.h>
#include <rex/logging.h>

#if defined(__SSSE3__)
#include <tmmintrin.h>
#endif

REXCVAR_DEFINE_BOOL(native_renderer_log_texture_writes, false, "ITC",
                    "Log which code writes textures' memory: on first load and when a texture "
                    "is found changed, what the library's memcpy wrote there (a debugging aid; "
                    "with --native_renderer_verify_textures every change is found)");

namespace native {

namespace xenos = rex::graphics::xenos;

namespace {

// Per frame in flight: texture data on its way into images. A frame that
// needs more defers the rest to the next frames (placeholders meanwhile).
constexpr VkDeviceSize kStagingSize = 64u << 20;

uint32_t Log2Ceil(uint32_t v) {
  return v <= 1 ? 0 : 32 - std::countl_zero(v - 1);
}
uint32_t NextPow2(uint32_t v) {
  return 1u << Log2Ceil(v);
}
uint32_t Align(uint32_t v, uint32_t a) {
  return (v + a - 1) / a * a;
}
uint32_t DivideUp(uint32_t v, uint32_t d) {
  return (v + d - 1) / d;
}

enum class Conversion { kNone, k4444To8888 };

// A guest format as stored (blocks) and the host format it's loaded as.
struct Format {
  VkFormat format;
  uint32_t block_width, block_height, bytes_per_block;
  // Components the format has; the last is repeated into the missing ones
  // before the fetch constant's swizzle.
  uint32_t components;
  Conversion conversion = Conversion::kNone;
};

// The formats this game's textures use (see "Textures in use" in
// docs/gpu-functions.md), and some neighbours. Packed formats store X in the
// lowest bits, as Vulkan's PACK formats named from A down to R do.
bool GetFormat(xenos::TextureFormat guest, Format& out) {
  using F = xenos::TextureFormat;
  switch (guest) {
    case F::k_8:
      out = {VK_FORMAT_R8_UNORM, 1, 1, 1, 1};
      return true;
    case F::k_8_8:
      out = {VK_FORMAT_R8G8_UNORM, 1, 1, 2, 2};
      return true;
    case F::k_8_8_8_8:
    case F::k_8_8_8_8_AS_16_16_16_16:
      out = {VK_FORMAT_R8G8B8A8_UNORM, 1, 1, 4, 4};
      return true;
    case F::k_2_10_10_10:
    case F::k_2_10_10_10_AS_16_16_16_16:
      out = {VK_FORMAT_A2B10G10R10_UNORM_PACK32, 1, 1, 4, 4};
      return true;
    case F::k_4_4_4_4:
      out = {VK_FORMAT_R8G8B8A8_UNORM, 1, 1, 2, 4, Conversion::k4444To8888};
      return true;
    case F::k_DXT1:
    case F::k_DXT1_AS_16_16_16_16:
      out = {VK_FORMAT_BC1_RGBA_UNORM_BLOCK, 4, 4, 8, 4};
      return true;
    case F::k_DXT2_3:
    case F::k_DXT2_3_AS_16_16_16_16:
      out = {VK_FORMAT_BC2_UNORM_BLOCK, 4, 4, 16, 4};
      return true;
    case F::k_DXT4_5:
    case F::k_DXT4_5_AS_16_16_16_16:
      out = {VK_FORMAT_BC3_UNORM_BLOCK, 4, 4, 16, 4};
      return true;
    case F::k_DXN:
      out = {VK_FORMAT_BC5_UNORM_BLOCK, 4, 4, 16, 2};
      return true;
    case F::k_16:
      out = {VK_FORMAT_R16_UNORM, 1, 1, 2, 1};
      return true;
    case F::k_16_16:
      out = {VK_FORMAT_R16G16_UNORM, 1, 1, 4, 2};
      return true;
    case F::k_16_16_16_16:
      out = {VK_FORMAT_R16G16B16A16_UNORM, 1, 1, 8, 4};
      return true;
    case F::k_16_FLOAT:
      out = {VK_FORMAT_R16_SFLOAT, 1, 1, 2, 1};
      return true;
    case F::k_16_16_FLOAT:
      out = {VK_FORMAT_R16G16_SFLOAT, 1, 1, 4, 2};
      return true;
    case F::k_16_16_16_16_FLOAT:
      out = {VK_FORMAT_R16G16B16A16_SFLOAT, 1, 1, 8, 4};
      return true;
    case F::k_32_FLOAT:
      out = {VK_FORMAT_R32_SFLOAT, 1, 1, 4, 1};
      return true;
    case F::k_32_32_FLOAT:
      out = {VK_FORMAT_R32G32_SFLOAT, 1, 1, 8, 2};
      return true;
    case F::k_32_32_32_32_FLOAT:
      out = {VK_FORMAT_R32G32B32A32_SFLOAT, 1, 1, 16, 4};
      return true;
    default:
      return false;  // depth (24_8), YUV, 10_11_11, 5_6_5 and others: not yet
  }
}

xenos::xe_gpu_texture_fetch_t ToFetch(const uint32_t words[6]) {
  xenos::xe_gpu_texture_fetch_t fetch;
  std::memcpy(&fetch, words, sizeof(fetch));
  return fetch;
}

// A 2D (or stacked) or cube texture's size and where its levels are, from
// the fetch constant (as the SDK's GetSubresourcesFromFetchConstant reads
// it).
struct Layout {
  uint32_t width, height, layers;
  uint32_t base_address, mip_address;  // physical; 0 when absent
  uint32_t min_level, max_level;
  uint32_t pitch_texels;               // the base level's row pitch
  uint32_t packed_level = UINT32_MAX;  // the first level in the mip tail
  bool tiled;
  bool cube;
};

bool GetLayout(const xenos::xe_gpu_texture_fetch_t& fetch, Layout& out) {
  switch (fetch.dimension) {
    case xenos::DataDimension::k2DOrStacked:
      out.layers = fetch.stacked ? fetch.size_2d.stack_depth + 1 : 1;
      out.cube = false;
      break;
    case xenos::DataDimension::kCube:
      out.layers = 6;
      out.cube = true;
      break;
    default:
      return false;  // 1D and 3D: not yet
  }
  out.width = fetch.size_2d.width + 1;
  out.height = fetch.size_2d.height + 1;
  out.tiled = fetch.tiled;
  out.pitch_texels = fetch.pitch << 5;
  uint32_t size_max_level = 31 - std::countl_zero(std::max(out.width, out.height));
  uint32_t base_page = fetch.base_address & 0x1FFFF;
  uint32_t mip_page = fetch.mip_address & 0x1FFFF;
  uint32_t min_level = 0, max_level = 0;
  if (mip_page) {
    min_level = std::min(uint32_t(fetch.mip_min_level), size_max_level);
    max_level = std::max(std::min(uint32_t(fetch.mip_max_level), size_max_level), min_level);
  }
  if (max_level) {
    if (!base_page) {
      min_level = std::max(min_level, 1u);
    }
    if (min_level) {
      base_page = 0;
    }
  } else {
    mip_page = 0;
  }
  if (!base_page && !mip_page) {
    return false;
  }
  out.base_address = base_page << 12;
  out.mip_address = mip_page << 12;
  out.min_level = min_level;
  out.max_level = max_level;
  if (fetch.packed_mips) {
    uint32_t log2_size = Log2Ceil(std::min(out.width, out.height));
    out.packed_level = log2_size > 4 ? log2_size - 4 : 0;
  }
  return true;
}

// Where a level packed into the mip tail sits in the tail's 32x32 tile, in
// blocks (the SDK's GetPackedMipOffset, for 2D): the tail of a wide texture
// is laid out downwards then rightwards, of a tall or square one the other
// way round.
void PackedMipOffset(const Layout& layout, const Format& format, uint32_t level, uint32_t& x,
                     uint32_t& y) {
  uint32_t log2_width = Log2Ceil(layout.width), log2_height = Log2Ceil(layout.height);
  uint32_t log2_size = std::min(log2_width, log2_height);
  uint32_t packed_base = log2_size > 4 ? log2_size - 4 : 0;
  uint32_t packed = level - packed_base;
  bool wide = log2_width > log2_height;
  x = y = 0;
  if (packed < 3) {
    (wide ? y : x) = 16 >> packed;
  } else {
    uint32_t offset = (1u << ((wide ? log2_width : log2_height) - packed_base)) >> (packed - 2);
    (wide ? x : y) = offset;
  }
  x /= format.block_width;
  y /= format.block_height;
}

// The 360's tiled layout of 2D textures: offset in bytes of block (x, y),
// pitch in blocks (the SDK's GetTiledOffset2D).
uint32_t TiledOffset2D(uint32_t x, uint32_t y, uint32_t pitch, uint32_t bytes_per_block_log2) {
  pitch = Align(pitch, 32);
  uint32_t macro = ((x >> 5) + (y >> 5) * (pitch >> 5)) << (bytes_per_block_log2 + 7);
  uint32_t micro = ((x & 7) + ((y & 0xE) << 2)) << bytes_per_block_log2;
  uint32_t offset = macro + ((micro & ~0xFu) << 1) + (micro & 0xF) + ((y & 1) << 4);
  return ((offset & ~0x1FFu) << 3) + ((y & 16) << 7) + ((offset & 0x1C0) << 2) +
         (((((y & 8) >> 2) + (x >> 3)) & 3) << 6) + (offset & 0x3F);
}

// Where a level's data is stored. The base has the fetch constant's pitch;
// mips are padded as levels of a power-of-two texture; every level's rows
// and row count are padded to 32 blocks, array layers to 4 KB. Mips follow
// each other from the mip address, and the levels from the packed level on
// share the packed level's storage, each at its offset in the tail.
struct Placement {
  uint32_t address;
  uint32_t pitch_blocks, row_pitch_bytes, layer_stride;
  uint32_t x, y;  // offset in blocks
};

Placement Place(const Layout& layout, const Format& format, uint32_t level) {
  Placement p{};
  auto strides = [&](uint32_t stored, bool base) {
    uint32_t pitch_texels =
        base ? layout.pitch_texels : std::max(NextPow2(layout.width) >> stored, 1u);
    uint32_t rows_texels = base ? layout.height : std::max(NextPow2(layout.height) >> stored, 1u);
    p.pitch_blocks = Align(DivideUp(pitch_texels, format.block_width), 32);
    p.row_pitch_bytes = p.pitch_blocks * format.bytes_per_block;
    if (!layout.tiled && !base) {
      p.row_pitch_bytes = Align(p.row_pitch_bytes, 256);
    }
    uint32_t rows = Align(DivideUp(rows_texels, format.block_height), 32);
    p.layer_stride = Align(p.row_pitch_bytes * rows, 4096);
  };
  uint32_t stored = std::min(level, layout.packed_level);
  if (level == 0) {
    p.address = layout.base_address;
    strides(0, true);
  } else {
    p.address = layout.mip_address;
    for (uint32_t s = 1; s < stored; s++) {
      strides(s, false);
      p.address += p.layer_stride * layout.layers;
    }
    strides(stored, false);
  }
  if (level >= layout.packed_level) {
    PackedMipOffset(layout, format, level, p.x, p.y);
  }
  return p;
}

// Where a level's data ends (physical): a tiled level fills whole 32x32
// block tiles, so its last layer's storage; an untiled one ends with its
// last row's data, as the padding after it can hold other data (an untiled
// 300x150 texture's padding rows changed every tick, 2026-10-15).
uint32_t LevelEnd(const Layout& layout, const Format& format, uint32_t level) {
  Placement p = Place(layout, format, level);
  uint32_t last_layer = p.address + (layout.layers - 1) * p.layer_stride;
  if (layout.tiled) {
    return last_layer + p.layer_stride;
  }
  uint32_t width_blocks = DivideUp(std::max(layout.width >> level, 1u), format.block_width);
  uint32_t height_blocks = DivideUp(std::max(layout.height >> level, 1u), format.block_height);
  return last_layer + (p.y + height_blocks - 1) * p.row_pitch_bytes +
         (p.x + width_blocks) * format.bytes_per_block;
}

void SwapEndian(uint8_t* data, size_t size, xenos::Endian endian) {
  size_t i = 0;
#if defined(__SSSE3__)
  // 16 bytes a shuffle; the scalar loops below finish the tail.
  __m128i shuffle;
  switch (endian) {
    case xenos::Endian::k8in16:
      shuffle = _mm_setr_epi8(1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14);
      break;
    case xenos::Endian::k8in32:
      shuffle = _mm_setr_epi8(3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
      break;
    case xenos::Endian::k16in32:
      shuffle = _mm_setr_epi8(2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13);
      break;
    default:
      return;
  }
  for (; i + 16 <= size; i += 16) {
    __m128i v = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + i));
    _mm_storeu_si128(reinterpret_cast<__m128i*>(data + i), _mm_shuffle_epi8(v, shuffle));
  }
#endif
  switch (endian) {
    case xenos::Endian::k8in16:
      for (; i + 2 <= size; i += 2) {
        std::swap(data[i], data[i + 1]);
      }
      break;
    case xenos::Endian::k8in32:
      for (; i + 4 <= size; i += 4) {
        std::swap(data[i], data[i + 3]);
        std::swap(data[i + 1], data[i + 2]);
      }
      break;
    case xenos::Endian::k16in32:
      for (; i + 4 <= size; i += 4) {
        std::swap(data[i], data[i + 2]);
        std::swap(data[i + 1], data[i + 3]);
      }
      break;
    default:
      break;
  }
}

// A row of a tiled level, gathered: blocks [x, x + count) of row y. The
// tiled layout keeps runs of 16 bytes contiguous (8 for 1-byte blocks: the
// micro tile's 8 columns), aligned in x, so whole runs are copied at once
// rather than block by block.
void GatherTiledRow(uint8_t* out, const uint8_t* base, uint32_t layer_address, uint32_t x,
                    uint32_t y, uint32_t count, uint32_t pitch_blocks,
                    uint32_t bytes_per_block_log2) {
  uint32_t run_blocks_log2 = bytes_per_block_log2 ? 4 - std::min(bytes_per_block_log2, 4u) : 3;
  uint32_t run_blocks = 1u << run_blocks_log2;
  uint32_t end = x + count;
  while (x < end) {
    uint32_t address =
        layer_address + TiledOffset2D(x, y, pitch_blocks, bytes_per_block_log2);
    uint32_t n = std::min(run_blocks - (x & (run_blocks - 1)), end - x);
    uint32_t bytes = n << bytes_per_block_log2;
    if (address + bytes <= kPhysicalMemorySize) {
      const uint8_t* in = PhysicalHostAddress(const_cast<uint8_t*>(base), address);
      if (bytes == 16) {
        std::memcpy(out, in, 16);
      } else if (bytes == 8) {
        std::memcpy(out, in, 8);
      } else {
        std::memcpy(out, in, bytes);
      }
    }
    out += bytes;
    x += n;
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

// What identifies a texture's data in the fetch constant: pitch and tiling;
// format, endianness, stacking and base address; size; mip levels;
// dimension, packing and mip address. Not the swizzle, signs or sampling.
using TextureKey = std::array<uint32_t, 6>;
TextureKey KeyOf(const uint32_t fetch[6]) {
  return {fetch[0] & 0xFFC00000u, fetch[1] & 0xFFFFF4FFu, fetch[2], 0, fetch[4] & 0x3FCu,
          fetch[5] & 0xFFFFFE00u};
}

}  // namespace

struct TextureCache::State {
  uint64_t mismatched_total = 0;
  DeviceContext context{};
  VkPhysicalDeviceMemoryProperties memory_properties{};

  struct Frame {
    VkBuffer staging = VK_NULL_HANDLE;
    VkDeviceMemory staging_memory = VK_NULL_HANDLE;
    uint8_t* staging_mapped = nullptr;
    VkCommandPool pool = VK_NULL_HANDLE;
    VkCommandBuffer uploads = VK_NULL_HANDLE;
  };
  std::vector<Frame> frames;
  uint32_t frame = 0;
  uint64_t frame_number = 0;
  VkDeviceSize staging_used = 0;
  bool recording = false;

  struct Texture {
    Layout layout{};
    Format format{};
    xenos::Endian endian = xenos::Endian::kNone;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    uint32_t levels = 0;
    bool loaded = false;
    // The library's memcpy wrote its memory since it was loaded: reload.
    bool changed = false;
    // With verify_every_frame_: its memory's hash when loaded or last
    // checked, and when that was.
    uint64_t hash = 0;
    uint64_t verified_frame = 0;
    uint64_t loaded_frame = 0;
    // The guest memory it's read from: base and mips (address, size).
    std::array<std::pair<uint32_t, uint32_t>, 2> ranges{};
    std::unordered_map<uint32_t, VkImageView> views;  // by swizzle
    // A resolve destination: drawn into, never loaded.
    bool resolved = false;
    uint32_t guest_format = 0;
    VkImageView attachment_view = VK_NULL_HANDLE;
  };
  std::map<TextureKey, Texture> textures;
  // Loaded textures' ranges by start address (with the longest range's
  // size, to find those overlapping a write), and memcpy's writes taken
  // this frame.
  std::multimap<uint32_t, std::pair<uint32_t, Texture*>> by_address;
  uint32_t longest_range = 0;
  std::vector<std::pair<uint32_t, uint32_t>> new_writes;
  void MarkWritten();
  // Resolve destinations by base address (page).
  std::unordered_map<uint32_t, Texture> resolved;
  std::unordered_map<uint64_t, VkSampler> samplers;
  Stats stats;

  // --native_renderer_log_texture_writes: who wrote textures' memory.
  struct WriteStats {
    uint64_t first = 0, first_bytes = 0, covered = 0, partly = 0, uncovered = 0,
             uncovered_bytes = 0;
    std::map<uint32_t, uint64_t> first_callers;
    // Textures changed without a recorded write (found by verify_every_frame_):
    // address: changes, and a description.
    std::map<uint32_t, std::pair<uint64_t, std::string>> unrecorded_by;
  } writes;
  // Textures changed without a recorded write: their memory when last
  // checked, to log which bytes changed.
  std::unordered_map<uint32_t, std::vector<uint8_t>> write_snapshots;
  uint32_t write_lines = 200;
  std::chrono::steady_clock::time_point writes_logged = std::chrono::steady_clock::now();
  void LogFirstLoad(const Texture& t);
  void LogUnrecordedChange(uint8_t* base, const Texture& t);
  void LogWriteSummary();

  bool CreateTexture(Texture& texture);
  uint64_t Hash(uint8_t* base, const Texture& texture) const;
  VkImageView GetSwizzledView(Texture& texture, uint32_t swizzle, VkImageViewType natural);
  bool Upload(uint8_t* base, Texture& texture);
  VkCommandBuffer Uploads();
};

std::string WriteCallers(const std::map<uint32_t, uint64_t>& callers) {
  std::string text;
  for (auto& [caller, count] : callers) {
    text += fmt::format(" {:08X}={}", caller, count);
  }
  return text.empty() ? " none" : text;
}

// A texture loaded for the first time: what the library's memcpy wrote
// into its memory.
void TextureCache::State::LogFirstLoad(const Texture& t) {
  uint64_t size = 0, bytes = 0;
  uint32_t first_caller = 0;
  for (const auto& [address, range_size] : t.ranges) {
    if (!range_size) {
      continue;
    }
    Buffers_Coverage c = Buffers_CoverageOf(address, range_size);
    size += range_size;
    bytes += c.bytes;
    if (c.bytes && !first_caller) {
      first_caller = c.first_caller;
    }
  }
  uint32_t address = t.layout.base_address ? t.layout.base_address : t.layout.mip_address;
  WriteStats& w = writes;
  w.first++;
  w.first_bytes += size;
  if (bytes >= size) {
    w.covered++;
  } else if (bytes) {
    w.partly++;
  } else {
    w.uncovered++;
    w.uncovered_bytes += size;
  }
  if (bytes) {
    w.first_callers[first_caller]++;
  }
  if (bytes < size && write_lines) {
    write_lines--;
    REXLOG_INFO("native renderer: texture writes: {:08X} ({}x{}, {} bytes) first loaded with "
                "{} bytes written by memcpy{}",
                address, t.layout.width, t.layout.height, size, bytes,
                bytes ? fmt::format(" (first from {:08X})", first_caller) : std::string());
  }
}

// A texture whose memory changed with no write recorded: which bytes of its
// first range changed since the last such change.
void TextureCache::State::LogUnrecordedChange(uint8_t* base, const Texture& t) {
  uint32_t address = t.layout.base_address ? t.layout.base_address : t.layout.mip_address;
  auto& [count, text] = writes.unrecorded_by[address];
  if (!count++) {
    text = fmt::format("{}x{} format {} levels {}-{}", t.layout.width, t.layout.height,
                       int(t.format.format), t.layout.min_level, t.layout.max_level);
  }
  auto [start, range_size] = t.ranges[0].second ? t.ranges[0] : t.ranges[1];
  const uint8_t* now = PhysicalHostAddress(base, start);
  std::vector<uint8_t>& was = write_snapshots[address];
  if (was.size() == range_size && write_lines) {
    uint32_t first = UINT32_MAX, last = 0, changed = 0;
    for (uint32_t i = 0; i < range_size; i++) {
      if (now[i] != was[i]) {
        first = std::min(first, i);
        last = i;
        changed++;
      }
    }
    write_lines--;
    REXLOG_INFO("native renderer: texture writes: {:08X}: {} bytes changed, offsets {}-{}",
                address, changed, first, last);
  }
  was.assign(now, now + range_size);
}

void TextureCache::State::LogWriteSummary() {
  auto now = std::chrono::steady_clock::now();
  if (now - writes_logged < std::chrono::seconds(5)) {
    return;
  }
  writes_logged = now;
  WriteStats& w = writes;
  REXLOG_INFO("native renderer: texture writes: {} first loads ({:.1f} MB): {} written wholly by "
              "memcpy, {} partly, {} not at all ({:.1f} MB); by first writer:{}",
              w.first, w.first_bytes / 1048576.0, w.covered, w.partly, w.uncovered,
              w.uncovered_bytes / 1048576.0, WriteCallers(w.first_callers));
  for (auto& [address, entry] : w.unrecorded_by) {
    REXLOG_INFO("native renderer: texture writes: {:08X} ({}) changed {} times without a "
                "recorded write",
                address, entry.second, entry.first);
  }
  w = WriteStats{};
}

bool TextureCache::State::CreateTexture(Texture& t) {
  VkDevice device = context.device;
  const Layout& l = t.layout;
  t.levels = t.resolved ? 1 : l.max_level + 1;
  VkImageCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  info.flags = l.cube ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
  info.imageType = VK_IMAGE_TYPE_2D;
  info.format = t.format.format;
  info.extent = {l.width, l.height, 1};
  info.mipLevels = t.levels;
  info.arrayLayers = l.layers;
  info.samples = VK_SAMPLE_COUNT_1_BIT;
  info.tiling = VK_IMAGE_TILING_OPTIMAL;
  info.usage = t.resolved ? VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                                VK_IMAGE_USAGE_TRANSFER_SRC_BIT
                          : VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (VkResult result = vk::vkCreateImage(device, &info, nullptr, &t.image);
      result != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: texture {}x{} format {}: vkCreateImage: {}", l.width, l.height,
                 int(t.format.format), int(result));
    t.image = VK_NULL_HANDLE;
    return false;
  }
  VkMemoryRequirements requirements;
  vk::vkGetImageMemoryRequirements(device, t.image, &requirements);
  VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  alloc.allocationSize = requirements.size;
  alloc.memoryTypeIndex = FindMemoryType(memory_properties, requirements.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (alloc.memoryTypeIndex == UINT32_MAX ||
      vk::vkAllocateMemory(device, &alloc, nullptr, &t.memory) != VK_SUCCESS ||
      vk::vkBindImageMemory(device, t.image, t.memory, 0) != VK_SUCCESS) {
    REXLOG_ERROR("native renderer: no memory for a {}x{} texture", l.width, l.height);
    vk::vkDestroyImage(device, t.image, nullptr);
    t.image = VK_NULL_HANDLE;
    return false;
  }

  if (t.resolved) {
    VkImageViewCreateInfo view{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view.image = t.image;
    view.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view.format = t.format.format;
    view.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    if (vk::vkCreateImageView(device, &view, nullptr, &t.attachment_view) != VK_SUCCESS) {
      return false;
    }
    stats.textures++;
    return true;
  }

  // The memory it's read from: the base level, and the mips up to the end of
  // the furthest one's data.
  const Layout& layout = t.layout;
  if (layout.base_address) {
    t.ranges[0] = {layout.base_address, LevelEnd(layout, t.format, 0) - layout.base_address};
  }
  if (layout.mip_address && layout.max_level) {
    uint32_t end = layout.mip_address;
    for (uint32_t level = 1; level <= layout.max_level; level++) {
      end = std::max(end, LevelEnd(layout, t.format, level));
    }
    t.ranges[1] = {layout.mip_address, end - layout.mip_address};
  }
  for (auto& [address, size] : t.ranges) {
    if (address >= kPhysicalMemorySize) {
      size = 0;
    } else {
      size = std::min(size, kPhysicalMemorySize - address);
    }
  }
  stats.textures++;
  return true;
}

uint64_t TextureCache::State::Hash(uint8_t* base, const Texture& t) const {
  ScopedCost cost(g_frame_costs.hash_ns);
  uint64_t hash = 0;
  for (const auto& [address, size] : t.ranges) {
    if (size) {
      hash = XXH3_64bits_withSeed(PhysicalHostAddress(base, address), size, hash);
    }
  }
  return hash;
}

VkCommandBuffer TextureCache::State::Uploads() {
  Frame& f = frames[frame];
  if (!recording) {
    VkCommandBufferBeginInfo begin{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    begin.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vk::vkBeginCommandBuffer(f.uploads, &begin);
    recording = true;
  }
  return f.uploads;
}

bool TextureCache::State::Upload(uint8_t* base, Texture& t) {
  const Layout& l = t.layout;
  const Format& f = t.format;
  uint32_t out_bytes_per_block = f.conversion == Conversion::k4444To8888 ? 4 : f.bytes_per_block;
  uint32_t bytes_per_block_log2 = std::countr_zero(f.bytes_per_block);
  uint32_t first_level = l.base_address ? 0 : 1;

  // The whole texture's size in staging, to fit it in one go.
  VkDeviceSize needed = 0;
  for (uint32_t level = first_level; level <= l.max_level; level++) {
    uint32_t w = DivideUp(std::max(l.width >> level, 1u), f.block_width);
    uint32_t h = DivideUp(std::max(l.height >> level, 1u), f.block_height);
    needed += Align(w * h * out_bytes_per_block, 16) * VkDeviceSize(l.layers);
  }
  if (staging_used + needed > kStagingSize) {
    stats.deferred++;
    return false;
  }
  ScopedCost cost(g_frame_costs.texture_ns);
  g_frame_costs.textures++;
  g_frame_costs.texture_bytes += needed;

  Frame& frame_data = frames[frame];
  std::vector<VkBufferImageCopy> copies;
  std::vector<uint8_t> row;
  for (uint32_t level = first_level; level <= l.max_level; level++) {
    Placement p = Place(l, f, level);
    uint32_t level_width = std::max(l.width >> level, 1u);
    uint32_t level_height = std::max(l.height >> level, 1u);
    uint32_t width_blocks = DivideUp(level_width, f.block_width);
    uint32_t height_blocks = DivideUp(level_height, f.block_height);
    uint32_t row_bytes = width_blocks * f.bytes_per_block;
    row.resize(row_bytes);
    for (uint32_t layer = 0; layer < l.layers; layer++) {
      uint32_t layer_address = p.address + layer * p.layer_stride;
      uint8_t* out = frame_data.staging_mapped + staging_used;
      for (uint32_t y = 0; y < height_blocks; y++) {
        // Gather a row of blocks, swap, then convert into staging.
        if (l.tiled) {
          GatherTiledRow(row.data(), base, layer_address, p.x, y + p.y, width_blocks,
                         p.pitch_blocks, bytes_per_block_log2);
        } else {
          uint32_t address =
              layer_address + (y + p.y) * p.row_pitch_bytes + p.x * f.bytes_per_block;
          if (address + row_bytes <= kPhysicalMemorySize) {
            std::memcpy(row.data(), PhysicalHostAddress(base, address), row_bytes);
          }
        }
        SwapEndian(row.data(), row_bytes, t.endian);
        uint8_t* out_row = out + size_t(y) * width_blocks * out_bytes_per_block;
        if (f.conversion == Conversion::k4444To8888) {
          for (uint32_t x = 0; x < width_blocks; x++) {
            uint16_t v;
            std::memcpy(&v, row.data() + 2 * x, 2);
            for (uint32_t c = 0; c < 4; c++) {
              out_row[4 * x + c] = uint8_t(((v >> (4 * c)) & 0xF) * 17);
            }
          }
        } else {
          std::memcpy(out_row, row.data(), row_bytes);
        }
      }
      VkBufferImageCopy copy{};
      copy.bufferOffset = staging_used;
      copy.bufferRowLength = width_blocks * f.block_width;
      copy.bufferImageHeight = height_blocks * f.block_height;
      copy.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, level, layer, 1};
      copy.imageExtent = {level_width, level_height, 1};
      copies.push_back(copy);
      staging_used += Align(width_blocks * height_blocks * out_bytes_per_block, 16);
    }
  }

  VkCommandBuffer commands = Uploads();
  VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.oldLayout = t.loaded ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_UNDEFINED;
  barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.srcQueueFamilyIndex = barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = t.image;
  barrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, t.levels, 0, l.layers};
  vk::vkCmdPipelineBarrier(commands,
                           t.loaded ? VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                                          VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
                                    : VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                           VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
  vk::vkCmdCopyBufferToImage(commands, frame_data.staging, t.image,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, uint32_t(copies.size()),
                             copies.data());
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  vk::vkCmdPipelineBarrier(commands, VK_PIPELINE_STAGE_TRANSFER_BIT,
                           VK_PIPELINE_STAGE_VERTEX_SHADER_BIT |
                               VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                           0, 0, nullptr, 0, nullptr, 1, &barrier);
  stats.loaded_bytes += needed;
  (t.loaded ? stats.reloaded : stats.loaded)++;
  t.loaded = true;
  return true;
}

TextureCache::TextureCache() : state_(std::make_unique<State>()) {}
TextureCache::~TextureCache() = default;

bool TextureCache::Init(const DeviceContext& context, uint32_t frames_in_flight) {
  State& s = *state_;
  s.context = context;
  VkDevice device = context.device;
  vk::vkGetPhysicalDeviceMemoryProperties(context.physical_device, &s.memory_properties);
  s.frames.resize(frames_in_flight);
  for (State::Frame& f : s.frames) {
    VkBufferCreateInfo info{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    info.size = kStagingSize;
    info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vk::vkCreateBuffer(device, &info, nullptr, &f.staging) != VK_SUCCESS) {
      return false;
    }
    VkMemoryRequirements requirements;
    vk::vkGetBufferMemoryRequirements(device, f.staging, &requirements);
    VkMemoryAllocateInfo alloc{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    alloc.allocationSize = requirements.size;
    alloc.memoryTypeIndex =
        FindMemoryType(s.memory_properties, requirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (alloc.memoryTypeIndex == UINT32_MAX ||
        vk::vkAllocateMemory(device, &alloc, nullptr, &f.staging_memory) != VK_SUCCESS ||
        vk::vkBindBufferMemory(device, f.staging, f.staging_memory, 0) != VK_SUCCESS ||
        vk::vkMapMemory(device, f.staging_memory, 0, VK_WHOLE_SIZE, 0,
                        reinterpret_cast<void**>(&f.staging_mapped)) != VK_SUCCESS) {
      return false;
    }
    VkCommandPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    pool_info.queueFamilyIndex = context.queue_family;
    if (vk::vkCreateCommandPool(device, &pool_info, nullptr, &f.pool) != VK_SUCCESS) {
      return false;
    }
    VkCommandBufferAllocateInfo command_alloc{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    command_alloc.commandPool = f.pool;
    command_alloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_alloc.commandBufferCount = 1;
    if (vk::vkAllocateCommandBuffers(device, &command_alloc, &f.uploads) != VK_SUCCESS) {
      return false;
    }
  }
  return true;
}

void TextureCache::BeginFrame(uint32_t frame_index) {
  State& s = *state_;
  s.frame = frame_index;
  s.frame_number++;
  s.staging_used = 0;
  s.recording = false;
  vk::vkResetCommandPool(s.context.device, s.frames[frame_index].pool, 0);
  s.MarkWritten();
}

// The textures memcpy has written since the last frame are marked to be
// reloaded (the game writes textures' memory with the library's memcpy:
// see docs/texture-handling-optimisation.md).
void TextureCache::State::MarkWritten() {
  new_writes.clear();
  Buffers_TakeNewWrites(new_writes);
  ScopedCost cost(g_frame_costs.mark_ns);
  stats.writes += new_writes.size();
  for (const auto& [start, end] : new_writes) {
    auto it = by_address.lower_bound(start > longest_range ? start - longest_range : 0);
    for (; it != by_address.end() && it->first < end; ++it) {
      auto& [size, texture] = it->second;
      if (it->first + size > start && !texture->changed) {
        texture->changed = true;
        stats.marked++;
      }
    }
  }
}

bool TextureCache::Holds(uint32_t start, uint32_t end) const {
  const State& s = *state_;
  auto it = s.by_address.lower_bound(start > s.longest_range ? start - s.longest_range : 0);
  for (; it != s.by_address.end() && it->first < end; ++it) {
    if (it->first + it->second.first > start) {
      return true;
    }
  }
  return false;
}

VkImageView TextureCache::GetView(uint8_t* base, const uint32_t fetch_words[6],
                                  VkImageViewType view_type) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  if (fetch.type != xenos::FetchConstantType::kTexture) {
    s.stats.unsupported++;
    return VK_NULL_HANDLE;
  }
  // A resolve destination at this address takes the place of guest memory.
  if (auto r = s.resolved.find(fetch.base_address & 0x1FFFF);
      r != s.resolved.end() && r->second.loaded) {
    State::Texture& t = r->second;
    s.stats.resolved_hits++;
    if (debug_hide_ == Hide::kResolved ||
        (debug_hide_ == Hide::kResolvedAddress && debug_hide_value_ == t.layout.base_address)) {
      return VK_NULL_HANDLE;
    }
    if (t.guest_format != uint32_t(fetch.format) || t.layout.width != fetch.size_2d.width + 1u ||
        t.layout.height != fetch.size_2d.height + 1u) {
      s.stats.resolved_mismatched++;
    }
    if (view_type != VK_IMAGE_VIEW_TYPE_2D_ARRAY) {
      s.stats.mismatched++;
    s.mismatched_total++;
      return VK_NULL_HANDLE;
    }
    return s.GetSwizzledView(t, fetch.swizzle, VK_IMAGE_VIEW_TYPE_2D_ARRAY);
  }

  if (debug_hide_ == Hide::kMemory ||
      (debug_hide_ == Hide::kMemoryFormat && debug_hide_value_ == uint32_t(fetch.format))) {
    return VK_NULL_HANDLE;
  }
  TextureKey key = KeyOf(fetch_words);
  auto it = s.textures.find(key);
  if (it == s.textures.end()) {
    State::Texture t;
    if (!GetLayout(fetch, t.layout) || !GetFormat(fetch.format, t.format)) {
      s.stats.unsupported++;
      if (s.stats.unsupported_sample == UINT32_MAX) {
        s.stats.unsupported_sample = uint32_t(fetch.format) << 8 | uint32_t(fetch.dimension);
      }
      return VK_NULL_HANDLE;
    }
    t.endian = fetch.endianness;
    if (!s.CreateTexture(t)) {
      return VK_NULL_HANDLE;
    }
    if (t.format.block_width == 1) {
      // Uncompressed: few, and the likeliest to be decoded wrong.
      REXLOG_INFO("native renderer: texture at {:08X}: format {} {}x{}x{} levels {}-{}, endian "
                  "{}, tiled {}, pitch {}, signs {:02X}, swizzle {:03o}, exp adjust {}",
                  t.layout.base_address ? t.layout.base_address : t.layout.mip_address,
                  uint32_t(fetch.format), t.layout.width, t.layout.height, t.layout.layers,
                  t.layout.min_level, t.layout.max_level, uint32_t(t.endian), t.layout.tiled,
                  t.layout.pitch_texels, (fetch.dword_0 >> 2) & 0xFF, uint32_t(fetch.swizzle),
                  int32_t(fetch.exp_adjust));
    }
    it = s.textures.emplace(key, std::move(t)).first;
    State::Texture& added = it->second;
    for (const auto& [address, size] : added.ranges) {
      if (size) {
        s.by_address.emplace(address, std::make_pair(size, &added));
        s.longest_range = std::max(s.longest_range, size);
      }
    }
  }
  State::Texture& t = it->second;
  VkImageViewType natural = t.layout.cube ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  if (view_type != natural) {
    s.stats.mismatched++;
    if (s.stats.mismatched_sample == UINT32_MAX) {
      s.stats.mismatched_sample = uint32_t(view_type) << 16 | uint32_t(fetch.format) << 8 |
                                  uint32_t(fetch.dimension);
      s.stats.mismatched_width = t.layout.width;
      s.stats.mismatched_height = t.layout.height;
    }
    return VK_NULL_HANDLE;
  }

  // Load; reload when memcpy has written its memory since (MarkWritten).
  // With verify_every_frame_, also hash it every frame, and reload it (and
  // log it) if it changed without a recorded write.
  bool log_writes = REXCVAR_GET(native_renderer_log_texture_writes);
  if (log_writes) {
    s.LogWriteSummary();
  }
  if (!t.loaded) {
    if (verify_every_frame_) {
      t.hash = s.Hash(base, t);
      t.verified_frame = s.frame_number;
    }
    if (!s.Upload(base, t)) {
      return VK_NULL_HANDLE;
    }
    t.changed = false;
    t.loaded_frame = s.frame_number;
    if (log_writes) {
      s.LogFirstLoad(t);
    }
  } else if (t.changed) {
    uint64_t hash = verify_every_frame_ ? s.Hash(base, t) : 0;
    if (s.Upload(base, t)) {  // else the old contents until it fits
      t.changed = false;
      t.hash = hash;
      t.verified_frame = s.frame_number;
    }
  } else if (verify_every_frame_ && t.verified_frame != s.frame_number) {
    t.verified_frame = s.frame_number;
    if (uint64_t hash = s.Hash(base, t); hash != t.hash) {
      s.stats.unrecorded++;
      if (s.stats.unrecorded_sample == UINT32_MAX) {
        s.stats.unrecorded_sample =
            t.layout.base_address ? t.layout.base_address : t.layout.mip_address;
        s.stats.unrecorded_frames = uint32_t(s.frame_number - t.loaded_frame);
      }
      if (log_writes) {
        s.LogUnrecordedChange(base, t);
      }
      if (s.Upload(base, t)) {
        t.hash = hash;
      }
    }
  }

  return s.GetSwizzledView(t, fetch.swizzle, natural);
}

VkImageView TextureCache::State::GetSwizzledView(Texture& t, uint32_t swizzle,
                                                 VkImageViewType natural) {
  VkImageView& view = t.views[swizzle];
  if (!view) {
    auto component = [&](uint32_t i) {
      uint32_t select = (swizzle >> (3 * i)) & 7;
      if (select == 4) {
        return VK_COMPONENT_SWIZZLE_ZERO;
      }
      if (select >= 5) {
        return VK_COMPONENT_SWIZZLE_ONE;
      }
      return VkComponentSwizzle(VK_COMPONENT_SWIZZLE_R +
                                std::min(select, t.format.components - 1));
    };
    VkImageViewCreateInfo info{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    info.image = t.image;
    info.viewType = natural;
    info.format = t.format.format;
    info.components = {component(0), component(1), component(2), component(3)};
    info.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, t.levels, 0, t.layout.layers};
    if (vk::vkCreateImageView(context.device, &info, nullptr, &view) != VK_SUCCESS) {
      view = VK_NULL_HANDLE;
    }
  }
  return view;
}

bool TextureCache::GetResolveDestination(const uint32_t fetch_words[6], ResolveDestination& out) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  uint32_t page = fetch.base_address & 0x1FFFF;
  uint32_t width = fetch.size_2d.width + 1, height = fetch.size_2d.height + 1;
  auto it = s.resolved.find(page);
  if (it != s.resolved.end() && (it->second.guest_format != uint32_t(fetch.format) ||
                                 it->second.layout.width != width ||
                                 it->second.layout.height != height)) {
    // Another texture at the same address: start over (the old image is
    // left behind, as frames in flight may still use it).
    REXLOG_WARN("native renderer: resolve destination at {:08X} changed from format {} {}x{} "
                "to format {} {}x{}",
                page << 12, it->second.guest_format, it->second.layout.width,
                it->second.layout.height, uint32_t(fetch.format), width, height);
    s.resolved.erase(it);
    it = s.resolved.end();
  }
  if (it == s.resolved.end()) {
    State::Texture t;
    t.resolved = true;
    t.guest_format = uint32_t(fetch.format);
    if (fetch.format == xenos::TextureFormat::k_24_8 ||
        fetch.format == xenos::TextureFormat::k_24_8_FLOAT) {
      t.format = {VK_FORMAT_R32_SFLOAT, 1, 1, 4, 1};  // depth, as the translator samples it
    } else if (!GetFormat(fetch.format, t.format) || t.format.block_width != 1 ||
               t.format.conversion != Conversion::kNone) {
      return false;
    }
    t.layout.width = width;
    t.layout.height = height;
    t.layout.layers = 1;
    t.layout.base_address = page << 12;
    if (!s.CreateTexture(t)) {
      return false;
    }
    REXLOG_INFO("native renderer: resolve destination at {:08X}: format {} {}x{} (host format {})",
                page << 12, t.guest_format, width, height, int(t.format.format));
    it = s.resolved.emplace(page, std::move(t)).first;
  }
  State::Texture& t = it->second;
  out = {t.image, t.attachment_view, t.format.format, width, height, !t.loaded};
  t.loaded = true;  // written by the caller
  return true;
}

uint64_t TextureCache::mismatched_total() const {
  return state_->mismatched_total;
}

bool TextureCache::DecodeGuest(uint8_t* base, const uint32_t fetch_words[6],
                               std::vector<uint8_t>& out, VkFormat& format, uint32_t& width,
                               uint32_t& height) {
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  Layout l;
  Format f;
  if (!GetLayout(fetch, l) || !l.base_address || !GetFormat(fetch.format, f) ||
      f.block_width != 1 || f.conversion != Conversion::kNone) {
    return false;
  }
  Placement p = Place(l, f, 0);
  uint32_t bytes_per_block_log2 = std::countr_zero(f.bytes_per_block);
  out.assign(size_t(l.width) * l.height * f.bytes_per_block, 0);
  for (uint32_t y = 0; y < l.height; y++) {
    uint8_t* row = out.data() + size_t(y) * l.width * f.bytes_per_block;
    for (uint32_t x = 0; x < l.width; x++) {
      uint32_t address =
          p.address + (l.tiled ? TiledOffset2D(x, y, p.pitch_blocks, bytes_per_block_log2)
                               : y * p.row_pitch_bytes + x * f.bytes_per_block);
      if (address + f.bytes_per_block <= kPhysicalMemorySize) {
        std::memcpy(row + x * f.bytes_per_block, PhysicalHostAddress(base, address),
                    f.bytes_per_block);
      }
    }
    SwapEndian(row, size_t(l.width) * f.bytes_per_block, fetch.endianness);
  }
  format = f.format;
  width = l.width;
  height = l.height;
  return true;
}

VkSampler TextureCache::GetSampler(const uint32_t fetch_words[6],
                                   const SamplerOverrides& overrides) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  auto pick = [](uint32_t override_value, uint32_t from_fetch, uint32_t use_fetch) {
    return override_value == use_fetch ? from_fetch : override_value;
  };
  uint32_t mag = pick(overrides.mag_filter, uint32_t(fetch.mag_filter), 3);
  uint32_t min = pick(overrides.min_filter, uint32_t(fetch.min_filter), 3);
  uint32_t mip = pick(overrides.mip_filter, uint32_t(fetch.mip_filter), 3);
  uint32_t aniso = pick(overrides.aniso_filter, uint32_t(fetch.aniso_filter), 7);
  if (!s.context.sampler_anisotropy) {
    aniso = 0;
  }
  Layout layout;
  uint32_t min_level = 0, max_level = 0;
  if (GetLayout(fetch, layout)) {
    min_level = layout.min_level;
    max_level = layout.max_level;
  }
  if (mip == uint32_t(xenos::TextureFilter::kBaseMap) || base_level_only_) {
    max_level = min_level;
  }
  uint32_t clamp[3] = {uint32_t(fetch.clamp_x), uint32_t(fetch.clamp_y), uint32_t(fetch.clamp_z)};
  if (fetch.dimension == xenos::DataDimension::kCube) {
    clamp[0] = clamp[1] = clamp[2] = uint32_t(xenos::ClampMode::kClampToEdge);
  }
  uint32_t border = uint32_t(fetch.border_color);
  uint64_t key = uint64_t(mag & 1) | uint64_t(min & 1) << 1 | uint64_t(mip & 3) << 2 |
                 uint64_t(aniso & 7) << 4 | uint64_t(clamp[0]) << 7 | uint64_t(clamp[1]) << 10 |
                 uint64_t(clamp[2]) << 13 | uint64_t(border) << 16 | uint64_t(min_level) << 18 |
                 uint64_t(max_level) << 22 | uint64_t(base_level_only_) << 30;
  VkSampler& sampler = s.samplers[key];
  if (sampler) {
    return sampler;
  }
  auto address_mode = [&](uint32_t mode) {
    switch (xenos::ClampMode(mode)) {
      case xenos::ClampMode::kRepeat:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
      case xenos::ClampMode::kMirroredRepeat:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
      case xenos::ClampMode::kClampToBorder:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
      case xenos::ClampMode::kMirrorClampToEdge:
      case xenos::ClampMode::kMirrorClampToHalfway:
      case xenos::ClampMode::kMirrorClampToBorder:
        return s.context.sampler_mirror_clamp_to_edge
                   ? VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
                   : VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
      default:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }
  };
  VkSamplerCreateInfo info{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  bool anisotropic = aniso >= uint32_t(xenos::AnisoFilter::kMax_2_1);
  info.magFilter = mag == 1 || anisotropic ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
  info.minFilter = min == 1 || anisotropic ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
  info.mipmapMode = mip == 1 ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
  info.addressModeU = address_mode(clamp[0]);
  info.addressModeV = address_mode(clamp[1]);
  info.addressModeW = address_mode(clamp[2]);
  if (anisotropic) {
    info.anisotropyEnable = VK_TRUE;
    info.maxAnisotropy =
        std::min(float(1u << (aniso - 1)), s.context.max_sampler_anisotropy);
  }
  info.minLod = float(min_level);
  info.maxLod = float(max_level);
  info.borderColor = border == 1   ? VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE
                     : border == 0 ? VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK
                                   : VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
  if (vk::vkCreateSampler(s.context.device, &info, nullptr, &sampler) != VK_SUCCESS) {
    sampler = VK_NULL_HANDLE;
  } else {
    s.stats.samplers++;
  }
  return sampler;
}

uint8_t TextureCache::SwizzledSigns(const uint32_t fetch_words[6]) {
  // As the SDK's texture_util::SwizzleSigns: each result component's sign
  // after the swizzle; constant components take signed only if everything
  // else is signed.
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  uint8_t signs = 0, constant_mask = 0;
  bool any_signed = false, any_not_signed = false;
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t select = (fetch.swizzle >> (3 * i)) & 7;
    if (select & 4) {
      constant_mask |= uint8_t(1) << (2 * i);
      continue;
    }
    uint32_t sign = (fetch.dword_0 >> (2 + 2 * select)) & 3;
    signs |= uint8_t(sign << (2 * i));
    (sign == uint32_t(xenos::TextureSign::kSigned) ? any_signed : any_not_signed) = true;
  }
  uint32_t constants_sign = 0;
  if (constant_mask == 0b01010101) {
    if (((fetch.dword_0 >> 2) & 0xFF) == 0b01010101) {
      constants_sign = 1;
    }
  } else if (any_signed && !any_not_signed) {
    constants_sign = 1;
  }
  return uint8_t(signs | constants_sign * constant_mask);
}

VkCommandBuffer TextureCache::EndUploads() {
  State& s = *state_;
  if (!s.recording) {
    return VK_NULL_HANDLE;
  }
  s.recording = false;
  VkCommandBuffer commands = s.frames[s.frame].uploads;
  vk::vkEndCommandBuffer(commands);
  return commands;
}

TextureCache::Stats TextureCache::TakeStats() {
  Stats stats = state_->stats;
  uint32_t textures = stats.textures, samplers = stats.samplers;
  state_->stats = {};
  state_->stats.textures = textures;
  state_->stats.samplers = samplers;
  return stats;
}

}  // namespace native
