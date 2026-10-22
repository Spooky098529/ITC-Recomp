// The collision world's owner (docs/functions/collision-world.md): see
// track_collision.h.

#include "track_collision/track_collision.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "base/log.h"

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
