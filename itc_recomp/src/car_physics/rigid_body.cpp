// The rigid body's updates and setters (docs/functions/car-physics.md):
// see car_physics.h.

#include "car_physics/car_physics.h"

#include "car_physics/float_ops.h"

#include "base/log.h"
#include "matrix_math/matrix_math.h"
#include "vehicle_accessors/vectors.h"

namespace car_physics {

using namespace ops;
namespace vl = vector_library;
namespace mm = matrix_math;

void RigidBodyUpdateOrigin(uint8_t* base, uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6E70 RigidBody::UpdateOrigin");
  const uint32_t f = frame;
  // +16 = the fourth row of +224, w 0.0.
  CopyWord(base, kZero, f + 92);
  CopyWord(base, body + 272, f + 80);
  CopyWord(base, body + 276, f + 84);
  CopyWord(base, body + 280, f + 88);
  vl::Vec4Copy(base, body + 16, f + 80);
  // +208 = +16 - +192 rotated by +224.
  vl::MatrixRotate(base, f + 112, body + 224, body + 192);
  vl::Vec3Difference(base, f + 128, body + 16, f + 112);
  vl::Vec4Copy(base, body + 208, f + 128);
  // The body matrix's fourth row = +208, w 1.0.
  CopyWord(base, kOne, f + 108);
  CopyWord(base, body + 208, f + 96);
  CopyWord(base, body + 212, f + 100);
  CopyWord(base, body + 216, f + 104);
  vl::Vec4Copy(base, body + 336, f + 96);
}

uint32_t RigidBodyUpdateVelocity(uint8_t* base, uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B70C8 RigidBody::UpdateVelocity");
  const uint32_t f = frame;
  // +32 = the momentum x the inverse mass.
  vl::Vec4Scaled(base, f + 96, body + 176, Lfs(base, body + 8));
  vl::Vec4Copy(base, body + 32, f + 96);
  const double speed = double(vl::Vec3Length(base, body + 32));
  Stfs(base, body + 160, speed);
  // +144 = the direction, (1, 0, 0, 0) when still.
  uint32_t direction;
  if (speed == Lfs(base, kZero)) {
    CopyWord(base, kOne, f + 80);
    CopyWord(base, kZero, f + 84);
    CopyWord(base, kZero, f + 88);
    CopyWord(base, kZero, f + 92);
    direction = f + 80;
  } else {
    const double one = Lfs(base, kOne);
    vl::Vec4Scaled(base, f + 96, body + 32, Fdivs(one, speed));
    direction = f + 96;
  }
  vl::Vec4Copy(base, body + 144, direction);
  // Above the speed cap: the velocity and momentum scaled down to it.
  const double cap = Lfs(base, body + 48);
  if (!(Lfs(base, body + 160) > cap)) {
    return body + 144;
  }
  vl::Vec4ScaledFrom(base, f + 96, body + 144, cap);
  vl::Vec4Copy(base, body + 32, f + 96);
  vl::Vec4Scaled(base, f + 96, body + 32, Lfs(base, body + 4));
  vl::Vec4Copy(base, body + 176, f + 96);
  CopyWord(base, body + 48, body + 160);
  return body + 176;
}

double RigidBodyUpdateAngularVelocity(uint8_t* base, uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7478 RigidBody::UpdateAngularVelocity");
  const uint32_t f = frame;
  vl::MatrixRotate(base, f + 80, body + 416, body + 400);
  vl::Vec4Copy(base, body + 96, f + 80);
  const double spin2 = double(vl::Vec3Dot(base, body + 96, body + 96));
  const double cap = Lfs(base, body + 112);
  if (!(spin2 > Fmuls(cap, cap))) {
    return spin2;
  }
  const double spin = double(vl::Sqrtf(spin2));
  const double scale = Fdivs(Lfs(base, body + 112), spin);
  vl::Vec4ScaleBy(base, body + 400, scale);
  vl::Vec4ScaleBy(base, body + 96, scale);
  return scale;
}

void RigidBodyUpdateOrientation(uint8_t* base, uint32_t body, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6F38 RigidBody::UpdateOrientation");
  const uint32_t f = frame;
  // Three matrices on the stack: R at f + 80, the inertia's diagonal at
  // f + 144, R^T at f + 208.
  mm::SetIdentity(base, f + 80);
  mm::SetIdentity(base, f + 208);
  mm::SetIdentity(base, f + 144);
  mm::FromQuaternion(base, body + 224, body + 80);
  vl::Vec4Copy(base, body + 288, body + 224);
  vl::Vec4Copy(base, body + 304, body + 240);
  vl::Vec4Copy(base, body + 320, body + 256);
  CopyWord(base, body + 368, f + 144);
  CopyWord(base, body + 372, f + 164);
  const uint32_t m00 = guest::LoadU32(base, body + 224);
  const uint32_t m01 = guest::LoadU32(base, body + 228);
  const uint32_t m02 = guest::LoadU32(base, body + 232);
  const uint32_t m11 = guest::LoadU32(base, body + 244);
  const uint32_t m12 = guest::LoadU32(base, body + 248);
  const uint32_t m21 = guest::LoadU32(base, body + 260);
  const uint32_t m22 = guest::LoadU32(base, body + 264);
  const uint32_t i2 = guest::LoadU32(base, body + 376);
  const uint32_t m10 = guest::LoadU32(base, body + 240);
  const uint32_t m20 = guest::LoadU32(base, body + 256);
  const uint32_t zero = guest::LoadU32(base, kZero);
  const uint32_t one = guest::LoadU32(base, kOne);
  for (uint32_t at : {92u, 108u, 124u, 128u, 132u, 136u}) {
    guest::StoreU32(base, f + at, zero);
  }
  guest::StoreU32(base, f + 140, one);
  for (uint32_t at : {220u, 236u, 252u}) {
    guest::StoreU32(base, f + at, zero);
  }
  guest::StoreU32(base, f + 184, i2);
  for (uint32_t at : {256u, 260u, 264u}) {
    guest::StoreU32(base, f + at, zero);
  }
  guest::StoreU32(base, f + 268, one);
  for (uint32_t at : {148u, 152u, 156u, 160u, 168u, 172u, 176u, 180u, 188u, 192u, 196u, 200u}) {
    guest::StoreU32(base, f + at, zero);
  }
  guest::StoreU32(base, f + 204, one);
  // R's upper 3x3.
  guest::StoreU32(base, f + 80, m00);
  guest::StoreU32(base, f + 84, m01);
  guest::StoreU32(base, f + 88, m02);
  guest::StoreU32(base, f + 96, m10);
  guest::StoreU32(base, f + 100, m11);
  guest::StoreU32(base, f + 104, m12);
  guest::StoreU32(base, f + 112, m20);
  guest::StoreU32(base, f + 116, m21);
  guest::StoreU32(base, f + 120, m22);
  // R^T's.
  guest::StoreU32(base, f + 208, m00);
  guest::StoreU32(base, f + 212, m10);
  guest::StoreU32(base, f + 216, m20);
  guest::StoreU32(base, f + 224, m01);
  guest::StoreU32(base, f + 228, m11);
  guest::StoreU32(base, f + 232, m21);
  guest::StoreU32(base, f + 240, m02);
  guest::StoreU32(base, f + 244, m12);
  guest::StoreU32(base, f + 248, m22);
  // +416 = R^T (diag R).
  mm::MultiplyReversed(base, body + 416, f + 80, f + 144);
  mm::MultiplyReversed(base, body + 416, body + 416, f + 208);
}

void RigidBodySetOrientation(uint8_t* base, uint32_t body, uint32_t quaternion, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B71E8 RigidBody::SetOrientation");
  // ld and std of the two doublewords.
  for (uint32_t at : {0u, 8u}) {
    const uint32_t hi = guest::LoadU32(base, quaternion + at);
    const uint32_t lo = guest::LoadU32(base, quaternion + at + 4);
    guest::StoreU32(base, body + 80 + at, hi);
    guest::StoreU32(base, body + 84 + at, lo);
  }
  mm::QuaternionNormalizeSelf(base, body + 80);
  RigidBodyUpdateOrientation(base, body, frame - 320);
  RigidBodyUpdateOrigin(base, body, frame - 176);
}

void RigidBodySetCenterOfMassPosition(uint8_t* base, uint32_t body, uint32_t point,
                                      uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B71B0 RigidBody::SetCenterOfMassPosition");
  vl::MatrixSetTranslation(base, body + 224, point);
  RigidBodyUpdateOrigin(base, body, frame - 176);
}

void RigidBodySetPosition(uint8_t* base, uint32_t body, uint32_t position, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B6710 RigidBody::SetPosition");
  const uint32_t f = frame;
  const uint32_t zero = guest::LoadU32(base, kZero);
  guest::StoreU32(base, f + 124, zero);
  vl::MatrixSetTranslation(base, body + 288, position);
  // +208 = the body matrix's translation, w 0.0.
  CopyWord(base, body + 336, f + 80);
  CopyWord(base, body + 340, f + 84);
  const uint32_t z = guest::LoadU32(base, body + 344);
  guest::StoreU32(base, f + 92, zero);
  guest::StoreU32(base, f + 88, z);
  vl::Vec4Copy(base, body + 208, f + 80);
  // +224's fourth row = the position + the offset rotated by +224.
  vl::MatrixRotate(base, f + 128, body + 224, body + 192);
  vl::Vec4Copy(base, f + 112, f + 128);
  vl::Vec3Sum(base, f + 128, position, f + 112);
  vl::MatrixSetTranslation(base, body + 224, f + 128);
  // +16 = that, w 0.0.
  CopyWord(base, body + 272, f + 96);
  CopyWord(base, body + 276, f + 100);
  const uint32_t z2 = guest::LoadU32(base, body + 280);
  guest::StoreU32(base, f + 108, zero);
  guest::StoreU32(base, f + 104, z2);
  vl::Vec4Copy(base, body + 16, f + 96);
}

uint32_t RigidBodySetVelocity(uint8_t* base, uint32_t body, uint32_t velocity, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7238 RigidBody::SetVelocity");
  vl::Vec4Scaled(base, frame + 80, velocity, Lfs(base, body + 4));
  vl::Vec4Copy(base, body + 176, frame + 80);
  return RigidBodyUpdateVelocity(base, body, frame - 160);
}

double RigidBodySetAngularVelocity(uint8_t* base, uint32_t body, uint32_t velocity,
                                   uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7500 RigidBody::SetAngularVelocity");
  const uint32_t f = frame;
  const uint32_t zero = guest::LoadU32(base, kZero);
  guest::StoreU32(base, f + 92, zero);
  guest::StoreU32(base, f + 108, zero);
  // The velocity in the body's axes, times the inertia, back to the world.
  mm::InverseRotate(base, f + 112, body + 224, velocity);
  vl::Vec4Copy(base, f + 80, f + 112);
  {
    const double i0 = Lfs(base, body + 352);
    const double v0 = Lfs(base, f + 80);
    Stfs(base, f + 96, Fmuls(i0, v0));
  }
  {
    const double i1 = Lfs(base, body + 356);
    const double v1 = Lfs(base, f + 84);
    Stfs(base, f + 100, Fmuls(i1, v1));
  }
  {
    const double i2 = Lfs(base, body + 360);
    const double v2 = Lfs(base, f + 88);
    Stfs(base, f + 104, Fmuls(i2, v2));
  }
  vl::MatrixRotate(base, f + 112, body + 224, f + 96);
  vl::Vec4Copy(base, body + 400, f + 112);
  return RigidBodyUpdateAngularVelocity(base, body, f - 144);
}

void RigidBodySetInertia(uint8_t* base, uint32_t body, uint32_t inertia, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B7280 RigidBody::SetInertia");
  CopyWord(base, inertia + 0, body + 352);
  CopyWord(base, inertia + 4, body + 356);
  CopyWord(base, inertia + 8, body + 360);
  const double one = Lfs(base, kOne);
  const double x = Lfs(base, body + 352);
  const double y = Lfs(base, body + 356);
  const double rx = Fdivs(one, x);
  const double z = Lfs(base, body + 360);
  const double ry = Fdivs(one, y);
  const double rz = Fdivs(one, z);
  Stfs(base, body + 368, rx);
  Stfs(base, body + 372, ry);
  Stfs(base, body + 376, rz);
  RigidBodyUpdateOrientation(base, body, frame);
}

uint32_t RigidBodyApplyImpulses(uint8_t* base, uint32_t body, double dt, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_820B75B8 RigidBody::ApplyImpulses");
  if (guest::LoadU8(base, body) != 0) {
    return body;
  }
  const uint32_t f = frame;
  vl::Vec4Scaled(base, f + 80, body + 128, dt);
  vl::Vec3AddTo(base, body + 400, f + 80);
  vl::Vec4Copy(base, body + 128, kZeroVector);
  RigidBodyUpdateAngularVelocity(base, body, f - 144);
  vl::Vec4Scaled(base, f + 80, body + 64, dt);
  vl::Vec3AddTo(base, body + 176, f + 80);
  vl::Vec4Copy(base, body + 64, kZeroVector);
  return RigidBodyUpdateVelocity(base, body, f - 160);
}

}  // namespace car_physics
