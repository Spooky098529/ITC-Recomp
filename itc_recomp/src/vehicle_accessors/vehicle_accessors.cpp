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
bool CopyLikeMemcpy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && c_runtime::Memcpy(base, destination, source, size);
}

// The stage `stage` (-1 for the current, +804) of the stage list.
uint32_t Stage(uint8_t* base, uint32_t stages, uint32_t stage) {
  if (int32_t(stage) == -1) {
    stage = Stages(base, stages).current();
  }
  return stages + RaceStage::kSize * stage;
}

namespace vl = vector_library;

// The rotation of `v` by the matrix at `m` (Matrix::Rotate's arithmetic),
// w 0.0.
vl::Vector Rotate(uint8_t* base, uint32_t m, const vl::Vector& v) {
  const Matrix matrix(base, m);
  auto muls = [](double a, double b) { return double(float(a * b)); };
  auto madds = [](double a, double b, double c) { return double(float(std::fma(a, b, c))); };
  const double x0 = madds(double(matrix.m10()), v.y, muls(double(matrix.m20()), v.z));
  const double y0 = madds(double(matrix.m01()), v.x, muls(double(matrix.m11()), v.y));
  const double z0 = madds(double(matrix.m02()), v.x, muls(double(matrix.m12()), v.y));
  return {float(madds(v.x, double(matrix.m00()), x0)), float(madds(double(matrix.m21()), v.z, y0)),
          float(madds(double(matrix.m22()), v.z, z0)), 0.0f};
}

// RigidBody::PointVelocity's value: +32 + +96 x (point - +16).
vl::Vector PointVelocityCross(uint8_t* base, uint32_t body, uint32_t point) {
  const Body b(base, body);
  const vl::Vector d = vl::Sub(vl::LoadVector(base, point), vl::LoadVector(base, b.center_of_mass_address()));
  return vl::Cross(vl::LoadVector(base, b.angular_velocity_address()), d);
}

}  // namespace

uint32_t VehicleGet(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C0468 Vehicle::Get");
  const uint32_t vehicles = G::vehicles(base);
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

uint32_t ContactObject(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8CC0 Contact::Object");
  const Contact c(base, contact);
  if (c.flags() & 0xFF000000u) {
    return 0;
  }
  return c.other();
}

uint32_t ContactPosition(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D00 Contact::Position");
  return Contact(nullptr, contact).position_address();
}

float ContactStrength(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D08 Contact::Strength");
  return Contact(base, contact).strength();
}

int32_t RacersIndexOf(uint8_t* base, uint32_t records, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820FF9F8 Racers::IndexOf");
  for (uint32_t i = 0; i < 93; i++) {
    if (RacerRecord(base, records + kRecordBytes * i).item_id() == id) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StagesHasRacer(uint8_t* base, uint32_t stages, uint32_t racer, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFCF8 Stages::HasRacer");
  if (int32_t(stage) == -1) {
    stage = Stages(base, stages).current();
  }
  const uint32_t slots = RaceStage(base, stages + RaceStage::kSize * stage).racer0_address();
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t slot = guest::LoadU32(base, slots + 8 * i);
    if (int32_t(slot) == -1) {
      return 0;
    }
    if (slot == racer) {
      return 1;
    }
  }
  return 0;
}

uint32_t SaveDataOf(uint8_t* base, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_82105620 SaveData::Of");
  return guest::LoadU32(base, G::save_data_address() + 4 * player);
}

uint32_t DebugSwitchOff() {
  ITC_NATIVE_RUNNING("sub_825C8388 DebugSwitchOff");
  return 0;
}

uint32_t RigidBodyIsFixed(uint8_t* base, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6638 RigidBody::IsFixed");
  return Body(base, body).fixed();
}

void RigidBodySetGravity(uint8_t* base, uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6640 RigidBody::SetGravity");
  const Body b(base, body);
  vl::Vec4Copy(base, b.gravity_address(), vector);
  const double mass = double(b.mass());
  vl::StoreVector(base, b.weight_address(), vl::Scale(vl::LoadVector(base, vector), mass));
}

void RigidBodySetMass(uint8_t* base, uint32_t body, double mass) {
  ITC_NATIVE_RUNNING("sub_820B6698 RigidBody::SetMass");
  const Body b(base, body);
  const uint8_t fixed = b.fixed();
  b.set_mass(float(mass));
  b.set_inverse_mass(float(double(G::one(base)) / mass));
  if (fixed != 0) {
    guest::StoreU32(base, b.inverse_mass_address(), G::zero_bits(base));
  }
  vl::StoreVector(base, b.weight_address(), vl::Scale(vl::LoadVector(base, b.gravity_address()), mass));
}

float RigidBodyMass(uint8_t* base, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6708 RigidBody::Mass");
  return Body(base, body).mass();
}

void RigidBodySetCenterOfMass(uint8_t* base, uint32_t body, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820B67E0 RigidBody::SetCenterOfMass");
  const Body b(base, body);
  vl::Vec4Copy(base, b.center_of_mass_offset_address(), offset);
  const vl::Vector rotated =
      Rotate(base, b.orientation_matrix_address(), vl::LoadVector(base, b.center_of_mass_offset_address()));
  const vl::Vector center = vl::Add(vl::LoadVector(base, b.origin_address()), rotated);
  // Matrix::SetTranslation on the matrix at +224, then +16 = its fourth
  // row's x, y, z with w 0.0.
  const Matrix orientation(base, b.orientation_matrix_address());
  orientation.set_m30(center.x);
  orientation.set_m31(center.y);
  orientation.set_m32(center.z);
  orientation.set_m33_bits(G::one_bits(base));
  const VectorView center_of_mass(base, b.center_of_mass_address());
  center_of_mass.set_x_bits(orientation.m30_bits());
  center_of_mass.set_y_bits(orientation.m31_bits());
  center_of_mass.set_z_bits(orientation.m32_bits());
  center_of_mass.set_w_bits(G::zero_bits(base));
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

float RigidBodyFloat48(uint8_t* base, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B68A0 RigidBody::Float48");
  const Body b(base, body);
  if (b.fixed() != 0) {
    return G::zero(base);
  }
  return b.speed_cap();
}

void RigidBodyPointVelocity(uint8_t* base, uint32_t out, uint32_t body, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B68C0 RigidBody::PointVelocity");
  const vl::Vector spin = PointVelocityCross(base, body, point);
  // Vec3::Sum: out's w = 0.0, then Vec3::Add's steps.
  schema::VectorLibVector(base, out).set_w_bits(G::zero_bits(base));
  vl::AddValue(base, out, Body(base, body).velocity_address(), spin, false);
}

void RigidBodySetFloat112(uint8_t* base, uint32_t body, double value) {
  ITC_NATIVE_RUNNING("sub_820B6950 RigidBody::SetFloat112");
  Body(base, body).set_spin_cap(float(value));
}

void RigidBodyAddForce(uint8_t* base, uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6958 RigidBody::AddForce");
  vl::Vec3AddTo(base, Body(base, body).force_address(), vector);
}

void RigidBodyAddTorque(uint8_t* base, uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6960 RigidBody::AddTorque");
  vl::Vec3AddTo(base, Body(base, body).torque_address(), vector);
}

namespace {

// AddForceAt's and AddTorqueAt's torque: +128 += (point - +16) x force.
void AddTorqueOf(uint8_t* base, uint32_t body, uint32_t force, uint32_t point) {
  const Body b(base, body);
  const vl::Vector d = vl::Sub(vl::LoadVector(base, point), vl::LoadVector(base, b.center_of_mass_address()));
  const vl::Vector torque = vl::Cross(d, vl::LoadVector(base, force));
  vl::AddValue(base, b.torque_address(), b.torque_address(), torque, false);
}

}  // namespace

void RigidBodyAddForceAt(uint8_t* base, uint32_t body, uint32_t force, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B6968 RigidBody::AddForceAt");
  AddTorqueOf(base, body, force, point);
  vl::Vec3AddTo(base, Body(base, body).force_address(), force);
}

void RigidBodyAddTorqueAt(uint8_t* base, uint32_t body, uint32_t force, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B69D8 RigidBody::AddTorqueAt");
  AddTorqueOf(base, body, force, point);
}

float VehicleWheelSlip(uint8_t* base, uint32_t vehicle, uint32_t wheel, double threshold) {
  ITC_NATIVE_RUNNING("sub_820C0180 Vehicle::WheelSlip");
  const Vehicle v(base, vehicle);
  const Wheel record(base, v.wheels_address() + Wheel::kSize * wheel);
  if (record.on_ground() == 0) {
    return G::zero(base);
  }
  // The wheel's spin times its radius (probably), along the first axis.
  const Axle axle(base, guest::LoadU32(base, v.wheel_axle_data_address() + 4 * wheel));
  const float rolled = float(double(axle.radius()) * double(record.spin()));
  const vl::Vector zero_rolled = {rolled, 0.0f, 0.0f, 0.0f};
  // The body's velocity at the wheel (RigidBody::PointVelocity).
  const Body body = v.body();
  const vl::Vector velocity = vl::Add(vl::LoadVector(base, body.velocity_address()),
                                      PointVelocityCross(base, body.address, record.mount_point_address()));
  const vl::Vector along = {vl::Dot(velocity, vl::LoadVector(base, record.forward_axis_address())), 0.0f,
                            vl::Dot(velocity, vl::LoadVector(base, record.side_axis_address())), 0.0f};
  const vl::Vector slip = vl::Sub(along, zero_rolled);
  const double length = double(float(std::sqrt(double(vl::Dot(slip, slip)))));
  const double least = double(float(threshold * double(G::slip_scale(base))));
  if (length > least) {
    return float(length);
  }
  return 0.0f;
}

float VehicleMaxWheelSlip(uint8_t* base, uint32_t vehicle, double threshold) {
  ITC_NATIVE_RUNNING("sub_820C0290 Vehicle::MaxWheelSlip");
  float most = G::zero(base);
  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const float slip = VehicleWheelSlip(base, vehicle, wheel, threshold);
    if (!(double(most) >= double(slip))) {
      most = slip;
    }
  }
  return most;
}

float VehicleFloat2224(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0140 Vehicle::Float2224");
  return Vehicle(base, vehicle).unknown_2224();
}

float VehicleSpeed(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0148 Vehicle::Speed");
  return Vehicle(base, vehicle).speed();
}

float VehicleDistancePerFrame(uint8_t* base, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0150 Vehicle::GetDistancePerFrame");
  return float(double(Vehicle(base, vehicle).speed()) * double(G::frame_seconds(base)));
}

uint32_t VehicleIdentity(uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0168 Vehicle::Identity");
  return Vehicle(nullptr, vehicle).own_track_position_address();
}

uint32_t VehicleSetObjectHalf114(uint8_t* base, uint32_t vehicle, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820C0450 Vehicle::SetObjectHalf114");
  const uint32_t object = Vehicle(base, vehicle).box();
  if (object != 0) {
    ObjectSetHalf114(base, object, value & 0xFFFF);
  }
  return object;
}

void QuaternionToMatrix3(uint8_t* base, uint32_t out, uint32_t quaternion) {
  ITC_NATIVE_RUNNING("sub_820C0490 Quaternion::ToMatrix3");
  using Fpscr = ::base::FloatMode;
  // The squared length: vmsum4fp128 on the 16 bytes at the quaternion
  // rounded down to 16 (lvx128), with flush-to-zero on.
  const uint32_t aligned = quaternion & ~0xFu;
  simde__m128 q = simde_mm_castsi128_ps(simde_mm_set_epi32(
      int32_t(guest::LoadU32(base, aligned + 0)), int32_t(guest::LoadU32(base, aligned + 4)),
      int32_t(guest::LoadU32(base, aligned + 8)), int32_t(guest::LoadU32(base, aligned + 12))));
  const uint32_t csr = Fpscr::getcsr();
  Fpscr::setcsr(csr | uint32_t(Fpscr::FlushMask));
  vector_library::PinInMode(q);
  simde__m128 squared = simde_mm_dp_ps(q, q, 0xFF);
  vector_library::PinInMode(squared);
  Fpscr::setcsr(csr);
  const schema::VectorLibVector q4(base, quaternion);
  const double y = double(q4.y());
  const double z = double(q4.z());
  const double x = double(q4.x());
  const double w = double(q4.w());
  auto muls = [](double a, double b) { return double(float(a * b)); };
  auto adds = [](double a, double b) { return double(float(a + b)); };
  auto subs = [](double a, double b) { return double(float(a - b)); };
  const double s =
      double(float(double(G::quaternion_scale(base)) / double(simde_mm_cvtss_f32(squared))));
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
  const Matrix m(base, out);
  m.set_m01(float(adds(xy, wz)));
  m.set_m02(float(subs(xz, wy)));
  m.set_m20(float(adds(xz, wy)));
  const double one = double(G::one(base));
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

void QuaternionToMatrix(uint8_t* base, uint32_t out, uint32_t quaternion) {
  ITC_NATIVE_RUNNING("sub_820C0550 Quaternion::ToMatrix");
  QuaternionToMatrix3(base, out, quaternion);
  const Matrix matrix(base, out);
  const uint32_t zero = G::zero_bits(base);
  matrix.set_m32_bits(zero);
  matrix.set_m31_bits(zero);
  matrix.set_m30_bits(zero);
  matrix.set_m33_bits(G::one_bits(base));
}

void VehicleSpeedRecord(uint8_t* base, uint32_t out, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_820C0590 Vehicle::SpeedRecord");
  const Vehicle v(base, vehicle);
  const schema::TrackPosition own(base, v.own_track_position_address());
  guest::StoreFloat(base, out, float(double(v.speed()) * double(G::kmh_scale(base))));
  guest::StoreU16(base, out + 4, own.route_section());
  guest::StoreU16(base, out + 6, own.route_slice());
}

// A list head's word (Link's and Unlink's r4): in guest memory, or inside a
// host object (the collision world's +20 and +24 through its handle, E5
// step 4; host structs keep the guest offsets, the word in host order).
static uint32_t LoadHead(uint8_t* base, uint32_t head) {
  if (memory::IsHandle(head)) {
    return *static_cast<const uint32_t*>(memory::HandleObject(head));
  }
  return guest::LoadU32(base, head);
}

static void StoreHead(uint8_t* base, uint32_t head, uint32_t value) {
  if (memory::IsHandle(head)) {
    *static_cast<uint32_t*>(memory::HandleObject(head)) = value;
    return;
  }
  guest::StoreU32(base, head, value);
}

void CollisionObjectLink(uint8_t* base, uint32_t object, uint32_t head) {
  ITC_NATIVE_RUNNING("sub_820B7BF0 CollisionObject::Link");
  const CollisionObject o(base, object);
  const uint32_t first = LoadHead(base, head);
  if (first != 0) {
    CollisionObject(base, first).set_previous(object);
  }
  o.set_previous(0);
  o.set_next(LoadHead(base, head));
  StoreHead(base, head, object);
}

void CollisionObjectUnlink(uint8_t* base, uint32_t object, uint32_t head) {
  ITC_NATIVE_RUNNING("sub_820B7C18 CollisionObject::Unlink");
  const CollisionObject o(base, object);
  const CollisionObject previous = o.previous();
  if (previous) {
    previous.set_next(o.next());
  } else {
    StoreHead(base, head, o.next().address);
  }
  const CollisionObject next = o.next();
  if (next) {
    next.set_previous(o.previous());
  }
}

uint32_t CollisionObjectFirstContact(uint8_t* base, uint32_t object, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_820B7C90 CollisionObject::FirstContact");
  for (Contact contact = CollisionObject(base, object).first_contact(); contact;
       contact = contact.next()) {
    if (contact.flags() & mask) {
      return contact.address;
    }
  }
  return 0;
}

uint32_t CollisionObjectWord264(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B7CB8 CollisionObject::Word264");
  return CollisionObject(base, object).first_pair<schema::View>().address;
}

void CollisionObjectSetWord264(uint8_t* base, uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B7CC0 CollisionObject::SetWord264");
  CollisionObject(base, object).set_first_pair(value);
}

namespace {

void SetBits(uint8_t* base, uint32_t at, uint32_t bits) {
  guest::StoreU32(base, at, bits | guest::LoadU32(base, at));
}

void ClearBits(uint8_t* base, uint32_t at, uint32_t bits) {
  guest::StoreU32(base, at, guest::LoadU32(base, at) & ~bits);
}

}  // namespace

void CollisionObjectClearBits252(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D18 CollisionObject::ClearBits252");
  ClearBits(base, CollisionObject(base, object).mask_address(), bits);
}

void CollisionObjectSetBits256(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D28 CollisionObject::SetBits256");
  SetBits(base, CollisionObject(base, object).response_bits_address(), bits);
}

void CollisionObjectClearBits256(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D38 CollisionObject::ClearBits256");
  ClearBits(base, CollisionObject(base, object).response_bits_address(), bits);
}

void CollisionObjectSetBits260(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D48 CollisionObject::SetBits260");
  SetBits(base, CollisionObject(base, object).push_out_bits_address(), bits);
}

void CollisionObjectClearBits260(uint8_t* base, uint32_t object, uint32_t bits) {
  ITC_NATIVE_RUNNING("sub_820B7D58 CollisionObject::ClearBits260");
  ClearBits(base, CollisionObject(base, object).push_out_bits_address(), bits);
}

void CollisionObjectSetSize(uint8_t* base, uint32_t object, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B7D68 CollisionObject::SetSize");
  vl::Vec4Copy(base, CollisionObject(base, object).push_scale_x_address(), vector);
}

void CollisionObjectSetFloat224(uint8_t* base, uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B7D70 CollisionObject::SetFloat224");
  CollisionObject(base, object).set_torque_scale(float(value));
}

void CollisionRecordCopy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_820B7D78 CollisionRecord::Copy");
  // Hit records, host (the world's) or guest (a caller's frame): dual views.
  const schema::CollisionHit to(base, out);
  const schema::CollisionHit from(base, record);
  vl::Vec4Copy(base, to.point_address(), from.point_address());
  vl::Vec4Copy(base, to.normal_address(), from.normal_address());
  vl::Vec4Copy(base, to.push_address(), from.push_address());
  // lfs and stfs, lwz and stw: the words' bits, in order.
  to.set_depth_bits(from.depth_bits());
  to.set_depth_2_bits(from.depth_2_bits());
  to.set_surface_bits(from.surface_bits());
  to.set_key(from.key());
  to.set_time_bits(from.time_bits());
}

void CollisionSetGlobal78(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B8768 Collision::SetGlobal78");
  G::set_query_walls(base, value);
}

void CollisionSetGlobal7C(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B8778 Collision::SetGlobal7C");
  G::set_query_ground(base, value);
}

void CollisionWorldSetCallback(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B8788 CollisionWorld::SetCallback");
  World(base, G::collision_world(base)).set_ground_callback(value);
}

uint32_t CollisionWorldNewContact(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B8A48 CollisionWorld::NewContact");
  const World world(base, G::collision_world(base));
  if (!(world.contacts_used() < world.contact_capacity())) {
    return 0;
  }
  const uint32_t used = world.contacts_used();
  const uint32_t contact = used * Contact::kSize + world.contacts<schema::View>().address;
  world.set_contacts_used(used + 1);
  return contact;
}

uint32_t CollisionWorldWord60(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B8C90 CollisionWorld::Word60");
  return World(base, G::collision_world(base)).query_count();
}

uint32_t CollisionWorldWord64(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B8CA0 CollisionWorld::Word64");
  return World(base, G::collision_world(base)).hits<schema::View>().address;
}

uint32_t CollisionWorldWord68(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820B8CB0 CollisionWorld::Word68");
  return World(base, G::collision_world(base)).merged_hits<schema::View>().address;
}

void Object344SetFloat20(uint8_t* base, uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9658 Object344::SetFloat20");
  Box(base, object).set_car_float_20(float(value));
}

void Object344SetFloat16(uint8_t* base, uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9660 Object344::SetFloat16");
  Box(base, object).set_car_float_16(float(value));
}

float Object344Float28(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B9668 Object344::Float28");
  return Box(base, object).friction();
}

void Object344SetFloat28(uint8_t* base, uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9670 Object344::SetFloat28");
  Box(base, object).set_friction(float(value));
}

void Object344SetFloat24(uint8_t* base, uint32_t object, double value) {
  ITC_NATIVE_RUNNING("sub_820B9678 Object344::SetFloat24");
  Box(base, object).set_car_float_24(float(value));
}

uint32_t ObjectHalf112(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B99B0 Object::Half112");
  return Box(base, object).unknown_112();
}

void ObjectSetHalf112(uint8_t* base, uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B99B8 Object::SetHalf112");
  Box(base, object).set_unknown_112(uint16_t(value));
}

uint32_t ObjectHalf114(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820B99C0 Object::Half114");
  return Box(base, object).unknown_114();
}

void ObjectSetHalf114(uint8_t* base, uint32_t object, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820B99C8 Object::SetHalf114");
  Box(base, object).set_unknown_114(uint16_t(value));
}

uint32_t ContactOtherObject(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8CE0 Contact::OtherObject");
  const Contact c(base, contact);
  if (c.flags() & 0xFF000000u) {
    return c.other();
  }
  return 0;
}

uint32_t ContactNext(uint8_t* base, uint32_t contact, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_820B8D10 Contact::Next");
  for (Contact c = Contact(base, contact).next(); c; c = c.next()) {
    if (c.flags() & mask) {
      return c.address;
    }
  }
  return 0;
}

void RaceDataSetWord59618(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FF918 RaceData::SetWord59618");
  if (value != 0) {
    schema::RaceData(base, G::race_data(base)).set_word_59618(value);
  }
}

void RaceDataSetWord5961C(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FF938 RaceData::SetWord5961C");
  if (value != 0) {
    schema::RaceData(base, G::race_data(base)).set_word_5961c(value);
  }
}

void RaceDataSetWord32(uint8_t* base, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FF968 RaceData::SetWord32");
  schema::RaceData(base, G::race_data(base)).set_mode(value);
}

uint32_t StagesHasNext(uint8_t* base, uint32_t stages) {
  ITC_NATIVE_RUNNING("sub_820FFA30 Stages::HasNext");
  const Stages list(base, stages);
  const int32_t last = int32_t(list.count() - 1);
  return int32_t(list.current()) < last ? 1 : 0;
}

void StagesSetType(uint8_t* base, uint32_t stages, uint32_t type, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFA50 Stages::SetType");
  if (int32_t(type) == 4) {
    return;
  }
  const Stages list(base, stages);
  if (int32_t(stage) == -1) {
    RaceStage(base, stages + RaceStage::kSize * list.count()).set_type(type);
    list.set_count(list.count() + 1);
    return;
  }
  RaceStage(base, stages + RaceStage::kSize * stage).set_type(type);
}

void StagesSetWord808(uint8_t* base, uint32_t stages, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_820FFAB0 Stages::SetWord808");
  Stages(base, stages).set_word_808(value);
}

uint32_t StagesWord808(uint8_t* base, uint32_t stages) {
  ITC_NATIVE_RUNNING("sub_820FFC68 Stages::Word808");
  return Stages(base, stages).word_808();
}

uint32_t StagesType(uint8_t* base, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFBE8 Stages::Type");
  return RaceStage(base, Stage(base, stages, stage)).type();
}

uint32_t StagesWord4(uint8_t* base, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC08 Stages::Word4");
  return RaceStage(base, Stage(base, stages, stage)).word_4();
}

uint32_t StagesIsType2Or3(uint8_t* base, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC30 Stages::IsType2Or3");
  const int32_t type = int32_t(RaceStage(base, Stage(base, stages, stage)).type());
  return type == 2 || type == 3 ? 1 : 0;
}

uint32_t StagesWord76(uint8_t* base, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC70 Stages::Word76");
  return RaceStage(base, Stage(base, stages, stage)).slot_count();
}

uint32_t StagesRacer(uint8_t* base, uint32_t stages, uint32_t slot, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFC98 Stages::Racer");
  return guest::LoadU32(base, RaceStage(base, Stage(base, stages, stage)).racer0_address() + 8 * slot);
}

uint32_t StagesWord48(uint8_t* base, uint32_t stages, uint32_t slot, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFCC8 Stages::Word48");
  return guest::LoadU32(base, RaceStage(base, Stage(base, stages, stage)).racer0_word_address() + 8 * slot);
}

void StagesCopy24(uint8_t* base, uint32_t out, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFD48 Stages::Copy24");
  const schema::StageRoute from(base, RaceStage(base, Stage(base, stages, stage)).route_address());
  const schema::StageRoute to(base, out);
  const uint32_t course = from.course();
  const uint32_t location = from.location();
  const uint32_t first_position = from.first_position();
  const uint32_t last_position = from.last_position();
  to.set_course(course);
  to.set_location(location);
  to.set_first_position(first_position);
  to.set_last_position(last_position);
}

bool StagesCopy(uint8_t* base, uint32_t out, uint32_t stages, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFD88 Stages::Copy");
  return CopyLikeMemcpy(base, out, Stage(base, stages, stage), 80);
}

void EntriesAdd(uint8_t* base, uint32_t list, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_820FFDD8 Entries::Add");
  // The results record's entries are 972 bytes apart.
  const schema::RacerResultsRecord results(base, list);
  guest::StoreU32(base, results.e0_record_index_address() + 972 * results.count(), a);
  guest::StoreU32(base, results.e0_item_address() + 972 * results.count(), b);
  results.set_count(results.count() + 1);
}

bool RacerRecordsAdd(uint8_t* base, uint32_t records, uint32_t record, uint32_t* added) {
  ITC_NATIVE_RUNNING("sub_820FFE08 RacerRecords::Add");
  const RacerRecordArray array(base, records);
  const uint32_t to = records + kRecordBytes * array.count();
  if (!CopyLikeMemcpy(base, to, record, kRecordBytes)) {
    return false;
  }
  array.set_count(array.count() + 1);
  *added = to;
  return true;
}

uint32_t RacerRecordsAt(uint8_t* base, uint32_t records, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_820FFE58 RacerRecords::At");
  if (int32_t(n) != -1) {
    return records + kRecordBytes * n;
  }
  const int32_t count = int32_t(RacerRecordArray(base, records).count());
  if (count <= 0) {
    return 0;
  }
  return records + kRecordBytes * uint32_t(count) - kRecordBytes;
}

namespace {

void CopyPair(uint8_t* base, uint32_t out, uint32_t pair) {
  const uint16_t first = Pair(base, pair).first();
  const uint16_t second = Pair(base, pair).second();
  Pair(base, out).set_first(first);
  Pair(base, out).set_second(second);
}

}  // namespace

void PairCopy(uint8_t* base, uint32_t out, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_820FFE98 Pair::Copy");
  CopyPair(base, out, pair);
}

void PairsLast(uint8_t* base, uint32_t out, uint32_t pairs) {
  ITC_NATIVE_RUNNING("sub_820FFEB0 Pairs::Last");
  for (uint32_t i = 0; i < 5; i++) {
    if (Pair(base, pairs + Pair::kSize * i).first() == 0xFFFF) {
      CopyPair(base, out, pairs + Pair::kSize * i - Pair::kSize);
      return;
    }
  }
  CopyPair(base, out, schema::AccessorPairs(base, pairs).pair4_first_address());
}

void PairsAt(uint8_t* base, uint32_t out, uint32_t pairs, uint32_t i) {
  ITC_NATIVE_RUNNING("sub_820FFF10 Pairs::At");
  if (int32_t(i) < 5) {
    CopyPair(base, out, pairs + Pair::kSize * i);
    return;
  }
  Pair(base, out).set_both(0xFFFFFFFFu);
}

void ListInit(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_82593FB0 List::Init");
  List(base, list).set_first(0);
  List(base, list).set_count(0);
}

uint32_t ListAt(uint8_t* base, uint32_t list, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_82593FC0 List::At");
  const ListNode first = List(base, list).first();
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
int32_t Claim(uint8_t* base, uint32_t list) {
  const int32_t count = int32_t(List(base, list).count());
  for (int32_t i = 0; i < count; i++) {
    const ListNode object(base, ListAt(base, list, uint32_t(i)));
    if (object.state() == 0) {
      object.set_state(3);
      return i;
    }
  }
  return -1;
}

uint32_t GetInUse(uint8_t* base, uint32_t list, uint32_t index) {
  if (int32_t(ListNode(base, ListAt(base, list, index)).state()) != 3) {
    return 0;
  }
  return ListAt(base, list, index);
}

}  // namespace

int32_t ScenePoolsClaimA(uint8_t* base, uint32_t scene, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_820C42C8 ScenePools::ClaimA");
  return Claim(base, schema::ScenePools(base, scene).pools_address() + 12 * pool);
}

int32_t ScenePoolsClaimB(uint8_t* base, uint32_t scene, uint32_t pool) {
  ITC_NATIVE_RUNNING("sub_820C5AB0 ScenePools::ClaimB");
  return Claim(base, schema::CarScene(base, scene).car_models_first_address() + 12 * pool);
}

uint32_t ScenePoolsGetA(uint8_t* base, uint32_t scene, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C4420 ScenePools::GetA");
  return GetInUse(base, schema::ScenePools(base, scene).pools_address() + 12 * pool, index);
}

uint32_t ScenePoolsGetB(uint8_t* base, uint32_t scene, uint32_t pool, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C5C20 ScenePools::GetB");
  return GetInUse(base, schema::CarScene(base, scene).car_models_first_address() + 12 * pool, index);
}

uint32_t ScenePoolsFindModel(uint8_t* base, uint32_t scene, uint32_t kind, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_820C5D70 ScenePools::FindModel");
  const schema::CarScene car_scene(base, scene);
  const int32_t count = int32_t(car_scene.scene_models_count());
  int32_t found = 0;
  for (int32_t i = 0; i < count; i++) {
    const ListNode object(base, ListAt(base, car_scene.scene_models_first_address(), uint32_t(i)));
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
