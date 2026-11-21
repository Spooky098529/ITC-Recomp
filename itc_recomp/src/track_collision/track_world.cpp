// The collision world's owner (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "base/log.h"
#include "schema/car_physics.h"
#include "schema/collision_world.h"
#include "schema/minimap.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/track_route.h"
#include "schema/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace track_collision {

using namespace car_physics::ops;
namespace cp = car_physics;
using G = schema::CollisionWorldGlobals;

uint32_t TimesThreePlus(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_8228F090 TimesThreePlus");
  return a * 3 + b;
}

uint32_t RaceRacerCount(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821705F8 Race::RacerCount");
  return G::racers_manager(base).racer_count();
}

uint32_t RacersCarsCollide(uint8_t* base, uint32_t records) {
  ITC_NATIVE_RUNNING("sub_820FEFF8 Racers::CarsCollide");
  // r3 is the racers object at the race data's +48 (the measure's fault:
  // its car count is RaceData's +0x59610).
  const int32_t count = int32_t(schema::RaceData(base, records).fault_car_count());
  if (count == 1) {
    return 0;
  }
  for (int32_t i = 0; i < count; i++) {
    if ((schema::Car(base, records + 3936 * uint32_t(i)).flags() & 0x8) != 0) {
      return 0;
    }
  }
  return 1;
}

void TrackCollisionTick(uint8_t* base, uint32_t frame, const cp::CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820FF050 TrackCollision::Tick");
  namespace va = vehicle_accessors;
  const uint32_t records = G::race_data(base).racer_records_address();
  double dt = double(G::tick_step(base));
  if (schema::TrackMesh(base, kMesh).state() != 2 || schema::TrackRoute(base, kRoute).state() != 2) {
    return;
  }
  const uint32_t collide = RacersCarsCollide(base, records);
  if (collide == 0) {
    dt = Lfs(base, kZero);
  }
  cp::CollisionWorldUpdate(base, dt, frame - 176, collide, guest);
  for (int32_t i = 0; i < int32_t(RaceRacerCount(base)); i++) {
    const schema::MinimapRacerEntry racer(
        base, guest::LoadU32(base, G::racers_manager(base).racers_address() + 4 * uint32_t(i)));
    const schema::Vehicle vehicle(base, va::VehicleGet(base, racer.vehicle_index()));
    const schema::VehicleStatus status(base, vehicle.status_address());
    uint32_t contact =
        schema::CollisionWorldObject(base, vehicle.collision_object()).first_contact().address;
    vehicle.set_collision_flags(0);
    while (contact != 0) {
      if (va::ContactObject(base, contact) != 0) {
        vehicle.set_collision_flags(vehicle.collision_flags() | 1);
      } else {
        va::ContactOtherObject(base, contact);
        vehicle.set_collision_flags(vehicle.collision_flags() | 2);
        Stfs(base, status.unknown_76_address(), double(va::ContactStrength(base, contact)));
      }
      contact = va::ContactNext(base, contact, 0xFFFFFFFF);
    }
    const uint32_t off_ground = status.unknown_80();
    if (vehicle.wheels_on_ground() == 0) {
      status.set_unknown_80(off_ground + 1);
      continue;
    }
    if (int32_t(off_ground) >= 10) {
      vehicle.set_collision_flags(vehicle.collision_flags() | 4);
      status.set_landing_ticks(status.unknown_80());
    }
    status.set_unknown_80(0);
  }
}

uint32_t TrackCollisionResetWorld(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820FEAF8 TrackCollision::ResetWorld");
  const uint32_t r3 = cp::CollisionWorldReset(base);
  constexpr uint32_t kBit25 = 0x02000000, kBit24 = 0x01000000, kBit26 = 0x04000000;
  const double zero = Lfs(base, kZero);
  cp::CollisionSurfaceSetC(base, kBit25, zero);
  cp::CollisionSurfaceSetC(base, kBit24, zero);
  cp::CollisionSurfaceSetC(base, kBit26, zero);
  cp::CollisionSurfaceSetD(base, kBit25, zero);
  const double one = Lfs(base, kOne);
  cp::CollisionSurfaceSetD(base, kBit24, one);
  cp::CollisionSurfaceSetD(base, kBit26, one);
  cp::CollisionSurfaceSetA(base, kBit25, zero);
  cp::CollisionSurfaceSetA(base, kBit24, double(G::half(base)));
  cp::CollisionSurfaceSetA(base, kBit26, double(G::quarter(base)));
  cp::CollisionSurfaceSetB(base, kBit25, zero);
  cp::CollisionSurfaceSetB(base, kBit24, zero);
  cp::CollisionSurfaceSetB(base, kBit26, zero);
  return r3;
}

}  // namespace track_collision
