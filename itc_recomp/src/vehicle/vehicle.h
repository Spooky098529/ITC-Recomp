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

// --- The engine's power curve (1,064 bytes: 32 entries of 16 bytes, +0
// the rpm, +4 the torque, +8 the power; a second table at +512; +1024 the
// peak's index, +1028 the count, +1032 and +1036 the peak's limits, +1040
// and +1044 the rpm range, +1048 the torque's scale, +1052 the peak's rpm,
// +1056 the most power and +1060 its rpm) ---

// PowerCurve::ShiftPeak (sub_820CE1D8): the peak moved by `shift` 100 rpm
// steps (clamped to the range), the rpm axis remapped below `r1` (a leaf
// without a frame: its own stores at r1 - 576 to r1 - 544, the remapped
// table from r1 - 528, written as the original does), the second table
// looked up on it, the most power and its rpm.
void PowerCurveShiftPeak(uint8_t* base, uint32_t curve, uint32_t shift, uint32_t r1);
// PowerCurve::Build (sub_820CE430): the tables cleared, the 32 points at
// `points` (and +128 the torque's scale) into rpm, torque and power (a
// point below 0.01 from the two entries before it), the peak and its
// limits, then ShiftPeak(0). `frame`: the original's 112 bytes.
void PowerCurveBuild(uint8_t* base, uint32_t curve, uint32_t points, uint32_t frame);
// PowerCurve::FromEngine (sub_820CE7D8): +1044 = the car data's +12 (its
// bits), then Build on its +24 (a tail call: `frame` is Build's).
void PowerCurveFromEngine(uint8_t* base, uint32_t curve, uint32_t engine, uint32_t frame);
// VehicleStatus::Reset (sub_820BFC48): the status record's fields reset,
// a power curve built on its stack (`frame` + 80: the original's 1,168
// bytes) from the vehicle's car data +1268 and shifted by 0, +104 = its
// +1048. Returns the original's r3 (`frame` + 80).
uint32_t VehicleStatusReset(uint8_t* base, uint32_t status, uint32_t vehicle, uint32_t frame);

// --- The vehicle set up from a car ---

// Vehicle::SetUpSurfaceGrip (sub_820C1E00): +2496 from the car's flags;
// for each axle, the tire type's and grade's factors (the tables at
// 0x826014A0 and 0x82601560) and nine 24-byte surface records built on the
// stack (`frame` + 80, from its four tables at + 112, + 160, + 208, + 256;
// the grade through + 296: the original's 480 bytes) and copied (memcpy)
// into +5716 + 24 j (front) or +5932 + 24 j (rear), their pointers at
// +2668 + 4 j and +2796 + 4 j. Returns the original's r3 (the last
// record's address).
uint32_t VehicleSetUpSurfaceGrip(uint8_t* base, uint32_t vehicle, uint32_t car, uint32_t frame);

// A callee's original in the caller's frame (r1 = the caller's frame, the
// registers as given), where its plain C++ refuses or can't run.
using CalleeOriginal =
    std::function<void(uint32_t address, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6)>;
// Vehicle::SetUp (sub_820C3150, `reset` false) and Vehicle::ResetForCar
// (sub_820C33B8, `reset` true): the vehicle set up from the car (its row:
// Vehicle::SetupFromCar into +3376, VehicleStatus::Reset, the physics set
// up (SetUpPhysics with 1, 7) or reset (ResetPhysics with 7),
// LinkTrackPositions, SetValue2388 from the car's +76, the engine's bytes,
// SetUpSurfaceGrip, the nitrous' values from two tables built in `frame` +
// 96 and + 128 (the integer through + 80: the original's 224 bytes), the
// +344 box's halfword +112). The callees that refuse (or, for the physics,
// without `native_heap`) run their originals through `original`. `f1` gets
// what the original leaves there (SetValue2388's value); returns its r3
// (the +344 box, or 0).
uint32_t VehicleSetUp(uint8_t* base, uint32_t vehicle, uint32_t car, uint32_t frame, bool reset,
                      bool native_heap, const CalleeOriginal& original, double* f1);

// --- The vehicle's tick ---

// VehicleStatus::Update (sub_820C23E8): the status record from the car's
// track position (copied to `frame` + 208 with memcpy, its route position
// one slice on through `frame` + 80), the slice's direction (+32), how far
// along it (+68), the point on it (+48), the distance across (+64, signed
// by the cross product's y), the body's x axis (+16; probably the wrong
// way flag +8 with hysteresis against cos(pi/4) and cos(pi/3)), with
// `flag` the trip (+84, +88), with a car object the engine's temperatures
// (+92, +96) and +100, then the brakes' (+2432) and +2428 of the vehicle.
// The temporaries in `frame` (the original's 384 bytes) as the original.
// False, with nothing done, where the car's track position's copy wouldn't
// be memcpy's native one (the original runs).
bool VehicleStatusUpdate(uint8_t* base, uint32_t status, uint32_t vehicle, uint32_t car,
                         uint32_t flag, uint32_t frame);

// A guest function's original in the caller's frame, with the registers
// given (`r1` the stack pointer it runs with).
struct GuestRegisters {
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0;
};
using GuestCall = std::function<void(uint32_t address, uint32_t r1, const GuestRegisters&)>;
// Vehicle::Tick (sub_820C3020): the status record's speed (km/h) and route
// position, then twice: ApplyInput with the racer's car input record
// ([0x826C5B30 + 4 x the index's low byte]), its flags' 0x1, 0x2 and
// 0x10000 cleared, the physics step with the float at 0x82601838,
// UpdateTrackPositions; then VehicleStatus::Update with `car` and `flag`.
// The callees that refuse (ApplyInput's never-seen paths, SteerTowards',
// the physics step's: its callback not the ground's, UpdateSteering's and
// UpdateTorqueSplit's) and the status update's copy run their originals
// through `call`. `frame`: the original's 144 bytes.
void VehicleTick(uint8_t* base, uint32_t vehicle, uint32_t racer, uint32_t car, uint32_t flag,
                 uint32_t frame, const GuestCall& call);

}  // namespace vehicle
