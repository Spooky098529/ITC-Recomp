// The decals' software rasterizer (see raster.h), step for step as the
// translated code (the operands in its order, its frames' words where it
// writes them).

#include "car_models/raster.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "texture_compression/steps.h"

#include <simde/x86/sse2.h>

#include <climits>
#include <cmath>

namespace car_models {

namespace {

using namespace texture_compression::steps;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void StoreSingle(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// std: a doubleword, high word first.
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return uint64_t(Load(base, address)) << 32 | Load(base, address + 4);
}
// A word sign-extended to a doubleword (extsw, then std).
void StoreExtended(uint8_t* base, uint32_t address, int32_t value) {
  Store64(base, address, uint64_t(int64_t(value)));
}
// 16 bytes copied as the original's two ld and two std (both loaded first).
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  const uint64_t a = Load64(base, from), b = Load64(base, from + 8);
  Store64(base, to, a);
  Store64(base, to + 8, b);
}

// fctidz's low word, as the translation does it: NaN to the low word of
// INT64_MIN (0), above INT64_MAX to INT64_MAX's, else truncated (cvttsd: the
// low word of INT64_MIN below the range).
uint32_t FctidzLow(double value) {
  if (std::isnan(value)) {
    return 0;
  }
  if (value > double(LLONG_MAX)) {
    return uint32_t(LLONG_MAX);
  }
  return uint32_t(simde_mm_cvttsd_si64(simde_mm_set_sd(value)));
}

// The packed blend of two ARGB words by `t` out of 256 (as wrapping words,
// as the original's mullw): alpha and green as (c0 x (256 - t) + c1 x t)
// >> 8 together, red and blue each product shifted first.
uint32_t Mix(uint32_t c0, uint32_t c1, uint32_t t) {
  const uint32_t rest = 256 - t;
  const uint32_t ag = ((c0 >> 8) & 0xFF00FF) * rest + ((c1 >> 8) & 0xFF00FF) * t;
  const uint32_t rb =
      (((c0 & 0xFF00FF) * rest) >> 8 & 0xFFFFFF) + (((c1 & 0xFF00FF) * t) >> 8 & 0xFFFFFF);
  return (ag & 0xFF00FF00u) | (rb & 0x00FF00FFu);
}

int32_t Signed(uint32_t value) {
  return int32_t(value);
}

}  // namespace

uint32_t SampleTexel(uint8_t* base, uint32_t image, double u, double v, uint32_t scratch) {
  ITC_NATIVE_RUNNING("sub_820ED2A0 SoftRaster::SampleTexel");
  const int32_t ix = Fctiwz(u);
  const int32_t iy = Fctiwz(v);
  Store(base, scratch, uint32_t(ix));
  Store(base, scratch, uint32_t(iy));
  StoreExtended(base, scratch, ix);
  StoreExtended(base, scratch + 8, iy);
  const double fu = Fsubs(u, IntToSingle(ix));
  const double fv = Fsubs(v, IntToSingle(iy));
  int32_t x0 = ix, x1 = int32_t(uint32_t(ix) + 1);
  if (ix < 0) {
    x0 = x1 = 0;
  } else if (const int32_t width = Signed(Load(base, image + 4)); !(width > x1)) {
    x0 = x1 = int32_t(uint32_t(width) - 1);
  }
  int32_t y0 = iy, y1 = int32_t(uint32_t(iy) + 1);
  if (iy < 0) {
    y0 = y1 = 0;
  } else if (const int32_t height = Signed(Load(base, image + 8)); !(height > y1)) {
    y0 = y1 = int32_t(uint32_t(height) - 1);
  }
  const uint32_t pitch = Load(base, image + 12);
  const uint32_t pixels = Load(base, image + 0);
  const double scale = Lfs(base, kTexelScale);
  const uint32_t tx = FctidzLow(Fmuls(fu, scale));
  const uint32_t ty = FctidzLow(Fmuls(fv, scale));
  const uint32_t row0 = pitch * uint32_t(y0) + pixels;
  const uint32_t row1 = pitch * uint32_t(y1) + pixels;
  const uint32_t a = Load(base, (uint32_t(x0) << 2) + row0);
  const uint32_t c = Load(base, (uint32_t(x0) << 2) + row1);
  const uint32_t b = Load(base, (uint32_t(x1) << 2) + row0);
  const uint32_t d = Load(base, (uint32_t(x1) << 2) + row1);
  Store(base, scratch, tx);
  Store(base, scratch, ty);
  return Mix(Mix(a, b, tx), Mix(c, d, tx), ty);
}

void BlendPixel(uint8_t* base, uint32_t image, uint32_t canvas, uint32_t region, int32_t x,
                int32_t y, uint32_t uv, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820ED498 SoftRaster::BlendPixel");
  const uint32_t frame = stack - kBlendPixelFrameBytes;
  const uint32_t address =
      (uint32_t(x) << 2) + (Load(base, region + 8) * uint32_t(y) + Load(base, canvas));
  const uint32_t destination = Load(base, address);
  const double v = Lfs(base, uv + 4);
  const double u = Lfs(base, uv + 0);
  const uint32_t source = SampleTexel(base, image, u, v, frame - 80);
  const uint32_t alpha = source >> 24;
  if (alpha == 0) {
    return;
  }
  Store(base, address, Mix(destination, source, alpha));
}

void DrawSpan(uint8_t* base, uint32_t canvas, uint32_t region, int32_t xa, int32_t xb,
              int32_t y, uint32_t a, uint32_t b, uint32_t image, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EDE00 SoftRaster::DrawSpan");
  const uint32_t f = stack - kDrawSpanFrameBytes;
  if (y < 0 || !(Signed(Load(base, region + 4)) > y) || xa == xb) {
    return;
  }
  int32_t left, right;
  if (xa < xb) {
    right = xb;
    left = xa;
    const uint64_t a0 = Load64(base, a), b0 = Load64(base, b);
    const uint64_t a8 = Load64(base, a + 8), b8 = Load64(base, b + 8);
    Store64(base, f + 112, a0);
    Store64(base, f + 128, b0);
    Store64(base, f + 120, a8);
    Store64(base, f + 136, b8);
  } else {
    left = xb;
    right = xa;
    const uint64_t b0 = Load64(base, b), a0 = Load64(base, a);
    const uint64_t b8 = Load64(base, b + 8), a8 = Load64(base, a + 8);
    Store64(base, f + 112, b0);
    Store64(base, f + 128, a0);
    Store64(base, f + 120, b8);
    Store64(base, f + 136, a8);
  }
  const uint32_t flags = Load(base, stack + 84);
  double lu, lv, ru, rv;  // f30, f31, f13, f0
  if (flags & 0x80000000u) {
    const double l_u = Lfs(base, f + 112);
    const double r_v = Lfs(base, f + 132);
    const double half = Lfs(base, kRasterHalf);
    lu = Fsubs(l_u, half);
    lv = Fsubs(Lfs(base, f + 116), half);
    const double r_u = Fsubs(Lfs(base, f + 128), half);
    StoreSingle(base, f + 112, lu);
    rv = Fsubs(r_v, half);
    StoreSingle(base, f + 116, lv);
    StoreSingle(base, f + 128, r_u);
    StoreSingle(base, f + 132, rv);
    ru = r_u;
  } else {
    lv = Lfs(base, f + 116);
    lu = Lfs(base, f + 112);
    rv = Lfs(base, f + 132);
    ru = Lfs(base, f + 128);
  }
  if (left < 0) {
    const int32_t skipped = int32_t(0u - uint32_t(left));
    const double du = Fsubs(ru, lu);
    const int32_t width = int32_t(uint32_t(right) - uint32_t(left));
    const double dv = Fsubs(rv, lv);
    StoreExtended(base, f + 80, skipped);
    StoreExtended(base, f + 96, width);
    const double length = IntToSingle(width);
    double t = IntToSingle(skipped);
    t = Fdivs(t, length);
    const double su = Fmuls(du, t);
    const double sv = Fmuls(dv, t);
    StoreSingle(base, f + 96, Fadds(su, lu));
    StoreSingle(base, f + 100, Fadds(sv, lv));
    Copy16(base, f + 112, f + 96);
    lv = Lfs(base, f + 116);
    lu = Lfs(base, f + 112);
    left = 0;
  }
  if (const int32_t width = Signed(Load(base, region + 0)); !(width > right)) {
    const int32_t length = int32_t(uint32_t(right) - uint32_t(left));
    const double du = Fsubs(lu, ru);
    const int32_t over = int32_t(uint32_t(right) - uint32_t(width));
    const double dv = Fsubs(lv, rv);
    right = width;
    StoreExtended(base, f + 80, length);
    StoreExtended(base, f + 96, over);
    double t = IntToSingle(over);
    const double l = IntToSingle(length);
    t = Fdivs(t, l);
    const double su = Fmuls(du, t);
    const double sv = Fmuls(dv, t);
    ru = Fadds(su, ru);
    StoreSingle(base, f + 112, ru);
    rv = Fadds(sv, rv);
    StoreSingle(base, f + 116, rv);
    Copy16(base, f + 128, f + 112);
    rv = Lfs(base, f + 132);
    ru = Lfs(base, f + 128);
  }
  if (!(left < right)) {
    return;
  }
  const double dv = Fsubs(rv, lv);  // f28
  const double du = Fsubs(ru, lu);  // f29
  StoreExtended(base, f + 96, int32_t(uint32_t(right) - uint32_t(left)));
  const double length = IntToSingle(int32_t(uint32_t(right) - uint32_t(left)));  // f27
  int32_t i = 0;
  for (int32_t x = left; x < right; x++, i++) {
    StoreExtended(base, f + 96, i);
    double t = IntToSingle(i);
    t = Fdivs(t, length);
    const double su = Fmuls(du, t);
    const double sv = Fmuls(dv, t);
    StoreSingle(base, f + 128, Fadds(su, lu));
    StoreSingle(base, f + 132, Fadds(sv, lv));
    Copy16(base, f + 112, f + 128);
    BlendPixel(base, image, canvas, region, x, y, f + 112, f);
  }
}

namespace {

// One half of a triangle: its rows from `first` to `end` (left out), each
// row's ends on edges A and B (x at the row's i / rows and at (i + 0.5) /
// rows, texels at the latter), the ends corrected to the pixels' centers,
// and a span. The frame's slots as each half uses them.
struct HalfSlots {
  uint32_t i;         // the row's number (a doubleword)
  uint32_t a, b;      // the ends' integers (words)
  uint32_t a64, b64;  // the same as doublewords
  uint32_t a_texel, b_texel;
  uint32_t a_copy, b_copy;  // what the span gets (its r8 and r9)
};

struct Edge {
  double x0, dx;
  double u0, du;
  double v0, dv;
};

void DrawHalf(uint8_t* base, uint32_t canvas, uint32_t region, uint32_t image, uint32_t f,
              uint32_t flags, int32_t first, int32_t end, double rows, const Edge& ea,
              const Edge& eb, const HalfSlots& s, bool lower) {
  const double half = Lfs(base, kRasterHalf);
  int32_t i = 0;
  for (int32_t y = first; y < end; y++, i++) {
    Store(base, f + 84, flags);
    StoreExtended(base, s.i, i);
    double t = IntToSingle(i);
    const double t0 = Fdivs(t, rows);
    t = Fadds(t, half);
    const double xa0 = Fmadds(ea.dx, t0, ea.x0);
    const double xb0 = Fmadds(eb.dx, t0, eb.x0);
    t = Fdivs(t, rows);
    const int32_t ia = Fctiwz(xa0);
    Store(base, s.a, uint32_t(ia));
    const int32_t ib = Fctiwz(xb0);
    Store(base, s.b, uint32_t(ib));
    const double xa = Fmadds(ea.dx, t, ea.x0);
    double ua = Fmuls(ea.du, t);
    double va = Fmuls(ea.dv, t);
    double ub = Fmuls(eb.du, t);
    double vb = Fmuls(eb.dv, t);
    const double xb = Fmadds(eb.dx, t, eb.x0);
    StoreExtended(base, s.a64, ia);
    StoreExtended(base, s.b64, ib);
    if (lower) {
      ua = Fadds(ea.u0, ua);
      va = Fadds(ea.v0, va);
      ub = Fadds(ub, eb.u0);
      vb = Fadds(vb, eb.v0);
    } else {
      ua = Fadds(ea.u0, ua);
      va = Fadds(ea.v0, va);
      ub = Fadds(eb.u0, ub);
      vb = Fadds(eb.v0, vb);
    }
    const double ab = Fsubs(xb, xa);
    const double ba = Fsubs(xa, xb);
    const double pa = IntToSingle(ia);
    const double pb = IntToSingle(ib);
    const double dua = Fsubs(ub, ua);
    const double dva = Fsubs(vb, va);
    const double dub = Fsubs(ua, ub);
    const double dvb = Fsubs(va, vb);
    double ca = Fsubs(pa, xa);
    double cb = Fsubs(pb, xb);
    ca = Fadds(ca, half);
    cb = Fadds(cb, half);
    const double sa = Fdivs(ca, ab);
    const double sb = Fdivs(cb, ba);
    StoreSingle(base, s.a_texel, Fadds(Fmuls(dua, sa), ua));
    StoreSingle(base, s.a_texel + 4, Fadds(Fmuls(dva, sa), va));
    StoreSingle(base, s.b_texel, Fadds(Fmuls(dub, sb), ub));
    StoreSingle(base, s.b_texel + 4, Fadds(Fmuls(dvb, sb), vb));
    // The copies: B's texel first in the upper half, A's in the lower (as
    // the original's loads and stores; nothing overlaps).
    Copy16(base, s.b_copy, s.b_texel);
    Copy16(base, s.a_copy, s.a_texel);
    DrawSpan(base, canvas, region, ia, ib, y, s.a_copy, s.b_copy, image, f);
  }
}

}  // namespace

void DrawTriangle(uint8_t* base, uint32_t canvas, uint32_t region, uint32_t positions,
                  uint32_t texels, uint32_t image, uint32_t flags, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EE0C8 SoftRaster::DrawTriangle");
  const uint32_t f = stack - kDrawTriangleFrameBytes;
  auto y_of = [&](uint32_t i) { return Lfs(base, positions + 16 * i + 4); };
  uint32_t top = 0, mid = 1, bottom = 2;
  if (y_of(0) > y_of(1)) {
    top = 1;
    mid = 0;
  }
  if (y_of(top) > Lfs(base, positions + 36)) {
    bottom = top;
    top = 2;
  }
  if (y_of(mid) > y_of(bottom)) {
    std::swap(mid, bottom);
  }
  const uint32_t top_texel = texels + 16 * top, mid_texel = texels + 16 * mid;
  const uint32_t bottom_texel = texels + 16 * bottom;
  Copy16(base, f + 128, positions + 16 * top);
  Copy16(base, f + 144, positions + 16 * bottom);
  Copy16(base, f + 160, positions + 16 * mid);
  Copy16(base, f + 176, mid_texel);
  const double half = Lfs(base, kRasterHalf);
  const int32_t y_top = Fctiwz(Fadds(Lfs(base, f + 132), half));
  Store(base, f + 116, uint32_t(y_top));
  const int32_t y_mid = Fctiwz(Fadds(Lfs(base, f + 164), half));
  Store(base, f + 124, uint32_t(y_mid));
  const int32_t y_bottom = Fctiwz(Fadds(Lfs(base, f + 148), half));
  Store(base, f + 100, uint32_t(y_bottom));
  Copy16(base, f + 96, top_texel);
  Copy16(base, f + 192, bottom_texel);
  if (y_top == y_bottom) {
    return;
  }
  const double x_top = Lfs(base, f + 128);     // f31
  const double x_bottom = Lfs(base, f + 144);  // f14
  const double dx = Fsubs(x_bottom, x_top);
  const double x_mid = Lfs(base, f + 160);  // f23
  const double v_mid = Lfs(base, f + 180);  // f16
  const uint32_t span_flags = (flags & 0x1FFFFFFF) | 0x80000000u;
  const uint64_t top0 = Load64(base, top_texel), bottom0 = Load64(base, bottom_texel);
  const uint64_t bottom8 = Load64(base, bottom_texel + 8), top8 = Load64(base, top_texel + 8);
  StoreExtended(base, f + 112, int32_t(uint32_t(y_bottom) - uint32_t(y_top)));
  StoreExtended(base, f + 120, int32_t(uint32_t(y_mid) - uint32_t(y_top)));
  const double u_mid = Lfs(base, f + 176);  // f15
  Store64(base, f + 128, bottom0);
  Store64(base, f + 144, top0);
  Store64(base, f + 136, bottom8);
  Store64(base, f + 152, top8);
  const double u_top = Lfs(base, f + 144);
  const double upper_rows = IntToSingle(int32_t(uint32_t(y_mid) - uint32_t(y_top)));  // f27
  double du = Lfs(base, f + 128);
  const double all_rows = IntToSingle(int32_t(uint32_t(y_bottom) - uint32_t(y_top)));
  const double v_top = Lfs(base, f + 148);
  double dv = Lfs(base, f + 132);
  du = Fsubs(du, u_top);
  dv = Fsubs(dv, v_top);
  const double t = Fdivs(upper_rows, all_rows);
  const double x_split = Fadds(Fmuls(dx, t), x_top);  // f26
  const double u_split = Fadds(Fmuls(du, t), u_top);  // f21
  const double v_split = Fadds(Fmuls(dv, t), v_top);  // f20
  if (y_top < y_mid) {
    const double u0 = Lfs(base, f + 96);   // f29
    const double v0 = Lfs(base, f + 100);  // f28
    const Edge a{x_top, Fsubs(x_mid, x_top), u0, Fsubs(u_mid, u0), v0, Fsubs(v_mid, v0)};
    const Edge b{x_top, Fsubs(x_split, x_top), u0, Fsubs(u_split, u0), v0, Fsubs(v_split, v0)};
    const HalfSlots slots{f + 120, f + 96,  f + 112, f + 128, f + 144,
                          f + 176, f + 160, f + 224, f + 208};
    DrawHalf(base, canvas, region, image, f, span_flags, y_top, y_mid, upper_rows, a, b, slots,
             false);
  }
  if (!(y_mid < y_bottom)) {
    return;
  }
  const double u_bottom = Lfs(base, f + 192);
  const double v_bottom = Lfs(base, f + 196);
  const Edge a{x_mid, Fsubs(x_bottom, x_mid), u_mid, Fsubs(u_bottom, u_mid), v_mid,
               Fsubs(v_bottom, v_mid)};
  const Edge b{x_split, Fsubs(x_bottom, x_split), u_split, Fsubs(u_bottom, u_split), v_split,
               Fsubs(v_bottom, v_split)};
  StoreExtended(base, f + 144, int32_t(uint32_t(y_bottom) - uint32_t(y_mid)));
  const double lower_rows = IntToSingle(int32_t(uint32_t(y_bottom) - uint32_t(y_mid)));
  const HalfSlots slots{f + 144, f + 112, f + 96,  f + 128, f + 120,
                        f + 192, f + 176, f + 208, f + 224};
  DrawHalf(base, canvas, region, image, f, span_flags, y_mid, y_bottom, lower_rows, a, b, slots,
           true);
}

void DecalCorner(uint8_t* base, uint32_t out, uint32_t p, uint32_t c, uint32_t scale,
                 uint32_t t, double angle) {
  ITC_NATIVE_RUNNING("sub_820ED1E8 Vinyl::DecalCorner");
  const double px = Lfs(base, p + 0);
  const double py = Lfs(base, p + 4);
  const double sy = Lfs(base, scale + 4);
  const double sx_ = Fmuls(Fsubs(px, Lfs(base, c + 0)), Lfs(base, scale + 0));  // f30
  const double sy_ = Fmuls(Fsubs(py, Lfs(base, c + 4)), sy);                    // f29
  const double sine = double(float(c_runtime::Sin(angle)));
  const double cosine = double(float(c_runtime::Cos(angle)));
  const double cx = Lfs(base, c + 0);
  const double a = Fmuls(sine, sy_);
  const double tx = Lfs(base, t + 0);
  const double b = Fmuls(sine, sx_);
  const double rx = Fmadds(cosine, sx_, -a);
  const double ry = Fmadds(cosine, sy_, b);
  StoreSingle(base, out + 0, Fadds(Fadds(rx, cx), tx));
  const double y = Fadds(ry, Lfs(base, t + 4));
  StoreSingle(base, out + 4, Fadds(y, Lfs(base, c + 4)));
}

bool DecalRegionPorted(uint32_t region) {
  return region <= 8;
}

std::optional<uint32_t> DrawDecal(uint8_t* base, const DecalArguments& arguments,
                                  uint32_t stack) {
  if (!DecalRegionPorted(arguments.region)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820EE638 Vinyl::DrawDecal");
  const uint32_t f = stack - kDrawDecalFrameBytes;
  // The registers spilled into the caller's frame.
  Store64(base, stack + 32, arguments.header[0]);
  Store64(base, stack + 40, arguments.header[1]);
  Store64(base, stack + 48, arguments.header[2]);
  Store64(base, stack + 64, arguments.record[0]);
  Store64(base, stack + 72, arguments.record[1]);
  Copy16(base, f + 80, kVinylRegions + 16 * arguments.region);
  const uint32_t width = guest::LoadU16(base, stack + 46);   // r28
  const uint32_t height = guest::LoadU16(base, stack + 48);  // r27
  StoreSingle(base, f + 112, Lfs(base, stack + 96));
  const uint32_t left = Load(base, f + 80);  // r24
  const uint32_t top = Load(base, f + 84);   // r23
  StoreSingle(base, f + 116, Lfs(base, stack + 100));
  const double half = Lfs(base, kRasterHalf);
  const uint32_t region_width = Load(base, f + 88) - left;  // r26
  const double offset_x = Lfs(base, stack + 64);
  StoreExtended(base, f + 96, Signed(width));
  const uint32_t region_height = Load(base, f + 92) - top;  // r25
  const double offset_y = Lfs(base, stack + 68);
  const double zero = Lfs(base, kRasterZero);  // f31
  StoreSingle(base, f + 208, zero);
  StoreSingle(base, f + 212, zero);
  StoreSingle(base, f + 228, zero);
  StoreSingle(base, f + 240, zero);
  StoreExtended(base, f + 80, Signed(height));
  const double w = IntToSingle(Signed(width));  // f30
  StoreExtended(base, f + 96, Signed(region_height));
  StoreSingle(base, f + 224, w);
  StoreSingle(base, f + 256, w);
  StoreExtended(base, f + 80, Signed(region_width));
  const double h = IntToSingle(Signed(height));  // f29
  StoreSingle(base, f + 244, h);
  const double cx = Fmuls(w, half);
  StoreSingle(base, f + 96, cx);
  StoreSingle(base, f + 260, h);
  const double rw = IntToSingle(Signed(region_width));
  const double cy = Fmuls(h, half);
  StoreSingle(base, f + 100, cy);
  const double rh = IntToSingle(Signed(region_height));
  const double ox = Fsubs(offset_x, cx);
  const double oy = Fsubs(offset_y, cy);
  StoreSingle(base, f + 80, Fmadds(rw, half, ox));
  StoreSingle(base, f + 84, Fmadds(rh, half, oy));
  for (uint32_t corner : {208u, 224u, 240u, 256u}) {
    DecalCorner(base, f + corner, f + corner, f + 96, f + 112, f + 80, arguments.angle);
  }
  StoreSingle(base, f + 144, zero);
  StoreSingle(base, f + 148, zero);
  StoreSingle(base, f + 160, w);
  StoreSingle(base, f + 164, zero);
  StoreSingle(base, f + 176, zero);
  StoreSingle(base, f + 180, h);
  StoreSingle(base, f + 192, w);
  StoreSingle(base, f + 196, h);
  if (arguments.mirrored != 0) {
    Copy16(base, f + 112, f + 144);
    Copy16(base, f + 144, f + 160);
    Copy16(base, f + 160, f + 112);
    Copy16(base, f + 112, f + 176);
    Copy16(base, f + 176, f + 192);
    Copy16(base, f + 192, f + 112);
  }
  const uint32_t pitch = Load(base, kVinylSheetWidth) << 2;
  Store(base, f + 80, region_width);
  Store(base, f + 84, region_height);
  Store(base, f + 116, width);
  Store(base, f + 120, height);
  Store(base, f + 124, width << 2);
  Store(base, f + 100, 0);
  Store(base, f + 88, pitch);
  Store(base, f + 96, pitch * top + (left << 2) + Load(base, stack + 132));
  Store(base, f + 112, Load(base, stack + 116));
  for (uint32_t i = 0; i < 32; i += 16) {
    DrawTriangle(base, f + 96, f + 80, f + 208 + i, f + 144 + i, f + 112, 0, f);
  }
  // The header's empty destructor (sub_82106540) on stack + 32 leaves r3.
  return stack + 32;
}

}  // namespace car_models
