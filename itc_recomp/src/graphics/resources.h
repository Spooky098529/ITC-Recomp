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

// sub_822A45B8 TextureLayout::LevelSize: a level's width, height and depth
// aligned in place (the width to 32 blocks, or when linear to 256 bytes a
// row and at least 32 blocks; the height to 32 blocks; the depth to 4 for
// 3D, dimension 2) and its bytes (4 KiB aligned: the whole for 3D, else a
// slice's times the depth). `bits` = bits a pixel. A linear format whose
// block has no bytes hits the original's divide trap (logged, as the
// translated code does, then 0).
uint32_t TextureLevelSize(uint32_t& width, uint32_t& height, uint32_t& depth, uint32_t bits,
                          uint32_t format, uint32_t dimension, uint32_t tiled);

// sub_822A4B98 Texture::LevelLayout: a level's width, height and depth
// (the base size halved per level, at least 1, plus the border; for a
// surface, type 4, its base size), its row pitch in bytes and its slice's
// bytes (the pitch times the aligned height; 4 KiB aligned but for the
// first two levels of a linear 2D or 1D texture, and for 3D textures).
// Packed mips (+48 bit 0x800) below 16 texels lay out at the mip tail's
// level.
struct LevelLayout {
  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t pitch;
  uint32_t slice;
};
LevelLayout TextureLevelLayout(uint8_t* base, uint32_t texture, uint32_t level);

// sub_822A4F60 Texture::LevelDesc: fills the 32-byte description at `desc`
// (+0 the format, +4 the type, +24 and +28 the level's width and height,
// the rest 0); returns the type (the original's r3).
uint32_t TextureLevelDesc(uint8_t* base, uint32_t texture, uint32_t level, uint32_t desc);

// sub_822A5B78 Texture::GetLevelDesc: TextureLevelDesc (the original jumps
// to it).

// sub_822A5F50 Surface::GetDesc: a texture's level (+0 bit 0x40000000)
// asks TextureLevelDesc of its parent (+24) and level (+28's top 4 bits); a
// render target or depth surface fills it from its own header (+0 its
// format +40, +24 and +28 its width and height from +36, +16 the
// multisample type, +8, +12, +20 = 0); +4 = 4 either way. Returns the
// original's r3: the level description's type, or the surface.
uint32_t SurfaceDesc(uint8_t* base, uint32_t surface, uint32_t desc);

// sub_822A5910 SurfaceHeader::Set: fills a 48-byte surface header (+0 = 4,
// +4 = 1, +20 unlocked, +24 the pitch and samples, +28 the EDRAM base and
// format bits, +32 the hi-z, +36 the size, +40 the format, +44 the EDRAM
// bytes) from the size, format, multisample type and `parameters` (a guest
// pointer to the EDRAM base, hi-z base and +8, or 0). Returns the EDRAM
// tiles and the hi-z size (depth formats 22 and 23).
struct SurfaceSizes {
  uint32_t tiles;
  uint32_t hi_z;
};
SurfaceSizes SetSurfaceHeader(uint8_t* base, uint32_t width, uint32_t height, uint32_t format,
                              int32_t multisample, uint32_t parameters, uint32_t header);

// --- Buffers (the allocation layer: run only with --native_heap) ---

// sub_822A6830 Device::CreateVertexBuffer: a 32-byte header and `bytes` of
// physical memory (write-combined with usage 0x4); 0 when either fails.
uint32_t CreateVertexBuffer(uint8_t* base, uint32_t bytes, uint32_t usage);

// sub_822A6968 Device::CreateIndexBuffer: the same with the index format in
// the header's top 3 bits.
uint32_t CreateIndexBuffer(uint8_t* base, uint32_t bytes, uint32_t usage, uint32_t format);

// sub_822A5E28 Device::CreateSurface: a 48-byte header (XMemAlloc) set by
// SetSurfaceHeader, +0 bit 0x00100000; with no `parameters`, its EDRAM from
// the allocator (+0 bit 0x80000000, the base in +28's low 12 bits; 0 when
// none fits or it passes 2,048 tiles) and the first depth surface takes the
// hi-z. 0 when it fails.
uint32_t CreateSurface(uint8_t* base, uint32_t width, uint32_t height, uint32_t format,
                       int32_t multisample, uint32_t parameters);

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
