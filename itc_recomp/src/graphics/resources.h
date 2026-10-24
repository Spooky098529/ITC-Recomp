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

}  // namespace graphics
