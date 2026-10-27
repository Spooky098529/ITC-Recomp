// The vehicle code above the car physics as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/vehicle.md):
// the seven vehicles made and destroyed, a vehicle's tick, its track
// positions, its status record and the engine's power curve, and the
// queries the race and camera code make. The vehicles are guest objects
// shared with the recompiled code, so these work on guest memory, with the
// originals' loads and stores in their order and their arithmetic step for
// step (src/car_physics/float_ops.h). The originals' temporaries are on
// their stack: `frame` is the original's stack frame (its r1 after the
// stwu), and the temporaries go at the same offsets; register saves and
// back chains aren't written. Flush-to-zero off on entry (the adapters'
// job). Callees made native earlier are called as plain C++; where one
// would refuse, the function decides that first, with nothing done, and
// returns false (its adapter then runs the original). Adapters:
// src/hooks/vehicle_native.cpp, behind --native_vehicle.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace vehicle {

constexpr uint32_t kVehicles = 0x826C2E34;   // the seven vehicles' block
constexpr uint32_t kVehicleBytes = 6272;
constexpr uint32_t kVehiclesBytes = 7 * kVehicleBytes;  // 43,904
constexpr uint32_t kStepSeconds = 0x82601838;  // 1/60, in .data

// --- Leaves ---

// Matrix::FromRows (sub_820BFBB8): out's four rows = the 16 bytes at a, b,
// c, d (as words, in that order). The original's r3 is out.
void MatrixFromRows(uint8_t* base, uint32_t out, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
// TrackPosition::Copy (sub_820C2150): halfwords +0 to +6, the doublewords
// +16 to +79 and halfwords +80, +82 copied (+8 to +15 not).
void TrackPositionCopy(uint8_t* base, uint32_t out, uint32_t from);
// CameraCurve::Load (sub_820BFCE8): +0 to +356 = the 90 floats at
// 0x82600EE8 + 360 x index, +360 to +716 = 0.0 (each pair stored in turn),
// +720 = word.
void CameraCurveLoad(uint8_t* base, uint32_t object, uint32_t index, uint32_t word);

// --- The route position ---

// RoutePosition::Step (sub_82170F90): the kind (sign-extended halfword)
// into `frame` + 80, AiPath::BranchMasksFor of it, the slice (+2) += step,
// then TrackRoute::Normalize on the game's route (0x826C5B10). Returns its
// r3. `frame`: the original's 112 bytes.
uint32_t RoutePositionStep(uint8_t* base, uint32_t position, uint32_t step, uint32_t kind,
                           uint32_t frame);

// --- The vehicle's track positions and world ---

// Vehicle::UpdateTrackPositions (sub_820C1D78): the car's track position
// (+5232) point = the body's position, TrackPosition::Update; each wheel's
// (+5328 + 96 i) point = its mount point (+480 + 368 i), Update. `frame`:
// the original's 112 bytes (Update's frames below it).
void VehicleUpdateTrackPositions(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicle::LinkTrackPositions (sub_820C1D08): +2984 = vehicle + 5232; each
// wheel's +20 = its track position and the current shape's slot 20
// (BoxShape::SetCacheWord) with it. False, with nothing done, where the
// collision object's shape record or its current copy isn't a box's.
// `r3`: what the original leaves (the current copy).
bool VehicleLinkTrackPositions(uint8_t* base, uint32_t vehicle, uint32_t* r3);
// Vehicle::Refresh (sub_820C0118): Vehicle::RefreshFromBody with f1 = the
// float at 0x82601838 (`f1` gets it). False, with nothing done, where the
// world's ground callback isn't the game's (RefreshFromBody's refusal).
// `frame`: RefreshFromBody's 96 bytes (a tail call). `r3`: its r3.
bool VehicleRefresh(uint8_t* base, uint32_t vehicle, uint32_t frame, double* f1, uint32_t* r3);
// Vehicle::RemoveFromWorld (sub_820C20E0): +6148 = 0; the collision object
// (+340) out of the world (CollisionWorld::RemoveObject) and zeroed; the
// +344 shape deleted through its slot 0 (r4 = 1, a box's
// Shape::DeletingDestructor) and zeroed. False, with nothing done, where
// the pairs' removal or the shape's deletion wouldn't be native (operator
// delete over the allocation layer: the caller checks the heap). `r3`: the
// old +344 (the original's).
bool VehicleRemoveFromWorld(uint8_t* base, uint32_t vehicle, uint32_t* r3);

// --- The seven vehicles ---

// Vehicle::Construct (sub_820C30D8): Vehicle::Vehicle, CarData::CarData on
// +3376, then the w of +5248 to +5296 and of the four track positions'
// vectors = 1.0. `frame`: the original's 96 bytes.
void VehicleConstructAll(uint8_t* base, uint32_t vehicle, uint32_t frame);
// Vehicles::Create (sub_820C3BD0): 43,904 bytes from operator new, the
// seven vehicles constructed in them (when not 0), [0x826C2E34] = the
// block, then memset(block, 0, 43,904) (address 0 too, as the original).
// nullopt, with nothing done, where operator new would take its error path.
// Returns the original's r3 (memset's: the block). Only while --native_heap
// routes the game's heap to the allocation layer. `frame`: the original's
// 112 bytes.
std::optional<uint32_t> VehiclesCreate(uint8_t* base, uint32_t frame);
// Vehicles::Destroy (sub_820C3B78) with Vehicles::DestroyAll
// (sub_820C21E0): with a block, the vehicles from the last to the first:
// their track positions' empty destructors (the vector destructor
// iterator), Vehicle::DestroyPhysics; operator delete of the block; then
// [0x826C2E34] = 0. False, with nothing done, where a vehicle's
// DestroyPhysics or the block's delete wouldn't be native. Only while
// --native_heap routes the game's heap to the layer.
bool VehiclesDestroy(uint8_t* base);

// --- The vehicle's input and queries ---

// Vehicle::SteerTowards' original (sub_820B5FD8) in the caller's frame,
// with f1 to f3 and r4, r5 as given, where its plain C++ refuses.
using SteerOriginal =
    std::function<void(double target, double dt, double f3, uint32_t left, uint32_t right)>;
// Vehicle::ApplyInput (sub_820BFD30): the car input record's steering (by
// its kind, byte +17: the pad's through SteerTowards, an AI driver's
// through SetSteering), brake, throttle and handbrake (their curves), the
// reverse throttle's ramp and the nitrous' toggle (byte +2424). The
// integers converted through `frame` + 80 and + 88 as the original (its
// 176 bytes). False, with nothing done, where a callee never seen in play
// would run (a wheel's steering sub_820B6170, the digital brake and
// throttle ramps sub_820B03E0 and sub_820B0428, the shift and digital
// setters sub_820B0098, sub_820B00A8, sub_820B00B8): then the original
// runs. Leaves r3 the vehicle and f1 dt (RampReverseThrottle's).
bool VehicleApplyInput(uint8_t* base, uint32_t vehicle, uint32_t input, uint32_t frame,
                       const SteerOriginal& steer_original);
// Vehicle::SlipWheelValue (sub_820C0310): once (bit 0x1 of [0x826C2E3C]),
// the threshold [0x826C2E38] = speed x 1/3.6; for each wheel on the
// ground, its slip (the point velocity along its forward and side axes
// against the spin x radius, through `frame` + 80 to + 159 as the original:
// its 240 bytes); the +684 of the one slipping most above the threshold
// (0.0 when none). Returns it (the original's f1).
double VehicleSlipWheelValue(uint8_t* base, uint32_t vehicle, double speed, uint32_t frame);
// Racers::NearestHumanVehicle (sub_820C2248): the racers whose vehicles'
// +6148 have 0x1 or 0x80, with their squared distances to the point, in a
// table of seven (0.0, 0) pairs at `frame` + 96 (the point at + 80, the
// racers found at + 160, the differences at + 192 and + 208: its 304
// bytes), selection-sorted as the original (passes for the racers found,
// each over the racer count); returns Race::RacerVehicle of pair n's
// racer.
uint32_t RacersNearestHumanVehicle(uint8_t* base, uint32_t point, uint32_t n, uint32_t frame);

}  // namespace vehicle
