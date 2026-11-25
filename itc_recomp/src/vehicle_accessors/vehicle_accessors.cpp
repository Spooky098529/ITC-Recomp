#include "vehicle_accessors/vehicle_accessors.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "schema/vehicle_accessors.h"
#include "schema/car_draw.h"
#include "schema/car_physics.h"
#include "schema/collision.h"
#include "schema/collision_world.h"
#include "schema/race_hud_machine.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/scene_loaders.h"
#include "vehicle_accessors/vectors.h"

#include <bit>
#include <cmath>

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

namespace vehicle_accessors {

namespace {

using G = schema::VehicleAccessorsGlobals;
using CollisionObject = schema::CollisionObject;
using Contact = schema::CollisionContact;
using Stages = schema::Stages;
using RaceStage = schema::RaceStage;
using RacerRecord = schema::Car;
using RacerRecordArray = schema::RacerRecordArray;
using List = schema::AccessorList;
using ListNode = schema::AccessorListNode;
using Box = schema::BoxShape;
using Pair = schema::AccessorPair;
using Matrix = schema::VectorLibMatrix;
using VectorView = schema::VectorLibVector;
using Body = schema::RigidBody;
using Vehicle = schema::Vehicle;
using Wheel = schema::VehicleWheel;
using Axle = schema::CarDataAxle;
using World = schema::CollisionWorld;

constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kRecordBytes = RacerRecord::kSize;

// The library's memcpy natively, where that's what it would do and its
// override has nothing to record (see Stages::Copy).
bool CopyLikeMemcpy(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && c_runtime::Memcpy(destination, source, size);
}

// The stage `stage` (-1 for the current, +804) of the stage list.
uint32_t Stage(uint32_t stages, uint32_t stage) {
  if (int32_t(stage) == -1) {
    stage = Stages(stages).current();
  }
  return stages + RaceStage::kSize * stage;
}

namespace vl = vector_library;

// The rotation of `v` by the matrix at `m` (Matrix::Rotate's arithmetic),
// w 0.0.
vl::Vector Rotate(uint32_t m, const vl::Vector& v) {
  const Matrix matrix(m);
  auto muls = [](double a, double b) { return double(float(a * b)); };
  auto madds = [](double a, double b, double c) { return double(float(std::fma(a, b, c))); };
  const double x0 = madds(double(matrix.m10()), v.y, muls(double(matrix.m20()), v.z));
  const double y0 = madds(double(matrix.m01()), v.x, muls(double(matrix.m11()), v.y));
  const double z0 = madds(double(matrix.m02()), v.x, muls(double(matrix.m12()), v.y));
  return {float(madds(v.x, double(matrix.m00()), x0)), float(madds(double(matrix.m21()), v.z, y0)),
          float(madds(double(matrix.m22()), v.z, z0)), 0.0f};
}

// RigidBody::PointVelocity's value: +32 + +96 x (point - +16).
vl::Vector PointVelocityCross(uint32_t body, uint32_t point) {
  const Body b(body);
  const vl::Vector d = vl::Sub(vl::LoadVector(point), vl::LoadVector(b.center_of_mass_address()));
  return vl::Cross(vl::LoadVector(b.angular_velocity_address()), d);
}

}  // namespace

uint32_t VehicleGet(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C0468 Vehicle::Get");
  const uint32_t vehicles = G::vehicles();
  if (vehicles == 0) {
    return 0;
  }
  return vehicles + kVehicleBytes * index;
}

uint32_t VehicleWheelContact(uint32_t vehicle, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_820C0170 Vehicle::WheelContact");
  return Vehicle(nullptr, vehicle).wheel_track_positions_address() + schema::TrackPosition::kSize * wheel;
}

uint32_t RigidBodyPosition(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B67D8 RigidBody::Position");
  return Body(nullptr, body).origin_address();
}

uint32_t RigidBodyMatrix(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6930 RigidBody::Matrix");
  return Body(nullptr, body).matrix_address();
}

uint32_t ContactObject(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8CC0 Contact::Object");
  const Contact c(contact);
  if (c.flags() & 0xFF000000u) {
    return 0;
  }
  return c.other();
}

uint32_t ContactPosition(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D00 Contact::Position");
  return Contact(nullptr, contact).position_address();
}

float ContactStrength(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D08 Contact::Strength");
  return Contact(contact).strength();
}

int32_t RacersIndexOf(uint32_t records, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820FF9F8 Racers::IndexOf");
  for (uint32_t i = 0; i < 93; i++) {
    if (RacerRecord(records + kRecordBytes * i).item_id() == id) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StagesHasRacer(uint32_t stages, uint32_t racer, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFCF8 Stages::HasRacer");
  if (int32_t(stage) == -1) {
    stage = Stages(stages).current();
  }
  const uint32_t slots = RaceStage(stages + RaceStage::kSize * stage).racer0_address();
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t slot = guest::LoadU32(slots + 8 * i);
    if (int32_t(slot) == -1) {
      return 0;
    }
    if (slot == racer) {
      return 1;
    }
  }
  return 0;
}

uint32_t SaveDataOf(uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82105620 SaveData::Of");
  return guest::LoadU32(G::save_data_address() + 4 * player);
}

uint32_t DebugSwitchOff() {
  ITC_NATIVE_RUNNING("sub_825C8388 DebugSwitchOff");
  return 0;
}

uint32_t RigidBodyIsFixed(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6638 RigidBody::IsFixed");
  return Body(body).fixed();
}

void RigidBodySetGravity(uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6640 RigidBody::SetGravity");
  const Body b(body);
  vl::Vec4Copy(b.gravity_address(), vector);
  const double mass = double(b.mass());
  vl::StoreVector(b.weight_address(), vl::Scale(vl::LoadVector(vector), mass));
}

void RigidBodySetMass(uint32_t body, double mass) {
  ITC_NATIVE_RUNNING("sub_820B6698 RigidBody::SetMass");
  const Body b(body);
  const uint8_t fixed = b.fixed();
  b.set_mass(float(mass));
  b.set_inverse_mass(float(double(G::one()) / mass));
  if (fixed != 0) {
    guest::StoreU32(b.inverse_mass_address(), G::zero_bits());
  }
  vl::StoreVector(b.weight_address(), vl::Scale(vl::LoadVector(b.gravity_address()), mass));
}

float RigidBodyMass(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6708 RigidBody::Mass");
  return Body(body).mass();
}

void RigidBodySetCenterOfMass(uint32_t body, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820B67E0 RigidBody::SetCenterOfMass");
  const Body b(body);
  vl::Vec4Copy(b.center_of_mass_offset_address(), offset);
  const vl::Vector rotated =
      Rotate(b.orientation_matrix_address(), vl::LoadVector(b.center_of_mass_offset_address()));
  const vl::Vector center = vl::Add(vl::LoadVector(b.origin_address()), rotated);
  // Matrix::SetTranslation on the matrix at +224, then +16 = its fourth
  // row's x, y, z with w 0.0.
  const Matrix orientation(b.orientation_matrix_address());
  orientation.set_m30(center.x);
  orientation.set_m31(center.y);
  orientation.set_m32(center.z);
  orientation.set_m33_bits(G::one_bits());
  const VectorView center_of_mass(b.center_of_mass_address());
  center_of_mass.set_x_bits(orientation.m30_bits());
  center_of_mass.set_y_bits(orientation.m31_bits());
  center_of_mass.set_z_bits(orientation.m32_bits());
  center_of_mass.set_w_bits(G::zero_bits());
}

uint32_t RigidBodyField80(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6880 RigidBody::Field80");
  return Body(nullptr, body).orientation_address();
}

uint32_t RigidBodyVelocity(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6888 RigidBody::Velocity");
  return Body(nullptr, body).velocity_address();
}

uint32_t RigidBodyField144(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6890 RigidBody::Field144");
  return Body(nullptr, body).velocity_direction_address();
}

uint32_t RigidBodyForce(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820988C0 RigidBody::Force");
  return Body(nullptr, body).force_address();
}

uint32_t RigidBodyTorque(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_82379328 RigidBody::Torque");
  return Body(nullptr, body).torque_address();
}

uint32_t RigidBodyField160(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6898 RigidBody::Field160");
  return Body(nullptr, body).speed_address();
}

uint32_t RigidBodyOrientation(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6928 RigidBody::Orientation");
  return Body(nullptr, body).orientation_matrix_address();
}

uint32_t RigidBodyAngularVelocity(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6948 RigidBody::AngularVelocity");
  return Body(nullptr, body).angular_velocity_address();
}

float RigidBodyFloat48(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B68A0 RigidBody::Float48");
  const Body b(body);
  if (b.fixed() != 0) {
    return G::zero();
  }
  return b.speed_cap();
}

void RigidBodyPointVelocity(uint32_t out, uint32_t body, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B68C0 RigidBody::PointVelocity");
  const vl::Vector spin = PointVelocityCross(body, point);
  // Vec3::Sum: out's w = 0.0, then Vec3::Add's steps.
  schema::VectorLibVector(out).set_w_bits(G::zero_bits());
  vl::AddValue(out, Body(body).velocity_address(), spin, false);
}

void RigidBodySetFloat112(uint32_t body, double value) {
  ITC_NATIVE_RUNNING("sub_820B6950 RigidBody::SetFloat112");
  Body(body).set_spin_cap(float(value));
}

void RigidBodyAddForce(uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6958 RigidBody::AddForce");
  vl::Vec3AddTo(Body(body).force_address(), vector);
}

void RigidBodyAddTorque(uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6960 RigidBody::AddTorque");
  vl::Vec3AddTo(Body(body).torque_address(), vector);
}

namespace {

// AddForceAt's and AddTorqueAt's torque: +128 += (point - +16) x force.
void AddTorqueOf(uint32_t body, uint32_t force, uint32_t point) {
  const Body b(body);
  const vl::Vector d = vl::Sub(vl::LoadVector(point), vl::LoadVector(b.center_of_mass_address()));
  const vl::Vector torque = vl::Cross(d, vl::LoadVector(force));
  vl::AddValue(b.torque_address(), b.torque_address(), torque, false);
}

}  // namespace

void RigidBodyAddForceAt(uint32_t body, uint32_t force, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B6968 RigidBody::AddForceAt");
  AddTorqueOf(body, force, point);
  vl::Vec3AddTo(Body(body).force_address(), force);
}

void RigidBodyAddTorqueAt(uint32_t body, uint32_t force, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B69D8 RigidBody::AddTorqueAt");
  AddTorqueOf(body, force, point);
}

float VehicleWheelSlip(uint32_t vehicle, uint32_t wheel, double threshold) {
  ITC_NATIVE_RUNNING("sub_820C0180 Vehicle::WheelSlip");
  const Vehicle v(vehicle);
  const Wheel record(v.wheels_address() + Wheel::kSize * wheel);
  if (record.on_ground() == 0) {
    return G::zero();
  }
  // The wheel's spin times its radius (probably), along the first axis.
  const Axle axle(guest::LoadU32(v.wheel_axle_data_address() + 4 * wheel));
  const float rolled = float(double(axle.radius()) * double(record.spin()));
  const vl::Vector zero_rolled = {rolled, 0.0f, 0.0f, 0.0f};
  // The body's velocity at the wheel (RigidBody::PointVelocity).
  const Body body = v.body();
  const vl::Vector velocity = vl::Add(vl::LoadVector(body.velocity_address()),
                                      PointVelocityCross(body.address, record.mount_point_address()));
  const vl::Vector along = {vl::Dot(velocity, vl::LoadVector(record.forward_axis_address())), 0.0f,
                            vl::Dot(velocity, vl::LoadVector(record.side_axis_address())), 0.0f};
  const vl::Vector slip = vl::Sub(along, zero_rolled);
  const double length = double(float(std::sqrt(double(vl::Dot(slip, slip)))));
  const double least = double(float(threshold * double(G::slip_scale())));
  if (length > least) {
    return float(length);
  }
  return 0.0f;
}

float VehicleMaxWheelSlip(uint32_t vehicle, double threshold) {
  ITC_NATIVE_RUNNING("sub_820C0290 Vehicle::MaxWheelSlip");
  float most = G::zero();
  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const float slip = VehicleWheelSlip(vehicle, wheel, threshold);
    if (!(double(most) >= double(slip))) {
      most = slip;
    }
  }
  return most;
}

float VehicleFloat2224(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0140 Vehicle::Float2224");
  return Vehicle(vehicle).unknown_2224();
}

float VehicleSpeed(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0148 Vehicle::Speed");
  return Vehicle(vehicle).speed();
}

float VehicleDistancePerFrame(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0150 Vehicle::GetDistancePerFrame");
  return float(double(Vehicle(vehicle).speed()) * double(G::frame_seconds()));
}

uint32_t VehicleIdentity(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0168 Vehicle::Identity");
  return Vehicle(nullptr, vehicle).own_track_position_address();
}

uint32_t VehicleSetObjectHalf114(uint32_t vehicle, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820C0450 Vehicle::SetObjectHalf114");
  const uint32_t object = Vehicle(vehicle).box();
  if (object != 0) {
    ObjectSetHalf114(object, value & 0xFFFF);
  }
  return object;
}

void QuaternionToMatrix3(uint32_t out, uint32_t quaternion) {
  ITC_NATIVE_RUNNING("sub_820C0490 Quaternion::ToMatrix3");
  using Fpscr = ::base::FloatMode;
  // The squared length: vmsum4fp128 on the 16 bytes at the quaternion
  // rounded down to 16 (lvx128), with flush-to-zero on.
  const uint32_t aligned = quaternion & ~0xFu;
  simde__m128 q = simde_mm_castsi128_ps(simde_mm_set_epi32(
      int32_t(guest::LoadU32(aligned + 0)), int32_t(guest::LoadU32(aligned + 4)),
      int32_t(guest::LoadU32(aligned + 8)), int32_t(guest::LoadU32(aligned + 12))));
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vector_library::PinInMode(q);
  simde__m128 squared = simde_mm_dp_ps(q, q, 0xFF);
  vector_library::PinInMode(squared);
  Fpscr::setcsr(csr);
  const schema::VectorLibVector q4(quaternion);
  const double y = double(q4.y());
  const double z = double(q4.z());
  const double x = double(q4.x());
  const double w = double(q4.w());
  auto muls = [](double a, double b) { return double(float(a * b)); };
  auto adds = [](double a, double b) { return double(float(a + b)); };
  auto subs = [](double a, double b) { return double(float(a - b)); };
  const double s =
      double(float(double(G::quaternion_scale()) / double(simde_mm_cvtss_f32(squared))));
  const double ys = muls(y, s);
  const double zs = muls(z, s);
  const double xs = muls(x, s);
  const double yy = muls(y, ys);
  const double zz = muls(z, zs);
  const double xx = muls(x, xs);
  const double wy = muls(w, ys);
  const double wx = muls(w, xs);
  const double xy = muls(x, ys);
  const double wz = muls(w, zs);
  const double xz = muls(x, zs);
  const double yz = muls(y, zs);
  const double zz_yy = adds(zz, yy);
  const double zz_xx = adds(zz, xx);
  const double yy_xx = adds(yy, xx);
  const Matrix m(out);
  m.set_m01(float(adds(xy, wz)));
  m.set_m02(float(subs(xz, wy)));
  m.set_m20(float(adds(xz, wy)));
  const double one = double(G::one());
  m.set_m10(float(subs(xy, wz)));
  const double yz_wx = adds(yz, wx);
  m.set_m00(float(subs(one, zz_yy)));
  const double row1 = subs(one, zz_xx);
  m.set_m12(float(yz_wx));
  const double yz_less_wx = subs(yz, wx);
  m.set_m11(float(row1));
  const double row2 = subs(one, yy_xx);
  m.set_m21(float(yz_less_wx));
  m.set_m22(float(row2));
}

void QuaternionToMatrix(uint32_t out, uint32_t quaternion) {
  ITC_NATIVE_RUNNING("sub_820C0550 Quaternion::ToMatrix");
  QuaternionToMatrix3(out, quaternion);
  const Matrix matrix(out);
  const uint32_t zero = G::zero_bits();
  matrix.set_m32_bits(zero);
  matrix.set_m31_bits(zero);
  matrix.set_m30_bits(zero);
  matrix.set_m33_bits(G::one_bits());
}

void VehicleSpeedRecord(uint32_t out, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0590 Vehicle::SpeedRecord");
  const Vehicle v(vehicle);
  const schema::TrackPosition own(v.own_track_position_address());
  guest::StoreFloat(out, float(double(v.speed()) * double(G::kmh_scale())));
  guest::StoreU16(out + 4, own.route_section());
  guest::StoreU16(out + 6, own.route_slice());
}

// A list head's word (Link's and Unlink's r4): in guest memory, or inside a
// host object (the collision world's +20 and +24 through its handle, E5
// step 4; host structs keep the guest offsets, the word in host order).
static uint32_t LoadHead(uint32_t head) {
  if (memory::IsHandle(head)) {
    return *static_cast<const uint32_t*>(memory::HandleObject(head));
  }
  return guest::LoadU32(head);
}

static void StoreHead(uint32_t head, uint32_t value) {
  if (memory::IsHandle(head)) {
    *static_cast<uint32_t*>(memory::HandleObject(head)) = value;
    return;
  }
  guest::StoreU32(head, value);
}

void CollisionObjectLink(uint32_t object, uint32_t head) {
  ITC_NATIVE_RUNNING("sub_820B7BF0 CollisionObject::Link");
  const CollisionObject o(object);
  const uint32_t first = LoadHead(head);
  if (first != 0) {
    CollisionObject(first).set_previous(object);
  }
  o.set_previous(0);
  o.set_next(LoadHead(head));
  StoreHead(head, object);
}

void CollisionObjectUnlink(uint32_t object, uint32_t head) {
  ITC_NATIVE_RUNNING("sub_820B7C18 CollisionObject::Unlink");
  const CollisionObject o(object);
  const CollisionObject previous = o.previous();
  if (previous) {
    previous.set_next(o.next());
  } else {
    StoreHead(head, o.next().address);
  }
  const CollisionObject next = o.next();
  if (next) {
    next.set_previous(o.previous());
  }
}

uint32_t CollisionObjectFirstContact(uint32_t object, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_820B7C90 CollisionObject::FirstContact");
  for (Contact contact = CollisionObject(object).first_contact(); contact;
       contact = contact.next()) {
    if (contact.flags() & mask) {
      return contact.address;
    }
  }
  return 0;
}

uint32_t CollisionObjectWord264(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7CB8 CollisionObject::Word264");
  return CollisionObject(object).first_pair<schema::View>().address;
}

void CollisionObjectSetWord264(uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B7CC0 CollisionObject::SetWord264");
  CollisionObject(object).set_first_pair(value);
}

namespace {

void SetBits(uint32_t at, uint32_t bits) {
  guest::StoreU32(at, bits | guest::LoadU32(at));
}

void ClearBits(uint32_t at, uint32_t bits) {
  guest::StoreU32(at, guest::LoadU32(at) & ~bits);
}

}  // namespace

void CollisionObjectClearBits252(uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D18 CollisionObject::ClearBits252");
  ClearBits(CollisionObject(object).mask_address(), bits);
}

void CollisionObjectSetBits256(uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D28 CollisionObject::SetBits256");
  SetBits(CollisionObject(object).response_bits_address(), bits);
}

void CollisionObjectClearBits256(uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D38 CollisionObject::ClearBits256");
  ClearBits(CollisionObject(object).response_bits_address(), bits);
}

void CollisionObjectSetBits260(uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D48 CollisionObject::SetBits260");
  SetBits(CollisionObject(object).push_out_bits_address(), bits);
}

void CollisionObjectClearBits260(uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D58 CollisionObject::ClearBits260");
  ClearBits(CollisionObject(object).push_out_bits_address(), bits);
}

void CollisionObjectSetSize(uint32_t object, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B7D68 CollisionObject::SetSize");
  vl::Vec4Copy(CollisionObject(object).push_scale_x_address(), vector);
}

void CollisionObjectSetFloat224(uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B7D70 CollisionObject::SetFloat224");
  CollisionObject(object).set_torque_scale(float(value));
}

void CollisionRecordCopy(uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820B7D78 CollisionRecord::Copy");
  // Hit records, host (the world's) or guest (a caller's frame): dual views.
  const schema::CollisionHit to(out);
  const schema::CollisionHit from(record);
  vl::Vec4Copy(to.point_address(), from.point_address());
  vl::Vec4Copy(to.normal_address(), from.normal_address());
  vl::Vec4Copy(to.push_address(), from.push_address());
  // lfs and stfs, lwz and stw: the words' bits, in order.
  to.set_depth_bits(from.depth_bits());
  to.set_depth_2_bits(from.depth_2_bits());
  to.set_surface_bits(from.surface_bits());
  to.set_key(from.key());
  to.set_time_bits(from.time_bits());
}

void CollisionSetGlobal78(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B8768 Collision::SetGlobal78");
  G::set_query_walls(value);
}

void CollisionSetGlobal7C(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B8778 Collision::SetGlobal7C");
  G::set_query_ground(value);
}

void CollisionWorldSetCallback(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B8788 CollisionWorld::SetCallback");
  World(G::collision_world()).set_ground_callback(value);
}

uint32_t CollisionWorldNewContact() {
  ITC_NATIVE_RUNNING("sub_820B8A48 CollisionWorld::NewContact");
  const World world(G::collision_world());
  if (!(world.contacts_used() < world.contact_capacity())) {
    return 0;
  }
  const uint32_t used = world.contacts_used();
  const uint32_t contact = used * Contact::kSize + world.contacts<schema::View>().address;
  world.set_contacts_used(used + 1);
  return contact;
}

uint32_t CollisionWorldWord60() {
  ITC_NATIVE_RUNNING("sub_820B8C90 CollisionWorld::Word60");
  return World(G::collision_world()).query_count();
}

uint32_t CollisionWorldWord64() {
  ITC_NATIVE_RUNNING("sub_820B8CA0 CollisionWorld::Word64");
  return World(G::collision_world()).hits<schema::View>().address;
}

uint32_t CollisionWorldWord68() {
  ITC_NATIVE_RUNNING("sub_820B8CB0 CollisionWorld::Word68");
  return World(G::collision_world()).merged_hits<schema::View>().address;
}

void Object344SetFloat20(uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9658 Object344::SetFloat20");
  Box(object).set_car_float_20(float(value));
}

void Object344SetFloat16(uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9660 Object344::SetFloat16");
  Box(object).set_car_float_16(float(value));
}

float Object344Float28(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B9668 Object344::Float28");
  return Box(object).friction();
}

void Object344SetFloat28(uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9670 Object344::SetFloat28");
  Box(object).set_friction(float(value));
}

void Object344SetFloat24(uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9678 Object344::SetFloat24");
  Box(object).set_car_float_24(float(value));
}

uint32_t ObjectHalf112(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B99B0 Object::Half112");
  return Box(object).unknown_112();
}

void ObjectSetHalf112(uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B99B8 Object::SetHalf112");
  Box(object).set_unknown_112(uint16_t(value));
}

uint32_t ObjectHalf114(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B99C0 Object::Half114");
  return Box(object).unknown_114();
}

void ObjectSetHalf114(uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B99C8 Object::SetHalf114");
  Box(object).set_unknown_114(uint16_t(value));
}

uint32_t ContactOtherObject(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8CE0 Contact::OtherObject");
  const Contact c(contact);
  if (c.flags() & 0xFF000000u) {
    return c.other();
  }
  return 0;
}

uint32_t ContactNext(uint32_t contact, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_820B8D10 Contact::Next");
  for (Contact c = Contact(contact).next(); c; c = c.next()) {
    if (c.flags() & mask) {
      return c.address;
    }
  }
  return 0;
}

void RaceDataSetWord59618(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FF918 RaceData::SetWord59618");
  if (value != 0) {
    schema::RaceData(G::race_data()).set_word_59618(value);
  }
}

void RaceDataSetWord5961C(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FF938 RaceData::SetWord5961C");
  if (value != 0) {
    schema::RaceData(G::race_data()).set_word_5961c(value);
  }
}

void RaceDataSetWord32(uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FF968 RaceData::SetWord32");
  schema::RaceData(G::race_data()).set_mode(value);
}

uint32_t StagesHasNext(uint32_t stages) {
  ITC_NATIVE_RUNNING("sub_820FFA30 Stages::HasNext");
  const Stages list(stages);
  const int32_t last = int32_t(list.count() - 1);
  return int32_t(list.current()) < last ? 1 : 0;
}

void StagesSetType(uint32_t stages, uint32_t type, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFA50 Stages::SetType");
  if (int32_t(type) == 4) {
    return;
  }
  const Stages list(stages);
  if (int32_t(stage) == -1) {
    RaceStage(stages + RaceStage::kSize * list.count()).set_type(type);
    list.set_count(list.count() + 1);
    return;
  }
  RaceStage(stages + RaceStage::kSize * stage).set_type(type);
}

void StagesSetWord808(uint32_t stages, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FFAB0 Stages::SetWord808");
  Stages(stages).set_word_808(value);
}

uint32_t StagesWord808(uint32_t stages) {
  ITC_NATIVE_RUNNING("sub_820FFC68 Stages::Word808");
  return Stages(stages).word_808();
}

uint32_t StagesType(uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFBE8 Stages::Type");
  return RaceStage(Stage(stages, stage)).type();
}

uint32_t StagesWord4(uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC08 Stages::Word4");
  return RaceStage(Stage(stages, stage)).word_4();
}

uint32_t StagesIsType2Or3(uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC30 Stages::IsType2Or3");
  const int32_t type = int32_t(RaceStage(Stage(stages, stage)).type());
  return type == 2 || type == 3 ? 1 : 0;
}

uint32_t StagesWord76(uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC70 Stages::Word76");
  return RaceStage(Stage(stages, stage)).slot_count();
}

uint32_t StagesRacer(uint32_t stages, uint32_t slot, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC98 Stages::Racer");
  return guest::LoadU32(RaceStage(Stage(stages, stage)).racer0_address() + 8 * slot);
}

uint32_t StagesWord48(uint32_t stages, uint32_t slot, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFCC8 Stages::Word48");
  return guest::LoadU32(RaceStage(Stage(stages, stage)).racer0_word_address() + 8 * slot);
}

void StagesCopy24(uint32_t out, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFD48 Stages::Copy24");
  const schema::StageRoute from(RaceStage(Stage(stages, stage)).route_address());
  const schema::StageRoute to(out);
  const uint32_t course = from.course();
  const uint32_t location = from.location();
  const uint32_t first_position = from.first_position();
  const uint32_t last_position = from.last_position();
  to.set_course(course);
  to.set_location(location);
  to.set_first_position(first_position);
  to.set_last_position(last_position);
}

bool StagesCopy(uint32_t out, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFD88 Stages::Copy");
  return CopyLikeMemcpy(out, Stage(stages, stage), 80);
}

void EntriesAdd(uint32_t list, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820FFDD8 Entries::Add");
  // The results record's entries are 972 bytes apart.
  const schema::RacerResultsRecord results(list);
  guest::StoreU32(results.e0_record_index_address() + 972 * results.count(), a);
  guest::StoreU32(results.e0_item_address() + 972 * results.count(), b);
  results.set_count(results.count() + 1);
}

bool RacerRecordsAdd(uint32_t records, uint32_t record, uint32_t* added) {
  ITC_NATIVE_RUNNING("sub_820FFE08 RacerRecords::Add");
  const RacerRecordArray array(records);
  const uint32_t to = records + kRecordBytes * array.count();
  if (!CopyLikeMemcpy(to, record, kRecordBytes)) {
    return false;
  }
  array.set_count(array.count() + 1);
  *added = to;
  return true;
}

uint32_t RacerRecordsAt(uint32_t records, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_820FFE58 RacerRecords::At");
  if (int32_t(n) != -1) {
    return records + kRecordBytes * n;
  }
  const int32_t count = int32_t(RacerRecordArray(records).count());
  if (count <= 0) {
    return 0;
  }
  return records + kRecordBytes * uint32_t(count) - kRecordBytes;
}

namespace {

void CopyPair(uint32_t out, uint32_t pair) {
  const uint16_t first = Pair(pair).first();
  const uint16_t second = Pair(pair).second();
  Pair(out).set_first(first);
  Pair(out).set_second(second);
}

}  // namespace

void PairCopy(uint32_t out, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_820FFE98 Pair::Copy");
  CopyPair(out, pair);
}

void PairsLast(uint32_t out, uint32_t pairs) {
  ITC_NATIVE_RUNNING("sub_820FFEB0 Pairs::Last");
  for (uint32_t i = 0; i < 5; i++) {
    if (Pair(pairs + Pair::kSize * i).first() == 0xFFFF) {
      CopyPair(out, pairs + Pair::kSize * i - Pair::kSize);
      return;
    }
  }
  CopyPair(out, schema::AccessorPairs(pairs).pair4_first_address());
}

void PairsAt(uint32_t out, uint32_t pairs, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820FFF10 Pairs::At");
  if (int32_t(i) < 5) {
    CopyPair(out, pairs + Pair::kSize * i);
    return;
  }
  Pair(out).set_both(0xFFFFFFFFu);
}

void ListInit(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82593FB0 List::Init");
  List(list).set_first(0);
  List(list).set_count(0);
}

uint32_t ListAt(uint32_t list, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_82593FC0 List::At");
  const ListNode first = List(list).first();
  uint32_t i = 0;
  for (ListNode node = first; node; node = node.next()) {
    if (int32_t(i) == int32_t(n)) {
      return node.address;
    }
    i++;
  }
  return first.address;
}

namespace {

// ClaimA's and ClaimB's steps on the pool's list (its count at +8).
int32_t Claim(uint32_t list) {
  const int32_t count = int32_t(List(list).count());
  for (int32_t i = 0; i < count; i++) {
    const ListNode object(ListAt(list, uint32_t(i)));
    if (object.state() == 0) {
      object.set_state(3);
      return i;
    }
  }
  return -1;
}

uint32_t GetInUse(uint32_t list, uint32_t index) {
  if (int32_t(ListNode(ListAt(list, index)).state()) != 3) {
    return 0;
  }
  return ListAt(list, index);
}

}  // namespace

int32_t ScenePoolsClaimA(uint32_t scene, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_820C42C8 ScenePools::ClaimA");
  return Claim(schema::ScenePools(scene).pools_address() + 12 * pool);
}

int32_t ScenePoolsClaimB(uint32_t scene, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_820C5AB0 ScenePools::ClaimB");
  return Claim(schema::CarScene(scene).car_models_first_address() + 12 * pool);
}

uint32_t ScenePoolsGetA(uint32_t scene, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C4420 ScenePools::GetA");
  return GetInUse(schema::ScenePools(scene).pools_address() + 12 * pool, index);
}

uint32_t ScenePoolsGetB(uint32_t scene, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C5C20 ScenePools::GetB");
  return GetInUse(schema::CarScene(scene).car_models_first_address() + 12 * pool, index);
}

uint32_t ScenePoolsFindModel(uint32_t scene, uint32_t kind, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_820C5D70 ScenePools::FindModel");
  const schema::CarScene car_scene(scene);
  const int32_t count = int32_t(car_scene.scene_models_count());
  int32_t found = 0;
  for (int32_t i = 0; i < count; i++) {
    const ListNode object(ListAt(car_scene.scene_models_first_address(), uint32_t(i)));
    if (int32_t(object.state()) != 3 || object.kind() != kind) {
      continue;
    }
    if (found == int32_t(n)) {
      return object.address;
    }
    found++;
  }
  return 0;
}

uint32_t ObjectRecord1888(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820C4550 Object::Record1888");
  return object != 0 ? object + 1888 : 0;
}

uint32_t DebugSwitchOn() {
  ITC_NATIVE_RUNNING("sub_825C8390 DebugSwitchOn");
  return 1;
}

}  // namespace vehicle_accessors
