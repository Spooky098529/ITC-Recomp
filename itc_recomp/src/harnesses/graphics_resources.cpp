// The graphics library's resources (src/graphics/,
// docs/functions/graphics-resources.md) against the recompiled functions.
// The header leaves run on headers of random words in the scratch area
// (with the type, dimension and child bits drawn so each branch is taken),
// the size math on random sizes (edge values now and then); outputs and r3
// are compared exactly (none is a float). The originals run with
// --native_graphics_resources off.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "graphics/edram.h"
#include "graphics/resources.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_graphics_resources);

namespace harness {

namespace {

namespace gr = graphics;

// Runs the recompiled version with the native flag off.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  const bool saved = ITC_FLAG(native_graphics_resources);
  ITC_FLAG(native_graphics_resources) = false;
  recompiled(ctx, base);
  ITC_FLAG(native_graphics_resources) = saved;
}

// --- The register steps, as the adapters (src/hooks/graphics_resources.cpp) ---

void NativeResourceType(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::ResourceType(base, ctx.r3.u32);
}
void NativeTextureFormat(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::TextureFormat(base, ctx.r3.u32);
}
void NativeBlockSize(PPCContext& ctx, uint8_t* base) {
  const gr::Block block = gr::GpuBlockSize(ctx.r3.u32);
  guest::StoreU32(base, ctx.r4.u32, block.width);
  guest::StoreU32(base, ctx.r5.u32, block.height);
}
void NativeBaseSize(PPCContext& ctx, uint8_t* base) {
  const gr::Extent size = gr::TextureBaseSize(base, ctx.r3.u32);
  guest::StoreU32(base, ctx.r4.u32, size.width);
  guest::StoreU32(base, ctx.r5.u32, size.height);
  guest::StoreU32(base, ctx.r6.u32, size.depth);
}
void NativeLevelCount(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::TextureLevelCount(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
void NativeEdramTiles(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::SurfaceEdramTiles(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32);
}

void NativeCreateVertexBuffer(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::CreateVertexBuffer(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeCreateIndexBuffer(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::CreateIndexBuffer(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}
void NativeGetRenderTarget(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::GetRenderTarget(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeGetDepthStencilSurface(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::GetDepthStencilSurface(base, ctx.r3.u32);
}
void NativeGetStreamSource(PPCContext& ctx, uint8_t* base) {
  const gr::StreamSource source = gr::GetStreamSource(base, ctx.r3.u32, ctx.r4.u32);
  guest::StoreU32(base, ctx.r5.u32, source.offset);
  guest::StoreU32(base, ctx.r6.u32, source.stride);
  ctx.r3.u64 = source.buffer;
}

void NativeNextRun(PPCContext& ctx, uint8_t* base) {
  uint32_t position = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t length = gr::edram::NextRun(base, ctx.r3.u32, position);
  guest::StoreU32(base, ctx.r4.u32, position);
  ctx.r3.u64 = length;
}
void NativeMark(PPCContext& ctx, uint8_t* base) {
  gr::edram::Mark(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32 != 0);
}
void NativeEdramAlloc(PPCContext& ctx, uint8_t* base) {
  const auto allocation = gr::edram::Alloc(base, ctx.r3.u32);
  if (!allocation) {
    ctx.r3.u64 = 0;
    return;
  }
  guest::StoreU32(base, ctx.r4.u32, allocation->base);
  ctx.r3.u64 = allocation->ok;
}
void NativeEdramFree(PPCContext& ctx, uint8_t* base) {
  gr::edram::Free(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeDestroySurface(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::DestroySurface(base, ctx.r31.u32, ctx.r11.u32);
  ctx.r1.s64 = ctx.r1.s64 + 112;
  __restgprlr_29(ctx, base);
}

void NativeLevelSize(PPCContext& ctx, uint8_t* base) {
  uint32_t width = guest::LoadU32(base, ctx.r3.u32);
  uint32_t height = guest::LoadU32(base, ctx.r4.u32);
  uint32_t depth = guest::LoadU32(base, ctx.r5.u32);
  const uint32_t bytes = gr::TextureLevelSize(width, height, depth, ctx.r6.u32, ctx.r7.u32,
                                              ctx.r8.u32, ctx.r9.u32);
  guest::StoreU32(base, ctx.r3.u32, width);
  guest::StoreU32(base, ctx.r4.u32, height);
  guest::StoreU32(base, ctx.r5.u32, depth);
  ctx.r3.u64 = bytes;
}
// SurfaceHeader::Set has no adapter (only CreateSurface calls it): the
// register steps of the original's.
void NativeSetSurfaceHeader(PPCContext& ctx, uint8_t* base) {
  const gr::SurfaceSizes sizes = gr::SetSurfaceHeader(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                      ctx.r6.s32, ctx.r7.u32, ctx.r8.u32);
  guest::StoreU32(base, ctx.r9.u32, sizes.tiles);
  guest::StoreU32(base, ctx.r10.u32, sizes.hi_z);
  ctx.r3.u64 = sizes.tiles;
}
void NativeCreateSurface(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::CreateSurface(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32, ctx.r7.u32);
}

void NativeLevelDesc(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::TextureLevelDesc(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

void NativeSurfaceDesc(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::SurfaceDesc(base, ctx.r3.u32, ctx.r4.u32);
}

void NativeLayoutSizes(PPCContext& ctx, uint8_t* base) {
  const uint32_t stack = ctx.r1.u32;
  const gr::TextureSizes sizes = gr::TextureLayoutSizes(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
      ctx.r10.u32, guest::LoadU32(base, stack + 84), guest::LoadU32(base, stack + 100));
  guest::StoreU32(base, guest::LoadU32(base, stack + 108), sizes.base_width);
  guest::StoreU32(base, guest::LoadU32(base, stack + 116), sizes.base_size);
  guest::StoreU32(base, guest::LoadU32(base, stack + 124), sizes.mip_size);
  ctx.r3.u64 = sizes.levels;
}
void NativeSetTextureHeader(PPCContext& ctx, uint8_t* base) {
  const uint32_t stack = ctx.r1.u32;
  const gr::TextureHeaderSizes sizes = gr::SetTextureHeader(
      base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
      ctx.r10.u32, guest::LoadU32(base, stack + 84), guest::LoadU32(base, stack + 92),
      guest::LoadU32(base, stack + 100), guest::LoadU32(base, stack + 108));
  guest::StoreU32(base, guest::LoadU32(base, stack + 116), sizes.base_size);
  guest::StoreU32(base, guest::LoadU32(base, stack + 124), sizes.mip_size);
  ctx.r3.u64 = sizes.r3;
}

void NativeMipTailOffset(PPCContext& ctx, uint8_t* base) {
  const gr::MipTailPlace place = gr::MipTailOffset(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                   ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32);
  guest::StoreU32(base, ctx.r10.u32, place.x);
  guest::StoreU32(base, guest::LoadU32(base, ctx.r1.u32 + 84), place.y);
  guest::StoreU32(base, guest::LoadU32(base, ctx.r1.u32 + 92), place.z);
  ctx.r3.u64 = place.offset;
}
void NativeLevelOffset(PPCContext& ctx, uint8_t* base) {
  const gr::LevelPlace place = gr::TextureLevelOffset(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
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

void NativeCreateTexture(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::CreateTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                 ctx.r7.u32, ctx.r8.u32, ctx.r10.u32);
}
void NativeGetSurfaceLevel(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = gr::GetSurfaceLevel(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeXGSetTextureHeader(PPCContext& ctx, uint8_t* base) {
  const uint32_t stack = ctx.r1.u32;
  ctx.r3.u64 = gr::XGSetTextureHeader(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                      ctx.r7.u32, ctx.r9.u32, ctx.r10.u32,
                                      guest::LoadU32(base, stack + 84),
                                      guest::LoadU32(base, stack + 92),
                                      guest::LoadU32(base, stack + 100),
                                      guest::LoadU32(base, stack + 108));
}

// The scratch area's layout.
constexpr uint32_t kHeader = 0;    // 64 bytes
constexpr uint32_t kParent = 64;   // 64 bytes
constexpr uint32_t kOut = 256;     // 3 words

uint32_t Word32(Random& r) { return uint32_t(r.Next()); }

// A size for the math: small, large, or an edge value.
uint32_t Size(Random& r) {
  switch (r.Below(6)) {
    case 0:
      return r.Below(4);
    case 1:
      return 0xFFFFFFFF - r.Below(4);
    case 2:
      return 0x80000000u + r.Below(4) - 2;
    case 3:
      return Word32(r);
    default:
      return 1 + r.Below(4096);
  }
}

// A random header at `address`: random words, then the type's low 4 bits,
// the child bit and the dimension drawn.
void RandomHeader(uint8_t* base, uint32_t address, Random& r) {
  for (uint32_t i = 0; i < 64; i += 4) {
    guest::StoreU32(base, address + i, Word32(r));
  }
  uint32_t common = guest::LoadU32(base, address);
  const uint32_t pick = r.Below(4);
  if (pick < 3) {
    common = (common & ~0xFu) | (pick == 0 ? 3 : 4);
  }
  if (r.Below(2)) {
    common ^= 0x40000000;
  }
  guest::StoreU32(base, address, common);
}

// Runs `f` (a header's leaf: r3 = the header, r4-r6 = the outputs) on both
// versions; compares the outputs and r3.
template <typename Setup>
void HeaderCase(Context& h, Results& results, GuestFunction recompiled, GuestFunction native,
                Setup setup, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    t.r3.u64 = h.data + kHeader;
    t.r4.u64 = h.data + kOut;
    t.r5.u64 = h.data + kOut + 4;
    t.r6.u64 = h.data + kOut + 8;
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, h.data + kOut, 12));
    out[v].Word(t.r3.u32);
  }
  results.Check(out[0], out[1], what);
}

void Headers(Context& h, Random& r) {
  struct Leaf {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Leaf leaves[] = {
      {"Resource::GetType (sub_822A6180)", Original<__imp__sub_822A6180>, NativeResourceType},
      {"Texture::Format (sub_822A44A8)", Original<__imp__sub_822A44A8>, NativeTextureFormat},
      {"Texture::BaseSize (sub_822A4348)", Original<__imp__sub_822A4348>, NativeBaseSize},
  };
  for (const Leaf& leaf : leaves) {
    Results results(leaf.name);
    for (int i = 0; i < 400; i++) {
      // The header and its parent, written the same for both versions.
      Fill(h.base, h.data, kScratchBytes);
      RandomHeader(h.base, h.data + kHeader, r);
      RandomHeader(h.base, h.data + kParent, r);
      // A texture's surface points at the parent.
      guest::StoreU32(h.base, h.data + kHeader + 24, h.data + kParent);
      const std::vector<uint8_t> inputs = Read(h.base, h.data, 128);
      HeaderCase(
          h, results, leaf.recompiled, leaf.native,
          [&] { Write(h.base, h.data, inputs.data(), uint32_t(inputs.size())); },
          fmt::format("case {}: {}", i, Hex(inputs)));
    }
    results.Report();
  }
}

// A texture header like the game's: random words, then the type (3, or a
// surface's 4 with or without the child bit), the format (+32's low 6 bits),
// the size (+36: 2D sizes up to 2,048 mostly), the border (+40's top bit,
// now and then), the dimension and packed mips (+48 bits 9-10 and 0x800),
// tiling and the pitch (+28's top bit and bits 22-30) drawn.
void RandomTexture(uint8_t* base, uint32_t address, Random& r) {
  RandomHeader(base, address, r);
  uint32_t common = guest::LoadU32(base, address);
  if (r.Below(4)) {
    common = (common & ~0x4000000Fu) | 3;
  }
  guest::StoreU32(base, address, common);
  auto set = [&](uint32_t offset, uint32_t mask, uint32_t value) {
    const uint32_t word = guest::LoadU32(base, address + offset);
    guest::StoreU32(base, address + offset, (word & ~mask) | (value & mask));
  };
  if (r.Below(4)) {
    const uint32_t w = r.Below(2) ? (1u << r.Below(12)) - 1 : r.Below(2048);
    const uint32_t h = r.Below(2) ? (1u << r.Below(12)) - 1 : r.Below(2048);
    set(36, 0x03FFFFFF, w | (h << 13));
  }
  if (r.Below(4)) {
    set(40, 0x80000000, 0);
  }
  if (r.Below(2)) {
    set(48, 0x600, 1u << 9);  // 2D
  }
  if (r.Below(4)) {
    set(32, 0x400, 0);
  }
  if (r.Below(2)) {
    set(44, 0x3C0, 0);
  }
}

void LevelDescs(Context& h, Random& r) {
  Results results("Texture::LevelDesc (sub_822A4F60) with Texture::LevelLayout (sub_822A4B98)");
  for (int i = 0; i < 2000; i++) {
    Fill(h.base, h.data, kScratchBytes);
    RandomTexture(h.base, h.data + kHeader, r);
    RandomTexture(h.base, h.data + kParent, r);
    guest::StoreU32(h.base, h.data + kHeader + 24, h.data + kParent);
    // A linear format with no bits a pixel hits LevelSize's divide trap
    // (a warning each version): mostly made tiled.
    const uint32_t format = guest::LoadU32(h.base, h.data + kHeader + 32) & 0x3F;
    if (guest::LoadU8(h.base, 0x82013468 + 1 + (format << 1)) == 0 && r.Below(16)) {
      const uint32_t w28 = guest::LoadU32(h.base, h.data + kHeader + 28);
      guest::StoreU32(h.base, h.data + kHeader + 28, w28 | 0x80000000);
    }
    const std::vector<uint8_t> inputs = Read(h.base, h.data, 128);
    const uint32_t level = r.Below(8) ? r.Below(13) : uint32_t(r.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Write(h.base, h.data, inputs.data(), uint32_t(inputs.size()));
      PPCContext t = CallContext(h);
      t.r3.u64 = h.data + kHeader;
      t.r4.u64 = level;
      t.r5.u64 = h.data + kOut;
      (v == 0 ? Original<__imp__sub_822A4F60> : NativeLevelDesc)(t, h.base);
      out[v].Bytes(Read(h.base, h.data + kOut, 32));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("case {}: level {}, {}", i, level, Hex(inputs)));
  }
  results.Report();
  // GetLevelDesc jumps to LevelDesc; Surface::GetDesc on a texture's
  // surface (asking its parent, a random texture) or a target's header.
  struct Desc {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Desc descs[] = {
      {"Texture::GetLevelDesc (sub_822A5B78)", Original<__imp__sub_822A5B78>, NativeLevelDesc},
      {"Surface::GetDesc (sub_822A5F50)", Original<__imp__sub_822A5F50>, NativeSurfaceDesc},
  };
  for (const Desc& d : descs) {
    Results more(d.name);
    const bool surface = d.native == NativeSurfaceDesc;
    for (int i = 0; i < 500; i++) {
      Fill(h.base, h.data, kScratchBytes);
      RandomTexture(h.base, h.data + kHeader, r);
      RandomTexture(h.base, h.data + kParent, r);
      if (surface) {
        // The header a surface; its parent (when a texture's level) a texture.
        const uint32_t common = guest::LoadU32(h.base, h.data + kHeader);
        guest::StoreU32(h.base, h.data + kHeader, (common & ~0xFu) | 4);
        const uint32_t parent = guest::LoadU32(h.base, h.data + kParent);
        guest::StoreU32(h.base, h.data + kParent, (parent & ~0x4000000Fu) | 3);
      }
      guest::StoreU32(h.base, h.data + kHeader + 24, h.data + kParent);
      const std::vector<uint8_t> inputs = Read(h.base, h.data, 128);
      const uint32_t level = r.Below(13);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, h.data, inputs.data(), uint32_t(inputs.size()));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kHeader;
        t.r4.u64 = surface ? h.data + kOut : level;
        t.r5.u64 = h.data + kOut;
        (v == 0 ? d.recompiled : d.native)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kOut, 32));
        out[v].Word(t.r3.u32);
      }
      more.Check(out[0], out[1], fmt::format("case {}: level {}, {}", i, level, Hex(inputs)));
    }
    more.Report();
  }
}

// A texture's size for the layouts: up to 2,048 mostly, a power of two or
// one less now and then, an edge value rarely.
uint32_t TextureSide(Random& r) {
  switch (r.Below(8)) {
    case 0:
      return 1u << r.Below(13);
    case 1:
      return (1u << r.Below(13)) - 1;
    case 2:
      return Size(r);
    default:
      return 1 + r.Below(2048);
  }
}

// TextureLayout::Sizes on random sizes, levels (0 now and then: the full
// chain; never so many the loop runs long), formats, dimensions, tiling,
// packing, borders and pitches (0 mostly); TextureHeader::Set on headers of
// random words with the types it knows, random formats, usages and fields.
// Compared: the outputs, the header, r3. A pitch with a format of no bits a
// pixel hits Sizes's divide trap (a warning each version).
void TextureHeaders(Context& h, Random& r) {
  {
    Results results("TextureLayout::Sizes (sub_822A46C8)");
    for (int i = 0; i < 2000; i++) {
      const uint32_t w = TextureSide(r), hh = TextureSide(r);
      const uint32_t d = r.Below(2) ? 1 : r.Below(4) ? r.Below(65) : TextureSide(r);
      const uint32_t levels = r.Below(4) == 0 ? 0 : r.Below(16);
      const uint32_t format = r.Below(8) ? r.Below(64) : uint32_t(r.Next());
      const uint32_t dimension = r.Below(16) ? r.Below(4) : uint32_t(r.Next());
      const uint32_t tiled = r.Below(8) ? r.Below(2) : uint32_t(r.Next());
      const uint32_t packed = r.Below(8) ? r.Below(2) : uint32_t(r.Next());
      const uint32_t border = r.Below(8) ? (r.Below(4) == 0) : uint32_t(r.Next());
      const uint32_t pitch = r.Below(4) ? 0 : r.Below(8) ? 32 * r.Below(256) : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        const uint32_t stack = t.r1.u32;
        guest::StoreU32(h.base, stack + 84, border);
        guest::StoreU32(h.base, stack + 92, 0x5A5A5A5A);  // not read
        guest::StoreU32(h.base, stack + 100, pitch);
        guest::StoreU32(h.base, stack + 108, h.data + kOut);
        guest::StoreU32(h.base, stack + 116, h.data + kOut + 4);
        guest::StoreU32(h.base, stack + 124, h.data + kOut + 8);
        t.r3.u64 = w;
        t.r4.u64 = hh;
        t.r5.u64 = d;
        t.r6.u64 = levels;
        t.r7.u64 = format;
        t.r8.u64 = dimension;
        t.r9.u64 = tiled;
        t.r10.u64 = packed;
        (v == 0 ? Original<__imp__sub_822A46C8> : NativeLayoutSizes)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kOut, 12));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {} x {} x {}, {} levels, format {:X}, dimension {:X}, "
                                "tiled {:X}, packed {:X}, border {:X}, pitch {:X}",
                                i, w, hh, d, levels, format, dimension, tiled, packed, border,
                                pitch));
    }
    results.Report();
  }
  {
    Results results("TextureHeader::Set (sub_822A5608) with TextureLayout::Sizes");
    const uint32_t types[] = {3, 3, 3, 17, 18, 19, 20};
    const uint32_t usages[] = {0, 0x4, 0x200, 0x204, 0x8};
    for (int i = 0; i < 2000; i++) {
      const uint32_t type = types[r.Below(7)];
      const uint32_t w = TextureSide(r), hh = TextureSide(r);
      const uint32_t d = type == 18 ? 6 : type == 3 || type == 20 ? 1 : 1 + r.Below(64);
      const uint32_t levels = r.Below(3) == 0 ? 0 : r.Below(14);
      const uint32_t usage = r.Below(4) ? usages[r.Below(5)] : uint32_t(r.Next());
      const uint32_t format = uint32_t(r.Next());
      const uint32_t packed = r.Below(8) ? r.Below(3) : uint32_t(r.Next());
      const uint32_t border = r.Below(8) ? (r.Below(4) == 0) : r.Below(4);
      const uint32_t field = r.Below(2) ? 0 : uint32_t(r.Next());
      const uint32_t pitch = r.Below(4) ? 0 : 32 * r.Below(256);
      std::vector<uint8_t> words(64);
      for (uint8_t& b : words) {
        b = uint8_t(r.Next());
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, h.data + kHeader, words.data(), uint32_t(words.size()));
        PPCContext t = CallContext(h);
        const uint32_t stack = t.r1.u32;
        guest::StoreU32(h.base, stack + 84, border);
        guest::StoreU32(h.base, stack + 92, field);
        guest::StoreU32(h.base, stack + 100, pitch);
        guest::StoreU32(h.base, stack + 108, h.data + kHeader);
        guest::StoreU32(h.base, stack + 116, h.data + kOut);
        guest::StoreU32(h.base, stack + 124, h.data + kOut + 4);
        t.r3.u64 = type;
        t.r4.u64 = w;
        t.r5.u64 = hh;
        t.r6.u64 = d;
        t.r7.u64 = levels;
        t.r8.u64 = usage;
        t.r9.u64 = format;
        t.r10.u64 = packed;
        (v == 0 ? Original<__imp__sub_822A5608> : NativeSetTextureHeader)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kHeader, 64));
        out[v].Bytes(Read(h.base, h.data + kOut, 8));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: type {}, {} x {} x {}, {} levels, usage {:X}, format "
                                "{:X}, packed {:X}, border {}, field {:X}, pitch {:X}, {}",
                                i, type, w, hh, d, levels, usage, format, packed, border, field,
                                pitch, Hex(words)));
    }
    results.Report();
  }
}

// TextureLayout::MipTailOffset on random tail levels, power-of-two sizes,
// pitches, slices and formats; Texture::LevelOffset on random textures (as
// LevelDescs's, packed now and then), faces and levels below 16. Compared:
// every output and, for the tail, r3 (no caller reads LevelOffset's r3).
void LevelOffsets(Context& h, Random& r) {
  {
    Results results("TextureLayout::MipTailOffset (sub_822A4A40)");
    for (int i = 0; i < 1000; i++) {
      const uint32_t level = r.Below(8) ? r.Below(12) : uint32_t(r.Next());
      const uint32_t w = r.Below(8) ? 1u << r.Below(6) : TextureSide(r);
      const uint32_t hh = r.Below(8) ? 1u << r.Below(6) : TextureSide(r);
      const uint32_t d = r.Below(2) ? 1 : r.Below(8) ? 1u << r.Below(6) : TextureSide(r);
      const uint32_t pitch = r.Below(8) ? 32 * r.Below(64) : uint32_t(r.Next());
      const uint32_t slice = r.Below(8) ? 4096 * r.Below(16) : uint32_t(r.Next());
      const uint32_t format = r.Below(8) ? r.Below(64) : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        guest::StoreU32(h.base, t.r1.u32 + 84, h.data + kOut + 4);
        guest::StoreU32(h.base, t.r1.u32 + 92, h.data + kOut + 8);
        t.r3.u64 = level;
        t.r4.u64 = w;
        t.r5.u64 = hh;
        t.r6.u64 = d;
        t.r7.u64 = pitch;
        t.r8.u64 = slice;
        t.r9.u64 = format;
        t.r10.u64 = h.data + kOut;
        (v == 0 ? Original<__imp__sub_822A4A40> : NativeMipTailOffset)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kOut, 12));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: level {}, {} x {} x {}, pitch {:X}, slice {:X}, "
                                "format {:X}",
                                i, level, w, hh, d, pitch, slice, format));
    }
    results.Report();
  }
  {
    Results results("Texture::LevelOffset (sub_822A4FF0) with TextureLayout::MipTailOffset");
    for (int i = 0; i < 3000; i++) {
      Fill(h.base, h.data, kScratchBytes);
      RandomTexture(h.base, h.data + kHeader, r);
      const uint32_t format = guest::LoadU32(h.base, h.data + kHeader + 32) & 0x3F;
      if (guest::LoadU8(h.base, 0x82013468 + 1 + (format << 1)) == 0 && r.Below(16)) {
        const uint32_t w28 = guest::LoadU32(h.base, h.data + kHeader + 28);
        guest::StoreU32(h.base, h.data + kHeader + 28, w28 | 0x80000000);
      }
      if (r.Below(2)) {  // small sizes reach the mip tail sooner
        const uint32_t w36 = guest::LoadU32(h.base, h.data + kHeader + 36);
        guest::StoreU32(h.base, h.data + kHeader + 36,
                        (w36 & ~0x03FFFFFFu) | r.Below(64) | (r.Below(64) << 13));
      }
      const std::vector<uint8_t> inputs = Read(h.base, h.data, 64);
      const uint32_t face = r.Below(4) ? r.Below(6) : uint32_t(r.Next());
      const uint32_t level = r.Below(16);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, h.data, inputs.data(), uint32_t(inputs.size()));
        PPCContext t = CallContext(h);
        guest::StoreU32(h.base, t.r1.u32 + 84, h.data + kOut + 20);
        guest::StoreU32(h.base, t.r1.u32 + 92, h.data + kOut + 24);
        guest::StoreU32(h.base, t.r1.u32 + 100, h.data + kOut + 28);
        t.r3.u64 = h.data + kHeader;
        t.r4.u64 = face;
        t.r5.u64 = level;
        t.r6.u64 = h.data + kOut;
        t.r7.u64 = h.data + kOut + 4;
        t.r8.u64 = h.data + kOut + 8;
        t.r9.u64 = h.data + kOut + 12;
        t.r10.u64 = h.data + kOut + 16;
        (v == 0 ? Original<__imp__sub_822A4FF0> : NativeLevelOffset)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kOut, 32));
        out[v].Bytes(Read(h.base, h.data, 64));
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: face {:X}, level {}, {}", i, face, level, Hex(inputs)));
    }
    results.Report();
  }
}

// Device::CreateTexture on random sizes (up to 512, so the data stays
// small), levels, usages, formats and the types it takes; compared: whether
// it made one, the header but its data addresses (+32's and +48's top 20
// bits: whether each is set), r3's low 12 bits. Each version's header and
// data freed after it. Texture::GetSurfaceLevel on random textures (their
// count at +4) and levels: the surface's 48 bytes, the texture's 64, whether
// r3 is set. XGSetTextureHeader on random sizes, levels, usages, formats,
// addresses (the mips' -1 now and then), pitches, with or without the
// header and the sizes out: what it writes and r3. Failed allocations
// aren't taken (the allocation layer stops the game when out of memory).
void TextureCreation(Context& h, Random& r) {
  {
    Results results("Device::CreateTexture (sub_822A5D08) with TextureHeader::Set");
    const uint32_t types[] = {3, 3, 3, 17, 18, 19, 20};
    const uint32_t usages[] = {0, 0x4, 0x200, 0x204, 0x8, 0x1};
    for (int i = 0; i < 150; i++) {
      const uint32_t type = types[r.Below(7)];
      const uint32_t w = 1 + r.Below(512), hh = type == 20 ? 1 : 1 + r.Below(512);
      const uint32_t d = type == 18 ? 6 : type == 17 || type == 19 ? 1 + r.Below(8) : 1;
      const uint32_t levels = r.Below(3) == 0 ? 0 : r.Below(10);
      const uint32_t usage = r.Below(4) ? usages[r.Below(6)] : uint32_t(r.Next());
      const uint32_t format = (uint32_t(r.Next()) & ~0x3Fu) | (r.Below(4) ? 6 : r.Below(64));
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = w;
        t.r4.u64 = hh;
        t.r5.u64 = d;
        t.r6.u64 = levels;
        t.r7.u64 = usage;
        t.r8.u64 = format;
        t.r9.u64 = 0;
        t.r10.u64 = type;
        (v == 0 ? Original<__imp__sub_822A5D08> : NativeCreateTexture)(t, h.base);
        const uint32_t header = t.r3.u32;
        out[v].Word(header != 0);
        out[v].Word(header & 0xFFF);
        if (header == 0) {
          continue;
        }
        std::vector<uint8_t> words = Read(h.base, header, 52);
        const uint32_t data = guest::LoadU32(h.base, header + 32) & 0xFFFFF000;
        const uint32_t mips = guest::LoadU32(h.base, header + 48) & 0xFFFFF000;
        out[v].Word(data != 0);
        out[v].Word(mips != 0);
        for (int k = 0; k < 3; k++) {  // the addresses' top 20 bits
          words[32 + k] = 0;
          words[48 + k] = 0;
        }
        words[35] &= 0x0F;
        words[51] &= 0x0F;
        out[v].Bytes(words);
        const uint32_t attributes = (usage & 0x4) ? 0xAC800000 : 0xBC800000;
        if (mips) {
          memory::crt::XMemFree(mips, attributes);
        }
        memory::crt::XMemFree(data, attributes);
        memory::crt::XMemFree(header, 0x24800000);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: type {}, {} x {} x {}, {} levels, usage {:X}, format "
                                "{:X}",
                                i, type, w, hh, d, levels, usage, format));
    }
    results.Report();
  }
  {
    Results results("Texture::GetSurfaceLevel (sub_822A5B80)");
    for (int i = 0; i < 300; i++) {
      Fill(h.base, h.data, kScratchBytes);
      RandomTexture(h.base, h.data + kHeader, r);
      RandomTexture(h.base, h.data + kParent, r);
      // A first reference of a resource holding a child AddRefs it too.
      guest::StoreU32(h.base, h.data + kHeader + 4, r.Below(100));
      guest::StoreU32(h.base, h.data + kHeader + 24, h.data + kParent);
      guest::StoreU32(h.base, h.data + kParent + 24, h.data + kParent);
      const std::vector<uint8_t> inputs = Read(h.base, h.data, 128);
      // Below 32: the original's Texture::LevelOffset call walks the levels.
      const uint32_t level = r.Below(4) ? r.Below(13) : r.Below(32);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, h.data, inputs.data(), uint32_t(inputs.size()));
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kHeader;
        t.r4.u64 = level;
        (v == 0 ? Original<__imp__sub_822A5B80> : NativeGetSurfaceLevel)(t, h.base);
        const uint32_t surface = t.r3.u32;
        out[v].Word(surface != 0);
        out[v].Bytes(Read(h.base, h.data, 128));
        if (surface != 0) {
          out[v].Bytes(Read(h.base, surface, 48));
          memory::crt::XMemFree(surface, 0x24800000);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}: level {:X}, {}", i, level, Hex(inputs)));
    }
    results.Report();
  }
  {
    Results results("XGSetTextureHeader (sub_8235A920) with XGSetTextureHeaderEx (sub_8235A720)");
    const uint32_t usages[] = {0, 0x4, 0x200, 0x204, 0x8};
    for (int i = 0; i < 2000; i++) {
      const uint32_t w = r.Below(8) == 0 ? (r.Below(2) ? 1 : 3) : TextureSide(r);
      const uint32_t hh = r.Below(8) == 0 ? (r.Below(2) ? 1 : 3) : TextureSide(r);
      const uint32_t levels = r.Below(3) == 0 ? 0 : r.Below(3) == 0 ? 1 : r.Below(14);
      const uint32_t usage = r.Below(4) ? usages[r.Below(5)] : uint32_t(r.Next());
      const uint32_t format = uint32_t(r.Next());
      const uint32_t base_address = uint32_t(r.Next());
      const uint32_t mip_address = r.Below(3) == 0 ? 0xFFFFFFFF : uint32_t(r.Next());
      const uint32_t pitch = r.Below(2) ? 0 : r.Below(2) ? 32 * r.Below(256) : uint32_t(r.Next());
      const bool header = r.Below(8) != 0;
      const bool sizes = r.Below(8) != 0;
      const uint32_t r8 = uint32_t(r.Next());
      std::vector<uint8_t> words(64);
      for (uint8_t& b : words) {
        b = uint8_t(r.Next());
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, h.data + kHeader, words.data(), uint32_t(words.size()));
        PPCContext t = CallContext(h);
        const uint32_t stack = t.r1.u32;
        guest::StoreU32(h.base, stack + 84, pitch);
        guest::StoreU32(h.base, stack + 92, header ? h.data + kHeader : 0);
        guest::StoreU32(h.base, stack + 100, sizes ? h.data + kOut : 0);
        guest::StoreU32(h.base, stack + 108, sizes ? h.data + kOut + 4 : 0);
        t.r3.u64 = w;
        t.r4.u64 = hh;
        t.r5.u64 = levels;
        t.r6.u64 = usage;
        t.r7.u64 = format;
        t.r8.u64 = r8;
        t.r9.u64 = base_address;
        t.r10.u64 = mip_address;
        (v == 0 ? Original<__imp__sub_8235A920> : NativeXGSetTextureHeader)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kHeader, 64));
        out[v].Bytes(Read(h.base, h.data + kOut, 8));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {} x {}, {} levels, usage {:X}, format {:X}, base {:X}, "
                                "mips {:X}, pitch {:X}, header {}, sizes {}",
                                i, w, hh, levels, usage, format, base_address, mip_address,
                                pitch, header, sizes));
    }
    results.Report();
  }
}

void BlockSizes(Context& h, Random& r) {
  Results results("GpuFormat::BlockSize (sub_822A4410)");
  for (int i = 0; i < 256; i++) {
    const uint32_t format = i < 128 ? uint32_t(i) : Word32(r);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = format;
      t.r4.u64 = h.data + kOut;
      t.r5.u64 = h.data + kOut + 4;
      (v == 0 ? Original<__imp__sub_822A4410> : NativeBlockSize)(t, h.base);
      out[v].Bytes(Read(h.base, h.data + kOut, 8));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("format {}", format));
  }
  results.Report();
}

void SizeMath(Context& h, Random& r) {
  {
    // A linear format whose block has no bytes (bits 0) hits the divide
    // trap: a few such cases (each version logs a warning).
    Results results("TextureLayout::LevelSize (sub_822A45B8)");
    const uint32_t bits[] = {1, 2, 4, 8, 16, 32, 64, 128};
    for (int i = 0; i < 1000; i++) {
      const uint32_t w = r.Below(3) ? r.Below(4097) : Size(r);
      const uint32_t hh = r.Below(3) ? r.Below(4097) : Size(r);
      const uint32_t d = r.Below(3) ? r.Below(65) : Size(r);
      const uint32_t b = i < 4 ? 0 : r.Below(8) ? bits[r.Below(8)] : uint32_t(r.Next());
      const uint32_t format = r.Below(4) ? r.Below(64) : uint32_t(r.Next());
      const uint32_t dimension = r.Below(8) ? r.Below(4) : uint32_t(r.Next());
      const uint32_t tiled = i < 4 ? 0 : r.Below(4) ? r.Below(2) : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        guest::StoreU32(h.base, h.data + kOut, w);
        guest::StoreU32(h.base, h.data + kOut + 4, hh);
        guest::StoreU32(h.base, h.data + kOut + 8, d);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kOut;
        t.r4.u64 = h.data + kOut + 4;
        t.r5.u64 = h.data + kOut + 8;
        t.r6.u64 = b;
        t.r7.u64 = format;
        t.r8.u64 = dimension;
        t.r9.u64 = tiled;
        (v == 0 ? Original<__imp__sub_822A45B8> : NativeLevelSize)(t, h.base);
        out[v].Bytes(Read(h.base, h.data + kOut, 12));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("{} x {} x {}, {} bits, format {:X}, dimension {:X}, tiled {:X}",
                                w, hh, d, b, format, dimension, tiled));
    }
    results.Report();
  }
  {
    Results results("TextureLayout::LevelCount (sub_822A4530)");
    for (int i = 0; i < 1000; i++) {
      const uint32_t w = Size(r), hh = Size(r), d = Size(r);
      const uint32_t border = r.Below(4) == 0 ? Size(r) : r.Below(3);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = w;
        t.r4.u64 = hh;
        t.r5.u64 = d;
        t.r6.u64 = border;
        (v == 0 ? Original<__imp__sub_822A4530> : NativeLevelCount)(t, h.base);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("{} x {} x {}, border {}", w, hh, d, border));
    }
    results.Report();
  }
  {
    Results results("Surface::EdramTiles (sub_8209AD68)");
    const uint32_t formats[] = {21, 32, 37, 6, 22, 23, 26, 0x40 | 21};
    for (int i = 0; i < 1000; i++) {
      const uint32_t w = r.Below(3) ? r.Below(2048) : Size(r);
      const uint32_t hh = r.Below(3) ? r.Below(2048) : Size(r);
      const uint32_t format = r.Below(2) ? formats[r.Below(8)] : Word32(r);
      const uint32_t multisample = r.Below(4) ? r.Below(4) : Word32(r);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = w;
        t.r4.u64 = hh;
        t.r5.u64 = format;
        t.r6.u64 = multisample;
        (v == 0 ? Original<__imp__sub_8209AD68> : NativeEdramTiles)(t, h.base);
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1],
                    fmt::format("{} x {}, format {:X}, samples {:X}", w, hh, format, multisample));
    }
    results.Report();
  }
}

// The buffers: both versions allocate, so the header's data address (+24)
// differs between them; its low 12 bits (the flags, the data 4 KiB aligned)
// are compared, and every other header word exactly. Each version's header
// and data are freed after it. The paths where an allocation fails aren't
// taken: the allocation layer stops the game when out of memory.
void Buffers(Context& h, Random& r) {
  const uint32_t usages[] = {0, 0x4, 0x200, 0x204, 0x8, 0x10};
  for (int index = 0; index < 2; index++) {
    Results results(index ? "Device::CreateIndexBuffer (sub_822A6968)"
                          : "Device::CreateVertexBuffer (sub_822A6830)");
    for (int i = 0; i < 60; i++) {
      const uint32_t bytes = 4 + r.Below(65536);
      const uint32_t usage = r.Below(4) ? usages[r.Below(6)] : uint32_t(r.Next());
      const uint32_t format = r.Below(4) ? r.Below(2) : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        t.r3.u64 = bytes;
        t.r4.u64 = usage;
        t.r5.u64 = format;
        t.r6.u64 = 0;
        GuestFunction f = index ? (v == 0 ? Original<__imp__sub_822A6968> : NativeCreateIndexBuffer)
                                : (v == 0 ? Original<__imp__sub_822A6830> : NativeCreateVertexBuffer);
        f(t, h.base);
        const uint32_t header = t.r3.u32;
        out[v].Word(header != 0);
        if (header == 0) {
          continue;
        }
        std::vector<uint8_t> words = Read(h.base, header, 32);
        const uint32_t data = guest::LoadU32(h.base, header + 24);
        out[v].Word(data & 0xFFF);
        out[v].Word((data & ~0xFFFu) != 0);
        for (int k = 0; k < 4; k++) {
          words[24 + k] = 0;
        }
        out[v].Bytes(words);
        const uint32_t protection = (usage & 0x4) ? 2 : 3;
        memory::crt::XMemFree(data & ~0xFFFu, 0x82800000u | (protection << 28));
        memory::crt::XMemFree(header, 0x24800000);
      }
      results.Check(out[0], out[1],
                    fmt::format("{} bytes, usage {:X}, format {:X}", bytes, usage, format));
    }
    results.Report();
  }
}

// The device's getters on a block standing for the device (its slots and
// the streams' fetch constants and strides random, a slot null now and
// then) and resources in the scratch area: some type 4 with the child bit,
// whose first reference also AddRefs their parent. Compared: the outputs,
// r3 and every resource's count.
void Getters(Context& h, Random& r) {
  constexpr uint32_t kDeviceBytes = 12304;
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  if (device == 0) {
    ITC_LOG_INFO("harness: graphics_resources: no block for the device");
    return;
  }
  constexpr uint32_t kResources = 8;
  constexpr uint32_t kResourceBytes = 64;
  auto resource = [&](uint32_t i) { return h.data + kResourceBytes * i; };
  struct Getter {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Getter getters[] = {
      {"Device::GetRenderTarget (sub_822A3998)", Original<__imp__sub_822A3998>,
       NativeGetRenderTarget},
      {"Device::GetDepthStencilSurface (sub_822A39E0)", Original<__imp__sub_822A39E0>,
       NativeGetDepthStencilSurface},
      {"Device::GetStreamSource (sub_822A3880)", Original<__imp__sub_822A3880>,
       NativeGetStreamSource},
  };
  for (const Getter& getter : getters) {
    Results results(getter.name);
    for (int i = 0; i < 200; i++) {
      // The inputs, the same for both versions.
      std::vector<uint32_t> device_words(kDeviceBytes / 4);
      for (uint32_t& w : device_words) {
        w = uint32_t(r.Next());
      }
      // The slots: +12184 to +12264 (render targets, depth, streams).
      for (uint32_t slot = 12184; slot < 12272; slot += 4) {
        device_words[slot / 4] = r.Below(5) == 0 ? 0 : resource(r.Below(kResources));
      }
      std::vector<uint8_t> resources(kResources * kResourceBytes);
      for (uint8_t& b : resources) {
        b = uint8_t(r.Next());
      }
      for (uint32_t k = 0; k < kResources; k++) {
        uint8_t* p = resources.data() + kResourceBytes * k;
        const uint32_t type = r.Below(2) ? 4 : r.Below(16);
        p[3] = uint8_t((p[3] & 0xF0) | type);
        p[0] = uint8_t(r.Below(2) ? (p[0] | 0x40) : (p[0] & ~0x40));
        // The count: 0 now and then (a first reference).
        const uint32_t count = r.Below(3) == 0 ? 0 : r.Below(100);
        p[4] = 0, p[5] = 0, p[6] = 0, p[7] = uint8_t(count);
        const uint32_t parent = resource(r.Below(kResources));
        p[24] = uint8_t(parent >> 24), p[25] = uint8_t(parent >> 16);
        p[26] = uint8_t(parent >> 8), p[27] = uint8_t(parent);
      }
      const uint32_t index = r.Below(4);  // the render targets
      const uint32_t stream = r.Below(16);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Write(h.base, h.data, resources.data(), uint32_t(resources.size()));
        guest::StoreU32s(h.base, device, device_words.data(), uint32_t(device_words.size()));
        PPCContext t = CallContext(h);
        t.r3.u64 = device;
        t.r4.u64 = &getter == &getters[2] ? stream : index;
        t.r5.u64 = h.data + 1024;
        t.r6.u64 = h.data + 1028;
        (v == 0 ? getter.recompiled : getter.native)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Bytes(Read(h.base, h.data + 1024, 8));
        for (uint32_t k = 0; k < kResources; k++) {
          out[v].Word(guest::LoadU32(h.base, resource(k) + 4));
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}: index {}, stream {}", i, index, stream));
    }
    results.Report();
  }
  memory::Free(device);
}


// --- EDRAM ---

namespace edram = gr::edram;

constexpr uint32_t kObjectAttributes = 0x64800000;
constexpr uint32_t kHiZTaken = 0x829537C4;

// An allocator's state: the bitmap's 192 words and the free count.
struct EdramState {
  std::vector<uint32_t> bitmap;
  uint32_t free;
};

// Runs of free and used tiles of random lengths (32-aligned now and then);
// the free count true, or now and then random.
EdramState RandomEdram(Random& r) {
  EdramState state{std::vector<uint32_t>(edram::kTiles / 32, 0), 0};
  uint32_t tile = 0;
  bool used = r.Below(2) != 0;
  const uint32_t scale = r.Below(3) == 0 ? 2000 : r.Below(2) ? 300 : 40;
  while (tile < edram::kTiles) {
    uint32_t run = 1 + r.Below(scale);
    if (r.Below(4) == 0) {
      run = (run + 31) & ~31u;
    }
    for (uint32_t i = 0; i < run && tile < edram::kTiles; i++, tile++) {
      if (used) {
        state.bitmap[tile / 32] |= 0x80000000u >> (tile % 32);
      } else {
        state.free++;
      }
    }
    used = !used;
  }
  if (r.Below(8) == 0) {
    state.free = r.Below(edram::kTiles + 1);
  }
  return state;
}

// An allocator object with the state (from the allocation layer, as the
// game's); for the recompiled side its critical section is set up.
uint32_t MakeAllocator(Context& h, const EdramState& state, bool recompiled) {
  const uint32_t object = memory::crt::XMemAlloc(h.base, edram::kObjectBytes, kObjectAttributes);
  guest::StoreU32(h.base, object + edram::kFree, state.free);
  guest::StoreU32s(h.base, object + edram::kBitmap, state.bitmap.data(),
                   uint32_t(state.bitmap.size()));
  guest::StoreU32(h.base, object + edram::kBitmapEnd, edram::kSentinel);
  if (recompiled) {
    PPCContext t = CallContext(h);
    t.r3.u64 = object;
    __imp__RtlInitializeCriticalSection(t, h.base);
  }
  return object;
}

// The allocator's state but its critical section (+0 to +27).
std::vector<uint8_t> AllocatorState(Context& h, uint32_t object) {
  return Read(h.base, object + edram::kFree, edram::kObjectBytes - edram::kFree);
}

// A position or a run's end: anywhere, at a word's edge, or at the end.
uint32_t Tile(Random& r) {
  switch (r.Below(5)) {
    case 0:
      return r.Below(edram::kTiles / 32 + 1) * 32;
    case 1:
      return edram::kTiles - r.Below(40);
    default:
      return r.Below(edram::kTiles);
  }
}


// A surface's format: the depth formats (22, 23), 54 (taken as 7), others,
// with the bits SurfaceHeader::Set tests (0x600, 0x1800, 0x6000 all set,
// 0x20000 and 0x18000 clear) now and then, or random.
uint32_t SurfaceFormat(Random& r) {
  const uint32_t lows[] = {22, 23, 54, 6, 7, 26, 21, 32, 37, 0, 1};
  uint32_t format = r.Below(4) ? lows[r.Below(11)] : r.Below(64);
  switch (r.Below(4)) {
    case 0:
      format |= 0x7E00;
      break;
    case 1:
      format |= uint32_t(r.Next()) & ~0x3Fu;
      break;
    default:
      format |= (uint32_t(r.Next()) & 0x7FFC0) & ~0x18000u;
      break;
  }
  return format;
}

int32_t Multisample(Random& r) {
  return r.Below(8) ? int32_t(r.Below(3)) : int32_t(uint32_t(r.Next()));
}

// SurfaceHeader::Set on a header of random words (its +36's low 3 bits
// kept), with or without parameters, the hi-z taken or not (0x829537C4,
// put back after).
void SurfaceHeaders(Context& h, Random& r) {
  Results results("SurfaceHeader::Set (sub_822A5910)");
  const uint32_t hi_z = guest::LoadU32(h.base, kHiZTaken);
  for (int i = 0; i < 1000; i++) {
    const uint32_t w = r.Below(4) ? 1 + r.Below(2048) : Size(r);
    const uint32_t hh = r.Below(4) ? 1 + r.Below(2048) : Size(r);
    const uint32_t format = SurfaceFormat(r);
    const int32_t multisample = Multisample(r);
    const bool with_parameters = r.Below(3) == 0;
    const uint32_t taken = r.Below(2);
    uint32_t words[16];
    for (uint32_t& word : words) {
      word = uint32_t(r.Next());
    }
    if (r.Below(2)) {
      words[1] = 0xFFFFFFFF;  // the hi-z base none
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32s(h.base, h.data + kHeader, words, 12);
      guest::StoreU32s(h.base, h.data + kParent, words + 12, 3);
      guest::StoreU32(h.base, kHiZTaken, taken);
      PPCContext t = CallContext(h);
      t.r3.u64 = w;
      t.r4.u64 = hh;
      t.r5.u64 = format;
      t.r6.s64 = multisample;
      t.r7.u64 = with_parameters ? h.data + kParent : 0;
      t.r8.u64 = h.data + kHeader;
      t.r9.u64 = h.data + kOut;
      t.r10.u64 = h.data + kOut + 4;
      (v == 0 ? Original<__imp__sub_822A5910> : NativeSetSurfaceHeader)(t, h.base);
      out[v].Bytes(Read(h.base, h.data + kHeader, 48));
      out[v].Bytes(Read(h.base, h.data + kOut, 8));
      out[v].Word(t.r3.u32);
      out[v].Word(guest::LoadU32(h.base, kHiZTaken));
    }
    results.Check(out[0], out[1],
                  fmt::format("{} x {}, format {:X}, samples {:X}, parameters {}, hi-z {}", w, hh,
                              format, uint32_t(multisample), with_parameters, taken));
  }
  guest::StoreU32(h.base, kHiZTaken, hi_z);
  results.Report();
}

void EdramLeaves(Context& h, Random& r) {
  {
    Results results("Edram::NextRun (sub_822B5C90)");
    for (int i = 0; i < 600; i++) {
      EdramState state = RandomEdram(r);
      if (i % 50 == 0) {
        // Used to the end from somewhere.
        for (uint32_t t = Tile(r); t < edram::kTiles; t++) {
          state.bitmap[t / 32] |= 0x80000000u >> (t % 32);
        }
      }
      const uint32_t position = i % 37 == 0 ? edram::kTiles : Tile(r);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t object = MakeAllocator(h, state, false);
        guest::StoreU32(h.base, h.data, position);
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        t.r4.u64 = h.data;
        (v == 0 ? Original<__imp__sub_822B5C90> : NativeNextRun)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(guest::LoadU32(h.base, h.data));
        out[v].Bytes(AllocatorState(h, object));
        memory::crt::XMemFree(object, kObjectAttributes);
      }
      results.Check(out[0], out[1], fmt::format("case {}: position {}", i, position));
    }
    results.Report();
  }
  {
    Results results("Edram::Mark (sub_822B5D70)");
    for (int i = 0; i < 600; i++) {
      const EdramState state = RandomEdram(r);
      uint32_t start = Tile(r), end = Tile(r);
      // Now and then reversed (the start's word set, the next word's bits
      // before the end set), but inside the object.
      if (start > end && (r.Below(8) != 0 || start >= edram::kTiles - 32)) {
        std::swap(start, end);
      }
      if (r.Below(6) == 0) {
        end = start + r.Below(32);  // in one word, or across one edge
        if (end > edram::kTiles) {
          end = edram::kTiles;
        }
      }
      const uint32_t used = r.Below(2);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t object = MakeAllocator(h, state, false);
        PPCContext t = CallContext(h);
        t.r3.u64 = object;
        t.r4.u64 = start;
        t.r5.u64 = end;
        t.r6.u64 = used;
        (v == 0 ? Original<__imp__sub_822B5D70> : NativeMark)(t, h.base);
        out[v].Bytes(AllocatorState(h, object));
        memory::crt::XMemFree(object, kObjectAttributes);
      }
      results.Check(out[0], out[1], fmt::format("case {}: [{}, {}) {}", i, start, end, used));
    }
    results.Report();
  }
}

// Alloc, Free and the surface's destroy work on the allocator at
// [0x82953E20]: each case puts its own there (the recompiled side's with
// its critical section set up, the native side's without), and the game's
// is put back after (the harnesses run on the main thread, before any race
// makes surfaces). Compared: the allocator from +28 on (the free count, the
// bitmap, the sentinel), the out and r3; the creation's critical section
// apart (expected to differ).
void EdramAllocator(Context& h, Random& r) {
  const uint32_t saved = guest::LoadU32(h.base, edram::kAllocator);
  {
    Results results("Edram::Alloc (sub_822B5EF8)");
    for (int i = 0; i < 400; i++) {
      const EdramState state = RandomEdram(r);
      uint32_t tiles = r.Below(4) == 0 ? r.Below(2049) : r.Below(400);
      if (r.Below(16) == 0) {
        tiles = state.free + r.Below(3);
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t object = MakeAllocator(h, state, v == 0);
        guest::StoreU32(h.base, edram::kAllocator, object);
        Fill(h.base, h.data, 16);
        PPCContext t = CallContext(h);
        t.r3.u64 = tiles;
        t.r4.u64 = h.data;
        (v == 0 ? Original<__imp__sub_822B5EF8> : NativeEdramAlloc)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(guest::LoadU32(h.base, h.data));
        out[v].Word(guest::LoadU32(h.base, edram::kAllocator) == object);
        out[v].Bytes(AllocatorState(h, object));
        memory::crt::XMemFree(object, kObjectAttributes);
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} tiles, {} free", i, tiles, state.free));
    }
    results.Report();
  }
  {
    // The first use: no allocator yet, one made. Its critical section is
    // the kernel's in the recompiled version and left cleared in the native
    // one (the user's call, 2026-10-24: a std::mutex in its place).
    Results results("Edram::Alloc (sub_822B5EF8), the allocator made");
    Results cs("Edram::Alloc (sub_822B5EF8), the allocator made: its critical section "
               "(expected to differ)");
    for (int i = 0; i < 8; i++) {
      const uint32_t tiles = i == 0 ? 0 : r.Below(2049);
      Outputs out[2], section[2];
      for (int v = 0; v < 2; v++) {
        guest::StoreU32(h.base, edram::kAllocator, 0);
        Fill(h.base, h.data, 16);
        PPCContext t = CallContext(h);
        t.r3.u64 = tiles;
        t.r4.u64 = h.data;
        (v == 0 ? Original<__imp__sub_822B5EF8> : NativeEdramAlloc)(t, h.base);
        const uint32_t object = guest::LoadU32(h.base, edram::kAllocator);
        out[v].Word(t.r3.u32);
        out[v].Word(guest::LoadU32(h.base, h.data));
        out[v].Word(object != 0);
        if (object != 0) {
          out[v].Bytes(AllocatorState(h, object));
          section[v].Bytes(Read(h.base, object, edram::kFree));
          memory::crt::XMemFree(object, kObjectAttributes);
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} tiles", i, tiles));
      cs.Check(section[0], section[1], fmt::format("case {}", i));
    }
    results.Report();
    cs.Report();
  }
  {
    Results results("Edram::Free (sub_822B5EA8)");
    for (int i = 0; i < 300; i++) {
      const EdramState state = RandomEdram(r);
      const uint32_t start = Tile(r);
      uint32_t tiles = r.Below(3) == 0 ? r.Below(32) : r.Below(1024);
      if (start + tiles > edram::kTiles) {
        tiles = edram::kTiles - start;
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t object = MakeAllocator(h, state, v == 0);
        guest::StoreU32(h.base, edram::kAllocator, object);
        PPCContext t = CallContext(h);
        t.r3.u64 = start;
        t.r4.u64 = tiles;
        (v == 0 ? Original<__imp__sub_822B5EA8> : NativeEdramFree)(t, h.base);
        out[v].Bytes(AllocatorState(h, object));
        memory::crt::XMemFree(object, kObjectAttributes);
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} at {}", i, tiles, start));
    }
    results.Report();
  }
  {
    // The surface's destroy: a 48-byte header from the allocation layer
    // (freed by it), entered with Destroy's 112-byte frame (r29-r31 and lr
    // in it, restored at the end; compared too). The hi-z flag is put back.
    Results results("Resource::Destroy's surface case (sub_822A6268)");
    const uint32_t hi_z = guest::LoadU32(h.base, kHiZTaken);
    for (int i = 0; i < 300; i++) {
      const EdramState state = RandomEdram(r);
      uint32_t common = uint32_t(r.Next());
      common = (common & ~0xFu) | 4;
      const uint32_t edram_base = r.Below(2048);
      const uint32_t tiles = r.Below(4) == 0 ? r.Below(32) : r.Below(1024);
      const uint32_t w28 = (uint32_t(r.Next()) & ~0xFFFu) | edram_base;
      const uint32_t w32 = uint32_t(r.Next());
      const uint32_t w44 = tiles * 5120 + r.Below(5120);
      const uint32_t frame[4] = {uint32_t(r.Next()), uint32_t(r.Next()), uint32_t(r.Next()),
                                 0x82000000u | (uint32_t(r.Next()) & 0xFFFFFC)};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t object = MakeAllocator(h, state, v == 0);
        guest::StoreU32(h.base, edram::kAllocator, object);
        guest::StoreU32(h.base, kHiZTaken, 1);
        const uint32_t surface = memory::crt::XMemAlloc(h.base, 48, kObjectAttributes);
        guest::StoreU32(h.base, surface, common);
        guest::StoreU32(h.base, surface + 28, w28);
        guest::StoreU32(h.base, surface + 32, w32);
        guest::StoreU32(h.base, surface + 44, w44);
        PPCContext t = CallContext(h);
        const uint32_t top = t.r1.u32;
        // Destroy's frame: r29-r31 saved at top - 32, -24, -16, lr at -8.
        for (int k = 0; k < 3; k++) {
          StoreU64(h.base, top - 32 + 8 * k, frame[k]);
        }
        guest::StoreU32(h.base, top - 8, frame[3]);
        t.r1.u64 = top - 112;
        t.r31.u64 = surface;
        t.r11.u64 = common;
        (v == 0 ? Original<__imp__sub_822A6268> : NativeDestroySurface)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Word(t.r1.u32 - top);
        out[v].Word(t.r29.u32);
        out[v].Word(t.r30.u32);
        out[v].Word(t.r31.u32);
        out[v].Word(uint32_t(t.lr));
        out[v].Word(guest::LoadU32(h.base, kHiZTaken));
        out[v].Bytes(AllocatorState(h, object));
        memory::crt::XMemFree(object, kObjectAttributes);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {:08X}, {} at {}", i, common, tiles, edram_base));
    }
    guest::StoreU32(h.base, kHiZTaken, hi_z);
    results.Report();
  }
  {
    // CreateSurface: the header from the allocation layer (freed after,
    // with its EDRAM left in the case's allocator), its EDRAM from the
    // case's allocator (as for Alloc; now and then too full, or a base past
    // 2,048 tiles), the hi-z taken or not.
    Results results("Device::CreateSurface (sub_822A5E28)");
    const uint32_t hi_z = guest::LoadU32(h.base, kHiZTaken);
    for (int i = 0; i < 400; i++) {
      EdramState state = RandomEdram(r);
      if (r.Below(4) == 0) {
        // Free only past tile 2,048 now and then (the base too high).
        for (uint32_t t = 0; t < 2048 + r.Below(256); t++) {
          if (!(state.bitmap[t / 32] & (0x80000000u >> (t % 32)))) {
            state.bitmap[t / 32] |= 0x80000000u >> (t % 32);
            state.free--;
          }
        }
      }
      const uint32_t w = 1 + r.Below(r.Below(4) ? 1280 : 4096);
      const uint32_t hh = 1 + r.Below(r.Below(4) ? 720 : 4096);
      const uint32_t format = SurfaceFormat(r);
      const int32_t multisample = r.Below(8) ? int32_t(r.Below(3)) : int32_t(r.Below(16));
      const bool with_parameters = r.Below(8) == 0;
      const uint32_t taken = r.Below(2);
      const uint32_t parameters[3] = {uint32_t(r.Next()), r.Below(2) ? 0xFFFFFFFF : uint32_t(r.Next()),
                                      uint32_t(r.Next())};
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t object = MakeAllocator(h, state, v == 0);
        guest::StoreU32(h.base, edram::kAllocator, object);
        guest::StoreU32(h.base, kHiZTaken, taken);
        Fill(h.base, h.data, kScratchBytes);
        guest::StoreU32s(h.base, h.data + kParent, parameters, 3);
        PPCContext t = CallContext(h);
        t.r3.u64 = w;
        t.r4.u64 = hh;
        t.r5.u64 = format;
        t.r6.s64 = multisample;
        t.r7.u64 = with_parameters ? h.data + kParent : 0;
        (v == 0 ? Original<__imp__sub_822A5E28> : NativeCreateSurface)(t, h.base);
        const uint32_t surface = t.r3.u32;
        out[v].Word(surface != 0);
        if (surface != 0) {
          out[v].Bytes(Read(h.base, surface, 48));
          memory::crt::XMemFree(surface, 0x24800000);
        }
        out[v].Word(guest::LoadU32(h.base, kHiZTaken));
        out[v].Bytes(AllocatorState(h, object));
        memory::crt::XMemFree(object, kObjectAttributes);
      }
      results.Check(out[0], out[1],
                    fmt::format("case {}: {} x {}, format {:X}, samples {}, parameters {}, hi-z {}",
                                i, w, hh, format, multisample, with_parameters, taken));
    }
    guest::StoreU32(h.base, kHiZTaken, hi_z);
    results.Report();
  }
  guest::StoreU32(h.base, edram::kAllocator, saved);
}

}  // namespace

void GraphicsResources(Context& h) {
  Random r;
  Headers(h, r);
  BlockSizes(h, r);
  SizeMath(h, r);
  LevelDescs(h, r);
  TextureHeaders(h, r);
  LevelOffsets(h, r);
  TextureCreation(h, r);
  Buffers(h, r);
  Getters(h, r);
  SurfaceHeaders(h, r);
  EdramLeaves(h, r);
  EdramAllocator(h, r);
}

}  // namespace harness
