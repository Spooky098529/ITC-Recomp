// The car objects' makers as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/car-makers.md, the fields in
// docs/known-data.md: "garage car", "garage stage", "scene model record",
// and race_setup.h's car object). The garage cars, the car objects, the
// scene model records and the stage are guest data shared with the
// recompiled code (the front end's screens), so these work on guest memory
// with the originals' loads and stores in their order: the scalar float work
// as the translated code's doubles rounded to single (built with
// -ffp-model=strict and -ffp-contract=off, CMakeLists.txt). Each that has a
// frame takes `stack`, the guest r1 at entry, and keeps the original's
// frame: its back chain written as stwu does (not its saved registers), its
// temporaries at the original's offsets (the leaves' scratch below the stack
// pointer too), its callees run from it. Callees already ours are called as
// plain C++; the rest are guest calls (front_end_car_draw::Calls). Expect
// flush-to-zero off at entry (the adapters turn it off) and leave it off.
// Adapters: src/hooks/car_makers.cpp.

#pragma once

#include <cstdint>

#include "scene_models/apply.h"

namespace front_end_car_draw {
struct Calls;
}

namespace car_makers {

// --- The scene model record's setters (src/car_makers/record.cpp) ---

// CarModels::BlockOf (sub_8215AAD0, with its case for id 1, sub_8215AB04,
// merged): a car id's model block (a byte-indexed switch): ids 1 to 71 by
// the table, others 0.
uint32_t BlockOf(uint32_t id);
// SceneModelRecord::SetPaint (sub_8215AC70): r3 = a 4,244-byte record, r4 =
// a car object, r5 = the paint pick (-1: the car's halfword +0). Traffic
// (kind 3): +176 = the pick; else three hue, saturation, value triples at
// +32, +48, +64 (from the paint copy at car +3376, -2, or its entry the
// pick; -2 overwrites the first two from the car's paint and sets the third
// by its halfword +6); then +4040 = 1 and the wheels' colors and floats.
// The conversions' scratch doublewords at `stack` - 16, - 24, - 32 as the
// original leaves them (a leaf: `stack` is r1). Returns the record (r3).
uint32_t SetPaint(uint8_t* base, uint32_t record, uint32_t car, uint32_t pick, uint32_t stack);
// SceneModelRecord::SetWheels (sub_8215B238): r3 = a record, r4 = a car
// object: +4052, +4068 = the wheel numbers - 1, +4104, +4124 (block 2's
// +236 when set), the tires' +4080 to +4100. The empty destructor
// sub_82106540 the original calls on the car twice is left out. Its
// 112-byte frame below `stack` (the back chain). Returns block 2 (r3).
uint32_t SetWheels(uint8_t* base, uint32_t record, uint32_t car, uint32_t stack);
// SceneModelRecord::SetChoice (sub_8215B1A0): r3 = a 12-byte out, r4:r5 = a
// 12-byte value {a, b, c} in registers, stored to `stack` + 32 and + 40 (the
// caller's frame, as the original does) and read back: c 0, out +0 = 0;
// else out +0 = 1, +4 = c - 1, +8 = 3a - 3 + k (k 1 for (1, 0), 2 for (1,
// 1), 1 for (2, 1), else 0). r3 is left.
void SetChoice(uint8_t* base, uint32_t out, uint64_t r4, uint64_t r5, uint32_t stack);
// CarRecord::ResetBytes (sub_820D0900): twelve words from `at` = 0, 0, 1,
// 2, 3, 29, 36, 31, 32, 37, 34, 35 (in the original's store order).
void ResetBytes(uint8_t* base, uint32_t at);
// CarRecord::SetBytes (sub_8215B0E0): r3 = a car record, r4 = a car's 20
// bytes at +1568: CarRecord::ResetBytes(+3632), +3632 = 0, +3636 to +3676 =
// the signed bytes r4 +1 to +11, +3684 = the signed byte r4 +0. Its
// 112-byte frame below `stack`. Returns the record + 3632 (r3).
uint32_t SetBytes(uint8_t* base, uint32_t record, uint32_t bytes, uint32_t stack);

// SceneModelRecord::FromCar (sub_8215B878): r3 = a 4,244-byte record, r4 =
// a car object, r5 = the paint pick (all 64 bits, for SetPaint). Its
// 256-byte frame below `stack`: SceneModelRecord::Reset; the kind (+0) by
// the car's flags +3888 (0x81: 2, 0x2: 1, else 3, traffic, which skips the
// parts, the decals and pool 4's record); +16 = CarModels::BlockOf(car
// +12); SetWheels, SetPaint; kind 2 the cockpit (sub_820D34B8 and the car's
// words, +3876 and +3880 by car +1708 and block 2's +232, read twice, the
// six SetChoice calls from the frame, +3884 and +3888); cars the parts
// (with an aero kit, Params::GetCarParts into frame + 128, copied to frame
// + 80 (memcpy), AeroParts::Get82 into frame + 128 and the kit's 24-byte
// entry at 0x820093E0 copied to frame + 80 (memcpy)); +3704; cars the
// decals (the six entries at +96 and the forty at +288, in their eight
// runs); +3768 by the car's halfword +4; cars pool 4's record and the tire
// grades through the table {0, 1, 1, 2, 2} at frame + 80; then
// CarRecord::SetBytes(+16, car +1568). The empty destructor the original
// calls on the car is left out. Callees already ours run as plain C++
// (memcpy and AeroParts::Get82 through `calls` where theirs refuse).
// Returns SetBytes' r3 (the record + 3648).
uint32_t FromCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t record,
                 uint32_t car, uint64_t pick, uint32_t stack);

// --- The garage cars (src/car_makers/garage.cpp) ---

// GarageCar::Reset (sub_82110448): r3 = a 1,968-byte garage car:
// Car::Clear; +1888, +1892 = [0x82000A34] (0.0), +1884, +1896, +1900 = 0,
// +1964 = 1; memcpy(+1904, Defaults::Table3044(), 60) (a guest call where
// memcpy's plain C++ declines). Its 96-byte frame below `stack`. Returns
// memcpy's r3.
uint64_t GarageCarReset(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t car,
                        uint32_t stack);
// GarageCars::StaticInit (sub_825D9EF0): the two 1,972-byte entries at
// 0x82814EB8, each GarageCar::Reset and its +1968 = 0. Its 112-byte frame
// below `stack` (Reset's below it). Returns the last Reset's r3.
uint64_t GarageCarsStaticInit(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t stack);
// CarData::FromGarageCar (sub_82179A60): r4 = a garage car, r5 = a car
// data (r3 not read). Its 4,016-byte frame below `stack`: a car object at
// frame + 80 (Car::Init, memcpy of the garage car's 1,884 bytes,
// Car::Refresh, Race::AddRacer(car, 0, 0.0, 0), Car::SetupVehicle(car,
// data)), then Vehicle::SetupFromCar(data, car) again; each callee's
// original through `calls` (from the frame) where its plain C++ declines.
// Returns the last's r3; `f1` gets what it leaves.
uint64_t FromGarageCar(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t garage,
                       uint32_t data, uint32_t stack, double* f1);
// TuningScreen::SetUpCarData (sub_821DF2F8): r3 = the tuning screen: its
// selected garage car (the save data [+18904]'s list at +6120 walked
// [+0x19920] steps from the head halfword +204), then a tail call
// CarData::FromGarageCar([[0x826D83E0]], the car, the screen + 18912) from
// the same `stack`. Returns its r3; `f1` as it leaves it.
uint64_t TuningScreenSetUpCarData(uint8_t* base, const front_end_car_draw::Calls& calls,
                                  uint32_t screen, uint32_t stack, double* f1);

// --- The garage stage's makers (src/car_makers/stand.cpp) ---

// GarageStage::FitCar (sub_82178C40): r3 = the stand's holder (its +0 the
// stage s), r4 = a car object. Its 1,968-byte frame below `stack`: a car
// data at frame + 80 (CarData::CarData, Vehicle::SetupFromCar(data, car),
// its original through `calls` where its plain C++ declines), then the
// stage's car sizes from it (s +6096 to +6204, the stage reloaded from the
// holder for each, as the original; sin of the axles' angles x
// [0x82002948]). Returns SetupFromCar's r3; `f1` gets the second sin's.
uint64_t GarageStageFitCar(uint8_t* base, const front_end_car_draw::Calls& calls,
                           uint32_t holder, uint32_t car, uint32_t stack, double* f1);
// GarageStage::CopyRecord (sub_8217FC60): r3 = the holder, r4 = a record:
// SceneModelRecord::Copy(m + 32, r4) on the stage's scene model m
// (ScenePools::GetB, pool 7, s +8); where its plain C++ declines (the
// callers' copy onto itself), its adapter through `calls` (from the 96-byte
// frame below `stack`). Returns the copy's r3.
uint64_t GarageStageCopyRecord(uint8_t* base, const front_end_car_draw::Calls& calls,
                               uint32_t holder, uint32_t record, uint32_t stack);
// GarageStage::ReapplyRecord (sub_82178DF8; `decals`) and
// ReapplyRecordNoDecals (sub_82178E60): r3 = the holder. Its 96-byte frame
// below `stack`: the stage's scene model m (ScenePools::GetB, pool 7, s +8),
// then SceneModel::ApplyPaint(m) and ApplyWheelPaint(m) (their plain C++
// inside their diagnostics' wraps, `calls.poll` the load poll's calls),
// SceneModel::ApplyRecord(m) (its plain C++, its adapter through `apply`
// where it declines: the race's path), SceneModel::ApplyLightKit(m) and, for
// ReapplyRecord, SceneModel::ReloadDecals(m) (their plain C++). Returns the
// last call's r3.
uint64_t GarageStageReapplyRecord(uint8_t* base, const front_end_car_draw::Calls& calls,
                                  const scene_models::ApplyCall& apply, uint32_t holder,
                                  uint32_t stack, bool decals);
// sub_8219ED10 and sub_8219EDB0 (the stand's busy marker shown and hidden,
// probably): movie 6 (UiMovies::Find with the id at frame + 80, the
// 112-byte frame below `stack`), then its named animations through their
// overrides (`calls`): FinishNamed 86, 87, PlayNamed 86, 85, LoopNamed 85;
// or FinishNamed 86, 87, 85, PlayNamed 87. Return the last call's r3.
uint64_t ShowBusyMarker(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t stack);
uint64_t HideBusyMarker(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t stack);

}  // namespace car_makers
