// The course's route (docs/functions/track-route.md): see track_route.h.

#include "track_route/track_route.h"

#include "car_physics/float_ops.h"
#include "strings_and_font/steps.h"
#include "vehicle_accessors/vectors.h"

#include "base/log.h"
#include "schema/track_route.h"

#include <bit>
#include <cmath>
#include <cstring>

namespace track_route {

using namespace car_physics::ops;
namespace steps = strings_and_font::steps;

namespace {

using G = schema::TrackRouteGlobals;
using Route = schema::TrackRoute;
using File = schema::TrackRouteFile;
using Sec = schema::TrackRouteSection;
using RLink = schema::TrackRouteLink;
using Slice = schema::TrackRouteSlice;
using Pos = schema::TrackRoutePosition;
using Branch = schema::TrackRouteBranch;
using Vec = schema::TrackRoutePoint;
using Word = schema::TrackRouteWord;
using MaskList = schema::TrackRouteMaskList;
using Half = schema::TrackRouteHalfword;
using Flt = schema::TrackRouteFloat;
using Path = schema::TrackRoutePath;

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
uint32_t LoadU16(uint32_t address) {
  return guest::LoadU16(address);
}
int32_t LoadS16(uint32_t address) {
  return int16_t(guest::LoadU16(address));
}
uint32_t LoadU8(uint32_t address) {
  return guest::LoadU8(address);
}

// The table's section `number` (its low 16 bits).
uint32_t SectionIn(uint32_t table, uint32_t number) {
  return Load(table + (number & 0xFFFF) * 4);
}
// The section of a route's file.
uint32_t Section(uint32_t file, uint32_t number) {
  return SectionIn(File(file).sections(), number);
}
// A slice's record (the 32-bit wrap of the original's 64-bit sum).
uint32_t Record(uint32_t section, int32_t slice) {
  return Sec(section).slices_address() + uint32_t(slice) * Slice::kSize;
}
uint32_t Kind(uint32_t section) {
  return Sec(section).kind() & 0xF;
}
// The lanes' count byte of the section's first slice (the section's +180).
uint32_t FirstSliceLanes(uint32_t section) {
  return Slice(Sec(section).slices_address()).lanes();
}

// A position moved over the link at `link` (StepForward's and
// StepAlongPath's ends, and the lane steps'): the section from the link, the
// slice less the slices' count (`slices()`, read when the original reads its
// word), in the original's order.
template <typename Slices>
uint32_t TakeForward(uint32_t position, uint32_t link, Slices slices) {
  const uint32_t section = RLink(link).section();
  const uint32_t slice = Pos(position).slice();
  Pos(position).set_section(uint16_t(section));
  Pos(position).set_slice(uint16_t(slice - slices()));
  return 1;
}
// The word at `address`, read when called.
auto WordAt(uint32_t address) {
  return [address] { return Load(address); };
}

// StepForward with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardWith(uint32_t route, uint32_t position, Slices slices) {
  ITC_NATIVE_RUNNING("sub_820F8B48 TrackRoute::StepForward");
  const uint32_t table = Route(route).file().sections();
  const uint32_t section = SectionIn(table, Pos(position).section());
  for (int32_t i = 2; i >= 0; i--) {
    const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(link).section();
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 0) {
      const uint32_t kind = Kind(SectionIn(table, to));
      if (kind == 2 || kind == 4) {
        continue;
      }
    }
    return TakeForward(position, link, slices);
  }
  return 0;
}

// BranchesMatching, each match handed to `found(section, link index)`.
template <typename Found>
uint32_t BranchesMatchingWith(uint32_t route, uint32_t section, uint32_t mask,
                              Found found) {
  ITC_NATIVE_RUNNING("sub_820F8BD0 TrackRoute::BranchesMatching");
  uint32_t count = 0;
  for (uint32_t i = 0; i <= 2; i++) {
    const uint32_t to = RLink(Sec(section).links_address() + 4 * i).section();
    if (to == 0xFFFF) {
      continue;
    }
    const uint32_t s = Section(Route(route).file().address, to);
    const uint32_t kind = Kind(s);
    if (kind == 2 || kind == 4) {
      continue;
    }
    const uint32_t branch = Sec(s).branch();
    const uint32_t bits = Word(mask).value();
    const uint32_t bit = (branch & 0x20) != 0 ? 0 : (1u << (branch & 0x1F));
    if ((bit & bits) == 0) {
      continue;
    }
    found(to, i);
    count++;
  }
  return count;
}

// The first match of BranchesMatching (its count, the first's section and
// link index), as the callers read it from their frames.
struct FirstBranch {
  uint32_t count = 0;
  uint32_t section = 0;
  uint32_t link = 0;
};
FirstBranch FirstBranchMatching(uint32_t route, uint32_t section, uint32_t mask) {
  FirstBranch first;
  bool any = false;
  first.count = BranchesMatchingWith(route, section, mask, [&](uint32_t to, uint32_t i) {
    if (!any) {
      any = true;
      first.section = to;
      first.link = i;
    }
  });
  return first;
}

// LinkLane with the link index as a value.
int32_t LinkLaneWith(uint32_t route, uint32_t position, int32_t link,
                     uint32_t lane_address) {
  ITC_NATIVE_RUNNING("sub_820F8CE8 TrackRoute::LinkLane");
  const uint32_t file = Route(route).file().address;
  const uint32_t number = Pos(position).section();
  const uint32_t section = Section(file, number);
  if (RLink(Sec(section).links_address() + uint32_t(link) * 4).section() == 0xFFFF) {
    return -1;
  }
  const int32_t lane = int16_t(Half(lane_address).value());
  return int8_t(LoadU8(Sec(section).link_lanes_address() + uint32_t(link) * 8 + uint32_t(lane)));
}

// A link's slice difference (LinkDistance): `from` less the slice of link
// `index` of `section`, plus 1 for a forward link, less 1 for a back one.
int32_t LinkSlices(int32_t from, uint32_t section, int32_t index) {
  int32_t d = int32_t(uint32_t(from) -
                      uint32_t(int16_t(RLink(Sec(section).links_address() + 4 * uint32_t(index)).slice())));
  if (index < 3) {
    d++;
  } else if (index < 6) {
    d--;
  }
  return d;
}

// StepForwardMasked with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardMaskedWith(uint32_t route, uint32_t position, Slices slices,
                               uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F93A0 TrackRoute::StepForwardMasked");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  if (MaskList(masks).first() != 0) {
    uint32_t mask = masks;
    for (uint32_t m = 0;;) {
      const FirstBranch first = FirstBranchMatching(route, section, mask);
      if (int32_t(first.count) > 0) {
        const uint32_t slice = Pos(position).slice();
        Pos(position).set_section(uint16_t(first.section));
        Pos(position).set_slice(uint16_t(slice - slices()));
        return 1;
      }
      m++;
      mask = masks + 4 * m;
      if (MaskList(mask).first() == 0) {
        break;
      }
    }
  }
  return StepForwardWith(route, position, slices);
}

// The lane steps' end: the position over the link (`link` its index) to
// the section `to`, the lane carried over the link (LinkLane, from the new
// section's lane bytes as the original reads them), kept below the new
// section's lanes; none: the last lane for link 0, else 0.
template <typename Slices>
uint32_t TakeLane(uint32_t route, uint32_t position, uint32_t lane_address,
                  uint32_t to, int32_t link, Slices slices) {
  const uint32_t slice = Pos(position).slice();
  Pos(position).set_section(uint16_t(to));
  Pos(position).set_slice(uint16_t(slice - slices()));
  const int32_t carried = LinkLaneWith(route, position, link, lane_address);
  Half(lane_address).set_value(uint16_t(carried));
  const int32_t lanes =
      int32_t(FirstSliceLanes(Section(Route(route).file().address, Pos(position).section())) & 0xF);
  const int32_t lane = int16_t(carried);
  if (lane < 0) {
    Half(lane_address).set_value(uint16_t(link == 0 ? lanes - 1 : 0));
  } else if (!(lane < lanes)) {
    Half(lane_address).set_value(uint16_t(lanes - 1));
  }
  return 1;
}

// StepForwardLane with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardLaneWith(uint32_t route, uint32_t position,
                             uint32_t lane_address, Slices slices) {
  ITC_NATIVE_RUNNING("sub_820F9448 TrackRoute::StepForwardLane");
  const uint32_t file = Route(route).file().address;
  const uint32_t table = File(file).sections();
  const uint32_t section = SectionIn(table, Pos(position).section());
  // The forward links whose lanes, added up in link order, hold the lane
  // (every linked section's lanes added; kinds 1, 3 and 4 not taken).
  int32_t lanes = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t link = Sec(section).links_address() + 4 * i;
    const uint32_t to = RLink(link).section();
    if (int16_t(to) < 0) {
      continue;
    }
    const uint32_t s = SectionIn(File(file).sections(), to);
    lanes += int32_t(FirstSliceLanes(s) & 0xF);
    const int32_t kind = int32_t(Kind(s));
    if (kind == 1 || (kind > 2 && kind <= 4)) {
      continue;
    }
    if (int16_t(Half(lane_address).value()) < lanes) {
      return TakeLane(route, position, lane_address, RLink(link).section(), int32_t(i), slices);
    }
  }
  // Else links 2, 1, 0 to a section not of kinds 1, 3 or 4, onto its last
  // lane (0 when it has none).
  for (int32_t i = 2; i >= 0; i--) {
    const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(link).section();
    if (int16_t(to) < 0) {
      continue;
    }
    const uint32_t kind = Kind(SectionIn(table, to));
    if (kind == 1 || kind - 3 <= 1) {
      continue;
    }
    const uint32_t next = RLink(link).section();
    TakeForward(position, link, slices);
    const int32_t last = int16_t(
        int32_t(FirstSliceLanes(Section(Route(route).file().address, next)) & 0xF) - 1);
    Half(lane_address).set_value(uint16_t(last));
    if (last < 0) {
      Half(lane_address).set_value(0);
    }
    return 1;
  }
  return 0;
}

// StepAlongPath with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepAlongPathWith(uint32_t route, uint32_t position, Slices slices,
                           uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820F8C58 TrackRoute::StepAlongPath");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  while (Path(path).first() < 0x8000) {
    for (uint32_t i = 0; i <= 2; i++) {
      const uint32_t link = Sec(section).links_address() + 4 * i;
      const int32_t to = int16_t(RLink(link).section());
      if (to != -1 && to == int16_t(Path(path).first())) {
        return TakeForward(position, link, slices);
      }
    }
    path += 2;
  }
  return 0;
}

// Whether the position is on the route: its section within the file's
// count, its slice within the section's.
bool OnRoute(uint32_t route, uint32_t position) {
  const uint32_t number = Pos(position).section();
  const int32_t section = int16_t(number);
  if (section < 0) {
    return false;
  }
  const uint32_t file = Route(route).file().address;
  if (!(int32_t(File(file).section_count()) > section)) {
    return false;
  }
  const int32_t slice = int16_t(Pos(position).slice());
  if (slice < 0) {
    return false;
  }
  return int16_t(Sec(Section(file, number)).slice_count()) > slice;
}

// Normalize and NormalizePath around their forward step: on the route, 1;
// a section off the route, (0, 0) and 0; else NormalizeBack then `forward()`,
// 1 when both did; else 0, the position (0, 0) unless it's on the route.
template <typename Forward>
uint32_t NormalizeWith(uint32_t route, uint32_t position, Forward forward) {
  if (OnRoute(route, position)) {
    return 1;
  }
  const int32_t section = int16_t(Pos(position).section());
  if (section >= 0 && section < int32_t(Route(route).file().section_count())) {
    if (NormalizeBack(route, position) != 0 && forward() != 0) {
      return 1;
    }
    if (OnRoute(route, position)) {
      return 0;
    }
  }
  Pos(position).set_packed(0);
  return 0;
}

// StepForwardLaneMasked with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardLaneMaskedWith(uint32_t route, uint32_t position,
                                   uint32_t lane, Slices slices, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9600 TrackRoute::StepForwardLaneMasked");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  if (MaskList(masks).first() == 0) {
    return 0;
  }
  uint32_t mask = masks;
  for (uint32_t m = 0;;) {
    const FirstBranch first = FirstBranchMatching(route, section, mask);
    if (int32_t(first.count) > 0) {
      // The matching link's lanes, added up over the links before it: the
      // first match taken when the lane is within them or it's the only one.
      int32_t lanes = 0;
      for (int32_t i = 0; i < 3; i++) {
        const uint32_t to = RLink(Sec(section).links_address() + 4 * uint32_t(i)).section();
        if (int16_t(to) < 0) {
          continue;
        }
        lanes += int32_t(FirstSliceLanes(Section(Route(route).file().address, to)) & 0xF);
        if (i != int32_t(int16_t(first.link))) {
          continue;
        }
        if (int16_t(Half(lane).value()) < lanes || !(int32_t(first.count) > 1)) {
          return TakeLane(route, position, lane, first.section, i, slices);
        }
      }
    }
    m++;
    mask = masks + 4 * m;
    if (MaskList(mask).first() == 0) {
      return 0;
    }
  }
}

// The lane normalizers' forward loop: while the slice is past the
// section's end, `step(count)` (a lane step with the section's slices'
// count); 1 once in a section, else `otherwise()`.
template <typename Step, typename Otherwise>
uint32_t ForwardByLane(uint32_t route, uint32_t position, Step step,
                       Otherwise otherwise) {
  for (;;) {
    const uint32_t section = Section(Route(route).file().address, Pos(position).section());
    const int32_t slice = int16_t(Pos(position).slice());
    const int32_t slices = int16_t(Sec(section).slice_count());
    if (slice < slices) {
      return 1;
    }
    if (step([slices] { return uint32_t(slices); }) == 0) {
      return otherwise();
    }
  }
}

// The lane normalizers' backward end: NormalizeBack, 1 when it did; else
// 0, the position (0, 0) unless it's on the route.
uint32_t BackOrClear(uint32_t route, uint32_t position) {
  if (NormalizeBack(route, position) != 0) {
    return 1;
  }
  if (!OnRoute(route, position)) {
    Pos(position).set_packed(0);
  }
  return 0;
}

// The point's offset across the road from the record's center (along its
// side direction): Locate's and LocateShoulder's (their steps the same).
// `half` = the width times 0.5.
struct Across {
  double offset;
  double half;
};
Across AcrossRoad(uint32_t point, uint32_t record) {
  const double dx = Fsubs(double(Vec(point).x()), double(Slice(record).center_x()));
  const double dz = Fsubs(double(Vec(point).z()), double(Slice(record).center_z()));
  const double side_x = double(Slice(record).side_x());
  const double half = Fmuls(double(Slice(record).width()), double(G::half()));
  const double side_z = double(Slice(record).side_z());
  return {Fmadds(dz, side_z, Fmuls(dx, side_x)), half};
}

}  // namespace

// --- Lookups ---

void LanePoint(uint32_t route, uint32_t out, uint32_t position, int32_t lane) {
  ITC_NATIVE_RUNNING("sub_820F8668 TrackRoute::LanePoint");
  const uint32_t file = Route(route).file().address;
  const uint32_t section = Section(file, Pos(position).section());
  const uint32_t record = Record(section, int16_t(Pos(position).slice()));
  const int32_t lanes = int32_t(Slice(record).lanes() & 0xF);
  if (!(lane < lanes)) {
    lane = lanes - 1;
  }
  const double width = double(Slice(record).width());
  const double side_x = double(Slice(record).side_x());
  const double center_x = double(Slice(record).center_x());
  const double half = double(G::half());
  const double one = Lfs(kOne);
  const double n = steps::ToSingle(lanes);
  const double l = steps::ToSingle(lane);
  const double offset = Fmuls(Fdivs(Fnmsubs(Fsubs(n, one), half, l), n), width);
  Vec(out).set_x(float(Fmadds(side_x, offset, center_x)));
  Vec(out).set_y(float(double(Slice(record).center_y())));
  const double side_z = double(Slice(record).side_z());
  const double center_z = double(Slice(record).center_z());
  const double z = Fmadds(side_z, offset, center_z);
  Vec(out).set_w(float(one));
  Vec(out).set_z(float(z));
}

uint32_t LaneAt(uint32_t route, uint32_t point, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8920 TrackRoute::LaneAt");
  const uint32_t file = Route(route).file().address;
  const uint32_t section = Section(file, Pos(position).section());
  const uint32_t record = Record(section, int16_t(Pos(position).slice()));
  const int32_t lanes = int32_t(Slice(record).lanes() & 0xF);
  if (lanes <= 1) {
    return 0;
  }
  const double across_x = Fsubs(double(Vec(point).x()), double(Slice(record).center_x()));
  const double across_z = Fsubs(double(Vec(point).z()), double(Slice(record).center_z()));
  const double along = Fmadds(across_z, double(Slice(record).side_z()),
                              Fmuls(across_x, double(Slice(record).side_x())));
  const double t = Fadds(Fdivs(along, double(Slice(record).width())), double(G::half()));
  if (!(t > Lfs(kZero))) {
    return 0;
  }
  if (!(t < Lfs(kOne))) {
    return uint32_t(lanes - 1);
  }
  return uint32_t(steps::Fctiwz(Fmuls(steps::ToSingle(lanes), t)));
}

int32_t Link(uint32_t route, uint32_t section, uint32_t link) {
  ITC_NATIVE_RUNNING("sub_820F89F0 TrackRoute::Link");
  const uint32_t file = Route(route).file().address;
  const uint32_t number = section & 0xFFFF;
  const uint32_t index = link & 0xFFFF;
  const uint32_t fallback = index >= 3 ? 3 : 0;
  const uint32_t s = Section(file, number);
  int32_t result = int32_t(number);
  const int32_t first = int16_t(RLink(Sec(s).links_address() + index * 4).section());
  if (first != -1) {
    result = first;
  } else {
    const int32_t second = int16_t(RLink(Sec(s).links_address() + fallback * 4).section());
    if (second != -1) {
      result = second;
    }
  }
  return int16_t(result);
}

int32_t LinkLane(uint32_t route, uint32_t position, uint32_t link_address,
                 uint32_t lane_address) {
  return LinkLaneWith(route, position, int16_t(Half(link_address).value()), lane_address);
}

int32_t SpecialSection(uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8D38 TrackRoute::SpecialSection");
  const uint32_t number = Pos(position).section();
  int32_t index;
  switch (int16_t(number)) {
    case 293:
      index = 0;
      break;
    case 230:
      index = 1;
      break;
    case 156:
      index = 2;
      break;
    case 20:
      index = 3;
      break;
    case 39:
      index = 4;
      break;
    case 77:
      index = 5;
      break;
    default:
      return -1;
  }
  const uint32_t section = Section(Route(route).file().address, number);
  if (Kind(section) != 4) {
    return -1;
  }
  const uint32_t record = Record(section, int16_t(Pos(position).slice()));
  return Slice(record).kind() == 4 ? index : -1;
}

uint32_t SliceTest(uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8DE8 TrackRoute::SliceTest");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  const int32_t kind = int32_t(Kind(section));
  if (kind <= 0) {
    return 0;
  }
  const uint32_t record = Record(section, int16_t(Pos(position).slice()));
  if (kind <= 2) {
    return Slice(record).mesh_section() == Slice(Sec(section).slices_address()).mesh_section() ? 1 : 0;
  }
  if (kind == 3) {
    return Slice(record).kind() == 3 ? 0 : 1;
  }
  if (kind == 4) {
    return Slice(record).kind() == 4 ? 1 : 0;
  }
  return 0;
}

uint32_t IsMainRoad(uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8E98 TrackRoute::IsMainRoad");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  if (Kind(section) - 1 <= 3) {
    return 0;
  }
  const int32_t branch = int32_t(Sec(section).branch());
  if (branch < 7) {
    return 1;
  }
  if (branch <= 8 || branch == 29) {
    return 0;
  }
  return 1;
}

uint32_t SectionOutcode(uint32_t route, uint32_t point, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_820F8EF0 TrackRoute::SectionOutcode");
  const uint32_t s = Section(Route(route).file().address, section);
  const double margin = double(G::bounds_margin());
  uint32_t code = 0;
  const double x = double(Vec(point).x());
  if (x < Fsubs(double(Sec(s).min_x()), margin)) {
    code = 1;
  } else if (x > Fadds(double(Sec(s).max_x()), margin)) {
    code = 2;
  }
  const double y = double(Vec(point).y());
  if (y < Fsubs(double(Sec(s).min_y()), margin)) {
    code |= 4;
  } else if (y > Fadds(double(Sec(s).max_y()), margin)) {
    code |= 8;
  }
  const double z = double(Vec(point).z());
  if (z < Fsubs(double(Sec(s).min_z()), margin)) {
    code |= 16;
  } else if (z > Fadds(double(Sec(s).max_z()), margin)) {
    code |= 32;
  }
  return code;
}

// --- Steps between sections ---

uint32_t StepForward(uint32_t route, uint32_t position, uint32_t count) {
  return StepForwardWith(route, position, WordAt(count));
}

uint32_t BranchesMatching(uint32_t route, uint32_t out, uint32_t section,
                          uint32_t mask) {
  return BranchesMatchingWith(route, section, mask, [&](uint32_t to, uint32_t i) {
    Branch(out).set_section(uint16_t(to));
    Branch(out).set_link(uint16_t(i));
    out += 4;
  });
}

uint32_t StepAlongPath(uint32_t route, uint32_t position, uint32_t count,
                       uint32_t path) {
  return StepAlongPathWith(route, position, WordAt(count), path);
}

uint32_t StepForwardMasked(uint32_t route, uint32_t position, uint32_t count,
                           uint32_t masks) {
  return StepForwardMaskedWith(route, position, WordAt(count), masks);
}

uint32_t NormalizeForward(uint32_t route, uint32_t position, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9EA8 TrackRoute::NormalizeForward");
  for (;;) {
    const uint32_t section = Section(Route(route).file().address, Pos(position).section());
    const int32_t slice = int16_t(Pos(position).slice());
    const int32_t slices = int16_t(Sec(section).slice_count());
    if (slice < slices) {
      return 1;
    }
    const auto count = [slices] { return uint32_t(slices); };
    const uint32_t stepped = masks == 0
                                 ? StepForwardWith(route, position, count)
                                 : StepForwardMaskedWith(route, position, count, masks);
    if (stepped == 0) {
      if (Pos(position).section() < 0x8000) {
        Pos(position).set_slice(uint16_t(slices - 1));
      } else {
        Pos(position).set_packed(0);
      }
      return 0;
    }
  }
}

uint32_t StepForwardLane(uint32_t route, uint32_t position, uint32_t lane,
                         uint32_t count) {
  return StepForwardLaneWith(route, position, lane, WordAt(count));
}

uint32_t StepForwardLaneMasked(uint32_t route, uint32_t position, uint32_t lane,
                               uint32_t count, uint32_t masks) {
  return StepForwardLaneMaskedWith(route, position, lane, WordAt(count), masks);
}

uint32_t StepBack(uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8A58 TrackRoute::StepBack");
  const uint32_t table = Route(route).file().sections();
  const uint32_t section = SectionIn(table, Pos(position).section());
  for (int32_t i = 5; i >= 3; i--) {
    const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(link).section();
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 3) {
      const uint32_t s = SectionIn(table, to);
      const uint32_t kind = Kind(s);
      if (kind == 1 || kind == 3) {
        continue;
      }
      const uint32_t branch = Sec(s).branch();
      if (branch == 7 || branch == 13 || branch == 29) {
        continue;
      }
      uint32_t back = 0;
      for (uint32_t j = 0; j < 3; j++) {
        if (RLink(Sec(s).links_address() + 4 * (3 + j)).section() != 0xFFFF) {
          back++;
        }
      }
      if (back == 0) {
        continue;
      }
    }
    Pos(position).set_section(uint16_t(RLink(link).section()));
    const uint32_t s = Section(Route(route).file().address, RLink(link).section());
    const uint32_t slice = Pos(position).slice();
    Pos(position).set_slice(uint16_t(Sec(s).slice_count() + slice));
    return 1;
  }
  return 0;
}

uint32_t NormalizeBack(uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F9320 TrackRoute::NormalizeBack");
  while (Pos(position).slice() >= 0x8000) {
    if (StepBack(route, position) == 0) {
      if (Pos(position).section() < 0x8000) {
        Pos(position).set_slice(0);
      } else {
        Pos(position).set_packed(0);
      }
      return 0;
    }
  }
  return 1;
}

uint32_t Normalize(uint32_t route, uint32_t position, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9FE8 TrackRoute::Normalize");
  return NormalizeWith(route, position,
                       [&] { return NormalizeForward(route, position, masks); });
}

uint32_t NormalizeForwardPath(uint32_t route, uint32_t position, uint32_t path,
                              uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9F58 TrackRoute::NormalizeForwardPath");
  if (path != 0) {
    for (;;) {
      const uint32_t section = Section(Route(route).file().address, Pos(position).section());
      const int32_t slice = int16_t(Pos(position).slice());
      const int32_t slices = int16_t(Sec(section).slice_count());
      if (slice < slices) {
        return 1;
      }
      const auto count = [slices] { return uint32_t(slices); };
      if (StepAlongPathWith(route, position, count, path) == 0 &&
          StepForwardWith(route, position, count) == 0) {
        break;
      }
    }
  }
  return NormalizeForward(route, position, masks);
}

uint32_t NormalizePath(uint32_t route, uint32_t position, uint32_t path,
                       uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820FA0F8 TrackRoute::NormalizePath");
  return NormalizeWith(route, position, [&] {
    return NormalizeForwardPath(route, position, path, masks);
  });
}

uint32_t NormalizeLane(uint32_t route, uint32_t position, uint32_t lane) {
  ITC_NATIVE_RUNNING("sub_820FA210 TrackRoute::NormalizeLane");
  if (OnRoute(route, position)) {
    return 1;
  }
  if (Pos(position).slice() >= 0x8000) {
    return BackOrClear(route, position);
  }
  return ForwardByLane(route, position,
      [&](auto slices) { return StepForwardLaneWith(route, position, lane, slices); },
      [&] { return Normalize(route, position, 0); });
}

uint32_t NormalizeLaneMasked(uint32_t route, uint32_t position, uint32_t lane,
                             uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820FA350 TrackRoute::NormalizeLaneMasked");
  if (OnRoute(route, position)) {
    return 1;
  }
  const int32_t section = int16_t(Pos(position).section());
  if (section < 0 || !(section < int32_t(Route(route).file().section_count()))) {
    Pos(position).set_packed(0);
    return 0;
  }
  if (Pos(position).slice() >= 0x8000) {
    return BackOrClear(route, position);
  }
  return ForwardByLane(route, position,
      [&](auto slices) {
        return StepForwardLaneMaskedWith(route, position, lane, slices, masks);
      },
      [&] { return NormalizeLane(route, position, lane); });
}

uint32_t Point(uint32_t route, uint32_t out, uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA860 TrackRoute::Point");
  const uint32_t copy = frame + 80;
  guest::StoreU32(copy, Pos(position).packed());
  const uint32_t valid = Normalize(route, copy, 0);
  const uint32_t section = LoadU16(copy);
  if (valid != 0) {
    const double one = Lfs(kOne);
    const int32_t slice = LoadS16(copy + 2);
    const uint32_t record = Record(Section(Route(route).file().address, section), slice);
    Vec(out).set_x(float(double(Slice(record).center_x())));
    Vec(out).set_y(float(double(Slice(record).center_y())));
    Vec(out).set_z(float(double(Slice(record).center_z())));
    Vec(out).set_w(float(one));
  }
  return section;
}

uint32_t AlongSlice(uint32_t route, uint32_t point, uint32_t position,
                    uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FAE78 TrackRoute::AlongSlice");
  const double across_x = Fsubs(double(Vec(point).x()), double(Slice(record).center_x()));
  const double forward_x = double(Slice(record).forward_x());
  const double z = double(Vec(point).z());
  const uint32_t file = Route(route).file().address;
  const double forward_z = double(Slice(record).forward_z());
  const double along_x = Fmuls(across_x, forward_x);
  const double across_z = Fsubs(z, double(Slice(record).center_z()));
  const double length = double(File(file).slice_length());
  const double along = Fmadds(across_z, forward_z, along_x);
  if (!(along < Fneg(length)) && !(along > length)) {
    return 0;
  }
  // The next slice, normalized.
  const uint32_t copy = frame + 80;
  const uint32_t section = Pos(position).section();
  const uint32_t slice = Pos(position).slice();
  guest::StoreU16(copy + 0, uint16_t(section));
  guest::StoreU16(copy + 2, uint16_t(slice + 1));
  if (along < Lfs(kZero)) {
    return 2;
  }
  if (Normalize(route, copy, 0) == 0) {
    return 1;
  }
  const uint32_t next = Record(Section(Route(route).file().address, LoadU16(copy)),
                               LoadS16(copy + 2));
  if (next != 0) {
    return 1;
  }
  // A null record (never: what follows reads address 8 and 0, as the
  // original does).
  const double dz = Fsubs(Lfs(8), double(Slice(record).center_z()));
  const double squared = Fmuls(along, along);
  const double dx = Fsubs(Lfs(0), double(Slice(record).center_x()));
  const double distance = Fmadds(dx, dx, Fmuls(dz, dz));
  return squared < distance ? 0 : 1;
}

uint32_t Locate(uint32_t route, uint32_t point, uint32_t position,
                uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FAF88 TrackRoute::Locate");
  const uint32_t along = AlongSlice(route, point, position, record, frame - 128);
  if (along != 0) {
    return along;
  }
  const Across a = AcrossRoad(point, record);
  if (a.offset < Fsubs(Fneg(double(Slice(record).left_margin())), a.half)) {
    return 5;
  }
  if (a.offset > Fadds(double(Slice(record).right_margin()), a.half)) {
    return 6;
  }
  return 0;
}

uint32_t LocateShoulder(uint32_t route, uint32_t point, uint32_t position,
                        uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB030 TrackRoute::LocateShoulder");
  const uint32_t along = AlongSlice(route, point, position, record, frame - 128);
  if (along != 0) {
    return along;
  }
  const Across a = AcrossRoad(point, record);
  if (a.offset < Fsubs(Fneg(double(Slice(record).left_margin())), a.half)) {
    return 5;
  }
  if (a.offset > Fadds(double(Slice(record).right_margin()), a.half)) {
    return 6;
  }
  const double shoulder = Fadds(a.half, double(G::half()));
  if (a.offset < Fneg(shoulder)) {
    return 3;
  }
  if (a.offset > shoulder) {
    return 4;
  }
  return 0;
}

uint32_t LocateAt(uint32_t route, uint32_t point, uint32_t position,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB888 TrackRoute::LocateAt");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  const uint32_t record = Record(section, int16_t(Pos(position).slice()));
  return LocateShoulder(route, point, position, record, frame);
}

uint32_t ToMainRoad(uint32_t route, uint32_t position, uint32_t flag,
                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA728 TrackRoute::ToMainRoad");
  const uint32_t copy = frame + 80;
  // Forward: up to 10 sections (the slice put past each section's end, then
  // normalized with the default branch mask), counting the slices moved.
  guest::StoreU32(copy, Pos(position).packed());
  int32_t moved = 0;
  bool found = false;
  for (int32_t tries = 10;;) {
    if (IsMainRoad(route, copy) != 0) {
      found = true;
      break;
    }
    if (--tries < 0) {
      break;
    }
    const int32_t slice = LoadS16(copy + 2);
    const uint32_t slices =
        Sec(Section(Route(route).file().address, LoadU16(copy))).slice_count();
    guest::StoreU16(copy + 2, uint16_t(slices));
    moved += int32_t(int16_t(slices)) - slice;
    if (Normalize(route, copy, G::default_branch_mask_address()) == 0) {
      break;
    }
  }
  if (!found) {
    // Back: up to 10 sections (the slice put before each section's start).
    guest::StoreU32(copy, Pos(position).packed());
    moved = 0;
    for (int32_t tries = 10;;) {
      if (IsMainRoad(route, copy) != 0) {
        break;
      }
      if (--tries < 0) {
        return 0;
      }
      moved = moved - LoadS16(copy + 2) - 1;
      guest::StoreU16(copy + 2, 0xFFFF);
      if (Normalize(route, copy, G::default_branch_mask_address()) == 0) {
        return 0;
      }
    }
  }
  if (int32_t(flag) != 0) {
    guest::StoreU16(copy + 2, uint16_t(LoadU16(copy + 2) - uint32_t(moved)));
  }
  const uint32_t result = Normalize(route, copy, G::default_branch_mask_address());
  Pos(position).set_packed(Load(copy));
  return result;
}

uint32_t NoteShoulder(uint32_t route, uint32_t point, uint32_t position,
                      uint32_t best, uint32_t distance, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB100 TrackRoute::NoteShoulder");
  const uint32_t section = Section(Route(route).file().address, Pos(position).section());
  const uint32_t record = Record(section, int16_t(Pos(position).slice()));
  const uint32_t where = LocateShoulder(route, point, position, record, frame - 112);
  if (where < 3 || where > 4) {
    return where;
  }
  const double dy = Fsubs(double(Vec(point).y()), double(Slice(record).center_y()));
  const double dx = Fsubs(double(Vec(point).x()), double(Slice(record).center_x()));
  const double dz = Fsubs(double(Vec(point).z()), double(Slice(record).center_z()));
  const double nearest = double(Flt(distance).value());
  const double squared = Fmadds(dz, dz, Fmadds(dx, dx, Fmuls(dy, dy)));
  if (squared < nearest) {
    Pos(best).set_section(uint16_t(Pos(position).section()));
    Pos(best).set_slice(uint16_t(Pos(position).slice()));
    Flt(distance).set_value(float(squared));
  }
  return where;
}

uint32_t TakeBranch(uint32_t route, uint32_t position, uint32_t which) {
  ITC_NATIVE_RUNNING("sub_820FA4A8 TrackRoute::TakeBranch");
  if (OnRoute(route, position)) {
    return 1;
  }
  const uint32_t table = Route(route).file().sections();
  const int32_t slice = int16_t(Pos(position).slice());
  const uint32_t section = SectionIn(table, Pos(position).section());
  int32_t counted = 0;
  // Whether this is the n-th link (the word at `which`), else counted.
  const auto nth = [&] {
    if (!(counted < int32_t(Word(which).value()))) {
      return true;
    }
    counted++;
    return false;
  };
  if (slice >= 0) {
    // Past the end: forward links 2, 1, 0 (2 and 1 not to kinds 2 or 4),
    // then 2 and 1 to kinds 2 and 4; the slice less the section's count.
    const auto take = [&](uint32_t link) {
      Pos(position).set_section(uint16_t(RLink(link).section()));
      Pos(position).set_slice(uint16_t(uint32_t(slice) - Sec(section).slice_count()));
      return Normalize(route, position, 0);
    };
    for (int32_t i = 2; i >= 0; i--) {
      const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
      const uint32_t to = RLink(link).section();
      if (to == 0xFFFF) {
        continue;
      }
      if (i > 0) {
        const uint32_t kind = Kind(SectionIn(table, to));
        if (kind == 2 || kind == 4) {
          continue;
        }
      }
      if (nth()) {
        return take(link);
      }
    }
    for (int32_t i = 2; i >= 1; i--) {
      const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
      const uint32_t to = RLink(link).section();
      if (to == 0xFFFF) {
        continue;
      }
      const uint32_t kind = Kind(SectionIn(table, to));
      if (kind != 2 && kind != 4) {
        continue;
      }
      if (nth()) {
        return take(link);
      }
    }
    return 0;
  }
  // Before the start: back links 5, 4, 3 (5 and 4 to sections not of kinds 1
  // or 3 with a back link of their own), then 5 and 4 to kinds 1 and 3; the
  // slice plus the new section's count.
  const auto take = [&](uint32_t link) {
    Pos(position).set_section(uint16_t(RLink(link).section()));
    const uint32_t to = Section(Route(route).file().address, RLink(link).section());
    Pos(position).set_slice(uint16_t(Sec(to).slice_count() + uint32_t(slice)));
    return Normalize(route, position, 0);
  };
  for (int32_t i = 5; i >= 3; i--) {
    const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(link).section();
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 3) {
      const uint32_t s = SectionIn(table, to);
      const uint32_t kind = Kind(s);
      if (kind == 1 || kind == 3) {
        continue;
      }
      uint32_t back = 0;
      for (uint32_t j = 0; j < 3; j++) {
        if (RLink(Sec(s).links_address() + 4 * (3 + j)).section() != 0xFFFF) {
          back++;
        }
      }
      if (back == 0) {
        continue;
      }
    }
    if (nth()) {
      return take(link);
    }
  }
  for (int32_t i = 5; i >= 4; i--) {
    const uint32_t link = Sec(section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(link).section();
    if (to == 0xFFFF) {
      continue;
    }
    const uint32_t kind = Kind(SectionIn(table, to));
    if (kind != 1 && kind != 3) {
      continue;
    }
    if (nth()) {
      return take(link);
    }
  }
  return 0;
}

void FullSearch(uint32_t route, uint32_t out, uint32_t point, uint32_t section,
                uint32_t before, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB2B0 TrackRoute::FullSearch");
  const uint32_t position = frame + 80;
  const uint32_t best = frame + 84;
  const uint32_t score = frame + 88;
  double nearest = double(G::farthest());
  Stfs(score, nearest);
  guest::StoreU16(best + 2, 0);
  guest::StoreU16(best + 0, 0xFFFF);
  const double reach = double(G::reach_squared());
  const int32_t given = int16_t(section);
  const auto table = [&] { return Route(route).file().sections(); };
  // Every slice of the section `s` (its number at `position`), the nearest
  // on it kept (OnSlice's score).
  const auto slices = [&](uint32_t s) {
    guest::StoreU16(position + 2, 0);
    if (!(int16_t(Sec(s).slice_count()) > 0)) {
      return;
    }
    int32_t slice = 0;
    do {
      if (OnSlice(route, point, position, score, frame - 128) == 1) {
        const double value = Lfs(score);
        if (value < nearest) {
          nearest = value;
          guest::StoreU32(best, Load(position));
        }
      }
      slice = int16_t(slice + 1);
      guest::StoreU16(position + 2, uint16_t(slice));
    } while (slice < int16_t(Sec(s).slice_count()));
  };
  // The sections the links of `s` lead to, within their bounds. `follow`:
  // the original's register for `s` taking each such section (its next
  // links read from there).
  const auto links = [&](uint32_t s, bool follow) {
    for (uint32_t i = 0; i < 12; i++) {
      const uint32_t to = RLink(Sec(s).links_address() + 4 * i).section();
      guest::StoreU16(position, uint16_t(to));
      if (int16_t(to) == -1 || SectionOutcode(route, point, to) != 0) {
        continue;
      }
      const uint32_t next = SectionIn(table(), to);
      if (follow) {
        s = next;
      }
      slices(next);
    }
  };
  const auto found = [&] { return nearest < reach && LoadU16(best) != 0xFFFF; };
  // The given section's links.
  bool done = false;
  if (given != -1) {
    links(SectionIn(table(), section), false);
    done = found();
  }
  // The section before and its links.
  const int32_t previous = int16_t(before);
  if (!done && previous != -1 && previous != given) {
    guest::StoreU16(position, uint16_t(before));
    const uint32_t s = SectionIn(table(), before);
    if (SectionOutcode(route, point, before) == 0) {
      slices(s);
    }
    links(s, true);
    done = found();
  }
  if (!done) {
    // Every section of kind 0x30 and up (the whole byte) within its bounds.
    uint32_t file = Route(route).file().address;
    guest::StoreU16(position, 0);
    for (int32_t i = 0; i < int32_t(File(file).section_count());) {
      const uint32_t s = SectionIn(File(file).sections(), uint32_t(i));
      if (!(Sec(s).kind() < 48) && SectionOutcode(route, point, uint32_t(i)) == 0) {
        slices(s);
      }
      file = Route(route).file().address;
      i = int16_t(i + 1);
      guest::StoreU16(position, uint16_t(i));
    }
    if (LoadU16(best) == 0xFFFF) {
      // None: the out position if whole, else the section before with
      // out's slice, else (0, 0).
      if (LoadU16(out + 0) != 0xFFFF && LoadU16(out + 2) != 0xFFFF) {
        guest::StoreU32(best, Load(out));
      } else if (previous != -1 && LoadU16(out + 2) != 0xFFFF) {
        guest::StoreU16(best + 0, uint16_t(before));
        guest::StoreU16(best + 2, uint16_t(LoadU16(out + 2)));
      } else {
        guest::StoreU16(best + 0, 0);
        guest::StoreU16(best + 2, 0);
      }
    }
  }
  guest::StoreU32(out, Load(best));
}

void Follow(uint32_t route, uint32_t position, uint32_t point, uint32_t before,
            uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB680 TrackRoute::Follow");
  const uint32_t copy = frame + 80;
  const uint32_t best = frame + 84;
  const uint32_t distance = frame + 88;
  const uint32_t which = frame + 92;
  guest::StoreU32(copy, Pos(position).packed());
  uint32_t section = LoadU16(copy);
  Stfs(distance, double(G::no_distance()));
  guest::StoreU16(best + 0, 0xFFFF);
  guest::StoreU16(best + 2, 0xFFFF);
  int32_t moved = 0;
  uint32_t branch = 0;
  if (int16_t(section) >= 0) {
    uint32_t slice = LoadU16(copy + 2);
    if (int16_t(slice) >= 0) {
      for (int32_t steps_left = 5;;) {
        const uint32_t where =
            NoteShoulder(route, point, copy, best, distance, frame - 128);
        if (where == 0) {
          Pos(position).set_packed(Load(copy));
          return;
        }
        if (where == 1 || where == 2) {
          // Past or before the slice: the next or previous one.
          const int32_t step = where == 1 ? 1 : -1;
          guest::StoreU16(copy + 2, uint16_t(slice + uint32_t(step)));
          moved += step;
          Normalize(route, copy, 0);
        } else if (where >= 7) {
          steps_left = 0;
        } else {
          const uint32_t first = Pos(position).section();
          if (int16_t(section) == int16_t(first)) {
            // Beside the road in the first section: the section beside it
            // (link 6 on the left, 8 on the right).
            const uint32_t s = SectionIn(Route(route).file().sections(), section);
            const uint32_t link = (where == 3 || where == 5) ? 6 : 8;
            const uint32_t to = RLink(Sec(s).links_address() + link * 4).section();
            if (to == 0xFFFF) {
              steps_left = 0;
            } else {
              guest::StoreU16(copy, uint16_t(to));
              Normalize(route, copy, 0);
            }
          } else {
            // Beside the road in another section: the first section's
            // next branch, from its slice plus the slices moved.
            const uint32_t from = Pos(position).slice();
            branch++;
            guest::StoreU16(copy, uint16_t(first));
            guest::StoreU32(which, branch);
            guest::StoreU16(copy + 2, uint16_t(from + uint32_t(moved)));
            if (TakeBranch(route, copy, which) == 0) {
              steps_left = 0;
            }
          }
        }
        if (--steps_left < 0) {
          break;
        }
        slice = LoadU16(copy + 2);
        section = LoadU16(copy);
      }
    }
  }
  // No slice found: the shoulder kept, else the full search, taken when its
  // slice's center is within 10 m in height.
  if (LoadS16(best + 0) >= 0 && LoadS16(best + 2) >= 0) {
    Pos(position).set_packed(Load(best));
    return;
  }
  const uint32_t word = Pos(position).packed();
  const uint32_t previous = Pos(before).section();
  const uint32_t current = Pos(position).section();
  guest::StoreU32(copy, word);
  FullSearch(route, copy, point, current, previous, frame - 192);
  const uint32_t found = LoadU16(copy);
  if (int16_t(found) < 0) {
    return;
  }
  const int32_t slice = LoadS16(copy + 2);
  if (slice < 0) {
    return;
  }
  const uint32_t record = Record(Section(Route(route).file().address, found), slice);
  if (record == 0) {
    return;
  }
  const double dy = Fsubs(double(Slice(record).center_y()), double(Vec(point).y()));
  if (!(Fmuls(dy, dy) < double(G::height_reach()))) {
    return;
  }
  Pos(position).set_packed(Load(copy));
}

uint32_t OnSlice(uint32_t route, uint32_t point, uint32_t position, uint32_t score,
                 uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB1B8 TrackRoute::OnSlice");
  const uint32_t number = Pos(position).section();
  if (SectionOutcode(route, point, number) != 0) {
    return 0;
  }
  const int32_t slice = int16_t(Pos(position).slice());
  const uint32_t section = Section(Route(route).file().address, number);
  const uint32_t record = Record(section, slice);
  if (Locate(route, point, position, record, frame - 112) != 0) {
    return 0;
  }
  if (score == 0) {
    return 1;
  }
  const double dy = Fsubs(double(Vec(point).y()), double(Slice(record).center_y()));
  double value = Fmuls(dy, dy);
  if (value < double(G::nine())) {
    value = Lfs(kZero);
  }
  if (Kind(section) - 1 <= 3) {
    value = Fadds(value, double(G::ten()));
  }
  const double dz = Fsubs(double(Vec(point).z()), double(Slice(record).center_z()));
  const double dx = Fsubs(double(Vec(point).x()), double(Slice(record).center_x()));
  Flt(score).set_value(float(Fadds(Fmadds(dx, dx, Fmuls(dz, dz)), value)));
  return 1;
}

// --- The linear index ---

uint32_t ToIndex(uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_820AB408 TrackRoute::ToIndex");
  const uint32_t file = Route(kRoute).file().address;
  const int32_t section = int16_t(packed >> 16);
  int32_t last;
  if (section < 0) {
    last = 0;
  } else {
    last = int32_t(File(file).section_count()) - 1;
    if (!(section > last)) {
      last = section;
    }
  }
  uint32_t index = uint32_t(int32_t(int16_t(packed)));
  if (!(section < int32_t(File(file).section_count()))) {
    index += uint32_t(int16_t(Sec(Section(file, uint32_t(last))).slice_count()));
  }
  if (last > 0) {
    const uint32_t table = File(file).sections();
    for (int32_t i = 0; i < last; i++) {
      index += uint32_t(int16_t(Sec(SectionIn(table, uint32_t(i))).slice_count()));
    }
  }
  return index;
}

uint32_t FromIndex(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820AB498 TrackRoute::FromIndex");
  const uint32_t file = Route(kRoute).file().address;
  const int32_t sections = int32_t(File(file).section_count());
  int32_t i = 0;
  uint32_t table = 0;
  bool inside = false;
  if (sections > 0) {
    table = File(file).sections();
    do {
      const int32_t slices = int16_t(Sec(SectionIn(table, uint32_t(i))).slice_count());
      if (int32_t(index) < slices) {
        break;
      }
      i++;
      index -= uint32_t(slices);
    } while (i < sections);
    inside = i < sections;
  }
  if (!inside) {
    table = File(file).sections();
    index += uint32_t(int16_t(Sec(SectionIn(table, uint32_t(sections - 1))).slice_count()));
  }
  int32_t section;
  if (i < 0) {
    section = 0;
  } else {
    section = sections - 1;
    if (!(i > section)) {
      section = i;
    }
  }
  section = int16_t(section);
  int32_t slice;
  if (int32_t(index) < 0) {
    slice = 0;
  } else {
    const int32_t last = int16_t(Sec(SectionIn(table, uint32_t(section))).slice_count()) - 1;
    slice = int32_t(index) > last ? last : int32_t(index);
  }
  return (uint32_t(uint16_t(section)) << 16) | uint16_t(slice);
}

void ClampPosition(uint32_t out, uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_820F8420 TrackRoute::ClampPosition");
  const uint32_t clamped = FromIndex(ToIndex(packed));
  Pos(out).set_section(uint16_t(clamped >> 16));
  Pos(out).set_slice(uint16_t(clamped));
}

// --- Distances ---

bool LinkDistance(uint32_t route, uint32_t a, uint32_t b, float& distance,
                  int32_t depth) {
  ITC_NATIVE_RUNNING("sub_820F8720 TrackRoute::LinkDistance");
  const uint32_t file = Route(route).file().address;
  const uint32_t table = File(file).sections();
  const uint32_t from = SectionIn(table, Pos(b).section());
  const int32_t target = int16_t(Pos(a).section());
  for (uint32_t i = 0; i < 12; i++) {
    if (int16_t(RLink(Sec(from).links_address() + 4 * i).section()) != target) {
      continue;
    }
    // b's section links straight to a's: the slices over the link back.
    const uint32_t to = SectionIn(table, RLink(Sec(from).links_address() + 4 * i).section());
    const int32_t back = int16_t(Pos(b).section());
    const int32_t slices = int32_t(uint32_t(int16_t(RLink(Sec(from).links_address() + 4 * i).slice())) -
                                   uint32_t(int16_t(Pos(a).slice())));
    for (int32_t m = 0; m < 12; m++) {
      if (int16_t(RLink(Sec(to).links_address() + 4 * uint32_t(m)).section()) == back) {
        const int32_t d = LinkSlices(slices, to, m);
        distance = float(Fmadds(double(File(file).slice_length()), steps::ToSingle(d), double(distance)));
        return true;
      }
    }
    return false;
  }
  if (depth - 1 < 0) {
    return false;
  }
  for (uint32_t j = 0; j < 12; j++) {
    const uint32_t link = Sec(from).links_address() + 4 * j;
    const uint32_t via = RLink(link).section();
    if (int16_t(via) == -1 || int16_t(Pos(b).section()) == int16_t(via)) {
      continue;
    }
    const int32_t back = int16_t(Pos(b).section());
    const uint32_t f = Route(route).file().address;
    const double so_far = double(distance);
    const int32_t slices = int16_t(RLink(link).slice());
    const uint32_t next = Section(f, via);
    for (int32_t k = 0; k < 12; k++) {
      if (int16_t(RLink(Sec(next).links_address() + 4 * uint32_t(k)).section()) != back) {
        continue;
      }
      const int32_t d = LinkSlices(slices, from, k);
      float further = float(Fmadds(double(File(f).slice_length()), steps::ToSingle(d), so_far));
      if (LinkDistance(route, a, link, further, depth - 1)) {
        distance = float(double(further));
        return true;
      }
      break;
    }
  }
  return false;
}

double Distance(uint32_t route, uint32_t a, uint32_t b, bool* linked) {
  ITC_NATIVE_RUNNING("sub_820F9278 TrackRoute::Distance");
  if (Pos(a).section() == Pos(b).section()) {
    const int32_t from = int16_t(Pos(a).slice());
    const int32_t to = int16_t(Pos(b).slice());
    const uint32_t file = Route(route).file().address;
    return Fmuls(steps::ToSingle(int32_t(uint32_t(to) - uint32_t(from))), double(File(file).slice_length()));
  }
  const int32_t to = int16_t(Pos(b).slice());
  const uint32_t file = Route(route).file().address;
  float distance = float(Fmuls(steps::ToSingle(to), double(File(file).slice_length())));
  const bool found = LinkDistance(route, a, b, distance, 2);
  if (linked != nullptr) {
    *linked = found;
  }
  return found ? double(distance) : double(G::no_distance());
}

// --- Distances along a path ---

namespace {

// fsqrts: the double's square root rounded to single.
double Fsqrts(double a) {
  return double(float(std::sqrt(a)));
}

// PathDistance's length of a difference: the vector at `from` (as lvx
// reads it, 16-aligned) dotted with itself (vmsum3fp128, flush-to-zero on),
// stored in all four words at `to` (as stvx, 16-aligned), its first word's
// square root.
double LengthThrough(uint32_t from, uint32_t to) {
  const vector_library::Vector v = vector_library::LoadVector(from & ~0xFu);
  const uint32_t dot = std::bit_cast<uint32_t>(vector_library::Dot(v, v));
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32((to & ~0xFu) + i, dot);
  }
  return Fsqrts(Lfs(to));
}

// PathDistance's difference of the point at `frame` + 112 (P) and a slice's
// center (the record at `record`), into `frame` + 96; with `keep`, the center into
// `frame` + 128 (Q) first, as the original stores it.
void Difference(uint32_t frame, uint32_t record, bool keep) {
  for (uint32_t i = 0; i < 12; i += 4) {
    const double p = Lfs(frame + 112 + i);
    const double c = Lfs(record + i);
    if (keep) {
      Stfs(frame + 128 + i, c);
    }
    Stfs(frame + 96 + i, Fsubs(p, c));
  }
}

}  // namespace

double PathDistance(uint32_t route, uint32_t a, uint32_t b, uint32_t path,
                    int32_t length, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA9D0 TrackRoute::PathDistance");
  const uint32_t at = frame + 80;    // the position stepped
  const uint32_t from = frame + 84;  // the last part's start
  double sum = Lfs(kZero);
  const double one = Lfs(kOne);
  Stfs(frame + 140, one);
  Stfs(frame + 108, one);
  const int32_t to_section = int16_t(b >> 16);
  // The path's last entry that is b's section (its index; 0 when none).
  int32_t last = 0;
  for (int32_t i = int16_t(length - 1); i >= 0; i = int16_t(i - 1)) {
    if (to_section == int16_t(Path(path + 2 * uint32_t(i)).first())) {
      last = i;
      break;
    }
  }
  const auto table = [&] { return Route(route).file().sections(); };
  // One slice along: the slice at `position` + 2 moved by `step`,
  // normalized; the distance from P to its center added (`dot_at` the
  // original's temporary), and P = the center. Returns the slice; `section`
  // its section, `offset` the slice times 84.
  const auto step = [&](uint32_t position, int32_t slice, int32_t delta, uint32_t dot_at,
                        uint32_t& section, uint32_t& offset) {
    slice += delta;
    guest::StoreU16(position + 2, uint16_t(slice));
    Normalize(route, position, 0);
    section = LoadU16(position);
    const uint32_t s = SectionIn(table(), section);
    slice = LoadS16(position + 2);
    offset = uint32_t(slice) * 84;
    Difference(frame, Sec(s).slices_address() + offset, true);
    // P = Q (its 16 bytes, read before the dot's store as the original).
    uint8_t center[16];
    std::memcpy(center, guest::Pointer(frame + 128), 16);
    const double length_of = LengthThrough(frame + 96, dot_at);
    std::memcpy(guest::Pointer(frame + 112), center, 16);
    sum = Fadds(length_of, sum);
    return slice;
  };
  guest::StoreU32(at, a);
  if (length > 1) {
    int32_t slice = int16_t(a);
    uint32_t section = LoadU16(at);
    uint32_t offset = uint32_t(slice) * 84;
    for (int32_t k = 1; k < length; k++) {
      const uint32_t next = Path(path + 2 * uint32_t(k)).first();
      const uint32_t current = SectionIn(table(), section);
      const uint32_t following = SectionIn(table(), next);
      // P: the slice's center.
      for (uint32_t i = 0; i < 12; i += 4) {
        Stfs(frame + 112 + i, Lfs(Sec(current).slices_address() + offset + i));
      }
      bool linked = false;
      // The next section's link back to this one: its slice here, stepped
      // to a slice at a time.
      for (uint32_t j = 0; j < 12; j++) {
        if (int16_t(RLink(Sec(following).links_address() + 4 * j).section()) != int16_t(section)) {
          continue;
        }
        const int32_t target = int16_t(RLink(Sec(following).links_address() + 4 * j).slice());
        if (slice < target) {
          Stfs(frame + 140, one);
          do {
            slice = step(at, slice, 1, frame + 144, section, offset);
          } while (slice < target);
        } else if (slice > target) {
          Stfs(frame + 140, one);
          do {
            slice = step(at, slice, -1, frame + 160, section, offset);
          } while (slice > target);
        }
        linked = true;
        break;
      }
      if (linked) {
        // This section's link to the next: over it, the distance from P to
        // the slice it leads to (P not moved).
        for (uint32_t m = 0; m < 12; m++) {
          const RLink link(Sec(current).links_address() + 4 * m);
          if (int16_t(link.section()) != int16_t(next)) {
            continue;
          }
          section = next;
          Stfs(frame + 140, one);
          guest::StoreU16(at, uint16_t(section));
          const uint32_t there = link.slice();
          guest::StoreU16(at + 2, uint16_t(there));
          slice = int16_t(there);
          offset = uint32_t(slice) * 84;
          Difference(frame, Sec(SectionIn(table(), next)).slices_address() + offset, false);
          sum = Fadds(LengthThrough(frame + 96, frame + 176), sum);
          break;
        }
      }
      if (int16_t(section) == to_section && k == last) {
        break;
      }
    }
  }
  // The last part, from the lesser slice to the greater, in the start's
  // section.
  uint32_t end = b;
  if (int16_t(b) < LoadS16(at + 2)) {
    guest::StoreU32(from, b);
    end = Load(at);
  } else {
    guest::StoreU32(from, Load(at));
  }
  uint32_t section = LoadU16(from);
  int32_t slice = LoadS16(from + 2);
  const int32_t target = int16_t(end);
  uint32_t offset = uint32_t(slice) * 84;
  const uint32_t start = Sec(SectionIn(table(), section)).slices_address() + offset;
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(frame + 112 + i, Lfs(start + i));
  }
  if (slice == target) {
    return sum;
  }
  Stfs(frame + 140, one);
  do {
    slice = step(from, slice, 1, frame + 176, section, offset);
  } while (slice != target);
  return sum;
}

// --- Points of a slice ---

bool SlicePoint(uint32_t out, uint32_t which, uint32_t section, uint32_t slice) {
  ITC_NATIVE_RUNNING("sub_820F9768 TrackRoute::SlicePoint");
  if (which >= 5 && which <= 16) {
    return false;
  }
  const uint32_t s = Section(Route(kRoute).file().address, section);
  const uint32_t slices = Sec(s).slices_address();
  const Slice record(slices + slice * Slice::kSize);
  const double cx = double(record.center_x());
  const double cy = double(record.center_y());
  const double cz = double(record.center_z());
  const uint32_t count = uint32_t(int16_t(Sec(s).slice_count()));
  // The two point tables after the records (probably the road's edges).
  const Vec first(slices + count * 84 + slice * 12);
  const Vec second(slices + count * 96 + slice * 12);
  const double ax = double(first.x());
  const double ay = double(first.y());
  const double az = double(first.z());
  const double bx = double(second.x());
  const double by = double(second.y());
  const double bz = double(second.z());
  switch (which) {
    case 0:
    case 3:
      Vec(out).set_x(float(cx));
      Vec(out).set_y(float(cy));
      Vec(out).set_z(float(cz));
      break;
    case 1:
      Vec(out).set_x(float(ax));
      Vec(out).set_y(float(ay));
      Vec(out).set_z(float(az));
      break;
    case 2:
      Vec(out).set_x(float(bx));
      Vec(out).set_y(float(by));
      Vec(out).set_z(float(bz));
      break;
    case 4: {
      const double half = double(G::half());
      const double x = Fadds(cx, cx), y = Fadds(cy, cy), z = Fadds(cz, cz);
      Vec(out).set_x(float(Fmuls(x, half)));
      const double hy = Fmuls(y, half), hz = Fmuls(z, half);
      Vec(out).set_y(float(hy));
      Vec(out).set_z(float(hz));
      break;
    }
    default:  // above 16: nothing
      break;
  }
  return true;
}

}  // namespace track_route
