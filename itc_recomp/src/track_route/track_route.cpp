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

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
uint32_t LoadU16(uint8_t* base, uint32_t address) {
  return guest::LoadU16(base, address);
}
int32_t LoadS16(uint8_t* base, uint32_t address) {
  return int16_t(guest::LoadU16(base, address));
}
uint32_t LoadU8(uint8_t* base, uint32_t address) {
  return guest::LoadU8(base, address);
}

// The table's section `number` (its low 16 bits).
uint32_t SectionIn(uint8_t* base, uint32_t table, uint32_t number) {
  return Load(base, table + (number & 0xFFFF) * 4);
}
// The section of a route's file.
uint32_t Section(uint8_t* base, uint32_t file, uint32_t number) {
  return SectionIn(base, File(base, file).sections(), number);
}
// A slice's record (the 32-bit wrap of the original's 64-bit sum).
uint32_t Record(uint8_t* base, uint32_t section, int32_t slice) {
  return Sec(base, section).slices_address() + uint32_t(slice) * Slice::kSize;
}
uint32_t Kind(uint8_t* base, uint32_t section) {
  return Sec(base, section).kind() & 0xF;
}
// The lanes' count byte of the section's first slice (the section's +180).
uint32_t FirstSliceLanes(uint8_t* base, uint32_t section) {
  return Slice(base, Sec(base, section).slices_address()).lanes();
}

// A position moved over the link at `link` (StepForward's and
// StepAlongPath's ends, and the lane steps'): the section from the link, the
// slice less the slices' count (`slices()`, read when the original reads its
// word), in the original's order.
template <typename Slices>
uint32_t TakeForward(uint8_t* base, uint32_t position, uint32_t link, Slices slices) {
  const uint32_t section = RLink(base, link).section();
  const uint32_t slice = Pos(base, position).slice();
  Pos(base, position).set_section(uint16_t(section));
  Pos(base, position).set_slice(uint16_t(slice - slices()));
  return 1;
}
// The word at `address`, read when called.
auto WordAt(uint8_t* base, uint32_t address) {
  return [base, address] { return Load(base, address); };
}

// StepForward with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardWith(uint8_t* base, uint32_t route, uint32_t position, Slices slices) {
  ITC_NATIVE_RUNNING("sub_820F8B48 TrackRoute::StepForward");
  const uint32_t table = Route(base, route).file().sections();
  const uint32_t section = SectionIn(base, table, Pos(base, position).section());
  for (int32_t i = 2; i >= 0; i--) {
    const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(base, link).section();
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 0) {
      const uint32_t kind = Kind(base, SectionIn(base, table, to));
      if (kind == 2 || kind == 4) {
        continue;
      }
    }
    return TakeForward(base, position, link, slices);
  }
  return 0;
}

// BranchesMatching, each match handed to `found(section, link index)`.
template <typename Found>
uint32_t BranchesMatchingWith(uint8_t* base, uint32_t route, uint32_t section, uint32_t mask,
                              Found found) {
  ITC_NATIVE_RUNNING("sub_820F8BD0 TrackRoute::BranchesMatching");
  uint32_t count = 0;
  for (uint32_t i = 0; i <= 2; i++) {
    const uint32_t to = RLink(base, Sec(base, section).links_address() + 4 * i).section();
    if (to == 0xFFFF) {
      continue;
    }
    const uint32_t s = Section(base, Route(base, route).file().address, to);
    const uint32_t kind = Kind(base, s);
    if (kind == 2 || kind == 4) {
      continue;
    }
    const uint32_t branch = Sec(base, s).branch();
    const uint32_t bits = Word(base, mask).value();
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
FirstBranch FirstBranchMatching(uint8_t* base, uint32_t route, uint32_t section, uint32_t mask) {
  FirstBranch first;
  bool any = false;
  first.count = BranchesMatchingWith(base, route, section, mask, [&](uint32_t to, uint32_t i) {
    if (!any) {
      any = true;
      first.section = to;
      first.link = i;
    }
  });
  return first;
}

// LinkLane with the link index as a value.
int32_t LinkLaneWith(uint8_t* base, uint32_t route, uint32_t position, int32_t link,
                     uint32_t lane_address) {
  ITC_NATIVE_RUNNING("sub_820F8CE8 TrackRoute::LinkLane");
  const uint32_t file = Route(base, route).file().address;
  const uint32_t number = Pos(base, position).section();
  const uint32_t section = Section(base, file, number);
  if (RLink(base, Sec(base, section).links_address() + uint32_t(link) * 4).section() == 0xFFFF) {
    return -1;
  }
  const int32_t lane = int16_t(Half(base, lane_address).value());
  return int8_t(LoadU8(base, Sec(base, section).link_lanes_address() + uint32_t(link) * 8 + uint32_t(lane)));
}

// A link's slice difference (LinkDistance): `from` less the slice of link
// `index` of `section`, plus 1 for a forward link, less 1 for a back one.
int32_t LinkSlices(uint8_t* base, int32_t from, uint32_t section, int32_t index) {
  int32_t d = int32_t(uint32_t(from) -
                      uint32_t(int16_t(RLink(base, Sec(base, section).links_address() + 4 * uint32_t(index)).slice())));
  if (index < 3) {
    d++;
  } else if (index < 6) {
    d--;
  }
  return d;
}

// StepForwardMasked with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardMaskedWith(uint8_t* base, uint32_t route, uint32_t position, Slices slices,
                               uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F93A0 TrackRoute::StepForwardMasked");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  if (MaskList(base, masks).first() != 0) {
    uint32_t mask = masks;
    for (uint32_t m = 0;;) {
      const FirstBranch first = FirstBranchMatching(base, route, section, mask);
      if (int32_t(first.count) > 0) {
        const uint32_t slice = Pos(base, position).slice();
        Pos(base, position).set_section(uint16_t(first.section));
        Pos(base, position).set_slice(uint16_t(slice - slices()));
        return 1;
      }
      m++;
      mask = masks + 4 * m;
      if (MaskList(base, mask).first() == 0) {
        break;
      }
    }
  }
  return StepForwardWith(base, route, position, slices);
}

// The lane steps' end: the position over the link (`link` its index) to
// the section `to`, the lane carried over the link (LinkLane, from the new
// section's lane bytes as the original reads them), kept below the new
// section's lanes; none: the last lane for link 0, else 0.
template <typename Slices>
uint32_t TakeLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane_address,
                  uint32_t to, int32_t link, Slices slices) {
  const uint32_t slice = Pos(base, position).slice();
  Pos(base, position).set_section(uint16_t(to));
  Pos(base, position).set_slice(uint16_t(slice - slices()));
  const int32_t carried = LinkLaneWith(base, route, position, link, lane_address);
  Half(base, lane_address).set_value(uint16_t(carried));
  const int32_t lanes =
      int32_t(FirstSliceLanes(base, Section(base, Route(base, route).file().address, Pos(base, position).section())) & 0xF);
  const int32_t lane = int16_t(carried);
  if (lane < 0) {
    Half(base, lane_address).set_value(uint16_t(link == 0 ? lanes - 1 : 0));
  } else if (!(lane < lanes)) {
    Half(base, lane_address).set_value(uint16_t(lanes - 1));
  }
  return 1;
}

// StepForwardLane with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardLaneWith(uint8_t* base, uint32_t route, uint32_t position,
                             uint32_t lane_address, Slices slices) {
  ITC_NATIVE_RUNNING("sub_820F9448 TrackRoute::StepForwardLane");
  const uint32_t file = Route(base, route).file().address;
  const uint32_t table = File(base, file).sections();
  const uint32_t section = SectionIn(base, table, Pos(base, position).section());
  // The forward links whose lanes, added up in link order, hold the lane
  // (every linked section's lanes added; kinds 1, 3 and 4 not taken).
  int32_t lanes = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t link = Sec(base, section).links_address() + 4 * i;
    const uint32_t to = RLink(base, link).section();
    if (int16_t(to) < 0) {
      continue;
    }
    const uint32_t s = SectionIn(base, File(base, file).sections(), to);
    lanes += int32_t(FirstSliceLanes(base, s) & 0xF);
    const int32_t kind = int32_t(Kind(base, s));
    if (kind == 1 || (kind > 2 && kind <= 4)) {
      continue;
    }
    if (int16_t(Half(base, lane_address).value()) < lanes) {
      return TakeLane(base, route, position, lane_address, RLink(base, link).section(), int32_t(i), slices);
    }
  }
  // Else links 2, 1, 0 to a section not of kinds 1, 3 or 4, onto its last
  // lane (0 when it has none).
  for (int32_t i = 2; i >= 0; i--) {
    const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(base, link).section();
    if (int16_t(to) < 0) {
      continue;
    }
    const uint32_t kind = Kind(base, SectionIn(base, table, to));
    if (kind == 1 || kind - 3 <= 1) {
      continue;
    }
    const uint32_t next = RLink(base, link).section();
    TakeForward(base, position, link, slices);
    const int32_t last = int16_t(
        int32_t(FirstSliceLanes(base, Section(base, Route(base, route).file().address, next)) & 0xF) - 1);
    Half(base, lane_address).set_value(uint16_t(last));
    if (last < 0) {
      Half(base, lane_address).set_value(0);
    }
    return 1;
  }
  return 0;
}

// StepAlongPath with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepAlongPathWith(uint8_t* base, uint32_t route, uint32_t position, Slices slices,
                           uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820F8C58 TrackRoute::StepAlongPath");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  while (Path(base, path).first() < 0x8000) {
    for (uint32_t i = 0; i <= 2; i++) {
      const uint32_t link = Sec(base, section).links_address() + 4 * i;
      const int32_t to = int16_t(RLink(base, link).section());
      if (to != -1 && to == int16_t(Path(base, path).first())) {
        return TakeForward(base, position, link, slices);
      }
    }
    path += 2;
  }
  return 0;
}

// Whether the position is on the route: its section within the file's
// count, its slice within the section's.
bool OnRoute(uint8_t* base, uint32_t route, uint32_t position) {
  const uint32_t number = Pos(base, position).section();
  const int32_t section = int16_t(number);
  if (section < 0) {
    return false;
  }
  const uint32_t file = Route(base, route).file().address;
  if (!(int32_t(File(base, file).section_count()) > section)) {
    return false;
  }
  const int32_t slice = int16_t(Pos(base, position).slice());
  if (slice < 0) {
    return false;
  }
  return int16_t(Sec(base, Section(base, file, number)).slice_count()) > slice;
}

// Normalize and NormalizePath around their forward step: on the route, 1;
// a section off the route, (0, 0) and 0; else NormalizeBack then `forward()`,
// 1 when both did; else 0, the position (0, 0) unless it's on the route.
template <typename Forward>
uint32_t NormalizeWith(uint8_t* base, uint32_t route, uint32_t position, Forward forward) {
  if (OnRoute(base, route, position)) {
    return 1;
  }
  const int32_t section = int16_t(Pos(base, position).section());
  if (section >= 0 && section < int32_t(Route(base, route).file().section_count())) {
    if (NormalizeBack(base, route, position) != 0 && forward() != 0) {
      return 1;
    }
    if (OnRoute(base, route, position)) {
      return 0;
    }
  }
  Pos(base, position).set_packed(0);
  return 0;
}

// StepForwardLaneMasked with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardLaneMaskedWith(uint8_t* base, uint32_t route, uint32_t position,
                                   uint32_t lane, Slices slices, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9600 TrackRoute::StepForwardLaneMasked");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  if (MaskList(base, masks).first() == 0) {
    return 0;
  }
  uint32_t mask = masks;
  for (uint32_t m = 0;;) {
    const FirstBranch first = FirstBranchMatching(base, route, section, mask);
    if (int32_t(first.count) > 0) {
      // The matching link's lanes, added up over the links before it: the
      // first match taken when the lane is within them or it's the only one.
      int32_t lanes = 0;
      for (int32_t i = 0; i < 3; i++) {
        const uint32_t to = RLink(base, Sec(base, section).links_address() + 4 * uint32_t(i)).section();
        if (int16_t(to) < 0) {
          continue;
        }
        lanes += int32_t(FirstSliceLanes(base, Section(base, Route(base, route).file().address, to)) & 0xF);
        if (i != int32_t(int16_t(first.link))) {
          continue;
        }
        if (int16_t(Half(base, lane).value()) < lanes || !(int32_t(first.count) > 1)) {
          return TakeLane(base, route, position, lane, first.section, i, slices);
        }
      }
    }
    m++;
    mask = masks + 4 * m;
    if (MaskList(base, mask).first() == 0) {
      return 0;
    }
  }
}

// The lane normalizers' forward loop: while the slice is past the
// section's end, `step(count)` (a lane step with the section's slices'
// count); 1 once in a section, else `otherwise()`.
template <typename Step, typename Otherwise>
uint32_t ForwardByLane(uint8_t* base, uint32_t route, uint32_t position, Step step,
                       Otherwise otherwise) {
  for (;;) {
    const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
    const int32_t slice = int16_t(Pos(base, position).slice());
    const int32_t slices = int16_t(Sec(base, section).slice_count());
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
uint32_t BackOrClear(uint8_t* base, uint32_t route, uint32_t position) {
  if (NormalizeBack(base, route, position) != 0) {
    return 1;
  }
  if (!OnRoute(base, route, position)) {
    Pos(base, position).set_packed(0);
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
Across AcrossRoad(uint8_t* base, uint32_t point, uint32_t record) {
  const double dx = Fsubs(double(Vec(base, point).x()), double(Slice(base, record).center_x()));
  const double dz = Fsubs(double(Vec(base, point).z()), double(Slice(base, record).center_z()));
  const double side_x = double(Slice(base, record).side_x());
  const double half = Fmuls(double(Slice(base, record).width()), double(G::half(base)));
  const double side_z = double(Slice(base, record).side_z());
  return {Fmadds(dz, side_z, Fmuls(dx, side_x)), half};
}

}  // namespace

// --- Lookups ---

void LanePoint(uint8_t* base, uint32_t route, uint32_t out, uint32_t position, int32_t lane) {
  ITC_NATIVE_RUNNING("sub_820F8668 TrackRoute::LanePoint");
  const uint32_t file = Route(base, route).file().address;
  const uint32_t section = Section(base, file, Pos(base, position).section());
  const uint32_t record = Record(base, section, int16_t(Pos(base, position).slice()));
  const int32_t lanes = int32_t(Slice(base, record).lanes() & 0xF);
  if (!(lane < lanes)) {
    lane = lanes - 1;
  }
  const double width = double(Slice(base, record).width());
  const double side_x = double(Slice(base, record).side_x());
  const double center_x = double(Slice(base, record).center_x());
  const double half = double(G::half(base));
  const double one = Lfs(base, kOne);
  const double n = steps::ToSingle(lanes);
  const double l = steps::ToSingle(lane);
  const double offset = Fmuls(Fdivs(Fnmsubs(Fsubs(n, one), half, l), n), width);
  Vec(base, out).set_x(float(Fmadds(side_x, offset, center_x)));
  Vec(base, out).set_y(float(double(Slice(base, record).center_y())));
  const double side_z = double(Slice(base, record).side_z());
  const double center_z = double(Slice(base, record).center_z());
  const double z = Fmadds(side_z, offset, center_z);
  Vec(base, out).set_w(float(one));
  Vec(base, out).set_z(float(z));
}

uint32_t LaneAt(uint8_t* base, uint32_t route, uint32_t point, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8920 TrackRoute::LaneAt");
  const uint32_t file = Route(base, route).file().address;
  const uint32_t section = Section(base, file, Pos(base, position).section());
  const uint32_t record = Record(base, section, int16_t(Pos(base, position).slice()));
  const int32_t lanes = int32_t(Slice(base, record).lanes() & 0xF);
  if (lanes <= 1) {
    return 0;
  }
  const double across_x = Fsubs(double(Vec(base, point).x()), double(Slice(base, record).center_x()));
  const double across_z = Fsubs(double(Vec(base, point).z()), double(Slice(base, record).center_z()));
  const double along = Fmadds(across_z, double(Slice(base, record).side_z()),
                              Fmuls(across_x, double(Slice(base, record).side_x())));
  const double t = Fadds(Fdivs(along, double(Slice(base, record).width())), double(G::half(base)));
  if (!(t > Lfs(base, kZero))) {
    return 0;
  }
  if (!(t < Lfs(base, kOne))) {
    return uint32_t(lanes - 1);
  }
  return uint32_t(steps::Fctiwz(Fmuls(steps::ToSingle(lanes), t)));
}

int32_t Link(uint8_t* base, uint32_t route, uint32_t section, uint32_t link) {
  ITC_NATIVE_RUNNING("sub_820F89F0 TrackRoute::Link");
  const uint32_t file = Route(base, route).file().address;
  const uint32_t number = section & 0xFFFF;
  const uint32_t index = link & 0xFFFF;
  const uint32_t fallback = index >= 3 ? 3 : 0;
  const uint32_t s = Section(base, file, number);
  int32_t result = int32_t(number);
  const int32_t first = int16_t(RLink(base, Sec(base, s).links_address() + index * 4).section());
  if (first != -1) {
    result = first;
  } else {
    const int32_t second = int16_t(RLink(base, Sec(base, s).links_address() + fallback * 4).section());
    if (second != -1) {
      result = second;
    }
  }
  return int16_t(result);
}

int32_t LinkLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t link_address,
                 uint32_t lane_address) {
  return LinkLaneWith(base, route, position, int16_t(Half(base, link_address).value()), lane_address);
}

int32_t SpecialSection(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8D38 TrackRoute::SpecialSection");
  const uint32_t number = Pos(base, position).section();
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
  const uint32_t section = Section(base, Route(base, route).file().address, number);
  if (Kind(base, section) != 4) {
    return -1;
  }
  const uint32_t record = Record(base, section, int16_t(Pos(base, position).slice()));
  return Slice(base, record).kind() == 4 ? index : -1;
}

uint32_t SliceTest(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8DE8 TrackRoute::SliceTest");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  const int32_t kind = int32_t(Kind(base, section));
  if (kind <= 0) {
    return 0;
  }
  const uint32_t record = Record(base, section, int16_t(Pos(base, position).slice()));
  if (kind <= 2) {
    return Slice(base, record).mesh_section() == Slice(base, Sec(base, section).slices_address()).mesh_section() ? 1 : 0;
  }
  if (kind == 3) {
    return Slice(base, record).kind() == 3 ? 0 : 1;
  }
  if (kind == 4) {
    return Slice(base, record).kind() == 4 ? 1 : 0;
  }
  return 0;
}

uint32_t IsMainRoad(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8E98 TrackRoute::IsMainRoad");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  if (Kind(base, section) - 1 <= 3) {
    return 0;
  }
  const int32_t branch = int32_t(Sec(base, section).branch());
  if (branch < 7) {
    return 1;
  }
  if (branch <= 8 || branch == 29) {
    return 0;
  }
  return 1;
}

uint32_t SectionOutcode(uint8_t* base, uint32_t route, uint32_t point, uint32_t section) {
  ITC_NATIVE_RUNNING("sub_820F8EF0 TrackRoute::SectionOutcode");
  const uint32_t s = Section(base, Route(base, route).file().address, section);
  const double margin = double(G::bounds_margin(base));
  uint32_t code = 0;
  const double x = double(Vec(base, point).x());
  if (x < Fsubs(double(Sec(base, s).min_x()), margin)) {
    code = 1;
  } else if (x > Fadds(double(Sec(base, s).max_x()), margin)) {
    code = 2;
  }
  const double y = double(Vec(base, point).y());
  if (y < Fsubs(double(Sec(base, s).min_y()), margin)) {
    code |= 4;
  } else if (y > Fadds(double(Sec(base, s).max_y()), margin)) {
    code |= 8;
  }
  const double z = double(Vec(base, point).z());
  if (z < Fsubs(double(Sec(base, s).min_z()), margin)) {
    code |= 16;
  } else if (z > Fadds(double(Sec(base, s).max_z()), margin)) {
    code |= 32;
  }
  return code;
}

// --- Steps between sections ---

uint32_t StepForward(uint8_t* base, uint32_t route, uint32_t position, uint32_t count) {
  return StepForwardWith(base, route, position, WordAt(base, count));
}

uint32_t BranchesMatching(uint8_t* base, uint32_t route, uint32_t out, uint32_t section,
                          uint32_t mask) {
  return BranchesMatchingWith(base, route, section, mask, [&](uint32_t to, uint32_t i) {
    Branch(base, out).set_section(uint16_t(to));
    Branch(base, out).set_link(uint16_t(i));
    out += 4;
  });
}

uint32_t StepAlongPath(uint8_t* base, uint32_t route, uint32_t position, uint32_t count,
                       uint32_t path) {
  return StepAlongPathWith(base, route, position, WordAt(base, count), path);
}

uint32_t StepForwardMasked(uint8_t* base, uint32_t route, uint32_t position, uint32_t count,
                           uint32_t masks) {
  return StepForwardMaskedWith(base, route, position, WordAt(base, count), masks);
}

uint32_t NormalizeForward(uint8_t* base, uint32_t route, uint32_t position, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9EA8 TrackRoute::NormalizeForward");
  for (;;) {
    const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
    const int32_t slice = int16_t(Pos(base, position).slice());
    const int32_t slices = int16_t(Sec(base, section).slice_count());
    if (slice < slices) {
      return 1;
    }
    const auto count = [slices] { return uint32_t(slices); };
    const uint32_t stepped = masks == 0
                                 ? StepForwardWith(base, route, position, count)
                                 : StepForwardMaskedWith(base, route, position, count, masks);
    if (stepped == 0) {
      if (Pos(base, position).section() < 0x8000) {
        Pos(base, position).set_slice(uint16_t(slices - 1));
      } else {
        Pos(base, position).set_packed(0);
      }
      return 0;
    }
  }
}

uint32_t StepForwardLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane,
                         uint32_t count) {
  return StepForwardLaneWith(base, route, position, lane, WordAt(base, count));
}

uint32_t StepForwardLaneMasked(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane,
                               uint32_t count, uint32_t masks) {
  return StepForwardLaneMaskedWith(base, route, position, lane, WordAt(base, count), masks);
}

uint32_t StepBack(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8A58 TrackRoute::StepBack");
  const uint32_t table = Route(base, route).file().sections();
  const uint32_t section = SectionIn(base, table, Pos(base, position).section());
  for (int32_t i = 5; i >= 3; i--) {
    const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(base, link).section();
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 3) {
      const uint32_t s = SectionIn(base, table, to);
      const uint32_t kind = Kind(base, s);
      if (kind == 1 || kind == 3) {
        continue;
      }
      const uint32_t branch = Sec(base, s).branch();
      if (branch == 7 || branch == 13 || branch == 29) {
        continue;
      }
      uint32_t back = 0;
      for (uint32_t j = 0; j < 3; j++) {
        if (RLink(base, Sec(base, s).links_address() + 4 * (3 + j)).section() != 0xFFFF) {
          back++;
        }
      }
      if (back == 0) {
        continue;
      }
    }
    Pos(base, position).set_section(uint16_t(RLink(base, link).section()));
    const uint32_t s = Section(base, Route(base, route).file().address, RLink(base, link).section());
    const uint32_t slice = Pos(base, position).slice();
    Pos(base, position).set_slice(uint16_t(Sec(base, s).slice_count() + slice));
    return 1;
  }
  return 0;
}

uint32_t NormalizeBack(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F9320 TrackRoute::NormalizeBack");
  while (Pos(base, position).slice() >= 0x8000) {
    if (StepBack(base, route, position) == 0) {
      if (Pos(base, position).section() < 0x8000) {
        Pos(base, position).set_slice(0);
      } else {
        Pos(base, position).set_packed(0);
      }
      return 0;
    }
  }
  return 1;
}

uint32_t Normalize(uint8_t* base, uint32_t route, uint32_t position, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9FE8 TrackRoute::Normalize");
  return NormalizeWith(base, route, position,
                       [&] { return NormalizeForward(base, route, position, masks); });
}

uint32_t NormalizeForwardPath(uint8_t* base, uint32_t route, uint32_t position, uint32_t path,
                              uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9F58 TrackRoute::NormalizeForwardPath");
  if (path != 0) {
    for (;;) {
      const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
      const int32_t slice = int16_t(Pos(base, position).slice());
      const int32_t slices = int16_t(Sec(base, section).slice_count());
      if (slice < slices) {
        return 1;
      }
      const auto count = [slices] { return uint32_t(slices); };
      if (StepAlongPathWith(base, route, position, count, path) == 0 &&
          StepForwardWith(base, route, position, count) == 0) {
        break;
      }
    }
  }
  return NormalizeForward(base, route, position, masks);
}

uint32_t NormalizePath(uint8_t* base, uint32_t route, uint32_t position, uint32_t path,
                       uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820FA0F8 TrackRoute::NormalizePath");
  return NormalizeWith(base, route, position, [&] {
    return NormalizeForwardPath(base, route, position, path, masks);
  });
}

uint32_t NormalizeLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane) {
  ITC_NATIVE_RUNNING("sub_820FA210 TrackRoute::NormalizeLane");
  if (OnRoute(base, route, position)) {
    return 1;
  }
  if (Pos(base, position).slice() >= 0x8000) {
    return BackOrClear(base, route, position);
  }
  return ForwardByLane(
      base, route, position,
      [&](auto slices) { return StepForwardLaneWith(base, route, position, lane, slices); },
      [&] { return Normalize(base, route, position, 0); });
}

uint32_t NormalizeLaneMasked(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane,
                             uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820FA350 TrackRoute::NormalizeLaneMasked");
  if (OnRoute(base, route, position)) {
    return 1;
  }
  const int32_t section = int16_t(Pos(base, position).section());
  if (section < 0 || !(section < int32_t(Route(base, route).file().section_count()))) {
    Pos(base, position).set_packed(0);
    return 0;
  }
  if (Pos(base, position).slice() >= 0x8000) {
    return BackOrClear(base, route, position);
  }
  return ForwardByLane(
      base, route, position,
      [&](auto slices) {
        return StepForwardLaneMaskedWith(base, route, position, lane, slices, masks);
      },
      [&] { return NormalizeLane(base, route, position, lane); });
}

uint32_t Point(uint8_t* base, uint32_t route, uint32_t out, uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA860 TrackRoute::Point");
  const uint32_t copy = frame + 80;
  guest::StoreU32(base, copy, Pos(base, position).packed());
  const uint32_t valid = Normalize(base, route, copy, 0);
  const uint32_t section = LoadU16(base, copy);
  if (valid != 0) {
    const double one = Lfs(base, kOne);
    const int32_t slice = LoadS16(base, copy + 2);
    const uint32_t record = Record(base, Section(base, Route(base, route).file().address, section), slice);
    Vec(base, out).set_x(float(double(Slice(base, record).center_x())));
    Vec(base, out).set_y(float(double(Slice(base, record).center_y())));
    Vec(base, out).set_z(float(double(Slice(base, record).center_z())));
    Vec(base, out).set_w(float(one));
  }
  return section;
}

uint32_t AlongSlice(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                    uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FAE78 TrackRoute::AlongSlice");
  const double across_x = Fsubs(double(Vec(base, point).x()), double(Slice(base, record).center_x()));
  const double forward_x = double(Slice(base, record).forward_x());
  const double z = double(Vec(base, point).z());
  const uint32_t file = Route(base, route).file().address;
  const double forward_z = double(Slice(base, record).forward_z());
  const double along_x = Fmuls(across_x, forward_x);
  const double across_z = Fsubs(z, double(Slice(base, record).center_z()));
  const double length = double(File(base, file).slice_length());
  const double along = Fmadds(across_z, forward_z, along_x);
  if (!(along < Fneg(length)) && !(along > length)) {
    return 0;
  }
  // The next slice, normalized.
  const uint32_t copy = frame + 80;
  const uint32_t section = Pos(base, position).section();
  const uint32_t slice = Pos(base, position).slice();
  guest::StoreU16(base, copy + 0, uint16_t(section));
  guest::StoreU16(base, copy + 2, uint16_t(slice + 1));
  if (along < Lfs(base, kZero)) {
    return 2;
  }
  if (Normalize(base, route, copy, 0) == 0) {
    return 1;
  }
  const uint32_t next = Record(base, Section(base, Route(base, route).file().address, LoadU16(base, copy)),
                               LoadS16(base, copy + 2));
  if (next != 0) {
    return 1;
  }
  // A null record (never: what follows reads address 8 and 0, as the
  // original does).
  const double dz = Fsubs(Lfs(base, 8), double(Slice(base, record).center_z()));
  const double squared = Fmuls(along, along);
  const double dx = Fsubs(Lfs(base, 0), double(Slice(base, record).center_x()));
  const double distance = Fmadds(dx, dx, Fmuls(dz, dz));
  return squared < distance ? 0 : 1;
}

uint32_t Locate(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FAF88 TrackRoute::Locate");
  const uint32_t along = AlongSlice(base, route, point, position, record, frame - 128);
  if (along != 0) {
    return along;
  }
  const Across a = AcrossRoad(base, point, record);
  if (a.offset < Fsubs(Fneg(double(Slice(base, record).left_margin())), a.half)) {
    return 5;
  }
  if (a.offset > Fadds(double(Slice(base, record).right_margin()), a.half)) {
    return 6;
  }
  return 0;
}

uint32_t LocateShoulder(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                        uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB030 TrackRoute::LocateShoulder");
  const uint32_t along = AlongSlice(base, route, point, position, record, frame - 128);
  if (along != 0) {
    return along;
  }
  const Across a = AcrossRoad(base, point, record);
  if (a.offset < Fsubs(Fneg(double(Slice(base, record).left_margin())), a.half)) {
    return 5;
  }
  if (a.offset > Fadds(double(Slice(base, record).right_margin()), a.half)) {
    return 6;
  }
  const double shoulder = Fadds(a.half, double(G::half(base)));
  if (a.offset < Fneg(shoulder)) {
    return 3;
  }
  if (a.offset > shoulder) {
    return 4;
  }
  return 0;
}

uint32_t LocateAt(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                  uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB888 TrackRoute::LocateAt");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  const uint32_t record = Record(base, section, int16_t(Pos(base, position).slice()));
  return LocateShoulder(base, route, point, position, record, frame);
}

uint32_t ToMainRoad(uint8_t* base, uint32_t route, uint32_t position, uint32_t flag,
                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA728 TrackRoute::ToMainRoad");
  const uint32_t copy = frame + 80;
  // Forward: up to 10 sections (the slice put past each section's end, then
  // normalized with the default branch mask), counting the slices moved.
  guest::StoreU32(base, copy, Pos(base, position).packed());
  int32_t moved = 0;
  bool found = false;
  for (int32_t tries = 10;;) {
    if (IsMainRoad(base, route, copy) != 0) {
      found = true;
      break;
    }
    if (--tries < 0) {
      break;
    }
    const int32_t slice = LoadS16(base, copy + 2);
    const uint32_t slices =
        Sec(base, Section(base, Route(base, route).file().address, LoadU16(base, copy))).slice_count();
    guest::StoreU16(base, copy + 2, uint16_t(slices));
    moved += int32_t(int16_t(slices)) - slice;
    if (Normalize(base, route, copy, G::default_branch_mask_address()) == 0) {
      break;
    }
  }
  if (!found) {
    // Back: up to 10 sections (the slice put before each section's start).
    guest::StoreU32(base, copy, Pos(base, position).packed());
    moved = 0;
    for (int32_t tries = 10;;) {
      if (IsMainRoad(base, route, copy) != 0) {
        break;
      }
      if (--tries < 0) {
        return 0;
      }
      moved = moved - LoadS16(base, copy + 2) - 1;
      guest::StoreU16(base, copy + 2, 0xFFFF);
      if (Normalize(base, route, copy, G::default_branch_mask_address()) == 0) {
        return 0;
      }
    }
  }
  if (int32_t(flag) != 0) {
    guest::StoreU16(base, copy + 2, uint16_t(LoadU16(base, copy + 2) - uint32_t(moved)));
  }
  const uint32_t result = Normalize(base, route, copy, G::default_branch_mask_address());
  Pos(base, position).set_packed(Load(base, copy));
  return result;
}

uint32_t NoteShoulder(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                      uint32_t best, uint32_t distance, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB100 TrackRoute::NoteShoulder");
  const uint32_t section = Section(base, Route(base, route).file().address, Pos(base, position).section());
  const uint32_t record = Record(base, section, int16_t(Pos(base, position).slice()));
  const uint32_t where = LocateShoulder(base, route, point, position, record, frame - 112);
  if (where < 3 || where > 4) {
    return where;
  }
  const double dy = Fsubs(double(Vec(base, point).y()), double(Slice(base, record).center_y()));
  const double dx = Fsubs(double(Vec(base, point).x()), double(Slice(base, record).center_x()));
  const double dz = Fsubs(double(Vec(base, point).z()), double(Slice(base, record).center_z()));
  const double nearest = double(Flt(base, distance).value());
  const double squared = Fmadds(dz, dz, Fmadds(dx, dx, Fmuls(dy, dy)));
  if (squared < nearest) {
    Pos(base, best).set_section(uint16_t(Pos(base, position).section()));
    Pos(base, best).set_slice(uint16_t(Pos(base, position).slice()));
    Flt(base, distance).set_value(float(squared));
  }
  return where;
}

uint32_t TakeBranch(uint8_t* base, uint32_t route, uint32_t position, uint32_t which) {
  ITC_NATIVE_RUNNING("sub_820FA4A8 TrackRoute::TakeBranch");
  if (OnRoute(base, route, position)) {
    return 1;
  }
  const uint32_t table = Route(base, route).file().sections();
  const int32_t slice = int16_t(Pos(base, position).slice());
  const uint32_t section = SectionIn(base, table, Pos(base, position).section());
  int32_t counted = 0;
  // Whether this is the n-th link (the word at `which`), else counted.
  const auto nth = [&] {
    if (!(counted < int32_t(Word(base, which).value()))) {
      return true;
    }
    counted++;
    return false;
  };
  if (slice >= 0) {
    // Past the end: forward links 2, 1, 0 (2 and 1 not to kinds 2 or 4),
    // then 2 and 1 to kinds 2 and 4; the slice less the section's count.
    const auto take = [&](uint32_t link) {
      Pos(base, position).set_section(uint16_t(RLink(base, link).section()));
      Pos(base, position).set_slice(uint16_t(uint32_t(slice) - Sec(base, section).slice_count()));
      return Normalize(base, route, position, 0);
    };
    for (int32_t i = 2; i >= 0; i--) {
      const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
      const uint32_t to = RLink(base, link).section();
      if (to == 0xFFFF) {
        continue;
      }
      if (i > 0) {
        const uint32_t kind = Kind(base, SectionIn(base, table, to));
        if (kind == 2 || kind == 4) {
          continue;
        }
      }
      if (nth()) {
        return take(link);
      }
    }
    for (int32_t i = 2; i >= 1; i--) {
      const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
      const uint32_t to = RLink(base, link).section();
      if (to == 0xFFFF) {
        continue;
      }
      const uint32_t kind = Kind(base, SectionIn(base, table, to));
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
    Pos(base, position).set_section(uint16_t(RLink(base, link).section()));
    const uint32_t to = Section(base, Route(base, route).file().address, RLink(base, link).section());
    Pos(base, position).set_slice(uint16_t(Sec(base, to).slice_count() + uint32_t(slice)));
    return Normalize(base, route, position, 0);
  };
  for (int32_t i = 5; i >= 3; i--) {
    const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(base, link).section();
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 3) {
      const uint32_t s = SectionIn(base, table, to);
      const uint32_t kind = Kind(base, s);
      if (kind == 1 || kind == 3) {
        continue;
      }
      uint32_t back = 0;
      for (uint32_t j = 0; j < 3; j++) {
        if (RLink(base, Sec(base, s).links_address() + 4 * (3 + j)).section() != 0xFFFF) {
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
    const uint32_t link = Sec(base, section).links_address() + 4 * uint32_t(i);
    const uint32_t to = RLink(base, link).section();
    if (to == 0xFFFF) {
      continue;
    }
    const uint32_t kind = Kind(base, SectionIn(base, table, to));
    if (kind != 1 && kind != 3) {
      continue;
    }
    if (nth()) {
      return take(link);
    }
  }
  return 0;
}

void FullSearch(uint8_t* base, uint32_t route, uint32_t out, uint32_t point, uint32_t section,
                uint32_t before, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB2B0 TrackRoute::FullSearch");
  const uint32_t position = frame + 80;
  const uint32_t best = frame + 84;
  const uint32_t score = frame + 88;
  double nearest = double(G::farthest(base));
  Stfs(base, score, nearest);
  guest::StoreU16(base, best + 2, 0);
  guest::StoreU16(base, best + 0, 0xFFFF);
  const double reach = double(G::reach_squared(base));
  const int32_t given = int16_t(section);
  const auto table = [&] { return Route(base, route).file().sections(); };
  // Every slice of the section `s` (its number at `position`), the nearest
  // on it kept (OnSlice's score).
  const auto slices = [&](uint32_t s) {
    guest::StoreU16(base, position + 2, 0);
    if (!(int16_t(Sec(base, s).slice_count()) > 0)) {
      return;
    }
    int32_t slice = 0;
    do {
      if (OnSlice(base, route, point, position, score, frame - 128) == 1) {
        const double value = Lfs(base, score);
        if (value < nearest) {
          nearest = value;
          guest::StoreU32(base, best, Load(base, position));
        }
      }
      slice = int16_t(slice + 1);
      guest::StoreU16(base, position + 2, uint16_t(slice));
    } while (slice < int16_t(Sec(base, s).slice_count()));
  };
  // The sections the links of `s` lead to, within their bounds. `follow`:
  // the original's register for `s` taking each such section (its next
  // links read from there).
  const auto links = [&](uint32_t s, bool follow) {
    for (uint32_t i = 0; i < 12; i++) {
      const uint32_t to = RLink(base, Sec(base, s).links_address() + 4 * i).section();
      guest::StoreU16(base, position, uint16_t(to));
      if (int16_t(to) == -1 || SectionOutcode(base, route, point, to) != 0) {
        continue;
      }
      const uint32_t next = SectionIn(base, table(), to);
      if (follow) {
        s = next;
      }
      slices(next);
    }
  };
  const auto found = [&] { return nearest < reach && LoadU16(base, best) != 0xFFFF; };
  // The given section's links.
  bool done = false;
  if (given != -1) {
    links(SectionIn(base, table(), section), false);
    done = found();
  }
  // The section before and its links.
  const int32_t previous = int16_t(before);
  if (!done && previous != -1 && previous != given) {
    guest::StoreU16(base, position, uint16_t(before));
    const uint32_t s = SectionIn(base, table(), before);
    if (SectionOutcode(base, route, point, before) == 0) {
      slices(s);
    }
    links(s, true);
    done = found();
  }
  if (!done) {
    // Every section of kind 0x30 and up (the whole byte) within its bounds.
    uint32_t file = Route(base, route).file().address;
    guest::StoreU16(base, position, 0);
    for (int32_t i = 0; i < int32_t(File(base, file).section_count());) {
      const uint32_t s = SectionIn(base, File(base, file).sections(), uint32_t(i));
      if (!(Sec(base, s).kind() < 48) && SectionOutcode(base, route, point, uint32_t(i)) == 0) {
        slices(s);
      }
      file = Route(base, route).file().address;
      i = int16_t(i + 1);
      guest::StoreU16(base, position, uint16_t(i));
    }
    if (LoadU16(base, best) == 0xFFFF) {
      // None: the out position if whole, else the section before with
      // out's slice, else (0, 0).
      if (LoadU16(base, out + 0) != 0xFFFF && LoadU16(base, out + 2) != 0xFFFF) {
        guest::StoreU32(base, best, Load(base, out));
      } else if (previous != -1 && LoadU16(base, out + 2) != 0xFFFF) {
        guest::StoreU16(base, best + 0, uint16_t(before));
        guest::StoreU16(base, best + 2, uint16_t(LoadU16(base, out + 2)));
      } else {
        guest::StoreU16(base, best + 0, 0);
        guest::StoreU16(base, best + 2, 0);
      }
    }
  }
  guest::StoreU32(base, out, Load(base, best));
}

void Follow(uint8_t* base, uint32_t route, uint32_t position, uint32_t point, uint32_t before,
            uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB680 TrackRoute::Follow");
  const uint32_t copy = frame + 80;
  const uint32_t best = frame + 84;
  const uint32_t distance = frame + 88;
  const uint32_t which = frame + 92;
  guest::StoreU32(base, copy, Pos(base, position).packed());
  uint32_t section = LoadU16(base, copy);
  Stfs(base, distance, double(G::no_distance(base)));
  guest::StoreU16(base, best + 0, 0xFFFF);
  guest::StoreU16(base, best + 2, 0xFFFF);
  int32_t moved = 0;
  uint32_t branch = 0;
  if (int16_t(section) >= 0) {
    uint32_t slice = LoadU16(base, copy + 2);
    if (int16_t(slice) >= 0) {
      for (int32_t steps_left = 5;;) {
        const uint32_t where =
            NoteShoulder(base, route, point, copy, best, distance, frame - 128);
        if (where == 0) {
          Pos(base, position).set_packed(Load(base, copy));
          return;
        }
        if (where == 1 || where == 2) {
          // Past or before the slice: the next or previous one.
          const int32_t step = where == 1 ? 1 : -1;
          guest::StoreU16(base, copy + 2, uint16_t(slice + uint32_t(step)));
          moved += step;
          Normalize(base, route, copy, 0);
        } else if (where >= 7) {
          steps_left = 0;
        } else {
          const uint32_t first = Pos(base, position).section();
          if (int16_t(section) == int16_t(first)) {
            // Beside the road in the first section: the section beside it
            // (link 6 on the left, 8 on the right).
            const uint32_t s = SectionIn(base, Route(base, route).file().sections(), section);
            const uint32_t link = (where == 3 || where == 5) ? 6 : 8;
            const uint32_t to = RLink(base, Sec(base, s).links_address() + link * 4).section();
            if (to == 0xFFFF) {
              steps_left = 0;
            } else {
              guest::StoreU16(base, copy, uint16_t(to));
              Normalize(base, route, copy, 0);
            }
          } else {
            // Beside the road in another section: the first section's
            // next branch, from its slice plus the slices moved.
            const uint32_t from = Pos(base, position).slice();
            branch++;
            guest::StoreU16(base, copy, uint16_t(first));
            guest::StoreU32(base, which, branch);
            guest::StoreU16(base, copy + 2, uint16_t(from + uint32_t(moved)));
            if (TakeBranch(base, route, copy, which) == 0) {
              steps_left = 0;
            }
          }
        }
        if (--steps_left < 0) {
          break;
        }
        slice = LoadU16(base, copy + 2);
        section = LoadU16(base, copy);
      }
    }
  }
  // No slice found: the shoulder kept, else the full search, taken when its
  // slice's center is within 10 m in height.
  if (LoadS16(base, best + 0) >= 0 && LoadS16(base, best + 2) >= 0) {
    Pos(base, position).set_packed(Load(base, best));
    return;
  }
  const uint32_t word = Pos(base, position).packed();
  const uint32_t previous = Pos(base, before).section();
  const uint32_t current = Pos(base, position).section();
  guest::StoreU32(base, copy, word);
  FullSearch(base, route, copy, point, current, previous, frame - 192);
  const uint32_t found = LoadU16(base, copy);
  if (int16_t(found) < 0) {
    return;
  }
  const int32_t slice = LoadS16(base, copy + 2);
  if (slice < 0) {
    return;
  }
  const uint32_t record = Record(base, Section(base, Route(base, route).file().address, found), slice);
  if (record == 0) {
    return;
  }
  const double dy = Fsubs(double(Slice(base, record).center_y()), double(Vec(base, point).y()));
  if (!(Fmuls(dy, dy) < double(G::height_reach(base)))) {
    return;
  }
  Pos(base, position).set_packed(Load(base, copy));
}

uint32_t OnSlice(uint8_t* base, uint32_t route, uint32_t point, uint32_t position, uint32_t score,
                 uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB1B8 TrackRoute::OnSlice");
  const uint32_t number = Pos(base, position).section();
  if (SectionOutcode(base, route, point, number) != 0) {
    return 0;
  }
  const int32_t slice = int16_t(Pos(base, position).slice());
  const uint32_t section = Section(base, Route(base, route).file().address, number);
  const uint32_t record = Record(base, section, slice);
  if (Locate(base, route, point, position, record, frame - 112) != 0) {
    return 0;
  }
  if (score == 0) {
    return 1;
  }
  const double dy = Fsubs(double(Vec(base, point).y()), double(Slice(base, record).center_y()));
  double value = Fmuls(dy, dy);
  if (value < double(G::nine(base))) {
    value = Lfs(base, kZero);
  }
  if (Kind(base, section) - 1 <= 3) {
    value = Fadds(value, double(G::ten(base)));
  }
  const double dz = Fsubs(double(Vec(base, point).z()), double(Slice(base, record).center_z()));
  const double dx = Fsubs(double(Vec(base, point).x()), double(Slice(base, record).center_x()));
  Flt(base, score).set_value(float(Fadds(Fmadds(dx, dx, Fmuls(dz, dz)), value)));
  return 1;
}

// --- The linear index ---

uint32_t ToIndex(uint8_t* base, uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_820AB408 TrackRoute::ToIndex");
  const uint32_t file = Route(base, kRoute).file().address;
  const int32_t section = int16_t(packed >> 16);
  int32_t last;
  if (section < 0) {
    last = 0;
  } else {
    last = int32_t(File(base, file).section_count()) - 1;
    if (!(section > last)) {
      last = section;
    }
  }
  uint32_t index = uint32_t(int32_t(int16_t(packed)));
  if (!(section < int32_t(File(base, file).section_count()))) {
    index += uint32_t(int16_t(Sec(base, Section(base, file, uint32_t(last))).slice_count()));
  }
  if (last > 0) {
    const uint32_t table = File(base, file).sections();
    for (int32_t i = 0; i < last; i++) {
      index += uint32_t(int16_t(Sec(base, SectionIn(base, table, uint32_t(i))).slice_count()));
    }
  }
  return index;
}

uint32_t FromIndex(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820AB498 TrackRoute::FromIndex");
  const uint32_t file = Route(base, kRoute).file().address;
  const int32_t sections = int32_t(File(base, file).section_count());
  int32_t i = 0;
  uint32_t table = 0;
  bool inside = false;
  if (sections > 0) {
    table = File(base, file).sections();
    do {
      const int32_t slices = int16_t(Sec(base, SectionIn(base, table, uint32_t(i))).slice_count());
      if (int32_t(index) < slices) {
        break;
      }
      i++;
      index -= uint32_t(slices);
    } while (i < sections);
    inside = i < sections;
  }
  if (!inside) {
    table = File(base, file).sections();
    index += uint32_t(int16_t(Sec(base, SectionIn(base, table, uint32_t(sections - 1))).slice_count()));
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
    const int32_t last = int16_t(Sec(base, SectionIn(base, table, uint32_t(section))).slice_count()) - 1;
    slice = int32_t(index) > last ? last : int32_t(index);
  }
  return (uint32_t(uint16_t(section)) << 16) | uint16_t(slice);
}

void ClampPosition(uint8_t* base, uint32_t out, uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_820F8420 TrackRoute::ClampPosition");
  const uint32_t clamped = FromIndex(base, ToIndex(base, packed));
  Pos(base, out).set_section(uint16_t(clamped >> 16));
  Pos(base, out).set_slice(uint16_t(clamped));
}

// --- Distances ---

bool LinkDistance(uint8_t* base, uint32_t route, uint32_t a, uint32_t b, float& distance,
                  int32_t depth) {
  ITC_NATIVE_RUNNING("sub_820F8720 TrackRoute::LinkDistance");
  const uint32_t file = Route(base, route).file().address;
  const uint32_t table = File(base, file).sections();
  const uint32_t from = SectionIn(base, table, Pos(base, b).section());
  const int32_t target = int16_t(Pos(base, a).section());
  for (uint32_t i = 0; i < 12; i++) {
    if (int16_t(RLink(base, Sec(base, from).links_address() + 4 * i).section()) != target) {
      continue;
    }
    // b's section links straight to a's: the slices over the link back.
    const uint32_t to = SectionIn(base, table, RLink(base, Sec(base, from).links_address() + 4 * i).section());
    const int32_t back = int16_t(Pos(base, b).section());
    const int32_t slices = int32_t(uint32_t(int16_t(RLink(base, Sec(base, from).links_address() + 4 * i).slice())) -
                                   uint32_t(int16_t(Pos(base, a).slice())));
    for (int32_t m = 0; m < 12; m++) {
      if (int16_t(RLink(base, Sec(base, to).links_address() + 4 * uint32_t(m)).section()) == back) {
        const int32_t d = LinkSlices(base, slices, to, m);
        distance = float(Fmadds(double(File(base, file).slice_length()), steps::ToSingle(d), double(distance)));
        return true;
      }
    }
    return false;
  }
  if (depth - 1 < 0) {
    return false;
  }
  for (uint32_t j = 0; j < 12; j++) {
    const uint32_t link = Sec(base, from).links_address() + 4 * j;
    const uint32_t via = RLink(base, link).section();
    if (int16_t(via) == -1 || int16_t(Pos(base, b).section()) == int16_t(via)) {
      continue;
    }
    const int32_t back = int16_t(Pos(base, b).section());
    const uint32_t f = Route(base, route).file().address;
    const double so_far = double(distance);
    const int32_t slices = int16_t(RLink(base, link).slice());
    const uint32_t next = Section(base, f, via);
    for (int32_t k = 0; k < 12; k++) {
      if (int16_t(RLink(base, Sec(base, next).links_address() + 4 * uint32_t(k)).section()) != back) {
        continue;
      }
      const int32_t d = LinkSlices(base, slices, from, k);
      float further = float(Fmadds(double(File(base, f).slice_length()), steps::ToSingle(d), so_far));
      if (LinkDistance(base, route, a, link, further, depth - 1)) {
        distance = float(double(further));
        return true;
      }
      break;
    }
  }
  return false;
}

double Distance(uint8_t* base, uint32_t route, uint32_t a, uint32_t b, bool* linked) {
  ITC_NATIVE_RUNNING("sub_820F9278 TrackRoute::Distance");
  if (Pos(base, a).section() == Pos(base, b).section()) {
    const int32_t from = int16_t(Pos(base, a).slice());
    const int32_t to = int16_t(Pos(base, b).slice());
    const uint32_t file = Route(base, route).file().address;
    return Fmuls(steps::ToSingle(int32_t(uint32_t(to) - uint32_t(from))), double(File(base, file).slice_length()));
  }
  const int32_t to = int16_t(Pos(base, b).slice());
  const uint32_t file = Route(base, route).file().address;
  float distance = float(Fmuls(steps::ToSingle(to), double(File(base, file).slice_length())));
  const bool found = LinkDistance(base, route, a, b, distance, 2);
  if (linked != nullptr) {
    *linked = found;
  }
  return found ? double(distance) : double(G::no_distance(base));
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
double LengthThrough(uint8_t* base, uint32_t from, uint32_t to) {
  const vector_library::Vector v = vector_library::LoadVector(base, from & ~0xFu);
  const uint32_t dot = std::bit_cast<uint32_t>(vector_library::Dot(v, v));
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, (to & ~0xFu) + i, dot);
  }
  return Fsqrts(Lfs(base, to));
}

// PathDistance's difference of the point at `frame` + 112 (P) and a slice's
// center (the record at `record`), into `frame` + 96; with `keep`, the center into
// `frame` + 128 (Q) first, as the original stores it.
void Difference(uint8_t* base, uint32_t frame, uint32_t record, bool keep) {
  for (uint32_t i = 0; i < 12; i += 4) {
    const double p = Lfs(base, frame + 112 + i);
    const double c = Lfs(base, record + i);
    if (keep) {
      Stfs(base, frame + 128 + i, c);
    }
    Stfs(base, frame + 96 + i, Fsubs(p, c));
  }
}

}  // namespace

double PathDistance(uint8_t* base, uint32_t route, uint32_t a, uint32_t b, uint32_t path,
                    int32_t length, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA9D0 TrackRoute::PathDistance");
  const uint32_t at = frame + 80;    // the position stepped
  const uint32_t from = frame + 84;  // the last part's start
  double sum = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  Stfs(base, frame + 140, one);
  Stfs(base, frame + 108, one);
  const int32_t to_section = int16_t(b >> 16);
  // The path's last entry that is b's section (its index; 0 when none).
  int32_t last = 0;
  for (int32_t i = int16_t(length - 1); i >= 0; i = int16_t(i - 1)) {
    if (to_section == int16_t(Path(base, path + 2 * uint32_t(i)).first())) {
      last = i;
      break;
    }
  }
  const auto table = [&] { return Route(base, route).file().sections(); };
  // One slice along: the slice at `position` + 2 moved by `step`,
  // normalized; the distance from P to its center added (`dot_at` the
  // original's temporary), and P = the center. Returns the slice; `section`
  // its section, `offset` the slice times 84.
  const auto step = [&](uint32_t position, int32_t slice, int32_t delta, uint32_t dot_at,
                        uint32_t& section, uint32_t& offset) {
    slice += delta;
    guest::StoreU16(base, position + 2, uint16_t(slice));
    Normalize(base, route, position, 0);
    section = LoadU16(base, position);
    const uint32_t s = SectionIn(base, table(), section);
    slice = LoadS16(base, position + 2);
    offset = uint32_t(slice) * 84;
    Difference(base, frame, Sec(base, s).slices_address() + offset, true);
    // P = Q (its 16 bytes, read before the dot's store as the original).
    uint8_t center[16];
    std::memcpy(center, base + frame + 128, 16);
    const double length_of = LengthThrough(base, frame + 96, dot_at);
    std::memcpy(base + frame + 112, center, 16);
    sum = Fadds(length_of, sum);
    return slice;
  };
  guest::StoreU32(base, at, a);
  if (length > 1) {
    int32_t slice = int16_t(a);
    uint32_t section = LoadU16(base, at);
    uint32_t offset = uint32_t(slice) * 84;
    for (int32_t k = 1; k < length; k++) {
      const uint32_t next = Path(base, path + 2 * uint32_t(k)).first();
      const uint32_t current = SectionIn(base, table(), section);
      const uint32_t following = SectionIn(base, table(), next);
      // P: the slice's center.
      for (uint32_t i = 0; i < 12; i += 4) {
        Stfs(base, frame + 112 + i, Lfs(base, Sec(base, current).slices_address() + offset + i));
      }
      bool linked = false;
      // The next section's link back to this one: its slice here, stepped
      // to a slice at a time.
      for (uint32_t j = 0; j < 12; j++) {
        if (int16_t(RLink(base, Sec(base, following).links_address() + 4 * j).section()) != int16_t(section)) {
          continue;
        }
        const int32_t target = int16_t(RLink(base, Sec(base, following).links_address() + 4 * j).slice());
        if (slice < target) {
          Stfs(base, frame + 140, one);
          do {
            slice = step(at, slice, 1, frame + 144, section, offset);
          } while (slice < target);
        } else if (slice > target) {
          Stfs(base, frame + 140, one);
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
          const RLink link(base, Sec(base, current).links_address() + 4 * m);
          if (int16_t(link.section()) != int16_t(next)) {
            continue;
          }
          section = next;
          Stfs(base, frame + 140, one);
          guest::StoreU16(base, at, uint16_t(section));
          const uint32_t there = link.slice();
          guest::StoreU16(base, at + 2, uint16_t(there));
          slice = int16_t(there);
          offset = uint32_t(slice) * 84;
          Difference(base, frame, Sec(base, SectionIn(base, table(), next)).slices_address() + offset, false);
          sum = Fadds(LengthThrough(base, frame + 96, frame + 176), sum);
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
  if (int16_t(b) < LoadS16(base, at + 2)) {
    guest::StoreU32(base, from, b);
    end = Load(base, at);
  } else {
    guest::StoreU32(base, from, Load(base, at));
  }
  uint32_t section = LoadU16(base, from);
  int32_t slice = LoadS16(base, from + 2);
  const int32_t target = int16_t(end);
  uint32_t offset = uint32_t(slice) * 84;
  const uint32_t start = Sec(base, SectionIn(base, table(), section)).slices_address() + offset;
  for (uint32_t i = 0; i < 12; i += 4) {
    Stfs(base, frame + 112 + i, Lfs(base, start + i));
  }
  if (slice == target) {
    return sum;
  }
  Stfs(base, frame + 140, one);
  do {
    slice = step(from, slice, 1, frame + 176, section, offset);
  } while (slice != target);
  return sum;
}

// --- Points of a slice ---

bool SlicePoint(uint8_t* base, uint32_t out, uint32_t which, uint32_t section, uint32_t slice) {
  ITC_NATIVE_RUNNING("sub_820F9768 TrackRoute::SlicePoint");
  if (which >= 5 && which <= 16) {
    return false;
  }
  const uint32_t s = Section(base, Route(base, kRoute).file().address, section);
  const uint32_t slices = Sec(base, s).slices_address();
  const Slice record(base, slices + slice * Slice::kSize);
  const double cx = double(record.center_x());
  const double cy = double(record.center_y());
  const double cz = double(record.center_z());
  const uint32_t count = uint32_t(int16_t(Sec(base, s).slice_count()));
  // The two point tables after the records (probably the road's edges).
  const Vec first(base, slices + count * 84 + slice * 12);
  const Vec second(base, slices + count * 96 + slice * 12);
  const double ax = double(first.x());
  const double ay = double(first.y());
  const double az = double(first.z());
  const double bx = double(second.x());
  const double by = double(second.y());
  const double bz = double(second.z());
  switch (which) {
    case 0:
    case 3:
      Vec(base, out).set_x(float(cx));
      Vec(base, out).set_y(float(cy));
      Vec(base, out).set_z(float(cz));
      break;
    case 1:
      Vec(base, out).set_x(float(ax));
      Vec(base, out).set_y(float(ay));
      Vec(base, out).set_z(float(az));
      break;
    case 2:
      Vec(base, out).set_x(float(bx));
      Vec(base, out).set_y(float(by));
      Vec(base, out).set_z(float(bz));
      break;
    case 4: {
      const double half = double(G::half(base));
      const double x = Fadds(cx, cx), y = Fadds(cy, cy), z = Fadds(cz, cz);
      Vec(base, out).set_x(float(Fmuls(x, half)));
      const double hy = Fmuls(y, half), hz = Fmuls(z, half);
      Vec(base, out).set_y(float(hy));
      Vec(base, out).set_z(float(hz));
      break;
    }
    default:  // above 16: nothing
      break;
  }
  return true;
}

}  // namespace track_route
