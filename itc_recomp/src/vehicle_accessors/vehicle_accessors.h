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
