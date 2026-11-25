// The decals' software rasterizer as plain C++ (docs/functions/car-models.md:
// Vinyl::DrawDecal and the triangles, spans, pixels and texels under it),
// step for step as the translated code: its float steps
// (texture_compression/steps.h, the operands in its order), its integer
// blends, and DrawDecal's stack frame words (its locals written where the
// original writes them, its saved registers not; the frames below it, which
// only this code read, are values since E5 step 6). Built with
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

// The frames (bytes; only DrawDecal's is made since E5 step 6, the others
// are the originals' sizes).
constexpr uint32_t kDrawDecalFrameBytes = 400;
constexpr uint32_t kDrawTriangleFrameBytes = 496;
constexpr uint32_t kDrawSpanFrameBytes = 272;
constexpr uint32_t kBlendPixelFrameBytes = 112;

// The sheet's regions and width, and the floats the rasterizer reads (0.5,
// 0.0 and the texel weights' scale): schema::CarModelsGlobals.

// sub_820ED2A0: the decal's texel at (u, v), bilinear: the four pixels
// around it (clamped to the image), weighted by the fractions times the
// scale (as integers out of 256) with the packed blend of BlendPixel.
// Returns the ARGB word. (The original wrote its conversions' words at its
// caller's r1 - 80, read by nothing: no longer stored since E5 step 6.)
uint32_t SampleTexel(uint32_t image, double u, double v);

// sub_820ED498 SoftRaster::BlendPixel: the canvas pixel at (x, y) blended
// with the texel at (u, v) by its alpha (skipped for 0). The original read
// u and v as floats from its caller's frame (a value since E5 step 6).
void BlendPixel(uint32_t image, uint32_t canvas, uint32_t region, int32_t x,
                int32_t y, double u, double v);

// A span's end's texel (single-rounded values), which the original handed
// over as a vertex in its caller's frame.
struct SpanEnd {
  double u = 0.0, v = 0.0;
};

// sub_820EDE00 SoftRaster::DrawSpan: row `y` from xa to xb (either order;
// the right end left out), its texels from `a` and `b` (each end's), less
// 0.5 when `flags` (the original's word at its caller's frame + 84) has its
// top bit; clipped to the region, each pixel blended.
void DrawSpan(uint32_t canvas, uint32_t region, int32_t xa, int32_t xb,
              int32_t y, const SpanEnd& a, const SpanEnd& b, uint32_t flags, uint32_t image);

// sub_820EE0C8 SoftRaster::DrawTriangle: the three vertices at `positions`
// (16 bytes apart) with their texels at `texels`, sorted by y and filled row
// by row from the rows' centers (spans of the top half, then the bottom),
// `flags` (its low 29 bits, with the top bit set) handed to each span.
// `stack`, the guest r1 at entry, is unused since E5 step 6 (no frame).
void DrawTriangle(uint32_t canvas, uint32_t region, uint32_t positions,
                  uint32_t texels, uint32_t image, uint32_t flags, uint32_t stack);

// sub_820ED1E8: a decal's corner: (p - c) scaled by `scale`, rotated by
// `angle` (sin and cos: c_runtime), plus c, plus the offset `t`, into `out`
// (which may be `p`).
void DecalCorner(uint32_t out, uint32_t p, uint32_t c, uint32_t scale,
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
// region's rectangle (CarModelsGlobals::vinyl_regions), its corners through
// DecalCorner, its texels mirrored left to right when asked, drawn as two
// triangles. `stack` is the guest r1 at entry. Returns the original's r3
// (`stack` + 32); nullopt, with nothing done, for a region not
// DecalRegionPorted.
std::optional<uint32_t> DrawDecal(const DecalArguments& arguments,
                                  uint32_t stack);

}  // namespace car_models
