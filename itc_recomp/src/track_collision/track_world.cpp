// The collision world's owner (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "base/log.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace track_collision {

using namespace car_physics::ops;
namespace cp = car_physics;

uint32_t TimesThreePlus(uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_8228F090 TimesThreePlus");
  return a * 3 + b;
}

uint32_t RaceRacerCount(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821705F8 Race::RacerCount");
  return guest::LoadU32(base, guest::LoadU32(base, 0x826D82F4) + 2364);
}

uint32_t RacersCarsCollide(uint8_t* base, uint32_t records) {
  ITC_NATIVE_RUNNING("sub_820FEFF8 Racers::CarsCollide");
  const int32_t count = int32_t(guest::LoadU32(base, records + 0x595E0));
  if (count == 1) {
    return 0;
  }
  for (int32_t i = 0; i < count; i++) {
    if ((guest::LoadU32(base, records + 3888 + 3936 * uint32_t(i)) & 0x8) != 0) {
      return 0;
    }
  }
  return 1;
}

void TrackCollisionTick(uint8_t* base, uint32_t frame, const cp::CollisionGuestCall& guest) {
  ITC_NATIVE_RUNNING("sub_820FF050 TrackCollision::Tick");
  namespace va = vehicle_accessors;
  const uint32_t records = guest::LoadU32(base, 0x826C5B2C) + 48;
  double dt = Lfs(base, 0x8260327C);
  if (guest::LoadU32(base, kMesh + 8) != 2 || guest::LoadU32(base, kRoute + 8) != 2) {
    return;
  }
  const uint32_t collide = RacersCarsCollide(base, records);
  if (collide == 0) {
    dt = Lfs(base, kZero);
  }
  cp::CollisionWorldUpdate(base, dt, frame - 176, collide, guest);
  for (int32_t i = 0; i < int32_t(RaceRacerCount(base)); i++) {
    const uint32_t racer =
        guest::LoadU32(base, guest::LoadU32(base, 0x826D82F4) + 2308 + 4 * uint32_t(i));
    const uint32_t vehicle = va::VehicleGet(base, guest::LoadU32(base, racer + 20));
    uint32_t contact = guest::LoadU32(base, guest::LoadU32(base, vehicle + 340) + 240);
    guest::StoreU32(base, vehicle + 6152, 0);
    while (contact != 0) {
      if (va::ContactObject(base, contact) != 0) {
        guest::StoreU32(base, vehicle + 6152, guest::LoadU32(base, vehicle + 6152) | 1);
      } else {
        va::ContactOtherObject(base, contact);
        guest::StoreU32(base, vehicle + 6152, guest::LoadU32(base, vehicle + 6152) | 2);
        Stfs(base, vehicle + 6236, double(va::ContactStrength(base, contact)));
      }
      contact = va::ContactNext(base, contact, 0xFFFFFFFF);
    }
    const uint32_t off_ground = guest::LoadU32(base, vehicle + 6240);
    if (guest::LoadU32(base, vehicle + 348) == 0) {
      guest::StoreU32(base, vehicle + 6240, off_ground + 1);
      continue;
    }
    if (int32_t(off_ground) >= 10) {
      guest::StoreU32(base, vehicle + 6152, guest::LoadU32(base, vehicle + 6152) | 4);
      guest::StoreU32(base, vehicle + 6232, guest::LoadU32(base, vehicle + 6240));
    }
    guest::StoreU32(base, vehicle + 6240, 0);
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
  cp::CollisionSurfaceSetA(base, kBit24, Lfs(base, 0x82001BE4));
  cp::CollisionSurfaceSetA(base, kBit26, Lfs(base, 0x82002968));
  cp::CollisionSurfaceSetB(base, kBit25, zero);
  cp::CollisionSurfaceSetB(base, kBit24, zero);
  cp::CollisionSurfaceSetB(base, kBit26, zero);
  return r3;
}

}  // namespace track_collision
