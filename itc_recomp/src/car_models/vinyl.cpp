// The vinyl sheet's texture (see vinyl.h), call for call as the recompiled
// functions.

#include "car_models/vinyl.h"

#include "base/log.h"
#include "graphics/resources.h"
#include "guest.h"
#include "memory/heap.h"
#include "models/xpr.h"
#include "schema/car_models.h"
#include "texture_compression/compress_surface.h"

#include <cstdlib>
#include <cstring>

namespace car_models {

namespace {

using schema::CarModelsAddressWord;

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}

void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("Car models: {} refused after its checks", what);
    std::abort();
  }
}

// memset (sub_825A71D0's plain work, as its adapter does it).
void Clear(uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(address, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

// XPhysicalAlloc's protection with X_MEM_LARGE_PAGES cleared when
// [0x829537B0] isn't 0 (its adapter's, src/hooks/heap.cpp).
uint32_t PhysicalProtection(uint32_t protection) {
  if (Load(0x829537B0) != 0) {
    protection &= ~0x20000000u;
  }
  return protection;
}

}  // namespace

uint32_t TextureCreate(uint32_t format, uint32_t width, uint32_t height,
                       uint32_t header_out, uint32_t data_out, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ECF08 Texture::Create");
  (void)stack;  // no guest frame (E5 step 6): nothing here calls guest code
  const CarModelsAddressWord header_word(header_out), data_word(data_out);
  // VirtualAlloc(0, 52, MEM_COMMIT, PAGE_READWRITE), then memset.
  header_word.set_value(memory::AllocPages(52, 4096));
  Clear(header_word.value(), 52);
  // XGSetTextureHeader (its stack words, which the original wrote in its
  // frame, are its arguments: no longer stored, E5 step 6).
  const uint32_t header = header_word.value();
  Require(graphics::XgHeaderCopyFits(header), "XGSetTextureHeader");
  const uint32_t size =
      graphics::XGSetTextureHeader(width, height, 1, 2, format, 0, 0, 0, header, 0, 0);
  // XPhysicalAlloc(size, -1, 0, 0x404), then memset.
  const uint32_t data =
      memory::AllocPhysical(size, 0, 0xFFFFFFFF, 0, PhysicalProtection(0x404));
  data_word.set_value(data);
  Clear(data, size);
  models::RelocateResource(header_word.value(), data_word.value());
  return 1;
}

bool TextureFreeable(uint32_t header_in) {
  const uint32_t header = CarModelsAddressWord(header_in).value();
  return header == 0 || memory::InRange(header);
}

uint32_t TextureFree(uint32_t header_in, uint32_t data_in) {
  ITC_NATIVE_RUNNING("sub_820ECFC8 Texture::Free");
  const CarModelsAddressWord header_word(header_in), data_word(data_in);
  if (const uint32_t header = header_word.value(); header != 0) {
    memory::FreePages(header);  // VirtualFree(header, 0, MEM_RELEASE)
    header_word.set_value(0);
  }
  const uint32_t data = data_word.value();
  if (data != 0) {
    memory::FreePhysical(data);  // XPhysicalFree
    data_word.set_value(0);
  }
  return data;
}

std::optional<uint32_t> VinylCompress(uint32_t width, uint32_t height,
                                      uint32_t header_in, uint32_t data_in, uint32_t stack,
                                      const VinylCalls& calls) {
  if (!TextureFreeable(header_in)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ED090 Vinyl::Compress");
  const uint32_t f = stack - kVinylCompressFrameBytes;
  for (uint32_t offset = 112; offset <= 132; offset += 4) {
    Store(f + offset, 0);
  }
  TextureCreate(kVinylFormat, width, height, f + 112, f + 116, f);
  const uint32_t texture = Load(f + 112);  // r26
  const uint32_t made = graphics::GetSurfaceLevel(texture, 0);  // r30
  graphics::LockSurfaceRect(made, f + 128, 0, 0);
  const uint32_t sheet =
      graphics::GetSurfaceLevel(CarModelsAddressWord(header_in).value(), 0);  // r29
  graphics::LockSurfaceRect(sheet, f + 120, 0, 16);
  for (uint32_t offset = 144; offset < 176; offset += 4) {
    Store(f + offset, 0);
  }
  graphics::SurfaceDesc(made, f + 144);
  for (uint32_t offset = 176; offset < 208; offset += 4) {
    Store(f + offset, 0);
  }
  graphics::SurfaceDesc(sheet, f + 176);
  // XGCompressSurface: its stack words, in the original's order.
  Store(f + 100, 1);
  Store(f + 92, 0);
  Store(f + 84, kSheetFormat);
  const texture_compression::CompressArguments arguments{
      Load(f + 132),  Load(f + 128), Load(f + 168),
      Load(f + 172),  kVinylFormat,        0,
      Load(f + 124),  Load(f + 120),
      double(guest::LoadFloat(kVinylAlphaReference))};
  Require(texture_compression::CompressSurface(arguments, f, calls.compress).has_value(),
          "XGCompressSurface");
  graphics::UnlockSurface(made);
  graphics::UnlockSurface(sheet);
  calls.release(made, 0);
  calls.release(sheet, 1);
  const uint32_t r3 = TextureFree(header_in, data_in);
  CarModelsAddressWord(header_in).set_value(texture);
  CarModelsAddressWord(data_in).set_value(Load(f + 116));
  return r3;
}

}  // namespace car_models
