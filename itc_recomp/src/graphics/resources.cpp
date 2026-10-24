// The graphics library's resources (docs/functions/graphics-resources.md):
// see resources.h.

#include "graphics/resources.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/edram.h"
#include "memory/c_runtime_heap.h"
#include "native/refcount.h"

#include <optional>

namespace graphics {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// XMemAlloc's attributes for a header (cleared), and XMemFree's for it.
constexpr uint32_t kHeaderAttributes = 0x64800000;
constexpr uint32_t kHeaderFreeAttributes = 0x24800000;
// A buffer's data: physical, with the protection field (bits 28-30) 3, or 2
// (write-combined, probably) with usage 0x4.
constexpr uint32_t kDataAttributes = 0x82800000;
constexpr uint32_t kUsageWriteCombined = 0x4;
constexpr uint32_t kUsage200 = 0x200;
constexpr uint32_t kUnlocked = 0xFFFF0000;  // the locked ranges at +20 and +24
// The formats' table (a halfword each, bits 8-11 a surface's format bits;
// probably bits a pixel elsewhere).
constexpr uint32_t kFormatTable = 0x82013468;
// 1 while a depth surface holds the hi-z.
constexpr uint32_t kHiZTaken = 0x829537C4;

}  // namespace

uint32_t ResourceType(uint8_t* base, uint32_t resource) {
  ITC_NATIVE_RUNNING("sub_822A6180 Resource::GetType");
  const uint32_t common = Load(base, resource);
  const uint32_t type = common & 0xF;
  if (type == 3) {
    switch ((Load(base, resource + 48) >> 9) & 3) {
      case 3:
        return 18;  // cube
      case 2:
        return 17;  // volume
      case 0:
        return 20;  // one-dimensional
      default:
        return (Load(base, resource + 32) & 0x400) ? 19 : 3;  // an array, or 2D
    }
  }
  if (type == 4 && (common & 0x40000000)) {
    const uint32_t parent = Load(base, resource + 24);
    if ((Load(base, parent + 48) & 0x600) == 0x400) {
      return 16;  // a volume's slice
    }
  }
  return type;
}

uint32_t TextureFormat(uint8_t* base, uint32_t texture) {
  ITC_NATIVE_RUNNING("sub_822A44A8 Texture::Format");
  const uint32_t w40 = Load(base, texture + 40);
  const uint32_t w28 = Load(base, texture + 28);
  const uint32_t w32 = Load(base, texture + 32);
  // The swizzle's four 3-bit fields (+40 bits 1-12), then +40's low bit.
  uint32_t swizzle = (w40 >> 7) & 0x3F;
  swizzle = (swizzle << 3) | ((w40 >> 4) & 7);
  swizzle = (swizzle << 3) | ((w40 >> 1) & 7);
  uint32_t format = (swizzle << 1) | (w40 & 1);
  // +28's four 2-bit fields (bits 2-9) and its top bit.
  format = (format << 2) | ((w28 >> 8) & 3);
  format = (format << 2) | ((w28 >> 6) & 3);
  format = (format << 2) | ((w28 >> 4) & 3);
  format = (format << 2) | ((w28 >> 2) & 3);
  format = (format << 1) | (w28 >> 31);
  // +32's bits 6-7, then its low 6 bits (the GPU format).
  format = (format << 2) | ((w32 >> 6) & 3);
  return (format << 6) | (w32 & 0x3F);
}

Block GpuBlockSize(uint32_t format) {
  ITC_NATIVE_RUNNING("sub_822A4410 GpuFormat::BlockSize");
  switch (format) {
    case 11:
    case 12:
    case 40:
      return {2, 1};
    case 39:
    case 44:
      return {4, 1};
    case 18:
    case 19:
    case 20:
    case 49:
    case 51:
    case 52:
    case 53:
    case 58:
    case 59:
    case 60:
    case 61:
      return {4, 4};
    default:
      return {1, 1};
  }
}

Extent TextureBaseSize(uint8_t* base, uint32_t texture) {
  ITC_NATIVE_RUNNING("sub_822A4348 Texture::BaseSize");
  const uint32_t dimension = (Load(base, texture + 48) >> 9) & 3;
  const uint32_t add = ((Load(base, texture + 40) >> 30) & 2) + 1;
  const uint32_t size = Load(base, texture + 36);
  if (dimension == 0) {
    return {(size & 0xFFFFFF) + add, 1, 1};
  }
  if (dimension == 2) {
    return {(size & 0x7FF) + add, ((size >> 11) & 0x7FF) + add, ((size >> 22) & 0x3FF) + add};
  }
  return {(size & 0x1FFF) + add, ((size >> 13) & 0x1FFF) + add, 1};
}

uint32_t TextureLevelCount(uint32_t width, uint32_t height, uint32_t depth, uint32_t border) {
  ITC_NATIVE_RUNNING("sub_822A4530 TextureLayout::LevelCount");
  const uint32_t twice = border << 1;
  const uint32_t d = depth - twice;
  uint32_t largest = int32_t(d) > 1 ? d : 1;
  const uint32_t h = height - twice;
  if (h > largest) {
    largest = h;
  }
  const uint32_t w = width - twice;
  if (w > largest) {
    largest = w;
  }
  return 32 - uint32_t(__builtin_clz(largest));  // largest is at least 1
}

uint32_t SurfaceEdramTiles(uint32_t width, uint32_t height, uint32_t format,
                           int32_t multisample) {
  ITC_NATIVE_RUNNING("sub_8209AD68 Surface::EdramTiles");
  if (multisample >= 1) {
    height <<= 1;
  }
  if (multisample == 2) {
    width <<= 1;
  }
  const uint32_t pitch = (width + 79) / 80 * 80;
  const uint32_t rows = (height + 15) & ~15u;
  const uint32_t low = format & 0x3F;
  const uint32_t bytes = (low == 21 || low == 32 || low == 37) ? 8 : 4;
  return rows * pitch * bytes / 5120;
}

uint32_t TextureLevelSize(uint32_t& width, uint32_t& height, uint32_t& depth, uint32_t bits,
                          uint32_t format, uint32_t dimension, uint32_t tiled) {
  ITC_NATIVE_RUNNING("sub_822A45B8 TextureLayout::LevelSize");
  const uint32_t depth_align = dimension == 2 ? 4 : 1;
  const Block block = GpuBlockSize(format);
  // The width's alignment in blocks: 32, or for linear textures 256 bytes'
  // worth when that's more.
  uint32_t row_blocks = 32;
  if (tiled == 0) {
    const uint32_t block_bytes = (block.width * block.height * bits) >> 3;
    if (block_bytes == 0) {
      ITC_LOG_WARN("tw/td trap hit (type 0): TextureLayout::LevelSize's divide by 0");
    }
    const uint32_t blocks = block_bytes ? 256 / block_bytes : 0;
    if (blocks >= 32) {
      row_blocks = blocks;
    }
  }
  const uint32_t width_align = block.width * row_blocks;
  width = (width + width_align - 1) & ~(width_align - 1);
  const uint32_t height_align = block.height << 5;
  height = (height + height_align - 1) & ~(height_align - 1);
  depth = (depth + depth_align - 1) & ~(depth_align - 1);
  const uint32_t slice = ((width * bits) >> 3) * height;
  if (dimension == 2) {
    return (depth * slice + 4095) & 0xFFFFF000;
  }
  return ((slice + 4095) & 0xFFFFF000) * depth;
}

namespace {

uint32_t Clz(uint32_t value) { return value == 0 ? 32 : uint32_t(__builtin_clz(value)); }
// PowerPC's slw and srw: a shift by 32 to 63 gives 0 (the shift's low 6 bits).
uint32_t Slw(uint32_t value, uint32_t shift) {
  return (shift & 0x20) ? 0 : value << (shift & 0x1F);
}
uint32_t Srw(uint32_t value, uint32_t shift) {
  return (shift & 0x20) ? 0 : value >> (shift & 0x1F);
}
uint32_t AtLeast1(uint32_t value) { return value > 1 ? value : 1; }

}  // namespace

LevelLayout TextureLevelLayout(uint8_t* base, uint32_t texture, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_822A4B98 Texture::LevelLayout");
  const uint32_t format = Load(base, texture + 32) & 0x3F;
  const uint32_t bits = guest::LoadU8(base, kFormatTable + 1 + (format << 1));
  const uint32_t type = ResourceType(base, texture);
  // The level's width, height and depth for TextureLevelSize (the
  // original's stack slots +88, +80 and +84; +80 is the aligned height).
  uint32_t width = 0, height = 0, depth = 0;
  uint32_t out_width, out_height, out_depth, pitch;
  // The level the layout and the alignment test use (the mip tail's for
  // packed mips).
  uint32_t at = level;
  if (type == 4) {
    const Extent size = TextureBaseSize(base, texture);
    const uint32_t w28 = Load(base, texture + 28);
    const uint32_t dimension = (Load(base, texture + 48) >> 9) & 3;
    out_height = size.height;
    out_width = size.width;
    out_depth = size.depth;
    const uint32_t row = ((w28 >> 22) & 0x1FF) * bits;
    width = out_width;
    height = out_height;
    depth = out_depth;
    TextureLevelSize(width, height, depth, bits, format, dimension, w28 >> 31);
    pitch = (row << 2) & 0x1FFFFFFC;
  } else {
    const uint32_t border = Load(base, texture + 40) >> 31;
    const uint32_t twice = border << 1;
    const Extent size = TextureBaseSize(base, texture);
    out_width = size.width;
    out_height = size.height;
    out_depth = size.depth;
    const uint32_t w48 = Load(base, texture + 48);
    const uint32_t dimension = (w48 >> 9) & 3;
    const bool packed = (w48 & 0x800) != 0;
    // The base size rounded up to powers of two (less the border).
    const uint32_t w_inner = out_width - twice;
    const uint32_t h_inner = out_height - twice;
    const uint32_t pow_width = Slw(1, border - Clz(w_inner - 1) + 32);
    const uint32_t pow_height = Slw(1, border - Clz(h_inner - 1) + 32);
    const uint32_t pow_depth =
        dimension == 2 ? Slw(1, border - Clz(out_depth - twice - 1) + 32) : 1;
    if (level == 0) {
      width = pow_width;
      height = pow_height;
      depth = pow_depth;
      if (!((pow_width <= 16 || pow_height <= 16) && packed)) {
        width = out_width;
        height = out_height;
        depth = out_depth;
      }
      const uint32_t w28 = Load(base, texture + 28);
      const uint32_t tiled = w28 >> 31;
      TextureLevelSize(width, height, depth, bits, format, dimension, tiled);
      const uint32_t w32 = Load(base, texture + 32);
      if (tiled == 0 && !(w32 & 0x400) && !packed && dimension == 1 &&
          !(Load(base, texture + 44) & 0x3C0) && border == 0) {
        // A linear 2D texture's height aligned only to its block.
        const Block block = GpuBlockSize(w32 & 0x3F);
        height = (block.height + out_height - 1) & ~(block.height - 1);
      }
      pitch = (((w28 >> 22) & 0x1FF) * bits << 2) & 0x1FFFFFFC;
    } else {
      const uint32_t base_width = out_width;
      const uint32_t base_height = out_height;
      out_width = AtLeast1(Srw(w_inner, level)) + twice;
      out_height = AtLeast1(Srw(h_inner, level)) + twice;
      out_depth = dimension == 2 ? AtLeast1(Srw(out_depth - twice, level)) + twice : 1;
      if (packed) {
        // The mip tail starts where the smaller side is below 16 texels.
        auto tail = [&] {
          const int32_t log_w = int32_t(border - Clz(base_width - twice - 1) + 32);
          const int32_t log_h = int32_t(border - Clz(base_height - twice - 1) + 32);
          const int32_t smaller = log_w < log_h ? log_w : log_h;
          return smaller - 4 > 0 ? uint32_t(smaller - 4) : 0u;
        };
        if (level >= tail()) {
          at = tail();
        }
      }
      width = AtLeast1(Srw(pow_width, at));
      height = AtLeast1(Srw(pow_height, at));
      depth = AtLeast1(Srw(pow_depth, at));
      TextureLevelSize(width, height, depth, bits, format, dimension,
                       Load(base, texture + 28) >> 31);
      pitch = (width * bits) >> 3;
    }
  }
  uint32_t slice = pitch * height;
  if (!(!(Load(base, texture + 28) & 0x80000000) && at <= 1 && (type == 3 || type == 20)) &&
      (Load(base, texture + 48) & 0x600) != 0x400) {
    slice = (slice + 4095) & 0xFFFFF000;
  }
  return {out_width, out_height, out_depth, pitch, slice};
}

uint32_t TextureLevelDesc(uint8_t* base, uint32_t texture, uint32_t level, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_822A4F60 Texture::LevelDesc");
  const LevelLayout layout = TextureLevelLayout(base, texture, level);
  Store(base, desc + 0, TextureFormat(base, texture));
  const uint32_t type = ResourceType(base, texture);
  Store(base, desc + 4, type);
  Store(base, desc + 8, 0);
  Store(base, desc + 12, 0);
  Store(base, desc + 20, 0);
  Store(base, desc + 24, layout.width);
  Store(base, desc + 28, layout.height);
  Store(base, desc + 16, 0);
  return type;
}

uint32_t SurfaceDesc(uint8_t* base, uint32_t surface, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_822A5F50 Surface::GetDesc");
  uint32_t r3 = surface;
  if (Load(base, surface) & 0x40000000) {
    const uint32_t level = Load(base, surface + 28) >> 28;
    r3 = TextureLevelDesc(base, Load(base, surface + 24), level, desc);
  } else {
    const uint32_t format = Load(base, surface + 40);
    Store(base, desc + 8, 0);
    Store(base, desc + 12, 0);
    Store(base, desc + 20, 0);
    Store(base, desc + 0, format);
    Store(base, desc + 24, ((Load(base, surface + 36) >> 18) & 0x3FFF) + 1);
    Store(base, desc + 28, ((Load(base, surface + 36) >> 3) & 0x7FFF) + 1);
    Store(base, desc + 16, guest::LoadU16(base, surface + 24) & 3);
  }
  Store(base, desc + 4, 4);
  return r3;
}

SurfaceSizes SetSurfaceHeader(uint8_t* base, uint32_t width, uint32_t height, uint32_t format,
                              int32_t multisample, uint32_t parameters, uint32_t header) {
  ITC_NATIVE_RUNNING("sub_822A5910 SurfaceHeader::Set");
  uint32_t low = format & 0x3F;
  const bool depth = low == 22 || low == 23;
  if (low == 54) {
    low = 7;
  }
  Store(base, header + 4, 1);
  Store(base, header + 0, 4);
  Store(base, header + 20, kUnlocked);
  const uint32_t tiles = SurfaceEdramTiles(width, height, format, multisample);
  // The hi-z: 32 x 16 pixel blocks (samples counted), for depth formats.
  uint32_t hi_z = 0;
  if (depth) {
    uint32_t w = width;
    uint32_t h = height;
    if (multisample >= 1) {
      h = height << 1;
    }
    if (multisample == 2) {
      w = width << 1;
    }
    hi_z = (((w + 31) >> 5) * ((h + 15) >> 4)) & 0x7FFFFF;
  }
  uint32_t edram_base = 0;
  uint32_t hi_z_base = 0xFFFFFFFF;
  uint32_t parameter8 = 0;
  if (parameters != 0) {
    edram_base = Load(base, parameters + 0);
    hi_z_base = Load(base, parameters + 4);
    parameter8 = Load(base, parameters + 8);
  } else if (depth && Load(base, kHiZTaken) == 0) {
    hi_z_base = 0;
  }
  // The pitch: 80-pixel tiles (40 with 4x samples), and the width aligned.
  uint32_t pitch, aligned;
  if (multisample == 0 || multisample == 1) {
    pitch = (width + 79) / 80 * 80;
    aligned = (width + 31) & ~31u;
  } else {
    pitch = (width + 39) / 40 * 40;
    aligned = (width + 15) & ~15u;
  }
  const uint32_t pitch_word = (aligned << 2) | (uint32_t(multisample) & 3);
  Store(base, header + 40, format);
  Store(base, header + 24, (pitch_word << 16) | (pitch & 0x3FFF));
  const uint32_t size = ((width - 1) << 18) | (((height - 1) << 3) & 0x3FFF8) |
                        (Load(base, header + 36) & 7);
  Store(base, header + 36, size);
  const uint32_t format_bits = (guest::LoadU16(base, kFormatTable + (low << 1)) >> 8) & 0xF;
  if (depth) {
    Store(base, header + 28, ((format_bits & 1) << 16) | (edram_base & 0xFFF));
    if (hi_z_base == 0xFFFFFFFF) {
      Store(base, header + 32, 0);
    } else {
      const uint32_t hi_z_word = (low != 22 ? 1 : 0) | (hi_z_base << 13);
      Store(base, header + 32, (hi_z_word << 4) | 1);
    }
  } else {
    uint32_t bits = format_bits;
    if (bits == 0 && !(format & 0x20000) && (format & 0x600) == 0x600 &&
        (format & 0x1800) == 0x1800 && (format & 0x6000) == 0x6000 && !(format & 0x18000)) {
      bits = 1;
    }
    bits = ((parameter8 << 4) & 0x3F0) | (bits & ~0x3F0u);
    Store(base, header + 28, ((bits << 16) & 0x3FF0000) | (edram_base & 0xFFF));
  }
  Store(base, header + 44, tiles * 5120);
  return {tiles, hi_z};
}

uint32_t CreateSurface(uint8_t* base, uint32_t width, uint32_t height, uint32_t format,
                       int32_t multisample, uint32_t parameters) {
  ITC_NATIVE_RUNNING("sub_822A5E28 Device::CreateSurface");
  const uint32_t header = memory::crt::XMemAlloc(base, 48, kHeaderAttributes);
  if (header == 0) {
    return 0;
  }
  const SurfaceSizes sizes =
      SetSurfaceHeader(base, width, height, format, multisample, parameters, header);
  Store(base, header + 0, Load(base, header + 0) | 0x00100000);
  if (parameters != 0) {
    return header;
  }
  const uint32_t low = Load(base, header + 40) & 0x3F;
  const bool depth = low == 22 || low == 23;
  const std::optional<edram::Allocation> allocation = edram::Alloc(base, sizes.tiles);
  if (!allocation || !allocation->ok) {
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  const uint32_t edram_base = allocation->base;
  if (edram_base + sizes.tiles > 2048) {
    edram::Free(base, edram_base, sizes.tiles);
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  Store(base, header + 0, Load(base, header + 0) | 0x80000000);
  Store(base, header + 28, (Load(base, header + 28) & 0xFFFFF000) | (edram_base & 0xFFF));
  if (depth && Load(base, kHiZTaken) == 0) {
    Store(base, kHiZTaken, 1);
    Store(base, header + 32, Load(base, header + 32) & 0x1FFFF);
  }
  return header;
}

uint32_t CreateVertexBuffer(uint8_t* base, uint32_t bytes, uint32_t usage) {
  ITC_NATIVE_RUNNING("sub_822A6830 Device::CreateVertexBuffer");
  const uint32_t header = memory::crt::XMemAlloc(base, 32, kHeaderAttributes);
  if (header == 0) {
    return 0;
  }
  uint32_t common = 0x00100001;
  uint32_t protection = 3;
  if (usage & kUsageWriteCombined) {
    common = 0x00300001;
    protection = 2;
  }
  if (usage & kUsage200) {
    common |= 0x00400000;
  }
  const uint32_t data = memory::crt::XMemAlloc(base, bytes, kDataAttributes | (protection << 28));
  if (data == 0) {
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  Store(base, header + 0, common);
  Store(base, header + 4, 1);
  const uint32_t old28 = Load(base, header + 28);
  Store(base, header + 24, data | 3);
  Store(base, header + 20, kUnlocked);
  Store(base, header + 28, (bytes & 0x3FFFFFC) | (old28 & 0x8000000) | 0x10000000 | 2);
  return header;
}

uint32_t CreateIndexBuffer(uint8_t* base, uint32_t bytes, uint32_t usage, uint32_t format) {
  ITC_NATIVE_RUNNING("sub_822A6968 Device::CreateIndexBuffer");
  const uint32_t header = memory::crt::XMemAlloc(base, 32, kHeaderAttributes);
  if (header == 0) {
    return 0;
  }
  uint32_t common = (format << 29) | 0x00100000 | 2;
  uint32_t protection = 3;
  if (usage & kUsageWriteCombined) {
    protection = 2;
    common |= 0x00200000;
  }
  if (usage & kUsage200) {
    common |= 0x00400000;
  }
  const uint32_t data = memory::crt::XMemAlloc(base, bytes, kDataAttributes | (protection << 28));
  if (data == 0) {
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  Store(base, header + 28, bytes);
  Store(base, header + 0, common);
  Store(base, header + 24, data);
  Store(base, header + 4, 1);
  Store(base, header + 20, kUnlocked);
  return header;
}

namespace {

// A device's slot, AddRef'ed when set.
uint32_t Current(uint8_t* base, uint32_t slot) {
  const uint32_t resource = Load(base, slot);
  if (resource != 0) {
    refcount::AddRef(base, resource);
  }
  return resource;
}

}  // namespace

uint32_t GetRenderTarget(uint8_t* base, uint32_t device, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_822A3998 Device::GetRenderTarget");
  return Current(base, device + ((index + 3046) << 2));
}

uint32_t GetDepthStencilSurface(uint8_t* base, uint32_t device) {
  ITC_NATIVE_RUNNING("sub_822A39E0 Device::GetDepthStencilSurface");
  return Current(base, device + 12200);
}

StreamSource GetStreamSource(uint8_t* base, uint32_t device, uint32_t stream) {
  ITC_NATIVE_RUNNING("sub_822A3880 Device::GetStreamSource");
  StreamSource source{Load(base, device + ((stream + 3051) << 2)), 0, 0};
  uint32_t offset = 0;
  if (source.buffer != 0) {
    // The stream's fetch constant's address, less the buffer's (+24's low
    // 29 bits, in the view its top 12 bits say).
    const uint32_t fetch = Load(base, device + ((223 - stream) << 3));
    const uint32_t data = Load(base, source.buffer + 24);
    offset = fetch - ((((data >> 20) & 0xFFF) + 512) & 0x1000) - (data & 0x1FFFFFFF);
    refcount::AddRef(base, source.buffer);
  }
  source.offset = offset << 2;
  source.stride = uint32_t(guest::LoadU8(base, device + stream + 12272)) << 2;
  return source;
}

uint32_t DestroySurface(uint8_t* base, uint32_t surface, uint32_t common) {
  ITC_NATIVE_RUNNING("sub_822A6268 Resource::DestroySurface");
  if (!(common & 0x40000000)) {
    if (common & 0x80000000) {
      const uint32_t tiles = Load(base, surface + 44) / 5120;
      edram::Free(base, Load(base, surface + 28) & 0xFFF, tiles);
    }
    if (Load(base, surface + 32) & 1) {
      Store(base, kHiZTaken, 0);
    }
  }
  return memory::crt::XMemFree(surface, kHeaderFreeAttributes);
}

}  // namespace graphics
