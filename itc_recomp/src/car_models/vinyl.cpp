// The vinyl sheet's texture (see vinyl.h), call for call as the recompiled
// functions.

#include "car_models/vinyl.h"

#include "base/log.h"
#include "graphics/resources.h"
#include "guest.h"
#include "memory/heap.h"
#include "models/xpr.h"
#include "texture_compression/compress_surface.h"

#include <cstdlib>
#include <cstring>

namespace car_models {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

// memset (sub_825A71D0's plain work, as its adapter does it).
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(base, address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

// XPhysicalAlloc's protection with X_MEM_LARGE_PAGES cleared when
// [0x829537B0] isn't 0 (its adapter's, src/hooks/heap.cpp).
uint32_t PhysicalProtection(uint8_t* base, uint32_t protection) {
  if (Load(base, 0x829537B0) != 0) {
    protection &= ~0x20000000u;
  }
  return protection;
}

}  // namespace

uint32_t TextureCreate(uint8_t* base, uint32_t format, uint32_t width, uint32_t height,
                       uint32_t header_out, uint32_t data_out, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECF08 Texture::Create");
  const uint32_t frame = stack - kTextureCreateFrameBytes;
  // VirtualAlloc(0, 52, MEM_COMMIT, PAGE_READWRITE), then memset.
  Store(base, header_out, memory::AllocPages(52, 4096));
  Clear(base, Load(base, header_out), 52);
  // XGSetTextureHeader: its stack words, in the original's order.
  const uint32_t header = Load(base, header_out);
  Store(base, frame + 92, header);
  Store(base, frame + 108, 0);
  Store(base, frame + 100, 0);
  Store(base, frame + 84, 0);
  Require(graphics::XgHeaderCopyFits(header), "XGSetTextureHeader");
  const uint32_t size =
      graphics::XGSetTextureHeader(base, width, height, 1, 2, format, 0, 0, 0, header, 0, 0);
  // XPhysicalAlloc(size, -1, 0, 0x404), then memset.
  const uint32_t data =
      memory::AllocPhysical(size, 0, 0xFFFFFFFF, 0, PhysicalProtection(base, 0x404));
  Store(base, data_out, data);
  Clear(base, data, size);
  models::RelocateResource(base, Load(base, header_out), Load(base, data_out));
  return 1;
}

bool TextureFreeable(uint8_t* base, uint32_t header_in) {
  const uint32_t header = Load(base, header_in);
  return header == 0 || memory::InRange(header);
}

uint32_t TextureFree(uint8_t* base, uint32_t header_in, uint32_t data_in) {
  ITC_NATIVE_RUNNING("sub_820ECFC8 Texture::Free");
  if (const uint32_t header = Load(base, header_in); header != 0) {
    memory::FreePages(header);  // VirtualFree(header, 0, MEM_RELEASE)
    Store(base, header_in, 0);
  }
  const uint32_t data = Load(base, data_in);
  if (data != 0) {
    memory::FreePhysical(data);  // XPhysicalFree
    Store(base, data_in, 0);
  }
  return data;
}

std::optional<uint32_t> VinylCompress(uint8_t* base, uint32_t width, uint32_t height,
                                      uint32_t header_in, uint32_t data_in, uint32_t stack,
                                      const VinylCalls& calls) {
  if (!TextureFreeable(base, header_in)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED090 Vinyl::Compress");
  const uint32_t f = stack - kVinylCompressFrameBytes;
  for (uint32_t offset = 112; offset <= 132; offset += 4) {
    Store(base, f + offset, 0);
  }
  TextureCreate(base, kVinylFormat, width, height, f + 112, f + 116, f);
  const uint32_t texture = Load(base, f + 112);  // r26
  const uint32_t made = graphics::GetSurfaceLevel(base, texture, 0);  // r30
  graphics::LockSurfaceRect(base, made, f + 128, 0, 0);
  const uint32_t sheet = graphics::GetSurfaceLevel(base, Load(base, header_in), 0);  // r29
  graphics::LockSurfaceRect(base, sheet, f + 120, 0, 16);
  for (uint32_t offset = 144; offset < 176; offset += 4) {
    Store(base, f + offset, 0);
  }
  graphics::SurfaceDesc(base, made, f + 144);
  for (uint32_t offset = 176; offset < 208; offset += 4) {
    Store(base, f + offset, 0);
  }
  graphics::SurfaceDesc(base, sheet, f + 176);
  // XGCompressSurface: its stack words, in the original's order.
  Store(base, f + 100, 1);
  Store(base, f + 92, 0);
  Store(base, f + 84, kSheetFormat);
  const texture_compression::CompressArguments arguments{
      Load(base, f + 132),  Load(base, f + 128), Load(base, f + 168),
      Load(base, f + 172),  kVinylFormat,        0,
      Load(base, f + 124),  Load(base, f + 120),
      double(guest::LoadFloat(base, kVinylAlphaReference))};
  Require(texture_compression::CompressSurface(base, arguments, f, calls.compress).has_value(),
          "XGCompressSurface");
  graphics::UnlockSurface(base, made);
  graphics::UnlockSurface(base, sheet);
  calls.release(made, 0);
  calls.release(sheet, 1);
  const uint32_t r3 = TextureFree(base, header_in, data_in);
  Store(base, header_in, texture);
  Store(base, data_in, Load(base, f + 116));
  return r3;
}

}  // namespace car_models
