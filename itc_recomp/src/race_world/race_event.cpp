// The race event's start placements (docs/functions/race-world.md):
// RaceEvent::PlaceStartsCurrent, PlaceStarts (with its split-off groups'
// loop, sub_82144874), PlaceGroupFixed, PlaceGroup, PlaceGroupTwoRows and
// PlaceGroupSplit. See race_world.h.

#include "race_world/race_world.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "schema/race_event.h"
#include "schema/track_route.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cmath>
#include <cstdint>

namespace race_world {

namespace {

using car_physics::ops::Fdivs;
using car_physics::ops::Lfs;

constexpr uint32_t kBackStep = 0x82008E3C;   // the start's step back
constexpr uint32_t kGroupStep = 0x82008E34;  // the step between entries
constexpr uint32_t kRows = 0x82008E14;       // a halfword a row
constexpr uint32_t kSplitSections = 0x82008DE8;

// The words fctiwz then stfd leave, as the translation does: a NaN
// 0x80000000 (high word 0), INT_MAX and above INT_MAX, else the truncated
// value sign-extended (INT_MIN below the range).
uint64_t FctiwzBits(double value) {
  if (std::isnan(value)) {
    return 0x80000000ull;
  }
  if (value >= double(INT32_MAX)) {
    return uint64_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return uint64_t(int64_t(INT32_MIN));
  }
  return uint64_t(int64_t(int32_t(value)));
}

void StoreDouble(uint32_t address, uint64_t bits) {
  guest::StoreU32(address, uint32_t(bits >> 32));
  guest::StoreU32(address + 4, uint32_t(bits));
}

using Group = schema::EventGroup;
using GroupEntry = schema::EventGroupEntry;
using Position = schema::TrackRoutePosition;

int32_t Count(uint32_t list) {
  return int16_t(Group(list).open());
}

// A group's list: the event's +36 + 12 x group (the original's
// event + 12 x (group + 3)).
uint32_t GroupList(uint32_t event, uint32_t group) {
  return schema::RaceEvent(event).group(guest::LoadU32(group)).address;
}

// Entry `i`'s list index (+2), PlaceEntry's `index`.
uint32_t ListIndex(uint32_t entries, uint32_t i) {
  return GroupEntry(nullptr, entries + GroupEntry::kSize * i).list_index_address();
}

}  // namespace

void PlaceGroup(uint32_t event, uint32_t position, uint32_t step, uint32_t group,
                uint32_t flag, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82143A40 RaceEvent::PlaceGroup");
  const uint32_t list = GroupList(event, group);
  const uint32_t step_at = frame + 230;
  const uint32_t copy = frame + 80;
  guest::StoreU16(step_at, uint16_t(step));
  const uint32_t masks = ai_drivers::BranchMasksFor(group);
  const uint32_t start = guest::LoadU32(position);
  const int32_t flagged = int32_t(guest::LoadU32(flag));
  const uint32_t entries = Group(list).entries().address;
  guest::StoreU32(copy, start);
  int32_t first = Count(list);
  if (flagged != 0 && first > 0) {
    first = (first + 2) / 2;
  }
  int32_t i = 0;
  if (first > 0) {
    for (int32_t k = 0; k < first; k++) {
      PlaceEntry(event, kRoute, ListIndex(entries, uint32_t(k)), copy, step_at, masks);
    }
    i = first;
  }
  if (int32_t(guest::LoadU32(flag)) != 0) {
    const uint16_t back = uint16_t(-int32_t(int16_t(step)));
    guest::StoreU32(copy, Position(position).packed());
    const uint16_t slice = Position(copy).slice();
    guest::StoreU16(step_at, back);
    Position(copy).set_slice(uint16_t(slice + back));
  }
  for (; i < Count(list); i++) {
    PlaceEntry(event, kRoute, ListIndex(entries, uint32_t(i)), copy, step_at, masks);
  }
}

void PlaceGroupTwoRows(uint32_t event, uint32_t first, uint32_t second,
                       uint32_t step, uint32_t group, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82143B78 RaceEvent::PlaceGroupTwoRows");
  const uint32_t list = GroupList(event, group);
  const uint32_t one = frame + 188;
  const uint32_t two = frame + 196;
  guest::StoreU32(one, first);
  guest::StoreU32(two, second);
  const uint32_t masks = ai_drivers::BranchMasksFor(group);
  const uint32_t entries = Group(list).entries().address;
  track_route::ToMainRoad(kRoute, one, 0, frame - 144);
  track_route::ToMainRoad(kRoute, two, 0, frame - 144);
  for (int32_t i = 0; i < Count(list); i += 2) {
    PlaceEntry(event, kRoute, ListIndex(entries, uint32_t(i)), one, step, masks);
  }
  for (int32_t i = 1; i < Count(list); i += 2) {
    PlaceEntry(event, kRoute, ListIndex(entries, uint32_t(i)), two, step, masks);
  }
}

void PlaceGroupSplit(uint32_t event, uint32_t position, uint32_t step,
                     uint32_t group, uint32_t row, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82143920 RaceEvent::PlaceGroupSplit");
  const uint32_t list = GroupList(event, group);
  const uint32_t copy = frame + 204;
  guest::StoreU32(copy, position);
  const uint32_t masks = ai_drivers::BranchMasksFor(group);
  const uint32_t entries = Group(list).entries().address;
  const int32_t first = (Count(list) + 1) / 2;
  int32_t i = 0;
  if (first > 0) {
    for (int32_t k = 0; k < first; k++) {
      PlaceEntry(event, kRoute, ListIndex(entries, uint32_t(k)), copy, step, masks);
    }
    i = first;
  }
  // The row's section from the two tables, slice 0, on the main road.
  const int32_t index =
      int16_t(guest::LoadU16((guest::LoadU32(row) << 1) + kRows));
  Position(copy).set_slice(0);
  Position(copy).set_section(guest::LoadU16((uint32_t(index) << 1) + kSplitSections));
  track_route::ToMainRoad(kRoute, copy, 0, frame - 144);
  for (; i < Count(list); i++) {
    PlaceEntry(event, kRoute, ListIndex(entries, uint32_t(i)), copy, step, masks);
  }
}

void PlaceGroupFixed(uint32_t event, uint32_t step, uint32_t group,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821443B0 RaceEvent::PlaceGroupFixed");
  const uint32_t g = guest::LoadU32(group);
  guest::StoreU16(frame + 82, 0);
  guest::StoreU16(frame + 86, 0);
  uint16_t one = 0;
  uint16_t two = 0;
  switch (g) {
    case 0:
      one = 284;
      two = 218;
      break;
    case 1:
      one = 153;
      two = 107;
      break;
    case 2:
    case 3: {
      guest::StoreU16(frame + 80, g == 2 ? 44 : 66);
      const uint16_t s = guest::LoadU16(step);
      guest::StoreU32(frame + 84, 1);
      PlaceGroup(event, frame + 80, s, group, frame + 84, frame - 192);
      return;
    }
    default:
      return;
  }
  guest::StoreU16(frame + 80, one);
  guest::StoreU16(frame + 84, two);
  PlaceGroupTwoRows(event, guest::LoadU32(frame + 80),
                    guest::LoadU32(frame + 84), step, group, frame - 160);
}

void PlaceStarts(uint32_t event, uint32_t kind, uint32_t player, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82144798 RaceEvent::PlaceStarts");
  const schema::TrackRouteFile file = schema::TrackRoute(kRoute).file();
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  const uint32_t position = frame + 92;
  const uint32_t step_at = frame + 80;
  const uint32_t row_at = frame + 84;
  const uint32_t group_at = frame + 88;
  const uint32_t flag_at = frame + 96;
  guest::StoreU32(position, Position(player).packed());
  const int32_t unset = int32_t(FirstUnsetFlag(save));
  const double length = double(file.slice_length());
  const uint16_t slice = Position(position).slice();
  const double back_step = Lfs(kBackStep);
  guest::StoreU32(group_at, 0);
  const double back = Fdivs(back_step, double(file.slice_length()));
  guest::StoreU32(row_at, 0xFFFFFFFF);
  const double between = Fdivs(Lfs(kGroupStep), length);
  StoreDouble(frame + 96, FctiwzBits(back));
  const uint16_t moved = uint16_t(slice - guest::LoadU16(frame + 102));
  StoreDouble(frame + 104, FctiwzBits(between));
  Position(position).set_slice(moved);
  const uint16_t step = guest::LoadU16(frame + 110);
  guest::StoreU16(step_at, step);
  track_route::ToMainRoad(kRoute, position, 1, frame - 144);
  // The kind's row (in the table at 0x82008E14) and the group placed by it
  // (a jump table; sub_82144874 is its first case); above 19, neither.
  int32_t row = -1;
  int32_t special = -1;
  const uint32_t k = guest::LoadU32(kind);
  if (k <= 19) {
    static constexpr int8_t kRow[20] = {0, 1, 0, 1, 2, 3, 2, 3, 0, 1,
                                        0, 1, 4, 5, 0, 1, 2, 3, 4, 5};
    static constexpr int8_t kSpecial[20] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
                                            0, 0, 2, 3, 0, 0, 1, 1, 2, 3};
    row = kRow[k];
    special = kSpecial[k];
    guest::StoreU32(row_at, uint32_t(row));
  }
  guest::StoreU32(group_at, 0);
  for (int32_t g = 0;;) {
    if (g != special) {
      PlaceGroupFixed(event, step_at, group_at, frame - 96);
    } else if (guest::LoadU16((uint32_t(row) << 1) + kRows) < 0x8000) {
      if (unset > 0) {
        PlaceGroupSplit(event, guest::LoadU32(position), step_at, group_at, row_at,
                        frame - 176);
      } else {
        guest::StoreU32(flag_at, 0);
        PlaceGroup(event, position, step, group_at, flag_at, frame - 192);
      }
    } else {
      guest::StoreU32(flag_at, 1);
      PlaceGroup(event, position, step, group_at, flag_at, frame - 192);
    }
    g = int32_t(guest::LoadU32(group_at)) + 1;
    guest::StoreU32(group_at, uint32_t(g));
    if (g >= 4) {
      break;
    }
  }
}

void PlaceStartsCurrent(uint32_t kind, uint32_t player, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82144A38 RaceEvent::PlaceStartsCurrent");
  PlaceStarts(guest::LoadU32(kRaceEvent), kind, player, sp - 176);
}

}  // namespace race_world
