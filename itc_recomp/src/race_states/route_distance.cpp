// The race's route at its start (docs/functions/race-states.md):
// RaceProgress::StartRoute, which sets each racer's start and first listed
// section, and RaceProgress::RouteDistance, which walks the stage's course
// route from its first section to its last into the progress's path (+6636,
// the last index then the count at +7148) and measures the race's length
// (+7156) and the laps' distances (+7164 on) along it.

#include "race_states/race_states.h"
#include "race_states/steps.h"

#include "base/log.h"
#include "car_physics/float_ops.h"
#include "race_setup/race_setup.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_states {

using namespace car_physics::ops;
using namespace steps;
namespace va = vehicle_accessors;

namespace {

constexpr uint32_t kPath = 6636;      // the route's sections walked (halfwords)
constexpr uint32_t kPathLast = 7148;  // the last's index while walking, then the count
constexpr uint32_t kLength = 7156;    // the race's length
constexpr uint32_t kLaps = 7164;      // the laps' distances (4 floats)
// RouteDistance's frame: the link lists (0, 1, 2, -1 at +128; 6, 7, 8, 9,
// -1 at +144), the course route's copy at +176 (its pairs, then from +20 its
// words: the start and the sections after it, -1 at the end), CourseRoutes::
// Get's out at +368.
constexpr uint32_t kRouteDistanceFrame = 656;
constexpr uint32_t kForward = 128, kSide = 144, kCopy = 176, kGetOut = 368;
// StartRoute's frame: the copy at +96, CourseRoutes::Get's out at +288.
constexpr uint32_t kStartCopy = 96, kStartGetOut = 288;

uint32_t PathAt(uint32_t progress, uint32_t i) {
  return progress + ((i + 3318) << 1);
}

// A section's record by its number (the originals' rlwinm by 2 into bits
// 14-29, or a halfword's rotation by 2).
uint32_t Record(uint8_t* base, uint32_t section) {
  return SectionRecord(base, section);
}

// The rows' test (case A and B): the record's byte +3 is `wanted` and its
// kind (+2's low four bits) 0 or 5.
bool Fits(uint8_t* base, uint32_t record, uint32_t wanted) {
  if (guest::LoadU8(base, record + 3) != wanted) {
    return false;
  }
  const uint32_t kind = guest::LoadU8(base, record + 2) & 0xF;
  return kind == 0 || kind == 5;
}

// The first link (from the list at `list`, -1 ended) of `record` not -1
// whose section's record has one of the row's three bytes (case A).
bool FindRow3(uint8_t* base, uint32_t record, uint32_t list, uint32_t row, int32_t* found) {
  for (uint32_t i = 0;; i++) {
    const int32_t link_index = int32_t(Load(base, list + 4 * i));
    if (link_index == -1) {
      return false;
    }
    const int32_t link = Link(base, record, uint32_t(link_index));
    if (link == -1) {
      continue;
    }
    const uint32_t target = Record(base, uint32_t(link));
    for (uint32_t j = 0; j < 3; j++) {
      // The original's test of the byte against -1 never holds (lbzx).
      if (Fits(base, target, guest::LoadU8(base, row + j))) {
        *found = link;
        return true;
      }
    }
  }
}

// The first link of `record` (from the list at `list`) not -1 whose
// section's record has the byte `wanted` (case B).
bool FindByte(uint8_t* base, uint32_t record, uint32_t list, uint32_t wanted, int32_t* found) {
  for (uint32_t i = 0;; i++) {
    const int32_t link_index = int32_t(Load(base, list + 4 * i));
    if (link_index == -1) {
      return false;
    }
    const int32_t link = Link(base, record, uint32_t(link_index));
    if (link == -1) {
      continue;
    }
    if (Fits(base, Record(base, uint32_t(link)), wanted)) {
      *found = link;
      return true;
    }
  }
}

// TrackRoute::PathDistance along the path (+6636, `length` sections) from
// the position word `a` to `b`; its frame below RouteDistance's.
double Along(uint8_t* base, uint32_t progress, uint32_t a, uint32_t b, int32_t length,
             uint32_t frame) {
  return track_route::PathDistance(base, race_world::kRoute, a, b, progress + kPath, length,
                                   frame - 288);
}

// The race's length and the laps' distances (cases A, B and D): the last
// position into `last_at`, the first into `first_at`; each lap's position
// (Pairs::At 1 to 4, until one whose section is 0xFFFF) into `lap_at` and
// +80, the first again into `again_at`.
void Measure(uint8_t* base, uint32_t progress, uint32_t frame, uint32_t last_at,
             uint32_t first_at, uint32_t lap_at, uint32_t again_at) {
  const uint32_t copy = frame + kCopy;
  va::PairsLast(base, frame + last_at, copy);
  va::PairCopy(base, frame + first_at, copy);
  Stfs(base, progress + kLength,
       Along(base, progress, Load(base, frame + first_at), Load(base, frame + last_at),
             int32_t(Load(base, progress + kPathLast)), frame));
  for (uint32_t i = 1; i < 5; i++) {
    va::PairsAt(base, frame + lap_at, copy, i);
    const uint32_t lap = Load(base, frame + lap_at);
    Store(base, frame + 80, lap);
    if (guest::LoadU16(base, frame + 80) == 0xFFFF) {
      return;
    }
    va::PairCopy(base, frame + again_at, copy);
    Stfs(base, progress + kLaps + 4 * (i - 1),
         Along(base, progress, Load(base, frame + again_at), lap,
               int32_t(Load(base, progress + kPathLast)), frame));
  }
}

}  // namespace

bool RouteDistanceNative(uint32_t index, uint32_t frame) {
  return race_setup::RouteCopiesNative(frame, kGetOut, index, kCopy);
}

void RouteDistance(uint8_t* base, uint32_t progress, uint32_t index, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82162330 RaceProgress::RouteDistance");
  const uint32_t lists[9] = {0, 1, 2, 0xFFFFFFFFu, 6, 7, 8, 9, 0xFFFFFFFFu};
  for (uint32_t i = 0; i < 9; i++) {
    Store(base, frame + kForward + 4 * i, lists[i]);
  }
  race_setup::CopyCourseRoute(base, frame, kGetOut, index, 0x821623A0, kCopy);
  const uint32_t copy = frame + kCopy;
  va::PairCopy(base, frame + 80, copy);
  const int32_t start = S16(base, frame + 80);
  va::PairsLast(base, frame + 80, copy);
  const int32_t last = S16(base, frame + 80);
  bool done = false;
  const int32_t kind = int32_t(index);
  const auto begin = [&] {
    Store(base, progress + kPathLast, 0);
    guest::StoreU16(base, progress + kPath, uint16_t(start));
  };

  if (kind == 8 || kind == 9 || kind == 10) {
    // Routes 8 to 10 (a loop): route 8's path follows each section's first
    // forward link back to its start; 9 and 10 keep the start alone.
    begin();
    if (kind != 8) {
      Store(base, progress + kPathLast, 1);
    } else {
      while (!done) {
        const uint32_t at = Load(base, progress + kPathLast);
        const uint32_t record = Record(base, guest::LoadU16(base, PathAt(progress, at)));
        for (uint32_t i = 0;; i++) {
          const int32_t link_index = int32_t(Load(base, frame + kForward + 4 * i));
          if (link_index == -1) {
            break;
          }
          const int32_t link = Link(base, record, uint32_t(link_index));
          if (link == -1) {
            continue;
          }
          if (link == last) {
            done = true;
          }
          Store(base, progress + kPathLast, at + 1);
          guest::StoreU16(base, PathAt(progress, at + 1), uint16_t(link));
          break;
        }
      }
      Store(base, progress + kPathLast, Load(base, progress + kPathLast) + 1);
    }
    const uint32_t copy_at = copy;
    va::PairsLast(base, frame + 124, copy_at);
    va::PairCopy(base, frame + 84, copy_at);
    Stfs(base, progress + kLength,
         Along(base, progress, Load(base, frame + 84), Load(base, frame + 124),
               int32_t(Load(base, progress + kPathLast)), frame));
    for (uint32_t i = 1; i < 5; i++) {
      va::PairsAt(base, frame + 124, copy, i);
      const uint32_t lap = Load(base, frame + 124);
      Store(base, frame + 80, lap);
      const int32_t section = S16(base, frame + 80);
      if (section == -1) {
        break;
      }
      // The whole path, but only the start's section when the lap is on it
      // at another slice than the last position's.
      int32_t length;
      if (section != S16(base, progress + kPath)) {
        length = int32_t(Load(base, progress + kPathLast));
      } else {
        va::PairsLast(base, frame + 84, copy);
        if (section != S16(base, frame + 84)) {
          length = 1;
        } else {
          va::PairsLast(base, frame + 168, copy);
          length = S16(base, frame + 82) == S16(base, frame + 170)
                       ? int32_t(Load(base, progress + kPathLast))
                       : 1;
        }
      }
      va::PairCopy(base, frame + 164, copy);
      Stfs(base, progress + kLaps + 4 * (i - 1),
           Along(base, progress, Load(base, frame + 164), lap, length, frame));
    }
    return;
  }

  if (kind == 6 || kind == 7) {
    // Routes 6 and 7: a row of 14 bytes each; each step's section has the
    // row's next byte (the row's place then moves on), else its current.
    const uint8_t rows[28] = {14, 4, 2, 12, 12, 2, 11, 11, 2, 4, 14, 255, 255, 255,
                              3,  1, 11, 11, 1, 12, 12, 1, 3, 3, 13, 8,   3,   255};
    for (uint32_t i = 0; i < 28; i++) {
      guest::StoreU8(base, frame + 96 + i, rows[i]);
    }
    const uint32_t row = frame + 96 + uint32_t((uint64_t(index) - 6) * 14);
    begin();
    int32_t place = 0;
    while (!done) {
      const int32_t next = place + 1;
      const uint32_t at = Load(base, progress + kPathLast);
      const uint32_t record = Record(base, guest::LoadU16(base, PathAt(progress, at)));
      int32_t pick = next < 14 ? next : 13;
      int32_t link = 0;
      for (;;) {
        // The original's test of the byte against -1 never holds (lbzx).
        const uint32_t wanted = guest::LoadU8(base, row + uint32_t(pick));
        if (FindByte(base, record, frame + kForward, wanted, &link) ||
            FindByte(base, record, frame + kSide, wanted, &link)) {
          break;
        }
        pick = place;
      }
      if (link == last) {
        done = true;
      }
      if (pick == next) {
        place = pick;
      }
      Store(base, progress + kPathLast, at + 1);
      guest::StoreU16(base, PathAt(progress, at + 1), uint16_t(link));
    }
    Store(base, progress + kPathLast, Load(base, progress + kPathLast) + 1);
    Measure(base, progress, frame, 124, 84, 124, 84);
    return;
  }

  if (index > 5) {
    // Routes 11 on: the copy's words from +24 name each next section; one a
    // forward link names is appended, else a side link whose own forward
    // link names it replaces the last (in the path and in the copy's words)
    // and it is appended after.
    begin();
    while (!done) {
      const uint32_t at = Load(base, progress + kPathLast);
      const int32_t wanted = int32_t(Load(base, frame + 200 + 4 * at));
      const uint32_t record = Record(base, guest::LoadU16(base, PathAt(progress, at)));
      bool forward = false;
      for (uint32_t i = 0;; i++) {
        const int32_t link_index = int32_t(Load(base, frame + kForward + 4 * i));
        if (link_index == -1) {
          break;
        }
        const int32_t link = Link(base, record, uint32_t(link_index));
        if (link != -1 && link == wanted) {
          if (link == last) {
            done = true;
          }
          Store(base, progress + kPathLast, at + 1);
          guest::StoreU16(base, PathAt(progress, at + 1), uint16_t(wanted));
          forward = true;
          break;
        }
      }
      if (forward) {
        continue;
      }
      bool replaced = false;
      for (uint32_t s = 0; !replaced; s++) {
        const int32_t side_index = int32_t(Load(base, frame + kSide + 4 * s));
        if (side_index == -1) {
          break;
        }
        const int32_t side = Link(base, record, uint32_t(side_index));
        if (side == -1) {
          continue;
        }
        const uint32_t side_record = Record(base, uint32_t(side) & 0xFFFF);
        for (uint32_t i = 0;; i++) {
          const int32_t link_index = int32_t(Load(base, frame + kForward + 4 * i));
          if (link_index == -1) {
            break;
          }
          const int32_t link = Link(base, side_record, uint32_t(link_index));
          if (link != -1 && link == wanted) {
            if (link == last) {
              done = true;
            }
            guest::StoreU16(base, PathAt(progress, Load(base, progress + kPathLast)),
                            uint16_t(side));
            const uint32_t now = Load(base, progress + kPathLast);
            Store(base, progress + kPathLast, now + 1);
            Store(base, frame + 196 + 4 * now, uint32_t(side));
            guest::StoreU16(base, PathAt(progress, now + 1), uint16_t(wanted));
            replaced = true;
            break;
          }
        }
      }
    }
    Store(base, progress + kPathLast, Load(base, progress + kPathLast) + 1);
    Measure(base, progress, frame, 124, 84, 124, 84);
    return;
  }

  // Routes 0 to 5: a row of three bytes each; each step's section is the
  // first forward link (else side link) whose section has one of them.
  const uint8_t rows[24] = {2, 6, 255, 1,  5, 255, 4,  5, 14, 3, 6, 13,
                            11, 1, 255, 12, 1, 255, 4, 5, 1,  3, 6, 2};
  for (uint32_t i = 0; i < 24; i++) {
    guest::StoreU8(base, frame + 96 + i, rows[i]);
  }
  const uint32_t row = frame + 96 + uint32_t(uint64_t(index) * 3);
  begin();
  while (!done) {
    const uint32_t at = Load(base, progress + kPathLast);
    const uint32_t record = Record(base, guest::LoadU16(base, PathAt(progress, at)));
    int32_t link = 0;
    if (FindRow3(base, record, frame + kForward, row, &link)) {
      if (link == last) {
        done = true;
      }
      Store(base, progress + kPathLast, at + 1);
      guest::StoreU16(base, PathAt(progress, at + 1), uint16_t(link));
    } else if (FindRow3(base, record, frame + kSide, row, &link)) {
      if (link == last) {
        done = true;
      }
      const uint32_t now = Load(base, progress + kPathLast) + 1;
      Store(base, progress + kPathLast, now);
      guest::StoreU16(base, PathAt(progress, now), uint16_t(link));
    }
  }
  Store(base, progress + kPathLast, Load(base, progress + kPathLast) + 1);
  Measure(base, progress, frame, 80, 84, 84, 124);
}

bool StartRouteNative(uint8_t* base, uint32_t machine, uint32_t frame) {
  const uint32_t index = Load(base, Load(base, machine + 9624) + 6076);
  return race_setup::RouteCopiesNative(frame, kStartGetOut, index, kStartCopy) &&
         RouteDistanceNative(index, frame - kRouteDistanceFrame);
}

namespace {

// The first slot (of 4) whose manager entry is there with flag 0x4: the
// address of its vehicle's track position (Vehicle::Identity +4) into
// `position`; false when none.
bool FirstRacer(uint8_t* base, uint32_t progress, uint32_t* position) {
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t entry = Load(base, progress + 48 + kSlotBytes * i);
    if (entry != 0 && (Load(base, entry) & 0x4) != 0) {
      const uint32_t vehicle = va::VehicleGet(base, Load(base, entry + 20));
      *position = va::VehicleIdentity(vehicle) + 4;
      return true;
    }
  }
  return false;
}

// Each results entry's slot start (+572, +574 = the start's halfwords
// +7152, +7154), the count +2184 read again each pass.
void SetStarts(uint8_t* base, uint32_t progress) {
  if (int32_t(Load(base, progress + 2184)) <= 0) {
    return;
  }
  uint32_t at = progress + 572;
  for (int32_t i = 0;;) {
    guest::StoreU16(base, at + 0, guest::LoadU16(base, progress + 7152));
    guest::StoreU16(base, at + 2, guest::LoadU16(base, progress + 7154));
    i++;
    at += kSlotBytes;
    if (!(i < int32_t(Load(base, progress + 2184)))) {
      return;
    }
  }
}

// The halfword at `from` appended to the list at `list` (count at `count`,
// 256 at most).
void Append(uint8_t* base, uint32_t list, uint32_t count, uint32_t from) {
  const int32_t n = int32_t(Load(base, count));
  if (n >= 256) {
    return;
  }
  guest::StoreU16(base, list + 2 * uint32_t(n), guest::LoadU16(base, from));
  Store(base, count, Load(base, count) + 1);
}

}  // namespace

void StartRoute(uint8_t* base, uint32_t machine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_821635D0 RaceProgress::StartRoute");
  const auto progress = [&] { return Load(base, machine + 9624); };
  const uint32_t first = progress();
  const int32_t type = int32_t(Load(base, first + 6100));
  if (type != 3 && type != 1) {
    // The start the first racer's position; each slot's list and the race's
    // get it.
    uint32_t position = 0;
    if (FirstRacer(base, first, &position)) {
      const uint32_t p = progress();
      Store(base, p + 7152, Load(base, position));
      SetStarts(base, p);
    }
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t p = progress();
      if (Load(base, p + kSlotBytes * i + 48) == 0) {
        continue;
      }
      Store(base, frame + 80, Load(base, p + 7152));
      const uint32_t slot = p + kSlotBytes * i + 40;
      Append(base, slot + 12, slot + 524, frame + 80);
    }
    const uint32_t p = progress();
    Store(base, frame + 80, Load(base, p + 7152));
    Append(base, p + 6120, p + 6632, frame + 80);
    if (int32_t(Load(base, progress() + 6100)) == 1) {
      const uint32_t q = progress();
      RouteDistance(base, q, Load(base, q + 6076), frame - kRouteDistanceFrame);
    }
    return;
  }
  // Types 1 and 3: the start the course route's first position; the path
  // walked and measured; each slot's list and the race's get the path up to
  // the first racer's section (or a section beside it).
  race_setup::CopyCourseRoute(base, frame, kStartGetOut, Load(base, first + 6076), 0x8216375C,
                              kStartCopy);
  va::PairCopy(base, frame + 80, frame + kStartCopy);
  {
    const uint32_t p = progress();
    Store(base, p + 7152, Load(base, frame + 80));
    SetStarts(base, p);
  }
  {
    const uint32_t p = progress();
    RouteDistance(base, p, Load(base, p + 6076), frame - kRouteDistanceFrame);
  }
  int32_t section = -1;
  {
    uint32_t position = 0;
    if (FirstRacer(base, progress(), &position)) {
      Store(base, frame + 80, Load(base, position));
      section = int16_t(guest::LoadU16(base, frame + 80));
    }
  }
  const uint32_t p = progress();
  const int32_t count = int32_t(Load(base, p + kPathLast));
  if (count <= 0) {
    return;
  }
  int32_t found = 0;
  for (;; found++) {
    if (found >= count) {
      return;
    }
    const int32_t listed = S16(base, p + kPath + 2 * uint32_t(found));
    if (section == listed ||
        SectionsBeside(base, uint32_t(section), uint32_t(listed), frame) != 0) {
      break;
    }
  }
  uint32_t offset = kPath;
  for (int32_t left = found + 1; left != 0; left--, offset += 2) {
    for (uint32_t i = 0; i < 4; i++) {
      if (Load(base, progress() + kSlotBytes * i + 48) == 0) {
        continue;
      }
      const uint32_t q = progress();
      const uint32_t slot = q + kSlotBytes * i + 40;
      Append(base, slot + 12, slot + 524, q + offset);
    }
    const uint32_t q = progress();
    Append(base, q + 6120, q + 6632, q + offset);
  }
}

}  // namespace race_states
