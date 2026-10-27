// The vehicle's register steps (src/vehicle/, docs/functions/vehicle.md),
// shared by the adapters (src/hooks/vehicle_native.cpp, behind --native_vehicle)
// and the harness (without the flag). Each turns flush-to-zero off first,
// leaves r3 (and f1 where the original leaves a value there) as the
// original does, and runs the original where the plain C++ refuses.

#pragma once

#include "guest.h"

namespace hooks {

// Matrix::FromRows sub_820BFBB8: r3 = out, r4 to r7 = the rows; r3 left.
void VehicleMatrixFromRows(PPCContext& ctx, uint8_t* base);
// TrackPosition::Copy sub_820C2150: r3 = out, r4 = from; r3 left.
void VehicleTrackPositionCopy(PPCContext& ctx, uint8_t* base);
// CameraCurve::Load sub_820BFCE8: r3 = the object, r4 = the index, r5 = the
// word; r3 left.
void VehicleCameraCurveLoad(PPCContext& ctx, uint8_t* base);
// RoutePosition::Step sub_82170F90: r3 = the position, r4 = the step, r5 =
// the kind; r3 = Normalize's.
void VehicleRoutePositionStep(PPCContext& ctx, uint8_t* base);
// Vehicle::UpdateTrackPositions sub_820C1D78: r3 = the vehicle (r3 left as
// TrackPosition::Update's plain C++ leaves it: not read by the caller).
void VehicleUpdateTrackPositions(PPCContext& ctx, uint8_t* base);
// Vehicle::LinkTrackPositions sub_820C1D08: r3 = the vehicle; r3 = the
// current shape copy.
void VehicleLinkTrackPositions(PPCContext& ctx, uint8_t* base);
// Vehicle::Refresh sub_820C0118: r3 = the vehicle; f1 = 1/60, r3 =
// RefreshFromBody's.
void VehicleRefresh(PPCContext& ctx, uint8_t* base);
// Vehicle::RemoveFromWorld sub_820C20E0: r3 = the vehicle; r3 = the old
// +344. Native only while --native_heap routes the heap to the layer.
void VehicleRemoveFromWorld(PPCContext& ctx, uint8_t* base);
// Vehicles::Create sub_820C3BD0 and Vehicles::Destroy sub_820C3B78: no
// arguments. Native only while --native_heap routes the heap to the layer.
void VehiclesCreate(PPCContext& ctx, uint8_t* base);
void VehiclesDestroy(PPCContext& ctx, uint8_t* base);

// Vehicle::ApplyInput sub_820BFD30: r3 = the vehicle, r4 = the input; r3
// the vehicle, f1 dt. SteerTowards' original in its frame where its plain
// C++ refuses; the whole original where a callee never seen in play would
// run.
void VehicleApplyInput(PPCContext& ctx, uint8_t* base);
// Vehicle::SlipWheelValue sub_820C0310: r3 = the vehicle, f1 = the speed;
// f1 = the value.
void VehicleSlipWheelValue(PPCContext& ctx, uint8_t* base);
// Racers::NearestHumanVehicle sub_820C2248: r3 = the point, r4 = n; r3 =
// the vehicle.
void VehicleNearestHuman(PPCContext& ctx, uint8_t* base);

// PowerCurve::ShiftPeak sub_820CE1D8 (r3 = the curve, r4 = the shift),
// PowerCurve::Build sub_820CE430 (r4 = the points), PowerCurve::FromEngine
// sub_820CE7D8 (r4 = the car data + 1268): r3 left (the curve).
void VehicleShiftPeak(PPCContext& ctx, uint8_t* base);
void VehicleBuildCurve(PPCContext& ctx, uint8_t* base);
void VehicleCurveFromEngine(PPCContext& ctx, uint8_t* base);
// VehicleStatus::Reset sub_820BFC48: r3 = the status record, r4 = the
// vehicle; r3 = its curve on the stack.
void VehicleStatusReset(PPCContext& ctx, uint8_t* base);

// Vehicle::SetUpSurfaceGrip sub_820C1E00: r3 = the vehicle, r4 = the car;
// r3 = the last record.
void VehicleSurfaceGrip(PPCContext& ctx, uint8_t* base);
// Vehicle::SetUp sub_820C3150 and Vehicle::ResetForCar sub_820C33B8: r3 =
// the vehicle, r4 = the car; r3 = the +344 box (or 0), f1 SetValue2388's.
void VehicleSetUp(PPCContext& ctx, uint8_t* base);
void VehicleResetForCar(PPCContext& ctx, uint8_t* base);

// VehicleStatus::Update sub_820C23E8: r3 = the status record, r4 = the
// vehicle, r5 = a car or 0, r6 = the flag (r3 not kept: Vehicle::Tick's
// caller doesn't read it). Vehicle::Tick sub_820C3020: r3 = the vehicle, r4
// = the racer, r5 = a car or 0, r6 = the flag.
void VehicleStatusUpdate(PPCContext& ctx, uint8_t* base);
void VehicleTick(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
