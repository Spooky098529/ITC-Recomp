// The racers' manager's streaming each tick (docs/functions/race-world.md):
// RaceManager::StreamOpponents and RaceManager::MoveUnseen. See
// race_world.h.

#include "race_world/race_world.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "car_physics/float_ops.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "racer_controllers/racer_controllers.h"
#include "track_route/track_route.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstdint>

namespace race_world {

using namespace car_physics::ops;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

constexpr uint32_t kInputs = 0x826C5B30;      // the racers' input records
constexpr uint32_t kFarKey = 0x8200222C;      // the key of entries flagged 0x10 or 0x400
constexpr uint32_t kTicksScale = 0x820099D4;  // a slice's ticks, by default
constexpr uint32_t kAhead = 0x820021E8;       // Neighbours 1: one ahead
constexpr uint32_t kBehind = 0x82001070;      // Neighbours 2: one behind
constexpr uint32_t kBoth = 0x820029F0;        // Neighbours 3: both
constexpr uint32_t kGiveVehicle = 0x821706E8;
constexpr uint32_t kRemoveFromWorld = 0x820C20E0;

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
// A section's record in the route's table.
uint32_t SectionRecord(uint8_t* base, uint32_t file, uint32_t section) {
  return Load(base, Load(base, file + 12) + (section << 2));
}

// An entry loses its vehicle (when it has one and its slot word is above
// 0): flags &= ~0x40, Vehicle::RemoveFromWorld (its original without
// `heap` or where its plain C++ refuses, with r1 = `frame`), the slot word
// 0 and +20 = -1.
void Lose(uint8_t* base, uint32_t manager, uint32_t entry, uint32_t frame, bool heap,
          const OriginalCall& original) {
  const uint32_t which = Load(base, entry + 20);
  if (int32_t(which) == -1 || int32_t(Load(base, manager + ((which + 584) << 2))) <= 0) {
    return;
  }
  Store(base, entry, Load(base, entry) & ~0x40u);
  const uint32_t car = va::VehicleGet(base, which);
  uint32_t removed = 0;
  if (!heap || !vehicle::VehicleRemoveFromWorld(base, car, &removed)) {
    original(kRemoveFromWorld, frame, car, 0, 0);
  }
  Store(base, manager + ((Load(base, entry + 20) + 584) << 2), 0);
  Store(base, entry + 20, 0xFFFFFFFFu);
}

// An entry with a vehicle put in the racers' list (+2308): +2364 and +2372
// count.
void List(uint8_t* base, uint32_t manager, uint32_t entry) {
  Store(base, manager + ((Load(base, manager + 2364) + 577) << 2), entry);
  const uint32_t listed = Load(base, manager + 2364);
  const uint32_t racers = Load(base, manager + 2372);
  Store(base, manager + 2364, listed + 1);
  Store(base, manager + 2372, racers + 1);
}

}  // namespace

void StreamOpponents(uint8_t* base, uint32_t manager, uint32_t player, double spawn,
                     double despawn, uint32_t frame, bool heap, const OriginalCall& original) {
  ITC_NATIVE_RUNNING("sub_82172118 RaceManager::StreamOpponents");
  // The player's route position (its vehicle's identity +4) at + 80, its
  // section's record.
  const uint32_t identity = va::VehicleIdentity(va::VehicleGet(base, Load(base, player + 20)));
  const uint32_t position = Load(base, identity + 4);
  const int32_t count = int32_t(Load(base, manager + 2368));
  const uint32_t file = Load(base, kRoute);
  Store(base, frame + 80, position);
  const uint32_t player_section = guest::LoadU16(base, frame + 80);
  const uint32_t player_record = SectionRecord(base, file, player_section);
  // The sorted list at + 176 emptied (+1776 to +1784).
  Store(base, manager + 2372, 0);
  Store(base, frame + 1780, 0);
  Store(base, frame + 1784, 0);
  Store(base, frame + 1776, 0);
  const uint32_t list = frame + 176;
  if (count > 0) {
    const double one = Lfs(base, kOne);
    const double far_key = Lfs(base, kFarKey);
    const double zero = Lfs(base, kZero);
    for (int32_t i = 0;;) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      const uint32_t flags = Load(base, entry);
      bool lose = (flags & 0x20) != 0 || (flags & 0x80) != 0;
      double key = 0.0;
      if (lose) {
      } else if ((flags & 0x4) != 0 || (flags & 0x8) != 0) {
        key = zero;
      } else if ((flags & 0x10) != 0 || (flags & 0x400) != 0) {
        key = far_key;
      } else {
        const uint32_t word = Load(base, entry + 8);
        const uint32_t now = Load(base, kRoute);
        const uint32_t a = guest::LoadU8(base, player_record + 3);
        Store(base, frame + 84, word);
        const uint32_t section = guest::LoadU16(base, frame + 84);
        const uint32_t b = guest::LoadU8(base, SectionRecord(base, now, section) + 3);
        if (BranchesMeet(a, b) == 0) {
          lose = true;
        } else {
          // Both slices' centers: the entry's at + 128, the player's at +
          // 112; the difference at + 96, its y = 0.0 and w = 1.0; its
          // length + 1.0.
          track_route::SlicePoint(base, frame + 128, 0, S16(section), LoadS16(base, frame + 86));
          track_route::SlicePoint(base, frame + 112, 0, S16(player_section),
                                  LoadS16(base, frame + 82));
          vl::Vec3Sub(base, frame + 96, frame + 128, frame + 112);
          Stfs(base, frame + 100, zero);
          Stfs(base, frame + 108, one);
          key = Fadds(double(vl::Vec3Normalize(base, frame + 96, frame + 96)), one);
        }
      }
          if (lose) {
        Lose(base, manager, entry, frame, heap, original);
      } else {
        SortedListInsert(base, list, entry, key);
      }
      i++;
      if (!(i < int32_t(Load(base, manager + 2368)))) {
        break;
      }
    }
  }
  // Past the fourth, the vehicle removed.
  uint32_t n = 0;
  for (uint32_t node = Load(base, frame + 1776); node != 0; node = Load(base, node + 12), n++) {
    const uint32_t entry = Load(base, node);
    if (n >= 4) {
      Lose(base, manager, entry, frame, heap, original);
    }
  }
  // Up to four: nearer than `spawn` given a vehicle (or keeping theirs),
  // beyond `despawn` losing theirs, between keeping theirs.
  for (uint32_t node = Load(base, frame + 1776); node != 0; node = Load(base, node + 12)) {
    const uint32_t entry = Load(base, node);
    if (Load(base, manager + 2372) >= 4) {
      Lose(base, manager, entry, frame, heap, original);
      continue;
    }
    const double key = Lfs(base, node + 4);
    if (key < spawn) {
      if (int32_t(Load(base, entry + 20)) != -1) {
        List(base, manager, entry);
        continue;
      }
      // RaceManager::GiveVehicle (never seen in play): its original.
      const uint64_t given = original(kGiveVehicle, frame, manager, entry, 0);
      if (uint32_t(given) == 0) {
        // No free slot: the empty sub_82106540 with (500, 220, 0,
        // 0x820099BC), probably a debug print compiled out.
        continue;
      }
      List(base, manager, entry);
      ai_drivers::ClearCarInput(base, frame + 144);
      const uint32_t to = Load(base, kInputs + ((Load(base, entry + 20) << 2) & 0x3FC));
      for (uint32_t w = 0; w < 5; w++) {
        Store(base, to + 4 * w, Load(base, frame + 144 + 4 * w));
      }
      const uint32_t number = Load(base, entry + 24);
      const uint32_t id = number * 3936 + Load(base, kRaceData) + 3960;
      Store(base, frame + 80, 1);
      racer_controllers::Setup(base, entry + 20, id, frame + 80, frame);
    } else if (key > despawn) {
      Lose(base, manager, entry, frame, heap, original);
    } else if (int32_t(Load(base, entry + 20)) != -1) {
      List(base, manager, entry);
    }
  }
}

void MoveUnseen(uint8_t* base, uint32_t manager, uint32_t player, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82173E28 RaceManager::MoveUnseen");
  const uint32_t vehicle = va::VehicleGet(base, Load(base, player + 20));
  const uint32_t identity = va::VehicleIdentity(vehicle);
  const uint32_t body = Load(base, vehicle + 336);
  Store(base, frame + 88, Load(base, identity + 4));
  const uint32_t at = va::RigidBodyPosition(body);
  const uint32_t racers = Load(base, manager + 2372);
  const uint32_t player_section = guest::LoadU16(base, frame + 88);
  const uint32_t race_flags = Load(base, Load(base, kRaceData) + 20);
  const uint32_t file = Load(base, kRoute);
  const uint32_t table = Load(base, file + 12);
  const double slice_length = Lfs(base, file + 8);
  uint32_t flag20 = (race_flags >> 5) & 1;
  const uint32_t player_record = Load(base, (player_section << 2) + table);
  const bool four = racers == 4;
  uint32_t hi[2] = {Load(base, at + 8), Load(base, at + 12)};
  Store(base, frame + 80, flag20);
  Store(base, frame + 92, guest::LoadU8(base, player_record + 3));
  uint32_t lo[2] = {Load(base, at + 0), Load(base, at + 4)};
  uint32_t flag10 = (race_flags >> 4) & 1;
  // The player's position (RigidBody::Position) at + 96.
  Store(base, frame + 96, lo[0]);
  Store(base, frame + 100, lo[1]);
  Store(base, frame + 104, hi[0]);
  Store(base, frame + 108, hi[1]);
  Store(base, frame + 84, flag10);
  // With the race data's flags 0x10 or 0x20: whether an entry of item kind
  // 2 (sections 200 to 203, 258 to 261; clears + 84) or 3 (187, 188, 193,
  // 194, 251 to 253; clears + 80) is already there.
  if ((flag10 != 0 || flag20 != 0) && int32_t(Load(base, manager + 2368)) > 0) {
    for (int32_t i = 0;;) {
      const uint32_t entry = manager + 4 + 28 * uint32_t(i);
      const uint32_t number = Load(base, entry + 24);
      const uint32_t race = Load(base, kRaceData);
      const int32_t id = int32_t(S16(Load(base, number * 3936 + race + 3960)));
      if (id >= 0 && (Load(base, entry) & 0xAC) == 0) {
        const int32_t s = int32_t(LoadS16(base, entry + 8));
        uint32_t which = 0;
        if (s > 203) {
          if (s >= 251) {
            if (s <= 253) {
              which = 3;
            } else if (uint32_t(s - 258) <= 3) {
              which = 2;
            }
          }
        } else if (s >= 200) {
          which = 2;
        } else if (s >= 187) {
          if (s <= 188 || (s > 192 && s <= 194)) {
            which = 3;
          }
        }
        if (which == 3 && flag20 != 0 &&
            guest::LoadU8(base, data_tables::Item(base, uint32_t(id)) + 36) == 3) {
          flag20 = 0;
          Store(base, frame + 80, flag20);
        } else if (which == 2 && flag10 != 0 &&
                   guest::LoadU8(base, data_tables::Item(base, uint32_t(id)) + 36) == 2) {
          flag10 = 0;
          Store(base, frame + 84, flag10);
        }
      }
      if (flag10 == 0 && flag20 == 0) {
        break;
      }
      i++;
      if (!(i < int32_t(Load(base, manager + 2368)))) {
        break;
      }
    }
  }
  // Each entry not flagged 0x20 or 0x80 (its index at + 88).
  Store(base, frame + 88, 0);
  if (!(int32_t(Load(base, manager + 2368)) > 0)) {
    return;
  }
  const double ahead = Lfs(base, kAhead);
  const double behind = Lfs(base, kBehind);
  const double both = Lfs(base, kBoth);
  const double scale = Lfs(base, kTicksScale);
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0;;) {
    const uint32_t entry = manager + 4 + 28 * i;
    const uint32_t flags = Load(base, entry);
    if ((flags & 0x20) != 0 || (flags & 0x80) != 0) {
    } else if ((flags & 0x40) != 0) {
      // With a vehicle: +8 its track position, +12 0.0, its slot word
      // counting its ticks (to 216,000, then 60).
      const uint32_t own = va::VehicleIdentity(va::VehicleGet(base, Load(base, entry + 20)));
      const uint32_t slot = manager + ((Load(base, entry + 20) + 584) << 2);
      guest::StoreU16(base, entry + 8, guest::LoadU16(base, own + 4));
      guest::StoreU16(base, entry + 10, guest::LoadU16(base, own + 6));
      Stfs(base, entry + 12, zero);
      Store(base, slot, Load(base, slot) + 1);
      const uint32_t again = manager + ((Load(base, entry + 20) + 584) << 2);
      if (int32_t(Load(base, again)) > 216000) {
        Store(base, again, 60);
      }
    } else {
      // Without: +12 counts ticks; once it reaches a slice's ticks, a slice
      // on along the route.
      const uint32_t number = Load(base, entry + 24);
      const double ticks_so_far = Lfs(base, entry + 12);
      const uint32_t race = Load(base, kRaceData);
      Stfs(base, entry + 12, Fadds(ticks_so_far, one));
      const uint32_t record = number * 3936 + race;
      JumpSections(base, entry, frame + 96, frame + 92, frame + 84, frame + 80, frame - 176);
      double ticks = 0.0;
      bool met = false;
      if (four && (Load(base, player) & 0x10) != 0) {
        const uint32_t now = Load(base, kRoute);
        const uint32_t section = guest::LoadU16(base, entry + 8);
        const uint32_t a = guest::LoadU8(base, player_record + 3);
        const uint32_t b = guest::LoadU8(base, SectionRecord(base, now, section) + 3);
        if (BranchesMeet(a, b) != 0) {
          met = true;
          if (int32_t(Load(base, vehicle + 6168)) != 0 ||
              double(va::VehicleDistancePerFrame(base, vehicle)) == zero) {
            ticks = Fmuls(slice_length, scale);
          } else {
            ticks = Fdivs(slice_length, double(va::VehicleDistancePerFrame(base, vehicle)));
          }
        }
      }
      if (!met) {
        ticks = Fmuls(slice_length, scale);
        switch (int32_t(Neighbours(base, manager, frame + 88))) {
          case 1:
            ticks = Fmuls(ticks, ahead);
            break;
          case 2:
            ticks = Fmuls(ticks, behind);
            break;
          case 3:
            ticks = Fmuls(ticks, both);
            break;
          default:
            break;
        }
      }
      if (!(Lfs(base, entry + 12) < ticks)) {
        const uint32_t item = Load(base, record + 3960);
        const uint32_t kind =
            int32_t(item) < 0
                ? 0xFFFFFFFFu
                : uint32_t(int32_t(int8_t(guest::LoadU8(base, data_tables::Item(base, item) + 36))));
        vehicle::RoutePositionStep(base, entry + 8, 1, S16(kind), frame - 112);
        Stfs(base, entry + 12, Fsubs(Lfs(base, entry + 12), ticks));
      }
    }
    i++;
    const int32_t count = int32_t(Load(base, manager + 2368));
    Store(base, frame + 88, i);
    if (!(int32_t(i) < count)) {
      break;
    }
  }
}

}  // namespace race_world
