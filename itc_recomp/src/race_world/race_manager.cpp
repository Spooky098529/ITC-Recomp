// The racers' manager's lists (docs/functions/race-world.md):
// RaceManager::ListRacers and RaceManager::AddRacers. See race_world.h.

#include "race_world/race_world.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "race_setup/race_setup.h"
#include "racer_controllers/racer_controllers.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cmath>
#include <cstdint>

namespace race_world {

using namespace car_physics::ops;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

constexpr uint32_t kVehicles = 0x826C2E34;   // the seven vehicles (a pointer)
constexpr uint32_t kInputs = 0x826C5B30;     // the racers' input records
constexpr uint32_t kFarKey = 0x8200222C;     // the key of entries flagged 0x10 or 0x400
constexpr uint32_t kTrafficGap = 0x82009968;  // the traffic's spacing
constexpr uint32_t kNearest = 0x8200996C;    // the four nearest racers' limit
constexpr uint32_t kTrafficBack = 0x820029D0;

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// A halfword sign-extended to a register (extsh, lha).
uint32_t S16(uint32_t value) {
  return uint32_t(int32_t(int16_t(uint16_t(value))));
}
uint32_t LoadS16(uint8_t* base, uint32_t address) {
  return S16(guest::LoadU16(base, address));
}
// The halfwords of a route position copied (lhz, sth twice).
void CopyPosition(uint8_t* base, uint32_t from, uint32_t to) {
  guest::StoreU16(base, to, guest::LoadU16(base, from));
  guest::StoreU16(base, to + 2, guest::LoadU16(base, from + 2));
}
// A section's record in the route's table (the halfword at `at`, shifted).
uint32_t SectionRecord(uint8_t* base, uint32_t file, uint32_t section) {
  return Load(base, Load(base, file + 12) + (section << 2));
}
// fctiwz's low word, as the translation leaves it (stfiwx).
uint32_t FctiwzWord(double value) {
  if (std::isnan(value)) {
    return 0x80000000u;
  }
  if (value >= double(INT32_MAX)) {
    return uint32_t(INT32_MAX);
  }
  if (value <= -2147483649.0) {
    return 0x80000000u;
  }
  return uint32_t(int32_t(value));
}
// divw as the translation does it: 0 for a zero divisor or INT_MIN / -1.
int32_t Divw(int32_t a, int32_t b) {
  return b != 0 && !(a == INT32_MIN && b == -1) ? a / b : 0;
}
// An input record cleared on the frame, copied to racer `index`'s
// ([0x826C5B30 + 4 x the index's low byte]).
void GiveInput(uint8_t* base, uint32_t input, uint32_t index) {
  ai_drivers::ClearCarInput(base, input);
  const uint32_t to = Load(base, kInputs + ((index << 2) & 0x3FC));
  for (uint32_t w = 0; w < 5; w++) {
    Store(base, to + 4 * w, Load(base, input + 4 * w));
  }
}
// An entry's distance to the player's slice point: both slices' centers
// (TrackRoute::SlicePoint, point 0: its plain C++ always answers it) at
// `frame` + `entry_at` and + `player_at`, entry less player at + 96, its y =
// `y` (and its w = `w` when given); its length (Vec3::Normalize, in place).
double FlatDistance(uint8_t* base, uint32_t frame, uint32_t section, uint32_t player_section,
                    uint32_t entry_at, uint32_t player_at, double y, const double* w) {
  track_route::SlicePoint(base, frame + entry_at, 0, S16(section), LoadS16(base, frame + 82));
  track_route::SlicePoint(base, frame + player_at, 0, S16(player_section),
                          LoadS16(base, frame + 86));
  vl::Vec3Sub(base, frame + 96, frame + entry_at, frame + player_at);
  Stfs(base, frame + 100, y);
  if (w != nullptr) {
    Stfs(base, frame + 108, *w);
  }
  return double(vl::Vec3Normalize(base, frame + 96, frame + 96));
}

}  // namespace

uint64_t ListRacers(uint8_t* base, uint32_t manager, uint32_t frame, bool heap,
                    const OriginalR3& original) {
  ITC_NATIVE_RUNNING("sub_82171E50 RaceManager::ListRacers");
  // The player's entry: the first flagged 0x4, or none (0: its +20 read at
  // 20, as the original).
  uint32_t player = 0;
  if (int32_t(Load(base, manager + 2368)) > 0) {
    for (int32_t i = 0;;) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      if ((Load(base, entry) & 0x4) != 0) {
        player = entry;
        break;
      }
      i++;
      if (!(i < int32_t(Load(base, manager + 2368)))) {
        break;
      }
    }
  }
  const uint32_t index = Load(base, player + 20);
  const uint32_t array = Load(base, kVehicles);
  const uint32_t vehicle = va::VehicleGet(base, index);
  Load(base, vehicle + 336);  // RigidBody::Position's argument (its answer unused)
  const uint32_t identity = va::VehicleIdentity(vehicle);
  // Vehicle::Identity's 64-bit r3 (Vehicle::Get's from the zero-extended
  // index), the original's r3 when no entry sets another.
  uint64_t r3 = (array == 0 ? 0 : uint64_t(index) * 6272 + array) + 5232;
  const int32_t count = int32_t(Load(base, manager + 2368));
  const uint32_t position = Load(base, identity + 4);
  Store(base, frame + 84, position);
  const uint32_t player_section = guest::LoadU16(base, frame + 84);
  const uint32_t player_record = SectionRecord(base, Load(base, kRoute), player_section);
  Store(base, manager + 2364, 0);
  Store(base, frame + 1748, 0);
  Store(base, manager + 2372, 0);
  Store(base, frame + 1752, 0);
  Store(base, frame + 1744, 0);
  if (count > 0) {
    const double one = Lfs(base, kOne);
    const double far_key = Lfs(base, kFarKey);
    const double zero = Lfs(base, kZero);
    for (int32_t i = 0; i < int32_t(Load(base, manager + 2368)); i++) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      const uint32_t flags = Load(base, entry);
      bool lose = (flags & 0x20) != 0 || (flags & 0x80) != 0;
      double key = 0.0;
      if (lose) {
      } else if ((flags & 0x4) != 0) {
        key = zero;
      } else if ((flags & 0x10) != 0 || (flags & 0x400) != 0) {
        key = far_key;
      } else {
        const uint32_t word = Load(base, entry + 8);
        const uint32_t file = Load(base, kRoute);
        const uint32_t a = guest::LoadU8(base, player_record + 3);
        Store(base, frame + 80, word);
        const uint32_t section = guest::LoadU16(base, frame + 80);
        const uint32_t b = guest::LoadU8(base, SectionRecord(base, file, section) + 3);
        r3 = BranchesMeet(a, b);
        if (r3 == 0) {
          lose = true;
        } else {
          // SlicePoint at + 112 (the entry) and + 128 (the player), the
          // difference's w = [0x82000A30] after its y = 0.0.
          key = Fadds(FlatDistance(base, frame, section, player_section, 112, 128, zero, &one),
                      one);
        }
      }
      if (!lose) {
        SortedListInsert(base, frame + 144, entry, key);
        r3 = uint64_t(frame) + 144;
        continue;
      }
      // The entry loses its vehicle.
      const uint32_t which = Load(base, entry + 20);
      r3 = which;
      if (int32_t(which) == -1 || int32_t(Load(base, manager + ((which + 584) << 2))) <= 0) {
        continue;
      }
      Store(base, entry, flags & ~0x40u);
      const uint32_t car = va::VehicleGet(base, which);
      uint32_t removed = 0;
      if (heap && vehicle::VehicleRemoveFromWorld(base, car, &removed)) {
        r3 = removed;
      } else {
        r3 = original(0x820C20E0, frame, car);
      }
      Store(base, manager + ((Load(base, entry + 20) + 584) << 2), 0);
      Store(base, entry + 20, 0xFFFFFFFFu);
    }
  }
  // The listed racers with a vehicle, nearest first.
  for (uint32_t node = Load(base, frame + 1744); node != 0; node = Load(base, node + 12)) {
    const uint32_t entry = Load(base, node);
    if (int32_t(Load(base, entry + 20)) == -1) {
      continue;
    }
    Store(base, manager + ((Load(base, manager + 2364) + 577) << 2), entry);
    const uint32_t listed = Load(base, manager + 2364);
    const uint32_t racers = Load(base, manager + 2372);
    Store(base, manager + 2364, listed + 1);
    Store(base, manager + 2372, racers + 1);
  }
  // The traffic entries in use.
  Store(base, manager + 2376, 0);
  for (uint32_t t = 0; t < 3; t++) {
    const uint32_t entry = manager + 2160 + 28 * t;
    if (Load(base, entry) == 0) {
      continue;
    }
    Store(base, manager + ((Load(base, manager + 2364) + 577) << 2), entry);
    const uint32_t listed = Load(base, manager + 2364);
    const uint32_t traffic = Load(base, manager + 2376);
    Store(base, manager + 2364, listed + 1);
    Store(base, manager + 2376, traffic + 1);
  }
  return r3;
}

void AddRacers(uint8_t* base, uint32_t manager, uint32_t records, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82173050 RaceManager::AddRacers");
  const double slice_length = Lfs(base, Load(base, kRoute) + 8);
  BuildNeighbors(base, manager + 2392, frame - 336);
  const uint32_t count_at = records + 0x595E0;
  // The player's record: the first flagged 0x1, or none (0: its position
  // read at 3904, as the original).
  uint32_t player = 0;
  {
    const int32_t n = int32_t(Load(base, count_at));
    for (int32_t i = 0; i < n; i++) {
      const uint32_t record = records + 3936 * uint32_t(i);
      if ((Load(base, record + 3888) & 0x1) != 0) {
        player = record;
        break;
      }
    }
  }
  const uint32_t player_at = player + 3904;
  if (int32_t(Load(base, Load(base, kRaceData) + 0xA3AF0)) == 1 &&
      int32_t(Load(base, Load(base, kRaceData) + 0xA3AF4)) == 2 &&
      race_setup::EventCurrent(base) != 0) {
    const uint32_t save = va::SaveDataOf(base, 0);
    Store(base, frame + 84, Load(base, save + 6108));
    PlaceStartsCurrent(base, frame + 84, player_at, frame);
  }
  bool back = false;
  if (int32_t(Load(base, Load(base, kRaceData) + 0xA3AF0)) == 1) {
    const int32_t value = int32_t(Load(base, va::SaveDataOf(base, 0) + 6100));
    back = value <= 999 && value == 1;
  }
  // The racers' records: their start positions and entries; the traffic's
  // records listed.
  {
    int32_t forward = 1, backward = -1, further = 30;
    for (int32_t i = 0; i < int32_t(Load(base, count_at)); i++) {
      const uint32_t record = records + 3936 * uint32_t(i);
      const uint32_t start = record + 3904;
      const uint32_t flags = Load(base, record + 3888);
      if ((flags & 0x83) == 0) {
        if ((flags & 0x4) != 0) {
          Store(base, manager + ((Load(base, manager + 2384) + 561) << 2), uint32_t(i));
          Store(base, manager + 2384, Load(base, manager + 2384) + 1);
        }
        continue;
      }
      if ((flags & 0x1) == 0 && (flags & 0x80) == 0) {
        if (guest::LoadU16(base, start) == 0xFFFF) {
          CopyPosition(base, player_at, start);
        }
        const uint32_t stages = Load(base, kRaceData) + 0x59620;
        if (va::StagesHasRacer(base, stages, uint32_t(i), 0xFFFFFFFFu) != 0) {
          const uint32_t item_id = Load(base, start + 8);
          CopyPosition(base, player_at, start);
          const uint32_t kind =
              int32_t(item_id) < 0
                  ? 0xFFFFFFFFu
                  : uint32_t(int32_t(int8_t(guest::LoadU8(
                        base, data_tables::Item(base, item_id) + 36))));
          if (back) {
            StepToMainRoad(base, start, S16(uint32_t(backward)), S16(kind), frame - 112);
          } else {
            vehicle::RoutePositionStep(base, start, S16(uint32_t(forward)), S16(kind),
                                       frame - 112);
          }
          forward++;
          backward--;
        } else if (int32_t(Load(base, start + 8)) >= 0 && int32_t(Load(base, start + 12)) >= 0) {
          const uint32_t event_entry = RaceEventEntry(base, start + 12);
          further += 30;
          CopyPosition(base, event_entry + 4, start);
        } else {
          StepToMainRoad(base, start, S16(uint32_t(further)), 0xFFFFFFFFu, frame - 112);
          further += 30;
        }
      }
      const uint32_t entry = manager + 4 + 28 * Load(base, manager + 2368);
      Store(base, entry + 24, uint32_t(i));
      Store(base, entry + 4, Load(base, manager + 2368));
      CopyPosition(base, start, entry + 8);
      Store(base, entry, 1);
      const uint32_t again = Load(base, record + 3888);
      if ((again & 0x1) != 0) {
        Store(base, entry, 4);
      } else if ((again & 0x80) != 0) {
        Store(base, entry, 8);
      }
      if ((Load(base, record + 3888) & 0x100) != 0) {
        Store(base, entry, Load(base, entry) | 0x20);
      }
      Store(base, manager + 2368, Load(base, manager + 2368) + 1);
    }
  }
  // The entries on the player's branches sorted by distance (the list at
  // `frame` + 208).
  const uint32_t position = Load(base, player_at);
  Store(base, frame + 84, position);
  const uint32_t player_section = guest::LoadU16(base, frame + 84);
  const uint32_t player_record = SectionRecord(base, Load(base, kRoute), player_section);
  const int32_t entries = int32_t(Load(base, manager + 2368));
  Store(base, frame + 1812, 0);
  Store(base, frame + 1816, 0);
  Store(base, frame + 1808, 0);
  if (entries > 0) {
    const double zero = Lfs(base, kZero);
    for (int32_t i = 0; i < int32_t(Load(base, manager + 2368)); i++) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      const uint32_t flags = Load(base, entry);
      if ((flags & 0x20) != 0) {
        continue;
      }
      double key = zero;
      if ((flags & 0x4) == 0) {
        const uint32_t word = Load(base, entry + 8);
        const uint32_t file = Load(base, kRoute);
        const uint32_t a = guest::LoadU8(base, player_record + 3);
        Store(base, frame + 80, word);
        const uint32_t section = guest::LoadU16(base, frame + 80);
        const uint32_t b = guest::LoadU8(base, SectionRecord(base, file, section) + 3);
        if (BranchesMeet(a, b) == 0) {
          continue;
        }
        // SlicePoint at + 128 (the entry) and + 112 (the player).
        key = FlatDistance(base, frame, section, player_section, 128, 112, zero, nullptr);
      }
      SortedListInsert(base, frame + 208, entry, key);
    }
  }
  // The nearest four within [0x8200996C] get a vehicle and a driver.
  Store(base, manager + 2372, 0);
  for (uint32_t node = entries > 0 ? Load(base, frame + 1808) : 0; node != 0;
       node = Load(base, node + 12)) {
    const uint32_t entry = Load(base, node);
    if (Load(base, manager + 2372) < 4 && Lfs(base, node + 4) < Lfs(base, kNearest)) {
      Store(base, entry, Load(base, entry) | 0x40);
      const uint32_t index_at = entry + 20;
      Store(base, index_at, Load(base, manager + 2364));
      Store(base, manager + ((Load(base, manager + 2364) + 577) << 2), entry);
      Store(base, manager + ((Load(base, manager + 2364) + 584) << 2), 1);
      GiveInput(base, frame + 144, Load(base, index_at));
      Store(base, frame + 84, 1);
      const uint32_t id =
          Load(base, entry + 24) * 3936 + Load(base, kRaceData) + 3960;
      racer_controllers::Setup(base, index_at, id, frame + 84, frame);
      const uint32_t racers = Load(base, manager + 2372);
      const uint32_t listed = Load(base, manager + 2364);
      Store(base, manager + 2372, racers + 1);
      Store(base, manager + 2364, listed + 1);
    } else {
      Store(base, entry, Load(base, entry) & ~0x40u);
    }
  }
  // The traffic: up to three entries (+2160) spread before the player.
  uint32_t traffic = Load(base, records + 0x595E8);
  Store(base, manager + 2380, traffic);
  if (traffic > 3) {
    Store(base, manager + 2380, 3);
  }
  traffic = Load(base, manager + 2380);
  Store(base, manager + 2376, 0);
  if (traffic == 0 || Load(base, manager + 2384) == 0 ||
      (Load(base, Load(base, kRaceData) + 0x59AE4) & 0x2000) != 0) {
    return;
  }
  const double gap = Lfs(base, kTrafficGap);
  Store(base, frame + 84, position);
  const double per_slice = Fdivs(Lfs(base, kOne), slice_length);
  const uint32_t spacing = Load(base, Load(base, kRaceData) + 0xA3AE0);
  const int64_t spread = int64_t(int32_t(traffic + 1));
  Store(base, frame + 96, uint32_t(uint64_t(spread) >> 32));
  Store(base, frame + 100, uint32_t(spread));
  const double spread_f = double(float(double(spread)));
  const double behind = Fmsubs(per_slice, gap, Lfs(base, kTrafficBack));
  const double each = Fmuls(Fdivs(gap, spread_f), per_slice);
  Store(base, frame + 88, FctiwzWord(each));
  Store(base, frame + 80, FctiwzWord(behind));
  StepToMainRoad(base, frame + 84, LoadS16(base, frame + 82), 0xFFFFFFFFu, frame - 112);
  const int32_t slices = int32_t(Load(base, frame + 88));
  Store(base, frame + 88, 1);
  const int32_t half = slices / 2;
  const uint32_t step = (0u - uint32_t(slices)) << 1;
  const int32_t quarter = slices / 4;
  Store(base, frame + 96, 0xFFFFFFFFu);
  uint32_t offset = step, counter = 0;
  uint64_t model = 0;
  for (uint32_t t = 0; int32_t(t) < int32_t(traffic); t++) {
    const uint32_t entry = manager + 2160 + 28 * t;
    const uint32_t start = Load(base, frame + 84);
    const uint32_t old = Load(base, entry + 16);
    if (half == 0) {
      ITC_LOG_WARN("RaceManager::AddRacers: tw/td trap hit (type 0), as the original's");
    }
    const uint32_t digit = counter - uint32_t(Divw(int32_t(counter), 10)) * 10;
    Store(base, frame + 80, start);
    const uint32_t sum = Load(base, Load(base, kRaceData) + 0xA3AE0) + uint32_t(model);
    if (int32_t(sum) == INT32_MIN && half == -1) {
      ITC_LOG_WARN("RaceManager::AddRacers: tw/td trap hit (type 0), as the original's");
    }
    const uint32_t remainder = sum - uint32_t(Divw(int32_t(sum), half) * half);
    const uint32_t number = digit + old + t;
    uint32_t ahead = offset - remainder + uint32_t(quarter);
    ahead = int32_t(ahead) > 0 ? ahead + 15 : ahead - 15;
    StepToMainRoad(base, frame + 80, S16(ahead), 0xFFFFFFFFu, frame - 112);
    const uint32_t file = Load(base, kRoute);
    const uint32_t branch =
        guest::LoadU8(base, SectionRecord(base, file, guest::LoadU16(base, frame + 80)) + 3);
    model = PickModel(base, number, branch, frame);
    const uint32_t record = Load(base, manager + ((uint32_t(model) + 561) << 2));
    Store(base, entry + 4, uint32_t(model));
    Store(base, entry + 0, 66);
    Store(base, entry + 24, record);
    Store(base, entry + 16, number);
    Store(base, entry + 20, Load(base, manager + 2364));
    for (int32_t n = int32_t(number); n > 42;) {
      n = int32_t(Load(base, entry + 16)) - 42;
      Store(base, entry + 16, uint32_t(n));
    }
    Store(base, manager + ((Load(base, manager + 2364) + 577) << 2), entry);
    Store(base, manager + ((Load(base, manager + 2364) + 584) << 2), 1);
    const uint32_t listed = Load(base, manager + 2364);
    const uint32_t counted = Load(base, manager + 2376);
    Store(base, manager + 2364, listed + 1);
    Store(base, manager + 2376, counted + 1);
    GiveInput(base, frame + 176, Load(base, entry + 20));
    racer_controllers::Setup(base, entry + 20, frame + 96, frame + 88, frame);
    const uint32_t placed = Load(base, frame + 80);
    offset += step;
    counter += spacing;
    Store(base, entry + 8, placed);
  }
}

}  // namespace race_world
