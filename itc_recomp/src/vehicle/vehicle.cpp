// The vehicle code above the car physics (docs/functions/vehicle.md): the
// leaves, the route position's step, the track positions, the vehicles'
// lifetime. See vehicle.h.

#include "vehicle/vehicle.h"

#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"

#include "ai_drivers/ai_drivers.h"
#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "matrix_math/matrix_math.h"
#include "minimap/minimap.h"
#include "strings_and_font/steps.h"
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

// --- The engine's power curve ---

namespace {

constexpr uint32_t kRpmStep = 0x82003314;     // 400.0
constexpr uint32_t kHundred = 0x82002A0C;     // 100.0
constexpr uint32_t kPowerScale = 0x82003380;  // torque x rpm to power
constexpr uint32_t kTopRpm = 0x82003384;      // 800.0 (the range's start)
constexpr uint32_t kTwo = 0x820021E8;         // 2.0
constexpr uint32_t kHundredth = 0x8200222C;   // 0.01

}  // namespace

void PowerCurveShiftPeak(uint8_t* base, uint32_t curve, uint32_t shift, uint32_t r1) {
  ITC_NATIVE_RUNNING("sub_820CE1D8 PowerCurve::ShiftPeak");
  const uint32_t c = curve;
  const uint32_t peak = guest::LoadU32(base, c + 1024);
  double low = Lfs(base, c + 1040);
  const double step = Lfs(base, kRpmStep);
  const double shifted = IntThroughStack(base, r1 - 576, uint64_t(int64_t(int32_t(shift))));
  const double by = Fmuls(shifted, Lfs(base, kHundred));
  const double index =
      IntThroughStack(base, r1 - 576, uint64_t(int64_t(int32_t(peak + 1))));
  double rpm = Fmadds(index, step, by);
  Stfs(base, c + 1052, rpm);
  if (rpm < low) {
    rpm = low;
  } else {
    const double high = Lfs(base, c + 1044);
    if (rpm > high) {
      rpm = high;
    }
  }
  const uint32_t count = guest::LoadU32(base, c + 1028);
  Stfs(base, c + 1052, rpm);
  const double top = Fsubs(rpm, step);  // the new peak's entry
  const double zero = Lfs(base, kZero);
  Stfs(base, c + 1056, zero);
  Stfs(base, c + 1060, zero);
  // The rpm axis remapped below r1: the points below the peak scaled to end
  // at it, those after stretched from it to the end.
  const uint32_t table = r1 - 528;
  if (int32_t(count) > 0) {
    for (int32_t i = 0; i < int32_t(count); i++) {
      const uint32_t at = table + 16 * uint32_t(i);
      if (i < int32_t(peak)) {
        const double a = IntThroughStack(base, r1 - 576, uint64_t(int64_t(i)));
        const double b = IntThroughStack(base, r1 - 544, uint64_t(int64_t(int32_t(peak))));
        Stfs(base, at, Fdivs(Fmuls(a, top), b));
      } else if (i > int32_t(peak)) {
        const double n = IntThroughStack(base, r1 - 568, uint64_t(int64_t(int32_t(count))));
        const double k =
            IntThroughStack(base, r1 - 560, uint64_t(int64_t(int32_t(uint32_t(i) - peak))));
        const double rest =
            IntThroughStack(base, r1 - 552, uint64_t(int64_t(int32_t(count - peak))));
        Stfs(base, at, Fadds(Fdivs(Fmuls(Fmsubs(n, step, top), k), rest), top));
      } else {
        Stfs(base, at, top);
      }
      const double remapped = Lfs(base, at);
      const uint32_t torque = guest::LoadU32(base, c + 4 + 16 * uint32_t(i));
      Stfs(base, at, Fadds(remapped, step));
      guest::StoreU32(base, at + 4, torque);  // lfs then stfs: its bits
    }
  }
  // Each point's torque looked up at its rpm on the remapped axis.
  if (int32_t(count) > 0) {
    const double scale = Lfs(base, kPowerScale);
    for (uint32_t j = 0; int32_t(j) < int32_t(guest::LoadU32(base, c + 1028)); j++) {
      const uint32_t out = c + 516 + 16 * j;
      const int32_t n = int32_t(guest::LoadU32(base, c + 1028));
      const double x = Lfs(base, c + 16 * j);
      double torque = Lfs(base, c + 4 + 16 * j);
      if (n > 1) {
        for (int32_t k = 1; k < int32_t(guest::LoadU32(base, c + 1028)); k++) {
          const uint32_t e = table + 16 * uint32_t(k);
          if (!(Lfs(base, e - 16) > x) && x < Lfs(base, e)) {
            const double x0 = Lfs(base, e - 16);
            const double x1 = Lfs(base, e);
            const double dx = Fsubs(x, x0);
            const double span = Fsubs(x1, x0);
            const double t0 = Lfs(base, e - 12);
            const double t1 = Lfs(base, e + 4);
            const double dt = Fsubs(t1, t0);
            torque = Fmadds(Fdivs(dx, span), dt, t0);
            break;
          }
        }
      }
      const double product = Fmuls(torque, x);
      Stfs(base, out, torque);
      Stfs(base, out - 4, x);
      const double power = Fmuls(product, scale);
      Stfs(base, out + 4, power);
      if (Lfs(base, c + 1056) < power) {
        Stfs(base, c + 1056, power);
        CopyWord(base, out - 4, c + 1060);
      }
    }
  }
}

void PowerCurveBuild(uint8_t* base, uint32_t curve, uint32_t points, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820CE430 PowerCurve::Build");
  const uint32_t c = curve;
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t at = c + 12 + 16 * i;
    for (uint32_t k = 0; k < 4; k++) {
      Stfs(base, at + 512 - 4 * k, zero);
      Stfs(base, at - 4 * k, zero);
    }
  }
  CopyWord(base, points + 128, c + 1048);
  Stfs(base, c + 1040, Lfs(base, kTopRpm));
  const double step = Lfs(base, kRpmStep);
  guest::StoreU32(base, c + 1028, 0);
  const double scale = Lfs(base, kPowerScale);
  const double two = Lfs(base, kTwo);
  const double least = Lfs(base, kHundredth);
  double most = zero;
  for (uint32_t i = 0; i < 32; i++) {
    const uint32_t e = c + 16 * i;
    const double point = Lfs(base, points + 4 * i);
    if (point < least) {
      // From the two entries before it (before the curve for the first).
      const double t2 = Lfs(base, e - 28);
      double torque = Fmsubs(Lfs(base, e - 12), two, t2);
      const double rpm =
          Fmuls(IntThroughStack(base, frame + 80, uint64_t(int64_t(int32_t(i + 1)))), step);
      Stfs(base, e + 0, rpm);
      if (!(torque > zero)) {
        torque = zero;
      }
      const double product = Fmuls(rpm, torque);
      Stfs(base, e + 4, torque);
      Stfs(base, e + 8, Fmuls(product, scale));
    } else {
      guest::StoreU32(base, c + 1028, guest::LoadU32(base, c + 1028) + 1);
      const double rpm =
          Fmuls(IntThroughStack(base, frame + 88, uint64_t(int64_t(int32_t(i + 1)))), step);
      Stfs(base, e + 0, rpm);
      const double torque = Fmuls(Lfs(base, c + 1048), Lfs(base, points + 4 * i));
      Stfs(base, e + 4, torque);
      Stfs(base, e + 8, Fmuls(Fmuls(torque, rpm), scale));
      if (most < torque) {
        most = torque;
        guest::StoreU32(base, c + 1024, i);
      }
    }
  }
  const uint32_t peak = (guest::LoadU32(base, c + 1024) << 4) & 0xFFFFFFF0u;
  const double start = Lfs(base, c + 1040);
  const double end = Lfs(base, c + 1044);
  guest::StoreU32(base, c + 1032, uint32_t(strings_and_font::steps::Fctiwz(
                                      Fmuls(Fsubs(start, Lfs(base, c + peak)), least))));
  int32_t below = int32_t(guest::LoadU32(base, c + 1032));
  guest::StoreU32(base, c + 1036, uint32_t(strings_and_font::steps::Fctiwz(
                                      Fmuls(Fsubs(end, Lfs(base, c + peak)), least))));
  if (below < -20) {
    below = -20;
  }
  int32_t above = int32_t(guest::LoadU32(base, c + 1036));
  guest::StoreU32(base, c + 1032, uint32_t(below));
  if (above > 20) {
    above = 20;
  }
  guest::StoreU32(base, c + 1036, uint32_t(above));
  PowerCurveShiftPeak(base, c, 0, frame);
}

void PowerCurveFromEngine(uint8_t* base, uint32_t curve, uint32_t engine, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820CE7D8 PowerCurve::FromEngine");
  CopyWord(base, engine + 12, curve + 1044);
  PowerCurveBuild(base, curve, engine + 24, frame);
}

uint32_t VehicleStatusReset(uint8_t* base, uint32_t status, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820BFC48 VehicleStatus::Reset");
  const uint32_t s = status;
  const double zero = Lfs(base, kZero);
  Stfs(base, s + 76, zero);
  Stfs(base, s + 0, zero);
  guest::StoreU32(base, s + 72, 0);
  Stfs(base, s + 84, zero);
  guest::StoreU32(base, s + 80, 0);
  Stfs(base, s + 88, zero);
  guest::StoreU32(base, s + 8, 0);
  const double eighty = Lfs(base, 0x82001E8C);
  Stfs(base, s + 92, eighty);
  guest::StoreU32(base, s + 12, 0);
  Stfs(base, s + 96, eighty);
  guest::StoreU16(base, s + 4, 0xFFFF);
  guest::StoreU16(base, s + 6, 0xFFFF);
  Stfs(base, s + 100, Lfs(base, kOne));
  PowerCurveFromEngine(base, frame + 80, vehicle + 4644, frame - 112);
  PowerCurveShiftPeak(base, frame + 80, 0, frame);
  CopyWord(base, frame + 80 + 1048, s + 104);
  return frame + 80;
}

// --- The vehicle set up from a car ---

namespace {

constexpr uint32_t kGripA = 0x826014A0, kGripB = 0x82601560, kGripRecords = 0x82601580;
constexpr uint32_t kGripScale = 0x82601488;  // .data

}  // namespace

uint32_t VehicleSetUpSurfaceGrip(uint8_t* base, uint32_t vehicle, uint32_t car, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820C1E00 Vehicle::SetUpSurfaceGrip");
  const uint32_t v = vehicle, f = frame;
  const double one = Lfs(base, kOne);
  // The four stack tables (nine floats each), in the original's order.
  Stfs(base, f + 112, one);
  const uint32_t flags = guest::LoadU32(base, car + 3888);
  Stfs(base, f + 116, one);
  Stfs(base, f + 120, one);
  Stfs(base, f + 124, one);
  const double base_factor = Lfs(base, 0x82001070);
  Stfs(base, f + 128, base_factor);
  Stfs(base, f + 144, base_factor);
  const double a2c = Lfs(base, 0x82002A2C);
  Stfs(base, f + 132, a2c);
  Stfs(base, f + 140, a2c);
  const double a08 = Lfs(base, 0x82002A08);
  for (uint32_t at = 208; at <= 232; at += 4) {
    Stfs(base, f + at, a08);
  }
  const double bb4 = Lfs(base, 0x82002BB4);
  const double bbc = Lfs(base, 0x82002BBC);
  Stfs(base, f + 136, bbc);
  Stfs(base, f + 240, bb4);
  Stfs(base, f + 160, bb4);
  Stfs(base, f + 164, bb4);
  const double zero = Lfs(base, kZero);
  for (uint32_t at = 256; at <= 284; at += 4) {
    Stfs(base, f + at, zero);
  }
  Stfs(base, f + 288, Lfs(base, 0x82002CC4));
  Stfs(base, f + 168, bb4);
  Stfs(base, f + 172, bb4);
  Stfs(base, f + 180, bb4);
  const double cc0 = Lfs(base, 0x82002CC0);
  Stfs(base, f + 184, bb4);
  Stfs(base, f + 188, bb4);
  const double hundredth = Lfs(base, kHundredth);
  Stfs(base, f + 236, hundredth);
  const double cbc = Lfs(base, 0x82002CBC);
  Stfs(base, f + 176, cc0);
  Stfs(base, f + 192, cbc);
  guest::StoreU32(base, v + 2496, (flags & 0x2) != 0 || (flags & 0x4) != 0 ? 0 : 1);
  const double scale = Lfs(base, 0x8200256C);
  const double quarter = Lfs(base, 0x82002968);
  uint32_t r3 = 0;
  for (uint32_t axle = 0; axle < 2; axle++) {
    const uint32_t type = guest::LoadU32(base, car + (axle == 0 ? 1712 : 1720));
    uint32_t grade = guest::LoadU32(base, car + (axle == 0 ? 1716 : 1724));
    if (type == 0) {
      grade = 0;
    }
    const double g =
        Fmuls(IntThroughStack(base, f + 296, uint64_t(int64_t(int32_t(grade)))), quarter);
    const uint32_t records = v + (axle == 0 ? 5716 : 5932);
    const uint32_t pair = (type << 3) & 0xFFFFFFF8u;
    const double soften = Fadds(g, one);
    const double a0 = Lfs(base, kGripA + pair), a1 = Lfs(base, kGripA + 4 + pair);
    const double b0 = Lfs(base, kGripB + pair);
    const double da = Fsubs(a1, a0);
    const double b1 = Lfs(base, kGripB + 4 + pair);
    const double db = Fsubs(b1, b0);
    const double a = Fmadds(da, g, a0);
    const double b = Fmadds(db, g, b0);
    const double first = Fmadds(a, scale, base_factor);
    const double second = Fmadds(b, scale, base_factor);
    const uint32_t nine = type * 9;
    for (uint32_t j = 0; j < 9; j++) {
      const uint32_t at = ((nine + j) << 2) & 0xFFFFFFFCu;
      const double factor = (j == 1 || j == 8) ? second : first;
      const double grip = Fmuls(Fmuls(Lfs(base, kGripRecords + at), factor), Lfs(base, kGripScale));
      const uint32_t record = records + 24 * j;
      Stfs(base, f + 92, grip);
      const double squared = Fmuls(grip, grip);
      Stfs(base, f + 84, Fmuls(Lfs(base, f + 112 + 4 * j), hundredth));
      CopyWord(base, f + 160 + 4 * j, f + 80);
      const double ratio = Fdivs(squared, soften);
      CopyWord(base, f + 208 + 4 * j, f + 88);
      CopyWord(base, f + 256 + 4 * j, f + 100);
      Stfs(base, f + 96, Fmuls(ratio, quarter));
      if (!c_runtime::Memcpy(base, record, f + 80, 24)) {
        // A stack range and the vehicle's never share memory.
        ITC_LOG_ERROR("vehicle: SetUpSurfaceGrip's record copy refused");
        for (uint32_t i = 0; i < 24; i++) {
          guest::StoreU8(base, record + i, guest::LoadU8(base, f + 80 + i));
        }
      }
      guest::StoreU32(base, v + 4 * (j + (axle == 0 ? 667 : 699)), record);
      r3 = record;
    }
  }
  return r3;
}

uint32_t VehicleSetUp(uint8_t* base, uint32_t vehicle, uint32_t car, uint32_t frame, bool reset,
                      bool native_heap, const CalleeOriginal& original, double* f1) {
  if (reset) {
    ITC_NATIVE_RUNNING("sub_820C33B8 Vehicle::ResetForCar");
  } else {
    ITC_NATIVE_RUNNING("sub_820C3150 Vehicle::SetUp");
  }
  const uint32_t v = vehicle, f = frame;
  const uint32_t block2 = va::ObjectRecord1888(car);
  const uint32_t data = v + 3376;
  guest::StoreU32(base, v + 6152, 0);
  const uint32_t flags = guest::LoadU32(base, car + 3888);
  guest::StoreU32(base, v + 5712, car);
  guest::StoreU32(base, v + 6148, flags);
  double ignored = 0.0;
  if (!cp::VehicleSetupFromCar(base, data, car, f - 368, &ignored)) {
    original(0x820C27E8, data, car, 0, 0);
  }
  VehicleStatusReset(base, v + 6160, v, f - 1168);
  uint32_t r3 = 0;
  if (reset) {
    if (!native_heap || !cp::VehicleResetPhysics(base, v, data, 7, f - 208, &ignored, &r3)) {
      original(0x820B5B08, v, data, 7, 0);
    }
  } else {
    if (!native_heap || !cp::VehicleSetUpPhysics(base, v, data, 1, 7, f - 208, &ignored, &r3)) {
      original(0x820B5670, v, data, 1, 7);
    }
  }
  if (!VehicleLinkTrackPositions(base, v, &r3)) {
    original(0x820C1D08, v, 0, 0, 0);
  }
  guest::StoreU8(base, v + 2664, 1);
  const int8_t extra = int8_t(guest::LoadU8(base, car + 76));
  double value = 0.0;
  uint32_t on = 0;
  if (extra == 0) {
    value = Lfs(base, kZero);
  } else {
    on = 1;
    value = Fmuls(IntThroughStack(base, f + 80, uint64_t(int64_t(extra))), Lfs(base, 0x82002BBC));
  }
  cp::VehicleSetValue2388(base, v, value, on);
  guest::StoreU8(base, v + 2386, int8_t(guest::LoadU8(base, car + 77)) != 0 ? 1 : 0);
  uint32_t mode = 0;
  uint8_t boosted = 0;
  if (guest::LoadU8(base, block2 + 131) == 1) {
    mode = 6;
    boosted = 1;
  } else {
    mode = guest::LoadU32(base, car + 1672);
    boosted = int32_t(mode) >= 3 ? 1 : 0;
  }
  guest::StoreU8(base, v + 2512, boosted);
  guest::StoreU32(base, v + 2524, mode);
  if (guest::LoadU8(base, block2 + 69) == 1) {
    if (int32_t(guest::LoadU32(base, car + 1656)) >= 8) {
      guest::StoreU8(base, data + 1605, 1);
      if (!reset) {
        guest::StoreU8(base, v + 2512, 1);
        guest::StoreU32(base, v + 2524, int32_t(mode) < 3 ? 3 : mode);
      }
    } else {
      guest::StoreU8(base, data + 1605, 0);
    }
  }
  guest::StoreU32(base, v + 2372, guest::LoadU32(base, v + 2372) | 2);
  guest::StoreU8(base, v + 2249, guest::LoadU32(base, car + 3892) != 0 ? 1 : 0);
  VehicleSetUpSurfaceGrip(base, v, car, f - 480);
  guest::StoreU8(base, v + 2380, 1);
  guest::StoreU8(base, v + 2384, 0);
  // The nitrous' tables (probably), in the original's order.
  guest::StoreU32(base, f + 96, 0);
  const uint32_t box = guest::LoadU32(base, v + 344);
  Stfs(base, f + 128, Lfs(base, kOne));
  guest::StoreU32(base, f + 112, 570);
  Stfs(base, f + 132, Lfs(base, kOneAndHalf));
  guest::StoreU32(base, f + 116, 630);
  Stfs(base, f + 136, Lfs(base, 0x82002CF4));
  guest::StoreU32(base, f + 100, 390);
  Stfs(base, f + 140, Lfs(base, 0x82002C40));
  Stfs(base, f + 144, Lfs(base, 0x82002C1C));
  guest::StoreU32(base, f + 104, 450);
  guest::StoreU32(base, f + 120, 690);
  guest::StoreU32(base, f + 108, 510);
  const uint32_t level = (guest::LoadU32(base, car + 1676) << 2) & 0xFFFFFFFCu;
  Stfs(base, f + 148, Lfs(base, 0x82002CF0));
  Stfs(base, f + 152, Lfs(base, 0x82002CEC));
  CopyWord(base, f + 128 + level, v + 2416);
  const int64_t extra_rpm = int32_t(guest::LoadU32(base, f + 96 + level));
  Stfs(base, v + 2420, IntThroughStack(base, f + 80, uint64_t(extra_rpm)));
  *f1 = value;
  if (box != 0) {
    const uint32_t car_flags = guest::LoadU32(base, car + 3888);
    const uint32_t half = (car_flags & 0x4) != 0 ? 2 : ((car_flags >> 1) & 1);
    va::ObjectSetHalf112(base, box, half & 0xFFFF);
  }
  return box;
}

// --- The vehicle's tick ---

namespace {

constexpr uint32_t kKmh = 0x820029E0;     // 3.6
constexpr uint32_t kInputs = 0x826C5B30;  // the racers' car input records

}  // namespace

bool VehicleStatusUpdate(uint8_t* base, uint32_t status, uint32_t vehicle, uint32_t car,
                         uint32_t flag, uint32_t frame) {
  const uint32_t s = status, v = vehicle, f = frame;
  if (!c_runtime::Memcpy(base, f + 208, v + 5232, 96)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820C23E8 VehicleStatus::Update");
  guest::StoreU32(base, f + 80, guest::LoadU32(base, f + 212));
  const uint32_t body = guest::LoadU32(base, v + 336);
  const uint32_t position = va::RigidBodyPosition(body);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 128 + i, guest::LoadU32(base, position + i));
  }
  RoutePositionStep(base, f + 80, 1, 0xFFFFFFFFu, f - 112);
  auto lha = [&](uint32_t at) { return uint32_t(int32_t(int16_t(guest::LoadU16(base, at)))); };
  track_route::SlicePoint(base, f + 112, 0, lha(f + 212), lha(f + 214));
  track_route::SlicePoint(base, f + 176, 0, lha(f + 80), lha(f + 82));
  const double zero = Lfs(base, kZero);
  Stfs(base, f + 132, zero);
  Stfs(base, f + 116, zero);
  Stfs(base, f + 180, zero);
  vl::Vec3Sub(base, f + 160, f + 176, f + 112);
  const uint32_t direction = s + 32;
  const double length = double(vl::Vec3Normalize(base, direction, f + 160));
  vl::Vec3Sub(base, f + 192, f + 128, f + 112);
  if (length == zero) {
    Stfs(base, s + 68, zero);
  } else {
    Stfs(base, s + 68, Fdivs(double(vl::Vec3Dot(base, f + 160, f + 192)), length));
  }
  const uint32_t point = s + 48;
  vl::Vec4ScaleTo(base, point, direction, Lfs(base, s + 68));
  vl::Vec3Add(base, point, point, f + 112);
  Stfs(base, s + 64, double(vl::Vec3Direction(base, f + 96, point, f + 128)));
  vl::Vec3CrossW1(base, f + 96, f + 160, f + 192);
  if (Lfs(base, f + 100) < zero) {
    Stfs(base, s + 64, Fmuls(Lfs(base, s + 64), Lfs(base, 0x82001BE0)));
  }
  const uint32_t axis = s + 16;
  matrix_math::QuaternionXAxis(base, axis, va::RigidBodyField80(guest::LoadU32(base, v + 336)),
                               f - 96);
  Stfs(base, s + 20, zero);
  vl::Vec3Normalize(base, axis, axis);
  const double along = double(vl::Vec3Dot(base, axis, direction));
  // Probably wrong way, with hysteresis.
  if (guest::LoadU32(base, s + 8) != 0) {
    const double limit = Fneg(double(float(c_runtime::Cos(
        std::bit_cast<double>((uint64_t(guest::LoadU32(base, 0x82002CE0)) << 32) |
                              guest::LoadU32(base, 0x82002CE4))))));
    if (along > limit) {
      guest::StoreU32(base, s + 8, 0);
    }
  } else {
    const double limit = Fneg(double(float(c_runtime::Cos(
        std::bit_cast<double>((uint64_t(guest::LoadU32(base, 0x82002CD8)) << 32) |
                              guest::LoadU32(base, 0x82002CDC))))));
    if (along < limit) {  // bge: a NaN leaves it
      guest::StoreU32(base, s + 8, 1);
    }
  }
  const double per_tick = Lfs(base, 0x820029EC);  // 1/30
  if (flag != 0) {
    const double moved = Fmuls(Fmuls(Lfs(base, v + 48), per_tick), Lfs(base, 0x820029D8));
    const double trip = Lfs(base, s + 84);
    Stfs(base, s + 88, Fadds(Lfs(base, s + 88), moved));
    Stfs(base, s + 84, Fadds(trip, moved));
  }
  const double hundredth = Lfs(base, kHundredth);
  if (car != 0) {
    // The engine's temperatures (probably), eased toward targets.
    const int64_t stage = int32_t(guest::LoadU32(base, car + 1656));
    const double top = Lfs(base, v + 4648);
    const int64_t part = int32_t(guest::LoadU32(base, car + 1664));
    const double rpm = Lfs(base, v + 2224);
    const double ratio = Fdivs(rpm, top);
    const double water = Lfs(base, s + 92);
    const double oil = Lfs(base, s + 96);
    guest::StoreU32(base, f + 144, uint32_t(uint64_t(stage) >> 32));
    guest::StoreU32(base, f + 148, uint32_t(uint64_t(stage)));
    const double f9a = Lfs(base, 0x8200257C);
    guest::StoreU32(base, f + 96, uint32_t(uint64_t(part) >> 32));
    guest::StoreU32(base, f + 100, uint32_t(uint64_t(part)));
    const double part_f = double(float(double(part)));
    const double eighty = Lfs(base, 0x82001E8C);
    const double f7a = Lfs(base, 0x82002CD0);
    const double f6a = Lfs(base, 0x82002CCC);
    const double stage_d = double(stage);
    double x = Fmuls(ratio, Lfs(base, kHundred));
    const double stage_f = double(float(stage_d));
    x = Fmuls(x, x);
    const double level = Fsubs(stage_f, part_f);
    double load = Fmuls(x, hundredth);
    const double level2 = Fadds(level, Lfs(base, 0x82002CAC));
    const double f10b = Lfs(base, 0x82002578);
    load = Fmuls(load, hundredth);
    const double rate_a = Fmadds(level2, f7a, f10b);
    const double rate_b = Fmadds(level2, f6a, f10b);
    const double f12c = Lfs(base, 0x82002A20);
    const double target_a = Fmadds(load, f9a, f12c);
    const double target_b = Fmadds(load, eighty, f12c);
    const double da = Fmuls(Fsubs(target_a, water), rate_a);
    const double db = Fmuls(Fsubs(target_b, oil), rate_b);
    Stfs(base, s + 92, Fmadds(da, per_tick, water));
    Stfs(base, s + 96, Fmadds(db, per_tick, oil));
    const double rpm2 = Lfs(base, v + 2228);
    const double curve = Fadds(Fnmsubs(rpm2, rpm2, rpm2), rpm2);
    const double root = double(float(std::sqrt(Fmuls(curve, Lfs(base, 0x82002814)))));
    const double torque = cp::VehicleEngineTorque(base, v, rpm2, f);
    const double heat = Fsubs(Lfs(base, 0x820027A4), Lfs(base, s + 96));
    const double ratio2 = double(float(std::sqrt(Fdivs(torque, Lfs(base, s + 104)))));
    double value = Fmuls(Fmuls(ratio2, root), Lfs(base, 0x82002BA4));
    value = Fmuls(Fmuls(value, heat), hundredth);
    Stfs(base, s + 100, value);
  }
  // The brakes' efficiency (+2432) and +2428, lowered.
  const double speed = Fmuls(Lfs(base, v + 48), per_tick);
  const double a = Lfs(base, v + 3544);
  const double fade = Fsubs(Lfs(base, v + 3540), a);
  const double brake = Lfs(base, v + 292);
  const double efficiency = Lfs(base, v + 2432);
  double lowered = Fmuls(Fmuls(Fmuls(fade, hundredth), Lfs(base, 0x82002A44)), speed);
  const double least = Lfs(base, kHalf);
  lowered = Fsubs(efficiency, Fmuls(Fmuls(lowered, brake), hundredth));
  if (lowered < least) {
    lowered = least;
  }
  Stfs(base, v + 2432, lowered);
  double wear = Fmuls(Lfs(base, s + 96), Lfs(base, 0x82002CC8));
  wear = Fsubs(Lfs(base, v + 2428), Fmuls(wear, hundredth));
  const double floor = Lfs(base, 0x82002A3C);
  if (wear < floor) {
    wear = floor;
  }
  Stfs(base, v + 2428, wear);
  return true;
}

void VehicleTick(uint8_t* base, uint32_t vehicle, uint32_t racer, uint32_t car, uint32_t flag,
                 uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_820C3020 Vehicle::Tick");
  const uint32_t v = vehicle, f = frame;
  const uint32_t s = v + 6160;
  const double kmh = Fmuls(Lfs(base, v + 48), Lfs(base, kKmh));
  const uint32_t input = guest::LoadU32(base, kInputs + ((racer << 2) & 0x3FCu));
  Stfs(base, s + 0, kmh);
  guest::StoreU16(base, s + 4, guest::LoadU16(base, v + 5236));
  guest::StoreU16(base, s + 6, guest::LoadU16(base, v + 5238));
  for (int step = 0; step < 2; step++) {
    const uint32_t input_frame = f - 176;
    auto steer = [&](double target, double dt, double f3, uint32_t left, uint32_t right) {
      GuestRegisters r;
      r.r3 = v;
      r.r4 = left;
      r.r5 = right;
      r.f1 = target;
      r.f2 = dt;
      r.f3 = f3;
      call(0x820B5FD8, input_frame, r);
    };
    if (!VehicleApplyInput(base, v, input, input_frame, steer)) {
      GuestRegisters r;
      r.r3 = v;
      r.r4 = input;
      call(0x820BFD30, f, r);
    }
    guest::StoreU32(base, input, guest::LoadU32(base, input) & 0xFFFFFFFCu & ~0x10000u);
    const double dt = Lfs(base, kStepSeconds);
    if (cp::CollisionWorldCallbackKnown(base)) {
      cp::VehiclePhysicsStep(base, v, dt, f - 112, [&](uint32_t address) {
        GuestRegisters r;
        r.r3 = v;
        call(address, f - 112, r);
      });
    } else {
      GuestRegisters r;
      r.r3 = v;
      r.f1 = dt;
      call(0x820B6498, f, r);
    }
    VehicleUpdateTrackPositions(base, v, f - 112);
  }
  if (!VehicleStatusUpdate(base, s, v, car, flag, f - 384)) {
    GuestRegisters r;
    r.r3 = s;
    r.r4 = v;
    r.r5 = car;
    r.r6 = flag;
    call(0x820C23E8, f, r);
  }
}

}  // namespace vehicle
