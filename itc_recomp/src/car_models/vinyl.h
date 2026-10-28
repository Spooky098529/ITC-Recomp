// The vinyl sheet's texture as plain C++ (docs/functions/car-models.md:
// Texture::Create, Texture::Free and Vinyl::Compress), over the allocation
// layer (memory/heap.h), the graphics library's plain C++
// (graphics/resources.h, models/xpr.h) and XGCompressSurface's
// (texture_compression/compress_surface.h). They allocate and free, so they
// run natively only while --native_heap routes the game's heap to the layer
// (hooks::NativeHeapActive(), the adapters' condition). Adapters:
// src/hooks/car_models.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "texture_compression/codecs.h"

namespace car_models {

// Texture::Create's and Vinyl::Compress's stack frames.
constexpr uint32_t kTextureCreateFrameBytes = 160;
constexpr uint32_t kVinylCompressFrameBytes = 272;

// The formats Vinyl::Compress converts between: the sheet (A8R8G8B8) and
// the texture it makes (DXT4/5).
constexpr uint32_t kSheetFormat = 0x18280086;
constexpr uint32_t kVinylFormat = 0x1A200054;
// The alpha reference Vinyl::Compress hands XGCompressSurface (a float, 0.0).
constexpr uint32_t kVinylAlphaReference = 0x82000A34;

// sub_820ECF08 Texture::Create: a 52-byte header (VirtualAlloc: the layer's
// pages) stored at `header_out` and cleared, set by XGSetTextureHeader (one
// level, usage 2, `format`, no addresses or pitch); its data
// (XPhysicalAlloc(size, -1, 0, 0x404)) stored at `data_out` and cleared; the
// header relocated to it (Resource::Relocate). `stack` is the guest r1 at
// entry: XGSetTextureHeader's stack words (+84 the pitch, +92 the header,
// +100, +108 the sizes' pointers) written in its frame as the original's.
// Returns 1 (the original's r3). Each word is read back from guest memory
// where the original reloads it.
uint32_t TextureCreate(uint8_t* base, uint32_t format, uint32_t width, uint32_t height,
                       uint32_t header_out, uint32_t data_out, uint32_t stack);

// sub_820ECFC8 Texture::Free: the header at `header_in` (VirtualFree) and
// the data at `data_in` (XPhysicalFree) freed where not 0, each word cleared
// after. Returns the original's r3 with the native heap's adapters: the data
// word (XPhysicalFree's adapter leaves its address), 0 without data.
// TextureFreeable says whether it runs natively: a header that's 0 or in the
// layer's range (VirtualFree's adapter runs the original's otherwise).
bool TextureFreeable(uint8_t* base, uint32_t header_in);
uint32_t TextureFree(uint8_t* base, uint32_t header_in, uint32_t data_in);

// Vinyl::Compress's guest calls: XGCompressSurface's (the codecs' vtables,
// through texture_compression::GuestCall) and Resource::Release
// (sub_822A67B0) on the new texture's surface (`which` 0) and then the
// sheet's (1), from its frame.
struct VinylCalls {
  texture_compression::GuestCall compress;
  std::function<void(uint32_t surface, int which)> release;
};

// sub_820ED090 Vinyl::Compress: a DXT5 texture of `width` x `height`
// (Texture::Create, into its frame's +112 and +116), the two textures'
// level 0 surfaces (Texture::GetSurfaceLevel) locked (Surface::LockRect: the
// new one into +128, the sheet at `header_in` into +120 with flag 0x10) and
// described (Surface::GetDesc into +144 and +176, cleared first), the sheet
// compressed into the new one (XGCompressSurface: the new surface's size,
// no point, the source A8R8G8B8 with no rectangle and flags 1, the alpha
// reference at 0x82000A34; +84, +92, +100 its stack words), both unlocked
// and released, the sheet freed (Texture::Free) and the new texture's header
// and data stored at `header_in` and `data_in`. `stack` is the guest r1 at
// entry (its frame is `stack` - 272). Returns the original's r3
// (Texture::Free's); nullopt, with nothing done, where a callee would go to
// the original (the sheet's header outside the layer's range).
std::optional<uint32_t> VinylCompress(uint8_t* base, uint32_t width, uint32_t height,
                                      uint32_t header_in, uint32_t data_in, uint32_t stack,
                                      const VinylCalls& calls);

}  // namespace car_models
