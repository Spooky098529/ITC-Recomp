// The vehicle accessors as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/vehicle-accessors.md): the small getters
// and setters the race overlays and the car code read the cars and the race
// through. The objects are guest objects shared with the recompiled code
// (the physics, the car update), so these work on guest memory (level 1 of
// "The boundary rule"). Adapters: src/hooks/vehicle_accessors.cpp.

#pragma once

#include <cstdint>

namespace vehicle_accessors {

// --- The vehicles and their wheels ---
//
// Vehicles are 6,272 bytes, in the array at [0x826C2E34].

// Vehicle::Get (sub_820C0468): vehicle `index` ([0x826C2E34] + 6,272 x
// index, no bounds check), or 0 when there's no array.
uint32_t VehicleGet(uint8_t* base, uint32_t index);

// Vehicle::WheelContact (sub_820C0170): the wheel's 96-byte contact record,
// vehicle + 5,328 + 96 x wheel.
uint32_t VehicleWheelContact(uint32_t vehicle, uint32_t wheel);

// --- The rigid body (the vehicle's +336) ---

// RigidBody::Position (sub_820B67D8): body + 208.
uint32_t RigidBodyPosition(uint32_t body);
// RigidBody::Matrix (sub_820B6930): body + 288 (64 bytes).
uint32_t RigidBodyMatrix(uint32_t body);

// RigidBody::IsFixed (sub_820B6638): its byte +0.
uint32_t RigidBodyIsFixed(uint8_t* base, uint32_t body);
// RigidBody::SetGravity (sub_820B6640): +480 = the vector (Vec4::Copy),
// +496 = it times the mass (+4), all four.
void RigidBodySetGravity(uint8_t* base, uint32_t body, uint32_t vector);
// RigidBody::SetMass (sub_820B6698): +4 = mass, +8 = 1.0 / mass (0.0 when
// fixed), +496 = +480 times mass, all four.
void RigidBodySetMass(uint8_t* base, uint32_t body, double mass);
// RigidBody::Mass (sub_820B6708): the float at +4.
float RigidBodyMass(uint8_t* base, uint32_t body);
// RigidBody::SetCenterOfMass (sub_820B67E0): +192 = offset; the matrix at
// +224's fourth row = +208 + the offset rotated by it, and +16 = that with
// w 0.0.
void RigidBodySetCenterOfMass(uint8_t* base, uint32_t body, uint32_t offset);
// sub_820B6880, RigidBody::Velocity (sub_820B6888), sub_820B6890,
// sub_820B6898, sub_820B6928 and RigidBody::AngularVelocity (sub_820B6948):
// body + 80, 32, 144, 160, 224 and 96.
uint32_t RigidBodyField80(uint32_t body);
uint32_t RigidBodyVelocity(uint32_t body);
uint32_t RigidBodyField144(uint32_t body);
uint32_t RigidBodyField160(uint32_t body);
uint32_t RigidBodyOrientation(uint32_t body);
uint32_t RigidBodyAngularVelocity(uint32_t body);
// sub_820B68A0: 0.0 when fixed, else the float at +48.
float RigidBodyFloat48(uint8_t* base, uint32_t body);
// RigidBody::PointVelocity (sub_820B68C0): out = +32 + +96 x (point -
// +16), w 1.0.
void RigidBodyPointVelocity(uint8_t* base, uint32_t out, uint32_t body, uint32_t point);
// sub_820B6950: +112 = value (rounded to single).
void RigidBodySetFloat112(uint8_t* base, uint32_t body, double value);
// RigidBody::AddForce (sub_820B6958) and AddTorque (sub_820B6960): +64 or
// +128 += the vector (Vec3::AddTo).
void RigidBodyAddForce(uint8_t* base, uint32_t body, uint32_t vector);
void RigidBodyAddTorque(uint8_t* base, uint32_t body, uint32_t vector);
// RigidBody::AddForceAt (sub_820B6968): +128 += (point - +16) x force,
// then +64 += force; AddTorqueAt (sub_820B69D8) the first only.
void RigidBodyAddForceAt(uint8_t* base, uint32_t body, uint32_t force, uint32_t point);
void RigidBodyAddTorqueAt(uint8_t* base, uint32_t body, uint32_t force, uint32_t point);

// Vehicle::WheelSlip (sub_820C0180): 0.0 when the wheel isn't on the
// ground; else the length of the contact's slip (the body's velocity at the
// wheel along the record's +144 and +128, less the wheel's spin times
// radius on the first), or 0.0 unless it's above threshold x the float at
// 0x82002BD8.
float VehicleWheelSlip(uint8_t* base, uint32_t vehicle, uint32_t wheel, double threshold);
// Vehicle::MaxWheelSlip (sub_820C0290): the largest WheelSlip of wheels 0-3
// (from 0.0; a NaN, once met, is kept).
float VehicleMaxWheelSlip(uint8_t* base, uint32_t vehicle, double threshold);

// --- Contacts (the collision world's 80-byte records) ---

// Contact::Object (sub_820B8CC0): the contact's word +0 when the top byte
// of its flags (+4) is 0, else 0.
uint32_t ContactObject(uint8_t* base, uint32_t contact);
// Contact::Position (sub_820B8D00): contact + 16.
uint32_t ContactPosition(uint32_t contact);
// Contact::Strength (sub_820B8D08): the float at +64.
float ContactStrength(uint8_t* base, uint32_t contact);

// --- The race data's racer records and stages ---

// Racers::IndexOf (sub_820FF9F8): the index (0-92) of the first 3,936-byte
// record from `records` whose word +3,912 is `id`, or -1.
int32_t RacersIndexOf(uint8_t* base, uint32_t records, uint32_t id);

// Stages::HasRacer (sub_820FFCF8): 1 when one of stage `stage`'s (the
// current, +804, for -1) four slots (+44 + 8i, until a -1) is `racer`.
uint32_t StagesHasRacer(uint8_t* base, uint32_t stages, uint32_t racer, uint32_t stage);

// --- Save data and switches ---

// SaveData::Of (sub_82105620): [0x826C5B58 + 4 x player] (no bounds check).
uint32_t SaveDataOf(uint8_t* base, uint32_t player);

// sub_825C8388: 0 (probably a debug switch compiled to a constant).
uint32_t DebugSwitchOff();

}  // namespace vehicle_accessors
