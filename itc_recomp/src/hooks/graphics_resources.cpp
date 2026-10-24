// The graphics library's resources natively: adapters in front of
// src/graphics/ (docs/functions/graphics-resources.md). Each leaves r3 as
// the original does (the return value, or r3 as it came for those with
// none).

#include "guest.h"

#include "base/flags.h"
#include "graphics/resources.h"

ITC_FLAG_BOOL(native_graphics_resources, true, "ITC",
              "Run the graphics library's resources (docs/functions/graphics-resources.md: "
              "Resource::GetType sub_822A6180, Texture::Format sub_822A44A8, "
              "GpuFormat::BlockSize sub_822A4410, Texture::BaseSize sub_822A4348, "
              "TextureLayout::LevelCount sub_822A4530, Surface::EdramTiles sub_8209AD68) "
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
