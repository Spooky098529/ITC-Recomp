// The vehicle's physics objects (docs/functions/car-physics.md): its
// collision object in the world, the world's ground callback, its teardown.
// See car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "c_runtime/math.h"
#include "c_runtime/strings.h"
#include "collision/collision.h"
#include "matrix_math/matrix_math.h"
#include "memory/c_runtime_heap.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace va = vehicle_accessors;
namespace co = collision;

namespace {

constexpr uint32_t kWorldPointer = 0x826C2D88;  // the collision world
constexpr uint32_t kGroundCallback = 0x820FEAB0;  // TrackCollision::GroundCallback

uint32_t World(uint8_t* base) {
  return guest::LoadU32(base, kWorldPointer);
}

// Whether CollisionShape::Place runs natively on the record twice (the
// copies swapped between): the template and both copies boxes, and with
// the template lowered (+112 not 0.0) its stack copy native (the same
// place both times).
bool PlaceTwiceFits(uint8_t* base, uint32_t record, uint32_t place_frame) {
  const uint32_t source = guest::LoadU32(base, record + 0);
  if (!co::IsBox(base, source) || !co::IsBox(base, guest::LoadU32(base, record + 4)) ||
      !co::IsBox(base, guest::LoadU32(base, record + 8))) {
    return false;
  }
  const bool lowered = Lfs(base, record + 112) != Lfs(base, kZero);
  return !lowered || co::BoxCopyFits(place_frame + 80, source);
}

}  // namespace

void RigidBodySetFloat48(uint8_t* base, uint32_t body, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_8259D5A8 RigidBody::SetFloat48");
  CopyWord(base, from, body + 48);
}

bool CollisionWorldCallbackKnown(uint8_t* base) {
  return guest::LoadU32(base, World(base) + 8) == kGroundCallback;
}

bool CollisionWorldCallback(uint8_t* base, uint32_t result, uint32_t point, uint32_t position,
                            uint32_t r1, uint32_t* r3) {
  if (!CollisionWorldCallbackKnown(base)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B8798 CollisionWorld::Callback");
  // The tail call: the callback's 96-byte frame from the caller's r1.
  *r3 = track_collision::TrackCollisionGroundCallback(base, result, point, position, r1 - 96);
  return true;
}

bool CollisionWorldPairsMovable(uint8_t* base) {
  const uint32_t world = World(base);
  const uint32_t pairs = guest::LoadU32(base, world + 36);
  const uint32_t count = guest::LoadU32(base, world + 40);
  if (count < 2) {
    return true;
  }
  const uint64_t end = uint64_t(pairs) + 32ull * count;
  return end <= 0xA0000000ull && c_runtime::InOneView(pairs, 32 * count);
}

bool CollisionObjectResetTransform(uint8_t* base, uint32_t object, uint32_t frame) {
  const uint32_t record = object + 32;
  const uint32_t place_frame = frame - 608;
  if (!PlaceTwiceFits(base, record, place_frame)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B7998 CollisionObject::ResetTransform");
  guest::StoreU32(base, object + 288, 1);
  co::CollisionShapePlace(base, record,
                          va::RigidBodyMatrix(guest::LoadU32(base, object + 16)), place_frame);
  co::CollisionShapePlace(base, record,
                          va::RigidBodyMatrix(guest::LoadU32(base, object + 16)), place_frame);
  return true;
}

bool CollisionObjectInitFits(uint8_t* base, uint32_t object, uint32_t shape, uint32_t memory) {
  return co::IsBox(base, shape) && co::CollisionShapeDestroyable(base, object + 32) &&
         !(memory == 0 && memory::crt::GetProcessHeap(base) == 0);
}

void CollisionObjectInit(uint8_t* base, uint32_t object, uint32_t body, uint32_t shape,
                         uint32_t groups, uint32_t memory, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7A00 CollisionObject::Init");
  const uint32_t o = object;
  guest::StoreU32(base, o + 16, body);
  guest::StoreU32(base, o + 20, shape);
  guest::StoreU32(base, o + 244, groups);
  co::CollisionShapeSetUp(base, o + 32, shape, va::RigidBodyMatrix(body), memory, frame - 128);
  for (uint32_t at = 248; at <= 260; at += 4) {
    guest::StoreU32(base, o + at, 0xFFFFFFFFu);
  }
  vl::Vec4Copy(base, o + 160, kZeroVector);
  vl::Vec4Copy(base, o + 176, kZeroVector);
  guest::StoreU32(base, o + 192, 0);
  guest::StoreU32(base, o + 240, 0);
  guest::StoreU32(base, o + 236, 0);
  // +208 = (1, 1, 1) with its w loaded first and stored back (lfs then
  // stfs: its bits).
  const uint32_t w = guest::LoadU32(base, o + 220);
  CopyWord(base, kOne, o + 208);
  CopyWord(base, kOne, o + 212);
  CopyWord(base, kOne, o + 216);
  guest::StoreU32(base, o + 220, w);
  guest::StoreU32(base, o + 268, 0);
  CopyWord(base, kOne, o + 224);
  for (uint32_t at = 272; at <= 288; at += 4) {
    guest::StoreU32(base, o + at, 0);
  }
  guest::StoreU32(base, o + 264, 0);
  vl::Vec4Copy(base, o + 304, kZeroVector);
}

bool CollisionWorldAddObject(uint8_t* base, uint32_t body, uint32_t shape, uint32_t groups,
                             uint32_t frame, uint32_t* object) {
  const uint32_t free_object = guest::LoadU32(base, World(base) + 24);
  if (free_object == 0 ||
      !CollisionObjectInitFits(base, free_object, shape, guest::LoadU32(base, World(base) + 0))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B9570 CollisionWorld::AddObject");
  va::CollisionObjectUnlink(base, free_object, World(base) + 24);
  va::CollisionObjectLink(base, free_object, World(base) + 20);
  uint32_t world = World(base);
  guest::StoreU32(base, world + 28, guest::LoadU32(base, world + 28) + 1);
  world = World(base);
  CollisionObjectInit(base, free_object, body, shape, groups, guest::LoadU32(base, world + 0),
                      frame - 112);
  CollisionWorldAddPairs(base, free_object, guest::LoadU32(base, free_object + 248));
  *object = free_object;
  return true;
}

bool VehicleDestroyPhysics(uint8_t* base, uint32_t vehicle, uint32_t* r3) {
  const uint32_t object = guest::LoadU32(base, vehicle + 340);
  const uint32_t shape = guest::LoadU32(base, vehicle + 344);
  const uint32_t body = guest::LoadU32(base, vehicle + 336);
  // Everything checked first: the pairs' removal, the shape's slot 0 (a
  // box's: Shape::DeletingDestructor) and its block, the body's block.
  if ((object != 0 && !CollisionWorldPairsMovable(base)) ||
      (shape != 0 && (!co::IsBox(base, shape) || !co::Deletable(shape))) ||
      !co::Deletable(body)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B0E70 Vehicle::DestroyPhysics");
  if (object != 0) {
    uint32_t ignored = 0;
    CollisionWorldRemoveObject(base, object, &ignored);
    guest::StoreU32(base, vehicle + 340, 0);
  }
  // r3: the +344 loaded (slot 0 returns the shape), then the body's.
  *r3 = shape;
  if (shape != 0) {
    co::ShapeDeletingDestructor(base, shape, 1);
    guest::StoreU32(base, vehicle + 344, 0);
  }
  if (body != 0) {
    c_runtime::EmptyDestructor();
    memory::crt::OperatorDelete(body);
    guest::StoreU32(base, vehicle + 336, 0);
    *r3 = body;
  }
  return true;
}

}  // namespace car_physics

namespace car_physics {

namespace {

constexpr uint32_t kHalf = 0x82001BE4;          // 0.5
constexpr uint32_t kSpeedLimit = 0x82001FD8;    // the slip term's speed squared, at most
constexpr uint32_t kSlipRatio = 0x8200296C;     // side over forward below this ratio: divided
constexpr uint32_t kSlipScale = 0x820029F8;     // the slip term's scale
constexpr uint32_t kUnflipSpeed = 0x820021E8;   // +48 below it (2.0)
constexpr uint32_t kUnflipUp = 0x820029C8;      // +16 . +96 below it

uint32_t WheelAt(uint32_t vehicle, uint32_t w) {
  return vehicle + 416 + 368 * w;
}

// Three words copied (lfs then stfs: their bits) and a fourth from `w`.
void CopyXyz(uint8_t* base, uint32_t from, uint32_t to, uint32_t w) {
  CopyWord(base, from + 0, to + 0);
  CopyWord(base, from + 4, to + 4);
  CopyWord(base, from + 8, to + 8);
  CopyWord(base, w, to + 12);
}

}  // namespace

uint32_t VehicleProbeGround(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1) {
  ITC_NATIVE_RUNNING("sub_820B4420 Vehicle::ProbeGround");
  const uint32_t v = vehicle, f = frame;
  const uint32_t up = v + 96;
  // +96 = 0.0 three times, its w loaded first and stored back.
  const uint32_t up_w = guest::LoadU32(base, up + 12);
  CopyWord(base, kZero, up + 0);
  CopyWord(base, kZero, up + 4);
  CopyWord(base, kZero, up + 8);
  guest::StoreU32(base, up + 12, up_w);
  for (uint32_t w = 0; w < 4; w++) {
    const uint32_t wheel = WheelAt(v, w);
    CopyWord(base, kZero, f + 204);
    CopyWord(base, kZero, f + 220);
    uint32_t r3 = 0;
    CollisionWorldCallback(base, f + 176, wheel + 80, guest::LoadU32(base, wheel + 20), f, &r3);
    Stfs(base, wheel + 96, Fmuls(Lfs(base, f + 196), Lfs(base, f + 176)));
    vl::Vec4Copy(base, wheel + 112, f + 192);
    const uint32_t next = guest::LoadU32(base, f + 228);
    const uint32_t surface = guest::LoadU32(base, f + 224);
    guest::StoreU32(base, wheel + 20, next);
    guest::StoreU32(base, wheel + 176, surface);
    const double offset = VehicleSurfaceValue(base, v, w);
    Stfs(base, wheel + 96, Fadds(Lfs(base, wheel + 96), offset));
    vl::Vec4Copy(base, wheel + 160, wheel + 64);
    Stfs(base, wheel + 164, Fsubs(Lfs(base, wheel + 164), Lfs(base, wheel + 96)));
    vl::Vec3AddTo(base, up, wheel + 112);
  }
  // The axles' midpoints.
  const double half = Lfs(base, kHalf);
  vl::Vec3Sum(base, f + 160, v + 576, v + 944);
  vl::Vec4Scaled(base, f + 80, f + 160, half);
  vl::Vec4Copy(base, v + 1904, f + 80);
  vl::Vec3Sum(base, f + 80, v + 1312, v + 1680);
  vl::Vec4Scaled(base, f + 160, f + 80, half);
  vl::Vec4Copy(base, v + 2032, f + 160);
  // The ground frame.
  vl::Vec3NormalizeSelf(base, up);
  vl::Vec3Cross(base, f + 80, up, v + 32);
  vl::Vec4Copy(base, v + 112, f + 80);
  vl::Vec3NormalizeSelf(base, v + 112);
  vl::Vec3Cross(base, f + 80, v + 112, up);
  vl::Vec4Copy(base, v + 128, f + 80);
  vl::Vec3NormalizeSelf(base, v + 128);
  // The rows with w 1.0 through the stack (the x, y, w, z stores' order is
  // the original's; nothing overlaps).
  CopyXyz(base, v + 112, f + 112, kOne);
  vl::Vec4Copy(base, v + 144, f + 112);
  CopyXyz(base, up, f + 128, kOne);
  vl::Vec4Copy(base, v + 160, f + 128);
  CopyXyz(base, v + 128, f + 144, kOne);
  vl::Vec4Copy(base, v + 176, f + 144);
  // The slip term: the velocity copied (ld, std: its bits), +92 = 0.0 first.
  const uint32_t velocity = va::RigidBodyVelocity(guest::LoadU32(base, v + 336));
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 96 + i, guest::LoadU32(base, velocity + i));
  }
  const double zero = Lfs(base, kZero);
  Stfs(base, v + 92, zero);
  double speed2 = double(vl::Vec3Dot(base, f + 96, f + 96));
  const double limit = Lfs(base, kSpeedLimit);
  if (speed2 > limit) {  // ble ("not greater"): a NaN stays
    speed2 = limit;
  }
  const double forward = double(vl::Vec3DotOf(base, f + 96, v + 112));
  const double side = double(vl::Vec3DotOf(base, f + 96, v + 128));
  const double forward_size = Fabs(forward);
  const double bound = Fmuls(forward_size, Lfs(base, kSlipRatio));
  double ratio;
  if (Fabs(side) < bound) {
    ratio = Fdivs(side, forward_size);
  } else {
    ratio = double(float(c_runtime::Atan2(side, forward_size)));
  }
  Stfs(base, v + 92, Fmuls(Fmuls(ratio, speed2), Lfs(base, kSlipScale)));
  Stfs(base, v + 3112, double(vl::Vec3DotOf(base, v + 64, up)));
  va::RigidBodyPointVelocity(base, f + 80, guest::LoadU32(base, v + 336), v + 1888);
  vl::Vec4Copy(base, v + 1968, f + 80);
  va::RigidBodyPointVelocity(base, f + 80, guest::LoadU32(base, v + 336), v + 2016);
  vl::Vec4Copy(base, v + 2096, f + 80);
  matrix_math::InverseRotate(base, f + 80, v + 144,
                             va::RigidBodyVelocity(guest::LoadU32(base, v + 336)));
  vl::Vec4Copy(base, v + 384, f + 80);
  vl::Vec3Cross(base, f + 160, up, v + 16);
  const double roll = double(vl::Vec3DotOf(base, f + 160, v + 112));
  Stfs(base, v + 88, roll);
  *f1 = roll;
  return f + 160;
}

bool VehicleUnflip(uint8_t* base, uint32_t vehicle, uint32_t frame, uint32_t* r3) {
  const uint32_t v = vehicle, f = frame;
  if (!(Lfs(base, v + 48) < Lfs(base, kUnflipSpeed))) {  // bge: NaNs return
    ITC_NATIVE_RUNNING("sub_820AFE90 Vehicle::Unflip");
    *r3 = v;
    return true;
  }
  const double tilt = double(vl::Vec3DotOf(base, v + 16, v + 96));
  if (!(tilt < Lfs(base, kUnflipUp))) {
    ITC_NATIVE_RUNNING("sub_820AFE90 Vehicle::Unflip");
    *r3 = v + 16;  // Vec3::Dot leaves r3 as its first vector
    return true;
  }
  if (!CollisionWorldCallbackKnown(base)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AFE90 Vehicle::Unflip");
  CopyWord(base, kZero, f + 316);
  CopyWord(base, kZero, f + 332);
  uint32_t ignored = 0;
  CollisionWorldCallback(base, f + 288, va::ContactPosition(guest::LoadU32(base, v + 336)),
                         guest::LoadU32(base, v + 2984), f, &ignored);
  matrix_math::SetIdentity(base, f + 192);
  // Row 1: the normal (w 1.0).
  CopyXyz(base, f + 304, f + 96, kOne);
  vl::Vec4Copy(base, f + 208, f + 96);
  // Row 2: normalized(row 1 x +2528), through the stack (w 0.0, then 1.0).
  CopyXyz(base, f + 208, f + 160, kZero);
  vl::Vec3Cross(base, f + 256, f + 160, v + 2528);
  CopyXyz(base, f + 256, f + 144, kOne);
  vl::Vec4Copy(base, f + 224, f + 144);
  vl::Vec3NormalizeSelf(base, f + 224);
  // Row 0: normalized(row 2 x row 2), the zero vector's (the game's).
  CopyXyz(base, f + 224, f + 112, kZero);
  CopyXyz(base, f + 224, f + 128, kZero);
  vl::Vec3Cross(base, f + 256, f + 128, f + 112);
  CopyXyz(base, f + 256, f + 80, kOne);
  vl::Vec4Copy(base, f + 192, f + 80);
  vl::Vec3NormalizeSelf(base, f + 192);
  matrix_math::QuaternionFromMatrix(base, f + 272, f + 192);
  // The body's position lowered by the distance (its 16 bytes copied by
  // ld, std: their bits).
  const uint32_t position = va::ContactPosition(guest::LoadU32(base, v + 336));
  const uint32_t data = guest::LoadU32(base, v + 272);
  const double distance = Lfs(base, f + 288);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 176 + i, guest::LoadU32(base, position + i));
  }
  Stfs(base, f + 180, Fsubs(Lfs(base, f + 180), distance));
  va::RigidBodySetCenterOfMass(base, guest::LoadU32(base, v + 336), data + 96);
  RigidBodySetCenterOfMassPosition(base, guest::LoadU32(base, v + 336), f + 176, f - 96);
  const uint32_t body = guest::LoadU32(base, v + 336);
  RigidBodySetOrientation(base, body, f + 272, f - 96);
  guest::StoreU8(base, v + 3132, 0);
  *r3 = body + 336;  // SetOrientation's (UpdateOrigin's)
  return true;
}

uint32_t VehicleRefreshFromBody(uint8_t* base, uint32_t vehicle, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B62E8 Vehicle::RefreshFromBody");
  VehicleSyncFromBody(base, vehicle, frame - 368);
  double f1 = 0.0;
  VehicleProbeGround(base, vehicle, frame - 304, &f1);
  return VehicleUpdateWheelAxes(base, vehicle, frame - 256);
}

}  // namespace car_physics

namespace car_physics {

namespace {

constexpr uint32_t kTravelLimit = 0x82001BDC;  // the collision object's +144, at most (negated)

}  // namespace

uint32_t VehiclePhysicsStep(uint8_t* base, uint32_t vehicle, double dt, uint32_t frame,
                            const std::function<void(uint32_t address)>& original) {
  ITC_NATIVE_RUNNING("sub_820B6498 Vehicle::PhysicsStep");
  const uint32_t v = vehicle, f = frame;
  double f1 = 0.0;
  VehicleBeginStep(base, v, dt);
  guest::StoreU8(base, v + 2456, 1);
  VehicleSyncFromBody(base, v, f - 368);
  VehicleProbeGround(base, v, f - 304, &f1);
  VehicleUpdateSuspension(base, v, f - 144, &f1);
  // The smallest of the wheels' +0 and the limit (ble, "not greater": a
  // NaN on either side keeps the one so far).
  double travel = Lfs(base, kTravelLimit);
  for (uint32_t w = 0; w < 4; w++) {
    const double wheel_travel = Lfs(base, WheelAt(v, w) + 0);
    if (travel > wheel_travel) {
      travel = wheel_travel;
    }
  }
  Stfs(base, guest::LoadU32(base, v + 340) + 144, Fneg(travel));
  VehicleTractionControl(base, v, f);
  VehicleShiftGears(base, v, f - 96);
  if (!VehicleUpdateSteering(base, v, f - 160)) {
    original(0x820B18B8);
  }
  VehicleUpdateWheelAxes(base, v, f - 256);
  VehicleSuspensionTorques(base, v, f - 256);
  VehicleWheelLoads(base, v);
  VehicleUpdateTireGrip(base, v);
  if (!VehicleUpdateTorqueSplit(base, v)) {
    original(0x820B2100);
  }
  VehicleUpdateEngineTorque(base, v, f - 96);
  VehicleUpdateTurbo(base, v);
  VehicleUpdateAtRest(base, v, f - 192);
  for (uint32_t w = 0; w < 4; w++) {
    CopyWord(base, kZero, WheelAt(v, w) + 284);
    CopyWord(base, kZero, WheelAt(v, w) + 288);
  }
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, WheelAt(v, w) + 204, 0);
  }
  const uint32_t data = guest::LoadU32(base, v + 272);
  CopyWord(base, data + 140, v + 2000);
  CopyWord(base, data + 144, v + 2128);
  // The substeps (the count read again each time, cmplw: unsigned).
  for (uint32_t i = 0; i < guest::LoadU32(base, v + 2368); i++) {
    VehicleAntiLock(base, v);
    VehicleBrakeTorques(base, v);
    VehicleUpdateTireForces(base, v, f - 272);
    VehicleAccumulateTireForces(base, v, f - 192);
    VehicleRollingResistance(base, v);
    VehicleUpdateDrivetrain(base, v, f - 176);
  }
  VehicleUpdateAero(base, v, f - 240);
  VehicleApplyForces(base, v, f - 352);
  VehicleSyncFromBody(base, v, f - 368);
  VehicleProbeGround(base, v, f - 304, &f1);
  VehicleUpdateWheelAxes(base, v, f - 256);
  VehicleUpdateBrakeHeat(base, v);
  VehicleUpdateBackfire(base, v, f);
  VehicleUpdateTireWear(base, v, f - 112);
  VehicleTurnWheels(base, v, f);
  uint32_t r3 = 0;
  VehicleUnflip(base, v, f - 384, &r3);
  CopyWord(base, v + 2516, v + 2520);
  guest::StoreU8(base, v + 2444, 0);
  guest::StoreU8(base, v + 2445, 0);
  return r3;
}

}  // namespace car_physics
