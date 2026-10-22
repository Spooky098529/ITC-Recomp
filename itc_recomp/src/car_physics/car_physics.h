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

// RigidBody::Integrate (sub_820B7658): nothing when fixed. The torque +128
// x dt into the angular momentum +400 (damped by 1 - +384 dt when +384 >
// 0.0), +128 = zero, UpdateAngularVelocity; with a spin, the orientation
// turned by it over dt (Quaternion::FromAxisAngle and Multiply,
// SetOrientation, UpdateOrientation); the gravity +496 into the force +64,
// that x dt into the momentum +176 (damped by 1 - +164 dt), +64 = zero,
// UpdateVelocity; the origin of mass +272 += the velocity +32 x dt,
// UpdateOrigin. Returns what the original leaves in r3 (the body when
// fixed, else +336). `frame`: 208 bytes.
uint32_t RigidBodyIntegrate(uint8_t* base, uint32_t body, double dt, uint32_t frame);
// RigidBody::Reset (sub_820B77D0): the 528 bytes cleared (memset), then the
// defaults: the speed and spin caps (+48, +112) the float at 0x82002A90, a
// mass of 1.0, gravity (0, [0x82002A8C], 0), the inertia's diagonal the
// float at 0x82002A10, the identity orientation (0x826381B0), no velocity,
// spin or center of mass offset; words +12 and +516 = 0. `frame`: 176
// bytes.
void RigidBodyReset(uint8_t* base, uint32_t body, uint32_t frame);
// RigidBody::RigidBody (sub_820B7928): the w of its vectors = 0.0, then
// Reset. `frame`: 96 bytes.
void RigidBodyConstruct(uint8_t* base, uint32_t body, uint32_t frame);
// RigidBody::EffectiveMassAt (sub_820B6A38): out (3 floats) = for each
// world axis e, 1 / (e x inverse mass + ((r x e) inverse inertia +368 to
// +376) x r) . e), r = point - the center of mass +16. Returns the last dot
// product (the original's f1). `frame`: 272 bytes.
double RigidBodyEffectiveMassAt(uint8_t* base, uint32_t body, uint32_t out, uint32_t point,
                                uint32_t frame);
// RigidBody::ContactImpulse (sub_820B6C78): vn = -(the velocity at the
// point . the normal); 0.0 when vn < 0.0; else +512 = 1 / (inverse mass +
// ((I^-1 (r x n)) x r) . n) and vn x that. `r3` (when given) gets what the
// original leaves there (a temporary of its frame). `frame`: 208 bytes.
double RigidBodyContactImpulse(uint8_t* base, uint32_t body, uint32_t point, uint32_t normal,
                               uint32_t frame, uint32_t* r3);
// RigidBody::FrictionImpulse (sub_820B6D48): out = the sliding direction at
// the point (the velocity's part across the normal, normalized) x min(f1 x
// f2, ContactImpulse along it); the zero vector when the velocity is zero
// or its part across the normal has a squared length at most the float at
// 0x8200222C. `frame`: 192 bytes.
void RigidBodyFrictionImpulse(uint8_t* base, uint32_t out, uint32_t body, uint32_t point,
                              uint32_t normal, double f1, double f2, uint32_t frame);
// RigidBody::ContactImpulseWith (sub_820B72C8): as ContactImpulse for body
// a against body b (the relative velocity, both bodies' inverse masses and
// angular terms; a's +512 set). False, with nothing done, when either body
// is fixed (byte +0 = 1): the original then reads its own stack's leftovers
// for that body's term, so it runs. Otherwise the impulse in `f1`, and in
// `r3` what the original leaves there. `frame`: 272 bytes.
bool RigidBodyContactImpulseWith(uint8_t* base, uint32_t a, uint32_t point, uint32_t normal,
                                 uint32_t b, uint32_t frame, double* f1, uint32_t* r3);

}  // namespace car_physics
