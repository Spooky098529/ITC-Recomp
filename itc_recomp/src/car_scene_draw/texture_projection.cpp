// TextureProjection::Fit (sub_82598768, docs/functions/car-scene-draw.md):
// the projection of a texture straight down (or along the light) fitted to
// the part of a camera pass's view frustum between two heights, step for
// step as the translated code (see car_scene_draw.h): the scalar float work
// through doubles rounded to single with flush-to-zero off, each vector
// float step with it on (the translation turns it on for those only), the
// stores and loads on its 1,248-byte frame at the original's offsets, and
// its callees' plain C++ run from that frame.

#include "car_scene_draw/car_scene_draw.h"

#include "car_scene_draw/ops.h"
#include "guest.h"

#include "base/log.h"
#include "interpolation/blend_state.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "schema/cameras.h"
#include "schema/car_scene_draw.h"

#include <cstdlib>

namespace car_scene_draw {

namespace {

using namespace ops;

using G = schema::CarSceneDrawGlobals;
using Point = schema::CarScenePoint;
using CameraPass = schema::CameraPass;

constexpr uint32_t kFitFrame = 1248;
// The frame's objects: the std::vector of points, its box and the frustum.
constexpr uint32_t kPoints = 128;
constexpr uint32_t kBox = 624;
constexpr uint32_t kFrustum = 784;

// The multiplies' return addresses (frame interpolation notes them; the one
// at 0x82599644 is the temporary camera of "The headlight beam far from the
// origin" in docs/frame-interpolation.md).
constexpr uint32_t kMultiplyCenter = 0x8259936C;
constexpr uint32_t kMultiplyScale = 0x82599644;
constexpr uint32_t kMultiplyFlip = 0x825996EC;
constexpr uint32_t kMultiplyFlipped = 0x825996FC;
constexpr uint32_t kMultiplyView = 0x8259970C;
constexpr uint32_t kMultiplyBias = 0x82599720;
constexpr uint32_t kPushBackReturn = 0x82598F58;

void Multiply(uint32_t out, uint32_t a, uint32_t b, uint32_t caller) {
  matrix_math::Multiply(out, a, b);
  interpolation::BlendState_OnMultiply(out, a, b, caller);
}

double Fnmadds(double a, double b, double c) { return double(float(-std::fma(a, b, c))); }

// The vector unit's float steps, each with flush-to-zero on.
simde__m128 VMul(simde__m128 a, simde__m128 b) {
  FlushToZero flush;
  return simde_mm_mul_ps(a, b);
}
simde__m128 VNmsub(simde__m128 a, simde__m128 b, simde__m128 c) {
  FlushToZero flush;
  return Nmsub(a, b, c);
}
simde__m128 VDot3(simde__m128 a, simde__m128 b) {
  FlushToZero flush;
  return Dot3(a, b);
}
// v scaled to length 1 by its x, y, z (or dot where dot is 0): ops::Normalized.
simde__m128 VNormalized(simde__m128 v, simde__m128 dot) {
  FlushToZero flush;
  return Normalized(v, dot);
}

// vmrghw and vmrglw as translated (word 0 in the top lane).
simde__m128 MergeHigh(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpackhi_epi32(simde_mm_castps_si128(b), simde_mm_castps_si128(a)));
}
simde__m128 MergeLow(simde__m128 a, simde__m128 b) {
  return simde_mm_castsi128_ps(
      simde_mm_unpacklo_epi32(simde_mm_castps_si128(b), simde_mm_castps_si128(a)));
}

// The point p times the row-vector matrix at `matrix` (its four rows read
// with lvx): the columns built by merges, each dotted with p (vmsum4fp128)
// and the four merged back, the original's every point transform.
simde__m128 Times(simde__m128 p, uint32_t matrix) {
  const simde__m128 r0 = Lvx(matrix);
  const simde__m128 r1 = Lvx(matrix + 16);
  const simde__m128 r2 = Lvx(matrix + 32);
  const simde__m128 r3 = Lvx(matrix + 48);
  const simde__m128 h02 = MergeHigh(r0, r2);
  const simde__m128 h13 = MergeHigh(r1, r3);
  const simde__m128 l02 = MergeLow(r0, r2);
  const simde__m128 l13 = MergeLow(r1, r3);
  const simde__m128 c0 = MergeHigh(h02, h13);
  const simde__m128 c1 = MergeLow(h02, h13);
  const simde__m128 c2 = MergeHigh(l02, l13);
  const simde__m128 c3 = MergeLow(l02, l13);
  simde__m128 d0, d1, d2, d3;
  {
    FlushToZero flush;
    d0 = simde_mm_dp_ps(p, c0, 0xFF);
    d1 = simde_mm_dp_ps(p, c1, 0xFF);
    d2 = simde_mm_dp_ps(p, c2, 0xFF);
    d3 = simde_mm_dp_ps(p, c3, 0xFF);
  }
  return MergeHigh(MergeHigh(d0, d2), MergeHigh(d1, d3));
}

// 64 bytes as eight ld and std pairs.
void Copy64(uint32_t to, uint32_t from) {
  for (uint32_t i = 0; i < 64; i += 8) {
    const uint32_t a = Load(from + i);
    const uint32_t b = Load(from + i + 4);
    Store(to + i, a);
    Store(to + i + 4, b);
  }
}

// One height plane's crossings (the loops at 0x82598D5C and 0x82598E6C):
// each far corner (+720) in the world, the eye's segment through it crossed
// with the plane, and where it crosses, the crossing back in the view
// replaces the corner.
void ClipFarCorners(uint32_t sp, uint32_t plane) {
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t corner = sp + 720 + 16 * j;
    Stvx(sp + 80, Times(Lvx(corner), sp + 144));
    if ((PlaneSegmentCrossing(sp + 96, plane, sp + 80, sp + 288, sp) & 0xFF) == 0) {
      continue;
    }
    Stvx(sp + 96, Times(Lvx(sp + 96), sp + 560));
    Copy16(corner, sp + 96);
  }
}

// The fit's two last crossings (0x82599488 and 0x82599528): the segment from
// +272 through +416 crossed with the plane, and where it crosses, its
// length from +272 (in every lane, at `length`) over the distance, kept in
// `widest` when larger.
void CrossingSpread(uint32_t sp, uint32_t plane, uint32_t length,
                    double distance, double one, double* widest) {
  if ((PlaneSegmentCrossing(sp + 96, plane, sp + 416, sp + 272, sp) & 0xFF) == 0) {
    return;
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    const double from = Lfs(sp + 272 + i);
    Stfs(sp + 128 + i, Fsubs(Lfs(sp + 96 + i), from));
  }
  Stfs(sp + 140, one);
  const simde__m128 dot = VDot3(Lvx(sp + 128), Lvx(sp + 128));
  Stvx(length, VNormalized(dot, dot));
  const double spread = Fdivs(Lfs(length), distance);
  if (!(*widest > spread)) {
    *widest = spread;
  }
}

}  // namespace

bool TextureProjectionFitDeclines(uint32_t pass) {
  return PerspectiveDeclines(Lfs(CameraPass(pass).field_of_view_address()));
}

bool TextureProjectionFit(uint32_t out, uint32_t pass, uint32_t direction,
                          double high, double low, double far_cut, double min_distance,
                          double distance_scale, uint32_t stack,
                          const PushBackCall& push_back, uint32_t* r3) {
  if (TextureProjectionFitDeclines(pass)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82598768 TextureProjection::Fit");
  const uint32_t sp = stack - kFitFrame;
  Store(sp, stack);  // the back chain, as stwu

  // The direction normalized (v127), kept at +336.
  const simde__m128 dir_in = Lvx(direction);
  const simde__m128 dir = VNormalized(dir_in, VDot3(dir_in, dir_in));
  Stvx(sp + 336, dir);
  // The pass's view (+560) and projection (+656).
  Copy64(sp + 560, CameraPass(pass).view_address());
  Copy64(sp + 656, CameraPass(pass).projection_address());
  // Its near and far ordered, the far cut to far_cut.
  double far_plane = Lfs(CameraPass(pass).far_address());   // f20
  double near_plane = Lfs(CameraPass(pass).near_address());  // f25
  if (far_plane < near_plane) {
    const double swapped = far_plane;
    far_plane = near_plane;
    near_plane = swapped;
  }
  if (!(far_plane < far_cut)) {
    far_plane = far_cut;
  }
  {
    const double width = Lfs(CameraPass(pass).width_address());
    const double height = Lfs(CameraPass(pass).height_address());
    const double fov = Lfs(CameraPass(pass).field_of_view_address());
    Perspective(sp + 656, fov, Fdivs(width, height), near_plane, far_plane, sp);
  }
  // The std::vector of 16-byte points at +128, empty.
  const schema::Vector16 points(sp + kPoints);
  points.set_first(0u);
  points.set_last(0u);
  points.set_end(0u);
  const schema::CarSceneFrustum frustum(sp + kFrustum);
  FrustumFromMatrix(frustum.address, sp + 656, sp);
  const double one = double(G::one());  // f30
  for (uint32_t i = 0; i < 8; i++) {
    Stfs(sp + 668 + 16 * i, one);
  }
  for (uint32_t at : {156u, 172u, 188u, 204u}) {
    Stfs(sp + at, one);
  }
  const double zero = double(G::zero());  // f31
  // The view's inverse (+144): the camera in the world.
  matrix_math::Inverse(sp + 144, sp + 560);

  // The height planes: up (0, 1, 0) normalized, at `low` (+304) and `high`
  // (+320); the eye (the origin in the world) at +288.
  Stfs(sp + 80, zero);
  Stfs(sp + 84, one);
  Stfs(sp + 88, zero);
  Stfs(sp + 92, zero);
  Stfs(sp + 288, zero);
  Stfs(sp + 292, zero);
  Stfs(sp + 296, zero);
  Stfs(sp + 300, one);
  const simde__m128 up1 = Lvx(sp + 80);
  const simde__m128 up1_dot = VDot3(up1, up1);
  Stfs(sp + 80, zero);
  Stfs(sp + 84, one);
  Stfs(sp + 88, zero);
  Stfs(sp + 92, zero);
  Stfs(sp + 108, one);
  const simde__m128 up2 = Lvx(sp + 80);
  const simde__m128 up2_dot = VDot3(up2, up2);
  Stvx(sp + 80, VNormalized(up1, up1_dot));
  {
    const double nz = Lfs(sp + 88);
    const double nx = Lfs(sp + 80);
    const double ny = Lfs(sp + 84);
    const double xz = Fadds(nz, nx);
    const double y = Fmuls(ny, low);
    Stfs(sp + 312, nz);
    Stfs(sp + 304, nx);
    Stfs(sp + 308, ny);
    Stfs(sp + 316, Fnmadds(xz, zero, y));
  }
  const simde__m128 eye = Times(Lvx(sp + 288), sp + 144);
  Stvx(sp + 80, VNormalized(up2, up2_dot));
  {
    const double nz = Lfs(sp + 88);
    const double nx = Lfs(sp + 80);
    const double ny = Lfs(sp + 84);
    const double xz = Fadds(nz, nx);
    const double y = Fmuls(ny, high);
    Stfs(sp + 328, nz);
    Stfs(sp + 320, nx);
    Stfs(sp + 324, ny);
    Stfs(sp + 332, Fnmadds(xz, zero, y));
  }
  Stvx(sp + 288, eye);

  // The frustum's corners: the near ones to +656, the far ones to +720.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t from = frustum.corners_address() + 32 * i;
    uint32_t w[8];
    for (uint32_t k = 0; k < 8; k++) {
      w[k] = Load(from + 4 * k);
    }
    for (uint32_t k = 0; k < 4; k++) {
      Store(sp + 656 + 16 * i + 4 * k, w[k]);
      Store(sp + 720 + 16 * i + 4 * k, w[4 + k]);
    }
  }
  // Each edge from a near corner to its far one: whether the near corner is
  // above the low plane and the eye's segment through the far one crosses
  // it between the near and far planes (counted in below), and the same
  // for below the high plane.
  uint32_t below = 0;  // r30
  uint32_t above = 0;  // r29
  double lowest = zero;   // f26 (not read after)
  double highest = zero;  // f27
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t far_corner = sp + 720 + 16 * j;
    const double plane_y = Lfs(sp + 312);
    const double plane_x = Lfs(sp + 304);
    Stvx(sp + 112, Times(Lvx(far_corner - 64), sp + 144));
    const double pz = Lfs(sp + 120);
    const double px = Lfs(sp + 112);
    const double py = Lfs(sp + 116);
    Stvx(sp + 80, Times(Lvx(far_corner), sp + 144));
    double side = Fmuls(pz, plane_y);
    side = Fmadds(px, plane_x, side);
    side = Fmadds(py, Lfs(sp + 308), side);
    side = Fadds(side, Lfs(sp + 316));
    if (side > zero) {
      if ((PlaneSegmentCrossing(sp + 96, sp + 304, sp + 80, sp + 288, sp) & 0xFF) != 0) {
        Stvx(sp + 96, Times(Lvx(sp + 96), sp + 560));
        const double depth = Lfs(sp + 104);
        if (depth > near_plane && depth < far_plane) {
          below++;
          if (depth > lowest) {
            lowest = depth;
          }
        }
      }
    }
    side = Fmuls(pz, Lfs(sp + 328));
    side = Fmadds(Lfs(sp + 320), px, side);
    side = Fmadds(py, Lfs(sp + 324), side);
    side = Fadds(side, Lfs(sp + 332));
    if (side < zero) {
      if ((PlaneSegmentCrossing(sp + 96, sp + 320, sp + 80, sp + 288, sp) & 0xFF) != 0) {
        Stvx(sp + 96, Times(Lvx(sp + 96), sp + 560));
        const double depth = Lfs(sp + 104);
        if (depth > near_plane && depth < far_plane) {
          above++;
          if (depth > highest) {
            highest = depth;
          }
        }
      }
    }
  }
  if (below == 4) {
    ClipFarCorners(sp, sp + 304);
  } else if (above == 4) {
    ClipFarCorners(sp, sp + 320);
  }
  // The eight corners into the vector.
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t value = sp + 656 + 16 * i;
    if (!Vector16PushBack(points.address, value, sp)) {
      push_back(points.address, value, sp, kPushBackReturn);
    }
  }

  // The view's turn (+208: the view without its shift) and the direction
  // turned by it (+112); a = d x (0, 0, -1) normalized (+80), c = d x a
  // normalized (+112): the turn to the direction's frame at +352.
  Stfs(sp + 80, zero);
  Stfs(sp + 84, zero);
  Stfs(sp + 92, zero);
  const double minus_one = double(G::minus_one());  // f21
  Stfs(sp + 88, minus_one);
  Copy64(sp + 208, sp + 560);
  Stfs(sp + 256, zero);
  Stfs(sp + 260, zero);
  Stfs(sp + 364, zero);
  Stfs(sp + 380, zero);
  Stfs(sp + 396, zero);
  Stfs(sp + 264, zero);
  Stfs(sp + 400, zero);
  Stfs(sp + 404, zero);
  Stfs(sp + 408, zero);
  Stfs(sp + 268, one);
  const simde__m128 back = Lvx(sp + 80);
  const simde__m128 d = Times(dir, sp + 208);
  Stvx(sp + 112, d);
  const simde__m128 a_raw =
      VNmsub(Permute<0x78>(d), Permute<0x9C>(back), VMul(Permute<0x9C>(d), Permute<0x78>(back)));
  Stvx(sp + 80, a_raw);
  const simde__m128 a = VNormalized(a_raw, VDot3(a_raw, a_raw));
  Stvx(sp + 80, a);
  Stfs(sp + 352, Lfs(sp + 80));
  Stfs(sp + 356, Lfs(sp + 112));
  Stfs(sp + 368, Lfs(sp + 84));
  Stfs(sp + 372, Lfs(sp + 116));
  Stfs(sp + 384, Lfs(sp + 88));
  Stfs(sp + 388, Lfs(sp + 120));
  const simde__m128 c_raw =
      VNmsub(Permute<0x78>(d), Permute<0x9C>(a), VMul(Permute<0x9C>(d), Permute<0x78>(a)));
  const simde__m128 c_dot = VDot3(c_raw, c_raw);
  const uint32_t first = points.first().address;  // r30
  Stfs(sp + 412, one);
  const uint32_t last = points.last().address;  // r31
  Stvx(sp + 112, VNormalized(c_raw, c_dot));
  const double cx = Lfs(sp + 112);  // f24
  const double cy = Lfs(sp + 116);  // f23
  const double cz = Lfs(sp + 120);  // f22
  Stfs(sp + 360, cx);
  Stfs(sp + 376, cy);
  Stfs(sp + 392, cz);
  // The points into that frame.
  if (first != 0) {
    const uint32_t count = uint32_t(int32_t(last - first) >> 4);
    for (uint32_t i = 0; i < count; i++) {
      const uint32_t at = first + 16 * i;
      Stvx(at, Times(Lvx(at), sp + 352));
    }
  }

  // Their box (+624), its center's x and y with its lowest z less the
  // distance (+80) and the spread of the points seen from there.
  const schema::CarSceneBox box(sp + kBox);
  BoxFromPoints(box.address, points.address, sp);
  double distance;  // f29
  const double half = double(G::half());  // f25
  double eye_z;  // f9
  {
    const double sx = Fadds(double(box.min_x()), double(box.max_x()));
    const double sy = Fadds(double(box.max_y()), double(box.min_y()));
    const double min_z = double(box.min_z());
    const double sz = Fadds(double(box.max_z()), min_z);
    distance = Fmuls(far_plane, distance_scale);
    Stfs(sp + 124, one);
    Stfs(sp + 112, Fmuls(sx, half));
    Stfs(sp + 116, Fmuls(sy, half));
    Stfs(sp + 120, Fmuls(sz, half));
    Copy16(sp + 80, sp + 112);
    Stfs(sp + 92, one);
    if (!(distance > min_distance)) {
      distance = min_distance;
    }
    eye_z = Fsubs(min_z, distance);
  }
  const double eye_y = Lfs(sp + 84);  // f8
  const double eye_x = Lfs(sp + 80);  // f7
  Stfs(sp + 88, eye_z);
  double spread_x = zero;  // f26
  double spread_y = zero;  // f28
  double depth = zero;     // f27
  for (uint32_t at = first; at != last; at += 16) {
    const Point point(at);
    const double z = Fsubs(double(point.z()), eye_z);
    const double x = Fsubs(double(point.x()), eye_x);
    const double y = Fsubs(double(point.y()), eye_y);
    const double scale = Fdivs(one, z);
    const double sx = Fmuls(x, scale);
    if (!(spread_x > Fabs(sx))) {
      spread_x = Fabs(sx);
    }
    const double sy = Fmuls(y, scale);
    if (!(spread_y > Fabs(sy))) {
      spread_y = Fabs(sy);
    }
    if (!(depth > z)) {
      depth = z;
    }
  }

  // The identity (+208, built from vupkd3d128's constant), copied to +656
  // with the eye's shift: +352 = the frame's turn x that shift, its inverse
  // at +496.
  {
    const simde__m128 k = VectorOf(std::bit_cast<uint32_t>(3.0f), std::bit_cast<uint32_t>(3.0f),
                                   std::bit_cast<uint32_t>(0.0f), std::bit_cast<uint32_t>(1.0f));
    Stfs(sp + 508, one);
    Stfs(sp + 524, one);
    Stfs(sp + 540, one);
    Stfs(sp + 556, one);
    Stvx(sp + 208, Permute<0x15>(k));
    Stvx(sp + 224, Permute<0x45>(k));
    Stvx(sp + 240, Permute<0x51>(k));
    Stvx(sp + 256, Permute<0x54>(k));
  }
  Copy64(sp + 656, sp + 208);
  Stfs(sp + 704, Fneg(eye_x));
  Stfs(sp + 708, Fneg(eye_y));
  Stfs(sp + 712, Fneg(eye_z));
  Multiply(sp + 352, sp + 352, sp + 656, kMultiplyCenter);
  matrix_math::Inverse(sp + 496, sp + 352);

  // The eye back in the world (+272) and a point the direction on from the
  // center's c (+416); the spread at the planes' crossings of that segment.
  const double ox = Fmuls(distance, cx);
  Stfs(sp + 284, one);
  const double oy = Fmuls(distance, cy);
  const double oz = Fmuls(distance, cz);
  Stfs(sp + 124, one);
  Stfs(sp + 140, one);
  Stfs(sp + 428, one);
  Stvx(sp + 272, Times(Lvx(sp + 80), sp + 496));
  {
    const double px = Fadds(ox, Lfs(sp + 272));
    const double py = Fadds(Lfs(sp + 276), oy);
    const double pz = Fadds(Lfs(sp + 280), oz);
    Stfs(sp + 112, px);
    Stfs(sp + 116, py);
    Stfs(sp + 120, pz);
    Stfs(sp + 128, Fadds(px, Lfs(sp + 336)));
    Stfs(sp + 132, Fadds(Lfs(sp + 340), py));
    Stfs(sp + 136, Fadds(Lfs(sp + 344), pz));
    Copy16(sp + 272, sp + 112);
    Copy16(sp + 416, sp + 128);
  }
  CrossingSpread(sp, sp + 320, sp + 336, distance, one, &spread_y);
  CrossingSpread(sp, sp + 304, sp + 416, distance, one, &spread_y);

  // The projection's scale (+208) and its multiply into +352.
  {
    double wx = Fmuls(spread_x, distance);
    double wy = Fmuls(spread_y, distance);
    const double near_offset = Fsubs(depth, distance);
    const double product = Fmuls(depth, distance);
    const double far_offset = Fsubs(distance, depth);
    for (uint32_t at : {212u, 216u, 220u, 224u, 232u}) {
      Stfs(sp + at, zero);
    }
    const double two = double(G::two());  // f28
    wx = Fmuls(wx, two);
    Stfs(sp + 236, zero);
    wy = Fmuls(wy, two);
    Stfs(sp + 240, zero);
    Stfs(sp + 248, Fdivs(depth, near_offset));
    Stfs(sp + 244, zero);
    const double offset = Fdivs(product, far_offset);
    Stfs(sp + 252, one);
    Stfs(sp + 256, zero);
    Stfs(sp + 260, zero);
    Stfs(sp + 264, offset);
    Stfs(sp + 268, zero);
    const double sx = Fdivs(distance, wx);
    const double sy = Fdivs(distance, wy);
    Stfs(sp + 208, Fmuls(sx, two));
    Stfs(sp + 228, Fmuls(sy, two));
    Multiply(sp + 352, sp + 352, sp + 208, kMultiplyScale);

    // The flip (+432) by the scale (+208), into +352; out = the view x
    // +352 x the bias matrix.
    Stfs(sp + 432, minus_one);
    Stfs(sp + 436, zero);
    Stfs(sp + 440, zero);
    Stfs(sp + 444, zero);
    Stfs(sp + 484, double(G::light_x()));
    Stfs(sp + 448, zero);
    Stfs(sp + 452, zero);
    Stfs(sp + 456, minus_one);
    Stfs(sp + 488, double(G::flip_488()));
    Stfs(sp + 460, zero);
    Stfs(sp + 464, zero);
    Stfs(sp + 468, one);
    const double shift = double(G::flip_264());
    Stfs(sp + 472, zero);
    Stfs(sp + 476, zero);
    Stfs(sp + 480, zero);
    Stfs(sp + 492, one);
    Stfs(sp + 208, one);
    for (uint32_t at : {212u, 216u, 220u, 224u}) {
      Stfs(sp + at, zero);
    }
    Stfs(sp + 228, two);
    for (uint32_t at : {232u, 236u, 240u, 244u}) {
      Stfs(sp + at, zero);
    }
    Stfs(sp + 248, half);
    Stfs(sp + 252, zero);
    Stfs(sp + 256, zero);
    Stfs(sp + 260, zero);
    Stfs(sp + 264, shift);
    Stfs(sp + 268, one);
  }
  Multiply(sp + 432, sp + 432, sp + 208, kMultiplyFlip);
  Multiply(sp + 352, sp + 352, sp + 432, kMultiplyFlipped);
  Multiply(out, sp + 560, sp + 352, kMultiplyView);
  Multiply(out, out, G::bias_matrix_address(), kMultiplyBias);
  *r3 = out;
  if (first != 0) {
    if (!memory::crt::OperatorDelete(first)) {
      ITC_LOG_ERROR("TextureProjection::Fit: operator delete refused {:08X}", first);
      std::abort();
    }
    *r3 = first;  // operator delete leaves its block in r3
  }
  return true;
}

}  // namespace car_scene_draw
