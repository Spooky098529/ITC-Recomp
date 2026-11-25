// The rigid body's updates and setters (docs/functions/car-physics.md):
// see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "matrix_math/matrix_math.h"
#include "memory/host_copy.h"
#include "schema/car_physics.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <cstring>

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace mm = matrix_math;
namespace va = vehicle_accessors;

void RigidBodyUpdateOrigin(uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6E70 RigidBody::UpdateOrigin");
  const uint32_t f = frame;
  // +16 = the fourth row of +224, w 0.0.
  CopyWord(kZero, f + 92);
  CopyWord(schema::RigidBody(body).orientation_matrix_address() + 48, f + 80);
  CopyWord(schema::RigidBody(body).orientation_matrix_address() + 52, f + 84);
  CopyWord(schema::RigidBody(body).orientation_matrix_address() + 56, f + 88);
  vl::Vec4Copy(schema::RigidBody(body).center_of_mass_address(), f + 80);
  // +208 = +16 - +192 rotated by +224.
  vl::MatrixRotate(f + 112, schema::RigidBody(body).orientation_matrix_address(), schema::RigidBody(body).center_of_mass_offset_address());
  vl::Vec3Difference(f + 128, schema::RigidBody(body).center_of_mass_address(), f + 112);
  vl::Vec4Copy(schema::RigidBody(body).origin_address(), f + 128);
  // The body matrix's fourth row = +208, w 1.0.
  CopyWord(kOne, f + 108);
  CopyWord(schema::RigidBody(body).origin_address(), f + 96);
  CopyWord(schema::RigidBody(body).origin_address() + 4, f + 100);
  CopyWord(schema::RigidBody(body).origin_address() + 8, f + 104);
  vl::Vec4Copy(schema::RigidBody(body).matrix_address() + 48, f + 96);
}

uint32_t RigidBodyUpdateVelocity(uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B70C8 RigidBody::UpdateVelocity");
  const uint32_t f = frame;
  // +32 = the momentum x the inverse mass.
  vl::Vec4Scaled(f + 96, schema::RigidBody(body).momentum_address(), double(schema::RigidBody(body).inverse_mass()));
  vl::Vec4Copy(schema::RigidBody(body).velocity_address(), f + 96);
  const double speed = double(vl::Vec3Length(schema::RigidBody(body).velocity_address()));
  schema::RigidBody(body).set_speed(float(speed));
  // +144 = the direction, (1, 0, 0, 0) when still.
  uint32_t direction;
  if (speed == Lfs(kZero)) {
    CopyWord(kOne, f + 80);
    CopyWord(kZero, f + 84);
    CopyWord(kZero, f + 88);
    CopyWord(kZero, f + 92);
    direction = f + 80;
  } else {
    const double one = Lfs(kOne);
    vl::Vec4Scaled(f + 96, schema::RigidBody(body).velocity_address(), Fdivs(one, speed));
    direction = f + 96;
  }
  vl::Vec4Copy(schema::RigidBody(body).velocity_direction_address(), direction);
  // Above the speed cap: the velocity and momentum scaled down to it.
  const double cap = double(schema::RigidBody(body).speed_cap());
  if (!(double(schema::RigidBody(body).speed()) > cap)) {
    return schema::RigidBody(body).velocity_direction_address();
  }
  vl::Vec4ScaledFrom(f + 96, schema::RigidBody(body).velocity_direction_address(), cap);
  vl::Vec4Copy(schema::RigidBody(body).velocity_address(), f + 96);
  vl::Vec4Scaled(f + 96, schema::RigidBody(body).velocity_address(), double(schema::RigidBody(body).mass()));
  vl::Vec4Copy(schema::RigidBody(body).momentum_address(), f + 96);
  CopyWord(schema::RigidBody(body).speed_cap_address(), schema::RigidBody(body).speed_address());
  return schema::RigidBody(body).momentum_address();
}

double RigidBodyUpdateAngularVelocity(uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7478 RigidBody::UpdateAngularVelocity");
  const uint32_t f = frame;
  vl::MatrixRotate(f + 80, schema::RigidBody(body).world_inverse_inertia_address(), schema::RigidBody(body).angular_momentum_address());
  vl::Vec4Copy(schema::RigidBody(body).angular_velocity_address(), f + 80);
  const double spin2 = double(vl::Vec3Dot(schema::RigidBody(body).angular_velocity_address(), schema::RigidBody(body).angular_velocity_address()));
  const double cap = double(schema::RigidBody(body).spin_cap());
  if (!(spin2 > Fmuls(cap, cap))) {
    return spin2;
  }
  const double spin = double(vl::Sqrtf(spin2));
  const double scale = Fdivs(double(schema::RigidBody(body).spin_cap()), spin);
  vl::Vec4ScaleBy(schema::RigidBody(body).angular_momentum_address(), scale);
  vl::Vec4ScaleBy(schema::RigidBody(body).angular_velocity_address(), scale);
  return scale;
}

void RigidBodyUpdateOrientation(uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6F38 RigidBody::UpdateOrientation");
  const uint32_t f = frame;
  // Three matrices on the stack: R at f + 80, the inertia's diagonal at
  // f + 144, R^T at f + 208.
  mm::SetIdentity(f + 80);
  mm::SetIdentity(f + 208);
  mm::SetIdentity(f + 144);
  mm::FromQuaternion(schema::RigidBody(body).orientation_matrix_address(), schema::RigidBody(body).orientation_address());
  vl::Vec4Copy(schema::RigidBody(body).matrix_address(), schema::RigidBody(body).orientation_matrix_address());
  vl::Vec4Copy(schema::RigidBody(body).matrix_address() + 16, schema::RigidBody(body).orientation_matrix_address() + 16);
  vl::Vec4Copy(schema::RigidBody(body).matrix_address() + 32, schema::RigidBody(body).orientation_matrix_address() + 32);
  CopyWord(schema::RigidBody(body).inverse_inertia_address(), f + 144);
  CopyWord(schema::RigidBody(body).inverse_inertia_address() + 4, f + 164);
  const uint32_t m00 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address());
  const uint32_t m01 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 4);
  const uint32_t m02 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 8);
  const uint32_t m11 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 20);
  const uint32_t m12 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 24);
  const uint32_t m21 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 36);
  const uint32_t m22 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 40);
  const uint32_t i2 = guest::LoadU32(schema::RigidBody(body).inverse_inertia_address() + 8);
  const uint32_t m10 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 16);
  const uint32_t m20 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 32);
  const uint32_t zero = guest::LoadU32(kZero);
  const uint32_t one = guest::LoadU32(kOne);
  for (uint32_t at : {92u, 108u, 124u, 128u, 132u, 136u}) {
    guest::StoreU32(f + at, zero);
  }
  guest::StoreU32(f + 140, one);
  for (uint32_t at : {220u, 236u, 252u}) {
    guest::StoreU32(f + at, zero);
  }
  guest::StoreU32(f + 184, i2);
  for (uint32_t at : {256u, 260u, 264u}) {
    guest::StoreU32(f + at, zero);
  }
  guest::StoreU32(f + 268, one);
  for (uint32_t at : {148u, 152u, 156u, 160u, 168u, 172u, 176u, 180u, 188u, 192u, 196u, 200u}) {
    guest::StoreU32(f + at, zero);
  }
  guest::StoreU32(f + 204, one);
  // R's upper 3x3.
  guest::StoreU32(f + 80, m00);
  guest::StoreU32(f + 84, m01);
  guest::StoreU32(f + 88, m02);
  guest::StoreU32(f + 96, m10);
  guest::StoreU32(f + 100, m11);
  guest::StoreU32(f + 104, m12);
  guest::StoreU32(f + 112, m20);
  guest::StoreU32(f + 116, m21);
  guest::StoreU32(f + 120, m22);
  // R^T's.
  guest::StoreU32(f + 208, m00);
  guest::StoreU32(f + 212, m10);
  guest::StoreU32(f + 216, m20);
  guest::StoreU32(f + 224, m01);
  guest::StoreU32(f + 228, m11);
  guest::StoreU32(f + 232, m21);
  guest::StoreU32(f + 240, m02);
  guest::StoreU32(f + 244, m12);
  guest::StoreU32(f + 248, m22);
  // +416 = R^T (diag R).
  mm::MultiplyReversed(schema::RigidBody(body).world_inverse_inertia_address(), f + 80, f + 144);
  mm::MultiplyReversed(schema::RigidBody(body).world_inverse_inertia_address(), schema::RigidBody(body).world_inverse_inertia_address(), f + 208);
}

void RigidBodySetOrientation(uint32_t body, uint32_t quaternion, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B71E8 RigidBody::SetOrientation");
  // ld and std of the two doublewords.
  for (uint32_t at : {0u, 8u}) {
    const uint32_t hi = guest::LoadU32(quaternion + at);
    const uint32_t lo = guest::LoadU32(quaternion + at + 4);
    guest::StoreU32(schema::RigidBody(body).orientation_address() + at, hi);
    guest::StoreU32(schema::RigidBody(body).orientation_address() + 4 + at, lo);
  }
  mm::QuaternionNormalizeSelf(schema::RigidBody(body).orientation_address());
  RigidBodyUpdateOrientation(body, frame - 320);
  RigidBodyUpdateOrigin(body, frame - 176);
}

void RigidBodySetCenterOfMassPosition(uint32_t body, uint32_t point,
                                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B71B0 RigidBody::SetCenterOfMassPosition");
  vl::MatrixSetTranslation(schema::RigidBody(body).orientation_matrix_address(), point);
  RigidBodyUpdateOrigin(body, frame - 176);
}

void RigidBodySetPosition(uint32_t body, uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6710 RigidBody::SetPosition");
  const uint32_t f = frame;
  const uint32_t zero = guest::LoadU32(kZero);
  guest::StoreU32(f + 124, zero);
  vl::MatrixSetTranslation(schema::RigidBody(body).matrix_address(), position);
  // +208 = the body matrix's translation, w 0.0.
  CopyWord(schema::RigidBody(body).matrix_address() + 48, f + 80);
  CopyWord(schema::RigidBody(body).matrix_address() + 52, f + 84);
  const uint32_t z = guest::LoadU32(schema::RigidBody(body).matrix_address() + 56);
  guest::StoreU32(f + 92, zero);
  guest::StoreU32(f + 88, z);
  vl::Vec4Copy(schema::RigidBody(body).origin_address(), f + 80);
  // +224's fourth row = the position + the offset rotated by +224.
  vl::MatrixRotate(f + 128, schema::RigidBody(body).orientation_matrix_address(), schema::RigidBody(body).center_of_mass_offset_address());
  vl::Vec4Copy(f + 112, f + 128);
  vl::Vec3Sum(f + 128, position, f + 112);
  vl::MatrixSetTranslation(schema::RigidBody(body).orientation_matrix_address(), f + 128);
  // +16 = that, w 0.0.
  CopyWord(schema::RigidBody(body).orientation_matrix_address() + 48, f + 96);
  CopyWord(schema::RigidBody(body).orientation_matrix_address() + 52, f + 100);
  const uint32_t z2 = guest::LoadU32(schema::RigidBody(body).orientation_matrix_address() + 56);
  guest::StoreU32(f + 108, zero);
  guest::StoreU32(f + 104, z2);
  vl::Vec4Copy(schema::RigidBody(body).center_of_mass_address(), f + 96);
}

uint32_t RigidBodySetVelocity(uint32_t body, uint32_t velocity, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7238 RigidBody::SetVelocity");
  vl::Vec4Scaled(frame + 80, velocity, double(schema::RigidBody(body).mass()));
  vl::Vec4Copy(schema::RigidBody(body).momentum_address(), frame + 80);
  return RigidBodyUpdateVelocity(body, frame - 160);
}

double RigidBodySetAngularVelocity(uint32_t body, uint32_t velocity,
                                   uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7500 RigidBody::SetAngularVelocity");
  const uint32_t f = frame;
  const uint32_t zero = guest::LoadU32(kZero);
  guest::StoreU32(f + 92, zero);
  guest::StoreU32(f + 108, zero);
  // The velocity in the body's axes, times the inertia, back to the world.
  mm::InverseRotate(f + 112, schema::RigidBody(body).orientation_matrix_address(), velocity);
  vl::Vec4Copy(f + 80, f + 112);
  {
    const double i0 = Lfs(schema::RigidBody(body).inertia_address());
    const double v0 = Lfs(f + 80);
    Stfs(f + 96, Fmuls(i0, v0));
  }
  {
    const double i1 = Lfs(schema::RigidBody(body).inertia_address() + 4);
    const double v1 = Lfs(f + 84);
    Stfs(f + 100, Fmuls(i1, v1));
  }
  {
    const double i2 = Lfs(schema::RigidBody(body).inertia_address() + 8);
    const double v2 = Lfs(f + 88);
    Stfs(f + 104, Fmuls(i2, v2));
  }
  vl::MatrixRotate(f + 112, schema::RigidBody(body).orientation_matrix_address(), f + 96);
  vl::Vec4Copy(schema::RigidBody(body).angular_momentum_address(), f + 112);
  return RigidBodyUpdateAngularVelocity(body, f - 144);
}

void RigidBodySetInertia(uint32_t body, uint32_t inertia, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7280 RigidBody::SetInertia");
  CopyWord(schema::PhysicsVector(inertia).x_address(), schema::RigidBody(body).inertia_address());
  CopyWord(schema::PhysicsVector(inertia).y_address(), schema::RigidBody(body).inertia_address() + 4);
  CopyWord(schema::PhysicsVector(inertia).z_address(), schema::RigidBody(body).inertia_address() + 8);
  const double one = Lfs(kOne);
  const double x = Lfs(schema::RigidBody(body).inertia_address());
  const double y = Lfs(schema::RigidBody(body).inertia_address() + 4);
  const double rx = Fdivs(one, x);
  const double z = Lfs(schema::RigidBody(body).inertia_address() + 8);
  const double ry = Fdivs(one, y);
  const double rz = Fdivs(one, z);
  Stfs(schema::RigidBody(body).inverse_inertia_address(), rx);
  Stfs(schema::RigidBody(body).inverse_inertia_address() + 4, ry);
  Stfs(schema::RigidBody(body).inverse_inertia_address() + 8, rz);
  RigidBodyUpdateOrientation(body, frame);
}

uint32_t RigidBodyApplyImpulses(uint32_t body, double dt, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B75B8 RigidBody::ApplyImpulses");
  if (schema::RigidBody(body).fixed() != 0) {
    return body;
  }
  const uint32_t f = frame;
  vl::Vec4Scaled(f + 80, schema::RigidBody(body).torque_address(), dt);
  vl::Vec3AddTo(schema::RigidBody(body).angular_momentum_address(), f + 80);
  vl::Vec4Copy(schema::RigidBody(body).torque_address(), kZeroVector);
  RigidBodyUpdateAngularVelocity(body, f - 144);
  vl::Vec4Scaled(f + 80, schema::RigidBody(body).force_address(), dt);
  vl::Vec3AddTo(schema::RigidBody(body).momentum_address(), f + 80);
  vl::Vec4Copy(schema::RigidBody(body).force_address(), kZeroVector);
  return RigidBodyUpdateVelocity(body, f - 160);
}

uint32_t RigidBodyIntegrate(uint32_t body, double dt, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7658 RigidBody::Integrate");
  if (schema::RigidBody(body).fixed() != 0) {
    return body;
  }
  const uint32_t f = frame;
  // The torque into the angular momentum, damped.
  vl::Vec4Scaled(f + 80, schema::RigidBody(body).torque_address(), dt);
  vl::Vec3AddTo(schema::RigidBody(body).angular_momentum_address(), f + 80);
  const double angular_damping = double(schema::RigidBody(body).angular_damping());
  const double zero = Lfs(kZero);
  const double one = Lfs(kOne);
  if (angular_damping > zero) {
    vl::Vec4ScaleBy(schema::RigidBody(body).angular_momentum_address(), Fnmsubs(angular_damping, dt, one));
  }
  vl::Vec4Copy(schema::RigidBody(body).torque_address(), kZeroVector);
  RigidBodyUpdateAngularVelocity(body, f - 144);
  // The orientation turned by the spin over dt.
  const double spin = double(vl::Vec3Length(schema::RigidBody(body).angular_velocity_address()));
  if (spin > zero) {
    vl::Vec4Scaled(f + 80, schema::RigidBody(body).angular_velocity_address(), Fdivs(one, spin));
    mm::QuaternionFromAxisAngle(f + 96, f + 80, Fmuls(spin, dt), f - 32);
    mm::QuaternionMultiply(f + 112, f + 96, schema::RigidBody(body).orientation_address());
    RigidBodySetOrientation(body, f + 112, f - 96);
    RigidBodyUpdateOrientation(body, f - 320);
  }
  // Gravity into the force, the force into the momentum, damped.
  vl::Vec3AddTo(schema::RigidBody(body).force_address(), schema::RigidBody(body).weight_address());
  vl::Vec4Scaled(f + 112, schema::RigidBody(body).force_address(), dt);
  vl::Vec3AddTo(schema::RigidBody(body).momentum_address(), f + 112);
  const double damping = double(schema::RigidBody(body).linear_damping());
  if (damping > zero) {
    vl::Vec4ScaleBy(schema::RigidBody(body).momentum_address(), Fnmsubs(damping, dt, one));
  }
  vl::Vec4Copy(schema::RigidBody(body).force_address(), kZeroVector);
  RigidBodyUpdateVelocity(body, f - 160);
  // The center of mass moved by the velocity.
  vl::Vec4Scaled(f + 112, schema::RigidBody(body).velocity_address(), dt);
  vl::Vec3AddTo(schema::RigidBody(body).orientation_matrix_address() + 48, f + 112);
  RigidBodyUpdateOrigin(body, f - 176);
  return schema::RigidBody(body).matrix_address() + 48;
}

void RigidBodyReset(uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B77D0 RigidBody::Reset");
  const uint32_t f = frame;
  // memset (sub_825A71D0), as its adapter does it (a host body through its
  // handle, E5 step 4).
  if (!memory::FillWithHandles(body, 0, 528)) {
    guest::ForEachHostPart(body, 528, [](uint8_t* host, uint32_t, uint32_t bytes) {
      std::memset(host, 0, bytes);
    });
  }
  schema::RigidBody(body).set_fixed(uint8_t(0));
  constexpr uint32_t kCap = 0x82002A90;
  constexpr uint32_t kGravity = 0x82002A8C;
  constexpr uint32_t kInertia = 0x82002A10;
  constexpr uint32_t kIdentityQuaternion = 0x826381B0;
  const uint32_t cap = guest::LoadU32(kCap);
  guest::StoreU32(schema::RigidBody(body).speed_cap_address(), cap);
  vl::Vec4Copy(schema::RigidBody(body).force_address(), kZeroVector);
  const uint32_t zero = guest::LoadU32(kZero);
  guest::StoreU32(schema::RigidBody(body).linear_damping_address(), zero);
  guest::StoreU32(schema::RigidBody(body).speed_address(), zero);
  vl::Vec4Copy(schema::RigidBody(body).momentum_address(), kZeroVector);
  va::RigidBodySetMass(body, Lfs(kOne));
  guest::StoreU32(f + 80, zero);
  guest::StoreU32(f + 88, zero);
  guest::StoreU32(f + 92, zero);
  CopyWord(kGravity, f + 84);
  va::RigidBodySetGravity(body, f + 80);
  RigidBodySetVelocity(body, kZeroVector, f - 112);
  guest::StoreU32(schema::RigidBody(body).spin_cap_address(), cap);
  vl::Vec4Copy(schema::RigidBody(body).torque_address(), kZeroVector);
  guest::StoreU32(schema::RigidBody(body).angular_damping_address(), zero);
  vl::Vec4Copy(schema::RigidBody(body).center_of_mass_offset_address(), kZeroVector);
  vl::Vec4Copy(schema::RigidBody(body).angular_momentum_address(), kZeroVector);
  mm::SetIdentity(schema::RigidBody(body).orientation_matrix_address());
  guest::StoreU32(f + 108, zero);
  const uint32_t inertia = guest::LoadU32(kInertia);
  guest::StoreU32(f + 96, inertia);
  guest::StoreU32(f + 100, inertia);
  guest::StoreU32(f + 104, inertia);
  // SetInertia has no frame of its own: its tail call's is below this one.
  RigidBodySetInertia(body, f + 96, f - 320);
  RigidBodySetOrientation(body, kIdentityQuaternion, f - 96);
  va::RigidBodySetCenterOfMass(body, kZeroVector);
  vl::MatrixSetTranslation(schema::RigidBody(body).orientation_matrix_address(), kZeroVector);
  RigidBodyUpdateOrigin(body, f - 176);
  RigidBodySetAngularVelocity(body, kZeroVector, f - 160);
  schema::RigidBody(body).set_vehicle_matrix(uint32_t(0));
  schema::RigidBody(body).set_flags(uint32_t(0));
}

void RigidBodyConstruct(uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7928 RigidBody::RigidBody");
  const uint32_t zero = guest::LoadU32(kZero);
  // The vectors' w words, in the original's order.
  const schema::RigidBody rb(body);
  guest::StoreU32(rb.center_of_mass_address() + 12, zero);
  guest::StoreU32(rb.velocity_address() + 12, zero);
  guest::StoreU32(rb.force_address() + 12, zero);
  guest::StoreU32(rb.angular_velocity_address() + 12, zero);
  guest::StoreU32(rb.torque_address() + 12, zero);
  guest::StoreU32(rb.velocity_direction_address() + 12, zero);
  guest::StoreU32(rb.momentum_address() + 12, zero);
  guest::StoreU32(rb.center_of_mass_offset_address() + 12, zero);
  guest::StoreU32(rb.origin_address() + 12, zero);
  guest::StoreU32(rb.inertia_address() + 12, zero);
  guest::StoreU32(rb.inverse_inertia_address() + 12, zero);
  guest::StoreU32(rb.angular_momentum_address() + 12, zero);
  guest::StoreU32(rb.gravity_address() + 12, zero);
  guest::StoreU32(rb.weight_address() + 12, zero);
  RigidBodyReset(body, frame - 176);
}

namespace {

// EffectiveMassAt's term for one axis: (r x e) scaled by the inverse
// inertia's diagonal at `cross` (the original's order of loads and
// multiplies: `first` puts the cross product's word first in x's).
void ScaleByInverseInertia(uint32_t body, uint32_t cross, bool first) {
  if (first) {
    const double x = Lfs(cross + 0);
    const double ix = Lfs(schema::RigidBody(body).inverse_inertia_address());
    const double sx = Fmuls(x, ix);
    const double y = Lfs(cross + 4);
    const double iy = Lfs(schema::RigidBody(body).inverse_inertia_address() + 4);
    const double sy = Fmuls(y, iy);
    Stfs(cross + 0, sx);
    Stfs(cross + 4, sy);
    const double z = Lfs(cross + 8);
    const double iz = Lfs(schema::RigidBody(body).inverse_inertia_address() + 8);
    Stfs(cross + 8, Fmuls(z, iz));
    return;
  }
  {
    const double x = Lfs(cross + 0);
    const double ix = Lfs(schema::RigidBody(body).inverse_inertia_address());
    Stfs(cross + 0, Fmuls(x, ix));
  }
  {
    const double y = Lfs(cross + 4);
    const double iy = Lfs(schema::RigidBody(body).inverse_inertia_address() + 4);
    Stfs(cross + 4, Fmuls(y, iy));
  }
  const double z = Lfs(cross + 8);
  const double iz = Lfs(schema::RigidBody(body).inverse_inertia_address() + 8);
  Stfs(cross + 8, Fmuls(z, iz));
}

}  // namespace

double RigidBodyEffectiveMassAt(uint32_t body, uint32_t out, uint32_t point,
                                uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6A38 RigidBody::EffectiveMassAt");
  const uint32_t f = frame;
  // The world axes: x at f + 176, y at f + 128, z at f + 144.
  const uint32_t one = guest::LoadU32(kOne);
  guest::StoreU32(f + 176, one);
  guest::StoreU32(f + 132, one);
  guest::StoreU32(f + 152, one);
  const uint32_t zero = guest::LoadU32(kZero);
  for (uint32_t at : {180u, 184u, 188u, 128u, 136u, 140u, 144u, 148u, 156u}) {
    guest::StoreU32(f + at, zero);
  }
  const double unit = Lfs(kOne);
  // r = point - the center of mass, at f + 160.
  vl::Vec3Difference(f + 160, point, schema::RigidBody(body).center_of_mass_address());
  struct Axis {
    uint32_t axis, cross, scaled, sum, out;
  };
  // Each axis: the axis x the inverse mass, plus ((r x e) I^-1) x r, dotted
  // with e (the temporaries move between the original's slots).
  const Axis axes[] = {{f + 176, f + 96, f + 192, f + 208, out + 0},
                       {f + 128, f + 80, f + 208, f + 192, out + 4},
                       {f + 144, f + 112, f + 208, f + 192, out + 8}};
  double dot = 0.0;
  for (int i = 0; i < 3; i++) {
    const Axis& a = axes[i];
    vl::Vec4Scaled(a.scaled, a.axis, double(schema::RigidBody(body).inverse_mass()));
    vl::Vec3Cross(a.cross, f + 160, a.axis);
    ScaleByInverseInertia(body, a.cross, i == 0);
    vl::Vec3Cross(f + 224, a.cross, f + 160);
    vl::Vec4Copy(a.cross, f + 224);
    vl::Vec3Sum(a.sum, a.scaled, a.cross);
    dot = double(vl::Vec3DotOf(a.sum, a.axis));
    Stfs(a.out, Fdivs(unit, dot));
  }
  return dot;
}

double RigidBodyContactImpulse(uint32_t body, uint32_t point, uint32_t normal,
                               uint32_t frame, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B6C78 RigidBody::ContactImpulse");
  const uint32_t f = frame;
  va::RigidBodyPointVelocity(f + 112, body, point);
  const double vn = Fneg(double(vl::Vec3DotOf(f + 112, normal)));
  const double zero = Lfs(kZero);
  if (vn < zero) {
    if (r3) {
      *r3 = f + 112;
    }
    return zero;
  }
  if (r3) {
    *r3 = f + 128;
  }
  vl::Vec3Difference(f + 96, point, schema::RigidBody(body).center_of_mass_address());
  vl::Vec3Cross(f + 80, f + 96, normal);
  vl::MatrixRotate(f + 144, schema::RigidBody(body).world_inverse_inertia_address(), f + 80);
  vl::Vec4Copy(f + 80, f + 144);
  vl::Vec3Cross(f + 128, f + 80, f + 96);
  const double angular = double(vl::Vec3DotOf(f + 128, normal));
  const double inverse_mass = double(schema::RigidBody(body).inverse_mass());
  const double sum = Fadds(inverse_mass, angular);
  const double one = Lfs(kOne);
  const double k = Fdivs(one, sum);
  schema::RigidBody(body).set_effective_mass(float(k));
  return Fmuls(k, vn);
}

void RigidBodyFrictionImpulse(uint32_t out, uint32_t body, uint32_t point,
                              uint32_t normal, double f1, double f2, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6D48 RigidBody::FrictionImpulse");
  const uint32_t f = frame;
  // ld and std of the zero vector's two doublewords.
  auto zero_out = [&] {
    for (uint32_t at : {0u, 8u}) {
      const uint32_t hi = guest::LoadU32(kZeroVector + at);
      const uint32_t lo = guest::LoadU32(kZeroVector + at + 4);
      guest::StoreU32(out + at, hi);
      guest::StoreU32(out + at + 4, lo);
    }
  };
  va::RigidBodyPointVelocity(f + 96, body, point);
  if (double(vl::Vec3Dot(f + 96, f + 96)) == Lfs(kZero)) {
    zero_out();
    return;
  }
  // The velocity's part across the normal: n x (n x v).
  vl::Vec3Cross(f + 80, normal, f + 96);
  vl::Vec3Cross(f + 112, normal, f + 80);
  vl::Vec4Copy(f + 80, f + 112);
  constexpr uint32_t kMinimum = 0x8200222C;
  if (!(double(vl::Vec3Dot(f + 80, f + 80)) > Lfs(kMinimum))) {
    zero_out();
    return;
  }
  vl::Vec3NormalizeSelf(f + 80);
  double limit = Fmuls(f1, f2);
  const double impulse = RigidBodyContactImpulse(body, point, f + 80, f - 208, nullptr);
  if (limit > impulse) {
    limit = impulse;
  }
  vl::Vec4Scaled(out, f + 80, limit);
}

bool RigidBodyContactImpulseWith(uint32_t a, uint32_t point, uint32_t normal,
                                 uint32_t b, uint32_t frame, double* f1, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820B72C8 RigidBody::ContactImpulseWith");
  const uint32_t f = frame;
  va::RigidBodyPointVelocity(f + 144, a, point);
  va::RigidBodyPointVelocity(f + 128, b, point);
  vl::Vec3Difference(f + 160, f + 144, f + 128);
  const double vn = Fneg(double(vl::Vec3DotOf(f + 160, normal)));
  const double zero = Lfs(kZero);
  if (!(vn > zero)) {
    *f1 = zero;
    *r3 = f + 160;
    return true;
  }
  if (schema::RigidBody(a).fixed() == 1 || schema::RigidBody(b).fixed() == 1) {
    return false;
  }
  // Each body's angular term, (I^-1 (r x n)) x r: a's at f + 80, b's at
  // f + 96 (w 0.0 first).
  const uint32_t zero_word = guest::LoadU32(kZero);
  guest::StoreU32(f + 92, zero_word);
  vl::Vec3Difference(f + 112, point, a + 16);
  vl::Vec3Cross(f + 192, f + 112, normal);
  vl::Vec4Copy(f + 80, f + 192);
  vl::MatrixRotate(f + 192, a + 416, f + 80);
  vl::Vec4Copy(f + 80, f + 192);
  vl::Vec3Cross(f + 192, f + 80, f + 112);
  vl::Vec4Copy(f + 80, f + 192);
  guest::StoreU32(f + 108, zero_word);
  vl::Vec3Difference(f + 112, point, b + 16);
  vl::Vec3Cross(f + 192, f + 112, normal);
  vl::Vec4Copy(f + 96, f + 192);
  vl::MatrixRotate(f + 176, b + 416, f + 96);
  vl::Vec4Copy(f + 96, f + 176);
  vl::Vec3Cross(f + 192, f + 96, f + 112);
  vl::Vec4Copy(f + 96, f + 192);
  vl::Vec3AddTo(f + 80, f + 96);
  const double angular = double(vl::Vec3DotOf(f + 80, normal));
  const double masses = Fadds(Lfs(b + 8), Lfs(a + 8));
  const double sum = Fadds(masses, angular);
  const double k = Fdivs(Lfs(kOne), sum);
  Stfs(a + 512, k);
  *f1 = Fmuls(k, vn);
  *r3 = f + 80;
  return true;
}

}  // namespace car_physics
