// The graphics library's resources as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in
// docs/functions/graphics-resources.md, the layouts in docs/known-data.md:
// "texture header", "surface header", "buffer header"): the math the game
// does on texture and surface headers, the buffers' creation, the device's
// current targets and stream, and the EDRAM allocator. The headers are guest
// memory shared with the recompiled code (and read by our renderer), so
// these work on guest memory. Adapters: src/hooks/graphics_resources.cpp.

#pragma once

#include <cstdint>

namespace graphics {

// --- Headers (leaves) ---

// sub_822A6180 Resource::GetType: the resource's type (D3DRTYPE, probably)
// from +0's low 4 bits, refined for textures (3) by their dimension and for
// a texture's surfaces (4) by their parent's.
uint32_t ResourceType(uint8_t* base, uint32_t resource);

// sub_822A44A8 Texture::Format: the format word rebuilt from the fetch
// constant's +28, +32 and +40.
uint32_t TextureFormat(uint8_t* base, uint32_t texture);

// sub_822A4410 GpuFormat::BlockSize: a GPU texture format's block, in texels.
struct Block {
  uint32_t width;
  uint32_t height;
};
Block GpuBlockSize(uint32_t format);

// sub_822A4348 Texture::BaseSize: the base level's size from the fetch
// constant's +36 by the dimension (+48), plus 1 (3 with +40's top bit).
// The original reloads +36 after each store to its outputs; this reads it
// once (the outputs never point into the header).
struct Extent {
  uint32_t width;
  uint32_t height;
  uint32_t depth;
};
Extent TextureBaseSize(uint8_t* base, uint32_t texture);

// sub_822A4530 TextureLayout::LevelCount: 1 + log2 of the largest of the
// sizes less twice the border (the depth at least 1).
uint32_t TextureLevelCount(uint32_t width, uint32_t height, uint32_t depth, uint32_t border);

// sub_8209AD68 Surface::EdramTiles: the 5,120-byte EDRAM tiles a surface of
// this size, format and multisample type takes.
uint32_t SurfaceEdramTiles(uint32_t width, uint32_t height, uint32_t format, int32_t multisample);

// --- Buffers (the allocation layer: run only with --native_heap) ---

// sub_822A6830 Device::CreateVertexBuffer: a 32-byte header and `bytes` of
// physical memory (write-combined with usage 0x4); 0 when either fails.
uint32_t CreateVertexBuffer(uint8_t* base, uint32_t bytes, uint32_t usage);

// sub_822A6968 Device::CreateIndexBuffer: the same with the index format in
// the header's top 3 bits.
uint32_t CreateIndexBuffer(uint8_t* base, uint32_t bytes, uint32_t usage, uint32_t format);

// --- The device's current resources (AddRef'ed when set) ---

// sub_822A3998 Device::GetRenderTarget: device +12184 + 4 x index.
uint32_t GetRenderTarget(uint8_t* base, uint32_t device, uint32_t index);

// sub_822A39E0 Device::GetDepthStencilSurface: device +12200.
uint32_t GetDepthStencilSurface(uint8_t* base, uint32_t device);

// sub_822A3880 Device::GetStreamSource: the stream's buffer (device +12204 +
// 4 x stream), its offset (from the stream's fetch constant, 0 with no
// buffer) and stride.
struct StreamSource {
  uint32_t buffer;
  uint32_t offset;
  uint32_t stride;
};
StreamSource GetStreamSource(uint8_t* base, uint32_t device, uint32_t stream);

// --- Destruction (the allocation layer: run only with --native_heap) ---

// sub_822A6268 Resource::Destroy's surface case: a render target's EDRAM
// freed (+0 bit 0x80000000; the base +28's low 12 bits, the tiles +44 /
// 5120), the hi-z taken back (+32 bit 0: 0x829537C4 = 0), but for a
// texture's surface (+0 bit 0x40000000); then the header freed. `common` is
// the header's +0 as the caller read it; returns XMemFree's r3.
uint32_t DestroySurface(uint8_t* base, uint32_t surface, uint32_t common);

}  // namespace graphics
