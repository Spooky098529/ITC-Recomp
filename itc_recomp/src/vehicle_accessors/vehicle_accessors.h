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

// sub_820C0140: the vehicle's float +2224.
float VehicleFloat2224(uint8_t* base, uint32_t vehicle);
// Vehicle::Speed (sub_820C0148): the float at +48 (m/s).
float VehicleSpeed(uint8_t* base, uint32_t vehicle);
// Vehicle::GetDistancePerFrame (sub_820C0150): +48 times the float at
// 0x820029EC (1/30).
float VehicleDistancePerFrame(uint8_t* base, uint32_t vehicle);
// Vehicle::Identity (sub_820C0168): vehicle + 5,232.
uint32_t VehicleIdentity(uint32_t vehicle);
// sub_820C0450: when the vehicle's +344 object is set, its halfword +114 =
// value (Object344SetHalf114); returns the object (0 when none).
uint32_t VehicleSetObjectHalf114(uint8_t* base, uint32_t vehicle, uint32_t value);
// Quaternion::ToMatrix3 (sub_820C0490): the rotation matrix's three rows
// (+0-+8, +16-+24, +32-+40; the rows' w left), scaled by the float at
// 0x820021E8 over the quaternion's squared length (vmsum4fp128: dpps with
// flush-to-zero on, on the 16 bytes at the quaternion rounded down to 16).
void QuaternionToMatrix3(uint8_t* base, uint32_t out, uint32_t quaternion);
// Quaternion::ToMatrix (sub_820C0550): ToMatrix3, then the fourth row =
// (0, 0, 0, 1).
void QuaternionToMatrix(uint8_t* base, uint32_t out, uint32_t quaternion);
// sub_820C0590: out +0 = speed x 3.6 (the float at 0x820029E0), out +4 and
// +6 = the vehicle's halfwords +5236 and +5238.
void VehicleSpeedRecord(uint8_t* base, uint32_t out, uint32_t vehicle);

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

// --- The collision object (the vehicle's +340) and the collision world ---

// CollisionObject::Link (sub_820B7BF0): the object first in the list at
// `head` (+228 next, +232 previous).
void CollisionObjectLink(uint8_t* base, uint32_t object, uint32_t head);
// CollisionObject::Unlink (sub_820B7C18): the object out of the list.
void CollisionObjectUnlink(uint8_t* base, uint32_t object, uint32_t head);
// CollisionObject::FirstContact (sub_820B7C90): the first contact in its
// list (+240, linked by +68) whose flags (+4) have a bit of `mask`, or 0.
uint32_t CollisionObjectFirstContact(uint8_t* base, uint32_t object, uint32_t mask);
// sub_820B7CB8 and sub_820B7CC0: the word +264, and its setter.
uint32_t CollisionObjectWord264(uint8_t* base, uint32_t object);
void CollisionObjectSetWord264(uint8_t* base, uint32_t object, uint32_t value);
// sub_820B7D18: clears `bits` in +252; sub_820B7D28 and sub_820B7D38 set
// and clear them in +256; sub_820B7D48 and sub_820B7D58 in +260.
void CollisionObjectClearBits252(uint8_t* base, uint32_t object, uint32_t bits);
void CollisionObjectSetBits256(uint8_t* base, uint32_t object, uint32_t bits);
void CollisionObjectClearBits256(uint8_t* base, uint32_t object, uint32_t bits);
void CollisionObjectSetBits260(uint8_t* base, uint32_t object, uint32_t bits);
void CollisionObjectClearBits260(uint8_t* base, uint32_t object, uint32_t bits);
// sub_820B7D68: +208 = the vector (Vec4::Copy).
void CollisionObjectSetSize(uint8_t* base, uint32_t object, uint32_t vector);
// sub_820B7D70: +224 = value (rounded to single).
void CollisionObjectSetFloat224(uint8_t* base, uint32_t object, double value);
// sub_820B7D78: copies a 68-byte record (three vectors, then +48 to +64).
void CollisionRecordCopy(uint8_t* base, uint32_t out, uint32_t record);
// sub_820B8768 and sub_820B8778: the value to 0x826C2D78 and 0x826C2D7C.
void CollisionSetGlobal78(uint8_t* base, uint32_t value);
void CollisionSetGlobal7C(uint8_t* base, uint32_t value);
// CollisionWorld::SetCallback (sub_820B8788): the world's (+[0x826C2D88])
// +8 = value.
void CollisionWorldSetCallback(uint8_t* base, uint32_t value);
// CollisionWorld::NewContact (sub_820B8A48): the next of the world's
// 80-byte records (+48 the array, +52 used, +44 the capacity), counted, or
// 0 when full.
uint32_t CollisionWorldNewContact(uint8_t* base);
// sub_820B8C90, sub_820B8CA0 and sub_820B8CB0: the world's words +60, +64
// and +68.
uint32_t CollisionWorldWord60(uint8_t* base);
uint32_t CollisionWorldWord64(uint8_t* base);
uint32_t CollisionWorldWord68(uint8_t* base);

// --- The vehicle's +344 object ---

// sub_820B9658, sub_820B9660, sub_820B9670 and sub_820B9678: +20, +16, +28
// and +24 = value (rounded to single); sub_820B9668: the float at +28.
void Object344SetFloat20(uint8_t* base, uint32_t object, double value);
void Object344SetFloat16(uint8_t* base, uint32_t object, double value);
float Object344Float28(uint8_t* base, uint32_t object);
void Object344SetFloat28(uint8_t* base, uint32_t object, double value);
void Object344SetFloat24(uint8_t* base, uint32_t object, double value);
// sub_820B99B0 and sub_820B99C0: the halfwords +112 and +114;
// sub_820B99B8 and sub_820B99C8 their setters (the value's low 16 bits).
uint32_t ObjectHalf112(uint8_t* base, uint32_t object);
void ObjectSetHalf112(uint8_t* base, uint32_t object, uint32_t value);
uint32_t ObjectHalf114(uint8_t* base, uint32_t object);
void ObjectSetHalf114(uint8_t* base, uint32_t object, uint32_t value);

// --- Contacts (the collision world's 80-byte records) ---

// Contact::Object (sub_820B8CC0): the contact's word +0 when the top byte
// of its flags (+4) is 0, else 0.
uint32_t ContactObject(uint8_t* base, uint32_t contact);
// Contact::Position (sub_820B8D00): contact + 16.
uint32_t ContactPosition(uint32_t contact);
// Contact::Strength (sub_820B8D08): the float at +64.
float ContactStrength(uint8_t* base, uint32_t contact);
// sub_820B8CE0: the contact's word +0 when the top byte of its flags is
// nonzero, else 0 (Contact::Object's other case).
uint32_t ContactOtherObject(uint8_t* base, uint32_t contact);
// Contact::Next (sub_820B8D10): the next contact (+68 on) whose flags have
// a bit of `mask`, or 0.
uint32_t ContactNext(uint8_t* base, uint32_t contact, uint32_t mask);

// --- The race data's racer records and stages ---

// Racers::IndexOf (sub_820FF9F8): the index (0-92) of the first 3,936-byte
// record from `records` whose word +3,912 is `id`, or -1.
int32_t RacersIndexOf(uint8_t* base, uint32_t records, uint32_t id);

// Stages::HasRacer (sub_820FFCF8): 1 when one of stage `stage`'s (the
// current, +804, for -1) four slots (+44 + 8i, until a -1) is `racer`.
uint32_t StagesHasRacer(uint8_t* base, uint32_t stages, uint32_t racer, uint32_t stage);

// sub_820FF918 and sub_820FF938: a nonzero value to the race data's
// ([0x826C5B2C]) +0x59618 or +0x5961C; sub_820FF968: the value to its +32.
void RaceDataSetWord59618(uint8_t* base, uint32_t value);
void RaceDataSetWord5961C(uint8_t* base, uint32_t value);
void RaceDataSetWord32(uint8_t* base, uint32_t value);
// Stages::HasNext (sub_820FFA30): 1 when the current stage (+804) is
// before the last (+800 the count, less 1; signed).
uint32_t StagesHasNext(uint8_t* base, uint32_t stages);
// Stages::SetType (sub_820FFA50): nothing for type 4; with stage -1 a stage
// is added (its +0 = type, the count +800 up), else that stage's +0 = type.
void StagesSetType(uint8_t* base, uint32_t stages, uint32_t type, uint32_t stage);
// sub_820FFAB0: +808 = value; sub_820FFC68: +808.
void StagesSetWord808(uint8_t* base, uint32_t stages, uint32_t value);
uint32_t StagesWord808(uint8_t* base, uint32_t stages);
// Stages::Type (sub_820FFBE8), sub_820FFC08 and sub_820FFC70: the stage's
// (80 bytes; -1 for the current, +804) words +0, +4 and +76; sub_820FFC30:
// 1 when its type is 2 or 3.
uint32_t StagesType(uint8_t* base, uint32_t stages, uint32_t stage);
uint32_t StagesWord4(uint8_t* base, uint32_t stages, uint32_t stage);
uint32_t StagesIsType2Or3(uint8_t* base, uint32_t stages, uint32_t stage);
uint32_t StagesWord76(uint8_t* base, uint32_t stages, uint32_t stage);
// Stages::Racer (sub_820FFC98) and sub_820FFCC8: the stage's words +44 and
// +48 + 8 x slot.
uint32_t StagesRacer(uint8_t* base, uint32_t stages, uint32_t slot, uint32_t stage);
uint32_t StagesWord48(uint8_t* base, uint32_t stages, uint32_t slot, uint32_t stage);
// sub_820FFD48: out = the stage's 16 bytes at +24.
void StagesCopy24(uint8_t* base, uint32_t out, uint32_t stages, uint32_t stage);
// Stages::Copy (sub_820FFD88): out = the stage's 80 bytes (memcpy). False,
// with nothing copied, where the library's memcpy must run (its copy
// shares memory, or the destination is in the physical views, which the
// renderer's mirrors track through memcpy's override).
bool StagesCopy(uint8_t* base, uint32_t out, uint32_t stages, uint32_t stage);
// sub_820FFDD8: adds an entry to a list of 972-byte entries (the count at
// +0): its +4 = a, +8 = b.
void EntriesAdd(uint8_t* base, uint32_t list, uint32_t a, uint32_t b);
// RacerRecords::Add (sub_820FFE08): the 3,936-byte record copied to the end
// of the array (the count at +0x49FE0) and counted; returns where it went.
// False as Stages::Copy.
bool RacerRecordsAdd(uint8_t* base, uint32_t records, uint32_t record, uint32_t* added);
// RacerRecords::At (sub_820FFE58): record n, or the last for -1 (0 when
// there are none).
uint32_t RacerRecordsAt(uint8_t* base, uint32_t records, uint32_t n);
// sub_820FFE98: out = the two halfwords at `pair`.
void PairCopy(uint8_t* base, uint32_t out, uint32_t pair);
// sub_820FFEB0: out = the pair before the first of five whose first
// halfword is 0xFFFF, or the fifth.
void PairsLast(uint8_t* base, uint32_t out, uint32_t pairs);
// sub_820FFF10: out = pair i for i below 5 (signed), else (-1, -1).
void PairsAt(uint8_t* base, uint32_t out, uint32_t pairs, uint32_t i);

// --- The scene's pools ---

// List::Init (sub_82593FB0): +0 (the first node) and +8 (the count) = 0.
void ListInit(uint8_t* base, uint32_t list);
// List::At (sub_82593FC0): node n (from +0, linked by each node's +8); 0
// for an empty list, and the first node when n is past the end.
uint32_t ListAt(uint8_t* base, uint32_t list, uint32_t n);
// ScenePools::ClaimA (sub_820C42C8) and ClaimB (sub_820C5AB0): the first
// free object (+16 = 0) of the scene's pool (scene + 56 + 12 x pool, or
// + 4944 for the second set; the count at +8) marked in use (+16 = 3), its
// index returned; -1 when none.
int32_t ScenePoolsClaimA(uint8_t* base, uint32_t scene, uint32_t pool);
int32_t ScenePoolsClaimB(uint8_t* base, uint32_t scene, uint32_t pool);
// ScenePools::GetA (sub_820C4420) and GetB (sub_820C5C20): the object when
// in use (+16 = 3), else 0.
uint32_t ScenePoolsGetA(uint8_t* base, uint32_t scene, uint32_t pool, uint32_t index);
uint32_t ScenePoolsGetB(uint8_t* base, uint32_t scene, uint32_t pool, uint32_t index);
// ScenePools::FindModel (sub_820C5D70): the n-th in-use object of the second
// set's pool 7 (scene + 5028) whose +32 is `kind`, or 0.
uint32_t ScenePoolsFindModel(uint8_t* base, uint32_t scene, uint32_t kind, uint32_t n);
// sub_820C4550: object + 1888, or 0 for none.
uint32_t ObjectRecord1888(uint32_t object);

// --- Save data and switches ---

// SaveData::Of (sub_82105620): [0x826C5B58 + 4 x player] (no bounds check).
uint32_t SaveDataOf(uint8_t* base, uint32_t player);

// sub_825C8388: 0 (probably a debug switch compiled to a constant).
uint32_t DebugSwitchOff();
// sub_825C8390: 1 (probably a switch like sub_825C8388).
uint32_t DebugSwitchOn();

}  // namespace vehicle_accessors
