// The wall query's parts: the cells near a box, the hit records and the
// segments' crossing (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "collision/collision.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace track_collision {

using namespace car_physics::ops;
namespace co = collision;
namespace va = vehicle_accessors;

namespace {

using G = schema::CollisionWorldGlobals;
using Query = schema::WallQuery;
using Point = schema::TrackQueryPoint;

uint32_t S16(uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(address))));
}

}  // namespace

uint32_t WallQueryAddCell(uint32_t query, uint32_t cell, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FC110 WallQuery::AddCell");
  const Query q(query);
  guest::StoreU32(frame + 28, cell);
  const int32_t count = int32_t(q.cell_count());
  if (count > 0) {
    const int32_t section = int32_t(S16(frame + 28));
    uint32_t at = q.cells_address();
    for (int32_t i = 0; i < int32_t(q.cell_count()); i++, at += 4) {
      if (int32_t(S16(at + 0)) == section &&
          int32_t(S16(at + 2)) == int32_t(S16(frame + 30))) {
        return 0;
      }
    }
  }
  guest::StoreU32(q.cells_address() + uint32_t(count) * 4, cell);
  q.set_cell_count(q.cell_count() + 1);
  return 1;
}

bool WallQueryGatherCells(uint32_t query, uint32_t mesh, uint32_t frame,
                          uint32_t* count) {
  ITC_NATIVE_RUNNING("sub_820FC188 WallQuery::GatherCells");
  const Query q(query);
  uint32_t box = 0;
  if (!co::CollisionShapeCurrent(q.record().address, &box) ||
      !co::IsBox(box)) {
    return false;
  }
  const uint32_t f = frame;
  // Half the corner count (srawi, addze): the lower corners.
  const uint32_t corners = uint32_t(int32_t(co::BoxCornerCount()) / 2);
  q.set_cell_count(0);
  for (uint32_t i = 0; i < corners; i++) {
    const schema::TrackPosition position(co::BoxCacheWord(box, i));
    if (WallQueryAddCell(query, position.cell(), f) == 0) {
      continue;
    }
    // The slices before and after it, and the cells the section's links
    // lead to from it or them.
    guest::StoreU32(f + 80, position.cell());
    guest::StoreU32(f + 84, position.cell());
    const uint32_t table = schema::TrackMesh(mesh).file().sections();
    const schema::TrackMeshSection section(guest::LoadU32(uint32_t(int32_t(int16_t(position.section()))) * 4 + table));
    const int32_t after = int16_t(guest::LoadU16(f + 82) + 1);
    guest::StoreU16(f + 82, uint16_t(after));
    const int32_t before = int16_t(guest::LoadU16(f + 86) - 1);
    guest::StoreU16(f + 86, uint16_t(before));
    if (after < int32_t(int16_t(section.slice_count()))) {
      WallQueryAddCell(query, guest::LoadU32(f + 80), f);
    }
    if (before >= 0) {
      WallQueryAddCell(query, guest::LoadU32(f + 84), f);
    }
    for (uint32_t k = 0; k < 6; k++) {
      const schema::TrackMeshLink link(section.links_address() + 6 * k);
      const int32_t from = int32_t(int16_t(link.from_slice()));
      if (from == -1) {
        continue;
      }
      if (from == int32_t(int16_t(position.slice())) || from == after || from == before) {
        guest::StoreU16(f + 88, link.section());
        guest::StoreU16(f + 90, link.slice());
        WallQueryAddCell(query, guest::LoadU32(f + 88), f);
      }
    }
  }
  *count = q.cell_count();
  return true;
}

uint32_t WallQueryAddHit(uint32_t query, uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_820FC2E0 WallQuery::AddHit");
  const Query q(query);
  const uint32_t room = va::CollisionWorldWord60();
  const uint32_t count = q.hit_count();
  const uint32_t index = int32_t(room) > int32_t(count) ? count : room - 1;
  const schema::CollisionHit record(index * 80 + q.hits().address);
  if (int32_t(room) > int32_t(count)) {
    q.set_hit_count(count + 1);
  }
  record.set_key(q.hit_wall());
  CopyWord(q.time_address(), record.time_address());
  const double scale = double(G::hit_depth_scale());
  Stfs(record.depth_address(), Fmuls(double(q.hit_depth()), scale));
  const double depth2 = double(q.hit_depth_again());
  record.set_surface_bits(surface);
  Stfs(record.depth_2_address(), Fmuls(depth2, scale));
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyWord(q.hit_point_x_address() + i, record.point_address() + i);
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyWord(q.hit_normal_x_address() + i, record.normal_address() + i);
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyWord(q.hit_push_x_address() + i, record.push_address() + i);
  }
  return room;
}

uint32_t SegmentsIntersect2D(uint32_t points, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_820FC658 Segments::Intersect2D");
  const schema::TrackSegments segments(points);
  const Point a = segments.a(), b = segments.b();
  const Point c = segments.c(), d = segments.d();
  // The bounds in x, then in z: a-b wholly on one side of c-d.
  auto apart = [&](bool z) {
    auto at = [&](const Point& p) { return double(z ? p.z() : p.x()); };
    const double pa = at(a), pb = at(b), pc = at(c);
    if (pa < pb) {
      if (pa > pc && pa > at(d)) {
        return true;
      }
      return pb < pc && pb < at(d);
    }
    if (pa < pc && pa < at(d)) {
      return true;
    }
    return pb > pc && pb > at(d);
  };
  if (apart(false) || apart(true)) {
    return 0;
  }
  double ax = double(a.x()), bx = double(b.x()), cx = double(c.x());
  double az = double(a.z()), bz = double(b.z()), cz = double(c.z());
  double dx = double(d.x()), dz = double(d.z());
  // The sides: c and d on one side of a-b, a and b on one side of c-d.
  {
    const double abz = Fsubs(az, bz);
    const double acx = Fsubs(ax, cx);
    const double adx = Fsubs(ax, dx);
    const double abx = Fsubs(ax, bx);
    const double caz = Fsubs(cz, az);
    const double daz = Fsubs(dz, az);
    const double t = Fmuls(acx, abz);
    const double u = Fmuls(adx, abz);
    const double side_c = Fmadds(caz, abx, t);
    const double side_d = Fmadds(daz, abx, u);
    if (Fmuls(side_d, side_c) > Lfs(kZero)) {
      return 0;
    }
  }
  const double zero = Lfs(kZero);
  {
    const double cdz = Fsubs(cz, dz);
    const double cdx = Fsubs(cx, dx);
    const double t = Fmuls(Fsubs(cx, bx), cdz);
    const double u = Fmuls(Fsubs(cx, ax), cdz);
    const double side_b = Fmadds(Fsubs(bz, cz), cdx, t);
    const double side_a = Fmadds(Fsubs(az, cz), cdx, u);
    if (Fmuls(side_b, side_a) > zero) {
      return 0;
    }
  }
  // Worked out around the four points' mean.
  const double quarter = double(G::quarter());
  const double mx = Fmuls(Fadds(Fadds(Fadds(dx, cx), bx), ax), quarter);
  const double mz = Fmuls(Fadds(Fadds(Fadds(dz, cz), bz), az), quarter);
  ax = Fsubs(ax, mx);
  bz = Fsubs(bz, mz);
  bx = Fsubs(bx, mx);
  cx = Fsubs(cx, mx);
  dx = Fsubs(dx, mx);
  az = Fsubs(az, mz);
  dz = Fsubs(dz, mz);
  cz = Fsubs(cz, mz);
  const double t = Fmuls(bz, ax);
  const double abx = Fsubs(ax, bx);
  const double cdx = Fsubs(cx, dx);
  const double baz = Fsubs(bz, az);
  const double u = Fmuls(dz, cx);
  const double dcz = Fsubs(dz, cz);
  const double cross_ab = Fmsubs(bx, az, t);
  const double cross_cd = Fmsubs(dx, cz, u);
  const double denominator = Fmsubs(cdx, baz, Fmuls(dcz, abx));
  if (denominator == zero) {
    return 0;
  }
  const double v = Fmuls(cdx, cross_ab);
  const double inverse = Fdivs(Lfs(kOne), denominator);
  const double w = Fmuls(cross_cd, baz);
  const double nx = Fmsubs(cross_cd, abx, v);
  const double nz = Fmsubs(dcz, cross_ab, w);
  const Point o(out);
  o.set_x(float(Fmadds(nx, inverse, mx)));
  o.set_z(float(Fmadds(nz, inverse, mz)));
  return 1;
}

}  // namespace track_collision
