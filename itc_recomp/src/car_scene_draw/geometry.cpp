// TextureProjection::Fit's geometry library (docs/functions/car-scene-draw.md):
// the frustum's planes and corners, a plane's crossing with a line and the
// box over a vector of points, step for step as the translated code (see
// car_scene_draw.h).

#include "car_scene_draw/car_scene_draw.h"

#include "car_scene_draw/ops.h"
#include "guest.h"
#include "schema/car_scene_draw.h"

#include "base/log.h"

#include <bit>
#include <cmath>

namespace car_scene_draw {

namespace {

using namespace ops;

using G = schema::CarSceneDrawGlobals;
using Point = schema::CarScenePoint;
using Plane = schema::CarScenePlane;

constexpr uint32_t kFrustumFrame = 336;
constexpr uint32_t kBoxFrame = 128;

}  // namespace

// --- The planes ---

uint32_t PlaneIntersect3(uint32_t out, uint32_t a, uint32_t b, uint32_t c,
                         uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_825976C8 Plane::Intersect3");
  const uint32_t s = stack;
  const Plane pa(a), pb(b), pc(c);
  const double one = double(G::one());
  // b's x, y, z and 1.0 at s - 64, c's at s - 48, then a's over b's, each
  // read back as a vector.
  Stfs(s - 64, double(pb.x()));
  Stfs(s - 60, double(pb.y()));
  Stfs(s - 56, double(pb.z()));
  Stfs(s - 52, one);
  Stfs(s - 48, double(pc.x()));
  Stfs(s - 44, double(pc.y()));
  const double cz = double(pc.z());
  const simde__m128 vb = Lvx(s - 64);
  Stfs(s - 40, cz);
  const simde__m128 v10 = Permute<0x78>(vb);
  Stfs(s - 36, one);
  const simde__m128 v11 = Permute<0x9C>(vb);
  Stfs(s - 64, double(pa.x()));
  Stfs(s - 60, double(pa.y()));
  const double az = double(pa.z());
  const simde__m128 vc = Lvx(s - 48);
  Stfs(s - 56, az);
  const simde__m128 v8 = Permute<0x9C>(vc);
  Stfs(s - 52, one);
  const simde__m128 v9 = Permute<0x78>(vc);
  const simde__m128 v7 = Lvx(s - 64);
  {
    // The three cross products (b x c, c x a, a x b, by their use) and the
    // triple product.
    FlushToZero flush;
    const simde__m128 v6 = Permute<0x78>(v7);
    const simde__m128 v5 = Permute<0x9C>(v7);
    simde__m128 v0 = simde_mm_mul_ps(v11, v9);
    simde__m128 v13 = simde_mm_mul_ps(v8, v6);
    simde__m128 v12 = simde_mm_mul_ps(v5, v10);
    v0 = Nmsub(v10, v8, v0);
    v13 = Nmsub(v9, v5, v13);
    v12 = Nmsub(v6, v11, v12);
    Stvx(s - 64, v13);
    Stvx(s - 32, v12);
    Stvx(s - 48, v0);
    v0 = simde_mm_dp_ps(v7, v0, 0xEF);
    Stvx(s - 16, v0);
  }
  double f0 = Lfs(s - 16);
  Stfs(s - 80, f0);
  const uint32_t exponent = Load(s - 80) & 0x7F800000u;
  if (exponent == 0 || exponent == 0x7F800000u) {
    return 0;
  }
  double f13 = double(pa.d());
  f0 = Fdivs(one, f0);
  double f9 = Fmuls(f13, Lfs(s - 48));
  double f8 = Fmuls(f13, Lfs(s - 44));
  f13 = Fmuls(f13, Lfs(s - 40));
  double f12 = double(pb.d());
  const double f7 = Fmuls(f12, Lfs(s - 64));
  const double f6 = Fmuls(f12, Lfs(s - 60));
  f12 = Fmuls(f12, Lfs(s - 56));
  double f11 = double(pc.d());
  const double f5 = Fmuls(f11, Lfs(s - 32));
  const double f4 = Fmuls(f11, Lfs(s - 28));
  f11 = Fmuls(f11, Lfs(s - 24));
  Stfs(s - 4, one);
  f9 = Fadds(f7, f9);
  f8 = Fadds(f6, f8);
  f13 = Fadds(f12, f13);
  f12 = Fadds(f9, f5);
  f9 = Fadds(f8, f4);
  f13 = Fadds(f13, f11);
  Stfs(s - 16, Fmuls(Fneg(f12), f0));
  Stfs(s - 12, Fmuls(Fneg(f9), f0));
  Stfs(s - 8, Fmuls(Fneg(f13), f0));
  Copy16(out, s - 16);
  Point(out).set_w(float(one));
  return 1;
}

uint32_t FrustumFromMatrix(uint32_t out, uint32_t matrix, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597878 Frustum::FromMatrix");
  const uint32_t f = stack - kFrustumFrame;
  Store(f, stack);  // the back chain, as stwu
  const schema::CarSceneFrustum frustum(out);
  // The matrix's row `row`, column `column` (CarSceneMatrix's m<row><column>).
  auto m = [&](int row, int column) {
    return Lfs(schema::CarSceneMatrix(matrix).m00_address() + 16 * row + 4 * column);
  };
  auto st = [&](uint32_t offset, double value) { Stfs(f + offset, value); };
  auto copy = [&](uint32_t to, uint32_t from) { Copy16(f + to, f + from); };
  const double one = double(G::one());
  for (uint32_t k = 0; k < 8; k++) {
    Point(frustum.corners_address() + 16 * k).set_w(float(one));  // the corners' w
  }
  // The six planes from the matrix's w column (3) with and against its x,
  // y and z columns, built through the frame as the original: each written
  // to a temporary, then copied to 128 + 16 k (k = 0 to 5: w - x, w + x,
  // w - y, w + y, w - z, w + z), the d's stored over the copies' 1.0.
  st(80, Fsubs(m(0, 3), m(0, 0)));
  st(224, Fadds(m(0, 0), m(0, 3)));
  st(84, Fsubs(m(1, 3), m(1, 0)));
  st(228, Fadds(m(1, 0), m(1, 3)));
  st(232, Fadds(m(2, 0), m(2, 3)));
  st(88, Fsubs(m(2, 3), m(2, 0)));
  st(92, one);
  st(96, Fsubs(m(0, 3), m(0, 1)));
  st(100, Fsubs(m(1, 3), m(1, 1)));
  st(236, one);
  st(104, Fsubs(m(2, 3), m(2, 1)));
  copy(128, 80);
  st(112, Fadds(m(0, 1), m(0, 3)));
  st(108, one);
  copy(160, 96);  // (the original holds it in registers until after 144)
  st(140, Fsubs(m(3, 3), m(3, 0)));
  st(116, Fadds(m(1, 1), m(1, 3)));
  copy(144, 224);
  st(120, Fadds(m(2, 1), m(2, 3)));
  st(156, Fadds(m(3, 0), m(3, 3)));
  st(124, one);
  copy(176, 112);
  st(172, Fsubs(m(3, 3), m(3, 1)));
  st(112, Fsubs(m(0, 3), m(0, 2)));
  st(116, Fsubs(m(1, 3), m(1, 2)));
  st(96, Fadds(m(0, 2), m(0, 3)));
  st(120, Fsubs(m(2, 3), m(2, 2)));
  st(124, one);
  copy(192, 112);
  st(100, Fadds(m(1, 2), m(1, 3)));
  st(104, Fadds(m(2, 2), m(2, 3)));
  st(108, one);
  copy(208, 96);
  st(188, Fadds(m(3, 1), m(3, 3)));
  st(204, Fsubs(m(3, 3), m(3, 2)));
  st(220, Fadds(m(3, 2), m(3, 3)));
  // Each plane scaled by 1 / the length of its x, y, z (stored y, x, z, d).
  for (uint32_t p = f + 128; p < f + 224; p += 16) {
    double y = Lfs(p + 4);
    double x = Lfs(p + 0);
    double length = Fmuls(x, x);
    double z = Lfs(p + 8);
    double d = Lfs(p + 12);
    length = Fmadds(z, z, length);
    length = Fmadds(y, y, length);
    length = Fsqrts(length);
    const double scale = Fdivs(one, length);
    Stfs(p + 4, Fmuls(y, scale));
    Stfs(p + 0, Fmuls(x, scale));
    Stfs(p + 8, Fmuls(z, scale));
    Stfs(p + 12, Fmuls(d, scale));
  }
  // The planes copied to out + 16 k through the frame's 80 (d first), their
  // x, y, z kept for the sign codes.
  double xyz[6][3];
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t p = f + 128 + 16 * k;
    const double d = Lfs(p + 12);
    for (int i = 0; i < 3; i++) {
      xyz[k][i] = Lfs(p + 4 * i);
    }
    st(92, d);
    for (int i = 0; i < 3; i++) {
      st(80 + 4 * i, xyz[k][i]);
    }
    for (uint32_t i = 0; i < 4; i++) {
      Store(frustum.planes_address() + 16 * k + 4 * i, Load(f + 80 + 4 * i));
    }
  }
  // +96 + 4 k: plane k's code, 1 for x < 0, 2 for y, 4 for z.
  const double zero = double(G::zero());
  for (uint32_t k = 0; k < 6; k++) {
    const uint32_t code = (xyz[k][0] < zero ? 1u : 0u) | (xyz[k][1] < zero ? 2u : 0u) |
                          (xyz[k][2] < zero ? 4u : 0u);
    Store(frustum.plane_codes_address() + 4 * k, code);
  }
  // Corner k from planes 4 or 5, 2 or 3 and 0 or 1 by k's bits.
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t planes = frustum.planes_address();
    const uint32_t a = (k & 1) != 0 ? planes + 64 : planes + 80;
    const uint32_t b = (k & 2) != 0 ? planes + 48 : planes + 32;
    const uint32_t c = (k & 4) != 0 ? planes + 0 : planes + 16;
    PlaneIntersect3(frustum.corners_address() + 16 * k, a, b, c, f);
  }
  return out;
}

uint32_t PlaneSegmentCrossing(uint32_t out, uint32_t plane, uint32_t b,
                              uint32_t a, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597E58 Plane::SegmentCrossing");
  const uint32_t s = stack;
  const Plane pl(plane);
  const Point pa(a), pb(b), po(out);
  const double f12 = double(pl.x());
  double f11 = double(pl.y());
  Stfs(s - 28, f11);
  const double ax = double(pa.x());
  Stfs(s - 32, f12);
  const double one = double(G::one());
  double f8 = double(pb.x());
  const double ay = double(pa.y());
  f8 = Fsubs(f8, ax);
  double f7 = double(pb.y());
  f11 = Fmuls(ay, f11);
  Stfs(s - 16, f8);
  f7 = Fsubs(f7, ay);
  Stfs(s - 20, double(G::zero()));
  const double pz = double(pl.z());
  Stfs(s - 24, pz);
  const double az = double(pa.z());
  const double bz = double(pb.z());
  Stfs(s - 12, f7);
  f7 = Fsubs(bz, az);
  Stfs(s - 4, one);
  const simde__m128 v7 = Lvx(s - 32);  // the plane's x, y, z and 0.0
  f11 = Fmadds(az, pz, f11);
  Stfs(s - 8, f7);
  const double pd = double(pl.d());
  const simde__m128 v10 = Lvx(s - 16);  // b - a and 1.0
  simde__m128 v0;
  simde__m128 v11;
  {
    FlushToZero flush;
    v11 = simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                          simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
    v0 = simde_mm_dp_ps(v10, v10, 0xEF);
  }
  // plane . a + d.
  double f0 = Fmadds(ax, f12, f11);
  f0 = Fadds(f0, pd);
  simde__m128 v13;
  simde__m128 v12;
  simde__m128 v9;
  {
    FlushToZero flush;
    v13 = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(v0));
    v9 = simde_mm_mul_ps(v0, v11);
    v12 = simde_mm_cmpeq_ps(v0, simde_mm_castsi128_ps(simde_mm_set1_epi32(0)));
  }
  const double minus_distance = Fneg(f0);
  {
    // The direction normalized (one Newton step on the reciprocal square
    // root; 0 where its length is 0), and its dot product with the plane.
    FlushToZero flush;
    const simde__m128 v8 = simde_mm_mul_ps(v13, v13);
    v11 = Nmsub(v9, v8, v11);
    v13 = simde_mm_add_ps(simde_mm_mul_ps(v13, v11), v13);
    v13 = simde_mm_mul_ps(v10, v13);
    v12 = simde_mm_castsi128_ps(
        simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_castps_si128(v12), simde_mm_castps_si128(v13)),
                          simde_mm_and_si128(simde_mm_castps_si128(v12), simde_mm_castps_si128(v0))));
    v0 = simde_mm_dp_ps(v7, v12, 0xEF);
    Stvx(s - 16, v12);
    Stvx(s - 32, v0);
  }
  const double along = Lfs(s - 32);
  if (Fabs(along) < double(G::crossing_epsilon())) {
    return 0;
  }
  // t = -(plane . a + d) / (plane . dir); out = dir t, then + a.
  f0 = Fdivs(minus_distance, along);
  double f12b = Lfs(s - 16);
  Stfs(s - 20, one);
  Stfs(s - 4, one);
  Stfs(s - 32, Fmuls(f12b, f0));
  f12b = Lfs(s - 12);
  Stfs(s - 28, Fmuls(f12b, f0));
  f12b = Lfs(s - 8);
  Stfs(s - 24, Fmuls(f12b, f0));
  Copy16(out, s - 32);
  const double x = Fadds(double(po.x()), double(pa.x()));
  Stfs(s - 16, x);
  const double ay2 = double(pa.y());
  const double az2 = double(pa.z());
  const double oy = double(po.y());
  const double oz = double(po.z());
  Stfs(s - 12, Fadds(oy, ay2));
  Stfs(s - 8, Fadds(oz, az2));
  Copy16(out, s - 16);
  po.set_w(float(one));
  return 1;
}

// --- The box ---

uint32_t Vector16Size(uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_825971E8 Vector16::Size");
  const schema::Vector16 v(vector);
  const uint32_t first = v.first().address;
  if (first == 0) {
    return 0;
  }
  return uint32_t(int32_t(v.last().address - first) >> 4);
}

void BoxExtend(uint32_t box, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_82597150 Box::Extend");
  // min = the point's unless min is below it (a NaN on either side takes
  // the point's).
  // (x, y, z: the fields at +0, +4, +8 of the min, the max and the point.)
  const schema::CarSceneBox b(box);
  const uint32_t p0 = Point(point).x_address();
  for (uint32_t i = 0; i < 12; i += 4) {
    const double min = Lfs(b.min_x_address() + i);
    const double p = Lfs(p0 + i);
    Stfs(b.min_x_address() + i, min < p ? min : p);
  }
  // max = the point's unless max is above it.
  for (uint32_t i = 0; i < 12; i += 4) {
    const double p = Lfs(p0 + i);
    const double max = Lfs(b.max_x_address() + i);
    Stfs(b.max_x_address() + i, max > p ? max : p);
  }
}

uint32_t BoxFromPoints(uint32_t box, uint32_t vector, uint32_t stack) {
  ITC_NATIVE_RUNNING("sub_82597210 Box::FromPoints");
  Store(stack - kBoxFrame, stack);  // the back chain, as stwu
  const schema::CarSceneBox b(box);
  const double min = double(G::box_min());
  b.set_min_x(float(min));
  b.set_min_y(float(min));
  b.set_min_z(float(min));
  const double one = double(G::one());
  b.set_min_w(float(one));
  b.set_max_w(float(one));
  const double max = double(G::box_max());
  b.set_max_x(float(max));
  b.set_max_y(float(max));
  b.set_max_z(float(max));
  if (Vector16Size(vector) != 0) {
    uint32_t i = 0;
    uint32_t offset = 0;
    do {
      BoxExtend(box, schema::Vector16(vector).first().address + offset);
      i++;
      offset += 16;
    } while (i < Vector16Size(vector));
  }
  return box;
}

}  // namespace car_scene_draw
