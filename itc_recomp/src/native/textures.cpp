#include <format>
#include "textures.h"
#include "frame_costs.h"

#include "buffers.h"
#include "gpu_memory.h"
#include "state.h"
#include "gpu/vulkan/backend.h"
#include "native/vulkan_types.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <bit>
#include <cmath>
#include <cstring>
#include <map>
#include <mutex>
#include <sys/mman.h>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>
#include "xenos_types.h"
#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"

#include <emmintrin.h>
#if defined(__SSSE3__)
#include <tmmintrin.h>
#endif

ITC_FLAG_BOOL(give_back_xpr_data, true, "ITC",
              "Upload XPR files' textures at load and give their data's pages back to the "
              "system (docs/pc-renderer.md, step 2b)");
ITC_FLAG_BOOL(native_renderer_log_texture_writes, false, "ITC",
              "Log which code writes textures' memory: on first load and when a texture "
              "is found changed, what the library's memcpy wrote there (a debugging aid; "
              "with --native_renderer_verify_textures every change is found)");

ITC_FLAG_DECLARE(bool, log_gpu_memory);
ITC_FLAG_BOOL(native_renderer_cache_texture_bindings, true, "ITC",
              "Answer texture views and samplers from a table of earlier answers by the "
              "fetch constant's words, instead of looking the texture up every binding");

namespace native {

namespace {
// Races left (TextureCache_RaceLeft), counted on the main thread.
std::atomic<uint64_t> g_races_left{0};

// XPR files loaded and released (TextureCache_XprLoaded, _XprReleased),
// from the threads that load them, for the render thread's frame start.
struct XprEvent {
  bool loaded;
  uint32_t data, size;
  std::vector<std::array<uint32_t, 6>> fetches;
};
std::mutex g_xpr_mutex;
std::vector<XprEvent> g_xpr_events;
}  // namespace


namespace {

// Per frame in flight: texture data on its way into images. A frame that
// needs more defers the rest to the next frames (placeholders meanwhile).
// Per frame in flight (host-visible: pinned system memory on a discrete
// GPU); a texture that doesn't fit waits for the next frame. 32 MB since
// docs/pc-renderer.md (64 before): the largest texture with its mips is
// ~21 MB.
constexpr VkDeviceSize kStagingSize = 32u << 20;

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
// docs/gpu-functions.md), and some neighbors. Packed formats store X in the
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
void GatherTiledRow(uint8_t* out, uint32_t layer_address, uint32_t x,
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
      const uint8_t* in = PhysicalHostAddress(address);
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

// GetView's answers by what it's asked: the fetch constant's 6 words and
// the view type.
// An entry holds while its generation is current; nearly every call
// repeats an earlier one ("Texture binding" in docs/native-renderer.md).
//
// The key is two 16-byte halves built in registers. Assembled on the stack
// from the words (stores of 16, 8 and 4 bytes) and read back with 16-byte
// loads, it cost most of the lookup: a load spanning several stores waits
// for them to be written (store forwarding fails).
class BindingTable {
 public:
  struct Key {
    __m128i low, high;  // words 0-3; words 4-5, the extra word and 0

    Key(const uint32_t words[6], uint32_t extra)
        : low(_mm_loadu_si128(reinterpret_cast<const __m128i*>(words))),
          high(_mm_unpacklo_epi64(
              _mm_loadl_epi64(reinterpret_cast<const __m128i*>(words + 4)),
              _mm_cvtsi32_si128(int(extra)))) {}
    Key() : low(_mm_setzero_si128()), high(_mm_setzero_si128()) {}
    bool operator==(const Key& other) const {
      __m128i differ = _mm_or_si128(_mm_xor_si128(low, other.low), _mm_xor_si128(high, other.high));
      return _mm_movemask_epi8(_mm_cmpeq_epi8(differ, _mm_setzero_si128())) == 0xFFFF;
    }
  };
  struct Entry {
    Key key;
    bool used;
    uint64_t value;       // the handle
    uint64_t generation;  // UINT64_MAX: not to be kept
  };

  // The key's entry, added unfilled (generation UINT64_MAX) if new. Valid
  // until the next call.
  Entry& Find(const Key& key) {
    if (used_ * 2 >= entries_.size()) {
      Grow();
    }
    size_t mask = entries_.size() - 1;
    for (size_t i = HashOf(key) & mask;; i = (i + 1) & mask) {
      Entry& e = entries_[i];
      if (!e.used) {
        e = {key, true, 0, UINT64_MAX};
        used_++;
        return e;
      }
      if (e.key == key) {
        return e;
      }
    }
  }

 private:
  static size_t HashOf(const Key& key) {
    uint64_t words[4] = {uint64_t(_mm_cvtsi128_si64(key.low)),
                         uint64_t(_mm_cvtsi128_si64(_mm_unpackhi_epi64(key.low, key.low))),
                         uint64_t(_mm_cvtsi128_si64(key.high)),
                         uint64_t(_mm_cvtsi128_si64(_mm_unpackhi_epi64(key.high, key.high)))};
    uint64_t h = 0;
    for (uint64_t word : words) {
      h = (h ^ word) * 0x9E3779B97F4A7C15ull;
    }
    // Multiplying carries bits only upward, and the address is in the high
    // half of a word: mixed down (MurmurHash3's finalizer), or the index
    // (the low bits) hardly depended on it (6-10 probes a lookup, not ~1).
    h ^= h >> 33;
    h *= 0xFF51AFD7ED558CCDull;
    h ^= h >> 33;
    h *= 0xC4CEB9FE1A85EC53ull;
    h ^= h >> 33;
    return size_t(h);
  }
  void Grow() {
    std::vector<Entry> old = std::move(entries_);
    entries_.assign(std::max<size_t>(old.size() * 2, 4096), Entry{});
    size_t mask = entries_.size() - 1;
    for (const Entry& e : old) {
      if (!e.used) {
        continue;
      }
      size_t i = HashOf(e.key) & mask;
      while (entries_[i].used) {
        i = (i + 1) & mask;
      }
      entries_[i] = e;
    }
  }
  std::vector<Entry> entries_;
  size_t used_ = 0;
};

}  // namespace

struct TextureCache::State {
  // XPR files' data blocks (by physical address): their textures, uploaded
  // at a frame's start; once all are on the GPU, the block's pages are
  // given back (`dropped`).
  struct Xpr {
    uint32_t size = 0;
    std::vector<std::array<uint32_t, 6>> fetches;
    bool dropped = false;
    uint32_t frames_waited = 0;
  };
  std::map<uint32_t, Xpr> xprs;
  uint64_t xpr_dropped_bytes = 0;
  uint32_t logged_dropped_misses = 0;
  uint32_t logged_dropped_writes = 0;
  uint64_t released_by_writes = 0;
  // An XPR released (or its memory written over): its textures leave (their
  // data is gone or about to be).
  void ReleaseXpr(std::map<uint32_t, Xpr>::iterator it) {
    for (const auto& fetch : it->second.fetches) {
      auto t = textures.find(KeyOf(fetch.data()));
      if (t == textures.end() || !t->second.kept) {
        continue;
      }
      t->second.kept = false;
      t->second.changed = true;  // so RetireOverwritten takes it for what replaces it
      RetireOverwritten(t->second);
    }
    if (it->second.dropped) {
      xpr_dropped_bytes -= it->second.size;
    }
    xprs.erase(it);
  }
  bool InDroppedXpr(uint32_t start, uint32_t end) const {
    auto it = xprs.upper_bound(start);
    if (it == xprs.begin()) {
      return false;
    }
    --it;
    return it->second.dropped && start < it->first + it->second.size && end > it->first;
  }
  uint64_t mismatched_total = 0;
  gpu::vulkan::Backend* backend = nullptr;
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
    TextureKey key{};
    Layout layout{};
    Format format{};
    xenos::Endian endian = xenos::Endian::kNone;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDeviceSize memory_size = 0;
    // With --log_gpu_memory: the second (State::now_s) it was last looked up
    // (with --native_renderer_cache_texture_bindings=false, last drawn).
    uint32_t last_drawn_s = 0;
    uint32_t levels = 0;
    bool loaded = false;
    // An XPR file's texture whose data's pages were given back once it was
    // on the GPU (docs/pc-renderer.md, step 2b): kept through race ends,
    // retired when the XPR is released.
    bool kept = false;
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
    uint32_t host_width = 0, host_height = 0;  // the image's (scaled)
    float scale = 1.0f;
    VkImageView attachment_view = VK_NULL_HANDLE;
    // A handed image's backend object (HandOver): its image, memory,
    // attachment_view and depth_view are the object's.
    gpu::Image* object = nullptr;
    // A resolve destination holding a pass's image (HandOver): its format
    // (format.format is the image's), and what readers apply.
    bool handed = false;
    VkFormat native_format = VK_FORMAT_UNDEFINED;
    bool swap_red_blue = false;
    float reader_scale = 0.0f;
    bool copied = false;  // a resolve copied into it (never handed images then)
    // A handed depth image: its views' aspect, and its depth-only view.
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageView depth_view = VK_NULL_HANDLE;
    // A resolve destination's last resolve (GetResolveDestination).
    std::chrono::steady_clock::time_point last_resolved{};
  };
  std::map<TextureKey, Texture> textures;
  // Loaded textures' ranges by start address (with the longest range's
  // size, to find those overlapping a write), and memcpy's writes taken
  // this frame.
  std::multimap<uint32_t, std::pair<uint32_t, Texture*>> by_address;
  uint32_t longest_range = 0;
  std::vector<std::pair<uint32_t, uint32_t>> new_writes;
  void MarkWritten();
  // Resolve destinations by base address (page) and the scale they're
  // drawn at (ResolveKey): one image per scale, for an address resolved
  // into from targets of different scales (the menus' frame and the race's
  // 3D view share one); draws sample the one written last (`current`).
  std::unordered_map<uint64_t, Texture> resolved;
  std::unordered_map<uint32_t, Texture*> current;
  static uint64_t ResolveKey(uint32_t page, float scale) {
    return uint64_t(std::bit_cast<uint32_t>(scale)) << 32 | page;
  }
  std::unordered_map<uint64_t, VkSampler> samplers;
  BindingTable view_answers;
  // Each fetch constant's last answer from them (a copy of the entry, so
  // with its generation: an answer not kept never matches).
  static std::array<BindingTable::Entry, 32> Unfilled() {
    std::array<BindingTable::Entry, 32> entries{};
    for (BindingTable::Entry& e : entries) {
      e.generation = UINT64_MAX;
    }
    return entries;
  }
  std::array<BindingTable::Entry, 32> last_views = Unfilled();
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

  // Textures freed after a race: destroyed once the frames that may still
  // use them are done (RetireLoaded, DestroyRetired).
  struct Retired {
    uint64_t frame_number;
    gpu_memory::Kind kind;
    VkImage image;
    VkDeviceMemory memory;
    VkDeviceSize memory_size;
    std::vector<VkImageView> views;
    // A handed image: destroyed by the backend (with its own views; `views`
    // holds only the cache's).
    gpu::Image* object = nullptr;
  };
  std::vector<Retired> retired;
  void Retire(const Texture& t);
  void RetireOverwritten(const Texture& t);
  void RetireResolveDestination(uint64_t key);
  void RetireMovedResolveDestinations(const Texture& t);
  static std::pair<uint32_t, uint32_t> ResolveRange(const Texture& t);
  void RetireLoaded();
  void DestroyRetired();

  // --log_gpu_memory: seconds since the cache started (set each frame), and
  // what new textures overlapped (NoteOverlaps), logged every 5 s
  // (LogAges).
  bool log_memory = false;
  std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
  uint32_t now_s = 0, logged_s = 0;
  struct Overlaps {
    uint64_t created = 0, overlapping = 0, old = 0, old_changed = 0, old_bytes = 0;
    uint64_t freed = 0, freed_bytes = 0;
  } overlaps;
  void NoteOverlaps(const Texture& t);
  void LogAges();
  uint32_t write_lines = 200;
  std::chrono::steady_clock::time_point writes_logged = std::chrono::steady_clock::now();
  void LogFirstLoad(const Texture& t);
  void LogUnrecordedChange(const Texture& t);
  void LogWriteSummary();

  bool CreateTexture(Texture& texture);
  uint64_t Hash(const Texture& texture) const;
  VkImageView GetSwizzledView(Texture& texture, uint32_t swizzle, VkImageViewType natural);
  bool Upload(Texture& texture);
  VkCommandBuffer Uploads();
};

std::string WriteCallers(const std::map<uint32_t, uint64_t>& callers) {
  std::string text;
  for (auto& [caller, count] : callers) {
    text += std::format(" {:08X}={}", caller, count);
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
    ITC_LOG_INFO("native renderer: texture writes: {:08X} ({}x{}, {} bytes) first loaded with "
                 "{} bytes written by memcpy{}",
                 address, t.layout.width, t.layout.height, size, bytes,
                 bytes ? std::format(" (first from {:08X})", first_caller) : std::string());
  }
}

// A texture whose memory changed with no write recorded: which bytes of its
// first range changed since the last such change.
void TextureCache::State::LogUnrecordedChange(const Texture& t) {
  uint32_t address = t.layout.base_address ? t.layout.base_address : t.layout.mip_address;
  auto& [count, text] = writes.unrecorded_by[address];
  if (!count++) {
    text = std::format("{}x{} format {} levels {}-{}", t.layout.width, t.layout.height,
                       int(t.format.format), t.layout.min_level, t.layout.max_level);
  }
  auto [start, range_size] = t.ranges[0].second ? t.ranges[0] : t.ranges[1];
  const uint8_t* now = PhysicalHostAddress(start);
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
    ITC_LOG_INFO("native renderer: texture writes: {:08X}: {} bytes changed, offsets {}-{}",
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
  ITC_LOG_INFO("native renderer: texture writes: {} first loads ({:.1f} MB): {} written wholly by "
               "memcpy, {} partly, {} not at all ({:.1f} MB); by first writer:{}",
               w.first, w.first_bytes / 1048576.0, w.covered, w.partly, w.uncovered,
               w.uncovered_bytes / 1048576.0, WriteCallers(w.first_callers));
  for (auto& [address, entry] : w.unrecorded_by) {
    ITC_LOG_INFO("native renderer: texture writes: {:08X} ({}) changed {} times without a "
                 "recorded write",
                 address, entry.second, entry.first);
  }
  w = WriteStats{};
}

bool TextureCache::State::CreateTexture(Texture& t) {
  const Layout& l = t.layout;
  t.levels = t.resolved ? 1 : l.max_level + 1;
  gpu::ImageDesc desc;
  desc.type = t.resolved ? gpu::ImageType::k2D
              : l.cube   ? gpu::ImageType::kCube
                         : gpu::ImageType::k2DArray;
  desc.format = vk_types::FormatFrom(t.format.format);
  desc.width = t.resolved ? t.host_width : l.width;
  desc.height = t.resolved ? t.host_height : l.height;
  desc.layers = l.layers;
  desc.levels = t.levels;
  desc.usage = t.resolved ? gpu::kImageSampled | gpu::kImageColorTarget | gpu::kImageCopySource
                          : gpu::kImageSampled | gpu::kImageCopyDest;
  if (desc.format == gpu::Format::kUndefined) {
    ITC_LOG_ERROR("native renderer: texture format {} has no gpu::Format", int(t.format.format));
    return false;
  }
  t.object = backend->CreateImage(desc);
  if (!t.object) {
    ITC_LOG_ERROR("native renderer: couldn't create a {}x{} texture of format {}", desc.width,
                  desc.height, int(t.format.format));
    return false;
  }
  const gpu::vulkan::ImageObject& object = gpu::vulkan::Backend::Object(t.object);
  t.image = object.image;
  t.memory = object.memory;
  t.memory_size = object.memory_size;
  gpu_memory::Allocated(t.resolved ? gpu_memory::Kind::kResolved : gpu_memory::Kind::kTexture,
                        t.memory_size);
  if (t.resolved) {
    t.attachment_view = object.view;  // the object's
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

uint64_t TextureCache::State::Hash(const Texture& t) const {
  ScopedCost cost(g_frame_costs.hash_ns);
  uint64_t hash = 0;
  for (const auto& [address, size] : t.ranges) {
    if (size) {
      hash = base::Hash64(PhysicalHostAddress(address), size, hash);
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

bool TextureCache::State::Upload(Texture& t) {
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
          GatherTiledRow(row.data(), layer_address, p.x, y + p.y, width_blocks,
                         p.pitch_blocks, bytes_per_block_log2);
        } else {
          uint32_t address =
              layer_address + (y + p.y) * p.row_pitch_bytes + p.x * f.bytes_per_block;
          if (address + row_bytes <= kPhysicalMemorySize) {
            std::memcpy(row.data(), PhysicalHostAddress(address), row_bytes);
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
  gpu::vulkan::Backend::Object(t.object).layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  stats.loaded_bytes += needed;
  (t.loaded ? stats.reloaded : stats.loaded)++;
  t.loaded = true;
  return true;
}

TextureCache::TextureCache() : state_(std::make_unique<State>()) {}
TextureCache::~TextureCache() = default;

bool TextureCache::Init(gpu::vulkan::Backend& backend, uint32_t frames_in_flight) {
  State& s = *state_;
  s.backend = &backend;
  const DeviceContext& context = backend.context();
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
    gpu_memory::Allocated(gpu_memory::Kind::kStaging, alloc.allocationSize);
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
  cache_bindings_ = ITC_FLAG(native_renderer_cache_texture_bindings);
  log_writes_ = ITC_FLAG(native_renderer_log_texture_writes);
  if (log_writes_) {
    s.LogWriteSummary();
  }
  s.DestroyRetired();
  s.log_memory = ITC_FLAG(log_gpu_memory);
  if (s.log_memory) {
    s.now_s = uint32_t(std::chrono::duration_cast<std::chrono::seconds>(
                           std::chrono::steady_clock::now() - s.start_time)
                           .count());
    if (s.now_s >= s.logged_s + 5) {
      s.logged_s = s.now_s;
      s.LogAges();
    }
  }
  if (uint64_t races = g_races_left.load(std::memory_order_relaxed); races != races_seen_) {
    races_seen_ = races;
    s.RetireLoaded();
    generation_++;  // answers and last texture sets hold their views
  }
  uint64_t marked = s.stats.marked;
  s.MarkWritten();
  if (s.stats.marked != marked) {
    generation_++;
    events_.marked_frames++;
  }
  ProcessXprs();
}

void TextureCache_XprLoaded(uint32_t data, uint32_t size,
                            std::vector<std::array<uint32_t, 6>> fetches) {
  std::lock_guard lock(g_xpr_mutex);
  g_xpr_events.push_back({true, data, size, std::move(fetches)});
}

void TextureCache_XprReleased(uint32_t data) {
  std::lock_guard lock(g_xpr_mutex);
  g_xpr_events.push_back({false, data, 0, {}});
}

void TextureCache::ProcessXprs() {
  State& s = *state_;
  std::vector<XprEvent> events;
  {
    std::lock_guard lock(g_xpr_mutex);
    events.swap(g_xpr_events);
  }
  if (!ITC_FLAG(give_back_xpr_data)) {
    return;
  }
  for (XprEvent& e : events) {
    uint32_t data = GpuPhysicalAddress(e.data);
    if (e.loaded) {
      State::Xpr& x = s.xprs[data];
      x = {};
      x.size = e.size;
      x.fetches = std::move(e.fetches);
      // The headers' base and mip addresses (words 1 and 5, 4 KB units) are
      // the CPU's view of the physical memory (0xE0000000, 4 KB back), as
      // RelocateTexture writes them; SetTexture binds the GPU's address. As
      // the draws will have them, or each texture is uploaded under a key no
      // draw asks for and the draws read the given-back pages' zeros (the
      // menus' 2D elements and much of a race, 2026-11-26). The sampler
      // fields set at binding aren't in the key (KeyOf).
      for (auto& fetch : x.fetches) {
        for (uint32_t w : {1u, 5u}) {
          if (fetch[w] & 0xFFFFF000u) {
            fetch[w] = GpuPhysicalAddress(fetch[w] & 0xFFFFF000u) | (fetch[w] & 0xFFFu);
          }
        }
      }
      continue;
    }
    auto it = s.xprs.find(data);
    if (it == s.xprs.end()) {
      continue;
    }
    s.ReleaseXpr(it);
    generation_++;
  }
  // Each XPR not yet given back: its textures uploaded (as a draw would;
  // one that doesn't fit this frame's staging waits for the next), then its
  // pages given back.
  for (auto& [data, x] : s.xprs) {
    if (x.dropped) {
      continue;
    }
    bool all = true;
    for (const auto& fetch : x.fetches) {
      xenos::xe_gpu_texture_fetch_t f = ToFetch(fetch.data());
      VkImageViewType natural = f.dimension == xenos::DataDimension::kCube
                                    ? VK_IMAGE_VIEW_TYPE_CUBE
                                    : VK_IMAGE_VIEW_TYPE_2D_ARRAY;
      bool cacheable = false;
      LookUpView(fetch.data(), natural, cacheable);
      auto t = s.textures.find(KeyOf(fetch.data()));
      if (t == s.textures.end()) {
        continue;  // a format we can't draw: nothing to keep
      }
      if (!t->second.loaded) {
        all = false;
      }
    }
    if (!all && ++x.frames_waited < 600) {
      continue;
    }
    if (!all) {
      // Left as is (its data stays) after 600 frames of trying.
      x.dropped = false;
      x.fetches.clear();
      continue;
    }
    for (const auto& fetch : x.fetches) {
      if (auto t = s.textures.find(KeyOf(fetch.data())); t != s.textures.end()) {
        t->second.kept = true;
      }
    }
    uint8_t* host = PhysicalHostAddress(data);
    uintptr_t first = (reinterpret_cast<uintptr_t>(host) + 0xFFF) & ~uintptr_t(0xFFF);
    uintptr_t end = (reinterpret_cast<uintptr_t>(host) + x.size) & ~uintptr_t(0xFFF);
    if (end > first) {
      madvise(reinterpret_cast<void*>(first), end - first, MADV_DONTNEED);
    }
    x.dropped = true;
    s.xpr_dropped_bytes += x.size;
  }
}

// The textures already held that a new one's memory overlaps: the game
// putting a texture where another was (the old one is then likely dead).
void TextureCache::State::NoteOverlaps(const Texture& t) {
  overlaps.created++;
  std::vector<const Texture*> found;
  for (const auto& [start, size] : t.ranges) {
    if (!size) {
      continue;
    }
    uint32_t end = start + size;
    auto it = by_address.lower_bound(start > longest_range ? start - longest_range : 0);
    for (; it != by_address.end() && it->first < end; ++it) {
      const Texture* old = it->second.second;
      if (it->first + it->second.first > start &&
          std::find(found.begin(), found.end(), old) == found.end()) {
        found.push_back(old);
      }
    }
  }
  overlaps.overlapping += !found.empty();
  for (const Texture* old : found) {
    overlaps.old++;
    overlaps.old_changed += old->changed;
    overlaps.old_bytes += old->memory_size;
  }
}

// Textures held by time since last looked up, and the overlaps since the
// last log.
void TextureCache::State::LogAges() {
  static constexpr uint32_t kLimits[] = {1, 10, 30, 60, UINT32_MAX};
  static constexpr const char* kNames[] = {"<1 s", "1-10 s", "10-30 s", "30-60 s", ">60 s"};
  uint64_t counts[5] = {}, bytes[5] = {};
  for (const auto& [key, t] : textures) {
    uint32_t age = now_s - t.last_drawn_s;
    size_t i = 0;
    while (age >= kLimits[i]) {
      i++;
    }
    counts[i]++;
    bytes[i] += t.memory_size;
  }
  std::string ages;
  for (size_t i = 0; i < 5; i++) {
    ages += std::format("{}{} {} ({:.1f} MB)", i ? ", " : "", kNames[i], counts[i],
                        bytes[i] / 1048576.0);
  }
  ITC_LOG_INFO("native renderer: textures by time since drawn: {}; new {}, {} overlapping "
               "held textures: {} of them ({} written since loaded, {:.1f} MB); overwritten "
               "freed {} ({:.1f} MB)",
               ages, overlaps.created, overlaps.overlapping, overlaps.old, overlaps.old_changed,
               overlaps.old_bytes / 1048576.0, overlaps.freed, overlaps.freed_bytes / 1048576.0);
  overlaps = {};
}

void TextureCache_RaceLeft() {
  g_races_left.fetch_add(1, std::memory_order_relaxed);
}

// Every texture loaded from guest memory (not resolve destinations, which
// guest memory doesn't hold) leaves the cache; draws load them again.
// Called before this frame's draws, so only earlier frames can use them.
void TextureCache::State::RetireLoaded() {
  VkDeviceSize bytes = 0;
  // XPR textures whose data's pages were given back stay: they can't be
  // loaded again (they leave when their XPR is released).
  std::vector<TextureKey> kept_keys;
  for (auto& [key, t] : textures) {
    if (t.kept) {
      kept_keys.push_back(key);
      continue;
    }
    bytes += t.memory_size;
    Retire(t);
  }
  if (log_memory) {
    ITC_LOG_INFO("native renderer: a race ended: {} textures freed ({:.1f} MB)", textures.size(),
                 bytes / 1048576.0);
  }
  std::erase_if(textures, [](const auto& entry) { return !entry.second.kept; });
  by_address.clear();
  longest_range = 0;
  for (auto& [key, t] : textures) {
    for (const auto& [address, size] : t.ranges) {
      if (size) {
        by_address.emplace(address, std::make_pair(size, &t));
        longest_range = std::max(longest_range, size);
      }
    }
  }
  write_snapshots.clear();
}

// Its image, memory and views are destroyed once the frames that may
// use them are done; the caller drops it from the maps.
void TextureCache::State::Retire(const Texture& t) {
  Retired r{frame_number,
            t.resolved ? gpu_memory::Kind::kResolved : gpu_memory::Kind::kTexture,
            t.image,
            t.memory,
            t.memory_size,
            {}};
  for (const auto& [swizzle, view] : t.views) {
    if (view) {
      r.views.push_back(view);
    }
  }
  // The image and its own views go with the object.
  r.object = t.object;
  r.image = VK_NULL_HANDLE;
  r.memory = VK_NULL_HANDLE;
  retired.push_back(std::move(r));
}

// A resolve destination leaves `resolved` (and `current`, if it's the one
// drawn from its address).
void TextureCache::State::RetireResolveDestination(uint64_t key) {
  auto it = resolved.find(key);
  uint32_t page = it->second.layout.base_address >> 12;
  if (auto c = current.find(page); c != current.end() && c->second == &it->second) {
    current.erase(c);
  }
  Retire(it->second);
  resolved.erase(it);
}

// The guest memory a resolve destination stands for: its size in 32x32
// tiles (the game's own spacing of them matches).
std::pair<uint32_t, uint32_t> TextureCache::State::ResolveRange(const Texture& t) {
  uint32_t width = (t.layout.width + 31) & ~31u, height = (t.layout.height + 31) & ~31u;
  return {t.layout.base_address, t.layout.base_address + width * height * t.format.bytes_per_block};
}

// A new resolve destination t (not yet held) is made: the held ones at
// other addresses whose memory it overlaps were moved there by the game
// (its glow chain is allocated again per race), so they go. Not one
// resolved into in the last 2 s: a resolve into part of a bigger one.
void TextureCache::State::RetireMovedResolveDestinations(const Texture& t) {
  auto [start, end] = ResolveRange(t);
  auto now = std::chrono::steady_clock::now();
  std::vector<uint64_t> moved;
  for (const auto& [key, old] : resolved) {
    auto [old_start, old_end] = ResolveRange(old);
    if (old.layout.base_address != t.layout.base_address && old_start < end && start < old_end &&
        now - old.last_resolved >= std::chrono::seconds(2)) {
      if (log_memory) {
        ITC_LOG_INFO("native renderer: resolve destination at {:08X} (format {} {}x{}) freed: "
                     "one made at {:08X} (format {} {}x{}) over its memory",
                     old.layout.base_address, old.guest_format, old.layout.width,
                     old.layout.height, t.layout.base_address, t.guest_format, t.layout.width,
                     t.layout.height);
      }
      moved.push_back(key);
    }
  }
  for (uint64_t key : moved) {
    RetireResolveDestination(key);
  }
}

// A new texture t (not yet held) is loaded: the held textures its memory
// overlaps that the game wrote over since they were loaded are dead (the
// memory holds t now; drawn again, they'd reload t's data), so they go.
// Not those unwritten: two textures reading the same memory both stay.
void TextureCache::State::RetireOverwritten(const Texture& t) {
  std::vector<Texture*> found;
  for (const auto& [start, size] : t.ranges) {
    if (!size) {
      continue;
    }
    uint32_t end = start + size;
    auto it = by_address.lower_bound(start > longest_range ? start - longest_range : 0);
    for (; it != by_address.end() && it->first < end; ++it) {
      Texture* old = it->second.second;
      if (it->first + it->second.first > start && old->changed &&
          std::find(found.begin(), found.end(), old) == found.end()) {
        found.push_back(old);
      }
    }
  }
  for (Texture* old : found) {
    overlaps.freed++;
    overlaps.freed_bytes += old->memory_size;
    Retire(*old);
    for (const auto& [address, size] : old->ranges) {
      if (!size) {
        continue;
      }
      auto [first, last] = by_address.equal_range(address);
      for (auto it = first; it != last;) {
        it = it->second.second == old ? by_address.erase(it) : std::next(it);
      }
    }
    TextureKey key = old->key;  // not a reference into the node erased
    textures.erase(key);
  }
}

// Each frame's start waits for its slot's last use, so a texture retired
// at frame N is unused once frame N + frames in flight starts.
void TextureCache::State::DestroyRetired() {
  if (retired.empty() || frame_number < retired.front().frame_number + frames.size()) {
    return;
  }
  auto due = std::partition_point(retired.begin(), retired.end(), [&](const Retired& r) {
    return frame_number >= r.frame_number + frames.size();
  });
  for (Retired& r : std::span(retired.begin(), due)) {
    for (VkImageView view : r.views) {
      backend->DestroyView(reinterpret_cast<gpu::View*>(view));
    }
    gpu_memory::Created(gpu_memory::Object::kImageView, -int64_t(r.views.size()));
    if (r.object) {
      backend->DestroyImage(r.object);
    } else {
      vk::vkDestroyImage(context.device, r.image, nullptr);
      vk::vkFreeMemory(context.device, r.memory, nullptr);
    }
    gpu_memory::Freed(r.kind, r.memory_size);
  }
  retired.erase(retired.begin(), due);
}

TextureCache::GenerationEvents TextureCache::TakeGenerationEvents() {
  GenerationEvents events = events_;
  events_ = {};
  return events;
}

// The textures memcpy has written since the last frame are marked to be
// reloaded (the game writes textures' memory with the library's memcpy:
// see docs/texture-handling-optimization.md).
void TextureCache::State::MarkWritten() {
  new_writes.clear();
  Buffers_TakeNewWrites(new_writes);
  ScopedCost cost(g_frame_costs.mark_ns);
  stats.writes += new_writes.size();
  for (const auto& [start, end] : new_writes) {
    // An XPR's data given back and written over: its memory was reused
    // without its release seen (seen leaving the tuning screen), so it's
    // taken as released, its textures retired.
    while (InDroppedXpr(start, end)) {
      auto it = std::prev(xprs.upper_bound(start));
      if (logged_dropped_writes < 20) {
        logged_dropped_writes++;
        ITC_LOG_INFO("native renderer: memcpy wrote {:08X}-{:08X} over the XPR data at {:08X} "
                     "({} bytes) given back: taken as released",
                     start, end, it->first, it->second.size);
      }
      ReleaseXpr(it);
      released_by_writes++;
      stats.marked++;  // the bindings' answers may hold its textures' views
    }
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

namespace {
template <typename Handle>
uint64_t HandleBits(Handle handle) {
  return uint64_t(reinterpret_cast<uintptr_t>(handle));
}
template <typename Handle>
Handle FromBits(uint64_t bits) {
  return reinterpret_cast<Handle>(uintptr_t(bits));
}
}  // namespace

VkImageView TextureCache::GetView(const uint32_t fetch_words[6],
                                  VkImageViewType view_type, uint32_t fetch_constant) {
  bool cacheable;
  // Hashing every texture every frame needs every call.
  if (!cache_bindings_ || verify_every_frame_) {
    unkept_answers_++;
    return LookUpView(fetch_words, view_type, cacheable);
  }
  State& s = *state_;
  BindingTable::Key key(fetch_words, uint32_t(view_type));
  // The same as this fetch constant's last call (about 44% of calls in a
  // race): no need to hash.
  BindingTable::Entry& last = s.last_views[fetch_constant & 31];
  if (last.generation == generation_ && last.key == key) {
    return FromBits<VkImageView>(last.value);
  }
  BindingTable::Entry& e = s.view_answers.Find(key);
  if (e.generation != generation_) {
    VkImageView view = LookUpView(fetch_words, view_type, cacheable);
    e.value = HandleBits(view);
    // Not kept: nothing (a placeholder, maybe only until staging has room),
    // or a texture still to be reloaded.
    e.generation = cacheable && view ? generation_ : UINT64_MAX;
    unkept_answers_ += e.generation == UINT64_MAX;
  }
  last = e;
  return FromBits<VkImageView>(e.value);
}

VkImageView TextureCache::LookUpView(const uint32_t fetch_words[6],
                                     VkImageViewType view_type, bool& cacheable) {
  State& s = *state_;
  cacheable = false;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  if (fetch.type != xenos::FetchConstantType::kTexture) {
    s.stats.unsupported++;
    return VK_NULL_HANDLE;
  }
  // A resolve destination at this address takes the place of guest memory.
  if (auto r = s.current.find(fetch.base_address & 0x1FFFF);
      r != s.current.end() && r->second->loaded) {
    State::Texture& t = *r->second;
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
    cacheable = true;
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
    if (s.logged_dropped_misses < 40) {
      uint32_t start = t.layout.base_address ? t.layout.base_address : t.layout.mip_address;
      if (s.InDroppedXpr(start, start + 1)) {
        s.logged_dropped_misses++;
        std::string header;
        for (const auto& [data, x] : s.xprs) {
          if (start < data || start >= data + x.size) {
            continue;
          }
          header += std::format(" (in XPR data {:08X}-{:08X}:", data, data + x.size);
          for (const auto& f : x.fetches) {
            header += std::format(" [{:08X} {:08X} {:08X} {:08X} {:08X} {:08X}]", f[0], f[1], f[2],
                                  f[3], f[4], f[5]);
          }
          header += ")";
        }
        const uint8_t* bytes = PhysicalHostAddress(start);
        uint32_t nonzero = 0;
        for (uint32_t i = 0; i < 65536; i++) {
          nonzero += bytes[i] != 0;
        }
        ITC_LOG_ERROR("native renderer: a texture at {:08X} (format {}, {}x{}) asked for from an "
                      "XPR whose data was given back ({} of its first 64 KB not zero); fetch "
                      "{:08X} {:08X} {:08X} {:08X} {:08X} {:08X}{}",
                      start, uint32_t(fetch.format), t.layout.width, t.layout.height, nonzero,
                      fetch_words[0], fetch_words[1], fetch_words[2], fetch_words[3],
                      fetch_words[4], fetch_words[5], header);
      }
    }
    if (!s.CreateTexture(t)) {
      return VK_NULL_HANDLE;
    }
    if (t.format.block_width == 1) {
      // Uncompressed: few, and the likeliest to be decoded wrong.
      ITC_LOG_INFO("native renderer: texture at {:08X}: format {} {}x{}x{} levels {}-{}, endian "
                   "{}, tiled {}, pitch {}, signs {:02X}, swizzle {:03o}, exp adjust {}",
                   t.layout.base_address ? t.layout.base_address : t.layout.mip_address,
                   uint32_t(fetch.format), t.layout.width, t.layout.height, t.layout.layers,
                   t.layout.min_level, t.layout.max_level, uint32_t(t.endian), t.layout.tiled,
                   t.layout.pitch_texels, (fetch.dword_0 >> 2) & 0xFF, uint32_t(fetch.swizzle),
                   int32_t(fetch.exp_adjust));
    }
    if (s.log_memory) {
      s.NoteOverlaps(t);
    }
    size_t held = s.textures.size();
    s.RetireOverwritten(t);
    if (s.textures.size() != held) {
      generation_++;  // answers and last texture sets may hold their views
    }
    t.key = key;
    it = s.textures.emplace(key, std::move(t)).first;
    events_.created++;
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
  bool log_writes = log_writes_;
  if (!t.loaded) {
    if (verify_every_frame_) {
      t.hash = s.Hash(t);
      t.verified_frame = s.frame_number;
    }
    if (!s.Upload(t)) {
      return VK_NULL_HANDLE;
    }
    t.changed = false;
    t.loaded_frame = s.frame_number;
    if (log_writes) {
      s.LogFirstLoad(t);
    }
  } else if (t.changed) {
    uint64_t hash = verify_every_frame_ ? s.Hash(t) : 0;
    if (s.Upload(t)) {  // else the old contents until it fits
      t.changed = false;
      t.hash = hash;
      t.verified_frame = s.frame_number;
    }
  } else if (verify_every_frame_ && t.verified_frame != s.frame_number) {
    t.verified_frame = s.frame_number;
    if (uint64_t hash = s.Hash(t); hash != t.hash) {
      s.stats.unrecorded++;
      if (s.stats.unrecorded_sample == UINT32_MAX) {
        s.stats.unrecorded_sample =
            t.layout.base_address ? t.layout.base_address : t.layout.mip_address;
        s.stats.unrecorded_frames = uint32_t(s.frame_number - t.loaded_frame);
      }
      if (log_writes) {
        s.LogUnrecordedChange(t);
      }
      if (s.Upload(t)) {
        t.hash = hash;
      }
    }
  }

  cacheable = !t.changed;  // else staging was full: try again next call
  t.last_drawn_s = s.now_s;
  return s.GetSwizzledView(t, fetch.swizzle, natural);
}

VkImageView TextureCache::State::GetSwizzledView(Texture& t, uint32_t swizzle,
                                                 VkImageViewType natural) {
  VkImageView& view = t.views[swizzle];
  if (!view) {
    auto component = [&](uint32_t i) {
      uint32_t select = (swizzle >> (3 * i)) & 7;
      // A handed pass's image isn't red and blue exchanged as a resolve's
      // copy is: the swizzle's X and Z exchanged instead.
      if (t.swap_red_blue && (select == 0 || select == 2)) {
        select ^= 2;
      }
      if (select == 4) {
        return gpu::Swizzle::kZero;
      }
      if (select >= 5) {
        return gpu::Swizzle::kOne;
      }
      return gpu::Swizzle(std::min(select, t.format.components - 1));
    };
    gpu::ViewDesc desc;
    desc.type = natural == VK_IMAGE_VIEW_TYPE_CUBE ? gpu::ImageType::kCube
                                                   : gpu::ImageType::k2DArray;
    for (uint32_t i = 0; i < 4; i++) {
      desc.swizzle[i] = component(i);
    }
    desc.depth_only = t.aspect == VK_IMAGE_ASPECT_DEPTH_BIT;
    view = gpu::vulkan::Backend::Handle(backend->CreateView(t.object, desc));
    if (view) {
      gpu_memory::Created(gpu_memory::Object::kImageView);
    }
  }
  return view;
}

bool TextureCache::GetResolveDestination(const uint32_t fetch_words[6], float scale,
                                         ResolveDestination& out) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  uint32_t page = fetch.base_address & 0x1FFFF;
  uint32_t width = fetch.size_2d.width + 1, height = fetch.size_2d.height + 1;
  uint64_t key = State::ResolveKey(page, scale);
  auto it = s.resolved.find(key);
  if (it != s.resolved.end() && (it->second.guest_format != uint32_t(fetch.format) ||
                                 it->second.layout.width != width ||
                                 it->second.layout.height != height)) {
    // Another texture at the same address: start over (the old image is
    // destroyed once the frames in flight that may use it are done).
    ITC_LOG_WARN("native renderer: resolve destination at {:08X} changed from format {} {}x{} "
                 "to format {} {}x{}",
                 page << 12, it->second.guest_format, it->second.layout.width,
                 it->second.layout.height, uint32_t(fetch.format), width, height);
    s.RetireResolveDestination(key);
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
    t.native_format = t.format.format;
    t.layout.width = width;
    t.layout.height = height;
    t.scale = scale;
    t.host_width = std::max(uint32_t(std::lround(width * scale)), 1u);
    t.host_height = std::max(uint32_t(std::lround(height * scale)), 1u);
    t.layout.layers = 1;
    t.layout.base_address = page << 12;
    if (!s.CreateTexture(t)) {
      return false;
    }
    ITC_LOG_INFO("native renderer: resolve destination at {:08X}: format {} {}x{} (host format {}, "
                 "{}x{}, scale {})",
                 page << 12, t.guest_format, width, height, int(t.format.format), t.host_width,
                 t.host_height, scale);
    size_t held = s.resolved.size();
    s.RetireMovedResolveDestinations(t);
    if (s.resolved.size() != held) {
      generation_++;  // answers and last texture sets may hold their views
    }
    it = s.resolved.emplace(key, std::move(t)).first;
    events_.created++;
  }
  State::Texture& t = it->second;
  t.last_resolved = std::chrono::steady_clock::now();
  if (State::Texture*& current = s.current[page]; current != &t || !t.loaded) {
    current = &t;
    generation_++;
    events_.resolve_changes++;
  }
  out = {t.object,        t.image,
         t.attachment_view, t.format.format, width,     height,
         t.host_width,    t.host_height,     t.scale,         !t.loaded, t.handed,
         t.native_format, t.copied};
  t.loaded = true;  // written by the caller
  return true;
}

TextureCache::HandedImage TextureCache::HandOver(const uint32_t fetch_words[6], float scale,
                                                 const HandedImage& image, bool swap_red_blue,
                                                 float reader_scale) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  uint32_t page = fetch.base_address & 0x1FFFF;
  auto it = s.resolved.find(State::ResolveKey(page, scale));
  if (it == s.resolved.end()) {
    return {};
  }
  State::Texture& t = it->second;
  VkDevice device = s.context.device;
  auto size_of = [&](VkImage i) {
    VkMemoryRequirements requirements;
    vk::vkGetImageMemoryRequirements(device, i, &requirements);
    return requirements.size;
  };
  // The views of the image it held go once no frame in flight can use them
  // (this frame's earlier draws may have sampled it).
  State::Retired views{s.frame_number, gpu_memory::Kind::kResolved, VK_NULL_HANDLE,
                       VK_NULL_HANDLE, 0, {}};
  for (const auto& [swizzle, view] : t.views) {
    if (view) {
      views.views.push_back(view);
    }
  }
  t.views.clear();
  HandedImage back;
  if (t.handed && t.format.format == image.format) {
    // A pass's image before: back to drawing (its views for sampling go).
    back = {t.object, t.image, t.memory, t.attachment_view, t.format.format, t.aspect,
            t.depth_view};
    gpu_memory::Freed(gpu_memory::Kind::kResolved, t.memory_size);
    gpu_memory::Allocated(gpu_memory::Kind::kImage, t.memory_size);
  } else {
    // Its own image, or a handed one of another format: the backend's to
    // destroy.
    views.object = t.object;
    views.memory_size = t.memory_size;
  }
  s.retired.push_back(std::move(views));
  t.object = image.object;
  t.image = image.image;
  t.memory = image.memory;
  t.attachment_view = image.view;
  t.aspect = image.aspect;
  t.depth_view = image.depth_view;
  t.memory_size = size_of(image.image);
  gpu_memory::Freed(gpu_memory::Kind::kImage, t.memory_size);
  gpu_memory::Allocated(gpu_memory::Kind::kResolved, t.memory_size);
  t.format.format = image.format;
  t.handed = true;
  t.swap_red_blue = swap_red_blue;
  t.reader_scale = reader_scale;
  t.loaded = true;
  s.current[page] = &t;
  generation_++;  // answers and last texture sets hold the old views
  events_.resolve_changes++;
  return back;
}

bool TextureCache::RestoreResolveDestination(const uint32_t fetch_words[6], float scale,
                                             ResolveDestination& out) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  uint32_t page = fetch.base_address & 0x1FFFF;
  auto it = s.resolved.find(State::ResolveKey(page, scale));
  if (it == s.resolved.end()) {
    return false;
  }
  State::Texture& t = it->second;
  t.copied = true;
  if (t.handed) {
    s.Retire(t);
    t.object = nullptr;
    t.image = VK_NULL_HANDLE;
    t.memory = VK_NULL_HANDLE;
    t.attachment_view = VK_NULL_HANDLE;
    t.depth_view = VK_NULL_HANDLE;
    t.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    t.views.clear();
    t.format.format = t.native_format;
    t.handed = false;
    t.swap_red_blue = false;
    t.reader_scale = 0.0f;
    t.loaded = false;
    if (!s.CreateTexture(t)) {
      return false;
    }
    generation_++;
  }
  return GetResolveDestination(fetch_words, scale, out);
}

bool TextureCache::HoldsHanded(const uint32_t fetch_words[6], float scale, VkImage image) const {
  const State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  auto it = s.resolved.find(State::ResolveKey(fetch.base_address & 0x1FFFF, scale));
  return it != s.resolved.end() && it->second.handed && it->second.image == image;
}

float TextureCache::ReaderScale(const uint32_t fetch_words[6]) const {
  const State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  auto r = s.current.find(fetch.base_address & 0x1FFFF);
  if (r == s.current.end() || !r->second->loaded || !r->second->handed) {
    return 0.0f;
  }
  return r->second->reader_scale;
}

uint64_t TextureCache::mismatched_total() const {
  return state_->mismatched_total;
}

bool TextureCache::DecodeGuest(const uint32_t fetch_words[6],
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
        std::memcpy(row + x * f.bytes_per_block, PhysicalHostAddress(address),
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

namespace {

// A sampler as the stage's state describes it, for a texture of these
// levels (a cube map clamps to the edge).
gpu::SamplerDesc DescFromState(const render::SamplerState& state, bool cube, uint32_t min_level,
                               uint32_t max_level, bool base_level_only) {
  gpu::SamplerDesc desc;
  desc.mag_filter = state.mag_filter;
  desc.min_filter = state.min_filter;
  desc.mip_mode = state.mip_mode;
  for (uint32_t i = 0; i < 3; i++) {
    desc.address[i] = cube ? gpu::AddressMode::kClampToEdge : state.address[i];
  }
  desc.max_anisotropy = state.max_anisotropy;
  desc.mip_lod_bias = state.mip_lod_bias;
  desc.min_lod = float(min_level);
  desc.max_lod = float(state.base_level_only || base_level_only ? min_level : max_level);
  desc.border = state.border;
  return desc;
}

}  // namespace

VkSampler TextureCache::GetSampler(const render::SamplerState& state,
                                   const uint32_t fetch_words[6]) {
  State& s = *state_;
  xenos::xe_gpu_texture_fetch_t fetch = ToFetch(fetch_words);
  Layout layout;
  uint32_t min_level = 0, max_level = 0;
  if (GetLayout(fetch, layout)) {
    min_level = layout.min_level;
    max_level = layout.max_level;
  }
  return SamplerFor(DescFromState(state, fetch.dimension == xenos::DataDimension::kCube,
                                  min_level, max_level, base_level_only_));
}

VkSampler TextureCache::SamplerFor(const gpu::SamplerDesc& desc) {
  State& s = *state_;
  // Keyed by the description's fields.
  uint64_t key = uint64_t(desc.mag_filter) | uint64_t(desc.min_filter) << 1 |
                 uint64_t(desc.mip_mode) << 2 | uint64_t(desc.address[0]) << 3 |
                 uint64_t(desc.address[1]) << 6 | uint64_t(desc.address[2]) << 9 |
                 uint64_t(desc.border) << 12 |
                 uint64_t(std::bit_cast<uint32_t>(desc.max_anisotropy) >> 23 & 0xFF) << 16 |
                 uint64_t(uint32_t(desc.min_lod) & 0xF) << 24 |
                 uint64_t(uint32_t(desc.max_lod) & 0xF) << 28 |
                 uint64_t(std::bit_cast<uint32_t>(desc.mip_lod_bias)) << 32;
  VkSampler& sampler = s.samplers[key];
  if (sampler) {
    return sampler;
  }
  sampler = gpu::vulkan::Backend::Handle(s.backend->CreateSampler(desc));
  if (sampler) {
    s.stats.samplers++;
    gpu_memory::Created(gpu_memory::Object::kSampler);
  }
  return sampler;
}

uint8_t TextureCache::SwizzledSigns(const uint32_t fetch_words[6]) {
  // As the SDK's xenos::SwizzleSigns: each result component's sign
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
