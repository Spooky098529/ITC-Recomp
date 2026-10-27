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
#include <functional>

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
// Vehicle::ApplyForces (sub_820B2CC0): the speed squared (+2440), the
// center of mass by byte +3132, the wheels' accumulators (+2544) summed in
// the ground frame into +368 and given to the body at their contact
// points, the tip check (byte +3241) setting the angular velocity, +80 and
// +84, the body's force (+2464 before +2608 is added; at rest the
// horizontal part cancelled) and torque (+2480), RigidBody::Integrate by
// +2352, and the body's +516 bits. `frame`: the original's 352 bytes (its
// callees' below). Returns what the original leaves in r3 (Integrate's).
uint32_t VehicleApplyForces(uint8_t* base, uint32_t vehicle, uint32_t frame);
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
// CollisionObject::Translate (sub_820B7B60): the body's center (+16) moved
// by the offset (RigidBody::SetCenterOfMassPosition), the shape record
// (+32) moved (CollisionShape::Translate), +304 += the offset, +284 = 0.
// False, with nothing done, where the record's current copy isn't a box
// (CollisionShape::Translate isn't native for it). `frame`: the original's
// 128 bytes (the callees' frames below it).
bool CollisionObjectTranslate(uint8_t* base, uint32_t object, uint32_t offset, uint32_t frame);
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
// The collision world's teardown (src/car_physics/collision_world.cpp),
// switched by --native_collision_world with its only caller,
// TrackCollision::Unload (src/track_collision/). Operator delete through
// the C runtime's heap over the allocation layer, so the caller calls these
// only while --native_heap routes the game's heap to the layer.
//
// CollisionObject::~CollisionObject (sub_820B7B40): +0 = the vtable
// 0x82002A94, +268 = 0, the shape record +32 destroyed
// (CollisionShape::~CollisionShape).
void CollisionObjectDestruct(uint8_t* base, uint32_t object);
// CollisionObject::DeletingDestructor (sub_820B9110, the vtable's slot 0):
// with bit 0x2 of `flags` an array (its count at r3 - 16, 320 bytes each)
// destroyed from the last and, with bit 0x1, its block (r3 - 16) freed
// (operator delete[]); else the one object, freed with operator delete.
// Returns the original's r3 (the block or the object).
uint32_t CollisionObjectDeletingDestructor(uint8_t* base, uint32_t object, uint32_t flags);
// Whether CollisionWorld::Destroy can run natively: a world, its blocks the
// heap's (or 0), its objects' slot 0 CollisionObject::DeletingDestructor
// and their shape records' copies deletable.
bool CollisionWorldDestroyable(uint8_t* base);
// CollisionWorld::Destroy (sub_820B86B8): the objects (+16) deleted through
// the first's slot 0 with r4 = 3 (or, with a count of 0, their block freed),
// the blocks +36 and +48 freed, the world freed, 0x826C2D88 = 0; each
// pointer zeroed after its block.
void CollisionWorldDestroy(uint8_t* base);
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
// CarData::Derive (sub_820AF1C0): the parameters derived from the loaded
// ones (its row): the axles' points, the four wheel set-up records (+880),
// the center of mass and the inertia, the axles' weight shares, a
// temporary rigid body (at `frame` + 352) for the effective masses, the
// suspension sub-records, +1800, +1804 and the 9 reciprocals at +1668.
// `frame`: the original's 1008 bytes (the callees' below it; temporaries
// at its offsets). Returns what the original leaves in f1 (the second
// sqrtf); r3 comes back as `frame` + 352.
double CarDataDerive(uint8_t* base, uint32_t data, uint32_t frame);
// sub_820C05C8 (the name ours: CarData::ApplySettings): the car object's
// signed setting bytes (car +72 to +94) and tuning (+1700, +1704, +1756)
// applied to the car data (its row), with the spec's block 2 (`spec`, car
// +1888) for the ride heights. `frame`: the original's 192 bytes (each
// setting byte converted through its +80, the tuning's table at +80 to +92;
// pow's x and y at +16 and +24 as the recompiled pow stores them). Returns
// what the original leaves in f1 (its last pow); r3 isn't kept (pow's).
double CarDataApplySettings(uint8_t* base, uint32_t data, uint32_t car, uint32_t spec,
                            uint32_t frame);
// Whether Vehicle::ApplyTuning's memcpy (36 bytes of the car's aero parts,
// car +3804 or +3840, to `frame` + 208) is copied natively
// (c_runtime::Memcpy's ranges, as the C runtime's other native callers
// check): callers check it before writing anything, and run the original
// where it isn't.
bool VehicleApplyTuningCopyFits(uint32_t car, uint32_t frame);
// Vehicle::ApplyTuning (sub_820C0D28): the car object's tuning levels (car
// +1648 on) applied to the car data (its row), through multiplier tables
// the original builds on its stack, the cylinder count from the spec's
// block 2 (car +1888) and the aero parts (car +3636). `frame`: the
// original's 480 bytes (the tables at +96 to +244, read at the level x 4
// unchecked, the levels' std at +80, the copied aero record at +208, a
// last unused array at +96 to +112; its register saves from +256 not
// reproduced). Call only where VehicleApplyTuningCopyFits. Returns r3 as
// the original leaves it (the last of a constant's base, 0x82000000
// sign-extended, memcpy's and Object::Record1888's); f1 comes back as the
// float at 0x8200256C (0.04; `*f1`).
uint64_t VehicleApplyTuning(uint8_t* base, uint32_t data, uint32_t car, uint32_t frame,
                            double* f1);
// Vehicle::SetupFromCar (sub_820C27E8): the car loader, the car data
// filled from the car object (`car`; its copies of its spec: block 2 at
// +1888, block 1 +2240, the engine stage +2880, the tires +3084 and +3176;
// its row): the copies, the gear ratios' magnitudes, the torque curve
// (copied again for fctiwz(+1280 x [0x82002CE8]) points, counted through
// `frame` + 80), the axles (CarData::FillAxle from the tires' copies at
// `frame` + 128 and + 208), the record by +176 from the table at
// 0x8260181C (unchecked), Vehicle::ApplyTuning, CarData::ApplySettings, the
// flag +3888, the axles' positions (built at `frame` + 112 and + 96, the
// camber settings' widening by sin), CarData::ConvertUnits and
// CarData::Derive. `frame`: the original's 368 bytes (its register saves
// from +288 not reproduced); the callees' frames below it at the
// original's offsets (ApplyTuning 480, ApplySettings 192, Derive 1008
// bytes). False, with nothing written, where ApplyTuning's copy wouldn't be
// native (VehicleApplyTuningCopyFits): the original runs there. Otherwise
// `f1` gets what the original leaves (Derive's), and r3 comes back as
// `frame` - 1008 + 352 (Derive's temporary body).
bool VehicleSetupFromCar(uint8_t* base, uint32_t data, uint32_t car, uint32_t frame,
                         double* f1);
// Vehicle::Vehicle (sub_820B42C8): the w of its vectors 0.0, +3260 to
// +3372 (every 16) 1.0, +336, +340, +344 = 0, Vehicle::Reset (its frame
// below `frame`, the original's 112 bytes; `f1` what it leaves), byte
// +2624 = 0.
void VehicleConstruct(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1);

// --- The vehicle's physics objects (src/car_physics/vehicle_physics.cpp):
// its collision object in the world, the world's ground callback, its
// teardown. Those that allocate or free guest memory (operator new and
// delete through the C runtime's heap over the allocation layer) are called
// only while --native_heap routes the game's heap to the layer (the
// adapters' condition). ---

// sub_8259D5A8 (RigidBody::SetFloat48): +48 = the float at `from` (lfs then
// stfs: its bits).
void RigidBodySetFloat48(uint8_t* base, uint32_t body, uint32_t from);
// CollisionWorld::Callback (sub_820B8798): a tail call to the function at
// the collision world's +8 (`[0x826C2D88]` + 8) with r3 to r5 passed on;
// natively only for TrackCollision::GroundCallback (0x820FEAB0, the only
// one set in play; its frame from `r1`, the caller's stack pointer).
// False, with nothing done, for any other: the original makes the call.
// `r3`: what the callback returns (1).
bool CollisionWorldCallbackKnown(uint8_t* base);
bool CollisionWorldCallback(uint8_t* base, uint32_t result, uint32_t point, uint32_t position,
                            uint32_t r1, uint32_t* r3);
// Whether CollisionWorld::RemoveObject's pair removals are all native (the
// pairs' array below the physical views and in one view, so a moved pair's
// memcpy is the native one: CollisionWorldRemovePair).
bool CollisionWorldPairsMovable(uint8_t* base);
// CollisionObject::ResetTransform (sub_820B7998): +288 = 1, then
// CollisionShape::Place (the shape record +32) by the body's matrix
// (RigidBody::Matrix of +16) twice. False, with nothing done, where a Place
// wouldn't be native (the record's template or copies not boxes, or the
// lowered template's stack copy not native). `frame`: the original's 112
// bytes (Place's 608 below it). r3 comes back as the record (+32).
bool CollisionObjectResetTransform(uint8_t* base, uint32_t object, uint32_t frame);
// Whether CollisionShape::SetUp runs natively for an object's record (its
// template a box, its copies deletable, the clones' allocation not
// operator new's error path): CollisionObject::Init's and AddObject's check.
bool CollisionObjectInitFits(uint8_t* base, uint32_t object, uint32_t shape, uint32_t memory);
// CollisionObject::Init (sub_820B7A00): +16 = the body, +20 = the shape,
// +244 = the groups; CollisionShape::SetUp(+32, the shape, the body's
// matrix, `memory`) (its frame below `frame`, the original's 112 bytes);
// +248 to +260 = -1; +160, +176, +304 = the zero vector; +192, +236, +240,
// +264 to +288 = 0; +208 = (1, 1, 1, its w kept), +224 = 1.0. Only where
// CollisionObjectInitFits.
void CollisionObjectInit(uint8_t* base, uint32_t object, uint32_t body, uint32_t shape,
                         uint32_t groups, uint32_t memory, uint32_t frame);
// CollisionWorld::AddObject (sub_820B9570): the first free object (+24)
// moved to the active list (+20), +28 up, CollisionObject::Init (with the
// world's +0; its frame below `frame`, the original's 128 bytes), its pairs
// made (AddPairs with its +248). Returns the object; false, with nothing
// done, with no free object or where Init wouldn't be native.
bool CollisionWorldAddObject(uint8_t* base, uint32_t body, uint32_t shape, uint32_t groups,
                             uint32_t frame, uint32_t* object);
// Vehicle::DestroyPhysics (sub_820B0E70): the collision object (+340) back
// to the world (RemoveObject), the +344 shape deleted through its slot 0
// (Shape::DeletingDestructor, r4 = 1), the rigid body (+336) destroyed
// (the empty destructor) and freed (operator delete), each zeroed. False,
// with nothing done, where one of those wouldn't be native (the pairs'
// moves, a shape that isn't a box, a block the layer didn't give). `r3`:
// what the original leaves (the body, or with none the old +344).
bool VehicleDestroyPhysics(uint8_t* base, uint32_t vehicle, uint32_t* r3);
// Vehicle::ProbeGround (sub_820B4420): per wheel the ground below its +80
// (CollisionWorld::Callback with its track position, the wheel's word +20,
// into a result at `frame` + 176): +96 the distance plus the surface's +0
// (Vehicle::SurfaceValue), +112 the normal, +20 the next position, +176 the
// surface, +160 the contact point; the vehicle's +96 the normals' sum; the
// axles' midpoints +1904, +2032; the ground frame +96 up, +112 forward,
// +128 side, +144 to +176 (w 1.0); +92 the slip term (side over forward,
// or atan2, times the speed squared clamped), +3112 the yaw rate, the
// axles' velocities +1968, +2096, the velocity in the ground frame +384,
// +88 the roll. Only where the callback is the ground's
// (CollisionWorldCallbackKnown). `frame`: the original's 304 bytes (the
// callback's from it). `f1` gets the last dot product (+88); returns the
// original's r3 (`frame` + 160).
uint32_t VehicleProbeGround(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1);
// Vehicle::Unflip (sub_820AFE90): with +48 below 2.0 and +16 . +96 below
// [0x820029C8], the ground under the body's center (the callback, r5 =
// +2984, into `frame` + 288), a matrix from its normal (row 1), row 2 the
// normal x +2528 normalized and row 0 row 2 x row 2 normalized (a zero
// vector: the game's), its quaternion; the body's center of mass set from
// the car data's +96, its position lowered by the distance, its
// orientation the quaternion; byte +3132 = 0. False, with nothing done,
// where the callback isn't the ground's (CollisionWorldCallbackKnown).
// `frame`: the original's 384 bytes. `r3`: what the original leaves there
// (the vehicle, +16, or the body's +336).
bool VehicleUnflip(uint8_t* base, uint32_t vehicle, uint32_t frame, uint32_t* r3);
// Vehicle::RefreshFromBody (sub_820B62E8): SyncFromBody, ProbeGround,
// UpdateWheelAxes (their frames below `frame`, the original's 96 bytes).
// Only where the callback is the ground's. Returns the original's r3
// (UpdateWheelAxes').
uint32_t VehicleRefreshFromBody(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::PhysicsStep (sub_820B6498): the car's physics step (its row:
// BeginStep with `dt`, the body's state, the ground, the suspension, the
// collision object's +144 = -min([0x82001BDC], the wheels' +0), the
// controls and the engine, the +2368 substeps of the tires and the
// drivetrain, the forces applied, the state and ground again, the wear and
// Unflip; +2520 = +2516, bytes +2444, +2445 = 0), each callee's plain C++
// with the vehicle (the original passes it on in r3, which they leave as
// it is) and its frame below `frame` (the original's 112 bytes) as the
// original's. Where UpdateSteering or UpdateTorqueSplit would refuse (tan's
// error path, mode 2: never in play), `original(address)` runs that
// function's original instead (the adapter's guest call, with r3 the
// vehicle and r1 `frame`), as the recompiled step's call to its adapter
// would. Only where the callback is the ground's. Returns the original's
// r3 (Unflip's).
uint32_t VehiclePhysicsStep(uint8_t* base, uint32_t vehicle, double dt, uint32_t frame,
                            const std::function<void(uint32_t address)>& original);
// Vehicle::SetUpPhysics (sub_820B5670) and Vehicle::ResetPhysics
// (sub_820B5B08): the vehicle's physics set up from its car data (their
// rows): Vehicle::Reset, the wheels cleared, the wheels' pointers, the
// mass; a new 528-byte rigid body (SetUpPhysics; ResetPhysics keeps +336)
// reset and given its mass, inertia, center of mass, +48, +112 and gravity;
// ResetPhysics' old collision object back to the world and old shape
// deleted (slot 0); a new 496-byte box (+344) from the car data's corners
// with its four values; a collision object from the world (AddObject) with
// its size, +224, groups (`groups` | 0x07000000) and masks; the fields and
// records cleared (ResetPhysics' +352 and +384 the zero vector too), the
// wheels' +228, the gauges +2968 to +2980, byte +2249 (`byte`;
// ResetPhysics' 1), SetUpGearing, SyncFromBody, ProbeGround, the drive and
// the torque split, the gear values with one gear. Operator new and delete
// through the C runtime's heap over the allocation layer (the adapters'
// condition: --native_heap). False, with nothing done, where a step
// wouldn't be native (the callback not the ground's, no process heap, no
// free collision object or its record's copies not deletable, the pairs'
// moves, the old shape not a box the layer gave). `frame`: the original's
// 208 bytes (the callees' below). `f1` gets ProbeGround's; `r3` what the
// original leaves (`frame` - 144).
bool VehicleSetUpPhysics(uint8_t* base, uint32_t vehicle, uint32_t data, uint32_t byte,
                         uint32_t groups, uint32_t frame, double* f1, uint32_t* r3);
bool VehicleResetPhysics(uint8_t* base, uint32_t vehicle, uint32_t data, uint32_t groups,
                         uint32_t frame, double* f1, uint32_t* r3);

// --- The collision world's set-up (src/car_physics/collision_world.cpp),
// over the C runtime's heap on the allocation layer (the adapter's
// condition: --native_heap) ---

// CollisionWorld::Create (sub_820B8D30) with no buffer (the only case in
// play; the buffer's paths stay the original's): a 72-byte world into
// 0x826C2D88 (+0 = 0, +56 = 0, +60 = 32, +12 = `objects`), +16 = that many
// 320-byte collision objects (operator new[] with a count header, each
// CollisionObject::CollisionObject; 0 when the allocation gives 0), +20 =
// 0, +24 the free list of all of them (+228 next, +232 previous), +28 = 0,
// +32 = `pairs`, +36 = that many 32-byte pairs, +40 = 0, +64 = 64 80-byte
// query records (their w zeroed), +68 = +64 + 80 x +60, +44 = `contacts`,
// +48 = that many 80-byte contacts (their w zeroed), +52 = 0, +4 = 1/30;
// CollisionWorld::ResetSurfaces; the default callbacks (0x826C2D78 =
// 0x825C8388, 0x826C2D7C = 0x820BC828, +8 = 0x820BC4D0). False, with
// nothing done, with no process heap or a size operator new[] would refuse
// (its error path). Returns the original's r3 (ResetSurfaces').
bool CollisionWorldCreate(uint8_t* base, uint32_t objects, uint32_t pairs, uint32_t contacts,
                          uint32_t* r3);
// CollisionWorld::SetQueryCapacity (sub_820B9198) with no buffer (+0 0):
// +60 = n, the old records (+64) freed (operator delete[]), +64 = 2n
// 80-byte records (their w zeroed; 0 when the allocation gives 0), +68 =
// +64 + 80n. False, with nothing done, where the old block isn't the
// layer's, the world has a buffer, or the size would take operator new[]'s
// error path. `r3`: what the original leaves (the new block).
bool CollisionWorldSetQueryCapacity(uint8_t* base, uint32_t n, uint32_t* r3);
// CollisionWorld::Setup (sub_820FF250): Create(50, 1000, 100, no buffer),
// the queries' function pointers (0x820FF1B0, 0x820FEBD0), the world's +8
// = TrackCollision::GroundCallback (0x820FEAB0), SetQueryCapacity(32) and
// the surface settings (C 0.0 for the three bits, D 0.0, 1.0, 1.0, A 0.0,
// 0.5, [0x82002968], B 0.0). False, with nothing done, where Create
// wouldn't be native. `r3`: what the original leaves (the query block).
bool CollisionWorldSetup(uint8_t* base, uint32_t* r3);

// --- The collision world's update (src/car_physics/collision_world.cpp) ---

// A guest call from the update's plain C++ (which stays without ctx): the
// function at `address` with r3 to r6, r1 and lr as given; returns the r3
// it leaves. The adapter's: the originals where a native callee refuses
// (CollisionShape::Place 0x820BB758, CollisionPair::Solve 0x820BBE30,
// CollisionObject::Respond 0x820B81C8), and the queries' targets other than
// TrackCollision::WallHits and GroundHits (called through the pointers).
using CollisionGuestCall = std::function<uint32_t(uint32_t address, uint32_t r3, uint32_t r4,
                                                  uint32_t r5, uint32_t r6, uint32_t r1,
                                                  uint32_t lr)>;
// CollisionObject::Respond (sub_820B81C8): the `count` hits merged into the
// world's +68 with a summary (CollisionRecords::Merge), the object pushed
// out by the summary (CollisionObject::Translate) when its +260 meets the
// summary's surface bits, and for each hit whose bits meet +256 (unless the
// first query's skip case: `flag`'s word set, body word +516 bits 0x1 and
// 0x2, |body +160| below [0x820021F8] and the summary's normal against body
// +144 above 0.5) the restitution and friction impulses, the surface's
// torque and a contact on +240. The 0x02000000 branch's Vec3::SubFrom
// (sub_82294330, never seen in play) as Vec3::Sub(v, v, b). False, with
// nothing done, where the record's current copy (+40) isn't a box
// (CollisionObject::Translate wouldn't be native). `frame`: the original's
// 416 bytes (pow's stores into its linkage area, +16 and +24, made). `r3`:
// what the original leaves there.
bool CollisionObjectRespond(uint8_t* base, uint32_t object, uint32_t hits, uint32_t count,
                            uint32_t flag, uint32_t frame, uint32_t* r3);
// CollisionObject::Collide (sub_820B8548): the world's query records (+64)
// filled by the queries through 0x826C2D78 (the shape record) and
// 0x826C2D7C (its current copy), r5 the object's +248; with hits, Respond
// (the flag 1, then 0, at `frame` + 80). TrackCollision::WallHits and
// GroundHits natively (whatever --native_collision_world says), other
// targets and their refusals by `guest`; Respond's refusals by `guest`.
// `frame`: the original's 128 bytes. Returns the original's r3.
uint32_t CollisionObjectCollide(uint8_t* base, uint32_t object, uint32_t frame,
                                const CollisionGuestCall& guest);
// CollisionWorld::CollideAll (sub_820B89F0): Collide for each active object
// whose +272 is 0. `frame`: the original's 96 bytes; `r3` the r3 it came
// with. Returns the original's r3.
uint32_t CollisionWorldCollideAll(uint8_t* base, uint32_t frame, uint32_t r3,
                                  const CollisionGuestCall& guest);
// CollisionWorld::SolvePairs (sub_820B87B0): the clock 0x826C2D80 += dt
// (wrapped by [0x82002AA0] with each pair's +28), 0x826C2D84 = dt; two
// passes: each active object's +304 = zero; each pair with a moved object
// (+288) or due: +28 = CollisionPair::NextCheckTime, and within the step
// the bodies' centers saved (`frame` + 80, + 96), CollisionPair::Solve
// (refusals by `guest`), and when it hits both objects Collide'd and their
// centers put back; after a hit, each object's +304 scaled by +208 and its
// body's center moved by it (the second pass only after hits). `frame`: the
// original's 208 bytes; `r3` the r3 it came with. Returns the original's r3.
uint32_t CollisionWorldSolvePairs(uint8_t* base, double dt, uint32_t frame, uint32_t r3,
                                  const CollisionGuestCall& guest);
// CollisionWorld::Update (sub_820B92C8): +52 = 0, +56 = 1; each active
// object's shape record placed by its body's matrix (CollisionShape::Place,
// refusals by `guest`) unless +284 is set, +240 = 0; SolvePairs unless dt
// is 0.0; CollideAll; each object with contacts and pushes (+192): the body's
// force += +160, its torque += +176 / +192 x +224, ApplyImpulses(1.0); then
// +288 = 0, and with pushes +160, +176 = zero, +192 = 0; +56 = 0. `frame`:
// the original's 176 bytes; `r3` the r3 it came with. Returns the
// original's r3.
uint32_t CollisionWorldUpdate(uint8_t* base, double dt, uint32_t frame, uint32_t r3,
                              const CollisionGuestCall& guest);

}  // namespace car_physics
