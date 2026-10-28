// The race world's leaves, the sorted lists, the route positions and the race
// event's start placements (docs/functions/race-world.md). See
// race_world.h.

#include "race_world/race_world.h"

#include "car_physics/float_ops.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_world {

using namespace car_physics::ops;
namespace va = vehicle_accessors;

namespace {

uint32_t Manager(uint8_t* base) {
  return guest::LoadU32(base, kManager);
}

// Traffic::PickModel's two tables, as the original writes them.
constexpr uint32_t kModels[2][42] = {
    {55, 62, 56, 64, 57, 58, 59, 60, 61, 57, 62, 63, 60, 69, 55, 56, 57, 58, 59, 60, 61,
     55, 62, 63, 57, 64, 68, 69, 55, 56, 57, 58, 59, 55, 60, 61, 62, 63, 65, 68, 62, 56},
    {55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 67, 68, 69, 55, 57, 58, 65, 59, 60, 56, 61,
     62, 70, 63, 65, 68, 69, 56, 57, 58, 59, 60, 61, 55, 66, 62, 63, 65, 68, 55, 56, 59},
};

}  // namespace

// --- The manager's leaves ---

uint32_t EntryOfRecord(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821705A8 RaceManager::EntryOfRecord");
  const uint32_t manager = Manager(base);
  if (manager == 0) {
    return 0;
  }
  const int32_t count = int32_t(guest::LoadU32(base, manager + 2368));
  uint32_t entry = manager + 4;
  for (int32_t i = 0; i < count; i++, entry += 28) {
    if (guest::LoadU32(base, entry + 24) == record) {
      return entry;
    }
  }
  return 0;
}

uint32_t BranchesMeet(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82170860 RaceManager::BranchesMeet");
  a &= 0xFF;
  b &= 0xFF;
  if (b == 0 || b == 29 || a == b) {
    return 1;
  }
  if (a > 29) {
    return 0;
  }
  const auto eleven_twelve = [&] { return b - 11 <= 1 ? 1u : 0u; };
  switch (a) {
    case 1:  // sub_821708B4
      if (b < 3) {
        return 0;
      }
      return b <= 5 ? 1 : eleven_twelve();
    case 2:
      if (b < 3) {
        return 0;
      }
      if (b <= 4 || b == 6) {
        return 1;
      }
      return eleven_twelve();
    case 3:
      if (b <= 2 || b == 6 || b == 8 || b == 13) {
        return 1;
      }
      return 0;
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    case 12:
    case 13:
    case 14:
    case 29:
      return 1;
    default:  // 9, 10, 15 to 28
      return 0;
  }
}

uint64_t PickModel(uint8_t* base, uint32_t number, uint32_t branch, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82171070 Traffic::PickModel");
  for (uint32_t t = 0; t < 2; t++) {
    const uint32_t table = sp - (t == 0 ? 400 : 232);
    for (uint32_t i = 0; i < 42; i++) {
      guest::StoreU32(base, table + 4 * i, kModels[t][i]);
    }
  }
  const int32_t b = int32_t(branch);
  const uint32_t table = (b == 7 || b == 8 || b == 13 || b == 14) ? sp - 232 : sp - 400;
  const int32_t n = int32_t(number);
  const uint32_t index = uint32_t(n - (n / 42) * 42);
  return uint64_t(guest::LoadU32(base, table + (index << 2))) - 55;
}

// --- The sorted lists ---

void SortedListInsert(uint8_t* base, uint32_t list, uint32_t item, double key) {
  ITC_NATIVE_RUNNING("sub_82170AA0 SortedList::Insert");
  const uint32_t count = guest::LoadU32(base, list + 1608);
  const uint32_t first = guest::LoadU32(base, list + 1600);
  const uint32_t node = (count << 4) + list;
  guest::StoreU32(base, list + 1608, count + 1);
  guest::StoreFloat(base, node + 4, float(key));
  guest::StoreU32(base, node + 0, item);
  auto key_of = [&](uint32_t n) { return Lfs(base, n + 4); };
  if (first == 0) {
    guest::StoreU32(base, node + 8, 0);
    guest::StoreU32(base, node + 12, 0);
    guest::StoreU32(base, list + 1600, node);
    guest::StoreU32(base, list + 1604, node);
    return;
  }
  uint32_t at = guest::LoadU32(base, list + 1604);
  if (!(key_of(at) > key)) {
    // Forward along +12 to the first node with a greater key.
    for (;;) {
      const uint32_t next = guest::LoadU32(base, at + 12);
      if (next == 0) {
        guest::StoreU32(base, at + 12, node);
        guest::StoreU32(base, node + 8, at);
        guest::StoreU32(base, node + 12, 0);
        guest::StoreU32(base, list + 1604, node);
        return;
      }
      at = next;
      if (key_of(at) > key_of(node)) {
        break;
      }
    }
    // Before `at`.
    const uint32_t previous = guest::LoadU32(base, at + 8);
    if (previous != 0) {
      guest::StoreU32(base, previous + 12, node);
    } else {
      guest::StoreU32(base, list + 1600, node);
    }
    const uint32_t before = guest::LoadU32(base, at + 8);
    guest::StoreU32(base, node + 12, at);
    guest::StoreU32(base, node + 8, before);
    guest::StoreU32(base, at + 8, node);
    guest::StoreU32(base, list + 1604, node);
    return;
  }
  // Back along +8 to the first node with a smaller key.
  for (;;) {
    const uint32_t previous = guest::LoadU32(base, at + 8);
    if (previous == 0) {
      guest::StoreU32(base, at + 8, node);
      guest::StoreU32(base, node + 8, 0);
      guest::StoreU32(base, node + 12, at);
      guest::StoreU32(base, list + 1600, node);
      guest::StoreU32(base, list + 1604, node);
      return;
    }
    at = previous;
    if (key_of(at) < key_of(node)) {
      break;
    }
  }
  // After `at`.
  const uint32_t next = guest::LoadU32(base, at + 12);
  uint32_t after = 0;
  if (next != 0) {
    guest::StoreU32(base, next + 8, node);
    after = guest::LoadU32(base, at + 12);
  }
  guest::StoreU32(base, node + 12, after);
  guest::StoreU32(base, at + 12, node);
  guest::StoreU32(base, node + 8, at);
  guest::StoreU32(base, list + 1604, node);
}

void SortedListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82170A88 SortedList::Clear");
  guest::StoreU32(base, list + 1600, 0);
  guest::StoreU32(base, list + 1604, 0);
  guest::StoreU32(base, list + 1608, 0);
}

// --- The race world's handlers ---

void OnPausedTick() {
  ITC_NATIVE_RUNNING("sub_82170560 RaceWorld::OnPausedTick");
}

// --- The racers' vehicles ---

uint32_t Release(uint8_t* base, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_82171360 RaceWorld::Release");
  uint32_t manager = Manager(base);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, manager + 2364)); i++) {
    const uint32_t racer = guest::LoadU32(base, manager + 2308 + 4 * uint32_t(i));
    r3 = va::VehicleGet(base, guest::LoadU32(base, racer + 20));
    guest::StoreU8(base, r3 + 2384, 0);
    manager = Manager(base);
  }
  return r3;
}

// --- The route positions ---

uint32_t RoutePositionToMainRoad(uint8_t* base, uint32_t position, uint32_t flag, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82143608 RoutePosition::ToMainRoad");
  return track_route::ToMainRoad(base, kRoute, position, flag, sp - 144);
}

uint32_t StepToMainRoad(uint8_t* base, uint32_t position, uint32_t step, uint32_t kind,
                        uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170F18 RoutePosition::StepToMainRoad");
  guest::StoreU32(base, frame + 80, uint32_t(int32_t(int16_t(kind))));
  const uint32_t masks = ai_drivers::BranchMasksFor(base, frame + 80);
  guest::StoreU16(base, position + 2, uint16_t(guest::LoadU16(base, position + 2) + step));
  RoutePositionToMainRoad(base, position, 1, frame);
  return track_route::Normalize(base, kRoute, position, masks);
}

// --- The race event's start placements ---

uint32_t RaceEventEntry(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_82143508 RaceEvent::Entry");
  const uint32_t list = guest::LoadU32(base, guest::LoadU32(base, kRaceEvent) + 156);
  return guest::LoadU32(base, index) * 12 + list;
}

uint32_t PlaceEntry(uint8_t* base, uint32_t event, uint32_t route, uint32_t index,
                    uint32_t position, uint32_t step, uint32_t masks) {
  ITC_NATIVE_RUNNING("sub_82143150 RaceEvent::PlaceEntry");
  const int32_t i = int16_t(guest::LoadU16(base, index));
  const uint32_t entry = uint32_t(i) * 12 + guest::LoadU32(base, event + 156);
  const uint32_t r3 = track_route::Normalize(base, route, position, masks);
  guest::StoreU16(base, entry + 4, guest::LoadU16(base, position + 0));
  guest::StoreU16(base, entry + 6, guest::LoadU16(base, position + 2));
  guest::StoreU16(base, position + 2,
                  uint16_t(guest::LoadU16(base, step) + guest::LoadU16(base, position + 2)));
  return r3;
}

// --- The save data ---

uint32_t FirstUnsetFlag(uint8_t* base, uint32_t save) {
  ITC_NATIVE_RUNNING("sub_82112190 SaveData::FirstUnsetFlag");
  for (uint32_t i = 0; i < 6; i++) {
    if ((guest::LoadU32(base, save + 6092) & (1u << i)) == 0) {
      return i;
    }
  }
  return 6;
}

}  // namespace race_world
