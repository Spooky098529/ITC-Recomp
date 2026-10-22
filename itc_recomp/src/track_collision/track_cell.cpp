// The track position's cell: which slice holds a point
// (docs/functions/collision-world.md): see track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"

#include "base/log.h"

namespace track_collision {

using namespace car_physics::ops;

namespace {

constexpr uint32_t kHeightTolerance = 0x82001BDC;  // the point's y within this of a slice's
constexpr uint32_t kCenterReach = 0x82002A28;      // each axis from the outline's center
constexpr uint32_t kCenterReachSquared = 0x8200681C;
constexpr uint32_t kInsideEpsilon = 0x820029D8;
constexpr uint32_t kBoundsMargin = 0x82001FD8;
constexpr uint32_t kFarthest = 0x82006824;  // SearchAll's first best distance
constexpr uint32_t kLinkedTried = 0x826C5B04;

uint32_t S16(uint8_t* base, uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(base, address))));
}
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(value)));
}
// The mesh file's section table ([[mesh + 4] + 4]).
uint32_t SectionTable(uint8_t* base, uint32_t mesh) {
  return guest::LoadU32(base, guest::LoadU32(base, mesh + 4) + 4);
}
uint32_t Slices(uint8_t* base, uint32_t section) {
  return guest::LoadU32(base, section + 72) + section;
}
uint32_t Outline(uint8_t* base, uint32_t section) {
  return guest::LoadU16(base, section + 2) + section;
}
// An outline's corner count: 3 when its +18 is 0xFFFF, else 4.
int32_t Corners(uint8_t* base, uint32_t slice) {
  return guest::LoadU16(base, slice + 18) == 0xFFFF ? 3 : 4;
}
// The point's y within the tolerance of the slice's height (a NaN passes,
// as in the original's two tests).
bool WithinHeight(uint8_t* base, uint32_t slice, double y, double tolerance) {
  const double height = Lfs(base, slice + 20);
  if (y > Fadds(height, tolerance)) {
    return false;
  }
  return !(y < Fsubs(height, tolerance));
}

}  // namespace

uint32_t TrackMeshSliceContains(uint8_t* base, uint32_t slice, uint32_t outline, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820F6A38 TrackMesh::SliceContains");
  if (!WithinHeight(base, slice, Lfs(base, point + 4), Lfs(base, kHeightTolerance))) {
    return 0;
  }
  const int32_t count = Corners(base, slice);
  double sum_x = Lfs(base, kZero), sum_z = sum_x;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t p = guest::LoadU16(base, slice + 12 + 2 * uint32_t(i)) * 8 + outline;
    sum_x = Fadds(Lfs(base, p + 0), sum_x);
    sum_z = Fadds(Lfs(base, p + 4), sum_z);
  }
  const double px = Lfs(base, point + 0), pz = Lfs(base, point + 8);
  const double inverse = Fdivs(Lfs(base, kOne), double(float(double(int64_t(count)))));
  const double dx = Fabs(Fmsubs(inverse, sum_x, px));
  const double dz = Fabs(Fmsubs(inverse, sum_z, pz));
  const double reach = Lfs(base, kCenterReach);
  if (dx > reach || dz > reach) {
    return 0;
  }
  if (Fmadds(dz, dz, Fmuls(dx, dx)) > Lfs(base, kCenterReachSquared)) {
    return 0;
  }
  const double epsilon = Lfs(base, kInsideEpsilon);
  for (int32_t i = 0; i < count; i++) {
    const double z = Lfs(base, point + 8);
    const uint32_t p = guest::LoadU16(base, slice + 12 + 2 * uint32_t(i)) * 8 + outline;
    const double ax = Fsubs(Lfs(base, p + 0), px);
    const double az = Fsubs(Lfs(base, p + 4), z);
    const uint32_t next = uint32_t((i + 1) % count);
    const uint32_t q = guest::LoadU16(base, slice + 12 + 2 * next) * 8 + outline;
    const double bz = Fsubs(Lfs(base, q + 4), z);
    const double bx = Fsubs(Lfs(base, q + 0), px);
    if (Fmsubs(bx, az, Fmuls(bz, ax)) > epsilon) {
      return 0;
    }
  }
  return 1;
}

uint32_t TrackMeshFindSlice(uint8_t* base, uint32_t section, uint32_t slice, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820F6BE8 TrackMesh::FindSlice");
  const uint32_t slices = Slices(base, section);
  const uint32_t outline = Outline(base, section);
  int32_t first, end;
  if (int32_t(slice) == -1) {
    end = int32_t(S16(base, section + 0));
    first = 0;
  } else {
    first = int32_t(slice) - 4;
    if (first < 0) {
      first = 0;
    }
    end = int32_t(S16(base, section + 0));
    if (!(int32_t(slice) + 4 > end)) {
      end = int32_t(slice) + 4;
    }
  }
  for (int32_t i = first; i < end; i++) {
    if (TrackMeshSliceContains(base, slices + uint32_t(i) * 28, outline, point) != 0) {
      return uint32_t(i);
    }
  }
  return uint32_t(-1);
}

uint32_t TrackPositionCrossSection(uint8_t* base, uint32_t position, uint32_t mesh,
                                   uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820F71B0 TrackPosition::CrossSection");
  uint32_t found = 0;
  uint32_t section_index = guest::LoadU16(base, position + 0);
  const uint32_t table = SectionTable(base, mesh);
  uint32_t section = guest::LoadU32(base, S16(section_index) * 4 + table);
  uint32_t start = guest::LoadU16(base, position + 2);
  uint32_t slice = start;
  if (int32_t(S16(start)) < 0) {
    start = 0;
  }
  uint32_t last = S16(TrackMeshFindSlice(base, section, S16(start), point));
  if (int32_t(last) != -1) {
    slice = last;
    found = 1;
  }
  if (int32_t(S16(slice)) < 0 || int32_t(S16(slice)) >= int32_t(S16(base, section + 0))) {
    // The sections its links lead to.
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = section + 6 + 6 * i;
      if (guest::LoadU16(base, link - 2) == 0xFFFF) {
        continue;
      }
      const uint32_t target_index = guest::LoadU16(base, link + 0);
      const uint32_t target_slice = S16(base, link + 2);
      const uint32_t target = guest::LoadU32(base, S16(target_index) * 4 + table);
      last = S16(TrackMeshFindSlice(base, target, target_slice, point));
      if (int32_t(last) != -1) {
        section_index = target_index;
        slice = last;
        section = target;
        found = 1;
        break;
      }
    }
  }
  const uint32_t count = guest::LoadU16(base, section + 0);
  if (int32_t(S16(slice)) < 0) {
    last = S16(TrackMeshFindSlice(base, section, S16(count - 1), point));
  } else if (!(int32_t(S16(slice)) < int32_t(S16(count)))) {
    last = S16(TrackMeshFindSlice(base, section, 0, point));
  }
  if (int32_t(S16(last)) != -1) {
    slice = last;
    found = 1;
  }
  if (int32_t(S16(slice)) < 0 || !(int32_t(S16(slice)) < int32_t(S16(count)))) {
    slice = 0;
  }
  guest::StoreU16(base, position + 0, uint16_t(section_index));
  guest::StoreU16(base, position + 2, uint16_t(slice));
  return found;
}

uint32_t TrackMeshSectionOutcode(uint8_t* base, uint32_t mesh, uint32_t point, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_820F7D70 TrackMesh::SectionOutcode");
  const uint32_t s = guest::LoadU32(base, S16(section) * 4 + SectionTable(base, mesh));
  const double margin = Lfs(base, kBoundsMargin);
  uint32_t code = 0;
  const double x = Lfs(base, point + 0);
  if (x < Fsubs(Lfs(base, s + 48), margin)) {
    code = 1;
  } else if (x > Fadds(Lfs(base, s + 56), margin)) {
    code = 2;
  }
  const double a = Lfs(base, s + 44), b = Lfs(base, s + 40);
  const double y = Lfs(base, point + 4);
  if (y < Fsubs(Fsubs(a, b), margin)) {
    code |= 4;
  } else if (y > Fadds(Fadds(b, a), margin)) {
    code |= 8;
  }
  const double z = Lfs(base, point + 8);
  if (z < Fsubs(Lfs(base, s + 52), margin)) {
    return code | 16;
  }
  if (z > Fadds(Lfs(base, s + 60), margin)) {
    code |= 32;
  }
  return code;
}

uint32_t TrackMeshNearestSlice(uint8_t* base, uint32_t section, uint32_t point, uint32_t best) {
  ITC_NATIVE_RUNNING("sub_820F6C88 TrackMesh::NearestSlice");
  uint32_t nearest = uint32_t(-1);
  double best_distance = Lfs(base, best);
  uint32_t slice = Slices(base, section);
  const uint32_t outline = Outline(base, section);
  if (!(int32_t(S16(base, section + 0)) > 0)) {
    return nearest;
  }
  const double reach = Lfs(base, kCenterReachSquared);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  const double tolerance = Lfs(base, kHeightTolerance);
  for (int32_t i = 0; i < int32_t(S16(base, section + 0)); i++, slice += 28) {
    if (!WithinHeight(base, slice, Lfs(base, point + 4), tolerance)) {
      continue;
    }
    const int32_t count = Corners(base, slice);
    double sum_x = zero, sum_z = zero;
    for (int32_t k = 0; k < count; k++) {
      const uint32_t p = guest::LoadU16(base, slice + 12 + 2 * uint32_t(k)) * 8 + outline;
      sum_x = Fadds(Lfs(base, p + 0), sum_x);
      sum_z = Fadds(Lfs(base, p + 4), sum_z);
    }
    const double px = Lfs(base, point + 0), pz = Lfs(base, point + 8);
    const double inverse = Fdivs(one, double(float(double(int64_t(count)))));
    const double cx = Fmuls(inverse, sum_x), cz = Fmuls(inverse, sum_z);
    const double dx = Fsubs(cx, px);
    const double dz = Fsubs(cz, pz);
    const double distance = Fmadds(dz, dz, Fmuls(dx, dx));
    if (distance > reach) {
      continue;
    }
    if (best_distance > distance) {
      Stfs(base, best, distance);
      best_distance = distance;
      nearest = uint32_t(i);
    }
    if (TrackMeshSliceContains(base, slice, outline, point) != 0) {
      Stfs(base, best, zero);
      return uint32_t(i);
    }
  }
  return nearest;
}

uint32_t TrackPositionSearchLinked(uint8_t* base, uint32_t position, uint32_t point, uint32_t mesh) {
  ITC_NATIVE_RUNNING("sub_820F7EE8 TrackPosition::SearchLinked");
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU16(base, kLinkedTried + 2 * i, 0xFFFF);
  }
  uint32_t tried = 0;
  const int32_t own = int32_t(S16(base, position + 0));
  if (own != -1) {
    const uint32_t section =
        guest::LoadU32(base, uint32_t(own) * 4 + SectionTable(base, mesh));
    bool found = false;
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = section + 6 + 6 * i;
      if (guest::LoadU16(base, link - 2) == 0xFFFF) {
        continue;
      }
      const uint32_t target = S16(base, link + 0);
      if (TrackMeshSectionOutcode(base, mesh, point, target) != 0) {
        continue;
      }
      const uint32_t slice = TrackMeshFindSlice(
          base, guest::LoadU32(base, target * 4 + SectionTable(base, mesh)), uint32_t(-1), point);
      if (int32_t(slice) != -1) {
        guest::StoreU16(base, position + 2, uint16_t(slice));
        guest::StoreU16(base, position + 0, uint16_t(target));
        found = true;
        break;
      }
      guest::StoreU16(base, kLinkedTried + 2 * tried, uint16_t(target));
      tried++;
    }
    guest::StoreU16(base, kLinkedTried + 2 * tried, 0xFFFF);
    if (found) {
      return 1;
    }
  }
  const uint32_t file = guest::LoadU32(base, mesh + 4);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, file + 0)); i++) {
    bool skip = false;
    for (uint32_t k = 0; k < tried; k++) {
      if (int32_t(S16(base, kLinkedTried + 2 * k)) == i) {
        skip = true;
        break;
      }
    }
    if (skip || TrackMeshSectionOutcode(base, mesh, point, S16(uint32_t(i))) != 0) {
      continue;
    }
    const uint32_t slice = TrackMeshFindSlice(
        base, guest::LoadU32(base, guest::LoadU32(base, file + 4) + 4 * uint32_t(i)), uint32_t(-1),
        point);
    if (int32_t(slice) != -1) {
      guest::StoreU16(base, position + 2, uint16_t(slice));
      guest::StoreU16(base, position + 0, uint16_t(i));
      return 1;
    }
  }
  return 0;
}

uint32_t TrackPositionSearchAll(uint8_t* base, uint32_t position, uint32_t point, uint32_t mesh,
                                uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F7E30 TrackPosition::SearchAll");
  uint32_t found = 0;
  CopyWord(base, kFarthest, frame + 80);
  uint32_t file = guest::LoadU32(base, mesh + 4);
  if (!(int32_t(guest::LoadU32(base, file + 0)) > 0)) {
    return found;
  }
  const double zero = Lfs(base, kZero);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, file + 0));
       i++, file = guest::LoadU32(base, mesh + 4)) {
    const uint32_t index = S16(uint32_t(i));
    if (TrackMeshSectionOutcode(base, mesh, point, index) != 0) {
      continue;
    }
    const uint32_t section = guest::LoadU32(base, guest::LoadU32(base, file + 4) + 4 * uint32_t(i));
    const uint32_t slice = TrackMeshNearestSlice(base, section, point, frame + 80);
    if (int32_t(slice) == -1) {
      continue;
    }
    found = 1;
    guest::StoreU16(base, position + 2, uint16_t(slice));
    guest::StoreU16(base, position + 0, uint16_t(index));
    if (Lfs(base, frame + 80) == zero) {
      return found;
    }
  }
  return found;
}

uint32_t TrackPositionFindCell(uint8_t* base, uint32_t mesh, uint32_t position, uint32_t point,
                               uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F8090 TrackPosition::FindCell");
  const uint32_t f = frame;
  const uint32_t original_slice = guest::LoadU16(base, position + 2);
  guest::StoreU32(base, f + 80, 0);
  if (int32_t(S16(base, position + 0)) == -1) {
    TrackPositionSearchLinked(base, position, point, mesh);
    return 0;
  }
  const uint32_t table = SectionTable(base, mesh);
  const uint32_t section = guest::LoadU32(base, S16(base, position + 0) * 4 + table);
  const uint32_t slices = Slices(base, section);
  const uint32_t outline = Outline(base, section);
  const int32_t here = int32_t(S16(original_slice));
  if (TrackMeshSliceContains(base, uint32_t(here) * 28 + slices, outline, point) == 1) {
    return 0;
  }
  // The slices 1, 2 and 3 after and before it (as halfwords).
  const int32_t count = int32_t(S16(base, section + 0));
  const int32_t before1 = int32_t(S16(uint32_t(here - 1))), after1 = int32_t(S16(uint32_t(here + 1)));
  const int32_t before2 = int32_t(S16(uint32_t(before1 - 1))),
                after2 = int32_t(S16(uint32_t(after1 + 1)));
  const int32_t before3 = int32_t(S16(uint32_t(before2 - 1))),
                after3 = int32_t(S16(uint32_t(after2 + 1)));
  auto test = [&](int32_t s) {
    if (TrackMeshSliceContains(base, uint32_t(s) * 28 + slices, outline, point) == 1) {
      guest::StoreU16(base, position + 2, uint16_t(s));
      return true;
    }
    return false;
  };
  if (after1 < count && test(after1)) {
    return 0;
  }
  if (before1 >= 0 && test(before1)) {
    return 0;
  }
  if (after2 < count && test(after2)) {
    return 0;
  }
  if (before2 >= 0 && test(before2)) {
    return 0;
  }
  if (after3 < count && test(after3)) {
    return 0;
  }
  if (before3 >= 0 && test(before3)) {
    return 0;
  }
  bool searched_cross = false;
  uint32_t crossed = 0;
  if (!(after1 < count)) {
    guest::StoreU16(base, position + 2, uint16_t(after1));
    crossed = TrackPositionCrossSection(base, position, mesh, point);
    searched_cross = true;
  } else if (!(before1 > 0)) {
    guest::StoreU16(base, position + 2, uint16_t(before1));
    crossed = TrackPositionCrossSection(base, position, mesh, point);
    searched_cross = true;
  } else {
    // The section's linked cells (any link with a slice it leaves from).
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = section + 6 + 6 * i;
      if (guest::LoadU16(base, link - 2) == 0xFFFF) {
        continue;
      }
      const uint32_t target_index = guest::LoadU16(base, link + 0);
      const uint32_t target_slice = guest::LoadU16(base, link + 2);
      const uint32_t target = guest::LoadU32(base, S16(target_index) * 4 + table);
      if (TrackMeshSliceContains(base, S16(target_slice) * 28 + Slices(base, target),
                                 Outline(base, target), point) == 1) {
        guest::StoreU16(base, position + 0, uint16_t(target_index));
        guest::StoreU16(base, position + 2, uint16_t(target_slice));
        return 0;
      }
    }
  }
  if (searched_cross && crossed != 0) {
    return 0;
  }
  // The sections linked from the position's (now) section: a slice near the
  // link's, or any, containing the point.
  if (int32_t(S16(base, position + 0)) != -1) {
    const uint32_t table2 = SectionTable(base, mesh);
    const uint32_t own = guest::LoadU32(base, S16(base, position + 0) * 4 + table2);
    for (uint32_t i = 0; i < 6; i++) {
      const uint32_t link = own + 6 + 6 * i;
      if (guest::LoadU16(base, link - 2) == 0xFFFF) {
        continue;
      }
      const uint32_t target_index = guest::LoadU16(base, link + 0);
      const uint32_t target_slice = S16(base, link + 2);
      const uint32_t target = guest::LoadU32(base, S16(target_index) * 4 + table2);
      const uint32_t target_slices = Slices(base, target);
      const uint32_t target_outline = Outline(base, target);
      uint32_t s = S16(TrackMeshFindSlice(base, target, target_slice, point));
      if (int32_t(s) == -1) {
        s = S16(TrackMeshFindSlice(base, target, uint32_t(-1), point));
      }
      if (int32_t(s) == -1) {
        continue;
      }
      if (TrackMeshSliceContains(base, s * 28 + target_slices, target_outline, point) == 1) {
        guest::StoreU16(base, position + 0, uint16_t(target_index));
        guest::StoreU16(base, position + 2, uint16_t(s));
        break;
      }
    }
  }
  if (guest::LoadU16(base, position + 2) == 0xFFFF) {
    guest::StoreU32(base, f + 80, uint32_t(-1));
    if (int32_t(TrackPositionSearchLinked(base, position, point, mesh)) != 1) {
      guest::StoreU32(base, f + 80, uint32_t(-2));
      if (int32_t(TrackPositionSearchAll(base, position, point, mesh, f - 160)) != 1) {
        guest::StoreU16(base, position + 2, uint16_t(original_slice));
        guest::StoreU32(base, f + 80, uint32_t(-3));
      }
    }
  }
  return guest::LoadU32(base, f + 80);
}

}  // namespace track_collision
