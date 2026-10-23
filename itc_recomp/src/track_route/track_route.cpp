// The course's route (docs/functions/track-route.md): see track_route.h.

#include "track_route/track_route.h"

#include "car_physics/float_ops.h"
#include "strings_and_font/steps.h"

#include "base/log.h"

namespace track_route {

using namespace car_physics::ops;
namespace steps = strings_and_font::steps;

namespace {

constexpr uint32_t kHalf = 0x82001BE4;          // 0.5
constexpr uint32_t kBoundsMargin = 0x82002568;  // 20.0
constexpr uint32_t kNoDistance = 0x82002814;    // 10000.0
constexpr uint32_t kNine = 0x82006844;          // 9.0
constexpr uint32_t kTen = 0x82001FD8;           // 10.0
constexpr uint32_t kDefaultBranchMask = 0x82008A00;  // 0xDFFFFE7F, then 0
constexpr uint32_t kHeightReach = 0x82002A0C;        // 100.0 (10 m squared)
constexpr uint32_t kReachSquared = 0x8200681C;       // 62500.0 (250 m squared)
constexpr uint32_t kFarthest = 0x82006824;           // FullSearch's first score

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
  return SectionIn(base, Load(base, file + 12), number);
}
// A slice's record (the 32-bit wrap of the original's 64-bit sum).
uint32_t Record(uint32_t section, int32_t slice) {
  return section + 124 + uint32_t(slice) * 84;
}
uint32_t Kind(uint8_t* base, uint32_t section) {
  return LoadU8(base, section + 2) & 0xF;
}

// A position moved over the link at `link` (StepForward's and
// StepAlongPath's ends, and the lane steps'): the section from the link, the
// slice less the slices' count (`slices()`, read when the original reads its
// word), in the original's order.
template <typename Slices>
uint32_t TakeForward(uint8_t* base, uint32_t position, uint32_t link, Slices slices) {
  const uint32_t section = LoadU16(base, link);
  const uint32_t slice = LoadU16(base, position + 2);
  guest::StoreU16(base, position + 0, uint16_t(section));
  guest::StoreU16(base, position + 2, uint16_t(slice - slices()));
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
  const uint32_t table = Load(base, Load(base, route) + 12);
  const uint32_t section = SectionIn(base, table, LoadU16(base, position));
  for (int32_t i = 2; i >= 0; i--) {
    const uint32_t link = section + 28 + 4 * uint32_t(i);
    const uint32_t to = LoadU16(base, link);
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
    const uint32_t to = LoadU16(base, section + 28 + 4 * i);
    if (to == 0xFFFF) {
      continue;
    }
    const uint32_t s = Section(base, Load(base, route), to);
    const uint32_t kind = Kind(base, s);
    if (kind == 2 || kind == 4) {
      continue;
    }
    const uint32_t branch = LoadU8(base, s + 3);
    const uint32_t bits = Load(base, mask);
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
  const uint32_t file = Load(base, route);
  const uint32_t number = LoadU16(base, position);
  const uint32_t section = Section(base, file, number);
  if (LoadU16(base, section + (uint32_t(link) + 7) * 4) == 0xFFFF) {
    return -1;
  }
  const int32_t lane = LoadS16(base, lane_address);
  return int8_t(LoadU8(base, section + uint32_t(link) * 8 + uint32_t(lane) + 76));
}

// A link's slice difference (LinkDistance): `from` less the slice of link
// `index` of `section`, plus 1 for a forward link, less 1 for a back one.
int32_t LinkSlices(uint8_t* base, int32_t from, uint32_t section, int32_t index) {
  int32_t d = int32_t(uint32_t(from) - uint32_t(LoadS16(base, section + 30 + 4 * uint32_t(index))));
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
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  if (Load(base, masks) != 0) {
    uint32_t mask = masks;
    for (uint32_t m = 0;;) {
      const FirstBranch first = FirstBranchMatching(base, route, section, mask);
      if (int32_t(first.count) > 0) {
        const uint32_t slice = LoadU16(base, position + 2);
        guest::StoreU16(base, position + 0, uint16_t(first.section));
        guest::StoreU16(base, position + 2, uint16_t(slice - slices()));
        return 1;
      }
      m++;
      mask = masks + 4 * m;
      if (Load(base, mask) == 0) {
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
  const uint32_t slice = LoadU16(base, position + 2);
  guest::StoreU16(base, position + 0, uint16_t(to));
  guest::StoreU16(base, position + 2, uint16_t(slice - slices()));
  const int32_t carried = LinkLaneWith(base, route, position, link, lane_address);
  guest::StoreU16(base, lane_address, uint16_t(carried));
  const int32_t lanes =
      int32_t(LoadU8(base, Section(base, Load(base, route), LoadU16(base, position)) + 180) & 0xF);
  const int32_t lane = int16_t(carried);
  if (lane < 0) {
    guest::StoreU16(base, lane_address, uint16_t(link == 0 ? lanes - 1 : 0));
  } else if (!(lane < lanes)) {
    guest::StoreU16(base, lane_address, uint16_t(lanes - 1));
  }
  return 1;
}

// StepForwardLane with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardLaneWith(uint8_t* base, uint32_t route, uint32_t position,
                             uint32_t lane_address, Slices slices) {
  ITC_NATIVE_RUNNING("sub_820F9448 TrackRoute::StepForwardLane");
  const uint32_t file = Load(base, route);
  const uint32_t table = Load(base, file + 12);
  const uint32_t section = SectionIn(base, table, LoadU16(base, position));
  // The forward links whose lanes, added up in link order, hold the lane
  // (every linked section's lanes added; kinds 1, 3 and 4 not taken).
  int32_t lanes = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t link = section + 28 + 4 * i;
    const uint32_t to = LoadU16(base, link);
    if (int16_t(to) < 0) {
      continue;
    }
    const uint32_t s = SectionIn(base, Load(base, file + 12), to);
    lanes += int32_t(LoadU8(base, s + 180) & 0xF);
    const int32_t kind = int32_t(Kind(base, s));
    if (kind == 1 || (kind > 2 && kind <= 4)) {
      continue;
    }
    if (LoadS16(base, lane_address) < lanes) {
      return TakeLane(base, route, position, lane_address, LoadU16(base, link), int32_t(i), slices);
    }
  }
  // Else links 2, 1, 0 to a section not of kinds 1, 3 or 4, onto its last
  // lane (0 when it has none).
  for (int32_t i = 2; i >= 0; i--) {
    const uint32_t link = section + 28 + 4 * uint32_t(i);
    const uint32_t to = LoadU16(base, link);
    if (int16_t(to) < 0) {
      continue;
    }
    const uint32_t kind = Kind(base, SectionIn(base, table, to));
    if (kind == 1 || kind - 3 <= 1) {
      continue;
    }
    const uint32_t next = LoadU16(base, link);
    TakeForward(base, position, link, slices);
    const int32_t last = int16_t(
        int32_t(LoadU8(base, Section(base, Load(base, route), next) + 180) & 0xF) - 1);
    guest::StoreU16(base, lane_address, uint16_t(last));
    if (last < 0) {
      guest::StoreU16(base, lane_address, 0);
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
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  while (LoadU16(base, path) < 0x8000) {
    for (uint32_t i = 0; i <= 2; i++) {
      const uint32_t link = section + 28 + 4 * i;
      const int32_t to = LoadS16(base, link);
      if (to != -1 && to == LoadS16(base, path)) {
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
  const uint32_t number = LoadU16(base, position);
  const int32_t section = int16_t(number);
  if (section < 0) {
    return false;
  }
  const uint32_t file = Load(base, route);
  if (!(int32_t(Load(base, file)) > section)) {
    return false;
  }
  const int32_t slice = LoadS16(base, position + 2);
  if (slice < 0) {
    return false;
  }
  return LoadS16(base, Section(base, file, number)) > slice;
}

// Normalize and NormalizePath around their forward step: on the route, 1;
// a section off the route, (0, 0) and 0; else NormalizeBack then `forward()`,
// 1 when both did; else 0, the position (0, 0) unless it's on the route.
template <typename Forward>
uint32_t NormalizeWith(uint8_t* base, uint32_t route, uint32_t position, Forward forward) {
  if (OnRoute(base, route, position)) {
    return 1;
  }
  const int32_t section = LoadS16(base, position);
  if (section >= 0 && section < int32_t(Load(base, Load(base, route)))) {
    if (NormalizeBack(base, route, position) != 0 && forward() != 0) {
      return 1;
    }
    if (OnRoute(base, route, position)) {
      return 0;
    }
  }
  guest::StoreU32(base, position, 0);
  return 0;
}

// StepForwardLaneMasked with the slices' count from `slices()`.
template <typename Slices>
uint32_t StepForwardLaneMaskedWith(uint8_t* base, uint32_t route, uint32_t position,
                                   uint32_t lane, Slices slices, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_820F9600 TrackRoute::StepForwardLaneMasked");
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  if (Load(base, masks) == 0) {
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
        const uint32_t to = LoadU16(base, section + 28 + 4 * uint32_t(i));
        if (int16_t(to) < 0) {
          continue;
        }
        lanes += int32_t(LoadU8(base, Section(base, Load(base, route), to) + 180) & 0xF);
        if (i != int32_t(int16_t(first.link))) {
          continue;
        }
        if (LoadS16(base, lane) < lanes || !(int32_t(first.count) > 1)) {
          return TakeLane(base, route, position, lane, first.section, i, slices);
        }
      }
    }
    m++;
    mask = masks + 4 * m;
    if (Load(base, mask) == 0) {
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
    const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
    const int32_t slice = LoadS16(base, position + 2);
    const int32_t slices = LoadS16(base, section);
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
    guest::StoreU32(base, position, 0);
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
  const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
  const double side_x = Lfs(base, record + 36);
  const double half = Fmuls(Lfs(base, record + 12), Lfs(base, kHalf));
  const double side_z = Lfs(base, record + 40);
  return {Fmadds(dz, side_z, Fmuls(dx, side_x)), half};
}

}  // namespace

// --- Lookups ---

void LanePoint(uint8_t* base, uint32_t route, uint32_t out, uint32_t position, int32_t lane) {
  ITC_NATIVE_RUNNING("sub_820F8668 TrackRoute::LanePoint");
  const uint32_t file = Load(base, route);
  const uint32_t section = Section(base, file, LoadU16(base, position));
  const uint32_t record = Record(section, LoadS16(base, position + 2));
  const int32_t lanes = int32_t(LoadU8(base, record + 56) & 0xF);
  if (!(lane < lanes)) {
    lane = lanes - 1;
  }
  const double width = Lfs(base, record + 12);
  const double side_x = Lfs(base, record + 36);
  const double center_x = Lfs(base, record + 0);
  const double half = Lfs(base, kHalf);
  const double one = Lfs(base, kOne);
  const double n = steps::ToSingle(lanes);
  const double l = steps::ToSingle(lane);
  const double offset = Fmuls(Fdivs(Fnmsubs(Fsubs(n, one), half, l), n), width);
  Stfs(base, out + 0, Fmadds(side_x, offset, center_x));
  Stfs(base, out + 4, Lfs(base, record + 4));
  const double side_z = Lfs(base, record + 40);
  const double center_z = Lfs(base, record + 8);
  const double z = Fmadds(side_z, offset, center_z);
  Stfs(base, out + 12, one);
  Stfs(base, out + 8, z);
}

uint32_t LaneAt(uint8_t* base, uint32_t route, uint32_t point, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8920 TrackRoute::LaneAt");
  const uint32_t file = Load(base, route);
  const uint32_t section = Section(base, file, LoadU16(base, position));
  const uint32_t record = Record(section, LoadS16(base, position + 2));
  const int32_t lanes = int32_t(LoadU8(base, record + 56) & 0xF);
  if (lanes <= 1) {
    return 0;
  }
  const double across_x = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  const double across_z = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
  const double along = Fmadds(across_z, Lfs(base, record + 40),
                              Fmuls(across_x, Lfs(base, record + 36)));
  const double t = Fadds(Fdivs(along, Lfs(base, record + 12)), Lfs(base, kHalf));
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
  const uint32_t file = Load(base, route);
  const uint32_t number = section & 0xFFFF;
  const uint32_t index = link & 0xFFFF;
  const uint32_t fallback = index >= 3 ? 3 : 0;
  const uint32_t s = Section(base, file, number);
  int32_t result = int32_t(number);
  const int32_t first = LoadS16(base, s + (index + 7) * 4);
  if (first != -1) {
    result = first;
  } else {
    const int32_t second = LoadS16(base, s + (fallback + 7) * 4);
    if (second != -1) {
      result = second;
    }
  }
  return int16_t(result);
}

int32_t LinkLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t link_address,
                 uint32_t lane_address) {
  return LinkLaneWith(base, route, position, LoadS16(base, link_address), lane_address);
}

int32_t SpecialSection(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8D38 TrackRoute::SpecialSection");
  const uint32_t number = LoadU16(base, position);
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
  const uint32_t section = Section(base, Load(base, route), number);
  if (Kind(base, section) != 4) {
    return -1;
  }
  const uint32_t record = Record(section, LoadS16(base, position + 2));
  return LoadU8(base, record + 57) == 4 ? index : -1;
}

uint32_t SliceTest(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8DE8 TrackRoute::SliceTest");
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  const int32_t kind = int32_t(Kind(base, section));
  if (kind <= 0) {
    return 0;
  }
  const uint32_t record = Record(section, LoadS16(base, position + 2));
  if (kind <= 2) {
    return LoadU16(base, record + 32) == LoadU16(base, section + 124 + 32) ? 1 : 0;
  }
  if (kind == 3) {
    return LoadU8(base, record + 57) == 3 ? 0 : 1;
  }
  if (kind == 4) {
    return LoadU8(base, record + 57) == 4 ? 1 : 0;
  }
  return 0;
}

uint32_t IsMainRoad(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F8E98 TrackRoute::IsMainRoad");
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  if (Kind(base, section) - 1 <= 3) {
    return 0;
  }
  const int32_t branch = int32_t(LoadU8(base, section + 3));
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
  const uint32_t s = Section(base, Load(base, route), section);
  const double margin = Lfs(base, kBoundsMargin);
  uint32_t code = 0;
  const double x = Lfs(base, point + 0);
  if (x < Fsubs(Lfs(base, s + 4), margin)) {
    code = 1;
  } else if (x > Fadds(Lfs(base, s + 16), margin)) {
    code = 2;
  }
  const double y = Lfs(base, point + 4);
  if (y < Fsubs(Lfs(base, s + 8), margin)) {
    code |= 4;
  } else if (y > Fadds(Lfs(base, s + 20), margin)) {
    code |= 8;
  }
  const double z = Lfs(base, point + 8);
  if (z < Fsubs(Lfs(base, s + 12), margin)) {
    code |= 16;
  } else if (z > Fadds(Lfs(base, s + 24), margin)) {
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
    guest::StoreU16(base, out + 0, uint16_t(to));
    guest::StoreU16(base, out + 2, uint16_t(i));
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
    const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
    const int32_t slice = LoadS16(base, position + 2);
    const int32_t slices = LoadS16(base, section);
    if (slice < slices) {
      return 1;
    }
    const auto count = [slices] { return uint32_t(slices); };
    const uint32_t stepped = masks == 0
                                 ? StepForwardWith(base, route, position, count)
                                 : StepForwardMaskedWith(base, route, position, count, masks);
    if (stepped == 0) {
      if (LoadU16(base, position) < 0x8000) {
        guest::StoreU16(base, position + 2, uint16_t(slices - 1));
      } else {
        guest::StoreU32(base, position, 0);
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
  const uint32_t table = Load(base, Load(base, route) + 12);
  const uint32_t section = SectionIn(base, table, LoadU16(base, position));
  for (int32_t i = 5; i >= 3; i--) {
    const uint32_t link = section + 28 + 4 * uint32_t(i);
    const uint32_t to = LoadU16(base, link);
    if (to == 0xFFFF) {
      continue;
    }
    if (i > 3) {
      const uint32_t s = SectionIn(base, table, to);
      const uint32_t kind = Kind(base, s);
      if (kind == 1 || kind == 3) {
        continue;
      }
      const uint32_t branch = LoadU8(base, s + 3);
      if (branch == 7 || branch == 13 || branch == 29) {
        continue;
      }
      uint32_t back = 0;
      for (uint32_t j = 0; j < 3; j++) {
        if (LoadU16(base, s + 40 + 4 * j) != 0xFFFF) {
          back++;
        }
      }
      if (back == 0) {
        continue;
      }
    }
    guest::StoreU16(base, position + 0, uint16_t(LoadU16(base, link)));
    const uint32_t s = Section(base, Load(base, route), LoadU16(base, link));
    const uint32_t slice = LoadU16(base, position + 2);
    guest::StoreU16(base, position + 2, uint16_t(LoadU16(base, s) + slice));
    return 1;
  }
  return 0;
}

uint32_t NormalizeBack(uint8_t* base, uint32_t route, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_820F9320 TrackRoute::NormalizeBack");
  while (LoadU16(base, position + 2) >= 0x8000) {
    if (StepBack(base, route, position) == 0) {
      if (LoadU16(base, position) < 0x8000) {
        guest::StoreU16(base, position + 2, 0);
      } else {
        guest::StoreU32(base, position, 0);
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
      const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
      const int32_t slice = LoadS16(base, position + 2);
      const int32_t slices = LoadS16(base, section);
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
  if (LoadU16(base, position + 2) >= 0x8000) {
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
  const int32_t section = LoadS16(base, position);
  if (section < 0 || !(section < int32_t(Load(base, Load(base, route))))) {
    guest::StoreU32(base, position, 0);
    return 0;
  }
  if (LoadU16(base, position + 2) >= 0x8000) {
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
  guest::StoreU32(base, copy, Load(base, position));
  const uint32_t valid = Normalize(base, route, copy, 0);
  const uint32_t section = LoadU16(base, copy);
  if (valid != 0) {
    const double one = Lfs(base, kOne);
    const int32_t slice = LoadS16(base, copy + 2);
    const uint32_t record = Record(Section(base, Load(base, route), section), slice);
    Stfs(base, out + 0, Lfs(base, record + 0));
    Stfs(base, out + 4, Lfs(base, record + 4));
    Stfs(base, out + 8, Lfs(base, record + 8));
    Stfs(base, out + 12, one);
  }
  return section;
}

uint32_t AlongSlice(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                    uint32_t record, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FAE78 TrackRoute::AlongSlice");
  const double across_x = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  const double forward_x = Lfs(base, record + 44);
  const double z = Lfs(base, point + 8);
  const uint32_t file = Load(base, route);
  const double forward_z = Lfs(base, record + 48);
  const double along_x = Fmuls(across_x, forward_x);
  const double across_z = Fsubs(z, Lfs(base, record + 8));
  const double length = Lfs(base, file + 8);
  const double along = Fmadds(across_z, forward_z, along_x);
  if (!(along < Fneg(length)) && !(along > length)) {
    return 0;
  }
  // The next slice, normalized.
  const uint32_t copy = frame + 80;
  const uint32_t section = LoadU16(base, position);
  const uint32_t slice = LoadU16(base, position + 2);
  guest::StoreU16(base, copy + 0, uint16_t(section));
  guest::StoreU16(base, copy + 2, uint16_t(slice + 1));
  if (along < Lfs(base, kZero)) {
    return 2;
  }
  if (Normalize(base, route, copy, 0) == 0) {
    return 1;
  }
  const uint32_t next = Record(Section(base, Load(base, route), LoadU16(base, copy)),
                               LoadS16(base, copy + 2));
  if (next != 0) {
    return 1;
  }
  // A null record (never: what follows reads address 8 and 0, as the
  // original does).
  const double dz = Fsubs(Lfs(base, 8), Lfs(base, record + 8));
  const double squared = Fmuls(along, along);
  const double dx = Fsubs(Lfs(base, 0), Lfs(base, record + 0));
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
  if (a.offset < Fsubs(Fneg(Lfs(base, record + 16)), a.half)) {
    return 5;
  }
  if (a.offset > Fadds(Lfs(base, record + 20), a.half)) {
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
  if (a.offset < Fsubs(Fneg(Lfs(base, record + 16)), a.half)) {
    return 5;
  }
  if (a.offset > Fadds(Lfs(base, record + 20), a.half)) {
    return 6;
  }
  const double shoulder = Fadds(a.half, Lfs(base, kHalf));
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
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  const uint32_t record = Record(section, LoadS16(base, position + 2));
  return LocateShoulder(base, route, point, position, record, frame);
}

uint32_t ToMainRoad(uint8_t* base, uint32_t route, uint32_t position, uint32_t flag,
                    uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FA728 TrackRoute::ToMainRoad");
  const uint32_t copy = frame + 80;
  // Forward: up to 10 sections (the slice put past each section's end, then
  // normalized with the default branch mask), counting the slices moved.
  guest::StoreU32(base, copy, Load(base, position));
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
    const uint32_t slices = LoadU16(base, Section(base, Load(base, route), LoadU16(base, copy)));
    guest::StoreU16(base, copy + 2, uint16_t(slices));
    moved += int32_t(int16_t(slices)) - slice;
    if (Normalize(base, route, copy, kDefaultBranchMask) == 0) {
      break;
    }
  }
  if (!found) {
    // Back: up to 10 sections (the slice put before each section's start).
    guest::StoreU32(base, copy, Load(base, position));
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
      if (Normalize(base, route, copy, kDefaultBranchMask) == 0) {
        return 0;
      }
    }
  }
  if (int32_t(flag) != 0) {
    guest::StoreU16(base, copy + 2, uint16_t(LoadU16(base, copy + 2) - uint32_t(moved)));
  }
  const uint32_t result = Normalize(base, route, copy, kDefaultBranchMask);
  guest::StoreU32(base, position, Load(base, copy));
  return result;
}

uint32_t NoteShoulder(uint8_t* base, uint32_t route, uint32_t point, uint32_t position,
                      uint32_t best, uint32_t distance, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB100 TrackRoute::NoteShoulder");
  const uint32_t section = Section(base, Load(base, route), LoadU16(base, position));
  const uint32_t record = Record(section, LoadS16(base, position + 2));
  const uint32_t where = LocateShoulder(base, route, point, position, record, frame - 112);
  if (where < 3 || where > 4) {
    return where;
  }
  const double dy = Fsubs(Lfs(base, point + 4), Lfs(base, record + 4));
  const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
  const double nearest = Lfs(base, distance);
  const double squared = Fmadds(dz, dz, Fmadds(dx, dx, Fmuls(dy, dy)));
  if (squared < nearest) {
    guest::StoreU16(base, best + 0, uint16_t(LoadU16(base, position + 0)));
    guest::StoreU16(base, best + 2, uint16_t(LoadU16(base, position + 2)));
    Stfs(base, distance, squared);
  }
  return where;
}

uint32_t TakeBranch(uint8_t* base, uint32_t route, uint32_t position, uint32_t which) {
  ITC_NATIVE_RUNNING("sub_820FA4A8 TrackRoute::TakeBranch");
  if (OnRoute(base, route, position)) {
    return 1;
  }
  const uint32_t table = Load(base, Load(base, route) + 12);
  const int32_t slice = LoadS16(base, position + 2);
  const uint32_t section = SectionIn(base, table, LoadU16(base, position));
  int32_t counted = 0;
  // Whether this is the n-th link (the word at `which`), else counted.
  const auto nth = [&] {
    if (!(counted < int32_t(Load(base, which)))) {
      return true;
    }
    counted++;
    return false;
  };
  if (slice >= 0) {
    // Past the end: forward links 2, 1, 0 (2 and 1 not to kinds 2 or 4),
    // then 2 and 1 to kinds 2 and 4; the slice less the section's count.
    const auto take = [&](uint32_t link) {
      guest::StoreU16(base, position + 0, uint16_t(LoadU16(base, link)));
      guest::StoreU16(base, position + 2, uint16_t(uint32_t(slice) - LoadU16(base, section)));
      return Normalize(base, route, position, 0);
    };
    for (int32_t i = 2; i >= 0; i--) {
      const uint32_t link = section + 28 + 4 * uint32_t(i);
      const uint32_t to = LoadU16(base, link);
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
      const uint32_t link = section + 28 + 4 * uint32_t(i);
      const uint32_t to = LoadU16(base, link);
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
    guest::StoreU16(base, position + 0, uint16_t(LoadU16(base, link)));
    const uint32_t to = Section(base, Load(base, route), LoadU16(base, link));
    guest::StoreU16(base, position + 2, uint16_t(LoadU16(base, to) + uint32_t(slice)));
    return Normalize(base, route, position, 0);
  };
  for (int32_t i = 5; i >= 3; i--) {
    const uint32_t link = section + 28 + 4 * uint32_t(i);
    const uint32_t to = LoadU16(base, link);
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
        if (LoadU16(base, s + 40 + 4 * j) != 0xFFFF) {
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
    const uint32_t link = section + 28 + 4 * uint32_t(i);
    const uint32_t to = LoadU16(base, link);
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
  double nearest = Lfs(base, kFarthest);
  Stfs(base, score, nearest);
  guest::StoreU16(base, best + 2, 0);
  guest::StoreU16(base, best + 0, 0xFFFF);
  const double reach = Lfs(base, kReachSquared);
  const int32_t given = int16_t(section);
  const auto table = [&] { return Load(base, Load(base, route) + 12); };
  // Every slice of the section `s` (its number at `position`), the nearest
  // on it kept (OnSlice's score).
  const auto slices = [&](uint32_t s) {
    guest::StoreU16(base, position + 2, 0);
    if (!(LoadS16(base, s) > 0)) {
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
    } while (slice < LoadS16(base, s));
  };
  // The sections the links of `s` lead to, within their bounds. `follow`:
  // the original's register for `s` taking each such section (its next
  // links read from there).
  const auto links = [&](uint32_t s, bool follow) {
    for (uint32_t i = 0; i < 12; i++) {
      const uint32_t to = LoadU16(base, s + 28 + 4 * i);
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
    uint32_t file = Load(base, route);
    guest::StoreU16(base, position, 0);
    for (int32_t i = 0; i < int32_t(Load(base, file));) {
      const uint32_t s = SectionIn(base, Load(base, file + 12), uint32_t(i));
      if (!(LoadU8(base, s + 2) < 48) && SectionOutcode(base, route, point, uint32_t(i)) == 0) {
        slices(s);
      }
      file = Load(base, route);
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
  guest::StoreU32(base, copy, Load(base, position));
  uint32_t section = LoadU16(base, copy);
  Stfs(base, distance, Lfs(base, kNoDistance));
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
          guest::StoreU32(base, position, Load(base, copy));
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
          const uint32_t first = LoadU16(base, position + 0);
          if (int16_t(section) == int16_t(first)) {
            // Beside the road in the first section: the section beside it
            // (link 6 on the left, 8 on the right).
            const uint32_t s = SectionIn(base, Load(base, Load(base, route) + 12), section);
            const uint32_t link = (where == 3 || where == 5) ? 6 : 8;
            const uint32_t to = LoadU16(base, s + (link + 7) * 4);
            if (to == 0xFFFF) {
              steps_left = 0;
            } else {
              guest::StoreU16(base, copy, uint16_t(to));
              Normalize(base, route, copy, 0);
            }
          } else {
            // Beside the road in another section: the first section's
            // next branch, from its slice plus the slices moved.
            const uint32_t from = LoadU16(base, position + 2);
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
    guest::StoreU32(base, position, Load(base, best));
    return;
  }
  const uint32_t word = Load(base, position);
  const uint32_t previous = LoadU16(base, before);
  const uint32_t current = LoadU16(base, position);
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
  const uint32_t record = Record(Section(base, Load(base, route), found), slice);
  if (record == 0) {
    return;
  }
  const double dy = Fsubs(Lfs(base, record + 4), Lfs(base, point + 4));
  if (!(Fmuls(dy, dy) < Lfs(base, kHeightReach))) {
    return;
  }
  guest::StoreU32(base, position, Load(base, copy));
}

uint32_t OnSlice(uint8_t* base, uint32_t route, uint32_t point, uint32_t position, uint32_t score,
                 uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820FB1B8 TrackRoute::OnSlice");
  const uint32_t number = LoadU16(base, position);
  if (SectionOutcode(base, route, point, number) != 0) {
    return 0;
  }
  const int32_t slice = LoadS16(base, position + 2);
  const uint32_t section = Section(base, Load(base, route), number);
  const uint32_t record = Record(section, slice);
  if (Locate(base, route, point, position, record, frame - 112) != 0) {
    return 0;
  }
  if (score == 0) {
    return 1;
  }
  const double dy = Fsubs(Lfs(base, point + 4), Lfs(base, record + 4));
  double value = Fmuls(dy, dy);
  if (value < Lfs(base, kNine)) {
    value = Lfs(base, kZero);
  }
  if (Kind(base, section) - 1 <= 3) {
    value = Fadds(value, Lfs(base, kTen));
  }
  const double dz = Fsubs(Lfs(base, point + 8), Lfs(base, record + 8));
  const double dx = Fsubs(Lfs(base, point + 0), Lfs(base, record + 0));
  Stfs(base, score, Fadds(Fmadds(dx, dx, Fmuls(dz, dz)), value));
  return 1;
}

// --- The linear index ---

uint32_t ToIndex(uint8_t* base, uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_820AB408 TrackRoute::ToIndex");
  const uint32_t file = Load(base, kRoute);
  const int32_t section = int16_t(packed >> 16);
  int32_t last;
  if (section < 0) {
    last = 0;
  } else {
    last = int32_t(Load(base, file)) - 1;
    if (!(section > last)) {
      last = section;
    }
  }
  uint32_t index = uint32_t(int32_t(int16_t(packed)));
  if (!(section < int32_t(Load(base, file)))) {
    index += uint32_t(LoadS16(base, Section(base, file, uint32_t(last))));
  }
  if (last > 0) {
    const uint32_t table = Load(base, file + 12);
    for (int32_t i = 0; i < last; i++) {
      index += uint32_t(LoadS16(base, SectionIn(base, table, uint32_t(i))));
    }
  }
  return index;
}

uint32_t FromIndex(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820AB498 TrackRoute::FromIndex");
  const uint32_t file = Load(base, kRoute);
  const int32_t sections = int32_t(Load(base, file));
  int32_t i = 0;
  uint32_t table = 0;
  bool inside = false;
  if (sections > 0) {
    table = Load(base, file + 12);
    do {
      const int32_t slices = LoadS16(base, SectionIn(base, table, uint32_t(i)));
      if (int32_t(index) < slices) {
        break;
      }
      i++;
      index -= uint32_t(slices);
    } while (i < sections);
    inside = i < sections;
  }
  if (!inside) {
    table = Load(base, file + 12);
    index += uint32_t(LoadS16(base, SectionIn(base, table, uint32_t(sections - 1))));
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
    const int32_t last = LoadS16(base, SectionIn(base, table, uint32_t(section))) - 1;
    slice = int32_t(index) > last ? last : int32_t(index);
  }
  return (uint32_t(uint16_t(section)) << 16) | uint16_t(slice);
}

void ClampPosition(uint8_t* base, uint32_t out, uint32_t packed) {
  ITC_NATIVE_RUNNING("sub_820F8420 TrackRoute::ClampPosition");
  const uint32_t clamped = FromIndex(base, ToIndex(base, packed));
  guest::StoreU16(base, out + 0, uint16_t(clamped >> 16));
  guest::StoreU16(base, out + 2, uint16_t(clamped));
}

// --- Distances ---

bool LinkDistance(uint8_t* base, uint32_t route, uint32_t a, uint32_t b, float& distance,
                  int32_t depth) {
  ITC_NATIVE_RUNNING("sub_820F8720 TrackRoute::LinkDistance");
  const uint32_t file = Load(base, route);
  const uint32_t table = Load(base, file + 12);
  const uint32_t from = SectionIn(base, table, LoadU16(base, b));
  const int32_t target = LoadS16(base, a);
  for (uint32_t i = 0; i < 12; i++) {
    if (LoadS16(base, from + 28 + 4 * i) != target) {
      continue;
    }
    // b's section links straight to a's: the slices over the link back.
    const uint32_t to = SectionIn(base, table, LoadU16(base, from + 28 + 4 * i));
    const int32_t back = LoadS16(base, b);
    const int32_t slices = int32_t(uint32_t(LoadS16(base, from + 30 + 4 * i)) -
                                   uint32_t(LoadS16(base, a + 2)));
    for (int32_t m = 0; m < 12; m++) {
      if (LoadS16(base, to + 28 + 4 * uint32_t(m)) == back) {
        const int32_t d = LinkSlices(base, slices, to, m);
        distance = float(Fmadds(Lfs(base, file + 8), steps::ToSingle(d), double(distance)));
        return true;
      }
    }
    return false;
  }
  if (depth - 1 < 0) {
    return false;
  }
  for (uint32_t j = 0; j < 12; j++) {
    const uint32_t link = from + 28 + 4 * j;
    const uint32_t via = LoadU16(base, link);
    if (int16_t(via) == -1 || LoadS16(base, b) == int16_t(via)) {
      continue;
    }
    const int32_t back = LoadS16(base, b);
    const uint32_t f = Load(base, route);
    const double so_far = double(distance);
    const int32_t slices = LoadS16(base, link + 2);
    const uint32_t next = Section(base, f, via);
    for (int32_t k = 0; k < 12; k++) {
      if (LoadS16(base, next + 28 + 4 * uint32_t(k)) != back) {
        continue;
      }
      const int32_t d = LinkSlices(base, slices, from, k);
      float further = float(Fmadds(Lfs(base, f + 8), steps::ToSingle(d), so_far));
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
  if (LoadU16(base, a) == LoadU16(base, b)) {
    const int32_t from = LoadS16(base, a + 2);
    const int32_t to = LoadS16(base, b + 2);
    const uint32_t file = Load(base, route);
    return Fmuls(steps::ToSingle(int32_t(uint32_t(to) - uint32_t(from))), Lfs(base, file + 8));
  }
  const int32_t to = LoadS16(base, b + 2);
  const uint32_t file = Load(base, route);
  float distance = float(Fmuls(steps::ToSingle(to), Lfs(base, file + 8)));
  const bool found = LinkDistance(base, route, a, b, distance, 2);
  if (linked != nullptr) {
    *linked = found;
  }
  return found ? double(distance) : Lfs(base, kNoDistance);
}

// --- Points of a slice ---

bool SlicePoint(uint8_t* base, uint32_t out, uint32_t which, uint32_t section, uint32_t slice) {
  ITC_NATIVE_RUNNING("sub_820F9768 TrackRoute::SlicePoint");
  if (which >= 5 && which <= 16) {
    return false;
  }
  const uint32_t s = Section(base, Load(base, kRoute), section);
  const uint32_t record = s + slice * 84;
  const double cx = Lfs(base, record + 124);
  const double cy = Lfs(base, record + 128);
  const double cz = Lfs(base, record + 132);
  const uint32_t count = uint32_t(LoadS16(base, s));
  const uint32_t first = s + count * 84 + slice * 12;
  const uint32_t second = s + count * 96 + slice * 12;
  const double ax = Lfs(base, first + 124);
  const double ay = Lfs(base, first + 128);
  const double az = Lfs(base, first + 132);
  const double bx = Lfs(base, second + 124);
  const double by = Lfs(base, second + 128);
  const double bz = Lfs(base, second + 132);
  switch (which) {
    case 0:
    case 3:
      Stfs(base, out + 0, cx);
      Stfs(base, out + 4, cy);
      Stfs(base, out + 8, cz);
      break;
    case 1:
      Stfs(base, out + 0, ax);
      Stfs(base, out + 4, ay);
      Stfs(base, out + 8, az);
      break;
    case 2:
      Stfs(base, out + 0, bx);
      Stfs(base, out + 4, by);
      Stfs(base, out + 8, bz);
      break;
    case 4: {
      const double half = Lfs(base, kHalf);
      const double x = Fadds(cx, cx), y = Fadds(cy, cy), z = Fadds(cz, cz);
      Stfs(base, out + 0, Fmuls(x, half));
      const double hy = Fmuls(y, half), hz = Fmuls(z, half);
      Stfs(base, out + 4, hy);
      Stfs(base, out + 8, hz);
      break;
    }
    default:  // above 16: nothing
      break;
  }
  return true;
}

}  // namespace track_route
