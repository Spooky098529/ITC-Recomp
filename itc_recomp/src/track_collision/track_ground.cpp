// The ground under a point: the mesh's triangles, their planes and colors,
// and the ground queries (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "collision/collision.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

namespace track_collision {

using namespace car_physics::ops;
namespace vl = vector_library;
namespace co = collision;
namespace va = vehicle_accessors;

namespace {

using Fpscr = ::base::FloatMode;

using G = schema::CollisionWorldGlobals;
using Point = schema::TrackQueryPoint;
using Section = schema::TrackMeshSection;
using Triangle = schema::TrackMeshTriangle;
using Position = schema::TrackPosition;
using Result = schema::TrackGroundResult;

// Section `index` of the mesh's file (the table at [[mesh + 4] + 4]).
uint32_t SectionOf(uint32_t mesh, uint32_t index) {
  return guest::LoadU32(index * 4 + schema::TrackMesh(mesh).file().sections());
}

// Vertex `index` of the section (12 bytes each from its +64).
uint32_t Vertex(uint32_t section, uint32_t index) {
  return index * 12 + Section(section).vertices() + section;
}

}  // namespace

// See track_collision.h (shared with the wall query).
void NormalizeVector(uint32_t from, uint32_t to) {
  const uint32_t in = from & ~0xFu;
  simde__m128 v = simde_mm_castsi128_ps(
      simde_mm_set_epi32(int(guest::LoadU32(in + 0)), int(guest::LoadU32(in + 4)),
                         int(guest::LoadU32(in + 8)), int(guest::LoadU32(in + 12))));
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
  guest::StoreU32(out + 0, lanes[3]);
  guest::StoreU32(out + 4, lanes[2]);
  guest::StoreU32(out + 8, lanes[1]);
  guest::StoreU32(out + 12, lanes[0]);
}

namespace {

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

uint32_t TrackMeshTriangleInSlice(uint32_t section, uint32_t slice, uint32_t point,
                                  uint32_t skip_kinds) {
  ITC_NATIVE_RUNNING("sub_820F72F8 TrackMesh::TriangleInSlice");
  const Section sec(section);
  const schema::TrackMeshSlice slice_at(sec.slices() + slice * 28 + section);
  const uint32_t vertices = sec.vertices() + section;
  const uint32_t first = slice_at.first_triangle();
  const int32_t count = int32_t(slice_at.triangle_count());
  uint32_t triangle = first * 12 + sec.triangles() + section;
  if (count <= 0) {
    return uint32_t(-1);
  }
  const double epsilon = double(G::inside_epsilon());
  for (int32_t i = 0; i < count; i++, triangle += 12) {
    const Triangle t(triangle);
    if (int32_t(skip_kinds) == 1) {
      int32_t kind = t.surface_kind();
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
    const uint32_t v0 = t.vertex_0() * 12 + vertices;
    const double pz = double(Point(point).z());
    const uint32_t v1 = t.vertex_1() * 12 + vertices;
    const double px = double(Point(point).x());
    const double f7 = Fsubs(double(Point(v0).x()), px);
    const double f8 = Fsubs(double(Point(v1).z()), pz);
    const double f6 = Fsubs(double(Point(v0).z()), pz);
    const double f9 = Fsubs(double(Point(v1).x()), px);
    if (Fmsubs(f9, f6, Fmuls(f8, f7)) > epsilon) {
      continue;
    }
    const uint32_t v2 = t.vertex_2() * 12 + vertices;
    const double pz2 = double(Point(point).z());
    const double f11 = Fsubs(double(Point(v2).z()), pz2);
    const double f12 = Fsubs(double(Point(v2).x()), px);
    if (Fmsubs(f12, f8, Fmuls(f11, f9)) > epsilon) {
      continue;
    }
    double last;
    if (t.vertex_3() == 0xFFFF) {
      last = Fmsubs(f11, f7, Fmuls(f12, f6));
    } else {
      const uint32_t v3 = t.vertex_3() * 12 + vertices;
      const double f10 = Fsubs(double(Point(v3).z()), pz2);
      const double f0 = Fsubs(double(Point(v3).x()), px);
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

void TrackMeshTriangleNormal(uint32_t out, uint32_t section, uint32_t triangle,
                             uint32_t a, uint32_t b, uint32_t c) {
  ITC_NATIVE_RUNNING("sub_820F7468 TrackMesh::TriangleNormal");
  const double one = Lfs(kOne);
  // The original's names: A the vertex `b`, B `a`, C `c`.
  const uint32_t vertex_0 = Triangle(triangle).vertex_0_address();
  const uint32_t va = Vertex(section, guest::LoadU16(vertex_0 + 2 * b));
  const uint32_t vb = Vertex(section, guest::LoadU16(vertex_0 + 2 * a));
  const uint32_t vc = Vertex(section, guest::LoadU16(vertex_0 + 2 * c));
  {
    const double ay = double(Point(va).y()), az = double(Point(va).z());
    const double t = Fsubs(ay, double(Point(vb).y()));
    const double u = Fsubs(double(Point(vc).z()), az);
    const double cy = Fsubs(double(Point(vc).y()), ay);
    const double d = Fsubs(az, double(Point(vb).z()));
    Point(out).set_x(float(Fmsubs(cy, d, Fmuls(t, u))));
  }
  {
    const double ax = double(Point(va).x()), az = double(Point(va).z());
    const double t = Fsubs(double(Point(vc).x()), ax);
    const double u = Fsubs(az, double(Point(vb).z()));
    const double cz = Fsubs(double(Point(vc).z()), az);
    const double d = Fsubs(ax, double(Point(vb).x()));
    Point(out).set_y(float(Fmsubs(cz, d, Fmuls(t, u))));
  }
  {
    const double ay = double(Point(va).y()), ax = double(Point(va).x());
    const double t = Fsubs(double(Point(vc).y()), ay);
    const double u = Fsubs(ax, double(Point(vb).x()));
    const double cx = Fsubs(double(Point(vc).x()), ax);
    const double d = Fsubs(ay, double(Point(vb).y()));
    Point(out).set_z(float(Fmsubs(cx, d, Fmuls(t, u))));
  }
  Point(out).set_w(float(one));
  NormalizeVector(out, out);
  Point(out).set_w(float(one));
}

double TrackMeshQuadHeight(uint32_t out, uint32_t section, uint32_t quad,
                           uint32_t point, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F7590 TrackMesh::QuadHeight");
  const uint32_t f = frame;
  const double pz = double(Point(point).z());
  const double px = double(Point(point).x());
  const double one = Lfs(kOne);
  Stfs(f + 92, one);
  Stfs(f + 108, one);
  const Triangle q(quad);
  const uint32_t v0 = Vertex(section, q.vertex_0());
  const uint32_t v2 = Vertex(section, q.vertex_2());
  const uint32_t v1 = Vertex(section, q.vertex_1());
  const uint32_t v3 = Vertex(section, q.vertex_3());
  const double d0z = Fsubs(double(Point(v0).z()), pz);
  const double d0x = Fsubs(double(Point(v0).x()), px);
  const double d2z = Fsubs(double(Point(v2).z()), pz);
  const double d2x = Fsubs(double(Point(v2).x()), px);
  Stfs(f + 148, d2z);
  Stfs(f + 112, d0x);
  Stfs(f + 116, d0z);
  Stfs(f + 144, d2x);
  const double side = Fmsubs(d2x, d0z, Fmuls(d2z, d0x));
  const double d1x = Fsubs(double(Point(v1).x()), px);
  const double d3x = Fsubs(double(Point(v3).x()), px);
  const double d3z = Fsubs(double(Point(v3).z()), pz);
  const double d1z = Fsubs(double(Point(v1).z()), pz);
  Stfs(f + 128, d1x);
  Stfs(f + 132, d1z);
  Stfs(f + 160, d3x);
  const double zero = Lfs(kZero);
  Stfs(f + 164, d3z);
  // The half the point is on: vertices 0, 1, 2 or 0, 2, 3.
  if (side > zero) {
    TrackMeshTriangleNormal(f + 80, section, quad, 0, 1, 2);
  } else {
    TrackMeshTriangleNormal(f + 80, section, quad, 0, 2, 3);
  }
  const double t = Fmuls(Lfs(f + 80), d0x);
  const double u = Fmuls(d3z, d1x);
  const double first_height =
      Fadds(Fdivs(Fmadds(Lfs(f + 88), d0z, t), Lfs(f + 84)),
            double(Point(v0).y()));
  const double other_side = Fmsubs(d3x, d1z, u);
  double other_y;
  uint32_t other;
  if (other_side > zero) {
    TrackMeshTriangleNormal(f + 96, section, quad, 1, 2, 3);
    other_y = double(Point(v1).y());
    other = f + 128;
  } else {
    TrackMeshTriangleNormal(f + 96, section, quad, 0, 1, 3);
    other_y = double(Point(v0).y());
    other = f + 112;
  }
  const double n2x = Lfs(f + 96);
  const double sum_x = Fadds(n2x, Lfs(f + 80));
  const double n2y = Lfs(f + 100);
  const double sum_y = Fadds(n2y, Lfs(f + 84));
  const double n2z = Lfs(f + 104);
  const double t2 = Fmuls(Lfs(other + 0), n2x);
  const double other_z = Lfs(other + 4);
  const double sum_z = Fadds(n2z, Lfs(f + 88));
  const double half = double(G::half());
  Point(out).set_x(float(Fmuls(sum_x, half)));
  Point(out).set_y(float(Fmuls(sum_y, half)));
  const double plane = Fmadds(other_z, n2z, t2);
  Point(out).set_z(float(Fmuls(sum_z, half)));
  const double second_height = Fadds(Fdivs(plane, n2y), other_y);
  const double result = Fmuls(Fadds(second_height, first_height), half);
  NormalizeVector(out, out);
  return result;
}

uint32_t TrackMeshFindTriangle(uint32_t mesh, uint32_t section_out,
                               uint32_t triangle_out, uint32_t point, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F7818 TrackMesh::FindTriangle");
  const Position pos(position);
  const schema::TrackFoundSection out_section(section_out);
  uint32_t slice_found = pos.slice();
  const uint32_t section_index = uint32_t(int32_t(int16_t(pos.section())));
  if (int32_t(section_index) == -1 || int32_t(int16_t(pos.slice())) == -1) {
    return uint32_t(-1);
  }
  out_section.set_section(SectionOf(mesh, section_index));
  const uint32_t slice = uint32_t(int32_t(int16_t(pos.slice())));
  uint32_t found = TrackMeshTriangleInSlice(out_section.section().address, slice, point, 0);
  auto done = [&](uint32_t triangle) {
    const Section section = out_section.section();
    schema::TrackFoundTriangle(triangle_out)
        .set_triangle(triangle * 12 + section.triangles() + section.address);
    return slice_found;
  };
  if (int32_t(found) != -1) {
    return done(found);
  }
  // The slices offset from it in its section.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t offsets = G::slice_offsets_address() + 4 * i;
    const uint32_t s =
        uint32_t(int32_t(int16_t(guest::LoadU32(offsets) + slice)));
    if (int32_t(s) < 0 || int32_t(s) >= int32_t(int16_t(out_section.section().slice_count()))) {
      continue;
    }
    const uint32_t section = SectionOf(mesh, uint32_t(int32_t(int16_t(pos.section()))));
    found = TrackMeshTriangleInSlice(section, s, point, 0);
    if (int32_t(found) != -1) {
      slice_found = s;
      out_section.set_section(section);
      return done(found);
    }
  }
  // The cells the section's links lead to from this slice.
  {
    const uint32_t links = out_section.section().links_address();
    for (uint32_t i = 0; i < 6; i++) {
      const schema::TrackMeshLink link(links + 6 * i);
      const int32_t from = int32_t(int16_t(link.from_slice()));
      if (from == -1 || from != int32_t(int16_t(pos.slice()))) {
        continue;
      }
      const uint32_t target_index = uint32_t(int32_t(int16_t(link.section())));
      const uint32_t target_slice = link.slice();
      const uint32_t section = SectionOf(mesh, target_index);
      found = TrackMeshTriangleInSlice(section, uint32_t(int32_t(int16_t(target_slice))),
                                       point, 0);
      if (int32_t(found) != -1) {
        slice_found = target_slice;
        out_section.set_section(section);
        return done(found);
      }
    }
  }
  // The links leaving from within 2 slices of it: their cell and the
  // slices offset from it.
  for (uint32_t offset = 0; offset < 36; offset += 6) {
    const schema::TrackMeshLink link(out_section.section().links_address() + offset);
    const int32_t from = int32_t(int16_t(link.from_slice()));
    if (from == -1) {
      continue;
    }
    const int32_t here = int32_t(int16_t(pos.slice()));
    if (from < here - 2 || from > here + 2) {
      continue;
    }
    const int32_t target_index = int32_t(int16_t(link.section()));
    if (target_index < 0) {
      continue;
    }
    const uint32_t target_slice =
        schema::TrackMeshLink(out_section.section().links_address() + offset).slice();
    const uint32_t section = SectionOf(mesh, uint32_t(target_index));
    if (from != here) {
      found = TrackMeshTriangleInSlice(section, uint32_t(int32_t(int16_t(target_slice))),
                                       point, 0);
      if (int32_t(found) != -1) {
        slice_found = target_slice;
        out_section.set_section(section);
        return done(found);
      }
    }
    const uint32_t around = uint32_t(int32_t(int16_t(target_slice)));
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t offsets = G::slice_offsets_address() + 4 * i;
      const uint32_t s = uint32_t(int32_t(int16_t(guest::LoadU32(offsets) + around)));
      if (int32_t(s) < 0 || int32_t(s) >= int32_t(int16_t(Section(section).slice_count()))) {
        continue;
      }
      found = TrackMeshTriangleInSlice(section, s, point, 0);
      if (int32_t(found) != -1) {
        slice_found = s;
        out_section.set_section(section);
        return done(found);
      }
    }
  }
  return uint32_t(-1);
}

void TrackMeshColorAt(uint32_t out, uint32_t point, uint32_t triangle,
                      uint32_t vertices, uint32_t colors) {
  ITC_NATIVE_RUNNING("sub_820F6E18 TrackMesh::ColorAt");
  // The three vertices A, B, C (the original's order) and their colors.
  uint32_t a, b, c;
  bool first_half = true;
  const Triangle tri(triangle);
  if (tri.vertex_3() != 0xFFFF) {
    const uint32_t v2 = tri.vertex_2() * 12 + vertices;
    const double pz = double(Point(point).z());
    const uint32_t v0 = tri.vertex_0() * 12 + vertices;
    const double px = double(Point(point).x());
    const double v2z = double(Point(v2).z()), v2x = double(Point(v2).x());
    const double dz = Fsubs(pz, v2z);
    const double dx = Fsubs(px, v2x);
    const double ez = Fsubs(double(Point(v0).z()), v2z);
    const double ex = Fsubs(double(Point(v0).x()), v2x);
    const double side = Fmsubs(ex, dz, Fmuls(ez, dx));
    first_half = !(side < Lfs(kZero));
  }
  if (first_half) {
    a = tri.vertex_0();
    b = tri.vertex_1();
    c = tri.vertex_2();
  } else {
    a = tri.vertex_2();
    b = tri.vertex_3();
    c = tri.vertex_0();
  }
  const uint32_t ca = guest::LoadU16(colors + 2 * a);
  const uint32_t cb = guest::LoadU16(colors + 2 * b);
  const uint32_t cc = guest::LoadU16(colors + 2 * c);
  const uint32_t va = a * 12 + vertices, vb = b * 12 + vertices, vc = c * 12 + vertices;
  const double ax = double(Point(va).x()), az = double(Point(va).z());
  const double bx = double(Point(vb).x()), bz = double(Point(vb).z());
  const double cx = double(Point(vc).x()), cz = double(Point(vc).z());
  const double pz = double(Point(point).z());
  const double px = double(Point(point).x());
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
  const double numerator = double(G::minus_one());
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
  const double k = double(G::color_scale());
  Point(out).set_x(float(Fmuls(red, k)));
  Point(out).set_y(float(Fmuls(green, k)));
  Point(out).set_z(float(Fmuls(blue, k)));
}

uint32_t TrackMeshGroundHeight(uint32_t mesh, uint32_t point, uint32_t position,
                               uint32_t color, uint32_t normal, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F7AA0 TrackMesh::GroundHeight");
  const uint32_t f = frame;
  const double one = Lfs(kOne);
  Stfs(f + 108, one);
  guest::StoreU32(f + 80, 0);
  guest::StoreU32(f + 84, 0);
  bool other_found = true;
  const double zero = Lfs(kZero);
  if (color != 0) {
    Point(color).set_x(float(zero));
    Point(color).set_y(float(zero));
    Point(color).set_z(float(zero));
    Point(color).set_w(float(one));
  }
  if (normal != 0) {
    Point(normal).set_x(float(zero));
    Point(normal).set_z(float(zero));
    Point(normal).set_w(float(one));
    Point(normal).set_y(float(zero));
    Point(normal).set_y(float(one));
  }
  const uint32_t slice =
      uint32_t(int32_t(int16_t(TrackMeshFindTriangle(mesh, f + 80, f + 84, point, position))));
  if (int32_t(slice) == -1) {
    return uint32_t(-1);
  }
  uint32_t triangle = guest::LoadU32(f + 84);
  const uint32_t section = guest::LoadU32(f + 80);
  const Section sec(section);
  const uint32_t vertex_offset = sec.vertices();
  uint32_t v0 = Triangle(triangle).vertex_0() * 12 + vertex_offset + section;
  uint32_t kind = Triangle(triangle).surface_kind();
  if (int32_t(kind) >= 9) {
    kind = kind == 20 ? 4 : 0;
  }
  if (kind == 4 || kind == 20) {
    // Another triangle without those kinds in the slice or those offset
    // from it.
    uint32_t other = TrackMeshTriangleInSlice(section, slice, point, 1);
    for (uint32_t i = 0; int32_t(other) == -1 && i < 4; i++) {
      const uint32_t s = slice + guest::LoadU32(G::slice_offsets_address() + 4 * i);
      if (int32_t(s) < 0 || int32_t(s) >= int32_t(int16_t(sec.slice_count()))) {
        continue;
      }
      other = TrackMeshTriangleInSlice(section, s, point, 1);
    }
    if (int32_t(other) != -1) {
      triangle = other * 12 + sec.triangles() + section;
      v0 = Triangle(triangle).vertex_0() * 12 + vertex_offset + section;
    } else {
      other_found = false;
    }
  }
  double height;
  if (Triangle(triangle).vertex_3() != 0xFFFF &&
      (Triangle(triangle).flags() & 0x1) != 0) {
    height = TrackMeshQuadHeight(f + 96, section, triangle, point, f - 208);
  } else {
    TrackMeshTriangleNormal(f + 96, section, triangle, 0, 1, 2);
    const double inverse = Fdivs(one, Lfs(f + 100));
    const double dx = Fsubs(double(Point(v0).x()), double(Point(point).x()));
    const double nx = Lfs(f + 96);
    const double dz = Fsubs(double(Point(v0).z()), double(Point(point).z()));
    const double nz = Lfs(f + 104);
    const double y = double(Point(v0).y());
    const double sx = Fmuls(inverse, nx);
    const double sz = Fmuls(inverse, nz);
    height = Fadds(Fmadds(dz, sz, Fmuls(sx, dx)), y);
  }
  height = Fadds(height, double(G::height_offset()));
  if (!other_found) {
    height = Fsubs(height, double(G::no_other_lowering()));
  }
  const double off = Fabs(Fsubs(double(Point(point).y()), height));
  if (!(off < double(G::catch_distance()))) {
    return uint32_t(-2);
  }
  Point(point).set_y(float(height));
  if (color != 0) {
    TrackMeshColorAt(color, point, triangle, sec.vertices() + section,
                     sec.colors() + section);
  }
  if (normal != 0) {
    Stfs(f + 108, one);
    NormalizeVector(f + 96, normal);
  }
  return kind;
}

uint32_t TrackCollisionGround(uint32_t result, uint32_t point, uint32_t position,
                              uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FE8F8 TrackCollision::Ground");
  const uint32_t f = frame;
  const Result res(result);
  const Point pt(point);
  const Position pos(position);
  if (position == 0) {
    res.set_position(0);
    vl::Vec4Copy(res.normal_x_address(), G::up_address());
    vl::Vec4Copy(res.normal_copy_x_address(), G::up_address());
    CopyWord(pt.y_address(), res.depth_address());
    res.set_surface(0x04000000);
    return 1;
  }
  // The point copied to the frame, 0.5 higher.
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(f + 80 + i, guest::LoadU32(point + i));
  }
  const bool same_x = double(pos.point_x()) == double(pt.x());
  Stfs(f + 84, Fadds(Lfs(f + 84), double(G::half())));
  uint32_t kind;
  if (same_x && double(pos.point_z()) == double(pt.z())) {
    CopyWord(pos.point_y_address(), f + 84);
    kind = uint32_t(int32_t(int16_t(pos.surface_kind())));
  } else {
    kind = TrackMeshGroundHeight(kMesh, f + 80, position, 0, pos.normal_x_address(), f - 240);
    CopyWord(f + 80, pos.point_x_address());
    CopyWord(f + 84, pos.point_y_address());
    CopyWord(f + 88, pos.point_z_address());
  }
  if (int32_t(kind) > -1) {
    pos.set_surface_kind(uint16_t(kind));
    res.set_position(position);
    CopyWord(pos.normal_x_address(), res.normal_x_address());
    CopyWord(pos.normal_y_address(), res.normal_y_address());
    CopyWord(pos.normal_z_address(), res.normal_z_address());
    vl::Vec4Copy(res.normal_copy_x_address(), res.normal_x_address());
    res.set_surface(kind);
    double height = Lfs(f + 84);
    if (int32_t(kind) == 4 || int32_t(kind) == 20) {
      height = Fadds(height, double(G::height_offset()));
    }
    res.set_depth(float(Fsubs(double(pt.y()), height)));
  } else {
    res.set_position(position);
    vl::Vec4Copy(res.normal_x_address(), G::up_address());
    vl::Vec4Copy(res.normal_copy_x_address(), G::up_address());
    CopyWord(kZero, res.depth_address());
    res.set_surface(0x04000000);
    CopyWord(pt.x_address(), pos.point_x_address());
    CopyWord(pt.y_address(), pos.point_y_address());
    CopyWord(pt.z_address(), pos.point_z_address());
  }
  return 1;
}

uint32_t TrackCollisionGroundCallback(uint32_t result, uint32_t point,
                                      uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FEAB0 TrackCollision::GroundCallback");
  TrackCollisionGround(result, point, position, frame - 144);
  const Result res(result);
  res.set_surface(res.surface() & 0x80FFFFFF);
  return 1;
}

bool TrackCollisionGroundHits(uint32_t hits, uint32_t box, uint32_t frame,
                              uint32_t* count) {
  if (!co::IsBox(box)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820FEBD0 TrackCollision::GroundHits");
  const uint32_t f = frame;
  const int32_t room = int32_t(va::CollisionWorldWord60());
  *count = 0;
  const schema::CollisionHit hit(hits);
  const double zero = Lfs(kZero);
  Stfs(hit.depth_address(), zero);
  // The lower corners in the order 0, 2, 1, 3; the lowest against its
  // track position's last height (the cache word of its place in that
  // order).
  guest::StoreU32(f + 80, 0);
  guest::StoreU32(f + 84, 2);
  guest::StoreU32(f + 88, 1);
  guest::StoreU32(f + 92, 3);
  uint32_t lowest = 0;
  double below;
  {
    const uint32_t corner = co::BoxLowerCorner(box, 0);
    const uint32_t position = co::BoxCacheWord(box, 0);
    below = Fsubs(double(Point(corner).y()), double(Position(position).point_y()));
  }
  for (uint32_t k = 1; k < 4; k++) {
    const uint32_t corner = co::BoxLowerCorner(box, guest::LoadU32(f + 80 + 4 * k));
    const uint32_t position = co::BoxCacheWord(box, k);
    const double d =
        Fsubs(double(Point(corner).y()), double(Position(position).point_y()));
    if (below > d) {
      below = d;
      lowest = k;
    }
  }
  Stfs(f + 252, zero);
  Stfs(f + 268, zero);
  const uint32_t corner =
      co::BoxLowerCorner(box, guest::LoadU32(lowest * 4 + f + 80));
  co::BoxCacheWord(box, lowest);
  if (!(below < zero)) {
    return true;
  }
  const uint32_t position = co::BoxCacheWord(box, lowest);
  TrackCollisionGround(f + 224, corner, position, f - 144);
  const double depth = Fneg(Lfs(f + 224));
  if (!(depth > zero)) {
    return true;
  }
  hit.set_key(0);
  vl::Vec4Copy(hit.normal_address(), f + 240);
  vl::Vec4Copy(hit.push_address(), f + 256);
  Stfs(hit.depth_address(), Fsubs(double(G::small_epsilon()), Lfs(f + 224)));
  vl::Vec4Copy(hit.point_address(), corner);
  hit.set_surface_bits(guest::LoadU32(f + 272));
  if (room > 0) {
    *count = 1;
  }
  return true;
}

}  // namespace track_collision
