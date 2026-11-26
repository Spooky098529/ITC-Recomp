// The graphics library's resources (docs/functions/graphics-resources.md):
// see resources.h.

#include "graphics/resources.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "c_runtime/strings.h"
#include "native/refcount.h"
#include "schema/device_state.h"
#include "schema/graphics_resources.h"

#include <array>
#include <bit>
#include <optional>

namespace graphics {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
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
using G = schema::GraphicsResourcesGlobals;
using schema::BufferHeader;
using schema::LevelDesc;
using schema::ResourceHeader;
using schema::SurfaceHeader;
using schema::TextureHeader;
// The formats' table (a halfword each, bits 8-11 a surface's format bits;
// probably bits a pixel elsewhere).
constexpr uint32_t kFormatTable = G::format_table_address();

}  // namespace

uint32_t ResourceType(uint32_t resource) {
  ITC_NATIVE_RUNNING("sub_822A6180 Resource::GetType");
  const ResourceHeader r(resource);
  const uint32_t common = r.common();
  const uint32_t type = common & 0xF;
  if (type == 3) {
    switch ((r.fetch_mips() >> 9) & 3) {
      case 3:
        return 18;  // cube
      case 2:
        return 17;  // volume
      case 0:
        return 20;  // one-dimensional
      default:
        return (r.fetch_format() & 0x400) ? 19 : 3;  // an array, or 2D
    }
  }
  if (type == 4 && (common & 0x40000000)) {
    const TextureHeader parent = r.parent();
    if ((parent.fetch_mips() & 0x600) == 0x400) {
      return 16;  // a volume's slice
    }
  }
  return type;
}

uint32_t TextureFormat(uint32_t texture) {
  ITC_NATIVE_RUNNING("sub_822A44A8 Texture::Format");
  const TextureHeader t(texture);
  const uint32_t w40 = t.fetch_swizzle();
  const uint32_t w28 = t.fetch_tiling();
  const uint32_t w32 = t.fetch_format();
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

Extent TextureBaseSize(uint32_t texture) {
  ITC_NATIVE_RUNNING("sub_822A4348 Texture::BaseSize");
  const TextureHeader t(texture);
  const uint32_t dimension = (t.fetch_mips() >> 9) & 3;
  const uint32_t add = ((t.fetch_swizzle() >> 30) & 2) + 1;
  const uint32_t size = t.fetch_size();
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

namespace {

// PowerPC's rotate-and-mask forms on 32-bit words.
uint32_t Rlwinm(uint32_t value, uint32_t shift, uint32_t mask) {
  return std::rotl(value, int(shift)) & mask;
}
uint32_t Rlwimi(uint32_t into, uint32_t value, uint32_t shift, uint32_t mask) {
  return (std::rotl(value, int(shift)) & mask) | (into & ~mask);
}
uint32_t Sraw(uint32_t value, uint32_t shift) { return uint32_t(int32_t(value) >> shift); }

}  // namespace

TextureSizes TextureLayoutSizes(uint32_t width, uint32_t height, uint32_t depth,
                                uint32_t levels, uint32_t format, uint32_t dimension,
                                uint32_t tiled, uint32_t packed, uint32_t border, uint32_t pitch) {
  ITC_NATIVE_RUNNING("sub_822A46C8 TextureLayout::Sizes");
  // A 2D texture with a depth over 1 is an array; arrays and cube maps lay
  // out one slice and multiply (the original's r14; the stack word at +92
  // tells arrays, aligned to 4 slices, from cube maps).
  const bool array = dimension == 1 && depth > 1;
  const bool slices = array || dimension == 3;
  uint32_t level_depth = slices ? 1 : depth;  // r28
  const uint32_t bits = guest::LoadU8(kFormatTable + 1 + (format << 1));
  const uint32_t twice = border << 1;
  const uint32_t w_inner = width - twice;   // r23
  const uint32_t h_inner = height - twice;  // r26
  // The sizes' log2, rounded up, plus the border (r29, r30).
  uint32_t log_w = border - Clz(w_inner - 1) + 32;
  uint32_t log_h = border - Clz(h_inner - 1) + 32;
  uint32_t d_inner = 1;  // r24
  uint32_t log_d = 0;
  if (dimension == 2) {
    d_inner = level_depth - twice;
    log_d = border - Clz(d_inner - 1) + 32;
  }
  // The base level's width, height and depth (the stack's +88, +84, +80):
  // powers of two when packed below 16 texels, else the size (the width
  // from the pitch when one is given).
  uint32_t w = Slw(1, log_w);
  uint32_t h = Slw(1, log_h);
  uint32_t d = Slw(1, log_d);
  if (!((w <= 16 || h <= 16) && packed != 0)) {
    if (pitch == 0) {
      w = width;
    } else {
      if (bits == 0) {
        ITC_LOG_WARN("tw/td trap hit (type 0): TextureLayout::Sizes's divide by 0");
      }
      w = bits ? (pitch << 3) / bits : 0;
    }
    h = height;
    d = level_depth;
  }
  TextureLevelSize(w, h, d, bits, format, dimension, tiled);
  TextureSizes sizes{levels, w, 0, 0};
  if (pitch == 0) {
    pitch = (w * bits) >> 3;
  }
  bool mips = true;
  if (dimension == 2) {
    sizes.base_size = (pitch * d * h + 4095) & 0xFFFFF000;
    if (slices) {
      sizes.base_size *= depth;
    }
  } else if (tiled == 0 && !slices) {
    if (dimension == 1 && levels == 1 && packed == 0 && border == 0) {
      // A linear 2D texture of one level: its height aligned only to the
      // block, no mips.
      const Block block = GpuBlockSize(format);
      sizes.base_size = ((block.height + height - 1) & ~(block.height - 1)) * pitch;
      mips = false;
    } else {
      sizes.base_size = pitch * d * h;
    }
  } else {
    sizes.base_size = ((pitch * h + 4095) & 0xFFFFF000) * d;
    if (slices) {
      sizes.base_size *= depth;
    }
  }
  if (!mips || !(levels > 1 || levels == 0)) {
    return sizes;
  }
  uint32_t largest = h_inner > d_inner ? h_inner : d_inner;
  if (w_inner > largest) {
    largest = w_inner;
  }
  if (levels == 0) {
    levels = 31 - Clz(largest) + 1;
    sizes.levels = levels;
  }
  uint32_t mip_size = 0;
  for (uint32_t count = levels - 1; count != 0; count--) {
    // Packed mips end where the mip tail starts (a side at 16 texels).
    if (packed != 0 && (log_w <= 4 || log_h <= 4)) {
      break;
    }
    if (log_w != 0) {
      log_w--;
    }
    if (log_h != 0) {
      log_h--;
    }
    if (level_depth > 1) {
      level_depth >>= 1;
    }
    uint32_t mw = Slw(1, log_w);
    uint32_t mh = Slw(1, log_h);
    uint32_t md = level_depth;
    mip_size += TextureLevelSize(mw, mh, md, bits, format, dimension, tiled);
  }
  if (slices) {
    const uint32_t align = array ? 4 : 1;
    mip_size *= (depth + align - 1) & ~(align - 1);
  }
  sizes.mip_size = mip_size;
  return sizes;
}

bool TextureTypeKnown(uint32_t type) {
  return type == 3 || type == 17 || type == 18 || type == 19 || type == 20;
}

namespace {

uint32_t& Word(uint32_t* words, uint32_t offset) { return words[offset / 4]; }

// TextureHeader::Set on a host copy of the header's 13 words (as guest
// words); see SetTextureHeader.
TextureHeaderSizes SetTextureHeaderWords(uint32_t* words, uint32_t type,
                                         uint32_t width, uint32_t height, uint32_t depth,
                                         uint32_t levels, uint32_t usage, uint32_t format,
                                         uint32_t packed, uint32_t border, uint32_t field,
                                         uint32_t pitch) {
  const uint32_t bordered = (border << 1) + 1;  // r25: the sizes' bias
  // The fetch constant's dimension (0 1D, 1 2D, 2 3D, 3 cube).
  uint32_t dimension = 1;
  switch (type) {
    case 20:
      dimension = 0;
      break;
    case 18:
      dimension = 3;
      break;
    case 17:
      dimension = 2;
      break;
    default:
      break;  // 3 and 19 (TextureTypeKnown is the caller's)
  }
  const uint32_t full = TextureLevelCount(width, height, type == 17 ? depth : 1, border);
  if (levels == 0) {
    levels = full;
  }
  if (packed == 2) {
    packed = levels > 1 ? 1 : 0;
  }
  const TextureSizes sizes = TextureLayoutSizes(width, height, depth, levels,
                                                format & 0x3F, dimension, (format >> 8) & 1,
                                                packed, border, pitch);
  uint32_t common = (usage & 0x4) ? 0x00300003 : 0x00100003;
  if (usage & 0x200) {
    common |= 0x00400000;
  }
  Word(words, 20) = (kUnlocked);
  Word(words, 24) = (kUnlocked);
  Word(words, 0) = (common);
  Word(words, 8) = (0);
  uint32_t w48 = Word(words, 48);
  Word(words, 4) = (1);
  w48 = Rlwimi(w48, dimension, 9, 0x600);
  Word(words, 48) = (w48);
  // The size word (+36), as Texture::BaseSize reads it back.
  if (dimension == 0) {
    Word(words, 36) = ((Word(words, 36) & 0xFF000000) |
                                 ((width - bordered) & 0x00FFFFFF));
  } else if (dimension == 1 || dimension == 3) {
    uint32_t size = (((height - bordered) << 13) & 0x3FFE000) | ((depth - 1) << 26);
    size |= (width - bordered) & 0x1FFF;
    Word(words, 36) = (size);
  } else {
    uint32_t size = (((height - bordered) << 11) & 0x3FF800) | ((depth - bordered) << 22);
    size |= (width - bordered) & 0x7FF;
    Word(words, 36) = (size);
  }
  // The format's fields into +28, +32, +40 and +44 (the original's steps).
  uint32_t r9 = Sraw(format, 15);
  uint32_t r8 = Sraw(format, 13);
  uint32_t r7 = Sraw(format, 11);
  uint32_t r6 = Sraw(format, 9);
  uint32_t r5 = Sraw(format, 8);
  uint32_t r4 = Sraw(format, 6);
  uint32_t r3 = Sraw(format, 27) & 7;
  r8 = Rlwimi(r8, r9, 2, 0xC);
  r3 |= Rlwinm(border, 21, 0xFFE00000);
  r9 = Rlwimi(sizes.base_width, r5, 14, 0xFFFFC000);
  r3 = Rlwimi(r3, field, 3, 0x1F8);
  uint32_t r30 = Sraw(format, 24);
  r8 &= 0xF;
  r30 = Rlwimi(r30, r3, 3, 0xFFFFFFF8);
  r3 = Word(words, 28);
  r7 = Rlwimi(r7, r8, 2, 0xFFFFFFFC);
  const uint32_t is_array = type == 19 ? 1 : 0;
  r8 = Sraw(format, 21);
  r6 = Rlwimi(r6, r7, 2, 0xFFFFFFFC);
  r4 = Rlwimi(r4, is_array, 4, 0x10);
  r3 = Rlwimi(r3, r9, 17, 0xFFC00000);
  uint32_t w44 = Word(words, 44);
  r8 = Rlwimi(r8, r30, 3, 0xFFFFFFF8);
  uint32_t w32 = Word(words, 32) & 0xFFFFFFC0;
  r7 = Sraw(format, 18);
  r7 = Rlwimi(r7, r8, 3, 0xFFFFFFF8);
  w44 = Rlwimi(w44, levels - 1, 6, 0x3C0);
  w48 = Rlwimi(w48, packed, 11, 0x800);
  const uint32_t w40 = Word(words, 40);
  r5 = Sraw(format, 17);
  r6 = Rlwinm(r6, 2, 0xFFFFFFFC);
  r5 = Rlwimi(r5, r7, 1, 0xFFFFFFFE);
  Word(words, 44) = (w44);
  Word(words, 48) = (w48);
  w32 |= format & 0x3F;
  r3 &= 0xFFFFFC00;
  w32 = Rlwimi(w32, r4, 6, 0xC0);
  r6 |= r3;
  w32 = Rlwimi(w32, r4, 6, 0x400);
  Word(words, 40) = (r5 | (w40 & 0x7FF80000));
  r6 |= 2;
  Word(words, 32) = (w32);
  Word(words, 28) = (r6);
  return {r3, sizes.base_size, sizes.mip_size};
}

// The header's words TextureHeader::Set writes (+12 and +16 it leaves).
constexpr uint32_t kSetWords[] = {0, 4, 8, 20, 24, 28, 32, 36, 40, 44, 48};

}  // namespace

TextureHeaderSizes SetTextureHeader(uint32_t type, uint32_t width, uint32_t height,
                                    uint32_t depth, uint32_t levels, uint32_t usage,
                                    uint32_t format, uint32_t packed, uint32_t border,
                                    uint32_t field, uint32_t pitch, uint32_t header) {
  ITC_NATIVE_RUNNING("sub_822A5608 TextureHeader::Set");
  uint32_t words[13];
  for (uint32_t i = 0; i < 13; i++) {
    words[i] = Load(header + 4 * i);
  }
  const TextureHeaderSizes sizes = SetTextureHeaderWords(words, type, width, height, depth, levels, usage, format, packed, border, field, pitch);
  for (uint32_t offset : kSetWords) {
    Store(header + offset, Word(words, offset));
  }
  return sizes;
}

LevelLayout TextureLevelLayout(uint32_t texture, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_822A4B98 Texture::LevelLayout");
  const TextureHeader t(texture);
  const uint32_t format = t.fetch_format() & 0x3F;
  const uint32_t bits = guest::LoadU8(kFormatTable + 1 + (format << 1));
  const uint32_t type = ResourceType(texture);
  // The level's width, height and depth for TextureLevelSize (the
  // original's stack slots +88, +80 and +84; +80 is the aligned height).
  uint32_t width = 0, height = 0, depth = 0;
  uint32_t out_width, out_height, out_depth, pitch;
  // The level the layout and the alignment test use (the mip tail's for
  // packed mips).
  uint32_t at = level;
  if (type == 4) {
    const Extent size = TextureBaseSize(texture);
    const uint32_t w28 = t.fetch_tiling();
    const uint32_t dimension = (t.fetch_mips() >> 9) & 3;
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
    const uint32_t border = t.fetch_swizzle() >> 31;
    const uint32_t twice = border << 1;
    const Extent size = TextureBaseSize(texture);
    out_width = size.width;
    out_height = size.height;
    out_depth = size.depth;
    const uint32_t w48 = t.fetch_mips();
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
      const uint32_t w28 = t.fetch_tiling();
      const uint32_t tiled = w28 >> 31;
      TextureLevelSize(width, height, depth, bits, format, dimension, tiled);
      const uint32_t w32 = t.fetch_format();
      if (tiled == 0 && !(w32 & 0x400) && !packed && dimension == 1 &&
          !(t.fetch_mip_range() & 0x3C0) && border == 0) {
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
      TextureLevelSize(width, height, depth, bits, format, dimension, t.fetch_tiling() >> 31);
      pitch = (width * bits) >> 3;
    }
  }
  uint32_t slice = pitch * height;
  if (!(!(t.fetch_tiling() & 0x80000000) && at <= 1 && (type == 3 || type == 20)) &&
      (t.fetch_mips() & 0x600) != 0x400) {
    slice = (slice + 4095) & 0xFFFFF000;
  }
  return {out_width, out_height, out_depth, pitch, slice};
}

MipTailPlace MipTailOffset(uint32_t level, uint32_t width, uint32_t height,
                           uint32_t depth, uint32_t pitch, uint32_t slice, uint32_t format) {
  ITC_NATIVE_RUNNING("sub_822A4A40 TextureLayout::MipTailOffset");
  const uint32_t bits = guest::LoadU8(kFormatTable + 1 + (format << 1));
  const Block block = GpuBlockSize(format);
  const uint32_t log_w = 32 - Clz(width - 1);
  const uint32_t log_h = 32 - Clz(height - 1);
  const uint32_t log_d = 32 - Clz(depth - 1);
  const uint32_t sides[2] = {Slw(1, log_w), Slw(1, log_h)};
  // The texel offsets (the original's stack +96, +100) and the slice's.
  uint32_t texels[2] = {0, 0};
  uint32_t z = 0;
  // The level steps along the longer side first (x when the width is the
  // larger), 16 texels then halving; from level 3 along the other.
  uint32_t along = log_h >= log_w ? 0 : 1;
  if (level < 3) {
    texels[along] = uint32_t(16 >> level);
  } else {
    along ^= 1;
    const uint32_t step = Srw(sides[along], level - 2);
    texels[along] = step;
    if (step < 4) {
      const uint32_t left = log_d - level;
      z = (left > 1 ? left : 1) << 2;
    }
  }
  // Blocks are at least 1 x 1 (the original's divide traps never hit).
  MipTailPlace place;
  place.x = texels[0] / block.width;
  place.y = texels[1] / block.height;
  place.z = z;
  place.offset = ((block.width * bits * texels[0]) >> 3) + texels[1] * pitch + z * slice;
  return place;
}

LevelPlace TextureLevelOffset(uint32_t texture, uint32_t face, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_822A4FF0 Texture::LevelOffset");
  const TextureHeader t(texture);
  const uint32_t bits = guest::LoadU8(kFormatTable + 1 + ((t.fetch_format() & 0x3F) << 1));
  const uint32_t border = t.fetch_swizzle() >> 31;
  const uint32_t dimension = (t.fetch_mips() >> 9) & 3;
  const uint32_t tiled = t.fetch_tiling() >> 31;
  const uint32_t twice = border << 1;
  const Extent size = TextureBaseSize(texture);
  LevelPlace place{};
  place.width = size.width;
  place.height = size.height;
  place.depth = size.depth;
  const uint32_t w_inner = size.width - twice;
  const uint32_t h_inner = size.height - twice;
  // The base size rounded up to powers of two (border kept).
  const uint32_t pow_w = Slw(1, border - Clz(w_inner - 1) + 32);
  const uint32_t pow_h = Slw(1, border - Clz(h_inner - 1) + 32);
  const uint32_t pow_d =
      dimension == 2 ? Slw(1, border - Clz(size.depth - twice - 1) + 32) : 1;
  // The base level itself in the mip tail (packed, a side at 16 texels).
  const bool packed_base =
      (pow_w <= 16 || pow_h <= 16) && (t.fetch_mips() & 0x800) != 0;
  uint32_t start =
      ((level != 0 && !packed_base) ? t.fetch_mips() : t.fetch_format()) & 0xFFFFF000;  // r14
  uint32_t pitch;           // r30
  uint32_t aligned_height;  // r19
  uint32_t aligned_depth;   // r17
  if (level == 0 && !packed_base) {
    const uint32_t w48 = t.fetch_mips();
    const uint32_t w32 = t.fetch_format();
    const uint32_t level_dimension = (w48 >> 9) & 3;
    const uint32_t format = w32 & 0x3F;
    uint32_t w = size.width, h = size.height, d = size.depth;
    TextureLevelSize(w, h, d, bits, format, level_dimension, tiled);
    const uint32_t w28 = t.fetch_tiling();
    aligned_height = h;
    if (!(w28 & 0x80000000) && !(w32 & 0x400) && !(w48 & 0x800) && level_dimension == 1 &&
        !(t.fetch_mip_range() & 0x3C0) && !(t.fetch_swizzle() & 0x80000000)) {
      // A linear 2D texture's height aligned only to its block.
      const Block block = GpuBlockSize(format);
      aligned_height = (block.height + size.height - 1) & ~(block.height - 1);
    }
    aligned_depth = d;
    if (dimension == 0) {
      pitch = (w * bits) >> 3;
    } else {
      pitch = (((w28 >> 22) & 0x1FF) * bits << 2) & 0x1FFFFFFC;
    }
  } else {
    place.width = AtLeast1(Srw(w_inner, level)) + twice;
    place.height = AtLeast1(Srw(h_inner, level)) + twice;
    place.depth = dimension == 2 ? AtLeast1(Srw(size.depth - twice, level)) + twice : 1;
    // A cube map's faces, or an array's slices (aligned to 4 with +32 bit
    // 0x400), share each level.
    uint32_t slices = 1;  // r16
    const uint32_t type = ResourceType(texture);
    if (type == 18 || type == 19) {
      const uint32_t align = Slw(1, (t.fetch_format() >> 9) & 2);
      slices = ((t.fetch_size() >> 26) + align) & ~(align - 1);
    }
    uint32_t log_w = 31 - Clz(pow_w);
    uint32_t log_h = 31 - Clz(pow_h);
    uint32_t log_d = 31 - Clz(pow_d);
    int32_t left;  // r20: the levels still to pass
    if (packed_base) {
      left = int32_t(level);
      log_w++;
      log_h++;
      log_d++;
    } else {
      left = int32_t(level - 1);
    }
    const uint32_t w48 = t.fetch_mips();
    const uint32_t level_dimension = (w48 >> 9) & 3;
    const uint32_t level_tiled = t.fetch_tiling() >> 31;
    const uint32_t format = t.fetch_format() & 0x3F;
    pitch = 0;
    aligned_height = 0;
    aligned_depth = 0;
    // left < 0 (a level from 2^31) is the adapter's (the original).
    while (left >= 0) {
      if (log_w != 0) {
        log_w--;
      }
      if (log_h != 0) {
        log_h--;
      }
      if (log_d != 0) {
        log_d--;
      }
      const uint32_t mw = Slw(1, log_w), mh = Slw(1, log_h), md = Slw(1, log_d);
      uint32_t w = mw, h = mh, d = md;
      TextureLevelSize(w, h, d, bits, format, level_dimension, level_tiled);
      pitch = (w * bits) >> 3;
      if ((mw <= 16 || mh <= 16) && (w48 & 0x800)) {
        // The level is in the mip tail.
        aligned_height = h;
        const MipTailPlace tail =
            MipTailOffset(uint32_t(left), mw, mh, md, pitch, pitch * h, format);
        aligned_depth = d;
        place.tail = tail.offset;
        start += tail.offset;
        break;
      }
      aligned_height = h;
      aligned_depth = d;
      if (left > 0) {
        uint32_t bytes;
        if (level_dimension == 2) {
          bytes = (pitch * d * h + 4095) & 0xFFFFF000;
        } else {
          bytes = ((pitch * h + 4095) & 0xFFFFF000) * d;
        }
        start += bytes * slices;
      }
      left--;
    }
  }
  const uint32_t slice = pitch * aligned_height;
  const uint32_t aligned = (slice + 4095) & 0xFFFFF000;
  place.offset = aligned * face + start;
  place.pitch = pitch;
  if ((t.fetch_mips() & 0x600) == 0x400) {
    place.slice = slice;
    place.size = (slice * aligned_depth + 4095) & 0xFFFFF000;
  } else {
    place.slice = aligned;
    place.size = aligned * aligned_depth;
  }
  return place;
}

uint32_t TextureLevelDesc(uint32_t texture, uint32_t level, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_822A4F60 Texture::LevelDesc");
  const LevelLayout layout = TextureLevelLayout(texture, level);
  const LevelDesc d(desc);
  d.set_format(TextureFormat(texture));
  const uint32_t type = ResourceType(texture);
  d.set_type(type);
  d.set_usage(0);
  d.set_pool(0);
  d.set_multisample_quality(0);
  d.set_width(layout.width);
  d.set_height(layout.height);
  d.set_multisample(0);
  return type;
}

uint32_t SurfaceDesc(uint32_t surface, uint32_t desc) {
  ITC_NATIVE_RUNNING("sub_822A5F50 Surface::GetDesc");
  uint32_t r3 = surface;
  const SurfaceHeader s(surface);
  const LevelDesc d(desc);
  if (s.common() & 0x40000000) {
    const uint32_t level = s.level_face() >> 28;
    r3 = TextureLevelDesc(s.parent().address, level, desc);
  } else {
    const uint32_t format = s.format();
    d.set_usage(0);
    d.set_pool(0);
    d.set_multisample_quality(0);
    d.set_format(format);
    d.set_width(((s.size() >> 18) & 0x3FFF) + 1);
    d.set_height(((s.size() >> 3) & 0x7FFF) + 1);
    d.set_multisample(s.multisample() & 3);
  }
  d.set_type(4);
  return r3;
}

SurfaceSizes SetSurfaceHeader(uint32_t width, uint32_t height, uint32_t format,
                              int32_t multisample, uint32_t parameters, uint32_t header) {
  ITC_NATIVE_RUNNING("sub_822A5910 SurfaceHeader::Set");
  uint32_t low = format & 0x3F;
  const bool depth = low == 22 || low == 23;
  if (low == 54) {
    low = 7;
  }
  const SurfaceHeader surface(header);
  surface.set_ref_count(1);
  surface.set_common(4);
  surface.set_locked_base(kUnlocked);
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
    const schema::SurfaceParameters p(parameters);
    edram_base = p.edram_base();
    hi_z_base = p.hi_z_base();
    parameter8 = p.unknown_8();
  } else if (depth && G::hi_z_taken() == 0) {
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
  surface.set_format(format);
  surface.set_pitch((pitch_word << 16) | (pitch & 0x3FFF));
  const uint32_t size =
      ((width - 1) << 18) | (((height - 1) << 3) & 0x3FFF8) | (surface.size() & 7);
  surface.set_size(size);
  const uint32_t format_bits = (guest::LoadU16(kFormatTable + (low << 1)) >> 8) & 0xF;
  if (depth) {
    surface.set_edram(((format_bits & 1) << 16) | (edram_base & 0xFFF));
    if (hi_z_base == 0xFFFFFFFF) {
      surface.set_hi_z(0);
    } else {
      const uint32_t hi_z_word = (low != 22 ? 1 : 0) | (hi_z_base << 13);
      surface.set_hi_z((hi_z_word << 4) | 1);
    }
  } else {
    uint32_t bits = format_bits;
    if (bits == 0 && !(format & 0x20000) && (format & 0x600) == 0x600 &&
        (format & 0x1800) == 0x1800 && (format & 0x6000) == 0x6000 && !(format & 0x18000)) {
      bits = 1;
    }
    bits = ((parameter8 << 4) & 0x3F0) | (bits & ~0x3F0u);
    surface.set_edram(((bits << 16) & 0x3FF0000) | (edram_base & 0xFFF));
  }
  surface.set_edram_bytes(tiles * 5120);
  return {tiles, hi_z};
}

uint32_t CreateSurface(uint32_t width, uint32_t height, uint32_t format,
                       int32_t multisample, uint32_t parameters) {
  ITC_NATIVE_RUNNING("sub_822A5E28 Device::CreateSurface");
  const uint32_t header = memory::crt::XMemAlloc(48, kHeaderAttributes);
  if (header == 0) {
    return 0;
  }
  const SurfaceSizes sizes =
      SetSurfaceHeader(width, height, format, multisample, parameters, header);
  const SurfaceHeader h(header);
  h.set_common(h.common() | 0x00100000);
  // No EDRAM is taken (docs/pc-renderer.md, step 4): our renderer gives each
  // surface an image of its own, keyed by the surface object, and never
  // reads the base. The original took tiles from the EDRAM allocator
  // (graphics/edram.h, removed with it) when no `parameters` fixed the base, failing when
  // none fit, and gave the first depth surface the hi-z; the base stays 0
  // (or the parameters'), +0 bit 0x80000000 unset, so DestroySurface frees
  // none.
  (void)sizes;
  return header;
}

uint32_t CreateVertexBuffer(uint32_t bytes, uint32_t usage) {
  ITC_NATIVE_RUNNING("sub_822A6830 Device::CreateVertexBuffer");
  const uint32_t header = memory::crt::XMemAlloc(32, kHeaderAttributes);
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
  const uint32_t data = memory::crt::XMemAlloc(bytes, kDataAttributes | (protection << 28));
  if (data == 0) {
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  const BufferHeader h(header);
  h.set_common(common);
  h.set_ref_count(1);
  const uint32_t old28 = h.size();
  h.set_data(data | 3);
  h.set_locked_base(kUnlocked);
  h.set_size((bytes & 0x3FFFFFC) | (old28 & 0x8000000) | 0x10000000 | 2);
  return header;
}

uint32_t CreateIndexBuffer(uint32_t bytes, uint32_t usage, uint32_t format) {
  ITC_NATIVE_RUNNING("sub_822A6968 Device::CreateIndexBuffer");
  const uint32_t header = memory::crt::XMemAlloc(32, kHeaderAttributes);
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
  const uint32_t data = memory::crt::XMemAlloc(bytes, kDataAttributes | (protection << 28));
  if (data == 0) {
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  const BufferHeader h(header);
  h.set_size(bytes);
  h.set_common(common);
  h.set_data(data);
  h.set_ref_count(1);
  h.set_locked_base(kUnlocked);
  return header;
}

uint32_t CreateTexture(uint32_t width, uint32_t height, uint32_t depth,
                       uint32_t levels, uint32_t usage, uint32_t format, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_822A5D08 Device::CreateTexture");
  const uint32_t header = memory::crt::XMemAlloc(52, kHeaderAttributes);
  if (header == 0) {
    return 0;
  }
  const TextureHeaderSizes sizes = SetTextureHeader(type, width, height, depth, levels,
                                                    usage, format, 2, 0, 0, 0, header);
  // Physical, the protection field 3, or 2 (write-combined) with usage 0x4.
  const uint32_t attributes = 0x8C800000 | (((~usage >> 2) & 1) | 2) << 28;
  const uint32_t data = memory::crt::XMemAlloc(sizes.base_size, attributes);
  if (data == 0) {
    memory::crt::XMemFree(header, kHeaderFreeAttributes);
    return 0;
  }
  uint32_t mips = 0;
  if (sizes.mip_size != 0) {
    mips = memory::crt::XMemAlloc(sizes.mip_size, attributes);
    if (mips == 0) {
      memory::crt::XMemFree(header, kHeaderFreeAttributes);
      memory::crt::XMemFree(data, 0xB1800000);
      return 0;
    }
  }
  const TextureHeader h(header);
  h.set_fetch_format((h.fetch_format() & 0xFFF) | (data & 0xFFFFF000));
  h.set_fetch_mips((h.fetch_mips() & 0xFFF) | (mips & 0xFFFFF000));
  return header;
}

uint32_t GetSurfaceLevel(uint32_t texture, uint32_t level) {
  ITC_NATIVE_RUNNING("sub_822A5B80 Texture::GetSurfaceLevel");
  const uint32_t surface = memory::crt::XMemAlloc(48, kHeaderAttributes);
  if (surface == 0) {
    return 0;
  }
  const SurfaceHeader s(surface);
  const uint32_t common = 0x40100004 | (TextureHeader(texture).common() & 0x200000);
  s.set_parent(texture);
  s.set_common(common);
  const uint32_t w28 = s.level_face();
  s.set_ref_count(1);
  s.set_locked_base(kUnlocked);
  s.set_level_face((w28 & 0x003FFFFF) | (level << 28));
  refcount::AddRef(texture);
  return surface;
}

uint32_t XGSetTextureHeader(uint32_t width, uint32_t height, uint32_t levels,
                            uint32_t usage, uint32_t format, uint32_t base_address,
                            uint32_t mip_address, uint32_t pitch, uint32_t header,
                            uint32_t base_size_out, uint32_t mip_size_out) {
  ITC_NATIVE_RUNNING("sub_8235A920 XGSetTextureHeader");
  XgTextureHeader args{};
  args.width = width;
  args.height = height;
  args.depth = 1;
  args.levels = levels;
  args.usage = usage;
  args.format = format;
  args.packed = 2;
  args.border = 0;
  args.field = 0;
  args.base_address = base_address;
  args.mip_address = mip_address;
  args.pitch = pitch;
  args.type = 3;
  args.header = header;
  args.base_size_out = base_size_out;
  args.mip_size_out = mip_size_out;
  return XGSetTextureHeaderEx(args);
}

bool XgHeaderCopyFits(uint32_t header) {
  return header == 0 || (header < 0xA0000000u && c_runtime::InOneView(header, 52));
}

uint32_t XGSetTextureHeaderEx(const XgTextureHeader& a) {
  ITC_NATIVE_RUNNING("sub_8235A720 XGSetTextureHeaderEx");
  const Block block = GpuBlockSize(a.format & 0x3F);
  // A single level (one asked, or the full chain of a 1 x 1 x 1 texture,
  // 3 x 3 x 3 with a border) of a 2D texture takes the pitch, in blocks.
  bool single = a.levels == 1;
  if (a.levels == 0) {
    if (a.width == 1 && a.height == 1 && a.depth == 1 && a.border == 0) {
      single = true;
    } else if (a.width == 3 && a.height == 3 && a.depth == 3 && a.border != 0) {
      single = true;
    }
  }
  // Over the block's height (the original's BlockSize's second output).
  const uint32_t pitch = single && a.type == 3 ? a.pitch / block.height : 0;
  // The header, built in a host copy of the original's zeroed stack block.
  uint32_t words[13] = {};
  const TextureHeaderSizes sizes =
      SetTextureHeaderWords(words, a.type, a.width, a.height, a.depth, a.levels, a.usage,
                            a.format, a.packed, a.border, a.field, pitch);
  Word(words, 32) = (a.base_address & 0xFFFFF000) | (Word(words, 32) & 0xFFF);
  Word(words, 4) = 1;
  uint32_t common = 3;
  if (a.usage & 0x4) {
    common = 0x00200003;
  }
  if (a.usage & 0x200) {
    common |= 0x00400000;
  }
  Word(words, 0) = common;
  Word(words, 20) = kUnlocked;
  Word(words, 24) = kUnlocked;
  uint32_t base_size = sizes.base_size;
  if (sizes.mip_size != 0) {
    if (a.mip_address == 0xFFFFFFFF) {
      // The mips right after the base data.
      base_size = (sizes.base_size + 4095) & 0xFFFFF000;
      Word(words, 48) = ((base_size + a.base_address) & ~0xFFFu) | (Word(words, 48) & 0xFFF);
    } else {
      Word(words, 48) = (a.mip_address & 0xFFFFF000) | (Word(words, 48) & 0xFFF);
    }
  } else {
    Word(words, 48) &= 0xFFF;
  }
  if (a.base_size_out != 0) {
    Store(a.base_size_out, base_size);
  }
  if (a.mip_size_out != 0) {
    Store(a.mip_size_out, sizes.mip_size);
  }
  if (a.header != 0) {  // the original's memcpy (XgHeaderCopyFits)
    for (uint32_t i = 0; i < 13; i++) {
      Store(a.header + 4 * i, words[i]);
    }
  }
  return base_size + sizes.mip_size;
}

namespace {

// A device's slot, AddRef'ed when set.
uint32_t Current(uint32_t slot) {
  const uint32_t resource = Load(slot);
  if (resource != 0) {
    refcount::AddRef(resource);
  }
  return resource;
}

}  // namespace

uint32_t GetRenderTarget(uint32_t device, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_822A3998 Device::GetRenderTarget");
  return Current(schema::Device(device).render_target0_address() + 4 * index);
}

uint32_t GetDepthStencilSurface(uint32_t device) {
  ITC_NATIVE_RUNNING("sub_822A39E0 Device::GetDepthStencilSurface");
  return Current(schema::Device(device).depth_surface_address());
}

StreamSource GetStreamSource(uint32_t device, uint32_t stream) {
  ITC_NATIVE_RUNNING("sub_822A3880 Device::GetStreamSource");
  const schema::Device d(device);
  StreamSource source{Load(d.streams_address() + 4 * stream), 0, 0};
  uint32_t offset = 0;
  if (source.buffer != 0) {
    // The stream's fetch constant's address, less the buffer's (+24's low
    // 29 bits, in the view its top 12 bits say).
    const uint32_t fetch = Load(d.fetch_constants_address() + 760 - 8 * stream);
    const uint32_t data = BufferHeader(source.buffer).data();
    offset = fetch - ((((data >> 20) & 0xFFF) + 512) & 0x1000) - (data & 0x1FFFFFFF);
    refcount::AddRef(source.buffer);
  }
  source.offset = offset << 2;
  source.stride = uint32_t(guest::LoadU8(d.stream_strides_address() + stream)) << 2;
  return source;
}

namespace {

// Adds `delta` to the big-endian word at `address` atomically (the
// library's lwarx/stwcx loop, without the recompiler's global lock, as
// src/native/refcount.cpp does); returns the old value.
uint32_t AddToWord(uint32_t address, uint32_t delta) {
  uint32_t* word = reinterpret_cast<uint32_t*>(guest::Pointer(address));
  uint32_t old_raw = __atomic_load_n(word, __ATOMIC_RELAXED);
  uint32_t value;
  do {
    value = __builtin_bswap32(old_raw);
  } while (!__atomic_compare_exchange_n(word, &old_raw, __builtin_bswap32(value + delta), false,
                                        __ATOMIC_SEQ_CST, __ATOMIC_RELAXED));
  return value;
}

constexpr uint32_t kLockCount = 256;  // +0's lock count step (bits 8-11)

// An address in the other view of physical memory (0x40000000 below the
// one at the same place).
uint32_t OtherView(uint32_t address) {
  return ((((address >> 20) & 0xFFF) + 512) & 0x1000) + (address & 0x1FFFFFFF) - 0x40000000;
}

}  // namespace

uint32_t LockResource(uint32_t resource, uint32_t mips, uint32_t data,
                      uint32_t mip_data, uint32_t address, uint32_t size, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A63F8 Resource::Lock");
  // The fence wait and the cache flush packet are Dropped (resources.h).
  const uint32_t other_view = flags & 0x10;
  const ResourceHeader r(resource);
  if (other_view || (r.common() & 0x200000)) {
    if (!(flags & 1)) {
      uint32_t offset, slot;
      if (mips != 0 && mip_data != 0) {
        offset = address - mip_data;
        slot = r.locked_mips_address();
      } else {
        offset = address - data;
        slot = r.locked_base_address();
      }
      const uint32_t range = Load(slot);
      uint32_t end = (offset + size + 127) >> 7;
      const uint32_t old_end = range & 0xFFFF;
      uint32_t start = range >> 16;
      if (end <= old_end) {
        end = old_end;
      }
      const uint32_t new_start = offset >> 7;
      if (new_start < start) {
        start = new_start;
      }
      Store(slot, ((start << 16) & 0xFFFF0000) | end);
    }
    if (other_view) {
      address = OtherView(address);
    }
  }
  AddToWord(r.common_address(), kLockCount);
  return address;
}

uint32_t UnlockResource(uint32_t resource, uint32_t data, uint32_t mip_data) {
  ITC_NATIVE_RUNNING("sub_822A6630 Resource::Unlock");
  const ResourceHeader r(resource);
  const uint32_t old = AddToWord(r.common_address(), 0u - kLockCount);
  uint32_t r3 = resource;
  if ((old & 0xF00) != 0x100) {
    return r3;
  }
  // The last lock: the ranges reset (their data cache flush Dropped; r3 is
  // the flush's start address, as the original leaves it).
  auto reset = [&](uint32_t slot, uint32_t start_address) {
    const uint32_t range = Load(slot);
    if (range == kUnlocked) {
      return;
    }
    const uint32_t common = r.common();
    Store(slot, kUnlocked);
    if (!(common & 0x200000)) {
      start_address = OtherView(start_address);
    }
    r3 = ((range >> 16) << 7) + start_address;
  };
  reset(r.locked_base_address(), data);
  if (mip_data != 0) {
    reset(r.locked_mips_address(), mip_data);
  }
  return r3;
}

LevelLock LockTextureLevel(uint32_t texture, uint32_t face, uint32_t level,
                           uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A54B0 Texture::LockLevel");
  const LevelPlace place = TextureLevelOffset(texture, face, level);
  const TextureHeader t(texture);
  const Block block = GpuBlockSize(t.fetch_format() & 0x3F);
  LevelLock lock;
  lock.pitch = place.pitch * block.height;
  lock.slice = place.slice;
  lock.tail = place.tail;
  lock.r3 = LockResource(texture, level, t.fetch_format() & 0xFFFFF000,
                         t.fetch_mips() & 0xFFFFF000, place.offset - place.tail,
                         place.size, flags);
  lock.address = place.tail + lock.r3;
  return lock;
}

uint32_t LockTextureRect(uint32_t texture, uint32_t face, uint32_t level,
                         uint32_t locked, uint32_t rect, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A5578 Texture::LockRect");
  const LevelLock lock = LockTextureLevel(texture, face, level, flags);
  uint32_t bits = lock.address;
  if (rect != 0) {
    const uint32_t format = TextureHeader(texture).fetch_format() & 0x3F;
    const uint32_t pixel_bits = guest::LoadU8(kFormatTable + 1 + (format << 1));
    const schema::GpuLockRect area(rect);
    bits = ((pixel_bits * area.left()) >> 3) + area.top() * lock.pitch + lock.address;
  }
  const schema::LockedRect out(locked);
  out.set_pitch(lock.pitch);
  out.set_bits(bits);
  return lock.r3;
}

uint32_t LockTextureRectLevel(uint32_t texture, uint32_t level, uint32_t locked,
                              uint32_t rect, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A5C38 Texture::LockRectLevel");
  return LockTextureRect(texture, 0, level, locked, rect, flags);
}

uint32_t UnlockTexture(uint32_t texture) {
  ITC_NATIVE_RUNNING("sub_822A43E0 Texture::Unlock");
  const TextureHeader t(texture);
  return UnlockResource(texture, t.fetch_format() & 0xFFFFF000, t.fetch_mips() & 0xFFFFF000);
}

uint32_t UnlockSurface(uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_822A43F8 Surface::Unlock");
  const TextureHeader t = SurfaceHeader(surface).parent();
  return UnlockResource(t.address, t.fetch_format() & 0xFFFFF000,
                        t.fetch_mips() & 0xFFFFF000);
}

uint32_t LockSurfaceRect(uint32_t surface, uint32_t locked, uint32_t rect,
                         uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A5FE8 Surface::LockRect");
  const SurfaceHeader s(surface);
  const uint32_t w28 = s.level_face();
  return LockTextureRect(s.parent().address, (w28 >> 22) & 0x3F, w28 >> 28, locked, rect,
                         flags);
}

uint32_t LockVertexBuffer(uint32_t buffer, uint32_t offset, uint32_t size,
                          uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A68F8 VertexBuffer::Lock");
  if (size == 0) {
    size = BufferHeader(buffer).size() & 0x3FFFFFC;
    offset = 0;
  }
  const uint32_t data = BufferHeader(buffer).data() & 0xFFFFFFFC;
  return LockResource(buffer, 0, data, 0, data + offset, size, flags);
}

uint32_t LockIndexBuffer(uint32_t buffer, uint32_t offset, uint32_t size,
                         uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_822A6A18 IndexBuffer::Lock");
  if (size == 0) {
    size = BufferHeader(buffer).size();
    offset = 0;
  }
  const uint32_t data = BufferHeader(buffer).data();
  return LockResource(buffer, 0, data, 0, data + offset, size, flags | 2);
}

uint32_t DestroySurface(uint32_t surface, uint32_t common) {
  ITC_NATIVE_RUNNING("sub_822A6268 Resource::DestroySurface");
  if (!(common & 0x40000000)) {
    // (Its EDRAM, +0 bit 0x80000000, was freed here: surfaces take none
    // since docs/pc-renderer.md step 4.)
    if (SurfaceHeader(surface).hi_z() & 1) {
      G::set_hi_z_taken(0);
    }
  }
  return memory::crt::XMemFree(surface, kHeaderFreeAttributes);
}

uint32_t UnlockVertexBuffer(uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_822A6958 VertexBuffer::Unlock");
  return UnlockResource(buffer, BufferHeader(buffer).data() & 0xFFFFFFFC, 0);
}

uint32_t UnlockIndexBuffer(uint32_t buffer) {
  ITC_NATIVE_RUNNING("sub_822A6A78 IndexBuffer::Unlock");
  return UnlockResource(buffer, BufferHeader(buffer).data(), 0);
}

uint32_t DestroyResource(uint32_t resource) {
  ITC_NATIVE_RUNNING("sub_822A6210 Resource::Destroy");
  // The fence waits are Dropped (resources.h).
  constexpr uint32_t kDataFreeAttributes = 0xB1800000;
  const ResourceHeader r(resource);
  const uint32_t common = r.common();
  switch (common & 0xF) {
    case 1:
      memory::crt::XMemFree(r.data() & 0xFFFFFFFC, kDataFreeAttributes);
      break;
    case 2:
    case 7:
    case 8:
      memory::crt::XMemFree(r.data(), kDataFreeAttributes);
      break;
    case 3: {
      const uint32_t mips = r.fetch_mips() & 0xFFFFF000;
      memory::crt::XMemFree(r.fetch_format() & 0xFFFFF000, kDataFreeAttributes);
      memory::crt::XMemFree(mips, kDataFreeAttributes);
      break;
    }
    case 4:
      return DestroySurface(resource, common);
    case 6:
      memory::crt::XMemFree(r.fetch_mips(), kDataFreeAttributes);
      break;
    default:
      break;  // 5 and the others: the header only (9 is the adapter's)
  }
  return memory::crt::XMemFree(resource, kHeaderFreeAttributes);
}

}  // namespace graphics
