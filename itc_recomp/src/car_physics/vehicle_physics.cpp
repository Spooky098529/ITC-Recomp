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

#include <algorithm>
#include <cstring>

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

namespace car_physics {

namespace {

constexpr uint32_t kMassScale = 0x820021E8;      // the axles' masses' factor (2.0)
constexpr uint32_t kFloat297C = 0x8200297C;      // the body's +48
constexpr uint32_t kFloat2980 = 0x82002980;      // the gauges' scale
constexpr uint32_t kBodyFloat112 = 0x820021F8;   // the body's +112
constexpr uint32_t kGravityY = 0x82002A8C;       // the gravity's y
constexpr uint32_t kObjectSizeY = 0x82002968;    // the collision object's size's y
constexpr uint32_t kObjectFloat224 = 0x82002208; // the collision object's +224
constexpr uint32_t kGaugeScale = 0x82002A88;     // the gauges' term

// memset (sub_825A71D0) with 0, as its adapter does it.
void Clear(uint8_t* base, uint32_t at, uint32_t bytes) {
  guest::ForEachHostPart(base, at, bytes,
                         [](uint8_t* host, uint32_t, uint32_t n) { std::memset(host, 0, n); });
}

// Whether every pair removal the collision world could make while its pairs
// grow to their capacity (+32) is native (CollisionWorldRemovePair's
// condition, over the whole array).
bool PairsMovableToCapacity(uint8_t* base) {
  const uint32_t world = World(base);
  const uint32_t pairs = guest::LoadU32(base, world + 36);
  uint32_t count = guest::LoadU32(base, world + 32);
  count = std::max(count, guest::LoadU32(base, world + 40));
  if (count < 2) {
    return true;
  }
  const uint64_t end = uint64_t(pairs) + 32ull * count;
  return end <= 0xA0000000ull && count < 0x08000000u &&
         c_runtime::InOneView(pairs, 32 * count);
}

// What both set-ups check before anything is written.
bool SetUpFits(uint8_t* base) {
  if (!CollisionWorldCallbackKnown(base) || memory::crt::GetProcessHeap(base) == 0 ||
      !PairsMovableToCapacity(base)) {
    return false;
  }
  const uint32_t free_object = guest::LoadU32(base, World(base) + 24);
  return free_object != 0 && co::CollisionShapeDestroyable(base, free_object + 32);
}

// The set-up's steps; `reset` for ResetPhysics (the body kept, the old
// collision object and shape removed, +352 and +384 zeroed).
void SetUp(uint8_t* base, uint32_t vehicle, uint32_t data, uint32_t byte, uint32_t groups,
           uint32_t frame, bool reset, double* f1, uint32_t* r3) {
  const uint32_t v = vehicle, f = frame;
  double reset_f1 = 0.0;
  VehicleReset(base, v, f - 240, &reset_f1);
  Clear(base, v + 416, 1472);
  guest::StoreU32(base, v + 272, data);
  guest::StoreU8(base, v + 2624, 1);
  for (uint32_t w = 0; w < 4; w++) {
    const bool front = w == 0 || w == 1;
    const uint32_t axle_data = guest::LoadU32(base, v + 272) + (front ? 336 : 608);
    guest::StoreU32(base, v + 2160 + 4 * w, axle_data);
    guest::StoreU32(base, v + 2144 + 4 * w, v + (front ? 1888 : 2016));
  }
  uint32_t d = guest::LoadU32(base, v + 272);
  {
    const double axles = Fadds(Lfs(base, d + 672), Lfs(base, d + 400));
    Stfs(base, v + 52, Fmadds(axles, Lfs(base, kMassScale), Lfs(base, d + 0)));
  }
  if (!reset) {
    const uint32_t body = memory::crt::OperatorNew(base, 528).value_or(0);
    if (body != 0) {
      RigidBodyConstruct(base, body, f - 96);
    }
    guest::StoreU32(base, v + 336, body);
  }
  RigidBodyReset(base, guest::LoadU32(base, v + 336), f - 176);
  va::RigidBodySetMass(base, guest::LoadU32(base, v + 336), Lfs(base, v + 52));
  d = guest::LoadU32(base, v + 272);
  RigidBodySetInertia(base, guest::LoadU32(base, v + 336), d + 64, f - 320);
  d = guest::LoadU32(base, v + 272);
  va::RigidBodySetCenterOfMass(base, guest::LoadU32(base, v + 336), d + 96);
  RigidBodySetFloat48(base, guest::LoadU32(base, v + 336), kFloat297C);
  va::RigidBodySetFloat112(base, guest::LoadU32(base, v + 336), Lfs(base, kBodyFloat112));
  // The gravity (0, [0x82002A8C], 0, 0) through the stack.
  CopyWord(base, kZero, f + 80);
  CopyWord(base, kZero, f + 88);
  CopyWord(base, kZero, f + 92);
  CopyWord(base, kGravityY, f + 84);
  va::RigidBodySetGravity(base, guest::LoadU32(base, v + 336), f + 80);
  if (reset) {
    const uint32_t old_object = guest::LoadU32(base, v + 340);
    if (old_object != 0) {
      uint32_t ignored = 0;
      CollisionWorldRemoveObject(base, old_object, &ignored);
    }
    const uint32_t old_shape = guest::LoadU32(base, v + 344);
    guest::StoreU32(base, v + 340, 0);
    if (old_shape != 0) {
      co::ShapeDeletingDestructor(base, old_shape, 1);  // slot 0 (a box's)
      guest::StoreU32(base, v + 344, 0);
    }
  }
  const uint32_t shape = memory::crt::OperatorNew(base, 496).value_or(0);
  if (shape != 0) {
    d = guest::LoadU32(base, v + 272);
    co::BoxConstruct(base, shape, d + 1760, d + 1776, f - 112);
  }
  guest::StoreU32(base, v + 344, shape);
  d = guest::LoadU32(base, v + 272);
  va::Object344SetFloat16(base, guest::LoadU32(base, v + 344), Lfs(base, d + 1792));
  va::Object344SetFloat20(base, guest::LoadU32(base, v + 344), Lfs(base, d + 1796));
  va::Object344SetFloat24(base, guest::LoadU32(base, v + 344), Lfs(base, d + 1792));
  va::Object344SetFloat28(base, guest::LoadU32(base, v + 344), Lfs(base, d + 1796));
  uint32_t object = 0;
  CollisionWorldAddObject(base, guest::LoadU32(base, v + 336), guest::LoadU32(base, v + 344),
                          groups, f - 128, &object);
  // The size (1.0, [0x82002968], 1.0, 0.0) through the stack.
  CopyWord(base, kZero, f + 108);
  guest::StoreU32(base, v + 340, object);
  CopyWord(base, kOne, f + 96);
  CopyWord(base, kOne, f + 104);
  CopyWord(base, kObjectSizeY, f + 100);
  va::CollisionObjectSetSize(base, object, f + 96);
  va::CollisionObjectSetFloat224(base, guest::LoadU32(base, v + 340), Lfs(base, kObjectFloat224));
  {
    uint32_t removed = 0, ignored = 0;
    CollisionObjectRemoveGroups(base, guest::LoadU32(base, v + 340), 0xFFFFFFFFu, &removed,
                                &ignored);
  }
  va::CollisionObjectClearBits256(base, guest::LoadU32(base, v + 340), 0xFFFFFFFFu);
  va::CollisionObjectClearBits260(base, guest::LoadU32(base, v + 340), 0xFFFFFFFFu);
  va::CollisionObjectClearBits252(base, guest::LoadU32(base, v + 340), 0xFFFFFFFFu);
  const uint32_t bits = groups | 0x07000000u;
  CollisionObjectAddGroups(base, guest::LoadU32(base, v + 340), bits);
  va::CollisionObjectSetBits256(base, guest::LoadU32(base, v + 340), bits);
  va::CollisionObjectSetBits260(base, guest::LoadU32(base, v + 340), bits);
  va::CollisionObjectClearBits252(base, guest::LoadU32(base, v + 340), 0x01000000u);
  // The fields cleared, in the original's order.
  CopyWord(base, kZero, v + 280);
  CopyWord(base, kZero, v + 284);
  CopyWord(base, kZero, v + 292);
  guest::StoreU8(base, v + 3132, 0);
  CopyWord(base, kZero, v + 296);
  if (reset) {
    CopyWord(base, kZero, v + 304);
    guest::StoreU8(base, v + 276, 0);
  } else {
    guest::StoreU8(base, v + 276, 0);
    CopyWord(base, kZero, v + 304);
  }
  CopyWord(base, kZero, v + 324);
  if (reset) {
    vl::Vec4Copy(base, v + 352, kZeroVector);
    vl::Vec4Copy(base, v + 384, kZeroVector);
  }
  Clear(base, v + 416, 1472);
  Clear(base, v + 1888, 128);
  Clear(base, v + 2016, 128);
  Clear(base, v + 2204, 144);
  for (uint32_t w = 0; w < 4; w++) {
    CopyWord(base, kOne, WheelAt(v, w) + 228);
    CopyWord(base, kZero, WheelAt(v, w) + 292);
  }
  d = guest::LoadU32(base, v + 272);
  CopyWord(base, kOne, v + 1984);
  CopyWord(base, kOne, v + 2112);
  // The gauges.
  const double one = Lfs(base, kOne), zero = Lfs(base, kZero);
  {
    const double mass = Lfs(base, v + 52);
    const double axles = Fadds(Lfs(base, d + 752), Lfs(base, d + 480));
    const double scale = Lfs(base, kFloat2980);
    const double front = Fmuls(Lfs(base, d + 1836), mass);
    const double axles_mass = Fmuls(axles, mass);
    Stfs(base, v + 2968, Fmuls(front, scale));
    const double rear = Fmuls(Lfs(base, d + 1840), mass);
    const double divisor = Fnmsubs(axles_mass, Lfs(base, kGaugeScale), one);
    Stfs(base, v + 2972, Fmuls(rear, scale));
    double a = Fdivs(Lfs(base, data + 1836), divisor);
    Stfs(base, v + 2976, a);
    double b = Fdivs(Lfs(base, data + 1840), divisor);
    Stfs(base, v + 2980, b);
    a = Fabs(a);
    Stfs(base, v + 2976, a);
    b = Fabs(b);
    Stfs(base, v + 2980, b);
    // Clamped to [0, 1] (bge, ble: a NaN stays).
    if (a < zero) {
      a = zero;
    } else if (a > one) {
      a = one;
    }
    Stfs(base, v + 2976, a);
    if (b < zero) {
      b = zero;
    } else if (b > one) {
      b = one;
    }
    Stfs(base, v + 2980, b);
  }
  guest::StoreU8(base, v + 2249, uint8_t(byte));
  VehicleSetUpGearing(base, v, f - 176);
  VehicleSyncFromBody(base, v, f - 368);
  *r3 = VehicleProbeGround(base, v, f - 304, f1);
  // The drive and the torque split.
  guest::StoreU32(base, v + 2376, guest::LoadU32(base, data + 172));
  CopyWord(base, data + 180, v + 1988);
  Stfs(base, v + 2116, Fsubs(one, Lfs(base, data + 180)));
  {
    const double split = Fsubs(one, Lfs(base, data + 1832));
    Stfs(base, v + 2936, Fmadds(split, Lfs(base, kHalf), one));
    const double split2 = Fsubs(one, Lfs(base, data + 1832));
    Stfs(base, v + 2940, Fnmsubs(split2, Lfs(base, kObjectSizeY), one));
  }
  for (uint32_t w = 0; w < 4; w++) {
    guest::StoreU32(base, WheelAt(v, w) + 204, 0);
  }
  d = guest::LoadU32(base, v + 272);
  CopyWord(base, d + 140, v + 2000);
  CopyWord(base, d + 144, v + 2128);
  if (int32_t(guest::LoadU32(base, d + 1268)) == 1) {
    // One gear: the gear values as Vehicle::SetGearValues sets them.
    const double ratio = Lfs(base, d + 1568);
    CopyWord(base, d + 1568, v + 2288);
    const double torque = Fmuls(Lfs(base, d + 1600), ratio);
    const double a = Lfs(base, v + 2328);
    const double sum = Fadds(Lfs(base, v + 2336), a);
    CopyWord(base, kZero, v + 2320);
    CopyWord(base, v + 2328, v + 2324);
    Stfs(base, v + 2312, sum);
    Stfs(base, v + 2304, Fabs(torque));
    Stfs(base, v + 2308, Fmuls(Lfs(base, d + 1544), Lfs(base, d + 1504)));
  }
}

}  // namespace

bool VehicleSetUpPhysics(uint8_t* base, uint32_t vehicle, uint32_t data, uint32_t byte,
                         uint32_t groups, uint32_t frame, double* f1, uint32_t* r3) {
  if (!SetUpFits(base)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B5670 Vehicle::SetUpPhysics");
  SetUp(base, vehicle, data, byte, groups, frame, false, f1, r3);
  return true;
}

bool VehicleResetPhysics(uint8_t* base, uint32_t vehicle, uint32_t data, uint32_t groups,
                         uint32_t frame, double* f1, uint32_t* r3) {
  const uint32_t old_shape = guest::LoadU32(base, vehicle + 344);
  if (!SetUpFits(base) ||
      (old_shape != 0 && (!co::IsBox(base, old_shape) || !co::Deletable(old_shape)))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B5B08 Vehicle::ResetPhysics");
  SetUp(base, vehicle, data, 1, groups, frame, true, f1, r3);
  return true;
}

}  // namespace car_physics
