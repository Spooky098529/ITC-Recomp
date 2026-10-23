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
