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
#include "memory/c_runtime_heap.h"
#include "track_collision/track_collision.h"
#include "track_route/track_route.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace vehicle {

using namespace car_physics::ops;
namespace cp = car_physics;
namespace co = collision;
namespace tc = track_collision;
namespace va = vehicle_accessors;

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

}  // namespace vehicle
