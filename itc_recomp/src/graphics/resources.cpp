// The graphics library's resources (docs/functions/graphics-resources.md):
// see resources.h.

#include "graphics/resources.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "native/refcount.h"

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

}  // namespace graphics
