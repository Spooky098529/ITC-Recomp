#include "vehicle_accessors/vehicle_accessors.h"

#include "guest.h"

#include "base/log.h"
#include "vehicle_accessors/vectors.h"

#include <bit>
#include <cmath>

namespace vehicle_accessors {

namespace {

constexpr uint32_t kVehicles = 0x826C2E34;   // the vehicle array's pointer
constexpr uint32_t kSaveData = 0x826C5B58;   // the players' save data pointers
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kOne = 0x82000A30;        // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;       // the game's float 0.0
constexpr uint32_t kSlipScale = 0x82002BD8;

namespace vl = vector_library;

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}

// The rotation of `v` by the matrix at `m` (Matrix::Rotate's arithmetic),
// w 0.0.
vl::Vector Rotate(uint8_t* base, uint32_t m, const vl::Vector& v) {
  auto at = [&](uint32_t offset) { return Lfs(base, m + offset); };
  auto muls = [](double a, double b) { return double(float(a * b)); };
  auto madds = [](double a, double b, double c) { return double(float(std::fma(a, b, c))); };
  const double x0 = madds(at(16), v.y, muls(at(32), v.z));
  const double y0 = madds(at(4), v.x, muls(at(20), v.y));
  const double z0 = madds(at(8), v.x, muls(at(24), v.y));
  return {float(madds(v.x, at(0), x0)), float(madds(at(36), v.z, y0)),
          float(madds(at(40), v.z, z0)), 0.0f};
}

// RigidBody::PointVelocity's value: +32 + +96 x (point - +16).
vl::Vector PointVelocityCross(uint8_t* base, uint32_t body, uint32_t point) {
  const vl::Vector d = vl::Sub(vl::LoadVector(base, point), vl::LoadVector(base, body + 16));
  return vl::Cross(vl::LoadVector(base, body + 96), d);
}

}  // namespace

uint32_t VehicleGet(uint8_t* base, uint32_t index) {
  ITC_NATIVE_RUNNING("sub_820C0468 Vehicle::Get");
  const uint32_t vehicles = guest::LoadU32(base, kVehicles);
  if (vehicles == 0) {
    return 0;
  }
  return vehicles + kVehicleBytes * index;
}

uint32_t VehicleWheelContact(uint32_t vehicle, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_820C0170 Vehicle::WheelContact");
  return vehicle + 5328 + 96 * wheel;
}

uint32_t RigidBodyPosition(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B67D8 RigidBody::Position");
  return body + 208;
}

uint32_t RigidBodyMatrix(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6930 RigidBody::Matrix");
  return body + 288;
}

uint32_t ContactObject(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8CC0 Contact::Object");
  if (guest::LoadU32(base, contact + 4) & 0xFF000000u) {
    return 0;
  }
  return guest::LoadU32(base, contact);
}

uint32_t ContactPosition(uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D00 Contact::Position");
  return contact + 16;
}

float ContactStrength(uint8_t* base, uint32_t contact) {
  ITC_NATIVE_RUNNING("sub_820B8D08 Contact::Strength");
  return guest::LoadFloat(base, contact + 64);
}

int32_t RacersIndexOf(uint8_t* base, uint32_t records, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820FF9F8 Racers::IndexOf");
  for (uint32_t i = 0; i < 93; i++) {
    if (guest::LoadU32(base, records + 3912 + 3936 * i) == id) {
      return int32_t(i);
    }
  }
  return -1;
}

uint32_t StagesHasRacer(uint8_t* base, uint32_t stages, uint32_t racer, uint32_t stage) {
  ITC_NATIVE_RUNNING("sub_820FFCF8 Stages::HasRacer");
  if (int32_t(stage) == -1) {
    stage = guest::LoadU32(base, stages + 804);
  }
  const uint32_t slots = stages + 80 * stage + 44;
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
  return guest::LoadU32(base, kSaveData + 4 * player);
}

uint32_t DebugSwitchOff() {
  ITC_NATIVE_RUNNING("sub_825C8388 DebugSwitchOff");
  return 0;
}

uint32_t RigidBodyIsFixed(uint8_t* base, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6638 RigidBody::IsFixed");
  return guest::LoadU8(base, body);
}

void RigidBodySetGravity(uint8_t* base, uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6640 RigidBody::SetGravity");
  vl::Vec4Copy(base, body + 480, vector);
  const double mass = Lfs(base, body + 4);
  vl::StoreVector(base, body + 496, vl::Scale(vl::LoadVector(base, vector), mass));
}

void RigidBodySetMass(uint8_t* base, uint32_t body, double mass) {
  ITC_NATIVE_RUNNING("sub_820B6698 RigidBody::SetMass");
  const uint8_t fixed = guest::LoadU8(base, body);
  guest::StoreFloat(base, body + 4, float(mass));
  guest::StoreFloat(base, body + 8, float(Lfs(base, kOne) / mass));
  if (fixed != 0) {
    guest::StoreU32(base, body + 8, guest::LoadU32(base, kZero));
  }
  vl::StoreVector(base, body + 496, vl::Scale(vl::LoadVector(base, body + 480), mass));
}

float RigidBodyMass(uint8_t* base, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6708 RigidBody::Mass");
  return guest::LoadFloat(base, body + 4);
}

void RigidBodySetCenterOfMass(uint8_t* base, uint32_t body, uint32_t offset) {
  ITC_NATIVE_RUNNING("sub_820B67E0 RigidBody::SetCenterOfMass");
  vl::Vec4Copy(base, body + 192, offset);
  const vl::Vector rotated = Rotate(base, body + 224, vl::LoadVector(base, body + 192));
  const vl::Vector center = vl::Add(vl::LoadVector(base, body + 208), rotated);
  // Matrix::SetTranslation on the matrix at +224, then +16 = its fourth
  // row's x, y, z with w 0.0.
  guest::StoreFloat(base, body + 272, center.x);
  guest::StoreFloat(base, body + 276, center.y);
  guest::StoreFloat(base, body + 280, center.z);
  guest::StoreU32(base, body + 284, guest::LoadU32(base, kOne));
  for (uint32_t i = 0; i < 12; i += 4) {
    guest::StoreU32(base, body + 16 + i, guest::LoadU32(base, body + 272 + i));
  }
  guest::StoreU32(base, body + 28, guest::LoadU32(base, kZero));
}

uint32_t RigidBodyField80(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6880 RigidBody::Field80");
  return body + 80;
}

uint32_t RigidBodyVelocity(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6888 RigidBody::Velocity");
  return body + 32;
}

uint32_t RigidBodyField144(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6890 RigidBody::Field144");
  return body + 144;
}

uint32_t RigidBodyField160(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6898 RigidBody::Field160");
  return body + 160;
}

uint32_t RigidBodyOrientation(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6928 RigidBody::Orientation");
  return body + 224;
}

uint32_t RigidBodyAngularVelocity(uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B6948 RigidBody::AngularVelocity");
  return body + 96;
}

float RigidBodyFloat48(uint8_t* base, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_820B68A0 RigidBody::Float48");
  if (guest::LoadU8(base, body) != 0) {
    return guest::LoadFloat(base, kZero);
  }
  return guest::LoadFloat(base, body + 48);
}

void RigidBodyPointVelocity(uint8_t* base, uint32_t out, uint32_t body, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B68C0 RigidBody::PointVelocity");
  const vl::Vector spin = PointVelocityCross(base, body, point);
  // Vec3::Sum: out's w = 0.0, then Vec3::Add's steps.
  guest::StoreU32(base, out + 12, guest::LoadU32(base, kZero));
  vl::AddValue(base, out, body + 32, spin, false);
}

void RigidBodySetFloat112(uint8_t* base, uint32_t body, double value) {
  ITC_NATIVE_RUNNING("sub_820B6950 RigidBody::SetFloat112");
  guest::StoreFloat(base, body + 112, float(value));
}

void RigidBodyAddForce(uint8_t* base, uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6958 RigidBody::AddForce");
  vl::Vec3AddTo(base, body + 64, vector);
}

void RigidBodyAddTorque(uint8_t* base, uint32_t body, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_820B6960 RigidBody::AddTorque");
  vl::Vec3AddTo(base, body + 128, vector);
}

namespace {

// AddForceAt's and AddTorqueAt's torque: +128 += (point - +16) x force.
void AddTorqueOf(uint8_t* base, uint32_t body, uint32_t force, uint32_t point) {
  const vl::Vector d = vl::Sub(vl::LoadVector(base, point), vl::LoadVector(base, body + 16));
  const vl::Vector torque = vl::Cross(d, vl::LoadVector(base, force));
  vl::AddValue(base, body + 128, body + 128, torque, false);
}

}  // namespace

void RigidBodyAddForceAt(uint8_t* base, uint32_t body, uint32_t force, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B6968 RigidBody::AddForceAt");
  AddTorqueOf(base, body, force, point);
  vl::Vec3AddTo(base, body + 64, force);
}

void RigidBodyAddTorqueAt(uint8_t* base, uint32_t body, uint32_t force, uint32_t point) {
  ITC_NATIVE_RUNNING("sub_820B69D8 RigidBody::AddTorqueAt");
  AddTorqueOf(base, body, force, point);
}

float VehicleWheelSlip(uint8_t* base, uint32_t vehicle, uint32_t wheel, double threshold) {
  ITC_NATIVE_RUNNING("sub_820C0180 Vehicle::WheelSlip");
  const uint32_t at = vehicle + 368 * wheel;
  const uint32_t record = at + 416;
  if (guest::LoadU8(base, at + 433) == 0) {
    return guest::LoadFloat(base, kZero);
  }
  // The wheel's spin times its radius (probably), along the first axis.
  const uint32_t wheel_data = guest::LoadU32(base, vehicle + 4 * (wheel + 540));
  const float rolled =
      float(Lfs(base, wheel_data + 52) * Lfs(base, record + 248));
  const vl::Vector zero_rolled = {rolled, 0.0f, 0.0f, 0.0f};
  // The body's velocity at the wheel (RigidBody::PointVelocity).
  const uint32_t body = guest::LoadU32(base, vehicle + 336);
  const vl::Vector velocity =
      vl::Add(vl::LoadVector(base, body + 32), PointVelocityCross(base, body, at + 480));
  const vl::Vector along = {vl::Dot(velocity, vl::LoadVector(base, record + 144)), 0.0f,
                            vl::Dot(velocity, vl::LoadVector(base, record + 128)), 0.0f};
  const vl::Vector slip = vl::Sub(along, zero_rolled);
  const double length = double(float(std::sqrt(double(vl::Dot(slip, slip)))));
  const double least = double(float(threshold * Lfs(base, kSlipScale)));
  if (length > least) {
    return float(length);
  }
  return 0.0f;
}

float VehicleMaxWheelSlip(uint8_t* base, uint32_t vehicle, double threshold) {
  ITC_NATIVE_RUNNING("sub_820C0290 Vehicle::MaxWheelSlip");
  float most = guest::LoadFloat(base, kZero);
  for (uint32_t wheel = 0; wheel < 4; wheel++) {
    const float slip = VehicleWheelSlip(base, vehicle, wheel, threshold);
    if (!(double(most) >= double(slip))) {
      most = slip;
    }
  }
  return most;
}

}  // namespace vehicle_accessors
