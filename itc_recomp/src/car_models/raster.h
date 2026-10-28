// The decals' software rasterizer as plain C++ (docs/functions/car-models.md:
// Vinyl::DrawDecal and the triangles, spans, pixels and texels under it),
// step for step as the translated code: its float steps
// (texture_compression/steps.h, the operands in its order), its integer
// blends, and its stack frames' words (each function's locals written where
// the original writes them, its saved registers not). Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt); flush-to-zero
// off is the adapter's job. Adapter: src/hooks/car_models.cpp.
//
// The structures, all in guest memory as the original keeps them:
// - a decal image (16 bytes): +0 the ARGB pixels, +4 the width, +8 the
//   height, +12 the pitch in bytes;
// - the canvas: a word holding the address of the region's first pixel, and
//   the region (12 bytes): +0 its width, +4 its height, +8 the sheet's pitch;
// - a vertex: its position or its texel (two floats, in 16 bytes; the other
//   8 bytes are carried along).

#pragma once

#include <cstdint>
#include <optional>

namespace car_models {

// The frames (bytes).
constexpr uint32_t kDrawDecalFrameBytes = 400;
constexpr uint32_t kDrawTriangleFrameBytes = 496;
constexpr uint32_t kDrawSpanFrameBytes = 272;
constexpr uint32_t kBlendPixelFrameBytes = 112;

// The regions of the sheet (16 bytes each: left, top, right, bottom) and its
// width (the pitch is 4 times it).
constexpr uint32_t kVinylRegions = 0x82602A50;
constexpr uint32_t kVinylSheetWidth = 0x82602570;
// The floats the rasterizer reads: 0.5, 0.0 and the texel weights' scale.
constexpr uint32_t kRasterHalf = 0x82001BE4;
constexpr uint32_t kRasterZero = 0x82000A34;
constexpr uint32_t kTexelScale = 0x82001FAC;

// sub_820ED2A0: the decal's texel at (u, v), bilinear: the four pixels
// around it (clamped to the image), weighted by the fractions times the
// scale (as integers out of 256) with the packed blend of BlendPixel. Its
// conversions' words written at `scratch` (its caller's r1 - 80: the words
// at -80 to -65). Returns the ARGB word.
uint32_t SampleTexel(uint8_t* base, uint32_t image, double u, double v, uint32_t scratch);

// sub_820ED498 SoftRaster::BlendPixel: the canvas pixel at (x, y) blended
// with the texel at the floats at `uv` by its alpha (skipped for 0). `stack`
// is the guest r1 at entry.
void BlendPixel(uint8_t* base, uint32_t image, uint32_t canvas, uint32_t region, int32_t x,
                int32_t y, uint32_t uv, uint32_t stack);

// sub_820EDE00 SoftRaster::DrawSpan: row `y` from xa to xb (either order;
// the right end left out), its texels from the vertices at `a` and `b`
// (each end's), less 0.5 when the flags word at `stack` + 84 has its top bit;
// clipped to the region, each pixel blended. `stack` is the guest r1 at
// entry.
void DrawSpan(uint8_t* base, uint32_t canvas, uint32_t region, int32_t xa, int32_t xb,
              int32_t y, uint32_t a, uint32_t b, uint32_t image, uint32_t stack);

// sub_820EE0C8 SoftRaster::DrawTriangle: the three vertices at `positions`
// (16 bytes apart) with their texels at `texels`, sorted by y and filled row
// by row from the rows' centers (spans of the top half, then the bottom),
// `flags` (its low 29 bits, with the top bit set) handed to each span.
// `stack` is the guest r1 at entry.
void DrawTriangle(uint8_t* base, uint32_t canvas, uint32_t region, uint32_t positions,
                  uint32_t texels, uint32_t image, uint32_t flags, uint32_t stack);

// sub_820ED1E8: a decal's corner: (p - c) scaled by `scale`, rotated by
// `angle` (sin and cos: c_runtime), plus c, plus the offset `t`, into `out`
// (which may be `p`).
void DecalCorner(uint8_t* base, uint32_t out, uint32_t p, uint32_t c, uint32_t scale,
                 uint32_t t, double angle);

// sub_820EE638 Vinyl::DrawDecal's registers: r3 the region, r4 mirrored, r5
// to r7 the TGA header's 20 bytes (r7's top word its last 4), r8 and r9 the
// record's +16 to +31, f1 the rotation.
struct DecalArguments {
  uint32_t region;
  uint32_t mirrored;
  uint64_t header[3];
  uint64_t record[2];
  double angle;
};

// Whether Vinyl::DrawDecal's region is one ported here: 0 to 9 (region 9,
// the whole sheet, is the recompiler's split-off sub_820EE698, the same
// steps; above 9 the original's switch leaves its region's frame words as
// they were, never in play).
bool DecalRegionPorted(uint32_t region);

// sub_820EE638 Vinyl::DrawDecal: the decal of the image whose header and
// pixels its caller left (the header from the registers, spilled to `stack`
// + 32 to + 79 as the original does; the scale at `stack` + 96, the pixels'
// address at + 116, the sheet's at + 132) as a quad the TGA's size in the
// region's rectangle (kVinylRegions), its corners through DecalCorner, its
// texels mirrored left to right when asked, drawn as two triangles. `stack`
// is the guest r1 at entry. Returns the original's r3 (`stack` + 32);
// nullopt, with nothing done, for a region not DecalRegionPorted.
std::optional<uint32_t> DrawDecal(uint8_t* base, const DecalArguments& arguments,
                                  uint32_t stack);

}  // namespace car_models
