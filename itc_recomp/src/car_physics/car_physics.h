// The car physics as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/car-physics.md): the rigid body, the
// vehicle's physics step and the collision world. The objects are guest
// objects shared with the recompiled code, so these work on guest memory,
// with the originals' loads and stores in their order and their arithmetic
// step for step (src/car_physics/float_ops.h), so the results are the
// translated code's bit for bit. The originals' temporaries are on their
// stack: `frame` is the original's stack frame (its r1 after the stwu,
// guest memory), and the temporaries go at the same offsets. Flush-to-zero
// off on entry (the adapters' job); the vector library's and the matrix
// math's plain C++ is called directly (src/vehicle_accessors/vectors.h,
// src/matrix_math/matrix_math.h). Built with -ffp-model=strict and
// -ffp-contract=off (CMakeLists.txt). Adapters: src/hooks/car_physics.cpp.

#pragma once

#include <cstdint>

namespace car_physics {

// --- The rigid body (the vehicle's +336; fields in docs/known-data.md) ---

// RigidBody::UpdateOrigin (sub_820B6E70): +16 (the center of mass, w 0.0)
// = the matrix +224's fourth row; the origin +208 = +16 - the offset +192
// rotated by +224; the body matrix's fourth row (+336) = +208, w 1.0.
// `frame`: 176 bytes.
void RigidBodyUpdateOrigin(uint8_t* base, uint32_t body, uint32_t frame);
// RigidBody::UpdateVelocity (sub_820B70C8): +32 = the momentum +176 x the
// inverse mass +8; +160 = its length; +144 = its direction ((1, 0, 0, 0)
// when still); above the speed cap +48, +32 and +176 scaled down to it and
// +160 = the cap. Returns what the original leaves in r3 (+176 when capped,
// else +144). `frame`: 160 bytes.
uint32_t RigidBodyUpdateVelocity(uint8_t* base, uint32_t body, uint32_t frame);
// RigidBody::UpdateAngularVelocity (sub_820B7478): +96 = the world inverse
// inertia +416 x the angular momentum +400; above the spin cap +112, +400
// and +96 scaled down to it. Returns what the original leaves in f1 (the
// squared spin, or the scale). `frame`: 144 bytes.
double RigidBodyUpdateAngularVelocity(uint8_t* base, uint32_t body, uint32_t frame);
// RigidBody::UpdateOrientation (sub_820B6F38): +224 = the quaternion +80's
// matrix (Matrix::FromQuaternion), its rows 0-2 into the body matrix +288
// to +320, and the world inverse inertia +416 = R^T diag(+368, +372, +376)
// R. `frame`: 320 bytes.
void RigidBodyUpdateOrientation(uint8_t* base, uint32_t body, uint32_t frame);
// RigidBody::SetOrientation (sub_820B71E8): +80 = the quaternion's 16
// bytes, normalized, then UpdateOrientation and UpdateOrigin. `frame`: 96
// bytes (the callees' frames below it).
void RigidBodySetOrientation(uint8_t* base, uint32_t body, uint32_t quaternion, uint32_t frame);
// RigidBody::SetCenterOfMassPosition (sub_820B71B0): +224's fourth row =
// the point (Matrix::SetTranslation), then UpdateOrigin. `frame`: 96 bytes.
void RigidBodySetCenterOfMassPosition(uint8_t* base, uint32_t body, uint32_t point,
                                      uint32_t frame);
// RigidBody::SetPosition (sub_820B6710): the body matrix's translation =
// the position, +208 = it (w 0.0), +224's fourth row = it + the offset
// +192 rotated by +224, +16 = that (w 0.0). `frame`: 192 bytes.
void RigidBodySetPosition(uint8_t* base, uint32_t body, uint32_t position, uint32_t frame);
// RigidBody::SetVelocity (sub_820B7238): +176 = the velocity x the mass
// +4, then UpdateVelocity. Returns UpdateVelocity's r3. `frame`: 112 bytes.
uint32_t RigidBodySetVelocity(uint8_t* base, uint32_t body, uint32_t velocity, uint32_t frame);
// RigidBody::SetAngularVelocity (sub_820B7500): +400 = R (R^T w x the
// inertia +352 to +360), then UpdateAngularVelocity. Returns its f1.
// `frame`: 160 bytes.
double RigidBodySetAngularVelocity(uint8_t* base, uint32_t body, uint32_t velocity,
                                   uint32_t frame);
// RigidBody::SetInertia (sub_820B7280): +352 to +360 = the vector's x, y,
// z, +368 to +376 their reciprocals; then UpdateOrientation (a tail call:
// `frame` is its 320 bytes).
void RigidBodySetInertia(uint8_t* base, uint32_t body, uint32_t inertia, uint32_t frame);
// RigidBody::ApplyImpulses (sub_820B75B8): nothing when fixed (byte +0);
// +400 += the torque +128 x dt, +128 = zero, UpdateAngularVelocity; +176 +=
// the force +64 x dt, +64 = zero, UpdateVelocity. Returns what the
// original leaves in r3 (the body when fixed, else UpdateVelocity's).
// `frame`: 144 bytes.
uint32_t RigidBodyApplyImpulses(uint8_t* base, uint32_t body, double dt, uint32_t frame);

}  // namespace car_physics
