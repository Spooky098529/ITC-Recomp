// The graphics library's resources natively: adapters in front of
// src/graphics/ (docs/functions/graphics-resources.md). Each leaves r3 as
// the original does (the return value, or r3 as it came for those with
// none).

#include "guest.h"

#include "base/flags.h"
#include "graphics/edram.h"
#include "graphics/resources.h"
#include "hooks/heap.h"

#include <optional>

ITC_FLAG_BOOL(native_graphics_resources, true, "ITC",
              "Run the graphics library's resources (docs/functions/graphics-resources.md: "
              "Resource::GetType sub_822A6180, Texture::Format sub_822A44A8, "
              "GpuFormat::BlockSize sub_822A4410, Texture::BaseSize sub_822A4348, "
              "TextureLayout::LevelCount sub_822A4530, Surface::EdramTiles sub_8209AD68, "
              "TextureLayout::LevelSize sub_822A45B8, Texture::LevelDesc sub_822A4F60 with "
              "Texture::LevelLayout sub_822A4B98, Texture::GetLevelDesc sub_822A5B78, "
              "Surface::GetDesc sub_822A5F50; Device::CreateSurface sub_822A5E28 "
              "with SurfaceHeader::Set sub_822A5910, with --native_heap; "
              "Device::CreateVertexBuffer sub_822A6830 and Device::CreateIndexBuffer "
              "sub_822A6968, with --native_heap; Device::GetRenderTarget sub_822A3998, "
              "Device::GetDepthStencilSurface sub_822A39E0, Device::GetStreamSource "
              "sub_822A3880; Resource::Destroy's surface case sub_822A6268, with "
              "--native_heap; Edram::Alloc sub_822B5EF8, with --native_heap, with "
              "Edram::NextRun sub_822B5C90 and Edram::Mark sub_822B5D70, and Edram::Free "
              "sub_822B5EA8, under a std::mutex in place of the kernel's critical section) "
              "natively instead of through the translated code (the same results; checked "
              "by --run_harnesses=graphics_resources)");

// Defines an adapter: the body (the arguments after the address) uses `ctx`
// and `base`.
#define ITC_GRAPHICS_RESOURCES(address, ...)       \
  extern "C" REX_FUNC(sub_##address) {             \
    REX_FUNC_PROLOGUE();                           \
    if (!ITC_FLAG(native_graphics_resources)) {    \
      __imp__sub_##address(ctx, base);             \
      return;                                      \
    }                                              \
    __VA_ARGS__;                                   \
  }

// The same for those that allocate: the game's own with --native_heap off.
#define ITC_GRAPHICS_RESOURCES_HEAP(address, ...)                           \
  extern "C" REX_FUNC(sub_##address) {                                      \
    REX_FUNC_PROLOGUE();                                                    \
    if (!ITC_FLAG(native_graphics_resources) || !hooks::NativeHeapActive()) { \
      __imp__sub_##address(ctx, base);                                      \
      return;                                                               \
    }                                                                       \
    __VA_ARGS__;                                                            \
  }

using namespace graphics;

// --- Headers: r3 = the header ---

ITC_GRAPHICS_RESOURCES(822A6180, ctx.r3.u64 = ResourceType(base, ctx.r3.u32))
ITC_GRAPHICS_RESOURCES(822A44A8, ctx.r3.u64 = TextureFormat(base, ctx.r3.u32))

// r3 = the format, r4, r5 = the block's width and height out (in that order).
ITC_GRAPHICS_RESOURCES(822A4410, {
  const Block block = GpuBlockSize(ctx.r3.u32);
  guest::StoreU32(base, ctx.r4.u32, block.width);
  guest::StoreU32(base, ctx.r5.u32, block.height);
})

// r3 = the texture, r4, r5, r6 = width, height and depth out.
ITC_GRAPHICS_RESOURCES(822A4348, {
  const Extent size = TextureBaseSize(base, ctx.r3.u32);
  guest::StoreU32(base, ctx.r4.u32, size.width);
  guest::StoreU32(base, ctx.r5.u32, size.height);
  guest::StoreU32(base, ctx.r6.u32, size.depth);
})

// r3, r4, r5 = width, height, depth, r6 = the border.
ITC_GRAPHICS_RESOURCES(822A4530, ctx.r3.u64 = TextureLevelCount(ctx.r3.u32, ctx.r4.u32,
                                                                ctx.r5.u32, ctx.r6.u32))

// r3 width, r4 height, r5 format, r6 the multisample type.
ITC_GRAPHICS_RESOURCES(8209AD68, ctx.r3.u64 = SurfaceEdramTiles(ctx.r3.u32, ctx.r4.u32,
                                                                ctx.r5.u32, ctx.r6.s32))

// r3 = the texture, r4 = the level, r5 = the description; returns the type.
// LevelLayout has no adapter: only LevelDesc calls it.
ITC_GRAPHICS_RESOURCES(822A4F60, ctx.r3.u64 = TextureLevelDesc(base, ctx.r3.u32, ctx.r4.u32,
                                                               ctx.r5.u32))

ITC_GRAPHICS_RESOURCES(822A5B78, ctx.r3.u64 = TextureLevelDesc(base, ctx.r3.u32, ctx.r4.u32,
                                                               ctx.r5.u32))

// r3 = the surface, r4 = the description.
ITC_GRAPHICS_RESOURCES(822A5F50, ctx.r3.u64 = SurfaceDesc(base, ctx.r3.u32, ctx.r4.u32))

// r3, r4, r5 = width, height and depth (aligned in place), r6 = bits a
// pixel, r7 = the format, r8 = the dimension, r9 = tiled; returns the bytes.
ITC_GRAPHICS_RESOURCES(822A45B8, {
  uint32_t width = guest::LoadU32(base, ctx.r3.u32);
  uint32_t height = guest::LoadU32(base, ctx.r4.u32);
  uint32_t depth = guest::LoadU32(base, ctx.r5.u32);
  const uint32_t bytes =
      TextureLevelSize(width, height, depth, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32);
  guest::StoreU32(base, ctx.r3.u32, width);
  guest::StoreU32(base, ctx.r4.u32, height);
  guest::StoreU32(base, ctx.r5.u32, depth);
  ctx.r3.u64 = bytes;
})

// --- Surfaces: r3 width, r4 height, r5 format, r6 the multisample type, r7
// the parameters (or 0). SurfaceHeader::Set has no adapter: only
// CreateSurface calls it. ---

ITC_GRAPHICS_RESOURCES_HEAP(822A5E28, ctx.r3.u64 = CreateSurface(base, ctx.r3.u32, ctx.r4.u32,
                                                                 ctx.r5.u32, ctx.r6.s32,
                                                                 ctx.r7.u32))

// --- Buffers: r3 = the bytes, r4 = the usage (r5 = the index format) ---

ITC_GRAPHICS_RESOURCES_HEAP(822A6830, ctx.r3.u64 = CreateVertexBuffer(base, ctx.r3.u32,
                                                                      ctx.r4.u32))
ITC_GRAPHICS_RESOURCES_HEAP(822A6968, ctx.r3.u64 = CreateIndexBuffer(base, ctx.r3.u32,
                                                                     ctx.r4.u32, ctx.r5.u32))

// --- The device's current resources: r3 = the device ---

ITC_GRAPHICS_RESOURCES(822A3998, ctx.r3.u64 = GetRenderTarget(base, ctx.r3.u32, ctx.r4.u32))
ITC_GRAPHICS_RESOURCES(822A39E0, ctx.r3.u64 = GetDepthStencilSurface(base, ctx.r3.u32))

// r4 = the stream, r5, r6 = the offset and stride out (in that order).
ITC_GRAPHICS_RESOURCES(822A3880, {
  const StreamSource source = GetStreamSource(base, ctx.r3.u32, ctx.r4.u32);
  guest::StoreU32(base, ctx.r5.u32, source.offset);
  guest::StoreU32(base, ctx.r6.u32, source.stride);
  ctx.r3.u64 = source.buffer;
})

// --- Destruction ---

// Resource::Destroy's surface case, split off by the recompiler: entered
// with Destroy's frame (r31 = the surface, r11 = its +0), so it ends as
// Destroy does (its 112-byte frame popped, r29-r31 and lr restored).
extern "C" REX_FUNC(sub_822A6268) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_graphics_resources) || !hooks::NativeHeapActive()) {
    __imp__sub_822A6268(ctx, base);
    return;
  }
  ctx.r3.u64 = DestroySurface(base, ctx.r31.u32, ctx.r11.u32);
  ctx.r1.s64 = ctx.r1.s64 + 112;
  __restgprlr_29(ctx, base);
}

// --- EDRAM: NextRun and Mark have no adapters (only Alloc and Free call
// them) ---

// r3 = the tiles, r4 = the base out; returns 1 or 0.
ITC_GRAPHICS_RESOURCES_HEAP(822B5EF8, {
  const std::optional<edram::Allocation> allocation = edram::Alloc(base, ctx.r3.u32);
  if (!allocation) {
    ctx.r3.u64 = 0;
    return;
  }
  guest::StoreU32(base, ctx.r4.u32, allocation->base);
  ctx.r3.u64 = allocation->ok;
})

// r3 = the first tile, r4 = the tiles. r3 left as it came (the original
// leaves RtlLeaveCriticalSection's). With Alloc's fallback (--native_heap),
// so both take the same lock.
ITC_GRAPHICS_RESOURCES_HEAP(822B5EA8, edram::Free(base, ctx.r3.u32, ctx.r4.u32))
