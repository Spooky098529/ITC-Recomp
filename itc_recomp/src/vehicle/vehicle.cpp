// The vehicle code above the car physics (docs/functions/vehicle.md): the
// leaves, the route position's step, the track positions, the vehicles'
// lifetime. See vehicle.h.

#include "vehicle/vehicle.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "c_runtime/arrays.h"
#include "collision/collision.h"
#include "minimap/minimap.h"
#include "memory/c_runtime_heap.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace vehicle {

using namespace car_physics::ops;
namespace cp = car_physics;
namespace co = collision;
namespace tc = track_collision;
namespace va = vehicle_accessors;
namespace vl = vector_library;

namespace {

constexpr uint32_t kRoute = 0x826C5B10;            // the game's route
constexpr uint32_t kCameraCurves = 0x82600EE8;     // 90 floats a curve, .data
constexpr uint32_t kEmptyDestructor = 0x82106540;  // sub_82106540

// Whether Vehicle::DestroyPhysics would run natively for the vehicle (its
// own checks: the pairs' removal, the shape a box the layer gave, the body
// a block the layer gave).
bool DestroyPhysicsFits(uint8_t* base, uint32_t vehicle) {
  const uint32_t object = guest::LoadU32(base, vehicle + 340);
  const uint32_t shape = guest::LoadU32(base, vehicle + 344);
  const uint32_t body = guest::LoadU32(base, vehicle + 336);
  return !((object != 0 && !cp::CollisionWorldPairsMovable(base)) ||
           (shape != 0 && (!co::IsBox(base, shape) || !co::Deletable(shape))) ||
           !co::Deletable(body));
}

}  // namespace

// --- Leaves ---

void MatrixFromRows(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t c,
                    uint32_t d) {
  ITC_NATIVE_RUNNING("sub_820BFBB8 Matrix::FromRows");
  const uint32_t rows[4] = {a, b, c, d};
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, out + 16 * row + i, guest::LoadU32(base, rows[row] + i));
    }
  }
}

void TrackPositionCopy(uint8_t* base, uint32_t out, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_820C2150 TrackPosition::Copy");
  for (uint32_t i = 0; i < 8; i += 2) {
    guest::StoreU16(base, out + i, guest::LoadU16(base, from + i));
  }
  for (uint32_t i = 16; i < 80; i += 8) {
    const uint64_t word = (uint64_t(guest::LoadU32(base, from + i)) << 32) |
                          guest::LoadU32(base, from + i + 4);
    guest::StoreU32(base, out + i, uint32_t(word >> 32));
    guest::StoreU32(base, out + i + 4, uint32_t(word));
  }
  guest::StoreU16(base, out + 80, guest::LoadU16(base, from + 80));
  guest::StoreU16(base, out + 82, guest::LoadU16(base, from + 82));
}

void CameraCurveLoad(uint8_t* base, uint32_t object, uint32_t index, uint32_t word) {
  ITC_NATIVE_RUNNING("sub_820BFCE8 CameraCurve::Load");
  const uint32_t table = kCameraCurves + uint32_t(uint64_t(index) * 360);
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < 90; i++) {
    Stfs(base, object + 4 * i + 360, zero);
    // lfs then stfs with only an addi between: the word's bits.
    CopyWord(base, table + 4 * i, object + 4 * i);
  }
  guest::StoreU32(base, object + 720, word);
}

// --- The route position ---

uint32_t RoutePositionStep(uint8_t* base, uint32_t position, uint32_t step, uint32_t kind,
                           uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82170F90 RoutePosition::Step");
  guest::StoreU32(base, frame + 80, uint32_t(int32_t(int16_t(kind))));
  const uint32_t masks = ai_drivers::BranchMasksFor(base, frame + 80);
  guest::StoreU16(base, position + 2, uint16_t(guest::LoadU16(base, position + 2) + step));
  return track_route::Normalize(base, kRoute, position, masks);
}

// --- The vehicle's track positions and world ---

void VehicleUpdateTrackPositions(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C1D78 Vehicle::UpdateTrackPositions");
  const uint32_t car = vehicle + 5232;
  const uint32_t position = va::RigidBodyPosition(guest::LoadU32(base, vehicle + 336));
  CopyWord(base, position + 0, car + 16);
  CopyWord(base, position + 4, car + 20);
  CopyWord(base, position + 8, car + 24);
  tc::TrackPositionUpdate(base, car, frame - 144);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t mount = vehicle + 480 + 368 * i;
    const uint32_t wheel = vehicle + 5328 + 96 * i;
    CopyWord(base, mount + 0, wheel + 16);
    CopyWord(base, mount + 4, wheel + 20);
    CopyWord(base, mount + 8, wheel + 24);
    tc::TrackPositionUpdate(base, wheel, frame - 144);
  }
}

bool VehicleLinkTrackPositions(uint8_t* base, uint32_t vehicle, uint32_t* r3) {
  uint32_t shape = 0;
  // Current asks the template's type (nothing written): first, then the
  // copy's slot 20 must be the box's.
  if (!co::CollisionShapeCurrent(base, guest::LoadU32(base, vehicle + 340) + 32, &shape) ||
      !co::IsBox(base, shape)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C1D08 Vehicle::LinkTrackPositions");
  guest::StoreU32(base, vehicle + 2984, vehicle + 5232);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t position = vehicle + 5328 + 96 * i;
    guest::StoreU32(base, vehicle + 436 + 368 * i, position);
    co::BoxSetCacheWord(base, shape, i, position);
  }
  *r3 = shape;
  return true;
}

bool VehicleRefresh(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1, uint32_t* r3) {
  if (!cp::CollisionWorldCallbackKnown(base)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C0118 Vehicle::Refresh");
  *f1 = Lfs(base, kStepSeconds);
  *r3 = cp::VehicleRefreshFromBody(base, vehicle, frame);
  return true;
}

bool VehicleRemoveFromWorld(uint8_t* base, uint32_t vehicle, uint32_t* r3) {
  const uint32_t object = guest::LoadU32(base, vehicle + 340);
  const uint32_t shape = guest::LoadU32(base, vehicle + 344);
  if ((object != 0 && !cp::CollisionWorldPairsMovable(base)) ||
      (shape != 0 && (!co::IsBox(base, shape) || !co::Deletable(shape)))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C20E0 Vehicle::RemoveFromWorld");
  guest::StoreU32(base, vehicle + 6148, 0);
  if (object != 0) {
    uint32_t ignored = 0;
    cp::CollisionWorldRemoveObject(base, object, &ignored);
    guest::StoreU32(base, vehicle + 340, 0);
  }
  // +344 reloaded after the removal, as the original.
  const uint32_t current = guest::LoadU32(base, vehicle + 344);
  if (current != 0) {
    co::ShapeDeletingDestructor(base, current, 1);  // slot 0 (a box's)
  }
  guest::StoreU32(base, vehicle + 344, 0);
  *r3 = current;
  return true;
}

// --- The seven vehicles ---

void VehicleConstructAll(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C30D8 Vehicle::Construct");
  double ignored = 0.0;
  cp::VehicleConstruct(base, vehicle, frame - 112, &ignored);
  cp::CarDataConstruct(base, vehicle + 3376, frame - 96);
  const double one = Lfs(base, kOne);
  Stfs(base, vehicle + 5260, one);
  Stfs(base, vehicle + 5276, one);
  Stfs(base, vehicle + 5292, one);
  Stfs(base, vehicle + 5308, one);
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t at = vehicle + 5328 + 44 + 96 * k;
    Stfs(base, at - 16, one);
    Stfs(base, at + 0, one);
    Stfs(base, at + 16, one);
    Stfs(base, at + 32, one);
  }
}

std::optional<uint32_t> VehiclesCreate(uint8_t* base, uint32_t frame) {
  const auto block = memory::crt::OperatorNew(base, kVehiclesBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820C3BD0 Vehicles::Create");
  if (*block != 0) {
    for (uint32_t i = 0; i < 7; i++) {
      VehicleConstructAll(base, *block + kVehicleBytes * i, frame - 96);
    }
  }
  guest::StoreU32(base, kVehicles, *block);
  guest::ForEachHostPart(base, *block, kVehiclesBytes,
                         [](uint8_t* host, uint32_t, uint32_t bytes) {
                           std::memset(host, 0, bytes);
                         });
  return *block;
}

bool VehiclesDestroy(uint8_t* base) {
  const uint32_t block = guest::LoadU32(base, kVehicles);
  if (block != 0) {
    if (!co::Deletable(block)) {
      return false;
    }
    for (uint32_t i = 0; i < 7; i++) {
      if (!DestroyPhysicsFits(base, block + kVehicleBytes * i)) {
        return false;
      }
    }
  }
  ITC_NATIVE_RUNNING("sub_820C3B78 Vehicles::Destroy");
  if (block != 0) {
    ITC_NATIVE_RUNNING("sub_820C21E0 Vehicles::DestroyAll");
    for (int32_t i = 6; i >= 0; i--) {
      const uint32_t vehicle = block + kVehicleBytes * uint32_t(i);
      // The empty destructor on each: the iterator calls nothing.
      c_runtime::VectorDestruct(
          base, vehicle + 5328, 96, 4, kEmptyDestructor,
          [](void*, uint64_t, uint64_t) {
            ITC_LOG_ERROR("vehicle: the track positions' destructor isn't the empty one");
          },
          nullptr);
      uint32_t ignored = 0;
      cp::VehicleDestroyPhysics(base, vehicle, &ignored);
    }
    memory::crt::OperatorDelete(block);
  }
  guest::StoreU32(base, kVehicles, 0);
  return true;
}

// --- The vehicle's input and queries ---

namespace {

constexpr uint32_t kTwentieth = 0x82002204;    // 0.05
constexpr uint32_t kInputScale = 0x82002BD4;   // 1/32,767
constexpr uint32_t kOneAndHalf = 0x820027A8;   // 1.5
constexpr uint32_t kHalf = 0x82002BC0;         // 0.5
constexpr uint32_t kFour = 0x820021F8;         // the handbrake ramp's rate
constexpr uint32_t kSlipFlag = 0x826C2E3C, kSlipThreshold = 0x826C2E38;
constexpr uint32_t kPerKmh = 0x82002BD8;       // 1/3.6

// std then lfd then fcfid then frsp: a 64-bit integer through the stack
// (its doubleword written) to a single.
double IntThroughStack(uint8_t* base, uint32_t at, uint64_t value) {
  guest::StoreU32(base, at, uint32_t(value >> 32));
  guest::StoreU32(base, at + 4, uint32_t(value));
  return double(float(double(int64_t(value))));
}
// extsb, addi 10, clrldi 32: a rate byte's word.
uint64_t RateOf(uint8_t byte) {
  return uint32_t(int32_t(int8_t(byte)) + 10);
}
// The brake's and throttle's curve: x = the input / 32,767, x^2 + (x - x^2
// + x - x^2) x (rate x 0.05) (as fmadds).
double PedalCurve(uint8_t* base, uint32_t input, uint32_t at, uint32_t byte, uint32_t frame) {
  const double x0 = IntThroughStack(base, frame + 88,
                                    uint64_t(int64_t(int16_t(guest::LoadU16(base, input + at)))));
  const double rate = IntThroughStack(base, frame + 80, RateOf(guest::LoadU8(base, input + byte)));
  const double x = Fmuls(x0, Lfs(base, kInputScale));
  const double k = Fmuls(rate, Lfs(base, kTwentieth));
  const double x2 = Fmuls(x, x);
  const double a = Fsubs(x, x2);
  const double b = Fsubs(Fadds(a, x), x2);
  return Fmadds(b, k, x2);
}

}  // namespace

bool VehicleApplyInput(uint8_t* base, uint32_t vehicle, uint32_t input, uint32_t frame,
                       const SteerOriginal& steer_original) {
  const uint32_t v = vehicle;
  const uint32_t kind = uint32_t(int32_t(int8_t(guest::LoadU8(base, input + 17))));
  const uint32_t flags = guest::LoadU32(base, input + 0);
  // The callees never seen in play: decided first (the setters before them
  // don't write the flags or +2249).
  const bool shifts = guest::LoadU8(base, v + 2249) != 0 ? (flags & 0x2000) != 0
                                                          : (flags & 0x3) != 0;
  if (kind == 1 || (flags & 0x20000000) != 0 || (flags & 0x08000000) != 0 || shifts) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820BFD30 Vehicle::ApplyInput");
  const double dt = Lfs(base, kStepSeconds);
  const double zero = Lfs(base, kZero);
  const double one = Lfs(base, kOne);
  const double twentieth = Lfs(base, kTwentieth);
  const double scale = Lfs(base, kInputScale);
  if (kind == 0) {
    // The pad: SteerTowards with a rate from byte +16.
    const double rate = IntThroughStack(base, frame + 88, RateOf(guest::LoadU8(base, input + 16)));
    const double f3 = Fmadds(Fnmsubs(rate, twentieth, one), Lfs(base, kOneAndHalf), Lfs(base, kHalf));
    const int32_t steer = int16_t(guest::LoadU16(base, input + 8));
    double target = zero;
    uint32_t left = 0, right = 0;
    if ((flags & 0x02000000) != 0) {
      if (steer == -32767) {
        left = 1;
      } else if (steer == 32767) {
        right = 1;
      }
    } else {
      target = Fmuls(IntThroughStack(base, frame + 88, uint64_t(int64_t(steer))), scale);
    }
    double f1 = 0.0;
    if (!cp::VehicleSteerTowards(base, v, target, dt, f3, left, right, &f1)) {
      steer_original(target, dt, f3, left, right);
    }
  } else if (kind == 2) {
    // An AI driver's.
    const int64_t steer = int16_t(guest::LoadU16(base, input + 8));
    cp::VehicleSetSteering(base, v, Fmuls(IntThroughStack(base, frame + 80, uint64_t(steer)), scale));
  }
  cp::VehicleSetBrake(base, v, PedalCurve(base, input, 6, 15, frame));
  cp::VehicleSetThrottle(base, v, PedalCurve(base, input, 4, 14, frame));
  if ((flags & 0x80000000) != 0) {
    const uint32_t on = guest::LoadU16(base, input + 10) == 32767 ? 1 : 0;
    cp::VehicleRampHandbrake(base, v, on, Fmuls(dt, Lfs(base, kFour)));
  } else {
    const int64_t value = int16_t(guest::LoadU16(base, input + 10));
    cp::VehicleSetHandbrake(base, v, IntThroughStack(base, frame + 88, uint64_t(value)));
  }
  cp::VehicleRampReverseThrottle(base, v, (flags & 0x200) != 0 ? 1 : 0, dt);
  if ((flags & 0x10000) != 0) {
    uint8_t on = 0;
    if (guest::LoadU8(base, v + 2424) == 0 && Lfs(base, v + 2420) > zero) {
      on = 1;
    }
    guest::StoreU8(base, v + 2424, on);
  }
  return true;
}

double VehicleSlipWheelValue(uint8_t* base, uint32_t vehicle, double speed, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C0310 Vehicle::SlipWheelValue");
  const uint32_t v = vehicle, f = frame;
  const uint32_t once = guest::LoadU32(base, kSlipFlag);
  if ((once & 1) == 0) {
    guest::StoreU32(base, kSlipFlag, once | 1);
    Stfs(base, kSlipThreshold, Fmuls(speed, Lfs(base, kPerKmh)));
  }
  const double zero = Lfs(base, kZero);
  double best = zero, most = zero;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t w = v + 368 * i;  // the wheel's fields at w + 433 to + 684
    if (guest::LoadU8(base, w + 433) == 0) {
      continue;
    }
    const uint32_t data = guest::LoadU32(base, v + 2160 + 4 * i);
    Stfs(base, f + 124, zero);
    Stfs(base, f + 92, zero);
    Stfs(base, f + 108, zero);
    const double spin = Lfs(base, w + 664);
    const uint32_t body = guest::LoadU32(base, v + 336);
    const double rolled = Fmuls(Lfs(base, data + 52), spin);
    Stfs(base, f + 84, zero);
    Stfs(base, f + 80, rolled);
    Stfs(base, f + 88, zero);
    va::RigidBodyPointVelocity(base, f + 144, body, w + 480);
    vl::Vec4Copy(base, f + 112, f + 144);
    Stfs(base, f + 96, double(vl::Vec3DotOf(base, f + 112, w + 560)));
    Stfs(base, f + 100, zero);
    Stfs(base, f + 104, double(vl::Vec3DotOf(base, f + 112, w + 544)));
    vl::Vec3Difference(base, f + 128, f + 96, f + 80);
    const double slip = double(vl::Sqrtf(double(vl::Vec3DotOf(base, f + 128, f + 128))));
    // bge on both: on only when the threshold and the most so far are below
    // it (a NaN keeps the wheel so far).
    if (Lfs(base, kSlipThreshold) < slip && most < slip) {
      best = Lfs(base, w + 684);
      most = slip;
    }
  }
  return best;
}

uint32_t RacersNearestHumanVehicle(uint8_t* base, uint32_t point, uint32_t n, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C2248 Racers::NearestHumanVehicle");
  const uint32_t f = frame;
  const int32_t count = int32_t(tc::RaceRacerCount(base));
  const double zero = Lfs(base, kZero);
  for (uint32_t k = 0; k < 7; k++) {
    Stfs(base, f + 96 + 8 * k, zero);
    guest::StoreU32(base, f + 100 + 8 * k, 0);
  }
  Stfs(base, f + 92, zero);
  CopyWord(base, point + 8, f + 88);
  CopyWord(base, point + 0, f + 80);
  CopyWord(base, point + 4, f + 84);
  uint32_t found = 0;
  for (int32_t racer = 0; racer < count; racer++) {
    const uint32_t flags = guest::LoadU32(base, minimap::RaceRacerVehicle(base, racer) + 6148);
    if ((flags & 0x1) != 0 || (flags & 0x80) != 0) {
      guest::StoreU32(base, f + 160 + 4 * found, uint32_t(racer));
      found++;
    }
  }
  for (uint32_t j = 0; j < found; j++) {
    const uint32_t racer = guest::LoadU32(base, f + 160 + 4 * j);
    const uint32_t vehicle = minimap::RaceRacerVehicle(base, racer);
    const uint32_t body = guest::LoadU32(base, vehicle + 336);
    vl::Vec3Difference(base, f + 208, f + 80, va::RigidBodyPosition(body));
    vl::Vec3Difference(base, f + 192, f + 80,
                       va::RigidBodyPosition(guest::LoadU32(base, vehicle + 336)));
    Stfs(base, f + 96 + 8 * j, double(vl::Vec3DotOf(base, f + 192, f + 208)));
    guest::StoreU32(base, f + 100 + 8 * j, racer);
  }
  // The selection sort: a pass per racer found, each over the racer count.
  int32_t next = 1;
  for (uint32_t pass = 0, at = f + 96; pass < found; pass++, at += 8, next++) {
    uint32_t least = at;
    if (next < count) {
      uint32_t other = at + 8;
      for (int32_t k = count - next; k > 0; k--, other += 8) {
        if (Lfs(base, least) > Lfs(base, other)) {
          least = other;
        }
      }
    }
    const uint32_t a0 = guest::LoadU32(base, at);
    const uint32_t m0 = guest::LoadU32(base, least), m1 = guest::LoadU32(base, least + 4);
    guest::StoreU32(base, least, a0);
    guest::StoreU32(base, least + 4, guest::LoadU32(base, at + 4));
    guest::StoreU32(base, at, m0);
    guest::StoreU32(base, at + 4, m1);
  }
  const uint32_t racer = guest::LoadU32(base, f + 100 + ((n << 3) & 0xFFFFFFF8u));
  return minimap::RaceRacerVehicle(base, racer);
}

}  // namespace vehicle
