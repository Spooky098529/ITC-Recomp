// The track position's cell: which slice holds a point
// (docs/functions/collision-world.md): see track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/float_ops.h"
#include "schema/collision_world.h"
#include "schema/track_route.h"
#include "track_route/track_route.h"

#include "base/log.h"

namespace track_collision {

using namespace car_physics::ops;

namespace {

using G = schema::CollisionWorldGlobals;
using Point = schema::TrackQueryPoint;
using Section = schema::TrackMeshSection;
using Slice = schema::TrackMeshSlice;
using Link = schema::TrackMeshLink;
using OutlinePoint = schema::TrackOutlinePoint;
using Position = schema::TrackPosition;

uint32_t S16(uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(address))));
}
uint32_t S16(uint16_t value) {
  return uint32_t(int32_t(int16_t(value)));
}
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(value)));
}
// The mesh file's section table ([[mesh + 4] + 4]).
uint32_t SectionTable(uint32_t mesh) {
  return schema::TrackMesh(mesh).file().sections();
}
uint32_t Slices(uint32_t section) {
  return Section(section).slices() + section;
}
uint32_t Outline(uint32_t section) {
  return Section(section).outline() + section;
}
// Corner `i` of the slice's outline (a point's index in the section's
// outline).
uint32_t Corner(uint32_t slice, uint32_t i) {
  return guest::LoadU16(Slice(slice).corners_address() + 2 * i);
}
// An outline's corner count: 3 when its +18 is 0xFFFF, else 4.
int32_t Corners(uint32_t slice) {
  return Slice(slice).corner_3() == 0xFFFF ? 3 : 4;
}
// The point's y within the tolerance of the slice's height (a NaN passes,
// as in the original's two tests).
bool WithinHeight(uint32_t slice, double y, double tolerance) {
  const double height = double(Slice(slice).height());
  if (y > Fadds(height, tolerance)) {
    return false;
  }
  return !(y < Fsubs(height, tolerance));
}

}  // namespace

uint32_t TrackMeshSliceContains(uint32_t slice, uint32_t outline, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820F6A38 TrackMesh::SliceContains");
  if (!WithinHeight(slice, double(Point(point).y()),
                    double(G::height_tolerance()))) {
    return 0;
  }
  const int32_t count = Corners(slice);
  double sum_x = Lfs(kZero), sum_z = sum_x;
  for (int32_t i = 0; i < count; i++) {
    const uint32_t p = Corner(slice, uint32_t(i)) * 8 + outline;
    sum_x = Fadds(double(OutlinePoint(p).x()), sum_x);
    sum_z = Fadds(double(OutlinePoint(p).z()), sum_z);
  }
  const double px = double(Point(point).x()), pz = double(Point(point).z());
  const double inverse = Fdivs(Lfs(kOne), double(float(double(int64_t(count)))));
  const double dx = Fabs(Fmsubs(inverse, sum_x, px));
  const double dz = Fabs(Fmsubs(inverse, sum_z, pz));
  const double reach = double(G::center_reach());
  if (dx > reach || dz > reach) {
    return 0;
  }
  if (Fmadds(dz, dz, Fmuls(dx, dx)) > double(G::center_reach_squared())) {
    return 0;
  }
  const double epsilon = double(G::inside_epsilon());
  for (int32_t i = 0; i < count; i++) {
    const double z = double(Point(point).z());
    const uint32_t p = Corner(slice, uint32_t(i)) * 8 + outline;
    const double ax = Fsubs(double(OutlinePoint(p).x()), px);
    const double az = Fsubs(double(OutlinePoint(p).z()), z);
    const uint32_t next = uint32_t((i + 1) % count);
    const uint32_t q = Corner(slice, next) * 8 + outline;
    const double bz = Fsubs(double(OutlinePoint(q).z()), z);
    const double bx = Fsubs(double(OutlinePoint(q).x()), px);
    if (Fmsubs(bx, az, Fmuls(bz, ax)) > epsilon) {
      return 0;
    }
  }
  return 1;
}

uint32_t TrackMeshFindSlice(uint32_t section, uint32_t slice, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820F6BE8 TrackMesh::FindSlice");
  const uint32_t slices = Slices(section);
  const uint32_t outline = Outline(section);
  int32_t first, end;
  if (int32_t(slice) == -1) {
    end = int32_t(S16(Section(section).slice_count()));
    first = 0;
  } else {
    first = int32_t(slice) - 4;
    if (first < 0) {
      first = 0;
    }
    end = int32_t(S16(Section(section).slice_count()));
    if (!(int32_t(slice) + 4 > end)) {
      end = int32_t(slice) + 4;
    }
  }
  for (int32_t i = first; i < end; i++) {
    if (TrackMeshSliceContains(slices + uint32_t(i) * 28, outline, point) != 0) {
      return uint32_t(i);
    }
  }
  return uint32_t(-1);
}

uint32_t TrackPositionCrossSection(uint32_t position, uint32_t mesh,
                                   uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820F71B0 TrackPosition::CrossSection");
  uint32_t found = 0;
  uint32_t section_index = Position(position).section();
  const uint32_t table = SectionTable(mesh);
  uint32_t section = guest::LoadU32(S16(section_index) * 4 + table);
  uint32_t start = Position(position).slice();
  uint32_t slice = start;
  if (int32_t(S16(start)) < 0) {
    start = 0;
  }
  uint32_t last = S16(TrackMeshFindSlice(section, S16(start), point));
  if (int32_t(last) != -1) {
    slice = last;
    found = 1;
  }
  if (int32_t(S16(slice)) < 0 ||
      int32_t(S16(slice)) >= int32_t(S16(Section(section).slice_count()))) {
    // The sections its links lead to.
    for (uint32_t i = 0; i < 6; i++) {
      const Link link(Section(section).links_address() + 6 * i);
      if (link.from_slice() == 0xFFFF) {
        continue;
      }
      const uint32_t target_index = link.section();
      const uint32_t target_slice = S16(link.slice());
      const uint32_t target = guest::LoadU32(S16(target_index) * 4 + table);
      last = S16(TrackMeshFindSlice(target, target_slice, point));
      if (int32_t(last) != -1) {
        section_index = target_index;
        slice = last;
        section = target;
        found = 1;
        break;
      }
    }
  }
  const uint32_t count = Section(section).slice_count();
  if (int32_t(S16(slice)) < 0) {
    last = S16(TrackMeshFindSlice(section, S16(count - 1), point));
  } else if (!(int32_t(S16(slice)) < int32_t(S16(count)))) {
    last = S16(TrackMeshFindSlice(section, 0, point));
  }
  if (int32_t(S16(last)) != -1) {
    slice = last;
    found = 1;
  }
  if (int32_t(S16(slice)) < 0 || !(int32_t(S16(slice)) < int32_t(S16(count)))) {
    slice = 0;
  }
  Position(position).set_section(uint16_t(section_index));
  Position(position).set_slice(uint16_t(slice));
  return found;
}

uint32_t TrackMeshSectionOutcode(uint32_t mesh, uint32_t point, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_820F7D70 TrackMesh::SectionOutcode");
  const uint32_t s = guest::LoadU32(S16(section) * 4 + SectionTable(mesh));
  const double margin = double(G::bounds_margin());
  uint32_t code = 0;
  const double x = double(Point(point).x());
  if (x < Fsubs(double(Section(s).min_x()), margin)) {
    code = 1;
  } else if (x > Fadds(double(Section(s).max_x()), margin)) {
    code = 2;
  }
  const double a = double(Section(s).center_y()), b = double(Section(s).half_height());
  const double y = double(Point(point).y());
  if (y < Fsubs(Fsubs(a, b), margin)) {
    code |= 4;
  } else if (y > Fadds(Fadds(b, a), margin)) {
    code |= 8;
  }
  const double z = double(Point(point).z());
  if (z < Fsubs(double(Section(s).min_z()), margin)) {
    return code | 16;
  }
  if (z > Fadds(double(Section(s).max_z()), margin)) {
    code |= 32;
  }
  return code;
}

uint32_t TrackMeshNearestSlice(uint32_t section, uint32_t point, uint32_t best) {
  ITC_NATIVE_RUNNING("sub_820F6C88 TrackMesh::NearestSlice");
  uint32_t nearest = uint32_t(-1);
  const schema::TrackNearestDistance best_view(best);
  double best_distance = double(best_view.distance());
  uint32_t slice = Slices(section);
  const uint32_t outline = Outline(section);
  if (!(int32_t(S16(Section(section).slice_count())) > 0)) {
    return nearest;
  }
  const double reach = double(G::center_reach_squared());
  const double one = Lfs(kOne);
  const double zero = Lfs(kZero);
  const double tolerance = double(G::height_tolerance());
  for (int32_t i = 0; i < int32_t(S16(Section(section).slice_count())); i++, slice += 28) {
    if (!WithinHeight(slice, double(Point(point).y()), tolerance)) {
      continue;
    }
    const int32_t count = Corners(slice);
    double sum_x = zero, sum_z = zero;
    for (int32_t k = 0; k < count; k++) {
      const uint32_t p = Corner(slice, uint32_t(k)) * 8 + outline;
      sum_x = Fadds(double(OutlinePoint(p).x()), sum_x);
      sum_z = Fadds(double(OutlinePoint(p).z()), sum_z);
    }
    const double px = double(Point(point).x()), pz = double(Point(point).z());
    const double inverse = Fdivs(one, double(float(double(int64_t(count)))));
    const double cx = Fmuls(inverse, sum_x), cz = Fmuls(inverse, sum_z);
    const double dx = Fsubs(cx, px);
    const double dz = Fsubs(cz, pz);
    const double distance = Fmadds(dz, dz, Fmuls(dx, dx));
    if (distance > reach) {
      continue;
    }
    if (best_distance > distance) {
      best_view.set_distance(float(distance));
      best_distance = distance;
      nearest = uint32_t(i);
    }
    if (TrackMeshSliceContains(slice, outline, point) != 0) {
      best_view.set_distance(float(zero));
      return uint32_t(i);
    }
  }
  return nearest;
}

uint32_t TrackPositionSearchLinked(uint32_t position, uint32_t point, uint32_t mesh) {
  ITC_NATIVE_RUNNING("sub_820F7EE8 TrackPosition::SearchLinked");
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU16(G::linked_tried_address() + 2 * i, 0xFFFF);
  }
  uint32_t tried = 0;
  const int32_t own = int32_t(S16(Position(position).section()));
  if (own != -1) {
    const uint32_t section =
        guest::LoadU32(uint32_t(own) * 4 + SectionTable(mesh));
    bool found = false;
    for (uint32_t i = 0; i < 6; i++) {
      const Link link(Section(section).links_address() + 6 * i);
      if (link.from_slice() == 0xFFFF) {
        continue;
      }
      const uint32_t target = S16(link.section());
      if (TrackMeshSectionOutcode(mesh, point, target) != 0) {
        continue;
      }
      const uint32_t slice = TrackMeshFindSlice(guest::LoadU32(target * 4 + SectionTable(mesh)), uint32_t(-1), point);
      if (int32_t(slice) != -1) {
        Position(position).set_slice(uint16_t(slice));
        Position(position).set_section(uint16_t(target));
        found = true;
        break;
      }
      guest::StoreU16(G::linked_tried_address() + 2 * tried, uint16_t(target));
      tried++;
    }
    guest::StoreU16(G::linked_tried_address() + 2 * tried, 0xFFFF);
    if (found) {
      return 1;
    }
  }
  const schema::TrackMeshFile file = schema::TrackMesh(mesh).file();
  for (int32_t i = 0; i < int32_t(file.section_count()); i++) {
    bool skip = false;
    for (uint32_t k = 0; k < tried; k++) {
      if (int32_t(S16(G::linked_tried_address() + 2 * k)) == i) {
        skip = true;
        break;
      }
    }
    if (skip || TrackMeshSectionOutcode(mesh, point, S16(uint32_t(i))) != 0) {
      continue;
    }
    const uint32_t slice = TrackMeshFindSlice(guest::LoadU32(file.sections() + 4 * uint32_t(i)), uint32_t(-1),
        point);
    if (int32_t(slice) != -1) {
      Position(position).set_slice(uint16_t(slice));
      Position(position).set_section(uint16_t(i));
      return 1;
    }
  }
  return 0;
}

uint32_t TrackPositionSearchAll(uint32_t position, uint32_t point, uint32_t mesh,
                                uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F7E30 TrackPosition::SearchAll");
  uint32_t found = 0;
  CopyWord(G::farthest_address(), frame + 80);
  const schema::TrackMesh m(mesh);
  schema::TrackMeshFile file = m.file();
  if (!(int32_t(file.section_count()) > 0)) {
    return found;
  }
  const double zero = Lfs(kZero);
  for (int32_t i = 0; i < int32_t(file.section_count()); i++, file = m.file()) {
    const uint32_t index = S16(uint32_t(i));
    if (TrackMeshSectionOutcode(mesh, point, index) != 0) {
      continue;
    }
    const uint32_t section = guest::LoadU32(file.sections() + 4 * uint32_t(i));
    const uint32_t slice = TrackMeshNearestSlice(section, point, frame + 80);
    if (int32_t(slice) == -1) {
      continue;
    }
    found = 1;
    Position(position).set_slice(uint16_t(slice));
    Position(position).set_section(uint16_t(index));
    if (Lfs(frame + 80) == zero) {
      return found;
    }
  }
  return found;
}

uint32_t TrackPositionFindCell(uint32_t mesh, uint32_t position, uint32_t point,
                               uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F8090 TrackPosition::FindCell");
  const uint32_t f = frame;
  const uint32_t original_slice = Position(position).slice();
  guest::StoreU32(f + 80, 0);
  if (int32_t(S16(Position(position).section())) == -1) {
    TrackPositionSearchLinked(position, point, mesh);
    return 0;
  }
  const uint32_t table = SectionTable(mesh);
  const uint32_t section =
      guest::LoadU32(S16(Position(position).section()) * 4 + table);
  const uint32_t slices = Slices(section);
  const uint32_t outline = Outline(section);
  const int32_t here = int32_t(S16(original_slice));
  if (TrackMeshSliceContains(uint32_t(here) * 28 + slices, outline, point) == 1) {
    return 0;
  }
  // The slices 1, 2 and 3 after and before it (as halfwords).
  const int32_t count = int32_t(S16(Section(section).slice_count()));
  const int32_t before1 = int32_t(S16(uint32_t(here - 1))), after1 = int32_t(S16(uint32_t(here + 1)));
  const int32_t before2 = int32_t(S16(uint32_t(before1 - 1))),
                after2 = int32_t(S16(uint32_t(after1 + 1)));
  const int32_t before3 = int32_t(S16(uint32_t(before2 - 1))),
                after3 = int32_t(S16(uint32_t(after2 + 1)));
  auto test = [&](int32_t s) {
    if (TrackMeshSliceContains(uint32_t(s) * 28 + slices, outline, point) == 1) {
      Position(position).set_slice(uint16_t(s));
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
    Position(position).set_slice(uint16_t(after1));
    crossed = TrackPositionCrossSection(position, mesh, point);
    searched_cross = true;
  } else if (!(before1 > 0)) {
    Position(position).set_slice(uint16_t(before1));
    crossed = TrackPositionCrossSection(position, mesh, point);
    searched_cross = true;
  } else {
    // The section's linked cells (any link with a slice it leaves from).
    for (uint32_t i = 0; i < 6; i++) {
      const Link link(Section(section).links_address() + 6 * i);
      if (link.from_slice() == 0xFFFF) {
        continue;
      }
      const uint32_t target_index = link.section();
      const uint32_t target_slice = link.slice();
      const uint32_t target = guest::LoadU32(S16(target_index) * 4 + table);
      if (TrackMeshSliceContains(S16(target_slice) * 28 + Slices(target),
                                 Outline(target), point) == 1) {
        Position(position).set_section(uint16_t(target_index));
        Position(position).set_slice(uint16_t(target_slice));
        return 0;
      }
    }
  }
  if (searched_cross && crossed != 0) {
    return 0;
  }
  // The sections linked from the position's (now) section: a slice near the
  // link's, or any, containing the point.
  if (int32_t(S16(Position(position).section())) != -1) {
    const uint32_t table2 = SectionTable(mesh);
    const uint32_t own = guest::LoadU32(S16(Position(position).section()) * 4 + table2);
    for (uint32_t i = 0; i < 6; i++) {
      const Link link(Section(own).links_address() + 6 * i);
      if (link.from_slice() == 0xFFFF) {
        continue;
      }
      const uint32_t target_index = link.section();
      const uint32_t target_slice = S16(link.slice());
      const uint32_t target = guest::LoadU32(S16(target_index) * 4 + table2);
      const uint32_t target_slices = Slices(target);
      const uint32_t target_outline = Outline(target);
      uint32_t s = S16(TrackMeshFindSlice(target, target_slice, point));
      if (int32_t(s) == -1) {
        s = S16(TrackMeshFindSlice(target, uint32_t(-1), point));
      }
      if (int32_t(s) == -1) {
        continue;
      }
      if (TrackMeshSliceContains(s * 28 + target_slices, target_outline, point) == 1) {
        Position(position).set_section(uint16_t(target_index));
        Position(position).set_slice(uint16_t(s));
        break;
      }
    }
  }
  if (Position(position).slice() == 0xFFFF) {
    guest::StoreU32(f + 80, uint32_t(-1));
    if (int32_t(TrackPositionSearchLinked(position, point, mesh)) != 1) {
      guest::StoreU32(f + 80, uint32_t(-2));
      if (int32_t(TrackPositionSearchAll(position, point, mesh, f - 160)) != 1) {
        Position(position).set_slice(uint16_t(original_slice));
        guest::StoreU32(f + 80, uint32_t(-3));
      }
    }
  }
  return guest::LoadU32(f + 80);
}

void TrackPositionUpdate(uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F8470 TrackPosition::Update");
  const Position pos(position);
  const uint32_t route_position = pos.route_position_address();
  const uint32_t point = pos.point_x_address();
  // The route position before, for Follow (its frame's +80).
  guest::StoreU32(frame + 80, pos.route_position());
  TrackPositionFindCell(kMesh, position, point, frame - 240);
  // The ground under the point raised by 0.5; its y put back when none.
  const double y = double(Point(point).y());
  Point(point).set_y(float(Fadds(y, double(G::half()))));
  const int32_t kind = int32_t(TrackMeshGroundHeight(kMesh, point, position,
                                                     pos.color_x_address(), pos.normal_x_address(),
                                                     frame - 240));
  if (!(kind > -1)) {
    Point(point).set_y(float(y));
  }
  pos.set_flags(0);
  if (kind == 4 || kind == 20) {
    pos.set_flags(1);
  }
  uint32_t bit = 0;
  switch (int32_t(TrackMeshSliceKind(kMesh, position) & 0xFFFF)) {
    case 30:
      bit = 2;
      break;
    case 31:
      bit = 4;
      break;
    case 32:
      bit = 8;
      break;
    default:
      break;
  }
  if (bit != 0) {
    pos.set_flags(uint16_t(pos.flags() | bit));
  }
  track_route::Follow(track_route::kRoute, route_position, point, frame + 80, frame - 176);
}

void TrackPositionInit(uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820F8578 TrackPosition::Init");
  const Position pos(position);
  const uint32_t route_position = pos.route_position_address();
  track_route::Point(track_route::kRoute, pos.point_x_address(), route_position, frame - 112);
  // The cell from the route slice's record's word +32.
  const schema::TrackRouteFile file = schema::TrackRoute(track_route::kRoute).file();
  const uint32_t number = pos.route_section();
  const uint32_t table = file.sections();
  const schema::TrackRouteSection section(guest::LoadU32(table + number * 4));
  const int32_t slice = int16_t(pos.route_slice());
  const uint32_t cell =
      schema::TrackRouteSlice(section.slices_address() + uint32_t(slice) * 84).mesh_cell();
  pos.set_surface_kind(0);
  pos.set_flags(0);
  Position(position).set_section(uint16_t(cell >> 16));
  Position(position).set_slice(uint16_t(cell));
  TrackPositionUpdate(position, frame - 144);
}

}  // namespace track_collision
