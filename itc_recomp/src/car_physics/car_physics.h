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

// --- The vehicle's physics step (src/car_physics/vehicle_step.cpp; the
// vehicle in r3, its fields in docs/known-data.md) ---

// Vehicle::BeginStep (sub_820AFAE8): +2352 = dt, +2356 = 1 / dt; the five
// accumulators +2544 + 16i zeroed; +2448, +2452 = 0.0; each wheel record
// 2's word +116 = 0; the step counter +3156 up. Returns the original's r3
// (+2608, the last accumulator).
uint32_t VehicleBeginStep(uint8_t* base, uint32_t vehicle, double dt);
// sub_820AFD30 (the name ours: Vehicle::WheelLoads): each wheel's load +280
// = max(+8, 0.0) on the ground, else 0.0 (all 0.0 with no wheel down).
void VehicleWheelLoads(uint8_t* base, uint32_t vehicle);
// sub_820B0508, sub_820B0560, sub_820B05B8 and sub_820B0610 (the names
// ours: Vehicle::SurfaceValue, SurfaceValue20, 12 and 16): wheel w's
// surface record's float +0, +20, +12 or +16 (0.0, 0.0, 1.0, 1.0 without
// one); the last three also store it at the wheel's +196, +184 or +188.
double VehicleSurfaceValue(uint8_t* base, uint32_t vehicle, uint32_t w);
double VehicleSurfaceValue20(uint8_t* base, uint32_t vehicle, uint32_t w);
double VehicleSurfaceValue12(uint8_t* base, uint32_t vehicle, uint32_t w);
double VehicleSurfaceValue16(uint8_t* base, uint32_t vehicle, uint32_t w);
// sub_820B0908 (the name ours: Vehicle::AccumulateTireForces): k = +2364 /
// +2352; per wheel +284 += k +308, +288 += k +304, and the accumulator
// +2544 + 16w += (+128 x +304 + +144 x +308) x k. Returns the original's
// r3 (+2592). `frame`: 192 bytes.
uint32_t VehicleAccumulateTireForces(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::RollingResistance (sub_820B2748): each wheel on the ground's
// spin +248 moves toward 0.0 (not past it) by +2364 / (wheel record 2 +108
// x 0.5) x |radius x load x the float at 0x82002A18|.
void VehicleRollingResistance(uint8_t* base, uint32_t vehicle);
// sub_820AFDA8 (the name ours: Vehicle::TurnWheels): each wheel's angle +28
// moves by -(spin +248 x dt) (reshaped past 0x820029B4 and 0x820029BC),
// wrapped to a turn. `frame`: the original's r1 (it stores the turn count
// below it, at r1 - 32 and r1 - 24, as the translated fctiwz does).
void VehicleTurnWheels(uint8_t* base, uint32_t vehicle, uint32_t frame);
// sub_820B0838 (the name ours: Vehicle::BrakeTorques): each wheel's +212,
// +216, +300 = 0.0; when braking (+292 + +296 nonzero), +300 = the wheel's
// brake +200 x +2432 x the axle's brake torque (car data), the rear's with
// the handbrake added and +216 the brake's share.
void VehicleBrakeTorques(uint8_t* base, uint32_t vehicle);
// sub_820B0718 (the name ours: Vehicle::AntiLock): byte +2401 = 0; each
// wheel's brake +200 = +292; with ABS (byte +2386) above the speed
// 0x82001FD8 km/h and braking, a wheel on the ground whose slip (+264,
// +268, when +268 < 0.0) is longer than 1 - the axle's grip (+2000, +2128)
// gets 0.0 and its count +204 up.
void VehicleAntiLock(uint8_t* base, uint32_t vehicle);
// sub_820AFBD8 (the name ours: Vehicle::SuspensionTorques): per wheel on
// the ground, v = +284 scaled by the car data's (+1824 or +1828 rear; front
// -(+112 . wheel +144) x +1820); the accumulator += +96 x v; the wheel's +8
// += v; the body gets the torque of (0, +8, 0) less its part along the
// wheel's +112, at the wheel's +160 (RigidBody::AddTorqueAt). Returns the
// original's r3 (the body + 128 after a wheel, else the vehicle). `frame`:
// 256 bytes.
uint32_t VehicleSuspensionTorques(uint8_t* base, uint32_t vehicle, uint32_t frame);

// Vehicle::EngineTorque (sub_820B00C8): x = the rpm normalized to 0-1; 0.0
// at 1.0 or more; else x = max(x, car data +1628), the torque curve (the
// 33 floats at car data +1292) read between its points at x times the float
// at 0x820029CC (the last point, car data +1420, at the end), times +2956
// and car data +1424. `frame`: the original's r1 (it passes the point's
// index through r1 - 16).
double VehicleEngineTorque(uint8_t* base, uint32_t vehicle, double x, uint32_t frame);
// Tire::MagicFormula (sub_820B09C8): D sin(C atan(B x - E (B x - atan(B
// x)))), the coefficients B, C, D, E at +0 to +12; atan and sin are the C
// runtime's (src/c_runtime/math.h).
double TireMagicFormula(uint8_t* base, uint32_t coefficients, double x);
// Vehicle::SetGearValues (sub_820B3C90): from the gear +2252 (-1 and 0 read
// as 0 and 1): +2324 = +2328, +2320 = 0.0, +2312 = +2336 + +2328, +2304 =
// |the gear's ratio x the final drive|, +2308 = the gear's other value x
// car data +1544.
void VehicleSetGearValues(uint8_t* base, uint32_t vehicle);
// Vehicle::AxleReactionTorque (sub_820B3130): for the axle's two wheels
// (from `first`), +352 = (-sign(spin) |brake +300| - +308) x the radius;
// returns their sum.
double VehicleAxleReactionTorque(uint8_t* base, uint32_t vehicle, uint32_t first);
// Vehicle::UpdateFreeAxle (sub_820B3010): the axle's two wheels' spin
// turned by the tire's torque, then slowed by the brake toward 0.0 (not
// past it); +252 = spin x inertia; with a brake, +212 = the brake's part.
void VehicleUpdateFreeAxle(uint8_t* base, uint32_t vehicle, uint32_t first);
// sub_820B3E08, sub_820B3E38 and sub_820B3E68 (the names ours:
// Vehicle::SetBrake, SetHandbrake, SetThrottle): the value clamped to
// [0.0, 1.0] into +292, +296 or +284; returned (the original's f1).
double VehicleSetBrake(uint8_t* base, uint32_t vehicle, double value);
double VehicleSetHandbrake(uint8_t* base, uint32_t vehicle, double value);
double VehicleSetThrottle(uint8_t* base, uint32_t vehicle, double value);
// sub_820B0478 and sub_820B04C0 (the names ours, probably:
// Vehicle::RampReverseThrottle, RampHandbrake): while the byte `on` is
// set, +288 (+296) += car data +1264 (+128) x time, to 1.0; else 0.0.
void VehicleRampReverseThrottle(uint8_t* base, uint32_t vehicle, uint32_t on, double time);
void VehicleRampHandbrake(uint8_t* base, uint32_t vehicle, uint32_t on, double time);
// sub_820B3C20 (the name ours: Vehicle::SetValue2388): +2388 = the value
// clamped to [0.0, 1.0], byte +2385 = `flag`, +2396 = 1 - value x the float
// at 0x82002A2C, clamped.
void VehicleSetValue2388(uint8_t* base, uint32_t vehicle, double value, uint32_t flag);

// --- The physics step's updates (src/car_physics/vehicle_update.cpp) ---

// Matrix::CopyRows (sub_820AFB78): the four rows of `in` into `out`
// (Vec4::Copy each).
void MatrixCopyRows(uint8_t* base, uint32_t out, uint32_t in);
// Matrix::MultiplyTranslation (sub_820B0D68): out = m x the translation by
// the vector's x, y, z (Matrix::Multiply, noted for interpolation as the
// original's call). The translation is built at `frame` + 80 (the vector
// and 1.0) and + 112 (its four rows): the original's frame, 192 bytes.
void MatrixMultiplyTranslation(uint8_t* base, uint32_t out, uint32_t m, uint32_t vector,
                               uint32_t frame);
// Vehicle::TractionControl (sub_820B1790): byte +2400 = 0; the driven
// pair's mean slip and spin raise the counter +2392 (to 30 past the float
// at 0x82000D44) or lower it (not below 0); the throttle +284 x= clamp(1 -
// counter / 30); byte +2400 = 1 when that changed it. `frame`: the
// original's r1 (it converts the counter through r1 - 16).
void VehicleTractionControl(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateTireGrip (sub_820B1FB8): per wheel, the grip +236 and the
// force limit +316 from the load (Tire::MagicFormula with the coefficients
// at 0x82600D20), the camber's loss and the surface (SurfaceValue12); 0.0
// off the ground.
void VehicleUpdateTireGrip(uint8_t* base, uint32_t vehicle);
// Vehicle::UpdateEngineTorque (sub_820B22D8): the throttle used (+2516,
// after the rev limiter and the clutch), the engine's torque (EngineTorque)
// with nitrous and engine braking into +2240 and +2236. `frame`: the
// original's r1 after its stwu (96 bytes; EngineTorque's below it).
void VehicleUpdateEngineTorque(uint8_t* base, uint32_t vehicle, uint32_t frame);

}  // namespace car_physics
