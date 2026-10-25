// The wall query's parts: the cells near a box, the hit records and the
// segments' crossing (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "collision/collision.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace track_collision {

using namespace car_physics::ops;
namespace co = collision;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kQuarter = 0x82002968;
constexpr uint32_t kDepthScale = 0x820029DC;

uint32_t S16(uint8_t* base, uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(base, address))));
}

}  // namespace

uint32_t WallQueryAddCell(uint8_t* base, uint32_t query, uint32_t cell, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FC110 WallQuery::AddCell");
  guest::StoreU32(base, frame + 28, cell);
  const int32_t count = int32_t(guest::LoadU32(base, query + 108));
  if (count > 0) {
    const int32_t section = int32_t(S16(base, frame + 28));
    uint32_t at = query + 112;
    for (int32_t i = 0; i < int32_t(guest::LoadU32(base, query + 108)); i++, at += 4) {
      if (int32_t(S16(base, at + 0)) == section &&
          int32_t(S16(base, at + 2)) == int32_t(S16(base, frame + 30))) {
        return 0;
      }
    }
  }
  guest::StoreU32(base, (uint32_t(count) + 28) * 4 + query, cell);
  guest::StoreU32(base, query + 108, guest::LoadU32(base, query + 108) + 1);
  return 1;
}

bool WallQueryGatherCells(uint8_t* base, uint32_t query, uint32_t mesh, uint32_t frame,
                          uint32_t* count) {
  ITC_NATIVE_RUNNING("sub_820FC188 WallQuery::GatherCells");
  uint32_t box = 0;
  if (!co::CollisionShapeCurrent(base, guest::LoadU32(base, query + 12), &box) ||
      !co::IsBox(base, box)) {
    return false;
  }
  const uint32_t f = frame;
  // Half the corner count (srawi, addze): the lower corners.
  const uint32_t corners = uint32_t(int32_t(co::BoxCornerCount()) / 2);
  guest::StoreU32(base, query + 108, 0);
  for (uint32_t i = 0; i < corners; i++) {
    const uint32_t position = co::BoxCacheWord(base, box, i);
    if (WallQueryAddCell(base, query, guest::LoadU32(base, position + 0), f) == 0) {
      continue;
    }
    // The slices before and after it, and the cells the section's links
    // lead to from it or them.
    guest::StoreU32(base, f + 80, guest::LoadU32(base, position + 0));
    guest::StoreU32(base, f + 84, guest::LoadU32(base, position + 0));
    const uint32_t table = guest::LoadU32(base, guest::LoadU32(base, mesh + 4) + 4);
    const uint32_t section = guest::LoadU32(base, S16(base, position + 0) * 4 + table);
    const int32_t after = int16_t(guest::LoadU16(base, f + 82) + 1);
    guest::StoreU16(base, f + 82, uint16_t(after));
    const int32_t before = int16_t(guest::LoadU16(base, f + 86) - 1);
    guest::StoreU16(base, f + 86, uint16_t(before));
    if (after < int32_t(S16(base, section + 0))) {
      WallQueryAddCell(base, query, guest::LoadU32(base, f + 80), f);
    }
    if (before >= 0) {
      WallQueryAddCell(base, query, guest::LoadU32(base, f + 84), f);
    }
    for (uint32_t k = 0; k < 6; k++) {
      const uint32_t link = section + 6 + 6 * k;
      const int32_t from = int32_t(S16(base, link - 2));
      if (from == -1) {
        continue;
      }
      if (from == int32_t(S16(base, position + 2)) || from == after || from == before) {
        guest::StoreU16(base, f + 88, guest::LoadU16(base, link + 0));
        guest::StoreU16(base, f + 90, guest::LoadU16(base, link + 2));
        WallQueryAddCell(base, query, guest::LoadU32(base, f + 88), f);
      }
    }
  }
  *count = guest::LoadU32(base, query + 108);
  return true;
}

uint32_t WallQueryAddHit(uint8_t* base, uint32_t query, uint32_t surface) {
  ITC_NATIVE_RUNNING("sub_820FC2E0 WallQuery::AddHit");
  const uint32_t room = va::CollisionWorldWord60(base);
  const uint32_t count = guest::LoadU32(base, query + 0);
  const uint32_t index = int32_t(room) > int32_t(count) ? count : room - 1;
  const uint32_t record = index * 80 + guest::LoadU32(base, query + 8);
  if (int32_t(room) > int32_t(count)) {
    guest::StoreU32(base, query + 0, count + 1);
  }
  guest::StoreU32(base, record + 60, guest::LoadU32(base, query + 104));
  CopyWord(base, query + 4, record + 64);
  const double scale = Lfs(base, kDepthScale);
  Stfs(base, record + 48, Fmuls(Lfs(base, query + 96), scale));
  const double depth2 = Lfs(base, query + 100);
  guest::StoreU32(base, record + 56, surface);
  Stfs(base, record + 52, Fmuls(depth2, scale));
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyWord(base, query + 48 + i, record + 0 + i);
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyWord(base, query + 64 + i, record + 16 + i);
  }
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyWord(base, query + 80 + i, record + 32 + i);
  }
  return room;
}

uint32_t SegmentsIntersect2D(uint8_t* base, uint32_t points, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_820FC658 Segments::Intersect2D");
  const uint32_t a = guest::LoadU32(base, points + 0), b = guest::LoadU32(base, points + 4);
  const uint32_t c = guest::LoadU32(base, points + 8), d = guest::LoadU32(base, points + 12);
  // The bounds in x, then in z: a-b wholly on one side of c-d.
  auto apart = [&](uint32_t axis) {
    const double pa = Lfs(base, a + axis), pb = Lfs(base, b + axis), pc = Lfs(base, c + axis);
    if (pa < pb) {
      if (pa > pc && pa > Lfs(base, d + axis)) {
        return true;
      }
      return pb < pc && pb < Lfs(base, d + axis);
    }
    if (pa < pc && pa < Lfs(base, d + axis)) {
      return true;
    }
    return pb > pc && pb > Lfs(base, d + axis);
  };
  if (apart(0) || apart(8)) {
    return 0;
  }
  double ax = Lfs(base, a + 0), bx = Lfs(base, b + 0), cx = Lfs(base, c + 0);
  double az = Lfs(base, a + 8), bz = Lfs(base, b + 8), cz = Lfs(base, c + 8);
  double dx = Lfs(base, d + 0), dz = Lfs(base, d + 8);
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
    if (Fmuls(side_d, side_c) > Lfs(base, kZero)) {
      return 0;
    }
  }
  const double zero = Lfs(base, kZero);
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
  const double quarter = Lfs(base, kQuarter);
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
  const double inverse = Fdivs(Lfs(base, kOne), denominator);
  const double w = Fmuls(cross_cd, baz);
  const double nx = Fmsubs(cross_cd, abx, v);
  const double nz = Fmsubs(dcz, cross_ab, w);
  Stfs(base, out + 0, Fmadds(nx, inverse, mx));
  Stfs(base, out + 8, Fmadds(nz, inverse, mz));
  return 1;
}

}  // namespace track_collision
