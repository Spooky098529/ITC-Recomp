// The decals' software rasterizer (see raster.h), step for step as the
// translated code (the operands in its order; DrawDecal's frame words where
// it writes them, the frames below it values since E5 step 6).

#include "car_models/raster.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"
#include "schema/car_models.h"
#include "texture_compression/steps.h"

#include <simde/x86/sse2.h>

#include <climits>
#include <cmath>

namespace car_models {

namespace {

using namespace texture_compression::steps;
using schema::DecalVertex;
using G = schema::CarModelsGlobals;

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
void StoreSingle(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}
// std: a doubleword, high word first.
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
uint64_t Load64(uint32_t address) {
  return uint64_t(Load(address)) << 32 | Load(address + 4);
}
// A word sign-extended to a doubleword (extsw, then std).
void StoreExtended(uint32_t address, int32_t value) {
  Store64(address, uint64_t(int64_t(value)));
}
// 16 bytes copied as the original's two ld and two std (both loaded first).
void Copy16(uint32_t to, uint32_t from) {
  const uint64_t a = Load64(from), b = Load64(from + 8);
  Store64(to, a);
  Store64(to + 8, b);
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

uint32_t SampleTexel(uint32_t image, double u, double v) {
  ITC_NATIVE_RUNNING("sub_820ED2A0 SoftRaster::SampleTexel");
  // (The conversions' words the original wrote at its caller's r1 - 80, read
  // by nothing, are no longer stored: E5 step 6.)
  const int32_t ix = Fctiwz(u);
  const int32_t iy = Fctiwz(v);
  const double fu = Fsubs(u, IntToSingle(ix));
  const double fv = Fsubs(v, IntToSingle(iy));
  const schema::DecalImage img(image);
  int32_t x0 = ix, x1 = int32_t(uint32_t(ix) + 1);
  if (ix < 0) {
    x0 = x1 = 0;
  } else if (const int32_t width = Signed(img.width()); !(width > x1)) {
    x0 = x1 = int32_t(uint32_t(width) - 1);
  }
  int32_t y0 = iy, y1 = int32_t(uint32_t(iy) + 1);
  if (iy < 0) {
    y0 = y1 = 0;
  } else if (const int32_t height = Signed(img.height()); !(height > y1)) {
    y0 = y1 = int32_t(uint32_t(height) - 1);
  }
  const uint32_t pitch = img.pitch();
  const uint32_t pixels = img.pixels();
  const double scale = double(G::texel_scale());
  const uint32_t tx = FctidzLow(Fmuls(fu, scale));
  const uint32_t ty = FctidzLow(Fmuls(fv, scale));
  const uint32_t row0 = pitch * uint32_t(y0) + pixels;
  const uint32_t row1 = pitch * uint32_t(y1) + pixels;
  const uint32_t a = Load((uint32_t(x0) << 2) + row0);
  const uint32_t c = Load((uint32_t(x0) << 2) + row1);
  const uint32_t b = Load((uint32_t(x1) << 2) + row0);
  const uint32_t d = Load((uint32_t(x1) << 2) + row1);
  return Mix(Mix(a, b, tx), Mix(c, d, tx), ty);
}

void BlendPixel(uint32_t image, uint32_t canvas, uint32_t region, int32_t x,
                int32_t y, double u, double v) {
  ITC_NATIVE_RUNNING("sub_820ED498 SoftRaster::BlendPixel");
  const uint32_t address =
      (uint32_t(x) << 2) + (schema::DecalRegion(region).pitch() * uint32_t(y) +
                            schema::DecalCanvas(canvas).first_pixel());
  const uint32_t destination = Load(address);
  const uint32_t source = SampleTexel(image, u, v);
  const uint32_t alpha = source >> 24;
  if (alpha == 0) {
    return;
  }
  Store(address, Mix(destination, source, alpha));
}

void DrawSpan(uint32_t canvas, uint32_t region, int32_t xa, int32_t xb,
              int32_t y, const SpanEnd& a, const SpanEnd& b, uint32_t flags, uint32_t image) {
  ITC_NATIVE_RUNNING("sub_820EDE00 SoftRaster::DrawSpan");
  const schema::DecalRegion r(region);
  if (y < 0 || !(Signed(r.height()) > y) || xa == xb) {
    return;
  }
  // The ends by x (the original copied them to its frame's +112 and +128:
  // each float there is the value, read back unchanged).
  int32_t left, right;
  SpanEnd l, rt;
  if (xa < xb) {
    right = xb;
    left = xa;
    l = a;
    rt = b;
  } else {
    left = xb;
    right = xa;
    l = b;
    rt = a;
  }
  double lu, lv, ru, rv;  // f30, f31, f13, f0
  if (flags & 0x80000000u) {
    const double half = double(G::float_half());
    lu = Fsubs(l.u, half);
    lv = Fsubs(l.v, half);
    ru = Fsubs(rt.u, half);
    rv = Fsubs(rt.v, half);
  } else {
    lv = l.v;
    lu = l.u;
    rv = rt.v;
    ru = rt.u;
  }
  if (left < 0) {
    const int32_t skipped = int32_t(0u - uint32_t(left));
    const double du = Fsubs(ru, lu);
    const int32_t width = int32_t(uint32_t(right) - uint32_t(left));
    const double dv = Fsubs(rv, lv);
    const double length = IntToSingle(width);
    double t = IntToSingle(skipped);
    t = Fdivs(t, length);
    const double su = Fmuls(du, t);
    const double sv = Fmuls(dv, t);
    const double nu = Fadds(su, lu);
    lv = Fadds(sv, lv);
    lu = nu;
    left = 0;
  }
  if (const int32_t width = Signed(r.width()); !(width > right)) {
    const int32_t length = int32_t(uint32_t(right) - uint32_t(left));
    const double du = Fsubs(lu, ru);
    const int32_t over = int32_t(uint32_t(right) - uint32_t(width));
    const double dv = Fsubs(lv, rv);
    right = width;
    double t = IntToSingle(over);
    const double len = IntToSingle(length);
    t = Fdivs(t, len);
    const double su = Fmuls(du, t);
    const double sv = Fmuls(dv, t);
    ru = Fadds(su, ru);
    rv = Fadds(sv, rv);
  }
  if (!(left < right)) {
    return;
  }
  const double dv = Fsubs(rv, lv);  // f28
  const double du = Fsubs(ru, lu);  // f29
  const double length = IntToSingle(int32_t(uint32_t(right) - uint32_t(left)));  // f27
  int32_t i = 0;
  for (int32_t x = left; x < right; x++, i++) {
    double t = IntToSingle(i);
    t = Fdivs(t, length);
    const double su = Fmuls(du, t);
    const double sv = Fmuls(dv, t);
    BlendPixel(image, canvas, region, x, y, Fadds(su, lu), Fadds(sv, lv));
  }
}

namespace {

struct Edge {
  double x0, dx;
  double u0, du;
  double v0, dv;
};

// One half of a triangle: its rows from `first` to `end` (left out), each
// row's ends on edges A and B (x at the row's i / rows and at (i + 0.5) /
// rows, texels at the latter), the ends corrected to the pixels' centers,
// and a span. (The original's frame slots for them, which only it and the
// span read, are values since E5 step 6.)
void DrawHalf(uint32_t canvas, uint32_t region, uint32_t image, uint32_t flags,
              int32_t first, int32_t end, double rows, const Edge& ea, const Edge& eb,
              bool lower) {
  const double half = double(G::float_half());
  int32_t i = 0;
  for (int32_t y = first; y < end; y++, i++) {
    double t = IntToSingle(i);
    const double t0 = Fdivs(t, rows);
    t = Fadds(t, half);
    const double xa0 = Fmadds(ea.dx, t0, ea.x0);
    const double xb0 = Fmadds(eb.dx, t0, eb.x0);
    t = Fdivs(t, rows);
    const int32_t ia = Fctiwz(xa0);
    const int32_t ib = Fctiwz(xb0);
    const double xa = Fmadds(ea.dx, t, ea.x0);
    double ua = Fmuls(ea.du, t);
    double va = Fmuls(ea.dv, t);
    double ub = Fmuls(eb.du, t);
    double vb = Fmuls(eb.dv, t);
    const double xb = Fmadds(eb.dx, t, eb.x0);
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
    const SpanEnd a{Fadds(Fmuls(dua, sa), ua), Fadds(Fmuls(dva, sa), va)};
    const SpanEnd b{Fadds(Fmuls(dub, sb), ub), Fadds(Fmuls(dvb, sb), vb)};
    DrawSpan(canvas, region, ia, ib, y, a, b, flags, image);
  }
}

}  // namespace

void DrawTriangle(uint32_t canvas, uint32_t region, uint32_t positions,
                  uint32_t texels, uint32_t image, uint32_t flags, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_820EE0C8 SoftRaster::DrawTriangle");
  (void)stack;  // no guest frame since E5 step 6
  auto y_of = [&](uint32_t i) { return double(DecalVertex(positions + 16 * i).y()); };
  uint32_t top = 0, mid = 1, bottom = 2;
  if (y_of(0) > y_of(1)) {
    top = 1;
    mid = 0;
  }
  if (y_of(top) > double(schema::DecalTriangle(positions).y2())) {
    bottom = top;
    top = 2;
  }
  if (y_of(mid) > y_of(bottom)) {
    std::swap(mid, bottom);
  }
  // The vertices the original copied into its frame (+128 the top's
  // position, +144 the bottom's, +160 the middle's, +176 the middle's
  // texel, +96 the top's, +192 the bottom's) and read back as floats: read
  // once here, as the copies were taken (E5 step 6).
  const DecalVertex top_p(positions + 16 * top), bottom_p(positions + 16 * bottom);
  const DecalVertex mid_p(positions + 16 * mid);
  const DecalVertex top_t(texels + 16 * top), mid_t(texels + 16 * mid);
  const DecalVertex bottom_t(texels + 16 * bottom);
  const double top_x = double(top_p.x()), top_y = double(top_p.y());
  const double bottom_x = double(bottom_p.x()), bottom_y = double(bottom_p.y());
  const double mid_x = double(mid_p.x()), mid_y = double(mid_p.y());
  const double u_mid = double(mid_t.x()), v_mid = double(mid_t.y());  // f15, f16
  const double u_top = double(top_t.x()), v_top = double(top_t.y());
  const double u_bottom = double(bottom_t.x()), v_bottom = double(bottom_t.y());
  const double half = double(G::float_half());
  const int32_t y_top = Fctiwz(Fadds(top_y, half));
  const int32_t y_mid = Fctiwz(Fadds(mid_y, half));
  const int32_t y_bottom = Fctiwz(Fadds(bottom_y, half));
  if (y_top == y_bottom) {
    return;
  }
  const double x_top = top_x;     // f31
  const double x_bottom = bottom_x;  // f14
  const double dx = Fsubs(x_bottom, x_top);
  const double x_mid = mid_x;  // f23
  const uint32_t span_flags = (flags & 0x1FFFFFFF) | 0x80000000u;
  const double upper_rows = IntToSingle(int32_t(uint32_t(y_mid) - uint32_t(y_top)));  // f27
  double du = u_bottom;
  const double all_rows = IntToSingle(int32_t(uint32_t(y_bottom) - uint32_t(y_top)));
  double dv = v_bottom;
  du = Fsubs(du, u_top);
  dv = Fsubs(dv, v_top);
  const double t = Fdivs(upper_rows, all_rows);
  const double x_split = Fadds(Fmuls(dx, t), x_top);  // f26
  const double u_split = Fadds(Fmuls(du, t), u_top);  // f21
  const double v_split = Fadds(Fmuls(dv, t), v_top);  // f20
  if (y_top < y_mid) {
    const double u0 = u_top;  // f29
    const double v0 = v_top;  // f28
    const Edge a{x_top, Fsubs(x_mid, x_top), u0, Fsubs(u_mid, u0), v0, Fsubs(v_mid, v0)};
    const Edge b{x_top, Fsubs(x_split, x_top), u0, Fsubs(u_split, u0), v0, Fsubs(v_split, v0)};
    DrawHalf(canvas, region, image, span_flags, y_top, y_mid, upper_rows, a, b, false);
  }
  if (!(y_mid < y_bottom)) {
    return;
  }
  const Edge a{x_mid, Fsubs(x_bottom, x_mid), u_mid, Fsubs(u_bottom, u_mid), v_mid,
               Fsubs(v_bottom, v_mid)};
  const Edge b{x_split, Fsubs(x_bottom, x_split), u_split, Fsubs(u_bottom, u_split), v_split,
               Fsubs(v_bottom, v_split)};
  const double lower_rows = IntToSingle(int32_t(uint32_t(y_bottom) - uint32_t(y_mid)));
  DrawHalf(canvas, region, image, span_flags, y_mid, y_bottom, lower_rows, a, b, true);
}

void DecalCorner(uint32_t out, uint32_t p, uint32_t c, uint32_t scale,
                 uint32_t t, double angle) {
  ITC_NATIVE_RUNNING("sub_820ED1E8 Vinyl::DecalCorner");
  const DecalVertex vp(p), vc(c), vs(scale), vt(t), vo(out);
  const double px = double(vp.x());
  const double py = double(vp.y());
  const double sy = double(vs.y());
  const double sx_ = Fmuls(Fsubs(px, double(vc.x())), double(vs.x()));  // f30
  const double sy_ = Fmuls(Fsubs(py, double(vc.y())), sy);               // f29
  const double sine = double(float(c_runtime::Sin(angle)));
  const double cosine = double(float(c_runtime::Cos(angle)));
  const double cx = double(vc.x());
  const double a = Fmuls(sine, sy_);
  const double tx = double(vt.x());
  const double b = Fmuls(sine, sx_);
  const double rx = Fmadds(cosine, sx_, -a);
  const double ry = Fmadds(cosine, sy_, b);
  vo.set_x(float(Fadds(Fadds(rx, cx), tx)));
  const double y = Fadds(ry, double(vt.y()));
  vo.set_y(float(Fadds(y, double(vc.y()))));
}

bool DecalRegionPorted(uint32_t region) {
  return region <= 9;
}

std::optional<uint32_t> DrawDecal(const DecalArguments& arguments,
                                  uint32_t stack) {
  if (!DecalRegionPorted(arguments.region)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820EE638 Vinyl::DrawDecal");
  const uint32_t f = stack - kDrawDecalFrameBytes;
  // The registers spilled into the caller's frame.
  Store64(stack + 32, arguments.header[0]);
  Store64(stack + 40, arguments.header[1]);
  Store64(stack + 48, arguments.header[2]);
  Store64(stack + 64, arguments.record[0]);
  Store64(stack + 72, arguments.record[1]);
  Copy16(f + 80, G::vinyl_regions_address() + 16 * arguments.region);
  const uint32_t width = guest::LoadU16(stack + 46);   // r28
  const uint32_t height = guest::LoadU16(stack + 48);  // r27
  StoreSingle(f + 112, Lfs(stack + 96));
  const uint32_t left = Load(f + 80);  // r24
  const uint32_t top = Load(f + 84);   // r23
  StoreSingle(f + 116, Lfs(stack + 100));
  const double half = double(G::float_half());
  const uint32_t region_width = Load(f + 88) - left;  // r26
  const double offset_x = Lfs(stack + 64);
  StoreExtended(f + 96, Signed(width));
  const uint32_t region_height = Load(f + 92) - top;  // r25
  const double offset_y = Lfs(stack + 68);
  const double zero = double(G::float_zero());  // f31
  StoreSingle(f + 208, zero);
  StoreSingle(f + 212, zero);
  StoreSingle(f + 228, zero);
  StoreSingle(f + 240, zero);
  StoreExtended(f + 80, Signed(height));
  const double w = IntToSingle(Signed(width));  // f30
  StoreExtended(f + 96, Signed(region_height));
  StoreSingle(f + 224, w);
  StoreSingle(f + 256, w);
  StoreExtended(f + 80, Signed(region_width));
  const double h = IntToSingle(Signed(height));  // f29
  StoreSingle(f + 244, h);
  const double cx = Fmuls(w, half);
  StoreSingle(f + 96, cx);
  StoreSingle(f + 260, h);
  const double rw = IntToSingle(Signed(region_width));
  const double cy = Fmuls(h, half);
  StoreSingle(f + 100, cy);
  const double rh = IntToSingle(Signed(region_height));
  const double ox = Fsubs(offset_x, cx);
  const double oy = Fsubs(offset_y, cy);
  StoreSingle(f + 80, Fmadds(rw, half, ox));
  StoreSingle(f + 84, Fmadds(rh, half, oy));
  for (uint32_t corner : {208u, 224u, 240u, 256u}) {
    DecalCorner(f + corner, f + corner, f + 96, f + 112, f + 80, arguments.angle);
  }
  StoreSingle(f + 144, zero);
  StoreSingle(f + 148, zero);
  StoreSingle(f + 160, w);
  StoreSingle(f + 164, zero);
  StoreSingle(f + 176, zero);
  StoreSingle(f + 180, h);
  StoreSingle(f + 192, w);
  StoreSingle(f + 196, h);
  if (arguments.mirrored != 0) {
    Copy16(f + 112, f + 144);
    Copy16(f + 144, f + 160);
    Copy16(f + 160, f + 112);
    Copy16(f + 112, f + 176);
    Copy16(f + 176, f + 192);
    Copy16(f + 192, f + 112);
  }
  const uint32_t pitch = G::vinyl_sheet_width() << 2;
  Store(f + 80, region_width);
  Store(f + 84, region_height);
  Store(f + 116, width);
  Store(f + 120, height);
  Store(f + 124, width << 2);
  Store(f + 100, 0);
  Store(f + 88, pitch);
  Store(f + 96, pitch * top + (left << 2) + Load(stack + 132));
  Store(f + 112, Load(stack + 116));
  for (uint32_t i = 0; i < 32; i += 16) {
    DrawTriangle(f + 96, f + 80, f + 208 + i, f + 144 + i, f + 112, 0, f);
  }
  // The header's empty destructor (sub_82106540) on stack + 32 leaves r3.
  return stack + 32;
}

}  // namespace car_models
