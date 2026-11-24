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
#include "schema/car_physics.h"
#include "schema/collision.h"
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

constexpr uint32_t kGroundCallback = 0x820FEAB0;  // TrackCollision::GroundCallback

uint32_t World(uint8_t* base) {
  return schema::CarPhysicsGlobals::collision_world(base).address;
}

// Whether CollisionShape::Place runs natively on the record twice (the
// copies swapped between): the template and both copies boxes, and with
// the template lowered (+112 not 0.0) its stack copy native (the same
// place both times).
bool PlaceTwiceFits(uint8_t* base, uint32_t record, uint32_t place_frame) {
  const schema::CollisionShape shape(base, record);
  const uint32_t source = shape.template_shape().address;
  if (!co::IsBox(base, source) || !co::IsBox(base, shape.previous().address) ||
      !co::IsBox(base, shape.current().address)) {
    return false;
  }
  const bool lowered = Lfs(base, shape.bottom_lowering_address()) != Lfs(base, kZero);
  return !lowered || co::BoxCopyFits(place_frame + 80, source);
}

}  // namespace

void RigidBodySetFloat48(uint8_t* base, uint32_t body, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_8259D5A8 RigidBody::SetFloat48");
  CopyWord(base, from, schema::RigidBody(base, body).speed_cap_address());
}

bool CollisionWorldCallbackKnown(uint8_t* base) {
  return schema::CollisionWorld(base, World(base)).ground_callback() == kGroundCallback;
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
  const uint32_t pairs = schema::CollisionWorld(base, world).pairs().address;
  const uint32_t count = schema::CollisionWorld(base, world).pair_count();
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
  schema::CollisionWorldObject(base, object).set_moved(uint32_t(1));
  co::CollisionShapePlace(base, record,
                          va::RigidBodyMatrix(schema::CollisionWorldObject(base, object).body().address), place_frame);
  co::CollisionShapePlace(base, record,
                          va::RigidBodyMatrix(schema::CollisionWorldObject(base, object).body().address), place_frame);
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
  const schema::CollisionWorldObject obj(base, o);
  obj.set_body(body);
  obj.set_shape(shape);
  obj.set_groups(groups);
  co::CollisionShapeSetUp(base, obj.shape_record_address(), shape, va::RigidBodyMatrix(body), memory, frame - 128);
  obj.set_collides_with(0xFFFFFFFFu);
  obj.set_mask(0xFFFFFFFFu);
  obj.set_response_bits(0xFFFFFFFFu);
  obj.set_push_out_bits(0xFFFFFFFFu);
  vl::Vec4Copy(base, obj.push_address(), kZeroVector);
  vl::Vec4Copy(base, obj.pair_torque_address(), kZeroVector);
  obj.set_push_count(uint32_t(0));
  obj.set_first_contact(uint32_t(0));
  obj.set_unknown_236(uint32_t(0));
  // +208 = (1, 1, 1) with its w loaded first and stored back (lfs then
  // stfs: its bits).
  const uint32_t w = guest::LoadU32(base, obj.push_scale_address() + 12);
  CopyWord(base, kOne, obj.push_scale_address());
  CopyWord(base, kOne, obj.push_scale_address() + 4);
  CopyWord(base, kOne, obj.push_scale_address() + 8);
  guest::StoreU32(base, obj.push_scale_address() + 12, w);
  obj.set_owner_id(uint32_t(0));
  CopyWord(base, kOne, obj.torque_scale_address());
  obj.set_no_queries(uint32_t(0));
  obj.set_unknown_276(uint32_t(0));
  obj.set_box_friction(uint32_t(0));
  obj.set_unmoved(uint32_t(0));
  obj.set_moved(uint32_t(0));
  obj.set_first_pair(uint32_t(0));
  vl::Vec4Copy(base, obj.push_out_address(), kZeroVector);
}

bool CollisionWorldAddObject(uint8_t* base, uint32_t body, uint32_t shape, uint32_t groups,
                             uint32_t frame, uint32_t* object) {
  const uint32_t free_object = schema::CollisionWorld(base, World(base)).free().address;
  if (free_object == 0 ||
      !CollisionObjectInitFits(base, free_object, shape, schema::CollisionWorld(base, World(base)).buffer())) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B9570 CollisionWorld::AddObject");
  va::CollisionObjectUnlink(base, free_object, schema::CollisionWorld(base, World(base)).address + 24);  // the free list's head
  va::CollisionObjectLink(base, free_object, schema::CollisionWorld(base, World(base)).address + 20);  // the active list's head
  uint32_t world = World(base);
  schema::CollisionWorld(base, world).set_active_count(uint32_t(schema::CollisionWorld(base, world).active_count() + 1));
  world = World(base);
  CollisionObjectInit(base, free_object, body, shape, groups, schema::CollisionWorld(base, world).buffer(),
                      frame - 112);
  CollisionWorldAddPairs(base, free_object, schema::CollisionWorldObject(base, free_object).collides_with());
  *object = free_object;
  return true;
}

bool VehicleDestroyPhysics(uint8_t* base, uint32_t vehicle, uint32_t* r3) {
  const uint32_t object = schema::Vehicle(base, vehicle).collision_object();
  const uint32_t shape = schema::Vehicle(base, vehicle).box();
  const uint32_t body = schema::Vehicle(base, vehicle).body().address;
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
    schema::Vehicle(base, vehicle).set_collision_object(uint32_t(0));
  }
  // r3: the +344 loaded (slot 0 returns the shape), then the body's.
  *r3 = shape;
  if (shape != 0) {
    co::ShapeDeletingDestructor(base, shape, 1);
    schema::Vehicle(base, vehicle).set_box(uint32_t(0));
  }
  if (body != 0) {
    c_runtime::EmptyDestructor();
    memory::crt::OperatorDelete(body);
    schema::Vehicle(base, vehicle).set_body(uint32_t(0));
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
  const uint32_t up = schema::Vehicle(base, v).ground_up_address();
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
    CollisionWorldCallback(base, f + 176, schema::VehicleWheel(base, wheel).position_address(), schema::VehicleWheel(base, wheel).track_position(), f, &r3);
    schema::VehicleWheel(base, wheel).set_ground_distance(float(Fmuls(Lfs(base, f + 196), Lfs(base, f + 176))));
    vl::Vec4Copy(base, schema::VehicleWheel(base, wheel).ground_normal_address(), f + 192);
    const uint32_t next = guest::LoadU32(base, f + 228);
    const uint32_t surface = guest::LoadU32(base, f + 224);
    schema::VehicleWheel(base, wheel).set_track_position(uint32_t(next));
    schema::VehicleWheel(base, wheel).set_surface(uint32_t(surface));
    const double offset = VehicleSurfaceValue(base, v, w);
    schema::VehicleWheel(base, wheel).set_ground_distance(float(Fadds(double(schema::VehicleWheel(base, wheel).ground_distance()), offset)));
    vl::Vec4Copy(base, schema::VehicleWheel(base, wheel).contact_point_address(), schema::VehicleWheel(base, wheel).mount_point_address());
    Stfs(base, schema::VehicleWheel(base, wheel).contact_point_address() + 4, Fsubs(Lfs(base, schema::VehicleWheel(base, wheel).contact_point_address() + 4), double(schema::VehicleWheel(base, wheel).ground_distance())));
    vl::Vec3AddTo(base, up, schema::VehicleWheel(base, wheel).ground_normal_address());
  }
  // The axles' midpoints.
  const double half = Lfs(base, kHalf);
  vl::Vec3Sum(base, f + 160, schema::VehicleWheel(base, schema::Vehicle(base, v).wheels_address()).contact_point_address(), schema::VehicleWheel(base, schema::Vehicle(base, v).wheels_address() + 368).contact_point_address());
  vl::Vec4Scaled(base, f + 80, f + 160, half);
  vl::Vec4Copy(base, schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).contacts_midpoint_address(), f + 80);
  vl::Vec3Sum(base, f + 80, schema::VehicleWheel(base, schema::Vehicle(base, v).wheels_address() + 736).contact_point_address(), schema::VehicleWheel(base, schema::Vehicle(base, v).wheels_address() + 1104).contact_point_address());
  vl::Vec4Scaled(base, f + 160, f + 80, half);
  vl::Vec4Copy(base, schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).contacts_midpoint_address(), f + 160);
  // The ground frame.
  vl::Vec3NormalizeSelf(base, up);
  vl::Vec3Cross(base, f + 80, up, schema::Vehicle(base, v).body_row2_address());
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_forward_address(), f + 80);
  vl::Vec3NormalizeSelf(base, schema::Vehicle(base, v).ground_forward_address());
  vl::Vec3Cross(base, f + 80, schema::Vehicle(base, v).ground_forward_address(), up);
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_side_address(), f + 80);
  vl::Vec3NormalizeSelf(base, schema::Vehicle(base, v).ground_side_address());
  // The rows with w 1.0 through the stack (the x, y, w, z stores' order is
  // the original's; nothing overlaps).
  CopyXyz(base, schema::Vehicle(base, v).ground_forward_address(), f + 112, kOne);
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_matrix_address(), f + 112);
  CopyXyz(base, up, f + 128, kOne);
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_matrix_address() + 16, f + 128);
  CopyXyz(base, schema::Vehicle(base, v).ground_side_address(), f + 144, kOne);
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_matrix_address() + 32, f + 144);
  // The slip term: the velocity copied (ld, std: its bits), +92 = 0.0 first.
  const uint32_t velocity = va::RigidBodyVelocity(schema::Vehicle(base, v).body().address);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 96 + i, guest::LoadU32(base, velocity + i));
  }
  const double zero = Lfs(base, kZero);
  schema::Vehicle(base, v).set_slip_term(float(zero));
  double speed2 = double(vl::Vec3Dot(base, f + 96, f + 96));
  const double limit = Lfs(base, kSpeedLimit);
  if (speed2 > limit) {  // ble ("not greater"): a NaN stays
    speed2 = limit;
  }
  const double forward = double(vl::Vec3DotOf(base, f + 96, schema::Vehicle(base, v).ground_forward_address()));
  const double side = double(vl::Vec3DotOf(base, f + 96, schema::Vehicle(base, v).ground_side_address()));
  const double forward_size = Fabs(forward);
  const double bound = Fmuls(forward_size, Lfs(base, kSlipRatio));
  double ratio;
  if (Fabs(side) < bound) {
    ratio = Fdivs(side, forward_size);
  } else {
    ratio = double(float(c_runtime::Atan2(side, forward_size)));
  }
  schema::Vehicle(base, v).set_slip_term(float(Fmuls(Fmuls(ratio, speed2), Lfs(base, kSlipScale))));
  schema::Vehicle(base, v).set_yaw_rate(float(double(vl::Vec3DotOf(base, schema::Vehicle(base, v).angular_velocity_address(), up))));
  va::RigidBodyPointVelocity(base, f + 80, schema::Vehicle(base, v).body().address, schema::Vehicle(base, v).front_axle_address());
  vl::Vec4Copy(base, schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).velocity_address(), f + 80);
  va::RigidBodyPointVelocity(base, f + 80, schema::Vehicle(base, v).body().address, schema::Vehicle(base, v).rear_axle_address());
  vl::Vec4Copy(base, schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).velocity_address(), f + 80);
  matrix_math::InverseRotate(base, f + 80, schema::Vehicle(base, v).ground_matrix_address(),
                             va::RigidBodyVelocity(schema::Vehicle(base, v).body().address));
  vl::Vec4Copy(base, schema::Vehicle(base, v).ground_velocity_address(), f + 80);
  vl::Vec3Cross(base, f + 160, up, schema::Vehicle(base, v).body_row1_address());
  const double roll = double(vl::Vec3DotOf(base, f + 160, schema::Vehicle(base, v).ground_forward_address()));
  schema::Vehicle(base, v).set_roll(float(roll));
  *f1 = roll;
  return f + 160;
}

bool VehicleUnflip(uint8_t* base, uint32_t vehicle, uint32_t frame, uint32_t* r3) {
  const uint32_t v = vehicle, f = frame;
  if (!(double(schema::Vehicle(base, v).speed()) < Lfs(base, kUnflipSpeed))) {  // bge: NaNs return
    ITC_NATIVE_RUNNING("sub_820AFE90 Vehicle::Unflip");
    *r3 = v;
    return true;
  }
  const double tilt = double(vl::Vec3DotOf(base, schema::Vehicle(base, v).body_row1_address(), schema::Vehicle(base, v).ground_up_address()));
  if (!(tilt < Lfs(base, kUnflipUp))) {
    ITC_NATIVE_RUNNING("sub_820AFE90 Vehicle::Unflip");
    *r3 = schema::Vehicle(base, v).body_row1_address();  // Vec3::Dot leaves r3 as its first vector
    return true;
  }
  if (!CollisionWorldCallbackKnown(base)) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AFE90 Vehicle::Unflip");
  CopyWord(base, kZero, f + 316);
  CopyWord(base, kZero, f + 332);
  uint32_t ignored = 0;
  CollisionWorldCallback(base, f + 288, va::ContactPosition(schema::Vehicle(base, v).body().address),
                         schema::Vehicle(base, v).track_position(), f, &ignored);
  matrix_math::SetIdentity(base, f + 192);
  // Row 1: the normal (w 1.0).
  CopyXyz(base, f + 304, f + 96, kOne);
  vl::Vec4Copy(base, f + 208, f + 96);
  // Row 2: normalized(row 1 x +2528), through the stack (w 0.0, then 1.0).
  CopyXyz(base, f + 208, f + 160, kZero);
  vl::Vec3Cross(base, f + 256, f + 160, schema::Vehicle(base, v).axis_address());
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
  const uint32_t position = va::ContactPosition(schema::Vehicle(base, v).body().address);
  const uint32_t data = schema::Vehicle(base, v).car_data().address;
  const double distance = Lfs(base, f + 288);
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, f + 176 + i, guest::LoadU32(base, position + i));
  }
  Stfs(base, f + 180, Fsubs(Lfs(base, f + 180), distance));
  va::RigidBodySetCenterOfMass(base, schema::Vehicle(base, v).body().address, schema::CarData(base, data).center_of_mass_ground_address());
  RigidBodySetCenterOfMassPosition(base, schema::Vehicle(base, v).body().address, f + 176, f - 96);
  const uint32_t body = schema::Vehicle(base, v).body().address;
  RigidBodySetOrientation(base, body, f + 272, f - 96);
  schema::Vehicle(base, v).set_upright(uint8_t(0));
  *r3 = schema::RigidBody(base, body).matrix_address() + 48;  // SetOrientation's (UpdateOrigin's)
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
  schema::Vehicle(base, v).set_unknown_2456(uint8_t(1));
  VehicleSyncFromBody(base, v, f - 368);
  VehicleProbeGround(base, v, f - 304, &f1);
  VehicleUpdateSuspension(base, v, f - 144, &f1);
  // The smallest of the wheels' +0 and the limit (ble, "not greater": a
  // NaN on either side keeps the one so far).
  double travel = Lfs(base, kTravelLimit);
  for (uint32_t w = 0; w < 4; w++) {
    const double wheel_travel = double(schema::VehicleWheel(base, WheelAt(v, w)).travel());
    if (travel > wheel_travel) {
      travel = wheel_travel;
    }
  }
  Stfs(base, schema::Vehicle(base, v).collision_object() + 144, Fneg(travel));
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
    CopyWord(base, kZero, schema::VehicleWheel(base, WheelAt(v, w)).force_integral_address());
    CopyWord(base, kZero, schema::VehicleWheel(base, WheelAt(v, w)).force_integral_2_address());
  }
  for (uint32_t w = 0; w < 4; w++) {
    schema::VehicleWheel(base, WheelAt(v, w)).set_abs_count(uint32_t(0));
  }
  const uint32_t data = schema::Vehicle(base, v).car_data().address;
  CopyWord(base, schema::CarData(base, data).abs_front_address(), schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).abs_threshold_address());
  CopyWord(base, schema::CarData(base, data).abs_rear_address(), schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).abs_threshold_address());
  // The substeps (the count read again each time, cmplw: unsigned).
  for (uint32_t i = 0; i < schema::Vehicle(base, v).substep_count(); i++) {
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
  CopyWord(base, schema::Vehicle(base, v).throttle_used_address(), schema::Vehicle(base, v).throttle_used_previous_address());
  schema::Vehicle(base, v).set_unknown_2444(uint8_t(0));
  schema::Vehicle(base, v).set_unknown_2445(uint8_t(0));
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
  const uint32_t pairs = schema::CollisionWorld(base, world).pairs().address;
  uint32_t count = schema::CollisionWorld(base, world).pair_capacity();
  count = std::max(count, schema::CollisionWorld(base, world).pair_count());
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
  const uint32_t free_object = schema::CollisionWorld(base, World(base)).free().address;
  return free_object != 0 && co::CollisionShapeDestroyable(base, free_object + 32);
}

// The set-up's steps; `reset` for ResetPhysics (the body kept, the old
// collision object and shape removed, +352 and +384 zeroed).
void SetUp(uint8_t* base, uint32_t vehicle, uint32_t data, uint32_t byte, uint32_t groups,
           uint32_t frame, bool reset, double* f1, uint32_t* r3) {
  const uint32_t v = vehicle, f = frame;
  double reset_f1 = 0.0;
  VehicleReset(base, v, f - 240, &reset_f1);
  Clear(base, schema::Vehicle(base, v).wheels_address(), 1472);
  schema::Vehicle(base, v).set_car_data(uint32_t(data));
  schema::Vehicle(base, v).set_unknown_2624(uint8_t(1));
  for (uint32_t w = 0; w < 4; w++) {
    const bool front = w == 0 || w == 1;
    const uint32_t axle_data = schema::Vehicle(base, v).car_data().address + (front ? 336 : 608);
    guest::StoreU32(base, schema::Vehicle(base, v).wheel_axle_data_address() + 4 * w, axle_data);
    guest::StoreU32(base, schema::Vehicle(base, v).wheel_axles_address() + 4 * w, v + (front ? 1888 : 2016));
  }
  uint32_t d = schema::Vehicle(base, v).car_data().address;
  {
    const double axles = Fadds(double(schema::CarDataAxle(base, schema::CarData(base, d).rear_axle_address()).wheel_mass()), double(schema::CarDataAxle(base, schema::CarData(base, d).front_axle_address()).wheel_mass()));
    schema::Vehicle(base, v).set_unknown_52(float(Fmadds(axles, Lfs(base, kMassScale), double(schema::CarData(base, d).mass()))));
  }
  if (!reset) {
    // Host memory (E5 step 4): the 528-byte body operator new gave.
    const uint32_t body = schema::RigidBody::New(base).address;
    if (body != 0) {
      RigidBodyConstruct(base, body, f - 96);
    }
    schema::Vehicle(base, v).set_body(uint32_t(body));
  }
  RigidBodyReset(base, schema::Vehicle(base, v).body().address, f - 176);
  va::RigidBodySetMass(base, schema::Vehicle(base, v).body().address, double(schema::Vehicle(base, v).unknown_52()));
  d = schema::Vehicle(base, v).car_data().address;
  RigidBodySetInertia(base, schema::Vehicle(base, v).body().address, schema::CarData(base, d).inertia_address(), f - 320);
  d = schema::Vehicle(base, v).car_data().address;
  va::RigidBodySetCenterOfMass(base, schema::Vehicle(base, v).body().address, schema::CarData(base, d).center_of_mass_ground_address());
  RigidBodySetFloat48(base, schema::Vehicle(base, v).body().address, kFloat297C);
  va::RigidBodySetFloat112(base, schema::Vehicle(base, v).body().address, Lfs(base, kBodyFloat112));
  // The gravity (0, [0x82002A8C], 0, 0) through the stack.
  CopyWord(base, kZero, f + 80);
  CopyWord(base, kZero, f + 88);
  CopyWord(base, kZero, f + 92);
  CopyWord(base, kGravityY, f + 84);
  va::RigidBodySetGravity(base, schema::Vehicle(base, v).body().address, f + 80);
  if (reset) {
    const uint32_t old_object = schema::Vehicle(base, v).collision_object();
    if (old_object != 0) {
      uint32_t ignored = 0;
      CollisionWorldRemoveObject(base, old_object, &ignored);
    }
    const uint32_t old_shape = schema::Vehicle(base, v).box();
    schema::Vehicle(base, v).set_collision_object(uint32_t(0));
    if (old_shape != 0) {
      co::ShapeDeletingDestructor(base, old_shape, 1);  // slot 0 (a box's)
      schema::Vehicle(base, v).set_box(uint32_t(0));
    }
  }
  // Host memory (E5 step 4): the 496-byte box operator new gave.
  const uint32_t shape = schema::BoxShape::New(base).address;
  if (shape != 0) {
    d = schema::Vehicle(base, v).car_data().address;
    co::BoxConstruct(base, shape, schema::CarData(base, d).box_1760_address(), schema::CarData(base, d).box_1776_address(), f - 112);
  }
  schema::Vehicle(base, v).set_box(uint32_t(shape));
  d = schema::Vehicle(base, v).car_data().address;
  va::Object344SetFloat16(base, schema::Vehicle(base, v).box(), double(schema::CarData(base, d).box_value_1792()));
  va::Object344SetFloat20(base, schema::Vehicle(base, v).box(), double(schema::CarData(base, d).box_value_1796()));
  va::Object344SetFloat24(base, schema::Vehicle(base, v).box(), double(schema::CarData(base, d).box_value_1792()));
  va::Object344SetFloat28(base, schema::Vehicle(base, v).box(), double(schema::CarData(base, d).box_value_1796()));
  uint32_t object = 0;
  CollisionWorldAddObject(base, schema::Vehicle(base, v).body().address, schema::Vehicle(base, v).box(),
                          groups, f - 128, &object);
  // The size (1.0, [0x82002968], 1.0, 0.0) through the stack.
  CopyWord(base, kZero, f + 108);
  schema::Vehicle(base, v).set_collision_object(uint32_t(object));
  CopyWord(base, kOne, f + 96);
  CopyWord(base, kOne, f + 104);
  CopyWord(base, kObjectSizeY, f + 100);
  va::CollisionObjectSetSize(base, object, f + 96);
  va::CollisionObjectSetFloat224(base, schema::Vehicle(base, v).collision_object(), Lfs(base, kObjectFloat224));
  {
    uint32_t removed = 0, ignored = 0;
    CollisionObjectRemoveGroups(base, schema::Vehicle(base, v).collision_object(), 0xFFFFFFFFu, &removed,
                                &ignored);
  }
  va::CollisionObjectClearBits256(base, schema::Vehicle(base, v).collision_object(), 0xFFFFFFFFu);
  va::CollisionObjectClearBits260(base, schema::Vehicle(base, v).collision_object(), 0xFFFFFFFFu);
  va::CollisionObjectClearBits252(base, schema::Vehicle(base, v).collision_object(), 0xFFFFFFFFu);
  const uint32_t bits = groups | 0x07000000u;
  CollisionObjectAddGroups(base, schema::Vehicle(base, v).collision_object(), bits);
  va::CollisionObjectSetBits256(base, schema::Vehicle(base, v).collision_object(), bits);
  va::CollisionObjectSetBits260(base, schema::Vehicle(base, v).collision_object(), bits);
  va::CollisionObjectClearBits252(base, schema::Vehicle(base, v).collision_object(), 0x01000000u);
  // The fields cleared, in the original's order.
  CopyWord(base, kZero, schema::Vehicle(base, v).steering_input_address());
  CopyWord(base, kZero, schema::Vehicle(base, v).throttle_address());
  CopyWord(base, kZero, schema::Vehicle(base, v).brake_address());
  schema::Vehicle(base, v).set_upright(uint8_t(0));
  CopyWord(base, kZero, schema::Vehicle(base, v).handbrake_address());
  if (reset) {
    CopyWord(base, kZero, schema::Vehicle(base, v).steering_address());
    schema::Vehicle(base, v).set_speed_sensitive_steering(uint8_t(0));
  } else {
    schema::Vehicle(base, v).set_speed_sensitive_steering(uint8_t(0));
    CopyWord(base, kZero, schema::Vehicle(base, v).steering_address());
  }
  CopyWord(base, kZero, schema::Vehicle(base, v).steering_smoothed_address());
  if (reset) {
    vl::Vec4Copy(base, schema::Vehicle(base, v).unknown_352_address(), kZeroVector);
    vl::Vec4Copy(base, schema::Vehicle(base, v).ground_velocity_address(), kZeroVector);
  }
  Clear(base, schema::Vehicle(base, v).wheels_address(), 1472);
  Clear(base, schema::Vehicle(base, v).front_axle_address(), 128);
  Clear(base, schema::Vehicle(base, v).rear_axle_address(), 128);
  Clear(base, schema::Vehicle(base, v).unknown_2204_address(), 144);
  for (uint32_t w = 0; w < 4; w++) {
    CopyWord(base, kOne, schema::VehicleWheel(base, WheelAt(v, w)).unknown_228_address());
    CopyWord(base, kZero, schema::VehicleWheel(base, WheelAt(v, w)).tire_wear_address());
  }
  d = schema::Vehicle(base, v).car_data().address;
  CopyWord(base, kOne, schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).grip_address());
  CopyWord(base, kOne, schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).grip_address());
  // The gauges.
  const double one = Lfs(base, kOne), zero = Lfs(base, kZero);
  {
    const double mass = double(schema::Vehicle(base, v).unknown_52());
    const double axles = Fadds(double(schema::CarDataAxle(base, schema::CarData(base, d).rear_axle_address()).unknown_144()), double(schema::CarDataAxle(base, schema::CarData(base, d).front_axle_address()).unknown_144()));
    const double scale = Lfs(base, kFloat2980);
    const double front = Fmuls(double(schema::CarData(base, d).front_mass_share()), mass);
    const double axles_mass = Fmuls(axles, mass);
    schema::Vehicle(base, v).set_gauge_0(float(Fmuls(front, scale)));
    const double rear = Fmuls(double(schema::CarData(base, d).rear_mass_share()), mass);
    const double divisor = Fnmsubs(axles_mass, Lfs(base, kGaugeScale), one);
    schema::Vehicle(base, v).set_gauge_1(float(Fmuls(rear, scale)));
    double a = Fdivs(double(schema::CarData(base, data).front_mass_share()), divisor);
    schema::Vehicle(base, v).set_gauge_2(float(a));
    double b = Fdivs(double(schema::CarData(base, data).rear_mass_share()), divisor);
    schema::Vehicle(base, v).set_gauge_3(float(b));
    a = Fabs(a);
    schema::Vehicle(base, v).set_gauge_2(float(a));
    b = Fabs(b);
    schema::Vehicle(base, v).set_gauge_3(float(b));
    // Clamped to [0, 1] (bge, ble: a NaN stays).
    if (a < zero) {
      a = zero;
    } else if (a > one) {
      a = one;
    }
    schema::Vehicle(base, v).set_gauge_2(float(a));
    if (b < zero) {
      b = zero;
    } else if (b > one) {
      b = one;
    }
    schema::Vehicle(base, v).set_gauge_3(float(b));
  }
  schema::Vehicle(base, v).set_automatic(uint8_t(uint8_t(byte)));
  VehicleSetUpGearing(base, v, f - 176);
  VehicleSyncFromBody(base, v, f - 368);
  *r3 = VehicleProbeGround(base, v, f - 304, f1);
  // The drive and the torque split.
  schema::Vehicle(base, v).set_drive_type(uint32_t(schema::CarData(base, data).drive_type()));
  CopyWord(base, schema::CarData(base, data).front_torque_share_address(), schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).torque_share_address());
  schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).set_torque_share(float(Fsubs(one, double(schema::CarData(base, data).front_torque_share()))));
  {
    const double split = Fsubs(one, double(schema::CarData(base, data).unknown_1832()));
    schema::Vehicle(base, v).set_unknown_2936(float(Fmadds(split, Lfs(base, kHalf), one)));
    const double split2 = Fsubs(one, double(schema::CarData(base, data).unknown_1832()));
    schema::Vehicle(base, v).set_unknown_2940(float(Fnmsubs(split2, Lfs(base, kObjectSizeY), one)));
  }
  for (uint32_t w = 0; w < 4; w++) {
    schema::VehicleWheel(base, WheelAt(v, w)).set_abs_count(uint32_t(0));
  }
  d = schema::Vehicle(base, v).car_data().address;
  CopyWord(base, schema::CarData(base, d).abs_front_address(), schema::VehicleAxle(base, schema::Vehicle(base, v).front_axle_address()).abs_threshold_address());
  CopyWord(base, schema::CarData(base, d).abs_rear_address(), schema::VehicleAxle(base, schema::Vehicle(base, v).rear_axle_address()).abs_threshold_address());
  if (int32_t(schema::CarData(base, d).gear_count()) == 1) {
    // One gear: the gear values as Vehicle::SetGearValues sets them.
    const double ratio = Lfs(base, schema::CarData(base, d).gear_ratios_copy_address() + 4);
    CopyWord(base, schema::CarData(base, d).gear_ratios_copy_address() + 4, schema::Vehicle(base, v).cvt_ratio_address());
    const double torque = Fmuls(double(schema::CarData(base, d).final_drive_copy()), ratio);
    const double a = double(schema::Vehicle(base, v).unknown_2328());
    const double sum = Fadds(double(schema::Vehicle(base, v).axle_radius()), a);
    CopyWord(base, kZero, schema::Vehicle(base, v).unknown_2320_address());
    CopyWord(base, schema::Vehicle(base, v).unknown_2328_address(), schema::Vehicle(base, v).unknown_2324_address());
    schema::Vehicle(base, v).set_unknown_2312(float(sum));
    schema::Vehicle(base, v).set_total_ratio(float(Fabs(torque)));
    schema::Vehicle(base, v).set_gear_value(float(Fmuls(double(schema::CarData(base, d).unknown_1544()), Lfs(base, schema::CarData(base, d).gear_table_3_address() + 4))));
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
  const uint32_t old_shape = schema::Vehicle(base, vehicle).box();
  if (!SetUpFits(base) ||
      (old_shape != 0 && (!co::IsBox(base, old_shape) || !co::Deletable(old_shape)))) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820B5B08 Vehicle::ResetPhysics");
  SetUp(base, vehicle, data, 1, groups, frame, true, f1, r3);
  return true;
}

}  // namespace car_physics
