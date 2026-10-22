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
// Vehicle::UpdateBrakeHeat (sub_820B35C8): each wheel's brake heat +220
// raised while braking (the heat added kept from the last braking wheel
// for the next ones, as the original), cooled toward the float at
// 0x82002568, clamped; the fade +3232 raised above car data +168, then
// cooled by dt x 0.5, clamped, and copied to +3236.
void VehicleUpdateBrakeHeat(uint8_t* base, uint32_t vehicle);
// Vehicle::UpdateBackfire (sub_820B3788): the charge +2500, byte +2504 and
// the strength +2508 (byte +3240 the shift's latch). `frame`: the
// original's r1 (it converts integers through r1 - 16 and r1 - 8).
void VehicleUpdateBackfire(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateTireWear (sub_820B39C8): the wear +292 by wheel (each
// wheel's share kept at `frame` + 80, the original's 112-byte frame), then
// each axle's grip (wheel record 2 +96).
void VehicleUpdateTireWear(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateSteering (sub_820B18B8): the steering limit (speed
// sensitive with byte +276 = 1) widened by the counter-steer term from the
// yaw rate; +328 = the old +304, +304 the steering, the front wheels'
// angles +308 and +312 (Ackermann, by tan and atan), the limits +316 and
// +320, +324 smoothed, +3116. The body's row 1 (w 0.0) goes to `frame` + 80
// (the original's 160-byte frame) for the dot product. False, with only
// that temporary written, where the original's tan would take its _except1
// path (an infinite or NaN steering, never in play): then the original runs.
bool VehicleUpdateSteering(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateTorqueSplit (sub_820B2100): the axles' torque shares
// +1988 and +2116 by the four-wheel drive's mode +2376. False, with nothing
// done, for mode 2 in a forward gear (sub_820B0340, never in play): then
// the original runs.
bool VehicleUpdateTorqueSplit(uint8_t* base, uint32_t vehicle);
// Vehicle::UpdateTurbo (sub_820B24A8): the boost +2944 toward its target,
// the blow-off valve (+2964, byte +2960) or the wastegate (+2952, byte
// +2948), and the torque factor +2956 with a turbo (engine byte +336).
void VehicleUpdateTurbo(uint8_t* base, uint32_t vehicle);
// Vehicle::UpdateTireForces (sub_820B3F80): the driven axles' +1996 and
// +2124; per wheel the slip speed +356, the slips +268, +264, +260 and the
// tire's forces +308 (and +332), +304 (Tire::MagicFormula, kept in a unit
// circle, less the surface's drag). The temporaries at `frame` + 80 to +
// 159 (the original's 272-byte frame). Returns what the original leaves in
// r3 (the vehicle, or `frame` + 112 when the last wheel didn't slip).
uint32_t VehicleUpdateTireForces(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateAtRest (sub_820B2808): the at-rest byte +2928, and when
// set the gear to 0, the wheels' spin and forces zeroed and the body's
// horizontal velocity and its spin stopped (RigidBody::SetVelocity,
// SetAngularVelocity, and the angular momentum +400's x, y, z zeroed).
// `frame`: the original's 192 bytes (its callees' below). Returns what the
// original leaves in r3.
uint32_t VehicleUpdateAtRest(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateAero (sub_820B2A48): the drag into the body's force
// accumulator +2608, the downforce into the wheels' (moving forward), and
// the engine's shake (+2340, +2344) into the front wheels'. `frame`: the
// original's 240 bytes (EngineTorque's below it). Returns what the
// original leaves in r3.
uint32_t VehicleUpdateAero(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::UpdateWheelAxes (sub_820B1B00): per wheel the toe +36 and the
// axes +144 and +128 (the vehicle's +112 and +128 turned by toe and
// steering; those two also into +2080 and +2064), +324 = +320, the contact's
// velocity +336 and its speeds +320, +328, the camber +32 and the wheel's
// quaternion +48. `frame`: the original's 256 bytes. Returns what the
// original leaves in r3 (wheel 3's +48).
uint32_t VehicleUpdateWheelAxes(uint8_t* base, uint32_t vehicle, uint32_t frame);

// --- The vehicle's state from its body, its copies and set-up
// (src/car_physics/vehicle_state.cpp) ---

// Vehicle::SyncFromBody (sub_820B13C8): the body matrix's rows 0-2 (w 0.0)
// into +0, +16, +32, its +160 into +48, its angular velocity into +64, the
// matrix into +208 (Matrix::CopyRows); per wheel the mount point (the car
// data's +896 + 96 w, its +16 also into wheel +24) through +208 into wheel
// +80 with the travel wheel +0 added to y, and without into wheel +64;
// +3312 = +3248 about the body's position (MultiplyTranslation twice and
// Matrix::Multiply, noted for interpolation as the original's call), the
// body's word +12 = its address; the axles' midpoints +1888, +2016 and
// tracks +1992, +2120. `frame`: the original's 368 bytes
// (MultiplyTranslation's below it). Returns what the original leaves in f1
// (0.5, the scale of the midpoints).
double VehicleSyncFromBody(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Wheel::Copy (sub_820B0A50): the wheel record's +0 to +359 field by field
// (not +100 to +108, +40 to +47 or +360 on).
void WheelCopy(uint8_t* base, uint32_t out, uint32_t wheel);
// sub_820B0CC0 (the name ours: WheelRecord2::Copy): six vectors, then the
// words +96 to +116 of a 120-byte record (wheel record 2).
void WheelRecord2Copy(uint8_t* base, uint32_t out, uint32_t record);

// Vehicle::Reset (sub_820B0EE8): the vehicle's physics state to its
// defaults (no allocation; the fields in its row), the wheels' quaternions
// +48 by Quaternion::FromAxisAngle (its scratch below `frame`), +3248 and
// +3312 the identity (built at `frame` + 80, the original's 240 bytes).
// `f1` gets what the original leaves there (the last FromAxisAngle's).
void VehicleReset(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1);
// Vehicle::SetUpGearing (sub_820B4AA8): the driven wheels' radius +2332
// and the axle's +2336 by the drive type, +2328, Vehicle::SetGearValues,
// the shift-up speeds +2632 + 4 x gear (Vehicle::EngineTorque, passing
// `frame`, the original's 176 bytes), +2300, +2296 and +2292 (the torque
// curve's peak; its index through `frame` + 80).
void VehicleSetUpGearing(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::SetSteering (sub_820B3D10): byte +276 = 1, +280 = the input
// clamped to [-1, 1], +3108 from the steering's sine ratio and the front
// wheels. Returns what the original leaves in f1 (the second sin's).
double VehicleSetSteering(uint8_t* base, uint32_t vehicle, double steer);
// sub_820B0668 (the name ours: SteerRecord::Fill): the 36-byte record at
// `out` (vehicle +2988) from the front axle data `record` and f1, f2, f3.
void SteerRecordFill(uint8_t* base, uint32_t out, uint32_t record, double f1, double f2,
                     double f3);
// sub_820B3E98 (the name ours: SteerRecord::Curve): the curve of the
// record at +2988 for an angle (tan of it, then a polynomial). False, with
// nothing done, where tan would take its _except1 path (an infinite or NaN
// angle, never in play): then the original runs.
bool SteerRecordCurve(uint8_t* base, uint32_t record, double angle, double* result);
// Vehicle::SteerTowards (sub_820B5FD8): byte +276 = 1; +280 moved toward
// the target (-1 or 1 with the bytes `left`, `right`) by a rate from dt and
// the speed, clamped to [-1, 1]; the record +2988 (SteerRecord::Fill) and
// +3108 (SteerRecord::Curve of the front wheels' mean slip). False, with
// nothing done, where the curve's tan would fail: then the original runs.
// `f1` gets what the original leaves there (the curve).
bool VehicleSteerTowards(uint8_t* base, uint32_t vehicle, double target, double dt, double f3,
                         uint32_t left, uint32_t right, double* f1);

// sub_820B0198 (the name ours: Vehicle::SuspensionSpring): wheel w's spring
// and damper (the suspension at its wheel data +96, the axle partner's
// anti-roll) over +2368 substeps of +2364 with `change` (f1) and the
// velocity (a float at `velocity`, updated); wheel +0 = the travel. Returns
// the force (the change of speed x -1 / dt x the mass), or -R32 when the
// wheel leaves the ground.
double VehicleSuspensionSpring(uint8_t* base, uint32_t vehicle, uint32_t w, double change,
                               uint32_t velocity);
// Vehicle::UpdateWheelSuspension (sub_820B5588): byte wheel +16 = 0; the
// vertical speed at the mount (RigidBody::PointVelocity at `frame` + 96,
// dotted with +160, negated at `frame` + 80: the original's 176 bytes)
// through SuspensionSpring; wheel +4, +8 (the force) and byte +17 (on the
// ground, counted in +348). Returns SuspensionSpring's (the original's f1).
double VehicleUpdateWheelSuspension(uint8_t* base, uint32_t vehicle, uint32_t w,
                                    uint32_t frame);
// Vehicle::UpdateSuspension (sub_820B6328): +348 = 0; the wheel order
// +2176 to +2188 sorted by load (+280); each wheel's +12 = +0, its
// UpdateWheelSuspension in that order (its frame below `frame`, the
// original's 144 bytes); each accumulator +2544 + 16 w += up (+96) x wheel
// +8. Returns the original's r3 (+2592); `f1` gets wheel 3's +8.
uint32_t VehicleUpdateSuspension(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1);

// Vehicle::UpdateDrivenAxle (sub_820B31D0): a limited-slip differential on
// the axle from `first` driven by f1: the wheels' accelerations (locked as
// one under the locking torque), their spins +248, +252 and brake forces
// +212, the axle's mean spin +2192. Its temporaries below `sp` (the
// caller's r1: the accelerations at -64, the flags at -48, the wheels at
// -56), as the original's.
void VehicleUpdateDrivenAxle(uint8_t* base, uint32_t vehicle, uint32_t first, double drive,
                             uint32_t sp);
// Vehicle::UpdateDrivetrain (sub_820B4778): +2200 = +2196; the driven
// axles' reactions (AxleReactionTorque) and the other's UpdateFreeAxle; the
// engine's spin solved against them over +2364, UpdateDrivenAxle given the
// axle term (twice for four-wheel drive); the engine's momentum +2208, rpm
// +2228, +2220, +2212, +2224. `frame`: the original's 176 bytes (the axles'
// half inertias at +80, +84 with both driven).
void VehicleUpdateDrivetrain(uint8_t* base, uint32_t vehicle, uint32_t frame);

// Vehicle::UpdateGearbox (sub_820B4CF0): with one gear (probably a CVT)
// its ratio +2288 and the gear values; the clutch +2264, the shift timer
// +2268, and the shifts: by the speed's shift points when the byte `manual`
// is clear, on the requests +2284 (up) and +2285 (down) either way;
// reverse, neutral, pulling away. A shift sets the gear +2252, the old
// +2256, the timer and the car data's rate (+1668 + 4 x old gear, or +400
// pulling away), then Vehicle::SetGearValues. `frame`: the original's 112
// bytes (integers converted through its +80).
void VehicleUpdateGearbox(uint8_t* base, uint32_t vehicle, uint32_t manual, uint32_t frame);
// sub_820B6430 (the name ours: Vehicle::ShiftGears): UpdateGearbox, manual
// unless the gear count is 1 or byte +2249 is set (its frame below
// `frame`, the original's 96 bytes); +2348 = -1.0 in reverse, else 1.0.
void VehicleShiftGears(uint8_t* base, uint32_t vehicle, uint32_t frame);

// --- The collision world (src/car_physics/collision_world.cpp) ---

// The surface settings' getters (sub_820BC1F8, sub_820BC240, sub_820BC288,
// sub_820BC2D0; the names ours: CollisionSurface::A to D): the group's value
// for the first of the flags' bits 0x01000000, 0x02000000, 0x04000000, else
// its default (D's is group B's, the game's). Their tail-calling wrappers
// (sub_820BC540, sub_820BC548, sub_820BC530, sub_820BC538: OfA to OfD).
double CollisionSurfaceA(uint8_t* base, uint32_t flags);
double CollisionSurfaceB(uint8_t* base, uint32_t flags);
double CollisionSurfaceC(uint8_t* base, uint32_t flags);
double CollisionSurfaceD(uint8_t* base, uint32_t flags);
double CollisionSurfaceOfA(uint8_t* base, uint32_t flags);
double CollisionSurfaceOfB(uint8_t* base, uint32_t flags);
double CollisionSurfaceOfC(uint8_t* base, uint32_t flags);
double CollisionSurfaceOfD(uint8_t* base, uint32_t flags);
// The setters (sub_820BC318, sub_820BC360, sub_820BC3A8, sub_820BC3F0):
// the default with any of 0x07000000, and each bit's value.
void CollisionSurfaceSetA(uint8_t* base, uint32_t bits, double value);
void CollisionSurfaceSetB(uint8_t* base, uint32_t bits, double value);
void CollisionSurfaceSetC(uint8_t* base, uint32_t bits, double value);
void CollisionSurfaceSetD(uint8_t* base, uint32_t bits, double value);
// sub_820BC438 (the name ours: CollisionSurfaces::Reset): groups A and C
// 0.5, B and D 1.0; CollisionWorld::ResetSurfaces (sub_820BC550) calls it.
void CollisionSurfacesReset(uint8_t* base);
void CollisionWorldResetSurfaces(uint8_t* base);
// sub_820BAA78 (the name ours: CollisionShape::Init): +28 and +44 = 0.0,
// words +0, +4, +8 = 0.
void CollisionShapeInit(uint8_t* base, uint32_t shape);
// CollisionObject::CollisionObject (sub_820B7AE8): the vtable 0x82002A94,
// the shape at +32 (CollisionShape::Init), the w of +160, +176, +208, +304
// = 0.0.
void CollisionObjectConstruct(uint8_t* base, uint32_t object);
// CollisionObject::AccumulateForce (sub_820B7DF8): +176 += (point - the
// body's center) x force (through `frame` + 80 to + 127, the original's
// 160 bytes); +160 per axis the force's when larger in magnitude; +284 = 0,
// +192 up. Returns the original's r3 (+176).
uint32_t CollisionObjectAccumulateForce(uint8_t* base, uint32_t object, uint32_t point,
                                        uint32_t force, uint32_t frame);
// CollisionObject::AddContact (sub_820B7C50): a new contact
// (CollisionWorld::NewContact) pushed on +240; returns it, or 0.
uint32_t CollisionObjectAddContact(uint8_t* base, uint32_t object);
// sub_820B7ED0 (the name ours: CollisionRecords::Merge): the `count` hit
// records copied to `out`, those with the same id (+60) merged (averaged,
// the push +32 normalized with its length in +48; merged ones' ids -1,
// dropped); the summary the average of those left with surface bits (more
// than one), else a copy of the first. Returns the count left. `frame`: the
// original's 304 bytes.
uint32_t CollisionRecordsMerge(uint8_t* base, uint32_t out, uint32_t summary, uint32_t hits,
                               uint32_t count, uint32_t frame);

// The world's pairs (32 bytes each, at the world's +36, counted by +40):
// +16 and +20 the objects A and B, +0/+8 the previous and next in A's list,
// +4/+12 in B's (each object's list from its head +264), +28 0.0.
// CollisionWorld::HasPair (sub_820B8C20): whether a pair holds a and b
// (either order).
bool CollisionWorldHasPair(uint8_t* base, uint32_t a, uint32_t b);
// CollisionWorld::LinkPair (sub_820BC068): the pair set up for a and b and
// put last in both lists. Returns the original's r3 (b when its list was
// empty, else 0).
uint32_t CollisionWorldLinkPair(uint8_t* base, uint32_t pair, uint32_t a, uint32_t b);
// CollisionWorld::AddPairs (sub_820B94B0): a pair for each other active
// object (the list from the world's +20, by +228) with a bit of `groups` in
// its +244, unless both have the same nonzero +268 or the pair exists.
// Returns the original's r3 (the object when nothing was tested).
uint32_t CollisionWorldAddPairs(uint8_t* base, uint32_t object, uint32_t groups);
// CollisionWorld::UnlinkPair (sub_820BBC40): the pair out of both lists.
// Returns the original's r3.
uint32_t CollisionWorldUnlinkPair(uint8_t* base, uint32_t pair);
// CollisionWorld::MovePair (sub_820BBD30): the neighbors and heads
// repointed to `slot`, the 32 bytes copied there (memcpy natively: its
// caller checks first that the copy can be).
void CollisionWorldMovePair(uint8_t* base, uint32_t pair, uint32_t slot);
// CollisionWorld::RemovePair (sub_820B8A88): pair `index` unlinked, the
// last moved into its slot, the count down. False, with nothing done, where
// the move's memcpy can't be done natively (the physical views, or ranges
// that may share memory): the original runs there. `r3`: the original's.
bool CollisionWorldRemovePair(uint8_t* base, uint32_t index, uint32_t* r3);
// CollisionWorld::Reset (sub_820B85E8): no active objects, all on the free
// list (+24, linked by +228 and +232), no pairs or contacts, +56 = 0, +60 =
// 32; then ResetSurfaces. Returns the original's r3 (0x826C2E30).
uint32_t CollisionWorldReset(uint8_t* base);

// CollisionWorld::RemovePairsOf (sub_820B8BA8): every pair with the object
// removed (RemovePair). CollisionWorld::RemovePairsByGroups (sub_820B8AF8):
// each pair whose other object's +244 has a bit of `groups` removed, unless
// the object's +244 still meets the other's +248 (on B's side, the other's
// +248 the object's +244). False where a RemovePair couldn't be native:
// the pairs before it are removed, and the original run from the start then
// does the same as it would have (the pairs before that index keep what it
// tests, which removals don't change). `r3`: the original's.
bool CollisionWorldRemovePairsOf(uint8_t* base, uint32_t object, uint32_t* r3);
bool CollisionWorldRemovePairsByGroups(uint8_t* base, uint32_t object, uint32_t groups,
                                       uint32_t* r3);
// CollisionWorld::RemoveObject (sub_820B9448): the object from the active
// list (+20) to the free list (+24), +28 down, its pairs removed
// (RemovePairsOf; false as it, with the rest done: the original
// RemovePairsOf then).
bool CollisionWorldRemoveObject(uint8_t* base, uint32_t object, uint32_t* r3);
// CollisionObject::AddGroups (sub_820B7CC8): the bits not in +248 set
// there, and with any in the low 24 bits, AddPairs with them. Returns the
// original's r3.
uint32_t CollisionObjectAddGroups(uint8_t* base, uint32_t object, uint32_t bits);
// CollisionObject::RemoveGroups (sub_820B7CF0): the bits of `bits` in +248
// cleared, and with any in the low 24 bits, RemovePairsByGroups with them
// (`removed_groups`; false as it).
bool CollisionObjectRemoveGroups(uint8_t* base, uint32_t object, uint32_t bits,
                                 uint32_t* removed_groups, uint32_t* r3);

// --- The car data and the vehicle's constructor (src/car_physics/car_data.cpp) ---

// CarData::SetDefaults (sub_820AECB8): the car data's defaults (its row);
// `frame`: the original's 192 bytes (two vectors built at +80 and +96, two
// at +112 and +128). Returns the original's r3 (+1776, its last Vec4::Copy's).
uint32_t CarDataSetDefaults(uint8_t* base, uint32_t data, uint32_t frame);
// CarData::CarData (sub_820AEF80): the w of its vectors 0.0, then
// SetDefaults (its frame below `frame`, the original's 96 bytes).
void CarDataConstruct(uint8_t* base, uint32_t data, uint32_t frame);
// sub_820AEE98 (the name ours: CarData::FillAxle): the axle record (+336
// when the byte `is_front` is set, else +608) from the car file's record at
// `source`.
void CarDataFillAxle(uint8_t* base, uint32_t data, uint32_t source, uint32_t is_front);
// CarData::ConvertUnits (sub_820AF018): the file's units converted. Returns
// what the original leaves in f1 (+812's new value).
double CarDataConvertUnits(uint8_t* base, uint32_t data);
// Vehicle::Vehicle (sub_820B42C8): the w of its vectors 0.0, +3260 to
// +3372 (every 16) 1.0, +336, +340, +344 = 0, Vehicle::Reset (its frame
// below `frame`, the original's 112 bytes; `f1` what it leaves), byte
// +2624 = 0.
void VehicleConstruct(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1);

}  // namespace car_physics
