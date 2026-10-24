// The graphics library's resources (docs/functions/graphics-resources.md):
// see resources.h.

#include "graphics/resources.h"

#include "guest.h"

#include "base/log.h"

namespace graphics {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }

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

}  // namespace graphics
