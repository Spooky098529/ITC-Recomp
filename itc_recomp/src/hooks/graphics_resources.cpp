// The graphics library's resources natively: adapters in front of
// src/graphics/ (docs/functions/graphics-resources.md). Each leaves r3 as
// the original does (the return value, or r3 as it came for those with
// none). Those reached only from native callers have no adapters, and the
// flag switches their callers: Texture::Format (sub_822A44A8), from
// LevelDesc; Texture::LevelDesc (sub_822A4F60), from GetLevelDesc and
// Surface::GetDesc; Texture::LevelLayout (sub_822A4B98), from LevelDesc;
// SurfaceHeader::Set (sub_822A5910), from CreateSurface; Edram::Alloc
// (sub_822B5EF8), from CreateSurface; Edram::Free (sub_822B5EA8), from
// CreateSurface and Resource::DestroySurface; Edram::NextRun and Mark
// (sub_822B5C90, sub_822B5D70), from Alloc and Free; TextureLayout::LevelCount
// and Sizes (sub_822A4530, sub_822A46C8), from TextureHeader::Set;
// TextureLayout::MipTailOffset (sub_822A4A40), from Texture::LevelOffset;
// Texture::BaseSize and TextureLayout::LevelSize (sub_822A4348, sub_822A45B8),
// from LevelLayout, LevelOffset and Sizes; XGSetTextureHeaderEx
// (sub_8235A720), from XGSetTextureHeader; TextureHeader::Set (sub_822A5608),
// from Device::CreateTexture and XGSetTextureHeaderEx.

#include "guest.h"

#include "base/flags.h"
#include "graphics/resources.h"
#include "hooks/heap.h"

ITC_FLAG_BOOL(native_graphics_resources, true, "ITC",
              "Run the graphics library's resources (docs/functions/graphics-resources.md: "
              "Resource::GetType sub_822A6180, Texture::Format sub_822A44A8, "
              "GpuFormat::BlockSize sub_822A4410, Texture::BaseSize sub_822A4348, "
              "TextureLayout::LevelCount sub_822A4530, Surface::EdramTiles sub_8209AD68, "
              "TextureLayout::LevelSize sub_822A45B8, Texture::LevelDesc sub_822A4F60 with "
              "Texture::LevelLayout sub_822A4B98, Texture::GetLevelDesc sub_822A5B78, "
              "Surface::GetDesc sub_822A5F50, TextureLayout::Sizes sub_822A46C8, "
              "TextureHeader::Set sub_822A5608, Texture::LevelOffset sub_822A4FF0 with "
              "TextureLayout::MipTailOffset sub_822A4A40, XGSetTextureHeader sub_8235A920 with "
              "XGSetTextureHeaderEx sub_8235A720; Device::CreateTexture sub_822A5D08 and "
              "Texture::GetSurfaceLevel sub_822A5B80, with --native_heap; Device::CreateSurface sub_822A5E28 "
              "with SurfaceHeader::Set sub_822A5910, with --native_heap; "
              "Device::CreateVertexBuffer sub_822A6830 and Device::CreateIndexBuffer "
              "sub_822A6968, with --native_heap; Device::GetRenderTarget sub_822A3998, "
              "Device::GetDepthStencilSurface sub_822A39E0, Device::GetStreamSource "
              "sub_822A3880; Resource::Destroy's surface case sub_822A6268, with "
              "--native_heap; Edram::Alloc sub_822B5EF8, with --native_heap, with "
              "Edram::NextRun sub_822B5C90 and Edram::Mark sub_822B5D70, and Edram::Free "
              "sub_822B5EA8, under a std::mutex in place of the kernel's critical section) "
              "natively instead of through the translated code (the same results; checked "
              "by --run_harnesses=graphics_resources). Those reached only from native "
              "callers have no adapters, and the flag switches their callers: sub_822A44A8, "
              "sub_822A4F60, sub_822A4B98, sub_822A5910, sub_822B5EF8, sub_822B5EA8, "
              "sub_822B5C90, sub_822B5D70, sub_822A4530, sub_822A46C8, sub_822A4A40, sub_822A4348, "
              "sub_822A45B8, sub_8235A720, sub_822A5608");

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

namespace {

// Texture::LevelOffset's outputs, through the pointers in r6-r10 and the
// caller's stack (+84, +92, +100), in the original's order.
void StoreLevelPlace(uint8_t* base, const PPCContext& ctx, const LevelPlace& place) {
  const uint32_t stack = ctx.r1.u32;
  guest::StoreU32(base, guest::LoadU32(base, stack + 100), place.offset);
  guest::StoreU32(base, ctx.r6.u32, place.pitch);
  guest::StoreU32(base, ctx.r7.u32, place.slice);
  guest::StoreU32(base, ctx.r8.u32, place.size);
  guest::StoreU32(base, ctx.r10.u32, place.width);
  guest::StoreU32(base, guest::LoadU32(base, stack + 84), place.height);
  guest::StoreU32(base, guest::LoadU32(base, stack + 92), place.depth);
  guest::StoreU32(base, ctx.r9.u32, place.tail);
}

}  // namespace

// --- Headers: r3 = the header ---

ITC_GRAPHICS_RESOURCES(822A6180, ctx.r3.u64 = ResourceType(base, ctx.r3.u32))

// r3 = the format, r4, r5 = the block's width and height out (in that order).
ITC_GRAPHICS_RESOURCES(822A4410, {
  const Block block = GpuBlockSize(ctx.r3.u32);
  guest::StoreU32(base, ctx.r4.u32, block.width);
  guest::StoreU32(base, ctx.r5.u32, block.height);
})

// r3 width, r4 height, r5 format, r6 the multisample type.
ITC_GRAPHICS_RESOURCES(8209AD68, ctx.r3.u64 = SurfaceEdramTiles(ctx.r3.u32, ctx.r4.u32,
                                                                ctx.r5.u32, ctx.r6.s32))

// r3 = the texture, r4 = the level, r5 = the description; returns the type.
ITC_GRAPHICS_RESOURCES(822A5B78, ctx.r3.u64 = TextureLevelDesc(base, ctx.r3.u32, ctx.r4.u32,
                                                               ctx.r5.u32))

// r3 = the surface, r4 = the description.
ITC_GRAPHICS_RESOURCES(822A5F50, ctx.r3.u64 = SurfaceDesc(base, ctx.r3.u32, ctx.r4.u32))

// --- Texture headers: arguments past r10 on the caller's stack (+84 on).
// TextureHeader::Set has no adapter: only CreateTexture and
// XGSetTextureHeaderEx call it. ---

// r3 the texture, r4 the face, r5 the level; out: r6 the pitch, r7 the
// slice, r8 the bytes over the depth, r9 the mip tail offset, r10 the width,
// the stack's +84 the height, +92 the depth, +100 the level's offset (stored
// in the original's order). r3 is left as it came (no caller reads it).
// Levels from 2^31 on run the original (it reads uninitialized stack words).
extern "C" REX_FUNC(sub_822A4FF0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_graphics_resources) || ctx.r5.u32 >= 0x80000000u) {
    __imp__sub_822A4FF0(ctx, base);
    return;
  }
  StoreLevelPlace(base, ctx, TextureLevelOffset(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32));
}

// r3-r5 width, height, depth, r6 levels, r7 usage, r8 format, r10 the type
// (others than TextureHeader::Set takes run the original).
extern "C" REX_FUNC(sub_822A5D08) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_graphics_resources) || !hooks::NativeHeapActive() ||
      !TextureTypeKnown(ctx.r10.u32)) {
    __imp__sub_822A5D08(ctx, base);
    return;
  }
  ctx.r3.u64 = CreateTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                             ctx.r8.u32, ctx.r10.u32);
}

// r3 = the texture, r4 = the level.
ITC_GRAPHICS_RESOURCES_HEAP(822A5B80, ctx.r3.u64 = GetSurfaceLevel(base, ctx.r3.u32, ctx.r4.u32))

// r3, r4 width and height, r5 levels, r6 usage, r7 format, r9 and r10 the
// base and mip addresses; the stack: +84 the pitch, +92 the header (copied
// there), +100 and +108 the base and mip sizes out. XGSetTextureHeaderEx
// has no adapter: only this calls it. A header in the physical views or
// across two views runs the original (its memcpy records those copies).
extern "C" REX_FUNC(sub_8235A920) {
  REX_FUNC_PROLOGUE();
  const uint32_t stack = ctx.r1.u32;
  const uint32_t header = guest::LoadU32(base, stack + 92);
  if (!ITC_FLAG(native_graphics_resources) || !XgHeaderCopyFits(header)) {
    __imp__sub_8235A920(ctx, base);
    return;
  }
  ctx.r3.u64 = XGSetTextureHeader(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                  ctx.r7.u32, ctx.r9.u32, ctx.r10.u32,
                                  guest::LoadU32(base, stack + 84), header,
                                  guest::LoadU32(base, stack + 100),
                                  guest::LoadU32(base, stack + 108));
}

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
