// The graphics library's resources (src/graphics/,
// docs/functions/graphics-resources.md) against the recompiled functions.
// The header leaves run on headers of random words in the scratch area
// (with the type, dimension and child bits drawn so each branch is taken),
// the size math on random sizes (edge values now and then); outputs and r3
// are compared exactly (none is a float). The originals run with
// --native_graphics_resources off.

#include "harness.h"

#include "base/flags.h"
#include "graphics/resources.h"

#include <fmt/format.h>

#include <string>
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

}  // namespace

void GraphicsResources(Context& h) {
  Random r;
  Headers(h, r);
  BlockSizes(h, r);
  SizeMath(h, r);
}

}  // namespace harness
