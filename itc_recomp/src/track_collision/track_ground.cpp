// The ground under a point: the mesh's triangles, their planes and colors,
// and the ground queries (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "collision/collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

namespace track_collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace co = collision;
namespace va = vehicle_accessors;

namespace {

using Fpscr = rex::platform::FPSCRPlatform;

constexpr uint32_t kHalf = 0x82001BE4;              // 0.5
constexpr uint32_t kSliceOffsets = 0x8200680C;      // 4 words: the slices tried around one
constexpr uint32_t kInsideEpsilon = 0x820029D8;     // an edge's cross product at most this
constexpr uint32_t kKindHeightOffset = 0x82002A08;  // added to the height (kinds 4 and 20)
constexpr uint32_t kNoOtherLowering = 0x82002FB0;   // kinds 4 and 20 with no other triangle
constexpr uint32_t kCatchDistance = 0x82002A0C;     // the ground within this of the point
constexpr uint32_t kColorScale = 0x82006820;
constexpr uint32_t kColorNumerator = 0x82001BE0;
constexpr uint32_t kUp = 0x82640000u - 32128;  // 0x82638280: the up vector
constexpr uint32_t kEpsilon = 0x8200222C;       // collision.md's small epsilon

uint32_t S16(uint8_t* base, uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(base, address))));
}

// Section `index` of the mesh's file (the table at [[mesh + 4] + 4]).
uint32_t Section(uint8_t* base, uint32_t mesh, uint32_t index) {
  return guest::LoadU32(base, index * 4 + guest::LoadU32(base, guest::LoadU32(base, mesh + 4) + 4));
}

// Vertex `index` of the section (12 bytes each from its +64).
uint32_t Vertex(uint8_t* base, uint32_t section, uint32_t index) {
  return index * 12 + guest::LoadU32(base, section + 64) + section;
}

// The vector unit's normalize as the translated code does it, flush-to-zero
// on: the 16 bytes at `from` rounded down to 16 (x, y, z, w), the squared
// length of x, y, z (vmsum3fp128: dpps 0xEF), its reciprocal square root as
// 1/sqrt and one Newton step (vrsqrtefp, vnmsubfp, vmaddfp), all four lanes
// times it; the squared length itself (0) where it's 0. Stored to the 16
// bytes at `to` rounded down.
void NormalizeVector(uint8_t* base, uint32_t from, uint32_t to) {
  const uint32_t in = from & ~0xFu;
  simde__m128 v = simde_mm_castsi128_ps(
      simde_mm_set_epi32(int(guest::LoadU32(base, in + 0)), int(guest::LoadU32(base, in + 4)),
                         int(guest::LoadU32(base, in + 8)), int(guest::LoadU32(base, in + 12))));
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vl::PinInMode(v);
  simde__m128 half = simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_set1_epi32(1)),
                                     simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000))));
  vl::PinInMode(half);
  const simde__m128 squared = simde_mm_dp_ps(v, v, 0xEF);
  simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(squared));
  const simde__m128 half_squared = simde_mm_mul_ps(squared, half);
  const simde__m128 zero = simde_mm_cmpeq_ps(squared, simde_mm_castsi128_ps(simde_mm_set1_epi32(0)));
  const simde__m128 estimate2 = simde_mm_mul_ps(estimate, estimate);
  const simde__m128 step = simde_mm_xor_ps(
      simde_mm_sub_ps(simde_mm_mul_ps(half_squared, estimate2), half),
      simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000))));
  estimate = simde_mm_add_ps(simde_mm_mul_ps(estimate, step), estimate);
  const simde__m128 scaled = simde_mm_mul_ps(v, estimate);
  simde__m128i result =
      simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_castps_si128(zero), simde_mm_castps_si128(scaled)),
                        simde_mm_and_si128(simde_mm_castps_si128(zero), simde_mm_castps_si128(squared)));
  vl::PinInMode(result);
  Fpscr::setcsr(csr);
  alignas(16) uint32_t lanes[4];
  simde_mm_store_si128(reinterpret_cast<simde__m128i*>(lanes), result);
  const uint32_t out = to & ~0xFu;
  guest::StoreU32(base, out + 0, lanes[3]);
  guest::StoreU32(base, out + 4, lanes[2]);
  guest::StoreU32(base, out + 8, lanes[1]);
  guest::StoreU32(base, out + 12, lanes[0]);
}

// An RGB555 color's channels as the original takes them: (c >> 7) & 0xF8,
// (c >> 2) & 0xF8, (c << 3) & 0xF8, each as a float.
double Red(uint32_t c) {
  return double(float(double(int64_t((c >> 7) & 0xF8))));
}
double Green(uint32_t c) {
  return double(float(double(int64_t((c >> 2) & 0xF8))));
}
double Blue(uint32_t c) {
  return double(float(double(int64_t((c << 3) & 0xF8))));
}

}  // namespace

uint32_t TrackMeshTriangleInSlice(uint8_t* base, uint32_t section, uint32_t slice, uint32_t point,
                                  uint32_t skip_kinds) {
  ITC_NATIVE_RUNNING("sub_820F72F8 TrackMesh::TriangleInSlice");
  const uint32_t slice_at = guest::LoadU32(base, section + 72) + slice * 28 + section;
  const uint32_t vertices = guest::LoadU32(base, section + 64) + section;
  const uint32_t first = guest::LoadU16(base, slice_at + 2);
  const int32_t count = int32_t(guest::LoadU16(base, slice_at + 0));
  uint32_t triangle = first * 12 + guest::LoadU32(base, section + 76) + section;
  if (count <= 0) {
    return uint32_t(-1);
  }
  const double epsilon = Lfs(base, kInsideEpsilon);
  for (int32_t i = 0; i < count; i++, triangle += 12) {
    if (int32_t(skip_kinds) == 1) {
      int32_t kind = guest::LoadU8(base, triangle + 9);
      if (kind >= 9) {
        if (kind == 20) {
          continue;
        }
        kind = 0;
      }
      if (kind == 4 || kind == 20) {
        continue;
      }
    }
    const uint32_t v0 = guest::LoadU16(base, triangle + 0) * 12 + vertices;
    const double pz = Lfs(base, point + 8);
    const uint32_t v1 = guest::LoadU16(base, triangle + 2) * 12 + vertices;
    const double px = Lfs(base, point + 0);
    const double f7 = Fsubs(Lfs(base, v0 + 0), px);
    const double f8 = Fsubs(Lfs(base, v1 + 8), pz);
    const double f6 = Fsubs(Lfs(base, v0 + 8), pz);
    const double f9 = Fsubs(Lfs(base, v1 + 0), px);
    if (Fmsubs(f9, f6, Fmuls(f8, f7)) > epsilon) {
      continue;
    }
    const uint32_t v2 = guest::LoadU16(base, triangle + 4) * 12 + vertices;
    const double pz2 = Lfs(base, point + 8);
    const double f11 = Fsubs(Lfs(base, v2 + 8), pz2);
    const double f12 = Fsubs(Lfs(base, v2 + 0), px);
    if (Fmsubs(f12, f8, Fmuls(f11, f9)) > epsilon) {
      continue;
    }
    double last;
    if (guest::LoadU16(base, triangle + 6) == 0xFFFF) {
      last = Fmsubs(f11, f7, Fmuls(f12, f6));
    } else {
      const uint32_t v3 = guest::LoadU16(base, triangle + 6) * 12 + vertices;
      const double f10 = Fsubs(Lfs(base, v3 + 8), pz2);
      const double f0 = Fsubs(Lfs(base, v3 + 0), px);
      if (Fmsubs(f0, f11, Fmuls(f10, f12)) > epsilon) {
        continue;
      }
      last = Fmsubs(f10, f7, Fmuls(f0, f6));
    }
    if (!(last > epsilon)) {
      return first + uint32_t(i);
    }
  }
  return uint32_t(-1);
}

void TrackMeshTriangleNormal(uint8_t* base, uint32_t out, uint32_t section, uint32_t triangle,
                             uint32_t a, uint32_t b, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_820F7468 TrackMesh::TriangleNormal");
  const double one = Lfs(base, kOne);
  // The original's names: A the vertex `b`, B `a`, C `c`.
  const uint32_t va = Vertex(base, section, guest::LoadU16(base, triangle + 2 * b));
  const uint32_t vb = Vertex(base, section, guest::LoadU16(base, triangle + 2 * a));
  const uint32_t vc = Vertex(base, section, guest::LoadU16(base, triangle + 2 * c));
  {
    const double ay = Lfs(base, va + 4), az = Lfs(base, va + 8);
    const double t = Fsubs(ay, Lfs(base, vb + 4));
    const double u = Fsubs(Lfs(base, vc + 8), az);
    const double cy = Fsubs(Lfs(base, vc + 4), ay);
    const double d = Fsubs(az, Lfs(base, vb + 8));
    Stfs(base, out + 0, Fmsubs(cy, d, Fmuls(t, u)));
  }
  {
    const double ax = Lfs(base, va + 0), az = Lfs(base, va + 8);
    const double t = Fsubs(Lfs(base, vc + 0), ax);
    const double u = Fsubs(az, Lfs(base, vb + 8));
    const double cz = Fsubs(Lfs(base, vc + 8), az);
    const double d = Fsubs(ax, Lfs(base, vb + 0));
    Stfs(base, out + 4, Fmsubs(cz, d, Fmuls(t, u)));
  }
  {
    const double ay = Lfs(base, va + 4), ax = Lfs(base, va + 0);
    const double t = Fsubs(Lfs(base, vc + 4), ay);
    const double u = Fsubs(ax, Lfs(base, vb + 0));
    const double cx = Fsubs(Lfs(base, vc + 0), ax);
    const double d = Fsubs(ay, Lfs(base, vb + 4));
    Stfs(base, out + 8, Fmsubs(cx, d, Fmuls(t, u)));
  }
  Stfs(base, out + 12, one);
  NormalizeVector(base, out, out);
  Stfs(base, out + 12, one);
}

double TrackMeshQuadHeight(uint8_t* base, uint32_t out, uint32_t section, uint32_t quad,
                           uint32_t point, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F7590 TrackMesh::QuadHeight");
  const uint32_t f = frame;
  const double pz = Lfs(base, point + 8);
  const double px = Lfs(base, point + 0);
  const double one = Lfs(base, kOne);
  Stfs(base, f + 92, one);
  Stfs(base, f + 108, one);
  const uint32_t v0 = Vertex(base, section, guest::LoadU16(base, quad + 0));
  const uint32_t v2 = Vertex(base, section, guest::LoadU16(base, quad + 4));
  const uint32_t v1 = Vertex(base, section, guest::LoadU16(base, quad + 2));
  const uint32_t v3 = Vertex(base, section, guest::LoadU16(base, quad + 6));
  const double d0z = Fsubs(Lfs(base, v0 + 8), pz);
  const double d0x = Fsubs(Lfs(base, v0 + 0), px);
  const double d2z = Fsubs(Lfs(base, v2 + 8), pz);
  const double d2x = Fsubs(Lfs(base, v2 + 0), px);
  Stfs(base, f + 148, d2z);
  Stfs(base, f + 112, d0x);
  Stfs(base, f + 116, d0z);
  Stfs(base, f + 144, d2x);
  const double side = Fmsubs(d2x, d0z, Fmuls(d2z, d0x));
  const double d1x = Fsubs(Lfs(base, v1 + 0), px);
  const double d3x = Fsubs(Lfs(base, v3 + 0), px);
  const double d3z = Fsubs(Lfs(base, v3 + 8), pz);
  const double d1z = Fsubs(Lfs(base, v1 + 8), pz);
  Stfs(base, f + 128, d1x);
  Stfs(base, f + 132, d1z);
  Stfs(base, f + 160, d3x);
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 164, d3z);
  // The half the point is on: vertices 0, 1, 2 or 0, 2, 3.
  if (side > zero) {
    TrackMeshTriangleNormal(base, f + 80, section, quad, 0, 1, 2);
  } else {
    TrackMeshTriangleNormal(base, f + 80, section, quad, 0, 2, 3);
  }
  const double t = Fmuls(Lfs(base, f + 80), d0x);
  const double u = Fmuls(d3z, d1x);
  const double first_height =
      Fadds(Fdivs(Fmadds(Lfs(base, f + 88), d0z, t), Lfs(base, f + 84)), Lfs(base, v0 + 4));
  const double other_side = Fmsubs(d3x, d1z, u);
  double other_y;
  uint32_t other;
  if (other_side > zero) {
    TrackMeshTriangleNormal(base, f + 96, section, quad, 1, 2, 3);
    other_y = Lfs(base, v1 + 4);
    other = f + 128;
  } else {
    TrackMeshTriangleNormal(base, f + 96, section, quad, 0, 1, 3);
    other_y = Lfs(base, v0 + 4);
    other = f + 112;
  }
  const double n2x = Lfs(base, f + 96);
  const double sum_x = Fadds(n2x, Lfs(base, f + 80));
  const double n2y = Lfs(base, f + 100);
  const double sum_y = Fadds(n2y, Lfs(base, f + 84));
  const double n2z = Lfs(base, f + 104);
  const double t2 = Fmuls(Lfs(base, other + 0), n2x);
  const double other_z = Lfs(base, other + 4);
  const double sum_z = Fadds(n2z, Lfs(base, f + 88));
  const double half = Lfs(base, kHalf);
  Stfs(base, out + 0, Fmuls(sum_x, half));
  Stfs(base, out + 4, Fmuls(sum_y, half));
  const double plane = Fmadds(other_z, n2z, t2);
  Stfs(base, out + 8, Fmuls(sum_z, half));
  const double second_height = Fadds(Fdivs(plane, n2y), other_y);
  const double result = Fmuls(Fadds(second_height, first_height), half);
  NormalizeVector(base, out, out);
  return result;
}

uint32_t TrackMeshFindTriangle(uint8_t* base, uint32_t mesh, uint32_t section_out,
                               uint32_t triangle_out, uint32_t point, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F7818 TrackMesh::FindTriangle");
  uint32_t slice_found = guest::LoadU16(base, position + 2);
  const uint32_t section_index = S16(base, position + 0);
  if (int32_t(section_index) == -1 || int32_t(S16(base, position + 2)) == -1) {
    return uint32_t(-1);
  }
  guest::StoreU32(base, section_out, Section(base, mesh, section_index));
  const uint32_t slice = S16(base, position + 2);
  uint32_t found = TrackMeshTriangleInSlice(base, guest::LoadU32(base, section_out), slice, point, 0);
  auto done = [&](uint32_t triangle) {
    const uint32_t section = guest::LoadU32(base, section_out);
    guest::StoreU32(base, triangle_out,
                    triangle * 12 + guest::LoadU32(base, section + 76) + section);
    return slice_found;
  };
  if (int32_t(found) != -1) {
    return done(found);
  }
  // The slices offset from it in its section.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t s = uint32_t(int32_t(int16_t(guest::LoadU32(base, kSliceOffsets + 4 * i) + slice)));
    if (int32_t(s) < 0 ||
        int32_t(s) >= int32_t(S16(base, guest::LoadU32(base, section_out) + 0))) {
      continue;
    }
    const uint32_t section = Section(base, mesh, S16(base, position + 0));
    found = TrackMeshTriangleInSlice(base, section, s, point, 0);
    if (int32_t(found) != -1) {
      slice_found = s;
      guest::StoreU32(base, section_out, section);
      return done(found);
    }
  }
  // The cells the section's links lead to from this slice.
  {
    const uint32_t links = guest::LoadU32(base, section_out) + 8;
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = links + 6 * i;
      const int32_t from = int32_t(S16(base, link - 4));
      if (from == -1 || from != int32_t(S16(base, position + 2))) {
        continue;
      }
      const uint32_t target_index = S16(base, link - 2);
      const uint32_t target_slice = guest::LoadU16(base, link + 0);
      const uint32_t section = Section(base, mesh, target_index);
      found = TrackMeshTriangleInSlice(base, section, uint32_t(int32_t(int16_t(target_slice))),
                                       point, 0);
      if (int32_t(found) != -1) {
        slice_found = target_slice;
        guest::StoreU32(base, section_out, section);
        return done(found);
      }
    }
  }
  // The links leaving from within 2 slices of it: their cell and the
  // slices offset from it.
  for (uint32_t offset = 0; offset < 36; offset += 6) {
    const uint32_t link = guest::LoadU32(base, section_out) + offset;
    const int32_t from = int32_t(S16(base, link + 4));
    if (from == -1) {
      continue;
    }
    const int32_t here = int32_t(S16(base, position + 2));
    if (from < here - 2 || from > here + 2) {
      continue;
    }
    const int32_t target_index = int32_t(S16(base, link + 6));
    if (target_index < 0) {
      continue;
    }
    const uint32_t target_slice =
        guest::LoadU16(base, guest::LoadU32(base, section_out) + offset + 8);
    const uint32_t section = Section(base, mesh, uint32_t(target_index));
    if (from != here) {
      found = TrackMeshTriangleInSlice(base, section, uint32_t(int32_t(int16_t(target_slice))),
                                       point, 0);
      if (int32_t(found) != -1) {
        slice_found = target_slice;
        guest::StoreU32(base, section_out, section);
        return done(found);
      }
    }
    const uint32_t around = uint32_t(int32_t(int16_t(target_slice)));
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t s =
          uint32_t(int32_t(int16_t(guest::LoadU32(base, kSliceOffsets + 4 * i) + around)));
      if (int32_t(s) < 0 || int32_t(s) >= int32_t(S16(base, section + 0))) {
        continue;
      }
      found = TrackMeshTriangleInSlice(base, section, s, point, 0);
      if (int32_t(found) != -1) {
        slice_found = s;
        guest::StoreU32(base, section_out, section);
        return done(found);
      }
    }
  }
  return uint32_t(-1);
}

void TrackMeshColorAt(uint8_t* base, uint32_t out, uint32_t point, uint32_t triangle,
                      uint32_t vertices, uint32_t colors) {
  ITC_NATIVE_RUNNING("sub_820F6E18 TrackMesh::ColorAt");
  // The three vertices A, B, C (the original's order) and their colors.
  uint32_t a, b, c;
  bool first_half = true;
  if (guest::LoadU16(base, triangle + 6) != 0xFFFF) {
    const uint32_t v2 = guest::LoadU16(base, triangle + 4) * 12 + vertices;
    const double pz = Lfs(base, point + 8);
    const uint32_t v0 = guest::LoadU16(base, triangle + 0) * 12 + vertices;
    const double px = Lfs(base, point + 0);
    const double v2z = Lfs(base, v2 + 8), v2x = Lfs(base, v2 + 0);
    const double dz = Fsubs(pz, v2z);
    const double dx = Fsubs(px, v2x);
    const double ez = Fsubs(Lfs(base, v0 + 8), v2z);
    const double ex = Fsubs(Lfs(base, v0 + 0), v2x);
    const double side = Fmsubs(ex, dz, Fmuls(ez, dx));
    first_half = !(side < Lfs(base, kZero));
  }
  if (first_half) {
    a = guest::LoadU16(base, triangle + 0);
    b = guest::LoadU16(base, triangle + 2);
    c = guest::LoadU16(base, triangle + 4);
  } else {
    a = guest::LoadU16(base, triangle + 4);
    b = guest::LoadU16(base, triangle + 6);
    c = guest::LoadU16(base, triangle + 0);
  }
  const uint32_t ca = guest::LoadU16(base, colors + 2 * a);
  const uint32_t cb = guest::LoadU16(base, colors + 2 * b);
  const uint32_t cc = guest::LoadU16(base, colors + 2 * c);
  const uint32_t va = a * 12 + vertices, vb = b * 12 + vertices, vc = c * 12 + vertices;
  const double ax = Lfs(base, va + 0), az = Lfs(base, va + 8);
  const double bx = Lfs(base, vb + 0), bz = Lfs(base, vb + 8);
  const double cx = Lfs(base, vc + 0), cz = Lfs(base, vc + 8);
  const double pz = Lfs(base, point + 8);
  const double px = Lfs(base, point + 0);
  const double ra = Red(ca), ga = Green(ca), ba = Blue(ca);
  const double rb = Red(cb), gb = Green(cb), bb = Blue(cb);
  const double rc = Red(cc), gc = Green(cc), bc = Blue(cc);
  const double dr_ab = Fsubs(ra, rb);
  const double acx = Fsubs(ax, cx);
  const double acz = Fsubs(az, cz);
  const double abx = Fsubs(ax, bx);
  const double pcz = Fsubs(pz, cz);
  const double abz = Fsubs(az, bz);
  const double pcx = Fsubs(px, cx);
  const double dr_ac = Fsubs(ra, rc);
  const double numerator = Lfs(base, kColorNumerator);
  const double db_ac = Fsubs(ba, bc);
  const double dg_ac = Fsubs(ga, gc);
  const double dg_ab = Fsubs(ga, gb);
  const double db_ab = Fsubs(ba, bb);
  const double t = Fmuls(acz, abx);
  const double u = Fmuls(abx, pcz);
  const double area = Fmsubs(abz, acx, t);
  const double w = Fmuls(acx, pcz);
  const double scale = Fdivs(numerator, area);
  const double s = Fmuls(Fmsubs(acz, pcx, w), scale);
  const double r = Fmuls(Fmsubs(abz, pcx, u), scale);
  const double red = Fsubs(Fsubs(Fmuls(dr_ab, s), Fmuls(dr_ac, r)), rc);
  const double green = Fsubs(Fsubs(Fmuls(dg_ab, s), Fmuls(dg_ac, r)), gc);
  const double blue = Fsubs(Fsubs(Fmuls(db_ab, s), Fmuls(db_ac, r)), bc);
  const double k = Lfs(base, kColorScale);
  Stfs(base, out + 0, Fmuls(red, k));
  Stfs(base, out + 4, Fmuls(green, k));
  Stfs(base, out + 8, Fmuls(blue, k));
}

uint32_t TrackMeshGroundHeight(uint8_t* base, uint32_t mesh, uint32_t point, uint32_t position,
                               uint32_t color, uint32_t normal, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F7AA0 TrackMesh::GroundHeight");
  const uint32_t f = frame;
  const double one = Lfs(base, kOne);
  Stfs(base, f + 108, one);
  guest::StoreU32(base, f + 80, 0);
  guest::StoreU32(base, f + 84, 0);
  bool other_found = true;
  const double zero = Lfs(base, kZero);
  if (color != 0) {
    Stfs(base, color + 0, zero);
    Stfs(base, color + 4, zero);
    Stfs(base, color + 8, zero);
    Stfs(base, color + 12, one);
  }
  if (normal != 0) {
    Stfs(base, normal + 0, zero);
    Stfs(base, normal + 8, zero);
    Stfs(base, normal + 12, one);
    Stfs(base, normal + 4, zero);
    Stfs(base, normal + 4, one);
  }
  const uint32_t slice =
      uint32_t(int32_t(int16_t(TrackMeshFindTriangle(base, mesh, f + 80, f + 84, point, position))));
  if (int32_t(slice) == -1) {
    return uint32_t(-1);
  }
  uint32_t triangle = guest::LoadU32(base, f + 84);
  const uint32_t section = guest::LoadU32(base, f + 80);
  const uint32_t vertex_offset = guest::LoadU32(base, section + 64);
  uint32_t v0 = guest::LoadU16(base, triangle + 0) * 12 + vertex_offset + section;
  uint32_t kind = guest::LoadU8(base, triangle + 9);
  if (int32_t(kind) >= 9) {
    kind = kind == 20 ? 4 : 0;
  }
  if (kind == 4 || kind == 20) {
    // Another triangle without those kinds in the slice or those offset
    // from it.
    uint32_t other = TrackMeshTriangleInSlice(base, section, slice, point, 1);
    for (uint32_t i = 0; int32_t(other) == -1 && i < 4; i++) {
      const uint32_t s = slice + guest::LoadU32(base, kSliceOffsets + 4 * i);
      if (int32_t(s) < 0 || int32_t(s) >= int32_t(S16(base, section + 0))) {
        continue;
      }
      other = TrackMeshTriangleInSlice(base, section, s, point, 1);
    }
    if (int32_t(other) != -1) {
      triangle = other * 12 + guest::LoadU32(base, section + 76) + section;
      v0 = guest::LoadU16(base, triangle + 0) * 12 + vertex_offset + section;
    } else {
      other_found = false;
    }
  }
  double height;
  if (guest::LoadU16(base, triangle + 6) != 0xFFFF &&
      (guest::LoadU16(base, triangle + 10) & 0x1) != 0) {
    height = TrackMeshQuadHeight(base, f + 96, section, triangle, point, f - 208);
  } else {
    TrackMeshTriangleNormal(base, f + 96, section, triangle, 0, 1, 2);
    const double inverse = Fdivs(one, Lfs(base, f + 100));
    const double dx = Fsubs(Lfs(base, v0 + 0), Lfs(base, point + 0));
    const double nx = Lfs(base, f + 96);
    const double dz = Fsubs(Lfs(base, v0 + 8), Lfs(base, point + 8));
    const double nz = Lfs(base, f + 104);
    const double y = Lfs(base, v0 + 4);
    const double sx = Fmuls(inverse, nx);
    const double sz = Fmuls(inverse, nz);
    height = Fadds(Fmadds(dz, sz, Fmuls(sx, dx)), y);
  }
  height = Fadds(height, Lfs(base, kKindHeightOffset));
  if (!other_found) {
    height = Fsubs(height, Lfs(base, kNoOtherLowering));
  }
  const double off = Fabs(Fsubs(Lfs(base, point + 4), height));
  if (!(off < Lfs(base, kCatchDistance))) {
    return uint32_t(-2);
  }
  Stfs(base, point + 4, height);
  if (color != 0) {
    TrackMeshColorAt(base, color, point, triangle, guest::LoadU32(base, section + 64) + section,
                     guest::LoadU32(base, section + 68) + section);
  }
  if (normal != 0) {
    Stfs(base, f + 108, one);
    NormalizeVector(base, f + 96, normal);
  }
  return kind;
}

uint32_t TrackCollisionGround(uint8_t* base, uint32_t result, uint32_t point, uint32_t position,
                              uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FE8F8 TrackCollision::Ground");
  const uint32_t f = frame;
  if (position == 0) {
    guest::StoreU32(base, result + 52, 0);
    vl::Vec4Copy(base, result + 16, kUp);
    vl::Vec4Copy(base, result + 32, kUp);
    CopyWord(base, point + 4, result + 0);
    guest::StoreU32(base, result + 48, 0x04000000);
    return 1;
  }
  // The point copied to the frame, 0.5 higher.
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 80 + i, guest::LoadU32(base, point + i));
  }
  const bool same_x = Lfs(base, position + 16) == Lfs(base, point + 0);
  Stfs(base, f + 84, Fadds(Lfs(base, f + 84), Lfs(base, kHalf)));
  uint32_t kind;
  if (same_x && Lfs(base, position + 24) == Lfs(base, point + 8)) {
    CopyWord(base, position + 20, f + 84);
    kind = S16(base, position + 80);
  } else {
    kind = TrackMeshGroundHeight(base, kMesh, f + 80, position, 0, position + 48, f - 240);
    CopyWord(base, f + 80, position + 16);
    CopyWord(base, f + 84, position + 20);
    CopyWord(base, f + 88, position + 24);
  }
  if (int32_t(kind) > -1) {
    guest::StoreU16(base, position + 80, uint16_t(kind));
    guest::StoreU32(base, result + 52, position);
    CopyWord(base, position + 48, result + 16);
    CopyWord(base, position + 52, result + 20);
    CopyWord(base, position + 56, result + 24);
    vl::Vec4Copy(base, result + 32, result + 16);
    guest::StoreU32(base, result + 48, kind);
    double height = Lfs(base, f + 84);
    if (int32_t(kind) == 4 || int32_t(kind) == 20) {
      height = Fadds(height, Lfs(base, kKindHeightOffset));
    }
    Stfs(base, result + 0, Fsubs(Lfs(base, point + 4), height));
  } else {
    guest::StoreU32(base, result + 52, position);
    vl::Vec4Copy(base, result + 16, kUp);
    vl::Vec4Copy(base, result + 32, kUp);
    CopyWord(base, kZero, result + 0);
    guest::StoreU32(base, result + 48, 0x04000000);
    CopyWord(base, point + 0, position + 16);
    CopyWord(base, point + 4, position + 20);
    CopyWord(base, point + 8, position + 24);
  }
  return 1;
}

uint32_t TrackCollisionGroundCallback(uint8_t* base, uint32_t result, uint32_t point,
                                      uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FEAB0 TrackCollision::GroundCallback");
  TrackCollisionGround(base, result, point, position, frame - 144);
  guest::StoreU32(base, result + 48, guest::LoadU32(base, result + 48) & 0x80FFFFFF);
  return 1;
}

bool TrackCollisionGroundHits(uint8_t* base, uint32_t hits, uint32_t box, uint32_t frame,
                              uint32_t* count) {
  if (!co::IsBox(base, box)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820FEBD0 TrackCollision::GroundHits");
  const uint32_t f = frame;
  const int32_t room = int32_t(va::CollisionWorldWord60(base));
  *count = 0;
  const double zero = Lfs(base, kZero);
  Stfs(base, hits + 48, zero);
  // The lower corners in the order 0, 2, 1, 3; the lowest against its
  // track position's last height (the cache word of its place in that
  // order).
  guest::StoreU32(base, f + 80, 0);
  guest::StoreU32(base, f + 84, 2);
  guest::StoreU32(base, f + 88, 1);
  guest::StoreU32(base, f + 92, 3);
  uint32_t lowest = 0;
  double below;
  {
    const uint32_t corner = co::BoxLowerCorner(base, box, 0);
    const uint32_t position = co::BoxCacheWord(base, box, 0);
    below = Fsubs(Lfs(base, corner + 4), Lfs(base, position + 20));
  }
  for (uint32_t k = 1; k < 4; k++) {
    const uint32_t corner = co::BoxLowerCorner(base, box, guest::LoadU32(base, f + 80 + 4 * k));
    const uint32_t position = co::BoxCacheWord(base, box, k);
    const double d = Fsubs(Lfs(base, corner + 4), Lfs(base, position + 20));
    if (below > d) {
      below = d;
      lowest = k;
    }
  }
  Stfs(base, f + 252, zero);
  Stfs(base, f + 268, zero);
  const uint32_t corner =
      co::BoxLowerCorner(base, box, guest::LoadU32(base, lowest * 4 + f + 80));
  co::BoxCacheWord(base, box, lowest);
  if (!(below < zero)) {
    return true;
  }
  const uint32_t position = co::BoxCacheWord(base, box, lowest);
  TrackCollisionGround(base, f + 224, corner, position, f - 144);
  const double depth = Fneg(Lfs(base, f + 224));
  if (!(depth > zero)) {
    return true;
  }
  guest::StoreU32(base, hits + 60, 0);
  vl::Vec4Copy(base, hits + 16, f + 240);
  vl::Vec4Copy(base, hits + 32, f + 256);
  Stfs(base, hits + 48, Fsubs(Lfs(base, kEpsilon), Lfs(base, f + 224)));
  vl::Vec4Copy(base, hits + 0, corner);
  guest::StoreU32(base, hits + 56, guest::LoadU32(base, f + 272));
  if (room > 0) {
    *count = 1;
  }
  return true;
}

}  // namespace track_collision
